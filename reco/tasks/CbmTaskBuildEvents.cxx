/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmTaskBuildEvents.h"

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiEvent.h"
#include "CbmDigiManager.h"
#include "CbmDigiTimeslice.h"

#include <FairLogger.h>

#include <TStopwatch.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include "EventBuilder.h"


using namespace std;


// -----   Constructor   -----------------------------------------------------
CbmTaskBuildEvents::CbmTaskBuildEvents() : FairTask("BuildEvents") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskBuildEvents::~CbmTaskBuildEvents()
{
  if (fEvents) delete fEvents;
}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskBuildEvents::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timerStep;
  TStopwatch timerTot;
  timerTot.Start();

  // --- Clear output vector
  fEvents->clear();

  // --- Construct a DigiTimeslice from the data in CbmDigiManager
  timerStep.Start();
  CbmDigiTimeslice ts;
  CbmDigiBranchBase* stsBranch      = fDigiMan->GetBranch(ECbmModuleId::kSts);
  const vector<CbmStsDigi>* digiVec = boost::any_cast<const vector<CbmStsDigi>*>(stsBranch->GetBranchContainer());
  assert(digiVec);
  ts.fData.fSts.fDigis = *digiVec;
  timerStep.Stop();
  fTimeFillTs += timerStep.RealTime();

  // --- Construct an artificial trigger list (just preliminary)
  vector<double> triggerVec {1000., 2000., 3000.};

  // --- Call event builder algorithm
  timerStep.Start();
  *fEvents = fAlgo(ts, triggerVec);
  timerStep.Stop();
  fTimeBuildEvt += timerStep.RealTime();

  // --- Timeslice statistics
  size_t numTriggers   = triggerVec.size();
  size_t numEvents     = fEvents->size();
  size_t numDigisStsTs = ts.fData.fSts.fDigis.size();
  size_t numDigisStsEv = 0;
  for (auto& event : (*fEvents))
    numDigisStsEv += event.fData.fSts.fDigis.size();

  // --- Timeslice log
  timerTot.Stop();
  fTimeTot += timerTot.RealTime();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTot.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", triggers " << numTriggers << ", events " << numEvents;
  logOut << ", frac digis " << 100. * double(numDigisStsEv) / double(numDigisStsTs) << " %";
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumTriggers += numTriggers;
  fNumEvents += numEvents;
  fNumDigisStsTs += numDigisStsTs;
  fNumDigisStsEv += numDigisStsEv;
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskBuildEvents::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices         : " << fNumTs;
  LOG(info) << "Triggers           : " << fNumTriggers;
  LOG(info) << "Events             : " << fNumEvents;
  LOG(info) << "Digis in timeslice : " << fNumDigisStsTs;
  LOG(info) << "Digis in events    : " << fNumDigisStsEv << " = " << fixed << setprecision(2)
            << 100. * double(fNumDigisStsEv) / double(fNumDigisStsTs) << " %";
  LOG(info) << "Time  / TS         : " << fixed << setprecision(2) << 1000. * fTimeTot / double(fNumTs) << " ms";
  LOG(info) << "Time fill TS       : " << fixed << setprecision(2) << 1000. * fTimeFillTs / double(fNumTs)
            << " ms = " << 100. * fTimeFillTs / fTimeTot << " %";
  LOG(info) << "Time build events  : " << fixed << setprecision(2) << 1000. * fTimeBuildEvt / double(fNumTs)
            << " ms = " << 100. * fTimeBuildEvt / fTimeTot << " %";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskBuildEvents::Init()
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

  // --- Register output array (CbmDigiEvent)
  if (ioman->GetObject("DigiEvent")) {
    LOG(fatal) << GetName() << ": Branch DigiEvent already exists!";
    return kFATAL;
  }
  fEvents = new vector<CbmDigiEvent>;
  ioman->RegisterAny("DigiEvent", fEvents, IsOutputBranchPersistent("DigiEvent"));
  if (!fEvents) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }

  // --- Configure algorithm
  fAlgo.SetTriggerWindow(ECbmModuleId::kSts, fEvtTimeStsMin, fEvtTimeStsMax);


  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskBuildEvents)
