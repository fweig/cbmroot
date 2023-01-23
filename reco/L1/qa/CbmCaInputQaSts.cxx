/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaInputQaSts.cxx
/// \date   13.01.2023
/// \brief  QA-task for CA tracking input from MuCh detector (implementation)
/// \author S.Zharko <s.zharko@gsi.de>

#include "CbmCaInputQaSts.h"

#include "CbmAddress.h"
#include "CbmCaQaCuts.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmQaCanvas.h"
#include "CbmQaEff.h"
#include "CbmStsCluster.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsTrackingInterface.h"
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

ClassImp(CbmCaInputQaSts);

namespace cuts = cbm::ca::qa::cuts;  // from CbmCaQaCuts.h
namespace phys = L1Constants::phys;  // from L1Constants.h

// ---------------------------------------------------------------------------------------------------------------------
//
CbmCaInputQaSts::CbmCaInputQaSts(int verbose, bool isMCUsed) : CbmQaTask("CbmCaInputQaSts", verbose, isMCUsed) {}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaInputQaSts::AnalyzeHistograms()
{
  int nSt = fpDetInterface->GetNtrackingStations();

  // ----- Fit efficiency curves
  TF1* pFitFn = new TF1("fitfn", "pol0", kEffRangeMin, kEffRangeMax);

  for (int iSt = 0; iSt < nSt; ++iSt) {
    pFitFn->SetParameter(0, 0.5);
    fvpe_reco_eff_vs_r[iSt]->Fit(pFitFn);
  }

  // ----- Fit pull distributions
  if (!gROOT->FindObject("Expk")) {
    new TFormula("Expk", "TMath::Power(TMath::Sqrt(1 + x[1] * x[1] * x[0] * x[0]) + x[0] * x[1], 1./x[1])");
  }

  TF1* pPullFit = new TF1("pullFitGausn", "gausn", -10., 10.);
  //TF1* pPullFit = new TF1("pullFitGausn", "[0] * Expk(-[2] * (x - [1]) * (x - [1]), [3])", -10., 10.);

  auto FitPulls = [&](TH1* pH, TF1* pFit) {
    pFit->SetParameter(0, 100);
    pFit->SetParameter(1, 0.001);
    pFit->SetParameter(2, 1.000);
    //pFit->SetParameter(3, 0.5);
    pH->Fit(pFit);
  };

  for (int iSt = 0; iSt < nSt; ++iSt) {
    FitPulls(fvph_pull_x[iSt], pPullFit);
    FitPulls(fvph_pull_y[iSt], pPullFit);
    FitPulls(fvph_pull_u[iSt], pPullFit);
    FitPulls(fvph_pull_v[iSt], pPullFit);
    FitPulls(fvph_pull_t[iSt], pPullFit);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmCaInputQaSts::Check() const
{
  LOG_IF(info, fVerbose > 0) << fName << ": Checking QA ...";

  bool res = true;

  int nSt = fpDetInterface->GetNtrackingStations();


  // **************************************************************
  // ** Basic checks, available both for real and simulated data **
  // **************************************************************

  // ----- Checks for mismatches in the order of stations
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
    int iBinMin = fvph_hit_station_delta_z[iSt]->FindBin(-cuts::kMaxDzStHitSts);
    int iBinMax = fvph_hit_station_delta_z[iSt]->FindBin(+cuts::kMaxDzStHitSts);

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
    LOG(info) << "-- Hit efficiency integrated over hit distance from station center";
    LOG(info) << "\tintergration range: [" << kEffRangeMin << ", " << kEffRangeMax << "] cm";
    LOG(info) << std::setw(10) << std::setfill(' ') << "St. ID" << ' ' << std::setw(12) << std::setfill(' ') << "eps.";
    for (int iSt = 0; iSt < nSt; ++iSt) {
      auto* pFitfn = (TF1*) fvpe_reco_eff_vs_r[iSt]->GetListOfFunctions()->FindObject("fitfn");
      double eff   = pFitfn->GetParameter(0);
      LOG(info) << std::setw(10) << std::setfill(' ') << iSt << ' ' << std::setw(12) << std::setfill(' ') << eff;
      if (eff < cuts::kMinEff) {
        LOG(error) << fName << ": station " << iSt << " has efficiency lower threshold (" << eff << " < "
                   << cuts::kMinEff << ')';
        res = false;
      }
    }

    // ----- Checks for pulls
    // For the hit pull is defined as a ration of the difference between hit and MC-point position or time component
    // values and an error of the hit position (or time) component, calculated in the same z-positions. In ideal case,
    // when the resolutions are well determined for detecting stations, the pull distributions should have a RMS equal
    // to unity. Here we allow a RMS of the pull distributions to be varied in a predefined range. If the RMS runs out
    // this range, QA task fails.
    // TODO: Add checks for center

    // Function returns width of histogram
    auto GetPullWidth = [&](TH1* pHist) {
      if (pHist->FindObject("pullFitGausn")) {
        TF1* pFitFn = (TF1*) pHist->FindObject("pullFitGausn");
        return pFitFn->GetParameter(2);
      }
      LOG(error) << fName << ": no fit function provided for pulls fitting (" << pHist->GetName() << "), RMS returned";
      return pHist->GetRMS();
    };

    LOG(info) << "-- Hit pull RMS:";
    LOG(info) << std::setw(10) << std::setfill(' ') << "St. ID" << ' ' << std::setw(12) << std::setfill(' ') << "RMS(x)"
              << ' ' << std::setw(12) << std::setfill(' ') << "RMS(y)" << ' ' << std::setw(12) << std::setfill(' ')
              << "RMS(u)" << ' ' << std::setw(12) << std::setfill(' ') << "RMS(v)" << ' ' << std::setw(12)
              << std::setfill(' ') << "RMS(t)";
    for (int iSt = 0; iSt < nSt; ++iSt) {
      double rmsPullX = GetPullWidth(fvph_pull_x[iSt]);
      double rmsPullY = GetPullWidth(fvph_pull_y[iSt]);
      double rmsPullU = GetPullWidth(fvph_pull_u[iSt]);
      double rmsPullV = GetPullWidth(fvph_pull_v[iSt]);
      double rmsPullT = GetPullWidth(fvph_pull_t[iSt]);
      LOG(info) << std::setw(10) << std::setfill(' ') << iSt << ' ' << std::setw(12) << std::setfill(' ') << rmsPullX
                << ' ' << std::setw(12) << std::setfill(' ') << rmsPullY << ' ' << std::setw(12) << std::setfill(' ')
                << rmsPullU << ' ' << std::setw(12) << std::setfill(' ') << rmsPullV << ' ' << std::setw(12)
                << std::setfill(' ') << rmsPullT;
    }
  }

  LOG_IF(error, !res) << fName << ": QA check failed";

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaInputQaSts::DeInit()
{
  // Vectors with pointers to histograms
  fvph_hit_ypos_vs_xpos.clear();
  fvph_hit_xpos_vs_zpos.clear();
  fvph_hit_ypos_vs_zpos.clear();

  fvph_hit_station_delta_z.clear();

  fvph_hit_dx.clear();
  fvph_hit_dy.clear();
  fvph_hit_du.clear();
  fvph_hit_dv.clear();
  fvph_hit_dt.clear();

  fvph_n_points_per_hit.clear();

  fvph_point_ypos_vs_xpos.clear();
  fvph_point_xpos_vs_zpos.clear();
  fvph_point_ypos_vs_zpos.clear();

  fvph_point_hit_delta_z.clear();

  fvph_res_x.clear();
  fvph_res_y.clear();
  fvph_res_u.clear();
  fvph_res_v.clear();
  fvph_res_t.clear();

  fvph_pull_x.clear();
  fvph_pull_y.clear();
  fvph_pull_u.clear();
  fvph_pull_v.clear();
  fvph_pull_t.clear();

  fvph_res_x_vs_x.clear();
  fvph_res_y_vs_y.clear();
  fvph_res_u_vs_u.clear();
  fvph_res_v_vs_v.clear();
  fvph_res_t_vs_t.clear();

  fvph_pull_x_vs_x.clear();
  fvph_pull_y_vs_y.clear();
  fvph_pull_u_vs_u.clear();
  fvph_pull_v_vs_v.clear();
  fvph_pull_t_vs_t.clear();

  fvpe_reco_eff_vs_xy.clear();
  fvpe_reco_eff_vs_r.clear();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaInputQaSts::FillHistograms()
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
    const auto* pHit = dynamic_cast<const CbmStsHit*>(fpHits->At(iH));
    LOG_IF(fatal, !pHit) << fName << ": hit with iH = " << iH << " is not an CbmStsHit (dynamic cast failed)";

    // *************************
    // ** Reconstructed hit QA

    // Hit station geometry info
    int iSt = fpDetInterface->GetTrackingStationIndex(pHit);
    LOG_IF(fatal, iSt < 0 || iSt >= nSt) << fName << ": index of station (" << iSt << ") is out of range "
                                         << "for hit with id = " << iH;
    double stPhiF = fpDetInterface->GetStripsStereoAngleFront(iSt);  // STS front strips stereo angle
    double stPhiB = fpDetInterface->GetStripsStereoAngleBack(iSt);   // STS back strips stereo angle

    // Hit position
    double xHit = pHit->GetX();
    double yHit = pHit->GetY();
    double zHit = pHit->GetZ();
    double uHit = xHit * cos(stPhiF) + yHit * sin(stPhiB);
    double vHit = xHit * cos(stPhiB) + yHit * sin(stPhiB);

    double duHit = pHit->GetDu();
    double dvHit = pHit->GetDv();
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
    fvph_hit_du[iSt]->Fill(duHit);
    fvph_hit_dv[iSt]->Fill(dvHit);
    fvph_hit_dt[iSt]->Fill(dtHit);

    fvph_hit_ypos_vs_xpos[nSt]->Fill(xHit, yHit);
    fvph_hit_xpos_vs_zpos[nSt]->Fill(zHit, xHit);
    fvph_hit_ypos_vs_zpos[nSt]->Fill(zHit, yHit);
    fvph_hit_dx[nSt]->Fill(dxHit);
    fvph_hit_dy[nSt]->Fill(dyHit);
    fvph_hit_du[nSt]->Fill(duHit);
    fvph_hit_dv[nSt]->Fill(dvHit);
    fvph_hit_dt[nSt]->Fill(dtHit);


    // **********************
    // ** MC information QA

    if (IsMCUsed()) {
      CbmMatch hitMatch = GetHitMatch(iH);

      // Evaluate number of hits per one MC point
      int nMCpoints = 0;  // Number of MC points for this hit
      for (int iLink = 0; iLink < hitMatch.GetNofLinks(); ++iLink) {
        const auto& link = hitMatch.GetLink(iLink);

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
      const auto& bestPointLink = hitMatch.GetMatchedLink();

      // Skip noisy links
      if (bestPointLink.GetIndex() < 0) { continue; }

      // Point matched by the best link
      const auto* pMCPoint = dynamic_cast<const CbmStsPoint*>(fpMCPoints->Get(bestPointLink));
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
      //double uMC = xMC * cos(stPhiF) + yMC * sin(stPhiF);
      //double vMC = xMC * cos(stPhiB) + yMC * sin(stPhiB);
      double zMC = pMCPoint->GetZIn();
      double tMC = pMCPoint->GetTime() + t0MC;

      // MC point entrance momenta
      double pxMC = pMCPoint->GetPx();
      double pyMC = pMCPoint->GetPy();
      double pzMC = pMCPoint->GetPz();
      double pMC  = sqrt(pxMC * pxMC + pyMC * pyMC + pzMC * pzMC);

      // MC point exit momenta
      double pxMCo = pMCPoint->GetPxOut();
      double pyMCo = pMCPoint->GetPyOut();
      double pzMCo = pMCPoint->GetPzOut();
      double pMCo  = sqrt(pxMCo * pxMCo + pyMCo * pyMCo + pzMCo * pzMCo);

      // Position and time shifted to z-coordinate of the hit
      double shiftZ = zHit - zMC;  // Difference between hit and point z positions
      double xMCs   = xMC + shiftZ * pxMC / pzMC;
      double yMCs   = yMC + shiftZ * pyMC / pzMC;
      double uMCs   = xMCs * cos(stPhiF) + yMCs * sin(stPhiF);
      double vMCs   = xMCs * cos(stPhiB) + yMCs * sin(stPhiB);
      double tMCs   = tMC + shiftZ / (pzMC * phys::kSpeedOfLight) * sqrt(mass * mass + pMC * pMC);

      // Residuals
      double xRes = xHit - xMCs;
      double yRes = yHit - yMCs;
      double uRes = uHit - uMCs;
      double vRes = vHit - vMCs;
      double tRes = tHit - tMCs;

      // ------ Cuts

      if (std::fabs(pMCPoint->GetPzOut()) < 0.01) { continue; }  // CUT ON MINIMUM MOMENTUM
      //if (pMCo < cuts::kMinP) { continue; }  // Momentum threshold


      fvph_point_ypos_vs_xpos[iSt]->Fill(xMC, yMC);
      fvph_point_xpos_vs_zpos[iSt]->Fill(zMC, xMC);
      fvph_point_ypos_vs_zpos[iSt]->Fill(zMC, yMC);

      fvph_point_hit_delta_z[iSt]->Fill(shiftZ);

      fvph_res_x[iSt]->Fill(xRes);
      fvph_res_y[iSt]->Fill(yRes);
      fvph_res_u[iSt]->Fill(uRes);
      fvph_res_v[iSt]->Fill(vRes);
      fvph_res_t[iSt]->Fill(tRes);

      fvph_pull_x[iSt]->Fill(xRes / dxHit);
      fvph_pull_y[iSt]->Fill(yRes / dyHit);
      fvph_pull_u[iSt]->Fill(uRes / duHit);
      fvph_pull_v[iSt]->Fill(vRes / dvHit);
      fvph_pull_t[iSt]->Fill(tRes / dtHit);

      fvph_res_x_vs_x[iSt]->Fill(xHit, xRes);
      fvph_res_y_vs_y[iSt]->Fill(yHit, yRes);
      fvph_res_u_vs_u[iSt]->Fill(uHit, uRes);
      fvph_res_v_vs_v[iSt]->Fill(vHit, vRes);
      fvph_res_t_vs_t[iSt]->Fill(tHit, tRes);

      fvph_pull_x_vs_x[iSt]->Fill(xHit, xRes / dxHit);
      fvph_pull_y_vs_y[iSt]->Fill(yHit, yRes / dyHit);
      fvph_pull_u_vs_u[iSt]->Fill(uHit, uRes / duHit);
      fvph_pull_v_vs_v[iSt]->Fill(vHit, vRes / dvHit);
      fvph_pull_t_vs_t[iSt]->Fill(tHit, tRes / dtHit);

      fvph_point_ypos_vs_xpos[nSt]->Fill(xMC, yMC);
      fvph_point_xpos_vs_zpos[nSt]->Fill(zMC, xMC);
      fvph_point_ypos_vs_zpos[nSt]->Fill(zMC, yMC);

      fvph_point_hit_delta_z[nSt]->Fill(shiftZ);

      fvph_res_x[nSt]->Fill(xRes);
      fvph_res_y[nSt]->Fill(yRes);
      fvph_res_u[nSt]->Fill(uRes);
      fvph_res_v[nSt]->Fill(vRes);
      fvph_res_t[nSt]->Fill(tRes);

      fvph_pull_x[nSt]->Fill(xRes / dxHit);
      fvph_pull_y[nSt]->Fill(yRes / dyHit);
      fvph_pull_u[nSt]->Fill(uRes / duHit);
      fvph_pull_v[nSt]->Fill(vRes / dvHit);
      fvph_pull_t[nSt]->Fill(tRes / dtHit);

      fvph_res_x_vs_x[nSt]->Fill(xHit, xRes);
      fvph_res_y_vs_y[nSt]->Fill(yHit, yRes);
      fvph_res_u_vs_u[nSt]->Fill(uHit, uRes);
      fvph_res_v_vs_v[nSt]->Fill(vHit, vRes);
      fvph_res_t_vs_t[nSt]->Fill(tHit, tRes);

      fvph_pull_x_vs_x[nSt]->Fill(xHit, xRes / dxHit);
      fvph_pull_y_vs_y[nSt]->Fill(yHit, yRes / dyHit);
      fvph_pull_u_vs_u[nSt]->Fill(uHit, uRes / duHit);
      fvph_pull_v_vs_v[nSt]->Fill(vHit, vRes / dvHit);
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
        const auto* pMCPoint = dynamic_cast<const CbmStsPoint*>(fpMCPoints->Get(iFile, iEvent, iP));
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
        bool isOneHitForOnePoint  = (vNofHitsPerPoint[iE][iP] == 1);
        bool isSevHitsForOnePoint = (vNofHitsPerPoint[iE][iP] > 1);

        fvpe_reco_eff_vs_xy[iSt]->Fill(isOneHitForOnePoint, xMC, yMC);
        fvpe_reco_eff_vs_xy[nSt]->Fill(isOneHitForOnePoint, xMC, yMC);

        fvpe_reco_eff_vs_r[iSt]->Fill(isOneHitForOnePoint, rMC);
        fvpe_reco_eff_vs_r[nSt]->Fill(isOneHitForOnePoint, rMC);

      }  // loop over MC-points: end
    }    // loop over MC-events: end
  }      // MC is used: end
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaSts::InitDataBranches()
{
  // STS tracking detector interface
  fpDetInterface = CbmStsTrackingInterface::Instance();

  LOG_IF(fatal, !fpDetInterface) << "\033[1;31m" << fName << ": tracking detector interface is undefined\033[0m";

  // FairRootManager
  auto* pFairRootManager = FairRootManager::Instance();
  LOG_IF(fatal, !pFairRootManager) << "\033[1;31m" << fName << ": FairRootManager instance is a null pointer\033[0m";

  // Time-slice
  fpTimeSlice = dynamic_cast<CbmTimeSlice*>(pFairRootManager->GetObject("TimeSlice."));
  LOG_IF(fatal, !fpTimeSlice) << "\033[1;31m" << fName << ": time-slice branch is not found\033[0m";

  // ----- Reconstructed data-branches initialization

  // Hits container
  fpHits = dynamic_cast<TClonesArray*>(pFairRootManager->GetObject("StsHit"));
  LOG_IF(fatal, !fpHits) << "\033[1;31m" << fName << ": container of reconstructed hits in STS is not found\033[0m";

  // Clusters container
  fpClusters = dynamic_cast<TClonesArray*>(pFairRootManager->GetObject("StsCluster"));
  LOG_IF(fatal, !fpClusters) << "\033[1;31m" << fName << ": container of hit clusters in STS is not found\033[0m";


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
    fpMCPoints = fpMCDataManager->InitBranch("StsPoint");
    LOG_IF(fatal, !fpMCTracks) << "\033[1;31m" << fName << ": MC point branch is not found for STS\033[0m";

    // Cluster matches
    fpClusterMatches = dynamic_cast<TClonesArray*>(pFairRootManager->GetObject("StsClusterMatch"));
    LOG_IF(fatal, !fpClusterMatches) << "\033[1;31m]" << fName << ": cluster match branch is not found for STS\033[0m";
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaSts::InitCanvases()
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

    // x-coordinate
    auto* pc_res_u = MakeCanvas<CbmQaCanvas>("res_u", "Residuals for u coordinate", 1600, 800);
    pc_res_u->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_res_u->cd(iSt + 1);
      fvph_res_u[iSt]->DrawCopy("", "");
    }

    // x-coordinate
    auto* pc_res_v = MakeCanvas<CbmQaCanvas>("res_v", "Residuals for v coordinate", 1600, 800);
    pc_res_v->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_res_v->cd(iSt + 1);
      fvph_res_v[iSt]->DrawCopy("", "");
    }

    // x-coordinate
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

    // x-coordinate
    auto* pc_pull_u = MakeCanvas<CbmQaCanvas>("pull_u", "Pulls for u coordinate", 1600, 800);
    pc_pull_u->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_pull_u->cd(iSt + 1);
      fvph_pull_u[iSt]->DrawCopy("", "");
    }

    // x-coordinate
    auto* pc_pull_v = MakeCanvas<CbmQaCanvas>("pull_v", "Pulls for v coordinate", 1600, 800);
    pc_pull_v->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_pull_v->cd(iSt + 1);
      fvph_pull_v[iSt]->DrawCopy("", "");
    }

    // x-coordinate
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
      fvpe_reco_eff_vs_r[iSt]->Paint("AP");
      auto* pGr = dynamic_cast<TGraphAsymmErrors*>(fvpe_reco_eff_vs_r[iSt]->GetPaintedGraph());
      if (!pGr) {
        LOG(error) << fName << ": unable to get painted graph from efficiency " << fvpe_reco_eff_vs_xy[iSt]->GetName();
        continue;
      }
      pGr->DrawClone("AP");
    }

    auto* pc_reco_eff_vs_xy = MakeCanvas<CbmQaCanvas>("reco_eff_vs_xy", "Hit efficiencies wrt x and y", 1600, 800);
    pc_reco_eff_vs_xy->Divide2D(nSt);
    for (int iSt = 0; iSt < nSt; ++iSt) {
      pc_reco_eff_vs_xy->cd(iSt + 1);
      fvpe_reco_eff_vs_xy[iSt]->Paint("colz");
      auto* pH2 = dynamic_cast<TH2F*>(fvpe_reco_eff_vs_xy[iSt]->GetPaintedHistogram());
      if (!pH2) {
        LOG(error) << fName << ": unable to get painted histogram from efficiency "
                   << fvpe_reco_eff_vs_xy[iSt]->GetName();
        continue;
      }
      pH2->DrawCopy("colz", "");
    }
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmCaInputQaSts::InitHistograms()
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
  fvph_hit_dv.resize(nSt + 1, nullptr);
  fvph_hit_du.resize(nSt + 1, nullptr);

  for (int iSt = 0; iSt <= nSt; ++iSt) {
    TString nsuff = (iSt == nSt) ? "" : Form("_st%d", iSt);               // Histogram name suffix
    TString tsuff = (iSt == nSt) ? "" : Form(" in STS station %d", iSt);  // Histogram title suffix
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

    sN               = (TString) "hit_du" + nsuff;
    sT               = (TString) "Hit position error across front strips" + tsuff + ";du_{hit} [cm]";
    fvph_hit_du[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRHitDu[0], kRHitDu[1]);

    sN               = (TString) "hit_dv" + nsuff;
    sT               = (TString) "Hit position error across back strips" + tsuff + ";dv_{hit} [cm]";
    fvph_hit_dv[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRHitDv[0], kRHitDv[1]);

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
    fvph_res_u.resize(nSt + 1, nullptr);
    fvph_res_v.resize(nSt + 1, nullptr);
    fvph_res_t.resize(nSt + 1, nullptr);
    fvph_pull_x.resize(nSt + 1, nullptr);
    fvph_pull_y.resize(nSt + 1, nullptr);
    fvph_pull_u.resize(nSt + 1, nullptr);
    fvph_pull_v.resize(nSt + 1, nullptr);
    fvph_pull_t.resize(nSt + 1, nullptr);
    fvph_res_x_vs_x.resize(nSt + 1, nullptr);
    fvph_res_y_vs_y.resize(nSt + 1, nullptr);
    fvph_res_u_vs_u.resize(nSt + 1, nullptr);
    fvph_res_v_vs_v.resize(nSt + 1, nullptr);
    fvph_res_t_vs_t.resize(nSt + 1, nullptr);
    fvph_pull_x_vs_x.resize(nSt + 1, nullptr);
    fvph_pull_y_vs_y.resize(nSt + 1, nullptr);
    fvph_pull_u_vs_u.resize(nSt + 1, nullptr);
    fvph_pull_v_vs_v.resize(nSt + 1, nullptr);
    fvph_pull_t_vs_t.resize(nSt + 1, nullptr);
    fvpe_reco_eff_vs_xy.resize(nSt + 1, nullptr);
    fvpe_reco_eff_vs_r.resize(nSt + 1, nullptr);

    for (int iSt = 0; iSt <= nSt; ++iSt) {
      TString nsuff = (iSt == nSt) ? "" : Form("_st%d", iSt);               // Histogram name suffix
      TString tsuff = (iSt == nSt) ? "" : Form(" in STS station %d", iSt);  // Histogram title suffix
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
      sT = (TString) "Distance between STS point and hit along z axis" + tsuff + ";z_{reco} - z_{MC} [cm]";
      fvph_point_hit_delta_z[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, -0.04, 0.04);

      sN              = (TString) "res_x" + nsuff;
      sT              = (TString) "Residuals for x-coordinate" + tsuff + ";x_{reco} - x_{MC} [cm]";
      fvph_res_x[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResX[0], kRResX[1]);

      sN              = (TString) "res_y" + nsuff;
      sT              = (TString) "Residuals for y-coordinate" + tsuff + ";y_{reco} - y_{MC} [cm]";
      fvph_res_y[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResY[0], kRResY[1]);

      sN              = (TString) "res_u" + nsuff;
      sT              = (TString) "Residuals for front strips coordinate" + tsuff + ";u_{reco} - u_{MC} [cm]";
      fvph_res_u[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResU[0], kRResU[1]);

      sN              = (TString) "res_v" + nsuff;
      sT              = (TString) "Residuals for back strips coordinate" + tsuff + ";v_{reco} - v_{MC} [cm]";
      fvph_res_v[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResV[1], kRResV[1]);

      sN              = (TString) "res_t" + nsuff;
      sT              = (TString) "Residuals for time" + tsuff + ";t_{reco} - t_{MC} [ns]";
      fvph_res_t[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRResT[0], kRResT[1]);

      sN               = (TString) "pull_x" + nsuff;
      sT               = (TString) "Pulls for x-coordinate" + tsuff + ";(x_{reco} - x_{MC}) / #sigma_{x}^{reco}";
      fvph_pull_x[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullX[0], kRPullX[1]);

      sN               = (TString) "pull_y" + nsuff;
      sT               = (TString) "Pulls for y-coordinate" + tsuff + ";(y_{reco} - y_{MC}) / #sigma_{y}^{reco}";
      fvph_pull_y[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullY[0], kRPullY[1]);

      sN = (TString) "pull_u" + nsuff;
      sT = (TString) "Pulls for front strips coordinate" + tsuff + ";(u_{reco} - u_{MC}) / #sigma_{u}^{reco}";
      fvph_pull_u[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullU[0], kRPullU[1]);

      sN = (TString) "pull_v" + nsuff;
      sT = (TString) "Pulls for back strips coordinate" + tsuff + ";(v_{reco} - v_{MC}) / #sigma_{v}^{reco}";
      fvph_pull_v[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullV[0], kRPullV[1]);

      sN               = (TString) "pull_t" + nsuff;
      sT               = (TString) "Pulls for time" + tsuff + ";(t_{reco} - t_{MC}) / #sigma_{t}^{reco}";
      fvph_pull_t[iSt] = MakeHisto<TH1F>(sN, sT, kNbins, kRPullT[0], kRPullT[1]);

      sN                   = (TString) "res_x_vs_x" + nsuff;
      sT                   = (TString) "Residuals for x-coordinate" + tsuff + ";x_{reco} [cm];x_{reco} - x_{MC} [cm]";
      fvph_res_x_vs_x[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResX[0], kRResX[1]);

      sN                   = (TString) "res_y_vs_y" + nsuff;
      sT                   = (TString) "Residuals for y-coordinate" + tsuff + ";y_{reco} [cm];y_{reco} - y_{MC} [cm]";
      fvph_res_y_vs_y[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResY[0], kRResY[1]);

      sN = (TString) "res_u_vs_u" + nsuff;
      sT = (TString) "Residuals for front strips coordinate" + tsuff + ";u_{reco} [cm];u_{reco} - u_{MC} [cm]";
      fvph_res_u_vs_u[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResU[0], kRResU[1]);

      sN = (TString) "res_v_vs_u" + nsuff;
      sT = (TString) "Residuals for back strips coordinate" + tsuff + ";v_{reco} [cm];v_{reco} - v_{MC} [cm]";
      fvph_res_v_vs_v[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResV[0], kRResV[1]);

      sN                   = (TString) "res_t_vs_t" + nsuff;
      sT                   = (TString) "Residuals for time" + tsuff + "t_{reco} [ns];t_{reco} - t_{MC} [ns]";
      fvph_res_t_vs_t[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRResT[0], kRResT[1]);

      sN = (TString) "pull_x_vs_x" + nsuff;
      sT = (TString) "Pulls for x-coordinate" + tsuff + "x_{reco} [cm];(x_{reco} - x_{MC}) / #sigma_{x}^{reco}";
      fvph_pull_x_vs_x[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullX[0], kRPullX[1]);

      sN = (TString) "pull_y_vs_y" + nsuff;
      sT = (TString) "Pulls for y-coordinate" + tsuff + ";y_{reco} [cm];(y_{reco} - y_{MC}) / #sigma_{y}^{reco}";
      fvph_pull_y_vs_y[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullY[0], kRPullY[1]);

      sN = (TString) "pull_u_vs_u" + nsuff;
      sT = (TString) "Pulls for front strips coordinate" + tsuff
           + ";u_{reco} [cm];(u_{reco} - u_{MC}) / #sigma_{u}^{reco}";
      fvph_pull_u_vs_u[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullU[0], kRPullU[1]);

      sN = (TString) "pull_v_vs_v" + nsuff;
      sT =
        (TString) "Pulls for back strips coordinate" + tsuff + ";v_{reco} [cm];(v_{reco} - v_{MC}) / #sigma_{v}^{reco}";
      fvph_pull_v_vs_v[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullV[0], kRPullV[1]);

      sN = (TString) "pull_t_vs_t" + nsuff;
      sT = (TString) "Pulls for time" + tsuff + ";t_{reco} [ns];(t_{reco} - t_{MC}) / #sigma_{t}^{reco}";
      fvph_pull_t_vs_t[iSt] = MakeHisto<TH2F>(sN, sT, kNbins, 0, 0, kNbins, kRPullT[0], kRPullT[1]);


      sN                       = (TString) "reco_eff_vs_xy" + nsuff;
      sT                       = (TString) "Hit rec. efficiency" + tsuff + ";x_{MC} [cm];y_{MC} [cm];#epsilon";
      fvpe_reco_eff_vs_xy[iSt] = MakeEfficiency<TEfficiency>(sN, sT, 100, -50, 50, 100, -50, 50);

      sN                      = (TString) "reco_eff_vs_r" + nsuff;
      sT                      = (TString) "Hit rec. efficiency" + tsuff + ";r_{MC} [cm];#epsilon";
      fvpe_reco_eff_vs_r[iSt] = MakeEfficiency<TEfficiency>(sN, sT, 100, -70, 70);
    }
  }
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmMatch CbmCaInputQaSts::GetHitMatch(int iHit) const
{
  CbmMatch aMatch;

  const auto* pHit = static_cast<const CbmStsHit*>(fpHits->At(iHit));  // NOTE: dynamic cast check is done in main loop

  // Get front and back clusters
  int iCf = pHit->GetFrontClusterId();
  int iCb = pHit->GetBackClusterId();

  // TODO: errors instead of fatals?
  LOG_IF(fatal, iCf < 0) << fName << ": hit with id = " << iHit << " has wrong front cluster (iCf = " << iCf << ')';
  LOG_IF(fatal, iCb < 0) << fName << ": hit with id = " << iHit << " has wrong back cluster (iCb = " << iCb << ')';

  const auto* pClusterF = dynamic_cast<const CbmStsCluster*>(fpClusters->At(iCf));
  const auto* pClusterB = dynamic_cast<const CbmStsCluster*>(fpClusters->At(iCb));

  LOG_IF(fatal, !pClusterF) << fName << ": front cluster does not exist for hit with id = " << iHit
                            << " and cluster id = " << iCf;
  LOG_IF(fatal, !pClusterB) << fName << ": back cluster does not exist for hit with id = " << iHit
                            << " and cluster id = " << iCb;

  const auto* pClusterMatchF = dynamic_cast<const CbmMatch*>(fpClusterMatches->At(iCf));
  const auto* pClusterMatchB = dynamic_cast<const CbmMatch*>(fpClusterMatches->At(iCb));

  LOG_IF(fatal, !pClusterMatchF) << fName << ": front cluster match was not found for cluster with id = " << iCf;
  LOG_IF(fatal, !pClusterMatchB) << fName << ": back cluster match was not found for cluster with id = " << iCf;

  // Check addresses of hit and cluster
  int addrHit      = pHit->GetAddress();
  int addrClusterF = pClusterF->GetAddress();
  int addrClusterB = pClusterB->GetAddress();
  bool ifAddrCons  = addrHit == addrClusterF && addrClusterF == addrClusterB;
  LOG_IF(fatal, !ifAddrCons) << fName << ": hit, front and (or) back cluster has inconsistent addresses: hit - "
                             << addrHit << ", front - " << addrClusterF << ", back - " << addrClusterB << '\n';

  // CUSTOM MATCHING IN STS: The link is selected if it is presented BOTH in front and back cluster, thus only true
  //                         hits are matched. The standard matching in STS assumes only one (front or back) cluster
  //                         to be matched, so it matches both true and fake hits.

  for (int iLinkF = 0; iLinkF < pClusterMatchF->GetNofLinks(); ++iLinkF) {
    const auto& linkF = pClusterMatchF->GetLink(iLinkF);
    for (int iLinkB = 0; iLinkB < pClusterMatchB->GetNofLinks(); ++iLinkB) {
      const auto& linkB = pClusterMatchB->GetLink(iLinkB);
      if (linkF == linkB) {
        aMatch.AddLink(linkF);
        aMatch.AddLink(linkB);
      }
    }
  }
  return aMatch;
}
