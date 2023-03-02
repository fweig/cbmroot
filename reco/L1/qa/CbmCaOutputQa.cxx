/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaOutputQa.cxx
/// @brief  Tracking output QA-task (implementation)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaOutputQa.h"

#include "FairRootManager.h"
#include "Logger.h"

#include "L1InitManager.h"

using ca::tools::Debugger;
using cbm::ca::OutputQa;

// ---------------------------------------------------------------------------------------------------------------------
//
OutputQa::OutputQa(int verbose, bool isMCUsed) : CbmQaTask("CbmCaOutputQa", "caout", verbose, isMCUsed) {}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::EnableDebugger(const char* filename)
{
  if (!fpDebugger.get()) { fpDebugger = std::make_shared<Debugger>(filename); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::FillHistograms()
{
  // ************************************************************************
  // ** Fill distributions for reconstructed tracks (including ghost ones) **
  // ************************************************************************

  for (const auto& recoTrk : fvRecoTracks) {
    // Reject tracks, which do not contain hits
    if (recoTrk.GetNofHits() < 1) { continue; }

    const auto& hitFst = (fvHits)[recoTrk.GetHitIndexes()[0]];                         // first hit of track
    const auto& hitSnd = (fvHits)[recoTrk.GetHitIndexes()[1]];                         // second hit of track
    const auto& hitLst = (fvHits)[recoTrk.GetHitIndexes()[recoTrk.GetNofHits() - 1]];  // last hit of track

    // ***************************************************
    // ** Histograms, requiring reconstructed data only **
    // ***************************************************
    //
    fph_reco_p->Fill(recoTrk.GetP());
    fph_reco_pt->Fill(recoTrk.GetPt());
    fph_reco_phi->Fill(recoTrk.GetPhi());
    fph_reco_tx->Fill(recoTrk.GetTx());
    fph_reco_ty->Fill(recoTrk.GetTy());
    fph_reco_nhits->Fill(recoTrk.GetNofHits());
    fph_reco_fsta->Fill(hitFst.GetStationId());
    fph_reco_fhitR->Fill(hitFst.GetR());


    // ******************************************
    // ** Histograms, requiring MC information **
    // ******************************************
    //
    if (IsMCUsed()) { fph_reco_purity->Fill(100 * recoTrk.GetMaxPurity()); }

    if (recoTrk.GetNDF() > 0) {
      if (IsMCUsed() && recoTrk.IsGhost()) {
        fph_ghost_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
        fph_ghost_prob->Fill(recoTrk.GetProb());
      }
      else {
        int iTmc          = recoTrk.GetMCTrackIndexes()[0];  // Index of first MC-track
        const auto& mcTrk = fMCData.GetTrack(iTmc);
        // NOTE: reconstructed tracks usually have reference to only one MC-track
        if (mcTrk.IsReconstructable()) {
          fph_reco_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
          fph_reco_prob->Fill(recoTrk.GetProb());
        }
        else {
          fph_rest_chi2_ndf->Fill(recoTrk.GetChiSq() / recoTrk.GetNDF());
          fph_rest_prob->Fill(recoTrk.GetProb());
        }
      }
    }  // recoTrk.GetNDF() > 0: end

    if (recoTrk.IsGhost()) {
      fph_ghost_purity->Fill(100 * recoTrk.GetMaxPurity());
      fph_ghost_p->Fill(recoTrk.GetP());
      fph_ghost_pt->Fill(recoTrk.GetPt());
      fph_ghost_phi->Fill(recoTrk.GetPhi());
      fph_ghost_nhits->Fill(recoTrk.GetNofHits());
      fph_ghost_fsta->Fill(hitFst.GetStationId());
      fph_ghost_fhitR->Fill(hitFst.GetR());

      // z-positions of the first and second hit stations
      double z0 = fpParameters->GetStation(hitFst.GetStationId()).fZ[0];
      double z1 = fpParameters->GetStation(hitSnd.GetStationId()).fZ[0];

      if (fabs(z1 - z0) > 1.e-4) {  // test for z1 != z2
        fph_ghost_tx->Fill((hitSnd.GetX() - hitFst.GetX()) / (z1 - z0));
        fph_ghost_ty->Fill((hitSnd.GetY() - hitFst.GetY()) / (z1 - z0));
      }

      fph_ghost_nhits_vs_p->Fill(recoTrk.GetP(), recoTrk.GetNofHits());
      fph_ghost_fsta_vs_p->Fill(recoTrk.GetP(), hitFst.GetStationId());
      if (hitFst.GetStationId() <= hitLst.GetStationId()) {
        fph_ghost_lsta_vs_fsta->Fill(hitFst.GetStationId(), hitLst.GetStationId());
      }
    }  // recoTrk.IsGhost(): end
  }    // loop over recoTrk: end


  // *************************************
  // ** Fill distributions of MC-tracks **
  // *************************************

  //for (const auto& mcTrk: fpMCData->GetTrackContainer()) {
  //
  //}
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitCanvases()
{
  // ***************************
  // **  Track distributions


  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitDataBranches()
{
  // Turn off detectors, which hits are not presented in input tree
  auto* fairManager = FairRootManager::Instance();
  fbUseMvd          = fbUseMvd && fairManager->GetObject("MvdHit");
  fbUseSts          = fbUseSts && fairManager->GetObject("StsHit");
  fbUseMuch         = fbUseMuch && fairManager->GetObject("MuchPixelHit");
  fbUseTrd          = fbUseTrd && fairManager->GetObject("TrdHit");
  fbUseTof          = fbUseTof && fairManager->GetObject("TofHit");


  LOG(info) << fName << ": Detector subsystems used:";
  LOG(info) << "\tMVD:  " << (fbUseMvd ? "ON" : "OFF");
  LOG(info) << "\tSTS:  " << (fbUseSts ? "ON" : "OFF");
  LOG(info) << "\tMuCh: " << (fbUseMuch ? "ON" : "OFF");
  LOG(info) << "\tTRD:  " << (fbUseTrd ? "ON" : "OFF");
  LOG(info) << "\tTOF:  " << (fbUseTof ? "ON" : "OFF");

  LOG(info) << fName << ": Initializing data branches";
  LOG_IF(fatal, !fpParameters.get())
    << fName << ": CA parameters object is not defined. Please, provide initializer or read parameters from binary "
    << "via OutputQa::ReadParameters(filename) from the qa macro";

  // Initialize IO data manager
  if (!fpDataManager.get()) { fpDataManager = std::make_shared<L1IODataManager>(); }

  // Initialize time slice reader instance
  if (!fpTSReader.get()) { fpTSReader = std::make_unique<TimeSliceReader>(fTrackingMode); }
  fpTSReader->SetDetector(L1DetectorID::kMvd, fbUseMvd);
  fpTSReader->SetDetector(L1DetectorID::kSts, fbUseSts);
  fpTSReader->SetDetector(L1DetectorID::kMuch, fbUseMuch);
  fpTSReader->SetDetector(L1DetectorID::kTrd, fbUseTrd);
  fpTSReader->SetDetector(L1DetectorID::kTof, fbUseTof);

  fpTSReader->RegisterParameters(fpParameters);
  fpTSReader->RegisterTracksContainer(fvRecoTracks);
  fpTSReader->RegisterQaHitContainer(fvHits);
  fpTSReader->RegisterHitIndexContainer(fvHitIds);

  if (!fpTSReader->InitRun()) { return kFATAL; }

  // Initialize MC module
  if (IsMCUsed()) {
    if (!fpMCModule.get()) { fpMCModule = std::make_unique<CbmCaMCModule>(fVerbose, fPerformanceMode); }
    fpMCModule->SetDetector(L1DetectorID::kMvd, fbUseMvd);
    fpMCModule->SetDetector(L1DetectorID::kSts, fbUseSts);
    fpMCModule->SetDetector(L1DetectorID::kMuch, fbUseMuch);
    fpMCModule->SetDetector(L1DetectorID::kTrd, fbUseTrd);
    fpMCModule->SetDetector(L1DetectorID::kTof, fbUseTof);

    fpMCModule->RegisterMCData(fMCData);
    fpMCModule->RegisterRecoTrackContainer(fvRecoTracks);
    fpMCModule->RegisterHitIndexContainer(fvHitIds);
    fpMCModule->RegisterQaHitContainer(fvHits);
    fpMCModule->RegisterParameters(fpParameters);
    fpMCModule->RegisterFirstHitIndexes(fpTSReader->GetHitFirstIndexDet());

    if (!fpMCModule->InitRun()) { return kFATAL; }
  }
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitHistograms()
{
  fph_reco_p     = MakeHistoFromConfig<TH1F>("reco_p");
  fph_reco_pt    = MakeHistoFromConfig<TH1F>("reco_pt");
  fph_reco_phi   = MakeHistoFromConfig<TH1F>("reco_phi");
  fph_reco_tx    = MakeHistoFromConfig<TH1F>("reco_tx");
  fph_reco_ty    = MakeHistoFromConfig<TH1F>("reco_ty");
  fph_reco_nhits = MakeHistoFromConfig<TH1F>("reco_nhits");
  fph_reco_fsta  = MakeHistoFromConfig<TH1F>("reco_fsta");
  fph_reco_fhitR = MakeHistoFromConfig<TH1F>("reco_fhitR");

  if (IsMCUsed()) {
    fph_reco_purity   = MakeHistoFromConfig<TH1F>("reco_purity");
    fph_reco_chi2_ndf = MakeHistoFromConfig<TH1F>("reco_chi2_ndf");  // TODO: Can be filled without MC
    fph_reco_prob     = MakeHistoFromConfig<TH1F>("reco_prob");      // TODO: Can be filled without MC
    fph_rest_chi2_ndf = MakeHistoFromConfig<TH1F>("rest_chi2_ndf");
    fph_rest_prob     = MakeHistoFromConfig<TH1F>("rest_prob");

    fph_ghost_p            = MakeHistoFromConfig<TH1F>("ghost_p");
    fph_ghost_pt           = MakeHistoFromConfig<TH1F>("ghost_pt");
    fph_ghost_phi          = MakeHistoFromConfig<TH1F>("ghost_phi");
    fph_ghost_nhits        = MakeHistoFromConfig<TH1F>("ghost_nhits");
    fph_ghost_fsta         = MakeHistoFromConfig<TH1F>("ghost_fsta");
    fph_ghost_purity       = MakeHistoFromConfig<TH1F>("ghost_purity");
    fph_ghost_chi2_ndf     = MakeHistoFromConfig<TH1F>("ghost_chi2_ndf");
    fph_ghost_prob         = MakeHistoFromConfig<TH1F>("ghost_prob");
    fph_ghost_tx           = MakeHistoFromConfig<TH1F>("ghost_tx");
    fph_ghost_ty           = MakeHistoFromConfig<TH1F>("ghost_ty");
    fph_ghost_fhitR        = MakeHistoFromConfig<TH1F>("ghost_fhitR");
    fph_ghost_nhits_vs_p   = MakeHistoFromConfig<TH2F>("ghost_nhits_vs_p");
    fph_ghost_fsta_vs_p    = MakeHistoFromConfig<TH2F>("ghost_fsta_vs_p");
    fph_ghost_lsta_vs_fsta = MakeHistoFromConfig<TH2F>("ghost_lsta_vs_fsta");

    // Residuals and pools of track parameters
    fph_fst_res_x    = MakeHistoFromConfig<TH1F>("fst_res_x");
    fph_fst_res_y    = MakeHistoFromConfig<TH1F>("fst_res_y");
    fph_fst_res_tx   = MakeHistoFromConfig<TH1F>("fst_res_tx");
    fph_fst_res_ty   = MakeHistoFromConfig<TH1F>("fst_res_ty");
    fph_fst_res_qp   = MakeHistoFromConfig<TH1F>("fst_res_qp");
    fph_fst_res_time = MakeHistoFromConfig<TH1F>("fst_res_time");
    fph_fst_res_v    = MakeHistoFromConfig<TH1F>("fst_res_v");

    fph_fst_pull_x    = MakeHistoFromConfig<TH1F>("fst_pull_x");
    fph_fst_pull_y    = MakeHistoFromConfig<TH1F>("fst_pull_y");
    fph_fst_pull_tx   = MakeHistoFromConfig<TH1F>("fst_pull_tx");
    fph_fst_pull_ty   = MakeHistoFromConfig<TH1F>("fst_pull_ty");
    fph_fst_pull_qp   = MakeHistoFromConfig<TH1F>("fst_pull_qp");
    fph_fst_pull_time = MakeHistoFromConfig<TH1F>("fst_pull_time");
    fph_fst_pull_v    = MakeHistoFromConfig<TH1F>("fst_pull_v");
  }

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitTimeSlice()
{
  int nMCTracks   = 0;
  int nMCPoints   = 0;
  int nHits       = 0;
  int nRecoTracks = 0;

  // Read reconstructed input
  fpTSReader->InitTimeSlice();
  nHits       = fvHits.size();
  nRecoTracks = fvRecoTracks.size();

  if (IsMCUsed()) {
    // Read MC information
    fpMCModule->InitEvent(nullptr);
    nMCPoints = fMCData.GetNofPoints();
    nMCTracks = fMCData.GetNofTracks();
  }

  LOG_IF(info, fVerbose > 1) << fName << ": Data sample consists of " << nHits << " hits, " << nRecoTracks
                             << " reco tracks, " << nMCTracks << " MC tracks, " << nMCPoints << " MC points";

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::ReadParameters(const char* filename)
{
  fpParameters = std::make_shared<L1Parameters>();

  L1InitManager manager;
  manager.ReadParametersObject(filename);
  manager.SendParameters(*fpParameters);

  LOG(info) << fpParameters->ToString(0);
}
