/** @file CbmKresEtaMCAnalysis.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef CBM_KRES_ETA_MC_ANALYSIS
#define CBM_KRES_ETA_MC_ANALYSIS

class TH1;
class TH1D;
class TH2D;

#include "CbmMCTrack.h"

#include <TClonesArray.h>

#include <vector>

using namespace std;

class CbmKresEtaMCAnalysis {

public:
  //***** brief Standard constructor.
  CbmKresEtaMCAnalysis();
  //***** brief Standard destructor.
  virtual ~CbmKresEtaMCAnalysis();


  void Init();
  void InitHistograms();
  void Finish();

  void Exec(int Event, double OpeningAngleCut, double GammaInvMassCut);

  Double_t CalculateOpeningAngleBetweenGammas_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2, CbmMCTrack* mctrack3,
                                                 CbmMCTrack* mctrack4);
  Double_t CalculateOpeningAngleBetweenGammas_Reco(TVector3 electron1, TVector3 electron2, TVector3 electron3,
                                                   TVector3 electron4);

  void EtaDoubleGammaAnalysis(vector<TVector3> RefMom, vector<CbmMCTrack*> MC, vector<Int_t> Id, vector<TH1*> gg);

  void EtaChargedPionsGammaAnalysis(vector<TVector3> RefMomPion, vector<CbmMCTrack*> MCPion, vector<Int_t> IdPion,
                                    vector<TVector3> RefMomEl, vector<CbmMCTrack*> MCEl, vector<Int_t> IdEl,
                                    vector<TH1*> ppg);

  void EtaPosNegNeutralPionsAnalysis(vector<TVector3> RefMomNeutral, vector<CbmMCTrack*> MCNeutral,
                                     vector<Int_t> IdNeutral, vector<TVector3> RefMomPion, vector<CbmMCTrack*> MCPion,
                                     vector<Int_t> IdPion, vector<TH1*> ppp);


  void EtaDoubleGammaAnalysis_plusBG(double OpeningAngleCut, double GammaInvMassCut, int Event, vector<TVector3> RefMom,
                                     vector<CbmMCTrack*> MC, vector<Int_t> Id, vector<TH1*> gg);
  void EtaChargedPionsGammaAnalysis_plusBG(double OpeningAngleCut, double GammaInvMassCut, int Event,
                                           vector<TVector3> RefMomPion, vector<CbmMCTrack*> MCPion,
                                           vector<Int_t> IdPion, vector<TVector3> RefMomEl, vector<CbmMCTrack*> MCEl,
                                           vector<Int_t> IdEl, vector<TH1*> ppg, vector<TH1*> ppp);

  void Mixing_gg();
  void Mixing_ppg();
  void Mixing_three_body();


private:
  TClonesArray* fMcTracks;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;

  vector<TVector3> El_Photon_Eta_refmomentum;
  vector<CbmMCTrack*> El_Photon_Eta_MCtrack;
  vector<int> El_Photon_Eta_Id;

  vector<TVector3> El_Photon_Pion_Eta_refmomentum;
  vector<CbmMCTrack*> El_Photon_Pion_Eta_MCtrack;
  vector<int> El_Photon_Pion_Eta_Id;

  vector<TVector3> Pion_Eta_refmomentum;
  vector<CbmMCTrack*> Pion_Eta_MCtrack;
  vector<int> Pion_Eta_Id;

  vector<TVector3> All_El_refmomentum;
  vector<CbmMCTrack*> All_El_MCtrack;
  vector<int> All_El_Id;

  vector<TVector3> All_Pion_refmomentum;
  vector<CbmMCTrack*> All_Pion_MCtrack;
  vector<int> All_Pion_Id;


  vector<TVector3> frefmomenta;
  vector<CbmMCTrack*> fMCtracks;
  vector<int> fMCId;

  std::vector<std::vector<TVector3>> EDGA_RefMom;
  std::vector<std::vector<CbmMCTrack*>> EDGA_MC;
  std::vector<std::vector<int>> EDGA_Id;

  std::vector<std::vector<TVector3>> ECPGA_leptons_RefMom;
  std::vector<std::vector<CbmMCTrack*>> ECPGA_leptons_MC;
  std::vector<std::vector<int>> ECPGA_leptons_Id;

  std::vector<std::vector<TVector3>> ECPGA_pions_RefMom;
  std::vector<std::vector<CbmMCTrack*>> ECPGA_pions_MC;


  vector<TH1*> fHistoList_eta_gg;
  TH1D* InvMass_eta_gg_mc;
  TH1D* InvMass_eta_gg_reffited;
  TH1D* InvMassPhoton_eta_gg_mc;
  TH1D* InvMassPhoton_eta_gg_reffited;
  TH1D* OpeningAnglePhoton_eta_gg_mc;
  TH1D* OpeningAnglePhoton_eta_gg_reffited;
  TH1D* OpeningAngle_eta_gg_between_gg_mc;
  TH1D* OpeningAngle_eta_gg_between_gg_reffited;
  TH1D* InvMass_eta_gg_allcombinations_mc;
  TH1D* InvMass_eta_gg_allcombinations_reffited;
  TH1D* EMT_eta_gg;
  TH1D* InvMass_eta_gg_reco_aftercuts;
  TH2D* rap_vs_pt_eta_gg_reco_aftercuts;
  TH2D* rap_vs_pt_NOTeta_gg_reco_aftercuts;


  vector<TH1*> fHistoList_eta_ppg;
  TH1D* InvMass_eta_ppg_mc;
  TH1D* InvMass_eta_ppg_reffited;
  TH1D* InvMassPhoton_eta_ppg_mc;
  TH1D* InvMassPhoton_eta_ppg_reffited;
  TH1D* OpeningAnglePhoton_eta_ppg_mc;
  TH1D* OpeningAnglePhoton_eta_ppg_reffited;
  TH1D* InvMass_eta_ppg_allcombinations_mc;
  TH1D* InvMass_eta_ppg_allcombinations_reffited;
  TH1D* Pion_P_fromEta_reco;
  TH1D* Pion_P_elsewhere_reco;
  TH1D* Pion_Pt_fromEta_reco;
  TH1D* Pion_Pt_elsewhere_reco;
  TH1D* OA_betweenPions_fromEta_mc;
  TH1D* OA_betweenPions_fromEta_reco;
  TH1D* OA_betweenPions_fromEta_reco_wrongcombinations;
  TH1D* EMT_eta_ppg;
  TH1D* EMT_eta_three_body;
  TH1D* InvMass_eta_ppg_reco_aftercuts;
  TH2D* rap_vs_pt_eta_ppg_reco_aftercuts;
  TH2D* rap_vs_pt_NOTeta_ppg_reco_aftercuts;


  vector<TH1*> fHistoList_eta_ppp;
  TH1D* InvMass_eta_ppp_mc;
  TH1D* InvMass_eta_ppp_reffited;
  TH1D* InvMass_eta_Npion_mc;
  TH1D* InvMass_eta_Npion_reffited;
  TH1D* InvMass_eta_ppp_allcombinations_mc;
  TH1D* InvMass_eta_ppp_allcombinations_reffited;


  // EMT for gg channel
  std::vector<int> EMT_gg_Event;
  std::vector<std::vector<TVector3>> EMT_gg_pair_momenta;

  // EMT for ppg channel
  std::vector<int> EMT_ppg_ee_Event;
  std::vector<std::vector<TVector3>> EMT_ppg_ee_pair_momenta;
  std::vector<int> EMT_ppg_pp_Event;
  std::vector<std::vector<TVector3>> EMT_ppg_pp_pair_momenta;

  // EMT for 3 body ==> completely uncorrelated bg
  std::vector<int> EMT_ppg_positive_pion_Event;
  std::vector<TVector3> EMT_ppg_positive_pion_momenta;
  std::vector<int> EMT_ppg_negative_pion_Event;
  std::vector<TVector3> EMT_ppg_negative_pion_momenta;


  //***** brief Copy constructor.
  CbmKresEtaMCAnalysis(const CbmKresEtaMCAnalysis&);

  //***** brief Assignment operator.
  CbmKresEtaMCAnalysis operator=(const CbmKresEtaMCAnalysis&);


  ClassDef(CbmKresEtaMCAnalysis, 1)
};

#endif
