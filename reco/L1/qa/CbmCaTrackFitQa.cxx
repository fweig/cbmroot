/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTrackFitQa.cxx
/// @brief  QA submodule for track fit results (residuals and puls) at selected z-coordinate (implementation)
/// @since  03.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaTrackFitQa.h"

#include "CbmL1Track.h"

#include "CaToolsMCData.h"
#include "L1Field.h"
#include "L1Fit.h"

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
void TrackFitQa::Init()
{
  fph_res_x  = MakeHisto<TH1F>("res_x", "", fBinsRESX, fLoRESX, fUpRESX);
  fph_res_y  = MakeHisto<TH1F>("res_y", "", fBinsRESY, fLoRESY, fUpRESY);
  fph_res_t  = MakeHisto<TH1F>("res_t", "", fBinsREST, fLoREST, fUpREST);
  fph_res_tx = MakeHisto<TH1F>("res_tx", "", fBinsRESTX, fLoRESTX, fUpRESTX);
  fph_res_ty = MakeHisto<TH1F>("res_ty", "", fBinsRESTY, fLoRESTY, fUpRESTY);
  fph_res_qp = MakeHisto<TH1F>("res_qp", "", fBinsRESQP, fLoRESQP, fUpRESQP);
  fph_res_vi = MakeHisto<TH1F>("res_vi", "", fBinsRESVI, fLoRESVI, fUpRESVI);

  fph_pull_x  = MakeHisto<TH1F>("pull_x", "", fBinsPULLX, fLoPULLX, fUpPULLX);
  fph_pull_y  = MakeHisto<TH1F>("pull_y", "", fBinsPULLY, fLoPULLY, fUpPULLY);
  fph_pull_t  = MakeHisto<TH1F>("pull_t", "", fBinsPULLT, fLoPULLT, fUpPULLT);
  fph_pull_tx = MakeHisto<TH1F>("pull_tx", "", fBinsPULLTX, fLoPULLTX, fUpPULLTX);
  fph_pull_ty = MakeHisto<TH1F>("pull_ty", "", fBinsPULLTY, fLoPULLTY, fUpPULLTY);
  fph_pull_qp = MakeHisto<TH1F>("pull_qp", "", fBinsPULLQP, fLoPULLQP, fUpPULLQP);
  fph_pull_vi = MakeHisto<TH1F>("pull_vi", "", fBinsPULLVI, fLoPULLVI, fUpPULLVI);

  // Set histogram titles
  TString sPrefix = (fsTitle.Length() > 0) ? fsTitle + " point " : "";
  fph_res_x->SetTitle(sPrefix + " residual for x-coordinate;x_{reco} - x_{MC} [cm]");
  fph_res_y->SetTitle(sPrefix + " residual for y-coordinate;y_{reco} - y_{MC} [cm]");
  fph_res_t->SetTitle(sPrefix + " residual for time;t_{reco} - t_{MC} [ns]");
  fph_res_tx->SetTitle(sPrefix + " residual for slope along x-axis;T_{x}^{reco} - T_{x}^{MC}");
  fph_res_ty->SetTitle(sPrefix + " residual for slope along y-axis;T_{y}^{reco} - T_{y}^{MC}");
  fph_res_qp->SetTitle(sPrefix + " residual for q/p;(q/p)_{reco} - (q/p)_{MC} [ec/GeV]");
  fph_res_vi->SetTitle(sPrefix + " residual for inverse speed;v^{-1}_{reco} - v^{-1}_{MC} [c^{-1}]");

  fph_pull_x->SetTitle(sPrefix + " pull for x-coordinate;(x_{reco} - x_{MC}) / #sigma_{x}");
  fph_pull_y->SetTitle(sPrefix + " pull for y-coordinate;(y_{reco} - y_{MC}) / #sigma_{y}");
  fph_pull_t->SetTitle(sPrefix + " pull for time;(t_{reco} - t_{MC}) / #sigma_{t}");
  fph_pull_tx->SetTitle(sPrefix + " pull for slope along x-axis;(T_{x}^{reco} - T_{x}^{MC}) / #sigma_{T_{x}}");
  fph_pull_ty->SetTitle(sPrefix + " pull for slope along y-axis;(T_{y}^{reco} - T_{y}^{MC}) / #sigma_{T_{y}}");
  fph_pull_qp->SetTitle(sPrefix + " pull for q/p;((q/p)_{reco} - (q/p)_{MC}) / #sigma_{q/p}");
  fph_pull_vi->SetTitle(sPrefix + " pull for inverse speed;(v^{-1}_{reco} - v^{-1}_{MC}) / #sigma_{v^{-1}}");
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

  double resX  = trParExtr.GetX() - mcPoint.GetXOut();    // residual of x-position [cm]
  double resY  = trParExtr.GetY() - mcPoint.GetYOut();    // residual of y-position [cm]
  double resTx = trParExtr.GetTx() - mcPoint.GetTxOut();  // residual of slope along x-axis [1]
  double resTy = trParExtr.GetTy() - mcPoint.GetTyOut();  // residual of slope along y-axis [1]
  double resQp = trParExtr.GetQp() - mcPoint.GetQpOut();  // residual of q/p [ec/GeV]

  double pullX  = resX / trParExtr.GetXErr();    // pull of x-position
  double pullY  = resY / trParExtr.GetYErr();    // pull of y-position
  double pullTx = resTx / trParExtr.GetTxErr();  // pull of slope along x-axis
  double pullTy = resTy / trParExtr.GetTyErr();  // pull of slope along y-axis
  double pullQp = resQp / trParExtr.GetQpErr();  // pull of q/p

  fph_res_x->Fill(resX, weight);
  fph_res_y->Fill(resY, weight);
  fph_res_tx->Fill(resTx, weight);
  fph_res_ty->Fill(resTy, weight);
  fph_res_qp->Fill(resQp, weight);

  fph_pull_x->Fill(pullX, weight);
  fph_pull_y->Fill(pullY, weight);
  fph_pull_tx->Fill(pullTx, weight);
  fph_pull_ty->Fill(pullTy, weight);
  fph_pull_qp->Fill(pullQp, weight);


  // ** Time-dependent measurements **
  if (bTimeMeasured) {
    double resT   = trParExtr.GetTime() - mcPoint.GetTime();  // residual of time [ns]
    double resVi  = (trParExtr.GetInvSpeed() - mcPoint.GetInvSpeedOut()) * L1Constants::phys::kSpeedOfLightInv;
    double pullT  = resT / trParExtr.GetTimeErr();       // pull of time
    double pullVi = resVi / trParExtr.GetInvSpeedErr();  // pull of inverse speed

    fph_res_t->Fill(resT, weight);
    fph_res_vi->Fill(resVi, weight);
    fph_pull_t->Fill(pullT, weight);
    fph_pull_vi->Fill(pullVi, weight);
  }
}
