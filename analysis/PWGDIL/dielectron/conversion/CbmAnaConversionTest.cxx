/**
 * \file CbmAnaConversionTest.cxx
 *
 * \author Sascha Reinecke <reinecke@uni-wuppertal.de>
 * \date 2015
 **/

#include "CbmAnaConversionTest.h"

#include "CbmAnaConversionCutSettings.h"
#include "CbmAnaConversionGlobalFunctions.h"
#include "CbmAnaConversionKinematicParams.h"
#include "CbmGlobalTrack.h"
#include "CbmL1PFFitter.h"
#include "CbmLitGlobalElectronId.h"
#include "CbmMCTrack.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"
#include <Logger.h>

#include <algorithm>
#include <map>

#include "L1Field.h"


using namespace std;


CbmAnaConversionTest::CbmAnaConversionTest()
  : fRichPoints(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fMcTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fGlobalTracks(NULL)
  , fPrimVertex(NULL)
  , fKFVertex()
  , fHistoList_test()
  , fElectrons_gtid()
  , fElectrons_mcid()
  , fElectrons_richInd()
  , fElectrons_pi0mcid()
  , fElectrons_same()
  , fElectrons_nofPerPi0(NULL)
  , fElectrons_nofPerPi0_withRichInd(NULL)
  , fhElectronsTest_invmass(NULL)
  , fhTest_ParticlesPerEvent(NULL)
  , fhTest_RICHelectronsPerEvent(NULL)
  , fhTest_PhotonsPerEvent_RICHonly(NULL)
  , fhTest_PhotonsPerEvent_STSandRICH(NULL)
  , fhTest_ReconstructedPi0PerEvent(NULL)
  , fhTest_invmass(NULL)
  , fhTest_invmass_pCut(NULL)
  , fhTest_invmass_GGcut(NULL)
  , fhTest_invmass_RICHindex0(NULL)
  , fhTest_invmass_RICHindex1(NULL)
  , fhTest_invmass_RICHindex2(NULL)
  , fhTest_invmass_RICHindex3(NULL)
  , fhTest_invmass_RICHindex4(NULL)
  , fhTest_invmass_MCcutAll(NULL)
  , fhTest_peakCheck1(NULL)
  , fhTest_peakCheck2(NULL)
  , fhTest_peakCheck3(NULL)
  , fhTest_invmass_ANNcuts(NULL)
  , fhTest_phaseSpace_pi0(NULL)
  , fhTest_phaseSpace_eta(NULL)
  , fVector_AllMomenta()
  , fVector_gt()
  , fVector_momenta()
  , fVector_chi()
  , fVector_gtIndex()
  , fVector_richIndex()
  , fVector_mcIndex()
  , fVector_withRichSignal()
  , fVector_reconstructedPhotons_FromSTSandRICH()
  , fVector_electronRICH_gt()
  , fVector_electronRICH_gtIndex()
  , fVector_electronRICH_mcIndex()
  , fVector_electronRICH_momenta()
  , fVector_electronRICH_reconstructedPhotons()
  , fVector_reconstructedPhotons_STSonly()
  , globalEventNo(0)
  , fMixedTest_STSonly_photons()
  , fMixedTest_STSonly_eventno()
  , fMixedTest_STSonly_hasRichInd()
  , fhTest_eventMixing_STSonly_2p2(NULL)
  , fhTest_eventMixing_STSonly_3p1(NULL)
  , fhTest_eventMixing_STSonly_4p0(NULL)
  , fMixedTest_3p1_photons()
  , fMixedTest_3p1_eventno()
  , fMixedTest_3p1_combined()
  , fMixedTest_3p1_ann()
  , fhTest_eventMixing_3p1(NULL)
  , fhTest_eventMixing_3p1_pCut(NULL)
  , fhTest_eventMixing_3p1_GGcut(NULL)
  , fhTest_eventMixing_3p1_ANNcuts(NULL) {}

CbmAnaConversionTest::~CbmAnaConversionTest() {}


void CbmAnaConversionTest::Init() {
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


void CbmAnaConversionTest::InitHistos() {
  fHistoList_test.clear();

  Double_t invmassSpectra_nof   = 800;
  Double_t invmassSpectra_start = -0.00125;
  Double_t invmassSpectra_end   = 1.99875;


  fElectrons_nofPerPi0 = new TH1I(
    "fElectrons_nofPerPi0", "fElectrons_nofPerPi0; nof; #", 7, -0.5, 6.5);
  fHistoList_test.push_back(fElectrons_nofPerPi0);
  fElectrons_nofPerPi0_withRichInd =
    new TH1I("fElectrons_nofPerPi0_withRichInd",
             "fElectrons_nofPerPi0_withRichInd; nof; #",
             7,
             -0.5,
             6.5);
  fHistoList_test.push_back(fElectrons_nofPerPi0_withRichInd);

  fhElectronsTest_invmass = new TH1D(
    "fhElectronsTest_invmass",
    "fhElectronsTest_invmass; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhElectronsTest_invmass);


  fhTest_PhotonsPerEvent_RICHonly =
    new TH1I("fhTest_PhotonsPerEvent_RICHonly",
             "fhTest_PhotonsPerEvent_RICHonly; nof; #",
             501,
             -0.5,
             500.5);
  fHistoList_test.push_back(fhTest_PhotonsPerEvent_RICHonly);
  fhTest_PhotonsPerEvent_STSandRICH =
    new TH1I("fhTest_PhotonsPerEvent_STSandRICH",
             "fhTest_PhotonsPerEvent_STSandRICH; nof; #",
             501,
             -0.5,
             500.5);
  fHistoList_test.push_back(fhTest_PhotonsPerEvent_STSandRICH);
  fhTest_ParticlesPerEvent = new TH1I("fhTest_ParticlesPerEvent",
                                      "fhTest_ParticlesPerEvent; nof; #",
                                      1001,
                                      -0.5,
                                      1000.5);
  fHistoList_test.push_back(fhTest_ParticlesPerEvent);
  fhTest_ReconstructedPi0PerEvent =
    new TH1I("fhTest_ReconstructedPi0PerEvent",
             "fhTest_ReconstructedPi0PerEvent; nof; #",
             101,
             -0.5,
             100.5);
  fHistoList_test.push_back(fhTest_ReconstructedPi0PerEvent);
  fhTest_RICHelectronsPerEvent =
    new TH1I("fhTest_RICHelectronsPerEvent",
             "fhTest_RICHelectronsPerEvent; nof; #",
             501,
             -0.5,
             500.5);
  fHistoList_test.push_back(fhTest_RICHelectronsPerEvent);

  fhTest_invmass = new TH1D(
    "fhTest_invmass",
    "fhTest_invmass; invariant mass of 3 e^{#pm} + 1 particle in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass);
  fhTest_invmass_pCut = new TH1D("fhTest_invmass_pCut",
                                 "fhTest_invmass_pCut; invariant mass of 3 "
                                 "e^{#pm} + 1 particle in GeV/c^{2}; #",
                                 invmassSpectra_nof,
                                 invmassSpectra_start,
                                 invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_pCut);
  fhTest_invmass_GGcut = new TH1D("fhTest_invmass_GGcut",
                                  "fhTest_invmass_GGcut; invariant mass of 3 "
                                  "e^{#pm} + 1 particle in GeV/c^{2}; #",
                                  invmassSpectra_nof,
                                  invmassSpectra_start,
                                  invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_GGcut);


  fhTest_invmass_RICHindex0 = new TH1D(
    "fhTest_invmass_RICHindex0",
    "fhTest_invmass_RICHindex0; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_RICHindex0);
  fhTest_invmass_RICHindex1 = new TH1D(
    "fhTest_invmass_RICHindex1",
    "fhTest_invmass_RICHindex1; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_RICHindex1);
  fhTest_invmass_RICHindex2 = new TH1D(
    "fhTest_invmass_RICHindex2",
    "fhTest_invmass_RICHindex2; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_RICHindex2);
  fhTest_invmass_RICHindex3 = new TH1D(
    "fhTest_invmass_RICHindex3",
    "fhTest_invmass_RICHindex3; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_RICHindex3);
  fhTest_invmass_RICHindex4 = new TH1D(
    "fhTest_invmass_RICHindex4",
    "fhTest_invmass_RICHindex4; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_RICHindex4);

  fhTest_invmass_MCcutAll =
    new TH2D("fhTest_invmass_MCcutAll",
             "fhTest_invmass_MCcutAll; case; invariant mass",
             25,
             0,
             25,
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fhTest_peakCheck1 =
    new TH1D("fhTest_peakCheck1", "fhTest_peakCheck1; sum; #", 15, -0.5, 14.5);
  fhTest_peakCheck2 =
    new TH1D("fhTest_peakCheck2", "fhTest_peakCheck2; sum; #", 15, -0.5, 14.5);
  fhTest_peakCheck3 =
    new TH1D("fhTest_peakCheck3", "fhTest_peakCheck3; sum; #", 20, -5.5, 14.5);
  fHistoList_test.push_back(fhTest_invmass_MCcutAll);
  fHistoList_test.push_back(fhTest_peakCheck1);
  fHistoList_test.push_back(fhTest_peakCheck2);
  fHistoList_test.push_back(fhTest_peakCheck3);

  fhTest_invmass_ANNcuts = new TH2D("fhTest_invmass_ANNcuts",
                                    "fhTest_invmass_ANNcuts;ann;invariant mass "
                                    "of 3 e^{#pm} + 1 particle in GeV/c^{2}",
                                    10,
                                    0,
                                    10,
                                    invmassSpectra_nof,
                                    invmassSpectra_start,
                                    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_invmass_ANNcuts);

  fhTest_phaseSpace_pi0 =
    new TH2D("fhTest_phaseSpace_pi0",
             "fhTest_phaseSpace_pi0;p_{t} in GeV/c;rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fhTest_phaseSpace_eta =
    new TH2D("fhTest_phaseSpace_eta",
             "fhTest_phaseSpace_eta;p_{t} in GeV/c;rapidity y",
             240,
             -2.,
             10.,
             270,
             -2.,
             7.);
  fHistoList_test.push_back(fhTest_phaseSpace_pi0);
  fHistoList_test.push_back(fhTest_phaseSpace_eta);

  fhTest_eventMixing_STSonly_2p2 =
    new TH1D("fhTest_eventMixing_STSonly_2p2",
             "fhTest_eventMixing_STSonly_2p2; invariant mass of 4 e^{#pm} in "
             "GeV/c^{2}; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_STSonly_2p2);
  fhTest_eventMixing_STSonly_3p1 =
    new TH1D("fhTest_eventMixing_STSonly_3p1",
             "fhTest_eventMixing_STSonly_3p1; invariant mass of 4 e^{#pm} in "
             "GeV/c^{2}; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_STSonly_3p1);
  fhTest_eventMixing_STSonly_4p0 =
    new TH1D("fhTest_eventMixing_STSonly_4p0",
             "fhTest_eventMixing_STSonly_4p0; invariant mass of 4 e^{#pm} in "
             "GeV/c^{2}; #",
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_STSonly_4p0);

  fhTest_eventMixing_3p1 = new TH1D(
    "fhTest_eventMixing_3p1",
    "fhTest_eventMixing_3p1; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_3p1);
  fhTest_eventMixing_3p1_pCut = new TH1D(
    "fhTest_eventMixing_3p1_pCut",
    "fhTest_eventMixing_3p1_pCut; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_3p1_pCut);
  fhTest_eventMixing_3p1_GGcut = new TH1D(
    "fhTest_eventMixing_3p1_GGcut",
    "fhTest_eventMixing_3p1_GGcut; invariant mass of 4 e^{#pm} in GeV/c^{2}; #",
    invmassSpectra_nof,
    invmassSpectra_start,
    invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_3p1_GGcut);

  fhTest_eventMixing_3p1_ANNcuts =
    new TH2D("fhTest_eventMixing_3p1_ANNcuts",
             "fhTest_eventMixing_3p1_ANNcuts;ann;invariant mass of 4 e^{#pm} "
             "in GeV/c^{2}",
             10,
             0,
             10,
             invmassSpectra_nof,
             invmassSpectra_start,
             invmassSpectra_end);
  fHistoList_test.push_back(fhTest_eventMixing_3p1_ANNcuts);
}


void CbmAnaConversionTest::Finish() {
  //gDirectory->cd("analysis-conversion");
  gDirectory->mkdir("Test");
  gDirectory->cd("Test");
  for (UInt_t i = 0; i < fHistoList_test.size(); i++) {
    fHistoList_test[i]->Write();
  }
  gDirectory->cd("..");
}


void CbmAnaConversionTest::Exec() {
  cout << "CbmAnaConversionTest: Exec()" << endl;

  globalEventNo++;

  if (fPrimVertex != NULL) {
    fKFVertex = CbmKFVertex(*fPrimVertex);
  } else {
    Fatal("CbmAnaConversionTest::Exec", "No PrimaryVertex array!");
  }

  fElectrons_mcid.clear();
  fElectrons_gtid.clear();
  fElectrons_richInd.clear();
  fElectrons_pi0mcid.clear();
  fElectrons_same.clear();


  Int_t nGTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t i = 0; i < nGTracks; i++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
    if (NULL == gTrack) continue;
    int stsInd  = gTrack->GetStsTrackIndex();
    int richInd = gTrack->GetRichRingIndex();
    if (stsInd < 0) continue;
    if (richInd < 0) continue;

    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
    if (stsTrack == NULL) continue;

    CbmTrackMatchNew* stsMatch =
      (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == NULL) continue;
    int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (stsMcTrackId < 0) continue;
    CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
    if (mcTrack1 == NULL) continue;

    CbmTrackMatchNew* richMatch =
      (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    if (richMatch == NULL) continue;
    int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId < 0) continue;
    CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
    if (mcTrack2 == NULL) continue;

    int pdg      = TMath::Abs(mcTrack1->GetPdgCode());
    int motherId = mcTrack1->GetMotherId();

    if (motherId < 0) continue;
    CbmMCTrack* mcTrack1mother = (CbmMCTrack*) fMcTracks->At(motherId);
    if (mcTrack1mother == NULL) continue;
    int grandmotherId = mcTrack1mother->GetMotherId();
    if (grandmotherId < 0) continue;
    CbmMCTrack* mcTrack1grandmother =
      (CbmMCTrack*) fMcTracks->At(grandmotherId);
    if (mcTrack1grandmother == NULL) continue;

    int pdg_mother      = TMath::Abs(mcTrack1mother->GetPdgCode());
    int pdg_grandmother = TMath::Abs(mcTrack1grandmother->GetPdgCode());

    cout << "CbmAnaConversionTest: pdg particle/mother/grandmother: " << pdg
         << " / " << pdg_mother << " / " << pdg_grandmother << endl;

    Int_t sameRichSts = (stsMcTrackId == richMcTrackId);

    if (TMath::Abs(pdg) == 11
        && (pdg_mother == 111 || pdg_grandmother == 111)) {
      fElectrons_richInd.push_back(richInd);
      fElectrons_gtid.push_back(i);
      fElectrons_mcid.push_back(stsMcTrackId);
      if (pdg_mother == 111) fElectrons_pi0mcid.push_back(motherId);
      if (pdg_grandmother == 111) fElectrons_pi0mcid.push_back(grandmotherId);
      fElectrons_same.push_back(sameRichSts);
    }
  }

  GetNofRichElectrons();


  DoSTSonlyAnalysis();
}


void CbmAnaConversionTest::GetNofRichElectrons() {

  std::multimap<int, int> electronMap;
  for (unsigned int i = 0; i < fElectrons_pi0mcid.size(); i++) {
    electronMap.insert(std::pair<int, int>(fElectrons_pi0mcid[i], i));
  }

  int check   = 0;
  int nofRich = 0;
  for (std::map<int, int>::iterator it = electronMap.begin();
       it != electronMap.end();
       ++it) {
    if (it == electronMap.begin()) {
      check = 1;
      if (fElectrons_same[it->second] > 0) nofRich++;
    }
    if (it != electronMap.begin()) {
      std::map<int, int>::iterator zwischen = it;
      zwischen--;
      int id     = it->first;
      int id_old = zwischen->first;
      if (id == id_old) {
        check++;
        if (fElectrons_same[it->second] > 0) nofRich++;
        if (check > 3) {
          std::map<int, int>::iterator zwischen2 = zwischen--;
          std::map<int, int>::iterator zwischen3 = zwischen2--;
          Double_t invmass                       = CalcInvMass(
            it->second, zwischen->second, zwischen2->second, zwischen3->second);
          if (invmass > 0) fhElectronsTest_invmass->Fill(invmass);
          if (nofRich == 4) {}
        }
      } else {
        fElectrons_nofPerPi0->Fill(check);
        fElectrons_nofPerPi0_withRichInd->Fill(nofRich);
        check   = 1;
        nofRich = 0;
        if (fElectrons_same[it->second] > 0) nofRich++;
      }
    }
  }
}


Double_t
CbmAnaConversionTest::CalcInvMass(Int_t e1, Int_t e2, Int_t e3, Int_t e4) {

  CbmMCTrack* mcTrack_e1 = (CbmMCTrack*) fMcTracks->At(fElectrons_mcid[e1]);
  CbmMCTrack* mcTrack_e2 = (CbmMCTrack*) fMcTracks->At(fElectrons_mcid[e2]);
  CbmMCTrack* mcTrack_e3 = (CbmMCTrack*) fMcTracks->At(fElectrons_mcid[e3]);
  CbmMCTrack* mcTrack_e4 = (CbmMCTrack*) fMcTracks->At(fElectrons_mcid[e4]);

  if (mcTrack_e1->GetPdgCode() + mcTrack_e2->GetPdgCode()
      + mcTrack_e3->GetPdgCode() + mcTrack_e4->GetPdgCode())
    return -1;

  TLorentzVector lorentz1;
  mcTrack_e1->Get4Momentum(lorentz1);
  TLorentzVector lorentz2;
  mcTrack_e2->Get4Momentum(lorentz2);
  TLorentzVector lorentz3;
  mcTrack_e3->Get4Momentum(lorentz3);
  TLorentzVector lorentz4;
  mcTrack_e4->Get4Momentum(lorentz4);

  TLorentzVector sum;
  sum = lorentz1 + lorentz2 + lorentz3 + lorentz4;

  return sum.Mag();
}


void CbmAnaConversionTest::DoSTSonlyAnalysis() {
  fVector_AllMomenta.clear();

  fVector_gt.clear();
  fVector_momenta.clear();
  fVector_chi.clear();
  fVector_gtIndex.clear();
  fVector_richIndex.clear();
  fVector_mcIndex.clear();
  fVector_withRichSignal.clear();
  fVector_reconstructedPhotons_FromSTSandRICH.clear();

  fVector_electronRICH_momenta.clear();
  fVector_electronRICH_gt.clear();
  fVector_electronRICH_gtIndex.clear();
  fVector_electronRICH_mcIndex.clear();
  fVector_electronRICH_reconstructedPhotons.clear();

  fVector_reconstructedPhotons_STSonly.clear();


  if (globalEventNo % 200 == 0) {
    MixedEventTest_3p1();
    fMixedTest_3p1_photons.clear();
    fMixedTest_3p1_eventno.clear();
    fMixedTest_3p1_combined.clear();
    fMixedTest_3p1_ann.clear();
  }

  if (globalEventNo % 20 == 0) {
    MixedEventTest_STSonly();
    fMixedTest_STSonly_photons.clear();
    fMixedTest_STSonly_eventno.clear();
    fMixedTest_STSonly_hasRichInd.clear();
  }


  Int_t nofRICHelectrons = 0;

  Int_t ngTracks = fGlobalTracks->GetEntriesFast();
  fhTest_ParticlesPerEvent->Fill(ngTracks);
  for (Int_t i = 0; i < ngTracks; i++) {
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(i);
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
    float result_chi_electron = chiPrim_electron[0];
    //float result_ndf_electron = stsTracks_electron[0].GetNDF();
    //		Double_t stsHits = stsTrack->GetNofHits();


    fVector_AllMomenta.push_back(refittedMomentum_electron);

    Double_t chiCut = 0;
    chiCut =
      CbmAnaConversionCutSettings::CalcChiCut(refittedMomentum_electron.Perp());

    if (result_chi_electron > chiCut) continue;

    //if(stsHits < 10) continue;
    //if(stsHits > 9) {
    fVector_momenta.push_back(refittedMomentum_electron);
    fVector_chi.push_back(result_chi_electron);
    fVector_gtIndex.push_back(i);
    fVector_gt.push_back(gTrack);
    fVector_richIndex.push_back(richInd);
    fVector_mcIndex.push_back(stsMcTrackId);
    //}

    Bool_t WithRichSignal = false;

    /*		if (richInd < 0) continue;
		CbmTrackMatchNew* richMatch  = (CbmTrackMatchNew*)fRichRingMatches->At(richInd);
		if (richMatch == NULL) continue;
		int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
		if (richMcTrackId < 0) continue;
		CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
		if (mcTrack2 == NULL) continue;
		
		Bool_t electron_rich = CbmLitGlobalElectronId::GetInstance().IsRichElectron(i, refittedMomentum_electron.Mag());
		if(electron_rich) {
			fVector_electronRICH_momenta.push_back(refittedMomentum_electron);
			fVector_electronRICH_gt.push_back(gTrack);
			fVector_electronRICH_gtIndex.push_back(i);
			nofRICHelectrons++;
			WithRichSignal = true;
		}
*/
    if (richInd >= 0) {
      CbmTrackMatchNew* richMatch =
        (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
      if (richMatch != NULL) {
        int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
        if (richMcTrackId >= 0) {
          CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
          if (mcTrack2 != NULL) {


            Bool_t electron_rich =
              CbmLitGlobalElectronId::GetInstance().IsRichElectron(
                i, refittedMomentum_electron.Mag());
            if (electron_rich) {
              fVector_electronRICH_momenta.push_back(refittedMomentum_electron);
              fVector_electronRICH_gt.push_back(gTrack);
              fVector_electronRICH_gtIndex.push_back(i);
              fVector_electronRICH_mcIndex.push_back(stsMcTrackId);
              nofRICHelectrons++;
              WithRichSignal = true;
            }
          }
        }
      }
    }
    fVector_withRichSignal.push_back(WithRichSignal);
  }

  fhTest_RICHelectronsPerEvent->Fill(nofRICHelectrons);

  // combination of 3 identified electrons in RICH and 1 track from STS with no RICH signal
  CombineElectrons_FromRICH();
  CombineElectrons_FromSTSandRICH();
  CombinePhotons();

  // starting point: track with STS signal only
  CombineElectrons_STSonly();
  CombinePhotons_STSonly();
}


void CbmAnaConversionTest::CombineElectrons_FromSTSandRICH() {
  Int_t nof_sts  = fVector_momenta.size();
  Int_t nof_rich = fVector_electronRICH_momenta.size();
  cout
    << "CbmAnaConversionTest: CombineElectrons_FromSTSandRICH, nof sts/rich - "
    << nof_sts << " / " << nof_rich << endl;
  Int_t nofPhotons = 0;
  if (nof_sts + nof_rich >= 2) {
    for (int a = 0; a < nof_sts; a++) {
      for (int b = 0; b < nof_rich; b++) {
        Int_t check1 =
          (fVector_gt[a]->GetParamLast()->GetQp()
           > 0);  // positive or negative charge (qp = charge over momentum ratio)
        Int_t check2 =
          (fVector_electronRICH_gt[b]->GetParamLast()->GetQp() > 0);
        Int_t test = check1 + check2;
        if (test != 1) continue;  // need one electron and one positron
        if (fVector_gtIndex[a] == fVector_electronRICH_gtIndex[b]) continue;
        if (fVector_richIndex[a] >= 0)
          continue;  // 4th lepton should have no signal in RICH, only in STS

        //CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fVector_momenta[a], fVector_electronRICH_momenta[b]);
        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
            fVector_momenta[a], fVector_electronRICH_momenta[b]);

        // opening angle cut depending on pt of e+e- pair
        Double_t openingAngleCut = 1.8 - 0.6 * paramsTest.fPt;
        //Double_t openingAngleCut = 1.5 - 0.5 * paramsTest.fPt;

        Double_t invMassCut = 0.03;

        Int_t IsPhoton_openingAngle1 = (paramsTest.fAngle < openingAngleCut);
        Int_t IsPhoton_invMass1      = (paramsTest.fMinv < invMassCut);


        if (IsPhoton_openingAngle1 && IsPhoton_invMass1) {
          nofPhotons++;
          vector<int> pair;  // = {a, b};
          pair.push_back(a);
          pair.push_back(b);
          fVector_reconstructedPhotons_FromSTSandRICH.push_back(pair);
          //fhElectrons_invmass_cut->Fill(params1.fMinv);

          vector<TVector3> pairmomenta;
          pairmomenta.push_back(fVector_momenta[a]);
          pairmomenta.push_back(fVector_electronRICH_momenta[b]);

          vector<Double_t> ann;
          ann.push_back(
            ElectronANNvalue(fVector_gtIndex[a], fVector_momenta[a].Mag()));
          ann.push_back(
            ElectronANNvalue(fVector_electronRICH_gtIndex[b],
                             fVector_electronRICH_momenta[b].Mag()));

          fMixedTest_3p1_photons.push_back(pairmomenta);
          fMixedTest_3p1_eventno.push_back(globalEventNo);
          fMixedTest_3p1_combined.push_back(1);
          fMixedTest_3p1_ann.push_back(ann);
        }
      }
    }
  }
  fhTest_PhotonsPerEvent_STSandRICH->Fill(nofPhotons);
  cout << "CbmAnaConversionTest: CombineElectronsFromSTSandRICH: Crosscheck - "
          "nof reconstructed photons: "
       << nofPhotons << endl;
}


void CbmAnaConversionTest::CombineElectrons_FromRICH() {
  Int_t nof = fVector_electronRICH_momenta.size();
  cout << "CbmAnaConversionTest: CombineElectronsFromRICH, nof - " << nof
       << endl;
  Int_t nofPhotons = 0;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t check1 =
          (fVector_electronRICH_gt[a]->GetParamLast()->GetQp()
           > 0);  // positive or negative charge (qp = charge over momentum ratio)
        Int_t check2 =
          (fVector_electronRICH_gt[b]->GetParamLast()->GetQp() > 0);
        Int_t test = check1 + check2;
        if (test != 1) continue;  // need one electron and one positron
        if (fVector_electronRICH_gtIndex[a] == fVector_electronRICH_gtIndex[b])
          continue;

        //CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fVector_electronRICH_momenta[a], fVector_electronRICH_momenta[b]);
        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
            fVector_electronRICH_momenta[a], fVector_electronRICH_momenta[b]);

        // opening angle cut depending on pt of e+e- pair
        Double_t openingAngleCut = 1.8 - 0.6 * paramsTest.fPt;
        //Double_t openingAngleCut = 1.5 - 0.5 * paramsTest.fPt;

        Double_t invMassCut = 0.03;

        Int_t IsPhoton_openingAngle1 = (paramsTest.fAngle < openingAngleCut);
        Int_t IsPhoton_invMass1      = (paramsTest.fMinv < invMassCut);


        if (IsPhoton_openingAngle1 && IsPhoton_invMass1) {
          nofPhotons++;
          vector<int> pair;  // = {a, b};
          pair.push_back(a);
          pair.push_back(b);
          fVector_electronRICH_reconstructedPhotons.push_back(pair);
          //fhElectrons_invmass_cut->Fill(params1.fMinv);

          vector<TVector3> pairmomenta;
          pairmomenta.push_back(fVector_electronRICH_momenta[a]);
          pairmomenta.push_back(fVector_electronRICH_momenta[b]);

          vector<Double_t> ann;
          ann.push_back(
            ElectronANNvalue(fVector_electronRICH_gtIndex[a],
                             fVector_electronRICH_momenta[a].Mag()));
          ann.push_back(
            ElectronANNvalue(fVector_electronRICH_gtIndex[b],
                             fVector_electronRICH_momenta[b].Mag()));

          fMixedTest_3p1_photons.push_back(pairmomenta);
          fMixedTest_3p1_eventno.push_back(globalEventNo);
          fMixedTest_3p1_combined.push_back(0);
          fMixedTest_3p1_ann.push_back(ann);
        }
      }
    }
  }
  fhTest_PhotonsPerEvent_RICHonly->Fill(nofPhotons);
  cout << "CbmAnaConversionTest: CombineElectronsFromRICH: Crosscheck - nof "
          "reconstructed photons: "
       << nofPhotons << endl;
}


void CbmAnaConversionTest::CombinePhotons() {
  Int_t nof_STSandRICH = fVector_reconstructedPhotons_FromSTSandRICH.size();
  Int_t nof_RICH       = fVector_electronRICH_reconstructedPhotons.size();
  cout << "CbmAnaConversionTest: CombinePhotons, nof - " << nof_STSandRICH
       << "/" << nof_RICH << endl;
  Int_t nofPi0 = 0;
  if (nof_STSandRICH + nof_RICH >= 2) {
    for (int a = 0; a < nof_STSandRICH; a++) {
      for (int b = 0; b < nof_RICH; b++) {
        Int_t electron11 = fVector_reconstructedPhotons_FromSTSandRICH
          [a][0];  // track with STS signal only
        Int_t electron12 = fVector_reconstructedPhotons_FromSTSandRICH
          [a][1];  // track with STS + RICH signal
        Int_t electron21 = fVector_electronRICH_reconstructedPhotons
          [b][0];  // track with STS + RICH signal
        Int_t electron22 = fVector_electronRICH_reconstructedPhotons
          [b][1];  // track with STS + RICH signal

        Int_t gtIndex11 = fVector_gtIndex[electron11];
        Int_t gtIndex12 = fVector_electronRICH_gtIndex[electron12];
        Int_t gtIndex21 = fVector_electronRICH_gtIndex[electron21];
        Int_t gtIndex22 = fVector_electronRICH_gtIndex[electron22];

        if (gtIndex11 == gtIndex12 || gtIndex11 == gtIndex21
            || gtIndex11 == gtIndex22 || gtIndex12 == gtIndex21
            || gtIndex12 == gtIndex22 || gtIndex21 == gtIndex22) {
          //if(electron12 == electron21 || electron12 == electron22)  {
          cout << "CbmAnaConversionTest: Test_DoubleIndex." << endl;
          continue;
        }

        //Double_t invmass = Invmass_4particlesRECO(fVector_momenta[electron11], fVector_electronRICH_momenta[electron12], fVector_electronRICH_momenta[electron21], fVector_electronRICH_momenta[electron22]);

        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
            fVector_momenta[electron11],
            fVector_electronRICH_momenta[electron12],
            fVector_electronRICH_momenta[electron21],
            fVector_electronRICH_momenta[electron22]);

        Double_t invmass = paramsTest.fMinv;

        fhTest_invmass->Fill(invmass);

        if (fVector_momenta[electron11].Mag() < 0.6) {
          fhTest_invmass_pCut->Fill(invmass);
        }

        Double_t OpeningAngleGG =
          CbmAnaConversionGlobalFunctions::OpeningAngleBetweenGamma(
            fVector_momenta[electron11],
            fVector_electronRICH_momenta[electron12],
            fVector_electronRICH_momenta[electron21],
            fVector_electronRICH_momenta[electron22]);
        if (OpeningAngleGG > 15 && OpeningAngleGG < 45)
          fhTest_invmass_GGcut->Fill(invmass);

        //Double_t pt = Pt_4particlesRECO(momenta[electron11], momenta[electron12], momenta[electron21], momenta[electron22]);
        //Double_t rap = Rap_4particlesRECO(momenta[electron11], momenta[electron12], momenta[electron21], momenta[electron22]);

        //Double_t opening_angle = OpeningAngleBetweenPhotons(momenta, reconstructedPhotons[a], reconstructedPhotons[b]);


        //CbmLmvmKinematicParams params1 = CalculateKinematicParams_4particles(momenta[electron11], momenta[electron12], momenta[electron21], momenta[electron22]);


        Double_t ANNe11 =
          ElectronANNvalue(gtIndex11, fVector_momenta[electron11].Mag());
        Double_t ANNe12 = ElectronANNvalue(
          gtIndex12, fVector_electronRICH_momenta[electron12].Mag());
        Double_t ANNe21 = ElectronANNvalue(
          gtIndex21, fVector_electronRICH_momenta[electron21].Mag());
        Double_t ANNe22 = ElectronANNvalue(
          gtIndex22, fVector_electronRICH_momenta[electron22].Mag());

        cout << "CbmAnaConversionTest: CombinePhotons, anns: " << ANNe11 << "/"
             << ANNe12 << "/" << ANNe21 << "/" << ANNe22 << endl;

        // ann-check only for those tracks, which have a signal in the RICH, i.e. not the first one
        if (ANNe12 > -1 && ANNe21 > -1 && ANNe22 > -1)
          fhTest_invmass_ANNcuts->Fill(1, invmass);
        if (ANNe12 > -0.9 && ANNe21 > -0.9 && ANNe22 > -0.9)
          fhTest_invmass_ANNcuts->Fill(2, invmass);
        if (ANNe12 > -0.8 && ANNe21 > -0.8 && ANNe22 > -0.8)
          fhTest_invmass_ANNcuts->Fill(3, invmass);
        if (ANNe12 > -0.7 && ANNe21 > -0.7 && ANNe22 > -0.7)
          fhTest_invmass_ANNcuts->Fill(4, invmass);
        if (ANNe12 > -0.6 && ANNe21 > -0.6 && ANNe22 > -0.6)
          fhTest_invmass_ANNcuts->Fill(5, invmass);
        if (ANNe12 > -0.5 && ANNe21 > -0.5 && ANNe22 > -0.5)
          fhTest_invmass_ANNcuts->Fill(6, invmass);
        if (ANNe12 > 0.0 && ANNe21 > 0.0 && ANNe22 > 0.0)
          fhTest_invmass_ANNcuts->Fill(7, invmass);


        cout << "CbmAnaConversionTest: CombinePhotons, photon combined! "
             << invmass << "/" << gtIndex11 << "/" << gtIndex12 << "/"
             << gtIndex21 << "/" << gtIndex22 << endl;
        nofPi0++;


        // MC-TRUE crosschecks
        CbmMCTrack* mctrack11 = (CbmMCTrack*) fMcTracks->At(
          fVector_mcIndex[electron11]);  // mctracks of four leptons
        CbmMCTrack* mctrack12 =
          (CbmMCTrack*) fMcTracks->At(fVector_electronRICH_mcIndex[electron12]);
        CbmMCTrack* mctrack21 =
          (CbmMCTrack*) fMcTracks->At(fVector_electronRICH_mcIndex[electron21]);
        CbmMCTrack* mctrack22 =
          (CbmMCTrack*) fMcTracks->At(fVector_electronRICH_mcIndex[electron22]);

        //				Int_t pdg11 = mctrack11->GetPdgCode();	// pdg codes of four leptons
        //				Int_t pdg12 = mctrack12->GetPdgCode();
        //				Int_t pdg21 = mctrack21->GetPdgCode();
        //				Int_t pdg22 = mctrack22->GetPdgCode();

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
                                 //	Int_t motherpdg12 = -2;
        Int_t motherpdg21 = -2;
        //	Int_t motherpdg22 = -2;
        if (mothermctrack11 != NULL)
          motherpdg11 = mothermctrack11->GetPdgCode();
        //	if(mothermctrack12 != NULL) motherpdg12 = mothermctrack12->GetPdgCode();
        if (mothermctrack21 != NULL)
          motherpdg21 = mothermctrack21->GetPdgCode();
        //	if(mothermctrack22 != NULL) motherpdg22 = mothermctrack22->GetPdgCode();

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
          fhTest_invmass_MCcutAll->Fill(1, invmass);
          if (TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 22) {
            fhTest_invmass_MCcutAll->Fill(2, invmass);
          }
          if (TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 22
              && grandmotherId11 == grandmotherId21
              && grandmotherId11 > 0) {  // decay in to gg of pi0 and eta
            fhTest_invmass_MCcutAll->Fill(3, invmass);
            if (invmass < 0.3)
              fhTest_phaseSpace_pi0->Fill(paramsTest.fPt, paramsTest.fRapidity);
            if (invmass > 0.3)
              fhTest_phaseSpace_eta->Fill(paramsTest.fPt, paramsTest.fRapidity);
          }
          if (TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 22
              && grandmotherId11 != grandmotherId21) {
            fhTest_invmass_MCcutAll->Fill(4, invmass);
          }
          if ((TMath::Abs(motherpdg11) == 22 && TMath::Abs(motherpdg21) == 111)
              || (TMath::Abs(motherpdg11) == 111
                  && TMath::Abs(motherpdg21) == 22)) {
            fhTest_invmass_MCcutAll->Fill(5, invmass);
            if (grandmotherId11 == motherId21
                || motherId11 == grandmotherId21) {  // Dalitz decay of pi0
              fhTest_invmass_MCcutAll->Fill(12, invmass);
              fhTest_phaseSpace_pi0->Fill(paramsTest.fPt, paramsTest.fRapidity);
            }
          }
          if (TMath::Abs(motherpdg11) == 111
              && TMath::Abs(motherpdg21) == 111) {
            fhTest_invmass_MCcutAll->Fill(6, invmass);
          }
          if ((TMath::Abs(motherpdg11) != 22 && TMath::Abs(motherpdg11) != 111)
              || (TMath::Abs(motherpdg21) != 22
                  && TMath::Abs(motherpdg21) != 111)) {
            fhTest_invmass_MCcutAll->Fill(7, invmass);
          }
          if (TMath::Abs(motherpdg11) != 22 && TMath::Abs(motherpdg11) != 111
              && TMath::Abs(motherpdg21) != 22
              && TMath::Abs(motherpdg21) != 111) {
            fhTest_invmass_MCcutAll->Fill(8, invmass);
          }
          if (grandmotherId11 == grandmotherId21) {
            fhTest_invmass_MCcutAll->Fill(9, invmass);
          }
        }
        if ((motherId11 == motherId12 && motherId21 != motherId22)
            || (motherId11 != motherId12 && motherId21 == motherId22)) {
          fhTest_invmass_MCcutAll->Fill(10, invmass);
        }
        if (motherId11 != motherId12 && motherId21 != motherId22) {
          fhTest_invmass_MCcutAll->Fill(11, invmass);
          if (sameGrandmothersSum == 12)
            fhTest_invmass_MCcutAll->Fill(13, invmass);
          if (sameGrandmothersSum == 6)
            fhTest_invmass_MCcutAll->Fill(14, invmass);
          if (sameGrandmothersSum == 4) {
            fhTest_invmass_MCcutAll->Fill(15, invmass);
            if (grandmotherId11 < 0 || grandmotherId12 < 0
                || grandmotherId21 < 0 || grandmotherId22 < 0) {
              fhTest_invmass_MCcutAll->Fill(16, invmass);
            }
            if (grandmotherId11 == grandmotherId12) {
              fhTest_invmass_MCcutAll->Fill(17, invmass);
            }
            if (grandmotherId11 != grandmotherId12) {
              fhTest_invmass_MCcutAll->Fill(18, invmass);
            }
          }
          if (sameGrandmothersSum == 2)
            fhTest_invmass_MCcutAll->Fill(19, invmass);
          if (sameGrandmothersSum == 0)
            fhTest_invmass_MCcutAll->Fill(20, invmass);
          fhTest_peakCheck1->Fill(sameGrandmothersSum);
          fhTest_peakCheck2->Fill(sameMothersSum);
          if ((grandmotherId11 < 0 || grandmotherId12 < 0 || grandmotherId21 < 0
               || grandmotherId22 < 0)
              && sameGrandmothersSum == 12) {
            fhTest_peakCheck3->Fill(grandmotherId11);
            fhTest_peakCheck3->Fill(grandmotherId12);
            fhTest_peakCheck3->Fill(grandmotherId21);
            fhTest_peakCheck3->Fill(grandmotherId22);
          }
        }
      }
    }
  }
  fhTest_ReconstructedPi0PerEvent->Fill(nofPi0);
}


CbmLmvmKinematicParams
CbmAnaConversionTest::CalculateKinematicParamsReco(const TVector3 electron1,
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


Double_t CbmAnaConversionTest::Invmass_4particlesRECO(const TVector3 part1,
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


void CbmAnaConversionTest::CombineElectrons_STSonly() {
  Int_t nof = fVector_momenta.size();
  cout << "CbmAnaConversionTest: CombineElectrons_STSonly, nof - " << nof
       << endl;
  Int_t nofPhotons = 0;
  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t check1 =
          (fVector_gt[a]->GetParamLast()->GetQp()
           > 0);  // positive or negative charge (qp = charge over momentum ratio)
        Int_t check2 = (fVector_gt[b]->GetParamLast()->GetQp() > 0);
        Int_t test   = check1 + check2;
        if (test != 1) continue;  // need one electron and one positron
        if (fVector_gtIndex[a] == fVector_gtIndex[b]) continue;

        //CbmLmvmKinematicParams params1 = CalculateKinematicParamsReco(fVector_electronRICH_momenta[a], fVector_electronRICH_momenta[b]);
        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_2particles_Reco(
            fVector_momenta[a], fVector_momenta[b]);

        // opening angle cut depending on pt of e+e- pair
        Double_t openingAngleCut = 1.8 - 0.6 * paramsTest.fPt;
        Double_t invMassCut      = 0.03;

        Int_t IsPhoton_openingAngle1 = (paramsTest.fAngle < openingAngleCut);
        Int_t IsPhoton_invMass1      = (paramsTest.fMinv < invMassCut);


        if (IsPhoton_openingAngle1 && IsPhoton_invMass1) {
          nofPhotons++;
          vector<int> pair;  // = {a, b};
          pair.push_back(a);
          pair.push_back(b);
          fVector_reconstructedPhotons_STSonly.push_back(pair);
          //fhElectrons_invmass_cut->Fill(params1.fMinv);

          // event mixing arrays
          vector<TVector3> pairmomenta;
          pairmomenta.push_back(fVector_momenta[a]);
          pairmomenta.push_back(fVector_momenta[b]);
          fMixedTest_STSonly_photons.push_back(pairmomenta);
          fMixedTest_STSonly_eventno.push_back(globalEventNo);
          vector<bool> pair_hasRichInd;
          //pair_hasRichInd.push_back(HasRichInd(fVector_gtIndex[a], a) );
          //pair_hasRichInd.push_back(HasRichInd(fVector_gtIndex[b], b) );
          pair_hasRichInd.push_back(fVector_withRichSignal[a]);
          pair_hasRichInd.push_back(fVector_withRichSignal[b]);
          fMixedTest_STSonly_hasRichInd.push_back(pair_hasRichInd);
        }
      }
    }
  }
}


void CbmAnaConversionTest::CombinePhotons_STSonly() {
  Int_t nof = fVector_reconstructedPhotons_STSonly.size();
  cout << "CbmAnaConversionTest: CombinePhotons_STSonly, nof - " << nof << endl;
  Int_t nofPi0   = 0;
  Int_t nofPi0_0 = 0;
  Int_t nofPi0_1 = 0;
  Int_t nofPi0_2 = 0;
  Int_t nofPi0_3 = 0;
  Int_t nofPi0_4 = 0;


  if (nof >= 2) {
    for (int a = 0; a < nof - 1; a++) {
      for (int b = a + 1; b < nof; b++) {
        Int_t electron11 =
          fVector_reconstructedPhotons_STSonly[a][0];  // track with STS signal
        Int_t electron12 =
          fVector_reconstructedPhotons_STSonly[a][1];  // track with STS signal
        Int_t electron21 =
          fVector_reconstructedPhotons_STSonly[b][0];  // track with STS signal
        Int_t electron22 =
          fVector_reconstructedPhotons_STSonly[b][1];  // track with STS signal

        Int_t gtIndex11 = fVector_gtIndex[electron11];
        Int_t gtIndex12 = fVector_gtIndex[electron12];
        Int_t gtIndex21 = fVector_gtIndex[electron21];
        Int_t gtIndex22 = fVector_gtIndex[electron22];

        //Int_t richIndex11 = fVector_richIndex[electron11];
        //Int_t richIndex12 = fVector_richIndex[electron12];
        //Int_t richIndex21 = fVector_richIndex[electron21];
        //Int_t richIndex22 = fVector_richIndex[electron22];

        if (gtIndex11 == gtIndex12 || gtIndex11 == gtIndex21
            || gtIndex11 == gtIndex22 || gtIndex12 == gtIndex21
            || gtIndex12 == gtIndex22 || gtIndex21 == gtIndex22) {
          //if(electron12 == electron21 || electron12 == electron22)  {
          //cout << "CbmAnaConversionTest - CombinePhotons_STSonly: Test_DoubleIndex." << endl;
          continue;
        }


        CbmAnaConversionKinematicParams paramsTest =
          CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
            fVector_momenta[electron11],
            fVector_momenta[electron12],
            fVector_momenta[electron21],
            fVector_momenta[electron22]);

        Double_t invmass = paramsTest.fMinv;


        //Int_t nofRICHindices = (HasRichInd(gtIndex11, electron11)) + (HasRichInd(gtIndex12, electron12)) + (HasRichInd(gtIndex21, electron21)) + (HasRichInd(gtIndex22, electron22));
        Int_t nofRICHindices = fVector_withRichSignal[electron11]
                               + fVector_withRichSignal[electron12]
                               + fVector_withRichSignal[electron21]
                               + fVector_withRichSignal[electron22];

        if (nofRICHindices == 0) {
          fhTest_invmass_RICHindex0->Fill(invmass);
          nofPi0_0++;
        }
        if (nofRICHindices == 1) {
          fhTest_invmass_RICHindex1->Fill(invmass);
          nofPi0_1++;
        }
        if (nofRICHindices == 2) {
          fhTest_invmass_RICHindex2->Fill(invmass);
          nofPi0_2++;
        }
        if (nofRICHindices == 3) {
          fhTest_invmass_RICHindex3->Fill(invmass);
          nofPi0_3++;
          //cout << "CbmAnaConversionTest: CombinePhotons_STSonly, photon combined! " << invmass << "/" << gtIndex11 << "/" << gtIndex12 << "/" << gtIndex21 << "/" << gtIndex22 << endl;
        }
        if (nofRICHindices == 4) {
          fhTest_invmass_RICHindex4->Fill(invmass);
          nofPi0_4++;
        }

        nofPi0++;
      }
    }
  }
  cout << "CbmAnaConversionTest: CombinePhotons_STSonly, nofPi0 - " << nofPi0
       << "/" << nofPi0_0 << "/" << nofPi0_1 << "/" << nofPi0_2 << "/"
       << nofPi0_3 << "/" << nofPi0_4 << endl;
}


Bool_t CbmAnaConversionTest::HasRichInd(Int_t gtIndex, Int_t arrayIndex) {
  CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(gtIndex);
  if (NULL == gTrack) return false;
  int stsInd  = gTrack->GetStsTrackIndex();
  int richInd = gTrack->GetRichRingIndex();

  if (stsInd < 0) return false;
  CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(stsInd);
  if (stsTrack == NULL) return false;
  CbmTrackMatchNew* stsMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
  if (stsMatch == NULL) return false;
  if (stsMatch->GetNofLinks() <= 0) return false;
  int stsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
  if (stsMcTrackId < 0) return false;
  CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(stsMcTrackId);
  if (mcTrack1 == NULL) return false;

  if (richInd < 0) return false;
  CbmTrackMatchNew* richMatch =
    (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
  if (richMatch == NULL) return false;
  int richMcTrackId = richMatch->GetMatchedLink().GetIndex();
  if (richMcTrackId < 0) return false;
  CbmMCTrack* mcTrack2 = (CbmMCTrack*) fMcTracks->At(richMcTrackId);
  if (mcTrack2 == NULL) return false;

  /*
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
		fPFFitter_electron.GetChiToVertex(stsTracks_electron, vField_electron, chiPrim_electron, fKFVertex, 3e6);

		TVector3 refittedMomentum_electron;
		const FairTrackParam* vtxTrack_electron = stsTracks_electron[0].GetParamFirst();
		vtxTrack_electron->Momentum(refittedMomentum_electron);
*/


  Bool_t electron_rich = CbmLitGlobalElectronId::GetInstance().IsRichElectron(
    gtIndex, fVector_momenta[arrayIndex].Mag());
  return electron_rich;

  //return true;
}


void CbmAnaConversionTest::MixedEventTest_3p1()
// combines photons from two different events, taken from each time 200 events
{
  Int_t nof = fMixedTest_3p1_photons.size();
  //cout << "CbmAnaConversionRecoFull: MixedEventTest4 - nof entries " << nof << endl;
  for (Int_t a = 0; a < nof - 1; a++) {
    for (Int_t b = a + 1; b < nof; b++) {
      if (fMixedTest_3p1_eventno[a] == fMixedTest_3p1_eventno[b])
        continue;  // to make sure that the photons are from two different events
      if (fMixedTest_3p1_combined[a] + fMixedTest_3p1_combined[b] != 1)
        continue;  // to make sure, that the combination 3+1 is used

      TVector3 e11 = fMixedTest_3p1_photons[a][0];
      TVector3 e12 = fMixedTest_3p1_photons[a][1];
      TVector3 e21 = fMixedTest_3p1_photons[b][0];
      TVector3 e22 = fMixedTest_3p1_photons[b][1];


      CbmAnaConversionKinematicParams params =
        CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
          e11, e12, e21, e22);
      fhTest_eventMixing_3p1->Fill(params.fMinv);
      //cout << "CbmAnaConversionRecoFull: MixedEventTest4(), event filled!, part" << endl;

      if ((fMixedTest_3p1_combined[a] == 1 && e11.Mag() < 0.6)
          || (fMixedTest_3p1_combined[b] == 1 && e21.Mag() < 0.6)) {
        fhTest_eventMixing_3p1_pCut->Fill(params.fMinv);
      }


      Double_t OpeningAngleGG =
        CbmAnaConversionGlobalFunctions::OpeningAngleBetweenGamma(
          e11, e12, e21, e22);
      if (OpeningAngleGG > 15 && OpeningAngleGG < 45)
        fhTest_eventMixing_3p1_GGcut->Fill(params.fMinv);


      //Double_t ANNe11 = fMixedTest_3p1_ann[a][0];
      Double_t ANNe12 = fMixedTest_3p1_ann[a][1];
      Double_t ANNe21 = fMixedTest_3p1_ann[b][0];
      Double_t ANNe22 = fMixedTest_3p1_ann[b][1];

      if (ANNe12 > -1 && ANNe21 > -1 && ANNe22 > -1)
        fhTest_eventMixing_3p1_ANNcuts->Fill(1, params.fMinv);
      if (ANNe12 > -0.9 && ANNe21 > -0.9 && ANNe22 > -0.9)
        fhTest_eventMixing_3p1_ANNcuts->Fill(2, params.fMinv);
      if (ANNe12 > -0.8 && ANNe21 > -0.8 && ANNe22 > -0.8)
        fhTest_eventMixing_3p1_ANNcuts->Fill(3, params.fMinv);
      if (ANNe12 > -0.7 && ANNe21 > -0.7 && ANNe22 > -0.7)
        fhTest_eventMixing_3p1_ANNcuts->Fill(4, params.fMinv);
      if (ANNe12 > -0.6 && ANNe21 > -0.6 && ANNe22 > -0.6)
        fhTest_eventMixing_3p1_ANNcuts->Fill(5, params.fMinv);
      if (ANNe12 > -0.5 && ANNe21 > -0.5 && ANNe22 > -0.5)
        fhTest_eventMixing_3p1_ANNcuts->Fill(6, params.fMinv);
      if (ANNe12 > -0.0 && ANNe21 > -0.0 && ANNe22 > -0.0)
        fhTest_eventMixing_3p1_ANNcuts->Fill(7, params.fMinv);
    }
  }
}


void CbmAnaConversionTest::MixedEventTest_STSonly() {
  Int_t nof = fMixedTest_STSonly_photons.size();
  for (Int_t a = 0; a < nof - 1; a++) {
    for (Int_t b = a + 1; b < nof; b++) {
      if (fMixedTest_STSonly_eventno[a] == fMixedTest_STSonly_eventno[b])
        continue;

      TVector3 e11 = fMixedTest_STSonly_photons[a][0];
      TVector3 e12 = fMixedTest_STSonly_photons[a][1];
      TVector3 e21 = fMixedTest_STSonly_photons[b][0];
      TVector3 e22 = fMixedTest_STSonly_photons[b][1];

      Bool_t IsRich11 = fMixedTest_STSonly_hasRichInd[a][0];
      Bool_t IsRich12 = fMixedTest_STSonly_hasRichInd[a][1];
      Bool_t IsRich21 = fMixedTest_STSonly_hasRichInd[b][0];
      Bool_t IsRich22 = fMixedTest_STSonly_hasRichInd[b][1];
      Int_t IsRichSum = IsRich11 + IsRich12 + IsRich21 + IsRich22;


      CbmAnaConversionKinematicParams params =
        CbmAnaConversionKinematicParams::KinematicParams_4particles_Reco(
          e11, e12, e21, e22);

      if (IsRichSum == 2) {
        fhTest_eventMixing_STSonly_2p2->Fill(params.fMinv);
      }
      if (IsRichSum == 3) {
        fhTest_eventMixing_STSonly_3p1->Fill(params.fMinv);
      }
      if (IsRichSum == 4) {
        fhTest_eventMixing_STSonly_4p0->Fill(params.fMinv);
      }
    }
  }
}


Double_t CbmAnaConversionTest::ElectronANNvalue(Int_t globalTrackIndex,
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
