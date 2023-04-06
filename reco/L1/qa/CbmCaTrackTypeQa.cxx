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

  fph_reco_p     = MakeHisto<TH1F>("reco_p", "", kBinsP, kLoP, kUpP);
  fph_reco_pt    = MakeHisto<TH1F>("reco_pt", "", kBinsPT, kLoPT, kUpPT);
  fph_reco_eta   = MakeHisto<TH1F>("reco_eta", "", kBinsETA, kLoETA, kUpETA);
  fph_reco_phi   = MakeHisto<TH1F>("reco_phi", "", kBinsPHI, kLoPHI, kUpPHI);
  fph_reco_theta = MakeHisto<TH1F>("reco_theta", "", kBinsTHETA, kLoTHETA, kUpTHETA);
  fph_reco_tx    = MakeHisto<TH1F>("reco_tx", "", kBinsTX, kLoTX, kUpTX);
  fph_reco_ty    = MakeHisto<TH1F>("reco_ty", "", kBinsTY, kLoTY, kUpTY);
  fph_reco_fhitR = MakeHisto<TH1F>("reco_fhitR", "", kBinsFHITR, kLoFHITR, kUpFHITR);
  fph_reco_nhits = MakeHisto<TH1F>("reco_nhits", "", kBinsNHITS, kLoNHITS, kUpNHITS);
  // TODO: ...

  fph_reco_p->SetTitle("Total momentum of reconstructed track;p_{reco} [GeV/c]");
  fph_reco_pt->SetTitle("Transverse momentum of reconstructed track;p_{T}_{reco} [GeV/c]");
  fph_reco_phi->SetTitle("Azimuthal angle of reconstructed track;#phi_{reco} [rad]");
  fph_reco_theta->SetTitle("Polar angle of reconstructed track;#theta_{reco} [rad]");
  fph_reco_tx->SetTitle("Slope along x-axis of reconstructed tracks;t_{x}");
  fph_reco_ty->SetTitle("Slope along y-axis of reconstructed tracks;t_{y}");
  fph_reco_fhitR->SetTitle("Distance of the first hit from z-axis for reconstructed tracks");
  fph_reco_nhits->SetTitle("Hit number of reconstructed tracks");

  if (fbUseMC) {
    fph_reco_pMC     = MakeHisto<TH1F>("reco_pMC", "", kBinsP, kLoP, kUpP);
    fph_reco_yMC     = MakeHisto<TH1F>("reco_yMC", "", kBinsY, kLoY, kUpY);
    fph_reco_pMC_yMC = MakeHisto<TH2F>("reco_pMC_yMC", "", kBinsY, kLoY, kUpY, kBinsP, kLoP, kUpP);
    fph_reco_phiMC   = MakeHisto<TH1F>("reco_phiMC", "", kBinsPHI, kLoPHI, kUpPHI);
    fph_reco_thetaMC = MakeHisto<TH1F>("reco_thetaMC", "", kBinsTHETA, kLoTHETA, kUpTHETA);

    fph_reco_pMC->SetTitle("MC total momentum of reconstructed track;p_{MC} [GeV/c]");
    fph_reco_yMC->SetTitle("MC rapidity of reconstructed track;y_{MC}");
    fph_reco_pMC_yMC->SetTitle("Transverse momentum of reconstructed track;y_{MC};p_{T}_{MC} [GeV/c]");
    fph_reco_phiMC->SetTitle("Azimuthal angle of reconstructed track;#phi_{reco} [rad]");
    fph_reco_thetaMC->SetTitle("Polar angle of reconstructed track;#theta_{reco} [rad]");

    fph_mc_pMC     = MakeHisto<TH1F>("mc_pMC", "", kBinsP, kLoP, kUpP);
    fph_mc_yMC     = MakeHisto<TH1F>("mc_yMC", "", kBinsY, kLoY, kUpY);
    fph_mc_pMC_yMC = MakeHisto<TH2F>("mc_pMC_yMC", "", kBinsY, kLoY, kUpY, kBinsP, kLoP, kUpP);

    fph_mc_pMC->SetTitle("MC total momentum of MC tracks;p_{MC} [GeV/c]");
    fph_mc_yMC->SetTitle("MC rapidity of MC tracks;y_{MC}");
    fph_mc_pMC_yMC->SetTitle("MC total momentum vs. MC rapidity of MC tracks;y_{MC};p_{MC} [GeV/c]");

    fpFitQaFirstHit = std::make_unique<TrackFitQa>("fst_hit", fsPrefix, fpFolderRoot);
    fpFitQaFirstHit->SetTitle("First hit");
    fpFitQaFirstHit->Init();

    fpFitQaLastHit = std::make_unique<TrackFitQa>("lst_hit", fsPrefix, fpFolderRoot);
    fpFitQaLastHit->SetTitle("Last hit");
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
  fph_reco_phi->Fill(recoTrack.GetPhi(), weight);
  fph_reco_theta->Fill(recoTrack.GetTheta(), weight);
  fph_reco_tx->Fill(recoTrack.GetTx(), weight);
  fph_reco_ty->Fill(recoTrack.GetTy(), weight);
  if (fbUseMC) {
    int iTrkMC = recoTrack.GetMatchedMCTrackIndex();
    if (iTrkMC > -1) {
      /// TODO: fill mass hypothesis into CbmL1Track
      const auto& mcTrack = fpMCData->GetTrack(iTrkMC);
      fph_reco_pMC->Fill(mcTrack.GetP(), weight);
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
void TrackTypeQa::FillMCTrack(int iTrkMC, double weight)
{
  assert(fbUseMC);

  const MCTrack& mcTrack = fpMCData->GetTrack(iTrkMC);
  fph_mc_pMC->Fill(mcTrack.GetP(), weight);
  fph_mc_yMC->Fill(mcTrack.GetRapidity(), weight);
  fph_mc_pMC_yMC->Fill(mcTrack.GetRapidity(), mcTrack.GetP(), weight);
}
