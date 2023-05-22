/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#include "StsUnpackChain.h"

#include "CbmStsDigi.h"

#include <Timeslice.hpp>

#include <xpu/host.h>

#include "log.hpp"

using namespace cbm::algo;

void sts::UnpackChain::Init(StsReadoutConfig config)
{
  fConfig = config;

  // Copied from CbmTaskUnpack in CBMRoot.

  // --- Common parameters for all components for STS
  uint32_t numChansPerAsicSts   = 128;  // R/O channels per ASIC for STS
  uint32_t numAsicsPerModuleSts = 16;   // Number of ASICs per module for STS

  // Create one algorithm per component for STS and configure it with parameters
  auto equipIdsSts = fConfig->GetEquipmentIds();
  for (auto equip : equipIdsSts) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsicSts;
    par->fNumAsicsPerModule = numAsicsPerModuleSts;
    const size_t numElinks  = fConfig->GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fConfig->Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 1.;
      elinkPar.fAdcGain    = 1.;
      // elinkPar.fIsPulser   = mapEntry.pulser;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoSts[equip].SetParams(std::move(par));
    L_(debug) << "--- Configured STS equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments
}

std::vector<CbmStsDigi> sts::UnpackChain::Run(const fles::Timeslice& timeslice)
{
  assert(fConfig.has_value() && "UnpackChain: Configuration not initialized (call Init first))");

  xpu::scoped_timer t_("STS Unpacker");

  std::vector<CbmStsDigi> digis;
  size_t numPulsers = 0;

  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {

    // --- Component log
    size_t numDigisInComp = 0;
    uint64_t numMsInComp  = 0;

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);

    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice.descriptor(comp, 0).eq_id;
      // std::cout << "STS Unpacker: Component " << comp << ", equipment ID " << equipmentId << std::endl;
      // for (auto e : fAlgoSts) {
      //   std::cout << "STS Unpacker: Equipment ID " << e.first << std::endl;
      // }
      const auto algoIt = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice.descriptor(comp, 0).sys_ver == 0x20);

      // --- Microslice loop
      numMsInComp = timeslice.num_microslices(comp);
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        const auto msDescriptor = timeslice.descriptor(comp, mslice);
        const auto msContent    = timeslice.content(comp, mslice);

        xpu::t_add_bytes(msDescriptor.size);

        auto result = (algoIt->second)(msContent, msDescriptor, timeslice.start_time());
        L_(trace) << "STS Unpacker: Component " << comp << ", microslice " << mslice << ", digis "
                  << result.first.size() << ", errors " << result.second.fNumNonHitOrTsbMessage << " | "
                  << result.second.fNumErrElinkOutOfRange << " | " << result.second.fNumErrInvalidFirstMessage << " | "
                  << result.second.fNumErrInvalidMsSize << " | " << result.second.fNumErrTimestampOverflow << " | ";

        // numPulsers += result.second.fNumPulserHits;
        numDigisInComp += result.first.size();

        digis.insert(digis.end(), result.first.begin(), result.first.end());
      }  //# microslice
    }    // system STS

  }  // component

  L_(info) << "Timeslice contains " << digis.size() << " STS digis (discarded " << numPulsers << " pulser hits)";
  return digis;
}
