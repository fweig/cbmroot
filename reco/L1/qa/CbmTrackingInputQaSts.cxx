/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingInputQaSts.cxx
 * @brief  Base class for the tracking input QA (definition)
 * @since  24.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#include "CbmTrackingInputQaSts.h"

#include "CbmDigiManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmStsAddress.h"  // TODO: TMP for tests, must be removed!!!! (S.Zharko)
#include "CbmStsCluster.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsTrackingInterface.h"  // Communicate via tracking detector interface
#include "CbmTimeSlice.h"

#include <FairRootManager.h>
#include <Logger.h>

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include <iostream>
#include <stdexcept>

ClassImp(CbmTrackingInputQaSts);

// ---------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaSts::CbmTrackingInputQaSts(int verbosity) : FairTask("CbmTrackingInputQaSts", verbosity) {}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmTrackingInputQaSts::~CbmTrackingInputQaSts() { DeInit(); }

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmTrackingInputQaSts::CheckDistributions()
{
  bool res = true;

  const int nStations = fpDetectorInterface->GetNtrackingStations();

  // ** Check pulls **
  //
  // Function to compare pulls distributions RMS with 1
  // TODO: Choose proper selection criteria (S.Zharko)
  auto CheckPullsDistribution = [&](const TH1& pHist) {
    // Checks sigma of distribution fit with unity
    auto* pFitFunc = pHist.GetFunction("gaus");
    if (!pFitFunc) {
      LOG(error) << "STS tracking input QA: fit function not found for histogram \"" << pHist.GetName() << '\"';
      return false;
    }
    auto vSigma = pFitFunc->GetParameter(2);
    auto eSigma = pFitFunc->GetParError(2);

    // Select 3 sigma interval
    LOG(info) << "Checking histogram \"" << pHist.GetName() << "\": sigma(fit) = " << vSigma << " +/- " << eSigma;
    if (std::fabs(vSigma - 1.) < 3. * eSigma) { return true; }
    else {
      return false;
    }

    //auto vRms = pHist.GetRMS();      // RMS value
    //auto eRms = pHist.GetRMSError(); // RMS error
    //std::cout << std::fabs(vRms - 1.) << '\n';
    //if (eRms > vRms * fMaxPullsRmsDiff ) {
    //  LOG(error) << "STS tracking input QA: the analysis of \"" << pHist.GetName() << "\" RMS is impossible due to "
    //             << "lack of statistics\n";
    //  // TODO: probably, here we can use fit? (S.Zharko)
    //  return false;
    //}
    //if (std::fabs(vRms - 1.) < fMaxPullsRmsDiff) {
    //  return true;
    //}
    //else {
    //  LOG(info) << "STS tracking input QA: RMS check for pulls distribution failed (histogram: \"" << pHist.GetName() << "\"): "
    //            << "RMS = " << vRms << " +/- " << eRms;
    //  return false;
    //}
    return false;
  };


  for (int iSt = 0; iSt < nStations; ++iSt) {
    res = CheckPullsDistribution(fHistPullX[iSt]) && res;
    res = CheckPullsDistribution(fHistPullY[iSt]) && res;
    res = CheckPullsDistribution(fHistPullT[iSt]) && res;
  }

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaSts::DeInit()
{
  fIsMcPresent = false;

  fpDetectorInterface = nullptr;
  fpTimeSlice         = nullptr;
  fpDigiManager       = nullptr;

  fpMcManager = nullptr;
  fpMcTracks  = nullptr;
  fpMcPoints  = nullptr;

  fpMcEventList    = nullptr;
  fpClusters       = nullptr;
  fpHits           = nullptr;
  fpClusterMatches = nullptr;
  fpHitMatches     = nullptr;

  fOutFolder.Clear();
  fpOutFolderHists = nullptr;

  fNevents.SetVal(0);

  fHistResidualX.clear();
  fHistResidualY.clear();
  fHistResidualT.clear();

  fHistPullX.clear();
  fHistPullY.clear();
  fHistPullT.clear();

  fHistPointsPerHit.clear();
  fHistHitsPerPoint.clear();

  fHistEfficiencyXY.clear();
  fHistEfficiencyR.clear();
}

// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaSts::Exec(Option_t*)
{
  // Run resolution QA
  ResolutionQa();

  // Update number of events
  fNevents.SetVal(fNevents.GetVal() + 1);
}

// --------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaSts::Finish()
{
  // Fit histograms
  bool isFitSucceed = this->FitHistograms();
  if (!isFitSucceed) { LOG(error) << "STS tracking input QA: histograms could not be fitted"; }

  // Check accumulated distributions
  bool areResolutionsOk = isFitSucceed && CheckDistributions();

  // Add output to a sink
  auto* pSink = FairRootManager::Instance()->GetSink();
  if (pSink) { pSink->WriteObject(&GetQa(), nullptr); }

  // TODO: Collect all the flags in one place and make a decision here (S.Zharko)

  if (areResolutionsOk) { LOG(info) << this->GetName() << ": \033[1;32mtask succeeded\033[0m"; }
  else {
    LOG(info) << this->GetName() << ": \033[1;31mtask failed\033[0m";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmTrackingInputQaSts::FitHistograms()
{
  bool res = true;  // flag: true - fit succeed, false - fit failed

  // Function, which provides fit of a histogram with necessary checks
  // If the histogram is empty, the function returns false.
  auto FitHistogram = [](TH1& hist, const char* fcnname, const char* fitopt) {
    if (hist.GetEntries() > 0) {
      hist.Fit(fcnname, fitopt);
      return true;
    }
    else {
      LOG(warn) << "STS tracking input QA: attempt to fit an empty histogram \"" << hist.GetName() << '\"';
      return false;
    }
  };

  const int nStations = fpDetectorInterface->GetNtrackingStations();
  for (int iSt = 0; iSt < nStations; ++iSt) {
    // Fit histograms
    res = FitHistogram(fHistResidualX[iSt], "gaus", "Q") && res;
    res = FitHistogram(fHistResidualY[iSt], "gaus", "Q") && res;
    res = FitHistogram(fHistResidualT[iSt], "gaus", "Q") && res;
    res = FitHistogram(fHistPullX[iSt], "gaus", "Q") && res;
    res = FitHistogram(fHistPullY[iSt], "gaus", "Q") && res;
    res = FitHistogram(fHistPullT[iSt], "gaus", "Q") && res;
  }

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::GeometryQa()
{
  // Test stations ordering: TODO - is it needed here? This test is done in L1
  return kSUCCESS;
}


// ---------------------------------------------------------------------------------------------------------------------
//
TFolder& CbmTrackingInputQaSts::GetQa()
{
  //gStyle->SetPaperSize(20, 20);

  // Loop over tracking stations
  const int nStations = fpDetectorInterface->GetNtrackingStations();
  for (int iSt = 0; iSt < nStations; ++iSt) {
    // Draw histograms
    fCanvResidualX.cd(iSt + 1);
    fHistResidualX[iSt].DrawCopy("", "");
    fCanvResidualY.cd(iSt + 1);
    fHistResidualY[iSt].DrawCopy("", "");
    fCanvResidualT.cd(iSt + 1);
    fHistResidualT[iSt].DrawCopy("", "");

    fCanvPullX.cd(iSt + 1);
    fHistPullX[iSt].DrawCopy("", "");
    fCanvPullY.cd(iSt + 1);
    fHistPullY[iSt].DrawCopy("", "");
    fCanvPullT.cd(iSt + 1);
    fHistPullT[iSt].DrawCopy("", "");

    fCanvPointsPerHit.cd(iSt + 1);
    fHistPointsPerHit[iSt].DrawCopy("", "");

    fCanvHitsPerPoint.cd(iSt + 1);
    fHistHitsPerPoint[iSt].DrawCopy("", "");

    fCanvEfficiencyR.cd(iSt + 1);
    fHistEfficiencyR[iSt].DrawCopy("", "");

    fCanvEfficiencyXY.cd(iSt + 1);
    fHistEfficiencyXY[iSt].DrawCopy("colz", "");
  }  // Loop over tracking stations: end
  return fOutFolder;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::Init()
{
  // Reset data fields
  DeInit();

  // If detector is not present, fpDetectorInterface is nullptr
  fpDetectorInterface = CbmStsTrackingInterface::Instance();
  if (!fpDetectorInterface) { LOG(fatal) << "CbmTrackingInputQa: Tracking detector interface is undefined"; }

  // Input data initialization
  ReadAndCreateDataBranches();

  // Histograms initialization
  InitHistograms();

  // Canvases initialization
  InitCanvases();

  // Register histograms in output file
  // TODO: Add subfolders for each subsystem like
  fOutFolder.SetOwner(false);
  fpOutFolderHists = fOutFolder.AddFolder("rawHist", "Raw histograms");
  gStyle->SetOptStat(0);

  fNevents.SetVal(0);  // redundant
  fpOutFolderHists->Add(&fNevents);

  // Register histograms in the output folder
  for (auto* histo : fHistList) {
    fpOutFolderHists->Add(histo);
  }

  // Do QA of geometry in the end of initialization
  return GeometryQa();
}


// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::InitCanvases()
{
  // Reset canvases
  fCanvResidualX.Clear();
  fCanvResidualY.Clear();
  fCanvResidualT.Clear();

  fCanvPullX.Clear();
  fCanvPullY.Clear();
  fCanvPullT.Clear();

  fCanvPointsPerHit.Clear();
  fCanvHitsPerPoint.Clear();

  fCanvEfficiencyR.Clear();
  fCanvEfficiencyXY.Clear();

  // Divide canvases into sections to store plots vs. station ID
  fCanvResidualX.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvResidualY.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvResidualT.Divide2D(fpDetectorInterface->GetNtrackingStations());

  fCanvPullX.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvPullY.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvPullT.Divide2D(fpDetectorInterface->GetNtrackingStations());

  fCanvPointsPerHit.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvHitsPerPoint.Divide2D(fpDetectorInterface->GetNtrackingStations());

  fCanvEfficiencyR.Divide2D(fpDetectorInterface->GetNtrackingStations());
  fCanvEfficiencyXY.Divide2D(fpDetectorInterface->GetNtrackingStations());

  // Add the canvases to the output folder
  fOutFolder.Add(&fCanvResidualX);
  fOutFolder.Add(&fCanvResidualY);
  fOutFolder.Add(&fCanvResidualT);

  fOutFolder.Add(&fCanvPullX);
  fOutFolder.Add(&fCanvPullY);
  fOutFolder.Add(&fCanvPullT);

  fOutFolder.Add(&fCanvPointsPerHit);
  fOutFolder.Add(&fCanvHitsPerPoint);

  fOutFolder.Add(&fCanvEfficiencyR);
  fOutFolder.Add(&fCanvEfficiencyXY);

  return kSUCCESS;
}

// --------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::InitHistograms()
{
  gStyle->SetOptStat();

  fHistResidualX.clear();
  fHistResidualY.clear();
  fHistResidualT.clear();

  fHistPullX.clear();
  fHistPullY.clear();
  fHistPullT.clear();

  fHistPointsPerHit.clear();
  fHistHitsPerPoint.clear();

  fHistEfficiencyR.clear();
  fHistEfficiencyXY.clear();

  const int nStations = fpDetectorInterface->GetNtrackingStations();

  fHistResidualX.reserve(nStations);
  fHistResidualY.reserve(nStations);
  fHistResidualT.reserve(nStations);

  fHistPullX.reserve(nStations);
  fHistPullY.reserve(nStations);
  fHistPullT.reserve(nStations);

  fHistPointsPerHit.reserve(nStations);
  fHistHitsPerPoint.reserve(nStations);

  fHistEfficiencyR.reserve(nStations);
  fHistEfficiencyXY.reserve(nStations);

  // Create histograms
  TString histName  = "";
  TString histTitle = "";
  int nBins         = -1;
  double rMin       = 0.;
  double rMax       = 0.;
  for (int iSt = 0; iSt < nStations; ++iSt) {
    // Residuals
    histName                    = Form("h1_sts_ResidualX_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Residual X; (X_{reco} - X_{MC}) [cm]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualX;
    fHistResidualX.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = Form("h1_sts_ResidualY_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Residual Y; (Y_{reco} - Y_{MC}) [cm]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualY;
    fHistResidualY.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = Form("h1_sts_ResidualT_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Residual T; (T_{reco} - T_{MC}) [ns]", iSt);
    std::tie(nBins, rMin, rMax) = fRangeResidualT;
    fHistResidualT.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // Pulls
    histName                    = Form("h1_sts_PullX_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Pull X; (X_{reco} - X_{MC}) / #sigmaX_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullX;
    fHistPullX.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = Form("h1_sts_PullY_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Pull Y; (Y_{reco} - Y_{MC}) / #sigmaY_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullY;
    fHistPullY.emplace_back(histName, histTitle, nBins, rMin, rMax);

    histName                    = Form("h1_sts_PullT_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Pull T; (T_{reco} - T_{MC}) / #sigmaT_{reco}", iSt);
    std::tie(nBins, rMin, rMax) = fRangePullT;
    fHistPullT.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // MC points per one hit
    histName                    = Form("h1_sts_PointsPerHit_st%d", iSt);
    histTitle                   = Form("STS: Station %d: MC Points per Hit; N_{MC Points} per hit", iSt);
    std::tie(nBins, rMin, rMax) = fRangePointsPerHit;
    fHistPointsPerHit.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // Hits per one mc point
    histName                    = Form("h1_sts_HitsPerPoint_st%d", iSt);
    histTitle                   = Form("STS: Station %d: Hit per MC points; N_{hits} per MC point", iSt);
    std::tie(nBins, rMin, rMax) = fRangeHitsPerPoint;
    fHistHitsPerPoint.emplace_back(histName, histTitle, nBins, rMin, rMax);

    // Efficiencies
    double xMax = fpDetectorInterface->GetXmax(iSt);  /// TODO: test ranges
    double yMax = fpDetectorInterface->GetYmax(iSt);  /// TODO: test ranges

    // Efficiency vs. distance of point from center
    histName  = Form("pr1_sts_EfficiencyR_st%d", iSt);
    histTitle = Form("STS: Station %d: Efficiency R; R [cm]", iSt);
    rMin      = 0.;
    rMax      = sqrt(xMax * xMax + yMax * yMax);
    nBins     = 100;
    fHistEfficiencyR.emplace_back(histName, histTitle, nBins, rMin, rMax);
    fHistEfficiencyR[iSt].SetOptStat(1110);

    // Efficiency vs. x and y
    histName  = Form("pr1_sts_EfficiencyXY_st%d", iSt);
    histTitle = Form("STS: Station %d: Efficiency XY; X [cm]; Y [cm]", iSt);
    nBins     = 50;
    fHistEfficiencyXY.emplace_back(histName, histTitle, nBins, -xMax, xMax, nBins, -yMax, yMax);
    fHistEfficiencyXY[iSt].SetOptStat(10);
  }

  // Register histograms
  for (int iSt = 0; iSt < nStations; ++iSt) {
    RegisterHist(&fHistResidualX[iSt]);
    RegisterHist(&fHistResidualY[iSt]);
    RegisterHist(&fHistResidualT[iSt]);

    RegisterHist(&fHistPullX[iSt]);
    RegisterHist(&fHistPullY[iSt]);
    RegisterHist(&fHistPullT[iSt]);

    RegisterHist(&fHistPointsPerHit[iSt]);
    RegisterHist(&fHistHitsPerPoint[iSt]);

    RegisterHist(&fHistEfficiencyXY[iSt]);
    RegisterHist(&fHistEfficiencyR[iSt]);
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmMatch CbmTrackingInputQaSts::MatchHits(const CbmStsHit* pHit, int iHit)
{
  CbmMatch res;  // Matching result

  // Front and back cluster indexes
  const int iClusterF = pHit->GetFrontClusterId();
  if (iClusterF < 0) {
    LOG(error) << "STD: hit (id = " << iHit << ") has incorrect front cluster index: " << iClusterF;
    throw std::runtime_error("STS tracking input QA: wrong front cluster index");
  }

  const int iClusterB = pHit->GetBackClusterId();
  if (iClusterB < 0) {
    LOG(error) << "STD: hit (id = " << iHit << ") has incorrect back cluster index: " << iClusterF;
    throw std::runtime_error("STS tracking input QA: wrong back cluster index");
  }

  // Front and back clusters of a hit
  const auto* pClusterF = dynamic_cast<const CbmStsCluster*>(fpClusters->At(iClusterF));
  if (!pClusterF) {
    LOG(error) << "STD: front cluster does not exist for hit (id = " << iHit << ')';
    throw std::runtime_error("STS tracking input QA: front cluster not found");
  }

  const auto* pClusterB = dynamic_cast<const CbmStsCluster*>(fpClusters->At(iClusterB));
  if (!pClusterB) {
    LOG(error) << "STD: back cluster does not exist for hit (id = " << iHit << ')';
    throw std::runtime_error("STS tracking input QA: back cluster not found");
  }

  const CbmMatch* pClusterMatchF = dynamic_cast<const CbmMatch*>(fpClusterMatches->At(iClusterF));
  const CbmMatch* pClusterMatchB = dynamic_cast<const CbmMatch*>(fpClusterMatches->At(iClusterB));

  // Check addresses
  const int addressHit      = pHit->GetAddress();
  const int addressClusterF = pClusterF->GetAddress();
  const int addressClusterB = pClusterB->GetAddress();
  if (addressHit != addressClusterF || addressHit != addressClusterB) {
    LOG(error) << "STS hit (id = " << iHit << ") and its front and(or) back clusters has different addresses: "
               << "hit address = " << addressHit << ", front cluster address = " << addressClusterF
               << ", back cluster address = " << addressClusterB;
    throw std::runtime_error("STS tracking input QA: inconsistent in hit and clusters addresses");
  }

  // CUSTOM MATCHING: we choose only those links, which are presented both in front and back clusters
  // The same matching is used in L1 tracking
  for (int iLinkF = 0; iLinkF < pClusterMatchF->GetNofLinks(); ++iLinkF) {
    const auto& linkF = pClusterMatchF->GetLink(iLinkF);
    for (int iLinkB = 0; iLinkB < pClusterMatchB->GetNofLinks(); ++iLinkB) {
      const auto& linkB = pClusterMatchB->GetLink(iLinkB);
      if (linkF == linkB) {
        res.AddLink(linkF);
        res.AddLink(linkB);
      }
    }
  }

  return res;  // Rely on NRVO
}

// ---------------------------------------------------------------------------------------------------------------------
//
double CbmTrackingInputQaSts::ParticleMass(int pdg)
{
  // FIXME: SZh: Use masses defined by CbmMCTrack class instead
  if (fabs(pdg) < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
    return TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
  }

  LOG(warn) << "\033[1;31m Found mass for pdg = " << pdg << " is undefined. "
            << "Please, provide data for this particle\033[0m\n";

  return 0.;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmTrackingInputQaSts::ReadAndCreateDataBranches()
{
  auto* pManager = FairRootManager::Instance();
  if (!pManager) { LOG(fatal) << "FairRootManager was not found"; }

  fpDigiManager = CbmDigiManager::Instance();
  fpDigiManager->Init();  // NOTE: is initialized only once

  // ************************************************
  // ** Reconstructed data branches initialization **
  // ************************************************

  fpTimeSlice = dynamic_cast<CbmTimeSlice*>(pManager->GetObject("TimeSlice."));
  if (!fpTimeSlice) {
    LOG(error) << "Time slice was not found";
    return kERROR;
  }

  fpHits = dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsHit"));
  ;
  if (!fpHits) {
    LOG(error) << "Hits input array was not found for STS";
    return kERROR;
  }

  fpClusters = dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsCluster"));
  if (!fpClusters) {
    LOG(error) << "Cluster input array was not found for STS";
    return kERROR;
  }

  // MC data initialization
  fpMcManager  = dynamic_cast<CbmMCDataManager*>(pManager->GetObject("MCDataManager"));
  fIsMcPresent = bool(fpMcManager);

  if (fIsMcPresent) {
    fpMcEventList    = dynamic_cast<CbmMCEventList*>(pManager->GetObject("MCEventList."));
    fpMcTracks       = fpMcManager->InitBranch("MCTrack");
    fpMcPoints       = fpMcManager->InitBranch("StsPoint");
    fpHitMatches     = dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsHitMatch"));
    fpClusterMatches = dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("StsClusterMatch"));

    if (!fpMcEventList) {
      LOG(error) << "MCEventList data were not found";
      return kERROR;
    }

    if (!fpMcTracks) {
      LOG(error) << "MC tracks data were not found";
      return kERROR;
    }

    if (!fpMcPoints) {
      LOG(error) << "MC points data were not found for STS";
      return kERROR;
    }

    if (!fpHitMatches) {
      LOG(error) << "Hits match data were not found for STS";
      return kERROR;
    }
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaSts::ResolutionQa()
{
  // Resolution QA is impossible without MC information
  if (!fIsMcPresent) { return; }

  const int nHits     = fpHits->GetEntriesFast();                     // Number of hits in event/ts
  const int nStations = fpDetectorInterface->GetNtrackingStations();  // Number of stations in event/ts
  const int nMcEvents = fpMcEventList->GetNofEvents();                // Number of MC events in event/ts

  std::vector<std::vector<int>> nHitsPerMcPoint;  // Number of hits per one MC point in different MC events
  nHitsPerMcPoint.resize(nMcEvents);

  // ** Loop over MC events within event/ts **
  for (int iE = 0; iE < nMcEvents; ++iE) {
    const int fileId  = fpMcEventList->GetFileIdByIndex(iE);
    const int eventId = fpMcEventList->GetEventIdByIndex(iE);
    const int nPoints = fpMcPoints->Size(fileId, eventId);
    nHitsPerMcPoint[iE].resize(nPoints, 0);
  }  // Loop over MC events within event/ts: end


  // ** Loop over hits within event/ts **
  for (int iHit = 0; iHit < nHits; ++iHit) {
    const auto* pHit = dynamic_cast<const CbmStsHit*>(fpHits->At(iHit));
    if (!pHit) {
      LOG(error) << "STS hit with index " << iHit << " does not exist in \"StsHit\" array";
      throw std::runtime_error("STS tracking input QA: hit does not exist");
    }

    // Check station index of a hit
    const int iStation = fpDetectorInterface->GetTrackingStationIndex(pHit);
    if (iStation < 0 || iStation >= nStations) {
      LOG(error) << "STS hit with index " << iHit << ": tracking station index = " << iStation
                 << " is out of range [0, " << nStations << ']';
      throw std::runtime_error("STS tracking input QA: hit has inconsistent station index");
    }

    // Get custom match for the hit
    const CbmMatch hitMatch = MatchHits(pHit, iHit);  // throws std::runtime_error

    // Fill number of hits per one MC point vector and update the number of hits per a given MC point
    int nMcPoints = 0;  // Number of non-noisy MC points per event
    for (int iLink = 0; iLink < hitMatch.GetNofLinks(); ++iLink) {
      const auto& link = hitMatch.GetLink(iLink);
      if (link.GetIndex() >= 0) {  // Select only non-noisy links (non-noisy digis)
        ++nMcPoints;
        const int iE = fpMcEventList->GetEventIndex(link);
        if (iE < 0 || iE >= nMcEvents) {
          LOG(error) << "STS: link points to a non-existing MC event (iHit = " << iHit << ')';
          throw std::runtime_error("STS tracking input QA: link points to non-existing MC event");
        }
        if (link.GetIndex() >= int(nHitsPerMcPoint[iE].size())) {
          LOG(error) << "STS: link points to a non-existing MC point (iHit = " << iHit << ')';
          throw std::runtime_error("STS tracking input QA: link points to non-existing MC point");
        }
        nHitsPerMcPoint[iE][link.GetIndex()]++;
      }
    }

    fHistPointsPerHit[iStation].Fill(nMcPoints);

    // Select only hits with one MC point
    if (nMcPoints != 1) { continue; }

    // Take the best link in the match
    const auto& bestLink = hitMatch.GetMatchedLink();

    // Skip noise
    if (bestLink.GetIndex() < 0) { continue; }

    // Get MC point
    const auto* pPoint = dynamic_cast<const CbmStsPoint*>(fpMcPoints->Get(bestLink));
    if (!pPoint) {
      LOG(error) << "STS: MC point does not exist (iHit = " << iHit << ')';
      throw std::runtime_error("STS tracking input QA: MC point does not exist");  // NOTE:
    }

    // TODO: Study the distributions of |pointZIn - stationZ|, |pointZout - stationZ| and |hitZ - stationZ|
    //const double stationZ = fpDetectorInterface->GetZ(iStation);

    // ***********************************
    // ** Calculate residuals and pools **
    // ***********************************

    // Get MC event time
    double t0 = fpMcEventList->GetEventTime(bestLink);
    if (t0 < 0) {
      LOG(error) << "STS: MC event time is undefined (iHit = " << iHit << ')';
      throw std::runtime_error("STS tracking input QA: the MC event time is undefined");
    }

    // Get time, space position and momenta components for the MC point (values are taken for the "In" point)
    double mcX  = pPoint->GetXIn();        // [cm]
    double mcY  = pPoint->GetYIn();        // [cm]
    double mcZ  = pPoint->GetZIn();        // [cm]
    double mcT  = pPoint->GetTime() + t0;  // [ns]
    double mcPx = pPoint->GetPx();         // [GeV/c]
    double mcPy = pPoint->GetPy();         // [GeV/c]
    double mcPz = pPoint->GetPz();         // [GeV/c]

    // Skip slow particles (TODO: why pZ, not p?
    if (fabs(pPoint->GetPzOut()) < fMinMomentum) { continue; }

    // Difference between z components of MC in point and the hit
    double dz = pHit->GetZ() - mcZ;  // [cm]

    // Propagate MC-point x and y "In" coordinates to z-plane of the hit
    mcX += dz * mcPx / mcPz;
    mcY += dz * mcPy / mcPz;

    // Propagete MC-point "In" time to z-plane of the hit
    int pdgCode = pPoint->GetPid();
    double mass = ParticleMass(pdgCode);
    constexpr double speedOfLight {29.9792458};  // cm/ns
    TVector3 mom;
    double mom2 = mcPx * mcPx + mcPy * mcPy + mcPz * mcPz;

    mcT += dz / (mcPz * speedOfLight) * sqrt(mass * mass + mom2);

    double dx   = pHit->GetX() - mcX;
    double dy   = pHit->GetY() - mcY;
    double dt   = pHit->GetTime() - mcT;
    double rmsX = pHit->GetDx();
    double rmsY = pHit->GetDy();
    double rmsT = pHit->GetTimeError();

    fHistResidualX[iStation].Fill(dx);
    fHistResidualY[iStation].Fill(dy);
    fHistResidualT[iStation].Fill(dt);

    fHistPullX[iStation].Fill(dx / rmsX);
    fHistPullY[iStation].Fill(dy / rmsY);
    fHistPullT[iStation].Fill(dt / rmsT);
  }  // Loop over hits within event/ts: end

  // *********************
  // ** Fill efficiency **
  // *********************

  // ** Loop over MC events within event/ts **
  for (int iE = 0; iE < nMcEvents; ++iE) {
    const int fileId  = fpMcEventList->GetFileIdByIndex(iE);
    const int eventId = fpMcEventList->GetEventIdByIndex(iE);
    const int nPoints = fpMcPoints->Size(fileId, eventId);

    // ** Loop over MC points **
    for (int iP = 0; iP < nPoints; ++iP) {
      const auto* pPoint = dynamic_cast<CbmStsPoint*>(fpMcPoints->Get(fileId, eventId, iP));
      if (!pPoint) {
        LOG(error) << "MC point does not exist for iE = " << iE << ", iP = " << iP;
        throw std::runtime_error("STS tracking input QA: MC point does not exist");
      }

      int address  = pPoint->GetDetectorID();
      int iStation = fpDetectorInterface->GetTrackingStationIndex(address);
      if (iStation < 0 || iStation >= nStations) {
        LOG(error) << "STS MC point with index " << iP << ": tracking station index = " << iStation
                   << " is out of range [0, " << nStations << ']';
        throw std::runtime_error("STS tracking input QA: MC point has inconsistent station index");
      }
      fHistHitsPerPoint[iStation].Fill(nHitsPerMcPoint[iE][iP]);

      double pointDistance = sqrt(pPoint->GetXIn() * pPoint->GetXIn() + pPoint->GetYIn() * pPoint->GetYIn());  // [cm]
      fHistEfficiencyR[iStation].Fill(pointDistance, (nHitsPerMcPoint[iE][iP] > 0));
      fHistEfficiencyXY[iStation].Fill(pPoint->GetXIn(), pPoint->GetYIn(), (nHitsPerMcPoint[iE][iP] > 0));
    }

  }  // Loop over MC events within event/ts: end
}


// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTrackingInputQaSts::SetParContainers() {}
