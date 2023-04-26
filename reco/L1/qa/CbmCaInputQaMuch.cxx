/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaInputQaMuch.cxx
/// \date   13.01.2023
/// \brief  QA-task for CA tracking input from MuCh detector (implementation)
/// \author S.Zharko <s.zharko@gsi.de>

#include "CbmCaInputQaMuch.h"

#include "CbmAddress.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchTrackingInterface.h"
#include "CbmQaCanvas.h"
#include "CbmQaEff.h"
#include "CbmTimeSlice.h"

#include "FairRootManager.h"
#include "Logger.h"

#include "TBox.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TFormula.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TStyle.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <numeric>

#include "L1Constants.h"

ClassImp(CbmCaInputQaMuch);

namespace phys = L1Constants::phys;  // from L1Constants.h

// ---------------------------------------------------------------------------------------------------------------------
//
CbmCaInputQaMuch::CbmCaInputQaMuch(int verbose, bool isMCUsed)
  : CbmQaTask("CbmCaInputQaMuch", "camuch", verbose, isMCUsed)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmCaInputQaMuch::Check()
{
  bool res = true;

  int nSt = fpDetInterface->GetNtrackingStations();


  // **************************************************************
  // ** Basic checks, available both for real and simulated data **
  // **************************************************************

  // ----- Checks for mismatches in the ordering of the stations
  //
  std::vector<double> vStationPos(nSt, 0.);
  for (int iSt = 0; iSt < nSt; ++iSt) {
    vStationPos[iSt] = fpDetInterface->GetZ(iSt);
  }

  if (!std::is_sorted(vStationPos.cbegin(), vStationPos.cend(), [](int l, int r) { return l <= r; })) {
    if (fVerbose > 0) {
      LOG(error) << fName << ": stations are ordered improperly along the beam axis:";
      for (auto zPos : vStationPos) {
        LOG(error) << "\t- " << zPos;
      }
    }
    res = false;
  }

  // ----- Checks for mismatch between station and hit z positions
  //   The purpose of this block is to be ensured, that hits belong to the correct tracking station. For each tracking
  // station a unified position z-coordinate is defined, which generally differs from the corresponding positions of
  // reconstructed hits. This is due to non-regular position along the beam axis for each detector sensor. Thus, the
  // positions of the hits along z-axis are distributed relatively to the position of the station in some range.
  // If hits goes out this range, it can signal about a mismatch between hits and geometry. For limits of the range
  // one should select large enough values to cover the difference described above and in the same time small enough
  // to avoid overlaps with the neighboring stations.
  //   For each station, a distribution of z_{hit} - z_{st} is integrated over the defined range and scaled by the
  // total number of entries to the distribution. If this value is smaller then unity, then some of the hits belong to
  // another station.
  //
  for (int iSt = 0; iSt < nSt; ++iSt) {
    int nHits = fvph_hit_station_delta_z[iSt]->GetEntries();
    if (!nHits) {
      LOG_IF(error, fVerbose > 0) << fName << ": station " << iSt << " does not have hits";
      res = false;
      continue;
    }
    int iBinMin = fvph_hit_station_delta_z[iSt]->FindBin(-fMaxDiffZStHit);
    int iBinMax = fvph_hit_station_delta_z[iSt]->FindBin(+fMaxDiffZStHit);

    if (fvph_hit_station_delta_z[iSt]->Integral(iBinMin, iBinMax) < nHits) {
      LOG_IF(error, fVerbose > 0) << fName << ": station " << iSt << " has mismatches in hit z-positions";
      res = false;
    }
  }


  // *******************************************************
  // ** Additional checks, if MC information is available **
  // *******************************************************

  if (IsMCUsed()) {
    // ----- Check efficiencies
    // Error is raised, if any station has integrated efficiency lower then a selected threshold.
    // Integrated efficiency is estimated with fitting the efficiency curve for hit distances (r) with a uniform
    // distribution in the range from kEffRangeMin to kEffRangeMax, where the efficiency is assigned to be constant
    //
    // Fit efficiency curves
    LOG(info) << "-- Hit efficiency integrated over hit distance from station center";
    LOG(info) << "\tintegration range: [" << fEffRange[0] << ", " << fEffRange[1] << "] cm";

    auto* pEffTable = MakeTable("eff_table", "Efficiency table", nSt, 2);
    pEffTable->SetNamesOfCols({"Station ID", "Efficiency"});
    pEffTable->SetColWidth(20);

    for (int iSt = 0; iSt < nSt; ++iSt) {
      auto [eff, effEL, effEU] = fvpe_reco_eff_vs_r[iSt]->GetTotalEfficiency();
      pEffTable->SetCell(iSt, 0, iSt);
      pEffTable->SetCell(iSt, 1, eff);
      res = CheckRange("Hit finder efficiency in station " + std::to_string(iSt), eff, fEffThrsh, 1.000);
    }

    LOG(info) << '\n' << pEffTable->ToString(3);

    // ----- Checks for residuals
    // Check hits for potential biases from central values

    // Function to fit a residual distribution, returns a structure
    auto FitResidualsAndGetMean = [&](TH1* pHist) {
      auto fit        = TF1("fitres", "gausn");
      double statMean = pHist->GetMean();
      double statSigm = pHist->GetStdDev();
      fit.SetParameters(100., statMean, statSigm);
      pHist->Fit("fitres", "MQ");
      pHist->Fit("fitres", "MQ");
      pHist->Fit("fitres", "MQ");
      // NOTE: Several fit procedures are made to avoid empty fit results
      std::array<double, 3> result;
      result[0] = fit.GetParameter(1);
      result[1] = -fit.GetParameter(2) * fResMeanThrsh;
      result[2] = +fit.GetParameter(2) * fResMeanThrsh;
      return result;
    };

    auto* pResidualsTable = MakeTable("residuals_mean", "Residual mean values in different stations", nSt, 4);
    pResidualsTable->SetNamesOfCols({"Station ID", "Residual(x) [cm]", "Residual(y) [cm]", "Residual(t) [ns]"});
    pResidualsTable->SetColWidth(20);

    // Fill residuals fit results and check boundaries
    for (int iSt = 0; iSt < nSt; ++iSt) {
      auto fitX = FitResidualsAndGetMean(fvph_res_x[iSt]);
      auto fitY = FitResidualsAndGetMean(fvph_res_y[iSt]);
      auto fitT = FitResidualsAndGetMean(fvph_res_t[iSt]);
      res = CheckRange("Mean of x residual [cm] in station " + std::to_string(iSt), fitX[0], fitX[1], fitX[2]) && res;
      res = CheckRange("Mean of y residual [cm] in station " + std::to_string(iSt), fitY[0], fitY[1], fitY[2]) && res;
      res = CheckRange("Mean of t residual [ns] in station " + std::to_string(iSt), fitT[0], fitT[1], fitT[2]) && res;
      pResidualsTable->SetCell(iSt, 0, iSt);
      pResidualsTable->SetCell(iSt, 1, fitX[0]);
      pResidualsTable->SetCell(iSt, 2, fitX[1]);
      pResidualsTable->SetCell(iSt, 3, fitX[2]);
    }

    LOG(info) << '\n' << pResidualsTable->ToString(8);

    // ----- Checks for pulls
    // For the hit pull is defined as a ration of the difference between hit and MC-point position or time component
    // values and an error of the hit position (or time) component, calculated in the same z-positions. In ideal case,
    // when the resolutions are well determined for detecting stations, the pull distributions should have a RMS equal
    // to unity. Here we allow a RMS of the pull distributions to be varied in a predefined range. If the RMS runs out
    // this range, QA task fails.
    // TODO: Add checks for center

    // Fit pull distributions

    // ** Kaniadakis Gaussian distribution, gives smaller chi2 / NDF
    //if (!gROOT->FindObject("Expk")) {
    //  new TFormula("Expk", "TMath::Power(TMath::Sqrt(1 + x[1] * x[1] * x[0] * x[0]) + x[0] * x[1], 1./x[1])");
    //}
    //TF1* pPullFit = new TF1("pullFitGausn", "[0] * Expk(-[2] * (x - [1]) * (x - [1]), [3])", -10., 10.);

    auto FitPullsAndGetSigma = [&](TH1* pHist) {
      auto fit = TF1("fitpull", "gausn(0)");
      fit.SetParameters(100, 0.001, 1.000);
      pHist->Fit("fitpull", "Q");
      return fit.GetParameter(2);
    };

    auto* pPullsTable = MakeTable("pulls_rms", "Pulls std. dev. values in different stations", nSt, 4);
    pPullsTable->SetNamesOfCols({"Station ID", "Pull(x) sigm", "Pull(y) sigm", "Pull(z) sigm"});
    pPullsTable->SetColWidth(20);

    for (int iSt = 0; iSt < nSt; ++iSt) {
      double rmsPullX = FitPullsAndGetSigma(fvph_pull_x[iSt]);
      double rmsPullY = FitPullsAndGetSigma(fvph_pull_y[iSt]);
      double rmsPullT = FitPullsAndGetSigma(fvph_pull_t[iSt]);

      double rmsPullHi = 1. + fPullWidthThrsh;
      double rmsPullLo = 1. - fPullWidthThrsh;

      res = CheckRange("Rms for x pull in station " + std::to_string(iSt), rmsPullX, rmsPullLo, rmsPullHi) && res;
      res = CheckRange("Rms for y pull in station " + std::to_string(iSt), rmsPullY, rmsPullLo, rmsPullHi) && res;
      res = CheckRange("Rms for t pull in station " + std::to_string(iSt), rmsPullT, rmsPullLo, rmsPullHi) && res;
      pPullsTable->SetCell(iSt, 0, iSt);
      pPullsTable->SetCell(iSt, 1, rmsPullX);
      pPullsTable->SetCell(iSt, 2, rmsPullY);
      pPullsTable->SetCell(iSt, 3, rmsPullT);
    }

    LOG(info) << '\n' << pPullsTable->ToString(3);
  }

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaInputQaMuch::DeInit()
{
  // Vectors with pointers to histograms
  fvph_hit_ypos_vs_xpos.clear();
  fvph_hit_xpos_vs_zpos.clear();
  fvph_hit_ypos_vs_zpos.clear();

  fvph_hit_station_delta_z.clear();

  fvph_hit_dx.clear();
  fvph_hit_dy.clear();
  fvph_hit_dt.clear();

  fvph_n_points_per_hit.clear();

  fvph_point_ypos_vs_xpos.clear();
  fvph_point_xpos_vs_zpos.clear();
  fvph_point_ypos_vs_zpos.clear();

  fvph_point_hit_delta_z.clear();

  fvph_res_x.clear();
  fvph_res_y.clear();
  fvph_res_t.clear();

  fvph_pull_x.clear();
  fvph_pull_y.clear();
  fvph_pull_t.clear();

  fvph_res_x_vs_x.clear();
  fvph_res_y_vs_y.clear();
  fvph_res_t_vs_t.clear();

  fvph_pull_x_vs_x.clear();
  fvph_pull_y_vs_y.clear();
  fvph_pull_t_vs_t.clear();

  fvpe_reco_eff_vs_xy.clear();
  fvpe_reco_eff_vs_r.clear();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaInputQaMuch::FillHistograms()
{
  int nSt       = fpDetInterface->GetNtrackingStations();
  int nHits     = fpHits->GetEntriesFast();
  int nMCevents = (IsMCUsed()) ? fpMCEventList->GetNofEvents() : -1;

  std::vector<std::vector<int>> vNofHitsPerPoint;  // Number of hits per MC point in different MC events

  if (IsMCUsed()) {
    vNofHitsPerPoint.resize(nMCevents);
    for (int iE = 0; iE < nMCevents; ++iE) {
      int iFile   = fpMCEventList->GetFileIdByIndex(iE);
      int iEvent  = fpMCEventList->GetEventIdByIndex(iE);
      int nPoints = fpMCPoints->Size(iFile, iEvent);
      vNofHitsPerPoint[iE].resize(nPoints, 0);
    }
  }

  for (int iH = 0; iH < nHits; ++iH) {
    const auto* pHit = dynamic_cast<const CbmMuchPixelHit*>(fpHits->At(iH));
    LOG_IF(fatal, !pHit) << fName << ": hit with iH = " << iH << " is not an CbmMuchPixelHit (dynamic cast failed)";

    // *************************
    // ** Reconstructed hit QA

    // Hit station geometry info
    int iSt = fpDetInterface->GetTrackingStationIndex(pHit);
    LOG_IF(fatal, iSt < 0 || iSt >= nSt) << fName << ": index of station (" << iSt << ") is out of range "
                                         << "for hit with id = " << iH;

    // Hit position
    double xHit = pHit->GetX();
    double yHit = pHit->GetY();
    double zHit = pHit->GetZ();

    double dxHit = pHit->GetDx();
    double dyHit = pHit->GetDy();
    //double dxyHit = pHit->GetDxy();

    // Hit time
    double tHit  = pHit->GetTime();
    double dtHit = pHit->GetTimeError();

    fvph_hit_ypos_vs_xpos[iSt]->Fill(xHit, yHit);
    fvph_hit_xpos_vs_zpos[iSt]->Fill(zHit, xHit);
    fvph_hit_ypos_vs_zpos[iSt]->Fill(zHit, yHit);

    fvph_hit_station_delta_z[iSt]->Fill(zHit - fpDetInterface->GetZ(iSt));

    fvph_hit_dx[iSt]->Fill(dxHit);
    fvph_hit_dy[iSt]->Fill(dyHit);
    fvph_hit_dt[iSt]->Fill(dtHit);

    fvph_hit_ypos_vs_xpos[nSt]->Fill(xHit, yHit);
    fvph_hit_xpos_vs_zpos[nSt]->Fill(zHit, xHit);
    fvph_hit_ypos_vs_zpos[nSt]->Fill(zHit, yHit);
    fvph_hit_dx[nSt]->Fill(dxHit);
    fvph_hit_dy[nSt]->Fill(dyHit);
    fvph_hit_dt[nSt]->Fill(dtHit);


    // **********************
    // ** MC information QA

    if (IsMCUsed()) {
      CbmMatch* pHitMatch = dynamic_cast<CbmMatch*>(fpHitMatches->At(iH));
      LOG_IF(fatal, !pHitMatch) << fName << ": match object not found for hit ID " << iH;

      // Evaluate number of hits per one MC point
      int nMCpoints = 0;  // Number of MC points for this hit
      for (int iLink = 0; iLink < pHitMatch->GetNofLinks(); ++iLink) {
        const auto& link = pHitMatch->GetLink(iLink);

        int iP = link.GetIndex();  // Index of MC point

        // Skip noisy links
        if (iP < 0) { continue; }

        ++nMCpoints;

        int iE = fpMCEventList->GetEventIndex(link);

        LOG_IF(fatal, iE < 0 || iE >= nMCevents) << fName << ": id of MC event is out of range (hit id = " << iH
                                                 << ", link id = " << iLink << ", event id = " << iE << ')';

        LOG_IF(fatal, iP >= (int) vNofHitsPerPoint[iE].size())
          << fName << ": index of MC point is out of range (hit id = " << iH << ", link id = " << iLink
          << ", event id = " << iE << ", point id = " << iP << ')';
        vNofHitsPerPoint[iE][iP]++;
      }

      fvph_n_points_per_hit[iSt]->Fill(nMCpoints);
      fvph_n_points_per_hit[nSt]->Fill(nMCpoints);

      if (nMCpoints != 1) { continue; }  // ??

      // The best link to in the match (probably, the cut on nMCpoints is meaningless)
      const auto& bestPointLink = pHitMatch->GetMatchedLink();

      // Skip noisy links
      if (bestPointLink.GetIndex() < 0) { continue; }

      // Point matched by the best link
      const auto* pMCPoint = dynamic_cast<const CbmMuchPoint*>(fpMCPoints->Get(bestPointLink));
      LOG_IF(fatal, !pMCPoint) << fName << ": MC point object does not exist for hit " << iH;

      // MC track for this point
      CbmLink bestTrackLink = bestPointLink;
      bestTrackLink.SetIndex(pMCPoint->GetTrackID());
      const auto* pMCTrack = dynamic_cast<const CbmMCTrack*>(fpMCTracks->Get(bestTrackLink));
      LOG_IF(fatal, !pMCTrack) << fName << ": MC track object does not exist for hit " << iH << " and link: ";

      double t0MC = fpMCEventList->GetEventTime(bestPointLink);
      LOG_IF(fatal, t0MC < 0) << fName << ": MC time zero is lower then 0 ns: " << t0MC;


      // ----- MC point properties
      //
      double mass = pMCTrack->GetMass();
      //double charge = pMCTrack->GetCharge();
      //double pdg    = pMCTrack->GetPdgCode();

      // Entrance position and time
      double xMC = pMCPoint->GetXIn();
      double yMC = pMCPoint->GetYIn();
      double zMC = pMCPoint->GetZIn();
      double tMC = pMCPoint->GetTime() + t0MC;

      // MC point entrance momenta
      double pxMC = pMCPoint->GetPx();
      double pyMC = pMCPoint->GetPy();
      double pzMC = pMCPoint->GetPz();
      double pMC  = sqrt(pxMC * pxMC + pyMC * pyMC + pzMC * pzMC);

      // MC point exit momenta
      // double pxMCo = pMCPoint->GetPxOut();
      // double pyMCo = pMCPoint->GetPyOut();
      // double pzMCo = pMCPoint->GetPzOut();
      // double pMCo  = sqrt(pxMCo * pxMCo + pyMCo * pyMCo + pzMCo * pzMCo);

      // Position and time shifted to z-coordinate of the hit
      double shiftZ = zHit - zMC;  // Difference between hit and point z positions
      double xMCs   = xMC + shiftZ * pxMC / pzMC;
      double yMCs   = yMC + shiftZ * pyMC / pzMC;
      double tMCs   = tMC + shiftZ / (pzMC * phys::kSpeedOfLight) * sqrt(mass * mass + pMC * pMC);

      // Residuals
      double xRes = xHit - xMCs;
      double yRes = yHit - yMCs;
      double tRes = tHit - tMCs;

      // ------ Cuts

      if (std::fabs(pMCPoint->GetPzOut()) < fMinMomentum) { continue; }  // CUT ON MINIMUM MOMENTUM
      //if (pMCo < cuts::kMinP) { continue; }  // Momentum threshold


      fvph_point_ypos_vs_xpos[iSt]->Fill(xMC, yMC);
      fvph_point_xpos_vs_zpos[iSt]->Fill(zMC, xMC);
      fvph_point_ypos_vs_zpos[iSt]->Fill(zMC, yMC);

      fvph_point_hit_delta_z[iSt]->Fill(shiftZ);

      fvph_res_x[iSt]->Fill(xRes);
      fvph_res_y[iSt]->Fill(yRes);
      fvph_res_t[iSt]->Fill(tRes);

      fvph_pull_x[iSt]->Fill(xRes / dxHit);
      fvph_pull_y[iSt]->Fill(yRes / dyHit);
      fvph_pull_t[iSt]->Fill(tRes / dtHit);

      fvph_res_x_vs_x[iSt]->Fill(xHit, xRes);
      fvph_res_y_vs_y[iSt]->Fill(yHit, yRes);
      fvph_res_t_vs_t[iSt]->Fill(tHit, tRes);

      fvph_pull_x_vs_x[iSt]->Fill(xHit, xRes / dxHit);
      fvph_pull_y_vs_y[iSt]->Fill(yHit, yRes / dyHit);
      fvph_pull_t_vs_t[iSt]->Fill(tHit, tRes / dtHit);

      fvph_point_ypos_vs_xpos[nSt]->Fill(xMC, yMC);
      fvph_point_xpos_vs_zpos[nSt]->Fill(zMC, xMC);
      fvph_point_ypos_vs_zpos[nSt]->Fill(zMC, yMC);

      fvph_point_hit_delta_z[nSt]->Fill(shiftZ);

      fvph_res_x[nSt]->Fill(xRes);
      fvph_res_y[nSt]->Fill(yRes);
      fvph_res_t[nSt]->Fill(tRes);

      fvph_pull_x[nSt]->Fill(xRes / dxHit);
      fvph_pull_y[nSt]->Fill(yRes / dyHit);
      fvph_pull_t[nSt]->Fill(tRes / dtHit);

      fvph_res_x_vs_x[nSt]->Fill(xHit, xRes);
      fvph_res_y_vs_y[nSt]->Fill(yHit, yRes);
      fvph_res_t_vs_t[nSt]->Fill(tHit, tRes);

      fvph_pull_x_vs_x[nSt]->Fill(xHit, xRes / dxHit);
      fvph_pull_y_vs_y[nSt]->Fill(yHit, yRes / dyHit);
      fvph_pull_t_vs_t[nSt]->Fill(tHit, tRes / dtHit);
    }
  }  // loop over hits: end

  // Fill hit reconstruction efficiencies
  if (IsMCUsed()) {
    for (int iE = 0; iE < nMCevents; ++iE) {
      int iFile   = fpMCEventList->GetFileIdByIndex(iE);
      int iEvent  = fpMCEventList->GetEventIdByIndex(iE);
      int nPoints = fpMCPoints->Size(iFile, iEvent);

      for (int iP = 0; iP < nPoints; ++iP) {
        const auto* pMCPoint = dynamic_cast<const CbmMuchPoint*>(fpMCPoints->Get(iFile, iEvent, iP));
        LOG_IF(fatal, !pMCPoint) << fName << ": MC point does not exist for iFile = " << iFile
                                 << ", iEvent = " << iEvent << ", iP = " << iP;
        int address = pMCPoint->GetDetectorID();
        int iSt     = fpDetInterface->GetTrackingStationIndex(address);
        LOG_IF(fatal, iSt < 0 || iSt >= nSt)
          << fName << ": MC point for FEI = " << iFile << ", " << iEvent << ", " << iP << " and address " << address
          << " has wrong station index: iSt = " << iSt;

        double xMC = pMCPoint->GetXIn();
        double yMC = pMCPoint->GetYIn();
        double rMC = sqrt(xMC * xMC + yMC * yMC);

        // Conditions under which point is accounted as reconstructed: point
        bool ifPointHasHits = (vNofHitsPerPoint[iE][iP] > 0);

        fvpe_reco_eff_vs_xy[iSt]->Fill(ifPointHasHits, xMC, yMC);
        fvpe_reco_eff_vs_xy[nSt]->Fill(ifPointHasHits, xMC, yMC);

        fvpe_reco_eff_vs_r[iSt]->Fill(ifPointHasHits, rMC);
        fvpe_reco_eff_vs_r[nSt]->Fill(ifPointHasHits, rMC);

      }  // loop over MC-points: end
    }    // loop over MC-events: end
  }      // MC is used: end
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaMuch::InitDataBranches()
{
  // STS tracking detector interface
  fpDetInterface = CbmMuchTrackingInterface::Instance();

  LOG_IF(fatal, !fpDetInterface) << "\033[1;31m" << fName << ": tracking detector interface is undefined\033[0m";

  // FairRootManager
  auto* pFairRootManager = FairRootManager::Instance();
  LOG_IF(fatal, !pFairRootManager) << "\033[1;31m" << fName << ": FairRootManager instance is a null pointer\033[0m";

  // Time-slice
  fpTimeSlice = dynamic_cast<CbmTimeSlice*>(pFairRootManager->GetObject("TimeSlice."));
  LOG_IF(fatal, !fpTimeSlice) << "\033[1;31m" << fName << ": time-slice branch is not found\033[0m";

  // ----- Reconstructed data-branches initialization

  // Hits container
  fpHits = dynamic_cast<TClonesArray*>(pFairRootManager->GetObject("MuchPixelHit"));
  LOG_IF(fatal, !fpHits) << "\033[1;31m" << fName << ": container of reconstructed hits in MuCh is not found\033[0m";

  // ----- MC-information branches initialization
  if (IsMCUsed()) {
    // MC manager
    fpMCDataManager = dynamic_cast<CbmMCDataManager*>(pFairRootManager->GetObject("MCDataManager"));
    LOG_IF(fatal, !fpMCDataManager) << "\033[1;31m" << fName << ": MC data manager branch is not found\033[0m";

    // MC event list
    fpMCEventList = dynamic_cast<CbmMCEventList*>(pFairRootManager->GetObject("MCEventList."));
    LOG_IF(fatal, !fpMCEventList) << "\033[1;31m" << fName << ": MC event list branch is not found\033[0m";

    // MC tracks
    fpMCTracks = fpMCDataManager->InitBranch("MCTrack");
    LOG_IF(fatal, !fpMCTracks) << "\033[1;31m" << fName << ": MC track branch is not found\033[0m";

    // MC points
    fpMCPoints = fpMCDataManager->InitBranch("MuchPoint");
    LOG_IF(fatal, !fpMCTracks) << "\033[1;31m" << fName << ": MC point branch is not found for MuCh\033[0m";

    // Hit matches
    fpHitMatches = dynamic_cast<TClonesArray*>(pFairRootManager->GetObject("MuchPixelHitMatch"));
    LOG_IF(fatal, !fpHitMatches) << "\033[1;31m]" << fName << ": hit match branch is not found for MuCh\033[0m";
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaMuch::InitCanvases()
{
  gStyle->SetOptFit(1);
  int nSt = fpDetInterface->GetNtrackingStations();

  // ********************
  // ** Drawing options

  // Contours
  constexpr auto contColor = kOrange + 7;
  constexpr auto contWidth = 2;  // Line width
  constexpr auto contStyle = 2;  // Line style
  constexpr auto contFill  = 0;  // Fill style

  // *********************************
  // ** Hit occupancies

  // ** Occupancy in XY plane
  auto* pc_hit_ypos_vs_xpos = MakeCanvas<CbmQaCanvas>("hit_ypos_vs_xpos", "", 1600, 800);
  pc_hit_ypos_vs_xpos->Divide2D(nSt);
  for (int iSt = 0; iSt < nSt; ++iSt) {
    pc_hit_ypos_vs_xpos->cd(iSt + 1);
    fvph_hit_ypos_vs_xpos[iSt]->DrawCopy("colz", "");


    // Reference size of the station saved to the detector interface
    auto* pInnerCircle = new TEllipse(0., 0., fpDetInterface->GetRmin(iSt));
    pInnerCircle->SetLineWidth(contWidth);
    pInnerCircle->SetLineStyle(contStyle);
    pInnerCircle->SetLineColor(contColor);
    pInnerCircle->SetFillStyle(contFill);
    pInnerCircle->Draw("SAME");

    auto* pOuterCircle = new TEllipse(0., 0., fpDetInterface->GetRmax(iSt));
    pOuterCircle->SetLineWidth(contWidth);
    pOuterCircle->SetLineStyle(contStyle);
    pOuterCircle->SetLineColor(contColor);
    pOuterCircle->SetFillStyle(contFill);
    pOuterCircle->Draw("SAME");

    // Square boarders of station, which are used for the field approximation
    double stXmin = +fpDetInterface->GetXmax(iSt);
    double stXmax = -fpDetInterface->GetXmax(iSt);
    double stYmin = -fpDetInterface->GetYmax(iSt);
    double stYmax = +fpDetInterface->GetYmax(iSt);

    auto* pBox = new TBox(stXmin, stYmin, stXmax, stYmax);
    pBox->SetLineWidth(contWidth);
    pBox->SetLineStyle(contStyle);
    pBox->SetLineColor(contColor);
    pBox->SetFillStyle(contFill);
    pBox->Draw("SAME");
  }

  // ----- Occupancy projections
  auto* pc_hit_xpos = MakeCanvas<CbmQaCanvas>("hit_xpos", "", 1400, 800);
  pc_hit_xpos->Divide2D(nSt);
  for (int iSt = 0; iSt < nSt; ++iSt) {
    pc_hit_xpos->cd(iSt + 1);
    auto* phProj = fvph_hit_ypos_vs_xpos[iSt]->ProjectionY();
    phProj->DrawCopy();
  }

  auto* pc_hit_ypos = MakeCanvas<CbmQaCanvas>("hit_ypos", "", 1400, 800);
  pc_hit_ypos->Divide2D(nSt);
  for (int iSt = 0; iSt < nSt; ++iSt) {
    pc_hit_ypos->cd(iSt + 1);
    auto* phProj = fvph_hit_ypos_vs_xpos[iSt]->ProjectionX();
    phProj->DrawCopy();
  }


  // ** Occupancy in XZ plane
  MakeCanvas<CbmQaCanvas>("hit_xpos_vs_zpos", "", 600, 400);
  fvph_hit_xpos_vs_zpos[nSt]->DrawCopy("colz", "");
  for (int iSt = 0; iSt < nSt; ++iSt) {
    // Station positions in detector IFS
    double stZmin = fpDetInterface->GetZ(iSt) - 0.5 * fpDetInterface->GetThickness(iSt);
    double stZmax = fpDetInterface->GetZ(iSt) + 0.5 * fpDetInterface->GetThickness(iSt);
    double stHmin = -fpDetInterface->GetRmax(iSt);
    double stHmax = +fpDetInterface->GetRmax(iSt);

    auto* pBox = new TBox(stZmin, stHmin, stZmax, stHmax);
    pBox->SetLineWidth(contWidth);
    pBox->SetLineStyle(contStyle);
    pBox->SetLineColor(contColor);
    pBox->SetFillStyle(contFill);
    pBox->Draw("SAME");
  }

  // ** Occupancy in YZ plane
  MakeCanvas<CbmQaCanvas>("hit_ypos_vs_zpos", "", 600, 400);
  fvph_hit_ypos_vs_zpos[nSt]->DrawCopy("colz", "");
  for (int iSt = 0; iSt < nSt; ++iSt) {
    // Station positions in detector IFS
    double stZmin = fpDetInterface->GetZ(iSt) - 0.5 * fpDetInterface->GetThickness(iSt);
    double stZmax = fpDetInterface->GetZ(iSt) + 0.5 * fpDetInterface->GetThickness(iSt);
    double stHmin = -fpDetInterface->GetRmax(iSt);
    double stHmax = +fpDetInterface->GetRmax(iSt);

    auto* pBox = new TBox(stZmin, stHmin, stZmax, stHmax);
    pBox->SetLineWidth(contWidth);
    pBox->SetLineStyle(contStyle);
    pBox->SetLineColor(contColor);
    pBox->SetFillStyle(contFill);
    pBox->Draw("SAME");
  }

  // ************
  // ************

  if (IsMCUsed()) {

    // **********************
    // ** Point occupancies

    // ** Occupancy in XY plane
    auto* pc_point_ypos_vs_xpos = MakeCanvas<CbmQaCanvas>("point_ypos_vs_xpos", "", 1600, 800);
    pc_point_ypos_vs_xpos->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_point_ypos_vs_xpos->cd(iSt + 1);
      fvph_point_ypos_vs_xpos[iSt]->DrawCopy("colz", "");

      // Reference size of the station saved to the detector interface
      auto* pInnerCircle = new TEllipse(0., 0., fpDetInterface->GetRmin(iSt));
      pInnerCircle->SetLineWidth(contWidth);
      pInnerCircle->SetLineStyle(contStyle);
      pInnerCircle->SetLineColor(contColor);
      pInnerCircle->SetFillStyle(contFill);
      pInnerCircle->Draw("SAME");

      auto* pOuterCircle = new TEllipse(0., 0., fpDetInterface->GetRmax(iSt));
      pOuterCircle->SetLineWidth(contWidth);
      pOuterCircle->SetLineStyle(contStyle);
      pOuterCircle->SetLineColor(contColor);
      pOuterCircle->SetFillStyle(contFill);
      pOuterCircle->Draw("SAME");

      // Square boarders of station, which are used for the field approximation
      double stXmin = +fpDetInterface->GetXmax(iSt);
      double stXmax = -fpDetInterface->GetXmax(iSt);
      double stYmin = -fpDetInterface->GetYmax(iSt);
      double stYmax = +fpDetInterface->GetYmax(iSt);

      auto* pBox = new TBox(stXmin, stYmin, stXmax, stYmax);
      pBox->SetLineWidth(contWidth);
      pBox->SetLineStyle(contStyle);
      pBox->SetLineColor(contColor);
      pBox->SetFillStyle(contFill);
      pBox->Draw("SAME");
    }

    // ** Occupancy in XZ plane
    MakeCanvas<CbmQaCanvas>("point_xpos_vs_zpos", "", 600, 400);
    fvph_point_xpos_vs_zpos[nSt]->SetStats(false);
    fvph_point_xpos_vs_zpos[nSt]->DrawCopy("colz", "");
    for (int iSt = 0; iSt < nSt; ++iSt) {
      // Station positions in detector IFS
      double stZmin = fpDetInterface->GetZ(iSt) - 0.5 * fpDetInterface->GetThickness(iSt);
      double stZmax = fpDetInterface->GetZ(iSt) + 0.5 * fpDetInterface->GetThickness(iSt);
      double stHmin = -fpDetInterface->GetRmax(iSt);
      double stHmax = +fpDetInterface->GetRmax(iSt);

      auto* pBox = new TBox(stZmin, stHmin, stZmax, stHmax);
      pBox->SetLineWidth(contWidth);
      pBox->SetLineStyle(contStyle);
      pBox->SetLineColor(contColor);
      pBox->SetFillStyle(contFill);
      pBox->Draw("SAME");
    }

    // ** Occupancy in YZ plane
    MakeCanvas<CbmQaCanvas>("point_ypos_vs_zpos", "", 600, 400);
    fvph_point_ypos_vs_zpos[nSt]->SetStats(false);
    fvph_point_ypos_vs_zpos[nSt]->DrawCopy("colz", "");
    for (int iSt = 0; iSt < nSt; ++iSt) {
      // Station positions in detector IFS
      double stZmin = fpDetInterface->GetZ(iSt) - 0.5 * fpDetInterface->GetThickness(iSt);
      double stZmax = fpDetInterface->GetZ(iSt) + 0.5 * fpDetInterface->GetThickness(iSt);
      double stHmin = -fpDetInterface->GetRmax(iSt);
      double stHmax = +fpDetInterface->GetRmax(iSt);

      auto* pBox = new TBox(stZmin, stHmin, stZmax, stHmax);
      pBox->SetLineWidth(contWidth);
      pBox->SetLineStyle(contStyle);
      pBox->SetLineColor(contColor);
      pBox->SetFillStyle(contFill);
      pBox->Draw("SAME");
    }

    // **************
    // ** Residuals

    // x-coordinate
    auto* pc_res_x = MakeCanvas<CbmQaCanvas>("res_x", "Residuals for x coordinate", 1600, 800);
    pc_res_x->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_res_x->cd(iSt + 1);
      fvph_res_x[iSt]->DrawCopy("", "");
    }

    // y-coordinate
    auto* pc_res_y = MakeCanvas<CbmQaCanvas>("res_y", "Residuals for y coordinate", 1600, 800);
    pc_res_y->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_res_y->cd(iSt + 1);
      fvph_res_y[iSt]->DrawCopy("", "");
    }

    // time
    auto* pc_res_t = MakeCanvas<CbmQaCanvas>("res_t", "Residuals for time", 1600, 800);
    pc_res_t->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_res_t->cd(iSt + 1);
      fvph_res_t[iSt]->DrawCopy("", "");
    }


    // **********
    // ** Pulls

    // x-coordinate
    auto* pc_pull_x = MakeCanvas<CbmQaCanvas>("pull_x", "Pulls for x coordinate", 1600, 800);
    pc_pull_x->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_pull_x->cd(iSt + 1);
      fvph_pull_x[iSt]->DrawCopy("", "");
    }

    // y-coordinate
    auto* pc_pull_y = MakeCanvas<CbmQaCanvas>("pull_y", "Pulls for y coordinate", 1600, 800);
    pc_pull_y->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_pull_y->cd(iSt + 1);
      fvph_pull_y[iSt]->DrawCopy("", "");
    }

    // time
    auto* pc_pull_t = MakeCanvas<CbmQaCanvas>("pull_t", "Pulls for time", 1600, 800);
    pc_pull_t->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_pull_t->cd(iSt + 1);
      fvph_pull_t[iSt]->DrawCopy("", "");
    }


    // ************************************
    // ** Hit reconstruction efficiencies

    auto* pc_reco_eff_vs_r =
      MakeCanvas<CbmQaCanvas>("reco_eff_vs_r", "Hit efficiencies wrt distance from center", 1600, 800);
    pc_reco_eff_vs_r->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_reco_eff_vs_r->cd(iSt + 1);
      fvpe_reco_eff_vs_r[iSt]->SetMarkerStyle(20);
      fvpe_reco_eff_vs_r[iSt]->DrawCopy("AP", "");
    }

    auto* pc_reco_eff_vs_xy = MakeCanvas<CbmQaCanvas>("reco_eff_vs_xy", "Hit efficiencies wrt x and y", 1600, 800);
    pc_reco_eff_vs_xy->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_reco_eff_vs_xy->cd(iSt + 1);
      fvpe_reco_eff_vs_xy[iSt]->DrawCopy("colz", "");
    }
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaMuch::InitHistograms()
{
  int nSt = fpDetInterface->GetNtrackingStations();

  // ----- Histograms initialization
  fvph_hit_ypos_vs_xpos.resize(nSt + 1, nullptr);
  fvph_hit_ypos_vs_zpos.resize(nSt + 1, nullptr);
  fvph_hit_xpos_vs_zpos.resize(nSt + 1, nullptr);

  fvph_hit_station_delta_z.resize(nSt);

  fvph_hit_dx.resize(nSt + 1, nullptr);
  fvph_hit_dy.resize(nSt + 1, nullptr);
  fvph_hit_dt.resize(nSt + 1, nullptr);

  for (int iSt = 0; iSt <= nSt; ++iSt) {
    TString nsuff = (iSt == nSt) ? "" : Form("_st%d", iSt);                // Histogram name suffix
    TString tsuff = (iSt == nSt) ? "" : Form(" in MuCh station %d", iSt);  // Histogram title suffix
    TString sN    = "";
    TString sT    = "";

    // Hit occupancy
    sN                         = (TString) "hit_ypos_vs_xpos" + nsuff;
    sT                         = (TString) "Hit occupancy in xy-Plane" + tsuff + ";x_{hit} [cm];y_{hit} [cm]";
    fvph_hit_ypos_vs_xpos[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, kRHitX[0], kRHitX[1], kNbins, kRHitY[0], kRHitY[1]);

    sN                         = (TString) "hit_xpos_vs_zpos" + nsuff;
    sT                         = (TString) "Hit occupancy in xz-Plane" + tsuff + ";z_{hit} [cm];x_{hit} [cm]";
    fvph_hit_xpos_vs_zpos[iSt] = MakeHisto<TH2F>(sN, sT, kNbinsZ, kRHitZ[0], kRHitZ[1], kNbins, kRHitX[0], kRHitX[1]);

    sN                         = (TString) "hit_ypos_vs_zpos" + nsuff;
    sT                         = (TString) "Hit occupancy in yz-plane" + tsuff + ";z_{hit} [cm];y_{hit} [cm]";
    fvph_hit_ypos_vs_zpos[iSt] = MakeHisto<TH2F>(sN, sT, kNbinsZ, kRHitZ[0], kRHitZ[1], kNbins, kRHitY[0], kRHitY[1]);

    // Hit errors
    sN               = (TString) "hit_dx" + nsuff;
    sT               = (TString) "Hit position error along x-axis" + tsuff + ";dx_{hit} [cm]";
    fvph_hit_dx[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRHitDx[0], kRHitDx[1]);

    sN               = (TString) "hit_dy" + nsuff;
    sT               = (TString) "Hit position error along y-axis" + tsuff + ";dy_{hit} [cm]";
    fvph_hit_dy[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRHitDy[0], kRHitDy[1]);

    sN               = (TString) "hit_dt" + nsuff;
    sT               = (TString) "Hit time error" + tsuff + ";dt_{hit} [ns]";
    fvph_hit_dt[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRHitDt[0], kRHitDt[1]);

    if (iSt == nSt) { continue; }  // Following histograms are defined only for separate station

    sN = (TString) "hit_station_delta_z" + nsuff;
    sT = (TString) "Different between hit and station z-positions" + tsuff + ";z_{hit} - z_{st} [cm]";
    fvph_hit_station_delta_z[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, -5., 5);

  }  // loop over station index: end

  // ----- Initialize histograms, which are use MC-information
  if (IsMCUsed()) {
    // Resize histogram vectors
    fvph_n_points_per_hit.resize(nSt + 1, nullptr);
    fvph_point_ypos_vs_xpos.resize(nSt + 1, nullptr);
    fvph_point_xpos_vs_zpos.resize(nSt + 1, nullptr);
    fvph_point_ypos_vs_zpos.resize(nSt + 1, nullptr);
    fvph_point_hit_delta_z.resize(nSt + 1, nullptr);
    fvph_res_x.resize(nSt + 1, nullptr);
    fvph_res_y.resize(nSt + 1, nullptr);
    fvph_res_t.resize(nSt + 1, nullptr);
    fvph_pull_x.resize(nSt + 1, nullptr);
    fvph_pull_y.resize(nSt + 1, nullptr);
    fvph_pull_t.resize(nSt + 1, nullptr);
    fvph_res_x_vs_x.resize(nSt + 1, nullptr);
    fvph_res_y_vs_y.resize(nSt + 1, nullptr);
    fvph_res_t_vs_t.resize(nSt + 1, nullptr);
    fvph_pull_x_vs_x.resize(nSt + 1, nullptr);
    fvph_pull_y_vs_y.resize(nSt + 1, nullptr);
    fvph_pull_t_vs_t.resize(nSt + 1, nullptr);
    fvpe_reco_eff_vs_xy.resize(nSt + 1, nullptr);
    fvpe_reco_eff_vs_r.resize(nSt + 1, nullptr);

    for (int iSt = 0; iSt <= nSt; ++iSt) {
      TString nsuff = (iSt == nSt) ? "" : Form("_st%d", iSt);                // Histogram name suffix
      TString tsuff = (iSt == nSt) ? "" : Form(" in MuCh station %d", iSt);  // Histogram title suffix
      TString sN    = "";
      TString sT    = "";

      sN                         = (TString) "n_points_per_hit" + nsuff;
      sT                         = (TString) "Number of points per hit" + tsuff + ";N_{point}/hit";
      fvph_n_points_per_hit[iSt] = MakeHisto<TH1F>(sN, sT, 10, -0.5, 9.5);

      // Point occupancy
      sN = (TString) "point_ypos_vs_xpos" + nsuff;
      sT = (TString) "Point occupancy in XY plane" + tsuff + ";x_{MC} [cm];y_{MC} [cm]";
      fvph_point_ypos_vs_xpos[iSt] =
        MakeHisto<TH2F>(sN, sT, kNbins, kRHitX[0], kRHitX[1], kNbins, kRHitY[0], kRHitY[1]);

      sN = (TString) "point_xpos_vs_zpos" + nsuff;
      sT = (TString) "Point Occupancy in XZ plane" + tsuff + ";z_{MC} [cm];x_{MC} [cm]";
      fvph_point_xpos_vs_zpos[iSt] =
        MakeHisto<TH2F>(sN, sT, kNbinsZ, kRHitZ[0], kRHitZ[1], kNbins, kRHitX[0], kRHitX[1]);

      sN = (TString) "point_ypos_vs_zpos" + nsuff;
      sT = (TString) "Point Occupancy in YZ Plane" + tsuff + ";z_{MC} [cm];y_{MC} [cm]";
      fvph_point_ypos_vs_zpos[iSt] =
        MakeHisto<TH2F>(sN, sT, kNbinsZ, kRHitZ[0], kRHitZ[1], kNbins, kRHitY[0], kRHitY[1]);

      // Difference between MC input z and hit z coordinates
      sN = (TString) "point_hit_delta_z" + nsuff;
      sT = (TString) "Distance between point and hit along z axis" + tsuff + ";z_{reco} - z_{MC} [cm]";
      fvph_point_hit_delta_z[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, -0.04, 0.04);

      sN              = (TString) "res_x" + nsuff;
      sT              = (TString) "Residuals for x-coordinate" + tsuff + ";x_{reco} - x_{MC} [cm]";
      fvph_res_x[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResX[0], kRResX[1]);

      sN              = (TString) "res_y" + nsuff;
      sT              = (TString) "Residuals for y-coordinate" + tsuff + ";y_{reco} - y_{MC} [cm]";
      fvph_res_y[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResY[0], kRResY[1]);

      sN              = (TString) "res_t" + nsuff;
      sT              = (TString) "Residuals for time" + tsuff + ";t_{reco} - t_{MC} [ns]";
      fvph_res_t[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResT[0], kRResT[1]);

      sN               = (TString) "pull_x" + nsuff;
      sT               = (TString) "Pulls for x-coordinate" + tsuff + ";(x_{reco} - x_{MC}) / #sigma_{x}^{reco}";
      fvph_pull_x[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullX[0], kRPullX[1]);

      sN               = (TString) "pull_y" + nsuff;
      sT               = (TString) "Pulls for y-coordinate" + tsuff + ";(y_{reco} - y_{MC}) / #sigma_{y}^{reco}";
      fvph_pull_y[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullY[0], kRPullY[1]);

      sN               = (TString) "pull_t" + nsuff;
      sT               = (TString) "Pulls for time" + tsuff + ";(t_{reco} - t_{MC}) / #sigma_{t}^{reco}";
      fvph_pull_t[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullT[0], kRPullT[1]);

      sN                   = (TString) "res_x_vs_x" + nsuff;
      sT                   = (TString) "Residuals for x-coordinate" + tsuff + ";x_{reco} [cm];x_{reco} - x_{MC} [cm]";
      fvph_res_x_vs_x[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResX[0], kRResX[1]);

      sN                   = (TString) "res_y_vs_y" + nsuff;
      sT                   = (TString) "Residuals for y-coordinate" + tsuff + ";y_{reco} [cm];y_{reco} - y_{MC} [cm]";
      fvph_res_y_vs_y[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResY[0], kRResY[1]);

      sN                   = (TString) "res_t_vs_t" + nsuff;
      sT                   = (TString) "Residuals for time" + tsuff + "t_{reco} [ns];t_{reco} - t_{MC} [ns]";
      fvph_res_t_vs_t[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResT[0], kRResT[1]);

      sN = (TString) "pull_x_vs_x" + nsuff;
      sT = (TString) "Pulls for x-coordinate" + tsuff + "x_{reco} [cm];(x_{reco} - x_{MC}) / #sigma_{x}^{reco}";
      fvph_pull_x_vs_x[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullX[0], kRPullX[1]);

      sN = (TString) "pull_y_vs_y" + nsuff;
      sT = (TString) "Pulls for y-coordinate" + tsuff + ";y_{reco} [cm];(y_{reco} - y_{MC}) / #sigma_{y}^{reco}";
      fvph_pull_y_vs_y[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullY[0], kRPullY[1]);

      sN = (TString) "pull_t_vs_t" + nsuff;
      sT = (TString) "Pulls for time" + tsuff + ";t_{reco} [ns];(t_{reco} - t_{MC}) / #sigma_{t}^{reco}";
      fvph_pull_t_vs_t[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullT[0], kRPullT[1]);


      sN                       = (TString) "reco_eff_vs_xy" + nsuff;
      sT                       = (TString) "Hit rec. efficiency" + tsuff + ";x_{MC} [cm];y_{MC} [cm];#epsilon";
      fvpe_reco_eff_vs_xy[iSt] = MakeEfficiency<CbmQaEff>(sN, sT, 100, -50, 50, 100, -50, 50);

      sN                      = (TString) "reco_eff_vs_r" + nsuff;
      sT                      = (TString) "Hit rec. efficiency" + tsuff + ";r_{MC} [cm];#epsilon";
      fvpe_reco_eff_vs_r[iSt] = MakeEfficiency<CbmQaEff>(sN, sT, 100, 0, 100);
    }
  }
  return kSUCCESS;
}
