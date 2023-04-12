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

  for (size_t iTrkReco = 0; iTrkReco < fvRecoTracks.size(); ++iTrkReco) {
    const auto& recoTrk = fvRecoTracks[iTrkReco];

    // Reject tracks, which do not contain hits
    if (recoTrk.GetNofHits() < 1) { continue; }

    fvpTrackHistograms[ETrackType::kAll]->FillRecoTrack(iTrkReco);

    if (IsMCUsed()) {
      if (recoTrk.IsGhost()) { fvpTrackHistograms[ETrackType::kGhost]->FillRecoTrack(iTrkReco); }
      int iTrkMC = recoTrk.GetMatchedMCTrackIndex();
      if (iTrkMC > -1) {
        const auto& mcTrk = fMCData.GetTrack(iTrkMC);
        int pdg           = mcTrk.GetPdgCode();
        bool isPrimary    = mcTrk.IsPrimary();

        if (isPrimary) {
          fvpTrackHistograms[ETrackType::kPrim]->FillRecoTrack(iTrkReco);

          if (pdg == +211) { fvpTrackHistograms[ETrackType::kPrimPIP]->FillRecoTrack(iTrkReco); }
          if (pdg == -211) { fvpTrackHistograms[ETrackType::kPrimPIM]->FillRecoTrack(iTrkReco); }
        }
        else {
          fvpTrackHistograms[ETrackType::kSec]->FillRecoTrack(iTrkReco);

          if (pdg == +211) { fvpTrackHistograms[ETrackType::kSecPIP]->FillRecoTrack(iTrkReco); }
          if (pdg == -211) { fvpTrackHistograms[ETrackType::kSecPIM]->FillRecoTrack(iTrkReco); }
        }
      }
    }


  }  // loop over recoTrk: end


  // *************************************
  // ** Fill distributions of MC-tracks **
  // *************************************
  if (IsMCUsed()) {
    for (int iTrkMC = 0; iTrkMC < fMCData.GetNofTracks(); ++iTrkMC) {
      const auto& mcTrk = fMCData.GetTrack(iTrkMC);

      // Cut tracks, which did not leave hits in tracker
      if (mcTrk.GetNofHits() == 0) { continue; }

      // Fill different track categories
      fvpTrackHistograms[ETrackType::kAll]->FillMCTrack(iTrkMC);

      int pdg        = mcTrk.GetPdgCode();
      bool isPrimary = mcTrk.IsPrimary();

      if (isPrimary) {
        fvpTrackHistograms[ETrackType::kPrim]->FillMCTrack(iTrkMC);

        if (pdg == +211) { fvpTrackHistograms[ETrackType::kPrimPIP]->FillMCTrack(iTrkMC); }
        if (pdg == -211) { fvpTrackHistograms[ETrackType::kPrimPIM]->FillMCTrack(iTrkMC); }
      }
      else {
        fvpTrackHistograms[ETrackType::kSec]->FillMCTrack(iTrkMC);

        if (pdg == +211) { fvpTrackHistograms[ETrackType::kSecPIP]->FillMCTrack(iTrkMC); }
        if (pdg == -211) { fvpTrackHistograms[ETrackType::kSecPIM]->FillMCTrack(iTrkMC); }
      }
    }
  }
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
  auto RegisterTrackQa = [&](const char* typeName, ETrackType type, bool bSuppressMC = false) {
    bool bUseMC              = IsMCUsed() && !bSuppressMC;
    fvpTrackHistograms[type] = std::make_unique<TrackTypeQa>(typeName, fsPrefix.Data(), bUseMC, fpFolderRoot);
    fvpTrackHistograms[type]->RegisterParameters(fpParameters);
    fvpTrackHistograms[type]->RegisterRecoHits(fvHits);
    fvpTrackHistograms[type]->RegisterRecoTracks(fvRecoTracks);
    fvpTrackHistograms[type]->RegisterMCData(fMCData);
    fvpTrackHistograms[type]->Init();
  };

  RegisterTrackQa("ghost", ETrackType::kGhost, /*suppress MC*/ true);
  RegisterTrackQa("all", ETrackType::kAll);
  if (IsMCUsed()) {
    RegisterTrackQa("prim", ETrackType::kPrim);
    RegisterTrackQa("sec", ETrackType::kSec);
    RegisterTrackQa("prim_pip", ETrackType::kPrimPIP);
    RegisterTrackQa("prim_pim", ETrackType::kPrimPIM);
    RegisterTrackQa("sec_pip", ETrackType::kSecPIP);
    RegisterTrackQa("sec_pim", ETrackType::kSecPIM);
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
