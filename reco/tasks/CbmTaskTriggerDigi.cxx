/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmTaskTriggerDigi.h"

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiManager.h"
#include "CbmStsDigi.h"

#include <FairLogger.h>

#include "TimeClusterTrigger.h"
#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>


using namespace std;


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

  // --- Get input digi vector
  CbmDigiBranchBase* stsBranch      = fDigiMan->GetBranch(ECbmModuleId::kSts);
  const vector<CbmStsDigi>* digiVec = boost::any_cast<const vector<CbmStsDigi>*>(stsBranch->GetBranchContainer());
  assert(digiVec);

  // --- Extract digi times into to a vector
  timerStep.Start();
  std::vector<double> digiTimes(digiVec->size());
  std::transform(digiVec->begin(), digiVec->end(), digiTimes.begin(),
                 [](const CbmStsDigi& digi) { return digi.GetTime(); });
  timerStep.Stop();
  fTimeExtract += timerStep.RealTime();

  // --- Call the trigger algorithm
  timerStep.Start();
  *fTriggers = fAlgo(digiTimes, fTriggerWindow, fMinNumDigis, fDeadTime);
  timerStep.Stop();
  fTimeFind += timerStep.RealTime();

  // --- Timeslice statistics
  size_t numDigis    = digiVec->size();
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

  // --- Check input data (STS only for the time being)
  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(fatal) << GetName() << ": No digi branch for STS";
    return kFATAL;
  }
  LOG(info) << "--- Found branch STS digi";

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
