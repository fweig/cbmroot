/** @file CbmKresConversionGeneral.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBM_KRES_CONVERSION_GENERAL
#define CBM_KRES_CONVERSION_GENERAL

class TClonesArray;
class TH1;
class TH1D;
class TH2D;
class TH3D;
class TGraph;
class TProfile2D;
class CbmRichRingLight;
class CbmRichRingFitterEllipseTau;

#include "FairMCEventHeader.h"

#include <TClonesArray.h>

#include <vector>


using namespace std;

class CbmKresConversionGeneral {

public:
  //***** brief Standard constructor.
  CbmKresConversionGeneral();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionGeneral();


  void Init();
  void InitHistograms();
  void Exec(int fEventNumGen);
  void Finish();

  void FitAndFillHistEllipse(CbmRichRingLight* ring);


private:
  TClonesArray* fMcTracks;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fRichPoints;
  TClonesArray* fRichHits;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;
  TClonesArray* fRichProjections;
  FairMCEventHeader* fArrayCentrality;

  CbmRichRingFitterEllipseTau* fTauFit;

  Double_t fMinAaxis;
  Double_t fMaxAaxis;
  Double_t fMinBaxis;
  Double_t fMaxBaxis;
  Double_t fMinRadius;
  Double_t fMaxRadius;

  TH2D* fitt;
  TGraph* imageellipse;
  TGraph* imagehits;


  // definition of histograms
  vector<TH1*> fHistoList;
  vector<TH1*> fHistoList_MC;
  TH2D* ForChristian_P_vs_R;
  TH2D* AllPoints2D;
  TH3D* AllPoints3D;
  TH1D* MC_PdgCodes;
  TH1D* MC_All_photons_Pt;
  TH1D* MC_Not_Direct_photons_Pt;
  TH1D* MC_Direct_photons_Pt;
  TH1D* MC_All_photons_P;
  TH1D* MC_Not_Direct_photons_P;
  TH1D* MC_Direct_photons_P;
  TH1D* MC_photons_mother_Pdg;
  TH1D* MC_Not_Direct_photons_theta;
  TH2D* MC_Not_Direct_photons_theta_vs_rap;
  TH1D* MC_Direct_photons_theta;
  TH2D* MC_Direct_photons_theta_vs_rap;
  TH2D* MC_Direct_photons_Pt_vs_rap;
  TH2D* MC_Direct_photons_Pt_vs_rap_est;
  TH2D* MC_electrons_Pt_vs_rap_est;
  TH2D* MC_Reconstructed_electrons_Pt_vs_rap_est;
  TH2D* MC_omega_Pt_vs_rap_est;
  TH1D* MC_pi0_Pt;
  TH1D* MC_pi0_Pt_est;
  TH2D* MC_pi0_Pt_vs_rap;
  TH2D* MC_pi0_Pt_vs_rap_primary;
  TH2D* Pi0_pt_vs_rap_est;
  TH2D* Pi0_pt_vs_rap_est_primary;
  TH1D* MC_pi0_theta;
  TH1D* MC_pi0_phi;
  TH1D* MC_pi0_Rapidity;
  TH2D* MC_pi0_theta_vs_rap;
  TH2D* MC_leptons_conversion_ZY;
  TH2D* MC_leptons_conversion_XY;
  TH2D* MC_leptons_conversion_XZ;
  TH2D* MC_leptons_from_pi0_start_vertex;
  TH1D* MC_leptons_from_pi0_P;
  TH1D* MC_eta_Pt;
  TH2D* MC_eta_Pt_vs_rap;
  TH2D* MC_eta_Pt_vs_rap_primary;
  TH1D* MC_eta_theta;
  TH2D* MC_eta_theta_vs_rap;

  TProfile2D* BoA_electrons;
  TH1D* BoA_1d_electrons;
  TH1D* A_1d_electrons;
  TH1D* B_1d_electrons;
  TProfile2D* A_electrons;
  TProfile2D* B_electrons;
  TH2D* NumberOfRings_electrons;
  TH2D* AllHits_electrons;
  TH1D* dR_electrons;
  TProfile2D* dR2d_electrons;
  TProfile2D* Distance_electron;
  TProfile2D* Distance_positron;

  TH1D* Tracks_electrons;
  TH1D* Rings_electrons;

  TH3D* fhBoverAXYZ;
  TH3D* fhBaxisXYZ;
  TH3D* fhAaxisXYZ;
  TH3D* fhdRXYZ;

  TH2D* Test_rings;

  TH2D* AllPointsPerPMT;
  TH2D* AllPointsPerPixel;

  TH2D* AllHits2D;
  TH3D* AllHits3D;
  TH2D* AllHitsPerPMT;
  TH2D* AllHitsPerPixel;
  TH2D* temporarygraph;
  TH1D* HitsPerPmtFullPlane;
  TH1D* HitsPerPmtFullMiddle;


  //***** brief Copy constructor.
  CbmKresConversionGeneral(const CbmKresConversionGeneral&);

  //***** brief Assignment operator.
  CbmKresConversionGeneral operator=(const CbmKresConversionGeneral&);


  ClassDef(CbmKresConversionGeneral, 1)
};

#endif
