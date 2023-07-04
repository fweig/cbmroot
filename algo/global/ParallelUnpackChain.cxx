/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#include "ParallelUnpackChain.h"

#include "CbmStsDigi.h"

#include <Timeslice.hpp>

#include <xpu/host.h>

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
  }
}

std::vector<CbmStsDigi> ParallelUnpackChain::Run(const fles::Timeslice& timeslice)
{
  assert(fConfig.has_value() && "UnpackChain: Configuration not initialized (call Init first))");

  xpu::scoped_timer t_("Parallel Unpacker");

  size_t numMs              = 0;
  const size_t bytesPerDigi = sizeof(CbmStsDigi);
  u64 maxNumDigis           = 0;
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
        uint64_t numDigisInMs = msByteSize / bytesPerDigi;
        const u32 numMessages = msByteSize / sizeof(stsxyter::Message);
        maxNumMessages += numMessages;
        maxNumDigis += numDigisInMs;
      }
    }
  }
  std::cout << "MaxMessages: " << maxNumMessages << std::endl;

  // Allocate Input buffers
  fStsUnpacker.fMsEquipmentId.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsDescriptors.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsContent.reset(maxNumMessages, xpu::buf_io);
  fStsUnpacker.fMsOffset.reset(numMs, xpu::buf_io);

  // Initialize Output buffers
  fStsUnpacker.fMsDigiCount.reset(numMs, xpu::buf_io);
  fStsUnpacker.fMsDigis.reset(maxNumMessages, xpu::buf_io);
  fStsUnpacker.fMsMonitor.reset(numMs, xpu::buf_io);

  xpu::set<sts::TheUnpacker>(fStsUnpacker);

  std::vector<stsxyter::Message> messages;  //storage of all messages
  u64 messageOffset = 0;
  // Counter for index to see how many comps were actually used
  u16 usedComps = 0;

  // Felix: For new Unpacker: Set pointer for MS and set descriptors and equiptment id.
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {
    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      uint64_t numMsInComp = timeslice.num_microslices(comp);
      u16 componentId = eqIdToIndex.at(timeslice.descriptor(comp, 0).eq_id);  // Felix
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        u16 index = usedComps * numMsInComp + mslice;
        
        const auto msContent  = timeslice.content(comp, mslice);
        const auto msDescr    = timeslice.descriptor(comp, mslice);
        const u32 numMessages = msDescr.size / sizeof(stsxyter::Message);
        const auto message    = reinterpret_cast<const stsxyter::Message*>(msContent);
        messages.insert(messages.end(), message, message + numMessages);

        // Fill input buffer
        fStsUnpacker.fMsEquipmentId[index]  = componentId;
        fStsUnpacker.fMsDescriptors[index]  = msDescr;

        // Offset for starting message (in MsContent) of each MS
        fStsUnpacker.fMsOffset[index]     = messageOffset;

        // Content contains all messages - multiple messages per MS. 
        for (u32 i = 0; i < numMessages; i++){
          u64 offset = messageOffset + i;
          fStsUnpacker.fMsContent[offset] = messages[offset];
        }
        messageOffset += numMessages;            
      }
    usedComps++;
    }
  }

  std::cout << "maxNumDigis: " << maxNumDigis << std::endl;
  std::cout << "numMs: " << numMs << std::endl;
  std::cout << "maxNumMessages: " << maxNumMessages << std::endl;


  // Felix: Unpacker call
  xpu::push_timer("Unpack TS");

  xpu::queue queue;

  // Transferring buffer data from host to device
  queue.copy(fStsUnpacker.fMsEquipmentId, xpu::h2d);
  queue.copy(fStsUnpacker.fMsDescriptors, xpu::h2d);
  queue.copy(fStsUnpacker.fMsContent, xpu::h2d);
  queue.copy(fStsUnpacker.fMsOffset, xpu::h2d);

  static constexpr uint64_t epochLength = stsxyter::kuHitNbTsBinsBinning;
  static constexpr uint32_t clockCycleNom = stsxyter::kulClockCycleNom;
  static constexpr uint32_t clockCycleDen = stsxyter::kulClockCycleDen;
  static constexpr u64 epochLengthInNs = epochLength * clockCycleNom / clockCycleDen;

  const u64 currentTsTime   = timeslice.start_time() / epochLengthInNs;

  // Kernel call
  queue.launch<sts::Unpack>(xpu::n_blocks(numMs), currentTsTime, numMs);
  //xpu::k_add_bytes<Unpack>()

  // Transfer output data from device to host
  queue.copy(fStsUnpacker.fMsDigiCount, xpu::d2h);
  queue.copy(fStsUnpacker.fMsDigis, xpu::d2h);
  queue.copy(fStsUnpacker.fMsMonitor, xpu::d2h);

  xpu::h_view outDigiCount {fStsUnpacker.fMsDigiCount};
  xpu::h_view outDigis {fStsUnpacker.fMsDigis};

  xpu::push_timer("Store digis");

  std::vector<CbmStsDigi> result;

  // copy data from buffers to output vector
  // TODO: Put on GPU

  for (u64 i = 0; i < numMs; i++){
    u64 offset      = fStsUnpacker.fMsOffset[i];
    u64 numOutDigis    = outDigiCount[i];

    for (u64 j = 0; j < numOutDigis; j++){
      result.push_back(outDigis[offset + j]);
    } 
  }
  xpu::pop_timer();
  xpu::pop_timer();


  L_(info) << "Timeslice contains " << result.size() << " STS digis (discarded " << 0 << " pulser hits)";

  fStsUnpacker = {};
  xpu::set<sts::TheUnpacker>(fStsUnpacker);
  return result;
}
