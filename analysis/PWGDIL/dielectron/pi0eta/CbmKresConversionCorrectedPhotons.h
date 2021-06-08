/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBM_KRES_CONVERSION_CORRECTED_PHOTONS
#define CBM_KRES_CONVERSION_CORRECTED_PHOTONS

#include "CbmKFVertex.h"
#include "CbmKresGammaCorrection.h"
#include "CbmKresSelectAnnPhotons.h"
#include "CbmKresTrainAnnDirectPhotons.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmMCTrack.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmStsTrack.h"
#include "CbmVertex.h"

#include "TH2D.h"
#include <TClonesArray.h>

#include "KFParticle.h"

class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresConversionCorrectedPhotons {

public:
  //***** brief Standard constructor.
  CbmKresConversionCorrectedPhotons();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionCorrectedPhotons();


  void Init(double OA, double IM);
  void InitHistograms();
  void Finish();

  void Exec(int fEventNumDP, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd,
                         int stsMcTrackId, CbmRichRing* RING);

  void SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd,
                        int richInd, int stsMcTrackId, CbmRichRing* RING);

  int FindInRich(int richInd, int stsMcTrackId);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  std::vector<TVector3> SaveAllHits(CbmStsTrack* track);

  void FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                        vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus,
                        vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                        vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus, std::vector<int> Rings_minus,
                        std::vector<int> Rings_plus, std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus,
                        vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus);

  void FindGammasOutside(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                         vector<CbmMCTrack*> MCtracks_minus_Outside, vector<CbmMCTrack*> MCtracks_plus_Outside,
                         vector<CbmStsTrack*> StsTrack_minus_Outside, vector<CbmStsTrack*> StsTrack_plus_Outside,
                         std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
                         std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside,
                         vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside);


  double CalculatePlaneAngle_last_fromHits(std::vector<TVector3> track_1, std::vector<TVector3> track_2);

  double CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  double CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  void CDP_likesign_Mixing_Target(double AngleCut, double InvMassCut);

  void CDP_likesign_Mixing_Outside(double AngleCut, double InvMassCut);

  void CDP_Mixing_Target(double AngleCut, double InvMassCut);

  void CDP_Mixing_Outside(double AngleCut, double InvMassCut);


private:
  CbmKresTrainAnnDirectPhotons* fTrainPhotons;
  Int_t AnnTrainPhotons;
  CbmKresSelectAnnPhotons* fAnnPhotonsSelection;
  Int_t UseAnnPhotons;
  CbmKresGammaCorrection* fGammaCorrection;
  Int_t UseCorrection;
  std::vector<std::vector<double>> corr_all;
  std::vector<std::vector<double>> corr_two;
  std::vector<std::vector<double>> corr_onetwo;
  double thresholdweight;


  TClonesArray* fMcTracks;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fRichProjections;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRichHits;
  TClonesArray* fArrayMvdHit;
  TClonesArray* fArrayStsHit;

  CbmVertex* fPrimVertex;
  CbmKFVertex fKFVertex;

  CbmRichRingFitterEllipseTau* fTauFit;

  // charged tracks from outside
  vector<CbmStsTrack*> VStsTrack_minus_Outside;
  vector<CbmMCTrack*> VMCtracks_minus_Outside;
  std::vector<int> VRings_minus_Outside;
  std::vector<int> VStsIndex_minus_Outside;
  vector<CbmRichRing*> VRichRing_minus_Outside;

  vector<CbmStsTrack*> VStsTrack_plus_Outside;
  vector<CbmMCTrack*> VMCtracks_plus_Outside;
  std::vector<int> VRings_plus_Outside;
  std::vector<int> VStsIndex_plus_Outside;
  vector<CbmRichRing*> VRichRing_plus_Outside;


  // charged tracks from the target
  vector<CbmMCTrack*> VMCtracks_minus_Target;
  vector<CbmStsTrack*> VStsTrack_minus_Target;
  vector<TVector3> VMomenta_minus_Target;
  std::vector<int> VRings_minus_Target;
  std::vector<int> VStsIndex_minus_Target;
  vector<CbmRichRing*> VRichRing_minus_Target;

  vector<CbmMCTrack*> VMCtracks_plus_Target;
  vector<CbmStsTrack*> VStsTrack_plus_Target;
  vector<TVector3> VMomenta_plus_Target;
  std::vector<int> VRings_plus_Target;
  std::vector<int> VStsIndex_plus_Target;
  vector<CbmRichRing*> VRichRing_plus_Target;


  // CDP_LK_EMT Target
  std::vector<TVector3> CDP_LK_EMT_momenta_minus_Target;
  std::vector<int> CDP_LK_EMT_NofRings_minus_Target;
  std::vector<CbmStsTrack*> CDP_LK_EMT_STS_minus_Target;
  std::vector<int> CDP_LK_EMT_STS_minus_index_Target;

  std::vector<TVector3> CDP_LK_EMT_momenta_plus_Target;
  std::vector<int> CDP_LK_EMT_NofRings_plus_Target;
  std::vector<CbmStsTrack*> CDP_LK_EMT_STS_plus_Target;
  std::vector<int> CDP_LK_EMT_STS_plus_index_Target;

  // CDP_LK_EMT Outside
  std::vector<int> CDP_LK_EMT_NofRings_minus_Outside;
  std::vector<CbmStsTrack*> CDP_LK_EMT_STS_minus_Outside;
  std::vector<int> CDP_LK_EMT_STS_minus_index_Outside;

  std::vector<int> CDP_LK_EMT_NofRings_plus_Outside;
  std::vector<CbmStsTrack*> CDP_LK_EMT_STS_plus_Outside;
  std::vector<int> CDP_LK_EMT_STS_plus_index_Outside;


  // CDP_EMT Target
  std::vector<int> CDP_EMT_Event_minus_Target;
  std::vector<TVector3> CDP_EMT_momenta_minus_Target;
  std::vector<int> CDP_EMT_NofRings_minus_Target;
  std::vector<std::vector<TVector3>> CDP_EMT_Hits_minus_Target;

  std::vector<int> CDP_EMT_Event_plus_Target;
  std::vector<TVector3> CDP_EMT_momenta_plus_Target;
  std::vector<int> CDP_EMT_NofRings_plus_Target;
  std::vector<std::vector<TVector3>> CDP_EMT_Hits_plus_Target;

  // CDP_EMT Outside
  std::vector<int> CDP_EMT_Event_minus_Outside;
  std::vector<CbmStsTrack*> CDP_EMT_momenta_minus_Outside;
  std::vector<KFParticle> CDP_EMT_KFTrack_minus_Outside;
  std::vector<int> CDP_EMT_NofRings_minus_Outside;
  std::vector<std::vector<TVector3>> CDP_EMT_Hits_minus_Outside;

  std::vector<int> CDP_EMT_Event_plus_Outside;
  std::vector<CbmStsTrack*> CDP_EMT_momenta_plus_Outside;
  std::vector<KFParticle> CDP_EMT_KFTrack_plus_Outside;
  std::vector<int> CDP_EMT_NofRings_plus_Outside;
  std::vector<std::vector<TVector3>> CDP_EMT_Hits_plus_Outside;


  // histograms
  // Target
  vector<TH1*> fHistoList_dp_Target;
  TH1D* CMother_PDG_Target;
  TH1D* CGrandMother_PDG_Target;

  // Outside
  vector<TH1*> fHistoList_dp_Outside;
  TH1D* CMother_PDG_Outside;
  TH1D* CGrandMother_PDG_Outside;

  // Both
  vector<TH1*> fHistoList_dp_Both;
  TH2D* CPdg_vs_Distance_for_dp;
  TH2D* CP_vs_Distance_for_dp;
  TH1D* CDP_AnnTruePairs;
  TH1D* CDP_AnnFalsePairs;
  TH1D* CDP_AnnTruePairs_AfterCuts;
  TH1D* CDP_AnnFalsePairs_AfterCuts;


  ///////   to check Cuts
  ///////   Both
  vector<TH1*> fHistoList_dp_cuts_Both;
  TH2D* CDP_candidates_InvMass_vs_OA_Both;
  TH2D* CDP_InvMass_vs_OA_Both;
  TH1D* CDP_candidates_InvMass_Both;
  TH1D* CDP_InvMass_Both;
  TH1D* CDP_candidates_OA_Both;
  TH1D* CDP_OA_Both;
  TH1D* CDP_candidates_PlaneAngles_last_Both;
  TH1D* CDP_PlaneAngles_last_Both;
  TH1D* CDP_candidates_PlaneAngles_first_Both;
  TH1D* CDP_PlaneAngles_first_Both;

  ///////   Target
  vector<TH1*> fHistoList_dp_cuts_Target;
  TH2D* CDP_candidates_InvMass_vs_OA_Target;
  TH2D* CDP_InvMass_vs_OA_Target;
  TH1D* CDP_candidates_InvMass_Target;
  TH1D* CDP_InvMass_Target;
  TH1D* CDP_candidates_OA_Target;
  TH1D* CDP_OA_Target;
  TH1D* CDP_candidates_PlaneAngles_last_Target;
  TH1D* CDP_PlaneAngles_last_Target;
  TH1D* CDP_candidates_PlaneAngles_first_Target;
  TH1D* CDP_PlaneAngles_first_Target;

  ///////   Outside
  vector<TH1*> fHistoList_dp_cuts_Outside;
  TH2D* CDP_candidates_InvMass_vs_OA_Outside;
  TH2D* CDP_InvMass_vs_OA_Outside;
  TH1D* CDP_candidates_InvMass_Outside;
  TH1D* CDP_InvMass_Outside;
  TH1D* CDP_candidates_OA_Outside;
  TH1D* CDP_OA_Outside;
  TH1D* CDP_candidates_PlaneAngles_last_Outside;
  TH1D* CDP_PlaneAngles_last_Outside;
  TH1D* CDP_candidates_PlaneAngles_first_Outside;
  TH1D* CDP_PlaneAngles_first_Outside;


  // Target => all
  vector<TH1*> fHistoList_dp_all_Target;
  TH1D* CDP_InvMassReco_all_Target;
  TH1D* CDP_OpeningAngleReco_all_Target;
  TH1D* CDP_Pdg_all_Target;
  TH1D* CDP_P_reco_all_Target;
  TH1D* CDP_Pt_reco_all_Target;
  TH1D* CPh_fromTarget_Pt_reco_all_Target;
  TH1D* CPh_fromPions_Pt_reco_all_Target;
  TH1D* CPh_fromEtas_Pt_reco_all_Target;
  TH1D* CPh_fromDalitz_Pt_reco_all_Target;
  TH1D* CPh_fromXi_Pt_reco_all_Target;
  TH1D* CPh_fromOther_Pt_reco_all_Target;
  TH1D* CPh_twoFromTarget_Pt_reco_all_Target;
  TH1D* CPh_fromCombinatorial_Pt_reco_all_Target;
  TH1D* CPh_fromConversion_Pt_reco_all_Target;
  TH2D* CPh_pt_vs_rap_est_all_Target;
  TH2D* CPh_pt_vs_rap_est_corr_all_Target;

  // Target => two
  vector<TH1*> fHistoList_dp_two_Target;
  TH1D* CDP_InvMassReco_two_Target;
  TH1D* CDP_OpeningAngleReco_two_Target;
  TH1D* CDP_Pdg_two_Target;
  TH1D* CDP_P_reco_two_Target;
  TH1D* CDP_Pt_reco_two_Target;
  TH1D* CPh_fromTarget_Pt_reco_two_Target;
  TH1D* CPh_fromPions_Pt_reco_two_Target;
  TH1D* CPh_fromEtas_Pt_reco_two_Target;
  TH1D* CPh_fromDalitz_Pt_reco_two_Target;
  TH1D* CPh_fromXi_Pt_reco_two_Target;
  TH1D* CPh_fromOther_Pt_reco_two_Target;
  TH1D* CPh_twoFromTarget_Pt_reco_two_Target;
  TH1D* CPh_fromCombinatorial_Pt_reco_two_Target;
  TH1D* CPh_fromConversion_Pt_reco_two_Target;
  TH2D* CPh_pt_vs_rap_est_two_Target;
  TH2D* CPh_pt_vs_rap_est_corr_two_Target;

  // Target => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Target;
  TH1D* CDP_InvMassReco_onetwo_Target;
  TH1D* CDP_OpeningAngleReco_onetwo_Target;
  TH1D* CDP_Pdg_onetwo_Target;
  TH1D* CDP_P_reco_onetwo_Target;
  TH1D* CDP_Pt_reco_onetwo_Target;
  TH1D* CPh_fromTarget_Pt_reco_onetwo_Target;
  TH1D* CPh_fromPions_Pt_reco_onetwo_Target;
  TH1D* CPh_fromEtas_Pt_reco_onetwo_Target;
  TH1D* CPh_fromDalitz_Pt_reco_onetwo_Target;
  TH1D* CPh_fromXi_Pt_reco_onetwo_Target;
  TH1D* CPh_fromOther_Pt_reco_onetwo_Target;
  TH1D* CPh_twoFromTarget_Pt_reco_onetwo_Target;
  TH1D* CPh_fromCombinatorial_Pt_reco_onetwo_Target;
  TH1D* CPh_fromConversion_Pt_reco_onetwo_Target;
  TH2D* CPh_pt_vs_rap_est_onetwo_Target;
  TH2D* CPh_pt_vs_rap_est_corr_onetwo_Target;


  // Outside => all
  vector<TH1*> fHistoList_dp_all_Outside;
  TH1D* CDP_InvMassReco_all_Outside;
  TH1D* CDP_OpeningAngleReco_all_Outside;
  TH1D* CDP_Pdg_all_Outside;
  TH1D* CDP_P_reco_all_Outside;
  TH1D* CDP_Pt_reco_all_Outside;
  TH1D* CPh_fromTarget_Pt_reco_all_Outside;
  TH1D* CPh_fromPions_Pt_reco_all_Outside;
  TH1D* CPh_fromEtas_Pt_reco_all_Outside;
  TH1D* CPh_fromDalitz_Pt_reco_all_Outside;
  TH1D* CPh_fromXi_Pt_reco_all_Outside;
  TH1D* CPh_fromOther_Pt_reco_all_Outside;
  TH1D* CPh_twoFromTarget_Pt_reco_all_Outside;
  TH1D* CPh_fromCombinatorial_Pt_reco_all_Outside;
  TH1D* CPh_fromConversion_Pt_reco_all_Outside;
  TH2D* CPh_pt_vs_rap_est_all_Outside;
  TH2D* CPh_pt_vs_rap_est_corr_all_Outside;

  // Outside => two
  vector<TH1*> fHistoList_dp_two_Outside;
  TH1D* CDP_InvMassReco_two_Outside;
  TH1D* CDP_OpeningAngleReco_two_Outside;
  TH1D* CDP_Pdg_two_Outside;
  TH1D* CDP_P_reco_two_Outside;
  TH1D* CDP_Pt_reco_two_Outside;
  TH1D* CPh_fromTarget_Pt_reco_two_Outside;
  TH1D* CPh_fromPions_Pt_reco_two_Outside;
  TH1D* CPh_fromEtas_Pt_reco_two_Outside;
  TH1D* CPh_fromDalitz_Pt_reco_two_Outside;
  TH1D* CPh_fromXi_Pt_reco_two_Outside;
  TH1D* CPh_fromOther_Pt_reco_two_Outside;
  TH1D* CPh_twoFromTarget_Pt_reco_two_Outside;
  TH1D* CPh_fromCombinatorial_Pt_reco_two_Outside;
  TH1D* CPh_fromConversion_Pt_reco_two_Outside;
  TH2D* CPh_pt_vs_rap_est_two_Outside;
  TH2D* CPh_pt_vs_rap_est_corr_two_Outside;

  // Outside => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Outside;
  TH1D* CDP_InvMassReco_onetwo_Outside;
  TH1D* CDP_OpeningAngleReco_onetwo_Outside;
  TH1D* CDP_Pdg_onetwo_Outside;
  TH1D* CDP_P_reco_onetwo_Outside;
  TH1D* CDP_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromTarget_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromPions_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromEtas_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromDalitz_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromXi_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromOther_Pt_reco_onetwo_Outside;
  TH1D* CPh_twoFromTarget_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromCombinatorial_Pt_reco_onetwo_Outside;
  TH1D* CPh_fromConversion_Pt_reco_onetwo_Outside;
  TH2D* CPh_pt_vs_rap_est_onetwo_Outside;
  TH2D* CPh_pt_vs_rap_est_corr_onetwo_Outside;


  // Both => all
  vector<TH1*> fHistoList_dp_all_Both;
  TH1D* CDP_InvMassReco_all_Both;
  TH1D* CDP_OpeningAngleReco_all_Both;
  TH1D* CDP_Pdg_all_Both;
  TH1D* CDP_P_reco_all_Both;
  TH1D* CDP_Pt_reco_all_Both;
  TH1D* CPh_fromTarget_Pt_reco_all_Both;
  TH1D* CPh_fromPions_Pt_reco_all_Both;
  TH1D* CPh_fromEtas_Pt_reco_all_Both;
  TH1D* CPh_fromDalitz_Pt_reco_all_Both;
  TH1D* CPh_fromXi_Pt_reco_all_Both;
  TH1D* CPh_fromOther_Pt_reco_all_Both;
  TH1D* CPh_twoFromTarget_Pt_reco_all_Both;
  TH1D* CPh_fromCombinatorial_Pt_reco_all_Both;
  TH1D* CPh_fromConversion_Pt_reco_all_Both;
  TH2D* CPh_pt_vs_rap_est_all_Both;
  TH2D* CPh_pt_vs_rap_est_corr_all_Both;

  // Both => two
  vector<TH1*> fHistoList_dp_two_Both;
  TH1D* CDP_InvMassReco_two_Both;
  TH1D* CDP_OpeningAngleReco_two_Both;
  TH1D* CDP_Pdg_two_Both;
  TH1D* CDP_P_reco_two_Both;
  TH1D* CDP_Pt_reco_two_Both;
  TH1D* CPh_fromTarget_Pt_reco_two_Both;
  TH1D* CPh_fromPions_Pt_reco_two_Both;
  TH1D* CPh_fromEtas_Pt_reco_two_Both;
  TH1D* CPh_fromDalitz_Pt_reco_two_Both;
  TH1D* CPh_fromXi_Pt_reco_two_Both;
  TH1D* CPh_fromOther_Pt_reco_two_Both;
  TH1D* CPh_twoFromTarget_Pt_reco_two_Both;
  TH1D* CPh_fromCombinatorial_Pt_reco_two_Both;
  TH1D* CPh_fromConversion_Pt_reco_two_Both;
  TH2D* CPh_pt_vs_rap_est_two_Both;
  TH2D* CPh_pt_vs_rap_est_corr_two_Both;

  // Both => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Both;
  TH1D* CDP_InvMassReco_onetwo_Both;
  TH1D* CDP_OpeningAngleReco_onetwo_Both;
  TH1D* CDP_Pdg_onetwo_Both;
  TH1D* CDP_P_reco_onetwo_Both;
  TH1D* CDP_Pt_reco_onetwo_Both;
  TH1D* CPh_fromTarget_Pt_reco_onetwo_Both;
  TH1D* CPh_fromPions_Pt_reco_onetwo_Both;
  TH1D* CPh_fromEtas_Pt_reco_onetwo_Both;
  TH1D* CPh_fromDalitz_Pt_reco_onetwo_Both;
  TH1D* CPh_fromXi_Pt_reco_onetwo_Both;
  TH1D* CPh_fromOther_Pt_reco_onetwo_Both;
  TH1D* CPh_twoFromTarget_Pt_reco_onetwo_Both;
  TH1D* CPh_fromCombinatorial_Pt_reco_onetwo_Both;
  TH1D* CPh_fromConversion_Pt_reco_onetwo_Both;
  TH2D* CPh_pt_vs_rap_est_onetwo_Both;
  TH2D* CPh_pt_vs_rap_est_corr_onetwo_Both;


  // EMT CDP Target
  TH1D* CDP_EMT_Pt_all_Target;
  TH1D* CDP_EMT_Pt_two_Target;
  TH1D* CDP_EMT_Pt_onetwo_Target;

  // EMT CDP Outside
  TH1D* CDP_EMT_Pt_all_Outside;
  TH1D* CDP_EMT_Pt_two_Outside;
  TH1D* CDP_EMT_Pt_onetwo_Outside;

  // EMT CDP Both
  TH1D* CDP_EMT_Pt_all_Both;
  TH1D* CDP_EMT_Pt_two_Both;
  TH1D* CDP_EMT_Pt_onetwo_Both;


  /////////////////// CDP_LK_EMT Target
  TH1D* CDP_LK_EMT_Pt_all_Target;
  TH1D* CDP_LK_EMT_Pt_two_Target;
  TH1D* CDP_LK_EMT_Pt_onetwo_Target;
  /////////////////////////////////////

  /////////////////// CDP_LK_EMT Outside
  TH1D* CDP_LK_EMT_Pt_all_Outside;
  TH1D* CDP_LK_EMT_Pt_two_Outside;
  TH1D* CDP_LK_EMT_Pt_onetwo_Outside;
  /////////////////////////////////////

  /////////////////// CDP_LK_EMT Both
  TH1D* CDP_LK_EMT_Pt_all_Both;
  TH1D* CDP_LK_EMT_Pt_two_Both;
  TH1D* CDP_LK_EMT_Pt_onetwo_Both;
  /////////////////////////////////////


  //***** brief Copy constructor.
  CbmKresConversionCorrectedPhotons(const CbmKresConversionCorrectedPhotons&);

  //***** brief Assignment operator.
  CbmKresConversionCorrectedPhotons operator=(const CbmKresConversionCorrectedPhotons&);


  ClassDef(CbmKresConversionCorrectedPhotons, 1)
};

#endif
