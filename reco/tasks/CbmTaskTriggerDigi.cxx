/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmTaskTriggerDigi.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmModuleList.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include <FairLogger.h>

#include "TimeClusterTrigger.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

// -----   Constructor   -----------------------------------------------------
CbmTaskTriggerDigi::CbmTaskTriggerDigi() : FairTask("TriggerDigi") {}
// ---------------------------------------------------------------------------

// -----   Destructor   ------------------------------------------------------
CbmTaskTriggerDigi::~CbmTaskTriggerDigi()
{
  if (fTriggers) delete fTriggers;
}
// ---------------------------------------------------------------------------

// -----   Execution   -------------------------------------------------------
void CbmTaskTriggerDigi::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timerStep;
  TStopwatch timerTot;
  timerTot.Start();

  // --- Get digi times
  std::vector<double> digiTimes;
  for (const auto& system : fSystems) {
    CbmDigiBranchBase* digiBranch = fDigiMan->GetBranch(system);
    std::vector<double> locDigiTimes;
    switch (system) {
      case ECbmModuleId::kMuch: {  //we do not support the "MuchBeamTimeDigi"
        locDigiTimes = GetDigiTimes<CbmMuchDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kSts: {
        locDigiTimes = GetDigiTimes<CbmStsDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kTof: {
        locDigiTimes = GetDigiTimes<CbmTofDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kTrd: {
        locDigiTimes = GetDigiTimes<CbmTrdDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kRich: {
        locDigiTimes = GetDigiTimes<CbmRichDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kPsd: {
        locDigiTimes = GetDigiTimes<CbmPsdDigi>(digiBranch);
        break;
      }
      case ECbmModuleId::kT0: {  //T0 has Tof digis
        locDigiTimes = GetDigiTimes<CbmTofDigi>(digiBranch);
        break;
      }
      default: LOG(fatal) << GetName() << ": Reading digis from unknown detector type!";
    }
    digiTimes.insert(digiTimes.end(), locDigiTimes.begin(), locDigiTimes.end());
  }
  if (fSystems.size() > 1) { std::sort(digiTimes.begin(), digiTimes.end()); }

  // --- Call the trigger algorithm
  timerStep.Start();
  *fTriggers = fAlgo(digiTimes, fTriggerWindow, fMinNumDigis, fDeadTime);
  timerStep.Stop();
  fTimeFind += timerStep.RealTime();

  // --- Timeslice statistics
  size_t numDigis    = digiTimes.size();
  size_t numTriggers = fTriggers->size();

  // --- Timeslice log
  timerTot.Stop();
  fTimeTot += timerTot.RealTime();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTot.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", digis " << numDigis << ", triggers " << numTriggers;
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumDigis += numDigis;
  fNumTriggers += numTriggers;
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskTriggerDigi::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices         : " << fNumTs;
  LOG(info) << "Digis              : " << fNumDigis;
  LOG(info) << "Triggers           : " << fNumTriggers;
  LOG(info) << "Time  / TS         : " << fixed << setprecision(2) << 1000. * fTimeTot / double(fNumTs) << " ms";
  LOG(info) << "Time extract       : " << fixed << setprecision(2) << 1000. * fTimeExtract / double(fNumTs)
            << " ms = " << 100. * fTimeExtract / fTimeTot << " %";
  LOG(info) << "Time find trigger  : " << fixed << setprecision(2) << 1000. * fTimeFind / double(fNumTs)
            << " ms = " << 100. * fTimeFind / fTimeTot << " %";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskTriggerDigi::Init()
{

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- DigiManager instance
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  std::cout << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";

  // --- Check input data
  for (const auto& system : fSystems) {
    if (!fDigiMan->IsPresent(system)) {
      LOG(fatal) << GetName() << ": No digi branch for " << CbmModuleList::GetModuleNameCaps(system);
      return kFATAL;
    }
    LOG(info) << "--- Found digi branch for " << CbmModuleList::GetModuleNameCaps(system);
  }

  // --- Register output array (Triggers)
  if (ioman->GetObject("Trigger")) {
    LOG(fatal) << GetName() << ": Branch Trigger already exists!";
    return kFATAL;
  }
  fTriggers = new vector<double>;
  ioman->RegisterAny("Trigger", fTriggers, IsOutputBranchPersistent("Trigger"));
  if (!fTriggers) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }
  LOG(info) << "--- Registered branch Trigger";

  LOG(info) << "--- Use algo TimeClusterTrigger with TriggerWin " << fTriggerWindow << " ns, threshold " << fMinNumDigis
            << ", dead time " << fDeadTime << " ns";
  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskTriggerDigi)
