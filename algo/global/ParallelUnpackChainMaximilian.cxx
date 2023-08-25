/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#include "ParallelUnpackChain.h"

#include "CbmStsDigi.h"

#include <Timeslice.hpp>

#include <xpu/host.h>

#include "StsUnpackChain.h"
#include "log.hpp"
#include <omp.h>

using namespace cbm::algo;
using std::cout;
using std::endl;

void ParallelUnpackChain::Init(sts::ReadoutMapping config)
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
      elinkPar.fAddress    = mapEntry.moduleAddress;  // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.asicNumber;     // ASIC number within module
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

std::vector<CbmStsDigi> ParallelUnpackChain::Run(const fles::Timeslice& timeslice)
{
  assert(fConfig.has_value() && "UnpackChain: Configuration not initialized (call Init first))");

  xpu::scoped_timer t_("Parallel Unpacker");

  size_t numMs         = 0;
  size_t numBytes      = 0;
  size_t numDigis      = 0;
  uint64_t numCompUsed = 0;

  //Additional Timer for measure


  //Additional Space for saving
  const int max_threads = omp_get_max_threads();
  std::vector < std::vector <CbmStsDigi> > space (max_threads);
  std::vector<CbmStsDigi> digisOut;
  for(int i = 0; i < max_threads; i++)
          space[i].reserve((30000000*timeslice.num_components())/max_threads);

  xpu::push_timer("Kernel");
  // ---  Component loop
#pragma omp parallel for schedule(dynamic) shared(timeslice) reduction(+ : numMs, numBytes, numDigis, numCompUsed)
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice.descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice.descriptor(comp, 0).sys_ver == 0x20);

      // --- Component log
      size_t numBytesInComp = 0;
      size_t numDigisInComp = 0;

      // --- Microslice loop
      uint64_t numMsInComp = timeslice.num_microslices(comp);
//#pragma omp parallel for schedule(dynamic) reduction(+ : numBytesInComp, numDigisInComp)
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        const auto msDescriptor = timeslice.descriptor(comp, mslice);
        const auto msContent    = timeslice.content(comp, mslice);
        numBytesInComp += msDescriptor.size;
        auto result = (algoIt->second)(msContent, msDescriptor, timeslice.start_time());

        numDigisInComp += result.first.size();

        space[omp_get_thread_num()].insert(space[omp_get_thread_num()].end(), result.first.begin(), result.first.end());

//#pragma omp critical(insert_sts_digis)
        //ftimeslice.fData.fSts.fDigis.insert(ftimeslice.fData.fSts.fDigis.end(), result.first.begin(),result.first.end());
      }  //# microslice

      numCompUsed++;
      numBytes += numBytesInComp;
      numDigis += numDigisInComp;
      numMs += numMsInComp;


    }  //? system (only STS)

  }  //# component
  xpu::pop_timer();

  xpu::push_timer("Resize + Copy");
  int sum = 0;
#pragma omp parallel for schedule(dynamic) reduction(+ : sum)
  for(int i = 0; i < max_threads; i++)
          sum += space[i].size();

  cout << "Anzahl Digis: " << sum << endl;

  digisOut.resize(sum);

#pragma omp parallel for schedule(dynamic)
  for(int i = 0; i<max_threads; i++)
  {
          unsigned int partial_sum = 0;
          for(int x = 0; x < i; x++)
                  partial_sum += space[x].size();
          for(unsigned int x = 0; x < space[i].size(); x++)
                  digisOut.at(partial_sum + x) = space[i][x];
  }
  xpu::pop_timer();

  return digisOut;
  
}