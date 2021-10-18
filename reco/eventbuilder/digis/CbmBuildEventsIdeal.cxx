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
CbmMatch CbmBuildEventsIdeal::EventsInMatch(const CbmMatch* match)
{
  // --- Collect links from different events

  CbmMatch eventMatch;
  Int_t nLinks = (match != nullptr) ? match->GetNofLinks() : 0;

  for (Int_t iLink = 0; iLink < nLinks; iLink++) {
    Int_t mcInput = match->GetLink(iLink).GetFile();
    Int_t mcEvent = match->GetLink(iLink).GetEntry();
    if (mcInput < 0 || mcEvent < 0) { continue; }
    eventMatch.AddLink(1., 0, mcEvent, mcInput);
  }
  return eventMatch;
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
  map<pair<Int_t, Int_t>, CbmDigiEvent*> digiEventMap;

  // --- Loop over all detector systems
  for (ECbmModuleId& system : fSystems) {

    // --- Skip system if no data branch or no match match present
    if (!fDigiMan->IsPresent(system)) continue;
    if (!fDigiMan->IsMatchPresent(system)) continue;

    ECbmDataType digiType = ECbmDataType::kUnknown;

    // --- Find the digi type
    switch (system) {
      case ECbmModuleId::kMvd: digiType = ECbmDataType::kMvdDigi; break;
      case ECbmModuleId::kSts: digiType = ECbmDataType::kStsDigi; break;
      case ECbmModuleId::kRich: digiType = ECbmDataType::kRichDigi; break;
      case ECbmModuleId::kMuch: digiType = ECbmDataType::kMuchDigi; break;
      case ECbmModuleId::kTrd: digiType = ECbmDataType::kTrdDigi; break;
      case ECbmModuleId::kTof: digiType = ECbmDataType::kTofDigi; break;
      case ECbmModuleId::kPsd: digiType = ECbmDataType::kPsdDigi; break;
      default: break;
    }  //? detector

    if (digiType == ECbmDataType::kUnknown) {
      LOG(fatal) << "unknown type of the module";
      assert(0);
      continue;  // in case assert(..) macro is switched off
    }

    // --- Loop over digis for the current system
    Int_t nDigis  = fDigiMan->GetNofDigis(system);
    UInt_t nNoise = 0;
    UInt_t nAmbig = 0;
    for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {

      // --- Get the MC input and event numbers through the match object

      CbmMatch matchedEvents = EventsInMatch(fDigiMan->GetMatch(system, iDigi));

      for (Int_t iLink = 0; iLink < matchedEvents.GetNofLinks(); iLink++) {
        const auto& link = matchedEvents.GetLink(iLink);
        auto eventID     = make_pair(link.GetFile(), link.GetEntry());

        // --- Get event pointer. If event is not yet present, create it.
        CbmEvent* event = nullptr;
        auto it         = eventMap.find(eventID);
        if (it == eventMap.end()) {
          event             = new CbmEvent();
          eventMap[eventID] = event;
        }
        else {
          event = it->second;
        }
        event->AddData(digiType, iDigi);

        // --- Get event pointer. If event is not yet present, create it.
        CbmDigiEvent* digiEvent = nullptr;
        auto it2                = digiEventMap.find(eventID);
        if (it2 == digiEventMap.end()) {
          digiEvent             = new CbmDigiEvent();
          digiEventMap[eventID] = digiEvent;
        }
        else {
          digiEvent = it2->second;
        }
        if (system == ECbmModuleId::kSts) {
          digiEvent->fData.fSts.fDigis.push_back(*(fDigiMan->Get<CbmStsDigi>(iDigi)));
        }


      }  //# links

      // --- Empty match objects or negative event numbers signal noise
      if (matchedEvents.GetNofLinks() == 0) { nNoise++; }

      // --- Count occurrences of multiple MC events in match
      if (matchedEvents.GetNofLinks() > 1) { nAmbig++; }

    }  //# digis
    LOG(debug) << GetName() << ": Detector " << CbmModuleList::GetModuleNameCaps(system) << ", digis " << nDigis << " ("
               << nAmbig << " ambiguous), noise " << nNoise;
    nDigisTot += nDigis;
    nDigisAmbig += nAmbig;
    nDigisNoise += nNoise;

  }  //# detector systems

  // events are already ordered in the map, create CbmEvents in the same order
  for (auto it = eventMap.begin(); it != eventMap.end(); it++) {
    assert(nEvents == fEvents->GetEntriesFast());
    CbmEvent* store = new ((*fEvents)[nEvents]) CbmEvent();
    store->Swap(*(it->second));
    store->SetNumber(nEvents++);
    delete it->second;
    it->second = nullptr;  // for a case
  }

  // Store CbmDigiEvent
  for (auto it = digiEventMap.begin(); it != digiEventMap.end(); it++) {
    fDigiEvents->push_back(*(it->second));
    delete it->second;
    it->second = nullptr;
  }


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
  // TODO: This shall be removed once reconstruction from DigiEvent is established.
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

  // Register output array (CbmDigiEvent)
  if (ioman->GetObject("DigiEvent")) {
    LOG(fatal) << GetName() << ": Branch DigiEvent already exists!";
    return kFATAL;
  }
  fDigiEvents = new std::vector<CbmDigiEvent>;
  ioman->RegisterAny("DigiEvent", fDigiEvents, IsOutputBranchPersistent("DigiEvent"));
  if (!fDigiEvents) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }


  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ===========================================================================


ClassImp(CbmBuildEventsIdeal)
