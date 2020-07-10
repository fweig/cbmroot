/**
 * \file CbmAnaConversion.cxx
 *
 * Base class for conversion analysis
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2014
 **/

#include "CbmAnaConversion.h"

// includes of CBMROOT classes
#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"
#include "FairLogger.h"
#include "FairMCPoint.h"
#include "FairTrackParam.h"

#include "CbmUtils.h"

#include "CbmL1PFFitter.h"
#include "CbmStsKFTrackFitter.h"
#include "L1Field.h"

#include "CbmKFParticleFinder.h"
#include "CbmKFParticleFinderQA.h"
#include "KFParticleTopoReconstructor.h"


// includes of standard c++ classes or ROOT classes
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3.h"
#include "TRandom3.h"
#include <boost/assign/list_of.hpp>
#include <iomanip>
#include <iostream>
#include <string>


// includes of further conversion classes
#include "CbmAnaConversionCutSettings.h"
#include "CbmAnaConversionKF.h"
#include "CbmAnaConversionPhotons.h"
#include "CbmAnaConversionPhotons2.h"
#include "CbmAnaConversionReco.h"
#include "CbmAnaConversionRecoFull.h"
#include "CbmAnaConversionRich.h"
#include "CbmAnaConversionTest.h"
#include "CbmAnaConversionTest2.h"
#include "CbmAnaConversionTomography.h"


#define M2E 2.6112004954086e-7

using namespace std;
using boost::assign::list_of;

CbmAnaConversion::CbmAnaConversion()
  : FairTask("CbmAnaConversion")
  , DoTomography(0)
  , DoRichAnalysis(0)
  , DoKFAnalysis(0)
  , DoReconstruction(0)
  , DoPhotons(0)
  , DoPhotons2(0)
  , DoRecoFull(0)
  , DoTest(0)
  , fhPdgCodes(NULL)
  , fhNofElPrim(NULL)
  , fhNofElSec(NULL)
  , fhNofElAll(NULL)
  , fhElectronSources(NULL)
  , fhNofPi0_perEvent(NULL)
  , fhNofPi0_perEvent_cut(NULL)
  , fhNofPi0_perEvent_cut2(NULL)
  , fhNofPi0_perEvent_cut3(NULL)
  , fhNofEta_perEvent(NULL)
  , fhNofEta_perEvent_cut(NULL)
  , fhNofEta_perEvent_cut2(NULL)
  , fhPi0_z(NULL)
  , fhPi0_z_cut(NULL)
  , fhPi0_pt(NULL)
  , fhPi0_pt_vs_rap(NULL)
  , fhPi0_theta(NULL)
  , fhPi0_theta_vs_rap(NULL)
  , fhEta_pt(NULL)
  , fhEta_pt_vs_rap(NULL)
  , fhEta_theta(NULL)
  , fhEta_theta_vs_rap(NULL)
  , fhRho_pt(NULL)
  , fhRho_pt_vs_rap(NULL)
  , fhRho_theta(NULL)
  , fhRho_theta_vs_rap(NULL)
  , fhOmega_pt(NULL)
  , fhOmega_pt_vs_rap(NULL)
  , fhOmega_theta(NULL)
  , fhOmega_theta_vs_rap(NULL)
  , fhElectronsFromPi0_z(NULL)
  , fhNofTracks_mctrack(NULL)
  , fhNofTracks_globaltrack(NULL)
  , fhInvariantMass_test(NULL)
  , fhInvariantMass_test2(NULL)
  , fhInvariantMass_test3(NULL)
  , fhInvariantMassReco_test(NULL)
  , fhInvariantMassReco_test2(NULL)
  , fhInvariantMassReco_test3(NULL)
  , fhInvariantMassReco_pi0(NULL)
  , fhMomentum_MCvsReco(NULL)
  , fhMomentum_MCvsReco_diff(NULL)
  , fhSearchGammas(NULL)
  , fPrimVertex(NULL)
  , fKFVertex()
  , fRichPoints(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fMcTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fGlobalTracks(NULL)
  , fhANN_output_electrons(NULL)
  , fhANN_output_electrons2(NULL)
  , fhANN_output_electrons_chiCut(NULL)
  , fhANN_output_electrons_vs_p(NULL)
  , fhANN_output_else(NULL)
  , fhANN_output_else2(NULL)
  , fhANN_output_else_chiCut(NULL)
  , fhANN_output_else_vs_p(NULL)
  , fEventNum(0)
  , test(0)
  , testint(0)
  , fAnalyseMode(0)
  , fKFparticle(NULL)
  , fKFparticleFinderQA(NULL)
  , fKFtopo(NULL)
  , trackindexarray()
  , particlecounter(0)
  , particlecounter_2daughters(0)
  , particlecounter_3daughters(0)
  , particlecounter_4daughters(0)
  , particlecounter_all(0)
  , fNofGeneratedPi0_allEvents(0)
  , fNofPi0_kfparticle_allEvents(0)
  , fNofGeneratedPi0(0)
  , fNofPi0_kfparticle(0)
  , fhPi0Ratio(NULL)
  , fhPi0_mass(NULL)
  , fhPi0_NDaughters(NULL)
  , fHistoList()
  , fHistoList_MC()
  , fHistoList_tomography()
  , fHistoList_reco()
  , fHistoList_reco_mom()
  , fHistoList_kfparticle()
  , fHistoList_richrings()
  , fHistoList_furtherAnalyses()
  , fMCTracklist()
  , fMCTracklist_all()
  , fRecoTracklist()
  , fRecoTracklistEPEM()
  , fRecoTracklistEPEM_id()
  , fRecoTracklistEPEM_chi()
  , fRecoTracklistEPEM_gtid()
  , fTestTracklist()
  , fTestTracklist_momentum()
  , fTestTracklist_chi()
  , fTestTracklist_richInd()
  , fTestTracklist_ndf()
  , fTestTracklist_nofhits()
  , fTestTracklist_noRichInd()
  , fTestTracklist_noRichInd_MCindex()
  , fTestTracklist_noRichInd_momentum()
  , fTestTracklist_noRichInd_chi()
  , fTestTracklist_noRichInd_richInd()
  , fTestTracklist_noRichInd_gTrackId()
  , fTestTracklist_noRichInd_ndf()
  , fTestTracklist_noRichInd_nofhits()
  , fRecoMomentum()
  , fRecoRefittedMomentum()
  , fhNofElectrons_4epem(NULL)
  , fhPi0_MC_occurence(NULL)
  , fhPi0_MC_occurence2(NULL)
  , fhPi0_Reco_occurence(NULL)
  , fhPi0_Reco_occurence2(NULL)
  , fhPi0_Reco_angles(NULL)
  , fhPi0_Reco_chi(NULL)
  , fhPi0_Reco_ndf(NULL)
  , fhPi0_Reco_ndf_vs_chi(NULL)
  , fhPi0_Reco_ndf_vs_startvertex(NULL)
  , fhPi0_Reco_startvertex_vs_chi(NULL)
  , fhPi0_Reco_startvertex_vs_nofhits(NULL)
  , fhPi0_noRichInd_diffPhi(NULL)
  , fhPi0_noRichInd_diffTheta(NULL)
  , fhPi0_Reco_invmass_cases(NULL)
  , fhPi0_Reco_noRichInd_invmass_cases(NULL)
  , fhPi0_Reco_invmass(NULL)
  , fhPi0_Reco_invmass_mc(NULL)
  , fhPi0_Reco_noRichInd_invmass(NULL)
  , fhPi0_Reco_noRichInd_invmass_mc(NULL)
  , fhPi0_Reco_noRichInd_ndf_vs_nofhits(NULL)
  , fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi(NULL)
  , fhPi0_Reco_ndf_vs_nofhits(NULL)
  , fhPi0_Reco_ndf_vs_nofhits_withChi(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_momentum(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_momentum_eRest(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_pt(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target(NULL)
  , fhPi0_Reco_noRichInd_chi_vs_pt_eRest(NULL)
  , fhPi0_Reco_chi_vs_momentum(NULL)
  , fhPi0_Reco_chi_vs_momentum_eFromPi0(NULL)
  , fhPi0_Reco_chi_vs_momentum_eFromPi0_Target(NULL)
  , fhPi0_Reco_chi_vs_pt(NULL)
  , fhPi0_Reco_chi_vs_pt_eFromPi0(NULL)
  , fhPi0_Reco_chi_vs_pt_eFromPi0_Target(NULL)
  , timer_all()
  , fTime_all(0.)
  , timer_exec()
  , fTime_exec(0.)
  , timer_mc()
  , fTime_mc(0.)
  , timer_rec()
  , fTime_rec(0.)
  , fAnaTomography(NULL)
  , fAnaRich(NULL)
  , fAnaKF(NULL)
  , fAnaReco(NULL)
  , fAnaPhotons(NULL)
  , fAnaPhotons2(NULL)
  , fAnaRecoFull(NULL)
  , fAnaTest(NULL)
  , fAnaTest2(NULL) {}

CbmAnaConversion::~CbmAnaConversion() {}

InitStatus CbmAnaConversion::Init() {
  //timer_all.Reset();
  timer_all.Start();

  cout << "CbmAnaConversion::Init" << endl;
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

  InitHistograms();

  particlecounter = 0;

  testint = 0;
  test    = 0;

  fNofGeneratedPi0_allEvents   = 0;
  fNofPi0_kfparticle_allEvents = 0;


  DoTomography     = 1;
  DoRichAnalysis   = 1;
  DoKFAnalysis     = 0;
  DoReconstruction = 1;
  DoPhotons        = 1;
  DoPhotons2       = 1;
  DoRecoFull       = 1;
  DoTest           = 1;

  if (DoTomography) {
    fAnaTomography = new CbmAnaConversionTomography();
    fAnaTomography->Init();
  }
  if (DoRichAnalysis) {
    fAnaRich = new CbmAnaConversionRich();
    fAnaRich->Init();
  }
  if (DoKFAnalysis) {
    fAnaKF = new CbmAnaConversionKF();
    fAnaKF->SetKF(fKFparticle, fKFparticleFinderQA);
    fAnaKF->Init();
  }
  if (DoReconstruction) {
    fAnaReco = new CbmAnaConversionReco();
    fAnaReco->Init();
  }
  if (DoPhotons) {
    fAnaPhotons = new CbmAnaConversionPhotons();
    fAnaPhotons->Init();
  }
  if (DoPhotons2) {
    fAnaPhotons2 = new CbmAnaConversionPhotons2();
    fAnaPhotons2->Init();
  }
  if (DoRecoFull) {
    fAnaRecoFull = new CbmAnaConversionRecoFull();
    fAnaRecoFull->Init();
  }
  if (DoTest) {
    fAnaTest = new CbmAnaConversionTest();
    fAnaTest->Init();
    fAnaTest2 = new CbmAnaConversionTest2();
    fAnaTest2->Init();
  }

  timer_all.Stop();
  fTime_all += timer_all.RealTime();

  return kSUCCESS;
}

void CbmAnaConversion::InitHistograms() {
  fHistoList.clear();
  fHistoList_kfparticle.clear();
  fHistoList_furtherAnalyses.clear();


  fhPdgCodes = new TH1D("fhPdfCodes", "fhPdgCodes;pdg code;#", 1000, 0, 1000);
  fhNofElPrim =
    new TH1D("fhNofElPrim", "fhNofElPrim;Nof prim El;Entries", 10., -0.5, 9.5);
  fhNofElSec =
    new TH1D("fhNofElSec", "fhNofElSec;Nof Sec El;Entries", 20., -0.5, 19.5);
  fhNofElAll =
    new TH1D("fhNofElAll", "fhNofElAll;Nof All El;Entries", 30., -0.5, 29.5);
  fhNofPi0_perEvent = new TH1D("fhNofPi0_perEvent",
                               "fhNofPi0_perEvent;Nof pi0;Entries",
                               1000.,
                               -0.5,
                               999.5);
  fhNofPi0_perEvent_cut =
    new TH1D("fhNofPi0_perEvent_cut",
             "fhNofPi0_perEvent_cut (Z<10cm);Nof pi0;Entries",
             800.,
             -0.5,
             799.5);
  fhNofPi0_perEvent_cut2 =
    new TH1D("fhNofPi0_perEvent_cut2",
             "fhNofPi0_perEvent_cut2 (motherId = -1);Nof pi0;Entries",
             800.,
             -0.5,
             799.5);
  fhNofPi0_perEvent_cut3 =
    new TH1D("fhNofPi0_perEvent_cut3",
             "fhNofPi0_perEvent_cut3 (conversion before 70cm);Nof pi0;Entries",
             100.,
             -0.5,
             99.5);
  fhNofEta_perEvent = new TH1D(
    "fhNofEta_perEvent", "fhNofEta_perEvent;Nof eta;Entries", 100., -0.5, 99.5);
  fhNofEta_perEvent_cut =
    new TH1D("fhNofEta_perEvent_cut",
             "fhNofEta_perEvent_cut (Z<4cm);Nof eta;Entries",
             100.,
             -0.5,
             99.5);
  fhNofEta_perEvent_cut2 =
    new TH1D("fhNofEta_perEvent_cut2",
             "fhNofEta_perEvent_cut2 (motherId = -1);Nof eta;Entries",
             100.,
             -0.5,
             99.5);
  fhPi0_z = new TH1D("fhPi0_z", "fhPi0_z;z in cm;Entries", 600., -0.5, 599.5);
  fhPi0_z_cut =
    new TH1D("fhPi0_z_cut", "fhPi0_z_cut;z in cm;Entries", 600., -0.5, 599.5);
  fhPi0_pt =
    new TH1D("fhPi0_pt", "fhPi0_pt;p_{t} in GeV/c;Entries", 200., 0., 10.);
  fhPi0_pt_vs_rap    = new TH2D("fhPi0_pt_vs_rap",
                             "fhPi0_pt_vs_rap;p_{t} in GeV/c; rapidity y",
                             240,
                             -2.,
                             10.,
                             270,
                             -2.,
                             7.);
  fhPi0_theta        = new TH1D("fhPi0_theta",
                         "fhPi0_theta;emission angle #theta in deg;Entries",
                         180.,
                         0.,
                         180.);
  fhPi0_theta_vs_rap = new TH2D("fhPi0_theta_vs_rap",
                                "fhPi0_theta_vs_rap;theta in deg;rapidity y",
                                180.,
                                0.,
                                180.,
                                270,
                                -2.,
                                7.);
  fhEta_pt =
    new TH1D("fhEta_pt", "fhEta_pt;p_{t} in GeV;Entries", 200., 0., 10.);
  fhEta_pt_vs_rap = new TH2D("fhEta_pt_vs_rap",
                             "fhEta_pt_vs_rap;p_{t} in GeV; rapidity y",
                             240,
                             -2.,
                             10.,
                             270,
                             -2.,
                             7.);
  fhEta_theta     = new TH1D("fhEta_theta",
                         "fhEta_theta;emission angle #theta in deg;Entries",
                         180.,
                         0.,
                         180.);
  fhEta_theta_vs_rap =
    new TH2D("fhEta_theta_vs_rap",
             "fhEta_theta_vs_rap;emission angle #theta in deg;rapidity y",
             180.,
             0.,
             180.,
             270,
             -2.,
             7.);
  fhRho_pt =
    new TH1D("fhRho_pt", "fhRho_pt;p_{t} in GeV/c;Entries", 200., 0., 10.);
  fhRho_pt_vs_rap = new TH2D("fhRho_pt_vs_rap",
                             "fhRho_pt_vs_rap;p_{t} in GeV/c; rapidity y",
                             240,
                             -2.,
                             10.,
                             270,
                             -2.,
                             7.);
  fhRho_theta     = new TH1D("fhRho_theta",
                         "fhRho_theta;emission angle #theta in deg;Entries",
                         180.,
                         0.,
                         180.);
  fhRho_theta_vs_rap =
    new TH2D("fhRho_theta_vs_rap",
             "fhRho_theta_vs_rap;emission angle #theta in deg;rapidity y",
             180.,
             0.,
             180.,
             270,
             -2.,
             7.);
  fhOmega_pt =
    new TH1D("fhOmega_pt", "fhOmega_pt;p_{t} in GeV/c;Entries", 200., 0., 10.);
  fhOmega_pt_vs_rap = new TH2D("fhOmega_pt_vs_rap",
                               "fhOmega_pt_vs_rap;p_{t} in GeV; rapidity y",
                               240,
                               -2.,
                               10.,
                               270,
                               -2.,
                               7.);
  fhOmega_theta     = new TH1D("fhOmega_theta",
                           "fhOmega_theta;emission angle #theta in deg;Entries",
                           180.,
                           0.,
                           180.);
  fhOmega_theta_vs_rap =
    new TH2D("fhOmega_theta_vs_rap",
             "fhOmega_theta_vs_rap;emission angle #theta in deg;rapidity y",
             180.,
             0.,
             180.,
             270,
             -2.,
             7.);


  fhElectronSources = new TH1D(
    "fhElectronSources", "fhElectronSources;Source;Entries", 6., 0., 6.);
  fhElectronsFromPi0_z =
    new TH1D("fhElectronsFromPi0_z",
             "fhElectronsFromPi0_z (= pos. of gamma conversion);z [cm];Entries",
             600.,
             -0.5,
             599.5);
  fHistoList.push_back(fhPdgCodes);
  fHistoList.push_back(fhNofPi0_perEvent);
  fHistoList.push_back(fhNofPi0_perEvent_cut);
  fHistoList.push_back(fhNofPi0_perEvent_cut2);
  fHistoList.push_back(fhNofPi0_perEvent_cut3);
  fHistoList.push_back(fhNofEta_perEvent);
  fHistoList.push_back(fhNofEta_perEvent_cut);
  fHistoList.push_back(fhNofEta_perEvent_cut2);
  fHistoList.push_back(fhPi0_z);
  fHistoList.push_back(fhPi0_z_cut);
  fHistoList.push_back(fhPi0_pt);
  fHistoList.push_back(fhPi0_pt_vs_rap);
  fHistoList.push_back(fhPi0_theta);
  fHistoList.push_back(fhPi0_theta_vs_rap);
  fHistoList.push_back(fhEta_pt);
  fHistoList.push_back(fhEta_pt_vs_rap);
  fHistoList.push_back(fhEta_theta);
  fHistoList.push_back(fhEta_theta_vs_rap);
  fHistoList.push_back(fhRho_pt);
  fHistoList.push_back(fhRho_pt_vs_rap);
  fHistoList.push_back(fhRho_theta);
  fHistoList.push_back(fhRho_theta_vs_rap);
  fHistoList.push_back(fhOmega_pt);
  fHistoList.push_back(fhOmega_pt_vs_rap);
  fHistoList.push_back(fhOmega_theta);
  fHistoList.push_back(fhOmega_theta_vs_rap);
  fHistoList.push_back(fhElectronSources);
  fHistoList.push_back(fhElectronsFromPi0_z);

  fhElectronSources->GetXaxis()->SetBinLabel(1, "gamma");
  fhElectronSources->GetXaxis()->SetBinLabel(2, "pi0");
  fhElectronSources->GetXaxis()->SetBinLabel(3, "eta");
  fhElectronSources->GetXaxis()->SetBinLabel(4, "else");
  fhElectronSources->GetXaxis()->SetBinLabel(5, "gamma from pi0");
  fhElectronSources->GetXaxis()->SetBinLabel(6, "gamma from eta");


  fhNofTracks_mctrack = new TH1D(
    "fhNofTracks_mctrack", "fhNofTracks_mctrack;nof;#", 1000., 0., 1000.);
  fHistoList.push_back(fhNofTracks_mctrack);
  fhNofTracks_globaltrack = new TH1D("fhNofTracks_globaltrack",
                                     "fhNofTracks_globaltrack;nof;#",
                                     1000.,
                                     0.,
                                     1000.);
  fHistoList.push_back(fhNofTracks_globaltrack);


  // for UrQMD events (invariant mass from pi0 -> gamma + gamma
  fhInvariantMass_test =
    new TH1D("fhInvariant", "fhInvariant;mass in GeV/c^{2}];#", 2000, 0., 2.);
  fhInvariantMass_test2 =
    new TH1D("fhInvariant2", "fhInvariant2;mass in GeV/c^{2}];#", 2000, 0., 2.);
  fhInvariantMass_test3 =
    new TH1D("fhInvariant3", "fhInvariant3;mass in GeV/c^{2}];#", 2000, 0., 2.);
  fHistoList.push_back(fhInvariantMass_test);
  fHistoList.push_back(fhInvariantMass_test2);
  fHistoList.push_back(fhInvariantMass_test3);

  fhInvariantMassReco_test = new TH1D(
    "fhInvariantReco", "fhInvariantReco;mass [GeV/c^2];#", 2000, 0., 2.);
  fhInvariantMassReco_test2 = new TH1D(
    "fhInvariantReco2", "fhInvariantReco2;mass [GeV/c^2];#", 2000, 0., 2.);
  fhInvariantMassReco_test3 = new TH1D(
    "fhInvariantReco3", "fhInvariantReco3;mass [GeV/c^2];#", 2000, 0., 2.);
  fHistoList.push_back(fhInvariantMassReco_test);
  fHistoList.push_back(fhInvariantMassReco_test2);
  fHistoList.push_back(fhInvariantMassReco_test3);

  fhInvariantMassReco_pi0 = new TH1D("fhInvariantReco_pi0",
                                     "fhInvariantReco_pi0;mass [GeV/c^2];#",
                                     2000,
                                     0.,
                                     2.);
  fHistoList.push_back(fhInvariantMassReco_pi0);


  // for reconstructed tracks
  fhMomentum_MCvsReco      = new TH2D("fhMomentum_MCvsReco",
                                 "fhMomentum_MCvsReco;MC;Reco",
                                 400,
                                 0.,
                                 40.,
                                 400,
                                 0.,
                                 40.);
  fhMomentum_MCvsReco_diff = new TH1D("fhMomentum_MCvsReco_diff",
                                      "fhMomentum_MCvsReco_diff;(MC-Reco)/MC",
                                      500,
                                      -0.01,
                                      4.);
  fHistoList.push_back(fhMomentum_MCvsReco);
  fHistoList.push_back(fhMomentum_MCvsReco_diff);


  fhSearchGammas =
    new TH1D("fhSearchGammas", "fhSearchGammas;mass;#", 100, -0.005, 0.995);
  fHistoList.push_back(fhSearchGammas);


  fhANN_output_electrons = new TH1D(
    "fhANN_output_electrons", "fhANN_output_electrons;ann output", 400, -2, 2.);
  fhANN_output_electrons2 = new TH1D("fhANN_output_electrons2",
                                     "fhANN_output_electrons2;ann output",
                                     400,
                                     -2,
                                     2.);
  fhANN_output_electrons_chiCut =
    new TH1D("fhANN_output_electrons_chiCut",
             "fhANN_output_electrons_chiCut;ann output",
             400,
             -2,
             2.);
  fhANN_output_electrons_vs_p =
    new TH2D("fhANN_output_electrons_vs_p",
             "fhANN_output_electrons_vs_p;momentum in GeV/c; ann output",
             100,
             0.,
             10.,
             400,
             -2,
             2.);
  fhANN_output_else =
    new TH1D("fhANN_output_else", "fhANN_output_else;ann output", 400, -2, 2.);
  fhANN_output_else2 = new TH1D(
    "fhANN_output_else2", "fhANN_output_else2;ann output", 400, -2, 2.);
  fhANN_output_else_chiCut = new TH1D("fhANN_output_else_chiCut",
                                      "fhANN_output_else_chiCut;ann output",
                                      400,
                                      -2,
                                      2.);
  fhANN_output_else_vs_p =
    new TH2D("fhANN_output_else_vs_p",
             "fhANN_output_else_vs_p;momentum in GeV/c; ann output",
             100,
             0.,
             10.,
             400,
             -2,
             2.);
  fHistoList.push_back(fhANN_output_electrons);
  fHistoList.push_back(fhANN_output_electrons2);
  fHistoList.push_back(fhANN_output_electrons_chiCut);
  fHistoList.push_back(fhANN_output_electrons_vs_p);
  fHistoList.push_back(fhANN_output_else);
  fHistoList.push_back(fhANN_output_else2);
  fHistoList.push_back(fhANN_output_else_chiCut);
  fHistoList.push_back(fhANN_output_else_vs_p);


  // #############################################
  // Histograms related to KFParticle results
  fhPi0_NDaughters = new TH1D(
    "fhPi0_NDaughters", "fhPi0_NDaughters;number of daughers;#", 4, 0.5, 4.5);
  fhPi0Ratio = new TH1D("fhPi0Ratio", "fhPi0Ratio; ratio;#", 1000, 0., 0.02);
  fhPi0_mass = new TH1D("fhPi0_mass", "fhPi0_mass;mass;#", 500, 0., 0.5);
  fHistoList_kfparticle.push_back(fhPi0_NDaughters);
  fHistoList_kfparticle.push_back(fhPi0Ratio);
  fHistoList_kfparticle.push_back(fhPi0_mass);


  fhNofElectrons_4epem =
    new TH1D("fhNofElectrons_4epem",
             "fhNofElectrons_4epem;number of electrons per event;#",
             101,
             -0.5,
             100.5);
  fHistoList.push_back(fhNofElectrons_4epem);

  fhPi0_MC_occurence =
    new TH1D("fhPi0_MC_occurence", "fhPi0_MC_occurence;;#", 20, 0, 20);
  fHistoList_furtherAnalyses.push_back(fhPi0_MC_occurence);
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(1, "== -1: all pi0 from target");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(2, "all pi0 -> gg");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(3, "all g -> e+e-");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(4, "both conv before 70cm");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(5, "both conv in target");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(6, "...");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(7,
                                              "!= -1: all pi0 not from target");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(8, "all pi0 -> gg");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(9, "all g -> e+e-");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(10, "both conv before 70cm");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(11, "both conv in target");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(12, "...");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(13, "daughters == 0");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(14, "daughters == 1");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(15, "daughters == 2");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(16, "daughters == 3");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(17, "daughters == 4");
  fhPi0_MC_occurence->GetXaxis()->SetBinLabel(18, "daughters > 4");

  fhPi0_MC_occurence2 =
    new TH1D("fhPi0_MC_occurence2", "fhPi0_MC_occurence2;;#", 20, 0, 20);
  fHistoList_furtherAnalyses.push_back(fhPi0_MC_occurence2);
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(
    1, "!= -1: all pi0 not from target");
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(2, "all pi0 -> gg");
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(3, "all g -> e+e-");
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(4, "both conv before 70cm");
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(5, "both conv in target");
  fhPi0_MC_occurence2->GetXaxis()->SetBinLabel(6, "...");

  fhPi0_Reco_occurence =
    new TH1D("fhPi0_Reco_occurence", "fhPi0_Reco_occurence;;#", 16, 0, 16);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_occurence);
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(1, "4 e from pi0 (not same)");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(2, "test");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(3, "x 1 e from same pi0");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(4, "x 2 e from same pi0");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(5, "x 3 e from same pi0");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(6, "x 4 e from same pi0");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(7, "x >4 e from same pi0");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(8, "...");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(9, "==4, within cuts");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(10, "==4, chi-check");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(11, "==4, chi+cuts");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(12, "richInd: ==1");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(13, "richInd: ==2");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(14, "richInd: ==3");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(15, "richInd: ==4");
  fhPi0_Reco_occurence->GetXaxis()->SetBinLabel(16, "richInd: >4");

  fhPi0_Reco_occurence2 =
    new TH1D("fhPi0_Reco_occurence2", "fhPi0_Reco_occurence2;;#", 16, 0, 16);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_occurence2);
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(1, "4 e from pi0 (not same)");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(2, "test");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(3, "x 1 e from same pi0");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(4, "x 2 e from same pi0");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(5, "x 3 e from same pi0");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(6, "x 4 e from same pi0");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(7, "x >4 e from same pi0");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(8, "...");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(9, "==4, within cuts");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(10, "==4, chi-check");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(11, "==4, chi+cuts");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(12, "...");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(13, "...");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(14, "...");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(15, "...");
  fhPi0_Reco_occurence2->GetXaxis()->SetBinLabel(16, "...");


  fhPi0_Reco_angles =
    new TH1D("fhPi0_Reco_angles", "fhPi0_Reco_angles;angle;#", 500, 0, 50);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_angles);
  fhPi0_Reco_chi =
    new TH1D("fhPi0_Reco_chi", "fhPi0_Reco_chi;chi;#", 500, 0, 500);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi);
  fhPi0_Reco_ndf =
    new TH1D("fhPi0_Reco_ndf", "fhPi0_Reco_ndf;ndf;#", 500, 0, 50);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_ndf);
  fhPi0_Reco_ndf_vs_chi = new TH2D("fhPi0_Reco_ndf_vs_chi",
                                   "fhPi0_Reco_ndf_vs_chi;ndf;chi",
                                   51,
                                   -0.5,
                                   50.5,
                                   500,
                                   0,
                                   50);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_ndf_vs_chi);
  fhPi0_Reco_ndf_vs_startvertex =
    new TH2D("fhPi0_Reco_ndf_vs_startvertex",
             "fhPi0_Reco_ndf_vs_startvertex;ndf;startvertex",
             51,
             -0.5,
             50.5,
             101,
             -0.5,
             100.5);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_ndf_vs_startvertex);
  fhPi0_Reco_startvertex_vs_chi =
    new TH2D("fhPi0_Reco_startvertex_vs_chi",
             "fhPi0_Reco_startvertex_vs_chi;startvertex;chi",
             101,
             -0.5,
             100.5,
             100,
             0,
             100);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_startvertex_vs_chi);
  fhPi0_Reco_startvertex_vs_nofhits =
    new TH2D("fhPi0_Reco_startvertex_vs_nofhits",
             "fhPi0_Reco_startvertex_vs_nofhits;startvertex;nofhits",
             101,
             -0.5,
             100.5,
             21,
             -0.5,
             20.5);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_startvertex_vs_nofhits);
  fhPi0_noRichInd_diffPhi = new TH1D("fhPi0_noRichInd_diffPhi",
                                     "fhPi0_noRichInd_diffPhi;phi difference;#",
                                     150,
                                     0,
                                     150);
  fHistoList_furtherAnalyses.push_back(fhPi0_noRichInd_diffPhi);
  fhPi0_noRichInd_diffTheta =
    new TH1D("fhPi0_noRichInd_diffTheta",
             "fhPi0_noRichInd_diffTheta;theta difference;#",
             150,
             0,
             150);
  fHistoList_furtherAnalyses.push_back(fhPi0_noRichInd_diffTheta);

  fhPi0_Reco_invmass_cases =
    new TH2D("fhPi0_Reco_invmass_cases",
             "fhPi0_Reco_invmass_cases;cases;invmass [GeV]",
             6,
             0,
             6,
             300,
             0,
             3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_invmass_cases);
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(1, "..");
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(2, "no cuts");
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(3, "only chi");
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(4, "only cuts");
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(5, "both");
  fhPi0_Reco_invmass_cases->GetXaxis()->SetBinLabel(6, "mc-true");
  fhPi0_Reco_noRichInd_invmass_cases =
    new TH2D("fhPi0_Reco_noRichInd_invmass_cases",
             "fhPi0_Reco_noRichInd_invmass_cases;cases;invmass in GeV^{2}",
             11,
             0,
             11,
             300,
             0,
             3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_invmass_cases);
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(1, "..");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(2, "no cuts");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(3, "only chi");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(4, "only cuts");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(5, "both");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(6, "mc-true");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(
    7, "richInd-no cuts");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(8, "richInd-chi");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(9,
                                                              "richInd-cuts");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(10,
                                                              "richInd-both");
  fhPi0_Reco_noRichInd_invmass_cases->GetXaxis()->SetBinLabel(11,
                                                              "richInd-mctrue");

  fhPi0_Reco_invmass = new TH1D(
    "fhPi0_Reco_invmass", "fhPi0_Reco_invmass;invmass [GeV];#", 300, 0, 3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_invmass);
  fhPi0_Reco_invmass_mc = new TH1D("fhPi0_Reco_invmass_mc",
                                   "fhPi0_Reco_invmass_mc;invmass_mc [GeV];#",
                                   300,
                                   0,
                                   3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_invmass_mc);
  fhPi0_Reco_noRichInd_invmass =
    new TH1D("fhPi0_Reco_noRichInd_invmass",
             "fhPi0_Reco_noRichInd_invmass;invmass_noRichInd [GeV];#",
             300,
             0,
             3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_invmass);
  fhPi0_Reco_noRichInd_invmass_mc =
    new TH1D("fhPi0_Reco_noRichInd_invmass_mc",
             "fhPi0_Reco_noRichInd_invmass_mc;invmass_noRichInd_mc [GeV];#",
             300,
             0,
             3);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_invmass_mc);

  fhPi0_Reco_noRichInd_ndf_vs_nofhits =
    new TH2D("fhPi0_Reco_noRichInd_ndf_vs_nofhits",
             "fhPi0_Reco_noRichInd_ndf_vs_nofhits;ndf;nofhits",
             31,
             -0.5,
             30.5,
             21,
             -0.5,
             20.5);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_ndf_vs_nofhits);
  fhPi0_Reco_ndf_vs_nofhits = new TH2D("fhPi0_Reco_ndf_vs_nofhits",
                                       "fhPi0_Reco_ndf_vs_nofhits;ndf;nofhits",
                                       31,
                                       -0.5,
                                       30.5,
                                       21,
                                       -0.5,
                                       20.5);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_ndf_vs_nofhits);
  fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi =
    new TH2D("fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi",
             "fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi;ndf;nofhits",
             31,
             -0.5,
             30.5,
             21,
             -0.5,
             20.5);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi);
  fhPi0_Reco_ndf_vs_nofhits_withChi =
    new TH2D("fhPi0_Reco_ndf_vs_nofhits_withChi",
             "fhPi0_Reco_ndf_vs_nofhits_withChi;ndf;nofhits",
             31,
             -0.5,
             30.5,
             21,
             -0.5,
             20.5);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_ndf_vs_nofhits_withChi);


  // ################################################

  fhPi0_Reco_noRichInd_chi_vs_momentum =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_momentum",
             "fhPi0_Reco_noRichInd_chi_vs_momentum;momentum [GeV];#chi^{2} of "
             "momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_chi_vs_momentum);
  fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0 =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0",
             "fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0;momentum "
             "[GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0);
  fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target",
             "fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target;momentum "
             "[GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target);
  fhPi0_Reco_noRichInd_chi_vs_momentum_eRest =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_momentum_eRest",
             "fhPi0_Reco_noRichInd_chi_vs_momentum_eRest;momentum "
             "[GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_noRichInd_chi_vs_momentum_eRest);

  fhPi0_Reco_noRichInd_chi_vs_pt =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_pt",
             "fhPi0_Reco_noRichInd_chi_vs_pt;pt [GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_chi_vs_pt);
  fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0 = new TH2D(
    "fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0",
    "fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0;pt [GeV];#chi^{2} of momentum fit",
    200,
    0.,
    10.,
    1000,
    0.,
    100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0);
  fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target =
    new TH2D("fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target",
             "fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target;pt [GeV];#chi^{2} "
             "of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target);
  fhPi0_Reco_noRichInd_chi_vs_pt_eRest = new TH2D(
    "fhPi0_Reco_noRichInd_chi_vs_pt_eRest",
    "fhPi0_Reco_noRichInd_chi_vs_pt_eRest;pt [GeV];#chi^{2} of momentum fit",
    200,
    0.,
    10.,
    1000,
    0.,
    100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_noRichInd_chi_vs_pt_eRest);


  // ################################################

  fhPi0_Reco_chi_vs_momentum = new TH2D(
    "fhPi0_Reco_chi_vs_momentum",
    "fhPi0_Reco_chi_vs_momentum;momentum [GeV];#chi^{2} of momentum fit",
    200,
    0.,
    10.,
    1000,
    0.,
    100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi_vs_momentum);
  fhPi0_Reco_chi_vs_momentum_eFromPi0 =
    new TH2D("fhPi0_Reco_chi_vs_momentum_eFromPi0",
             "fhPi0_Reco_chi_vs_momentum_eFromPi0;momentum [GeV];#chi^{2} of "
             "momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi_vs_momentum_eFromPi0);
  fhPi0_Reco_chi_vs_momentum_eFromPi0_Target =
    new TH2D("fhPi0_Reco_chi_vs_momentum_eFromPi0_Target",
             "fhPi0_Reco_chi_vs_momentum_eFromPi0_Target;momentum "
             "[GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(
    fhPi0_Reco_chi_vs_momentum_eFromPi0_Target);

  fhPi0_Reco_chi_vs_pt =
    new TH2D("fhPi0_Reco_chi_vs_pt",
             "fhPi0_Reco_chi_vs_pt;pt [GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi_vs_pt);
  fhPi0_Reco_chi_vs_pt_eFromPi0 =
    new TH2D("fhPi0_Reco_chi_vs_pt_eFromPi0",
             "fhPi0_Reco_chi_vs_pt_eFromPi0;pt [GeV];#chi^{2} of momentum fit",
             200,
             0.,
             10.,
             1000,
             0.,
             100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi_vs_pt_eFromPi0);
  fhPi0_Reco_chi_vs_pt_eFromPi0_Target = new TH2D(
    "fhPi0_Reco_chi_vs_pt_eFromPi0_Target",
    "fhPi0_Reco_chi_vs_pt_eFromPi0_Target;pt [GeV];#chi^{2} of momentum fit",
    200,
    0.,
    10.,
    1000,
    0.,
    100.);
  fHistoList_furtherAnalyses.push_back(fhPi0_Reco_chi_vs_pt_eFromPi0_Target);
}


void CbmAnaConversion::Exec(Option_t*) {
  timer_exec.Start();
  timer_all.Start();


  cout
    << "======================================================================="
    << endl;
  cout << "========== CbmAnaConversion, event No. " << fEventNum << endl;
  cout
    << "======================================================================="
    << endl;

  fEventNum++;

  fNofGeneratedPi0   = 0;
  fNofPi0_kfparticle = 0;


  // arrays of tracks
  fMCTracklist.clear();
  fMCTracklist_all.clear();
  fRecoTracklist.clear();
  fRecoTracklistEPEM.clear();
  fRecoTracklistEPEM_id.clear();
  fRecoTracklistEPEM_chi.clear();
  fRecoTracklistEPEM_gtid.clear();
  fRecoMomentum.clear();
  fRecoRefittedMomentum.clear();

  fTestTracklist.clear();
  fTestTracklist_momentum.clear();
  fTestTracklist_chi.clear();
  fTestTracklist_richInd.clear();
  fTestTracklist_ndf.clear();
  fTestTracklist_nofhits.clear();

  fTestTracklist_noRichInd.clear();
  fTestTracklist_noRichInd_MCindex.clear();
  fTestTracklist_noRichInd_momentum.clear();
  fTestTracklist_noRichInd_chi.clear();
  fTestTracklist_noRichInd_richInd.clear();
  fTestTracklist_noRichInd_gTrackId.clear();
  fTestTracklist_noRichInd_ndf.clear();
  fTestTracklist_noRichInd_nofhits.clear();

  // several counters
  int countPrimEl = 0;
  int countSecEl  = 0;
  int countAllEl  = 0;
  //	int countGammaEl = 0;
  //	int countMothers = 0;
  int countPrimPart              = 0;
  int countPi0MC                 = 0;
  int countPi0MC_cut             = 0;
  int countPi0MC_fromPrimary     = 0;
  int countPi0MC_reconstructible = 0;
  int countEtaMC                 = 0;
  int countEtaMC_cut             = 0;
  int countEtaMC_fromPrimary     = 0;

  if (fPrimVertex != NULL) {
    fKFVertex = CbmKFVertex(*fPrimVertex);
  } else {
    Fatal("CbmAnaConversion::Exec", "No PrimaryVertex array!");
  }

  if (DoKFAnalysis) {
    //	fAnaKF->SetSignalIds(fKFparticleFinderQA->GetSignalIds());
    //	fAnaKF->SetGhostIds(fKFparticleFinderQA->GetGhostIds());
    fAnaKF->Exec();
  }

  if (DoRichAnalysis) { fAnaRich->AnalyseRICHdata(); }

  if (DoPhotons) { fAnaPhotons->Exec(); }

  if (DoPhotons2) { fAnaPhotons2->Exec(); }

  if (DoRecoFull) { fAnaRecoFull->Exec(); }

  if (DoTomography) {
    fAnaTomography->Exec();  // analyse gamma-conversions with MC data
  }

  if (DoTest) {
    fAnaTest->Exec();
    fAnaTest2->Exec();
  }

  // ========================================================================================
  // START - Analyse MC tracks
  timer_mc.Start();

  Int_t nofMcTracks = fMcTracks->GetEntriesFast();
  fhNofTracks_mctrack->Fill(nofMcTracks);
  for (int i = 0; i < nofMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (mctrack == NULL) continue;

    FillMCTracklists(mctrack, i);  // fill tracklists for further analyses

    fhPdgCodes->Fill(TMath::Abs(mctrack->GetPdgCode()));

    if (mctrack->GetMotherId() == -1) { countPrimPart++; }
    if (mctrack->GetMotherId() == -1
        && TMath::Abs(mctrack->GetPdgCode()) == 11) {
      countPrimEl++;
    }
    if (mctrack->GetMotherId() != -1
        && TMath::Abs(mctrack->GetPdgCode()) == 11) {
      countSecEl++;
    }
    if (TMath::Abs(mctrack->GetPdgCode()) == 11) { countAllEl++; }


    if (mctrack->GetPdgCode() == 111) {  // particle is pi0
      countPi0MC++;
      TVector3 v;
      mctrack->GetStartVertex(v);
      if (v.Z() <= 4) { countPi0MC_cut++; }
      fhPi0_z->Fill(v.Z());
      //Double_t r2 = v.Z()*v.Z() * tan(25./180*TMath::Pi()) * tan(25./180*TMath::Pi());
      //if( (v.X()*v.X() + v.Y()*v.Y()) <= r2) {
      //	fhPi0_z_cut->Fill(v.Z());
      //}


      TVector3 momentum;
      mctrack->GetMomentum(momentum);

      int motherId = mctrack->GetMotherId();
      if (motherId == -1) {
        countPi0MC_fromPrimary++;
        fhPi0_pt->Fill(mctrack->GetPt());
        fhPi0_pt_vs_rap->Fill(mctrack->GetPt(), mctrack->GetRapidity());
        fhPi0_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
        fhPi0_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(),
                                 mctrack->GetRapidity());
        fhPi0_z_cut->Fill(v.Z());
      }

      Bool_t reconstructible = AnalysePi0_MC(mctrack, i);
      if (reconstructible) countPi0MC_reconstructible++;
    }


    if (TMath::Abs(mctrack->GetPdgCode()) == 221) {  // particle is eta
      countEtaMC++;
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);

      if (v.Z() <= 4) { countEtaMC_cut++; }
      int motherId = mctrack->GetMotherId();
      if (motherId == -1) {
        countEtaMC_fromPrimary++;
        fhEta_pt->Fill(mctrack->GetPt());
        fhEta_pt_vs_rap->Fill(mctrack->GetPt(), mctrack->GetRapidity());
        fhEta_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
        fhEta_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(),
                                 mctrack->GetRapidity());
      }
    }


    if (TMath::Abs(mctrack->GetPdgCode()) == 113
        || TMath::Abs(mctrack->GetPdgCode())
             == 213) {  // particle is rho(770)^0 or rho^+/-
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);

      //if (v.Z() <= 1) {
      fhRho_pt->Fill(mctrack->GetPt());
      fhRho_pt_vs_rap->Fill(mctrack->GetPt(), mctrack->GetRapidity());
      fhRho_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
      fhRho_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(),
                               mctrack->GetRapidity());
      //}
    }


    if (TMath::Abs(mctrack->GetPdgCode()) == 223
        || TMath::Abs(mctrack->GetPdgCode())
             == 3334) {  // particle is omega(782)^0 or omega^+/-
      TVector3 v, momentum;
      mctrack->GetStartVertex(v);
      mctrack->GetMomentum(momentum);

      //if (v.Z() <= 1) {
      fhOmega_pt->Fill(mctrack->GetPt());
      fhOmega_pt_vs_rap->Fill(mctrack->GetPt(), mctrack->GetRapidity());
      fhOmega_theta->Fill(momentum.Theta() * 180. / TMath::Pi());
      fhOmega_theta_vs_rap->Fill(momentum.Theta() * 180. / TMath::Pi(),
                                 mctrack->GetRapidity());
      //}
    }

    if (TMath::Abs(mctrack->GetPdgCode()) == 11) {  // particle is electron
      AnalyseElectrons(mctrack);
    }
  }


  cout << "CbmAnaConversion::Exec - Number of pi0 in MC sample: " << countPi0MC
       << endl;
  cout << "CbmAnaConversion::Exec - Number of pi0 from primary: "
       << countPi0MC_fromPrimary << endl;
  fhNofPi0_perEvent->Fill(countPi0MC);
  fhNofPi0_perEvent_cut->Fill(countPi0MC_cut);
  fhNofPi0_perEvent_cut2->Fill(countPi0MC_fromPrimary);
  fhNofPi0_perEvent_cut3->Fill(countPi0MC_reconstructible);
  fhNofEta_perEvent->Fill(countEtaMC);
  fhNofEta_perEvent_cut->Fill(countEtaMC_cut);
  fhNofEta_perEvent_cut2->Fill(countEtaMC_fromPrimary);

  fNofGeneratedPi0 = countPi0MC_fromPrimary;
  fNofGeneratedPi0_allEvents += fNofGeneratedPi0;
  fNofPi0_kfparticle_allEvents += fNofPi0_kfparticle;
  fhPi0Ratio->Fill(1.0 * fNofPi0_kfparticle / fNofGeneratedPi0);

  InvariantMassTest();

  if (DoReconstruction) {
    fAnaReco->SetTracklistMC(fMCTracklist_all);
    //fAnaReco->InvariantMassMC_all();
  }

  ReconstructGamma();


  fhNofElPrim->Fill(countPrimEl);
  fhNofElSec->Fill(countSecEl);
  fhNofElAll->Fill(countAllEl);


  // END - Analyse MC tracks
  // ========================================================================================
  timer_mc.Stop();
  fTime_mc += timer_mc.RealTime();


  // ========================================================================================
  // START - Analyse reconstructed tracks
  timer_rec.Start();

  Int_t nofElectrons4epem = 0;

  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  fhNofTracks_globaltrack->Fill(ngTracks);
  for (Int_t iGTrack = 0; iGTrack < ngTracks; iGTrack++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGTrack);
    if (NULL == gTrack) continue;
    int stsInd  = gTrack->GetStsTrackIndex();
    int richInd = gTrack->GetRichRingIndex();
    //		int trdInd = gTrack->GetTrdTrackIndex();
    //		int tofInd = gTrack->GetTofHitIndex();

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
    float result_chi = chiPrim[0];


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
    float result_chi_electron = chiPrim_electron[0];
    float result_ndf_electron = stsTracks_electron[0].GetNDF();


    Double_t startvertexZ = vtxTrack_electron->GetZ();
    fhPi0_Reco_ndf->Fill(result_ndf_electron);
    fhPi0_Reco_chi->Fill(result_chi_electron);
    fhPi0_Reco_ndf_vs_chi->Fill(result_ndf_electron, result_chi_electron);
    fhPi0_Reco_ndf_vs_startvertex->Fill(result_ndf_electron, startvertexZ);
    fhPi0_Reco_startvertex_vs_chi->Fill(startvertexZ, result_chi_electron);

    Double_t nofhits_sts = stsTrack->GetNofHits();
    fhPi0_Reco_startvertex_vs_nofhits->Fill(startvertexZ, nofhits_sts);

    fhPi0_Reco_noRichInd_chi_vs_momentum->Fill(refittedMomentum_electron.Mag(),
                                               result_chi_electron);
    fhPi0_Reco_noRichInd_chi_vs_pt->Fill(refittedMomentum_electron.Perp(),
                                         result_chi_electron);

    fTestTracklist_noRichInd.push_back(mcTrack1);
    fTestTracklist_noRichInd_MCindex.push_back(stsMcTrackId);
    fTestTracklist_noRichInd_momentum.push_back(refittedMomentum_electron);
    fTestTracklist_noRichInd_chi.push_back(result_chi_electron);
    fTestTracklist_noRichInd_richInd.push_back(richInd);
    fTestTracklist_noRichInd_gTrackId.push_back(iGTrack);
    fTestTracklist_noRichInd_ndf.push_back(result_ndf_electron);
    fTestTracklist_noRichInd_nofhits.push_back(nofhits_sts);


    if (richInd < 0) continue;

    // ANN output for rings
    Int_t pdg         = mcTrack1->GetPdgCode();
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richInd));
    if (NULL != ring) {
      Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(
        iGTrack, refittedMomentum_electron.Mag());

      if (TMath::Abs(pdg) == 11) {
        fhANN_output_electrons->Fill(ann);
        if (result_chi_electron <= CbmAnaConversionCutSettings::CalcChiCut(
              refittedMomentum_electron.Perp())) {
          fhANN_output_electrons_chiCut->Fill(ann);
        }
      }
      if (TMath::Abs(pdg) != 11) {
        fhANN_output_else->Fill(ann);
        if (result_chi_electron <= CbmAnaConversionCutSettings::CalcChiCut(
              refittedMomentum_electron.Perp())) {
          fhANN_output_else_chiCut->Fill(ann);
        }
      }
    }


    TVector3 stsMomentumVec;  // momenta as measured by STS
    stsTrack->GetParamFirst()->Momentum(stsMomentumVec);
    Double_t stsMomentum = stsMomentumVec.Mag();

    TVector3 mcMomentumVec;  // momenta from true-MC data
    mcTrack1->GetMomentum(mcMomentumVec);
    Double_t mcMomentum = mcMomentumVec.Mag();
    fhMomentum_MCvsReco->Fill(mcMomentum, stsMomentum);
    fhMomentum_MCvsReco_diff->Fill(TMath::Abs(mcMomentum - stsMomentum)
                                   / mcMomentum);

    TVector3
      bothtogether;  // combination of measured (STS) momenta and MC momenta
    bothtogether.SetX(mcMomentumVec.X());
    bothtogether.SetY(stsMomentumVec.Y());
    bothtogether.SetZ(stsMomentumVec.Z());


    // Fill tracklists containing momenta from mc-true, measured in sts, refitted at primary
    Bool_t isFilled = FillRecoTracklistEPEM(mcTrack1,
                                            stsMomentumVec,
                                            refittedMomentum,
                                            stsMcTrackId,
                                            result_chi,
                                            iGTrack);
    if (isFilled) nofElectrons4epem++;


    CbmTrackMatchNew* richMatch =
      (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    if (richMatch == NULL) continue;
    int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId < 0) continue;
    CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
    if (mcTrack2 == NULL) continue;

    //if(stsMcTrackId != richMcTrackId) continue;


    if (stsMcTrackId == richMcTrackId) {
      //CbmRichRing* ring = static_cast<CbmRichRing*> (fRichRings->At(richInd));
      if (NULL != ring) {
        Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(
          iGTrack, refittedMomentum_electron.Mag());
        if (TMath::Abs(pdg) == 11) {
          fhANN_output_electrons2->Fill(ann);
          fhANN_output_electrons_vs_p->Fill(refittedMomentum_electron.Mag(),
                                            ann);
        }
        if (TMath::Abs(pdg) != 11) {
          fhANN_output_else2->Fill(ann);
          fhANN_output_else_vs_p->Fill(refittedMomentum_electron.Mag(), ann);
        }
      }
    }


    //int pdg = TMath::Abs(mcTrack1->GetPdgCode());
    //int motherId = mcTrack1->GetMotherId();
    //double momentum = mcTrack1->GetP();
    stsMatch->GetTrueOverAllHitsRatio();


    if (DoTomography) { fAnaTomography->TomographyReco(mcTrack1); }
    FillRecoTracklist(mcTrack1);


    fTestTracklist.push_back(mcTrack1);
    fTestTracklist_momentum.push_back(refittedMomentum_electron);
    fTestTracklist_chi.push_back(result_chi_electron);
    fTestTracklist_richInd.push_back(richInd);
    fTestTracklist_ndf.push_back(result_ndf_electron);
    fTestTracklist_nofhits.push_back(nofhits_sts);

    fhPi0_Reco_chi_vs_pt->Fill(refittedMomentum_electron.Perp(),
                               result_chi_electron);
    fhPi0_Reco_chi_vs_momentum->Fill(refittedMomentum_electron.Mag(),
                                     result_chi_electron);
  }

  AnalysePi0_Reco();
  AnalysePi0_Reco_noRichInd();


  fhNofElectrons_4epem->Fill(nofElectrons4epem);


  //	InvariantMassTestReco();

  if (DoReconstruction) {
    fAnaReco->SetTracklistReco(fRecoTracklistEPEM,
                               fRecoMomentum,
                               fRecoRefittedMomentum,
                               fRecoTracklistEPEM_id,
                               fRecoTracklistEPEM_chi,
                               fRecoTracklistEPEM_gtid);
    fAnaReco->InvariantMassTest_4epem();
    fAnaReco->CalculateInvMassWithFullRecoCuts();
  }

  // END - analyse reconstructed tracks
  // ========================================================================================
  timer_rec.Stop();
  fTime_rec += timer_rec.RealTime();


  // =========================================================================================================================
  // ============================================== END - EXEC function ======================================================
  // =========================================================================================================================
  timer_exec.Stop();
  fTime_exec += timer_exec.RealTime();
  timer_all.Stop();
  fTime_all += timer_all.RealTime();
}


void CbmAnaConversion::Finish() {
  timer_all.Start();

  cout << "\n\n############### CALLING FINISH ROUTINES... ############" << endl;


  // Write histograms to a file
  gDirectory->mkdir("analysis-conversion");
  gDirectory->cd("analysis-conversion");


  gDirectory->mkdir("KFParticle");
  gDirectory->cd("KFParticle");
  for (UInt_t i = 0; i < fHistoList_kfparticle.size(); i++) {
    fHistoList_kfparticle[i]->Write();
  }
  gDirectory->cd("..");


  if (DoTomography) { fAnaTomography->Finish(); }
  if (DoRichAnalysis) { fAnaRich->Finish(); }
  if (DoKFAnalysis) { fAnaKF->Finish(); }
  if (DoReconstruction) { fAnaReco->Finish(); }
  if (DoRecoFull) { fAnaRecoFull->Finish(); }
  if (DoPhotons) { fAnaPhotons->Finish(); }
  if (DoPhotons2) { fAnaPhotons2->Finish(); }
  if (DoTest) { fAnaTest->Finish(); }
  if (DoTest) { fAnaTest2->Finish(); }


  gDirectory->mkdir("further analyses");
  gDirectory->cd("further analyses");
  for (UInt_t i = 0; i < fHistoList_furtherAnalyses.size(); i++) {
    fHistoList_furtherAnalyses[i]->Write();
  }
  gDirectory->cd("..");

  for (UInt_t i = 0; i < fHistoList.size(); i++) {
    fHistoList[i]->Write();
  }
  gDirectory->cd("..");


  timer_all.Stop();
  fTime_all += timer_all.RealTime();

  cout << endl;
  cout << "############### FINISHED MAIN TASK ##############" << endl;
  cout << "Particlecounter: " << particlecounter << endl;
  cout << "Particlecounter (2 daughters): " << particlecounter_2daughters
       << endl;
  cout << "Particlecounter (3 daughters): " << particlecounter_3daughters
       << endl;
  cout << "Particlecounter (4 daughters): " << particlecounter_4daughters
       << endl;
  cout << "Particlecounter_all: " << particlecounter_all << endl;
  cout << "#####################################" << endl;
  cout << "Number of generated pi0 (all events): " << fNofGeneratedPi0_allEvents
       << endl;
  cout << "Number of reconstructed pi0 (all events): "
       << fNofPi0_kfparticle_allEvents << "\t - fraction: "
       << 1.0 * fNofPi0_kfparticle_allEvents / fNofGeneratedPi0_allEvents
       << endl;
  cout << "#####################################" << endl;
  cout << "############### OVERALL TIMERS ###############" << endl;
  cout << std::fixed;
  cout << std::setprecision(1);
  cout << "Complete time: " << fTime_all << endl;
  cout << "Exec time: " << fTime_exec << endl;
  cout << "MC time: " << fTime_mc << "\t RECO time: " << fTime_rec << endl;
  cout << "############### ############## ###############" << endl;
  cout << "Number of events in fhNofPi0_perEvent histogram: "
       << fhNofPi0_perEvent->GetEntries() << endl;
  cout << "############### ############## ###############" << endl;
  // =========================================================================================================================
  // ============================================== END - FINISH function ====================================================
  // =========================================================================================================================
}


void CbmAnaConversion::AnalyseElectrons(CbmMCTrack* mctrack) {
  int motherId = mctrack->GetMotherId();
  if (motherId == -1) return;
  CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(motherId);
  int mcMotherPdg    = -1;
  if (NULL != mother) mcMotherPdg = mother->GetPdgCode();
  //cout << mcMotherPdg << endl;

  if (mcMotherPdg == 22) {
    fhElectronSources->Fill(0);
    int grandmotherId = mother->GetMotherId();
    if (grandmotherId == -1) return;
    CbmMCTrack* grandmother = (CbmMCTrack*) fMcTracks->At(grandmotherId);
    int mcGrandmotherPdg    = -1;
    if (NULL != grandmother) mcGrandmotherPdg = grandmother->GetPdgCode();
    if (mcGrandmotherPdg == 111) fhElectronSources->Fill(4);
    if (mcGrandmotherPdg == 221) fhElectronSources->Fill(5);

    if (mcGrandmotherPdg == 111) {
      TVector3 v;
      mctrack->GetStartVertex(v);
      fhElectronsFromPi0_z->Fill(v.Z());
    }
  }
  if (mcMotherPdg == 111) fhElectronSources->Fill(1);
  if (mcMotherPdg == 221) fhElectronSources->Fill(2);
  if (mcMotherPdg != 22 && mcMotherPdg != 111 && mcMotherPdg != 221)
    fhElectronSources->Fill(3);

  if (mcMotherPdg == 22) {
    TVector3 v;
    mctrack->GetStartVertex(v);
    //	fhGammaZ->Fill(v.Z());
    //	countGammaEl++;
  }
}


CbmLmvmKinematicParams
CbmAnaConversion::CalculateKinematicParams(const CbmMCTrack* mctrackP,
                                           const CbmMCTrack* mctrackM) {
  CbmLmvmKinematicParams params;

  TVector3 momP;  //momentum e+
  mctrackP->GetMomentum(momP);
  Double_t energyP = TMath::Sqrt(momP.Mag2() + M2E);
  TLorentzVector lorVecP(momP, energyP);

  TVector3 momM;  //momentum e-
  mctrackM->GetMomentum(momM);
  Double_t energyM = TMath::Sqrt(momM.Mag2() + M2E);
  TLorentzVector lorVecM(momM, energyM);

  TVector3 momPair    = momP + momM;
  Double_t energyPair = energyP + energyM;
  Double_t ptPair     = momPair.Perp();
  Double_t pzPair     = momPair.Pz();
  Double_t yPair =
    0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
  Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
  Double_t theta     = 180. * anglePair / TMath::Pi();
  Double_t minv =
    2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(momM.Mag() * momP.Mag());

  params.fMomentumMag = momPair.Mag();
  params.fPt          = ptPair;
  params.fRapidity    = yPair;
  params.fMinv        = minv;
  params.fAngle       = theta;
  return params;
}

Double_t CbmAnaConversion::Invmass_2gammas(const CbmMCTrack* mctrack1,
                                           const CbmMCTrack* mctrack2)
// calculation of invariant mass from two gammas (m=0)
{
  TVector3 mom1;
  mctrack1->GetMomentum(mom1);
  Double_t energy1 = TMath::Sqrt(mom1.Mag2());
  TLorentzVector lorVec1(mom1, energy1);

  TVector3 mom2;
  mctrack2->GetMomentum(mom2);
  Double_t energy2 = TMath::Sqrt(mom2.Mag2());
  TLorentzVector lorVec2(mom2, energy2);

  TLorentzVector sum = lorVec1 + lorVec2;
  return sum.Mag();
}


Double_t CbmAnaConversion::Invmass_2particles(const CbmMCTrack* mctrack1,
                                              const CbmMCTrack* mctrack2)
// calculation of invariant mass from two electrons/positrons
{
  TVector3 mom1;
  mctrack1->GetMomentum(mom1);
  Double_t energy1 = TMath::Sqrt(mom1.Mag2() + M2E);
  TLorentzVector lorVec1(mom1, energy1);

  TVector3 mom2;
  mctrack2->GetMomentum(mom2);
  Double_t energy2 = TMath::Sqrt(mom2.Mag2() + M2E);
  TLorentzVector lorVec2(mom2, energy2);

  TLorentzVector sum = lorVec1 + lorVec2;
  return sum.Mag();
}


Double_t CbmAnaConversion::Invmass_4particles(const CbmMCTrack* mctrack1,
                                              const CbmMCTrack* mctrack2,
                                              const CbmMCTrack* mctrack3,
                                              const CbmMCTrack* mctrack4)
// calculation of invariant mass from four electrons/positrons
{
  /*    TVector3 mom1;
    mctrack1->GetMomentum(mom1);
    TVector3 tempmom1;
    tempmom1.SetX(SmearValue(mom1.X()));
    tempmom1.SetY(SmearValue(mom1.Y()));
    tempmom1.SetZ(SmearValue(mom1.Z()));
    Double_t energy1 = TMath::Sqrt(tempmom1.Mag2() + M2E);
    TLorentzVector lorVec1(tempmom1, energy1);

    TVector3 mom2;
    mctrack2->GetMomentum(mom2);
    TVector3 tempmom2;
    tempmom2.SetX(SmearValue(mom2.X()));
    tempmom2.SetY(SmearValue(mom2.Y()));
    tempmom2.SetZ(SmearValue(mom2.Z()));
    Double_t energy2 = TMath::Sqrt(tempmom2.Mag2() + M2E);
    TLorentzVector lorVec2(tempmom2, energy2);

    TVector3 mom3;
    mctrack3->GetMomentum(mom3);
    TVector3 tempmom3;
    tempmom3.SetX(SmearValue(mom3.X()));
    tempmom3.SetY(SmearValue(mom3.Y()));
    tempmom3.SetZ(SmearValue(mom3.Z()));
    Double_t energy3 = TMath::Sqrt(tempmom3.Mag2() + M2E);
    TLorentzVector lorVec3(tempmom3, energy3);

    TVector3 mom4;
    mctrack4->GetMomentum(mom4);
    TVector3 tempmom4;
    tempmom4.SetX(SmearValue(mom4.X()));
    tempmom4.SetY(SmearValue(mom4.Y()));
    tempmom4.SetZ(SmearValue(mom4.Z()));
    Double_t energy4 = TMath::Sqrt(tempmom4.Mag2() + M2E);
    TLorentzVector lorVec4(tempmom4, energy4);
*/
  TLorentzVector lorVec1;
  mctrack1->Get4Momentum(lorVec1);

  TLorentzVector lorVec2;
  mctrack2->Get4Momentum(lorVec2);

  TLorentzVector lorVec3;
  mctrack3->Get4Momentum(lorVec3);

  TLorentzVector lorVec4;
  mctrack4->Get4Momentum(lorVec4);


  TLorentzVector sum;
  sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;
  cout << "mc: \t" << sum.Px() << " / " << sum.Py() << " / " << sum.Pz()
       << " / " << sum.E() << "\t => mag = " << sum.Mag() << endl;
  return sum.Mag();
}


Double_t CbmAnaConversion::Invmass_4particlesRECO(const TVector3 part1,
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


// =========================================================================================================================
// ============================================== UP TO HERE: Invariant mass calculation ===================================
// =========================================================================================================================


void CbmAnaConversion::FillMCTracklists(CbmMCTrack* mctrack, int)
// fill all relevant tracklists containing MC tracks
{
  Bool_t electrons = true;
  Bool_t gammas    = true;

  // fill gamma tracklist
  if (TMath::Abs(mctrack->GetPdgCode()) == 22 && gammas) {
    int motherId = mctrack->GetMotherId();
    if (motherId != -1) {
      CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(motherId);
      int mcMotherPdg    = -1;
      if (NULL != mother) mcMotherPdg = mother->GetPdgCode();
      if (mcMotherPdg == 111) {  // pdg code 111 = pi0
        fMCTracklist.push_back(mctrack);
      }
    }
  }

  // fill electron tracklists
  if (TMath::Abs(mctrack->GetPdgCode()) == 11 && electrons) {
    TVector3 v;
    mctrack->GetStartVertex(v);
    if (v.Z() <= 70) {
      int motherId = mctrack->GetMotherId();
      if (motherId != -1 || motherId == -1) {
        CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(motherId);
        int mcMotherPdg    = -1;
        if (NULL != mother) mcMotherPdg = mother->GetPdgCode();
        if (mcMotherPdg == 22 || mcMotherPdg == 111 || mcMotherPdg == 221) {
          int grandmotherId = mother->GetMotherId();
          if (grandmotherId != -1 || grandmotherId == -1) {
            //	CbmMCTrack* grandmother = (CbmMCTrack*) fMcTracks->At(grandmotherId);
            //	int mcGrandmotherPdg = -1;
            //	if (NULL != grandmother) mcGrandmotherPdg = grandmother->GetPdgCode();
            //	if(mcGrandmotherPdg == 111) {
            fMCTracklist_all.push_back(mctrack);
            //	}
          }
        }
      }
    }
  }
}


void CbmAnaConversion::FillRecoTracklist(CbmMCTrack* mctrack) {
  if (TMath::Abs(mctrack->GetPdgCode()) == 11) {
    int motherId = mctrack->GetMotherId();
    if (motherId != -1) {
      CbmMCTrack* mother = (CbmMCTrack*) fMcTracks->At(motherId);
      int mcMotherPdg    = -1;
      if (NULL != mother) mcMotherPdg = mother->GetPdgCode();
      if (mcMotherPdg == 111
          || mcMotherPdg == 22) {  // pdg code 111 = pi0, 22 = gamma
        fRecoTracklist.push_back(mctrack);
        //	cout << "pdg " << mctrack->GetPdgCode() << "\t motherid " << motherId << endl;
        test++;
      }
    }
  }
}


Bool_t CbmAnaConversion::FillRecoTracklistEPEM(CbmMCTrack* mctrack,
                                               TVector3 stsMomentum,
                                               TVector3 refittedMom,
                                               int i,
                                               Double_t chi,
                                               Int_t GlobalTrackId) {
  Bool_t isFilled = false;
  if (TMath::Abs(mctrack->GetPdgCode()) == 11) {
    int motherId = mctrack->GetMotherId();
    if (motherId != -1) {
      fRecoTracklistEPEM.push_back(mctrack);
      fRecoTracklistEPEM_id.push_back(i);
      fRecoTracklistEPEM_chi.push_back(chi);
      fRecoTracklistEPEM_gtid.push_back(GlobalTrackId);
      fRecoMomentum.push_back(stsMomentum);
      fRecoRefittedMomentum.push_back(refittedMom);
      isFilled = true;
    }
  }
  return isFilled;
}


void CbmAnaConversion::CalculateInvMass_MC_2particles() {
  for (unsigned int i = 0; i < fMCTracklist_all.size(); i++) {
    for (unsigned int j = i + 1; j < fMCTracklist_all.size(); j++) {
      //			Double_t invmass = Invmass_2particles(fMCTracklist_all[i],fMCTracklist_all[j]);

      int motherId_i = fMCTracklist_all[i]->GetMotherId();
      int motherId_j = fMCTracklist_all[j]->GetMotherId();

      CbmMCTrack* mother_i = (CbmMCTrack*) fMcTracks->At(motherId_i);
      int mcMotherPdg_i    = -1;
      if (NULL != mother_i) mcMotherPdg_i = mother_i->GetPdgCode();

      //			CbmMCTrack* mother_j = (CbmMCTrack*) fMcTracks->At(motherId_j);
      //			int mcMotherPdg_j  = -1;
      //			if (NULL != mother_j) mcMotherPdg_j = mother_j->GetPdgCode();

      if (motherId_i == motherId_j
          && ((fMCTracklist_all[i]->GetPdgCode() == 11
               && fMCTracklist_all[j]->GetPdgCode() == -11)
              || (fMCTracklist_all[i]->GetPdgCode() == -11
                  && fMCTracklist_all[j]->GetPdgCode() == 11))) {
        //	fhInvariantMass_MC_all->Fill(invmass);
        //cout << "e+e- decay detected! MotherId " << motherId_i << "\t invariant mass: " << invmass << endl;

        if (mcMotherPdg_i == 111) {
          //fhInvariantMass_MC_pi0->Fill(invmass);
          cout << "-- mother particle of decay: pi0" << endl;
        }
      }
    }
  }
}


void CbmAnaConversion::InvariantMassTest()
// calculation of invariant mass via pi0-> gamma gamma, ONLY FROM MC DATA!
{
  for (unsigned int i = 0; i < fMCTracklist.size(); i++) {
    for (unsigned int j = i + 1; j < fMCTracklist.size(); j++) {

      //if(fMCTracklist[i]->GetPx() != fMCTracklist[j]->GetPx() && fMCTracklist[i]->GetPy() != fMCTracklist[j]->GetPy()) {
      Double_t invmass = Invmass_2gammas(fMCTracklist[i], fMCTracklist[j]);
      fhInvariantMass_test->Fill(invmass);
      TVector3 vi;
      fMCTracklist[i]->GetStartVertex(vi);
      TVector3 vj;
      fMCTracklist[j]->GetStartVertex(vj);

      int motherId_i = fMCTracklist[i]->GetMotherId();
      int motherId_j = fMCTracklist[j]->GetMotherId();

      if (motherId_i == motherId_j) {
        fhInvariantMass_test2->Fill(invmass);

        if (invmass < 0.001) {
          //	cout << "gamma1 " << fMCTracklist[i]->GetPx() << "\t" << fMCTracklist[i]->GetPy() << "\t" << fMCTracklist[i]->GetPz() << endl;
          //	cout << "gamma2 " << fMCTracklist[j]->GetPx() << "\t" << fMCTracklist[j]->GetPy() << "\t" << fMCTracklist[j]->GetPz() << endl;
        }
      }
      if (vi.Z() == vj.Z() && vi.Y() == vj.Y() && vi.X() == vj.X()) {
        fhInvariantMass_test3->Fill(invmass);
      }
      //}
    }
  }
}


void CbmAnaConversion::InvariantMassTestReco()
// calculation of invariant mass via pi0 -> .. -> e+ e- e+ e-, ONLY FROM RECONSTRUCTED TRACKS!
{
  //	cout << "InvariantMassTestReco - Start..." << endl;
  //	cout << "InvariantMassTestReco - Size of fRecoTracklist:\t " << fRecoTracklist.size() << endl;
  if (fRecoTracklist.size() >= 4) {
    for (unsigned int i = 0; i < fRecoTracklist.size(); i++) {
      for (unsigned int j = i + 1; j < fRecoTracklist.size(); j++) {
        for (unsigned int k = j + 1; k < fRecoTracklist.size(); k++) {
          for (unsigned int l = k + 1; l < fRecoTracklist.size(); l++) {

            if (fRecoTracklist[i]->GetPdgCode()
                  + fRecoTracklist[j]->GetPdgCode()
                  + fRecoTracklist[k]->GetPdgCode()
                  + fRecoTracklist[l]->GetPdgCode()
                != 0)
              continue;

            int motherId1      = fRecoTracklist[i]->GetMotherId();
            int motherId2      = fRecoTracklist[j]->GetMotherId();
            int motherId3      = fRecoTracklist[k]->GetMotherId();
            int motherId4      = fRecoTracklist[l]->GetMotherId();
            int grandmotherId1 = -1;
            int grandmotherId2 = -1;
            int grandmotherId3 = -1;
            int grandmotherId4 = -1;

            int mcMotherPdg1 = -1;
            int mcMotherPdg2 = -1;
            int mcMotherPdg3 = -1;
            /*
						int mcMotherPdg4  = -1;
						int mcGrandmotherPdg1  = -1;
						int mcGrandmotherPdg2  = -1;
						int mcGrandmotherPdg3  = -1;
						int mcGrandmotherPdg4  = -1;
*/

            if (motherId1 != -1) {
              CbmMCTrack* mother1 = (CbmMCTrack*) fMcTracks->At(motherId1);
              if (NULL != mother1) mcMotherPdg1 = mother1->GetPdgCode();
              grandmotherId1 = mother1->GetMotherId();
              if (grandmotherId1 != -1) {
                //								CbmMCTrack* grandmother1 = (CbmMCTrack*) fMcTracks->At(grandmotherId1);
                //								if (NULL != grandmother1) mcGrandmotherPdg1 = grandmother1->GetPdgCode();
              }
            }
            if (motherId2 != -1) {
              CbmMCTrack* mother2 = (CbmMCTrack*) fMcTracks->At(motherId2);
              if (NULL != mother2) mcMotherPdg2 = mother2->GetPdgCode();
              grandmotherId2 = mother2->GetMotherId();
              if (grandmotherId2 != -1) {
                //								CbmMCTrack* grandmother2 = (CbmMCTrack*) fMcTracks->At(grandmotherId2);
                //								if (NULL != grandmother2) mcGrandmotherPdg2 = grandmother2->GetPdgCode();
              }
            }
            if (motherId3 != -1) {
              CbmMCTrack* mother3 = (CbmMCTrack*) fMcTracks->At(motherId3);
              if (NULL != mother3) mcMotherPdg3 = mother3->GetPdgCode();
              grandmotherId3 = mother3->GetMotherId();
              if (grandmotherId3 != -1) {
                //								CbmMCTrack* grandmother3 = (CbmMCTrack*) fMcTracks->At(grandmotherId3);
                //								if (NULL != grandmother3) mcGrandmotherPdg3 = grandmother3->GetPdgCode();
              }
            }
            if (motherId4 != -1) {
              CbmMCTrack* mother4 = (CbmMCTrack*) fMcTracks->At(motherId4);
              //							if (NULL != mother4) mcMotherPdg4 = mother4->GetPdgCode();
              grandmotherId4 = mother4->GetMotherId();
              if (grandmotherId4 != -1) {
                //								CbmMCTrack* grandmother4 = (CbmMCTrack*) fMcTracks->At(grandmotherId4);
                //								if (NULL != grandmother4) mcGrandmotherPdg4 = grandmother4->GetPdgCode();
              }
            }


            if (motherId1 == motherId2 && motherId2 == motherId3
                && motherId3 == motherId4) {
              Double_t invmass = Invmass_4particles(fRecoTracklist[i],
                                                    fRecoTracklist[j],
                                                    fRecoTracklist[k],
                                                    fRecoTracklist[l]);
              fhInvariantMassReco_pi0->Fill(invmass);
              //		cout << "Decay pi0 -> e+ e- e+ e- detected!" << endl;
            }

            if (motherId1 == motherId2 && motherId1 == motherId3) {}
            if (motherId1 == motherId2 && motherId1 == motherId4) {}
            if (motherId1 == motherId3 && motherId1 == motherId4) {}
            if (motherId2 == motherId3 && motherId2 == motherId4) {}

            //	if(  ((motherId1 == motherId2 && motherId3 == motherId4) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg3 == 22 || mcMotherPdg3 == 111))
            //	  || ((motherId1 == motherId3 && motherId2 == motherId4) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg2 == 22 || mcMotherPdg2 == 111))
            //	  || ((motherId1 == motherId4 && motherId2 == motherId3) && (mcMotherPdg1 == 22 || mcMotherPdg1 == 111) && (mcMotherPdg2 == 22 || mcMotherPdg2 == 111))) {
            if (((motherId1 == motherId2 && motherId3 == motherId4)
                 && (mcMotherPdg1 == 22) && (mcMotherPdg3 == 22)
                 && grandmotherId1 == grandmotherId3)
                || ((motherId1 == motherId3 && motherId2 == motherId4)
                    && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 22)
                    && grandmotherId1 == grandmotherId2)
                || ((motherId1 == motherId4 && motherId2 == motherId3)
                    && (mcMotherPdg1 == 22) && (mcMotherPdg2 == 22)
                    && grandmotherId1 == grandmotherId2)) {
              Double_t invmass = Invmass_4particles(fRecoTracklist[i],
                                                    fRecoTracklist[j],
                                                    fRecoTracklist[k],
                                                    fRecoTracklist[l]);
              fhInvariantMassReco_pi0->Fill(invmass);
              //		cout << "Decay pi0 -> 2gamma -> e+ e-! \t MotherId " << motherId1 << "\t" << motherId2 << "\t" << motherId3 << "\t" << motherId4 <<
              //			 "\t GrandmotherId " << grandmotherId1 << "\t" << grandmotherId2 << "\t" << grandmotherId3 << "\t" << grandmotherId4 << endl;
            }
          }
        }
      }
    }
  }
  //	cout << "InvariantMassTestReco - End!" << endl;
}


int CbmAnaConversion::GetTest() { return testint; }


int CbmAnaConversion::GetNofEvents() { return fEventNum; }


void CbmAnaConversion::SetMode(int mode = 0)
// mode 1 = tomography
// mode 2 = urqmd
// mode 3 = pluto
{
  fAnalyseMode = mode;
}


void CbmAnaConversion::ReconstructGamma() {
  if (fMCTracklist_all.size() >= 2) {
    for (unsigned int i = 0; i < fMCTracklist_all.size(); i++) {
      for (unsigned int j = i + 1; j < fMCTracklist_all.size(); j++) {
        if (fMCTracklist_all[i]->GetPdgCode()
              + fMCTracklist_all[j]->GetPdgCode()
            != 0)
          continue;

        int motherId1 = fMCTracklist_all[i]->GetMotherId();
        int motherId2 = fMCTracklist_all[j]->GetMotherId();

        if (motherId1 == motherId2) {
          Double_t invmass =
            Invmass_2particles(fMCTracklist_all[i], fMCTracklist_all[j]);
          fhSearchGammas->Fill(invmass);
        }
      }
    }
  }
}


void CbmAnaConversion::SetKF(CbmKFParticleFinder* kfparticle,
                             CbmKFParticleFinderQA* kfparticleQA) {

  fKFparticle         = kfparticle;
  fKFparticleFinderQA = kfparticleQA;
  if (fKFparticle) {
    cout << "kf works" << endl;
  } else {
    cout << "kf works not" << endl;
  }
}


Bool_t CbmAnaConversion::AnalysePi0_MC(CbmMCTrack* mctrack, int trackid) {
  Bool_t reconstructible = false;
  if (mctrack->GetMotherId() == -1) {
    fhPi0_MC_occurence->Fill(0);

    Int_t daughters                   = 0;
    Int_t gammaDaughters              = 0;
    Int_t electronDaughters           = 0;
    Int_t electronDaughtersBeforeRICH = 0;
    Int_t electronDaughtersInTarget   = 0;
    vector<int> gammaDaughterIDs;
    gammaDaughterIDs.clear();
    vector<int> electronDaughterIDs;
    electronDaughterIDs.clear();

    Int_t nofMcTracks = fMcTracks->GetEntriesFast();
    for (int i = 0; i < nofMcTracks; i++) {
      CbmMCTrack* mctrack_switch = (CbmMCTrack*) fMcTracks->At(i);
      if (mctrack_switch == NULL) continue;
      Int_t motherID = mctrack_switch->GetMotherId();
      Int_t pdg      = mctrack_switch->GetPdgCode();
      if (motherID == trackid && pdg == 22) {
        gammaDaughters++;
        gammaDaughterIDs.push_back(i);
      }
      if (motherID == trackid) { daughters++; }
    }


    if (daughters == 0) fhPi0_MC_occurence->Fill(12);
    if (daughters == 1) fhPi0_MC_occurence->Fill(13);
    if (daughters == 2) fhPi0_MC_occurence->Fill(14);
    if (daughters == 3) fhPi0_MC_occurence->Fill(15);
    if (daughters == 4) fhPi0_MC_occurence->Fill(16);
    if (daughters > 4) fhPi0_MC_occurence->Fill(17);


    if (gammaDaughters == 2) {
      fhPi0_MC_occurence->Fill(1);

      for (int i = 0; i < nofMcTracks; i++) {
        CbmMCTrack* mctrack_switch = (CbmMCTrack*) fMcTracks->At(i);
        if (mctrack_switch == NULL) continue;
        Int_t motherID = mctrack_switch->GetMotherId();
        Int_t pdg      = mctrack_switch->GetPdgCode();
        if (TMath::Abs(pdg) == 11
            && (motherID == gammaDaughterIDs[0]
                || motherID == gammaDaughterIDs[1])) {
          electronDaughters++;
          electronDaughterIDs.push_back(i);

          TVector3 startvertex;
          mctrack_switch->GetStartVertex(startvertex);
          if (startvertex.Z() <= 70) { electronDaughtersBeforeRICH++; }
          if (startvertex.Z() <= 4) { electronDaughtersInTarget++; }
        }
      }

      if (electronDaughters == 4) { fhPi0_MC_occurence->Fill(2); }
      if (electronDaughtersBeforeRICH == 4) {
        fhPi0_MC_occurence->Fill(3);
        reconstructible = true;
      }
      if (electronDaughtersInTarget == 4) { fhPi0_MC_occurence->Fill(4); }
    }
  } else {
    fhPi0_MC_occurence2->Fill(0);
    Int_t daughters                   = 0;
    Int_t gammaDaughters              = 0;
    Int_t electronDaughters           = 0;
    Int_t electronDaughtersBeforeRICH = 0;
    Int_t electronDaughtersInTarget   = 0;
    vector<int> gammaDaughterIDs;
    gammaDaughterIDs.clear();
    vector<int> electronDaughterIDs;
    electronDaughterIDs.clear();

    Int_t nofMcTracks = fMcTracks->GetEntriesFast();
    for (int i = 0; i < nofMcTracks; i++) {
      CbmMCTrack* mctrack_switch = (CbmMCTrack*) fMcTracks->At(i);
      if (mctrack_switch == NULL) continue;
      Int_t motherID = mctrack_switch->GetMotherId();
      Int_t pdg      = mctrack_switch->GetPdgCode();
      if (motherID == trackid && pdg == 22) {
        gammaDaughters++;
        gammaDaughterIDs.push_back(i);
      }
      if (motherID == trackid) { daughters++; }
    }

    if (gammaDaughters == 2) {
      fhPi0_MC_occurence2->Fill(1);
      for (int i = 0; i < nofMcTracks; i++) {
        CbmMCTrack* mctrack_switch = (CbmMCTrack*) fMcTracks->At(i);
        if (mctrack_switch == NULL) continue;
        Int_t motherID = mctrack_switch->GetMotherId();
        Int_t pdg      = mctrack_switch->GetPdgCode();
        if (TMath::Abs(pdg) == 11
            && (motherID == gammaDaughterIDs[0]
                || motherID == gammaDaughterIDs[1])) {
          electronDaughters++;
          electronDaughterIDs.push_back(i);

          TVector3 startvertex;
          mctrack_switch->GetStartVertex(startvertex);
          if (startvertex.Z() <= 70) { electronDaughtersBeforeRICH++; }
          if (startvertex.Z() <= 4) { electronDaughtersInTarget++; }
        }
      }
      if (electronDaughters == 4) { fhPi0_MC_occurence2->Fill(2); }
      if (electronDaughtersBeforeRICH == 4) { fhPi0_MC_occurence2->Fill(3); }
      if (electronDaughtersInTarget == 4) { fhPi0_MC_occurence2->Fill(4); }
    }
  }

  return reconstructible;
}


void CbmAnaConversion::AnalysePi0_Reco() {
  Int_t electrons = 0;
  std::multimap<int, int>
    electronPi0ID_map;  // contains all IDs of pi0, from which an electron has been detected
  std::multimap<int, int> electronPi0ID_map_richInd;

  Int_t nof = fTestTracklist.size();
  for (int i = 0; i < nof; i++) {
    Int_t motherID = fTestTracklist[i]->GetMotherId();
    Int_t pdg      = fTestTracklist[i]->GetPdgCode();

    if (TMath::Abs(pdg) == 11 && motherID != -1) {
      CbmMCTrack* mctrack_mother = (CbmMCTrack*) fMcTracks->At(motherID);
      if (mctrack_mother == NULL) continue;
      Int_t grandmotherID = mctrack_mother->GetMotherId();
      Int_t motherPdg     = mctrack_mother->GetPdgCode();

      if (motherPdg == 22 && grandmotherID != -1) {
        CbmMCTrack* mctrack_grandmother =
          (CbmMCTrack*) fMcTracks->At(grandmotherID);
        if (mctrack_grandmother == NULL) continue;
        Int_t grandmotherPdg = mctrack_grandmother->GetPdgCode();

        if (grandmotherPdg == 111) {
          electrons++;
          electronPi0ID_map.insert(std::pair<int, int>(grandmotherID, i));
          if (!(fTestTracklist_richInd[i] < 0)) {
            electronPi0ID_map_richInd.insert(
              std::pair<int, int>(grandmotherID, i));
          }
          fhPi0_Reco_chi_vs_pt_eFromPi0->Fill(fTestTracklist_momentum[i].Perp(),
                                              fTestTracklist_chi[i]);
          fhPi0_Reco_chi_vs_momentum_eFromPi0->Fill(
            fTestTracklist_momentum[i].Mag(), fTestTracklist_chi[i]);

          TVector3 startvertex;
          fTestTracklist[i]->GetStartVertex(startvertex);
          if (startvertex.Z() < 1) {
            fhPi0_Reco_chi_vs_pt_eFromPi0_Target->Fill(
              fTestTracklist_momentum[i].Perp(), fTestTracklist_chi[i]);
            fhPi0_Reco_chi_vs_momentum_eFromPi0_Target->Fill(
              fTestTracklist_momentum[i].Mag(), fTestTracklist_chi[i]);
          }
        }
      }
    }
  }

  if (
    electrons
    >= 4) {  // at least 4 electrons from pi0 (NOT from the same one) have been detected
    fhPi0_Reco_occurence->Fill(0);
  }


  int samePi0counter = 0;
  int check          = 0;
  for (std::map<int, int>::iterator it = electronPi0ID_map.begin();
       it != electronPi0ID_map.end();
       ++it) {
    if (it == electronPi0ID_map.begin()) check = 1;
    if (it != electronPi0ID_map.begin()) {
      std::map<int, int>::iterator zwischen = it;
      zwischen--;
      int id     = it->first;
      int id_old = zwischen->first;
      if (id == id_old) {
        check++;
        if (check > 3) {
          fhPi0_Reco_occurence->Fill(1);
          samePi0counter++;
        }
      } else {
        if (check == 1) fhPi0_Reco_occurence->Fill(2);
        if (check == 2) fhPi0_Reco_occurence->Fill(3);
        if (check == 3) fhPi0_Reco_occurence->Fill(4);
        if (check == 4) fhPi0_Reco_occurence->Fill(5);
        if (check > 4) fhPi0_Reco_occurence->Fill(6);

        if (check == 4) {
          std::map<int, int>::iterator alt3 = zwischen;
          alt3--;
          std::map<int, int>::iterator alt4 = alt3;
          alt4--;
          std::map<int, int>::iterator alt5 = alt4;
          alt5--;
          cout << "CbmAnaConversion: AnalysePi0_Reco: " << alt5->first << "/"
               << zwischen->first << "/" << alt3->first << "/" << alt4->first
               << endl;
          cout << "CbmAnaConversion: AnalysePi0_Reco: " << alt5->second << "/"
               << zwischen->second << "/" << alt3->second << "/" << alt4->second
               << endl;
          Bool_t IsWithinCuts = AnalysePi0_Reco_calc(
            alt5->second, zwischen->second, alt3->second, alt4->second);

          Double_t chi_e1 = fTestTracklist_chi[alt5->second];
          Double_t chi_e2 = fTestTracklist_chi[zwischen->second];
          Double_t chi_e3 = fTestTracklist_chi[alt3->second];
          Double_t chi_e4 = fTestTracklist_chi[alt4->second];


          Double_t invmass =
            Invmass_4particlesRECO(fTestTracklist_momentum[alt5->second],
                                   fTestTracklist_momentum[zwischen->second],
                                   fTestTracklist_momentum[alt3->second],
                                   fTestTracklist_momentum[alt4->second]);
          fhPi0_Reco_invmass->Fill(invmass);
          fhPi0_Reco_invmass_cases->Fill(1, invmass);
          Double_t invmass_mc =
            Invmass_4particles(fTestTracklist[alt5->second],
                               fTestTracklist[zwischen->second],
                               fTestTracklist[alt3->second],
                               fTestTracklist[alt4->second]);
          fhPi0_Reco_invmass_mc->Fill(invmass_mc);
          fhPi0_Reco_invmass_cases->Fill(5, invmass_mc);


          std::map<int, int>::iterator temp = zwischen;
          for (int i = 0; i < 4; i++) {
            int ndf     = fTestTracklist_ndf[temp->second];
            int nofhits = fTestTracklist_nofhits[temp->second];
            fhPi0_Reco_ndf_vs_nofhits->Fill(ndf, nofhits);
            temp--;
          }


          if (chi_e1 <= 3 && chi_e2 <= 3 && chi_e3 <= 3 && chi_e4 <= 3) {
            fhPi0_Reco_occurence->Fill(9);
            fhPi0_Reco_invmass_cases->Fill(2, invmass);
            if (IsWithinCuts) {
              fhPi0_Reco_occurence->Fill(10);
              fhPi0_Reco_invmass_cases->Fill(4, invmass);
            }

            std::map<int, int>::iterator temp2 = zwischen;
            for (int i = 0; i < 4; i++) {
              int ndf     = fTestTracklist_ndf[temp2->second];
              int nofhits = fTestTracklist_nofhits[temp2->second];
              fhPi0_Reco_ndf_vs_nofhits_withChi->Fill(ndf, nofhits);
              temp2--;
            }
          }
          if (IsWithinCuts) fhPi0_Reco_invmass_cases->Fill(3, invmass);

          fhPi0_Reco_chi->Fill(chi_e1);
          fhPi0_Reco_chi->Fill(chi_e2);
          fhPi0_Reco_chi->Fill(chi_e3);
          fhPi0_Reco_chi->Fill(chi_e4);
        }

        check = 1;
      }
    }
  }

  if (samePi0counter >= 4) {
    //fhPi0_MC_occurence->Fill(13);
  }


  int samePi0counter2 = 0;
  int check2          = 0;
  cout << "CbmAnaConversion: RecoPi0: electronmapsize: "
       << electronPi0ID_map_richInd.size() << endl;
  for (std::map<int, int>::iterator it = electronPi0ID_map_richInd.begin();
       it != electronPi0ID_map_richInd.end();
       ++it) {
    if (it == electronPi0ID_map_richInd.begin()) check = 1;
    if (it != electronPi0ID_map_richInd.begin()) {
      std::map<int, int>::iterator zwischen = it;
      zwischen--;
      int id     = it->first;
      int id_old = zwischen->first;
      if (id == id_old) {
        check2++;
        if (check2 > 3) {
          //fhPi0_Reco_occurence->Fill(1);
          samePi0counter2++;
        }
      } else {
        if (check2 == 1) fhPi0_Reco_occurence->Fill(11);
        if (check2 == 2) fhPi0_Reco_occurence->Fill(12);
        if (check2 == 3) fhPi0_Reco_occurence->Fill(13);
        if (check2 == 4) fhPi0_Reco_occurence->Fill(14);
        if (check2 > 4) fhPi0_Reco_occurence->Fill(15);
        /*
				if(check == 4) {
					std::map<int,int>::iterator alt3 = zwischen;
					alt3--;
					std::map<int,int>::iterator alt4 = alt3;
					alt4--;
					std::map<int,int>::iterator alt5 = alt4;
					alt5--;
					cout << "CbmAnaConversion: AnalysePi0_Reco: " << alt5->first << "/" << zwischen->first << "/" << alt3->first << "/" << alt4->first << endl;
					cout << "CbmAnaConversion: AnalysePi0_Reco: " << alt5->second << "/" << zwischen->second << "/" << alt3->second << "/" << alt4->second << endl;
					AnalysePi0_Reco_calc(alt5->second, zwischen->second, alt3->second, alt4->second);
				
				}
				*/
        check2 = 1;
      }
    }
  }
}


Bool_t CbmAnaConversion::AnalysePi0_Reco_calc(int e1, int e2, int e3, int e4) {
  CbmMCTrack* mctrack_e1 = fTestTracklist[e1];
  CbmMCTrack* mctrack_e2 = fTestTracklist[e2];
  CbmMCTrack* mctrack_e3 = fTestTracklist[e3];
  CbmMCTrack* mctrack_e4 = fTestTracklist[e4];

  Int_t motherID_e1 = mctrack_e1->GetMotherId();
  Int_t motherID_e2 = mctrack_e2->GetMotherId();
  Int_t motherID_e3 = mctrack_e3->GetMotherId();
  Int_t motherID_e4 = mctrack_e4->GetMotherId();


  Double_t energy1 = TMath::Sqrt(fTestTracklist_momentum[e1].Mag2() + M2E);
  TLorentzVector lorVec1(fTestTracklist_momentum[e1], energy1);

  Double_t energy2 = TMath::Sqrt(fTestTracklist_momentum[e2].Mag2() + M2E);
  TLorentzVector lorVec2(fTestTracklist_momentum[e2], energy2);

  Double_t energy3 = TMath::Sqrt(fTestTracklist_momentum[e3].Mag2() + M2E);
  TLorentzVector lorVec3(fTestTracklist_momentum[e3], energy3);

  Double_t energy4 = TMath::Sqrt(fTestTracklist_momentum[e4].Mag2() + M2E);
  TLorentzVector lorVec4(fTestTracklist_momentum[e4], energy4);


  Bool_t IsWithinCuts      = false;
  Double_t OpeningAngleCut = 1.5;

  if (motherID_e1 == motherID_e2 && motherID_e3 == motherID_e4) {
    Double_t anglePair1 = lorVec1.Angle(lorVec2.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec3.Angle(lorVec4.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    fhPi0_Reco_angles->Fill(theta1);
    fhPi0_Reco_angles->Fill(theta2);
    cout << "CbmAnaConversion: AnalysePi0_Reco_calc: " << theta1 << "/"
         << theta2 << endl;
  }

  if (motherID_e1 == motherID_e3 && motherID_e2 == motherID_e4) {
    Double_t anglePair1 = lorVec1.Angle(lorVec3.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec2.Angle(lorVec4.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    fhPi0_Reco_angles->Fill(theta1);
    fhPi0_Reco_angles->Fill(theta2);
    cout << "CbmAnaConversion: AnalysePi0_Reco_calc: " << theta1 << "/"
         << theta2 << endl;
  }

  if (motherID_e1 == motherID_e4 && motherID_e2 == motherID_e3) {
    Double_t anglePair1 = lorVec1.Angle(lorVec4.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec2.Angle(lorVec3.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    fhPi0_Reco_angles->Fill(theta1);
    fhPi0_Reco_angles->Fill(theta2);
    cout << "CbmAnaConversion: AnalysePi0_Reco_calc: " << theta1 << "/"
         << theta2 << endl;
  }

  if (IsWithinCuts == true) { fhPi0_Reco_occurence->Fill(8); }

  return IsWithinCuts;
}


void CbmAnaConversion::AnalysePi0_Reco_noRichInd() {
  Int_t electrons = 0;
  std::multimap<int, int>
    electronPi0ID_map;  // contains all IDs of pi0, from which an electron has been detected
  std::multimap<int, int>
    electronPi0ID_map_richInd;  // contains all IDs of pi0, from which an electron has been detected

  Int_t nof = fTestTracklist_noRichInd.size();
  for (int i = 0; i < nof; i++) {
    Int_t motherID = fTestTracklist_noRichInd[i]->GetMotherId();
    Int_t pdg      = fTestTracklist_noRichInd[i]->GetPdgCode();

    if (TMath::Abs(pdg) == 11 && motherID != -1) {
      CbmMCTrack* mctrack_mother = (CbmMCTrack*) fMcTracks->At(motherID);
      if (mctrack_mother == NULL) continue;
      Int_t grandmotherID = mctrack_mother->GetMotherId();
      Int_t motherPdg     = mctrack_mother->GetPdgCode();

      if (motherPdg == 22 && grandmotherID != -1) {
        CbmMCTrack* mctrack_grandmother =
          (CbmMCTrack*) fMcTracks->At(grandmotherID);
        if (mctrack_grandmother == NULL) continue;
        Int_t grandmotherPdg = mctrack_grandmother->GetPdgCode();

        if (grandmotherPdg == 111) {
          electrons++;
          electronPi0ID_map.insert(std::pair<int, int>(grandmotherID, i));
          if (!(fTestTracklist_noRichInd_richInd[i] < 0)) {
            electronPi0ID_map_richInd.insert(
              std::pair<int, int>(grandmotherID, i));
          }
          fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0->Fill(
            fTestTracklist_noRichInd_momentum[i].Perp(),
            fTestTracklist_noRichInd_chi[i]);
          fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0->Fill(
            fTestTracklist_noRichInd_momentum[i].Mag(),
            fTestTracklist_noRichInd_chi[i]);

          TVector3 startvertex;
          fTestTracklist_noRichInd[i]->GetStartVertex(startvertex);
          if (startvertex.Z() < 1) {
            fhPi0_Reco_noRichInd_chi_vs_pt_eFromPi0_Target->Fill(
              fTestTracklist_noRichInd_momentum[i].Perp(),
              fTestTracklist_noRichInd_chi[i]);
            fhPi0_Reco_noRichInd_chi_vs_momentum_eFromPi0_Target->Fill(
              fTestTracklist_noRichInd_momentum[i].Mag(),
              fTestTracklist_noRichInd_chi[i]);
          }
        } else {
          fhPi0_Reco_noRichInd_chi_vs_pt_eRest->Fill(
            fTestTracklist_noRichInd_momentum[i].Perp(),
            fTestTracklist_noRichInd_chi[i]);
          fhPi0_Reco_noRichInd_chi_vs_momentum_eRest->Fill(
            fTestTracklist_noRichInd_momentum[i].Mag(),
            fTestTracklist_noRichInd_chi[i]);
        }
      } else {
        fhPi0_Reco_noRichInd_chi_vs_pt_eRest->Fill(
          fTestTracklist_noRichInd_momentum[i].Perp(),
          fTestTracklist_noRichInd_chi[i]);
        fhPi0_Reco_noRichInd_chi_vs_momentum_eRest->Fill(
          fTestTracklist_noRichInd_momentum[i].Mag(),
          fTestTracklist_noRichInd_chi[i]);
      }
    } else {
      if (TMath::Abs(pdg) != 11) continue;
      fhPi0_Reco_noRichInd_chi_vs_pt_eRest->Fill(
        fTestTracklist_noRichInd_momentum[i].Perp(),
        fTestTracklist_noRichInd_chi[i]);
      fhPi0_Reco_noRichInd_chi_vs_momentum_eRest->Fill(
        fTestTracklist_noRichInd_momentum[i].Mag(),
        fTestTracklist_noRichInd_chi[i]);
    }
  }

  if (
    electrons
    >= 4) {  // at least 4 electrons from pi0 (NOT from the same one) have been detected
    fhPi0_Reco_occurence2->Fill(0);
  }


  int samePi0counter    = 0;
  int check             = 0;
  int check_withRichInd = 0;
  for (std::map<int, int>::iterator it = electronPi0ID_map.begin();
       it != electronPi0ID_map.end();
       ++it) {
    if (it == electronPi0ID_map.begin()) check = 1;
    if (it != electronPi0ID_map.begin()) {
      std::map<int, int>::iterator zwischen = it;
      zwischen--;
      int id     = it->first;
      int id_old = zwischen->first;
      if (id == id_old) {
        check++;
        if (check > 3) {
          //fhPi0_Reco_occurence->Fill(1);
          samePi0counter++;
        }
        if (fTestTracklist_noRichInd_richInd[it->second] >= 0) {
          check_withRichInd++;
        }
      } else {
        if (check == 1) fhPi0_Reco_occurence2->Fill(2);
        if (check == 2) fhPi0_Reco_occurence2->Fill(3);
        if (check == 3) fhPi0_Reco_occurence2->Fill(4);
        if (check == 4) fhPi0_Reco_occurence2->Fill(5);
        if (check > 4) fhPi0_Reco_occurence2->Fill(6);

        if (check == 4) {
          std::map<int, int>::iterator alt3 = zwischen;
          alt3--;
          std::map<int, int>::iterator alt4 = alt3;
          alt4--;
          std::map<int, int>::iterator alt5 = alt4;
          alt5--;
          cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd: " << alt5->first
               << "/" << zwischen->first << "/" << alt3->first << "/"
               << alt4->first << endl;
          cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd: "
               << alt5->second << "/" << zwischen->second << "/" << alt3->second
               << "/" << alt4->second << endl;
          Bool_t IsWithinCuts = AnalysePi0_Reco_noRichInd_calc(
            alt5->second, zwischen->second, alt3->second, alt4->second);

          Double_t chi_e1 = fTestTracklist_noRichInd_chi[alt5->second];
          Double_t chi_e2 = fTestTracklist_noRichInd_chi[zwischen->second];
          Double_t chi_e3 = fTestTracklist_noRichInd_chi[alt3->second];
          Double_t chi_e4 = fTestTracklist_noRichInd_chi[alt4->second];

          Int_t richInd_e1 = fTestTracklist_noRichInd_richInd[alt5->second];
          Int_t richInd_e2 = fTestTracklist_noRichInd_richInd[zwischen->second];
          Int_t richInd_e3 = fTestTracklist_noRichInd_richInd[alt3->second];
          Int_t richInd_e4 = fTestTracklist_noRichInd_richInd[alt4->second];


          Double_t invmass = Invmass_4particlesRECO(
            fTestTracklist_noRichInd_momentum[alt5->second],
            fTestTracklist_noRichInd_momentum[zwischen->second],
            fTestTracklist_noRichInd_momentum[alt3->second],
            fTestTracklist_noRichInd_momentum[alt4->second]);
          fhPi0_Reco_noRichInd_invmass->Fill(invmass);
          fhPi0_Reco_noRichInd_invmass_cases->Fill(1, invmass);
          Double_t invmass_mc =
            Invmass_4particles(fTestTracklist_noRichInd[alt5->second],
                               fTestTracklist_noRichInd[zwischen->second],
                               fTestTracklist_noRichInd[alt3->second],
                               fTestTracklist_noRichInd[alt4->second]);
          fhPi0_Reco_noRichInd_invmass_mc->Fill(invmass_mc);
          fhPi0_Reco_noRichInd_invmass_cases->Fill(5, invmass_mc);


          std::map<int, int>::iterator temp = zwischen;
          for (int i = 0; i < 4; i++) {
            int ndf     = fTestTracklist_noRichInd_ndf[temp->second];
            int nofhits = fTestTracklist_noRichInd_nofhits[temp->second];
            fhPi0_Reco_noRichInd_ndf_vs_nofhits->Fill(ndf, nofhits);
            temp--;
          }

          if (chi_e1 <= 3 && chi_e2 <= 3 && chi_e3 <= 3 && chi_e4 <= 3) {
            fhPi0_Reco_occurence2->Fill(9);
            fhPi0_Reco_noRichInd_invmass_cases->Fill(2, invmass);
            if (IsWithinCuts) {
              fhPi0_Reco_occurence2->Fill(10);
              fhPi0_Reco_noRichInd_invmass_cases->Fill(4, invmass);
            }

            std::map<int, int>::iterator temp2 = zwischen;
            for (int i = 0; i < 4; i++) {
              int ndf     = fTestTracklist_noRichInd_ndf[temp2->second];
              int nofhits = fTestTracklist_noRichInd_nofhits[temp2->second];
              fhPi0_Reco_noRichInd_ndf_vs_nofhits_withChi->Fill(ndf, nofhits);
              temp2--;
            }
          }

          if (IsWithinCuts) {
            fhPi0_Reco_noRichInd_invmass_cases->Fill(3, invmass);
          }

          if (richInd_e1 >= 0 && richInd_e2 >= 0 && richInd_e3 >= 0
              && richInd_e4 >= 0) {
            fhPi0_Reco_noRichInd_invmass_cases->Fill(6, invmass);
            fhPi0_Reco_noRichInd_invmass_cases->Fill(10, invmass_mc);
            if (IsWithinCuts) {
              fhPi0_Reco_noRichInd_invmass_cases->Fill(8, invmass);
            }
            if (chi_e1 <= 3 && chi_e2 <= 3 && chi_e3 <= 3 && chi_e4 <= 3) {
              fhPi0_Reco_noRichInd_invmass_cases->Fill(7, invmass);
              if (IsWithinCuts) {
                fhPi0_Reco_noRichInd_invmass_cases->Fill(9, invmass);
              }
            }
          }
        }

        if (check > 4) {
          std::map<int, int>::iterator temp = zwischen;
          cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd, check>4: ";
          for (int i = 0; i < check; i++) {
            TVector3 momentum_mc;
            fTestTracklist_noRichInd[temp->second]->GetMomentum(momentum_mc);
            Double_t theta_mc = 180.0 * momentum_mc.Theta() / TMath::Pi();
            Double_t phi_mc   = 180.0 * momentum_mc.Phi() / TMath::Pi();
            TVector3 momentum_reco =
              fTestTracklist_noRichInd_momentum[temp->second];
            Double_t theta_reco = 180.0 * momentum_reco.Theta() / TMath::Pi();
            Double_t phi_reco   = 180.0 * momentum_reco.Phi() / TMath::Pi();
            cout << "(" << temp->first << "/" << temp->second << "/"
                 << fTestTracklist_noRichInd_MCindex[temp->second] << "/"
                 << momentum_reco.Mag() << "/" << theta_mc << "-" << phi_mc
                 << "/" << theta_reco << "-" << phi_reco << ") \t";
            temp--;
            fhPi0_noRichInd_diffPhi->Fill(TMath::Abs(phi_mc - phi_reco));
            fhPi0_noRichInd_diffTheta->Fill(TMath::Abs(theta_mc - theta_reco));
          }
          cout << endl;
        }


        if (check_withRichInd == 1) fhPi0_Reco_occurence2->Fill(11);
        if (check_withRichInd == 2) fhPi0_Reco_occurence2->Fill(12);
        if (check_withRichInd == 3) fhPi0_Reco_occurence2->Fill(13);
        if (check_withRichInd == 4) fhPi0_Reco_occurence2->Fill(14);
        if (check_withRichInd > 4) fhPi0_Reco_occurence2->Fill(15);


        // reset of check-parameters for next loop
        check = 1;
        if (fTestTracklist_noRichInd_richInd[it->second] >= 0) {
          check_withRichInd = 1;
        } else {
          check_withRichInd = 0;
        }
      }
    }
  }

  if (samePi0counter >= 4) {
    //fhPi0_MC_occurence->Fill(13);
  }
}


Bool_t CbmAnaConversion::AnalysePi0_Reco_noRichInd_calc(int e1,
                                                        int e2,
                                                        int e3,
                                                        int e4) {
  CbmMCTrack* mctrack_e1 = fTestTracklist_noRichInd[e1];
  CbmMCTrack* mctrack_e2 = fTestTracklist_noRichInd[e2];
  CbmMCTrack* mctrack_e3 = fTestTracklist_noRichInd[e3];
  CbmMCTrack* mctrack_e4 = fTestTracklist_noRichInd[e4];

  Int_t motherID_e1 = mctrack_e1->GetMotherId();
  Int_t motherID_e2 = mctrack_e2->GetMotherId();
  Int_t motherID_e3 = mctrack_e3->GetMotherId();
  Int_t motherID_e4 = mctrack_e4->GetMotherId();


  Double_t energy1 =
    TMath::Sqrt(fTestTracklist_noRichInd_momentum[e1].Mag2() + M2E);
  TLorentzVector lorVec1(fTestTracklist_noRichInd_momentum[e1], energy1);

  Double_t energy2 =
    TMath::Sqrt(fTestTracklist_noRichInd_momentum[e2].Mag2() + M2E);
  TLorentzVector lorVec2(fTestTracklist_noRichInd_momentum[e2], energy2);

  Double_t energy3 =
    TMath::Sqrt(fTestTracklist_noRichInd_momentum[e3].Mag2() + M2E);
  TLorentzVector lorVec3(fTestTracklist_noRichInd_momentum[e3], energy3);

  Double_t energy4 =
    TMath::Sqrt(fTestTracklist_noRichInd_momentum[e4].Mag2() + M2E);
  TLorentzVector lorVec4(fTestTracklist_noRichInd_momentum[e4], energy4);


  Bool_t IsWithinCuts      = false;
  Double_t OpeningAngleCut = 1.5;

  if (motherID_e1 == motherID_e2 && motherID_e3 == motherID_e4) {
    Double_t anglePair1 = lorVec1.Angle(lorVec2.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec3.Angle(lorVec4.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd_calc: " << theta1
         << "/" << theta2 << endl;
  }

  if (motherID_e1 == motherID_e3 && motherID_e2 == motherID_e4) {
    Double_t anglePair1 = lorVec1.Angle(lorVec3.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec2.Angle(lorVec4.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd_calc: " << theta1
         << "/" << theta2 << endl;
  }

  if (motherID_e1 == motherID_e4 && motherID_e2 == motherID_e3) {
    Double_t anglePair1 = lorVec1.Angle(lorVec4.Vect());
    Double_t theta1     = 180. * anglePair1 / TMath::Pi();

    Double_t anglePair2 = lorVec2.Angle(lorVec3.Vect());
    Double_t theta2     = 180. * anglePair2 / TMath::Pi();

    if (theta1 <= OpeningAngleCut && theta2 <= OpeningAngleCut)
      IsWithinCuts = true;
    cout << "CbmAnaConversion: AnalysePi0_Reco_noRichInd_calc: " << theta1
         << "/" << theta2 << endl;
  }

  if (IsWithinCuts == true) { fhPi0_Reco_occurence2->Fill(8); }

  return IsWithinCuts;
}


ClassImp(CbmAnaConversion)
