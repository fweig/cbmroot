/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith, Volker Friese [committer] */

/** @file CbmStsBuildEventsQA.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20.09.2016
 **/

#include "CbmBuildEventsQa.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"
#include "CbmQaCanvas.h"
#include "CbmStsDigi.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TStopwatch.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;

// =====   Constructor   =====================================================
CbmBuildEventsQa::CbmBuildEventsQa()
  : FairTask("BuildEventsQA")
  , fEvents(NULL)
  , fNofEntries(0)
  , fOutFolder("BuildEventsQA", "Build Events QA")
{
}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsQa::~CbmBuildEventsQa() { DeInit(); }
// ===========================================================================


// =====   De-initialisation   =============================================
void CbmBuildEventsQa::DeInit()
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
InitStatus CbmBuildEventsQa::Init()
{
  DeInit();

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmEvent)
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fEvents) {
    LOG(fatal) << "CbmBuildEventsQa::Init"
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

  InitHistograms();

  return kSUCCESS;
}
// ===========================================================================


// ==================== Init histograms ======================================
void CbmBuildEventsQa::InitHistograms()
{
  // --- Init histogram folder
  histFolder = fOutFolder.AddFolder("hist", "Histogramms");

  // --- Init histograms
  fhCorrectDigiRatioAll = new TH1F("fhCorrectDigiRatioAll", "Correct digis per event [pct]", 416, -2, 102);
  fhCorrectDigiRatioAllNoNoise =
    new TH1F("fhCorrectDigiRatioAllNoNoise", "Correct digis per event [pct], disregarding noise", 416, -2, 102);
  fhNoiseDigiRatioAll = new TH1F("fhNoiseDigiRatioAll", "Noise digis per event [pct]", 416, -2, 102);
  fhFoundDigiRatioAll = new TH1F("fhFoundDigiRatioAll", "Found digis per event [pct]", 416, -2, 102);
  fhCorrectVsFoundAll = new TH2I("fhCorrectVsFoundAll", "Correct digis  [pct] vs. Found digis [pct]; Correct; Found ",
                                 110, -5., 105., 110, -5., 105.);
  fhCorrectVsFoundAllNoNoise =
    new TH2I("fhCorrectVsFoundAllNoNoise", "Correct digis  [pct] vs. Found digis [pct], no noise; Correct; Found ", 110,
             -5., 105., 110, -5., 105.);

  histFolder->Add(fhCorrectDigiRatioAll);
  histFolder->Add(fhCorrectDigiRatioAllNoNoise);
  histFolder->Add(fhNoiseDigiRatioAll);
  histFolder->Add(fhFoundDigiRatioAll);
  histFolder->Add(fhCorrectVsFoundAll);
  histFolder->Add(fhCorrectVsFoundAllNoNoise);

  fCanvAllSystems = new CbmQaCanvas("cAllSystems", "", 3 * 400, 2 * 400);
  fCanvAllSystems->Divide2D(6);
  fOutFolder.Add(fCanvAllSystems);

  for (ECbmModuleId& system : fSystems) {
    TString moduleName = CbmModuleList::GetModuleNameCaps(system);
    TString h1name     = "fhCorrectDigiRatio" + moduleName;
    TString h2name     = "fhCorrectDigiRatioNoNoise" + moduleName;
    TString h3name     = "fhNoiseDigiRatio" + moduleName;
    TString h4name     = "fhFoundDigiRatio" + moduleName;
    TString h5name     = "fhCorrectVsFound" + moduleName;
    TString h6name     = "fhCorrectVsFoundNoNoise" + moduleName;

    fhMapSystemsCorrectDigi[system] =
      new TH1F(h1name, Form("Correct digis per event, %s [pct]", moduleName.Data()), 416, -2, 102);
    fhMapSystemsCorrectDigiNoNoise[system] =
      new TH1F(h2name, Form("Correct digis per event, %s [pct], disregarding noise", moduleName.Data()), 416, -2, 102);
    fhMapSystemsNoiseDigi[system] =
      new TH1F(h3name, Form("Noise digis per event, %s [pct]", moduleName.Data()), 416, -2, 102);
    fhMapSystemsFoundDigi[system] =
      new TH1F(h4name, Form("Found digis per event, %s [pct]", moduleName.Data()), 416, -2, 102);
    fhMapSystemsCorrectVsFound[system] =
      new TH2I(h5name, Form("Correct digis  [pct] vs. Found digis [pct], %s; Correct; Found", moduleName.Data()), 110,
               -5., 105., 110, -5., 105.);
    fhMapSystemsCorrectVsFoundNoNoise[system] = new TH2I(
      h6name, Form("Correct digis  [pct] vs. Found digis [pct], %s, no noise; Correct; Found", moduleName.Data()), 110,
      -5., 105., 110, -5., 105.);

    histFolder->Add(fhMapSystemsCorrectDigi[system]);
    histFolder->Add(fhMapSystemsCorrectDigiNoNoise[system]);
    histFolder->Add(fhMapSystemsNoiseDigi[system]);
    histFolder->Add(fhMapSystemsFoundDigi[system]);
    histFolder->Add(fhMapSystemsCorrectVsFound[system]);
    histFolder->Add(fhMapSystemsCorrectVsFoundNoNoise[system]);

    fCanvMapSystems[system] =
      new CbmQaCanvas(Form("c%s", moduleName.Data()), Form("%s", moduleName.Data()), 3 * 400, 2 * 400);
    fCanvMapSystems[system]->Divide2D(6);
    fOutFolder.Add(fCanvMapSystems[system]);
  }
}
// ===========================================================================


// =====   Task execution   ==================================================
void CbmBuildEventsQa::Exec(Option_t*)
{
  // --- Time and counters
  TStopwatch timer;
  timer.Start();

  // --- Event loop
  int nEvents = fEvents->GetEntriesFast();
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);

    // --- Match event to MC
    LOG(info) << "";
    MatchEvent(event);
    if (event->GetMatch()->GetNofLinks() < 1) {
      LOG(warning) << "No links in this event match object, skipping the event";
      continue;
    }  // if (-1 == event->GetMatch()->GetNofLinks())
    int matchedMcEventNr = event->GetMatch()->GetMatchedLink().GetEntry();

    //match to -1 only if event is pure noise
    if (event->GetMatch()->GetNofLinks() > 1 && matchedMcEventNr == -1) {
      matchedMcEventNr = getMatchedMcEventNoNoise(event);
    }

    LOG(info) << GetName() << ": Event " << event->GetNumber() << ", digis in event: " << event->GetNofData()
              << ", links to MC events: " << event->GetMatch()->GetNofLinks()
              << ", matched MC event number: " << matchedMcEventNr;
    if (matchedMcEventNr == -1) { LOG(info) << "(event is pure noise)"; }

    LOG(info) << "Start time: " << event->GetStartTime() << ", end time: " << event->GetEndTime()
              << ", middle time: " << (event->GetStartTime() + event->GetEndTime()) / 2.;

    const std::vector<CbmLink> linkList = event->GetMatch()->GetLinks();
    for (size_t iLink = 0; iLink < linkList.size(); iLink++) {
      int linkedEvent    = linkList.at(iLink).GetEntry();
      float linkedWeight = linkList.at(iLink).GetWeight();
      std::string isMatched;
      if (linkedEvent == matchedMcEventNr) { isMatched = " (matched)"; }
      else {
        isMatched = "";
      }
      LOG(info) << "Link " << iLink << ": MC event " << linkedEvent << " weight " << linkedWeight << isMatched;
    }

    // --- Loop over all detector systems
    for (ECbmModuleId& system : fSystems) {

      // --- Skip system if no data branch or no match match present
      if (!fDigiMan->IsPresent(system)) continue;
      if (!fDigiMan->IsMatchPresent(system)) continue;

      // --- Counters
      int nDigis        = event->GetNofData(GetDigiType(system));
      int nDigisNoise   = 0;
      int nDigisCorrect = 0;
      int nLinks        = 0;
      int nLinksNoise   = 0;
      int nLinksCorrect = 0;

      // --- Loop over digis in event
      for (int iDigi = 0; iDigi < nDigis; iDigi++) {
        unsigned int index = event->GetIndex(GetDigiType(system), iDigi);

        const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
        assert(digiMatch);

        // --- Check MC event of digi match
        if (digiMatch->GetNofLinks()) {
          if (digiMatch->GetMatchedLink().GetEntry() == matchedMcEventNr) nDigisCorrect++;
          if (digiMatch->GetMatchedLink().GetEntry() == -1) nDigisNoise++;
        }

        for (int iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
          int entry = digiMatch->GetLink(iLink).GetEntry();
          nLinks++;
          if (entry == matchedMcEventNr) nLinksCorrect++;
          if (entry == -1) nLinksNoise++;
        }
      }

      // --- Counters
      int totDigis      = fDigiMan->GetNofDigis(system);
      int totEventDigis = 0;

      // --- Loop over all digis for the current system
      for (int iDigi = 0; iDigi < totDigis; iDigi++) {

        // --- Get the event number through the match object
        const CbmMatch* match = fDigiMan->GetMatch(system, iDigi);
        assert(match);
        int mcEvent = -1;

        if (match->GetNofLinks()) { mcEvent = match->GetMatchedLink().GetEntry(); }
        //digi belongs to current event
        if (mcEvent == matchedMcEventNr) totEventDigis++;
      }

      // --- QA output
      if (0 < nDigis) {
        const double correctDigisPercent        = 100. * Double_t(nDigisCorrect) / Double_t(nDigis);
        const double correctDigisPercentNoNoise = 100. * Double_t(nDigisCorrect) / Double_t(nDigis - nDigisNoise);
        const double noiseDigisPercent          = 100. * Double_t(nDigisNoise) / Double_t(nDigis);
        const double foundDigisPercent          = 100. * Double_t(nDigisCorrect) / Double_t(totEventDigis);
        const double correctLinksPercent        = 100. * Double_t(nLinksCorrect) / Double_t(nLinks);
        const double correctLinksPercentNoNoise = 100. * Double_t(nLinksCorrect) / Double_t(nLinks - nLinksNoise);
        const double noiseLinksPercent          = 100. * Double_t(nLinksNoise) / Double_t(nLinks);

        LOG(info) << GetName() << ": Detector " << CbmModuleList::GetModuleNameCaps(system);
        LOG(info) << "Correct digis " << nDigisCorrect << " / " << nDigis << " = " << correctDigisPercent << " %";
        if (matchedMcEventNr != -1) {
          LOG(info) << "Noise digis " << nDigisNoise << " / " << nDigis << " = " << noiseDigisPercent << " %";
          LOG(info) << "Correct digis, disregarding noise " << nDigisCorrect << " / " << nDigis - nDigisNoise << " = "
                    << correctDigisPercentNoNoise << " %";
        }
        LOG(info) << "Correct digi links " << nLinksCorrect << " / " << nLinks << " = " << correctLinksPercent << " % ";
        if (matchedMcEventNr != -1) {
          LOG(info) << "Noise digi links " << nLinksNoise << " / " << nLinks << " = " << noiseLinksPercent << " % ";
          LOG(info) << "Correct digi links, disregarding noise " << nLinksCorrect << " / " << nLinks - nLinksNoise
                    << " = " << correctLinksPercentNoNoise << " % ";
        }
        LOG(info) << "Digi percentage found " << nDigisCorrect << " / " << totEventDigis << " = " << foundDigisPercent
                  << " % ";

        //fill histograms
        if (matchedMcEventNr != -1) {  //ignore events which are pure noise
          fhCorrectDigiRatioAll->Fill(correctDigisPercent);
          fhCorrectDigiRatioAllNoNoise->Fill(correctDigisPercentNoNoise);
          fhNoiseDigiRatioAll->Fill(noiseDigisPercent);
          fhFoundDigiRatioAll->Fill(foundDigisPercent);

          fhCorrectVsFoundAll->Fill(correctDigisPercent, foundDigisPercent);
          fhCorrectVsFoundAllNoNoise->Fill(correctDigisPercentNoNoise, foundDigisPercent);

          fhMapSystemsCorrectDigi[system]->Fill(correctDigisPercent);
          fhMapSystemsCorrectDigiNoNoise[system]->Fill(correctDigisPercentNoNoise);
          fhMapSystemsNoiseDigi[system]->Fill(noiseDigisPercent);
          fhMapSystemsFoundDigi[system]->Fill(foundDigisPercent);

          fhMapSystemsCorrectVsFound[system]->Fill(correctDigisPercent, foundDigisPercent);
          fhMapSystemsCorrectVsFoundNoNoise[system]->Fill(correctDigisPercentNoNoise, foundDigisPercent);
        }
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
int CbmBuildEventsQa::getMatchedMcEventNoNoise(const CbmEvent* event)
{
  const std::vector<CbmLink> linkList = event->GetMatch()->GetLinks();
  int matchedEvent                    = -1;
  float matchedWeight                 = 0.0;
  for (size_t iLink = 0; iLink < linkList.size(); iLink++) {
    const int linkedEvent    = linkList.at(iLink).GetEntry();
    const float linkedWeight = linkList.at(iLink).GetWeight();
    if (linkedEvent != -1 && linkedWeight > matchedWeight) {
      matchedEvent  = linkedEvent;
      matchedWeight = linkedWeight;
    }
  }
  return matchedEvent;
}

// =====   Match event   =====================================================
void CbmBuildEventsQa::MatchEvent(CbmEvent* event)
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
    int iNbDigis = event->GetNofData(GetDigiType(system));
    for (int iDigi = 0; iDigi < iNbDigis; iDigi++) {
      int index                 = event->GetIndex(GetDigiType(system), iDigi);
      const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
      assert(digiMatch);

      // --- Update event match with digi links
      // --- N.b.: The member "index" of CbmLink has here no meaning, since
      // --- there is only one MC event per tree entry.
      for (int iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
        int file        = digiMatch->GetLink(iLink).GetFile();
        int entry       = digiMatch->GetLink(iLink).GetEntry();
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
void CbmBuildEventsQa::Finish()
{
  //output histograms
  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetSink()) {
    LOG(error) << "No sink found";
    return;
  }

  fCanvAllSystems->cd(1);
  fhCorrectDigiRatioAll->DrawCopy("colz", "");

  fCanvAllSystems->cd(2);
  fhCorrectDigiRatioAllNoNoise->DrawCopy("colz", "");

  fCanvAllSystems->cd(3);
  fhNoiseDigiRatioAll->DrawCopy("colz", "");

  fCanvAllSystems->cd(4);
  fhFoundDigiRatioAll->DrawCopy("colz", "");

  fCanvAllSystems->cd(5);
  fhCorrectVsFoundAll->DrawCopy("colz", "");

  fCanvAllSystems->cd(6);
  fhCorrectVsFoundAllNoNoise->DrawCopy("colz", "");

  for (ECbmModuleId& system : fSystems) {
    fCanvMapSystems[system]->cd(1);
    fhMapSystemsCorrectDigi[system]->DrawCopy("colz", "");

    fCanvMapSystems[system]->cd(2);
    fhMapSystemsCorrectDigiNoNoise[system]->DrawCopy("colz", "");

    fCanvMapSystems[system]->cd(3);
    fhMapSystemsNoiseDigi[system]->DrawCopy("colz", "");

    fCanvMapSystems[system]->cd(4);
    fhMapSystemsFoundDigi[system]->DrawCopy("colz", "");

    fCanvMapSystems[system]->cd(5);
    fhMapSystemsCorrectVsFound[system]->DrawCopy("colz", "");

    fCanvMapSystems[system]->cd(6);
    fhMapSystemsCorrectVsFoundNoNoise[system]->DrawCopy("colz", "");
  }

  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// ===========================================================================


// =====  Get digi type  =====================================================
ECbmDataType CbmBuildEventsQa::GetDigiType(ECbmModuleId system)
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


ClassImp(CbmBuildEventsQa)
