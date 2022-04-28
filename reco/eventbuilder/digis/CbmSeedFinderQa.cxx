/* Copyright (C) 2007-2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmSeedFinderQa.h"

#include "CbmMCEventList.h"
#include "CbmQaCanvas.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TH1.h"
#include "TH2.h"

CbmSeedFinderQa::CbmSeedFinderQa() : fOutFolder("SeedFinderQA", "Seed finder QA")
{
  // --- Init histogram folder
  histFolder = fOutFolder.AddFolder("hist", "Histogramms");

  // --- Init histograms
  fhLinkedMCEventsPerTrigger =
    new TH1F("fhLinkedMCEventsPerTrigger", "Linked MC events per trigger (=0 for pure noise)", 5, -1.5, 3.5);
  fhLinkedMCEventsPerTrigger->SetCanExtend(TH1::kAllAxes);

  fhLinkedTriggersPerMCEvent = new TH1F("fhLinkedTriggersPerMCEvent", "Linked triggers per MC event", 5, -1.5, 3.5);
  fhLinkedTriggersPerMCEvent->SetCanExtend(TH1::kAllAxes);

  fhMatchedTriggersPerMCEvent = new TH1F("fhMatchedTriggersPerMCEvent", "Matched triggers per MC event", 5, -1.5, 3.5);
  fhMatchedTriggersPerMCEvent->SetCanExtend(TH1::kAllAxes);

  fhCorrectDigiRatio = new TH1F("fhCorrectDigiRatio", "Correct digis per seed [pct]", 416, -2, 102);
  fhCorrectDigiRatioNoNoise =
    new TH1F("fhCorrectDigiRatioNoNoise", "Correct digis per seed [pct], disregarding noise", 416, -2, 102);
  fhNoiseDigiRatio = new TH1F("fhNoiseDigiRatio", "Noise digis per seed [pct]", 416, -2, 102);
  fhFoundDigiRatio = new TH1F("fhFoundDigiRatio", "Found digis per seed [pct]", 416, -2, 102);
  fhCorrectVsFound = new TH2I("fhCorrectVsFound", "Correct digis  [pct] vs. Found digis [pct]; Correct; Found ", 110,
                              -5., 105., 110, -5., 105.);
  fhCorrectVsFoundNoNoise =
    new TH2I("fhCorrectVsFoundNoNoise", "Correct digis  [pct] vs. Found digis [pct], no noise; Correct; Found ", 110,
             -5., 105., 110, -5., 105.);

  fhTimeOffset = new TH1F("fhTimeOffsetMatched", "tSeed - tMCMatched [ns]", 20, -5, 5);
  fhTimeOffset->SetCanExtend(TH1::kAllAxes);

  fhTimeOffsetSingletOnly = new TH1F("fhTimeOffsetSingletOnly", "tSeed - tMCMatched [ns], one-to-one only", 20, -5, 5);
  fhTimeOffsetSingletOnly->SetCanExtend(TH1::kAllAxes);

  fhTimeOffsetClosest = new TH1F("fhTimeOffsetClosest", "tSeed - tMCClosest [ns]", 20, -5, 5);
  fhTimeOffsetClosest->SetCanExtend(TH1::kAllAxes);

  histFolder->Add(fhCorrectDigiRatio);
  histFolder->Add(fhCorrectDigiRatioNoNoise);
  histFolder->Add(fhNoiseDigiRatio);
  histFolder->Add(fhFoundDigiRatio);
  histFolder->Add(fhCorrectVsFound);
  histFolder->Add(fhCorrectVsFoundNoNoise);
  histFolder->Add(fhTimeOffset);
  histFolder->Add(fhTimeOffsetSingletOnly);
  histFolder->Add(fhTimeOffsetClosest);
  histFolder->Add(fhLinkedMCEventsPerTrigger);
  histFolder->Add(fhLinkedTriggersPerMCEvent);
  histFolder->Add(fhMatchedTriggersPerMCEvent);

  fCanv = new CbmQaCanvas("cSummary", "", 4 * 400, 3 * 400);
  fCanv->Divide2D(11);
  fOutFolder.Add(fCanv);
}

CbmSeedFinderQa::~CbmSeedFinderQa()
{
  delete fhCorrectDigiRatio;
  delete fhCorrectDigiRatioNoNoise;
  delete fhNoiseDigiRatio;
  delete fhFoundDigiRatio;
  delete fhCorrectVsFound;
  delete fhCorrectVsFoundNoNoise;
  delete fhTimeOffset;
  delete fhTimeOffsetClosest;
  delete fhLinkedMCEventsPerTrigger;
  delete fhLinkedTriggersPerMCEvent;
  delete fhMatchedTriggersPerMCEvent;
  delete fCanv;
}

void CbmSeedFinderQa::Init()
{
  if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetObject("MCEventList.")) {
    LOG(error) << "No MC event list found";
    return;
  }
  fEventList = (CbmMCEventList*) FairRootManager::Instance()->GetObject("MCEventList.");
}

void CbmSeedFinderQa::ResetPerTsStorage()
{
  fvEventMatchesPerTs.clear();
  fvSeedTimesPerTs.clear();
}

void CbmSeedFinderQa::FillQaSeedInfo(const int32_t WinStart, const int32_t WinEnd,
                                     const std::vector<CbmMatch>* vDigiMatch, const double seedTime)
{
  fvSeedTimesFull.push_back(seedTime);
  fvSeedTimesPerTs.push_back(seedTime);

  int32_t digiCount             = 0;
  int32_t noiseDigiCount        = 0;
  int32_t correctDigiCount      = 0;
  int32_t matchedEventDigiCount = 0;
  CbmMatch seedMatch;

  for (int32_t iDigi = WinStart; iDigi <= WinEnd; iDigi++) {
    const CbmMatch* digiMatch = &(vDigiMatch->at(iDigi));
    digiCount++;
    if (digiMatch->GetNofLinks() == 0) {
      //skip digis with no links to avoid T0 pollution
      noiseDigiCount++;
      continue;
    }
    if (digiMatch->GetMatchedLink().GetEntry() == -1) {
      noiseDigiCount++;
      continue;  //disregard noise digis
    }

    // Update seed match with digi links
    for (int32_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
      int32_t entry = digiMatch->GetLink(iLink).GetEntry();
      if (entry == -1) { continue; }  //ignore noise links
      int32_t file = digiMatch->GetLink(iLink).GetFile();
      //double weight = digiMatch->GetLink(iLink).GetWeight();
      const double weight =
        .00001;  // assign the same weight to all links, since different detectors don't use the same scale
      //     LOG(info) << "Adding link (number, weight, entry, file): " << iLink << " " << weight << " "
      //              << entry << " " << file;
      seedMatch.AddLink(weight, 0, entry, file);
    }
  }
  fvFullDigiCount.push_back(digiCount);
  fvNoiseDigiCount.push_back(noiseDigiCount);
  fvLinkedMCEventsCount.push_back(seedMatch.GetNofLinks());

  if (seedMatch.GetNofLinks() == 0)  //seed is only noise digis
  {
    seedMatch.AddLink(1.0, 0, -1, 0);
    fvEventMatches.push_back(seedMatch);
    fvEventMatchesPerTs.push_back(seedMatch);

    //should never show up in histograms
    fvCorrectDigiRatio.push_back(std::numeric_limits<double>::quiet_NaN());
    fvCorrectDigiRatioNoNoise.push_back(std::numeric_limits<double>::quiet_NaN());
    fvFoundDigiRatio.push_back(std::numeric_limits<double>::quiet_NaN());
    fvTimeOffset.push_back(std::numeric_limits<double>::quiet_NaN());
    return;
  }
  else {
    fvEventMatches.push_back(seedMatch);
    fvEventMatchesPerTs.push_back(seedMatch);
  }

  //correct digis in seed window
  for (int32_t iDigi = WinStart; iDigi <= WinEnd; iDigi++) {
    const CbmMatch* digiMatch = &(vDigiMatch->at(iDigi));
    if (digiMatch->GetNofLinks() == 0) { continue; }  //skip digis with no links to avoid T0 pollution
    const int32_t entry = digiMatch->GetMatchedLink().GetEntry();
    if (entry != -1)  // disregarding noise
    {
      if (entry == seedMatch.GetMatchedLink().GetEntry()) { correctDigiCount++; }
    }
  }
  const double correctDigiRatio = (double) correctDigiCount / digiCount;
  fvCorrectDigiRatio.push_back(correctDigiRatio);

  const double correctDigiRatioNoNoise = (double) correctDigiCount / (digiCount - noiseDigiCount);
  fvCorrectDigiRatioNoNoise.push_back(correctDigiRatioNoNoise);

  //found digis of matched event in seed window
  for (uint32_t iDigi = 0; iDigi < vDigiMatch->size(); iDigi++) {
    const CbmMatch* digiMatch = &(vDigiMatch->at(iDigi));
    if (digiMatch->GetNofLinks() == 0) { continue; }  //skip digis with no links to avoid T0 pollution
    const int matchedEvent = digiMatch->GetMatchedLink().GetEntry();
    if (matchedEvent == seedMatch.GetMatchedLink().GetEntry()) { matchedEventDigiCount++; }
  }
  const double foundDigiRatio = (double) correctDigiCount / matchedEventDigiCount;
  fvFoundDigiRatio.push_back(foundDigiRatio);

  //seed time offset to MC
  const CbmLink& eventLink = seedMatch.GetMatchedLink();
  const double timeDiff    = seedTime - fEventList->GetEventTime(eventLink.GetEntry(), eventLink.GetFile());
  fvTimeOffset.push_back(timeDiff);
}

void CbmSeedFinderQa::FillQaMCInfo()
{
  const uint32_t nEvents = fEventList->GetNofEvents();
  if (nEvents == 0) { return; }

  std::vector<uint32_t> vLinkedTriggersPerMCEvent;
  std::vector<uint32_t> vMatchedTriggersPerMCEvent;
  vLinkedTriggersPerMCEvent.resize(nEvents, 0);
  vMatchedTriggersPerMCEvent.resize(nEvents, 0);

  for (uint32_t iSeed = 0; iSeed < fvEventMatchesPerTs.size(); iSeed++) {
    const CbmMatch eventMatch = fvEventMatchesPerTs.at(iSeed);
    const CbmLink matchedLink = eventMatch.GetMatchedLink();
    if (fEventList->GetEventIndex(matchedLink) == -1) { continue; }

    for (int32_t iLink = 0; iLink < eventMatch.GetNofLinks(); iLink++) {
      const CbmLink eventLink = eventMatch.GetLink(iLink);
      vLinkedTriggersPerMCEvent[fEventList->GetEventIndex(eventLink)]++;
    }
    vMatchedTriggersPerMCEvent[fEventList->GetEventIndex(matchedLink)]++;
  }

  for (uint32_t iSeed = 0; iSeed < fvEventMatchesPerTs.size(); iSeed++) {
    const CbmMatch eventMatch = fvEventMatchesPerTs.at(iSeed);
    const CbmLink matchedLink = eventMatch.GetMatchedLink();
    if (fEventList->GetEventIndex(matchedLink) == -1) { continue; }

    if (vMatchedTriggersPerMCEvent[fEventList->GetEventIndex(matchedLink)] == 1) {
      const double seedTime = fvSeedTimesPerTs[iSeed];
      const double timeDiff = seedTime - fEventList->GetEventTime(matchedLink.GetEntry(), matchedLink.GetFile());
      fhTimeOffsetSingletOnly->Fill(timeDiff);
    }
  }

  for (const auto& value : vLinkedTriggersPerMCEvent) {
    fhLinkedTriggersPerMCEvent->Fill(value);
  }
  for (const auto& value : vMatchedTriggersPerMCEvent) {
    fhMatchedTriggersPerMCEvent->Fill(value);
  }

  // get sorted vector of MC event times
  std::vector<double> vMCEventTimes;
  for (uint32_t iEvent = 0; iEvent < nEvents; iEvent++) {
    vMCEventTimes.push_back(fEventList->GetEventTimeByIndex(iEvent));
  }
  std::sort(std::begin(vMCEventTimes), std::end(vMCEventTimes));

  //find closest MC event for each seed (assumes both arrays are sorted in time)
  auto minElem = vMCEventTimes.begin();
  for (const auto& seedTime : fvSeedTimesPerTs) {
    auto comp = [&, seedTime](double val1, double val2) { return fabs(seedTime - val1) < fabs(seedTime - val2); };
    minElem   = std::min_element(minElem, vMCEventTimes.end(), comp);
    fhTimeOffsetClosest->Fill(seedTime - *minElem);
  }
}

void CbmSeedFinderQa::FillHistos()
{
  for (uint32_t iEvent = 0; iEvent < fvEventMatches.size(); iEvent++) {

    fhLinkedMCEventsPerTrigger->Fill(fvLinkedMCEventsCount.at(iEvent));

    const CbmMatch* match = &(fvEventMatches.at(iEvent));
    const CbmLink& link   = match->GetMatchedLink();
    if (link.GetEntry() == -1) { continue; }

    fhTimeOffset->Fill(fvTimeOffset.at(iEvent));
    const int32_t digiCount                 = fvFullDigiCount.at(iEvent);
    const int32_t noiseCount                = fvNoiseDigiCount.at(iEvent);
    const double noiseDigisPercent          = 100. * (double) noiseCount / digiCount;
    const double correctDigisPercent        = 100. * fvCorrectDigiRatio.at(iEvent);
    const double correctDigisPercentNoNoise = 100. * fvCorrectDigiRatioNoNoise.at(iEvent);
    const double foundDigisPercent          = 100. * fvFoundDigiRatio.at(iEvent);
    fhCorrectDigiRatio->Fill(correctDigisPercent);
    fhCorrectDigiRatioNoNoise->Fill(correctDigisPercentNoNoise);
    fhNoiseDigiRatio->Fill(noiseDigisPercent);
    fhFoundDigiRatio->Fill(foundDigisPercent);
    fhCorrectVsFound->Fill(correctDigisPercent, foundDigisPercent);
    fhCorrectVsFoundNoNoise->Fill(correctDigisPercentNoNoise, foundDigisPercent);
  }
}

void CbmSeedFinderQa::OutputQa()
{
  for (uint32_t iEvent = 0; iEvent < fvEventMatches.size(); iEvent++) {
    const CbmMatch* match   = &(fvEventMatches.at(iEvent));
    const int32_t mcEventNr = match->GetMatchedLink().GetEntry();

    std::cout << "QA for seed # " << iEvent << std::endl;
    std::cout << "Seed time: " << fvSeedTimesFull.at(iEvent) << std::endl;
    std::cout << "Links to MC events: " << match->GetNofLinks() << ", matched MC event number " << mcEventNr
              << std::endl;
    if (mcEventNr == -1) {
      std::cout << "Warning: Seed was constructed from noise digis only (MC event = -1)!" << std::endl;
      std::cout << "         Please increase your noise threshold!" << std::endl;
    }
    std::cout << "Total digis in seed window: " << fvFullDigiCount.at(iEvent);
    std::cout << ", Noise digis in seed window: " << fvNoiseDigiCount.at(iEvent) << std::endl;
    std::cout << "Fraction of correctly matched digis in seed window: " << fvCorrectDigiRatio.at(iEvent) << std::endl;
    std::cout << "Fraction of correctly matched digis in seed window (disregarding noise): "
              << fvCorrectDigiRatioNoNoise.at(iEvent) << std::endl;
    std::cout << "Fraction of digis of matched event found in seed window: " << fvFoundDigiRatio.at(iEvent);
    std::cout << " (only from this timeslice)" << std::endl;
  }
  FillHistos();
  WriteHistos();
}

void CbmSeedFinderQa::WriteHistos()
{
  fCanv->cd(1);
  fhCorrectDigiRatio->DrawCopy("colz", "");

  fCanv->cd(2);
  fhCorrectDigiRatioNoNoise->DrawCopy("colz", "");

  fCanv->cd(3);
  fhNoiseDigiRatio->DrawCopy("colz", "");

  fCanv->cd(4);
  fhFoundDigiRatio->DrawCopy("colz", "");

  fCanv->cd(5);
  fhCorrectVsFound->DrawCopy("colz", "");

  fCanv->cd(6);
  fhCorrectVsFoundNoNoise->DrawCopy("colz", "");

  fCanv->cd(7);
  fhTimeOffset->DrawCopy("colz", "");

  fCanv->cd(8);
  fhTimeOffsetSingletOnly->DrawCopy("colz", "");

  fCanv->cd(9);
  fhTimeOffsetClosest->DrawCopy("colz", "");

  fCanv->cd(10);
  fhLinkedMCEventsPerTrigger->DrawCopy("colz", "");

  fCanv->cd(11);
  fhLinkedTriggersPerMCEvent->DrawCopy("colz", "");

  fCanv->cd(12);
  fhMatchedTriggersPerMCEvent->DrawCopy("colz", "");

  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
