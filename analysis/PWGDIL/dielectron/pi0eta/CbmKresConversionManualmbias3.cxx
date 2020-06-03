/**
 *    file CbmKresConversionManualmbias3.cxx
 *
 *    author Ievgenii Kres
 *    date 19.05.2017
 *    modified 30.01.2020
 *
 *
 *    Central class for the pi^0 conversion analysis, when the collision of two nuclei is not central. The same analysis as in "CbmKresConversionManual.cxx" (but most probably here is outdated)
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *
 *    Dedicated for collisions in my case in range (6 < b <= 10 fm)
 *
 *    Most probably here is outdated !!!!
 **/

#include "CbmKresConversionManualmbias3.h"

#include "CbmKresFunctions.h"
#include "CbmKresConversionBG.h"

#include "FairRootManager.h"
#include "CbmMCTrack.h"
#include "CbmGlobalTrack.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmRichRingLight.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmKFParticleInterface.h"
#include "KFParticle.h"
#include "CbmMvdHit.h"
#include "CbmStsHit.h"

#include <iostream>
#include "TDirectory.h"


using namespace std;

CbmKresConversionManualmbias3::CbmKresConversionManualmbias3()
  : fMcTracks(nullptr),
	fGlobalTracks(nullptr),
	fStsTracks(nullptr),
	fStsTrackMatches(nullptr),
	fRichProjections(nullptr),
	fRichRings(nullptr),
	fRichRingMatches(nullptr),
	fRichHits(nullptr),
	fArrayMvdHit(nullptr),
	fArrayStsHit(nullptr),
	fArrayCentrality(nullptr),
	fPrimVertex(nullptr),
	fKFVertex(),
	fTauFit(nullptr),
	fAnaBG(nullptr),
	VStsTrack_minus_Outside_mbias3(),
	VMCtracks_minus_Outside_mbias3(),
	VRings_minus_Outside_mbias3(),
	VStsIndex_minus_Outside_mbias3(),
	VRichRing_minus_Outside_mbias3(),
	VStsTrack_plus_Outside_mbias3(),
	VMCtracks_plus_Outside_mbias3(),
	VRings_plus_Outside_mbias3(),
	VStsIndex_plus_Outside_mbias3(),
	VRichRing_plus_Outside_mbias3(),
	VMCtracks_minus_Target_mbias3(),
	VStsTrack_minus_Target_mbias3(),
	VMomenta_minus_Target_mbias3(),
	VRings_minus_Target_mbias3(),
	VStsIndex_minus_Target_mbias3(),
	VRichRing_minus_Target_mbias3(),
	VMCtracks_plus_Target_mbias3(),
	VStsTrack_plus_Target_mbias3(),
	VMomenta_plus_Target_mbias3(),
	VRings_plus_Target_mbias3(),
	VStsIndex_plus_Target_mbias3(),
	VRichRing_plus_Target_mbias3(),
	frefmomenta(),
	frefId(),
	fMCtracks(),
	EMT_man_Event_Target_mbias3(),
	EMT_man_pair_momenta_Target_mbias3(),
	EMT_man_NofRings_Target_mbias3(),
	EMT_man_Event_Outside_mbias3(),
	EMT_man_pair_momenta_Outside_mbias3(),
	EMT_man_NofRings_Outside_mbias3(),
	EMT_man_Event_Both_mbias3(),
	EMT_man_pair_momenta_Both_mbias3(),
	EMT_man_NofRings_Both_mbias3(),
	Gammas_all_Target_mbias3(),
	Gammas_zero_Target_mbias3(),
	Gammas_one_Target_mbias3(),
	Gammas_two_Target_mbias3(),
	Gammas_onetwo_Target_mbias3(),
	Gammas_stsIndex_all_Target_mbias3(),
	Gammas_stsIndex_zero_Target_mbias3(),
	Gammas_stsIndex_one_Target_mbias3(),
	Gammas_stsIndex_two_Target_mbias3(),
	Gammas_stsIndex_onetwo_Target_mbias3(),
	Gammas_MC_all_Target_mbias3(),
	Gammas_MC_zero_Target_mbias3(),
	Gammas_MC_one_Target_mbias3(),
	Gammas_MC_two_Target_mbias3(),
	Gammas_MC_onetwo_Target_mbias3(),
	Gammas_all_Outside_mbias3(),
	Gammas_zero_Outside_mbias3(),
	Gammas_one_Outside_mbias3(),
	Gammas_two_Outside_mbias3(),
	Gammas_onetwo_Outside_mbias3(),
	Gammas_stsIndex_all_Outside_mbias3(),
	Gammas_stsIndex_zero_Outside_mbias3(),
	Gammas_stsIndex_one_Outside_mbias3(),
	Gammas_stsIndex_two_Outside_mbias3(),
	Gammas_stsIndex_onetwo_Outside_mbias3(),
	Gammas_MC_all_Outside_mbias3(),
	Gammas_MC_zero_Outside_mbias3(),
	Gammas_MC_one_Outside_mbias3(),
	Gammas_MC_two_Outside_mbias3(),
	Gammas_MC_onetwo_Outside_mbias3(),
	Gammas_all_Both_mbias3(),
	Gammas_zero_Both_mbias3(),
	Gammas_one_Both_mbias3(),
	Gammas_two_Both_mbias3(),
	Gammas_onetwo_Both_mbias3(),
	Gammas_stsIndex_all_Both_mbias3(),
	Gammas_stsIndex_zero_Both_mbias3(),
	Gammas_stsIndex_one_Both_mbias3(),
	Gammas_stsIndex_two_Both_mbias3(),
	Gammas_stsIndex_onetwo_Both_mbias3(),
	Gammas_MC_all_Both_mbias3(),
	Gammas_MC_zero_Both_mbias3(),
	Gammas_MC_one_Both_mbias3(),
	Gammas_MC_two_Both_mbias3(),
	Gammas_MC_onetwo_Both_mbias3(),
	fHistoList_man_cuts_Both_mbias3(),
	InvMass_vs_OA_candidates_Both_mbias3(nullptr),
	InvMass_vs_OA_fromPi0_Both_mbias3(nullptr),
	GammasInvMass_candidates_Both_mbias3(nullptr),
	GammasOA_candidates_Both_mbias3(nullptr),
	GammasInvMass_fromPi0_Both_mbias3(nullptr),
	GammasOA_fromPi0_Both_mbias3(nullptr),
	PlaneAngles_last_candidates_Both_mbias3(nullptr),
	PlaneAngles_last_fromPi0_Both_mbias3(nullptr),
	PlaneAngles_first_candidates_Both_mbias3(nullptr),
	PlaneAngles_first_fromPi0_Both_mbias3(nullptr),
	fHistoList_man_cuts_Target_mbias3(),
	InvMass_vs_OA_candidates_Target_mbias3(nullptr),
	InvMass_vs_OA_fromPi0_Target_mbias3(nullptr),
	GammasInvMass_candidates_Target_mbias3(nullptr),
	GammasOA_candidates_Target_mbias3(nullptr),
	GammasInvMass_fromPi0_Target_mbias3(nullptr),
	GammasOA_fromPi0_Target_mbias3(nullptr),
	PlaneAngles_last_candidates_Target_mbias3(nullptr),
	PlaneAngles_last_fromPi0_Target_mbias3(nullptr),
	PlaneAngles_first_candidates_Target_mbias3(nullptr),
	PlaneAngles_first_fromPi0_Target_mbias3(nullptr),
	fHistoList_man_cuts_Outside_mbias3(),
	InvMass_vs_OA_candidates_Outside_mbias3(nullptr),
	InvMass_vs_OA_fromPi0_Outside_mbias3(nullptr),
	GammasInvMass_candidates_Outside_mbias3(nullptr),
	GammasOA_candidates_Outside_mbias3(nullptr),
	GammasInvMass_fromPi0_Outside_mbias3(nullptr),
	GammasOA_fromPi0_Outside_mbias3(nullptr),
	PlaneAngles_last_candidates_Outside_mbias3(nullptr),
	PlaneAngles_last_fromPi0_Outside_mbias3(nullptr),
	PlaneAngles_first_candidates_Outside_mbias3(nullptr),
	PlaneAngles_first_fromPi0_Outside_mbias3(nullptr),
	fHistoList_man_all_Target_mbias3(),
	GammaInvMassReco_all_Target_mbias3(nullptr),
	GammaOpeningAngleReco_all_Target_mbias3(nullptr),
	Pdg_all_Target_mbias3(nullptr),
	P_reco_all_Target_mbias3(nullptr),
	Pt_reco_all_Target_mbias3(nullptr),
	Pi0InvMassReco_all_Target_mbias3(nullptr),
	EMT_InvMass_all_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_all_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_est_all_Target_mbias3(nullptr),
	fHistoList_man_zero_Target_mbias3(),
	GammaInvMassReco_zero_Target_mbias3(nullptr),
	GammaOpeningAngleReco_zero_Target_mbias3(nullptr),
	Pdg_zero_Target_mbias3(nullptr),
	P_reco_zero_Target_mbias3(nullptr),
	Pt_reco_zero_Target_mbias3(nullptr),
	Pi0InvMassReco_zero_Target_mbias3(nullptr),
	EMT_InvMass_zero_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_zero_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_est_zero_Target_mbias3(nullptr),
	fHistoList_man_one_Target_mbias3(),
	GammaInvMassReco_one_Target_mbias3(nullptr),
	GammaOpeningAngleReco_one_Target_mbias3(nullptr),
	Pdg_one_Target_mbias3(nullptr),
	P_reco_one_Target_mbias3(nullptr),
	Pt_reco_one_Target_mbias3(nullptr),
	Pi0InvMassReco_one_Target_mbias3(nullptr),
	EMT_InvMass_one_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_one_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_est_one_Target_mbias3(nullptr),
	fHistoList_man_two_Target_mbias3(),
	GammaInvMassReco_two_Target_mbias3(nullptr),
	GammaOpeningAngleReco_two_Target_mbias3(nullptr),
	Pdg_two_Target_mbias3(nullptr),
	P_reco_two_Target_mbias3(nullptr),
	Pt_reco_two_Target_mbias3(nullptr),
	Pi0InvMassReco_two_Target_mbias3(nullptr),
	EMT_InvMass_two_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_two_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_est_two_Target_mbias3(nullptr),
	fHistoList_man_onetwo_Target_mbias3(),
	GammaInvMassReco_onetwo_Target_mbias3(nullptr),
	GammaOpeningAngleReco_onetwo_Target_mbias3(nullptr),
	Pdg_onetwo_Target_mbias3(nullptr),
	P_reco_onetwo_Target_mbias3(nullptr),
	Pt_reco_onetwo_Target_mbias3(nullptr),
	Pi0InvMassReco_onetwo_Target_mbias3(nullptr),
	EMT_InvMass_onetwo_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_onetwo_Target_mbias3(nullptr),
	Pi0_pt_vs_rap_est_onetwo_Target_mbias3(nullptr),
	fHistoList_man_all_Outside_mbias3(),
	GammaInvMassReco_all_Outside_mbias3(nullptr),
	GammaOpeningAngleReco_all_Outside_mbias3(nullptr),
	Pdg_all_Outside_mbias3(nullptr),
	P_reco_all_Outside_mbias3(nullptr),
	Pt_reco_all_Outside_mbias3(nullptr),
	Pi0InvMassReco_all_Outside_mbias3(nullptr),
	EMT_InvMass_all_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_all_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_est_all_Outside_mbias3(nullptr),
	fHistoList_man_zero_Outside_mbias3(),
	GammaInvMassReco_zero_Outside_mbias3(nullptr),
	GammaOpeningAngleReco_zero_Outside_mbias3(nullptr),
	Pdg_zero_Outside_mbias3(nullptr),
	P_reco_zero_Outside_mbias3(nullptr),
	Pt_reco_zero_Outside_mbias3(nullptr),
	Pi0InvMassReco_zero_Outside_mbias3(nullptr),
	EMT_InvMass_zero_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_zero_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_est_zero_Outside_mbias3(nullptr),
	fHistoList_man_one_Outside_mbias3(),
	GammaInvMassReco_one_Outside_mbias3(nullptr),
	GammaOpeningAngleReco_one_Outside_mbias3(nullptr),
	Pdg_one_Outside_mbias3(nullptr),
	P_reco_one_Outside_mbias3(nullptr),
	Pt_reco_one_Outside_mbias3(nullptr),
	Pi0InvMassReco_one_Outside_mbias3(nullptr),
	EMT_InvMass_one_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_one_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_est_one_Outside_mbias3(nullptr),
	fHistoList_man_two_Outside_mbias3(),
	GammaInvMassReco_two_Outside_mbias3(nullptr),
	GammaOpeningAngleReco_two_Outside_mbias3(nullptr),
	Pdg_two_Outside_mbias3(nullptr),
	P_reco_two_Outside_mbias3(nullptr),
	Pt_reco_two_Outside_mbias3(nullptr),
	Pi0InvMassReco_two_Outside_mbias3(nullptr),
	EMT_InvMass_two_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_two_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_est_two_Outside_mbias3(nullptr),
	fHistoList_man_onetwo_Outside_mbias3(),
	GammaInvMassReco_onetwo_Outside_mbias3(nullptr),
	GammaOpeningAngleReco_onetwo_Outside_mbias3(nullptr),
	Pdg_onetwo_Outside_mbias3(nullptr),
	P_reco_onetwo_Outside_mbias3(nullptr),
	Pt_reco_onetwo_Outside_mbias3(nullptr),
	Pi0InvMassReco_onetwo_Outside_mbias3(nullptr),
	EMT_InvMass_onetwo_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_onetwo_Outside_mbias3(nullptr),
	Pi0_pt_vs_rap_est_onetwo_Outside_mbias3(nullptr),
	fHistoList_man_all_Both_mbias3(),
	GammaInvMassReco_all_Both_mbias3(nullptr),
	GammaOpeningAngleReco_all_Both_mbias3(nullptr),
	Pdg_all_Both_mbias3(nullptr),
	P_reco_all_Both_mbias3(nullptr),
	Pt_reco_all_Both_mbias3(nullptr),
	Pi0InvMassReco_all_Both_mbias3(nullptr),
	EMT_InvMass_all_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_all_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_est_all_Both_mbias3(nullptr),
	fHistoList_man_zero_Both_mbias3(),
	GammaInvMassReco_zero_Both_mbias3(nullptr),
	GammaOpeningAngleReco_zero_Both_mbias3(nullptr),
	Pdg_zero_Both_mbias3(nullptr),
	P_reco_zero_Both_mbias3(nullptr),
	Pt_reco_zero_Both_mbias3(nullptr),
	Pi0InvMassReco_zero_Both_mbias3(nullptr),
	EMT_InvMass_zero_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_zero_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_est_zero_Both_mbias3(nullptr),
	fHistoList_man_one_Both_mbias3(),
	GammaInvMassReco_one_Both_mbias3(nullptr),
	GammaOpeningAngleReco_one_Both_mbias3(nullptr),
	Pdg_one_Both_mbias3(nullptr),
	P_reco_one_Both_mbias3(nullptr),
	Pt_reco_one_Both_mbias3(nullptr),
	Pi0InvMassReco_one_Both_mbias3(nullptr),
	EMT_InvMass_one_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_one_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_est_one_Both_mbias3(nullptr),
	fHistoList_man_two_Both_mbias3(),
	GammaInvMassReco_two_Both_mbias3(nullptr),
	GammaOpeningAngleReco_two_Both_mbias3(nullptr),
	Pdg_two_Both_mbias3(nullptr),
	P_reco_two_Both_mbias3(nullptr),
	Pt_reco_two_Both_mbias3(nullptr),
	Pi0InvMassReco_two_Both_mbias3(nullptr),
	EMT_InvMass_two_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_two_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_est_two_Both_mbias3(nullptr),
	fHistoList_man_onetwo_Both_mbias3(),
	GammaInvMassReco_onetwo_Both_mbias3(nullptr),
	GammaOpeningAngleReco_onetwo_Both_mbias3(nullptr),
	Pdg_onetwo_Both_mbias3(nullptr),
	P_reco_onetwo_Both_mbias3(nullptr),
	Pt_reco_onetwo_Both_mbias3(nullptr),
	Pi0InvMassReco_onetwo_Both_mbias3(nullptr),
	EMT_InvMass_onetwo_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_onetwo_Both_mbias3(nullptr),
	Pi0_pt_vs_rap_est_onetwo_Both_mbias3(nullptr),
	fHistoList_man_Both_mbias3(),
	Pdg_vs_Distance_mbias3(nullptr),
	P_vs_Distance_mbias3(nullptr),
	fHistoList_multiplicity_man_Target_mbias3(),
	MultiplicityGamma_all_Target_mbias3(nullptr),
	MultiplicityGamma_zero_Target_mbias3(nullptr),
	MultiplicityGamma_one_Target_mbias3(nullptr),
	MultiplicityGamma_two_Target_mbias3(nullptr),
	MultiplicityGamma_onetwo_Target_mbias3(nullptr),
	MultiplicityChargedParticles_all_Target_mbias3(nullptr),
	MultiplicityChargedParticles_zero_Target_mbias3(nullptr),
	MultiplicityChargedParticles_one_Target_mbias3(nullptr),
	MultiplicityChargedParticles_two_Target_mbias3(nullptr),
	MultiplicityChargedParticles_onetwo_Target_mbias3(nullptr),
	fHistoList_multiplicity_man_Outside_mbias3(),
	MultiplicityGamma_all_Outside_mbias3(nullptr),
	MultiplicityGamma_zero_Outside_mbias3(nullptr),
	MultiplicityGamma_one_Outside_mbias3(nullptr),
	MultiplicityGamma_two_Outside_mbias3(nullptr),
	MultiplicityGamma_onetwo_Outside_mbias3(nullptr),
	MultiplicityChargedParticles_all_Outside_mbias3(nullptr),
	MultiplicityChargedParticles_zero_Outside_mbias3(nullptr),
	MultiplicityChargedParticles_one_Outside_mbias3(nullptr),
	MultiplicityChargedParticles_two_Outside_mbias3(nullptr),
	MultiplicityChargedParticles_onetwo_Outside_mbias3(nullptr),
	fHistoList_multiplicity_man_Both_mbias3(),
	MultiplicityGamma_all_Both_mbias3(nullptr),
	MultiplicityGamma_zero_Both_mbias3(nullptr),
	MultiplicityGamma_one_Both_mbias3(nullptr),
	MultiplicityGamma_two_Both_mbias3(nullptr),
	MultiplicityGamma_onetwo_Both_mbias3(nullptr),
	MultiplicityChargedParticles_all_Both_mbias3(nullptr),
	MultiplicityChargedParticles_zero_Both_mbias3(nullptr),
	MultiplicityChargedParticles_one_Both_mbias3(nullptr),
	MultiplicityChargedParticles_two_Both_mbias3(nullptr),
	MultiplicityChargedParticles_onetwo_Both_mbias3(nullptr),
	fHistoList_rap_vs_pt_InM_mbias3(),
	rap_vs_Pt_InM_1_mbias3(nullptr),
	rap_vs_Pt_InM_2_mbias3(nullptr), 
	rap_vs_Pt_InM_3_mbias3(nullptr),
	rap_vs_Pt_InM_4_mbias3(nullptr),
	rap_vs_Pt_InM_5_mbias3(nullptr),
	rap_vs_Pt_InM_6_mbias3(nullptr),
	rap_vs_Pt_InM_7_mbias3(nullptr),
	rap_vs_Pt_InM_8_mbias3(nullptr),
	rap_vs_Pt_InM_9_mbias3(nullptr),
	rap_vs_Pt_InM_10_mbias3(nullptr),
	rap_vs_Pt_InM_11_mbias3(nullptr),
	rap_vs_Pt_InM_12_mbias3(nullptr),
	rap_vs_Pt_InM_13_mbias3(nullptr),
	rap_vs_Pt_InM_14_mbias3(nullptr),
	rap_vs_Pt_InM_15_mbias3(nullptr),
	rap_vs_Pt_InM_16_mbias3(nullptr),
	rap_vs_Pt_InM_17_mbias3(nullptr),
	rap_vs_Pt_InM_18_mbias3(nullptr),
	rap_vs_Pt_InM_19_mbias3(nullptr),
	rap_vs_Pt_InM_20_mbias3(nullptr),
	rap_vs_Pt_InM_21_mbias3(nullptr),
	rap_vs_Pt_InM_22_mbias3(nullptr),
	rap_vs_Pt_InM_23_mbias3(nullptr),
	rap_vs_Pt_InM_24_mbias3(nullptr),
	rap_vs_Pt_InM_25_mbias3(nullptr),
	rap_vs_Pt_InM_26_mbias3(nullptr),
	rap_vs_Pt_InM_27_mbias3(nullptr),
	rap_vs_Pt_InM_28_mbias3(nullptr),
	rap_vs_Pt_InM_29_mbias3(nullptr),
	rap_vs_Pt_InM_30_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_1_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_2_mbias3(nullptr), 
	rap_vs_Pt_InM_mixing_3_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_4_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_5_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_6_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_7_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_8_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_9_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_10_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_11_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_12_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_13_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_14_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_15_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_16_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_17_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_18_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_19_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_20_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_21_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_22_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_23_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_24_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_25_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_26_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_27_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_28_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_29_mbias3(nullptr),
	rap_vs_Pt_InM_mixing_30_mbias3(nullptr),
	fHistoList_bg_InM_all_Target_mbias3(),
	fHistoList_bg_InM_zero_Target_mbias3(),
	fHistoList_bg_InM_one_Target_mbias3(),
	fHistoList_bg_InM_two_Target_mbias3(),
	fHistoList_bg_InM_onetwo_Target_mbias3(),
	fHistoList_bg_InM_all_Outside_mbias3(),
	fHistoList_bg_InM_zero_Outside_mbias3(),
	fHistoList_bg_InM_one_Outside_mbias3(),
	fHistoList_bg_InM_two_Outside_mbias3(),
	fHistoList_bg_InM_onetwo_Outside_mbias3(),
	fHistoList_bg_InM_all_Both_mbias3(),
	BG1_InM_all_Both_mbias3(nullptr),
	BG2_InM_all_Both_mbias3(nullptr),
	BG3_InM_all_Both_mbias3(nullptr),
	BG4_InM_all_Both_mbias3(nullptr),
	BG5_InM_all_Both_mbias3(nullptr),
	BG6_InM_all_Both_mbias3(nullptr),
	BG7_InM_all_Both_mbias3(nullptr),
	BG8_InM_all_Both_mbias3(nullptr),
	BG9_InM_all_Both_mbias3(nullptr),
	BG10_InM_all_Both_mbias3(nullptr),
	PdgCase8_InM_all_Both_mbias3(nullptr),
	PdgCase8mothers_InM_all_Both_mbias3(nullptr),
	sameMIDcase8_InM_all_Both_mbias3(nullptr),
	sameGRIDcase8_InM_all_Both_mbias3(nullptr),
	Case1ZYPos_InM_all_Both_mbias3(nullptr),
	sameMIDcase8_mothedPDG_InM_all_Both_mbias3(nullptr),
	PdgCase8NonEComeFromTarget_mbias3_InM_all_Both_mbias3(nullptr),
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_all_Both_mbias3(nullptr),
	PdgCase8motherNonE_InM_all_Both_mbias3(nullptr),
	Case8ElFromDalitz_InM_all_Both_mbias3(nullptr),
	Case8NonElFrom_pn_InM_all_Both_mbias3(nullptr),
	Case8NonElFrom_eta_InM_all_Both_mbias3(nullptr),
	Case8NonElFrom_kaon_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEPdg_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherPdg_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherIM_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_all_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_all_Both_mbias3(nullptr),
	fHistoList_bg_InM_zero_Both_mbias3(),
	BG1_InM_zero_Both_mbias3(nullptr),
	BG2_InM_zero_Both_mbias3(nullptr),
	BG3_InM_zero_Both_mbias3(nullptr),
	BG4_InM_zero_Both_mbias3(nullptr),
	BG5_InM_zero_Both_mbias3(nullptr),
	BG6_InM_zero_Both_mbias3(nullptr),
	BG7_InM_zero_Both_mbias3(nullptr),
	BG8_InM_zero_Both_mbias3(nullptr),
	BG9_InM_zero_Both_mbias3(nullptr),
	BG10_InM_zero_Both_mbias3(nullptr),
	PdgCase8_InM_zero_Both_mbias3(nullptr),
	PdgCase8mothers_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8_InM_zero_Both_mbias3(nullptr),
	sameGRIDcase8_InM_zero_Both_mbias3(nullptr),
	Case1ZYPos_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8_mothedPDG_InM_zero_Both_mbias3(nullptr),
	PdgCase8NonEComeFromTarget_mbias3_InM_zero_Both_mbias3(nullptr),
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_zero_Both_mbias3(nullptr),
	PdgCase8motherNonE_InM_zero_Both_mbias3(nullptr),
	Case8ElFromDalitz_InM_zero_Both_mbias3(nullptr),
	Case8NonElFrom_pn_InM_zero_Both_mbias3(nullptr),
	Case8NonElFrom_eta_InM_zero_Both_mbias3(nullptr),
	Case8NonElFrom_kaon_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEPdg_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherIM_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_zero_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_zero_Both_mbias3(nullptr),
	BG1_InM_one_Both_mbias3(nullptr),
	BG2_InM_one_Both_mbias3(nullptr),
	BG3_InM_one_Both_mbias3(nullptr),
	BG4_InM_one_Both_mbias3(nullptr),
	BG5_InM_one_Both_mbias3(nullptr),
	BG6_InM_one_Both_mbias3(nullptr),
	BG7_InM_one_Both_mbias3(nullptr),
	BG8_InM_one_Both_mbias3(nullptr),
	BG9_InM_one_Both_mbias3(nullptr),
	BG10_InM_one_Both_mbias3(nullptr),
	PdgCase8_InM_one_Both_mbias3(nullptr),
	PdgCase8mothers_InM_one_Both_mbias3(nullptr),
	sameMIDcase8_InM_one_Both_mbias3(nullptr),
	sameGRIDcase8_InM_one_Both_mbias3(nullptr),
	Case1ZYPos_InM_one_Both_mbias3(nullptr),
	sameMIDcase8_mothedPDG_InM_one_Both_mbias3(nullptr),
	PdgCase8NonEComeFromTarget_mbias3_InM_one_Both_mbias3(nullptr),
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_one_Both_mbias3(nullptr),
	PdgCase8motherNonE_InM_one_Both_mbias3(nullptr),
	Case8ElFromDalitz_InM_one_Both_mbias3(nullptr),
	Case8NonElFrom_pn_InM_one_Both_mbias3(nullptr),
	Case8NonElFrom_eta_InM_one_Both_mbias3(nullptr),
	Case8NonElFrom_kaon_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEPdg_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherPdg_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherIM_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_one_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_one_Both_mbias3(nullptr),
	fHistoList_bg_InM_two_Both_mbias3(),
	BG1_InM_two_Both_mbias3(nullptr),
	BG2_InM_two_Both_mbias3(nullptr),
	BG3_InM_two_Both_mbias3(nullptr),
	BG4_InM_two_Both_mbias3(nullptr),
	BG5_InM_two_Both_mbias3(nullptr),
	BG6_InM_two_Both_mbias3(nullptr),
	BG7_InM_two_Both_mbias3(nullptr),
	BG8_InM_two_Both_mbias3(nullptr),
	BG9_InM_two_Both_mbias3(nullptr),
	BG10_InM_two_Both_mbias3(nullptr),
	PdgCase8_InM_two_Both_mbias3(nullptr),
	PdgCase8mothers_InM_two_Both_mbias3(nullptr),
	sameMIDcase8_InM_two_Both_mbias3(nullptr),
	sameGRIDcase8_InM_two_Both_mbias3(nullptr),
	Case1ZYPos_InM_two_Both_mbias3(nullptr),
	sameMIDcase8_mothedPDG_InM_two_Both_mbias3(nullptr),
	PdgCase8NonEComeFromTarget_mbias3_InM_two_Both_mbias3(nullptr),
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_two_Both_mbias3(nullptr),
	PdgCase8motherNonE_InM_two_Both_mbias3(nullptr),
	Case8ElFromDalitz_InM_two_Both_mbias3(nullptr),
	Case8NonElFrom_pn_InM_two_Both_mbias3(nullptr),
	Case8NonElFrom_eta_InM_two_Both_mbias3(nullptr),
	Case8NonElFrom_kaon_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEPdg_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherPdg_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherIM_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_two_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_two_Both_mbias3(nullptr),
	fHistoList_bg_InM_onetwo_Both_mbias3(),
	BG1_InM_onetwo_Both_mbias3(nullptr),
	BG2_InM_onetwo_Both_mbias3(nullptr),
	BG3_InM_onetwo_Both_mbias3(nullptr),
	BG4_InM_onetwo_Both_mbias3(nullptr),
	BG5_InM_onetwo_Both_mbias3(nullptr),
	BG6_InM_onetwo_Both_mbias3(nullptr),
	BG7_InM_onetwo_Both_mbias3(nullptr),
	BG8_InM_onetwo_Both_mbias3(nullptr),
	BG9_InM_onetwo_Both_mbias3(nullptr),
	BG10_InM_onetwo_Both_mbias3(nullptr),
	PdgCase8_InM_onetwo_Both_mbias3(nullptr),
	PdgCase8mothers_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8_InM_onetwo_Both_mbias3(nullptr),
	sameGRIDcase8_InM_onetwo_Both_mbias3(nullptr),
	Case1ZYPos_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias3(nullptr),
	PdgCase8NonEComeFromTarget_mbias3_InM_onetwo_Both_mbias3(nullptr),
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_onetwo_Both_mbias3(nullptr),
	PdgCase8motherNonE_InM_onetwo_Both_mbias3(nullptr),
	Case8ElFromDalitz_InM_onetwo_Both_mbias3(nullptr),
	Case8NonElFrom_pn_InM_onetwo_Both_mbias3(nullptr),
	Case8NonElFrom_eta_InM_onetwo_Both_mbias3(nullptr),
	Case8NonElFrom_kaon_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEPdg_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_onetwo_Both_mbias3(nullptr),
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_onetwo_Both_mbias3(nullptr)
{

}

CbmKresConversionManualmbias3::~CbmKresConversionManualmbias3()
{

}

void CbmKresConversionManualmbias3::Init()
{
	FairRootManager* ioman = FairRootManager::Instance();
	if (nullptr == ioman) { Fatal("CbmKresConversionManualmbias3::Init", "RootManager not instantised!"); }

	fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
	if ( nullptr == fMcTracks) { Fatal("CbmKresConversionManualmbias3::Init", "No MCTrack array!"); }

	fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
	if ( nullptr == fPrimVertex ) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
	if ( nullptr == fPrimVertex ) { LOG(fatal) << "CbmKresConversionManualmbias3::Init  No PrimaryVertex array!"; }

	fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
	if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionManualmbias3::Init", "No GlobalTrack array!"); }

	fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
	if ( nullptr == fStsTracks) { Fatal("CbmKresConversionManualmbias3::Init", "No StsTrack array!"); }

	fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
	if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionManualmbias3::Init", "No StsTrackMatch array!"); }

	fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
	if (nullptr == fRichProjections) {Fatal("CbmKresConversionManualmbias3::Init", "No RichProjection array!");}

	fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
	if (nullptr == fRichRings) { Fatal("CbmKresConversionManualmbias3::Init","No RichRing array!"); }

	fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
	if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionManualmbias3::Init","No RichRingMatch array!"); }

	fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
	if ( nullptr == fRichHits) { Fatal("CbmKresConversionManualmbias3::Init","No RichHit array!"); }

	fArrayMvdHit = (TClonesArray*) ioman->GetObject("MvdHit");
	if(nullptr == fArrayMvdHit) { Fatal("CbmKresConversionManualmbias3::Init","No MvdHit array!"); }

	fArrayStsHit = (TClonesArray*) ioman->GetObject("StsHit");
	if(nullptr == fArrayStsHit) { Fatal("CbmKresConversionManualmbias3::Init","No StsHit array!"); }

	fArrayCentrality = (FairMCEventHeader*) ioman->GetObject("MCEventHeader.");
	if(nullptr == fArrayCentrality) { Fatal("CbmAnaConversion2::Init","No fArrayCentrality array!"); }


	fTauFit = new CbmRichRingFitterEllipseTau();

	InitHistograms();

	fAnaBG = new CbmKresConversionBG();
	fAnaBG->Init();
}

void CbmKresConversionManualmbias3::Exec(int fEventNumMan, double OpeningAngleCut, double GammaInvMassCut, int RealPID)
{
	cout << "CbmKresConversionManualmbias3, event No. " <<  fEventNumMan << endl;

	double centrality_mbias3 = fArrayCentrality->GetB();

	if (fEventNumMan == 1000){
		EMT_man_Event_Both_mbias3.insert( EMT_man_Event_Both_mbias3.end(), EMT_man_Event_Outside_mbias3.begin(), EMT_man_Event_Outside_mbias3.end() );
		EMT_man_Event_Both_mbias3.insert( EMT_man_Event_Both_mbias3.end(), EMT_man_Event_Target_mbias3.begin(), EMT_man_Event_Target_mbias3.end() );
		EMT_man_pair_momenta_Both_mbias3.insert( EMT_man_pair_momenta_Both_mbias3.end(), EMT_man_pair_momenta_Outside_mbias3.begin(), EMT_man_pair_momenta_Outside_mbias3.end() );
		EMT_man_pair_momenta_Both_mbias3.insert( EMT_man_pair_momenta_Both_mbias3.end(), EMT_man_pair_momenta_Target_mbias3.begin(), EMT_man_pair_momenta_Target_mbias3.end() );
		EMT_man_NofRings_Both_mbias3.insert( EMT_man_NofRings_Both_mbias3.end(), EMT_man_NofRings_Outside_mbias3.begin(), EMT_man_NofRings_Outside_mbias3.end() );
		EMT_man_NofRings_Both_mbias3.insert( EMT_man_NofRings_Both_mbias3.end(), EMT_man_NofRings_Target_mbias3.begin(), EMT_man_NofRings_Target_mbias3.end() );
		Mixing_Both();
		EMT_man_Event_Both_mbias3.clear();
		EMT_man_pair_momenta_Both_mbias3.clear();
		EMT_man_NofRings_Both_mbias3.clear();  

		Mixing_Target();
		EMT_man_Event_Target_mbias3.clear();
		EMT_man_pair_momenta_Target_mbias3.clear();
		EMT_man_NofRings_Target_mbias3.clear();  

		Mixing_Outside();
		EMT_man_Event_Outside_mbias3.clear();
		EMT_man_pair_momenta_Outside_mbias3.clear();
		EMT_man_NofRings_Outside_mbias3.clear();  

	}
	if (centrality_mbias3 <= 6 || centrality_mbias3 > 10) return;


	if (fPrimVertex != nullptr){
		fKFVertex = CbmKFVertex(*fPrimVertex);
	} else {
		return;
	}



	Gammas_all_Target_mbias3.clear();
	Gammas_one_Target_mbias3.clear();
	Gammas_two_Target_mbias3.clear();
	Gammas_zero_Target_mbias3.clear();
	Gammas_onetwo_Target_mbias3.clear();
	Gammas_stsIndex_all_Target_mbias3.clear();
	Gammas_stsIndex_one_Target_mbias3.clear();
	Gammas_stsIndex_two_Target_mbias3.clear();
	Gammas_stsIndex_zero_Target_mbias3.clear();
	Gammas_stsIndex_onetwo_Target_mbias3.clear();
	Gammas_MC_all_Target_mbias3.clear();
	Gammas_MC_one_Target_mbias3.clear();
	Gammas_MC_two_Target_mbias3.clear();
	Gammas_MC_zero_Target_mbias3.clear();
	Gammas_MC_onetwo_Target_mbias3.clear();
	Gammas_all_Outside_mbias3.clear();
	Gammas_one_Outside_mbias3.clear();
	Gammas_two_Outside_mbias3.clear();
	Gammas_zero_Outside_mbias3.clear();
	Gammas_onetwo_Outside_mbias3.clear();
	Gammas_stsIndex_all_Outside_mbias3.clear();
	Gammas_stsIndex_one_Outside_mbias3.clear();
	Gammas_stsIndex_two_Outside_mbias3.clear();
	Gammas_stsIndex_zero_Outside_mbias3.clear();
	Gammas_stsIndex_onetwo_Outside_mbias3.clear();
	Gammas_MC_all_Outside_mbias3.clear();
	Gammas_MC_one_Outside_mbias3.clear();
	Gammas_MC_two_Outside_mbias3.clear();
	Gammas_MC_zero_Outside_mbias3.clear();
	Gammas_MC_onetwo_Outside_mbias3.clear();
	Gammas_all_Both_mbias3.clear();
	Gammas_one_Both_mbias3.clear();
	Gammas_two_Both_mbias3.clear();
	Gammas_zero_Both_mbias3.clear();
	Gammas_onetwo_Both_mbias3.clear();
	Gammas_stsIndex_all_Both_mbias3.clear();
	Gammas_stsIndex_one_Both_mbias3.clear();
	Gammas_stsIndex_two_Both_mbias3.clear();
	Gammas_stsIndex_zero_Both_mbias3.clear();
	Gammas_stsIndex_onetwo_Both_mbias3.clear();
	Gammas_MC_all_Both_mbias3.clear();
	Gammas_MC_one_Both_mbias3.clear();
	Gammas_MC_two_Both_mbias3.clear();
	Gammas_MC_zero_Both_mbias3.clear();
	Gammas_MC_onetwo_Both_mbias3.clear();




	VMCtracks_minus_Outside_mbias3.clear();
	VStsTrack_minus_Outside_mbias3.clear();
	VRings_minus_Outside_mbias3.clear();
	VStsIndex_minus_Outside_mbias3.clear();
	VRichRing_minus_Outside_mbias3.clear();
	VMCtracks_plus_Outside_mbias3.clear();
	VStsTrack_plus_Outside_mbias3.clear();
	VRings_plus_Outside_mbias3.clear();
	VStsIndex_plus_Outside_mbias3.clear();
	VRichRing_plus_Outside_mbias3.clear();

	VMCtracks_minus_Target_mbias3.clear();
	VMomenta_minus_Target_mbias3.clear();
	VStsTrack_minus_Target_mbias3.clear();
	VRings_minus_Target_mbias3.clear();
	VStsIndex_minus_Target_mbias3.clear();
	VRichRing_minus_Target_mbias3.clear();  
	VMCtracks_plus_Target_mbias3.clear();
	VStsTrack_plus_Target_mbias3.clear();
	VMomenta_plus_Target_mbias3.clear();
	VRings_plus_Target_mbias3.clear();
	VStsIndex_plus_Target_mbias3.clear();
	VRichRing_plus_Target_mbias3.clear();


	Int_t ngTracks = fGlobalTracks->GetEntriesFast();
	for (Int_t i = 0; i < ngTracks; i++) {
			CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
			if(nullptr == gTrack) continue;
			Int_t stsInd = gTrack->GetStsTrackIndex();
			Int_t richInd = gTrack->GetRichRingIndex();
	 
			if (stsInd < 0) continue;
			CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
			if (stsTrack == nullptr) continue;
			CbmTrackMatchNew* stsMatch  = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
			if (stsMatch == nullptr) continue;
			if(stsMatch->GetNofLinks() <= 0) continue;
			Int_t stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
			if (stsMcTrackId < 0) continue;
			CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
			if (mcTrack == nullptr) continue;


			FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(i);
			if (richInd < 0 && proj->GetX() > -115 && proj->GetX() < 115 && ((proj->GetY() < -120 && proj->GetY() > -200) || (proj->GetY() > 120 && proj->GetY() < 200)) ) continue;
			CbmRichRing* Ring = nullptr; 
			if (richInd > -1){
				Ring = static_cast<CbmRichRing*>(fRichRings->At(richInd));
				//Pdg_vs_Distance_mbias3->Fill(TMath::Abs(mcTrack->GetPdgCode()), Ring->GetDistance());
				//if (TMath::Abs(mcTrack->GetPdgCode()) == 11) P_vs_Distance_mbias3->Fill(mcTrack->GetP(), Ring->GetDistance());
			}


			// Doing fit with Fit To primary Vertex and calculate chi2 to primary vertex
			double chi2 = 0;
			TVector3 Momentum = CbmKresFunctions::FitToVertexAndGetChi(stsTrack, fKFVertex.GetRefX(), fKFVertex.GetRefY(), fKFVertex.GetRefZ(), chi2);
			const FairTrackParam* track_par = stsTrack->GetParamFirst();
			double charge = track_par->GetQp();
			

			if (chi2 != chi2 ) continue;
			if (chi2 == 0 ) continue;

			if (chi2 > 3){ 
				SaveOutsideTracks(mcTrack, stsTrack, charge, stsInd, richInd, stsMcTrackId, Ring);
			}
			if (chi2 > 3) continue;

			SaveTargetTracks(mcTrack, stsTrack, Momentum, charge, stsInd, richInd, stsMcTrackId, Ring);
			
	}


	FindGammasTarget(fEventNumMan, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Target_mbias3, VMCtracks_plus_Target_mbias3, VStsTrack_minus_Target_mbias3, VStsTrack_plus_Target_mbias3, VMomenta_minus_Target_mbias3, VMomenta_plus_Target_mbias3, VRings_minus_Target_mbias3, VRings_plus_Target_mbias3, VStsIndex_minus_Target_mbias3, VStsIndex_plus_Target_mbias3, VRichRing_minus_Target_mbias3, VRichRing_plus_Target_mbias3);

	FindGammasOutside(fEventNumMan, OpeningAngleCut, GammaInvMassCut, RealPID, VMCtracks_minus_Outside_mbias3, VMCtracks_plus_Outside_mbias3, VStsTrack_minus_Outside_mbias3, VStsTrack_plus_Outside_mbias3, VRings_minus_Outside_mbias3, VRings_plus_Outside_mbias3, VStsIndex_minus_Outside_mbias3, VStsIndex_plus_Outside_mbias3, VRichRing_minus_Outside_mbias3, VRichRing_plus_Outside_mbias3);

	FindGammasBoth();



	FindPi0("All",   "Target", Gammas_all_Target_mbias3, Gammas_stsIndex_all_Target_mbias3, Gammas_MC_all_Target_mbias3, Pi0InvMassReco_all_Target_mbias3, Pi0_pt_vs_rap_all_Target_mbias3, Pi0_pt_vs_rap_est_all_Target_mbias3, MultiplicityGamma_all_Target_mbias3, MultiplicityChargedParticles_all_Target_mbias3, fHistoList_bg_InM_all_Target_mbias3);
	FindPi0("Zero",  "Target", Gammas_zero_Target_mbias3, Gammas_stsIndex_zero_Target_mbias3, Gammas_MC_zero_Target_mbias3, Pi0InvMassReco_zero_Target_mbias3, Pi0_pt_vs_rap_zero_Target_mbias3, Pi0_pt_vs_rap_est_zero_Target_mbias3, MultiplicityGamma_zero_Target_mbias3, MultiplicityChargedParticles_zero_Target_mbias3, fHistoList_bg_InM_zero_Target_mbias3);
	FindPi0("One",   "Target", Gammas_one_Target_mbias3, Gammas_stsIndex_one_Target_mbias3, Gammas_MC_one_Target_mbias3, Pi0InvMassReco_one_Target_mbias3, Pi0_pt_vs_rap_one_Target_mbias3, Pi0_pt_vs_rap_est_one_Target_mbias3, MultiplicityGamma_one_Target_mbias3, MultiplicityChargedParticles_one_Target_mbias3, fHistoList_bg_InM_one_Target_mbias3);
	FindPi0("Two",   "Target", Gammas_two_Target_mbias3, Gammas_stsIndex_two_Target_mbias3, Gammas_MC_two_Target_mbias3, Pi0InvMassReco_two_Target_mbias3, Pi0_pt_vs_rap_two_Target_mbias3, Pi0_pt_vs_rap_est_two_Target_mbias3, MultiplicityGamma_two_Target_mbias3, MultiplicityChargedParticles_two_Target_mbias3, fHistoList_bg_InM_two_Target_mbias3);
	FindPi0("OneTwo","Target", Gammas_onetwo_Target_mbias3, Gammas_stsIndex_onetwo_Target_mbias3, Gammas_MC_onetwo_Target_mbias3, Pi0InvMassReco_onetwo_Target_mbias3, Pi0_pt_vs_rap_onetwo_Target_mbias3, Pi0_pt_vs_rap_est_onetwo_Target_mbias3, MultiplicityGamma_onetwo_Target_mbias3, MultiplicityChargedParticles_onetwo_Target_mbias3, fHistoList_bg_InM_onetwo_Target_mbias3);

	FindPi0("All",   "Outside", Gammas_all_Outside_mbias3, Gammas_stsIndex_all_Outside_mbias3, Gammas_MC_all_Outside_mbias3, Pi0InvMassReco_all_Outside_mbias3, Pi0_pt_vs_rap_all_Outside_mbias3, Pi0_pt_vs_rap_est_all_Outside_mbias3, MultiplicityGamma_all_Outside_mbias3, MultiplicityChargedParticles_all_Outside_mbias3, fHistoList_bg_InM_all_Outside_mbias3);
	FindPi0("Zero",  "Outside", Gammas_zero_Outside_mbias3, Gammas_stsIndex_zero_Outside_mbias3, Gammas_MC_zero_Outside_mbias3, Pi0InvMassReco_zero_Outside_mbias3, Pi0_pt_vs_rap_zero_Outside_mbias3, Pi0_pt_vs_rap_est_zero_Outside_mbias3, MultiplicityGamma_zero_Outside_mbias3, MultiplicityChargedParticles_zero_Outside_mbias3, fHistoList_bg_InM_zero_Outside_mbias3);
	FindPi0("One",   "Outside", Gammas_one_Outside_mbias3, Gammas_stsIndex_one_Outside_mbias3, Gammas_MC_one_Outside_mbias3, Pi0InvMassReco_one_Outside_mbias3, Pi0_pt_vs_rap_one_Outside_mbias3, Pi0_pt_vs_rap_est_one_Outside_mbias3, MultiplicityGamma_one_Outside_mbias3, MultiplicityChargedParticles_one_Outside_mbias3, fHistoList_bg_InM_one_Outside_mbias3);
	FindPi0("Two",   "Outside", Gammas_two_Outside_mbias3, Gammas_stsIndex_two_Outside_mbias3, Gammas_MC_two_Outside_mbias3, Pi0InvMassReco_two_Outside_mbias3, Pi0_pt_vs_rap_two_Outside_mbias3, Pi0_pt_vs_rap_est_two_Outside_mbias3, MultiplicityGamma_two_Outside_mbias3, MultiplicityChargedParticles_two_Outside_mbias3, fHistoList_bg_InM_two_Outside_mbias3);
	FindPi0("OneTwo","Outside", Gammas_onetwo_Outside_mbias3, Gammas_stsIndex_onetwo_Outside_mbias3, Gammas_MC_onetwo_Outside_mbias3, Pi0InvMassReco_onetwo_Outside_mbias3, Pi0_pt_vs_rap_onetwo_Outside_mbias3, Pi0_pt_vs_rap_est_onetwo_Outside_mbias3, MultiplicityGamma_onetwo_Outside_mbias3, MultiplicityChargedParticles_onetwo_Outside_mbias3, fHistoList_bg_InM_onetwo_Outside_mbias3);

	FindPi0("All",   "Both", Gammas_all_Both_mbias3, Gammas_stsIndex_all_Both_mbias3, Gammas_MC_all_Both_mbias3, Pi0InvMassReco_all_Both_mbias3, Pi0_pt_vs_rap_all_Both_mbias3, Pi0_pt_vs_rap_est_all_Both_mbias3, MultiplicityGamma_all_Both_mbias3, MultiplicityChargedParticles_all_Both_mbias3, fHistoList_bg_InM_all_Both_mbias3);
	FindPi0("Zero",  "Both", Gammas_zero_Both_mbias3, Gammas_stsIndex_zero_Both_mbias3, Gammas_MC_zero_Both_mbias3, Pi0InvMassReco_zero_Both_mbias3, Pi0_pt_vs_rap_zero_Both_mbias3, Pi0_pt_vs_rap_est_zero_Both_mbias3, MultiplicityGamma_zero_Both_mbias3, MultiplicityChargedParticles_zero_Both_mbias3, fHistoList_bg_InM_zero_Both_mbias3);
	FindPi0("One",   "Both", Gammas_one_Both_mbias3, Gammas_stsIndex_one_Both_mbias3, Gammas_MC_one_Both_mbias3, Pi0InvMassReco_one_Both_mbias3, Pi0_pt_vs_rap_one_Both_mbias3, Pi0_pt_vs_rap_est_one_Both_mbias3, MultiplicityGamma_one_Both_mbias3, MultiplicityChargedParticles_one_Both_mbias3, fHistoList_bg_InM_one_Both_mbias3);
	FindPi0("Two",   "Both", Gammas_two_Both_mbias3, Gammas_stsIndex_two_Both_mbias3, Gammas_MC_two_Both_mbias3, Pi0InvMassReco_two_Both_mbias3, Pi0_pt_vs_rap_two_Both_mbias3, Pi0_pt_vs_rap_est_two_Both_mbias3, MultiplicityGamma_two_Both_mbias3, MultiplicityChargedParticles_two_Both_mbias3, fHistoList_bg_InM_two_Both_mbias3);
	FindPi0("OneTwo","Both", Gammas_onetwo_Both_mbias3, Gammas_stsIndex_onetwo_Both_mbias3, Gammas_MC_onetwo_Both_mbias3, Pi0InvMassReco_onetwo_Both_mbias3, Pi0_pt_vs_rap_onetwo_Both_mbias3, Pi0_pt_vs_rap_est_onetwo_Both_mbias3, MultiplicityGamma_onetwo_Both_mbias3, MultiplicityChargedParticles_onetwo_Both_mbias3, fHistoList_bg_InM_onetwo_Both_mbias3);


}


void CbmKresConversionManualmbias3::SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING)
{
	int InRich = FindInRich(richInd, stsMcTrackId);
	if (charge < 0){
		VMCtracks_minus_Outside_mbias3.push_back(mcTrack1);
		VStsTrack_minus_Outside_mbias3.push_back(stsTrack);
		VRings_minus_Outside_mbias3.push_back(InRich);
		VStsIndex_minus_Outside_mbias3.push_back(stsInd);
		VRichRing_minus_Outside_mbias3.push_back(RING);
	}
	if (charge > 0){
		VMCtracks_plus_Outside_mbias3.push_back(mcTrack1);
		VStsTrack_plus_Outside_mbias3.push_back(stsTrack);
		VRings_plus_Outside_mbias3.push_back(InRich);
		VStsIndex_plus_Outside_mbias3.push_back(stsInd);
		VRichRing_plus_Outside_mbias3.push_back(RING);
	}
}

void CbmKresConversionManualmbias3::SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING)
{
	int InRich = FindInRich(richInd, stsMcTrackId);
	if (charge < 0){
		VMCtracks_minus_Target_mbias3.push_back(mcTrack1);
		VStsTrack_minus_Target_mbias3.push_back(stsTrack);
		VMomenta_minus_Target_mbias3.push_back(refmom);
		VRings_minus_Target_mbias3.push_back(InRich);
		VStsIndex_minus_Target_mbias3.push_back(stsInd);
		VRichRing_minus_Target_mbias3.push_back(RING);
	}
	if (charge > 0){
		VMCtracks_plus_Target_mbias3.push_back(mcTrack1);
		VStsTrack_plus_Target_mbias3.push_back(stsTrack);
		VMomenta_plus_Target_mbias3.push_back(refmom);
		VRings_plus_Target_mbias3.push_back(InRich);
		VStsIndex_plus_Target_mbias3.push_back(stsInd);
		VRichRing_plus_Target_mbias3.push_back(RING);
	}
}



void CbmKresConversionManualmbias3::FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
	vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus, 
	vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
	vector<TVector3> Momenta_minus,  vector<TVector3> Momenta_plus, 
	std::vector<int> Rings_minus, std::vector<int> Rings_plus, 
	std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus, 
	vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus)
{
	for (size_t i=0; i<Momenta_minus.size(); i++){
		for (size_t j=0; j<Momenta_plus.size(); j++){

			CbmStsTrack* sts1 = StsTrack_minus.at(i);
			CbmStsTrack* sts2 = StsTrack_plus.at(j);

			TVector3 part1 = Momenta_minus[i];
			TVector3 part2 = Momenta_plus[j];
			CbmMCTrack* part1MC = MCtracks_minus[i];
			CbmMCTrack* part2MC = MCtracks_plus[j];

			int richcheck_0 = 0;
			int richcheck_1 = 0;
			if (RealPID == 1){
				// Real PID
				richcheck_0 = CheckIfElectron(richRing_minus[i], Momenta_minus[i].Mag());
				richcheck_1 = CheckIfElectron(richRing_plus[j], Momenta_plus[j].Mag());
			} else {  
				// MC   PID
				richcheck_0 = Rings_minus[i];
				richcheck_1 = Rings_plus[j];
			}
			int richcheck = richcheck_0 + richcheck_1;

			Double_t InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
			Double_t OpeningAngle = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);

			InvMass_vs_OA_candidates_Target_mbias3->Fill(InvmassReco, OpeningAngle);
			InvMass_vs_OA_candidates_Both_mbias3->Fill(InvmassReco, OpeningAngle);
			GammasInvMass_candidates_Target_mbias3->Fill(InvmassReco);
			GammasInvMass_candidates_Both_mbias3->Fill(InvmassReco);
			GammasOA_candidates_Target_mbias3->Fill(OpeningAngle);
			GammasOA_candidates_Both_mbias3->Fill(OpeningAngle);			
			
			double PlaneAngle_last = CalculatePlaneAngle_last(sts1, sts2);
			double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);

			//if (PlaneAngle_last > 20) continue;

			if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1){
				CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
				if (mcTrackmama->GetMotherId() != -1 && mcTrackmama->GetPdgCode() == 22){
					CbmMCTrack* mcTrackgrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
					if(mcTrackgrmama->GetPdgCode() == 111){
						GammasInvMass_fromPi0_Target_mbias3->Fill(InvmassReco);
						GammasInvMass_fromPi0_Both_mbias3->Fill(InvmassReco);
						GammasOA_fromPi0_Target_mbias3->Fill(OpeningAngle);
						GammasOA_fromPi0_Both_mbias3->Fill(OpeningAngle);
						InvMass_vs_OA_fromPi0_Target_mbias3->Fill(InvmassReco, OpeningAngle);
						InvMass_vs_OA_fromPi0_Both_mbias3->Fill(InvmassReco, OpeningAngle);				    	
						PlaneAngles_last_fromPi0_Target_mbias3->Fill(PlaneAngle_last);
						PlaneAngles_last_fromPi0_Both_mbias3->Fill(PlaneAngle_last);
						PlaneAngles_first_fromPi0_Target_mbias3->Fill(PlaneAngle_first);
						PlaneAngles_first_fromPi0_Both_mbias3->Fill(PlaneAngle_first);
					}
				}
			}

			PlaneAngles_last_candidates_Target_mbias3->Fill(PlaneAngle_last);
			PlaneAngles_last_candidates_Both_mbias3->Fill(PlaneAngle_last);
			PlaneAngles_first_candidates_Target_mbias3->Fill(PlaneAngle_first);
			PlaneAngles_first_candidates_Both_mbias3->Fill(PlaneAngle_first);

			// cuts
			if (TMath::Abs(OpeningAngle) > AngleCut) continue;
			if (TMath::Abs(InvmassReco) > InvMassCut) continue;

			frefmomenta.clear();
			frefmomenta.push_back(part1);
			frefmomenta.push_back(part2);
			frefId.clear();
			frefId.push_back(stsIndex_minus[i]);
			frefId.push_back(stsIndex_plus[j]);
			fMCtracks.clear();
			fMCtracks.push_back(part1MC);
			fMCtracks.push_back(part2MC);

			// for event mixing
			EMT_man_Event_Target_mbias3.push_back(EventNumMan);
			EMT_man_pair_momenta_Target_mbias3.push_back(frefmomenta);
			EMT_man_NofRings_Target_mbias3.push_back(richcheck);

			
			// everything (RICH == 0, RICH == 1, RICH == 2) together
			if(richcheck == 0 || richcheck == 1 || richcheck == 2 ){
				Gammas_all_Target_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_all_Target_mbias3.push_back(frefId);
				Gammas_MC_all_Target_mbias3.push_back(fMCtracks);
				GammaInvMassReco_all_Target_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_all_Target_mbias3->Fill(OpeningAngle);
				Pdg_all_Target_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_all_Target_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_all_Target_mbias3->Fill(part1.Mag());
				P_reco_all_Target_mbias3->Fill(part2.Mag());
				Pt_reco_all_Target_mbias3->Fill(part1.Perp());
				Pt_reco_all_Target_mbias3->Fill(part2.Perp());
				GammaInvMassReco_all_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_all_Both_mbias3->Fill(OpeningAngle);
				Pdg_all_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_all_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_all_Both_mbias3->Fill(part1.Mag());
				P_reco_all_Both_mbias3->Fill(part2.Mag());
				Pt_reco_all_Both_mbias3->Fill(part1.Perp());
				Pt_reco_all_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 0 
			if(richcheck == 0 ){
				Gammas_zero_Target_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_zero_Target_mbias3.push_back(frefId);
				Gammas_MC_zero_Target_mbias3.push_back(fMCtracks);
				GammaInvMassReco_zero_Target_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_zero_Target_mbias3->Fill(OpeningAngle);
				Pdg_zero_Target_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_zero_Target_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_zero_Target_mbias3->Fill(part1.Mag());
				P_reco_zero_Target_mbias3->Fill(part2.Mag());
				Pt_reco_zero_Target_mbias3->Fill(part1.Perp());
				Pt_reco_zero_Target_mbias3->Fill(part2.Perp());
				GammaInvMassReco_zero_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_zero_Both_mbias3->Fill(OpeningAngle);
				Pdg_zero_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_zero_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_zero_Both_mbias3->Fill(part1.Mag());
				P_reco_zero_Both_mbias3->Fill(part2.Mag());
				Pt_reco_zero_Both_mbias3->Fill(part1.Perp());
				Pt_reco_zero_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 1 
			if(richcheck == 1 ){
				Gammas_one_Target_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_one_Target_mbias3.push_back(frefId);
				Gammas_MC_one_Target_mbias3.push_back(fMCtracks);
				GammaInvMassReco_one_Target_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_one_Target_mbias3->Fill(OpeningAngle);
				Pdg_one_Target_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_one_Target_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_one_Target_mbias3->Fill(part1.Mag());
				P_reco_one_Target_mbias3->Fill(part2.Mag());
				Pt_reco_one_Target_mbias3->Fill(part1.Perp());
				Pt_reco_one_Target_mbias3->Fill(part2.Perp());
				GammaInvMassReco_one_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_one_Both_mbias3->Fill(OpeningAngle);
				Pdg_one_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_one_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_one_Both_mbias3->Fill(part1.Mag());
				P_reco_one_Both_mbias3->Fill(part2.Mag());
				Pt_reco_one_Both_mbias3->Fill(part1.Perp());
				Pt_reco_one_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 2 
			if(richcheck == 2 ){
				Gammas_two_Target_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_two_Target_mbias3.push_back(frefId);
				Gammas_MC_two_Target_mbias3.push_back(fMCtracks);
				GammaInvMassReco_two_Target_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_two_Target_mbias3->Fill(OpeningAngle);
				Pdg_two_Target_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_two_Target_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_two_Target_mbias3->Fill(part1.Mag());
				P_reco_two_Target_mbias3->Fill(part2.Mag());
				Pt_reco_two_Target_mbias3->Fill(part1.Perp());
				Pt_reco_two_Target_mbias3->Fill(part2.Perp());
				GammaInvMassReco_two_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_two_Both_mbias3->Fill(OpeningAngle);
				Pdg_two_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_two_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_two_Both_mbias3->Fill(part1.Mag());
				P_reco_two_Both_mbias3->Fill(part2.Mag());
				Pt_reco_two_Both_mbias3->Fill(part1.Perp());
				Pt_reco_two_Both_mbias3->Fill(part2.Perp());
			}

			// cases, when RICH == 1 or RICH == 2 together
			if(richcheck == 1 || richcheck == 2 ){
				Gammas_onetwo_Target_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_onetwo_Target_mbias3.push_back(frefId);
				Gammas_MC_onetwo_Target_mbias3.push_back(fMCtracks);
				GammaInvMassReco_onetwo_Target_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_onetwo_Target_mbias3->Fill(OpeningAngle);
				Pdg_onetwo_Target_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_onetwo_Target_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_onetwo_Target_mbias3->Fill(part1.Mag());
				P_reco_onetwo_Target_mbias3->Fill(part2.Mag());
				Pt_reco_onetwo_Target_mbias3->Fill(part1.Perp());
				Pt_reco_onetwo_Target_mbias3->Fill(part2.Perp());
				GammaInvMassReco_onetwo_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_onetwo_Both_mbias3->Fill(OpeningAngle);
				Pdg_onetwo_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_onetwo_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_onetwo_Both_mbias3->Fill(part1.Mag());
				P_reco_onetwo_Both_mbias3->Fill(part2.Mag());
				Pt_reco_onetwo_Both_mbias3->Fill(part1.Perp());
				Pt_reco_onetwo_Both_mbias3->Fill(part2.Perp());
			} 
		}
	}
	
	//cout << "number of gammas Target with 0-2 electron identified in RICH = " << Gammas_all_Target_mbias3.size() << endl;	
	//cout << "number of gammas Target with  0  electron identified in RICH = " << Gammas_zero_Target_mbias3.size() << endl;
	//cout << "number of gammas Target with  1  electron identified in RICH = " << Gammas_one_Target_mbias3.size() << endl;
	//cout << "number of gammas Target with  2  electron identified in RICH = " << Gammas_two_Target_mbias3.size() << endl;
	//cout << "number of gammas Target with 1-2 electron identified in RICH = " << Gammas_onetwo_Target_mbias3.size() << endl;

}






void CbmKresConversionManualmbias3::FindGammasOutside(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
	vector<CbmMCTrack*> MCtracks_minus_Outside_mbias3, vector<CbmMCTrack*> MCtracks_plus_Outside_mbias3,
	vector<CbmStsTrack*> StsTrack_minus_Outside_mbias3, vector<CbmStsTrack*> StsTrack_plus_Outside_mbias3,
	std::vector<int> Rings_minus_Outside_mbias3, std::vector<int> Rings_plus_Outside_mbias3,
	std::vector<int> stsIndex_minus_Outside_mbias3, std::vector<int> stsIndex_plus_Outside_mbias3, 
	vector<CbmRichRing*> richRing_minus_Outside_mbias3, vector<CbmRichRing*> richRing_plus_Outside_mbias3)
{
	for (size_t i=0; i<StsTrack_minus_Outside_mbias3.size(); i++){
		for (size_t j=0; j<StsTrack_plus_Outside_mbias3.size(); j++){

			CbmStsTrack* part1STS = StsTrack_minus_Outside_mbias3[i];
			CbmStsTrack* part2STS = StsTrack_plus_Outside_mbias3[j];
			CbmMCTrack* part1MC = MCtracks_minus_Outside_mbias3[i];
			CbmMCTrack* part2MC = MCtracks_plus_Outside_mbias3[j];

			KFParticle electron;
			CbmKFParticleInterface::SetKFParticleFromStsTrack(part1STS, &electron, 11);
			KFParticle positron;
			CbmKFParticleInterface::SetKFParticleFromStsTrack(part2STS, &positron, -11);
			const KFParticle* daughters[2] = {&electron, &positron};
			KFParticle intersection;
			intersection.Construct(daughters, 2);

			if (intersection.GetZ() > 75 || intersection.GetZ() < -5) continue; // kick weird intersections

			
			TVector3 part1 = CbmKresFunctions::FitToVertex(part1STS, intersection.GetX(), intersection.GetY(), intersection.GetZ());
			TVector3 part2 = CbmKresFunctions::FitToVertex(part2STS, intersection.GetX(), intersection.GetY(), intersection.GetZ());


			//cout << "=================" << endl;
			//cout << "FitToVertex part1: px = " << part1.X() << "; \t py = " << part1.Y() << "; \t pz = " << part1.Z() << endl;
			//cout << "FitToVertex part2: px = " << part2.X() << "; \t py = " << part2.Y() << "; \t pz = " << part2.Z() << endl;
			//cout << "KFParticle  part1: px = " << electron.KFParticleBase::Px() << "; \t py = " << electron.KFParticleBase::Py() << "; \t pz = " << electron.KFParticleBase::Pz() << endl;
			//cout << "KFParticle  part2: px = " << positron.KFParticleBase::Px() << "; \t py = " << positron.KFParticleBase::Py() << "; \t pz = " << positron.KFParticleBase::Pz() << endl;
			//TVector3 part1(electron.KFParticleBase::Px(), electron.KFParticleBase::Py(), electron.KFParticleBase::Pz());
			//TVector3 part2(positron.KFParticleBase::Px(), positron.KFParticleBase::Py(), positron.KFParticleBase::Pz());
			

			int richcheck = 0;
			int richcheck_0 = 0;
			int richcheck_1 = 0;
			if (RealPID == 1){
				// Real PID
				richcheck_0 = CheckIfElectron(richRing_minus_Outside_mbias3[i], part1.Mag());
				richcheck_1 = CheckIfElectron(richRing_plus_Outside_mbias3[j], part2.Mag());
				richcheck = richcheck_0 + richcheck_1;
			} else {
				// MC   PID
				richcheck = Rings_minus_Outside_mbias3[i] + Rings_plus_Outside_mbias3[j];
			}


			Double_t InvmassReco = CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
			Double_t OpeningAngle = CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);

			InvMass_vs_OA_candidates_Outside_mbias3->Fill(InvmassReco, OpeningAngle);
			InvMass_vs_OA_candidates_Both_mbias3->Fill(InvmassReco, OpeningAngle);
			GammasInvMass_candidates_Outside_mbias3->Fill(InvmassReco);
			GammasInvMass_candidates_Both_mbias3->Fill(InvmassReco);
			GammasOA_candidates_Outside_mbias3->Fill(OpeningAngle);
			GammasOA_candidates_Both_mbias3->Fill(OpeningAngle);			
			
			double PlaneAngle_last = CalculatePlaneAngle_last(part1STS, part2STS);
			double PlaneAngle_first = CalculatePlaneAngle_first(part1STS, part2STS);

			// if (PlaneAngle_last > 20) continue;
			
			if (part1MC->GetMotherId() == part2MC->GetMotherId() && part1MC->GetMotherId() != -1){
				CbmMCTrack* mcTrackmama = (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
				if (mcTrackmama->GetMotherId() != -1 && mcTrackmama->GetPdgCode() == 22){
					CbmMCTrack* mcTrackgrmama = (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
					if(mcTrackgrmama->GetPdgCode() == 111){
						GammasInvMass_fromPi0_Outside_mbias3->Fill(InvmassReco);
						GammasInvMass_fromPi0_Both_mbias3->Fill(InvmassReco);
						GammasOA_fromPi0_Outside_mbias3->Fill(OpeningAngle);
						GammasOA_fromPi0_Both_mbias3->Fill(OpeningAngle);				    	
						InvMass_vs_OA_fromPi0_Outside_mbias3->Fill(InvmassReco, OpeningAngle);
						InvMass_vs_OA_fromPi0_Both_mbias3->Fill(InvmassReco, OpeningAngle);
						PlaneAngles_last_fromPi0_Outside_mbias3->Fill(PlaneAngle_last);
						PlaneAngles_last_fromPi0_Both_mbias3->Fill(PlaneAngle_last);
						PlaneAngles_first_fromPi0_Outside_mbias3->Fill(PlaneAngle_first);
						PlaneAngles_first_fromPi0_Both_mbias3->Fill(PlaneAngle_first);
					}
				}
			}
			
			PlaneAngles_last_candidates_Outside_mbias3->Fill(PlaneAngle_last);
			PlaneAngles_last_candidates_Both_mbias3->Fill(PlaneAngle_last);
			PlaneAngles_first_candidates_Outside_mbias3->Fill(PlaneAngle_first);
			PlaneAngles_first_candidates_Both_mbias3->Fill(PlaneAngle_first);

			// cuts
			if (TMath::Abs(OpeningAngle) > AngleCut) continue;
			if (TMath::Abs(InvmassReco) > InvMassCut) continue;
			
			frefmomenta.clear();
			frefmomenta.push_back(part1);
			frefmomenta.push_back(part2);

			frefId.clear();
			frefId.push_back(stsIndex_minus_Outside_mbias3[i]);
			frefId.push_back(stsIndex_plus_Outside_mbias3[j]);
			fMCtracks.clear();
			fMCtracks.push_back(part1MC);
			fMCtracks.push_back(part2MC);
			

			// for event mixing
			EMT_man_Event_Outside_mbias3.push_back(EventNumMan);  
			EMT_man_pair_momenta_Outside_mbias3.push_back(frefmomenta);  
			EMT_man_NofRings_Outside_mbias3.push_back(richcheck);			


			// everything (RICH == 0, RICH == 1, RICH == 2) together
			if(richcheck == 0 || richcheck == 1 || richcheck == 2 ){
				Gammas_all_Outside_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_all_Outside_mbias3.push_back(frefId);
				Gammas_MC_all_Outside_mbias3.push_back(fMCtracks);
				GammaInvMassReco_all_Outside_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_all_Outside_mbias3->Fill(OpeningAngle);
				Pdg_all_Outside_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_all_Outside_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_all_Outside_mbias3->Fill(part1.Mag());
				P_reco_all_Outside_mbias3->Fill(part2.Mag());
				Pt_reco_all_Outside_mbias3->Fill(part1.Perp());
				Pt_reco_all_Outside_mbias3->Fill(part2.Perp());
				GammaInvMassReco_all_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_all_Both_mbias3->Fill(OpeningAngle);
				Pdg_all_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_all_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_all_Both_mbias3->Fill(part1.Mag());
				P_reco_all_Both_mbias3->Fill(part2.Mag());
				Pt_reco_all_Both_mbias3->Fill(part1.Perp());
				Pt_reco_all_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 0 
			if(richcheck == 0 ){
				Gammas_zero_Outside_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_zero_Outside_mbias3.push_back(frefId);
				Gammas_MC_zero_Outside_mbias3.push_back(fMCtracks);
				GammaInvMassReco_zero_Outside_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_zero_Outside_mbias3->Fill(OpeningAngle);
				Pdg_zero_Outside_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_zero_Outside_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_zero_Outside_mbias3->Fill(part1.Mag());
				P_reco_zero_Outside_mbias3->Fill(part2.Mag());
				Pt_reco_zero_Outside_mbias3->Fill(part1.Perp());
				Pt_reco_zero_Outside_mbias3->Fill(part2.Perp());
				GammaInvMassReco_zero_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_zero_Both_mbias3->Fill(OpeningAngle);
				Pdg_zero_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_zero_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_zero_Both_mbias3->Fill(part1.Mag());
				P_reco_zero_Both_mbias3->Fill(part2.Mag());
				Pt_reco_zero_Both_mbias3->Fill(part1.Perp());
				Pt_reco_zero_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 1 
			if(richcheck == 1 ){
				Gammas_one_Outside_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_one_Outside_mbias3.push_back(frefId);
				Gammas_MC_one_Outside_mbias3.push_back(fMCtracks);
				GammaInvMassReco_one_Outside_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_one_Outside_mbias3->Fill(OpeningAngle);
				Pdg_one_Outside_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_one_Outside_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_one_Outside_mbias3->Fill(part1.Mag());
				P_reco_one_Outside_mbias3->Fill(part2.Mag());
				Pt_reco_one_Outside_mbias3->Fill(part1.Perp());
				Pt_reco_one_Outside_mbias3->Fill(part2.Perp());
				GammaInvMassReco_one_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_one_Both_mbias3->Fill(OpeningAngle);
				Pdg_one_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_one_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_one_Both_mbias3->Fill(part1.Mag());
				P_reco_one_Both_mbias3->Fill(part2.Mag());
				Pt_reco_one_Both_mbias3->Fill(part1.Perp());
				Pt_reco_one_Both_mbias3->Fill(part2.Perp());
			}

			// only cases, when RICH == 2 
			if(richcheck == 2 ){
				Gammas_two_Outside_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_two_Outside_mbias3.push_back(frefId);
				Gammas_MC_two_Outside_mbias3.push_back(fMCtracks);
				GammaInvMassReco_two_Outside_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_two_Outside_mbias3->Fill(OpeningAngle);
				Pdg_two_Outside_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_two_Outside_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_two_Outside_mbias3->Fill(part1.Mag());
				P_reco_two_Outside_mbias3->Fill(part2.Mag());
				Pt_reco_two_Outside_mbias3->Fill(part1.Perp());
				Pt_reco_two_Outside_mbias3->Fill(part2.Perp());
				GammaInvMassReco_two_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_two_Both_mbias3->Fill(OpeningAngle);
				Pdg_two_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_two_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_two_Both_mbias3->Fill(part1.Mag());
				P_reco_two_Both_mbias3->Fill(part2.Mag());
				Pt_reco_two_Both_mbias3->Fill(part1.Perp());
				Pt_reco_two_Both_mbias3->Fill(part2.Perp());
			}

			// cases, when RICH == 1 or RICH == 2 together
			if(richcheck == 1 || richcheck == 2 ){
				Gammas_onetwo_Outside_mbias3.push_back(frefmomenta);
				Gammas_stsIndex_onetwo_Outside_mbias3.push_back(frefId);
				Gammas_MC_onetwo_Outside_mbias3.push_back(fMCtracks);
				GammaInvMassReco_onetwo_Outside_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_onetwo_Outside_mbias3->Fill(OpeningAngle);
				Pdg_onetwo_Outside_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_onetwo_Outside_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_onetwo_Outside_mbias3->Fill(part1.Mag());
				P_reco_onetwo_Outside_mbias3->Fill(part2.Mag());
				Pt_reco_onetwo_Outside_mbias3->Fill(part1.Perp());
				Pt_reco_onetwo_Outside_mbias3->Fill(part2.Perp());
				GammaInvMassReco_onetwo_Both_mbias3->Fill(InvmassReco);
				GammaOpeningAngleReco_onetwo_Both_mbias3->Fill(OpeningAngle);
				Pdg_onetwo_Both_mbias3->Fill(TMath::Abs(part1MC->GetPdgCode()));
				Pdg_onetwo_Both_mbias3->Fill(TMath::Abs(part2MC->GetPdgCode()));
				P_reco_onetwo_Both_mbias3->Fill(part1.Mag());
				P_reco_onetwo_Both_mbias3->Fill(part2.Mag());
				Pt_reco_onetwo_Both_mbias3->Fill(part1.Perp());
				Pt_reco_onetwo_Both_mbias3->Fill(part2.Perp());
			}
		}
	}
	
	//cout << "number of gammas Outside with 0-2 electron identified in RICH = " << Gammas_all_Outside_mbias3.size() << endl;
	//cout << "number of gammas Outside with  0  electron identified in RICH = " << Gammas_zero_Outside_mbias3.size() << endl;
	//cout << "number of gammas Outside with  1  electron identified in RICH = " << Gammas_one_Outside_mbias3.size() << endl;
	//cout << "number of gammas Outside with  2  electron identified in RICH = " << Gammas_two_Outside_mbias3.size() << endl;
	//cout << "number of gammas Outside with 1-2 electron identified in RICH = " << Gammas_onetwo_Outside_mbias3.size() << endl;

}





void CbmKresConversionManualmbias3::FindGammasBoth()
{
	Gammas_all_Both_mbias3.insert( Gammas_all_Both_mbias3.end(), Gammas_all_Outside_mbias3.begin(), Gammas_all_Outside_mbias3.end() );
	Gammas_all_Both_mbias3.insert( Gammas_all_Both_mbias3.end(), Gammas_all_Target_mbias3.begin(), Gammas_all_Target_mbias3.end() );	
	Gammas_zero_Both_mbias3.insert( Gammas_zero_Both_mbias3.end(), Gammas_zero_Outside_mbias3.begin(), Gammas_zero_Outside_mbias3.end() );
	Gammas_zero_Both_mbias3.insert( Gammas_zero_Both_mbias3.end(), Gammas_zero_Target_mbias3.begin(), Gammas_zero_Target_mbias3.end() );
	Gammas_one_Both_mbias3.insert( Gammas_one_Both_mbias3.end(), Gammas_one_Outside_mbias3.begin(), Gammas_one_Outside_mbias3.end() );
	Gammas_one_Both_mbias3.insert( Gammas_one_Both_mbias3.end(), Gammas_one_Target_mbias3.begin(), Gammas_one_Target_mbias3.end() );
	Gammas_two_Both_mbias3.insert( Gammas_two_Both_mbias3.end(), Gammas_two_Outside_mbias3.begin(), Gammas_two_Outside_mbias3.end() );
	Gammas_two_Both_mbias3.insert( Gammas_two_Both_mbias3.end(), Gammas_two_Target_mbias3.begin(), Gammas_two_Target_mbias3.end() );
	Gammas_onetwo_Both_mbias3.insert( Gammas_onetwo_Both_mbias3.end(), Gammas_onetwo_Outside_mbias3.begin(), Gammas_onetwo_Outside_mbias3.end() );
	Gammas_onetwo_Both_mbias3.insert( Gammas_onetwo_Both_mbias3.end(), Gammas_onetwo_Target_mbias3.begin(), Gammas_onetwo_Target_mbias3.end() );

	Gammas_stsIndex_all_Both_mbias3.insert( Gammas_stsIndex_all_Both_mbias3.end(), Gammas_stsIndex_all_Outside_mbias3.begin(), Gammas_stsIndex_all_Outside_mbias3.end() );
	Gammas_stsIndex_all_Both_mbias3.insert( Gammas_stsIndex_all_Both_mbias3.end(), Gammas_stsIndex_all_Target_mbias3.begin(), Gammas_stsIndex_all_Target_mbias3.end() );
	Gammas_stsIndex_zero_Both_mbias3.insert( Gammas_stsIndex_zero_Both_mbias3.end(), Gammas_stsIndex_zero_Outside_mbias3.begin(), Gammas_stsIndex_zero_Outside_mbias3.end() );
	Gammas_stsIndex_zero_Both_mbias3.insert( Gammas_stsIndex_zero_Both_mbias3.end(), Gammas_stsIndex_zero_Target_mbias3.begin(), Gammas_stsIndex_zero_Target_mbias3.end() );
	Gammas_stsIndex_one_Both_mbias3.insert( Gammas_stsIndex_one_Both_mbias3.end(), Gammas_stsIndex_one_Outside_mbias3.begin(), Gammas_stsIndex_one_Outside_mbias3.end() );
	Gammas_stsIndex_one_Both_mbias3.insert( Gammas_stsIndex_one_Both_mbias3.end(), Gammas_stsIndex_one_Target_mbias3.begin(), Gammas_stsIndex_one_Target_mbias3.end() );
	Gammas_stsIndex_two_Both_mbias3.insert( Gammas_stsIndex_two_Both_mbias3.end(), Gammas_stsIndex_two_Outside_mbias3.begin(), Gammas_stsIndex_two_Outside_mbias3.end() );
	Gammas_stsIndex_two_Both_mbias3.insert( Gammas_stsIndex_two_Both_mbias3.end(), Gammas_stsIndex_two_Target_mbias3.begin(), Gammas_stsIndex_two_Target_mbias3.end() );
	Gammas_stsIndex_onetwo_Both_mbias3.insert( Gammas_stsIndex_onetwo_Both_mbias3.end(), Gammas_stsIndex_onetwo_Outside_mbias3.begin(), Gammas_stsIndex_onetwo_Outside_mbias3.end() );
	Gammas_stsIndex_onetwo_Both_mbias3.insert( Gammas_stsIndex_onetwo_Both_mbias3.end(), Gammas_stsIndex_onetwo_Target_mbias3.begin(), Gammas_stsIndex_onetwo_Target_mbias3.end() );

	Gammas_MC_all_Both_mbias3.insert( Gammas_MC_all_Both_mbias3.end(), Gammas_MC_all_Outside_mbias3.begin(), Gammas_MC_all_Outside_mbias3.end() );
	Gammas_MC_all_Both_mbias3.insert( Gammas_MC_all_Both_mbias3.end(), Gammas_MC_all_Target_mbias3.begin(), Gammas_MC_all_Target_mbias3.end() );
	Gammas_MC_zero_Both_mbias3.insert( Gammas_MC_zero_Both_mbias3.end(), Gammas_MC_zero_Outside_mbias3.begin(), Gammas_MC_zero_Outside_mbias3.end() );
	Gammas_MC_zero_Both_mbias3.insert( Gammas_MC_zero_Both_mbias3.end(), Gammas_MC_zero_Target_mbias3.begin(), Gammas_MC_zero_Target_mbias3.end() );
	Gammas_MC_one_Both_mbias3.insert( Gammas_MC_one_Both_mbias3.end(), Gammas_MC_one_Outside_mbias3.begin(), Gammas_MC_one_Outside_mbias3.end() );
	Gammas_MC_one_Both_mbias3.insert( Gammas_MC_one_Both_mbias3.end(), Gammas_MC_one_Target_mbias3.begin(), Gammas_MC_one_Target_mbias3.end() );
	Gammas_MC_two_Both_mbias3.insert( Gammas_MC_two_Both_mbias3.end(), Gammas_MC_two_Outside_mbias3.begin(), Gammas_MC_two_Outside_mbias3.end() );
	Gammas_MC_two_Both_mbias3.insert( Gammas_MC_two_Both_mbias3.end(), Gammas_MC_two_Target_mbias3.begin(), Gammas_MC_two_Target_mbias3.end() );
	Gammas_MC_onetwo_Both_mbias3.insert( Gammas_MC_onetwo_Both_mbias3.end(), Gammas_MC_onetwo_Outside_mbias3.begin(), Gammas_MC_onetwo_Outside_mbias3.end() );
	Gammas_MC_onetwo_Both_mbias3.insert( Gammas_MC_onetwo_Both_mbias3.end(), Gammas_MC_onetwo_Target_mbias3.begin(), Gammas_MC_onetwo_Target_mbias3.end() );

	//cout << "number of gammas Both with 0-2 electron identified in RICH = " << Gammas_all_Both_mbias3.size() << endl;
	//cout << "number of gammas Both with  0  electron identified in RICH = " << Gammas_zero_Both_mbias3.size() << endl;
	//cout << "number of gammas Both with  1  electron identified in RICH = " << Gammas_one_Both_mbias3.size() << endl;
	//cout << "number of gammas Both with  2  electron identified in RICH = " << Gammas_two_Both_mbias3.size() << endl;
	//cout << "number of gammas Both with 1-2 electron identified in RICH = " << Gammas_onetwo_Both_mbias3.size() << endl;
}



void CbmKresConversionManualmbias3::FindPi0(TString mod, TString position, vector< vector<TVector3> > Gammas, vector< vector<int> > StsIndex, vector< vector<CbmMCTrack*> > GammasMC, 
									TH1D* Pi0InvMassReco, TH2D* Pi0_pt_vs_rap, TH2D* Pi0_pt_vs_rap_est, TH2D* MultiplicityGamma, TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases)
{
	// combine all gamma in pi0 --> calculate inv mass for gammas and pi0          // not the case, when one particle used twice for different gammas
	if(Gammas.size() < 2) return; // min 2 gammas to form pi0 are required
	for (size_t gamma1=0; gamma1<Gammas.size()-1; gamma1++){
		for (size_t gamma2=gamma1+1; gamma2<Gammas.size(); gamma2++){
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

			if(StsIndex[gamma1][0] == StsIndex[gamma2][0] || StsIndex[gamma1][0] == StsIndex[gamma2][1] || 
			   StsIndex[gamma1][1] == StsIndex[gamma2][0] || StsIndex[gamma1][1] == StsIndex[gamma2][1]) continue; // particles not used twice --> different

			CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

			//Double_t OA1 = CbmKresFunctions::CalculateOpeningAngle_Reco(e11, e12);
			//Double_t OA2 = CbmKresFunctions::CalculateOpeningAngle_Reco(e21, e22);
			//if (params.fRapidity > 2.8 && params.fRapidity <= 3.6 && params.fPt > 0.0 && params.fPt <= 0.4 && OA1 > 1 && OA2 > 1) continue;

			Pi0InvMassReco->Fill(params.fMinv);
			MultiplicityGamma->Fill(Gammas.size(), params.fMinv);
			MultiplicityChargedParticles->Fill(fGlobalTracks->GetEntriesFast(), params.fMinv);

			// separate by rap and Pt only for onetwo
			if (mod == "OneTwo" && position == "Both"){ 
				if (params.fRapidity > 1.2 && params.fRapidity <= 1.6){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_1_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_2_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_3_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_4_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_5_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 1.6 && params.fRapidity <= 2.0){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_6_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_7_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_8_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_9_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_10_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.0 && params.fRapidity <= 2.4){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_11_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_12_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_13_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_14_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_15_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.4 && params.fRapidity <= 2.8){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_16_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_17_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_18_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_19_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_20_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.8 && params.fRapidity <= 3.2){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_21_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_22_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_23_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_24_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_25_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 3.2 && params.fRapidity <= 3.6){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_26_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_27_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_28_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_29_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_30_mbias3->Fill(params.fMinv);
				}
			}


			if(position == "Both") fAnaBG->Exec(mcTrack1, mcTrack2, mcTrack3, mcTrack4, params.fMinv, BGCases);


			// fill histos with rapidity and Pt for correctly reconstructed pi0(dalitz) and pi0(g+g)
			if (nullptr == mcTrack1 || nullptr == mcTrack2 || nullptr == mcTrack3 || nullptr == mcTrack4) continue;
			if( TMath::Abs(mcTrack1->GetPdgCode()) != 11 || TMath::Abs(mcTrack2->GetPdgCode()) != 11 || TMath::Abs(mcTrack3->GetPdgCode()) != 11 || TMath::Abs(mcTrack4->GetPdgCode()) != 11) continue;
			if(mcTrack1->GetPdgCode() + mcTrack2->GetPdgCode() != 0) continue;
			if(mcTrack3->GetPdgCode() + mcTrack4->GetPdgCode() != 0) continue;
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
			int mcMotherPdg1 = mother1->GetPdgCode();
			int mcMotherPdg2 = mother2->GetPdgCode();
			int mcMotherPdg3 = mother3->GetPdgCode();
			int mcMotherPdg4 = mother4->GetPdgCode();
			int grandmotherId1 = mother1->GetMotherId();
			int grandmotherId2 = mother2->GetMotherId();
			int grandmotherId3 = mother3->GetMotherId();
			int grandmotherId4 = mother4->GetMotherId();

			if( mcMotherPdg1 == 22 && mcMotherPdg2 == 22 && mcMotherPdg3 == 111 && mcMotherPdg4 == 111){
				if (grandmotherId1 != motherId3) continue;
				Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
				Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
			}

			if( mcMotherPdg1 == 111 && mcMotherPdg2 == 111 && mcMotherPdg3 == 22 && mcMotherPdg4 == 22){
				if (grandmotherId3 != motherId1) continue;
				Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
				Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
			}

			if( mcMotherPdg1 == 22 && mcMotherPdg2 == 22 && mcMotherPdg3 == 22 && mcMotherPdg4 == 22){
				if (grandmotherId1 != grandmotherId2 || grandmotherId3 != grandmotherId4 || grandmotherId1 != grandmotherId3) continue;
				if (grandmotherId1 == -1) continue;
				CbmMCTrack* grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
				if (nullptr == grandmother1) continue;
				int mcGrandMotherPdg1 = grandmother1->GetPdgCode();
				if (mcGrandMotherPdg1 != 111 ) continue;
				Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
				Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
			}
		}
	}
}


void CbmKresConversionManualmbias3::Mixing_Target()
// TARGET
{
	Int_t nof_Target = EMT_man_Event_Target_mbias3.size();
	cout << "Mixing in Manual(Target_mbias3) - nof entries " << nof_Target << endl;
	for(Int_t a = 0; a < nof_Target-1; a++) {
		for(Int_t b = a+1; b < nof_Target; b++) {
			if (EMT_man_Event_Target_mbias3[a] == EMT_man_Event_Target_mbias3[b]) continue;		// to make sure that the photons are from two different events
			TVector3 e11 = EMT_man_pair_momenta_Target_mbias3[a][0];
			TVector3 e12 = EMT_man_pair_momenta_Target_mbias3[a][1];
			TVector3 e21 = EMT_man_pair_momenta_Target_mbias3[b][0];
			TVector3 e22 = EMT_man_pair_momenta_Target_mbias3[b][1];
			CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

			EMT_InvMass_all_Target_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Target_mbias3[a] == 0 && EMT_man_NofRings_Target_mbias3[b] == 0) EMT_InvMass_zero_Target_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Target_mbias3[a] == 1 && EMT_man_NofRings_Target_mbias3[b] == 1) EMT_InvMass_one_Target_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Target_mbias3[a] == 2 && EMT_man_NofRings_Target_mbias3[b] == 2) EMT_InvMass_two_Target_mbias3->Fill(params.fMinv);
			if ( (EMT_man_NofRings_Target_mbias3[a] == 1 || EMT_man_NofRings_Target_mbias3[a] == 2) && (EMT_man_NofRings_Target_mbias3[b] == 1 || EMT_man_NofRings_Target_mbias3[b] == 2) ) EMT_InvMass_onetwo_Target_mbias3->Fill(params.fMinv);
		}
	}
}



void CbmKresConversionManualmbias3::Mixing_Outside()
// OUTSIDE
{
	Int_t nof_Outside = EMT_man_Event_Outside_mbias3.size();
	cout << "Mixing in Manual(Outside_mbias3) - nof entries " << nof_Outside << endl;
	for(Int_t a = 0; a < nof_Outside-1; a++) {
		for(Int_t b = a+1; b < nof_Outside; b++) {
			if(EMT_man_Event_Outside_mbias3[a] == EMT_man_Event_Outside_mbias3[b]) continue;		// to make sure that the photons are from two different events
			TVector3 e11 = EMT_man_pair_momenta_Outside_mbias3[a][0];
			TVector3 e12 = EMT_man_pair_momenta_Outside_mbias3[a][1];
			TVector3 e21 = EMT_man_pair_momenta_Outside_mbias3[b][0];
			TVector3 e22 = EMT_man_pair_momenta_Outside_mbias3[b][1];
			CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

			EMT_InvMass_all_Outside_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Outside_mbias3[a] == 0 && EMT_man_NofRings_Outside_mbias3[b] == 0) EMT_InvMass_zero_Outside_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Outside_mbias3[a] == 1 && EMT_man_NofRings_Outside_mbias3[b] == 1) EMT_InvMass_one_Outside_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Outside_mbias3[a] == 2 && EMT_man_NofRings_Outside_mbias3[b] == 2) EMT_InvMass_two_Outside_mbias3->Fill(params.fMinv);
			if ( (EMT_man_NofRings_Outside_mbias3[a] == 1 || EMT_man_NofRings_Outside_mbias3[a] == 2) && (EMT_man_NofRings_Outside_mbias3[b] == 1 || EMT_man_NofRings_Outside_mbias3[b] == 2) ) EMT_InvMass_onetwo_Outside_mbias3->Fill(params.fMinv);
		}
	}
}



void CbmKresConversionManualmbias3::Mixing_Both()
// BOTH
{
	Int_t nof_Both = EMT_man_Event_Both_mbias3.size();
	cout << "Mixing in Manual(Both_mbias3) - nof entries " << nof_Both << endl;
	for(Int_t a = 0; a < nof_Both-1; a++) {
		for(Int_t b = a+1; b < nof_Both; b++) {
			if(EMT_man_Event_Both_mbias3[a] == EMT_man_Event_Both_mbias3[b]) continue;		// to make sure that the photons are from two different events
			TVector3 e11 = EMT_man_pair_momenta_Both_mbias3[a][0];
			TVector3 e12 = EMT_man_pair_momenta_Both_mbias3[a][1];
			TVector3 e21 = EMT_man_pair_momenta_Both_mbias3[b][0];
			TVector3 e22 = EMT_man_pair_momenta_Both_mbias3[b][1];
			CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

			EMT_InvMass_all_Both_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Both_mbias3[a] == 0 && EMT_man_NofRings_Both_mbias3[b] == 0) EMT_InvMass_zero_Both_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Both_mbias3[a] == 1 && EMT_man_NofRings_Both_mbias3[b] == 1) EMT_InvMass_one_Both_mbias3->Fill(params.fMinv);
			if (EMT_man_NofRings_Both_mbias3[a] == 2 && EMT_man_NofRings_Both_mbias3[b] == 2) EMT_InvMass_two_Both_mbias3->Fill(params.fMinv);
			if ( (EMT_man_NofRings_Both_mbias3[a] == 1 || EMT_man_NofRings_Both_mbias3[a] == 2) && (EMT_man_NofRings_Both_mbias3[b] == 1 || EMT_man_NofRings_Both_mbias3[b] == 2) ) EMT_InvMass_onetwo_Both_mbias3->Fill(params.fMinv);
			
			// separate by rap and Pt only for onetwo
			if ( (EMT_man_NofRings_Both_mbias3[a] == 1 || EMT_man_NofRings_Both_mbias3[a] == 2) && (EMT_man_NofRings_Both_mbias3[b] == 1 || EMT_man_NofRings_Both_mbias3[b] == 2) ) {
				if (params.fRapidity > 1.2 && params.fRapidity <= 1.6){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_1_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_2_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_3_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_4_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_5_mbias3->Fill(params.fMinv);
				}				
				if (params.fRapidity > 1.6 && params.fRapidity <= 2.0){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_6_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_7_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_8_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_9_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_10_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.0 && params.fRapidity <= 2.4){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_11_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_12_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_13_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_14_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_15_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.4 && params.fRapidity <= 2.8){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_16_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_17_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_18_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_19_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_20_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 2.8 && params.fRapidity <= 3.2){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_21_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_22_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_23_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_24_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_25_mbias3->Fill(params.fMinv);
				}
				if (params.fRapidity > 3.2 && params.fRapidity <= 3.6){
					if(params.fPt > 0.0 && params.fPt <= 0.4) rap_vs_Pt_InM_mixing_26_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.4 && params.fPt <= 0.8) rap_vs_Pt_InM_mixing_27_mbias3->Fill(params.fMinv);
					if(params.fPt > 0.8 && params.fPt <= 1.2) rap_vs_Pt_InM_mixing_28_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.2 && params.fPt <= 1.6) rap_vs_Pt_InM_mixing_29_mbias3->Fill(params.fMinv);
					if(params.fPt > 1.6 && params.fPt <= 2.0) rap_vs_Pt_InM_mixing_30_mbias3->Fill(params.fMinv);
				}
			}

		}
	}
}



int CbmKresConversionManualmbias3::FindInRich(int richInd, int stsMcTrackId)
{
	int RingsInRich = 0;
	if (richInd > 0){
		CbmTrackMatchNew* richMatch  = (CbmTrackMatchNew*)fRichRingMatches->At(richInd);
		if (richMatch != nullptr && richMatch->GetNofLinks()>0){
			int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
			if (richMcTrackId > 0){
				if(stsMcTrackId == richMcTrackId){                                             // check that global track was matched correctly for STS and RICH together
					CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
					if (mcTrack2 != nullptr){
						int pdgRICH = mcTrack2->GetPdgCode();
						if (TMath::Abs(pdgRICH) == 11 ) RingsInRich++;
					}
				}
			}
		}
	}
	return RingsInRich;
}


int CbmKresConversionManualmbias3::CheckIfElectron(CbmRichRing* ring, double momentum)
{
	int identified = 0;

	if (nullptr != ring){ 
		CbmRichRingLight ringHit;
		int nofHits = ring->GetNofHits();
		for (int i = 0; i < nofHits; i++){
			int hitInd = ring->GetHit(i);
			CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
			if (nullptr == hit) continue;
			CbmRichHitLight hl(hit->GetX(), hit->GetY());
			ringHit.AddHit(hl);
		}
		fTauFit->DoFit(&ringHit);
		if (ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6  && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6 && momentum > 0.2 && momentum < 4.) identified ++;
		//if (ring->GetDistance() < 2 && ringHit.GetAaxis() > 4 && ringHit.GetAaxis() < 6  && ringHit.GetBaxis() > 4 && ringHit.GetBaxis() < 6 && momentum > 0.2 && momentum < 4.) identified ++;
	}

	return identified;
}



double CbmKresConversionManualmbias3::CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
{
	double FinalAngle = 400;
	Int_t hits1sts = Sts_1->GetNofStsHits();
	Int_t hits2sts = Sts_2->GetNofStsHits();
	Int_t hits1mvd = Sts_1->GetNofMvdHits();
	Int_t hits2mvd = Sts_2->GetNofMvdHits();

	double Xpart1 = 0;
	double Ypart1 = 0;
	double Zpart1 = 0;
	double Xpart2 = 0; 
	double Ypart2 = 0;
	double Zpart2 = 0;
	if (hits1sts > 0){
		Int_t stsHitIndex1 = Sts_1->GetStsHitIndex(hits1sts-1);
		CbmStsHit* stsHit1 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex1);
		Xpart1 = stsHit1->GetX(); 
		Ypart1 = stsHit1->GetY(); 
		Zpart1 = stsHit1->GetZ();
	} else{
		Int_t mvdHitIndex1 = Sts_1->GetMvdHitIndex(hits1mvd-1);
		CbmMvdHit* mvdHit1 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex1);
		Xpart1 = mvdHit1->GetX(); 
		Ypart1 = mvdHit1->GetY(); 
		Zpart1 = mvdHit1->GetZ();
	}

	if (hits2sts > 0){
		Int_t stsHitIndex2 = Sts_2->GetStsHitIndex(hits2sts-1);
		CbmStsHit* stsHit2 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex2);
		Xpart2 = stsHit2->GetX(); 
		Ypart2 = stsHit2->GetY(); 
		Zpart2 = stsHit2->GetZ();
	} else{
		Int_t mvdHitIndex2 = Sts_2->GetMvdHitIndex(hits2mvd-1);
		CbmMvdHit* mvdHit2 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex2);
		Xpart2 = mvdHit2->GetX(); 
		Ypart2 = mvdHit2->GetY(); 
		Zpart2 = mvdHit2->GetZ();
	}

	// check difference in 2 cm, because of two slices of every STS and Mvd stations
	if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 > Zpart2){	// if last hits are in different stations --> try to find the latest common station
		for (int i = hits1sts-2; i > -1; i--){	// start from second last station
			Int_t stsHitIndex = Sts_1->GetStsHitIndex(i);
			CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
			Xpart1 = stsHit->GetX(); 
			Ypart1 = stsHit->GetY(); 
			Zpart1 = stsHit->GetZ();
			if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
		}
		if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 > Zpart2){
			for (int i = hits1mvd-2; i > -1; i--){
				Int_t mvdHitIndex = Sts_1->GetMvdHitIndex(i);
				CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
				Xpart1 = mvdHit->GetX(); 
				Ypart1 = mvdHit->GetY(); 
				Zpart1 = mvdHit->GetZ();
				if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
			}
		}
	}

	if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 < Zpart2){	// if last hits are in different stations --> try to find the latest common station
		for (int i = hits2sts-2; i > -1; i--){	// start from second last station
			Int_t stsHitIndex = Sts_2->GetStsHitIndex(i);
			CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
			Xpart2 = stsHit->GetX(); 
			Ypart2 = stsHit->GetY(); 
			Zpart2 = stsHit->GetZ();
			if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
		}
		if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 < Zpart2){
			for (int i = hits2mvd-2; i > -1; i--){
				Int_t mvdHitIndex = Sts_2->GetMvdHitIndex(i);
				CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
				Xpart2 = mvdHit->GetX(); 
				Ypart2 = mvdHit->GetY(); 
				Zpart2 = mvdHit->GetZ();
				if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
			}
		}
	}
	
	// calculate angle if we found common station
	if ( TMath::Abs(Zpart1-Zpart2) < 2 && Zpart1 != 0 && Zpart2 != 0){
		FinalAngle = TMath::ATan2(Ypart1-Ypart2,Xpart1-Xpart2)*(180/TMath::Pi());
	}

	return TMath::Abs( TMath::Abs(FinalAngle) - 180 );
}

double CbmKresConversionManualmbias3::CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2)
{
	double FinalAngle = 400;
	Int_t hits1sts = Sts_1->GetNofStsHits();
	Int_t hits2sts = Sts_2->GetNofStsHits();
	Int_t hits1mvd = Sts_1->GetNofMvdHits();
	Int_t hits2mvd = Sts_2->GetNofMvdHits();

	double Xpart1 = 0;
	double Ypart1 = 0;
	double Zpart1 = 0;
	double Xpart2 = 0; 
	double Ypart2 = 0;
	double Zpart2 = 0;
	if (hits1mvd > 0){
		Int_t mvdHitIndex1 = Sts_1->GetMvdHitIndex(0);
		CbmMvdHit* mvdHit1 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex1);
		Xpart1 = mvdHit1->GetX(); 
		Ypart1 = mvdHit1->GetY(); 
		Zpart1 = mvdHit1->GetZ();		
	} else{
		Int_t stsHitIndex1 = Sts_1->GetStsHitIndex(0);
		CbmStsHit* stsHit1 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex1);
		Xpart1 = stsHit1->GetX(); 
		Ypart1 = stsHit1->GetY(); 
		Zpart1 = stsHit1->GetZ();
	}

	if (hits2mvd > 0){
		Int_t mvdHitIndex2 = Sts_2->GetMvdHitIndex(0);
		CbmMvdHit* mvdHit2 = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex2);
		Xpart2 = mvdHit2->GetX(); 
		Ypart2 = mvdHit2->GetY(); 
		Zpart2 = mvdHit2->GetZ();
	} else{
		Int_t stsHitIndex2 = Sts_2->GetStsHitIndex(0);
		CbmStsHit* stsHit2 = (CbmStsHit*) fArrayStsHit->At(stsHitIndex2);
		Xpart2 = stsHit2->GetX(); 
		Ypart2 = stsHit2->GetY(); 
		Zpart2 = stsHit2->GetZ();
	}

	// check difference in 2 cm, because of two slices of every STS and Mvd stations
	if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 < Zpart2){	// if first hits are in different stations --> try to find the earliest common station
		for (int i = 1; i < hits1mvd; i++){	// start from second hit
			Int_t mvdHitIndex = Sts_1->GetMvdHitIndex(i);
			CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
			Xpart1 = mvdHit->GetX(); 
			Ypart1 = mvdHit->GetY(); 
			Zpart1 = mvdHit->GetZ();			
			if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
		}
		if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 < Zpart2){
			for (int i = 0; i < hits1sts; i++){
				Int_t stsHitIndex = Sts_1->GetStsHitIndex(i);
				CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
				Xpart1 = stsHit->GetX(); 
				Ypart1 = stsHit->GetY(); 
				Zpart1 = stsHit->GetZ();				
				if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
			}
		}
	}

	if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 > Zpart2){	// if first hits are in different stations --> try to find the earliest common station
		for (int i = 1; i < hits2mvd; i++){	// start from second hit
			Int_t mvdHitIndex = Sts_2->GetMvdHitIndex(i);
			CbmMvdHit* mvdHit = (CbmMvdHit*) fArrayMvdHit->At(mvdHitIndex);
			Xpart2 = mvdHit->GetX(); 
			Ypart2 = mvdHit->GetY(); 
			Zpart2 = mvdHit->GetZ();			
			if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
		}
		if (TMath::Abs(Zpart1-Zpart2) > 2 && Zpart1 > Zpart2){
			for (int i = 0; i < hits2sts; i++){
				Int_t stsHitIndex = Sts_2->GetStsHitIndex(i);
				CbmStsHit* stsHit = (CbmStsHit*) fArrayStsHit->At(stsHitIndex);
				Xpart2 = stsHit->GetX(); 
				Ypart2 = stsHit->GetY(); 
				Zpart2 = stsHit->GetZ();				
				if (TMath::Abs(Zpart1-Zpart2) < 2) continue;
			}
		}
	}
	
	// calculate angle if we found common station
	if ( TMath::Abs(Zpart1-Zpart2) < 2 && Zpart1 != 0 && Zpart2 != 0){
		FinalAngle = TMath::ATan2(Ypart1-Ypart2,Xpart1-Xpart2)*(180/TMath::Pi());
	}

	return TMath::Abs( TMath::Abs(FinalAngle) - 180 );
}





void CbmKresConversionManualmbias3::Finish()
{
	gDirectory->mkdir("Target_mbias3");
	gDirectory->cd("Target_mbias3");
	gDirectory->mkdir("CheckCuts_Target_mbias3");
	gDirectory->cd("CheckCuts_Target_mbias3");
	for (UInt_t i = 0; i < fHistoList_man_cuts_Target_mbias3.size(); i++){
		fHistoList_man_cuts_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("multiplicity");
	gDirectory->cd("multiplicity");
	for (UInt_t i = 0; i < fHistoList_multiplicity_man_Target_mbias3.size(); i++){
		fHistoList_multiplicity_man_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("all");
	gDirectory->cd("all");
	for (UInt_t i = 0; i < fHistoList_man_all_Target_mbias3.size(); i++){
		fHistoList_man_all_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("zero");
	gDirectory->cd("zero");
	for (UInt_t i = 0; i < fHistoList_man_zero_Target_mbias3.size(); i++){
		fHistoList_man_zero_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("one");
	gDirectory->cd("one");
	for (UInt_t i = 0; i < fHistoList_man_one_Target_mbias3.size(); i++){
		fHistoList_man_one_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("two");
	gDirectory->cd("two");
	for (UInt_t i = 0; i < fHistoList_man_two_Target_mbias3.size(); i++){
		fHistoList_man_two_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("onetwo");
	gDirectory->cd("onetwo");
	for (UInt_t i = 0; i < fHistoList_man_onetwo_Target_mbias3.size(); i++){
		fHistoList_man_onetwo_Target_mbias3[i]->Write();
	}
	gDirectory->cd("..");

	gDirectory->cd("..");




	gDirectory->mkdir("Outside_mbias3");
	gDirectory->cd("Outside_mbias3");
	gDirectory->mkdir("CheckCuts_Outside_mbias3");
	gDirectory->cd("CheckCuts_Outside_mbias3");
	for (UInt_t i = 0; i < fHistoList_man_cuts_Outside_mbias3.size(); i++){
		fHistoList_man_cuts_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("multiplicity");
	gDirectory->cd("multiplicity");
	for (UInt_t i = 0; i < fHistoList_multiplicity_man_Outside_mbias3.size(); i++){
		fHistoList_multiplicity_man_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("all");
	gDirectory->cd("all");
	for (UInt_t i = 0; i < fHistoList_man_all_Outside_mbias3.size(); i++){
		fHistoList_man_all_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("zero");
	gDirectory->cd("zero");
	for (UInt_t i = 0; i < fHistoList_man_zero_Outside_mbias3.size(); i++){
		fHistoList_man_zero_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("one");
	gDirectory->cd("one");
	for (UInt_t i = 0; i < fHistoList_man_one_Outside_mbias3.size(); i++){
		fHistoList_man_one_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("two");
	gDirectory->cd("two");
	for (UInt_t i = 0; i < fHistoList_man_two_Outside_mbias3.size(); i++){
		fHistoList_man_two_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("onetwo");
	gDirectory->cd("onetwo");
	for (UInt_t i = 0; i < fHistoList_man_onetwo_Outside_mbias3.size(); i++){
		fHistoList_man_onetwo_Outside_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->cd("..");




	gDirectory->mkdir("Both_mbias3");
	gDirectory->cd("Both_mbias3");
	gDirectory->mkdir("CheckCuts_Both_mbias3");
	gDirectory->cd("CheckCuts_Both_mbias3");
	for (UInt_t i = 0; i < fHistoList_man_cuts_Both_mbias3.size(); i++){
		fHistoList_man_cuts_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("multiplicity");
	gDirectory->cd("multiplicity");
	for (UInt_t i = 0; i < fHistoList_multiplicity_man_Both_mbias3.size(); i++){
		fHistoList_multiplicity_man_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("rap_vs_Pt_onetwo_Both_mbias3");
	gDirectory->cd("rap_vs_Pt_onetwo_Both_mbias3");
	for (UInt_t i = 0; i < fHistoList_rap_vs_pt_InM_mbias3.size(); i++){
		fHistoList_rap_vs_pt_InM_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("BG_Both_mbias3");
	gDirectory->cd("BG_Both_mbias3");
	gDirectory->mkdir("BG_Both_mbias3_all");
	gDirectory->cd("BG_Both_mbias3_all");
	for (UInt_t i = 0; i < fHistoList_bg_InM_all_Both_mbias3.size(); i++){
		fHistoList_bg_InM_all_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("BG_Both_mbias3_zero");
	gDirectory->cd("BG_Both_mbias3_zero");
	for (UInt_t i = 0; i < fHistoList_bg_InM_zero_Both_mbias3.size(); i++){
		fHistoList_bg_InM_zero_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("BG_Both_mbias3_one");
	gDirectory->cd("BG_Both_mbias3_one");
	for (UInt_t i = 0; i < fHistoList_bg_InM_one_Both_mbias3.size(); i++){
		fHistoList_bg_InM_one_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("BG_Both_mbias3_two");
	gDirectory->cd("BG_Both_mbias3_two");
	for (UInt_t i = 0; i < fHistoList_bg_InM_two_Both_mbias3.size(); i++){
		fHistoList_bg_InM_two_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("BG_Both_mbias3_onetwo");
	gDirectory->cd("BG_Both_mbias3_onetwo");
	for (UInt_t i = 0; i < fHistoList_bg_InM_onetwo_Both_mbias3.size(); i++){
		fHistoList_bg_InM_onetwo_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->cd("..");
	gDirectory->mkdir("all");
	gDirectory->cd("all");
	for (UInt_t i = 0; i < fHistoList_man_all_Both_mbias3.size(); i++){
		fHistoList_man_all_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("zero");
	gDirectory->cd("zero");
	for (UInt_t i = 0; i < fHistoList_man_zero_Both_mbias3.size(); i++){
		fHistoList_man_zero_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("one");
	gDirectory->cd("one");
	for (UInt_t i = 0; i < fHistoList_man_one_Both_mbias3.size(); i++){
		fHistoList_man_one_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("two");
	gDirectory->cd("two");
	for (UInt_t i = 0; i < fHistoList_man_two_Both_mbias3.size(); i++){
		fHistoList_man_two_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	gDirectory->mkdir("onetwo");
	gDirectory->cd("onetwo");
	for (UInt_t i = 0; i < fHistoList_man_onetwo_Both_mbias3.size(); i++){
		fHistoList_man_onetwo_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");
	for (UInt_t i = 0; i < fHistoList_man_Both_mbias3.size(); i++){
		fHistoList_man_Both_mbias3[i]->Write();
	}
	gDirectory->cd("..");


}

void CbmKresConversionManualmbias3::InitHistograms()
{
		///////   histograms to check Cuts
	InvMass_vs_OA_candidates_Both_mbias3 = new TH2D("InvMass_vs_OA_candidates_Both_mbias3","InvMass_vs_OA_candidates_Both_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Both_mbias3.push_back(InvMass_vs_OA_candidates_Both_mbias3);
	InvMass_vs_OA_fromPi0_Both_mbias3 = new TH2D("InvMass_vs_OA_fromPi0_Both_mbias3","InvMass_vs_OA_fromPi0_Both_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Both_mbias3.push_back(InvMass_vs_OA_fromPi0_Both_mbias3);
	GammasInvMass_candidates_Both_mbias3 = new TH1D("GammasInvMass_candidates_Both_mbias3","GammasInvMass_candidates_Both_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Both_mbias3.push_back(GammasInvMass_candidates_Both_mbias3);
	GammasOA_candidates_Both_mbias3 = new TH1D("GammasOA_candidates_Both_mbias3","GammasOA_candidates_Both_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Both_mbias3.push_back(GammasOA_candidates_Both_mbias3);
	GammasInvMass_fromPi0_Both_mbias3 = new TH1D("GammasInvMass_fromPi0_Both_mbias3","GammasInvMass_fromPi0_Both_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Both_mbias3.push_back(GammasInvMass_fromPi0_Both_mbias3);
	GammasOA_fromPi0_Both_mbias3 = new TH1D("GammasOA_fromPi0_Both_mbias3","GammasOA_fromPi0_Both_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Both_mbias3.push_back(GammasOA_fromPi0_Both_mbias3);
	PlaneAngles_last_candidates_Both_mbias3 = new TH1D("PlaneAngles_last_candidates_Both_mbias3","PlaneAngles_last_candidates_Both_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Both_mbias3.push_back(PlaneAngles_last_candidates_Both_mbias3);
	PlaneAngles_last_fromPi0_Both_mbias3 = new TH1D("PlaneAngles_last_fromPi0_Both_mbias3","PlaneAngles_last_fromPi0_Both_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Both_mbias3.push_back(PlaneAngles_last_fromPi0_Both_mbias3);
	PlaneAngles_first_candidates_Both_mbias3 = new TH1D("PlaneAngles_first_candidates_Both_mbias3","PlaneAngles_first_candidates_Both_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Both_mbias3.push_back(PlaneAngles_first_candidates_Both_mbias3);
	PlaneAngles_first_fromPi0_Both_mbias3 = new TH1D("PlaneAngles_first_fromPi0_Both_mbias3","PlaneAngles_first_fromPi0_Both_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Both_mbias3.push_back(PlaneAngles_first_fromPi0_Both_mbias3);




	InvMass_vs_OA_candidates_Target_mbias3 = new TH2D("InvMass_vs_OA_candidates_Target_mbias3","InvMass_vs_OA_candidates_Target_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Target_mbias3.push_back(InvMass_vs_OA_candidates_Target_mbias3);
	InvMass_vs_OA_fromPi0_Target_mbias3 = new TH2D("InvMass_vs_OA_fromPi0_Target_mbias3","InvMass_vs_OA_fromPi0_Target_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Target_mbias3.push_back(InvMass_vs_OA_fromPi0_Target_mbias3);
	GammasInvMass_candidates_Target_mbias3 = new TH1D("GammasInvMass_candidates_Target_mbias3","GammasInvMass_candidates_Target_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Target_mbias3.push_back(GammasInvMass_candidates_Target_mbias3);
	GammasOA_candidates_Target_mbias3 = new TH1D("GammasOA_candidates_Target_mbias3","GammasOA_candidates_Target_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Target_mbias3.push_back(GammasOA_candidates_Target_mbias3);
	GammasInvMass_fromPi0_Target_mbias3 = new TH1D("GammasInvMass_fromPi0_Target_mbias3","GammasInvMass_fromPi0_Target_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Target_mbias3.push_back(GammasInvMass_fromPi0_Target_mbias3);
	GammasOA_fromPi0_Target_mbias3 = new TH1D("GammasOA_fromPi0_Target_mbias3","GammasOA_fromPi0_Target_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Target_mbias3.push_back(GammasOA_fromPi0_Target_mbias3);
	PlaneAngles_last_candidates_Target_mbias3 = new TH1D("PlaneAngles_last_candidates_Target_mbias3","PlaneAngles_last_candidates_Target_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Target_mbias3.push_back(PlaneAngles_last_candidates_Target_mbias3);
	PlaneAngles_last_fromPi0_Target_mbias3 = new TH1D("PlaneAngles_last_fromPi0_Target_mbias3","PlaneAngles_last_fromPi0_Target_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Target_mbias3.push_back(PlaneAngles_last_fromPi0_Target_mbias3);
	PlaneAngles_first_candidates_Target_mbias3 = new TH1D("PlaneAngles_first_candidates_Target_mbias3","PlaneAngles_first_candidates_Target_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Target_mbias3.push_back(PlaneAngles_first_candidates_Target_mbias3);
	PlaneAngles_first_fromPi0_Target_mbias3 = new TH1D("PlaneAngles_first_fromPi0_Target_mbias3","PlaneAngles_first_fromPi0_Target_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Target_mbias3.push_back(PlaneAngles_first_fromPi0_Target_mbias3);






	InvMass_vs_OA_candidates_Outside_mbias3 = new TH2D("InvMass_vs_OA_candidates_Outside_mbias3","InvMass_vs_OA_candidates_Outside_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Outside_mbias3.push_back(InvMass_vs_OA_candidates_Outside_mbias3);
	InvMass_vs_OA_fromPi0_Outside_mbias3 = new TH2D("InvMass_vs_OA_fromPi0_Outside_mbias3","InvMass_vs_OA_fromPi0_Outside_mbias3; invariant mass in GeV/c^{2}; opening angle in degree", 500, 0, 0.5, 500, 0, 50);
	fHistoList_man_cuts_Outside_mbias3.push_back(InvMass_vs_OA_fromPi0_Outside_mbias3);
	GammasInvMass_candidates_Outside_mbias3 = new TH1D("GammasInvMass_candidates_Outside_mbias3","GammasInvMass_candidates_Outside_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Outside_mbias3.push_back(GammasInvMass_candidates_Outside_mbias3);
	GammasOA_candidates_Outside_mbias3 = new TH1D("GammasOA_candidates_Outside_mbias3","GammasOA_candidates_Outside_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Outside_mbias3.push_back(GammasOA_candidates_Outside_mbias3);
	GammasInvMass_fromPi0_Outside_mbias3 = new TH1D("GammasInvMass_fromPi0_Outside_mbias3","GammasInvMass_fromPi0_Outside_mbias3; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
	fHistoList_man_cuts_Outside_mbias3.push_back(GammasInvMass_fromPi0_Outside_mbias3);
	GammasOA_fromPi0_Outside_mbias3 = new TH1D("GammasOA_fromPi0_Outside_mbias3","GammasOA_fromPi0_Outside_mbias3; opening angle in degree;#", 300, -0.1, 29.9);
	fHistoList_man_cuts_Outside_mbias3.push_back(GammasOA_fromPi0_Outside_mbias3);
	PlaneAngles_last_candidates_Outside_mbias3 = new TH1D("PlaneAngles_last_candidates_Outside_mbias3","PlaneAngles_last_candidates_Outside_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Outside_mbias3.push_back(PlaneAngles_last_candidates_Outside_mbias3);
	PlaneAngles_last_fromPi0_Outside_mbias3 = new TH1D("PlaneAngles_last_fromPi0_Outside_mbias3","PlaneAngles_last_fromPi0_Outside_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Outside_mbias3.push_back(PlaneAngles_last_fromPi0_Outside_mbias3);
	PlaneAngles_first_candidates_Outside_mbias3 = new TH1D("PlaneAngles_first_candidates_Outside_mbias3","PlaneAngles_first_candidates_Outside_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Outside_mbias3.push_back(PlaneAngles_first_candidates_Outside_mbias3);
	PlaneAngles_first_fromPi0_Outside_mbias3 = new TH1D("PlaneAngles_first_fromPi0_Outside_mbias3","PlaneAngles_first_fromPi0_Outside_mbias3; #theta angle in degree;#", 720, -1., 179.);
	fHistoList_man_cuts_Outside_mbias3.push_back(PlaneAngles_first_fromPi0_Outside_mbias3);






	// Target_mbias3 => all
	GammaInvMassReco_all_Target_mbias3 = new TH1D("GammaInvMassReco_all_Target_mbias3","GammaInvMassReco_all_Target_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_all_Target_mbias3.push_back(GammaInvMassReco_all_Target_mbias3);
	GammaOpeningAngleReco_all_Target_mbias3 = new TH1D("GammaOpeningAngleReco_all_Target_mbias3","GammaOpeningAngleReco_all_Target_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_all_Target_mbias3.push_back(GammaOpeningAngleReco_all_Target_mbias3);
	Pdg_all_Target_mbias3 = new TH1D("Pdg_all_Target_mbias3","Pdg_all_Target_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_all_Target_mbias3.push_back(Pdg_all_Target_mbias3);
	P_reco_all_Target_mbias3 = new TH1D("P_reco_all_Target_mbias3","P_reco_all_Target_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_all_Target_mbias3.push_back(P_reco_all_Target_mbias3);
	Pt_reco_all_Target_mbias3 = new TH1D("Pt_reco_all_Target_mbias3","Pt_reco_all_Target_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_all_Target_mbias3.push_back(Pt_reco_all_Target_mbias3);
	Pi0InvMassReco_all_Target_mbias3 = new TH1D("Pi0InvMassReco_all_Target_mbias3","Pi0InvMassReco_all_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Target_mbias3.push_back(Pi0InvMassReco_all_Target_mbias3);
	EMT_InvMass_all_Target_mbias3 = new TH1D("EMT_InvMass_all_Target_mbias3","EMT_InvMass_all_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Target_mbias3.push_back(EMT_InvMass_all_Target_mbias3);
	Pi0_pt_vs_rap_all_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_all_Target_mbias3", "Pi0_pt_vs_rap_all_Target_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_all_Target_mbias3.push_back(Pi0_pt_vs_rap_all_Target_mbias3);
	Pi0_pt_vs_rap_est_all_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_est_all_Target_mbias3", "Pi0_pt_vs_rap_est_all_Target_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_all_Target_mbias3.push_back(Pi0_pt_vs_rap_est_all_Target_mbias3); 


	// Target_mbias3 => zero
	GammaInvMassReco_zero_Target_mbias3 = new TH1D("GammaInvMassReco_zero_Target_mbias3","GammaInvMassReco_zero_Target_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_zero_Target_mbias3.push_back(GammaInvMassReco_zero_Target_mbias3);
	GammaOpeningAngleReco_zero_Target_mbias3 = new TH1D("GammaOpeningAngleReco_zero_Target_mbias3","GammaOpeningAngleReco_zero_Target_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_zero_Target_mbias3.push_back(GammaOpeningAngleReco_zero_Target_mbias3);
	Pdg_zero_Target_mbias3 = new TH1D("Pdg_zero_Target_mbias3","Pdg_zero_Target_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_zero_Target_mbias3.push_back(Pdg_zero_Target_mbias3);
	P_reco_zero_Target_mbias3 = new TH1D("P_reco_zero_Target_mbias3","P_reco_zero_Target_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_zero_Target_mbias3.push_back(P_reco_zero_Target_mbias3);
	Pt_reco_zero_Target_mbias3 = new TH1D("Pt_reco_zero_Target_mbias3","Pt_reco_zero_Target_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_zero_Target_mbias3.push_back(Pt_reco_zero_Target_mbias3);
	Pi0InvMassReco_zero_Target_mbias3 = new TH1D("Pi0InvMassReco_zero_Target_mbias3","Pi0InvMassReco_zero_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Target_mbias3.push_back(Pi0InvMassReco_zero_Target_mbias3);
	EMT_InvMass_zero_Target_mbias3 = new TH1D("EMT_InvMass_zero_Target_mbias3","EMT_InvMass_zero_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Target_mbias3.push_back(EMT_InvMass_zero_Target_mbias3);
	Pi0_pt_vs_rap_zero_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_zero_Target_mbias3", "Pi0_pt_vs_rap_zero_Target_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_zero_Target_mbias3.push_back(Pi0_pt_vs_rap_zero_Target_mbias3);
	Pi0_pt_vs_rap_est_zero_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_est_zero_Target_mbias3", "Pi0_pt_vs_rap_est_zero_Target_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_zero_Target_mbias3.push_back(Pi0_pt_vs_rap_est_zero_Target_mbias3); 



	// Target_mbias3 => one
	GammaInvMassReco_one_Target_mbias3 = new TH1D("GammaInvMassReco_one_Target_mbias3","GammaInvMassReco_one_Target_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_one_Target_mbias3.push_back(GammaInvMassReco_one_Target_mbias3);
	GammaOpeningAngleReco_one_Target_mbias3 = new TH1D("GammaOpeningAngleReco_one_Target_mbias3","GammaOpeningAngleReco_one_Target_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_one_Target_mbias3.push_back(GammaOpeningAngleReco_one_Target_mbias3);
	Pdg_one_Target_mbias3 = new TH1D("Pdg_one_Target_mbias3","Pdg_one_Target_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_one_Target_mbias3.push_back(Pdg_one_Target_mbias3);
	P_reco_one_Target_mbias3 = new TH1D("P_reco_one_Target_mbias3","P_reco_one_Target_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_one_Target_mbias3.push_back(P_reco_one_Target_mbias3);
	Pt_reco_one_Target_mbias3 = new TH1D("Pt_reco_one_Target_mbias3","Pt_reco_one_Target_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_one_Target_mbias3.push_back(Pt_reco_one_Target_mbias3);
	Pi0InvMassReco_one_Target_mbias3 = new TH1D("Pi0InvMassReco_one_Target_mbias3","Pi0InvMassReco_one_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Target_mbias3.push_back(Pi0InvMassReco_one_Target_mbias3);
	EMT_InvMass_one_Target_mbias3 = new TH1D("EMT_InvMass_one_Target_mbias3","EMT_InvMass_one_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Target_mbias3.push_back(EMT_InvMass_one_Target_mbias3);
	Pi0_pt_vs_rap_one_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_one_Target_mbias3", "Pi0_pt_vs_rap_one_Target_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_one_Target_mbias3.push_back(Pi0_pt_vs_rap_one_Target_mbias3);
	Pi0_pt_vs_rap_est_one_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_est_one_Target_mbias3", "Pi0_pt_vs_rap_est_one_Target_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_one_Target_mbias3.push_back(Pi0_pt_vs_rap_est_one_Target_mbias3); 


	// Target_mbias3 => two
	GammaInvMassReco_two_Target_mbias3 = new TH1D("GammaInvMassReco_two_Target_mbias3","GammaInvMassReco_two_Target_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_two_Target_mbias3.push_back(GammaInvMassReco_two_Target_mbias3);
	GammaOpeningAngleReco_two_Target_mbias3 = new TH1D("GammaOpeningAngleReco_two_Target_mbias3","GammaOpeningAngleReco_two_Target_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_two_Target_mbias3.push_back(GammaOpeningAngleReco_two_Target_mbias3);
	Pdg_two_Target_mbias3 = new TH1D("Pdg_two_Target_mbias3","Pdg_two_Target_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_two_Target_mbias3.push_back(Pdg_two_Target_mbias3);
	P_reco_two_Target_mbias3 = new TH1D("P_reco_two_Target_mbias3","P_reco_two_Target_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_two_Target_mbias3.push_back(P_reco_two_Target_mbias3);
	Pt_reco_two_Target_mbias3 = new TH1D("Pt_reco_two_Target_mbias3","Pt_reco_two_Target_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_two_Target_mbias3.push_back(Pt_reco_two_Target_mbias3);
	Pi0InvMassReco_two_Target_mbias3 = new TH1D("Pi0InvMassReco_two_Target_mbias3","Pi0InvMassReco_two_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Target_mbias3.push_back(Pi0InvMassReco_two_Target_mbias3);
	EMT_InvMass_two_Target_mbias3 = new TH1D("EMT_InvMass_two_Target_mbias3","EMT_InvMass_two_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Target_mbias3.push_back(EMT_InvMass_two_Target_mbias3);
	Pi0_pt_vs_rap_two_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_two_Target_mbias3", "Pi0_pt_vs_rap_two_Target_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_two_Target_mbias3.push_back(Pi0_pt_vs_rap_two_Target_mbias3);
	Pi0_pt_vs_rap_est_two_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_est_two_Target_mbias3", "Pi0_pt_vs_rap_est_two_Target_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_two_Target_mbias3.push_back(Pi0_pt_vs_rap_est_two_Target_mbias3); 


	// Target_mbias3 => onetwo
	GammaInvMassReco_onetwo_Target_mbias3 = new TH1D("GammaInvMassReco_onetwo_Target_mbias3","GammaInvMassReco_onetwo_Target_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_onetwo_Target_mbias3.push_back(GammaInvMassReco_onetwo_Target_mbias3);
	GammaOpeningAngleReco_onetwo_Target_mbias3 = new TH1D("GammaOpeningAngleReco_onetwo_Target_mbias3","GammaOpeningAngleReco_onetwo_Target_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_onetwo_Target_mbias3.push_back(GammaOpeningAngleReco_onetwo_Target_mbias3);
	Pdg_onetwo_Target_mbias3 = new TH1D("Pdg_onetwo_Target_mbias3","Pdg_onetwo_Target_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_onetwo_Target_mbias3.push_back(Pdg_onetwo_Target_mbias3);
	P_reco_onetwo_Target_mbias3 = new TH1D("P_reco_onetwo_Target_mbias3","P_reco_onetwo_Target_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_onetwo_Target_mbias3.push_back(P_reco_onetwo_Target_mbias3);
	Pt_reco_onetwo_Target_mbias3 = new TH1D("Pt_reco_onetwo_Target_mbias3","Pt_reco_onetwo_Target_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_onetwo_Target_mbias3.push_back(Pt_reco_onetwo_Target_mbias3);
	Pi0InvMassReco_onetwo_Target_mbias3 = new TH1D("Pi0InvMassReco_onetwo_Target_mbias3","Pi0InvMassReco_onetwo_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Target_mbias3.push_back(Pi0InvMassReco_onetwo_Target_mbias3);
	EMT_InvMass_onetwo_Target_mbias3 = new TH1D("EMT_InvMass_onetwo_Target_mbias3","EMT_InvMass_onetwo_Target_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Target_mbias3.push_back(EMT_InvMass_onetwo_Target_mbias3);
	Pi0_pt_vs_rap_onetwo_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_onetwo_Target_mbias3", "Pi0_pt_vs_rap_onetwo_Target_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_onetwo_Target_mbias3.push_back(Pi0_pt_vs_rap_onetwo_Target_mbias3);
	Pi0_pt_vs_rap_est_onetwo_Target_mbias3 = new TH2D("Pi0_pt_vs_rap_est_onetwo_Target_mbias3", "Pi0_pt_vs_rap_est_onetwo_Target_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_onetwo_Target_mbias3.push_back(Pi0_pt_vs_rap_est_onetwo_Target_mbias3); 



	






	// Outside_mbias3 => all
	GammaInvMassReco_all_Outside_mbias3 = new TH1D("GammaInvMassReco_all_Outside_mbias3","GammaInvMassReco_all_Outside_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_all_Outside_mbias3.push_back(GammaInvMassReco_all_Outside_mbias3);
	GammaOpeningAngleReco_all_Outside_mbias3 = new TH1D("GammaOpeningAngleReco_all_Outside_mbias3","GammaOpeningAngleReco_all_Outside_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_all_Outside_mbias3.push_back(GammaOpeningAngleReco_all_Outside_mbias3);
	Pdg_all_Outside_mbias3 = new TH1D("Pdg_all_Outside_mbias3","Pdg_all_Outside_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_all_Outside_mbias3.push_back(Pdg_all_Outside_mbias3);
	P_reco_all_Outside_mbias3 = new TH1D("P_reco_all_Outside_mbias3","P_reco_all_Outside_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_all_Outside_mbias3.push_back(P_reco_all_Outside_mbias3);
	Pt_reco_all_Outside_mbias3 = new TH1D("Pt_reco_all_Outside_mbias3","Pt_reco_all_Outside_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_all_Outside_mbias3.push_back(Pt_reco_all_Outside_mbias3);
	Pi0InvMassReco_all_Outside_mbias3 = new TH1D("Pi0InvMassReco_all_Outside_mbias3","Pi0InvMassReco_all_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Outside_mbias3.push_back(Pi0InvMassReco_all_Outside_mbias3);
	EMT_InvMass_all_Outside_mbias3 = new TH1D("EMT_InvMass_all_Outside_mbias3","EMT_InvMass_all_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Outside_mbias3.push_back(EMT_InvMass_all_Outside_mbias3);
	Pi0_pt_vs_rap_all_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_all_Outside_mbias3", "Pi0_pt_vs_rap_all_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_all_Outside_mbias3.push_back(Pi0_pt_vs_rap_all_Outside_mbias3);
	Pi0_pt_vs_rap_est_all_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_est_all_Outside_mbias3", "Pi0_pt_vs_rap_est_all_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_all_Outside_mbias3.push_back(Pi0_pt_vs_rap_est_all_Outside_mbias3); 


	// Outside_mbias3 => zero
	GammaInvMassReco_zero_Outside_mbias3 = new TH1D("GammaInvMassReco_zero_Outside_mbias3","GammaInvMassReco_zero_Outside_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_zero_Outside_mbias3.push_back(GammaInvMassReco_zero_Outside_mbias3);
	GammaOpeningAngleReco_zero_Outside_mbias3 = new TH1D("GammaOpeningAngleReco_zero_Outside_mbias3","GammaOpeningAngleReco_zero_Outside_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_zero_Outside_mbias3.push_back(GammaOpeningAngleReco_zero_Outside_mbias3);
	Pdg_zero_Outside_mbias3 = new TH1D("Pdg_zero_Outside_mbias3","Pdg_zero_Outside_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_zero_Outside_mbias3.push_back(Pdg_zero_Outside_mbias3);
	P_reco_zero_Outside_mbias3 = new TH1D("P_reco_zero_Outside_mbias3","P_reco_zero_Outside_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_zero_Outside_mbias3.push_back(P_reco_zero_Outside_mbias3);
	Pt_reco_zero_Outside_mbias3 = new TH1D("Pt_reco_zero_Outside_mbias3","Pt_reco_zero_Outside_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_zero_Outside_mbias3.push_back(Pt_reco_zero_Outside_mbias3);
	Pi0InvMassReco_zero_Outside_mbias3 = new TH1D("Pi0InvMassReco_zero_Outside_mbias3","Pi0InvMassReco_zero_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Outside_mbias3.push_back(Pi0InvMassReco_zero_Outside_mbias3);
	EMT_InvMass_zero_Outside_mbias3 = new TH1D("EMT_InvMass_zero_Outside_mbias3","EMT_InvMass_zero_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Outside_mbias3.push_back(EMT_InvMass_zero_Outside_mbias3);
	Pi0_pt_vs_rap_zero_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_zero_Outside_mbias3", "Pi0_pt_vs_rap_zero_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_zero_Outside_mbias3.push_back(Pi0_pt_vs_rap_zero_Outside_mbias3);
	Pi0_pt_vs_rap_est_zero_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_est_zero_Outside_mbias3", "Pi0_pt_vs_rap_est_zero_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_zero_Outside_mbias3.push_back(Pi0_pt_vs_rap_est_zero_Outside_mbias3); 


	// Outside_mbias3 => one
	GammaInvMassReco_one_Outside_mbias3 = new TH1D("GammaInvMassReco_one_Outside_mbias3","GammaInvMassReco_one_Outside_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_one_Outside_mbias3.push_back(GammaInvMassReco_one_Outside_mbias3);
	GammaOpeningAngleReco_one_Outside_mbias3 = new TH1D("GammaOpeningAngleReco_one_Outside_mbias3","GammaOpeningAngleReco_one_Outside_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_one_Outside_mbias3.push_back(GammaOpeningAngleReco_one_Outside_mbias3);
	Pdg_one_Outside_mbias3 = new TH1D("Pdg_one_Outside_mbias3","Pdg_one_Outside_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_one_Outside_mbias3.push_back(Pdg_one_Outside_mbias3);
	P_reco_one_Outside_mbias3 = new TH1D("P_reco_one_Outside_mbias3","P_reco_one_Outside_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_one_Outside_mbias3.push_back(P_reco_one_Outside_mbias3);
	Pt_reco_one_Outside_mbias3 = new TH1D("Pt_reco_one_Outside_mbias3","Pt_reco_one_Outside_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_one_Outside_mbias3.push_back(Pt_reco_one_Outside_mbias3);
	Pi0InvMassReco_one_Outside_mbias3 = new TH1D("Pi0InvMassReco_one_Outside_mbias3","Pi0InvMassReco_one_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Outside_mbias3.push_back(Pi0InvMassReco_one_Outside_mbias3);
	EMT_InvMass_one_Outside_mbias3 = new TH1D("EMT_InvMass_one_Outside_mbias3","EMT_InvMass_one_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Outside_mbias3.push_back(EMT_InvMass_one_Outside_mbias3);
	Pi0_pt_vs_rap_one_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_one_Outside_mbias3", "Pi0_pt_vs_rap_one_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_one_Outside_mbias3.push_back(Pi0_pt_vs_rap_one_Outside_mbias3);
	Pi0_pt_vs_rap_est_one_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_est_one_Outside_mbias3", "Pi0_pt_vs_rap_est_one_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_one_Outside_mbias3.push_back(Pi0_pt_vs_rap_est_one_Outside_mbias3); 


	// Outside_mbias3 => two
	GammaInvMassReco_two_Outside_mbias3 = new TH1D("GammaInvMassReco_two_Outside_mbias3","GammaInvMassReco_two_Outside_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_two_Outside_mbias3.push_back(GammaInvMassReco_two_Outside_mbias3);
	GammaOpeningAngleReco_two_Outside_mbias3 = new TH1D("GammaOpeningAngleReco_two_Outside_mbias3","GammaOpeningAngleReco_two_Outside_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_two_Outside_mbias3.push_back(GammaOpeningAngleReco_two_Outside_mbias3);
	Pdg_two_Outside_mbias3 = new TH1D("Pdg_two_Outside_mbias3","Pdg_two_Outside_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_two_Outside_mbias3.push_back(Pdg_two_Outside_mbias3);
	P_reco_two_Outside_mbias3 = new TH1D("P_reco_two_Outside_mbias3","P_reco_two_Outside_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_two_Outside_mbias3.push_back(P_reco_two_Outside_mbias3);
	Pt_reco_two_Outside_mbias3 = new TH1D("Pt_reco_two_Outside_mbias3","Pt_reco_two_Outside_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_two_Outside_mbias3.push_back(Pt_reco_two_Outside_mbias3);
	Pi0InvMassReco_two_Outside_mbias3 = new TH1D("Pi0InvMassReco_two_Outside_mbias3","Pi0InvMassReco_two_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Outside_mbias3.push_back(Pi0InvMassReco_two_Outside_mbias3);
	EMT_InvMass_two_Outside_mbias3 = new TH1D("EMT_InvMass_two_Outside_mbias3","EMT_InvMass_two_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Outside_mbias3.push_back(EMT_InvMass_two_Outside_mbias3);
	Pi0_pt_vs_rap_two_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_two_Outside_mbias3", "Pi0_pt_vs_rap_two_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_two_Outside_mbias3.push_back(Pi0_pt_vs_rap_two_Outside_mbias3);
	Pi0_pt_vs_rap_est_two_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_est_two_Outside_mbias3", "Pi0_pt_vs_rap_est_two_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_two_Outside_mbias3.push_back(Pi0_pt_vs_rap_est_two_Outside_mbias3); 


	// Outside_mbias3 => onetwo
	GammaInvMassReco_onetwo_Outside_mbias3 = new TH1D("GammaInvMassReco_onetwo_Outside_mbias3","GammaInvMassReco_onetwo_Outside_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_onetwo_Outside_mbias3.push_back(GammaInvMassReco_onetwo_Outside_mbias3);
	GammaOpeningAngleReco_onetwo_Outside_mbias3 = new TH1D("GammaOpeningAngleReco_onetwo_Outside_mbias3","GammaOpeningAngleReco_onetwo_Outside_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_onetwo_Outside_mbias3.push_back(GammaOpeningAngleReco_onetwo_Outside_mbias3);
	Pdg_onetwo_Outside_mbias3 = new TH1D("Pdg_onetwo_Outside_mbias3","Pdg_onetwo_Outside_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_onetwo_Outside_mbias3.push_back(Pdg_onetwo_Outside_mbias3);
	P_reco_onetwo_Outside_mbias3 = new TH1D("P_reco_onetwo_Outside_mbias3","P_reco_onetwo_Outside_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_onetwo_Outside_mbias3.push_back(P_reco_onetwo_Outside_mbias3);
	Pt_reco_onetwo_Outside_mbias3 = new TH1D("Pt_reco_onetwo_Outside_mbias3","Pt_reco_onetwo_Outside_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_onetwo_Outside_mbias3.push_back(Pt_reco_onetwo_Outside_mbias3);
	Pi0InvMassReco_onetwo_Outside_mbias3 = new TH1D("Pi0InvMassReco_onetwo_Outside_mbias3","Pi0InvMassReco_onetwo_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Outside_mbias3.push_back(Pi0InvMassReco_onetwo_Outside_mbias3);
	EMT_InvMass_onetwo_Outside_mbias3 = new TH1D("EMT_InvMass_onetwo_Outside_mbias3","EMT_InvMass_onetwo_Outside_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Outside_mbias3.push_back(EMT_InvMass_onetwo_Outside_mbias3);
	Pi0_pt_vs_rap_onetwo_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_onetwo_Outside_mbias3", "Pi0_pt_vs_rap_onetwo_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_onetwo_Outside_mbias3.push_back(Pi0_pt_vs_rap_onetwo_Outside_mbias3);
	Pi0_pt_vs_rap_est_onetwo_Outside_mbias3 = new TH2D("Pi0_pt_vs_rap_est_onetwo_Outside_mbias3", "Pi0_pt_vs_rap_est_onetwo_Outside_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_onetwo_Outside_mbias3.push_back(Pi0_pt_vs_rap_est_onetwo_Outside_mbias3); 





	






	// Both_mbias3 => all
	GammaInvMassReco_all_Both_mbias3 = new TH1D("GammaInvMassReco_all_Both_mbias3","GammaInvMassReco_all_Both_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_all_Both_mbias3.push_back(GammaInvMassReco_all_Both_mbias3);
	GammaOpeningAngleReco_all_Both_mbias3 = new TH1D("GammaOpeningAngleReco_all_Both_mbias3","GammaOpeningAngleReco_all_Both_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_all_Both_mbias3.push_back(GammaOpeningAngleReco_all_Both_mbias3);
	Pdg_all_Both_mbias3 = new TH1D("Pdg_all_Both_mbias3","Pdg_all_Both_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_all_Both_mbias3.push_back(Pdg_all_Both_mbias3);
	P_reco_all_Both_mbias3 = new TH1D("P_reco_all_Both_mbias3","P_reco_all_Both_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_all_Both_mbias3.push_back(P_reco_all_Both_mbias3);
	Pt_reco_all_Both_mbias3 = new TH1D("Pt_reco_all_Both_mbias3","Pt_reco_all_Both_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_all_Both_mbias3.push_back(Pt_reco_all_Both_mbias3);
	Pi0InvMassReco_all_Both_mbias3 = new TH1D("Pi0InvMassReco_all_Both_mbias3","Pi0InvMassReco_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Both_mbias3.push_back(Pi0InvMassReco_all_Both_mbias3);
	EMT_InvMass_all_Both_mbias3 = new TH1D("EMT_InvMass_all_Both_mbias3","EMT_InvMass_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_all_Both_mbias3.push_back(EMT_InvMass_all_Both_mbias3);
	Pi0_pt_vs_rap_all_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_all_Both_mbias3", "Pi0_pt_vs_rap_all_Both_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_all_Both_mbias3.push_back(Pi0_pt_vs_rap_all_Both_mbias3);
	Pi0_pt_vs_rap_est_all_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_est_all_Both_mbias3", "Pi0_pt_vs_rap_est_all_Both_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_all_Both_mbias3.push_back(Pi0_pt_vs_rap_est_all_Both_mbias3); 


	// Both_mbias3 => zero
	GammaInvMassReco_zero_Both_mbias3 = new TH1D("GammaInvMassReco_zero_Both_mbias3","GammaInvMassReco_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_zero_Both_mbias3.push_back(GammaInvMassReco_zero_Both_mbias3);
	GammaOpeningAngleReco_zero_Both_mbias3 = new TH1D("GammaOpeningAngleReco_zero_Both_mbias3","GammaOpeningAngleReco_zero_Both_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_zero_Both_mbias3.push_back(GammaOpeningAngleReco_zero_Both_mbias3);
	Pdg_zero_Both_mbias3 = new TH1D("Pdg_zero_Both_mbias3","Pdg_zero_Both_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_zero_Both_mbias3.push_back(Pdg_zero_Both_mbias3);
	P_reco_zero_Both_mbias3 = new TH1D("P_reco_zero_Both_mbias3","P_reco_zero_Both_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_zero_Both_mbias3.push_back(P_reco_zero_Both_mbias3);
	Pt_reco_zero_Both_mbias3 = new TH1D("Pt_reco_zero_Both_mbias3","Pt_reco_zero_Both_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_zero_Both_mbias3.push_back(Pt_reco_zero_Both_mbias3);
	Pi0InvMassReco_zero_Both_mbias3 = new TH1D("Pi0InvMassReco_zero_Both_mbias3","Pi0InvMassReco_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Both_mbias3.push_back(Pi0InvMassReco_zero_Both_mbias3);
	EMT_InvMass_zero_Both_mbias3 = new TH1D("EMT_InvMass_zero_Both_mbias3","EMT_InvMass_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_zero_Both_mbias3.push_back(EMT_InvMass_zero_Both_mbias3);
	Pi0_pt_vs_rap_zero_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_zero_Both_mbias3", "Pi0_pt_vs_rap_zero_Both_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_zero_Both_mbias3.push_back(Pi0_pt_vs_rap_zero_Both_mbias3);
	Pi0_pt_vs_rap_est_zero_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_est_zero_Both_mbias3", "Pi0_pt_vs_rap_est_zero_Both_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_zero_Both_mbias3.push_back(Pi0_pt_vs_rap_est_zero_Both_mbias3); 


	// Both_mbias3 => one
	GammaInvMassReco_one_Both_mbias3 = new TH1D("GammaInvMassReco_one_Both_mbias3","GammaInvMassReco_one_Both_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_one_Both_mbias3.push_back(GammaInvMassReco_one_Both_mbias3);
	GammaOpeningAngleReco_one_Both_mbias3 = new TH1D("GammaOpeningAngleReco_one_Both_mbias3","GammaOpeningAngleReco_one_Both_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_one_Both_mbias3.push_back(GammaOpeningAngleReco_one_Both_mbias3);
	Pdg_one_Both_mbias3 = new TH1D("Pdg_one_Both_mbias3","Pdg_one_Both_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_one_Both_mbias3.push_back(Pdg_one_Both_mbias3);
	P_reco_one_Both_mbias3 = new TH1D("P_reco_one_Both_mbias3","P_reco_one_Both_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_one_Both_mbias3.push_back(P_reco_one_Both_mbias3);
	Pt_reco_one_Both_mbias3 = new TH1D("Pt_reco_one_Both_mbias3","Pt_reco_one_Both_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_one_Both_mbias3.push_back(Pt_reco_one_Both_mbias3);
	Pi0InvMassReco_one_Both_mbias3 = new TH1D("Pi0InvMassReco_one_Both_mbias3","Pi0InvMassReco_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Both_mbias3.push_back(Pi0InvMassReco_one_Both_mbias3);
	EMT_InvMass_one_Both_mbias3 = new TH1D("EMT_InvMass_one_Both_mbias3","EMT_InvMass_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_one_Both_mbias3.push_back(EMT_InvMass_one_Both_mbias3);
	Pi0_pt_vs_rap_one_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_one_Both_mbias3", "Pi0_pt_vs_rap_one_Both_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_one_Both_mbias3.push_back(Pi0_pt_vs_rap_one_Both_mbias3);
	Pi0_pt_vs_rap_est_one_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_est_one_Both_mbias3", "Pi0_pt_vs_rap_est_one_Both_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_one_Both_mbias3.push_back(Pi0_pt_vs_rap_est_one_Both_mbias3); 


	// Both_mbias3 => two
	GammaInvMassReco_two_Both_mbias3 = new TH1D("GammaInvMassReco_two_Both_mbias3","GammaInvMassReco_two_Both_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_two_Both_mbias3.push_back(GammaInvMassReco_two_Both_mbias3);
	GammaOpeningAngleReco_two_Both_mbias3 = new TH1D("GammaOpeningAngleReco_two_Both_mbias3","GammaOpeningAngleReco_two_Both_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_two_Both_mbias3.push_back(GammaOpeningAngleReco_two_Both_mbias3);
	Pdg_two_Both_mbias3 = new TH1D("Pdg_two_Both_mbias3","Pdg_two_Both_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_two_Both_mbias3.push_back(Pdg_two_Both_mbias3);
	P_reco_two_Both_mbias3 = new TH1D("P_reco_two_Both_mbias3","P_reco_two_Both_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_two_Both_mbias3.push_back(P_reco_two_Both_mbias3);
	Pt_reco_two_Both_mbias3 = new TH1D("Pt_reco_two_Both_mbias3","Pt_reco_two_Both_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_two_Both_mbias3.push_back(Pt_reco_two_Both_mbias3);
	Pi0InvMassReco_two_Both_mbias3 = new TH1D("Pi0InvMassReco_two_Both_mbias3","Pi0InvMassReco_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Both_mbias3.push_back(Pi0InvMassReco_two_Both_mbias3);
	EMT_InvMass_two_Both_mbias3 = new TH1D("EMT_InvMass_two_Both_mbias3","EMT_InvMass_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_two_Both_mbias3.push_back(EMT_InvMass_two_Both_mbias3);
	Pi0_pt_vs_rap_two_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_two_Both_mbias3", "Pi0_pt_vs_rap_two_Both_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_two_Both_mbias3.push_back(Pi0_pt_vs_rap_two_Both_mbias3);
	Pi0_pt_vs_rap_est_two_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_est_two_Both_mbias3", "Pi0_pt_vs_rap_est_two_Both_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_two_Both_mbias3.push_back(Pi0_pt_vs_rap_est_two_Both_mbias3); 


	// Both_mbias3 => onetwo
	GammaInvMassReco_onetwo_Both_mbias3 = new TH1D("GammaInvMassReco_onetwo_Both_mbias3","GammaInvMassReco_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
	fHistoList_man_onetwo_Both_mbias3.push_back(GammaInvMassReco_onetwo_Both_mbias3);
	GammaOpeningAngleReco_onetwo_Both_mbias3 = new TH1D("GammaOpeningAngleReco_onetwo_Both_mbias3","GammaOpeningAngleReco_onetwo_Both_mbias3; angle [deg];#", 200, -0.1, 19.9);
	fHistoList_man_onetwo_Both_mbias3.push_back(GammaOpeningAngleReco_onetwo_Both_mbias3);
	Pdg_onetwo_Both_mbias3 = new TH1D("Pdg_onetwo_Both_mbias3","Pdg_onetwo_Both_mbias3; Id;#", 800, 0, 400);
	fHistoList_man_onetwo_Both_mbias3.push_back(Pdg_onetwo_Both_mbias3);
	P_reco_onetwo_Both_mbias3 = new TH1D("P_reco_onetwo_Both_mbias3","P_reco_onetwo_Both_mbias3; P in GeV/c^{2};#", 600, 0, 6);
	fHistoList_man_onetwo_Both_mbias3.push_back(P_reco_onetwo_Both_mbias3);
	Pt_reco_onetwo_Both_mbias3 = new TH1D("Pt_reco_onetwo_Both_mbias3","Pt_reco_onetwo_Both_mbias3; P_{t} in GeV/c^{2};#", 300, 0, 3);
	fHistoList_man_onetwo_Both_mbias3.push_back(Pt_reco_onetwo_Both_mbias3);
	Pi0InvMassReco_onetwo_Both_mbias3 = new TH1D("Pi0InvMassReco_onetwo_Both_mbias3","Pi0InvMassReco_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Both_mbias3.push_back(Pi0InvMassReco_onetwo_Both_mbias3);
	EMT_InvMass_onetwo_Both_mbias3 = new TH1D("EMT_InvMass_onetwo_Both_mbias3","EMT_InvMass_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_man_onetwo_Both_mbias3.push_back(EMT_InvMass_onetwo_Both_mbias3);
	Pi0_pt_vs_rap_onetwo_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_onetwo_Both_mbias3", "Pi0_pt_vs_rap_onetwo_Both_mbias3; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);  
	fHistoList_man_onetwo_Both_mbias3.push_back(Pi0_pt_vs_rap_onetwo_Both_mbias3);
	Pi0_pt_vs_rap_est_onetwo_Both_mbias3 = new TH2D("Pi0_pt_vs_rap_est_onetwo_Both_mbias3", "Pi0_pt_vs_rap_est_onetwo_Both_mbias3; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
	fHistoList_man_onetwo_Both_mbias3.push_back(Pi0_pt_vs_rap_est_onetwo_Both_mbias3); 





	// Both_mbias3
	Pdg_vs_Distance_mbias3 = new TH2D("Pdg_vs_Distance_mbias3","Pdg_vs_Distance_mbias3; pdg; distance in cm", 2500, 0, 2499, 500, 0, 50);
	fHistoList_man_Both_mbias3.push_back(Pdg_vs_Distance_mbias3);
	P_vs_Distance_mbias3 = new TH2D("P_vs_Distance_mbias3","Distance between projected track and center of the ring (for e+ and e-); P in GeV/c^{2}; distance in cm", 300, 0, 3, 300, 0, 15);
	fHistoList_man_Both_mbias3.push_back(P_vs_Distance_mbias3);





	// Multiplicity Target_mbias3
	MultiplicityGamma_all_Target_mbias3 = new TH2D("MultiplicityGamma_all_Target_mbias3","MultiplicityGamma_all_Target_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityGamma_all_Target_mbias3);
	MultiplicityGamma_zero_Target_mbias3 = new TH2D("MultiplicityGamma_zero_Target_mbias3","MultiplicityGamma_zero_Target_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityGamma_zero_Target_mbias3);
	MultiplicityGamma_one_Target_mbias3 = new TH2D("MultiplicityGamma_one_Target_mbias3","MultiplicityGamma_one_Target_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityGamma_one_Target_mbias3);
	MultiplicityGamma_two_Target_mbias3 = new TH2D("MultiplicityGamma_two_Target_mbias3","MultiplicityGamma_two_Target_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityGamma_two_Target_mbias3);
	MultiplicityGamma_onetwo_Target_mbias3 = new TH2D("MultiplicityGamma_onetwo_Target_mbias3","MultiplicityGamma_onetwo_Target_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityGamma_onetwo_Target_mbias3);

	MultiplicityChargedParticles_all_Target_mbias3 = new TH2D("MultiplicityChargedParticles_all_Target_mbias3","MultiplicityChargedParticles_all_Target_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityChargedParticles_all_Target_mbias3);
	MultiplicityChargedParticles_zero_Target_mbias3 = new TH2D("MultiplicityChargedParticles_zero_Target_mbias3","MultiplicityChargedParticles_zero_Target_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityChargedParticles_zero_Target_mbias3);
	MultiplicityChargedParticles_one_Target_mbias3 = new TH2D("MultiplicityChargedParticles_one_Target_mbias3","MultiplicityChargedParticles_one_Target_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityChargedParticles_one_Target_mbias3);
	MultiplicityChargedParticles_two_Target_mbias3 = new TH2D("MultiplicityChargedParticles_two_Target_mbias3","MultiplicityChargedParticles_two_Target_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityChargedParticles_two_Target_mbias3);
	MultiplicityChargedParticles_onetwo_Target_mbias3 = new TH2D("MultiplicityChargedParticles_onetwo_Target_mbias3","MultiplicityChargedParticles_onetwo_Target_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Target_mbias3.push_back(MultiplicityChargedParticles_onetwo_Target_mbias3);

	// Multiplicity Outside_mbias3
	MultiplicityGamma_all_Outside_mbias3 = new TH2D("MultiplicityGamma_all_Outside_mbias3","MultiplicityGamma_all_Outside_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityGamma_all_Outside_mbias3);
	MultiplicityGamma_zero_Outside_mbias3 = new TH2D("MultiplicityGamma_zero_Outside_mbias3","MultiplicityGamma_zero_Outside_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityGamma_zero_Outside_mbias3);
	MultiplicityGamma_one_Outside_mbias3 = new TH2D("MultiplicityGamma_one_Outside_mbias3","MultiplicityGamma_one_Outside_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityGamma_one_Outside_mbias3);
	MultiplicityGamma_two_Outside_mbias3 = new TH2D("MultiplicityGamma_two_Outside_mbias3","MultiplicityGamma_two_Outside_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityGamma_two_Outside_mbias3);
	MultiplicityGamma_onetwo_Outside_mbias3 = new TH2D("MultiplicityGamma_onetwo_Outside_mbias3","MultiplicityGamma_onetwo_Outside_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityGamma_onetwo_Outside_mbias3);

	MultiplicityChargedParticles_all_Outside_mbias3 = new TH2D("MultiplicityChargedParticles_all_Outside_mbias3","MultiplicityChargedParticles_all_Outside_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityChargedParticles_all_Outside_mbias3);
	MultiplicityChargedParticles_zero_Outside_mbias3 = new TH2D("MultiplicityChargedParticles_zero_Outside_mbias3","MultiplicityChargedParticles_zero_Outside_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityChargedParticles_zero_Outside_mbias3);
	MultiplicityChargedParticles_one_Outside_mbias3 = new TH2D("MultiplicityChargedParticles_one_Outside_mbias3","MultiplicityChargedParticles_one_Outside_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityChargedParticles_one_Outside_mbias3);
	MultiplicityChargedParticles_two_Outside_mbias3 = new TH2D("MultiplicityChargedParticles_two_Outside_mbias3","MultiplicityChargedParticles_two_Outside_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityChargedParticles_two_Outside_mbias3);
	MultiplicityChargedParticles_onetwo_Outside_mbias3 = new TH2D("MultiplicityChargedParticles_onetwo_Outside_mbias3","MultiplicityChargedParticles_onetwo_Outside_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Outside_mbias3.push_back(MultiplicityChargedParticles_onetwo_Outside_mbias3);


	// Multiplicity Both_mbias3
	MultiplicityGamma_all_Both_mbias3 = new TH2D("MultiplicityGamma_all_Both_mbias3","MultiplicityGamma_all_Both_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityGamma_all_Both_mbias3);
	MultiplicityGamma_zero_Both_mbias3 = new TH2D("MultiplicityGamma_zero_Both_mbias3","MultiplicityGamma_zero_Both_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityGamma_zero_Both_mbias3);
	MultiplicityGamma_one_Both_mbias3 = new TH2D("MultiplicityGamma_one_Both_mbias3","MultiplicityGamma_one_Both_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityGamma_one_Both_mbias3);
	MultiplicityGamma_two_Both_mbias3 = new TH2D("MultiplicityGamma_two_Both_mbias3","MultiplicityGamma_two_Both_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityGamma_two_Both_mbias3);
	MultiplicityGamma_onetwo_Both_mbias3 = new TH2D("MultiplicityGamma_onetwo_Both_mbias3","MultiplicityGamma_onetwo_Both_mbias3; Nof gammas in event; invariant mass in GeV/c^{2};#", 400, 0, 30, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityGamma_onetwo_Both_mbias3);

	MultiplicityChargedParticles_all_Both_mbias3 = new TH2D("MultiplicityChargedParticles_all_Both_mbias3","MultiplicityChargedParticles_all_Both_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityChargedParticles_all_Both_mbias3);
	MultiplicityChargedParticles_zero_Both_mbias3 = new TH2D("MultiplicityChargedParticles_zero_Both_mbias3","MultiplicityChargedParticles_zero_Both_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityChargedParticles_zero_Both_mbias3);
	MultiplicityChargedParticles_one_Both_mbias3 = new TH2D("MultiplicityChargedParticles_one_Both_mbias3","MultiplicityChargedParticles_one_Both_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityChargedParticles_one_Both_mbias3);
	MultiplicityChargedParticles_two_Both_mbias3 = new TH2D("MultiplicityChargedParticles_two_Both_mbias3","MultiplicityChargedParticles_two_Both_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityChargedParticles_two_Both_mbias3);
	MultiplicityChargedParticles_onetwo_Both_mbias3 = new TH2D("MultiplicityChargedParticles_onetwo_Both_mbias3","MultiplicityChargedParticles_onetwo_Both_mbias3; Nof charged particles in event; invariant mass in GeV/c^{2};#", 1000, 0, 1000, 1000, 0, 2.0);
	fHistoList_multiplicity_man_Both_mbias3.push_back(MultiplicityChargedParticles_onetwo_Both_mbias3);





	//   rap_vs_Pt for "OneTwo" and "Both_mbias3"
	fHistoList_rap_vs_pt_InM_mbias3.push_back(Pi0_pt_vs_rap_est_onetwo_Both_mbias3);
	rap_vs_Pt_InM_1_mbias3  = new TH1D("rap_vs_Pt_InM_1_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_1_mbias3);
	rap_vs_Pt_InM_2_mbias3  = new TH1D("rap_vs_Pt_InM_2_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_2_mbias3);
	rap_vs_Pt_InM_3_mbias3  = new TH1D("rap_vs_Pt_InM_3_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_3_mbias3);
	rap_vs_Pt_InM_4_mbias3  = new TH1D("rap_vs_Pt_InM_4_mbias3","rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_4_mbias3);
	rap_vs_Pt_InM_5_mbias3  = new TH1D("rap_vs_Pt_InM_5_mbias3","rapidity = (1.2-1.6)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_5_mbias3);
	rap_vs_Pt_InM_6_mbias3  = new TH1D("rap_vs_Pt_InM_6_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_6_mbias3);
	rap_vs_Pt_InM_7_mbias3  = new TH1D("rap_vs_Pt_InM_7_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_7_mbias3);
	rap_vs_Pt_InM_8_mbias3  = new TH1D("rap_vs_Pt_InM_8_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_8_mbias3);
	rap_vs_Pt_InM_9_mbias3  = new TH1D("rap_vs_Pt_InM_9_mbias3","rapidity = (1.6-2.0)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_9_mbias3);
	rap_vs_Pt_InM_10_mbias3  = new TH1D("rap_vs_Pt_InM_10_mbias3","rapidity = (1.6-2.0)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_10_mbias3);
	rap_vs_Pt_InM_11_mbias3  = new TH1D("rap_vs_Pt_InM_11_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_11_mbias3);
	rap_vs_Pt_InM_12_mbias3  = new TH1D("rap_vs_Pt_InM_12_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_12_mbias3);
	rap_vs_Pt_InM_13_mbias3  = new TH1D("rap_vs_Pt_InM_13_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_13_mbias3);
	rap_vs_Pt_InM_14_mbias3 = new TH1D("rap_vs_Pt_InM_14_mbias3","rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_14_mbias3);
	rap_vs_Pt_InM_15_mbias3 = new TH1D("rap_vs_Pt_InM_15_mbias3","rapidity = (2.0-2.4)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_15_mbias3);
	rap_vs_Pt_InM_16_mbias3 = new TH1D("rap_vs_Pt_InM_16_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_16_mbias3);
	rap_vs_Pt_InM_17_mbias3 = new TH1D("rap_vs_Pt_InM_17_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_17_mbias3);
	rap_vs_Pt_InM_18_mbias3 = new TH1D("rap_vs_Pt_InM_18_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_18_mbias3);
	rap_vs_Pt_InM_19_mbias3 = new TH1D("rap_vs_Pt_InM_19_mbias3","rapidity = (2.4-2.8)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_19_mbias3);
	rap_vs_Pt_InM_20_mbias3 = new TH1D("rap_vs_Pt_InM_20_mbias3","rapidity = (2.4-2.8)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_20_mbias3);
	rap_vs_Pt_InM_21_mbias3 = new TH1D("rap_vs_Pt_InM_21_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_21_mbias3);
	rap_vs_Pt_InM_22_mbias3 = new TH1D("rap_vs_Pt_InM_22_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_22_mbias3);
	rap_vs_Pt_InM_23_mbias3 = new TH1D("rap_vs_Pt_InM_23_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_23_mbias3);
	rap_vs_Pt_InM_24_mbias3 = new TH1D("rap_vs_Pt_InM_24_mbias3","rapidity = (2.8-3.2)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_24_mbias3);
	rap_vs_Pt_InM_25_mbias3 = new TH1D("rap_vs_Pt_InM_25_mbias3","rapidity = (2.8-3.2)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_25_mbias3);
	rap_vs_Pt_InM_26_mbias3 = new TH1D("rap_vs_Pt_InM_26_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_26_mbias3);
	rap_vs_Pt_InM_27_mbias3 = new TH1D("rap_vs_Pt_InM_27_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_27_mbias3);
	rap_vs_Pt_InM_28_mbias3 = new TH1D("rap_vs_Pt_InM_28_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_28_mbias3);
	rap_vs_Pt_InM_29_mbias3 = new TH1D("rap_vs_Pt_InM_29_mbias3","rapidity = (3.2-3.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_29_mbias3);
	rap_vs_Pt_InM_30_mbias3 = new TH1D("rap_vs_Pt_InM_30_mbias3","rapidity = (3.2-3.6)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_30_mbias3);

	rap_vs_Pt_InM_mixing_1_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_1_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_1_mbias3);
	rap_vs_Pt_InM_mixing_2_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_2_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_2_mbias3);
	rap_vs_Pt_InM_mixing_3_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_3_mbias3","rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_3_mbias3);
	rap_vs_Pt_InM_mixing_4_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_4_mbias3","rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_4_mbias3);
	rap_vs_Pt_InM_mixing_5_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_5_mbias3","rapidity = (1.2-1.6)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_5_mbias3);
	rap_vs_Pt_InM_mixing_6_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_6_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_6_mbias3);
	rap_vs_Pt_InM_mixing_7_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_7_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_7_mbias3);
	rap_vs_Pt_InM_mixing_8_mbias3  = new TH1D("rap_vs_Pt_InM_mixing_8_mbias3","rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_8_mbias3);
	rap_vs_Pt_InM_mixing_9_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_9_mbias3","rapidity = (1.6-2.0)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_9_mbias3);
	rap_vs_Pt_InM_mixing_10_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_10_mbias3","rapidity = (1.6-2.0)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_10_mbias3);
	rap_vs_Pt_InM_mixing_11_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_11_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_11_mbias3);
	rap_vs_Pt_InM_mixing_12_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_12_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_12_mbias3);
	rap_vs_Pt_InM_mixing_13_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_13_mbias3","rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_13_mbias3);
	rap_vs_Pt_InM_mixing_14_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_14_mbias3","rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_14_mbias3);
	rap_vs_Pt_InM_mixing_15_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_15_mbias3","rapidity = (2.0-2.4)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_15_mbias3);
	rap_vs_Pt_InM_mixing_16_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_16_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_16_mbias3);
	rap_vs_Pt_InM_mixing_17_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_17_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_17_mbias3);
	rap_vs_Pt_InM_mixing_18_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_18_mbias3","rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_18_mbias3);
	rap_vs_Pt_InM_mixing_19_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_19_mbias3","rapidity = (2.4-2.8)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_19_mbias3);
	rap_vs_Pt_InM_mixing_20_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_20_mbias3","rapidity = (2.4-2.8)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_20_mbias3);
	rap_vs_Pt_InM_mixing_21_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_21_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_21_mbias3);
	rap_vs_Pt_InM_mixing_22_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_22_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_22_mbias3);
	rap_vs_Pt_InM_mixing_23_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_23_mbias3","rapidity = (2.8-3.2)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_23_mbias3);
	rap_vs_Pt_InM_mixing_24_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_24_mbias3","rapidity = (2.8-3.2)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_24_mbias3);
	rap_vs_Pt_InM_mixing_25_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_25_mbias3","rapidity = (2.8-3.2)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_25_mbias3);
	rap_vs_Pt_InM_mixing_26_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_26_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_26_mbias3);
	rap_vs_Pt_InM_mixing_27_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_27_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_27_mbias3);
	rap_vs_Pt_InM_mixing_28_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_28_mbias3","rapidity = (3.2-3.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_28_mbias3);
	rap_vs_Pt_InM_mixing_29_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_29_mbias3","rapidity = (3.2-3.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_29_mbias3);
	rap_vs_Pt_InM_mixing_30_mbias3 = new TH1D("rap_vs_Pt_InM_mixing_30_mbias3","rapidity = (3.2-3.6)      P_{t} = (1.6-2.0 GeV/c^{2}) ;invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_rap_vs_pt_InM_mbias3.push_back(rap_vs_Pt_InM_mixing_30_mbias3);



	// BG cases
	//Both_mbias3 all
	BG1_InM_all_Both_mbias3 = new TH1D("BG1_InM_all_Both_mbias3","BG1_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG1_InM_all_Both_mbias3);
	BG2_InM_all_Both_mbias3 = new TH1D("BG2_InM_all_Both_mbias3","BG2_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG2_InM_all_Both_mbias3);
	BG3_InM_all_Both_mbias3 = new TH1D("BG3_InM_all_Both_mbias3","BG3_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG3_InM_all_Both_mbias3);
	BG4_InM_all_Both_mbias3 = new TH1D("BG4_InM_all_Both_mbias3","BG4_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG4_InM_all_Both_mbias3);
	BG5_InM_all_Both_mbias3 = new TH1D("BG5_InM_all_Both_mbias3","BG5_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG5_InM_all_Both_mbias3);
	BG6_InM_all_Both_mbias3 = new TH1D("BG6_InM_all_Both_mbias3","BG6_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG6_InM_all_Both_mbias3);
	BG7_InM_all_Both_mbias3 = new TH1D("BG7_InM_all_Both_mbias3","BG7_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG7_InM_all_Both_mbias3);
	BG8_InM_all_Both_mbias3 = new TH1D("BG8_InM_all_Both_mbias3","BG8_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG8_InM_all_Both_mbias3);
	BG9_InM_all_Both_mbias3 = new TH1D("BG9_InM_all_Both_mbias3","BG9_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG9_InM_all_Both_mbias3);
	BG10_InM_all_Both_mbias3 = new TH1D("BG10_InM_all_Both_mbias3","BG10_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(BG10_InM_all_Both_mbias3);
	PdgCase8_InM_all_Both_mbias3 = new TH1D("PdgCase8_InM_all_Both_mbias3","PdgCase8_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(PdgCase8_InM_all_Both_mbias3);
	PdgCase8mothers_InM_all_Both_mbias3 = new TH1D("PdgCase8mothers_InM_all_Both_mbias3","PdgCase8mothers_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(PdgCase8mothers_InM_all_Both_mbias3);
	sameMIDcase8_InM_all_Both_mbias3 = new TH1D("sameMIDcase8_InM_all_Both_mbias3","sameMIDcase8_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0); 
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8_InM_all_Both_mbias3);
	sameGRIDcase8_InM_all_Both_mbias3 = new TH1D("sameGRIDcase8_InM_all_Both_mbias3","sameGRIDcase8_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameGRIDcase8_InM_all_Both_mbias3);
	Case1ZYPos_InM_all_Both_mbias3 = new TH2D("Case1ZYPos_InM_all_Both_mbias3","Case1ZYPos_InM_all_Both_mbias3; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
	fHistoList_bg_InM_all_Both_mbias3.push_back(Case1ZYPos_InM_all_Both_mbias3);
	sameMIDcase8_mothedPDG_InM_all_Both_mbias3 = new TH1D("sameMIDcase8_mothedPDG_InM_all_Both_mbias3","sameMIDcase8_mothedPDG_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8_mothedPDG_InM_all_Both_mbias3);
	PdgCase8NonEComeFromTarget_mbias3_InM_all_Both_mbias3 = new TH1D("PdgCase8NonEComeFromTarget_mbias3_InM_all_Both_mbias3","PdgCase8NonEComeFromTarget_mbias3_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(PdgCase8NonEComeFromTarget_mbias3_InM_all_Both_mbias3);
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_all_Both_mbias3 = new TH1D("PdgCase8NonE_NOT_FromTarget_mbias3_InM_all_Both_mbias3","PdgCase8NonE_NOT_FromTarget_mbias3_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(PdgCase8NonE_NOT_FromTarget_mbias3_InM_all_Both_mbias3);
	PdgCase8motherNonE_InM_all_Both_mbias3 = new TH1D("PdgCase8motherNonE_InM_all_Both_mbias3","PdgCase8motherNonE_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(PdgCase8motherNonE_InM_all_Both_mbias3);
	Case8ElFromDalitz_InM_all_Both_mbias3 = new TH1D("Case8ElFromDalitz_InM_all_Both_mbias3","Case8ElFromDalitz_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(Case8ElFromDalitz_InM_all_Both_mbias3);
	Case8NonElFrom_pn_InM_all_Both_mbias3 = new TH1D("Case8NonElFrom_pn_InM_all_Both_mbias3","Case8NonElFrom_pn_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(Case8NonElFrom_pn_InM_all_Both_mbias3);
	Case8NonElFrom_eta_InM_all_Both_mbias3 = new TH1D("Case8NonElFrom_eta_InM_all_Both_mbias3","Case8NonElFrom_eta_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(Case8NonElFrom_eta_InM_all_Both_mbias3);
	Case8NonElFrom_kaon_InM_all_Both_mbias3 = new TH1D("Case8NonElFrom_kaon_InM_all_Both_mbias3","Case8NonElFrom_kaon_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(Case8NonElFrom_kaon_InM_all_Both_mbias3);
	sameMIDcase8NonEPdg_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEPdg_InM_all_Both_mbias3","sameMIDcase8NonEPdg_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEPdg_InM_all_Both_mbias3);
	sameMIDcase8NonEMotherPdg_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherPdg_InM_all_Both_mbias3","sameMIDcase8NonEMotherPdg_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEMotherPdg_InM_all_Both_mbias3);
	sameMIDcase8NonEMotherIM_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherIM_InM_all_Both_mbias3","sameMIDcase8NonEMotherIM_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEMotherIM_InM_all_Both_mbias3);
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEPdgFromTarget_mbias3_InM_all_Both_mbias3","sameMIDcase8NonEPdgFromTarget_mbias3_InM_all_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEPdgFromTarget_mbias3_InM_all_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3IM_InM_all_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3IM_InM_all_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3IM_InM_all_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3P_InM_all_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3P_InM_all_Both_mbias3; P in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3P_InM_all_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_all_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_all_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_all_Both_mbias3; Pt in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_all_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_all_Both_mbias3);
	//Both_mbias3 zero
	BG1_InM_zero_Both_mbias3 = new TH1D("BG1_InM_zero_Both_mbias3","BG1_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG1_InM_zero_Both_mbias3);
	BG2_InM_zero_Both_mbias3 = new TH1D("BG2_InM_zero_Both_mbias3","BG2_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG2_InM_zero_Both_mbias3);
	BG3_InM_zero_Both_mbias3 = new TH1D("BG3_InM_zero_Both_mbias3","BG3_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG3_InM_zero_Both_mbias3);
	BG4_InM_zero_Both_mbias3 = new TH1D("BG4_InM_zero_Both_mbias3","BG4_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG4_InM_zero_Both_mbias3);
	BG5_InM_zero_Both_mbias3 = new TH1D("BG5_InM_zero_Both_mbias3","BG5_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG5_InM_zero_Both_mbias3);
	BG6_InM_zero_Both_mbias3 = new TH1D("BG6_InM_zero_Both_mbias3","BG6_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG6_InM_zero_Both_mbias3);
	BG7_InM_zero_Both_mbias3 = new TH1D("BG7_InM_zero_Both_mbias3","BG7_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG7_InM_zero_Both_mbias3);
	BG8_InM_zero_Both_mbias3 = new TH1D("BG8_InM_zero_Both_mbias3","BG8_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG8_InM_zero_Both_mbias3);
	BG9_InM_zero_Both_mbias3 = new TH1D("BG9_InM_zero_Both_mbias3","BG9_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG9_InM_zero_Both_mbias3);
	BG10_InM_zero_Both_mbias3 = new TH1D("BG10_InM_zero_Both_mbias3","BG10_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(BG10_InM_zero_Both_mbias3);
	PdgCase8_InM_zero_Both_mbias3 = new TH1D("PdgCase8_InM_zero_Both_mbias3","PdgCase8_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(PdgCase8_InM_zero_Both_mbias3);
	PdgCase8mothers_InM_zero_Both_mbias3 = new TH1D("PdgCase8mothers_InM_zero_Both_mbias3","PdgCase8mothers_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(PdgCase8mothers_InM_zero_Both_mbias3);
	sameMIDcase8_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8_InM_zero_Both_mbias3","sameMIDcase8_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0); 
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8_InM_zero_Both_mbias3);
	sameGRIDcase8_InM_zero_Both_mbias3 = new TH1D("sameGRIDcase8_InM_zero_Both_mbias3","sameGRIDcase8_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameGRIDcase8_InM_zero_Both_mbias3);
	Case1ZYPos_InM_zero_Both_mbias3 = new TH2D("Case1ZYPos_InM_zero_Both_mbias3","Case1ZYPos_InM_zero_Both_mbias3; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(Case1ZYPos_InM_zero_Both_mbias3);
	sameMIDcase8_mothedPDG_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8_mothedPDG_InM_zero_Both_mbias3","sameMIDcase8_mothedPDG_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8_mothedPDG_InM_zero_Both_mbias3);
	PdgCase8NonEComeFromTarget_mbias3_InM_zero_Both_mbias3 = new TH1D("PdgCase8NonEComeFromTarget_mbias3_InM_zero_Both_mbias3","PdgCase8NonEComeFromTarget_mbias3_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(PdgCase8NonEComeFromTarget_mbias3_InM_zero_Both_mbias3);
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_zero_Both_mbias3 = new TH1D("PdgCase8NonE_NOT_FromTarget_mbias3_InM_zero_Both_mbias3","PdgCase8NonE_NOT_FromTarget_mbias3_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(PdgCase8NonE_NOT_FromTarget_mbias3_InM_zero_Both_mbias3);
	PdgCase8motherNonE_InM_zero_Both_mbias3 = new TH1D("PdgCase8motherNonE_InM_zero_Both_mbias3","PdgCase8motherNonE_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(PdgCase8motherNonE_InM_zero_Both_mbias3);
	Case8ElFromDalitz_InM_zero_Both_mbias3 = new TH1D("Case8ElFromDalitz_InM_zero_Both_mbias3","Case8ElFromDalitz_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(Case8ElFromDalitz_InM_zero_Both_mbias3);
	Case8NonElFrom_pn_InM_zero_Both_mbias3 = new TH1D("Case8NonElFrom_pn_InM_zero_Both_mbias3","Case8NonElFrom_pn_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(Case8NonElFrom_pn_InM_zero_Both_mbias3);
	Case8NonElFrom_eta_InM_zero_Both_mbias3 = new TH1D("Case8NonElFrom_eta_InM_zero_Both_mbias3","Case8NonElFrom_eta_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(Case8NonElFrom_eta_InM_zero_Both_mbias3);
	Case8NonElFrom_kaon_InM_zero_Both_mbias3 = new TH1D("Case8NonElFrom_kaon_InM_zero_Both_mbias3","Case8NonElFrom_kaon_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(Case8NonElFrom_kaon_InM_zero_Both_mbias3);
	sameMIDcase8NonEPdg_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEPdg_InM_zero_Both_mbias3","sameMIDcase8NonEPdg_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEPdg_InM_zero_Both_mbias3);
	sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias3","sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias3);
	sameMIDcase8NonEMotherIM_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherIM_InM_zero_Both_mbias3","sameMIDcase8NonEMotherIM_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEMotherIM_InM_zero_Both_mbias3);
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEPdgFromTarget_mbias3_InM_zero_Both_mbias3","sameMIDcase8NonEPdgFromTarget_mbias3_InM_zero_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEPdgFromTarget_mbias3_InM_zero_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3IM_InM_zero_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3IM_InM_zero_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3IM_InM_zero_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3P_InM_zero_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3P_InM_zero_Both_mbias3; P in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3P_InM_zero_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_zero_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_zero_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_zero_Both_mbias3; Pt in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_zero_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_zero_Both_mbias3);
	//Both_mbias3 one
	BG1_InM_one_Both_mbias3 = new TH1D("BG1_InM_one_Both_mbias3","BG1_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG1_InM_one_Both_mbias3);
	BG2_InM_one_Both_mbias3 = new TH1D("BG2_InM_one_Both_mbias3","BG2_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG2_InM_one_Both_mbias3);
	BG3_InM_one_Both_mbias3 = new TH1D("BG3_InM_one_Both_mbias3","BG3_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG3_InM_one_Both_mbias3);
	BG4_InM_one_Both_mbias3 = new TH1D("BG4_InM_one_Both_mbias3","BG4_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG4_InM_one_Both_mbias3);
	BG5_InM_one_Both_mbias3 = new TH1D("BG5_InM_one_Both_mbias3","BG5_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG5_InM_one_Both_mbias3);
	BG6_InM_one_Both_mbias3 = new TH1D("BG6_InM_one_Both_mbias3","BG6_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG6_InM_one_Both_mbias3);
	BG7_InM_one_Both_mbias3 = new TH1D("BG7_InM_one_Both_mbias3","BG7_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG7_InM_one_Both_mbias3);
	BG8_InM_one_Both_mbias3 = new TH1D("BG8_InM_one_Both_mbias3","BG8_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG8_InM_one_Both_mbias3);
	BG9_InM_one_Both_mbias3 = new TH1D("BG9_InM_one_Both_mbias3","BG9_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG9_InM_one_Both_mbias3);
	BG10_InM_one_Both_mbias3 = new TH1D("BG10_InM_one_Both_mbias3","BG10_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(BG10_InM_one_Both_mbias3);
	PdgCase8_InM_one_Both_mbias3 = new TH1D("PdgCase8_InM_one_Both_mbias3","PdgCase8_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(PdgCase8_InM_one_Both_mbias3);
	PdgCase8mothers_InM_one_Both_mbias3 = new TH1D("PdgCase8mothers_InM_one_Both_mbias3","PdgCase8mothers_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(PdgCase8mothers_InM_one_Both_mbias3);
	sameMIDcase8_InM_one_Both_mbias3 = new TH1D("sameMIDcase8_InM_one_Both_mbias3","sameMIDcase8_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0); 
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8_InM_one_Both_mbias3);
	sameGRIDcase8_InM_one_Both_mbias3 = new TH1D("sameGRIDcase8_InM_one_Both_mbias3","sameGRIDcase8_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameGRIDcase8_InM_one_Both_mbias3);
	Case1ZYPos_InM_one_Both_mbias3 = new TH2D("Case1ZYPos_InM_one_Both_mbias3","Case1ZYPos_InM_one_Both_mbias3; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
	fHistoList_bg_InM_one_Both_mbias3.push_back(Case1ZYPos_InM_one_Both_mbias3);
	sameMIDcase8_mothedPDG_InM_one_Both_mbias3 = new TH1D("sameMIDcase8_mothedPDG_InM_one_Both_mbias3","sameMIDcase8_mothedPDG_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8_mothedPDG_InM_one_Both_mbias3);
	PdgCase8NonEComeFromTarget_mbias3_InM_one_Both_mbias3 = new TH1D("PdgCase8NonEComeFromTarget_mbias3_InM_one_Both_mbias3","PdgCase8NonEComeFromTarget_mbias3_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(PdgCase8NonEComeFromTarget_mbias3_InM_one_Both_mbias3);
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_one_Both_mbias3 = new TH1D("PdgCase8NonE_NOT_FromTarget_mbias3_InM_one_Both_mbias3","PdgCase8NonE_NOT_FromTarget_mbias3_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(PdgCase8NonE_NOT_FromTarget_mbias3_InM_one_Both_mbias3);
	PdgCase8motherNonE_InM_one_Both_mbias3 = new TH1D("PdgCase8motherNonE_InM_one_Both_mbias3","PdgCase8motherNonE_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(PdgCase8motherNonE_InM_one_Both_mbias3);
	Case8ElFromDalitz_InM_one_Both_mbias3 = new TH1D("Case8ElFromDalitz_InM_one_Both_mbias3","Case8ElFromDalitz_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(Case8ElFromDalitz_InM_one_Both_mbias3);
	Case8NonElFrom_pn_InM_one_Both_mbias3 = new TH1D("Case8NonElFrom_pn_InM_one_Both_mbias3","Case8NonElFrom_pn_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(Case8NonElFrom_pn_InM_one_Both_mbias3);
	Case8NonElFrom_eta_InM_one_Both_mbias3 = new TH1D("Case8NonElFrom_eta_InM_one_Both_mbias3","Case8NonElFrom_eta_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(Case8NonElFrom_eta_InM_one_Both_mbias3);
	Case8NonElFrom_kaon_InM_one_Both_mbias3 = new TH1D("Case8NonElFrom_kaon_InM_one_Both_mbias3","Case8NonElFrom_kaon_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(Case8NonElFrom_kaon_InM_one_Both_mbias3);
	sameMIDcase8NonEPdg_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEPdg_InM_one_Both_mbias3","sameMIDcase8NonEPdg_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEPdg_InM_one_Both_mbias3);
	sameMIDcase8NonEMotherPdg_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherPdg_InM_one_Both_mbias3","sameMIDcase8NonEMotherPdg_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEMotherPdg_InM_one_Both_mbias3);
	sameMIDcase8NonEMotherIM_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherIM_InM_one_Both_mbias3","sameMIDcase8NonEMotherIM_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEMotherIM_InM_one_Both_mbias3);
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEPdgFromTarget_mbias3_InM_one_Both_mbias3","sameMIDcase8NonEPdgFromTarget_mbias3_InM_one_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEPdgFromTarget_mbias3_InM_one_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3IM_InM_one_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3IM_InM_one_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3IM_InM_one_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3P_InM_one_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3P_InM_one_Both_mbias3; P in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3P_InM_one_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_one_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_one_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_one_Both_mbias3; Pt in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_one_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_one_Both_mbias3);
	//Both_mbias3 two
	BG1_InM_two_Both_mbias3 = new TH1D("BG1_InM_two_Both_mbias3","BG1_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG1_InM_two_Both_mbias3);
	BG2_InM_two_Both_mbias3 = new TH1D("BG2_InM_two_Both_mbias3","BG2_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG2_InM_two_Both_mbias3);
	BG3_InM_two_Both_mbias3 = new TH1D("BG3_InM_two_Both_mbias3","BG3_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG3_InM_two_Both_mbias3);
	BG4_InM_two_Both_mbias3 = new TH1D("BG4_InM_two_Both_mbias3","BG4_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG4_InM_two_Both_mbias3);
	BG5_InM_two_Both_mbias3 = new TH1D("BG5_InM_two_Both_mbias3","BG5_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG5_InM_two_Both_mbias3);
	BG6_InM_two_Both_mbias3 = new TH1D("BG6_InM_two_Both_mbias3","BG6_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG6_InM_two_Both_mbias3);
	BG7_InM_two_Both_mbias3 = new TH1D("BG7_InM_two_Both_mbias3","BG7_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG7_InM_two_Both_mbias3);
	BG8_InM_two_Both_mbias3 = new TH1D("BG8_InM_two_Both_mbias3","BG8_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG8_InM_two_Both_mbias3);
	BG9_InM_two_Both_mbias3 = new TH1D("BG9_InM_two_Both_mbias3","BG9_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG9_InM_two_Both_mbias3);
	BG10_InM_two_Both_mbias3 = new TH1D("BG10_InM_two_Both_mbias3","BG10_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(BG10_InM_two_Both_mbias3);
	PdgCase8_InM_two_Both_mbias3 = new TH1D("PdgCase8_InM_two_Both_mbias3","PdgCase8_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(PdgCase8_InM_two_Both_mbias3);
	PdgCase8mothers_InM_two_Both_mbias3 = new TH1D("PdgCase8mothers_InM_two_Both_mbias3","PdgCase8mothers_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(PdgCase8mothers_InM_two_Both_mbias3);
	sameMIDcase8_InM_two_Both_mbias3 = new TH1D("sameMIDcase8_InM_two_Both_mbias3","sameMIDcase8_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0); 
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8_InM_two_Both_mbias3);
	sameGRIDcase8_InM_two_Both_mbias3 = new TH1D("sameGRIDcase8_InM_two_Both_mbias3","sameGRIDcase8_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameGRIDcase8_InM_two_Both_mbias3);
	Case1ZYPos_InM_two_Both_mbias3 = new TH2D("Case1ZYPos_InM_two_Both_mbias3","Case1ZYPos_InM_two_Both_mbias3; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
	fHistoList_bg_InM_two_Both_mbias3.push_back(Case1ZYPos_InM_two_Both_mbias3);
	sameMIDcase8_mothedPDG_InM_two_Both_mbias3 = new TH1D("sameMIDcase8_mothedPDG_InM_two_Both_mbias3","sameMIDcase8_mothedPDG_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8_mothedPDG_InM_two_Both_mbias3);
	PdgCase8NonEComeFromTarget_mbias3_InM_two_Both_mbias3 = new TH1D("PdgCase8NonEComeFromTarget_mbias3_InM_two_Both_mbias3","PdgCase8NonEComeFromTarget_mbias3_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(PdgCase8NonEComeFromTarget_mbias3_InM_two_Both_mbias3);
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_two_Both_mbias3 = new TH1D("PdgCase8NonE_NOT_FromTarget_mbias3_InM_two_Both_mbias3","PdgCase8NonE_NOT_FromTarget_mbias3_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(PdgCase8NonE_NOT_FromTarget_mbias3_InM_two_Both_mbias3);
	PdgCase8motherNonE_InM_two_Both_mbias3 = new TH1D("PdgCase8motherNonE_InM_two_Both_mbias3","PdgCase8motherNonE_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(PdgCase8motherNonE_InM_two_Both_mbias3);
	Case8ElFromDalitz_InM_two_Both_mbias3 = new TH1D("Case8ElFromDalitz_InM_two_Both_mbias3","Case8ElFromDalitz_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(Case8ElFromDalitz_InM_two_Both_mbias3);
	Case8NonElFrom_pn_InM_two_Both_mbias3 = new TH1D("Case8NonElFrom_pn_InM_two_Both_mbias3","Case8NonElFrom_pn_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(Case8NonElFrom_pn_InM_two_Both_mbias3);
	Case8NonElFrom_eta_InM_two_Both_mbias3 = new TH1D("Case8NonElFrom_eta_InM_two_Both_mbias3","Case8NonElFrom_eta_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(Case8NonElFrom_eta_InM_two_Both_mbias3);
	Case8NonElFrom_kaon_InM_two_Both_mbias3 = new TH1D("Case8NonElFrom_kaon_InM_two_Both_mbias3","Case8NonElFrom_kaon_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(Case8NonElFrom_kaon_InM_two_Both_mbias3);
	sameMIDcase8NonEPdg_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEPdg_InM_two_Both_mbias3","sameMIDcase8NonEPdg_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEPdg_InM_two_Both_mbias3);
	sameMIDcase8NonEMotherPdg_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherPdg_InM_two_Both_mbias3","sameMIDcase8NonEMotherPdg_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEMotherPdg_InM_two_Both_mbias3);
	sameMIDcase8NonEMotherIM_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherIM_InM_two_Both_mbias3","sameMIDcase8NonEMotherIM_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEMotherIM_InM_two_Both_mbias3);
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEPdgFromTarget_mbias3_InM_two_Both_mbias3","sameMIDcase8NonEPdgFromTarget_mbias3_InM_two_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEPdgFromTarget_mbias3_InM_two_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3IM_InM_two_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3IM_InM_two_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3IM_InM_two_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3P_InM_two_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3P_InM_two_Both_mbias3; P in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3P_InM_two_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_two_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_two_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_two_Both_mbias3; Pt in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_two_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_two_Both_mbias3);
	//Both_mbias3 onetwo
	BG1_InM_onetwo_Both_mbias3 = new TH1D("BG1_InM_onetwo_Both_mbias3","BG1_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG1_InM_onetwo_Both_mbias3);
	BG2_InM_onetwo_Both_mbias3 = new TH1D("BG2_InM_onetwo_Both_mbias3","BG2_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG2_InM_onetwo_Both_mbias3);
	BG3_InM_onetwo_Both_mbias3 = new TH1D("BG3_InM_onetwo_Both_mbias3","BG3_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG3_InM_onetwo_Both_mbias3);
	BG4_InM_onetwo_Both_mbias3 = new TH1D("BG4_InM_onetwo_Both_mbias3","BG4_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG4_InM_onetwo_Both_mbias3);
	BG5_InM_onetwo_Both_mbias3 = new TH1D("BG5_InM_onetwo_Both_mbias3","BG5_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG5_InM_onetwo_Both_mbias3);
	BG6_InM_onetwo_Both_mbias3 = new TH1D("BG6_InM_onetwo_Both_mbias3","BG6_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG6_InM_onetwo_Both_mbias3);
	BG7_InM_onetwo_Both_mbias3 = new TH1D("BG7_InM_onetwo_Both_mbias3","BG7_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG7_InM_onetwo_Both_mbias3);
	BG8_InM_onetwo_Both_mbias3 = new TH1D("BG8_InM_onetwo_Both_mbias3","BG8_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG8_InM_onetwo_Both_mbias3);
	BG9_InM_onetwo_Both_mbias3 = new TH1D("BG9_InM_onetwo_Both_mbias3","BG9_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG9_InM_onetwo_Both_mbias3);
	BG10_InM_onetwo_Both_mbias3 = new TH1D("BG10_InM_onetwo_Both_mbias3","BG10_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(BG10_InM_onetwo_Both_mbias3);
	PdgCase8_InM_onetwo_Both_mbias3 = new TH1D("PdgCase8_InM_onetwo_Both_mbias3","PdgCase8_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(PdgCase8_InM_onetwo_Both_mbias3);
	PdgCase8mothers_InM_onetwo_Both_mbias3 = new TH1D("PdgCase8mothers_InM_onetwo_Both_mbias3","PdgCase8mothers_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(PdgCase8mothers_InM_onetwo_Both_mbias3);
	sameMIDcase8_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8_InM_onetwo_Both_mbias3","sameMIDcase8_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0); 
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8_InM_onetwo_Both_mbias3);
	sameGRIDcase8_InM_onetwo_Both_mbias3 = new TH1D("sameGRIDcase8_InM_onetwo_Both_mbias3","sameGRIDcase8_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameGRIDcase8_InM_onetwo_Both_mbias3);
	Case1ZYPos_InM_onetwo_Both_mbias3 = new TH2D("Case1ZYPos_InM_onetwo_Both_mbias3","Case1ZYPos_InM_onetwo_Both_mbias3; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(Case1ZYPos_InM_onetwo_Both_mbias3);
	sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias3","sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias3);
	PdgCase8NonEComeFromTarget_mbias3_InM_onetwo_Both_mbias3 = new TH1D("PdgCase8NonEComeFromTarget_mbias3_InM_onetwo_Both_mbias3","PdgCase8NonEComeFromTarget_mbias3_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(PdgCase8NonEComeFromTarget_mbias3_InM_onetwo_Both_mbias3);
	PdgCase8NonE_NOT_FromTarget_mbias3_InM_onetwo_Both_mbias3 = new TH1D("PdgCase8NonE_NOT_FromTarget_mbias3_InM_onetwo_Both_mbias3","PdgCase8NonE_NOT_FromTarget_mbias3_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(PdgCase8NonE_NOT_FromTarget_mbias3_InM_onetwo_Both_mbias3);
	PdgCase8motherNonE_InM_onetwo_Both_mbias3 = new TH1D("PdgCase8motherNonE_InM_onetwo_Both_mbias3","PdgCase8motherNonE_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(PdgCase8motherNonE_InM_onetwo_Both_mbias3);
	Case8ElFromDalitz_InM_onetwo_Both_mbias3 = new TH1D("Case8ElFromDalitz_InM_onetwo_Both_mbias3","Case8ElFromDalitz_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(Case8ElFromDalitz_InM_onetwo_Both_mbias3);
	Case8NonElFrom_pn_InM_onetwo_Both_mbias3 = new TH1D("Case8NonElFrom_pn_InM_onetwo_Both_mbias3","Case8NonElFrom_pn_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(Case8NonElFrom_pn_InM_onetwo_Both_mbias3);
	Case8NonElFrom_eta_InM_onetwo_Both_mbias3 = new TH1D("Case8NonElFrom_eta_InM_onetwo_Both_mbias3","Case8NonElFrom_eta_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(Case8NonElFrom_eta_InM_onetwo_Both_mbias3);
	Case8NonElFrom_kaon_InM_onetwo_Both_mbias3 = new TH1D("Case8NonElFrom_kaon_InM_onetwo_Both_mbias3","Case8NonElFrom_kaon_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(Case8NonElFrom_kaon_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEPdg_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEPdg_InM_onetwo_Both_mbias3","sameMIDcase8NonEPdg_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEPdg_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias3","sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias3","sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEPdgFromTarget_mbias3_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEPdgFromTarget_mbias3_InM_onetwo_Both_mbias3","sameMIDcase8NonEPdgFromTarget_mbias3_InM_onetwo_Both_mbias3; Id ;#", 5000, -2500, 2500);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEPdgFromTarget_mbias3_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3IM_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3IM_InM_onetwo_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3IM_InM_onetwo_Both_mbias3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3IM_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3P_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3P_InM_onetwo_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3P_InM_onetwo_Both_mbias3; P in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3P_InM_onetwo_Both_mbias3);
	sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_onetwo_Both_mbias3 = new TH1D("sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_onetwo_Both_mbias3","sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_onetwo_Both_mbias3; Pt in GeV/c^{2} ;#", 200, 0, 10);
	fHistoList_bg_InM_onetwo_Both_mbias3.push_back(sameMIDcase8NonEComeFromTarget_mbias3Pt_InM_onetwo_Both_mbias3);


}
