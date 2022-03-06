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
  fT0Digis->clear();
  fStsDigis->clear();
  fRichDigis->clear();
  fMuchDigis->clear();
  fTrdDigis->clear();
  fTofDigis->clear();
  fPsdDigis->clear();

  //fRecoEvents->Clear(); //causes memory leak
  fRecoEvents->Delete();

  // --- Event loop
  Int_t eventNr = 0;
  for (auto& digiEvent : *fDigiEvents) {

    // --- Create CbmEvent object
    CbmEvent* recoEvent = new ((*fRecoEvents)[eventNr]) CbmEvent(eventNr);

    // --- Copy T0 digis
    FillTree<CbmTofDigi>(digiEvent.fData.fT0.fDigis, fT0Digis, recoEvent, ECbmDataType::kT0Digi);

    // --- Copy STS digis
    FillTree<CbmStsDigi>(digiEvent.fData.fSts.fDigis, fStsDigis, recoEvent, ECbmDataType::kStsDigi);

    // --- Copy RICH digis
    FillTree<CbmRichDigi>(digiEvent.fData.fRich.fDigis, fRichDigis, recoEvent, ECbmDataType::kRichDigi);

    // --- Copy MUCH digis
    FillTree<CbmMuchDigi>(digiEvent.fData.fMuch.fDigis, fMuchDigis, recoEvent, ECbmDataType::kMuchDigi);

    // --- Copy TRD digis
    FillTree<CbmTrdDigi>(digiEvent.fData.fTrd.fDigis, fTrdDigis, recoEvent, ECbmDataType::kTrdDigi);

    // --- Copy TOF digis
    FillTree<CbmTofDigi>(digiEvent.fData.fTof.fDigis, fTofDigis, recoEvent, ECbmDataType::kTofDigi);

    // --- Copy PSD digis
    FillTree<CbmPsdDigi>(digiEvent.fData.fPsd.fDigis, fPsdDigis, recoEvent, ECbmDataType::kPsdDigi);

    eventNr++;
  }  //# events

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", events " << fDigiEvents->size() << ", Digis: T0 " << fT0Digis->size() << " STS "
         << fStsDigis->size() << " RICH " << fRichDigis->size() << " MUCH " << fMuchDigis->size() << " TRD "
         << fTrdDigis->size() << " TOF " << fTofDigis->size() << " PSD" << fPsdDigis->size();
  LOG(info) << logOut.str();

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

  // --- If DigiEvents are present, create Digi and CbmEvent branches
  if (fDigiEvents) {
    LOG(info) << GetName() << ": Found branch DigiEvent";

    // --- Event
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

    // --- T0 digis
    fT0Digis = new std::vector<CbmTofDigi>;
    frm->RegisterAny("T0Digi", fT0Digis, kFALSE);

    // --- STS digis
    fStsDigis = new std::vector<CbmStsDigi>;
    frm->RegisterAny("StsDigi", fStsDigis, kFALSE);

    // --- RICH digis
    fRichDigis = new std::vector<CbmRichDigi>;
    frm->RegisterAny("RichDigi", fRichDigis, kFALSE);

    // --- MUCH digis
    fMuchDigis = new std::vector<CbmMuchDigi>;
    frm->RegisterAny("MuchDigi", fMuchDigis, kFALSE);

    // --- TRD digis
    fTrdDigis = new std::vector<CbmTrdDigi>;
    frm->RegisterAny("TrdDigi", fTrdDigis, kFALSE);

    // --- TOF digis
    fTofDigis = new std::vector<CbmTofDigi>;
    frm->RegisterAny("TofDigi", fTofDigis, kFALSE);

    // --- PSD digis
    fPsdDigis = new std::vector<CbmPsdDigi>;
    frm->RegisterAny("PsdDigi", fPsdDigis, kFALSE);
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
