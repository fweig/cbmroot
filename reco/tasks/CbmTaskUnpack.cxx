/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmTaskUnpack.h"

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiEvent.h"
#include "CbmDigiManager.h"
#include "CbmDigiTimeslice.h"
#include "CbmSourceTs.h"

#include "MicrosliceDescriptor.hpp"

#include <FairRunOnline.h>
#include <Logger.h>

#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#ifdef WITH_EXECUTION
#include <execution>
#endif
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

#include "UnpackSts.h"


using namespace std;
using cbm::algo::UnpackMuchElinkPar;
using cbm::algo::UnpackMuchPar;
using cbm::algo::UnpackStsElinkPar;
using cbm::algo::UnpackStsPar;
using cbm::algo::UnpackT0ElinkPar;
using cbm::algo::UnpackT0Par;
using cbm::algo::UnpackTofElinkPar;
using cbm::algo::UnpackTofPar;

// -----   Constructor   -----------------------------------------------------
CbmTaskUnpack::CbmTaskUnpack() : FairTask("Unpack") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskUnpack::~CbmTaskUnpack()
{
  if (fTimeslice) delete fTimeslice;
}
// ---------------------------------------------------------------------------


// ----------------- Microslice loop ------------------------------------------
template<class Digi, class UnpackAlgo>
uint64_t CbmTaskUnpack::MsLoop(const fles::Timeslice* timeslice, UnpackAlgo& algo, const uint64_t comp,
                               std::vector<Digi>* digis, size_t* numBytesInComp, size_t* numDigisInComp)
{
  const uint64_t numMsInComp = timeslice->num_microslices(comp);

  for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
    const auto msDescriptor = timeslice->descriptor(comp, mslice);
    const auto msContent    = timeslice->content(comp, mslice);
    *numBytesInComp += msDescriptor.size;
    auto result = algo(msContent, msDescriptor, timeslice->start_time());
    LOG(debug1) << GetName() << ": Component " << comp << ", microslice " << mslice << ", digis " << result.first.size()
                << ", errors " << result.second.fNumNonHitOrTsbMessage << " | " << result.second.fNumErrElinkOutOfRange
                << " | " << result.second.fNumErrInvalidFirstMessage << " | " << result.second.fNumErrInvalidMsSize
                << " | " << result.second.fNumErrTimestampOverflow << " | ";
    *numDigisInComp += result.first.size();
#pragma omp critical(insert_digis)
    digis->insert(digis->end(), result.first.begin(), result.first.end());
  }
  return numMsInComp;
}
// ----------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskUnpack::Exec(Option_t*)
{

  // --- Reset output branch (CbmDigiTimeslice)
  fTimeslice->Clear();

  // --- Get FLES timeslice
  assert(fSource);
  fles::Timeslice* timeslice = fSource->GetTimeslice();
  assert(timeslice);

  // --- Timer and counters
  TStopwatch timer;
  timer.Start();
  size_t numMs         = 0;
  size_t numBytes      = 0;
  size_t numDigis      = 0;
  uint64_t numCompUsed = 0;

  // ---  Component loop
#pragma omp parallel for schedule(dynamic) shared(timeslice) reduction(+ : numMs, numBytes, numDigis, numCompUsed)
  for (uint64_t comp = 0; comp < timeslice->num_components(); comp++) {

    // --- Component log
    size_t numBytesInComp = 0;
    size_t numDigisInComp = 0;
    uint64_t numMsInComp  = 0;

    TStopwatch compTimer;
    compTimer.Start();

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice->descriptor(comp, 0).sys_id);

    // STS
    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice->descriptor(comp, 0).sys_ver == 0x20);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fSts.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system STS

    // MUCH
    if (systemId == fles::SubsystemIdentifier::MUCH) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoMuch.find(equipmentId);
      assert(algoIt != fAlgoMuch.end());

      // The current algorithm works for the MUCH data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice->descriptor(comp, 0).sys_ver == 0x20);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fMuch.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system MUCH

    // TOF
    if (systemId == fles::SubsystemIdentifier::RPC) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoTof.find(equipmentId);
      assert(algoIt != fAlgoTof.end());

      // The current algorithm works for the TOF data format version XXXX used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXXX);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fTof.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system TOF

    // T0
    if (systemId == fles::SubsystemIdentifier::T0) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoT0.find(equipmentId);
      assert(algoIt != fAlgoT0.end());

      // The current algorithm works for the T0 data format version XXXX used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      //assert(timeslice->descriptor(comp, 0).sys_ver == XXXX);

      // --- Microslice loop
      numMsInComp =
        MsLoop(timeslice, algoIt->second, comp, &fTimeslice->fData.fT0.fDigis, &numBytesInComp, &numDigisInComp);

      numCompUsed++;
    }  // system T0

    compTimer.Stop();
    LOG(debug) << GetName() << ": Component " << comp << ", microslices " << numMsInComp << " input size "
               << numBytesInComp << " bytes, "
               << ", digis " << numDigisInComp << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";

    numBytes += numBytesInComp;
    numDigis += numDigisInComp;
    numMs += numMsInComp;

  }  //# component

  // --- Sorting of output digis. Is required by both digi trigger and event builder.
#ifdef WITH_EXECUTION
  std::sort(std::execution::par_unseq, fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fMuch.fDigis.begin(), fTimeslice->fData.fMuch.fDigis.end(),
            [](CbmMuchDigi digi1, CbmMuchDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fTof.fDigis.begin(), fTimeslice->fData.fTof.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(std::execution::par_unseq, fTimeslice->fData.fT0.fDigis.begin(), fTimeslice->fData.fT0.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#else
  std::sort(fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fMuch.fDigis.begin(), fTimeslice->fData.fMuch.fDigis.end(),
            [](CbmMuchDigi digi1, CbmMuchDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fTof.fDigis.begin(), fTimeslice->fData.fTof.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
  std::sort(fTimeslice->fData.fT0.fDigis.begin(), fTimeslice->fData.fT0.fDigis.end(),
            [](CbmTofDigi digi1, CbmTofDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#endif

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(15) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << " (index " << timeslice->index() << ")";
  logOut << ", components " << numCompUsed << " / " << timeslice->num_components() << ", microslices " << numMs;
  logOut << ", input rate " << double(numBytes) / timer.RealTime() / 1.e6 << " MB/s";
  logOut << ", digis " << numDigis;
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumMs += numMs;
  fNumBytes += numBytes;
  fNumDigis += numDigis;
  fTime += timer.RealTime();
}
// ----------------------------------------------------------------------------


// -----   End-of-run action   ------------------------------------------------
void CbmTaskUnpack::Finish()
{
  double timePerTs = 1000. * fTime / double(fNumTs);  // in ms
  double rate      = fNumBytes / 1.e6 / fTime;        // in MB/s
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices     : " << fNumTs;
  LOG(info) << "Microslices    : " << fNumMs;
  LOG(info) << "Digis          : " << fNumDigis;
  LOG(info) << "Av. input rate : " << fixed << setprecision(2) << rate << " MB/s";
  LOG(info) << "Time / TS      : " << fixed << setprecision(2) << timePerTs << " ms";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskUnpack::Init()
{

  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";

  // --- Get source instance
  fSource = dynamic_cast<CbmSourceTs*>(FairRunOnline::Instance()->GetSource());
  if (fSource == nullptr) {
    LOG(error) << GetName() << ": No valid source class registered!";
    return kFATAL;
  }
  LOG(info) << "--- Found CbmSourceTs instance";

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Register output array (CbmDigiTimeslice)
  if (ioman->GetObject("DigiTimeslice")) {
    LOG(fatal) << GetName() << ": Branch DigiTimeslice already exists!";
    return kFATAL;
  }
  fTimeslice = new CbmDigiTimeslice();
  ioman->RegisterAny("DigiTimeslice.", fTimeslice, IsOutputBranchPersistent("DigiTimeslice."));
  LOG(info) << "--- Registered branch DigiTimeslice.";

  // --- Common parameters for all components for STS
  uint32_t numChansPerAsicSts   = 128;  // R/O channels per ASIC for STS
  uint32_t numAsicsPerModuleSts = 16;   // Number of ASICs per module for STS

  // Create one algorithm per component for STS and configure it with parameters
  auto equipIdsSts = fStsConfig.GetEquipmentIds();
  for (auto& equip : equipIdsSts) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsicSts;
    par->fNumAsicsPerModule = numAsicsPerModuleSts;
    const size_t numElinks  = fStsConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fStsConfig.Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 1.;
      elinkPar.fAdcGain    = 1.;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoSts[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  // Create one algorithm per component and configure it with parameters
  auto equipIdsMuch = fMuchConfig.GetEquipmentIds();
  for (auto& equip : equipIdsMuch) {
    std::unique_ptr<UnpackMuchPar> par(new UnpackMuchPar());
    const size_t numElinks = fMuchConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackMuchElinkPar elinkPar;
      elinkPar.fAddress    = fMuchConfig.Map(equip, elink);  // Vector of MUCH addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoMuch[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }

  // Create one algorithm per component and configure it with parameters
  auto equipIdsTof = fTofConfig.GetEquipmentIds();
  for (auto& equip : equipIdsTof) {
    std::unique_ptr<UnpackTofPar> par(new UnpackTofPar());
    const size_t numElinks = fTofConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackTofElinkPar elinkPar;
      elinkPar.fChannelUId = fTofConfig.Map(equip, elink);  // Vector of TOF addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoTof[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }

  // Create one algorithm per component and configure it with parameters
  auto equipIdsT0 = fT0Config.GetEquipmentIds();
  for (auto& equip : equipIdsT0) {
    std::unique_ptr<UnpackT0Par> par(new UnpackT0Par());
    const size_t numElinks = fT0Config.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackT0ElinkPar elinkPar;
      elinkPar.fChannelUId = fT0Config.Map(equip, elink);  // Vector of T0 addresses for this elink
      elinkPar.fTimeOffset = 0.;
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoT0[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }


  LOG(info) << "--- Configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(debug) << "Readout map:" << fStsConfig.PrintReadoutMap();
  LOG(info) << "--- Configured " << fAlgoMuch.size() << " unpacker algorithms for MUCH.";
  LOG(info) << "--- Configured " << fAlgoTof.size() << " unpacker algorithms for TOF.";
  LOG(info) << "--- Configured " << fAlgoT0.size() << " unpacker algorithms for T0.";

  LOG(info) << "==================================================";

  return kSUCCESS;
}
// ----------------------------------------------------------------------------


ClassImp(CbmTaskUnpack)
