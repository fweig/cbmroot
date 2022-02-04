/* Copyright (C) 2007-2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmSeedFinderQa.h"

void CbmSeedFinderQa::FillQaInfo(const int32_t WinStart, const int32_t WinEnd, const std::vector<CbmMatch>* vDigiMatch,
                                 const double seedTime)
{
  fvSeedTimesFull.push_back(seedTime);

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
  fvFullDigiCount.push_back(digiCount);
  fvNoiseDigiCount.push_back(noiseDigiCount);

  if (seedMatch.GetNofLinks() == 0)  //seed is only noise digis
  {
    seedMatch.AddLink(1.0, 0, -1, 0);
    fvEventMatches.push_back(seedMatch);
    fvCorrectDigiRatio.push_back(-1.0);
    fvFoundDigiRatio.push_back(-1.0);
    return;
  }
  else {
    fvEventMatches.push_back(seedMatch);
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
  fvCorrectDigiRatio.push_back(correctDigiRatio);

  //found digis of matched event in seed window
  for (uint32_t iDigi = 0; iDigi < vDigiMatch->size(); iDigi++) {
    const CbmMatch* digiMatch = &(vDigiMatch->at(iDigi));
    if (digiMatch->GetNofLinks() == 0) { continue; }  //skip digis with no links to avoid T0 pollution
    const int matchedEvent = digiMatch->GetMatchedLink().GetEntry();
    if (matchedEvent == seedMatch.GetMatchedLink().GetEntry()) { matchedEventDigiCount++; }
  }
  const double foundDigiRatio = (double) correctDigiCount / matchedEventDigiCount;
  fvFoundDigiRatio.push_back(foundDigiRatio);
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
    std::cout << "Fraction of correctly matched digis in seed window (disregarding noise): "
              << fvCorrectDigiRatio.at(iEvent) << std::endl;
    std::cout << "Fraction of digis of matched event found in seed window: " << fvFoundDigiRatio.at(iEvent);
    std::cout << " (only from this timeslice)" << std::endl;
  }
}
