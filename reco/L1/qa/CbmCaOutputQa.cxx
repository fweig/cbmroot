/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaOutputQa.cxx
/// @brief  Tracking output QA-task (implementation)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaOutputQa.h"

#include "CbmCaMCModule.h"
#include "CbmQaCanvas.h"

#include "FairRootManager.h"
#include "Logger.h"

#include "THStack.h"
#include "TPad.h"

#include "L1InitManager.h"

using ca::tools::Debugger;
using ::ca::tools::MCTrack;
using cbm::ca::MCModule;
using cbm::ca::OutputQa;

// ---------------------------------------------------------------------------------------------------------------------
//
OutputQa::OutputQa(int verbose, bool isMCUsed) : CbmQaTask("CbmCaOutputQa", "caout", verbose, isMCUsed)
{
  // Create MC module

  // Create TS reader
  fpTSReader = std::make_unique<TimeSliceReader>();

  // Turn on default track classes
  AddTrackType(ETrackType::kAll);
  AddTrackType(ETrackType::kGhost);
  AddTrackType(ETrackType::kPrim);
  AddTrackType(ETrackType::kSec);
  AddTrackType(ETrackType::kPrimE);
  AddTrackType(ETrackType::kPrimPI);
  AddTrackType(ETrackType::kPrimK);
  AddTrackType(ETrackType::kPrimMU);
  AddTrackType(ETrackType::kPrimPPBAR);
  AddTrackType(ETrackType::kSecE);
  AddTrackType(ETrackType::kSecPI);
  AddTrackType(ETrackType::kSecK);
  AddTrackType(ETrackType::kSecMU);
  AddTrackType(ETrackType::kSecPPBAR);

  AddTrackType(ETrackType::kPrimPIP);
  AddTrackType(ETrackType::kPrimPIM);
  AddTrackType(ETrackType::kSecPIP);
  AddTrackType(ETrackType::kSecPIM);
  AddTrackType(ETrackType::kPrimKP);
  AddTrackType(ETrackType::kPrimKM);
  AddTrackType(ETrackType::kSecKP);
  AddTrackType(ETrackType::kSecKM);
  AddTrackType(ETrackType::kPrimP);
  AddTrackType(ETrackType::kPrimPBAR);
  AddTrackType(ETrackType::kSecP);
  AddTrackType(ETrackType::kSecPBAR);

  //AddTrackType(ETrackType::kAllE);
  //AddTrackType(ETrackType::kAllMU);
  //AddTrackType(ETrackType::kAllPI);
  //AddTrackType(ETrackType::kAllK);
  //AddTrackType(ETrackType::kAllPPBAR);

  // Init track type histograms drawing attributes
  InitDrawingAttributes();
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
  }      // kEXPTRACKFILL = true
  else {
    for (size_t iTrkReco = 0; iTrkReco < fvRecoTracks.size(); ++iTrkReco) {
      const auto& recoTrk = fvRecoTracks[iTrkReco];

      // Reject tracks, which do not contain hits
      if (recoTrk.GetNofHits() < 1) { continue; }

      FillRecoTrack(ETrackType::kAll, iTrkReco);

      if (IsMCUsed()) {
        // NOTE: The ghost status of track is now defined by its purity, thus it can still contain MC information
        if (recoTrk.IsGhost()) { FillRecoTrack(ETrackType::kGhost, iTrkReco); }

        int iTrkMC = recoTrk.GetMatchedMCTrackIndex();
        if (iTrkMC > -1) {
          const auto& mcTrk = fMCData.GetTrack(iTrkMC);
          int pdg           = mcTrk.GetPdgCode();
          bool isPrimary    = mcTrk.IsPrimary();

          // Cut tracks, which did not leave hits in tracker
          if (mcTrk.GetNofHits() == 0) { continue; }

          if (isPrimary) { FillRecoTrack(ETrackType::kPrim, iTrkReco); }
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

          // Track distributions for different particle species
          switch (std::abs(pdg)) {
            case 211:  // pion
              FillRecoTrack(ETrackType::kAllPI, iTrkReco);
              if (isPrimary) {
                FillRecoTrack(ETrackType::kPrimPI, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kPrimPIP : ETrackType::kPrimPIM, iTrkReco);
              }
              else {
                FillRecoTrack(ETrackType::kSecPI, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kSecPIP : ETrackType::kSecPIM, iTrkReco);
              }
              break;
            case 2212:  // proton
              FillRecoTrack(ETrackType::kAllPPBAR, iTrkReco);
              if (isPrimary) {
                FillRecoTrack(ETrackType::kPrimPPBAR, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kPrimP : ETrackType::kPrimPBAR, iTrkReco);
              }
              else {
                FillRecoTrack(ETrackType::kSecPPBAR, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kSecP : ETrackType::kSecPBAR, iTrkReco);
              }
              break;
            case 321:  // kaon
              FillRecoTrack(ETrackType::kAllK, iTrkReco);
              if (isPrimary) {
                FillRecoTrack(ETrackType::kPrimK, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kPrimKP : ETrackType::kPrimKM, iTrkReco);
              }
              else {
                FillRecoTrack(ETrackType::kSecK, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kSecKP : ETrackType::kSecKM, iTrkReco);
              }
              break;
            case 11:  // electron
              FillRecoTrack(ETrackType::kAllE, iTrkReco);
              if (isPrimary) {
                FillRecoTrack(ETrackType::kPrimE, iTrkReco);
                FillRecoTrack((pdg < 0) ? ETrackType::kPrimEP : ETrackType::kPrimEM, iTrkReco);
              }
              else {
                FillRecoTrack(ETrackType::kSecE, iTrkReco);
                FillRecoTrack((pdg < 0) ? ETrackType::kSecEP : ETrackType::kSecEM, iTrkReco);
              }
              break;
            case 13:  // muon
              FillRecoTrack(ETrackType::kAllMU, iTrkReco);
              if (isPrimary) {
                FillRecoTrack(ETrackType::kPrimMU, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kPrimMUP : ETrackType::kPrimMUM, iTrkReco);
              }
              else {
                FillRecoTrack(ETrackType::kSecMU, iTrkReco);
                FillRecoTrack((pdg > 0) ? ETrackType::kSecMUP : ETrackType::kSecMUM, iTrkReco);
              }
              break;
          }  // switch abs(pdg): end
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

        int pdg        = mcTrk.GetPdgCode();
        bool isPrimary = mcTrk.IsPrimary();

        // Fill different track categories
        FillMCTrack(ETrackType::kAll, iTrkMC);
        if (isPrimary) { FillMCTrack(ETrackType::kPrim, iTrkMC); }
        else {
          FillMCTrack(ETrackType::kSec, iTrkMC);
        }

        // Track distributions for different particle species
        switch (std::abs(pdg)) {
          case 211:  // pion
            FillMCTrack(ETrackType::kAllPI, iTrkMC);
            if (isPrimary) {
              FillMCTrack(ETrackType::kPrimPI, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kPrimPIP : ETrackType::kPrimPIM, iTrkMC);
            }
            else {
              FillMCTrack(ETrackType::kSecPI, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kSecPIP : ETrackType::kSecPIM, iTrkMC);
            }
            break;
          case 2212:  // proton
            FillMCTrack(ETrackType::kAllPPBAR, iTrkMC);
            if (isPrimary) {
              FillMCTrack(ETrackType::kPrimPPBAR, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kPrimP : ETrackType::kPrimPBAR, iTrkMC);
            }
            else {
              FillMCTrack(ETrackType::kSecPPBAR, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kSecP : ETrackType::kSecPBAR, iTrkMC);
            }
            break;
          case 321:  // kaon
            FillMCTrack(ETrackType::kAllK, iTrkMC);
            if (isPrimary) {
              FillMCTrack(ETrackType::kPrimK, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kPrimKP : ETrackType::kPrimKM, iTrkMC);
            }
            else {
              FillMCTrack(ETrackType::kSecK, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kSecKP : ETrackType::kSecKM, iTrkMC);
            }
            break;
          case 11:  // electron
            FillMCTrack(ETrackType::kAllE, iTrkMC);
            if (isPrimary) {
              FillMCTrack(ETrackType::kPrimE, iTrkMC);
              FillMCTrack((pdg < 0) ? ETrackType::kPrimEP : ETrackType::kPrimEM, iTrkMC);
            }
            else {
              FillMCTrack(ETrackType::kSecE, iTrkMC);
              FillMCTrack((pdg < 0) ? ETrackType::kSecEP : ETrackType::kSecEM, iTrkMC);
            }
            break;
          case 13:  // muon
            FillMCTrack(ETrackType::kAllMU, iTrkMC);
            if (isPrimary) {
              FillMCTrack(ETrackType::kPrimMU, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kPrimMUP : ETrackType::kPrimMUM, iTrkMC);
            }
            else {
              FillMCTrack(ETrackType::kSecMU, iTrkMC);
              FillMCTrack((pdg > 0) ? ETrackType::kSecMUP : ETrackType::kSecMUM, iTrkMC);
            }
            break;
        }  // switch abs(pdg): end
      }    // iTrkMC
    }      // IsMCUsed()
  }        // kEXPTRACKFILL = false
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitCanvases()
{
  /// Set of track types to compare
  std::vector<ETrackType> vCmpTypesGeneral = {kAll, kPrim, kSec};
  std::vector<ETrackType> vCmpTypesPrim    = {kPrim, kPrimE, kPrimMU, kPrimPI, kPrimK, kPrimPPBAR};
  std::vector<ETrackType> vCmpTypesSec     = {kSec, kSecE, kSecMU, kSecPI, kSecK, kSecPPBAR};
  std::vector<ETrackType> vCmpTypesPions   = {kAllPI, kPrimPIP, kPrimPIM, kSecPIP, kSecPIM};
  std::vector<ETrackType> vCmpTypesKaons   = {kAllK, kPrimKP, kPrimKM, kSecKP, kSecKM};
  std::vector<ETrackType> vCmpTypesProtons = {kAllPPBAR, kPrimP, kPrimPBAR, kSecP, kSecPBAR};

  /// @brief Function to draw generic canvas of histogram comparison
  auto DrawTrackDistributions = [&](CbmQaCanvas* pCanv, std::function<TH1F*(ETrackType)> Hist) {
    pCanv->Divide2D(6);
    pCanv->cd(1);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesGeneral, Hist);
    pCanv->cd(2);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesPrim, Hist);
    pCanv->cd(3);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesSec, Hist);
    pCanv->cd(4);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesPions, Hist);
    pCanv->cd(5);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesKaons, Hist);
    pCanv->cd(6);
    gPad->SetLogy();
    DrawSetOf<TH1F>(vCmpTypesProtons, Hist);
  };

  /// @brief Function to draw generic canvas of efficiencies comparison
  auto DrawTrackEfficiens = [&](CbmQaCanvas* pCanv, std::function<TProfile*(ETrackType)> Prof) {
    pCanv->Divide2D(3);
    pCanv->cd(1);
    DrawSetOf<TProfile>(vCmpTypesGeneral, Prof);
    pCanv->cd(2);
    DrawSetOf<TProfile>(vCmpTypesPrim, Prof);
    pCanv->cd(3);
    DrawSetOf<TProfile>(vCmpTypesSec, Prof);
  };


  if (IsMCUsed()) {
    // **  Reconstructed track distributions  **
    // Reconstructed pseudorapidity
    auto* pc_reco_eta =
      MakeCanvas<CbmQaCanvas>("reco_eta", "Reconstructed track pseudorapidity", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_reco_eta, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_reco_eta; });

    // MC pseudorapidity
    auto* pc_reco_etaMC =
      MakeCanvas<CbmQaCanvas>("reco_etaMC", "Reconstructed track MC pseudorapidity", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_reco_etaMC, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_reco_etaMC; });

    // MC momentum
    auto* pc_reco_pMC =
      MakeCanvas<CbmQaCanvas>("reco_pMC", "Reconstructed track MC momentum", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_reco_pMC, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_reco_pMC; });

    // MC rapidity
    auto* pc_reco_yMC =
      MakeCanvas<CbmQaCanvas>("reco_yMC", "Reconstructed track MC rapidity", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_reco_yMC, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_reco_yMC; });

    // **  MC track distributions  **

    // MC momentum
    auto* pc_mc_pMC =
      MakeCanvas<CbmQaCanvas>("mc_pMC", "MC reconstructable track MC momentum", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_mc_pMC, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_mc_pMC; });

    // MC rapidity
    auto* pc_mc_yMC =
      MakeCanvas<CbmQaCanvas>("mc_yMC", "MC reconstructable track MC rapidity", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawTrackDistributions(pc_mc_yMC, [&](ETrackType t) -> TH1F* { return fvpTrackHistograms[t]->fph_mc_yMC; });

    // MC rapidity vs. MC momentum
    auto* pc_mc_pMC_yMC =
      MakeCanvas<CbmQaCanvas>("mc_pMC_yMC", "MC track MC mom. vs. rapidity ", kCXSIZEPX * 3, kCYSIZEPX * 2);
    DrawSetOf<TH2F>(vCmpTypesGeneral, [&](ETrackType t) -> TH2F* { return fvpTrackHistograms[t]->fph_reco_pMC_yMC; });

    // **  Efficiencies  **

    // MC momentum
    auto* pc_eff_pMC = MakeCanvas<CbmQaCanvas>("eff_pMC", "Tracking Eff. vs. MC momentum", kCXSIZEPX * 3, kCYSIZEPX);
    DrawTrackEfficiens(pc_eff_pMC, [&](ETrackType t) -> TProfile* { return fvpTrackHistograms[t]->fph_eff_pMC; });

    auto* pc_eff_yMC = MakeCanvas<CbmQaCanvas>("eff_yMC", "Tracking Eff. vs. MC rapidity", kCXSIZEPX * 3, kCYSIZEPX);
    DrawTrackEfficiens(pc_eff_yMC, [&](ETrackType t) -> TProfile* { return fvpTrackHistograms[t]->fph_eff_yMC; });

    auto* pc_eff_thetaMC =
      MakeCanvas<CbmQaCanvas>("eff_thetaMC", "Tracking Eff. vs. MC polar angle", kCXSIZEPX * 3, kCYSIZEPX);
    DrawTrackEfficiens(pc_eff_thetaMC,
                       [&](ETrackType t) -> TProfile* { return fvpTrackHistograms[t]->fph_eff_thetaMC; });

    auto* pc_eff_phiMC =
      MakeCanvas<CbmQaCanvas>("eff_phiMC", "Tracking Eff. vs. MC azimuthal angle", kCXSIZEPX * 3, kCYSIZEPX);
    DrawTrackEfficiens(pc_eff_phiMC, [&](ETrackType t) -> TProfile* { return fvpTrackHistograms[t]->fph_eff_phiMC; });

    auto* pc_eff_etaMC =
      MakeCanvas<CbmQaCanvas>("eff_etaMC", "Tracking Eff. vs. MC pseudorapidity", kCXSIZEPX * 3, kCYSIZEPX);
    DrawTrackEfficiens(pc_eff_etaMC, [&](ETrackType t) -> TProfile* { return fvpTrackHistograms[t]->fph_eff_etaMC; });


    // ** Pulls and residuals **
    // NOTE: stored in a subdirectory for a given track type and point type
    for (int iType = 0; iType < ETrackType::kEND; ++iType) {
      if (fvbTrackTypeOn[iType] && fvpTrackHistograms[iType]->IsMCUsed()) {
        fvpTrackHistograms[iType]->fpFitQaFirstHit->CreateResidualPlot();
        fvpTrackHistograms[iType]->fpFitQaFirstHit->CreatePullPlot();
        fvpTrackHistograms[iType]->fpFitQaLastHit->CreateResidualPlot();
        fvpTrackHistograms[iType]->fpFitQaLastHit->CreatePullPlot();
      }
    }
  }

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
  fpTSReader->SetTrackingMode(fTrackingMode);
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
    fpMCModule = std::make_shared<MCModule>(fVerbose, fPerformanceMode);
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
  auto RegisterTrackQa = [&](const char* typeName, const char* title, ETrackType type, bool bSuppressMC = false) {
    if (!fvbTrackTypeOn[type]) { return; }
    bool bUseMC              = IsMCUsed() && !bSuppressMC;
    fvpTrackHistograms[type] = std::make_unique<TrackTypeQa>(typeName, fsPrefix.Data(), bUseMC, fpFolderRoot);
    fvpTrackHistograms[type]->SetTitle(title);
    fvpTrackHistograms[type]->RegisterParameters(fpParameters);
    fvpTrackHistograms[type]->RegisterRecoHits(fvHits);
    fvpTrackHistograms[type]->RegisterRecoTracks(fvRecoTracks);
    fvpTrackHistograms[type]->RegisterMCData(fMCData);
    fvpTrackHistograms[type]->SetDrawAtt(fvTrackDrawAtts[type].fColor, fvTrackDrawAtts[type].fMarker);
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
        case kEND: break;
      }
    }

    fvpTrackHistograms[type]->Init();
  };

  for (int i = 0; i < ETrackType::kEND; ++i) {
    LOG(info) << i << ' ' << fvpTrackHistograms[i].get() << ' ' << fvbTrackTypeOn[i];
  }

  RegisterTrackQa("all", "all", ETrackType::kAll);
  if (IsMCUsed()) {
    RegisterTrackQa("ghost", "ghost", ETrackType::kGhost, true);
    RegisterTrackQa("prim", "primary", ETrackType::kPrim);
    RegisterTrackQa("sec", "secondary", ETrackType::kSec);
    RegisterTrackQa("all_pi", "all #pi^{#pm}", ETrackType::kAllPI);
    RegisterTrackQa("prim_pi", "primary #pi^{#pm}", ETrackType::kPrimPI);
    RegisterTrackQa("prim_pip", "primary #pi^{#plus}", ETrackType::kPrimPIP);
    RegisterTrackQa("prim_pim", "primary #pi^{#minus}", ETrackType::kPrimPIM);
    RegisterTrackQa("sec_pi", "secondary #pi^{#pm}", ETrackType::kSecPI);
    RegisterTrackQa("sec_pip", "secondary #pi^{#plus}", ETrackType::kSecPIP);
    RegisterTrackQa("sec_pim", "secondary #pi^{#minus}", ETrackType::kSecPIM);
    RegisterTrackQa("all_e", "all e^{#pm}", ETrackType::kAllE);
    RegisterTrackQa("prim_e", "primary e^{#pm}", ETrackType::kPrimE);
    RegisterTrackQa("prim_ep", "primary e^{#plus}", ETrackType::kPrimEP);
    RegisterTrackQa("prim_em", "primary e^{#minus}", ETrackType::kPrimEM);
    RegisterTrackQa("sec_e", "secondary e^{#pm}", ETrackType::kSecE);
    RegisterTrackQa("sec_ep", "secondary e^{#plus}", ETrackType::kSecEP);
    RegisterTrackQa("sec_em", "secondary e^{#minus}", ETrackType::kSecEM);
    RegisterTrackQa("all_mu", "all #mu^{#pm}", ETrackType::kAllMU);
    RegisterTrackQa("prim_mu", "primary #mu^{#pm}", ETrackType::kPrimMU);
    RegisterTrackQa("prim_mup", "primary #mu^{#plus}", ETrackType::kPrimMUP);
    RegisterTrackQa("prim_mum", "primary #mu^{#minus}", ETrackType::kPrimMUM);
    RegisterTrackQa("sec_mu", "secondary #mu^{#pm}", ETrackType::kSecMU);
    RegisterTrackQa("sec_mup", "secondary #mu^{#plus}", ETrackType::kSecMUP);
    RegisterTrackQa("sec_mum", "secondary #mu^{#minus}", ETrackType::kSecMUM);
    RegisterTrackQa("all_k", "all K^{#pm}", ETrackType::kAllK);
    RegisterTrackQa("prim_k", "primary K^{#pm}", ETrackType::kPrimK);
    RegisterTrackQa("prim_kp", "primary K^{#plus}", ETrackType::kPrimKP);
    RegisterTrackQa("prim_km", "primary K^{#minus}", ETrackType::kPrimKM);
    RegisterTrackQa("sec_k", "secondary K^{#pm}", ETrackType::kSecK);
    RegisterTrackQa("sec_kp", "secondary K^{#plus}", ETrackType::kSecKP);
    RegisterTrackQa("sec_km", "secondary K^{#minus}", ETrackType::kSecKM);
    RegisterTrackQa("all_ppbar", "all p/#bar{p}", ETrackType::kAllPPBAR);
    RegisterTrackQa("prim_ppbar", "primary p/#bar{p}", ETrackType::kPrimPPBAR);
    RegisterTrackQa("prim_p", "primary p", ETrackType::kPrimP);
    RegisterTrackQa("prim_pbar", "primary #bar{p}", ETrackType::kPrimPBAR);
    RegisterTrackQa("sec_ppbar", "secondary p/#bar{p}", ETrackType::kSecPPBAR);
    RegisterTrackQa("sec_p", "secondary p", ETrackType::kSecP);
    RegisterTrackQa("sec_pbar", "secondary #bar{p}", ETrackType::kSecPBAR);
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

  // Match tracks and points
  // Read MC tracks and points
  if (IsMCUsed()) {
    fpMCModule->InitEvent(nullptr);
    nMCPoints = fMCData.GetNofPoints();
    nMCTracks = fMCData.GetNofTracks();
    fpMCModule->MatchRecoAndMC();
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

  LOG(info) << fpParameters->ToString(5);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::InitDrawingAttributes()
{
  fvTrackDrawAtts[ETrackType::kAll]   = {1, 20};
  fvTrackDrawAtts[ETrackType::kGhost] = {kGray, 20};
  fvTrackDrawAtts[ETrackType::kPrim]  = {kGray + 3, 21};
  fvTrackDrawAtts[ETrackType::kSec]   = {kGray + 2, 25};

  fvTrackDrawAtts[ETrackType::kAllPI]   = {kRed - 4, 20};
  fvTrackDrawAtts[ETrackType::kPrimPI]  = {kRed - 2, 21};
  fvTrackDrawAtts[ETrackType::kPrimPIP] = {kRed - 1, 22};
  fvTrackDrawAtts[ETrackType::kPrimPIM] = {kRed - 3, 23};
  fvTrackDrawAtts[ETrackType::kSecPI]   = {kRed - 8, 25};
  fvTrackDrawAtts[ETrackType::kSecPIP]  = {kRed - 6, 26};
  fvTrackDrawAtts[ETrackType::kSecPIM]  = {kRed - 10, 32};

  fvTrackDrawAtts[ETrackType::kAllK]   = {kBlue - 4, 20};
  fvTrackDrawAtts[ETrackType::kPrimK]  = {kBlue - 2, 21};
  fvTrackDrawAtts[ETrackType::kPrimKP] = {kBlue - 1, 22};
  fvTrackDrawAtts[ETrackType::kPrimKM] = {kBlue - 3, 23};
  fvTrackDrawAtts[ETrackType::kSecK]   = {kBlue - 8, 25};
  fvTrackDrawAtts[ETrackType::kSecKP]  = {kBlue - 6, 26};
  fvTrackDrawAtts[ETrackType::kSecKM]  = {kBlue - 10, 32};

  fvTrackDrawAtts[ETrackType::kAllPPBAR]  = {kGreen - 4, 20};
  fvTrackDrawAtts[ETrackType::kPrimPPBAR] = {kGreen - 2, 21};
  fvTrackDrawAtts[ETrackType::kPrimP]     = {kGreen - 1, 22};
  fvTrackDrawAtts[ETrackType::kPrimPBAR]  = {kGreen - 3, 23};
  fvTrackDrawAtts[ETrackType::kSecPPBAR]  = {kGreen - 8, 25};
  fvTrackDrawAtts[ETrackType::kSecP]      = {kGreen - 6, 26};
  fvTrackDrawAtts[ETrackType::kSecPBAR]   = {kGreen - 10, 32};

  fvTrackDrawAtts[ETrackType::kAllE]   = {kCyan - 4, 20};
  fvTrackDrawAtts[ETrackType::kPrimE]  = {kCyan - 2, 21};
  fvTrackDrawAtts[ETrackType::kPrimEP] = {kCyan - 1, 22};
  fvTrackDrawAtts[ETrackType::kPrimEM] = {kCyan - 3, 23};
  fvTrackDrawAtts[ETrackType::kSecE]   = {kCyan - 8, 25};
  fvTrackDrawAtts[ETrackType::kSecEP]  = {kCyan - 6, 26};
  fvTrackDrawAtts[ETrackType::kSecEM]  = {kCyan - 10, 32};

  fvTrackDrawAtts[ETrackType::kAllMU]   = {kMagenta - 4, 20};
  fvTrackDrawAtts[ETrackType::kPrimMU]  = {kMagenta - 2, 21};
  fvTrackDrawAtts[ETrackType::kPrimMUP] = {kMagenta - 1, 22};
  fvTrackDrawAtts[ETrackType::kPrimMUM] = {kMagenta - 3, 23};
  fvTrackDrawAtts[ETrackType::kSecMU]   = {kMagenta - 8, 25};
  fvTrackDrawAtts[ETrackType::kSecMUP]  = {kMagenta - 6, 26};
  fvTrackDrawAtts[ETrackType::kSecMUM]  = {kMagenta - 10, 32};
}
