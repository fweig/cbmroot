#ifndef CBM_KRES_CONVERSION_MANUAL_mbias_4
#define CBM_KRES_CONVERSION_MANUAL_mbias_4

#include <TClonesArray.h>
#include "TH2D.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmRichRing.h"
#include "CbmKFVertex.h"
#include "FairMCEventHeader.h"

#include "CbmKresConversionBG.h"
class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresConversionManualmbias4
{

public:
  //***** brief Standard constructor.
  CbmKresConversionManualmbias4();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionManualmbias4();


  void Init();
  void Finish();
  void InitHistograms();

  void Exec(int fEventNumMan, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);

  void SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);

  int FindInRich(int richInd, int stsMcTrackId);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  void FindGammasTarget(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus, 
                        vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                        vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus, 
                        std::vector<int> Rings_minus, std::vector<int> Rings_plus, 
                        std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus, 
                        vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus);

  void FindGammasOutside(int EventNumMan, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus_Outside_mbias4, vector<CbmMCTrack*> MCtracks_plus_Outside_mbias4,
                        vector<CbmStsTrack*> StsTrack_minus_Outside_mbias4, vector<CbmStsTrack*> StsTrack_plus_Outside_mbias4,
                        std::vector<int> Rings_minus_Outside_mbias4, std::vector<int> Rings_plus_Outside_mbias4,
                        std::vector<int> stsIndex_minus_Outside_mbias4, std::vector<int> stsIndex_plus_Outside_mbias4, 
                        vector<CbmRichRing*> richRing_minus_Outside_mbias4, vector<CbmRichRing*> richRing_plus_Outside_mbias4);

  void FindGammasBoth();

  void FindPi0(TString mod, TString position, vector< vector<TVector3> > Gammas, vector< vector<int> > StsIndex, vector< vector<CbmMCTrack*> > GammasMC, 
              TH1D* Pi0InvMassReco, TH2D* Pi0_pt_vs_rap, TH2D* Pi0_pt_vs_rap_est, TH2D* MultiplicityGamma, TH2D* MultiplicityChargedParticles, vector<TH1*> BGCases);

  void Mixing_Target();

  void Mixing_Outside();

  void Mixing_Both();

  double CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  double CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

private:

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
  FairMCEventHeader *fArrayCentrality; 

  CbmVertex * fPrimVertex;
  CbmKFVertex fKFVertex;

  CbmRichRingFitterEllipseTau* fTauFit;

  CbmKresConversionBG *fAnaBG;


    // charged tracks from outside
  vector<CbmStsTrack*> VStsTrack_minus_Outside_mbias4;
  vector<CbmMCTrack*> VMCtracks_minus_Outside_mbias4;
  std::vector<int> VRings_minus_Outside_mbias4;
  std::vector<int> VStsIndex_minus_Outside_mbias4;
  vector<CbmRichRing*> VRichRing_minus_Outside_mbias4;

  vector<CbmStsTrack*> VStsTrack_plus_Outside_mbias4;
  vector<CbmMCTrack*> VMCtracks_plus_Outside_mbias4;
  std::vector<int> VRings_plus_Outside_mbias4;
  std::vector<int> VStsIndex_plus_Outside_mbias4;
  vector<CbmRichRing*> VRichRing_plus_Outside_mbias4;


  // charged tracks from the Target_mbias4  
  vector<CbmMCTrack*> VMCtracks_minus_Target_mbias4;
  vector<CbmStsTrack*> VStsTrack_minus_Target_mbias4;
  vector<TVector3> VMomenta_minus_Target_mbias4;
  std::vector<int> VRings_minus_Target_mbias4;
  std::vector<int> VStsIndex_minus_Target_mbias4;
  vector<CbmRichRing*> VRichRing_minus_Target_mbias4;
  
  vector<CbmMCTrack*> VMCtracks_plus_Target_mbias4;
  vector<CbmStsTrack*> VStsTrack_plus_Target_mbias4;
  vector<TVector3> VMomenta_plus_Target_mbias4;
  std::vector<int> VRings_plus_Target_mbias4;
  std::vector<int> VStsIndex_plus_Target_mbias4;
  vector<CbmRichRing*> VRichRing_plus_Target_mbias4;


  vector<TVector3> frefmomenta;
  std::vector<int> frefId;
  std::vector<CbmMCTrack*> fMCtracks;


  // EMT Target_mbias4 Target_mbias4
  std::vector<int>                      EMT_man_Event_Target_mbias4;
  std::vector< std::vector<TVector3> >  EMT_man_pair_momenta_Target_mbias4;
  std::vector<int>                      EMT_man_NofRings_Target_mbias4;

  // EMT Target_mbias4 Outside_mbias4
  std::vector<int>                      EMT_man_Event_Outside_mbias4;
  std::vector< std::vector<TVector3> >  EMT_man_pair_momenta_Outside_mbias4;
  std::vector<int>                      EMT_man_NofRings_Outside_mbias4;

  // EMT Target_mbias4 Both_mbias4
  std::vector<int>                      EMT_man_Event_Both_mbias4;
  std::vector< std::vector<TVector3> >  EMT_man_pair_momenta_Both_mbias4;
  std::vector<int>                      EMT_man_NofRings_Both_mbias4;

  // combined gammas from the Target_mbias4
  std::vector< std::vector<TVector3> > Gammas_all_Target_mbias4;  
  std::vector< std::vector<TVector3> > Gammas_zero_Target_mbias4;
  std::vector< std::vector<TVector3> > Gammas_one_Target_mbias4;
  std::vector< std::vector<TVector3> > Gammas_two_Target_mbias4;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Target_mbias4;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Target_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Target_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Target_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Target_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Target_mbias4;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Target_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Target_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Target_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Target_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Target_mbias4;


  // combined gammas from Outside_mbias4
  std::vector< std::vector<TVector3> > Gammas_all_Outside_mbias4;
  std::vector< std::vector<TVector3> > Gammas_zero_Outside_mbias4;
  std::vector< std::vector<TVector3> > Gammas_one_Outside_mbias4;
  std::vector< std::vector<TVector3> > Gammas_two_Outside_mbias4;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Outside_mbias4;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Outside_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Outside_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Outside_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Outside_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Outside_mbias4;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Outside_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Outside_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Outside_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Outside_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Outside_mbias4;


  // combined gammas from the Target_mbias4 and Outside_mbias4 together
  std::vector< std::vector<TVector3> > Gammas_all_Both_mbias4;
  std::vector< std::vector<TVector3> > Gammas_zero_Both_mbias4;
  std::vector< std::vector<TVector3> > Gammas_one_Both_mbias4;
  std::vector< std::vector<TVector3> > Gammas_two_Both_mbias4;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Both_mbias4;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Both_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_zero_Both_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_one_Both_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Both_mbias4;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Both_mbias4;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Both_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_zero_Both_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_one_Both_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Both_mbias4;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Both_mbias4;
 
  


  // histograms
  ///////   histograms to check Cuts => Both_mbias4
  vector<TH1*> fHistoList_man_cuts_Both_mbias4;
  TH2D * InvMass_vs_OA_candidates_Both_mbias4;
  TH2D * InvMass_vs_OA_fromPi0_Both_mbias4;
  TH1D * GammasInvMass_candidates_Both_mbias4;
  TH1D * GammasOA_candidates_Both_mbias4;
  TH1D * GammasInvMass_fromPi0_Both_mbias4;
  TH1D * GammasOA_fromPi0_Both_mbias4;
  TH1D * PlaneAngles_last_candidates_Both_mbias4;
  TH1D * PlaneAngles_last_fromPi0_Both_mbias4;
  TH1D * PlaneAngles_first_candidates_Both_mbias4;
  TH1D * PlaneAngles_first_fromPi0_Both_mbias4;




  ///////   histograms to check Cuts => Target_mbias4
  vector<TH1*> fHistoList_man_cuts_Target_mbias4;
  TH2D * InvMass_vs_OA_candidates_Target_mbias4;
  TH2D * InvMass_vs_OA_fromPi0_Target_mbias4;
  TH1D * GammasInvMass_candidates_Target_mbias4;
  TH1D * GammasOA_candidates_Target_mbias4;
  TH1D * GammasInvMass_fromPi0_Target_mbias4;
  TH1D * GammasOA_fromPi0_Target_mbias4;
  TH1D * PlaneAngles_last_candidates_Target_mbias4;
  TH1D * PlaneAngles_last_fromPi0_Target_mbias4;
  TH1D * PlaneAngles_first_candidates_Target_mbias4;
  TH1D * PlaneAngles_first_fromPi0_Target_mbias4;




  ///////   histograms to check Cuts => Outside_mbias4
  vector<TH1*> fHistoList_man_cuts_Outside_mbias4;
  TH2D * InvMass_vs_OA_candidates_Outside_mbias4;
  TH2D * InvMass_vs_OA_fromPi0_Outside_mbias4;
  TH1D * GammasInvMass_candidates_Outside_mbias4;
  TH1D * GammasOA_candidates_Outside_mbias4;
  TH1D * GammasInvMass_fromPi0_Outside_mbias4;
  TH1D * GammasOA_fromPi0_Outside_mbias4;
  TH1D * PlaneAngles_last_candidates_Outside_mbias4;
  TH1D * PlaneAngles_last_fromPi0_Outside_mbias4;
  TH1D * PlaneAngles_first_candidates_Outside_mbias4;
  TH1D * PlaneAngles_first_fromPi0_Outside_mbias4;



  ///////   Target_mbias4
  vector<TH1*> fHistoList_man_all_Target_mbias4;
  TH1D * GammaInvMassReco_all_Target_mbias4;
  TH1D * GammaOpeningAngleReco_all_Target_mbias4;
  TH1D * Pdg_all_Target_mbias4;
  TH1D * P_reco_all_Target_mbias4;
  TH1D * Pt_reco_all_Target_mbias4;
  TH1D * Pi0InvMassReco_all_Target_mbias4;
  TH1D * EMT_InvMass_all_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_all_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_est_all_Target_mbias4;

  vector<TH1*> fHistoList_man_zero_Target_mbias4;
  TH1D * GammaInvMassReco_zero_Target_mbias4;
  TH1D * GammaOpeningAngleReco_zero_Target_mbias4;
  TH1D * Pdg_zero_Target_mbias4;
  TH1D * P_reco_zero_Target_mbias4;
  TH1D * Pt_reco_zero_Target_mbias4;
  TH1D * Pi0InvMassReco_zero_Target_mbias4;
  TH1D * EMT_InvMass_zero_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_zero_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_est_zero_Target_mbias4;

  vector<TH1*> fHistoList_man_one_Target_mbias4;
  TH1D * GammaInvMassReco_one_Target_mbias4;
  TH1D * GammaOpeningAngleReco_one_Target_mbias4;
  TH1D * Pdg_one_Target_mbias4;
  TH1D * P_reco_one_Target_mbias4;
  TH1D * Pt_reco_one_Target_mbias4;
  TH1D * Pi0InvMassReco_one_Target_mbias4;
  TH1D * EMT_InvMass_one_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_one_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_est_one_Target_mbias4;

  vector<TH1*> fHistoList_man_two_Target_mbias4;
  TH1D * GammaInvMassReco_two_Target_mbias4;
  TH1D * GammaOpeningAngleReco_two_Target_mbias4;
  TH1D * Pdg_two_Target_mbias4;
  TH1D * P_reco_two_Target_mbias4;
  TH1D * Pt_reco_two_Target_mbias4;
  TH1D * Pi0InvMassReco_two_Target_mbias4;
  TH1D * EMT_InvMass_two_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_two_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_est_two_Target_mbias4;

  vector<TH1*> fHistoList_man_onetwo_Target_mbias4;
  TH1D * GammaInvMassReco_onetwo_Target_mbias4;
  TH1D * GammaOpeningAngleReco_onetwo_Target_mbias4;
  TH1D * Pdg_onetwo_Target_mbias4;
  TH1D * P_reco_onetwo_Target_mbias4;
  TH1D * Pt_reco_onetwo_Target_mbias4;
  TH1D * Pi0InvMassReco_onetwo_Target_mbias4;
  TH1D * EMT_InvMass_onetwo_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_onetwo_Target_mbias4;
  TH2D * Pi0_pt_vs_rap_est_onetwo_Target_mbias4;






  ///////   Outside_mbias4
  vector<TH1*> fHistoList_man_all_Outside_mbias4;
  TH1D * GammaInvMassReco_all_Outside_mbias4;
  TH1D * GammaOpeningAngleReco_all_Outside_mbias4;
  TH1D * Pdg_all_Outside_mbias4;
  TH1D * P_reco_all_Outside_mbias4;
  TH1D * Pt_reco_all_Outside_mbias4;
  TH1D * Pi0InvMassReco_all_Outside_mbias4;
  TH1D * EMT_InvMass_all_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_all_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_est_all_Outside_mbias4;

  vector<TH1*> fHistoList_man_zero_Outside_mbias4;
  TH1D * GammaInvMassReco_zero_Outside_mbias4;
  TH1D * GammaOpeningAngleReco_zero_Outside_mbias4;
  TH1D * Pdg_zero_Outside_mbias4;
  TH1D * P_reco_zero_Outside_mbias4;
  TH1D * Pt_reco_zero_Outside_mbias4;
  TH1D * Pi0InvMassReco_zero_Outside_mbias4;
  TH1D * EMT_InvMass_zero_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_zero_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_est_zero_Outside_mbias4;

  vector<TH1*> fHistoList_man_one_Outside_mbias4;
  TH1D * GammaInvMassReco_one_Outside_mbias4;
  TH1D * GammaOpeningAngleReco_one_Outside_mbias4;
  TH1D * Pdg_one_Outside_mbias4;
  TH1D * P_reco_one_Outside_mbias4;
  TH1D * Pt_reco_one_Outside_mbias4;
  TH1D * Pi0InvMassReco_one_Outside_mbias4;
  TH1D * EMT_InvMass_one_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_one_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_est_one_Outside_mbias4;

  vector<TH1*> fHistoList_man_two_Outside_mbias4;
  TH1D * GammaInvMassReco_two_Outside_mbias4;
  TH1D * GammaOpeningAngleReco_two_Outside_mbias4;
  TH1D * Pdg_two_Outside_mbias4;
  TH1D * P_reco_two_Outside_mbias4;
  TH1D * Pt_reco_two_Outside_mbias4;
  TH1D * Pi0InvMassReco_two_Outside_mbias4;
  TH1D * EMT_InvMass_two_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_two_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_est_two_Outside_mbias4;

  vector<TH1*> fHistoList_man_onetwo_Outside_mbias4;
  TH1D * GammaInvMassReco_onetwo_Outside_mbias4;
  TH1D * GammaOpeningAngleReco_onetwo_Outside_mbias4;
  TH1D * Pdg_onetwo_Outside_mbias4;
  TH1D * P_reco_onetwo_Outside_mbias4;
  TH1D * Pt_reco_onetwo_Outside_mbias4;
  TH1D * Pi0InvMassReco_onetwo_Outside_mbias4;
  TH1D * EMT_InvMass_onetwo_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_onetwo_Outside_mbias4;
  TH2D * Pi0_pt_vs_rap_est_onetwo_Outside_mbias4;






  ///////   Both_mbias4
  vector<TH1*> fHistoList_man_all_Both_mbias4;
  TH1D * GammaInvMassReco_all_Both_mbias4;
  TH1D * GammaOpeningAngleReco_all_Both_mbias4;
  TH1D * Pdg_all_Both_mbias4;
  TH1D * P_reco_all_Both_mbias4;
  TH1D * Pt_reco_all_Both_mbias4;
  TH1D * Pi0InvMassReco_all_Both_mbias4;
  TH1D * EMT_InvMass_all_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_all_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_est_all_Both_mbias4;

  vector<TH1*> fHistoList_man_zero_Both_mbias4;
  TH1D * GammaInvMassReco_zero_Both_mbias4;
  TH1D * GammaOpeningAngleReco_zero_Both_mbias4;
  TH1D * Pdg_zero_Both_mbias4;
  TH1D * P_reco_zero_Both_mbias4;
  TH1D * Pt_reco_zero_Both_mbias4;
  TH1D * Pi0InvMassReco_zero_Both_mbias4;
  TH1D * EMT_InvMass_zero_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_zero_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_est_zero_Both_mbias4;

  vector<TH1*> fHistoList_man_one_Both_mbias4;
  TH1D * GammaInvMassReco_one_Both_mbias4;
  TH1D * GammaOpeningAngleReco_one_Both_mbias4;
  TH1D * Pdg_one_Both_mbias4;
  TH1D * P_reco_one_Both_mbias4;
  TH1D * Pt_reco_one_Both_mbias4;
  TH1D * Pi0InvMassReco_one_Both_mbias4;
  TH1D * EMT_InvMass_one_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_one_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_est_one_Both_mbias4;

  vector<TH1*> fHistoList_man_two_Both_mbias4;
  TH1D * GammaInvMassReco_two_Both_mbias4;
  TH1D * GammaOpeningAngleReco_two_Both_mbias4;
  TH1D * Pdg_two_Both_mbias4;
  TH1D * P_reco_two_Both_mbias4;
  TH1D * Pt_reco_two_Both_mbias4;
  TH1D * Pi0InvMassReco_two_Both_mbias4;
  TH1D * EMT_InvMass_two_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_two_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_est_two_Both_mbias4;

  vector<TH1*> fHistoList_man_onetwo_Both_mbias4;
  TH1D * GammaInvMassReco_onetwo_Both_mbias4;
  TH1D * GammaOpeningAngleReco_onetwo_Both_mbias4;
  TH1D * Pdg_onetwo_Both_mbias4;
  TH1D * P_reco_onetwo_Both_mbias4;
  TH1D * Pt_reco_onetwo_Both_mbias4;
  TH1D * Pi0InvMassReco_onetwo_Both_mbias4;
  TH1D * EMT_InvMass_onetwo_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_onetwo_Both_mbias4;
  TH2D * Pi0_pt_vs_rap_est_onetwo_Both_mbias4;





  // Both_mbias4 additional histograms
  vector<TH1*> fHistoList_man_Both_mbias4;
  TH2D * Pdg_vs_Distance_mbias4;
  TH2D * P_vs_Distance_mbias4;




  // multiplicity Target_mbias4
  vector<TH1*> fHistoList_multiplicity_man_Target_mbias4;
  TH2D * MultiplicityGamma_all_Target_mbias4;
  TH2D * MultiplicityGamma_zero_Target_mbias4;
  TH2D * MultiplicityGamma_one_Target_mbias4;
  TH2D * MultiplicityGamma_two_Target_mbias4;
  TH2D * MultiplicityGamma_onetwo_Target_mbias4;
  TH2D * MultiplicityChargedParticles_all_Target_mbias4;
  TH2D * MultiplicityChargedParticles_zero_Target_mbias4;
  TH2D * MultiplicityChargedParticles_one_Target_mbias4;
  TH2D * MultiplicityChargedParticles_two_Target_mbias4;
  TH2D * MultiplicityChargedParticles_onetwo_Target_mbias4;

  // multiplicity Outside_mbias4
  vector<TH1*> fHistoList_multiplicity_man_Outside_mbias4;
  TH2D * MultiplicityGamma_all_Outside_mbias4;
  TH2D * MultiplicityGamma_zero_Outside_mbias4;
  TH2D * MultiplicityGamma_one_Outside_mbias4;
  TH2D * MultiplicityGamma_two_Outside_mbias4;
  TH2D * MultiplicityGamma_onetwo_Outside_mbias4;
  TH2D * MultiplicityChargedParticles_all_Outside_mbias4;
  TH2D * MultiplicityChargedParticles_zero_Outside_mbias4;
  TH2D * MultiplicityChargedParticles_one_Outside_mbias4;
  TH2D * MultiplicityChargedParticles_two_Outside_mbias4;
  TH2D * MultiplicityChargedParticles_onetwo_Outside_mbias4;

  // multiplicity Both_mbias4
  vector<TH1*> fHistoList_multiplicity_man_Both_mbias4;
  TH2D * MultiplicityGamma_all_Both_mbias4;
  TH2D * MultiplicityGamma_zero_Both_mbias4;
  TH2D * MultiplicityGamma_one_Both_mbias4;
  TH2D * MultiplicityGamma_two_Both_mbias4;
  TH2D * MultiplicityGamma_onetwo_Both_mbias4;
  TH2D * MultiplicityChargedParticles_all_Both_mbias4;
  TH2D * MultiplicityChargedParticles_zero_Both_mbias4;
  TH2D * MultiplicityChargedParticles_one_Both_mbias4;
  TH2D * MultiplicityChargedParticles_two_Both_mbias4;
  TH2D * MultiplicityChargedParticles_onetwo_Both_mbias4;


  //   rap_vs_Pt for "OneTwo" and "Both_mbias4"
  vector<TH1*> fHistoList_rap_vs_pt_InM_mbias4;
  TH1D * rap_vs_Pt_InM_1_mbias4;
  TH1D * rap_vs_Pt_InM_2_mbias4; 
  TH1D * rap_vs_Pt_InM_3_mbias4;
  TH1D * rap_vs_Pt_InM_4_mbias4;
  TH1D * rap_vs_Pt_InM_5_mbias4;
  TH1D * rap_vs_Pt_InM_6_mbias4;
  TH1D * rap_vs_Pt_InM_7_mbias4;
  TH1D * rap_vs_Pt_InM_8_mbias4;
  TH1D * rap_vs_Pt_InM_9_mbias4;
  TH1D * rap_vs_Pt_InM_10_mbias4;
  TH1D * rap_vs_Pt_InM_11_mbias4;
  TH1D * rap_vs_Pt_InM_12_mbias4;
  TH1D * rap_vs_Pt_InM_13_mbias4;
  TH1D * rap_vs_Pt_InM_14_mbias4;
  TH1D * rap_vs_Pt_InM_15_mbias4;
  TH1D * rap_vs_Pt_InM_16_mbias4;
  TH1D * rap_vs_Pt_InM_17_mbias4;
  TH1D * rap_vs_Pt_InM_18_mbias4;
  TH1D * rap_vs_Pt_InM_19_mbias4;
  TH1D * rap_vs_Pt_InM_20_mbias4;
  TH1D * rap_vs_Pt_InM_21_mbias4;
  TH1D * rap_vs_Pt_InM_22_mbias4;
  TH1D * rap_vs_Pt_InM_23_mbias4;
  TH1D * rap_vs_Pt_InM_24_mbias4;
  TH1D * rap_vs_Pt_InM_25_mbias4;
  TH1D * rap_vs_Pt_InM_26_mbias4;
  TH1D * rap_vs_Pt_InM_27_mbias4;
  TH1D * rap_vs_Pt_InM_28_mbias4;
  TH1D * rap_vs_Pt_InM_29_mbias4;
  TH1D * rap_vs_Pt_InM_30_mbias4;

  TH1D * rap_vs_Pt_InM_mixing_1_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_2_mbias4; 
  TH1D * rap_vs_Pt_InM_mixing_3_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_4_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_5_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_6_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_7_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_8_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_9_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_10_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_11_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_12_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_13_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_14_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_15_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_16_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_17_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_18_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_19_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_20_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_21_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_22_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_23_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_24_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_25_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_26_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_27_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_28_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_29_mbias4;
  TH1D * rap_vs_Pt_InM_mixing_30_mbias4;


  // BG cases Target_mbias4
  vector<TH1*> fHistoList_bg_InM_all_Target_mbias4;
  vector<TH1*> fHistoList_bg_InM_zero_Target_mbias4;
  vector<TH1*> fHistoList_bg_InM_one_Target_mbias4;
  vector<TH1*> fHistoList_bg_InM_two_Target_mbias4;
  vector<TH1*> fHistoList_bg_InM_onetwo_Target_mbias4;
  // BG cases Outside_mbias4
  vector<TH1*> fHistoList_bg_InM_all_Outside_mbias4;
  vector<TH1*> fHistoList_bg_InM_zero_Outside_mbias4;
  vector<TH1*> fHistoList_bg_InM_one_Outside_mbias4;
  vector<TH1*> fHistoList_bg_InM_two_Outside_mbias4;
  vector<TH1*> fHistoList_bg_InM_onetwo_Outside_mbias4;

  // BG cases Both_mbias4
  // all
  vector<TH1*> fHistoList_bg_InM_all_Both_mbias4;
  TH1D * BG1_InM_all_Both_mbias4;
  TH1D * BG2_InM_all_Both_mbias4;
  TH1D * BG3_InM_all_Both_mbias4;
  TH1D * BG4_InM_all_Both_mbias4;
  TH1D * BG5_InM_all_Both_mbias4;
  TH1D * BG6_InM_all_Both_mbias4;
  TH1D * BG7_InM_all_Both_mbias4;
  TH1D * BG8_InM_all_Both_mbias4;
  TH1D * BG9_InM_all_Both_mbias4;
  TH1D * BG10_InM_all_Both_mbias4;
  TH1D * PdgCase8_InM_all_Both_mbias4;
  TH1D * PdgCase8mothers_InM_all_Both_mbias4;
  TH1D * sameMIDcase8_InM_all_Both_mbias4;
  TH1D * sameGRIDcase8_InM_all_Both_mbias4;
  TH2D * Case1ZYPos_InM_all_Both_mbias4;
  TH1D * sameMIDcase8_mothedPDG_InM_all_Both_mbias4;
  TH1D * PdgCase8NonEComeFromTarget_mbias4_InM_all_Both_mbias4;
  TH1D * PdgCase8NonE_NOT_FromTarget_mbias4_InM_all_Both_mbias4;
  TH1D * PdgCase8motherNonE_InM_all_Both_mbias4;
  TH1D * Case8ElFromDalitz_InM_all_Both_mbias4;
  TH1D * Case8NonElFrom_pn_InM_all_Both_mbias4;
  TH1D * Case8NonElFrom_eta_InM_all_Both_mbias4;
  TH1D * Case8NonElFrom_kaon_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEPdg_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherPdg_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherIM_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEPdgFromTarget_mbias4_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4IM_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4P_InM_all_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4Pt_InM_all_Both_mbias4;
  // zero
  vector<TH1*> fHistoList_bg_InM_zero_Both_mbias4;
  TH1D * BG1_InM_zero_Both_mbias4;
  TH1D * BG2_InM_zero_Both_mbias4;
  TH1D * BG3_InM_zero_Both_mbias4;
  TH1D * BG4_InM_zero_Both_mbias4;
  TH1D * BG5_InM_zero_Both_mbias4;
  TH1D * BG6_InM_zero_Both_mbias4;
  TH1D * BG7_InM_zero_Both_mbias4;
  TH1D * BG8_InM_zero_Both_mbias4;
  TH1D * BG9_InM_zero_Both_mbias4;
  TH1D * BG10_InM_zero_Both_mbias4;
  TH1D * PdgCase8_InM_zero_Both_mbias4;
  TH1D * PdgCase8mothers_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8_InM_zero_Both_mbias4;
  TH1D * sameGRIDcase8_InM_zero_Both_mbias4;
  TH2D * Case1ZYPos_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8_mothedPDG_InM_zero_Both_mbias4;
  TH1D * PdgCase8NonEComeFromTarget_mbias4_InM_zero_Both_mbias4;
  TH1D * PdgCase8NonE_NOT_FromTarget_mbias4_InM_zero_Both_mbias4;
  TH1D * PdgCase8motherNonE_InM_zero_Both_mbias4;
  TH1D * Case8ElFromDalitz_InM_zero_Both_mbias4;
  TH1D * Case8NonElFrom_pn_InM_zero_Both_mbias4;
  TH1D * Case8NonElFrom_eta_InM_zero_Both_mbias4;
  TH1D * Case8NonElFrom_kaon_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEPdg_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherPdg_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherIM_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEPdgFromTarget_mbias4_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4IM_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4P_InM_zero_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4Pt_InM_zero_Both_mbias4;
  // one
  vector<TH1*> fHistoList_bg_InM_one_Both_mbias4;
  TH1D * BG1_InM_one_Both_mbias4;
  TH1D * BG2_InM_one_Both_mbias4;
  TH1D * BG3_InM_one_Both_mbias4;
  TH1D * BG4_InM_one_Both_mbias4;
  TH1D * BG5_InM_one_Both_mbias4;
  TH1D * BG6_InM_one_Both_mbias4;
  TH1D * BG7_InM_one_Both_mbias4;
  TH1D * BG8_InM_one_Both_mbias4;
  TH1D * BG9_InM_one_Both_mbias4;
  TH1D * BG10_InM_one_Both_mbias4;
  TH1D * PdgCase8_InM_one_Both_mbias4;
  TH1D * PdgCase8mothers_InM_one_Both_mbias4;
  TH1D * sameMIDcase8_InM_one_Both_mbias4;
  TH1D * sameGRIDcase8_InM_one_Both_mbias4;
  TH2D * Case1ZYPos_InM_one_Both_mbias4;
  TH1D * sameMIDcase8_mothedPDG_InM_one_Both_mbias4;
  TH1D * PdgCase8NonEComeFromTarget_mbias4_InM_one_Both_mbias4;
  TH1D * PdgCase8NonE_NOT_FromTarget_mbias4_InM_one_Both_mbias4;
  TH1D * PdgCase8motherNonE_InM_one_Both_mbias4;
  TH1D * Case8ElFromDalitz_InM_one_Both_mbias4;
  TH1D * Case8NonElFrom_pn_InM_one_Both_mbias4;
  TH1D * Case8NonElFrom_eta_InM_one_Both_mbias4;
  TH1D * Case8NonElFrom_kaon_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEPdg_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherPdg_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherIM_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEPdgFromTarget_mbias4_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4IM_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4P_InM_one_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4Pt_InM_one_Both_mbias4;
  // two
  vector<TH1*> fHistoList_bg_InM_two_Both_mbias4;
  TH1D * BG1_InM_two_Both_mbias4;
  TH1D * BG2_InM_two_Both_mbias4;
  TH1D * BG3_InM_two_Both_mbias4;
  TH1D * BG4_InM_two_Both_mbias4;
  TH1D * BG5_InM_two_Both_mbias4;
  TH1D * BG6_InM_two_Both_mbias4;
  TH1D * BG7_InM_two_Both_mbias4;
  TH1D * BG8_InM_two_Both_mbias4;
  TH1D * BG9_InM_two_Both_mbias4;
  TH1D * BG10_InM_two_Both_mbias4;
  TH1D * PdgCase8_InM_two_Both_mbias4;
  TH1D * PdgCase8mothers_InM_two_Both_mbias4;
  TH1D * sameMIDcase8_InM_two_Both_mbias4;
  TH1D * sameGRIDcase8_InM_two_Both_mbias4;
  TH2D * Case1ZYPos_InM_two_Both_mbias4;
  TH1D * sameMIDcase8_mothedPDG_InM_two_Both_mbias4;
  TH1D * PdgCase8NonEComeFromTarget_mbias4_InM_two_Both_mbias4;
  TH1D * PdgCase8NonE_NOT_FromTarget_mbias4_InM_two_Both_mbias4;
  TH1D * PdgCase8motherNonE_InM_two_Both_mbias4;
  TH1D * Case8ElFromDalitz_InM_two_Both_mbias4;
  TH1D * Case8NonElFrom_pn_InM_two_Both_mbias4;
  TH1D * Case8NonElFrom_eta_InM_two_Both_mbias4;
  TH1D * Case8NonElFrom_kaon_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEPdg_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherPdg_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherIM_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEPdgFromTarget_mbias4_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4IM_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4P_InM_two_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4Pt_InM_two_Both_mbias4;
  // onetwo
  vector<TH1*> fHistoList_bg_InM_onetwo_Both_mbias4;
  TH1D * BG1_InM_onetwo_Both_mbias4;
  TH1D * BG2_InM_onetwo_Both_mbias4;
  TH1D * BG3_InM_onetwo_Both_mbias4;
  TH1D * BG4_InM_onetwo_Both_mbias4;
  TH1D * BG5_InM_onetwo_Both_mbias4;
  TH1D * BG6_InM_onetwo_Both_mbias4;
  TH1D * BG7_InM_onetwo_Both_mbias4;
  TH1D * BG8_InM_onetwo_Both_mbias4;
  TH1D * BG9_InM_onetwo_Both_mbias4;
  TH1D * BG10_InM_onetwo_Both_mbias4;
  TH1D * PdgCase8_InM_onetwo_Both_mbias4;
  TH1D * PdgCase8mothers_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8_InM_onetwo_Both_mbias4;
  TH1D * sameGRIDcase8_InM_onetwo_Both_mbias4;
  TH2D * Case1ZYPos_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8_mothedPDG_InM_onetwo_Both_mbias4;
  TH1D * PdgCase8NonEComeFromTarget_mbias4_InM_onetwo_Both_mbias4;
  TH1D * PdgCase8NonE_NOT_FromTarget_mbias4_InM_onetwo_Both_mbias4;
  TH1D * PdgCase8motherNonE_InM_onetwo_Both_mbias4;
  TH1D * Case8ElFromDalitz_InM_onetwo_Both_mbias4;
  TH1D * Case8NonElFrom_pn_InM_onetwo_Both_mbias4;
  TH1D * Case8NonElFrom_eta_InM_onetwo_Both_mbias4;
  TH1D * Case8NonElFrom_kaon_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEPdg_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherPdg_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEMotherIM_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEPdgFromTarget_mbias4_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4IM_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4P_InM_onetwo_Both_mbias4;
  TH1D * sameMIDcase8NonEComeFromTarget_mbias4Pt_InM_onetwo_Both_mbias4;


  //***** brief Copy constructor.
  CbmKresConversionManualmbias4(const CbmKresConversionManualmbias4&);

  //***** brief Assignment operator.
  CbmKresConversionManualmbias4 operator=(const CbmKresConversionManualmbias4&);


  ClassDef(CbmKresConversionManualmbias4,2)
};

#endif

