/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTrackFitQa.cxx
/// @brief  QA submodule for track fit results (residuals and puls) at selected z-coordinate (implementation)
/// @since  03.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaTrackFitQa.h"

#include "CbmL1Track.h"
#include "CbmQaCanvas.h"

#include "TF1.h"
#include "TFormula.h"
#include "TH1.h"
#include "TProfile.h"
#include "TString.h"

#include <algorithm>

#include "CaToolsMCData.h"
#include "L1Field.h"
#include "L1Fit.h"
#include "L1Utils.h"


// *******************************************************
// **  Implementation of cbm::ca::TrackFitQa functions  **
// *******************************************************

using cbm::ca::TrackFitQa;

// ---------------------------------------------------------------------------------------------------------------------
//
TrackFitQa::TrackFitQa(const char* pointTag, const char* prefixName, TFolder* pParentFolder)
  : CbmQaIO(pointTag, Form("%s_%s", prefixName, pointTag), pParentFolder)
{
  fStoringMode = EStoringMode::kSAMEDIR;
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaCanvas* TrackFitQa::CreateResidualPlot()
{
  auto* pCanv = MakeCanvas<CbmQaCanvas>("canv_residuals", " residuals", kCXSIZEPX * 4, kCYSIZEPX * 2);
  pCanv->Divide2D(7);


  for (int iType = static_cast<int>(ETrackParType::kBEGIN); iType < static_cast<int>(ETrackParType::kEND); ++iType) {
    ETrackParType type = static_cast<ETrackParType>(iType);
    if (fvbParIgnored[type]) { continue; }
    pCanv->cd(iType + 1);
    fvphResiduals[type]->Draw();
  }

  return pCanv;
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaCanvas* TrackFitQa::CreatePullPlot()
{
  auto* pCanv = MakeCanvas<CbmQaCanvas>(fsPrefix + "_canv_pulls", " pulls", kCXSIZEPX * 4, kCYSIZEPX * 2);
  pCanv->Divide2D(7);

  for (int iType = static_cast<int>(ETrackParType::kBEGIN); iType < static_cast<int>(ETrackParType::kEND); ++iType) {
    ETrackParType type = static_cast<ETrackParType>(iType);
    if (fvbParIgnored[type]) { continue; }
    auto fit = TF1("fitpull", "[0] * TMath::Exp(TMath::ASinH(-0.5*[3]*((x-[1])/[2])**2)/[3])", -10., 10.);
    fit.SetParameters(100, 0., 1., .3);
    fit.SetParLimits(3, 0., 2.);
    pCanv->cd(iType + 1);
    fvphPulls[type]->Draw();
    fvphPulls[type]->Fit("fitpull", "Q");
  }

  return pCanv;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackFitQa::Init()
{
  // Init default distribution properties
  SetDefaultProperties();

  auto CreateResidualHisto = [&](ETrackParType t, const char* name, const char* title) {
    if (fvbParIgnored[t]) { return; }
    TString sPrefix  = (fsTitle.Length() > 0) ? fsTitle + " point residual for " : "residual for ";
    fvphResiduals[t] = MakeHisto<TH1F>(name, sPrefix + title, fvRBins[t], fvRLo[t], fvRUp[t]);
  };

  auto CreatePullHisto = [&](ETrackParType t, const char* name, const char* title) {
    if (fvbParIgnored[t]) { return; }
    TString sPrefix = (fsTitle.Length() > 0) ? fsTitle + " point pull for " : "pull for ";
    fvphPulls[t]    = MakeHisto<TH1F>(name, sPrefix + title, fvPBins[t], fvPLo[t], fvPUp[t]);
  };

  CreateResidualHisto(ETrackParType::kX, "res_x", "x-coordinate;x^{reco} - x^{MC} [cm]");
  CreateResidualHisto(ETrackParType::kY, "res_y", "y-coordinate;y^{reco} - y^{MC} [cm]");
  CreateResidualHisto(ETrackParType::kTX, "res_tx", "slope along x-axis;T_{x}^{reco} - T_{x}^{MC}");
  CreateResidualHisto(ETrackParType::kTY, "res_ty", "slope along y-axis;T_{y}^{reco} - T_{y}^{MC}");
  CreateResidualHisto(ETrackParType::kQP, "res_qp", "charge over mom.;(q/p)^{reco} - (q/p)^{MC} [ec/GeV]");
  CreateResidualHisto(ETrackParType::kTIME, "res_t", "time;t^{reco} - t^{MC} [ns]");
  CreateResidualHisto(ETrackParType::kVI, "res_vi", "inverse speed;v^{-1}_{reco} - v^{-1}_{MC} [c^{-1}]");

  CreatePullHisto(ETrackParType::kX, "pull_x", "x-coordinate;(x^{reco} - x^{MC})/#sigma_{x}");
  CreatePullHisto(ETrackParType::kY, "pull_y", "y-coordinate;(y^{reco} - y^{MC})/#sigma_{y}");
  CreatePullHisto(ETrackParType::kTX, "pull_tx", "slope along x-axis;(T_{x}^{reco} - T_{x}^{MC})/#sigma_{T_{x}}");
  CreatePullHisto(ETrackParType::kTY, "pull_ty", "slope along y-axis;(T_{y}^{reco} - T_{y}^{MC})/#sigma_{T_{y}}");
  CreatePullHisto(ETrackParType::kQP, "pull_qp", "charge over mom.;((q/p)^{reco} - (q/p)^{MC})/#sigma_{q/p}");
  CreatePullHisto(ETrackParType::kTIME, "pull_t", "time;(t^{reco} - t^{MC})/#sigma_{t}");
  CreatePullHisto(ETrackParType::kVI, "pull_vi", "inverse speed;(v^{-1}_{reco} - v^{-1}_{MC})/#sigma_{v^{-1}}");

  // FIXME: Replace hardcoded parameters with variables
  fph_res_p_pMC         = MakeHisto<TProfile>("res_p_vs_pMC", "", 100, 0.0, 10.0, -2., 2.);
  fph_res_phi_phiMC     = MakeHisto<TProfile>("res_phi_vs_phiMC", "", 100, -3.2, 3.2, -2., 2.);
  fph_res_theta_thetaMC = MakeHisto<TProfile>("res_theta_vs_phiMC", "", 100, 0., 3.2, -2., 2.);

  // Set histogram titles
  TString sPrefix = (fsTitle.Length() > 0) ? fsTitle + " point " : "";
  fph_res_p_pMC->SetTitle(sPrefix + " resolution of momentum;p^{MC} [GeV/c];#delta p [GeV/c]");
  fph_res_phi_phiMC->SetTitle(sPrefix + " resolution of polar angle;#phi^{MC} [rad];#delta#phi [rad]");
  fph_res_theta_thetaMC->SetTitle(sPrefix + " resolution of polar angle;#theta^{MC} [rad];#delta#theta [rad]");
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackFitQa::Fill(const L1TrackPar& trPar, const ::ca::tools::MCPoint& mcPoint, bool bTimeMeasured, double weight)
{
  // Probably, a bottleneck
  L1Fit fitter;
  fitter.SetParticleMass(fMass);
  fitter.SetMask(fmask::One());
  fitter.SetDoFitVelocity(true);
  fitter.SetTrack(trPar);
  L1FieldRegion fieldRegion;
  fieldRegion.SetUseOriginalField();  // Precised magnetic field is used
  fitter.Extrapolate(mcPoint.GetZOut(), fieldRegion);

  const L1TrackPar& trParExtr = fitter.Tr();  // Track parameters extrapolated to given MC point

  // ** Time-independent measurements **
  FillResAndPull(ETrackParType::kX, trParExtr.GetX(), trParExtr.GetXErr(), mcPoint.GetXOut(), weight);
  FillResAndPull(ETrackParType::kY, trParExtr.GetY(), trParExtr.GetYErr(), mcPoint.GetYOut(), weight);
  FillResAndPull(ETrackParType::kTX, trParExtr.GetTx(), trParExtr.GetTxErr(), mcPoint.GetTxOut(), weight);
  FillResAndPull(ETrackParType::kTY, trParExtr.GetTy(), trParExtr.GetTyErr(), mcPoint.GetTyOut(), weight);
  FillResAndPull(ETrackParType::kQP, trParExtr.GetQp(), trParExtr.GetQpErr(), mcPoint.GetQpOut(), weight);

  // TODO: in which point do we calculate MC parameters (central, entrance, exit)??
  double recoP    = std::fabs(mcPoint.GetCharge() / trParExtr.GetQp());  // reco mom. (with MC charge)
  double resP     = recoP - mcPoint.GetPOut();                           // residual of total momentum
  double resPhi   = trParExtr.GetPhi() - mcPoint.GetPhiOut();            // residual of azimuthal angle
  double resTheta = trParExtr.GetTheta() - mcPoint.GetThetaOut();        // residual of polar angle

  resPhi = std::atan2(std::sin(resPhi), std::cos(resPhi));  // overflow over (-pi, pi] protection

  fph_res_p_pMC->Fill(mcPoint.GetPOut(), resP);
  fph_res_phi_phiMC->Fill(mcPoint.GetPhiOut(), resPhi);
  fph_res_theta_thetaMC->Fill(mcPoint.GetThetaOut(), resTheta);

  // ** Time-dependent measurements **
  if (bTimeMeasured) {
    FillResAndPull(ETrackParType::kTIME, trParExtr.GetTime(), trParExtr.GetTimeErr(), mcPoint.GetTime(), weight);
    FillResAndPull(ETrackParType::kVI, trParExtr.GetInvSpeed(), trParExtr.GetInvSpeedErr(), mcPoint.GetInvSpeedOut(),
                   weight);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackFitQa::SetDefaultProperties()
{
  // ** Residual distribution parameters **
  fvRBins[ETrackParType::kX]    = 200;     ///< Number of bins, residual of x
  fvRLo[ETrackParType::kX]      = -4.e-3;  ///< Lower boundary, residual of x [cm]
  fvRUp[ETrackParType::kX]      = +4.e-3;  ///< Upper boundary, residual of x [cm]
  fvRBins[ETrackParType::kY]    = 200;     ///< Number of bins, residual of y
  fvRLo[ETrackParType::kY]      = -4.e-2;  ///< Lower boundary, residual of y [cm]
  fvRUp[ETrackParType::kY]      = +4.e-2;  ///< Upper boundary, residual of y [cm]
  fvRBins[ETrackParType::kTX]   = 200;     ///< Number of bins, residual of slope along x-axis
  fvRLo[ETrackParType::kTX]     = -4.e-3;  ///< Lower boundary, residual of slope along x-axis
  fvRUp[ETrackParType::kTX]     = +4.e-3;  ///< Upper boundary, residual of slope along x-axis
  fvRBins[ETrackParType::kTY]   = 200;     ///< Number of bins, residual of slope along y-axis
  fvRLo[ETrackParType::kTY]     = -4.e-3;  ///< Lower boundary, residual of slope along y-axis
  fvRUp[ETrackParType::kTY]     = +4.e-3;  ///< Upper boundary, residual of slope along y-axis
  fvRBins[ETrackParType::kQP]   = 200;     ///< Number of bins, residual of q/p
  fvRLo[ETrackParType::kQP]     = -1.;     ///< Lower boundary, residual of q/p [ec/GeV]
  fvRUp[ETrackParType::kQP]     = +1.;     ///< Upper boundary, residual of q/p [ec/GeV]
  fvRBins[ETrackParType::kTIME] = 200;     ///< Number of bins, residual of time
  fvRLo[ETrackParType::kTIME]   = -1.;     ///< Lower boundary, residual of time [ns]
  fvRUp[ETrackParType::kTIME]   = +1.;     ///< Upper boundary, residual of time [ns]
  fvRBins[ETrackParType::kVI]   = 200;     ///< Number of bins, residual of inverse speed
  fvRLo[ETrackParType::kVI]     = -2.;     ///< Lower boundary, residual of inverse speed [1/c]
  fvRUp[ETrackParType::kVI]     = +2.;     ///< Upper boundary, residual of inverse speed [1/c]

  // ** Pulls distribution parameters **
  fvPBins[ETrackParType::kX]    = 200;   ///< Number of bins, pull of x
  fvPLo[ETrackParType::kX]      = -4.;   ///< Lower boundary, pull of x [cm]
  fvPUp[ETrackParType::kX]      = +4.;   ///< Upper boundary, pull of x [cm]
  fvPBins[ETrackParType::kY]    = 200;   ///< Number of bins, pull of y
  fvPLo[ETrackParType::kY]      = -4.;   ///< Lower boundary, pull of y [cm]
  fvPUp[ETrackParType::kY]      = +4.;   ///< Upper boundary, pull of y [cm]
  fvPBins[ETrackParType::kTX]   = 200;   ///< Number of bins, pull of slope along x-axis
  fvPLo[ETrackParType::kTX]     = -4.;   ///< Lower boundary, pull of slope along x-axis
  fvPUp[ETrackParType::kTX]     = +4.;   ///< Upper boundary, pull of slope along x-axis
  fvPBins[ETrackParType::kTY]   = 200;   ///< Number of bins, pull of slope along y-axis
  fvPLo[ETrackParType::kTY]     = -4.;   ///< Lower boundary, pull of slope along y-axis
  fvPUp[ETrackParType::kTY]     = +4.;   ///< Upper boundary, pull of slope along y-axis
  fvPBins[ETrackParType::kQP]   = 200;   ///< Number of bins, pull of q/p
  fvPLo[ETrackParType::kQP]     = -10.;  ///< Lower boundary, pull of q/p [ec/GeV]
  fvPUp[ETrackParType::kQP]     = +10.;  ///< Upper boundary, pull of q/p [ec/GeV]
  fvPBins[ETrackParType::kTIME] = 200;   ///< Number of bins, pull of time
  fvPLo[ETrackParType::kTIME]   = -10.;  ///< Lower boundary, pull of time [ns]
  fvPUp[ETrackParType::kTIME]   = +10.;  ///< Upper boundary, pull of time [ns]
  fvPBins[ETrackParType::kVI]   = 200;   ///< Number of bins, pull of inverse speed
  fvPLo[ETrackParType::kVI]     = -2.;   ///< Lower boundary, pull of inverse speed [1/c]
  fvPUp[ETrackParType::kVI]     = +2.;   ///< Upper boundary, pull of inverse speed [1/c]
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackFitQa::SetResidualHistoProperties(ETrackParType type, int nBins, double lo, double up)
{
  fvRBins[type] = nBins;
  fvRLo[type]   = lo;
  fvRUp[type]   = up;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackFitQa::SetPullHistoProperties(ETrackParType type, int nBins, double lo, double up)
{
  fvPBins[type] = nBins;
  fvPLo[type]   = lo;
  fvPUp[type]   = up;
}
