/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmSeedFinderSlidingWindow.h"

#include "CbmMatch.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

CbmSeedFinderSlidingWindow::~CbmSeedFinderSlidingWindow()
{
  if (fvEventMatches != nullptr) { delete fvEventMatches; }
  if (fvSeedTimesFull != nullptr) { delete fvSeedTimesFull; }
  if (fvFullDigiCount != nullptr) { delete fvFullDigiCount; }
  if (fvNoiseDigiCount != nullptr) { delete fvNoiseDigiCount; }
  if (fvCorrectDigiRatio != nullptr) { delete fvCorrectDigiRatio; }
  if (fvFoundDigiRatio != nullptr) { delete fvFoundDigiRatio; }
}

template<>
double CbmSeedFinderSlidingWindow::GetTime(const std::vector<double>* vIn, int32_t i);

template<class inType>
void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<inType>* vIn, const std::vector<CbmMatch>* vDigiMatch)
{
  const int32_t nDigisTot = vIn->size();
  int32_t nDigisWin       = 0;
  int32_t winStartN       = 0;
  double winStartT        = -1111;

  // Reset output array
  fvSeedTimes->clear();
  //LOG(debug) << "CbmTaskBuildRawEvents::FillSeedTimesSlidingWindow:  digis in slice " << nDigisTot;

  for (int32_t i = 0; i < nDigisTot; i++) {
    const double currentT = GetTime(vIn, i);
    int32_t j             = 0;

    nDigisWin++;
    if (winStartT == -1111) {
      winStartN = i;
      winStartT = currentT;
    }
    if (currentT - winStartT > fdWindDur) {
      for (j = winStartN; j < i; j++) {

        winStartT = GetTime(vIn, j);
        if (currentT - winStartT <= fdWindDur) {
          winStartN = j;
          break;
        }
        nDigisWin--;
      }
      if (j == i) {
        winStartN = i;
        winStartT = currentT;
      }
    }

    if (nDigisWin >= fminDigis) {
      // Reached required number of digis
      fvSeedTimes->push_back((currentT + winStartT)
                             / 2.);  //one possibility. perhaps better to place seed at end of window.

      if (vDigiMatch && fvEventMatches) {  // QA mode
        FillEventMatch(winStartN, i, vDigiMatch);
        fvSeedTimesFull->push_back((currentT + winStartT) / 2.);
      }
      nDigisWin = 0;

      for (j = i + 1; j < nDigisTot; j++) {
        const double newStartT = GetTime(vIn, j);
        if (newStartT - currentT > fdDeadT) break;
      }
      if (j == nDigisTot) {
        //Reached the end of the slice
        break;
      }
      i         = j - 1;
      winStartN = j;
      winStartT = GetTime(vIn, j);
    }
  }

  if (vDigiMatch && fvEventMatches) {  // QA mode
    std::cout << "CbmSeedFinderSlidingWindow::FillSeedTimes(): Found " << GetNofSeeds() << " seeds for this timeslice."
              << std::endl;
  }
}
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmMuchBeamTimeDigi>*,
                                                        const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmMuchDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmPsdDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmRichDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmStsDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmTofDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<CbmTrdDigi>*, const std::vector<CbmMatch>*);
template void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<double>*, const std::vector<CbmMatch>*);

template<class inType>
double CbmSeedFinderSlidingWindow::GetTime(const std::vector<inType>* vIn, int32_t i)
{
  const inType* digi = &(vIn->at(i));
  if (digi == nullptr) {
    std::cout << "CbmSeedFinderSlidingWindow: Error, non-allocated digi in input vector at position: " << i
              << std::endl;
  }
  return digi->GetTime();
}

template<>
double CbmSeedFinderSlidingWindow::GetTime(const std::vector<double>* vIn, int32_t i)
{
  return vIn->at(i);
}

void CbmSeedFinderSlidingWindow::FillEventMatch(int32_t WinStart, int32_t WinEnd,
                                                const std::vector<CbmMatch>* vDigiMatch)
{
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
      int32_t file  = digiMatch->GetLink(iLink).GetFile();
      double weight = digiMatch->GetLink(iLink).GetWeight();
      //     LOG(info) << "Adding link (number, weight, entry, file): " << iLink << " " << weight << " "
      //              << entry << " " << file;
      seedMatch.AddLink(weight, 0, entry, file);
    }
  }
  fvFullDigiCount->push_back(digiCount);
  fvNoiseDigiCount->push_back(noiseDigiCount);

  if (seedMatch.GetNofLinks() == 0)  //seed is only noise digis
  {
    seedMatch.AddLink(1.0, 0, -1, 0);
    fvEventMatches->push_back(seedMatch);
    fvCorrectDigiRatio->push_back(-1.0);
    fvFoundDigiRatio->push_back(-1.0);
    return;
  }
  else {
    fvEventMatches->push_back(seedMatch);
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
  const double correctDigiRatio = (double) correctDigiCount / (digiCount - noiseDigiCount);
  fvCorrectDigiRatio->push_back(correctDigiRatio);

  //found digis of matched event in seed window
  for (uint32_t iDigi = 0; iDigi < vDigiMatch->size(); iDigi++) {
    const CbmMatch* digiMatch = &(vDigiMatch->at(iDigi));
    if (digiMatch->GetNofLinks() == 0) { continue; }  //skip digis with no links to avoid T0 pollution
    const int matchedEvent = digiMatch->GetMatchedLink().GetEntry();
    if (matchedEvent == seedMatch.GetMatchedLink().GetEntry()) { matchedEventDigiCount++; }
  }
  const double foundDigiRatio = (double) correctDigiCount / matchedEventDigiCount;
  fvFoundDigiRatio->push_back(foundDigiRatio);
}

void CbmSeedFinderSlidingWindow::SetQa(bool doQA)
{
  if (doQA == true) {
    if (fvEventMatches == nullptr) { fvEventMatches = new std::vector<CbmMatch>; };
    if (fvSeedTimesFull == nullptr) { fvSeedTimesFull = new std::vector<double>; };
    if (fvFullDigiCount == nullptr) { fvFullDigiCount = new std::vector<int32_t>; };
    if (fvNoiseDigiCount == nullptr) { fvNoiseDigiCount = new std::vector<int32_t>; };
    if (fvCorrectDigiRatio == nullptr) { fvCorrectDigiRatio = new std::vector<double>; };
    if (fvFoundDigiRatio == nullptr) { fvFoundDigiRatio = new std::vector<double>; };
    fvEventMatches->clear();
    fvFullDigiCount->clear();
    fvNoiseDigiCount->clear();
    fvSeedTimesFull->clear();
    fvCorrectDigiRatio->clear();
    fvFoundDigiRatio->clear();
  }
  else {
    if (fvEventMatches != nullptr) { delete fvEventMatches; }
    if (fvSeedTimesFull != nullptr) { delete fvSeedTimesFull; }
    if (fvFullDigiCount != nullptr) { delete fvFullDigiCount; }
    if (fvNoiseDigiCount != nullptr) { delete fvNoiseDigiCount; }
    if (fvCorrectDigiRatio != nullptr) { delete fvCorrectDigiRatio; }
    if (fvFoundDigiRatio != nullptr) { delete fvFoundDigiRatio; }
  }
}

void CbmSeedFinderSlidingWindow::OutputQa()
{
  if (fvEventMatches == nullptr) {
    std::cout << "CbmSeedFinderSlidingWindow::OutputQa(): Error, QA mode not active (no event matches present)."
              << std::endl;
    exit(1);
  }
  for (uint32_t iEvent = 0; iEvent < fvEventMatches->size(); iEvent++) {
    const CbmMatch* match   = &(fvEventMatches->at(iEvent));
    const int32_t mcEventNr = match->GetMatchedLink().GetEntry();

    std::cout << "QA for seed # " << iEvent << std::endl;
    std::cout << "Seed time: " << fvSeedTimesFull->at(iEvent) << std::endl;
    std::cout << "Links to MC events: " << match->GetNofLinks() << ", matched MC event number " << mcEventNr
              << std::endl;
    if (mcEventNr == -1) {
      std::cout << "Warning: Seed was constructed from noise digis only (MC event = -1)!" << std::endl;
      std::cout << "         Please increase your noise threshold!" << std::endl;
    }
    std::cout << "Total digis in seed window: " << fvFullDigiCount->at(iEvent);
    std::cout << ", Noise digis in seed window: " << fvNoiseDigiCount->at(iEvent) << std::endl;
    std::cout << "Fraction of correctly matched digis in seed window (disregarding noise): "
              << fvCorrectDigiRatio->at(iEvent) << std::endl;
    std::cout << "Fraction of digis of matched event found in seed window: " << fvFoundDigiRatio->at(iEvent);
    std::cout << " (only from this timeslice)" << std::endl;
  }
}
