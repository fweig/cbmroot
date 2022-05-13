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

#include <FairLogger.h>
#include <FairRunOnline.h>

#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#ifdef WITH_EXECUTION
#include <execution>
#endif
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "UnpackSts.h"


using namespace std;
using cbm::algo::UnpackStsElinkPar;
using cbm::algo::UnpackStsPar;


// -----   Constructor   -----------------------------------------------------
CbmTaskUnpack::CbmTaskUnpack() : FairTask("Unpack") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskUnpack::~CbmTaskUnpack()
{
  if (fTimeslice) delete fTimeslice;
}
// ---------------------------------------------------------------------------


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

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice->descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice->descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice->descriptor(comp, 0).sys_ver == 0x20);

      // --- Component log
      size_t numBytesInComp = 0;
      size_t numDigisInComp = 0;
      TStopwatch compTimer;
      compTimer.Start();

      // --- Microslice loop
      uint64_t numMsInComp = timeslice->num_microslices(comp);
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        const auto msDescriptor = timeslice->descriptor(comp, mslice);
        const auto msContent    = timeslice->content(comp, mslice);
        numBytesInComp += msDescriptor.size;
        auto result = (algoIt->second)(msContent, msDescriptor, timeslice->start_time());
        LOG(debug1) << GetName() << ": Component " << comp << ", microslice " << mslice << ", digis "
                    << result.first.size() << ", errors " << result.second.fNumNonHitOrTsbMessage << " | "
                    << result.second.fNumErrElinkOutOfRange << " | " << result.second.fNumErrInvalidFirstMessage
                    << " | " << result.second.fNumErrInvalidMsSize << " | " << result.second.fNumErrTimestampOverflow
                    << " | ";
        numDigisInComp += result.first.size();
#pragma omp critical(insert_sts_digis)
        fTimeslice->fData.fSts.fDigis.insert(fTimeslice->fData.fSts.fDigis.end(), result.first.begin(),
                                             result.first.end());
      }  //# microslice

      compTimer.Stop();
      LOG(debug) << GetName() << ": Component " << comp << ", microslices " << numMsInComp << " input size "
                 << numBytesInComp << " bytes, "
                 << ", digis " << numDigisInComp << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";
      numCompUsed++;
      numBytes += numBytesInComp;
      numDigis += numDigisInComp;
      numMs += numMsInComp;

    }  //? system (only STS)

  }  //# component

  // --- Sorting of output digis. Is required by both digi trigger and event builder.
#ifdef WITH_EXECUTION
  std::sort(std::execution::par_unseq, fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
#else
  std::sort(fTimeslice->fData.fSts.fDigis.begin(), fTimeslice->fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });
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
  std::cout << std::endl;
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

  std::cout << std::endl;
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

  // --- Common parameters for all components
  uint32_t numChansPerAsic   = 128;  // R/O channels per ASIC
  uint32_t numAsicsPerModule = 16;   // Number of ASICs per module

  // Create one algorithm per component and configure it with parameters
  auto equipIds = fStsConfig.GetEquipmentIds();
  for (auto& equip : equipIds) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsic;
    par->fNumAsicsPerModule = numAsicsPerModule;
    const size_t numElinks  = fStsConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fStsConfig.Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 0.;
      elinkPar.fAdcGain    = 0.;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoSts[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  LOG(info) << "--- Configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(debug) << "Readout map:" << fStsConfig.PrintReadoutMap();
  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------


ClassImp(CbmTaskUnpack)
