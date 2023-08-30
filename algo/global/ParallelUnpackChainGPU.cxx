/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#include "ParallelUnpackChain.h"

#include "CbmStsDigi.h"

#include <Timeslice.hpp>

#include <xpu/host.h>
#include <omp.h>   // TODO: Check if needed

#include "StsUnpackChain.h"
#include "log.hpp"

using namespace cbm::algo;
using std::cout;
using std::endl;

void ParallelUnpackChain::Init(sts::ReadoutMapping config)
{
  fConfig = config;
  eqIdToIndex.clear();

  // Copied from CbmTaskUnpack in CBMRoot.

  // --- Common parameters for all components for STS
  uint32_t numChansPerAsicSts   = 128;  // R/O channels per ASIC for STS
  uint32_t numAsicsPerModuleSts = 16;   // Number of ASICs per module for STS

  auto equipIdsSts = fConfig->GetEquipmentIds();
  /*
  size_t nComponents = equipIdsSts.size();
  for (size_t i = 0; i < nComponents; i++) {
    eqIdToIndex[equipIdsSts[i]] = i;
  }
  //Initialize parameter buffers
  const size_t numAllElinks                     = fConfig->GetNumElinks();
  
  
  fStsUnpacker.fParams.reset(nComponents, xpu::buf_io);
  fStsUnpacker.fElinkParams.reset(numAllElinks, xpu::buf_io);
  for (size_t comp = 0; comp < nComponents; comp++) {
    u16 eqId                                    = equipIdsSts[comp];
    const size_t numElinks                      = fConfig->GetNumElinks(eqId);
    // auto params                                 = fStsUnpacker.fParams.h();
    xpu::h_view params {fStsUnpacker.fParams};
    params[comp].fNumElinks                     = numElinks;
    params[comp].fNumChansPerAsic               = numChansPerAsicSts;
    params[comp].fNumAsicsPerModule             = numAsicsPerModuleSts;
    params[comp].fElinkOffset                   = (comp == 0) ? 0 : params[comp - 1].fElinkOffset + params[comp - 1].fNumElinks;

    for (size_t elink = 0; elink < numElinks; elink++) {
      xpu::h_view elinkTemp {fStsUnpacker.fElinkParams};
      sts::UnpackElinkPar& elinkPar             = elinkTemp[params[comp].fElinkOffset + elink];
      auto mapEntry                             = fConfig->Map(eqId, elink);
      elinkPar.fAddress                         = mapEntry.moduleAddress;  // Module address for this elink
      elinkPar.fAsicNr                          = mapEntry.asicNumber;     // ASIC number within module
      elinkPar.fTimeOffset                      = 0.;
      elinkPar.fAdcOffset                       = 1.;
      elinkPar.fAdcGain                         = 1.;
    }
    L_(debug) << "--- Configured STS equipment " << eqId << " with " << numElinks << " elinks";
  }*/
  fnComponents = equipIdsSts.size();
  for (size_t i = 0; i < fnComponents; i++) {
    eqIdToIndex[equipIdsSts[i]] = i;
  }

  fnumAllElinks = fConfig->GetNumElinks();

  fUnpackPar.resize(fnComponents);
  fUnpackElinkPar.resize(fnumAllElinks);
  
  for (size_t comp = 0; comp < fnComponents; comp++) {
    u16 eqId                                    = equipIdsSts[comp];
    const size_t numElinks                      = fConfig->GetNumElinks(eqId);
    // auto params                                 = fStsUnpacker.fParams.h();
    fUnpackPar[comp].fNumElinks                     = numElinks;
    fUnpackPar[comp].fNumChansPerAsic               = numChansPerAsicSts;
    fUnpackPar[comp].fNumAsicsPerModule             = numAsicsPerModuleSts;
    fUnpackPar[comp].fElinkOffset                   = (comp == 0) ? 0 : fUnpackPar[comp - 1].fElinkOffset + fUnpackPar[comp - 1].fNumElinks;

    for (size_t elink = 0; elink < numElinks; elink++) {
      sts::UnpackElinkPar& elinkPar             = fUnpackElinkPar[fUnpackPar[comp].fElinkOffset + elink];
      auto mapEntry                             = fConfig->Map(eqId, elink);
      elinkPar.fAddress                         = mapEntry.moduleAddress;  // Module address for this elink
      elinkPar.fAsicNr                          = mapEntry.asicNumber;     // ASIC number within module
      elinkPar.fTimeOffset                      = 0.;
      elinkPar.fAdcOffset                       = 1.;
      elinkPar.fAdcGain                         = 1.;
    }
    L_(debug) << "--- Configured STS equipment " << eqId << " with " << numElinks << " elinks";
  }
}

std::vector<CbmStsDigi> ParallelUnpackChain::Run(const fles::Timeslice& timeslice)
{
  assert(fConfig.has_value() && "UnpackChain: Configuration not initialized (call Init first))");

  xpu::scoped_timer t_("Parallel Unpack Chain");
  xpu::push_timer("Buffer Preparations");

  size_t numMs              = 0;
  u64 maxNumMessages        = 0;

  // Count number of timeslices and B bytes per timeslice to calculate offsets output buffer
  // maxNum digis per timeslice is calculated by dividing bytes per mslice / size of CbmStsDigi  
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {
    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      uint64_t numMsInComp = timeslice.num_microslices(comp);
      numMs += numMsInComp;
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        uint64_t msByteSize   = timeslice.descriptor(comp, mslice).size;
        const u32 numMessages = msByteSize / fMsgSize;
        maxNumMessages += numMessages;
      }
    }
  }

  // Allocate input buffers
  fStsUnpacker.fMsEquipmentId.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsDescriptors.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsContent.reset(maxNumMessages, xpu::buf_io);
  fStsUnpacker.fMsOffset.reset(numMs, xpu::buf_io);

  // Allocate parameter buffers
  fStsUnpacker.fParams.reset(fnComponents, xpu::buf_io);
  fStsUnpacker.fElinkParams.reset(fnumAllElinks, xpu::buf_io);

  // Allocate output buffers
  fStsUnpacker.fMsDigiCount.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsDigis.reset(maxNumMessages, xpu::buf_io);
  fStsUnpacker.fMsMonitor.reset(numMs, xpu::buf_io);

  // get host side views for data input
  xpu::h_view fMsEquipmentId {fStsUnpacker.fMsEquipmentId};
  xpu::h_view fMsDescriptors {fStsUnpacker.fMsDescriptors};
  xpu::h_view fMsContent {fStsUnpacker.fMsContent};
  xpu::h_view fMsOffset {fStsUnpacker.fMsOffset};
  
  xpu::set<sts::TheUnpacker>(fStsUnpacker);

  
  u64 messageOffset = 0;
  // Counter for index to see how many comps were actually used
  u16 usedComps = 0;

  // sequential loop for the offsets
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {
    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      uint64_t numMsInComp = timeslice.num_microslices(comp); 
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        u16 index = usedComps * numMsInComp + mslice;

        const auto msDescr    = timeslice.descriptor(comp, mslice);
        const u32 numMessages = msDescr.size / fMsgSize;
        fMsOffset[index]     = messageOffset;
        messageOffset += numMessages;            
      }
    usedComps++;
    }
  }

  // Fill buffers in parallel
  usedComps = 0;
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {
    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      uint64_t numMsInComp = timeslice.num_microslices(comp);
      u16 componentId = eqIdToIndex.at(timeslice.descriptor(comp, 0).eq_id);  

#pragma omp parallel for schedule (dynamic) shared (numMsInComp, usedComps)
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        u16 index = usedComps * numMsInComp + mslice;
        
        const auto msContent  = timeslice.content(comp, mslice);
        const auto msDescr    = timeslice.descriptor(comp, mslice);
        const u32 numMessages = msDescr.size / fMsgSize;
        
        std::vector<stsxyter::Message> messages;  
        messages.reserve(numMessages * fMsgSize); // TODO: find out if necessary
        const auto message    = reinterpret_cast<const stsxyter::Message*>(msContent);
        messages.insert(messages.end(), message, message + numMessages);

        // Fill input buffer
        fMsEquipmentId[index]  = componentId; 
        fMsDescriptors[index]  = msDescr;

        // Content contains all messages - multiple messages per MS. 
        for (u32 i = 0; i < numMessages; i++){
          u64 offset = fMsOffset[index] + i;
          fMsContent[offset] = messages[i];
        }     
      }
    usedComps++;
    }
  }

  // Copy Unpack Parameters from vector to buffer -- may be done in init?
  xpu::h_view params {fStsUnpacker.fParams};
  xpu::h_view elinkParams {fStsUnpacker.fElinkParams};
#pragma omp parallel for schedule (dynamic)  
  for (u32 i = 0; i < fUnpackPar.size(); i++){
    params[i] = fUnpackPar[i];
  }
#pragma omp parallel for schedule (dynamic)
  for (u32 i = 0; i < fUnpackElinkPar.size(); i++){
    elinkParams[i] = fUnpackElinkPar[i];
  }
  xpu::pop_timer(); 
  // End of buffer preparations

  xpu::queue queue;

  // Transferring buffer data from host to device
  queue.copy(fStsUnpacker.fMsEquipmentId, xpu::h2d);
  queue.copy(fStsUnpacker.fMsDescriptors, xpu::h2d);
  queue.copy(fStsUnpacker.fMsContent, xpu::h2d);
  queue.copy(fStsUnpacker.fMsOffset, xpu::h2d);
  queue.copy(fStsUnpacker.fElinkParams, xpu::h2d);
  queue.copy(fStsUnpacker.fParams, xpu::h2d);

  const u64 currentTsTime   = timeslice.start_time() / fEpochLengthInNs;

  // Unpacker Kernel call
  queue.launch<sts::Unpack>(xpu::n_blocks(numMs), currentTsTime);
  xpu::k_add_bytes<sts::Unpack>(maxNumMessages * fMsgSize);

  // Transfer output data from device to host
  queue.copy(fStsUnpacker.fMsDigiCount, xpu::d2h);
  queue.copy(fStsUnpacker.fMsMonitor, xpu::d2h);
  queue.wait();

  // count output digis
  xpu::push_timer("Count OutDigis");
  xpu::h_view outDigiCount {fStsUnpacker.fMsDigiCount};
  u32 numOutDigis = 0;
  for (u32 i = 0; i < outDigiCount.size(); i++){
      numOutDigis += outDigiCount[i];
  }
  xpu::pop_timer();
  
  // resize the output buffer according to the number of digis
  xpu::push_timer("OutBuffer Prep");
  fStsUnpacker.fMsOutBuffer.reset(numOutDigis, xpu::buf_io);
  xpu::set<sts::TheUnpacker>(fStsUnpacker);
  xpu::pop_timer();

  // execute Kernel for copying Digis from temp buffer to correctly sized buffer
  xpu::push_timer("CopyOut Kernel");
  queue.launch<sts::CopyOut>(xpu::n_blocks(numMs));
  xpu::k_add_bytes<sts::CopyOut>(numOutDigis * sizeof(CbmStsDigi));
  queue.copy(fStsUnpacker.fMsOutBuffer, xpu::d2h);
  xpu::h_view outBuffer {fStsUnpacker.fMsOutBuffer};
  xpu::pop_timer();

  // Resize the output vector for UnpackChain Return
  xpu::push_timer("Resize");
  std::vector<CbmStsDigi> result;
  result.resize(numOutDigis);
  xpu::pop_timer();

  // Copy from output buffer to return vector -- delete when both unpacker and hitfinder run on GPU
  xpu::push_timer("Parallel Copy");
#pragma omp parallel for schedule (static)
  for (u64 i = 0; i < numOutDigis; i++){
    result[i] = outBuffer[i]; 
  }
  xpu::pop_timer();

  L_(info) << "Timeslice contains " << result.size() << " STS digis (discarded " << 0 << " pulser hits)";

  // reset to prevent XPU bugs
  fStsUnpacker = {};  
  xpu::set<sts::TheUnpacker>(fStsUnpacker);
  return result;
}
