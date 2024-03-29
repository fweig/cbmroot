/* Copyright (C) 2016 Fakultaet fuer Mathematik und Naturwissenschaften, Bergische Universitaet Wuppertal, Wuppertal
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sascha Reinecke [committer], Florian Uhlig */

/**
 * \file CbmAnaConversionTest2.h
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2016
 **/


#ifndef CBM_ANA_CONVERSION_TEST2
#define CBM_ANA_CONVERSION_TEST2

// included from ROOT
#include <TClonesArray.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStopwatch.h>

// included from CbmRoot
#include "CbmGlobalTrack.h"
#include "CbmKFVertex.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmMCTrack.h"
#include "CbmVertex.h"

#include <vector>

#include "LmvmKinePar.h"

class CbmAnaConversionTest2 {

public:
  CbmAnaConversionTest2();
  virtual ~CbmAnaConversionTest2();

  void Init();
  void InitHistos();
  void Finish();

  void Exec();

  void InvariantMassTest_3RICH();
  void InvariantMassTest_2RICH();
  Int_t NofDaughters(Int_t motherId);


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

  std::vector<TH1*> fHistoList_test2;  // list of all histograms related to rich rings


  // arrays for CombinePhotons_STSonly()
  TH1D* fhTest2_invmass_RICHindex0;
  TH1D* fhTest2_invmass_RICHindex1;
  TH1D* fhTest2_invmass_RICHindex2;
  TH1D* fhTest2_invmass_RICHindex3;
  TH1D* fhTest2_invmass_RICHindex4;


  // arrays for STS only analysis (i.e. signal only in STS is required, not necessarily in RICH)
  std::vector<CbmGlobalTrack*> fVector_gt;
  std::vector<TVector3> fVector_momenta;
  std::vector<CbmMCTrack*> fVector_mctrack;
  std::vector<int> fVector_gtIndex;
  std::vector<int> fVector_richIndex;


  TH1D* fhTest2_invmass_gee_mc;
  TH1D* fhTest2_invmass_gee_refitted;
  TH1D* fhTest2_invmass_gg_mc;
  TH1D* fhTest2_invmass_gg_refitted;
  TH1D* fhTest2_invmass_all_mc;
  TH1D* fhTest2_invmass_all_refitted;

  TH2D* fhTest2_pt_vs_rap_gee;
  TH2D* fhTest2_pt_vs_rap_gg;
  TH2D* fhTest2_pt_vs_rap_all;

  TH1D* fhTest2_startvertexElectrons_gee;
  TH1D* fhTest2_startvertexElectrons_gg;
  TH1D* fhTest2_startvertexElectrons_all;


  TH1D* fhTest2_2rich_invmass_gee_mc;
  TH1D* fhTest2_2rich_invmass_gee_refitted;
  TH1D* fhTest2_2rich_invmass_gg_mc;
  TH1D* fhTest2_2rich_invmass_gg_refitted;
  TH1D* fhTest2_2rich_invmass_all_mc;
  TH1D* fhTest2_2rich_invmass_all_refitted;

  TH2D* fhTest2_2rich_pt_vs_rap_gee;
  TH2D* fhTest2_2rich_pt_vs_rap_gg;
  TH2D* fhTest2_2rich_pt_vs_rap_all;


  // further tests
  TH2D* fhTest2_electrons_pt_vs_p;
  TH2D* fhTest2_electrons_pt_vs_p_withRICH;
  TH2D* fhTest2_electrons_pt_vs_p_noRICH;
  TH1D* fhTest2_3rich_electrons_theta_included;
  TH1D* fhTest2_3rich_electrons_theta_missing;
  TH2D* fhTest2_3rich_electrons_thetaVSp_included;
  TH2D* fhTest2_3rich_electrons_thetaVSp_missing;


  CbmAnaConversionTest2(const CbmAnaConversionTest2&);
  CbmAnaConversionTest2 operator=(const CbmAnaConversionTest2&);

  ClassDef(CbmAnaConversionTest2, 1)
};

#endif
