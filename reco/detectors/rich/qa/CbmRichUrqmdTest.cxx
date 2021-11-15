/* Copyright (C) 2012-2020 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

#include "CbmRichUrqmdTest.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmRichDetectorData.h"
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
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

#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace Cbm;

CbmRichUrqmdTest::CbmRichUrqmdTest() : FairTask("CbmRichUrqmdTest") {}

CbmRichUrqmdTest::~CbmRichUrqmdTest() {}

InitStatus CbmRichUrqmdTest::Init()
{
  cout << "CbmRichUrqmdTest::Init" << endl;
  fMcTracks        = InitOrFatalMc("MCTrack", "CbmRichUrqmdTest::Init");
  fRichPoints      = InitOrFatalMc("RichPoint", "CbmRichUrqmdTest::Init");
  fRichHits        = GetOrFatal<TClonesArray>("RichHit", "CbmRichUrqmdTest::Init");
  fRichRings       = GetOrFatal<TClonesArray>("RichRing", "CbmRichUrqmdTest::Init");
  fRichRingMatches = GetOrFatal<TClonesArray>("RichRingMatch", "CbmRichUrqmdTest::Init");
  fRichProjections = GetOrFatal<TClonesArray>("RichProjection", "CbmRichUrqmdTest::Init");
  fEventList       = GetOrFatal<CbmMCEventList>("MCEventList.", "CbmRichUrqmdTest::Init");

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  fVertexZStsSlices = {make_pair(0., 5.),   make_pair(5., 15.),  make_pair(15., 25.), make_pair(25., 35.),
                       make_pair(35., 45.), make_pair(45., 55.), make_pair(55., 65.), make_pair(65., 75.),
                       make_pair(75., 85.), make_pair(85., 95.), make_pair(95., 105.)};

  InitHistograms();


  return kSUCCESS;
}

void CbmRichUrqmdTest::Exec(Option_t* /*option*/)
{
  fEventNum++;

  cout << "CbmRichUrqmdTest, event No. " << fEventNum << endl;

  fNofHitsInRingMap = CbmRichUtil::CreateNofHitsInRingMap(fRichHits, fRichPoints, fMcTracks, fDigiMan);
  NofRings();
  NofHitsAndPoints();
  NofProjections();
  Vertex();
  PmtXYSource();
}

void CbmRichUrqmdTest::InitHistograms()
{
  fHM = new CbmHistManager();

  fHM->Create1<TH1D>("fh_vertex_z", "fh_vertex_z;z [cm];# vertices/ev.", 350, -1., 350);
  fHM->Create1<TH1D>("fh_vertex_z_sts", "fh_vertex_z_sts;z [cm];# vertices/ev.", 222, -1., 110.);
  fHM->Create2<TH2D>("fh_vertex_xy", "fh_vertex_xy;x [cm];y [cm];# vertices/ev.", 100, -200., 200., 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_zy", "fh_vertex_zy;z [cm];y [cm];# vertices/ev.", 350, -1., 350, 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_zx", "fh_vertex_zx;z [cm];x [cm];# vertices/ev.", 350, -1., 350, 100, -200., 200.);


  fHM->Create2<TH2D>("fh_vertex_xy_z100_180", "fh_vertex_xy_z100_180;x [cm];y [cm];# vertices/ev.", 100, -200., 200.,
                     100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_xy_z180_370", "fh_vertex_xy_z180_370;x [cm];y [cm];# vertices/ev.", 100, -200., 200.,
                     100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_xy_z180_230", "fh_vertex_xy_z180_230;x [cm];y [cm];# vertices/ev.", 100, -200., 200.,
                     100, -200., 200.);

  for (auto pair : fVertexZStsSlices) {
    string name = "fh_vertex_xy_z" + to_string(pair.first) + "_" + to_string(pair.second);
    fHM->Create2<TH2D>(name, name + ";x [cm];y [cm];# vertices/ev.", 100, -100., 100., 100, -100., 100.);
  }

  fHM->Create1<TH1D>("fh_nof_rings_1hit", "fh_nof_rings_1hit;# detected particles/ev.;Yield", 250, -.5, 249.5);
  fHM->Create1<TH1D>("fh_nof_rings_7hits", "fh_nof_rings_7hits;# detected particles/ev.;Yield", 250, -.5, 249.5);
  fHM->Create1<TH1D>("fh_nof_rings_prim_1hit", "fh_nof_rings_prim_1hit;# detected particles/ev.;Yield", 50, -.5, 69.5);
  fHM->Create1<TH1D>("fh_nof_rings_prim_7hits", "fh_nof_rings_prim_7hits;# detected particles/ev.;Yield", 50, -.5,
                     69.5);
  fHM->Create1<TH1D>("fh_nof_rings_target_1hit", "fh_nof_rings_target_1hit;# detected particles/ev.;Yield", 60, -.5,
                     79.5);
  fHM->Create1<TH1D>("fh_nof_rings_target_7hits", "fh_nof_rings_target_7hits;# detected particles/ev.;Yield", 60, -.5,
                     79.5);

  fHM->Create1<TH1D>("fh_secel_mom", "fh_secel_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_gamma_target_mom", "fh_gamma_target_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_gamma_nontarget_mom", "fh_gamma_nontarget_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_pi_mom", "fh_pi_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_kaon_mom", "fh_kaon_mom;p [GeV/c];Number per event", 100, 0., 20);
  fHM->Create1<TH1D>("fh_mu_mom", "fh_mu_mom;p [GeV/c];Number per event", 100, 0., 20);

  fHM->Create1<TH1D>("fh_nof_points_per_event", "fh_nof_points_per_event;Particle;# MC points/ev.", 7, .5, 7.5);
  fHM->Create1<TH1D>("fh_nof_hits_per_event", "fh_nof_hits_per_event;# hits per event;Yield", 200, 0, 2000);
  fHM->Create1<TH1D>("fh_nof_hits_per_pmt", "fh_nof_hits_per_pmt;# hits per PMT;% of total", 65, -0.5, 64.5);

  vector<double> xPmtBins = CbmRichUtil::GetPmtHistXbins();
  vector<double> yPmtBins = CbmRichUtil::GetPmtHistYbins();

  // before drawing must be normalized by 1/64
  fHM->Create2<TH2D>("fh_hitrate_xy", "fh_hitrate_xy;X [cm];Y [cm];# hits/pixel/s", xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_hits_xy", "fh_hits_xy;X [cm];Y [cm];# hits/PMT/ev.", xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_points_xy", "fh_points_xy;X [cm];Y [cm];# MC points/PMT/ev.", xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_points_xy_pions", "fh_points_xy_pions;X [cm];Y [cm];# MC points/PMT/ev.", xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_points_xy_gamma_target", "fh_points_xy_gamma_target;X [cm];Y [cm];# MC points/PMT/ev.",
                     xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_points_xy_gamma_nontarget", "fh_points_xy_gamma_nontarget;X [cm];Y [cm];# MC points/PMT/ev.",
                     xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_skipped_pmt_10_xy", "fh_skipped_pmt_10_xy;X [cm];Y [cm];# skipped PMTs (>10 hits) [%]",
                     xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_skipped_pmt_20_xy", "fh_skipped_pmt_20_xy;X [cm];Y [cm];# skipped PMTs (>20 hits) [%]",
                     xPmtBins, yPmtBins);
  fHM->Create2<TH2D>("fh_skipped_pmt_30_xy", "fh_skipped_pmt_30_xy;X [cm];Y [cm];# skipped PMTs (>30 hits) [%]",
                     xPmtBins, yPmtBins);

  fHM->Create1<TH1D>("fh_nof_proj_per_event", "fh_nof_proj_per_event;# tracks/ev.;Yield", 50, 0, 1000);
  fHM->Create2<TH2D>("fh_proj_xy", "fh_proj_xy;X [cm];Y [cm];# tracks/cm^{2}/ev.", 240, -120, 120, 420, -210, 210);
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

      for (auto pair : fVertexZStsSlices) {
        string name = "fh_vertex_xy_z" + to_string(pair.first) + "_" + to_string(pair.second);
        if (v.Z() > pair.first && v.Z() <= pair.second) { fHM->H2(name)->Fill(v.X(), v.Y()); }
      }
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

    TCanvas* c = fHM->CreateCanvas("rich_urqmd_vertex_sts_xyz", "rich_urqmd_vertex_sts_xyz", 1600, 1200);
    c->Divide(4, 3);
    int i = 1;
    for (auto pair : fVertexZStsSlices) {
      string name = "fh_vertex_xy_z" + to_string(pair.first) + "_" + to_string(pair.second);
      fHM->H2(name)->Scale(1. / fEventNum);
      c->cd(i++);
      DrawH2(fHM->H2(name), kLinear, kLinear, kLog);
      DrawTextOnPad(to_string(pair.first) + " cm < Z < " + to_string(pair.second) + " cm", 0.3, 0.9, 0.7, 0.98);
    }

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
           {ss1.str(), ss2.str(), ss3.str(), ss4.str(), ss5.str(), ss6.str()}, kLinear, kLog, true, 0.5, 0.7, 0.99,
           0.99, "hist");
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
    DrawH1(fHM->H1("fh_nof_hits_per_event"), kLinear, kLinear, "hist");
    fHM->H1("fh_nof_hits_per_event")->SetStats(true);
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
