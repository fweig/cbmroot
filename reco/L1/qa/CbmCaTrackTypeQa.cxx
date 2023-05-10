/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTrackTypeQa.h
/// @brief  QA submodule for different track types (header)
/// @since  27.03.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaTrackTypeQa.h"

#include "CbmCaTrackFitQa.h"
#include "CbmL1Track.h"

#include "CaToolsMCData.h"

using ca::tools::MCPoint;
using ca::tools::MCTrack;
using cbm::ca::TrackTypeQa;


// ---------------------------------------------------------------------------------------------------------------------
//
TrackTypeQa::TrackTypeQa(const char* typeName, const char* prefixName, bool bUseMC, TFolder* pParentFolder)
  : CbmQaIO(typeName, Form("%s_%s", prefixName, typeName), pParentFolder)
  , fbUseMC(bUseMC)
{
  fStoringMode = EStoringMode::kSAMEDIR;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::Init()
{
  // TODO: Replace assertions with exceptions
  assert(fpvRecoTracks);
  assert(fpvHits);
  assert(!fbUseMC || fpMCData);

  //
  // ** Distributions of reconstructed tracks vs. reconstructed quantities **
  //
  fph_reco_p     = MakeHisto<TH1F>("reco_p", "", kBinsP, kLoP, kUpP);
  fph_reco_pt    = MakeHisto<TH1F>("reco_pt", "", kBinsPT, kLoPT, kUpPT);
  fph_reco_eta   = MakeHisto<TH1F>("reco_eta", "", kBinsETA, kLoETA, kUpETA);
  fph_reco_phi   = MakeHisto<TH1F>("reco_phi", "", kBinsPHI, kLoPHI, kUpPHI);
  fph_reco_theta = MakeHisto<TH1F>("reco_theta", "", kBinsTHETA, kLoTHETA, kUpTHETA);
  fph_reco_theta_phi = MakeHisto<TH2F>("reco_theta_phi", "", kBinsPHI, kLoPHI, kUpPHI, kBinsTHETA, kLoTHETA, kUpTHETA);
  fph_reco_tx    = MakeHisto<TH1F>("reco_tx", "", kBinsTX, kLoTX, kUpTX);
  fph_reco_ty    = MakeHisto<TH1F>("reco_ty", "", kBinsTY, kLoTY, kUpTY);
  fph_reco_ty_tx     = MakeHisto<TH2F>("reco_ty_tx", "", kBinsTX, kLoTX, kUpTX, kBinsTY, kLoTY, kUpTY);
  fph_reco_fhitR = MakeHisto<TH1F>("reco_fhitR", "", kBinsFHITR, kLoFHITR, kUpFHITR);
  // TODO: ...

  fph_reco_p->SetTitle("Total momentum of reconstructed track;p^{reco} [GeV/c];Counts");
  fph_reco_pt->SetTitle("Transverse momentum of reconstructed track;p_{T}^{reco} [GeV/c];Counts");
  fph_reco_phi->SetTitle("Azimuthal angle of reconstructed track;#phi^{reco} [rad];Counts");
  fph_reco_eta->SetTitle("Pseudorapidity of reconstructed track;#eta^{reco};Counts");
  fph_reco_theta->SetTitle("Polar angle of reconstructed track;#theta^{reco} [rad];Counts");
  fph_reco_theta_phi->SetTitle(
    "Polar angle vs. azimuthal angle of reconstructed track;#phi^{reco} [rad];#theta^{reco} [rad];Counts");
  fph_reco_tx->SetTitle("Slope along x-axis of reconstructed tracks;t_{x}^{reco};Counts");
  fph_reco_ty->SetTitle("Slope along y-axis of reconstructed tracks;t_{y}^{reco};Counts");
  fph_reco_ty_tx->SetTitle("Slope along y-axis vs. x-axis of reconstructed tracks;t_{x}^{reco};t_{y}^{reco};");
  fph_reco_fhitR->SetTitle("Distance of the first hit from z-axis for reconstructed tracks;R^{reco} [cm];Counts");

  if (fbUseMC) {
    //
    // ** Distributions of reconstructed tracks vs. MC quantities **
    //
    fph_reco_pMC     = MakeHisto<TH1F>("reco_pMC", "", kBinsP, kLoP, kUpP);
    fph_reco_etaMC   = MakeHisto<TH1F>("reco_etaMC", "", kBinsETA, kLoETA, kUpETA);
    fph_reco_yMC     = MakeHisto<TH1F>("reco_yMC", "", kBinsY, kLoY, kUpY);
    fph_reco_pMC_yMC = MakeHisto<TH2F>("reco_pMC_yMC", "", kBinsY, kLoY, kUpY, kBinsP, kLoP, kUpP);
    fph_reco_phiMC   = MakeHisto<TH1F>("reco_phiMC", "", kBinsPHI, kLoPHI, kUpPHI);
    fph_reco_thetaMC = MakeHisto<TH1F>("reco_thetaMC", "", kBinsTHETA, kLoTHETA, kUpTHETA);
    fph_reco_nhits   = MakeHisto<TH1F>("reco_nhits", "", kBinsNHITS, kLoNHITS, kUpNHITS);

    fph_reco_pMC->SetTitle("MC total momentum of reconstructed track;p^{MC} [GeV/c];Counts");
    fph_reco_yMC->SetTitle("MC rapidity of reconstructed track;y^{MC};Counts");
    fph_reco_etaMC->SetTitle("MC pseudorapidity of reconstructed track;#eta^{MC};Counts");
    fph_reco_pMC_yMC->SetTitle("Transverse momentum of reconstructed track;y^{MC};p_{T}^{MC} [GeV/c];Counts");
    fph_reco_phiMC->SetTitle("Azimuthal angle of reconstructed track;#phi^{MC} [rad];Counts");
    fph_reco_thetaMC->SetTitle("Polar angle of reconstructed track;#theta^{MC} [rad];Counts");
    fph_reco_nhits->SetTitle("Hit number of reconstructed tracks;N^{MC}_{hits};Counts");


    fph_mc_pMC       = MakeHisto<TH1F>("mc_pMC", "", kBinsP, kLoP, kUpP);
    fph_mc_etaMC     = MakeHisto<TH1F>("mc_etaMC", "", kBinsETA, kLoETA, kUpETA);
    fph_mc_yMC       = MakeHisto<TH1F>("mc_yMC", "", kBinsY, kLoY, kUpY);
    fph_mc_pMC_yMC   = MakeHisto<TH2F>("mc_pMC_yMC", "", kBinsY, kLoY, kUpY, kBinsP, kLoP, kUpP);
    fph_mc_txMC      = MakeHisto<TH1F>("mc_txMC", "", kBinsTX, kLoTX, kUpTX);
    fph_mc_tyMC      = MakeHisto<TH1F>("mc_tyMC", "", kBinsTY, kLoTY, kUpTY);
    fph_mc_tyMC_txMC = MakeHisto<TH2F>("mc_tyMC_txMC", "", kBinsTX, kLoTX, kUpTX, kBinsTY, kLoTY, kUpTY);
    fph_mc_thetaMC_phiMC =
      MakeHisto<TH2F>("mc_thetaMC_phiMC", "", kBinsPHI, kLoPHI, kUpPHI, kBinsTHETA, kLoTHETA, kUpTHETA);

    fph_mc_pMC->SetTitle("MC total momentum of MC tracks;p^{MC} [GeV/c];Counts");
    fph_mc_yMC->SetTitle("MC rapidity of MC tracks;y^{MC};Counts");
    fph_mc_etaMC->SetTitle("MC pseudorapidity of MC track;#eta^{MC};Counts");
    fph_mc_pMC_yMC->SetTitle("MC total momentum vs. MC rapidity of MC tracks;y^{MC};p^{MC} [GeV/c];Counts");
    fph_mc_txMC->SetTitle("Slope along x-axis of MC tracks;t_{x}^{MC};Counts");
    fph_mc_tyMC->SetTitle("Slope along y-axis of MC tracks;t_{y}^{MC};Counts");
    fph_mc_tyMC_txMC->SetTitle("Slope along y-axis vs. x-axis of MC tracks;t_{x}^{MC};t_{y}^{MC};");
    fph_mc_thetaMC_phiMC->SetTitle("Polar angle vs. azimuthal angle of MC track;#phi^{MC} [rad];#theta^{MC} [rad]");

    //
    // ** Efficiencies **
    //
    fph_eff_int     = MakeHisto<TProfile>("eff_int", "", 1, -0.5, 0.5, 0., 1.);
    fph_eff_pMC     = MakeHisto<TProfile>("eff_pMC", "", kBinsP, kLoP, kUpP, 0., 1.);
    fph_eff_yMC     = MakeHisto<TProfile>("eff_yMC", "", kBinsY, kLoY, kUpY, 0., 1.);
    fph_eff_ptMC    = MakeHisto<TProfile>("eff_ptMC", "", kBinsPT, kLoPT, kUpPT, 0., 1.);
    fph_eff_thetaMC = MakeHisto<TProfile>("eff_thetaMC", "", kBinsTHETA, kLoTHETA, kUpTHETA, 0., 1.);
    fph_eff_etaMC   = MakeHisto<TProfile>("eff_etaMC", "", kBinsTHETA, kLoTHETA, kUpTHETA, 0., 1.);
    fph_eff_phiMC   = MakeHisto<TProfile>("eff_phiMC", "", kBinsPHI, kLoPHI, kUpPHI, 0., 1.);
    fph_eff_nhitsMC = MakeHisto<TProfile>("eff_nhitsMC", "", kBinsNHITS, kLoNHITS, kUpNHITS, 0., 1.);
    fph_eff_txMC    = MakeHisto<TProfile>("eff_txMC", "", kBinsTX, kLoTX, kUpTX, 0., 1.);
    fph_eff_tyMC    = MakeHisto<TProfile>("eff_tyMC", "", kBinsTY, kLoTY, kUpTX, 0., 1.);

    fph_eff_int->SetTitle("Integrated efficiency;;#epsilon_{CA}");
    fph_eff_pMC->SetTitle("Efficiency vs. MC total momentum;p_{MC} [GeV/c];#epsilon_{CA}");
    fph_eff_yMC->SetTitle("Efficiency vs. MC rapidity;y_{MC};#epsilon");
    fph_eff_ptMC->SetTitle("Efficiency vs. MC total momentum;p_{T}^{MC} [GeV/c];#epsilon_{CA}");
    fph_eff_thetaMC->SetTitle("Efficiency vs. MC polar angle;#theta^{MC};#epsilon_{CA}");
    fph_eff_etaMC->SetTitle("Efficiency vs. MC pseudorapidity;#eta^{MC};#epsilon_{CA}");
    fph_eff_phiMC->SetTitle("Efficiency vs. MC azimuthal angle;#phi^{MC};#epsilon_{CA}");
    fph_eff_nhitsMC->SetTitle("Efficiency vs. MC number of stations with hits;N_{hit}^{MC};#epsilon_{CA}");
    fph_eff_txMC->SetTitle("Efficiency vs. MC slope along x-axis;t_{x}^{MC};#epsilon_{CA}");
    fph_eff_tyMC->SetTitle("Efficiency vs. MC slope along y-axis;t_{y}^{MC};#epsilon_{CA}");

    fph_eff_pMC_yMC = MakeHisto<TProfile2D>("eff_pMC_yMC", "", kBinsY, kLoY, kUpY, kBinsP, kLoP, kUpP, 0., 1.);
    fph_eff_thetaMC_phiMC =
      MakeHisto<TProfile2D>("eff_theta_phi", "", kBinsPHI, kLoPHI, kUpPHI, kBinsTHETA, kLoTHETA, kUpTHETA, 0., 1.);

    fph_eff_pMC_yMC->SetTitle("Efficiency vs. MC total momentum and MC rapidity;y^{MC};#epsilon_{CA};p^{MC} [GeV/c]");
    fph_eff_thetaMC_phiMC->SetTitle(
      "Efficiency vs. MC polar and azimuthal angles;#phi^{MC} [rad];#theta^{MC} [rad];#epsilon_{CA}");
    //
    // ** Track fit parameter properties (residuals and pulls) **
    //
    fpFitQaFirstHit = std::make_unique<TrackFitQa>("fst_hit", fsPrefix, fpFolderRoot);
    fpFitQaFirstHit->SetTitle("First hit");
    fpFitQaFirstHit->Init();

    fpFitQaLastHit = std::make_unique<TrackFitQa>("lst_hit", fsPrefix, fpFolderRoot);
    fpFitQaLastHit->SetTitle("Last hit");
    fpFitQaLastHit->SetResidualHistoProperties(ETrackParType::kX, 200, -0.4, +0.4);
    fpFitQaLastHit->SetResidualHistoProperties(ETrackParType::kY, 200, -0.8, +0.8);
    fpFitQaLastHit->Init();

    fpFitQaVertex = std::make_unique<TrackFitQa>("vertex", fsPrefix, fpFolderRoot);
    fpFitQaVertex->SetTitle("Vertex");
    fpFitQaVertex->Init();
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::FillRecoTrack(int iTrkReco, double weight)
{
  const auto& recoTrack = (*fpvRecoTracks)[iTrkReco];
  fph_reco_p->Fill(recoTrack.GetP(), weight);
  fph_reco_pt->Fill(recoTrack.GetPt(), weight);
  fph_reco_eta->Fill(recoTrack.GetEta(), weight);
  fph_reco_phi->Fill(recoTrack.GetPhi(), weight);
  fph_reco_theta->Fill(recoTrack.GetTheta(), weight);
  fph_reco_tx->Fill(recoTrack.GetTx(), weight);
  fph_reco_ty->Fill(recoTrack.GetTy(), weight);
  fph_reco_ty_tx->Fill(recoTrack.GetTx(), recoTrack.GetTy(), weight);
  fph_reco_theta_phi->Fill(recoTrack.GetPhi(), recoTrack.GetTheta(), weight);
  if (fbUseMC) {
    int iTrkMC = recoTrack.GetMatchedMCTrackIndex();
    if (iTrkMC > -1) {
      /// TODO: fill mass hypothesis into CbmL1Track
      const auto& mcTrack = fpMCData->GetTrack(iTrkMC);
      fph_reco_pMC->Fill(mcTrack.GetP(), weight);
      fph_reco_etaMC->Fill(mcTrack.GetEta(), weight);
      fph_reco_yMC->Fill(mcTrack.GetRapidity(), weight);
      fph_reco_pMC_yMC->Fill(mcTrack.GetRapidity(), mcTrack.GetP(), weight);

      // *****************************
      // ** Track fit parameters QA **
      // *****************************

      int nTimeMeasurements = 0;
      for (int iH : recoTrack.GetHitIndexes()) {
        int iSt = (*fpvHits)[iH].GetStationId();
        if (fpParameters->GetStation(iSt).timeInfo) { ++nTimeMeasurements; }
      }
      bool isTimeFitted = (nTimeMeasurements > 1);

      // ** First hit **
      int iHfst = recoTrack.GetFirstHitIndex();
      int iPfst = (*fpvHits)[iHfst].GetMCPointIndex();
      if (iPfst > -1) {
        const auto& mcPoint = fpMCData->GetPoint(iPfst);
        L1TrackPar trPar(recoTrack.T, recoTrack.C);
        fpFitQaFirstHit->Fill(trPar, mcPoint, isTimeFitted, weight);
      }

      // ** Last hit **
      int iHlst = recoTrack.GetLastHitIndex();
      int iPlst = (*fpvHits)[iHlst].GetMCPointIndex();
      if (iPlst > -1) {
        const auto& mcPoint = fpMCData->GetPoint(iPlst);
        L1TrackPar trPar(recoTrack.TLast, recoTrack.CLast);
        fpFitQaLastHit->Fill(trPar, mcPoint, isTimeFitted, weight);
      }
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::FillRecoTracks()
{
  for (int iTrkReco = 0; iTrkReco < (int) fpvRecoTracks->size(); ++iTrkReco) {
    const auto& recoTrk = (*fpvRecoTracks)[iTrkReco];

    // Reject tracks, which do not contain hits
    if (recoTrk.GetNofHits() < 1) { continue; }

    // Reject tracks, which do not pass the applied cuts
    if (!fRecoTrackCut(recoTrk)) { continue; }

    // Reject tracks, which have MC tracks not passing the applied cuts
    if (fbUseMC) {
      int iTrkMC = recoTrk.GetMatchedMCTrackIndex();
      if (iTrkMC > -1) {
        const auto& mcTrk = fpMCData->GetTrack(iTrkMC);
        if (!fMCTrackCut(mcTrk)) { continue; }
      }
    }

    // Fill histograms
    this->FillRecoTrack(iTrkReco);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::FillMCTrack(int iTrkMC, double weight)
{
  assert(fbUseMC);

  const MCTrack& mcTrack = fpMCData->GetTrack(iTrkMC);

  // Only reconstructable tracks are considered (?)
  if (!mcTrack.IsReconstructable()) { return; }

  // ** Distributions **
  fph_mc_pMC->Fill(mcTrack.GetP(), weight);
  fph_mc_etaMC->Fill(mcTrack.GetEta(), weight);
  fph_mc_yMC->Fill(mcTrack.GetRapidity(), weight);
  fph_mc_pMC_yMC->Fill(mcTrack.GetRapidity(), mcTrack.GetP(), weight);
  fph_mc_txMC->Fill(mcTrack.GetTx(), weight);
  fph_mc_tyMC->Fill(mcTrack.GetTy(), weight);
  fph_mc_tyMC_txMC->Fill(mcTrack.GetTx(), mcTrack.GetTy(), weight);
  fph_mc_thetaMC_phiMC->Fill(mcTrack.GetPhi(), mcTrack.GetTheta(), weight);

  // ** Efficiencies **
  bool bReco = mcTrack.IsReconstructed();

  // NOTE: Weight is ignored in efficiencies
  fph_eff_int->Fill(0., bReco);
  fph_eff_pMC->Fill(mcTrack.GetP(), bReco);
  fph_eff_etaMC->Fill(mcTrack.GetEta(), bReco);
  fph_eff_yMC->Fill(mcTrack.GetRapidity(), bReco);
  fph_eff_ptMC->Fill(mcTrack.GetPt(), bReco);
  fph_eff_thetaMC->Fill(mcTrack.GetTheta(), bReco);
  fph_eff_phiMC->Fill(mcTrack.GetPhi(), bReco);
  fph_eff_nhitsMC->Fill(mcTrack.GetTotNofStationsWithHit(), bReco);
  fph_eff_txMC->Fill(mcTrack.GetTx(), bReco);
  fph_eff_tyMC->Fill(mcTrack.GetTy(), bReco);

  fph_eff_pMC_yMC->Fill(mcTrack.GetRapidity(), mcTrack.GetP(), bReco);
  fph_eff_thetaMC_phiMC->Fill(mcTrack.GetPhi(), mcTrack.GetTheta(), bReco);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::FillMCTracks()
{
  assert(fbUseMC);
  for (int iTrkMC = 0; iTrkMC < fpMCData->GetNofTracks(); ++iTrkMC) {
    const auto& mcTrk = fpMCData->GetTrack(iTrkMC);

    // Cut tracks, which did not leave hits in tracker
    //if (mcTrk.GetNofHits() == 0) { continue; }
    if (!mcTrk.IsReconstructable()) { continue; }

    // Cut tracks, which do not pass the applied cut
    if (!fMCTrackCut(mcTrk)) { continue; }

    // TODO: Investigate, how to apply reconstructed track cuts here
    // NOTE: Reconstructed track cut analogs must be applied to mc tracks

    // Fill histograms
    this->FillMCTrack(iTrkMC);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::SetDrawAtt(Color_t markerCol, Style_t markerSty, Color_t lineCol, Style_t lineSty)
{
  fMarkerColor = markerCol;
  fMarkerStyle = markerSty;
  fLineColor   = (lineCol > -1) ? lineCol : markerCol;
  fLineStyle   = lineSty;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TrackTypeQa::SetHistoProperties(TH1* pHist)
{
  pHist->SetStats(true);
  pHist->Sumw2();
  if (!pHist->InheritsFrom("TH2") && !pHist->InheritsFrom("TH3")) {
    pHist->SetMarkerStyle(fMarkerStyle);
    pHist->SetLineStyle(fLineStyle);
  }
  pHist->SetMarkerColor(fMarkerColor);
  pHist->SetLineColor(fLineColor);
}
