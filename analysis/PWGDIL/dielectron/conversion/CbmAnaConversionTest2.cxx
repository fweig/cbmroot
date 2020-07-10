/**
 * \file CbmAnaConversionTest2.cxx
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2016
 **/

#include "CbmAnaConversionTest2.h"

#include "CbmAnaConversionCutSettings.h"
#include "CbmAnaConversionKinematicParams.h"
#include "CbmGlobalTrack.h"
#include "CbmL1PFFitter.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmMCTrack.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "L1Field.h"

#include <algorithm>
#include <map>


using namespace std;


CbmAnaConversionTest2::CbmAnaConversionTest2()
  : fRichPoints(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fMcTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fGlobalTracks(NULL)
  , fPrimVertex(NULL)
  , fKFVertex()
  , fHistoList_test2()
  , fhTest2_invmass_RICHindex0(NULL)
  , fhTest2_invmass_RICHindex1(NULL)
  , fhTest2_invmass_RICHindex2(NULL)
  , fhTest2_invmass_RICHindex3(NULL)
  , fhTest2_invmass_RICHindex4(NULL)
  , fVector_gt()
  , fVector_momenta()
  , fVector_mctrack()
  , fVector_gtIndex()
  , fVector_richIndex()
  , fhTest2_invmass_gee_mc(NULL)
  , fhTest2_invmass_gee_refitted(NULL)
  , fhTest2_invmass_gg_mc(NULL)
  , fhTest2_invmass_gg_refitted(NULL)
  , fhTest2_invmass_all_mc(NULL)
  , fhTest2_invmass_all_refitted(NULL)
  , fhTest2_pt_vs_rap_gee(NULL)
  , fhTest2_pt_vs_rap_gg(NULL)
  , fhTest2_pt_vs_rap_all(NULL)
  , fhTest2_startvertexElectrons_gee(NULL)
  , fhTest2_startvertexElectrons_gg(NULL)
  , fhTest2_startvertexElectrons_all(NULL)
  , fhTest2_2rich_invmass_gee_mc(NULL)
  , fhTest2_2rich_invmass_gee_refitted(NULL)
  , fhTest2_2rich_invmass_gg_mc(NULL)
  , fhTest2_2rich_invmass_gg_refitted(NULL)
  , fhTest2_2rich_invmass_all_mc(NULL)
  , fhTest2_2rich_invmass_all_refitted(NULL)
  , fhTest2_2rich_pt_vs_rap_gee(NULL)
  , fhTest2_2rich_pt_vs_rap_gg(NULL)
  , fhTest2_2rich_pt_vs_rap_all(NULL)
  , fhTest2_electrons_pt_vs_p(NULL)
  , fhTest2_electrons_pt_vs_p_withRICH(NULL)
  , fhTest2_electrons_pt_vs_p_noRICH(NULL)
  , fhTest2_3rich_electrons_theta_included(NULL)
  , fhTest2_3rich_electrons_theta_missing(NULL)
  , fhTest2_3rich_electrons_thetaVSp_included(NULL)
  , fhTest2_3rich_electrons_thetaVSp_missing(NULL) {}

CbmAnaConversionTest2::~CbmAnaConversionTest2() {}


void CbmAnaConversionTest2::Init() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) {
    Fatal("CbmAnaConversion::Init", "RootManager not instantised!");
  }

  fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
  if (NULL == fRichPoints) {
    Fatal("CbmAnaConversion::Init", "No RichPoint array!");
  }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (NULL == fMcTracks) {
    Fatal("CbmAnaConversion::Init", "No MCTrack array!");
  }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (NULL == fStsTracks) {
    Fatal("CbmAnaConversion::Init", "No StsTrack array!");
  }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (NULL == fStsTrackMatches) {
    Fatal("CbmAnaConversion::Init", "No StsTrackMatch array!");
  }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) {
    Fatal("CbmAnaConversion::Init", "No GlobalTrack array!");
  }

  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) {
    fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex"));
  }
  if (nullptr == fPrimVertex) { LOG(fatal) << "No PrimaryVertex array!"; }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (NULL == fRichRings) {
    Fatal("CbmAnaConversion::Init", "No RichRing array!");
  }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) {
    Fatal("CbmAnaConversion::Init", "No RichRingMatch array!");
  }

  InitHistos();
}


void CbmAnaConversionTest2::InitHistos() {
  fHistoList_test2.clear();

  Double_t invmassSpectra_nof   = 800;
  Double_t invmassSpectra_start = -0.00125;
  Double_t invmassSpectra_end   = 1.99875;


  fhTest2_invmass_RICHindex0 = new TH1D(
    "fhTest2_invmass_RICHindex0",
    "fhTest2_invmass_RICHindex0; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    600,
    -0.0025,
    2.9975);
  fHistoList_test2.push_back(fhTest2_invmass_RICHindex0);
  fhTest2_invmass_RICHindex1 = new TH1D(
    "fhTest2_invmass_RICHindex1",
    "fhTest2_invmass_RICHindex1; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    600,
    -0.0025,
    2.9975);
  fHistoList_test2.push_back(fhTest2_invmass_RICHindex1);
  fhTest2_invmass_RICHindex2 = new TH1D(
    "fhTest2_invmass_RICHindex2",
    "fhTest2_invmass_RICHindex2; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    600,
    -0.0025,
    2.9975);
  fHistoList_test2.push_back(fhTest2_invmass_RICHindex2);
  fhTest2_invmass_RICHindex3 = new TH1D(
    "fhTest2_invmass_RICHindex3",
    "fhTest2_invmass_RICHindex3; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    600,
    -0.0025,
    2.9975);
  fHistoList_test2.push_back(fhTest2_invmass_RICHindex3);
  fhTest2_invmass_RICHindex4 = new TH1D(
    "fhTest2_invmass_RICHindex4",
    "fhTest2_invmass_RICHindex4; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    600,
    -0.0025,
    2.9975);
  fHistoList_test2.push_back(fhTest2_invmass_RICHindex4);


  fhTest2_invmass_gee_mc = new TH1D(
    "fhTest2_invmass_gee_mc",
    "fhTest2_invmass_gee_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_invmass_gee_refitted = new TH1D(
    "fhTest2_invmass_gee_refitted",
    "fhTest2_invmass_gee_refitted;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_invmass_gg_mc =
    new TH1D("fhTest2_invmass_gg_mc",
             "fhTest2_invmass_gg_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fhTest2_invmass_gg_refitted = new TH1D(
    "fhTest2_invmass_gg_refitted",
    "fhTest2_invmass_gg_refitted;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_invmass_all_mc = new TH1D(
    "fhTest2_invmass_all_mc",
    "fhTest2_invmass_all_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_invmass_all_refitted = new TH1D(
    "fhTest2_invmass_all_refitted",
    "fhTest2_invmass_all_refitted;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test2.push_back(fhTest2_invmass_gee_mc);
  fHistoList_test2.push_back(fhTest2_invmass_gee_refitted);
  fHistoList_test2.push_back(fhTest2_invmass_gg_mc);
  fHistoList_test2.push_back(fhTest2_invmass_gg_refitted);
  fHistoList_test2.push_back(fhTest2_invmass_all_mc);
  fHistoList_test2.push_back(fhTest2_invmass_all_refitted);

  fhTest2_pt_vs_rap_gee =
    new TH2D("fhTest2_pt_vs_rap_gee",
             "fhTest2_pt_vs_rap_gee;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fhTest2_pt_vs_rap_gg =
    new TH2D("fhTest2_pt_vs_rap_gg",
             "fhTest2_pt_vs_rap_gg;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fhTest2_pt_vs_rap_all =
    new TH2D("fhTest2_pt_vs_rap_all",
             "fhTest2_pt_vs_rap_all;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fHistoList_test2.push_back(fhTest2_pt_vs_rap_gee);
  fHistoList_test2.push_back(fhTest2_pt_vs_rap_gg);
  fHistoList_test2.push_back(fhTest2_pt_vs_rap_all);

  fhTest2_startvertexElectrons_gee =
    new TH1D("fhTest2_startvertexElectrons_gee",
             "fhTest2_startvertexElectrons_gee;z in cm;#",
             411,
             -5.25,
             200.25);
  fhTest2_startvertexElectrons_gg =
    new TH1D("fhTest2_startvertexElectrons_gg",
             "fhTest2_startvertexElectrons_gg;z in cm;#",
             411,
             -5.25,
             200.25);
  fhTest2_startvertexElectrons_all =
    new TH1D("fhTest2_startvertexElectrons_all",
             "fhTest2_startvertexElectrons_all;z in cm;#",
             411,
             -5.25,
             200.25);
  fHistoList_test2.push_back(fhTest2_startvertexElectrons_gee);
  fHistoList_test2.push_back(fhTest2_startvertexElectrons_gg);
  fHistoList_test2.push_back(fhTest2_startvertexElectrons_all);


  // 2 leptons in RICH
  fhTest2_2rich_invmass_gee_mc = new TH1D(
    "fhTest2_2rich_invmass_gee_mc",
    "fhTest2_2rich_invmass_gee_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_2rich_invmass_gee_refitted =
    new TH1D("fhTest2_2rich_invmass_gee_refitted",
             "fhTest2_2rich_invmass_gee_refitted;invariant mass of 4 e^{#pm} "
             "in GeV/c^{2};#",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fhTest2_2rich_invmass_gg_mc = new TH1D(
    "fhTest2_2rich_invmass_gg_mc",
    "fhTest2_2rich_invmass_gg_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_2rich_invmass_gg_refitted =
    new TH1D("fhTest2_2rich_invmass_gg_refitted",
             "fhTest2_2rich_invmass_gg_refitted;invariant mass of 4 e^{#pm} in "
             "GeV/c^{2};#",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fhTest2_2rich_invmass_all_mc = new TH1D(
    "fhTest2_2rich_invmass_all_mc",
    "fhTest2_2rich_invmass_all_mc;invariant mass of 4 e^{#pm} in GeV/c^{2};#",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fhTest2_2rich_invmass_all_refitted =
    new TH1D("fhTest2_2rich_invmass_all_refitted",
             "fhTest2_2rich_invmass_all_refitted;invariant mass of 4 e^{#pm} "
             "in GeV/c^{2};#",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_gee_mc);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_gee_refitted);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_gg_mc);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_gg_refitted);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_all_mc);
  fHistoList_test2.push_back(fhTest2_2rich_invmass_all_refitted);


  fhTest2_2rich_pt_vs_rap_gee =
    new TH2D("fhTest2_2rich_pt_vs_rap_gee",
             "fhTest2_2rich_pt_vs_rap_gee;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fhTest2_2rich_pt_vs_rap_gg =
    new TH2D("fhTest2_2rich_pt_vs_rap_gg",
             "fhTest2_2rich_pt_vs_rap_gg;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fhTest2_2rich_pt_vs_rap_all =
    new TH2D("fhTest2_2rich_pt_vs_rap_all",
             "fhTest2_2rich_pt_vs_rap_all;p_{t} in GeV/c; rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fHistoList_test2.push_back(fhTest2_2rich_pt_vs_rap_gee);
  fHistoList_test2.push_back(fhTest2_2rich_pt_vs_rap_gg);
  fHistoList_test2.push_back(fhTest2_2rich_pt_vs_rap_all);


  // further tests
  fhTest2_electrons_pt_vs_p =
    new TH2D("fhTest2_electrons_pt_vs_p",
             "fhTest2_electrons_pt_vs_p;p_{t} in GeV/c; p in GeV/c",
             240,
             -2.,
             10.,
             360,
             -2.,
             16.);
  fHistoList_test2.push_back(fhTest2_electrons_pt_vs_p);
  fhTest2_electrons_pt_vs_p_withRICH =
    new TH2D("fhTest2_electrons_pt_vs_p_withRICH",
             "fhTest2_electrons_pt_vs_p_withRICH;p_{t} in GeV/c; p in GeV/c",
             240,
             -2.,
             10.,
             360,
             -2.,
             16.);
  fHistoList_test2.push_back(fhTest2_electrons_pt_vs_p_withRICH);
  fhTest2_electrons_pt_vs_p_noRICH =
    new TH2D("fhTest2_electrons_pt_vs_p_noRICH",
             "fhTest2_electrons_pt_vs_p_noRICH;p_{t} in GeV/c; p in GeV/c",
             240,
             -2.,
             10.,
             360,
             -2.,
             16.);
  fHistoList_test2.push_back(fhTest2_electrons_pt_vs_p_noRICH);

  fhTest2_3rich_electrons_theta_included =
    new TH1D("fhTest2_3rich_electrons_theta_included",
             "fhTest2_3rich_electrons_theta_included;theta angle [deg];#",
             90,
             0,
             90);
  fHistoList_test2.push_back(fhTest2_3rich_electrons_theta_included);
  fhTest2_3rich_electrons_theta_missing =
    new TH1D("fhTest2_3rich_electrons_theta_missing",
             "fhTest2_3rich_electrons_theta_missing;theta angle [deg];#",
             90,
             0,
             90);
  fHistoList_test2.push_back(fhTest2_3rich_electrons_theta_missing);
  fhTest2_3rich_electrons_thetaVSp_included = new TH2D(
    "fhTest2_3rich_electrons_thetaVSp_included",
    "fhTest2_3rich_electrons_thetaVSp_included;theta angle [deg];p in GeV/c",
    90,
    0,
    90,
    540,
    -2.,
    16.);
  fHistoList_test2.push_back(fhTest2_3rich_electrons_thetaVSp_included);
  fhTest2_3rich_electrons_thetaVSp_missing = new TH2D(
    "fhTest2_3rich_electrons_thetaVSp_missing",
    "fhTest2_3rich_electrons_thetaVSp_missing;theta angle [deg];p in GeV/c",
    90,
    0,
    90,
    540,
    -2.,
    16.);
  fHistoList_test2.push_back(fhTest2_3rich_electrons_thetaVSp_missing);
}


void CbmAnaConversionTest2::Finish() {
  //gDirectory->cd("analysis-conversion");
  gDirectory->mkdir("Test2");
  gDirectory->cd("Test2");
  for (UInt_t i = 0; i < fHistoList_test2.size(); i++) {
    fHistoList_test2[i]->Write();
  }
  gDirectory->cd("..");
}


void CbmAnaConversionTest2::Exec() {
  fVector_gt.clear();
  fVector_momenta.clear();
  fVector_mctrack.clear();
  fVector_gtIndex.clear();
  fVector_richIndex.clear();


  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t i = 0; i < ngTracks; i++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
    if (NULL == gTrack) continue;
    int stsInd  = gTrack->GetStsTrackIndex();
    int richInd = gTrack->GetRichRingIndex();

    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == NULL) continue;
    CbmTrackMatchNew* stsMatch =
      (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == NULL) continue;
    if (stsMatch->GetNofLinks() <= 0) continue;
    int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
    if (mcTrack1 == NULL) continue;


    // calculate refitted momenta at primary vertex
    TVector3 refittedMomentum;
    CbmL1PFFitter fPFFitter;
    vector<CbmStsTrack> stsTracks;
    stsTracks.resize(1);
    stsTracks[0] = *stsTrack;
    vector<L1FieldRegion> vField;
    vector<float> chiPrim;
    fPFFitter.GetChiToVertex(stsTracks, vField, chiPrim, fKFVertex, 3e6);
    //cand.chi2sts = stsTracks[0].GetChiSq() / stsTracks[0].GetNDF();
    //cand.chi2Prim = chiPrim[0];
    const FairTrackParam* vtxTrack = stsTracks[0].GetParamFirst();
    vtxTrack->Momentum(refittedMomentum);
    //float result_chi = chiPrim[0];


    // Doing refit of momenta with electron assumption
    CbmL1PFFitter fPFFitter_electron;
    vector<CbmStsTrack> stsTracks_electron;
    stsTracks_electron.resize(1);
    stsTracks_electron[0] = *stsTrack;
    vector<L1FieldRegion> vField_electron;
    vector<float> chiPrim_electron;
    vector<int> pidHypo_electron;
    pidHypo_electron.push_back(11);
    fPFFitter_electron.Fit(stsTracks_electron, pidHypo_electron);
    fPFFitter_electron.GetChiToVertex(
      stsTracks_electron, vField_electron, chiPrim_electron, fKFVertex, 3e6);

    TVector3 refittedMomentum_electron;
    const FairTrackParam* vtxTrack_electron =
      stsTracks_electron[0].GetParamFirst();
    vtxTrack_electron->Momentum(refittedMomentum_electron);
    //float result_chi_electron = chiPrim_electron[0];
    //float result_ndf_electron = stsTracks_electron[0].GetNDF();
    //Double_t stsHits = stsTrack->GetNofHits();


    Int_t pdg = mcTrack1->GetPdgCode();

    if (TMath::Abs(pdg) == 11) {
      fhTest2_electrons_pt_vs_p->Fill(refittedMomentum_electron.Perp(),
                                      refittedMomentum_electron.Mag());
      if (richInd > 0) {
        fhTest2_electrons_pt_vs_p_withRICH->Fill(
          refittedMomentum_electron.Perp(), refittedMomentum_electron.Mag());
      }
      if (richInd < 0) {
        fhTest2_electrons_pt_vs_p_noRICH->Fill(refittedMomentum_electron.Perp(),
                                               refittedMomentum_electron.Mag());
      }


      fVector_momenta.push_back(refittedMomentum_electron);
      fVector_mctrack.push_back(mcTrack1);
      fVector_gtIndex.push_back(i);
      fVector_gt.push_back(gTrack);

      if (richInd < 0) {
        fVector_richIndex.push_back(0);
        continue;
      }
      CbmTrackMatchNew* richMatch =
        (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
      if (richMatch == NULL) {
        fVector_richIndex.push_back(0);
        continue;
      }
      int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
      if (richMcTrackId < 0) {
        fVector_richIndex.push_back(0);
        continue;
      }
      CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
      if (mcTrack2 == NULL) {
        fVector_richIndex.push_back(0);
        continue;
      }

      fVector_richIndex.push_back(richInd);
    }
  }

  InvariantMassTest_3RICH();
  InvariantMassTest_2RICH();
}


void CbmAnaConversionTest2::InvariantMassTest_3RICH()
// Calculating invariant mass of 4 ep/em, using MC data AND reconstructed momentum
{
  cout << "CbmAnaConversionTest2: InvariantMassTest_3RICH - Start..." << endl;
  cout << "CbmAnaConversionTest2: InvariantMassTest_3RICH - "
       << fVector_mctrack.size() << endl;
  int fill = 0;
  if (fVector_mctrack.size() < 4) return;
  for (unsigned int i = 0; i < fVector_mctrack.size() - 3; i++) {
    if (i % 10 == 0)
      cout << "CbmAnaConversionTest2: InvariantMassTest_3RICH - iteration i = "
           << i << endl;
    for (unsigned int j = i + 1; j < fVector_mctrack.size() - 2; j++) {
      for (unsigned int k = j + 1; k < fVector_mctrack.size() - 1; k++) {
        for (unsigned int l = k + 1; l < fVector_mctrack.size(); l++) {
          if (fVector_mctrack[i]->GetPdgCode()
                + fVector_mctrack[j]->GetPdgCode()
                + fVector_mctrack[k]->GetPdgCode()
                + fVector_mctrack[l]->GetPdgCode()
              != 0)
            continue;

          if (fVector_mctrack.size() != fVector_richIndex.size()) {
            cout << "CbmAnaConversionTest2: InvariantMassTest_4epem - not "
                    "matching number of entries!"
                 << endl;
            continue;
          }

          Bool_t IsPi0 = false;

          // starting points of each electron (-> i.e. conversion points of gamma OR decay points of pi0, depending on decay channel)
          TVector3 pi0start_i;
          fVector_mctrack[i]->GetStartVertex(pi0start_i);
          TVector3 pi0start_j;
          fVector_mctrack[j]->GetStartVertex(pi0start_j);
          TVector3 pi0start_k;
          fVector_mctrack[k]->GetStartVertex(pi0start_k);
          TVector3 pi0start_l;
          fVector_mctrack[l]->GetStartVertex(pi0start_l);


          Int_t richIndex1 = (fVector_richIndex[i] > 0);
          Int_t richIndex2 = (fVector_richIndex[j] > 0);
          Int_t richIndex3 = (fVector_richIndex[k] > 0);
          Int_t richIndex4 = (fVector_richIndex[l] > 0);

          if (richIndex1 + richIndex2 + richIndex3 + richIndex4 != 3) continue;


          int motherId1 = fVector_mctrack[i]->GetMotherId();
          int motherId2 = fVector_mctrack[j]->GetMotherId();
          int motherId3 = fVector_mctrack[k]->GetMotherId();
          int motherId4 = fVector_mctrack[l]->GetMotherId();


          if ((motherId1 == motherId2 && motherId3 == motherId4)
              || (motherId1 == motherId3 && motherId2 == motherId4)
              || (motherId1 == motherId4
                  && motherId2 == motherId3)) {  // start 1


            int grandmotherId1 = -1;
            int grandmotherId2 = -1;
            int grandmotherId3 = -1;
            int grandmotherId4 = -1;

            int mcMotherPdg1 = -1;
            //						int mcMotherPdg2  = -1; (FU) not used
            //						int mcMotherPdg3  = -1; (FU) not used
            //						int mcMotherPdg4  = -1; (FU) not used
            int mcGrandmotherPdg1 = -1;
            //						int mcGrandmotherPdg2  = -1;
            //						int mcGrandmotherPdg3  = -1;
            //						int mcGrandmotherPdg4  = -1;

            CbmMCTrack* grandmother1 = nullptr;

            if (motherId1 != -1) {
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              grandmotherId1 = mother1->GetMotherId();
              if (grandmotherId1 != -1) {
                grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
                if (NULL != grandmother1)
                  mcGrandmotherPdg1 = grandmother1->GetPdgCode();
              }
            }
            if (motherId2 != -1) {
              CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
              //							if (NULL != mother2) mcMotherPdg2 = mother2->GetPdgCode(); (FU) not used
              grandmotherId2 = mother2->GetMotherId();
              if (grandmotherId2 != -1) {
                //								CbmMCTrack* grandmother2 = (CbmMCTrack*) fMcTracks->At(grandmotherId2);
                //								if (NULL != grandmother2) mcGrandmotherPdg2 = grandmother2->GetPdgCode();
              }
            }
            if (motherId3 != -1) {
              CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
              //							if (NULL != mother3) mcMotherPdg3 = mother3->GetPdgCode(); (FU) not used
              grandmotherId3 = mother3->GetMotherId();
              if (grandmotherId3 != -1) {
                //								CbmMCTrack* grandmother3 = (CbmMCTrack*) fMcTracks->At(grandmotherId3);
                //								if (NULL != grandmother3) mcGrandmotherPdg3 = grandmother3->GetPdgCode();
              }
            }
            if (motherId4 != -1) {
              CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
              //							if (NULL != mother4) mcMotherPdg4 = mother4->GetPdgCode(); (FU) not used
              grandmotherId4 = mother4->GetMotherId();
              if (grandmotherId4 != -1) {
                //								CbmMCTrack* grandmother4 = (CbmMCTrack*) fMcTracks->At(grandmotherId4);
                //								if (NULL != grandmother4) mcGrandmotherPdg4 = grandmother4->GetPdgCode();
              }
            }

            if (grandmotherId1 == -1) continue;

            if (motherId1 == motherId2 && motherId3 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId3) != 2)
                continue;
              if ((grandmotherId1 == motherId3 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId3 && mcMotherPdg1 == 111)) {

                fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_invmass_all_mc->Fill(invmass_mc);
                fhTest2_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_pt_vs_rap_gee->Fill(params_reco.fPt,
                                            params_reco.fRapidity);
                fhTest2_pt_vs_rap_all->Fill(params_reco.fPt,
                                            params_reco.fRapidity);

                IsPi0 = true;
              }
            }
            if (motherId1 == motherId3 && motherId2 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2)
                continue;
              if ((grandmotherId1 == motherId2 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {

                fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_invmass_all_mc->Fill(invmass_mc);
                fhTest2_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_pt_vs_rap_gee->Fill(params_reco.fPt,
                                            params_reco.fRapidity);
                fhTest2_pt_vs_rap_all->Fill(params_reco.fPt,
                                            params_reco.fRapidity);

                IsPi0 = true;
              }
            }
            if (motherId1 == motherId4 && motherId2 == motherId3) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2)
                continue;
              if ((grandmotherId1 == motherId2 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {

                fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
                fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_invmass_all_mc->Fill(invmass_mc);
                fhTest2_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_pt_vs_rap_gee->Fill(params_reco.fPt,
                                            params_reco.fRapidity);
                fhTest2_pt_vs_rap_all->Fill(params_reco.fPt,
                                            params_reco.fRapidity);

                IsPi0 = true;
              }
            }


            // ===================================================================================================
            // HERE DECAY pi0 -> gamma gamma -> e+e- e+e-
            if (grandmotherId1 == grandmotherId2
                && grandmotherId1 == grandmotherId3
                && grandmotherId1 == grandmotherId4) {
              if (mcGrandmotherPdg1 != 111) continue;  // 111 = pi0, 221 = eta

              TVector3 pi0start;
              grandmother1->GetStartVertex(pi0start);

              fhTest2_startvertexElectrons_gg->Fill(pi0start_i.Z());
              fhTest2_startvertexElectrons_gg->Fill(pi0start_j.Z());
              fhTest2_startvertexElectrons_gg->Fill(pi0start_k.Z());
              fhTest2_startvertexElectrons_gg->Fill(pi0start_l.Z());
              fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
              fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
              fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
              fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());


              // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
              if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1
                  || pi0start_l.Z() > 1)
                continue;

              Double_t invmass_mc   = 0;
              Double_t invmass_reco = 0;
              CbmAnaConversionKinematicParams params_mc =
                CbmAnaConversionKinematicParams::KinematicParams_4particles_MC(
                  fVector_mctrack[i],
                  fVector_mctrack[j],
                  fVector_mctrack[k],
                  fVector_mctrack[l]);
              CbmAnaConversionKinematicParams params_reco =
                CbmAnaConversionKinematicParams::
                  KinematicParams_4particles_Reco(fVector_momenta[i],
                                                  fVector_momenta[j],
                                                  fVector_momenta[k],
                                                  fVector_momenta[l]);
              invmass_mc   = params_mc.fMinv;
              invmass_reco = params_reco.fMinv;

              fhTest2_invmass_gg_mc->Fill(invmass_mc);
              fhTest2_invmass_gg_refitted->Fill(invmass_reco);
              fhTest2_invmass_all_mc->Fill(invmass_mc);
              fhTest2_invmass_all_refitted->Fill(invmass_reco);

              fhTest2_pt_vs_rap_gg->Fill(params_reco.fPt,
                                         params_reco.fRapidity);
              fhTest2_pt_vs_rap_all->Fill(params_reco.fPt,
                                          params_reco.fRapidity);

              IsPi0 = true;

              cout << "########################################################"
                      "##############"
                   << endl;

              fill++;
            }


            if (richIndex1 <= 0 && IsPi0) {
              fhTest2_3rich_electrons_theta_missing->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_thetaVSp_missing->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi(),
                fVector_momenta[i].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi(),
                fVector_momenta[j].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi(),
                fVector_momenta[k].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi(),
                fVector_momenta[l].Mag());
            }
            if (richIndex2 <= 0 && IsPi0) {
              fhTest2_3rich_electrons_theta_missing->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_thetaVSp_missing->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi(),
                fVector_momenta[j].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi(),
                fVector_momenta[i].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi(),
                fVector_momenta[k].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi(),
                fVector_momenta[l].Mag());
            }
            if (richIndex3 <= 0 && IsPi0) {
              fhTest2_3rich_electrons_theta_missing->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_thetaVSp_missing->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi(),
                fVector_momenta[k].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi(),
                fVector_momenta[i].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi(),
                fVector_momenta[j].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi(),
                fVector_momenta[l].Mag());
            }
            if (richIndex4 <= 0 && IsPi0) {
              fhTest2_3rich_electrons_theta_missing->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_theta_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi());
              fhTest2_3rich_electrons_thetaVSp_missing->Fill(
                fVector_momenta[l].Theta() * 180 / TMath::Pi(),
                fVector_momenta[l].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[i].Theta() * 180 / TMath::Pi(),
                fVector_momenta[i].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[j].Theta() * 180 / TMath::Pi(),
                fVector_momenta[j].Mag());
              fhTest2_3rich_electrons_thetaVSp_included->Fill(
                fVector_momenta[k].Theta() * 180 / TMath::Pi(),
                fVector_momenta[k].Mag());
            }

          }  // end 1
        }
      }
    }
  }
}


void CbmAnaConversionTest2::InvariantMassTest_2RICH()
// Calculating invariant mass of 4 ep/em, using MC data AND reconstructed momentum
{
  cout << "CbmAnaConversionTest2: InvariantMassTest_2RICH - Start..." << endl;
  cout << "CbmAnaConversionTest2: InvariantMassTest_2RICH - "
       << fVector_mctrack.size() << endl;
  int fill = 0;
  if (fVector_mctrack.size() < 4) return;
  for (unsigned int i = 0; i < fVector_mctrack.size() - 3; i++) {
    if (i % 10 == 0)
      cout << "CbmAnaConversionTest2: InvariantMassTest_2RICH - iteration i = "
           << i << endl;
    for (unsigned int j = i + 1; j < fVector_mctrack.size() - 2; j++) {
      for (unsigned int k = j + 1; k < fVector_mctrack.size() - 1; k++) {
        for (unsigned int l = k + 1; l < fVector_mctrack.size(); l++) {
          if (fVector_mctrack[i]->GetPdgCode()
                + fVector_mctrack[j]->GetPdgCode()
                + fVector_mctrack[k]->GetPdgCode()
                + fVector_mctrack[l]->GetPdgCode()
              != 0)
            continue;

          if (fVector_mctrack.size() != fVector_richIndex.size()) {
            cout << "CbmAnaConversionTest2: InvariantMassTest_4epem - not "
                    "matching number of entries!"
                 << endl;
            continue;
          }


          // starting points of each electron (-> i.e. conversion points of gamma OR decay points of pi0, depending on decay channel)
          TVector3 pi0start_i;
          fVector_mctrack[i]->GetStartVertex(pi0start_i);
          TVector3 pi0start_j;
          fVector_mctrack[j]->GetStartVertex(pi0start_j);
          TVector3 pi0start_k;
          fVector_mctrack[k]->GetStartVertex(pi0start_k);
          TVector3 pi0start_l;
          fVector_mctrack[l]->GetStartVertex(pi0start_l);


          Int_t richIndex1 = (fVector_richIndex[i] > 0);
          Int_t richIndex2 = (fVector_richIndex[j] > 0);
          Int_t richIndex3 = (fVector_richIndex[k] > 0);
          Int_t richIndex4 = (fVector_richIndex[l] > 0);

          if (richIndex1 + richIndex2 + richIndex3 + richIndex4 != 2) continue;

          int motherId1 = fVector_mctrack[i]->GetMotherId();
          int motherId2 = fVector_mctrack[j]->GetMotherId();
          int motherId3 = fVector_mctrack[k]->GetMotherId();
          int motherId4 = fVector_mctrack[l]->GetMotherId();


          if ((motherId1 == motherId2 && motherId3 == motherId4)
              || (motherId1 == motherId3 && motherId2 == motherId4)
              || (motherId1 == motherId4 && motherId2 == motherId3)) {


            int grandmotherId1 = -1;
            int grandmotherId2 = -1;
            int grandmotherId3 = -1;
            int grandmotherId4 = -1;

            int mcMotherPdg1 = -1;
            //						int mcMotherPdg2  = -1; (FU) not used
            //						int mcMotherPdg3  = -1; (FU) not used
            //						int mcMotherPdg4  = -1; (FU) not used
            int mcGrandmotherPdg1 = -1;
            //						int mcGrandmotherPdg2  = -1;
            //						int mcGrandmotherPdg3  = -1;
            //						int mcGrandmotherPdg4  = -1;

            CbmMCTrack* grandmother1 = nullptr;

            if (motherId1 != -1) {
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              grandmotherId1 = mother1->GetMotherId();
              if (grandmotherId1 != -1) {
                grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
                if (NULL != grandmother1)
                  mcGrandmotherPdg1 = grandmother1->GetPdgCode();
              }
            }
            if (motherId2 != -1) {
              CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
              //							if (NULL != mother2) mcMotherPdg2 = mother2->GetPdgCode(); (FU) not used
              grandmotherId2 = mother2->GetMotherId();
              if (grandmotherId2 != -1) {
                //								CbmMCTrack* grandmother2 = (CbmMCTrack*) fMcTracks->At(grandmotherId2);
                //								if (NULL != grandmother2) mcGrandmotherPdg2 = grandmother2->GetPdgCode();
              }
            }
            if (motherId3 != -1) {
              CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
              //							if (NULL != mother3) mcMotherPdg3 = mother3->GetPdgCode(); (FU) not used
              grandmotherId3 = mother3->GetMotherId();
              if (grandmotherId3 != -1) {
                //								CbmMCTrack* grandmother3 = (CbmMCTrack*) fMcTracks->At(grandmotherId3);
                //								if (NULL != grandmother3) mcGrandmotherPdg3 = grandmother3->GetPdgCode();
              }
            }
            if (motherId4 != -1) {
              CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
              //							if (NULL != mother4) mcMotherPdg4 = mother4->GetPdgCode(); (FU) not used
              grandmotherId4 = mother4->GetMotherId();
              if (grandmotherId4 != -1) {
                //								CbmMCTrack* grandmother4 = (CbmMCTrack*) fMcTracks->At(grandmotherId4);
                //								if (NULL != grandmother4) mcGrandmotherPdg4 = grandmother4->GetPdgCode();
              }
            }

            if (grandmotherId1 == -1) continue;

            if (motherId1 == motherId2 && motherId3 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId3) != 2)
                continue;
              if ((grandmotherId1 == motherId3 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId3 && mcMotherPdg1 == 111)) {
                /*
								fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
								*/
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_2rich_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_2rich_invmass_all_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_2rich_pt_vs_rap_gee->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
                fhTest2_2rich_pt_vs_rap_all->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
              }
            }
            if (motherId1 == motherId3 && motherId2 == motherId4) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2)
                continue;
              if ((grandmotherId1 == motherId2 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {
                /*
								fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
								*/
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_2rich_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_2rich_invmass_all_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_2rich_pt_vs_rap_gee->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
                fhTest2_2rich_pt_vs_rap_all->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
              }
            }
            if (motherId1 == motherId4 && motherId2 == motherId3) {
              if (NofDaughters(motherId1) != 2 || NofDaughters(motherId2) != 2)
                continue;
              if ((grandmotherId1 == motherId2 && mcGrandmotherPdg1 == 111)
                  || (motherId1 == grandmotherId2 && mcMotherPdg1 == 111)) {
                /*
								fhTest2_startvertexElectrons_gee->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_gee->Fill(pi0start_l.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
								fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
								*/
                // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
                if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1
                    || pi0start_k.Z() > 1 || pi0start_l.Z() > 1)
                  continue;


                Double_t invmass_mc   = 0;
                Double_t invmass_reco = 0;
                CbmAnaConversionKinematicParams params_mc =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_MC(fVector_mctrack[i],
                                                  fVector_mctrack[j],
                                                  fVector_mctrack[k],
                                                  fVector_mctrack[l]);
                CbmAnaConversionKinematicParams params_reco =
                  CbmAnaConversionKinematicParams::
                    KinematicParams_4particles_Reco(fVector_momenta[i],
                                                    fVector_momenta[j],
                                                    fVector_momenta[k],
                                                    fVector_momenta[l]);
                invmass_mc   = params_mc.fMinv;
                invmass_reco = params_reco.fMinv;

                fhTest2_2rich_invmass_gee_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_gee_refitted->Fill(invmass_reco);
                fhTest2_2rich_invmass_all_mc->Fill(invmass_mc);
                fhTest2_2rich_invmass_all_refitted->Fill(invmass_reco);

                fhTest2_2rich_pt_vs_rap_gee->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
                fhTest2_2rich_pt_vs_rap_all->Fill(params_reco.fPt,
                                                  params_reco.fRapidity);
              }
            }


            // ===================================================================================================
            // HERE DECAY pi0 -> gamma gamma -> e+e- e+e-
            if (grandmotherId1 == grandmotherId2
                && grandmotherId1 == grandmotherId3
                && grandmotherId1 == grandmotherId4) {
              if (mcGrandmotherPdg1 != 111) continue;  // 111 = pi0, 221 = eta

              TVector3 pi0start;
              grandmother1->GetStartVertex(pi0start);
              /*
						fhTest2_startvertexElectrons_gg->Fill(pi0start_i.Z());
						fhTest2_startvertexElectrons_gg->Fill(pi0start_j.Z());
						fhTest2_startvertexElectrons_gg->Fill(pi0start_k.Z());
						fhTest2_startvertexElectrons_gg->Fill(pi0start_l.Z());
						fhTest2_startvertexElectrons_all->Fill(pi0start_i.Z());
						fhTest2_startvertexElectrons_all->Fill(pi0start_j.Z());
						fhTest2_startvertexElectrons_all->Fill(pi0start_k.Z());
						fhTest2_startvertexElectrons_all->Fill(pi0start_l.Z());
						*/

              // consider only electrons from the target (only then the momenta are correctly refitted/reconstructed)
              if (pi0start_i.Z() > 1 || pi0start_j.Z() > 1 || pi0start_k.Z() > 1
                  || pi0start_l.Z() > 1)
                continue;

              Double_t invmass_mc   = 0;
              Double_t invmass_reco = 0;
              CbmAnaConversionKinematicParams params_mc =
                CbmAnaConversionKinematicParams::KinematicParams_4particles_MC(
                  fVector_mctrack[i],
                  fVector_mctrack[j],
                  fVector_mctrack[k],
                  fVector_mctrack[l]);
              CbmAnaConversionKinematicParams params_reco =
                CbmAnaConversionKinematicParams::
                  KinematicParams_4particles_Reco(fVector_momenta[i],
                                                  fVector_momenta[j],
                                                  fVector_momenta[k],
                                                  fVector_momenta[l]);
              invmass_mc   = params_mc.fMinv;
              invmass_reco = params_reco.fMinv;

              fhTest2_2rich_invmass_gg_mc->Fill(invmass_mc);
              fhTest2_2rich_invmass_gg_refitted->Fill(invmass_reco);
              fhTest2_2rich_invmass_all_mc->Fill(invmass_mc);
              fhTest2_2rich_invmass_all_refitted->Fill(invmass_reco);

              fhTest2_2rich_pt_vs_rap_gg->Fill(params_reco.fPt,
                                               params_reco.fRapidity);
              fhTest2_2rich_pt_vs_rap_all->Fill(params_reco.fPt,
                                                params_reco.fRapidity);

              cout << "########################################################"
                      "##############"
                   << endl;

              fill++;
            }
          }
        }
      }
    }
  }
}


Int_t CbmAnaConversionTest2::NofDaughters(Int_t motherId) {
  Int_t nofDaughters = 0;
  for (unsigned int i = 0; i < fVector_mctrack.size(); i++) {
    Int_t motherId_temp = fVector_mctrack[i]->GetMotherId();
    if (motherId == motherId_temp) nofDaughters++;
  }


  return nofDaughters;
}
