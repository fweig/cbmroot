/* Copyright (C) 2012-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

/**
 * \file CbmRichUrqmdTest.cxx
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 **/

#include "CbmRichUrqmdTest.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDigi.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"

#include "FairMCPoint.h"
#include "FairTrackParam.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH1.h"
#include "TH1D.h"
#include "TStyle.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using boost::assign::list_of;

CbmRichUrqmdTest::CbmRichUrqmdTest()
  : FairTask("CbmRichUrqmdTest")
  , fHM(NULL)
  , fOutputDir("")
  , fRichHits(NULL)
  , fRichRings(NULL)
  , fRichPoints(NULL)
  , fMcTracks(NULL)
  , fRichRingMatches(NULL)
  , fRichProjections(NULL)
  , fDigiMan(nullptr)
  , fEventList(NULL)
  , fEventNum(0)
  , fMinNofHits(7)
  , fNofHitsInRingMap()
{
}

CbmRichUrqmdTest::~CbmRichUrqmdTest() {}

InitStatus CbmRichUrqmdTest::Init()
{
  cout << "CbmRichUrqmdTest::Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { Fatal("CbmRichUrqmdTest::Init", "RootManager not instantised!"); }

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (mcManager == nullptr) LOG(fatal) << "CbmRichUrqmdTest::Init() NULL MCDataManager.";

  fMcTracks = mcManager->InitBranch("MCTrack");
  if (NULL == fMcTracks) { LOG(fatal) << "CbmRichUrqmdTest::Init No MCTrack!"; }

  fRichPoints = mcManager->InitBranch("RichPoint");
  if (NULL == fRichPoints) { LOG(fatal) << "CbmRichUrqmdTest::Init No RichPoint!"; }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (NULL == fRichHits) { LOG(fatal) << "CbmRichUrqmdTest::Init No RichHit!"; }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (NULL == fRichRings) { LOG(fatal) << "CbmRichUrqmdTest::Init No RichRing!"; }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) { LOG(fatal) << "CbmRichUrqmdTest::Init No RichRingMatch!"; }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (NULL == fRichProjections) { LOG(fatal) << "CbmRichUrqmdTest::Init No fRichProjection !"; }

  fEventList = (CbmMCEventList*) ioman->GetObject("MCEventList.");
  if (NULL == fEventList) { LOG(fatal) << "CbmRichUrqmdTest::Init No MCEventList!"; }

  InitHistograms();

  return kSUCCESS;
}

void CbmRichUrqmdTest::Exec(Option_t* /*option*/)
{
  fEventNum++;

  cout << "CbmRichUrqmdTest, event No. " << fEventNum << endl;

  FillRichRingNofHits();
  NofRings();
  NofHitsAndPoints();
  NofProjections();
  Vertex();
  PmtXYSource();
}

void CbmRichUrqmdTest::InitHistograms()
{
  fHM = new CbmHistManager();

  fHM->Create1<TH1D>("fh_vertex_z", "fh_vertex_z;z [cm];# vertices per event", 350, -1., 350);
  fHM->Create1<TH1D>("fh_vertex_z_sts", "fh_vertex_z_sts;z [cm];# vertices per event", 222, -1., 110.);
  fHM->Create2<TH2D>("fh_vertex_xy", "fh_vertex_xy;x [cm];y [cm];# vertices per event", 100, -200., 200., 100, -200.,
                     200.);
  fHM->Create2<TH2D>("fh_vertex_zy", "fh_vertex_zy;z [cm];y [cm];# vertices per event", 350, -1., 350, 100, -200.,
                     200.);
  fHM->Create2<TH2D>("fh_vertex_zx", "fh_vertex_zx;z [cm];x [cm];# vertices per event", 350, -1., 350, 100, -200.,
                     200.);
  fHM->Create2<TH2D>("fh_vertex_xy_z100_180", "fh_vertex_xy_z100_180;x [cm];y [cm];# vertices per event", 100, -200.,
                     200., 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_xy_z180_370", "fh_vertex_xy_z180_370;x [cm];y [cm];# vertices per event", 100, -200.,
                     200., 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_xy_z180_230", "fh_vertex_xy_z180_230;x [cm];y [cm];# vertices per event", 100, -200.,
                     200., 100, -200., 200.);

  fHM->Create2<TH2D>("fh_vertex_xy_z5", "fh_vertex_xy_z5;x [cm];y [cm];# vertices per event", 100, -100., 100., 100,
                     -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z5_15", "fh_vertex_xy_z5_15;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z15_25", "fh_vertex_xy_z15_25;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z25_35", "fh_vertex_xy_z25_35;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z35_45", "fh_vertex_xy_z35_45;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z45_55", "fh_vertex_xy_z45_55;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z55_65", "fh_vertex_xy_z55_65;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z65_75", "fh_vertex_xy_z65_75;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z75_85", "fh_vertex_xy_z75_85;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z85_95", "fh_vertex_xy_z85_95;x [cm];y [cm];# vertices per event", 100, -100., 100.,
                     100, -100., 100.);
  fHM->Create2<TH2D>("fh_vertex_xy_z95_105", "fh_vertex_xy_z95_105;x [cm];y [cm];# vertices per event", 100, -100.,
                     100., 100, -100., 100.);

  fHM->Create1<TH1D>("fh_nof_rings_1hit", "fh_nof_rings_1hit;# detected particles/event;Yield", 250, -.5, 249.5);
  fHM->Create1<TH1D>("fh_nof_rings_7hits", "fh_nof_rings_7hits;# detected particles/event;Yield", 250, -.5, 249.5);
  fHM->Create1<TH1D>("fh_nof_rings_prim_1hit", "fh_nof_rings_prim_1hit;# detected particles/event;Yield", 50, -.5,
                     69.5);
  fHM->Create1<TH1D>("fh_nof_rings_prim_7hits", "fh_nof_rings_prim_7hits;# detected particles/event;Yield", 50, -.5,
                     69.5);
  fHM->Create1<TH1D>("fh_nof_rings_target_1hit", "fh_nof_rings_target_1hit;# detected particles/event;Yield", 60, -.5,
                     79.5);
  fHM->Create1<TH1D>("fh_nof_rings_target_7hits", "fh_nof_rings_target_7hits;# detected particles/event;Yield", 60, -.5,
                     79.5);

  fHM->Create1<TH1D>("fh_secel_mom", "fh_secel_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_gamma_target_mom", "fh_gamma_target_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_gamma_nontarget_mom", "fh_gamma_nontarget_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_pi_mom", "fh_pi_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_kaon_mom", "fh_kaon_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_mu_mom", "fh_mu_mom;p [GeV/c];Number per event", 100, 0., 20);

  fHM->Create1<TH1D>("fh_nof_points_per_event", "fh_nof_points_per_event;Particle;# MC points per event", 7, .5, 7.5);
  fHM->Create1<TH1D>("fh_nof_hits_per_event", "fh_nof_hits_per_event;# hits per event;Yield", 100, 0, 4000);
  fHM->Create1<TH1D>("fh_nof_hits_per_pmt", "fh_nof_hits_per_pmt;# hits per PMT;% of total", 65, -0.5, 64.5);

  vector<Double_t> xPmtBins = CbmRichDraw::GetPmtHistXbins();
  vector<Double_t> yPmtBins = CbmRichDraw::GetPmtHistYbins();

  // before drawing must be normalized by 1/64
  TH2D* fh_hitrate_xy = new TH2D("fh_hitrate_xy", "fh_hitrate_xy;X [cm];Y [cm];# hits/pixel/s", xPmtBins.size() - 1,
                                 &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_hitrate_xy", fh_hitrate_xy);

  TH2D* fh_hits_xy = new TH2D("fh_hits_xy", "fh_hits_xy;X [cm];Y [cm];# hits/PMT/event", xPmtBins.size() - 1,
                              &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_hits_xy", fh_hits_xy);

  TH2D* fh_points_xy = new TH2D("fh_points_xy", "fh_points_xy;X [cm];Y [cm];# MC points/PMT/event", xPmtBins.size() - 1,
                                &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_points_xy", fh_points_xy);

  TH2D* fh_points_xy_pions = new TH2D("fh_points_xy_pions", "fh_points_xy_pions;X [cm];Y [cm];# MC points/PMT/event",
                                      xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_points_xy_pions", fh_points_xy_pions);

  TH2D* fh_points_xy_gamma_target =
    new TH2D("fh_points_xy_gamma_target", "fh_points_xy_gamma_target;X [cm];Y [cm];# MC points/PMT/event",
             xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_points_xy_gamma_target", fh_points_xy_gamma_target);

  TH2D* fh_points_xy_gamma_nontarget =
    new TH2D("fh_points_xy_gamma_nontarget", "fh_points_xy_gamma_nontarget;X [cm];Y [cm];# MC points/PMT/event",
             xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_points_xy_gamma_nontarget", fh_points_xy_gamma_nontarget);

  TH2D* fh_skipped_pmt_10_xy =
    new TH2D("fh_skipped_pmt_10_xy", "fh_skipped_pmt_10_xy;X [cm];Y [cm];# skipped PMTs (>10 hits) [%]",
             xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_skipped_pmt_10_xy", fh_skipped_pmt_10_xy);

  TH2D* fh_skipped_pmt_20_xy =
    new TH2D("fh_skipped_pmt_20_xy", "fh_skipped_pmt_20_xy;X [cm];Y [cm];# skipped PMTs (>20 hits) [%]",
             xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_skipped_pmt_20_xy", fh_skipped_pmt_20_xy);

  TH2D* fh_skipped_pmt_30_xy =
    new TH2D("fh_skipped_pmt_30_xy", "fh_skipped_pmt_30_xy;X [cm];Y [cm];# skipped PMTs (>30 hits) [%]",
             xPmtBins.size() - 1, &xPmtBins[0], yPmtBins.size() - 1, &yPmtBins[0]);
  fHM->Add("fh_skipped_pmt_30_xy", fh_skipped_pmt_30_xy);

  fHM->Create1<TH1D>("fh_nof_proj_per_event", "fh_nof_proj_per_event;# tracks per event;Yield", 50, 0, 1000);
  fHM->Create2<TH2D>("fh_proj_xy", "fh_proj_xy;X [cm];Y [cm];# tracks/cm^{2}/event", 240, -120, 120, 420, -210, 210);
}

void CbmRichUrqmdTest::FillRichRingNofHits()
{
  fNofHitsInRingMap.clear();
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iHit));
    if (NULL == hit) continue;
    Int_t digiIndex = hit->GetRefId();
    if (digiIndex < 0) continue;
    const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(digiIndex);
    if (NULL == digi) continue;
    const CbmMatch* digiMatch = fDigiMan->GetMatch(ECbmModuleId::kRich, digiIndex);
    if (NULL == digiMatch) continue;
    Int_t eventId = digiMatch->GetMatchedLink().GetEntry();

    vector<pair<Int_t, Int_t>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, hit, fRichPoints, fMcTracks, eventId);
    for (UInt_t i = 0; i < motherIds.size(); i++) {
      fNofHitsInRingMap[motherIds[i]]++;
    }
  }
}

void CbmRichUrqmdTest::NofRings()
{
  Int_t fileId   = 0;
  Int_t nofRings = fRichRings->GetEntriesFast();
  int nRings1hit = 0, nRings7hits = 0;
  int nRingsPrim1hit = 0, nRingsPrim7hits = 0;
  int nRingsTarget1hit = 0, nRingsTarget7hits = 0;
  for (Int_t iR = 0; iR < nofRings; iR++) {
    const CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
    if (NULL == ring) continue;
    const CbmTrackMatchNew* ringMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (NULL == ringMatch) continue;

    Int_t mcEventId           = ringMatch->GetMatchedLink().GetEntry();
    Int_t mcTrackId           = ringMatch->GetMatchedLink().GetIndex();
    const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, mcEventId, mcTrackId));
    if (NULL == mcTrack) continue;
    Int_t motherId = mcTrack->GetMotherId();
    Int_t pdg      = TMath::Abs(mcTrack->GetPdgCode());
    double mom     = mcTrack->GetP();
    TVector3 vert;
    mcTrack->GetStartVertex(vert);
    double dZ = vert.Z();

    if (motherId == -1 && pdg == 11) continue;  // do not calculate embedded electrons

    int nofHits = ring->GetNofHits();
    if (nofHits >= 1) nRings1hit++;
    if (nofHits >= fMinNofHits) nRings7hits++;

    if (motherId == -1 && nofHits >= 1) nRingsPrim1hit++;
    if (motherId == -1 && nofHits >= fMinNofHits) nRingsPrim7hits++;

    if (dZ < 0.1 && nofHits >= 1) nRingsTarget1hit++;
    if (dZ < 0.1 && nofHits >= fMinNofHits) nRingsTarget7hits++;

    if (nofHits >= 1) {
      if (motherId != -1) {
        int motherPdg            = -1;
        const CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, mcEventId, motherId));
        if (NULL != mother) motherPdg = mother->GetPdgCode();
        if (motherId != -1 && pdg == 11 && motherPdg != 22) fHM->H1("fh_secel_mom")->Fill(mom);

        if (motherId != -1 && pdg == 11 && motherPdg == 22) {
          if (dZ < 0.1) { fHM->H1("fh_gamma_target_mom")->Fill(mom); }
          else {
            fHM->H1("fh_gamma_nontarget_mom")->Fill(mom);
          }
        }
      }
      if (pdg == 211) fHM->H1("fh_pi_mom")->Fill(mom);
      if (pdg == 321) fHM->H1("fh_kaon_mom")->Fill(mom);
      if (pdg == 13) fHM->H1("fh_mu_mom")->Fill(mom);
    }
  }
  fHM->H1("fh_nof_rings_1hit")->Fill(nRings1hit);
  fHM->H1("fh_nof_rings_7hits")->Fill(nRings7hits);

  fHM->H1("fh_nof_rings_prim_1hit")->Fill(nRingsPrim1hit);
  fHM->H1("fh_nof_rings_prim_7hits")->Fill(nRingsPrim7hits);

  fHM->H1("fh_nof_rings_target_1hit")->Fill(nRingsTarget1hit);
  fHM->H1("fh_nof_rings_target_7hits")->Fill(nRingsTarget7hits);
}

void CbmRichUrqmdTest::NofHitsAndPoints()
{
  int nofHits = fRichHits->GetEntriesFast();
  fHM->H1("fh_nof_hits_per_event")->Fill(nofHits);
  map<Int_t, Int_t> digisPerPmtMap;
  for (int iH = 0; iH < nofHits; iH++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    if (nullptr == hit) continue;
    fHM->H2("fh_hits_xy")->Fill(hit->GetX(), hit->GetY());
    fHM->H2("fh_hitrate_xy")->Fill(hit->GetX(), hit->GetY());

    Int_t digiInd               = hit->GetRefId();
    const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(digiInd);
    if (nullptr == richDigi) continue;
    CbmRichPixelData* pixelData = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(richDigi->GetAddress());
    if (nullptr == pixelData) continue;
    Int_t pmtId = pixelData->fPmtId;
    digisPerPmtMap[pmtId]++;
  }

  for (auto const& it : digisPerPmtMap) {
    Int_t pmtId             = it.first;
    Int_t nofDigis          = it.second;
    CbmRichPmtData* pmtData = CbmRichDigiMapManager::GetInstance().GetPmtDataById(pmtId);
    TVector3 inPos(pmtData->fX, pmtData->fY, pmtData->fZ);
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    if (nofDigis > 10) fHM->H2("fh_skipped_pmt_10_xy")->Fill(outPos.X(), outPos.Y());
    if (nofDigis > 20) fHM->H2("fh_skipped_pmt_20_xy")->Fill(outPos.X(), outPos.Y());
    if (nofDigis > 30) fHM->H2("fh_skipped_pmt_30_xy")->Fill(outPos.X(), outPos.Y());
  }

  vector<Int_t> allPmtIds = CbmRichDigiMapManager::GetInstance().GetPmtIds();
  for (Int_t pmtId : allPmtIds) {
    fHM->H1("fh_nof_hits_per_pmt")->Fill(digisPerPmtMap[pmtId]);
  }

  Int_t nofEvents = fEventList->GetNofEvents();
  for (Int_t iE = 0; iE < nofEvents; iE++) {
    Int_t fileId  = fEventList->GetFileIdByIndex(iE);
    Int_t eventId = fEventList->GetEventIdByIndex(iE);
    int nofPoints = fRichPoints->Size(fileId, eventId);
    for (int i = 0; i < nofPoints; i++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, i));
      if (NULL == point) continue;
      fHM->H1("fh_nof_points_per_event")->Fill(1);

      Int_t mcPhotonTrackId = point->GetTrackID();
      if (mcPhotonTrackId < 0) continue;
      const CbmMCTrack* mcPhotonTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, mcPhotonTrackId));
      if (NULL == mcPhotonTrack) continue;
      Int_t motherPhotonId = mcPhotonTrack->GetMotherId();
      if (motherPhotonId < 0) continue;
      const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherPhotonId));
      if (NULL == mcTrack) continue;
      Int_t motherId = mcTrack->GetMotherId();

      Int_t pdg = TMath::Abs(mcTrack->GetPdgCode());
      TVector3 vert;
      mcTrack->GetStartVertex(vert);
      double dZ = vert.Z();

      if (motherId == -1 && pdg == 11) continue;  // do not calculate embedded electrons

      if (motherId != -1) {
        int motherPdg            = -1;
        const CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherId));
        if (NULL != mother) motherPdg = mother->GetPdgCode();
        if (motherId != -1 && pdg == 11 && motherPdg != 22) fHM->H1("fh_nof_points_per_event")->Fill(2);

        if (motherId != -1 && pdg == 11 && motherPdg == 22) {
          if (dZ < 0.1) { fHM->H1("fh_nof_points_per_event")->Fill(3); }
          else {
            fHM->H1("fh_nof_points_per_event")->Fill(4);
          }
        }
      }
      if (pdg == 211) fHM->H1("fh_nof_points_per_event")->Fill(5);
      if (pdg == 321) fHM->H1("fh_nof_points_per_event")->Fill(6);
      if (pdg == 13) fHM->H1("fh_nof_points_per_event")->Fill(7);
    }
  }
}

void CbmRichUrqmdTest::PmtXYSource()
{
  Int_t nofEvents = fEventList->GetNofEvents();
  for (Int_t iE = 0; iE < nofEvents; iE++) {
    Int_t fileId  = fEventList->GetFileIdByIndex(iE);
    Int_t eventId = fEventList->GetEventIdByIndex(iE);
    int nofPoints = fRichPoints->Size(fileId, eventId);
    for (int i = 0; i < nofPoints; i++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, i));
      if (NULL == point) continue;

      Int_t iMCTrack          = point->GetTrackID();
      const CbmMCTrack* track = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iMCTrack));
      if (NULL == track) continue;

      Int_t iMother = track->GetMotherId();
      if (iMother == -1) continue;

      const CbmMCTrack* track2 = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iMother));
      if (NULL == track2) continue;
      int pdg      = TMath::Abs(track2->GetPdgCode());
      int motherId = track2->GetMotherId();
      TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
      TVector3 outPos;
      CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

      fHM->H2("fh_points_xy")->Fill(outPos.X(), outPos.Y());
      if (motherId != -1) {
        int motherPdg            = -1;
        const CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherId));
        if (NULL != mother) motherPdg = mother->GetPdgCode();
        TVector3 vert;
        track2->GetStartVertex(vert);
        if (motherId != -1 && pdg == 11 && motherPdg == 22) {
          if (vert.Z() < 0.1) { fHM->H2("fh_points_xy_gamma_target")->Fill(outPos.X(), outPos.Y()); }
          else {
            fHM->H2("fh_points_xy_gamma_nontarget")->Fill(outPos.X(), outPos.Y());
          }
        }
      }
      if (pdg == 211) fHM->H2("fh_points_xy_pions")->Fill(outPos.X(), outPos.Y());
    }
  }
}

void CbmRichUrqmdTest::NofProjections()
{
  if (fRichProjections == NULL) return;
  int nofProj     = fRichProjections->GetEntriesFast();
  int nofGoodProj = 0;
  for (int i = 0; i < nofProj; i++) {
    FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(i);
    if (NULL == proj) continue;
    fHM->H2("fh_proj_xy")->Fill(proj->GetX(), proj->GetY());
    if (proj->GetX() != 0 && proj->GetY() != 0) nofGoodProj++;
  }
  fHM->H1("fh_nof_proj_per_event")->Fill(nofGoodProj);
}

void CbmRichUrqmdTest::Vertex()
{
  Int_t nofEvents = fEventList->GetNofEvents();
  for (Int_t iE = 0; iE < nofEvents; iE++) {
    Int_t fileId  = fEventList->GetFileIdByIndex(iE);
    Int_t eventId = fEventList->GetEventIdByIndex(iE);

    int nMcTracks = fMcTracks->Size(fileId, eventId);
    for (int i = 0; i < nMcTracks; i++) {
      //At least one hit in RICH
      pair<Int_t, Int_t> val = std::make_pair(eventId, i);
      if (fNofHitsInRingMap[val] < 1) continue;
      const CbmMCTrack* mctrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, i));
      TVector3 v;
      mctrack->GetStartVertex(v);
      fHM->H1("fh_vertex_z")->Fill(v.Z());
      fHM->H1("fh_vertex_z_sts")->Fill(v.Z());
      fHM->H2("fh_vertex_xy")->Fill(v.X(), v.Y());
      fHM->H2("fh_vertex_zy")->Fill(v.Z(), v.Y());
      fHM->H2("fh_vertex_zx")->Fill(v.Z(), v.X());
      if (v.Z() >= 100 && v.Z() <= 180) fHM->H2("fh_vertex_xy_z100_180")->Fill(v.X(), v.Y());
      if (v.Z() >= 180 && v.Z() <= 370) fHM->H2("fh_vertex_xy_z180_370")->Fill(v.X(), v.Y());
      if (v.Z() >= 180 && v.Z() <= 230) fHM->H2("fh_vertex_xy_z180_230")->Fill(v.X(), v.Y());

      if (v.Z() <= 5) fHM->H2("fh_vertex_xy_z5")->Fill(v.X(), v.Y());
      if (v.Z() > 5 && v.Z() <= 15) fHM->H2("fh_vertex_xy_z5_15")->Fill(v.X(), v.Y());
      if (v.Z() > 15 && v.Z() <= 25) fHM->H2("fh_vertex_xy_z15_25")->Fill(v.X(), v.Y());
      if (v.Z() > 25 && v.Z() <= 35) fHM->H2("fh_vertex_xy_z25_35")->Fill(v.X(), v.Y());
      if (v.Z() > 35 && v.Z() <= 45) fHM->H2("fh_vertex_xy_z35_45")->Fill(v.X(), v.Y());
      if (v.Z() > 45 && v.Z() <= 55) fHM->H2("fh_vertex_xy_z45_55")->Fill(v.X(), v.Y());
      if (v.Z() > 55 && v.Z() <= 65) fHM->H2("fh_vertex_xy_z55_65")->Fill(v.X(), v.Y());
      if (v.Z() > 65 && v.Z() <= 75) fHM->H2("fh_vertex_xy_z65_75")->Fill(v.X(), v.Y());
      if (v.Z() > 75 && v.Z() <= 85) fHM->H2("fh_vertex_xy_z75_85")->Fill(v.X(), v.Y());
      if (v.Z() > 85 && v.Z() <= 95) fHM->H2("fh_vertex_xy_z85_95")->Fill(v.X(), v.Y());
      if (v.Z() > 95 && v.Z() <= 105) fHM->H2("fh_vertex_xy_z95_105")->Fill(v.X(), v.Y());
    }
  }
}

void CbmRichUrqmdTest::DrawHist()
{
  cout.precision(4);

  SetDefaultDrawStyle();

  {
    fHM->H1("fh_vertex_z")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_vertex_z", "rich_urqmd_vertex_z", 800, 800);
    DrawH1(fHM->H1("fh_vertex_z"), kLinear, kLog, "hist");
  }

  {
    fHM->H1("fh_vertex_z_sts")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_vertex_z_sts", "rich_urqmd_vertex_z_sts", 800, 800);
    DrawH1(fHM->H1("fh_vertex_z_sts"), kLinear, kLog, "hist");
  }


  {
    fHM->H2("fh_vertex_xy")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_zy")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_zx")->Scale(1. / fEventNum);
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_vertex_xyz", "rich_urqmd_vertex_xyz", 1500, 500);
    c->Divide(3, 1);
    c->cd(1);
    DrawH2(fHM->H2("fh_vertex_xy"), kLinear, kLinear, kLog);
    c->cd(2);
    DrawH2(fHM->H2("fh_vertex_zy"), kLinear, kLinear, kLog);
    c->cd(3);
    DrawH2(fHM->H2("fh_vertex_zx"), kLinear, kLinear, kLog);
  }

  {
    gStyle->SetOptTitle(1);

    fHM->H2("fh_vertex_xy_z5")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z5_15")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z15_25")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z25_35")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z35_45")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z45_55")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z55_65")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z65_75")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z75_85")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z85_95")->Scale(1. / fEventNum);
    fHM->H2("fh_vertex_xy_z95_105")->Scale(1. / fEventNum);


    TCanvas* c = fHM->CreateCanvas("rich_urqmd_vertex_sts_xyz", "rich_urqmd_vertex_sts_xyz", 1600, 1200);
    c->Divide(4, 3);
    c->cd(1);
    DrawH2(fHM->H2("fh_vertex_xy_z5"), kLinear, kLinear, kLog);
    DrawTextOnPad("Z < 5 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(2);
    DrawH2(fHM->H2("fh_vertex_xy_z5_15"), kLinear, kLinear, kLog);
    DrawTextOnPad("5 < Z < 15 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(3);
    DrawH2(fHM->H2("fh_vertex_xy_z15_25"), kLinear, kLinear, kLog);
    DrawTextOnPad("15 < Z < 25 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(4);
    DrawH2(fHM->H2("fh_vertex_xy_z25_35"), kLinear, kLinear, kLog);
    DrawTextOnPad("25 < Z < 35 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(5);
    DrawH2(fHM->H2("fh_vertex_xy_z35_45"), kLinear, kLinear, kLog);
    DrawTextOnPad("35 < Z < 45 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(6);
    DrawH2(fHM->H2("fh_vertex_xy_z45_55"), kLinear, kLinear, kLog);
    DrawTextOnPad("45 < Z < 55 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(7);
    DrawH2(fHM->H2("fh_vertex_xy_z55_65"), kLinear, kLinear, kLog);
    DrawTextOnPad("55 < Z < 65 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(8);
    DrawH2(fHM->H2("fh_vertex_xy_z65_75"), kLinear, kLinear, kLog);
    DrawTextOnPad("65 < Z < 75 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(9);
    DrawH2(fHM->H2("fh_vertex_xy_z75_85"), kLinear, kLinear, kLog);
    DrawTextOnPad("75 < Z < 85 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(10);
    DrawH2(fHM->H2("fh_vertex_xy_z85_95"), kLinear, kLinear, kLog);
    DrawTextOnPad("85 < Z < 95 cm", 0.3, 0.9, 0.7, 0.98);
    c->cd(11);
    DrawH2(fHM->H2("fh_vertex_xy_z95_105"), kLinear, kLinear, kLog);
    DrawTextOnPad("95 < Z < 105 cm", 0.3, 0.9, 0.7, 0.98);

    gStyle->SetOptTitle(0);
  }

  {
    fHM->H2("fh_vertex_xy_z100_180")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_vertex_xy_z100_180", "rich_urqmd_vertex_xy_z100_180", 800, 800);
    DrawH2(fHM->H2("fh_vertex_xy_z100_180"));
  }

  {
    fHM->H2("fh_vertex_xy_z180_370")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_vertex_xy_z180_370", "rich_urqmd_vertex_xy_z180_370", 800, 800);
    DrawH2(fHM->H2("fh_vertex_xy_z180_370"));
  }

  {
    fHM->H2("fh_vertex_xy_z180_230")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_vertex_xy_z180_230", "rich_urqmd_vertex_xy_z180_230", 800, 800);
    DrawH2(fHM->H2("fh_vertex_xy_z180_230"));
  }

  {
    fHM->H1("fh_nof_points_per_event")->Scale(1. / fEventNum);
    fHM->CreateCanvas("rich_urqmd_nof_points_per_event", "rich_urqmd_nof_points_per_event", 800, 800);
    //gStyle->SetPaintTextFormat("4.1f");
    string labels[7] = {"all",
                        "e^{#pm}_{sec} other",
                        "e^{#pm}_{target} from #gamma",
                        "e^{#pm}_{not target} from #gamma",
                        "#pi^{#pm}",
                        "K^{#pm}",
                        "#mu^{#pm}"};
    for (Int_t i = 1; i <= 7; i++) {
      fHM->H1("fh_nof_points_per_event")->GetXaxis()->SetBinLabel(i, labels[i - 1].c_str());
    }
    fHM->H1("fh_nof_points_per_event")->GetXaxis()->SetLabelSize(0.05);
    //fHM->H1("fh_nof_points_per_event")->SetMarkerSize(0.15);
    DrawH1(fHM->H1("fh_nof_points_per_event"), kLinear, kLog, "hist text0");
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_rings", "rich_urqmd_nof_rings", 800, 800);
    fHM->H1("fh_nof_rings_1hit")->Scale(1. / fHM->H1("fh_nof_rings_1hit")->Integral());
    fHM->H1("fh_nof_rings_7hits")->Scale(1. / fHM->H1("fh_nof_rings_7hits")->Integral());
    stringstream ss1, ss2;
    ss1 << "At least 1 hit detected (" << fHM->H1("fh_nof_rings_1hit")->GetMean() << ")";
    ss2 << "At least 7 hits detected (" << fHM->H1("fh_nof_rings_7hits")->GetMean() << ")";
    DrawH1({fHM->H1("fh_nof_rings_1hit"), fHM->H1("fh_nof_rings_7hits")}, {ss1.str(), ss2.str()}, kLinear, kLinear,
           true, 0.3, 0.85, 0.99, 0.99, "hist");
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_rings_prim", "rich_urqmd_nof_rings_prim", 800, 800);
    fHM->H1("fh_nof_rings_prim_1hit")->Scale(1. / fHM->H1("fh_nof_rings_prim_1hit")->Integral());
    fHM->H1("fh_nof_rings_prim_7hits")->Scale(1. / fHM->H1("fh_nof_rings_prim_7hits")->Integral());
    stringstream ss1, ss2;
    ss1 << "At least 1 hit detected (" << fHM->H1("fh_nof_rings_prim_1hit")->GetMean() << ")";
    ss2 << "At least 7 hits detected (" << fHM->H1("fh_nof_rings_prim_7hits")->GetMean() << ")";
    DrawH1({fHM->H1("fh_nof_rings_prim_1hit"), fHM->H1("fh_nof_rings_prim_7hits")}, {ss1.str(), ss2.str()}, kLinear,
           kLinear, true, 0.3, 0.85, 0.99, 0.99, "hist");
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_rings_target", "rich_urqmd_nof_rings_target", 800, 800);
    fHM->H1("fh_nof_rings_target_1hit")->Scale(1. / fHM->H1("fh_nof_rings_target_1hit")->Integral());
    fHM->H1("fh_nof_rings_target_7hits")->Scale(1. / fHM->H1("fh_nof_rings_target_7hits")->Integral());
    stringstream ss1, ss2;
    ss1 << "At least 1 hit detected (" << fHM->H1("fh_nof_rings_target_1hit")->GetMean() << ")";
    ss2 << "At least 7 hits detected (" << fHM->H1("fh_nof_rings_target_7hits")->GetMean() << ")";
    DrawH1({fHM->H1("fh_nof_rings_target_1hit"), fHM->H1("fh_nof_rings_target_7hits")}, {ss1.str(), ss2.str()}, kLinear,
           kLinear, true, 0.3, 0.85, 0.99, 0.99, "hist");
  }

  {
    fHM->CreateCanvas("rich_urqmd_sources_mom", "rich_urqmd_sources_mom", 800, 800);
    fHM->H1("fh_gamma_target_mom")->Scale(1. / fEventNum);
    fHM->H1("fh_gamma_nontarget_mom")->Scale(1. / fEventNum);
    fHM->H1("fh_secel_mom")->Scale(1. / fEventNum);
    fHM->H1("fh_pi_mom")->Scale(1. / fEventNum);
    fHM->H1("fh_kaon_mom")->Scale(1. / fEventNum);
    fHM->H1("fh_mu_mom")->Scale(1. / fEventNum);
    stringstream ss1, ss2, ss3, ss4, ss5, ss6;
    ss1 << "e^{#pm}_{target} from #gamma (" << fHM->H1("fh_gamma_target_mom")->GetEntries() / fEventNum << ")";
    ss2 << "e^{#pm}_{not target} from #gamma (" << fHM->H1("fh_gamma_nontarget_mom")->GetEntries() / fEventNum << ")";
    ss3 << "e^{#pm}_{sec} other (" << fHM->H1("fh_secel_mom")->GetEntries() / fEventNum << ")";
    ss4 << "#pi^{#pm} (" << fHM->H1("fh_pi_mom")->GetEntries() / fEventNum << ")";
    ss5 << "K^{#pm} (" << fHM->H1("fh_kaon_mom")->GetEntries() / fEventNum << ")";
    ss6 << "#mu^{#pm} (" << fHM->H1("fh_mu_mom")->GetEntries() / fEventNum << ")";
    DrawH1({fHM->H1("fh_gamma_target_mom"), fHM->H1("fh_gamma_nontarget_mom"), fHM->H1("fh_secel_mom"),
            fHM->H1("fh_pi_mom"), fHM->H1("fh_kaon_mom"), fHM->H1("fh_mu_mom")},
           list_of(ss1.str())(ss2.str())(ss3.str())(ss4.str())(ss5.str())(ss6.str()), kLinear, kLog, true, 0.5, 0.7,
           0.99, 0.99, "hist");
  }

  {
    TCanvas* c  = fHM->CreateCanvas("rich_urqmd_hits_xy", "rich_urqmd_hits_xy", 800, 800);
    TH2D* clone = (TH2D*) fHM->H2("fh_hits_xy")->Clone();
    clone->Scale(1. / (fEventNum));
    CbmRichDraw::DrawPmtH2(clone, c, true);
  }

  {
    TCanvas* c  = fHM->CreateCanvas("rich_urqmd_occupancy_xy", "rich_urqmd_occupancy_xy", 800, 800);
    TH2D* clone = (TH2D*) fHM->H2("fh_hits_xy")->Clone();
    clone->GetZaxis()->SetTitle("Occupancy:# hits/PMT/ev./64 [%]");
    clone->Scale(100. / (fEventNum * 64.));
    CbmRichDraw::DrawPmtH2(clone, c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_skipped_pmt_10_xy", "rich_urqmd_skipped_pmt_10_xy", 800, 800);
    fHM->H2("fh_skipped_pmt_10_xy")->Scale(100. / (fEventNum));
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_skipped_pmt_10_xy"), c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_skipped_pmt_20_xy", "rich_urqmd_skipped_pmt_20_xy", 800, 800);
    fHM->H2("fh_skipped_pmt_20_xy")->Scale(100. / (fEventNum));
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_skipped_pmt_20_xy"), c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_skipped_pmt_30_xy", "rich_urqmd_skipped_pmt_30_xy", 800, 800);
    fHM->H2("fh_skipped_pmt_30_xy")->Scale(100. / (fEventNum));
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_skipped_pmt_30_xy"), c, true);
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_hits_per_pmt", "rich_urqmd_nof_hits_per_pmt", 800, 800);
    fHM->H1("fh_nof_hits_per_pmt")->Scale(100. / fHM->H1("fh_nof_hits_per_pmt")->Integral());
    DrawH1(fHM->H1("fh_nof_hits_per_pmt"), kLinear, kLog, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_points_xy", "rich_urqmd_points_xy", 800, 800);
    fHM->H2("fh_points_xy")->Scale(1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_points_xy"), c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_points_xy_pions", "rich_urqmd_points_xy_pions", 800, 800);
    fHM->H2("fh_points_xy_pions")->Scale(1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_points_xy_pions"), c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_points_xy_gamma_target", "rich_urqmd_points_xy_gamma_target", 800, 800);
    fHM->H2("fh_points_xy_gamma_target")->Scale(1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_points_xy_gamma_target"), c, true);
  }

  {
    TCanvas* c =
      fHM->CreateCanvas("rich_urqmd_points_xy_gamma_nontarget", "rich_urqmd_points_xy_gamma_nontarget", 800, 800);
    fHM->H2("fh_points_xy_gamma_nontarget")->Scale(1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_points_xy_gamma_nontarget"), c, true);
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_hits_per_event", "rich_urqmd_nof_hits_per_event", 800, 800);
    fHM->H1("fh_nof_hits_per_event")->Scale(1. / fHM->H1("fh_nof_hits_per_event")->Integral());
    DrawH1andFitGauss(fHM->H1("fh_nof_hits_per_event"));
    cout << "Mean number of hits per event = " << fHM->H1("fh_nof_hits_per_event")->GetMean() << endl;
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_hitrate_xy", "rich_urqmd_hitrate_xy", 800, 800);
    fHM->H2("fh_hitrate_xy")->Scale(1e7 / (fEventNum * 64.));
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_hitrate_xy"), c, true);
    //DrawH2(fHM->H2("fh_hitrate_xy"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_urqmd_proj_xy", "rich_urqmd_proj_xy", 800, 800);
    fHM->H2("fh_proj_xy")->Scale(1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_proj_xy"), c);
  }

  {
    fHM->CreateCanvas("rich_urqmd_nof_proj_per_event", "rich_urqmd_nof_proj_per_event", 800, 800);
    fHM->H1("fh_nof_proj_per_event")->Scale(1. / fEventNum);
    DrawH1andFitGauss(fHM->H1("fh_nof_proj_per_event"));
    cout << "Number of track projections per event = " << fHM->H1("fh_nof_proj_per_event")->GetMean() << endl;
  }
}

void CbmRichUrqmdTest::Finish()
{
  DrawHist();
  fHM->SaveCanvasToImage(fOutputDir);
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
}

ClassImp(CbmRichUrqmdTest)
