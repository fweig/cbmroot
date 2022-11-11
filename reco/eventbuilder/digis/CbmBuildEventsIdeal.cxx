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
#include "CbmMCEventList.h"
#include "CbmModuleList.h"
#include "CbmTimeSlice.h"

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
  int32_t nLinks = (match != nullptr) ? match->GetNofLinks() : 0;

  for (int32_t iLink = 0; iLink < nLinks; iLink++) {
    int32_t mcInput = match->GetLink(iLink).GetFile();
    int32_t mcEvent = match->GetLink(iLink).GetEntry();
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
  uint64_t nDigisTot   = 0;  // Total number of digis
  uint64_t nDigisNoise = 0;  // Digis without link to MC event
  uint64_t nDigisAmbig = 0;  // Digis with links to multiple MC events

  // --- Clear output array
  fEvents->Delete();
  fDigiEvents->clear();

  // --- Timeslice start time
  double tsStart = fTimeslice->GetStartTime();

  // --- Bookkeeping: Map from (input number, event number) to event
  map<pair<int32_t, int32_t>, CbmEvent> eventMap;
  map<pair<int32_t, int32_t>, CbmDigiEvent> digiEventMap;

  // --- Loop over all detector systems
  for (ECbmModuleId& system : fSystems) {

    // --- Skip system if no data branch or no match match present
    if (!fDigiMan->IsPresent(system)) continue;
    if (!fDigiMan->IsMatchPresent(system)) continue;

    // --- Find the digi type
    ECbmDataType digiType = ECbmDataType::kUnknown;
    switch (system) {
      case ECbmModuleId::kMvd: digiType = ECbmDataType::kMvdDigi; break;
      case ECbmModuleId::kSts: digiType = ECbmDataType::kStsDigi; break;
      case ECbmModuleId::kRich: digiType = ECbmDataType::kRichDigi; break;
      case ECbmModuleId::kMuch: digiType = ECbmDataType::kMuchDigi; break;
      case ECbmModuleId::kTrd: digiType = ECbmDataType::kTrdDigi; break;
      case ECbmModuleId::kTof: digiType = ECbmDataType::kTofDigi; break;
      case ECbmModuleId::kPsd: digiType = ECbmDataType::kPsdDigi; break;
      case ECbmModuleId::kT0: digiType = ECbmDataType::kT0Digi; break;
      default: break;
    }  //? detector

    if (digiType == ECbmDataType::kUnknown) {
      LOG(fatal) << "unknown type of the module";
      assert(0);
      continue;
    }

    // --- Loop over digis for the current system
    int64_t nDigis  = fDigiMan->GetNofDigis(system);
    uint64_t nNoise = 0;
    uint64_t nAmbig = 0;
    for (int32_t iDigi = 0; iDigi < nDigis; iDigi++) {

      // --- Get the MC input and event numbers through the match object
      CbmMatch matchedEvents = EventsInMatch(fDigiMan->GetMatch(system, iDigi));

      for (int32_t iLink = 0; iLink < matchedEvents.GetNofLinks(); iLink++) {
        const auto& link = matchedEvents.GetLink(iLink);
        auto eventID     = make_pair(link.GetFile(), link.GetEntry());

        // --- Get event. If not yet present, create it. Add digi to event.
        auto eventIt = eventMap.find(eventID);
        if (eventIt == eventMap.end()) {
          auto result = eventMap.insert(make_pair(eventID, CbmEvent()));
          assert(result.second);
          eventIt = result.first;
        }
        eventIt->second.AddData(digiType, iDigi);

        // --- Get digiEvent. If not yet present, create it. Add digi to event.
        auto digiEventIt = digiEventMap.find(eventID);
        if (digiEventIt == digiEventMap.end()) {
          auto result = digiEventMap.insert(make_pair(eventID, CbmDigiEvent()));
          assert(result.second);
          digiEventIt = result.first;
        }
        // TODO: Restricted to STS for the moment, until CbmDigiEvent is expanded to all
        // detector systems (digi types)
        if (system == ECbmModuleId::kSts) {
          digiEventIt->second.fData.fSts.fDigis.push_back(*(fDigiMan->Get<CbmStsDigi>(iDigi)));
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

  // --- Move CbmEvent objects from map (already ordered) to output branch
  int32_t nEvents = 0;
  for (auto& kv : eventMap) {

    // TODO: Retrieving the MC event time from MCEventList currently does not work in case
    // of multiple MC sources and/or repeat mode.
    // Until this is fixed, the event time is set to the timeslice start time.
    /*
    double eventTime = fMCEvents->GetEventTime(kv.first.second, kv.first.first);
    if (eventTime < 0.) {
      LOG(info) << fMCEvents->ToString("long");
      LOG(fatal) << GetName() << ": MC event " << kv.first.second << " from source " << kv.first.first
                 << " not found in MCEventList!";
      continue;
    }
    eventTime -= tsStart;
    */

    double eventTime = tsStart;
    assert(nEvents == fEvents->GetEntriesFast());
    CbmEvent* store = new ((*fEvents)[nEvents]) CbmEvent();
    store->Swap(kv.second);
    store->SetStartTime(eventTime);
    store->SetEndTime(eventTime);
    store->SetNumber(nEvents++);
  }

  // --- Move CbmDigiEvent objects from map (already ordered) to output branch
  uint64_t evNum = 0;
  for (auto& kv : digiEventMap) {

    // TODO: Retrieving the MC event time from MCEventList currently does not work in case
    // of multiple MC sources and/or repeat mode.
    // Until this is fixed, the event time is set to the timeslice start time.
    /*
    double eventTime = fMCEvents->GetEventTime(kv.first.second, kv.first.first);
    if (eventTime < 0.) {
      LOG(info) << fMCEvents->ToString("long");
      LOG(fatal) << GetName() << ": MC event " << kv.first.second << " from source " << kv.first.first
                 << " not found in MCEventList!";
      continue;
    }
    eventTime -= tsStart;
    */

    double eventTime  = tsStart;
    kv.second.fTime   = eventTime;
    kv.second.fNumber = evNum++;
    fDigiEvents->push_back(std::move(kv.second));
  }

  // --- Timeslice log and statistics
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumEntries;
  if (fEvents) logOut << ", events " << nEvents;
  logOut << ", digis " << nDigisTot << " (" << nDigisAmbig << " ambiguous), noise: " << nDigisNoise;
  LOG(info) << logOut.str();
  fNumEntries++;
  fNumEvents += nEvents;
  fNumDigisTotal += nDigisTot;
  fNumDigisAmbig += nDigisAmbig;
  fNumDigisNoise += nDigisNoise;
  fTime += timer.RealTime();

  // --- For debug: event info
  if (fair::Logger::Logging(fair::Severity::debug)) {
    for (int32_t iEvent = 0; iEvent < fEvents->GetEntriesFast(); iEvent++) {
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
  LOG(info) << "Time slices          : " << fNumEntries;
  LOG(info) << "All digis       / TS : " << fixed << setprecision(2) << double(fNumDigisTotal) / double(fNumEntries);
  LOG(info) << "Ambiguous digis / TS : " << fixed << setprecision(2) << double(fNumDigisAmbig) / double(fNumEntries)
            << " = " << 100. * double(fNumDigisAmbig) / double(fNumDigisTotal) << " %";
  LOG(info) << "Noise digis     / TS : " << fixed << setprecision(2) << double(fNumDigisNoise) / double(fNumEntries)
            << " = " << 100. * double(fNumDigisNoise) / double(fNumDigisTotal) << " %";
  LOG(info) << "Events               : " << fNumEvents;
  LOG(info) << "Time  / TS           : " << fixed << setprecision(2) << 1000. * fTime / double(fNumEntries) << " ms";
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


  // --- Check input digis
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

  // --- Time slice meta-data (input)
  fTimeslice = dynamic_cast<CbmTimeSlice*>(ioman->GetObject("TimeSlice."));
  if (fTimeslice == nullptr) {
    LOG(warn) << GetName() << ": No TimeSlice branch found!";
    return kFATAL;
  }

  // --- MC event list (input)
  fMCEvents = dynamic_cast<CbmMCEventList*>(ioman->GetObject("MCEventList."));
  if (fMCEvents == nullptr) {
    LOG(warn) << GetName() << ": No MCEventList branch found!";
    return kFATAL;
  }

  // --- Register output array (CbmEvent)
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

  // --- Register output array (CbmDigiEvent)
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
