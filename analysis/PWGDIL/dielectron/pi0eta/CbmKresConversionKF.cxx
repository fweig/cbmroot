/**
 *    file CbmKresConversionKF.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *
 *    Class for the pi^0 conversion analysis using reconstructed photons from KFParticle.
 *    Different cases are considered depending on the lepton identification in the RICH (for gammas): 0 out of 2, at least 1 out of 2, and 2 out of 2.
 *    The best choice would be 1 out of 2 or 2 out of 2 because of signal to background and amount of reconstructed particles.
 *    Separating conversions inside the target region and in the detector region gives also a lot of benefits from the point of view of photon reconstruction.
 *
 *    For make it work one needs to add KFParticle reconstruction in the analysis macro before calling "CbmKresConversionMain.cxx", otherwise kfparticle will not be initialized (See Maksym Zyzak or Vasiliev macro)
 *
 **/

#include "CbmKresConversionKF.h"

#include "CbmGlobalTrack.h"
#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "CbmKresConversionBG.h"
#include "CbmKresFunctions.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmMCTrack.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichRingLight.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "KFParticle.h"
#include "KFParticleTopoReconstructor.h"


using namespace std;

CbmKresConversionKF::CbmKresConversionKF()
  : fKFparticle(nullptr)
  , fKFparticleFinderQA(nullptr)
  , fKFtopo(nullptr)
  , fTauFit(nullptr)
  , fAnaBG(nullptr)
  , fMcTracks(nullptr)
  , fStsTracks(nullptr)
  , fStsTrackMatches(nullptr)
  , fGlobalTracks(nullptr)
  , fRichRingMatches(nullptr)
  , fRichProjections(nullptr)
  , fRichRings(nullptr)
  , fRichHits(nullptr)
  , frefmomentum()
  , GammasAll()
  , GammasZero()
  , GammasOne()
  , GammasTwo()
  , GammasOneTwo()
  , fStsInd()
  , GammasAllStsIndex()
  , GammasZeroStsIndex()
  , GammasOneStsIndex()
  , GammasTwoStsIndex()
  , GammasOneTwoStsIndex()
  , fmcvector()
  , GammasAllMC()
  , GammasZeroMC()
  , GammasOneMC()
  , GammasTwoMC()
  , GammasOneTwoMC()
  , GammasAllZ()
  , GammasZeroZ()
  , GammasOneZ()
  , GammasTwoZ()
  , GammasOneTwoZ()
  , EMT_Event()
  , EMT_pair_momenta()
  , EMT_NofRings()
  , EMT_Z()
  , EMT_Event_multi_all()
  , EMT_pair_momenta_multi_all()
  , EMT_Event_multi_one()
  , EMT_pair_momenta_multi_one()
  , EMT_Event_multi_two()
  , EMT_pair_momenta_multi_two()
  , EMT_Event_multi_zero()
  , EMT_pair_momenta_multi_zero()
  , EMT_Event_multi_onetwo()
  , EMT_pair_momenta_multi_onetwo()
  , EMT_multi_all()
  , EMT_multi_one()
  , EMT_multi_two()
  , EMT_multi_zero()
  , EMT_multi_onetwo()
  , fHistoList_CheckForCuts()
  , CheckForCuts_InvMass_MC(nullptr)
  , CheckForCuts_InvMass_Reco(nullptr)
  , CheckForCuts_OA_MC(nullptr)
  , CheckForCuts_OA_Reco(nullptr)
  , CheckForCuts_InvMass_MC_from_one_pi0(nullptr)
  , CheckForCuts_InvMass_Reco_from_one_pi0(nullptr)
  , CheckForCuts_OA_MC_from_one_pi0(nullptr)
  , CheckForCuts_OA_Reco_from_one_pi0(nullptr)
  , CheckForCuts_z_vs_InvMass_MC_from_one_pi0(nullptr)
  , CheckForCuts_z_vs_InvMass_Reco_from_one_pi0(nullptr)
  , CheckForCuts_z_vs_OA_MC_from_one_pi0(nullptr)
  , CheckForCuts_z_vs_OA_Reco_from_one_pi0(nullptr)
  , CheckForCuts_InvMass_Reco_from_one_pi0_less4cm(nullptr)
  , CheckForCuts_OA_Reco_from_one_pi0_less4cm(nullptr)
  , CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm(nullptr)
  , CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm(nullptr)
  , CheckForCuts_InvMass_Reco_from_one_pi0_more21cm(nullptr)
  , CheckForCuts_OA_Reco_from_one_pi0_more21cm(nullptr)
  , fHistoList_All()
  , fGammaInvMassReco_All(nullptr)
  , fGammaOpeningAngleReco_All(nullptr)
  , fPdg_All(nullptr)
  , fP_reco_All(nullptr)
  , fPt_reco_All(nullptr)
  , fPi0InvMassRecoKF_All(nullptr)
  , fEMT_InvMass_All(nullptr)
  , fPi0_pt_vs_rap_All(nullptr)
  , fPi0_pt_vs_rap_est_All(nullptr)
  , fHistoList_All_target()
  , fGammaInvMassReco_All_target(nullptr)
  , fGammaOpeningAngleReco_All_target(nullptr)
  , fPi0InvMassRecoKF_All_target(nullptr)
  , fEMT_InvMass_All_target(nullptr)
  , fHistoList_All_mvd()
  , fGammaInvMassReco_All_mvd(nullptr)
  , fGammaOpeningAngleReco_All_mvd(nullptr)
  , fPi0InvMassRecoKF_All_mvd(nullptr)
  , fHistoList_All_sts()
  , fGammaInvMassReco_All_sts(nullptr)
  , fGammaOpeningAngleReco_All_sts(nullptr)
  , fPi0InvMassRecoKF_All_sts(nullptr)
  , fHistoList_All_outside()
  , fGammaInvMassReco_All_outside(nullptr)
  , fGammaOpeningAngleReco_All_outside(nullptr)
  , fPi0InvMassRecoKF_All_outside(nullptr)
  , fEMT_InvMass_All_outside(nullptr)
  , fHistoList_Zero()
  , fGammaInvMassReco_Zero(nullptr)
  , fGammaOpeningAngleReco_Zero(nullptr)
  , fPdg_Zero(nullptr)
  , fP_reco_Zero(nullptr)
  , fPt_reco_Zero(nullptr)
  , fPi0InvMassRecoKF_Zero(nullptr)
  , fEMT_InvMass_Zero(nullptr)
  , fPi0_pt_vs_rap_Zero(nullptr)
  , fPi0_pt_vs_rap_est_Zero(nullptr)
  , fHistoList_Zero_target()
  , fGammaInvMassReco_Zero_target(nullptr)
  , fGammaOpeningAngleReco_Zero_target(nullptr)
  , fPi0InvMassRecoKF_Zero_target(nullptr)
  , fEMT_InvMass_Zero_target(nullptr)
  , fHistoList_Zero_mvd()
  , fGammaInvMassReco_Zero_mvd(nullptr)
  , fGammaOpeningAngleReco_Zero_mvd(nullptr)
  , fPi0InvMassRecoKF_Zero_mvd(nullptr)
  , fHistoList_Zero_sts()
  , fGammaInvMassReco_Zero_sts(nullptr)
  , fGammaOpeningAngleReco_Zero_sts(nullptr)
  , fPi0InvMassRecoKF_Zero_sts(nullptr)
  , fHistoList_Zero_outside()
  , fGammaInvMassReco_Zero_outside(nullptr)
  , fGammaOpeningAngleReco_Zero_outside(nullptr)
  , fPi0InvMassRecoKF_Zero_outside(nullptr)
  , fEMT_InvMass_Zero_outside(nullptr)
  , fHistoList_One()
  , fGammaInvMassReco_One(nullptr)
  , fGammaOpeningAngleReco_One(nullptr)
  , fPdg_One(nullptr)
  , fP_reco_One(nullptr)
  , fPt_reco_One(nullptr)
  , fPi0InvMassRecoKF_One(nullptr)
  , fEMT_InvMass_One(nullptr)
  , fPi0_pt_vs_rap_One(nullptr)
  , fPi0_pt_vs_rap_est_One(nullptr)
  , fHistoList_One_target()
  , fGammaInvMassReco_One_target(nullptr)
  , fGammaOpeningAngleReco_One_target(nullptr)
  , fPi0InvMassRecoKF_One_target(nullptr)
  , fEMT_InvMass_One_target(nullptr)
  , fHistoList_One_mvd()
  , fGammaInvMassReco_One_mvd(nullptr)
  , fGammaOpeningAngleReco_One_mvd(nullptr)
  , fPi0InvMassRecoKF_One_mvd(nullptr)
  , fHistoList_One_sts()
  , fGammaInvMassReco_One_sts(nullptr)
  , fGammaOpeningAngleReco_One_sts(nullptr)
  , fPi0InvMassRecoKF_One_sts(nullptr)
  , fHistoList_One_outside()
  , fGammaInvMassReco_One_outside(nullptr)
  , fGammaOpeningAngleReco_One_outside(nullptr)
  , fPi0InvMassRecoKF_One_outside(nullptr)
  , fEMT_InvMass_One_outside(nullptr)
  , fHistoList_Two()
  , fGammaInvMassReco_Two(nullptr)
  , fGammaOpeningAngleReco_Two(nullptr)
  , fPdg_Two(nullptr)
  , fP_reco_Two(nullptr)
  , fPt_reco_Two(nullptr)
  , fPi0InvMassRecoKF_Two(nullptr)
  , fEMT_InvMass_Two(nullptr)
  , fPi0_pt_vs_rap_Two(nullptr)
  , fPi0_pt_vs_rap_est_Two(nullptr)
  , fHistoList_Two_target()
  , fGammaInvMassReco_Two_target(nullptr)
  , fGammaOpeningAngleReco_Two_target(nullptr)
  , fPi0InvMassRecoKF_Two_target(nullptr)
  , fEMT_InvMass_Two_target(nullptr)
  , fHistoList_Two_mvd()
  , fGammaInvMassReco_Two_mvd(nullptr)
  , fGammaOpeningAngleReco_Two_mvd(nullptr)
  , fPi0InvMassRecoKF_Two_mvd(nullptr)
  , fHistoList_Two_sts()
  , fGammaInvMassReco_Two_sts(nullptr)
  , fGammaOpeningAngleReco_Two_sts(nullptr)
  , fPi0InvMassRecoKF_Two_sts(nullptr)
  , fHistoList_Two_outside()
  , fGammaInvMassReco_Two_outside(nullptr)
  , fGammaOpeningAngleReco_Two_outside(nullptr)
  , fPi0InvMassRecoKF_Two_outside(nullptr)
  , fEMT_InvMass_Two_outside(nullptr)
  , fHistoList_OneTwo()
  , fGammaInvMassReco_OneTwo(nullptr)
  , fGammaOpeningAngleReco_OneTwo(nullptr)
  , fPdg_OneTwo(nullptr)
  , fP_reco_OneTwo(nullptr)
  , fPt_reco_OneTwo(nullptr)
  , fPi0InvMassRecoKF_OneTwo(nullptr)
  , fEMT_InvMass_OneTwo(nullptr)
  , fPi0_pt_vs_rap_OneTwo(nullptr)
  , fPi0_pt_vs_rap_est_OneTwo(nullptr)
  , fHistoList_OneTwo_target()
  , fGammaInvMassReco_OneTwo_target(nullptr)
  , fGammaOpeningAngleReco_OneTwo_target(nullptr)
  , fPi0InvMassRecoKF_OneTwo_target(nullptr)
  , fEMT_InvMass_OneTwo_target(nullptr)
  , fHistoList_OneTwo_mvd()
  , fGammaInvMassReco_OneTwo_mvd(nullptr)
  , fGammaOpeningAngleReco_OneTwo_mvd(nullptr)
  , fPi0InvMassRecoKF_OneTwo_mvd(nullptr)
  , fHistoList_OneTwo_sts()
  , fGammaInvMassReco_OneTwo_sts(nullptr)
  , fGammaOpeningAngleReco_OneTwo_sts(nullptr)
  , fPi0InvMassRecoKF_OneTwo_sts(nullptr)
  , fHistoList_OneTwo_outside()
  , fGammaInvMassReco_OneTwo_outside(nullptr)
  , fGammaOpeningAngleReco_OneTwo_outside(nullptr)
  , fPi0InvMassRecoKF_OneTwo_outside(nullptr)
  , fEMT_InvMass_OneTwo_outside(nullptr)
  , fHistoList_multiplicity()
  , MultiplicityGamma_All(nullptr)
  , MultiplicityGamma_Zero(nullptr)
  , MultiplicityGamma_One(nullptr)
  , MultiplicityGamma_Two(nullptr)
  , MultiplicityGamma_OneTwo(nullptr)
  , MultiplicityChargedParticles_All(nullptr)
  , MultiplicityChargedParticles_Zero(nullptr)
  , MultiplicityChargedParticles_One(nullptr)
  , MultiplicityChargedParticles_Two(nullptr)
  , MultiplicityChargedParticles_OneTwo(nullptr)
  , fHistoList_multiplicity_All()
  , fHistoList_multiplicity_One()
  , fHistoList_multiplicity_Two()
  , fHistoList_multiplicity_Zero()
  , fHistoList_multiplicity_OneTwo()
  , EMTMulti_InvMass_All_m1(nullptr)
  , EMTMulti_InvMass_All_m2(nullptr)
  , EMTMulti_InvMass_All_m3(nullptr)
  , EMTMulti_InvMass_All_m4(nullptr)
  , EMTMulti_InvMass_All_m5(nullptr)
  , EMTMulti_InvMass_All_m6(nullptr)
  , EMTMulti_InvMass_All_m7(nullptr)
  , EMTMulti_InvMass_Zero_m1(nullptr)
  , EMTMulti_InvMass_Zero_m2(nullptr)
  , EMTMulti_InvMass_Zero_m3(nullptr)
  , EMTMulti_InvMass_Zero_m4(nullptr)
  , EMTMulti_InvMass_Zero_m5(nullptr)
  , EMTMulti_InvMass_Zero_m6(nullptr)
  , EMTMulti_InvMass_Zero_m7(nullptr)
  , EMTMulti_InvMass_One_m1(nullptr)
  , EMTMulti_InvMass_One_m2(nullptr)
  , EMTMulti_InvMass_One_m3(nullptr)
  , EMTMulti_InvMass_One_m4(nullptr)
  , EMTMulti_InvMass_One_m5(nullptr)
  , EMTMulti_InvMass_One_m6(nullptr)
  , EMTMulti_InvMass_One_m7(nullptr)
  , EMTMulti_InvMass_Two_m1(nullptr)
  , EMTMulti_InvMass_Two_m2(nullptr)
  , EMTMulti_InvMass_Two_m3(nullptr)
  , EMTMulti_InvMass_Two_m4(nullptr)
  , EMTMulti_InvMass_Two_m5(nullptr)
  , EMTMulti_InvMass_Two_m6(nullptr)
  , EMTMulti_InvMass_Two_m7(nullptr)
  , EMTMulti_InvMass_OneTwo_m1(nullptr)
  , EMTMulti_InvMass_OneTwo_m2(nullptr)
  , EMTMulti_InvMass_OneTwo_m3(nullptr)
  , EMTMulti_InvMass_OneTwo_m4(nullptr)
  , EMTMulti_InvMass_OneTwo_m5(nullptr)
  , EMTMulti_InvMass_OneTwo_m6(nullptr)
  , EMTMulti_InvMass_OneTwo_m7(nullptr)
  , fHistoList_bg_all()
  , BG1_all(nullptr)
  , BG2_all(nullptr)
  , BG3_all(nullptr)
  , BG4_all(nullptr)
  , BG5_all(nullptr)
  , BG6_all(nullptr)
  , BG7_all(nullptr)
  , BG8_all(nullptr)
  , BG9_all(nullptr)
  , BG10_all(nullptr)
  , PdgCase8_all(nullptr)
  , PdgCase8mothers_all(nullptr)
  , sameMIDcase8_all(nullptr)
  , sameGRIDcase8_all(nullptr)
  , Case1ZYPos_all(nullptr)
  , sameMIDcase8_mothedPDG_all(nullptr)
  , PdgCase8NonEComeFromTarget_all(nullptr)
  , PdgCase8NonE_NOT_FromTarget_all(nullptr)
  , PdgCase8motherNonE_all(nullptr)
  , Case8ElFromDalitz_all(nullptr)
  , Case8NonElFrom_pn_all(nullptr)
  , Case8NonElFrom_eta_all(nullptr)
  , Case8NonElFrom_kaon_all(nullptr)
  , sameMIDcase8NonEPdg_all(nullptr)
  , sameMIDcase8NonEMotherPdg_all(nullptr)
  , sameMIDcase8NonEMotherIM_all(nullptr)
  , sameMIDcase8NonEPdgFromTarget_all(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_all(nullptr)
  , sameMIDcase8NonEComeFromTargetP_all(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_all(nullptr)
  , fHistoList_bg_zero()
  , BG1_zero(nullptr)
  , BG2_zero(nullptr)
  , BG3_zero(nullptr)
  , BG4_zero(nullptr)
  , BG5_zero(nullptr)
  , BG6_zero(nullptr)
  , BG7_zero(nullptr)
  , BG8_zero(nullptr)
  , BG9_zero(nullptr)
  , BG10_zero(nullptr)
  , PdgCase8_zero(nullptr)
  , PdgCase8mothers_zero(nullptr)
  , sameMIDcase8_zero(nullptr)
  , sameGRIDcase8_zero(nullptr)
  , Case1ZYPos_zero(nullptr)
  , sameMIDcase8_mothedPDG_zero(nullptr)
  , PdgCase8NonEComeFromTarget_zero(nullptr)
  , PdgCase8NonE_NOT_FromTarget_zero(nullptr)
  , PdgCase8motherNonE_zero(nullptr)
  , Case8ElFromDalitz_zero(nullptr)
  , Case8NonElFrom_pn_zero(nullptr)
  , Case8NonElFrom_eta_zero(nullptr)
  , Case8NonElFrom_kaon_zero(nullptr)
  , sameMIDcase8NonEPdg_zero(nullptr)
  , sameMIDcase8NonEMotherPdg_zero(nullptr)
  , sameMIDcase8NonEMotherIM_zero(nullptr)
  , sameMIDcase8NonEPdgFromTarget_zero(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_zero(nullptr)
  , sameMIDcase8NonEComeFromTargetP_zero(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_zero(nullptr)
  , fHistoList_bg_one()
  , BG1_one(nullptr)
  , BG2_one(nullptr)
  , BG3_one(nullptr)
  , BG4_one(nullptr)
  , BG5_one(nullptr)
  , BG6_one(nullptr)
  , BG7_one(nullptr)
  , BG8_one(nullptr)
  , BG9_one(nullptr)
  , BG10_one(nullptr)
  , PdgCase8_one(nullptr)
  , PdgCase8mothers_one(nullptr)
  , sameMIDcase8_one(nullptr)
  , sameGRIDcase8_one(nullptr)
  , Case1ZYPos_one(nullptr)
  , sameMIDcase8_mothedPDG_one(nullptr)
  , PdgCase8NonEComeFromTarget_one(nullptr)
  , PdgCase8NonE_NOT_FromTarget_one(nullptr)
  , PdgCase8motherNonE_one(nullptr)
  , Case8ElFromDalitz_one(nullptr)
  , Case8NonElFrom_pn_one(nullptr)
  , Case8NonElFrom_eta_one(nullptr)
  , Case8NonElFrom_kaon_one(nullptr)
  , sameMIDcase8NonEPdg_one(nullptr)
  , sameMIDcase8NonEMotherPdg_one(nullptr)
  , sameMIDcase8NonEMotherIM_one(nullptr)
  , sameMIDcase8NonEPdgFromTarget_one(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_one(nullptr)
  , sameMIDcase8NonEComeFromTargetP_one(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_one(nullptr)
  , fHistoList_bg_two()
  , BG1_two(nullptr)
  , BG2_two(nullptr)
  , BG3_two(nullptr)
  , BG4_two(nullptr)
  , BG5_two(nullptr)
  , BG6_two(nullptr)
  , BG7_two(nullptr)
  , BG8_two(nullptr)
  , BG9_two(nullptr)
  , BG10_two(nullptr)
  , PdgCase8_two(nullptr)
  , PdgCase8mothers_two(nullptr)
  , sameMIDcase8_two(nullptr)
  , sameGRIDcase8_two(nullptr)
  , Case1ZYPos_two(nullptr)
  , sameMIDcase8_mothedPDG_two(nullptr)
  , PdgCase8NonEComeFromTarget_two(nullptr)
  , PdgCase8NonE_NOT_FromTarget_two(nullptr)
  , PdgCase8motherNonE_two(nullptr)
  , Case8ElFromDalitz_two(nullptr)
  , Case8NonElFrom_pn_two(nullptr)
  , Case8NonElFrom_eta_two(nullptr)
  , Case8NonElFrom_kaon_two(nullptr)
  , sameMIDcase8NonEPdg_two(nullptr)
  , sameMIDcase8NonEMotherPdg_two(nullptr)
  , sameMIDcase8NonEMotherIM_two(nullptr)
  , sameMIDcase8NonEPdgFromTarget_two(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_two(nullptr)
  , sameMIDcase8NonEComeFromTargetP_two(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_two(nullptr)
  , fHistoList_bg_onetwo()
  , BG1_onetwo(nullptr)
  , BG2_onetwo(nullptr)
  , BG3_onetwo(nullptr)
  , BG4_onetwo(nullptr)
  , BG5_onetwo(nullptr)
  , BG6_onetwo(nullptr)
  , BG7_onetwo(nullptr)
  , BG8_onetwo(nullptr)
  , BG9_onetwo(nullptr)
  , BG10_onetwo(nullptr)
  , PdgCase8_onetwo(nullptr)
  , PdgCase8mothers_onetwo(nullptr)
  , sameMIDcase8_onetwo(nullptr)
  , sameGRIDcase8_onetwo(nullptr)
  , Case1ZYPos_onetwo(nullptr)
  , sameMIDcase8_mothedPDG_onetwo(nullptr)
  , PdgCase8NonEComeFromTarget_onetwo(nullptr)
  , PdgCase8NonE_NOT_FromTarget_onetwo(nullptr)
  , PdgCase8motherNonE_onetwo(nullptr)
  , Case8ElFromDalitz_onetwo(nullptr)
  , Case8NonElFrom_pn_onetwo(nullptr)
  , Case8NonElFrom_eta_onetwo(nullptr)
  , Case8NonElFrom_kaon_onetwo(nullptr)
  , sameMIDcase8NonEPdg_onetwo(nullptr)
  , sameMIDcase8NonEMotherPdg_onetwo(nullptr)
  , sameMIDcase8NonEMotherIM_onetwo(nullptr)
  , sameMIDcase8NonEPdgFromTarget_onetwo(nullptr)
  , sameMIDcase8NonEComeFromTargetIM_onetwo(nullptr)
  , sameMIDcase8NonEComeFromTargetP_onetwo(nullptr)
  , sameMIDcase8NonEComeFromTargetPt_onetwo(nullptr)
{
}

CbmKresConversionKF::~CbmKresConversionKF() {}

void CbmKresConversionKF::SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA)
{
  fKFparticle         = kfparticle;
  fKFparticleFinderQA = kfparticleQA;
  if (fKFparticle) { cout << "CbmKresConversionKF: kf works" << endl; }
  else {
    cout << "CbmKresConversionKF: kf does not work" << endl;
  }
}

void CbmKresConversionKF::Init()
{
  fKFtopo = fKFparticle->GetTopoReconstructor();

  fTauFit = new CbmRichRingFitterEllipseTau();

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmKresConversionKF::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMcTracks) { Fatal("CbmKresConversionKF::Init", "No MCTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmKresConversionKF::Init", "No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmKresConversionKF::Init", "No StsTrackMatch array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmKresConversionKF::Init", "No GlobalTrack array!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmKresConversionKF::Init", "No RichRingMatch array!"); }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) { Fatal("CbmKresConversionKF::Init", "No RichProjection array!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmKresConversionKF::Init", "No RichRing array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmKresConversionKF::Init", "No RichHit array!"); }

  InitHistograms();

  fAnaBG = new CbmKresConversionBG();
  fAnaBG->Init();
}


void CbmKresConversionKF::Exec(int fEventNumKF, double OpeningAngleCut, double GammaInvMassCut, int RealPID)
{
  cout << "CbmKresConversionKF, event No. " << fEventNumKF << endl;

  //***** extract all particles from KFParticleFinder
  vector<KFParticle> particlevector;
  particlevector = fKFtopo->GetParticles();

  //***** extract from all particles in KFParticleFinder only gammas
  vector<KFParticle> allgammas;
  allgammas.clear();
  for (size_t vv = 0; vv < particlevector.size(); vv++) {
    if (particlevector[vv].KFParticleBase::GetPDG() == 22) {               // particle is gamma
      if (particlevector[vv].KFParticleBase::NDaughters() != 2) continue;  // check - if gamma has two particles
      allgammas.push_back(particlevector[vv]);
    }
  }
  cout << "number of all gammas from KFParticleFinder before any cuts = " << allgammas.size() << endl;

  //***** sort gammas via number of identified leptons in RICH
  GammasAll.clear();
  GammasZero.clear();
  GammasOne.clear();
  GammasTwo.clear();
  GammasOneTwo.clear();
  GammasAllStsIndex.clear();
  GammasZeroStsIndex.clear();
  GammasOneStsIndex.clear();
  GammasTwoStsIndex.clear();
  GammasOneTwoStsIndex.clear();
  GammasAllMC.clear();
  GammasZeroMC.clear();
  GammasOneMC.clear();
  GammasTwoMC.clear();
  GammasOneTwoMC.clear();
  GammasAllZ.clear();
  GammasZeroZ.clear();
  GammasOneZ.clear();
  GammasTwoZ.clear();
  GammasOneTwoZ.clear();


  FindGammas(allgammas, particlevector, fEventNumKF, OpeningAngleCut, GammaInvMassCut, RealPID);


  FindPi0("All", GammasAll, GammasAllStsIndex, GammasAllMC, GammasAllZ, fPi0InvMassRecoKF_All, fPi0_pt_vs_rap_All,
          fPi0_pt_vs_rap_est_All, fPi0InvMassRecoKF_All_target, fPi0InvMassRecoKF_All_mvd, fPi0InvMassRecoKF_All_sts,
          fPi0InvMassRecoKF_All_outside, MultiplicityGamma_All, MultiplicityChargedParticles_All, fHistoList_bg_all);

  FindPi0("Zero", GammasZero, GammasZeroStsIndex, GammasZeroMC, GammasZeroZ, fPi0InvMassRecoKF_Zero,
          fPi0_pt_vs_rap_Zero, fPi0_pt_vs_rap_est_Zero, fPi0InvMassRecoKF_Zero_target, fPi0InvMassRecoKF_Zero_mvd,
          fPi0InvMassRecoKF_Zero_sts, fPi0InvMassRecoKF_Zero_outside, MultiplicityGamma_Zero,
          MultiplicityChargedParticles_Zero, fHistoList_bg_zero);

  FindPi0("One", GammasOne, GammasOneStsIndex, GammasOneMC, GammasOneZ, fPi0InvMassRecoKF_One, fPi0_pt_vs_rap_One,
          fPi0_pt_vs_rap_est_One, fPi0InvMassRecoKF_One_target, fPi0InvMassRecoKF_One_mvd, fPi0InvMassRecoKF_One_sts,
          fPi0InvMassRecoKF_One_outside, MultiplicityGamma_One, MultiplicityChargedParticles_One, fHistoList_bg_one);

  FindPi0("Two", GammasTwo, GammasTwoStsIndex, GammasTwoMC, GammasTwoZ, fPi0InvMassRecoKF_Two, fPi0_pt_vs_rap_Two,
          fPi0_pt_vs_rap_est_Two, fPi0InvMassRecoKF_Two_target, fPi0InvMassRecoKF_Two_mvd, fPi0InvMassRecoKF_Two_sts,
          fPi0InvMassRecoKF_Two_outside, MultiplicityGamma_Two, MultiplicityChargedParticles_Two, fHistoList_bg_two);

  FindPi0("OneTwo", GammasOneTwo, GammasOneTwoStsIndex, GammasOneTwoMC, GammasOneTwoZ, fPi0InvMassRecoKF_OneTwo,
          fPi0_pt_vs_rap_OneTwo, fPi0_pt_vs_rap_est_OneTwo, fPi0InvMassRecoKF_OneTwo_target,
          fPi0InvMassRecoKF_OneTwo_mvd, fPi0InvMassRecoKF_OneTwo_sts, fPi0InvMassRecoKF_OneTwo_outside,
          MultiplicityGamma_OneTwo, MultiplicityChargedParticles_OneTwo, fHistoList_bg_onetwo);


  if (fEventNumKF % 500 == 0) {
    MixedEvent();
    EMT_Event.clear();
    EMT_pair_momenta.clear();
    EMT_NofRings.clear();
    EMT_Z.clear();
  }

  if (fEventNumKF % 1000 == 0) {
    MixedEventMulti();
    EMT_Event_multi_all.clear();
    EMT_pair_momenta_multi_all.clear();
    EMT_Event_multi_zero.clear();
    EMT_pair_momenta_multi_zero.clear();
    EMT_Event_multi_one.clear();
    EMT_pair_momenta_multi_one.clear();
    EMT_Event_multi_two.clear();
    EMT_pair_momenta_multi_two.clear();
    EMT_Event_multi_onetwo.clear();
    EMT_pair_momenta_multi_onetwo.clear();
    EMT_multi_all.clear();
    EMT_multi_one.clear();
    EMT_multi_two.clear();
    EMT_multi_zero.clear();
    EMT_multi_onetwo.clear();
  }


  // pi0, which maksym accept
  vector<vector<KFParticle>> primpi0;
  primpi0 = fKFtopo->GetKFParticleFinder()->GetPrimaryPi0();
  vector<KFParticle> primpi0inside;
  primpi0inside = primpi0.at(0);
  for (size_t tt = 0; tt < primpi0inside.size(); tt++) {
    cout << "\t *********** primpi0->GetPt = " << primpi0inside[tt].GetPt()
         << "; ->GetMass = " << primpi0inside[tt].GetMass() << "; ->GetX = " << primpi0inside[tt].GetX()
         << "; ->GetY = " << primpi0inside[tt].GetY() << "; ->GetZ = " << primpi0inside[tt].GetZ()
         << "; ->GetE = " << primpi0inside[tt].GetE() << endl;
  }
}


void CbmKresConversionKF::FindGammas(vector<KFParticle> allgammas, vector<KFParticle> particlevector, int Event,
                                     double AngleCut, double InvMassCut, int RealPID)
{
  for (size_t tt = 0; tt < allgammas.size(); tt++) {
    if (allgammas[tt].GetZ() > 75 || allgammas[tt].GetZ() < -5) continue;
    std::vector<int> electronIds = allgammas[tt].KFParticleBase::DaughterIds();
    std::vector<int> grDaughter0 = particlevector[electronIds.at(0)].KFParticleBase::DaughterIds();
    std::vector<int> grDaughter1 = particlevector[electronIds.at(1)].KFParticleBase::DaughterIds();
    if (grDaughter0.size() != 1 || grDaughter1.size() != 1) continue;  // check that it made only two particles

    // STS ind
    CbmStsTrack* stsTrack0 = (CbmStsTrack*) fStsTracks->At(grDaughter0.at(0));
    CbmStsTrack* stsTrack1 = (CbmStsTrack*) fStsTracks->At(grDaughter1.at(0));
    if (stsTrack0 == nullptr || stsTrack1 == nullptr) continue;
    CbmTrackMatchNew* stsMatch0 = (CbmTrackMatchNew*) fStsTrackMatches->At(grDaughter0.at(0));
    CbmTrackMatchNew* stsMatch1 = (CbmTrackMatchNew*) fStsTrackMatches->At(grDaughter1.at(0));
    if (stsMatch0 == nullptr || stsMatch1 == nullptr) continue;
    if (stsMatch0->GetNofLinks() <= 0 || stsMatch1->GetNofLinks() <= 0) continue;
    int stsMcTrackId0 = stsMatch0->GetMatchedLink().GetIndex();
    int stsMcTrackId1 = stsMatch1->GetMatchedLink().GetIndex();
    if (stsMcTrackId0 < 0 || stsMcTrackId1 < 0) continue;
    CbmMCTrack* mcTrack0 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId0);
    CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId1);
    if (mcTrack0 == nullptr || mcTrack1 == nullptr) continue;

    TVector3 refmomentum0 =
      CbmKresFunctions::FitToVertex(stsTrack0, allgammas[tt].GetX(), allgammas[tt].GetY(), allgammas[tt].GetZ());
    TVector3 refmomentum1 =
      CbmKresFunctions::FitToVertex(stsTrack1, allgammas[tt].GetX(), allgammas[tt].GetY(), allgammas[tt].GetZ());

    // RICH ind
    Int_t ngTracks = fGlobalTracks->GetEntriesFast();
    int richInd0   = 99999;
    int richInd1   = 99999;
    for (Int_t iTr = 0; iTr < ngTracks; iTr++) {
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTr);
      if (nullptr == gTrack) continue;
      int stsInd = gTrack->GetStsTrackIndex();
      if (stsInd < 0) continue;
      if (stsInd == grDaughter0.at(0)) {
        if (gTrack->GetRichRingIndex() > -1) richInd0 = gTrack->GetRichRingIndex();
      }
      if (stsInd == grDaughter1.at(0)) {
        if (gTrack->GetRichRingIndex() > -1) richInd1 = gTrack->GetRichRingIndex();
      }
    }

    int richcheck_0 = 0;
    int richcheck_1 = 0;
    //***** MCPID for RICH identification
    if (RealPID != 1) {
      if (richInd0 != 99999) {
        CbmTrackMatchNew* richMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd0);
        if (richMatch == nullptr) continue;
        if (richMatch->GetNofLinks() <= 0) continue;
        int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
        if (richMcTrackId < 0) continue;
        if (stsMcTrackId0 != richMcTrackId)
          continue;  // check that global track was matched correctly for STS and RICH together
        CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
        if (mcTrack2 == nullptr) continue;
        int pdgRICH = mcTrack2->GetPdgCode();
        if (TMath::Abs(pdgRICH) == 11) richcheck_0++;
      }
      if (richInd1 != 99999) {
        CbmTrackMatchNew* richMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd1);
        if (richMatch == nullptr) continue;
        if (richMatch->GetNofLinks() <= 0) continue;
        int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
        if (richMcTrackId < 0) continue;
        if (stsMcTrackId1 != richMcTrackId)
          continue;  // check that global track was matched correctly for STS and RICH together
        CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
        if (mcTrack2 == nullptr) continue;
        int pdgRICH = mcTrack2->GetPdgCode();
        if (TMath::Abs(pdgRICH) == 11) richcheck_1++;
      }
    }

    // Real PID for RICH identification
    if (RealPID == 1) {
      if (richInd0 != 99999) {
        CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(richInd0));
        richcheck_0           = CheckIfElectron(richRing, refmomentum0.Mag());
      }
      if (richInd1 != 99999) {
        CbmRichRing* richRing = static_cast<CbmRichRing*>(fRichRings->At(richInd1));
        richcheck_1           = CheckIfElectron(richRing, refmomentum1.Mag());
      }
    }
    // Real RICH PID (END).

    int richcheck = richcheck_0 + richcheck_1;


    FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(grDaughter0.at(0));
    if (richcheck_0 == 0 && proj->GetX() > -115 && proj->GetX() < 115
        && ((proj->GetY() < -120 && proj->GetY() > -200) || (proj->GetY() > 120 && proj->GetY() < 200)))
      continue;
    FairTrackParam* proj2 = (FairTrackParam*) fRichProjections->At(grDaughter1.at(0));
    if (richcheck_1 == 0 && proj2->GetX() > -115 && proj2->GetX() < 115
        && ((proj2->GetY() < -120 && proj2->GetY() > -200) || (proj2->GetY() > 120 && proj2->GetY() < 200)))
      continue;


    //cout << "=================" << endl;
    //cout << "FitToVertex part1: px = " << refmomentum0.X() << "; \t py = " << refmomentum0.Y() << "; \t pz = " << refmomentum0.Z() << endl;
    //cout << "FitToVertex part2: px = " << refmomentum1.X() << "; \t py = " << refmomentum1.Y() << "; \t pz = " << refmomentum1.Z() << endl;
    //cout << "KFParticle  part1: px = " << particlevector[electronIds.at(0)].KFParticleBase::Px() << "; \t py = " << particlevector[electronIds.at(0)].KFParticleBase::Py() << "; \t pz = " << particlevector[electronIds.at(0)].KFParticleBase::Pz() << endl;
    //cout << "KFParticle  part2: px = " << particlevector[electronIds.at(1)].KFParticleBase::Px() << "; \t py = " << particlevector[electronIds.at(1)].KFParticleBase::Py() << "; \t pz = " << particlevector[electronIds.at(1)].KFParticleBase::Pz() << endl;
    //TVector3 refmomentum0(particlevector[electronIds.at(0)].KFParticleBase::Px(), particlevector[electronIds.at(0)].KFParticleBase::Py(), particlevector[electronIds.at(0)].KFParticleBase::Pz());
    //TVector3 refmomentum1(particlevector[electronIds.at(1)].KFParticleBase::Px(), particlevector[electronIds.at(1)].KFParticleBase::Py(), particlevector[electronIds.at(1)].KFParticleBase::Pz());


    frefmomentum.clear();
    frefmomentum.push_back(refmomentum0);
    frefmomentum.push_back(refmomentum1);
    fmcvector.clear();
    fmcvector.push_back(mcTrack0);
    fmcvector.push_back(mcTrack1);
    fStsInd.clear();
    fStsInd.push_back(grDaughter0.at(0));
    fStsInd.push_back(grDaughter1.at(0));

    Double_t invmassTrue      = CbmKresFunctions::Invmass_2particles_MC(fmcvector.at(0), fmcvector.at(1));
    Double_t invmassReco      = CbmKresFunctions::Invmass_2particles_RECO(frefmomentum.at(0), frefmomentum.at(1));
    Double_t opening_angle_mc = CbmKresFunctions::CalculateOpeningAngle_MC(fmcvector.at(0), fmcvector.at(1));
    Double_t opening_angle_refitted =
      CbmKresFunctions::CalculateOpeningAngle_Reco(frefmomentum.at(0), frefmomentum.at(1));


    // graphs for understanding cuts:
    CheckForCuts_InvMass_MC->Fill(invmassTrue);
    CheckForCuts_InvMass_Reco->Fill(invmassReco);
    CheckForCuts_OA_MC->Fill(opening_angle_mc);
    CheckForCuts_OA_Reco->Fill(opening_angle_refitted);


    if (TMath::Abs(mcTrack0->GetPdgCode()) == 11 && (mcTrack0->GetPdgCode() + mcTrack1->GetPdgCode()) == 0
        && (mcTrack0->GetMotherId() == mcTrack1->GetMotherId()) && mcTrack0->GetMotherId() != -1) {
      CbmMCTrack* mcTrackMother = (CbmMCTrack*) fMcTracks->At(mcTrack0->GetMotherId());
      if (mcTrackMother != nullptr && mcTrackMother->GetPdgCode() == 22
          && mcTrackMother->GetMotherId() != -1) {  // electrons/positrons from gamma
        CbmMCTrack* mcTrackMotherOfGamma = (CbmMCTrack*) fMcTracks->At(mcTrackMother->GetMotherId());  // pi0
        if (mcTrackMotherOfGamma->GetPdgCode() == 111) {
          CheckForCuts_OA_MC_from_one_pi0->Fill(opening_angle_mc);
          CheckForCuts_OA_Reco_from_one_pi0->Fill(opening_angle_refitted);
          CheckForCuts_InvMass_MC_from_one_pi0->Fill(invmassTrue);
          CheckForCuts_InvMass_Reco_from_one_pi0->Fill(invmassReco);
          CheckForCuts_z_vs_InvMass_MC_from_one_pi0->Fill(mcTrack0->GetStartZ(), invmassTrue);
          CheckForCuts_z_vs_OA_MC_from_one_pi0->Fill(mcTrack0->GetStartZ(), opening_angle_mc);
          CheckForCuts_z_vs_InvMass_Reco_from_one_pi0->Fill(allgammas[tt].GetZ(), invmassReco);
          CheckForCuts_z_vs_OA_Reco_from_one_pi0->Fill(allgammas[tt].GetZ(), opening_angle_refitted);
          if (allgammas[tt].GetZ() <= 4) {
            CheckForCuts_InvMass_Reco_from_one_pi0_less4cm->Fill(invmassReco);
            CheckForCuts_OA_Reco_from_one_pi0_less4cm->Fill(opening_angle_refitted);
          }
          if (allgammas[tt].GetZ() <= 21 && allgammas[tt].GetZ() > 4) {
            CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm->Fill(invmassReco);
            CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm->Fill(opening_angle_refitted);
          }
          if (allgammas[tt].GetZ() > 21) {
            CheckForCuts_InvMass_Reco_from_one_pi0_more21cm->Fill(invmassReco);
            CheckForCuts_OA_Reco_from_one_pi0_more21cm->Fill(opening_angle_refitted);
          }
        }
      }
    }


    // cuts
    if (TMath::Abs(opening_angle_refitted) > AngleCut) continue;
    if (TMath::Abs(invmassReco) > InvMassCut) continue;


    // for event mixing
    EMT_Event.push_back(Event);
    EMT_pair_momenta.push_back(frefmomentum);
    EMT_NofRings.push_back(richcheck);
    EMT_Z.push_back(allgammas[tt].GetZ());


    // everything (RICH == 0, RICH == 1, RICH == 2) together
    if (richcheck == 0 || richcheck == 1 || richcheck == 2) {
      // for event mixing multi
      EMT_Event_multi_all.push_back(Event);
      EMT_pair_momenta_multi_all.push_back(frefmomentum);

      GammasAll.push_back(frefmomentum);
      GammasAllStsIndex.push_back(fStsInd);
      GammasAllMC.push_back(fmcvector);
      GammasAllZ.push_back(allgammas[tt].GetZ());

      fGammaInvMassReco_All->Fill(invmassReco);
      fGammaOpeningAngleReco_All->Fill(opening_angle_refitted);
      fPdg_All->Fill(TMath::Abs(mcTrack0->GetPdgCode()));
      fPdg_All->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      fP_reco_All->Fill(refmomentum0.Mag());
      fP_reco_All->Fill(refmomentum1.Mag());
      fPt_reco_All->Fill(refmomentum0.Perp());
      fPt_reco_All->Fill(refmomentum1.Perp());
      if (allgammas[tt].GetZ() < 4) {
        fGammaInvMassReco_All_target->Fill(invmassReco);
        fGammaOpeningAngleReco_All_target->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4 && allgammas[tt].GetZ() < 21) {
        fGammaInvMassReco_All_mvd->Fill(invmassReco);
        fGammaOpeningAngleReco_All_mvd->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 21 && allgammas[tt].GetZ() < 75) {
        fGammaInvMassReco_All_sts->Fill(invmassReco);
        fGammaOpeningAngleReco_All_sts->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4) {
        fGammaInvMassReco_All_outside->Fill(invmassReco);
        fGammaOpeningAngleReco_All_outside->Fill(opening_angle_refitted);
      }
    }


    // only cases, when RICH == 0
    if (richcheck == 0) {
      // for event mixing multi
      EMT_Event_multi_zero.push_back(Event);
      EMT_pair_momenta_multi_zero.push_back(frefmomentum);

      GammasZero.push_back(frefmomentum);
      GammasZeroStsIndex.push_back(fStsInd);
      GammasZeroMC.push_back(fmcvector);
      GammasZeroZ.push_back(allgammas[tt].GetZ());

      fGammaInvMassReco_Zero->Fill(invmassReco);
      fGammaOpeningAngleReco_Zero->Fill(opening_angle_refitted);
      fPdg_Zero->Fill(TMath::Abs(mcTrack0->GetPdgCode()));
      fPdg_Zero->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      fP_reco_Zero->Fill(refmomentum0.Mag());
      fP_reco_Zero->Fill(refmomentum1.Mag());
      fPt_reco_Zero->Fill(refmomentum0.Perp());
      fPt_reco_Zero->Fill(refmomentum1.Perp());
      if (allgammas[tt].GetZ() < 4) {
        fGammaInvMassReco_Zero_target->Fill(invmassReco);
        fGammaOpeningAngleReco_Zero_target->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4 && allgammas[tt].GetZ() < 21) {
        fGammaInvMassReco_Zero_mvd->Fill(invmassReco);
        fGammaOpeningAngleReco_Zero_mvd->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 21 && allgammas[tt].GetZ() < 75) {
        fGammaInvMassReco_Zero_sts->Fill(invmassReco);
        fGammaOpeningAngleReco_Zero_sts->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4) {
        fGammaInvMassReco_Zero_outside->Fill(invmassReco);
        fGammaOpeningAngleReco_Zero_outside->Fill(opening_angle_refitted);
      }
    }

    // only cases, when RICH == 1
    if (richcheck == 1) {
      // for event mixing multi
      EMT_Event_multi_one.push_back(Event);
      EMT_pair_momenta_multi_one.push_back(frefmomentum);

      GammasOne.push_back(frefmomentum);
      GammasOneStsIndex.push_back(fStsInd);
      GammasOneMC.push_back(fmcvector);
      GammasOneZ.push_back(allgammas[tt].GetZ());

      fGammaInvMassReco_One->Fill(invmassReco);
      fGammaOpeningAngleReco_One->Fill(opening_angle_refitted);
      fPdg_One->Fill(TMath::Abs(mcTrack0->GetPdgCode()));
      fPdg_One->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      fP_reco_One->Fill(refmomentum0.Mag());
      fP_reco_One->Fill(refmomentum1.Mag());
      fPt_reco_One->Fill(refmomentum0.Perp());
      fPt_reco_One->Fill(refmomentum1.Perp());
      if (allgammas[tt].GetZ() < 4) {
        fGammaInvMassReco_One_target->Fill(invmassReco);
        fGammaOpeningAngleReco_One_target->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4 && allgammas[tt].GetZ() < 21) {
        fGammaInvMassReco_One_mvd->Fill(invmassReco);
        fGammaOpeningAngleReco_One_mvd->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 21 && allgammas[tt].GetZ() < 75) {
        fGammaInvMassReco_One_sts->Fill(invmassReco);
        fGammaOpeningAngleReco_One_sts->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4) {
        fGammaInvMassReco_One_outside->Fill(invmassReco);
        fGammaOpeningAngleReco_One_outside->Fill(opening_angle_refitted);
      }
    }

    // only cases, when RICH == 2
    if (richcheck == 2) {
      // for event mixing multi
      EMT_Event_multi_two.push_back(Event);
      EMT_pair_momenta_multi_two.push_back(frefmomentum);

      GammasTwo.push_back(frefmomentum);
      GammasTwoStsIndex.push_back(fStsInd);
      GammasTwoMC.push_back(fmcvector);
      GammasTwoZ.push_back(allgammas[tt].GetZ());

      fGammaInvMassReco_Two->Fill(invmassReco);
      fGammaOpeningAngleReco_Two->Fill(opening_angle_refitted);
      fPdg_Two->Fill(TMath::Abs(mcTrack0->GetPdgCode()));
      fPdg_Two->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      fP_reco_Two->Fill(refmomentum0.Mag());
      fP_reco_Two->Fill(refmomentum1.Mag());
      fPt_reco_Two->Fill(refmomentum0.Perp());
      fPt_reco_Two->Fill(refmomentum1.Perp());
      if (allgammas[tt].GetZ() < 4) {
        fGammaInvMassReco_Two_target->Fill(invmassReco);
        fGammaOpeningAngleReco_Two_target->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4 && allgammas[tt].GetZ() < 21) {
        fGammaInvMassReco_Two_mvd->Fill(invmassReco);
        fGammaOpeningAngleReco_Two_mvd->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 21 && allgammas[tt].GetZ() < 75) {
        fGammaInvMassReco_Two_sts->Fill(invmassReco);
        fGammaOpeningAngleReco_Two_sts->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4) {
        fGammaInvMassReco_Two_outside->Fill(invmassReco);
        fGammaOpeningAngleReco_Two_outside->Fill(opening_angle_refitted);
      }
    }

    // cases, when RICH == 1 or RICH == 2 together
    if (richcheck == 1 || richcheck == 2) {
      // for event mixing multi
      EMT_Event_multi_onetwo.push_back(Event);
      EMT_pair_momenta_multi_onetwo.push_back(frefmomentum);

      GammasOneTwo.push_back(frefmomentum);
      GammasOneTwoStsIndex.push_back(fStsInd);
      GammasOneTwoMC.push_back(fmcvector);
      GammasOneTwoZ.push_back(allgammas[tt].GetZ());

      fGammaInvMassReco_OneTwo->Fill(invmassReco);
      fGammaOpeningAngleReco_OneTwo->Fill(opening_angle_refitted);
      fPdg_OneTwo->Fill(TMath::Abs(mcTrack0->GetPdgCode()));
      fPdg_OneTwo->Fill(TMath::Abs(mcTrack1->GetPdgCode()));
      fP_reco_OneTwo->Fill(refmomentum0.Mag());
      fP_reco_OneTwo->Fill(refmomentum1.Mag());
      fPt_reco_OneTwo->Fill(refmomentum0.Perp());
      fPt_reco_OneTwo->Fill(refmomentum1.Perp());
      if (allgammas[tt].GetZ() < 4) {
        fGammaInvMassReco_OneTwo_target->Fill(invmassReco);
        fGammaOpeningAngleReco_OneTwo_target->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4 && allgammas[tt].GetZ() < 21) {
        fGammaInvMassReco_OneTwo_mvd->Fill(invmassReco);
        fGammaOpeningAngleReco_OneTwo_mvd->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 21 && allgammas[tt].GetZ() < 75) {
        fGammaInvMassReco_OneTwo_sts->Fill(invmassReco);
        fGammaOpeningAngleReco_OneTwo_sts->Fill(opening_angle_refitted);
      }
      if (allgammas[tt].GetZ() > 4) {
        fGammaInvMassReco_OneTwo_outside->Fill(invmassReco);
        fGammaOpeningAngleReco_OneTwo_outside->Fill(opening_angle_refitted);
      }
    }
  }

  //cout << "number of gammas with 0-2 electron identified in RICH = " << GammasAll.size() << endl;
  //cout << "number of gammas with  0  electron identified in RICH = " << GammasZero.size() << endl;
  //cout << "number of gammas with  1  electron identified in RICH = " << GammasOne.size() << endl;
  //cout << "number of gammas with  2  electron identified in RICH = " << GammasTwo.size() << endl;
  //cout << "number of gammas with 1-2 electron identified in RICH = " << GammasOneTwo.size() << endl;

  for (size_t kk = 0; kk < GammasAll.size(); kk++) {
    EMT_multi_all.push_back(GammasAll.size());
  }
  for (size_t kk = 0; kk < GammasZero.size(); kk++) {
    EMT_multi_zero.push_back(GammasZero.size());
  }
  for (size_t kk = 0; kk < GammasOne.size(); kk++) {
    EMT_multi_one.push_back(GammasOne.size());
  }
  for (size_t kk = 0; kk < GammasTwo.size(); kk++) {
    EMT_multi_two.push_back(GammasTwo.size());
  }
  for (size_t kk = 0; kk < GammasOneTwo.size(); kk++) {
    EMT_multi_onetwo.push_back(GammasOneTwo.size());
  }
}


void CbmKresConversionKF::FindPi0(TString /*mod*/, vector<vector<TVector3>> Gammas, vector<vector<int>> StsIndex,
                                  vector<vector<CbmMCTrack*>> GammasMC, vector<Double_t> GammasZ,
                                  TH1D* Pi0InvMassRecoKF, TH2D* Pi0_pt_vs_rap, TH2D* Pi0_pt_vs_rap_est,
                                  TH1D* Pi0InvMassRecoKF_target, TH1D* Pi0InvMassRecoKF_mvd, TH1D* Pi0InvMassRecoKF_sts,
                                  TH1D* Pi0InvMassRecoKF_outside, TH2D* MultiplicityGamma,
                                  TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases)

{
  // combine all gamma in pi0 --> calculate inv mass for pi0 // not the case, when one particle used twice for different gammas
  if (Gammas.size() < 2) return;  // min 2 gammas to form pi0 are required

  for (size_t gamma1 = 0; gamma1 < Gammas.size() - 1; gamma1++) {
    for (size_t gamma2 = gamma1 + 1; gamma2 < Gammas.size(); gamma2++) {

      if (StsIndex[gamma1][0] == StsIndex[gamma2][0] || StsIndex[gamma1][0] == StsIndex[gamma2][1]
          || StsIndex[gamma1][1] == StsIndex[gamma2][0] || StsIndex[gamma1][1] == StsIndex[gamma2][1])
        continue;  // particles not used twice --> different

      // 4 reconstructed particles from gammas
      TVector3 e1 = Gammas[gamma1][0];
      TVector3 e2 = Gammas[gamma1][1];
      TVector3 e3 = Gammas[gamma2][0];
      TVector3 e4 = Gammas[gamma2][1];

      // MC true data for this particles
      CbmMCTrack* mcTrack1 = GammasMC[gamma1][0];
      CbmMCTrack* mcTrack2 = GammasMC[gamma1][1];
      CbmMCTrack* mcTrack3 = GammasMC[gamma2][0];
      CbmMCTrack* mcTrack4 = GammasMC[gamma2][1];

      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e1, e2, e3, e4);

      Pi0InvMassRecoKF->Fill(params.fMinv);

      MultiplicityGamma->Fill(Gammas.size(), params.fMinv);
      MultiplicityChargedParticles->Fill(fGlobalTracks->GetEntriesFast(), params.fMinv);

      // check reconsrtucted z position of conversion
      if (GammasZ[gamma1] < 4 && GammasZ[gamma2] < 4) { Pi0InvMassRecoKF_target->Fill(params.fMinv); }
      if (GammasZ[gamma1] > 4 && GammasZ[gamma1] < 21 && GammasZ[gamma2] > 4 && GammasZ[gamma2] < 21) {
        Pi0InvMassRecoKF_mvd->Fill(params.fMinv);
      }
      if (GammasZ[gamma1] > 21 && GammasZ[gamma1] < 75 && GammasZ[gamma2] > 21 && GammasZ[gamma2] < 75) {
        Pi0InvMassRecoKF_sts->Fill(params.fMinv);
      }
      if (GammasZ[gamma1] > 4 && GammasZ[gamma2] > 4) { Pi0InvMassRecoKF_outside->Fill(params.fMinv); }

      fAnaBG->Exec(mcTrack1, mcTrack2, mcTrack3, mcTrack4, params.fMinv, BGCases);

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
      }

      if (mcMotherPdg1 == 111 && mcMotherPdg2 == 111 && mcMotherPdg3 == 22 && mcMotherPdg4 == 22) {
        if (grandmotherId3 != motherId1) continue;
        Pi0_pt_vs_rap->Fill(params.fRapidity, params.fPt);
        Pi0_pt_vs_rap_est->Fill(params.fRapidity, params.fPt);
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
      }
    }
  }
}


int CbmKresConversionKF::CheckIfElectron(CbmRichRing* ring, double momentum)
{
  int identified = 0;

  if (nullptr != ring) {
    CbmRichRingLight ringHit;
    int nofHits = ring->GetNofHits();
    for (int i = 0; i < nofHits; i++) {
      Int_t hitInd    = ring->GetHit(i);
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


void CbmKresConversionKF::MixedEvent()
// combines photons from two different events, taken from each time N events
{
  Int_t nof = EMT_Event.size();
  cout << "MixedEvent CbmKresConversionKF - nof entries " << nof << endl;
  for (Int_t a = 0; a < nof - 1; a++) {
    for (Int_t b = a + 1; b < nof; b++) {
      if (EMT_Event[a] == EMT_Event[b]) continue;  // to make sure that the photons are from two different events
      TVector3 e11                  = EMT_pair_momenta[a][0];
      TVector3 e12                  = EMT_pair_momenta[a][1];
      TVector3 e21                  = EMT_pair_momenta[b][0];
      TVector3 e22                  = EMT_pair_momenta[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      fEMT_InvMass_All->Fill(params.fMinv);
      if (EMT_NofRings[a] == 0 && EMT_NofRings[b] == 0) fEMT_InvMass_Zero->Fill(params.fMinv);
      if (EMT_NofRings[a] == 1 && EMT_NofRings[b] == 1) fEMT_InvMass_One->Fill(params.fMinv);
      if (EMT_NofRings[a] == 2 && EMT_NofRings[b] == 2) fEMT_InvMass_Two->Fill(params.fMinv);
      if ((EMT_NofRings[a] == 1 || EMT_NofRings[a] == 2) && (EMT_NofRings[b] == 1 || EMT_NofRings[b] == 2))
        fEMT_InvMass_OneTwo->Fill(params.fMinv);

      // for inside the targte and outside the target
      if (EMT_Z[a] < 4 && EMT_Z[b] < 4) {
        fEMT_InvMass_All_target->Fill(params.fMinv);
        if (EMT_NofRings[a] == 0 && EMT_NofRings[b] == 0) fEMT_InvMass_Zero_target->Fill(params.fMinv);
        if (EMT_NofRings[a] == 1 && EMT_NofRings[b] == 1) fEMT_InvMass_One_target->Fill(params.fMinv);
        if (EMT_NofRings[a] == 2 && EMT_NofRings[b] == 2) fEMT_InvMass_Two_target->Fill(params.fMinv);
        if ((EMT_NofRings[a] == 1 || EMT_NofRings[a] == 2) && (EMT_NofRings[b] == 1 || EMT_NofRings[b] == 2))
          fEMT_InvMass_OneTwo_target->Fill(params.fMinv);
      }
      if (EMT_Z[a] > 4 && EMT_Z[b] > 4) {
        fEMT_InvMass_All_outside->Fill(params.fMinv);
        if (EMT_NofRings[a] == 0 && EMT_NofRings[b] == 0) fEMT_InvMass_Zero_outside->Fill(params.fMinv);
        if (EMT_NofRings[a] == 1 && EMT_NofRings[b] == 1) fEMT_InvMass_One_outside->Fill(params.fMinv);
        if (EMT_NofRings[a] == 2 && EMT_NofRings[b] == 2) fEMT_InvMass_Two_outside->Fill(params.fMinv);
        if ((EMT_NofRings[a] == 1 || EMT_NofRings[a] == 2) && (EMT_NofRings[b] == 1 || EMT_NofRings[b] == 2))
          fEMT_InvMass_OneTwo_outside->Fill(params.fMinv);
      }
    }
  }
}


void CbmKresConversionKF::MixedEventMulti()
// combines photons from two different events, taken from each time N events
{
  // all
  Int_t nof_all = EMT_Event_multi_all.size();
  cout << "MixedEventMulti CbmKresConversionKF - nof entries all " << nof_all << endl;
  for (Int_t a = 0; a < nof_all - 1; a++) {
    for (Int_t b = a + 1; b < nof_all; b++) {
      if (EMT_Event_multi_all[a] == EMT_Event_multi_all[b])
        continue;  // to make sure that the photons are from two different events
      if (EMT_multi_all[a] != EMT_multi_all[b]) continue;  // check same multiplicity
      TVector3 e11                  = EMT_pair_momenta_multi_all[a][0];
      TVector3 e12                  = EMT_pair_momenta_multi_all[a][1];
      TVector3 e21                  = EMT_pair_momenta_multi_all[b][0];
      TVector3 e22                  = EMT_pair_momenta_multi_all[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      if (EMT_multi_all[a] == 1) EMTMulti_InvMass_All_m1->Fill(params.fMinv);
      if (EMT_multi_all[a] == 2) EMTMulti_InvMass_All_m2->Fill(params.fMinv);
      if (EMT_multi_all[a] == 3) EMTMulti_InvMass_All_m3->Fill(params.fMinv);
      if (EMT_multi_all[a] == 4) EMTMulti_InvMass_All_m4->Fill(params.fMinv);
      if (EMT_multi_all[a] == 5) EMTMulti_InvMass_All_m5->Fill(params.fMinv);
      if (EMT_multi_all[a] == 6) EMTMulti_InvMass_All_m6->Fill(params.fMinv);
      if (EMT_multi_all[a] == 7) EMTMulti_InvMass_All_m7->Fill(params.fMinv);
    }
  }

  // zero
  Int_t nof_zero = EMT_Event_multi_zero.size();
  cout << "MixedEvent - nof entries zero " << nof_zero << endl;
  for (Int_t a = 0; a < nof_zero - 1; a++) {
    for (Int_t b = a + 1; b < nof_zero; b++) {
      if (EMT_Event_multi_zero[a] == EMT_Event_multi_zero[b])
        continue;  // to make sure that the photons are from two different events
      if (EMT_multi_zero[a] != EMT_multi_zero[b]) continue;  // check same multiplicity
      TVector3 e11                  = EMT_pair_momenta_multi_zero[a][0];
      TVector3 e12                  = EMT_pair_momenta_multi_zero[a][1];
      TVector3 e21                  = EMT_pair_momenta_multi_zero[b][0];
      TVector3 e22                  = EMT_pair_momenta_multi_zero[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      if (EMT_multi_zero[a] == 1) EMTMulti_InvMass_Zero_m1->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 2) EMTMulti_InvMass_Zero_m2->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 3) EMTMulti_InvMass_Zero_m3->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 4) EMTMulti_InvMass_Zero_m4->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 5) EMTMulti_InvMass_Zero_m5->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 6) EMTMulti_InvMass_Zero_m6->Fill(params.fMinv);
      if (EMT_multi_zero[a] == 7) EMTMulti_InvMass_Zero_m7->Fill(params.fMinv);
    }
  }

  // one
  Int_t nof_one = EMT_Event_multi_one.size();
  cout << "MixedEvent - nof entries one " << nof_one << endl;
  for (Int_t a = 0; a < nof_one - 1; a++) {
    for (Int_t b = a + 1; b < nof_one; b++) {
      if (EMT_Event_multi_one[a] == EMT_Event_multi_one[b])
        continue;  // to make sure that the photons are from two different events
      if (EMT_multi_one[a] != EMT_multi_one[b]) continue;  // check same multiplicity
      TVector3 e11                  = EMT_pair_momenta_multi_one[a][0];
      TVector3 e12                  = EMT_pair_momenta_multi_one[a][1];
      TVector3 e21                  = EMT_pair_momenta_multi_one[b][0];
      TVector3 e22                  = EMT_pair_momenta_multi_one[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      if (EMT_multi_one[a] == 1) EMTMulti_InvMass_One_m1->Fill(params.fMinv);
      if (EMT_multi_one[a] == 2) EMTMulti_InvMass_One_m2->Fill(params.fMinv);
      if (EMT_multi_one[a] == 3) EMTMulti_InvMass_One_m3->Fill(params.fMinv);
      if (EMT_multi_one[a] == 4) EMTMulti_InvMass_One_m4->Fill(params.fMinv);
      if (EMT_multi_one[a] == 5) EMTMulti_InvMass_One_m5->Fill(params.fMinv);
      if (EMT_multi_one[a] == 6) EMTMulti_InvMass_One_m6->Fill(params.fMinv);
      if (EMT_multi_one[a] == 7) EMTMulti_InvMass_One_m7->Fill(params.fMinv);
    }
  }

  // two
  Int_t nof_two = EMT_Event_multi_two.size();
  cout << "MixedEvent - nof entries two " << nof_two << endl;
  for (Int_t a = 0; a < nof_two - 1; a++) {
    for (Int_t b = a + 1; b < nof_two; b++) {
      if (EMT_Event_multi_two[a] == EMT_Event_multi_two[b])
        continue;  // to make sure that the photons are from two different events
      if (EMT_multi_two[a] != EMT_multi_two[b]) continue;  // check same multiplicity
      TVector3 e11                  = EMT_pair_momenta_multi_two[a][0];
      TVector3 e12                  = EMT_pair_momenta_multi_two[a][1];
      TVector3 e21                  = EMT_pair_momenta_multi_two[b][0];
      TVector3 e22                  = EMT_pair_momenta_multi_two[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      if (EMT_multi_two[a] == 1) EMTMulti_InvMass_Two_m1->Fill(params.fMinv);
      if (EMT_multi_two[a] == 2) EMTMulti_InvMass_Two_m2->Fill(params.fMinv);
      if (EMT_multi_two[a] == 3) EMTMulti_InvMass_Two_m3->Fill(params.fMinv);
      if (EMT_multi_two[a] == 4) EMTMulti_InvMass_Two_m4->Fill(params.fMinv);
      if (EMT_multi_two[a] == 5) EMTMulti_InvMass_Two_m5->Fill(params.fMinv);
      if (EMT_multi_two[a] == 6) EMTMulti_InvMass_Two_m6->Fill(params.fMinv);
      if (EMT_multi_two[a] == 7) EMTMulti_InvMass_Two_m7->Fill(params.fMinv);
    }
  }

  // onetwo
  Int_t nof_onetwo = EMT_Event_multi_onetwo.size();
  cout << "MixedEvent - nof entries onetwo " << nof_onetwo << endl;
  for (Int_t a = 0; a < nof_onetwo - 1; a++) {
    for (Int_t b = a + 1; b < nof_onetwo; b++) {
      if (EMT_Event_multi_onetwo[a] == EMT_Event_multi_onetwo[b])
        continue;  // to make sure that the photons are from two different events
      if (EMT_multi_onetwo[a] != EMT_multi_onetwo[b]) continue;  // check same multiplicity
      TVector3 e11                  = EMT_pair_momenta_multi_onetwo[a][0];
      TVector3 e12                  = EMT_pair_momenta_multi_onetwo[a][1];
      TVector3 e21                  = EMT_pair_momenta_multi_onetwo[b][0];
      TVector3 e22                  = EMT_pair_momenta_multi_onetwo[b][1];
      CbmLmvmKinematicParams params = CbmKresFunctions::CalculateKinematicParams_4particles(e11, e12, e21, e22);

      if (EMT_multi_onetwo[a] == 1) EMTMulti_InvMass_OneTwo_m1->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 2) EMTMulti_InvMass_OneTwo_m2->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 3) EMTMulti_InvMass_OneTwo_m3->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 4) EMTMulti_InvMass_OneTwo_m4->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 5) EMTMulti_InvMass_OneTwo_m5->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 6) EMTMulti_InvMass_OneTwo_m6->Fill(params.fMinv);
      if (EMT_multi_onetwo[a] == 7) EMTMulti_InvMass_OneTwo_m7->Fill(params.fMinv);
    }
  }
}


void CbmKresConversionKF::Finish()
{
  gDirectory->mkdir("KF");
  gDirectory->cd("KF");


  gDirectory->mkdir("CheckCuts");
  gDirectory->cd("CheckCuts");
  for (UInt_t i = 0; i < fHistoList_CheckForCuts.size(); i++) {
    fHistoList_CheckForCuts[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("multiplicity");
  gDirectory->cd("multiplicity");
  gDirectory->mkdir("All");
  gDirectory->cd("All");
  for (UInt_t i = 0; i < fHistoList_multiplicity_All.size(); i++) {
    fHistoList_multiplicity_All[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Zero");
  gDirectory->cd("Zero");
  for (UInt_t i = 0; i < fHistoList_multiplicity_Zero.size(); i++) {
    fHistoList_multiplicity_Zero[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("One");
  gDirectory->cd("One");
  for (UInt_t i = 0; i < fHistoList_multiplicity_One.size(); i++) {
    fHistoList_multiplicity_One[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("Two");
  gDirectory->cd("Two");
  for (UInt_t i = 0; i < fHistoList_multiplicity_Two.size(); i++) {
    fHistoList_multiplicity_Two[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("OneTwo");
  gDirectory->cd("OneTwo");
  for (UInt_t i = 0; i < fHistoList_multiplicity_OneTwo.size(); i++) {
    fHistoList_multiplicity_OneTwo[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_multiplicity.size(); i++) {
    fHistoList_multiplicity[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("BG");
  gDirectory->cd("BG");
  gDirectory->mkdir("BG_all");
  gDirectory->cd("BG_all");
  for (UInt_t i = 0; i < fHistoList_bg_all.size(); i++) {
    fHistoList_bg_all[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_zero");
  gDirectory->cd("BG_zero");
  for (UInt_t i = 0; i < fHistoList_bg_zero.size(); i++) {
    fHistoList_bg_zero[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_one");
  gDirectory->cd("BG_one");
  for (UInt_t i = 0; i < fHistoList_bg_one.size(); i++) {
    fHistoList_bg_one[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_two");
  gDirectory->cd("BG_two");
  for (UInt_t i = 0; i < fHistoList_bg_two.size(); i++) {
    fHistoList_bg_two[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("BG_onetwo");
  gDirectory->cd("BG_onetwo");
  for (UInt_t i = 0; i < fHistoList_bg_onetwo.size(); i++) {
    fHistoList_bg_onetwo[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->cd("..");


  gDirectory->mkdir("g->All");
  gDirectory->cd("g->All");
  gDirectory->mkdir("target(<4cm)");
  gDirectory->cd("target(<4cm)");
  for (UInt_t i = 0; i < fHistoList_All_target.size(); i++) {
    fHistoList_All_target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("mvd(4cm-21cm)");
  gDirectory->cd("mvd(4cm-21cm)");
  for (UInt_t i = 0; i < fHistoList_All_mvd.size(); i++) {
    fHistoList_All_mvd[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("sts(21cm-75cm)");
  gDirectory->cd("sts(21cm-75cm)");
  for (UInt_t i = 0; i < fHistoList_All_sts.size(); i++) {
    fHistoList_All_sts[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("outside the target(>4cm)");
  gDirectory->cd("outside the target(>4cm)");
  for (UInt_t i = 0; i < fHistoList_All_outside.size(); i++) {
    fHistoList_All_outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_All.size(); i++) {
    fHistoList_All[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("g->Zero");
  gDirectory->cd("g->Zero");
  gDirectory->mkdir("target(<4cm)");
  gDirectory->cd("target(<4cm)");
  for (UInt_t i = 0; i < fHistoList_Zero_target.size(); i++) {
    fHistoList_Zero_target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("mvd(4cm-21cm)");
  gDirectory->cd("mvd(4cm-21cm)");
  for (UInt_t i = 0; i < fHistoList_Zero_mvd.size(); i++) {
    fHistoList_Zero_mvd[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("sts(21cm-75cm)");
  gDirectory->cd("sts(21cm-75cm)");
  for (UInt_t i = 0; i < fHistoList_Zero_sts.size(); i++) {
    fHistoList_Zero_sts[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("outside the target(>4cm)");
  gDirectory->cd("outside the target(>4cm)");
  for (UInt_t i = 0; i < fHistoList_Zero_outside.size(); i++) {
    fHistoList_Zero_outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Zero.size(); i++) {
    fHistoList_Zero[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("g->One");
  gDirectory->cd("g->One");
  gDirectory->mkdir("target(<4cm)");
  gDirectory->cd("target(<4cm)");
  for (UInt_t i = 0; i < fHistoList_One_target.size(); i++) {
    fHistoList_One_target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("mvd(4cm-21cm)");
  gDirectory->cd("mvd(4cm-21cm)");
  for (UInt_t i = 0; i < fHistoList_One_mvd.size(); i++) {
    fHistoList_One_mvd[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("sts(21cm-75cm)");
  gDirectory->cd("sts(21cm-75cm)");
  for (UInt_t i = 0; i < fHistoList_One_sts.size(); i++) {
    fHistoList_One_sts[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("outside the target(>4cm)");
  gDirectory->cd("outside the target(>4cm)");
  for (UInt_t i = 0; i < fHistoList_One_outside.size(); i++) {
    fHistoList_One_outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_One.size(); i++) {
    fHistoList_One[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("g->Two");
  gDirectory->cd("g->Two");
  gDirectory->mkdir("target(<4cm)");
  gDirectory->cd("target(<4cm)");
  for (UInt_t i = 0; i < fHistoList_Two_target.size(); i++) {
    fHistoList_Two_target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("mvd(4cm-21cm)");
  gDirectory->cd("mvd(4cm-21cm)");
  for (UInt_t i = 0; i < fHistoList_Two_mvd.size(); i++) {
    fHistoList_Two_mvd[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("sts(21cm-75cm)");
  gDirectory->cd("sts(21cm-75cm)");
  for (UInt_t i = 0; i < fHistoList_Two_sts.size(); i++) {
    fHistoList_Two_sts[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("outside the target(>4cm)");
  gDirectory->cd("outside the target(>4cm)");
  for (UInt_t i = 0; i < fHistoList_Two_outside.size(); i++) {
    fHistoList_Two_outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_Two.size(); i++) {
    fHistoList_Two[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->mkdir("g->OneTwo");
  gDirectory->cd("g->OneTwo");
  gDirectory->mkdir("target(<4cm)");
  gDirectory->cd("target(<4cm)");
  for (UInt_t i = 0; i < fHistoList_OneTwo_target.size(); i++) {
    fHistoList_OneTwo_target[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("mvd(4cm-21cm)");
  gDirectory->cd("mvd(4cm-21cm)");
  for (UInt_t i = 0; i < fHistoList_OneTwo_mvd.size(); i++) {
    fHistoList_OneTwo_mvd[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("sts(21cm-75cm)");
  gDirectory->cd("sts(21cm-75cm)");
  for (UInt_t i = 0; i < fHistoList_OneTwo_sts.size(); i++) {
    fHistoList_OneTwo_sts[i]->Write();
  }
  gDirectory->cd("..");
  gDirectory->mkdir("outside the target(>4cm)");
  gDirectory->cd("outside the target(>4cm)");
  for (UInt_t i = 0; i < fHistoList_OneTwo_outside.size(); i++) {
    fHistoList_OneTwo_outside[i]->Write();
  }
  gDirectory->cd("..");
  for (UInt_t i = 0; i < fHistoList_OneTwo.size(); i++) {
    fHistoList_OneTwo[i]->Write();
  }
  gDirectory->cd("..");


  gDirectory->cd("..");
}

void CbmKresConversionKF::InitHistograms()
{
  ///////   histograms to check Cuts
  CheckForCuts_InvMass_MC =
    new TH1D("CheckForCuts_InvMass_MC", "CheckForCuts_InvMass_MC; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_MC);
  CheckForCuts_InvMass_Reco =
    new TH1D("CheckForCuts_InvMass_Reco", "CheckForCuts_InvMass_Reco; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_Reco);
  CheckForCuts_OA_MC = new TH1D("CheckForCuts_OA_MC", "CheckForCuts_OA_MC; #theta angle [deg];#", 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_MC);
  CheckForCuts_OA_Reco =
    new TH1D("CheckForCuts_OA_Reco", "CheckForCuts_OA_Reco; #theta angle [deg];#", 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_Reco);
  CheckForCuts_InvMass_MC_from_one_pi0 =
    new TH1D("CheckForCuts_InvMass_MC_from_one_pi0",
             "CheckForCuts_InvMass_MC_from_one_pi0; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_MC_from_one_pi0);
  CheckForCuts_InvMass_Reco_from_one_pi0 =
    new TH1D("CheckForCuts_InvMass_Reco_from_one_pi0",
             "CheckForCuts_InvMass_Reco_from_one_pi0; invariant mass in GeV/c^{2};#", 510, -0.01, 0.5);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_Reco_from_one_pi0);
  CheckForCuts_OA_MC_from_one_pi0 =
    new TH1D("CheckForCuts_OA_MC_from_one_pi0", "CheckForCuts_OA_MC_from_one_pi0; angle [deg];#", 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_MC_from_one_pi0);
  CheckForCuts_OA_Reco_from_one_pi0 =
    new TH1D("CheckForCuts_OA_Reco_from_one_pi0", "CheckForCuts_OA_Reco_from_one_pi0; angle [deg];#", 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_Reco_from_one_pi0);
  CheckForCuts_z_vs_InvMass_MC_from_one_pi0 = new TH2D("CheckForCuts_z_vs_InvMass_MC_from_one_pi0",
                                                       "CheckForCuts_z_vs_InvMass_MC_from_one_pi0; Z [cm]; invariant "
                                                       "mass in GeV/c^{2}; Nof",
                                                       1000, -1, 99, 310, -0.01, 0.3);
  fHistoList_CheckForCuts.push_back(CheckForCuts_z_vs_InvMass_MC_from_one_pi0);
  CheckForCuts_z_vs_InvMass_Reco_from_one_pi0 =
    new TH2D("CheckForCuts_z_vs_InvMass_Reco_from_one_pi0",
             "CheckForCuts_z_vs_InvMass_Reco_from_one_pi0; Z [cm]; invariant "
             "mass in GeV/c^{2}; Nof",
             1000, -1, 99, 310, -0.01, 0.3);
  fHistoList_CheckForCuts.push_back(CheckForCuts_z_vs_InvMass_Reco_from_one_pi0);
  CheckForCuts_z_vs_OA_MC_from_one_pi0 =
    new TH2D("CheckForCuts_z_vs_OA_MC_from_one_pi0", "CheckForCuts_z_vs_OA_MC_from_one_pi0; Z [cm]; #theta in deg; Nof",
             1000, -1, 99, 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_z_vs_OA_MC_from_one_pi0);
  CheckForCuts_z_vs_OA_Reco_from_one_pi0 =
    new TH2D("CheckForCuts_z_vs_OA_Reco_from_one_pi0",
             "CheckForCuts_z_vs_OA_Reco_from_one_pi0; Z [cm]; #theta in deg; Nof", 1000, -1, 99, 300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_z_vs_OA_Reco_from_one_pi0);
  CheckForCuts_InvMass_Reco_from_one_pi0_less4cm =
    new TH1D("CheckForCuts_InvMass_Reco_from_one_pi0_less4cm",
             "CheckForCuts_InvMass_Reco_from_one_pi0_less4cm; invariant mass "
             "in GeV/c^{2};#",
             310, -0.01, 0.3);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_Reco_from_one_pi0_less4cm);
  CheckForCuts_OA_Reco_from_one_pi0_less4cm =
    new TH1D("CheckForCuts_OA_Reco_from_one_pi0_less4cm", "CheckForCuts_OA_Reco_from_one_pi0_less4cm; angle [deg];#",
             300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_Reco_from_one_pi0_less4cm);
  CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm =
    new TH1D("CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm",
             "CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm; invariant mass "
             "in GeV/c^{2};#",
             310, -0.01, 0.3);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_Reco_from_one_pi0_4cm_21cm);
  CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm =
    new TH1D("CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm", "CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm; angle [deg];#",
             300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_Reco_from_one_pi0_4cm_21cm);
  CheckForCuts_InvMass_Reco_from_one_pi0_more21cm =
    new TH1D("CheckForCuts_InvMass_Reco_from_one_pi0_more21cm",
             "CheckForCuts_InvMass_Reco_from_one_pi0_more21cm; invariant mass "
             "in GeV/c^{2};#",
             310, -0.01, 0.3);
  fHistoList_CheckForCuts.push_back(CheckForCuts_InvMass_Reco_from_one_pi0_more21cm);
  CheckForCuts_OA_Reco_from_one_pi0_more21cm =
    new TH1D("CheckForCuts_OA_Reco_from_one_pi0_more21cm", "CheckForCuts_OA_Reco_from_one_pi0_more21cm; angle [deg];#",
             300, -0.1, 29.9);
  fHistoList_CheckForCuts.push_back(CheckForCuts_OA_Reco_from_one_pi0_more21cm);


  // 0-2 => All
  fGammaInvMassReco_All =
    new TH1D("fGammaInvMassReco_All", "fGammaInvMassReco_All; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_All.push_back(fGammaInvMassReco_All);
  fGammaOpeningAngleReco_All =
    new TH1D("fGammaOpeningAngleReco_All", "fGammaOpeningAngleReco_All; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_All.push_back(fGammaOpeningAngleReco_All);
  fPdg_All = new TH1D("fPdg_All", "fPdg_All; Id ;#", 800, 0, 400);
  fHistoList_All.push_back(fPdg_All);
  fP_reco_All = new TH1D("fP_reco_All", "fP_reco_All; P in GeV/c^{2} ;#", 600, 0, 6);
  fHistoList_All.push_back(fP_reco_All);
  fPt_reco_All = new TH1D("fPt_reco_All", "fPt_reco_All; Pt in GeV/c^{2} ;#", 300, 0, 3);
  fHistoList_All.push_back(fPt_reco_All);
  fPi0InvMassRecoKF_All =
    new TH1D("fPi0InvMassRecoKF_All", "fPi0InvMassRecoKF_All; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All.push_back(fPi0InvMassRecoKF_All);
  fEMT_InvMass_All = new TH1D("fEMT_InvMass_All", "fEMT_InvMass_All; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All.push_back(fEMT_InvMass_All);
  fPi0_pt_vs_rap_All =
    new TH2D("fPi0_pt_vs_rap_All", "fPi0_pt_vs_rap_All; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_All.push_back(fPi0_pt_vs_rap_All);
  fPi0_pt_vs_rap_est_All =
    new TH2D("fPi0_pt_vs_rap_est_All", "fPi0_pt_vs_rap_est_All; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
  fHistoList_All.push_back(fPi0_pt_vs_rap_est_All);
  // 0-2 => All  target
  fGammaInvMassReco_All_target = new TH1D(
    "fGammaInvMassReco_All_target", "fGammaInvMassReco_All_target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_All_target.push_back(fGammaInvMassReco_All_target);
  fGammaOpeningAngleReco_All_target =
    new TH1D("fGammaOpeningAngleReco_All_target", "fGammaOpeningAngleReco_All_target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_All_target.push_back(fGammaOpeningAngleReco_All_target);
  fPi0InvMassRecoKF_All_target = new TH1D("fPi0InvMassRecoKF_All_target",
                                          "fPi0InvMassRecoKF_All_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_target.push_back(fPi0InvMassRecoKF_All_target);
  fEMT_InvMass_All_target =
    new TH1D("fEMT_InvMass_All_target", "fEMT_InvMass_All_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_target.push_back(fEMT_InvMass_All_target);
  // 0-2 => All  mvd
  fGammaInvMassReco_All_mvd =
    new TH1D("fGammaInvMassReco_All_mvd", "fGammaInvMassReco_All_mvd; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_All_mvd.push_back(fGammaInvMassReco_All_mvd);
  fGammaOpeningAngleReco_All_mvd =
    new TH1D("fGammaOpeningAngleReco_All_mvd", "fGammaOpeningAngleReco_All_mvd; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_All_mvd.push_back(fGammaOpeningAngleReco_All_mvd);
  fPi0InvMassRecoKF_All_mvd =
    new TH1D("fPi0InvMassRecoKF_All_mvd", "fPi0InvMassRecoKF_All_mvd; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_mvd.push_back(fPi0InvMassRecoKF_All_mvd);
  // 0-2 => All  sts
  fGammaInvMassReco_All_sts =
    new TH1D("fGammaInvMassReco_All_sts", "fGammaInvMassReco_All_sts; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_All_sts.push_back(fGammaInvMassReco_All_sts);
  fGammaOpeningAngleReco_All_sts =
    new TH1D("fGammaOpeningAngleReco_All_sts", "fGammaOpeningAngleReco_All_sts; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_All_sts.push_back(fGammaOpeningAngleReco_All_sts);
  fPi0InvMassRecoKF_All_sts =
    new TH1D("fPi0InvMassRecoKF_All_sts", "fPi0InvMassRecoKF_All_sts; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_sts.push_back(fPi0InvMassRecoKF_All_sts);
  // 0-2 => All  outside the target (mvd+sts+pipe)
  fGammaInvMassReco_All_outside = new TH1D(
    "fGammaInvMassReco_All_outside", "fGammaInvMassReco_All_outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_All_outside.push_back(fGammaInvMassReco_All_outside);
  fGammaOpeningAngleReco_All_outside = new TH1D("fGammaOpeningAngleReco_All_outside",
                                                "fGammaOpeningAngleReco_All_outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_All_outside.push_back(fGammaOpeningAngleReco_All_outside);
  fPi0InvMassRecoKF_All_outside = new TH1D(
    "fPi0InvMassRecoKF_All_outside", "fPi0InvMassRecoKF_All_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_outside.push_back(fPi0InvMassRecoKF_All_outside);
  fEMT_InvMass_All_outside =
    new TH1D("fEMT_InvMass_All_outside", "fEMT_InvMass_All_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_All_outside.push_back(fEMT_InvMass_All_outside);


  // 0 => Zero
  fGammaInvMassReco_Zero =
    new TH1D("fGammaInvMassReco_Zero", "fGammaInvMassReco_Zero; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Zero.push_back(fGammaInvMassReco_Zero);
  fGammaOpeningAngleReco_Zero =
    new TH1D("fGammaOpeningAngleReco_Zero", "fGammaOpeningAngleReco_Zero; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Zero.push_back(fGammaOpeningAngleReco_Zero);
  fPdg_Zero = new TH1D("fPdg_Zero", "fPdg_Zero; Id ;#", 800, 0, 400);
  fHistoList_Zero.push_back(fPdg_Zero);
  fP_reco_Zero = new TH1D("fP_reco_Zero", "fP_reco_Zero; P in GeV/c^{2} ;#", 600, 0, 6);
  fHistoList_Zero.push_back(fP_reco_Zero);
  fPt_reco_Zero = new TH1D("fPt_reco_Zero", "fPt_reco_Zero; Pt in GeV/c^{2} ;#", 300, 0, 3);
  fHistoList_Zero.push_back(fPt_reco_Zero);
  fPi0InvMassRecoKF_Zero =
    new TH1D("fPi0InvMassRecoKF_Zero", "fPi0InvMassRecoKF_Zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero.push_back(fPi0InvMassRecoKF_Zero);
  fEMT_InvMass_Zero = new TH1D("fEMT_InvMass_Zero", "fEMT_InvMass_Zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero.push_back(fEMT_InvMass_Zero);
  fPi0_pt_vs_rap_Zero =
    new TH2D("fPi0_pt_vs_rap_Zero", "fPi0_pt_vs_rap_Zero; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_Zero.push_back(fPi0_pt_vs_rap_Zero);
  fPi0_pt_vs_rap_est_Zero =
    new TH2D("fPi0_pt_vs_rap_est_Zero", "fPi0_pt_vs_rap_est_Zero; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
  fHistoList_Zero.push_back(fPi0_pt_vs_rap_est_Zero);
  // 0 => Zero  target
  fGammaInvMassReco_Zero_target = new TH1D(
    "fGammaInvMassReco_Zero_target", "fGammaInvMassReco_Zero_target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Zero_target.push_back(fGammaInvMassReco_Zero_target);
  fGammaOpeningAngleReco_Zero_target = new TH1D("fGammaOpeningAngleReco_Zero_target",
                                                "fGammaOpeningAngleReco_Zero_target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Zero_target.push_back(fGammaOpeningAngleReco_Zero_target);
  fPi0InvMassRecoKF_Zero_target = new TH1D(
    "fPi0InvMassRecoKF_Zero_target", "fPi0InvMassRecoKF_Zero_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_target.push_back(fPi0InvMassRecoKF_Zero_target);
  fEMT_InvMass_Zero_target =
    new TH1D("fEMT_InvMass_Zero_target", "fEMT_InvMass_Zero_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_target.push_back(fEMT_InvMass_Zero_target);
  // 0 => Zero  mvd
  fGammaInvMassReco_Zero_mvd = new TH1D("fGammaInvMassReco_Zero_mvd",
                                        "fGammaInvMassReco_Zero_mvd; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Zero_mvd.push_back(fGammaInvMassReco_Zero_mvd);
  fGammaOpeningAngleReco_Zero_mvd =
    new TH1D("fGammaOpeningAngleReco_Zero_mvd", "fGammaOpeningAngleReco_Zero_mvd; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Zero_mvd.push_back(fGammaOpeningAngleReco_Zero_mvd);
  fPi0InvMassRecoKF_Zero_mvd =
    new TH1D("fPi0InvMassRecoKF_Zero_mvd", "fPi0InvMassRecoKF_Zero_mvd; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_mvd.push_back(fPi0InvMassRecoKF_Zero_mvd);
  // 0 => Zero  sts
  fGammaInvMassReco_Zero_sts = new TH1D("fGammaInvMassReco_Zero_sts",
                                        "fGammaInvMassReco_Zero_sts; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Zero_sts.push_back(fGammaInvMassReco_Zero_sts);
  fGammaOpeningAngleReco_Zero_sts =
    new TH1D("fGammaOpeningAngleReco_Zero_sts", "fGammaOpeningAngleReco_Zero_sts; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Zero_sts.push_back(fGammaOpeningAngleReco_Zero_sts);
  fPi0InvMassRecoKF_Zero_sts =
    new TH1D("fPi0InvMassRecoKF_Zero_sts", "fPi0InvMassRecoKF_Zero_sts; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_sts.push_back(fPi0InvMassRecoKF_Zero_sts);
  // 0 => Zero  outside the target (mvd+sts+pipe)
  fGammaInvMassReco_Zero_outside = new TH1D(
    "fGammaInvMassReco_Zero_outside", "fGammaInvMassReco_Zero_outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Zero_outside.push_back(fGammaInvMassReco_Zero_outside);
  fGammaOpeningAngleReco_Zero_outside = new TH1D("fGammaOpeningAngleReco_Zero_outside",
                                                 "fGammaOpeningAngleReco_Zero_outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Zero_outside.push_back(fGammaOpeningAngleReco_Zero_outside);
  fPi0InvMassRecoKF_Zero_outside = new TH1D(
    "fPi0InvMassRecoKF_Zero_outside", "fPi0InvMassRecoKF_Zero_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_outside.push_back(fPi0InvMassRecoKF_Zero_outside);
  fEMT_InvMass_Zero_outside =
    new TH1D("fEMT_InvMass_Zero_outside", "fEMT_InvMass_Zero_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Zero_outside.push_back(fEMT_InvMass_Zero_outside);


  // 1 => One
  fGammaInvMassReco_One =
    new TH1D("fGammaInvMassReco_One", "fGammaInvMassReco_One; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_One.push_back(fGammaInvMassReco_One);
  fGammaOpeningAngleReco_One =
    new TH1D("fGammaOpeningAngleReco_One", "fGammaOpeningAngleReco_One; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_One.push_back(fGammaOpeningAngleReco_One);
  fPdg_One = new TH1D("fPdg_One", "fPdg_One; Id ;#", 800, 0, 400);
  fHistoList_One.push_back(fPdg_One);
  fP_reco_One = new TH1D("fP_reco_One", "fP_reco_One; P in GeV/c^{2} ;#", 600, 0, 6);
  fHistoList_One.push_back(fP_reco_One);
  fPt_reco_One = new TH1D("fPt_reco_One", "fPt_reco_One; Pt in GeV/c^{2} ;#", 300, 0, 3);
  fHistoList_One.push_back(fPt_reco_One);
  fPi0InvMassRecoKF_One =
    new TH1D("fPi0InvMassRecoKF_One", "fPi0InvMassRecoKF_One; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One.push_back(fPi0InvMassRecoKF_One);
  fEMT_InvMass_One = new TH1D("fEMT_InvMass_One", "fEMT_InvMass_One; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One.push_back(fEMT_InvMass_One);
  fPi0_pt_vs_rap_One =
    new TH2D("fPi0_pt_vs_rap_One", "fPi0_pt_vs_rap_One; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_One.push_back(fPi0_pt_vs_rap_One);
  fPi0_pt_vs_rap_est_One =
    new TH2D("fPi0_pt_vs_rap_est_One", "fPi0_pt_vs_rap_est_One; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
  fHistoList_One.push_back(fPi0_pt_vs_rap_est_One);
  // 1 => One  target
  fGammaInvMassReco_One_target = new TH1D(
    "fGammaInvMassReco_One_target", "fGammaInvMassReco_One_target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_One_target.push_back(fGammaInvMassReco_One_target);
  fGammaOpeningAngleReco_One_target =
    new TH1D("fGammaOpeningAngleReco_One_target", "fGammaOpeningAngleReco_One_target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_One_target.push_back(fGammaOpeningAngleReco_One_target);
  fPi0InvMassRecoKF_One_target = new TH1D("fPi0InvMassRecoKF_One_target",
                                          "fPi0InvMassRecoKF_One_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_target.push_back(fPi0InvMassRecoKF_One_target);
  fEMT_InvMass_One_target =
    new TH1D("fEMT_InvMass_One_target", "fEMT_InvMass_One_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_target.push_back(fEMT_InvMass_One_target);
  // 1 => One  mvd
  fGammaInvMassReco_One_mvd =
    new TH1D("fGammaInvMassReco_One_mvd", "fGammaInvMassReco_One_mvd; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_One_mvd.push_back(fGammaInvMassReco_One_mvd);
  fGammaOpeningAngleReco_One_mvd =
    new TH1D("fGammaOpeningAngleReco_One_mvd", "fGammaOpeningAngleReco_One_mvd; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_One_mvd.push_back(fGammaOpeningAngleReco_One_mvd);
  fPi0InvMassRecoKF_One_mvd =
    new TH1D("fPi0InvMassRecoKF_One_mvd", "fPi0InvMassRecoKF_One_mvd; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_mvd.push_back(fPi0InvMassRecoKF_One_mvd);
  // 1 => One  sts
  fGammaInvMassReco_One_sts =
    new TH1D("fGammaInvMassReco_One_sts", "fGammaInvMassReco_One_sts; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_One_sts.push_back(fGammaInvMassReco_One_sts);
  fGammaOpeningAngleReco_One_sts =
    new TH1D("fGammaOpeningAngleReco_One_sts", "fGammaOpeningAngleReco_One_sts; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_One_sts.push_back(fGammaOpeningAngleReco_One_sts);
  fPi0InvMassRecoKF_One_sts =
    new TH1D("fPi0InvMassRecoKF_One_sts", "fPi0InvMassRecoKF_One_sts; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_sts.push_back(fPi0InvMassRecoKF_One_sts);
  // 1 => One  outside the target (mvd+sts+pipe)
  fGammaInvMassReco_One_outside = new TH1D(
    "fGammaInvMassReco_One_outside", "fGammaInvMassReco_One_outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_One_outside.push_back(fGammaInvMassReco_One_outside);
  fGammaOpeningAngleReco_One_outside = new TH1D("fGammaOpeningAngleReco_One_outside",
                                                "fGammaOpeningAngleReco_One_outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_One_outside.push_back(fGammaOpeningAngleReco_One_outside);
  fPi0InvMassRecoKF_One_outside = new TH1D(
    "fPi0InvMassRecoKF_One_outside", "fPi0InvMassRecoKF_One_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_outside.push_back(fPi0InvMassRecoKF_One_outside);
  fEMT_InvMass_One_outside =
    new TH1D("fEMT_InvMass_One_outside", "fEMT_InvMass_One_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_One_outside.push_back(fEMT_InvMass_One_outside);


  // 2 => Two
  fGammaInvMassReco_Two =
    new TH1D("fGammaInvMassReco_Two", "fGammaInvMassReco_Two; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Two.push_back(fGammaInvMassReco_Two);
  fGammaOpeningAngleReco_Two =
    new TH1D("fGammaOpeningAngleReco_Two", "fGammaOpeningAngleReco_Two; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Two.push_back(fGammaOpeningAngleReco_Two);
  fPdg_Two = new TH1D("fPdg_Two", "fPdg_Two; Id ;#", 800, 0, 400);
  fHistoList_Two.push_back(fPdg_Two);
  fP_reco_Two = new TH1D("fP_reco_Two", "fP_reco_Two; P in GeV/c^{2} ;#", 600, 0, 6);
  fHistoList_Two.push_back(fP_reco_Two);
  fPt_reco_Two = new TH1D("fPt_reco_Two", "fPt_reco_Two; Pt in GeV/c^{2} ;#", 300, 0, 3);
  fHistoList_Two.push_back(fPt_reco_Two);
  fPi0InvMassRecoKF_Two =
    new TH1D("fPi0InvMassRecoKF_Two", "fPi0InvMassRecoKF_Two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two.push_back(fPi0InvMassRecoKF_Two);
  fEMT_InvMass_Two = new TH1D("fEMT_InvMass_Two", "fEMT_InvMass_Two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two.push_back(fEMT_InvMass_Two);
  fPi0_pt_vs_rap_Two =
    new TH2D("fPi0_pt_vs_rap_Two", "fPi0_pt_vs_rap_Two; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_Two.push_back(fPi0_pt_vs_rap_Two);
  fPi0_pt_vs_rap_est_Two =
    new TH2D("fPi0_pt_vs_rap_est_Two", "fPi0_pt_vs_rap_est_Two; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
  fHistoList_Two.push_back(fPi0_pt_vs_rap_est_Two);
  // 2 => Two  target
  fGammaInvMassReco_Two_target = new TH1D(
    "fGammaInvMassReco_Two_target", "fGammaInvMassReco_Two_target; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Two_target.push_back(fGammaInvMassReco_Two_target);
  fGammaOpeningAngleReco_Two_target =
    new TH1D("fGammaOpeningAngleReco_Two_target", "fGammaOpeningAngleReco_Two_target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Two_target.push_back(fGammaOpeningAngleReco_Two_target);
  fPi0InvMassRecoKF_Two_target = new TH1D("fPi0InvMassRecoKF_Two_target",
                                          "fPi0InvMassRecoKF_Two_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_target.push_back(fPi0InvMassRecoKF_Two_target);
  fEMT_InvMass_Two_target =
    new TH1D("fEMT_InvMass_Two_target", "fEMT_InvMass_Two_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_target.push_back(fEMT_InvMass_Two_target);
  // 2 => Two  mvd
  fGammaInvMassReco_Two_mvd =
    new TH1D("fGammaInvMassReco_Two_mvd", "fGammaInvMassReco_Two_mvd; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Two_mvd.push_back(fGammaInvMassReco_Two_mvd);
  fGammaOpeningAngleReco_Two_mvd =
    new TH1D("fGammaOpeningAngleReco_Two_mvd", "fGammaOpeningAngleReco_Two_mvd; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Two_mvd.push_back(fGammaOpeningAngleReco_Two_mvd);
  fPi0InvMassRecoKF_Two_mvd =
    new TH1D("fPi0InvMassRecoKF_Two_mvd", "fPi0InvMassRecoKF_Two_mvd; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_mvd.push_back(fPi0InvMassRecoKF_Two_mvd);
  // 2 => Two  sts
  fGammaInvMassReco_Two_sts =
    new TH1D("fGammaInvMassReco_Two_sts", "fGammaInvMassReco_Two_sts; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Two_sts.push_back(fGammaInvMassReco_Two_sts);
  fGammaOpeningAngleReco_Two_sts =
    new TH1D("fGammaOpeningAngleReco_Two_sts", "fGammaOpeningAngleReco_Two_sts; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Two_sts.push_back(fGammaOpeningAngleReco_Two_sts);
  fPi0InvMassRecoKF_Two_sts =
    new TH1D("fPi0InvMassRecoKF_Two_sts", "fPi0InvMassRecoKF_Two_sts; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_sts.push_back(fPi0InvMassRecoKF_Two_sts);
  // 2 => Two  outside the target (mvd+sts+pipe)
  fGammaInvMassReco_Two_outside = new TH1D(
    "fGammaInvMassReco_Two_outside", "fGammaInvMassReco_Two_outside; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_Two_outside.push_back(fGammaInvMassReco_Two_outside);
  fGammaOpeningAngleReco_Two_outside = new TH1D("fGammaOpeningAngleReco_Two_outside",
                                                "fGammaOpeningAngleReco_Two_outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_Two_outside.push_back(fGammaOpeningAngleReco_Two_outside);
  fPi0InvMassRecoKF_Two_outside = new TH1D(
    "fPi0InvMassRecoKF_Two_outside", "fPi0InvMassRecoKF_Two_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_outside.push_back(fPi0InvMassRecoKF_Two_outside);
  fEMT_InvMass_Two_outside =
    new TH1D("fEMT_InvMass_Two_outside", "fEMT_InvMass_Two_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_Two_outside.push_back(fEMT_InvMass_Two_outside);


  // 1-2 => OneTwo
  fGammaInvMassReco_OneTwo =
    new TH1D("fGammaInvMassReco_OneTwo", "fGammaInvMassReco_OneTwo; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_OneTwo.push_back(fGammaInvMassReco_OneTwo);
  fGammaOpeningAngleReco_OneTwo =
    new TH1D("fGammaOpeningAngleReco_OneTwo", "fGammaOpeningAngleReco_OneTwo; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_OneTwo.push_back(fGammaOpeningAngleReco_OneTwo);
  fPdg_OneTwo = new TH1D("fPdg_OneTwo", "fPdg_OneTwo; Id ;#", 800, 0, 400);
  fHistoList_OneTwo.push_back(fPdg_OneTwo);
  fP_reco_OneTwo = new TH1D("fP_reco_OneTwo", "fP_reco_OneTwo; P in GeV/c^{2} ;#", 600, 0, 6);
  fHistoList_OneTwo.push_back(fP_reco_OneTwo);
  fPt_reco_OneTwo = new TH1D("fPt_reco_OneTwo", "fPt_reco_OneTwo; Pt in GeV/c^{2} ;#", 300, 0, 3);
  fHistoList_OneTwo.push_back(fPt_reco_OneTwo);
  fPi0InvMassRecoKF_OneTwo =
    new TH1D("fPi0InvMassRecoKF_OneTwo", "fPi0InvMassRecoKF_OneTwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo.push_back(fPi0InvMassRecoKF_OneTwo);
  fEMT_InvMass_OneTwo =
    new TH1D("fEMT_InvMass_OneTwo", "fEMT_InvMass_OneTwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo.push_back(fEMT_InvMass_OneTwo);
  fPi0_pt_vs_rap_OneTwo =
    new TH2D("fPi0_pt_vs_rap_OneTwo", "fPi0_pt_vs_rap_OneTwo; rapidity y; p_{t} in GeV/c ", 90, -2., 7., 60, -1., 5.);
  fHistoList_OneTwo.push_back(fPi0_pt_vs_rap_OneTwo);
  fPi0_pt_vs_rap_est_OneTwo = new TH2D(
    "fPi0_pt_vs_rap_est_OneTwo", "fPi0_pt_vs_rap_est_OneTwo; rapidity y; p_{t} in GeV/c ", 10, 0., 4., 10, 0., 4.);
  fHistoList_OneTwo.push_back(fPi0_pt_vs_rap_est_OneTwo);
  // 1-2 => OneTwo  target
  fGammaInvMassReco_OneTwo_target =
    new TH1D("fGammaInvMassReco_OneTwo_target", "fGammaInvMassReco_OneTwo_target; invariant mass in GeV/c^{2};#", 110,
             -0.01, 0.1);
  fHistoList_OneTwo_target.push_back(fGammaInvMassReco_OneTwo_target);
  fGammaOpeningAngleReco_OneTwo_target = new TH1D(
    "fGammaOpeningAngleReco_OneTwo_target", "fGammaOpeningAngleReco_OneTwo_target; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_OneTwo_target.push_back(fGammaOpeningAngleReco_OneTwo_target);
  fPi0InvMassRecoKF_OneTwo_target = new TH1D(
    "fPi0InvMassRecoKF_OneTwo_target", "fPi0InvMassRecoKF_OneTwo_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo_target.push_back(fPi0InvMassRecoKF_OneTwo_target);
  fEMT_InvMass_OneTwo_target =
    new TH1D("fEMT_InvMass_OneTwo_target", "fEMT_InvMass_OneTwo_target; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo_target.push_back(fEMT_InvMass_OneTwo_target);
  // 1-2 => OneTwo  mvd
  fGammaInvMassReco_OneTwo_mvd = new TH1D(
    "fGammaInvMassReco_OneTwo_mvd", "fGammaInvMassReco_OneTwo_mvd; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_OneTwo_mvd.push_back(fGammaInvMassReco_OneTwo_mvd);
  fGammaOpeningAngleReco_OneTwo_mvd =
    new TH1D("fGammaOpeningAngleReco_OneTwo_mvd", "fGammaOpeningAngleReco_OneTwo_mvd; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_OneTwo_mvd.push_back(fGammaOpeningAngleReco_OneTwo_mvd);
  fPi0InvMassRecoKF_OneTwo_mvd = new TH1D("fPi0InvMassRecoKF_OneTwo_mvd",
                                          "fPi0InvMassRecoKF_OneTwo_mvd; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo_mvd.push_back(fPi0InvMassRecoKF_OneTwo_mvd);
  // 1-2 => OneTwo  sts
  fGammaInvMassReco_OneTwo_sts = new TH1D(
    "fGammaInvMassReco_OneTwo_sts", "fGammaInvMassReco_OneTwo_sts; invariant mass in GeV/c^{2};#", 110, -0.01, 0.1);
  fHistoList_OneTwo_sts.push_back(fGammaInvMassReco_OneTwo_sts);
  fGammaOpeningAngleReco_OneTwo_sts =
    new TH1D("fGammaOpeningAngleReco_OneTwo_sts", "fGammaOpeningAngleReco_OneTwo_sts; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_OneTwo_sts.push_back(fGammaOpeningAngleReco_OneTwo_sts);
  fPi0InvMassRecoKF_OneTwo_sts = new TH1D("fPi0InvMassRecoKF_OneTwo_sts",
                                          "fPi0InvMassRecoKF_OneTwo_sts; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo_sts.push_back(fPi0InvMassRecoKF_OneTwo_sts);
  // 1-2 => OneTwo  outside the target (mvd+sts+pipe)
  fGammaInvMassReco_OneTwo_outside =
    new TH1D("fGammaInvMassReco_OneTwo_outside", "fGammaInvMassReco_OneTwo_outside; invariant mass in GeV/c^{2};#", 110,
             -0.01, 0.1);
  fHistoList_OneTwo_outside.push_back(fGammaInvMassReco_OneTwo_outside);
  fGammaOpeningAngleReco_OneTwo_outside = new TH1D(
    "fGammaOpeningAngleReco_OneTwo_outside", "fGammaOpeningAngleReco_OneTwo_outside; angle [deg];#", 200, -0.1, 19.9);
  fHistoList_OneTwo_outside.push_back(fGammaOpeningAngleReco_OneTwo_outside);
  fPi0InvMassRecoKF_OneTwo_outside =
    new TH1D("fPi0InvMassRecoKF_OneTwo_outside", "fPi0InvMassRecoKF_OneTwo_outside; invariant mass in GeV/c^{2};#",
             1000, 0, 2.0);
  fHistoList_OneTwo_outside.push_back(fPi0InvMassRecoKF_OneTwo_outside);
  fEMT_InvMass_OneTwo_outside =
    new TH1D("fEMT_InvMass_OneTwo_outside", "fEMT_InvMass_OneTwo_outside; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_OneTwo_outside.push_back(fEMT_InvMass_OneTwo_outside);


  // multiplicity plots: How number of gammas change EMT plot and how many gammas we have per event after our cuts
  MultiplicityGamma_All =
    new TH2D("MultiplicityGamma_All", "MultiplicityGamma_All; Nof gammas in event; invariant mass in GeV/c^{2};#", 400,
             0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityGamma_All);
  MultiplicityGamma_Zero = new TH2D("MultiplicityGamma_Zero",
                                    "MultiplicityGamma_Zero; Nof gammas in "
                                    "event; invariant mass in GeV/c^{2};#",
                                    400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityGamma_Zero);
  MultiplicityGamma_One =
    new TH2D("MultiplicityGamma_One", "MultiplicityGamma_One; Nof gammas in event; invariant mass in GeV/c^{2};#", 400,
             0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityGamma_One);
  MultiplicityGamma_Two =
    new TH2D("MultiplicityGamma_Two", "MultiplicityGamma_Two; Nof gammas in event; invariant mass in GeV/c^{2};#", 400,
             0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityGamma_Two);
  MultiplicityGamma_OneTwo = new TH2D("MultiplicityGamma_OneTwo",
                                      "MultiplicityGamma_OneTwo; Nof gammas in "
                                      "event; invariant mass in GeV/c^{2};#",
                                      400, 0, 30, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityGamma_OneTwo);

  MultiplicityChargedParticles_All = new TH2D("MultiplicityChargedParticles_All",
                                              "MultiplicityChargedParticles_All; Nof charged particles in "
                                              "event; invariant mass in GeV/c^{2};#",
                                              1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityChargedParticles_All);
  MultiplicityChargedParticles_Zero = new TH2D("MultiplicityChargedParticles_Zero",
                                               "MultiplicityChargedParticles_Zero; Nof charged particles in "
                                               "event; invariant mass in GeV/c^{2};#",
                                               1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityChargedParticles_Zero);
  MultiplicityChargedParticles_One = new TH2D("MultiplicityChargedParticles_One",
                                              "MultiplicityChargedParticles_One; Nof charged particles in "
                                              "event; invariant mass in GeV/c^{2};#",
                                              1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityChargedParticles_One);
  MultiplicityChargedParticles_Two = new TH2D("MultiplicityChargedParticles_Two",
                                              "MultiplicityChargedParticles_Two; Nof charged particles in "
                                              "event; invariant mass in GeV/c^{2};#",
                                              1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityChargedParticles_Two);
  MultiplicityChargedParticles_OneTwo = new TH2D("MultiplicityChargedParticles_OneTwo",
                                                 "MultiplicityChargedParticles_OneTwo; Nof charged particles in "
                                                 "event; invariant mass in GeV/c^{2};#",
                                                 1000, 0, 1000, 1000, 0, 2.0);
  fHistoList_multiplicity.push_back(MultiplicityChargedParticles_OneTwo);


  // Multi EMT
  EMTMulti_InvMass_All_m1 =
    new TH1D("EMTMulti_InvMass_All_m1", "EMTMulti_InvMass_All_m1; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m1);
  EMTMulti_InvMass_All_m2 =
    new TH1D("EMTMulti_InvMass_All_m2", "EMTMulti_InvMass_All_m2; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m2);
  EMTMulti_InvMass_All_m3 =
    new TH1D("EMTMulti_InvMass_All_m3", "EMTMulti_InvMass_All_m3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m3);
  EMTMulti_InvMass_All_m4 =
    new TH1D("EMTMulti_InvMass_All_m4", "EMTMulti_InvMass_All_m4; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m4);
  EMTMulti_InvMass_All_m5 =
    new TH1D("EMTMulti_InvMass_All_m5", "EMTMulti_InvMass_All_m5; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m5);
  EMTMulti_InvMass_All_m6 =
    new TH1D("EMTMulti_InvMass_All_m6", "EMTMulti_InvMass_All_m6; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m6);
  EMTMulti_InvMass_All_m7 =
    new TH1D("EMTMulti_InvMass_All_m7", "EMTMulti_InvMass_All_m7; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_All.push_back(EMTMulti_InvMass_All_m7);

  EMTMulti_InvMass_Zero_m1 =
    new TH1D("EMTMulti_InvMass_Zero_m1", "EMTMulti_InvMass_Zero_m1; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m1);
  EMTMulti_InvMass_Zero_m2 =
    new TH1D("EMTMulti_InvMass_Zero_m2", "EMTMulti_InvMass_Zero_m2; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m2);
  EMTMulti_InvMass_Zero_m3 =
    new TH1D("EMTMulti_InvMass_Zero_m3", "EMTMulti_InvMass_Zero_m3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m3);
  EMTMulti_InvMass_Zero_m4 =
    new TH1D("EMTMulti_InvMass_Zero_m4", "EMTMulti_InvMass_Zero_m4; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m4);
  EMTMulti_InvMass_Zero_m5 =
    new TH1D("EMTMulti_InvMass_Zero_m5", "EMTMulti_InvMass_Zero_m5; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m5);
  EMTMulti_InvMass_Zero_m6 =
    new TH1D("EMTMulti_InvMass_Zero_m6", "EMTMulti_InvMass_Zero_m6; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m6);
  EMTMulti_InvMass_Zero_m7 =
    new TH1D("EMTMulti_InvMass_Zero_m7", "EMTMulti_InvMass_Zero_m7; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Zero.push_back(EMTMulti_InvMass_Zero_m7);

  EMTMulti_InvMass_One_m1 =
    new TH1D("EMTMulti_InvMass_One_m1", "EMTMulti_InvMass_One_m1; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m1);
  EMTMulti_InvMass_One_m2 =
    new TH1D("EMTMulti_InvMass_One_m2", "EMTMulti_InvMass_One_m2; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m2);
  EMTMulti_InvMass_One_m3 =
    new TH1D("EMTMulti_InvMass_One_m3", "EMTMulti_InvMass_One_m3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m3);
  EMTMulti_InvMass_One_m4 =
    new TH1D("EMTMulti_InvMass_One_m4", "EMTMulti_InvMass_One_m4; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m4);
  EMTMulti_InvMass_One_m5 =
    new TH1D("EMTMulti_InvMass_One_m5", "EMTMulti_InvMass_One_m5; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m5);
  EMTMulti_InvMass_One_m6 =
    new TH1D("EMTMulti_InvMass_One_m6", "EMTMulti_InvMass_One_m6; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m6);
  EMTMulti_InvMass_One_m7 =
    new TH1D("EMTMulti_InvMass_One_m7", "EMTMulti_InvMass_One_m7; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_One.push_back(EMTMulti_InvMass_One_m7);

  EMTMulti_InvMass_Two_m1 =
    new TH1D("EMTMulti_InvMass_Two_m1", "EMTMulti_InvMass_Two_m1; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m1);
  EMTMulti_InvMass_Two_m2 =
    new TH1D("EMTMulti_InvMass_Two_m2", "EMTMulti_InvMass_Two_m2; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m2);
  EMTMulti_InvMass_Two_m3 =
    new TH1D("EMTMulti_InvMass_Two_m3", "EMTMulti_InvMass_Two_m3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m3);
  EMTMulti_InvMass_Two_m4 =
    new TH1D("EMTMulti_InvMass_Two_m4", "EMTMulti_InvMass_Two_m4; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m4);
  EMTMulti_InvMass_Two_m5 =
    new TH1D("EMTMulti_InvMass_Two_m5", "EMTMulti_InvMass_Two_m5; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m5);
  EMTMulti_InvMass_Two_m6 =
    new TH1D("EMTMulti_InvMass_Two_m6", "EMTMulti_InvMass_Two_m6; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m6);
  EMTMulti_InvMass_Two_m7 =
    new TH1D("EMTMulti_InvMass_Two_m7", "EMTMulti_InvMass_Two_m7; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_Two.push_back(EMTMulti_InvMass_Two_m7);

  EMTMulti_InvMass_OneTwo_m1 =
    new TH1D("EMTMulti_InvMass_OneTwo_m1", "EMTMulti_InvMass_OneTwo_m1; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m1);
  EMTMulti_InvMass_OneTwo_m2 =
    new TH1D("EMTMulti_InvMass_OneTwo_m2", "EMTMulti_InvMass_OneTwo_m2; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m2);
  EMTMulti_InvMass_OneTwo_m3 =
    new TH1D("EMTMulti_InvMass_OneTwo_m3", "EMTMulti_InvMass_OneTwo_m3; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m3);
  EMTMulti_InvMass_OneTwo_m4 =
    new TH1D("EMTMulti_InvMass_OneTwo_m4", "EMTMulti_InvMass_OneTwo_m4; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m4);
  EMTMulti_InvMass_OneTwo_m5 =
    new TH1D("EMTMulti_InvMass_OneTwo_m5", "EMTMulti_InvMass_OneTwo_m5; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m5);
  EMTMulti_InvMass_OneTwo_m6 =
    new TH1D("EMTMulti_InvMass_OneTwo_m6", "EMTMulti_InvMass_OneTwo_m6; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m6);
  EMTMulti_InvMass_OneTwo_m7 =
    new TH1D("EMTMulti_InvMass_OneTwo_m7", "EMTMulti_InvMass_OneTwo_m7; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_multiplicity_OneTwo.push_back(EMTMulti_InvMass_OneTwo_m7);


  // BG cases
  //Both all
  BG1_all = new TH1D("BG1_all", "BG1_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG1_all);
  BG2_all = new TH1D("BG2_all", "BG2_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG2_all);
  BG3_all = new TH1D("BG3_all", "BG3_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG3_all);
  BG4_all = new TH1D("BG4_all", "BG4_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG4_all);
  BG5_all = new TH1D("BG5_all", "BG5_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG5_all);
  BG6_all = new TH1D("BG6_all", "BG6_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG6_all);
  BG7_all = new TH1D("BG7_all", "BG7_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG7_all);
  BG8_all = new TH1D("BG8_all", "BG8_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG8_all);
  BG9_all = new TH1D("BG9_all", "BG9_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG9_all);
  BG10_all = new TH1D("BG10_all", "BG10_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(BG10_all);
  PdgCase8_all = new TH1D("PdgCase8_all", "PdgCase8_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(PdgCase8_all);
  PdgCase8mothers_all = new TH1D("PdgCase8mothers_all", "PdgCase8mothers_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(PdgCase8mothers_all);
  sameMIDcase8_all = new TH1D("sameMIDcase8_all", "sameMIDcase8_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(sameMIDcase8_all);
  sameGRIDcase8_all = new TH1D("sameGRIDcase8_all", "sameGRIDcase8_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(sameGRIDcase8_all);
  Case1ZYPos_all = new TH2D("Case1ZYPos_all", "Case1ZYPos_all; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_all.push_back(Case1ZYPos_all);
  sameMIDcase8_mothedPDG_all =
    new TH1D("sameMIDcase8_mothedPDG_all", "sameMIDcase8_mothedPDG_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(sameMIDcase8_mothedPDG_all);
  PdgCase8NonEComeFromTarget_all =
    new TH1D("PdgCase8NonEComeFromTarget_all", "PdgCase8NonEComeFromTarget_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(PdgCase8NonEComeFromTarget_all);
  PdgCase8NonE_NOT_FromTarget_all =
    new TH1D("PdgCase8NonE_NOT_FromTarget_all", "PdgCase8NonE_NOT_FromTarget_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(PdgCase8NonE_NOT_FromTarget_all);
  PdgCase8motherNonE_all = new TH1D("PdgCase8motherNonE_all", "PdgCase8motherNonE_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(PdgCase8motherNonE_all);
  Case8ElFromDalitz_all =
    new TH1D("Case8ElFromDalitz_all", "Case8ElFromDalitz_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(Case8ElFromDalitz_all);
  Case8NonElFrom_pn_all =
    new TH1D("Case8NonElFrom_pn_all", "Case8NonElFrom_pn_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(Case8NonElFrom_pn_all);
  Case8NonElFrom_eta_all =
    new TH1D("Case8NonElFrom_eta_all", "Case8NonElFrom_eta_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(Case8NonElFrom_eta_all);
  Case8NonElFrom_kaon_all =
    new TH1D("Case8NonElFrom_kaon_all", "Case8NonElFrom_kaon_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(Case8NonElFrom_kaon_all);
  sameMIDcase8NonEPdg_all = new TH1D("sameMIDcase8NonEPdg_all", "sameMIDcase8NonEPdg_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(sameMIDcase8NonEPdg_all);
  sameMIDcase8NonEMotherPdg_all =
    new TH1D("sameMIDcase8NonEMotherPdg_all", "sameMIDcase8NonEMotherPdg_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(sameMIDcase8NonEMotherPdg_all);
  sameMIDcase8NonEMotherIM_all = new TH1D("sameMIDcase8NonEMotherIM_all",
                                          "sameMIDcase8NonEMotherIM_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(sameMIDcase8NonEMotherIM_all);
  sameMIDcase8NonEPdgFromTarget_all =
    new TH1D("sameMIDcase8NonEPdgFromTarget_all", "sameMIDcase8NonEPdgFromTarget_all; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_all.push_back(sameMIDcase8NonEPdgFromTarget_all);
  sameMIDcase8NonEComeFromTargetIM_all =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_all",
             "sameMIDcase8NonEComeFromTargetIM_all; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_all.push_back(sameMIDcase8NonEComeFromTargetIM_all);
  sameMIDcase8NonEComeFromTargetP_all = new TH1D("sameMIDcase8NonEComeFromTargetP_all",
                                                 "sameMIDcase8NonEComeFromTargetP_all; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_all.push_back(sameMIDcase8NonEComeFromTargetP_all);
  sameMIDcase8NonEComeFromTargetPt_all = new TH1D(
    "sameMIDcase8NonEComeFromTargetPt_all", "sameMIDcase8NonEComeFromTargetPt_all; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_all.push_back(sameMIDcase8NonEComeFromTargetPt_all);
  //Both zero
  BG1_zero = new TH1D("BG1_zero", "BG1_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG1_zero);
  BG2_zero = new TH1D("BG2_zero", "BG2_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG2_zero);
  BG3_zero = new TH1D("BG3_zero", "BG3_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG3_zero);
  BG4_zero = new TH1D("BG4_zero", "BG4_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG4_zero);
  BG5_zero = new TH1D("BG5_zero", "BG5_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG5_zero);
  BG6_zero = new TH1D("BG6_zero", "BG6_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG6_zero);
  BG7_zero = new TH1D("BG7_zero", "BG7_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG7_zero);
  BG8_zero = new TH1D("BG8_zero", "BG8_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG8_zero);
  BG9_zero = new TH1D("BG9_zero", "BG9_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG9_zero);
  BG10_zero = new TH1D("BG10_zero", "BG10_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(BG10_zero);
  PdgCase8_zero = new TH1D("PdgCase8_zero", "PdgCase8_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(PdgCase8_zero);
  PdgCase8mothers_zero = new TH1D("PdgCase8mothers_zero", "PdgCase8mothers_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(PdgCase8mothers_zero);
  sameMIDcase8_zero = new TH1D("sameMIDcase8_zero", "sameMIDcase8_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(sameMIDcase8_zero);
  sameGRIDcase8_zero =
    new TH1D("sameGRIDcase8_zero", "sameGRIDcase8_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(sameGRIDcase8_zero);
  Case1ZYPos_zero = new TH2D("Case1ZYPos_zero", "Case1ZYPos_zero; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_zero.push_back(Case1ZYPos_zero);
  sameMIDcase8_mothedPDG_zero =
    new TH1D("sameMIDcase8_mothedPDG_zero", "sameMIDcase8_mothedPDG_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(sameMIDcase8_mothedPDG_zero);
  PdgCase8NonEComeFromTarget_zero =
    new TH1D("PdgCase8NonEComeFromTarget_zero", "PdgCase8NonEComeFromTarget_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(PdgCase8NonEComeFromTarget_zero);
  PdgCase8NonE_NOT_FromTarget_zero =
    new TH1D("PdgCase8NonE_NOT_FromTarget_zero", "PdgCase8NonE_NOT_FromTarget_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(PdgCase8NonE_NOT_FromTarget_zero);
  PdgCase8motherNonE_zero = new TH1D("PdgCase8motherNonE_zero", "PdgCase8motherNonE_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(PdgCase8motherNonE_zero);
  Case8ElFromDalitz_zero =
    new TH1D("Case8ElFromDalitz_zero", "Case8ElFromDalitz_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(Case8ElFromDalitz_zero);
  Case8NonElFrom_pn_zero =
    new TH1D("Case8NonElFrom_pn_zero", "Case8NonElFrom_pn_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(Case8NonElFrom_pn_zero);
  Case8NonElFrom_eta_zero =
    new TH1D("Case8NonElFrom_eta_zero", "Case8NonElFrom_eta_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(Case8NonElFrom_eta_zero);
  Case8NonElFrom_kaon_zero =
    new TH1D("Case8NonElFrom_kaon_zero", "Case8NonElFrom_kaon_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(Case8NonElFrom_kaon_zero);
  sameMIDcase8NonEPdg_zero = new TH1D("sameMIDcase8NonEPdg_zero", "sameMIDcase8NonEPdg_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEPdg_zero);
  sameMIDcase8NonEMotherPdg_zero =
    new TH1D("sameMIDcase8NonEMotherPdg_zero", "sameMIDcase8NonEMotherPdg_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEMotherPdg_zero);
  sameMIDcase8NonEMotherIM_zero = new TH1D(
    "sameMIDcase8NonEMotherIM_zero", "sameMIDcase8NonEMotherIM_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEMotherIM_zero);
  sameMIDcase8NonEPdgFromTarget_zero =
    new TH1D("sameMIDcase8NonEPdgFromTarget_zero", "sameMIDcase8NonEPdgFromTarget_zero; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEPdgFromTarget_zero);
  sameMIDcase8NonEComeFromTargetIM_zero =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_zero",
             "sameMIDcase8NonEComeFromTargetIM_zero; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEComeFromTargetIM_zero);
  sameMIDcase8NonEComeFromTargetP_zero = new TH1D(
    "sameMIDcase8NonEComeFromTargetP_zero", "sameMIDcase8NonEComeFromTargetP_zero; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEComeFromTargetP_zero);
  sameMIDcase8NonEComeFromTargetPt_zero = new TH1D(
    "sameMIDcase8NonEComeFromTargetPt_zero", "sameMIDcase8NonEComeFromTargetPt_zero; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_zero.push_back(sameMIDcase8NonEComeFromTargetPt_zero);
  //Both one
  BG1_one = new TH1D("BG1_one", "BG1_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG1_one);
  BG2_one = new TH1D("BG2_one", "BG2_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG2_one);
  BG3_one = new TH1D("BG3_one", "BG3_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG3_one);
  BG4_one = new TH1D("BG4_one", "BG4_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG4_one);
  BG5_one = new TH1D("BG5_one", "BG5_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG5_one);
  BG6_one = new TH1D("BG6_one", "BG6_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG6_one);
  BG7_one = new TH1D("BG7_one", "BG7_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG7_one);
  BG8_one = new TH1D("BG8_one", "BG8_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG8_one);
  BG9_one = new TH1D("BG9_one", "BG9_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG9_one);
  BG10_one = new TH1D("BG10_one", "BG10_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(BG10_one);
  PdgCase8_one = new TH1D("PdgCase8_one", "PdgCase8_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(PdgCase8_one);
  PdgCase8mothers_one = new TH1D("PdgCase8mothers_one", "PdgCase8mothers_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(PdgCase8mothers_one);
  sameMIDcase8_one = new TH1D("sameMIDcase8_one", "sameMIDcase8_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(sameMIDcase8_one);
  sameGRIDcase8_one = new TH1D("sameGRIDcase8_one", "sameGRIDcase8_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(sameGRIDcase8_one);
  Case1ZYPos_one = new TH2D("Case1ZYPos_one", "Case1ZYPos_one; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_one.push_back(Case1ZYPos_one);
  sameMIDcase8_mothedPDG_one =
    new TH1D("sameMIDcase8_mothedPDG_one", "sameMIDcase8_mothedPDG_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(sameMIDcase8_mothedPDG_one);
  PdgCase8NonEComeFromTarget_one =
    new TH1D("PdgCase8NonEComeFromTarget_one", "PdgCase8NonEComeFromTarget_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(PdgCase8NonEComeFromTarget_one);
  PdgCase8NonE_NOT_FromTarget_one =
    new TH1D("PdgCase8NonE_NOT_FromTarget_one", "PdgCase8NonE_NOT_FromTarget_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(PdgCase8NonE_NOT_FromTarget_one);
  PdgCase8motherNonE_one = new TH1D("PdgCase8motherNonE_one", "PdgCase8motherNonE_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(PdgCase8motherNonE_one);
  Case8ElFromDalitz_one =
    new TH1D("Case8ElFromDalitz_one", "Case8ElFromDalitz_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(Case8ElFromDalitz_one);
  Case8NonElFrom_pn_one =
    new TH1D("Case8NonElFrom_pn_one", "Case8NonElFrom_pn_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(Case8NonElFrom_pn_one);
  Case8NonElFrom_eta_one =
    new TH1D("Case8NonElFrom_eta_one", "Case8NonElFrom_eta_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(Case8NonElFrom_eta_one);
  Case8NonElFrom_kaon_one =
    new TH1D("Case8NonElFrom_kaon_one", "Case8NonElFrom_kaon_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(Case8NonElFrom_kaon_one);
  sameMIDcase8NonEPdg_one = new TH1D("sameMIDcase8NonEPdg_one", "sameMIDcase8NonEPdg_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(sameMIDcase8NonEPdg_one);
  sameMIDcase8NonEMotherPdg_one =
    new TH1D("sameMIDcase8NonEMotherPdg_one", "sameMIDcase8NonEMotherPdg_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(sameMIDcase8NonEMotherPdg_one);
  sameMIDcase8NonEMotherIM_one = new TH1D("sameMIDcase8NonEMotherIM_one",
                                          "sameMIDcase8NonEMotherIM_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(sameMIDcase8NonEMotherIM_one);
  sameMIDcase8NonEPdgFromTarget_one =
    new TH1D("sameMIDcase8NonEPdgFromTarget_one", "sameMIDcase8NonEPdgFromTarget_one; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_one.push_back(sameMIDcase8NonEPdgFromTarget_one);
  sameMIDcase8NonEComeFromTargetIM_one =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_one",
             "sameMIDcase8NonEComeFromTargetIM_one; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_one.push_back(sameMIDcase8NonEComeFromTargetIM_one);
  sameMIDcase8NonEComeFromTargetP_one = new TH1D("sameMIDcase8NonEComeFromTargetP_one",
                                                 "sameMIDcase8NonEComeFromTargetP_one; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_one.push_back(sameMIDcase8NonEComeFromTargetP_one);
  sameMIDcase8NonEComeFromTargetPt_one = new TH1D(
    "sameMIDcase8NonEComeFromTargetPt_one", "sameMIDcase8NonEComeFromTargetPt_one; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_one.push_back(sameMIDcase8NonEComeFromTargetPt_one);
  //Both two
  BG1_two = new TH1D("BG1_two", "BG1_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG1_two);
  BG2_two = new TH1D("BG2_two", "BG2_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG2_two);
  BG3_two = new TH1D("BG3_two", "BG3_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG3_two);
  BG4_two = new TH1D("BG4_two", "BG4_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG4_two);
  BG5_two = new TH1D("BG5_two", "BG5_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG5_two);
  BG6_two = new TH1D("BG6_two", "BG6_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG6_two);
  BG7_two = new TH1D("BG7_two", "BG7_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG7_two);
  BG8_two = new TH1D("BG8_two", "BG8_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG8_two);
  BG9_two = new TH1D("BG9_two", "BG9_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG9_two);
  BG10_two = new TH1D("BG10_two", "BG10_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(BG10_two);
  PdgCase8_two = new TH1D("PdgCase8_two", "PdgCase8_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(PdgCase8_two);
  PdgCase8mothers_two = new TH1D("PdgCase8mothers_two", "PdgCase8mothers_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(PdgCase8mothers_two);
  sameMIDcase8_two = new TH1D("sameMIDcase8_two", "sameMIDcase8_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(sameMIDcase8_two);
  sameGRIDcase8_two = new TH1D("sameGRIDcase8_two", "sameGRIDcase8_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(sameGRIDcase8_two);
  Case1ZYPos_two = new TH2D("Case1ZYPos_two", "Case1ZYPos_two; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_two.push_back(Case1ZYPos_two);
  sameMIDcase8_mothedPDG_two =
    new TH1D("sameMIDcase8_mothedPDG_two", "sameMIDcase8_mothedPDG_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(sameMIDcase8_mothedPDG_two);
  PdgCase8NonEComeFromTarget_two =
    new TH1D("PdgCase8NonEComeFromTarget_two", "PdgCase8NonEComeFromTarget_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(PdgCase8NonEComeFromTarget_two);
  PdgCase8NonE_NOT_FromTarget_two =
    new TH1D("PdgCase8NonE_NOT_FromTarget_two", "PdgCase8NonE_NOT_FromTarget_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(PdgCase8NonE_NOT_FromTarget_two);
  PdgCase8motherNonE_two = new TH1D("PdgCase8motherNonE_two", "PdgCase8motherNonE_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(PdgCase8motherNonE_two);
  Case8ElFromDalitz_two =
    new TH1D("Case8ElFromDalitz_two", "Case8ElFromDalitz_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(Case8ElFromDalitz_two);
  Case8NonElFrom_pn_two =
    new TH1D("Case8NonElFrom_pn_two", "Case8NonElFrom_pn_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(Case8NonElFrom_pn_two);
  Case8NonElFrom_eta_two =
    new TH1D("Case8NonElFrom_eta_two", "Case8NonElFrom_eta_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(Case8NonElFrom_eta_two);
  Case8NonElFrom_kaon_two =
    new TH1D("Case8NonElFrom_kaon_two", "Case8NonElFrom_kaon_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(Case8NonElFrom_kaon_two);
  sameMIDcase8NonEPdg_two = new TH1D("sameMIDcase8NonEPdg_two", "sameMIDcase8NonEPdg_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(sameMIDcase8NonEPdg_two);
  sameMIDcase8NonEMotherPdg_two =
    new TH1D("sameMIDcase8NonEMotherPdg_two", "sameMIDcase8NonEMotherPdg_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(sameMIDcase8NonEMotherPdg_two);
  sameMIDcase8NonEMotherIM_two = new TH1D("sameMIDcase8NonEMotherIM_two",
                                          "sameMIDcase8NonEMotherIM_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(sameMIDcase8NonEMotherIM_two);
  sameMIDcase8NonEPdgFromTarget_two =
    new TH1D("sameMIDcase8NonEPdgFromTarget_two", "sameMIDcase8NonEPdgFromTarget_two; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_two.push_back(sameMIDcase8NonEPdgFromTarget_two);
  sameMIDcase8NonEComeFromTargetIM_two =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_two",
             "sameMIDcase8NonEComeFromTargetIM_two; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_two.push_back(sameMIDcase8NonEComeFromTargetIM_two);
  sameMIDcase8NonEComeFromTargetP_two = new TH1D("sameMIDcase8NonEComeFromTargetP_two",
                                                 "sameMIDcase8NonEComeFromTargetP_two; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_two.push_back(sameMIDcase8NonEComeFromTargetP_two);
  sameMIDcase8NonEComeFromTargetPt_two = new TH1D(
    "sameMIDcase8NonEComeFromTargetPt_two", "sameMIDcase8NonEComeFromTargetPt_two; Pt in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_two.push_back(sameMIDcase8NonEComeFromTargetPt_two);
  //Both onetwo
  BG1_onetwo = new TH1D("BG1_onetwo", "BG1_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG1_onetwo);
  BG2_onetwo = new TH1D("BG2_onetwo", "BG2_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG2_onetwo);
  BG3_onetwo = new TH1D("BG3_onetwo", "BG3_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG3_onetwo);
  BG4_onetwo = new TH1D("BG4_onetwo", "BG4_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG4_onetwo);
  BG5_onetwo = new TH1D("BG5_onetwo", "BG5_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG5_onetwo);
  BG6_onetwo = new TH1D("BG6_onetwo", "BG6_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG6_onetwo);
  BG7_onetwo = new TH1D("BG7_onetwo", "BG7_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG7_onetwo);
  BG8_onetwo = new TH1D("BG8_onetwo", "BG8_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG8_onetwo);
  BG9_onetwo = new TH1D("BG9_onetwo", "BG9_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG9_onetwo);
  BG10_onetwo = new TH1D("BG10_onetwo", "BG10_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(BG10_onetwo);
  PdgCase8_onetwo = new TH1D("PdgCase8_onetwo", "PdgCase8_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(PdgCase8_onetwo);
  PdgCase8mothers_onetwo = new TH1D("PdgCase8mothers_onetwo", "PdgCase8mothers_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(PdgCase8mothers_onetwo);
  sameMIDcase8_onetwo =
    new TH1D("sameMIDcase8_onetwo", "sameMIDcase8_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(sameMIDcase8_onetwo);
  sameGRIDcase8_onetwo =
    new TH1D("sameGRIDcase8_onetwo", "sameGRIDcase8_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(sameGRIDcase8_onetwo);
  Case1ZYPos_onetwo = new TH2D("Case1ZYPos_onetwo", "Case1ZYPos_onetwo; z[cm]; y[cm]", 400, -1, 200, 200, -50, 50);
  fHistoList_bg_onetwo.push_back(Case1ZYPos_onetwo);
  sameMIDcase8_mothedPDG_onetwo =
    new TH1D("sameMIDcase8_mothedPDG_onetwo", "sameMIDcase8_mothedPDG_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(sameMIDcase8_mothedPDG_onetwo);
  PdgCase8NonEComeFromTarget_onetwo =
    new TH1D("PdgCase8NonEComeFromTarget_onetwo", "PdgCase8NonEComeFromTarget_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(PdgCase8NonEComeFromTarget_onetwo);
  PdgCase8NonE_NOT_FromTarget_onetwo =
    new TH1D("PdgCase8NonE_NOT_FromTarget_onetwo", "PdgCase8NonE_NOT_FromTarget_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(PdgCase8NonE_NOT_FromTarget_onetwo);
  PdgCase8motherNonE_onetwo =
    new TH1D("PdgCase8motherNonE_onetwo", "PdgCase8motherNonE_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(PdgCase8motherNonE_onetwo);
  Case8ElFromDalitz_onetwo =
    new TH1D("Case8ElFromDalitz_onetwo", "Case8ElFromDalitz_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(Case8ElFromDalitz_onetwo);
  Case8NonElFrom_pn_onetwo =
    new TH1D("Case8NonElFrom_pn_onetwo", "Case8NonElFrom_pn_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(Case8NonElFrom_pn_onetwo);
  Case8NonElFrom_eta_onetwo =
    new TH1D("Case8NonElFrom_eta_onetwo", "Case8NonElFrom_eta_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(Case8NonElFrom_eta_onetwo);
  Case8NonElFrom_kaon_onetwo =
    new TH1D("Case8NonElFrom_kaon_onetwo", "Case8NonElFrom_kaon_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(Case8NonElFrom_kaon_onetwo);
  sameMIDcase8NonEPdg_onetwo =
    new TH1D("sameMIDcase8NonEPdg_onetwo", "sameMIDcase8NonEPdg_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEPdg_onetwo);
  sameMIDcase8NonEMotherPdg_onetwo =
    new TH1D("sameMIDcase8NonEMotherPdg_onetwo", "sameMIDcase8NonEMotherPdg_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEMotherPdg_onetwo);
  sameMIDcase8NonEMotherIM_onetwo = new TH1D(
    "sameMIDcase8NonEMotherIM_onetwo", "sameMIDcase8NonEMotherIM_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEMotherIM_onetwo);
  sameMIDcase8NonEPdgFromTarget_onetwo =
    new TH1D("sameMIDcase8NonEPdgFromTarget_onetwo", "sameMIDcase8NonEPdgFromTarget_onetwo; Id ;#", 5000, -2500, 2500);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEPdgFromTarget_onetwo);
  sameMIDcase8NonEComeFromTargetIM_onetwo =
    new TH1D("sameMIDcase8NonEComeFromTargetIM_onetwo",
             "sameMIDcase8NonEComeFromTargetIM_onetwo; invariant mass in GeV/c^{2};#", 1000, 0, 2.0);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEComeFromTargetIM_onetwo);
  sameMIDcase8NonEComeFromTargetP_onetwo = new TH1D(
    "sameMIDcase8NonEComeFromTargetP_onetwo", "sameMIDcase8NonEComeFromTargetP_onetwo; P in GeV/c^{2} ;#", 200, 0, 10);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEComeFromTargetP_onetwo);
  sameMIDcase8NonEComeFromTargetPt_onetwo =
    new TH1D("sameMIDcase8NonEComeFromTargetPt_onetwo", "sameMIDcase8NonEComeFromTargetPt_onetwo; Pt in GeV/c^{2} ;#",
             200, 0, 10);
  fHistoList_bg_onetwo.push_back(sameMIDcase8NonEComeFromTargetPt_onetwo);
}
