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
  for (const auto& entry : fEventWindows) {
    auto system                   = entry.first;
    CbmDigiBranchBase* digiBranch = fDigiMan->GetBranch(system);
    switch (system) {
      case ECbmModuleId::kSts: {
        const vector<CbmStsDigi>* digiVec =
          boost::any_cast<const vector<CbmStsDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fSts.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kRich: {
        const vector<CbmRichDigi>* digiVec =
          boost::any_cast<const vector<CbmRichDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fRich.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kMuch: {
        const vector<CbmMuchDigi>* digiVec =
          boost::any_cast<const vector<CbmMuchDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fMuch.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kTrd: {
        const vector<CbmTrdDigi>* digiVec =
          boost::any_cast<const vector<CbmTrdDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fTrd.fDigis = *digiVec;
        break;
      }
      case ECbmModuleId::kTof: {
        const vector<CbmTofDigi>* digiVec =
          boost::any_cast<const vector<CbmTofDigi>*>(digiBranch->GetBranchContainer());
        assert(digiVec);
        ts.fData.fTof.fDigis = *digiVec;
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
      default: LOG(fatal) << GetName() << ": Unknown detector type!";
    }
  }
  return ts;
}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskBuildEvents::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timerStep;
  TStopwatch timerTot;
  timerTot.Start();
  std::map<ECbmModuleId, size_t> numDigisTs;
  std::map<ECbmModuleId, size_t> numDigisEv;

  // --- Clear output vector
  fEvents->clear();

  // --- If the input is already CbmDigiTimeslice (from unpacking), use that directly
  if (fTimeslice) {
    timerStep.Start();
    *fEvents = fAlgo(*fTimeslice, *fTriggers);
    timerStep.Stop();
    fTimeBuildEvt += timerStep.RealTime();
    for (const auto& entry : fEventWindows)
      numDigisTs[entry.first] = GetNumDigis(fTimeslice->fData, entry.first);
  }

  // --- If input is not DigiTimeslice: construct a transientDigiTimeslice from the data in CbmDigiManager
  else {
    timerStep.Start();
    CbmDigiTimeslice ts = FillTimeSlice();
    for (const auto& entry : fEventWindows)
      numDigisTs[entry.first] = GetNumDigis(ts.fData, entry.first);
    timerStep.Stop();
    fTimeFillTs += timerStep.RealTime();
    timerStep.Start();
    *fEvents = fAlgo(ts, *fTriggers);
    timerStep.Stop();
    fTimeBuildEvt += timerStep.RealTime();
  }

  // --- Timeslice statistics
  size_t numTriggers = fTriggers->size();
  size_t numEvents   = fEvents->size();
  for (const auto& entry : fEventWindows) {
    for (auto& event : (*fEvents)) {
      numDigisEv[entry.first] += GetNumDigis(event.fData, entry.first);
    }
  }

  // --- Timeslice log
  timerTot.Stop();
  fTimeTot += timerTot.RealTime();
  stringstream logOut;
  logOut << setw(15) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTot.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", triggers " << numTriggers << ", events " << numEvents;

  for (const auto& entry : fEventWindows) {
    auto system = entry.first;
    logOut << ", frac " << CbmModuleList::GetModuleNameCaps(system) << " digis "
           << 100. * double(numDigisEv[system]) / double(numDigisTs[system]) << " %";
  }

  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fNumTriggers += numTriggers;
  fNumEvents += numEvents;

  for (const auto& entry : fEventWindows) {
    auto system = entry.first;
    fNumDigisTs[system] += numDigisTs[system];
    fNumDigisEv[system] += numDigisEv[system];
  }
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskBuildEvents::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices           : " << fNumTs;
  LOG(info) << "Triggers             : " << fNumTriggers;
  LOG(info) << "Events               : " << fNumEvents;
  for (const auto& entry : fEventWindows) {
    auto system = entry.first;
    LOG(info) << setw(4) << left << CbmModuleList::GetModuleNameCaps(system)
              << " digis in TS     : " << fNumDigisTs[system];
    LOG(info) << setw(4) << left << CbmModuleList::GetModuleNameCaps(system)
              << " digis in events : " << fNumDigisEv[system] << " = " << fixed << setprecision(2)
              << 100. * double(fNumDigisEv[system]) / double(fNumDigisTs[system]) << " %";
  }
  LOG(info) << "Time  / TS           : " << fixed << setprecision(2) << 1000. * fTimeTot / double(fNumTs) << " ms";
  LOG(info) << "Time fill TS         : " << fixed << setprecision(2) << 1000. * fTimeFillTs / double(fNumTs)
            << " ms = " << 100. * fTimeFillTs / fTimeTot << " %";
  LOG(info) << "Time build events    : " << fixed << setprecision(2) << 1000. * fTimeBuildEvt / double(fNumTs)
            << " ms = " << 100. * fTimeBuildEvt / fTimeTot << " %";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----  Number of digis in the timeslice   ----------------------------------
size_t CbmTaskBuildEvents::GetNumDigis(const CbmDigiData& data, ECbmModuleId system)
{
  size_t result = 0;
  switch (system) {
    case ECbmModuleId::kSts: result = data.fSts.fDigis.size(); break;
    case ECbmModuleId::kRich: result = data.fRich.fDigis.size(); break;
    case ECbmModuleId::kMuch: result = data.fMuch.fDigis.size(); break;
    case ECbmModuleId::kTrd: result = data.fTrd.fDigis.size(); break;
    case ECbmModuleId::kTof: result = data.fTof.fDigis.size(); break;
    case ECbmModuleId::kPsd: result = data.fPsd.fDigis.size(); break;
    case ECbmModuleId::kT0: result = data.fT0.fDigis.size(); break;
    default: result = 0; break;
  }
  return result;
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskBuildEvents::Init()
{
  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  std::cout << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";

  // --- Check input data
  // --- DigiTimeslice: Unpacked data from FLES
  fTimeslice = ioman->InitObjectAs<const CbmDigiTimeslice*>("DigiTimeslice.");
  if (fTimeslice) { LOG(info) << "--- Found branch DigiTimeslice."; }
  // --- DigiManager: Simulated digi data
  else {
    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();
    for (const auto& entry : fEventWindows) {
      auto system = entry.first;
      if (!fDigiMan->IsPresent(system)) {
        LOG(fatal) << GetName() << ": No digi branch for " << CbmModuleList::GetModuleNameCaps(system);
        return kFATAL;
      }
      LOG(info) << "--- Found digi branch for " << CbmModuleList::GetModuleNameCaps(system);
    }
  }

  // --- Initialize diagnostics
  for (const auto& entry : fEventWindows) {
    auto system         = entry.first;
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
  for (const auto& entry : fEventWindows) {
    auto system       = entry.first;
    const double tMin = entry.second.first;
    const double tMax = entry.second.second;
    fAlgo.SetEventWindow(system, tMin, tMax);
    LOG(info) << "--- Use algo EventBuilder with event window [" << tMin << ", " << tMax << "] ns for "
              << CbmModuleList::GetModuleNameCaps(system);
  }

  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskBuildEvents)
