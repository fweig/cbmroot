/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer], Felix Weiglhofer */
#include "MsUnpacker.h"

#include <cassert>
#include <utility>
#include <vector>

#include <cmath>

#include "StsXyterMessage.h"

using std::unique_ptr;
using std::vector;

namespace cbm::algo::sts
{

  // ----   Algorithm execution   ---------------------------------------------
  void MsUnpacker::operator()(const u32 msIndex, const uint64_t tTimeslice)
  {
    std::vector<CbmStsDigi> digisOut;
    digisOut.reserve(1000);
    UnpackMonitorData monitor;

    const auto& msDescr = fMsDescriptors.at(msIndex);
    const u8* msContent = fMsContent.at(msIndex);

    TimeSpec time;

    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    const uint64_t epochLengthInNs = fkEpochLength * fkClockCycleNom / fkClockCycleDen;
    time.fCurrentTsTime            = tTimeslice / epochLengthInNs;

    // --- Current TS_MSB epoch cycle
    auto const msTime  = msDescr.idx;  // Unix time of MS in ns
    time.fCurrentCycle = std::ldiv(msTime, fkCycleLength).quot;

    // --- Number of messages in microslice
    auto msSize = msDescr.size;
    if (msSize % sizeof(stsxyter::Message) != 0) {
      monitor.fNumErrInvalidMsSize++;
      return;
    }
    const uint32_t numMessages = msSize / sizeof(stsxyter::Message);
    if (numMessages < 2) {
      monitor.fNumErrInvalidMsSize++;
      return;
    }

    // --- Interpret MS content as sequence of SMX messages
    auto message = reinterpret_cast<const stsxyter::Message*>(msContent);

    // --- The first message in the MS is expected to be of type EPOCH and can be ignored.
    if (message[0].GetMessType() != stsxyter::MessType::Epoch) {
      monitor.fNumErrInvalidFirstMessage++;
      return;
    }

    // --- The second message must be of type ts_msb.
    if (message[1].GetMessType() != stsxyter::MessType::TsMsb) {
      monitor.fNumErrInvalidFirstMessage++;
      return;
    }
    ProcessTsmsbMessage(message[1], time);

    // --- Message loop
    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].GetMessType()) {

        case stsxyter::MessType::Hit: {
          ProcessHitMessage(msIndex, time, message[messageNr], digisOut, monitor);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message[messageNr], time);
          break;
        }
        default: {
          monitor.fNumNonHitOrTsbMessage++;
          break;
        }

      }  //? Message type

    }  //# Messages

    fMsDigis.at(msIndex)   = std::move(digisOut);
    fMsMonitor.at(msIndex) = monitor;
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  inline void MsUnpacker::ProcessHitMessage(const u32 msIndex, const TimeSpec& time, const stsxyter::Message& message,
                                            vector<CbmStsDigi>& digiVec, UnpackMonitorData& monitor) const
  {
    u16 eqId        = fMsEquipmentId.at(msIndex);
    u32 elinkOffset = fElinkOffsetPerComponent.at(eqId);
    u32 elinkNum    = fElinkOffsetPerComponent.at(eqId + 1) - elinkOffset;

    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= elinkNum) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackElinkPar& elinkPar = fElinkParams.at(elinkOffset + elink);
    uint32_t asicNr                = elinkPar.fAsicNr;

    // --- Hardware-to-software address
    uint32_t numChansPerModule = fNumAsicsPerModule[eqId] * fNumChansPerAsic[eqId];
    uint32_t address           = elinkPar.fAddress;
    uint32_t channel           = 0;
    if (asicNr < fNumAsicsPerModule[eqId] / 2) {  // front side (n side)
      channel = message.GetHitChannel() + fNumChansPerAsic[eqId] * asicNr;
    }
    else {  // back side (p side)
      channel = numChansPerModule - message.GetHitChannel()
                - fNumChansPerAsic[eqId] * (asicNr - fNumAsicsPerModule[eqId] / 2) - 1;
    }

    // --- Expand time stamp to time within timeslice (in clock cycle)
    uint64_t messageTime = message.GetHitTimeBinning() + time.fCurrentEpochTime;

    // --- Convert time stamp from clock cycles to ns. Round to nearest full ns.
    messageTime = (messageTime * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;

    // --- Correct ASIC-wise offsets
    messageTime -= elinkPar.fTimeOffset;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    digiVec.emplace_back(address, channel, messageTime, charge);
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch (TS_MSB) message   ------------------------------
  inline void MsUnpacker::ProcessTsmsbMessage(const stsxyter::Message& message, TimeSpec& time) const
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
    if (epoch < time.fCurrentEpoch) time.fCurrentCycle++;

    // --- Update current epoch counter
    time.fCurrentEpoch = epoch;

    // --- Calculate epoch time in clocks cycles relative to timeslice start time
    time.fCurrentEpochTime = (time.fCurrentCycle * fkEpochsPerCycle + epoch - time.fCurrentTsTime) * fkEpochLength;
  }
  // --------------------------------------------------------------------------


}  // namespace cbm::algo::sts
