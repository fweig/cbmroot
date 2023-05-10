/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "UnpackStsXpu.h"

#include <cassert>
#include <utility>
#include <vector>

#include <cmath>

#include "StsXyterMessage.h"

using std::unique_ptr;
using std::vector;

XPU_BLOCK_SIZE_1D(cbm::algo::UnpackStsXpu::Unpack, 32);

namespace cbm::algo
{

  // ----   Algorithm execution   ---------------------------------------------
  UnpackStsXpu::resultType UnpackStsXpu::operator()(const fles::Timeslice* ts, StsReadoutConfig& config)
  {
    // --- Output data
    resultType result = {};
    std::cout << "Called UnpackStsXpu::operator()()." << std::endl;

    // ---  Init local storage vectors
    std::vector<stsxyter::Message> messages;  //storage of all messages
    std::vector<uint64_t> messCount;          //storage of number of messages per MS
    std::vector<uint64_t> messOffset;         //storage of MS offset in message buffer
    std::vector<uint64_t> msIdx;              //storage of MS idx / start time
    std::vector<uint32_t> compIdx;            //storage of comp idx for MS

    auto equipIdsSts         = config.GetEquipmentIds();
    const size_t numStsComps = equipIdsSts.size();

    // --- Loop over components in unpacker config
    for (size_t comp = 0; comp < numStsComps; comp++) {
      auto equip = equipIdsSts[comp];
      // --- Loop over components in timeslice
      for (uint64_t tsComp = 0; tsComp < ts->num_components(); tsComp++) {

        // --- Skip if TS component is not from STS (equipment IDs are non-unique across systems)
        auto systemId = static_cast<fles::SubsystemIdentifier>(ts->descriptor(tsComp, 0).sys_id);
        if (systemId != fles::SubsystemIdentifier::STS) continue;

        if (equip == ts->descriptor(tsComp, 0).eq_id) {
          const uint64_t numMsInComp = ts->num_microslices(tsComp);
          for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
            const auto msDescr = ts->descriptor(tsComp, mslice);
            if (msDescr.size % sizeof(stsxyter::Message) != 0) {
              result.second.fNumErrInvalidMsSize++;
              continue;
            }
            const uint32_t numMessages = msDescr.size / sizeof(stsxyter::Message);
            if (numMessages < 2) {
              result.second.fNumErrInvalidMsSize++;
              continue;
            }
            msIdx.push_back(msDescr.idx);
            compIdx.push_back(comp);
            messCount.push_back(numMessages);
            messOffset.push_back(messages.size());
            const auto msContent = ts->content(tsComp, mslice);
            auto mess            = reinterpret_cast<const stsxyter::Message*>(msContent);
            messages.insert(messages.end(), mess, mess + numMessages);
          }
        }
      }
    }
    // --- Total number of microslices
    const uint64_t numMs = messCount.size();

    // --- Store SMX messages to be unpacked (TS content)
    xpu::d_buffer<stsxyter::Message> tsContent {messages.size()};
    xpu::copy(tsContent.d(), messages.data(), messages.size());

    // --- Store auxiliary host-device buffers
    xpu::hd_buffer<uint64_t> msMessCount {numMs};  //modified by kernel, stores numDigis after execution
    xpu::copy(msMessCount.d(), messCount.data(), messCount.size());

    xpu::hd_buffer<uint64_t> msMessOffset {numMs};  //unchanged but needed on device and host
    std::copy(messOffset.begin(), messOffset.end(), msMessOffset.h());
    xpu::copy(msMessOffset, xpu::host_to_device);

    // --- Store auxiliary device buffers
    xpu::d_buffer<uint64_t> msStartTime {numMs};
    xpu::d_buffer<uint32_t> msCompIdx {numMs};
    xpu::copy(msStartTime.d(), msIdx.data(), msIdx.size());
    xpu::copy(msCompIdx.d(), compIdx.data(), compIdx.size());

    // --- Create output buffer with maximum possible size
    xpu::hd_buffer<CbmStsDigi> digisOut {messages.size()};

    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    const uint64_t epochLengthInNs = fkEpochLength * fkClockCycleNom / fkClockCycleDen;
    const uint64_t currentTsTime   = ts->start_time() / epochLengthInNs;

    // --- Do unpacking for each microslice
    xpu::run_kernel<Unpack>(xpu::grid::n_threads(numMs), fParams.d(), fElinkParams.d(), tsContent.d(), msMessCount.d(),
                            msMessOffset.d(), msStartTime.d(), msCompIdx.d(), digisOut.d(), currentTsTime, numMs);

    // --- Copy results back to host (only two buffers are modified on device)
    xpu::copy(msMessCount, xpu::device_to_host);
    xpu::copy(digisOut, xpu::device_to_host);

    // --- Store digis  TO DO: make Kernel for this, needs a way to sum arrays in XPU first
    for (uint64_t i = 0; i < numMs; i++) {
      uint64_t offset   = msMessOffset.h()[i];
      uint64_t numDigis = msMessCount.h()[i];
      for (uint64_t j = 0; j < numDigis; j++) {
        result.first.push_back(digisOut.h()[offset + j]);
      }
    }

    return result;
  }

  XPU_KERNEL(UnpackStsXpu::Unpack, xpu::no_smem, UnpackStsXpuPar* params, UnpackStsXpuElinkPar* elinkParams,
             stsxyter::Message* content, uint64_t* msMessCount, uint64_t* msMessOffset, uint64_t* msStartTime,
             uint32_t* msCompIdx, CbmStsDigi* digisOut, const uint64_t currentTsTime, int NElems)
  {
    int id = xpu::block_idx::x() * xpu::block_dim::x() + xpu::thread_idx::x();
    if (id >= NElems || msMessCount[id] < 2) return;  // exit if out of bounds or too few messages

    UnpackStsXpuMonitorData monitor;  //Monitor data, currently not stored. TO DO: Implement!

    // --- Get message count and offset for this MS
    const uint32_t numMessages = msMessCount[id];
    const uint32_t messOffset  = msMessOffset[id];

    // --- Get starting position of this MS in message buffer
    stsxyter::Message* message = &content[messOffset];

    // --- Get starting position of this MS in digi buffer
    CbmStsDigi* digis = &digisOut[messOffset];

    // --- Get component index and unpack parameters of this MS
    const uint32_t comp              = msCompIdx[id];
    const UnpackStsXpuPar& unpackPar = params[comp];

    // --- Get starting position of elink parameters of this MS
    UnpackStsXpuElinkPar* elinkPar = &elinkParams[unpackPar.fElinkOffset];

    // --- Init counter for produced digis
    uint64_t numDigis = 0;

    // --- The first message in the MS is expected to be of type EPOCH and can be ignored.
    if (message[0].GetMessType() != stsxyter::MessType::Epoch) {
      monitor.fNumErrInvalidFirstMessage++;
      msMessCount[id] = 0;
      return;
    }

    // --- The second message must be of type ts_msb.
    if (message[1].GetMessType() != stsxyter::MessType::TsMsb) {
      monitor.fNumErrInvalidFirstMessage++;
      msMessCount[id] = 0;
      return;
    }

    // --- Current TS_MSB epoch cycle
    uint64_t currentCycle = msStartTime[id] / fkCycleLength;

    // --- Process first message (ts_msb)
    uint32_t currentEpoch     = 0;  ///< Current epoch number within epoch cycle
    uint64_t currentEpochTime = 0;  ///< Current epoch time relative to timeslice in clock cycles
    ProcessTsmsbMessage(message[1], currentEpoch, currentEpochTime, currentCycle, currentTsTime);

    // --- Message loop
    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].GetMessType()) {
        case stsxyter::MessType::Hit: {
          ProcessHitMessage(message[messageNr], digis, numDigis, unpackPar, elinkPar, monitor, currentEpochTime);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message[messageNr], currentEpoch, currentEpochTime, currentCycle, currentTsTime);
          break;
        }
        default: {
          monitor.fNumNonHitOrTsbMessage++;
          break;
        }
      }
    }
    // --- Store number of digis in buffer
    msMessCount[id] = numDigis;
  }


  // -----   Process hit message   --------------------------------------------
  XPU_D inline void UnpackStsXpu::ProcessHitMessage(const stsxyter::Message& message, CbmStsDigi* digis,
                                                    uint64_t& numDigis, const UnpackStsXpuPar& unpackPar,
                                                    UnpackStsXpuElinkPar* elinkParams, UnpackStsXpuMonitorData& monitor,
                                                    const uint64_t currentEpochTime)
  {
    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= unpackPar.fNumElinks) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackStsXpuElinkPar& elinkPar = elinkParams[elink];
    uint32_t asicNr                      = elinkPar.fAsicNr;

    // --- Hardware-to-software address
    uint32_t numChansPerModule = unpackPar.fNumAsicsPerModule * unpackPar.fNumChansPerAsic;
    uint32_t address           = elinkPar.fAddress;
    uint32_t channel           = 0;
    if (asicNr < unpackPar.fNumAsicsPerModule / 2) {  // front side (n side)
      channel = message.GetHitChannel() + unpackPar.fNumChansPerAsic * asicNr;
    }
    else {  // back side (p side)
      channel = numChansPerModule - message.GetHitChannel()
                - unpackPar.fNumChansPerAsic * (asicNr - unpackPar.fNumAsicsPerModule / 2) - 1;
    }

    // --- Expand time stamp to time within timeslice (in clock cycle)
    uint64_t messageTime = message.GetHitTimeBinning() + currentEpochTime;

    // --- Convert time stamp from clock cycles to ns. Round to nearest full ns.
    messageTime = (messageTime * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;

    // --- Correct ASIC-wise offsets
    messageTime -= elinkPar.fTimeOffset;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    digis[numDigis] = CbmStsDigi(address, channel, messageTime, charge);
    numDigis++;
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch (TS_MSB) message   ------------------------------
  XPU_D inline void UnpackStsXpu::ProcessTsmsbMessage(const stsxyter::Message& message, uint32_t& currentEpoch,
                                                      uint64_t& currentEpochTime, uint64_t& currentCycle,
                                                      const uint64_t currentTsTime)
  {
    // The compression of time is based on the hierarchy epoch cycle - epoch - message time.
    // Cycles are counted from the start of Unix time and are multiples of an epoch (ts_msb).
    // The epoch number is counted within each cycle. The time in the hit message is expressed
    // in units of the readout clock cycle relative to the current epoch.
    // The ts_msb message indicates the start of a new epoch. Its basic information is the epoch
    // number within the current cycle. A cycle wrap resets the epoch number to zero, so it is
    // indicated by the epoch number being smaller than the previous one (epoch messages are
    // seemingly not consecutively in the data stream, but only if there are hit messages in between).
    auto epoch = message.GetTsMsbValBinning();

    // --- Cycle wrap
    if (epoch < currentEpoch) currentCycle++;

    // --- Update current epoch counter
    currentEpoch = epoch;

    // --- Calculate epoch time in clocks cycles relative to timeslice start time
    currentEpochTime = (currentCycle * fkEpochsPerCycle + epoch - currentTsTime) * fkEpochLength;
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
