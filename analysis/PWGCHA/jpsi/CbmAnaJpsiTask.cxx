/* Copyright (C) 2015-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Adrian Amatus Weber, Semen Lebedev */

#include "CbmAnaJpsiTask.h"

#include "CbmAnaJpsiCandidate.h"
#include "CbmAnaJpsiCuts.h"
#include "CbmAnaJpsiHist.h"
#include "CbmAnaJpsiKinematicParams.h"
#include "CbmAnaJpsiUtils.h"
#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmMCTrack.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"
#include "cbm/elid/CbmLitGlobalElectronId.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TMath.h"

#include <iostream>

using namespace std;

CbmAnaJpsiTask::CbmAnaJpsiTask()
  : FairTask("CbmAnaJpsiTask")
  , fEventNum(0)
  , fMcTracks(NULL)
  , fStsPoints(NULL)
  , fStsHits(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fRichPoints(NULL)
  , fRichHits(NULL)
  , fRichRings(NULL)
  , fRichRingMatches(NULL)
  , fTrdPoints(NULL)
  , fTrdHits(NULL)
  , fTrdTracks(NULL)
  , fTrdTrackMatches(NULL)
  , fTofPoints(NULL)
  , fTofHits(NULL)
  , fTofHitsMatches(NULL)
  , fGlobalTracks(NULL)
  , fJpsiCandidates(NULL)
  , fNofHitsInRingMap()
  , fPrimVertex(NULL)
  , fKFVertex()
  , fCandidates()
  , fCuts()
  , fWeight(1.14048e-6)
  , fHM(NULL)
  , fUseTrd(kTRUE)
  , fUseTof(kFALSE)
{
  /*
    fUseTrd=true;
    fUseTof=false;
	fWeight=1.14048e-6;
*/
}

CbmAnaJpsiTask::~CbmAnaJpsiTask() {}

InitStatus CbmAnaJpsiTask::Init()
{
  //cout << "CbmRichUrqmdTest::Init"<<endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { Fatal("CbmAnaJpsiTask::Init", "RootManager not instantised!"); }

  fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (NULL == fMcTracks) { Fatal("CbmAnaJpsiTask::Init", "No MCtrack Array! "); }

  fStsPoints = (TClonesArray*) ioman->GetObject("StsPoint");
  if (NULL == fStsPoints) { Fatal("CbmAnaJpsiTask::Init", "No StsPoint Array! "); }

  fStsHits = (TClonesArray*) ioman->GetObject("StsHit");
  if (NULL == fStsHits) { Fatal("CbmAnaJpsiTask::Init", "No StsHit Array! "); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (NULL == fStsTracks) { Fatal("CbmAnaJpsiTask::Init", "No StsTracks Array! "); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (NULL == fStsTrackMatches) { Fatal("CbmAnaJpsiTask::Init", "No StsTrackMatches Array!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (NULL == fRichHits) { Fatal("CbmAnaJpsiTask::Init", "No RichHits Array! "); }

  fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmAnaJpsiTask::Init", "No RichPoint Array! "); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmAnaJpsiTask::Init", "No RichRings Array! "); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) { Fatal("CbmAnaJpsiTask::Init", "No RichRingMatch array!"); }

  if (fUseTrd == true) {
    fTrdHits = (TClonesArray*) ioman->GetObject("TrdHit");
    if (NULL == fTrdHits) { Fatal("CbmAnaJpsiTask::Init", "No TrdHits Array! "); }

    fTrdPoints = (TClonesArray*) ioman->GetObject("TrdPoint");
    if (NULL == fTrdPoints) { Fatal("CbmAnaJpsiTask::Init", "No TrdPoint Array! "); }

    fTrdTrackMatches = (TClonesArray*) ioman->GetObject("TrdTrackMatch");
    if (NULL == fTrdTrackMatches) { Fatal("CbmAnaDielectronTask::Init", "No TrdTrackMatch array!"); }

    fTrdTracks = (TClonesArray*) ioman->GetObject("TrdTrack");
    if (NULL == fTrdTracks) { Fatal("CbmAnaJpsiTask::Init", "No TrdTracks Array!"); }
  }

  fTofHits = (TClonesArray*) ioman->GetObject("TofHit");
  if (NULL == fTofHits) { Fatal("CbmAnaJpsiTask::Init", "No TofHits Array! "); }

  fTofHitsMatches = (TClonesArray*) ioman->GetObject("TofHitMatch");
  if (NULL == fTofHitsMatches) { Fatal("CbmAnaJpsiTask::Init", "No TofHitMatch Array! "); }

  fTofPoints = (TClonesArray*) ioman->GetObject("TofPoint");
  if (NULL == fTofPoints) { Fatal("CbmAnaJpsiTask::Init", "No TofPoint Array! "); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { Fatal("CbmAnaJpsiTask::Init", "No GlobalTracks Array!"); }

  //   fPrimVertex = (CbmVertex*) ioman->GetObject("PrimaryVertex");
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) { LOG(fatal) << "No PrimaryVertex array!"; }

  fJpsiCandidates = new TClonesArray("CbmAnaJpsiCandidate");
  ioman->Register("JpsiCandidates", "Jpsi", fJpsiCandidates, IsOutputBranchPersistent("JpsiCandidates"));

  InitHist();

  return kSUCCESS;
}


void CbmAnaJpsiTask::CreateAnalysisStepsH1(const string& name, const string& axisX, const string& axisY, double nBins,
                                           double min, double max)
{
  for (Int_t i = 0; i < CbmAnaJpsiHist::fNofAnaSteps; i++) {
    string hname = name + "_" + CbmAnaJpsiHist::fAnaSteps[i];
    fHM->Create1<TH1D>(hname, hname + ";" + axisX + ";" + axisY, nBins, min, max);
  }
}

void CbmAnaJpsiTask::CreateAnalysisStepsH2(const string& name, const string& axisX, const string& axisY,
                                           const string& axisZ, double nBinsX, double minX, double maxX, double nBinsY,
                                           double minY, double maxY)
{
  for (Int_t i = 0; i < CbmAnaJpsiHist::fNofAnaSteps; i++) {
    string hname = name + "_" + CbmAnaJpsiHist::fAnaSteps[i];
    fHM->Create2<TH2D>(hname, hname + ";" + axisX + ";" + axisY + ";" + axisZ, nBinsX, minX, maxX, nBinsY, minY, maxY);
  }
}

void CbmAnaJpsiTask::CreateSourceTypesH1(const string& name, const string& axisX, const string& axisY, double nBins,
                                         double min, double max)
{
  for (Int_t i = 0; i < CbmAnaJpsiHist::fNofSourceTypes; i++) {
    string hname = name + "_" + CbmAnaJpsiHist::fSourceTypes[i];
    fHM->Create1<TH1D>(hname, hname + ";" + axisX + ";" + axisY, nBins, min, max);
  }
}

void CbmAnaJpsiTask::CreateSourceTypesH2(const string& name, const string& axisX, const string& axisY,
                                         const string& axisZ, double nBinsX, double minX, double maxX, double nBinsY,
                                         double minY, double maxY)
{
  string hname = "";
  for (Int_t i = 0; i < CbmAnaJpsiHist::fNofSourceTypes; i++) {
    hname = name + "_" + CbmAnaJpsiHist::fSourceTypes[i];
    fHM->Create2<TH2D>(hname, hname + ";" + axisX + ";" + axisY + ";" + axisZ, nBinsX, minX, maxX, nBinsY, minY, maxY);
  }
}

void CbmAnaJpsiTask::CreateAnaStepsPairSourceH1(const string& name, const string& axisX, const string& axisY,
                                                double nBins, double min, double max)
{
  for (Int_t i = 0; i < CbmAnaJpsiHist::fNofAnaSteps - 2; i++) {
    string hname1 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "gg";
    fHM->Create1<TH1D>(hname1, hname1 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname2 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "gp";
    fHM->Create1<TH1D>(hname2, hname2 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname3 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "go";
    fHM->Create1<TH1D>(hname3, hname3 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname4 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "pg";
    fHM->Create1<TH1D>(hname4, hname4 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname5 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "pp";
    fHM->Create1<TH1D>(hname5, hname5 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname6 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "po";
    fHM->Create1<TH1D>(hname6, hname6 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname7 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "og";
    fHM->Create1<TH1D>(hname7, hname7 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname8 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "op";
    fHM->Create1<TH1D>(hname8, hname8 + ";" + axisX + ";" + axisY, nBins, min, max);
    string hname9 = name + "_" + CbmAnaJpsiHist::fAnaSteps[i + 2] + "_" + "oo";
    fHM->Create1<TH1D>(hname9, hname9 + ";" + axisX + ";" + axisY, nBins, min, max);
  }
}

void CbmAnaJpsiTask::InitHist()
{
  fHM = new CbmHistManager();

  // Event number counter
  fHM->Create1<TH1D>("fh_event_number", "fh_event_number;a.u.;Number of events", 1, 0, 1.);

  //RICH PMT plane XY
  CreateSourceTypesH2("fh_rich_pmt_xy", "X [cm]", "Y [cm]", "Hits/cm^{2}/event", 220, -110, 110, 400, -200, 200);

  //distributions of ID and analysis cuts
  CreateSourceTypesH1("fh_track_chi2prim", "#chi^{2}_{prim}", "particles/event", 200, 0., 20.);
  CreateSourceTypesH1("fh_track_mom", "P [GeV/c]", "particles/event", 160, 0., 16.);
  CreateSourceTypesH1("fh_track_chi2sts", "#chi^{2}_{STS}", "particles/event", 80, 0., 8.);
  CreateSourceTypesH1("fh_track_rapidity", "Rapidity", "particles/event", 40, 0., 4.);
  CreateSourceTypesH1("fh_track_pt", "P_{t} [GeV/c]", "particles/event", 40, 0., 4.);
  CreateSourceTypesH1("fh_track_rich_ann", "RICH ANN output", "particles/event", 120, -1.2, 1.2);
  CreateSourceTypesH1("fh_track_trd_ann", "TRD ANN output", "particles/event", 120, -1.2, 1.2);
  CreateSourceTypesH2("fh_track_tof_m2", "P [GeV/c]", "m^{2} [GeV/c^{2}]^{2}", "particles/event", 900, 0., 9., 360,
                      -0.4, 1.4);

  //vertex of the secondary electrons from gamma conversion
  fHM->Create2<TH2D>("fh_vertex_el_gamma_xz", "fh_vertex_el_gamma_xz;Z [cm];X [cm];Counter per event", 200, -10., 190.,
                     400, -130., 130.);
  fHM->Create2<TH2D>("fh_vertex_el_gamma_yz", "fh_vertex_el_gamma_yz;Z [cm];Y [cm];Counter per event", 200, -10., 190.,
                     400, -130., 130.);
  fHM->Create2<TH2D>("fh_vertex_el_gamma_xy", "fh_vertex_el_gamma_xy;X [cm];Y [cm];Counter per event", 400, -130., 130.,
                     400, -130., 130.);
  fHM->Create2<TH2D>("fh_vertex_el_gamma_rz", "fh_vertex_el_gamma_rz;Z [cm];#sqrt{X^{2}+Y^{2}} [cm];Counter per event",
                     300, -10., 190., 300, 0., 150.);

  // Number of BG and signal tracks after each cut
  fHM->Create1<TH1D>("fh_nof_bg_tracks", "fh_nof_bg_tracks;Analysis steps;Number of BG particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);
  fHM->Create1<TH1D>("fh_nof_el_tracks", "fh_nof_el_tracks;Analysis steps;Number of signal particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);
  fHM->Create2<TH2D>("fh_source_tracks", "fh_source_tracks;Analysis steps;Particle", CbmAnaJpsiHist::fNofAnaSteps, 0.,
                     CbmAnaJpsiHist::fNofAnaSteps, 7, 0., 7.);

  //Create invariant mass histograms
  CreateAnalysisStepsH1("fh_signal_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0, 4.);
  CreateAnalysisStepsH1("fh_bg_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);
  CreateAnalysisStepsH1("fh_pi0_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);
  CreateAnalysisStepsH1("fh_gamma_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);

  // minv for true matched and mismatched tracks
  CreateAnalysisStepsH1("fh_bg_truematch_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);
  CreateAnalysisStepsH1("fh_bg_truematch_el_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);
  CreateAnalysisStepsH1("fh_bg_truematch_notel_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);
  CreateAnalysisStepsH1("fh_bg_mismatch_minv", "M_{ee} [GeV/c^{2}]", "particles/event", 4000, 0., 4.);

  //Invariant mass vs. Mc Pt
  CreateAnalysisStepsH2("fh_signal_minv_pt", "M_{ee} [GeV/c^{2}]", "P_{t} [GeV/c]", "particles/event", 100, 0., 4., 20,
                        0., 2.);

  // Momentum distribution of the signal
  CreateAnalysisStepsH1("fh_signal_mom", "P [GeV/c]", "particles/event", 250, 0., 25.);
  //Pt/y distibution of the signal
  CreateAnalysisStepsH2("fh_signal_pty", "Rapidity", "P_{t} [GeV/c]", "particles/event", 80, 0., 4., 80, 0., 4.);

  //Number of mismatches after each cut
  fHM->Create1<TH1D>("fh_nof_mismatches", "fh_nof_mismatches;Analysis steps;particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);
  fHM->Create1<TH1D>("fh_nof_mismatches_rich", "fh_nof_mismatches_rich;Analysis steps;particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);
  fHM->Create1<TH1D>("fh_nof_mismatches_trd", "fh_nof_mismatches_trd;Analysis steps;particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);
  fHM->Create1<TH1D>("fh_nof_mismatches_tof", "fh_nof_mismatches_tof;Analysis steps;particles/event",
                     CbmAnaJpsiHist::fNofAnaSteps, 0., CbmAnaJpsiHist::fNofAnaSteps);

  //Momentum distribution of signal electrons
  CreateAnalysisStepsH1("fh_track_el_mom", "P [GeV/c]", "particles/event", 250, 0., 25.);
  fHM->Create2<TH2D>("fh_track_el_mom_mc_rec", "fh_track_el_mom_mc_rec;P_{mc};P_{rec};Entries", 250, 0., 25., 250, 0.,
                     25.);

  CreateAnaStepsPairSourceH1("fh_bg_participants_minv", "m_{inv}", "particles/event", 4000, 0., 4.);

  CreateAnalysisStepsH1("fh_PdgCode_of Others_BG", "PDGCode", "particles/event", 500, -0.5, 499.5);

  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_0",
                     "fh_ee_signal_minv_diff_ptcuts_0;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_1",
                     "fh_ee_signal_minv_diff_ptcuts_1;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_2",
                     "fh_ee_signal_minv_diff_ptcuts_2;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_3",
                     "fh_ee_signal_minv_diff_ptcuts_3;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_4",
                     "fh_ee_signal_minv_diff_ptcuts_4;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_5",
                     "fh_ee_signal_minv_diff_ptcuts_5;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_6",
                     "fh_ee_signal_minv_diff_ptcuts_6;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
  fHM->Create1<TH1D>("fh_ee_signal_minv_diff_ptcuts_7",
                     "fh_ee_signal_minv_diff_ptcuts_7;M_{ee} [GeV/c^{2}];particles/event", 4000, 0., 4.);
}


void CbmAnaJpsiTask::Exec(Option_t*)
{
  fJpsiCandidates->Clear();

  fHM->H1("fh_event_number")->Fill(0.5);

  fEventNum++;
  cout << "CbmAnaJpsiTask, event No. " << fEventNum << endl;

  if (fPrimVertex != NULL) { fKFVertex = CbmKFVertex(*fPrimVertex); }
  else {
    Fatal("CbmAnaJpsiTask::Exec", "No PrimaryVertex array!");
  }

  if (fPrimVertex != NULL) { fKFVertex = CbmKFVertex(*fPrimVertex); }
  else {
    Fatal("CbmAnaDielectronTask::Exec", "No PrimaryVertex array!");
  }

  FillRichRingNofHits();

  MCPairs();

  RichPmtXY();

  FillCandidates();

  AssignMcToCandidates();

  DifferenceSignalAndBg();

  SingleParticleAcceptance();

  PairMcAndAcceptance();

  SignalAndBgReco();

  CopyCandidatesToOutputArray();
}

void CbmAnaJpsiTask::FillRichRingNofHits()
{
  fNofHitsInRingMap.clear();
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iHit));
    if (NULL == hit) continue;

    Int_t iPoint = hit->GetRefId();
    if (iPoint < 0) continue;

    FairMCPoint* point = static_cast<FairMCPoint*>(fRichPoints->At(iPoint));
    if (NULL == point) continue;

    Int_t iMCTrack    = point->GetTrackID();
    CbmMCTrack* track = static_cast<CbmMCTrack*>(fMcTracks->At(iMCTrack));
    if (NULL == track) continue;

    Int_t iMother = track->GetMotherId();
    if (iMother == -1) continue;

    fNofHitsInRingMap[iMother]++;
  }
}

void CbmAnaJpsiTask::MCPairs()
{
  Int_t nMcTracks = fMcTracks->GetEntriesFast();
  for (Int_t i = 0; i < nMcTracks; i++) {
    CbmMCTrack* mctrack      = (CbmMCTrack*) fMcTracks->At(i);
    Bool_t isMcGammaElectron = CbmAnaJpsiUtils::IsMcGammaElectron(mctrack, fMcTracks);
    if (isMcGammaElectron) {
      TVector3 v;
      mctrack->GetStartVertex(v);
      fHM->H2("fh_vertex_el_gamma_xz")->Fill(v.Z(), v.X());
      fHM->H2("fh_vertex_el_gamma_yz")->Fill(v.Z(), v.Y());
      fHM->H2("fh_vertex_el_gamma_xy")->Fill(v.X(), v.Y());
      fHM->H2("fh_vertex_el_gamma_rz")->Fill(v.Z(), sqrt(v.X() * v.X() + v.Y() * v.Y()));
    }
  }  // nMcTracks
}  //MC Pairs

void CbmAnaJpsiTask::FillCandidates()
{
  fCandidates.clear();
  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();

  for (Int_t iGTrack = 0; iGTrack < nofGlobalTracks; iGTrack++) {
    // create candidate, in which we will store some parameters of reconstructed global track
    CbmAnaJpsiCandidate cand;

    //get GlobalTrack from array
    CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGTrack);
    if (NULL == globalTrack) continue;

    // get StsTrack from global track
    cand.fStsInd = globalTrack->GetStsTrackIndex();
    if (cand.fStsInd < 0) continue;
    CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(cand.fStsInd);
    if (NULL == stsTrack) continue;

    //calculate and get track parameters
    CbmAnaJpsiUtils::CalculateAndSetTrackParamsToCandidate(&cand, stsTrack, fKFVertex);

    // RICH
    cand.fRichInd = globalTrack->GetRichRingIndex();
    if (cand.fRichInd < 0) continue;
    CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(cand.fRichInd);
    if (NULL == richRing) continue;

    // TRD
    CbmTrdTrack* trdTrack = NULL;
    if (fUseTrd == true) {
      cand.fTrdInd = globalTrack->GetTrdTrackIndex();
      if (cand.fTrdInd < 0) continue;
      trdTrack = (CbmTrdTrack*) fTrdTracks->At(cand.fTrdInd);
      if (trdTrack == NULL) continue;
    }

    // ToF
    cand.fTofInd = globalTrack->GetTofHitIndex();
    if (cand.fTofInd < 0) continue;
    CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(cand.fTofInd);
    if (tofHit == NULL) continue;

    IsElectron(iGTrack, cand.fMomentum.Mag(), &cand);
    IsRecoTrackAccepted(&cand);
    if (!cand.fIsRecoTrackAccepted) continue;
    // push candidate to the array
    // we store only candidate which have all local segments: STS, RICH, TRD, TOF
    fCandidates.push_back(cand);
  }
}

void CbmAnaJpsiTask::AssignMcToCandidates()
{
  int nCand = fCandidates.size();
  for (int i = 0; i < nCand; i++) {
    //reset MC information
    fCandidates[i].ResetMcParams();

    //STS
    //MCTrackId of the candidate is defined by STS track
    int stsInd                 = fCandidates[i].fStsInd;
    CbmTrackMatchNew* stsMatch = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
    if (stsMatch == NULL) continue;
    fCandidates[i].fStsMcTrackId = stsMatch->GetMatchedLink().GetIndex();
    if (fCandidates[i].fStsMcTrackId < 0) continue;
    CbmMCTrack* mcTrack1 = (CbmMCTrack*) fMcTracks->At(fCandidates[i].fStsMcTrackId);
    if (mcTrack1 == NULL) continue;
    fCandidates[i].fStsMcMotherId      = mcTrack1->GetMotherId();
    fCandidates[i].fMcPdg              = TMath::Abs(mcTrack1->GetPdgCode());
    fCandidates[i].fIsMcSignalElectron = CbmAnaJpsiUtils::IsMcSignalElectron(mcTrack1);
    fCandidates[i].fIsMcGammaElectron  = CbmAnaJpsiUtils::IsMcGammaElectron(mcTrack1, fMcTracks);
    fCandidates[i].fIsMcPi0Electron    = CbmAnaJpsiUtils::IsMcPi0Electron(mcTrack1, fMcTracks);

    // RICH
    int richInd                 = fCandidates[i].fRichInd;
    CbmTrackMatchNew* richMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
    if (richMatch == NULL) continue;
    fCandidates[i].fRichMcTrackId = richMatch->GetMatchedLink().GetIndex();

    // TRD
    //      CbmTrdTrack* trdTrack = NULL;
    if (fUseTrd == true) {
      int trdInd                 = fCandidates[i].fTrdInd;
      CbmTrackMatchNew* trdMatch = (CbmTrackMatchNew*) fTrdTrackMatches->At(trdInd);
      if (trdMatch == NULL) continue;
      fCandidates[i].fTrdMcTrackId = trdMatch->GetMatchedLink().GetIndex();
    }

    // ToF
    int tofInd = fCandidates[i].fTofInd;
    if (tofInd < 0) continue;
    CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(tofInd);
    if (tofHit == NULL) continue;
    CbmMatch* tofHitMatch = static_cast<CbmMatch*>(fTofHitsMatches->At(tofInd));
    if (tofHitMatch == NULL) { continue; }
    Int_t tofPointIndex = tofHitMatch->GetMatchedLink().GetIndex();
    if (tofPointIndex < 0) continue;
    FairMCPoint* tofPoint = (FairMCPoint*) fTofPoints->At(tofPointIndex);
    if (tofPoint == NULL) continue;
    fCandidates[i].fTofMcTrackId = tofPoint->GetTrackID();


    IsMismatch(&fCandidates[i]);
  }  // candidates
}

void CbmAnaJpsiTask::DifferenceSignalAndBg()
{
  Int_t nCand = fCandidates.size();
  for (Int_t i = 0; i < nCand; i++) {
    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H1("fh_track_chi2prim_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(fCandidates[i].fChi2Prim, fWeight);
    }
    else {
      fHM->H1("fh_track_chi2prim_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fChi2Prim);
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H1("fh_track_chi2prim_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fChi2Prim);
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H1("fh_track_chi2prim_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fChi2Prim);
    }
    if (fCandidates[i].fChi2Prim >= fCuts.fChiPrimCut) continue;


    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H1("fh_track_rich_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])->Fill(fCandidates[i].fRichAnn, fWeight);
    }
    else {
      fHM->H1("fh_track_rich_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fRichAnn);
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H1("fh_track_rich_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fRichAnn);
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H1("fh_track_rich_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fRichAnn);
    }
    if (!fCandidates[i].fIsRichEl) continue;

    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H1("fh_track_trd_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])->Fill(fCandidates[i].fTrdAnn, fWeight);
    }
    else {
      fHM->H1("fh_track_trd_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fTrdAnn);
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H1("fh_track_trd_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fTrdAnn);
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H1("fh_track_trd_ann_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fTrdAnn);
    }
    if (!fCandidates[i].fIsTrdEl) continue;

    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H2("fh_track_tof_m2_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(fCandidates[i].fMomentum.Mag(), fCandidates[i].fMass2, fWeight);
    }
    else {
      fHM->H2("fh_track_tof_m2_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])
        ->Fill(fCandidates[i].fMomentum.Mag(), fCandidates[i].fMass2);
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H2("fh_track_tof_m2_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])
        ->Fill(fCandidates[i].fMomentum.Mag(), fCandidates[i].fMass2);
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H2("fh_track_tof_m2_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])
        ->Fill(fCandidates[i].fMomentum.Mag(), fCandidates[i].fMass2);
    }
    if (!fCandidates[i].fIsElectron) continue;

    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H1("fh_track_pt_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(fCandidates[i].fMomentum.Perp(), fWeight);
    }
    else {
      fHM->H1("fh_track_pt_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fMomentum.Perp());
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H1("fh_track_pt_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fMomentum.Perp());
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H1("fh_track_pt_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fMomentum.Perp());
    }

  }  // loop over candidates

  for (Int_t i = 0; i < nCand; i++) {
    if (fCandidates[i].fIsMcSignalElectron) {
      fHM->H1("fh_track_mom_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(fCandidates[i].fMomentum.Mag(), fWeight);
      fHM->H1("fh_track_chi2sts_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])->Fill(fCandidates[i].fChi2sts, fWeight);
      fHM->H1("fh_track_rapidity_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(fCandidates[i].fRapidity, fWeight);
    }
    else {
      fHM->H1("fh_track_mom_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fMomentum.Mag());
      fHM->H1("fh_track_chi2sts_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fChi2sts);
      fHM->H1("fh_track_rapidity_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(fCandidates[i].fRapidity);
    }
    if (fCandidates[i].fIsMcGammaElectron) {
      fHM->H1("fh_track_mom_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fMomentum.Mag());
      fHM->H1("fh_track_chi2sts_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fChi2sts);
      fHM->H1("fh_track_rapidity_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(fCandidates[i].fRapidity);
    }
    if (fCandidates[i].fIsMcPi0Electron) {
      fHM->H1("fh_track_mom_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fMomentum.Mag());
      fHM->H1("fh_track_chi2sts_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fChi2sts);
      fHM->H1("fh_track_rapidity_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(fCandidates[i].fRapidity);
    }
  }  // loop over candidates
}

Bool_t CbmAnaJpsiTask::IsMcTrackAccepted(Int_t mcTrackInd)
{
  CbmMCTrack* tr = (CbmMCTrack*) fMcTracks->At(mcTrackInd);
  if (tr == NULL) return false;
  Int_t nRichPoints = fNofHitsInRingMap[mcTrackInd];
  //TVector3 v;
  //tr->GetStartVertex(v);
  return (tr->GetNPoints(ECbmModuleId::kMvd) + tr->GetNPoints(ECbmModuleId::kSts) >= 4 && nRichPoints >= 7
          && tr->GetNPoints(ECbmModuleId::kTrd) >= 6
          && tr->GetNPoints(ECbmModuleId::kTof) > 0 /*&& tr->GetPt()>fCuts.fPtCut && v.Mag()<0.01*/);
}


void CbmAnaJpsiTask::IsRecoTrackAccepted(CbmAnaJpsiCandidate* cand)
{
  CbmStsTrack* stsTrack = (CbmStsTrack*) fStsTracks->At(cand->fStsInd);
  if (stsTrack == NULL) cand->fIsRecoTrackAccepted = false;
  int nStsHits          = stsTrack->GetNofHits();
  int nMvdHits          = stsTrack->GetNofMvdHits();
  CbmRichRing* richRing = (CbmRichRing*) fRichRings->At(cand->fRichInd);
  if (richRing == NULL) cand->fIsRecoTrackAccepted = false;
  int nRichHits         = richRing->GetNofHits();
  CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTracks->At(cand->fTrdInd);
  if (trdTrack == NULL) cand->fIsRecoTrackAccepted = false;
  int nTrdHits             = trdTrack->GetNofHits();
  CbmTofHit* tofHit        = (CbmTofHit*) fTofHits->At(cand->fTofInd);
  bool nTofHitsGreaterZero = false;
  if (tofHit == NULL) { cand->fIsRecoTrackAccepted = false; }
  else {
    nTofHitsGreaterZero = true;
  }
  if ((nMvdHits + nStsHits) >= 4 && nRichHits >= 7 && nTrdHits >= 6 && nTofHitsGreaterZero)
    cand->fIsRecoTrackAccepted = true;
}

void CbmAnaJpsiTask::SingleParticleAcceptance()
{
  Int_t nMcTracks = fMcTracks->GetEntriesFast();
  for (Int_t i = 0; i < nMcTracks; i++) {
    CbmMCTrack* mctrack = (CbmMCTrack*) fMcTracks->At(i);
    if (CbmAnaJpsiUtils::IsMcSignalElectron(mctrack)) {
      double_t mom = mctrack->GetP();
      fHM->H1("fh_nof_el_tracks")->Fill(kJpsiMc + 0.5, fWeight);
      fHM->H1("fh_track_el_mom_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Fill(mom, fWeight);
      if (IsMcTrackAccepted(i)) {
        fHM->H1("fh_nof_el_tracks")->Fill(kJpsiAcc + 0.5, fWeight);
        fHM->H1("fh_track_el_mom_" + CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Fill(mom, fWeight);
      }
    }
  }
}

void CbmAnaJpsiTask::PairMcAndAcceptance()
{
  Int_t nMcTracks = fMcTracks->GetEntriesFast();
  for (Int_t iP = 0; iP < nMcTracks; iP++) {
    CbmMCTrack* mctrackP       = (CbmMCTrack*) fMcTracks->At(iP);
    Bool_t isMcSignalElectronP = CbmAnaJpsiUtils::IsMcSignalElectron(mctrackP);
    if (!isMcSignalElectronP) continue;
    if (mctrackP->GetPdgCode() != 11) continue;
    Bool_t isAccP = IsMcTrackAccepted(iP);
    for (Int_t iM = 0; iM < nMcTracks; iM++) {
      if (iP == iM) continue;
      CbmMCTrack* mctrackM       = (CbmMCTrack*) fMcTracks->At(iM);
      Bool_t isMcSignalElectronM = CbmAnaJpsiUtils::IsMcSignalElectron(mctrackM);
      if (!isMcSignalElectronM) continue;
      if (mctrackM->GetPdgCode() != -11) continue;
      Bool_t isAccM               = IsMcTrackAccepted(iM);
      CbmAnaJpsiKinematicParams p = CbmAnaJpsiKinematicParams::KinematicParamsWithMcTracks(mctrackP, mctrackM);

      if (isMcSignalElectronM && isMcSignalElectronP) {
        fHM->H2("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Fill(p.fRapidity, p.fPt, fWeight);
        fHM->H1("fh_signal_mom_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Fill(p.fMomentumMag, fWeight);
        fHM->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Fill(p.fMinv, fWeight);
        fHM->H2("fh_signal_minv_pt_" + CbmAnaJpsiHist::fAnaSteps[kJpsiMc])->Fill(p.fMinv, p.fPt, fWeight);
        //accepted e+/-
        if (isAccP && isAccM) {
          //no FillPairHists because we have not declared KinematicParamCand jet
          fHM->H2("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Fill(p.fRapidity, p.fPt, fWeight);
          fHM->H1("fh_signal_mom_" + CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Fill(p.fMomentumMag, fWeight);
          fHM->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Fill(p.fMinv, fWeight);
          fHM->H2("fh_signal_minv_pt_" + CbmAnaJpsiHist::fAnaSteps[kJpsiAcc])->Fill(p.fMinv, p.fPt, fWeight);
        }
      }
    }
  }
}  // PairsAcceptance


void CbmAnaJpsiTask::PairSource(CbmAnaJpsiCandidate* candP, CbmAnaJpsiCandidate* candM, CbmAnaJpsiAnalysisSteps step,
                                CbmAnaJpsiKinematicParams* parRec)
{
  Bool_t isBG     = !(candP->fIsMcSignalElectron && candM->fIsMcSignalElectron);
  Double_t weight = 1.;
  if (candP->fIsMcSignalElectron || candM->fIsMcSignalElectron) weight = fWeight;
  if (isBG) {
    if (candM->fIsMcGammaElectron) {
      if (candP->fIsMcGammaElectron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gg")
          ->Fill(parRec->fMinv, weight);  //gamma + gamma
      }
      else if (candP->fIsMcPi0Electron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gp")
          ->Fill(parRec->fMinv, weight);  //gamma + Pi0
      }
      else {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_go")
          ->Fill(parRec->fMinv, weight);  //gamma + other
        fHM->H1("fh_PdgCode_of Others_BG_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill((double) candP->fMcPdg - 0.5);
      }
    }
    else if (candM->fIsMcPi0Electron) {
      if (candP->fIsMcGammaElectron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_gp")
          ->Fill(parRec->fMinv, weight);  //pi0 + gamma
      }
      else if (candP->fIsMcPi0Electron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_pp")
          ->Fill(parRec->fMinv, weight);  //pi0 + Pi0
      }
      else {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_po")
          ->Fill(parRec->fMinv, weight);  //pi0 + other
        fHM->H1("fh_PdgCode_of Others_BG_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill((double) candP->fMcPdg - 0.5);
      }
    }
    else {
      fHM->H1("fh_PdgCode_of Others_BG_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill((double) candM->fMcPdg - 0.5);
      if (candP->fIsMcGammaElectron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_og")
          ->Fill(parRec->fMinv, weight);  //other + gamma
      }
      else if (candP->fIsMcPi0Electron) {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_po")
          ->Fill(parRec->fMinv, weight);  //other + Pi0
      }
      else {
        fHM->H1("fh_bg_participants_minv_" + CbmAnaJpsiHist::fAnaSteps[step] + "_oo")
          ->Fill(parRec->fMinv, weight);  //other + other
        fHM->H1("fh_PdgCode_of Others_BG_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill((double) candP->fMcPdg - 0.5);
      }
    }
  }
}


void CbmAnaJpsiTask::TrackSource(CbmAnaJpsiCandidate* cand, CbmAnaJpsiAnalysisSteps step, Int_t pdg)
{
  int binNum = (double) step + 0.5;
  //	Double_t mom = cand->fMomentum.Mag();
  //	Double_t pt = cand->fMomentum.Perp();


  if (cand->fIsMcSignalElectron) {
    fHM->H1("fh_nof_el_tracks")->Fill(binNum, fWeight);

    CbmMCTrack* mcCand = (CbmMCTrack*) fMcTracks->At(cand->fStsMcTrackId);
    if (mcCand != NULL) {
      fHM->H1("fh_track_el_mom_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(mcCand->GetP(), fWeight);
      if (step == kJpsiReco) fHM->H2("fh_track_el_mom_mc_rec")->Fill(mcCand->GetP(), cand->fMomentum.Mag(), fWeight);
    }
  }
  else {
    fHM->H1("fh_nof_bg_tracks")->Fill(binNum);

    if (cand->fIsMismatch) fHM->H1("fh_nof_mismatches")->Fill(binNum);
    if (cand->fStsMcTrackId != cand->fRichMcTrackId) fHM->H1("fh_nof_mismatches_rich")->Fill(binNum);
    if (fUseTrd && cand->fStsMcTrackId != cand->fTrdMcTrackId) fHM->H1("fh_nof_mismatches_trd")->Fill(binNum);
    if (cand->fStsMcTrackId != cand->fTofMcTrackId) fHM->H1("fh_nof_mismatches_tof")->Fill(binNum);


    if (cand->fIsMcGammaElectron) { fHM->H2("fh_source_tracks")->Fill(binNum, 0.5); }
    else if (cand->fIsMcPi0Electron) {
      fHM->H2("fh_source_tracks")->Fill(binNum, 1.5);
    }
    else if (pdg == 211 || pdg == -211) {  //Pi+-
      fHM->H2("fh_source_tracks")->Fill(binNum, 2.5);
    }
    else if (pdg == 2212) {  //P
      fHM->H2("fh_source_tracks")->Fill(binNum, 3.5);
    }
    else if (pdg == 321 || pdg == -321) {
      fHM->H2("fh_source_tracks")->Fill(binNum, 4.5);
    }
    else if ((pdg == 11 || pdg == -11) && !cand->fIsMcGammaElectron && !cand->fIsMcPi0Electron
             && !cand->fIsMcSignalElectron) {
      fHM->H2("fh_source_tracks")->Fill(binNum, 5.5);
    }
    else {
      fHM->H2("fh_source_tracks")->Fill(binNum, 6.5);
    }
  }  //Signal or not
}  //TrackSource


void CbmAnaJpsiTask::FillPairHists(CbmAnaJpsiCandidate* candP, CbmAnaJpsiCandidate* candM,
                                   CbmAnaJpsiKinematicParams* parMc, CbmAnaJpsiKinematicParams* parRec,
                                   CbmAnaJpsiAnalysisSteps step)
{
  Bool_t isSignal = candP->fIsMcSignalElectron && candM->fIsMcSignalElectron;
  Bool_t isPi0 = (candP->fIsMcPi0Electron && candM->fIsMcPi0Electron && candP->fStsMcMotherId == candM->fStsMcMotherId);
  Bool_t isGamma =
    (candP->fIsMcGammaElectron && candM->fIsMcGammaElectron && candP->fStsMcMotherId == candM->fStsMcMotherId);
  Bool_t isBG = !isSignal;  //(!isGamma) && (!isPi0) && (!(candP->fIsMcSignalElectron || candM->fIsMcSignalElectron));
  Bool_t isMismatch = (candP->fIsMismatch || candM->fIsMismatch);

  if (isSignal)
    fHM->H2("fh_signal_pty_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parMc->fRapidity, parMc->fPt, fWeight);
  if (isSignal) fHM->H1("fh_signal_mom_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parMc->fMomentumMag, fWeight);
  if (isSignal) fHM->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv, fWeight);
  if (isSignal)
    fHM->H2("fh_signal_minv_pt_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv, parMc->fPt, fWeight);
  if (isBG) {
    if (candP->fIsMcSignalElectron || candM->fIsMcSignalElectron) {
      fHM->H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv, fWeight);
    }
    else {
      fHM->H1("fh_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
    }
  }
  PairSource(candP, candM, step, parRec);
  if (isPi0) fHM->H1("fh_pi0_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
  if (isGamma) fHM->H1("fh_gamma_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
  if (isBG && isMismatch) fHM->H1("fh_bg_mismatch_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
  if (isBG && !isMismatch) {
    fHM->H1("fh_bg_truematch_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
    if (candP->fMcPdg == 11 && candM->fMcPdg == 11)
      fHM->H1("fh_bg_truematch_el_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
    if (candP->fMcPdg != 11 || candM->fMcPdg != 11)
      fHM->H1("fh_bg_truematch_notel_minv_" + CbmAnaJpsiHist::fAnaSteps[step])->Fill(parRec->fMinv);
  }
}


void CbmAnaJpsiTask::SignalAndBgReco()
{
  Int_t ncand = fCandidates.size();

  for (Int_t i = 0; i < ncand; i++) {
    Int_t pdg = 0;
    if (fCandidates[i].fStsMcTrackId > 0) {
      CbmMCTrack* mcTrack = (CbmMCTrack*) fMcTracks->At(fCandidates[i].fStsMcTrackId);
      if (NULL != mcTrack) pdg = mcTrack->GetPdgCode();
    }

    Bool_t isChi2Prim = (fCandidates[i].fChi2Prim < fCuts.fChiPrimCut);
    Bool_t isEl       = (fCandidates[i].fIsElectron);
    Bool_t isPtCut    = (fCandidates[i].fMomentum.Perp() > fCuts.fPtCut);

    TrackSource(&fCandidates[i], kJpsiReco, pdg);
    if (isChi2Prim) TrackSource(&fCandidates[i], kJpsiChi2Prim, pdg);
    if (isChi2Prim && isEl) TrackSource(&fCandidates[i], kJpsiElId, pdg);
    if (isChi2Prim && isEl && isPtCut) TrackSource(&fCandidates[i], kJpsiPtCut, pdg);
  }

  for (Int_t iM = 0; iM < ncand; iM++) {
    if (fCandidates[iM].fCharge < 0) continue;
    CbmMCTrack* mctrackM = NULL;
    if (fCandidates[iM].fStsMcTrackId >= 0) mctrackM = (CbmMCTrack*) fMcTracks->At(fCandidates[iM].fStsMcTrackId);

    for (Int_t iP = 0; iP < ncand; iP++) {
      if (fCandidates[iP].fCharge > 0) continue;
      CbmMCTrack* mctrackP = NULL;
      if (fCandidates[iP].fStsMcTrackId >= 0) mctrackP = (CbmMCTrack*) fMcTracks->At(fCandidates[iP].fStsMcTrackId);
      if (iM == iP) continue;

      CbmAnaJpsiKinematicParams pMC;
      if (mctrackP != NULL && mctrackM != NULL)
        pMC = CbmAnaJpsiKinematicParams::KinematicParamsWithMcTracks(mctrackP, mctrackM);

      CbmAnaJpsiKinematicParams pRec =
        CbmAnaJpsiKinematicParams::KinematicParamsWithCandidates(&fCandidates[iP], &fCandidates[iM]);

      Bool_t isChiPrimary =
        (fCandidates[iP].fChi2Prim < fCuts.fChiPrimCut && fCandidates[iM].fChi2Prim < fCuts.fChiPrimCut);
      Bool_t isEl = (fCandidates[iP].fIsElectron && fCandidates[iM].fIsElectron);
      Bool_t isPtCut =
        (fCandidates[iP].fMomentum.Perp() > fCuts.fPtCut && fCandidates[iM].fMomentum.Perp() > fCuts.fPtCut);
      Bool_t isSignal = (fCandidates[iP].fIsMcSignalElectron && fCandidates[iM].fIsMcSignalElectron);

      FillPairHists(&fCandidates[iP], &fCandidates[iM], &pMC, &pRec, kJpsiReco);
      if (isChiPrimary) { FillPairHists(&fCandidates[iP], &fCandidates[iM], &pMC, &pRec, kJpsiChi2Prim); }
      if (isChiPrimary && isEl) { FillPairHists(&fCandidates[iP], &fCandidates[iM], &pMC, &pRec, kJpsiElId); }
      if (isChiPrimary && isEl && isPtCut) {
        FillPairHists(&fCandidates[iP], &fCandidates[iM], &pMC, &pRec, kJpsiPtCut);
      }

      if (isSignal && isChiPrimary && isEl && pRec.fPt < 0.4) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_0")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 0.4 && pRec.fPt < 0.8) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_1")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 0.8 && pRec.fPt < 1.2) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_2")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 1.2 && pRec.fPt < 1.6) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_3")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 1.6 && pRec.fPt < 2.0) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_4")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 2.0 && pRec.fPt < 2.4) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_5")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 2.4 && pRec.fPt < 3.0) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_6")->Fill(pRec.fMinv, fWeight);
      }
      if (isSignal && isChiPrimary && isEl && pRec.fPt >= 3.0 && pRec.fPt < 6.0) {
        fHM->H1("fh_ee_signal_minv_diff_ptcuts_7")->Fill(pRec.fMinv, fWeight);
      }

    }  //iM
  }    //iP
}

void CbmAnaJpsiTask::IsElectron(Int_t globalTrackIndex, Double_t momentum, CbmAnaJpsiCandidate* cand)
{
  Bool_t richEl  = CbmLitGlobalElectronId::GetInstance().IsRichElectron(globalTrackIndex, momentum);
  cand->fRichAnn = CbmLitGlobalElectronId::GetInstance().GetRichAnn(globalTrackIndex, momentum);

  Bool_t trdEl  = CbmLitGlobalElectronId::GetInstance().IsTrdElectron(globalTrackIndex, momentum);
  cand->fTrdAnn = CbmLitGlobalElectronId::GetInstance().GetTrdAnn(globalTrackIndex, momentum);

  Bool_t tofEl = CbmLitGlobalElectronId::GetInstance().IsTofElectron(globalTrackIndex, momentum);
  //Bool_t momCut = (fCuts.fMomentumCut > 0.)?(momentum < fCuts.fMomentumCut):true;

  cand->fIsRichEl   = richEl;
  cand->fIsTrdEl    = trdEl;
  cand->fIsTofEl    = (fUseTof) ? tofEl : true;
  cand->fIsElectron = (richEl && trdEl && tofEl);
}


void CbmAnaJpsiTask::IsMismatch(CbmAnaJpsiCandidate* cand)
{
  Bool_t IsTrdMcTrackId = (fUseTrd) ? (cand->fStsMcTrackId == cand->fTrdMcTrackId) : true;
  if (cand->fStsMcTrackId == cand->fRichMcTrackId && IsTrdMcTrackId && cand->fStsMcTrackId == cand->fTofMcTrackId
      && cand->fStsMcTrackId != -1)
    cand->fIsMismatch = false;
  else {
    cand->fIsMismatch = true;
  }
}

void CbmAnaJpsiTask::RichPmtXY()
{
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t i = 0; i < nofRichHits; i++) {
    // get the RichHit from array
    CbmRichHit* richHit = (CbmRichHit*) fRichHits->At(i);
    if (NULL == richHit) continue;
    Int_t PointInd = richHit->GetRefId();
    if (PointInd < 0) continue;

    // get the McRichPoint of the RichHit
    CbmRichPoint* richMcPoint = (CbmRichPoint*) fRichPoints->At(PointInd);
    if (NULL == richMcPoint) continue;

    // get the RICH photon MC Track
    Int_t photonMcTrackId = richMcPoint->GetTrackID();
    if (photonMcTrackId == -1) continue;
    CbmMCTrack* photonMcTrack = (CbmMCTrack*) fMcTracks->At(photonMcTrackId);
    if (NULL == photonMcTrack) continue;

    // get photon mother MC Track (electron, pion etc.)
    Int_t photonMotherId = photonMcTrack->GetMotherId();
    if (photonMotherId == -1) continue;
    CbmMCTrack* photonMotherMcTrack = (CbmMCTrack*) fMcTracks->At(photonMotherId);
    if (NULL == photonMotherMcTrack) continue;

    Bool_t isMcSignalElectron = CbmAnaJpsiUtils::IsMcSignalElectron(photonMotherMcTrack);
    Bool_t isMcGammaElectron  = CbmAnaJpsiUtils::IsMcGammaElectron(photonMotherMcTrack, fMcTracks);
    Bool_t isMcPi0Electron    = CbmAnaJpsiUtils::IsMcPi0Electron(photonMotherMcTrack, fMcTracks);
    if (isMcSignalElectron) {
      fHM->H2("fh_rich_pmt_xy_" + CbmAnaJpsiHist::fSourceTypes[kJpsiSignal])
        ->Fill(richHit->GetX(), richHit->GetY(), fWeight);
    }
    else {
      fHM->H2("fh_rich_pmt_xy_" + CbmAnaJpsiHist::fSourceTypes[kJpsiBg])->Fill(richHit->GetX(), richHit->GetY());
    }
    if (isMcGammaElectron)
      fHM->H2("fh_rich_pmt_xy_" + CbmAnaJpsiHist::fSourceTypes[kJpsiGamma])->Fill(richHit->GetX(), richHit->GetY());
    if (isMcPi0Electron)
      fHM->H2("fh_rich_pmt_xy_" + CbmAnaJpsiHist::fSourceTypes[kJpsiPi0])->Fill(richHit->GetX(), richHit->GetY());
  }
}

void CbmAnaJpsiTask::CopyCandidatesToOutputArray()
{
  for (UInt_t i = 0; i < fCandidates.size(); i++) {
    new ((*fJpsiCandidates)[i]) CbmAnaJpsiCandidate(fCandidates[i]);
  }
}


void CbmAnaJpsiTask::Finish()
{
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
}


ClassImp(CbmAnaJpsiTask)
