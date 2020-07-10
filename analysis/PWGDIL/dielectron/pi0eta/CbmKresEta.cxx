/**
 *    file CbmKresEta.cxx
 *
 *    author Ievgenii Kres
 *    date 27.03.2018
 *    modified 30.01.2020
 *
 *
 *    Central class for the eta conversion analysis.
 *    The analysis routine is similar to thr pi^0 analysis in file CbmKresConversionManual.cxx, but with some extra features and different cuts specified for eta analysis.
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *
 **/

#include "CbmKresEta.h"
#include "CbmKresFunctions.h"

#include "TDirectory.h"
#include <iostream>

#include "CbmGlobalTrack.h"
#include "CbmKFParticleInterface.h"
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
#include "KFParticle.h"
#include "L1Field.h"


using namespace std;

CbmKresEta::CbmKresEta()
  : fMcTracks(nullptr)
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
  , EMT_eta_gg_Event_Target()
  , EMT_eta_gg_pair_momenta_Target()
  , EMT_eta_gg_NofRings_Target()
  , EMT_eta_gg_Event_Outside()
  , EMT_eta_gg_pair_momenta_Outside()
  , EMT_eta_gg_NofRings_Outside()
  , EMT_eta_gg_Event_Both()
  , EMT_eta_gg_pair_momenta_Both()
  , EMT_eta_gg_NofRings_Both()
  , Gammas_all_Target()
  , Gammas_two_Target()
  , Gammas_onetwo_Target()
  , Gammas_stsIndex_all_Target()
  , Gammas_stsIndex_two_Target()
  , Gammas_stsIndex_onetwo_Target()
  , Gammas_MCIndex_all_Target()
  , Gammas_MCIndex_two_Target()
  , Gammas_MCIndex_onetwo_Target()
  , Gammas_MC_all_Target()
  , Gammas_MC_two_Target()
  , Gammas_MC_onetwo_Target()
  , Gammas_all_Outside()
  , Gammas_two_Outside()
  , Gammas_onetwo_Outside()
  , Gammas_stsIndex_all_Outside()
  , Gammas_stsIndex_two_Outside()
  , Gammas_stsIndex_onetwo_Outside()
  , Gammas_MCIndex_all_Outside()
  , Gammas_MCIndex_two_Outside()
  , Gammas_MCIndex_onetwo_Outside()
  , Gammas_MC_all_Outside()
  , Gammas_MC_two_Outside()
  , Gammas_MC_onetwo_Outside()
  , Gammas_all_Both()
  , Gammas_two_Both()
  , Gammas_onetwo_Both()
  , Gammas_stsIndex_all_Both()
  , Gammas_stsIndex_two_Both()
  , Gammas_stsIndex_onetwo_Both()
  , Gammas_MCIndex_all_Both()
  , Gammas_MCIndex_two_Both()
  , Gammas_MCIndex_onetwo_Both()
  , Gammas_MC_all_Both()
  , Gammas_MC_two_Both()
  , Gammas_MC_onetwo_Both()
  , fHistoList_Eta()
  , Particle_pull_px_Target(nullptr)
  , Particle_pull_py_Target(nullptr)
  , Particle_pull_pz_Target(nullptr)
  , Particle_pull_px_Outside(nullptr)
  , Particle_pull_py_Outside(nullptr)
  , Particle_pull_pz_Outside(nullptr)
  , Particle_pull_X_Outside(nullptr)
  , Particle_pull_Y_Outside(nullptr)
  , Particle_pull_Z_Outside(nullptr)
  , fHistoList_Eta_cuts_Target()
  , fHistoList_Eta_cuts_zeroInRich_Target()
  , GammasInvMass_fromEta_beforeCuts_zeroInRich_Target(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target(nullptr)
  , GammasMomentum_fromEta_beforeCuts_zeroInRich_Target(nullptr)
  , GammasRapidity_fromEta_beforeCuts_zeroInRich_Target(nullptr)
  , PlaneAngles_last_fromEta_zeroInRich_Target(nullptr)
  , PlaneAngles_first_fromEta_zeroInRich_Target(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target(nullptr)
  , PlaneAngles_last_wrongpairs_zeroInRich_Target(nullptr)
  , PlaneAngles_first_wrongpairs_zeroInRich_Target(nullptr)
  , fHistoList_Eta_cuts_oneInRich_Target()
  , GammasInvMass_fromEta_beforeCuts_oneInRich_Target(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target(nullptr)
  , GammasMomentum_fromEta_beforeCuts_oneInRich_Target(nullptr)
  , GammasRapidity_fromEta_beforeCuts_oneInRich_Target(nullptr)
  , PlaneAngles_last_fromEta_oneInRich_Target(nullptr)
  , PlaneAngles_first_fromEta_oneInRich_Target(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target(nullptr)
  , PlaneAngles_last_wrongpairs_oneInRich_Target(nullptr)
  , PlaneAngles_first_wrongpairs_oneInRich_Target(nullptr)
  , fHistoList_Eta_cuts_twoInRich_Target()
  , GammasInvMass_fromEta_beforeCuts_twoInRich_Target(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target(nullptr)
  , GammasMomentum_fromEta_beforeCuts_twoInRich_Target(nullptr)
  , GammasRapidity_fromEta_beforeCuts_twoInRich_Target(nullptr)
  , PlaneAngles_last_fromEta_twoInRich_Target(nullptr)
  , PlaneAngles_first_fromEta_twoInRich_Target(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target(nullptr)
  , PlaneAngles_last_wrongpairs_twoInRich_Target(nullptr)
  , PlaneAngles_first_wrongpairs_twoInRich_Target(nullptr)
  , fHistoList_Eta_cuts_Outside()
  , fHistoList_Eta_cuts_zeroInRich_Outside()
  , GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside(nullptr)
  , PlaneAngles_last_fromEta_zeroInRich_Outside(nullptr)
  , PlaneAngles_first_fromEta_zeroInRich_Outside(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside(nullptr)
  , PlaneAngles_last_wrongpairs_zeroInRich_Outside(nullptr)
  , PlaneAngles_first_wrongpairs_zeroInRich_Outside(nullptr)
  , fHistoList_Eta_cuts_oneInRich_Outside()
  , GammasInvMass_fromEta_beforeCuts_oneInRich_Outside(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside(nullptr)
  , GammasMomentum_fromEta_beforeCuts_oneInRich_Outside(nullptr)
  , GammasRapidity_fromEta_beforeCuts_oneInRich_Outside(nullptr)
  , PlaneAngles_last_fromEta_oneInRich_Outside(nullptr)
  , PlaneAngles_first_fromEta_oneInRich_Outside(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside(nullptr)
  , PlaneAngles_last_wrongpairs_oneInRich_Outside(nullptr)
  , PlaneAngles_first_wrongpairs_oneInRich_Outside(nullptr)
  , fHistoList_Eta_cuts_twoInRich_Outside()
  , GammasInvMass_fromEta_beforeCuts_twoInRich_Outside(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside(nullptr)
  , GammasMomentum_fromEta_beforeCuts_twoInRich_Outside(nullptr)
  , GammasRapidity_fromEta_beforeCuts_twoInRich_Outside(nullptr)
  , PlaneAngles_last_fromEta_twoInRich_Outside(nullptr)
  , PlaneAngles_first_fromEta_twoInRich_Outside(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside(nullptr)
  , PlaneAngles_last_wrongpairs_twoInRich_Outside(nullptr)
  , PlaneAngles_first_wrongpairs_twoInRich_Outside(nullptr)
  , fHistoList_Eta_cuts_Both()
  , fHistoList_Eta_cuts_zeroInRich_Both()
  , GammasInvMass_fromEta_beforeCuts_zeroInRich_Both(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both(nullptr)
  , GammasMomentum_fromEta_beforeCuts_zeroInRich_Both(nullptr)
  , GammasRapidity_fromEta_beforeCuts_zeroInRich_Both(nullptr)
  , PlaneAngles_last_fromEta_zeroInRich_Both(nullptr)
  , PlaneAngles_first_fromEta_zeroInRich_Both(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both(nullptr)
  , PlaneAngles_last_wrongpairs_zeroInRich_Both(nullptr)
  , PlaneAngles_first_wrongpairs_zeroInRich_Both(nullptr)
  , fHistoList_Eta_cuts_oneInRich_Both()
  , GammasInvMass_fromEta_beforeCuts_oneInRich_Both(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both(nullptr)
  , GammasMomentum_fromEta_beforeCuts_oneInRich_Both(nullptr)
  , GammasRapidity_fromEta_beforeCuts_oneInRich_Both(nullptr)
  , PlaneAngles_last_fromEta_oneInRich_Both(nullptr)
  , PlaneAngles_first_fromEta_oneInRich_Both(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both(nullptr)
  , PlaneAngles_last_wrongpairs_oneInRich_Both(nullptr)
  , PlaneAngles_first_wrongpairs_oneInRich_Both(nullptr)
  , PlaneAngles_last_fromEta_afterCuts_oneInRich_Both(nullptr)
  , PlaneAngles_first_fromEta_afterCuts_oneInRich_Both(nullptr)
  , PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both(nullptr)
  , PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both(nullptr)
  , fHistoList_Eta_cuts_twoInRich_Both()
  , GammasInvMass_fromEta_beforeCuts_twoInRich_Both(nullptr)
  , GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both(nullptr)
  , GammasMomentum_fromEta_beforeCuts_twoInRich_Both(nullptr)
  , GammasRapidity_fromEta_beforeCuts_twoInRich_Both(nullptr)
  , PlaneAngles_last_fromEta_twoInRich_Both(nullptr)
  , PlaneAngles_first_fromEta_twoInRich_Both(nullptr)
  , GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both(nullptr)
  , GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both(nullptr)
  , GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both(nullptr)
  , GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both(nullptr)
  , PlaneAngles_last_wrongpairs_twoInRich_Both(nullptr)
  , PlaneAngles_first_wrongpairs_twoInRich_Both(nullptr)
  , fHistoList_Eta_all_Target()
  , AngleBetweenGammas_trueEta_before_cuts_all_Target(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_all_Target(nullptr)
  , InvMass_GammaCombinations_all_Target(nullptr)
  , Particles_PDG_all_Target(nullptr)
  , TrueEta_pt_vs_rap_all_Target(nullptr)
  , WrongEta_pt_vs_rap_all_Target(nullptr)
  , TrueEta_pt_vs_rap_est_all_Target(nullptr)
  , WrongEta_pt_vs_rap_est_all_Target(nullptr)
  , TrueEta_InvMass_after_cuts_all_Target(nullptr)
  , WrongEta_InvMass_after_cuts_all_Target(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_all_Target(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_all_Target(nullptr)
  , EMT_Eta_InvMass_all_Target(nullptr)
  , fHistoList_Eta_onetwo_Target()
  , AngleBetweenGammas_trueEta_before_cuts_onetwo_Target(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target(nullptr)
  , InvMass_GammaCombinations_onetwo_Target(nullptr)
  , Particles_PDG_onetwo_Target(nullptr)
  , TrueEta_pt_vs_rap_onetwo_Target(nullptr)
  , WrongEta_pt_vs_rap_onetwo_Target(nullptr)
  , TrueEta_pt_vs_rap_est_onetwo_Target(nullptr)
  , WrongEta_pt_vs_rap_est_onetwo_Target(nullptr)
  , TrueEta_InvMass_after_cuts_onetwo_Target(nullptr)
  , WrongEta_InvMass_after_cuts_onetwo_Target(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target(nullptr)
  , EMT_Eta_InvMass_onetwo_Target(nullptr)
  , fHistoList_Eta_two_Target()
  , AngleBetweenGammas_trueEta_before_cuts_two_Target(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_two_Target(nullptr)
  , InvMass_GammaCombinations_two_Target(nullptr)
  , Particles_PDG_two_Target(nullptr)
  , TrueEta_pt_vs_rap_two_Target(nullptr)
  , WrongEta_pt_vs_rap_two_Target(nullptr)
  , TrueEta_pt_vs_rap_est_two_Target(nullptr)
  , WrongEta_pt_vs_rap_est_two_Target(nullptr)
  , TrueEta_InvMass_after_cuts_two_Target(nullptr)
  , WrongEta_InvMass_after_cuts_two_Target(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_two_Target(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_two_Target(nullptr)
  , EMT_Eta_InvMass_two_Target(nullptr)
  , fHistoList_rap_pt_Eta_all_Target()
  , fHistoList_rap_pt_Eta_onetwo_Target()
  , fHistoList_rap_pt_Eta_two_Target()
  , fHistoList_Eta_all_Outside()
  , AngleBetweenGammas_trueEta_before_cuts_all_Outside(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_all_Outside(nullptr)
  , InvMass_GammaCombinations_all_Outside(nullptr)
  , Particles_PDG_all_Outside(nullptr)
  , TrueEta_pt_vs_rap_all_Outside(nullptr)
  , WrongEta_pt_vs_rap_all_Outside(nullptr)
  , TrueEta_pt_vs_rap_est_all_Outside(nullptr)
  , WrongEta_pt_vs_rap_est_all_Outside(nullptr)
  , TrueEta_InvMass_after_cuts_all_Outside(nullptr)
  , WrongEta_InvMass_after_cuts_all_Outside(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_all_Outside(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_all_Outside(nullptr)
  , EMT_Eta_InvMass_all_Outside(nullptr)
  , fHistoList_Eta_onetwo_Outside()
  , AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside(nullptr)
  , InvMass_GammaCombinations_onetwo_Outside(nullptr)
  , Particles_PDG_onetwo_Outside(nullptr)
  , TrueEta_pt_vs_rap_onetwo_Outside(nullptr)
  , WrongEta_pt_vs_rap_onetwo_Outside(nullptr)
  , TrueEta_pt_vs_rap_est_onetwo_Outside(nullptr)
  , WrongEta_pt_vs_rap_est_onetwo_Outside(nullptr)
  , TrueEta_InvMass_after_cuts_onetwo_Outside(nullptr)
  , WrongEta_InvMass_after_cuts_onetwo_Outside(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside(nullptr)
  , EMT_Eta_InvMass_onetwo_Outside(nullptr)
  , fHistoList_Eta_two_Outside()
  , AngleBetweenGammas_trueEta_before_cuts_two_Outside(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_two_Outside(nullptr)
  , InvMass_GammaCombinations_two_Outside(nullptr)
  , Particles_PDG_two_Outside(nullptr)
  , TrueEta_pt_vs_rap_two_Outside(nullptr)
  , WrongEta_pt_vs_rap_two_Outside(nullptr)
  , TrueEta_pt_vs_rap_est_two_Outside(nullptr)
  , WrongEta_pt_vs_rap_est_two_Outside(nullptr)
  , TrueEta_InvMass_after_cuts_two_Outside(nullptr)
  , WrongEta_InvMass_after_cuts_two_Outside(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_two_Outside(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_two_Outside(nullptr)
  , EMT_Eta_InvMass_two_Outside(nullptr)
  , fHistoList_rap_pt_Eta_all_Outside()
  , fHistoList_rap_pt_Eta_onetwo_Outside()
  , fHistoList_rap_pt_Eta_two_Outside()
  , fHistoList_Eta_all_Both()
  , AngleBetweenGammas_trueEta_before_cuts_all_Both(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_all_Both(nullptr)
  , InvMass_GammaCombinations_all_Both(nullptr)
  , Particles_PDG_all_Both(nullptr)
  , TrueEta_pt_vs_rap_all_Both(nullptr)
  , WrongEta_pt_vs_rap_all_Both(nullptr)
  , TrueEta_pt_vs_rap_est_all_Both(nullptr)
  , WrongEta_pt_vs_rap_est_all_Both(nullptr)
  , TrueEta_InvMass_after_cuts_all_Both(nullptr)
  , WrongEta_InvMass_after_cuts_all_Both(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_all_Both(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_all_Both(nullptr)
  , EMT_Eta_InvMass_all_Both(nullptr)
  , fHistoList_rap_pt_Eta_all_Both()
  , multi_InvMass_Eta_all_Both_1(nullptr)
  , multi_InvMass_Eta_all_Both_2(nullptr)
  , multi_InvMass_Eta_all_Both_3(nullptr)
  , multi_InvMass_Eta_all_Both_4(nullptr)
  , multi_InvMass_Eta_all_Both_5(nullptr)
  , multi_InvMass_Eta_all_Both_6(nullptr)
  , multi_InvMass_Eta_all_Both_7(nullptr)
  , multi_InvMass_Eta_all_Both_8(nullptr)
  , multi_InvMass_Eta_all_Both_9(nullptr)
  , multi_InvMass_Eta_all_Both_10(nullptr)
  , multi_InvMass_Eta_all_Both_11(nullptr)
  , multi_InvMass_Eta_all_Both_12(nullptr)
  , multi_InvMass_Eta_all_Both_13(nullptr)
  , multi_InvMass_Eta_all_Both_14(nullptr)
  , multi_InvMass_Eta_all_Both_15(nullptr)
  , multi_InvMass_Eta_all_Both_16(nullptr)
  , multi_InvMass_Eta_all_Both_17(nullptr)
  , multi_EMT_Eta_all_Both_1(nullptr)
  , multi_EMT_Eta_all_Both_2(nullptr)
  , multi_EMT_Eta_all_Both_3(nullptr)
  , multi_EMT_Eta_all_Both_4(nullptr)
  , multi_EMT_Eta_all_Both_5(nullptr)
  , multi_EMT_Eta_all_Both_6(nullptr)
  , multi_EMT_Eta_all_Both_7(nullptr)
  , multi_EMT_Eta_all_Both_8(nullptr)
  , multi_EMT_Eta_all_Both_9(nullptr)
  , multi_EMT_Eta_all_Both_10(nullptr)
  , multi_EMT_Eta_all_Both_11(nullptr)
  , multi_EMT_Eta_all_Both_12(nullptr)
  , multi_EMT_Eta_all_Both_13(nullptr)
  , multi_EMT_Eta_all_Both_14(nullptr)
  , multi_EMT_Eta_all_Both_15(nullptr)
  , multi_EMT_Eta_all_Both_16(nullptr)
  , multi_EMT_Eta_all_Both_17(nullptr)
  , fHistoList_Eta_onetwo_Both()
  , AngleBetweenGammas_trueEta_before_cuts_onetwo_Both(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both(nullptr)
  , InvMass_GammaCombinations_onetwo_Both(nullptr)
  , Particles_PDG_onetwo_Both(nullptr)
  , TrueEta_pt_vs_rap_onetwo_Both(nullptr)
  , WrongEta_pt_vs_rap_onetwo_Both(nullptr)
  , TrueEta_pt_vs_rap_est_onetwo_Both(nullptr)
  , WrongEta_pt_vs_rap_est_onetwo_Both(nullptr)
  , TrueEta_InvMass_after_cuts_onetwo_Both(nullptr)
  , WrongEta_InvMass_after_cuts_onetwo_Both(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both(nullptr)
  , EMT_Eta_InvMass_onetwo_Both(nullptr)
  , fHistoList_rap_pt_Eta_onetwo_Both()
  , multi_InvMass_Eta_onetwo_Both_1(nullptr)
  , multi_InvMass_Eta_onetwo_Both_2(nullptr)
  , multi_InvMass_Eta_onetwo_Both_3(nullptr)
  , multi_InvMass_Eta_onetwo_Both_4(nullptr)
  , multi_InvMass_Eta_onetwo_Both_5(nullptr)
  , multi_InvMass_Eta_onetwo_Both_6(nullptr)
  , multi_InvMass_Eta_onetwo_Both_7(nullptr)
  , multi_InvMass_Eta_onetwo_Both_8(nullptr)
  , multi_InvMass_Eta_onetwo_Both_9(nullptr)
  , multi_InvMass_Eta_onetwo_Both_10(nullptr)
  , multi_InvMass_Eta_onetwo_Both_11(nullptr)
  , multi_InvMass_Eta_onetwo_Both_12(nullptr)
  , multi_InvMass_Eta_onetwo_Both_13(nullptr)
  , multi_InvMass_Eta_onetwo_Both_14(nullptr)
  , multi_InvMass_Eta_onetwo_Both_15(nullptr)
  , multi_InvMass_Eta_onetwo_Both_16(nullptr)
  , multi_InvMass_Eta_onetwo_Both_17(nullptr)
  , multi_EMT_Eta_onetwo_Both_1(nullptr)
  , multi_EMT_Eta_onetwo_Both_2(nullptr)
  , multi_EMT_Eta_onetwo_Both_3(nullptr)
  , multi_EMT_Eta_onetwo_Both_4(nullptr)
  , multi_EMT_Eta_onetwo_Both_5(nullptr)
  , multi_EMT_Eta_onetwo_Both_6(nullptr)
  , multi_EMT_Eta_onetwo_Both_7(nullptr)
  , multi_EMT_Eta_onetwo_Both_8(nullptr)
  , multi_EMT_Eta_onetwo_Both_9(nullptr)
  , multi_EMT_Eta_onetwo_Both_10(nullptr)
  , multi_EMT_Eta_onetwo_Both_11(nullptr)
  , multi_EMT_Eta_onetwo_Both_12(nullptr)
  , multi_EMT_Eta_onetwo_Both_13(nullptr)
  , multi_EMT_Eta_onetwo_Both_14(nullptr)
  , multi_EMT_Eta_onetwo_Both_15(nullptr)
  , multi_EMT_Eta_onetwo_Both_16(nullptr)
  , multi_EMT_Eta_onetwo_Both_17(nullptr)
  , fHistoList_Eta_two_Both()
  , AngleBetweenGammas_trueEta_before_cuts_two_Both(nullptr)
  , AngleBetweenGammas_wrongEta_before_cuts_two_Both(nullptr)
  , InvMass_GammaCombinations_two_Both(nullptr)
  , Particles_PDG_two_Both(nullptr)
  , TrueEta_pt_vs_rap_two_Both(nullptr)
  , WrongEta_pt_vs_rap_two_Both(nullptr)
  , TrueEta_pt_vs_rap_est_two_Both(nullptr)
  , WrongEta_pt_vs_rap_est_two_Both(nullptr)
  , TrueEta_InvMass_after_cuts_two_Both(nullptr)
  , WrongEta_InvMass_after_cuts_two_Both(nullptr)
  , TrueEta_AngleBetweenGammas_after_cuts_two_Both(nullptr)
  , WrongEta_AngleBetweenGammas_after_cuts_two_Both(nullptr)
  , EMT_Eta_InvMass_two_Both(nullptr)
  , fHistoList_rap_pt_Eta_two_Both()
  , multi_InvMass_Eta_two_Both_1(nullptr)
  , multi_InvMass_Eta_two_Both_2(nullptr)
  , multi_InvMass_Eta_two_Both_3(nullptr)
  , multi_InvMass_Eta_two_Both_4(nullptr)
  , multi_InvMass_Eta_two_Both_5(nullptr)
  , multi_InvMass_Eta_two_Both_6(nullptr)
  , multi_InvMass_Eta_two_Both_7(nullptr)
  , multi_InvMass_Eta_two_Both_8(nullptr)
  , multi_InvMass_Eta_two_Both_9(nullptr)
  , multi_InvMass_Eta_two_Both_10(nullptr)
  , multi_InvMass_Eta_two_Both_11(nullptr)
  , multi_InvMass_Eta_two_Both_12(nullptr)
  , multi_InvMass_Eta_two_Both_13(nullptr)
  , multi_InvMass_Eta_two_Both_14(nullptr)
  , multi_InvMass_Eta_two_Both_15(nullptr)
  , multi_InvMass_Eta_two_Both_16(nullptr)
  , multi_InvMass_Eta_two_Both_17(nullptr)
  , multi_EMT_Eta_two_Both_1(nullptr)
  , multi_EMT_Eta_two_Both_2(nullptr)
  , multi_EMT_Eta_two_Both_3(nullptr)
  , multi_EMT_Eta_two_Both_4(nullptr)
  , multi_EMT_Eta_two_Both_5(nullptr)
  , multi_EMT_Eta_two_Both_6(nullptr)
  , multi_EMT_Eta_two_Both_7(nullptr)
  , multi_EMT_Eta_two_Both_8(nullptr)
  , multi_EMT_Eta_two_Both_9(nullptr)
  , multi_EMT_Eta_two_Both_10(nullptr)
  , multi_EMT_Eta_two_Both_11(nullptr)
  , multi_EMT_Eta_two_Both_12(nullptr)
  , multi_EMT_Eta_two_Both_13(nullptr)
  , multi_EMT_Eta_two_Both_14(nullptr)
  , multi_EMT_Eta_two_Both_15(nullptr)
  , multi_EMT_Eta_two_Both_16(nullptr)
  , multi_EMT_Eta_two_Both_17(nullptr) {}

CbmKresEta::~CbmKresEta() {}

void CbmKresEta::Init() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    Fatal("CbmKresEta::Init", "RootManager not instantised!");
  }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresEta::Init", "No MCTrack array!"); }

  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) {
    fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
  }
  if (nullptr == fPrimVertex) {
    LOG(fatal) << "CbmKresEta::Init  No PrimaryVertex array!";
  }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) {
    Fatal("CbmKresEta::Init", "No GlobalTrack array!");
  }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) {
    Fatal("CbmKresEta::Init", "No StsTrack array!");
  }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) {
    Fatal("CbmKresEta::Init", "No StsTrackMatch array!");
  }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) {
    Fatal("CbmKresEta::Init", "No RichProjection array!");
  }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) {
    Fatal("CbmKresEta::Init", "No RichRing array!");
  }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) {
    Fatal("CbmKresEta::Init", "No RichRingMatch array!");
  }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmKresEta::Init", "No RichHit array!"); }

  fArrayMvdHit = (TClonesArray*) ioman->GetObject("MvdHit");
  if (nullptr == fArrayMvdHit) {
    Fatal("CbmKresEta::Init", "No MvdHit array!");
  }

  fArrayStsHit = (TClonesArray*) ioman->GetObject("StsHit");
  if (nullptr == fArrayStsHit) {
    Fatal("CbmKresEta::Init", "No StsHit array!");
  }

  fTauFit = new CbmRichRingFitterEllipseTau();

  InitHistograms();
}

void CbmKresEta::Exec(int fEventNumEta,
                      double OpeningAngleCut,
                      double GammaInvMassCut,
                      int RealPID) {
  // cout << "CbmKresEta, event No. " <<  fEventNumEta << endl;

  if (fPrimVertex != nullptr) {
    fKFVertex = CbmKFVertex(*fPrimVertex);
  } else {
    Fatal("CbmKresConversionManual::Exec", "No PrimaryVertex array!");
  }

  Gammas_all_Target.clear();
  Gammas_two_Target.clear();
  Gammas_onetwo_Target.clear();
  Gammas_stsIndex_all_Target.clear();
  Gammas_stsIndex_two_Target.clear();
  Gammas_stsIndex_onetwo_Target.clear();
  Gammas_MCIndex_all_Target.clear();
  Gammas_MCIndex_two_Target.clear();
  Gammas_MCIndex_onetwo_Target.clear();
  Gammas_MC_all_Target.clear();
  Gammas_MC_two_Target.clear();
  Gammas_MC_onetwo_Target.clear();
  Gammas_all_Outside.clear();
  Gammas_two_Outside.clear();
  Gammas_onetwo_Outside.clear();
  Gammas_stsIndex_all_Outside.clear();
  Gammas_stsIndex_two_Outside.clear();
  Gammas_stsIndex_onetwo_Outside.clear();
  Gammas_MCIndex_all_Outside.clear();
  Gammas_MCIndex_two_Outside.clear();
  Gammas_MCIndex_onetwo_Outside.clear();
  Gammas_MC_all_Outside.clear();
  Gammas_MC_two_Outside.clear();
  Gammas_MC_onetwo_Outside.clear();
  Gammas_all_Both.clear();
  Gammas_two_Both.clear();
  Gammas_onetwo_Both.clear();
  Gammas_stsIndex_all_Both.clear();
  Gammas_stsIndex_two_Both.clear();
  Gammas_stsIndex_onetwo_Both.clear();
  Gammas_MCIndex_all_Both.clear();
  Gammas_MCIndex_two_Both.clear();
  Gammas_MCIndex_onetwo_Both.clear();
  Gammas_MC_all_Both.clear();
  Gammas_MC_two_Both.clear();
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

  FindGammasTarget(fEventNumEta,
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
                   VRichRing_plus_Target,
                   VMCIndex_minus_Target,
                   VMCIndex_plus_Target);

  FindGammasOutside(fEventNumEta,
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
                    VRichRing_plus_Outside,
                    VMCIndex_minus_Outside,
                    VMCIndex_plus_Outside);

  FindGammasBoth();

  FindEta("all",
          "Target",
          Gammas_all_Target,
          Gammas_stsIndex_all_Target,
          Gammas_MCIndex_all_Target,
          Gammas_MC_all_Target,
          fHistoList_Eta_all_Target,
          fHistoList_rap_pt_Eta_all_Target);
  FindEta("onetwo",
          "Target",
          Gammas_onetwo_Target,
          Gammas_stsIndex_onetwo_Target,
          Gammas_MCIndex_onetwo_Target,
          Gammas_MC_onetwo_Target,
          fHistoList_Eta_onetwo_Target,
          fHistoList_rap_pt_Eta_onetwo_Target);
  FindEta("two",
          "Target",
          Gammas_two_Target,
          Gammas_stsIndex_two_Target,
          Gammas_MCIndex_two_Target,
          Gammas_MC_two_Target,
          fHistoList_Eta_two_Target,
          fHistoList_rap_pt_Eta_two_Target);

  FindEta("all",
          "Outside",
          Gammas_all_Outside,
          Gammas_stsIndex_all_Outside,
          Gammas_MCIndex_all_Outside,
          Gammas_MC_all_Outside,
          fHistoList_Eta_all_Outside,
          fHistoList_rap_pt_Eta_all_Outside);
  FindEta("onetwo",
          "Outside",
          Gammas_onetwo_Outside,
          Gammas_stsIndex_onetwo_Outside,
          Gammas_MCIndex_onetwo_Outside,
          Gammas_MC_onetwo_Outside,
          fHistoList_Eta_onetwo_Outside,
          fHistoList_rap_pt_Eta_onetwo_Outside);
  FindEta("two",
          "Outside",
          Gammas_two_Outside,
          Gammas_stsIndex_two_Outside,
          Gammas_MCIndex_two_Outside,
          Gammas_MC_two_Outside,
          fHistoList_Eta_two_Outside,
          fHistoList_rap_pt_Eta_two_Outside);

  FindEta("all",
          "Both",
          Gammas_all_Both,
          Gammas_stsIndex_all_Both,
          Gammas_MCIndex_all_Both,
          Gammas_MC_all_Both,
          fHistoList_Eta_all_Both,
          fHistoList_rap_pt_Eta_all_Both);
  FindEta("onetwo",
          "Both",
          Gammas_onetwo_Both,
          Gammas_stsIndex_onetwo_Both,
          Gammas_MCIndex_onetwo_Both,
          Gammas_MC_onetwo_Both,
          fHistoList_Eta_onetwo_Both,
          fHistoList_rap_pt_Eta_onetwo_Both);
  FindEta("two",
          "Both",
          Gammas_two_Both,
          Gammas_stsIndex_two_Both,
          Gammas_MCIndex_two_Both,
          Gammas_MC_two_Both,
          fHistoList_Eta_two_Both,
          fHistoList_rap_pt_Eta_two_Both);

  int numformix = 500;
  if (fEventNumEta % numformix == 0) {
    EMT_eta_gg_Event_Both.insert(EMT_eta_gg_Event_Both.end(),
                                 EMT_eta_gg_Event_Outside.begin(),
                                 EMT_eta_gg_Event_Outside.end());
    EMT_eta_gg_Event_Both.insert(EMT_eta_gg_Event_Both.end(),
                                 EMT_eta_gg_Event_Target.begin(),
                                 EMT_eta_gg_Event_Target.end());
    EMT_eta_gg_pair_momenta_Both.insert(EMT_eta_gg_pair_momenta_Both.end(),
                                        EMT_eta_gg_pair_momenta_Outside.begin(),
                                        EMT_eta_gg_pair_momenta_Outside.end());
    EMT_eta_gg_pair_momenta_Both.insert(EMT_eta_gg_pair_momenta_Both.end(),
                                        EMT_eta_gg_pair_momenta_Target.begin(),
                                        EMT_eta_gg_pair_momenta_Target.end());
    EMT_eta_gg_NofRings_Both.insert(EMT_eta_gg_NofRings_Both.end(),
                                    EMT_eta_gg_NofRings_Outside.begin(),
                                    EMT_eta_gg_NofRings_Outside.end());
    EMT_eta_gg_NofRings_Both.insert(EMT_eta_gg_NofRings_Both.end(),
                                    EMT_eta_gg_NofRings_Target.begin(),
                                    EMT_eta_gg_NofRings_Target.end());
    Mixing_Both(fHistoList_rap_pt_Eta_all_Both,
                fHistoList_rap_pt_Eta_onetwo_Both,
                fHistoList_rap_pt_Eta_two_Both);
    EMT_eta_gg_Event_Both.clear();
    EMT_eta_gg_pair_momenta_Both.clear();
    EMT_eta_gg_NofRings_Both.clear();
  }

  if (fEventNumEta % numformix == 0) {
    Mixing_Target();
    EMT_eta_gg_Event_Target.clear();
    EMT_eta_gg_pair_momenta_Target.clear();
    EMT_eta_gg_NofRings_Target.clear();
  }

  if (fEventNumEta % numformix == 0) {
    Mixing_Outside();
    EMT_eta_gg_Event_Outside.clear();
    EMT_eta_gg_pair_momenta_Outside.clear();
    EMT_eta_gg_NofRings_Outside.clear();
  }
}


void CbmKresEta::SaveOutsideTracks(CbmMCTrack* mcTrack1,
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


void CbmKresEta::SaveTargetTracks(CbmMCTrack* mcTrack1,
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

int CbmKresEta::FindInRich(int richInd, int stsMcTrackId) {
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

int CbmKresEta::CheckIfElectron(CbmRichRing* ring, double momentum) {
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
  }

  return identified;
}

double CbmKresEta::CalculatePlaneAngle_last(CbmStsTrack* Sts_1,
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

double CbmKresEta::CalculatePlaneAngle_first(CbmStsTrack* Sts_1,
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


void CbmKresEta::FindGammasTarget(int EventNumEta,
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
                                  vector<CbmRichRing*> richRing_plus,
                                  vector<Int_t> MCIndex_minus,
                                  vector<Int_t> MCIndex_plus) {
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
        richcheck_0 =
          CheckIfElectron(richRing_minus[i], Momenta_minus[i].Mag());
        richcheck_1 = CheckIfElectron(richRing_plus[j], Momenta_plus[j].Mag());
      } else {
        // MC   PID
        richcheck_0 = Rings_minus[i];
        richcheck_1 = Rings_plus[j];
      }
      int richcheck = richcheck_0 + richcheck_1;

      double InvmassReco =
        CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      double PlaneAngle_last  = CalculatePlaneAngle_last(sts1, sts2);
      double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      int TruePair =
        0;  //correctly reconstructed photon from eta meson:          0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() != -1
            && mcTrackmama->GetPdgCode() == 22) {
          CbmMCTrack* mcTrackgrmama =
            (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
          if (mcTrackgrmama->GetPdgCode() == 221) { TruePair = 1; }
        }
      }

      if (TruePair == 1) {
        if (richcheck == 0) {
          GammasInvMass_fromEta_beforeCuts_zeroInRich_Target->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_zeroInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_zeroInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_zeroInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_zeroInRich_Target->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_zeroInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_zeroInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_zeroInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_zeroInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_zeroInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 1) {
          GammasInvMass_fromEta_beforeCuts_oneInRich_Target->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_oneInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_oneInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_oneInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_oneInRich_Target->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_oneInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both->Fill(OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_oneInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_oneInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_oneInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_oneInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 2) {
          GammasInvMass_fromEta_beforeCuts_twoInRich_Target->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_twoInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_twoInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_twoInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_twoInRich_Target->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_twoInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both->Fill(OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_twoInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_twoInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_twoInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_twoInRich_Both->Fill(PlaneAngle_last);
        }
        Particle_pull_px_Target->Fill(part1MC->GetPx() - part1.X());
        Particle_pull_py_Target->Fill(part1MC->GetPy() - part1.Y());
        Particle_pull_pz_Target->Fill(part1MC->GetPz() - part1.Z());
        Particle_pull_px_Target->Fill(part2MC->GetPx() - part2.X());
        Particle_pull_py_Target->Fill(part2MC->GetPy() - part2.Y());
        Particle_pull_pz_Target->Fill(part2MC->GetPz() - part2.Z());
      } else {
        if (richcheck == 0) {
          GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_zeroInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_zeroInRich_Target->Fill(
            PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_zeroInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_zeroInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 1) {
          GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_oneInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_oneInRich_Target->Fill(PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_oneInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_oneInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 2) {
          GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_twoInRich_Target->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_twoInRich_Target->Fill(PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_twoInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_twoInRich_Both->Fill(PlaneAngle_last);
        }
      }


      // cuts for gamma reconstruction
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;

      if (richcheck == 1 && TruePair == 1) {
        PlaneAngles_first_fromEta_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_first);
        PlaneAngles_last_fromEta_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_last);
      }
      if (richcheck == 1 && TruePair == 0) {
        PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_first);
        PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_last);
      }

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
      EMT_eta_gg_Event_Target.push_back(EventNumEta);
      EMT_eta_gg_pair_momenta_Target.push_back(frefmomenta);
      EMT_eta_gg_NofRings_Target.push_back(richcheck);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Target.push_back(frefmomenta);
        Gammas_stsIndex_all_Target.push_back(frefId);
        Gammas_MCIndex_all_Target.push_back(fMCId);
        Gammas_MC_all_Target.push_back(fMCtracks);
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Target.push_back(frefmomenta);
        Gammas_stsIndex_two_Target.push_back(frefId);
        Gammas_MCIndex_two_Target.push_back(fMCId);
        Gammas_MC_two_Target.push_back(fMCtracks);
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Target.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Target.push_back(frefId);
        Gammas_MCIndex_onetwo_Target.push_back(fMCId);
        Gammas_MC_onetwo_Target.push_back(fMCtracks);
      }
    }
  }
}


void CbmKresEta::FindGammasOutside(int EventNumEta,
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
                                   std::vector<int> stsIndex_plus_Outside,
                                   vector<CbmRichRing*> richRing_minus_Outside,
                                   vector<CbmRichRing*> richRing_plus_Outside,
                                   vector<Int_t> MCIndex_minus_Outside,
                                   vector<Int_t> MCIndex_plus_Outside) {
  for (size_t i = 0; i < StsTrack_minus_Outside.size(); i++) {
    for (size_t j = 0; j < StsTrack_plus_Outside.size(); j++) {

      CbmStsTrack* sts1 = StsTrack_minus_Outside[i];
      CbmStsTrack* sts2 = StsTrack_plus_Outside[j];

      CbmMCTrack* part1MC = MCtracks_minus_Outside[i];
      CbmMCTrack* part2MC = MCtracks_plus_Outside[j];

      KFParticle electron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts1, &electron, 11);
      KFParticle positron;
      CbmKFParticleInterface::SetKFParticleFromStsTrack(sts2, &positron, -11);
      const KFParticle* daughters[2] = {&electron, &positron};
      KFParticle intersection;
      intersection.Construct(daughters, 2);

      if (intersection.GetZ() > 75 || intersection.GetZ() < -5)
        continue;  // kick weird intersections

      // fit to the vertex fitter
      // TVector3 part1 = CbmKresFunctions::FitToVertex(sts1, intersection.GetX(), intersection.GetY(), intersection.GetZ());
      // TVector3 part2 = CbmKresFunctions::FitToVertex(sts2, intersection.GetX(), intersection.GetY(), intersection.GetZ());

      double chi_electron = 0;
      double chi_positron = 0;
      TVector3 part1 =
        CbmKresFunctions::FitToVertexAndGetChi(sts1,
                                               intersection.GetX(),
                                               intersection.GetY(),
                                               intersection.GetZ(),
                                               chi_electron);
      TVector3 part2 =
        CbmKresFunctions::FitToVertexAndGetChi(sts2,
                                               intersection.GetX(),
                                               intersection.GetY(),
                                               intersection.GetZ(),
                                               chi_positron);
      if (chi_electron != chi_electron) continue;
      if (chi_electron == 0) continue;
      if (chi_electron > 3) continue;
      if (chi_positron != chi_positron) continue;
      if (chi_positron == 0) continue;
      if (chi_positron > 3) continue;

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

      double InvmassReco =
        CbmKresFunctions::Invmass_2particles_RECO(part1, part2);
      double OpeningAngle =
        CbmKresFunctions::CalculateOpeningAngle_Reco(part1, part2);
      double PlaneAngle_last  = CalculatePlaneAngle_last(sts1, sts2);
      double PlaneAngle_first = CalculatePlaneAngle_first(sts1, sts2);
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParamsReco(part1, part2);

      int TruePair =
        0;  //correctly reconstructed photon from eta meson:          0 - means wrong pair combination; 		1 - means correct pair
      if (part1MC->GetMotherId() == part2MC->GetMotherId()
          && part1MC->GetMotherId() != -1) {
        CbmMCTrack* mcTrackmama =
          (CbmMCTrack*) fMcTracks->At(part1MC->GetMotherId());
        if (mcTrackmama->GetMotherId() != -1
            && mcTrackmama->GetPdgCode() == 22) {
          CbmMCTrack* mcTrackgrmama =
            (CbmMCTrack*) fMcTracks->At(mcTrackmama->GetMotherId());
          if (mcTrackgrmama->GetPdgCode() == 221) { TruePair = 1; }
        }
      }

      if (TruePair == 1) {
        if (richcheck == 0) {
          GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside->Fill(
            InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_zeroInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_zeroInRich_Outside->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_zeroInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_zeroInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_zeroInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_zeroInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_zeroInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 1) {
          GammasInvMass_fromEta_beforeCuts_oneInRich_Outside->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_oneInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_oneInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_oneInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_oneInRich_Outside->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_oneInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both->Fill(OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_oneInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_oneInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_oneInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_oneInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 2) {
          GammasInvMass_fromEta_beforeCuts_twoInRich_Outside->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_twoInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_twoInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_fromEta_twoInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_fromEta_twoInRich_Outside->Fill(PlaneAngle_first);

          GammasInvMass_fromEta_beforeCuts_twoInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both->Fill(OpeningAngle);
          GammasMomentum_fromEta_beforeCuts_twoInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_fromEta_beforeCuts_twoInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_fromEta_twoInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_fromEta_twoInRich_Both->Fill(PlaneAngle_last);
        }
        Particle_pull_px_Outside->Fill(part1MC->GetPx() - part1.X());
        Particle_pull_py_Outside->Fill(part1MC->GetPy() - part1.Y());
        Particle_pull_pz_Outside->Fill(part1MC->GetPz() - part1.Z());
        Particle_pull_px_Outside->Fill(part2MC->GetPx() - part2.X());
        Particle_pull_py_Outside->Fill(part2MC->GetPy() - part2.Y());
        Particle_pull_pz_Outside->Fill(part2MC->GetPz() - part2.Z());
        Particle_pull_X_Outside->Fill(part1MC->GetStartX()
                                      - intersection.GetX());
        Particle_pull_Y_Outside->Fill(part1MC->GetStartY()
                                      - intersection.GetY());
        Particle_pull_Z_Outside->Fill(part1MC->GetStartZ()
                                      - intersection.GetZ());
        Particle_pull_X_Outside->Fill(part2MC->GetStartX()
                                      - intersection.GetX());
        Particle_pull_Y_Outside->Fill(part2MC->GetStartY()
                                      - intersection.GetY());
        Particle_pull_Z_Outside->Fill(part2MC->GetStartZ()
                                      - intersection.GetZ());
      } else {
        if (richcheck == 0) {
          GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_zeroInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_zeroInRich_Outside->Fill(
            PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_zeroInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_zeroInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 1) {
          GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_oneInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_oneInRich_Outside->Fill(
            PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_oneInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_oneInRich_Both->Fill(PlaneAngle_last);
        }
        if (richcheck == 2) {
          GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside->Fill(
            InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside->Fill(
            params.fRapidity);
          PlaneAngles_last_wrongpairs_twoInRich_Outside->Fill(PlaneAngle_last);
          PlaneAngles_first_wrongpairs_twoInRich_Outside->Fill(
            PlaneAngle_first);

          GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both->Fill(InvmassReco);
          GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            OpeningAngle);
          GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            params.fMomentumMag);
          GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both->Fill(
            params.fRapidity);
          PlaneAngles_first_wrongpairs_twoInRich_Both->Fill(PlaneAngle_first);
          PlaneAngles_last_wrongpairs_twoInRich_Both->Fill(PlaneAngle_last);
        }
      }


      // cuts for gamma reconstruction
      if (TMath::Abs(OpeningAngle) > AngleCut) continue;
      if (TMath::Abs(InvmassReco) > InvMassCut) continue;


      if (richcheck == 1 && TruePair == 1) {
        PlaneAngles_first_fromEta_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_first);
        PlaneAngles_last_fromEta_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_last);
      }
      if (richcheck == 1 && TruePair == 0) {
        PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_first);
        PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both->Fill(
          PlaneAngle_last);
      }


      frefmomenta.clear();
      frefmomenta.push_back(part1);
      frefmomenta.push_back(part2);
      frefId.clear();
      frefId.push_back(stsIndex_minus_Outside[i]);
      frefId.push_back(stsIndex_plus_Outside[j]);
      fMCId.clear();
      fMCId.push_back(MCIndex_minus_Outside[i]);
      fMCId.push_back(MCIndex_plus_Outside[j]);
      fMCtracks.clear();
      fMCtracks.push_back(part1MC);
      fMCtracks.push_back(part2MC);

      // for event mixing
      EMT_eta_gg_Event_Outside.push_back(EventNumEta);
      EMT_eta_gg_pair_momenta_Outside.push_back(frefmomenta);
      EMT_eta_gg_NofRings_Outside.push_back(richcheck);


      // everything (RICH == 0, RICH == 1, RICH == 2) together
      if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
        Gammas_all_Outside.push_back(frefmomenta);
        Gammas_stsIndex_all_Outside.push_back(frefId);
        Gammas_MCIndex_all_Outside.push_back(fMCId);
        Gammas_MC_all_Outside.push_back(fMCtracks);
      }

      // only cases, when RICH == 2
      if (richcheck == 2) {
        Gammas_two_Outside.push_back(frefmomenta);
        Gammas_stsIndex_two_Outside.push_back(frefId);
        Gammas_MCIndex_two_Outside.push_back(fMCId);
        Gammas_MC_two_Outside.push_back(fMCtracks);
      }

      // cases, when RICH == 1 or RICH == 2 together
      if (richcheck == 1 || richcheck == 2) {
        Gammas_onetwo_Outside.push_back(frefmomenta);
        Gammas_stsIndex_onetwo_Outside.push_back(frefId);
        Gammas_MCIndex_onetwo_Outside.push_back(fMCId);
        Gammas_MC_onetwo_Outside.push_back(fMCtracks);
      }
    }
  }
}

void CbmKresEta::FindGammasBoth() {
  Gammas_all_Both.insert(Gammas_all_Both.end(),
                         Gammas_all_Outside.begin(),
                         Gammas_all_Outside.end());
  Gammas_all_Both.insert(
    Gammas_all_Both.end(), Gammas_all_Target.begin(), Gammas_all_Target.end());
  Gammas_two_Both.insert(Gammas_two_Both.end(),
                         Gammas_two_Outside.begin(),
                         Gammas_two_Outside.end());
  Gammas_two_Both.insert(
    Gammas_two_Both.end(), Gammas_two_Target.begin(), Gammas_two_Target.end());
  Gammas_onetwo_Both.insert(Gammas_onetwo_Both.end(),
                            Gammas_onetwo_Outside.begin(),
                            Gammas_onetwo_Outside.end());
  Gammas_onetwo_Both.insert(Gammas_onetwo_Both.end(),
                            Gammas_onetwo_Target.begin(),
                            Gammas_onetwo_Target.end());

  Gammas_stsIndex_all_Both.insert(Gammas_stsIndex_all_Both.end(),
                                  Gammas_stsIndex_all_Outside.begin(),
                                  Gammas_stsIndex_all_Outside.end());
  Gammas_stsIndex_all_Both.insert(Gammas_stsIndex_all_Both.end(),
                                  Gammas_stsIndex_all_Target.begin(),
                                  Gammas_stsIndex_all_Target.end());
  Gammas_stsIndex_two_Both.insert(Gammas_stsIndex_two_Both.end(),
                                  Gammas_stsIndex_two_Outside.begin(),
                                  Gammas_stsIndex_two_Outside.end());
  Gammas_stsIndex_two_Both.insert(Gammas_stsIndex_two_Both.end(),
                                  Gammas_stsIndex_two_Target.begin(),
                                  Gammas_stsIndex_two_Target.end());
  Gammas_stsIndex_onetwo_Both.insert(Gammas_stsIndex_onetwo_Both.end(),
                                     Gammas_stsIndex_onetwo_Outside.begin(),
                                     Gammas_stsIndex_onetwo_Outside.end());
  Gammas_stsIndex_onetwo_Both.insert(Gammas_stsIndex_onetwo_Both.end(),
                                     Gammas_stsIndex_onetwo_Target.begin(),
                                     Gammas_stsIndex_onetwo_Target.end());

  Gammas_MCIndex_all_Both.insert(Gammas_MCIndex_all_Both.end(),
                                 Gammas_MCIndex_all_Outside.begin(),
                                 Gammas_MCIndex_all_Outside.end());
  Gammas_MCIndex_all_Both.insert(Gammas_MCIndex_all_Both.end(),
                                 Gammas_MCIndex_all_Target.begin(),
                                 Gammas_MCIndex_all_Target.end());
  Gammas_MCIndex_two_Both.insert(Gammas_MCIndex_two_Both.end(),
                                 Gammas_MCIndex_two_Outside.begin(),
                                 Gammas_MCIndex_two_Outside.end());
  Gammas_MCIndex_two_Both.insert(Gammas_MCIndex_two_Both.end(),
                                 Gammas_MCIndex_two_Target.begin(),
                                 Gammas_MCIndex_two_Target.end());
  Gammas_MCIndex_onetwo_Both.insert(Gammas_MCIndex_onetwo_Both.end(),
                                    Gammas_MCIndex_onetwo_Outside.begin(),
                                    Gammas_MCIndex_onetwo_Outside.end());
  Gammas_MCIndex_onetwo_Both.insert(Gammas_MCIndex_onetwo_Both.end(),
                                    Gammas_MCIndex_onetwo_Target.begin(),
                                    Gammas_MCIndex_onetwo_Target.end());

  Gammas_MC_all_Both.insert(Gammas_MC_all_Both.end(),
                            Gammas_MC_all_Outside.begin(),
                            Gammas_MC_all_Outside.end());
  Gammas_MC_all_Both.insert(Gammas_MC_all_Both.end(),
                            Gammas_MC_all_Target.begin(),
                            Gammas_MC_all_Target.end());
  Gammas_MC_two_Both.insert(Gammas_MC_two_Both.end(),
                            Gammas_MC_two_Outside.begin(),
                            Gammas_MC_two_Outside.end());
  Gammas_MC_two_Both.insert(Gammas_MC_two_Both.end(),
                            Gammas_MC_two_Target.begin(),
                            Gammas_MC_two_Target.end());
  Gammas_MC_onetwo_Both.insert(Gammas_MC_onetwo_Both.end(),
                               Gammas_MC_onetwo_Outside.begin(),
                               Gammas_MC_onetwo_Outside.end());
  Gammas_MC_onetwo_Both.insert(Gammas_MC_onetwo_Both.end(),
                               Gammas_MC_onetwo_Target.begin(),
                               Gammas_MC_onetwo_Target.end());
}


void CbmKresEta::FindEta(TString /*mod*/,
                         TString position,
                         vector<vector<TVector3>> Gammas,
                         vector<vector<int>> StsIndex,
                         vector<vector<int>> MCIndex,
                         vector<vector<CbmMCTrack*>> GammasMC,
                         vector<TH1*> gg,
                         vector<TH1*> rap_pt_separation) {
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

      if (StsIndex[gamma1][0] == StsIndex[gamma2][0]
          || StsIndex[gamma1][0] == StsIndex[gamma2][1]
          || StsIndex[gamma1][1] == StsIndex[gamma2][0]
          || StsIndex[gamma1][1] == StsIndex[gamma2][1])
        continue;  // particles are not used twice --> different

      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParams_4particles(
          e11, e12, e21, e22);
      double openingAngleBetweenGammasReco =
        CbmKresFunctions::CalculateOpeningAngleBetweenGammas_Reco(
          e11, e12, e21, e22);

      int TrueEta  = 0;
      int STSmcId1 = MCIndex[gamma1][0];
      int STSmcId2 = MCIndex[gamma1][1];
      int STSmcId3 = MCIndex[gamma2][0];
      int STSmcId4 = MCIndex[gamma2][1];
      if (STSmcId1 != STSmcId2 && STSmcId1 != STSmcId3 && STSmcId1 != STSmcId4
          && STSmcId2 != STSmcId3 && STSmcId2 != STSmcId4
          && STSmcId3 != STSmcId4) {
        if (TMath::Abs(mcTrack1->GetPdgCode()) == 11
            && TMath::Abs(mcTrack2->GetPdgCode()) == 11
            && TMath::Abs(mcTrack3->GetPdgCode()) == 11
            && TMath::Abs(mcTrack4->GetPdgCode()) == 11) {
          if ((mcTrack1->GetMotherId() == mcTrack2->GetMotherId()
               && mcTrack3->GetMotherId() == mcTrack4->GetMotherId())
              || (mcTrack1->GetMotherId() == mcTrack3->GetMotherId()
                  && mcTrack2->GetMotherId() == mcTrack4->GetMotherId())
              || (mcTrack1->GetMotherId() == mcTrack4->GetMotherId()
                  && mcTrack2->GetMotherId() == mcTrack3->GetMotherId())) {
            CbmMCTrack* MothTrack1 =
              (CbmMCTrack*) fMcTracks->At(mcTrack1->GetMotherId());
            CbmMCTrack* MothTrack2 =
              (CbmMCTrack*) fMcTracks->At(mcTrack2->GetMotherId());
            CbmMCTrack* MothTrack3 =
              (CbmMCTrack*) fMcTracks->At(mcTrack3->GetMotherId());
            CbmMCTrack* MothTrack4 =
              (CbmMCTrack*) fMcTracks->At(mcTrack4->GetMotherId());
            if (MothTrack1->GetPdgCode() == 22 && MothTrack2->GetPdgCode() == 22
                && MothTrack3->GetPdgCode() == 22
                && MothTrack4->GetPdgCode() == 22
                && MothTrack1->GetMotherId() != -1
                && MothTrack1->GetMotherId() == MothTrack2->GetMotherId()
                && MothTrack1->GetMotherId() == MothTrack3->GetMotherId()
                && MothTrack1->GetMotherId() == MothTrack4->GetMotherId()) {
              CbmMCTrack* GrandMothTrack1 =
                (CbmMCTrack*) fMcTracks->At(MothTrack1->GetMotherId());
              if (GrandMothTrack1->GetPdgCode() == 221) {
                TrueEta = 1;
                // cout << "  " << mod << ";  " << position << " ***** CbmKresEta  ***** " << endl;
                // cout << "Decay eta -> gamma gamma -> e+e- e+e- detected!\t\t reco mass: " << params.fMinv << endl;
                // cout << "motherids: " << mcTrack1->GetMotherId() << "/" << mcTrack2->GetMotherId() << "/" << mcTrack3->GetMotherId() << "/" << mcTrack4->GetMotherId() << endl;
                // cout << "grandmotherid: " << MothTrack1->GetMotherId() << "/" << MothTrack2->GetMotherId() << "/" << MothTrack3->GetMotherId() << "/" << MothTrack4->GetMotherId() << endl;
                // cout << "pdgs: " << GrandMothTrack1->GetPdgCode() << "-->" << MothTrack1->GetPdgCode() << "/" << MothTrack2->GetPdgCode() << "/" << MothTrack3->GetPdgCode() << "/" << MothTrack4->GetPdgCode() << endl;
                // cout << "particle 1: \t" << mcTrack1->GetPdgCode() << ";\t pt = " << mcTrack1->GetPt() << ";\t X = " << mcTrack1->GetStartX() << ";\t Y = " << mcTrack1->GetStartY() << ";\t Z = " << mcTrack1->GetStartZ() << ";\t E = " << mcTrack1->GetEnergy() << endl;
                // cout << "particle 2: \t" << mcTrack2->GetPdgCode() << ";\t pt = " << mcTrack2->GetPt() << ";\t X = " << mcTrack2->GetStartX() << ";\t Y = " << mcTrack2->GetStartY() << ";\t Z = " << mcTrack2->GetStartZ() << ";\t E = " << mcTrack2->GetEnergy() << endl;
                // cout << "particle 3: \t" << mcTrack3->GetPdgCode() << ";\t pt = " << mcTrack3->GetPt() << ";\t X = " << mcTrack3->GetStartX() << ";\t Y = " << mcTrack3->GetStartY() << ";\t Z = " << mcTrack3->GetStartZ() << ";\t E = " << mcTrack3->GetEnergy() << endl;
                // cout << "particle 4: \t" << mcTrack4->GetPdgCode() << ";\t pt = " << mcTrack4->GetPt() << ";\t X = " << mcTrack4->GetStartX() << ";\t Y = " << mcTrack4->GetStartY() << ";\t Z = " << mcTrack4->GetStartZ() << ";\t E = " << mcTrack4->GetEnergy() << endl;
                // cout << " ***** CbmKresEta (End)  ***** " << endl;
              }
            }
          }
        }
      }

      if (TrueEta == 1) gg[0]->Fill(openingAngleBetweenGammasReco);
      if (TrueEta == 0) gg[1]->Fill(openingAngleBetweenGammasReco);

      if (openingAngleBetweenGammasReco < 10
          || openingAngleBetweenGammasReco > 40)
        continue;

      gg[2]->Fill(params.fMinv);

      gg[3]->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      gg[3]->Fill(TMath::Abs(mcTrack2->GetPdgCode()));
      gg[3]->Fill(TMath::Abs(mcTrack3->GetPdgCode()));
      gg[3]->Fill(TMath::Abs(mcTrack4->GetPdgCode()));

      // separate by rap and Pt only for case "Both"
      if (position == "Both") {
        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation[1]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation[2]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation[3]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation[4]->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation[5]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation[6]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation[7]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation[8]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation[9]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation[10]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation[11]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation[12]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation[13]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation[14]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation[15]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation[16]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation[17]->Fill(params.fMinv);
        }
      }

      if (TrueEta == 1) {
        gg[4]->Fill(params.fRapidity, params.fPt);
        gg[6]->Fill(params.fRapidity, params.fPt);
        gg[8]->Fill(params.fMinv);
        gg[10]->Fill(openingAngleBetweenGammasReco);
      }

      if (TrueEta == 0) {
        gg[5]->Fill(params.fRapidity, params.fPt);
        gg[7]->Fill(params.fRapidity, params.fPt);
        gg[9]->Fill(params.fMinv);
        gg[11]->Fill(openingAngleBetweenGammasReco);
      }
    }
  }
}

void CbmKresEta::Mixing_Target()
// TARGET
{
  int nof_Target = EMT_eta_gg_Event_Target.size();
  cout << "Mixing for Eta(Target) - nof entries " << nof_Target << endl;
  for (Int_t a = 0; a < nof_Target - 1; a++) {
    for (Int_t b = a + 1; b < nof_Target; b++) {
      if (EMT_eta_gg_Event_Target[a] == EMT_eta_gg_Event_Target[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11 = EMT_eta_gg_pair_momenta_Target[a][0];
      TVector3 e12 = EMT_eta_gg_pair_momenta_Target[a][1];
      TVector3 e21 = EMT_eta_gg_pair_momenta_Target[b][0];
      TVector3 e22 = EMT_eta_gg_pair_momenta_Target[b][1];
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParams_4particles(
          e11, e12, e21, e22);
      double openingAngleBetweenGammasReco =
        CbmKresFunctions::CalculateOpeningAngleBetweenGammas_Reco(
          e11, e12, e21, e22);
      if (openingAngleBetweenGammasReco < 10
          || openingAngleBetweenGammasReco > 40)
        continue;

      EMT_Eta_InvMass_all_Target->Fill(params.fMinv);
      if (EMT_eta_gg_NofRings_Target[a] == 2
          && EMT_eta_gg_NofRings_Target[b] == 2)
        EMT_Eta_InvMass_two_Target->Fill(params.fMinv);
      if ((EMT_eta_gg_NofRings_Target[a] == 1
           || EMT_eta_gg_NofRings_Target[a] == 2)
          && (EMT_eta_gg_NofRings_Target[b] == 1
              || EMT_eta_gg_NofRings_Target[b] == 2))
        EMT_Eta_InvMass_onetwo_Target->Fill(params.fMinv);
    }
  }
}


void CbmKresEta::Mixing_Outside()
// OUTSIDE
{
  Int_t nof_Outside = EMT_eta_gg_Event_Outside.size();
  cout << "Mixing for Eta(Outside) - nof entries " << nof_Outside << endl;
  for (Int_t a = 0; a < nof_Outside - 1; a++) {
    for (Int_t b = a + 1; b < nof_Outside; b++) {
      if (EMT_eta_gg_Event_Outside[a] == EMT_eta_gg_Event_Outside[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11 = EMT_eta_gg_pair_momenta_Outside[a][0];
      TVector3 e12 = EMT_eta_gg_pair_momenta_Outside[a][1];
      TVector3 e21 = EMT_eta_gg_pair_momenta_Outside[b][0];
      TVector3 e22 = EMT_eta_gg_pair_momenta_Outside[b][1];
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParams_4particles(
          e11, e12, e21, e22);
      double openingAngleBetweenGammasReco =
        CbmKresFunctions::CalculateOpeningAngleBetweenGammas_Reco(
          e11, e12, e21, e22);
      if (openingAngleBetweenGammasReco < 10
          || openingAngleBetweenGammasReco > 40)
        continue;

      EMT_Eta_InvMass_all_Outside->Fill(params.fMinv);
      if (EMT_eta_gg_NofRings_Outside[a] == 2
          && EMT_eta_gg_NofRings_Outside[b] == 2)
        EMT_Eta_InvMass_two_Outside->Fill(params.fMinv);
      if ((EMT_eta_gg_NofRings_Outside[a] == 1
           || EMT_eta_gg_NofRings_Outside[a] == 2)
          && (EMT_eta_gg_NofRings_Outside[b] == 1
              || EMT_eta_gg_NofRings_Outside[b] == 2))
        EMT_Eta_InvMass_onetwo_Outside->Fill(params.fMinv);
    }
  }
}

void CbmKresEta::Mixing_Both(vector<TH1*> rap_pt_separation_all,
                             vector<TH1*> rap_pt_separation_onetwo,
                             vector<TH1*> rap_pt_separation_two)
// BOTH
{
  Int_t nof_Both = EMT_eta_gg_Event_Both.size();
  cout << "Mixing for Eta(Both) - nof entries " << nof_Both << endl;
  for (Int_t a = 0; a < nof_Both - 1; a++) {
    for (Int_t b = a + 1; b < nof_Both; b++) {
      if (EMT_eta_gg_Event_Both[a] == EMT_eta_gg_Event_Both[b])
        continue;  // to make sure that the photons are from two different events
      TVector3 e11 = EMT_eta_gg_pair_momenta_Both[a][0];
      TVector3 e12 = EMT_eta_gg_pair_momenta_Both[a][1];
      TVector3 e21 = EMT_eta_gg_pair_momenta_Both[b][0];
      TVector3 e22 = EMT_eta_gg_pair_momenta_Both[b][1];
      CbmLmvmKinematicParams params =
        CbmKresFunctions::CalculateKinematicParams_4particles(
          e11, e12, e21, e22);
      double openingAngleBetweenGammasReco =
        CbmKresFunctions::CalculateOpeningAngleBetweenGammas_Reco(
          e11, e12, e21, e22);
      if (openingAngleBetweenGammasReco < 10
          || openingAngleBetweenGammasReco > 40)
        continue;

      EMT_Eta_InvMass_all_Both->Fill(params.fMinv);
      if (EMT_eta_gg_NofRings_Both[a] == 2 && EMT_eta_gg_NofRings_Both[b] == 2)
        EMT_Eta_InvMass_two_Both->Fill(params.fMinv);
      if ((EMT_eta_gg_NofRings_Both[a] == 1 || EMT_eta_gg_NofRings_Both[a] == 2)
          && (EMT_eta_gg_NofRings_Both[b] == 1
              || EMT_eta_gg_NofRings_Both[b] == 2))
        EMT_Eta_InvMass_onetwo_Both->Fill(params.fMinv);

      // separate by rap and Pt only for all
      if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
        if (params.fPt > 0.0 && params.fPt <= 0.4)
          rap_pt_separation_all[18]->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.8)
          rap_pt_separation_all[19]->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 1.2)
          rap_pt_separation_all[20]->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.6)
          rap_pt_separation_all[21]->Fill(params.fMinv);
      }
      if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
        if (params.fPt > 0.0 && params.fPt <= 0.4)
          rap_pt_separation_all[22]->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.8)
          rap_pt_separation_all[23]->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 1.2)
          rap_pt_separation_all[24]->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.6)
          rap_pt_separation_all[25]->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
        if (params.fPt > 0.0 && params.fPt <= 0.4)
          rap_pt_separation_all[26]->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.8)
          rap_pt_separation_all[27]->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 1.2)
          rap_pt_separation_all[28]->Fill(params.fMinv);
        if (params.fPt > 1.2 && params.fPt <= 1.6)
          rap_pt_separation_all[29]->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
        if (params.fPt > 0.0 && params.fPt <= 0.4)
          rap_pt_separation_all[30]->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.8)
          rap_pt_separation_all[31]->Fill(params.fMinv);
        if (params.fPt > 0.8 && params.fPt <= 1.2)
          rap_pt_separation_all[32]->Fill(params.fMinv);
      }
      if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
        if (params.fPt > 0.0 && params.fPt <= 0.4)
          rap_pt_separation_all[33]->Fill(params.fMinv);
        if (params.fPt > 0.4 && params.fPt <= 0.8)
          rap_pt_separation_all[34]->Fill(params.fMinv);
      }

      // separate by rap and Pt only for onetwo
      if ((EMT_eta_gg_NofRings_Both[a] == 1 || EMT_eta_gg_NofRings_Both[a] == 2)
          && (EMT_eta_gg_NofRings_Both[b] == 1
              || EMT_eta_gg_NofRings_Both[b] == 2)) {
        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_onetwo[18]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_onetwo[19]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_onetwo[20]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_onetwo[21]->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_onetwo[22]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_onetwo[23]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_onetwo[24]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_onetwo[25]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_onetwo[26]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_onetwo[27]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_onetwo[28]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_onetwo[29]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_onetwo[30]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_onetwo[31]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_onetwo[32]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_onetwo[33]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_onetwo[34]->Fill(params.fMinv);
        }
      }

      // separate by rap and Pt only for two
      if (EMT_eta_gg_NofRings_Both[a] == 2
          && EMT_eta_gg_NofRings_Both[b] == 2) {
        if (params.fRapidity > 1.2 && params.fRapidity <= 1.6) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_two[18]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_two[19]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_two[20]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_two[21]->Fill(params.fMinv);
        }
        if (params.fRapidity > 1.6 && params.fRapidity <= 2.0) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_two[22]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_two[23]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_two[24]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_two[25]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.0 && params.fRapidity <= 2.4) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_two[26]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_two[27]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_two[28]->Fill(params.fMinv);
          if (params.fPt > 1.2 && params.fPt <= 1.6)
            rap_pt_separation_two[29]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.4 && params.fRapidity <= 2.8) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_two[30]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_two[31]->Fill(params.fMinv);
          if (params.fPt > 0.8 && params.fPt <= 1.2)
            rap_pt_separation_two[32]->Fill(params.fMinv);
        }
        if (params.fRapidity > 2.8 && params.fRapidity <= 3.2) {
          if (params.fPt > 0.0 && params.fPt <= 0.4)
            rap_pt_separation_two[33]->Fill(params.fMinv);
          if (params.fPt > 0.4 && params.fPt <= 0.8)
            rap_pt_separation_two[34]->Fill(params.fMinv);
        }
      }
    }
  }
}


void CbmKresEta::Finish() {
  gDirectory->mkdir("Eta");
  gDirectory->cd("Eta");

  gDirectory->mkdir("Cuts_for_Eta_gg");
  gDirectory->cd("Cuts_for_Eta_gg");

  gDirectory->mkdir("Cuts_for_Eta_gg_Target");
  gDirectory->cd("Cuts_for_Eta_gg_Target");
  gDirectory->mkdir("Cuts_for_Eta_gg_zeroInRich_Target");
  gDirectory->cd("Cuts_for_Eta_gg_zeroInRich_Target");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_zeroInRich_Target.size(); i++) {
    fHistoList_Eta_cuts_zeroInRich_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_oneInRich_Target");
  gDirectory->cd("Cuts_for_Eta_gg_oneInRich_Target");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_oneInRich_Target.size(); i++) {
    fHistoList_Eta_cuts_oneInRich_Target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_twoInRich_Target");
  gDirectory->cd("Cuts_for_Eta_gg_twoInRich_Target");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_twoInRich_Target.size(); i++) {
    fHistoList_Eta_cuts_twoInRich_Target[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_Target.size(); i++) {
    fHistoList_Eta_cuts_Target[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("Cuts_for_Eta_gg_Outside");
  gDirectory->cd("Cuts_for_Eta_gg_Outside");
  gDirectory->mkdir("Cuts_for_Eta_gg_zeroInRich_Outside");
  gDirectory->cd("Cuts_for_Eta_gg_zeroInRich_Outside");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_zeroInRich_Outside.size(); i++) {
    fHistoList_Eta_cuts_zeroInRich_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_oneInRich_Outside");
  gDirectory->cd("Cuts_for_Eta_gg_oneInRich_Outside");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_oneInRich_Outside.size(); i++) {
    fHistoList_Eta_cuts_oneInRich_Outside[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_twoInRich_Outside");
  gDirectory->cd("Cuts_for_Eta_gg_twoInRich_Outside");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_twoInRich_Outside.size(); i++) {
    fHistoList_Eta_cuts_twoInRich_Outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_Outside.size(); i++) {
    fHistoList_Eta_cuts_Outside[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("Cuts_for_Eta_gg_Both");
  gDirectory->cd("Cuts_for_Eta_gg_Both");
  gDirectory->mkdir("Cuts_for_Eta_gg_zeroInRich_Both");
  gDirectory->cd("Cuts_for_Eta_gg_zeroInRich_Both");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_zeroInRich_Both.size(); i++) {
    fHistoList_Eta_cuts_zeroInRich_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_oneInRich_Both");
  gDirectory->cd("Cuts_for_Eta_gg_oneInRich_Both");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_oneInRich_Both.size(); i++) {
    fHistoList_Eta_cuts_oneInRich_Both[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Cuts_for_Eta_gg_twoInRich_Both");
  gDirectory->cd("Cuts_for_Eta_gg_twoInRich_Both");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_twoInRich_Both.size(); i++) {
    fHistoList_Eta_cuts_twoInRich_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_cuts_Both.size(); i++) {
    fHistoList_Eta_cuts_Both[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");


  gDirectory->mkdir("Eta_gg_Target");
  gDirectory->cd("Eta_gg_Target");

  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_Eta_all_Target.size(); i++) {
    fHistoList_Eta_all_Target[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_Eta_onetwo_Target.size(); i++) {
    fHistoList_Eta_onetwo_Target[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_Eta_two_Target.size(); i++) {
    fHistoList_Eta_two_Target[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");


  gDirectory->mkdir("Eta_gg_Outside");
  gDirectory->cd("Eta_gg_Outside");

  gDirectory->mkdir("all");
  gDirectory->cd("all");
  for (UInt_t i = 0; i < fHistoList_Eta_all_Outside.size(); i++) {
    fHistoList_Eta_all_Outside[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  for (UInt_t i = 0; i < fHistoList_Eta_onetwo_Outside.size(); i++) {
    fHistoList_Eta_onetwo_Outside[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("two");
  gDirectory->cd("two");
  for (UInt_t i = 0; i < fHistoList_Eta_two_Outside.size(); i++) {
    fHistoList_Eta_two_Outside[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");


  gDirectory->mkdir("Eta_gg_Both");
  gDirectory->cd("Eta_gg_Both");

  gDirectory->mkdir("all");
  gDirectory->cd("all");
  gDirectory->mkdir("multi_all_Both");
  gDirectory->cd("multi_all_Both");
  for (UInt_t i = 0; i < fHistoList_rap_pt_Eta_all_Both.size(); i++) {
    fHistoList_rap_pt_Eta_all_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_all_Both.size(); i++) {
    fHistoList_Eta_all_Both[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("onetwo");
  gDirectory->cd("onetwo");
  gDirectory->mkdir("multi_onetwo_Both");
  gDirectory->cd("multi_onetwo_Both");
  for (UInt_t i = 0; i < fHistoList_rap_pt_Eta_onetwo_Both.size(); i++) {
    fHistoList_rap_pt_Eta_onetwo_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_onetwo_Both.size(); i++) {
    fHistoList_Eta_onetwo_Both[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("two");
  gDirectory->cd("two");
  gDirectory->mkdir("multi_two_Both");
  gDirectory->cd("multi_two_Both");
  for (UInt_t i = 0; i < fHistoList_rap_pt_Eta_two_Both.size(); i++) {
    fHistoList_rap_pt_Eta_two_Both[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Eta_two_Both.size(); i++) {
    fHistoList_Eta_two_Both[i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->cd("..");


  for (UInt_t i = 0; i < fHistoList_Eta.size(); i++) {
    fHistoList_Eta[i]->Write();
  }


  gDirectory->cd("..");
}

void CbmKresEta::InitHistograms() {
  Particle_pull_px_Target =
    new TH1D("Particle_pull_px_Target",
             "Target track, pull Px: mc - reco; P_{x}(mc) - P_{x}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_px_Target);
  Particle_pull_py_Target =
    new TH1D("Particle_pull_py_Target",
             "Target track, pull Px: mc - reco; P_{y}(mc) - P_{y}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_py_Target);
  Particle_pull_pz_Target =
    new TH1D("Particle_pull_pz_Target",
             "Target track, pull Px: mc - reco; P_{z}(mc) - P_{z}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_pz_Target);
  Particle_pull_px_Outside =
    new TH1D("Particle_pull_px_Outside",
             "Outside track, pull Px: mc - reco; P_{x}(mc) - P_{x}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_px_Outside);
  Particle_pull_py_Outside =
    new TH1D("Particle_pull_py_Outside",
             "Outside track, pull Px: mc - reco; P_{y}(mc) - P_{y}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_py_Outside);
  Particle_pull_pz_Outside =
    new TH1D("Particle_pull_pz_Outside",
             "Outside track, pull Px: mc - reco; P_{z}(mc) - P_{z}(reco)",
             100,
             -0.5,
             0.5);
  fHistoList_Eta.push_back(Particle_pull_pz_Outside);
  Particle_pull_X_Outside =
    new TH1D("Particle_pull_X_Outside",
             "Outside track, pull Px: mc - reco; X(mc) - X(reco)",
             500,
             -5,
             5);
  fHistoList_Eta.push_back(Particle_pull_X_Outside);
  Particle_pull_Y_Outside =
    new TH1D("Particle_pull_Y_Outside",
             "Outside track, pull Px: mc - reco; Y(mc) - Y(reco)",
             500,
             -5,
             5);
  fHistoList_Eta.push_back(Particle_pull_Y_Outside);
  Particle_pull_Z_Outside =
    new TH1D("Particle_pull_Z_Outside",
             "Outside track, pull Px: mc - reco; Z(mc) - Z(reco)",
             500,
             -5,
             5);
  fHistoList_Eta.push_back(Particle_pull_Z_Outside);

  /////////// histograms to check Cuts => zeroInRich_Target
  GammasInvMass_fromEta_beforeCuts_zeroInRich_Target =
    new TH1D("GammasInvMass_fromEta_beforeCuts_zeroInRich_Target",
             "GammasInvMass_fromEta_beforeCuts_zeroInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasInvMass_fromEta_beforeCuts_zeroInRich_Target);
  GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target",
             "GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target);
  GammasMomentum_fromEta_beforeCuts_zeroInRich_Target =
    new TH1D("GammasMomentum_fromEta_beforeCuts_zeroInRich_Target",
             "GammasMomentum_fromEta_beforeCuts_zeroInRich_Target; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasMomentum_fromEta_beforeCuts_zeroInRich_Target);
  GammasRapidity_fromEta_beforeCuts_zeroInRich_Target =
    new TH1D("GammasRapidity_fromEta_beforeCuts_zeroInRich_Target",
             "GammasRapidity_fromEta_beforeCuts_zeroInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasRapidity_fromEta_beforeCuts_zeroInRich_Target);
  PlaneAngles_last_fromEta_zeroInRich_Target = new TH1D(
    "PlaneAngles_last_fromEta_zeroInRich_Target",
    "PlaneAngles_last_fromEta_zeroInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    PlaneAngles_last_fromEta_zeroInRich_Target);
  PlaneAngles_first_fromEta_zeroInRich_Target = new TH1D(
    "PlaneAngles_first_fromEta_zeroInRich_Target",
    "PlaneAngles_first_fromEta_zeroInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    PlaneAngles_first_fromEta_zeroInRich_Target);
  GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target",
             "GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target);
  GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target",
             "GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target);
  GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target",
    "GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target);
  GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target",
             "GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target);
  PlaneAngles_last_wrongpairs_zeroInRich_Target = new TH1D(
    "PlaneAngles_last_wrongpairs_zeroInRich_Target",
    "PlaneAngles_last_wrongpairs_zeroInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    PlaneAngles_last_wrongpairs_zeroInRich_Target);
  PlaneAngles_first_wrongpairs_zeroInRich_Target = new TH1D(
    "PlaneAngles_first_wrongpairs_zeroInRich_Target",
    "PlaneAngles_first_wrongpairs_zeroInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Target.push_back(
    PlaneAngles_first_wrongpairs_zeroInRich_Target);
  /////////// histograms to check Cuts => oneInRich_Target
  GammasInvMass_fromEta_beforeCuts_oneInRich_Target =
    new TH1D("GammasInvMass_fromEta_beforeCuts_oneInRich_Target",
             "GammasInvMass_fromEta_beforeCuts_oneInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasInvMass_fromEta_beforeCuts_oneInRich_Target);
  GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target",
             "GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target);
  GammasMomentum_fromEta_beforeCuts_oneInRich_Target =
    new TH1D("GammasMomentum_fromEta_beforeCuts_oneInRich_Target",
             "GammasMomentum_fromEta_beforeCuts_oneInRich_Target; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasMomentum_fromEta_beforeCuts_oneInRich_Target);
  GammasRapidity_fromEta_beforeCuts_oneInRich_Target =
    new TH1D("GammasRapidity_fromEta_beforeCuts_oneInRich_Target",
             "GammasRapidity_fromEta_beforeCuts_oneInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasRapidity_fromEta_beforeCuts_oneInRich_Target);
  PlaneAngles_last_fromEta_oneInRich_Target = new TH1D(
    "PlaneAngles_last_fromEta_oneInRich_Target",
    "PlaneAngles_last_fromEta_oneInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    PlaneAngles_last_fromEta_oneInRich_Target);
  PlaneAngles_first_fromEta_oneInRich_Target = new TH1D(
    "PlaneAngles_first_fromEta_oneInRich_Target",
    "PlaneAngles_first_fromEta_oneInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    PlaneAngles_first_fromEta_oneInRich_Target);
  GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target",
             "GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target);
  GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target",
             "GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target);
  GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target",
    "GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target);
  GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target",
             "GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target);
  PlaneAngles_last_wrongpairs_oneInRich_Target = new TH1D(
    "PlaneAngles_last_wrongpairs_oneInRich_Target",
    "PlaneAngles_last_wrongpairs_oneInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    PlaneAngles_last_wrongpairs_oneInRich_Target);
  PlaneAngles_first_wrongpairs_oneInRich_Target = new TH1D(
    "PlaneAngles_first_wrongpairs_oneInRich_Target",
    "PlaneAngles_first_wrongpairs_oneInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Target.push_back(
    PlaneAngles_first_wrongpairs_oneInRich_Target);
  /////////// histograms to check Cuts => twoInRich_Target
  GammasInvMass_fromEta_beforeCuts_twoInRich_Target =
    new TH1D("GammasInvMass_fromEta_beforeCuts_twoInRich_Target",
             "GammasInvMass_fromEta_beforeCuts_twoInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasInvMass_fromEta_beforeCuts_twoInRich_Target);
  GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target",
             "GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target);
  GammasMomentum_fromEta_beforeCuts_twoInRich_Target =
    new TH1D("GammasMomentum_fromEta_beforeCuts_twoInRich_Target",
             "GammasMomentum_fromEta_beforeCuts_twoInRich_Target; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasMomentum_fromEta_beforeCuts_twoInRich_Target);
  GammasRapidity_fromEta_beforeCuts_twoInRich_Target =
    new TH1D("GammasRapidity_fromEta_beforeCuts_twoInRich_Target",
             "GammasRapidity_fromEta_beforeCuts_twoInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasRapidity_fromEta_beforeCuts_twoInRich_Target);
  PlaneAngles_last_fromEta_twoInRich_Target = new TH1D(
    "PlaneAngles_last_fromEta_twoInRich_Target",
    "PlaneAngles_last_fromEta_twoInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    PlaneAngles_last_fromEta_twoInRich_Target);
  PlaneAngles_first_fromEta_twoInRich_Target = new TH1D(
    "PlaneAngles_first_fromEta_twoInRich_Target",
    "PlaneAngles_first_fromEta_twoInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    PlaneAngles_first_fromEta_twoInRich_Target);
  GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target",
             "GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target);
  GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target",
             "GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target);
  GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target",
    "GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target);
  GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target",
             "GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target);
  PlaneAngles_last_wrongpairs_twoInRich_Target = new TH1D(
    "PlaneAngles_last_wrongpairs_twoInRich_Target",
    "PlaneAngles_last_wrongpairs_twoInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    PlaneAngles_last_wrongpairs_twoInRich_Target);
  PlaneAngles_first_wrongpairs_twoInRich_Target = new TH1D(
    "PlaneAngles_first_wrongpairs_twoInRich_Target",
    "PlaneAngles_first_wrongpairs_twoInRich_Target; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Target.push_back(
    PlaneAngles_first_wrongpairs_twoInRich_Target);


  /////////// histograms to check Cuts => zeroInRich_Outside
  GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside",
             "GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside);
  GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside",
             "GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside);
  GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside",
             "GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside);
  GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside",
             "GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside);
  PlaneAngles_last_fromEta_zeroInRich_Outside = new TH1D(
    "PlaneAngles_last_fromEta_zeroInRich_Outside",
    "PlaneAngles_last_fromEta_zeroInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    PlaneAngles_last_fromEta_zeroInRich_Outside);
  PlaneAngles_first_fromEta_zeroInRich_Outside = new TH1D(
    "PlaneAngles_first_fromEta_zeroInRich_Outside",
    "PlaneAngles_first_fromEta_zeroInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    PlaneAngles_first_fromEta_zeroInRich_Outside);
  GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside",
             "GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside; "
             "invariant mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside);
  GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside",
             "GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside; "
             "opening angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside);
  GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside",
    "GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside);
  GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside = new TH1D(
    "GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside",
    "GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside; rapidity",
    400,
    0.,
    4.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside);
  PlaneAngles_last_wrongpairs_zeroInRich_Outside = new TH1D(
    "PlaneAngles_last_wrongpairs_zeroInRich_Outside",
    "PlaneAngles_last_wrongpairs_zeroInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    PlaneAngles_last_wrongpairs_zeroInRich_Outside);
  PlaneAngles_first_wrongpairs_zeroInRich_Outside = new TH1D(
    "PlaneAngles_first_wrongpairs_zeroInRich_Outside",
    "PlaneAngles_first_wrongpairs_zeroInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Outside.push_back(
    PlaneAngles_first_wrongpairs_zeroInRich_Outside);
  /////////// histograms to check Cuts => oneInRich_Outside
  GammasInvMass_fromEta_beforeCuts_oneInRich_Outside =
    new TH1D("GammasInvMass_fromEta_beforeCuts_oneInRich_Outside",
             "GammasInvMass_fromEta_beforeCuts_oneInRich_Outside; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasInvMass_fromEta_beforeCuts_oneInRich_Outside);
  GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside",
             "GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside);
  GammasMomentum_fromEta_beforeCuts_oneInRich_Outside =
    new TH1D("GammasMomentum_fromEta_beforeCuts_oneInRich_Outside",
             "GammasMomentum_fromEta_beforeCuts_oneInRich_Outside; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasMomentum_fromEta_beforeCuts_oneInRich_Outside);
  GammasRapidity_fromEta_beforeCuts_oneInRich_Outside =
    new TH1D("GammasRapidity_fromEta_beforeCuts_oneInRich_Outside",
             "GammasRapidity_fromEta_beforeCuts_oneInRich_Outside; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasRapidity_fromEta_beforeCuts_oneInRich_Outside);
  PlaneAngles_last_fromEta_oneInRich_Outside = new TH1D(
    "PlaneAngles_last_fromEta_oneInRich_Outside",
    "PlaneAngles_last_fromEta_oneInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    PlaneAngles_last_fromEta_oneInRich_Outside);
  PlaneAngles_first_fromEta_oneInRich_Outside = new TH1D(
    "PlaneAngles_first_fromEta_oneInRich_Outside",
    "PlaneAngles_first_fromEta_oneInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    PlaneAngles_first_fromEta_oneInRich_Outside);
  GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside",
             "GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside);
  GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside",
             "GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside);
  GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside",
    "GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside);
  GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside",
             "GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside);
  PlaneAngles_last_wrongpairs_oneInRich_Outside = new TH1D(
    "PlaneAngles_last_wrongpairs_oneInRich_Outside",
    "PlaneAngles_last_wrongpairs_oneInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    PlaneAngles_last_wrongpairs_oneInRich_Outside);
  PlaneAngles_first_wrongpairs_oneInRich_Outside = new TH1D(
    "PlaneAngles_first_wrongpairs_oneInRich_Outside",
    "PlaneAngles_first_wrongpairs_oneInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Outside.push_back(
    PlaneAngles_first_wrongpairs_oneInRich_Outside);
  /////////// histograms to check Cuts => twoInRich_Outside
  GammasInvMass_fromEta_beforeCuts_twoInRich_Outside =
    new TH1D("GammasInvMass_fromEta_beforeCuts_twoInRich_Outside",
             "GammasInvMass_fromEta_beforeCuts_twoInRich_Outside; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasInvMass_fromEta_beforeCuts_twoInRich_Outside);
  GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside",
             "GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside);
  GammasMomentum_fromEta_beforeCuts_twoInRich_Outside =
    new TH1D("GammasMomentum_fromEta_beforeCuts_twoInRich_Outside",
             "GammasMomentum_fromEta_beforeCuts_twoInRich_Outside; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasMomentum_fromEta_beforeCuts_twoInRich_Outside);
  GammasRapidity_fromEta_beforeCuts_twoInRich_Outside =
    new TH1D("GammasRapidity_fromEta_beforeCuts_twoInRich_Outside",
             "GammasRapidity_fromEta_beforeCuts_twoInRich_Outside; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasRapidity_fromEta_beforeCuts_twoInRich_Outside);
  PlaneAngles_last_fromEta_twoInRich_Outside = new TH1D(
    "PlaneAngles_last_fromEta_twoInRich_Outside",
    "PlaneAngles_last_fromEta_twoInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    PlaneAngles_last_fromEta_twoInRich_Outside);
  PlaneAngles_first_fromEta_twoInRich_Outside = new TH1D(
    "PlaneAngles_first_fromEta_twoInRich_Outside",
    "PlaneAngles_first_fromEta_twoInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    PlaneAngles_first_fromEta_twoInRich_Outside);
  GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside",
             "GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside);
  GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside",
             "GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside);
  GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside = new TH1D(
    "GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside",
    "GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside; p in GeV/c",
    400,
    0,
    4);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside);
  GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside",
             "GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside);
  PlaneAngles_last_wrongpairs_twoInRich_Outside = new TH1D(
    "PlaneAngles_last_wrongpairs_twoInRich_Outside",
    "PlaneAngles_last_wrongpairs_twoInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    PlaneAngles_last_wrongpairs_twoInRich_Outside);
  PlaneAngles_first_wrongpairs_twoInRich_Outside = new TH1D(
    "PlaneAngles_first_wrongpairs_twoInRich_Outside",
    "PlaneAngles_first_wrongpairs_twoInRich_Outside; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Outside.push_back(
    PlaneAngles_first_wrongpairs_twoInRich_Outside);


  /////////// histograms to check Cuts => zeroInRich_Both
  GammasInvMass_fromEta_beforeCuts_zeroInRich_Both =
    new TH1D("GammasInvMass_fromEta_beforeCuts_zeroInRich_Both",
             "GammasInvMass_fromEta_beforeCuts_zeroInRich_Both; invariant mass "
             "in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasInvMass_fromEta_beforeCuts_zeroInRich_Both);
  GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both",
             "GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both);
  GammasMomentum_fromEta_beforeCuts_zeroInRich_Both =
    new TH1D("GammasMomentum_fromEta_beforeCuts_zeroInRich_Both",
             "GammasMomentum_fromEta_beforeCuts_zeroInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasMomentum_fromEta_beforeCuts_zeroInRich_Both);
  GammasRapidity_fromEta_beforeCuts_zeroInRich_Both =
    new TH1D("GammasRapidity_fromEta_beforeCuts_zeroInRich_Both",
             "GammasRapidity_fromEta_beforeCuts_zeroInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasRapidity_fromEta_beforeCuts_zeroInRich_Both);
  PlaneAngles_last_fromEta_zeroInRich_Both = new TH1D(
    "PlaneAngles_last_fromEta_zeroInRich_Both",
    "PlaneAngles_last_fromEta_zeroInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    PlaneAngles_last_fromEta_zeroInRich_Both);
  PlaneAngles_first_fromEta_zeroInRich_Both = new TH1D(
    "PlaneAngles_first_fromEta_zeroInRich_Both",
    "PlaneAngles_first_fromEta_zeroInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    PlaneAngles_first_fromEta_zeroInRich_Both);
  GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both",
             "GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both);
  GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both",
             "GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both);
  GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both =
    new TH1D("GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both",
             "GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both);
  GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both",
             "GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both);
  PlaneAngles_last_wrongpairs_zeroInRich_Both = new TH1D(
    "PlaneAngles_last_wrongpairs_zeroInRich_Both",
    "PlaneAngles_last_wrongpairs_zeroInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    PlaneAngles_last_wrongpairs_zeroInRich_Both);
  PlaneAngles_first_wrongpairs_zeroInRich_Both = new TH1D(
    "PlaneAngles_first_wrongpairs_zeroInRich_Both",
    "PlaneAngles_first_wrongpairs_zeroInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_zeroInRich_Both.push_back(
    PlaneAngles_first_wrongpairs_zeroInRich_Both);
  /////////// histograms to check Cuts => oneInRich_Both
  GammasInvMass_fromEta_beforeCuts_oneInRich_Both =
    new TH1D("GammasInvMass_fromEta_beforeCuts_oneInRich_Both",
             "GammasInvMass_fromEta_beforeCuts_oneInRich_Both; invariant mass "
             "in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasInvMass_fromEta_beforeCuts_oneInRich_Both);
  GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both",
             "GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both; opening angle "
             "in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both);
  GammasMomentum_fromEta_beforeCuts_oneInRich_Both =
    new TH1D("GammasMomentum_fromEta_beforeCuts_oneInRich_Both",
             "GammasMomentum_fromEta_beforeCuts_oneInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasMomentum_fromEta_beforeCuts_oneInRich_Both);
  GammasRapidity_fromEta_beforeCuts_oneInRich_Both =
    new TH1D("GammasRapidity_fromEta_beforeCuts_oneInRich_Both",
             "GammasRapidity_fromEta_beforeCuts_oneInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasRapidity_fromEta_beforeCuts_oneInRich_Both);
  PlaneAngles_last_fromEta_oneInRich_Both = new TH1D(
    "PlaneAngles_last_fromEta_oneInRich_Both",
    "PlaneAngles_last_fromEta_oneInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_last_fromEta_oneInRich_Both);
  PlaneAngles_first_fromEta_oneInRich_Both = new TH1D(
    "PlaneAngles_first_fromEta_oneInRich_Both",
    "PlaneAngles_first_fromEta_oneInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_first_fromEta_oneInRich_Both);
  GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both",
             "GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both);
  GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both",
             "GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both);
  GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both =
    new TH1D("GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both",
             "GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both);
  GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both",
             "GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both);
  PlaneAngles_last_wrongpairs_oneInRich_Both = new TH1D(
    "PlaneAngles_last_wrongpairs_oneInRich_Both",
    "PlaneAngles_last_wrongpairs_oneInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_last_wrongpairs_oneInRich_Both);
  PlaneAngles_first_wrongpairs_oneInRich_Both = new TH1D(
    "PlaneAngles_first_wrongpairs_oneInRich_Both",
    "PlaneAngles_first_wrongpairs_oneInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_first_wrongpairs_oneInRich_Both);
  PlaneAngles_last_fromEta_afterCuts_oneInRich_Both =
    new TH1D("PlaneAngles_last_fromEta_afterCuts_oneInRich_Both",
             "PlaneAngles_last_fromEta_afterCuts_oneInRich_Both; #theta angle "
             "in degree;#",
             720,
             -1.,
             179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_last_fromEta_afterCuts_oneInRich_Both);
  PlaneAngles_first_fromEta_afterCuts_oneInRich_Both =
    new TH1D("PlaneAngles_first_fromEta_afterCuts_oneInRich_Both",
             "PlaneAngles_first_fromEta_afterCuts_oneInRich_Both; #theta angle "
             "in degree;#",
             720,
             -1.,
             179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_first_fromEta_afterCuts_oneInRich_Both);
  PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both =
    new TH1D("PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both",
             "PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both; #theta "
             "angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both);
  PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both =
    new TH1D("PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both",
             "PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both; #theta "
             "angle in degree;#",
             720,
             -1.,
             179.);
  fHistoList_Eta_cuts_oneInRich_Both.push_back(
    PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both);
  /////////// histograms to check Cuts => twoInRich_Both
  GammasInvMass_fromEta_beforeCuts_twoInRich_Both =
    new TH1D("GammasInvMass_fromEta_beforeCuts_twoInRich_Both",
             "GammasInvMass_fromEta_beforeCuts_twoInRich_Both; invariant mass "
             "in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasInvMass_fromEta_beforeCuts_twoInRich_Both);
  GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both =
    new TH1D("GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both",
             "GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both; opening angle "
             "in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both);
  GammasMomentum_fromEta_beforeCuts_twoInRich_Both =
    new TH1D("GammasMomentum_fromEta_beforeCuts_twoInRich_Both",
             "GammasMomentum_fromEta_beforeCuts_twoInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasMomentum_fromEta_beforeCuts_twoInRich_Both);
  GammasRapidity_fromEta_beforeCuts_twoInRich_Both =
    new TH1D("GammasRapidity_fromEta_beforeCuts_twoInRich_Both",
             "GammasRapidity_fromEta_beforeCuts_twoInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasRapidity_fromEta_beforeCuts_twoInRich_Both);
  PlaneAngles_last_fromEta_twoInRich_Both = new TH1D(
    "PlaneAngles_last_fromEta_twoInRich_Both",
    "PlaneAngles_last_fromEta_twoInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    PlaneAngles_last_fromEta_twoInRich_Both);
  PlaneAngles_first_fromEta_twoInRich_Both = new TH1D(
    "PlaneAngles_first_fromEta_twoInRich_Both",
    "PlaneAngles_first_fromEta_twoInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    PlaneAngles_first_fromEta_twoInRich_Both);
  GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both =
    new TH1D("GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both",
             "GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both; invariant "
             "mass in GeV/c^{2}",
             510,
             -0.01,
             0.5);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both);
  GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both =
    new TH1D("GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both",
             "GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both; opening "
             "angle in degree",
             300,
             -0.1,
             29.9);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both);
  GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both =
    new TH1D("GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both",
             "GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both; p in GeV/c",
             400,
             0,
             4);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both);
  GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both =
    new TH1D("GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both",
             "GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both; rapidity",
             400,
             0.,
             4.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both);
  PlaneAngles_last_wrongpairs_twoInRich_Both = new TH1D(
    "PlaneAngles_last_wrongpairs_twoInRich_Both",
    "PlaneAngles_last_wrongpairs_twoInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    PlaneAngles_last_wrongpairs_twoInRich_Both);
  PlaneAngles_first_wrongpairs_twoInRich_Both = new TH1D(
    "PlaneAngles_first_wrongpairs_twoInRich_Both",
    "PlaneAngles_first_wrongpairs_twoInRich_Both; #theta angle in degree;#",
    720,
    -1.,
    179.);
  fHistoList_Eta_cuts_twoInRich_Both.push_back(
    PlaneAngles_first_wrongpairs_twoInRich_Both);


  /////////////////////////////// TARGET //////////////////////////////////////
  //////// eta -> g + g    reconstruction case "all" and "Target"
  AngleBetweenGammas_trueEta_before_cuts_all_Target =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_all_Target",
             "AngleBetweenGammas_trueEta_before_cuts_all_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Target.push_back(
    AngleBetweenGammas_trueEta_before_cuts_all_Target);
  AngleBetweenGammas_wrongEta_before_cuts_all_Target =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_all_Target",
             "AngleBetweenGammas_wrongEta_before_cuts_all_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Target.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_all_Target);
  InvMass_GammaCombinations_all_Target = new TH1D(
    "InvMass_GammaCombinations_all_Target",
    "InvMass_GammaCombinations_all_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Target.push_back(InvMass_GammaCombinations_all_Target);
  Particles_PDG_all_Target = new TH1D("Particles_PDG_all_Target",
                                      "Particles_PDG_all_Target ; PDG",
                                      2300,
                                      0,
                                      2300);
  fHistoList_Eta_all_Target.push_back(Particles_PDG_all_Target);
  TrueEta_pt_vs_rap_all_Target =
    new TH2D("TrueEta_pt_vs_rap_all_Target",
             "TrueEta_pt_vs_rap_all_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Target.push_back(TrueEta_pt_vs_rap_all_Target);
  WrongEta_pt_vs_rap_all_Target =
    new TH2D("WrongEta_pt_vs_rap_all_Target",
             "WrongEta_pt_vs_rap_all_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Target.push_back(WrongEta_pt_vs_rap_all_Target);
  TrueEta_pt_vs_rap_est_all_Target =
    new TH2D("TrueEta_pt_vs_rap_est_all_Target",
             "TrueEta_pt_vs_rap_est_all_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Target.push_back(TrueEta_pt_vs_rap_est_all_Target);
  WrongEta_pt_vs_rap_est_all_Target =
    new TH2D("WrongEta_pt_vs_rap_est_all_Target",
             "WrongEta_pt_vs_rap_est_all_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Target.push_back(WrongEta_pt_vs_rap_est_all_Target);
  TrueEta_InvMass_after_cuts_all_Target = new TH1D(
    "TrueEta_InvMass_after_cuts_all_Target",
    "TrueEta_InvMass_after_cuts_all_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Target.push_back(TrueEta_InvMass_after_cuts_all_Target);
  WrongEta_InvMass_after_cuts_all_Target = new TH1D(
    "WrongEta_InvMass_after_cuts_all_Target",
    "WrongEta_InvMass_after_cuts_all_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Target.push_back(WrongEta_InvMass_after_cuts_all_Target);
  TrueEta_AngleBetweenGammas_after_cuts_all_Target =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_all_Target",
             "TrueEta_AngleBetweenGammas_after_cuts_all_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Target.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_all_Target);
  WrongEta_AngleBetweenGammas_after_cuts_all_Target =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_all_Target",
             "WrongEta_AngleBetweenGammas_after_cuts_all_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Target.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_all_Target);
  EMT_Eta_InvMass_all_Target =
    new TH1D("EMT_Eta_InvMass_all_Target",
             "EMT_Eta_InvMass_all_Target; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_all_Target.push_back(EMT_Eta_InvMass_all_Target);
  //////// eta -> g + g    reconstruction case "onetwo" and "Target"
  AngleBetweenGammas_trueEta_before_cuts_onetwo_Target =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_onetwo_Target",
             "AngleBetweenGammas_trueEta_before_cuts_onetwo_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Target.push_back(
    AngleBetweenGammas_trueEta_before_cuts_onetwo_Target);
  AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target",
             "AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Target.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target);
  InvMass_GammaCombinations_onetwo_Target = new TH1D(
    "InvMass_GammaCombinations_onetwo_Target",
    "InvMass_GammaCombinations_onetwo_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Target.push_back(
    InvMass_GammaCombinations_onetwo_Target);
  Particles_PDG_onetwo_Target = new TH1D("Particles_PDG_onetwo_Target",
                                         "Particles_PDG_onetwo_Target ; PDG",
                                         2300,
                                         0,
                                         2300);
  fHistoList_Eta_onetwo_Target.push_back(Particles_PDG_onetwo_Target);
  TrueEta_pt_vs_rap_onetwo_Target =
    new TH2D("TrueEta_pt_vs_rap_onetwo_Target",
             "TrueEta_pt_vs_rap_onetwo_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Target.push_back(TrueEta_pt_vs_rap_onetwo_Target);
  WrongEta_pt_vs_rap_onetwo_Target =
    new TH2D("WrongEta_pt_vs_rap_onetwo_Target",
             "WrongEta_pt_vs_rap_onetwo_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Target.push_back(WrongEta_pt_vs_rap_onetwo_Target);
  TrueEta_pt_vs_rap_est_onetwo_Target =
    new TH2D("TrueEta_pt_vs_rap_est_onetwo_Target",
             "TrueEta_pt_vs_rap_est_onetwo_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_onetwo_Target.push_back(TrueEta_pt_vs_rap_est_onetwo_Target);
  WrongEta_pt_vs_rap_est_onetwo_Target = new TH2D(
    "WrongEta_pt_vs_rap_est_onetwo_Target",
    "WrongEta_pt_vs_rap_est_onetwo_Target; rapidity y; p_{t} in GeV/c ",
    10,
    0.,
    4.,
    5,
    0.,
    2.);
  fHistoList_Eta_onetwo_Target.push_back(WrongEta_pt_vs_rap_est_onetwo_Target);
  TrueEta_InvMass_after_cuts_onetwo_Target = new TH1D(
    "TrueEta_InvMass_after_cuts_onetwo_Target",
    "TrueEta_InvMass_after_cuts_onetwo_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Target.push_back(
    TrueEta_InvMass_after_cuts_onetwo_Target);
  WrongEta_InvMass_after_cuts_onetwo_Target = new TH1D(
    "WrongEta_InvMass_after_cuts_onetwo_Target",
    "WrongEta_InvMass_after_cuts_onetwo_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Target.push_back(
    WrongEta_InvMass_after_cuts_onetwo_Target);
  TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target",
             "TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Target.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target);
  WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target",
             "WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Target.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target);
  EMT_Eta_InvMass_onetwo_Target =
    new TH1D("EMT_Eta_InvMass_onetwo_Target",
             "EMT_Eta_InvMass_onetwo_Target; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_onetwo_Target.push_back(EMT_Eta_InvMass_onetwo_Target);
  //////// eta -> g + g    reconstruction case "two" and "Target"
  AngleBetweenGammas_trueEta_before_cuts_two_Target =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_two_Target",
             "AngleBetweenGammas_trueEta_before_cuts_two_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Target.push_back(
    AngleBetweenGammas_trueEta_before_cuts_two_Target);
  AngleBetweenGammas_wrongEta_before_cuts_two_Target =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_two_Target",
             "AngleBetweenGammas_wrongEta_before_cuts_two_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Target.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_two_Target);
  InvMass_GammaCombinations_two_Target = new TH1D(
    "InvMass_GammaCombinations_two_Target",
    "InvMass_GammaCombinations_two_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Target.push_back(InvMass_GammaCombinations_two_Target);
  Particles_PDG_two_Target = new TH1D("Particles_PDG_two_Target",
                                      "Particles_PDG_two_Target ; PDG",
                                      2300,
                                      0,
                                      2300);
  fHistoList_Eta_two_Target.push_back(Particles_PDG_two_Target);
  TrueEta_pt_vs_rap_two_Target =
    new TH2D("TrueEta_pt_vs_rap_two_Target",
             "TrueEta_pt_vs_rap_two_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Target.push_back(TrueEta_pt_vs_rap_two_Target);
  WrongEta_pt_vs_rap_two_Target =
    new TH2D("WrongEta_pt_vs_rap_two_Target",
             "WrongEta_pt_vs_rap_two_Target; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Target.push_back(WrongEta_pt_vs_rap_two_Target);
  TrueEta_pt_vs_rap_est_two_Target =
    new TH2D("TrueEta_pt_vs_rap_est_two_Target",
             "TrueEta_pt_vs_rap_est_two_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Target.push_back(TrueEta_pt_vs_rap_est_two_Target);
  WrongEta_pt_vs_rap_est_two_Target =
    new TH2D("WrongEta_pt_vs_rap_est_two_Target",
             "WrongEta_pt_vs_rap_est_two_Target; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Target.push_back(WrongEta_pt_vs_rap_est_two_Target);
  TrueEta_InvMass_after_cuts_two_Target = new TH1D(
    "TrueEta_InvMass_after_cuts_two_Target",
    "TrueEta_InvMass_after_cuts_two_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Target.push_back(TrueEta_InvMass_after_cuts_two_Target);
  WrongEta_InvMass_after_cuts_two_Target = new TH1D(
    "WrongEta_InvMass_after_cuts_two_Target",
    "WrongEta_InvMass_after_cuts_two_Target; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Target.push_back(WrongEta_InvMass_after_cuts_two_Target);
  TrueEta_AngleBetweenGammas_after_cuts_two_Target =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_two_Target",
             "TrueEta_AngleBetweenGammas_after_cuts_two_Target (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Target.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_two_Target);
  WrongEta_AngleBetweenGammas_after_cuts_two_Target =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_two_Target",
             "WrongEta_AngleBetweenGammas_after_cuts_two_Target (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Target.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_two_Target);
  EMT_Eta_InvMass_two_Target =
    new TH1D("EMT_Eta_InvMass_two_Target",
             "EMT_Eta_InvMass_two_Target; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_two_Target.push_back(EMT_Eta_InvMass_two_Target);
  /////////////////////////////// TARGET (END) //////////////////////////////////////


  /////////////////////////////// OUTSIDE //////////////////////////////////////
  //////// eta -> g + g    reconstruction case "all" and "Outside"
  AngleBetweenGammas_trueEta_before_cuts_all_Outside =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_all_Outside",
             "AngleBetweenGammas_trueEta_before_cuts_all_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Outside.push_back(
    AngleBetweenGammas_trueEta_before_cuts_all_Outside);
  AngleBetweenGammas_wrongEta_before_cuts_all_Outside =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_all_Outside",
             "AngleBetweenGammas_wrongEta_before_cuts_all_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Outside.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_all_Outside);
  InvMass_GammaCombinations_all_Outside = new TH1D(
    "InvMass_GammaCombinations_all_Outside",
    "InvMass_GammaCombinations_all_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Outside.push_back(InvMass_GammaCombinations_all_Outside);
  Particles_PDG_all_Outside = new TH1D("Particles_PDG_all_Outside",
                                       "Particles_PDG_all_Outside ; PDG",
                                       2300,
                                       0,
                                       2300);
  fHistoList_Eta_all_Outside.push_back(Particles_PDG_all_Outside);
  TrueEta_pt_vs_rap_all_Outside =
    new TH2D("TrueEta_pt_vs_rap_all_Outside",
             "TrueEta_pt_vs_rap_all_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Outside.push_back(TrueEta_pt_vs_rap_all_Outside);
  WrongEta_pt_vs_rap_all_Outside =
    new TH2D("WrongEta_pt_vs_rap_all_Outside",
             "WrongEta_pt_vs_rap_all_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Outside.push_back(WrongEta_pt_vs_rap_all_Outside);
  TrueEta_pt_vs_rap_est_all_Outside =
    new TH2D("TrueEta_pt_vs_rap_est_all_Outside",
             "TrueEta_pt_vs_rap_est_all_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Outside.push_back(TrueEta_pt_vs_rap_est_all_Outside);
  WrongEta_pt_vs_rap_est_all_Outside =
    new TH2D("WrongEta_pt_vs_rap_est_all_Outside",
             "WrongEta_pt_vs_rap_est_all_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Outside.push_back(WrongEta_pt_vs_rap_est_all_Outside);
  TrueEta_InvMass_after_cuts_all_Outside = new TH1D(
    "TrueEta_InvMass_after_cuts_all_Outside",
    "TrueEta_InvMass_after_cuts_all_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Outside.push_back(TrueEta_InvMass_after_cuts_all_Outside);
  WrongEta_InvMass_after_cuts_all_Outside = new TH1D(
    "WrongEta_InvMass_after_cuts_all_Outside",
    "WrongEta_InvMass_after_cuts_all_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Outside.push_back(WrongEta_InvMass_after_cuts_all_Outside);
  TrueEta_AngleBetweenGammas_after_cuts_all_Outside =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_all_Outside",
             "TrueEta_AngleBetweenGammas_after_cuts_all_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Outside.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_all_Outside);
  WrongEta_AngleBetweenGammas_after_cuts_all_Outside =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_all_Outside",
             "WrongEta_AngleBetweenGammas_after_cuts_all_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Outside.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_all_Outside);
  EMT_Eta_InvMass_all_Outside =
    new TH1D("EMT_Eta_InvMass_all_Outside",
             "EMT_Eta_InvMass_all_Outside; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_all_Outside.push_back(EMT_Eta_InvMass_all_Outside);
  //////// eta -> g + g    reconstruction case "onetwo" and "Outside"
  AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside",
             "AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Outside.push_back(
    AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside);
  AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside",
             "AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Outside.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside);
  InvMass_GammaCombinations_onetwo_Outside = new TH1D(
    "InvMass_GammaCombinations_onetwo_Outside",
    "InvMass_GammaCombinations_onetwo_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Outside.push_back(
    InvMass_GammaCombinations_onetwo_Outside);
  Particles_PDG_onetwo_Outside = new TH1D("Particles_PDG_onetwo_Outside",
                                          "Particles_PDG_onetwo_Outside ; PDG",
                                          2300,
                                          0,
                                          2300);
  fHistoList_Eta_onetwo_Outside.push_back(Particles_PDG_onetwo_Outside);
  TrueEta_pt_vs_rap_onetwo_Outside =
    new TH2D("TrueEta_pt_vs_rap_onetwo_Outside",
             "TrueEta_pt_vs_rap_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Outside.push_back(TrueEta_pt_vs_rap_onetwo_Outside);
  WrongEta_pt_vs_rap_onetwo_Outside =
    new TH2D("WrongEta_pt_vs_rap_onetwo_Outside",
             "WrongEta_pt_vs_rap_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Outside.push_back(WrongEta_pt_vs_rap_onetwo_Outside);
  TrueEta_pt_vs_rap_est_onetwo_Outside = new TH2D(
    "TrueEta_pt_vs_rap_est_onetwo_Outside",
    "TrueEta_pt_vs_rap_est_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
    10,
    0.,
    4.,
    5,
    0.,
    2.);
  fHistoList_Eta_onetwo_Outside.push_back(TrueEta_pt_vs_rap_est_onetwo_Outside);
  WrongEta_pt_vs_rap_est_onetwo_Outside = new TH2D(
    "WrongEta_pt_vs_rap_est_onetwo_Outside",
    "WrongEta_pt_vs_rap_est_onetwo_Outside; rapidity y; p_{t} in GeV/c ",
    10,
    0.,
    4.,
    5,
    0.,
    2.);
  fHistoList_Eta_onetwo_Outside.push_back(
    WrongEta_pt_vs_rap_est_onetwo_Outside);
  TrueEta_InvMass_after_cuts_onetwo_Outside = new TH1D(
    "TrueEta_InvMass_after_cuts_onetwo_Outside",
    "TrueEta_InvMass_after_cuts_onetwo_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Outside.push_back(
    TrueEta_InvMass_after_cuts_onetwo_Outside);
  WrongEta_InvMass_after_cuts_onetwo_Outside = new TH1D(
    "WrongEta_InvMass_after_cuts_onetwo_Outside",
    "WrongEta_InvMass_after_cuts_onetwo_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Outside.push_back(
    WrongEta_InvMass_after_cuts_onetwo_Outside);
  TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside",
             "TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Outside.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside);
  WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside",
             "WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Outside.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside);
  EMT_Eta_InvMass_onetwo_Outside =
    new TH1D("EMT_Eta_InvMass_onetwo_Outside",
             "EMT_Eta_InvMass_onetwo_Outside; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_onetwo_Outside.push_back(EMT_Eta_InvMass_onetwo_Outside);
  //////// eta -> g + g    reconstruction case "two" and "Outside"
  AngleBetweenGammas_trueEta_before_cuts_two_Outside =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_two_Outside",
             "AngleBetweenGammas_trueEta_before_cuts_two_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Outside.push_back(
    AngleBetweenGammas_trueEta_before_cuts_two_Outside);
  AngleBetweenGammas_wrongEta_before_cuts_two_Outside =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_two_Outside",
             "AngleBetweenGammas_wrongEta_before_cuts_two_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Outside.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_two_Outside);
  InvMass_GammaCombinations_two_Outside = new TH1D(
    "InvMass_GammaCombinations_two_Outside",
    "InvMass_GammaCombinations_two_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Outside.push_back(InvMass_GammaCombinations_two_Outside);
  Particles_PDG_two_Outside = new TH1D("Particles_PDG_two_Outside",
                                       "Particles_PDG_two_Outside ; PDG",
                                       2300,
                                       0,
                                       2300);
  fHistoList_Eta_two_Outside.push_back(Particles_PDG_two_Outside);
  TrueEta_pt_vs_rap_two_Outside =
    new TH2D("TrueEta_pt_vs_rap_two_Outside",
             "TrueEta_pt_vs_rap_two_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Outside.push_back(TrueEta_pt_vs_rap_two_Outside);
  WrongEta_pt_vs_rap_two_Outside =
    new TH2D("WrongEta_pt_vs_rap_two_Outside",
             "WrongEta_pt_vs_rap_two_Outside; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Outside.push_back(WrongEta_pt_vs_rap_two_Outside);
  TrueEta_pt_vs_rap_est_two_Outside =
    new TH2D("TrueEta_pt_vs_rap_est_two_Outside",
             "TrueEta_pt_vs_rap_est_two_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Outside.push_back(TrueEta_pt_vs_rap_est_two_Outside);
  WrongEta_pt_vs_rap_est_two_Outside =
    new TH2D("WrongEta_pt_vs_rap_est_two_Outside",
             "WrongEta_pt_vs_rap_est_two_Outside; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Outside.push_back(WrongEta_pt_vs_rap_est_two_Outside);
  TrueEta_InvMass_after_cuts_two_Outside = new TH1D(
    "TrueEta_InvMass_after_cuts_two_Outside",
    "TrueEta_InvMass_after_cuts_two_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Outside.push_back(TrueEta_InvMass_after_cuts_two_Outside);
  WrongEta_InvMass_after_cuts_two_Outside = new TH1D(
    "WrongEta_InvMass_after_cuts_two_Outside",
    "WrongEta_InvMass_after_cuts_two_Outside; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Outside.push_back(WrongEta_InvMass_after_cuts_two_Outside);
  TrueEta_AngleBetweenGammas_after_cuts_two_Outside =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_two_Outside",
             "TrueEta_AngleBetweenGammas_after_cuts_two_Outside (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Outside.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_two_Outside);
  WrongEta_AngleBetweenGammas_after_cuts_two_Outside =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_two_Outside",
             "WrongEta_AngleBetweenGammas_after_cuts_two_Outside (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Outside.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_two_Outside);
  EMT_Eta_InvMass_two_Outside =
    new TH1D("EMT_Eta_InvMass_two_Outside",
             "EMT_Eta_InvMass_two_Outside; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_two_Outside.push_back(EMT_Eta_InvMass_two_Outside);
  /////////////////////////////// OUTSIDE (END) //////////////////////////////////////


  /////////////////////////////// BOTH //////////////////////////////////////
  //////// eta -> g + g    reconstruction case "all" and "Both"
  AngleBetweenGammas_trueEta_before_cuts_all_Both =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_all_Both",
             "AngleBetweenGammas_trueEta_before_cuts_all_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Both.push_back(
    AngleBetweenGammas_trueEta_before_cuts_all_Both);
  AngleBetweenGammas_wrongEta_before_cuts_all_Both =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_all_Both",
             "AngleBetweenGammas_wrongEta_before_cuts_all_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Both.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_all_Both);
  InvMass_GammaCombinations_all_Both = new TH1D(
    "InvMass_GammaCombinations_all_Both",
    "InvMass_GammaCombinations_all_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Both.push_back(InvMass_GammaCombinations_all_Both);
  Particles_PDG_all_Both = new TH1D(
    "Particles_PDG_all_Both", "Particles_PDG_all_Both ; PDG", 2300, 0, 2300);
  fHistoList_Eta_all_Both.push_back(Particles_PDG_all_Both);
  TrueEta_pt_vs_rap_all_Both =
    new TH2D("TrueEta_pt_vs_rap_all_Both",
             "TrueEta_pt_vs_rap_all_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Both.push_back(TrueEta_pt_vs_rap_all_Both);
  WrongEta_pt_vs_rap_all_Both =
    new TH2D("WrongEta_pt_vs_rap_all_Both",
             "WrongEta_pt_vs_rap_all_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_all_Both.push_back(WrongEta_pt_vs_rap_all_Both);
  TrueEta_pt_vs_rap_est_all_Both =
    new TH2D("TrueEta_pt_vs_rap_est_all_Both",
             "TrueEta_pt_vs_rap_est_all_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Both.push_back(TrueEta_pt_vs_rap_est_all_Both);
  WrongEta_pt_vs_rap_est_all_Both =
    new TH2D("WrongEta_pt_vs_rap_est_all_Both",
             "WrongEta_pt_vs_rap_est_all_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_all_Both.push_back(WrongEta_pt_vs_rap_est_all_Both);
  TrueEta_InvMass_after_cuts_all_Both = new TH1D(
    "TrueEta_InvMass_after_cuts_all_Both",
    "TrueEta_InvMass_after_cuts_all_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Both.push_back(TrueEta_InvMass_after_cuts_all_Both);
  WrongEta_InvMass_after_cuts_all_Both = new TH1D(
    "WrongEta_InvMass_after_cuts_all_Both",
    "WrongEta_InvMass_after_cuts_all_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_all_Both.push_back(WrongEta_InvMass_after_cuts_all_Both);
  TrueEta_AngleBetweenGammas_after_cuts_all_Both =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_all_Both",
             "TrueEta_AngleBetweenGammas_after_cuts_all_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Both.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_all_Both);
  WrongEta_AngleBetweenGammas_after_cuts_all_Both =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_all_Both",
             "WrongEta_AngleBetweenGammas_after_cuts_all_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_all_Both.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_all_Both);
  EMT_Eta_InvMass_all_Both =
    new TH1D("EMT_Eta_InvMass_all_Both",
             "EMT_Eta_InvMass_all_Both; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_all_Both.push_back(EMT_Eta_InvMass_all_Both);
  ///////////// multidifferential analysis case "all" and "Both"
  fHistoList_rap_pt_Eta_all_Both.push_back(TrueEta_pt_vs_rap_est_all_Both);
  multi_InvMass_Eta_all_Both_1 =
    new TH1D("multi_InvMass_Eta_all_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_1);
  multi_InvMass_Eta_all_Both_2 =
    new TH1D("multi_InvMass_Eta_all_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_2);
  multi_InvMass_Eta_all_Both_3 =
    new TH1D("multi_InvMass_Eta_all_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_3);
  multi_InvMass_Eta_all_Both_4 =
    new TH1D("multi_InvMass_Eta_all_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_4);
  multi_InvMass_Eta_all_Both_5 =
    new TH1D("multi_InvMass_Eta_all_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_5);
  multi_InvMass_Eta_all_Both_6 =
    new TH1D("multi_InvMass_Eta_all_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_6);
  multi_InvMass_Eta_all_Both_7 =
    new TH1D("multi_InvMass_Eta_all_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_7);
  multi_InvMass_Eta_all_Both_8 =
    new TH1D("multi_InvMass_Eta_all_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_8);
  multi_InvMass_Eta_all_Both_9 =
    new TH1D("multi_InvMass_Eta_all_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_9);
  multi_InvMass_Eta_all_Both_10 =
    new TH1D("multi_InvMass_Eta_all_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_10);
  multi_InvMass_Eta_all_Both_11 =
    new TH1D("multi_InvMass_Eta_all_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_11);
  multi_InvMass_Eta_all_Both_12 =
    new TH1D("multi_InvMass_Eta_all_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_12);
  multi_InvMass_Eta_all_Both_13 =
    new TH1D("multi_InvMass_Eta_all_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_13);
  multi_InvMass_Eta_all_Both_14 =
    new TH1D("multi_InvMass_Eta_all_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_14);
  multi_InvMass_Eta_all_Both_15 =
    new TH1D("multi_InvMass_Eta_all_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_15);
  multi_InvMass_Eta_all_Both_16 =
    new TH1D("multi_InvMass_Eta_all_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_16);
  multi_InvMass_Eta_all_Both_17 =
    new TH1D("multi_InvMass_Eta_all_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_InvMass_Eta_all_Both_17);
  multi_EMT_Eta_all_Both_1 =
    new TH1D("multi_EMT_Eta_all_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_1);
  multi_EMT_Eta_all_Both_2 =
    new TH1D("multi_EMT_Eta_all_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_2);
  multi_EMT_Eta_all_Both_3 =
    new TH1D("multi_EMT_Eta_all_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_3);
  multi_EMT_Eta_all_Both_4 =
    new TH1D("multi_EMT_Eta_all_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_4);
  multi_EMT_Eta_all_Both_5 =
    new TH1D("multi_EMT_Eta_all_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_5);
  multi_EMT_Eta_all_Both_6 =
    new TH1D("multi_EMT_Eta_all_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_6);
  multi_EMT_Eta_all_Both_7 =
    new TH1D("multi_EMT_Eta_all_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_7);
  multi_EMT_Eta_all_Both_8 =
    new TH1D("multi_EMT_Eta_all_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_8);
  multi_EMT_Eta_all_Both_9 =
    new TH1D("multi_EMT_Eta_all_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_9);
  multi_EMT_Eta_all_Both_10 =
    new TH1D("multi_EMT_Eta_all_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_10);
  multi_EMT_Eta_all_Both_11 =
    new TH1D("multi_EMT_Eta_all_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_11);
  multi_EMT_Eta_all_Both_12 =
    new TH1D("multi_EMT_Eta_all_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_12);
  multi_EMT_Eta_all_Both_13 =
    new TH1D("multi_EMT_Eta_all_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_13);
  multi_EMT_Eta_all_Both_14 =
    new TH1D("multi_EMT_Eta_all_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_14);
  multi_EMT_Eta_all_Both_15 =
    new TH1D("multi_EMT_Eta_all_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_15);
  multi_EMT_Eta_all_Both_16 =
    new TH1D("multi_EMT_Eta_all_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_16);
  multi_EMT_Eta_all_Both_17 =
    new TH1D("multi_EMT_Eta_all_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_all_Both.push_back(multi_EMT_Eta_all_Both_17);


  //////// eta -> g + g    reconstruction case "onetwo" and "Both"
  AngleBetweenGammas_trueEta_before_cuts_onetwo_Both =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_onetwo_Both",
             "AngleBetweenGammas_trueEta_before_cuts_onetwo_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Both.push_back(
    AngleBetweenGammas_trueEta_before_cuts_onetwo_Both);
  AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both",
             "AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Both.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both);
  InvMass_GammaCombinations_onetwo_Both = new TH1D(
    "InvMass_GammaCombinations_onetwo_Both",
    "InvMass_GammaCombinations_onetwo_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Both.push_back(InvMass_GammaCombinations_onetwo_Both);
  Particles_PDG_onetwo_Both = new TH1D("Particles_PDG_onetwo_Both",
                                       "Particles_PDG_onetwo_Both ; PDG",
                                       2300,
                                       0,
                                       2300);
  fHistoList_Eta_onetwo_Both.push_back(Particles_PDG_onetwo_Both);
  TrueEta_pt_vs_rap_onetwo_Both =
    new TH2D("TrueEta_pt_vs_rap_onetwo_Both",
             "TrueEta_pt_vs_rap_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Both.push_back(TrueEta_pt_vs_rap_onetwo_Both);
  WrongEta_pt_vs_rap_onetwo_Both =
    new TH2D("WrongEta_pt_vs_rap_onetwo_Both",
             "WrongEta_pt_vs_rap_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_onetwo_Both.push_back(WrongEta_pt_vs_rap_onetwo_Both);
  TrueEta_pt_vs_rap_est_onetwo_Both =
    new TH2D("TrueEta_pt_vs_rap_est_onetwo_Both",
             "TrueEta_pt_vs_rap_est_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_onetwo_Both.push_back(TrueEta_pt_vs_rap_est_onetwo_Both);
  WrongEta_pt_vs_rap_est_onetwo_Both =
    new TH2D("WrongEta_pt_vs_rap_est_onetwo_Both",
             "WrongEta_pt_vs_rap_est_onetwo_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_onetwo_Both.push_back(WrongEta_pt_vs_rap_est_onetwo_Both);
  TrueEta_InvMass_after_cuts_onetwo_Both = new TH1D(
    "TrueEta_InvMass_after_cuts_onetwo_Both",
    "TrueEta_InvMass_after_cuts_onetwo_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Both.push_back(TrueEta_InvMass_after_cuts_onetwo_Both);
  WrongEta_InvMass_after_cuts_onetwo_Both = new TH1D(
    "WrongEta_InvMass_after_cuts_onetwo_Both",
    "WrongEta_InvMass_after_cuts_onetwo_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_onetwo_Both.push_back(WrongEta_InvMass_after_cuts_onetwo_Both);
  TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both",
             "TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Both.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both);
  WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both",
             "WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_onetwo_Both.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both);
  EMT_Eta_InvMass_onetwo_Both =
    new TH1D("EMT_Eta_InvMass_onetwo_Both",
             "EMT_Eta_InvMass_onetwo_Both; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_onetwo_Both.push_back(EMT_Eta_InvMass_onetwo_Both);
  ///////////// multidifferential analysis case "onetwo" and "Both"
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(
    TrueEta_pt_vs_rap_est_onetwo_Both);
  multi_InvMass_Eta_onetwo_Both_1 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_1);
  multi_InvMass_Eta_onetwo_Both_2 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_2);
  multi_InvMass_Eta_onetwo_Both_3 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_3);
  multi_InvMass_Eta_onetwo_Both_4 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_4);
  multi_InvMass_Eta_onetwo_Both_5 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_5);
  multi_InvMass_Eta_onetwo_Both_6 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_6);
  multi_InvMass_Eta_onetwo_Both_7 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_7);
  multi_InvMass_Eta_onetwo_Both_8 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_8);
  multi_InvMass_Eta_onetwo_Both_9 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_9);
  multi_InvMass_Eta_onetwo_Both_10 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_10);
  multi_InvMass_Eta_onetwo_Both_11 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_11);
  multi_InvMass_Eta_onetwo_Both_12 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_12);
  multi_InvMass_Eta_onetwo_Both_13 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_13);
  multi_InvMass_Eta_onetwo_Both_14 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_14);
  multi_InvMass_Eta_onetwo_Both_15 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_15);
  multi_InvMass_Eta_onetwo_Both_16 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_16);
  multi_InvMass_Eta_onetwo_Both_17 =
    new TH1D("multi_InvMass_Eta_onetwo_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_InvMass_Eta_onetwo_Both_17);
  multi_EMT_Eta_onetwo_Both_1 =
    new TH1D("multi_EMT_Eta_onetwo_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_1);
  multi_EMT_Eta_onetwo_Both_2 =
    new TH1D("multi_EMT_Eta_onetwo_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_2);
  multi_EMT_Eta_onetwo_Both_3 =
    new TH1D("multi_EMT_Eta_onetwo_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_3);
  multi_EMT_Eta_onetwo_Both_4 =
    new TH1D("multi_EMT_Eta_onetwo_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_4);
  multi_EMT_Eta_onetwo_Both_5 =
    new TH1D("multi_EMT_Eta_onetwo_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_5);
  multi_EMT_Eta_onetwo_Both_6 =
    new TH1D("multi_EMT_Eta_onetwo_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_6);
  multi_EMT_Eta_onetwo_Both_7 =
    new TH1D("multi_EMT_Eta_onetwo_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_7);
  multi_EMT_Eta_onetwo_Both_8 =
    new TH1D("multi_EMT_Eta_onetwo_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_8);
  multi_EMT_Eta_onetwo_Both_9 =
    new TH1D("multi_EMT_Eta_onetwo_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_9);
  multi_EMT_Eta_onetwo_Both_10 =
    new TH1D("multi_EMT_Eta_onetwo_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_10);
  multi_EMT_Eta_onetwo_Both_11 =
    new TH1D("multi_EMT_Eta_onetwo_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_11);
  multi_EMT_Eta_onetwo_Both_12 =
    new TH1D("multi_EMT_Eta_onetwo_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_12);
  multi_EMT_Eta_onetwo_Both_13 =
    new TH1D("multi_EMT_Eta_onetwo_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_13);
  multi_EMT_Eta_onetwo_Both_14 =
    new TH1D("multi_EMT_Eta_onetwo_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_14);
  multi_EMT_Eta_onetwo_Both_15 =
    new TH1D("multi_EMT_Eta_onetwo_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_15);
  multi_EMT_Eta_onetwo_Both_16 =
    new TH1D("multi_EMT_Eta_onetwo_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_16);
  multi_EMT_Eta_onetwo_Both_17 =
    new TH1D("multi_EMT_Eta_onetwo_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_onetwo_Both.push_back(multi_EMT_Eta_onetwo_Both_17);


  //////// eta -> g + g    reconstruction case "two" and "Both"
  AngleBetweenGammas_trueEta_before_cuts_two_Both =
    new TH1D("AngleBetweenGammas_trueEta_before_cuts_two_Both",
             "AngleBetweenGammas_trueEta_before_cuts_two_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Both.push_back(
    AngleBetweenGammas_trueEta_before_cuts_two_Both);
  AngleBetweenGammas_wrongEta_before_cuts_two_Both =
    new TH1D("AngleBetweenGammas_wrongEta_before_cuts_two_Both",
             "AngleBetweenGammas_wrongEta_before_cuts_two_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Both.push_back(
    AngleBetweenGammas_wrongEta_before_cuts_two_Both);
  InvMass_GammaCombinations_two_Both = new TH1D(
    "InvMass_GammaCombinations_two_Both",
    "InvMass_GammaCombinations_two_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Both.push_back(InvMass_GammaCombinations_two_Both);
  Particles_PDG_two_Both = new TH1D(
    "Particles_PDG_two_Both", "Particles_PDG_two_Both ; PDG", 2300, 0, 2300);
  fHistoList_Eta_two_Both.push_back(Particles_PDG_two_Both);
  TrueEta_pt_vs_rap_two_Both =
    new TH2D("TrueEta_pt_vs_rap_two_Both",
             "TrueEta_pt_vs_rap_two_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Both.push_back(TrueEta_pt_vs_rap_two_Both);
  WrongEta_pt_vs_rap_two_Both =
    new TH2D("WrongEta_pt_vs_rap_two_Both",
             "WrongEta_pt_vs_rap_two_Both; rapidity y; p_{t} in GeV/c ",
             90,
             -2.,
             7.,
             60,
             -1.,
             5.);
  fHistoList_Eta_two_Both.push_back(WrongEta_pt_vs_rap_two_Both);
  TrueEta_pt_vs_rap_est_two_Both =
    new TH2D("TrueEta_pt_vs_rap_est_two_Both",
             "TrueEta_pt_vs_rap_est_two_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Both.push_back(TrueEta_pt_vs_rap_est_two_Both);
  WrongEta_pt_vs_rap_est_two_Both =
    new TH2D("WrongEta_pt_vs_rap_est_two_Both",
             "WrongEta_pt_vs_rap_est_two_Both; rapidity y; p_{t} in GeV/c ",
             10,
             0.,
             4.,
             5,
             0.,
             2.);
  fHistoList_Eta_two_Both.push_back(WrongEta_pt_vs_rap_est_two_Both);
  TrueEta_InvMass_after_cuts_two_Both = new TH1D(
    "TrueEta_InvMass_after_cuts_two_Both",
    "TrueEta_InvMass_after_cuts_two_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Both.push_back(TrueEta_InvMass_after_cuts_two_Both);
  WrongEta_InvMass_after_cuts_two_Both = new TH1D(
    "WrongEta_InvMass_after_cuts_two_Both",
    "WrongEta_InvMass_after_cuts_two_Both; invariant mass in GeV/c^{2};#",
    200,
    0.0,
    2.0);
  fHistoList_Eta_two_Both.push_back(WrongEta_InvMass_after_cuts_two_Both);
  TrueEta_AngleBetweenGammas_after_cuts_two_Both =
    new TH1D("TrueEta_AngleBetweenGammas_after_cuts_two_Both",
             "TrueEta_AngleBetweenGammas_after_cuts_two_Both (between "
             "#gamma#gamma from #eta); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Both.push_back(
    TrueEta_AngleBetweenGammas_after_cuts_two_Both);
  WrongEta_AngleBetweenGammas_after_cuts_two_Both =
    new TH1D("WrongEta_AngleBetweenGammas_after_cuts_two_Both",
             "WrongEta_AngleBetweenGammas_after_cuts_two_Both (between "
             "#gamma#gamma); angle [deg]",
             900,
             -0.1,
             89.9);
  fHistoList_Eta_two_Both.push_back(
    WrongEta_AngleBetweenGammas_after_cuts_two_Both);
  EMT_Eta_InvMass_two_Both =
    new TH1D("EMT_Eta_InvMass_two_Both",
             "EMT_Eta_InvMass_two_Both; invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_Eta_two_Both.push_back(EMT_Eta_InvMass_two_Both);
  ///////////// multidifferential analysis case "two" and "Both"
  fHistoList_rap_pt_Eta_two_Both.push_back(TrueEta_pt_vs_rap_est_two_Both);
  multi_InvMass_Eta_two_Both_1 =
    new TH1D("multi_InvMass_Eta_two_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_1);
  multi_InvMass_Eta_two_Both_2 =
    new TH1D("multi_InvMass_Eta_two_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_2);
  multi_InvMass_Eta_two_Both_3 =
    new TH1D("multi_InvMass_Eta_two_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_3);
  multi_InvMass_Eta_two_Both_4 =
    new TH1D("multi_InvMass_Eta_two_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_4);
  multi_InvMass_Eta_two_Both_5 =
    new TH1D("multi_InvMass_Eta_two_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_5);
  multi_InvMass_Eta_two_Both_6 =
    new TH1D("multi_InvMass_Eta_two_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_6);
  multi_InvMass_Eta_two_Both_7 =
    new TH1D("multi_InvMass_Eta_two_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_7);
  multi_InvMass_Eta_two_Both_8 =
    new TH1D("multi_InvMass_Eta_two_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_8);
  multi_InvMass_Eta_two_Both_9 =
    new TH1D("multi_InvMass_Eta_two_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_9);
  multi_InvMass_Eta_two_Both_10 =
    new TH1D("multi_InvMass_Eta_two_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_10);
  multi_InvMass_Eta_two_Both_11 =
    new TH1D("multi_InvMass_Eta_two_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_11);
  multi_InvMass_Eta_two_Both_12 =
    new TH1D("multi_InvMass_Eta_two_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_12);
  multi_InvMass_Eta_two_Both_13 =
    new TH1D("multi_InvMass_Eta_two_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_13);
  multi_InvMass_Eta_two_Both_14 =
    new TH1D("multi_InvMass_Eta_two_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_14);
  multi_InvMass_Eta_two_Both_15 =
    new TH1D("multi_InvMass_Eta_two_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_15);
  multi_InvMass_Eta_two_Both_16 =
    new TH1D("multi_InvMass_Eta_two_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_16);
  multi_InvMass_Eta_two_Both_17 =
    new TH1D("multi_InvMass_Eta_two_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_InvMass_Eta_two_Both_17);
  multi_EMT_Eta_two_Both_1 =
    new TH1D("multi_EMT_Eta_two_Both_1",
             "rapidity = (1.2-1.6)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_1);
  multi_EMT_Eta_two_Both_2 =
    new TH1D("multi_EMT_Eta_two_Both_2",
             "rapidity = (1.2-1.6)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_2);
  multi_EMT_Eta_two_Both_3 =
    new TH1D("multi_EMT_Eta_two_Both_3",
             "rapidity = (1.2-1.6)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_3);
  multi_EMT_Eta_two_Both_4 =
    new TH1D("multi_EMT_Eta_two_Both_4",
             "rapidity = (1.2-1.6)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_4);
  multi_EMT_Eta_two_Both_5 =
    new TH1D("multi_EMT_Eta_two_Both_5",
             "rapidity = (1.6-2.0)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_5);
  multi_EMT_Eta_two_Both_6 =
    new TH1D("multi_EMT_Eta_two_Both_6",
             "rapidity = (1.6-2.0)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_6);
  multi_EMT_Eta_two_Both_7 =
    new TH1D("multi_EMT_Eta_two_Both_7",
             "rapidity = (1.6-2.0)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_7);
  multi_EMT_Eta_two_Both_8 =
    new TH1D("multi_EMT_Eta_two_Both_8",
             "rapidity = (1.6-2.0)      P_{t} = (1.2-2.0 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_8);
  multi_EMT_Eta_two_Both_9 =
    new TH1D("multi_EMT_Eta_two_Both_9",
             "rapidity = (2.0-2.4)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_9);
  multi_EMT_Eta_two_Both_10 =
    new TH1D("multi_EMT_Eta_two_Both_10",
             "rapidity = (2.0-2.4)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_10);
  multi_EMT_Eta_two_Both_11 =
    new TH1D("multi_EMT_Eta_two_Both_11",
             "rapidity = (2.0-2.4)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_11);
  multi_EMT_Eta_two_Both_12 =
    new TH1D("multi_EMT_Eta_two_Both_12",
             "rapidity = (2.0-2.4)      P_{t} = (1.2-1.6 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_12);
  multi_EMT_Eta_two_Both_13 =
    new TH1D("multi_EMT_Eta_two_Both_13",
             "rapidity = (2.4-2.8)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_13);
  multi_EMT_Eta_two_Both_14 =
    new TH1D("multi_EMT_Eta_two_Both_14",
             "rapidity = (2.4-2.8)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_14);
  multi_EMT_Eta_two_Both_15 =
    new TH1D("multi_EMT_Eta_two_Both_15",
             "rapidity = (2.4-2.8)      P_{t} = (0.8-1.2 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_15);
  multi_EMT_Eta_two_Both_16 =
    new TH1D("multi_EMT_Eta_two_Both_16",
             "rapidity = (2.8-3.2)      P_{t} = (0.0-0.4 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_16);
  multi_EMT_Eta_two_Both_17 =
    new TH1D("multi_EMT_Eta_two_Both_17",
             "rapidity = (2.8-3.2)      P_{t} = (0.4-0.8 GeV/c^{2}) ; "
             "invariant mass in GeV/c^{2};#",
             200,
             0.0,
             2.0);
  fHistoList_rap_pt_Eta_two_Both.push_back(multi_EMT_Eta_two_Both_17);
  /////////////////////////////// BOTH (END) //////////////////////////////////////
}
