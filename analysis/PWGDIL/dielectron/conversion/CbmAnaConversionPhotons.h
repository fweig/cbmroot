/* Copyright (C) 2014-2016 Fakultaet fuer Mathematik und Naturwissenschaften, Bergische Universitaet Wuppertal, Wuppertal
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sascha Reinecke [committer], Florian Uhlig */

/**
 * \file CbmAnaConversionPhotons.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/


#ifndef CBM_ANA_CONVERSION_PHOTONS
#define CBM_ANA_CONVERSION_PHOTONS

// included from ROOT
#include <TClonesArray.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStopwatch.h>

// included from CbmRoot
#include "CbmKFVertex.h"
#include "CbmMCTrack.h"
#include "CbmVertex.h"

#include <vector>

#include "LmvmKinePar.h"

/*
class CbmAnaConversionKinematicParams
{
public:
   Double_t fMomentumMag; // Absolute value of momentum
   Double_t fPt; // Transverse momentum
   Double_t fRapidity; // Rapidity
   Double_t fMinv; // Invariant mass
   Double_t fAngle; // Opening angle
   TVector3 fMomentumPhoton;
};
*/

class CbmAnaConversionPhotons {

public:
  CbmAnaConversionPhotons();
  virtual ~CbmAnaConversionPhotons();

  void Init();
  void InitHistos();
  void Finish();
  void Exec();


  void AnalysePhotons();
  void FillMCTracklist(CbmMCTrack* mctrack);
  void FillMCTracklist_allElectrons(CbmMCTrack* mctrack);
  void FillMCTracklist_allGammas(CbmMCTrack* mctrack);
  void FillRecoTracklist_allElectrons(CbmMCTrack* mctrack, TVector3 refittedMom);
  void CombineElectrons();
  int CheckMC(CbmMCTrack* mctrack);

  //CbmAnaConversionKinematicParams CalculateKinematicParams(const CbmMCTrack* mctrackP, const CbmMCTrack* mctrackM);
  LmvmKinePar CalculateKinematicParams(const CbmMCTrack* mctrackP, const CbmMCTrack* mctrackM);
  LmvmKinePar CalculateKinematicParamsReco(const TVector3 electron1, const TVector3 electron2);


  void AnalyseElectronsFromGammaMC();
  void AnalyseElectronsFromGammaReco();
  void AnalyseElectronsFromPi0Reco();
  void AnalyseGammas();


private:
  TClonesArray* fRichPoints;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;
  TClonesArray* fMcTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fGlobalTracks;
  CbmVertex* fPrimVertex;
  CbmKFVertex fKFVertex;

  std::vector<TH1*> fHistoList_photons;  // list of all histograms related to rich rings
  std::vector<TH1*> fHistoList_EFG;
  std::vector<TH1*> fHistoList_EFG_angle;
  std::vector<TH1*> fHistoList_EFG_invmass;
  std::vector<TH1*> fHistoList_EFG_energy;


  TH1I* fPhotons_nofPerEvent;
  TH1D* fPhotons_pt;
  TH1I* fPhotons_test;
  TH1I* fPhotons_nofMC;
  TH1D* fPhotons_energyMC;
  TH1D* fPhotons_ptMC;
  TH2D* fPhotons_ptRapMC;
  TH1D* fPhotonsRest_energyMC;
  TH1D* fPhotonsRest_pdgMotherMC;
  TH1D* fPhotonsRest_ptMC;

  TH1D* fPhotonsRest_ptMC_pi0;
  TH1D* fPhotonsRest_ptMC_n;
  TH1D* fPhotonsRest_ptMC_e;
  TH1D* fPhotonsRest_ptMC_eta;

  std::vector<CbmMCTrack*> fMCTracklist;
  std::vector<CbmMCTrack*> fMCTracklist_allElectronsFromGamma;
  std::vector<CbmMCTrack*> fRecoTracklist_allElectronsFromGamma;
  std::vector<TVector3> fRecoTracklist_allElectronsFromGammaMom;
  std::vector<CbmMCTrack*> fRecoTracklist_allElectronsFromPi0;
  std::vector<TVector3> fRecoTracklist_allElectronsFromPi0Mom;

  std::vector<CbmMCTrack*> fMCTracklist_allGammas;
  std::vector<CbmMCTrack*> fMCTracklist_allGammasFromPi0;

  Int_t global_nof_photonsMC;
  Int_t global_nof_photonsReco;
  TH1D* fhGlobalNofDirectPhotons;

  // distributions of opening angles from MC-true data
  TH1D* fhEFG_angle_all;
  TH1D* fhEFG_angle_combBack;
  TH1D* fhEFG_angle_allSameG;
  TH1D* fhEFG_angle_direct;
  TH1D* fhEFG_angle_pi0;
  TH1D* fhEFG_angle_eta;

  // opening angle vs energy from MC-true data
  //TH2D * fhEFG_angleVSenergy_all;
  //TH2D * fhEFG_angleVSenergy_combBack;
  TH2D* fhEFG_angleVSenergy_allSameG;
  TH2D* fhEFG_angleVSenergy_direct;
  TH2D* fhEFG_angleVSenergy_pi0;
  TH2D* fhEFG_angleVSenergy_eta;

  // opening angle vs pt from MC-true data
  TH2D* fhEFG_angleVSpt_all;
  TH2D* fhEFG_angleVSpt_combBack;
  TH2D* fhEFG_angleVSpt_allSameG;
  TH2D* fhEFG_angleVSpt_direct;
  TH2D* fhEFG_angleVSpt_pi0;
  TH2D* fhEFG_angleVSpt_eta;

  // distribution of invariant masses from MC-true data
  TH1D* fhEFG_invmass_all;
  TH1D* fhEFG_invmass_combBack;
  TH1D* fhEFG_invmass_allSameG;
  TH1D* fhEFG_invmass_direct;
  TH1D* fhEFG_invmass_pi0;
  TH1D* fhEFG_invmass_eta;

  // distribution of energies from MC-true data
  TH1D* fhEFG_energy_all;
  TH1D* fhEFG_energy_combBack;
  TH1D* fhEFG_energy_allSameG;
  TH1D* fhEFG_energy_direct;
  TH1D* fhEFG_energy_pi0;
  TH1D* fhEFG_energy_eta;

  // distribution of opening angle for energies below 1 GeV from MC-true data
  TH1D* fhEFG_angleBelow1GeV_all;
  TH1D* fhEFG_angleBelow1GeV_combBack;
  TH1D* fhEFG_angleBelow1GeV_allSameG;
  TH1D* fhEFG_angleBelow1GeV_direct;
  TH1D* fhEFG_angleBelow1GeV_pi0;
  TH1D* fhEFG_angleBelow1GeV_eta;

  // distribution of opening angle for energies above 1 GeV from MC-true data
  TH1D* fhEFG_angleAbove1GeV_all;
  TH1D* fhEFG_angleAbove1GeV_combBack;
  TH1D* fhEFG_angleAbove1GeV_allSameG;
  TH1D* fhEFG_angleAbove1GeV_direct;
  TH1D* fhEFG_angleAbove1GeV_pi0;
  TH1D* fhEFG_angleAbove1GeV_eta;

  // distribution of pair-wise momenta from MC-true data
  TH1D* fhEFG_momentumPair_all;
  TH1D* fhEFG_momentumPair_combBack;
  TH1D* fhEFG_momentumPair_allSameG;
  TH1D* fhEFG_momentumPair_direct;
  TH1D* fhEFG_momentumPair_pi0;
  TH1D* fhEFG_momentumPair_eta;

  // startvertex (= point of conversion) on z-axis versus opening angle from MC-true data
  //TH2D * fhEFG_startvertexVSangle_all;
  //TH2D * fhEFG_startvertexVSangle_combBack;
  TH2D* fhEFG_startvertexVSangle_allSameG;
  TH2D* fhEFG_startvertexVSangle_direct;
  TH2D* fhEFG_startvertexVSangle_pi0;
  TH2D* fhEFG_startvertexVSangle_eta;

  TH1D* fhEFG_startvertex_allSameG;

  // distribution of opening angles from reconstructed momenta
  TH1D* fhEFG_angle_all_reco;
  TH1D* fhEFG_angle_combBack_reco;
  TH1D* fhEFG_angle_allSameG_reco;
  TH1D* fhEFG_angle_direct_reco;
  TH1D* fhEFG_angle_pi0_reco;
  TH1D* fhEFG_angle_eta_reco;

  // distribution of opening angles from reconstructed momenta, with application of opening angle cuts
  TH1D* fhEFG_angle_all_reco_cuts;
  TH1D* fhEFG_angle_combBack_reco_cuts;
  TH1D* fhEFG_angle_allSameG_reco_cuts;

  // histogram for comparison of different opening angle cuts and their influence on signal and background amounts
  TH1I* fhEFG_angle_reco_CUTcomparison;

  // distribution of opening angles for photon-energies below 1GeV from reconstructed momenta
  TH1D* fhEFG_angleBelow1GeV_all_reco;
  TH1D* fhEFG_angleBelow1GeV_combBack_reco;
  TH1D* fhEFG_angleBelow1GeV_allSameG_reco;
  TH1D* fhEFG_angleBelow1GeV_direct_reco;
  TH1D* fhEFG_angleBelow1GeV_pi0_reco;
  TH1D* fhEFG_angleBelow1GeV_eta_reco;

  // distribution of invariant masses from reconstructed momenta
  TH1D* fhEFG_invmass_all_reco;
  TH1D* fhEFG_invmass_combBack_reco;
  TH1D* fhEFG_invmass_allSameG_reco;
  TH1D* fhEFG_invmass_direct_reco;
  TH1D* fhEFG_invmass_pi0_reco;
  TH1D* fhEFG_invmass_eta_reco;

  // distribution of invariant masses from reconstructed momenta with cut on opening angle
  TH1D* fhEFG_invmass_all_reco_cut;
  TH1D* fhEFG_invmass_combBack_reco_cut;
  TH1D* fhEFG_invmass_allSameG_reco_cut;
  TH1D* fhEFG_invmass_direct_reco_cut;
  TH1D* fhEFG_invmass_pi0_reco_cut;
  TH1D* fhEFG_invmass_eta_reco_cut;

  // opening angle vs pt from reconstructed data
  TH2D* fhEFG_angleVSpt_all_reco;
  TH2D* fhEFG_angleVSpt_combBack_reco;
  TH2D* fhEFG_angleVSpt_allSameG_reco;
  TH2D* fhEFG_angleVSpt_direct_reco;
  TH2D* fhEFG_angleVSpt_pi0_reco;
  TH2D* fhEFG_angleVSpt_eta_reco;

  // invariant mass vs pt from reconstructed data
  TH2D* fhEFG_invmassVSpt_all_reco;
  TH2D* fhEFG_invmassVSpt_combBack_reco;
  TH2D* fhEFG_invmassVSpt_allSameG_reco;
  TH2D* fhEFG_invmassVSpt_direct_reco;
  TH2D* fhEFG_invmassVSpt_pi0_reco;
  TH2D* fhEFG_invmassVSpt_eta_reco;

  // momentum resolution of photons from reconstructed data
  TH2D* fhEFG_momentumResolutionPhoton_reco;
  TH2D* fhEFG_momentumResolutionElectrons_reco;

  // opening angles for e+e- pairs from pi0 dalitz decays (i.e. e+e- pairs directly stemming from pi0)
  TH1D* fhEFPI0_angle_reco;

  // histos for direct combination of two gammas
  TH1D* fhG_invmass;
  TH1D* fhG_invmass_pi0;


  // timer
  TStopwatch timer;
  Double_t fTime;

  CbmAnaConversionPhotons(const CbmAnaConversionPhotons&);
  CbmAnaConversionPhotons operator=(const CbmAnaConversionPhotons&);

  ClassDef(CbmAnaConversionPhotons, 1)
};

#endif
