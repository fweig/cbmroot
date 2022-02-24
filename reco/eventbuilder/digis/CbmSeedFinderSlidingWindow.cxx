/* Copyright (C) 2007-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmSeedFinderSlidingWindow.h"

#include "CbmMCEventList.h"
#include "CbmMatch.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmSeedFinderQa.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "FairRootManager.h"
#include <Logger.h>

CbmSeedFinderSlidingWindow::~CbmSeedFinderSlidingWindow()
{
  if (fQa != nullptr) { delete fQa; }
}

template<>
double CbmSeedFinderSlidingWindow::GetTime(const std::vector<double>* vIn, int32_t i);

template<class inType>
void CbmSeedFinderSlidingWindow::FillSeedTimes(const std::vector<inType>* vIn, const std::vector<CbmMatch>* vDigiMatch)
{
  // Reset output array
  fvSeedTimes->clear();

  // Ideal mode ignores digi input and copies MC event list
  if (fbIdealMode) {

    std::vector<CbmMatch> eventMatches;

    for (uint32_t i = 0; i < fEventList->GetNofEvents(); i++) {

      if (fIdealModeFileId != -1 && fEventList->GetFileIdByIndex(i) != fIdealModeFileId) { continue; }
      const double seedTime = fEventList->GetEventTimeByIndex(i);
      fvSeedTimes->push_back(seedTime);

      if (fQa) {
        const uint32_t eventId = fEventList->GetEventIdByIndex(i);
        const uint32_t fileId  = fEventList->GetFileIdByIndex(i);
        CbmMatch eventMatch;
        eventMatch.AddLink(1.0, 0, eventId, fileId);
        eventMatches.push_back(eventMatch);
      }
    }
    if (fQa) {
      for (uint32_t i = 0; i < fvSeedTimes->size(); i++) {
        fQa->FillQaInfo(i, i, &eventMatches, fvSeedTimes->at(i));
      }
    }
    return;
  }

  const int32_t nDigisTot = vIn->size();
  int32_t nDigisWin       = 0;
  int32_t winStartN       = 0;
  double winStartT        = -1111;

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
      const double seedTime =
        fdOffset + (currentT + winStartT) / 2.;  //one possibility. perhaps better to place seed at end of window.
      fvSeedTimes->push_back(seedTime);

      if (vDigiMatch && fQa) {  // QA mode
        fQa->FillQaInfo(winStartN, i, vDigiMatch, seedTime);
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

  //if (fQa && vDigiMatch) {  // QA mode
  //  std::cout << "CbmSeedFinderSlidingWindow::FillSeedTimes(): Found " << GetNofSeeds() << " seeds for this timeslice."
  //            << std::endl;
  //}
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

void CbmSeedFinderSlidingWindow::SetQa(bool doQA)
{
  if (doQA == true) {
    if (fQa == nullptr) { fQa = new CbmSeedFinderQa(); }
  }
  else {
    if (fQa != nullptr) { delete fQa; }
  }
}

void CbmSeedFinderSlidingWindow::Init()
{
  if (fQa) { fQa->Init(); }

  if (fbIdealMode) {
    if (!FairRootManager::Instance() || !FairRootManager::Instance()->GetObject("MCEventList.")) {
      LOG(error) << "No MC event list found";
      return;
    }
    fEventList = (CbmMCEventList*) FairRootManager::Instance()->GetObject("MCEventList.");
  }
}

void CbmSeedFinderSlidingWindow::OutputQa()
{
  if (fQa) { fQa->OutputQa(); }
}
