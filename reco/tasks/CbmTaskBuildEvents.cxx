/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmTaskBuildEvents.h"

#include "CbmDigiBranchBase.h"
#include "CbmDigiManager.h"
#include "CbmDigiTimeslice.h"
#include "CbmModuleList.h"

#include <FairLogger.h>

#include <TStopwatch.h>

#include <cassert>
#include <iomanip>
#include <iostream>

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


// ------ Construct a DigiTimeslice from the data in CbmDigiManager ----------
CbmDigiTimeslice CbmTaskBuildEvents::FillTimeSlice()
{
  CbmDigiTimeslice ts;
  for (const auto& system : fSystems) {
    CbmDigiBranchBase* digiBranch = fDigiMan->GetBranch(system);
    switch (system) {
      case ECbmModuleId::kMuch: {  //we do not support the "MuchBeamTimeDigi"
        const vector<CbmMuchDigi>* digiVec =
          boost::any_cast<const vector<CbmMuchDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fMuch.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kSts: {
        const vector<CbmStsDigi>* digiVec =
          boost::any_cast<const vector<CbmStsDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fSts.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kTof: {
        const vector<CbmTofDigi>* digiVec =
          boost::any_cast<const vector<CbmTofDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fTof.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kTrd: {
        const vector<CbmTrdDigi>* digiVec =
          boost::any_cast<const vector<CbmTrdDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fTrd.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kRich: {
        const vector<CbmRichDigi>* digiVec =
          boost::any_cast<const vector<CbmRichDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fRich.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kPsd: {
        const vector<CbmPsdDigi>* digiVec =
          boost::any_cast<const vector<CbmPsdDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fPsd.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kT0: {  //T0 has Tof digis
        const vector<CbmTofDigi>* digiVec =
          boost::any_cast<const vector<CbmTofDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fT0.fDigis = *digiVec;
        break;
      }
      default: LOG(fatal) << GetName() << ": Reading digis from unknown detector type!";
    }
  }
  return ts;
}

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
  CbmDigiTimeslice ts = FillTimeSlice();
  timerStep.Stop();
  fTimeFillTs += timerStep.RealTime();

  // --- Call event builder algorithm
  timerStep.Start();
  *fEvents = fAlgo(ts, *fTriggers);
  timerStep.Stop();
  fTimeBuildEvt += timerStep.RealTime();

  // --- Timeslice statistics
  size_t numTriggers = fTriggers->size();
  size_t numEvents   = fEvents->size();
  std::map<ECbmModuleId, size_t> NumDigisTs;
  std::map<ECbmModuleId, size_t> NumDigisEv;

  for (const auto& system : fSystems) {
    NumDigisEv[system] = 0;
    switch (system) {
      case ECbmModuleId::kMuch: {
        NumDigisTs[system] = ts.fData.fMuch.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fMuch.fDigis.size();
        break;
      }
      case ECbmModuleId::kSts: {
        NumDigisTs[system] = ts.fData.fSts.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fSts.fDigis.size();
        break;
      }
      case ECbmModuleId::kTof: {
        NumDigisTs[system] = ts.fData.fTof.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fTof.fDigis.size();
        break;
      }
      case ECbmModuleId::kTrd: {
        NumDigisTs[system] = ts.fData.fTrd.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fTrd.fDigis.size();
        break;
      }
      case ECbmModuleId::kRich: {
        NumDigisTs[system] = ts.fData.fRich.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fRich.fDigis.size();
        break;
      }
      case ECbmModuleId::kPsd: {
        NumDigisTs[system] = ts.fData.fPsd.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fPsd.fDigis.size();
        break;
      }
      case ECbmModuleId::kT0: {
        NumDigisTs[system] = ts.fData.fT0.fDigis.size();
        for (auto& event : (*fEvents))
          NumDigisEv[system] += event.fData.fT0.fDigis.size();
        break;
      }
      default: break;
    }
  }

  // --- Timeslice log
  timerTot.Stop();
  fTimeTot += timerTot.RealTime();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTot.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", triggers " << numTriggers << ", events " << numEvents;

  for (const auto& system : fSystems) {
    logOut << ", frac " << CbmModuleList::GetModuleNameCaps(system) << " digis "
           << 100. * double(NumDigisEv[system]) / double(NumDigisTs[system]) << " %";
  }

  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumTriggers += numTriggers;
  fNumEvents += numEvents;

  for (const auto& system : fSystems) {
    fNumDigisTs[system] += NumDigisTs[system];
    fNumDigisEv[system] += NumDigisEv[system];
  }
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

  for (const auto& system : fSystems) {
    LOG(info) << CbmModuleList::GetModuleNameCaps(system) << " digis in timeslice : " << fNumDigisTs[system];
    LOG(info) << CbmModuleList::GetModuleNameCaps(system) << " digis in events    : " << fNumDigisEv[system] << " = "
              << fixed << setprecision(2) << 100. * double(fNumDigisEv[system]) / double(fNumDigisTs[system]) << " %";
  }

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

  // --- Check input data
  for (const auto& system : fSystems) {
    if (!fDigiMan->IsPresent(system)) {
      LOG(fatal) << GetName() << ": No digi branch for " << CbmModuleList::GetModuleNameCaps(system);
      return kFATAL;
    }
    LOG(info) << "--- Found digi branch for " << CbmModuleList::GetModuleNameCaps(system);
  }

  // --- Initialize diagnostics
  for (const auto& system : fSystems) {
    fNumDigisTs[system] = 0;
    fNumDigisEv[system] = 0;
  }

  // --- Get input data (triggers)
  fTriggers = ioman->InitObjectAs<std::vector<double> const*>("Trigger");
  if (!fTriggers) {
    LOG(fatal) << GetName() << ": No Trigger branch!" << endl;
    return kFATAL;
  }
  LOG(info) << "--- Found branch Trigger";

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
  LOG(info) << "--- Registered branch DigiEvent";

  // --- Configure algorithm
  for (const auto& system : fSystems) {
    if (fTriggerWindows.find(system) == fTriggerWindows.end()) {
      LOG(fatal) << GetName() << ": no trigger window supplied for requested detector.";
    }
    const double tMin = fTriggerWindows.find(system)->second.first;
    const double tMax = fTriggerWindows.find(system)->second.second;
    fAlgo.SetTriggerWindow(system, tMin, tMax);
    LOG(info) << "--- Use algo EventBuilder with event window [" << tMin << ", " << tMax << "] ns for "
              << CbmModuleList::GetModuleNameCaps(system);
  }

  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskBuildEvents)
