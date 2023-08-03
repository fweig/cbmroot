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

XPU_EXPORT(cbm::algo::sts::TheUnpacker);
XPU_EXPORT(cbm::algo::sts::Unpack);
XPU_EXPORT(cbm::algo::sts::CopyOut);

namespace cbm::algo::sts
{
  XPU_D void Unpack::operator()(context& ctx, const uint64_t currentTsTime)
  {
    ctx.cmem<TheUnpacker>()(ctx, currentTsTime);
  }

  XPU_D void CopyOut::operator()(context& ctx)
  {
    ctx.cmem<TheUnpacker>().CopyOut(ctx);
  }


  // ----   Algorithm execution   ---------------------------------------------
  XPU_D void MsUnpacker::operator()(Unpack::context & ctx, const uint64_t currentTsTime) const
  {
    //int id = ctx.block_idx_x() * ctx.block_dim_x() + ctx.thread_idx_x();
    int id = ctx.block_idx_x();

    UnpackMonitorData monitor;  //Monitor data, currently not stored. TO DO: Implement!

    // --- Get message count and offset for this MS
    const auto& msDescr = fMsDescriptors[id];
    //const u8* msContent = fMsContent[id];

    const u32 numMessages = msDescr.size / sizeof(stsxyter::Message);
    const u32 messOffset  = fMsOffset[id];

    // --- Get starting position of this MS in message buffer
    // stsxyter::Message* message = &content[messOffset];
    // --- Interpret MS content as sequence of SMX messages
    //auto message = reinterpret_cast<const stsxyter::Message*>(msContent);

    // --- Get starting position of this MS in digi buffer
    CbmStsDigi* digis = &fMsDigis[messOffset];

    // --- Get component index and unpack parameters of this MS
    const uint32_t comp              = fMsEquipmentId[id];
    const UnpackPar& unpackPar       = fParams[comp];

    // --- Get starting position of elink parameters of this MS
    UnpackElinkPar* elinkPar = &fElinkParams[unpackPar.fElinkOffset];

    TimeSpec time;
    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    time.fCurrentTsTime            = currentTsTime;

    auto const msTime              = msDescr.idx;
    time.fCurrentCycle = msTime / fkCycleLength;

    if (numMessages < 2){
      monitor.fNumErrInvalidMsSize++;
      fMsDigiCount[id] = 0;
      return;
    }

    stsxyter::Message firstMessage = fMsContent[messOffset];
    // --- The first message in the MS is expected to be of type EPOCH and can be ignored.
    if (firstMessage.GetMessType() != stsxyter::MessType::Epoch) {
      monitor.fNumErrInvalidFirstMessage++;
      fMsDigiCount[id] = 0;
      return;
    }

    stsxyter::Message secondMessage = fMsContent[messOffset + 1];
    // --- The second message must be of type ts_msb.
    if (secondMessage.GetMessType() != stsxyter::MessType::TsMsb) {
      monitor.fNumErrInvalidFirstMessage++;
      fMsDigiCount[id] = 0;
      return;
    }

    ProcessTsmsbMessage(secondMessage, time);

    // --- Init counter for produced digis
    u64 numDigis = 0;

    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr += fSmemBufferSize) {
      
      u64 messIndex = messOffset + messageNr;
      // Copy Data to smem
      u16 messageBufferSize = xpu::min(numMessages - messageNr, u32 (fSmemBufferSize));
      for(u32 i = ctx.thread_idx_x(); i < messageBufferSize; i += ctx.block_dim_x()){
        ctx.smem().messageBuffer[i] = fMsContent[messIndex + i];
      }

      xpu::barrier(ctx.pos());
  
      auto& digiCounter = ctx.smem().digiCounter;

      if (ctx.thread_idx_x() == 0){
        ctx.smem().startNumDigis = numDigis;
        CbmStsDigi* smemDigis = &ctx.smem().digiBuffer[0];
        digiCounter = 0;

        for (u32 j = 0; j < messageBufferSize; j++){
          stsxyter::Message message = ctx.smem().messageBuffer[j];
          // --- Action depending on message type
          switch (message.GetMessType()) {
            case stsxyter::MessType::Hit: {
              ProcessHitMessage(message, smemDigis, digiCounter, unpackPar, elinkPar, monitor,
                                        time);
              break;
            }
            case stsxyter::MessType::TsMsb: {
              ProcessTsmsbMessage(message, time);
              break;
            }
            default: {
              monitor.fNumNonHitOrTsbMessage++;
              break;
            }
          }
        }
        numDigis += digiCounter;
      }

      xpu::barrier(ctx.pos());

      for (u32 k = ctx.thread_idx_x(); k < digiCounter; k += ctx.block_dim_x()){
        u64 outIndex = ctx.smem().startNumDigis + k;
        digis[outIndex] = ctx.smem().digiBuffer[k]; 
      }

      xpu::barrier(ctx.pos());

      
        //? Message type

    }  //# Messages
    // --- Store number of digis in buffer
    if (ctx.thread_idx_x() == 0){
      fMsDigiCount[id] = numDigis;
    }
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  XPU_D inline void MsUnpacker::ProcessHitMessage(const stsxyter::Message& message, CbmStsDigi* digis, uint64_t& numDigis,
                                        const UnpackPar& unpackPar, UnpackElinkPar* elinkParams,
                                        UnpackMonitorData& monitor, TimeSpec& time) const
  {
    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= unpackPar.fNumElinks) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackElinkPar& elinkPar = elinkParams[elink];
    uint32_t asicNr                = elinkPar.fAsicNr;

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
    uint64_t messageTime = message.GetHitTimeBinning() + time.fCurrentEpochTime;

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
  XPU_D inline void MsUnpacker::ProcessTsmsbMessage(const stsxyter::Message& message, TimeSpec& time) const
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

  XPU_D void MsUnpacker::CopyOut(CopyOut::context & ctx) const
  {
    u32 blockId = ctx.block_idx_x();
    u32 threadId = ctx.thread_idx_x();
    u32 blockDim = ctx.block_dim_x();

    // count digis written by blocks with lower IDs to get starting index
    u64 startDigi = 0;
    for (u64 i = 0; i < blockId; i++){
      startDigi += fMsDigiCount[i];
    }
    
    // starting parameters are the same for all threads in one block
    u64 numDigis = fMsDigiCount[blockId];
    u64 offset = fMsOffset[blockId];
    
    // each thread copys the data corresponding to their threadnumber + blockDim
    for (u64 i = threadId; i < numDigis; i+=blockDim){
      fMsOutBuffer[startDigi + i] = fMsDigis[offset + i];
    }
  }


}  // namespace cbm::algo::sts
