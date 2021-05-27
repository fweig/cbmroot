/** @file CbmKresConversionManual.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef CBM_KRES_CONVERSION_MANUAL
#define CBM_KRES_CONVERSION_MANUAL

#include "CbmKFVertex.h"
#include "CbmKresConversionBG.h"
#include "CbmKresSelectAnn.h"
#include "CbmKresTrainAnn.h"
#include "CbmMCTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmVertex.h"

#include "TH2D.h"
#include <TClonesArray.h>

class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresConversionManual {

public:
  //***** brief Standard constructor.
  CbmKresConversionManual();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionManual();


  void Init();
  void InitHistograms();
  void Finish();

  void Exec(int fEventNumMan, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd,
                         int stsMcTrackId, CbmRichRing* RING);

  void SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd,
                        int richInd, int stsMcTrackId, CbmRichRing* RING);

  int FindInRich(int richInd, int stsMcTrackId);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  void FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                        vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus,
                        vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                        vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus, std::vector<int> Rings_minus,
                        std::vector<int> Rings_plus, std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus,
                        vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus,
                        vector<Int_t> MCIndex_minus, vector<Int_t> MCIndex_plus);

  void FindGammasOutside(int EventNumMan, double AngleCut, double InvMassCut, int RealPID,
                         vector<CbmMCTrack*> MCtracks_minus_Outside, vector<CbmMCTrack*> MCtracks_plus_Outside,
                         vector<CbmStsTrack*> StsTrack_minus_Outside, vector<CbmStsTrack*> StsTrack_plus_Outside,
                         std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
                         std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside,
                         vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside,
                         vector<Int_t> MCIndex_minus_Outside, vector<Int_t> MCIndex_plus_Outside);

  void FindGammasBoth();

  void FindPi0(TString mod, TString position, vector<vector<TVector3>> Gammas, vector<vector<int>> StsIndex,
               vector<vector<int>> MCIndex, vector<vector<CbmMCTrack*>> GammasMC, TH1D* Pi0InvMassReco,
               TH2D* Pi0_pt_vs_rap, TH2D* Pi0_pt_vs_rap_est, TH2D* MultiplicityGamma,
               TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases, TH1D* DalitzPi0, TH1D* PhotonsPi0);

  void Mixing_Target();

  void Mixing_Outside();

  void Mixing_Both();

  double CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  double CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  void FindGammasBothWithAdditionalCuts();

  void Mixing_WAC();


private:
  CbmKresTrainAnn* fTrain;
  Int_t AnnTrain;
  CbmKresSelectAnn* fAnnSelection;
  Int_t UseAnn;


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

  CbmKresConversionBG* fAnaBG;

  // charged tracks from outside
  vector<CbmStsTrack*> VStsTrack_minus_Outside;
  vector<CbmMCTrack*> VMCtracks_minus_Outside;
  std::vector<int> VRings_minus_Outside;
  std::vector<int> VStsIndex_minus_Outside;
  vector<CbmRichRing*> VRichRing_minus_Outside;
  vector<Int_t> VMCIndex_minus_Outside;

  vector<CbmStsTrack*> VStsTrack_plus_Outside;
  vector<CbmMCTrack*> VMCtracks_plus_Outside;
  std::vector<int> VRings_plus_Outside;
  std::vector<int> VStsIndex_plus_Outside;
  vector<CbmRichRing*> VRichRing_plus_Outside;
  vector<Int_t> VMCIndex_plus_Outside;


  // charged tracks from the target
  vector<CbmMCTrack*> VMCtracks_minus_Target;
  vector<CbmStsTrack*> VStsTrack_minus_Target;
  vector<TVector3> VMomenta_minus_Target;
  std::vector<int> VRings_minus_Target;
  std::vector<int> VStsIndex_minus_Target;
  vector<CbmRichRing*> VRichRing_minus_Target;
  vector<Int_t> VMCIndex_minus_Target;

  vector<CbmMCTrack*> VMCtracks_plus_Target;
  vector<CbmStsTrack*> VStsTrack_plus_Target;
  vector<TVector3> VMomenta_plus_Target;
  std::vector<int> VRings_plus_Target;
  std::vector<int> VStsIndex_plus_Target;
  vector<CbmRichRing*> VRichRing_plus_Target;
  vector<Int_t> VMCIndex_plus_Target;


  vector<TVector3> frefmomenta;
  std::vector<int> frefId;
  std::vector<int> fMCId;
  std::vector<CbmMCTrack*> fMCtracks;


  // EMT target Target
  std::vector<int> EMT_man_Event_Target;
  std::vector<std::vector<TVector3>> EMT_man_pair_momenta_Target;
  std::vector<int> EMT_man_NofRings_Target;

  // EMT target Outside
  std::vector<int> EMT_man_Event_Outside;
  std::vector<std::vector<TVector3>> EMT_man_pair_momenta_Outside;
  std::vector<int> EMT_man_NofRings_Outside;

  // EMT target Both
  std::vector<int> EMT_man_Event_Both;
  std::vector<std::vector<TVector3>> EMT_man_pair_momenta_Both;
  std::vector<int> EMT_man_NofRings_Both;

  // combined gammas from the target
  std::vector<std::vector<TVector3>> Gammas_all_Target;
  std::vector<std::vector<TVector3>> Gammas_zero_Target;
  std::vector<std::vector<TVector3>> Gammas_one_Target;
  std::vector<std::vector<TVector3>> Gammas_two_Target;
  std::vector<std::vector<TVector3>> Gammas_onetwo_Target;

  std::vector<std::vector<int>> Gammas_stsIndex_all_Target;
  std::vector<std::vector<int>> Gammas_stsIndex_zero_Target;
  std::vector<std::vector<int>> Gammas_stsIndex_one_Target;
  std::vector<std::vector<int>> Gammas_stsIndex_two_Target;
  std::vector<std::vector<int>> Gammas_stsIndex_onetwo_Target;

  std::vector<std::vector<int>> Gammas_MCIndex_all_Target;
  std::vector<std::vector<int>> Gammas_MCIndex_zero_Target;
  std::vector<std::vector<int>> Gammas_MCIndex_one_Target;
  std::vector<std::vector<int>> Gammas_MCIndex_two_Target;
  std::vector<std::vector<int>> Gammas_MCIndex_onetwo_Target;

  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_all_Target;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_zero_Target;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_one_Target;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_two_Target;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_onetwo_Target;


  // combined gammas from outside
  std::vector<std::vector<TVector3>> Gammas_all_Outside;
  std::vector<std::vector<TVector3>> Gammas_zero_Outside;
  std::vector<std::vector<TVector3>> Gammas_one_Outside;
  std::vector<std::vector<TVector3>> Gammas_two_Outside;
  std::vector<std::vector<TVector3>> Gammas_onetwo_Outside;

  std::vector<std::vector<int>> Gammas_stsIndex_all_Outside;
  std::vector<std::vector<int>> Gammas_stsIndex_zero_Outside;
  std::vector<std::vector<int>> Gammas_stsIndex_one_Outside;
  std::vector<std::vector<int>> Gammas_stsIndex_two_Outside;
  std::vector<std::vector<int>> Gammas_stsIndex_onetwo_Outside;

  std::vector<std::vector<int>> Gammas_MCIndex_all_Outside;
  std::vector<std::vector<int>> Gammas_MCIndex_zero_Outside;
  std::vector<std::vector<int>> Gammas_MCIndex_one_Outside;
  std::vector<std::vector<int>> Gammas_MCIndex_two_Outside;
  std::vector<std::vector<int>> Gammas_MCIndex_onetwo_Outside;

  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_all_Outside;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_zero_Outside;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_one_Outside;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_two_Outside;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_onetwo_Outside;


  // combined gammas from the target and outside together
  std::vector<std::vector<TVector3>> Gammas_all_Both;
  std::vector<std::vector<TVector3>> Gammas_zero_Both;
  std::vector<std::vector<TVector3>> Gammas_one_Both;
  std::vector<std::vector<TVector3>> Gammas_two_Both;
  std::vector<std::vector<TVector3>> Gammas_onetwo_Both;

  std::vector<std::vector<int>> Gammas_stsIndex_all_Both;
  std::vector<std::vector<int>> Gammas_stsIndex_zero_Both;
  std::vector<std::vector<int>> Gammas_stsIndex_one_Both;
  std::vector<std::vector<int>> Gammas_stsIndex_two_Both;
  std::vector<std::vector<int>> Gammas_stsIndex_onetwo_Both;

  std::vector<std::vector<int>> Gammas_MCIndex_all_Both;
  std::vector<std::vector<int>> Gammas_MCIndex_zero_Both;
  std::vector<std::vector<int>> Gammas_MCIndex_one_Both;
  std::vector<std::vector<int>> Gammas_MCIndex_two_Both;
  std::vector<std::vector<int>> Gammas_MCIndex_onetwo_Both;

  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_all_Both;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_zero_Both;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_one_Both;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_two_Both;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_onetwo_Both;


  // WAC
  std::vector<std::vector<TVector3>> Gamma_WAC;
  std::vector<std::vector<int>> Gammas_stsIndex_WAC;
  std::vector<std::vector<int>> Gammas_MCIndex_WAC;
  std::vector<std::vector<CbmMCTrack*>> Gammas_MC_WAC;
  // WAC event mixing
  std::vector<int> EMT_man_Event_WAC;
  std::vector<std::vector<TVector3>> EMT_man_pair_momenta_WAC;
  std::vector<int> EMT_man_NofRings_WAC;


  // histograms
  ///////   histograms to check Cuts => Both
  vector<TH1*> fHistoList_man_cuts_Both;
  TH2D* InvMass_vs_OA_candidates_Both;
  TH2D* InvMass_vs_OA_fromPi0_Both;
  TH1D* GammasInvMass_candidates_Both;
  TH1D* GammasOA_candidates_Both;
  TH1D* GammasInvMass_fromPi0_Both;
  TH1D* GammasOA_fromPi0_Both;
  TH1D* PlaneAngles_last_candidates_Both;
  TH1D* PlaneAngles_last_fromPi0_Both;
  TH1D* PlaneAngles_first_candidates_Both;
  TH1D* PlaneAngles_first_fromPi0_Both;


  ///////   histograms to check Cuts => Target
  vector<TH1*> fHistoList_man_cuts_Target;
  TH2D* InvMass_vs_OA_candidates_Target;
  TH2D* InvMass_vs_OA_fromPi0_Target;
  TH1D* GammasInvMass_candidates_Target;
  TH1D* GammasOA_candidates_Target;
  TH1D* GammasInvMass_fromPi0_Target;
  TH1D* GammasOA_fromPi0_Target;
  TH1D* PlaneAngles_last_candidates_Target;
  TH1D* PlaneAngles_last_fromPi0_Target;
  TH1D* PlaneAngles_first_candidates_Target;
  TH1D* PlaneAngles_first_fromPi0_Target;


  ///////   histograms to check Cuts => Outside
  vector<TH1*> fHistoList_man_cuts_Outside;
  TH2D* InvMass_vs_OA_candidates_Outside;
  TH2D* InvMass_vs_OA_fromPi0_Outside;
  TH1D* GammasInvMass_candidates_Outside;
  TH1D* GammasOA_candidates_Outside;
  TH1D* GammasInvMass_fromPi0_Outside;
  TH1D* GammasOA_fromPi0_Outside;
  TH1D* PlaneAngles_last_candidates_Outside;
  TH1D* PlaneAngles_last_fromPi0_Outside;
  TH1D* PlaneAngles_first_candidates_Outside;
  TH1D* PlaneAngles_first_fromPi0_Outside;


  ///////   Target
  vector<TH1*> fHistoList_man_all_Target;
  TH1D* GammaInvMassReco_all_Target;
  TH1D* GammaOpeningAngleReco_all_Target;
  TH1D* Pdg_all_Target;
  TH1D* P_reco_all_Target;
  TH1D* Pt_reco_all_Target;
  TH1D* Pi0InvMassReco_all_Target;
  TH1D* EMT_InvMass_all_Target;
  TH2D* Pi0_pt_vs_rap_all_Target;
  TH2D* Pi0_pt_vs_rap_est_all_Target;
  TH1D* DalitzPi0_all_Target;
  TH1D* PhotonsPi0_all_Target;

  vector<TH1*> fHistoList_man_zero_Target;
  TH1D* GammaInvMassReco_zero_Target;
  TH1D* GammaOpeningAngleReco_zero_Target;
  TH1D* Pdg_zero_Target;
  TH1D* P_reco_zero_Target;
  TH1D* Pt_reco_zero_Target;
  TH1D* Pi0InvMassReco_zero_Target;
  TH1D* EMT_InvMass_zero_Target;
  TH2D* Pi0_pt_vs_rap_zero_Target;
  TH2D* Pi0_pt_vs_rap_est_zero_Target;
  TH1D* DalitzPi0_zero_Target;
  TH1D* PhotonsPi0_zero_Target;

  vector<TH1*> fHistoList_man_one_Target;
  TH1D* GammaInvMassReco_one_Target;
  TH1D* GammaOpeningAngleReco_one_Target;
  TH1D* Pdg_one_Target;
  TH1D* P_reco_one_Target;
  TH1D* Pt_reco_one_Target;
  TH1D* Pi0InvMassReco_one_Target;
  TH1D* EMT_InvMass_one_Target;
  TH2D* Pi0_pt_vs_rap_one_Target;
  TH2D* Pi0_pt_vs_rap_est_one_Target;
  TH1D* DalitzPi0_one_Target;
  TH1D* PhotonsPi0_one_Target;

  vector<TH1*> fHistoList_man_two_Target;
  TH1D* GammaInvMassReco_two_Target;
  TH1D* GammaOpeningAngleReco_two_Target;
  TH1D* Pdg_two_Target;
  TH1D* P_reco_two_Target;
  TH1D* Pt_reco_two_Target;
  TH1D* Pi0InvMassReco_two_Target;
  TH1D* EMT_InvMass_two_Target;
  TH2D* Pi0_pt_vs_rap_two_Target;
  TH2D* Pi0_pt_vs_rap_est_two_Target;
  TH1D* DalitzPi0_two_Target;
  TH1D* PhotonsPi0_two_Target;

  vector<TH1*> fHistoList_man_onetwo_Target;
  TH1D* GammaInvMassReco_onetwo_Target;
  TH1D* GammaOpeningAngleReco_onetwo_Target;
  TH1D* Pdg_onetwo_Target;
  TH1D* P_reco_onetwo_Target;
  TH1D* Pt_reco_onetwo_Target;
  TH1D* Pi0InvMassReco_onetwo_Target;
  TH1D* EMT_InvMass_onetwo_Target;
  TH2D* Pi0_pt_vs_rap_onetwo_Target;
  TH2D* Pi0_pt_vs_rap_est_onetwo_Target;
  TH1D* DalitzPi0_onetwo_Target;
  TH1D* PhotonsPi0_onetwo_Target;


  ///////   Outside
  vector<TH1*> fHistoList_man_all_Outside;
  TH1D* GammaInvMassReco_all_Outside;
  TH1D* GammaOpeningAngleReco_all_Outside;
  TH1D* Pdg_all_Outside;
  TH1D* P_reco_all_Outside;
  TH1D* Pt_reco_all_Outside;
  TH1D* Pi0InvMassReco_all_Outside;
  TH1D* EMT_InvMass_all_Outside;
  TH2D* Pi0_pt_vs_rap_all_Outside;
  TH2D* Pi0_pt_vs_rap_est_all_Outside;
  TH1D* DalitzPi0_all_Outside;
  TH1D* PhotonsPi0_all_Outside;

  vector<TH1*> fHistoList_man_zero_Outside;
  TH1D* GammaInvMassReco_zero_Outside;
  TH1D* GammaOpeningAngleReco_zero_Outside;
  TH1D* Pdg_zero_Outside;
  TH1D* P_reco_zero_Outside;
  TH1D* Pt_reco_zero_Outside;
  TH1D* Pi0InvMassReco_zero_Outside;
  TH1D* EMT_InvMass_zero_Outside;
  TH2D* Pi0_pt_vs_rap_zero_Outside;
  TH2D* Pi0_pt_vs_rap_est_zero_Outside;
  TH1D* DalitzPi0_zero_Outside;
  TH1D* PhotonsPi0_zero_Outside;

  vector<TH1*> fHistoList_man_one_Outside;
  TH1D* GammaInvMassReco_one_Outside;
  TH1D* GammaOpeningAngleReco_one_Outside;
  TH1D* Pdg_one_Outside;
  TH1D* P_reco_one_Outside;
  TH1D* Pt_reco_one_Outside;
  TH1D* Pi0InvMassReco_one_Outside;
  TH1D* EMT_InvMass_one_Outside;
  TH2D* Pi0_pt_vs_rap_one_Outside;
  TH2D* Pi0_pt_vs_rap_est_one_Outside;
  TH1D* DalitzPi0_one_Outside;
  TH1D* PhotonsPi0_one_Outside;

  vector<TH1*> fHistoList_man_two_Outside;
  TH1D* GammaInvMassReco_two_Outside;
  TH1D* GammaOpeningAngleReco_two_Outside;
  TH1D* Pdg_two_Outside;
  TH1D* P_reco_two_Outside;
  TH1D* Pt_reco_two_Outside;
  TH1D* Pi0InvMassReco_two_Outside;
  TH1D* EMT_InvMass_two_Outside;
  TH2D* Pi0_pt_vs_rap_two_Outside;
  TH2D* Pi0_pt_vs_rap_est_two_Outside;
  TH1D* DalitzPi0_two_Outside;
  TH1D* PhotonsPi0_two_Outside;

  vector<TH1*> fHistoList_man_onetwo_Outside;
  TH1D* GammaInvMassReco_onetwo_Outside;
  TH1D* GammaOpeningAngleReco_onetwo_Outside;
  TH1D* Pdg_onetwo_Outside;
  TH1D* P_reco_onetwo_Outside;
  TH1D* Pt_reco_onetwo_Outside;
  TH1D* Pi0InvMassReco_onetwo_Outside;
  TH1D* EMT_InvMass_onetwo_Outside;
  TH2D* Pi0_pt_vs_rap_onetwo_Outside;
  TH2D* Pi0_pt_vs_rap_est_onetwo_Outside;
  TH1D* DalitzPi0_onetwo_Outside;
  TH1D* PhotonsPi0_onetwo_Outside;


  ///////   Both
  vector<TH1*> fHistoList_man_all_Both;
  TH1D* GammaInvMassReco_all_Both;
  TH1D* GammaOpeningAngleReco_all_Both;
  TH1D* Pdg_all_Both;
  TH1D* P_reco_all_Both;
  TH1D* Pt_reco_all_Both;
  TH1D* Pi0InvMassReco_all_Both;
  TH1D* EMT_InvMass_all_Both;
  TH2D* Pi0_pt_vs_rap_all_Both;
  TH2D* Pi0_pt_vs_rap_est_all_Both;
  TH1D* DalitzPi0_all_Both;
  TH1D* PhotonsPi0_all_Both;

  vector<TH1*> fHistoList_man_zero_Both;
  TH1D* GammaInvMassReco_zero_Both;
  TH1D* GammaOpeningAngleReco_zero_Both;
  TH1D* Pdg_zero_Both;
  TH1D* P_reco_zero_Both;
  TH1D* Pt_reco_zero_Both;
  TH1D* Pi0InvMassReco_zero_Both;
  TH1D* EMT_InvMass_zero_Both;
  TH2D* Pi0_pt_vs_rap_zero_Both;
  TH2D* Pi0_pt_vs_rap_est_zero_Both;
  TH1D* DalitzPi0_zero_Both;
  TH1D* PhotonsPi0_zero_Both;

  vector<TH1*> fHistoList_man_one_Both;
  TH1D* GammaInvMassReco_one_Both;
  TH1D* GammaOpeningAngleReco_one_Both;
  TH1D* Pdg_one_Both;
  TH1D* P_reco_one_Both;
  TH1D* Pt_reco_one_Both;
  TH1D* Pi0InvMassReco_one_Both;
  TH1D* EMT_InvMass_one_Both;
  TH2D* Pi0_pt_vs_rap_one_Both;
  TH2D* Pi0_pt_vs_rap_est_one_Both;
  TH1D* DalitzPi0_one_Both;
  TH1D* PhotonsPi0_one_Both;

  vector<TH1*> fHistoList_man_two_Both;
  TH1D* GammaInvMassReco_two_Both;
  TH1D* GammaOpeningAngleReco_two_Both;
  TH1D* Pdg_two_Both;
  TH1D* P_reco_two_Both;
  TH1D* Pt_reco_two_Both;
  TH1D* Pi0InvMassReco_two_Both;
  TH1D* EMT_InvMass_two_Both;
  TH2D* Pi0_pt_vs_rap_two_Both;
  TH2D* Pi0_pt_vs_rap_est_two_Both;
  TH1D* DalitzPi0_two_Both;
  TH1D* PhotonsPi0_two_Both;

  vector<TH1*> fHistoList_man_onetwo_Both;
  TH1D* GammaInvMassReco_onetwo_Both;
  TH1D* GammaOpeningAngleReco_onetwo_Both;
  TH1D* Pdg_onetwo_Both;
  TH1D* P_reco_onetwo_Both;
  TH1D* Pt_reco_onetwo_Both;
  TH1D* Pi0InvMassReco_onetwo_Both;
  TH1D* EMT_InvMass_onetwo_Both;
  TH2D* Pi0_pt_vs_rap_onetwo_Both;
  TH2D* Pi0_pt_vs_rap_est_onetwo_Both;
  TH1D* DalitzPi0_onetwo_Both;
  TH1D* PhotonsPi0_onetwo_Both;


  // Both additional histograms
  vector<TH1*> fHistoList_man_Both;
  TH2D* Pdg_vs_Distance;
  TH2D* P_vs_Distance;


  // multiplicity Target
  vector<TH1*> fHistoList_multiplicity_man_Target;
  TH2D* MultiplicityGamma_all_Target;
  TH2D* MultiplicityGamma_zero_Target;
  TH2D* MultiplicityGamma_one_Target;
  TH2D* MultiplicityGamma_two_Target;
  TH2D* MultiplicityGamma_onetwo_Target;
  TH2D* MultiplicityChargedParticles_all_Target;
  TH2D* MultiplicityChargedParticles_zero_Target;
  TH2D* MultiplicityChargedParticles_one_Target;
  TH2D* MultiplicityChargedParticles_two_Target;
  TH2D* MultiplicityChargedParticles_onetwo_Target;

  // multiplicity Outside
  vector<TH1*> fHistoList_multiplicity_man_Outside;
  TH2D* MultiplicityGamma_all_Outside;
  TH2D* MultiplicityGamma_zero_Outside;
  TH2D* MultiplicityGamma_one_Outside;
  TH2D* MultiplicityGamma_two_Outside;
  TH2D* MultiplicityGamma_onetwo_Outside;
  TH2D* MultiplicityChargedParticles_all_Outside;
  TH2D* MultiplicityChargedParticles_zero_Outside;
  TH2D* MultiplicityChargedParticles_one_Outside;
  TH2D* MultiplicityChargedParticles_two_Outside;
  TH2D* MultiplicityChargedParticles_onetwo_Outside;

  // multiplicity Both
  vector<TH1*> fHistoList_multiplicity_man_Both;
  TH2D* MultiplicityGamma_all_Both;
  TH2D* MultiplicityGamma_zero_Both;
  TH2D* MultiplicityGamma_one_Both;
  TH2D* MultiplicityGamma_two_Both;
  TH2D* MultiplicityGamma_onetwo_Both;
  TH2D* MultiplicityChargedParticles_all_Both;
  TH2D* MultiplicityChargedParticles_zero_Both;
  TH2D* MultiplicityChargedParticles_one_Both;
  TH2D* MultiplicityChargedParticles_two_Both;
  TH2D* MultiplicityChargedParticles_onetwo_Both;


  // //   rap_vs_Pt for "OneTwo" and "Both"
  vector<TH1*> fHistoList_rap_vs_pt_InM;
  TH1D* rap_vs_Pt_InM_1;
  TH1D* rap_vs_Pt_InM_2;
  TH1D* rap_vs_Pt_InM_3;
  TH1D* rap_vs_Pt_InM_4;
  TH1D* rap_vs_Pt_InM_5;
  TH1D* rap_vs_Pt_InM_6;
  TH1D* rap_vs_Pt_InM_7;
  TH1D* rap_vs_Pt_InM_8;
  TH1D* rap_vs_Pt_InM_9;
  TH1D* rap_vs_Pt_InM_10;
  TH1D* rap_vs_Pt_InM_11;
  TH1D* rap_vs_Pt_InM_12;
  TH1D* rap_vs_Pt_InM_13;
  TH1D* rap_vs_Pt_InM_14;
  TH1D* rap_vs_Pt_InM_15;
  TH1D* rap_vs_Pt_InM_16;
  TH1D* rap_vs_Pt_InM_17;
  TH1D* rap_vs_Pt_InM_18;
  TH1D* rap_vs_Pt_InM_19;
  TH1D* rap_vs_Pt_InM_20;
  TH1D* rap_vs_Pt_InM_21;
  TH1D* rap_vs_Pt_InM_22;
  TH1D* rap_vs_Pt_InM_23;
  TH1D* rap_vs_Pt_InM_24;
  TH1D* rap_vs_Pt_InM_25;
  TH1D* rap_vs_Pt_InM_26;
  TH1D* rap_vs_Pt_InM_27;
  TH1D* rap_vs_Pt_InM_28;
  TH1D* rap_vs_Pt_InM_29;
  TH1D* rap_vs_Pt_InM_30;
  TH1D* rap_vs_Pt_InM_31;
  TH1D* rap_vs_Pt_InM_32;
  TH1D* rap_vs_Pt_InM_33;
  TH1D* rap_vs_Pt_InM_34;
  TH1D* rap_vs_Pt_InM_35;
  TH1D* rap_vs_Pt_InM_36;
  TH1D* rap_vs_Pt_InM_37;
  TH1D* rap_vs_Pt_InM_38;
  TH1D* rap_vs_Pt_InM_39;
  TH1D* rap_vs_Pt_InM_40;
  TH1D* rap_vs_Pt_InM_41;
  TH1D* rap_vs_Pt_InM_42;
  TH1D* rap_vs_Pt_InM_43;
  TH1D* rap_vs_Pt_InM_44;
  TH1D* rap_vs_Pt_InM_45;
  TH1D* rap_vs_Pt_InM_46;
  TH1D* rap_vs_Pt_InM_47;
  TH1D* rap_vs_Pt_InM_48;
  TH1D* rap_vs_Pt_InM_49;
  TH1D* rap_vs_Pt_InM_50;
  TH1D* rap_vs_Pt_InM_51;
  TH1D* rap_vs_Pt_InM_52;
  TH1D* rap_vs_Pt_InM_53;
  TH1D* rap_vs_Pt_InM_54;
  TH1D* rap_vs_Pt_InM_55;
  TH1D* rap_vs_Pt_InM_56;
  TH1D* rap_vs_Pt_InM_57;
  TH1D* rap_vs_Pt_InM_58;
  TH1D* rap_vs_Pt_InM_59;
  TH1D* rap_vs_Pt_InM_60;
  TH1D* rap_vs_Pt_InM_61;
  TH1D* rap_vs_Pt_InM_62;
  TH1D* rap_vs_Pt_InM_63;
  TH1D* rap_vs_Pt_InM_64;
  TH1D* rap_vs_Pt_InM_65;
  TH1D* rap_vs_Pt_InM_66;
  TH1D* rap_vs_Pt_InM_67;
  TH1D* rap_vs_Pt_InM_68;
  TH1D* rap_vs_Pt_InM_69;
  TH1D* rap_vs_Pt_InM_70;
  TH1D* rap_vs_Pt_InM_71;
  TH1D* rap_vs_Pt_InM_72;
  TH1D* rap_vs_Pt_InM_73;
  TH1D* rap_vs_Pt_InM_74;
  TH1D* rap_vs_Pt_InM_75;
  TH1D* rap_vs_Pt_InM_76;
  TH1D* rap_vs_Pt_InM_81;
  TH1D* rap_vs_Pt_InM_82;
  TH1D* rap_vs_Pt_InM_83;
  TH1D* rap_vs_Pt_InM_84;
  TH1D* rap_vs_Pt_InM_85;
  TH1D* rap_vs_Pt_InM_86;
  TH1D* rap_vs_Pt_InM_87;
  TH1D* rap_vs_Pt_InM_88;
  TH1D* rap_vs_Pt_InM_89;
  TH1D* rap_vs_Pt_InM_90;
  TH1D* rap_vs_Pt_InM_91;
  TH1D* rap_vs_Pt_InM_92;
  TH1D* rap_vs_Pt_InM_101;
  TH1D* rap_vs_Pt_InM_102;
  TH1D* rap_vs_Pt_InM_103;
  TH1D* rap_vs_Pt_InM_104;
  TH1D* rap_vs_Pt_InM_105;
  TH1D* rap_vs_Pt_InM_106;
  TH1D* rap_vs_Pt_InM_107;
  TH1D* rap_vs_Pt_InM_108;
  TH1D* rap_vs_Pt_InM_111;
  TH1D* rap_vs_Pt_InM_112;
  TH1D* rap_vs_Pt_InM_113;
  TH1D* rap_vs_Pt_InM_114;

  TH1D* rap_vs_Pt_InM_mixing_1;
  TH1D* rap_vs_Pt_InM_mixing_2;
  TH1D* rap_vs_Pt_InM_mixing_3;
  TH1D* rap_vs_Pt_InM_mixing_4;
  TH1D* rap_vs_Pt_InM_mixing_5;
  TH1D* rap_vs_Pt_InM_mixing_6;
  TH1D* rap_vs_Pt_InM_mixing_7;
  TH1D* rap_vs_Pt_InM_mixing_8;
  TH1D* rap_vs_Pt_InM_mixing_9;
  TH1D* rap_vs_Pt_InM_mixing_10;
  TH1D* rap_vs_Pt_InM_mixing_11;
  TH1D* rap_vs_Pt_InM_mixing_12;
  TH1D* rap_vs_Pt_InM_mixing_13;
  TH1D* rap_vs_Pt_InM_mixing_14;
  TH1D* rap_vs_Pt_InM_mixing_15;
  TH1D* rap_vs_Pt_InM_mixing_16;
  TH1D* rap_vs_Pt_InM_mixing_17;
  TH1D* rap_vs_Pt_InM_mixing_18;
  TH1D* rap_vs_Pt_InM_mixing_19;
  TH1D* rap_vs_Pt_InM_mixing_20;
  TH1D* rap_vs_Pt_InM_mixing_21;
  TH1D* rap_vs_Pt_InM_mixing_22;
  TH1D* rap_vs_Pt_InM_mixing_23;
  TH1D* rap_vs_Pt_InM_mixing_24;
  TH1D* rap_vs_Pt_InM_mixing_25;
  TH1D* rap_vs_Pt_InM_mixing_26;
  TH1D* rap_vs_Pt_InM_mixing_27;
  TH1D* rap_vs_Pt_InM_mixing_28;
  TH1D* rap_vs_Pt_InM_mixing_29;
  TH1D* rap_vs_Pt_InM_mixing_30;
  TH1D* rap_vs_Pt_InM_mixing_31;
  TH1D* rap_vs_Pt_InM_mixing_32;
  TH1D* rap_vs_Pt_InM_mixing_33;
  TH1D* rap_vs_Pt_InM_mixing_34;
  TH1D* rap_vs_Pt_InM_mixing_35;
  TH1D* rap_vs_Pt_InM_mixing_36;
  TH1D* rap_vs_Pt_InM_mixing_37;
  TH1D* rap_vs_Pt_InM_mixing_38;
  TH1D* rap_vs_Pt_InM_mixing_39;
  TH1D* rap_vs_Pt_InM_mixing_40;
  TH1D* rap_vs_Pt_InM_mixing_41;
  TH1D* rap_vs_Pt_InM_mixing_42;
  TH1D* rap_vs_Pt_InM_mixing_43;
  TH1D* rap_vs_Pt_InM_mixing_44;
  TH1D* rap_vs_Pt_InM_mixing_45;
  TH1D* rap_vs_Pt_InM_mixing_46;
  TH1D* rap_vs_Pt_InM_mixing_47;
  TH1D* rap_vs_Pt_InM_mixing_48;
  TH1D* rap_vs_Pt_InM_mixing_49;
  TH1D* rap_vs_Pt_InM_mixing_50;
  TH1D* rap_vs_Pt_InM_mixing_51;
  TH1D* rap_vs_Pt_InM_mixing_52;
  TH1D* rap_vs_Pt_InM_mixing_53;
  TH1D* rap_vs_Pt_InM_mixing_54;
  TH1D* rap_vs_Pt_InM_mixing_55;
  TH1D* rap_vs_Pt_InM_mixing_56;
  TH1D* rap_vs_Pt_InM_mixing_57;
  TH1D* rap_vs_Pt_InM_mixing_58;
  TH1D* rap_vs_Pt_InM_mixing_59;
  TH1D* rap_vs_Pt_InM_mixing_60;
  TH1D* rap_vs_Pt_InM_mixing_61;
  TH1D* rap_vs_Pt_InM_mixing_62;
  TH1D* rap_vs_Pt_InM_mixing_63;
  TH1D* rap_vs_Pt_InM_mixing_64;
  TH1D* rap_vs_Pt_InM_mixing_65;
  TH1D* rap_vs_Pt_InM_mixing_66;
  TH1D* rap_vs_Pt_InM_mixing_67;
  TH1D* rap_vs_Pt_InM_mixing_68;
  TH1D* rap_vs_Pt_InM_mixing_69;
  TH1D* rap_vs_Pt_InM_mixing_70;
  TH1D* rap_vs_Pt_InM_mixing_71;
  TH1D* rap_vs_Pt_InM_mixing_72;
  TH1D* rap_vs_Pt_InM_mixing_73;
  TH1D* rap_vs_Pt_InM_mixing_74;
  TH1D* rap_vs_Pt_InM_mixing_75;
  TH1D* rap_vs_Pt_InM_mixing_76;
  TH1D* rap_vs_Pt_InM_mixing_81;
  TH1D* rap_vs_Pt_InM_mixing_82;
  TH1D* rap_vs_Pt_InM_mixing_83;
  TH1D* rap_vs_Pt_InM_mixing_84;
  TH1D* rap_vs_Pt_InM_mixing_85;
  TH1D* rap_vs_Pt_InM_mixing_86;
  TH1D* rap_vs_Pt_InM_mixing_87;
  TH1D* rap_vs_Pt_InM_mixing_88;
  TH1D* rap_vs_Pt_InM_mixing_89;
  TH1D* rap_vs_Pt_InM_mixing_90;
  TH1D* rap_vs_Pt_InM_mixing_91;
  TH1D* rap_vs_Pt_InM_mixing_92;
  TH1D* rap_vs_Pt_InM_mixing_101;
  TH1D* rap_vs_Pt_InM_mixing_102;
  TH1D* rap_vs_Pt_InM_mixing_103;
  TH1D* rap_vs_Pt_InM_mixing_104;
  TH1D* rap_vs_Pt_InM_mixing_105;
  TH1D* rap_vs_Pt_InM_mixing_106;
  TH1D* rap_vs_Pt_InM_mixing_107;
  TH1D* rap_vs_Pt_InM_mixing_108;
  TH1D* rap_vs_Pt_InM_mixing_111;
  TH1D* rap_vs_Pt_InM_mixing_112;
  TH1D* rap_vs_Pt_InM_mixing_113;
  TH1D* rap_vs_Pt_InM_mixing_114;


  // //   rap_vs_Pt for "All" and "Both"
  vector<TH1*> fHistoList_rap_vs_pt_InM_all;
  TH1D* rap_vs_Pt_InM_all_1;
  TH1D* rap_vs_Pt_InM_all_2;
  TH1D* rap_vs_Pt_InM_all_3;
  TH1D* rap_vs_Pt_InM_all_4;
  TH1D* rap_vs_Pt_InM_all_5;
  TH1D* rap_vs_Pt_InM_all_6;
  TH1D* rap_vs_Pt_InM_all_7;
  TH1D* rap_vs_Pt_InM_all_8;
  TH1D* rap_vs_Pt_InM_all_9;
  TH1D* rap_vs_Pt_InM_all_10;
  TH1D* rap_vs_Pt_InM_all_11;
  TH1D* rap_vs_Pt_InM_all_12;
  TH1D* rap_vs_Pt_InM_all_13;
  TH1D* rap_vs_Pt_InM_all_14;
  TH1D* rap_vs_Pt_InM_all_15;
  TH1D* rap_vs_Pt_InM_all_16;
  TH1D* rap_vs_Pt_InM_all_17;
  TH1D* rap_vs_Pt_InM_all_18;
  TH1D* rap_vs_Pt_InM_all_19;
  TH1D* rap_vs_Pt_InM_all_20;
  TH1D* rap_vs_Pt_InM_all_21;
  TH1D* rap_vs_Pt_InM_all_22;
  TH1D* rap_vs_Pt_InM_all_23;
  TH1D* rap_vs_Pt_InM_all_24;
  TH1D* rap_vs_Pt_InM_all_25;
  TH1D* rap_vs_Pt_InM_all_26;
  TH1D* rap_vs_Pt_InM_all_27;
  TH1D* rap_vs_Pt_InM_all_28;
  TH1D* rap_vs_Pt_InM_all_29;
  TH1D* rap_vs_Pt_InM_all_30;
  TH1D* rap_vs_Pt_InM_all_31;
  TH1D* rap_vs_Pt_InM_all_32;
  TH1D* rap_vs_Pt_InM_all_33;
  TH1D* rap_vs_Pt_InM_all_34;
  TH1D* rap_vs_Pt_InM_all_35;
  TH1D* rap_vs_Pt_InM_all_36;
  TH1D* rap_vs_Pt_InM_all_37;
  TH1D* rap_vs_Pt_InM_all_38;
  TH1D* rap_vs_Pt_InM_all_39;
  TH1D* rap_vs_Pt_InM_all_40;
  TH1D* rap_vs_Pt_InM_all_41;
  TH1D* rap_vs_Pt_InM_all_42;
  TH1D* rap_vs_Pt_InM_all_43;
  TH1D* rap_vs_Pt_InM_all_44;
  TH1D* rap_vs_Pt_InM_all_45;
  TH1D* rap_vs_Pt_InM_all_46;
  TH1D* rap_vs_Pt_InM_all_47;
  TH1D* rap_vs_Pt_InM_all_48;
  TH1D* rap_vs_Pt_InM_all_49;
  TH1D* rap_vs_Pt_InM_all_50;
  TH1D* rap_vs_Pt_InM_all_51;
  TH1D* rap_vs_Pt_InM_all_52;
  TH1D* rap_vs_Pt_InM_all_53;
  TH1D* rap_vs_Pt_InM_all_54;
  TH1D* rap_vs_Pt_InM_all_55;
  TH1D* rap_vs_Pt_InM_all_56;
  TH1D* rap_vs_Pt_InM_all_57;
  TH1D* rap_vs_Pt_InM_all_58;
  TH1D* rap_vs_Pt_InM_all_59;
  TH1D* rap_vs_Pt_InM_all_60;
  TH1D* rap_vs_Pt_InM_all_61;
  TH1D* rap_vs_Pt_InM_all_62;
  TH1D* rap_vs_Pt_InM_all_63;
  TH1D* rap_vs_Pt_InM_all_64;
  TH1D* rap_vs_Pt_InM_all_65;
  TH1D* rap_vs_Pt_InM_all_66;
  TH1D* rap_vs_Pt_InM_all_67;
  TH1D* rap_vs_Pt_InM_all_68;
  TH1D* rap_vs_Pt_InM_all_69;
  TH1D* rap_vs_Pt_InM_all_70;
  TH1D* rap_vs_Pt_InM_all_71;
  TH1D* rap_vs_Pt_InM_all_72;
  TH1D* rap_vs_Pt_InM_all_73;
  TH1D* rap_vs_Pt_InM_all_74;
  TH1D* rap_vs_Pt_InM_all_75;
  TH1D* rap_vs_Pt_InM_all_76;
  TH1D* rap_vs_Pt_InM_all_81;
  TH1D* rap_vs_Pt_InM_all_82;
  TH1D* rap_vs_Pt_InM_all_83;
  TH1D* rap_vs_Pt_InM_all_84;
  TH1D* rap_vs_Pt_InM_all_85;
  TH1D* rap_vs_Pt_InM_all_86;
  TH1D* rap_vs_Pt_InM_all_87;
  TH1D* rap_vs_Pt_InM_all_88;
  TH1D* rap_vs_Pt_InM_all_89;
  TH1D* rap_vs_Pt_InM_all_90;
  TH1D* rap_vs_Pt_InM_all_91;
  TH1D* rap_vs_Pt_InM_all_92;
  TH1D* rap_vs_Pt_InM_all_101;
  TH1D* rap_vs_Pt_InM_all_102;
  TH1D* rap_vs_Pt_InM_all_103;
  TH1D* rap_vs_Pt_InM_all_104;
  TH1D* rap_vs_Pt_InM_all_105;
  TH1D* rap_vs_Pt_InM_all_106;
  TH1D* rap_vs_Pt_InM_all_107;
  TH1D* rap_vs_Pt_InM_all_108;
  TH1D* rap_vs_Pt_InM_all_111;
  TH1D* rap_vs_Pt_InM_all_112;
  TH1D* rap_vs_Pt_InM_all_113;
  TH1D* rap_vs_Pt_InM_all_114;

  TH1D* rap_vs_Pt_InM_all_mixing_1;
  TH1D* rap_vs_Pt_InM_all_mixing_2;
  TH1D* rap_vs_Pt_InM_all_mixing_3;
  TH1D* rap_vs_Pt_InM_all_mixing_4;
  TH1D* rap_vs_Pt_InM_all_mixing_5;
  TH1D* rap_vs_Pt_InM_all_mixing_6;
  TH1D* rap_vs_Pt_InM_all_mixing_7;
  TH1D* rap_vs_Pt_InM_all_mixing_8;
  TH1D* rap_vs_Pt_InM_all_mixing_9;
  TH1D* rap_vs_Pt_InM_all_mixing_10;
  TH1D* rap_vs_Pt_InM_all_mixing_11;
  TH1D* rap_vs_Pt_InM_all_mixing_12;
  TH1D* rap_vs_Pt_InM_all_mixing_13;
  TH1D* rap_vs_Pt_InM_all_mixing_14;
  TH1D* rap_vs_Pt_InM_all_mixing_15;
  TH1D* rap_vs_Pt_InM_all_mixing_16;
  TH1D* rap_vs_Pt_InM_all_mixing_17;
  TH1D* rap_vs_Pt_InM_all_mixing_18;
  TH1D* rap_vs_Pt_InM_all_mixing_19;
  TH1D* rap_vs_Pt_InM_all_mixing_20;
  TH1D* rap_vs_Pt_InM_all_mixing_21;
  TH1D* rap_vs_Pt_InM_all_mixing_22;
  TH1D* rap_vs_Pt_InM_all_mixing_23;
  TH1D* rap_vs_Pt_InM_all_mixing_24;
  TH1D* rap_vs_Pt_InM_all_mixing_25;
  TH1D* rap_vs_Pt_InM_all_mixing_26;
  TH1D* rap_vs_Pt_InM_all_mixing_27;
  TH1D* rap_vs_Pt_InM_all_mixing_28;
  TH1D* rap_vs_Pt_InM_all_mixing_29;
  TH1D* rap_vs_Pt_InM_all_mixing_30;
  TH1D* rap_vs_Pt_InM_all_mixing_31;
  TH1D* rap_vs_Pt_InM_all_mixing_32;
  TH1D* rap_vs_Pt_InM_all_mixing_33;
  TH1D* rap_vs_Pt_InM_all_mixing_34;
  TH1D* rap_vs_Pt_InM_all_mixing_35;
  TH1D* rap_vs_Pt_InM_all_mixing_36;
  TH1D* rap_vs_Pt_InM_all_mixing_37;
  TH1D* rap_vs_Pt_InM_all_mixing_38;
  TH1D* rap_vs_Pt_InM_all_mixing_39;
  TH1D* rap_vs_Pt_InM_all_mixing_40;
  TH1D* rap_vs_Pt_InM_all_mixing_41;
  TH1D* rap_vs_Pt_InM_all_mixing_42;
  TH1D* rap_vs_Pt_InM_all_mixing_43;
  TH1D* rap_vs_Pt_InM_all_mixing_44;
  TH1D* rap_vs_Pt_InM_all_mixing_45;
  TH1D* rap_vs_Pt_InM_all_mixing_46;
  TH1D* rap_vs_Pt_InM_all_mixing_47;
  TH1D* rap_vs_Pt_InM_all_mixing_48;
  TH1D* rap_vs_Pt_InM_all_mixing_49;
  TH1D* rap_vs_Pt_InM_all_mixing_50;
  TH1D* rap_vs_Pt_InM_all_mixing_51;
  TH1D* rap_vs_Pt_InM_all_mixing_52;
  TH1D* rap_vs_Pt_InM_all_mixing_53;
  TH1D* rap_vs_Pt_InM_all_mixing_54;
  TH1D* rap_vs_Pt_InM_all_mixing_55;
  TH1D* rap_vs_Pt_InM_all_mixing_56;
  TH1D* rap_vs_Pt_InM_all_mixing_57;
  TH1D* rap_vs_Pt_InM_all_mixing_58;
  TH1D* rap_vs_Pt_InM_all_mixing_59;
  TH1D* rap_vs_Pt_InM_all_mixing_60;
  TH1D* rap_vs_Pt_InM_all_mixing_61;
  TH1D* rap_vs_Pt_InM_all_mixing_62;
  TH1D* rap_vs_Pt_InM_all_mixing_63;
  TH1D* rap_vs_Pt_InM_all_mixing_64;
  TH1D* rap_vs_Pt_InM_all_mixing_65;
  TH1D* rap_vs_Pt_InM_all_mixing_66;
  TH1D* rap_vs_Pt_InM_all_mixing_67;
  TH1D* rap_vs_Pt_InM_all_mixing_68;
  TH1D* rap_vs_Pt_InM_all_mixing_69;
  TH1D* rap_vs_Pt_InM_all_mixing_70;
  TH1D* rap_vs_Pt_InM_all_mixing_71;
  TH1D* rap_vs_Pt_InM_all_mixing_72;
  TH1D* rap_vs_Pt_InM_all_mixing_73;
  TH1D* rap_vs_Pt_InM_all_mixing_74;
  TH1D* rap_vs_Pt_InM_all_mixing_75;
  TH1D* rap_vs_Pt_InM_all_mixing_76;
  TH1D* rap_vs_Pt_InM_all_mixing_81;
  TH1D* rap_vs_Pt_InM_all_mixing_82;
  TH1D* rap_vs_Pt_InM_all_mixing_83;
  TH1D* rap_vs_Pt_InM_all_mixing_84;
  TH1D* rap_vs_Pt_InM_all_mixing_85;
  TH1D* rap_vs_Pt_InM_all_mixing_86;
  TH1D* rap_vs_Pt_InM_all_mixing_87;
  TH1D* rap_vs_Pt_InM_all_mixing_88;
  TH1D* rap_vs_Pt_InM_all_mixing_89;
  TH1D* rap_vs_Pt_InM_all_mixing_90;
  TH1D* rap_vs_Pt_InM_all_mixing_91;
  TH1D* rap_vs_Pt_InM_all_mixing_92;
  TH1D* rap_vs_Pt_InM_all_mixing_101;
  TH1D* rap_vs_Pt_InM_all_mixing_102;
  TH1D* rap_vs_Pt_InM_all_mixing_103;
  TH1D* rap_vs_Pt_InM_all_mixing_104;
  TH1D* rap_vs_Pt_InM_all_mixing_105;
  TH1D* rap_vs_Pt_InM_all_mixing_106;
  TH1D* rap_vs_Pt_InM_all_mixing_107;
  TH1D* rap_vs_Pt_InM_all_mixing_108;
  TH1D* rap_vs_Pt_InM_all_mixing_111;
  TH1D* rap_vs_Pt_InM_all_mixing_112;
  TH1D* rap_vs_Pt_InM_all_mixing_113;
  TH1D* rap_vs_Pt_InM_all_mixing_114;

  //pt separation "onetwo"
  vector<TH1*> fHistoList_pt_onetwo;
  TH1D* Pi0_pt_est_onetwo_Both;
  TH1D* pt_onetwo_1;
  TH1D* pt_onetwo_2;
  TH1D* pt_onetwo_3;
  TH1D* pt_onetwo_4;
  TH1D* pt_onetwo_5;
  TH1D* pt_onetwo_6;
  TH1D* pt_onetwo_7;
  TH1D* pt_onetwo_8;
  TH1D* pt_onetwo_9;
  TH1D* pt_onetwo_10;
  TH1D* pt_onetwo_11;
  TH1D* pt_onetwo_12;
  TH1D* pt_onetwo_13;
  TH1D* pt_onetwo_14;
  TH1D* pt_onetwo_15;
  TH1D* pt_onetwo_16;
  TH1D* pt_onetwo_17;
  TH1D* pt_onetwo_18;
  TH1D* pt_onetwo_19;
  TH1D* pt_onetwo_20;

  TH1D* pt_onetwo_mixing_1;
  TH1D* pt_onetwo_mixing_2;
  TH1D* pt_onetwo_mixing_3;
  TH1D* pt_onetwo_mixing_4;
  TH1D* pt_onetwo_mixing_5;
  TH1D* pt_onetwo_mixing_6;
  TH1D* pt_onetwo_mixing_7;
  TH1D* pt_onetwo_mixing_8;
  TH1D* pt_onetwo_mixing_9;
  TH1D* pt_onetwo_mixing_10;
  TH1D* pt_onetwo_mixing_11;
  TH1D* pt_onetwo_mixing_12;
  TH1D* pt_onetwo_mixing_13;
  TH1D* pt_onetwo_mixing_14;
  TH1D* pt_onetwo_mixing_15;
  TH1D* pt_onetwo_mixing_16;
  TH1D* pt_onetwo_mixing_17;
  TH1D* pt_onetwo_mixing_18;
  TH1D* pt_onetwo_mixing_19;
  TH1D* pt_onetwo_mixing_20;

  //pt separation "all"
  vector<TH1*> fHistoList_pt_all;
  TH1D* Pi0_pt_est_all_Both;
  TH1D* pt_all_1;
  TH1D* pt_all_2;
  TH1D* pt_all_3;
  TH1D* pt_all_4;
  TH1D* pt_all_5;
  TH1D* pt_all_6;
  TH1D* pt_all_7;
  TH1D* pt_all_8;
  TH1D* pt_all_9;
  TH1D* pt_all_10;
  TH1D* pt_all_11;
  TH1D* pt_all_12;
  TH1D* pt_all_13;
  TH1D* pt_all_14;
  TH1D* pt_all_15;
  TH1D* pt_all_16;
  TH1D* pt_all_17;
  TH1D* pt_all_18;
  TH1D* pt_all_19;
  TH1D* pt_all_20;

  TH1D* pt_all_mixing_1;
  TH1D* pt_all_mixing_2;
  TH1D* pt_all_mixing_3;
  TH1D* pt_all_mixing_4;
  TH1D* pt_all_mixing_5;
  TH1D* pt_all_mixing_6;
  TH1D* pt_all_mixing_7;
  TH1D* pt_all_mixing_8;
  TH1D* pt_all_mixing_9;
  TH1D* pt_all_mixing_10;
  TH1D* pt_all_mixing_11;
  TH1D* pt_all_mixing_12;
  TH1D* pt_all_mixing_13;
  TH1D* pt_all_mixing_14;
  TH1D* pt_all_mixing_15;
  TH1D* pt_all_mixing_16;
  TH1D* pt_all_mixing_17;
  TH1D* pt_all_mixing_18;
  TH1D* pt_all_mixing_19;
  TH1D* pt_all_mixing_20;


  // BG cases Target
  vector<TH1*> fHistoList_bg_InM_all_Target;
  vector<TH1*> fHistoList_bg_InM_zero_Target;
  vector<TH1*> fHistoList_bg_InM_one_Target;
  vector<TH1*> fHistoList_bg_InM_two_Target;
  vector<TH1*> fHistoList_bg_InM_onetwo_Target;
  // BG cases Outside
  vector<TH1*> fHistoList_bg_InM_all_Outside;
  vector<TH1*> fHistoList_bg_InM_zero_Outside;
  vector<TH1*> fHistoList_bg_InM_one_Outside;
  vector<TH1*> fHistoList_bg_InM_two_Outside;
  vector<TH1*> fHistoList_bg_InM_onetwo_Outside;

  // BG cases Both
  // all
  vector<TH1*> fHistoList_bg_InM_all_Both;
  TH1D* BG1_InM_all_Both;
  TH1D* BG2_InM_all_Both;
  TH1D* BG3_InM_all_Both;
  TH1D* BG4_InM_all_Both;
  TH1D* BG5_InM_all_Both;
  TH1D* BG6_InM_all_Both;
  TH1D* BG7_InM_all_Both;
  TH1D* BG8_InM_all_Both;
  TH1D* BG9_InM_all_Both;
  TH1D* BG10_InM_all_Both;
  TH1D* PdgCase8_InM_all_Both;
  TH1D* PdgCase8mothers_InM_all_Both;
  TH1D* sameMIDcase8_InM_all_Both;
  TH1D* sameGRIDcase8_InM_all_Both;
  TH2D* Case1ZYPos_InM_all_Both;
  TH1D* sameMIDcase8_mothedPDG_InM_all_Both;
  TH1D* PdgCase8NonEComeFromTarget_InM_all_Both;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_all_Both;
  TH1D* PdgCase8motherNonE_InM_all_Both;
  TH1D* Case8ElFromDalitz_InM_all_Both;
  TH1D* Case8NonElFrom_pn_InM_all_Both;
  TH1D* Case8NonElFrom_eta_InM_all_Both;
  TH1D* Case8NonElFrom_kaon_InM_all_Both;
  TH1D* sameMIDcase8NonEPdg_InM_all_Both;
  TH1D* sameMIDcase8NonEMotherPdg_InM_all_Both;
  TH1D* sameMIDcase8NonEMotherIM_InM_all_Both;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_all_Both;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_all_Both;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_all_Both;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_all_Both;
  // zero
  vector<TH1*> fHistoList_bg_InM_zero_Both;
  TH1D* BG1_InM_zero_Both;
  TH1D* BG2_InM_zero_Both;
  TH1D* BG3_InM_zero_Both;
  TH1D* BG4_InM_zero_Both;
  TH1D* BG5_InM_zero_Both;
  TH1D* BG6_InM_zero_Both;
  TH1D* BG7_InM_zero_Both;
  TH1D* BG8_InM_zero_Both;
  TH1D* BG9_InM_zero_Both;
  TH1D* BG10_InM_zero_Both;
  TH1D* PdgCase8_InM_zero_Both;
  TH1D* PdgCase8mothers_InM_zero_Both;
  TH1D* sameMIDcase8_InM_zero_Both;
  TH1D* sameGRIDcase8_InM_zero_Both;
  TH2D* Case1ZYPos_InM_zero_Both;
  TH1D* sameMIDcase8_mothedPDG_InM_zero_Both;
  TH1D* PdgCase8NonEComeFromTarget_InM_zero_Both;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_zero_Both;
  TH1D* PdgCase8motherNonE_InM_zero_Both;
  TH1D* Case8ElFromDalitz_InM_zero_Both;
  TH1D* Case8NonElFrom_pn_InM_zero_Both;
  TH1D* Case8NonElFrom_eta_InM_zero_Both;
  TH1D* Case8NonElFrom_kaon_InM_zero_Both;
  TH1D* sameMIDcase8NonEPdg_InM_zero_Both;
  TH1D* sameMIDcase8NonEMotherPdg_InM_zero_Both;
  TH1D* sameMIDcase8NonEMotherIM_InM_zero_Both;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_zero_Both;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_zero_Both;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_zero_Both;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_zero_Both;
  // one
  vector<TH1*> fHistoList_bg_InM_one_Both;
  TH1D* BG1_InM_one_Both;
  TH1D* BG2_InM_one_Both;
  TH1D* BG3_InM_one_Both;
  TH1D* BG4_InM_one_Both;
  TH1D* BG5_InM_one_Both;
  TH1D* BG6_InM_one_Both;
  TH1D* BG7_InM_one_Both;
  TH1D* BG8_InM_one_Both;
  TH1D* BG9_InM_one_Both;
  TH1D* BG10_InM_one_Both;
  TH1D* PdgCase8_InM_one_Both;
  TH1D* PdgCase8mothers_InM_one_Both;
  TH1D* sameMIDcase8_InM_one_Both;
  TH1D* sameGRIDcase8_InM_one_Both;
  TH2D* Case1ZYPos_InM_one_Both;
  TH1D* sameMIDcase8_mothedPDG_InM_one_Both;
  TH1D* PdgCase8NonEComeFromTarget_InM_one_Both;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_one_Both;
  TH1D* PdgCase8motherNonE_InM_one_Both;
  TH1D* Case8ElFromDalitz_InM_one_Both;
  TH1D* Case8NonElFrom_pn_InM_one_Both;
  TH1D* Case8NonElFrom_eta_InM_one_Both;
  TH1D* Case8NonElFrom_kaon_InM_one_Both;
  TH1D* sameMIDcase8NonEPdg_InM_one_Both;
  TH1D* sameMIDcase8NonEMotherPdg_InM_one_Both;
  TH1D* sameMIDcase8NonEMotherIM_InM_one_Both;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_one_Both;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_one_Both;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_one_Both;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_one_Both;
  // two
  vector<TH1*> fHistoList_bg_InM_two_Both;
  TH1D* BG1_InM_two_Both;
  TH1D* BG2_InM_two_Both;
  TH1D* BG3_InM_two_Both;
  TH1D* BG4_InM_two_Both;
  TH1D* BG5_InM_two_Both;
  TH1D* BG6_InM_two_Both;
  TH1D* BG7_InM_two_Both;
  TH1D* BG8_InM_two_Both;
  TH1D* BG9_InM_two_Both;
  TH1D* BG10_InM_two_Both;
  TH1D* PdgCase8_InM_two_Both;
  TH1D* PdgCase8mothers_InM_two_Both;
  TH1D* sameMIDcase8_InM_two_Both;
  TH1D* sameGRIDcase8_InM_two_Both;
  TH2D* Case1ZYPos_InM_two_Both;
  TH1D* sameMIDcase8_mothedPDG_InM_two_Both;
  TH1D* PdgCase8NonEComeFromTarget_InM_two_Both;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_two_Both;
  TH1D* PdgCase8motherNonE_InM_two_Both;
  TH1D* Case8ElFromDalitz_InM_two_Both;
  TH1D* Case8NonElFrom_pn_InM_two_Both;
  TH1D* Case8NonElFrom_eta_InM_two_Both;
  TH1D* Case8NonElFrom_kaon_InM_two_Both;
  TH1D* sameMIDcase8NonEPdg_InM_two_Both;
  TH1D* sameMIDcase8NonEMotherPdg_InM_two_Both;
  TH1D* sameMIDcase8NonEMotherIM_InM_two_Both;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_two_Both;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_two_Both;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_two_Both;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_two_Both;
  // onetwo
  vector<TH1*> fHistoList_bg_InM_onetwo_Both;
  TH1D* BG1_InM_onetwo_Both;
  TH1D* BG2_InM_onetwo_Both;
  TH1D* BG3_InM_onetwo_Both;
  TH1D* BG4_InM_onetwo_Both;
  TH1D* BG5_InM_onetwo_Both;
  TH1D* BG6_InM_onetwo_Both;
  TH1D* BG7_InM_onetwo_Both;
  TH1D* BG8_InM_onetwo_Both;
  TH1D* BG9_InM_onetwo_Both;
  TH1D* BG10_InM_onetwo_Both;
  TH1D* PdgCase8_InM_onetwo_Both;
  TH1D* PdgCase8mothers_InM_onetwo_Both;
  TH1D* sameMIDcase8_InM_onetwo_Both;
  TH1D* sameGRIDcase8_InM_onetwo_Both;
  TH2D* Case1ZYPos_InM_onetwo_Both;
  TH1D* sameMIDcase8_mothedPDG_InM_onetwo_Both;
  TH1D* PdgCase8NonEComeFromTarget_InM_onetwo_Both;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_onetwo_Both;
  TH1D* PdgCase8motherNonE_InM_onetwo_Both;
  TH1D* Case8ElFromDalitz_InM_onetwo_Both;
  TH1D* Case8NonElFrom_pn_InM_onetwo_Both;
  TH1D* Case8NonElFrom_eta_InM_onetwo_Both;
  TH1D* Case8NonElFrom_kaon_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEPdg_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEMotherPdg_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEMotherIM_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_onetwo_Both;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_onetwo_Both;


  TH1D* AnnTruePairs;
  TH1D* AnnFalsePairs;
  TH1D* AnnTruePairs_AfterCuts;
  TH1D* AnnFalsePairs_AfterCuts;

  //WAC
  vector<TH1*> fHistoList_man_WAC;
  TH1D* Pi0InvMassReco_WAC;
  TH1D* EMT_InvMass_WAC;
  TH2D* Pi0_pt_vs_rap_WAC;
  TH2D* Pi0_pt_vs_rap_est_WAC;
  TH1D* DalitzPi0_WAC;
  TH1D* PhotonsPi0_WAC;
  TH2D* MultiplicityGamma_WAC;
  TH2D* MultiplicityChargedParticles_WAC;
  vector<TH1*> fHistoList_bg_InM_WAC;
  TH1D* BG1_InM_WAC;
  TH1D* BG2_InM_WAC;
  TH1D* BG3_InM_WAC;
  TH1D* BG4_InM_WAC;
  TH1D* BG5_InM_WAC;
  TH1D* BG6_InM_WAC;
  TH1D* BG7_InM_WAC;
  TH1D* BG8_InM_WAC;
  TH1D* BG9_InM_WAC;
  TH1D* BG10_InM_WAC;
  TH1D* PdgCase8_InM_WAC;
  TH1D* PdgCase8mothers_InM_WAC;
  TH1D* sameMIDcase8_InM_WAC;
  TH1D* sameGRIDcase8_InM_WAC;
  TH2D* Case1ZYPos_InM_WAC;
  TH1D* sameMIDcase8_mothedPDG_InM_WAC;
  TH1D* PdgCase8NonEComeFromTarget_InM_WAC;
  TH1D* PdgCase8NonE_NOT_FromTarget_InM_WAC;
  TH1D* PdgCase8motherNonE_InM_WAC;
  TH1D* Case8ElFromDalitz_InM_WAC;
  TH1D* Case8NonElFrom_pn_InM_WAC;
  TH1D* Case8NonElFrom_eta_InM_WAC;
  TH1D* Case8NonElFrom_kaon_InM_WAC;
  TH1D* sameMIDcase8NonEPdg_InM_WAC;
  TH1D* sameMIDcase8NonEMotherPdg_InM_WAC;
  TH1D* sameMIDcase8NonEMotherIM_InM_WAC;
  TH1D* sameMIDcase8NonEPdgFromTarget_InM_WAC;
  TH1D* sameMIDcase8NonEComeFromTargetIM_InM_WAC;
  TH1D* sameMIDcase8NonEComeFromTargetP_InM_WAC;
  TH1D* sameMIDcase8NonEComeFromTargetPt_InM_WAC;

  vector<TH1*> fHistoList_manual;
  TH1D* Chi2_for_Primary;
  TH1D* Chi2_for_Secondary;


  //***** brief Copy constructor.
  CbmKresConversionManual(const CbmKresConversionManual&);

  //***** brief Assignment operator.
  CbmKresConversionManual operator=(const CbmKresConversionManual&);


  ClassDef(CbmKresConversionManual, 1)
};

#endif
