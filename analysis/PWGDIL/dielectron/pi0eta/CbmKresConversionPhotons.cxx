/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ievgenii Kres, Florian Uhlig [committer] */

/**
 *    file CbmKresConversionPhotons.cxx
 *
 *    author Ievgenii Kres
 *    date 26.06.2017
 *    modified 30.01.2020
 *
 *    Class for the reconstruction of direct photons.
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *    Logic is similar to the "CbmKresConversionManual.cxx"
 *
 **/

#include "CbmKresConversionPhotons.h"

#include "CbmGlobalTrack.h"
#include "CbmKFParticleInterface.h"
#include "CbmKresConversionBG.h"
#include "CbmKresFunctions.h"
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

#include <iostream>

#include "KFParticle.h"
#include "L1Field.h"

using namespace std;

CbmKresConversionPhotons::CbmKresConversionPhotons()
  : fTrainPhotons(nullptr)
  , AnnTrainPhotons(0)
  , fAnnPhotonsSelection(nullptr)
  , UseAnnPhotons(0)
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
  , frefmomenta()
  , frefId()
  , fMCtracks()
  , DP_LK_EMT_momenta_minus_Target()
  , DP_LK_EMT_NofRings_minus_Target()
  , DP_LK_EMT_STS_minus_Target()
  , DP_LK_EMT_STS_minus_index_Target()
  , DP_LK_EMT_momenta_plus_Target()
  , DP_LK_EMT_NofRings_plus_Target()
  , DP_LK_EMT_STS_plus_Target()
  , DP_LK_EMT_STS_plus_index_Target()
  , DP_LK_EMT_NofRings_minus_Outside()
  , DP_LK_EMT_STS_minus_Outside()
  , DP_LK_EMT_STS_minus_index_Outside()
  , DP_LK_EMT_NofRings_plus_Outside()
  , DP_LK_EMT_STS_plus_Outside()
  , DP_LK_EMT_STS_plus_index_Outside()
  , DP_EMT_Event_minus_Target()
  , DP_EMT_momenta_minus_Target()
  , DP_EMT_NofRings_minus_Target()
  , DP_EMT_Hits_minus_Target()
  , DP_EMT_Event_plus_Target()
  , DP_EMT_momenta_plus_Target()
  , DP_EMT_NofRings_plus_Target()
  , DP_EMT_Hits_plus_Target()
  , DP_EMT_Event_minus_Outside()
  , DP_EMT_STS_minus_Outside()
  , DP_EMT_KFTrack_minus_Outside()
  , DP_EMT_NofRings_minus_Outside()
  , DP_EMT_Hits_minus_Outside()
  , DP_EMT_Event_plus_Outside()
  , DP_EMT_STS_plus_Outside()
  , DP_EMT_KFTrack_plus_Outside()
  , DP_EMT_NofRings_plus_Outside()
  , DP_EMT_Hits_plus_Outside()
  , Gammas_all_Target()
  , Gammas_zero_Target()
  , Gammas_one_Target()
  , Gammas_two_Target()
  , Gammas_onetwo_Target()
  , Gammas_stsIndex_all_Target()
  , Gammas_stsIndex_zero_Target()
  , Gammas_stsIndex_one_Target()
  , Gammas_stsIndex_two_Target()
  , Gammas_stsIndex_onetwo_Target()
  , Gammas_MC_all_Target()
  , Gammas_MC_zero_Target()
  , Gammas_MC_one_Target()
  , Gammas_MC_two_Target()
  , Gammas_MC_onetwo_Target()
  , Gammas_all_Outside()
  , Gammas_zero_Outside()
  , Gammas_one_Outside()
  , Gammas_two_Outside()
  , Gammas_onetwo_Outside()
  , Gammas_stsIndex_all_Outside()
  , Gammas_stsIndex_zero_Outside()
  , Gammas_stsIndex_one_Outside()
  , Gammas_stsIndex_two_Outside()
  , Gammas_stsIndex_onetwo_Outside()
  , Gammas_MC_all_Outside()
  , Gammas_MC_zero_Outside()
  , Gammas_MC_one_Outside()
  , Gammas_MC_two_Outside()
  , Gammas_MC_onetwo_Outside()
  , Gammas_all_Both()
  , Gammas_zero_Both()
  , Gammas_one_Both()
  , Gammas_two_Both()
  , Gammas_onetwo_Both()
  , Gammas_stsIndex_all_Both()
  , Gammas_stsIndex_zero_Both()
  , Gammas_stsIndex_one_Both()
  , Gammas_stsIndex_two_Both()
  , Gammas_stsIndex_onetwo_Both()
  , Gammas_MC_all_Both()
  , Gammas_MC_zero_Both()
  , Gammas_MC_one_Both()
  , Gammas_MC_two_Both()
  , Gammas_MC_onetwo_Both()
  , fHistoList_dp_Target()
  , Mother_PDG_Target(nullptr)
  , GrandMother_PDG_Target(nullptr)
  , fHistoList_dp_Outside()
  , Mother_PDG_Outside(nullptr)
  , GrandMother_PDG_Outside(nullptr)
  , fHistoList_dp_Both()
  , Pdg_vs_Distance_for_dp(nullptr)
  , P_vs_Distance_for_dp(nullptr)
  , DP_AnnTruePairs(nullptr)
  , DP_AnnFalsePairs(nullptr)
  , DP_AnnTruePairs_AfterCuts(nullptr)
  , DP_AnnFalsePairs_AfterCuts(nullptr)
  , fHistoList_dp_cuts_Both()
  , DP_candidates_InvMass_vs_OA_Both(nullptr)
  , DP_InvMass_vs_OA_Both(nullptr)
  , DP_candidates_InvMass_Both(nullptr)
  , DP_InvMass_Both(nullptr)
  , DP_candidates_OA_Both(nullptr)
  , DP_OA_Both(nullptr)
  , DP_candidates_PlaneAngles_last_Both(nullptr)
  , DP_PlaneAngles_last_Both(nullptr)
  , DP_candidates_PlaneAngles_first_Both(nullptr)
  , DP_PlaneAngles_first_Both(nullptr)
  , fHistoList_dp_cuts_Target()
  , DP_candidates_InvMass_vs_OA_Target(nullptr)
  , DP_InvMass_vs_OA_Target(nullptr)
  , DP_candidates_InvMass_Target(nullptr)
  , DP_InvMass_Target(nullptr)
  , DP_candidates_OA_Target(nullptr)
  , DP_OA_Target(nullptr)
  , DP_candidates_PlaneAngles_last_Target(nullptr)
  , DP_PlaneAngles_last_Target(nullptr)
  , DP_candidates_PlaneAngles_first_Target(nullptr)
  , DP_PlaneAngles_first_Target(nullptr)
  , fHistoList_dp_cuts_Outside()
  , DP_candidates_InvMass_vs_OA_Outside(nullptr)
  , DP_InvMass_vs_OA_Outside(nullptr)
  , DP_candidates_InvMass_Outside(nullptr)
  , DP_InvMass_Outside(nullptr)
  , DP_candidates_OA_Outside(nullptr)
  , DP_OA_Outside(nullptr)
  , DP_candidates_PlaneAngles_last_Outside(nullptr)
  , DP_PlaneAngles_last_Outside(nullptr)
  , DP_candidates_PlaneAngles_first_Outside(nullptr)
  , DP_PlaneAngles_first_Outside(nullptr)
  , fHistoList_dp_all_Target()
  , DP_InvMassReco_all_Target(nullptr)
  , DP_OpeningAngleReco_all_Target(nullptr)
  , DP_Pdg_all_Target(nullptr)
  , DP_P_reco_all_Target(nullptr)
  , DP_Pt_reco_all_Target(nullptr)
  , Ph_fromTarget_Pt_reco_all_Target(nullptr)
  , Ph_fromPions_Pt_reco_all_Target(nullptr)
  , Ph_fromEtas_Pt_reco_all_Target(nullptr)
  , Ph_fromDalitz_Pt_reco_all_Target(nullptr)
  , fHistoList_dp_zero_Target()
  , DP_InvMassReco_zero_Target(nullptr)
  , DP_OpeningAngleReco_zero_Target(nullptr)
  , DP_Pdg_zero_Target(nullptr)
  , DP_P_reco_zero_Target(nullptr)
  , DP_Pt_reco_zero_Target(nullptr)
  , Ph_fromTarget_Pt_reco_zero_Target(nullptr)
  , Ph_fromPions_Pt_reco_zero_Target(nullptr)
  , Ph_fromEtas_Pt_reco_zero_Target(nullptr)
  , Ph_fromDalitz_Pt_reco_zero_Target(nullptr)
  , fHistoList_dp_one_Target()
  , DP_InvMassReco_one_Target(nullptr)
  , DP_OpeningAngleReco_one_Target(nullptr)
  , DP_Pdg_one_Target(nullptr)
  , DP_P_reco_one_Target(nullptr)
  , DP_Pt_reco_one_Target(nullptr)
  , Ph_fromTarget_Pt_reco_one_Target(nullptr)
  , Ph_fromPions_Pt_reco_one_Target(nullptr)
  , Ph_fromEtas_Pt_reco_one_Target(nullptr)
  , Ph_fromDalitz_Pt_reco_one_Target(nullptr)
  , fHistoList_dp_two_Target()
  , DP_InvMassReco_two_Target(nullptr)
  , DP_OpeningAngleReco_two_Target(nullptr)
  , DP_Pdg_two_Target(nullptr)
  , DP_P_reco_two_Target(nullptr)
  , DP_Pt_reco_two_Target(nullptr)
  , Ph_fromTarget_Pt_reco_two_Target(nullptr)
  , Ph_fromPions_Pt_reco_two_Target(nullptr)
  , Ph_fromEtas_Pt_reco_two_Target(nullptr)
  , Ph_fromDalitz_Pt_reco_two_Target(nullptr)
  , Ph_fromXi_Pt_reco_two_Target(nullptr)
  , Ph_fromOther_Pt_reco_two_Target(nullptr)
  , Ph_twoFromTarget_Pt_reco_two_Target(nullptr)
  , Ph_fromCombinatorial_Pt_reco_two_Target(nullptr)
  , Ph_fromConversion_Pt_reco_two_Target(nullptr)
  , twoFromTarget_PDG_two_Target(nullptr)
  , fromCombinatorial_PDG_two_Target(nullptr)
  , CombinatorialGrMotherPdg_two_Target(nullptr)
  , CombinatorialMotherPdg_two_Target(nullptr)
  , Electrons_two_Target(nullptr)
  , Pions_two_Target(nullptr)
  , PionElectron_two_Target(nullptr)
  , elsePionOrElectron_two_Target(nullptr)
  , DalitzAndConversion_Pt_two_Target(nullptr)
  , DoubleConversion_Pt_two_Target(nullptr)
  , fromFireball_P_two_Target(nullptr)
  , twoFromTarget_P_two_Target(nullptr)
  , fromCombinatorial_electron_P_two_Target(nullptr)
  , fromCombinatorial_NOTelectron_P_two_Target(nullptr)
  , fHistoList_dp_onetwo_Target()
  , DP_InvMassReco_onetwo_Target(nullptr)
  , DP_OpeningAngleReco_onetwo_Target(nullptr)
  , DP_Pdg_onetwo_Target(nullptr)
  , DP_P_reco_onetwo_Target(nullptr)
  , DP_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromTarget_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromPions_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromEtas_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromDalitz_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromXi_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromOther_Pt_reco_onetwo_Target(nullptr)
  , Ph_twoFromTarget_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromCombinatorial_Pt_reco_onetwo_Target(nullptr)
  , Ph_fromConversion_Pt_reco_onetwo_Target(nullptr)
  , twoFromTarget_PDG_onetwo_Target(nullptr)
  , fromCombinatorial_PDG_onetwo_Target(nullptr)
  , CombinatorialGrMotherPdg_onetwo_Target(nullptr)
  , CombinatorialMotherPdg_onetwo_Target(nullptr)
  , Electrons_onetwo_Target(nullptr)
  , Pions_onetwo_Target(nullptr)
  , PionElectron_onetwo_Target(nullptr)
  , elsePionOrElectron_onetwo_Target(nullptr)
  , DalitzAndConversion_Pt_onetwo_Target(nullptr)
  , DoubleConversion_Pt_onetwo_Target(nullptr)
  , fromFireball_P_onetwo_Target(nullptr)
  , twoFromTarget_P_onetwo_Target(nullptr)
  , fromCombinatorial_electron_P_onetwo_Target(nullptr)
  , fromCombinatorial_NOTelectron_P_onetwo_Target(nullptr)
  , fHistoList_dp_all_Outside()
  , DP_InvMassReco_all_Outside(nullptr)
  , DP_OpeningAngleReco_all_Outside(nullptr)
  , DP_Pdg_all_Outside(nullptr)
  , DP_P_reco_all_Outside(nullptr)
  , DP_Pt_reco_all_Outside(nullptr)
  , Ph_fromTarget_Pt_reco_all_Outside(nullptr)
  , Ph_fromPions_Pt_reco_all_Outside(nullptr)
  , Ph_fromEtas_Pt_reco_all_Outside(nullptr)
  , Ph_fromDalitz_Pt_reco_all_Outside(nullptr)
  , fHistoList_dp_zero_Outside()
  , DP_InvMassReco_zero_Outside(nullptr)
  , DP_OpeningAngleReco_zero_Outside(nullptr)
  , DP_Pdg_zero_Outside(nullptr)
  , DP_P_reco_zero_Outside(nullptr)
  , DP_Pt_reco_zero_Outside(nullptr)
  , Ph_fromTarget_Pt_reco_zero_Outside(nullptr)
  , Ph_fromPions_Pt_reco_zero_Outside(nullptr)
  , Ph_fromEtas_Pt_reco_zero_Outside(nullptr)
  , Ph_fromDalitz_Pt_reco_zero_Outside(nullptr)
  , fHistoList_dp_one_Outside()
  , DP_InvMassReco_one_Outside(nullptr)
  , DP_OpeningAngleReco_one_Outside(nullptr)
  , DP_Pdg_one_Outside(nullptr)
  , DP_P_reco_one_Outside(nullptr)
  , DP_Pt_reco_one_Outside(nullptr)
  , Ph_fromTarget_Pt_reco_one_Outside(nullptr)
  , Ph_fromPions_Pt_reco_one_Outside(nullptr)
  , Ph_fromEtas_Pt_reco_one_Outside(nullptr)
  , Ph_fromDalitz_Pt_reco_one_Outside(nullptr)
  , fHistoList_dp_two_Outside()
  , DP_InvMassReco_two_Outside(nullptr)
  , DP_OpeningAngleReco_two_Outside(nullptr)
  , DP_Pdg_two_Outside(nullptr)
  , DP_P_reco_two_Outside(nullptr)
  , DP_Pt_reco_two_Outside(nullptr)
  , Ph_fromTarget_Pt_reco_two_Outside(nullptr)
  , Ph_fromPions_Pt_reco_two_Outside(nullptr)
  , Ph_fromEtas_Pt_reco_two_Outside(nullptr)
  , Ph_fromDalitz_Pt_reco_two_Outside(nullptr)
  , Ph_fromXi_Pt_reco_two_Outside(nullptr)
  , Ph_fromOther_Pt_reco_two_Outside(nullptr)
  , Ph_twoFromTarget_Pt_reco_two_Outside(nullptr)
  , Ph_fromCombinatorial_Pt_reco_two_Outside(nullptr)
  , Ph_fromConversion_Pt_reco_two_Outside(nullptr)
  , twoFromTarget_PDG_two_Outside(nullptr)
  , fromCombinatorial_PDG_two_Outside(nullptr)
  , CombinatorialGrMotherPdg_two_Outside(nullptr)
  , CombinatorialMotherPdg_two_Outside(nullptr)
  , Electrons_two_Outside(nullptr)
  , Pions_two_Outside(nullptr)
  , PionElectron_two_Outside(nullptr)
  , elsePionOrElectron_two_Outside(nullptr)
  , DalitzAndConversion_Pt_two_Outside(nullptr)
  , DoubleConversion_Pt_two_Outside(nullptr)
  , fromFireball_P_two_Outside(nullptr)
  , twoFromTarget_P_two_Outside(nullptr)
  , fromCombinatorial_electron_P_two_Outside(nullptr)
  , fromCombinatorial_NOTelectron_P_two_Outside(nullptr)
  , fHistoList_dp_onetwo_Outside()
  , DP_InvMassReco_onetwo_Outside(nullptr)
  , DP_OpeningAngleReco_onetwo_Outside(nullptr)
  , DP_Pdg_onetwo_Outside(nullptr)
  , DP_P_reco_onetwo_Outside(nullptr)
  , DP_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromTarget_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromPions_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromEtas_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromDalitz_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromXi_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromOther_Pt_reco_onetwo_Outside(nullptr)
  , Ph_twoFromTarget_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromCombinatorial_Pt_reco_onetwo_Outside(nullptr)
  , Ph_fromConversion_Pt_reco_onetwo_Outside(nullptr)
  , twoFromTarget_PDG_onetwo_Outside(nullptr)
  , fromCombinatorial_PDG_onetwo_Outside(nullptr)
  , CombinatorialGrMotherPdg_onetwo_Outside(nullptr)
  , CombinatorialMotherPdg_onetwo_Outside(nullptr)
  , Electrons_onetwo_Outside(nullptr)
  , Pions_onetwo_Outside(nullptr)
  , PionElectron_onetwo_Outside(nullptr)
  , elsePionOrElectron_onetwo_Outside(nullptr)
  , DalitzAndConversion_Pt_onetwo_Outside(nullptr)
  , DoubleConversion_Pt_onetwo_Outside(nullptr)
  , fromFireball_P_onetwo_Outside(nullptr)
  , twoFromTarget_P_onetwo_Outside(nullptr)
  , fromCombinatorial_electron_P_onetwo_Outside(nullptr)
  , fromCombinatorial_NOTelectron_P_onetwo_Outside(nullptr)
  , fHistoList_dp_all_Both()
  , DP_InvMassReco_all_Both(nullptr)
  , DP_OpeningAngleReco_all_Both(nullptr)
  , DP_Pdg_all_Both(nullptr)
  , DP_P_reco_all_Both(nullptr)
  , DP_Pt_reco_all_Both(nullptr)
  , Ph_fromTarget_Pt_reco_all_Both(nullptr)
  , Ph_fromPions_Pt_reco_all_Both(nullptr)
  , Ph_fromEtas_Pt_reco_all_Both(nullptr)
  , Ph_fromDalitz_Pt_reco_all_Both(nullptr)
  , Ph_pt_vs_rap_est_all_Both(nullptr)
  , fHistoList_dp_zero_Both()
  , DP_InvMassReco_zero_Both(nullptr)
  , DP_OpeningAngleReco_zero_Both(nullptr)
  , DP_Pdg_zero_Both(nullptr)
  , DP_P_reco_zero_Both(nullptr)
  , DP_Pt_reco_zero_Both(nullptr)
  , Ph_fromTarget_Pt_reco_zero_Both(nullptr)
  , Ph_fromPions_Pt_reco_zero_Both(nullptr)
  , Ph_fromEtas_Pt_reco_zero_Both(nullptr)
  , Ph_fromDalitz_Pt_reco_zero_Both(nullptr)
  , Ph_pt_vs_rap_est_zero_Both(nullptr)
  , fHistoList_dp_one_Both()
  , DP_InvMassReco_one_Both(nullptr)
  , DP_OpeningAngleReco_one_Both(nullptr)
  , DP_Pdg_one_Both(nullptr)
  , DP_P_reco_one_Both(nullptr)
  , DP_Pt_reco_one_Both(nullptr)
  , Ph_fromTarget_Pt_reco_one_Both(nullptr)
  , Ph_fromPions_Pt_reco_one_Both(nullptr)
  , Ph_fromEtas_Pt_reco_one_Both(nullptr)
  , Ph_fromDalitz_Pt_reco_one_Both(nullptr)
  , Ph_pt_vs_rap_est_one_Both(nullptr)
  , fHistoList_dp_two_Both()
  , DP_InvMassReco_two_Both(nullptr)
  , DP_OpeningAngleReco_two_Both(nullptr)
  , DP_Pdg_two_Both(nullptr)
  , DP_P_reco_two_Both(nullptr)
  , DP_Pt_reco_two_Both(nullptr)
  , Ph_fromTarget_Pt_reco_two_Both(nullptr)
  , Ph_fromPions_Pt_reco_two_Both(nullptr)
  , Ph_fromEtas_Pt_reco_two_Both(nullptr)
  , Ph_fromDalitz_Pt_reco_two_Both(nullptr)
  , Ph_twoFromTarget_Pt_reco_two_Both(nullptr)
  , Ph_fromCombinatorial_Pt_reco_two_Both(nullptr)
  , Ph_fromConversion_Pt_reco_two_Both(nullptr)
  , Ph_pt_vs_rap_est_two_Both(nullptr)
  , fHistoList_dp_onetwo_Both()
  , DP_InvMassReco_onetwo_Both(nullptr)
  , DP_OpeningAngleReco_onetwo_Both(nullptr)
  , DP_Pdg_onetwo_Both(nullptr)
  , DP_P_reco_onetwo_Both(nullptr)
  , DP_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromTarget_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromPions_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromEtas_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromDalitz_Pt_reco_onetwo_Both(nullptr)
  , Ph_twoFromTarget_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromCombinatorial_Pt_reco_onetwo_Both(nullptr)
  , Ph_fromConversion_Pt_reco_onetwo_Both(nullptr)
  , Ph_pt_vs_rap_est_onetwo_Both(nullptr)
  , DP_EMT_Pt_all_Target(nullptr)
  , DP_EMT_Pt_zero_Target(nullptr)
  , DP_EMT_Pt_one_Target(nullptr)
  , DP_EMT_Pt_two_Target(nullptr)
  , DP_EMT_Pt_onetwo_Target(nullptr)
  , DP_EMT_Pt_all_Outside(nullptr)
  , DP_EMT_Pt_zero_Outside(nullptr)
  , DP_EMT_Pt_one_Outside(nullptr)
  , DP_EMT_Pt_two_Outside(nullptr)
  , DP_EMT_Pt_onetwo_Outside(nullptr)
  , DP_EMT_Pt_all_Both(nullptr)
  , DP_EMT_Pt_zero_Both(nullptr)
  , DP_EMT_Pt_one_Both(nullptr)
  , DP_EMT_Pt_two_Both(nullptr)
  , DP_EMT_Pt_onetwo_Both(nullptr)
  , DP_LK_EMT_neg_Pt_all_Target(nullptr)
  , DP_LK_EMT_neg_Pt_zero_Target(nullptr)
  , DP_LK_EMT_neg_Pt_one_Target(nullptr)
  , DP_LK_EMT_neg_Pt_two_Target(nullptr)
  , DP_LK_EMT_neg_Pt_onetwo_Target(nullptr)
  , DP_LK_EMT_pos_Pt_all_Target(nullptr)
  , DP_LK_EMT_pos_Pt_zero_Target(nullptr)
  , DP_LK_EMT_pos_Pt_one_Target(nullptr)
  , DP_LK_EMT_pos_Pt_two_Target(nullptr)
  , DP_LK_EMT_pos_Pt_onetwo_Target(nullptr)
  , DP_LK_EMT_Pt_all_Target(nullptr)
  , DP_LK_EMT_Pt_zero_Target(nullptr)
  , DP_LK_EMT_Pt_one_Target(nullptr)
  , DP_LK_EMT_Pt_two_Target(nullptr)
  , DP_LK_EMT_Pt_onetwo_Target(nullptr)
  , DP_LK_EMT_neg_Pt_all_Outside(nullptr)
  , DP_LK_EMT_neg_Pt_zero_Outside(nullptr)
  , DP_LK_EMT_neg_Pt_one_Outside(nullptr)
  , DP_LK_EMT_neg_Pt_two_Outside(nullptr)
  , DP_LK_EMT_neg_Pt_onetwo_Outside(nullptr)
  , DP_LK_EMT_pos_Pt_all_Outside(nullptr)
  , DP_LK_EMT_pos_Pt_zero_Outside(nullptr)
  , DP_LK_EMT_pos_Pt_one_Outside(nullptr)
  , DP_LK_EMT_pos_Pt_two_Outside(nullptr)
  , DP_LK_EMT_pos_Pt_onetwo_Outside(nullptr)
  , DP_LK_EMT_Pt_all_Outside(nullptr)
  , DP_LK_EMT_Pt_zero_Outside(nullptr)
  , DP_LK_EMT_Pt_one_Outside(nullptr)
  , DP_LK_EMT_Pt_two_Outside(nullptr)
  , DP_LK_EMT_Pt_onetwo_Outside(nullptr)
{
}

CbmKresConversionPhotons::~CbmKresConversionPhotons() {}

void CbmKresConversionPhotons::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresConversionPhotons::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresConversionPhotons::Init", "No MCTrack array!"); }

  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) { LOG(fatal) << "CbmKresConversionPhotons::Init  No PrimaryVertex array!"; }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionPhotons::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresConversionPhotons::Init", "No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionPhotons::Init", "No StsTrackMatch array!"); }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) { Fatal("CbmKresConversionPhotons::Init", "No RichProjection array!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmKresConversionPhotons::Init", "No RichRing array!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionPhotons::Init", "No RichRingMatch array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmKresConversionPhotons::Init", "No RichHit array!"); }

  fArrayMvdHit = (TClonesArray*) ioman->GetObject("MvdHit");
  if (nullptr == fArrayMvdHit) { Fatal("CbmKresConversionPhotons::Init", "No MvdHit array!"); }

  fArrayStsHit = (TClonesArray*) ioman->GetObject("StsHit");
  if (nullptr == fArrayStsHit) { Fatal("CbmKresConversionPhotons::Init", "No StsHit array!"); }


  fTauFit = new CbmRichRingFitterEllipseTau();

  InitHistograms();

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
}


void CbmKresConversionPhotons::Exec(int fEventNumDP, double OpeningAngleCut, double GammaInvMassCut, int RealPID)
{
  cout << "CbmKresConversionPhotons, event No. " << fEventNumDP << endl;

  if (fPrimVertex != nullptr) { fKFVertex = CbmKFVertex(*fPrimVertex); }
  else {
    Fatal("CbmKresConversionPhotons::Exec", "No PrimaryVertex array!");
  }


  Gammas_all_Target.clear();
  Gammas_one_Target.clear();
  Gammas_two_Target.clear();
  Gammas_zero_Target.clear();
  Gammas_onetwo_Target.clear();
  Gammas_stsIndex_all_Target.clear();
  Gammas_stsIndex_one_Target.clear();
  Gammas_stsIndex_two_Target.clear();
  Gammas_stsIndex_zero_Target.clear();
  Gammas_stsIndex_onetwo_Target.clear();
  Gammas_MC_all_Target.clear();
  Gammas_MC_one_Target.clear();
  Gammas_MC_two_Target.clear();
  Gammas_MC_zero_Target.clear();
  Gammas_MC_onetwo_Target.clear();
  Gammas_all_Outside.clear();
  Gammas_one_Outside.clear();
  Gammas_two_Outside.clear();
  Gammas_zero_Outside.clear();
  Gammas_onetwo_Outside.clear();
  Gammas_stsIndex_all_Outside.clear();
  Gammas_stsIndex_one_Outside.clear();
  Gammas_stsIndex_two_Outside.clear();
  Gammas_stsIndex_zero_Outside.clear();
  Gammas_stsIndex_onetwo_Outside.clear();
  Gammas_MC_all_Outside.clear();
  Gammas_MC_one_Outside.clear();
  Gammas_MC_two_Outside.clear();
  Gammas_MC_zero_Outside.clear();
  Gammas_MC_onetwo_Outside.clear();
  Gammas_all_Both.clear();
  Gammas_one_Both.clear();
  Gammas_two_Both.clear();
  Gammas_zero_Both.clear();
  Gammas_onetwo_Both.clear();
  Gammas_stsIndex_all_Both.clear();
  Gammas_stsIndex_one_Both.clear();
  Gammas_stsIndex_two_Both.clear();
  Gammas_stsIndex_zero_Both.clear();
  Gammas_stsIndex_onetwo_Both.clear();
  Gammas_MC_all_Both.clear();
  Gammas_MC_one_Both.clear();
  Gammas_MC_two_Both.clear();
  Gammas_MC_zero_Both.clear();
  Gammas_MC_onetwo_Both.clear();


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
    CbmTrackMatchNew* stsMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == nullptr) continue;
    if (stsMatch->GetNofLinks() <= 0) continue;
    Int_t stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
    if (mcTrack == nullptr) continue;


    FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(i);
    if (richInd < 0 && proj->GetX() > -115 && proj->GetX() < 115
        && ((proj->GetY() < -120 && proj->GetY() > -200) || (proj->GetY() > 120 && proj->GetY() < 200)))
      continue;
    CbmRichRing* Ring = nullptr;
    if (richInd > -1) {
      Ring = static_cast<CbmRichRing*>(fRichRings->At(richInd));
      Pdg_vs_Distance_for_dp->Fill(TMath::Abs(mcTrack->GetPdgCode()), CbmRichUtil::GetRingTrackDistance(i));
      if (TMath::Abs(mcTrack->GetPdgCode()) == 11)
        P_vs_Distance_for_dp->Fill(mcTrack->GetP(), CbmRichUtil::GetRingTrackDistance(i));
    }

    // Doing fit with Fit To primary Vertex and calculate chi2 to primary vertex
    double chi2       = 0;
    TVector3 Momentum = CbmKresFunctions::FitToVertexAndGetChi(stsTrack, fKFVertex.GetRefX(), fKFVertex.GetRefY(),
                                                               fKFVertex.GetRefZ(), chi2);
    const FairTrackParam* track_par = stsTrack->GetParamFirst();
    double charge                   = track_par->GetQp();


    if (chi2 != chi2) continue;
    if (chi2 == 0) continue;

    if (chi2 > 3) { SaveOutsideTracks(mcTrack, stsTrack, charge, stsInd, richInd, stsMcTrackId, Ring); }
    if (chi2 > 3) continue;

    SaveTargetTracks(mcTrack, stsTrack, Momentum, charge, stsInd, richInd, stsMcTrackId, Ring);
  }


  FindGammasTarget(fEventNumDP, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Target,
                   VMCtracks_plus_Target, VStsTrack_minus_Target, VStsTrack_plus_Target, VMomenta_minus_Target,
                   VMomenta_plus_Target, VRings_minus_Target, VRings_plus_Target, VStsIndex_minus_Target,
                   VStsIndex_plus_Target, VRichRing_minus_Target, VRichRing_plus_Target);

  FindGammasOutside(fEventNumDP, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Outside,
                    VMCtracks_plus_Outside, VStsTrack_minus_Outside, VStsTrack_plus_Outside, VRings_minus_Outside,
                    VRings_plus_Outside, VStsIndex_minus_Outside, VStsIndex_plus_Outside, VRichRing_minus_Outside,
                    VRichRing_plus_Outside);

  FindGammasBoth();


  if (UseAnnPhotons == 1) {
    int numformix = 1000;
    if (fEventNumDP % numformix == 0) {
      DP_Mixing_Target(OpeningAngleCut, GammaInvMassCut);
      DP_EMT_Event_minus_Target.clear();
      DP_EMT_momenta_minus_Target.clear();
      DP_EMT_NofRings_minus_Target.clear();
      DP_EMT_Hits_minus_Target.clear();

      DP_EMT_Event_plus_Target.clear();
      DP_EMT_momenta_plus_Target.clear();
      DP_EMT_NofRings_plus_Target.clear();
      DP_EMT_Hits_plus_Target.clear();
    }

    if (fEventNumDP % numformix == 0) {
      DP_Mixing_Outside(OpeningAngleCut, GammaInvMassCut);
      DP_EMT_Event_minus_Outside.clear();
      DP_EMT_STS_minus_Outside.clear();
      DP_EMT_KFTrack_minus_Outside.clear();
      DP_EMT_NofRings_minus_Outside.clear();
      DP_EMT_Hits_minus_Outside.clear();

      DP_EMT_Event_plus_Outside.clear();
      DP_EMT_STS_plus_Outside.clear();
      DP_EMT_KFTrack_plus_Outside.clear();
      DP_EMT_NofRings_plus_Outside.clear();
      DP_EMT_Hits_plus_Outside.clear();
    }

    // DP_LK_EMT Target
    DP_likesign_Mixing_Target(OpeningAngleCut, GammaInvMassCut);
    DP_LK_EMT_momenta_minus_Target.clear();
    DP_LK_EMT_NofRings_minus_Target.clear();
    DP_LK_EMT_STS_minus_Target.clear();
    DP_LK_EMT_STS_minus_index_Target.clear();
    DP_LK_EMT_momenta_plus_Target.clear();
    DP_LK_EMT_NofRings_plus_Target.clear();
    DP_LK_EMT_STS_plus_Target.clear();
    DP_LK_EMT_STS_plus_index_Target.clear();

    // DP_LK_EMT Outside
    DP_likesign_Mixing_Outside(OpeningAngleCut, GammaInvMassCut);
    DP_LK_EMT_NofRings_minus_Outside.clear();
    DP_LK_EMT_STS_minus_Outside.clear();
    DP_LK_EMT_STS_minus_index_Outside.clear();
    DP_LK_EMT_NofRings_plus_Outside.clear();
    DP_LK_EMT_STS_plus_Outside.clear();
    DP_LK_EMT_STS_plus_index_Outside.clear();
  }
}


void CbmKresConversionPhotons::SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd,
                                                 int richInd, int stsMcTrackId, CbmRichRing* RING)
{
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

void CbmKresConversionPhotons::SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom,
                                                double charge, int stsInd, int richInd, int stsMcTrackId,
                                                CbmRichRing* RING)
{
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


void CbmKresConversionPhotons::FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                                                vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus,
                                                vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                                                vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus,
                                                std::vector<int> Rings_minus, std::vector<int> Rings_plus,
                                                std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus,
                                                vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus)
{
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
        richcheck_0 = CheckIfElectron(richRing_minus[i], Momenta_minus[i].Mag());
        richcheck_1 = CheckIfElectron(richRing_plus[j], Momenta_plus[j].Mag());
      }
      else {
        // MC   PID
        richcheck_0 = Rings_minus[i];
        richcheck_1 = Rings_plus[j];
      }
      int richcheck = richcheck_0 + richcheck_1;

      Double_t InvmassReco          = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle         = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      DP_candidates_InvMass_vs_OA_Target->Fill(InvmassReco, OpeningAngle);
      DP_candidates_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
      DP_candidates_InvMass_Target->Fill(InvmassReco);
      DP_candidates_InvMass_Both->Fill(InvmassReco);
      DP_candidates_OA_Target->Fill(OpeningAngle);
      DP_candidates_OA_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(sts1, sts2);
      double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);

      int IdForANN = 0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() == -1 && mcTrackmama->GetPdgCode() == 22) {
          IdForANN = 1;
          DP_InvMass_Target->Fill(InvmassReco);
          DP_InvMass_Both->Fill(InvmassReco);
          DP_OA_Target->Fill(OpeningAngle);
          DP_OA_Both->Fill(OpeningAngle);
          DP_InvMass_vs_OA_Target->Fill(InvmassReco, OpeningAngle);
          DP_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
          DP_PlaneAngles_last_Target->Fill(PlaneAngle_last);
          DP_PlaneAngles_last_Both->Fill(PlaneAngle_last);
          DP_PlaneAngles_first_Target->Fill(PlaneAngle_first);
          DP_PlaneAngles_first_Both->Fill(PlaneAngle_first);
        }
      }

      // run ANN
      if (AnnTrainPhotons == 1) {
        fTrainPhotons->Exec(EventNumMan, IdForANN, InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(),
                            part1, part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnnPhotons == 1) {
        AnnValue =
          fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), part1, part2);
        if (IdForANN == 1) DP_AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) DP_AnnFalsePairs->Fill(AnnValue);
      }


      DP_candidates_PlaneAngles_last_Target->Fill(PlaneAngle_last);
      DP_candidates_PlaneAngles_last_Both->Fill(PlaneAngle_last);
      DP_candidates_PlaneAngles_first_Target->Fill(PlaneAngle_first);
      DP_candidates_PlaneAngles_first_Both->Fill(PlaneAngle_first);


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
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) { fromFireball = 1; }
          else {
            CbmMCTrack* mcTrackGrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            if (mcTrackGrmama->GetPdgCode() == 111) fromPions = 1;
            if (mcTrackGrmama->GetPdgCode() == 221) fromEta = 1;
            if (mcTrackGrmama->GetPdgCode() == 3212) fromXi = 1;
            fromConversion = 1;
          }
        }
        else {
          if (mcTrackmama->GetPdgCode() == 111) fromDalitz = 1;
          if (mcTrackmama->GetPdgCode() != 111) fromOther = 1;
        }
      }
      else if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() == -1) {
        twoFromTarget = 1;
      }
      else {
        fromCombinatorial = 1;
      }


      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        Mother_PDG_Target->Fill(mcTrackmama->GetPdgCode());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) { GrandMother_PDG_Target->Fill(mcTrackmama->GetMotherId()); }
          else {
            CbmMCTrack* mcTrackGrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            GrandMother_PDG_Target->Fill(TMath::Abs(mcTrackGrmama->GetPdgCode()));
          }
        }
      }


      if (IdForANN == 1) DP_AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) DP_AnnFalsePairs_AfterCuts->Fill(AnnValue);

      frefmomenta.clear();
      frefmomenta.push_back(part1);
      frefmomenta.push_back(part2);
      frefId.clear();
      frefId.push_back(stsIndex_minus[i]);
      frefId.push_back(stsIndex_plus[j]);
      fMCtracks.clear();
      fMCtracks.push_back(part1MC);
      fMCtracks.push_back(part2MC);


      // for event mixing DP
      std::vector<TVector3> minusTrack = SaveAllHits(sts1);
      std::vector<TVector3> plusTrack  = SaveAllHits(sts2);

      DP_EMT_Event_minus_Target.push_back(EventNumMan);
      DP_EMT_momenta_minus_Target.push_back(part1);
      DP_EMT_NofRings_minus_Target.push_back(richcheck_0);
      DP_EMT_Hits_minus_Target.push_back(minusTrack);

      DP_EMT_Event_plus_Target.push_back(EventNumMan);
      DP_EMT_momenta_plus_Target.push_back(part2);
      DP_EMT_NofRings_plus_Target.push_back(richcheck_1);
      DP_EMT_Hits_plus_Target.push_back(plusTrack);

      // DP_LK_EMT
      DP_LK_EMT_momenta_minus_Target.push_back(part1);
      DP_LK_EMT_NofRings_minus_Target.push_back(richcheck_0);
      DP_LK_EMT_STS_minus_Target.push_back(sts1);
      DP_LK_EMT_STS_minus_index_Target.push_back(sts1_index);

      DP_LK_EMT_momenta_plus_Target.push_back(part2);
      DP_LK_EMT_NofRings_plus_Target.push_back(richcheck_1);
      DP_LK_EMT_STS_plus_Target.push_back(sts2);
      DP_LK_EMT_STS_plus_index_Target.push_back(sts2_index);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Target.push_back(frefmomenta);
        Gammas_stsIndex_all_Target.push_back(frefId);
        Gammas_MC_all_Target.push_back(fMCtracks);
        DP_InvMassReco_all_Target->Fill(InvmassReco);
        DP_OpeningAngleReco_all_Target->Fill(OpeningAngle);
        DP_Pdg_all_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_all_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_all_Target->Fill(params.fMomentumMag);
        DP_Pt_reco_all_Target->Fill(params.fPt);
        DP_InvMassReco_all_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_all_Both->Fill(OpeningAngle);
        DP_Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_all_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_all_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_all_Target->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_all_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_all_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_all_Target->Fill(params.fPt);
          Ph_fromPions_Pt_reco_all_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_all_Target->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_all_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_all_Target->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_all_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 0
      if (richcheck == 0) {
        Gammas_zero_Target.push_back(frefmomenta);
        Gammas_stsIndex_zero_Target.push_back(frefId);
        Gammas_MC_zero_Target.push_back(fMCtracks);
        DP_InvMassReco_zero_Target->Fill(InvmassReco);
        DP_OpeningAngleReco_zero_Target->Fill(OpeningAngle);
        DP_Pdg_zero_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_zero_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_zero_Target->Fill(params.fMomentumMag);
        DP_Pt_reco_zero_Target->Fill(params.fPt);
        DP_InvMassReco_zero_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_zero_Both->Fill(OpeningAngle);
        DP_Pdg_zero_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_zero_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_zero_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_zero_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_zero_Target->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_zero_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_zero_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_zero_Target->Fill(params.fPt);
          Ph_fromPions_Pt_reco_zero_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_zero_Target->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_zero_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_zero_Target->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_zero_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 1
      if (richcheck == 1) {
        Gammas_one_Target.push_back(frefmomenta);
        Gammas_stsIndex_one_Target.push_back(frefId);
        Gammas_MC_one_Target.push_back(fMCtracks);
        DP_InvMassReco_one_Target->Fill(InvmassReco);
        DP_OpeningAngleReco_one_Target->Fill(OpeningAngle);
        DP_Pdg_one_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_one_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_one_Target->Fill(params.fMomentumMag);
        DP_Pt_reco_one_Target->Fill(params.fPt);
        DP_InvMassReco_one_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_one_Both->Fill(OpeningAngle);
        DP_Pdg_one_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_one_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_one_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_one_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_one_Target->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_one_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_one_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_one_Target->Fill(params.fPt);
          Ph_fromPions_Pt_reco_one_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_one_Target->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_one_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_one_Target->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_one_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Target.push_back(frefmomenta);
        Gammas_stsIndex_two_Target.push_back(frefId);
        Gammas_MC_two_Target.push_back(fMCtracks);
        DP_InvMassReco_two_Target->Fill(InvmassReco);
        DP_OpeningAngleReco_two_Target->Fill(OpeningAngle);
        DP_Pdg_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_two_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_two_Target->Fill(params.fMomentumMag);
        DP_Pt_reco_two_Target->Fill(params.fPt);
        DP_InvMassReco_two_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_two_Both->Fill(OpeningAngle);
        DP_Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_two_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_two_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_two_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_two_Both->Fill(params.fRapidity, params.fPt);
          fromFireball_P_two_Target->Fill(part1.Mag());
          fromFireball_P_two_Target->Fill(part2.Mag());
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromPions_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromXi == 1) { Ph_fromXi_Pt_reco_two_Target->Fill(params.fPt); }
        if (fromOther == 1) { Ph_fromOther_Pt_reco_two_Target->Fill(params.fPt); }
        if (fromCombinatorial == 1) {

          // particle 1
          if (part1MC->GetMotherId() != -1) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CombinatorialMotherPdg_two_Target->Fill(TMath::Abs(mama1->GetPdgCode()));
            if (mama1->GetMotherId() != -1) {
              CbmMCTrack* grmama1 = (CbmMCTrack*) fMcTracks->At(mama1->GetMotherId());
              CombinatorialGrMotherPdg_two_Target->Fill(TMath::Abs(grmama1->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_two_Target->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_two_Target->Fill(-1);
          }

          // particle 2
          if (part2MC->GetMotherId() != -1) {
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            CombinatorialMotherPdg_two_Target->Fill(TMath::Abs(mama2->GetPdgCode()));
            if (mama2->GetMotherId() != -1) {
              CbmMCTrack* grmama2 = (CbmMCTrack*) fMcTracks->At(mama2->GetMotherId());
              CombinatorialGrMotherPdg_two_Target->Fill(TMath::Abs(grmama2->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_two_Target->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_two_Target->Fill(-1);
          }

          // Check sources of BG
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            Electrons_two_Target->Fill(params.fPt);
          }
          else if (TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 211) {
            Pions_two_Target->Fill(params.fPt);
          }
          else if ((TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 11)
                   || (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 211)) {
            PionElectron_two_Target->Fill(params.fPt);
          }
          else {
            elsePionOrElectron_two_Target->Fill(params.fPt);
          }

          // check special cases of BG, when two particles are electrons, but wrongly combined (with some correlation)
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            int mama1PDG      = mama1->GetPdgCode();
            int mama2PDG      = mama2->GetPdgCode();
            if (
              (mama1PDG == 22 && mama2PDG == 111 && mama1->GetMotherId() == part2MC->GetMotherId())
              || (mama1PDG == 111 && mama2PDG == 22
                  && mama2->GetMotherId()
                       == part1MC
                            ->GetMotherId())) {  // pi -> e+e-g    ===> combined wrongly (e+- from dalitz and e-+ from conversion)
              DalitzAndConversion_Pt_two_Target->Fill(params.fPt);
            }
            if (
              mama1PDG == 22 && mama2PDG == 22 && mama1->GetMotherId() == mama2->GetMotherId()
              && part1MC->GetMotherId()
                   != part2MC
                        ->GetMotherId()) {  // pi -> gg    ===> combined wrongly (e+- from first and e-+ from second conversion)
              DoubleConversion_Pt_two_Target->Fill(params.fPt);
            }
          }

          Ph_fromCombinatorial_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromCombinatorial_Pt_reco_two_Both->Fill(params.fPt);
          fromCombinatorial_PDG_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          fromCombinatorial_PDG_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          if (TMath::Abs(part1MC->GetPdgCode() == 11)) { fromCombinatorial_electron_P_two_Target->Fill(part1.Mag()); }
          else {
            fromCombinatorial_NOTelectron_P_two_Target->Fill(part1.Mag());
          }
          if (TMath::Abs(part2MC->GetPdgCode() == 11)) { fromCombinatorial_electron_P_two_Target->Fill(part2.Mag()); }
          else {
            fromCombinatorial_NOTelectron_P_two_Target->Fill(part2.Mag());
          }
        }
        if (fromConversion == 1) {
          Ph_fromConversion_Pt_reco_two_Target->Fill(params.fPt);
          Ph_fromConversion_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (twoFromTarget == 1) {
          Ph_twoFromTarget_Pt_reco_two_Target->Fill(params.fPt);
          Ph_twoFromTarget_Pt_reco_two_Both->Fill(params.fPt);
          twoFromTarget_PDG_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_PDG_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_P_two_Target->Fill(TMath::Abs(part1.Mag()));
          twoFromTarget_P_two_Target->Fill(TMath::Abs(part2.Mag()));
        }
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Target.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Target.push_back(frefId);
        Gammas_MC_onetwo_Target.push_back(fMCtracks);
        DP_InvMassReco_onetwo_Target->Fill(InvmassReco);
        DP_OpeningAngleReco_onetwo_Target->Fill(OpeningAngle);
        DP_Pdg_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_onetwo_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_onetwo_Target->Fill(params.fMomentumMag);
        DP_Pt_reco_onetwo_Target->Fill(params.fPt);
        DP_InvMassReco_onetwo_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_onetwo_Both->Fill(OpeningAngle);
        DP_Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_onetwo_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_onetwo_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_onetwo_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_onetwo_Both->Fill(params.fRapidity, params.fPt);
          fromFireball_P_onetwo_Target->Fill(part1.Mag());
          fromFireball_P_onetwo_Target->Fill(part2.Mag());
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromPions_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromXi == 1) { Ph_fromXi_Pt_reco_onetwo_Target->Fill(params.fPt); }
        if (fromOther == 1) { Ph_fromOther_Pt_reco_onetwo_Target->Fill(params.fPt); }
        if (fromCombinatorial == 1) {

          // particle 1
          if (part1MC->GetMotherId() != -1) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CombinatorialMotherPdg_onetwo_Target->Fill(TMath::Abs(mama1->GetPdgCode()));
            if (mama1->GetMotherId() != -1) {
              CbmMCTrack* grmama1 = (CbmMCTrack*) fMcTracks->At(mama1->GetMotherId());
              CombinatorialGrMotherPdg_onetwo_Target->Fill(TMath::Abs(grmama1->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_onetwo_Target->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_onetwo_Target->Fill(-1);
          }

          // particle 2
          if (part2MC->GetMotherId() != -1) {
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            CombinatorialMotherPdg_onetwo_Target->Fill(TMath::Abs(mama2->GetPdgCode()));
            if (mama2->GetMotherId() != -1) {
              CbmMCTrack* grmama2 = (CbmMCTrack*) fMcTracks->At(mama2->GetMotherId());
              CombinatorialGrMotherPdg_onetwo_Target->Fill(TMath::Abs(grmama2->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_onetwo_Target->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_onetwo_Target->Fill(-1);
          }

          // Check sources of BG
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            Electrons_onetwo_Target->Fill(params.fPt);
          }
          else if (TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 211) {
            Pions_onetwo_Target->Fill(params.fPt);
          }
          else if ((TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 11)
                   || (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 211)) {
            PionElectron_onetwo_Target->Fill(params.fPt);
          }
          else {
            elsePionOrElectron_onetwo_Target->Fill(params.fPt);
          }

          // check special cases of BG, when two particles are electrons, but wrongly combined (with some correlation)
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            int mama1PDG      = mama1->GetPdgCode();
            int mama2PDG      = mama2->GetPdgCode();
            if (
              (mama1PDG == 22 && mama2PDG == 111 && mama1->GetMotherId() == part2MC->GetMotherId())
              || (mama1PDG == 111 && mama2PDG == 22
                  && mama2->GetMotherId()
                       == part1MC
                            ->GetMotherId())) {  // pi -> e+e-g    ===> combined wrongly (e+- from dalitz and e-+ from conversion)
              DalitzAndConversion_Pt_onetwo_Target->Fill(params.fPt);
            }
            if (
              mama1PDG == 22 && mama2PDG == 22 && mama1->GetMotherId() == mama2->GetMotherId()
              && part1MC->GetMotherId()
                   != part2MC
                        ->GetMotherId()) {  // pi -> gg    ===> combined wrongly (e+- from first and e-+ from second conversion)
              DoubleConversion_Pt_onetwo_Target->Fill(params.fPt);
            }
          }

          Ph_fromCombinatorial_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromCombinatorial_Pt_reco_onetwo_Both->Fill(params.fPt);
          fromCombinatorial_PDG_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          fromCombinatorial_PDG_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          if (TMath::Abs(part1MC->GetPdgCode() == 11)) {
            fromCombinatorial_electron_P_onetwo_Target->Fill(part1.Mag());
          }
          else {
            fromCombinatorial_NOTelectron_P_onetwo_Target->Fill(part1.Mag());
          }
          if (TMath::Abs(part2MC->GetPdgCode() == 11)) {
            fromCombinatorial_electron_P_onetwo_Target->Fill(part2.Mag());
          }
          else {
            fromCombinatorial_NOTelectron_P_onetwo_Target->Fill(part2.Mag());
          }
        }
        if (fromConversion == 1) {
          Ph_fromConversion_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_fromConversion_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (twoFromTarget == 1) {
          Ph_twoFromTarget_Pt_reco_onetwo_Target->Fill(params.fPt);
          Ph_twoFromTarget_Pt_reco_onetwo_Both->Fill(params.fPt);
          twoFromTarget_PDG_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_PDG_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_P_onetwo_Target->Fill(TMath::Abs(part1.Mag()));
          twoFromTarget_P_onetwo_Target->Fill(TMath::Abs(part2.Mag()));
        }
      }
    }
  }
}


void CbmKresConversionPhotons::FindGammasOutside(
  int EventNumMan, double AngleCut, double InvMassCut, int RealPID, vector<CbmMCTrack*> MCtracks_minus_Outside,
  vector<CbmMCTrack*> MCtracks_plus_Outside, vector<CbmStsTrack*> StsTrack_minus_Outside,
  vector<CbmStsTrack*> StsTrack_plus_Outside, std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
  std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside,
  vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside)
{
  for (size_t i = 0; i < StsTrack_minus_Outside.size(); i++) {
    for (size_t j = 0; j < StsTrack_plus_Outside.size(); j++) {

      CbmStsTrack* part1STS = StsTrack_minus_Outside[i];
      CbmStsTrack* part2STS = StsTrack_plus_Outside[j];
      CbmMCTrack* part1MC   = MCtracks_minus_Outside[i];
      CbmMCTrack* part2MC   = MCtracks_plus_Outside[j];
      int sts1_index        = stsIndex_minus_Outside[i];
      //			int sts2_index = stsIndex_plus_Outside[j];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(part1STS, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(part2STS, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5) continue;  // kick weird intersections

      // fit to the vertex fitter
      TVector3 part1 =
        CbmKresFunctions::FitToVertex(part1STS, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      TVector3 part2 =
        CbmKresFunctions::FitToVertex(part2STS, intersection.GetX(), intersection.GetY(), intersection.GetZ());

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
      }
      else {
        // MC   PID
        richcheck = Rings_minus_Outside[i] + Rings_plus_Outside[j];
      }


      Double_t InvmassReco          = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle         = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      DP_candidates_InvMass_vs_OA_Outside->Fill(InvmassReco, OpeningAngle);
      DP_candidates_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
      DP_candidates_InvMass_Outside->Fill(InvmassReco);
      DP_candidates_InvMass_Both->Fill(InvmassReco);
      DP_candidates_OA_Outside->Fill(OpeningAngle);
      DP_candidates_OA_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(part1STS, part2STS);
      double PlaneAngle_first = CalculatePlaneAngle_first(part1STS, part2STS);

      int IdForANN = 0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() == -1 && mcTrackmama->GetPdgCode() == 22) {
          IdForANN = 1;
          DP_InvMass_Outside->Fill(InvmassReco);
          DP_InvMass_Both->Fill(InvmassReco);
          DP_OA_Outside->Fill(OpeningAngle);
          DP_OA_Both->Fill(OpeningAngle);
          DP_InvMass_vs_OA_Outside->Fill(InvmassReco, OpeningAngle);
          DP_InvMass_vs_OA_Both->Fill(InvmassReco, OpeningAngle);
          DP_PlaneAngles_last_Outside->Fill(PlaneAngle_last);
          DP_PlaneAngles_last_Both->Fill(PlaneAngle_last);
          DP_PlaneAngles_first_Outside->Fill(PlaneAngle_first);
          DP_PlaneAngles_first_Both->Fill(PlaneAngle_first);
        }
      }

      // run ANN
      if (AnnTrainPhotons == 1) {
        fTrainPhotons->Exec(EventNumMan, IdForANN, InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(),
                            part1, part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnnPhotons == 1) {
        AnnValue =
          fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), part1, part2);
        if (IdForANN == 1) DP_AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) DP_AnnFalsePairs->Fill(AnnValue);
      }

      DP_candidates_PlaneAngles_last_Outside->Fill(PlaneAngle_last);
      DP_candidates_PlaneAngles_last_Both->Fill(PlaneAngle_last);
      DP_candidates_PlaneAngles_first_Outside->Fill(PlaneAngle_first);
      DP_candidates_PlaneAngles_first_Both->Fill(PlaneAngle_first);

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
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) { fromFireball = 1; }
          else {
            CbmMCTrack* mcTrackGrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            if (mcTrackGrmama->GetPdgCode() == 111) fromPions = 1;
            if (mcTrackGrmama->GetPdgCode() == 221) fromEta = 1;
            if (mcTrackGrmama->GetPdgCode() == 3212) fromXi = 1;
            fromConversion = 1;
          }
        }
        else {
          if (mcTrackmama->GetPdgCode() == 111) fromDalitz = 1;
          if (mcTrackmama->GetPdgCode() != 111) fromOther = 1;
        }
      }
      else if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() == -1) {
        twoFromTarget = 1;
      }
      else {
        fromCombinatorial = 1;
      }


      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetPdgCode() == 22) {
          if (mcTrackmama->GetMotherId() == -1) { GrandMother_PDG_Outside->Fill(mcTrackmama->GetMotherId()); }
          else {
            CbmMCTrack* mcTrackGrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
            GrandMother_PDG_Outside->Fill(TMath::Abs(mcTrackGrmama->GetPdgCode()));
          }
        }
        else {
          Mother_PDG_Outside->Fill(TMath::Abs(mcTrackmama->GetPdgCode()));
        }
      }

      if (IdForANN == 1) DP_AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) DP_AnnFalsePairs_AfterCuts->Fill(AnnValue);

      frefmomenta.clear();
      frefmomenta.push_back(part1);
      frefmomenta.push_back(part2);

      frefId.clear();
      frefId.push_back(stsIndex_minus_Outside[i]);
      frefId.push_back(stsIndex_plus_Outside[j]);
      fMCtracks.clear();
      fMCtracks.push_back(part1MC);
      fMCtracks.push_back(part2MC);


      // for event mixing DP
      std::vector<TVector3> minusTrack = SaveAllHits(part1STS);
      std::vector<TVector3> plusTrack  = SaveAllHits(part2STS);


      DP_EMT_Event_minus_Outside.push_back(EventNumMan);
      DP_EMT_STS_minus_Outside.push_back(part1STS);
      DP_EMT_KFTrack_minus_Outside.push_back(electron);
      DP_EMT_NofRings_minus_Outside.push_back(richcheck_0);
      DP_EMT_Hits_minus_Outside.push_back(minusTrack);

      DP_EMT_Event_plus_Outside.push_back(EventNumMan);
      DP_EMT_STS_plus_Outside.push_back(part2STS);
      DP_EMT_KFTrack_plus_Outside.push_back(positron);
      DP_EMT_NofRings_plus_Outside.push_back(richcheck_1);
      DP_EMT_Hits_plus_Outside.push_back(plusTrack);

      // DP_LK_EMT
      DP_LK_EMT_NofRings_minus_Outside.push_back(richcheck_0);
      DP_LK_EMT_STS_minus_Outside.push_back(part1STS);
      DP_LK_EMT_STS_minus_index_Outside.push_back(sts1_index);

      DP_LK_EMT_NofRings_plus_Outside.push_back(richcheck_1);
      DP_LK_EMT_STS_plus_Outside.push_back(part2STS);
      DP_LK_EMT_STS_plus_index_Outside.push_back(sts1_index);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Outside.push_back(frefmomenta);
        Gammas_stsIndex_all_Outside.push_back(frefId);
        Gammas_MC_all_Outside.push_back(fMCtracks);
        DP_InvMassReco_all_Outside->Fill(InvmassReco);
        DP_OpeningAngleReco_all_Outside->Fill(OpeningAngle);
        DP_Pdg_all_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_all_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_all_Outside->Fill(params.fMomentumMag);
        DP_Pt_reco_all_Outside->Fill(params.fPt);
        DP_InvMassReco_all_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_all_Both->Fill(OpeningAngle);
        DP_Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_all_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_all_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_all_Outside->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_all_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_all_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_all_Outside->Fill(params.fPt);
          Ph_fromPions_Pt_reco_all_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_all_Outside->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_all_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_all_Outside->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_all_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 0
      if (richcheck == 0) {
        Gammas_zero_Outside.push_back(frefmomenta);
        Gammas_stsIndex_zero_Outside.push_back(frefId);
        Gammas_MC_zero_Outside.push_back(fMCtracks);
        DP_InvMassReco_zero_Outside->Fill(InvmassReco);
        DP_OpeningAngleReco_zero_Outside->Fill(OpeningAngle);
        DP_Pdg_zero_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_zero_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_zero_Outside->Fill(params.fMomentumMag);
        DP_Pt_reco_zero_Outside->Fill(params.fPt);
        DP_InvMassReco_zero_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_zero_Both->Fill(OpeningAngle);
        DP_Pdg_zero_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_zero_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_zero_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_zero_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_zero_Outside->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_zero_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_zero_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_zero_Outside->Fill(params.fPt);
          Ph_fromPions_Pt_reco_zero_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_zero_Outside->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_zero_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_zero_Outside->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_zero_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 1
      if (richcheck == 1) {
        Gammas_one_Outside.push_back(frefmomenta);
        Gammas_stsIndex_one_Outside.push_back(frefId);
        Gammas_MC_one_Outside.push_back(fMCtracks);
        DP_InvMassReco_one_Outside->Fill(InvmassReco);
        DP_OpeningAngleReco_one_Outside->Fill(OpeningAngle);
        DP_Pdg_one_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_one_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_one_Outside->Fill(params.fMomentumMag);
        DP_Pt_reco_one_Outside->Fill(params.fPt);
        DP_InvMassReco_one_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_one_Both->Fill(OpeningAngle);
        DP_Pdg_one_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_one_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_one_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_one_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_one_Outside->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_one_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_one_Both->Fill(params.fRapidity, params.fPt);
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_one_Outside->Fill(params.fPt);
          Ph_fromPions_Pt_reco_one_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_one_Outside->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_one_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_one_Outside->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_one_Both->Fill(params.fPt);
        }
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Outside.push_back(frefmomenta);
        Gammas_stsIndex_two_Outside.push_back(frefId);
        Gammas_MC_two_Outside.push_back(fMCtracks);
        DP_InvMassReco_two_Outside->Fill(InvmassReco);
        DP_OpeningAngleReco_two_Outside->Fill(OpeningAngle);
        DP_Pdg_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_two_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_two_Outside->Fill(params.fMomentumMag);
        DP_Pt_reco_two_Outside->Fill(params.fPt);
        DP_InvMassReco_two_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_two_Both->Fill(OpeningAngle);
        DP_Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_two_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_two_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_two_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_two_Both->Fill(params.fRapidity, params.fPt);
          fromFireball_P_two_Outside->Fill(part1.Mag());
          fromFireball_P_two_Outside->Fill(part2.Mag());
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromPions_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (fromXi == 1) { Ph_fromXi_Pt_reco_two_Outside->Fill(params.fPt); }
        if (fromOther == 1) { Ph_fromOther_Pt_reco_two_Outside->Fill(params.fPt); }
        if (fromCombinatorial == 1) {

          // particle 1
          if (part1MC->GetMotherId() != -1) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CombinatorialMotherPdg_two_Outside->Fill(TMath::Abs(mama1->GetPdgCode()));
            if (mama1->GetMotherId() != -1) {
              CbmMCTrack* grmama1 = (CbmMCTrack*) fMcTracks->At(mama1->GetMotherId());
              CombinatorialGrMotherPdg_two_Outside->Fill(TMath::Abs(grmama1->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_two_Outside->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_two_Outside->Fill(-1);
          }

          // particle 2
          if (part2MC->GetMotherId() != -1) {
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            CombinatorialMotherPdg_two_Outside->Fill(TMath::Abs(mama2->GetPdgCode()));
            if (mama2->GetMotherId() != -1) {
              CbmMCTrack* grmama2 = (CbmMCTrack*) fMcTracks->At(mama2->GetMotherId());
              CombinatorialGrMotherPdg_two_Outside->Fill(TMath::Abs(grmama2->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_two_Outside->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_two_Outside->Fill(-1);
          }

          // Check sources of BG
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            Electrons_two_Outside->Fill(params.fPt);
          }
          else if (TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 211) {
            Pions_two_Outside->Fill(params.fPt);
          }
          else if ((TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 11)
                   || (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 211)) {
            PionElectron_two_Outside->Fill(params.fPt);
          }
          else {
            elsePionOrElectron_two_Outside->Fill(params.fPt);
          }

          // check special cases of BG, when two particles are electrons, but wrongly combined (with some correlation)
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            int mama1PDG      = mama1->GetPdgCode();
            int mama2PDG      = mama2->GetPdgCode();
            if (
              (mama1PDG == 22 && mama2PDG == 111 && mama1->GetMotherId() == part2MC->GetMotherId())
              || (mama1PDG == 111 && mama2PDG == 22
                  && mama2->GetMotherId()
                       == part1MC
                            ->GetMotherId())) {  // pi -> e+e-g    ===> combined wrongly (e+- from dalitz and e-+ from conversion)
              DalitzAndConversion_Pt_two_Outside->Fill(params.fPt);
            }
            if (
              mama1PDG == 22 && mama2PDG == 22 && mama1->GetMotherId() == mama2->GetMotherId()
              && part1MC->GetMotherId()
                   != part2MC
                        ->GetMotherId()) {  // pi -> gg    ===> combined wrongly (e+- from first and e-+ from second conversion)
              DoubleConversion_Pt_two_Outside->Fill(params.fPt);
            }
          }

          Ph_fromCombinatorial_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromCombinatorial_Pt_reco_two_Both->Fill(params.fPt);
          fromCombinatorial_PDG_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          fromCombinatorial_PDG_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          if (TMath::Abs(part1MC->GetPdgCode() == 11)) { fromCombinatorial_electron_P_two_Outside->Fill(part1.Mag()); }
          else {
            fromCombinatorial_NOTelectron_P_two_Outside->Fill(part1.Mag());
          }
          if (TMath::Abs(part2MC->GetPdgCode() == 11)) { fromCombinatorial_electron_P_two_Outside->Fill(part2.Mag()); }
          else {
            fromCombinatorial_NOTelectron_P_two_Outside->Fill(part2.Mag());
          }
        }
        if (fromConversion == 1) {
          Ph_fromConversion_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_fromConversion_Pt_reco_two_Both->Fill(params.fPt);
        }
        if (twoFromTarget == 1) {
          Ph_twoFromTarget_Pt_reco_two_Outside->Fill(params.fPt);
          Ph_twoFromTarget_Pt_reco_two_Both->Fill(params.fPt);
          twoFromTarget_PDG_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_PDG_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_P_two_Outside->Fill(TMath::Abs(part1.Mag()));
          twoFromTarget_P_two_Outside->Fill(TMath::Abs(part2.Mag()));
        }
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Outside.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Outside.push_back(frefId);
        Gammas_MC_onetwo_Outside.push_back(fMCtracks);
        DP_InvMassReco_onetwo_Outside->Fill(InvmassReco);
        DP_OpeningAngleReco_onetwo_Outside->Fill(OpeningAngle);
        DP_Pdg_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_onetwo_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_onetwo_Outside->Fill(params.fMomentumMag);
        DP_Pt_reco_onetwo_Outside->Fill(params.fPt);
        DP_InvMassReco_onetwo_Both->Fill(InvmassReco);
        DP_OpeningAngleReco_onetwo_Both->Fill(OpeningAngle);
        DP_Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        DP_Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        DP_P_reco_onetwo_Both->Fill(params.fMomentumMag);
        DP_Pt_reco_onetwo_Both->Fill(params.fPt);
        if (fromFireball == 1) {
          Ph_fromTarget_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromTarget_Pt_reco_onetwo_Both->Fill(params.fPt);
          Ph_pt_vs_rap_est_onetwo_Both->Fill(params.fRapidity, params.fPt);
          fromFireball_P_onetwo_Outside->Fill(part1.Mag());
          fromFireball_P_onetwo_Outside->Fill(part2.Mag());
        }
        if (fromPions == 1) {
          Ph_fromPions_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromPions_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromEta == 1) {
          Ph_fromEtas_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromEtas_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromDalitz == 1) {
          Ph_fromDalitz_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromDalitz_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (fromXi == 1) { Ph_fromXi_Pt_reco_onetwo_Outside->Fill(params.fPt); }
        if (fromOther == 1) { Ph_fromOther_Pt_reco_onetwo_Outside->Fill(params.fPt); }
        if (fromCombinatorial == 1) {

          // particle 1
          if (part1MC->GetMotherId() != -1) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CombinatorialMotherPdg_onetwo_Outside->Fill(TMath::Abs(mama1->GetPdgCode()));
            if (mama1->GetMotherId() != -1) {
              CbmMCTrack* grmama1 = (CbmMCTrack*) fMcTracks->At(mama1->GetMotherId());
              CombinatorialGrMotherPdg_onetwo_Outside->Fill(TMath::Abs(grmama1->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_onetwo_Outside->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_onetwo_Outside->Fill(-1);
          }

          // particle 2
          if (part2MC->GetMotherId() != -1) {
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            CombinatorialMotherPdg_onetwo_Outside->Fill(TMath::Abs(mama2->GetPdgCode()));
            if (mama2->GetMotherId() != -1) {
              CbmMCTrack* grmama2 = (CbmMCTrack*) fMcTracks->At(mama2->GetMotherId());
              CombinatorialGrMotherPdg_onetwo_Outside->Fill(TMath::Abs(grmama2->GetPdgCode()));
            }
            else {
              CombinatorialGrMotherPdg_onetwo_Outside->Fill(-1);
            }
          }
          else {
            CombinatorialMotherPdg_onetwo_Outside->Fill(-1);
          }

          // Check sources of BG
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            Electrons_onetwo_Outside->Fill(params.fPt);
          }
          else if (TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 211) {
            Pions_onetwo_Outside->Fill(params.fPt);
          }
          else if ((TMath::Abs(part1MC->GetPdgCode()) == 211 && TMath::Abs(part2MC->GetPdgCode()) == 11)
                   || (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 211)) {
            PionElectron_onetwo_Outside->Fill(params.fPt);
          }
          else {
            elsePionOrElectron_onetwo_Outside->Fill(params.fPt);
          }

          // check special cases of BG, when two particles are electrons, but wrongly combined (with some correlation)
          if (TMath::Abs(part1MC->GetPdgCode()) == 11 && TMath::Abs(part2MC->GetPdgCode()) == 11) {
            CbmMCTrack* mama1 = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
            CbmMCTrack* mama2 = (CbmMCTrack*) fMcTracks->At(part2MC->GetMotherId());
            int mama1PDG      = mama1->GetPdgCode();
            int mama2PDG      = mama2->GetPdgCode();
            if (
              (mama1PDG == 22 && mama2PDG == 111 && mama1->GetMotherId() == part2MC->GetMotherId())
              || (mama1PDG == 111 && mama2PDG == 22
                  && mama2->GetMotherId()
                       == part1MC
                            ->GetMotherId())) {  // pi -> e+e-g    ===> combined wrongly (e+- from dalitz and e-+ from conversion)
              DalitzAndConversion_Pt_onetwo_Outside->Fill(params.fPt);
            }
            if (
              mama1PDG == 22 && mama2PDG == 22 && mama1->GetMotherId() == mama2->GetMotherId()
              && part1MC->GetMotherId()
                   != part2MC
                        ->GetMotherId()) {  // pi -> gg    ===> combined wrongly (e+- from first and e-+ from second conversion)
              DoubleConversion_Pt_onetwo_Outside->Fill(params.fPt);
            }
          }

          Ph_fromCombinatorial_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromCombinatorial_Pt_reco_onetwo_Both->Fill(params.fPt);
          fromCombinatorial_PDG_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          fromCombinatorial_PDG_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          if (TMath::Abs(part1MC->GetPdgCode() == 11)) {
            fromCombinatorial_electron_P_onetwo_Outside->Fill(part1.Mag());
          }
          else {
            fromCombinatorial_NOTelectron_P_onetwo_Outside->Fill(part1.Mag());
          }
          if (TMath::Abs(part2MC->GetPdgCode() == 11)) {
            fromCombinatorial_electron_P_onetwo_Outside->Fill(part2.Mag());
          }
          else {
            fromCombinatorial_NOTelectron_P_onetwo_Outside->Fill(part2.Mag());
          }
        }
        if (fromConversion == 1) {
          Ph_fromConversion_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_fromConversion_Pt_reco_onetwo_Both->Fill(params.fPt);
        }
        if (twoFromTarget == 1) {
          Ph_twoFromTarget_Pt_reco_onetwo_Outside->Fill(params.fPt);
          Ph_twoFromTarget_Pt_reco_onetwo_Both->Fill(params.fPt);
          twoFromTarget_PDG_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_PDG_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
          twoFromTarget_P_onetwo_Outside->Fill(TMath::Abs(part1.Mag()));
          twoFromTarget_P_onetwo_Outside->Fill(TMath::Abs(part2.Mag()));
        }
      }
    }
  }
}

void CbmKresConversionPhotons::FindGammasBoth()
{
  Gammas_all_Both.insert(Gammas_all_Both.end(), Gammas_all_Outside.begin(), Gammas_all_Outside.end());
  Gammas_all_Both.insert(Gammas_all_Both.end(), Gammas_all_Target.begin(), Gammas_all_Target.end());
  Gammas_zero_Both.insert(Gammas_zero_Both.end(), Gammas_zero_Outside.begin(), Gammas_zero_Outside.end());
  Gammas_zero_Both.insert(Gammas_zero_Both.end(), Gammas_zero_Target.begin(), Gammas_zero_Target.end());
  Gammas_one_Both.insert(Gammas_one_Both.end(), Gammas_one_Outside.begin(), Gammas_one_Outside.end());
  Gammas_one_Both.insert(Gammas_one_Both.end(), Gammas_one_Target.begin(), Gammas_one_Target.end());
  Gammas_two_Both.insert(Gammas_two_Both.end(), Gammas_two_Outside.begin(), Gammas_two_Outside.end());
  Gammas_two_Both.insert(Gammas_two_Both.end(), Gammas_two_Target.begin(), Gammas_two_Target.end());
  Gammas_onetwo_Both.insert(Gammas_onetwo_Both.end(), Gammas_onetwo_Outside.begin(), Gammas_onetwo_Outside.end());
  Gammas_onetwo_Both.insert(Gammas_onetwo_Both.end(), Gammas_onetwo_Target.begin(), Gammas_onetwo_Target.end());

  Gammas_stsIndex_all_Both.insert(Gammas_stsIndex_all_Both.end(), Gammas_stsIndex_all_Outside.begin(),
                                  Gammas_stsIndex_all_Outside.end());
  Gammas_stsIndex_all_Both.insert(Gammas_stsIndex_all_Both.end(), Gammas_stsIndex_all_Target.begin(),
                                  Gammas_stsIndex_all_Target.end());
  Gammas_stsIndex_zero_Both.insert(Gammas_stsIndex_zero_Both.end(), Gammas_stsIndex_zero_Outside.begin(),
                                   Gammas_stsIndex_zero_Outside.end());
  Gammas_stsIndex_zero_Both.insert(Gammas_stsIndex_zero_Both.end(), Gammas_stsIndex_zero_Target.begin(),
                                   Gammas_stsIndex_zero_Target.end());
  Gammas_stsIndex_one_Both.insert(Gammas_stsIndex_one_Both.end(), Gammas_stsIndex_one_Outside.begin(),
                                  Gammas_stsIndex_one_Outside.end());
  Gammas_stsIndex_one_Both.insert(Gammas_stsIndex_one_Both.end(), Gammas_stsIndex_one_Target.begin(),
                                  Gammas_stsIndex_one_Target.end());
  Gammas_stsIndex_two_Both.insert(Gammas_stsIndex_two_Both.end(), Gammas_stsIndex_two_Outside.begin(),
                                  Gammas_stsIndex_two_Outside.end());
  Gammas_stsIndex_two_Both.insert(Gammas_stsIndex_two_Both.end(), Gammas_stsIndex_two_Target.begin(),
                                  Gammas_stsIndex_two_Target.end());
  Gammas_stsIndex_onetwo_Both.insert(Gammas_stsIndex_onetwo_Both.end(), Gammas_stsIndex_onetwo_Outside.begin(),
                                     Gammas_stsIndex_onetwo_Outside.end());
  Gammas_stsIndex_onetwo_Both.insert(Gammas_stsIndex_onetwo_Both.end(), Gammas_stsIndex_onetwo_Target.begin(),
                                     Gammas_stsIndex_onetwo_Target.end());

  Gammas_MC_all_Both.insert(Gammas_MC_all_Both.end(), Gammas_MC_all_Outside.begin(), Gammas_MC_all_Outside.end());
  Gammas_MC_all_Both.insert(Gammas_MC_all_Both.end(), Gammas_MC_all_Target.begin(), Gammas_MC_all_Target.end());
  Gammas_MC_zero_Both.insert(Gammas_MC_zero_Both.end(), Gammas_MC_zero_Outside.begin(), Gammas_MC_zero_Outside.end());
  Gammas_MC_zero_Both.insert(Gammas_MC_zero_Both.end(), Gammas_MC_zero_Target.begin(), Gammas_MC_zero_Target.end());
  Gammas_MC_one_Both.insert(Gammas_MC_one_Both.end(), Gammas_MC_one_Outside.begin(), Gammas_MC_one_Outside.end());
  Gammas_MC_one_Both.insert(Gammas_MC_one_Both.end(), Gammas_MC_one_Target.begin(), Gammas_MC_one_Target.end());
  Gammas_MC_two_Both.insert(Gammas_MC_two_Both.end(), Gammas_MC_two_Outside.begin(), Gammas_MC_two_Outside.end());
  Gammas_MC_two_Both.insert(Gammas_MC_two_Both.end(), Gammas_MC_two_Target.begin(), Gammas_MC_two_Target.end());
  Gammas_MC_onetwo_Both.insert(Gammas_MC_onetwo_Both.end(), Gammas_MC_onetwo_Outside.begin(),
                               Gammas_MC_onetwo_Outside.end());
  Gammas_MC_onetwo_Both.insert(Gammas_MC_onetwo_Both.end(), Gammas_MC_onetwo_Target.begin(),
                               Gammas_MC_onetwo_Target.end());
}


int CbmKresConversionPhotons::FindInRich(int richInd, int stsMcTrackId)
{
  int RingsInRich = 0;
  if (richInd > -1) {
    CbmTrackMatchNew* richMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    if (richMatch != nullptr && richMatch->GetNofLinks() > 0) {
      int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
      if (richMcTrackId > 0) {
        if (stsMcTrackId == richMcTrackId) {  // check that global track was matched correctly for STS and RICH together
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


int CbmKresConversionPhotons::CheckIfElectron(CbmRichRing* ring, double momentum)
{
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
    if (ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6 && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6
        && momentum > 0.2 && momentum < 4.)
      identified++;
    //if (ring->GetDistance() < 2 && ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6  && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6 && momentum > 0.2 && momentum < 4.) identified ++;
  }

  return identified;
}

std::vector<TVector3> CbmKresConversionPhotons::SaveAllHits(CbmStsTrack* track)
{
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

double CbmKresConversionPhotons::CalculatePlaneAngle_last_fromHits(std::vector<TVector3> track_1,
                                                                   std::vector<TVector3> track_2)
{
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
  if (TMath::Abs(last1.Z() - last2.Z()) > 2
      && last1.Z() > last2.Z()) {  // if last hits are in different stations --> try to find the latest common station
    for (int i = hits_1 - 2; i > -1; i--) {  // start from second last station
      if (TMath::Abs(last1.Z() - last2.Z()) < 2) continue;
      last1  = track_1[i];
      Xpart1 = last1.X();
      Ypart1 = last1.Y();
      Zpart1 = last1.Z();
    }
  }

  if (TMath::Abs(last1.Z() - last2.Z()) > 2
      && last1.Z() < last2.Z()) {  // if last hits are in different stations --> try to find the latest common station
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
    FinalAngle = TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}


double CbmKresConversionPhotons::CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
{
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
  }
  else {
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
  }
  else {
    Int_t mvdHitIndex2 = Sts_2->GetMvdHitIndex(hits2mvd - 1);
    CbmMvdHit* mvdHit2 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex2);
    Xpart2             = mvdHit2->GetX();
    Ypart2             = mvdHit2->GetY();
    Zpart2             = mvdHit2->GetZ();
  }

  // check difference in 2 cm, because of two slices of every STS and Mvd stations
  if (TMath::Abs(Zpart1 - Zpart2) > 2
      && Zpart1 > Zpart2) {  // if last hits are in different stations --> try to find the latest common station
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

  if (TMath::Abs(Zpart1 - Zpart2) > 2
      && Zpart1 < Zpart2) {  // if last hits are in different stations --> try to find the latest common station
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
    FinalAngle = TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}

double CbmKresConversionPhotons::CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
{
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
  }
  else {
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
  }
  else {
    Int_t stsHitIndex2 = Sts_2->GetStsHitIndex(0);
    CbmStsHit* stsHit2 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex2);
    Xpart2             = stsHit2->GetX();
    Ypart2             = stsHit2->GetY();
    Zpart2             = stsHit2->GetZ();
  }

  // check difference in 2 cm, because of two slices of every STS and Mvd stations
  if (TMath::Abs(Zpart1 - Zpart2) > 2
      && Zpart1 < Zpart2) {  // if first hits are in different stations --> try to find the earliest common station
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

  if (TMath::Abs(Zpart1 - Zpart2) > 2
      && Zpart1 > Zpart2) {  // if first hits are in different stations --> try to find the earliest common station
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
    FinalAngle = TMath::ATan2(Ypart1 - Ypart2, Xpart1 - Xpart2) * (180 / TMath::Pi());
  }

  return TMath::Abs(TMath::Abs(FinalAngle) - 180);
}

void CbmKresConversionPhotons::DP_likesign_Mixing_Target(double AngleCut, double InvMassCut)
// mix particles with the same charge TARGET
{
  int nof_minus = DP_LK_EMT_momenta_minus_Target.size();
  for (int a = 0; a < nof_minus - 1; a++) {
    for (int b = a + 1; b < nof_minus; b++) {
      if (DP_LK_EMT_STS_minus_index_Target[a] == DP_LK_EMT_STS_minus_index_Target[b]) continue;
      TVector3 e1       = DP_LK_EMT_momenta_minus_Target[a];
      TVector3 e2       = DP_LK_EMT_momenta_minus_Target[b];
      CbmStsTrack* sts1 = DP_LK_EMT_STS_minus_Target[a];
      CbmStsTrack* sts2 = DP_LK_EMT_STS_minus_Target[b];

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      int rings_amount = DP_LK_EMT_NofRings_minus_Target[a] + DP_LK_EMT_NofRings_minus_Target[b];

      DP_LK_EMT_neg_Pt_all_Target->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_neg_Pt_zero_Target->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_neg_Pt_one_Target->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_neg_Pt_two_Target->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) { DP_LK_EMT_neg_Pt_onetwo_Target->Fill(params.fPt); }
      DP_LK_EMT_Pt_all_Target->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_Pt_zero_Target->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_Pt_one_Target->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_Pt_two_Target->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_Pt_onetwo_Target->Fill(params.fPt);
    }
  }


  int nof_plus = DP_LK_EMT_momenta_plus_Target.size();
  for (int a = 0; a < nof_plus - 1; a++) {
    for (int b = a + 1; b < nof_plus; b++) {
      if (DP_LK_EMT_STS_plus_index_Target[a] == DP_LK_EMT_STS_plus_index_Target[b]) continue;
      TVector3 e1       = DP_LK_EMT_momenta_plus_Target[a];
      TVector3 e2       = DP_LK_EMT_momenta_plus_Target[b];
      CbmStsTrack* sts1 = DP_LK_EMT_STS_plus_Target[a];
      CbmStsTrack* sts2 = DP_LK_EMT_STS_plus_Target[b];

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      int rings_amount = DP_LK_EMT_NofRings_plus_Target[a] + DP_LK_EMT_NofRings_plus_Target[b];

      DP_LK_EMT_pos_Pt_all_Target->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_pos_Pt_zero_Target->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_pos_Pt_one_Target->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_pos_Pt_two_Target->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) { DP_LK_EMT_pos_Pt_onetwo_Target->Fill(params.fPt); }
      DP_LK_EMT_Pt_all_Target->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_Pt_zero_Target->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_Pt_one_Target->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_Pt_two_Target->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_Pt_onetwo_Target->Fill(params.fPt);
    }
  }
}

void CbmKresConversionPhotons::DP_likesign_Mixing_Outside(double AngleCut, double InvMassCut)
// mix particles with the same charge OUTSIDE
{
  int nof_minus = DP_LK_EMT_NofRings_minus_Outside.size();
  for (int a = 0; a < nof_minus - 1; a++) {
    for (int b = a + 1; b < nof_minus; b++) {
      if (DP_LK_EMT_STS_minus_index_Outside[a] == DP_LK_EMT_STS_minus_index_Outside[b]) continue;
      CbmStsTrack* sts1 = DP_LK_EMT_STS_minus_Outside[a];
      CbmStsTrack* sts2 = DP_LK_EMT_STS_minus_Outside[b];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts1, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts2, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5) continue;  // kick weird intersections

      TVector3 e1 = CbmKresFunctions::FitToVertex(sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      TVector3 e2 = CbmKresFunctions::FitToVertex(sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      int rings_amount = DP_LK_EMT_NofRings_minus_Outside[a] + DP_LK_EMT_NofRings_minus_Outside[b];

      DP_LK_EMT_neg_Pt_all_Outside->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_neg_Pt_zero_Outside->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_neg_Pt_one_Outside->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_neg_Pt_two_Outside->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_neg_Pt_onetwo_Outside->Fill(params.fPt);
      DP_LK_EMT_Pt_all_Outside->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_Pt_zero_Outside->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_Pt_one_Outside->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_Pt_two_Outside->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_Pt_onetwo_Outside->Fill(params.fPt);
    }
  }


  int nof_plus = DP_LK_EMT_NofRings_plus_Outside.size();
  for (int a = 0; a < nof_plus - 1; a++) {
    for (int b = a + 1; b < nof_plus; b++) {
      if (DP_LK_EMT_STS_plus_index_Outside[a] == DP_LK_EMT_STS_plus_index_Outside[b]) continue;
      CbmStsTrack* sts1 = DP_LK_EMT_STS_plus_Outside[a];
      CbmStsTrack* sts2 = DP_LK_EMT_STS_plus_Outside[b];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts1, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts2, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5) continue;  // kick weird intersections

      TVector3 e1 = CbmKresFunctions::FitToVertex(sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      TVector3 e2 = CbmKresFunctions::FitToVertex(sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);

      int rings_amount = DP_LK_EMT_NofRings_plus_Outside[a] + DP_LK_EMT_NofRings_plus_Outside[b];

      DP_LK_EMT_pos_Pt_all_Outside->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_pos_Pt_zero_Outside->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_pos_Pt_one_Outside->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_pos_Pt_two_Outside->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_pos_Pt_onetwo_Outside->Fill(params.fPt);
      DP_LK_EMT_Pt_all_Outside->Fill(params.fPt);
      if (rings_amount == 0) DP_LK_EMT_Pt_zero_Outside->Fill(params.fPt);
      if (rings_amount == 1) DP_LK_EMT_Pt_one_Outside->Fill(params.fPt);
      if (rings_amount == 2) DP_LK_EMT_Pt_two_Outside->Fill(params.fPt);
      if ((rings_amount == 1 || rings_amount == 2)) DP_LK_EMT_Pt_onetwo_Outside->Fill(params.fPt);
    }
  }
}


void CbmKresConversionPhotons::DP_Mixing_Target(double AngleCut, double InvMassCut)
// TARGET
{
  int nof_Target = DP_EMT_Event_minus_Target.size();
  cout << "Mixing for direct photons in Target - nof entries " << nof_Target << endl;

  for (size_t a = 0; a < DP_EMT_Event_minus_Target.size(); a++) {
    for (size_t b = 0; b < DP_EMT_Event_plus_Target.size(); b++) {
      if (DP_EMT_Event_minus_Target[a] == DP_EMT_Event_plus_Target[b])
        continue;  // to make sure that two particles are from two different events
      TVector3 e1                 = DP_EMT_momenta_minus_Target[a];
      TVector3 e2                 = DP_EMT_momenta_plus_Target[b];
      std::vector<TVector3> hits1 = DP_EMT_Hits_minus_Target[a];
      std::vector<TVector3> hits2 = DP_EMT_Hits_plus_Target[b];

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last_fromHits(hits1, hits2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);
      int rings_amount              = DP_EMT_NofRings_minus_Target[a] + DP_EMT_NofRings_plus_Target[b];

      DP_EMT_Pt_all_Target->Fill(params.fPt);
      DP_EMT_Pt_all_Both->Fill(params.fPt);
      if (rings_amount == 0) {
        DP_EMT_Pt_zero_Target->Fill(params.fPt);
        DP_EMT_Pt_zero_Both->Fill(params.fPt);
      }
      if (rings_amount == 1) {
        DP_EMT_Pt_one_Target->Fill(params.fPt);
        DP_EMT_Pt_one_Both->Fill(params.fPt);
      }
      if (rings_amount == 2) {
        DP_EMT_Pt_two_Target->Fill(params.fPt);
        DP_EMT_Pt_two_Both->Fill(params.fPt);
      }
      if ((rings_amount == 1 || rings_amount == 2)) {
        DP_EMT_Pt_onetwo_Target->Fill(params.fPt);
        DP_EMT_Pt_onetwo_Both->Fill(params.fPt);
      }
    }
  }
}


void CbmKresConversionPhotons::DP_Mixing_Outside(double AngleCut, double InvMassCut)
// OUTSIDE
{
  int nof_Outside = DP_EMT_Event_minus_Outside.size();
  cout << "Mixing for direct photons in Outside - nof entries " << nof_Outside << endl;

  for (size_t a = 0; a < DP_EMT_Event_minus_Outside.size(); a++) {
    for (size_t b = 0; b < DP_EMT_Event_plus_Outside.size(); b++) {
      if (DP_EMT_Event_minus_Outside[a] == DP_EMT_Event_plus_Outside[b])
        continue;  // to make sure that two particles are from two different events

      KFParticle electron = DP_EMT_KFTrack_minus_Outside[a];
      KFParticle positron = DP_EMT_KFTrack_plus_Outside[b];

      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5) continue;  // kick weird intersections

      // TVector3 e1 = CbmKresFunctions::FitToVertex(sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      // TVector3 e2 = CbmKresFunctions::FitToVertex(sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      TVector3 e1(electron.KFParticleBase::Px(), electron.KFParticleBase::Py(), electron.KFParticleBase::Pz());
      TVector3 e2(positron.KFParticleBase::Px(), positron.KFParticleBase::Py(), positron.KFParticleBase::Pz());

      std::vector<TVector3> hits1 = DP_EMT_Hits_minus_Outside[a];
      std::vector<TVector3> hits2 = DP_EMT_Hits_plus_Outside[b];

      double InvmassReco     = CbmKresFunctions::Invmass_2particles_RECO(e1, e2);
      double OpeningAngle    = CbmKresFunctions::CalculateOpeningAngle_Reco(e1, e2);
      double PlaneAngle_last = CalculatePlaneAngle_last_fromHits(hits1, hits2);

      //			double AnnValue = fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), e1, e2);
      fAnnPhotonsSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), e1, e2);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9 || AnnValue > 1.1) continue;

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParamsReco(e1, e2);
      int rings_amount              = DP_EMT_NofRings_minus_Outside[a] + DP_EMT_NofRings_plus_Outside[b];

      DP_EMT_Pt_all_Outside->Fill(params.fPt);
      DP_EMT_Pt_all_Both->Fill(params.fPt);
      if (rings_amount == 0) {
        DP_EMT_Pt_zero_Outside->Fill(params.fPt);
        DP_EMT_Pt_zero_Both->Fill(params.fPt);
      }
      if (rings_amount == 1) {
        DP_EMT_Pt_one_Outside->Fill(params.fPt);
        DP_EMT_Pt_one_Both->Fill(params.fPt);
      }
      if (rings_amount == 2) {
        DP_EMT_Pt_two_Outside->Fill(params.fPt);
        DP_EMT_Pt_two_Both->Fill(params.fPt);
      }
      if ((rings_amount == 1 || rings_amount == 2)) {
        DP_EMT_Pt_onetwo_Outside->Fill(params.fPt);
        DP_EMT_Pt_onetwo_Both->Fill(params.fPt);
      }
    }
  }
}


void CbmKresConversionPhotons::Finish()
{
  gDirectory->mkdir("direct photons");
  gDirectory->cd("direct photons");


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
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_dp_zero_Target.size(); i++) {
    fHistoList_dp_zero_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_dp_one_Target.size(); i++) {
    fHistoList_dp_one_Target[i]->Write();
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
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_dp_zero_Outside.size(); i++) {
    fHistoList_dp_zero_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_dp_one_Outside.size(); i++) {
    fHistoList_dp_one_Outside[i]->Write();
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
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_dp_zero_Both.size(); i++) {
    fHistoList_dp_zero_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_dp_one_Both.size(); i++) {
    fHistoList_dp_one_Both[i]->Write();
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


void CbmKresConversionPhotons::InitHistograms()
{
  // Target
  Mother_PDG_Target = new TH1D("Mother_PDG_Target", "Mother_PDG_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_Target.push_back(Mother_PDG_Target);
  GrandMother_PDG_Target = new TH1D("GrandMother_PDG_Target", "GrandMother_PDG_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_Target.push_back(GrandMother_PDG_Target);

  // Outside
  Mother_PDG_Outside = new TH1D("Mother_PDG_Outside", "Mother_PDG_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_Outside.push_back(Mother_PDG_Outside);
  GrandMother_PDG_Outside = new TH1D("GrandMother_PDG_Outside", "GrandMother_PDG_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_Outside.push_back(GrandMother_PDG_Outside);

  // Both
  Pdg_vs_Distance_for_dp =
    new TH2D("Pdg_vs_Distance_for_dp", "Pdg_vs_Distance_for_dp; pdg; distance in cm", 2500, 0, 2499, 500, 0, 50);
  fHistoList_dp_Both.push_back(Pdg_vs_Distance_for_dp);
  P_vs_Distance_for_dp = new TH2D("P_vs_Distance_for_dp",
                                  "Distance between projected track and center of the ring (for e+ "
                                  "and e-); P in GeV/c^{2}; distance in cm",
                                  300, 0, 3, 300, 0, 15);
  fHistoList_dp_Both.push_back(P_vs_Distance_for_dp);
  DP_AnnTruePairs = new TH1D("DP_AnnTruePairs", "DP_AnnTruePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(DP_AnnTruePairs);
  DP_AnnFalsePairs = new TH1D("DP_AnnFalsePairs", "DP_AnnFalsePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(DP_AnnFalsePairs);
  DP_AnnTruePairs_AfterCuts =
    new TH1D("DP_AnnTruePairs_AfterCuts", "DP_AnnTruePairs_AfterCuts; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(DP_AnnTruePairs_AfterCuts);
  DP_AnnFalsePairs_AfterCuts =
    new TH1D("DP_AnnFalsePairs_AfterCuts", "DP_AnnFalsePairs_AfterCuts; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_dp_Both.push_back(DP_AnnFalsePairs_AfterCuts);


  ///////   histograms to check Cuts
  ///////   Both
  DP_candidates_InvMass_vs_OA_Both = new TH2D("DP_candidates_InvMass_vs_OA_Both",
                                              "DP_candidates_InvMass_vs_OA_Both; invariant mass in GeV/c^{2}; "
                                              "opening angle in degree",
                                              500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Both.push_back(DP_candidates_InvMass_vs_OA_Both);
  DP_InvMass_vs_OA_Both = new TH2D("DP_InvMass_vs_OA_Both",
                                   "DP_InvMass_vs_OA_Both; invariant mass in "
                                   "GeV/c^{2}; opening angle in degree",
                                   500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Both.push_back(DP_InvMass_vs_OA_Both);
  DP_candidates_InvMass_Both = new TH1D("DP_candidates_InvMass_Both",
                                        "DP_candidates_InvMass_Both; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Both.push_back(DP_candidates_InvMass_Both);
  DP_InvMass_Both = new TH1D("DP_InvMass_Both", "DP_InvMass_Both; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Both.push_back(DP_InvMass_Both);
  DP_candidates_OA_Both =
    new TH1D("DP_candidates_OA_Both", "DP_candidates_OA_Both; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Both.push_back(DP_candidates_OA_Both);
  DP_OA_Both = new TH1D("DP_OA_Both", "DP_OA_Both; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Both.push_back(DP_OA_Both);
  DP_candidates_PlaneAngles_last_Both =
    new TH1D("DP_candidates_PlaneAngles_last_Both", "DP_candidates_PlaneAngles_last_Both; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_dp_cuts_Both.push_back(DP_candidates_PlaneAngles_last_Both);
  DP_PlaneAngles_last_Both =
    new TH1D("DP_PlaneAngles_last_Both", "DP_PlaneAngles_last_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Both.push_back(DP_PlaneAngles_last_Both);
  DP_candidates_PlaneAngles_first_Both =
    new TH1D("DP_candidates_PlaneAngles_first_Both", "DP_candidates_PlaneAngles_first_Both; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_dp_cuts_Both.push_back(DP_candidates_PlaneAngles_first_Both);
  DP_PlaneAngles_first_Both =
    new TH1D("DP_PlaneAngles_first_Both", "DP_PlaneAngles_first_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Both.push_back(DP_PlaneAngles_first_Both);

  ///////   Target
  DP_candidates_InvMass_vs_OA_Target = new TH2D("DP_candidates_InvMass_vs_OA_Target",
                                                "DP_candidates_InvMass_vs_OA_Target; invariant mass in GeV/c^{2}; "
                                                "opening angle in degree",
                                                500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Target.push_back(DP_candidates_InvMass_vs_OA_Target);
  DP_InvMass_vs_OA_Target = new TH2D("DP_InvMass_vs_OA_Target",
                                     "DP_InvMass_vs_OA_Target; invariant mass "
                                     "in GeV/c^{2}; opening angle in degree",
                                     500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Target.push_back(DP_InvMass_vs_OA_Target);
  DP_candidates_InvMass_Target = new TH1D(
    "DP_candidates_InvMass_Target", "DP_candidates_InvMass_Target; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Target.push_back(DP_candidates_InvMass_Target);
  DP_InvMass_Target =
    new TH1D("DP_InvMass_Target", "DP_InvMass_Target; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Target.push_back(DP_InvMass_Target);
  DP_candidates_OA_Target =
    new TH1D("DP_candidates_OA_Target", "DP_candidates_OA_Target; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Target.push_back(DP_candidates_OA_Target);
  DP_OA_Target = new TH1D("DP_OA_Target", "DP_OA_Target; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Target.push_back(DP_OA_Target);
  DP_candidates_PlaneAngles_last_Target =
    new TH1D("DP_candidates_PlaneAngles_last_Target", "DP_candidates_PlaneAngles_last_Target; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_dp_cuts_Target.push_back(DP_candidates_PlaneAngles_last_Target);
  DP_PlaneAngles_last_Target =
    new TH1D("DP_PlaneAngles_last_Target", "DP_PlaneAngles_last_Target; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Target.push_back(DP_PlaneAngles_last_Target);
  DP_candidates_PlaneAngles_first_Target =
    new TH1D("DP_candidates_PlaneAngles_first_Target",
             "DP_candidates_PlaneAngles_first_Target; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Target.push_back(DP_candidates_PlaneAngles_first_Target);
  DP_PlaneAngles_first_Target =
    new TH1D("DP_PlaneAngles_first_Target", "DP_PlaneAngles_first_Target; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Target.push_back(DP_PlaneAngles_first_Target);

  ///////   Outside
  DP_candidates_InvMass_vs_OA_Outside = new TH2D("DP_candidates_InvMass_vs_OA_Outside",
                                                 "DP_candidates_InvMass_vs_OA_Outside; invariant mass in "
                                                 "GeV/c^{2}; opening angle in degree",
                                                 500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Outside.push_back(DP_candidates_InvMass_vs_OA_Outside);
  DP_InvMass_vs_OA_Outside = new TH2D("DP_InvMass_vs_OA_Outside",
                                      "DP_InvMass_vs_OA_Outside; invariant mass in GeV/c^{2}; opening "
                                      "angle in degree",
                                      500, 0, 0.5, 500, 0, 50);
  fHistoList_dp_cuts_Outside.push_back(DP_InvMass_vs_OA_Outside);
  DP_candidates_InvMass_Outside = new TH1D(
    "DP_candidates_InvMass_Outside", "DP_candidates_InvMass_Outside; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Outside.push_back(DP_candidates_InvMass_Outside);
  DP_InvMass_Outside =
    new TH1D("DP_InvMass_Outside", "DP_InvMass_Outside; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_dp_cuts_Outside.push_back(DP_InvMass_Outside);
  DP_candidates_OA_Outside =
    new TH1D("DP_candidates_OA_Outside", "DP_candidates_OA_Outside; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Outside.push_back(DP_candidates_OA_Outside);
  DP_OA_Outside = new TH1D("DP_OA_Outside", "DP_OA_Outside; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_dp_cuts_Outside.push_back(DP_OA_Outside);
  DP_candidates_PlaneAngles_last_Outside =
    new TH1D("DP_candidates_PlaneAngles_last_Outside",
             "DP_candidates_PlaneAngles_last_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Outside.push_back(DP_candidates_PlaneAngles_last_Outside);
  DP_PlaneAngles_last_Outside =
    new TH1D("DP_PlaneAngles_last_Outside", "DP_PlaneAngles_last_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Outside.push_back(DP_PlaneAngles_last_Outside);
  DP_candidates_PlaneAngles_first_Outside =
    new TH1D("DP_candidates_PlaneAngles_first_Outside",
             "DP_candidates_PlaneAngles_first_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Outside.push_back(DP_candidates_PlaneAngles_first_Outside);
  DP_PlaneAngles_first_Outside =
    new TH1D("DP_PlaneAngles_first_Outside", "DP_PlaneAngles_first_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_dp_cuts_Outside.push_back(DP_PlaneAngles_first_Outside);


  // Target => all
  DP_InvMassReco_all_Target = new TH1D("DP_InvMassReco_all_Target",
                                       "DP_InvMassReco_all_Target; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_all_Target.push_back(DP_InvMassReco_all_Target);
  DP_OpeningAngleReco_all_Target =
    new TH1D("DP_OpeningAngleReco_all_Target", "DP_OpeningAngleReco_all_Target; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_all_Target.push_back(DP_OpeningAngleReco_all_Target);
  DP_Pdg_all_Target = new TH1D("DP_Pdg_all_Target", "DP_Pdg_all_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Target.push_back(DP_Pdg_all_Target);
  DP_P_reco_all_Target = new TH1D("DP_P_reco_all_Target", "DP_P_reco_all_Target; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_all_Target.push_back(DP_P_reco_all_Target);
  DP_Pt_reco_all_Target = new TH1D("DP_Pt_reco_all_Target", "DP_Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(DP_Pt_reco_all_Target);
  Ph_fromTarget_Pt_reco_all_Target =
    new TH1D("Ph_fromTarget_Pt_reco_all_Target", "Ph_fromTarget_Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(Ph_fromTarget_Pt_reco_all_Target);
  Ph_fromPions_Pt_reco_all_Target =
    new TH1D("Ph_fromPions_Pt_reco_all_Target", "Ph_fromPions_Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(Ph_fromPions_Pt_reco_all_Target);
  Ph_fromEtas_Pt_reco_all_Target =
    new TH1D("Ph_fromEtas_Pt_reco_all_Target", "Ph_fromEtas_Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(Ph_fromEtas_Pt_reco_all_Target);
  Ph_fromDalitz_Pt_reco_all_Target =
    new TH1D("Ph_fromDalitz_Pt_reco_all_Target", "Ph_fromDalitz_Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(Ph_fromDalitz_Pt_reco_all_Target);

  // Target => zero
  DP_InvMassReco_zero_Target = new TH1D("DP_InvMassReco_zero_Target",
                                        "DP_InvMassReco_zero_Target; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_zero_Target.push_back(DP_InvMassReco_zero_Target);
  DP_OpeningAngleReco_zero_Target =
    new TH1D("DP_OpeningAngleReco_zero_Target", "DP_OpeningAngleReco_zero_Target; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_zero_Target.push_back(DP_OpeningAngleReco_zero_Target);
  DP_Pdg_zero_Target = new TH1D("DP_Pdg_zero_Target", "DP_Pdg_zero_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_zero_Target.push_back(DP_Pdg_zero_Target);
  DP_P_reco_zero_Target = new TH1D("DP_P_reco_zero_Target", "DP_P_reco_zero_Target; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_zero_Target.push_back(DP_P_reco_zero_Target);
  DP_Pt_reco_zero_Target = new TH1D("DP_Pt_reco_zero_Target", "DP_Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(DP_Pt_reco_zero_Target);
  Ph_fromTarget_Pt_reco_zero_Target =
    new TH1D("Ph_fromTarget_Pt_reco_zero_Target", "Ph_fromTarget_Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(Ph_fromTarget_Pt_reco_zero_Target);
  Ph_fromPions_Pt_reco_zero_Target =
    new TH1D("Ph_fromPions_Pt_reco_zero_Target", "Ph_fromPions_Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(Ph_fromPions_Pt_reco_zero_Target);
  Ph_fromEtas_Pt_reco_zero_Target =
    new TH1D("Ph_fromEtas_Pt_reco_zero_Target", "Ph_fromEtas_Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(Ph_fromEtas_Pt_reco_zero_Target);
  Ph_fromDalitz_Pt_reco_zero_Target =
    new TH1D("Ph_fromDalitz_Pt_reco_zero_Target", "Ph_fromDalitz_Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(Ph_fromDalitz_Pt_reco_zero_Target);

  // Target => one
  DP_InvMassReco_one_Target = new TH1D("DP_InvMassReco_one_Target",
                                       "DP_InvMassReco_one_Target; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_one_Target.push_back(DP_InvMassReco_one_Target);
  DP_OpeningAngleReco_one_Target =
    new TH1D("DP_OpeningAngleReco_one_Target", "DP_OpeningAngleReco_one_Target; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_one_Target.push_back(DP_OpeningAngleReco_one_Target);
  DP_Pdg_one_Target = new TH1D("DP_Pdg_one_Target", "DP_Pdg_one_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_one_Target.push_back(DP_Pdg_one_Target);
  DP_P_reco_one_Target = new TH1D("DP_P_reco_one_Target", "DP_P_reco_one_Target; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_one_Target.push_back(DP_P_reco_one_Target);
  DP_Pt_reco_one_Target = new TH1D("DP_Pt_reco_one_Target", "DP_Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(DP_Pt_reco_one_Target);
  Ph_fromTarget_Pt_reco_one_Target =
    new TH1D("Ph_fromTarget_Pt_reco_one_Target", "Ph_fromTarget_Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(Ph_fromTarget_Pt_reco_one_Target);
  Ph_fromPions_Pt_reco_one_Target =
    new TH1D("Ph_fromPions_Pt_reco_one_Target", "Ph_fromPions_Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(Ph_fromPions_Pt_reco_one_Target);
  Ph_fromEtas_Pt_reco_one_Target =
    new TH1D("Ph_fromEtas_Pt_reco_one_Target", "Ph_fromEtas_Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(Ph_fromEtas_Pt_reco_one_Target);
  Ph_fromDalitz_Pt_reco_one_Target =
    new TH1D("Ph_fromDalitz_Pt_reco_one_Target", "Ph_fromDalitz_Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(Ph_fromDalitz_Pt_reco_one_Target);

  // Target => two
  DP_InvMassReco_two_Target = new TH1D("DP_InvMassReco_two_Target",
                                       "DP_InvMassReco_two_Target; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_two_Target.push_back(DP_InvMassReco_two_Target);
  DP_OpeningAngleReco_two_Target =
    new TH1D("DP_OpeningAngleReco_two_Target", "DP_OpeningAngleReco_two_Target; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_two_Target.push_back(DP_OpeningAngleReco_two_Target);
  DP_Pdg_two_Target = new TH1D("DP_Pdg_two_Target", "DP_Pdg_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(DP_Pdg_two_Target);
  DP_P_reco_two_Target = new TH1D("DP_P_reco_two_Target", "DP_P_reco_two_Target; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_two_Target.push_back(DP_P_reco_two_Target);
  DP_Pt_reco_two_Target = new TH1D("DP_Pt_reco_two_Target", "DP_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DP_Pt_reco_two_Target);
  Ph_fromTarget_Pt_reco_two_Target =
    new TH1D("Ph_fromTarget_Pt_reco_two_Target", "Ph_fromTarget_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromTarget_Pt_reco_two_Target);
  Ph_fromPions_Pt_reco_two_Target =
    new TH1D("Ph_fromPions_Pt_reco_two_Target", "Ph_fromPions_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromPions_Pt_reco_two_Target);
  Ph_fromEtas_Pt_reco_two_Target =
    new TH1D("Ph_fromEtas_Pt_reco_two_Target", "Ph_fromEtas_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromEtas_Pt_reco_two_Target);
  Ph_fromXi_Pt_reco_two_Target =
    new TH1D("Ph_fromXi_Pt_reco_two_Target", "Ph_fromXi_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromXi_Pt_reco_two_Target);
  Ph_fromDalitz_Pt_reco_two_Target =
    new TH1D("Ph_fromDalitz_Pt_reco_two_Target", "Ph_fromDalitz_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromDalitz_Pt_reco_two_Target);
  Ph_fromOther_Pt_reco_two_Target =
    new TH1D("Ph_fromOther_Pt_reco_two_Target", "Ph_fromOther_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromOther_Pt_reco_two_Target);
  Ph_twoFromTarget_Pt_reco_two_Target = new TH1D("Ph_twoFromTarget_Pt_reco_two_Target",
                                                 "Ph_twoFromTarget_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_twoFromTarget_Pt_reco_two_Target);
  Ph_fromCombinatorial_Pt_reco_two_Target =
    new TH1D("Ph_fromCombinatorial_Pt_reco_two_Target", "Ph_fromCombinatorial_Pt_reco_two_Target; P_{t} in GeV/c^{2};#",
             30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromCombinatorial_Pt_reco_two_Target);
  Ph_fromConversion_Pt_reco_two_Target = new TH1D(
    "Ph_fromConversion_Pt_reco_two_Target", "Ph_fromConversion_Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Ph_fromConversion_Pt_reco_two_Target);

  twoFromTarget_PDG_two_Target =
    new TH1D("twoFromTarget_PDG_two_Target", "twoFromTarget_PDG_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(twoFromTarget_PDG_two_Target);
  fromCombinatorial_PDG_two_Target =
    new TH1D("fromCombinatorial_PDG_two_Target", "fromCombinatorial_PDG_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(fromCombinatorial_PDG_two_Target);
  CombinatorialMotherPdg_two_Target =
    new TH1D("CombinatorialMotherPdg_two_Target", "CombinatorialMotherPdg_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(CombinatorialMotherPdg_two_Target);
  CombinatorialGrMotherPdg_two_Target =
    new TH1D("CombinatorialGrMotherPdg_two_Target", "CombinatorialGrMotherPdg_two_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Target.push_back(CombinatorialGrMotherPdg_two_Target);
  Electrons_two_Target = new TH1D("Electrons_two_Target", "Electrons_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Electrons_two_Target);
  Pions_two_Target = new TH1D("Pions_two_Target", "Pions_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(Pions_two_Target);
  PionElectron_two_Target =
    new TH1D("PionElectron_two_Target", "PionElectron_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(PionElectron_two_Target);
  elsePionOrElectron_two_Target =
    new TH1D("elsePionOrElectron_two_Target", "elsePionOrElectron_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(elsePionOrElectron_two_Target);
  DalitzAndConversion_Pt_two_Target =
    new TH1D("DalitzAndConversion_Pt_two_Target", "DalitzAndConversion_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DalitzAndConversion_Pt_two_Target);
  DoubleConversion_Pt_two_Target =
    new TH1D("DoubleConversion_Pt_two_Target", "DoubleConversion_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DoubleConversion_Pt_two_Target);
  fromFireball_P_two_Target =
    new TH1D("fromFireball_P_two_Target", "fromFireball_P_two_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Target.push_back(fromFireball_P_two_Target);
  twoFromTarget_P_two_Target =
    new TH1D("twoFromTarget_P_two_Target", "twoFromTarget_P_two_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Target.push_back(twoFromTarget_P_two_Target);
  fromCombinatorial_electron_P_two_Target =
    new TH1D("fromCombinatorial_electron_P_two_Target", "fromCombinatorial_electron_P_two_Target; P_{t} in GeV/c^{2};#",
             120, 0, 6);
  fHistoList_dp_two_Target.push_back(fromCombinatorial_electron_P_two_Target);
  fromCombinatorial_NOTelectron_P_two_Target =
    new TH1D("fromCombinatorial_NOTelectron_P_two_Target",
             "fromCombinatorial_NOTelectron_P_two_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Target.push_back(fromCombinatorial_NOTelectron_P_two_Target);

  // Target => onetwo
  DP_InvMassReco_onetwo_Target = new TH1D(
    "DP_InvMassReco_onetwo_Target", "DP_InvMassReco_onetwo_Target; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_onetwo_Target.push_back(DP_InvMassReco_onetwo_Target);
  DP_OpeningAngleReco_onetwo_Target =
    new TH1D("DP_OpeningAngleReco_onetwo_Target", "DP_OpeningAngleReco_onetwo_Target; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_onetwo_Target.push_back(DP_OpeningAngleReco_onetwo_Target);
  DP_Pdg_onetwo_Target = new TH1D("DP_Pdg_onetwo_Target", "DP_Pdg_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(DP_Pdg_onetwo_Target);
  DP_P_reco_onetwo_Target =
    new TH1D("DP_P_reco_onetwo_Target", "DP_P_reco_onetwo_Target; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_onetwo_Target.push_back(DP_P_reco_onetwo_Target);
  DP_Pt_reco_onetwo_Target =
    new TH1D("DP_Pt_reco_onetwo_Target", "DP_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DP_Pt_reco_onetwo_Target);
  Ph_fromTarget_Pt_reco_onetwo_Target = new TH1D("Ph_fromTarget_Pt_reco_onetwo_Target",
                                                 "Ph_fromTarget_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromTarget_Pt_reco_onetwo_Target);
  Ph_fromPions_Pt_reco_onetwo_Target = new TH1D("Ph_fromPions_Pt_reco_onetwo_Target",
                                                "Ph_fromPions_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromPions_Pt_reco_onetwo_Target);
  Ph_fromEtas_Pt_reco_onetwo_Target =
    new TH1D("Ph_fromEtas_Pt_reco_onetwo_Target", "Ph_fromEtas_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromEtas_Pt_reco_onetwo_Target);
  Ph_fromXi_Pt_reco_onetwo_Target =
    new TH1D("Ph_fromXi_Pt_reco_onetwo_Target", "Ph_fromXi_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromXi_Pt_reco_onetwo_Target);
  Ph_fromDalitz_Pt_reco_onetwo_Target = new TH1D("Ph_fromDalitz_Pt_reco_onetwo_Target",
                                                 "Ph_fromDalitz_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromDalitz_Pt_reco_onetwo_Target);
  Ph_fromOther_Pt_reco_onetwo_Target = new TH1D("Ph_fromOther_Pt_reco_onetwo_Target",
                                                "Ph_fromOther_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromOther_Pt_reco_onetwo_Target);
  Ph_twoFromTarget_Pt_reco_onetwo_Target = new TH1D(
    "Ph_twoFromTarget_Pt_reco_onetwo_Target", "Ph_twoFromTarget_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_twoFromTarget_Pt_reco_onetwo_Target);
  Ph_fromCombinatorial_Pt_reco_onetwo_Target =
    new TH1D("Ph_fromCombinatorial_Pt_reco_onetwo_Target",
             "Ph_fromCombinatorial_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromCombinatorial_Pt_reco_onetwo_Target);
  Ph_fromConversion_Pt_reco_onetwo_Target =
    new TH1D("Ph_fromConversion_Pt_reco_onetwo_Target", "Ph_fromConversion_Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#",
             30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Ph_fromConversion_Pt_reco_onetwo_Target);

  twoFromTarget_PDG_onetwo_Target =
    new TH1D("twoFromTarget_PDG_onetwo_Target", "twoFromTarget_PDG_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(twoFromTarget_PDG_onetwo_Target);
  fromCombinatorial_PDG_onetwo_Target =
    new TH1D("fromCombinatorial_PDG_onetwo_Target", "fromCombinatorial_PDG_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(fromCombinatorial_PDG_onetwo_Target);
  CombinatorialMotherPdg_onetwo_Target =
    new TH1D("CombinatorialMotherPdg_onetwo_Target", "CombinatorialMotherPdg_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(CombinatorialMotherPdg_onetwo_Target);
  CombinatorialGrMotherPdg_onetwo_Target =
    new TH1D("CombinatorialGrMotherPdg_onetwo_Target", "CombinatorialGrMotherPdg_onetwo_Target; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Target.push_back(CombinatorialGrMotherPdg_onetwo_Target);
  Electrons_onetwo_Target =
    new TH1D("Electrons_onetwo_Target", "Electrons_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Electrons_onetwo_Target);
  Pions_onetwo_Target = new TH1D("Pions_onetwo_Target", "Pions_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(Pions_onetwo_Target);
  PionElectron_onetwo_Target =
    new TH1D("PionElectron_onetwo_Target", "PionElectron_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(PionElectron_onetwo_Target);
  elsePionOrElectron_onetwo_Target =
    new TH1D("elsePionOrElectron_onetwo_Target", "elsePionOrElectron_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(elsePionOrElectron_onetwo_Target);
  DalitzAndConversion_Pt_onetwo_Target = new TH1D(
    "DalitzAndConversion_Pt_onetwo_Target", "DalitzAndConversion_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DalitzAndConversion_Pt_onetwo_Target);
  DoubleConversion_Pt_onetwo_Target =
    new TH1D("DoubleConversion_Pt_onetwo_Target", "DoubleConversion_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DoubleConversion_Pt_onetwo_Target);
  fromFireball_P_onetwo_Target =
    new TH1D("fromFireball_P_onetwo_Target", "fromFireball_P_onetwo_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Target.push_back(fromFireball_P_onetwo_Target);
  twoFromTarget_P_onetwo_Target =
    new TH1D("twoFromTarget_P_onetwo_Target", "twoFromTarget_P_onetwo_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Target.push_back(twoFromTarget_P_onetwo_Target);
  fromCombinatorial_electron_P_onetwo_Target =
    new TH1D("fromCombinatorial_electron_P_onetwo_Target",
             "fromCombinatorial_electron_P_onetwo_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Target.push_back(fromCombinatorial_electron_P_onetwo_Target);
  fromCombinatorial_NOTelectron_P_onetwo_Target =
    new TH1D("fromCombinatorial_NOTelectron_P_onetwo_Target",
             "fromCombinatorial_NOTelectron_P_onetwo_Target; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Target.push_back(fromCombinatorial_NOTelectron_P_onetwo_Target);


  // Outside => all
  DP_InvMassReco_all_Outside = new TH1D("DP_InvMassReco_all_Outside",
                                        "DP_InvMassReco_all_Outside; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_all_Outside.push_back(DP_InvMassReco_all_Outside);
  DP_OpeningAngleReco_all_Outside =
    new TH1D("DP_OpeningAngleReco_all_Outside", "DP_OpeningAngleReco_all_Outside; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_all_Outside.push_back(DP_OpeningAngleReco_all_Outside);
  DP_Pdg_all_Outside = new TH1D("DP_Pdg_all_Outside", "DP_Pdg_all_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Outside.push_back(DP_Pdg_all_Outside);
  DP_P_reco_all_Outside = new TH1D("DP_P_reco_all_Outside", "DP_P_reco_all_Outside; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_all_Outside.push_back(DP_P_reco_all_Outside);
  DP_Pt_reco_all_Outside = new TH1D("DP_Pt_reco_all_Outside", "DP_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(DP_Pt_reco_all_Outside);
  Ph_fromTarget_Pt_reco_all_Outside =
    new TH1D("Ph_fromTarget_Pt_reco_all_Outside", "Ph_fromTarget_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(Ph_fromTarget_Pt_reco_all_Outside);
  Ph_fromPions_Pt_reco_all_Outside =
    new TH1D("Ph_fromPions_Pt_reco_all_Outside", "Ph_fromPions_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(Ph_fromPions_Pt_reco_all_Outside);
  Ph_fromEtas_Pt_reco_all_Outside =
    new TH1D("Ph_fromEtas_Pt_reco_all_Outside", "Ph_fromEtas_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(Ph_fromEtas_Pt_reco_all_Outside);
  Ph_fromDalitz_Pt_reco_all_Outside =
    new TH1D("Ph_fromDalitz_Pt_reco_all_Outside", "Ph_fromDalitz_Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(Ph_fromDalitz_Pt_reco_all_Outside);

  // Outside => zero
  DP_InvMassReco_zero_Outside = new TH1D(
    "DP_InvMassReco_zero_Outside", "DP_InvMassReco_zero_Outside; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_zero_Outside.push_back(DP_InvMassReco_zero_Outside);
  DP_OpeningAngleReco_zero_Outside =
    new TH1D("DP_OpeningAngleReco_zero_Outside", "DP_OpeningAngleReco_zero_Outside; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_zero_Outside.push_back(DP_OpeningAngleReco_zero_Outside);
  DP_Pdg_zero_Outside = new TH1D("DP_Pdg_zero_Outside", "DP_Pdg_zero_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_zero_Outside.push_back(DP_Pdg_zero_Outside);
  DP_P_reco_zero_Outside = new TH1D("DP_P_reco_zero_Outside", "DP_P_reco_zero_Outside; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_zero_Outside.push_back(DP_P_reco_zero_Outside);
  DP_Pt_reco_zero_Outside =
    new TH1D("DP_Pt_reco_zero_Outside", "DP_Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(DP_Pt_reco_zero_Outside);
  Ph_fromTarget_Pt_reco_zero_Outside = new TH1D("Ph_fromTarget_Pt_reco_zero_Outside",
                                                "Ph_fromTarget_Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(Ph_fromTarget_Pt_reco_zero_Outside);
  Ph_fromPions_Pt_reco_zero_Outside =
    new TH1D("Ph_fromPions_Pt_reco_zero_Outside", "Ph_fromPions_Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(Ph_fromPions_Pt_reco_zero_Outside);
  Ph_fromEtas_Pt_reco_zero_Outside =
    new TH1D("Ph_fromEtas_Pt_reco_zero_Outside", "Ph_fromEtas_Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(Ph_fromEtas_Pt_reco_zero_Outside);
  Ph_fromDalitz_Pt_reco_zero_Outside = new TH1D("Ph_fromDalitz_Pt_reco_zero_Outside",
                                                "Ph_fromDalitz_Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(Ph_fromDalitz_Pt_reco_zero_Outside);

  // Outside => one
  DP_InvMassReco_one_Outside = new TH1D("DP_InvMassReco_one_Outside",
                                        "DP_InvMassReco_one_Outside; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_one_Outside.push_back(DP_InvMassReco_one_Outside);
  DP_OpeningAngleReco_one_Outside =
    new TH1D("DP_OpeningAngleReco_one_Outside", "DP_OpeningAngleReco_one_Outside; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_one_Outside.push_back(DP_OpeningAngleReco_one_Outside);
  DP_Pdg_one_Outside = new TH1D("DP_Pdg_one_Outside", "DP_Pdg_one_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_one_Outside.push_back(DP_Pdg_one_Outside);
  DP_P_reco_one_Outside = new TH1D("DP_P_reco_one_Outside", "DP_P_reco_one_Outside; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_one_Outside.push_back(DP_P_reco_one_Outside);
  DP_Pt_reco_one_Outside = new TH1D("DP_Pt_reco_one_Outside", "DP_Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(DP_Pt_reco_one_Outside);
  Ph_fromTarget_Pt_reco_one_Outside =
    new TH1D("Ph_fromTarget_Pt_reco_one_Outside", "Ph_fromTarget_Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(Ph_fromTarget_Pt_reco_one_Outside);
  Ph_fromPions_Pt_reco_one_Outside =
    new TH1D("Ph_fromPions_Pt_reco_one_Outside", "Ph_fromPions_Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(Ph_fromPions_Pt_reco_one_Outside);
  Ph_fromEtas_Pt_reco_one_Outside =
    new TH1D("Ph_fromEtas_Pt_reco_one_Outside", "Ph_fromEtas_Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(Ph_fromEtas_Pt_reco_one_Outside);
  Ph_fromDalitz_Pt_reco_one_Outside =
    new TH1D("Ph_fromDalitz_Pt_reco_one_Outside", "Ph_fromDalitz_Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(Ph_fromDalitz_Pt_reco_one_Outside);

  // Outside => two
  DP_InvMassReco_two_Outside = new TH1D("DP_InvMassReco_two_Outside",
                                        "DP_InvMassReco_two_Outside; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_two_Outside.push_back(DP_InvMassReco_two_Outside);
  DP_OpeningAngleReco_two_Outside =
    new TH1D("DP_OpeningAngleReco_two_Outside", "DP_OpeningAngleReco_two_Outside; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_two_Outside.push_back(DP_OpeningAngleReco_two_Outside);
  DP_Pdg_two_Outside = new TH1D("DP_Pdg_two_Outside", "DP_Pdg_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(DP_Pdg_two_Outside);
  DP_P_reco_two_Outside = new TH1D("DP_P_reco_two_Outside", "DP_P_reco_two_Outside; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_two_Outside.push_back(DP_P_reco_two_Outside);
  DP_Pt_reco_two_Outside = new TH1D("DP_Pt_reco_two_Outside", "DP_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DP_Pt_reco_two_Outside);
  Ph_fromTarget_Pt_reco_two_Outside =
    new TH1D("Ph_fromTarget_Pt_reco_two_Outside", "Ph_fromTarget_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromTarget_Pt_reco_two_Outside);
  Ph_fromPions_Pt_reco_two_Outside =
    new TH1D("Ph_fromPions_Pt_reco_two_Outside", "Ph_fromPions_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromPions_Pt_reco_two_Outside);
  Ph_fromEtas_Pt_reco_two_Outside =
    new TH1D("Ph_fromEtas_Pt_reco_two_Outside", "Ph_fromEtas_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromEtas_Pt_reco_two_Outside);
  Ph_fromXi_Pt_reco_two_Outside =
    new TH1D("Ph_fromXi_Pt_reco_two_Outside", "Ph_fromXi_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromXi_Pt_reco_two_Outside);
  Ph_fromDalitz_Pt_reco_two_Outside =
    new TH1D("Ph_fromDalitz_Pt_reco_two_Outside", "Ph_fromDalitz_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromDalitz_Pt_reco_two_Outside);
  Ph_fromOther_Pt_reco_two_Outside =
    new TH1D("Ph_fromOther_Pt_reco_two_Outside", "Ph_fromOther_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromOther_Pt_reco_two_Outside);
  Ph_twoFromTarget_Pt_reco_two_Outside = new TH1D(
    "Ph_twoFromTarget_Pt_reco_two_Outside", "Ph_twoFromTarget_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_twoFromTarget_Pt_reco_two_Outside);
  Ph_fromCombinatorial_Pt_reco_two_Outside =
    new TH1D("Ph_fromCombinatorial_Pt_reco_two_Outside",
             "Ph_fromCombinatorial_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromCombinatorial_Pt_reco_two_Outside);
  Ph_fromConversion_Pt_reco_two_Outside = new TH1D(
    "Ph_fromConversion_Pt_reco_two_Outside", "Ph_fromConversion_Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Ph_fromConversion_Pt_reco_two_Outside);
  twoFromTarget_PDG_two_Outside =
    new TH1D("twoFromTarget_PDG_two_Outside", "twoFromTarget_PDG_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(twoFromTarget_PDG_two_Outside);
  fromCombinatorial_PDG_two_Outside =
    new TH1D("fromCombinatorial_PDG_two_Outside", "fromCombinatorial_PDG_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(fromCombinatorial_PDG_two_Outside);
  CombinatorialMotherPdg_two_Outside =
    new TH1D("CombinatorialMotherPdg_two_Outside", "CombinatorialMotherPdg_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(CombinatorialMotherPdg_two_Outside);
  CombinatorialGrMotherPdg_two_Outside =
    new TH1D("CombinatorialGrMotherPdg_two_Outside", "CombinatorialGrMotherPdg_two_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Outside.push_back(CombinatorialGrMotherPdg_two_Outside);
  Electrons_two_Outside = new TH1D("Electrons_two_Outside", "Electrons_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Electrons_two_Outside);
  Pions_two_Outside = new TH1D("Pions_two_Outside", "Pions_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(Pions_two_Outside);
  PionElectron_two_Outside =
    new TH1D("PionElectron_two_Outside", "PionElectron_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(PionElectron_two_Outside);
  elsePionOrElectron_two_Outside =
    new TH1D("elsePionOrElectron_two_Outside", "elsePionOrElectron_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(elsePionOrElectron_two_Outside);
  DalitzAndConversion_Pt_two_Outside = new TH1D("DalitzAndConversion_Pt_two_Outside",
                                                "DalitzAndConversion_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DalitzAndConversion_Pt_two_Outside);
  DoubleConversion_Pt_two_Outside =
    new TH1D("DoubleConversion_Pt_two_Outside", "DoubleConversion_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DoubleConversion_Pt_two_Outside);
  fromFireball_P_two_Outside =
    new TH1D("fromFireball_P_two_Outside", "fromFireball_P_two_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Outside.push_back(fromFireball_P_two_Outside);
  twoFromTarget_P_two_Outside =
    new TH1D("twoFromTarget_P_two_Outside", "twoFromTarget_P_two_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Outside.push_back(twoFromTarget_P_two_Outside);
  fromCombinatorial_electron_P_two_Outside =
    new TH1D("fromCombinatorial_electron_P_two_Outside",
             "fromCombinatorial_electron_P_two_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Outside.push_back(fromCombinatorial_electron_P_two_Outside);
  fromCombinatorial_NOTelectron_P_two_Outside =
    new TH1D("fromCombinatorial_NOTelectron_P_two_Outside",
             "fromCombinatorial_NOTelectron_P_two_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_two_Outside.push_back(fromCombinatorial_NOTelectron_P_two_Outside);

  // Outside => onetwo
  DP_InvMassReco_onetwo_Outside = new TH1D(
    "DP_InvMassReco_onetwo_Outside", "DP_InvMassReco_onetwo_Outside; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_onetwo_Outside.push_back(DP_InvMassReco_onetwo_Outside);
  DP_OpeningAngleReco_onetwo_Outside =
    new TH1D("DP_OpeningAngleReco_onetwo_Outside", "DP_OpeningAngleReco_onetwo_Outside; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_onetwo_Outside.push_back(DP_OpeningAngleReco_onetwo_Outside);
  DP_Pdg_onetwo_Outside = new TH1D("DP_Pdg_onetwo_Outside", "DP_Pdg_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(DP_Pdg_onetwo_Outside);
  DP_P_reco_onetwo_Outside =
    new TH1D("DP_P_reco_onetwo_Outside", "DP_P_reco_onetwo_Outside; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_onetwo_Outside.push_back(DP_P_reco_onetwo_Outside);
  DP_Pt_reco_onetwo_Outside =
    new TH1D("DP_Pt_reco_onetwo_Outside", "DP_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DP_Pt_reco_onetwo_Outside);
  Ph_fromTarget_Pt_reco_onetwo_Outside = new TH1D(
    "Ph_fromTarget_Pt_reco_onetwo_Outside", "Ph_fromTarget_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromTarget_Pt_reco_onetwo_Outside);
  Ph_fromPions_Pt_reco_onetwo_Outside = new TH1D("Ph_fromPions_Pt_reco_onetwo_Outside",
                                                 "Ph_fromPions_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromPions_Pt_reco_onetwo_Outside);
  Ph_fromEtas_Pt_reco_onetwo_Outside = new TH1D("Ph_fromEtas_Pt_reco_onetwo_Outside",
                                                "Ph_fromEtas_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromEtas_Pt_reco_onetwo_Outside);
  Ph_fromXi_Pt_reco_onetwo_Outside =
    new TH1D("Ph_fromXi_Pt_reco_onetwo_Outside", "Ph_fromXi_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromXi_Pt_reco_onetwo_Outside);
  Ph_fromDalitz_Pt_reco_onetwo_Outside = new TH1D(
    "Ph_fromDalitz_Pt_reco_onetwo_Outside", "Ph_fromDalitz_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromDalitz_Pt_reco_onetwo_Outside);
  Ph_fromOther_Pt_reco_onetwo_Outside = new TH1D("Ph_fromOther_Pt_reco_onetwo_Outside",
                                                 "Ph_fromOther_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromOther_Pt_reco_onetwo_Outside);
  Ph_twoFromTarget_Pt_reco_onetwo_Outside =
    new TH1D("Ph_twoFromTarget_Pt_reco_onetwo_Outside", "Ph_twoFromTarget_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#",
             30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_twoFromTarget_Pt_reco_onetwo_Outside);
  Ph_fromCombinatorial_Pt_reco_onetwo_Outside =
    new TH1D("Ph_fromCombinatorial_Pt_reco_onetwo_Outside",
             "Ph_fromCombinatorial_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromCombinatorial_Pt_reco_onetwo_Outside);
  Ph_fromConversion_Pt_reco_onetwo_Outside =
    new TH1D("Ph_fromConversion_Pt_reco_onetwo_Outside",
             "Ph_fromConversion_Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Ph_fromConversion_Pt_reco_onetwo_Outside);
  twoFromTarget_PDG_onetwo_Outside =
    new TH1D("twoFromTarget_PDG_onetwo_Outside", "twoFromTarget_PDG_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(twoFromTarget_PDG_onetwo_Outside);
  fromCombinatorial_PDG_onetwo_Outside =
    new TH1D("fromCombinatorial_PDG_onetwo_Outside", "fromCombinatorial_PDG_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(fromCombinatorial_PDG_onetwo_Outside);
  CombinatorialMotherPdg_onetwo_Outside =
    new TH1D("CombinatorialMotherPdg_onetwo_Outside", "CombinatorialMotherPdg_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(CombinatorialMotherPdg_onetwo_Outside);
  CombinatorialGrMotherPdg_onetwo_Outside = new TH1D("CombinatorialGrMotherPdg_onetwo_Outside",
                                                     "CombinatorialGrMotherPdg_onetwo_Outside; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Outside.push_back(CombinatorialGrMotherPdg_onetwo_Outside);
  Electrons_onetwo_Outside =
    new TH1D("Electrons_onetwo_Outside", "Electrons_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Electrons_onetwo_Outside);
  Pions_onetwo_Outside = new TH1D("Pions_onetwo_Outside", "Pions_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(Pions_onetwo_Outside);
  PionElectron_onetwo_Outside =
    new TH1D("PionElectron_onetwo_Outside", "PionElectron_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(PionElectron_onetwo_Outside);
  elsePionOrElectron_onetwo_Outside =
    new TH1D("elsePionOrElectron_onetwo_Outside", "elsePionOrElectron_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(elsePionOrElectron_onetwo_Outside);
  DalitzAndConversion_Pt_onetwo_Outside = new TH1D(
    "DalitzAndConversion_Pt_onetwo_Outside", "DalitzAndConversion_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DalitzAndConversion_Pt_onetwo_Outside);
  DoubleConversion_Pt_onetwo_Outside = new TH1D("DoubleConversion_Pt_onetwo_Outside",
                                                "DoubleConversion_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DoubleConversion_Pt_onetwo_Outside);
  fromFireball_P_onetwo_Outside =
    new TH1D("fromFireball_P_onetwo_Outside", "fromFireball_P_onetwo_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Outside.push_back(fromFireball_P_onetwo_Outside);
  twoFromTarget_P_onetwo_Outside =
    new TH1D("twoFromTarget_P_onetwo_Outside", "twoFromTarget_P_onetwo_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Outside.push_back(twoFromTarget_P_onetwo_Outside);
  fromCombinatorial_electron_P_onetwo_Outside =
    new TH1D("fromCombinatorial_electron_P_onetwo_Outside",
             "fromCombinatorial_electron_P_onetwo_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Outside.push_back(fromCombinatorial_electron_P_onetwo_Outside);
  fromCombinatorial_NOTelectron_P_onetwo_Outside =
    new TH1D("fromCombinatorial_NOTelectron_P_onetwo_Outside",
             "fromCombinatorial_NOTelectron_P_onetwo_Outside; P_{t} in GeV/c^{2};#", 120, 0, 6);
  fHistoList_dp_onetwo_Outside.push_back(fromCombinatorial_NOTelectron_P_onetwo_Outside);


  // Both => all
  DP_InvMassReco_all_Both =
    new TH1D("DP_InvMassReco_all_Both", "DP_InvMassReco_all_Both; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_all_Both.push_back(DP_InvMassReco_all_Both);
  DP_OpeningAngleReco_all_Both =
    new TH1D("DP_OpeningAngleReco_all_Both", "DP_OpeningAngleReco_all_Both; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_all_Both.push_back(DP_OpeningAngleReco_all_Both);
  DP_Pdg_all_Both = new TH1D("DP_Pdg_all_Both", "DP_Pdg_all_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_all_Both.push_back(DP_Pdg_all_Both);
  DP_P_reco_all_Both = new TH1D("DP_P_reco_all_Both", "DP_P_reco_all_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_all_Both.push_back(DP_P_reco_all_Both);
  DP_Pt_reco_all_Both = new TH1D("DP_Pt_reco_all_Both", "DP_Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(DP_Pt_reco_all_Both);
  Ph_fromTarget_Pt_reco_all_Both =
    new TH1D("Ph_fromTarget_Pt_reco_all_Both", "Ph_fromTarget_Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(Ph_fromTarget_Pt_reco_all_Both);
  Ph_fromPions_Pt_reco_all_Both =
    new TH1D("Ph_fromPions_Pt_reco_all_Both", "Ph_fromPions_Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(Ph_fromPions_Pt_reco_all_Both);
  Ph_fromEtas_Pt_reco_all_Both =
    new TH1D("Ph_fromEtas_Pt_reco_all_Both", "Ph_fromEtas_Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(Ph_fromEtas_Pt_reco_all_Both);
  Ph_fromDalitz_Pt_reco_all_Both =
    new TH1D("Ph_fromDalitz_Pt_reco_all_Both", "Ph_fromDalitz_Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(Ph_fromDalitz_Pt_reco_all_Both);
  Ph_pt_vs_rap_est_all_Both = new TH2D(
    "Ph_pt_vs_rap_est_all_Both", "Ph_pt_vs_rap_est_all_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_dp_all_Both.push_back(Ph_pt_vs_rap_est_all_Both);

  // Both => zero
  DP_InvMassReco_zero_Both =
    new TH1D("DP_InvMassReco_zero_Both", "DP_InvMassReco_zero_Both; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_zero_Both.push_back(DP_InvMassReco_zero_Both);
  DP_OpeningAngleReco_zero_Both =
    new TH1D("DP_OpeningAngleReco_zero_Both", "DP_OpeningAngleReco_zero_Both; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_zero_Both.push_back(DP_OpeningAngleReco_zero_Both);
  DP_Pdg_zero_Both = new TH1D("DP_Pdg_zero_Both", "DP_Pdg_zero_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_zero_Both.push_back(DP_Pdg_zero_Both);
  DP_P_reco_zero_Both = new TH1D("DP_P_reco_zero_Both", "DP_P_reco_zero_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_zero_Both.push_back(DP_P_reco_zero_Both);
  DP_Pt_reco_zero_Both = new TH1D("DP_Pt_reco_zero_Both", "DP_Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(DP_Pt_reco_zero_Both);
  Ph_fromTarget_Pt_reco_zero_Both =
    new TH1D("Ph_fromTarget_Pt_reco_zero_Both", "Ph_fromTarget_Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(Ph_fromTarget_Pt_reco_zero_Both);
  Ph_fromPions_Pt_reco_zero_Both =
    new TH1D("Ph_fromPions_Pt_reco_zero_Both", "Ph_fromPions_Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(Ph_fromPions_Pt_reco_zero_Both);
  Ph_fromEtas_Pt_reco_zero_Both =
    new TH1D("Ph_fromEtas_Pt_reco_zero_Both", "Ph_fromEtas_Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(Ph_fromEtas_Pt_reco_zero_Both);
  Ph_fromDalitz_Pt_reco_zero_Both =
    new TH1D("Ph_fromDalitz_Pt_reco_zero_Both", "Ph_fromDalitz_Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(Ph_fromDalitz_Pt_reco_zero_Both);
  Ph_pt_vs_rap_est_zero_Both = new TH2D(
    "Ph_pt_vs_rap_est_zero_Both", "Ph_pt_vs_rap_est_zero_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_dp_zero_Both.push_back(Ph_pt_vs_rap_est_zero_Both);

  // Both => one
  DP_InvMassReco_one_Both =
    new TH1D("DP_InvMassReco_one_Both", "DP_InvMassReco_one_Both; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_one_Both.push_back(DP_InvMassReco_one_Both);
  DP_OpeningAngleReco_one_Both =
    new TH1D("DP_OpeningAngleReco_one_Both", "DP_OpeningAngleReco_one_Both; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_one_Both.push_back(DP_OpeningAngleReco_one_Both);
  DP_Pdg_one_Both = new TH1D("DP_Pdg_one_Both", "DP_Pdg_one_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_one_Both.push_back(DP_Pdg_one_Both);
  DP_P_reco_one_Both = new TH1D("DP_P_reco_one_Both", "DP_P_reco_one_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_one_Both.push_back(DP_P_reco_one_Both);
  DP_Pt_reco_one_Both = new TH1D("DP_Pt_reco_one_Both", "DP_Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(DP_Pt_reco_one_Both);
  Ph_fromTarget_Pt_reco_one_Both =
    new TH1D("Ph_fromTarget_Pt_reco_one_Both", "Ph_fromTarget_Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(Ph_fromTarget_Pt_reco_one_Both);
  Ph_fromPions_Pt_reco_one_Both =
    new TH1D("Ph_fromPions_Pt_reco_one_Both", "Ph_fromPions_Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(Ph_fromPions_Pt_reco_one_Both);
  Ph_fromEtas_Pt_reco_one_Both =
    new TH1D("Ph_fromEtas_Pt_reco_one_Both", "Ph_fromEtas_Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(Ph_fromEtas_Pt_reco_one_Both);
  Ph_fromDalitz_Pt_reco_one_Both =
    new TH1D("Ph_fromDalitz_Pt_reco_one_Both", "Ph_fromDalitz_Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(Ph_fromDalitz_Pt_reco_one_Both);
  Ph_pt_vs_rap_est_one_Both = new TH2D(
    "Ph_pt_vs_rap_est_one_Both", "Ph_pt_vs_rap_est_one_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_dp_one_Both.push_back(Ph_pt_vs_rap_est_one_Both);

  // Both => two
  DP_InvMassReco_two_Both =
    new TH1D("DP_InvMassReco_two_Both", "DP_InvMassReco_two_Both; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_two_Both.push_back(DP_InvMassReco_two_Both);
  DP_OpeningAngleReco_two_Both =
    new TH1D("DP_OpeningAngleReco_two_Both", "DP_OpeningAngleReco_two_Both; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_two_Both.push_back(DP_OpeningAngleReco_two_Both);
  DP_Pdg_two_Both = new TH1D("DP_Pdg_two_Both", "DP_Pdg_two_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_two_Both.push_back(DP_Pdg_two_Both);
  DP_P_reco_two_Both = new TH1D("DP_P_reco_two_Both", "DP_P_reco_two_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_two_Both.push_back(DP_P_reco_two_Both);
  DP_Pt_reco_two_Both = new TH1D("DP_Pt_reco_two_Both", "DP_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(DP_Pt_reco_two_Both);
  Ph_fromTarget_Pt_reco_two_Both =
    new TH1D("Ph_fromTarget_Pt_reco_two_Both", "Ph_fromTarget_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromTarget_Pt_reco_two_Both);
  Ph_fromPions_Pt_reco_two_Both =
    new TH1D("Ph_fromPions_Pt_reco_two_Both", "Ph_fromPions_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromPions_Pt_reco_two_Both);
  Ph_fromEtas_Pt_reco_two_Both =
    new TH1D("Ph_fromEtas_Pt_reco_two_Both", "Ph_fromEtas_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromEtas_Pt_reco_two_Both);
  Ph_fromDalitz_Pt_reco_two_Both =
    new TH1D("Ph_fromDalitz_Pt_reco_two_Both", "Ph_fromDalitz_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromDalitz_Pt_reco_two_Both);
  Ph_twoFromTarget_Pt_reco_two_Both =
    new TH1D("Ph_twoFromTarget_Pt_reco_two_Both", "Ph_twoFromTarget_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_twoFromTarget_Pt_reco_two_Both);
  Ph_fromCombinatorial_Pt_reco_two_Both = new TH1D(
    "Ph_fromCombinatorial_Pt_reco_two_Both", "Ph_fromCombinatorial_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromCombinatorial_Pt_reco_two_Both);
  Ph_fromConversion_Pt_reco_two_Both = new TH1D("Ph_fromConversion_Pt_reco_two_Both",
                                                "Ph_fromConversion_Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(Ph_fromConversion_Pt_reco_two_Both);
  Ph_pt_vs_rap_est_two_Both = new TH2D(
    "Ph_pt_vs_rap_est_two_Both", "Ph_pt_vs_rap_est_two_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_dp_two_Both.push_back(Ph_pt_vs_rap_est_two_Both);

  // Both => onetwo
  DP_InvMassReco_onetwo_Both = new TH1D("DP_InvMassReco_onetwo_Both",
                                        "DP_InvMassReco_onetwo_Both; invariant mass in GeV/c^{2};#", 50, -0.005, 0.045);
  fHistoList_dp_onetwo_Both.push_back(DP_InvMassReco_onetwo_Both);
  DP_OpeningAngleReco_onetwo_Both =
    new TH1D("DP_OpeningAngleReco_onetwo_Both", "DP_OpeningAngleReco_onetwo_Both; angle [deg];#", 45, -0.5, 4.);
  fHistoList_dp_onetwo_Both.push_back(DP_OpeningAngleReco_onetwo_Both);
  DP_Pdg_onetwo_Both = new TH1D("DP_Pdg_onetwo_Both", "DP_Pdg_onetwo_Both; Id;#", 1000, -10, 490);
  fHistoList_dp_onetwo_Both.push_back(DP_Pdg_onetwo_Both);
  DP_P_reco_onetwo_Both = new TH1D("DP_P_reco_onetwo_Both", "DP_P_reco_onetwo_Both; P in GeV/c^{2};#", 200, 0, 10);
  fHistoList_dp_onetwo_Both.push_back(DP_P_reco_onetwo_Both);
  DP_Pt_reco_onetwo_Both = new TH1D("DP_Pt_reco_onetwo_Both", "DP_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(DP_Pt_reco_onetwo_Both);
  Ph_fromTarget_Pt_reco_onetwo_Both =
    new TH1D("Ph_fromTarget_Pt_reco_onetwo_Both", "Ph_fromTarget_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromTarget_Pt_reco_onetwo_Both);
  Ph_fromPions_Pt_reco_onetwo_Both =
    new TH1D("Ph_fromPions_Pt_reco_onetwo_Both", "Ph_fromPions_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromPions_Pt_reco_onetwo_Both);
  Ph_fromEtas_Pt_reco_onetwo_Both =
    new TH1D("Ph_fromEtas_Pt_reco_onetwo_Both", "Ph_fromEtas_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromEtas_Pt_reco_onetwo_Both);
  Ph_fromDalitz_Pt_reco_onetwo_Both =
    new TH1D("Ph_fromDalitz_Pt_reco_onetwo_Both", "Ph_fromDalitz_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromDalitz_Pt_reco_onetwo_Both);
  Ph_twoFromTarget_Pt_reco_onetwo_Both = new TH1D(
    "Ph_twoFromTarget_Pt_reco_onetwo_Both", "Ph_twoFromTarget_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_twoFromTarget_Pt_reco_onetwo_Both);
  Ph_fromCombinatorial_Pt_reco_onetwo_Both =
    new TH1D("Ph_fromCombinatorial_Pt_reco_onetwo_Both",
             "Ph_fromCombinatorial_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromCombinatorial_Pt_reco_onetwo_Both);
  Ph_fromConversion_Pt_reco_onetwo_Both = new TH1D(
    "Ph_fromConversion_Pt_reco_onetwo_Both", "Ph_fromConversion_Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(Ph_fromConversion_Pt_reco_onetwo_Both);
  Ph_pt_vs_rap_est_onetwo_Both =
    new TH2D("Ph_pt_vs_rap_est_onetwo_Both", "Ph_pt_vs_rap_est_onetwo_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_dp_onetwo_Both.push_back(Ph_pt_vs_rap_est_onetwo_Both);


  DP_EMT_Pt_all_Target = new TH1D("DP_EMT_Pt_all_Target", "DP_EMT_Pt_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(DP_EMT_Pt_all_Target);
  DP_EMT_Pt_zero_Target = new TH1D("DP_EMT_Pt_zero_Target", "DP_EMT_Pt_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(DP_EMT_Pt_zero_Target);
  DP_EMT_Pt_one_Target = new TH1D("DP_EMT_Pt_one_Target", "DP_EMT_Pt_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(DP_EMT_Pt_one_Target);
  DP_EMT_Pt_two_Target = new TH1D("DP_EMT_Pt_two_Target", "DP_EMT_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DP_EMT_Pt_two_Target);
  DP_EMT_Pt_onetwo_Target =
    new TH1D("DP_EMT_Pt_onetwo_Target", "DP_EMT_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DP_EMT_Pt_onetwo_Target);


  DP_EMT_Pt_all_Outside = new TH1D("DP_EMT_Pt_all_Outside", "DP_EMT_Pt_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(DP_EMT_Pt_all_Outside);
  DP_EMT_Pt_zero_Outside = new TH1D("DP_EMT_Pt_zero_Outside", "DP_EMT_Pt_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(DP_EMT_Pt_zero_Outside);
  DP_EMT_Pt_one_Outside = new TH1D("DP_EMT_Pt_one_Outside", "DP_EMT_Pt_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(DP_EMT_Pt_one_Outside);
  DP_EMT_Pt_two_Outside = new TH1D("DP_EMT_Pt_two_Outside", "DP_EMT_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DP_EMT_Pt_two_Outside);
  DP_EMT_Pt_onetwo_Outside =
    new TH1D("DP_EMT_Pt_onetwo_Outside", "DP_EMT_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DP_EMT_Pt_onetwo_Outside);


  DP_EMT_Pt_all_Both = new TH1D("DP_EMT_Pt_all_Both", "DP_EMT_Pt_all_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Both.push_back(DP_EMT_Pt_all_Both);
  DP_EMT_Pt_zero_Both = new TH1D("DP_EMT_Pt_zero_Both", "DP_EMT_Pt_zero_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Both.push_back(DP_EMT_Pt_zero_Both);
  DP_EMT_Pt_one_Both = new TH1D("DP_EMT_Pt_one_Both", "DP_EMT_Pt_one_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Both.push_back(DP_EMT_Pt_one_Both);
  DP_EMT_Pt_two_Both = new TH1D("DP_EMT_Pt_two_Both", "DP_EMT_Pt_two_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Both.push_back(DP_EMT_Pt_two_Both);
  DP_EMT_Pt_onetwo_Both = new TH1D("DP_EMT_Pt_onetwo_Both", "DP_EMT_Pt_onetwo_Both; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Both.push_back(DP_EMT_Pt_onetwo_Both);


  // DP_LK_EMT Target
  DP_LK_EMT_neg_Pt_all_Target =
    new TH1D("DP_LK_EMT_neg_Pt_all_Target", "DP_LK_EMT_neg_Pt_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(DP_LK_EMT_neg_Pt_all_Target);
  DP_LK_EMT_neg_Pt_zero_Target =
    new TH1D("DP_LK_EMT_neg_Pt_zero_Target", "DP_LK_EMT_neg_Pt_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(DP_LK_EMT_neg_Pt_zero_Target);
  DP_LK_EMT_neg_Pt_one_Target =
    new TH1D("DP_LK_EMT_neg_Pt_one_Target", "DP_LK_EMT_neg_Pt_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(DP_LK_EMT_neg_Pt_one_Target);
  DP_LK_EMT_neg_Pt_two_Target =
    new TH1D("DP_LK_EMT_neg_Pt_two_Target", "DP_LK_EMT_neg_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DP_LK_EMT_neg_Pt_two_Target);
  DP_LK_EMT_neg_Pt_onetwo_Target =
    new TH1D("DP_LK_EMT_neg_Pt_onetwo_Target", "DP_LK_EMT_neg_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DP_LK_EMT_neg_Pt_onetwo_Target);

  DP_LK_EMT_pos_Pt_all_Target =
    new TH1D("DP_LK_EMT_pos_Pt_all_Target", "DP_LK_EMT_pos_Pt_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(DP_LK_EMT_pos_Pt_all_Target);
  DP_LK_EMT_pos_Pt_zero_Target =
    new TH1D("DP_LK_EMT_pos_Pt_zero_Target", "DP_LK_EMT_pos_Pt_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(DP_LK_EMT_pos_Pt_zero_Target);
  DP_LK_EMT_pos_Pt_one_Target =
    new TH1D("DP_LK_EMT_pos_Pt_one_Target", "DP_LK_EMT_pos_Pt_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(DP_LK_EMT_pos_Pt_one_Target);
  DP_LK_EMT_pos_Pt_two_Target =
    new TH1D("DP_LK_EMT_pos_Pt_two_Target", "DP_LK_EMT_pos_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DP_LK_EMT_pos_Pt_two_Target);
  DP_LK_EMT_pos_Pt_onetwo_Target =
    new TH1D("DP_LK_EMT_pos_Pt_onetwo_Target", "DP_LK_EMT_pos_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DP_LK_EMT_pos_Pt_onetwo_Target);

  DP_LK_EMT_Pt_all_Target =
    new TH1D("DP_LK_EMT_Pt_all_Target", "DP_LK_EMT_Pt_all_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Target.push_back(DP_LK_EMT_Pt_all_Target);
  DP_LK_EMT_Pt_zero_Target =
    new TH1D("DP_LK_EMT_Pt_zero_Target", "DP_LK_EMT_Pt_zero_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Target.push_back(DP_LK_EMT_Pt_zero_Target);
  DP_LK_EMT_Pt_one_Target =
    new TH1D("DP_LK_EMT_Pt_one_Target", "DP_LK_EMT_Pt_one_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Target.push_back(DP_LK_EMT_Pt_one_Target);
  DP_LK_EMT_Pt_two_Target =
    new TH1D("DP_LK_EMT_Pt_two_Target", "DP_LK_EMT_Pt_two_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Target.push_back(DP_LK_EMT_Pt_two_Target);
  DP_LK_EMT_Pt_onetwo_Target =
    new TH1D("DP_LK_EMT_Pt_onetwo_Target", "DP_LK_EMT_Pt_onetwo_Target; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Target.push_back(DP_LK_EMT_Pt_onetwo_Target);


  // DP_LK_EMT Outside
  DP_LK_EMT_neg_Pt_all_Outside =
    new TH1D("DP_LK_EMT_neg_Pt_all_Outside", "DP_LK_EMT_neg_Pt_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(DP_LK_EMT_neg_Pt_all_Outside);
  DP_LK_EMT_neg_Pt_zero_Outside =
    new TH1D("DP_LK_EMT_neg_Pt_zero_Outside", "DP_LK_EMT_neg_Pt_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(DP_LK_EMT_neg_Pt_zero_Outside);
  DP_LK_EMT_neg_Pt_one_Outside =
    new TH1D("DP_LK_EMT_neg_Pt_one_Outside", "DP_LK_EMT_neg_Pt_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(DP_LK_EMT_neg_Pt_one_Outside);
  DP_LK_EMT_neg_Pt_two_Outside =
    new TH1D("DP_LK_EMT_neg_Pt_two_Outside", "DP_LK_EMT_neg_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DP_LK_EMT_neg_Pt_two_Outside);
  DP_LK_EMT_neg_Pt_onetwo_Outside =
    new TH1D("DP_LK_EMT_neg_Pt_onetwo_Outside", "DP_LK_EMT_neg_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DP_LK_EMT_neg_Pt_onetwo_Outside);

  DP_LK_EMT_pos_Pt_all_Outside =
    new TH1D("DP_LK_EMT_pos_Pt_all_Outside", "DP_LK_EMT_pos_Pt_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(DP_LK_EMT_pos_Pt_all_Outside);
  DP_LK_EMT_pos_Pt_zero_Outside =
    new TH1D("DP_LK_EMT_pos_Pt_zero_Outside", "DP_LK_EMT_pos_Pt_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(DP_LK_EMT_pos_Pt_zero_Outside);
  DP_LK_EMT_pos_Pt_one_Outside =
    new TH1D("DP_LK_EMT_pos_Pt_one_Outside", "DP_LK_EMT_pos_Pt_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(DP_LK_EMT_pos_Pt_one_Outside);
  DP_LK_EMT_pos_Pt_two_Outside =
    new TH1D("DP_LK_EMT_pos_Pt_two_Outside", "DP_LK_EMT_pos_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DP_LK_EMT_pos_Pt_two_Outside);
  DP_LK_EMT_pos_Pt_onetwo_Outside =
    new TH1D("DP_LK_EMT_pos_Pt_onetwo_Outside", "DP_LK_EMT_pos_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DP_LK_EMT_pos_Pt_onetwo_Outside);

  DP_LK_EMT_Pt_all_Outside =
    new TH1D("DP_LK_EMT_Pt_all_Outside", "DP_LK_EMT_Pt_all_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_all_Outside.push_back(DP_LK_EMT_Pt_all_Outside);
  DP_LK_EMT_Pt_zero_Outside =
    new TH1D("DP_LK_EMT_Pt_zero_Outside", "DP_LK_EMT_Pt_zero_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_zero_Outside.push_back(DP_LK_EMT_Pt_zero_Outside);
  DP_LK_EMT_Pt_one_Outside =
    new TH1D("DP_LK_EMT_Pt_one_Outside", "DP_LK_EMT_Pt_one_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_one_Outside.push_back(DP_LK_EMT_Pt_one_Outside);
  DP_LK_EMT_Pt_two_Outside =
    new TH1D("DP_LK_EMT_Pt_two_Outside", "DP_LK_EMT_Pt_two_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_two_Outside.push_back(DP_LK_EMT_Pt_two_Outside);
  DP_LK_EMT_Pt_onetwo_Outside =
    new TH1D("DP_LK_EMT_Pt_onetwo_Outside", "DP_LK_EMT_Pt_onetwo_Outside; P_{t} in GeV/c^{2};#", 30, 0, 3);
  fHistoList_dp_onetwo_Outside.push_back(DP_LK_EMT_Pt_onetwo_Outside);
}
