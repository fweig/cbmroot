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
using ::ca::tools::MCTrack;
using cbm::ca::OutputQa;

// ---------------------------------------------------------------------------------------------------------------------
//
OutputQa::OutputQa(int verbose, bool isMCUsed) : CbmQaTask("CbmCaOutputQa", "caout", verbose, isMCUsed)
{
  // Turn on default track classes
  AddTrackType(ETrackType::kAll);
  AddTrackType(ETrackType::kGhost);
  AddTrackType(ETrackType::kPrim);
  AddTrackType(ETrackType::kSec);
  AddTrackType(ETrackType::kPrimPIP);
  AddTrackType(ETrackType::kPrimPIM);
  AddTrackType(ETrackType::kSecPIP);
  AddTrackType(ETrackType::kSecPIM);
  AddTrackType(ETrackType::kPrimMUP);
  AddTrackType(ETrackType::kPrimMUM);
  AddTrackType(ETrackType::kSecMUP);
  AddTrackType(ETrackType::kSecMUM);

  //AddTrackType(ETrackType::kAllE);
  //AddTrackType(ETrackType::kAllMU);
  //AddTrackType(ETrackType::kAllPI);
  //AddTrackType(ETrackType::kAllK);
  //AddTrackType(ETrackType::kAllPPBAR);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::AddTrackType(ETrackType type, bool flag)
{
  LOG_IF(fatal, type >= kEND || type < 0) << fName << ": attempt to add unsupported track type " << type;
  fvbTrackTypeOn[type] = flag;
}

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

  // TODO: (experimental) more general approach for filling track types utilizing cuts on MC and reco tracks
  if constexpr (kEXPTRACKFILL) {
    // Fill histograms for each defined track type
    for (int iType = 0; iType < ETrackType::kEND; ++iType) {
      if (fvbTrackTypeOn[iType]) {
        fvpTrackHistograms[iType]->FillRecoTracks();
        if (fvpTrackHistograms[iType]->IsMCUsed()) { fvpTrackHistograms[iType]->FillMCTracks(); }
      }  // if track type ID filled
    }    // track type ID
  }
  else {
    for (size_t iTrkReco = 0; iTrkReco < fvRecoTracks.size(); ++iTrkReco) {
      const auto& recoTrk = fvRecoTracks[iTrkReco];

      // Reject tracks, which do not contain hits
      if (recoTrk.GetNofHits() < 1) { continue; }

      if (fvbTrackTypeOn[ETrackType::kAll]) { fvpTrackHistograms[ETrackType::kAll]->FillRecoTrack(iTrkReco); }

      if (IsMCUsed()) {
        if (fvbTrackTypeOn[ETrackType::kGhost] && recoTrk.IsGhost()) {
          fvpTrackHistograms[ETrackType::kGhost]->FillRecoTrack(iTrkReco);
        }
        int iTrkMC = recoTrk.GetMatchedMCTrackIndex();
        if (iTrkMC > -1) {
          const auto& mcTrk = fMCData.GetTrack(iTrkMC);
          int pdg           = mcTrk.GetPdgCode();
          bool isPrimary    = mcTrk.IsPrimary();

          // Cut tracks, which did not leave hits in tracker
          if (mcTrk.GetNofHits() == 0) { continue; }

          if (isPrimary) {
            if (fvbTrackTypeOn[ETrackType::kPrim]) { fvpTrackHistograms[ETrackType::kPrim]->FillRecoTrack(iTrkReco); }

            if (fvbTrackTypeOn[ETrackType::kPrimPI] && std::abs(pdg) == 211) {
              fvpTrackHistograms[ETrackType::kPrimPI]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kPrimPIP] && pdg == +211) {
              fvpTrackHistograms[ETrackType::kPrimPIP]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kPrimPIM] && pdg == -211) {
              fvpTrackHistograms[ETrackType::kPrimPIM]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kPrimMU] && std::abs(pdg) == 13) {
              fvpTrackHistograms[ETrackType::kPrimMU]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kPrimMUP] && pdg == +13) {
              fvpTrackHistograms[ETrackType::kPrimMUP]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kPrimMUM] && pdg == -13) {
              fvpTrackHistograms[ETrackType::kPrimMUM]->FillRecoTrack(iTrkReco);
            }
          }
          else {
            if (fvbTrackTypeOn[ETrackType::kSec]) { fvpTrackHistograms[ETrackType::kSec]->FillRecoTrack(iTrkReco); }

            if (fvbTrackTypeOn[ETrackType::kSecPI] && std::abs(pdg) == 211) {
              fvpTrackHistograms[ETrackType::kSecPI]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kSecPIP] && pdg == +211) {
              fvpTrackHistograms[ETrackType::kSecPIP]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kSecPIM] && pdg == -211) {
              fvpTrackHistograms[ETrackType::kSecPIM]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kSecMU] && std::abs(pdg) == 13) {
              fvpTrackHistograms[ETrackType::kSecMU]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kSecMUP] && pdg == +13) {
              fvpTrackHistograms[ETrackType::kSecMUP]->FillRecoTrack(iTrkReco);
            }

            if (fvbTrackTypeOn[ETrackType::kSecMUM] && pdg == -13) {
              fvpTrackHistograms[ETrackType::kSecMUM]->FillRecoTrack(iTrkReco);
            }
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
        if (fvbTrackTypeOn[ETrackType::kAll]) { fvpTrackHistograms[ETrackType::kAll]->FillMCTrack(iTrkMC); }

        int pdg        = mcTrk.GetPdgCode();
        bool isPrimary = mcTrk.IsPrimary();

        if (isPrimary) {
          if (fvbTrackTypeOn[ETrackType::kPrim]) { fvpTrackHistograms[ETrackType::kPrim]->FillMCTrack(iTrkMC); }

          if (fvbTrackTypeOn[ETrackType::kPrimPI] && std::abs(pdg) == 211) {
            fvpTrackHistograms[ETrackType::kPrimPI]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kPrimPIP] && pdg == +211) {
            fvpTrackHistograms[ETrackType::kPrimPIP]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kPrimPIM] && pdg == -211) {
            fvpTrackHistograms[ETrackType::kPrimPIM]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kPrimMU] && std::abs(pdg) == 13) {
            fvpTrackHistograms[ETrackType::kPrimMU]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kPrimMUP] && pdg == +13) {
            fvpTrackHistograms[ETrackType::kPrimMUP]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kPrimMUM] && pdg == -13) {
            fvpTrackHistograms[ETrackType::kPrimMUM]->FillMCTrack(iTrkMC);
          }
        }
        else {
          if (fvbTrackTypeOn[ETrackType::kSec]) { fvpTrackHistograms[ETrackType::kSec]->FillMCTrack(iTrkMC); }

          if (fvbTrackTypeOn[ETrackType::kSecPIP] && std::abs(pdg) == 211) {
            fvpTrackHistograms[ETrackType::kSecPIP]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kSecPIP] && pdg == +211) {
            fvpTrackHistograms[ETrackType::kPrimPIP]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kSecPIM] && pdg == -211) {
            fvpTrackHistograms[ETrackType::kPrimPIM]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kSecMU] && std::abs(pdg) == 13) {
            fvpTrackHistograms[ETrackType::kSecMU]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kSecMUP] && pdg == +13) {
            fvpTrackHistograms[ETrackType::kSecMUP]->FillMCTrack(iTrkMC);
          }

          if (fvbTrackTypeOn[ETrackType::kSecMUM] && pdg == -13) {
            fvpTrackHistograms[ETrackType::kSecMUM]->FillMCTrack(iTrkMC);
          }
        }
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
    if (!fvbTrackTypeOn[type]) { return; }
    bool bUseMC              = IsMCUsed() && !bSuppressMC;
    fvpTrackHistograms[type] = std::make_unique<TrackTypeQa>(typeName, fsPrefix.Data(), bUseMC, fpFolderRoot);
    fvpTrackHistograms[type]->RegisterParameters(fpParameters);
    fvpTrackHistograms[type]->RegisterRecoHits(fvHits);
    fvpTrackHistograms[type]->RegisterRecoTracks(fvRecoTracks);
    fvpTrackHistograms[type]->RegisterMCData(fMCData);

    if constexpr (kEXPTRACKFILL) {
      // Define track cuts
      switch (type) {
        //
        // General
        case ETrackType::kAll: break;
        case ETrackType::kGhost:
          fvpTrackHistograms[type]->SetRecoTrackCut([](const CbmL1Track& t) -> bool { return t.IsGhost(); });
          break;
        case kPrim:
          fvpTrackHistograms[type]->SetMCTrackCut([](const MCTrack& t) -> bool { return t.IsPrimary(); });
          break;
        case kSec:
          fvpTrackHistograms[type]->SetMCTrackCut([](const MCTrack& t) -> bool { return !t.IsPrimary(); });
          break;
        //
        // Electrons
        case kAllE:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return std::abs(t.GetPdgCode()) == 11; });
          break;
        case kPrimE:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && std::abs(t.GetPdgCode()) == 11; });
          break;
        case kPrimEP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == -11; });
          break;
        case kPrimEM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == 11; });
          break;
        case kSecE:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && std::abs(t.GetPdgCode()) == 11; });
          break;
        case kSecEP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == -11; });
          break;
        case kSecEM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == 11; });
          break;
        //
        // Muons
        case kAllMU:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return std::abs(t.GetPdgCode()) == 13; });
          break;
        case kPrimMU:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && std::abs(t.GetPdgCode()) == 13; });
          break;
        case kPrimMUP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == -13; });
          break;
        case kPrimMUM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == 13; });
          break;
        case kSecMU:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && std::abs(t.GetPdgCode()) == 13; });
          break;
        case kSecMUP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == -13; });
          break;
        case kSecMUM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == 13; });
          break;
        //
        // Pions
        case kAllPI:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return std::abs(t.GetPdgCode()) == 211; });
          break;
        case kPrimPI:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && std::abs(t.GetPdgCode()) == 211; });
          break;
        case kPrimPIP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == 211; });
          break;
        case kPrimPIM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == -211; });
          break;
        case kSecPI:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && std::abs(t.GetPdgCode()) == 211; });
          break;
        case kSecPIP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == 211; });
          break;
        case kSecPIM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == -211; });
          break;
        //
        // Kaons
        case kAllK:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return std::abs(t.GetPdgCode()) == 211; });
          break;
        case kPrimK:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && std::abs(t.GetPdgCode()) == 321; });
          break;
        case kPrimKP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == 321; });
          break;
        case kPrimKM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == -321; });
          break;
        case kSecK:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && std::abs(t.GetPdgCode()) == 321; });
          break;
        case kSecKP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == 321; });
          break;
        case kSecKM:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == -321; });
          break;
        //
        // Protons
        case kAllPPBAR:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return std::abs(t.GetPdgCode()) == 2212; });
          break;
        case kPrimPPBAR:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && std::abs(t.GetPdgCode()) == 2212; });
          break;
        case kPrimP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == 2212; });
          break;
        case kPrimPBAR:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return t.IsPrimary() && t.GetPdgCode() == -2212; });
          break;
        case kSecPPBAR:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && std::abs(t.GetPdgCode()) == 2212; });
          break;
        case kSecP:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == 2212; });
          break;
        case kSecPBAR:
          fvpTrackHistograms[type]->SetMCTrackCut(
            [](const MCTrack& t) -> bool { return !t.IsPrimary() && t.GetPdgCode() == -2212; });
          break;
      }
    }

    fvpTrackHistograms[type]->Init();
  };

  for (int i = 0; i < ETrackType::kEND; ++i) {
    LOG(info) << i << ' ' << fvpTrackHistograms[i].get() << ' ' << fvbTrackTypeOn[i];
  }

  RegisterTrackQa("all", ETrackType::kAll);
  if (IsMCUsed()) {
    RegisterTrackQa("ghost", ETrackType::kGhost, /*suppress MC*/ true);
    RegisterTrackQa("prim", ETrackType::kPrim);
    RegisterTrackQa("sec", ETrackType::kSec);
    RegisterTrackQa("all_pi", ETrackType::kAllPI);
    RegisterTrackQa("prim_pi", ETrackType::kPrimPI);
    RegisterTrackQa("prim_pip", ETrackType::kPrimPIP);
    RegisterTrackQa("prim_pim", ETrackType::kPrimPIM);
    RegisterTrackQa("sec_pi", ETrackType::kSecPI);
    RegisterTrackQa("sec_pip", ETrackType::kSecPIP);
    RegisterTrackQa("sec_pim", ETrackType::kSecPIM);
    RegisterTrackQa("all_e", ETrackType::kAllE);
    RegisterTrackQa("prim_e", ETrackType::kPrimE);
    RegisterTrackQa("prim_ep", ETrackType::kPrimEP);
    RegisterTrackQa("prim_em", ETrackType::kPrimEM);
    RegisterTrackQa("sec_e", ETrackType::kSecE);
    RegisterTrackQa("sec_ep", ETrackType::kSecEP);
    RegisterTrackQa("sec_em", ETrackType::kSecEM);
    RegisterTrackQa("all_mu", ETrackType::kAllMU);
    RegisterTrackQa("prim_mu", ETrackType::kPrimMU);
    RegisterTrackQa("prim_mup", ETrackType::kPrimMUP);
    RegisterTrackQa("prim_mum", ETrackType::kPrimMUM);
    RegisterTrackQa("sec_mu", ETrackType::kSecMU);
    RegisterTrackQa("sec_mup", ETrackType::kSecMUP);
    RegisterTrackQa("sec_mum", ETrackType::kSecMUM);
    RegisterTrackQa("all_k", ETrackType::kAllK);
    RegisterTrackQa("prim_k", ETrackType::kPrimK);
    RegisterTrackQa("prim_kp", ETrackType::kPrimKP);
    RegisterTrackQa("prim_km", ETrackType::kPrimKM);
    RegisterTrackQa("sec_k", ETrackType::kSecK);
    RegisterTrackQa("sec_kp", ETrackType::kSecKP);
    RegisterTrackQa("sec_km", ETrackType::kSecKM);
    RegisterTrackQa("all_ppbar", ETrackType::kAllPPBAR);
    RegisterTrackQa("prim_ppbar", ETrackType::kPrimPPBAR);
    RegisterTrackQa("prim_p", ETrackType::kPrimP);
    RegisterTrackQa("prim_pbar", ETrackType::kPrimPBAR);
    RegisterTrackQa("sec_ppbar", ETrackType::kSecPPBAR);
    RegisterTrackQa("sec_p", ETrackType::kSecP);
    RegisterTrackQa("sec_pbar", ETrackType::kSecPBAR);
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
