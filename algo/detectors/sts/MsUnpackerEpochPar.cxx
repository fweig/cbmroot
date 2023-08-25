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
    int blockId  = ctx.block_idx_x();
    int threadId = ctx.thread_idx_x();
    int blockDim = ctx.block_dim_x();

    UnpackMonitorData monitor;  //Monitor data, currently not stored. TO DO: Implement!

    // --- Get message count and offset for this MS
    const auto& msDescr = fMsDescriptors[blockId];

    const u32 numMessages = msDescr.size / sizeof(stsxyter::Message);
    const u32 messOffset  = fMsOffset[blockId];

    // --- Get component index and unpack parameters of this MS
    const uint32_t comp              = fMsEquipmentId[blockId];
    const UnpackPar& unpackPar       = fParams[comp];

    // --- Get starting position of elink parameters of this MS
    UnpackElinkPar* elinkPar = &fElinkParams[unpackPar.fElinkOffset];

    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    auto const msTime              = msDescr.idx;
    u64 currentCycle = msTime / fkCycleLength;

    if (numMessages < 2){
      monitor.fNumErrInvalidMsSize++;
      fMsDigiCount[blockId] = 0;
      return;
    }

    stsxyter::Message firstMessage = fMsContent[messOffset];
    // --- The first message in the MS is expected to be of type EPOCH and can be ignored.
    if (firstMessage.GetMessType() != stsxyter::MessType::Epoch) {
      monitor.fNumErrInvalidFirstMessage++;
      fMsDigiCount[blockId] = 0;
      return;
    }

    stsxyter::Message secondMessage = fMsContent[messOffset + 1];
    // --- The second message must be of type ts_msb.
    if (secondMessage.GetMessType() != stsxyter::MessType::TsMsb) {
      monitor.fNumErrInvalidFirstMessage++;
      fMsDigiCount[blockId] = 0;
      return;
    }

    // Initialize smem
    if (threadId == 0){
      ctx.smem().bcastBlockDigiOffset = 0;  // = numDigis
      ctx.smem().tsMsbMsg[0] = secondMessage.GetTsMsbValBinning();
      ctx.smem().bcastNCycles = 0;
    }
    xpu::barrier(ctx.pos());

    // find the next multiple of blockdim for numMessages, for better loop performance
    int tmp = numMessages % blockDim;
    int numMessagesBlockDim = numMessages;
    if (tmp) {
      numMessagesBlockDim += blockDim - tmp;
    }

    for (uint32_t messageNr = 2 + threadId; messageNr < numMessagesBlockDim + 2; messageNr += blockDim) {
      stsxyter::Message currentMsg;
      currentMsg.reset();
      if (messageNr < numMessages){
        u64 messIndex = messOffset + messageNr;
        currentMsg = fMsContent[messIndex];
      }
      int isTsMsbMsg = currentMsg.GetMessType() == stsxyter::MessType::TsMsb;
      int isHitMsg = currentMsg.GetMessType() == stsxyter::MessType::Hit;
      
      // Find index in shared memory to write tsmsb_msg
      int tsMsbIdx;
      Unpack::scan_t(ctx.pos(), ctx.smem().scan).inclusive_sum(isTsMsbMsg, tsMsbIdx);

      int cycleWrap = 0;
      u64 currentEpoch;
      if (isTsMsbMsg) {
        // Write tsmsb_msg to shared memory
        currentEpoch = currentMsg.GetTsMsbValBinning();   
        ctx.smem().tsMsbMsg[tsMsbIdx] = currentEpoch;
      
        // Find epoch cycle wraps   
        // compare current epoch to epoch of last TSMSG
        if (currentEpoch < ctx.smem().tsMsbMsg[tsMsbIdx - 1]){cycleWrap++;}
      }
      xpu::barrier(ctx.pos());

      // synchronize all previous cycleWraps
      int nCycleWraps;
      Unpack::scan_t(ctx.pos(), ctx.smem().scan).inclusive_sum(cycleWrap, nCycleWraps);
      xpu::barrier(ctx.pos());

      // unpack Digis
      CbmStsDigi digi;
      if (isHitMsg){
        currentEpoch = ctx.smem().tsMsbMsg[tsMsbIdx];
        
        // --- Calculate epoch time in clocks cycles relative to timeslice start time
        u64 currentEpochTime = ((currentCycle + nCycleWraps + ctx.smem().bcastNCycles) * fkEpochsPerCycle + currentEpoch - currentTsTime) * fkEpochLength;
        ProcessHitMessage(currentMsg, &digi, unpackPar, elinkPar, monitor, currentEpochTime);
      }
      xpu::barrier(ctx.pos());

      //calculate digi index by counting nr of previous digis in this iteration
      int digiIndex;
      Unpack::scan_t(ctx.pos(), ctx.smem().scan).inclusive_sum(isHitMsg, digiIndex);
      digiIndex--;
      xpu::barrier(ctx.pos());

      // write digi to global memory
      if(isHitMsg){
        fMsDigis[messOffset + digiIndex + ctx.smem().bcastBlockDigiOffset] = digi;
      }
      xpu::barrier(ctx.pos());

      // broadcast globally needed values
      if (threadId == (blockDim - 1)) { 
        ctx.smem().bcastBlockDigiOffset += digiIndex + 1; 
        ctx.smem().bcastNCycles += nCycleWraps;
        if(tsMsbIdx > 0){
          ctx.smem().tsMsbMsg[0] = currentEpoch;
        }
      }      
      xpu::barrier(ctx.pos());

    }  //# Messages
    // --- Store number of digis in buffer
    if (threadId == 0){
      fMsDigiCount[blockId] = ctx.smem().bcastBlockDigiOffset;
    }
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  XPU_D inline void MsUnpacker::ProcessHitMessage(const stsxyter::Message& message, CbmStsDigi* digi,
                                        const UnpackPar& unpackPar, UnpackElinkPar* elinkParams,
                                        UnpackMonitorData& monitor, u64 currentEpochTime) const
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
    uint64_t messageTime = message.GetHitTimeBinning() + currentEpochTime;

    // --- Convert time stamp from clock cycles to ns. Round to nearest full ns.
    messageTime = (messageTime * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;

    // --- Correct ASIC-wise offsets
    messageTime -= elinkPar.fTimeOffset;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    *digi = CbmStsDigi(address, channel, messageTime, charge);
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