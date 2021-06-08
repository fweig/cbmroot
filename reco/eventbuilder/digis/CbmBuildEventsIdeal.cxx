/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsBuildEventsIdeal.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.09.2016
 **/

#include "CbmBuildEventsIdeal.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"

#include <Logger.h>

#include <TClonesArray.h>
#include <TStopwatch.h>

#include <cassert>
#include <iomanip>
#include <iostream>

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsIdeal::CbmBuildEventsIdeal() : FairTask("BuildEventsIdeal") {}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsIdeal::~CbmBuildEventsIdeal() {}
// ===========================================================================


// =====   Number if different pairs (input,event) in a match object   =======
UInt_t CbmBuildEventsIdeal::EventsInMatch(const CbmMatch* match)
{

  // --- No or empty match object
  if (match == nullptr) return 0;
  if (!match->GetNofLinks()) return 0;

  // --- Only one link
  if (match->GetNofLinks() == 1) return 1;

  // --- More than one link: check whether they are from different events
  CbmMatch testMatch;
  for (Int_t iLink = 0; iLink < match->GetNofLinks(); iLink++) {
    Int_t input = match->GetLink(iLink).GetFile();
    Int_t event = match->GetLink(iLink).GetEntry();
    testMatch.AddLink(1., 0, event, input);
  }
  return testMatch.GetNofLinks();
}
// ===========================================================================


// =====   Task execution   ==================================================
void CbmBuildEventsIdeal::Exec(Option_t*)
{

  // --- Timer and counters
  TStopwatch timer;
  timer.Start();
  Int_t nEvents      = 0;  // Number of created events
  UInt_t nDigisTot   = 0;  // Total number of digis
  UInt_t nDigisNoise = 0;  // Digis without link to MC event
  UInt_t nDigisAmbig = 0;  // Digis with links to multiple MC events

  // --- Clear output array
  fEvents->Delete();

  // --- Bookkeeping: Map from (input number, event number) to event
  map<pair<Int_t, Int_t>, CbmEvent*> eventMap;

  // --- Loop over all detector systems
  for (ECbmModuleId& system : fSystems) {

    // --- Skip system if no data branch or no match match present
    if (!fDigiMan->IsPresent(system)) continue;
    if (!fDigiMan->IsMatchPresent(system)) continue;

    // --- Loop over digis for the current system
    Int_t nDigis  = fDigiMan->GetNofDigis(system);
    UInt_t nNoise = 0;
    UInt_t nAmbig = 0;
    for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {

      // --- Get the MC input and event number through the match object
      const CbmMatch* match = fDigiMan->GetMatch(system, iDigi);
      assert(match);
      Int_t mcInput = -1;
      Int_t mcEvent = -1;
      if (match->GetNofLinks()) {
        mcInput = match->GetMatchedLink().GetFile();
        mcEvent = match->GetMatchedLink().GetEntry();
      }

      // --- Empty match objects or negative event numbers signal noise
      if (mcInput < 0 || mcEvent < 0) {
        nNoise++;
        continue;
      }

      // --- Count occurrences of multiple MC events in match
      if (EventsInMatch(match) > 1) nAmbig++;

      // --- Get event pointer. If event is not yet present, create it.
      CbmEvent* event = nullptr;
      auto it         = eventMap.find(make_pair(mcInput, mcEvent));
      if (it == eventMap.end()) {
        assert(nEvents == fEvents->GetEntriesFast());
        event                                 = new ((*fEvents)[nEvents]) CbmEvent(nEvents);
        eventMap[make_pair(mcInput, mcEvent)] = event;
        nEvents++;
      }
      else
        event = it->second;

      // --- Fill digi index into event
      switch (system) {
        case ECbmModuleId::kMvd: event->AddData(ECbmDataType::kMvdDigi, iDigi); break;
        case ECbmModuleId::kSts: event->AddData(ECbmDataType::kStsDigi, iDigi); break;
        case ECbmModuleId::kRich: event->AddData(ECbmDataType::kRichDigi, iDigi); break;
        case ECbmModuleId::kMuch: event->AddData(ECbmDataType::kMuchDigi, iDigi); break;
        case ECbmModuleId::kTrd: event->AddData(ECbmDataType::kTrdDigi, iDigi); break;
        case ECbmModuleId::kTof: event->AddData(ECbmDataType::kTofDigi, iDigi); break;
        case ECbmModuleId::kPsd: event->AddData(ECbmDataType::kPsdDigi, iDigi); break;
        default: break;
      }  //? detector

    }  //# digis
    LOG(debug) << GetName() << ": Detector " << CbmModuleList::GetModuleNameCaps(system) << ", digis " << nDigis << " ("
               << nAmbig << " ambiguous), noise " << nNoise;
    nDigisTot += nDigis;
    nDigisAmbig += nAmbig;
    nDigisNoise += nNoise;

  }  //# detector systems

  assert(nEvents == fEvents->GetEntriesFast());

  // --- Timeslice log and statistics
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << "] ms: ";
  logOut << "TS " << fNofEntries;
  if (fEvents) logOut << ", events " << nEvents;
  logOut << ", digis " << nDigisTot << " (" << nDigisAmbig << " ambiguous), noise: " << nDigisNoise;
  LOG(info) << logOut.str();
  fNofEntries++;
  fNofEvents += nEvents;
  fNofDigisTotal += nDigisTot;
  fNofDigisAmbig += nDigisAmbig;
  fNofDigisNoise += nDigisNoise;
  fTime += timer.RealTime();

  // --- For debug: event info
  if (fair::Logger::Logging(fair::Severity::debug)) {
    for (Int_t iEvent = 0; iEvent < fEvents->GetEntriesFast(); iEvent++) {
      CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);
      LOG(info) << event->ToString();
    }
  }
}
// ===========================================================================


// =====   End-of-timeslice action   =========================================
void CbmBuildEventsIdeal::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices          : " << fNofEntries;
  LOG(info) << "All digis       / TS : " << fixed << setprecision(2) << fNofDigisTotal / Double_t(fNofEntries);
  LOG(info) << "Ambiguous digis / TS : " << fixed << setprecision(2) << fNofDigisAmbig / Double_t(fNofEntries) << " = "
            << 100. * fNofDigisAmbig / fNofDigisTotal << " %";
  LOG(info) << "Noise digis     / TS : " << fixed << setprecision(2) << fNofDigisNoise / Double_t(fNofEntries) << " = "
            << 100. * fNofDigisNoise / fNofDigisTotal << " %";
  LOG(info) << "Events               : " << fNofEvents;
  LOG(info) << "Time  / TS           : " << fixed << setprecision(2) << 1000. * fTime / Double_t(fNofEntries) << " ms";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsIdeal::Init()
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
  for (ECbmModuleId system = ECbmModuleId::kMvd; system < ECbmModuleId::kNofSystems; ++system) {
    if (fDigiMan->IsMatchPresent(system)) {
      LOG(info) << GetName() << ": Found match branch for " << CbmModuleList::GetModuleNameCaps(system);
      fSystems.push_back(system);
    }
  }
  if (fSystems.empty()) {
    LOG(fatal) << GetName() << ": No match branch found!";
    return kFATAL;
  }

  // Register output array (CbmEvent)
  if (ioman->GetObject("CbmEvent")) {
    LOG(fatal) << GetName() << ": Branch CbmEvent already exists!";
    return kFATAL;
  }
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register("CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));
  if (!fEvents) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }

  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ===========================================================================


ClassImp(CbmBuildEventsIdeal)
