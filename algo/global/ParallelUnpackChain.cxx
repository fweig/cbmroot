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

  size_t numMs              = 0;
  size_t numBytes           = 0;
  size_t numDigis           = 0;
  uint64_t numCompUsed      = 0;
  size_t maxNumDigis        = 0;
  const size_t bytesPerDigi = 4;

  // Count number of Timeslices and B Bytes per TImeslice to calculate Offsets for space vector
  // To parallelize over mslices the space vector needs to bhave size = nr_mslices
  // mmaxNum Digis per Timeslice is calculated by dividing bytes per mslice / bytes per digi
  // which is currently a value between 4 and 5 -- feb 2023.
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++) {
    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      uint64_t numMsInComp = timeslice.num_microslices(comp);
      numMs += numMsInComp;
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        uint64_t msByteSize     = timeslice.descriptor(comp, mslice).size;
        uint64_t numDigisInComp = msByteSize / bytesPerDigi;
        if (numDigisInComp > maxNumDigis) { maxNumDigis = numDigisInComp; }
      }
    }
  }
  std::cout << "maxNumDigis: " << maxNumDigis << std::endl;
  std::cout << "numMs: " << numMs << std::endl;
  std::vector<std::vector<CbmStsDigi>> space(numMs);


  for (unsigned int i = 0; i < numMs; i++) {
    space[i].reserve(maxNumDigis);
  }

  // ---  Component loop  l
  /*
  uint64_t numMicroslices = 0;
  uint64_t numDigisBefore = 0;
  cout << "///////////////////////////////" << endl;
  for (uint64_t comp = 0; comp < timeslice.num_components(); comp++){
        numMicroslices += timeslice.num_microslices(comp);
        for (uint64_t mslice = 0; mslice < timeslice.num_microslices(comp); mslice++){
                cout << FormatMsHeaderPrintout(timeslice.descriptor(comp, mslice)) << endl;
        }
  }
  cout << "//////////////////////////////" << endl;
*/

  xpu::push_timer("Unpack TS");
  uint64_t rejectedComps = 0;

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

#pragma omp parallel for schedule(dynamic) shared(timeslice, algoIt, numMsInComp)                                      \
  reduction(+ : numBytesInComp, numDigisInComp)
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        const auto msDescriptor = timeslice.descriptor(comp, mslice);
        const auto msContent    = timeslice.content(comp, mslice);
        numBytesInComp += msDescriptor.size;
        cbm::algo::UnpackSts msUnpack = algoIt->second;  //Copy algorithm to ensure every thread has its own copy
        auto result                   = msUnpack(msContent, msDescriptor, timeslice.start_time());
        L_(trace) << "Parallel Unpacker"
                  << ": Component " << comp << ", microslice " << mslice << ", digis " << result.first.size()
                  << ", errors " << result.second.fNumNonHitOrTsbMessage << " | "
                  << result.second.fNumErrElinkOutOfRange << " | " << result.second.fNumErrInvalidFirstMessage << " | "
                  << result.second.fNumErrInvalidMsSize << " | " << result.second.fNumErrTimestampOverflow << " | ";

        numDigisInComp += result.first.size();
        size_t index = (comp - rejectedComps) * numMsInComp + mslice;
        space[index].insert(space[index].end(), result.first.begin(), result.first.end());


        //#pragma omp critical(insert_sts_digis)
        //fTimeslice.fData.fSts.fDigis.insert(fTimeslice.fData.fSts.fDigis.end(), result.first.begin(),result.first.end());
      }  //# microslice

      L_(debug) << "Parallel Unpacker"
                << ": Component " << comp << ", microslices " << numMsInComp << " input size " << numBytesInComp
                << "bytes "
                << ", digis " << numDigisInComp;
      // << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";

      numCompUsed++;
      numBytes += numBytesInComp;
      numDigis += numDigisInComp;
      // std::cout << "Komponente brauchte: " << compTimer.RealTime() * 1000 << "ms" << std::endl;

    }  //? system (only STS)
    else {
      rejectedComps++;
    }

  }  //# component
  // cout << "Timeslice brauchte: " << timeslicetimer.RealTime() * 1000 << "ms" << endl;
  xpu::pop_timer();

  int sum = 0;
#pragma omp parallel for schedule(dynamic) reduction(+ : sum)
  for (unsigned int i = 0; i < numMs; i++)
    sum += space[i].size();

  cout << "Anzahl Digis: " << sum << endl;
  cout << "Anzahl Digis(NumDigis): " << numDigis << endl;

  fSumDigis += sum;


  xpu::push_timer("Compact Digis");
  //fTimeslice.fData.fSts.fDigis.resize(sum);
  testVec.resize(sum);
#pragma omp parallel for schedule(dynamic)
  for (unsigned int i = 0; i < numMs; i++) {
    unsigned int partial_sum = 0;
    for (unsigned int x = 0; x < i; x++)
      partial_sum += space[x].size();
    for (unsigned int x = 0; x < space[i].size(); x++)
      testVec.at(partial_sum + x) = space[i][x];
  }
  xpu::pop_timer();

  //fVecDigis += fTimeslice.fData.fSts.fDigis.size();
  //fVecDigis += testVec.size();

  // cout << "Resize time: " << resizeTimer.RealTime() * 1000 << "ms" << endl;
  // resizeTimeGesamt += resizeTimer.RealTime() * 1000;

  // timer.Stop();

  xpu::push_timer("Copy to output");
  std::vector<CbmStsDigi> digisOut;
  digisOut.reserve(testVec.size());
  std::copy(testVec.begin(), testVec.end(), std::back_inserter(digisOut));
  xpu::pop_timer();


  fVecDigis += digisOut.size();
  L_(info) << "Timeslice contains " << digisOut.size() << " STS digis (discarded " << 0 << " pulser hits)";
  return digisOut;
}
