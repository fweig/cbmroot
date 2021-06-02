/** @file CbmBuildEventsQA.cxx
  * @copyright Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Dominik Smith, Volker Friese [committer] **/

/** @file CbmStsBuildEventsQA.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20.09.2016
 **/

#include "CbmBuildEventsQA.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"
#include "CbmStsDigi.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TH1.h"
#include "TStopwatch.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;

// =====   Constructor   =====================================================
CbmBuildEventsQA::CbmBuildEventsQA()
  : FairTask("BuildEventsQA")
  , fEvents(NULL)
  , fNofEntries(0)
  , fOutFolder("BuildEventsQA", "Build Events QA")
{
}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsQA::~CbmBuildEventsQA() { DeInit(); }
// ===========================================================================


// =====   De-initialisation   =============================================
void CbmBuildEventsQA::DeInit()
{
  fOutFolder.Clear();
  histFolder = nullptr;
  SafeDelete(fhCorrectDigiRatioAll);
  SafeDelete(fhFoundDigiRatioAll);

  for (auto& p : fhMapSystemsCorrectDigi) {
    SafeDelete(p.second);
  }
  for (auto& p : fhMapSystemsFoundDigi) {
    SafeDelete(p.second);
  }
  fhMapSystemsCorrectDigi.clear();
  fhMapSystemsFoundDigi.clear();
}

// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsQA::Init()
{
  DeInit();

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmEvent)
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fEvents) {
    LOG(fatal) << "CbmBuildEventsQA::Init"
               << "No CbmEvent TClonesArray found!";
  }

  // --- DigiManager instance
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

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

  // --- Init histograms
  histFolder            = fOutFolder.AddFolder("hist", "Histogramms");
  fhCorrectDigiRatioAll = new TH1F("fhCorrectDigiRatioAll", "\% correct digis per event", 1001, 0., 100.1);
  fhFoundDigiRatioAll   = new TH1F("fhFoundDigiRatioAll", "\% found digis per event", 1001, 0., 100.1);
  histFolder->Add(fhCorrectDigiRatioAll);
  histFolder->Add(fhFoundDigiRatioAll);
  for (ECbmModuleId& system : fSystems) {
    TString h1name                  = "fhCorrectDigiRatio" + CbmModuleList::GetModuleNameCaps(system);
    TString h2name                  = "fhFoundDigiRatio" + CbmModuleList::GetModuleNameCaps(system);
    fhMapSystemsCorrectDigi[system] = new TH1F(h1name, "\% correct digis per event", 1001, 0., 100.1);
    fhMapSystemsFoundDigi[system]   = new TH1F(h2name, "\% found digis per event", 1001, 0., 100.1);
    histFolder->Add(fhMapSystemsCorrectDigi[system]);
    histFolder->Add(fhMapSystemsFoundDigi[system]);
  }
  return kSUCCESS;
}
// ===========================================================================


// =====   Task execution   ==================================================
void CbmBuildEventsQA::Exec(Option_t*)
{
  // --- Time and counters
  TStopwatch timer;
  timer.Start();

  // --- Event loop
  Int_t nEvents = fEvents->GetEntriesFast();
  for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
    CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);

    // --- Match event to MC
    MatchEvent(event);
    if (event->GetMatch()->GetNofLinks() < 1) {
      LOG(warning) << "No links in this event match object, skipping the event";
      continue;
    }  // if (-1 == event->GetMatch()->GetNofLinks())
    Int_t mcEventNr = event->GetMatch()->GetMatchedLink().GetEntry();
    LOG(info) << GetName() << ": Event " << event->GetNumber() << ", digis in event: " << event->GetNofData()
              << ", links to MC events: " << event->GetMatch()->GetNofLinks() << ", matched MC event number "
              << mcEventNr;

    // --- Loop over all detector systems
    for (ECbmModuleId& system : fSystems) {

      // --- Skip system if no data branch or no match match present
      if (!fDigiMan->IsPresent(system)) continue;
      if (!fDigiMan->IsMatchPresent(system)) continue;

      // --- Counters
      Int_t nDigis        = event->GetNofData(GetDigiType(system));
      Int_t nDigiCorrect  = 0;
      Int_t nLinks        = 0;
      Int_t nLinksCorrect = 0;

      // --- Loop over digis in event
      for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
        UInt_t index = event->GetIndex(GetDigiType(system), iDigi);

        const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
        assert(digiMatch);

        // --- Check MC event of digi match
        if (digiMatch->GetNofLinks()) {
          if (digiMatch->GetMatchedLink().GetEntry() == mcEventNr) nDigiCorrect++;
        }

        for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
          Int_t entry = digiMatch->GetLink(iLink).GetEntry();
          nLinks++;
          if (entry == mcEventNr) nLinksCorrect++;
        }
      }

      // --- Counters
      Int_t totDigis      = fDigiMan->GetNofDigis(system);
      Int_t totEventDigis = 0;

      // --- Loop over all digis for the current system
      for (Int_t iDigi = 0; iDigi < totDigis; iDigi++) {

        // --- Get the event number through the match object
        const CbmMatch* match = fDigiMan->GetMatch(system, iDigi);
        assert(match);
        Int_t mcEvent = -1;

        if (match->GetNofLinks()) { mcEvent = match->GetMatchedLink().GetEntry(); }
        //digi belongs to current event
        if (mcEvent == mcEventNr) totEventDigis++;
      }

      // --- QA output
      if (0 < nDigis) {
        LOG(info) << GetName() << ": Detector " << CbmModuleList::GetModuleNameCaps(system);
        LOG(info) << "Correct digis " << nDigiCorrect << " / " << nDigis << " = "
                  << 100. * Double_t(nDigiCorrect) / Double_t(nDigis) << " %";
        LOG(info) << "Correct digi links " << nLinksCorrect << " / " << nLinks << " = "
                  << 100. * Double_t(nLinksCorrect) / Double_t(nLinks) << " % ";
        LOG(info) << "Digi percentage found " << nDigiCorrect << " / " << totEventDigis << " = "
                  << 100. * Double_t(nDigiCorrect) / Double_t(totEventDigis) << " % ";

        //fill histograms
        fhCorrectDigiRatioAll->Fill(100. * Double_t(nLinksCorrect) / Double_t(nLinks));
        fhFoundDigiRatioAll->Fill(100. * Double_t(nDigiCorrect) / Double_t(totEventDigis));
        fhMapSystemsCorrectDigi[system]->Fill(100. * Double_t(nLinksCorrect) / Double_t(nLinks));
        fhMapSystemsFoundDigi[system]->Fill(100. * Double_t(nDigiCorrect) / Double_t(totEventDigis));
      }
      else {
        LOG(info) << GetName() << ": Detector " << CbmModuleList::GetModuleNameCaps(system)
                  << ", no digis in this event";
      }
    }  // systems
  }    //# events

  // Timer and counters
  fNofEntries++;
  timer.Stop();

  // --- Execution log
  LOG(info) << "+ " << setw(20) << GetName() << ": Entry " << setw(6) << right << fNofEntries << ", real time " << fixed
            << setprecision(6) << timer.RealTime() << " s, events: " << fEvents->GetEntriesFast();
}
// ===========================================================================


// =====   Match event   =====================================================
void CbmBuildEventsQA::MatchEvent(CbmEvent* event)
{
  // --- Get event match object. If present, will be cleared first. If not,
  // --- it will be created.
  CbmMatch* match = event->GetMatch();
  if (!match) {
    LOG(info) << "No match data found in event. Creating new.";
    match = new CbmMatch();
    event->SetMatch(match);
  }
  else {
    LOG(info) << "Match data found in event. Clearing.";
    match->ClearLinks();
  }

  // --- Loop over all detector systems
  for (ECbmModuleId& system : fSystems) {

    //Skip if reference detectors are set and current system isn't one
    if (!fRefDetectors.empty()
        && std::find(fRefDetectors.begin(), fRefDetectors.end(), system) == fRefDetectors.end()) {
      continue;
    }

    // --- Loop over digis in event
    Int_t iNbDigis = event->GetNofData(GetDigiType(system));
    for (Int_t iDigi = 0; iDigi < iNbDigis; iDigi++) {
      Int_t index               = event->GetIndex(GetDigiType(system), iDigi);
      const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
      assert(digiMatch);

      // --- Update event match with digi links
      // --- N.b.: The member "index" of CbmLink has here no meaning, since
      // --- there is only one MC event per tree entry.
      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
        Int_t file      = digiMatch->GetLink(iLink).GetFile();
        Int_t entry     = digiMatch->GetLink(iLink).GetEntry();
        Double_t weight = digiMatch->GetLink(iLink).GetWeight();
        //     LOG(info) << "Adding link (weight, entry, file): " << weight << " "
        //		<< entry << " " << file;
        match->AddLink(weight, 0, entry, file);
      }
    }
  }
}
// ===========================================================================


// =====  Finish task  =======================================================
void CbmBuildEventsQA::Finish()
{
  //output histograms
  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
    LOG(error) << "No sink found";
    return;
  }
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// ===========================================================================


// =====  Get digi type  =====================================================
ECbmDataType CbmBuildEventsQA::GetDigiType(ECbmModuleId system)
{
  switch (system) {
    case ECbmModuleId::kMvd: return ECbmDataType::kMvdDigi;
    case ECbmModuleId::kSts: return ECbmDataType::kStsDigi;
    case ECbmModuleId::kRich: return ECbmDataType::kRichDigi;
    case ECbmModuleId::kMuch: return ECbmDataType::kMuchDigi;
    case ECbmModuleId::kTrd: return ECbmDataType::kTrdDigi;
    case ECbmModuleId::kTof: return ECbmDataType::kTofDigi;
    case ECbmModuleId::kPsd: return ECbmDataType::kPsdDigi;
    default: return ECbmDataType::kUnknown;
  }
}
// ===========================================================================


ClassImp(CbmBuildEventsQA)
