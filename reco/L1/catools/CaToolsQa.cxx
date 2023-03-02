/* Copyright (C) 2022-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsQa.h
/// \brief  Tracking performance class (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsQa.h"

#include "CbmL1Hit.h"    // TODO: SZh: move CbmL1Hit to ca::tools::Hit
#include "CbmL1Track.h"  // TODO: SZh: move CbmL1Track to ca::tools::RecoTrack

#include "FairRunAna.h"

#include <utility>  // for std::move

#include "CaToolsMCData.h"
#include "L1Parameters.h"

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
void Qa::InitHistograms()
{
  LOG_IF(fatal, !fpOutFile) << "CA QA: Output file was not defined";

  TDirectory* currentDir = gDirectory;
  gDirectory             = fpHistoDir;

  // ----- Initialize histograms and profiles --------------------------------------------------------------------------
  // Distributions of reconstructed tracks by different quantities
  fph_reco_p        = MakeHisto<TH1F>("ca_reco_p", "", kNbinsRecoP, 0., kAxMaxRecoP);
  fph_reco_pt       = MakeHisto<TH1F>("ca_reco_pt", "", kNbinsRecoPt, 0., kAxMaxRecoPt);
  fph_reco_phi      = MakeHisto<TH1F>("ca_reco_phi", "", kNbinsRecoPhi, -kAxMaxRecoPhi, kAxMaxRecoPhi);
  fph_reco_nhits    = MakeHisto<TH1F>("ca_reco_nhits", "", kNbinsNofHits, -0.5, kNbinsNofHits - 0.5);
  fph_reco_fsta     = MakeHisto<TH1F>("ca_reco_fsta", "", kNbinsNofSta, -0.5, kNbinsNofSta - 0.5);
  fph_reco_purity   = MakeHisto<TH1F>("ca_reco_purity", "", kNbinsPurity, -0.5, 100.5);
  fph_reco_chi2_ndf = MakeHisto<TH1F>("ca_reco_chi2_ndf", "", kNbinsChi2NDF, -0.5, kAxMaxChi2NDF);
  fph_reco_prob     = MakeHisto<TH1F>("ca_reco_prob", "", kNbinsProb, 0., 1.01);
  fph_rest_chi2_ndf = MakeHisto<TH1F>("ca_rest_chi2_ndf", "", kNbinsChi2NDF, -0.5, kAxMaxChi2NDF);
  fph_rest_prob     = MakeHisto<TH1F>("ca_rest_prob", "", kNbinsProb, 0., 1.01);

  fph_reco_p->SetTitle("Momentum of reconstructed track;(p/q)_{reco} [GeV/ec]");
  fph_reco_pt->SetTitle("Transverse momentum of reconstructed track;(p_{T}/q)_{reco} [GeV/ec]");
  fph_reco_phi->SetTitle("Azimuthal angle of reconstructed track;#phi_{reco} [rad]");
  fph_reco_nhits->SetTitle("Number of hits in a reconstructed track;N_{hits}");
  fph_reco_fsta->SetTitle("First station of reconstructed track;ID_{f.st.}");
  fph_reco_purity->SetTitle("Purity of reconstructed track;purity [%]");
  fph_reco_chi2_ndf->SetTitle("Fit #chi^{2}/NDF of reconstructed track;(#chi^{2}/NDF)_{reco}");
  fph_reco_chi2_ndf->SetTitle("Fit #chi^{2}/NDF of the rest track;(#chi^{2}/NDF)_{reco}");
  fph_reco_prob->SetTitle("Fit probability of reconstructed track;Prob_{reco}");
  fph_rest_prob->SetTitle("Fit probability of the rest tracks;Prob_{reco}");


  // Distributions of reconstructed tracks by different quantities
  fph_ghost_p        = MakeHisto<TH1F>("ca_ghost_p", "", kNbinsRecoP, 0., kAxMaxRecoP);
  fph_ghost_pt       = MakeHisto<TH1F>("ca_ghost_pt", "", kNbinsRecoPt, 0., kAxMaxRecoPt);
  fph_ghost_phi      = MakeHisto<TH1F>("ca_ghost_phi", "", kNbinsRecoPhi, -kAxMaxRecoPhi, kAxMaxRecoPhi);
  fph_ghost_nhits    = MakeHisto<TH1F>("ca_ghost_nhits", "", kNbinsNofHits, -0.5, kNbinsNofHits - 0.5);
  fph_ghost_fsta     = MakeHisto<TH1F>("ca_ghost_fsta", "", kNbinsNofSta, -0.5, kNbinsNofSta - 0.5);
  fph_ghost_purity   = MakeHisto<TH1F>("ca_ghost_purity", "", kNbinsPurity, -0.5, 100.5);
  fph_ghost_chi2_ndf = MakeHisto<TH1F>("ca_ghost_chi2_ndf", "", kNbinsChi2NDF, 0., kAxMaxChi2NDF);
  fph_ghost_prob     = MakeHisto<TH1F>("ca_ghost_prob", "", kNbinsProb, 0., 1.01);
  fph_ghost_tx       = MakeHisto<TH1F>("ca_ghost_tx", "", kNbinsTx, -2., 2.);
  fph_ghost_ty       = MakeHisto<TH1F>("ca_ghost_ty", "", kNbinsTy, -2., 2.);
  fph_ghost_fhitR    = MakeHisto<TH1F>("ca_ghost_fhitR", "", kNbinsHitDist, 0., 150.);
  fph_ghost_nhits_vs_p =
    MakeHisto<TH2F>("ca_ghost_nhits_vs_p", "", kNbinsRecoP, 0., kAxMaxRecoP, kNbinsNofHits, -0.5, kNbinsNofHits - 0.5);
  fph_ghost_fsta_vs_p =
    MakeHisto<TH2F>("ca_ghost_fsta_vs_p", "", kNbinsRecoP, 0., kAxMaxRecoP, kNbinsNofSta, -0.5, kNbinsNofSta - 0.5);
  fph_ghost_lsta_vs_fsta = MakeHisto<TH2F>("ca_ghost_lsta_vs_fsta", "", kNbinsNofSta, -0.5, kNbinsNofSta - 0.5,
                                           kNbinsNofSta, -0.5, kNbinsNofSta - 0.5);

  fph_ghost_p->SetTitle("Momentum of ghost track;(p/q)_{reco} [GeV/ec]");
  fph_ghost_pt->SetTitle("Transverse momentum of ghost track;(p_{T}/q)_{reco} [GeV/ec]");
  fph_ghost_phi->SetTitle("Azimuthal angle of ghost track;#phi_{reco} [rad]");
  fph_ghost_nhits->SetTitle("Number of hits in a ghost track;N_{hits}");
  fph_ghost_fsta->SetTitle("First station of ghost track;ID_{f.st.}");
  fph_ghost_purity->SetTitle("Purity of ghost track;purity [%]");
  fph_ghost_chi2_ndf->SetTitle("Fit #chi^{2}/NDF of ghost track;(#chi^{2}/NDF)_{reco}");
  fph_ghost_prob->SetTitle("Fit probability of ghost track;Prob_{reco}");
  fph_ghost_tx->SetTitle("Slope along x-axis of ghost tracks;t_{x}");
  fph_ghost_ty->SetTitle("Slope along y-axis of ghost tracks;t_{y}");
  fph_ghost_fhitR->SetTitle("Distance of the first hit from z-axis;R [cm]");
  fph_ghost_nhits_vs_p->SetTitle("Number of hits vs. momentum for ghost track;(p/q)_{reco} [GeV/ec];N_{hits}");
  fph_ghost_fsta_vs_p->SetTitle("First station vs. momentum for ghost track;(p/q)_{reco} [GeV/ec];ID_{f.st.}");
  fph_ghost_lsta_vs_fsta->SetTitle("Last station vs. first station for ghost track;ID_{first st.};ID_{last st.}");

  // ----- Reset event counter
  fNofEvents = 0;

  LOG(info) << "CA QA: Initialized histograms: (directory \033[1;31m" << gDirectory->GetName() << "\033[0m)";
  for (auto& entry : fmpHistoList) {
    LOG(info) << "\t- " << entry.first;
  }

  gDirectory = currentDir;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Qa::FillHistograms()
{
  assert(fpParameters);
  assert(fpMCData);
  assert(fpvRecoTracks);
  assert(fpvHits);

  // Update event counter
  fNofEvents++;


  // ************************************************************************
  // ** Fill distributions for reconstructed tracks (including ghost ones) **
  // ************************************************************************

  //for (const auto& recoTrk : *fpvRecoTracks) {
  //  // Reject tracks, which do not contain hits
  //  if (recoTrk.GetNofHits() < 1) { continue; }

  //  const auto& hitFst = (*fpvHits)[recoTrk.GetHitIndexes()[0]];                         // first hit of track
  //  const auto& hitSnd = (*fpvHits)[recoTrk.GetHitIndexes()[1]];                         // second hit of track
  //  const auto& hitLst = (*fpvHits)[recoTrk.GetHitIndexes()[recoTrk.GetNofHits() - 1]];  // last hit of track

  //  fph_reco_p->Fill(recoTrk.GetP());
  //  fph_reco_pt->Fill(recoTrk.GetPt());
  //  fph_reco_phi->Fill(recoTrk.GetPhi());
  //  fph_reco_nhits->Fill(recoTrk.GetNofHits());
  //  //fph_reco_fsta->Fill(hitFst.GetStationId());
  //  fph_reco_purity->Fill(100 * recoTrk.GetMaxPurity());

  //  if (recoTrk.GetNDF() > 0) {
  //    if (recoTrk.IsGhost()) {
  //      fph_ghost_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
  //      fph_ghost_prob->Fill(recoTrk.GetProb());
  //    }
  //    else {
  //      int iTmc          = recoTrk.GetMCTrackIndexes()[0];  // Index of first MC-track
  //      const auto& mcTrk = fpMCData->GetTrack(iTmc);
  //      // NOTE: reconstructed tracks usually have reference to only one MC-track
  //      if (mcTrk.IsReconstructable()) {
  //        fph_reco_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
  //        fph_reco_prob->Fill(recoTrk.GetProb());
  //      }
  //      else {
  //        fph_rest_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
  //        fph_rest_prob->Fill(recoTrk.GetProb());
  //      }
  //    }
  //  }  // recoTrk.GetNDF() > 0: end

  //  if (recoTrk.IsGhost()) {
  //    fph_ghost_purity->Fill(100 * recoTrk.GetMaxPurity());
  //    fph_ghost_p->Fill(recoTrk.GetP());
  //    fph_ghost_pt->Fill(recoTrk.GetPt());
  //    fph_ghost_nhits->Fill(recoTrk.GetNofHits());
  //    fph_ghost_fsta->Fill(hitFst.GetStationId());
  //    fph_ghost_fhitR->Fill(hitFst.GetDistFromBP());

  //    // z-positions of the first and second hit stations
  //    double z0 = fpParameters->GetStation(hitFst.GetStationId()).fZ[0];
  //    double z1 = fpParameters->GetStation(hitSnd.GetStationId()).fZ[0];

  //    if (fabs(z1 - z0) > 1.e-4) {  // test for z1 != z2
  //      fph_ghost_tx->Fill((hitSnd.GetX() - hitFst.GetX()) / (z1 - z0));
  //      fph_ghost_ty->Fill((hitSnd.GetY() - hitFst.GetY()) / (z1 - z0));
  //    }

  //    fph_ghost_nhits_vs_p->Fill(recoTrk.GetP(), recoTrk.GetNofHits());
  //    fph_ghost_fsta_vs_p->Fill(recoTrk.GetP(), hitFst.GetStationId());
  //    if (hitFst.GetStationId() <= hitLst.GetStationId()) {
  //      fph_ghost_lsta_vs_fsta->Fill(hitFst.GetStationId(), hitLst.GetStationId());
  //    }
  //  }  // recoTrk.IsGhost(): end
  //}  // loop over recoTrk: end


  // *************************************
  // ** Fill distributions of MC-tracks **
  // *************************************

  //for (const auto& mcTrk: fpMCData->GetTrackContainer()) {
  //
  //}
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Qa::SaveHistograms()
{
  fpOutFile->cd();
  RecursiveWrite(fpHistoDir);
  LOG(info) << "CA QA: Histograms have been written to \033[1;31m" << fpOutFile->GetName() << "\033[0m";
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Qa::RecursiveWrite(TObject* pObj)
{
  if (!pObj->IsFolder()) { pObj->Write(); }
  else {
    TDirectory* pObjDir = dynamic_cast<TDirectory*>(pObj);
    LOG_IF(fatal, !pObjDir) << "CA QA: Incorrect dynamic cast from the input pointer to RecursiveWrite function";

    TDirectory* pCurDir = gDirectory;
    TDirectory* pSubDir = pCurDir->mkdir(pObjDir->GetName());
    pSubDir->cd();
    TList* pObjList = pObjDir->GetList();
    TIter it(pObjList);
    while (TObject* pNext = it()) {
      RecursiveWrite(pNext);
    }
    pCurDir->cd();
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void Qa::SetOutputFilename(const char* filename)
{
  fpOutFile = std::make_unique<TFile>(filename, "RECREATE");
  if (!fpOutFile.get() || fpOutFile->IsZombie()) {
    LOG(fatal) << "CA QA: ROOT histogram output file \"" << filename << "\" cannot be created";
  }
  fpHistoDir = gROOT->mkdir("histograms");
}
