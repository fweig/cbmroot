/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaBase.cxx
 * @brief  Base class for the tracking input QA (definition)
 * @since  24.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackingInputQaBase.h"

#include "CbmCluster.h"
#include "CbmDigiManager.h"
#include "CbmMCEventList.h"
#include "CbmMatch.h"
#include "CbmPixelHit.h"
#include "CbmStsAddress.h"  // TODO: TMP for tests, must be removed!!!! (S.Zharko)
#include "CbmTimeSlice.h"

#include "FairLogger.h"

#include "TClonesArray.h"

#include <iostream>

// --------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaBase::CbmTrackingInputQaBase(const char* name, int verbose) : FairTask(name, verbose) {}

// --------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaBase::~CbmTrackingInputQaBase() {}

ClassImp(CbmTrackingInputQaBase);

//
// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::Clear(Option_t*)
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::Clear(Option_t*)\n";
}
//
// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::Exec(Option_t*)
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::Exec(Option_t*)\n";

  // Run resolution Qa
  ResolutionQa();

  // Update number of events
  fNevents.SetVal(fNevents.GetVal() + 1);
}
//
// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::Finish() { std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::Finish()\n"; }
//
// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaBase::Init()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::Init()\n";

  // If detector is not present, fpDetectorInterface is nullptr
  if (!fpDetectorInterface) { LOG(fatal) << "CbmTrackingInputQa: Tracking detector interface is undefined"; }

  // Input data initialization
  InitInputData();

  // Base histograms initialization
  InitBaseHistograms();

  // Register histograms in output file
  // TODO: Add subfolders for each subsystem like
  fpOutFolder =
    std::make_shared<TFolder>(TString("TrackingInputQa_") + fDetName, TString("TrackingInputQa_") + fDetName);
  fpOutFolder->SetOwner(false);
  fpOutFolderHists = fpOutFolder->AddFolder("rawHist", "Raw histograms");
  gStyle->SetOptStat(0);

  fNevents.SetVal(0);
  fpOutFolderHists->Add(&fNevents);

  // Register histograms in the output folder
  for (auto* histo : fHistList) {
    fpOutFolderHists->Add(histo);
  }

  // Geometry QA
  GeometryQa();

  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaBase::InitInputData()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::InitInputData()\n";

  auto* manager = FairRootManager::Instance();
  fpDigiManager = CbmDigiManager::Instance();
  fpDigiManager->Init();  // NOTE: is initialized only once

  // Reconstructed data initialization
  fpTimeSlice = dynamic_cast<CbmTimeSlice*>(manager->GetObject("TimeSlice."));
  if (!fpTimeSlice) {
    LOG(error) << "Time slice was not found";
    return kERROR;
  }

  fpHits = GetHitsInput();
  if (!fpHits) {
    LOG(error) << "Hits input array was not found for " << fDetTitle;
    return kERROR;
  }

  fpClusters = GetClustersInput();
  if (!fpClusters) {
    LOG(error) << "Cluster input array was not found for " << fDetTitle;
    return kERROR;
  }

  // MC data initialization
  fpMcManager  = dynamic_cast<CbmMCDataManager*>(manager->GetObject("MCDataManager"));
  fIsMcPresent = bool(fpMcManager);

  if (fIsMcPresent) {
    fpMcEventList = dynamic_cast<CbmMCEventList*>(manager->GetObject("MCEventList."));
    fpMcTracks    = fpMcManager->InitBranch("MCTrack");
    fpMcPoints    = GetMcPointsInput();
    fpHitMatches  = GetHitMatchesInput();

    if (!fpMcEventList) {
      LOG(error) << "MCEventList data were not found";
      return kERROR;
    }

    if (!fpMcTracks) {
      LOG(error) << "MC tracks data were not found";
      return kERROR;
    }

    if (!fpMcPoints) {
      LOG(error) << "MC points data were not found for " << fDetTitle;
      return kERROR;
    }

    if (!fpHitMatches) {
      LOG(error) << "Hits match data were not found for " << fDetTitle;
      return kERROR;
    }

    if (!fpDigiManager->IsMatchPresent(fDetId)) {
      LOG(error) << "No digi matches were found for " << fDetTitle;
      return kERROR;
    }
  }

  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaBase::InitBaseHistograms()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::InitBaseHistograms()\n";

  fHistResidualX.clear();
  fHistResidualY.clear();
  fHistResidualT.clear();

  fHistPullX.clear();
  fHistPullY.clear();
  fHistPullT.clear();

  fHistPointsPerHit.clear();
  fHistHitsPerPoint.clear();

  int nTrackingStations = fpDetectorInterface->GetNtrackingStations();

  fHistResidualX.reserve(nTrackingStations);
  fHistResidualY.reserve(nTrackingStations);
  fHistResidualT.reserve(nTrackingStations);

  fHistPullX.reserve(nTrackingStations);
  fHistPullY.reserve(nTrackingStations);
  fHistPullT.reserve(nTrackingStations);

  fHistPointsPerHit.reserve(nTrackingStations);
  fHistHitsPerPoint.reserve(nTrackingStations);

  // Create histograms
  TString histName  = "";
  TString histTitle = "";
  int nBins         = -1;
  int rMin          = 0.;
  int rMax          = 0.;
  for (int iSt = 0; iSt < nTrackingStations; ++iSt) {
    // Residuals
    histName                    = fDetName + Form("_st%d_h1ResidualX", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Residual X; (X_{reco} - X_{MC}) [cm]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualX;
    fHistResidualX.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = fDetName + Form("_st%d_h1ResidualY", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Residual Y; (Y_{reco} - Y_{MC}) [cm]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualY;
    fHistResidualY.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = fDetName + Form("_st%d_h1ResidualT", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Residual T; (T_{reco} - T_{MC}) [cm]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualT;
    fHistResidualT.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // Pulls
    histName                    = fDetName + Form("_st%d_h1PullX", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Pull X; (X_{reco} - X_{MC}) / #sigmaX_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullX;
    fHistPullX.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = fDetName + Form("_st%d_h1PullY", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Pull Y; (Y_{reco} - Y_{MC}) / #sigmaY_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullY;
    fHistPullY.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = fDetName + Form("_st%d_h1PullT", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Pull T; (T_{reco} - T_{MC}) / #sigmaT_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullT;
    fHistPullT.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // MC points per one hit
    histName                    = fDetName + Form("_st%d_h1PointsPerHit", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: MC Points per Hit; N_{MC Points} per hit", iSt);
    std::tie(nBins, rMin, rMax) = fRangePointsPerHit;
    fHistPointsPerHit.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // Hits per one mc point
    histName                    = fDetName + Form("_st%d_h1HitsPerPoint", iSt);
    histTitle                   = fDetTitle + Form(" Station %d: Hit per MC points; N_{hits} per MC point", iSt);
    std::tie(nBins, rMin, rMax) = fRangeHitsPerPoint;
    fHistHitsPerPoint.emplace_back(histName, histTitle, nBins, rMin, rMax);
  }

  // Register histograms
  for (int iSt = 0; iSt < nTrackingStations; ++iSt) {
    RegisterHist(&fHistResidualX[iSt]);
    RegisterHist(&fHistResidualY[iSt]);
    RegisterHist(&fHistResidualT[iSt]);

    RegisterHist(&fHistPullX[iSt]);
    RegisterHist(&fHistPullY[iSt]);
    RegisterHist(&fHistPullT[iSt]);

    RegisterHist(&fHistPointsPerHit[iSt]);
    RegisterHist(&fHistHitsPerPoint[iSt]);
  }

  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::Reset() { std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::Reset()\n"; }

// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::SetParContainers()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::SetParContainers()\n";
}

// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaBase::GeometryQa()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::GeometryQa()\n";
  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaBase::ResolutionQa()
{
  std::cout << "\033[1;32mCALL:\033[0mCbmTrackingInputQaBase::ResolutionQa()\n";

  // Resolution QA is impossible without MC information
  if (!fIsMcPresent) { return; }

  int nHits     = fpHits->GetEntriesFast();                     // Hits in the event/time slice
  int nClusters = fpClusters->GetEntriesFast();                 // Clusters in the event/time slice
  int nDigis    = fpDigiManager->GetNofDigis(fDetId);           // Number of digies
  int nStations = fpDetectorInterface->GetNtrackingStations();  // Number of tracking stations

  // *********************************************
  // ** Loop over hits in this event/time slice **
  // *********************************************

  for (int iHit = 0; iHit < nHits; ++iHit) {
    /// TODO: put it into a function from iHit

    auto* hit = dynamic_cast<CbmPixelHit*>(fpHits->At(iHit));
    if (!hit) { LOG(error) << "There is an empty hit at position " << iHit << " (" << fDetTitle << ")"; }

    // Tracking station index
    int iStation = fpDetectorInterface->GetTrackingStationIndex(hit);
    if (iStation < 0 || iStation >= nStations) {
      LOG(fatal) << "Wrong tracking station index was calculated by the tracking detector interface for " << fDetTitle
                 << ": station index = " << iStation << " (expected in the range [0, " << nStations - 1 << "])";
      return;
    }

  }  // Loop over hits in this event/time slice: END
}
