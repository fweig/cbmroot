/** @file CbmKresConversionManual.cxx
  * @copyright Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Ievgenii Kres, Florian Uhlig [committer] **/

/**
 *    file CbmKresConversionManual.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *
 *    Central class for the pi^0 conversion analysis.
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *
 **/

#include "CbmKresConversionManual.h"

#include "CbmGlobalTrack.h"
#include "CbmKFParticleInterface.h"
#include "CbmKresConversionBG.h"
#include "CbmKresFunctions.h"
#include "CbmL1PFFitter.h"
#include "CbmMCTrack.h"
#include "CbmMvdHit.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterEllipseTau.h"
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

CbmKresConversionManual::CbmKresConversionManual()
  : fTrain(nullptr)
  , AnnTrain(0)
  , fAnnSelection(nullptr)
  , UseAnn()
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
  , fAnaBG(nullptr)
  , VStsTrack_minus_Outside()
  , VMCtracks_minus_Outside()
  , VRings_minus_Outside()
  , VStsIndex_minus_Outside()
  , VRichRing_minus_Outside()
  , VMCIndex_minus_Outside()
  , VStsTrack_plus_Outside()
  , VMCtracks_plus_Outside()
  , VRings_plus_Outside()
  , VStsIndex_plus_Outside()
  , VRichRing_plus_Outside()
  , VMCIndex_plus_Outside()
  , VMCtracks_minus_Target()
  , VStsTrack_minus_Target()
  , VMomenta_minus_Target()
  , VRings_minus_Target()
  , VStsIndex_minus_Target()
  , VRichRing_minus_Target()
  , VMCIndex_minus_Target()
  , VMCtracks_plus_Target()
  , VStsTrack_plus_Target()
  , VMomenta_plus_Target()
  , VRings_plus_Target()
  , VStsIndex_plus_Target()
  , VRichRing_plus_Target()
  , VMCIndex_plus_Target()
  , frefmomenta()
  , frefId()
  , fMCId()
  , fMCtracks()
  , EMT_man_Event_Target()
  , EMT_man_pair_momenta_Target()
  , EMT_man_NofRings_Target()
  , EMT_man_Event_Outside()
  , EMT_man_pair_momenta_Outside()
  , EMT_man_NofRings_Outside()
  , EMT_man_Event_Both()
  , EMT_man_pair_momenta_Both()
  , EMT_man_NofRings_Both()
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
  , Gammas_MCIndex_all_Target()
  , Gammas_MCIndex_zero_Target()
  , Gammas_MCIndex_one_Target()
  , Gammas_MCIndex_two_Target()
  , Gammas_MCIndex_onetwo_Target()
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
  , Gammas_MCIndex_all_Outside()
  , Gammas_MCIndex_zero_Outside()
  , Gammas_MCIndex_one_Outside()
  , Gammas_MCIndex_two_Outside()
  , Gammas_MCIndex_onetwo_Outside()
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
  , Gammas_MCIndex_all_Both()
  , Gammas_MCIndex_zero_Both()
  , Gammas_MCIndex_one_Both()
  , Gammas_MCIndex_two_Both()
  , Gammas_MCIndex_onetwo_Both()
  , Gammas_MC_all_Both()
  , Gammas_MC_zero_Both()
  , Gammas_MC_one_Both()
  , Gammas_MC_two_Both()
  , Gammas_MC_onetwo_Both()
  , Gamma_WAC()
  , Gammas_stsIndex_WAC()
  , Gammas_MCIndex_WAC()
  , Gammas_MC_WAC()
  , EMT_man_Event_WAC()
  , EMT_man_pair_momenta_WAC()
  , EMT_man_NofRings_WAC()
  , fHistoList_man_cuts_Both()
  , InvMass_vs_OA_candidates_Both(nullptr)
  , InvMass_vs_OA_fromPi0_Both(nullptr)
  , GammasInvMass_candidates_Both(nullptr)
  , GammasOA_candidates_Both(nullptr)
  , GammasInvMass_fromPi0_Both(nullptr)
  , GammasOA_fromPi0_Both(nullptr)
  , PlaneAngles_last_candidates_Both(nullptr)
  , PlaneAngles_last_fromPi0_Both(nullptr)
  , PlaneAngles_first_candidates_Both(nullptr)
  , PlaneAngles_first_fromPi0_Both(nullptr)
  , fHistoList_man_cuts_Target()
  , InvMass_vs_OA_candidates_Target(nullptr)
  , InvMass_vs_OA_fromPi0_Target(nullptr)
  , GammasInvMass_candidates_Target(nullptr)
  , GammasOA_candidates_Target(nullptr)
  , GammasInvMass_fromPi0_Target(nullptr)
  , GammasOA_fromPi0_Target(nullptr)
  , PlaneAngles_last_candidates_Target(nullptr)
  , PlaneAngles_last_fromPi0_Target(nullptr)
  , PlaneAngles_first_candidates_Target(nullptr)
  , PlaneAngles_first_fromPi0_Target(nullptr)
  , fHistoList_man_cuts_Outside()
  , InvMass_vs_OA_candidates_Outside(nullptr)
  , InvMass_vs_OA_fromPi0_Outside(nullptr)
  , GammasInvMass_candidates_Outside(nullptr)
  , GammasOA_candidates_Outside(nullptr)
  , GammasInvMass_fromPi0_Outside(nullptr)
  , GammasOA_fromPi0_Outside(nullptr)
  , PlaneAngles_last_candidates_Outside(nullptr)
  , PlaneAngles_last_fromPi0_Outside(nullptr)
  , PlaneAngles_first_candidates_Outside(nullptr)
  , PlaneAngles_first_fromPi0_Outside(nullptr)
  , fHistoList_man_all_Target()
  , GammaInvMassReco_all_Target(nullptr)
  , GammaOpeningAngleReco_all_Target(nullptr)
  , Pdg_all_Target(nullptr)
  , P_reco_all_Target(nullptr)
  , Pt_reco_all_Target(nullptr)
  , Pi0InvMassReco_all_Target(nullptr)
  , EMT_InvMass_all_Target(nullptr)
  , Pi0_pt_vs_rap_all_Target(nullptr)
  , Pi0_pt_vs_rap_est_all_Target(nullptr)
  , DalitzPi0_all_Target(nullptr)
  , PhotonsPi0_all_Target(nullptr)
  , fHistoList_man_zero_Target()
  , GammaInvMassReco_zero_Target(nullptr)
  , GammaOpeningAngleReco_zero_Target(nullptr)
  , Pdg_zero_Target(nullptr)
  , P_reco_zero_Target(nullptr)
  , Pt_reco_zero_Target(nullptr)
  , Pi0InvMassReco_zero_Target(nullptr)
  , EMT_InvMass_zero_Target(nullptr)
  , Pi0_pt_vs_rap_zero_Target(nullptr)
  , Pi0_pt_vs_rap_est_zero_Target(nullptr)
  , DalitzPi0_zero_Target(nullptr)
  , PhotonsPi0_zero_Target(nullptr)
  , fHistoList_man_one_Target()
  , GammaInvMassReco_one_Target(nullptr)
  , GammaOpeningAngleReco_one_Target(nullptr)
  , Pdg_one_Target(nullptr)
  , P_reco_one_Target(nullptr)
  , Pt_reco_one_Target(nullptr)
  , Pi0InvMassReco_one_Target(nullptr)
  , EMT_InvMass_one_Target(nullptr)
  , Pi0_pt_vs_rap_one_Target(nullptr)
  , Pi0_pt_vs_rap_est_one_Target(nullptr)
  , DalitzPi0_one_Target(nullptr)
  , PhotonsPi0_one_Target(nullptr)
  , fHistoList_man_two_Target()
  , GammaInvMassReco_two_Target(nullptr)
  , GammaOpeningAngleReco_two_Target(nullptr)
  , Pdg_two_Target(nullptr)
  , P_reco_two_Target(nullptr)
  , Pt_reco_two_Target(nullptr)
  , Pi0InvMassReco_two_Target(nullptr)
  , EMT_InvMass_two_Target(nullptr)
  , Pi0_pt_vs_rap_two_Target(nullptr)
  , Pi0_pt_vs_rap_est_two_Target(nullptr)
  , DalitzPi0_two_Target(nullptr)
  , PhotonsPi0_two_Target(nullptr)
  , fHistoList_man_onetwo_Target()
  , GammaInvMassReco_onetwo_Target(nullptr)
  , GammaOpeningAngleReco_onetwo_Target(nullptr)
  , Pdg_onetwo_Target(nullptr)
  , P_reco_onetwo_Target(nullptr)
  , Pt_reco_onetwo_Target(nullptr)
  , Pi0InvMassReco_onetwo_Target(nullptr)
  , EMT_InvMass_onetwo_Target(nullptr)
  , Pi0_pt_vs_rap_onetwo_Target(nullptr)
  , Pi0_pt_vs_rap_est_onetwo_Target(nullptr)
  , DalitzPi0_onetwo_Target(nullptr)
  , PhotonsPi0_onetwo_Target(nullptr)
  , fHistoList_man_all_Outside()
  , GammaInvMassReco_all_Outside(nullptr)
  , GammaOpeningAngleReco_all_Outside(nullptr)
  , Pdg_all_Outside(nullptr)
  , P_reco_all_Outside(nullptr)
  , Pt_reco_all_Outside(nullptr)
  , Pi0InvMassReco_all_Outside(nullptr)
  , EMT_InvMass_all_Outside(nullptr)
  , Pi0_pt_vs_rap_all_Outside(nullptr)
  , Pi0_pt_vs_rap_est_all_Outside(nullptr)
  , DalitzPi0_all_Outside(nullptr)
  , PhotonsPi0_all_Outside(nullptr)
  , fHistoList_man_zero_Outside()
  , GammaInvMassReco_zero_Outside(nullptr)
  , GammaOpeningAngleReco_zero_Outside(nullptr)
  , Pdg_zero_Outside(nullptr)
  , P_reco_zero_Outside(nullptr)
  , Pt_reco_zero_Outside(nullptr)
  , Pi0InvMassReco_zero_Outside(nullptr)
  , EMT_InvMass_zero_Outside(nullptr)
  , Pi0_pt_vs_rap_zero_Outside(nullptr)
  , Pi0_pt_vs_rap_est_zero_Outside(nullptr)
  , DalitzPi0_zero_Outside(nullptr)
  , PhotonsPi0_zero_Outside(nullptr)
  , fHistoList_man_one_Outside()
  , GammaInvMassReco_one_Outside(nullptr)
  , GammaOpeningAngleReco_one_Outside(nullptr)
  , Pdg_one_Outside(nullptr)
  , P_reco_one_Outside(nullptr)
  , Pt_reco_one_Outside(nullptr)
  , Pi0InvMassReco_one_Outside(nullptr)
  , EMT_InvMass_one_Outside(nullptr)
  , Pi0_pt_vs_rap_one_Outside(nullptr)
  , Pi0_pt_vs_rap_est_one_Outside(nullptr)
  , DalitzPi0_one_Outside(nullptr)
  , PhotonsPi0_one_Outside(nullptr)
  , fHistoList_man_two_Outside()
  , GammaInvMassReco_two_Outside(nullptr)
  , GammaOpeningAngleReco_two_Outside(nullptr)
  , Pdg_two_Outside(nullptr)
  , P_reco_two_Outside(nullptr)
  , Pt_reco_two_Outside(nullptr)
  , Pi0InvMassReco_two_Outside(nullptr)
  , EMT_InvMass_two_Outside(nullptr)
  , Pi0_pt_vs_rap_two_Outside(nullptr)
  , Pi0_pt_vs_rap_est_two_Outside(nullptr)
  , DalitzPi0_two_Outside(nullptr)
  , PhotonsPi0_two_Outside(nullptr)
  , fHistoList_man_onetwo_Outside()
  , GammaInvMassReco_onetwo_Outside(nullptr)
  , GammaOpeningAngleReco_onetwo_Outside(nullptr)
  , Pdg_onetwo_Outside(nullptr)
  , P_reco_onetwo_Outside(nullptr)
  , Pt_reco_onetwo_Outside(nullptr)
  , Pi0InvMassReco_onetwo_Outside(nullptr)
  , EMT_InvMass_onetwo_Outside(nullptr)
  , Pi0_pt_vs_rap_onetwo_Outside(nullptr)
  , Pi0_pt_vs_rap_est_onetwo_Outside(nullptr)
  , DalitzPi0_onetwo_Outside(nullptr)
  , PhotonsPi0_onetwo_Outside(nullptr)
  , fHistoList_man_all_Both()
  , GammaInvMassReco_all_Both(nullptr)
  , GammaOpeningAngleReco_all_Both(nullptr)
  , Pdg_all_Both(nullptr)
  , P_reco_all_Both(nullptr)
  , Pt_reco_all_Both(nullptr)
  , Pi0InvMassReco_all_Both(nullptr)
  , EMT_InvMass_all_Both(nullptr)
  , Pi0_pt_vs_rap_all_Both(nullptr)
  , Pi0_pt_vs_rap_est_all_Both(nullptr)
  , DalitzPi0_all_Both(nullptr)
  , PhotonsPi0_all_Both(nullptr)
  , fHistoList_man_zero_Both()
  , GammaInvMassReco_zero_Both(nullptr)
  , GammaOpeningAngleReco_zero_Both(nullptr)
  , Pdg_zero_Both(nullptr)
  , P_reco_zero_Both(nullptr)
  , Pt_reco_zero_Both(nullptr)
  , Pi0InvMassReco_zero_Both(nullptr)
  , EMT_InvMass_zero_Both(nullptr)
  , Pi0_pt_vs_rap_zero_Both(nullptr)
  , Pi0_pt_vs_rap_est_zero_Both(nullptr)
  , DalitzPi0_zero_Both(nullptr)
  , PhotonsPi0_zero_Both(nullptr)
  , fHistoList_man_one_Both()
  , GammaInvMassReco_one_Both(nullptr)
  , GammaOpeningAngleReco_one_Both(nullptr)
  , Pdg_one_Both(nullptr)
  , P_reco_one_Both(nullptr)
  , Pt_reco_one_Both(nullptr)
  , Pi0InvMassReco_one_Both(nullptr)
  , EMT_InvMass_one_Both(nullptr)
  , Pi0_pt_vs_rap_one_Both(nullptr)
  , Pi0_pt_vs_rap_est_one_Both(nullptr)
  , DalitzPi0_one_Both(nullptr)
  , PhotonsPi0_one_Both(nullptr)
  , fHistoList_man_two_Both()
  , GammaInvMassReco_two_Both(nullptr)
  , GammaOpeningAngleReco_two_Both(nullptr)
  , Pdg_two_Both(nullptr)
  , P_reco_two_Both(nullptr)
  , Pt_reco_two_Both(nullptr)
  , Pi0InvMassReco_two_Both(nullptr)
  , EMT_InvMass_two_Both(nullptr)
  , Pi0_pt_vs_rap_two_Both(nullptr)
  , Pi0_pt_vs_rap_est_two_Both(nullptr)
  , DalitzPi0_two_Both(nullptr)
  , PhotonsPi0_two_Both(nullptr)
  , fHistoList_man_onetwo_Both()
  , GammaInvMassReco_onetwo_Both(nullptr)
  , GammaOpeningAngleReco_onetwo_Both(nullptr)
  , Pdg_onetwo_Both(nullptr)
  , P_reco_onetwo_Both(nullptr)
  , Pt_reco_onetwo_Both(nullptr)
  , Pi0InvMassReco_onetwo_Both(nullptr)
  , EMT_InvMass_onetwo_Both(nullptr)
  , Pi0_pt_vs_rap_onetwo_Both(nullptr)
  , Pi0_pt_vs_rap_est_onetwo_Both(nullptr)
  , DalitzPi0_onetwo_Both(nullptr)
  , PhotonsPi0_onetwo_Both(nullptr)
  , fHistoList_man_Both()
  , Pdg_vs_Distance(nullptr)
  , P_vs_Distance(nullptr)
  , fHistoList_multiplicity_man_Target()
  , MultiplicityGamma_all_Target(nullptr)
  , MultiplicityGamma_zero_Target(nullptr)
  , MultiplicityGamma_one_Target(nullptr)
  , MultiplicityGamma_two_Target(nullptr)
  , MultiplicityGamma_onetwo_Target(nullptr)
  , MultiplicityChargedParticles_all_Target(nullptr)
  , MultiplicityChargedParticles_zero_Target(nullptr)
  , MultiplicityChargedParticles_one_Target(nullptr)
  , MultiplicityChargedParticles_two_Target(nullptr)
  , MultiplicityChargedParticles_onetwo_Target(nullptr)
  , fHistoList_multiplicity_man_Outside()
  , MultiplicityGamma_all_Outside(nullptr)
  , MultiplicityGamma_zero_Outside(nullptr)
  , MultiplicityGamma_one_Outside(nullptr)
  , MultiplicityGamma_two_Outside(nullptr)
  , MultiplicityGamma_onetwo_Outside(nullptr)
  , MultiplicityChargedParticles_all_Outside(nullptr)
  , MultiplicityChargedParticles_zero_Outside(nullptr)
  , MultiplicityChargedParticles_one_Outside(nullptr)
  , MultiplicityChargedParticles_two_Outside(nullptr)
  , MultiplicityChargedParticles_onetwo_Outside(nullptr)
  , fHistoList_multiplicity_man_Both()
  , MultiplicityGamma_all_Both(nullptr)
  , MultiplicityGamma_zero_Both(nullptr)
  , MultiplicityGamma_one_Both(nullptr)
  , MultiplicityGamma_two_Both(nullptr)
  , MultiplicityGamma_onetwo_Both(nullptr)
  , MultiplicityChargedParticles_all_Both(nullptr)
  , MultiplicityChargedParticles_zero_Both(nullptr)
  , MultiplicityChargedParticles_one_Both(nullptr)
  , MultiplicityChargedParticles_two_Both(nullptr)
  , MultiplicityChargedParticles_onetwo_Both(nullptr)
  , fHistoList_rap_vs_pt_InM()
  , rap_vs_Pt_InM_1(nullptr)
  , rap_vs_Pt_InM_2(nullptr)
  , rap_vs_Pt_InM_3(nullptr)
  , rap_vs_Pt_InM_4(nullptr)
  , rap_vs_Pt_InM_5(nullptr)
  , rap_vs_Pt_InM_6(nullptr)
  , rap_vs_Pt_InM_7(nullptr)
  , rap_vs_Pt_InM_8(nullptr)
  , rap_vs_Pt_InM_9(nullptr)
  , rap_vs_Pt_InM_10(nullptr)
  , rap_vs_Pt_InM_11(nullptr)
  , rap_vs_Pt_InM_12(nullptr)
  , rap_vs_Pt_InM_13(nullptr)
  , rap_vs_Pt_InM_14(nullptr)
  , rap_vs_Pt_InM_15(nullptr)
  , rap_vs_Pt_InM_16(nullptr)
  , rap_vs_Pt_InM_17(nullptr)
  , rap_vs_Pt_InM_18(nullptr)
  , rap_vs_Pt_InM_19(nullptr)
  , rap_vs_Pt_InM_20(nullptr)
  , rap_vs_Pt_InM_21(nullptr)
  , rap_vs_Pt_InM_22(nullptr)
  , rap_vs_Pt_InM_23(nullptr)
  , rap_vs_Pt_InM_24(nullptr)
  , rap_vs_Pt_InM_25(nullptr)
  , rap_vs_Pt_InM_26(nullptr)
  , rap_vs_Pt_InM_27(nullptr)
  , rap_vs_Pt_InM_28(nullptr)
  , rap_vs_Pt_InM_29(nullptr)
  , rap_vs_Pt_InM_30(nullptr)
  , rap_vs_Pt_InM_31(nullptr)
  , rap_vs_Pt_InM_32(nullptr)
  , rap_vs_Pt_InM_33(nullptr)
  , rap_vs_Pt_InM_34(nullptr)
  , rap_vs_Pt_InM_35(nullptr)
  , rap_vs_Pt_InM_36(nullptr)
  , rap_vs_Pt_InM_37(nullptr)
  , rap_vs_Pt_InM_38(nullptr)
  , rap_vs_Pt_InM_39(nullptr)
  , rap_vs_Pt_InM_40(nullptr)
  , rap_vs_Pt_InM_41(nullptr)
  , rap_vs_Pt_InM_42(nullptr)
  , rap_vs_Pt_InM_43(nullptr)
  , rap_vs_Pt_InM_44(nullptr)
  , rap_vs_Pt_InM_45(nullptr)
  , rap_vs_Pt_InM_46(nullptr)
  , rap_vs_Pt_InM_47(nullptr)
  , rap_vs_Pt_InM_48(nullptr)
  , rap_vs_Pt_InM_49(nullptr)
  , rap_vs_Pt_InM_50(nullptr)
  , rap_vs_Pt_InM_51(nullptr)
  , rap_vs_Pt_InM_52(nullptr)
  , rap_vs_Pt_InM_53(nullptr)
  , rap_vs_Pt_InM_54(nullptr)
  , rap_vs_Pt_InM_55(nullptr)
  , rap_vs_Pt_InM_56(nullptr)
  , rap_vs_Pt_InM_57(nullptr)
  , rap_vs_Pt_InM_58(nullptr)
  , rap_vs_Pt_InM_59(nullptr)
  , rap_vs_Pt_InM_60(nullptr)
  , rap_vs_Pt_InM_61(nullptr)
  , rap_vs_Pt_InM_62(nullptr)
  , rap_vs_Pt_InM_63(nullptr)
  , rap_vs_Pt_InM_64(nullptr)
  , rap_vs_Pt_InM_65(nullptr)
  , rap_vs_Pt_InM_66(nullptr)
  , rap_vs_Pt_InM_67(nullptr)
  , rap_vs_Pt_InM_68(nullptr)
  , rap_vs_Pt_InM_69(nullptr)
  , rap_vs_Pt_InM_70(nullptr)
  , rap_vs_Pt_InM_71(nullptr)
  , rap_vs_Pt_InM_72(nullptr)
  , rap_vs_Pt_InM_73(nullptr)
  , rap_vs_Pt_InM_74(nullptr)
  , rap_vs_Pt_InM_75(nullptr)
  , rap_vs_Pt_InM_76(nullptr)
  , rap_vs_Pt_InM_81(nullptr)
  , rap_vs_Pt_InM_82(nullptr)
  , rap_vs_Pt_InM_83(nullptr)
  , rap_vs_Pt_InM_84(nullptr)
  , rap_vs_Pt_InM_85(nullptr)
  , rap_vs_Pt_InM_86(nullptr)
  , rap_vs_Pt_InM_87(nullptr)
  , rap_vs_Pt_InM_88(nullptr)
  , rap_vs_Pt_InM_89(nullptr)
  , rap_vs_Pt_InM_90(nullptr)
  , rap_vs_Pt_InM_91(nullptr)
  , rap_vs_Pt_InM_92(nullptr)
  , rap_vs_Pt_InM_101(nullptr)
  , rap_vs_Pt_InM_102(nullptr)
  , rap_vs_Pt_InM_103(nullptr)
  , rap_vs_Pt_InM_104(nullptr)
  , rap_vs_Pt_InM_105(nullptr)
  , rap_vs_Pt_InM_106(nullptr)
  , rap_vs_Pt_InM_107(nullptr)
  , rap_vs_Pt_InM_108(nullptr)
  , rap_vs_Pt_InM_111(nullptr)
  , rap_vs_Pt_InM_112(nullptr)
  , rap_vs_Pt_InM_113(nullptr)
  , rap_vs_Pt_InM_114(nullptr)
  , rap_vs_Pt_InM_mixing_1(nullptr)
  , rap_vs_Pt_InM_mixing_2(nullptr)
  , rap_vs_Pt_InM_mixing_3(nullptr)
  , rap_vs_Pt_InM_mixing_4(nullptr)
  , rap_vs_Pt_InM_mixing_5(nullptr)
  , rap_vs_Pt_InM_mixing_6(nullptr)
  , rap_vs_Pt_InM_mixing_7(nullptr)
  , rap_vs_Pt_InM_mixing_8(nullptr)
  , rap_vs_Pt_InM_mixing_9(nullptr)
  , rap_vs_Pt_InM_mixing_10(nullptr)
  , rap_vs_Pt_InM_mixing_11(nullptr)
  , rap_vs_Pt_InM_mixing_12(nullptr)
  , rap_vs_Pt_InM_mixing_13(nullptr)
  , rap_vs_Pt_InM_mixing_14(nullptr)
  , rap_vs_Pt_InM_mixing_15(nullptr)
  , rap_vs_Pt_InM_mixing_16(nullptr)
  , rap_vs_Pt_InM_mixing_17(nullptr)
  , rap_vs_Pt_InM_mixing_18(nullptr)
  , rap_vs_Pt_InM_mixing_19(nullptr)
  , rap_vs_Pt_InM_mixing_20(nullptr)
  , rap_vs_Pt_InM_mixing_21(nullptr)
  , rap_vs_Pt_InM_mixing_22(nullptr)
  , rap_vs_Pt_InM_mixing_23(nullptr)
  , rap_vs_Pt_InM_mixing_24(nullptr)
  , rap_vs_Pt_InM_mixing_25(nullptr)
  , rap_vs_Pt_InM_mixing_26(nullptr)
  , rap_vs_Pt_InM_mixing_27(nullptr)
  , rap_vs_Pt_InM_mixing_28(nullptr)
  , rap_vs_Pt_InM_mixing_29(nullptr)
  , rap_vs_Pt_InM_mixing_30(nullptr)
  , rap_vs_Pt_InM_mixing_31(nullptr)
  , rap_vs_Pt_InM_mixing_32(nullptr)
  , rap_vs_Pt_InM_mixing_33(nullptr)
  , rap_vs_Pt_InM_mixing_34(nullptr)
  , rap_vs_Pt_InM_mixing_35(nullptr)
  , rap_vs_Pt_InM_mixing_36(nullptr)
  , rap_vs_Pt_InM_mixing_37(nullptr)
  , rap_vs_Pt_InM_mixing_38(nullptr)
  , rap_vs_Pt_InM_mixing_39(nullptr)
  , rap_vs_Pt_InM_mixing_40(nullptr)
  , rap_vs_Pt_InM_mixing_41(nullptr)
  , rap_vs_Pt_InM_mixing_42(nullptr)
  , rap_vs_Pt_InM_mixing_43(nullptr)
  , rap_vs_Pt_InM_mixing_44(nullptr)
  , rap_vs_Pt_InM_mixing_45(nullptr)
  , rap_vs_Pt_InM_mixing_46(nullptr)
  , rap_vs_Pt_InM_mixing_47(nullptr)
  , rap_vs_Pt_InM_mixing_48(nullptr)
  , rap_vs_Pt_InM_mixing_49(nullptr)
  , rap_vs_Pt_InM_mixing_50(nullptr)
  , rap_vs_Pt_InM_mixing_51(nullptr)
  , rap_vs_Pt_InM_mixing_52(nullptr)
  , rap_vs_Pt_InM_mixing_53(nullptr)
  , rap_vs_Pt_InM_mixing_54(nullptr)
  , rap_vs_Pt_InM_mixing_55(nullptr)
  , rap_vs_Pt_InM_mixing_56(nullptr)
  , rap_vs_Pt_InM_mixing_57(nullptr)
  , rap_vs_Pt_InM_mixing_58(nullptr)
  , rap_vs_Pt_InM_mixing_59(nullptr)
  , rap_vs_Pt_InM_mixing_60(nullptr)
  , rap_vs_Pt_InM_mixing_61(nullptr)
  , rap_vs_Pt_InM_mixing_62(nullptr)
  , rap_vs_Pt_InM_mixing_63(nullptr)
  , rap_vs_Pt_InM_mixing_64(nullptr)
  , rap_vs_Pt_InM_mixing_65(nullptr)
  , rap_vs_Pt_InM_mixing_66(nullptr)
  , rap_vs_Pt_InM_mixing_67(nullptr)
  , rap_vs_Pt_InM_mixing_68(nullptr)
  , rap_vs_Pt_InM_mixing_69(nullptr)
  , rap_vs_Pt_InM_mixing_70(nullptr)
  , rap_vs_Pt_InM_mixing_71(nullptr)
  , rap_vs_Pt_InM_mixing_72(nullptr)
  , rap_vs_Pt_InM_mixing_73(nullptr)
  , rap_vs_Pt_InM_mixing_74(nullptr)
  , rap_vs_Pt_InM_mixing_75(nullptr)
  , rap_vs_Pt_InM_mixing_76(nullptr)
  , rap_vs_Pt_InM_mixing_81(nullptr)
  , rap_vs_Pt_InM_mixing_82(nullptr)
  , rap_vs_Pt_InM_mixing_83(nullptr)
  , rap_vs_Pt_InM_mixing_84(nullptr)
  , rap_vs_Pt_InM_mixing_85(nullptr)
  , rap_vs_Pt_InM_mixing_86(nullptr)
  , rap_vs_Pt_InM_mixing_87(nullptr)
  , rap_vs_Pt_InM_mixing_88(nullptr)
  , rap_vs_Pt_InM_mixing_89(nullptr)
  , rap_vs_Pt_InM_mixing_90(nullptr)
  , rap_vs_Pt_InM_mixing_91(nullptr)
  , rap_vs_Pt_InM_mixing_92(nullptr)
  , rap_vs_Pt_InM_mixing_101(nullptr)
  , rap_vs_Pt_InM_mixing_102(nullptr)
  , rap_vs_Pt_InM_mixing_103(nullptr)
  , rap_vs_Pt_InM_mixing_104(nullptr)
  , rap_vs_Pt_InM_mixing_105(nullptr)
  , rap_vs_Pt_InM_mixing_106(nullptr)
  , rap_vs_Pt_InM_mixing_107(nullptr)
  , rap_vs_Pt_InM_mixing_108(nullptr)
  , rap_vs_Pt_InM_mixing_111(nullptr)
  , rap_vs_Pt_InM_mixing_112(nullptr)
  , rap_vs_Pt_InM_mixing_113(nullptr)
  , rap_vs_Pt_InM_mixing_114(nullptr)
  , fHistoList_rap_vs_pt_InM_all()
  , rap_vs_Pt_InM_all_1(nullptr)
  , rap_vs_Pt_InM_all_2(nullptr)
  , rap_vs_Pt_InM_all_3(nullptr)
  , rap_vs_Pt_InM_all_4(nullptr)
  , rap_vs_Pt_InM_all_5(nullptr)
  , rap_vs_Pt_InM_all_6(nullptr)
  , rap_vs_Pt_InM_all_7(nullptr)
  , rap_vs_Pt_InM_all_8(nullptr)
  , rap_vs_Pt_InM_all_9(nullptr)
  , rap_vs_Pt_InM_all_10(nullptr)
  , rap_vs_Pt_InM_all_11(nullptr)
  , rap_vs_Pt_InM_all_12(nullptr)
  , rap_vs_Pt_InM_all_13(nullptr)
  , rap_vs_Pt_InM_all_14(nullptr)
  , rap_vs_Pt_InM_all_15(nullptr)
  , rap_vs_Pt_InM_all_16(nullptr)
  , rap_vs_Pt_InM_all_17(nullptr)
  , rap_vs_Pt_InM_all_18(nullptr)
  , rap_vs_Pt_InM_all_19(nullptr)
  , rap_vs_Pt_InM_all_20(nullptr)
  , rap_vs_Pt_InM_all_21(nullptr)
  , rap_vs_Pt_InM_all_22(nullptr)
  , rap_vs_Pt_InM_all_23(nullptr)
  , rap_vs_Pt_InM_all_24(nullptr)
  , rap_vs_Pt_InM_all_25(nullptr)
  , rap_vs_Pt_InM_all_26(nullptr)
  , rap_vs_Pt_InM_all_27(nullptr)
  , rap_vs_Pt_InM_all_28(nullptr)
  , rap_vs_Pt_InM_all_29(nullptr)
  , rap_vs_Pt_InM_all_30(nullptr)
  , rap_vs_Pt_InM_all_31(nullptr)
  , rap_vs_Pt_InM_all_32(nullptr)
  , rap_vs_Pt_InM_all_33(nullptr)
  , rap_vs_Pt_InM_all_34(nullptr)
  , rap_vs_Pt_InM_all_35(nullptr)
  , rap_vs_Pt_InM_all_36(nullptr)
  , rap_vs_Pt_InM_all_37(nullptr)
  , rap_vs_Pt_InM_all_38(nullptr)
  , rap_vs_Pt_InM_all_39(nullptr)
  , rap_vs_Pt_InM_all_40(nullptr)
  , rap_vs_Pt_InM_all_41(nullptr)
  , rap_vs_Pt_InM_all_42(nullptr)
  , rap_vs_Pt_InM_all_43(nullptr)
  , rap_vs_Pt_InM_all_44(nullptr)
  , rap_vs_Pt_InM_all_45(nullptr)
  , rap_vs_Pt_InM_all_46(nullptr)
  , rap_vs_Pt_InM_all_47(nullptr)
  , rap_vs_Pt_InM_all_48(nullptr)
  , rap_vs_Pt_InM_all_49(nullptr)
  , rap_vs_Pt_InM_all_50(nullptr)
  , rap_vs_Pt_InM_all_51(nullptr)
  , rap_vs_Pt_InM_all_52(nullptr)
  , rap_vs_Pt_InM_all_53(nullptr)
  , rap_vs_Pt_InM_all_54(nullptr)
  , rap_vs_Pt_InM_all_55(nullptr)
  , rap_vs_Pt_InM_all_56(nullptr)
  , rap_vs_Pt_InM_all_57(nullptr)
  , rap_vs_Pt_InM_all_58(nullptr)
  , rap_vs_Pt_InM_all_59(nullptr)
  , rap_vs_Pt_InM_all_60(nullptr)
  , rap_vs_Pt_InM_all_61(nullptr)
  , rap_vs_Pt_InM_all_62(nullptr)
  , rap_vs_Pt_InM_all_63(nullptr)
  , rap_vs_Pt_InM_all_64(nullptr)
  , rap_vs_Pt_InM_all_65(nullptr)
  , rap_vs_Pt_InM_all_66(nullptr)
  , rap_vs_Pt_InM_all_67(nullptr)
  , rap_vs_Pt_InM_all_68(nullptr)
  , rap_vs_Pt_InM_all_69(nullptr)
  , rap_vs_Pt_InM_all_70(nullptr)
  , rap_vs_Pt_InM_all_71(nullptr)
  , rap_vs_Pt_InM_all_72(nullptr)
  , rap_vs_Pt_InM_all_73(nullptr)
  , rap_vs_Pt_InM_all_74(nullptr)
  , rap_vs_Pt_InM_all_75(nullptr)
  , rap_vs_Pt_InM_all_76(nullptr)
  , rap_vs_Pt_InM_all_81(nullptr)
  , rap_vs_Pt_InM_all_82(nullptr)
  , rap_vs_Pt_InM_all_83(nullptr)
  , rap_vs_Pt_InM_all_84(nullptr)
  , rap_vs_Pt_InM_all_85(nullptr)
  , rap_vs_Pt_InM_all_86(nullptr)
  , rap_vs_Pt_InM_all_87(nullptr)
  , rap_vs_Pt_InM_all_88(nullptr)
  , rap_vs_Pt_InM_all_89(nullptr)
  , rap_vs_Pt_InM_all_90(nullptr)
  , rap_vs_Pt_InM_all_91(nullptr)
  , rap_vs_Pt_InM_all_92(nullptr)
  , rap_vs_Pt_InM_all_101(nullptr)
  , rap_vs_Pt_InM_all_102(nullptr)
  , rap_vs_Pt_InM_all_103(nullptr)
  , rap_vs_Pt_InM_all_104(nullptr)
  , rap_vs_Pt_InM_all_105(nullptr)
  , rap_vs_Pt_InM_all_106(nullptr)
  , rap_vs_Pt_InM_all_107(nullptr)
  , rap_vs_Pt_InM_all_108(nullptr)
  , rap_vs_Pt_InM_all_111(nullptr)
  , rap_vs_Pt_InM_all_112(nullptr)
  , rap_vs_Pt_InM_all_113(nullptr)
  , rap_vs_Pt_InM_all_114(nullptr)
  , rap_vs_Pt_InM_all_mixing_1(nullptr)
  , rap_vs_Pt_InM_all_mixing_2(nullptr)
  , rap_vs_Pt_InM_all_mixing_3(nullptr)
  , rap_vs_Pt_InM_all_mixing_4(nullptr)
  , rap_vs_Pt_InM_all_mixing_5(nullptr)
  , rap_vs_Pt_InM_all_mixing_6(nullptr)
  , rap_vs_Pt_InM_all_mixing_7(nullptr)
  , rap_vs_Pt_InM_all_mixing_8(nullptr)
  , rap_vs_Pt_InM_all_mixing_9(nullptr)
  , rap_vs_Pt_InM_all_mixing_10(nullptr)
  , rap_vs_Pt_InM_all_mixing_11(nullptr)
  , rap_vs_Pt_InM_all_mixing_12(nullptr)
  , rap_vs_Pt_InM_all_mixing_13(nullptr)
  , rap_vs_Pt_InM_all_mixing_14(nullptr)
  , rap_vs_Pt_InM_all_mixing_15(nullptr)
  , rap_vs_Pt_InM_all_mixing_16(nullptr)
  , rap_vs_Pt_InM_all_mixing_17(nullptr)
  , rap_vs_Pt_InM_all_mixing_18(nullptr)
  , rap_vs_Pt_InM_all_mixing_19(nullptr)
  , rap_vs_Pt_InM_all_mixing_20(nullptr)
  , rap_vs_Pt_InM_all_mixing_21(nullptr)
  , rap_vs_Pt_InM_all_mixing_22(nullptr)
  , rap_vs_Pt_InM_all_mixing_23(nullptr)
  , rap_vs_Pt_InM_all_mixing_24(nullptr)
  , rap_vs_Pt_InM_all_mixing_25(nullptr)
  , rap_vs_Pt_InM_all_mixing_26(nullptr)
  , rap_vs_Pt_InM_all_mixing_27(nullptr)
  , rap_vs_Pt_InM_all_mixing_28(nullptr)
  , rap_vs_Pt_InM_all_mixing_29(nullptr)
  , rap_vs_Pt_InM_all_mixing_30(nullptr)
  , rap_vs_Pt_InM_all_mixing_31(nullptr)
  , rap_vs_Pt_InM_all_mixing_32(nullptr)
  , rap_vs_Pt_InM_all_mixing_33(nullptr)
  , rap_vs_Pt_InM_all_mixing_34(nullptr)
  , rap_vs_Pt_InM_all_mixing_35(nullptr)
  , rap_vs_Pt_InM_all_mixing_36(nullptr)
  , rap_vs_Pt_InM_all_mixing_37(nullptr)
  , rap_vs_Pt_InM_all_mixing_38(nullptr)
  , rap_vs_Pt_InM_all_mixing_39(nullptr)
  , rap_vs_Pt_InM_all_mixing_40(nullptr)
  , rap_vs_Pt_InM_all_mixing_41(nullptr)
  , rap_vs_Pt_InM_all_mixing_42(nullptr)
  , rap_vs_Pt_InM_all_mixing_43(nullptr)
  , rap_vs_Pt_InM_all_mixing_44(nullptr)
  , rap_vs_Pt_InM_all_mixing_45(nullptr)
  , rap_vs_Pt_InM_all_mixing_46(nullptr)
  , rap_vs_Pt_InM_all_mixing_47(nullptr)
  , rap_vs_Pt_InM_all_mixing_48(nullptr)
  , rap_vs_Pt_InM_all_mixing_49(nullptr)
  , rap_vs_Pt_InM_all_mixing_50(nullptr)
  , rap_vs_Pt_InM_all_mixing_51(nullptr)
  , rap_vs_Pt_InM_all_mixing_52(nullptr)
  , rap_vs_Pt_InM_all_mixing_53(nullptr)
  , rap_vs_Pt_InM_all_mixing_54(nullptr)
  , rap_vs_Pt_InM_all_mixing_55(nullptr)
  , rap_vs_Pt_InM_all_mixing_56(nullptr)
  , rap_vs_Pt_InM_all_mixing_57(nullptr)
  , rap_vs_Pt_InM_all_mixing_58(nullptr)
  , rap_vs_Pt_InM_all_mixing_59(nullptr)
  , rap_vs_Pt_InM_all_mixing_60(nullptr)
  , rap_vs_Pt_InM_all_mixing_61(nullptr)
  , rap_vs_Pt_InM_all_mixing_62(nullptr)
  , rap_vs_Pt_InM_all_mixing_63(nullptr)
  , rap_vs_Pt_InM_all_mixing_64(nullptr)
  , rap_vs_Pt_InM_all_mixing_65(nullptr)
  , rap_vs_Pt_InM_all_mixing_66(nullptr)
  , rap_vs_Pt_InM_all_mixing_67(nullptr)
  , rap_vs_Pt_InM_all_mixing_68(nullptr)
  , rap_vs_Pt_InM_all_mixing_69(nullptr)
  , rap_vs_Pt_InM_all_mixing_70(nullptr)
  , rap_vs_Pt_InM_all_mixing_71(nullptr)
  , rap_vs_Pt_InM_all_mixing_72(nullptr)
  , rap_vs_Pt_InM_all_mixing_73(nullptr)
  , rap_vs_Pt_InM_all_mixing_74(nullptr)
  , rap_vs_Pt_InM_all_mixing_75(nullptr)
  , rap_vs_Pt_InM_all_mixing_76(nullptr)
  , rap_vs_Pt_InM_all_mixing_81(nullptr)
  , rap_vs_Pt_InM_all_mixing_82(nullptr)
  , rap_vs_Pt_InM_all_mixing_83(nullptr)
  , rap_vs_Pt_InM_all_mixing_84(nullptr)
  , rap_vs_Pt_InM_all_mixing_85(nullptr)
  , rap_vs_Pt_InM_all_mixing_86(nullptr)
  , rap_vs_Pt_InM_all_mixing_87(nullptr)
  , rap_vs_Pt_InM_all_mixing_88(nullptr)
  , rap_vs_Pt_InM_all_mixing_89(nullptr)
  , rap_vs_Pt_InM_all_mixing_90(nullptr)
  , rap_vs_Pt_InM_all_mixing_91(nullptr)
  , rap_vs_Pt_InM_all_mixing_92(nullptr)
  , rap_vs_Pt_InM_all_mixing_101(nullptr)
  , rap_vs_Pt_InM_all_mixing_102(nullptr)
  , rap_vs_Pt_InM_all_mixing_103(nullptr)
  , rap_vs_Pt_InM_all_mixing_104(nullptr)
  , rap_vs_Pt_InM_all_mixing_105(nullptr)
  , rap_vs_Pt_InM_all_mixing_106(nullptr)
  , rap_vs_Pt_InM_all_mixing_107(nullptr)
  , rap_vs_Pt_InM_all_mixing_108(nullptr)
  , rap_vs_Pt_InM_all_mixing_111(nullptr)
  , rap_vs_Pt_InM_all_mixing_112(nullptr)
  , rap_vs_Pt_InM_all_mixing_113(nullptr)
  , rap_vs_Pt_InM_all_mixing_114(nullptr)
  , fHistoList_pt_onetwo()
  , Pi0_pt_est_onetwo_Both(nullptr)
  , pt_onetwo_1(nullptr)
  , pt_onetwo_2(nullptr)
  , pt_onetwo_3(nullptr)
  , pt_onetwo_4(nullptr)
  , pt_onetwo_5(nullptr)
  , pt_onetwo_6(nullptr)
  , pt_onetwo_7(nullptr)
  , pt_onetwo_8(nullptr)
  , pt_onetwo_9(nullptr)
  , pt_onetwo_10(nullptr)
  , pt_onetwo_11(nullptr)
  , pt_onetwo_12(nullptr)
  , pt_onetwo_13(nullptr)
  , pt_onetwo_14(nullptr)
  , pt_onetwo_15(nullptr)
  , pt_onetwo_16(nullptr)
  , pt_onetwo_17(nullptr)
  , pt_onetwo_18(nullptr)
  , pt_onetwo_19(nullptr)
  , pt_onetwo_20(nullptr)
  , pt_onetwo_mixing_1(nullptr)
  , pt_onetwo_mixing_2(nullptr)
  , pt_onetwo_mixing_3(nullptr)
  , pt_onetwo_mixing_4(nullptr)
  , pt_onetwo_mixing_5(nullptr)
  , pt_onetwo_mixing_6(nullptr)
  , pt_onetwo_mixing_7(nullptr)
  , pt_onetwo_mixing_8(nullptr)
  , pt_onetwo_mixing_9(nullptr)
  , pt_onetwo_mixing_10(nullptr)
  , pt_onetwo_mixing_11(nullptr)
  , pt_onetwo_mixing_12(nullptr)
  , pt_onetwo_mixing_13(nullptr)
  , pt_onetwo_mixing_14(nullptr)
  , pt_onetwo_mixing_15(nullptr)
  , pt_onetwo_mixing_16(nullptr)
  , pt_onetwo_mixing_17(nullptr)
  , pt_onetwo_mixing_18(nullptr)
  , pt_onetwo_mixing_19(nullptr)
  , pt_onetwo_mixing_20(nullptr)
  , fHistoList_pt_all()
  , Pi0_pt_est_all_Both(nullptr)
  , pt_all_1(nullptr)
  , pt_all_2(nullptr)
  , pt_all_3(nullptr)
  , pt_all_4(nullptr)
  , pt_all_5(nullptr)
  , pt_all_6(nullptr)
  , pt_all_7(nullptr)
  , pt_all_8(nullptr)
  , pt_all_9(nullptr)
  , pt_all_10(nullptr)
  , pt_all_11(nullptr)
  , pt_all_12(nullptr)
  , pt_all_13(nullptr)
  , pt_all_14(nullptr)
  , pt_all_15(nullptr)
  , pt_all_16(nullptr)
  , pt_all_17(nullptr)
  , pt_all_18(nullptr)
  , pt_all_19(nullptr)
  , pt_all_20(nullptr)
  , pt_all_mixing_1(nullptr)
  , pt_all_mixing_2(nullptr)
  , pt_all_mixing_3(nullptr)
  , pt_all_mixing_4(nullptr)
  , pt_all_mixing_5(nullptr)
  , pt_all_mixing_6(nullptr)
  , pt_all_mixing_7(nullptr)
  , pt_all_mixing_8(nullptr)
  , pt_all_mixing_9(nullptr)
  , pt_all_mixing_10(nullptr)
  , pt_all_mixing_11(nullptr)
  , pt_all_mixing_12(nullptr)
  , pt_all_mixing_13(nullptr)
  , pt_all_mixing_14(nullptr)
  , pt_all_mixing_15(nullptr)
  , pt_all_mixing_16(nullptr)
  , pt_all_mixing_17(nullptr)
  , pt_all_mixing_18(nullptr)
  , pt_all_mixing_19(nullptr)
  , pt_all_mixing_20(nullptr)
  , fHistoList_bg_InM_all_Target()
  , fHistoList_bg_InM_zero_Target()
  , fHistoList_bg_InM_one_Target()
  , fHistoList_bg_InM_two_Target()
  , fHistoList_bg_InM_onetwo_Target()
  , fHistoList_bg_InM_all_Outside()
  , fHistoList_bg_InM_zero_Outside()
  , fHistoList_bg_InM_one_Outside()
  , fHistoList_bg_InM_two_Outside()
  , fHistoList_bg_InM_onetwo_Outside()
  , fHistoList_bg_InM_all_Both()
  , BG1_InM_all_Both(nullptr)
  , BG2_InM_all_Both(nullptr)
  , BG3_InM_all_Both(nullptr)
  , BG4_InM_all_Both(nullptr)
  , BG5_InM_all_Both(nullptr)
  , BG6_InM_all_Both(nullptr)
  , BG7_InM_all_Both(nullptr)
  , BG8_InM_all_Both(nullptr)
  , BG9_InM_all_Both(nullptr)
  , BG10_InM_all_Both(nullptr)
  , PdgCase8_InM_all_Both(nullptr)
  , PdgCase8mothers_InM_all_Both(nullptr)
  , sameMIDcase8_InM_all_Both(nullptr)
  , sameGRIDcase8_InM_all_Both(nullptr)
  , Case1ZYPos_InM_all_Both(nullptr)
  , sameMIDcase8_mothedPDG_InM_all_Both(nullptr)
  , PdgCase8NonEComeFromTarget_InM_all_Both(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_all_Both(nullptr)
  , PdgCase8motherNonE_InM_all_Both(nullptr)
  , Case8ElFromDalitz_InM_all_Both(nullptr)
  , Case8NonElFrom_pn_InM_all_Both(nullptr)
  , Case8NonElFrom_eta_InM_all_Both(nullptr)
  , Case8NonElFrom_kaon_InM_all_Both(nullptr)
  , sameMIDcase8NonEPdg_InM_all_Both(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_all_Both(nullptr)
  , sameMIDcase8NonEMotherIM_InM_all_Both(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_all_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_all_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_all_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_all_Both(nullptr)
  , fHistoList_bg_InM_zero_Both()
  , BG1_InM_zero_Both(nullptr)
  , BG2_InM_zero_Both(nullptr)
  , BG3_InM_zero_Both(nullptr)
  , BG4_InM_zero_Both(nullptr)
  , BG5_InM_zero_Both(nullptr)
  , BG6_InM_zero_Both(nullptr)
  , BG7_InM_zero_Both(nullptr)
  , BG8_InM_zero_Both(nullptr)
  , BG9_InM_zero_Both(nullptr)
  , BG10_InM_zero_Both(nullptr)
  , PdgCase8_InM_zero_Both(nullptr)
  , PdgCase8mothers_InM_zero_Both(nullptr)
  , sameMIDcase8_InM_zero_Both(nullptr)
  , sameGRIDcase8_InM_zero_Both(nullptr)
  , Case1ZYPos_InM_zero_Both(nullptr)
  , sameMIDcase8_mothedPDG_InM_zero_Both(nullptr)
  , PdgCase8NonEComeFromTarget_InM_zero_Both(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_zero_Both(nullptr)
  , PdgCase8motherNonE_InM_zero_Both(nullptr)
  , Case8ElFromDalitz_InM_zero_Both(nullptr)
  , Case8NonElFrom_pn_InM_zero_Both(nullptr)
  , Case8NonElFrom_eta_InM_zero_Both(nullptr)
  , Case8NonElFrom_kaon_InM_zero_Both(nullptr)
  , sameMIDcase8NonEPdg_InM_zero_Both(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_zero_Both(nullptr)
  , sameMIDcase8NonEMotherIM_InM_zero_Both(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_zero_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_zero_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_zero_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_zero_Both(nullptr)
  , fHistoList_bg_InM_one_Both()
  , BG1_InM_one_Both(nullptr)
  , BG2_InM_one_Both(nullptr)
  , BG3_InM_one_Both(nullptr)
  , BG4_InM_one_Both(nullptr)
  , BG5_InM_one_Both(nullptr)
  , BG6_InM_one_Both(nullptr)
  , BG7_InM_one_Both(nullptr)
  , BG8_InM_one_Both(nullptr)
  , BG9_InM_one_Both(nullptr)
  , BG10_InM_one_Both(nullptr)
  , PdgCase8_InM_one_Both(nullptr)
  , PdgCase8mothers_InM_one_Both(nullptr)
  , sameMIDcase8_InM_one_Both(nullptr)
  , sameGRIDcase8_InM_one_Both(nullptr)
  , Case1ZYPos_InM_one_Both(nullptr)
  , sameMIDcase8_mothedPDG_InM_one_Both(nullptr)
  , PdgCase8NonEComeFromTarget_InM_one_Both(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_one_Both(nullptr)
  , PdgCase8motherNonE_InM_one_Both(nullptr)
  , Case8ElFromDalitz_InM_one_Both(nullptr)
  , Case8NonElFrom_pn_InM_one_Both(nullptr)
  , Case8NonElFrom_eta_InM_one_Both(nullptr)
  , Case8NonElFrom_kaon_InM_one_Both(nullptr)
  , sameMIDcase8NonEPdg_InM_one_Both(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_one_Both(nullptr)
  , sameMIDcase8NonEMotherIM_InM_one_Both(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_one_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_one_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_one_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_one_Both(nullptr)
  , fHistoList_bg_InM_two_Both()
  , BG1_InM_two_Both(nullptr)
  , BG2_InM_two_Both(nullptr)
  , BG3_InM_two_Both(nullptr)
  , BG4_InM_two_Both(nullptr)
  , BG5_InM_two_Both(nullptr)
  , BG6_InM_two_Both(nullptr)
  , BG7_InM_two_Both(nullptr)
  , BG8_InM_two_Both(nullptr)
  , BG9_InM_two_Both(nullptr)
  , BG10_InM_two_Both(nullptr)
  , PdgCase8_InM_two_Both(nullptr)
  , PdgCase8mothers_InM_two_Both(nullptr)
  , sameMIDcase8_InM_two_Both(nullptr)
  , sameGRIDcase8_InM_two_Both(nullptr)
  , Case1ZYPos_InM_two_Both(nullptr)
  , sameMIDcase8_mothedPDG_InM_two_Both(nullptr)
  , PdgCase8NonEComeFromTarget_InM_two_Both(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_two_Both(nullptr)
  , PdgCase8motherNonE_InM_two_Both(nullptr)
  , Case8ElFromDalitz_InM_two_Both(nullptr)
  , Case8NonElFrom_pn_InM_two_Both(nullptr)
  , Case8NonElFrom_eta_InM_two_Both(nullptr)
  , Case8NonElFrom_kaon_InM_two_Both(nullptr)
  , sameMIDcase8NonEPdg_InM_two_Both(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_two_Both(nullptr)
  , sameMIDcase8NonEMotherIM_InM_two_Both(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_two_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_two_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_two_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_two_Both(nullptr)
  , fHistoList_bg_InM_onetwo_Both()
  , BG1_InM_onetwo_Both(nullptr)
  , BG2_InM_onetwo_Both(nullptr)
  , BG3_InM_onetwo_Both(nullptr)
  , BG4_InM_onetwo_Both(nullptr)
  , BG5_InM_onetwo_Both(nullptr)
  , BG6_InM_onetwo_Both(nullptr)
  , BG7_InM_onetwo_Both(nullptr)
  , BG8_InM_onetwo_Both(nullptr)
  , BG9_InM_onetwo_Both(nullptr)
  , BG10_InM_onetwo_Both(nullptr)
  , PdgCase8_InM_onetwo_Both(nullptr)
  , PdgCase8mothers_InM_onetwo_Both(nullptr)
  , sameMIDcase8_InM_onetwo_Both(nullptr)
  , sameGRIDcase8_InM_onetwo_Both(nullptr)
  , Case1ZYPos_InM_onetwo_Both(nullptr)
  , sameMIDcase8_mothedPDG_InM_onetwo_Both(nullptr)
  , PdgCase8NonEComeFromTarget_InM_onetwo_Both(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both(nullptr)
  , PdgCase8motherNonE_InM_onetwo_Both(nullptr)
  , Case8ElFromDalitz_InM_onetwo_Both(nullptr)
  , Case8NonElFrom_pn_InM_onetwo_Both(nullptr)
  , Case8NonElFrom_eta_InM_onetwo_Both(nullptr)
  , Case8NonElFrom_kaon_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEPdg_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEMotherIM_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both(nullptr)
  , AnnTruePairs(nullptr)
  , AnnFalsePairs(nullptr)
  , AnnTruePairs_AfterCuts(nullptr)
  , AnnFalsePairs_AfterCuts(nullptr)
  , fHistoList_man_WAC()
  , Pi0InvMassReco_WAC(nullptr)
  , EMT_InvMass_WAC(nullptr)
  , Pi0_pt_vs_rap_WAC(nullptr)
  , Pi0_pt_vs_rap_est_WAC(nullptr)
  , DalitzPi0_WAC(nullptr)
  , PhotonsPi0_WAC(nullptr)
  , MultiplicityGamma_WAC(nullptr)
  , MultiplicityChargedParticles_WAC(nullptr)
  , fHistoList_bg_InM_WAC()
  , BG1_InM_WAC(nullptr)
  , BG2_InM_WAC(nullptr)
  , BG3_InM_WAC(nullptr)
  , BG4_InM_WAC(nullptr)
  , BG5_InM_WAC(nullptr)
  , BG6_InM_WAC(nullptr)
  , BG7_InM_WAC(nullptr)
  , BG8_InM_WAC(nullptr)
  , BG9_InM_WAC(nullptr)
  , BG10_InM_WAC(nullptr)
  , PdgCase8_InM_WAC(nullptr)
  , PdgCase8mothers_InM_WAC(nullptr)
  , sameMIDcase8_InM_WAC(nullptr)
  , sameGRIDcase8_InM_WAC(nullptr)
  , Case1ZYPos_InM_WAC(nullptr)
  , sameMIDcase8_mothedPDG_InM_WAC(nullptr)
  , PdgCase8NonEComeFromTarget_InM_WAC(nullptr)
  , PdgCase8NonE_NOT_FromTarget_InM_WAC(nullptr)
  , PdgCase8motherNonE_InM_WAC(nullptr)
  , Case8ElFromDalitz_InM_WAC(nullptr)
  , Case8NonElFrom_pn_InM_WAC(nullptr)
  , Case8NonElFrom_eta_InM_WAC(nullptr)
  , Case8NonElFrom_kaon_InM_WAC(nullptr)
  , sameMIDcase8NonEPdg_InM_WAC(nullptr)
  , sameMIDcase8NonEMotherPdg_InM_WAC(nullptr)
  , sameMIDcase8NonEMotherIM_InM_WAC(nullptr)
  , sameMIDcase8NonEPdgFromTarget_InM_WAC(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_InM_WAC(nullptr)
  , sameMIDcase8NonEComeFromTargetP_InM_WAC(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_InM_WAC(nullptr)
  , fHistoList_manual()
  , Chi2_for_Primary(nullptr)
  , Chi2_for_Secondary(nullptr)
{
}

CbmKresConversionManual::~CbmKresConversionManual() {}

void CbmKresConversionManual::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresConversionManual::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresConversionManual::Init", "No MCTrack array!"); }

  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) { LOG(fatal) << "CbmKresConversionManual::Init  No PrimaryVertex array!"; }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionManual::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresConversionManual::Init", "No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionManual::Init", "No StsTrackMatch array!"); }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) { Fatal("CbmKresConversionManual::Init", "No RichProjection array!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmKresConversionManual::Init", "No RichRing array!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionManual::Init", "No RichRingMatch array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmKresConversionManual::Init", "No RichHit array!"); }

  fArrayMvdHit = (TClonesArray*) ioman->GetObject("MvdHit");
  if (nullptr == fArrayMvdHit) { Fatal("CbmKresConversionManual::Init", "No MvdHit array!"); }

  fArrayStsHit = (TClonesArray*) ioman->GetObject("StsHit");
  if (nullptr == fArrayStsHit) { Fatal("CbmKresConversionManual::Init", "No StsHit array!"); }


  fTauFit = new CbmRichRingFitterEllipseTau();

  InitHistograms();

  fAnaBG = new CbmKresConversionBG();
  fAnaBG->Init();


  AnnTrain =
    0;  //  identificator for artificial neural network. Either it used of trained. not both together at the same time

  UseAnn = 1 - AnnTrain;

  if (AnnTrain == 1) {
    fTrain = new CbmKresTrainAnn();
    fTrain->Init();
    cout << endl;
    cout << "\t *** CbmKresTrainAnn                                  ==>  is "
            "activated"
         << endl;
  }

  if (UseAnn == 1) {
    fAnnSelection = new CbmKresSelectAnn();
    fAnnSelection->Init();
    cout << endl;
    cout << "\t *** CbmKresSelectAnn                                  ==>  is "
            "activated"
         << endl;
  }
}


void CbmKresConversionManual::Exec(int fEventNumMan, double OpeningAngleCut, double GammaInvMassCut, int RealPID)
{
  // cout << "CbmKresConversionManual, event No. " <<  fEventNumMan << endl;

  if (fPrimVertex != nullptr) { fKFVertex = CbmKFVertex(*fPrimVertex); }
  else {
    Fatal("CbmKresConversionManual::Exec", "No PrimaryVertex array here!");
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
  Gammas_MCIndex_all_Target.clear();
  Gammas_MCIndex_zero_Target.clear();
  Gammas_MCIndex_one_Target.clear();
  Gammas_MCIndex_two_Target.clear();
  Gammas_MCIndex_onetwo_Target.clear();
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
  Gammas_MCIndex_all_Outside.clear();
  Gammas_MCIndex_zero_Outside.clear();
  Gammas_MCIndex_one_Outside.clear();
  Gammas_MCIndex_two_Outside.clear();
  Gammas_MCIndex_onetwo_Outside.clear();
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
  Gammas_MCIndex_all_Both.clear();
  Gammas_MCIndex_zero_Both.clear();
  Gammas_MCIndex_one_Both.clear();
  Gammas_MCIndex_two_Both.clear();
  Gammas_MCIndex_onetwo_Both.clear();
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
  VMCIndex_minus_Outside.clear();
  VMCtracks_plus_Outside.clear();
  VStsTrack_plus_Outside.clear();
  VRings_plus_Outside.clear();
  VStsIndex_plus_Outside.clear();
  VRichRing_plus_Outside.clear();
  VMCIndex_plus_Outside.clear();

  VMCtracks_minus_Target.clear();
  VMomenta_minus_Target.clear();
  VStsTrack_minus_Target.clear();
  VRings_minus_Target.clear();
  VStsIndex_minus_Target.clear();
  VRichRing_minus_Target.clear();
  VMCIndex_minus_Target.clear();
  VMCtracks_plus_Target.clear();
  VStsTrack_plus_Target.clear();
  VMomenta_plus_Target.clear();
  VRings_plus_Target.clear();
  VStsIndex_plus_Target.clear();
  VRichRing_plus_Target.clear();
  VMCIndex_plus_Target.clear();

  //WAC - > WithAdditionalCuts
  Gamma_WAC.clear();
  Gammas_stsIndex_WAC.clear();
  Gammas_MCIndex_WAC.clear();
  Gammas_MC_WAC.clear();


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
      Pdg_vs_Distance->Fill(TMath::Abs(mcTrack->GetPdgCode()), CbmRichUtil::GetRingTrackDistance(i));
      if (TMath::Abs(mcTrack->GetPdgCode()) == 11)
        P_vs_Distance->Fill(mcTrack->GetP(), CbmRichUtil::GetRingTrackDistance(i));
    }


    // Doing fit with Fit To primary Vertex and calculate chi2 to primary vertex
    double chi2       = 0;
    TVector3 Momentum = CbmKresFunctions::FitToVertexAndGetChi(stsTrack, fKFVertex.GetRefX(), fKFVertex.GetRefY(),
                                                               fKFVertex.GetRefZ(), chi2);
    const FairTrackParam* track_par = stsTrack->GetParamFirst();
    double charge                   = track_par->GetQp();


    // Doing refit of momenta with electron assumption
    /*CbmL1PFFitter fPFFitter_electron;
		vector<CbmStsTrack> stsTracks_electron;
		stsTracks_electron.resize(1);
		stsTracks_electron[0] = *stsTrack;
		vector<L1FieldRegion> vField_electron;
		vector<float> chiPrim_electron;
		vector<int> pidHypo_electron;
		pidHypo_electron.push_back(11);
		fPFFitter_electron.Fit(stsTracks_electron, pidHypo_electron);
		fPFFitter_electron.GetChiToVertex(stsTracks_electron, vField_electron, chiPrim_electron, fKFVertex, 3e6);

		TVector3 Momentum;
		const FairTrackParam* vtxTrack_electron = stsTracks_electron[0].GetParamFirst();
		vtxTrack_electron->Momentum(Momentum);
		double charge = vtxTrack_electron->GetQp()*Momentum.Mag();
		float chi2 = chiPrim_electron[0];*/


    if (chi2 != chi2) continue;
    if (chi2 == 0) continue;

    if (mcTrack->GetStartZ() < 3.) Chi2_for_Primary->Fill(chi2);
    if (mcTrack->GetStartZ() > 3.) Chi2_for_Secondary->Fill(chi2);

    if (chi2 > 3) { SaveOutsideTracks(mcTrack, stsTrack, charge, stsInd, richInd, stsMcTrackId, Ring); }
    if (chi2 > 3) continue;

    SaveTargetTracks(mcTrack, stsTrack, Momentum, charge, stsInd, richInd, stsMcTrackId, Ring);
  }


  FindGammasTarget(fEventNumMan, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Target,
                   VMCtracks_plus_Target, VStsTrack_minus_Target, VStsTrack_plus_Target, VMomenta_minus_Target,
                   VMomenta_plus_Target, VRings_minus_Target, VRings_plus_Target, VStsIndex_minus_Target,
                   VStsIndex_plus_Target, VRichRing_minus_Target, VRichRing_plus_Target, VMCIndex_minus_Target,
                   VMCIndex_plus_Target);

  FindGammasOutside(fEventNumMan, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Outside,
                    VMCtracks_plus_Outside, VStsTrack_minus_Outside, VStsTrack_plus_Outside, VRings_minus_Outside,
                    VRings_plus_Outside, VStsIndex_minus_Outside, VStsIndex_plus_Outside, VRichRing_minus_Outside,
                    VRichRing_plus_Outside, VMCIndex_minus_Outside, VMCIndex_plus_Outside);

  FindGammasBoth();

  FindGammasBothWithAdditionalCuts();


  FindPi0("All", "Target", Gammas_all_Target, Gammas_stsIndex_all_Target, Gammas_MCIndex_all_Target,
          Gammas_MC_all_Target, Pi0InvMassReco_all_Target, Pi0_pt_vs_rap_all_Target, Pi0_pt_vs_rap_est_all_Target,
          MultiplicityGamma_all_Target, MultiplicityChargedParticles_all_Target, fHistoList_bg_InM_all_Target,
          DalitzPi0_all_Target, PhotonsPi0_all_Target);
  FindPi0("Zero", "Target", Gammas_zero_Target, Gammas_stsIndex_zero_Target, Gammas_MCIndex_zero_Target,
          Gammas_MC_zero_Target, Pi0InvMassReco_zero_Target, Pi0_pt_vs_rap_zero_Target, Pi0_pt_vs_rap_est_zero_Target,
          MultiplicityGamma_zero_Target, MultiplicityChargedParticles_zero_Target, fHistoList_bg_InM_zero_Target,
          DalitzPi0_zero_Target, PhotonsPi0_zero_Target);
  FindPi0("One", "Target", Gammas_one_Target, Gammas_stsIndex_one_Target, Gammas_MCIndex_one_Target,
          Gammas_MC_one_Target, Pi0InvMassReco_one_Target, Pi0_pt_vs_rap_one_Target, Pi0_pt_vs_rap_est_one_Target,
          MultiplicityGamma_one_Target, MultiplicityChargedParticles_one_Target, fHistoList_bg_InM_one_Target,
          DalitzPi0_one_Target, PhotonsPi0_one_Target);
  FindPi0("Two", "Target", Gammas_two_Target, Gammas_stsIndex_two_Target, Gammas_MCIndex_two_Target,
          Gammas_MC_two_Target, Pi0InvMassReco_two_Target, Pi0_pt_vs_rap_two_Target, Pi0_pt_vs_rap_est_two_Target,
          MultiplicityGamma_two_Target, MultiplicityChargedParticles_two_Target, fHistoList_bg_InM_two_Target,
          DalitzPi0_two_Target, PhotonsPi0_two_Target);
  FindPi0("OneTwo", "Target", Gammas_onetwo_Target, Gammas_stsIndex_onetwo_Target, Gammas_MCIndex_onetwo_Target,
          Gammas_MC_onetwo_Target, Pi0InvMassReco_onetwo_Target, Pi0_pt_vs_rap_onetwo_Target,
          Pi0_pt_vs_rap_est_onetwo_Target, MultiplicityGamma_onetwo_Target, MultiplicityChargedParticles_onetwo_Target,
          fHistoList_bg_InM_onetwo_Target, DalitzPi0_onetwo_Target, PhotonsPi0_onetwo_Target);

  FindPi0("All", "Outside", Gammas_all_Outside, Gammas_stsIndex_all_Outside, Gammas_MCIndex_all_Outside,
          Gammas_MC_all_Outside, Pi0InvMassReco_all_Outside, Pi0_pt_vs_rap_all_Outside, Pi0_pt_vs_rap_est_all_Outside,
          MultiplicityGamma_all_Outside, MultiplicityChargedParticles_all_Outside, fHistoList_bg_InM_all_Outside,
          DalitzPi0_all_Outside, PhotonsPi0_all_Outside);
  FindPi0("Zero", "Outside", Gammas_zero_Outside, Gammas_stsIndex_zero_Outside, Gammas_MCIndex_zero_Outside,
          Gammas_MC_zero_Outside, Pi0InvMassReco_zero_Outside, Pi0_pt_vs_rap_zero_Outside,
          Pi0_pt_vs_rap_est_zero_Outside, MultiplicityGamma_zero_Outside, MultiplicityChargedParticles_zero_Outside,
          fHistoList_bg_InM_zero_Outside, DalitzPi0_zero_Outside, PhotonsPi0_zero_Outside);
  FindPi0("One", "Outside", Gammas_one_Outside, Gammas_stsIndex_one_Outside, Gammas_MCIndex_one_Outside,
          Gammas_MC_one_Outside, Pi0InvMassReco_one_Outside, Pi0_pt_vs_rap_one_Outside, Pi0_pt_vs_rap_est_one_Outside,
          MultiplicityGamma_one_Outside, MultiplicityChargedParticles_one_Outside, fHistoList_bg_InM_one_Outside,
          DalitzPi0_one_Outside, PhotonsPi0_one_Outside);
  FindPi0("Two", "Outside", Gammas_two_Outside, Gammas_stsIndex_two_Outside, Gammas_MCIndex_two_Outside,
          Gammas_MC_two_Outside, Pi0InvMassReco_two_Outside, Pi0_pt_vs_rap_two_Outside, Pi0_pt_vs_rap_est_two_Outside,
          MultiplicityGamma_two_Outside, MultiplicityChargedParticles_two_Outside, fHistoList_bg_InM_two_Outside,
          DalitzPi0_two_Outside, PhotonsPi0_two_Outside);
  FindPi0("OneTwo", "Outside", Gammas_onetwo_Outside, Gammas_stsIndex_onetwo_Outside, Gammas_MCIndex_onetwo_Outside,
          Gammas_MC_onetwo_Outside, Pi0InvMassReco_onetwo_Outside, Pi0_pt_vs_rap_onetwo_Outside,
          Pi0_pt_vs_rap_est_onetwo_Outside, MultiplicityGamma_onetwo_Outside,
          MultiplicityChargedParticles_onetwo_Outside, fHistoList_bg_InM_onetwo_Outside, DalitzPi0_onetwo_Outside,
          PhotonsPi0_onetwo_Outside);

  FindPi0("All", "Both", Gammas_all_Both, Gammas_stsIndex_all_Both, Gammas_MCIndex_all_Both, Gammas_MC_all_Both,
          Pi0InvMassReco_all_Both, Pi0_pt_vs_rap_all_Both, Pi0_pt_vs_rap_est_all_Both, MultiplicityGamma_all_Both,
          MultiplicityChargedParticles_all_Both, fHistoList_bg_InM_all_Both, DalitzPi0_all_Both, PhotonsPi0_all_Both);
  FindPi0("Zero", "Both", Gammas_zero_Both, Gammas_stsIndex_zero_Both, Gammas_MCIndex_zero_Both, Gammas_MC_zero_Both,
          Pi0InvMassReco_zero_Both, Pi0_pt_vs_rap_zero_Both, Pi0_pt_vs_rap_est_zero_Both, MultiplicityGamma_zero_Both,
          MultiplicityChargedParticles_zero_Both, fHistoList_bg_InM_zero_Both, DalitzPi0_zero_Both,
          PhotonsPi0_zero_Both);
  FindPi0("One", "Both", Gammas_one_Both, Gammas_stsIndex_one_Both, Gammas_MCIndex_one_Both, Gammas_MC_one_Both,
          Pi0InvMassReco_one_Both, Pi0_pt_vs_rap_one_Both, Pi0_pt_vs_rap_est_one_Both, MultiplicityGamma_one_Both,
          MultiplicityChargedParticles_one_Both, fHistoList_bg_InM_one_Both, DalitzPi0_one_Both, PhotonsPi0_one_Both);
  FindPi0("Two", "Both", Gammas_two_Both, Gammas_stsIndex_two_Both, Gammas_MCIndex_two_Both, Gammas_MC_two_Both,
          Pi0InvMassReco_two_Both, Pi0_pt_vs_rap_two_Both, Pi0_pt_vs_rap_est_two_Both, MultiplicityGamma_two_Both,
          MultiplicityChargedParticles_two_Both, fHistoList_bg_InM_two_Both, DalitzPi0_two_Both, PhotonsPi0_two_Both);
  FindPi0("OneTwo", "Both", Gammas_onetwo_Both, Gammas_stsIndex_onetwo_Both, Gammas_MCIndex_onetwo_Both,
          Gammas_MC_onetwo_Both, Pi0InvMassReco_onetwo_Both, Pi0_pt_vs_rap_onetwo_Both, Pi0_pt_vs_rap_est_onetwo_Both,
          MultiplicityGamma_onetwo_Both, MultiplicityChargedParticles_onetwo_Both, fHistoList_bg_InM_onetwo_Both,
          DalitzPi0_onetwo_Both, PhotonsPi0_onetwo_Both);


  FindPi0("All", "Both", Gamma_WAC, Gammas_stsIndex_WAC, Gammas_MCIndex_WAC, Gammas_MC_WAC, Pi0InvMassReco_WAC,
          Pi0_pt_vs_rap_WAC, Pi0_pt_vs_rap_est_WAC, MultiplicityGamma_WAC, MultiplicityChargedParticles_WAC,
          fHistoList_bg_InM_WAC, DalitzPi0_WAC, PhotonsPi0_WAC);


  int numformix = 500;
  if (fEventNumMan % numformix == 0) {
    EMT_man_Event_Both.insert(EMT_man_Event_Both.end(), EMT_man_Event_Outside.begin(), EMT_man_Event_Outside.end());
    EMT_man_Event_Both.insert(EMT_man_Event_Both.end(), EMT_man_Event_Target.begin(), EMT_man_Event_Target.end());
    EMT_man_pair_momenta_Both.insert(EMT_man_pair_momenta_Both.end(), EMT_man_pair_momenta_Outside.begin(),
                                     EMT_man_pair_momenta_Outside.end());
    EMT_man_pair_momenta_Both.insert(EMT_man_pair_momenta_Both.end(), EMT_man_pair_momenta_Target.begin(),
                                     EMT_man_pair_momenta_Target.end());
    EMT_man_NofRings_Both.insert(EMT_man_NofRings_Both.end(), EMT_man_NofRings_Outside.begin(),
                                 EMT_man_NofRings_Outside.end());
    EMT_man_NofRings_Both.insert(EMT_man_NofRings_Both.end(), EMT_man_NofRings_Target.begin(),
                                 EMT_man_NofRings_Target.end());
    Mixing_Both();
    EMT_man_Event_Both.clear();
    EMT_man_pair_momenta_Both.clear();
    EMT_man_NofRings_Both.clear();
  }

  if (fEventNumMan % numformix == 0) {
    Mixing_WAC();
    EMT_man_Event_WAC.clear();
    EMT_man_pair_momenta_WAC.clear();
    EMT_man_NofRings_WAC.clear();
  }

  if (fEventNumMan % numformix == 0) {
    Mixing_Target();
    EMT_man_Event_Target.clear();
    EMT_man_pair_momenta_Target.clear();
    EMT_man_NofRings_Target.clear();
  }

  if (fEventNumMan % numformix == 0) {
    Mixing_Outside();
    EMT_man_Event_Outside.clear();
    EMT_man_pair_momenta_Outside.clear();
    EMT_man_NofRings_Outside.clear();
  }
}


void CbmKresConversionManual::SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd,
                                                int richInd, int stsMcTrackId, CbmRichRing* RING)
{
  int InRich = FindInRich(richInd, stsMcTrackId);
  if (charge < 0) {
    VMCtracks_minus_Outside.push_back(mcTrack1);
    VStsTrack_minus_Outside.push_back(stsTrack);
    VRings_minus_Outside.push_back(InRich);
    VStsIndex_minus_Outside.push_back(stsInd);
    VRichRing_minus_Outside.push_back(RING);
    VMCIndex_minus_Outside.push_back(stsMcTrackId);
  }
  if (charge > 0) {
    VMCtracks_plus_Outside.push_back(mcTrack1);
    VStsTrack_plus_Outside.push_back(stsTrack);
    VRings_plus_Outside.push_back(InRich);
    VStsIndex_plus_Outside.push_back(stsInd);
    VRichRing_plus_Outside.push_back(RING);
    VMCIndex_plus_Outside.push_back(stsMcTrackId);
  }
}

void CbmKresConversionManual::SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom,
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
    VMCIndex_minus_Target.push_back(stsMcTrackId);
  }
  if (charge > 0) {
    VMCtracks_plus_Target.push_back(mcTrack1);
    VStsTrack_plus_Target.push_back(stsTrack);
    VMomenta_plus_Target.push_back(refmom);
    VRings_plus_Target.push_back(InRich);
    VStsIndex_plus_Target.push_back(stsInd);
    VRichRing_plus_Target.push_back(RING);
    VMCIndex_plus_Target.push_back(stsMcTrackId);
  }
}


void CbmKresConversionManual::FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                                               vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus,
                                               vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                                               vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus,
                                               std::vector<int> Rings_minus, std::vector<int> Rings_plus,
                                               std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus,
                                               vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus,
                                               vector<Int_t> MCIndex_minus, vector<Int_t> MCIndex_plus)
{
  for (size_t i = 0; i < Momenta_minus.size(); i++) {
    for (size_t j = 0; j < Momenta_plus.size(); j++) {

      CbmStsTrack* sts1 = StsTrack_minus.at(i);
      CbmStsTrack* sts2 = StsTrack_plus.at(j);

      TVector3 part1      = Momenta_minus[i];
      TVector3 part2      = Momenta_plus[j];
      CbmMCTrack* part1MC = MCtracks_minus[i];
      CbmMCTrack* part2MC = MCtracks_plus[j];

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

      Double_t InvmassReco  = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);

      InvMass_vs_OA_candidates_Target->Fill(InvmassReco, OpeningAngle);
      InvMass_vs_OA_candidates_Both->Fill(InvmassReco, OpeningAngle);
      GammasInvMass_candidates_Target->Fill(InvmassReco);
      GammasInvMass_candidates_Both->Fill(InvmassReco);
      GammasOA_candidates_Target->Fill(OpeningAngle);
      GammasOA_candidates_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(sts1, sts2);
      double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);


      int IdForANN = 0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() != -1 && mcTrackmama->GetPdgCode() == 22) {
          CbmMCTrack* mcTrackgrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
          //IdForANN = 1;
          if (mcTrackgrmama->GetPdgCode() == 111) {
            GammasInvMass_fromPi0_Target->Fill(InvmassReco);
            GammasInvMass_fromPi0_Both->Fill(InvmassReco);
            GammasOA_fromPi0_Target->Fill(OpeningAngle);
            GammasOA_fromPi0_Both->Fill(OpeningAngle);
            InvMass_vs_OA_fromPi0_Target->Fill(InvmassReco, OpeningAngle);
            InvMass_vs_OA_fromPi0_Both->Fill(InvmassReco, OpeningAngle);
            PlaneAngles_last_fromPi0_Target->Fill(PlaneAngle_last);
            PlaneAngles_last_fromPi0_Both->Fill(PlaneAngle_last);
            PlaneAngles_first_fromPi0_Target->Fill(PlaneAngle_first);
            PlaneAngles_first_fromPi0_Both->Fill(PlaneAngle_first);
            IdForANN = 1;
          }
        }
      }

      // run ANN
      if (AnnTrain == 1) {
        fTrain->Exec(EventNumMan, IdForANN, InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), part1,
                     part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnn == 1) {
        AnnValue =
          fAnnSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, fKFVertex.GetRefZ(), part1, part2);
        if (IdForANN == 1) AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) AnnFalsePairs->Fill(AnnValue);
      }


      PlaneAngles_last_candidates_Target->Fill(PlaneAngle_last);
      PlaneAngles_last_candidates_Both->Fill(PlaneAngle_last);
      PlaneAngles_first_candidates_Target->Fill(PlaneAngle_first);
      PlaneAngles_first_candidates_Both->Fill(PlaneAngle_first);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9) continue;

      if (IdForANN == 1) AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) AnnFalsePairs_AfterCuts->Fill(AnnValue);

      frefmomenta.clear();
      frefmomenta.push_back(part1);
      frefmomenta.push_back(part2);
      frefId.clear();
      frefId.push_back(stsIndex_minus[i]);
      frefId.push_back(stsIndex_plus[j]);
      fMCId.clear();
      fMCId.push_back(MCIndex_minus[i]);
      fMCId.push_back(MCIndex_plus[j]);
      fMCtracks.clear();
      fMCtracks.push_back(part1MC);
      fMCtracks.push_back(part2MC);

      // for event mixing
      EMT_man_Event_Target.push_back(EventNumMan);
      EMT_man_pair_momenta_Target.push_back(frefmomenta);
      EMT_man_NofRings_Target.push_back(richcheck);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Target.push_back(frefmomenta);
        Gammas_stsIndex_all_Target.push_back(frefId);
        Gammas_MCIndex_all_Target.push_back(fMCId);
        Gammas_MC_all_Target.push_back(fMCtracks);
        GammaInvMassReco_all_Target->Fill(InvmassReco);
        GammaOpeningAngleReco_all_Target->Fill(OpeningAngle);
        Pdg_all_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_all_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_all_Target->Fill(part1.Mag());
        P_reco_all_Target->Fill(part2.Mag());
        Pt_reco_all_Target->Fill(part1.Perp());
        Pt_reco_all_Target->Fill(part2.Perp());
        GammaInvMassReco_all_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_all_Both->Fill(OpeningAngle);
        Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_all_Both->Fill(part1.Mag());
        P_reco_all_Both->Fill(part2.Mag());
        Pt_reco_all_Both->Fill(part1.Perp());
        Pt_reco_all_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 0
      if (richcheck == 0) {
        Gammas_zero_Target.push_back(frefmomenta);
        Gammas_stsIndex_zero_Target.push_back(frefId);
        Gammas_MCIndex_zero_Target.push_back(fMCId);
        Gammas_MC_zero_Target.push_back(fMCtracks);
        GammaInvMassReco_zero_Target->Fill(InvmassReco);
        GammaOpeningAngleReco_zero_Target->Fill(OpeningAngle);
        Pdg_zero_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_zero_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_zero_Target->Fill(part1.Mag());
        P_reco_zero_Target->Fill(part2.Mag());
        Pt_reco_zero_Target->Fill(part1.Perp());
        Pt_reco_zero_Target->Fill(part2.Perp());
        GammaInvMassReco_zero_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_zero_Both->Fill(OpeningAngle);
        Pdg_zero_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_zero_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_zero_Both->Fill(part1.Mag());
        P_reco_zero_Both->Fill(part2.Mag());
        Pt_reco_zero_Both->Fill(part1.Perp());
        Pt_reco_zero_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 1
      if (richcheck == 1) {
        Gammas_one_Target.push_back(frefmomenta);
        Gammas_stsIndex_one_Target.push_back(frefId);
        Gammas_MCIndex_one_Target.push_back(fMCId);
        Gammas_MC_one_Target.push_back(fMCtracks);
        GammaInvMassReco_one_Target->Fill(InvmassReco);
        GammaOpeningAngleReco_one_Target->Fill(OpeningAngle);
        Pdg_one_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_one_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_one_Target->Fill(part1.Mag());
        P_reco_one_Target->Fill(part2.Mag());
        Pt_reco_one_Target->Fill(part1.Perp());
        Pt_reco_one_Target->Fill(part2.Perp());
        GammaInvMassReco_one_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_one_Both->Fill(OpeningAngle);
        Pdg_one_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_one_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_one_Both->Fill(part1.Mag());
        P_reco_one_Both->Fill(part2.Mag());
        Pt_reco_one_Both->Fill(part1.Perp());
        Pt_reco_one_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Target.push_back(frefmomenta);
        Gammas_stsIndex_two_Target.push_back(frefId);
        Gammas_MCIndex_two_Target.push_back(fMCId);
        Gammas_MC_two_Target.push_back(fMCtracks);
        GammaInvMassReco_two_Target->Fill(InvmassReco);
        GammaOpeningAngleReco_two_Target->Fill(OpeningAngle);
        Pdg_two_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_two_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_two_Target->Fill(part1.Mag());
        P_reco_two_Target->Fill(part2.Mag());
        Pt_reco_two_Target->Fill(part1.Perp());
        Pt_reco_two_Target->Fill(part2.Perp());
        GammaInvMassReco_two_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_two_Both->Fill(OpeningAngle);
        Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_two_Both->Fill(part1.Mag());
        P_reco_two_Both->Fill(part2.Mag());
        Pt_reco_two_Both->Fill(part1.Perp());
        Pt_reco_two_Both->Fill(part2.Perp());

        // WAC event mixing
        EMT_man_Event_WAC.push_back(EventNumMan);
        EMT_man_pair_momenta_WAC.push_back(frefmomenta);
        EMT_man_NofRings_WAC.push_back(richcheck);
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Target.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Target.push_back(frefId);
        Gammas_MCIndex_onetwo_Target.push_back(fMCId);
        Gammas_MC_onetwo_Target.push_back(fMCtracks);
        GammaInvMassReco_onetwo_Target->Fill(InvmassReco);
        GammaOpeningAngleReco_onetwo_Target->Fill(OpeningAngle);
        Pdg_onetwo_Target->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_onetwo_Target->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_onetwo_Target->Fill(part1.Mag());
        P_reco_onetwo_Target->Fill(part2.Mag());
        Pt_reco_onetwo_Target->Fill(part1.Perp());
        Pt_reco_onetwo_Target->Fill(part2.Perp());
        GammaInvMassReco_onetwo_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_onetwo_Both->Fill(OpeningAngle);
        Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_onetwo_Both->Fill(part1.Mag());
        P_reco_onetwo_Both->Fill(part2.Mag());
        Pt_reco_onetwo_Both->Fill(part1.Perp());
        Pt_reco_onetwo_Both->Fill(part2.Perp());
      }
    }
  }

  //cout << "number of gammas Target with 0-2 electron identified in RICH = " << Gammas_all_Target.size() << endl;
  //cout << "number of gammas Target with  0  electron identified in RICH = " << Gammas_zero_Target.size() << endl;
  //cout << "number of gammas Target with  1  electron identified in RICH = " << Gammas_one_Target.size() << endl;
  //cout << "number of gammas Target with  2  electron identified in RICH = " << Gammas_two_Target.size() << endl;
  //cout << "number of gammas Target with 1-2 electron identified in RICH = " << Gammas_onetwo_Target.size() << endl;
}


void CbmKresConversionManual::FindGammasOutside(
  int EventNumMan, double AngleCut, double InvMassCut, int RealPID, vector<CbmMCTrack*> MCtracks_minus_Outside,
  vector<CbmMCTrack*> MCtracks_plus_Outside, vector<CbmStsTrack*> StsTrack_minus_Outside,
  vector<CbmStsTrack*> StsTrack_plus_Outside, std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
  std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside,
  vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside,
  vector<Int_t> MCIndex_minus_Outside, vector<Int_t> MCIndex_plus_Outside)
{
  for (size_t i = 0; i < StsTrack_minus_Outside.size(); i++) {
    for (size_t j = 0; j < StsTrack_plus_Outside.size(); j++) {

      CbmStsTrack* part1STS = StsTrack_minus_Outside[i];
      CbmStsTrack* part2STS = StsTrack_plus_Outside[j];
      CbmMCTrack* part1MC   = MCtracks_minus_Outside[i];
      CbmMCTrack* part2MC   = MCtracks_plus_Outside[j];

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

      // From maksym program
      //TVector3 part1(electron.KFParticleBase::Px(), electron.KFParticleBase::Py(), electron.KFParticleBase::Pz());
      //TVector3 part2(positron.KFParticleBase::Px(), positron.KFParticleBase::Py(), positron.KFParticleBase::Pz());


      //cout << "=================" << endl;
      //cout << "FitToVertex part1: px = " << part1.X() << "; \t py = " << part1.Y() << "; \t pz = " << part1.Z() << endl;
      //cout << "FitToVertex part2: px = " << part2.X() << "; \t py = " << part2.Y() << "; \t pz = " << part2.Z() << endl;
      //cout << "KFParticle  part1: px = " << electron.KFParticleBase::Px() << "; \t py = " << electron.KFParticleBase::Py() << "; \t pz = " << electron.KFParticleBase::Pz() << endl;
      //cout << "KFParticle  part2: px = " << positron.KFParticleBase::Px() << "; \t py = " << positron.KFParticleBase::Py() << "; \t pz = " << positron.KFParticleBase::Pz() << endl;


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


      Double_t InvmassReco  = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      Double_t OpeningAngle = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);

      InvMass_vs_OA_candidates_Outside->Fill(InvmassReco, OpeningAngle);
      InvMass_vs_OA_candidates_Both->Fill(InvmassReco, OpeningAngle);
      GammasInvMass_candidates_Outside->Fill(InvmassReco);
      GammasInvMass_candidates_Both->Fill(InvmassReco);
      GammasOA_candidates_Outside->Fill(OpeningAngle);
      GammasOA_candidates_Both->Fill(OpeningAngle);

      double PlaneAngle_last  = CalculatePlaneAngle_last(part1STS, part2STS);
      double PlaneAngle_first = CalculatePlaneAngle_first(part1STS, part2STS);


      int IdForANN = 0;  // 0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() != -1 && mcTrackmama->GetPdgCode() == 22) {
          CbmMCTrack* mcTrackgrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
          // IdForANN = 1;
          if (mcTrackgrmama->GetPdgCode() == 111) {
            GammasInvMass_fromPi0_Outside->Fill(InvmassReco);
            GammasInvMass_fromPi0_Both->Fill(InvmassReco);
            GammasOA_fromPi0_Outside->Fill(OpeningAngle);
            GammasOA_fromPi0_Both->Fill(OpeningAngle);
            InvMass_vs_OA_fromPi0_Outside->Fill(InvmassReco, OpeningAngle);
            InvMass_vs_OA_fromPi0_Both->Fill(InvmassReco, OpeningAngle);
            PlaneAngles_last_fromPi0_Outside->Fill(PlaneAngle_last);
            PlaneAngles_last_fromPi0_Both->Fill(PlaneAngle_last);
            PlaneAngles_first_fromPi0_Outside->Fill(PlaneAngle_first);
            PlaneAngles_first_fromPi0_Both->Fill(PlaneAngle_first);
            IdForANN = 1;
          }
        }
      }

      // run ANN
      if (AnnTrain == 1) {
        fTrain->Exec(EventNumMan, IdForANN, InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), part1,
                     part2);
        continue;
      }

      double AnnValue = 999;
      if (UseAnn == 1) {
        AnnValue =
          fAnnSelection->DoSelect(InvmassReco, OpeningAngle, PlaneAngle_last, intersection.GetZ(), part1, part2);
        if (IdForANN == 1) AnnTruePairs->Fill(AnnValue);
        if (IdForANN == 0) AnnFalsePairs->Fill(AnnValue);
      }

      PlaneAngles_last_candidates_Outside->Fill(PlaneAngle_last);
      PlaneAngles_last_candidates_Both->Fill(PlaneAngle_last);
      PlaneAngles_first_candidates_Outside->Fill(PlaneAngle_first);
      PlaneAngles_first_candidates_Both->Fill(PlaneAngle_first);

      // cuts
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;
      // if (AnnValue < 0.9) continue;

      if (IdForANN == 1) AnnTruePairs_AfterCuts->Fill(AnnValue);
      if (IdForANN == 0) AnnFalsePairs_AfterCuts->Fill(AnnValue);

      frefmomenta.clear();
      frefmomenta.push_back(part1);
      frefmomenta.push_back(part2);

      frefId.clear();
      frefId.push_back(stsIndex_minus_Outside[i]);
      frefId.push_back(stsIndex_plus_Outside[j]);
      fMCtracks.clear();
      fMCtracks.push_back(part1MC);
      fMCtracks.push_back(part2MC);
      fMCId.clear();
      fMCId.push_back(MCIndex_minus_Outside[i]);
      fMCId.push_back(MCIndex_plus_Outside[j]);


      // for event mixing
      EMT_man_Event_Outside.push_back(EventNumMan);
      EMT_man_pair_momenta_Outside.push_back(frefmomenta);
      EMT_man_NofRings_Outside.push_back(richcheck);

      // WAC event mixing
      EMT_man_Event_WAC.push_back(EventNumMan);
      EMT_man_pair_momenta_WAC.push_back(frefmomenta);
      EMT_man_NofRings_WAC.push_back(richcheck);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Outside.push_back(frefmomenta);
        Gammas_stsIndex_all_Outside.push_back(frefId);
        Gammas_MCIndex_all_Outside.push_back(fMCId);
        Gammas_MC_all_Outside.push_back(fMCtracks);
        GammaInvMassReco_all_Outside->Fill(InvmassReco);
        GammaOpeningAngleReco_all_Outside->Fill(OpeningAngle);
        Pdg_all_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_all_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_all_Outside->Fill(part1.Mag());
        P_reco_all_Outside->Fill(part2.Mag());
        Pt_reco_all_Outside->Fill(part1.Perp());
        Pt_reco_all_Outside->Fill(part2.Perp());
        GammaInvMassReco_all_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_all_Both->Fill(OpeningAngle);
        Pdg_all_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_all_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_all_Both->Fill(part1.Mag());
        P_reco_all_Both->Fill(part2.Mag());
        Pt_reco_all_Both->Fill(part1.Perp());
        Pt_reco_all_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 0
      if (richcheck == 0) {
        Gammas_zero_Outside.push_back(frefmomenta);
        Gammas_stsIndex_zero_Outside.push_back(frefId);
        Gammas_MCIndex_zero_Outside.push_back(fMCId);
        Gammas_MC_zero_Outside.push_back(fMCtracks);
        GammaInvMassReco_zero_Outside->Fill(InvmassReco);
        GammaOpeningAngleReco_zero_Outside->Fill(OpeningAngle);
        Pdg_zero_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_zero_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_zero_Outside->Fill(part1.Mag());
        P_reco_zero_Outside->Fill(part2.Mag());
        Pt_reco_zero_Outside->Fill(part1.Perp());
        Pt_reco_zero_Outside->Fill(part2.Perp());
        GammaInvMassReco_zero_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_zero_Both->Fill(OpeningAngle);
        Pdg_zero_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_zero_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_zero_Both->Fill(part1.Mag());
        P_reco_zero_Both->Fill(part2.Mag());
        Pt_reco_zero_Both->Fill(part1.Perp());
        Pt_reco_zero_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 1
      if (richcheck == 1) {
        Gammas_one_Outside.push_back(frefmomenta);
        Gammas_stsIndex_one_Outside.push_back(frefId);
        Gammas_MCIndex_one_Outside.push_back(fMCId);
        Gammas_MC_one_Outside.push_back(fMCtracks);
        GammaInvMassReco_one_Outside->Fill(InvmassReco);
        GammaOpeningAngleReco_one_Outside->Fill(OpeningAngle);
        Pdg_one_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_one_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_one_Outside->Fill(part1.Mag());
        P_reco_one_Outside->Fill(part2.Mag());
        Pt_reco_one_Outside->Fill(part1.Perp());
        Pt_reco_one_Outside->Fill(part2.Perp());
        GammaInvMassReco_one_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_one_Both->Fill(OpeningAngle);
        Pdg_one_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_one_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_one_Both->Fill(part1.Mag());
        P_reco_one_Both->Fill(part2.Mag());
        Pt_reco_one_Both->Fill(part1.Perp());
        Pt_reco_one_Both->Fill(part2.Perp());
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Outside.push_back(frefmomenta);
        Gammas_stsIndex_two_Outside.push_back(frefId);
        Gammas_MCIndex_two_Outside.push_back(fMCId);
        Gammas_MC_two_Outside.push_back(fMCtracks);
        GammaInvMassReco_two_Outside->Fill(InvmassReco);
        GammaOpeningAngleReco_two_Outside->Fill(OpeningAngle);
        Pdg_two_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_two_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_two_Outside->Fill(part1.Mag());
        P_reco_two_Outside->Fill(part2.Mag());
        Pt_reco_two_Outside->Fill(part1.Perp());
        Pt_reco_two_Outside->Fill(part2.Perp());
        GammaInvMassReco_two_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_two_Both->Fill(OpeningAngle);
        Pdg_two_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_two_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_two_Both->Fill(part1.Mag());
        P_reco_two_Both->Fill(part2.Mag());
        Pt_reco_two_Both->Fill(part1.Perp());
        Pt_reco_two_Both->Fill(part2.Perp());
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Outside.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Outside.push_back(frefId);
        Gammas_MCIndex_onetwo_Outside.push_back(fMCId);
        Gammas_MC_onetwo_Outside.push_back(fMCtracks);
        GammaInvMassReco_onetwo_Outside->Fill(InvmassReco);
        GammaOpeningAngleReco_onetwo_Outside->Fill(OpeningAngle);
        Pdg_onetwo_Outside->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_onetwo_Outside->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_onetwo_Outside->Fill(part1.Mag());
        P_reco_onetwo_Outside->Fill(part2.Mag());
        Pt_reco_onetwo_Outside->Fill(part1.Perp());
        Pt_reco_onetwo_Outside->Fill(part2.Perp());
        GammaInvMassReco_onetwo_Both->Fill(InvmassReco);
        GammaOpeningAngleReco_onetwo_Both->Fill(OpeningAngle);
        Pdg_onetwo_Both->Fill(TMath::Abs(part1MC->GetPdgCode()));
        Pdg_onetwo_Both->Fill(TMath::Abs(part2MC->GetPdgCode()));
        P_reco_onetwo_Both->Fill(part1.Mag());
        P_reco_onetwo_Both->Fill(part2.Mag());
        Pt_reco_onetwo_Both->Fill(part1.Perp());
        Pt_reco_onetwo_Both->Fill(part2.Perp());
      }
    }
  }

  //cout << "number of gammas Outside with 0-2 electron identified in RICH = " << Gammas_all_Outside.size() << endl;
  //cout << "number of gammas Outside with  0  electron identified in RICH = " << Gammas_zero_Outside.size() << endl;
  //cout << "number of gammas Outside with  1  electron identified in RICH = " << Gammas_one_Outside.size() << endl;
  //cout << "number of gammas Outside with  2  electron identified in RICH = " << Gammas_two_Outside.size() << endl;
  //cout << "number of gammas Outside with 1-2 electron identified in RICH = " << Gammas_onetwo_Outside.size() << endl;
}


void CbmKresConversionManual::FindGammasBoth()
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

  Gammas_MCIndex_all_Both.insert(Gammas_MCIndex_all_Both.end(), Gammas_MCIndex_all_Outside.begin(),
                                 Gammas_MCIndex_all_Outside.end());
  Gammas_MCIndex_all_Both.insert(Gammas_MCIndex_all_Both.end(), Gammas_MCIndex_all_Target.begin(),
                                 Gammas_MCIndex_all_Target.end());
  Gammas_MCIndex_zero_Both.insert(Gammas_MCIndex_zero_Both.end(), Gammas_MCIndex_zero_Outside.begin(),
                                  Gammas_MCIndex_zero_Outside.end());
  Gammas_MCIndex_zero_Both.insert(Gammas_MCIndex_zero_Both.end(), Gammas_MCIndex_zero_Target.begin(),
                                  Gammas_MCIndex_zero_Target.end());
  Gammas_MCIndex_one_Both.insert(Gammas_MCIndex_one_Both.end(), Gammas_MCIndex_one_Outside.begin(),
                                 Gammas_MCIndex_one_Outside.end());
  Gammas_MCIndex_one_Both.insert(Gammas_MCIndex_one_Both.end(), Gammas_MCIndex_one_Target.begin(),
                                 Gammas_MCIndex_one_Target.end());
  Gammas_MCIndex_two_Both.insert(Gammas_MCIndex_two_Both.end(), Gammas_MCIndex_two_Outside.begin(),
                                 Gammas_MCIndex_two_Outside.end());
  Gammas_MCIndex_two_Both.insert(Gammas_MCIndex_two_Both.end(), Gammas_MCIndex_two_Target.begin(),
                                 Gammas_MCIndex_two_Target.end());
  Gammas_MCIndex_onetwo_Both.insert(Gammas_MCIndex_onetwo_Both.end(), Gammas_MCIndex_onetwo_Outside.begin(),
                                    Gammas_MCIndex_onetwo_Outside.end());
  Gammas_MCIndex_onetwo_Both.insert(Gammas_MCIndex_onetwo_Both.end(), Gammas_MCIndex_onetwo_Target.begin(),
                                    Gammas_MCIndex_onetwo_Target.end());

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

  //cout << "number of gammas Both with 0-2 electron identified in RICH = " << Gammas_all_Both.size() << endl;
  //cout << "number of gammas Both with  0  electron identified in RICH = " << Gammas_zero_Both.size() << endl;
  //cout << "number of gammas Both with  1  electron identified in RICH = " << Gammas_one_Both.size() << endl;
  //cout << "number of gammas Both with  2  electron identified in RICH = " << Gammas_two_Both.size() << endl;
  //cout << "number of gammas Both with 1-2 electron identified in RICH = " << Gammas_onetwo_Both.size() << endl;
}


void CbmKresConversionManual::FindGammasBothWithAdditionalCuts()
{
  // from target should be always 2 particles identified like electrons. From outside 0,1,2   		-->  target (two) + outside (all)
  Gamma_WAC.insert(Gamma_WAC.end(), Gammas_two_Target.begin(), Gammas_two_Target.end());
  Gamma_WAC.insert(Gamma_WAC.end(), Gammas_all_Outside.begin(), Gammas_all_Outside.end());
  Gammas_stsIndex_WAC.insert(Gammas_stsIndex_WAC.end(), Gammas_stsIndex_two_Target.begin(),
                             Gammas_stsIndex_two_Target.end());
  Gammas_stsIndex_WAC.insert(Gammas_stsIndex_WAC.end(), Gammas_stsIndex_all_Outside.begin(),
                             Gammas_stsIndex_all_Outside.end());
  Gammas_MCIndex_WAC.insert(Gammas_MCIndex_WAC.end(), Gammas_MCIndex_two_Target.begin(),
                            Gammas_MCIndex_two_Target.end());
  Gammas_MCIndex_WAC.insert(Gammas_MCIndex_WAC.end(), Gammas_MCIndex_all_Outside.begin(),
                            Gammas_MCIndex_all_Outside.end());
  Gammas_MC_WAC.insert(Gammas_MC_WAC.end(), Gammas_MC_two_Target.begin(), Gammas_MC_two_Target.end());
  Gammas_MC_WAC.insert(Gammas_MC_WAC.end(), Gammas_MC_all_Outside.begin(), Gammas_MC_all_Outside.end());
}


void CbmKresConversionManual::FindPi0(TString mod, TString position, vector<vector<TVector3>> Gammas,
                                      vector<vector<int>> StsIndex, vector<vector<int>> MCIndex,
                                      vector<vector<CbmMCTrack*>> GammasMC, TH1D* Pi0InvMassReco, TH2D* Pi0_pt_vs_rap,
                                      TH2D* Pi0_pt_vs_rap_est, TH2D* MultiplicityGamma,
                                      TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases, TH1D* DalitzPi0,
                                      TH1D* PhotonsPi0)
{
  // combine all gamma in pi0 --> calculate inv mass for gammas and pi0          // not the case, when one particle used twice for different gammas
  if (Gammas.size() < 2) return;  // min 2 gammas to form pi0 are required
  for (size_t gamma1 = 0; gamma1 < Gammas.size() - 1; gamma1++) {
    for (size_t gamma2 = gamma1 + 1; gamma2 < Gammas.size(); gamma2++) {
      // 4 reconstructed particles from gammas
      TVector3 e11 = Gammas[gamma1][0];
      TVector3 e12 = Gammas[gamma1][1];
      TVector3 e21 = Gammas[gamma2][0];
      TVector3 e22 = Gammas[gamma2][1];

      // MC true data for this particles
      CbmMCTrack* mcTrack1 = GammasMC[gamma1][0];
      CbmMCTrack* mcTrack2 = GammasMC[gamma1][1];
      CbmMCTrack* mcTrack3 = GammasMC[gamma2][0];
      CbmMCTrack* mcTrack4 = GammasMC[gamma2][1];

      if (StsIndex[gamma1][0] == StsIndex[gamma2][0] || StsIndex[gamma1][0] == StsIndex[gamma2][1]
          || StsIndex[gamma1][1] == StsIndex[gamma2][0] || StsIndex[gamma1][1] == StsIndex[gamma2][1])
        continue;  // particles are not used twice --> different

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      CbmKresFunctions::CalculateOpeningAngle_Reco(e11, e12);
      CbmKresFunctions::CalculateOpeningAngle_Reco(e21, e22);
      //			Double_t OA1 = CbmKresFunctions::CalculateOpeningAngle_Reco(e11, e12);
      //			Double_t OA2 = CbmKresFunctions::CalculateOpeningAngle_Reco(e21, e22);
      //if (params.fRapidity > 2.8 && params.fRapidity <= 3.6 && params.fPt > 0.0 && params.fPt <= 0.4 && OA1 > 1 && OA2 > 1) continue;

      Pi0InvMassReco->Fill(params.fMinv);
      MultiplicityGamma->Fill(Gammas.size(), params.fMinv);
      MultiplicityChargedParticles->Fill(fGlobalTracks->GetEntriesFast(), params.fMinv);

      // separate by rap and Pt only for onetwo
      if (mod == "OneTwo" && position == "Both") {

        // only pt
        if (params.fPt > 0.0 && params.fPt <= 0.1) pt_onetwo_1->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) pt_onetwo_2->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) pt_onetwo_3->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) pt_onetwo_4->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) pt_onetwo_5->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) pt_onetwo_6->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) pt_onetwo_7->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) pt_onetwo_8->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) pt_onetwo_9->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) pt_onetwo_10->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) pt_onetwo_11->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) pt_onetwo_12->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) pt_onetwo_13->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) pt_onetwo_14->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) pt_onetwo_15->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) pt_onetwo_16->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) pt_onetwo_17->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) pt_onetwo_18->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) pt_onetwo_19->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) pt_onetwo_20->Fill(params.fMinv);

        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_1->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_2->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_3->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_4->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_5->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_6->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_7->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_8->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_9->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_10->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_11->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_12->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_13->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_14->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_15->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_16->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_17->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_18->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_19->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_20->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_21->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_22->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_23->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_24->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_25->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_26->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_27->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_28->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_29->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_30->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_31->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_32->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_33->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_34->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_35->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_36->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_37->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_38->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_39->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_40->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_41->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_42->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_43->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_44->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_45->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_46->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_47->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_48->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_49->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_50->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_51->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_52->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_53->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_54->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_55->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_56->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_57->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_58->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_59->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_60->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_61->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_62->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_63->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_64->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_65->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_66->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_67->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_68->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_69->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_70->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_71->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_72->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_73->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_74->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_75->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_76->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_81->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_82->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_83->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_84->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_85->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_86->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_87->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_88->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_89->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_90->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_91->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_92->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.2 && params.fRapidity <= 3.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_101->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_102->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_103->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_104->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_105->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_106->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_107->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_108->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.6 && params.fRapidity <= 4.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_111->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_112->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_113->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_114->Fill(params.fMinv);
        }
      }


      // separate by rap and Pt only for onetwo
      if (mod == "All" && position == "Both") {

        // only pt
        if (params.fPt > 0.0 && params.fPt <= 0.1) pt_all_1->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) pt_all_2->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) pt_all_3->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) pt_all_4->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) pt_all_5->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) pt_all_6->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) pt_all_7->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) pt_all_8->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) pt_all_9->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) pt_all_10->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) pt_all_11->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) pt_all_12->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) pt_all_13->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) pt_all_14->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) pt_all_15->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) pt_all_16->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) pt_all_17->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) pt_all_18->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) pt_all_19->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) pt_all_20->Fill(params.fMinv);

        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_1->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_2->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_3->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_4->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_5->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_6->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_7->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_8->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_9->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_10->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_11->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_12->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_13->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_14->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_15->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_16->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_17->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_18->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_19->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_20->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_21->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_22->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_23->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_24->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_25->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_26->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_27->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_28->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_29->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_30->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_31->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_32->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_33->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_34->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_35->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_36->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_37->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_38->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_39->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_40->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_41->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_42->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_43->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_44->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_45->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_46->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_47->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_48->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_49->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_50->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_51->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_52->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_53->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_54->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_55->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_56->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_57->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_58->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_59->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_60->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_61->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_62->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_63->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_64->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_65->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_66->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_67->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_68->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_69->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_70->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_71->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_72->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_73->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_74->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_75->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_76->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_81->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_82->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_83->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_84->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_85->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_86->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_87->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_88->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_89->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_90->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_91->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_92->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.2 && params.fRapidity <= 3.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_101->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_102->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_103->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_104->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_105->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_106->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_107->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_108->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.6 && params.fRapidity <= 4.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_111->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_112->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_113->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_114->Fill(params.fMinv);
        }
      }


      if (position == "Both") fAnaBG->Exec(mcTrack1, mcTrack2, mcTrack3, mcTrack4, params.fMinv, BGCases);


      int STSmcId1 = MCIndex[gamma1][0];
      int STSmcId2 = MCIndex[gamma1][1];
      int STSmcId3 = MCIndex[gamma2][0];
      int STSmcId4 = MCIndex[gamma2][1];
      if (STSmcId1 == STSmcId2 || STSmcId1 == STSmcId3 || STSmcId1 == STSmcId4 || STSmcId2 == STSmcId3
          || STSmcId2 == STSmcId4 || STSmcId3 == STSmcId4)
        continue;  // particle is not used twice

      // fill histos with rapidity and Pt for correctly reconstructed pi0(dalitz) and pi0(g+g)
      if (nullptr == mcTrack1 || nullptr == mcTrack2 || nullptr == mcTrack3 || nullptr == mcTrack4) continue;
      if (TMath::Abs(mcTrack1->GetPdgCode()) != 11 || TMath::Abs(mcTrack2->GetPdgCode()) != 11
          || TMath::Abs(mcTrack3->GetPdgCode()) != 11 || TMath::Abs(mcTrack4->GetPdgCode()) != 11)
        continue;
      if (mcTrack1->GetPdgCode() + mcTrack2->GetPdgCode() != 0) continue;
      if (mcTrack3->GetPdgCode() + mcTrack4->GetPdgCode() != 0) continue;
      int motherId1 = mcTrack1->GetMotherId();
      int motherId2 = mcTrack2->GetMotherId();
      int motherId3 = mcTrack3->GetMotherId();
      int motherId4 = mcTrack4->GetMotherId();
      if (motherId1 == -1 || motherId2 == -1 || motherId3 == -1 || motherId4 == -1) continue;
      if (motherId1 != motherId2 || motherId3 != motherId4) continue;
      CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
      CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
      CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
      CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
      if (nullptr == mother1 || nullptr == mother2 || nullptr == mother3 || nullptr == mother4) continue;
      int mcMotherPdg1   = mother1->GetPdgCode();
      int mcMotherPdg2   = mother2->GetPdgCode();
      int mcMotherPdg3   = mother3->GetPdgCode();
      int mcMotherPdg4   = mother4->GetPdgCode();
      int grandmotherId1 = mother1->GetMotherId();
      int grandmotherId2 = mother2->GetMotherId();
      int grandmotherId3 = mother3->GetMotherId();
      int grandmotherId4 = mother4->GetMotherId();

      if (mcMotherPdg1 == 22 && mcMotherPdg2 == 22 && mcMotherPdg3 == 111 && mcMotherPdg4 == 111) {
        if (grandmotherId1 != motherId3) continue;
        Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
        Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
        if (mod == "OneTwo" && position == "Both") Pi0_pt_est_onetwo_Both->Fill(params.fPt);
        if (mod == "All" && position == "Both") Pi0_pt_est_all_Both->Fill(params.fPt);
        DalitzPi0->Fill(params.fMinv);
      }

      if (mcMotherPdg1 == 111 && mcMotherPdg2 == 111 && mcMotherPdg3 == 22 && mcMotherPdg4 == 22) {
        if (grandmotherId3 != motherId1) continue;
        Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
        Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
        if (mod == "OneTwo" && position == "Both") Pi0_pt_est_onetwo_Both->Fill(params.fPt);
        if (mod == "All" && position == "Both") Pi0_pt_est_all_Both->Fill(params.fPt);
        DalitzPi0->Fill(params.fMinv);
      }

      if (mcMotherPdg1 == 22 && mcMotherPdg2 == 22 && mcMotherPdg3 == 22 && mcMotherPdg4 == 22) {
        if (grandmotherId1 != grandmotherId2 || grandmotherId3 != grandmotherId4 || grandmotherId1 != grandmotherId3)
          continue;
        if (grandmotherId1 == -1) continue;
        CbmMCTrack* grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
        if (nullptr == grandmother1) continue;
        int mcGrandMotherPdg1 = grandmother1->GetPdgCode();
        if (mcGrandMotherPdg1 != 111) continue;
        Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
        Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
        if (mod == "OneTwo" && position == "Both") Pi0_pt_est_onetwo_Both->Fill(params.fPt);
        if (mod == "All" && position == "Both") Pi0_pt_est_all_Both->Fill(params.fPt);
        PhotonsPi0->Fill(params.fMinv);
      }
    }
  }
}


void CbmKresConversionManual::Mixing_Target()
// TARGET
{
  Int_t nof_Target = EMT_man_Event_Target.size();
  cout << "Mixing in Manual(Target) - nof entries " << nof_Target << endl;
  for (Int_t a = 0; a < nof_Target - 1; a++) {
    for (Int_t b = a + 1; b < nof_Target; b++) {
      if (EMT_man_Event_Target[a] == EMT_man_Event_Target[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11                  = EMT_man_pair_momenta_Target[a][0];
      TVector3 e12                  = EMT_man_pair_momenta_Target[a][1];
      TVector3 e21                  = EMT_man_pair_momenta_Target[b][0];
      TVector3 e22                  = EMT_man_pair_momenta_Target[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      EMT_InvMass_all_Target->Fill(params.fMinv);
      if (EMT_man_NofRings_Target[a] == 0 && EMT_man_NofRings_Target[b] == 0)
        EMT_InvMass_zero_Target->Fill(params.fMinv);
      if (EMT_man_NofRings_Target[a] == 1 && EMT_man_NofRings_Target[b] == 1)
        EMT_InvMass_one_Target->Fill(params.fMinv);
      if (EMT_man_NofRings_Target[a] == 2 && EMT_man_NofRings_Target[b] == 2)
        EMT_InvMass_two_Target->Fill(params.fMinv);
      if ((EMT_man_NofRings_Target[a] == 1 || EMT_man_NofRings_Target[a] == 2)
          && (EMT_man_NofRings_Target[b] == 1 || EMT_man_NofRings_Target[b] == 2))
        EMT_InvMass_onetwo_Target->Fill(params.fMinv);
    }
  }
}


void CbmKresConversionManual::Mixing_Outside()
// OUTSIDE
{
  Int_t nof_Outside = EMT_man_Event_Outside.size();
  cout << "Mixing in Manual(Outside) - nof entries " << nof_Outside << endl;
  for (Int_t a = 0; a < nof_Outside - 1; a++) {
    for (Int_t b = a + 1; b < nof_Outside; b++) {
      if (EMT_man_Event_Outside[a] == EMT_man_Event_Outside[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11                  = EMT_man_pair_momenta_Outside[a][0];
      TVector3 e12                  = EMT_man_pair_momenta_Outside[a][1];
      TVector3 e21                  = EMT_man_pair_momenta_Outside[b][0];
      TVector3 e22                  = EMT_man_pair_momenta_Outside[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      EMT_InvMass_all_Outside->Fill(params.fMinv);
      if (EMT_man_NofRings_Outside[a] == 0 && EMT_man_NofRings_Outside[b] == 0)
        EMT_InvMass_zero_Outside->Fill(params.fMinv);
      if (EMT_man_NofRings_Outside[a] == 1 && EMT_man_NofRings_Outside[b] == 1)
        EMT_InvMass_one_Outside->Fill(params.fMinv);
      if (EMT_man_NofRings_Outside[a] == 2 && EMT_man_NofRings_Outside[b] == 2)
        EMT_InvMass_two_Outside->Fill(params.fMinv);
      if ((EMT_man_NofRings_Outside[a] == 1 || EMT_man_NofRings_Outside[a] == 2)
          && (EMT_man_NofRings_Outside[b] == 1 || EMT_man_NofRings_Outside[b] == 2))
        EMT_InvMass_onetwo_Outside->Fill(params.fMinv);
    }
  }
}


void CbmKresConversionManual::Mixing_Both()
// BOTH
{
  Int_t nof_Both = EMT_man_Event_Both.size();
  cout << "Mixing in Manual(Both) - nof entries " << nof_Both << endl;
  for (Int_t a = 0; a < nof_Both - 1; a++) {
    for (Int_t b = a + 1; b < nof_Both; b++) {
      if (EMT_man_Event_Both[a] == EMT_man_Event_Both[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11                  = EMT_man_pair_momenta_Both[a][0];
      TVector3 e12                  = EMT_man_pair_momenta_Both[a][1];
      TVector3 e21                  = EMT_man_pair_momenta_Both[b][0];
      TVector3 e22                  = EMT_man_pair_momenta_Both[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      EMT_InvMass_all_Both->Fill(params.fMinv);
      if (EMT_man_NofRings_Both[a] == 0 && EMT_man_NofRings_Both[b] == 0) EMT_InvMass_zero_Both->Fill(params.fMinv);
      if (EMT_man_NofRings_Both[a] == 1 && EMT_man_NofRings_Both[b] == 1) EMT_InvMass_one_Both->Fill(params.fMinv);
      if (EMT_man_NofRings_Both[a] == 2 && EMT_man_NofRings_Both[b] == 2) EMT_InvMass_two_Both->Fill(params.fMinv);
      if ((EMT_man_NofRings_Both[a] == 1 || EMT_man_NofRings_Both[a] == 2)
          && (EMT_man_NofRings_Both[b] == 1 || EMT_man_NofRings_Both[b] == 2))
        EMT_InvMass_onetwo_Both->Fill(params.fMinv);

      // separate by rap and Pt only for onetwo
      if ((EMT_man_NofRings_Both[a] == 1 || EMT_man_NofRings_Both[a] == 2)
          && (EMT_man_NofRings_Both[b] == 1 || EMT_man_NofRings_Both[b] == 2)) {
        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_1->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_2->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_3->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_4->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_5->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_6->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_7->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_8->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_mixing_9->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_mixing_10->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_mixing_11->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_12->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_mixing_13->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_mixing_14->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_mixing_15->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_16->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_mixing_17->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_mixing_18->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_mixing_19->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_20->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_21->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_22->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_23->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_24->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_25->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_26->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_27->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_28->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_mixing_29->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_mixing_30->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_mixing_31->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_32->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_mixing_33->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_mixing_34->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_mixing_35->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_36->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_mixing_37->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_mixing_38->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_mixing_39->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_40->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_41->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_42->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_43->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_44->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_45->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_46->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_47->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_48->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_mixing_49->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_mixing_50->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_mixing_51->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_52->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_mixing_53->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_mixing_54->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_mixing_55->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_56->Fill(params.fMinv);
          if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_mixing_57->Fill(params.fMinv);
          if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_mixing_58->Fill(params.fMinv);
          if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_mixing_59->Fill(params.fMinv);
          if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_60->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_61->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_62->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_63->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_64->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_65->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_66->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_67->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_68->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_mixing_69->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_mixing_70->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_mixing_71->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_72->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_mixing_73->Fill(params.fMinv);
          if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_mixing_74->Fill(params.fMinv);
          if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_mixing_75->Fill(params.fMinv);
          if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_76->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_81->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_82->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_83->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_84->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_85->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_86->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_87->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_88->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_mixing_89->Fill(params.fMinv);
          if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_mixing_90->Fill(params.fMinv);
          if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_mixing_91->Fill(params.fMinv);
          if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_92->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.2 && params.fRapidity <= 3.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_101->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_102->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_103->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_104->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_mixing_105->Fill(params.fMinv);
          if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_mixing_106->Fill(params.fMinv);
          if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_mixing_107->Fill(params.fMinv);
          if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_108->Fill(params.fMinv);
        }
        if (params.fRapidity > 3.6 && params.fRapidity <= 4.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_mixing_111->Fill(params.fMinv);
          if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_mixing_112->Fill(params.fMinv);
          if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_mixing_113->Fill(params.fMinv);
          if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_114->Fill(params.fMinv);
        }
      }

      // separate by rap and Pt only for all
      if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_1->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_2->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_3->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_4->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_5->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_6->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_7->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_8->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_mixing_9->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_mixing_10->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_mixing_11->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_mixing_12->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_mixing_13->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_mixing_14->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_mixing_15->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_mixing_16->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_mixing_17->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_mixing_18->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_mixing_19->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_mixing_20->Fill(params.fMinv);
      }
      if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_21->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_22->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_23->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_24->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_25->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_26->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_27->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_28->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_mixing_29->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_mixing_30->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_mixing_31->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_mixing_32->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_mixing_33->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_mixing_34->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_mixing_35->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_mixing_36->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_mixing_37->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_mixing_38->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_mixing_39->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_mixing_40->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_41->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_42->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_43->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_44->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_45->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_46->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_47->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_48->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_mixing_49->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_mixing_50->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_mixing_51->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_mixing_52->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_mixing_53->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_mixing_54->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_mixing_55->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_mixing_56->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) rap_vs_Pt_InM_all_mixing_57->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) rap_vs_Pt_InM_all_mixing_58->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) rap_vs_Pt_InM_all_mixing_59->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) rap_vs_Pt_InM_all_mixing_60->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_61->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_62->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_63->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_64->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_65->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_66->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_67->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_68->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_mixing_69->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_mixing_70->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_mixing_71->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_mixing_72->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) rap_vs_Pt_InM_all_mixing_73->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) rap_vs_Pt_InM_all_mixing_74->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) rap_vs_Pt_InM_all_mixing_75->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) rap_vs_Pt_InM_all_mixing_76->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_81->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_82->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_83->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_84->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_85->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_86->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_87->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_88->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) rap_vs_Pt_InM_all_mixing_89->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) rap_vs_Pt_InM_all_mixing_90->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) rap_vs_Pt_InM_all_mixing_91->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) rap_vs_Pt_InM_all_mixing_92->Fill(params.fMinv);
      }
      if (params.fRapidity > 3.2 && params.fRapidity <= 3.6) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_101->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_102->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_103->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_104->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) rap_vs_Pt_InM_all_mixing_105->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) rap_vs_Pt_InM_all_mixing_106->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) rap_vs_Pt_InM_all_mixing_107->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) rap_vs_Pt_InM_all_mixing_108->Fill(params.fMinv);
      }
      if (params.fRapidity > 3.6 && params.fRapidity <= 4.0) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) rap_vs_Pt_InM_all_mixing_111->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) rap_vs_Pt_InM_all_mixing_112->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) rap_vs_Pt_InM_all_mixing_113->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) rap_vs_Pt_InM_all_mixing_114->Fill(params.fMinv);
      }

      // separate by Pt only for onetwo
      if ((EMT_man_NofRings_Both[a] == 1 || EMT_man_NofRings_Both[a] == 2)
          && (EMT_man_NofRings_Both[b] == 1 || EMT_man_NofRings_Both[b] == 2)) {
        if (params.fPt > 0.0 && params.fPt <= 0.1) pt_onetwo_mixing_1->Fill(params.fMinv);
        if (params.fPt > 0.1 && params.fPt <= 0.2) pt_onetwo_mixing_2->Fill(params.fMinv);
        if (params.fPt > 0.2 && params.fPt <= 0.3) pt_onetwo_mixing_3->Fill(params.fMinv);
        if (params.fPt > 0.3 && params.fPt <= 0.4) pt_onetwo_mixing_4->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.5) pt_onetwo_mixing_5->Fill(params.fMinv);
        if (params.fPt > 0.5 && params.fPt <= 0.6) pt_onetwo_mixing_6->Fill(params.fMinv);
        if (params.fPt > 0.6 && params.fPt <= 0.7) pt_onetwo_mixing_7->Fill(params.fMinv);
        if (params.fPt > 0.7 && params.fPt <= 0.8) pt_onetwo_mixing_8->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 0.9) pt_onetwo_mixing_9->Fill(params.fMinv);
        if (params.fPt > 0.9 && params.fPt <= 1.0) pt_onetwo_mixing_10->Fill(params.fMinv);
        if (params.fPt > 1.0 && params.fPt <= 1.1) pt_onetwo_mixing_11->Fill(params.fMinv);
        if (params.fPt > 1.1 && params.fPt <= 1.2) pt_onetwo_mixing_12->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.3) pt_onetwo_mixing_13->Fill(params.fMinv);
        if (params.fPt > 1.3 && params.fPt <= 1.4) pt_onetwo_mixing_14->Fill(params.fMinv);
        if (params.fPt > 1.4 && params.fPt <= 1.5) pt_onetwo_mixing_15->Fill(params.fMinv);
        if (params.fPt > 1.5 && params.fPt <= 1.6) pt_onetwo_mixing_16->Fill(params.fMinv);
        if (params.fPt > 1.6 && params.fPt <= 1.7) pt_onetwo_mixing_17->Fill(params.fMinv);
        if (params.fPt > 1.7 && params.fPt <= 1.8) pt_onetwo_mixing_18->Fill(params.fMinv);
        if (params.fPt > 1.8 && params.fPt <= 1.9) pt_onetwo_mixing_19->Fill(params.fMinv);
        if (params.fPt > 1.9 && params.fPt <= 2.0) pt_onetwo_mixing_20->Fill(params.fMinv);
      }

      if (params.fPt > 0.0 && params.fPt <= 0.1) pt_all_mixing_1->Fill(params.fMinv);
      if (params.fPt > 0.1 && params.fPt <= 0.2) pt_all_mixing_2->Fill(params.fMinv);
      if (params.fPt > 0.2 && params.fPt <= 0.3) pt_all_mixing_3->Fill(params.fMinv);
      if (params.fPt > 0.3 && params.fPt <= 0.4) pt_all_mixing_4->Fill(params.fMinv);
      if (params.fPt > 0.4 && params.fPt <= 0.5) pt_all_mixing_5->Fill(params.fMinv);
      if (params.fPt > 0.5 && params.fPt <= 0.6) pt_all_mixing_6->Fill(params.fMinv);
      if (params.fPt > 0.6 && params.fPt <= 0.7) pt_all_mixing_7->Fill(params.fMinv);
      if (params.fPt > 0.7 && params.fPt <= 0.8) pt_all_mixing_8->Fill(params.fMinv);
      if (params.fPt > 0.8 && params.fPt <= 0.9) pt_all_mixing_9->Fill(params.fMinv);
      if (params.fPt > 0.9 && params.fPt <= 1.0) pt_all_mixing_10->Fill(params.fMinv);
      if (params.fPt > 1.0 && params.fPt <= 1.1) pt_all_mixing_11->Fill(params.fMinv);
      if (params.fPt > 1.1 && params.fPt <= 1.2) pt_all_mixing_12->Fill(params.fMinv);
      if (params.fPt > 1.2 && params.fPt <= 1.3) pt_all_mixing_13->Fill(params.fMinv);
      if (params.fPt > 1.3 && params.fPt <= 1.4) pt_all_mixing_14->Fill(params.fMinv);
      if (params.fPt > 1.4 && params.fPt <= 1.5) pt_all_mixing_15->Fill(params.fMinv);
      if (params.fPt > 1.5 && params.fPt <= 1.6) pt_all_mixing_16->Fill(params.fMinv);
      if (params.fPt > 1.6 && params.fPt <= 1.7) pt_all_mixing_17->Fill(params.fMinv);
      if (params.fPt > 1.7 && params.fPt <= 1.8) pt_all_mixing_18->Fill(params.fMinv);
      if (params.fPt > 1.8 && params.fPt <= 1.9) pt_all_mixing_19->Fill(params.fMinv);
      if (params.fPt > 1.9 && params.fPt <= 2.0) pt_all_mixing_20->Fill(params.fMinv);
    }
  }
}

void CbmKresConversionManual::Mixing_WAC()
// WAC
{
  Int_t nof_WAC = EMT_man_Event_WAC.size();
  cout << "Mixing in Manual(WAC) - nof entries " << nof_WAC << endl;
  for (Int_t a = 0; a < nof_WAC - 1; a++) {
    for (Int_t b = a + 1; b < nof_WAC; b++) {
      if (EMT_man_Event_WAC[a] == EMT_man_Event_WAC[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11                  = EMT_man_pair_momenta_WAC[a][0];
      TVector3 e12                  = EMT_man_pair_momenta_WAC[a][1];
      TVector3 e21                  = EMT_man_pair_momenta_WAC[b][0];
      TVector3 e22                  = EMT_man_pair_momenta_WAC[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);
      EMT_InvMass_WAC->Fill(params.fMinv);
    }
  }
}

int CbmKresConversionManual::FindInRich(int richInd, int stsMcTrackId)
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


int CbmKresConversionManual::CheckIfElectron(CbmRichRing* ring, double momentum)
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


double CbmKresConversionManual::CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
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

double CbmKresConversionManual::CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
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


void CbmKresConversionManual::Finish()
{
  gDirectory->mkdir("Manual");
  gDirectory->cd("Manual");


  gDirectory->mkdir("Target");
  gDirectory->cd("Target");
  gDirectory->mkdir("CheckCuts_Target");
  gDirectory->cd("CheckCuts_Target");
  for (UInt_t i = 0; i < fHistoList_man_cuts_Target.size(); i++) {
    fHistoList_man_cuts_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("multiplicity");
  gDirectory->cd("multiplicity");
  for (UInt_t i = 0; i < fHistoList_multiplicity_man_Target.size(); i++) {
    fHistoList_multiplicity_man_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_man_all_Target.size(); i++) {
    fHistoList_man_all_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_man_zero_Target.size(); i++) {
    fHistoList_man_zero_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_man_one_Target.size(); i++) {
    fHistoList_man_one_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_man_two_Target.size(); i++) {
    fHistoList_man_two_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_man_onetwo_Target.size(); i++) {
    fHistoList_man_onetwo_Target[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");


  gDirectory->mkdir("Outside");
  gDirectory->cd("Outside");
  gDirectory->mkdir("CheckCuts_Outside");
  gDirectory->cd("CheckCuts_Outside");
  for (UInt_t i = 0; i < fHistoList_man_cuts_Outside.size(); i++) {
    fHistoList_man_cuts_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("multiplicity");
  gDirectory->cd("multiplicity");
  for (UInt_t i = 0; i < fHistoList_multiplicity_man_Outside.size(); i++) {
    fHistoList_multiplicity_man_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_man_all_Outside.size(); i++) {
    fHistoList_man_all_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_man_zero_Outside.size(); i++) {
    fHistoList_man_zero_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_man_one_Outside.size(); i++) {
    fHistoList_man_one_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_man_two_Outside.size(); i++) {
    fHistoList_man_two_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_man_onetwo_Outside.size(); i++) {
    fHistoList_man_onetwo_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->cd("..");


  gDirectory->mkdir("Both");
  gDirectory->cd("Both");
  gDirectory->mkdir("CheckCuts_Both");
  gDirectory->cd("CheckCuts_Both");
  for (UInt_t i = 0; i < fHistoList_man_cuts_Both.size(); i++) {
    fHistoList_man_cuts_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("multiplicity");
  gDirectory->cd("multiplicity");
  for (UInt_t i = 0; i < fHistoList_multiplicity_man_Both.size(); i++) {
    fHistoList_multiplicity_man_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("rap_vs_Pt_onetwo_Both");
  gDirectory->cd("rap_vs_Pt_onetwo_Both");
  for (UInt_t i = 0; i < fHistoList_rap_vs_pt_InM.size(); i++) {
    fHistoList_rap_vs_pt_InM[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("rap_vs_Pt_all_Both");
  gDirectory->cd("rap_vs_Pt_all_Both");
  for (UInt_t i = 0; i < fHistoList_rap_vs_pt_InM_all.size(); i++) {
    fHistoList_rap_vs_pt_InM_all[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("pt_onetwo_Both");
  gDirectory->cd("pt_onetwo_Both");
  for (UInt_t i = 0; i < fHistoList_pt_onetwo.size(); i++) {
    fHistoList_pt_onetwo[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("pt_all_Both");
  gDirectory->cd("pt_all_Both");
  for (UInt_t i = 0; i < fHistoList_pt_all.size(); i++) {
    fHistoList_pt_all[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("BG_Both");
  gDirectory->cd("BG_Both");
  gDirectory->mkdir("BG_Both_all");
  gDirectory->cd("BG_Both_all");
  for (UInt_t i = 0; i < fHistoList_bg_InM_all_Both.size(); i++) {
    fHistoList_bg_InM_all_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_Both_zero");
  gDirectory->cd("BG_Both_zero");
  for (UInt_t i = 0; i < fHistoList_bg_InM_zero_Both.size(); i++) {
    fHistoList_bg_InM_zero_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_Both_one");
  gDirectory->cd("BG_Both_one");
  for (UInt_t i = 0; i < fHistoList_bg_InM_one_Both.size(); i++) {
    fHistoList_bg_InM_one_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_Both_two");
  gDirectory->cd("BG_Both_two");
  for (UInt_t i = 0; i < fHistoList_bg_InM_two_Both.size(); i++) {
    fHistoList_bg_InM_two_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_Both_onetwo");
  gDirectory->cd("BG_Both_onetwo");
  for (UInt_t i = 0; i < fHistoList_bg_InM_onetwo_Both.size(); i++) {
    fHistoList_bg_InM_onetwo_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->cd("..");
  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_man_all_Both.size(); i++) {
    fHistoList_man_all_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("zero");
  gDirectory->cd("zero");
  for (UInt_t i = 0; i < fHistoList_man_zero_Both.size(); i++) {
    fHistoList_man_zero_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("one");
  gDirectory->cd("one");
  for (UInt_t i = 0; i < fHistoList_man_one_Both.size(); i++) {
    fHistoList_man_one_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_man_two_Both.size(); i++) {
    fHistoList_man_two_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_man_onetwo_Both.size(); i++) {
    fHistoList_man_onetwo_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_man_Both.size(); i++) {
    fHistoList_man_Both[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("WAC");
  gDirectory->cd("WAC");
  gDirectory->mkdir("BG_WAC");
  gDirectory->cd("BG_WAC");
  for (UInt_t i = 0; i < fHistoList_bg_InM_WAC.size(); i++) {
    fHistoList_bg_InM_WAC[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_man_WAC.size(); i++) {
    fHistoList_man_WAC[i]->Write();
  }
  gDirectory->cd("..");

  for (UInt_t i = 0; i < fHistoList_manual.size(); i++) {
    fHistoList_manual[i]->Write();
  }

  gDirectory->cd("..");
}


void CbmKresConversionManual::InitHistograms()
{
  ///////   histograms to check Cuts
  InvMass_vs_OA_candidates_Both = new TH2D("InvMass_vs_OA_candidates_Both",
                                           "InvMass_vs_OA_candidates_Both; invariant mass in GeV/c^{2}; "
                                           "opening angle in degree",
                                           500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Both.push_back(InvMass_vs_OA_candidates_Both);
  InvMass_vs_OA_fromPi0_Both = new TH2D("InvMass_vs_OA_fromPi0_Both",
                                        "InvMass_vs_OA_fromPi0_Both; invariant mass in GeV/c^{2}; opening "
                                        "angle in degree",
                                        500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Both.push_back(InvMass_vs_OA_fromPi0_Both);
  GammasInvMass_candidates_Both = new TH1D(
    "GammasInvMass_candidates_Both", "GammasInvMass_candidates_Both; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_man_cuts_Both.push_back(GammasInvMass_candidates_Both);
  GammasOA_candidates_Both =
    new TH1D("GammasOA_candidates_Both", "GammasOA_candidates_Both; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Both.push_back(GammasOA_candidates_Both);
  GammasInvMass_fromPi0_Both = new TH1D("GammasInvMass_fromPi0_Both",
                                        "GammasInvMass_fromPi0_Both; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_man_cuts_Both.push_back(GammasInvMass_fromPi0_Both);
  GammasOA_fromPi0_Both =
    new TH1D("GammasOA_fromPi0_Both", "GammasOA_fromPi0_Both; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Both.push_back(GammasOA_fromPi0_Both);
  PlaneAngles_last_candidates_Both = new TH1D(
    "PlaneAngles_last_candidates_Both", "PlaneAngles_last_candidates_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Both.push_back(PlaneAngles_last_candidates_Both);
  PlaneAngles_last_fromPi0_Both = new TH1D("PlaneAngles_last_fromPi0_Both",
                                           "PlaneAngles_last_fromPi0_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Both.push_back(PlaneAngles_last_fromPi0_Both);
  PlaneAngles_first_candidates_Both = new TH1D(
    "PlaneAngles_first_candidates_Both", "PlaneAngles_first_candidates_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Both.push_back(PlaneAngles_first_candidates_Both);
  PlaneAngles_first_fromPi0_Both = new TH1D("PlaneAngles_first_fromPi0_Both",
                                            "PlaneAngles_first_fromPi0_Both; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Both.push_back(PlaneAngles_first_fromPi0_Both);


  InvMass_vs_OA_candidates_Target = new TH2D("InvMass_vs_OA_candidates_Target",
                                             "InvMass_vs_OA_candidates_Target; invariant mass in GeV/c^{2}; "
                                             "opening angle in degree",
                                             500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Target.push_back(InvMass_vs_OA_candidates_Target);
  InvMass_vs_OA_fromPi0_Target = new TH2D("InvMass_vs_OA_fromPi0_Target",
                                          "InvMass_vs_OA_fromPi0_Target; invariant mass in GeV/c^{2}; "
                                          "opening angle in degree",
                                          500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Target.push_back(InvMass_vs_OA_fromPi0_Target);
  GammasInvMass_candidates_Target =
    new TH1D("GammasInvMass_candidates_Target", "GammasInvMass_candidates_Target; invariant mass in GeV/c^{2};#", 510,
             -0.01, 0.5);
  fHistoList_man_cuts_Target.push_back(GammasInvMass_candidates_Target);
  GammasOA_candidates_Target =
    new TH1D("GammasOA_candidates_Target", "GammasOA_candidates_Target; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Target.push_back(GammasOA_candidates_Target);
  GammasInvMass_fromPi0_Target = new TH1D(
    "GammasInvMass_fromPi0_Target", "GammasInvMass_fromPi0_Target; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_man_cuts_Target.push_back(GammasInvMass_fromPi0_Target);
  GammasOA_fromPi0_Target =
    new TH1D("GammasOA_fromPi0_Target", "GammasOA_fromPi0_Target; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Target.push_back(GammasOA_fromPi0_Target);
  PlaneAngles_last_candidates_Target =
    new TH1D("PlaneAngles_last_candidates_Target", "PlaneAngles_last_candidates_Target; #theta angle in degree;#", 720,
             -1., 179.);
  fHistoList_man_cuts_Target.push_back(PlaneAngles_last_candidates_Target);
  PlaneAngles_last_fromPi0_Target = new TH1D(
    "PlaneAngles_last_fromPi0_Target", "PlaneAngles_last_fromPi0_Target; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Target.push_back(PlaneAngles_last_fromPi0_Target);
  PlaneAngles_first_candidates_Target =
    new TH1D("PlaneAngles_first_candidates_Target", "PlaneAngles_first_candidates_Target; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_man_cuts_Target.push_back(PlaneAngles_first_candidates_Target);
  PlaneAngles_first_fromPi0_Target = new TH1D(
    "PlaneAngles_first_fromPi0_Target", "PlaneAngles_first_fromPi0_Target; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Target.push_back(PlaneAngles_first_fromPi0_Target);


  InvMass_vs_OA_candidates_Outside = new TH2D("InvMass_vs_OA_candidates_Outside",
                                              "InvMass_vs_OA_candidates_Outside; invariant mass in GeV/c^{2}; "
                                              "opening angle in degree",
                                              500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Outside.push_back(InvMass_vs_OA_candidates_Outside);
  InvMass_vs_OA_fromPi0_Outside = new TH2D("InvMass_vs_OA_fromPi0_Outside",
                                           "InvMass_vs_OA_fromPi0_Outside; invariant mass in GeV/c^{2}; "
                                           "opening angle in degree",
                                           500, 0, 0.5, 500, 0, 50);
  fHistoList_man_cuts_Outside.push_back(InvMass_vs_OA_fromPi0_Outside);
  GammasInvMass_candidates_Outside =
    new TH1D("GammasInvMass_candidates_Outside", "GammasInvMass_candidates_Outside; invariant mass in GeV/c^{2};#", 510,
             -0.01, 0.5);
  fHistoList_man_cuts_Outside.push_back(GammasInvMass_candidates_Outside);
  GammasOA_candidates_Outside =
    new TH1D("GammasOA_candidates_Outside", "GammasOA_candidates_Outside; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Outside.push_back(GammasOA_candidates_Outside);
  GammasInvMass_fromPi0_Outside = new TH1D(
    "GammasInvMass_fromPi0_Outside", "GammasInvMass_fromPi0_Outside; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_man_cuts_Outside.push_back(GammasInvMass_fromPi0_Outside);
  GammasOA_fromPi0_Outside =
    new TH1D("GammasOA_fromPi0_Outside", "GammasOA_fromPi0_Outside; opening angle in degree;#", 300, -0.1, 29.9);
  fHistoList_man_cuts_Outside.push_back(GammasOA_fromPi0_Outside);
  PlaneAngles_last_candidates_Outside =
    new TH1D("PlaneAngles_last_candidates_Outside", "PlaneAngles_last_candidates_Outside; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_man_cuts_Outside.push_back(PlaneAngles_last_candidates_Outside);
  PlaneAngles_last_fromPi0_Outside = new TH1D(
    "PlaneAngles_last_fromPi0_Outside", "PlaneAngles_last_fromPi0_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Outside.push_back(PlaneAngles_last_fromPi0_Outside);
  PlaneAngles_first_candidates_Outside =
    new TH1D("PlaneAngles_first_candidates_Outside", "PlaneAngles_first_candidates_Outside; #theta angle in degree;#",
             720, -1., 179.);
  fHistoList_man_cuts_Outside.push_back(PlaneAngles_first_candidates_Outside);
  PlaneAngles_first_fromPi0_Outside = new TH1D(
    "PlaneAngles_first_fromPi0_Outside", "PlaneAngles_first_fromPi0_Outside; #theta angle in degree;#", 720, -1., 179.);
  fHistoList_man_cuts_Outside.push_back(PlaneAngles_first_fromPi0_Outside);


  // Target => all
  GammaInvMassReco_all_Target = new TH1D("GammaInvMassReco_all_Target",
                                         "GammaInvMassReco_all_Target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_all_Target.push_back(GammaInvMassReco_all_Target);
  GammaOpeningAngleReco_all_Target =
    new TH1D("GammaOpeningAngleReco_all_Target", "GammaOpeningAngleReco_all_Target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_all_Target.push_back(GammaOpeningAngleReco_all_Target);
  Pdg_all_Target = new TH1D("Pdg_all_Target", "Pdg_all_Target; Id;#", 800, 0, 400);
  fHistoList_man_all_Target.push_back(Pdg_all_Target);
  P_reco_all_Target = new TH1D("P_reco_all_Target", "P_reco_all_Target; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_all_Target.push_back(P_reco_all_Target);
  Pt_reco_all_Target = new TH1D("Pt_reco_all_Target", "Pt_reco_all_Target; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_all_Target.push_back(Pt_reco_all_Target);
  Pi0InvMassReco_all_Target =
    new TH1D("Pi0InvMassReco_all_Target", "Pi0InvMassReco_all_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Target.push_back(Pi0InvMassReco_all_Target);
  EMT_InvMass_all_Target =
    new TH1D("EMT_InvMass_all_Target", "EMT_InvMass_all_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Target.push_back(EMT_InvMass_all_Target);
  Pi0_pt_vs_rap_all_Target = new TH2D(
    "Pi0_pt_vs_rap_all_Target", "Pi0_pt_vs_rap_all_Target; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_all_Target.push_back(Pi0_pt_vs_rap_all_Target);
  Pi0_pt_vs_rap_est_all_Target =
    new TH2D("Pi0_pt_vs_rap_est_all_Target", "Pi0_pt_vs_rap_est_all_Target; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_all_Target.push_back(Pi0_pt_vs_rap_est_all_Target);
  DalitzPi0_all_Target =
    new TH1D("DalitzPi0_all_Target", "DalitzPi0_all_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Target.push_back(DalitzPi0_all_Target);
  PhotonsPi0_all_Target =
    new TH1D("PhotonsPi0_all_Target", "PhotonsPi0_all_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Target.push_back(PhotonsPi0_all_Target);


  // Target => zero
  GammaInvMassReco_zero_Target = new TH1D(
    "GammaInvMassReco_zero_Target", "GammaInvMassReco_zero_Target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_zero_Target.push_back(GammaInvMassReco_zero_Target);
  GammaOpeningAngleReco_zero_Target =
    new TH1D("GammaOpeningAngleReco_zero_Target", "GammaOpeningAngleReco_zero_Target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_zero_Target.push_back(GammaOpeningAngleReco_zero_Target);
  Pdg_zero_Target = new TH1D("Pdg_zero_Target", "Pdg_zero_Target; Id;#", 800, 0, 400);
  fHistoList_man_zero_Target.push_back(Pdg_zero_Target);
  P_reco_zero_Target = new TH1D("P_reco_zero_Target", "P_reco_zero_Target; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_zero_Target.push_back(P_reco_zero_Target);
  Pt_reco_zero_Target = new TH1D("Pt_reco_zero_Target", "Pt_reco_zero_Target; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_zero_Target.push_back(Pt_reco_zero_Target);
  Pi0InvMassReco_zero_Target =
    new TH1D("Pi0InvMassReco_zero_Target", "Pi0InvMassReco_zero_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Target.push_back(Pi0InvMassReco_zero_Target);
  EMT_InvMass_zero_Target =
    new TH1D("EMT_InvMass_zero_Target", "EMT_InvMass_zero_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Target.push_back(EMT_InvMass_zero_Target);
  Pi0_pt_vs_rap_zero_Target = new TH2D(
    "Pi0_pt_vs_rap_zero_Target", "Pi0_pt_vs_rap_zero_Target; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_zero_Target.push_back(Pi0_pt_vs_rap_zero_Target);
  Pi0_pt_vs_rap_est_zero_Target =
    new TH2D("Pi0_pt_vs_rap_est_zero_Target", "Pi0_pt_vs_rap_est_zero_Target; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_zero_Target.push_back(Pi0_pt_vs_rap_est_zero_Target);
  DalitzPi0_zero_Target =
    new TH1D("DalitzPi0_zero_Target", "DalitzPi0_zero_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Target.push_back(DalitzPi0_zero_Target);
  PhotonsPi0_zero_Target =
    new TH1D("PhotonsPi0_zero_Target", "PhotonsPi0_zero_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Target.push_back(PhotonsPi0_zero_Target);


  // Target => one
  GammaInvMassReco_one_Target = new TH1D("GammaInvMassReco_one_Target",
                                         "GammaInvMassReco_one_Target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_one_Target.push_back(GammaInvMassReco_one_Target);
  GammaOpeningAngleReco_one_Target =
    new TH1D("GammaOpeningAngleReco_one_Target", "GammaOpeningAngleReco_one_Target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_one_Target.push_back(GammaOpeningAngleReco_one_Target);
  Pdg_one_Target = new TH1D("Pdg_one_Target", "Pdg_one_Target; Id;#", 800, 0, 400);
  fHistoList_man_one_Target.push_back(Pdg_one_Target);
  P_reco_one_Target = new TH1D("P_reco_one_Target", "P_reco_one_Target; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_one_Target.push_back(P_reco_one_Target);
  Pt_reco_one_Target = new TH1D("Pt_reco_one_Target", "Pt_reco_one_Target; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_one_Target.push_back(Pt_reco_one_Target);
  Pi0InvMassReco_one_Target =
    new TH1D("Pi0InvMassReco_one_Target", "Pi0InvMassReco_one_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Target.push_back(Pi0InvMassReco_one_Target);
  EMT_InvMass_one_Target =
    new TH1D("EMT_InvMass_one_Target", "EMT_InvMass_one_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Target.push_back(EMT_InvMass_one_Target);
  Pi0_pt_vs_rap_one_Target = new TH2D(
    "Pi0_pt_vs_rap_one_Target", "Pi0_pt_vs_rap_one_Target; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_one_Target.push_back(Pi0_pt_vs_rap_one_Target);
  Pi0_pt_vs_rap_est_one_Target =
    new TH2D("Pi0_pt_vs_rap_est_one_Target", "Pi0_pt_vs_rap_est_one_Target; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_one_Target.push_back(Pi0_pt_vs_rap_est_one_Target);
  DalitzPi0_one_Target =
    new TH1D("DalitzPi0_one_Target", "DalitzPi0_one_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Target.push_back(DalitzPi0_one_Target);
  PhotonsPi0_one_Target =
    new TH1D("PhotonsPi0_one_Target", "PhotonsPi0_one_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Target.push_back(PhotonsPi0_one_Target);


  // Target => two
  GammaInvMassReco_two_Target = new TH1D("GammaInvMassReco_two_Target",
                                         "GammaInvMassReco_two_Target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_two_Target.push_back(GammaInvMassReco_two_Target);
  GammaOpeningAngleReco_two_Target =
    new TH1D("GammaOpeningAngleReco_two_Target", "GammaOpeningAngleReco_two_Target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_two_Target.push_back(GammaOpeningAngleReco_two_Target);
  Pdg_two_Target = new TH1D("Pdg_two_Target", "Pdg_two_Target; Id;#", 800, 0, 400);
  fHistoList_man_two_Target.push_back(Pdg_two_Target);
  P_reco_two_Target = new TH1D("P_reco_two_Target", "P_reco_two_Target; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_two_Target.push_back(P_reco_two_Target);
  Pt_reco_two_Target = new TH1D("Pt_reco_two_Target", "Pt_reco_two_Target; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_two_Target.push_back(Pt_reco_two_Target);
  Pi0InvMassReco_two_Target =
    new TH1D("Pi0InvMassReco_two_Target", "Pi0InvMassReco_two_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Target.push_back(Pi0InvMassReco_two_Target);
  EMT_InvMass_two_Target =
    new TH1D("EMT_InvMass_two_Target", "EMT_InvMass_two_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Target.push_back(EMT_InvMass_two_Target);
  Pi0_pt_vs_rap_two_Target = new TH2D(
    "Pi0_pt_vs_rap_two_Target", "Pi0_pt_vs_rap_two_Target; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_two_Target.push_back(Pi0_pt_vs_rap_two_Target);
  Pi0_pt_vs_rap_est_two_Target =
    new TH2D("Pi0_pt_vs_rap_est_two_Target", "Pi0_pt_vs_rap_est_two_Target; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_two_Target.push_back(Pi0_pt_vs_rap_est_two_Target);
  DalitzPi0_two_Target =
    new TH1D("DalitzPi0_two_Target", "DalitzPi0_two_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Target.push_back(DalitzPi0_two_Target);
  PhotonsPi0_two_Target =
    new TH1D("PhotonsPi0_two_Target", "PhotonsPi0_two_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Target.push_back(PhotonsPi0_two_Target);


  // Target => onetwo
  GammaInvMassReco_onetwo_Target = new TH1D(
    "GammaInvMassReco_onetwo_Target", "GammaInvMassReco_onetwo_Target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_onetwo_Target.push_back(GammaInvMassReco_onetwo_Target);
  GammaOpeningAngleReco_onetwo_Target = new TH1D("GammaOpeningAngleReco_onetwo_Target",
                                                 "GammaOpeningAngleReco_onetwo_Target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_onetwo_Target.push_back(GammaOpeningAngleReco_onetwo_Target);
  Pdg_onetwo_Target = new TH1D("Pdg_onetwo_Target", "Pdg_onetwo_Target; Id;#", 800, 0, 400);
  fHistoList_man_onetwo_Target.push_back(Pdg_onetwo_Target);
  P_reco_onetwo_Target = new TH1D("P_reco_onetwo_Target", "P_reco_onetwo_Target; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_onetwo_Target.push_back(P_reco_onetwo_Target);
  Pt_reco_onetwo_Target = new TH1D("Pt_reco_onetwo_Target", "Pt_reco_onetwo_Target; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_onetwo_Target.push_back(Pt_reco_onetwo_Target);
  Pi0InvMassReco_onetwo_Target = new TH1D("Pi0InvMassReco_onetwo_Target",
                                          "Pi0InvMassReco_onetwo_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Target.push_back(Pi0InvMassReco_onetwo_Target);
  EMT_InvMass_onetwo_Target =
    new TH1D("EMT_InvMass_onetwo_Target", "EMT_InvMass_onetwo_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Target.push_back(EMT_InvMass_onetwo_Target);
  Pi0_pt_vs_rap_onetwo_Target =
    new TH2D("Pi0_pt_vs_rap_onetwo_Target", "Pi0_pt_vs_rap_onetwo_Target; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60,
             -1., 5.);
  fHistoList_man_onetwo_Target.push_back(Pi0_pt_vs_rap_onetwo_Target);
  Pi0_pt_vs_rap_est_onetwo_Target =
    new TH2D("Pi0_pt_vs_rap_est_onetwo_Target", "Pi0_pt_vs_rap_est_onetwo_Target; rapidity y; p_{t} in GeV/c ", 10, 0.,
             4., 40, 0., 4.);
  fHistoList_man_onetwo_Target.push_back(Pi0_pt_vs_rap_est_onetwo_Target);
  DalitzPi0_onetwo_Target =
    new TH1D("DalitzPi0_onetwo_Target", "DalitzPi0_onetwo_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Target.push_back(DalitzPi0_onetwo_Target);
  PhotonsPi0_onetwo_Target =
    new TH1D("PhotonsPi0_onetwo_Target", "PhotonsPi0_onetwo_Target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Target.push_back(PhotonsPi0_onetwo_Target);


  // Outside => all
  GammaInvMassReco_all_Outside = new TH1D(
    "GammaInvMassReco_all_Outside", "GammaInvMassReco_all_Outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_all_Outside.push_back(GammaInvMassReco_all_Outside);
  GammaOpeningAngleReco_all_Outside =
    new TH1D("GammaOpeningAngleReco_all_Outside", "GammaOpeningAngleReco_all_Outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_all_Outside.push_back(GammaOpeningAngleReco_all_Outside);
  Pdg_all_Outside = new TH1D("Pdg_all_Outside", "Pdg_all_Outside; Id;#", 800, 0, 400);
  fHistoList_man_all_Outside.push_back(Pdg_all_Outside);
  P_reco_all_Outside = new TH1D("P_reco_all_Outside", "P_reco_all_Outside; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_all_Outside.push_back(P_reco_all_Outside);
  Pt_reco_all_Outside = new TH1D("Pt_reco_all_Outside", "Pt_reco_all_Outside; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_all_Outside.push_back(Pt_reco_all_Outside);
  Pi0InvMassReco_all_Outside =
    new TH1D("Pi0InvMassReco_all_Outside", "Pi0InvMassReco_all_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Outside.push_back(Pi0InvMassReco_all_Outside);
  EMT_InvMass_all_Outside =
    new TH1D("EMT_InvMass_all_Outside", "EMT_InvMass_all_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Outside.push_back(EMT_InvMass_all_Outside);
  Pi0_pt_vs_rap_all_Outside = new TH2D(
    "Pi0_pt_vs_rap_all_Outside", "Pi0_pt_vs_rap_all_Outside; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_all_Outside.push_back(Pi0_pt_vs_rap_all_Outside);
  Pi0_pt_vs_rap_est_all_Outside =
    new TH2D("Pi0_pt_vs_rap_est_all_Outside", "Pi0_pt_vs_rap_est_all_Outside; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_all_Outside.push_back(Pi0_pt_vs_rap_est_all_Outside);
  DalitzPi0_all_Outside =
    new TH1D("DalitzPi0_all_Outside", "DalitzPi0_all_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Outside.push_back(DalitzPi0_all_Outside);
  PhotonsPi0_all_Outside =
    new TH1D("PhotonsPi0_all_Outside", "PhotonsPi0_all_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Outside.push_back(PhotonsPi0_all_Outside);


  // Outside => zero
  GammaInvMassReco_zero_Outside = new TH1D(
    "GammaInvMassReco_zero_Outside", "GammaInvMassReco_zero_Outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_zero_Outside.push_back(GammaInvMassReco_zero_Outside);
  GammaOpeningAngleReco_zero_Outside = new TH1D("GammaOpeningAngleReco_zero_Outside",
                                                "GammaOpeningAngleReco_zero_Outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_zero_Outside.push_back(GammaOpeningAngleReco_zero_Outside);
  Pdg_zero_Outside = new TH1D("Pdg_zero_Outside", "Pdg_zero_Outside; Id;#", 800, 0, 400);
  fHistoList_man_zero_Outside.push_back(Pdg_zero_Outside);
  P_reco_zero_Outside = new TH1D("P_reco_zero_Outside", "P_reco_zero_Outside; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_zero_Outside.push_back(P_reco_zero_Outside);
  Pt_reco_zero_Outside = new TH1D("Pt_reco_zero_Outside", "Pt_reco_zero_Outside; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_zero_Outside.push_back(Pt_reco_zero_Outside);
  Pi0InvMassReco_zero_Outside =
    new TH1D("Pi0InvMassReco_zero_Outside", "Pi0InvMassReco_zero_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Outside.push_back(Pi0InvMassReco_zero_Outside);
  EMT_InvMass_zero_Outside =
    new TH1D("EMT_InvMass_zero_Outside", "EMT_InvMass_zero_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Outside.push_back(EMT_InvMass_zero_Outside);
  Pi0_pt_vs_rap_zero_Outside = new TH2D(
    "Pi0_pt_vs_rap_zero_Outside", "Pi0_pt_vs_rap_zero_Outside; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_zero_Outside.push_back(Pi0_pt_vs_rap_zero_Outside);
  Pi0_pt_vs_rap_est_zero_Outside =
    new TH2D("Pi0_pt_vs_rap_est_zero_Outside", "Pi0_pt_vs_rap_est_zero_Outside; rapidity y; p_{t} in GeV/c ", 10, 0.,
             4., 40, 0., 4.);
  fHistoList_man_zero_Outside.push_back(Pi0_pt_vs_rap_est_zero_Outside);
  DalitzPi0_zero_Outside =
    new TH1D("DalitzPi0_zero_Outside", "DalitzPi0_zero_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Outside.push_back(DalitzPi0_zero_Outside);
  PhotonsPi0_zero_Outside =
    new TH1D("PhotonsPi0_zero_Outside", "PhotonsPi0_zero_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Outside.push_back(PhotonsPi0_zero_Outside);


  // Outside => one
  GammaInvMassReco_one_Outside = new TH1D(
    "GammaInvMassReco_one_Outside", "GammaInvMassReco_one_Outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_one_Outside.push_back(GammaInvMassReco_one_Outside);
  GammaOpeningAngleReco_one_Outside =
    new TH1D("GammaOpeningAngleReco_one_Outside", "GammaOpeningAngleReco_one_Outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_one_Outside.push_back(GammaOpeningAngleReco_one_Outside);
  Pdg_one_Outside = new TH1D("Pdg_one_Outside", "Pdg_one_Outside; Id;#", 800, 0, 400);
  fHistoList_man_one_Outside.push_back(Pdg_one_Outside);
  P_reco_one_Outside = new TH1D("P_reco_one_Outside", "P_reco_one_Outside; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_one_Outside.push_back(P_reco_one_Outside);
  Pt_reco_one_Outside = new TH1D("Pt_reco_one_Outside", "Pt_reco_one_Outside; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_one_Outside.push_back(Pt_reco_one_Outside);
  Pi0InvMassReco_one_Outside =
    new TH1D("Pi0InvMassReco_one_Outside", "Pi0InvMassReco_one_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Outside.push_back(Pi0InvMassReco_one_Outside);
  EMT_InvMass_one_Outside =
    new TH1D("EMT_InvMass_one_Outside", "EMT_InvMass_one_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Outside.push_back(EMT_InvMass_one_Outside);
  Pi0_pt_vs_rap_one_Outside = new TH2D(
    "Pi0_pt_vs_rap_one_Outside", "Pi0_pt_vs_rap_one_Outside; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_one_Outside.push_back(Pi0_pt_vs_rap_one_Outside);
  Pi0_pt_vs_rap_est_one_Outside =
    new TH2D("Pi0_pt_vs_rap_est_one_Outside", "Pi0_pt_vs_rap_est_one_Outside; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_one_Outside.push_back(Pi0_pt_vs_rap_est_one_Outside);
  DalitzPi0_one_Outside =
    new TH1D("DalitzPi0_one_Outside", "DalitzPi0_one_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Outside.push_back(DalitzPi0_one_Outside);
  PhotonsPi0_one_Outside =
    new TH1D("PhotonsPi0_one_Outside", "PhotonsPi0_one_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Outside.push_back(PhotonsPi0_one_Outside);


  // Outside => two
  GammaInvMassReco_two_Outside = new TH1D(
    "GammaInvMassReco_two_Outside", "GammaInvMassReco_two_Outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_two_Outside.push_back(GammaInvMassReco_two_Outside);
  GammaOpeningAngleReco_two_Outside =
    new TH1D("GammaOpeningAngleReco_two_Outside", "GammaOpeningAngleReco_two_Outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_two_Outside.push_back(GammaOpeningAngleReco_two_Outside);
  Pdg_two_Outside = new TH1D("Pdg_two_Outside", "Pdg_two_Outside; Id;#", 800, 0, 400);
  fHistoList_man_two_Outside.push_back(Pdg_two_Outside);
  P_reco_two_Outside = new TH1D("P_reco_two_Outside", "P_reco_two_Outside; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_two_Outside.push_back(P_reco_two_Outside);
  Pt_reco_two_Outside = new TH1D("Pt_reco_two_Outside", "Pt_reco_two_Outside; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_two_Outside.push_back(Pt_reco_two_Outside);
  Pi0InvMassReco_two_Outside =
    new TH1D("Pi0InvMassReco_two_Outside", "Pi0InvMassReco_two_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Outside.push_back(Pi0InvMassReco_two_Outside);
  EMT_InvMass_two_Outside =
    new TH1D("EMT_InvMass_two_Outside", "EMT_InvMass_two_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Outside.push_back(EMT_InvMass_two_Outside);
  Pi0_pt_vs_rap_two_Outside = new TH2D(
    "Pi0_pt_vs_rap_two_Outside", "Pi0_pt_vs_rap_two_Outside; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_two_Outside.push_back(Pi0_pt_vs_rap_two_Outside);
  Pi0_pt_vs_rap_est_two_Outside =
    new TH2D("Pi0_pt_vs_rap_est_two_Outside", "Pi0_pt_vs_rap_est_two_Outside; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_two_Outside.push_back(Pi0_pt_vs_rap_est_two_Outside);
  DalitzPi0_two_Outside =
    new TH1D("DalitzPi0_two_Outside", "DalitzPi0_two_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Outside.push_back(DalitzPi0_two_Outside);
  PhotonsPi0_two_Outside =
    new TH1D("PhotonsPi0_two_Outside", "PhotonsPi0_two_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Outside.push_back(PhotonsPi0_two_Outside);


  // Outside => onetwo
  GammaInvMassReco_onetwo_Outside =
    new TH1D("GammaInvMassReco_onetwo_Outside", "GammaInvMassReco_onetwo_Outside; invariant mass in GeV/c^{2};#", 110,
             -0.01, 0.1);
  fHistoList_man_onetwo_Outside.push_back(GammaInvMassReco_onetwo_Outside);
  GammaOpeningAngleReco_onetwo_Outside = new TH1D(
    "GammaOpeningAngleReco_onetwo_Outside", "GammaOpeningAngleReco_onetwo_Outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_onetwo_Outside.push_back(GammaOpeningAngleReco_onetwo_Outside);
  Pdg_onetwo_Outside = new TH1D("Pdg_onetwo_Outside", "Pdg_onetwo_Outside; Id;#", 800, 0, 400);
  fHistoList_man_onetwo_Outside.push_back(Pdg_onetwo_Outside);
  P_reco_onetwo_Outside = new TH1D("P_reco_onetwo_Outside", "P_reco_onetwo_Outside; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_onetwo_Outside.push_back(P_reco_onetwo_Outside);
  Pt_reco_onetwo_Outside =
    new TH1D("Pt_reco_onetwo_Outside", "Pt_reco_onetwo_Outside; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_onetwo_Outside.push_back(Pt_reco_onetwo_Outside);
  Pi0InvMassReco_onetwo_Outside = new TH1D(
    "Pi0InvMassReco_onetwo_Outside", "Pi0InvMassReco_onetwo_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Outside.push_back(Pi0InvMassReco_onetwo_Outside);
  EMT_InvMass_onetwo_Outside =
    new TH1D("EMT_InvMass_onetwo_Outside", "EMT_InvMass_onetwo_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Outside.push_back(EMT_InvMass_onetwo_Outside);
  Pi0_pt_vs_rap_onetwo_Outside =
    new TH2D("Pi0_pt_vs_rap_onetwo_Outside", "Pi0_pt_vs_rap_onetwo_Outside; rapidity y; p_{t} in GeV/c ", 90, -2., 7.,
             60, -1., 5.);
  fHistoList_man_onetwo_Outside.push_back(Pi0_pt_vs_rap_onetwo_Outside);
  Pi0_pt_vs_rap_est_onetwo_Outside =
    new TH2D("Pi0_pt_vs_rap_est_onetwo_Outside", "Pi0_pt_vs_rap_est_onetwo_Outside; rapidity y; p_{t} in GeV/c ", 10,
             0., 4., 40, 0., 4.);
  fHistoList_man_onetwo_Outside.push_back(Pi0_pt_vs_rap_est_onetwo_Outside);
  DalitzPi0_onetwo_Outside =
    new TH1D("DalitzPi0_onetwo_Outside", "DalitzPi0_onetwo_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Outside.push_back(DalitzPi0_onetwo_Outside);
  PhotonsPi0_onetwo_Outside =
    new TH1D("PhotonsPi0_onetwo_Outside", "PhotonsPi0_onetwo_Outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Outside.push_back(PhotonsPi0_onetwo_Outside);


  // Both => all
  GammaInvMassReco_all_Both =
    new TH1D("GammaInvMassReco_all_Both", "GammaInvMassReco_all_Both; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_all_Both.push_back(GammaInvMassReco_all_Both);
  GammaOpeningAngleReco_all_Both =
    new TH1D("GammaOpeningAngleReco_all_Both", "GammaOpeningAngleReco_all_Both; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_all_Both.push_back(GammaOpeningAngleReco_all_Both);
  Pdg_all_Both = new TH1D("Pdg_all_Both", "Pdg_all_Both; Id;#", 800, 0, 400);
  fHistoList_man_all_Both.push_back(Pdg_all_Both);
  P_reco_all_Both = new TH1D("P_reco_all_Both", "P_reco_all_Both; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_all_Both.push_back(P_reco_all_Both);
  Pt_reco_all_Both = new TH1D("Pt_reco_all_Both", "Pt_reco_all_Both; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_all_Both.push_back(Pt_reco_all_Both);
  Pi0InvMassReco_all_Both =
    new TH1D("Pi0InvMassReco_all_Both", "Pi0InvMassReco_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Both.push_back(Pi0InvMassReco_all_Both);
  EMT_InvMass_all_Both =
    new TH1D("EMT_InvMass_all_Both", "EMT_InvMass_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Both.push_back(EMT_InvMass_all_Both);
  Pi0_pt_vs_rap_all_Both =
    new TH2D("Pi0_pt_vs_rap_all_Both", "Pi0_pt_vs_rap_all_Both; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_all_Both.push_back(Pi0_pt_vs_rap_all_Both);
  Pi0_pt_vs_rap_est_all_Both = new TH2D(
    "Pi0_pt_vs_rap_est_all_Both", "Pi0_pt_vs_rap_est_all_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_man_all_Both.push_back(Pi0_pt_vs_rap_est_all_Both);
  DalitzPi0_all_Both =
    new TH1D("DalitzPi0_all_Both", "DalitzPi0_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Both.push_back(DalitzPi0_all_Both);
  PhotonsPi0_all_Both =
    new TH1D("PhotonsPi0_all_Both", "PhotonsPi0_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_all_Both.push_back(PhotonsPi0_all_Both);


  // Both => zero
  GammaInvMassReco_zero_Both = new TH1D("GammaInvMassReco_zero_Both",
                                        "GammaInvMassReco_zero_Both; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_zero_Both.push_back(GammaInvMassReco_zero_Both);
  GammaOpeningAngleReco_zero_Both =
    new TH1D("GammaOpeningAngleReco_zero_Both", "GammaOpeningAngleReco_zero_Both; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_zero_Both.push_back(GammaOpeningAngleReco_zero_Both);
  Pdg_zero_Both = new TH1D("Pdg_zero_Both", "Pdg_zero_Both; Id;#", 800, 0, 400);
  fHistoList_man_zero_Both.push_back(Pdg_zero_Both);
  P_reco_zero_Both = new TH1D("P_reco_zero_Both", "P_reco_zero_Both; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_zero_Both.push_back(P_reco_zero_Both);
  Pt_reco_zero_Both = new TH1D("Pt_reco_zero_Both", "Pt_reco_zero_Both; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_zero_Both.push_back(Pt_reco_zero_Both);
  Pi0InvMassReco_zero_Both =
    new TH1D("Pi0InvMassReco_zero_Both", "Pi0InvMassReco_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Both.push_back(Pi0InvMassReco_zero_Both);
  EMT_InvMass_zero_Both =
    new TH1D("EMT_InvMass_zero_Both", "EMT_InvMass_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Both.push_back(EMT_InvMass_zero_Both);
  Pi0_pt_vs_rap_zero_Both = new TH2D("Pi0_pt_vs_rap_zero_Both", "Pi0_pt_vs_rap_zero_Both; rapidity y; p_{t} in GeV/c ",
                                     90, -2., 7., 60, -1., 5.);
  fHistoList_man_zero_Both.push_back(Pi0_pt_vs_rap_zero_Both);
  Pi0_pt_vs_rap_est_zero_Both = new TH2D(
    "Pi0_pt_vs_rap_est_zero_Both", "Pi0_pt_vs_rap_est_zero_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_man_zero_Both.push_back(Pi0_pt_vs_rap_est_zero_Both);
  DalitzPi0_zero_Both =
    new TH1D("DalitzPi0_zero_Both", "DalitzPi0_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Both.push_back(DalitzPi0_zero_Both);
  PhotonsPi0_zero_Both =
    new TH1D("PhotonsPi0_zero_Both", "PhotonsPi0_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_zero_Both.push_back(PhotonsPi0_zero_Both);


  // Both => one
  GammaInvMassReco_one_Both =
    new TH1D("GammaInvMassReco_one_Both", "GammaInvMassReco_one_Both; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_one_Both.push_back(GammaInvMassReco_one_Both);
  GammaOpeningAngleReco_one_Both =
    new TH1D("GammaOpeningAngleReco_one_Both", "GammaOpeningAngleReco_one_Both; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_one_Both.push_back(GammaOpeningAngleReco_one_Both);
  Pdg_one_Both = new TH1D("Pdg_one_Both", "Pdg_one_Both; Id;#", 800, 0, 400);
  fHistoList_man_one_Both.push_back(Pdg_one_Both);
  P_reco_one_Both = new TH1D("P_reco_one_Both", "P_reco_one_Both; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_one_Both.push_back(P_reco_one_Both);
  Pt_reco_one_Both = new TH1D("Pt_reco_one_Both", "Pt_reco_one_Both; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_one_Both.push_back(Pt_reco_one_Both);
  Pi0InvMassReco_one_Both =
    new TH1D("Pi0InvMassReco_one_Both", "Pi0InvMassReco_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Both.push_back(Pi0InvMassReco_one_Both);
  EMT_InvMass_one_Both =
    new TH1D("EMT_InvMass_one_Both", "EMT_InvMass_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Both.push_back(EMT_InvMass_one_Both);
  Pi0_pt_vs_rap_one_Both =
    new TH2D("Pi0_pt_vs_rap_one_Both", "Pi0_pt_vs_rap_one_Both; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_one_Both.push_back(Pi0_pt_vs_rap_one_Both);
  Pi0_pt_vs_rap_est_one_Both = new TH2D(
    "Pi0_pt_vs_rap_est_one_Both", "Pi0_pt_vs_rap_est_one_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_man_one_Both.push_back(Pi0_pt_vs_rap_est_one_Both);
  DalitzPi0_one_Both =
    new TH1D("DalitzPi0_one_Both", "DalitzPi0_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Both.push_back(DalitzPi0_one_Both);
  PhotonsPi0_one_Both =
    new TH1D("PhotonsPi0_one_Both", "PhotonsPi0_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_one_Both.push_back(PhotonsPi0_one_Both);


  // Both => two
  GammaInvMassReco_two_Both =
    new TH1D("GammaInvMassReco_two_Both", "GammaInvMassReco_two_Both; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_two_Both.push_back(GammaInvMassReco_two_Both);
  GammaOpeningAngleReco_two_Both =
    new TH1D("GammaOpeningAngleReco_two_Both", "GammaOpeningAngleReco_two_Both; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_two_Both.push_back(GammaOpeningAngleReco_two_Both);
  Pdg_two_Both = new TH1D("Pdg_two_Both", "Pdg_two_Both; Id;#", 800, 0, 400);
  fHistoList_man_two_Both.push_back(Pdg_two_Both);
  P_reco_two_Both = new TH1D("P_reco_two_Both", "P_reco_two_Both; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_two_Both.push_back(P_reco_two_Both);
  Pt_reco_two_Both = new TH1D("Pt_reco_two_Both", "Pt_reco_two_Both; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_two_Both.push_back(Pt_reco_two_Both);
  Pi0InvMassReco_two_Both =
    new TH1D("Pi0InvMassReco_two_Both", "Pi0InvMassReco_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Both.push_back(Pi0InvMassReco_two_Both);
  EMT_InvMass_two_Both =
    new TH1D("EMT_InvMass_two_Both", "EMT_InvMass_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Both.push_back(EMT_InvMass_two_Both);
  Pi0_pt_vs_rap_two_Both =
    new TH2D("Pi0_pt_vs_rap_two_Both", "Pi0_pt_vs_rap_two_Both; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_two_Both.push_back(Pi0_pt_vs_rap_two_Both);
  Pi0_pt_vs_rap_est_two_Both = new TH2D(
    "Pi0_pt_vs_rap_est_two_Both", "Pi0_pt_vs_rap_est_two_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_man_two_Both.push_back(Pi0_pt_vs_rap_est_two_Both);
  DalitzPi0_two_Both =
    new TH1D("DalitzPi0_two_Both", "DalitzPi0_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Both.push_back(DalitzPi0_two_Both);
  PhotonsPi0_two_Both =
    new TH1D("PhotonsPi0_two_Both", "PhotonsPi0_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_two_Both.push_back(PhotonsPi0_two_Both);


  // Both => onetwo
  GammaInvMassReco_onetwo_Both = new TH1D(
    "GammaInvMassReco_onetwo_Both", "GammaInvMassReco_onetwo_Both; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_man_onetwo_Both.push_back(GammaInvMassReco_onetwo_Both);
  GammaOpeningAngleReco_onetwo_Both =
    new TH1D("GammaOpeningAngleReco_onetwo_Both", "GammaOpeningAngleReco_onetwo_Both; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_man_onetwo_Both.push_back(GammaOpeningAngleReco_onetwo_Both);
  Pdg_onetwo_Both = new TH1D("Pdg_onetwo_Both", "Pdg_onetwo_Both; Id;#", 800, 0, 400);
  fHistoList_man_onetwo_Both.push_back(Pdg_onetwo_Both);
  P_reco_onetwo_Both = new TH1D("P_reco_onetwo_Both", "P_reco_onetwo_Both; P in GeV/c^{2};#", 600, 0, 6);
  fHistoList_man_onetwo_Both.push_back(P_reco_onetwo_Both);
  Pt_reco_onetwo_Both = new TH1D("Pt_reco_onetwo_Both", "Pt_reco_onetwo_Both; P_{t} in GeV/c^{2};#", 300, 0, 3);
  fHistoList_man_onetwo_Both.push_back(Pt_reco_onetwo_Both);
  Pi0InvMassReco_onetwo_Both =
    new TH1D("Pi0InvMassReco_onetwo_Both", "Pi0InvMassReco_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Both.push_back(Pi0InvMassReco_onetwo_Both);
  EMT_InvMass_onetwo_Both =
    new TH1D("EMT_InvMass_onetwo_Both", "EMT_InvMass_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Both.push_back(EMT_InvMass_onetwo_Both);
  Pi0_pt_vs_rap_onetwo_Both = new TH2D(
    "Pi0_pt_vs_rap_onetwo_Both", "Pi0_pt_vs_rap_onetwo_Both; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_onetwo_Both.push_back(Pi0_pt_vs_rap_onetwo_Both);
  Pi0_pt_vs_rap_est_onetwo_Both =
    new TH2D("Pi0_pt_vs_rap_est_onetwo_Both", "Pi0_pt_vs_rap_est_onetwo_Both; rapidity y; p_{t} in GeV/c ", 10, 0., 4.,
             40, 0., 4.);
  fHistoList_man_onetwo_Both.push_back(Pi0_pt_vs_rap_est_onetwo_Both);
  DalitzPi0_onetwo_Both =
    new TH1D("DalitzPi0_onetwo_Both", "DalitzPi0_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Both.push_back(DalitzPi0_onetwo_Both);
  PhotonsPi0_onetwo_Both =
    new TH1D("PhotonsPi0_onetwo_Both", "PhotonsPi0_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_onetwo_Both.push_back(PhotonsPi0_onetwo_Both);


  // Both
  Pdg_vs_Distance = new TH2D("Pdg_vs_Distance", "Pdg_vs_Distance; pdg; distance in cm", 2500, 0, 2499, 500, 0, 50);
  fHistoList_man_Both.push_back(Pdg_vs_Distance);
  P_vs_Distance = new TH2D("P_vs_Distance",
                           "Distance between projected track and center of the ring (for e+ "
                           "and e-); P in GeV/c^{2}; distance in cm",
                           300, 0, 3, 300, 0, 15);
  fHistoList_man_Both.push_back(P_vs_Distance);


  // Multiplicity Target
  MultiplicityGamma_all_Target = new TH2D("MultiplicityGamma_all_Target",
                                          "MultiplicityGamma_all_Target; Nof gammas in event; invariant "
                                          "mass in GeV/c^{2};#",
                                          400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityGamma_all_Target);
  MultiplicityGamma_zero_Target = new TH2D("MultiplicityGamma_zero_Target",
                                           "MultiplicityGamma_zero_Target; Nof gammas in event; invariant "
                                           "mass in GeV/c^{2};#",
                                           400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityGamma_zero_Target);
  MultiplicityGamma_one_Target = new TH2D("MultiplicityGamma_one_Target",
                                          "MultiplicityGamma_one_Target; Nof gammas in event; invariant "
                                          "mass in GeV/c^{2};#",
                                          400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityGamma_one_Target);
  MultiplicityGamma_two_Target = new TH2D("MultiplicityGamma_two_Target",
                                          "MultiplicityGamma_two_Target; Nof gammas in event; invariant "
                                          "mass in GeV/c^{2};#",
                                          400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityGamma_two_Target);
  MultiplicityGamma_onetwo_Target = new TH2D("MultiplicityGamma_onetwo_Target",
                                             "MultiplicityGamma_onetwo_Target; Nof gammas in event; invariant "
                                             "mass in GeV/c^{2};#",
                                             400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityGamma_onetwo_Target);

  MultiplicityChargedParticles_all_Target = new TH2D("MultiplicityChargedParticles_all_Target",
                                                     "MultiplicityChargedParticles_all_Target; Nof charged particles "
                                                     "in event; invariant mass in GeV/c^{2};#",
                                                     1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityChargedParticles_all_Target);
  MultiplicityChargedParticles_zero_Target = new TH2D("MultiplicityChargedParticles_zero_Target",
                                                      "MultiplicityChargedParticles_zero_Target; Nof charged particles "
                                                      "in event; invariant mass in GeV/c^{2};#",
                                                      1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityChargedParticles_zero_Target);
  MultiplicityChargedParticles_one_Target = new TH2D("MultiplicityChargedParticles_one_Target",
                                                     "MultiplicityChargedParticles_one_Target; Nof charged particles "
                                                     "in event; invariant mass in GeV/c^{2};#",
                                                     1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityChargedParticles_one_Target);
  MultiplicityChargedParticles_two_Target = new TH2D("MultiplicityChargedParticles_two_Target",
                                                     "MultiplicityChargedParticles_two_Target; Nof charged particles "
                                                     "in event; invariant mass in GeV/c^{2};#",
                                                     1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityChargedParticles_two_Target);
  MultiplicityChargedParticles_onetwo_Target = new TH2D("MultiplicityChargedParticles_onetwo_Target",
                                                        "MultiplicityChargedParticles_onetwo_Target; Nof charged "
                                                        "particles in event; invariant mass in GeV/c^{2};#",
                                                        1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Target.push_back(MultiplicityChargedParticles_onetwo_Target);

  // Multiplicity Outside
  MultiplicityGamma_all_Outside = new TH2D("MultiplicityGamma_all_Outside",
                                           "MultiplicityGamma_all_Outside; Nof gammas in event; invariant "
                                           "mass in GeV/c^{2};#",
                                           400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityGamma_all_Outside);
  MultiplicityGamma_zero_Outside = new TH2D("MultiplicityGamma_zero_Outside",
                                            "MultiplicityGamma_zero_Outside; Nof gammas in event; invariant "
                                            "mass in GeV/c^{2};#",
                                            400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityGamma_zero_Outside);
  MultiplicityGamma_one_Outside = new TH2D("MultiplicityGamma_one_Outside",
                                           "MultiplicityGamma_one_Outside; Nof gammas in event; invariant "
                                           "mass in GeV/c^{2};#",
                                           400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityGamma_one_Outside);
  MultiplicityGamma_two_Outside = new TH2D("MultiplicityGamma_two_Outside",
                                           "MultiplicityGamma_two_Outside; Nof gammas in event; invariant "
                                           "mass in GeV/c^{2};#",
                                           400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityGamma_two_Outside);
  MultiplicityGamma_onetwo_Outside = new TH2D("MultiplicityGamma_onetwo_Outside",
                                              "MultiplicityGamma_onetwo_Outside; Nof gammas in event; invariant "
                                              "mass in GeV/c^{2};#",
                                              400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityGamma_onetwo_Outside);

  MultiplicityChargedParticles_all_Outside = new TH2D("MultiplicityChargedParticles_all_Outside",
                                                      "MultiplicityChargedParticles_all_Outside; Nof charged particles "
                                                      "in event; invariant mass in GeV/c^{2};#",
                                                      1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityChargedParticles_all_Outside);
  MultiplicityChargedParticles_zero_Outside =
    new TH2D("MultiplicityChargedParticles_zero_Outside",
             "MultiplicityChargedParticles_zero_Outside; Nof charged particles "
             "in event; invariant mass in GeV/c^{2};#",
             1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityChargedParticles_zero_Outside);
  MultiplicityChargedParticles_one_Outside = new TH2D("MultiplicityChargedParticles_one_Outside",
                                                      "MultiplicityChargedParticles_one_Outside; Nof charged particles "
                                                      "in event; invariant mass in GeV/c^{2};#",
                                                      1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityChargedParticles_one_Outside);
  MultiplicityChargedParticles_two_Outside = new TH2D("MultiplicityChargedParticles_two_Outside",
                                                      "MultiplicityChargedParticles_two_Outside; Nof charged particles "
                                                      "in event; invariant mass in GeV/c^{2};#",
                                                      1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityChargedParticles_two_Outside);
  MultiplicityChargedParticles_onetwo_Outside = new TH2D("MultiplicityChargedParticles_onetwo_Outside",
                                                         "MultiplicityChargedParticles_onetwo_Outside; Nof charged "
                                                         "particles in event; invariant mass in GeV/c^{2};#",
                                                         1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Outside.push_back(MultiplicityChargedParticles_onetwo_Outside);


  // Multiplicity Both
  MultiplicityGamma_all_Both = new TH2D("MultiplicityGamma_all_Both",
                                        "MultiplicityGamma_all_Both; Nof gammas in event; invariant mass "
                                        "in GeV/c^{2};#",
                                        400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_all_Both);
  MultiplicityGamma_zero_Both = new TH2D("MultiplicityGamma_zero_Both",
                                         "MultiplicityGamma_zero_Both; Nof gammas in event; invariant mass "
                                         "in GeV/c^{2};#",
                                         400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_zero_Both);
  MultiplicityGamma_one_Both = new TH2D("MultiplicityGamma_one_Both",
                                        "MultiplicityGamma_one_Both; Nof gammas in event; invariant mass "
                                        "in GeV/c^{2};#",
                                        400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_one_Both);
  MultiplicityGamma_two_Both = new TH2D("MultiplicityGamma_two_Both",
                                        "MultiplicityGamma_two_Both; Nof gammas in event; invariant mass "
                                        "in GeV/c^{2};#",
                                        400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_two_Both);
  MultiplicityGamma_onetwo_Both = new TH2D("MultiplicityGamma_onetwo_Both",
                                           "MultiplicityGamma_onetwo_Both; Nof gammas in event; invariant "
                                           "mass in GeV/c^{2};#",
                                           400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_onetwo_Both);

  MultiplicityChargedParticles_all_Both = new TH2D("MultiplicityChargedParticles_all_Both",
                                                   "MultiplicityChargedParticles_all_Both; Nof charged particles in "
                                                   "event; invariant mass in GeV/c^{2};#",
                                                   1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_all_Both);
  MultiplicityChargedParticles_zero_Both = new TH2D("MultiplicityChargedParticles_zero_Both",
                                                    "MultiplicityChargedParticles_zero_Both; Nof charged particles in "
                                                    "event; invariant mass in GeV/c^{2};#",
                                                    1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_zero_Both);
  MultiplicityChargedParticles_one_Both = new TH2D("MultiplicityChargedParticles_one_Both",
                                                   "MultiplicityChargedParticles_one_Both; Nof charged particles in "
                                                   "event; invariant mass in GeV/c^{2};#",
                                                   1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_one_Both);
  MultiplicityChargedParticles_two_Both = new TH2D("MultiplicityChargedParticles_two_Both",
                                                   "MultiplicityChargedParticles_two_Both; Nof charged particles in "
                                                   "event; invariant mass in GeV/c^{2};#",
                                                   1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_two_Both);
  MultiplicityChargedParticles_onetwo_Both = new TH2D("MultiplicityChargedParticles_onetwo_Both",
                                                      "MultiplicityChargedParticles_onetwo_Both; Nof charged particles "
                                                      "in event; invariant mass in GeV/c^{2};#",
                                                      1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_onetwo_Both);


  //  rap_vs_Pt for "OneTwo" and "Both" NEW => Pt step = 0.1
  fHistoList_rap_vs_pt_InM.push_back(Pi0_pt_vs_rap_est_onetwo_Both);
  rap_vs_Pt_InM_1 = new TH1D("rap_vs_Pt_InM_1",
                             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.1 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_1);
  rap_vs_Pt_InM_2 = new TH1D("rap_vs_Pt_InM_2",
                             "rapidity = (1.2-1.6)      P_{t} = (0.1-0.2 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_2);
  rap_vs_Pt_InM_3 = new TH1D("rap_vs_Pt_InM_3",
                             "rapidity = (1.2-1.6)      P_{t} = (0.2-0.3 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_3);
  rap_vs_Pt_InM_4 = new TH1D("rap_vs_Pt_InM_4",
                             "rapidity = (1.2-1.6)      P_{t} = (0.3-0.4 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_4);
  rap_vs_Pt_InM_5 = new TH1D("rap_vs_Pt_InM_5",
                             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.5 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_5);
  rap_vs_Pt_InM_6 = new TH1D("rap_vs_Pt_InM_6",
                             "rapidity = (1.2-1.6)      P_{t} = (0.5-0.6 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_6);
  rap_vs_Pt_InM_7 = new TH1D("rap_vs_Pt_InM_7",
                             "rapidity = (1.2-1.6)      P_{t} = (0.6-0.7 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_7);
  rap_vs_Pt_InM_8 = new TH1D("rap_vs_Pt_InM_8",
                             "rapidity = (1.2-1.6)      P_{t} = (0.7-0.8 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_8);
  rap_vs_Pt_InM_9 = new TH1D("rap_vs_Pt_InM_9",
                             "rapidity = (1.2-1.6)      P_{t} = (0.8-0.9 "
                             "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                             1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_9);
  rap_vs_Pt_InM_10 = new TH1D("rap_vs_Pt_InM_10",
                              "rapidity = (1.2-1.6)      P_{t} = (0.9-1.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_10);
  rap_vs_Pt_InM_11 = new TH1D("rap_vs_Pt_InM_11",
                              "rapidity = (1.2-1.6)      P_{t} = (1.0-1.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_11);
  rap_vs_Pt_InM_12 = new TH1D("rap_vs_Pt_InM_12",
                              "rapidity = (1.2-1.6)      P_{t} = (1.1-1.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_12);
  rap_vs_Pt_InM_13 = new TH1D("rap_vs_Pt_InM_13",
                              "rapidity = (1.2-1.6)      P_{t} = (1.2-1.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_13);
  rap_vs_Pt_InM_14 = new TH1D("rap_vs_Pt_InM_14",
                              "rapidity = (1.2-1.6)      P_{t} = (1.3-1.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_14);
  rap_vs_Pt_InM_15 = new TH1D("rap_vs_Pt_InM_15",
                              "rapidity = (1.2-1.6)      P_{t} = (1.4-1.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_15);
  rap_vs_Pt_InM_16 = new TH1D("rap_vs_Pt_InM_16",
                              "rapidity = (1.2-1.6)      P_{t} = (1.5-1.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_16);
  rap_vs_Pt_InM_17 = new TH1D("rap_vs_Pt_InM_17",
                              "rapidity = (1.2-1.6)      P_{t} = (1.6-1.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_17);
  rap_vs_Pt_InM_18 = new TH1D("rap_vs_Pt_InM_18",
                              "rapidity = (1.2-1.6)      P_{t} = (1.7-1.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_18);
  rap_vs_Pt_InM_19 = new TH1D("rap_vs_Pt_InM_19",
                              "rapidity = (1.2-1.6)      P_{t} = (1.8-1.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_19);
  rap_vs_Pt_InM_20 = new TH1D("rap_vs_Pt_InM_20",
                              "rapidity = (1.2-1.6)      P_{t} = (1.9-2.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_20);
  rap_vs_Pt_InM_21 = new TH1D("rap_vs_Pt_InM_21",
                              "rapidity = (1.6-2.0)      P_{t} = (0.0-0.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_21);
  rap_vs_Pt_InM_22 = new TH1D("rap_vs_Pt_InM_22",
                              "rapidity = (1.6-2.0)      P_{t} = (0.1-0.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_22);
  rap_vs_Pt_InM_23 = new TH1D("rap_vs_Pt_InM_23",
                              "rapidity = (1.6-2.0)      P_{t} = (0.2-0.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_23);
  rap_vs_Pt_InM_24 = new TH1D("rap_vs_Pt_InM_24",
                              "rapidity = (1.6-2.0)      P_{t} = (0.3-0.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_24);
  rap_vs_Pt_InM_25 = new TH1D("rap_vs_Pt_InM_25",
                              "rapidity = (1.6-2.0)      P_{t} = (0.4-0.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_25);
  rap_vs_Pt_InM_26 = new TH1D("rap_vs_Pt_InM_26",
                              "rapidity = (1.6-2.0)      P_{t} = (0.5-0.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_26);
  rap_vs_Pt_InM_27 = new TH1D("rap_vs_Pt_InM_27",
                              "rapidity = (1.6-2.0)      P_{t} = (0.6-0.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_27);
  rap_vs_Pt_InM_28 = new TH1D("rap_vs_Pt_InM_28",
                              "rapidity = (1.6-2.0)      P_{t} = (0.7-0.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_28);
  rap_vs_Pt_InM_29 = new TH1D("rap_vs_Pt_InM_29",
                              "rapidity = (1.6-2.0)      P_{t} = (0.8-0.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_29);
  rap_vs_Pt_InM_30 = new TH1D("rap_vs_Pt_InM_30",
                              "rapidity = (1.6-2.0)      P_{t} = (0.9-1.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_30);
  rap_vs_Pt_InM_31 = new TH1D("rap_vs_Pt_InM_31",
                              "rapidity = (1.6-2.0)      P_{t} = (1.0-1.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_31);
  rap_vs_Pt_InM_32 = new TH1D("rap_vs_Pt_InM_32",
                              "rapidity = (1.6-2.0)      P_{t} = (1.1-1.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_32);
  rap_vs_Pt_InM_33 = new TH1D("rap_vs_Pt_InM_33",
                              "rapidity = (1.6-2.0)      P_{t} = (1.2-1.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_33);
  rap_vs_Pt_InM_34 = new TH1D("rap_vs_Pt_InM_34",
                              "rapidity = (1.6-2.0)      P_{t} = (1.3-1.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_34);
  rap_vs_Pt_InM_35 = new TH1D("rap_vs_Pt_InM_35",
                              "rapidity = (1.6-2.0)      P_{t} = (1.4-1.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_35);
  rap_vs_Pt_InM_36 = new TH1D("rap_vs_Pt_InM_36",
                              "rapidity = (1.6-2.0)      P_{t} = (1.5-1.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_36);
  rap_vs_Pt_InM_37 = new TH1D("rap_vs_Pt_InM_37",
                              "rapidity = (1.6-2.0)      P_{t} = (1.6-1.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_37);
  rap_vs_Pt_InM_38 = new TH1D("rap_vs_Pt_InM_38",
                              "rapidity = (1.6-2.0)      P_{t} = (1.7-1.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_38);
  rap_vs_Pt_InM_39 = new TH1D("rap_vs_Pt_InM_39",
                              "rapidity = (1.6-2.0)      P_{t} = (1.8-1.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_39);
  rap_vs_Pt_InM_40 = new TH1D("rap_vs_Pt_InM_40",
                              "rapidity = (1.6-2.0)      P_{t} = (1.9-2.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_40);
  rap_vs_Pt_InM_41 = new TH1D("rap_vs_Pt_InM_41",
                              "rapidity = (2.0-2.4)      P_{t} = (0.0-0.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_41);
  rap_vs_Pt_InM_42 = new TH1D("rap_vs_Pt_InM_42",
                              "rapidity = (2.0-2.4)      P_{t} = (0.1-0.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_42);
  rap_vs_Pt_InM_43 = new TH1D("rap_vs_Pt_InM_43",
                              "rapidity = (2.0-2.4)      P_{t} = (0.2-0.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_43);
  rap_vs_Pt_InM_44 = new TH1D("rap_vs_Pt_InM_44",
                              "rapidity = (2.0-2.4)      P_{t} = (0.3-0.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_44);
  rap_vs_Pt_InM_45 = new TH1D("rap_vs_Pt_InM_45",
                              "rapidity = (2.0-2.4)      P_{t} = (0.4-0.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_45);
  rap_vs_Pt_InM_46 = new TH1D("rap_vs_Pt_InM_46",
                              "rapidity = (2.0-2.4)      P_{t} = (0.5-0.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_46);
  rap_vs_Pt_InM_47 = new TH1D("rap_vs_Pt_InM_47",
                              "rapidity = (2.0-2.4)      P_{t} = (0.6-0.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_47);
  rap_vs_Pt_InM_48 = new TH1D("rap_vs_Pt_InM_48",
                              "rapidity = (2.0-2.4)      P_{t} = (0.7-0.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_48);
  rap_vs_Pt_InM_49 = new TH1D("rap_vs_Pt_InM_49",
                              "rapidity = (2.0-2.4)      P_{t} = (0.8-0.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_49);
  rap_vs_Pt_InM_50 = new TH1D("rap_vs_Pt_InM_50",
                              "rapidity = (2.0-2.4)      P_{t} = (0.9-1.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_50);
  rap_vs_Pt_InM_51 = new TH1D("rap_vs_Pt_InM_51",
                              "rapidity = (2.0-2.4)      P_{t} = (1.0-1.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_51);
  rap_vs_Pt_InM_52 = new TH1D("rap_vs_Pt_InM_52",
                              "rapidity = (2.0-2.4)      P_{t} = (1.1-1.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_52);
  rap_vs_Pt_InM_53 = new TH1D("rap_vs_Pt_InM_53",
                              "rapidity = (2.0-2.4)      P_{t} = (1.2-1.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_53);
  rap_vs_Pt_InM_54 = new TH1D("rap_vs_Pt_InM_54",
                              "rapidity = (2.0-2.4)      P_{t} = (1.3-1.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_54);
  rap_vs_Pt_InM_55 = new TH1D("rap_vs_Pt_InM_55",
                              "rapidity = (2.0-2.4)      P_{t} = (1.4-1.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_55);
  rap_vs_Pt_InM_56 = new TH1D("rap_vs_Pt_InM_56",
                              "rapidity = (2.0-2.4)      P_{t} = (1.5-1.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_56);
  rap_vs_Pt_InM_57 = new TH1D("rap_vs_Pt_InM_57",
                              "rapidity = (2.0-2.4)      P_{t} = (1.6-1.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_57);
  rap_vs_Pt_InM_58 = new TH1D("rap_vs_Pt_InM_58",
                              "rapidity = (2.0-2.4)      P_{t} = (1.7-1.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_58);
  rap_vs_Pt_InM_59 = new TH1D("rap_vs_Pt_InM_59",
                              "rapidity = (2.0-2.4)      P_{t} = (1.8-1.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_59);
  rap_vs_Pt_InM_60 = new TH1D("rap_vs_Pt_InM_60",
                              "rapidity = (2.0-2.4)      P_{t} = (1.9-2.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_60);
  rap_vs_Pt_InM_61 = new TH1D("rap_vs_Pt_InM_61",
                              "rapidity = (2.4-2.8)      P_{t} = (0.0-0.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_61);
  rap_vs_Pt_InM_62 = new TH1D("rap_vs_Pt_InM_62",
                              "rapidity = (2.4-2.8)      P_{t} = (0.1-0.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_62);
  rap_vs_Pt_InM_63 = new TH1D("rap_vs_Pt_InM_63",
                              "rapidity = (2.4-2.8)      P_{t} = (0.2-0.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_63);
  rap_vs_Pt_InM_64 = new TH1D("rap_vs_Pt_InM_64",
                              "rapidity = (2.4-2.8)      P_{t} = (0.3-0.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_64);
  rap_vs_Pt_InM_65 = new TH1D("rap_vs_Pt_InM_65",
                              "rapidity = (2.4-2.8)      P_{t} = (0.4-0.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_65);
  rap_vs_Pt_InM_66 = new TH1D("rap_vs_Pt_InM_66",
                              "rapidity = (2.4-2.8)      P_{t} = (0.5-0.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_66);
  rap_vs_Pt_InM_67 = new TH1D("rap_vs_Pt_InM_67",
                              "rapidity = (2.4-2.8)      P_{t} = (0.6-0.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_67);
  rap_vs_Pt_InM_68 = new TH1D("rap_vs_Pt_InM_68",
                              "rapidity = (2.4-2.8)      P_{t} = (0.7-0.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_68);
  rap_vs_Pt_InM_69 = new TH1D("rap_vs_Pt_InM_69",
                              "rapidity = (2.4-2.8)      P_{t} = (0.8-0.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_69);
  rap_vs_Pt_InM_70 = new TH1D("rap_vs_Pt_InM_70",
                              "rapidity = (2.4-2.8)      P_{t} = (0.9-1.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_70);
  rap_vs_Pt_InM_71 = new TH1D("rap_vs_Pt_InM_71",
                              "rapidity = (2.4-2.8)      P_{t} = (1.0-1.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_71);
  rap_vs_Pt_InM_72 = new TH1D("rap_vs_Pt_InM_72",
                              "rapidity = (2.4-2.8)      P_{t} = (1.1-1.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_72);
  rap_vs_Pt_InM_73 = new TH1D("rap_vs_Pt_InM_73",
                              "rapidity = (2.4-2.8)      P_{t} = (1.2-1.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_73);
  rap_vs_Pt_InM_74 = new TH1D("rap_vs_Pt_InM_74",
                              "rapidity = (2.4-2.8)      P_{t} = (1.3-1.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_74);
  rap_vs_Pt_InM_75 = new TH1D("rap_vs_Pt_InM_75",
                              "rapidity = (2.4-2.8)      P_{t} = (1.4-1.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_75);
  rap_vs_Pt_InM_76 = new TH1D("rap_vs_Pt_InM_76",
                              "rapidity = (2.4-2.8)      P_{t} = (1.5-1.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_76);
  rap_vs_Pt_InM_81 = new TH1D("rap_vs_Pt_InM_81",
                              "rapidity = (2.8-3.2)      P_{t} = (0.0-0.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_81);
  rap_vs_Pt_InM_82 = new TH1D("rap_vs_Pt_InM_82",
                              "rapidity = (2.8-3.2)      P_{t} = (0.1-0.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_82);
  rap_vs_Pt_InM_83 = new TH1D("rap_vs_Pt_InM_83",
                              "rapidity = (2.8-3.2)      P_{t} = (0.2-0.3 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_83);
  rap_vs_Pt_InM_84 = new TH1D("rap_vs_Pt_InM_84",
                              "rapidity = (2.8-3.2)      P_{t} = (0.3-0.4 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_84);
  rap_vs_Pt_InM_85 = new TH1D("rap_vs_Pt_InM_85",
                              "rapidity = (2.8-3.2)      P_{t} = (0.4-0.5 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_85);
  rap_vs_Pt_InM_86 = new TH1D("rap_vs_Pt_InM_86",
                              "rapidity = (2.8-3.2)      P_{t} = (0.5-0.6 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_86);
  rap_vs_Pt_InM_87 = new TH1D("rap_vs_Pt_InM_87",
                              "rapidity = (2.8-3.2)      P_{t} = (0.6-0.7 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_87);
  rap_vs_Pt_InM_88 = new TH1D("rap_vs_Pt_InM_88",
                              "rapidity = (2.8-3.2)      P_{t} = (0.7-0.8 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_88);
  rap_vs_Pt_InM_89 = new TH1D("rap_vs_Pt_InM_89",
                              "rapidity = (2.8-3.2)      P_{t} = (0.8-0.9 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_89);
  rap_vs_Pt_InM_90 = new TH1D("rap_vs_Pt_InM_90",
                              "rapidity = (2.8-3.2)      P_{t} = (0.9-1.0 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_90);
  rap_vs_Pt_InM_91 = new TH1D("rap_vs_Pt_InM_91",
                              "rapidity = (2.8-3.2)      P_{t} = (1.0-1.1 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_91);
  rap_vs_Pt_InM_92 = new TH1D("rap_vs_Pt_InM_92",
                              "rapidity = (2.8-3.2)      P_{t} = (1.1-1.2 "
                              "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                              1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_92);
  rap_vs_Pt_InM_101 = new TH1D("rap_vs_Pt_InM_101",
                               "rapidity = (3.2-3.6)      P_{t} = (0.0-0.1 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_101);
  rap_vs_Pt_InM_102 = new TH1D("rap_vs_Pt_InM_102",
                               "rapidity = (3.2-3.6)      P_{t} = (0.1-0.2 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_102);
  rap_vs_Pt_InM_103 = new TH1D("rap_vs_Pt_InM_103",
                               "rapidity = (3.2-3.6)      P_{t} = (0.2-0.3 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_103);
  rap_vs_Pt_InM_104 = new TH1D("rap_vs_Pt_InM_104",
                               "rapidity = (3.2-3.6)      P_{t} = (0.3-0.4 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_104);
  rap_vs_Pt_InM_105 = new TH1D("rap_vs_Pt_InM_105",
                               "rapidity = (3.2-3.6)      P_{t} = (0.4-0.5 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_105);
  rap_vs_Pt_InM_106 = new TH1D("rap_vs_Pt_InM_106",
                               "rapidity = (3.2-3.6)      P_{t} = (0.5-0.6 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_106);
  rap_vs_Pt_InM_107 = new TH1D("rap_vs_Pt_InM_107",
                               "rapidity = (3.2-3.6)      P_{t} = (0.6-0.7 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_107);
  rap_vs_Pt_InM_108 = new TH1D("rap_vs_Pt_InM_108",
                               "rapidity = (3.2-3.6)      P_{t} = (0.7-0.8 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_108);
  rap_vs_Pt_InM_111 = new TH1D("rap_vs_Pt_InM_111",
                               "rapidity = (3.6-4.0)      P_{t} = (0.0-0.1 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_111);
  rap_vs_Pt_InM_112 = new TH1D("rap_vs_Pt_InM_112",
                               "rapidity = (3.6-4.0)      P_{t} = (0.1-0.2 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_112);
  rap_vs_Pt_InM_113 = new TH1D("rap_vs_Pt_InM_113",
                               "rapidity = (3.6-4.0)      P_{t} = (0.2-0.3 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_113);
  rap_vs_Pt_InM_114 = new TH1D("rap_vs_Pt_InM_114",
                               "rapidity = (3.6-4.0)      P_{t} = (0.3-0.4 "
                               "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                               1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_114);

  rap_vs_Pt_InM_mixing_1 = new TH1D("rap_vs_Pt_InM_mixing_1",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_1);
  rap_vs_Pt_InM_mixing_2 = new TH1D("rap_vs_Pt_InM_mixing_2",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_2);
  rap_vs_Pt_InM_mixing_3 = new TH1D("rap_vs_Pt_InM_mixing_3",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_3);
  rap_vs_Pt_InM_mixing_4 = new TH1D("rap_vs_Pt_InM_mixing_4",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_4);
  rap_vs_Pt_InM_mixing_5 = new TH1D("rap_vs_Pt_InM_mixing_5",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_5);
  rap_vs_Pt_InM_mixing_6 = new TH1D("rap_vs_Pt_InM_mixing_6",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_6);
  rap_vs_Pt_InM_mixing_7 = new TH1D("rap_vs_Pt_InM_mixing_7",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_7);
  rap_vs_Pt_InM_mixing_8 = new TH1D("rap_vs_Pt_InM_mixing_8",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_8);
  rap_vs_Pt_InM_mixing_9 = new TH1D("rap_vs_Pt_InM_mixing_9",
                                    "rapidity = (1.2-1.6)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                    "mass in GeV/c^{2};#",
                                    1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_9);
  rap_vs_Pt_InM_mixing_10 = new TH1D("rap_vs_Pt_InM_mixing_10",
                                     "rapidity = (1.2-1.6)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_10);
  rap_vs_Pt_InM_mixing_11 = new TH1D("rap_vs_Pt_InM_mixing_11",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_11);
  rap_vs_Pt_InM_mixing_12 = new TH1D("rap_vs_Pt_InM_mixing_12",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_12);
  rap_vs_Pt_InM_mixing_13 = new TH1D("rap_vs_Pt_InM_mixing_13",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_13);
  rap_vs_Pt_InM_mixing_14 = new TH1D("rap_vs_Pt_InM_mixing_14",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_14);
  rap_vs_Pt_InM_mixing_15 = new TH1D("rap_vs_Pt_InM_mixing_15",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_15);
  rap_vs_Pt_InM_mixing_16 = new TH1D("rap_vs_Pt_InM_mixing_16",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_16);
  rap_vs_Pt_InM_mixing_17 = new TH1D("rap_vs_Pt_InM_mixing_17",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_17);
  rap_vs_Pt_InM_mixing_18 = new TH1D("rap_vs_Pt_InM_mixing_18",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_18);
  rap_vs_Pt_InM_mixing_19 = new TH1D("rap_vs_Pt_InM_mixing_19",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_19);
  rap_vs_Pt_InM_mixing_20 = new TH1D("rap_vs_Pt_InM_mixing_20",
                                     "rapidity = (1.2-1.6)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_20);
  rap_vs_Pt_InM_mixing_21 = new TH1D("rap_vs_Pt_InM_mixing_21",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_21);
  rap_vs_Pt_InM_mixing_22 = new TH1D("rap_vs_Pt_InM_mixing_22",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_22);
  rap_vs_Pt_InM_mixing_23 = new TH1D("rap_vs_Pt_InM_mixing_23",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_23);
  rap_vs_Pt_InM_mixing_24 = new TH1D("rap_vs_Pt_InM_mixing_24",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_24);
  rap_vs_Pt_InM_mixing_25 = new TH1D("rap_vs_Pt_InM_mixing_25",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_25);
  rap_vs_Pt_InM_mixing_26 = new TH1D("rap_vs_Pt_InM_mixing_26",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_26);
  rap_vs_Pt_InM_mixing_27 = new TH1D("rap_vs_Pt_InM_mixing_27",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_27);
  rap_vs_Pt_InM_mixing_28 = new TH1D("rap_vs_Pt_InM_mixing_28",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_28);
  rap_vs_Pt_InM_mixing_29 = new TH1D("rap_vs_Pt_InM_mixing_29",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_29);
  rap_vs_Pt_InM_mixing_30 = new TH1D("rap_vs_Pt_InM_mixing_30",
                                     "rapidity = (1.6-2.0)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_30);
  rap_vs_Pt_InM_mixing_31 = new TH1D("rap_vs_Pt_InM_mixing_31",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_31);
  rap_vs_Pt_InM_mixing_32 = new TH1D("rap_vs_Pt_InM_mixing_32",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_32);
  rap_vs_Pt_InM_mixing_33 = new TH1D("rap_vs_Pt_InM_mixing_33",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_33);
  rap_vs_Pt_InM_mixing_34 = new TH1D("rap_vs_Pt_InM_mixing_34",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_34);
  rap_vs_Pt_InM_mixing_35 = new TH1D("rap_vs_Pt_InM_mixing_35",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_35);
  rap_vs_Pt_InM_mixing_36 = new TH1D("rap_vs_Pt_InM_mixing_36",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_36);
  rap_vs_Pt_InM_mixing_37 = new TH1D("rap_vs_Pt_InM_mixing_37",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_37);
  rap_vs_Pt_InM_mixing_38 = new TH1D("rap_vs_Pt_InM_mixing_38",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_38);
  rap_vs_Pt_InM_mixing_39 = new TH1D("rap_vs_Pt_InM_mixing_39",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_39);
  rap_vs_Pt_InM_mixing_40 = new TH1D("rap_vs_Pt_InM_mixing_40",
                                     "rapidity = (1.6-2.0)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_40);
  rap_vs_Pt_InM_mixing_41 = new TH1D("rap_vs_Pt_InM_mixing_41",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_41);
  rap_vs_Pt_InM_mixing_42 = new TH1D("rap_vs_Pt_InM_mixing_42",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_42);
  rap_vs_Pt_InM_mixing_43 = new TH1D("rap_vs_Pt_InM_mixing_43",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_43);
  rap_vs_Pt_InM_mixing_44 = new TH1D("rap_vs_Pt_InM_mixing_44",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_44);
  rap_vs_Pt_InM_mixing_45 = new TH1D("rap_vs_Pt_InM_mixing_45",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_45);
  rap_vs_Pt_InM_mixing_46 = new TH1D("rap_vs_Pt_InM_mixing_46",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_46);
  rap_vs_Pt_InM_mixing_47 = new TH1D("rap_vs_Pt_InM_mixing_47",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_47);
  rap_vs_Pt_InM_mixing_48 = new TH1D("rap_vs_Pt_InM_mixing_48",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_48);
  rap_vs_Pt_InM_mixing_49 = new TH1D("rap_vs_Pt_InM_mixing_49",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_49);
  rap_vs_Pt_InM_mixing_50 = new TH1D("rap_vs_Pt_InM_mixing_50",
                                     "rapidity = (2.0-2.4)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_50);
  rap_vs_Pt_InM_mixing_51 = new TH1D("rap_vs_Pt_InM_mixing_51",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_51);
  rap_vs_Pt_InM_mixing_52 = new TH1D("rap_vs_Pt_InM_mixing_52",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_52);
  rap_vs_Pt_InM_mixing_53 = new TH1D("rap_vs_Pt_InM_mixing_53",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_53);
  rap_vs_Pt_InM_mixing_54 = new TH1D("rap_vs_Pt_InM_mixing_54",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_54);
  rap_vs_Pt_InM_mixing_55 = new TH1D("rap_vs_Pt_InM_mixing_55",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_55);
  rap_vs_Pt_InM_mixing_56 = new TH1D("rap_vs_Pt_InM_mixing_56",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_56);
  rap_vs_Pt_InM_mixing_57 = new TH1D("rap_vs_Pt_InM_mixing_57",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_57);
  rap_vs_Pt_InM_mixing_58 = new TH1D("rap_vs_Pt_InM_mixing_58",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_58);
  rap_vs_Pt_InM_mixing_59 = new TH1D("rap_vs_Pt_InM_mixing_59",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_59);
  rap_vs_Pt_InM_mixing_60 = new TH1D("rap_vs_Pt_InM_mixing_60",
                                     "rapidity = (2.0-2.4)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_60);
  rap_vs_Pt_InM_mixing_61 = new TH1D("rap_vs_Pt_InM_mixing_61",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_61);
  rap_vs_Pt_InM_mixing_62 = new TH1D("rap_vs_Pt_InM_mixing_62",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_62);
  rap_vs_Pt_InM_mixing_63 = new TH1D("rap_vs_Pt_InM_mixing_63",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_63);
  rap_vs_Pt_InM_mixing_64 = new TH1D("rap_vs_Pt_InM_mixing_64",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_64);
  rap_vs_Pt_InM_mixing_65 = new TH1D("rap_vs_Pt_InM_mixing_65",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_65);
  rap_vs_Pt_InM_mixing_66 = new TH1D("rap_vs_Pt_InM_mixing_66",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_66);
  rap_vs_Pt_InM_mixing_67 = new TH1D("rap_vs_Pt_InM_mixing_67",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_67);
  rap_vs_Pt_InM_mixing_68 = new TH1D("rap_vs_Pt_InM_mixing_68",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_68);
  rap_vs_Pt_InM_mixing_69 = new TH1D("rap_vs_Pt_InM_mixing_69",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_69);
  rap_vs_Pt_InM_mixing_70 = new TH1D("rap_vs_Pt_InM_mixing_70",
                                     "rapidity = (2.4-2.8)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_70);
  rap_vs_Pt_InM_mixing_71 = new TH1D("rap_vs_Pt_InM_mixing_71",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_71);
  rap_vs_Pt_InM_mixing_72 = new TH1D("rap_vs_Pt_InM_mixing_72",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_72);
  rap_vs_Pt_InM_mixing_73 = new TH1D("rap_vs_Pt_InM_mixing_73",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_73);
  rap_vs_Pt_InM_mixing_74 = new TH1D("rap_vs_Pt_InM_mixing_74",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_74);
  rap_vs_Pt_InM_mixing_75 = new TH1D("rap_vs_Pt_InM_mixing_75",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_75);
  rap_vs_Pt_InM_mixing_76 = new TH1D("rap_vs_Pt_InM_mixing_76",
                                     "rapidity = (2.4-2.8)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_76);
  rap_vs_Pt_InM_mixing_81 = new TH1D("rap_vs_Pt_InM_mixing_81",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_81);
  rap_vs_Pt_InM_mixing_82 = new TH1D("rap_vs_Pt_InM_mixing_82",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_82);
  rap_vs_Pt_InM_mixing_83 = new TH1D("rap_vs_Pt_InM_mixing_83",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_83);
  rap_vs_Pt_InM_mixing_84 = new TH1D("rap_vs_Pt_InM_mixing_84",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_84);
  rap_vs_Pt_InM_mixing_85 = new TH1D("rap_vs_Pt_InM_mixing_85",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_85);
  rap_vs_Pt_InM_mixing_86 = new TH1D("rap_vs_Pt_InM_mixing_86",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_86);
  rap_vs_Pt_InM_mixing_87 = new TH1D("rap_vs_Pt_InM_mixing_87",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_87);
  rap_vs_Pt_InM_mixing_88 = new TH1D("rap_vs_Pt_InM_mixing_88",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_88);
  rap_vs_Pt_InM_mixing_89 = new TH1D("rap_vs_Pt_InM_mixing_89",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_89);
  rap_vs_Pt_InM_mixing_90 = new TH1D("rap_vs_Pt_InM_mixing_90",
                                     "rapidity = (2.8-3.2)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_90);
  rap_vs_Pt_InM_mixing_91 = new TH1D("rap_vs_Pt_InM_mixing_91",
                                     "rapidity = (2.8-3.2)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_91);
  rap_vs_Pt_InM_mixing_92 = new TH1D("rap_vs_Pt_InM_mixing_92",
                                     "rapidity = (2.8-3.2)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                     "mass in GeV/c^{2};#",
                                     1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_92);
  rap_vs_Pt_InM_mixing_101 = new TH1D("rap_vs_Pt_InM_mixing_101",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_101);
  rap_vs_Pt_InM_mixing_102 = new TH1D("rap_vs_Pt_InM_mixing_102",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_102);
  rap_vs_Pt_InM_mixing_103 = new TH1D("rap_vs_Pt_InM_mixing_103",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_103);
  rap_vs_Pt_InM_mixing_104 = new TH1D("rap_vs_Pt_InM_mixing_104",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_104);
  rap_vs_Pt_InM_mixing_105 = new TH1D("rap_vs_Pt_InM_mixing_105",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_105);
  rap_vs_Pt_InM_mixing_106 = new TH1D("rap_vs_Pt_InM_mixing_106",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_106);
  rap_vs_Pt_InM_mixing_107 = new TH1D("rap_vs_Pt_InM_mixing_107",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_107);
  rap_vs_Pt_InM_mixing_108 = new TH1D("rap_vs_Pt_InM_mixing_108",
                                      "rapidity = (3.2-3.6)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_108);
  rap_vs_Pt_InM_mixing_111 = new TH1D("rap_vs_Pt_InM_mixing_111",
                                      "rapidity = (3.6-4.0)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_111);
  rap_vs_Pt_InM_mixing_112 = new TH1D("rap_vs_Pt_InM_mixing_112",
                                      "rapidity = (3.6-4.0)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_112);
  rap_vs_Pt_InM_mixing_113 = new TH1D("rap_vs_Pt_InM_mixing_113",
                                      "rapidity = (3.6-4.0)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_113);
  rap_vs_Pt_InM_mixing_114 = new TH1D("rap_vs_Pt_InM_mixing_114",
                                      "rapidity = (3.6-4.0)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                      "mass in GeV/c^{2};#",
                                      1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM.push_back(rap_vs_Pt_InM_mixing_114);

  //  rap_vs_Pt for "All" and "Both" NEW => Pt step = 0.1
  fHistoList_rap_vs_pt_InM_all.push_back(Pi0_pt_vs_rap_est_all_Both);
  rap_vs_Pt_InM_all_1 = new TH1D("rap_vs_Pt_InM_all_1",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.0-0.1 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_1);
  rap_vs_Pt_InM_all_2 = new TH1D("rap_vs_Pt_InM_all_2",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.1-0.2 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_2);
  rap_vs_Pt_InM_all_3 = new TH1D("rap_vs_Pt_InM_all_3",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.2-0.3 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_3);
  rap_vs_Pt_InM_all_4 = new TH1D("rap_vs_Pt_InM_all_4",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.3-0.4 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_4);
  rap_vs_Pt_InM_all_5 = new TH1D("rap_vs_Pt_InM_all_5",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.4-0.5 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_5);
  rap_vs_Pt_InM_all_6 = new TH1D("rap_vs_Pt_InM_all_6",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.5-0.6 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_6);
  rap_vs_Pt_InM_all_7 = new TH1D("rap_vs_Pt_InM_all_7",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.6-0.7 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_7);
  rap_vs_Pt_InM_all_8 = new TH1D("rap_vs_Pt_InM_all_8",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.7-0.8 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_8);
  rap_vs_Pt_InM_all_9 = new TH1D("rap_vs_Pt_InM_all_9",
                                 "rapidity = (1.2-1.6)      P_{t} = (0.8-0.9 "
                                 "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                 1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_9);
  rap_vs_Pt_InM_all_10 = new TH1D("rap_vs_Pt_InM_all_10",
                                  "rapidity = (1.2-1.6)      P_{t} = (0.9-1.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_10);
  rap_vs_Pt_InM_all_11 = new TH1D("rap_vs_Pt_InM_all_11",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.0-1.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_11);
  rap_vs_Pt_InM_all_12 = new TH1D("rap_vs_Pt_InM_all_12",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.1-1.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_12);
  rap_vs_Pt_InM_all_13 = new TH1D("rap_vs_Pt_InM_all_13",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.2-1.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_13);
  rap_vs_Pt_InM_all_14 = new TH1D("rap_vs_Pt_InM_all_14",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.3-1.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_14);
  rap_vs_Pt_InM_all_15 = new TH1D("rap_vs_Pt_InM_all_15",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.4-1.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_15);
  rap_vs_Pt_InM_all_16 = new TH1D("rap_vs_Pt_InM_all_16",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.5-1.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_16);
  rap_vs_Pt_InM_all_17 = new TH1D("rap_vs_Pt_InM_all_17",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.6-1.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_17);
  rap_vs_Pt_InM_all_18 = new TH1D("rap_vs_Pt_InM_all_18",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.7-1.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_18);
  rap_vs_Pt_InM_all_19 = new TH1D("rap_vs_Pt_InM_all_19",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.8-1.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_19);
  rap_vs_Pt_InM_all_20 = new TH1D("rap_vs_Pt_InM_all_20",
                                  "rapidity = (1.2-1.6)      P_{t} = (1.9-2.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_20);
  rap_vs_Pt_InM_all_21 = new TH1D("rap_vs_Pt_InM_all_21",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.0-0.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_21);
  rap_vs_Pt_InM_all_22 = new TH1D("rap_vs_Pt_InM_all_22",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.1-0.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_22);
  rap_vs_Pt_InM_all_23 = new TH1D("rap_vs_Pt_InM_all_23",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.2-0.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_23);
  rap_vs_Pt_InM_all_24 = new TH1D("rap_vs_Pt_InM_all_24",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.3-0.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_24);
  rap_vs_Pt_InM_all_25 = new TH1D("rap_vs_Pt_InM_all_25",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.4-0.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_25);
  rap_vs_Pt_InM_all_26 = new TH1D("rap_vs_Pt_InM_all_26",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.5-0.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_26);
  rap_vs_Pt_InM_all_27 = new TH1D("rap_vs_Pt_InM_all_27",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.6-0.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_27);
  rap_vs_Pt_InM_all_28 = new TH1D("rap_vs_Pt_InM_all_28",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.7-0.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_28);
  rap_vs_Pt_InM_all_29 = new TH1D("rap_vs_Pt_InM_all_29",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.8-0.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_29);
  rap_vs_Pt_InM_all_30 = new TH1D("rap_vs_Pt_InM_all_30",
                                  "rapidity = (1.6-2.0)      P_{t} = (0.9-1.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_30);
  rap_vs_Pt_InM_all_31 = new TH1D("rap_vs_Pt_InM_all_31",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.0-1.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_31);
  rap_vs_Pt_InM_all_32 = new TH1D("rap_vs_Pt_InM_all_32",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.1-1.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_32);
  rap_vs_Pt_InM_all_33 = new TH1D("rap_vs_Pt_InM_all_33",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.2-1.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_33);
  rap_vs_Pt_InM_all_34 = new TH1D("rap_vs_Pt_InM_all_34",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.3-1.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_34);
  rap_vs_Pt_InM_all_35 = new TH1D("rap_vs_Pt_InM_all_35",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.4-1.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_35);
  rap_vs_Pt_InM_all_36 = new TH1D("rap_vs_Pt_InM_all_36",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.5-1.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_36);
  rap_vs_Pt_InM_all_37 = new TH1D("rap_vs_Pt_InM_all_37",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.6-1.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_37);
  rap_vs_Pt_InM_all_38 = new TH1D("rap_vs_Pt_InM_all_38",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.7-1.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_38);
  rap_vs_Pt_InM_all_39 = new TH1D("rap_vs_Pt_InM_all_39",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.8-1.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_39);
  rap_vs_Pt_InM_all_40 = new TH1D("rap_vs_Pt_InM_all_40",
                                  "rapidity = (1.6-2.0)      P_{t} = (1.9-2.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_40);
  rap_vs_Pt_InM_all_41 = new TH1D("rap_vs_Pt_InM_all_41",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.0-0.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_41);
  rap_vs_Pt_InM_all_42 = new TH1D("rap_vs_Pt_InM_all_42",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.1-0.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_42);
  rap_vs_Pt_InM_all_43 = new TH1D("rap_vs_Pt_InM_all_43",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.2-0.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_43);
  rap_vs_Pt_InM_all_44 = new TH1D("rap_vs_Pt_InM_all_44",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.3-0.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_44);
  rap_vs_Pt_InM_all_45 = new TH1D("rap_vs_Pt_InM_all_45",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.4-0.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_45);
  rap_vs_Pt_InM_all_46 = new TH1D("rap_vs_Pt_InM_all_46",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.5-0.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_46);
  rap_vs_Pt_InM_all_47 = new TH1D("rap_vs_Pt_InM_all_47",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.6-0.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_47);
  rap_vs_Pt_InM_all_48 = new TH1D("rap_vs_Pt_InM_all_48",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.7-0.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_48);
  rap_vs_Pt_InM_all_49 = new TH1D("rap_vs_Pt_InM_all_49",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.8-0.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_49);
  rap_vs_Pt_InM_all_50 = new TH1D("rap_vs_Pt_InM_all_50",
                                  "rapidity = (2.0-2.4)      P_{t} = (0.9-1.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_50);
  rap_vs_Pt_InM_all_51 = new TH1D("rap_vs_Pt_InM_all_51",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.0-1.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_51);
  rap_vs_Pt_InM_all_52 = new TH1D("rap_vs_Pt_InM_all_52",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.1-1.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_52);
  rap_vs_Pt_InM_all_53 = new TH1D("rap_vs_Pt_InM_all_53",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.2-1.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_53);
  rap_vs_Pt_InM_all_54 = new TH1D("rap_vs_Pt_InM_all_54",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.3-1.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_54);
  rap_vs_Pt_InM_all_55 = new TH1D("rap_vs_Pt_InM_all_55",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.4-1.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_55);
  rap_vs_Pt_InM_all_56 = new TH1D("rap_vs_Pt_InM_all_56",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.5-1.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_56);
  rap_vs_Pt_InM_all_57 = new TH1D("rap_vs_Pt_InM_all_57",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.6-1.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_57);
  rap_vs_Pt_InM_all_58 = new TH1D("rap_vs_Pt_InM_all_58",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.7-1.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_58);
  rap_vs_Pt_InM_all_59 = new TH1D("rap_vs_Pt_InM_all_59",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.8-1.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_59);
  rap_vs_Pt_InM_all_60 = new TH1D("rap_vs_Pt_InM_all_60",
                                  "rapidity = (2.0-2.4)      P_{t} = (1.9-2.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_60);
  rap_vs_Pt_InM_all_61 = new TH1D("rap_vs_Pt_InM_all_61",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.0-0.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_61);
  rap_vs_Pt_InM_all_62 = new TH1D("rap_vs_Pt_InM_all_62",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.1-0.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_62);
  rap_vs_Pt_InM_all_63 = new TH1D("rap_vs_Pt_InM_all_63",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.2-0.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_63);
  rap_vs_Pt_InM_all_64 = new TH1D("rap_vs_Pt_InM_all_64",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.3-0.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_64);
  rap_vs_Pt_InM_all_65 = new TH1D("rap_vs_Pt_InM_all_65",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.4-0.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_65);
  rap_vs_Pt_InM_all_66 = new TH1D("rap_vs_Pt_InM_all_66",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.5-0.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_66);
  rap_vs_Pt_InM_all_67 = new TH1D("rap_vs_Pt_InM_all_67",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.6-0.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_67);
  rap_vs_Pt_InM_all_68 = new TH1D("rap_vs_Pt_InM_all_68",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.7-0.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_68);
  rap_vs_Pt_InM_all_69 = new TH1D("rap_vs_Pt_InM_all_69",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.8-0.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_69);
  rap_vs_Pt_InM_all_70 = new TH1D("rap_vs_Pt_InM_all_70",
                                  "rapidity = (2.4-2.8)      P_{t} = (0.9-1.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_70);
  rap_vs_Pt_InM_all_71 = new TH1D("rap_vs_Pt_InM_all_71",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.0-1.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_71);
  rap_vs_Pt_InM_all_72 = new TH1D("rap_vs_Pt_InM_all_72",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.1-1.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_72);
  rap_vs_Pt_InM_all_73 = new TH1D("rap_vs_Pt_InM_all_73",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.2-1.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_73);
  rap_vs_Pt_InM_all_74 = new TH1D("rap_vs_Pt_InM_all_74",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.3-1.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_74);
  rap_vs_Pt_InM_all_75 = new TH1D("rap_vs_Pt_InM_all_75",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.4-1.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_75);
  rap_vs_Pt_InM_all_76 = new TH1D("rap_vs_Pt_InM_all_76",
                                  "rapidity = (2.4-2.8)      P_{t} = (1.5-1.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_76);
  rap_vs_Pt_InM_all_81 = new TH1D("rap_vs_Pt_InM_all_81",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.0-0.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_81);
  rap_vs_Pt_InM_all_82 = new TH1D("rap_vs_Pt_InM_all_82",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.1-0.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_82);
  rap_vs_Pt_InM_all_83 = new TH1D("rap_vs_Pt_InM_all_83",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.2-0.3 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_83);
  rap_vs_Pt_InM_all_84 = new TH1D("rap_vs_Pt_InM_all_84",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.3-0.4 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_84);
  rap_vs_Pt_InM_all_85 = new TH1D("rap_vs_Pt_InM_all_85",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.4-0.5 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_85);
  rap_vs_Pt_InM_all_86 = new TH1D("rap_vs_Pt_InM_all_86",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.5-0.6 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_86);
  rap_vs_Pt_InM_all_87 = new TH1D("rap_vs_Pt_InM_all_87",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.6-0.7 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_87);
  rap_vs_Pt_InM_all_88 = new TH1D("rap_vs_Pt_InM_all_88",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.7-0.8 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_88);
  rap_vs_Pt_InM_all_89 = new TH1D("rap_vs_Pt_InM_all_89",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.8-0.9 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_89);
  rap_vs_Pt_InM_all_90 = new TH1D("rap_vs_Pt_InM_all_90",
                                  "rapidity = (2.8-3.2)      P_{t} = (0.9-1.0 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_90);
  rap_vs_Pt_InM_all_91 = new TH1D("rap_vs_Pt_InM_all_91",
                                  "rapidity = (2.8-3.2)      P_{t} = (1.0-1.1 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_91);
  rap_vs_Pt_InM_all_92 = new TH1D("rap_vs_Pt_InM_all_92",
                                  "rapidity = (2.8-3.2)      P_{t} = (1.1-1.2 "
                                  "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                  1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_92);
  rap_vs_Pt_InM_all_101 = new TH1D("rap_vs_Pt_InM_all_101",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.0-0.1 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_101);
  rap_vs_Pt_InM_all_102 = new TH1D("rap_vs_Pt_InM_all_102",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.1-0.2 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_102);
  rap_vs_Pt_InM_all_103 = new TH1D("rap_vs_Pt_InM_all_103",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.2-0.3 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_103);
  rap_vs_Pt_InM_all_104 = new TH1D("rap_vs_Pt_InM_all_104",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.3-0.4 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_104);
  rap_vs_Pt_InM_all_105 = new TH1D("rap_vs_Pt_InM_all_105",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.4-0.5 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_105);
  rap_vs_Pt_InM_all_106 = new TH1D("rap_vs_Pt_InM_all_106",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.5-0.6 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_106);
  rap_vs_Pt_InM_all_107 = new TH1D("rap_vs_Pt_InM_all_107",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.6-0.7 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_107);
  rap_vs_Pt_InM_all_108 = new TH1D("rap_vs_Pt_InM_all_108",
                                   "rapidity = (3.2-3.6)      P_{t} = (0.7-0.8 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_108);
  rap_vs_Pt_InM_all_111 = new TH1D("rap_vs_Pt_InM_all_111",
                                   "rapidity = (3.6-4.0)      P_{t} = (0.0-0.1 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_111);
  rap_vs_Pt_InM_all_112 = new TH1D("rap_vs_Pt_InM_all_112",
                                   "rapidity = (3.6-4.0)      P_{t} = (0.1-0.2 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_112);
  rap_vs_Pt_InM_all_113 = new TH1D("rap_vs_Pt_InM_all_113",
                                   "rapidity = (3.6-4.0)      P_{t} = (0.2-0.3 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_113);
  rap_vs_Pt_InM_all_114 = new TH1D("rap_vs_Pt_InM_all_114",
                                   "rapidity = (3.6-4.0)      P_{t} = (0.3-0.4 "
                                   "GeV/c^{2}) ;invariant mass in GeV/c^{2};#",
                                   1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_114);

  rap_vs_Pt_InM_all_mixing_1 = new TH1D("rap_vs_Pt_InM_all_mixing_1",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_1);
  rap_vs_Pt_InM_all_mixing_2 = new TH1D("rap_vs_Pt_InM_all_mixing_2",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_2);
  rap_vs_Pt_InM_all_mixing_3 = new TH1D("rap_vs_Pt_InM_all_mixing_3",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_3);
  rap_vs_Pt_InM_all_mixing_4 = new TH1D("rap_vs_Pt_InM_all_mixing_4",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_4);
  rap_vs_Pt_InM_all_mixing_5 = new TH1D("rap_vs_Pt_InM_all_mixing_5",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_5);
  rap_vs_Pt_InM_all_mixing_6 = new TH1D("rap_vs_Pt_InM_all_mixing_6",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_6);
  rap_vs_Pt_InM_all_mixing_7 = new TH1D("rap_vs_Pt_InM_all_mixing_7",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_7);
  rap_vs_Pt_InM_all_mixing_8 = new TH1D("rap_vs_Pt_InM_all_mixing_8",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_8);
  rap_vs_Pt_InM_all_mixing_9 = new TH1D("rap_vs_Pt_InM_all_mixing_9",
                                        "rapidity = (1.2-1.6)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                        "mass in GeV/c^{2};#",
                                        1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_9);
  rap_vs_Pt_InM_all_mixing_10 = new TH1D("rap_vs_Pt_InM_all_mixing_10",
                                         "rapidity = (1.2-1.6)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_10);
  rap_vs_Pt_InM_all_mixing_11 = new TH1D("rap_vs_Pt_InM_all_mixing_11",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_11);
  rap_vs_Pt_InM_all_mixing_12 = new TH1D("rap_vs_Pt_InM_all_mixing_12",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_12);
  rap_vs_Pt_InM_all_mixing_13 = new TH1D("rap_vs_Pt_InM_all_mixing_13",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_13);
  rap_vs_Pt_InM_all_mixing_14 = new TH1D("rap_vs_Pt_InM_all_mixing_14",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_14);
  rap_vs_Pt_InM_all_mixing_15 = new TH1D("rap_vs_Pt_InM_all_mixing_15",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_15);
  rap_vs_Pt_InM_all_mixing_16 = new TH1D("rap_vs_Pt_InM_all_mixing_16",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_16);
  rap_vs_Pt_InM_all_mixing_17 = new TH1D("rap_vs_Pt_InM_all_mixing_17",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_17);
  rap_vs_Pt_InM_all_mixing_18 = new TH1D("rap_vs_Pt_InM_all_mixing_18",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_18);
  rap_vs_Pt_InM_all_mixing_19 = new TH1D("rap_vs_Pt_InM_all_mixing_19",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_19);
  rap_vs_Pt_InM_all_mixing_20 = new TH1D("rap_vs_Pt_InM_all_mixing_20",
                                         "rapidity = (1.2-1.6)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_20);
  rap_vs_Pt_InM_all_mixing_21 = new TH1D("rap_vs_Pt_InM_all_mixing_21",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_21);
  rap_vs_Pt_InM_all_mixing_22 = new TH1D("rap_vs_Pt_InM_all_mixing_22",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_22);
  rap_vs_Pt_InM_all_mixing_23 = new TH1D("rap_vs_Pt_InM_all_mixing_23",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_23);
  rap_vs_Pt_InM_all_mixing_24 = new TH1D("rap_vs_Pt_InM_all_mixing_24",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_24);
  rap_vs_Pt_InM_all_mixing_25 = new TH1D("rap_vs_Pt_InM_all_mixing_25",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_25);
  rap_vs_Pt_InM_all_mixing_26 = new TH1D("rap_vs_Pt_InM_all_mixing_26",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_26);
  rap_vs_Pt_InM_all_mixing_27 = new TH1D("rap_vs_Pt_InM_all_mixing_27",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_27);
  rap_vs_Pt_InM_all_mixing_28 = new TH1D("rap_vs_Pt_InM_all_mixing_28",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_28);
  rap_vs_Pt_InM_all_mixing_29 = new TH1D("rap_vs_Pt_InM_all_mixing_29",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_29);
  rap_vs_Pt_InM_all_mixing_30 = new TH1D("rap_vs_Pt_InM_all_mixing_30",
                                         "rapidity = (1.6-2.0)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_30);
  rap_vs_Pt_InM_all_mixing_31 = new TH1D("rap_vs_Pt_InM_all_mixing_31",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_31);
  rap_vs_Pt_InM_all_mixing_32 = new TH1D("rap_vs_Pt_InM_all_mixing_32",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_32);
  rap_vs_Pt_InM_all_mixing_33 = new TH1D("rap_vs_Pt_InM_all_mixing_33",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_33);
  rap_vs_Pt_InM_all_mixing_34 = new TH1D("rap_vs_Pt_InM_all_mixing_34",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_34);
  rap_vs_Pt_InM_all_mixing_35 = new TH1D("rap_vs_Pt_InM_all_mixing_35",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_35);
  rap_vs_Pt_InM_all_mixing_36 = new TH1D("rap_vs_Pt_InM_all_mixing_36",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_36);
  rap_vs_Pt_InM_all_mixing_37 = new TH1D("rap_vs_Pt_InM_all_mixing_37",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_37);
  rap_vs_Pt_InM_all_mixing_38 = new TH1D("rap_vs_Pt_InM_all_mixing_38",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_38);
  rap_vs_Pt_InM_all_mixing_39 = new TH1D("rap_vs_Pt_InM_all_mixing_39",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_39);
  rap_vs_Pt_InM_all_mixing_40 = new TH1D("rap_vs_Pt_InM_all_mixing_40",
                                         "rapidity = (1.6-2.0)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_40);
  rap_vs_Pt_InM_all_mixing_41 = new TH1D("rap_vs_Pt_InM_all_mixing_41",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_41);
  rap_vs_Pt_InM_all_mixing_42 = new TH1D("rap_vs_Pt_InM_all_mixing_42",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_42);
  rap_vs_Pt_InM_all_mixing_43 = new TH1D("rap_vs_Pt_InM_all_mixing_43",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_43);
  rap_vs_Pt_InM_all_mixing_44 = new TH1D("rap_vs_Pt_InM_all_mixing_44",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_44);
  rap_vs_Pt_InM_all_mixing_45 = new TH1D("rap_vs_Pt_InM_all_mixing_45",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_45);
  rap_vs_Pt_InM_all_mixing_46 = new TH1D("rap_vs_Pt_InM_all_mixing_46",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_46);
  rap_vs_Pt_InM_all_mixing_47 = new TH1D("rap_vs_Pt_InM_all_mixing_47",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_47);
  rap_vs_Pt_InM_all_mixing_48 = new TH1D("rap_vs_Pt_InM_all_mixing_48",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_48);
  rap_vs_Pt_InM_all_mixing_49 = new TH1D("rap_vs_Pt_InM_all_mixing_49",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_49);
  rap_vs_Pt_InM_all_mixing_50 = new TH1D("rap_vs_Pt_InM_all_mixing_50",
                                         "rapidity = (2.0-2.4)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_50);
  rap_vs_Pt_InM_all_mixing_51 = new TH1D("rap_vs_Pt_InM_all_mixing_51",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_51);
  rap_vs_Pt_InM_all_mixing_52 = new TH1D("rap_vs_Pt_InM_all_mixing_52",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_52);
  rap_vs_Pt_InM_all_mixing_53 = new TH1D("rap_vs_Pt_InM_all_mixing_53",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_53);
  rap_vs_Pt_InM_all_mixing_54 = new TH1D("rap_vs_Pt_InM_all_mixing_54",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_54);
  rap_vs_Pt_InM_all_mixing_55 = new TH1D("rap_vs_Pt_InM_all_mixing_55",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_55);
  rap_vs_Pt_InM_all_mixing_56 = new TH1D("rap_vs_Pt_InM_all_mixing_56",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_56);
  rap_vs_Pt_InM_all_mixing_57 = new TH1D("rap_vs_Pt_InM_all_mixing_57",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_57);
  rap_vs_Pt_InM_all_mixing_58 = new TH1D("rap_vs_Pt_InM_all_mixing_58",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_58);
  rap_vs_Pt_InM_all_mixing_59 = new TH1D("rap_vs_Pt_InM_all_mixing_59",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_59);
  rap_vs_Pt_InM_all_mixing_60 = new TH1D("rap_vs_Pt_InM_all_mixing_60",
                                         "rapidity = (2.0-2.4)      P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_60);
  rap_vs_Pt_InM_all_mixing_61 = new TH1D("rap_vs_Pt_InM_all_mixing_61",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_61);
  rap_vs_Pt_InM_all_mixing_62 = new TH1D("rap_vs_Pt_InM_all_mixing_62",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_62);
  rap_vs_Pt_InM_all_mixing_63 = new TH1D("rap_vs_Pt_InM_all_mixing_63",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_63);
  rap_vs_Pt_InM_all_mixing_64 = new TH1D("rap_vs_Pt_InM_all_mixing_64",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_64);
  rap_vs_Pt_InM_all_mixing_65 = new TH1D("rap_vs_Pt_InM_all_mixing_65",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_65);
  rap_vs_Pt_InM_all_mixing_66 = new TH1D("rap_vs_Pt_InM_all_mixing_66",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_66);
  rap_vs_Pt_InM_all_mixing_67 = new TH1D("rap_vs_Pt_InM_all_mixing_67",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_67);
  rap_vs_Pt_InM_all_mixing_68 = new TH1D("rap_vs_Pt_InM_all_mixing_68",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_68);
  rap_vs_Pt_InM_all_mixing_69 = new TH1D("rap_vs_Pt_InM_all_mixing_69",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_69);
  rap_vs_Pt_InM_all_mixing_70 = new TH1D("rap_vs_Pt_InM_all_mixing_70",
                                         "rapidity = (2.4-2.8)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_70);
  rap_vs_Pt_InM_all_mixing_71 = new TH1D("rap_vs_Pt_InM_all_mixing_71",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_71);
  rap_vs_Pt_InM_all_mixing_72 = new TH1D("rap_vs_Pt_InM_all_mixing_72",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_72);
  rap_vs_Pt_InM_all_mixing_73 = new TH1D("rap_vs_Pt_InM_all_mixing_73",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_73);
  rap_vs_Pt_InM_all_mixing_74 = new TH1D("rap_vs_Pt_InM_all_mixing_74",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_74);
  rap_vs_Pt_InM_all_mixing_75 = new TH1D("rap_vs_Pt_InM_all_mixing_75",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_75);
  rap_vs_Pt_InM_all_mixing_76 = new TH1D("rap_vs_Pt_InM_all_mixing_76",
                                         "rapidity = (2.4-2.8)      P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_76);
  rap_vs_Pt_InM_all_mixing_81 = new TH1D("rap_vs_Pt_InM_all_mixing_81",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_81);
  rap_vs_Pt_InM_all_mixing_82 = new TH1D("rap_vs_Pt_InM_all_mixing_82",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_82);
  rap_vs_Pt_InM_all_mixing_83 = new TH1D("rap_vs_Pt_InM_all_mixing_83",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_83);
  rap_vs_Pt_InM_all_mixing_84 = new TH1D("rap_vs_Pt_InM_all_mixing_84",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_84);
  rap_vs_Pt_InM_all_mixing_85 = new TH1D("rap_vs_Pt_InM_all_mixing_85",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_85);
  rap_vs_Pt_InM_all_mixing_86 = new TH1D("rap_vs_Pt_InM_all_mixing_86",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_86);
  rap_vs_Pt_InM_all_mixing_87 = new TH1D("rap_vs_Pt_InM_all_mixing_87",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_87);
  rap_vs_Pt_InM_all_mixing_88 = new TH1D("rap_vs_Pt_InM_all_mixing_88",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_88);
  rap_vs_Pt_InM_all_mixing_89 = new TH1D("rap_vs_Pt_InM_all_mixing_89",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_89);
  rap_vs_Pt_InM_all_mixing_90 = new TH1D("rap_vs_Pt_InM_all_mixing_90",
                                         "rapidity = (2.8-3.2)      P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_90);
  rap_vs_Pt_InM_all_mixing_91 = new TH1D("rap_vs_Pt_InM_all_mixing_91",
                                         "rapidity = (2.8-3.2)      P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_91);
  rap_vs_Pt_InM_all_mixing_92 = new TH1D("rap_vs_Pt_InM_all_mixing_92",
                                         "rapidity = (2.8-3.2)      P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant "
                                         "mass in GeV/c^{2};#",
                                         1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_92);
  rap_vs_Pt_InM_all_mixing_101 = new TH1D("rap_vs_Pt_InM_all_mixing_101",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_101);
  rap_vs_Pt_InM_all_mixing_102 = new TH1D("rap_vs_Pt_InM_all_mixing_102",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_102);
  rap_vs_Pt_InM_all_mixing_103 = new TH1D("rap_vs_Pt_InM_all_mixing_103",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_103);
  rap_vs_Pt_InM_all_mixing_104 = new TH1D("rap_vs_Pt_InM_all_mixing_104",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_104);
  rap_vs_Pt_InM_all_mixing_105 = new TH1D("rap_vs_Pt_InM_all_mixing_105",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_105);
  rap_vs_Pt_InM_all_mixing_106 = new TH1D("rap_vs_Pt_InM_all_mixing_106",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_106);
  rap_vs_Pt_InM_all_mixing_107 = new TH1D("rap_vs_Pt_InM_all_mixing_107",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_107);
  rap_vs_Pt_InM_all_mixing_108 = new TH1D("rap_vs_Pt_InM_all_mixing_108",
                                          "rapidity = (3.2-3.6)      P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_108);
  rap_vs_Pt_InM_all_mixing_111 = new TH1D("rap_vs_Pt_InM_all_mixing_111",
                                          "rapidity = (3.6-4.0)      P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_111);
  rap_vs_Pt_InM_all_mixing_112 = new TH1D("rap_vs_Pt_InM_all_mixing_112",
                                          "rapidity = (3.6-4.0)      P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_112);
  rap_vs_Pt_InM_all_mixing_113 = new TH1D("rap_vs_Pt_InM_all_mixing_113",
                                          "rapidity = (3.6-4.0)      P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_113);
  rap_vs_Pt_InM_all_mixing_114 = new TH1D("rap_vs_Pt_InM_all_mixing_114",
                                          "rapidity = (3.6-4.0)      P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant "
                                          "mass in GeV/c^{2};#",
                                          1000, 0, 2.0);
  fHistoList_rap_vs_pt_InM_all.push_back(rap_vs_Pt_InM_all_mixing_114);


  // Pt separation sith step 0.1 "onetwo"
  Pi0_pt_est_onetwo_Both =
    new TH1D("Pi0_pt_est_onetwo_Both", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 20, 0, 2.0);
  fHistoList_pt_onetwo.push_back(Pi0_pt_est_onetwo_Both);
  pt_onetwo_1 = new TH1D("pt_onetwo_1", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_1);
  pt_onetwo_2 = new TH1D("pt_onetwo_2", "P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_2);
  pt_onetwo_3 = new TH1D("pt_onetwo_3", "P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_3);
  pt_onetwo_4 = new TH1D("pt_onetwo_4", "P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_4);
  pt_onetwo_5 = new TH1D("pt_onetwo_5", "P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_5);
  pt_onetwo_6 = new TH1D("pt_onetwo_6", "P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_6);
  pt_onetwo_7 = new TH1D("pt_onetwo_7", "P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_7);
  pt_onetwo_8 = new TH1D("pt_onetwo_8", "P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_8);
  pt_onetwo_9 = new TH1D("pt_onetwo_9", "P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_9);
  pt_onetwo_10 = new TH1D("pt_onetwo_10", "P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_10);
  pt_onetwo_11 = new TH1D("pt_onetwo_11", "P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_11);
  pt_onetwo_12 = new TH1D("pt_onetwo_12", "P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_12);
  pt_onetwo_13 = new TH1D("pt_onetwo_13", "P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_13);
  pt_onetwo_14 = new TH1D("pt_onetwo_14", "P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_14);
  pt_onetwo_15 = new TH1D("pt_onetwo_15", "P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_15);
  pt_onetwo_16 = new TH1D("pt_onetwo_16", "P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_16);
  pt_onetwo_17 = new TH1D("pt_onetwo_17", "P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_17);
  pt_onetwo_18 = new TH1D("pt_onetwo_18", "P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_18);
  pt_onetwo_19 = new TH1D("pt_onetwo_19", "P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_19);
  pt_onetwo_20 = new TH1D("pt_onetwo_20", "P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_20);

  pt_onetwo_mixing_1 =
    new TH1D("pt_onetwo_mixing_1", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_1);
  pt_onetwo_mixing_2 =
    new TH1D("pt_onetwo_mixing_2", "P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_2);
  pt_onetwo_mixing_3 =
    new TH1D("pt_onetwo_mixing_3", "P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_3);
  pt_onetwo_mixing_4 =
    new TH1D("pt_onetwo_mixing_4", "P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_4);
  pt_onetwo_mixing_5 =
    new TH1D("pt_onetwo_mixing_5", "P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_5);
  pt_onetwo_mixing_6 =
    new TH1D("pt_onetwo_mixing_6", "P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_6);
  pt_onetwo_mixing_7 =
    new TH1D("pt_onetwo_mixing_7", "P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_7);
  pt_onetwo_mixing_8 =
    new TH1D("pt_onetwo_mixing_8", "P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_8);
  pt_onetwo_mixing_9 =
    new TH1D("pt_onetwo_mixing_9", "P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_9);
  pt_onetwo_mixing_10 =
    new TH1D("pt_onetwo_mixing_10", "P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_10);
  pt_onetwo_mixing_11 =
    new TH1D("pt_onetwo_mixing_11", "P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_11);
  pt_onetwo_mixing_12 =
    new TH1D("pt_onetwo_mixing_12", "P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_12);
  pt_onetwo_mixing_13 =
    new TH1D("pt_onetwo_mixing_13", "P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_13);
  pt_onetwo_mixing_14 =
    new TH1D("pt_onetwo_mixing_14", "P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_14);
  pt_onetwo_mixing_15 =
    new TH1D("pt_onetwo_mixing_15", "P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_15);
  pt_onetwo_mixing_16 =
    new TH1D("pt_onetwo_mixing_16", "P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_16);
  pt_onetwo_mixing_17 =
    new TH1D("pt_onetwo_mixing_17", "P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_17);
  pt_onetwo_mixing_18 =
    new TH1D("pt_onetwo_mixing_18", "P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_18);
  pt_onetwo_mixing_19 =
    new TH1D("pt_onetwo_mixing_19", "P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_19);
  pt_onetwo_mixing_20 =
    new TH1D("pt_onetwo_mixing_20", "P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_onetwo.push_back(pt_onetwo_mixing_20);

  // Pt separation sith step 0.1 "all"
  Pi0_pt_est_all_Both =
    new TH1D("Pi0_pt_est_all_Both", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 20, 0, 2.0);
  fHistoList_pt_all.push_back(Pi0_pt_est_all_Both);
  pt_all_1 = new TH1D("pt_all_1", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_1);
  pt_all_2 = new TH1D("pt_all_2", "P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_2);
  pt_all_3 = new TH1D("pt_all_3", "P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_3);
  pt_all_4 = new TH1D("pt_all_4", "P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_4);
  pt_all_5 = new TH1D("pt_all_5", "P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_5);
  pt_all_6 = new TH1D("pt_all_6", "P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_6);
  pt_all_7 = new TH1D("pt_all_7", "P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_7);
  pt_all_8 = new TH1D("pt_all_8", "P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_8);
  pt_all_9 = new TH1D("pt_all_9", "P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_9);
  pt_all_10 = new TH1D("pt_all_10", "P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_10);
  pt_all_11 = new TH1D("pt_all_11", "P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_11);
  pt_all_12 = new TH1D("pt_all_12", "P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_12);
  pt_all_13 = new TH1D("pt_all_13", "P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_13);
  pt_all_14 = new TH1D("pt_all_14", "P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_14);
  pt_all_15 = new TH1D("pt_all_15", "P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_15);
  pt_all_16 = new TH1D("pt_all_16", "P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_16);
  pt_all_17 = new TH1D("pt_all_17", "P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_17);
  pt_all_18 = new TH1D("pt_all_18", "P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_18);
  pt_all_19 = new TH1D("pt_all_19", "P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_19);
  pt_all_20 = new TH1D("pt_all_20", "P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_20);

  pt_all_mixing_1 =
    new TH1D("pt_all_mixing_1", "P_{t} = (0.0-0.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_1);
  pt_all_mixing_2 =
    new TH1D("pt_all_mixing_2", "P_{t} = (0.1-0.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_2);
  pt_all_mixing_3 =
    new TH1D("pt_all_mixing_3", "P_{t} = (0.2-0.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_3);
  pt_all_mixing_4 =
    new TH1D("pt_all_mixing_4", "P_{t} = (0.3-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_4);
  pt_all_mixing_5 =
    new TH1D("pt_all_mixing_5", "P_{t} = (0.4-0.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_5);
  pt_all_mixing_6 =
    new TH1D("pt_all_mixing_6", "P_{t} = (0.5-0.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_6);
  pt_all_mixing_7 =
    new TH1D("pt_all_mixing_7", "P_{t} = (0.6-0.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_7);
  pt_all_mixing_8 =
    new TH1D("pt_all_mixing_8", "P_{t} = (0.7-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_8);
  pt_all_mixing_9 =
    new TH1D("pt_all_mixing_9", "P_{t} = (0.8-0.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_9);
  pt_all_mixing_10 =
    new TH1D("pt_all_mixing_10", "P_{t} = (0.9-1.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_10);
  pt_all_mixing_11 =
    new TH1D("pt_all_mixing_11", "P_{t} = (1.0-1.1 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_11);
  pt_all_mixing_12 =
    new TH1D("pt_all_mixing_12", "P_{t} = (1.1-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_12);
  pt_all_mixing_13 =
    new TH1D("pt_all_mixing_13", "P_{t} = (1.2-1.3 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_13);
  pt_all_mixing_14 =
    new TH1D("pt_all_mixing_14", "P_{t} = (1.3-1.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_14);
  pt_all_mixing_15 =
    new TH1D("pt_all_mixing_15", "P_{t} = (1.4-1.5 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_15);
  pt_all_mixing_16 =
    new TH1D("pt_all_mixing_16", "P_{t} = (1.5-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_16);
  pt_all_mixing_17 =
    new TH1D("pt_all_mixing_17", "P_{t} = (1.6-1.7 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_17);
  pt_all_mixing_18 =
    new TH1D("pt_all_mixing_18", "P_{t} = (1.7-1.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_18);
  pt_all_mixing_19 =
    new TH1D("pt_all_mixing_19", "P_{t} = (1.8-1.9 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_19);
  pt_all_mixing_20 =
    new TH1D("pt_all_mixing_20", "P_{t} = (1.9-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_pt_all.push_back(pt_all_mixing_20);


  // BG cases
  //Both all
  BG1_InM_all_Both = new TH1D("BG1_InM_all_Both", "BG1_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG1_InM_all_Both);
  BG2_InM_all_Both = new TH1D("BG2_InM_all_Both", "BG2_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG2_InM_all_Both);
  BG3_InM_all_Both = new TH1D("BG3_InM_all_Both", "BG3_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG3_InM_all_Both);
  BG4_InM_all_Both = new TH1D("BG4_InM_all_Both", "BG4_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG4_InM_all_Both);
  BG5_InM_all_Both = new TH1D("BG5_InM_all_Both", "BG5_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG5_InM_all_Both);
  BG6_InM_all_Both = new TH1D("BG6_InM_all_Both", "BG6_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG6_InM_all_Both);
  BG7_InM_all_Both = new TH1D("BG7_InM_all_Both", "BG7_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG7_InM_all_Both);
  BG8_InM_all_Both = new TH1D("BG8_InM_all_Both", "BG8_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG8_InM_all_Both);
  BG9_InM_all_Both = new TH1D("BG9_InM_all_Both", "BG9_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG9_InM_all_Both);
  BG10_InM_all_Both = new TH1D("BG10_InM_all_Both", "BG10_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(BG10_InM_all_Both);
  PdgCase8_InM_all_Both = new TH1D("PdgCase8_InM_all_Both", "PdgCase8_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(PdgCase8_InM_all_Both);
  PdgCase8mothers_InM_all_Both =
    new TH1D("PdgCase8mothers_InM_all_Both", "PdgCase8mothers_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(PdgCase8mothers_InM_all_Both);
  sameMIDcase8_InM_all_Both =
    new TH1D("sameMIDcase8_InM_all_Both", "sameMIDcase8_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8_InM_all_Both);
  sameGRIDcase8_InM_all_Both =
    new TH1D("sameGRIDcase8_InM_all_Both", "sameGRIDcase8_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(sameGRIDcase8_InM_all_Both);
  Case1ZYPos_InM_all_Both =
    new TH2D("Case1ZYPos_InM_all_Both", "Case1ZYPos_InM_all_Both; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_all_Both.push_back(Case1ZYPos_InM_all_Both);
  sameMIDcase8_mothedPDG_InM_all_Both =
    new TH1D("sameMIDcase8_mothedPDG_InM_all_Both", "sameMIDcase8_mothedPDG_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8_mothedPDG_InM_all_Both);
  PdgCase8NonEComeFromTarget_InM_all_Both = new TH1D(
    "PdgCase8NonEComeFromTarget_InM_all_Both", "PdgCase8NonEComeFromTarget_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(PdgCase8NonEComeFromTarget_InM_all_Both);
  PdgCase8NonE_NOT_FromTarget_InM_all_Both = new TH1D(
    "PdgCase8NonE_NOT_FromTarget_InM_all_Both", "PdgCase8NonE_NOT_FromTarget_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(PdgCase8NonE_NOT_FromTarget_InM_all_Both);
  PdgCase8motherNonE_InM_all_Both =
    new TH1D("PdgCase8motherNonE_InM_all_Both", "PdgCase8motherNonE_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(PdgCase8motherNonE_InM_all_Both);
  Case8ElFromDalitz_InM_all_Both = new TH1D(
    "Case8ElFromDalitz_InM_all_Both", "Case8ElFromDalitz_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(Case8ElFromDalitz_InM_all_Both);
  Case8NonElFrom_pn_InM_all_Both = new TH1D(
    "Case8NonElFrom_pn_InM_all_Both", "Case8NonElFrom_pn_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(Case8NonElFrom_pn_InM_all_Both);
  Case8NonElFrom_eta_InM_all_Both = new TH1D(
    "Case8NonElFrom_eta_InM_all_Both", "Case8NonElFrom_eta_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(Case8NonElFrom_eta_InM_all_Both);
  Case8NonElFrom_kaon_InM_all_Both =
    new TH1D("Case8NonElFrom_kaon_InM_all_Both", "Case8NonElFrom_kaon_InM_all_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(Case8NonElFrom_kaon_InM_all_Both);
  sameMIDcase8NonEPdg_InM_all_Both =
    new TH1D("sameMIDcase8NonEPdg_InM_all_Both", "sameMIDcase8NonEPdg_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEPdg_InM_all_Both);
  sameMIDcase8NonEMotherPdg_InM_all_Both = new TH1D("sameMIDcase8NonEMotherPdg_InM_all_Both",
                                                    "sameMIDcase8NonEMotherPdg_InM_all_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEMotherPdg_InM_all_Both);
  sameMIDcase8NonEMotherIM_InM_all_Both =
    new TH1D("sameMIDcase8NonEMotherIM_InM_all_Both",
             "sameMIDcase8NonEMotherIM_InM_all_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEMotherIM_InM_all_Both);
  sameMIDcase8NonEPdgFromTarget_InM_all_Both =
    new TH1D("sameMIDcase8NonEPdgFromTarget_InM_all_Both", "sameMIDcase8NonEPdgFromTarget_InM_all_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEPdgFromTarget_InM_all_Both);
  sameMIDcase8NonEComeFromTargetIM_InM_all_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_all_Both",
             "sameMIDcase8NonEComeFromTargetIM_InM_all_Both; invariant mass in "
             "GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEComeFromTargetIM_InM_all_Both);
  sameMIDcase8NonEComeFromTargetP_InM_all_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_all_Both",
             "sameMIDcase8NonEComeFromTargetP_InM_all_Both; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEComeFromTargetP_InM_all_Both);
  sameMIDcase8NonEComeFromTargetPt_InM_all_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_all_Both",
             "sameMIDcase8NonEComeFromTargetPt_InM_all_Both; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_all_Both.push_back(sameMIDcase8NonEComeFromTargetPt_InM_all_Both);
  //Both zero
  BG1_InM_zero_Both = new TH1D("BG1_InM_zero_Both", "BG1_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG1_InM_zero_Both);
  BG2_InM_zero_Both = new TH1D("BG2_InM_zero_Both", "BG2_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG2_InM_zero_Both);
  BG3_InM_zero_Both = new TH1D("BG3_InM_zero_Both", "BG3_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG3_InM_zero_Both);
  BG4_InM_zero_Both = new TH1D("BG4_InM_zero_Both", "BG4_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG4_InM_zero_Both);
  BG5_InM_zero_Both = new TH1D("BG5_InM_zero_Both", "BG5_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG5_InM_zero_Both);
  BG6_InM_zero_Both = new TH1D("BG6_InM_zero_Both", "BG6_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG6_InM_zero_Both);
  BG7_InM_zero_Both = new TH1D("BG7_InM_zero_Both", "BG7_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG7_InM_zero_Both);
  BG8_InM_zero_Both = new TH1D("BG8_InM_zero_Both", "BG8_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG8_InM_zero_Both);
  BG9_InM_zero_Both = new TH1D("BG9_InM_zero_Both", "BG9_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG9_InM_zero_Both);
  BG10_InM_zero_Both =
    new TH1D("BG10_InM_zero_Both", "BG10_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(BG10_InM_zero_Both);
  PdgCase8_InM_zero_Both = new TH1D("PdgCase8_InM_zero_Both", "PdgCase8_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(PdgCase8_InM_zero_Both);
  PdgCase8mothers_InM_zero_Both =
    new TH1D("PdgCase8mothers_InM_zero_Both", "PdgCase8mothers_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(PdgCase8mothers_InM_zero_Both);
  sameMIDcase8_InM_zero_Both =
    new TH1D("sameMIDcase8_InM_zero_Both", "sameMIDcase8_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8_InM_zero_Both);
  sameGRIDcase8_InM_zero_Both =
    new TH1D("sameGRIDcase8_InM_zero_Both", "sameGRIDcase8_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(sameGRIDcase8_InM_zero_Both);
  Case1ZYPos_InM_zero_Both =
    new TH2D("Case1ZYPos_InM_zero_Both", "Case1ZYPos_InM_zero_Both; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_zero_Both.push_back(Case1ZYPos_InM_zero_Both);
  sameMIDcase8_mothedPDG_InM_zero_Both =
    new TH1D("sameMIDcase8_mothedPDG_InM_zero_Both", "sameMIDcase8_mothedPDG_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8_mothedPDG_InM_zero_Both);
  PdgCase8NonEComeFromTarget_InM_zero_Both = new TH1D(
    "PdgCase8NonEComeFromTarget_InM_zero_Both", "PdgCase8NonEComeFromTarget_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(PdgCase8NonEComeFromTarget_InM_zero_Both);
  PdgCase8NonE_NOT_FromTarget_InM_zero_Both = new TH1D(
    "PdgCase8NonE_NOT_FromTarget_InM_zero_Both", "PdgCase8NonE_NOT_FromTarget_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(PdgCase8NonE_NOT_FromTarget_InM_zero_Both);
  PdgCase8motherNonE_InM_zero_Both =
    new TH1D("PdgCase8motherNonE_InM_zero_Both", "PdgCase8motherNonE_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(PdgCase8motherNonE_InM_zero_Both);
  Case8ElFromDalitz_InM_zero_Both = new TH1D(
    "Case8ElFromDalitz_InM_zero_Both", "Case8ElFromDalitz_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(Case8ElFromDalitz_InM_zero_Both);
  Case8NonElFrom_pn_InM_zero_Both = new TH1D(
    "Case8NonElFrom_pn_InM_zero_Both", "Case8NonElFrom_pn_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(Case8NonElFrom_pn_InM_zero_Both);
  Case8NonElFrom_eta_InM_zero_Both =
    new TH1D("Case8NonElFrom_eta_InM_zero_Both", "Case8NonElFrom_eta_InM_zero_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(Case8NonElFrom_eta_InM_zero_Both);
  Case8NonElFrom_kaon_InM_zero_Both =
    new TH1D("Case8NonElFrom_kaon_InM_zero_Both", "Case8NonElFrom_kaon_InM_zero_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(Case8NonElFrom_kaon_InM_zero_Both);
  sameMIDcase8NonEPdg_InM_zero_Both =
    new TH1D("sameMIDcase8NonEPdg_InM_zero_Both", "sameMIDcase8NonEPdg_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEPdg_InM_zero_Both);
  sameMIDcase8NonEMotherPdg_InM_zero_Both = new TH1D(
    "sameMIDcase8NonEMotherPdg_InM_zero_Both", "sameMIDcase8NonEMotherPdg_InM_zero_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEMotherPdg_InM_zero_Both);
  sameMIDcase8NonEMotherIM_InM_zero_Both =
    new TH1D("sameMIDcase8NonEMotherIM_InM_zero_Both",
             "sameMIDcase8NonEMotherIM_InM_zero_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEMotherIM_InM_zero_Both);
  sameMIDcase8NonEPdgFromTarget_InM_zero_Both =
    new TH1D("sameMIDcase8NonEPdgFromTarget_InM_zero_Both", "sameMIDcase8NonEPdgFromTarget_InM_zero_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEPdgFromTarget_InM_zero_Both);
  sameMIDcase8NonEComeFromTargetIM_InM_zero_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_zero_Both",
             "sameMIDcase8NonEComeFromTargetIM_InM_zero_Both; invariant mass "
             "in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEComeFromTargetIM_InM_zero_Both);
  sameMIDcase8NonEComeFromTargetP_InM_zero_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_zero_Both",
             "sameMIDcase8NonEComeFromTargetP_InM_zero_Both; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEComeFromTargetP_InM_zero_Both);
  sameMIDcase8NonEComeFromTargetPt_InM_zero_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_zero_Both",
             "sameMIDcase8NonEComeFromTargetPt_InM_zero_Both; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_zero_Both.push_back(sameMIDcase8NonEComeFromTargetPt_InM_zero_Both);
  //Both one
  BG1_InM_one_Both = new TH1D("BG1_InM_one_Both", "BG1_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG1_InM_one_Both);
  BG2_InM_one_Both = new TH1D("BG2_InM_one_Both", "BG2_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG2_InM_one_Both);
  BG3_InM_one_Both = new TH1D("BG3_InM_one_Both", "BG3_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG3_InM_one_Both);
  BG4_InM_one_Both = new TH1D("BG4_InM_one_Both", "BG4_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG4_InM_one_Both);
  BG5_InM_one_Both = new TH1D("BG5_InM_one_Both", "BG5_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG5_InM_one_Both);
  BG6_InM_one_Both = new TH1D("BG6_InM_one_Both", "BG6_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG6_InM_one_Both);
  BG7_InM_one_Both = new TH1D("BG7_InM_one_Both", "BG7_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG7_InM_one_Both);
  BG8_InM_one_Both = new TH1D("BG8_InM_one_Both", "BG8_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG8_InM_one_Both);
  BG9_InM_one_Both = new TH1D("BG9_InM_one_Both", "BG9_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG9_InM_one_Both);
  BG10_InM_one_Both = new TH1D("BG10_InM_one_Both", "BG10_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(BG10_InM_one_Both);
  PdgCase8_InM_one_Both = new TH1D("PdgCase8_InM_one_Both", "PdgCase8_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(PdgCase8_InM_one_Both);
  PdgCase8mothers_InM_one_Both =
    new TH1D("PdgCase8mothers_InM_one_Both", "PdgCase8mothers_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(PdgCase8mothers_InM_one_Both);
  sameMIDcase8_InM_one_Both =
    new TH1D("sameMIDcase8_InM_one_Both", "sameMIDcase8_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8_InM_one_Both);
  sameGRIDcase8_InM_one_Both =
    new TH1D("sameGRIDcase8_InM_one_Both", "sameGRIDcase8_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(sameGRIDcase8_InM_one_Both);
  Case1ZYPos_InM_one_Both =
    new TH2D("Case1ZYPos_InM_one_Both", "Case1ZYPos_InM_one_Both; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_one_Both.push_back(Case1ZYPos_InM_one_Both);
  sameMIDcase8_mothedPDG_InM_one_Both =
    new TH1D("sameMIDcase8_mothedPDG_InM_one_Both", "sameMIDcase8_mothedPDG_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8_mothedPDG_InM_one_Both);
  PdgCase8NonEComeFromTarget_InM_one_Both = new TH1D(
    "PdgCase8NonEComeFromTarget_InM_one_Both", "PdgCase8NonEComeFromTarget_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(PdgCase8NonEComeFromTarget_InM_one_Both);
  PdgCase8NonE_NOT_FromTarget_InM_one_Both = new TH1D(
    "PdgCase8NonE_NOT_FromTarget_InM_one_Both", "PdgCase8NonE_NOT_FromTarget_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(PdgCase8NonE_NOT_FromTarget_InM_one_Both);
  PdgCase8motherNonE_InM_one_Both =
    new TH1D("PdgCase8motherNonE_InM_one_Both", "PdgCase8motherNonE_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(PdgCase8motherNonE_InM_one_Both);
  Case8ElFromDalitz_InM_one_Both = new TH1D(
    "Case8ElFromDalitz_InM_one_Both", "Case8ElFromDalitz_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(Case8ElFromDalitz_InM_one_Both);
  Case8NonElFrom_pn_InM_one_Both = new TH1D(
    "Case8NonElFrom_pn_InM_one_Both", "Case8NonElFrom_pn_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(Case8NonElFrom_pn_InM_one_Both);
  Case8NonElFrom_eta_InM_one_Both = new TH1D(
    "Case8NonElFrom_eta_InM_one_Both", "Case8NonElFrom_eta_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(Case8NonElFrom_eta_InM_one_Both);
  Case8NonElFrom_kaon_InM_one_Both =
    new TH1D("Case8NonElFrom_kaon_InM_one_Both", "Case8NonElFrom_kaon_InM_one_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(Case8NonElFrom_kaon_InM_one_Both);
  sameMIDcase8NonEPdg_InM_one_Both =
    new TH1D("sameMIDcase8NonEPdg_InM_one_Both", "sameMIDcase8NonEPdg_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEPdg_InM_one_Both);
  sameMIDcase8NonEMotherPdg_InM_one_Both = new TH1D("sameMIDcase8NonEMotherPdg_InM_one_Both",
                                                    "sameMIDcase8NonEMotherPdg_InM_one_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEMotherPdg_InM_one_Both);
  sameMIDcase8NonEMotherIM_InM_one_Both =
    new TH1D("sameMIDcase8NonEMotherIM_InM_one_Both",
             "sameMIDcase8NonEMotherIM_InM_one_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEMotherIM_InM_one_Both);
  sameMIDcase8NonEPdgFromTarget_InM_one_Both =
    new TH1D("sameMIDcase8NonEPdgFromTarget_InM_one_Both", "sameMIDcase8NonEPdgFromTarget_InM_one_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEPdgFromTarget_InM_one_Both);
  sameMIDcase8NonEComeFromTargetIM_InM_one_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_one_Both",
             "sameMIDcase8NonEComeFromTargetIM_InM_one_Both; invariant mass in "
             "GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEComeFromTargetIM_InM_one_Both);
  sameMIDcase8NonEComeFromTargetP_InM_one_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_one_Both",
             "sameMIDcase8NonEComeFromTargetP_InM_one_Both; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEComeFromTargetP_InM_one_Both);
  sameMIDcase8NonEComeFromTargetPt_InM_one_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_one_Both",
             "sameMIDcase8NonEComeFromTargetPt_InM_one_Both; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_one_Both.push_back(sameMIDcase8NonEComeFromTargetPt_InM_one_Both);
  //Both two
  BG1_InM_two_Both = new TH1D("BG1_InM_two_Both", "BG1_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG1_InM_two_Both);
  BG2_InM_two_Both = new TH1D("BG2_InM_two_Both", "BG2_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG2_InM_two_Both);
  BG3_InM_two_Both = new TH1D("BG3_InM_two_Both", "BG3_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG3_InM_two_Both);
  BG4_InM_two_Both = new TH1D("BG4_InM_two_Both", "BG4_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG4_InM_two_Both);
  BG5_InM_two_Both = new TH1D("BG5_InM_two_Both", "BG5_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG5_InM_two_Both);
  BG6_InM_two_Both = new TH1D("BG6_InM_two_Both", "BG6_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG6_InM_two_Both);
  BG7_InM_two_Both = new TH1D("BG7_InM_two_Both", "BG7_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG7_InM_two_Both);
  BG8_InM_two_Both = new TH1D("BG8_InM_two_Both", "BG8_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG8_InM_two_Both);
  BG9_InM_two_Both = new TH1D("BG9_InM_two_Both", "BG9_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG9_InM_two_Both);
  BG10_InM_two_Both = new TH1D("BG10_InM_two_Both", "BG10_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(BG10_InM_two_Both);
  PdgCase8_InM_two_Both = new TH1D("PdgCase8_InM_two_Both", "PdgCase8_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(PdgCase8_InM_two_Both);
  PdgCase8mothers_InM_two_Both =
    new TH1D("PdgCase8mothers_InM_two_Both", "PdgCase8mothers_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(PdgCase8mothers_InM_two_Both);
  sameMIDcase8_InM_two_Both =
    new TH1D("sameMIDcase8_InM_two_Both", "sameMIDcase8_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8_InM_two_Both);
  sameGRIDcase8_InM_two_Both =
    new TH1D("sameGRIDcase8_InM_two_Both", "sameGRIDcase8_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(sameGRIDcase8_InM_two_Both);
  Case1ZYPos_InM_two_Both =
    new TH2D("Case1ZYPos_InM_two_Both", "Case1ZYPos_InM_two_Both; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_two_Both.push_back(Case1ZYPos_InM_two_Both);
  sameMIDcase8_mothedPDG_InM_two_Both =
    new TH1D("sameMIDcase8_mothedPDG_InM_two_Both", "sameMIDcase8_mothedPDG_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8_mothedPDG_InM_two_Both);
  PdgCase8NonEComeFromTarget_InM_two_Both = new TH1D(
    "PdgCase8NonEComeFromTarget_InM_two_Both", "PdgCase8NonEComeFromTarget_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(PdgCase8NonEComeFromTarget_InM_two_Both);
  PdgCase8NonE_NOT_FromTarget_InM_two_Both = new TH1D(
    "PdgCase8NonE_NOT_FromTarget_InM_two_Both", "PdgCase8NonE_NOT_FromTarget_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(PdgCase8NonE_NOT_FromTarget_InM_two_Both);
  PdgCase8motherNonE_InM_two_Both =
    new TH1D("PdgCase8motherNonE_InM_two_Both", "PdgCase8motherNonE_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(PdgCase8motherNonE_InM_two_Both);
  Case8ElFromDalitz_InM_two_Both = new TH1D(
    "Case8ElFromDalitz_InM_two_Both", "Case8ElFromDalitz_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(Case8ElFromDalitz_InM_two_Both);
  Case8NonElFrom_pn_InM_two_Both = new TH1D(
    "Case8NonElFrom_pn_InM_two_Both", "Case8NonElFrom_pn_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(Case8NonElFrom_pn_InM_two_Both);
  Case8NonElFrom_eta_InM_two_Both = new TH1D(
    "Case8NonElFrom_eta_InM_two_Both", "Case8NonElFrom_eta_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(Case8NonElFrom_eta_InM_two_Both);
  Case8NonElFrom_kaon_InM_two_Both =
    new TH1D("Case8NonElFrom_kaon_InM_two_Both", "Case8NonElFrom_kaon_InM_two_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(Case8NonElFrom_kaon_InM_two_Both);
  sameMIDcase8NonEPdg_InM_two_Both =
    new TH1D("sameMIDcase8NonEPdg_InM_two_Both", "sameMIDcase8NonEPdg_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEPdg_InM_two_Both);
  sameMIDcase8NonEMotherPdg_InM_two_Both = new TH1D("sameMIDcase8NonEMotherPdg_InM_two_Both",
                                                    "sameMIDcase8NonEMotherPdg_InM_two_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEMotherPdg_InM_two_Both);
  sameMIDcase8NonEMotherIM_InM_two_Both =
    new TH1D("sameMIDcase8NonEMotherIM_InM_two_Both",
             "sameMIDcase8NonEMotherIM_InM_two_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEMotherIM_InM_two_Both);
  sameMIDcase8NonEPdgFromTarget_InM_two_Both =
    new TH1D("sameMIDcase8NonEPdgFromTarget_InM_two_Both", "sameMIDcase8NonEPdgFromTarget_InM_two_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEPdgFromTarget_InM_two_Both);
  sameMIDcase8NonEComeFromTargetIM_InM_two_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_two_Both",
             "sameMIDcase8NonEComeFromTargetIM_InM_two_Both; invariant mass in "
             "GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEComeFromTargetIM_InM_two_Both);
  sameMIDcase8NonEComeFromTargetP_InM_two_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_two_Both",
             "sameMIDcase8NonEComeFromTargetP_InM_two_Both; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEComeFromTargetP_InM_two_Both);
  sameMIDcase8NonEComeFromTargetPt_InM_two_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_two_Both",
             "sameMIDcase8NonEComeFromTargetPt_InM_two_Both; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_two_Both.push_back(sameMIDcase8NonEComeFromTargetPt_InM_two_Both);
  //Both onetwo
  BG1_InM_onetwo_Both =
    new TH1D("BG1_InM_onetwo_Both", "BG1_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG1_InM_onetwo_Both);
  BG2_InM_onetwo_Both =
    new TH1D("BG2_InM_onetwo_Both", "BG2_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG2_InM_onetwo_Both);
  BG3_InM_onetwo_Both =
    new TH1D("BG3_InM_onetwo_Both", "BG3_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG3_InM_onetwo_Both);
  BG4_InM_onetwo_Both =
    new TH1D("BG4_InM_onetwo_Both", "BG4_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG4_InM_onetwo_Both);
  BG5_InM_onetwo_Both =
    new TH1D("BG5_InM_onetwo_Both", "BG5_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG5_InM_onetwo_Both);
  BG6_InM_onetwo_Both =
    new TH1D("BG6_InM_onetwo_Both", "BG6_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG6_InM_onetwo_Both);
  BG7_InM_onetwo_Both =
    new TH1D("BG7_InM_onetwo_Both", "BG7_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG7_InM_onetwo_Both);
  BG8_InM_onetwo_Both =
    new TH1D("BG8_InM_onetwo_Both", "BG8_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG8_InM_onetwo_Both);
  BG9_InM_onetwo_Both =
    new TH1D("BG9_InM_onetwo_Both", "BG9_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG9_InM_onetwo_Both);
  BG10_InM_onetwo_Both =
    new TH1D("BG10_InM_onetwo_Both", "BG10_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(BG10_InM_onetwo_Both);
  PdgCase8_InM_onetwo_Both = new TH1D("PdgCase8_InM_onetwo_Both", "PdgCase8_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(PdgCase8_InM_onetwo_Both);
  PdgCase8mothers_InM_onetwo_Both =
    new TH1D("PdgCase8mothers_InM_onetwo_Both", "PdgCase8mothers_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(PdgCase8mothers_InM_onetwo_Both);
  sameMIDcase8_InM_onetwo_Both = new TH1D("sameMIDcase8_InM_onetwo_Both",
                                          "sameMIDcase8_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8_InM_onetwo_Both);
  sameGRIDcase8_InM_onetwo_Both = new TH1D(
    "sameGRIDcase8_InM_onetwo_Both", "sameGRIDcase8_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(sameGRIDcase8_InM_onetwo_Both);
  Case1ZYPos_InM_onetwo_Both =
    new TH2D("Case1ZYPos_InM_onetwo_Both", "Case1ZYPos_InM_onetwo_Both; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_onetwo_Both.push_back(Case1ZYPos_InM_onetwo_Both);
  sameMIDcase8_mothedPDG_InM_onetwo_Both = new TH1D("sameMIDcase8_mothedPDG_InM_onetwo_Both",
                                                    "sameMIDcase8_mothedPDG_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8_mothedPDG_InM_onetwo_Both);
  PdgCase8NonEComeFromTarget_InM_onetwo_Both =
    new TH1D("PdgCase8NonEComeFromTarget_InM_onetwo_Both", "PdgCase8NonEComeFromTarget_InM_onetwo_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(PdgCase8NonEComeFromTarget_InM_onetwo_Both);
  PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both =
    new TH1D("PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both", "PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both; Id ;#", 5000,
             -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both);
  PdgCase8motherNonE_InM_onetwo_Both =
    new TH1D("PdgCase8motherNonE_InM_onetwo_Both", "PdgCase8motherNonE_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(PdgCase8motherNonE_InM_onetwo_Both);
  Case8ElFromDalitz_InM_onetwo_Both =
    new TH1D("Case8ElFromDalitz_InM_onetwo_Both", "Case8ElFromDalitz_InM_onetwo_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(Case8ElFromDalitz_InM_onetwo_Both);
  Case8NonElFrom_pn_InM_onetwo_Both =
    new TH1D("Case8NonElFrom_pn_InM_onetwo_Both", "Case8NonElFrom_pn_InM_onetwo_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(Case8NonElFrom_pn_InM_onetwo_Both);
  Case8NonElFrom_eta_InM_onetwo_Both =
    new TH1D("Case8NonElFrom_eta_InM_onetwo_Both", "Case8NonElFrom_eta_InM_onetwo_Both; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(Case8NonElFrom_eta_InM_onetwo_Both);
  Case8NonElFrom_kaon_InM_onetwo_Both =
    new TH1D("Case8NonElFrom_kaon_InM_onetwo_Both",
             "Case8NonElFrom_kaon_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(Case8NonElFrom_kaon_InM_onetwo_Both);
  sameMIDcase8NonEPdg_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEPdg_InM_onetwo_Both", "sameMIDcase8NonEPdg_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEPdg_InM_onetwo_Both);
  sameMIDcase8NonEMotherPdg_InM_onetwo_Both = new TH1D(
    "sameMIDcase8NonEMotherPdg_InM_onetwo_Both", "sameMIDcase8NonEMotherPdg_InM_onetwo_Both; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEMotherPdg_InM_onetwo_Both);
  sameMIDcase8NonEMotherIM_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEMotherIM_InM_onetwo_Both",
             "sameMIDcase8NonEMotherIM_InM_onetwo_Both; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEMotherIM_InM_onetwo_Both);
  sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both", "sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both; Id ;#",
             5000, -2500, 2500);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both);
  sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both",
             "sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both; invariant mass "
             "in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both);
  sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both",
             "sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both);
  sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both",
             "sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_InM_onetwo_Both.push_back(sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both);

  AnnTruePairs = new TH1D("AnnTruePairs", "AnnTruePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_man_Both.push_back(AnnTruePairs);
  AnnFalsePairs = new TH1D("AnnFalsePairs", "AnnFalsePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_man_Both.push_back(AnnFalsePairs);
  AnnTruePairs_AfterCuts = new TH1D("AnnTruePairs_AfterCuts", "AnnTruePairs; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_man_Both.push_back(AnnTruePairs_AfterCuts);
  AnnFalsePairs_AfterCuts =
    new TH1D("AnnFalsePairs_AfterCuts", "AnnFalsePairs_AfterCuts; Ann value ;#", 100, -1.2, 1.2);
  fHistoList_man_Both.push_back(AnnFalsePairs_AfterCuts);


  // WAC
  Pi0InvMassReco_WAC =
    new TH1D("Pi0InvMassReco_WAC", "Pi0InvMassReco_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_WAC.push_back(Pi0InvMassReco_WAC);
  EMT_InvMass_WAC = new TH1D("EMT_InvMass_WAC", "EMT_InvMass_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_WAC.push_back(EMT_InvMass_WAC);
  Pi0_pt_vs_rap_WAC =
    new TH2D("Pi0_pt_vs_rap_WAC", "Pi0_pt_vs_rap_WAC; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_man_WAC.push_back(Pi0_pt_vs_rap_WAC);
  Pi0_pt_vs_rap_est_WAC =
    new TH2D("Pi0_pt_vs_rap_est_WAC", "Pi0_pt_vs_rap_est_WAC; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 40, 0., 4.);
  fHistoList_man_WAC.push_back(Pi0_pt_vs_rap_est_WAC);
  DalitzPi0_WAC = new TH1D("DalitzPi0_WAC", "DalitzPi0_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_WAC.push_back(DalitzPi0_WAC);
  PhotonsPi0_WAC = new TH1D("PhotonsPi0_WAC", "PhotonsPi0_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_man_WAC.push_back(PhotonsPi0_WAC);
  MultiplicityGamma_WAC =
    new TH2D("MultiplicityGamma_WAC", "MultiplicityGamma_WAC; Nof gammas in event; invariant mass in GeV/c^{2};#", 400,
             0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityGamma_WAC);
  MultiplicityChargedParticles_WAC = new TH2D("MultiplicityChargedParticles_WAC",
                                              "MultiplicityChargedParticles_WAC; Nof charged particles in "
                                              "event; invariant mass in GeV/c^{2};#",
                                              1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity_man_Both.push_back(MultiplicityChargedParticles_WAC);
  BG1_InM_WAC = new TH1D("BG1_InM_WAC", "BG1_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG1_InM_WAC);
  BG2_InM_WAC = new TH1D("BG2_InM_WAC", "BG2_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG2_InM_WAC);
  BG3_InM_WAC = new TH1D("BG3_InM_WAC", "BG3_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG3_InM_WAC);
  BG4_InM_WAC = new TH1D("BG4_InM_WAC", "BG4_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG4_InM_WAC);
  BG5_InM_WAC = new TH1D("BG5_InM_WAC", "BG5_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG5_InM_WAC);
  BG6_InM_WAC = new TH1D("BG6_InM_WAC", "BG6_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG6_InM_WAC);
  BG7_InM_WAC = new TH1D("BG7_InM_WAC", "BG7_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG7_InM_WAC);
  BG8_InM_WAC = new TH1D("BG8_InM_WAC", "BG8_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG8_InM_WAC);
  BG9_InM_WAC = new TH1D("BG9_InM_WAC", "BG9_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG9_InM_WAC);
  BG10_InM_WAC = new TH1D("BG10_InM_WAC", "BG10_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(BG10_InM_WAC);
  PdgCase8_InM_WAC = new TH1D("PdgCase8_InM_WAC", "PdgCase8_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(PdgCase8_InM_WAC);
  PdgCase8mothers_InM_WAC = new TH1D("PdgCase8mothers_InM_WAC", "PdgCase8mothers_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(PdgCase8mothers_InM_WAC);
  sameMIDcase8_InM_WAC =
    new TH1D("sameMIDcase8_InM_WAC", "sameMIDcase8_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8_InM_WAC);
  sameGRIDcase8_InM_WAC =
    new TH1D("sameGRIDcase8_InM_WAC", "sameGRIDcase8_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(sameGRIDcase8_InM_WAC);
  Case1ZYPos_InM_WAC = new TH2D("Case1ZYPos_InM_WAC", "Case1ZYPos_InM_WAC; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_InM_WAC.push_back(Case1ZYPos_InM_WAC);
  sameMIDcase8_mothedPDG_InM_WAC =
    new TH1D("sameMIDcase8_mothedPDG_InM_WAC", "sameMIDcase8_mothedPDG_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8_mothedPDG_InM_WAC);
  PdgCase8NonEComeFromTarget_InM_WAC =
    new TH1D("PdgCase8NonEComeFromTarget_InM_WAC", "PdgCase8NonEComeFromTarget_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(PdgCase8NonEComeFromTarget_InM_WAC);
  PdgCase8NonE_NOT_FromTarget_InM_WAC =
    new TH1D("PdgCase8NonE_NOT_FromTarget_InM_WAC", "PdgCase8NonE_NOT_FromTarget_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(PdgCase8NonE_NOT_FromTarget_InM_WAC);
  PdgCase8motherNonE_InM_WAC =
    new TH1D("PdgCase8motherNonE_InM_WAC", "PdgCase8motherNonE_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(PdgCase8motherNonE_InM_WAC);
  Case8ElFromDalitz_InM_WAC =
    new TH1D("Case8ElFromDalitz_InM_WAC", "Case8ElFromDalitz_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(Case8ElFromDalitz_InM_WAC);
  Case8NonElFrom_pn_InM_WAC =
    new TH1D("Case8NonElFrom_pn_InM_WAC", "Case8NonElFrom_pn_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(Case8NonElFrom_pn_InM_WAC);
  Case8NonElFrom_eta_InM_WAC =
    new TH1D("Case8NonElFrom_eta_InM_WAC", "Case8NonElFrom_eta_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(Case8NonElFrom_eta_InM_WAC);
  Case8NonElFrom_kaon_InM_WAC =
    new TH1D("Case8NonElFrom_kaon_InM_WAC", "Case8NonElFrom_kaon_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(Case8NonElFrom_kaon_InM_WAC);
  sameMIDcase8NonEPdg_InM_WAC =
    new TH1D("sameMIDcase8NonEPdg_InM_WAC", "sameMIDcase8NonEPdg_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEPdg_InM_WAC);
  sameMIDcase8NonEMotherPdg_InM_WAC =
    new TH1D("sameMIDcase8NonEMotherPdg_InM_WAC", "sameMIDcase8NonEMotherPdg_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEMotherPdg_InM_WAC);
  sameMIDcase8NonEMotherIM_InM_WAC =
    new TH1D("sameMIDcase8NonEMotherIM_InM_WAC", "sameMIDcase8NonEMotherIM_InM_WAC; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEMotherIM_InM_WAC);
  sameMIDcase8NonEPdgFromTarget_InM_WAC = new TH1D("sameMIDcase8NonEPdgFromTarget_InM_WAC",
                                                   "sameMIDcase8NonEPdgFromTarget_InM_WAC; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEPdgFromTarget_InM_WAC);
  sameMIDcase8NonEComeFromTargetIM_InM_WAC =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_InM_WAC",
             "sameMIDcase8NonEComeFromTargetIM_InM_WAC; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEComeFromTargetIM_InM_WAC);
  sameMIDcase8NonEComeFromTargetP_InM_WAC =
    new TH1D("sameMIDcase8NonEComeFromTargetP_InM_WAC", "sameMIDcase8NonEComeFromTargetP_InM_WAC; P in GeV/c^{2} ;#",
             200, 0, 10);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEComeFromTargetP_InM_WAC);
  sameMIDcase8NonEComeFromTargetPt_InM_WAC =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_InM_WAC", "sameMIDcase8NonEComeFromTargetPt_InM_WAC; Pt in GeV/c^{2} ;#",
             200, 0, 10);
  fHistoList_bg_InM_WAC.push_back(sameMIDcase8NonEComeFromTargetPt_InM_WAC);


  Chi2_for_Primary = new TH1D("Chi2_for_Primary", "Chi2_for_Primary; #chi^{2};entries", 200, 0, 20);
  fHistoList_manual.push_back(Chi2_for_Primary);
  Chi2_for_Secondary = new TH1D("Chi2_for_Secondary", "Chi2_for_Secondary; #chi^{2};entries", 200, 0, 20);
  fHistoList_manual.push_back(Chi2_for_Secondary);
}
