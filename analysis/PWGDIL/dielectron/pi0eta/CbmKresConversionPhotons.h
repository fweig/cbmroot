#ifndef CBM_KRES_CONVERSION_PHOTONS
#define CBM_KRES_CONVERSION_PHOTONS

#include <TClonesArray.h>
#include "TH2D.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmRichRing.h"
#include "CbmKFVertex.h"
#include "CbmVertex.h"
#include "KFParticle.h"
#include "CbmRichRingFitterEllipseTau.h"

#include "CbmKresTrainAnnDirectPhotons.h"
#include "CbmKresSelectAnnPhotons.h"

#include "CbmLmvmKinematicParams.h"

class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresConversionPhotons
{

public: 
  //***** brief Standard constructor.
  CbmKresConversionPhotons();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionPhotons(); 


  void Init();
  void InitHistograms();
  void Finish();

  void Exec(int fEventNumDP, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);

  void SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);

  int FindInRich(int richInd, int stsMcTrackId);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  std::vector<TVector3> SaveAllHits(CbmStsTrack* track);

  void FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus, 
                        vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                        vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus, 
                        std::vector<int> Rings_minus, std::vector<int> Rings_plus, 
                        std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus, 
                        vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus);

  void FindGammasOutside(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus_Outside, vector<CbmMCTrack*> MCtracks_plus_Outside,
                        vector<CbmStsTrack*> StsTrack_minus_Outside, vector<CbmStsTrack*> StsTrack_plus_Outside,
                        std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
                        std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside, 
                        vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside);

  void FindGammasBoth();

  double CalculatePlaneAngle_last_fromHits(std::vector<TVector3> track_1, std::vector<TVector3> track_2);

  double CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  double CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  void DP_likesign_Mixing_Target(double AngleCut, double InvMassCut);

  void DP_likesign_Mixing_Outside(double AngleCut, double InvMassCut);

  void DP_Mixing_Target(double AngleCut, double InvMassCut);

  void DP_Mixing_Outside(double AngleCut, double InvMassCut);





private:

  CbmKresTrainAnnDirectPhotons * fTrainPhotons;
  Int_t AnnTrainPhotons;
  CbmKresSelectAnnPhotons * fAnnPhotonsSelection;
  Int_t UseAnnPhotons;

  
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

  CbmVertex * fPrimVertex;
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


  vector<TVector3> frefmomenta;
  std::vector<int> frefId;
  std::vector<CbmMCTrack*> fMCtracks;

  // DP_LK_EMT Target
  std::vector<TVector3>                 DP_LK_EMT_momenta_minus_Target;
  std::vector<int>                      DP_LK_EMT_NofRings_minus_Target;      
  std::vector<CbmStsTrack*>             DP_LK_EMT_STS_minus_Target;
  std::vector <int>                     DP_LK_EMT_STS_minus_index_Target;

  std::vector<TVector3>                 DP_LK_EMT_momenta_plus_Target;
  std::vector<int>                      DP_LK_EMT_NofRings_plus_Target;
  std::vector<CbmStsTrack*>             DP_LK_EMT_STS_plus_Target;
  std::vector <int>                     DP_LK_EMT_STS_plus_index_Target;

  // DP_LK_EMT Outside
  std::vector<int>                      DP_LK_EMT_NofRings_minus_Outside;      
  std::vector<CbmStsTrack*>             DP_LK_EMT_STS_minus_Outside;
  std::vector <int>                     DP_LK_EMT_STS_minus_index_Outside;

  std::vector<int>                      DP_LK_EMT_NofRings_plus_Outside;
  std::vector<CbmStsTrack*>             DP_LK_EMT_STS_plus_Outside;
  std::vector <int>                     DP_LK_EMT_STS_plus_index_Outside;


  // DP_EMT Target
  std::vector<int>                      DP_EMT_Event_minus_Target;
  std::vector<TVector3>                 DP_EMT_momenta_minus_Target;
  std::vector<int>                      DP_EMT_NofRings_minus_Target;
  std::vector< std::vector<TVector3> >  DP_EMT_Hits_minus_Target;

  std::vector<int>                      DP_EMT_Event_plus_Target;
  std::vector<TVector3>                 DP_EMT_momenta_plus_Target;
  std::vector<int>                      DP_EMT_NofRings_plus_Target;
  std::vector< std::vector<TVector3> >  DP_EMT_Hits_plus_Target;

  // DP_EMT Outside
  std::vector<int>                      DP_EMT_Event_minus_Outside;
  std::vector<CbmStsTrack*>             DP_EMT_STS_minus_Outside;
  std::vector<KFParticle>               DP_EMT_KFTrack_minus_Outside;
  std::vector<int>                      DP_EMT_NofRings_minus_Outside;
  std::vector< std::vector<TVector3> >  DP_EMT_Hits_minus_Outside;

  std::vector<int>                      DP_EMT_Event_plus_Outside;
  std::vector<CbmStsTrack*>             DP_EMT_STS_plus_Outside;
  std::vector<KFParticle>               DP_EMT_KFTrack_plus_Outside;
  std::vector<int>                      DP_EMT_NofRings_plus_Outside;
  std::vector< std::vector<TVector3> >  DP_EMT_Hits_plus_Outside;


  // combined gammas from the target
  std::vector< std::vector<TVector3> > Gammas_all_Target;  
  std::vector< std::vector<TVector3> > Gammas_zero_Target;
  std::vector< std::vector<TVector3> > Gammas_one_Target;
  std::vector< std::vector<TVector3> > Gammas_two_Target;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Target;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Target;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Target;


  // combined gammas from outside
  std::vector< std::vector<TVector3> > Gammas_all_Outside;
  std::vector< std::vector<TVector3> > Gammas_zero_Outside;
  std::vector< std::vector<TVector3> > Gammas_one_Outside;
  std::vector< std::vector<TVector3> > Gammas_two_Outside;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Outside;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Outside;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Outside;


  // combined gammas from the target and outside together
  std::vector< std::vector<TVector3> > Gammas_all_Both;
  std::vector< std::vector<TVector3> > Gammas_zero_Both;
  std::vector< std::vector<TVector3> > Gammas_one_Both;
  std::vector< std::vector<TVector3> > Gammas_two_Both;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Both;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Both;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Both;


  // histograms
  // Target
  vector<TH1*> fHistoList_dp_Target;
  TH1D * Mother_PDG_Target;
  TH1D * GrandMother_PDG_Target;

  // Outside
  vector<TH1*> fHistoList_dp_Outside;
  TH1D * Mother_PDG_Outside;
  TH1D * GrandMother_PDG_Outside;

  // Both
  vector<TH1*> fHistoList_dp_Both;
  TH2D * Pdg_vs_Distance_for_dp;
  TH2D * P_vs_Distance_for_dp;
  TH1D * DP_AnnTruePairs;
  TH1D * DP_AnnFalsePairs;
  TH1D * DP_AnnTruePairs_AfterCuts;
  TH1D * DP_AnnFalsePairs_AfterCuts;


  ///////   to check Cuts
  ///////   Both
  vector<TH1*> fHistoList_dp_cuts_Both;
  TH2D * DP_candidates_InvMass_vs_OA_Both;
  TH2D * DP_InvMass_vs_OA_Both;
  TH1D * DP_candidates_InvMass_Both;
  TH1D * DP_InvMass_Both;
  TH1D * DP_candidates_OA_Both;
  TH1D * DP_OA_Both;
  TH1D * DP_candidates_PlaneAngles_last_Both;
  TH1D * DP_PlaneAngles_last_Both;
  TH1D * DP_candidates_PlaneAngles_first_Both;
  TH1D * DP_PlaneAngles_first_Both;

  ///////   Target
  vector<TH1*> fHistoList_dp_cuts_Target;
  TH2D * DP_candidates_InvMass_vs_OA_Target;
  TH2D * DP_InvMass_vs_OA_Target;
  TH1D * DP_candidates_InvMass_Target;
  TH1D * DP_InvMass_Target;
  TH1D * DP_candidates_OA_Target;
  TH1D * DP_OA_Target;
  TH1D * DP_candidates_PlaneAngles_last_Target;
  TH1D * DP_PlaneAngles_last_Target;
  TH1D * DP_candidates_PlaneAngles_first_Target;
  TH1D * DP_PlaneAngles_first_Target;

  ///////   Outside
  vector<TH1*> fHistoList_dp_cuts_Outside;
  TH2D * DP_candidates_InvMass_vs_OA_Outside;
  TH2D * DP_InvMass_vs_OA_Outside;
  TH1D * DP_candidates_InvMass_Outside;
  TH1D * DP_InvMass_Outside;
  TH1D * DP_candidates_OA_Outside;
  TH1D * DP_OA_Outside;
  TH1D * DP_candidates_PlaneAngles_last_Outside;
  TH1D * DP_PlaneAngles_last_Outside;
  TH1D * DP_candidates_PlaneAngles_first_Outside;
  TH1D * DP_PlaneAngles_first_Outside;






  // Target => all
  vector<TH1*> fHistoList_dp_all_Target;
  TH1D * DP_InvMassReco_all_Target;
  TH1D * DP_OpeningAngleReco_all_Target;
  TH1D * DP_Pdg_all_Target;
  TH1D * DP_P_reco_all_Target;
  TH1D * DP_Pt_reco_all_Target;
  TH1D * Ph_fromTarget_Pt_reco_all_Target;
  TH1D * Ph_fromPions_Pt_reco_all_Target;
  TH1D * Ph_fromEtas_Pt_reco_all_Target;
  TH1D * Ph_fromDalitz_Pt_reco_all_Target;

  // Target => zero
  vector<TH1*> fHistoList_dp_zero_Target;
  TH1D * DP_InvMassReco_zero_Target;
  TH1D * DP_OpeningAngleReco_zero_Target;
  TH1D * DP_Pdg_zero_Target;
  TH1D * DP_P_reco_zero_Target;
  TH1D * DP_Pt_reco_zero_Target;
  TH1D * Ph_fromTarget_Pt_reco_zero_Target;
  TH1D * Ph_fromPions_Pt_reco_zero_Target;
  TH1D * Ph_fromEtas_Pt_reco_zero_Target;
  TH1D * Ph_fromDalitz_Pt_reco_zero_Target;

  // Target => one
  vector<TH1*> fHistoList_dp_one_Target;
  TH1D * DP_InvMassReco_one_Target;
  TH1D * DP_OpeningAngleReco_one_Target;
  TH1D * DP_Pdg_one_Target;
  TH1D * DP_P_reco_one_Target;
  TH1D * DP_Pt_reco_one_Target;
  TH1D * Ph_fromTarget_Pt_reco_one_Target;
  TH1D * Ph_fromPions_Pt_reco_one_Target;
  TH1D * Ph_fromEtas_Pt_reco_one_Target;
  TH1D * Ph_fromDalitz_Pt_reco_one_Target;

  // Target => two
  vector<TH1*> fHistoList_dp_two_Target;
  TH1D * DP_InvMassReco_two_Target;
  TH1D * DP_OpeningAngleReco_two_Target;
  TH1D * DP_Pdg_two_Target;
  TH1D * DP_P_reco_two_Target;
  TH1D * DP_Pt_reco_two_Target;
  TH1D * Ph_fromTarget_Pt_reco_two_Target;
  TH1D * Ph_fromPions_Pt_reco_two_Target;
  TH1D * Ph_fromEtas_Pt_reco_two_Target;
  TH1D * Ph_fromDalitz_Pt_reco_two_Target;
  TH1D * Ph_fromXi_Pt_reco_two_Target;
  TH1D * Ph_fromOther_Pt_reco_two_Target;
  TH1D * Ph_twoFromTarget_Pt_reco_two_Target;
  TH1D * Ph_fromCombinatorial_Pt_reco_two_Target;
  TH1D * Ph_fromConversion_Pt_reco_two_Target;
  
  TH1D * twoFromTarget_PDG_two_Target;
  TH1D * fromCombinatorial_PDG_two_Target;
  TH1D * CombinatorialGrMotherPdg_two_Target;
  TH1D * CombinatorialMotherPdg_two_Target;
  TH1D * Electrons_two_Target;
  TH1D * Pions_two_Target;
  TH1D * PionElectron_two_Target;
  TH1D * elsePionOrElectron_two_Target;
  TH1D * DalitzAndConversion_Pt_two_Target;
  TH1D * DoubleConversion_Pt_two_Target;
  TH1D * fromFireball_P_two_Target;
  TH1D * twoFromTarget_P_two_Target;
  TH1D * fromCombinatorial_electron_P_two_Target;
  TH1D * fromCombinatorial_NOTelectron_P_two_Target;


  // Target => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Target;
  TH1D * DP_InvMassReco_onetwo_Target;
  TH1D * DP_OpeningAngleReco_onetwo_Target;
  TH1D * DP_Pdg_onetwo_Target;
  TH1D * DP_P_reco_onetwo_Target;
  TH1D * DP_Pt_reco_onetwo_Target;
  TH1D * Ph_fromTarget_Pt_reco_onetwo_Target;
  TH1D * Ph_fromPions_Pt_reco_onetwo_Target;
  TH1D * Ph_fromEtas_Pt_reco_onetwo_Target;
  TH1D * Ph_fromDalitz_Pt_reco_onetwo_Target;
  TH1D * Ph_fromXi_Pt_reco_onetwo_Target;
  TH1D * Ph_fromOther_Pt_reco_onetwo_Target;
  TH1D * Ph_twoFromTarget_Pt_reco_onetwo_Target;
  TH1D * Ph_fromCombinatorial_Pt_reco_onetwo_Target;
  TH1D * Ph_fromConversion_Pt_reco_onetwo_Target;

  TH1D * twoFromTarget_PDG_onetwo_Target;
  TH1D * fromCombinatorial_PDG_onetwo_Target;
  TH1D * CombinatorialGrMotherPdg_onetwo_Target;
  TH1D * CombinatorialMotherPdg_onetwo_Target;
  TH1D * Electrons_onetwo_Target;
  TH1D * Pions_onetwo_Target;
  TH1D * PionElectron_onetwo_Target;
  TH1D * elsePionOrElectron_onetwo_Target;
  TH1D * DalitzAndConversion_Pt_onetwo_Target;
  TH1D * DoubleConversion_Pt_onetwo_Target;
  TH1D * fromFireball_P_onetwo_Target;
  TH1D * twoFromTarget_P_onetwo_Target;
  TH1D * fromCombinatorial_electron_P_onetwo_Target;
  TH1D * fromCombinatorial_NOTelectron_P_onetwo_Target;




  // Outside => all
  vector<TH1*> fHistoList_dp_all_Outside;
  TH1D * DP_InvMassReco_all_Outside;
  TH1D * DP_OpeningAngleReco_all_Outside;
  TH1D * DP_Pdg_all_Outside;
  TH1D * DP_P_reco_all_Outside;
  TH1D * DP_Pt_reco_all_Outside;  
  TH1D * Ph_fromTarget_Pt_reco_all_Outside;
  TH1D * Ph_fromPions_Pt_reco_all_Outside;
  TH1D * Ph_fromEtas_Pt_reco_all_Outside;
  TH1D * Ph_fromDalitz_Pt_reco_all_Outside;

  // Outside => zero
  vector<TH1*> fHistoList_dp_zero_Outside;
  TH1D * DP_InvMassReco_zero_Outside;
  TH1D * DP_OpeningAngleReco_zero_Outside;
  TH1D * DP_Pdg_zero_Outside;
  TH1D * DP_P_reco_zero_Outside;
  TH1D * DP_Pt_reco_zero_Outside;
  TH1D * Ph_fromTarget_Pt_reco_zero_Outside;
  TH1D * Ph_fromPions_Pt_reco_zero_Outside;
  TH1D * Ph_fromEtas_Pt_reco_zero_Outside;
  TH1D * Ph_fromDalitz_Pt_reco_zero_Outside;

  // Outside => one
  vector<TH1*> fHistoList_dp_one_Outside;
  TH1D * DP_InvMassReco_one_Outside;
  TH1D * DP_OpeningAngleReco_one_Outside;
  TH1D * DP_Pdg_one_Outside;
  TH1D * DP_P_reco_one_Outside;
  TH1D * DP_Pt_reco_one_Outside;
  TH1D * Ph_fromTarget_Pt_reco_one_Outside;
  TH1D * Ph_fromPions_Pt_reco_one_Outside;
  TH1D * Ph_fromEtas_Pt_reco_one_Outside;
  TH1D * Ph_fromDalitz_Pt_reco_one_Outside;

  // Outside => two
  vector<TH1*> fHistoList_dp_two_Outside;
  TH1D * DP_InvMassReco_two_Outside;
  TH1D * DP_OpeningAngleReco_two_Outside;
  TH1D * DP_Pdg_two_Outside;
  TH1D * DP_P_reco_two_Outside;
  TH1D * DP_Pt_reco_two_Outside;
  TH1D * Ph_fromTarget_Pt_reco_two_Outside;
  TH1D * Ph_fromPions_Pt_reco_two_Outside;
  TH1D * Ph_fromEtas_Pt_reco_two_Outside;
  TH1D * Ph_fromDalitz_Pt_reco_two_Outside;
  TH1D * Ph_fromXi_Pt_reco_two_Outside;
  TH1D * Ph_fromOther_Pt_reco_two_Outside;
  TH1D * Ph_twoFromTarget_Pt_reco_two_Outside;
  TH1D * Ph_fromCombinatorial_Pt_reco_two_Outside;
  TH1D * Ph_fromConversion_Pt_reco_two_Outside;  
  TH1D * twoFromTarget_PDG_two_Outside;
  TH1D * fromCombinatorial_PDG_two_Outside;
  TH1D * CombinatorialGrMotherPdg_two_Outside;
  TH1D * CombinatorialMotherPdg_two_Outside;
  TH1D * Electrons_two_Outside;
  TH1D * Pions_two_Outside;
  TH1D * PionElectron_two_Outside;
  TH1D * elsePionOrElectron_two_Outside;
  TH1D * DalitzAndConversion_Pt_two_Outside;
  TH1D * DoubleConversion_Pt_two_Outside;
  TH1D * fromFireball_P_two_Outside;
  TH1D * twoFromTarget_P_two_Outside;
  TH1D * fromCombinatorial_electron_P_two_Outside;
  TH1D * fromCombinatorial_NOTelectron_P_two_Outside;

  // Outside => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Outside;
  TH1D * DP_InvMassReco_onetwo_Outside;
  TH1D * DP_OpeningAngleReco_onetwo_Outside;
  TH1D * DP_Pdg_onetwo_Outside;
  TH1D * DP_P_reco_onetwo_Outside;
  TH1D * DP_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromTarget_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromPions_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromEtas_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromDalitz_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromXi_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromOther_Pt_reco_onetwo_Outside;
  TH1D * Ph_twoFromTarget_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromCombinatorial_Pt_reco_onetwo_Outside;
  TH1D * Ph_fromConversion_Pt_reco_onetwo_Outside;  
  TH1D * twoFromTarget_PDG_onetwo_Outside;
  TH1D * fromCombinatorial_PDG_onetwo_Outside;
  TH1D * CombinatorialGrMotherPdg_onetwo_Outside;
  TH1D * CombinatorialMotherPdg_onetwo_Outside;
  TH1D * Electrons_onetwo_Outside;
  TH1D * Pions_onetwo_Outside;
  TH1D * PionElectron_onetwo_Outside;
  TH1D * elsePionOrElectron_onetwo_Outside;
  TH1D * DalitzAndConversion_Pt_onetwo_Outside;
  TH1D * DoubleConversion_Pt_onetwo_Outside;
  TH1D * fromFireball_P_onetwo_Outside;
  TH1D * twoFromTarget_P_onetwo_Outside;
  TH1D * fromCombinatorial_electron_P_onetwo_Outside;
  TH1D * fromCombinatorial_NOTelectron_P_onetwo_Outside;




  // Both => all
  vector<TH1*> fHistoList_dp_all_Both;
  TH1D * DP_InvMassReco_all_Both;
  TH1D * DP_OpeningAngleReco_all_Both;
  TH1D * DP_Pdg_all_Both;
  TH1D * DP_P_reco_all_Both;
  TH1D * DP_Pt_reco_all_Both;
  TH1D * Ph_fromTarget_Pt_reco_all_Both;
  TH1D * Ph_fromPions_Pt_reco_all_Both;
  TH1D * Ph_fromEtas_Pt_reco_all_Both;
  TH1D * Ph_fromDalitz_Pt_reco_all_Both;
  TH2D * Ph_pt_vs_rap_est_all_Both;
  
  // Both => zero
  vector<TH1*> fHistoList_dp_zero_Both;
  TH1D * DP_InvMassReco_zero_Both;
  TH1D * DP_OpeningAngleReco_zero_Both;
  TH1D * DP_Pdg_zero_Both;
  TH1D * DP_P_reco_zero_Both;
  TH1D * DP_Pt_reco_zero_Both;
  TH1D * Ph_fromTarget_Pt_reco_zero_Both;
  TH1D * Ph_fromPions_Pt_reco_zero_Both;
  TH1D * Ph_fromEtas_Pt_reco_zero_Both;
  TH1D * Ph_fromDalitz_Pt_reco_zero_Both;
  TH2D * Ph_pt_vs_rap_est_zero_Both;

  // Both => one
  vector<TH1*> fHistoList_dp_one_Both;
  TH1D * DP_InvMassReco_one_Both;
  TH1D * DP_OpeningAngleReco_one_Both;
  TH1D * DP_Pdg_one_Both;
  TH1D * DP_P_reco_one_Both;
  TH1D * DP_Pt_reco_one_Both;
  TH1D * Ph_fromTarget_Pt_reco_one_Both;
  TH1D * Ph_fromPions_Pt_reco_one_Both;
  TH1D * Ph_fromEtas_Pt_reco_one_Both;
  TH1D * Ph_fromDalitz_Pt_reco_one_Both;
  TH2D * Ph_pt_vs_rap_est_one_Both;

  // Both => two
  vector<TH1*> fHistoList_dp_two_Both;
  TH1D * DP_InvMassReco_two_Both;
  TH1D * DP_OpeningAngleReco_two_Both;
  TH1D * DP_Pdg_two_Both;
  TH1D * DP_P_reco_two_Both;
  TH1D * DP_Pt_reco_two_Both;
  TH1D * Ph_fromTarget_Pt_reco_two_Both;
  TH1D * Ph_fromPions_Pt_reco_two_Both;
  TH1D * Ph_fromEtas_Pt_reco_two_Both;
  TH1D * Ph_fromDalitz_Pt_reco_two_Both;
  TH1D * Ph_twoFromTarget_Pt_reco_two_Both;
  TH1D * Ph_fromCombinatorial_Pt_reco_two_Both;
  TH1D * Ph_fromConversion_Pt_reco_two_Both;
  TH2D * Ph_pt_vs_rap_est_two_Both;

  // Both => onetwo
  vector<TH1*> fHistoList_dp_onetwo_Both;
  TH1D * DP_InvMassReco_onetwo_Both;
  TH1D * DP_OpeningAngleReco_onetwo_Both;
  TH1D * DP_Pdg_onetwo_Both;
  TH1D * DP_P_reco_onetwo_Both;
  TH1D * DP_Pt_reco_onetwo_Both;
  TH1D * Ph_fromTarget_Pt_reco_onetwo_Both;
  TH1D * Ph_fromPions_Pt_reco_onetwo_Both;
  TH1D * Ph_fromEtas_Pt_reco_onetwo_Both;
  TH1D * Ph_fromDalitz_Pt_reco_onetwo_Both;
  TH1D * Ph_twoFromTarget_Pt_reco_onetwo_Both;
  TH1D * Ph_fromCombinatorial_Pt_reco_onetwo_Both;
  TH1D * Ph_fromConversion_Pt_reco_onetwo_Both;
  TH2D * Ph_pt_vs_rap_est_onetwo_Both;



  // EMT DP target
  TH1D * DP_EMT_Pt_all_Target;
  TH1D * DP_EMT_Pt_zero_Target;
  TH1D * DP_EMT_Pt_one_Target;
  TH1D * DP_EMT_Pt_two_Target;
  TH1D * DP_EMT_Pt_onetwo_Target;

  // EMT DP Outside
  TH1D * DP_EMT_Pt_all_Outside;
  TH1D * DP_EMT_Pt_zero_Outside;
  TH1D * DP_EMT_Pt_one_Outside;
  TH1D * DP_EMT_Pt_two_Outside;
  TH1D * DP_EMT_Pt_onetwo_Outside;

  // EMT DP Both
  TH1D * DP_EMT_Pt_all_Both;
  TH1D * DP_EMT_Pt_zero_Both;
  TH1D * DP_EMT_Pt_one_Both;
  TH1D * DP_EMT_Pt_two_Both;
  TH1D * DP_EMT_Pt_onetwo_Both;

  /////////////////// DP_LK_EMT Target
  TH1D * DP_LK_EMT_neg_Pt_all_Target;
  TH1D * DP_LK_EMT_neg_Pt_zero_Target;
  TH1D * DP_LK_EMT_neg_Pt_one_Target;
  TH1D * DP_LK_EMT_neg_Pt_two_Target;
  TH1D * DP_LK_EMT_neg_Pt_onetwo_Target;

  TH1D * DP_LK_EMT_pos_Pt_all_Target;
  TH1D * DP_LK_EMT_pos_Pt_zero_Target;
  TH1D * DP_LK_EMT_pos_Pt_one_Target;
  TH1D * DP_LK_EMT_pos_Pt_two_Target;
  TH1D * DP_LK_EMT_pos_Pt_onetwo_Target;

  TH1D * DP_LK_EMT_Pt_all_Target;
  TH1D * DP_LK_EMT_Pt_zero_Target;
  TH1D * DP_LK_EMT_Pt_one_Target;
  TH1D * DP_LK_EMT_Pt_two_Target;
  TH1D * DP_LK_EMT_Pt_onetwo_Target;
  /////////////////////////////////////

  /////////////////// DP_LK_EMT Outside
  TH1D * DP_LK_EMT_neg_Pt_all_Outside;
  TH1D * DP_LK_EMT_neg_Pt_zero_Outside;
  TH1D * DP_LK_EMT_neg_Pt_one_Outside;
  TH1D * DP_LK_EMT_neg_Pt_two_Outside;
  TH1D * DP_LK_EMT_neg_Pt_onetwo_Outside;

  TH1D * DP_LK_EMT_pos_Pt_all_Outside;
  TH1D * DP_LK_EMT_pos_Pt_zero_Outside;
  TH1D * DP_LK_EMT_pos_Pt_one_Outside;
  TH1D * DP_LK_EMT_pos_Pt_two_Outside;
  TH1D * DP_LK_EMT_pos_Pt_onetwo_Outside;

  TH1D * DP_LK_EMT_Pt_all_Outside;
  TH1D * DP_LK_EMT_Pt_zero_Outside;
  TH1D * DP_LK_EMT_Pt_one_Outside;
  TH1D * DP_LK_EMT_Pt_two_Outside;
  TH1D * DP_LK_EMT_Pt_onetwo_Outside;
  /////////////////////////////////////





  //***** brief Copy constructor.
  CbmKresConversionPhotons(const CbmKresConversionPhotons&);

  //***** brief Assignment operator.
  CbmKresConversionPhotons operator=(const CbmKresConversionPhotons&);


  ClassDef(CbmKresConversionPhotons,1)
};

#endif

