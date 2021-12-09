/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#include "CbmTaskMakeRecoEvents.h"

#include "CbmDefs.h"
#include "CbmEvent.h"

#include <FairLogger.h>

#include <TClonesArray.h>
#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>


using namespace std;


// -----   Constructor   -----------------------------------------------------
CbmTaskMakeRecoEvents::CbmTaskMakeRecoEvents() : FairTask("MakeRecoEvents") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskMakeRecoEvents::~CbmTaskMakeRecoEvents() {}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskMakeRecoEvents::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timer;
  timer.Start();

  // --- No action if no DigiEvent branch is present
  if (!fDigiEvents) return;

  // --- Clear output arrays
  fStsDigis->clear();
  fRecoEvents->Clear();


  // --- Event loop
  Int_t eventNr = 0;
  for (auto& digiEvent : *fDigiEvents) {

    // --- Copy StsDigis
    size_t startIndex = fStsDigis->size();
    fStsDigis->insert(fStsDigis->end(), digiEvent.fData.fSts.fDigis.begin(), digiEvent.fData.fSts.fDigis.end());
    size_t stopIndex = startIndex + digiEvent.fData.fSts.fDigis.size();

    // --- Create and fill CbmEvent object
    CbmEvent* recoEvent = new ((*fRecoEvents)[eventNr]) CbmEvent(eventNr);
    for (size_t index = startIndex; index < stopIndex; index++) {
      recoEvent->AddData(ECbmDataType::kStsDigi, index);
    }

    eventNr++;
  }  //# events

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", events " << fDigiEvents->size() << ", STS digis " << fStsDigis->size();
  LOG(info) << logOut.str();
  assert(fDigiEvents->size() == static_cast<size_t>(fRecoEvents->GetEntriesFast()));

  // --- Run statistics
  fNumTs++;
  fTimeTot += timer.RealTime();
  assert(fDigiEvents->size() == static_cast<size_t>(fRecoEvents->GetEntriesFast()));
  fNumEvents += fDigiEvents->size();
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskMakeRecoEvents::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices         : " << fNumTs;
  LOG(info) << "Events             : " << fNumEvents;
  LOG(info) << "Time  / TS         : " << fixed << setprecision(2) << 1000. * fTimeTot / double(fNumTs) << " ms";
  LOG(info) << "=====================================";
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskMakeRecoEvents::Init()
{

  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising ";

  // --- Get FairRootManager instance
  FairRootManager* frm = FairRootManager::Instance();
  assert(frm);

  // --- Try to get input vector (CbmDigiEvent)
  fDigiEvents = frm->InitObjectAs<const std::vector<CbmDigiEvent>*>("DigiEvent");

  // --- If DigiEvents are present, create StsDigi and CbmEvent branches
  if (fDigiEvents) {
    LOG(info) << GetName() << ": Found branch DigiEvent";
    if (frm->GetObject("CbmEvent")) {
      LOG(error) << GetName() << ": Found branch CbmEvent! Aborting...";
      return kFATAL;
    }
    fRecoEvents = new TClonesArray("CbmEvent", 1);
    frm->Register("CbmEvent", "Reco events", fRecoEvents, IsOutputBranchPersistent("CbmEvent"));
    if (frm->GetObject("StsDigi")) {
      LOG(error) << GetName() << ": Found branch StsDigi! Aborting...";
      return kFATAL;
    }
    fStsDigis = new std::vector<CbmStsDigi>;
    frm->RegisterAny("StsDigi", fStsDigis, kFALSE);
  }

  // --- If no DigiEvent branch is present, there must be a CbmEvent branch
  else {
    fRecoEvents = dynamic_cast<TClonesArray*>(frm->GetObject("CbmEvent"));
    if (fRecoEvents == nullptr) {
      LOG(error) << GetName() << ": Neither DigiEvent nor CbmEvent branch found! Aborting...";
      return kFATAL;
    }
  }

  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskMakeRecoEvents)
