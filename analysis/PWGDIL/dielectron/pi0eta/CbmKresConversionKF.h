/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBM_KRES_CONVERSION_KF
#define CBM_KRES_CONVERSION_KF

#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "CbmKresConversionBG.h"
#include "CbmMCTrack.h"
#include "CbmRichRing.h"

#include <TClonesArray.h>
#include <TH1.h>
#include <TH2D.h>

#include "KFParticle.h"
#include "KFTopoPerformance.h"

class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresConversionKF {

public:
  //***** brief Standard constructor.
  CbmKresConversionKF();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionKF();


  void Init();
  void InitHistograms();
  void Finish();

  void SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA);

  void Exec(int fEventNumKF, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void FindGammas(vector<KFParticle> allgammas, vector<KFParticle> particlevector, int Event, double AngleCut,
                  double InvMassCut, int RealPID);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  void FindPi0(TString mod, vector<vector<TVector3>> Gammas, vector<vector<int>> StsIndex,
               vector<vector<CbmMCTrack*>> GammasMC, vector<Double_t> GammasZ, TH1D* Pi0InvMassRecoKF,
               TH2D* Pi0_pt_vs_rap, TH2D* Pi0_pt_vs_rap_est, TH1D* Pi0InvMassRecoKF_target, TH1D* Pi0InvMassRecoKF_mvd,
               TH1D* Pi0InvMassRecoKF_sts, TH1D* Pi0InvMassRecoKF_outside, TH2D* MultiplicityGamma,
               TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases);

  void MixedEvent();
  void MixedEventMulti();


private:
  CbmKFParticleFinder* fKFparticle;
  CbmKFParticleFinderQA* fKFparticleFinderQA;
  const KFParticleTopoReconstructor* fKFtopo;

  CbmRichRingFitterEllipseTau* fTauFit;

  CbmKresConversionBG* fAnaBG;

  TClonesArray* fMcTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fGlobalTracks;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRichProjections;
  TClonesArray* fRichRings;
  TClonesArray* fRichHits;

  std::vector<TVector3> frefmomentum;
  std::vector<std::vector<TVector3>> GammasAll;
  std::vector<std::vector<TVector3>> GammasZero;
  std::vector<std::vector<TVector3>> GammasOne;
  std::vector<std::vector<TVector3>> GammasTwo;
  std::vector<std::vector<TVector3>> GammasOneTwo;

  std::vector<int> fStsInd;
  std::vector<std::vector<int>> GammasAllStsIndex;
  std::vector<std::vector<int>> GammasZeroStsIndex;
  std::vector<std::vector<int>> GammasOneStsIndex;
  std::vector<std::vector<int>> GammasTwoStsIndex;
  std::vector<std::vector<int>> GammasOneTwoStsIndex;

  std::vector<CbmMCTrack*> fmcvector;
  std::vector<std::vector<CbmMCTrack*>> GammasAllMC;
  std::vector<std::vector<CbmMCTrack*>> GammasZeroMC;
  std::vector<std::vector<CbmMCTrack*>> GammasOneMC;
  std::vector<std::vector<CbmMCTrack*>> GammasTwoMC;
  std::vector<std::vector<CbmMCTrack*>> GammasOneTwoMC;

  std::vector<Double_t> GammasAllZ;
  std::vector<Double_t> GammasZeroZ;
  std::vector<Double_t> GammasOneZ;
  std::vector<Double_t> GammasTwoZ;
  std::vector<Double_t> GammasOneTwoZ;


  // normal EMT
  std::vector<int> EMT_Event;
  std::vector<std::vector<TVector3>> EMT_pair_momenta;
  std::vector<int> EMT_NofRings;
  std::vector<Double_t> EMT_Z;

  // multi EMT
  std::vector<int> EMT_Event_multi_all;
  std::vector<std::vector<TVector3>> EMT_pair_momenta_multi_all;

  std::vector<int> EMT_Event_multi_one;
  std::vector<std::vector<TVector3>> EMT_pair_momenta_multi_one;

  std::vector<int> EMT_Event_multi_two;
  std::vector<std::vector<TVector3>> EMT_pair_momenta_multi_two;

  std::vector<int> EMT_Event_multi_zero;
  std::vector<std::vector<TVector3>> EMT_pair_momenta_multi_zero;

  std::vector<int> EMT_Event_multi_onetwo;
  std::vector<std::vector<TVector3>> EMT_pair_momenta_multi_onetwo;

  std::vector<int> EMT_multi_all;
  std::vector<int> EMT_multi_one;
  std::vector<int> EMT_multi_two;
  std::vector<int> EMT_multi_zero;
  std::vector<int> EMT_multi_onetwo;


  ///////   histograms to check Cuts
  vector<TH1*> fHistoList_CheckForCuts;
  TH1D* CheckForCuts_InvMass_MC;
  TH1D* CheckForCuts_InvMass_Reco;
  TH1D* CheckForCuts_OA_MC;
  TH1D* CheckForCuts_OA_Reco;

  TH1D* CheckForCuts_InvMass_MC_from_one_pi0;
  TH1D* CheckForCuts_InvMass_Reco_from_one_pi0;
  TH1D* CheckForCuts_OA_MC_from_one_pi0;
  TH1D* CheckForCuts_OA_Reco_from_one_pi0;
  TH2D* CheckForCuts_z_vs_InvMass_MC_from_one_pi0;
  TH2D* CheckForCuts_z_vs_InvMass_Reco_from_one_pi0;
  TH2D* CheckForCuts_z_vs_OA_MC_from_one_pi0;
  TH2D* CheckForCuts_z_vs_OA_Reco_from_one_pi0;
  TH1D* CheckForCuts_InvMass_Reco_from_one_pi0_less4cm;
  TH1D* CheckForCuts_OA_Reco_from_one_pi0_less4cm;
  TH1D* CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm;
  TH1D* CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm;
  TH1D* CheckForCuts_InvMass_Reco_from_one_pi0_more21cm;
  TH1D* CheckForCuts_OA_Reco_from_one_pi0_more21cm;

  // 0-2 => All
  vector<TH1*> fHistoList_All;
  TH1D* fGammaInvMassReco_All;
  TH1D* fGammaOpeningAngleReco_All;
  TH1D* fPdg_All;
  TH1D* fP_reco_All;
  TH1D* fPt_reco_All;
  TH1D* fPi0InvMassRecoKF_All;
  TH1D* fEMT_InvMass_All;
  TH2D* fPi0_pt_vs_rap_All;
  TH2D* fPi0_pt_vs_rap_est_All;
  // 0-2 => All  target
  vector<TH1*> fHistoList_All_target;
  TH1D* fGammaInvMassReco_All_target;
  TH1D* fGammaOpeningAngleReco_All_target;
  TH1D* fPi0InvMassRecoKF_All_target;
  TH1D* fEMT_InvMass_All_target;
  // 0-2 => All  mvd
  vector<TH1*> fHistoList_All_mvd;
  TH1D* fGammaInvMassReco_All_mvd;
  TH1D* fGammaOpeningAngleReco_All_mvd;
  TH1D* fPi0InvMassRecoKF_All_mvd;
  // 0-2 => All  sts
  vector<TH1*> fHistoList_All_sts;
  TH1D* fGammaInvMassReco_All_sts;
  TH1D* fGammaOpeningAngleReco_All_sts;
  TH1D* fPi0InvMassRecoKF_All_sts;
  // 0-2 => All  outside the target (mvd+sts+pipe)
  vector<TH1*> fHistoList_All_outside;
  TH1D* fGammaInvMassReco_All_outside;
  TH1D* fGammaOpeningAngleReco_All_outside;
  TH1D* fPi0InvMassRecoKF_All_outside;
  TH1D* fEMT_InvMass_All_outside;

  // 0 => Zero
  vector<TH1*> fHistoList_Zero;
  TH1D* fGammaInvMassReco_Zero;
  TH1D* fGammaOpeningAngleReco_Zero;
  TH1D* fPdg_Zero;
  TH1D* fP_reco_Zero;
  TH1D* fPt_reco_Zero;
  TH1D* fPi0InvMassRecoKF_Zero;
  TH1D* fEMT_InvMass_Zero;
  TH2D* fPi0_pt_vs_rap_Zero;
  TH2D* fPi0_pt_vs_rap_est_Zero;
  // 0 => Zero  target
  vector<TH1*> fHistoList_Zero_target;
  TH1D* fGammaInvMassReco_Zero_target;
  TH1D* fGammaOpeningAngleReco_Zero_target;
  TH1D* fPi0InvMassRecoKF_Zero_target;
  TH1D* fEMT_InvMass_Zero_target;
  // 0 => Zero  mvd
  vector<TH1*> fHistoList_Zero_mvd;
  TH1D* fGammaInvMassReco_Zero_mvd;
  TH1D* fGammaOpeningAngleReco_Zero_mvd;
  TH1D* fPi0InvMassRecoKF_Zero_mvd;
  // 0 => Zero  sts
  vector<TH1*> fHistoList_Zero_sts;
  TH1D* fGammaInvMassReco_Zero_sts;
  TH1D* fGammaOpeningAngleReco_Zero_sts;
  TH1D* fPi0InvMassRecoKF_Zero_sts;
  // 0 => Zero  outside the target (mvd+sts+pipe)
  vector<TH1*> fHistoList_Zero_outside;
  TH1D* fGammaInvMassReco_Zero_outside;
  TH1D* fGammaOpeningAngleReco_Zero_outside;
  TH1D* fPi0InvMassRecoKF_Zero_outside;
  TH1D* fEMT_InvMass_Zero_outside;

  // 1 => One
  vector<TH1*> fHistoList_One;
  TH1D* fGammaInvMassReco_One;
  TH1D* fGammaOpeningAngleReco_One;
  TH1D* fPdg_One;
  TH1D* fP_reco_One;
  TH1D* fPt_reco_One;
  TH1D* fPi0InvMassRecoKF_One;
  TH1D* fEMT_InvMass_One;
  TH2D* fPi0_pt_vs_rap_One;
  TH2D* fPi0_pt_vs_rap_est_One;
  // 1 => One  target
  vector<TH1*> fHistoList_One_target;
  TH1D* fGammaInvMassReco_One_target;
  TH1D* fGammaOpeningAngleReco_One_target;
  TH1D* fPi0InvMassRecoKF_One_target;
  TH1D* fEMT_InvMass_One_target;
  // 1 => One  mvd
  vector<TH1*> fHistoList_One_mvd;
  TH1D* fGammaInvMassReco_One_mvd;
  TH1D* fGammaOpeningAngleReco_One_mvd;
  TH1D* fPi0InvMassRecoKF_One_mvd;
  // 1 => One  sts
  vector<TH1*> fHistoList_One_sts;
  TH1D* fGammaInvMassReco_One_sts;
  TH1D* fGammaOpeningAngleReco_One_sts;
  TH1D* fPi0InvMassRecoKF_One_sts;
  // 1 => One  outside the target (mvd+sts+pipe)
  vector<TH1*> fHistoList_One_outside;
  TH1D* fGammaInvMassReco_One_outside;
  TH1D* fGammaOpeningAngleReco_One_outside;
  TH1D* fPi0InvMassRecoKF_One_outside;
  TH1D* fEMT_InvMass_One_outside;


  // 2 => Two
  vector<TH1*> fHistoList_Two;
  TH1D* fGammaInvMassReco_Two;
  TH1D* fGammaOpeningAngleReco_Two;
  TH1D* fPdg_Two;
  TH1D* fP_reco_Two;
  TH1D* fPt_reco_Two;
  TH1D* fPi0InvMassRecoKF_Two;
  TH1D* fEMT_InvMass_Two;
  TH2D* fPi0_pt_vs_rap_Two;
  TH2D* fPi0_pt_vs_rap_est_Two;
  // 2 => Two  target
  vector<TH1*> fHistoList_Two_target;
  TH1D* fGammaInvMassReco_Two_target;
  TH1D* fGammaOpeningAngleReco_Two_target;
  TH1D* fPi0InvMassRecoKF_Two_target;
  TH1D* fEMT_InvMass_Two_target;
  // 2 => Two  mvd
  vector<TH1*> fHistoList_Two_mvd;
  TH1D* fGammaInvMassReco_Two_mvd;
  TH1D* fGammaOpeningAngleReco_Two_mvd;
  TH1D* fPi0InvMassRecoKF_Two_mvd;
  // 2 => Two  sts
  vector<TH1*> fHistoList_Two_sts;
  TH1D* fGammaInvMassReco_Two_sts;
  TH1D* fGammaOpeningAngleReco_Two_sts;
  TH1D* fPi0InvMassRecoKF_Two_sts;
  // 2 => Two  outside the target (mvd+sts+pipe)
  vector<TH1*> fHistoList_Two_outside;
  TH1D* fGammaInvMassReco_Two_outside;
  TH1D* fGammaOpeningAngleReco_Two_outside;
  TH1D* fPi0InvMassRecoKF_Two_outside;
  TH1D* fEMT_InvMass_Two_outside;

  // 1-2 => OneTwo
  vector<TH1*> fHistoList_OneTwo;
  TH1D* fGammaInvMassReco_OneTwo;
  TH1D* fGammaOpeningAngleReco_OneTwo;
  TH1D* fPdg_OneTwo;
  TH1D* fP_reco_OneTwo;
  TH1D* fPt_reco_OneTwo;
  TH1D* fPi0InvMassRecoKF_OneTwo;
  TH1D* fEMT_InvMass_OneTwo;
  TH2D* fPi0_pt_vs_rap_OneTwo;
  TH2D* fPi0_pt_vs_rap_est_OneTwo;
  // 1-2 => OneTwo  target
  vector<TH1*> fHistoList_OneTwo_target;
  TH1D* fGammaInvMassReco_OneTwo_target;
  TH1D* fGammaOpeningAngleReco_OneTwo_target;
  TH1D* fPi0InvMassRecoKF_OneTwo_target;
  TH1D* fEMT_InvMass_OneTwo_target;
  // 1-2 => OneTwo  mvd
  vector<TH1*> fHistoList_OneTwo_mvd;
  TH1D* fGammaInvMassReco_OneTwo_mvd;
  TH1D* fGammaOpeningAngleReco_OneTwo_mvd;
  TH1D* fPi0InvMassRecoKF_OneTwo_mvd;
  // 1-2 => OneTwo  sts
  vector<TH1*> fHistoList_OneTwo_sts;
  TH1D* fGammaInvMassReco_OneTwo_sts;
  TH1D* fGammaOpeningAngleReco_OneTwo_sts;
  TH1D* fPi0InvMassRecoKF_OneTwo_sts;
  // 1-2 => OneTwo  outside the target (mvd+sts+pipe)
  vector<TH1*> fHistoList_OneTwo_outside;
  TH1D* fGammaInvMassReco_OneTwo_outside;
  TH1D* fGammaOpeningAngleReco_OneTwo_outside;
  TH1D* fPi0InvMassRecoKF_OneTwo_outside;
  TH1D* fEMT_InvMass_OneTwo_outside;


  // multiplicity
  vector<TH1*> fHistoList_multiplicity;
  TH2D* MultiplicityGamma_All;
  TH2D* MultiplicityGamma_Zero;
  TH2D* MultiplicityGamma_One;
  TH2D* MultiplicityGamma_Two;
  TH2D* MultiplicityGamma_OneTwo;

  TH2D* MultiplicityChargedParticles_All;
  TH2D* MultiplicityChargedParticles_Zero;
  TH2D* MultiplicityChargedParticles_One;
  TH2D* MultiplicityChargedParticles_Two;
  TH2D* MultiplicityChargedParticles_OneTwo;


  // Multi EMT
  vector<TH1*> fHistoList_multiplicity_All;
  vector<TH1*> fHistoList_multiplicity_One;
  vector<TH1*> fHistoList_multiplicity_Two;
  vector<TH1*> fHistoList_multiplicity_Zero;
  vector<TH1*> fHistoList_multiplicity_OneTwo;

  TH1D* EMTMulti_InvMass_All_m1;
  TH1D* EMTMulti_InvMass_All_m2;
  TH1D* EMTMulti_InvMass_All_m3;
  TH1D* EMTMulti_InvMass_All_m4;
  TH1D* EMTMulti_InvMass_All_m5;
  TH1D* EMTMulti_InvMass_All_m6;
  TH1D* EMTMulti_InvMass_All_m7;

  TH1D* EMTMulti_InvMass_Zero_m1;
  TH1D* EMTMulti_InvMass_Zero_m2;
  TH1D* EMTMulti_InvMass_Zero_m3;
  TH1D* EMTMulti_InvMass_Zero_m4;
  TH1D* EMTMulti_InvMass_Zero_m5;
  TH1D* EMTMulti_InvMass_Zero_m6;
  TH1D* EMTMulti_InvMass_Zero_m7;

  TH1D* EMTMulti_InvMass_One_m1;
  TH1D* EMTMulti_InvMass_One_m2;
  TH1D* EMTMulti_InvMass_One_m3;
  TH1D* EMTMulti_InvMass_One_m4;
  TH1D* EMTMulti_InvMass_One_m5;
  TH1D* EMTMulti_InvMass_One_m6;
  TH1D* EMTMulti_InvMass_One_m7;

  TH1D* EMTMulti_InvMass_Two_m1;
  TH1D* EMTMulti_InvMass_Two_m2;
  TH1D* EMTMulti_InvMass_Two_m3;
  TH1D* EMTMulti_InvMass_Two_m4;
  TH1D* EMTMulti_InvMass_Two_m5;
  TH1D* EMTMulti_InvMass_Two_m6;
  TH1D* EMTMulti_InvMass_Two_m7;

  TH1D* EMTMulti_InvMass_OneTwo_m1;
  TH1D* EMTMulti_InvMass_OneTwo_m2;
  TH1D* EMTMulti_InvMass_OneTwo_m3;
  TH1D* EMTMulti_InvMass_OneTwo_m4;
  TH1D* EMTMulti_InvMass_OneTwo_m5;
  TH1D* EMTMulti_InvMass_OneTwo_m6;
  TH1D* EMTMulti_InvMass_OneTwo_m7;

  // BG cases Both
  // all
  vector<TH1*> fHistoList_bg_all;
  TH1D* BG1_all;
  TH1D* BG2_all;
  TH1D* BG3_all;
  TH1D* BG4_all;
  TH1D* BG5_all;
  TH1D* BG6_all;
  TH1D* BG7_all;
  TH1D* BG8_all;
  TH1D* BG9_all;
  TH1D* BG10_all;
  TH1D* PdgCase8_all;
  TH1D* PdgCase8mothers_all;
  TH1D* sameMIDcase8_all;
  TH1D* sameGRIDcase8_all;
  TH2D* Case1ZYPos_all;
  TH1D* sameMIDcase8_mothedPDG_all;
  TH1D* PdgCase8NonEComeFromTarget_all;
  TH1D* PdgCase8NonE_NOT_FromTarget_all;
  TH1D* PdgCase8motherNonE_all;
  TH1D* Case8ElFromDalitz_all;
  TH1D* Case8NonElFrom_pn_all;
  TH1D* Case8NonElFrom_eta_all;
  TH1D* Case8NonElFrom_kaon_all;
  TH1D* sameMIDcase8NonEPdg_all;
  TH1D* sameMIDcase8NonEMotherPdg_all;
  TH1D* sameMIDcase8NonEMotherIM_all;
  TH1D* sameMIDcase8NonEPdgFromTarget_all;
  TH1D* sameMIDcase8NonEComeFromTargetIM_all;
  TH1D* sameMIDcase8NonEComeFromTargetP_all;
  TH1D* sameMIDcase8NonEComeFromTargetPt_all;
  // zero
  vector<TH1*> fHistoList_bg_zero;
  TH1D* BG1_zero;
  TH1D* BG2_zero;
  TH1D* BG3_zero;
  TH1D* BG4_zero;
  TH1D* BG5_zero;
  TH1D* BG6_zero;
  TH1D* BG7_zero;
  TH1D* BG8_zero;
  TH1D* BG9_zero;
  TH1D* BG10_zero;
  TH1D* PdgCase8_zero;
  TH1D* PdgCase8mothers_zero;
  TH1D* sameMIDcase8_zero;
  TH1D* sameGRIDcase8_zero;
  TH2D* Case1ZYPos_zero;
  TH1D* sameMIDcase8_mothedPDG_zero;
  TH1D* PdgCase8NonEComeFromTarget_zero;
  TH1D* PdgCase8NonE_NOT_FromTarget_zero;
  TH1D* PdgCase8motherNonE_zero;
  TH1D* Case8ElFromDalitz_zero;
  TH1D* Case8NonElFrom_pn_zero;
  TH1D* Case8NonElFrom_eta_zero;
  TH1D* Case8NonElFrom_kaon_zero;
  TH1D* sameMIDcase8NonEPdg_zero;
  TH1D* sameMIDcase8NonEMotherPdg_zero;
  TH1D* sameMIDcase8NonEMotherIM_zero;
  TH1D* sameMIDcase8NonEPdgFromTarget_zero;
  TH1D* sameMIDcase8NonEComeFromTargetIM_zero;
  TH1D* sameMIDcase8NonEComeFromTargetP_zero;
  TH1D* sameMIDcase8NonEComeFromTargetPt_zero;
  // one
  vector<TH1*> fHistoList_bg_one;
  TH1D* BG1_one;
  TH1D* BG2_one;
  TH1D* BG3_one;
  TH1D* BG4_one;
  TH1D* BG5_one;
  TH1D* BG6_one;
  TH1D* BG7_one;
  TH1D* BG8_one;
  TH1D* BG9_one;
  TH1D* BG10_one;
  TH1D* PdgCase8_one;
  TH1D* PdgCase8mothers_one;
  TH1D* sameMIDcase8_one;
  TH1D* sameGRIDcase8_one;
  TH2D* Case1ZYPos_one;
  TH1D* sameMIDcase8_mothedPDG_one;
  TH1D* PdgCase8NonEComeFromTarget_one;
  TH1D* PdgCase8NonE_NOT_FromTarget_one;
  TH1D* PdgCase8motherNonE_one;
  TH1D* Case8ElFromDalitz_one;
  TH1D* Case8NonElFrom_pn_one;
  TH1D* Case8NonElFrom_eta_one;
  TH1D* Case8NonElFrom_kaon_one;
  TH1D* sameMIDcase8NonEPdg_one;
  TH1D* sameMIDcase8NonEMotherPdg_one;
  TH1D* sameMIDcase8NonEMotherIM_one;
  TH1D* sameMIDcase8NonEPdgFromTarget_one;
  TH1D* sameMIDcase8NonEComeFromTargetIM_one;
  TH1D* sameMIDcase8NonEComeFromTargetP_one;
  TH1D* sameMIDcase8NonEComeFromTargetPt_one;
  // two
  vector<TH1*> fHistoList_bg_two;
  TH1D* BG1_two;
  TH1D* BG2_two;
  TH1D* BG3_two;
  TH1D* BG4_two;
  TH1D* BG5_two;
  TH1D* BG6_two;
  TH1D* BG7_two;
  TH1D* BG8_two;
  TH1D* BG9_two;
  TH1D* BG10_two;
  TH1D* PdgCase8_two;
  TH1D* PdgCase8mothers_two;
  TH1D* sameMIDcase8_two;
  TH1D* sameGRIDcase8_two;
  TH2D* Case1ZYPos_two;
  TH1D* sameMIDcase8_mothedPDG_two;
  TH1D* PdgCase8NonEComeFromTarget_two;
  TH1D* PdgCase8NonE_NOT_FromTarget_two;
  TH1D* PdgCase8motherNonE_two;
  TH1D* Case8ElFromDalitz_two;
  TH1D* Case8NonElFrom_pn_two;
  TH1D* Case8NonElFrom_eta_two;
  TH1D* Case8NonElFrom_kaon_two;
  TH1D* sameMIDcase8NonEPdg_two;
  TH1D* sameMIDcase8NonEMotherPdg_two;
  TH1D* sameMIDcase8NonEMotherIM_two;
  TH1D* sameMIDcase8NonEPdgFromTarget_two;
  TH1D* sameMIDcase8NonEComeFromTargetIM_two;
  TH1D* sameMIDcase8NonEComeFromTargetP_two;
  TH1D* sameMIDcase8NonEComeFromTargetPt_two;
  // onetwo
  vector<TH1*> fHistoList_bg_onetwo;
  TH1D* BG1_onetwo;
  TH1D* BG2_onetwo;
  TH1D* BG3_onetwo;
  TH1D* BG4_onetwo;
  TH1D* BG5_onetwo;
  TH1D* BG6_onetwo;
  TH1D* BG7_onetwo;
  TH1D* BG8_onetwo;
  TH1D* BG9_onetwo;
  TH1D* BG10_onetwo;
  TH1D* PdgCase8_onetwo;
  TH1D* PdgCase8mothers_onetwo;
  TH1D* sameMIDcase8_onetwo;
  TH1D* sameGRIDcase8_onetwo;
  TH2D* Case1ZYPos_onetwo;
  TH1D* sameMIDcase8_mothedPDG_onetwo;
  TH1D* PdgCase8NonEComeFromTarget_onetwo;
  TH1D* PdgCase8NonE_NOT_FromTarget_onetwo;
  TH1D* PdgCase8motherNonE_onetwo;
  TH1D* Case8ElFromDalitz_onetwo;
  TH1D* Case8NonElFrom_pn_onetwo;
  TH1D* Case8NonElFrom_eta_onetwo;
  TH1D* Case8NonElFrom_kaon_onetwo;
  TH1D* sameMIDcase8NonEPdg_onetwo;
  TH1D* sameMIDcase8NonEMotherPdg_onetwo;
  TH1D* sameMIDcase8NonEMotherIM_onetwo;
  TH1D* sameMIDcase8NonEPdgFromTarget_onetwo;
  TH1D* sameMIDcase8NonEComeFromTargetIM_onetwo;
  TH1D* sameMIDcase8NonEComeFromTargetP_onetwo;
  TH1D* sameMIDcase8NonEComeFromTargetPt_onetwo;


  //***** brief Copy constructor.
  CbmKresConversionKF(const CbmKresConversionKF&);

  //***** brief Assignment operator.
  CbmKresConversionKF operator=(const CbmKresConversionKF&);


  ClassDef(CbmKresConversionKF, 1)
};

#endif
