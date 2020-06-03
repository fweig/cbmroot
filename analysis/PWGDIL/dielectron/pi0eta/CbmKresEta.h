#ifndef CBM_KRES_ETA
#define CBM_KRES_ETA

#include <TClonesArray.h>
#include "TH2D.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "CbmRichRing.h"
#include "CbmKFVertex.h"
#include "CbmVertex.h"

class CbmRichRingFitterEllipseTau;

using namespace std;

class CbmKresEta
{

public:
  //***** brief Standard constructor.
  CbmKresEta();
  //***** brief Standard destructor.
  virtual ~CbmKresEta();


  void Init();
  void InitHistograms();
  void Finish();

  void Exec(int fEventNumEta, double OpeningAngleCut, double GammaInvMassCut, int RealPID);

  void SaveOutsideTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);
  void SaveTargetTracks(CbmMCTrack* mcTrack1, CbmStsTrack* stsTrack, TVector3 refmom, double charge, int stsInd, int richInd, int stsMcTrackId, CbmRichRing* RING);

  int FindInRich(int richInd, int stsMcTrackId);

  int CheckIfElectron(CbmRichRing* ring, double momentum);

  double CalculatePlaneAngle_last(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  double CalculatePlaneAngle_first(CbmStsTrack* Sts_1, CbmStsTrack* Sts_2);

  void FindGammasTarget(int EventNumEta, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus, vector<CbmMCTrack*> MCtracks_plus, 
                        vector<CbmStsTrack*> StsTrack_minus, vector<CbmStsTrack*> StsTrack_plus,
                        vector<TVector3> Momenta_minus, vector<TVector3> Momenta_plus, 
                        std::vector<int> Rings_minus, std::vector<int> Rings_plus, 
                        std::vector<int> stsIndex_minus, std::vector<int> stsIndex_plus, 
                        vector<CbmRichRing*> richRing_minus, vector<CbmRichRing*> richRing_plus,
                        vector<Int_t>  MCIndex_minus, vector<Int_t>  MCIndex_plus);

  void FindGammasOutside(int EventNumEta, double AngleCut, double InvMassCut, int RealPID, 
                        vector<CbmMCTrack*> MCtracks_minus_Outside, vector<CbmMCTrack*> MCtracks_plus_Outside,
                        vector<CbmStsTrack*> StsTrack_minus_Outside, vector<CbmStsTrack*> StsTrack_plus_Outside,
                        std::vector<int> Rings_minus_Outside, std::vector<int> Rings_plus_Outside,
                        std::vector<int> stsIndex_minus_Outside, std::vector<int> stsIndex_plus_Outside, 
                        vector<CbmRichRing*> richRing_minus_Outside, vector<CbmRichRing*> richRing_plus_Outside,
                        vector<Int_t>  MCIndex_minus_Outside, vector<Int_t>  MCIndex_plus_Outside);

  void FindGammasBoth();

  void FindEta(TString mod, TString position, vector< vector<TVector3> > Gammas, vector< vector<int> > StsIndex, vector< vector<int> > MCIndex, vector< vector<CbmMCTrack*> > GammasMC, 
             vector<TH1*> gg, vector<TH1*> rap_pt_separation);

  void Mixing_Target();
  void Mixing_Outside();
  void Mixing_Both(vector<TH1*> rap_pt_separation_all, vector<TH1*> rap_pt_separation_onetwo, vector<TH1*> rap_pt_separation_two);


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

  CbmVertex * fPrimVertex;
  CbmKFVertex fKFVertex;

  CbmRichRingFitterEllipseTau* fTauFit;


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
  std::vector<int>                      EMT_eta_gg_Event_Target;
  std::vector< std::vector<TVector3> >  EMT_eta_gg_pair_momenta_Target;
  std::vector<int>                      EMT_eta_gg_NofRings_Target;

  // EMT target Outside
  std::vector<int>                      EMT_eta_gg_Event_Outside;
  std::vector< std::vector<TVector3> >  EMT_eta_gg_pair_momenta_Outside;
  std::vector<int>                      EMT_eta_gg_NofRings_Outside;

  // EMT target Both
  std::vector<int>                      EMT_eta_gg_Event_Both;
  std::vector< std::vector<TVector3> >  EMT_eta_gg_pair_momenta_Both;
  std::vector<int>                      EMT_eta_gg_NofRings_Both;


    // combined gammas from the Target
  std::vector< std::vector<TVector3> > Gammas_all_Target;
  std::vector< std::vector<TVector3> > Gammas_two_Target;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Target;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Target;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Target;

  std::vector< std::vector<int> > Gammas_MCIndex_all_Target;
  std::vector< std::vector<int> > Gammas_MCIndex_two_Target;
  std::vector< std::vector<int> > Gammas_MCIndex_onetwo_Target;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Target;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Target;


  // combined gammas from Outside
  std::vector< std::vector<TVector3> > Gammas_all_Outside;
  std::vector< std::vector<TVector3> > Gammas_two_Outside;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Outside;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Outside;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Outside;

  std::vector< std::vector<int> > Gammas_MCIndex_all_Outside;
  std::vector< std::vector<int> > Gammas_MCIndex_two_Outside;
  std::vector< std::vector<int> > Gammas_MCIndex_onetwo_Outside;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Outside;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Outside;


  // combined gammas from Both: Target and Outside
  std::vector< std::vector<TVector3> > Gammas_all_Both;
  std::vector< std::vector<TVector3> > Gammas_two_Both;
  std::vector< std::vector<TVector3> > Gammas_onetwo_Both;

  std::vector< std::vector<int> > Gammas_stsIndex_all_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_two_Both;
  std::vector< std::vector<int> > Gammas_stsIndex_onetwo_Both;

  std::vector< std::vector<int> > Gammas_MCIndex_all_Both;
  std::vector< std::vector<int> > Gammas_MCIndex_two_Both;
  std::vector< std::vector<int> > Gammas_MCIndex_onetwo_Both;

  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_all_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_two_Both;
  std::vector< std::vector<CbmMCTrack*> > Gammas_MC_onetwo_Both;








  // histograms
  vector<TH1*> fHistoList_Eta;
  TH1D * Particle_pull_px_Target;
  TH1D * Particle_pull_py_Target;
  TH1D * Particle_pull_pz_Target;
  TH1D * Particle_pull_px_Outside;
  TH1D * Particle_pull_py_Outside;
  TH1D * Particle_pull_pz_Outside;
  TH1D * Particle_pull_X_Outside;
  TH1D * Particle_pull_Y_Outside;
  TH1D * Particle_pull_Z_Outside;


  ///////   histograms to check Cuts => Target
  vector<TH1*> fHistoList_Eta_cuts_Target;

  vector<TH1*> fHistoList_Eta_cuts_zeroInRich_Target;
  TH1D * GammasInvMass_fromEta_beforeCuts_zeroInRich_Target;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Target;
  TH1D * GammasMomentum_fromEta_beforeCuts_zeroInRich_Target;
  TH1D * GammasRapidity_fromEta_beforeCuts_zeroInRich_Target;
  TH1D * PlaneAngles_last_fromEta_zeroInRich_Target;
  TH1D * PlaneAngles_first_fromEta_zeroInRich_Target;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Target;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Target;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Target;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Target;
  TH1D * PlaneAngles_last_wrongpairs_zeroInRich_Target;
  TH1D * PlaneAngles_first_wrongpairs_zeroInRich_Target;

  vector<TH1*> fHistoList_Eta_cuts_oneInRich_Target;
  TH1D * GammasInvMass_fromEta_beforeCuts_oneInRich_Target;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_oneInRich_Target;
  TH1D * GammasMomentum_fromEta_beforeCuts_oneInRich_Target;
  TH1D * GammasRapidity_fromEta_beforeCuts_oneInRich_Target;
  TH1D * PlaneAngles_last_fromEta_oneInRich_Target;
  TH1D * PlaneAngles_first_fromEta_oneInRich_Target;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_oneInRich_Target;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Target;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_oneInRich_Target;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_oneInRich_Target;
  TH1D * PlaneAngles_last_wrongpairs_oneInRich_Target;
  TH1D * PlaneAngles_first_wrongpairs_oneInRich_Target;

  vector<TH1*> fHistoList_Eta_cuts_twoInRich_Target;
  TH1D * GammasInvMass_fromEta_beforeCuts_twoInRich_Target;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_twoInRich_Target;
  TH1D * GammasMomentum_fromEta_beforeCuts_twoInRich_Target;
  TH1D * GammasRapidity_fromEta_beforeCuts_twoInRich_Target;
  TH1D * PlaneAngles_last_fromEta_twoInRich_Target;
  TH1D * PlaneAngles_first_fromEta_twoInRich_Target;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_twoInRich_Target;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Target;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_twoInRich_Target;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_twoInRich_Target;
  TH1D * PlaneAngles_last_wrongpairs_twoInRich_Target;
  TH1D * PlaneAngles_first_wrongpairs_twoInRich_Target;



  ///////   histograms to check Cuts => Outside
  vector<TH1*> fHistoList_Eta_cuts_Outside;

  vector<TH1*> fHistoList_Eta_cuts_zeroInRich_Outside;
  TH1D * GammasInvMass_fromEta_beforeCuts_zeroInRich_Outside;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Outside;
  TH1D * GammasMomentum_fromEta_beforeCuts_zeroInRich_Outside;
  TH1D * GammasRapidity_fromEta_beforeCuts_zeroInRich_Outside;
  TH1D * PlaneAngles_last_fromEta_zeroInRich_Outside;
  TH1D * PlaneAngles_first_fromEta_zeroInRich_Outside;
  TH1D * GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Outside;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Outside;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Outside;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Outside;
  TH1D * PlaneAngles_last_wrongpairs_zeroInRich_Outside;
  TH1D * PlaneAngles_first_wrongpairs_zeroInRich_Outside;

  vector<TH1*> fHistoList_Eta_cuts_oneInRich_Outside;
  TH1D * GammasInvMass_fromEta_beforeCuts_oneInRich_Outside;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_oneInRich_Outside;
  TH1D * GammasMomentum_fromEta_beforeCuts_oneInRich_Outside;
  TH1D * GammasRapidity_fromEta_beforeCuts_oneInRich_Outside;
  TH1D * PlaneAngles_last_fromEta_oneInRich_Outside;
  TH1D * PlaneAngles_first_fromEta_oneInRich_Outside;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_oneInRich_Outside;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Outside;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_oneInRich_Outside;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_oneInRich_Outside;
  TH1D * PlaneAngles_last_wrongpairs_oneInRich_Outside;
  TH1D * PlaneAngles_first_wrongpairs_oneInRich_Outside;

  vector<TH1*> fHistoList_Eta_cuts_twoInRich_Outside;
  TH1D * GammasInvMass_fromEta_beforeCuts_twoInRich_Outside;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_twoInRich_Outside;
  TH1D * GammasMomentum_fromEta_beforeCuts_twoInRich_Outside;
  TH1D * GammasRapidity_fromEta_beforeCuts_twoInRich_Outside;
  TH1D * PlaneAngles_last_fromEta_twoInRich_Outside;
  TH1D * PlaneAngles_first_fromEta_twoInRich_Outside;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_twoInRich_Outside;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Outside;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_twoInRich_Outside;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_twoInRich_Outside;
  TH1D * PlaneAngles_last_wrongpairs_twoInRich_Outside;
  TH1D * PlaneAngles_first_wrongpairs_twoInRich_Outside;

  ///////   histograms to check Cuts => Both
  vector<TH1*> fHistoList_Eta_cuts_Both;

  vector<TH1*> fHistoList_Eta_cuts_zeroInRich_Both;
  TH1D * GammasInvMass_fromEta_beforeCuts_zeroInRich_Both;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_zeroInRich_Both;
  TH1D * GammasMomentum_fromEta_beforeCuts_zeroInRich_Both;
  TH1D * GammasRapidity_fromEta_beforeCuts_zeroInRich_Both;
  TH1D * PlaneAngles_last_fromEta_zeroInRich_Both;
  TH1D * PlaneAngles_first_fromEta_zeroInRich_Both;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_zeroInRich_Both;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_zeroInRich_Both;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_zeroInRich_Both;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_zeroInRich_Both;
  TH1D * PlaneAngles_last_wrongpairs_zeroInRich_Both;
  TH1D * PlaneAngles_first_wrongpairs_zeroInRich_Both;

  vector<TH1*> fHistoList_Eta_cuts_oneInRich_Both;
  TH1D * GammasInvMass_fromEta_beforeCuts_oneInRich_Both;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_oneInRich_Both;
  TH1D * GammasMomentum_fromEta_beforeCuts_oneInRich_Both;
  TH1D * GammasRapidity_fromEta_beforeCuts_oneInRich_Both;
  TH1D * PlaneAngles_last_fromEta_oneInRich_Both;
  TH1D * PlaneAngles_first_fromEta_oneInRich_Both;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_oneInRich_Both;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_oneInRich_Both;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_oneInRich_Both;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_oneInRich_Both;
  TH1D * PlaneAngles_last_wrongpairs_oneInRich_Both;
  TH1D * PlaneAngles_first_wrongpairs_oneInRich_Both;
  TH1D * PlaneAngles_last_fromEta_afterCuts_oneInRich_Both;
  TH1D * PlaneAngles_first_fromEta_afterCuts_oneInRich_Both;
  TH1D * PlaneAngles_last_wrongpairs_afterCuts_oneInRich_Both;
  TH1D * PlaneAngles_first_wrongpairs_afterCuts_oneInRich_Both;

  vector<TH1*> fHistoList_Eta_cuts_twoInRich_Both;
  TH1D * GammasInvMass_fromEta_beforeCuts_twoInRich_Both;
  TH1D * GammasOpenAngle_fromEta_beforeCuts_twoInRich_Both;
  TH1D * GammasMomentum_fromEta_beforeCuts_twoInRich_Both;
  TH1D * GammasRapidity_fromEta_beforeCuts_twoInRich_Both;
  TH1D * PlaneAngles_last_fromEta_twoInRich_Both;
  TH1D * PlaneAngles_first_fromEta_twoInRich_Both;  
  TH1D * GammasInvMass_wrongpairs_beforeCuts_twoInRich_Both;
  TH1D * GammasOpenAngle_wrongpairs_beforeCuts_twoInRich_Both;
  TH1D * GammasMomentum_wrongpairs_beforeCuts_twoInRich_Both;
  TH1D * GammasRapidity_wrongpairs_beforeCuts_twoInRich_Both;
  TH1D * PlaneAngles_last_wrongpairs_twoInRich_Both;
  TH1D * PlaneAngles_first_wrongpairs_twoInRich_Both;


  /////////////////////////////// TARGET //////////////////////////////////////
  ///////// reconstructed Eta "all" and "Target"
  vector<TH1*> fHistoList_Eta_all_Target;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_all_Target;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_all_Target;
  TH1D * InvMass_GammaCombinations_all_Target;
  TH1D * Particles_PDG_all_Target;
  TH2D * TrueEta_pt_vs_rap_all_Target;
  TH2D * WrongEta_pt_vs_rap_all_Target;
  TH2D * TrueEta_pt_vs_rap_est_all_Target;
  TH2D * WrongEta_pt_vs_rap_est_all_Target;
  TH1D * TrueEta_InvMass_after_cuts_all_Target;
  TH1D * WrongEta_InvMass_after_cuts_all_Target;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_all_Target;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_all_Target;
  TH1D * EMT_Eta_InvMass_all_Target;
  ///////// reconstructed Eta "onetwo" and "Target"
  vector<TH1*> fHistoList_Eta_onetwo_Target;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_onetwo_Target;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_onetwo_Target;
  TH1D * InvMass_GammaCombinations_onetwo_Target;
  TH1D * Particles_PDG_onetwo_Target;
  TH2D * TrueEta_pt_vs_rap_onetwo_Target;
  TH2D * WrongEta_pt_vs_rap_onetwo_Target;
  TH2D * TrueEta_pt_vs_rap_est_onetwo_Target;
  TH2D * WrongEta_pt_vs_rap_est_onetwo_Target;
  TH1D * TrueEta_InvMass_after_cuts_onetwo_Target;
  TH1D * WrongEta_InvMass_after_cuts_onetwo_Target;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_onetwo_Target;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_onetwo_Target;
  TH1D * EMT_Eta_InvMass_onetwo_Target;
  ///////// reconstructed Eta "two" and "Target"
  vector<TH1*> fHistoList_Eta_two_Target;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_two_Target;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_two_Target;
  TH1D * InvMass_GammaCombinations_two_Target;
  TH1D * Particles_PDG_two_Target;
  TH2D * TrueEta_pt_vs_rap_two_Target;
  TH2D * WrongEta_pt_vs_rap_two_Target;
  TH2D * TrueEta_pt_vs_rap_est_two_Target;
  TH2D * WrongEta_pt_vs_rap_est_two_Target;
  TH1D * TrueEta_InvMass_after_cuts_two_Target;
  TH1D * WrongEta_InvMass_after_cuts_two_Target;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_two_Target;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_two_Target;
  TH1D * EMT_Eta_InvMass_two_Target;
  /////////////////////////////// TARGET (END) //////////////////////////////////////
  vector<TH1*> fHistoList_rap_pt_Eta_all_Target;
  vector<TH1*> fHistoList_rap_pt_Eta_onetwo_Target;
  vector<TH1*> fHistoList_rap_pt_Eta_two_Target;


  /////////////////////////////// OUTSIDE //////////////////////////////////////
  ///////// reconstructed Eta "all" and "Outside"
  vector<TH1*> fHistoList_Eta_all_Outside;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_all_Outside;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_all_Outside;
  TH1D * InvMass_GammaCombinations_all_Outside;
  TH1D * Particles_PDG_all_Outside;
  TH2D * TrueEta_pt_vs_rap_all_Outside;
  TH2D * WrongEta_pt_vs_rap_all_Outside;
  TH2D * TrueEta_pt_vs_rap_est_all_Outside;
  TH2D * WrongEta_pt_vs_rap_est_all_Outside;
  TH1D * TrueEta_InvMass_after_cuts_all_Outside;
  TH1D * WrongEta_InvMass_after_cuts_all_Outside;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_all_Outside;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_all_Outside;
  TH1D * EMT_Eta_InvMass_all_Outside;
  ///////// reconstructed Eta "onetwo" and "Outside"
  vector<TH1*> fHistoList_Eta_onetwo_Outside;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_onetwo_Outside;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_onetwo_Outside;
  TH1D * InvMass_GammaCombinations_onetwo_Outside;
  TH1D * Particles_PDG_onetwo_Outside;
  TH2D * TrueEta_pt_vs_rap_onetwo_Outside;
  TH2D * WrongEta_pt_vs_rap_onetwo_Outside;
  TH2D * TrueEta_pt_vs_rap_est_onetwo_Outside;
  TH2D * WrongEta_pt_vs_rap_est_onetwo_Outside;
  TH1D * TrueEta_InvMass_after_cuts_onetwo_Outside;
  TH1D * WrongEta_InvMass_after_cuts_onetwo_Outside;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_onetwo_Outside;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_onetwo_Outside;
  TH1D * EMT_Eta_InvMass_onetwo_Outside;
  ///////// reconstructed Eta "two" and "Outside"
  vector<TH1*> fHistoList_Eta_two_Outside;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_two_Outside;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_two_Outside;
  TH1D * InvMass_GammaCombinations_two_Outside;
  TH1D * Particles_PDG_two_Outside;
  TH2D * TrueEta_pt_vs_rap_two_Outside;
  TH2D * WrongEta_pt_vs_rap_two_Outside;
  TH2D * TrueEta_pt_vs_rap_est_two_Outside;
  TH2D * WrongEta_pt_vs_rap_est_two_Outside;
  TH1D * TrueEta_InvMass_after_cuts_two_Outside;
  TH1D * WrongEta_InvMass_after_cuts_two_Outside;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_two_Outside;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_two_Outside;
  TH1D * EMT_Eta_InvMass_two_Outside;
  /////////////////////////////// OUTSIDE (END) //////////////////////////////////////
  vector<TH1*> fHistoList_rap_pt_Eta_all_Outside;
  vector<TH1*> fHistoList_rap_pt_Eta_onetwo_Outside;
  vector<TH1*> fHistoList_rap_pt_Eta_two_Outside;




  /////////////////////////////// BOTH //////////////////////////////////////
  ///////// reconstructed Eta "All" and "Both"
  vector<TH1*> fHistoList_Eta_all_Both;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_all_Both;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_all_Both;
  TH1D * InvMass_GammaCombinations_all_Both;
  TH1D * Particles_PDG_all_Both;
  TH2D * TrueEta_pt_vs_rap_all_Both;
  TH2D * WrongEta_pt_vs_rap_all_Both;
  TH2D * TrueEta_pt_vs_rap_est_all_Both;
  TH2D * WrongEta_pt_vs_rap_est_all_Both;
  TH1D * TrueEta_InvMass_after_cuts_all_Both;
  TH1D * WrongEta_InvMass_after_cuts_all_Both;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_all_Both;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_all_Both;
  TH1D * EMT_Eta_InvMass_all_Both;
  //////// multidimensional Eta analysis
  vector<TH1*> fHistoList_rap_pt_Eta_all_Both;
  TH1D * multi_InvMass_Eta_all_Both_1;
  TH1D * multi_InvMass_Eta_all_Both_2;
  TH1D * multi_InvMass_Eta_all_Both_3;
  TH1D * multi_InvMass_Eta_all_Both_4;
  TH1D * multi_InvMass_Eta_all_Both_5;
  TH1D * multi_InvMass_Eta_all_Both_6;
  TH1D * multi_InvMass_Eta_all_Both_7;
  TH1D * multi_InvMass_Eta_all_Both_8;
  TH1D * multi_InvMass_Eta_all_Both_9;
  TH1D * multi_InvMass_Eta_all_Both_10;
  TH1D * multi_InvMass_Eta_all_Both_11;
  TH1D * multi_InvMass_Eta_all_Both_12;
  TH1D * multi_InvMass_Eta_all_Both_13;
  TH1D * multi_InvMass_Eta_all_Both_14;
  TH1D * multi_InvMass_Eta_all_Both_15;
  TH1D * multi_InvMass_Eta_all_Both_16;
  TH1D * multi_InvMass_Eta_all_Both_17;
  TH1D * multi_EMT_Eta_all_Both_1;
  TH1D * multi_EMT_Eta_all_Both_2;
  TH1D * multi_EMT_Eta_all_Both_3;
  TH1D * multi_EMT_Eta_all_Both_4;
  TH1D * multi_EMT_Eta_all_Both_5;
  TH1D * multi_EMT_Eta_all_Both_6;
  TH1D * multi_EMT_Eta_all_Both_7;
  TH1D * multi_EMT_Eta_all_Both_8;
  TH1D * multi_EMT_Eta_all_Both_9;
  TH1D * multi_EMT_Eta_all_Both_10;
  TH1D * multi_EMT_Eta_all_Both_11;
  TH1D * multi_EMT_Eta_all_Both_12;
  TH1D * multi_EMT_Eta_all_Both_13;
  TH1D * multi_EMT_Eta_all_Both_14;
  TH1D * multi_EMT_Eta_all_Both_15;
  TH1D * multi_EMT_Eta_all_Both_16;
  TH1D * multi_EMT_Eta_all_Both_17;



  ///////// reconstructed Eta "onetwo" and "Both"
  vector<TH1*> fHistoList_Eta_onetwo_Both;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_onetwo_Both;
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_onetwo_Both;
  TH1D * InvMass_GammaCombinations_onetwo_Both;
  TH1D * Particles_PDG_onetwo_Both;
  TH2D * TrueEta_pt_vs_rap_onetwo_Both;
  TH2D * WrongEta_pt_vs_rap_onetwo_Both;
  TH2D * TrueEta_pt_vs_rap_est_onetwo_Both;
  TH2D * WrongEta_pt_vs_rap_est_onetwo_Both;
  TH1D * TrueEta_InvMass_after_cuts_onetwo_Both;
  TH1D * WrongEta_InvMass_after_cuts_onetwo_Both;
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_onetwo_Both;
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_onetwo_Both;
  TH1D * EMT_Eta_InvMass_onetwo_Both;
  //////// multidimensional Eta analysis
  vector<TH1*> fHistoList_rap_pt_Eta_onetwo_Both;
  TH1D * multi_InvMass_Eta_onetwo_Both_1;
  TH1D * multi_InvMass_Eta_onetwo_Both_2;
  TH1D * multi_InvMass_Eta_onetwo_Both_3;
  TH1D * multi_InvMass_Eta_onetwo_Both_4;
  TH1D * multi_InvMass_Eta_onetwo_Both_5;
  TH1D * multi_InvMass_Eta_onetwo_Both_6;
  TH1D * multi_InvMass_Eta_onetwo_Both_7;
  TH1D * multi_InvMass_Eta_onetwo_Both_8;
  TH1D * multi_InvMass_Eta_onetwo_Both_9;
  TH1D * multi_InvMass_Eta_onetwo_Both_10;
  TH1D * multi_InvMass_Eta_onetwo_Both_11;
  TH1D * multi_InvMass_Eta_onetwo_Both_12;
  TH1D * multi_InvMass_Eta_onetwo_Both_13;
  TH1D * multi_InvMass_Eta_onetwo_Both_14;
  TH1D * multi_InvMass_Eta_onetwo_Both_15;
  TH1D * multi_InvMass_Eta_onetwo_Both_16;
  TH1D * multi_InvMass_Eta_onetwo_Both_17;
  TH1D * multi_EMT_Eta_onetwo_Both_1;
  TH1D * multi_EMT_Eta_onetwo_Both_2;
  TH1D * multi_EMT_Eta_onetwo_Both_3;
  TH1D * multi_EMT_Eta_onetwo_Both_4;
  TH1D * multi_EMT_Eta_onetwo_Both_5;
  TH1D * multi_EMT_Eta_onetwo_Both_6;
  TH1D * multi_EMT_Eta_onetwo_Both_7;
  TH1D * multi_EMT_Eta_onetwo_Both_8;
  TH1D * multi_EMT_Eta_onetwo_Both_9;
  TH1D * multi_EMT_Eta_onetwo_Both_10;
  TH1D * multi_EMT_Eta_onetwo_Both_11;
  TH1D * multi_EMT_Eta_onetwo_Both_12;
  TH1D * multi_EMT_Eta_onetwo_Both_13;
  TH1D * multi_EMT_Eta_onetwo_Both_14;
  TH1D * multi_EMT_Eta_onetwo_Both_15;
  TH1D * multi_EMT_Eta_onetwo_Both_16;
  TH1D * multi_EMT_Eta_onetwo_Both_17;



  ///////// reconstructed Eta "two" and "Both"
  vector<TH1*> fHistoList_Eta_two_Both;
  TH1D * AngleBetweenGammas_trueEta_before_cuts_two_Both;             // 0
  TH1D * AngleBetweenGammas_wrongEta_before_cuts_two_Both;            // 1
  TH1D * InvMass_GammaCombinations_two_Both;                          // 2
  TH1D * Particles_PDG_two_Both;                                      // 3
  TH2D * TrueEta_pt_vs_rap_two_Both;                                  // 4
  TH2D * WrongEta_pt_vs_rap_two_Both;                                 // 5
  TH2D * TrueEta_pt_vs_rap_est_two_Both;                              // 6
  TH2D * WrongEta_pt_vs_rap_est_two_Both;                             // 7
  TH1D * TrueEta_InvMass_after_cuts_two_Both;                         // 8
  TH1D * WrongEta_InvMass_after_cuts_two_Both;                        // 9
  TH1D * TrueEta_AngleBetweenGammas_after_cuts_two_Both;              // 10
  TH1D * WrongEta_AngleBetweenGammas_after_cuts_two_Both;             // 11
  TH1D * EMT_Eta_InvMass_two_Both;                                    // 12
  //////// multidimensional Eta analysis
  vector<TH1*> fHistoList_rap_pt_Eta_two_Both;
  TH1D * multi_InvMass_Eta_two_Both_1;
  TH1D * multi_InvMass_Eta_two_Both_2;
  TH1D * multi_InvMass_Eta_two_Both_3;
  TH1D * multi_InvMass_Eta_two_Both_4;
  TH1D * multi_InvMass_Eta_two_Both_5;
  TH1D * multi_InvMass_Eta_two_Both_6;
  TH1D * multi_InvMass_Eta_two_Both_7;
  TH1D * multi_InvMass_Eta_two_Both_8;
  TH1D * multi_InvMass_Eta_two_Both_9;
  TH1D * multi_InvMass_Eta_two_Both_10;
  TH1D * multi_InvMass_Eta_two_Both_11;
  TH1D * multi_InvMass_Eta_two_Both_12;
  TH1D * multi_InvMass_Eta_two_Both_13;
  TH1D * multi_InvMass_Eta_two_Both_14;
  TH1D * multi_InvMass_Eta_two_Both_15;
  TH1D * multi_InvMass_Eta_two_Both_16;
  TH1D * multi_InvMass_Eta_two_Both_17;
  TH1D * multi_EMT_Eta_two_Both_1;
  TH1D * multi_EMT_Eta_two_Both_2;
  TH1D * multi_EMT_Eta_two_Both_3;
  TH1D * multi_EMT_Eta_two_Both_4;
  TH1D * multi_EMT_Eta_two_Both_5;
  TH1D * multi_EMT_Eta_two_Both_6;
  TH1D * multi_EMT_Eta_two_Both_7;
  TH1D * multi_EMT_Eta_two_Both_8;
  TH1D * multi_EMT_Eta_two_Both_9;
  TH1D * multi_EMT_Eta_two_Both_10;
  TH1D * multi_EMT_Eta_two_Both_11;
  TH1D * multi_EMT_Eta_two_Both_12;
  TH1D * multi_EMT_Eta_two_Both_13;
  TH1D * multi_EMT_Eta_two_Both_14;
  TH1D * multi_EMT_Eta_two_Both_15;
  TH1D * multi_EMT_Eta_two_Both_16;
  TH1D * multi_EMT_Eta_two_Both_17;
  /////////////////////////////// BOTH (END) //////////////////////////////////////


  //***** brief Copy constructor.
  CbmKresEta(const CbmKresEta&);

  //***** brief Assignment operator.
  CbmKresEta operator=(const CbmKresEta&);


	ClassDef(CbmKresEta,1)
};

#endif

