/**
 *    file CbmKresConversionCorrectedPhotons.cxx
 *
 *    author Ievgenii Kres
 *    date 06.02.2018
 *    modified 30.01.2020
 *
 *    Class for the reconstruction of direct photons and correct them.
 *    Logic is similar to the "CbmKresConversionPhotons.cxx" + correction loop
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *
 *    One needs root files already for this analysis, what are not allowed to load in the trunk !!!
 **/

#include "CbmKresConversionCorrectedPhotons.h"
#include "CbmKresConversionBG.h"
#include "CbmKresFunctions.h"

#include "CbmGlobalTrack.h"
#include "CbmKFParticleInterface.h"
#include "CbmL1PFFitter.h"
#include "CbmMCTrack.h"
#include "CbmMvdHit.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"
#include "CbmRichRingLight.h"
#include "CbmRichUtil.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "FairRootManager.h"
#include "KFParticle.h"
#include "L1Field.h"

#include <iostream>

using namespace std;

CbmKresConversionCorrectedPhotons::CbmKresConversionCorrectedPhotons()
  : fTrainPhotons(nullptr)
  , AnnTrainPhotons(0)
  , fAnnPhotonsSelection(nullptr)
  , UseAnnPhotons(0)
  , fGammaCorrection(nullptr)
  , UseCorrection(0)
  , corr_all()
  , corr_two()
  , corr_onetwo()
  , thresholdweight(0.)
  , fMcTracks(nullptr)
  , fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fStsTrackMatches(nullptr)
  , fRichProjections(nullptr)
  , fRichRings(nullptr)
  , fRichRingMatches(nullptr)
  , fRichHits(nullptr)
  , fArrayMvdHit(nullptr)
  , fArrayStsHit(nullptr)
  , fPrimVertex(nullptr)
  , fKFVertex()
  , fTauFit(nullptr)
  , VStsTrack_minus_Outside()
  , VMCtracks_minus_Outside()
  , VRings_minus_Outside()
  , VStsIndex_minus_Outside()
  , VRichRing_minus_Outside()
  , VStsTrack_plus_Outside()
  , VMCtracks_plus_Outside()
  , VRings_plus_Outside()
  , VStsIndex_plus_Outside()
  , VRichRing_plus_Outside()
  , VMCtracks_minus_Target()
  , VStsTrack_minus_Target()
  , VMomenta_minus_Target()
  , VRings_minus_Target()
  , VStsIndex_minus_Target()
  , VRichRing_minus_Target()
  , VMCtracks_plus_Target()
  , VStsTrack_plus_Target()
  , VMomenta_plus_Target()
  , VRings_plus_Target()
  , VStsIndex_plus_Target()
  , VRichRing_plus_Target()
  , CDP_LK_EMT_momenta_minus_Target()
  , CDP_LK_EMT_NofRings_minus_Target()
  , CDP_LK_EMT_STS_minus_Target()
  , CDP_LK_EMT_STS_minus_index_Target()
  , CDP_LK_EMT_momenta_plus_Target()
  , CDP_LK_EMT_NofRings_plus_Target()
  , CDP_LK_EMT_STS_plus_Target()
  , CDP_LK_EMT_STS_plus_index_Target()
  , CDP_LK_EMT_NofRings_minus_Outside()
  , CDP_LK_EMT_STS_minus_Outside()
  , CDP_LK_EMT_STS_minus_index_Outside()
  , CDP_LK_EMT_NofRings_plus_Outside()
  , CDP_LK_EMT_STS_plus_Outside()
  , CDP_LK_EMT_STS_plus_index_Outside()
  , CDP_EMT_Event_minus_Target()
  , CDP_EMT_momenta_minus_Target()
  , CDP_EMT_NofRings_minus_Target()
  , CDP_EMT_Hits_minus_Target()
  , CDP_EMT_Event_plus_Target()
  , CDP_EMT_momenta_plus_Target()
  , CDP_EMT_NofRings_plus_Target()
  , CDP_EMT_Hits_plus_Target()
  , CDP_EMT_Event_minus_Outside()
  , CDP_EMT_momenta_minus_Outside()
  , CDP_EMT_KFTrack_minus_Outside()
  , CDP_EMT_NofRings_minus_Outside()
  , CDP_EMT_Hits_minus_Outside()
  , CDP_EMT_Event_plus_Outside()
  , CDP_EMT_momenta_plus_Outside()
  , CDP_EMT_KFTrack_plus_Outside()
  , CDP_EMT_NofRings_plus_Outside()
  , CDP_EMT_Hits_plus_Outside()
  , fHistoList_dp_Target()
  , CMother_PDG_Target(nullptr)
  , CGrandMother_PDG_Target(nullptr)
  , fHistoList_dp_Outside()
  , CMother_PDG_Outside(nullptr)
  , CGrandMother_PDG_Outside(nullptr)
  , fHistoList_dp_Both()
  , CPdg_vs_Distance_for_dp(nullptr)
  , CP_vs_Distance_for_dp(nullptr)
  , CDP_AnnTruePairs(nullptr)
  , CDP_AnnFalsePairs(nullptr)
  , CDP_AnnTruePairs_AfterCuts(nullptr)
  , CDP_AnnFalsePairs_AfterCuts(nullptr)
  , fHistoList_dp_cuts_Both()
  , CDP_candidates_InvMass_vs_OA_Both(nullptr)
  , CDP_InvMass_vs_OA_Both(nullptr)
  , CDP_candidates_InvMass_Both(nullptr)
  , CDP_InvMass_Both(nullptr)
  , CDP_candidates_OA_Both(nullptr)
  , CDP_OA_Both(nullptr)
  , CDP_candidates_PlaneAngles_last_Both(nullptr)
  , CDP_PlaneAngles_last_Both(nullptr)
  , CDP_candidates_PlaneAngles_first_Both(nullptr)
  , CDP_PlaneAngles_first_Both(nullptr)
  , fHistoList_dp_cuts_Target()
  , CDP_candidates_InvMass_vs_OA_Target(nullptr)
  , CDP_InvMass_vs_OA_Target(nullptr)
  , CDP_candidates_InvMass_Target(nullptr)
  , CDP_InvMass_Target(nullptr)
  , CDP_candidates_OA_Target(nullptr)
  , CDP_OA_Target(nullptr)
  , CDP_candidates_PlaneAngles_last_Target(nullptr)
  , CDP_PlaneAngles_last_Target(nullptr)
  , CDP_candidates_PlaneAngles_first_Target(nullptr)
  , CDP_PlaneAngles_first_Target(nullptr)
  , fHistoList_dp_cuts_Outside()
  , CDP_candidates_InvMass_vs_OA_Outside(nullptr)
  , CDP_InvMass_vs_OA_Outside(nullptr)
  , CDP_candidates_InvMass_Outside(nullptr)
  , CDP_InvMass_Outside(nullptr)
  , CDP_candidates_OA_Outside(nullptr)
  , CDP_OA_Outside(nullptr)
  , CDP_candidates_PlaneAngles_last_Outside(nullptr)
  , CDP_PlaneAngles_last_Outside(nullptr)
  , CDP_candidates_PlaneAngles_first_Outside(nullptr)
  , CDP_PlaneAngles_first_Outside(nullptr)
  , fHistoList_dp_all_Target()
  , CDP_InvMassReco_all_Target(nullptr)
  , CDP_OpeningAngleReco_all_Target(nullptr)
  , CDP_Pdg_all_Target(nullptr)
  , CDP_P_reco_all_Target(nullptr)
  , CDP_Pt_reco_all_Target(nullptr)
  , CPh_fromTarget_Pt_reco_all_Target(nullptr)
  , CPh_fromPions_Pt_reco_all_Target(nullptr)
  , CPh_fromEtas_Pt_reco_all_Target(nullptr)
  , CPh_fromDalitz_Pt_reco_all_Target(nullptr)
  , CPh_fromXi_Pt_reco_all_Target(nullptr)
  , CPh_fromOther_Pt_reco_all_Target(nullptr)
  , CPh_twoFromTarget_Pt_reco_all_Target(nullptr)
  , CPh_fromCombinatorial_Pt_reco_all_Target(nullptr)
  , CPh_fromConversion_Pt_reco_all_Target(nullptr)
  , CPh_pt_vs_rap_est_all_Target(nullptr)
  , CPh_pt_vs_rap_est_corr_all_Target(nullptr)
  , fHistoList_dp_two_Target()
  , CDP_InvMassReco_two_Target(nullptr)
  , CDP_OpeningAngleReco_two_Target(nullptr)
  , CDP_Pdg_two_Target(nullptr)
  , CDP_P_reco_two_Target(nullptr)
  , CDP_Pt_reco_two_Target(nullptr)
  , CPh_fromTarget_Pt_reco_two_Target(nullptr)
  , CPh_fromPions_Pt_reco_two_Target(nullptr)
  , CPh_fromEtas_Pt_reco_two_Target(nullptr)
  , CPh_fromDalitz_Pt_reco_two_Target(nullptr)
  , CPh_fromXi_Pt_reco_two_Target(nullptr)
  , CPh_fromOther_Pt_reco_two_Target(nullptr)
  , CPh_twoFromTarget_Pt_reco_two_Target(nullptr)
  , CPh_fromCombinatorial_Pt_reco_two_Target(nullptr)
  , CPh_fromConversion_Pt_reco_two_Target(nullptr)
  , CPh_pt_vs_rap_est_two_Target(nullptr)
  , CPh_pt_vs_rap_est_corr_two_Target(nullptr)
  , fHistoList_dp_onetwo_Target()
  , CDP_InvMassReco_onetwo_Target(nullptr)
  , CDP_OpeningAngleReco_onetwo_Target(nullptr)
  , CDP_Pdg_onetwo_Target(nullptr)
  , CDP_P_reco_onetwo_Target(nullptr)
  , CDP_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromTarget_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromPions_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromEtas_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromDalitz_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromXi_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromOther_Pt_reco_onetwo_Target(nullptr)
  , CPh_twoFromTarget_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromCombinatorial_Pt_reco_onetwo_Target(nullptr)
  , CPh_fromConversion_Pt_reco_onetwo_Target(nullptr)
  , CPh_pt_vs_rap_est_onetwo_Target(nullptr)
  , CPh_pt_vs_rap_est_corr_onetwo_Target(nullptr)
  , fHistoList_dp_all_Outside()
  , CDP_InvMassReco_all_Outside(nullptr)
  , CDP_OpeningAngleReco_all_Outside(nullptr)
  , CDP_Pdg_all_Outside(nullptr)
  , CDP_P_reco_all_Outside(nullptr)
  , CDP_Pt_reco_all_Outside(nullptr)
  , CPh_fromTarget_Pt_reco_all_Outside(nullptr)
  , CPh_fromPions_Pt_reco_all_Outside(nullptr)
  , CPh_fromEtas_Pt_reco_all_Outside(nullptr)
  , CPh_fromDalitz_Pt_reco_all_Outside(nullptr)
  , CPh_fromXi_Pt_reco_all_Outside(nullptr)
  , CPh_fromOther_Pt_reco_all_Outside(nullptr)
  , CPh_twoFromTarget_Pt_reco_all_Outside(nullptr)
  , CPh_fromCombinatorial_Pt_reco_all_Outside(nullptr)
  , CPh_fromConversion_Pt_reco_all_Outside(nullptr)
  , CPh_pt_vs_rap_est_all_Outside(nullptr)
  , CPh_pt_vs_rap_est_corr_all_Outside(nullptr)
  , fHistoList_dp_two_Outside()
  , CDP_InvMassReco_two_Outside(nullptr)
  , CDP_OpeningAngleReco_two_Outside(nullptr)
  , CDP_Pdg_two_Outside(nullptr)
  , CDP_P_reco_two_Outside(nullptr)
  , CDP_Pt_reco_two_Outside(nullptr)
  , CPh_fromTarget_Pt_reco_two_Outside(nullptr)
  , CPh_fromPions_Pt_reco_two_Outside(nullptr)
  , CPh_fromEtas_Pt_reco_two_Outside(nullptr)
  , CPh_fromDalitz_Pt_reco_two_Outside(nullptr)
  , CPh_fromXi_Pt_reco_two_Outside(nullptr)
  , CPh_fromOther_Pt_reco_two_Outside(nullptr)
  , CPh_twoFromTarget_Pt_reco_two_Outside(nullptr)
  , CPh_fromCombinatorial_Pt_reco_two_Outside(nullptr)
  , CPh_fromConversion_Pt_reco_two_Outside(nullptr)
  , CPh_pt_vs_rap_est_two_Outside(nullptr)
  , CPh_pt_vs_rap_est_corr_two_Outside(nullptr)
  , fHistoList_dp_onetwo_Outside()
  , CDP_InvMassReco_onetwo_Outside(nullptr)
  , CDP_OpeningAngleReco_onetwo_Outside(nullptr)
  , CDP_Pdg_onetwo_Outside(nullptr)
  , CDP_P_reco_onetwo_Outside(nullptr)
  , CDP_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromTarget_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromPions_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromEtas_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromDalitz_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromXi_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromOther_Pt_reco_onetwo_Outside(nullptr)
  , CPh_twoFromTarget_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromCombinatorial_Pt_reco_onetwo_Outside(nullptr)
  , CPh_fromConversion_Pt_reco_onetwo_Outside(nullptr)
  , CPh_pt_vs_rap_est_onetwo_Outside(nullptr)
  , CPh_pt_vs_rap_est_corr_onetwo_Outside(nullptr)
  , fHistoList_dp_all_Both()
  , CDP_InvMassReco_all_Both(nullptr)
  , CDP_OpeningAngleReco_all_Both(nullptr)
  , CDP_Pdg_all_Both(nullptr)
  , CDP_P_reco_all_Both(nullptr)
  , CDP_Pt_reco_all_Both(nullptr)
  , CPh_fromTarget_Pt_reco_all_Both(nullptr)
  , CPh_fromPions_Pt_reco_all_Both(nullptr)
  , CPh_fromEtas_Pt_reco_all_Both(nullptr)
  , CPh_fromDalitz_Pt_reco_all_Both(nullptr)
  , CPh_fromXi_Pt_reco_all_Both(nullptr)
  , CPh_fromOther_Pt_reco_all_Both(nullptr)
  , CPh_twoFromTarget_Pt_reco_all_Both(nullptr)
  , CPh_fromCombinatorial_Pt_reco_all_Both(nullptr)
  , CPh_fromConversion_Pt_reco_all_Both(nullptr)
  , CPh_pt_vs_rap_est_all_Both(nullptr)
  , CPh_pt_vs_rap_est_corr_all_Both(nullptr)
  , fHistoList_dp_two_Both()
  , CDP_InvMassReco_two_Both(nullptr)
  , CDP_OpeningAngleReco_two_Both(nullptr)
  , CDP_Pdg_two_Both(nullptr)
  , CDP_P_reco_two_Both(nullptr)
  , CDP_Pt_reco_two_Both(nullptr)
  , CPh_fromTarget_Pt_reco_two_Both(nullptr)
  , CPh_fromPions_Pt_reco_two_Both(nullptr)
  , CPh_fromEtas_Pt_reco_two_Both(nullptr)
  , CPh_fromDalitz_Pt_reco_two_Both(nullptr)
  , CPh_fromXi_Pt_reco_two_Both(nullptr)
  , CPh_fromOther_Pt_reco_two_Both(nullptr)
  , CPh_twoFromTarget_Pt_reco_two_Both(nullptr)
  , CPh_fromCombinatorial_Pt_reco_two_Both(nullptr)
  , CPh_fromConversion_Pt_reco_two_Both(nullptr)
  , CPh_pt_vs_rap_est_two_Both(nullptr)
  , CPh_pt_vs_rap_est_corr_two_Both(nullptr)
  , fHistoList_dp_onetwo_Both()
  , CDP_InvMassReco_onetwo_Both(nullptr)
  , CDP_OpeningAngleReco_onetwo_Both(nullptr)
  , CDP_Pdg_onetwo_Both(nullptr)
  , CDP_P_reco_onetwo_Both(nullptr)
  , CDP_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromTarget_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromPions_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromEtas_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromDalitz_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromXi_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromOther_Pt_reco_onetwo_Both(nullptr)
  , CPh_twoFromTarget_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromCombinatorial_Pt_reco_onetwo_Both(nullptr)
  , CPh_fromConversion_Pt_reco_onetwo_Both(nullptr)
  , CPh_pt_vs_rap_est_onetwo_Both(nullptr)
  , CPh_pt_vs_rap_est_corr_onetwo_Both(nullptr)
  , CDP_EMT_Pt_all_Target(nullptr)
  , CDP_EMT_Pt_two_Target(nullptr)
  , CDP_EMT_Pt_onetwo_Target(nullptr)
  , CDP_EMT_Pt_all_Outside(nullptr)
  , CDP_EMT_Pt_two_Outside(nullptr)
  , CDP_EMT_Pt_onetwo_Outside(nullptr)
  , CDP_EMT_Pt_all_Both(nullptr)
  , CDP_EMT_Pt_two_Both(nullptr)
  , CDP_EMT_Pt_onetwo_Both(nullptr)
  , CDP_LK_EMT_Pt_all_Target(nullptr)
  , CDP_LK_EMT_Pt_two_Target(nullptr)
  , CDP_LK_EMT_Pt_onetwo_Target(nullptr)
  , CDP_LK_EMT_Pt_all_Outside(nullptr)
  , CDP_LK_EMT_Pt_two_Outside(nullptr)
  , CDP_LK_EMT_Pt_onetwo_Outside(nullptr)
  , CDP_LK_EMT_Pt_all_Both(nullptr)
  , CDP_LK_EMT_Pt_two_Both(nullptr)
  , CDP_LK_EMT_Pt_onetwo_Both(nullptr) {}

CbmKresConversionCorrectedPhotons::~CbmKresConversionCorrectedPhotons() {}

void CbmKresConversionCorrectedPhotons::Init(double OA, double IM) {
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    Fatal("CbmKresConversionCorrectedPhotons::Init",
          "RootManager not instantised!");
  }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No MCTrack array!");
  }

  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) {
    fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
  }
  if (nullptr == fPrimVertex) {
    LOG(fatal)
      << "CbmKresConversionCorrectedPhotons::Init  No PrimaryVertex array!";
  }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No GlobalTrack array!");
  }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No StsTrack array!");
  }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No StsTrackMatch array!");
  }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) {
    Fatal("CbmKresConversionCorrectedPhotons::Init",
          "No RichProjection array!");
  }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No RichRing array!");
  }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No RichRingMatch array!");
  }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No RichHit array!");
  }

  fArrayMvdHit = (TClonesArray*) ioman->GetObject("MvdHit");
  if (nullptr == fArrayMvdHit) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No MvdHit array!");
  }

  fArrayStsHit = (TClonesArray*) ioman->GetObject("StsHit");
  if (nullptr == fArrayStsHit) {
    Fatal("CbmKresConversionCorrectedPhotons::Init", "No StsHit array!");
  }


  fTauFit = new CbmRichRingFitterEllipseTau();

  InitHistograms();

  thresholdweight = 500;

  UseCorrection   = 1;
  AnnTrainPhotons = 0;

  UseAnnPhotons = 1 - AnnTrainPhotons;

  if (AnnTrainPhotons == 1) {
    fTrainPhotons = new CbmKresTrainAnnDirectPhotons();
    fTrainPhotons->Init();
    cout << endl;
    cout << "\t *** CbmKresTrainAnnDirectPhotons                      ==>  is "
            "activated"
         << endl;
  }


  if (UseAnnPhotons == 1) {
    fAnnPhotonsSelection = new CbmKresSelectAnnPhotons();
    fAnnPhotonsSelection->Init();
    cout << endl;
    cout << "\t *** CbmKresSelectAnnPhotons                           ==>  is "
            "activated"
         << endl;
  }

  if (UseCorrection == 1) {
    fGammaCorrection = new CbmKresGammaCorrection();
    fGammaCorrection->Init(corr_all, corr_two, corr_onetwo, OA, IM);
    cout << endl;
    cout << "\t *** CbmKresGammaCorrection                             ==>  is "
            "activated"
         << endl;
  }
}


void CbmKresConversionCorrectedPhotons::Exec(int fEventNumDP,
                                             double OpeningAngleCut,
                                             double GammaInvMassCut,
                                             int RealPID) {
  cout << "CbmKresConversionCorrectedPhotons, event No. " << fEventNumDP
       << endl;

  if (fPrimVertex != nullptr) {
    fKFVertex = CbmKFVertex(*fPrimVertex);
  } else {
    Fatal("CbmKresConversionCorrectedPhotons::Exec", "No PrimaryVertex array!");
  }

  VMCtracks_minus_Outside.clear();
  VStsTrack_minus_Outside.clear();
  VRings_minus_Outside.clear();
  VStsIndex_minus_Outside.clear();
  VRichRing_minus_Outside.clear();
  VMCtracks_plus_Outside.clear();
  VStsTrack_plus_Outside.clear();
  VRings_plus_Outside.clear();
  VStsIndex_plus_Outside.clear();
  VRichRing_plus_Outside.clear();

  VMCtracks_minus_Target.clear();
  VMomenta_minus_Target.clear();
  VStsTrack_minus_Target.clear();
  VRings_minus_Target.clear();
  VStsIndex_minus_Target.clear();
  VRichRing_minus_Target.clear();
  VMCtracks_plus_Target.clear();
  VStsTrack_plus_Target.clear();
  VMomenta_plus_Target.clear();
  VRings_plus_Target.clear();
  VStsIndex_plus_Target.clear();
  VRichRing_plus_Target.clear();


  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t i = 0; i < ngTracks; i++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
    if (nullptr == gTrack) continue;
    Int_t stsInd  = gTrack->GetStsTrackIndex();
    Int_t richInd = gTrack->GetRichRingIndex();

    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == nullptr) continue;
    CbmTrackMatchNew* stsMatch =
      (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == nullptr) continue;
    if (stsMatch->GetNofLinks() <= 0) continue;
    Int_t stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
    if (mcTrack == nullptr) continue;


    FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(i);
    if (richInd < 0 && proj->GetX() > -115 && proj->GetX() < 115
        && ((proj->GetY() < -120 && proj->GetY() > -200)
            || (proj->GetY() > 120 && proj->GetY() < 200)))
      continue;
    CbmRichRing* Ring = nullptr;
    if (richInd > -1) {
      Ring = static_cast<CbmRichRing*>(fRichRings->At(richInd));
      CPdg_vs_Distance_for_dp->Fill(TMath::Abs(mcTrack->GetPdgCode()),
                                    CbmRichUtil::GetRingTrackDistance(i));
      if (TMath::Abs(mcTrack->GetPdgCode()) == 11)
        CP_vs_Distance_for_dp->Fill(mcTrack->GetP(),
                                    CbmRichUtil::GetRingTrackDistance(i));
    }

    // Doing fit with Fit To primary Vertex and calculate chi2 to primary vertex
    double chi2 = 0;
    TVector3 Momentum =
      CbmKresFunctions::FitToVertexAndGetChi(stsTrack,
                                             fKFVertex.GetRefX(),
                                             fKFVertex.GetRefY(),
                                             fKFVertex.GetRefZ(),
                                             chi2);
    const FairTrackParam* track_par = stsTrack->GetParamFirst();
    double charge                   = track_par->GetQp();


    if (chi2 != chi2) continue;
    if (chi2 == 0) continue;

    if (chi2 > 3) {
      SaveOutsideTracks(
        mcTrack, stsTrack, charge, stsInd, richInd, stsMcTrackId, Ring);
    }
    if (chi2 > 3) continue;

    SaveTargetTracks(
      mcTrack, stsTrack, Momentum, charge, stsInd, richInd, stsMcTrackId, Ring);
  }


  FindGammasTarget(fEventNumDP,
                   OpeningAngleCut,
                   GammaInvMassCut,
                   RealPID,
                   VMCtracks_minus_Target,
                   VMCtracks_plus_Target,
                   VStsTrack_minus_Target,
                   VStsTrack_plus_Target,
                   VMomenta_minus_Target,
                   VMomenta_plus_Target,
                   VRings_minus_Target,
                   VRings_plus_Target,
                   VStsIndex_minus_Target,
                   VStsIndex_plus_Target,
                   VRichRing_minus_Target,
                   VRichRing_plus_Target);

  FindGammasOutside(fEventNumDP,
                    OpeningAngleCut,
                    GammaInvMassCut,
                    RealPID,
                    VMCtracks_minus_Outside,
                    VMCtracks_plus_Outside,
                    VStsTrack_minus_Outside,
                    VStsTrack_plus_Outside,
                    VRings_minus_Outside,
                    VRings_plus_Outside,
                    VStsIndex_minus_Outside,
                    VStsIndex_plus_Outside,
                    VRichRing_minus_Outside,
                    VRichRing_plus_Outside);


  if (UseAnnPhotons == 1) {
    int numformix = 1000;
    if (fEventNumDP % numformix == 0) {
      CDP_Mixing_Target(OpeningAngleCut, GammaInvMassCut);
      CDP_EMT_Event_minus_Target.clear();
      CDP_EMT_momenta_minus_Target.clear();
      CDP_EMT_NofRings_minus_Target.clear();
      CDP_EMT_Hits_minus_Target.clear();

      CDP_EMT_Event_plus_Target.clear();
      CDP_EMT_momenta_plus_Target.clear();
      CDP_EMT_NofRings_plus_Target.clear();
      CDP_EMT_Hits_plus_Target.clear();
    }

    if (fEventNumDP % numformix == 0) {
      CDP_Mixing_Outside(OpeningAngleCut, GammaInvMassCut);
      CDP_EMT_Event_minus_Outside.clear();
      CDP_EMT_momenta_minus_Outside.clear();
      CDP_EMT_KFTrack_minus_Outside.clear();
      CDP_EMT_NofRings_minus_Outside.clear();
      CDP_EMT_Hits_minus_Outside.clear();

      CDP_EMT_Event_plus_Outside.clear();
      CDP_EMT_momenta_plus_Outside.clear();
      CDP_EMT_KFTrack_plus_Outside.clear();
      CDP_EMT_NofRings_plus_Outside.clear();
      CDP_EMT_Hits_plus_Outside.clear();
    }

    // CDP_LK_EMT Target
    CDP_likesign_Mixing_Target(OpeningAngleCut, GammaInvMassCut);
    CDP_LK_EMT_momenta_minus_Target.clear();
    CDP_LK_EMT_NofRings_minus_Target.clear();
    CDP_LK_EMT_STS_minus_Target.clear();
    CDP_LK_EMT_STS_minus_index_Target.clear();
    CDP_LK_EMT_momenta_plus_Target.clear();
    CDP_LK_EMT_NofRings_plus_Target.clear();
    CDP_LK_EMT_STS_plus_Target.clear();
    CDP_LK_EMT_STS_plus_index_Target.clear();

    // CDP_LK_EMT Outside
    CDP_likesign_Mixing_Outside(OpeningAngleCut, GammaInvMassCut);
    CDP_LK_EMT_NofRings_minus_Outside.clear();
    CDP_LK_EMT_STS_minus_Outside.clear();
    CDP_LK_EMT_STS_minus_index_Outside.clear();
    CDP_LK_EMT_NofRings_plus_Outside.clear();
    CDP_LK_EMT_STS_plus_Outside.clear();
    CDP_LK_EMT_STS_plus_index_Outside.clear();
  }
}


void CbmKresConversionCorrectedPhotons::SaveOutsideTracks(CbmMCTrack* mcTrack1,
                                                          CbmStsTrack* stsTrack,
                                                          double charge,
                                                          int stsInd,
                                                          int richInd,
                                                          int stsMcTrackId,
                                                          CbmRichRing* RING) {
  int InRich = FindInRich(richInd, stsMcTrackId);
  if (charge < 0) {
    VMCtracks_minus_Outside.push_back(mcTrack1);
    VStsTrack_minus_Outside.push_back(stsTrack);
    VRings_minus_Outside.push_back(InRich);
    VStsIndex_minus_Outside.push_back(stsInd);
    VRichRing_minus_Outside.push_back(RING);
  }
  if (charge > 0) {
    VMCtracks_plus_Outside.push_back(mcTrack1);
    VStsTrack_plus_Outside.push_back(stsTrack);
    VRings_plus_Outside.push_back(InRich);
    VStsIndex_plus_Outside.push_back(stsInd);
    VRichRing_plus_Outside.push_back(RING);
  }
}

void CbmKresConversionCorrectedPhotons::SaveTargetTracks(CbmMCTrack* mcTrack1,
                                                         CbmStsTrack* stsTrack,
                                                         TVector3 refmom,
                                                         double charge,
                                                         int stsInd,
                                                         int richInd,
                                                         int stsMcTrackId,
                                                         CbmRichRing* RING) {
  int InRich = FindInRich(richInd, stsMcTrackId);
  if (charge < 0) {
    VMCtracks_minus_Target.push_back(mcTrack1);
    VStsTrack_minus_Target.push_back(stsTrack);
    VMomenta_minus_Target.push_back(refmom);
    VRings_minus_Target.push_back(InRich);
    VStsIndex_minus_Target.push_back(stsInd);
    VRichRing_minus_Target.push_back(RING);
  }
  if (charge > 0) {
    VMCtracks_plus_Target.push_back(mcTrack1);
    VStsTrack_plus_Target.push_back(stsTrack);
    VMomenta_plus_Target.push_back(refmom);
    VRings_plus_Target.push_back(InRich);
    VStsIndex_plus_Target.push_back(stsInd);
    VRichRing_plus_Target.push_back(RING);
  }
}


void CbmKresConversionCorrectedPhotons::FindGammasTarget(
  int EventNumMan,
  double AngleCut,
  double InvMassCut,
  int RealPID,
  vector<CbmMCTrack*> MCtracks_minus,
  vector<CbmMCTrack*> MCtracks_plus,
  vector<CbmStsTrack*> StsTrack_minus,
  vector<CbmStsTrack*> StsTrack_plus,
  vector<TVector3> Momenta_minus,
  vector<TVector3> Momenta_plus,
  std::vector<int> Rings_minus,
  std::vector<int> Rings_plus,
  std::vector<int> stsIndex_minus,
  std::vector<int> stsIndex_plus,
  vector<CbmRichRing*> richRing_minus,
  vector<CbmRichRing*> richRing_plus) {
  for (size_t i = 0; i < Momenta_minus.size(); i++) {
    for (size_t j = 0; j < Momenta_plus.size(); j++) {

      CbmStsTrack* sts1 = StsTrack_minus.at(i);
      CbmStsTrack* sts2 = StsTrack_plus.at(j);

      TVector3 part1      = Momenta_minus[i];
      TVector3 part2      = Momenta_plus[j];
      CbmMCTrack* part1MC = MCtracks_minus[i];
      CbmMCTrack* part2MC = MCtracks_plus[j];
      int sts1_index      = stsIndex_minus[i];
      int sts2_index      = stsIndex_plus[j];

      int richcheck_0 = 0;
      int richcheck_1 = 0;
      if (RealPID == 1) {
        // Real PID
        richcheck_0 =
          CheckIfElectron(richRing_minus[i], Momenta_minus[i].Mag());
        richcheck_1 = CheckIfElectron(richRing_plus[j], Momenta_plus[j].Mag());
      } else {
        // MC   PID
        richcheck_0 = Rings_minus[i];
        richcheck_1 = Rings_plus[j];
      }
      int richcheck = richcheck_0 + richcheck_1;

      Double_t InvmassReco =
        CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step = 0.4;
      double pt_step  = 0.1;
      int rap_coef    = params.fRapidity / rap_step;
      int pt_coef     = params.fPt / pt_step;

      CDP_candidates_InvMass_vs_OA_Target->Fill(InvmassReco, OpeningAngle);
      CDP_candidates_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
      CDP_candidates_InvMass_Target->Fill(InvmassReco);
      CDP_candidates_InvMass_Both->Fill(InvmassReco);
      CDP_candidates_OA_Target->Fill(OpeningAngle);
      CDP_candidates_OA_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(sts1, sts2);
      double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);

      int IdForANN =
        0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() == -1
            && mcTrackmama->GetPdgCode() == 22) {
          IdForANN = 1;
          CDP_InvMass_Target->Fill(InvmassReco);
          CDP_InvMass_Both->Fill(InvmassReco);
          CDP_OA_Target->Fill(OpeningAngle);
          CDP_OA_Both->Fill(OpeningAngle);
          CDP_InvMass_vs_OA_Target->Fill(InvmassReco, OpeningAngle);
          CDP_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
          CDP_PlaneAngles_last_Target->Fill(PlaneAngle_last);
          CDP_PlaneAngles_last_Both->Fill(PlaneAngle_last);
          CDP_PlaneAngles_first_Target->Fill(PlaneAngle_first);
          CDP_PlaneAngles_first_Both->Fill(PlaneAngle_first);
        }
      }

      // run ANN
      if (AnnTrainPhotons == 1) {
        fTrainPhotons->Exec(EventNumMan,
                            IdForANN,
                            InvmassReco,
                            OpeningAngle,
                            PlaneAngle_last,
                            fKFVertex.GetRefZ(),
                            part1,
                            part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnnPhotons == 1) {
        AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco,
                                                  OpeningAngle,
                                                  PlaneAngle_last,
                                                  fKFVertex.GetRefZ(),
                                                  part1,
                                                  part2);
        if (IdForANN == 1) CDP_AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) CDP_AnnFalsePairs->Fill(AnnValue);
      }


      CDP_candidates_PlaneAngles_last_Target->Fill(PlaneAngle_last);
      CDP_candidates_PlaneAngles_last_Both->Fill(PlaneAngle_last);
      CDP_candidates_PlaneAngles_first_Target->Fill(PlaneAngle_first);
      CDP_candidates_PlaneAngles_first_Both->Fill(PlaneAngle_first);


      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      int fromPions         = 0;
      int fromEta           = 0;
      int fromFireball      = 0;
      int fromDalitz        = 0;
      int fromXi            = 0;
      int fromOther         = 0;
      int fromCombinatorial = 0;
      int fromConversion    = 0;
      int twoFromTarget     = 0;
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) {
            fromFireball = 1;
          } else {
            CbmMCTrack* mcTrackGrmama =
              (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            if (mcTrackGrmama->GetPdgCode() == 111) fromPions = 1;
            if (mcTrackGrmama->GetPdgCode() == 221) fromEta = 1;
            if (mcTrackGrmama->GetPdgCode() == 3212) fromXi = 1;
            fromConversion = 1;
          }
        } else {
          if (mcTrackmama->GetPdgCode() == 111) fromDalitz = 1;
          if (mcTrackmama->GetPdgCode() != 111) fromOther = 1;
        }
      } else if (part1MC->GetMotherId() == part2MC->GetMotherId()
                 && part1MC->GetMotherId() == -1) {
        twoFromTarget = 1;
      } else {
        fromCombinatorial = 1;
      }


      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        CMother_PDG_Target->Fill(mcTrackmama->GetPdgCode());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) {
            CGrandMother_PDG_Target->Fill(mcTrackmama->GetMotherId());
          } else {
            CbmMCTrack* mcTrackGrmama =
              (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            CGrandMother_PDG_Target->Fill(
              TMath::Abs(mcTrackGrmama->GetPdgCode()));
          }
        }
      }


      if (IdForANN == 1) CDP_AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) CDP_AnnFalsePairs_AfterCuts->Fill(AnnValue);


      // for event mixing CDP
      std::vector<TVector3> minusTrack = SaveAllHits(sts1);
      std::vector<TVector3> plusTrack  = SaveAllHits(sts2);

      CDP_EMT_Event_minus_Target.push_back(EventNumMan);
      CDP_EMT_momenta_minus_Target.push_back(part1);
      CDP_EMT_NofRings_minus_Target.push_back(richcheck_0);
      CDP_EMT_Hits_minus_Target.push_back(minusTrack);

      CDP_EMT_Event_plus_Target.push_back(EventNumMan);
      CDP_EMT_momenta_plus_Target.push_back(part2);
      CDP_EMT_NofRings_plus_Target.push_back(richcheck_1);
      CDP_EMT_Hits_plus_Target.push_back(plusTrack);

      // CDP_LK_EMT
      CDP_LK_EMT_momenta_minus_Target.push_back(part1);
      CDP_LK_EMT_NofRings_minus_Target.push_back(richcheck_0);
      CDP_LK_EMT_STS_minus_Target.push_back(sts1);
      CDP_LK_EMT_STS_minus_index_Target.push_back(sts1_index);

      CDP_LK_EMT_momenta_plus_Target.push_back(part2);
      CDP_LK_EMT_NofRings_plus_Target.push_back(richcheck_1);
      CDP_LK_EMT_STS_plus_Target.push_back(sts2);
      CDP_LK_EMT_STS_plus_index_Target.push_back(sts2_index);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        double weight_all = 0;
        if (corr_all[rap_coef][pt_coef] != 0)
          weight_all = 1 / corr_all[rap_coef][pt_coef];
        if (weight_all > thresholdweight) weight_all = 0;
        CDP_InvMassReco_all_Target->Fill(InvmassReco, weight_all);
        CDP_OpeningAngleReco_all_Target->Fill(OpeningAngle, weight_all);
        CDP_Pdg_all_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_all_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_all_Target->Fill(params.fMomentumMag, weight_all);
        CDP_Pt_reco_all_Target->Fill(params.fPt, weight_all);
        CDP_InvMassReco_all_Both->Fill(InvmassReco, weight_all);
        CDP_OpeningAngleReco_all_Both->Fill(OpeningAngle, weight_all);
        CDP_Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_all_Both->Fill(params.fMomentumMag, weight_all);
        CDP_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromTarget_Pt_reco_all_Both->Fill(params.fPt, weight_all);
          CPh_pt_vs_rap_est_all_Target->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_all_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_all_Target->Fill(
            params.fRapidity, params.fPt, weight_all);
          CPh_pt_vs_rap_est_corr_all_Both->Fill(
            params.fRapidity, params.fPt, weight_all);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromPions_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromEtas_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromDalitz_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromXi_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromOther_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_all_Target->Fill(params.fPt,
                                                         weight_all);
          CPh_fromCombinatorial_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_fromConversion_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_all_Target->Fill(params.fPt, weight_all);
          CPh_twoFromTarget_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        double weight_two = 0;
        if (corr_two[rap_coef][pt_coef] != 0)
          weight_two = 1 / corr_two[rap_coef][pt_coef];
        if (weight_two > thresholdweight) weight_two = 0;
        CDP_InvMassReco_two_Target->Fill(InvmassReco, weight_two);
        CDP_OpeningAngleReco_two_Target->Fill(OpeningAngle, weight_two);
        CDP_Pdg_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_two_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_two_Target->Fill(params.fMomentumMag, weight_two);
        CDP_Pt_reco_two_Target->Fill(params.fPt, weight_two);
        CDP_InvMassReco_two_Both->Fill(InvmassReco, weight_two);
        CDP_OpeningAngleReco_two_Both->Fill(OpeningAngle, weight_two);
        CDP_Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_two_Both->Fill(params.fMomentumMag, weight_two);
        CDP_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromTarget_Pt_reco_two_Both->Fill(params.fPt, weight_two);
          CPh_pt_vs_rap_est_two_Target->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_two_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_two_Target->Fill(
            params.fRapidity, params.fPt, weight_two);
          CPh_pt_vs_rap_est_corr_two_Both->Fill(
            params.fRapidity, params.fPt, weight_two);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromPions_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromEtas_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromDalitz_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromXi_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromOther_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_two_Target->Fill(params.fPt,
                                                         weight_two);
          CPh_fromCombinatorial_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_fromConversion_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_two_Target->Fill(params.fPt, weight_two);
          CPh_twoFromTarget_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        double weight_onetwo = 0;
        if (corr_onetwo[rap_coef][pt_coef] != 0)
          weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];
        if (weight_onetwo > thresholdweight) weight_onetwo = 0;
        // cout << "Rapidity = " << params.fRapidity << "; Pt = " << params.fPt << endl;
        // cout << "rap_coef = " << rap_coef << "; pt_coef = " << pt_coef << "; correction factor is " <<  corr_onetwo[rap_coef][pt_coef] << endl;
        CDP_InvMassReco_onetwo_Target->Fill(InvmassReco, weight_onetwo);
        CDP_OpeningAngleReco_onetwo_Target->Fill(OpeningAngle, weight_onetwo);
        CDP_Pdg_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_onetwo_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_onetwo_Target->Fill(params.fMomentumMag, weight_onetwo);
        CDP_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
        CDP_InvMassReco_onetwo_Both->Fill(InvmassReco, weight_onetwo);
        CDP_OpeningAngleReco_onetwo_Both->Fill(OpeningAngle, weight_onetwo);
        CDP_Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_onetwo_Both->Fill(params.fMomentumMag, weight_onetwo);
        CDP_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromTarget_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
          CPh_pt_vs_rap_est_onetwo_Target->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_onetwo_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_onetwo_Target->Fill(
            params.fRapidity, params.fPt, weight_onetwo);
          CPh_pt_vs_rap_est_corr_onetwo_Both->Fill(
            params.fRapidity, params.fPt, weight_onetwo);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromPions_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromEtas_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromDalitz_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromXi_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_onetwo_Target->Fill(params.fPt, weight_onetwo);
          CPh_fromOther_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_onetwo_Target->Fill(params.fPt,
                                                            weight_onetwo);
          CPh_fromCombinatorial_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                          weight_onetwo);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_onetwo_Target->Fill(params.fPt,
                                                         weight_onetwo);
          CPh_fromConversion_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                       weight_onetwo);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_onetwo_Target->Fill(params.fPt,
                                                        weight_onetwo);
          CPh_twoFromTarget_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                      weight_onetwo);
        }
      }
    }
  }
}


void CbmKresConversionCorrectedPhotons::FindGammasOutside(
  int EventNumMan,
  double AngleCut,
  double InvMassCut,
  int RealPID,
  vector<CbmMCTrack*> MCtracks_minus_Outside,
  vector<CbmMCTrack*> MCtracks_plus_Outside,
  vector<CbmStsTrack*> StsTrack_minus_Outside,
  vector<CbmStsTrack*> StsTrack_plus_Outside,
  std::vector<int> Rings_minus_Outside,
  std::vector<int> Rings_plus_Outside,
  std::vector<int> stsIndex_minus_Outside,
  std::vector<int> /*stsIndex_plus_Outside*/,
  vector<CbmRichRing*> richRing_minus_Outside,
  vector<CbmRichRing*> richRing_plus_Outside) {
  for (size_t i = 0; i < StsTrack_minus_Outside.size(); i++) {
    for (size_t j = 0; j < StsTrack_plus_Outside.size(); j++) {

      CbmStsTrack* part1STS = StsTrack_minus_Outside[i];
      CbmStsTrack* part2STS = StsTrack_plus_Outside[j];
      CbmMCTrack* part1MC   = MCtracks_minus_Outside[i];
      CbmMCTrack* part2MC   = MCtracks_plus_Outside[j];
      int sts1_index        = stsIndex_minus_Outside[i];
      //			int sts2_index = stsIndex_plus_Outside[j];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(
        part1STS, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(
        part2STS, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5)
        continue;  // kick weird intersections

      // fit to the vertex fitter
      TVector3 part1 = CbmKresFunctions::FitToVertex(part1STS,
                                                     intersection.GetX(),
                                                     intersection.GetY(),
                                                     intersection.GetZ());
      TVector3 part2 = CbmKresFunctions::FitToVertex(part2STS,
                                                     intersection.GetX(),
                                                     intersection.GetY(),
                                                     intersection.GetZ());

      // TVector3 part1(electron.KFParticleBase::Px(), electron.KFParticleBase::Py(), electron.KFParticleBase::Pz());
      // TVector3 part2(positron.KFParticleBase::Px(), positron.KFParticleBase::Py(), positron.KFParticleBase::Pz());

      int richcheck   = 0;
      int richcheck_0 = 0;
      int richcheck_1 = 0;
      if (RealPID == 1) {
        // Real PID
        richcheck_0 = CheckIfElectron(richRing_minus_Outside[i], part1.Mag());
        richcheck_1 = CheckIfElectron(richRing_plus_Outside[j], part2.Mag());
        richcheck   = richcheck_0 + richcheck_1;
      } else {
        // MC   PID
        richcheck = Rings_minus_Outside[i] + Rings_plus_Outside[j];
      }


      Double_t InvmassReco =
        CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step = 0.4;
      double pt_step  = 0.1;
      int rap_coef    = params.fRapidity / rap_step;
      int pt_coef     = params.fPt / pt_step;

      CDP_candidates_InvMass_vs_OA_Outside->Fill(InvmassReco, OpeningAngle);
      CDP_candidates_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
      CDP_candidates_InvMass_Outside->Fill(InvmassReco);
      CDP_candidates_InvMass_Both->Fill(InvmassReco);
      CDP_candidates_OA_Outside->Fill(OpeningAngle);
      CDP_candidates_OA_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(part1STS, part2STS);
      double PlaneAngle_first = CalculatePlaneAngle_first(part1STS, part2STS);

      int IdForANN =
        0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() == -1
            && mcTrackmama->GetPdgCode() == 22) {
          IdForANN = 1;
          CDP_InvMass_Outside->Fill(InvmassReco);
          CDP_InvMass_Both->Fill(InvmassReco);
          CDP_OA_Outside->Fill(OpeningAngle);
          CDP_OA_Both->Fill(OpeningAngle);
          CDP_InvMass_vs_OA_Outside->Fill(InvmassReco, OpeningAngle);
          CDP_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
          CDP_PlaneAngles_last_Outside->Fill(PlaneAngle_last);
          CDP_PlaneAngles_last_Both->Fill(PlaneAngle_last);
          CDP_PlaneAngles_first_Outside->Fill(PlaneAngle_first);
          CDP_PlaneAngles_first_Both->Fill(PlaneAngle_first);
        }
      }

      // run ANN
      if (AnnTrainPhotons == 1) {
        fTrainPhotons->Exec(EventNumMan,
                            IdForANN,
                            InvmassReco,
                            OpeningAngle,
                            PlaneAngle_last,
                            intersection.GetZ(),
                            part1,
                            part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnnPhotons == 1) {
        AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco,
                                                  OpeningAngle,
                                                  PlaneAngle_last,
                                                  intersection.GetZ(),
                                                  part1,
                                                  part2);
        if (IdForANN == 1) CDP_AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) CDP_AnnFalsePairs->Fill(AnnValue);
      }

      CDP_candidates_PlaneAngles_last_Outside->Fill(PlaneAngle_last);
      CDP_candidates_PlaneAngles_last_Both->Fill(PlaneAngle_last);
      CDP_candidates_PlaneAngles_first_Outside->Fill(PlaneAngle_first);
      CDP_candidates_PlaneAngles_first_Both->Fill(PlaneAngle_first);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;


      int fromPions         = 0;
      int fromEta           = 0;
      int fromFireball      = 0;
      int fromDalitz        = 0;
      int fromXi            = 0;
      int fromOther         = 0;
      int fromCombinatorial = 0;
      int fromConversion    = 0;
      int twoFromTarget     = 0;
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) {
            fromFireball = 1;
          } else {
            CbmMCTrack* mcTrackGrmama =
              (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            if (mcTrackGrmama->GetPdgCode() == 111) fromPions = 1;
            if (mcTrackGrmama->GetPdgCode() == 221) fromEta = 1;
            if (mcTrackGrmama->GetPdgCode() == 3212) fromXi = 1;
            fromConversion = 1;
          }
        } else {
          if (mcTrackmama->GetPdgCode() == 111) fromDalitz = 1;
          if (mcTrackmama->GetPdgCode() != 111) fromOther = 1;
        }
      } else if (part1MC->GetMotherId() == part2MC->GetMotherId()
                 && part1MC->GetMotherId() == -1) {
        twoFromTarget = 1;
      } else {
        fromCombinatorial = 1;
      }


      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) {
            CGrandMother_PDG_Outside->Fill(mcTrackmama->GetMotherId());
          } else {
            CbmMCTrack* mcTrackGrmama =
              (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            CGrandMother_PDG_Outside->Fill(
              TMath::Abs(mcTrackGrmama->GetPdgCode()));
          }
        } else {
          CMother_PDG_Outside->Fill(TMath::Abs(mcTrackmama->GetPdgCode()));
        }
      }

      if (IdForANN == 1) CDP_AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) CDP_AnnFalsePairs_AfterCuts->Fill(AnnValue);


      // for event mixing CDP
      std::vector<TVector3> minusTrack = SaveAllHits(part1STS);
      std::vector<TVector3> plusTrack  = SaveAllHits(part2STS);

      CDP_EMT_Event_minus_Outside.push_back(EventNumMan);
      CDP_EMT_momenta_minus_Outside.push_back(part1STS);
      CDP_EMT_KFTrack_minus_Outside.push_back(electron);
      CDP_EMT_NofRings_minus_Outside.push_back(richcheck_0);
      CDP_EMT_Hits_minus_Outside.push_back(minusTrack);

      CDP_EMT_Event_plus_Outside.push_back(EventNumMan);
      CDP_EMT_momenta_plus_Outside.push_back(part2STS);
      CDP_EMT_KFTrack_plus_Outside.push_back(positron);
      CDP_EMT_NofRings_plus_Outside.push_back(richcheck_1);
      CDP_EMT_Hits_plus_Outside.push_back(plusTrack);


      // CDP_LK_EMT
      CDP_LK_EMT_NofRings_minus_Outside.push_back(richcheck_0);
      CDP_LK_EMT_STS_minus_Outside.push_back(part1STS);
      CDP_LK_EMT_STS_minus_index_Outside.push_back(sts1_index);

      CDP_LK_EMT_NofRings_plus_Outside.push_back(richcheck_1);
      CDP_LK_EMT_STS_plus_Outside.push_back(part2STS);
      CDP_LK_EMT_STS_plus_index_Outside.push_back(sts1_index);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        double weight_all = 0;
        // cout << "Rapidity = " << params.fRapidity << "; Pt = " << params.fPt << endl;
        // cout << "rap_coef = " << rap_coef << "; pt_coef = " << pt_coef << "; correction factor is " <<  corr_all[rap_coef][pt_coef] << endl;
        if (corr_all[rap_coef][pt_coef] != 0)
          weight_all = 1 / corr_all[rap_coef][pt_coef];
        if (weight_all > thresholdweight) weight_all = 0;
        CDP_InvMassReco_all_Outside->Fill(InvmassReco, weight_all);
        CDP_OpeningAngleReco_all_Outside->Fill(OpeningAngle, weight_all);
        CDP_Pdg_all_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_all_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_all_Outside->Fill(params.fMomentumMag, weight_all);
        CDP_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
        CDP_InvMassReco_all_Both->Fill(InvmassReco, weight_all);
        CDP_OpeningAngleReco_all_Both->Fill(OpeningAngle, weight_all);
        CDP_Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_all_Both->Fill(params.fMomentumMag, weight_all);
        CDP_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromTarget_Pt_reco_all_Both->Fill(params.fPt, weight_all);
          CPh_pt_vs_rap_est_all_Outside->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_all_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_all_Outside->Fill(
            params.fRapidity, params.fPt, weight_all);
          CPh_pt_vs_rap_est_corr_all_Both->Fill(
            params.fRapidity, params.fPt, weight_all);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromPions_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromEtas_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromDalitz_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromXi_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromOther_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_all_Outside->Fill(params.fPt,
                                                          weight_all);
          CPh_fromCombinatorial_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_fromConversion_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_all_Outside->Fill(params.fPt, weight_all);
          CPh_twoFromTarget_Pt_reco_all_Both->Fill(params.fPt, weight_all);
        }
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        double weight_two = 0;
        if (corr_two[rap_coef][pt_coef] != 0)
          weight_two = 1 / corr_two[rap_coef][pt_coef];
        if (weight_two > thresholdweight) weight_two = 0;
        CDP_InvMassReco_two_Outside->Fill(InvmassReco, weight_two);
        CDP_OpeningAngleReco_two_Outside->Fill(OpeningAngle, weight_two);
        CDP_Pdg_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_two_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_two_Outside->Fill(params.fMomentumMag, weight_two);
        CDP_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
        CDP_InvMassReco_two_Both->Fill(InvmassReco, weight_two);
        CDP_OpeningAngleReco_two_Both->Fill(OpeningAngle, weight_two);
        CDP_Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_two_Both->Fill(params.fMomentumMag, weight_two);
        CDP_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromTarget_Pt_reco_two_Both->Fill(params.fPt, weight_two);
          CPh_pt_vs_rap_est_two_Outside->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_two_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_two_Outside->Fill(
            params.fRapidity, params.fPt, weight_two);
          CPh_pt_vs_rap_est_corr_two_Both->Fill(
            params.fRapidity, params.fPt, weight_two);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromPions_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromEtas_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromDalitz_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromXi_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromOther_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_two_Outside->Fill(params.fPt,
                                                          weight_two);
          CPh_fromCombinatorial_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_fromConversion_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_two_Outside->Fill(params.fPt, weight_two);
          CPh_twoFromTarget_Pt_reco_two_Both->Fill(params.fPt, weight_two);
        }
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        double weight_onetwo = 0;
        if (corr_onetwo[rap_coef][pt_coef] != 0)
          weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];
        if (weight_onetwo > thresholdweight) weight_onetwo = 0;
        CDP_InvMassReco_onetwo_Outside->Fill(InvmassReco, weight_onetwo);
        CDP_OpeningAngleReco_onetwo_Outside->Fill(OpeningAngle, weight_onetwo);
        CDP_Pdg_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_onetwo_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_onetwo_Outside->Fill(params.fMomentumMag, weight_onetwo);
        CDP_Pt_reco_onetwo_Outside->Fill(params.fPt, weight_onetwo);
        CDP_InvMassReco_onetwo_Both->Fill(InvmassReco, weight_onetwo);
        CDP_OpeningAngleReco_onetwo_Both->Fill(OpeningAngle, weight_onetwo);
        CDP_Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        CDP_Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        CDP_P_reco_onetwo_Both->Fill(params.fMomentumMag, weight_onetwo);
        CDP_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        if (fromFireball == 1) {
          CPh_fromTarget_Pt_reco_onetwo_Outside->Fill(params.fPt,
                                                      weight_onetwo);
          CPh_fromTarget_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
          CPh_pt_vs_rap_est_onetwo_Outside->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_onetwo_Both->Fill(params.fRapidity, params.fPt);
          CPh_pt_vs_rap_est_corr_onetwo_Outside->Fill(
            params.fRapidity, params.fPt, weight_onetwo);
          CPh_pt_vs_rap_est_corr_onetwo_Both->Fill(
            params.fRapidity, params.fPt, weight_onetwo);
        }
        if (fromPions == 1) {
          CPh_fromPions_Pt_reco_onetwo_Outside->Fill(params.fPt, weight_onetwo);
          CPh_fromPions_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromEta == 1) {
          CPh_fromEtas_Pt_reco_onetwo_Outside->Fill(params.fPt, weight_onetwo);
          CPh_fromEtas_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromDalitz == 1) {
          CPh_fromDalitz_Pt_reco_onetwo_Outside->Fill(params.fPt,
                                                      weight_onetwo);
          CPh_fromDalitz_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromXi == 1) {
          CPh_fromXi_Pt_reco_onetwo_Outside->Fill(params.fPt, weight_onetwo);
          CPh_fromXi_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromOther == 1) {
          CPh_fromOther_Pt_reco_onetwo_Outside->Fill(params.fPt, weight_onetwo);
          CPh_fromOther_Pt_reco_onetwo_Both->Fill(params.fPt, weight_onetwo);
        }
        if (fromCombinatorial == 1) {
          CPh_fromCombinatorial_Pt_reco_onetwo_Outside->Fill(params.fPt,
                                                             weight_onetwo);
          CPh_fromCombinatorial_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                          weight_onetwo);
        }
        if (fromConversion == 1) {
          CPh_fromConversion_Pt_reco_onetwo_Outside->Fill(params.fPt,
                                                          weight_onetwo);
          CPh_fromConversion_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                       weight_onetwo);
        }
        if (twoFromTarget == 1) {
          CPh_twoFromTarget_Pt_reco_onetwo_Outside->Fill(params.fPt,
                                                         weight_onetwo);
          CPh_twoFromTarget_Pt_reco_onetwo_Both->Fill(params.fPt,
                                                      weight_onetwo);
        }
      }
    }
  }
}


int CbmKresConversionCorrectedPhotons::FindInRich(int richInd,
                                                  int stsMcTrackId) {
  int RingsInRich = 0;
  if (richInd > -1) {
    CbmTrackMatchNew* richMatch =
      (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    if (richMatch != nullptr && richMatch->GetNofLinks() > 0) {
      int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
      if (richMcTrackId > 0) {
        if (
          stsMcTrackId
          == richMcTrackId) {  // check that global track was matched correctly for STS and RICH together
          CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
          if (mcTrack2 != nullptr) {
            int pdgRICH = mcTrack2->GetPdgCode();
            if (TMath::Abs(pdgRICH) == 11) RingsInRich++;
          }
        }
      }
    }
  }
  return RingsInRich;
}


int CbmKresConversionCorrectedPhotons::CheckIfElectron(CbmRichRing* ring,
                                                       double momentum) {
  int identified = 0;

  if (nullptr != ring) {
    CbmRichRingLight ringHit;
    int nofHits = ring->GetNofHits();
    for (int i = 0; i < nofHits; i++) {
      int hitInd      = ring->GetHit(i);
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
      if (nullptr == hit) continue;
      CbmRichHitLight hl(hit->GetX(), hit->GetY());
      ringHit.AddHit(hl);
    }
    fTauFit->DoFit(&ringHit);
    if (ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6
        && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6 && momentum > 0.2
        && momentum < 4.)
      identified++;
    //if (ring->GetDistance() < 2 && ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6  && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6 && momentum > 0.2 && momentum < 4.) identified ++;
  }

  return identified;
}

std::vector<TVector3>
CbmKresConversionCorrectedPhotons::SaveAllHits(CbmStsTrack* track) {
  std::vector<TVector3> AllHitsOfTrack;
  AllHitsOfTrack.clear();

  Int_t hitsMVD = track->GetNofMvdHits();
  Int_t hitsSTS = track->GetNofStsHits();

  for (int i = 0; i < hitsMVD; i++) {
    Int_t mvdHitIndex = track->GetMvdHitIndex(i);
    CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
    TVector3 position;
    mvdHit->Position(position);
    AllHitsOfTrack.push_back(position);
  }
  for (int i = 0; i < hitsSTS; i++) {
    Int_t stsHitIndex = track->GetStsHitIndex(i);
    CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
    TVector3 position;
    stsHit->Position(position);
    AllHitsOfTrack.push_back(position);
  }

  return AllHitsOfTrack;
}

double CbmKresConversionCorrectedPhotons::CalculatePlaneAngle_last_fromHits(
  std::vector<TVector3> track_1,
  std::vector<TVector3> track_2) {
  double FinalAngle = 400;

  int hits_1 = track_1.size();
  int hits_2 = track_2.size();

  double Xpart1 = 0;
  double Ypart1 = 0;
  double Zpart1 = 0;
  double Xpart2 = 0;
  double Ypart2 = 0;
  double Zpart2 = 0;

  TVector3 last1 = track_1[hits_1 - 1];
  TVector3 last2 = track_2[hits_2 - 1];

  // // check difference in 2 cm, because of two slices of every STS and Mvd stations
  if (
    TMath::Abs(last1.Z() - last2.Z()) > 2
    && last1.Z()
         > last2
             .Z()) {  // if last hits are in different stations --> try to find the latest common station
    for (int i = hits_1 - 2; i > -1; i--) {  // start from second last station
      if (TMath::Abs(last1.Z() - last2.Z()) < 2) continue;
      last1  = track_1[i];
      Xpart1 = last1.X();
      Ypart1 = last1.Y();
      Zpart1 = last1.Z();
    }
  }

  if (
    TMath::Abs(last1.Z() - last2.Z()) > 2
    && last1.Z()
         < last2
             .Z()) {  // if last hits are in different stations --> try to find the latest common station
    for (int i = hits_2 - 2; i > -1; i--) {  // start from second last station
      if (TMath::Abs(last1.Z() - last2.Z()) < 2) continue;
      last2  = track_2[i];
      Xpart2 = last2.X();
      Ypart2 = last2.Y();
      Zpart2 = last2.Z();
    }
  }

  // calculate angle if we have found common station
  if (TMath::Abs(Zpart1 - Zpart2) < 2 && Zpart1 != 0 && Zpart2 != 0) {
    FinalAngle =
      TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}


double CbmKresConversionCorrectedPhotons::CalculatePlaneAngle_last(
  CbmStsTrack* Sts_1,
  CbmStsTrack* Sts_2) {
  double FinalAngle = 400;
  Int_t hits1sts    = Sts_1->GetNofStsHits();
  Int_t hits2sts    = Sts_2->GetNofStsHits();
  Int_t hits1mvd    = Sts_1->GetNofMvdHits();
  Int_t hits2mvd    = Sts_2->GetNofMvdHits();

  double Xpart1 = 0;
  double Ypart1 = 0;
  double Zpart1 = 0;
  double Xpart2 = 0;
  double Ypart2 = 0;
  double Zpart2 = 0;
  if (hits1sts > 0) {
    Int_t stsHitIndex1 = Sts_1->GetStsHitIndex(hits1sts - 1);
    CbmStsHit* stsHit1 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex1);
    Xpart1             = stsHit1->GetX();
    Ypart1             = stsHit1->GetY();
    Zpart1             = stsHit1->GetZ();
  } else {
    Int_t mvdHitIndex1 = Sts_1->GetMvdHitIndex(hits1mvd - 1);
    CbmMvdHit* mvdHit1 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex1);
    Xpart1             = mvdHit1->GetX();
    Ypart1             = mvdHit1->GetY();
    Zpart1             = mvdHit1->GetZ();
  }

  if (hits2sts > 0) {
    Int_t stsHitIndex2 = Sts_2->GetStsHitIndex(hits2sts - 1);
    CbmStsHit* stsHit2 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex2);
    Xpart2             = stsHit2->GetX();
    Ypart2             = stsHit2->GetY();
    Zpart2             = stsHit2->GetZ();
  } else {
    Int_t mvdHitIndex2 = Sts_2->GetMvdHitIndex(hits2mvd - 1);
    CbmMvdHit* mvdHit2 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex2);
    Xpart2             = mvdHit2->GetX();
    Ypart2             = mvdHit2->GetY();
    Zpart2             = mvdHit2->GetZ();
  }

  // check difference in 2 cm, because of two slices of every STS and Mvd stations
  if (
    TMath::Abs(Zpart1 - Zpart2) > 2
    && Zpart1
         > Zpart2) {  // if last hits are in different stations --> try to find the latest common station
    for (int i = hits1sts - 2; i > -1; i--) {  // start from second last station
      if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
      Int_t stsHitIndex = Sts_1->GetStsHitIndex(i);
      CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
      Xpart1            = stsHit->GetX();
      Ypart1            = stsHit->GetY();
      Zpart1            = stsHit->GetZ();
    }
    if (TMath::Abs(Zpart1 - Zpart2) > 2 && Zpart1 > Zpart2) {
      for (int i = hits1mvd - 2; i > -1; i--) {
        if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
        Int_t mvdHitIndex = Sts_1->GetMvdHitIndex(i);
        CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
        Xpart1            = mvdHit->GetX();
        Ypart1            = mvdHit->GetY();
        Zpart1            = mvdHit->GetZ();
      }
    }
  }

  if (
    TMath::Abs(Zpart1 - Zpart2) > 2
    && Zpart1
         < Zpart2) {  // if last hits are in different stations --> try to find the latest common station
    for (int i = hits2sts - 2; i > -1; i--) {  // start from second last station
      if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
      Int_t stsHitIndex = Sts_2->GetStsHitIndex(i);
      CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
      Xpart2            = stsHit->GetX();
      Ypart2            = stsHit->GetY();
      Zpart2            = stsHit->GetZ();
    }
    if (TMath::Abs(Zpart1 - Zpart2) > 2 && Zpart1 < Zpart2) {
      for (int i = hits2mvd - 2; i > -1; i--) {
        if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
        Int_t mvdHitIndex = Sts_2->GetMvdHitIndex(i);
        CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
        Xpart2            = mvdHit->GetX();
        Ypart2            = mvdHit->GetY();
        Zpart2            = mvdHit->GetZ();
      }
    }
  }

  // calculate angle if we found common station
  if (TMath::Abs(Zpart1 - Zpart2) < 2 && Zpart1 != 0 && Zpart2 != 0) {
    FinalAngle =
      TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}

double CbmKresConversionCorrectedPhotons::CalculatePlaneAngle_first(
  CbmStsTrack* Sts_1,
  CbmStsTrack* Sts_2) {
  double FinalAngle = 400;
  Int_t hits1sts    = Sts_1->GetNofStsHits();
  Int_t hits2sts    = Sts_2->GetNofStsHits();
  Int_t hits1mvd    = Sts_1->GetNofMvdHits();
  Int_t hits2mvd    = Sts_2->GetNofMvdHits();

  double Xpart1 = 0;
  double Ypart1 = 0;
  double Zpart1 = 0;
  double Xpart2 = 0;
  double Ypart2 = 0;
  double Zpart2 = 0;
  if (hits1mvd > 0) {
    Int_t mvdHitIndex1 = Sts_1->GetMvdHitIndex(0);
    CbmMvdHit* mvdHit1 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex1);
    Xpart1             = mvdHit1->GetX();
    Ypart1             = mvdHit1->GetY();
    Zpart1             = mvdHit1->GetZ();
  } else {
    Int_t stsHitIndex1 = Sts_1->GetStsHitIndex(0);
    CbmStsHit* stsHit1 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex1);
    Xpart1             = stsHit1->GetX();
    Ypart1             = stsHit1->GetY();
    Zpart1             = stsHit1->GetZ();
  }

  if (hits2mvd > 0) {
    Int_t mvdHitIndex2 = Sts_2->GetMvdHitIndex(0);
    CbmMvdHit* mvdHit2 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex2);
    Xpart2             = mvdHit2->GetX();
    Ypart2             = mvdHit2->GetY();
    Zpart2             = mvdHit2->GetZ();
  } else {
    Int_t stsHitIndex2 = Sts_2->GetStsHitIndex(0);
    CbmStsHit* stsHit2 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex2);
    Xpart2             = stsHit2->GetX();
    Ypart2             = stsHit2->GetY();
    Zpart2             = stsHit2->GetZ();
  }

  // check difference in 2 cm, because of two slices of every STS and Mvd stations
  if (
    TMath::Abs(Zpart1 - Zpart2) > 2
    && Zpart1
         < Zpart2) {  // if first hits are in different stations --> try to find the earliest common station
    for (int i = 1; i < hits1mvd; i++) {  // start from second hit
      if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
      Int_t mvdHitIndex = Sts_1->GetMvdHitIndex(i);
      CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
      Xpart1            = mvdHit->GetX();
      Ypart1            = mvdHit->GetY();
      Zpart1            = mvdHit->GetZ();
    }
    if (TMath::Abs(Zpart1 - Zpart2) > 2 && Zpart1 < Zpart2) {
      for (int i = 0; i < hits1sts; i++) {
        if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
        Int_t stsHitIndex = Sts_1->GetStsHitIndex(i);
        CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
        Xpart1            = stsHit->GetX();
        Ypart1            = stsHit->GetY();
        Zpart1            = stsHit->GetZ();
      }
    }
  }

  if (
    TMath::Abs(Zpart1 - Zpart2) > 2
    && Zpart1
         > Zpart2) {  // if first hits are in different stations --> try to find the earliest common station
    for (int i = 1; i < hits2mvd; i++) {  // start from second hit
      if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
      Int_t mvdHitIndex = Sts_2->GetMvdHitIndex(i);
      CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
      Xpart2            = mvdHit->GetX();
      Ypart2            = mvdHit->GetY();
      Zpart2            = mvdHit->GetZ();
    }
    if (TMath::Abs(Zpart1 - Zpart2) > 2 && Zpart1 > Zpart2) {
      for (int i = 0; i < hits2sts; i++) {
        if (TMath::Abs(Zpart1 - Zpart2) < 2) continue;
        Int_t stsHitIndex = Sts_2->GetStsHitIndex(i);
        CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
        Xpart2            = stsHit->GetX();
        Ypart2            = stsHit->GetY();
        Zpart2            = stsHit->GetZ();
      }
    }
  }

  // calculate angle if we found common station
  if (TMath::Abs(Zpart1 - Zpart2) < 2 && Zpart1 != 0 && Zpart2 != 0) {
    FinalAngle =
      TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}

void CbmKresConversionCorrectedPhotons::CDP_likesign_Mixing_Target(
  double AngleCut,
  double InvMassCut)
// mix particles with the same charge TARGET
{
  int nof_minus = CDP_LK_EMT_momenta_minus_Target.size();
  for (int a = 0; a < nof_minus - 1; a++) {
    for (int b = a + 1; b < nof_minus; b++) {
      if (CDP_LK_EMT_STS_minus_index_Target[a]
          == CDP_LK_EMT_STS_minus_index_Target[b])
        continue;
      TVector3 e1       = CDP_LK_EMT_momenta_minus_Target[a];
      TVector3 e2       = CDP_LK_EMT_momenta_minus_Target[b];
      CbmStsTrack* sts1 = CDP_LK_EMT_STS_minus_Target[a];
      CbmStsTrack* sts2 = CDP_LK_EMT_STS_minus_Target[b];

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     fKFVertex.GetRefZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount = CDP_LK_EMT_NofRings_minus_Target[a]
                         + CDP_LK_EMT_NofRings_minus_Target[b];

      CDP_LK_EMT_Pt_all_Target->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Target->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Target->Fill(params.fPt, weight_onetwo);
      CDP_LK_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_onetwo);
    }
  }


  int nof_plus = CDP_LK_EMT_momenta_plus_Target.size();
  for (int a = 0; a < nof_plus - 1; a++) {
    for (int b = a + 1; b < nof_plus; b++) {
      if (CDP_LK_EMT_STS_plus_index_Target[a]
          == CDP_LK_EMT_STS_plus_index_Target[b])
        continue;
      TVector3 e1       = CDP_LK_EMT_momenta_plus_Target[a];
      TVector3 e2       = CDP_LK_EMT_momenta_plus_Target[b];
      CbmStsTrack* sts1 = CDP_LK_EMT_STS_plus_Target[a];
      CbmStsTrack* sts2 = CDP_LK_EMT_STS_plus_Target[b];

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     fKFVertex.GetRefZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount =
        CDP_LK_EMT_NofRings_plus_Target[a] + CDP_LK_EMT_NofRings_plus_Target[b];

      CDP_LK_EMT_Pt_all_Target->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Target->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Target->Fill(params.fPt, weight_onetwo);
      CDP_LK_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_onetwo);
    }
  }
}

void CbmKresConversionCorrectedPhotons::CDP_likesign_Mixing_Outside(
  double AngleCut,
  double InvMassCut)
// mix particles with the same charge OUTSIDE
{
  int nof_minus = CDP_LK_EMT_NofRings_minus_Outside.size();
  for (int a = 0; a < nof_minus - 1; a++) {
    for (int b = a + 1; b < nof_minus; b++) {
      if (CDP_LK_EMT_STS_minus_index_Outside[a]
          == CDP_LK_EMT_STS_minus_index_Outside[b])
        continue;
      CbmStsTrack* sts1 = CDP_LK_EMT_STS_minus_Outside[a];
      CbmStsTrack* sts2 = CDP_LK_EMT_STS_minus_Outside[b];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts1, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts2, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5)
        continue;  // kick weird intersections

      TVector3 e1 = CbmKresFunctions::FitToVertex(
        sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      TVector3 e2 = CbmKresFunctions::FitToVertex(
        sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     fKFVertex.GetRefZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount = CDP_LK_EMT_NofRings_minus_Outside[a]
                         + CDP_LK_EMT_NofRings_minus_Outside[b];

      CDP_LK_EMT_Pt_all_Outside->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Outside->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Outside->Fill(params.fPt, weight_onetwo);
      CDP_LK_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_onetwo);
    }
  }


  int nof_plus = CDP_LK_EMT_NofRings_plus_Outside.size();
  for (int a = 0; a < nof_plus - 1; a++) {
    for (int b = a + 1; b < nof_plus; b++) {
      if (CDP_LK_EMT_STS_plus_index_Outside[a]
          == CDP_LK_EMT_STS_plus_index_Outside[b])
        continue;
      CbmStsTrack* sts1 = CDP_LK_EMT_STS_plus_Outside[a];
      CbmStsTrack* sts2 = CDP_LK_EMT_STS_plus_Outside[b];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts1, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts2, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5)
        continue;  // kick weird intersections

      TVector3 e1 = CbmKresFunctions::FitToVertex(
        sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      TVector3 e2 = CbmKresFunctions::FitToVertex(
        sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     fKFVertex.GetRefZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount = CDP_LK_EMT_NofRings_plus_Outside[a]
                         + CDP_LK_EMT_NofRings_plus_Outside[b];

      CDP_LK_EMT_Pt_all_Outside->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Outside->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Outside->Fill(params.fPt, weight_onetwo);
      CDP_LK_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2)
        CDP_LK_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      if ((rings_amount == 1 || rings_amount == 2))
        CDP_LK_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_onetwo);
    }
  }
}


void CbmKresConversionCorrectedPhotons::CDP_Mixing_Target(double AngleCut,
                                                          double InvMassCut)
// TARGET
{
  int nof_Target = CDP_EMT_Event_minus_Target.size();
  cout << "Mixing for corrected direct photons in Target - nof entries "
       << nof_Target << endl;

  for (size_t a = 0; a < CDP_EMT_Event_minus_Target.size(); a++) {
    for (size_t b = 0; b < CDP_EMT_Event_plus_Target.size(); b++) {
      if (CDP_EMT_Event_minus_Target[a] == CDP_EMT_Event_plus_Target[b])
        continue;  // to make sure that two particles are from two different events
      TVector3 e1                 = CDP_EMT_momenta_minus_Target[a];
      TVector3 e2                 = CDP_EMT_momenta_plus_Target[b];
      std::vector<TVector3> hits1 = CDP_EMT_Hits_minus_Target[a];
      std::vector<TVector3> hits2 = CDP_EMT_Hits_plus_Target[b];

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last_fromHits(hits1, hits2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     fKFVertex.GetRefZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount =
        CDP_EMT_NofRings_minus_Target[a] + CDP_EMT_NofRings_plus_Target[b];

      CDP_EMT_Pt_all_Target->Fill(params.fPt, weight_all);
      CDP_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2) {
        CDP_EMT_Pt_two_Target->Fill(params.fPt, weight_two);
        CDP_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      }
      if ((rings_amount == 1 || rings_amount == 2)) {
        CDP_EMT_Pt_onetwo_Target->Fill(params.fPt, weight_two);
        CDP_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_two);
      }
    }
  }
}


void CbmKresConversionCorrectedPhotons::CDP_Mixing_Outside(double AngleCut,
                                                           double InvMassCut)
// OUTSIDE
{
  int nof_Outside = CDP_EMT_Event_minus_Outside.size();
  cout << "Mixing for corrected direct photons in Outside - nof entries "
       << nof_Outside << endl;

  for (size_t a = 0; a < CDP_EMT_Event_minus_Outside.size(); a++) {
    for (size_t b = 0; b < CDP_EMT_Event_plus_Outside.size(); b++) {
      if (CDP_EMT_Event_minus_Outside[a] == CDP_EMT_Event_plus_Outside[b])
        continue;  // to make sure that two particles are from two different events

      KFParticle electron = CDP_EMT_KFTrack_minus_Outside[a];
      KFParticle positron = CDP_EMT_KFTrack_plus_Outside[b];

      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5)
        continue;  // kick weird intersections

      TVector3 e1(electron.KFParticleBase::Px(),
                  electron.KFParticleBase::Py(),
                  electron.KFParticleBase::Pz());
      TVector3 e2(positron.KFParticleBase::Px(),
                  positron.KFParticleBase::Py(),
                  positron.KFParticleBase::Pz());

      std::vector<TVector3> hits1 = CDP_EMT_Hits_minus_Outside[a];
      std::vector<TVector3> hits2 = CDP_EMT_Hits_plus_Outside[b];

      double InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last_fromHits(hits1, hits2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco,
                                     OpeningAngle,
                                     PlaneAngle_last,
                                     intersection.GetZ(),
                                     e1,
                                     e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      if (params.fRapidity != params.fRapidity) continue;
      if (params.fPt != params.fPt) continue;
      if (params.fRapidity >= 4) continue;
      if (params.fPt >= 3) continue;

      double rap_step      = 0.4;
      double pt_step       = 0.1;
      int rap_coef         = params.fRapidity / rap_step;
      int pt_coef          = params.fPt / pt_step;
      double weight_all    = 0;
      double weight_two    = 0;
      double weight_onetwo = 0;
      if (corr_all[rap_coef][pt_coef] != 0)
        weight_all = 1 / corr_all[rap_coef][pt_coef];
      if (corr_two[rap_coef][pt_coef] != 0)
        weight_two = 1 / corr_two[rap_coef][pt_coef];
      if (corr_onetwo[rap_coef][pt_coef] != 0)
        weight_onetwo = 1 / corr_onetwo[rap_coef][pt_coef];

      if (weight_all > thresholdweight) weight_all = 0;
      if (weight_two > thresholdweight) weight_two = 0;
      if (weight_onetwo > thresholdweight) weight_onetwo = 0;

      int rings_amount =
        CDP_EMT_NofRings_minus_Outside[a] + CDP_EMT_NofRings_plus_Outside[b];

      CDP_EMT_Pt_all_Outside->Fill(params.fPt, weight_all);
      CDP_EMT_Pt_all_Both->Fill(params.fPt, weight_all);
      if (rings_amount == 2) {
        CDP_EMT_Pt_two_Outside->Fill(params.fPt, weight_two);
        CDP_EMT_Pt_two_Both->Fill(params.fPt, weight_two);
      }
      if ((rings_amount == 1 || rings_amount == 2)) {
        CDP_EMT_Pt_onetwo_Outside->Fill(params.fPt, weight_two);
        CDP_EMT_Pt_onetwo_Both->Fill(params.fPt, weight_two);
      }
    }
  }
}


void CbmKresConversionCorrectedPhotons::Finish() {
  gDirectory->mkdir("corrected photons");
  gDirectory->cd("corrected photons");

  if (UseCorrection) { fGammaCorrection->Finish(); }

  gDirectory->mkdir("Target");
  gDirectory->cd("Target");
  gDirectory->mkdir("CheckCuts_Target");
  gDirectory->cd("CheckCuts_Target");
  for (UInt_t i = 0; i < fHistoList_dp_cuts_Target.size(); i++) {
    fHistoList_dp_cuts_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_dp_all_Target.size(); i++) {
    fHistoList_dp_all_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_dp_two_Target.size(); i++) {
    fHistoList_dp_two_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_dp_onetwo_Target.size(); i++) {
    fHistoList_dp_onetwo_Target[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_dp_Target.size(); i++) {
    fHistoList_dp_Target[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("Outside");
  gDirectory->cd("Outside");
  gDirectory->mkdir("CheckCuts_Outside");
  gDirectory->cd("CheckCuts_Outside");
  for (UInt_t i = 0; i < fHistoList_dp_cuts_Outside.size(); i++) {
    fHistoList_dp_cuts_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_dp_all_Outside.size(); i++) {
    fHistoList_dp_all_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_dp_two_Outside.size(); i++) {
    fHistoList_dp_two_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_dp_onetwo_Outside.size(); i++) {
    fHistoList_dp_onetwo_Outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_dp_Outside.size(); i++) {
    fHistoList_dp_Outside[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("Both");
  gDirectory->cd("Both");
  gDirectory->mkdir("CheckCuts_Both");
  gDirectory->cd("CheckCuts_Both");
  for (UInt_t i = 0; i < fHistoList_dp_cuts_Both.size(); i++) {
    fHistoList_dp_cuts_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_dp_all_Both.size(); i++) {
    fHistoList_dp_all_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_dp_two_Both.size(); i++) {
    fHistoList_dp_two_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_dp_onetwo_Both.size(); i++) {
    fHistoList_dp_onetwo_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_dp_Both.size(); i++) {
    fHistoList_dp_Both[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->cd("..");
}


void CbmKresConversionCorrectedPhotons::InitHistograms() {
  // Target
  CMother_PDG_Target =
    new TH1D("CMother_PDG_Target", "CMother_PDG_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_Target.push_back(CMother_PDG_Target);
  CGrandMother_PDG_Target = new TH1D(
    "CGrandMother_PDG_Target", "CGrandMother_PDG_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_Target.push_back(CGrandMother_PDG_Target);

  // Outside
  CMother_PDG_Outside = new TH1D(
    "CMother_PDG_Outside", "CMother_PDG_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_Outside.push_back(CMother_PDG_Outside);
  CGrandMother_PDG_Outside = new TH1D("CGrandMother_PDG_Outside",
                                      "CGrandMother_PDG_Outside; Id;#",
                                      1000,
                                      -10,
                                      490);
  fHistoList_dp_Outside.push_back(CGrandMother_PDG_Outside);

  // Both
  CPdg_vs_Distance_for_dp =
    new TH2D("CPdg_vs_Distance_for_dp",
             "CPdg_vs_Distance_for_dp; pdg; distance in cm",
             2500,
             0,
             2499,
             500,
             0,
             50);
  fHistoList_dp_Both.push_back(CPdg_vs_Distance_for_dp);
  CP_vs_Distance_for_dp =
    new TH2D("CP_vs_Distance_for_dp",
             "CDistance between projected track and center of the ring (for e+ "
             "and e-); P in GeV/c^{2}; distance in cm",
             300,
             0,
             3,
             300,
             0,
             15);
  fHistoList_dp_Both.push_back(CP_vs_Distance_for_dp);
  CDP_AnnTruePairs = new TH1D(
    "CDP_AnnTruePairs", "CDP_AnnTruePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(CDP_AnnTruePairs);
  CDP_AnnFalsePairs = new TH1D(
    "CDP_AnnFalsePairs", "CDP_AnnFalsePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(CDP_AnnFalsePairs);
  CDP_AnnTruePairs_AfterCuts =
    new TH1D("CDP_AnnTruePairs_AfterCuts",
             "CDP_AnnTruePairs_AfterCuts; Ann value ;#",
             100,
             -1.2,
             1.2);
  fHistoList_dp_Both.push_back(CDP_AnnTruePairs_AfterCuts);
  CDP_AnnFalsePairs_AfterCuts =
    new TH1D("CDP_AnnFalsePairs_AfterCuts",
             "CDP_AnnFalsePairs_AfterCuts; Ann value ;#",
             100,
             -1.2,
             1.2);
  fHistoList_dp_Both.push_back(CDP_AnnFalsePairs_AfterCuts);


  ///////   histograms to check Cuts
  ///////   Both
  CDP_candidates_InvMass_vs_OA_Both =
    new TH2D("CDP_candidates_InvMass_vs_OA_Both",
             "CDP_candidates_InvMass_vs_OA_Both; invariant mass in GeV/c^{2}; "
             "opening angle in degree",
             500,
             0,
             0.5,
             500,
             0,
             50);
  fHistoList_dp_cuts_Both.push_back(CDP_candidates_InvMass_vs_OA_Both);
  CDP_InvMass_vs_OA_Both = new TH2D("CDP_InvMass_vs_OA_Both",
                                    "CDP_InvMass_vs_OA_Both; invariant mass in "
                                    "GeV/c^{2}; opening angle in degree",
                                    500,
                                    0,
                                    0.5,
                                    500,
                                    0,
                                    50);
  fHistoList_dp_cuts_Both.push_back(CDP_InvMass_vs_OA_Both);
  CDP_candidates_InvMass_Both =
    new TH1D("CDP_candidates_InvMass_Both",
             "CDP_candidates_InvMass_Both; invariant mass in GeV/c^{2};#",
             510,
             -0.01,
             0.5);
  fHistoList_dp_cuts_Both.push_back(CDP_candidates_InvMass_Both);
  CDP_InvMass_Both = new TH1D("CDP_InvMass_Both",
                              "CDP_InvMass_Both; invariant mass in GeV/c^{2};#",
                              510,
                              -0.01,
                              0.5);
  fHistoList_dp_cuts_Both.push_back(CDP_InvMass_Both);
  CDP_candidates_OA_Both =
    new TH1D("CDP_candidates_OA_Both",
             "CDP_candidates_OA_Both; opening angle in degree;#",
             300,
             -0.1,
             29.9);
  fHistoList_dp_cuts_Both.push_back(CDP_candidates_OA_Both);
  CDP_OA_Both = new TH1D(
    "CDP_OA_Both", "CDP_OA_Both; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Both.push_back(CDP_OA_Both);
  CDP_candidates_PlaneAngles_last_Both =
    new TH1D("CDP_candidates_PlaneAngles_last_Both",
             "CDP_candidates_PlaneAngles_last_Both; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Both.push_back(CDP_candidates_PlaneAngles_last_Both);
  CDP_PlaneAngles_last_Both =
    new TH1D("CDP_PlaneAngles_last_Both",
             "CDP_PlaneAngles_last_Both; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Both.push_back(CDP_PlaneAngles_last_Both);
  CDP_candidates_PlaneAngles_first_Both =
    new TH1D("CDP_candidates_PlaneAngles_first_Both",
             "CDP_candidates_PlaneAngles_first_Both; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Both.push_back(CDP_candidates_PlaneAngles_first_Both);
  CDP_PlaneAngles_first_Both =
    new TH1D("CDP_PlaneAngles_first_Both",
             "CDP_PlaneAngles_first_Both; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Both.push_back(CDP_PlaneAngles_first_Both);

  ///////   Target
  CDP_candidates_InvMass_vs_OA_Target =
    new TH2D("CDP_candidates_InvMass_vs_OA_Target",
             "CDP_candidates_InvMass_vs_OA_Target; invariant mass in "
             "GeV/c^{2}; opening angle in degree",
             500,
             0,
             0.5,
             500,
             0,
             50);
  fHistoList_dp_cuts_Target.push_back(CDP_candidates_InvMass_vs_OA_Target);
  CDP_InvMass_vs_OA_Target =
    new TH2D("CDP_InvMass_vs_OA_Target",
             "CDP_InvMass_vs_OA_Target; invariant mass in GeV/c^{2}; opening "
             "angle in degree",
             500,
             0,
             0.5,
             500,
             0,
             50);
  fHistoList_dp_cuts_Target.push_back(CDP_InvMass_vs_OA_Target);
  CDP_candidates_InvMass_Target =
    new TH1D("CDP_candidates_InvMass_Target",
             "CDP_candidates_InvMass_Target; invariant mass in GeV/c^{2};#",
             510,
             -0.01,
             0.5);
  fHistoList_dp_cuts_Target.push_back(CDP_candidates_InvMass_Target);
  CDP_InvMass_Target =
    new TH1D("CDP_InvMass_Target",
             "CDP_InvMass_Target; invariant mass in GeV/c^{2};#",
             510,
             -0.01,
             0.5);
  fHistoList_dp_cuts_Target.push_back(CDP_InvMass_Target);
  CDP_candidates_OA_Target =
    new TH1D("CDP_candidates_OA_Target",
             "CDP_candidates_OA_Target; opening angle in degree;#",
             300,
             -0.1,
             29.9);
  fHistoList_dp_cuts_Target.push_back(CDP_candidates_OA_Target);
  CDP_OA_Target = new TH1D("CDP_OA_Target",
                           "CDP_OA_Target; opening angle in degree;#",
                           300,
                           -0.1,
                           29.9);
  fHistoList_dp_cuts_Target.push_back(CDP_OA_Target);
  CDP_candidates_PlaneAngles_last_Target =
    new TH1D("CDP_candidates_PlaneAngles_last_Target",
             "CDP_candidates_PlaneAngles_last_Target; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Target.push_back(CDP_candidates_PlaneAngles_last_Target);
  CDP_PlaneAngles_last_Target =
    new TH1D("CDP_PlaneAngles_last_Target",
             "CDP_PlaneAngles_last_Target; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Target.push_back(CDP_PlaneAngles_last_Target);
  CDP_candidates_PlaneAngles_first_Target = new TH1D(
    "CDP_candidates_PlaneAngles_first_Target",
    "CDP_candidates_PlaneAngles_first_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_dp_cuts_Target.push_back(CDP_candidates_PlaneAngles_first_Target);
  CDP_PlaneAngles_first_Target =
    new TH1D("CDP_PlaneAngles_first_Target",
             "CDP_PlaneAngles_first_Target; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Target.push_back(CDP_PlaneAngles_first_Target);

  ///////   Outside
  CDP_candidates_InvMass_vs_OA_Outside =
    new TH2D("CDP_candidates_InvMass_vs_OA_Outside",
             "CDP_candidates_InvMass_vs_OA_Outside; invariant mass in "
             "GeV/c^{2}; opening angle in degree",
             500,
             0,
             0.5,
             500,
             0,
             50);
  fHistoList_dp_cuts_Outside.push_back(CDP_candidates_InvMass_vs_OA_Outside);
  CDP_InvMass_vs_OA_Outside =
    new TH2D("CDP_InvMass_vs_OA_Outside",
             "CDP_InvMass_vs_OA_Outside; invariant mass in GeV/c^{2}; opening "
             "angle in degree",
             500,
             0,
             0.5,
             500,
             0,
             50);
  fHistoList_dp_cuts_Outside.push_back(CDP_InvMass_vs_OA_Outside);
  CDP_candidates_InvMass_Outside =
    new TH1D("CDP_candidates_InvMass_Outside",
             "CDP_candidates_InvMass_Outside; invariant mass in GeV/c^{2};#",
             510,
             -0.01,
             0.5);
  fHistoList_dp_cuts_Outside.push_back(CDP_candidates_InvMass_Outside);
  CDP_InvMass_Outside =
    new TH1D("CDP_InvMass_Outside",
             "CDP_InvMass_Outside; invariant mass in GeV/c^{2};#",
             510,
             -0.01,
             0.5);
  fHistoList_dp_cuts_Outside.push_back(CDP_InvMass_Outside);
  CDP_candidates_OA_Outside =
    new TH1D("CDP_candidates_OA_Outside",
             "CDP_candidates_OA_Outside; opening angle in degree;#",
             300,
             -0.1,
             29.9);
  fHistoList_dp_cuts_Outside.push_back(CDP_candidates_OA_Outside);
  CDP_OA_Outside = new TH1D("CDP_OA_Outside",
                            "CDP_OA_Outside; opening angle in degree;#",
                            300,
                            -0.1,
                            29.9);
  fHistoList_dp_cuts_Outside.push_back(CDP_OA_Outside);
  CDP_candidates_PlaneAngles_last_Outside = new TH1D(
    "CDP_candidates_PlaneAngles_last_Outside",
    "CDP_candidates_PlaneAngles_last_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_dp_cuts_Outside.push_back(CDP_candidates_PlaneAngles_last_Outside);
  CDP_PlaneAngles_last_Outside =
    new TH1D("CDP_PlaneAngles_last_Outside",
             "CDP_PlaneAngles_last_Outside; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Outside.push_back(CDP_PlaneAngles_last_Outside);
  CDP_candidates_PlaneAngles_first_Outside = new TH1D(
    "CDP_candidates_PlaneAngles_first_Outside",
    "CDP_candidates_PlaneAngles_first_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_dp_cuts_Outside.push_back(
    CDP_candidates_PlaneAngles_first_Outside);
  CDP_PlaneAngles_first_Outside =
    new TH1D("CDP_PlaneAngles_first_Outside",
             "CDP_PlaneAngles_first_Outside; #theta angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_dp_cuts_Outside.push_back(CDP_PlaneAngles_first_Outside);


  // Target => all
  CDP_InvMassReco_all_Target =
    new TH1D("CDP_InvMassReco_all_Target",
             "CDP_InvMassReco_all_Target; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_all_Target.push_back(CDP_InvMassReco_all_Target);
  CDP_OpeningAngleReco_all_Target =
    new TH1D("CDP_OpeningAngleReco_all_Target",
             "CDP_OpeningAngleReco_all_Target; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_all_Target.push_back(CDP_OpeningAngleReco_all_Target);
  CDP_Pdg_all_Target =
    new TH1D("CDP_Pdg_all_Target", "CDP_Pdg_all_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Target.push_back(CDP_Pdg_all_Target);
  CDP_P_reco_all_Target = new TH1D("CDP_P_reco_all_Target",
                                   "CDP_P_reco_all_Target; P in GeV/c^{2};#",
                                   200,
                                   0,
                                   10);
  fHistoList_dp_all_Target.push_back(CDP_P_reco_all_Target);
  CDP_Pt_reco_all_Target =
    new TH1D("CDP_Pt_reco_all_Target",
             "CDP_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CDP_Pt_reco_all_Target);
  CPh_fromTarget_Pt_reco_all_Target =
    new TH1D("CPh_fromTarget_Pt_reco_all_Target",
             "CPh_fromTarget_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromTarget_Pt_reco_all_Target);
  CPh_fromPions_Pt_reco_all_Target =
    new TH1D("CPh_fromPions_Pt_reco_all_Target",
             "CPh_fromPions_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromPions_Pt_reco_all_Target);
  CPh_fromEtas_Pt_reco_all_Target =
    new TH1D("CPh_fromEtas_Pt_reco_all_Target",
             "CPh_fromEtas_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromEtas_Pt_reco_all_Target);
  CPh_fromDalitz_Pt_reco_all_Target =
    new TH1D("CPh_fromDalitz_Pt_reco_all_Target",
             "CPh_fromDalitz_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromDalitz_Pt_reco_all_Target);
  CPh_fromXi_Pt_reco_all_Target =
    new TH1D("CPh_fromXi_Pt_reco_all_Target",
             "CPh_fromXi_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromXi_Pt_reco_all_Target);
  CPh_fromOther_Pt_reco_all_Target =
    new TH1D("CPh_fromOther_Pt_reco_all_Target",
             "CPh_fromOther_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromOther_Pt_reco_all_Target);
  CPh_twoFromTarget_Pt_reco_all_Target =
    new TH1D("CPh_twoFromTarget_Pt_reco_all_Target",
             "CPh_twoFromTarget_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_twoFromTarget_Pt_reco_all_Target);
  CPh_fromCombinatorial_Pt_reco_all_Target =
    new TH1D("CPh_fromCombinatorial_Pt_reco_all_Target",
             "CPh_fromCombinatorial_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromCombinatorial_Pt_reco_all_Target);
  CPh_fromConversion_Pt_reco_all_Target =
    new TH1D("CPh_fromConversion_Pt_reco_all_Target",
             "CPh_fromConversion_Pt_reco_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CPh_fromConversion_Pt_reco_all_Target);
  CPh_pt_vs_rap_est_all_Target =
    new TH2D("CPh_pt_vs_rap_est_all_Target",
             "CPh_pt_vs_rap_est_all_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Target.push_back(CPh_pt_vs_rap_est_all_Target);
  CPh_pt_vs_rap_est_corr_all_Target =
    new TH2D("CPh_pt_vs_rap_est_corr_all_Target",
             "CPh_pt_vs_rap_est_corr_all_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Target.push_back(CPh_pt_vs_rap_est_corr_all_Target);

  // Target => two
  CDP_InvMassReco_two_Target =
    new TH1D("CDP_InvMassReco_two_Target",
             "CDP_InvMassReco_two_Target; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_two_Target.push_back(CDP_InvMassReco_two_Target);
  CDP_OpeningAngleReco_two_Target =
    new TH1D("CDP_OpeningAngleReco_two_Target",
             "CDP_OpeningAngleReco_two_Target; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_two_Target.push_back(CDP_OpeningAngleReco_two_Target);
  CDP_Pdg_two_Target =
    new TH1D("CDP_Pdg_two_Target", "CDP_Pdg_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(CDP_Pdg_two_Target);
  CDP_P_reco_two_Target = new TH1D("CDP_P_reco_two_Target",
                                   "CDP_P_reco_two_Target; P in GeV/c^{2};#",
                                   200,
                                   0,
                                   10);
  fHistoList_dp_two_Target.push_back(CDP_P_reco_two_Target);
  CDP_Pt_reco_two_Target =
    new TH1D("CDP_Pt_reco_two_Target",
             "CDP_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CDP_Pt_reco_two_Target);
  CPh_fromTarget_Pt_reco_two_Target =
    new TH1D("CPh_fromTarget_Pt_reco_two_Target",
             "CPh_fromTarget_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromTarget_Pt_reco_two_Target);
  CPh_fromPions_Pt_reco_two_Target =
    new TH1D("CPh_fromPions_Pt_reco_two_Target",
             "CPh_fromPions_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromPions_Pt_reco_two_Target);
  CPh_fromEtas_Pt_reco_two_Target =
    new TH1D("CPh_fromEtas_Pt_reco_two_Target",
             "CPh_fromEtas_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromEtas_Pt_reco_two_Target);
  CPh_fromDalitz_Pt_reco_two_Target =
    new TH1D("CPh_fromDalitz_Pt_reco_two_Target",
             "CPh_fromDalitz_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromDalitz_Pt_reco_two_Target);
  CPh_fromXi_Pt_reco_two_Target =
    new TH1D("CPh_fromXi_Pt_reco_two_Target",
             "CPh_fromXi_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromXi_Pt_reco_two_Target);
  CPh_fromOther_Pt_reco_two_Target =
    new TH1D("CPh_fromOther_Pt_reco_two_Target",
             "CPh_fromOther_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromOther_Pt_reco_two_Target);
  CPh_twoFromTarget_Pt_reco_two_Target =
    new TH1D("CPh_twoFromTarget_Pt_reco_two_Target",
             "CPh_twoFromTarget_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_twoFromTarget_Pt_reco_two_Target);
  CPh_fromCombinatorial_Pt_reco_two_Target =
    new TH1D("CPh_fromCombinatorial_Pt_reco_two_Target",
             "CPh_fromCombinatorial_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromCombinatorial_Pt_reco_two_Target);
  CPh_fromConversion_Pt_reco_two_Target =
    new TH1D("CPh_fromConversion_Pt_reco_two_Target",
             "CPh_fromConversion_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CPh_fromConversion_Pt_reco_two_Target);
  CPh_pt_vs_rap_est_two_Target =
    new TH2D("CPh_pt_vs_rap_est_two_Target",
             "CPh_pt_vs_rap_est_two_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Target.push_back(CPh_pt_vs_rap_est_two_Target);
  CPh_pt_vs_rap_est_corr_two_Target =
    new TH2D("CPh_pt_vs_rap_est_corr_two_Target",
             "CPh_pt_vs_rap_est_corr_two_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Target.push_back(CPh_pt_vs_rap_est_corr_two_Target);

  // Target => onetwo
  CDP_InvMassReco_onetwo_Target =
    new TH1D("CDP_InvMassReco_onetwo_Target",
             "CDP_InvMassReco_onetwo_Target; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_onetwo_Target.push_back(CDP_InvMassReco_onetwo_Target);
  CDP_OpeningAngleReco_onetwo_Target =
    new TH1D("CDP_OpeningAngleReco_onetwo_Target",
             "CDP_OpeningAngleReco_onetwo_Target; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_onetwo_Target.push_back(CDP_OpeningAngleReco_onetwo_Target);
  CDP_Pdg_onetwo_Target = new TH1D(
    "CDP_Pdg_onetwo_Target", "CDP_Pdg_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(CDP_Pdg_onetwo_Target);
  CDP_P_reco_onetwo_Target =
    new TH1D("CDP_P_reco_onetwo_Target",
             "CDP_P_reco_onetwo_Target; P in GeV/c^{2};#",
             200,
             0,
             10);
  fHistoList_dp_onetwo_Target.push_back(CDP_P_reco_onetwo_Target);
  CDP_Pt_reco_onetwo_Target =
    new TH1D("CDP_Pt_reco_onetwo_Target",
             "CDP_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CDP_Pt_reco_onetwo_Target);
  CPh_fromTarget_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromTarget_Pt_reco_onetwo_Target",
             "CPh_fromTarget_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromTarget_Pt_reco_onetwo_Target);
  CPh_fromPions_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromPions_Pt_reco_onetwo_Target",
             "CPh_fromPions_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromPions_Pt_reco_onetwo_Target);
  CPh_fromEtas_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromEtas_Pt_reco_onetwo_Target",
             "CPh_fromEtas_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromEtas_Pt_reco_onetwo_Target);
  CPh_fromDalitz_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromDalitz_Pt_reco_onetwo_Target",
             "CPh_fromDalitz_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromDalitz_Pt_reco_onetwo_Target);
  CPh_fromXi_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromXi_Pt_reco_onetwo_Target",
             "CPh_fromXi_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromXi_Pt_reco_onetwo_Target);
  CPh_fromOther_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromOther_Pt_reco_onetwo_Target",
             "CPh_fromOther_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CPh_fromOther_Pt_reco_onetwo_Target);
  CPh_twoFromTarget_Pt_reco_onetwo_Target =
    new TH1D("CPh_twoFromTarget_Pt_reco_onetwo_Target",
             "CPh_twoFromTarget_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(
    CPh_twoFromTarget_Pt_reco_onetwo_Target);
  CPh_fromCombinatorial_Pt_reco_onetwo_Target = new TH1D(
    "CPh_fromCombinatorial_Pt_reco_onetwo_Target",
    "CPh_fromCombinatorial_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
    30,
    0,
    3);
  fHistoList_dp_onetwo_Target.push_back(
    CPh_fromCombinatorial_Pt_reco_onetwo_Target);
  CPh_fromConversion_Pt_reco_onetwo_Target =
    new TH1D("CPh_fromConversion_Pt_reco_onetwo_Target",
             "CPh_fromConversion_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(
    CPh_fromConversion_Pt_reco_onetwo_Target);
  CPh_pt_vs_rap_est_onetwo_Target =
    new TH2D("CPh_pt_vs_rap_est_onetwo_Target",
             "CPh_pt_vs_rap_est_onetwo_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_onetwo_Target.push_back(CPh_pt_vs_rap_est_onetwo_Target);
  CPh_pt_vs_rap_est_corr_onetwo_Target = new TH2D(
    "CPh_pt_vs_rap_est_corr_onetwo_Target",
    "CPh_pt_vs_rap_est_corr_onetwo_Target; rapidity y; p_{t} in GeV/c ",
    10,
    0.,
    4.,
    40,
    0.,
    4.);
  fHistoList_dp_onetwo_Target.push_back(CPh_pt_vs_rap_est_corr_onetwo_Target);


  // Outside => all
  CDP_InvMassReco_all_Outside =
    new TH1D("CDP_InvMassReco_all_Outside",
             "CDP_InvMassReco_all_Outside; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_all_Outside.push_back(CDP_InvMassReco_all_Outside);
  CDP_OpeningAngleReco_all_Outside =
    new TH1D("CDP_OpeningAngleReco_all_Outside",
             "CDP_OpeningAngleReco_all_Outside; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_all_Outside.push_back(CDP_OpeningAngleReco_all_Outside);
  CDP_Pdg_all_Outside = new TH1D(
    "CDP_Pdg_all_Outside", "CDP_Pdg_all_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Outside.push_back(CDP_Pdg_all_Outside);
  CDP_P_reco_all_Outside = new TH1D("CDP_P_reco_all_Outside",
                                    "CDP_P_reco_all_Outside; P in GeV/c^{2};#",
                                    200,
                                    0,
                                    10);
  fHistoList_dp_all_Outside.push_back(CDP_P_reco_all_Outside);
  CDP_Pt_reco_all_Outside =
    new TH1D("CDP_Pt_reco_all_Outside",
             "CDP_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CDP_Pt_reco_all_Outside);
  CPh_fromTarget_Pt_reco_all_Outside =
    new TH1D("CPh_fromTarget_Pt_reco_all_Outside",
             "CPh_fromTarget_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromTarget_Pt_reco_all_Outside);
  CPh_fromPions_Pt_reco_all_Outside =
    new TH1D("CPh_fromPions_Pt_reco_all_Outside",
             "CPh_fromPions_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromPions_Pt_reco_all_Outside);
  CPh_fromEtas_Pt_reco_all_Outside =
    new TH1D("CPh_fromEtas_Pt_reco_all_Outside",
             "CPh_fromEtas_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromEtas_Pt_reco_all_Outside);
  CPh_fromDalitz_Pt_reco_all_Outside =
    new TH1D("CPh_fromDalitz_Pt_reco_all_Outside",
             "CPh_fromDalitz_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromDalitz_Pt_reco_all_Outside);
  CPh_fromXi_Pt_reco_all_Outside =
    new TH1D("CPh_fromXi_Pt_reco_all_Outside",
             "CPh_fromXi_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromXi_Pt_reco_all_Outside);
  CPh_fromOther_Pt_reco_all_Outside =
    new TH1D("CPh_fromOther_Pt_reco_all_Outside",
             "CPh_fromOther_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromOther_Pt_reco_all_Outside);
  CPh_twoFromTarget_Pt_reco_all_Outside =
    new TH1D("CPh_twoFromTarget_Pt_reco_all_Outside",
             "CPh_twoFromTarget_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_twoFromTarget_Pt_reco_all_Outside);
  CPh_fromCombinatorial_Pt_reco_all_Outside =
    new TH1D("CPh_fromCombinatorial_Pt_reco_all_Outside",
             "CPh_fromCombinatorial_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(
    CPh_fromCombinatorial_Pt_reco_all_Outside);
  CPh_fromConversion_Pt_reco_all_Outside =
    new TH1D("CPh_fromConversion_Pt_reco_all_Outside",
             "CPh_fromConversion_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CPh_fromConversion_Pt_reco_all_Outside);
  CPh_pt_vs_rap_est_all_Outside =
    new TH2D("CPh_pt_vs_rap_est_all_Outside",
             "CPh_pt_vs_rap_est_all_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Outside.push_back(CPh_pt_vs_rap_est_all_Outside);
  CPh_pt_vs_rap_est_corr_all_Outside =
    new TH2D("CPh_pt_vs_rap_est_corr_all_Outside",
             "CPh_pt_vs_rap_est_corr_all_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Outside.push_back(CPh_pt_vs_rap_est_corr_all_Outside);

  // Outside => two
  CDP_InvMassReco_two_Outside =
    new TH1D("CDP_InvMassReco_two_Outside",
             "CDP_InvMassReco_two_Outside; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_two_Outside.push_back(CDP_InvMassReco_two_Outside);
  CDP_OpeningAngleReco_two_Outside =
    new TH1D("CDP_OpeningAngleReco_two_Outside",
             "CDP_OpeningAngleReco_two_Outside; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_two_Outside.push_back(CDP_OpeningAngleReco_two_Outside);
  CDP_Pdg_two_Outside = new TH1D(
    "CDP_Pdg_two_Outside", "CDP_Pdg_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(CDP_Pdg_two_Outside);
  CDP_P_reco_two_Outside = new TH1D("CDP_P_reco_two_Outside",
                                    "CDP_P_reco_two_Outside; P in GeV/c^{2};#",
                                    200,
                                    0,
                                    10);
  fHistoList_dp_two_Outside.push_back(CDP_P_reco_two_Outside);
  CDP_Pt_reco_two_Outside =
    new TH1D("CDP_Pt_reco_two_Outside",
             "CDP_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CDP_Pt_reco_two_Outside);
  CPh_fromTarget_Pt_reco_two_Outside =
    new TH1D("CPh_fromTarget_Pt_reco_two_Outside",
             "CPh_fromTarget_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromTarget_Pt_reco_two_Outside);
  CPh_fromPions_Pt_reco_two_Outside =
    new TH1D("CPh_fromPions_Pt_reco_two_Outside",
             "CPh_fromPions_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromPions_Pt_reco_two_Outside);
  CPh_fromEtas_Pt_reco_two_Outside =
    new TH1D("CPh_fromEtas_Pt_reco_two_Outside",
             "CPh_fromEtas_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromEtas_Pt_reco_two_Outside);
  CPh_fromDalitz_Pt_reco_two_Outside =
    new TH1D("CPh_fromDalitz_Pt_reco_two_Outside",
             "CPh_fromDalitz_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromDalitz_Pt_reco_two_Outside);
  CPh_fromXi_Pt_reco_two_Outside =
    new TH1D("CPh_fromXi_Pt_reco_two_Outside",
             "CPh_fromXi_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromXi_Pt_reco_two_Outside);
  CPh_fromOther_Pt_reco_two_Outside =
    new TH1D("CPh_fromOther_Pt_reco_two_Outside",
             "CPh_fromOther_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromOther_Pt_reco_two_Outside);
  CPh_twoFromTarget_Pt_reco_two_Outside =
    new TH1D("CPh_twoFromTarget_Pt_reco_two_Outside",
             "CPh_twoFromTarget_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_twoFromTarget_Pt_reco_two_Outside);
  CPh_fromCombinatorial_Pt_reco_two_Outside =
    new TH1D("CPh_fromCombinatorial_Pt_reco_two_Outside",
             "CPh_fromCombinatorial_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(
    CPh_fromCombinatorial_Pt_reco_two_Outside);
  CPh_fromConversion_Pt_reco_two_Outside =
    new TH1D("CPh_fromConversion_Pt_reco_two_Outside",
             "CPh_fromConversion_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CPh_fromConversion_Pt_reco_two_Outside);
  CPh_pt_vs_rap_est_two_Outside =
    new TH2D("CPh_pt_vs_rap_est_two_Outside",
             "CPh_pt_vs_rap_est_two_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Outside.push_back(CPh_pt_vs_rap_est_two_Outside);
  CPh_pt_vs_rap_est_corr_two_Outside =
    new TH2D("CPh_pt_vs_rap_est_corr_two_Outside",
             "CPh_pt_vs_rap_est_corr_two_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Outside.push_back(CPh_pt_vs_rap_est_corr_two_Outside);

  // Outside => onetwo
  CDP_InvMassReco_onetwo_Outside =
    new TH1D("CDP_InvMassReco_onetwo_Outside",
             "CDP_InvMassReco_onetwo_Outside; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_onetwo_Outside.push_back(CDP_InvMassReco_onetwo_Outside);
  CDP_OpeningAngleReco_onetwo_Outside =
    new TH1D("CDP_OpeningAngleReco_onetwo_Outside",
             "CDP_OpeningAngleReco_onetwo_Outside; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_onetwo_Outside.push_back(CDP_OpeningAngleReco_onetwo_Outside);
  CDP_Pdg_onetwo_Outside = new TH1D(
    "CDP_Pdg_onetwo_Outside", "CDP_Pdg_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(CDP_Pdg_onetwo_Outside);
  CDP_P_reco_onetwo_Outside =
    new TH1D("CDP_P_reco_onetwo_Outside",
             "CDP_P_reco_onetwo_Outside; P in GeV/c^{2};#",
             200,
             0,
             10);
  fHistoList_dp_onetwo_Outside.push_back(CDP_P_reco_onetwo_Outside);
  CDP_Pt_reco_onetwo_Outside =
    new TH1D("CDP_Pt_reco_onetwo_Outside",
             "CDP_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CDP_Pt_reco_onetwo_Outside);
  CPh_fromTarget_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromTarget_Pt_reco_onetwo_Outside",
             "CPh_fromTarget_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromTarget_Pt_reco_onetwo_Outside);
  CPh_fromPions_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromPions_Pt_reco_onetwo_Outside",
             "CPh_fromPions_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromPions_Pt_reco_onetwo_Outside);
  CPh_fromEtas_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromEtas_Pt_reco_onetwo_Outside",
             "CPh_fromEtas_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromEtas_Pt_reco_onetwo_Outside);
  CPh_fromDalitz_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromDalitz_Pt_reco_onetwo_Outside",
             "CPh_fromDalitz_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromDalitz_Pt_reco_onetwo_Outside);
  CPh_fromXi_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromXi_Pt_reco_onetwo_Outside",
             "CPh_fromXi_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromXi_Pt_reco_onetwo_Outside);
  CPh_fromOther_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromOther_Pt_reco_onetwo_Outside",
             "CPh_fromOther_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CPh_fromOther_Pt_reco_onetwo_Outside);
  CPh_twoFromTarget_Pt_reco_onetwo_Outside =
    new TH1D("CPh_twoFromTarget_Pt_reco_onetwo_Outside",
             "CPh_twoFromTarget_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(
    CPh_twoFromTarget_Pt_reco_onetwo_Outside);
  CPh_fromCombinatorial_Pt_reco_onetwo_Outside = new TH1D(
    "CPh_fromCombinatorial_Pt_reco_onetwo_Outside",
    "CPh_fromCombinatorial_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
    30,
    0,
    3);
  fHistoList_dp_onetwo_Outside.push_back(
    CPh_fromCombinatorial_Pt_reco_onetwo_Outside);
  CPh_fromConversion_Pt_reco_onetwo_Outside =
    new TH1D("CPh_fromConversion_Pt_reco_onetwo_Outside",
             "CPh_fromConversion_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(
    CPh_fromConversion_Pt_reco_onetwo_Outside);
  CPh_pt_vs_rap_est_onetwo_Outside =
    new TH2D("CPh_pt_vs_rap_est_onetwo_Outside",
             "CPh_pt_vs_rap_est_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_onetwo_Outside.push_back(CPh_pt_vs_rap_est_onetwo_Outside);
  CPh_pt_vs_rap_est_corr_onetwo_Outside = new TH2D(
    "CPh_pt_vs_rap_est_corr_onetwo_Outside",
    "CPh_pt_vs_rap_est_corr_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
    10,
    0.,
    4.,
    40,
    0.,
    4.);
  fHistoList_dp_onetwo_Outside.push_back(CPh_pt_vs_rap_est_corr_onetwo_Outside);


  // Both => all
  CDP_InvMassReco_all_Both =
    new TH1D("CDP_InvMassReco_all_Both",
             "CDP_InvMassReco_all_Both; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_all_Both.push_back(CDP_InvMassReco_all_Both);
  CDP_OpeningAngleReco_all_Both =
    new TH1D("CDP_OpeningAngleReco_all_Both",
             "CDP_OpeningAngleReco_all_Both; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_all_Both.push_back(CDP_OpeningAngleReco_all_Both);
  CDP_Pdg_all_Both =
    new TH1D("CDP_Pdg_all_Both", "CDP_Pdg_all_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Both.push_back(CDP_Pdg_all_Both);
  CDP_P_reco_all_Both = new TH1D(
    "CDP_P_reco_all_Both", "CDP_P_reco_all_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_all_Both.push_back(CDP_P_reco_all_Both);
  CDP_Pt_reco_all_Both = new TH1D("CDP_Pt_reco_all_Both",
                                  "CDP_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
                                  30,
                                  0,
                                  3);
  fHistoList_dp_all_Both.push_back(CDP_Pt_reco_all_Both);
  CPh_fromTarget_Pt_reco_all_Both =
    new TH1D("CPh_fromTarget_Pt_reco_all_Both",
             "CPh_fromTarget_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromTarget_Pt_reco_all_Both);
  CPh_fromPions_Pt_reco_all_Both =
    new TH1D("CPh_fromPions_Pt_reco_all_Both",
             "CPh_fromPions_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromPions_Pt_reco_all_Both);
  CPh_fromEtas_Pt_reco_all_Both =
    new TH1D("CPh_fromEtas_Pt_reco_all_Both",
             "CPh_fromEtas_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromEtas_Pt_reco_all_Both);
  CPh_fromDalitz_Pt_reco_all_Both =
    new TH1D("CPh_fromDalitz_Pt_reco_all_Both",
             "CPh_fromDalitz_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromDalitz_Pt_reco_all_Both);
  CPh_fromXi_Pt_reco_all_Both =
    new TH1D("CPh_fromXi_Pt_reco_all_Both",
             "CPh_fromXi_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromXi_Pt_reco_all_Both);
  CPh_fromOther_Pt_reco_all_Both =
    new TH1D("CPh_fromOther_Pt_reco_all_Both",
             "CPh_fromOther_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromOther_Pt_reco_all_Both);
  CPh_twoFromTarget_Pt_reco_all_Both =
    new TH1D("CPh_twoFromTarget_Pt_reco_all_Both",
             "CPh_twoFromTarget_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_twoFromTarget_Pt_reco_all_Both);
  CPh_fromCombinatorial_Pt_reco_all_Both =
    new TH1D("CPh_fromCombinatorial_Pt_reco_all_Both",
             "CPh_fromCombinatorial_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromCombinatorial_Pt_reco_all_Both);
  CPh_fromConversion_Pt_reco_all_Both =
    new TH1D("CPh_fromConversion_Pt_reco_all_Both",
             "CPh_fromConversion_Pt_reco_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CPh_fromConversion_Pt_reco_all_Both);
  CPh_pt_vs_rap_est_all_Both =
    new TH2D("CPh_pt_vs_rap_est_all_Both",
             "CPh_pt_vs_rap_est_all_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Both.push_back(CPh_pt_vs_rap_est_all_Both);
  CPh_pt_vs_rap_est_corr_all_Both =
    new TH2D("CPh_pt_vs_rap_est_corr_all_Both",
             "CPh_pt_vs_rap_est_corr_all_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_all_Both.push_back(CPh_pt_vs_rap_est_corr_all_Both);

  // Both => two
  CDP_InvMassReco_two_Both =
    new TH1D("CDP_InvMassReco_two_Both",
             "CDP_InvMassReco_two_Both; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_two_Both.push_back(CDP_InvMassReco_two_Both);
  CDP_OpeningAngleReco_two_Both =
    new TH1D("CDP_OpeningAngleReco_two_Both",
             "CDP_OpeningAngleReco_two_Both; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_two_Both.push_back(CDP_OpeningAngleReco_two_Both);
  CDP_Pdg_two_Both =
    new TH1D("CDP_Pdg_two_Both", "CDP_Pdg_two_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Both.push_back(CDP_Pdg_two_Both);
  CDP_P_reco_two_Both = new TH1D(
    "CDP_P_reco_two_Both", "CDP_P_reco_two_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_two_Both.push_back(CDP_P_reco_two_Both);
  CDP_Pt_reco_two_Both = new TH1D("CDP_Pt_reco_two_Both",
                                  "CDP_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
                                  30,
                                  0,
                                  3);
  fHistoList_dp_two_Both.push_back(CDP_Pt_reco_two_Both);
  CPh_fromTarget_Pt_reco_two_Both =
    new TH1D("CPh_fromTarget_Pt_reco_two_Both",
             "CPh_fromTarget_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromTarget_Pt_reco_two_Both);
  CPh_fromPions_Pt_reco_two_Both =
    new TH1D("CPh_fromPions_Pt_reco_two_Both",
             "CPh_fromPions_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromPions_Pt_reco_two_Both);
  CPh_fromEtas_Pt_reco_two_Both =
    new TH1D("CPh_fromEtas_Pt_reco_two_Both",
             "CPh_fromEtas_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromEtas_Pt_reco_two_Both);
  CPh_fromDalitz_Pt_reco_two_Both =
    new TH1D("CPh_fromDalitz_Pt_reco_two_Both",
             "CPh_fromDalitz_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromDalitz_Pt_reco_two_Both);
  CPh_fromXi_Pt_reco_two_Both =
    new TH1D("CPh_fromXi_Pt_reco_two_Both",
             "CPh_fromXi_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromXi_Pt_reco_two_Both);
  CPh_fromOther_Pt_reco_two_Both =
    new TH1D("CPh_fromOther_Pt_reco_two_Both",
             "CPh_fromOther_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromOther_Pt_reco_two_Both);
  CPh_twoFromTarget_Pt_reco_two_Both =
    new TH1D("CPh_twoFromTarget_Pt_reco_two_Both",
             "CPh_twoFromTarget_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_twoFromTarget_Pt_reco_two_Both);
  CPh_fromCombinatorial_Pt_reco_two_Both =
    new TH1D("CPh_fromCombinatorial_Pt_reco_two_Both",
             "CPh_fromCombinatorial_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromCombinatorial_Pt_reco_two_Both);
  CPh_fromConversion_Pt_reco_two_Both =
    new TH1D("CPh_fromConversion_Pt_reco_two_Both",
             "CPh_fromConversion_Pt_reco_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CPh_fromConversion_Pt_reco_two_Both);
  CPh_pt_vs_rap_est_two_Both =
    new TH2D("CPh_pt_vs_rap_est_two_Both",
             "CPh_pt_vs_rap_est_two_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Both.push_back(CPh_pt_vs_rap_est_two_Both);
  CPh_pt_vs_rap_est_corr_two_Both =
    new TH2D("CPh_pt_vs_rap_est_corr_two_Both",
             "CPh_pt_vs_rap_est_corr_two_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_two_Both.push_back(CPh_pt_vs_rap_est_corr_two_Both);

  // Both => onetwo
  CDP_InvMassReco_onetwo_Both =
    new TH1D("CDP_InvMassReco_onetwo_Both",
             "CDP_InvMassReco_onetwo_Both; invariant mass in GeV/c^{2};#",
             50,
             -0.005,
             0.045);
  fHistoList_dp_onetwo_Both.push_back(CDP_InvMassReco_onetwo_Both);
  CDP_OpeningAngleReco_onetwo_Both =
    new TH1D("CDP_OpeningAngleReco_onetwo_Both",
             "CDP_OpeningAngleReco_onetwo_Both; angle [deg];#",
             45,
             -0.5,
             4.);
  fHistoList_dp_onetwo_Both.push_back(CDP_OpeningAngleReco_onetwo_Both);
  CDP_Pdg_onetwo_Both = new TH1D(
    "CDP_Pdg_onetwo_Both", "CDP_Pdg_onetwo_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Both.push_back(CDP_Pdg_onetwo_Both);
  CDP_P_reco_onetwo_Both = new TH1D("CDP_P_reco_onetwo_Both",
                                    "CDP_P_reco_onetwo_Both; P in GeV/c^{2};#",
                                    200,
                                    0,
                                    10);
  fHistoList_dp_onetwo_Both.push_back(CDP_P_reco_onetwo_Both);
  CDP_Pt_reco_onetwo_Both =
    new TH1D("CDP_Pt_reco_onetwo_Both",
             "CDP_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CDP_Pt_reco_onetwo_Both);
  CPh_fromTarget_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromTarget_Pt_reco_onetwo_Both",
             "CPh_fromTarget_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromTarget_Pt_reco_onetwo_Both);
  CPh_fromPions_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromPions_Pt_reco_onetwo_Both",
             "CPh_fromPions_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromPions_Pt_reco_onetwo_Both);
  CPh_fromEtas_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromEtas_Pt_reco_onetwo_Both",
             "CPh_fromEtas_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromEtas_Pt_reco_onetwo_Both);
  CPh_fromDalitz_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromDalitz_Pt_reco_onetwo_Both",
             "CPh_fromDalitz_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromDalitz_Pt_reco_onetwo_Both);
  CPh_fromXi_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromXi_Pt_reco_onetwo_Both",
             "CPh_fromXi_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromXi_Pt_reco_onetwo_Both);
  CPh_fromOther_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromOther_Pt_reco_onetwo_Both",
             "CPh_fromOther_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromOther_Pt_reco_onetwo_Both);
  CPh_twoFromTarget_Pt_reco_onetwo_Both =
    new TH1D("CPh_twoFromTarget_Pt_reco_onetwo_Both",
             "CPh_twoFromTarget_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_twoFromTarget_Pt_reco_onetwo_Both);
  CPh_fromCombinatorial_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromCombinatorial_Pt_reco_onetwo_Both",
             "CPh_fromCombinatorial_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(
    CPh_fromCombinatorial_Pt_reco_onetwo_Both);
  CPh_fromConversion_Pt_reco_onetwo_Both =
    new TH1D("CPh_fromConversion_Pt_reco_onetwo_Both",
             "CPh_fromConversion_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CPh_fromConversion_Pt_reco_onetwo_Both);
  CPh_pt_vs_rap_est_onetwo_Both =
    new TH2D("CPh_pt_vs_rap_est_onetwo_Both",
             "CPh_pt_vs_rap_est_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_onetwo_Both.push_back(CPh_pt_vs_rap_est_onetwo_Both);
  CPh_pt_vs_rap_est_corr_onetwo_Both =
    new TH2D("CPh_pt_vs_rap_est_corr_onetwo_Both",
             "CPh_pt_vs_rap_est_corr_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             40,
             0.,
             4.);
  fHistoList_dp_onetwo_Both.push_back(CPh_pt_vs_rap_est_corr_onetwo_Both);

  // EMT Target
  CDP_EMT_Pt_all_Target =
    new TH1D("CDP_EMT_Pt_all_Target",
             "CDP_EMT_Pt_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CDP_EMT_Pt_all_Target);
  CDP_EMT_Pt_two_Target =
    new TH1D("CDP_EMT_Pt_two_Target",
             "CDP_EMT_Pt_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CDP_EMT_Pt_two_Target);
  CDP_EMT_Pt_onetwo_Target =
    new TH1D("CDP_EMT_Pt_onetwo_Target",
             "CDP_EMT_Pt_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CDP_EMT_Pt_onetwo_Target);

  // EMT Outside
  CDP_EMT_Pt_all_Outside =
    new TH1D("CDP_EMT_Pt_all_Outside",
             "CDP_EMT_Pt_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CDP_EMT_Pt_all_Outside);
  CDP_EMT_Pt_two_Outside =
    new TH1D("CDP_EMT_Pt_two_Outside",
             "CDP_EMT_Pt_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CDP_EMT_Pt_two_Outside);
  CDP_EMT_Pt_onetwo_Outside =
    new TH1D("CDP_EMT_Pt_onetwo_Outside",
             "CDP_EMT_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CDP_EMT_Pt_onetwo_Outside);

  // EMT Both
  CDP_EMT_Pt_all_Both = new TH1D("CDP_EMT_Pt_all_Both",
                                 "CDP_EMT_Pt_all_Both; P_{t} in GeV/c^{2};#",
                                 30,
                                 0,
                                 3);
  fHistoList_dp_all_Both.push_back(CDP_EMT_Pt_all_Both);
  CDP_EMT_Pt_two_Both = new TH1D("CDP_EMT_Pt_two_Both",
                                 "CDP_EMT_Pt_two_Both; P_{t} in GeV/c^{2};#",
                                 30,
                                 0,
                                 3);
  fHistoList_dp_two_Both.push_back(CDP_EMT_Pt_two_Both);
  CDP_EMT_Pt_onetwo_Both =
    new TH1D("CDP_EMT_Pt_onetwo_Both",
             "CDP_EMT_Pt_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CDP_EMT_Pt_onetwo_Both);


  // CDP_LK_EMT Target
  CDP_LK_EMT_Pt_all_Target =
    new TH1D("CDP_LK_EMT_Pt_all_Target",
             "CDP_LK_EMT_Pt_all_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Target.push_back(CDP_LK_EMT_Pt_all_Target);
  CDP_LK_EMT_Pt_two_Target =
    new TH1D("CDP_LK_EMT_Pt_two_Target",
             "CDP_LK_EMT_Pt_two_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Target.push_back(CDP_LK_EMT_Pt_two_Target);
  CDP_LK_EMT_Pt_onetwo_Target =
    new TH1D("CDP_LK_EMT_Pt_onetwo_Target",
             "CDP_LK_EMT_Pt_onetwo_Target; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Target.push_back(CDP_LK_EMT_Pt_onetwo_Target);


  // CDP_LK_EMT Outside
  CDP_LK_EMT_Pt_all_Outside =
    new TH1D("CDP_LK_EMT_Pt_all_Outside",
             "CDP_LK_EMT_Pt_all_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Outside.push_back(CDP_LK_EMT_Pt_all_Outside);
  CDP_LK_EMT_Pt_two_Outside =
    new TH1D("CDP_LK_EMT_Pt_two_Outside",
             "CDP_LK_EMT_Pt_two_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Outside.push_back(CDP_LK_EMT_Pt_two_Outside);
  CDP_LK_EMT_Pt_onetwo_Outside =
    new TH1D("CDP_LK_EMT_Pt_onetwo_Outside",
             "CDP_LK_EMT_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Outside.push_back(CDP_LK_EMT_Pt_onetwo_Outside);


  // CDP_LK_EMT Both
  CDP_LK_EMT_Pt_all_Both =
    new TH1D("CDP_LK_EMT_Pt_all_Both",
             "CDP_LK_EMT_Pt_all_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_all_Both.push_back(CDP_LK_EMT_Pt_all_Both);
  CDP_LK_EMT_Pt_two_Both =
    new TH1D("CDP_LK_EMT_Pt_two_Both",
             "CDP_LK_EMT_Pt_two_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_two_Both.push_back(CDP_LK_EMT_Pt_two_Both);
  CDP_LK_EMT_Pt_onetwo_Both =
    new TH1D("CDP_LK_EMT_Pt_onetwo_Both",
             "CDP_LK_EMT_Pt_onetwo_Both; P_{t} in GeV/c^{2};#",
             30,
             0,
             3);
  fHistoList_dp_onetwo_Both.push_back(CDP_LK_EMT_Pt_onetwo_Both);
}
