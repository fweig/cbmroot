/**
 * \file CbmAnaConversionRecoFull.cxx
 *
 * 
 * 
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/

#include "CbmAnaConversionRecoFull.h"

// standard includes
#include <iostream>

// includes from ROOT
#include "TRandom3.h"

// included from CbmRoot
#include "CbmAnaConversionCutSettings.h"
#include "CbmAnaConversionGlobalFunctions.h"
#include "CbmAnaConversionKinematicParams.h"
#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmL1PFFitter.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmMCTrack.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include <Logger.h>
#include "FairRootManager.h"
#include "L1Field.h"


#define M2E 2.6112004954086e-7
using namespace std;


CbmAnaConversionRecoFull::CbmAnaConversionRecoFull()
  : fRichPoints(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fMcTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fGlobalTracks(NULL)
  , fPrimVertex(NULL)
  , fKFVertex()
  , fHistoList_recofull()
  , fHistoList_recofull_1()
  , fHistoList_recofull_2()
  , fHistoList_recofull_3()
  , fHistoList_recofull_4()
  , fHistoList_recofull_new()
  , fhElectrons(NULL)
  , fhMomentumFits(NULL)
  , fhMomentumFits_electronRich(NULL)
  , fhMomentumFits_pi0reco(NULL)
  , fElectrons_track()
  , fElectrons_momenta()
  , fElectrons_momentaChi()
  , fElectrons_mctrackID()
  , fElectrons_track_1()
  , fElectrons_momenta_1()
  , fElectrons_momentaChi_1()
  , fElectrons_mctrackID_1()
  , fVector_photons_pairs_1()
  , fElectrons_track_2()
  , fElectrons_momenta_2()
  , fElectrons_momentaChi_2()
  , fElectrons_mctrackID_2()
  , fVector_photons_pairs_2()
  , fElectrons_track_3()
  , fElectrons_momenta_3()
  , fElectrons_momentaChi_3()
  , fElectrons_mctrackID_3()
  , fVector_photons_pairs_3()
  , fElectrons_track_4()
  , fElectrons_momenta_4()
  , fElectrons_momentaChi_4()
  , fElectrons_mctrackID_4()
  , fVector_photons_pairs_4()
  , fElectrons_track_new()
  , fElectrons_momenta_new()
  , fElectrons_momentaChi_new()
  , fElectrons_mctrackID_new()
  , fElectrons_globaltrackID_new()
  , fVector_photons_pairs_new()
  , fhElectrons_invmass(NULL)
  , fhElectrons_invmass_cut(NULL)
  , fhElectrons_nofPerEvent(NULL)
  , fhPhotons_nofPerEvent(NULL)
  , fVector_photons_pairs()
  , fVector_photons_momenta()
  , fhPhotons_invmass(NULL)
  , fhPhotons_invmass_cut(NULL)
  , fhPhotons_invmass_cut_chi1(NULL)
  , fhPhotons_invmass_cut_chi3(NULL)
  , fhPhotons_invmass_cut_chi5(NULL)
  , fhPhotons_invmass_cut_chi10(NULL)
  , fhPhotons_invmass_cut_chi25(NULL)
  , fhPhotons_invmass_cut_chi40(NULL)
  , fhPhotons_invmass_cut_chi65(NULL)
  , fhPhotons_invmass_cut_chi80(NULL)
  , fhPhotons_invmass_cut_ptBin1(NULL)
  , fhPhotons_invmass_cut_ptBin2(NULL)
  , fhPhotons_invmass_cut_ptBin3(NULL)
  , fhPhotons_invmass_cut_ptBin4(NULL)
  , fhPhotons_invmass_vs_chi(NULL)
  , fhPhotons_startvertex_vs_chi(NULL)
  , fhPhotons_angleBetween(NULL)
  , fhPhotons_invmass_vs_pt(NULL)
  , fhPhotons_rapidity_vs_pt(NULL)
  , fhPhotons_invmass_vs_openingAngle(NULL)
  , fhPhotons_openingAngle_vs_momentum(NULL)
  , fhPhotons_MC_motherpdg(NULL)
  , fhPhotons_MC_invmass1(NULL)
  , fhPhotons_MC_invmass2(NULL)
  , fhPhotons_MC_invmass3(NULL)
  , fhPhotons_MC_invmass4(NULL)
  , fhPhotons_MC_startvertexZ(NULL)
  , fhPhotons_MC_motherIdCut(NULL)
  , fhPhotons_Refit_chiDiff(NULL)
  , fhPhotons_Refit_momentumDiff(NULL)
  , fhPhotons_Refit_chiDistribution(NULL)
  , fhPhotons_RefitPion_chiDistribution(NULL)
  , fElectrons_track_refit()
  , fElectrons_momenta_refit()
  , fVector_photons_pairs_refit()
  , fhPhotons_invmass_refit(NULL)
  , fhPhotons_invmass_refit_cut(NULL)
  , fVector_photons_pairs_direction()
  , fhPhotons_invmass_direction(NULL)
  , fhPhotons_invmass_direction_cut(NULL)
  , fhPhotons_boostAngle(NULL)
  , fhPhotons_boostAngleMC(NULL)
  , fhPhotons_boostAngleTest(NULL)
  , fhPhotons_tX(NULL)
  , fhPhotons_tY(NULL)
  , fhElectrons_nofPerEvent_1(NULL)
  , fhPhotons_nofPerEvent_1(NULL)
  , fhPhotons_invmass_1(NULL)
  , fhPhotons_invmass_ptBin1_1(NULL)
  , fhPhotons_invmass_ptBin2_1(NULL)
  , fhPhotons_invmass_ptBin3_1(NULL)
  , fhPhotons_invmass_ptBin4_1(NULL)
  , fhElectrons_invmass_1(NULL)
  , fhPhotons_invmass_vs_pt_1(NULL)
  , fhElectrons_nofPerEvent_2(NULL)
  , fhPhotons_nofPerEvent_2(NULL)
  , fhPhotons_invmass_2(NULL)
  , fhPhotons_invmass_ptBin1_2(NULL)
  , fhPhotons_invmass_ptBin2_2(NULL)
  , fhPhotons_invmass_ptBin3_2(NULL)
  , fhPhotons_invmass_ptBin4_2(NULL)
  , fhElectrons_invmass_2(NULL)
  , fhPhotons_invmass_vs_pt_2(NULL)
  , fhElectrons_nofPerEvent_3(NULL)
  , fhPhotons_nofPerEvent_3(NULL)
  , fhPhotons_invmass_3(NULL)
  , fhPhotons_invmass_ptBin1_3(NULL)
  , fhPhotons_invmass_ptBin2_3(NULL)
  , fhPhotons_invmass_ptBin3_3(NULL)
  , fhPhotons_invmass_ptBin4_3(NULL)
  , fhElectrons_invmass_3(NULL)
  , fhPhotons_invmass_vs_pt_3(NULL)
  , fhElectrons_nofPerEvent_4(NULL)
  , fhPhotons_nofPerEvent_4(NULL)
  , fhPhotons_invmass_4(NULL)
  , fhPhotons_invmass_ptBin1_4(NULL)
  , fhPhotons_invmass_ptBin2_4(NULL)
  , fhPhotons_invmass_ptBin3_4(NULL)
  , fhPhotons_invmass_ptBin4_4(NULL)
  , fhElectrons_invmass_4(NULL)
  , fhPhotons_invmass_vs_pt_4(NULL)
  , fhElectrons_nofPerEvent_new()
  , fhPhotons_nofPerEvent_new()
  , fhPhotons_nofPerEventAfter_new()
  , fhPhotons_invmass_new()
  , fhPhotons_invmass_ann0_new()
  , fhPhotons_invmass_ptBin1_new()
  , fhPhotons_invmass_ptBin2_new()
  , fhPhotons_invmass_ptBin3_new()
  , fhPhotons_invmass_ptBin4_new()
  , fhElectrons_invmass_new()
  , fhPhotons_invmass_vs_pt_new()
  , fhPhotons_invmass_MCcut1_new()
  , fhPhotons_invmass_MCcut2_new()
  , fhPhotons_invmass_MCcut3_new()
  , fhPhotons_invmass_MCcut4_new()
  , fhPhotons_invmass_MCcut5_new()
  , fhPhotons_invmass_MCcut6_new()
  , fhPhotons_invmass_MCcut7_new()
  , fhPhotons_invmass_MCcutTest_new()
  , fhPhotons_invmass_MCcutTest2_new()
  , fhPhotons_invmass_MCcutTest3_new()
  , fhPhotons_invmass_MCcutAll_new()
  , fhPhotons_pt_vs_rap_new()
  , fhElectrons_openingAngle_sameSign_new()
  , fhPhotons_stats()
  , fhPhotons_MCtrue_pdgCodes()
  , fhPhotons_peakCheck1()
  , fhPhotons_peakCheck2()
  , fhPhotons_invmass_ANNcuts_new()
  , fhPhotons_phaseSpace_pi0()
  , fhPhotons_phaseSpace_eta()
  , fMixedEventsElectrons_list1()
  , fMixedEventsElectrons_list2()
  , fMixedEventsElectrons_list3()
  , fMixedEventsElectrons_list4()
  , fMixedEventsElectrons_list1_gtrack()
  , fMixedEventsElectrons_list2_gtrack()
  , fMixedEventsElectrons_list3_gtrack()
  , fMixedEventsElectrons_list4_gtrack()
  , fhMixedEventsTest2_invmass(NULL)
  , fMixedTest3_momenta()
  , fMixedTest3_gtrack()
  , fMixedTest3_eventno()
  , globalEventNo()
  , fhMixedEventsTest3_invmass(NULL)
  , fMixedTest4_photons()
  , fMixedTest4_mctracks()
  , fMixedTest4_isRichElectronAnn0()
  , fMixedTest4_ElectronAnns()
  , fMixedTest4_eventno()
  , fhMixedEventsTest4_invmass(NULL)
  , fhMixedEventsTest4_invmass_ann0(NULL)
  , fhMixedEventsTest4_pt_vs_rap(NULL)
  , fhMixedEventsTest4_invmass_ptBin1(NULL)
  , fhMixedEventsTest4_invmass_ptBin2(NULL)
  , fhMixedEventsTest4_invmass_ptBin3(NULL)
  , fhMixedEventsTest4_invmass_ptBin4(NULL)
  , fhMixedEventsTest4_invmass_ANNcuts(NULL)
  , timer()
  , fTime(0.) {}

CbmAnaConversionRecoFull::~CbmAnaConversionRecoFull() {}


void CbmAnaConversionRecoFull::Init() {
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
  CbmLitGlobalElectronId::GetInstance().SetRichAnnCut(-0.8);

  globalEventNo = 0;
}


void CbmAnaConversionRecoFull::InitHistos() {
  fHistoList_recofull.clear();
  fHistoList_recofull_1.clear();
  fHistoList_recofull_2.clear();
  fHistoList_recofull_3.clear();
  fHistoList_recofull_4.clear();

  for (int i = 1; i <= 4; i++) {
    fHistoList_recofull_new[i].clear();
  }

  Double_t invmassSpectra_nof   = 800;
  Double_t invmassSpectra_start = -0.00125;
  Double_t invmassSpectra_end   = 1.99875;


  fhElectrons = new TH1I("fhElectrons", "fhElectrons;; #", 8, 0., 8.);
  fHistoList_recofull.push_back(fhElectrons);
  fhElectrons->GetXaxis()->SetBinLabel(1, "electrons rich");
  fhElectrons->GetXaxis()->SetBinLabel(2, "electrons trd");
  fhElectrons->GetXaxis()->SetBinLabel(3, "electrons tof");
  fhElectrons->GetXaxis()->SetBinLabel(4, "electrons rich+trd");
  fhElectrons->GetXaxis()->SetBinLabel(5, "electrons rich+tof");
  fhElectrons->GetXaxis()->SetBinLabel(6, "electrons trd+tof");
  fhElectrons->GetXaxis()->SetBinLabel(7, "electrons rich+trd+tof");
  fhElectrons->GetXaxis()->SetBinLabel(8, "electrons 2 out of 3");


  fhMomentumFits =
    new TH1D("fhMomentumFits", "fhMomentumFits; chi; #", 10000, 0., 1000.);
  fHistoList_recofull.push_back(fhMomentumFits);
  fhMomentumFits_electronRich = new TH1D("fhMomentumFits_electronRich",
                                         "fhMomentumFits_electronRich; chi; #",
                                         10000,
                                         0.,
                                         1000.);
  fHistoList_recofull.push_back(fhMomentumFits_electronRich);
  fhMomentumFits_pi0reco = new TH1D("fhMomentumFits_pi0reco",
                                    "fhMomentumFits_pi0reco; chi; #",
                                    10000,
                                    0.,
                                    1000.);
  fHistoList_recofull.push_back(fhMomentumFits_pi0reco);


  fhElectrons_invmass = new TH1D("fhElectrons_invmass",
                                 "fhElectrons_invmass; invariant mass; #",
                                 invmassSpectra_nof,
                                 invmassSpectra_start,
                                 invmassSpectra_end);
  fhElectrons_invmass_cut =
    new TH1D("fhElectrons_invmass_cut",
             "fhElectrons_invmass_cut; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhElectrons_invmass);
  fHistoList_recofull.push_back(fhElectrons_invmass_cut);

  fhElectrons_nofPerEvent =
    new TH1D("fhElectrons_nofPerEvent",
             "fhElectrons_nofPerEvent; nof electrons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull.push_back(fhElectrons_nofPerEvent);
  fhPhotons_nofPerEvent =
    new TH1D("fhPhotons_nofPerEvent",
             "fhPhotons_nofPerEvent; nof photons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull.push_back(fhPhotons_nofPerEvent);

  fhPhotons_invmass = new TH1D("fhPhotons_invmass",
                               "fhPhotons_invmass; invariant mass; #",
                               invmassSpectra_nof,
                               invmassSpectra_start,
                               invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass);
  fhPhotons_invmass_cut = new TH1D("fhPhotons_invmass_cut",
                                   "fhPhotons_invmass_cut; invariant mass; #",
                                   invmassSpectra_nof,
                                   invmassSpectra_start,
                                   invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut);


  fhPhotons_invmass_cut_ptBin1 =
    new TH1D("fhPhotons_invmass_cut_ptBin1",
             "fhPhotons_invmass_cut_ptBin1; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_ptBin1);
  fhPhotons_invmass_cut_ptBin2 =
    new TH1D("fhPhotons_invmass_cut_ptBin2",
             "fhPhotons_invmass_cut_ptBin2; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_ptBin2);
  fhPhotons_invmass_cut_ptBin3 =
    new TH1D("fhPhotons_invmass_cut_ptBin3",
             "fhPhotons_invmass_cut_ptBin3; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_ptBin3);
  fhPhotons_invmass_cut_ptBin4 =
    new TH1D("fhPhotons_invmass_cut_ptBin4",
             "fhPhotons_invmass_cut_ptBin4; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_ptBin4);


  fhPhotons_angleBetween =
    new TH1D("fhPhotons_angleBetween",
             "fhPhotons_angleBetween; opening angle [deg]; #",
             1000,
             0,
             100);
  fHistoList_recofull.push_back(fhPhotons_angleBetween);


  fhPhotons_invmass_cut_chi1 =
    new TH1D("fhPhotons_invmass_cut_chi1",
             "fhPhotons_invmass_cut_chi1; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi1);
  fhPhotons_invmass_cut_chi3 =
    new TH1D("fhPhotons_invmass_cut_chi3",
             "fhPhotons_invmass_cut_chi3; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi3);
  fhPhotons_invmass_cut_chi5 =
    new TH1D("fhPhotons_invmass_cut_chi5",
             "fhPhotons_invmass_cut_chi5; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi5);
  fhPhotons_invmass_cut_chi10 =
    new TH1D("fhPhotons_invmass_cut_chi10",
             "fhPhotons_invmass_cut_chi10; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi10);
  fhPhotons_invmass_cut_chi25 =
    new TH1D("fhPhotons_invmass_cut_chi25",
             "fhPhotons_invmass_cut_chi25; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi25);
  fhPhotons_invmass_cut_chi40 =
    new TH1D("fhPhotons_invmass_cut_chi40",
             "fhPhotons_invmass_cut_chi40; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi40);
  fhPhotons_invmass_cut_chi65 =
    new TH1D("fhPhotons_invmass_cut_chi65",
             "fhPhotons_invmass_cut_chi65; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi65);
  fhPhotons_invmass_cut_chi80 =
    new TH1D("fhPhotons_invmass_cut_chi80",
             "fhPhotons_invmass_cut_chi80; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_cut_chi80);

  fhPhotons_invmass_vs_chi =
    new TH2D("fhPhotons_invmass_vs_chi",
             "fhPhotons_invmass_vs_chi; invariant mass; chi",
             600,
             -0.0025,
             2.9975,
             1000,
             0.,
             100.);
  fHistoList_recofull.push_back(fhPhotons_invmass_vs_chi);
  fhPhotons_startvertex_vs_chi =
    new TH2D("fhPhotons_startvertex_vs_chi",
             "fhPhotons_startvertex_vs_chi; startvertex; chi",
             400,
             -5.,
             95.,
             1000,
             0.,
             200.);
  fHistoList_recofull.push_back(fhPhotons_startvertex_vs_chi);
  fhPhotons_invmass_vs_pt = new TH2D("fhPhotons_invmass_vs_pt",
                                     "fhPhotons_invmass_vs_pt; invmass; pt",
                                     600,
                                     -0.0025,
                                     2.9975,
                                     100,
                                     0.,
                                     10.);
  fHistoList_recofull.push_back(fhPhotons_invmass_vs_pt);
  fhPhotons_rapidity_vs_pt =
    new TH2D("fhPhotons_rapidity_vs_pt",
             "fhPhotons_rapidity_vs_pt; pt [GeV]; rap [GeV]",
             240,
             -2.,
             10.,
             210,
             0.,
             7.);
  fHistoList_recofull.push_back(fhPhotons_rapidity_vs_pt);

  fhPhotons_invmass_vs_openingAngle =
    new TH2D("fhPhotons_invmass_vs_openingAngle",
             "fhPhotons_invmass_vs_openingAngle; invmass; openingAngle",
             600,
             -0.0025,
             2.9975,
             100,
             0.,
             10.);
  fHistoList_recofull.push_back(fhPhotons_invmass_vs_openingAngle);
  fhPhotons_openingAngle_vs_momentum =
    new TH2D("fhPhotons_openingAngle_vs_momentum",
             "fhPhotons_openingAngle_vs_momentum; openingAngle; momentum",
             100,
             0.,
             10.,
             100,
             0.,
             10.);
  fHistoList_recofull.push_back(fhPhotons_openingAngle_vs_momentum);


  fhPhotons_MC_motherpdg = new TH1D("fhPhotons_MC_motherpdg",
                                    "fhPhotons_MC_motherpdg; pdg; #",
                                    1000,
                                    0.,
                                    1000.);
  fHistoList_recofull.push_back(fhPhotons_MC_motherpdg);
  fhPhotons_MC_invmass1 =
    new TH1D("fhPhotons_MC_invmass1",
             "fhPhotons_MC_minvmass1 (MC-true cut: at least gamma correct "
             "reconstructed); invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_MC_invmass1);
  fhPhotons_MC_invmass2 = new TH1D("fhPhotons_MC_invmass2",
                                   "fhPhotons_MC_minvmass2 (MC-true cut: gamma "
                                   "from same mother); invariant mass; #",
                                   invmassSpectra_nof,
                                   invmassSpectra_start,
                                   invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_MC_invmass2);
  fhPhotons_MC_invmass3 = new TH1D("fhPhotons_MC_invmass3",
                                   "fhPhotons_MC_minvmass3 (MC-true cut: gamma "
                                   "from different mother); invariant mass; #",
                                   invmassSpectra_nof,
                                   invmassSpectra_start,
                                   invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_MC_invmass3);
  fhPhotons_MC_invmass4 =
    new TH1D("fhPhotons_MC_invmass4",
             "fhPhotons_MC_minvmass4 (MC-true cut: wrong combination of "
             "electrons); invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_MC_invmass4);

  fhPhotons_MC_startvertexZ =
    new TH1D("fhPhotons_MC_startvertexZ",
             "fhPhotons_MC_startvertexZ; startvertexZ [cm]; #",
             1000,
             -5.,
             95.);
  fHistoList_recofull.push_back(fhPhotons_MC_startvertexZ);

  fhPhotons_MC_motherIdCut =
    new TH1D("fhPhotons_MC_motherIdCut",
             "fhPhotons_MC_motherIdCut; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_MC_motherIdCut);

  fhPhotons_Refit_chiDiff =
    new TH1D("fhPhotons_Refit_chiDiff",
             "fhPhotons_Refit_chiDiff; difference of chi; #",
             1000,
             -0.1,
             99.9);
  fHistoList_recofull.push_back(fhPhotons_Refit_chiDiff);

  fhPhotons_Refit_momentumDiff =
    new TH1D("fhPhotons_Refit_momentumDiff",
             "fhPhotons_Refit_momentumDiff; difference of momentum mag; #",
             1000,
             -0.1,
             4.9);
  fHistoList_recofull.push_back(fhPhotons_Refit_momentumDiff);

  fhPhotons_Refit_chiDistribution =
    new TH1D("fhPhotons_Refit_chiDistribution",
             "fhPhotons_Refit_chiDistribution; chi value; #",
             10000,
             0.,
             1000.);
  fHistoList_recofull.push_back(fhPhotons_Refit_chiDistribution);

  fhPhotons_RefitPion_chiDistribution =
    new TH1D("fhPhotons_RefitPion_chiDistribution",
             "fhPhotons_RefitPion_chiDistribution; chi value; #",
             10000,
             0.,
             1000.);
  fHistoList_recofull.push_back(fhPhotons_RefitPion_chiDistribution);

  fhPhotons_invmass_refit =
    new TH1D("fhPhotons_invmass_refit",
             "fhPhotons_invmass_refit; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_refit);
  fhPhotons_invmass_refit_cut =
    new TH1D("fhPhotons_invmass_refit_cut",
             "fhPhotons_invmass_refit_cut; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_refit_cut);


  fhPhotons_invmass_direction =
    new TH1D("fhPhotons_invmass_direction",
             "fhPhotons_invmass_direction; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_direction);
  fhPhotons_invmass_direction_cut =
    new TH1D("fhPhotons_invmass_direction_cut",
             "fhPhotons_invmass_direction_cut; invariant mass; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull.push_back(fhPhotons_invmass_direction_cut);
  fhPhotons_boostAngle = new TH1D("fhPhotons_boostAngle",
                                  "fhPhotons_boostAngle; boostAngle; #",
                                  7400,
                                  -370.,
                                  370.);
  fHistoList_recofull.push_back(fhPhotons_boostAngle);
  fhPhotons_boostAngleMC = new TH1D("fhPhotons_boostAngleMC",
                                    "fhPhotons_boostAngleMC; boostAngle; #",
                                    7400,
                                    -370.,
                                    370.);
  fHistoList_recofull.push_back(fhPhotons_boostAngleMC);
  fhPhotons_boostAngleTest = new TH1D("fhPhotons_boostAngleTest",
                                      "fhPhotons_boostAngleTest; boostAngle; #",
                                      10000,
                                      -5.,
                                      5.);
  fHistoList_recofull.push_back(fhPhotons_boostAngleTest);


  fhPhotons_tX =
    new TH1D("fhPhotons_tX", "fhPhotons_tX; tX; #", 201, -1.005, 1.005);
  fHistoList_recofull.push_back(fhPhotons_tX);
  fhPhotons_tY =
    new TH1D("fhPhotons_tY", "fhPhotons_tY; tY; #", 201, -1.005, 1.005);
  fHistoList_recofull.push_back(fhPhotons_tY);


  // histograms for index = 1 (no refit, no chicut)
  fhElectrons_nofPerEvent_1 =
    new TH1D("fhElectrons_nofPerEvent_1",
             "fhElectrons_nofPerEvent_1; nof electrons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_1.push_back(fhElectrons_nofPerEvent_1);
  fhPhotons_nofPerEvent_1 =
    new TH1D("fhPhotons_nofPerEvent_1",
             "fhPhotons_nofPerEvent_1; nof photons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_1.push_back(fhPhotons_nofPerEvent_1);

  fhPhotons_invmass_1 = new TH1D("fhPhotons_invmass_1",
                                 "fhPhotons_invmass_1; invariant mass; #",
                                 600,
                                 -0.0025,
                                 2.9975);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_1);

  fhPhotons_invmass_ptBin1_1 =
    new TH1D("fhPhotons_invmass_ptBin1_1",
             "fhPhotons_invmass_ptBin1_1; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_ptBin1_1);
  fhPhotons_invmass_ptBin2_1 =
    new TH1D("fhPhotons_invmass_ptBin2_1",
             "fhPhotons_invmass_ptBin2_1; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_ptBin2_1);
  fhPhotons_invmass_ptBin3_1 =
    new TH1D("fhPhotons_invmass_ptBin3_1",
             "fhPhotons_invmass_ptBin3_1; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_ptBin3_1);
  fhPhotons_invmass_ptBin4_1 =
    new TH1D("fhPhotons_invmass_ptBin4_1",
             "fhPhotons_invmass_ptBin4_1; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_ptBin4_1);

  fhElectrons_invmass_1 = new TH1D("fhElectrons_invmass_1",
                                   "fhElectrons_invmass_1; invariant mass; #",
                                   600,
                                   -0.0025,
                                   2.9975);
  fHistoList_recofull_1.push_back(fhElectrons_invmass_1);
  fhPhotons_invmass_vs_pt_1 = new TH2D("fhPhotons_invmass_vs_pt_1",
                                       "fhPhotons_invmass_vs_pt_1; invmass; pt",
                                       600,
                                       -0.0025,
                                       2.9975,
                                       100,
                                       0.,
                                       10.);
  fHistoList_recofull_1.push_back(fhPhotons_invmass_vs_pt_1);


  // histograms for index = 2 (no refit, with chicut)
  fhElectrons_nofPerEvent_2 =
    new TH1D("fhElectrons_nofPerEvent_2",
             "fhElectrons_nofPerEvent_2; nof electrons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_2.push_back(fhElectrons_nofPerEvent_2);
  fhPhotons_nofPerEvent_2 =
    new TH1D("fhPhotons_nofPerEvent_2",
             "fhPhotons_nofPerEvent_2; nof photons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_2.push_back(fhPhotons_nofPerEvent_2);

  fhPhotons_invmass_2 = new TH1D("fhPhotons_invmass_2",
                                 "fhPhotons_invmass_2; invariant mass; #",
                                 600,
                                 -0.0025,
                                 2.9975);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_2);

  fhPhotons_invmass_ptBin1_2 =
    new TH1D("fhPhotons_invmass_ptBin1_2",
             "fhPhotons_invmass_ptBin1_2; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_ptBin1_2);
  fhPhotons_invmass_ptBin2_2 =
    new TH1D("fhPhotons_invmass_ptBin2_2",
             "fhPhotons_invmass_ptBin2_2; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_ptBin2_2);
  fhPhotons_invmass_ptBin3_2 =
    new TH1D("fhPhotons_invmass_ptBin3_2",
             "fhPhotons_invmass_ptBin3_2; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_ptBin3_2);
  fhPhotons_invmass_ptBin4_2 =
    new TH1D("fhPhotons_invmass_ptBin4_2",
             "fhPhotons_invmass_ptBin4_2; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_ptBin4_2);

  fhElectrons_invmass_2 = new TH1D("fhElectrons_invmass_2",
                                   "fhElectrons_invmass_2; invariant mass; #",
                                   600,
                                   -0.0025,
                                   2.9975);
  fHistoList_recofull_2.push_back(fhElectrons_invmass_2);
  fhPhotons_invmass_vs_pt_2 = new TH2D("fhPhotons_invmass_vs_pt_2",
                                       "fhPhotons_invmass_vs_pt_2; invmass; pt",
                                       600,
                                       -0.0025,
                                       2.9975,
                                       100,
                                       0.,
                                       10.);
  fHistoList_recofull_2.push_back(fhPhotons_invmass_vs_pt_2);


  // histograms for index = 3 (with refit, no chicut)
  fhElectrons_nofPerEvent_3 =
    new TH1D("fhElectrons_nofPerEvent_3",
             "fhElectrons_nofPerEvent_3; nof electrons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_3.push_back(fhElectrons_nofPerEvent_3);
  fhPhotons_nofPerEvent_3 =
    new TH1D("fhPhotons_nofPerEvent_3",
             "fhPhotons_nofPerEvent_3; nof photons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_3.push_back(fhPhotons_nofPerEvent_3);

  fhPhotons_invmass_3 = new TH1D("fhPhotons_invmass_3",
                                 "fhPhotons_invmass_3; invariant mass; #",
                                 600,
                                 -0.0025,
                                 2.9975);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_3);

  fhPhotons_invmass_ptBin1_3 =
    new TH1D("fhPhotons_invmass_ptBin1_3",
             "fhPhotons_invmass_ptBin1_3; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_ptBin1_3);
  fhPhotons_invmass_ptBin2_3 =
    new TH1D("fhPhotons_invmass_ptBin2_3",
             "fhPhotons_invmass_ptBin2_3; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_ptBin2_3);
  fhPhotons_invmass_ptBin3_3 =
    new TH1D("fhPhotons_invmass_ptBin3_3",
             "fhPhotons_invmass_ptBin3_3; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_ptBin3_3);
  fhPhotons_invmass_ptBin4_3 =
    new TH1D("fhPhotons_invmass_ptBin4_3",
             "fhPhotons_invmass_ptBin4_3; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_ptBin4_3);

  fhElectrons_invmass_3 = new TH1D("fhElectrons_invmass_3",
                                   "fhElectrons_invmass_3; invariant mass; #",
                                   600,
                                   -0.0025,
                                   2.9975);
  fHistoList_recofull_3.push_back(fhElectrons_invmass_3);
  fhPhotons_invmass_vs_pt_3 = new TH2D("fhPhotons_invmass_vs_pt_3",
                                       "fhPhotons_invmass_vs_pt_3; invmass; pt",
                                       600,
                                       -0.0025,
                                       2.9975,
                                       100,
                                       0.,
                                       10.);
  fHistoList_recofull_3.push_back(fhPhotons_invmass_vs_pt_3);


  // histograms for index = 4 (with refit, with chicut)
  fhElectrons_nofPerEvent_4 =
    new TH1D("fhElectrons_nofPerEvent_4",
             "fhElectrons_nofPerEvent_4; nof electrons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_4.push_back(fhElectrons_nofPerEvent_4);
  fhPhotons_nofPerEvent_4 =
    new TH1D("fhPhotons_nofPerEvent_4",
             "fhPhotons_nofPerEvent_4; nof photons per event; #",
             101,
             -0.5,
             100.5);
  fHistoList_recofull_4.push_back(fhPhotons_nofPerEvent_4);

  fhPhotons_invmass_4 = new TH1D("fhPhotons_invmass_4",
                                 "fhPhotons_invmass_4; invariant mass; #",
                                 600,
                                 -0.0025,
                                 2.9975);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_4);

  fhPhotons_invmass_ptBin1_4 =
    new TH1D("fhPhotons_invmass_ptBin1_4",
             "fhPhotons_invmass_ptBin1_4; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_ptBin1_4);
  fhPhotons_invmass_ptBin2_4 =
    new TH1D("fhPhotons_invmass_ptBin2_4",
             "fhPhotons_invmass_ptBin2_4; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_ptBin2_4);
  fhPhotons_invmass_ptBin3_4 =
    new TH1D("fhPhotons_invmass_ptBin3_4",
             "fhPhotons_invmass_ptBin3_4; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_ptBin3_4);
  fhPhotons_invmass_ptBin4_4 =
    new TH1D("fhPhotons_invmass_ptBin4_4",
             "fhPhotons_invmass_ptBin4_4; invariant mass; #",
             600,
             -0.0025,
             2.9975);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_ptBin4_4);

  fhElectrons_invmass_4 = new TH1D("fhElectrons_invmass_4",
                                   "fhElectrons_invmass_4; invariant mass; #",
                                   600,
                                   -0.0025,
                                   2.9975);
  fHistoList_recofull_4.push_back(fhElectrons_invmass_4);
  fhPhotons_invmass_vs_pt_4 = new TH2D("fhPhotons_invmass_vs_pt_4",
                                       "fhPhotons_invmass_vs_pt_4; invmass; pt",
                                       600,
                                       -0.0025,
                                       2.9975,
                                       100,
                                       0.,
                                       10.);
  fHistoList_recofull_4.push_back(fhPhotons_invmass_vs_pt_4);


  for (int i = 1; i <= 4; i++) {
    fhElectrons_nofPerEvent_new[i] = new TH1D(
      Form("fhElectrons_nofPerEvent_new_%i", i),
      Form("fhElectrons_nofPerEvent_new_%i; nof electrons per event; #", i),
      31,
      -0.5,
      30.5);
    fHistoList_recofull_new[i].push_back(fhElectrons_nofPerEvent_new[i]);
    fhPhotons_nofPerEvent_new[i] = new TH1D(
      Form("fhPhotons_nofPerEvent_new_%i", i),
      Form("fhPhotons_nofPerEvent_new_%i; nof photons per event; #", i),
      31,
      -0.5,
      30.5);
    fHistoList_recofull_new[i].push_back(fhPhotons_nofPerEvent_new[i]);
    fhPhotons_nofPerEventAfter_new[i] = new TH1D(
      Form("fhPhotons_nofPerEventAfter_new_%i", i),
      Form("fhPhotons_nofPerEventAfter_new_%i; nof photons per event; #", i),
      31,
      -0.5,
      30.5);
    fHistoList_recofull_new[i].push_back(fhPhotons_nofPerEventAfter_new[i]);
    fhPi0_nofPerEvent_new[i] =
      new TH1D(Form("fhPi0_nofPerEvent_new_%i", i),
               Form("fhPi0_nofPerEvent_new_%i; nof pi0 per event; #", i),
               31,
               -0.5,
               30.5);
    fHistoList_recofull_new[i].push_back(fhPi0_nofPerEvent_new[i]);

    fhPhotons_invmass_new[i] = new TH1D(
      Form("fhPhotons_invmass_new_%i", i),
      Form(
        "fhPhotons_invmass_new_%i; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
        i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_new[i]);
    fhPhotons_invmass_ann0_new[i] =
      new TH1D(Form("fhPhotons_invmass_ann0_new_%i", i),
               Form("fhPhotons_invmass_ann0_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ann0_new[i]);

    fhPhotons_invmass_ptBin1_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin1_new_%i", i),
               Form("fhPhotons_invmass_ptBin1_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin1_new[i]);
    fhPhotons_invmass_ptBin2_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin2_new_%i", i),
               Form("fhPhotons_invmass_ptBin2_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin2_new[i]);
    fhPhotons_invmass_ptBin3_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin3_new_%i", i),
               Form("fhPhotons_invmass_ptBin3_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin3_new[i]);
    fhPhotons_invmass_ptBin4_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin4_new_%i", i),
               Form("fhPhotons_invmass_ptBin4_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin4_new[i]);
    fhPhotons_invmass_ptBin5_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin5_new_%i", i),
               Form("fhPhotons_invmass_ptBin5_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin5_new[i]);
    fhPhotons_invmass_ptBin6_new[i] =
      new TH1D(Form("fhPhotons_invmass_ptBin6_new_%i", i),
               Form("fhPhotons_invmass_ptBin6_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ptBin6_new[i]);

    fhElectrons_invmass_new[i] =
      new TH1D(Form("fhElectrons_invmass_new_%i", i),
               Form("fhElectrons_invmass_new_%i; invariant mass of 4 e^{#pm} "
                    "in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhElectrons_invmass_new[i]);

    fhPhotons_invmass_vs_pt_new[i] =
      new TH2D(Form("fhPhotons_invmass_vs_pt_new_%i", i),
               Form("fhPhotons_invmass_vs_pt_new_%i; invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}; pt",
                    i),
               600,
               -0.0025,
               2.9975,
               100,
               0.,
               10.);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_vs_pt_new[i]);

    fhPhotons_invmass_MCcut1_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut1_new_%i", i),
      Form(
        "fhPhotons_invmass_MCcut1_new_%i (MC-true cut: at least gamma correct "
        "reconstructed); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
        i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut1_new[i]);
    fhPhotons_invmass_MCcut2_new[i] =
      new TH1D(Form("fhPhotons_invmass_MCcut2_new_%i", i),
               Form("fhPhotons_invmass_MCcut2_new_%i (MC-true cut: gamma from "
                    "same mother); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut2_new[i]);
    fhPhotons_invmass_MCcut3_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut3_new_%i", i),
      Form("fhPhotons_invmass_MCcut3_new_%i (MC-true cut: gamma from different "
           "mother); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
           i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut3_new[i]);
    fhPhotons_invmass_MCcut4_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut4_new_%i", i),
      Form("fhPhotons_invmass_MCcut4_new_%i (MC-true cut: wrong combination of "
           "electrons); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
           i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut4_new[i]);
    fhPhotons_invmass_MCcut5_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut5_new_%i", i),
      Form("fhPhotons_invmass_MCcut5_new_%i (MC-true cut: wrong combination of "
           "electrons); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
           i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut5_new[i]);
    fhPhotons_invmass_MCcut6_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut6_new_%i", i),
      Form("fhPhotons_invmass_MCcut6_new_%i (MC-true cut: wrong combination of "
           "electrons); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
           i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut6_new[i]);
    fhPhotons_invmass_MCcut7_new[i] = new TH1D(
      Form("fhPhotons_invmass_MCcut7_new_%i", i),
      Form("fhPhotons_invmass_MCcut7_new_%i (MC-true cut: wrong combination of "
           "electrons); invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
           i),
      invmassSpectra_nof,
      invmassSpectra_start,
      invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcut7_new[i]);

    fhPhotons_invmass_MCcutAll_new[i] =
      new TH2D(Form("fhPhotons_invmass_MCcutAll_new_%i", i),
               Form("fhPhotons_invmass_MCcutAll_new_%i; case; invmass", i),
               25,
               0.,
               25.,
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcutAll_new[i]);

    fhPhotons_invmass_MCcutTest_new[i] =
      new TH1D(Form("fhPhotons_invmass_MCcutTest_new_%i", i),
               Form("fhPhotons_invmass_MCcutTest_new_%i (MC-true cut: test); "
                    "invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcutTest_new[i]);
    fhPhotons_invmass_MCcutTest2_new[i] =
      new TH1D(Form("fhPhotons_invmass_MCcutTest2_new_%i", i),
               Form("fhPhotons_invmass_MCcutTest2_new_%i (MC-true cut: test); "
                    "invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcutTest2_new[i]);
    fhPhotons_invmass_MCcutTest3_new[i] =
      new TH1D(Form("fhPhotons_invmass_MCcutTest3_new_%i", i),
               Form("fhPhotons_invmass_MCcutTest3_new_%i (MC-true cut: test); "
                    "invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_MCcutTest3_new[i]);

    fhPhotons_pt_vs_rap_new[i] = new TH2D(
      Form("fhPhotons_pt_vs_rap_new_%i", i),
      Form("fhPhotons_pt_vs_rap_new_%i; p_{t} in GeV/c; rapidity y", i),
      240,
      -2.,
      10.,
      270,
      -2.,
      7.);
    fHistoList_recofull_new[i].push_back(fhPhotons_pt_vs_rap_new[i]);

    fhElectrons_openingAngle_sameSign_new[i] = new TH1D(
      Form("fhElectrons_openingAngle_sameSign_new_%i", i),
      Form("fhElectrons_openingAngle_sameSign_new_%i; opening angle; #", i),
      101,
      -0.5,
      100.5);
    fHistoList_recofull_new[i].push_back(
      fhElectrons_openingAngle_sameSign_new[i]);


    fhMixedEventsTest_invmass[i] =
      new TH1D(Form("fhMixedEventsTest_invmass_%i", i),
               Form("fhMixedEventsTest_invmass_%i; invariant mass of 4 e^{#pm} "
                    "in GeV/c^{2}; #",
                    i),
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhMixedEventsTest_invmass[i]);

    fhPhotons_stats[i] = new TH1D(Form("fhPhotons_stats_%i", i),
                                  Form("fhPhotons_stats_%i; ; #", i),
                                  16,
                                  0,
                                  16);
    fHistoList_recofull_new[i].push_back(fhPhotons_stats[i]);
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(1, "fullrec_allCombs");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(2, "fullrec_cut1");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(3, "fullrec_cut2");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(4, "fullrec_cut3");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(5, "fullrec_cut4");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(6, "fullrec_cut5");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(7, "fullrec_cut6");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(8, "");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(9, "mix_allCombs");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(10, "mix_cut1");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(11, "mix_cut2");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(12, "mix_cut3");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(13, "mix_cut4");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(14, "mix_cut5");
    fhPhotons_stats[i]->GetXaxis()->SetBinLabel(15, "mix_cut6");


    fhPhotons_peakCheck1[i] =
      new TH1D(Form("fhPhotons_peakCheck1_%i", i),
               Form("fhPhotons_peakCheck1_%i; sum; #", i),
               20,
               -0.5,
               19.5);
    fhPhotons_peakCheck2[i] =
      new TH1D(Form("fhPhotons_peakCheck2_%i", i),
               Form("fhPhotons_peakCheck2_%i; sum; #", i),
               20,
               -0.5,
               19.5);
    fHistoList_recofull_new[i].push_back(fhPhotons_peakCheck1[i]);
    fHistoList_recofull_new[i].push_back(fhPhotons_peakCheck2[i]);

    fhPhotons_invmass_ANNcuts_new[i] =
      new TH2D(Form("fhPhotons_invmass_ANNcuts_new_%i", i),
               Form("fhPhotons_invmass_ANNcuts_new_%i;ann;invariant mass of 4 "
                    "e^{#pm} in GeV/c^{2}",
                    i),
               10,
               0,
               10,
               invmassSpectra_nof,
               invmassSpectra_start,
               invmassSpectra_end);
    fHistoList_recofull_new[i].push_back(fhPhotons_invmass_ANNcuts_new[i]);


    fhPhotons_phaseSpace_pi0[i] = new TH2D(
      Form("fhPhotons_phaseSpace_pi0_%i", i),
      Form("fhPhotons_phaseSpace_pi0_%i; p_{t} in GeV/c;rapidity y", i),
      240,
      -2.,
      10.,
      270,
      -2.,
      7.);
    fhPhotons_phaseSpace_eta[i] = new TH2D(
      Form("fhPhotons_phaseSpace_eta_%i", i),
      Form("fhPhotons_phaseSpace_eta_%i; p_{t} in GeV/c;rapidity y", i),
      240,
      -2.,
      10.,
      270,
      -2.,
      7.);
    fHistoList_recofull_new[i].push_back(fhPhotons_phaseSpace_pi0[i]);
    fHistoList_recofull_new[i].push_back(fhPhotons_phaseSpace_eta[i]);
  }


  fhPhotons_MCtrue_pdgCodes =
    new TH1D(Form("fhPhotons_MCtrue_pdgCodes_%i", 4),
             Form("fhPhotons_MCtrue_pdgCodes_%i; pdg code; #", 4),
             1000,
             0,
             1000);
  fHistoList_recofull_new[4].push_back(fhPhotons_MCtrue_pdgCodes);


  // EVENT MIXING HISTOGRAMS
  // =======================
  fhMixedEventsTest2_invmass =
    new TH1D(Form("fhMixedEventsTest2_invmass_%i", 4),
             Form("fhMixedEventsTest2_invmass_%i; invariant mass of 4 e^{#pm} "
                  "in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest2_invmass);
  fhMixedEventsTest3_invmass =
    new TH1D(Form("fhMixedEventsTest3_invmass_%i", 4),
             Form("fhMixedEventsTest3_invmass_%i; invariant mass of 4 e^{#pm} "
                  "in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest3_invmass);

  fhMixedEventsTest4_invmass =
    new TH1D(Form("fhMixedEventsTest4_invmass_%i", 4),
             Form("fhMixedEventsTest4_invmass_%i; invariant mass of 4 e^{#pm} "
                  "in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass);
  fhMixedEventsTest4_invmass_ann0 =
    new TH1D(Form("fhMixedEventsTest4_invmass_ann0_%i", 4),
             Form("fhMixedEventsTest4_invmass_ann0_%i; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ann0);

  fhMixedEventsTest4_pt_vs_rap = new TH2D(
    Form("fhMixedEventsTest4_pt_vs_rap_%i", 4),
    Form("fhMixedEventsTest4_pt_vs_rap_%i; p_{t} in GeV/c; rapidity y", 4),
    240,
    -2.,
    10.,
    270,
    -2.,
    7.);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_pt_vs_rap);

  fhMixedEventsTest4_invmass_ptBin1 =
    new TH1D(Form("fhMixedEventsTest4_invmass_%i_ptBin1", 4),
             Form("fhMixedEventsTest4_invmass_%i_ptBin1; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ptBin1);
  fhMixedEventsTest4_invmass_ptBin2 =
    new TH1D(Form("fhMixedEventsTest4_invmass_%i_ptBin2", 4),
             Form("fhMixedEventsTest4_invmass_%i_ptBin2; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ptBin2);
  fhMixedEventsTest4_invmass_ptBin3 =
    new TH1D(Form("fhMixedEventsTest4_invmass_%i_ptBin3", 4),
             Form("fhMixedEventsTest4_invmass_%i_ptBin3; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ptBin3);
  fhMixedEventsTest4_invmass_ptBin4 =
    new TH1D(Form("fhMixedEventsTest4_invmass_%i_ptBin4", 4),
             Form("fhMixedEventsTest4_invmass_%i_ptBin4; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}; #",
                  4),
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ptBin4);

  fhMixedEventsTest4_invmass_ANNcuts =
    new TH2D(Form("fhMixedEventsTest4_invmass_ANNcuts_%i", 4),
             Form("fhMixedEventsTest4_invmass_ANNcuts_%i;; invariant mass of 4 "
                  "e^{#pm} in GeV/c^{2}",
                  4),
             10,
             0,
             10,
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_recofull_new[4].push_back(fhMixedEventsTest4_invmass_ANNcuts);
}


void CbmAnaConversionRecoFull::Finish() {


  gDirectory->mkdir("RecoFull");
  gDirectory->cd("RecoFull");

  gDirectory->mkdir("NoRefit-NoChicut");  // index 1
  gDirectory->cd("NoRefit-NoChicut");
  for (UInt_t i = 0; i < fHistoList_recofull_new[1].size(); i++) {
    //for (UInt_t i = 0; i < fHistoList_recofull_1.size(); i++){
    //fHistoList_recofull_1[i]->Write();
    fHistoList_recofull_new[1][i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("NoRefit-WithChicut");  // index 2
  gDirectory->cd("NoRefit-WithChicut");
  for (UInt_t i = 0; i < fHistoList_recofull_new[2].size(); i++) {
    //for (UInt_t i = 0; i < fHistoList_recofull_2.size(); i++){
    //fHistoList_recofull_2[i]->Write();
    fHistoList_recofull_new[2][i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("WithRefit-NoChicut");  // index 3
  gDirectory->cd("WithRefit-NoChicut");
  for (UInt_t i = 0; i < fHistoList_recofull_new[3].size(); i++) {
    //for (UInt_t i = 0; i < fHistoList_recofull_3.size(); i++){
    //fHistoList_recofull_3[i]->Write();
    fHistoList_recofull_new[3][i]->Write();
  }
  gDirectory->cd("..");

  gDirectory->mkdir("WithRefit-WithChicut");  // index 4
  gDirectory->cd("WithRefit-WithChicut");
  for (UInt_t i = 0; i < fHistoList_recofull_new[4].size(); i++) {
    //for (UInt_t i = 0; i < fHistoList_recofull_4.size(); i++){
    //fHistoList_recofull_4[i]->Write();
    fHistoList_recofull_new[4][i]->Write();
  }
  gDirectory->cd("..");

  for (UInt_t i = 0; i < fHistoList_recofull.size(); i++) {
    fHistoList_recofull[i]->Write();
  }
  gDirectory->cd("..");


  cout << "CbmAnaConversionRecoFull: Realtime - " << fTime << endl;
}

void CbmAnaConversionRecoFull::Exec() {
  timer.Start();

  globalEventNo++;

  if (fPrimVertex != NULL) {
    fKFVertex = CbmKFVertex(*fPrimVertex);
  } else {
    Fatal("CbmAnaConversion::Exec", "No PrimaryVertex array!");
  }


  fElectrons_track.clear();
  fElectrons_momenta.clear();
  fElectrons_momentaChi.clear();
  fElectrons_mctrackID.clear();
  fVector_photons_pairs.clear();

  fElectrons_track_1.clear();
  fElectrons_momenta_1.clear();
  fElectrons_momentaChi_1.clear();
  fElectrons_mctrackID_1.clear();
  fVector_photons_pairs_1.clear();

  fElectrons_track_2.clear();
  fElectrons_momenta_2.clear();
  fElectrons_momentaChi_2.clear();
  fElectrons_mctrackID_2.clear();
  fVector_photons_pairs_2.clear();

  fElectrons_track_3.clear();
  fElectrons_momenta_3.clear();
  fElectrons_momentaChi_3.clear();
  fElectrons_mctrackID_3.clear();
  fVector_photons_pairs_3.clear();

  fElectrons_track_4.clear();
  fElectrons_momenta_4.clear();
  fElectrons_momentaChi_4.clear();
  fElectrons_mctrackID_4.clear();
  fVector_photons_pairs_4.clear();


  for (int i = 1; i <= 4; i++) {
    fElectrons_track_new[i].clear();
    fElectrons_momenta_new[i].clear();
    fElectrons_momentaChi_new[i].clear();
    fElectrons_mctrackID_new[i].clear();
    fElectrons_globaltrackID_new[i].clear();
    fVector_photons_pairs_new[i].clear();

    while (fMixedEventsElectrons[i].size() > 20) {
      fMixedEventsElectrons[i].erase(fMixedEventsElectrons[i].begin());
      fMixedEventsElectrons_gtrack[i].erase(
        fMixedEventsElectrons_gtrack[i].begin());
    }
  }


  // execution each 20/100 events (i.e. accumulation of data over 20/100 events)
  if (globalEventNo % 20 == 0) {
    MixedEventTest3();
    fMixedTest3_momenta.clear();
    fMixedTest3_gtrack.clear();
    fMixedTest3_eventno.clear();
  }
  if (globalEventNo % 200 == 0) {
    MixedEventTest4();
    fMixedTest4_photons.clear();
    fMixedTest4_mctracks.clear();
    fMixedTest4_eventno.clear();
    fMixedTest4_isRichElectronAnn0.clear();
    fMixedTest4_ElectronAnns.clear();
  }


  fElectrons_track_refit.clear();
  fElectrons_momenta_refit.clear();
  fVector_photons_pairs_refit.clear();

  fVector_photons_pairs_direction.clear();

  Int_t nofGT_richsts  = 0;
  Int_t nofElectrons   = 0;
  Int_t nofElectrons_1 = 0;
  Int_t nofElectrons_2 = 0;
  Int_t nofElectrons_3 = 0;
  Int_t nofElectrons_4 = 0;


  Int_t FilledMixedEventElectron =
    0;  // just one variable for test1 here, because only the first identified electron of each event should be taken (therefore the check below for variable < 5)

  // everything related to reconstructed data
  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  cout << "CbmAnaConversionRecoFull: number of global tracks "
       << nofGlobalTracks << endl;
  for (int iG = 0; iG < nofGlobalTracks; iG++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iG);
    if (NULL == gTrack) continue;
    int stsInd  = gTrack->GetStsTrackIndex();
    int richInd = gTrack->GetRichRingIndex();
    if (richInd < 0) continue;
    if (stsInd < 0) continue;

    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == NULL) continue;

    /*		CbmTrackMatchNew* stsMatch  = (CbmTrackMatchNew*)fStsTrackMatches->At(stsInd);
		if (stsMatch == NULL) continue;
		int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
		if (stsMcTrackId < 0) continue;
		CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
		if (mcTrack1 == NULL) continue;

		CbmTrackMatchNew* richMatch  = (CbmTrackMatchNew*)fRichRingMatches->At(richInd);
		if (richMatch == NULL) continue;
		int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
		if (richMcTrackId < 0) continue;
		CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
		if (mcTrack2 == NULL) continue;
*/
    CbmTrackMatchNew* stsMatch =
      (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    int stsMcTrackId = 0;
    CbmMCTrack* mcTrack1;
    if (stsMatch != NULL) {
      if (stsMatch->GetNofLinks() > 0) {
        stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
        if (stsMcTrackId >= 0) {
          mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
          if (mcTrack1 == NULL) stsMcTrackId = 0;
        }
      }
    }

    CbmTrackMatchNew* richMatch =
      (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    int richMcTrackId    = 0;
    CbmMCTrack* mcTrack2 = NULL;
    if (richMatch != NULL) {
      richMcTrackId = richMatch->GetMatchedLink().GetIndex();
      if (richMcTrackId >= 0) {
        mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
        if (mcTrack2 == NULL) richMcTrackId = 0;
      }
    }

    //if(stsMcTrackId != richMcTrackId) continue;
    nofGT_richsts++;

    //		int pdg = TMath::Abs(mcTrack1->GetPdgCode());


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

    //		Double_t result_chi2ndf = stsTracks[0].GetChiSq() / stsTracks[0].GetNDF();
    float result_chi = chiPrim[0];
    fhMomentumFits->Fill(result_chi);
    //		Int_t pidHypo = gTrack->GetPidHypo();

    // #################################################################
    // ##### up to here standard calculation of reconstructed data #####
    // #################################################################


    // indizes in the following
    // 1 = no refit, no chicut
    // 2 = no refit, with chicut
    // 3 = with refit, no chicut
    // 4 = with refit, with chicut


    Bool_t electron_rich2 =
      CbmLitGlobalElectronId::GetInstance().IsRichElectron(
        iG, refittedMomentum.Mag());


    Double_t chiCut =
      CbmAnaConversionCutSettings::CalcChiCut(refittedMomentum.Perp());


    if (electron_rich2) {  // electron identification without refit of momentum
      nofElectrons_1++;
      fElectrons_track_1.push_back(gTrack);
      fElectrons_momenta_1.push_back(refittedMomentum);
      fElectrons_momentaChi_1.push_back(result_chi);
      //fElectrons_mctrackID_1.push_back(richMcTrackId);
      fElectrons_mctrackID_1.push_back(stsMcTrackId);

      fElectrons_track_new[1].push_back(gTrack);
      fElectrons_momenta_new[1].push_back(refittedMomentum);
      fElectrons_momentaChi_new[1].push_back(result_chi);
      //fElectrons_mctrackID_1.push_back(richMcTrackId);
      fElectrons_mctrackID_new[1].push_back(stsMcTrackId);
      fElectrons_globaltrackID_new[1].push_back(iG);

      if (FilledMixedEventElectron < 5) {
        fMixedEventsElectrons[1].push_back(refittedMomentum);
        fMixedEventsElectrons_gtrack[1].push_back(gTrack);
        FilledMixedEventElectron++;
      }

      // using the old method
      fElectrons_track.push_back(gTrack);
      fElectrons_momenta.push_back(refittedMomentum);
      fElectrons_momentaChi.push_back(result_chi);
      //fElectrons_mctrackID_1.push_back(richMcTrackId);
      fElectrons_mctrackID.push_back(stsMcTrackId);

      if (result_chi <= chiCut) {
        nofElectrons_2++;
        fElectrons_track_2.push_back(gTrack);
        fElectrons_momenta_2.push_back(refittedMomentum);
        fElectrons_momentaChi_2.push_back(result_chi);
        //fElectrons_mctrackID_2.push_back(richMcTrackId);
        fElectrons_mctrackID_2.push_back(stsMcTrackId);

        fElectrons_track_new[2].push_back(gTrack);
        fElectrons_momenta_new[2].push_back(refittedMomentum);
        fElectrons_momentaChi_new[2].push_back(result_chi);
        //fElectrons_mctrackID_1.push_back(richMcTrackId);
        fElectrons_mctrackID_new[2].push_back(stsMcTrackId);
        fElectrons_globaltrackID_new[2].push_back(iG);

        if (FilledMixedEventElectron < 5) {
          fMixedEventsElectrons[2].push_back(refittedMomentum);
          fMixedEventsElectrons_gtrack[2].push_back(gTrack);
          FilledMixedEventElectron++;
        }
      }
    }


    // Doing refit of momenta

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
    float result_chi_electron = chiPrim_electron[0];

    refittedMomentum = refittedMomentum_electron;
    result_chi       = result_chi_electron;

    //cout << "refitted momenta!" << endl;


    Bool_t electron_rich = CbmLitGlobalElectronId::GetInstance().IsRichElectron(
      iG, refittedMomentum.Mag());
    //Bool_t electron_rich = (pdg == 11);
    Bool_t electron_trd = CbmLitGlobalElectronId::GetInstance().IsTrdElectron(
      iG, refittedMomentum.Mag());
    Bool_t electron_tof = CbmLitGlobalElectronId::GetInstance().IsTofElectron(
      iG, refittedMomentum.Mag());


    if (electron_rich) {  // electron identification with refit of momentum
      nofElectrons_3++;
      fElectrons_track_3.push_back(gTrack);
      fElectrons_momenta_3.push_back(refittedMomentum);
      fElectrons_momentaChi_3.push_back(result_chi);
      //fElectrons_mctrackID_3.push_back(richMcTrackId);
      fElectrons_mctrackID_3.push_back(stsMcTrackId);

      fElectrons_track_new[3].push_back(gTrack);
      fElectrons_momenta_new[3].push_back(refittedMomentum);
      fElectrons_momentaChi_new[3].push_back(result_chi);
      //fElectrons_mctrackID_1.push_back(richMcTrackId);
      fElectrons_mctrackID_new[3].push_back(stsMcTrackId);
      fElectrons_globaltrackID_new[3].push_back(iG);

      if (FilledMixedEventElectron < 5) {
        fMixedEventsElectrons[3].push_back(refittedMomentum);
        fMixedEventsElectrons_gtrack[3].push_back(gTrack);
        FilledMixedEventElectron++;
      }
      if (result_chi <= chiCut) {
        nofElectrons_4++;
        fElectrons_track_4.push_back(gTrack);
        fElectrons_momenta_4.push_back(refittedMomentum);
        fElectrons_momentaChi_4.push_back(result_chi);
        //fElectrons_mctrackID_4.push_back(richMcTrackId);
        fElectrons_mctrackID_4.push_back(stsMcTrackId);

        fElectrons_track_new[4].push_back(gTrack);
        fElectrons_momenta_new[4].push_back(refittedMomentum);
        fElectrons_momentaChi_new[4].push_back(result_chi);
        //fElectrons_mctrackID_1.push_back(richMcTrackId);
        fElectrons_mctrackID_new[4].push_back(stsMcTrackId);
        fElectrons_globaltrackID_new[4].push_back(iG);

        // for event mixing technique
        if (FilledMixedEventElectron < 5) {  // test1
          fMixedEventsElectrons[4].push_back(refittedMomentum);
          fMixedEventsElectrons_gtrack[4].push_back(gTrack);
          FilledMixedEventElectron++;
        }
        // test3
        fMixedTest3_momenta.push_back(refittedMomentum);
        fMixedTest3_gtrack.push_back(gTrack);
        fMixedTest3_eventno.push_back(globalEventNo);
      }

      fhMomentumFits_electronRich->Fill(result_chi);
      //TVector3 startvertex;
      //mcTrack2->GetStartVertex(startvertex);
      //fhPhotons_startvertex_vs_chi->Fill(startvertex.Z(), result_chi);
    }


    if (electron_rich) fhElectrons->Fill(0);
    if (electron_trd) fhElectrons->Fill(1);
    if (electron_tof) fhElectrons->Fill(2);
    if (electron_rich && electron_trd) fhElectrons->Fill(3);
    if (electron_rich && electron_tof) fhElectrons->Fill(4);
    if (electron_trd && electron_tof) fhElectrons->Fill(5);
    if (electron_rich && electron_trd && electron_tof) fhElectrons->Fill(6);
    if ((electron_rich && electron_trd) || (electron_rich && electron_tof)
        || (electron_trd && electron_tof))
      fhElectrons->Fill(7);
  }


  // for mixed events technique, test2
  fMixedEventsElectrons_list1        = fMixedEventsElectrons_list2;
  fMixedEventsElectrons_list2        = fMixedEventsElectrons_list3;
  fMixedEventsElectrons_list3        = fMixedEventsElectrons_list4;
  fMixedEventsElectrons_list4        = fElectrons_momenta_new[4];
  fMixedEventsElectrons_list1_gtrack = fMixedEventsElectrons_list2_gtrack;
  fMixedEventsElectrons_list2_gtrack = fMixedEventsElectrons_list3_gtrack;
  fMixedEventsElectrons_list3_gtrack = fMixedEventsElectrons_list4_gtrack;
  fMixedEventsElectrons_list4_gtrack = fElectrons_track_new[4];


  fhElectrons_nofPerEvent->Fill(nofElectrons);
  fhElectrons_nofPerEvent_1->Fill(nofElectrons_1);
  fhElectrons_nofPerEvent_2->Fill(nofElectrons_2);
  fhElectrons_nofPerEvent_3->Fill(nofElectrons_3);
  fhElectrons_nofPerEvent_4->Fill(nofElectrons_4);

  cout << "CbmAnaConversionRecoFull: number of global tracks in STS and RICH "
       << nofGT_richsts << endl;

  CombineElectrons();
  CombinePhotons();

  cout << "CbmAnaConversionRecoFull: combining for each case!" << endl;

  for (int i = 1; i <= 4; i++) {
    cout << "CbmAnaConversionRecoFull: case " << i << endl;
    CombineElectrons(fElectrons_track_new[i],
                     fElectrons_momenta_new[i],
                     fElectrons_momentaChi_new[i],
                     fElectrons_mctrackID_new[i],
                     fVector_photons_pairs_new[i],
                     i);
    CombinePhotons(fElectrons_track_new[i],
                   fElectrons_momenta_new[i],
                   fElectrons_momentaChi_new[i],
                   fElectrons_mctrackID_new[i],
                   fVector_photons_pairs_new[i],
                   i);
  }
  /*
	CombineElectrons(fElectrons_track_1, fElectrons_momenta_1, fElectrons_momentaChi_1, fElectrons_mctrackID_1, fVector_photons_pairs_1, 1);
	CombinePhotons(fElectrons_track_1, fElectrons_momenta_1, fElectrons_momentaChi_1, fElectrons_mctrackID_1, fVector_photons_pairs_1, 1);
	
	CombineElectrons(fElectrons_track_2, fElectrons_momenta_2, fElectrons_momentaChi_2, fElectrons_mctrackID_2, fVector_photons_pairs_2, 2);
	CombinePhotons(fElectrons_track_2, fElectrons_momenta_2, fElectrons_momentaChi_2, fElectrons_mctrackID_2, fVector_photons_pairs_2, 2);
	
	CombineElectrons(fElectrons_track_3, fElectrons_momenta_3, fElectrons_momentaChi_3, fElectrons_mctrackID_3, fVector_photons_pairs_3, 3);
	CombinePhotons(fElectrons_track_3, fElectrons_momenta_3, fElectrons_momentaChi_3, fElectrons_mctrackID_3, fVector_photons_pairs_3, 3);
	
	CombineElectrons(fElectrons_track_4, fElectrons_momenta_4, fElectrons_momentaChi_4, fElectrons_mctrackID_4, fVector_photons_pairs_4, 4);
	CombinePhotons(fElectrons_track_4, fElectrons_momenta_4, fElectrons_momentaChi_4, fElectrons_mctrackID_4, fVector_photons_pairs_4, 4);
*/
  //CombineElectronsRefit();
  //CombinePhotonsRefit();

  //CombinePhotonsDirection();


  MixedEventTest();
  MixedEventTest2();

  timer.Stop();
  fTime += timer.RealTime();
}


//void CbmAnaConversionRecoFull::CombineElectrons(	vector<CbmGlobalTrack*> gtrack, vector<TVector3> momenta, vector<float> momentaChi, vector<int> mctrackID)
void CbmAnaConversionRecoFull::CombineElectrons(
  vector<CbmGlobalTrack*> gtrack,
  vector<TVector3> momenta,
  vector<float> /*momentaChi*/,
  vector<int> /*mctrackID*/,
  vector<vector<int>> reconstructedPhotons,
  Int_t index) {
  Int_t nof = momenta.size();
  fhElectrons_nofPerEvent_new[index]->Fill(nof);
  cout << "CbmAnaConversionRecoFull: " << index << ": CombineElectrons, nof - "
       << nof << endl;
  Int_t nofPhotons = 0;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t check1 =
          (gtrack[a]->GetParamLast()->GetQp()
           > 0);  // positive or negative charge (qp = charge over momentum ratio)
        Int_t check2 = (gtrack[b]->GetParamLast()->GetQp() > 0);
        Int_t test   = check1 + check2;
        if (test != 1) continue;  // need one electron and one positron
        //if(fElectrons_momentaChi[a] > 10 || fElectrons_momentaChi[b] > 10) continue;

        CbmLmvmKinematicParams params1 =
          CalculateKinematicParamsReco(momenta[a], momenta[b]);

        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
            momenta[a], momenta[b]);

        Double_t openingAngleCut = 1.8 - 0.6 * paramsTest.fPt;
        Double_t invMassCut      = 0.03;

        Int_t IsPhoton_openingAngle1 = (paramsTest.fAngle < openingAngleCut);
        Int_t IsPhoton_invMass1      = (paramsTest.fMinv < invMassCut);

        /*
				Double_t tXa = gtrack[a]->GetParamLast()->GetTx();
				Double_t tYa = gtrack[a]->GetParamLast()->GetTy();
				Double_t tXb = gtrack[b]->GetParamLast()->GetTx();
				Double_t tYb = gtrack[b]->GetParamLast()->GetTy();
*/

        if (IsPhoton_openingAngle1 && IsPhoton_invMass1) {
          nofPhotons++;
          vector<int> pair;  // = {a, b};
          pair.push_back(a);
          pair.push_back(b);
          reconstructedPhotons.push_back(pair);

          if (index == 1) {
            fhElectrons_invmass_1->Fill(params1.fMinv);
            fVector_photons_pairs_1.push_back(pair);
            //fVector_photons_pairs.push_back(pair);
          }
          if (index == 2) {
            fhElectrons_invmass_2->Fill(params1.fMinv);
            fVector_photons_pairs_2.push_back(pair);
          }
          if (index == 3) {
            fhElectrons_invmass_3->Fill(params1.fMinv);
            fVector_photons_pairs_3.push_back(pair);
          }
          if (index == 4) {
            fhElectrons_invmass_4->Fill(params1.fMinv);
            fVector_photons_pairs_4.push_back(pair);
          }

          fhElectrons_invmass_new[index]->Fill(params1.fMinv);
          fVector_photons_pairs_new[index].push_back(pair);


          //fVector_photons_momenta.push_back(params1.momPair);

          //fhPhotons_tX->Fill(tXa);
          //fhPhotons_tX->Fill(tXb);
          //fhPhotons_tY->Fill(tYa);
          //fhPhotons_tY->Fill(tYb);

          TVector3 momentumE1f;
          TVector3 momentumE2f;
          gtrack[a]->GetParamFirst()->Momentum(momentumE1f);
          gtrack[b]->GetParamFirst()->Momentum(momentumE2f);
          TVector3 momentumE1l;
          TVector3 momentumE2l;
          gtrack[a]->GetParamLast()->Momentum(momentumE1l);
          gtrack[b]->GetParamLast()->Momentum(momentumE2l);

          //Double_t energyE1 = TMath::Sqrt(momentumE1.Mag2() + M2E);
          //TLorentzVector lorVecE1(momentumE1, energyE1);
          //Double_t energyE2 = TMath::Sqrt(momentumE2.Mag2() + M2E);
          //TLorentzVector lorVecE2(momentumE2, energyE2);
          //TLorentzVector g = lorVecE1 + lorVecE2;

          //lorVecE1.Boost(g.BoostVector() );
          //lorVecE2.Boost(g.BoostVector() );

          TVector3 normal1     = momentumE1f.Cross(momentumE1l);
          TVector3 normal2     = momentumE2f.Cross(momentumE2l);
          Double_t normalAngle = normal1.Angle(normal2);
          Double_t theta       = 180. * normalAngle / TMath::Pi();

          //Double_t boostAngle = lorVecE1.Angle(lorVecE2.Vect());

          //fhPhotons_boostAngle->Fill(normalAngle);


          //if( TMath::Abs(tXa - tXb) < 0.5 || TMath::Abs(tYa - tYb) < 0.5 ) {
          if (theta < 30) { fVector_photons_pairs_direction.push_back(pair); }


          // mixed event test, test4
          if (index == 4) {
            vector<TVector3> pairmomenta;
            pairmomenta.push_back(momenta[a]);
            pairmomenta.push_back(momenta[b]);
            vector<CbmMCTrack*> pair_mctracks;
            pair_mctracks.push_back(
              (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID_new[index][a]));
            pair_mctracks.push_back(
              (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID_new[index][b]));
            fMixedTest4_photons.push_back(pairmomenta);
            fMixedTest4_eventno.push_back(globalEventNo);
            fMixedTest4_mctracks.push_back(pair_mctracks);

            vector<Bool_t> IsRichElectronAnn0;
            IsRichElectronAnn0.push_back(
              CbmLitGlobalElectronId::GetInstance().IsRichElectron(
                fElectrons_globaltrackID_new[index][a], momenta[a].Mag()));
            IsRichElectronAnn0.push_back(
              CbmLitGlobalElectronId::GetInstance().IsRichElectron(
                fElectrons_globaltrackID_new[index][b], momenta[b].Mag()));
            fMixedTest4_isRichElectronAnn0.push_back(IsRichElectronAnn0);

            vector<Double_t> electronANNs;
            electronANNs.push_back(ElectronANNvalue(
              fElectrons_globaltrackID_new[index][a], momenta[a].Mag()));
            electronANNs.push_back(ElectronANNvalue(
              fElectrons_globaltrackID_new[index][b], momenta[b].Mag()));
            fMixedTest4_ElectronAnns.push_back(electronANNs);
          }
        }
      }
    }
  }
  //if(index == 1) fhPhotons_nofPerEvent_1->Fill(nofPhotons);
  //if(index == 2) fhPhotons_nofPerEvent_2->Fill(nofPhotons);
  //if(index == 3) fhPhotons_nofPerEvent_3->Fill(nofPhotons);
  //if(index == 4) fhPhotons_nofPerEvent_4->Fill(nofPhotons);
  fhPhotons_nofPerEvent_new[index]->Fill(nofPhotons);
  //fhPhotons_nofPerEvent->Fill(nofPhotons);
  cout << "CbmAnaConversionRecoFull: CombineElectrons: " << index
       << ": Crosscheck - nof reconstructed photons: " << nofPhotons << endl;
}


Double_t CbmAnaConversionRecoFull::Invmass_4particlesRECO(const TVector3 part1,
                                                          const TVector3 part2,
                                                          const TVector3 part3,
                                                          const TVector3 part4)
// calculation of invariant mass from four electrons/positrons
{
  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

  return sum.Mag();
}


Double_t CbmAnaConversionRecoFull::Pt_4particlesRECO(const TVector3 part1,
                                                     const TVector3 part2,
                                                     const TVector3 part3,
                                                     const TVector3 part4) {
  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

  Double_t perp = sum.Perp();
  Double_t pt   = TMath::Sqrt(sum.X() * sum.X() + sum.Y() * sum.Y());

  cout << "CbmAnaConversionRecoFull::Pt_4particlesRECO: perp/pt = " << perp
       << " / " << pt << endl;

  return perp;
}


Double_t CbmAnaConversionRecoFull::Rap_4particlesRECO(const TVector3 part1,
                                                      const TVector3 part2,
                                                      const TVector3 part3,
                                                      const TVector3 part4) {
  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;


  TVector3 momPair    = part1 + part2 + part3 + part4;
  Double_t energyPair = energy1 + energy2 + energy3 + energy4;
  Double_t pzPair     = momPair.Pz();
  Double_t yPair =
    0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));

  cout << "CbmAnaConversionRecoFull::Rap_4particlesRECO: yPair = " << yPair
       << endl;

  return yPair;
}


CbmLmvmKinematicParams CbmAnaConversionRecoFull::CalculateKinematicParamsReco(
  const TVector3 electron1,
  const TVector3 electron2) {
  CbmLmvmKinematicParams params;

  Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
  TLorentzVector lorVecP(electron1, energyP);

  Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
  TLorentzVector lorVecM(electron2, energyM);

  TVector3 momPair    = electron1 + electron2;
  Double_t energyPair = energyP + energyM;
  Double_t ptPair     = momPair.Perp();
  Double_t pzPair     = momPair.Pz();
  Double_t yPair =
    0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
  Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
  Double_t theta     = 180. * anglePair / TMath::Pi();
  Double_t minv      = 2. * TMath::Sin(anglePair / 2.)
                  * TMath::Sqrt(electron1.Mag() * electron2.Mag());

  params.fMomentumMag = momPair.Mag();
  params.fPt          = ptPair;
  params.fRapidity    = yPair;
  params.fMinv        = minv;
  params.fAngle       = theta;
  return params;
}


CbmLmvmKinematicParams
CbmAnaConversionRecoFull::CalculateKinematicParams_4particles(
  const TVector3 part1,
  const TVector3 part2,
  const TVector3 part3,
  const TVector3 part4) {
  CbmLmvmKinematicParams params;

  Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
  TLorentzVector lorVec1(part1, energy1);

  Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
  TLorentzVector lorVec2(part2, energy2);

  Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
  TLorentzVector lorVec3(part3, energy3);

  Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
  TLorentzVector lorVec4(part4, energy4);

  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

  TVector3 momPair    = part1 + part2 + part3 + part4;
  Double_t energyPair = energy1 + energy2 + energy3 + energy4;
  Double_t ptPair     = momPair.Perp();
  Double_t pzPair     = momPair.Pz();
  Double_t yPair =
    0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
  //    Double_t anglePair = 0;
  //    Double_t theta = 180.*anglePair/TMath::Pi();
  Double_t minv = sum.Mag();

  params.fMomentumMag = momPair.Mag();
  params.fPt          = ptPair;
  params.fRapidity    = yPair;
  params.fMinv        = minv;
  params.fAngle       = 0;
  return params;
}


void CbmAnaConversionRecoFull::CombinePhotons(
  vector<CbmGlobalTrack*> gtrack,
  vector<TVector3> momenta,
  vector<float> momentaChi,
  vector<int>,
  vector<vector<int>> reconstructedPhotons,
  Int_t index)
// index: kind of electron cut: 1 = no refit, no chicut; 2 = no refit, with chicut; 3 = with refit, no chicut; 4 = with refit, with chicut
{
  if (index == 1) reconstructedPhotons = fVector_photons_pairs_1;
  if (index == 2) reconstructedPhotons = fVector_photons_pairs_2;
  if (index == 3) reconstructedPhotons = fVector_photons_pairs_3;
  if (index == 4) reconstructedPhotons = fVector_photons_pairs_4;

  reconstructedPhotons = fVector_photons_pairs_new[index];

  Int_t nofPi0 = 0;

  Int_t nof = reconstructedPhotons.size();
  fhPhotons_nofPerEventAfter_new[index]->Fill(nof);
  cout << "CbmAnaConversionRecoFull: " << index << ": CombinePhotons, nof - "
       << nof << endl;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        cout << "####" << endl;
        cout << "CbmAnaConversionRecoFull: calculating invariant mass! (" << a
             << "/" << b << ")" << endl;
        Int_t electron11 = reconstructedPhotons[a][0];
        Int_t electron12 = reconstructedPhotons[a][1];
        Int_t electron21 = reconstructedPhotons[b][0];
        Int_t electron22 = reconstructedPhotons[b][1];
        cout << "CbmAnaConversionRecoFull: calculating invariant mass! ("
             << electron11 << "/" << electron12 << "/" << electron21 << "/"
             << electron22 << ")" << endl;

        if (electron11 == electron12 || electron11 == electron21
            || electron11 == electron22 || electron12 == electron21
            || electron12 == electron22 || electron21 == electron22) {
          cout << "CbmAnaConversionRecoFull: RecoFull_DoubleIndex!" << endl;
          continue;
        }
        nofPi0++;

        Int_t charge11 = (gtrack[electron11]->GetParamLast()->GetQp() > 0);
        //Int_t charge12 = (gtrack[electron12]->GetParamLast()->GetQp() > 0);
        Int_t charge21 = (gtrack[electron21]->GetParamLast()->GetQp() > 0);
        Int_t charge22 = (gtrack[electron22]->GetParamLast()->GetQp() > 0);
        if ((charge11 + charge21 == 2) || (charge11 + charge21 == 0)) {
          CbmAnaConversionKinematicParams paramsTest1 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              momenta[electron11], momenta[electron21]);
          CbmAnaConversionKinematicParams paramsTest2 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              momenta[electron12], momenta[electron22]);
          fhElectrons_openingAngle_sameSign_new[index]->Fill(
            paramsTest1.fAngle);
          fhElectrons_openingAngle_sameSign_new[index]->Fill(
            paramsTest2.fAngle);
        }
        if ((charge11 + charge22 == 2) || (charge11 + charge22 == 0)) {
          CbmAnaConversionKinematicParams paramsTest1 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              momenta[electron11], momenta[electron22]);
          CbmAnaConversionKinematicParams paramsTest2 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              momenta[electron12], momenta[electron21]);
          fhElectrons_openingAngle_sameSign_new[index]->Fill(
            paramsTest1.fAngle);
          fhElectrons_openingAngle_sameSign_new[index]->Fill(
            paramsTest2.fAngle);
        }

        Double_t invmass = Invmass_4particlesRECO(momenta[electron11],
                                                  momenta[electron12],
                                                  momenta[electron21],
                                                  momenta[electron22]);

        if (index == 1) fhPhotons_invmass_1->Fill(invmass);
        if (index == 2) fhPhotons_invmass_2->Fill(invmass);
        if (index == 3) fhPhotons_invmass_3->Fill(invmass);
        if (index == 4) fhPhotons_invmass_4->Fill(invmass);
        fhPhotons_invmass_new[index]->Fill(invmass);

        Double_t pt  = Pt_4particlesRECO(momenta[electron11],
                                        momenta[electron12],
                                        momenta[electron21],
                                        momenta[electron22]);
        Double_t rap = Rap_4particlesRECO(momenta[electron11],
                                          momenta[electron12],
                                          momenta[electron21],
                                          momenta[electron22]);

        fhPhotons_pt_vs_rap_new[index]->Fill(pt, rap);

        Double_t opening_angle = OpeningAngleBetweenPhotons(
          momenta, reconstructedPhotons[a], reconstructedPhotons[b]);
        if (index == 1) fhPhotons_angleBetween->Fill(opening_angle);


        CbmLmvmKinematicParams params1 =
          CalculateKinematicParams_4particles(momenta[electron11],
                                              momenta[electron12],
                                              momenta[electron21],
                                              momenta[electron22]);

        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
            momenta[electron11],
            momenta[electron12],
            momenta[electron21],
            momenta[electron22]);

        Bool_t IsRichElectron_ann0_e11 =
          CbmLitGlobalElectronId::GetInstance().IsRichElectron(
            fElectrons_globaltrackID_new[index][electron11],
            momenta[electron11].Mag());
        Bool_t IsRichElectron_ann0_e12 =
          CbmLitGlobalElectronId::GetInstance().IsRichElectron(
            fElectrons_globaltrackID_new[index][electron12],
            momenta[electron12].Mag());
        Bool_t IsRichElectron_ann0_e21 =
          CbmLitGlobalElectronId::GetInstance().IsRichElectron(
            fElectrons_globaltrackID_new[index][electron21],
            momenta[electron21].Mag());
        Bool_t IsRichElectron_ann0_e22 =
          CbmLitGlobalElectronId::GetInstance().IsRichElectron(
            fElectrons_globaltrackID_new[index][electron22],
            momenta[electron22].Mag());

        Double_t ANNe11 =
          ElectronANNvalue(fElectrons_globaltrackID_new[index][electron11],
                           momenta[electron11].Mag());
        Double_t ANNe12 =
          ElectronANNvalue(fElectrons_globaltrackID_new[index][electron12],
                           momenta[electron12].Mag());
        Double_t ANNe21 =
          ElectronANNvalue(fElectrons_globaltrackID_new[index][electron21],
                           momenta[electron21].Mag());
        Double_t ANNe22 =
          ElectronANNvalue(fElectrons_globaltrackID_new[index][electron22],
                           momenta[electron22].Mag());

        if (IsRichElectron_ann0_e11 && IsRichElectron_ann0_e12
            && IsRichElectron_ann0_e21 && IsRichElectron_ann0_e22) {
          fhPhotons_invmass_ann0_new[index]->Fill(invmass);
        }
        if (ANNe11 > -1 && ANNe12 > -1 && ANNe21 > -1 && ANNe22 > -1)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(1, invmass);
        if (ANNe11 > -0.9 && ANNe12 > -0.9 && ANNe21 > -0.9 && ANNe22 > -0.9)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(2, invmass);
        if (ANNe11 > -0.8 && ANNe12 > -0.8 && ANNe21 > -0.8 && ANNe22 > -0.8)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(3, invmass);
        if (ANNe11 > -0.7 && ANNe12 > -0.7 && ANNe21 > -0.7 && ANNe22 > -0.7)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(4, invmass);
        if (ANNe11 > -0.6 && ANNe12 > -0.6 && ANNe21 > -0.6 && ANNe22 > -0.6)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(5, invmass);
        if (ANNe11 > -0.5 && ANNe12 > -0.5 && ANNe21 > -0.5 && ANNe22 > -0.5)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(6, invmass);
        if (ANNe11 > -0.0 && ANNe12 > -0.0 && ANNe21 > -0.0 && ANNe22 > -0.0)
          fhPhotons_invmass_ANNcuts_new[index]->Fill(7, invmass);

        cout << "CbmAnaConversionRecoFull: debug: (" << invmass << "/"
             << paramsTest.fMinv << ") - (" << pt << "/" << paramsTest.fPt
             << ") - (" << rap << "/" << paramsTest.fRapidity << ")" << endl;

        //TVector3 momentumE1;
        //TVector3 momentumE2;
        //gtrack[a]->GetParamLast()->Momentum(momentumE1);
        //gtrack[b]->GetParamLast()->Momentum(momentumE2);

        Double_t energyE11 = TMath::Sqrt(momenta[electron11].Mag2() + M2E);
        TLorentzVector lorVecE11(momenta[electron11], energyE11);
        Double_t energyE12 = TMath::Sqrt(momenta[electron12].Mag2() + M2E);
        TLorentzVector lorVecE12(momenta[electron12], energyE12);
        Double_t energyE21 = TMath::Sqrt(momenta[electron21].Mag2() + M2E);
        TLorentzVector lorVecE21(momenta[electron21], energyE21);
        Double_t energyE22 = TMath::Sqrt(momenta[electron22].Mag2() + M2E);
        TLorentzVector lorVecE22(momenta[electron22], energyE22);
        TLorentzVector g1 = lorVecE11 + lorVecE12;
        TLorentzVector g2 = lorVecE21 + lorVecE22;
        TLorentzVector pi = lorVecE11 + lorVecE12 + lorVecE21 + lorVecE22;

        g1.Boost(-pi.BoostVector());
        g2.Boost(-pi.BoostVector());

        Double_t boostAngle = g1.Angle(g2.Vect());
        Double_t theta      = 180. * boostAngle / TMath::Pi();

        if (index == 1) fhPhotons_boostAngleTest->Fill(boostAngle);

        //if(opening_angle < 10) {
        if (true) {
          if (false) {
            fhPhotons_boostAngle->Fill(theta);

            fhPhotons_invmass_cut->Fill(invmass);
            fhPhotons_invmass_vs_pt->Fill(invmass, pt);
            fhPhotons_rapidity_vs_pt->Fill(pt, rap);
            fhPhotons_invmass_vs_openingAngle->Fill(invmass, opening_angle);
            fhPhotons_openingAngle_vs_momentum->Fill(opening_angle,
                                                     params1.fMomentumMag);

            fhMomentumFits_pi0reco->Fill(momentaChi[electron11]);
            fhMomentumFits_pi0reco->Fill(momentaChi[electron12]);
            fhMomentumFits_pi0reco->Fill(momentaChi[electron21]);
            fhMomentumFits_pi0reco->Fill(momentaChi[electron22]);

            fhPhotons_invmass_vs_chi->Fill(invmass, momentaChi[electron11]);
            fhPhotons_invmass_vs_chi->Fill(invmass, momentaChi[electron12]);
            fhPhotons_invmass_vs_chi->Fill(invmass, momentaChi[electron21]);
            fhPhotons_invmass_vs_chi->Fill(invmass, momentaChi[electron22]);
          }

          if (index == 1) {
            if (pt <= 1) fhPhotons_invmass_ptBin1_1->Fill(invmass);
            if (pt > 1 && pt <= 2) fhPhotons_invmass_ptBin2_1->Fill(invmass);
            if (pt > 2 && pt <= 3) fhPhotons_invmass_ptBin3_1->Fill(invmass);
            if (pt > 3 && pt <= 4) fhPhotons_invmass_ptBin4_1->Fill(invmass);
            fhPhotons_invmass_vs_pt_1->Fill(invmass, pt);
          }
          if (index == 2) {
            if (pt <= 1) fhPhotons_invmass_ptBin1_2->Fill(invmass);
            if (pt > 1 && pt <= 2) fhPhotons_invmass_ptBin2_2->Fill(invmass);
            if (pt > 2 && pt <= 3) fhPhotons_invmass_ptBin3_2->Fill(invmass);
            if (pt > 3 && pt <= 4) fhPhotons_invmass_ptBin4_2->Fill(invmass);
            fhPhotons_invmass_vs_pt_2->Fill(invmass, pt);
          }
          if (index == 3) {
            if (pt <= 1) fhPhotons_invmass_ptBin1_3->Fill(invmass);
            if (pt > 1 && pt <= 2) fhPhotons_invmass_ptBin2_3->Fill(invmass);
            if (pt > 2 && pt <= 3) fhPhotons_invmass_ptBin3_3->Fill(invmass);
            if (pt > 3 && pt <= 4) fhPhotons_invmass_ptBin4_3->Fill(invmass);
            fhPhotons_invmass_vs_pt_3->Fill(invmass, pt);
          }
          if (index == 4) {
            if (pt <= 1) fhPhotons_invmass_ptBin1_4->Fill(invmass);
            if (pt > 1 && pt <= 2) fhPhotons_invmass_ptBin2_4->Fill(invmass);
            if (pt > 2 && pt <= 3) fhPhotons_invmass_ptBin3_4->Fill(invmass);
            if (pt > 3 && pt <= 4) fhPhotons_invmass_ptBin4_4->Fill(invmass);
            fhPhotons_invmass_vs_pt_4->Fill(invmass, pt);

            if (!(fElectrons_mctrackID_new[index][electron11]
                    == fElectrons_mctrackID_new[index][electron12]
                  || fElectrons_mctrackID_new[index][electron11]
                       == fElectrons_mctrackID_new[index][electron21]
                  || fElectrons_mctrackID_new[index][electron11]
                       == fElectrons_mctrackID_new[index][electron22]
                  || fElectrons_mctrackID_new[index][electron12]
                       == fElectrons_mctrackID_new[index][electron21]
                  || fElectrons_mctrackID_new[index][electron12]
                       == fElectrons_mctrackID_new[index][electron22]
                  || fElectrons_mctrackID_new[index][electron21]
                       == fElectrons_mctrackID_new[index][electron22])) {
              fhPhotons_invmass_MCcutTest_new[index]->Fill(invmass);
            }
          }

          if (pt <= 0.5) fhPhotons_invmass_ptBin1_new[index]->Fill(invmass);
          if (pt > 0.5 && pt <= 1)
            fhPhotons_invmass_ptBin2_new[index]->Fill(invmass);
          if (pt > 1 && pt <= 1.5)
            fhPhotons_invmass_ptBin3_new[index]->Fill(invmass);
          if (pt > 1.5 && pt <= 2)
            fhPhotons_invmass_ptBin4_new[index]->Fill(invmass);
          if (pt > 2 && pt <= 2.5)
            fhPhotons_invmass_ptBin5_new[index]->Fill(invmass);
          if (pt > 2.5 && pt <= 3)
            fhPhotons_invmass_ptBin6_new[index]->Fill(invmass);
          fhPhotons_invmass_vs_pt_new[index]->Fill(invmass, pt);

          /*
					Double_t chicut = 1.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi1->Fill(invmass);
					}
					chicut = 3.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi3->Fill(invmass);
					}
					chicut = 5.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi5->Fill(invmass);
					}
					chicut = 10.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi10->Fill(invmass);
					}
					chicut = 25.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi25->Fill(invmass);
					}
					chicut = 40.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi40->Fill(invmass);
					}
					chicut = 65.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi65->Fill(invmass);
					}
					chicut = 80.0;
					if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
						fhPhotons_invmass_cut_chi80->Fill(invmass);
					}
					*/


          if (fElectrons_mctrackID_new[index][electron11] == 0
              || fElectrons_mctrackID_new[index][electron12] == 0
              || fElectrons_mctrackID_new[index][electron21] == 0
              || fElectrons_mctrackID_new[index][electron22] == 0)
            continue;

          // CROSSCHECK WITH MC-TRUE DATA!
          CbmMCTrack* mctrack11 = (CbmMCTrack*) fMcTracks->At(
            fElectrons_mctrackID_new[index]
                                    [electron11]);  // mctracks of four leptons
          CbmMCTrack* mctrack12 = (CbmMCTrack*) fMcTracks->At(
            fElectrons_mctrackID_new[index][electron12]);
          CbmMCTrack* mctrack21 = (CbmMCTrack*) fMcTracks->At(
            fElectrons_mctrackID_new[index][electron21]);
          CbmMCTrack* mctrack22 = (CbmMCTrack*) fMcTracks->At(
            fElectrons_mctrackID_new[index][electron22]);

          Int_t pdg11 = mctrack11->GetPdgCode();  // pdg codes of four leptons
          Int_t pdg12 = mctrack12->GetPdgCode();
          Int_t pdg21 = mctrack21->GetPdgCode();
          Int_t pdg22 = mctrack22->GetPdgCode();

          Int_t motherId11 =
            mctrack11->GetMotherId();  // motherIDs of four leptons
          Int_t motherId12 = mctrack12->GetMotherId();
          Int_t motherId21 = mctrack21->GetMotherId();
          Int_t motherId22 = mctrack22->GetMotherId();

          CbmMCTrack* mothermctrack11 =
            NULL;  // mctracks of mother particles of the four leptons
          CbmMCTrack* mothermctrack12 = NULL;
          CbmMCTrack* mothermctrack21 = NULL;
          CbmMCTrack* mothermctrack22 = NULL;
          if (motherId11 > 0)
            mothermctrack11 = (CbmMCTrack*) fMcTracks->At(motherId11);
          if (motherId11 > 0)
            mothermctrack12 = (CbmMCTrack*) fMcTracks->At(motherId12);
          if (motherId11 > 0)
            mothermctrack21 = (CbmMCTrack*) fMcTracks->At(motherId21);
          if (motherId11 > 0)
            mothermctrack22 = (CbmMCTrack*) fMcTracks->At(motherId22);

          Int_t motherpdg11 = -2;  // pdg codes of the mother particles
          Int_t motherpdg12 = -2;
          Int_t motherpdg21 = -2;
          Int_t motherpdg22 = -2;
          if (mothermctrack11 != NULL)
            motherpdg11 = mothermctrack11->GetPdgCode();
          if (mothermctrack12 != NULL)
            motherpdg12 = mothermctrack12->GetPdgCode();
          if (mothermctrack21 != NULL)
            motherpdg21 = mothermctrack21->GetPdgCode();
          if (mothermctrack22 != NULL)
            motherpdg22 = mothermctrack22->GetPdgCode();

          Int_t grandmotherId11 = -2;  // grandmotherIDs of four leptons
          Int_t grandmotherId12 = -2;
          Int_t grandmotherId21 = -2;
          Int_t grandmotherId22 = -2;
          if (mothermctrack11 != NULL)
            grandmotherId11 = mothermctrack11->GetMotherId();
          if (mothermctrack12 != NULL)
            grandmotherId12 = mothermctrack12->GetMotherId();
          if (mothermctrack21 != NULL)
            grandmotherId21 = mothermctrack21->GetMotherId();
          if (mothermctrack22 != NULL)
            grandmotherId22 = mothermctrack22->GetMotherId();


          Int_t sameGrandmothers1 = 0;
          Int_t sameGrandmothers2 = 0;
          Int_t sameGrandmothers3 = 0;
          Int_t sameGrandmothers4 = 0;
          if (grandmotherId11 == grandmotherId12) sameGrandmothers1++;
          if (grandmotherId11 == grandmotherId21) sameGrandmothers1++;
          if (grandmotherId11 == grandmotherId22) sameGrandmothers1++;
          if (grandmotherId12 == grandmotherId11) sameGrandmothers2++;
          if (grandmotherId12 == grandmotherId21) sameGrandmothers2++;
          if (grandmotherId12 == grandmotherId22) sameGrandmothers2++;
          if (grandmotherId21 == grandmotherId11) sameGrandmothers3++;
          if (grandmotherId21 == grandmotherId12) sameGrandmothers3++;
          if (grandmotherId21 == grandmotherId22) sameGrandmothers3++;
          if (grandmotherId22 == grandmotherId11) sameGrandmothers4++;
          if (grandmotherId22 == grandmotherId12) sameGrandmothers4++;
          if (grandmotherId22 == grandmotherId21) sameGrandmothers4++;
          Int_t sameGrandmothersSum = sameGrandmothers1 + sameGrandmothers2
                                      + sameGrandmothers3 + sameGrandmothers4;

          Int_t sameMothers1 = 0;
          Int_t sameMothers2 = 0;
          Int_t sameMothers3 = 0;
          Int_t sameMothers4 = 0;
          if (motherId11 == motherId12) sameMothers1++;
          if (motherId11 == motherId21) sameMothers1++;
          if (motherId11 == motherId22) sameMothers1++;
          if (motherId12 == motherId11) sameMothers2++;
          if (motherId12 == motherId21) sameMothers2++;
          if (motherId12 == motherId22) sameMothers2++;
          if (motherId21 == motherId11) sameMothers3++;
          if (motherId21 == motherId12) sameMothers3++;
          if (motherId21 == motherId22) sameMothers3++;
          if (motherId22 == motherId11) sameMothers4++;
          if (motherId22 == motherId12) sameMothers4++;
          if (motherId22 == motherId21) sameMothers4++;
          Int_t sameMothersSum =
            sameMothers1 + sameMothers2 + sameMothers3 + sameMothers4;


          if (
            motherId11 == motherId12
            && motherId21
                 == motherId22) {  // both combined e+e- pairs come from the same mother (which can be gamma, pi0, or whatever)
            fhPhotons_invmass_MCcutAll_new[index]->Fill(1, invmass);
            if (TMath::Abs(motherpdg11) == 22
                && TMath::Abs(motherpdg21) == 22) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(2, invmass);
            }
            if (TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 22
                && grandmotherId11 == grandmotherId21
                && grandmotherId11 > 0) {  // decay in to gg of pi0 and eta
              fhPhotons_invmass_MCcutAll_new[index]->Fill(3, invmass);
              if (invmass < 0.3)
                fhPhotons_phaseSpace_pi0[index]->Fill(paramsTest.fPt,
                                                      paramsTest.fRapidity);
              if (invmass > 0.3)
                fhPhotons_phaseSpace_eta[index]->Fill(paramsTest.fPt,
                                                      paramsTest.fRapidity);
            }
            if (TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 22
                && grandmotherId11 != grandmotherId21) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(4, invmass);
            }
            if ((TMath::Abs(motherpdg11) == 22
                 && TMath::Abs(motherpdg21) == 111)
                || (TMath::Abs(motherpdg11) == 111
                    && TMath::Abs(motherpdg21) == 22)) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(5, invmass);
              if (grandmotherId11 == motherId21
                  || motherId11 == grandmotherId21) {  // Dalitz decay
                fhPhotons_invmass_MCcutAll_new[index]->Fill(12, invmass);
                fhPhotons_phaseSpace_pi0[index]->Fill(paramsTest.fPt,
                                                      paramsTest.fRapidity);
              }
            }
            if (TMath::Abs(motherpdg11) == 111
                && TMath::Abs(motherpdg21) == 111) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(6, invmass);
            }
            if ((TMath::Abs(motherpdg11) != 22
                 && TMath::Abs(motherpdg11) != 111)
                || (TMath::Abs(motherpdg21) != 22
                    && TMath::Abs(motherpdg21) != 111)) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(7, invmass);
            }
            if (TMath::Abs(motherpdg11) != 22 && TMath::Abs(motherpdg11) != 111
                && TMath::Abs(motherpdg21) != 22
                && TMath::Abs(motherpdg21) != 111) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(8, invmass);
            }
            if (grandmotherId11 == grandmotherId21) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(9, invmass);
            }
          }
          if ((motherId11 == motherId12 && motherId21 != motherId22)
              || (motherId11 != motherId12 && motherId21 == motherId22)) {
            fhPhotons_invmass_MCcutAll_new[index]->Fill(10, invmass);
          }
          if (motherId11 != motherId12 && motherId21 != motherId22) {
            fhPhotons_invmass_MCcutAll_new[index]->Fill(11, invmass);
            if (sameGrandmothersSum == 12)
              fhPhotons_invmass_MCcutAll_new[index]->Fill(13, invmass);
            if (sameGrandmothersSum == 6)
              fhPhotons_invmass_MCcutAll_new[index]->Fill(14, invmass);
            if (sameGrandmothersSum == 4) {
              fhPhotons_invmass_MCcutAll_new[index]->Fill(15, invmass);
              if (grandmotherId11 < 0 || grandmotherId12 < 0
                  || grandmotherId21 < 0 || grandmotherId22 < 0) {
                fhPhotons_invmass_MCcutAll_new[index]->Fill(16, invmass);
              }
              if (grandmotherId11 == grandmotherId12) {
                fhPhotons_invmass_MCcutAll_new[index]->Fill(17, invmass);
              }
              if (grandmotherId11 != grandmotherId12) {
                fhPhotons_invmass_MCcutAll_new[index]->Fill(18, invmass);
              }
              if ((motherId11 == motherId21 || motherId11 == motherId22)
                  && (motherId12 == motherId21 || motherId11 == motherId22)) {
                fhPhotons_invmass_MCcutAll_new[index]->Fill(19, invmass);
              }
            }
            if (sameGrandmothersSum == 2)
              fhPhotons_invmass_MCcutAll_new[index]->Fill(20, invmass);
            if (sameGrandmothersSum == 0)
              fhPhotons_invmass_MCcutAll_new[index]->Fill(21, invmass);
            fhPhotons_peakCheck1[index]->Fill(sameGrandmothersSum);
            fhPhotons_peakCheck2[index]->Fill(sameMothersSum);

            cout << "CbmAnaConversionRecoFull: MC-Crosscheck: " << electron11
                 << "/" << electron12 << "/" << electron21 << "/" << electron22
                 << " - " << pdg11 << "/" << pdg12 << "/" << pdg21 << "/"
                 << pdg22 << " - " << motherId11 << "/" << motherId12 << "/"
                 << motherId21 << "/" << motherId22 << " - " << motherpdg11
                 << "/" << motherpdg12 << "/" << motherpdg21 << "/"
                 << motherpdg22 << endl;
            cout << "CbmAnaConversionRecoFull: MC-Crosscheck: "
                 << grandmotherId11 << "/" << grandmotherId12 << "/"
                 << grandmotherId21 << "/" << grandmotherId22 << endl;
          }


          // ################################################
          if (index == 4 && TMath::Abs(pdg11) == 11 && TMath::Abs(pdg12) == 11
              && TMath::Abs(pdg21) == 11 && TMath::Abs(pdg22) == 11) {
            fhPhotons_invmass_MCcutTest2_new[index]->Fill(invmass);
          }
          if (index == 4 && (pdg11 + pdg12 + pdg21 + pdg22) == 0) {
            fhPhotons_invmass_MCcutTest3_new[index]->Fill(invmass);
          }

          if (index == 4 && invmass < 0.1) {
            fhPhotons_MCtrue_pdgCodes->Fill(TMath::Abs(pdg11));
            fhPhotons_MCtrue_pdgCodes->Fill(TMath::Abs(pdg21));
            fhPhotons_MCtrue_pdgCodes->Fill(TMath::Abs(pdg12));
            fhPhotons_MCtrue_pdgCodes->Fill(TMath::Abs(pdg22));
          }


          TVector3 startvertex11;
          mctrack11->GetStartVertex(startvertex11);
          TVector3 startvertex12;
          mctrack12->GetStartVertex(startvertex12);
          TVector3 startvertex21;
          mctrack21->GetStartVertex(startvertex21);
          TVector3 startvertex22;
          mctrack22->GetStartVertex(startvertex22);

          //fhPhotons_MC_startvertexZ->Fill(startvertex11.Z());
          //fhPhotons_MC_startvertexZ->Fill(startvertex12.Z());
          //fhPhotons_MC_startvertexZ->Fill(startvertex21.Z());
          //fhPhotons_MC_startvertexZ->Fill(startvertex22.Z());

          fhPhotons_stats[index]->Fill(0);  // all combinations

          if (motherId11 == motherId12 && motherId21 == motherId22) {
            fhPhotons_invmass_MCcut1_new[index]->Fill(invmass);
            fhPhotons_stats[index]->Fill(1);
            if (motherId11 != -1 && motherId21 != -1) {
              CbmMCTrack* mothermctrack11n =
                (CbmMCTrack*) fMcTracks->At(motherId11);
              CbmMCTrack* mothermctrack21n =
                (CbmMCTrack*) fMcTracks->At(motherId21);

              Int_t grandmotherId11n = mothermctrack11n->GetMotherId();
              Int_t grandmotherId21n = mothermctrack21n->GetMotherId();

              if (grandmotherId11n == grandmotherId21n) {
                fhPhotons_invmass_MCcut2_new[index]->Fill(invmass);
                fhPhotons_stats[index]->Fill(2);
                if (grandmotherId11n == -1) continue;
                CbmMCTrack* pi0mctrack11 =
                  (CbmMCTrack*) fMcTracks->At(grandmotherId11n);
                if (pi0mctrack11->GetMotherId() == -1) {
                  //fhPhotons_MC_motherIdCut->Fill(invmass);
                }
              }
              if (grandmotherId11n != grandmotherId21n) {
                fhPhotons_invmass_MCcut3_new[index]->Fill(invmass);
                fhPhotons_stats[index]->Fill(3);
              }
            }
          }

          if (motherId11 != motherId12 || motherId21 != motherId22) {
            fhPhotons_invmass_MCcut4_new[index]->Fill(invmass);
            fhPhotons_stats[index]->Fill(4);
          }
          if ((motherId11 != motherId12 && motherId21 == motherId22)
              || (motherId11 == motherId12 && motherId21 != motherId22)) {
            fhPhotons_invmass_MCcut5_new[index]->Fill(invmass);
            fhPhotons_stats[index]->Fill(5);
          }
          if (motherId11 != motherId12 && motherId21 != motherId22) {
            fhPhotons_invmass_MCcut6_new[index]->Fill(invmass);
            fhPhotons_stats[index]->Fill(6);
            if (TMath::Abs(pdg11) != 11 || TMath::Abs(pdg12) != 11
                || TMath::Abs(pdg21) != 11 || TMath::Abs(pdg22) != 11) {
              fhPhotons_invmass_MCcut7_new[index]->Fill(invmass);
            }
          }


          /*
					if(motherId11 != -1 && motherId12 != -1 && motherId21 != -1 && motherId22 != -1) {
						CbmMCTrack* mothermctrack11 = (CbmMCTrack*)fMcTracks->At(motherId11);
						CbmMCTrack* mothermctrack12 = (CbmMCTrack*)fMcTracks->At(motherId12);
						CbmMCTrack* mothermctrack21 = (CbmMCTrack*)fMcTracks->At(motherId21);
						CbmMCTrack* mothermctrack22 = (CbmMCTrack*)fMcTracks->At(motherId22);
					
						fhPhotons_MC_motherpdg->Fill(mothermctrack11->GetPdgCode());
						fhPhotons_MC_motherpdg->Fill(mothermctrack12->GetPdgCode());
						fhPhotons_MC_motherpdg->Fill(mothermctrack21->GetPdgCode());
						fhPhotons_MC_motherpdg->Fill(mothermctrack22->GetPdgCode());
					}
					
					
					
					TLorentzVector lorVecE11c;
					TLorentzVector lorVecE12c;
					TLorentzVector lorVecE21c;
					TLorentzVector lorVecE22c;
					
					mctrack11->Get4Momentum(lorVecE11c);
					mctrack12->Get4Momentum(lorVecE12c);
					mctrack21->Get4Momentum(lorVecE21c);
					mctrack22->Get4Momentum(lorVecE22c);
					
					TLorentzVector g1c = lorVecE11c + lorVecE12c;
					TLorentzVector g2c = lorVecE21c + lorVecE22c;
					TLorentzVector pic = lorVecE11c + lorVecE12c + lorVecE21c + lorVecE22c;
					
					g1c.Boost(-pic.BoostVector() );
					g2c.Boost(-pic.BoostVector() );
					
					Double_t boostAnglec = g1c.Angle(g2c.Vect());
					Double_t thetac = 180.*boostAnglec/TMath::Pi();
					fhPhotons_boostAngleMC->Fill(thetac);
					
					
					*/
        }
      }
    }
  }
  fhPi0_nofPerEvent_new[index]->Fill(nofPi0);
}


Double_t
CbmAnaConversionRecoFull::OpeningAngleBetweenPhotons(vector<TVector3> momenta,
                                                     vector<int> photon1,
                                                     vector<int> photon2) {
  Double_t energy1 = TMath::Sqrt(momenta[photon1[0]].Mag2() + M2E);
  TLorentzVector lorVec1(momenta[photon1[0]], energy1);

  Double_t energy2 = TMath::Sqrt(momenta[photon1[1]].Mag2() + M2E);
  TLorentzVector lorVec2(momenta[photon1[1]], energy2);

  Double_t energy3 = TMath::Sqrt(momenta[photon2[0]].Mag2() + M2E);
  TLorentzVector lorVec3(momenta[photon2[0]], energy3);

  Double_t energy4 = TMath::Sqrt(momenta[photon2[1]].Mag2() + M2E);
  TLorentzVector lorVec4(momenta[photon2[1]], energy4);

  TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
  TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

  Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
  Double_t theta               = 180. * angleBetweenPhotons / TMath::Pi();

  return theta;
}


Double_t
CbmAnaConversionRecoFull::OpeningAngleBetweenPhotons2(vector<int> photon1,
                                                      vector<int> photon2) {
  Double_t energy1 = TMath::Sqrt(fElectrons_momenta[photon1[0]].Mag2() + M2E);
  TLorentzVector lorVec1(fElectrons_momenta[photon1[0]], energy1);

  Double_t energy2 = TMath::Sqrt(fElectrons_momenta[photon1[1]].Mag2() + M2E);
  TLorentzVector lorVec2(fElectrons_momenta[photon1[1]], energy2);

  Double_t energy3 = TMath::Sqrt(fElectrons_momenta[photon2[0]].Mag2() + M2E);
  TLorentzVector lorVec3(fElectrons_momenta[photon2[0]], energy3);

  Double_t energy4 = TMath::Sqrt(fElectrons_momenta[photon2[1]].Mag2() + M2E);
  TLorentzVector lorVec4(fElectrons_momenta[photon2[1]], energy4);

  TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
  TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

  Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
  Double_t theta               = 180. * angleBetweenPhotons / TMath::Pi();

  return theta;
}


/*
void CbmAnaConversionRecoFull::CombineElectronsRefit()
{
	Int_t nof = fElectrons_momenta_refit.size();
	cout << "CbmAnaConversionRecoFull: CombineElectronsRefit, nof - " << nof << endl;
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				Int_t check1 = (fElectrons_track_refit[a]->GetParamLast()->GetQp() > 0);	// positive or negative charge (qp = charge over momentum ratio)
				Int_t check2 = (fElectrons_track_refit[b]->GetParamLast()->GetQp() > 0);
				Int_t test = check1 + check2;
				if(test != 1) continue;		// need one electron and one positron
				//if(fElectrons_momentaChi[a] > 10 || fElectrons_momentaChi[b] > 10) continue;
				
				CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fElectrons_momenta_refit[a], fElectrons_momenta_refit[b]);
				
				Double_t openingAngleCut = 1;
				Double_t invMassCut = 0.03;
				
				Int_t IsPhoton_openingAngle1	= (params1.fAngle < openingAngleCut);
				Int_t IsPhoton_invMass1			= (params1.fMinv < invMassCut);
				
				if(IsPhoton_openingAngle1 && IsPhoton_invMass1) {
					vector<int> pair; // = {a, b};
					pair.push_back(a);
					pair.push_back(b);
					fVector_photons_pairs_refit.push_back(pair);
				}
			}
		}
	}
}




void CbmAnaConversionRecoFull::CombinePhotonsRefit()
{
	Int_t nof = fVector_photons_pairs_refit.size();
	cout << "CbmAnaConversionRecoFull: CombinePhotonsRefit, nof - " << nof << endl;
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				Int_t electron11 = fVector_photons_pairs_refit[a][0];
				Int_t electron12 = fVector_photons_pairs_refit[a][1];
				Int_t electron21 = fVector_photons_pairs_refit[b][0];
				Int_t electron22 = fVector_photons_pairs_refit[b][1];
			
				Double_t invmass = Invmass_4particlesRECO(fElectrons_momenta_refit[electron11], fElectrons_momenta_refit[electron12], fElectrons_momenta_refit[electron21], fElectrons_momenta_refit[electron22]);
				fhPhotons_invmass_refit->Fill(invmass);
				
				Double_t opening_angle = OpeningAngleBetweenPhotonsRefit(fVector_photons_pairs_refit[a], fVector_photons_pairs_refit[b]);
				//fhPhotons_angleBetween->Fill(opening_angle);
				
				if(opening_angle < 8) {
					fhPhotons_invmass_refit_cut->Fill(invmass);
					
					//Double_t chicut = 1.0;
					//if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
					//	fhPhotons_invmass_cut_chi1->Fill(invmass);
					//}
				}
			}
		}
	}
}




Double_t CbmAnaConversionRecoFull::OpeningAngleBetweenPhotonsRefit(vector<int> photon1, vector<int> photon2)
{
	Double_t energy1 = TMath::Sqrt(fElectrons_momenta_refit[photon1[0]].Mag2() + M2E);
	TLorentzVector lorVec1(fElectrons_momenta_refit[photon1[0]], energy1);
	
	Double_t energy2 = TMath::Sqrt(fElectrons_momenta_refit[photon1[1]].Mag2() + M2E);
	TLorentzVector lorVec2(fElectrons_momenta_refit[photon1[1]], energy2);
	
	Double_t energy3 = TMath::Sqrt(fElectrons_momenta_refit[photon2[0]].Mag2() + M2E);
	TLorentzVector lorVec3(fElectrons_momenta_refit[photon2[0]], energy3);
	
	Double_t energy4 = TMath::Sqrt(fElectrons_momenta_refit[photon2[1]].Mag2() + M2E);
	TLorentzVector lorVec4(fElectrons_momenta_refit[photon2[1]], energy4);

	TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
	TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

	Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
	Double_t theta = 180.*angleBetweenPhotons/TMath::Pi();

	return theta;
}



void CbmAnaConversionRecoFull::CombinePhotonsDirection()
{
	Int_t nof = fVector_photons_pairs_direction.size();
	cout << "CbmAnaConversionRecoFull: CombinePhotonsDirection, nof - " << nof << endl;
	if(nof >= 2) {
		for(int a=0; a<nof-1; a++) {
			for(int b=a+1; b<nof; b++) {
				Int_t electron11 = fVector_photons_pairs_direction[a][0];
				Int_t electron12 = fVector_photons_pairs_direction[a][1];
				Int_t electron21 = fVector_photons_pairs_direction[b][0];
				Int_t electron22 = fVector_photons_pairs_direction[b][1];
			
				Double_t invmass = Invmass_4particlesRECO(fElectrons_momenta[electron11], fElectrons_momenta[electron12], fElectrons_momenta[electron21], fElectrons_momenta[electron22]);
				fhPhotons_invmass_direction->Fill(invmass);
				
				Double_t opening_angle = OpeningAngleBetweenPhotonsRefit(fVector_photons_pairs_direction[a], fVector_photons_pairs_direction[b]);
				//fhPhotons_angleBetween->Fill(opening_angle);
				
				if(opening_angle < 8) {
					fhPhotons_invmass_direction_cut->Fill(invmass);
					
					//Double_t chicut = 1.0;
					//if(fElectrons_momentaChi[electron11] < chicut && fElectrons_momentaChi[electron12] < chicut && fElectrons_momentaChi[electron21] < chicut && fElectrons_momentaChi[electron22] < chicut) {
					//	fhPhotons_invmass_cut_chi1->Fill(invmass);
					//}
				}
			}
		}
	}
}
*/


/*
void CbmAnaConversionRecoFull::CombineElectrons()
{
	Int_t nof = fElectrons_momenta.size();
	if(nof >= 4) {
		for(int a=0; a<nof-3; a++) {
			for(int b=a; b<nof-2; b++) {
				for(int c=b; c<nof-1; c++) {
					for(int d=c; d<nof; d++) {
						Int_t check1 = (fElectrons_track[a]->GetParamLast()->GetQp() > 0);	// positive or negative charge (qp = charge over momentum ratio)
						Int_t check2 = (fElectrons_track[b]->GetParamLast()->GetQp() > 0);
						Int_t check3 = (fElectrons_track[c]->GetParamLast()->GetQp() > 0);
						Int_t check4 = (fElectrons_track[d]->GetParamLast()->GetQp() > 0);
						Int_t test = check1 + check2 + check3 + check4;
						if(test != 2) continue;		// need two electrons and two positrons
						
						
						Double_t invmass = Invmass_4particlesRECO(fElectrons_momenta[a], fElectrons_momenta[b], fElectrons_momenta[c], fElectrons_momenta[d]);
						fhElectrons_invmass->Fill(invmass);
						
						
						CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[b]);
						CbmLmvmKinematicParams params2 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[c]);
						CbmLmvmKinematicParams params3 = CalculateKinematicParamsReco(fElectrons_momenta[a], fElectrons_momenta[d]);
						CbmLmvmKinematicParams params4 = CalculateKinematicParamsReco(fElectrons_momenta[b], fElectrons_momenta[c]);
						CbmLmvmKinematicParams params5 = CalculateKinematicParamsReco(fElectrons_momenta[b], fElectrons_momenta[d]);
						CbmLmvmKinematicParams params6 = CalculateKinematicParamsReco(fElectrons_momenta[c], fElectrons_momenta[d]);
						
						Double_t openingAngleCut = 1;
						Int_t IsPhoton_openingAngle1 = (params1.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle2 = (params2.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle3 = (params3.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle4 = (params4.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle5 = (params5.fAngle < openingAngleCut);
						Int_t IsPhoton_openingAngle6 = (params6.fAngle < openingAngleCut);
						
						Double_t invMassCut = 0.03;
						Int_t IsPhoton_invMass1 = (params1.fMinv < invMassCut);
						Int_t IsPhoton_invMass2 = (params2.fMinv < invMassCut);
						Int_t IsPhoton_invMass3 = (params3.fMinv < invMassCut);
						Int_t IsPhoton_invMass4 = (params4.fMinv < invMassCut);
						Int_t IsPhoton_invMass5 = (params5.fMinv < invMassCut);
						Int_t IsPhoton_invMass6 = (params6.fMinv < invMassCut);
						
						if(IsPhoton_openingAngle1 && IsPhoton_openingAngle6 && IsPhoton_invMass1 && IsPhoton_invMass6 && (check1 + check2 == 1) && (check3 + check4 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
						if(IsPhoton_openingAngle2 && IsPhoton_openingAngle5 && IsPhoton_invMass2 && IsPhoton_invMass5 && (check1 + check3 == 1) && (check2 + check4 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
						if(IsPhoton_openingAngle3 && IsPhoton_openingAngle4 && IsPhoton_invMass3 && IsPhoton_invMass4 && (check1 + check4 == 1) && (check2 + check3 == 1)) {
							fhElectrons_invmass_cut->Fill(invmass);
						}
					}
				}
			}
		}
	}
}
*/


void CbmAnaConversionRecoFull::CombineElectrons() {
  Int_t nof = fElectrons_momenta.size();
  cout << "CbmAnaConversionRecoFull: CombineElectrons, nof - " << nof << endl;
  Int_t nofPhotons = 0;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t check1 =
          (fElectrons_track[a]->GetParamLast()->GetQp()
           > 0);  // positive or negative charge (qp = charge over momentum ratio)
        Int_t check2 = (fElectrons_track[b]->GetParamLast()->GetQp() > 0);
        Int_t test   = check1 + check2;
        if (test != 1) continue;  // need one electron and one positron
        //if(fElectrons_momentaChi[a] > 10 || fElectrons_momentaChi[b] > 10) continue;

        CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(
          fElectrons_momenta[a], fElectrons_momenta[b]);

        // standard fixed opening angle cut
        //Double_t openingAngleCut = 1;

        // opening angle cut depending on pt of e+e- pair
        Double_t openingAngleCut = 1.8 - 0.6 * params1.fPt;
        //Double_t openingAngleCut = 1.5 - 0.5 * params1.fPt;
        //Double_t openingAngleCut = 1.2;

        Double_t invMassCut = 0.03;

        Int_t IsPhoton_openingAngle1 = (params1.fAngle < openingAngleCut);
        Int_t IsPhoton_invMass1      = (params1.fMinv < invMassCut);

        Double_t tXa = fElectrons_track[a]->GetParamLast()->GetTx();
        Double_t tYa = fElectrons_track[a]->GetParamLast()->GetTy();
        Double_t tXb = fElectrons_track[b]->GetParamLast()->GetTx();
        Double_t tYb = fElectrons_track[b]->GetParamLast()->GetTy();

        fhElectrons_invmass->Fill(params1.fMinv);

        if (IsPhoton_openingAngle1 && IsPhoton_invMass1) {
          nofPhotons++;
          vector<int> pair;  // = {a, b};
          pair.push_back(a);
          pair.push_back(b);
          fVector_photons_pairs.push_back(pair);
          fhElectrons_invmass_cut->Fill(params1.fMinv);
          //fVector_photons_momenta.push_back(params1.momPair);

          fhPhotons_tX->Fill(tXa);
          fhPhotons_tX->Fill(tXb);
          fhPhotons_tY->Fill(tYa);
          fhPhotons_tY->Fill(tYb);

          TVector3 momentumE1f;
          TVector3 momentumE2f;
          fElectrons_track[a]->GetParamFirst()->Momentum(momentumE1f);
          fElectrons_track[b]->GetParamFirst()->Momentum(momentumE2f);
          TVector3 momentumE1l;
          TVector3 momentumE2l;
          fElectrons_track[a]->GetParamLast()->Momentum(momentumE1l);
          fElectrons_track[b]->GetParamLast()->Momentum(momentumE2l);

          //Double_t energyE1 = TMath::Sqrt(momentumE1.Mag2() + M2E);
          //TLorentzVector lorVecE1(momentumE1, energyE1);
          //Double_t energyE2 = TMath::Sqrt(momentumE2.Mag2() + M2E);
          //TLorentzVector lorVecE2(momentumE2, energyE2);
          //TLorentzVector g = lorVecE1 + lorVecE2;

          //lorVecE1.Boost(g.BoostVector() );
          //lorVecE2.Boost(g.BoostVector() );

          TVector3 normal1     = momentumE1f.Cross(momentumE1l);
          TVector3 normal2     = momentumE2f.Cross(momentumE2l);
          Double_t normalAngle = normal1.Angle(normal2);
          Double_t theta       = 180. * normalAngle / TMath::Pi();

          //Double_t boostAngle = lorVecE1.Angle(lorVecE2.Vect());

          //fhPhotons_boostAngle->Fill(normalAngle);


          //if( TMath::Abs(tXa - tXb) < 0.5 || TMath::Abs(tYa - tYb) < 0.5 ) {
          if (theta < 30) { fVector_photons_pairs_direction.push_back(pair); }
        }
      }
    }
  }
  fhPhotons_nofPerEvent->Fill(nofPhotons);
}


void CbmAnaConversionRecoFull::CombinePhotons() {
  Int_t nof = fVector_photons_pairs.size();
  cout << "CbmAnaConversionRecoFull: CombinePhotons, nof - " << nof << endl;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t electron11 = fVector_photons_pairs[a][0];
        Int_t electron12 = fVector_photons_pairs[a][1];
        Int_t electron21 = fVector_photons_pairs[b][0];
        Int_t electron22 = fVector_photons_pairs[b][1];


        if (electron11 == electron12 || electron11 == electron21
            || electron11 == electron22 || electron12 == electron21
            || electron12 == electron22 || electron21 == electron22) {
          cout << "CbmAnaConversionRecoFull: "
                  "RecoFull_CombinePhotons()_DoubleIndex!"
               << endl;
          continue;
        }

        Double_t invmass =
          Invmass_4particlesRECO(fElectrons_momenta[electron11],
                                 fElectrons_momenta[electron12],
                                 fElectrons_momenta[electron21],
                                 fElectrons_momenta[electron22]);
        fhPhotons_invmass->Fill(invmass);

        Double_t pt  = Pt_4particlesRECO(fElectrons_momenta[electron11],
                                        fElectrons_momenta[electron12],
                                        fElectrons_momenta[electron21],
                                        fElectrons_momenta[electron22]);
        Double_t rap = Rap_4particlesRECO(fElectrons_momenta[electron11],
                                          fElectrons_momenta[electron12],
                                          fElectrons_momenta[electron21],
                                          fElectrons_momenta[electron22]);

        Double_t opening_angle = OpeningAngleBetweenPhotons2(
          fVector_photons_pairs[a], fVector_photons_pairs[b]);
        fhPhotons_angleBetween->Fill(opening_angle);


        CbmLmvmKinematicParams params1 =
          CalculateKinematicParams_4particles(fElectrons_momenta[electron11],
                                              fElectrons_momenta[electron12],
                                              fElectrons_momenta[electron21],
                                              fElectrons_momenta[electron22]);


        //TVector3 momentumE1;
        //TVector3 momentumE2;
        //fElectrons_track[a]->GetParamLast()->Momentum(momentumE1);
        //fElectrons_track[b]->GetParamLast()->Momentum(momentumE2);

        Double_t energyE11 =
          TMath::Sqrt(fElectrons_momenta[electron11].Mag2() + M2E);
        TLorentzVector lorVecE11(fElectrons_momenta[electron11], energyE11);
        Double_t energyE12 =
          TMath::Sqrt(fElectrons_momenta[electron12].Mag2() + M2E);
        TLorentzVector lorVecE12(fElectrons_momenta[electron12], energyE12);
        Double_t energyE21 =
          TMath::Sqrt(fElectrons_momenta[electron21].Mag2() + M2E);
        TLorentzVector lorVecE21(fElectrons_momenta[electron21], energyE21);
        Double_t energyE22 =
          TMath::Sqrt(fElectrons_momenta[electron22].Mag2() + M2E);
        TLorentzVector lorVecE22(fElectrons_momenta[electron22], energyE22);
        TLorentzVector g1 = lorVecE11 + lorVecE12;
        TLorentzVector g2 = lorVecE21 + lorVecE22;
        TLorentzVector pi = lorVecE11 + lorVecE12 + lorVecE21 + lorVecE22;

        g1.Boost(-pi.BoostVector());
        g2.Boost(-pi.BoostVector());

        Double_t boostAngle = g1.Angle(g2.Vect());
        Double_t theta      = 180. * boostAngle / TMath::Pi();

        fhPhotons_boostAngleTest->Fill(boostAngle);

        //if(opening_angle < 10) {
        if (true) {
          fhPhotons_boostAngle->Fill(theta);

          fhPhotons_invmass_cut->Fill(invmass);
          fhPhotons_invmass_vs_pt->Fill(invmass, pt);
          fhPhotons_rapidity_vs_pt->Fill(pt, rap);
          fhPhotons_invmass_vs_openingAngle->Fill(invmass, opening_angle);
          fhPhotons_openingAngle_vs_momentum->Fill(opening_angle,
                                                   params1.fMomentumMag);

          fhMomentumFits_pi0reco->Fill(fElectrons_momentaChi[electron11]);
          fhMomentumFits_pi0reco->Fill(fElectrons_momentaChi[electron12]);
          fhMomentumFits_pi0reco->Fill(fElectrons_momentaChi[electron21]);
          fhMomentumFits_pi0reco->Fill(fElectrons_momentaChi[electron22]);

          fhPhotons_invmass_vs_chi->Fill(invmass,
                                         fElectrons_momentaChi[electron11]);
          fhPhotons_invmass_vs_chi->Fill(invmass,
                                         fElectrons_momentaChi[electron12]);
          fhPhotons_invmass_vs_chi->Fill(invmass,
                                         fElectrons_momentaChi[electron21]);
          fhPhotons_invmass_vs_chi->Fill(invmass,
                                         fElectrons_momentaChi[electron22]);

          if (pt <= 1) fhPhotons_invmass_cut_ptBin1->Fill(invmass);
          if (pt > 1 && pt <= 2) fhPhotons_invmass_cut_ptBin2->Fill(invmass);
          if (pt > 2 && pt <= 3) fhPhotons_invmass_cut_ptBin3->Fill(invmass);
          if (pt > 3 && pt <= 4) fhPhotons_invmass_cut_ptBin4->Fill(invmass);

          Double_t chicut = 1.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi1->Fill(invmass);
          }
          chicut = 3.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi3->Fill(invmass);
          }
          chicut = 5.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi5->Fill(invmass);
          }
          chicut = 10.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi10->Fill(invmass);
          }
          chicut = 25.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi25->Fill(invmass);
          }
          chicut = 40.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi40->Fill(invmass);
          }
          chicut = 65.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi65->Fill(invmass);
          }
          chicut = 80.0;
          if (fElectrons_momentaChi[electron11] < chicut
              && fElectrons_momentaChi[electron12] < chicut
              && fElectrons_momentaChi[electron21] < chicut
              && fElectrons_momentaChi[electron22] < chicut) {
            fhPhotons_invmass_cut_chi80->Fill(invmass);
          }


          if (fElectrons_mctrackID[electron11] == 0
              || fElectrons_mctrackID[electron12] == 0
              || fElectrons_mctrackID[electron21] == 0
              || fElectrons_mctrackID[electron22] == 0)
            continue;
          // CROSSCHECK WITH MC-TRUE DATA!
          CbmMCTrack* mctrack11 =
            (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID[electron11]);
          CbmMCTrack* mctrack12 =
            (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID[electron12]);
          CbmMCTrack* mctrack21 =
            (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID[electron21]);
          CbmMCTrack* mctrack22 =
            (CbmMCTrack*) fMcTracks->At(fElectrons_mctrackID[electron22]);

          Int_t motherId11 = mctrack11->GetMotherId();
          Int_t motherId12 = mctrack12->GetMotherId();
          Int_t motherId21 = mctrack21->GetMotherId();
          Int_t motherId22 = mctrack22->GetMotherId();

          TVector3 startvertex11;
          mctrack11->GetStartVertex(startvertex11);
          TVector3 startvertex12;
          mctrack12->GetStartVertex(startvertex12);
          TVector3 startvertex21;
          mctrack21->GetStartVertex(startvertex21);
          TVector3 startvertex22;
          mctrack22->GetStartVertex(startvertex22);

          fhPhotons_MC_startvertexZ->Fill(startvertex11.Z());
          fhPhotons_MC_startvertexZ->Fill(startvertex12.Z());
          fhPhotons_MC_startvertexZ->Fill(startvertex21.Z());
          fhPhotons_MC_startvertexZ->Fill(startvertex22.Z());

          if (motherId11 == motherId12 && motherId21 == motherId22) {
            fhPhotons_MC_invmass1->Fill(invmass);
            if (motherId11 != -1 && motherId21 != -1) {
              CbmMCTrack* mothermctrack11 =
                (CbmMCTrack*) fMcTracks->At(motherId11);
              CbmMCTrack* mothermctrack21 =
                (CbmMCTrack*) fMcTracks->At(motherId21);

              Int_t grandmotherId11 = mothermctrack11->GetMotherId();
              Int_t grandmotherId21 = mothermctrack21->GetMotherId();

              if (grandmotherId11 == grandmotherId21) {
                fhPhotons_MC_invmass2->Fill(invmass);
                if (grandmotherId11 == -1) continue;
                CbmMCTrack* pi0mctrack11 =
                  (CbmMCTrack*) fMcTracks->At(grandmotherId11);
                if (pi0mctrack11->GetMotherId() == -1) {
                  fhPhotons_MC_motherIdCut->Fill(invmass);
                }
              }
              if (grandmotherId11 != grandmotherId21) {
                fhPhotons_MC_invmass3->Fill(invmass);
              }
            }
          }

          if (motherId11 != motherId12 || motherId21 != motherId22) {
            fhPhotons_MC_invmass4->Fill(invmass);
          }


          if (motherId11 != -1 && motherId12 != -1 && motherId21 != -1
              && motherId22 != -1) {
            CbmMCTrack* mothermctrack11 =
              (CbmMCTrack*) fMcTracks->At(motherId11);
            CbmMCTrack* mothermctrack12 =
              (CbmMCTrack*) fMcTracks->At(motherId12);
            CbmMCTrack* mothermctrack21 =
              (CbmMCTrack*) fMcTracks->At(motherId21);
            CbmMCTrack* mothermctrack22 =
              (CbmMCTrack*) fMcTracks->At(motherId22);

            fhPhotons_MC_motherpdg->Fill(mothermctrack11->GetPdgCode());
            fhPhotons_MC_motherpdg->Fill(mothermctrack12->GetPdgCode());
            fhPhotons_MC_motherpdg->Fill(mothermctrack21->GetPdgCode());
            fhPhotons_MC_motherpdg->Fill(mothermctrack22->GetPdgCode());
          }


          TLorentzVector lorVecE11c;
          TLorentzVector lorVecE12c;
          TLorentzVector lorVecE21c;
          TLorentzVector lorVecE22c;

          mctrack11->Get4Momentum(lorVecE11c);
          mctrack12->Get4Momentum(lorVecE12c);
          mctrack21->Get4Momentum(lorVecE21c);
          mctrack22->Get4Momentum(lorVecE22c);

          TLorentzVector g1c = lorVecE11c + lorVecE12c;
          TLorentzVector g2c = lorVecE21c + lorVecE22c;
          TLorentzVector pic =
            lorVecE11c + lorVecE12c + lorVecE21c + lorVecE22c;

          g1c.Boost(-pic.BoostVector());
          g2c.Boost(-pic.BoostVector());

          Double_t boostAnglec = g1c.Angle(g2c.Vect());
          Double_t thetac      = 180. * boostAnglec / TMath::Pi();
          fhPhotons_boostAngleMC->Fill(thetac);
        }
      }
    }
  }
}


void CbmAnaConversionRecoFull::MixedEventTest()
// takes all first identified electrons from all events and combines them afterwards
{
  for (int i = 1; i < 5; i++) {
    if (fMixedEventsElectrons[i].size() < 4) continue;
    for (UInt_t a = 0; a < fMixedEventsElectrons[i].size() - 3; a++) {
      for (UInt_t b = a + 1; b < fMixedEventsElectrons[i].size() - 2; b++) {
        for (UInt_t c = b + 1; c < fMixedEventsElectrons[i].size() - 1; c++) {
          for (UInt_t d = c + 1; d < fMixedEventsElectrons[i].size(); d++) {
            Int_t check1 =
              (fMixedEventsElectrons_gtrack[i][a]->GetParamLast()->GetQp() > 0);
            Int_t check2 =
              (fMixedEventsElectrons_gtrack[i][b]->GetParamLast()->GetQp() > 0);
            Int_t check3 =
              (fMixedEventsElectrons_gtrack[i][c]->GetParamLast()->GetQp() > 0);
            Int_t check4 =
              (fMixedEventsElectrons_gtrack[i][d]->GetParamLast()->GetQp() > 0);

            if (check1 + check2 + check3 + check4 != 2) continue;

            TVector3 e1 = fMixedEventsElectrons[i][a];
            TVector3 e2 = fMixedEventsElectrons[i][b];
            TVector3 e3 = fMixedEventsElectrons[i][c];
            TVector3 e4 = fMixedEventsElectrons[i][d];


            CbmAnaConversionKinematicParams params12 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e1, e2);
            CbmAnaConversionKinematicParams params13 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e1, e3);
            CbmAnaConversionKinematicParams params14 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e1, e4);
            CbmAnaConversionKinematicParams params23 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e2, e3);
            CbmAnaConversionKinematicParams params24 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e2, e4);
            CbmAnaConversionKinematicParams params34 =
              CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
                e3, e4);


            Int_t angleCheck12 =
              (params12.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params12.fPt)
               && check1 + check2 == 1);
            Int_t angleCheck13 =
              (params13.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params13.fPt)
               && check1 + check3 == 1);
            Int_t angleCheck14 =
              (params14.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params14.fPt)
               && check1 + check4 == 1);
            Int_t angleCheck23 =
              (params23.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params23.fPt)
               && check2 + check3 == 1);
            Int_t angleCheck24 =
              (params24.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params24.fPt)
               && check2 + check4 == 1);
            Int_t angleCheck34 =
              (params34.fAngle
                 < CbmAnaConversionCutSettings::CalcOpeningAngleCut(
                   params34.fPt)
               && check3 + check4 == 1);

            if (angleCheck12 && angleCheck34) {
              CbmAnaConversionKinematicParams params =
                CbmAnaConversionKinematicParams::
                  KinematicParams_4particles_Reco(e1, e2, e3, e4);
              fhMixedEventsTest_invmass[i]->Fill(params.fMinv);
              cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                      "filled!, part"
                   << i << endl;
            }
            if (angleCheck13 && angleCheck24) {
              CbmAnaConversionKinematicParams params =
                CbmAnaConversionKinematicParams::
                  KinematicParams_4particles_Reco(e1, e2, e3, e4);
              fhMixedEventsTest_invmass[i]->Fill(params.fMinv);
              cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                      "filled!, part"
                   << i << endl;
            }
            if (angleCheck14 && angleCheck23) {
              CbmAnaConversionKinematicParams params =
                CbmAnaConversionKinematicParams::
                  KinematicParams_4particles_Reco(e1, e2, e3, e4);
              fhMixedEventsTest_invmass[i]->Fill(params.fMinv);
              cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                      "filled!, part"
                   << i << endl;
            }
          }
        }
      }
    }
  }
}


void CbmAnaConversionRecoFull::MixedEventTest2()
// takes all identified electron from the last 4 events
{
  if (fMixedEventsElectrons_list1.size() == 0
      || fMixedEventsElectrons_list2.size() == 0
      || fMixedEventsElectrons_list3.size() == 0
      || fMixedEventsElectrons_list4.size() == 0)
    return;
  for (UInt_t a = 0; a < fMixedEventsElectrons_list1.size(); a++) {
    for (UInt_t b = 0; b < fMixedEventsElectrons_list2.size(); b++) {
      for (UInt_t c = 0; c < fMixedEventsElectrons_list3.size(); c++) {
        for (UInt_t d = 0; d < fMixedEventsElectrons_list4.size(); d++) {
          Int_t check1 =
            (fMixedEventsElectrons_list1_gtrack[a]->GetParamLast()->GetQp()
             > 0);
          Int_t check2 =
            (fMixedEventsElectrons_list2_gtrack[b]->GetParamLast()->GetQp()
             > 0);
          Int_t check3 =
            (fMixedEventsElectrons_list3_gtrack[c]->GetParamLast()->GetQp()
             > 0);
          Int_t check4 =
            (fMixedEventsElectrons_list4_gtrack[d]->GetParamLast()->GetQp()
             > 0);

          if (check1 + check2 + check3 + check4 != 2) continue;

          TVector3 e1 = fMixedEventsElectrons_list1[a];
          TVector3 e2 = fMixedEventsElectrons_list2[b];
          TVector3 e3 = fMixedEventsElectrons_list3[c];
          TVector3 e4 = fMixedEventsElectrons_list4[d];


          CbmAnaConversionKinematicParams params12 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e2);
          CbmAnaConversionKinematicParams params13 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e3);
          CbmAnaConversionKinematicParams params14 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e4);
          CbmAnaConversionKinematicParams params23 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e2, e3);
          CbmAnaConversionKinematicParams params24 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e2, e4);
          CbmAnaConversionKinematicParams params34 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e3, e4);


          Int_t angleCheck12 =
            (params12.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params12.fPt)
             && check1 + check2 == 1);
          Int_t angleCheck13 =
            (params13.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params13.fPt)
             && check1 + check3 == 1);
          Int_t angleCheck14 =
            (params14.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params14.fPt)
             && check1 + check4 == 1);
          Int_t angleCheck23 =
            (params23.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params23.fPt)
             && check2 + check3 == 1);
          Int_t angleCheck24 =
            (params24.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params24.fPt)
             && check2 + check4 == 1);
          Int_t angleCheck34 =
            (params34.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params34.fPt)
             && check3 + check4 == 1);

          if (angleCheck12 && angleCheck34) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest2_invmass->Fill(params.fMinv);
            cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                    "filled!, part"
                 << endl;
          }
          if (angleCheck13 && angleCheck24) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest2_invmass->Fill(params.fMinv);
            cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                    "filled!, part"
                 << endl;
          }
          if (angleCheck14 && angleCheck23) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest2_invmass->Fill(params.fMinv);
            cout << "CbmAnaConversionRecoFull: MixedEventTest(), event "
                    "filled!, part"
                 << endl;
          }
        }
      }
    }
  }
}


void CbmAnaConversionRecoFull::MixedEventTest3()
// takes all electrons from the last 20 events and combines them
{
  Int_t nof = fMixedTest3_momenta.size();
  cout << "CbmAnaConversionRecoFull: MixedEventTest3 - nof entries " << nof
       << endl;
  for (Int_t a = 0; a < nof - 3; a++) {
    for (Int_t b = a + 1; b < nof - 1; b++) {
      for (Int_t c = b + 1; c < nof - 1; c++) {
        for (Int_t d = c + 1; d < nof; d++) {
          if (fMixedTest3_eventno[a] == fMixedTest3_eventno[b]
              || fMixedTest3_eventno[a] == fMixedTest3_eventno[c]
              || fMixedTest3_eventno[a] == fMixedTest3_eventno[d]
              || fMixedTest3_eventno[b] == fMixedTest3_eventno[c]
              || fMixedTest3_eventno[b] == fMixedTest3_eventno[d]
              || fMixedTest3_eventno[c] == fMixedTest3_eventno[d])
            continue;
          Int_t check1 = (fMixedTest3_gtrack[a]->GetParamLast()->GetQp() > 0);
          Int_t check2 = (fMixedTest3_gtrack[b]->GetParamLast()->GetQp() > 0);
          Int_t check3 = (fMixedTest3_gtrack[c]->GetParamLast()->GetQp() > 0);
          Int_t check4 = (fMixedTest3_gtrack[d]->GetParamLast()->GetQp() > 0);

          if (check1 + check2 + check3 + check4 != 2) continue;

          TVector3 e1 = fMixedTest3_momenta[a];
          TVector3 e2 = fMixedTest3_momenta[b];
          TVector3 e3 = fMixedTest3_momenta[c];
          TVector3 e4 = fMixedTest3_momenta[d];


          CbmAnaConversionKinematicParams params12 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e2);
          CbmAnaConversionKinematicParams params13 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e3);
          CbmAnaConversionKinematicParams params14 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e1, e4);
          CbmAnaConversionKinematicParams params23 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e2, e3);
          CbmAnaConversionKinematicParams params24 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e2, e4);
          CbmAnaConversionKinematicParams params34 =
            CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
              e3, e4);


          Int_t angleCheck12 =
            (params12.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params12.fPt)
             && check1 + check2 == 1);
          Int_t angleCheck13 =
            (params13.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params13.fPt)
             && check1 + check3 == 1);
          Int_t angleCheck14 =
            (params14.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params14.fPt)
             && check1 + check4 == 1);
          Int_t angleCheck23 =
            (params23.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params23.fPt)
             && check2 + check3 == 1);
          Int_t angleCheck24 =
            (params24.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params24.fPt)
             && check2 + check4 == 1);
          Int_t angleCheck34 =
            (params34.fAngle
               < CbmAnaConversionCutSettings::CalcOpeningAngleCut(params34.fPt)
             && check3 + check4 == 1);

          if (angleCheck12 && angleCheck34) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest3_invmass->Fill(params.fMinv);
            //cout << "CbmAnaConversionRecoFull: MixedEventTest3(), event filled!, part" << endl;
          }
          if (angleCheck13 && angleCheck24) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest3_invmass->Fill(params.fMinv);
            //cout << "CbmAnaConversionRecoFull: MixedEventTest3(), event filled!, part" << endl;
          }
          if (angleCheck14 && angleCheck23) {
            CbmAnaConversionKinematicParams params =
              CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
                e1, e2, e3, e4);
            fhMixedEventsTest3_invmass->Fill(params.fMinv);
            //cout << "CbmAnaConversionRecoFull: MixedEventTest3(), event filled!, part" << endl;
          }
        }
      }
    }
  }
}


void CbmAnaConversionRecoFull::MixedEventTest4()
// combines photons from two different events, taken from each time 200 events
{
  Int_t nof = fMixedTest4_photons.size();
  cout << "CbmAnaConversionRecoFull: MixedEventTest4 - nof entries " << nof
       << endl;
  for (Int_t a = 0; a < nof - 1; a++) {
    for (Int_t b = a + 1; b < nof; b++) {
      if (fMixedTest4_eventno[a] == fMixedTest4_eventno[b]) continue;

      TVector3 e11 = fMixedTest4_photons[a][0];
      TVector3 e12 = fMixedTest4_photons[a][1];
      TVector3 e21 = fMixedTest4_photons[b][0];
      TVector3 e22 = fMixedTest4_photons[b][1];

      CbmMCTrack* mctrack11 = fMixedTest4_mctracks[a][0];
      CbmMCTrack* mctrack12 = fMixedTest4_mctracks[a][1];
      CbmMCTrack* mctrack21 = fMixedTest4_mctracks[b][0];
      CbmMCTrack* mctrack22 = fMixedTest4_mctracks[b][1];

      Int_t motherId11 = mctrack11->GetMotherId();
      Int_t motherId12 = mctrack12->GetMotherId();
      Int_t motherId21 = mctrack21->GetMotherId();
      Int_t motherId22 = mctrack22->GetMotherId();


      CbmAnaConversionKinematicParams params =
        CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
          e11, e12, e21, e22);
      fhMixedEventsTest4_invmass->Fill(params.fMinv);
      fhMixedEventsTest4_pt_vs_rap->Fill(params.fPt, params.fRapidity);
      //cout << "CbmAnaConversionRecoFull: MixedEventTest4(), event filled!, part" << endl;

      fhPhotons_stats[4]->Fill(8);  // all combinations
      if (motherId11 == motherId12 && motherId21 == motherId22) {
        fhPhotons_stats[4]->Fill(9);
      }
      if (motherId11 != motherId12 || motherId21 != motherId22) {
        fhPhotons_stats[4]->Fill(12);
      }
      if ((motherId11 != motherId12 && motherId21 == motherId22)
          || (motherId11 == motherId12 && motherId21 != motherId22)) {
        fhPhotons_stats[4]->Fill(13);
      }
      if (motherId11 != motherId12 && motherId21 != motherId22) {
        fhPhotons_stats[4]->Fill(14);
      }


      if (params.fPt <= 0.5)
        fhMixedEventsTest4_invmass_ptBin1->Fill(params.fMinv);
      if (params.fPt > 0.5 && params.fPt <= 1)
        fhMixedEventsTest4_invmass_ptBin2->Fill(params.fMinv);
      if (params.fPt > 1 && params.fPt <= 1.5)
        fhMixedEventsTest4_invmass_ptBin3->Fill(params.fMinv);
      if (params.fPt > 1.5 && params.fPt <= 2)
        fhMixedEventsTest4_invmass_ptBin4->Fill(params.fMinv);


      Bool_t IsRichElectron_ann0_e11 = fMixedTest4_isRichElectronAnn0[a][0];
      Bool_t IsRichElectron_ann0_e12 = fMixedTest4_isRichElectronAnn0[a][1];
      Bool_t IsRichElectron_ann0_e21 = fMixedTest4_isRichElectronAnn0[b][0];
      Bool_t IsRichElectron_ann0_e22 = fMixedTest4_isRichElectronAnn0[b][1];

      if (IsRichElectron_ann0_e11 && IsRichElectron_ann0_e12
          && IsRichElectron_ann0_e21 && IsRichElectron_ann0_e22) {
        fhMixedEventsTest4_invmass_ann0->Fill(params.fMinv);
      }


      Double_t ANNe11 = fMixedTest4_ElectronAnns[a][0];
      Double_t ANNe12 = fMixedTest4_ElectronAnns[a][1];
      Double_t ANNe21 = fMixedTest4_ElectronAnns[b][0];
      Double_t ANNe22 = fMixedTest4_ElectronAnns[b][1];

      if (ANNe11 > -1 && ANNe12 > -1 && ANNe21 > -1 && ANNe22 > -1)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(1, params.fMinv);
      if (ANNe11 > -0.9 && ANNe12 > -0.9 && ANNe21 > -0.9 && ANNe22 > -0.9)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(2, params.fMinv);
      if (ANNe11 > -0.8 && ANNe12 > -0.8 && ANNe21 > -0.8 && ANNe22 > -0.8)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(3, params.fMinv);
      if (ANNe11 > -0.7 && ANNe12 > -0.7 && ANNe21 > -0.7 && ANNe22 > -0.7)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(4, params.fMinv);
      if (ANNe11 > -0.6 && ANNe12 > -0.6 && ANNe21 > -0.6 && ANNe22 > -0.6)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(5, params.fMinv);
      if (ANNe11 > -0.5 && ANNe12 > -0.5 && ANNe21 > -0.5 && ANNe22 > -0.5)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(6, params.fMinv);
      if (ANNe11 > -0.0 && ANNe12 > -0.0 && ANNe21 > -0.0 && ANNe22 > -0.0)
        fhMixedEventsTest4_invmass_ANNcuts->Fill(7, params.fMinv);
    }
  }
}


Double_t CbmAnaConversionRecoFull::ElectronANNvalue(Int_t globalTrackIndex,
                                                    Double_t momentum) {
  if (NULL == fGlobalTracks || NULL == fRichRings) return -2;
  //CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
  const CbmGlobalTrack* globalTrack =
    static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Int_t richId = globalTrack->GetRichRingIndex();
  if (richId < 0) return -2;
  CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richId));
  if (NULL == ring) return -2;

  Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(
    globalTrackIndex, momentum);
  return ann;
}
