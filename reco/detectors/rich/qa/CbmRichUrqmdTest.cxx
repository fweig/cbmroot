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

  fHM->Create1<TH1D>("fh_vertex_z", "fh_vertex_z;Z [cm];# vertices/ev.", 400, -50., 350);
  fHM->Create1<TH1D>("fh_vertex_z_sts", "fh_vertex_z_sts;Z [cm];# vertices/ev.", 320, -50., 110.);
  fHM->Create2<TH2D>("fh_vertex_xy", "fh_vertex_xy;X [cm];Y [cm];# vertices/ev.", 100, -200., 200., 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_zy", "fh_vertex_zy;Z [cm];Y [cm];# vertices/ev.", 400, -50., 350, 100, -200., 200.);
  fHM->Create2<TH2D>("fh_vertex_zx", "fh_vertex_zx;Z [cm];X [cm];# vertices/ev.", 400, -50., 350, 100, -200., 200.);

  vector<string> vertexZTypes {"z60_140", "z140_330", "z140_190"};
  for (const string& t : vertexZTypes) {
    fHM->Create2<TH2D>("fh_vertex_xy_" + t, "fh_vertex_xy_" + t + ";X [cm];Y [cm];# vertices/ev.", 100, -200., 200.,
                       100, -200., 200.);
  }

  for (auto pair : fVertexZStsSlices) {
    string name = "fh_vertex_xy_z" + to_string(pair.first) + "_" + to_string(pair.second);
    fHM->Create2<TH2D>(name, name + ";x [cm];y [cm];# vertices/ev.", 100, -100., 100., 100, -100., 100.);
  }

  vector<string> nofRingsTypes {"1hit", "7hits", "prim_1hit", "prim_7hits", "target_1hit", "target_7hits"};
  for (const string& t : nofRingsTypes) {
    double nofBins = (t == "1hit" || t == "7hits") ? 30 : 100;
    fHM->Create1<TH1D>("fh_nof_rings_" + t, "fh_nof_rings_" + t + ";# particles/ev.;Yield", nofBins, -.5,
                       nofBins - 0.5);
  }

  vector<string> momP {"fh_secel_mom", "fh_gamma_target_mom", "fh_gamma_nontarget_mom",
                       "fh_pi_mom",    "fh_kaon_mom",         "fh_mu_mom"};
  for (const string& t : momP) {
    fHM->Create1<TH1D>(t, t + ";P [GeV/c];Number per event", 100, 0., 20);
  }

  fHM->Create1<TH1D>("fh_nof_points_per_event_src", "fh_nof_points_per_event_src;Particle;# MC points/ev.", 7, .5, 7.5);
  fHM->Create1<TH1D>("fh_nof_hits_per_event", "fh_nof_hits_per_event;# hits/ev.;Yield", 200, 0, 2000);
  fHM->Create1<TH1D>("fh_nof_points_per_event", "fh_nof_points_per_event;# points/ev.;Yield", 200, 0, 10000);
  fHM->Create1<TH1D>("fh_nof_hits_per_pmt", "fh_nof_hits_per_pmt;# hits/PMT;% of total", 65, -0.5, 64.5);

  vector<double> xPmtBins = CbmRichUtil::GetPmtHistXbins();
  vector<double> yPmtBins = CbmRichUtil::GetPmtHistYbins();

  // before drawing this histogram must be normalized by 1/64
  fHM->Create2<TH2D>("fh_hitrate_xy", "fh_hitrate_xy;X [cm];Y [cm];# hits/pixel/s", xPmtBins, yPmtBins);

  fHM->Create2<TH2D>("fh_hits_xy", "fh_hits_xy;X [cm];Y [cm];# hits/PMT/ev.", xPmtBins, yPmtBins);

  vector<string> pointXYTypes {"", "_pions", "_gamma_target", "_gamma_nontarget"};
  for (const string& t : pointXYTypes) {
    fHM->Create2<TH2D>("fh_points_xy" + t, "fh_points_xy" + t + ";X [cm];Y [cm];# MC points/PMT/ev.", xPmtBins,
                       yPmtBins);
  }

  vector<string> skippedPmtTypes {"10", "20", "30"};
  for (const string& t : skippedPmtTypes) {
    fHM->Create2<TH2D>("fh_skipped_pmt_xy_" + t,
                       "fh_skipped_pmt_xy_" + t + ";X [cm];Y [cm];# skipped PMTs (>" + t + " hits) [%]", xPmtBins,
                       yPmtBins);
  }

  fHM->Create1<TH1D>("fh_nof_proj_per_event", "fh_nof_proj_per_event;# tracks/ev.;Yield", 600, -.5, 599.5);
  fHM->Create2<TH2D>("fh_proj_xy", "fh_proj_xy;X [cm];Y [cm];# tracks/cm^{2}/ev.", 240, -120, 120, 420, -210, 210);
}

void CbmRichUrqmdTest::NofRings()
{
  int fileId     = 0;
  int nofRings   = fRichRings->GetEntriesFast();
  int nRings1hit = 0, nRings7hits = 0;
  int nRingsPrim1hit = 0, nRingsPrim7hits = 0;
  int nRingsTarget1hit = 0, nRingsTarget7hits = 0;
  for (int iR = 0; iR < nofRings; iR++) {
    const CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
    if (ring == nullptr) continue;
    const CbmTrackMatchNew* ringMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (ringMatch == nullptr) continue;

    int mcEventId             = ringMatch->GetMatchedLink().GetEntry();
    int mcTrackId             = ringMatch->GetMatchedLink().GetIndex();
    const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, mcEventId, mcTrackId));
    if (mcTrack == nullptr) continue;
    int motherId = mcTrack->GetMotherId();
    int pdgAbs   = std::abs(mcTrack->GetPdgCode());
    double mom   = mcTrack->GetP();
    TVector3 vert;
    mcTrack->GetStartVertex(vert);
    double dZ = vert.Z();

    if (motherId == -1 && pdgAbs == 11) continue;  // do not calculate embedded electrons

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
        if (mother != nullptr) motherPdg = mother->GetPdgCode();
        if (motherId != -1 && pdgAbs == 11 && motherPdg != 22) fHM->H1("fh_secel_mom")->Fill(mom);

        if (motherId != -1 && pdgAbs == 11 && motherPdg == 22) {
          if (dZ < 0.1) { fHM->H1("fh_gamma_target_mom")->Fill(mom); }
          else {
            fHM->H1("fh_gamma_nontarget_mom")->Fill(mom);
          }
        }
      }
      if (pdgAbs == 211) fHM->H1("fh_pi_mom")->Fill(mom);
      if (pdgAbs == 321) fHM->H1("fh_kaon_mom")->Fill(mom);
      if (pdgAbs == 13) fHM->H1("fh_mu_mom")->Fill(mom);
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
  map<int, int> digisPerPmtMap;
  for (int iH = 0; iH < nofHits; iH++) {
    const CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    if (hit == nullptr) continue;
    fHM->H2("fh_hits_xy")->Fill(hit->GetX(), hit->GetY());
    fHM->H2("fh_hitrate_xy")->Fill(hit->GetX(), hit->GetY());

    int digiInd                 = hit->GetRefId();
    const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(digiInd);
    if (richDigi == nullptr) continue;
    CbmRichPixelData* pixelData = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(richDigi->GetAddress());
    if (pixelData == nullptr) continue;
    int pmtId = pixelData->fPmtId;
    digisPerPmtMap[pmtId]++;
  }

  for (auto const& it : digisPerPmtMap) {
    int pmtId               = it.first;
    int nofDigis            = it.second;
    CbmRichPmtData* pmtData = CbmRichDigiMapManager::GetInstance().GetPmtDataById(pmtId);
    TVector3 inPos(pmtData->fX, pmtData->fY, pmtData->fZ);
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    if (nofDigis > 10) fHM->H2("fh_skipped_pmt_xy_10")->Fill(outPos.X(), outPos.Y());
    if (nofDigis > 20) fHM->H2("fh_skipped_pmt_xy_20")->Fill(outPos.X(), outPos.Y());
    if (nofDigis > 30) fHM->H2("fh_skipped_pmt_xy_30")->Fill(outPos.X(), outPos.Y());
  }

  vector<int> allPmtIds = CbmRichDigiMapManager::GetInstance().GetPmtIds();
  for (int pmtId : allPmtIds) {
    fHM->H1("fh_nof_hits_per_pmt")->Fill(digisPerPmtMap[pmtId]);
  }

  int nofEvents = fEventList->GetNofEvents();
  for (int iE = 0; iE < nofEvents; iE++) {
    int fileId    = fEventList->GetFileIdByIndex(iE);
    int eventId   = fEventList->GetEventIdByIndex(iE);
    int nofPoints = fRichPoints->Size(fileId, eventId);
    fHM->H1("fh_nof_points_per_event")->Fill(nofPoints);
    for (int i = 0; i < nofPoints; i++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, i));
      if (point == nullptr) continue;
      fHM->H1("fh_nof_points_per_event_src")->Fill(1);

      int mcPhotonTrackId = point->GetTrackID();
      if (mcPhotonTrackId < 0) continue;
      const CbmMCTrack* mcPhotonTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, mcPhotonTrackId));
      if (mcPhotonTrack == nullptr) continue;
      int motherPhotonId = mcPhotonTrack->GetMotherId();
      if (motherPhotonId < 0) continue;
      const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherPhotonId));
      if (mcTrack == nullptr) continue;
      int motherId = mcTrack->GetMotherId();

      int pdgAbs = std::abs(mcTrack->GetPdgCode());
      TVector3 vert;
      mcTrack->GetStartVertex(vert);
      double dZ = vert.Z();

      if (motherId == -1 && pdgAbs == 11) continue;  // do not calculate embedded electrons

      if (motherId != -1) {
        int motherPdg            = -1;
        const CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherId));
        if (mother != nullptr) motherPdg = mother->GetPdgCode();
        if (motherId != -1 && pdgAbs == 11 && motherPdg != 22) fHM->H1("fh_nof_points_per_event_src")->Fill(2);

        if (motherId != -1 && pdgAbs == 11 && motherPdg == 22) {
          if (dZ < 0.1) { fHM->H1("fh_nof_points_per_event_src")->Fill(3); }
          else {
            fHM->H1("fh_nof_points_per_event_src")->Fill(4);
          }
        }
      }
      if (pdgAbs == 211) fHM->H1("fh_nof_points_per_event_src")->Fill(5);
      if (pdgAbs == 321) fHM->H1("fh_nof_points_per_event_src")->Fill(6);
      if (pdgAbs == 13) fHM->H1("fh_nof_points_per_event_src")->Fill(7);
    }
  }
}

void CbmRichUrqmdTest::PmtXYSource()
{
  int nofEvents = fEventList->GetNofEvents();
  for (int iE = 0; iE < nofEvents; iE++) {
    int fileId    = fEventList->GetFileIdByIndex(iE);
    int eventId   = fEventList->GetEventIdByIndex(iE);
    int nofPoints = fRichPoints->Size(fileId, eventId);
    for (int i = 0; i < nofPoints; i++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, i));
      if (point == nullptr) continue;

      int iMCTrack            = point->GetTrackID();
      const CbmMCTrack* track = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iMCTrack));
      if (track == nullptr) continue;

      int iMother = track->GetMotherId();
      if (iMother == -1) continue;

      const CbmMCTrack* track2 = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iMother));
      if (track2 == nullptr) continue;
      int pdgAbs   = std::abs(track2->GetPdgCode());
      int motherId = track2->GetMotherId();
      TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
      TVector3 outPos;
      CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

      fHM->H2("fh_points_xy")->Fill(outPos.X(), outPos.Y());
      if (motherId != -1) {
        int motherPdg            = -1;
        const CbmMCTrack* mother = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, motherId));
        if (mother != nullptr) motherPdg = mother->GetPdgCode();
        TVector3 vert;
        track2->GetStartVertex(vert);
        if (motherId != -1 && pdgAbs == 11 && motherPdg == 22) {
          if (vert.Z() < 0.1) { fHM->H2("fh_points_xy_gamma_target")->Fill(outPos.X(), outPos.Y()); }
          else {
            fHM->H2("fh_points_xy_gamma_nontarget")->Fill(outPos.X(), outPos.Y());
          }
        }
      }
      if (pdgAbs == 211) fHM->H2("fh_points_xy_pions")->Fill(outPos.X(), outPos.Y());
    }
  }
}

void CbmRichUrqmdTest::NofProjections()
{
  if (fRichProjections == nullptr) return;
  int nofProj     = fRichProjections->GetEntriesFast();
  int nofGoodProj = 0;
  for (int i = 0; i < nofProj; i++) {
    FairTrackParam* proj = (FairTrackParam*) fRichProjections->At(i);
    if (proj == nullptr) continue;
    fHM->H2("fh_proj_xy")->Fill(proj->GetX(), proj->GetY());
    if (proj->GetX() != 0 && proj->GetY() != 0) nofGoodProj++;
  }
  fHM->H1("fh_nof_proj_per_event")->Fill(nofGoodProj);
}

void CbmRichUrqmdTest::Vertex()
{
  int nofEvents = fEventList->GetNofEvents();
  for (int iE = 0; iE < nofEvents; iE++) {
    int fileId  = fEventList->GetFileIdByIndex(iE);
    int eventId = fEventList->GetEventIdByIndex(iE);

    int nMcTracks = fMcTracks->Size(fileId, eventId);
    for (int i = 0; i < nMcTracks; i++) {
      //At least one hit in RICH
      pair<int, int> val = std::make_pair(eventId, i);
      if (fNofHitsInRingMap[val] < 1) continue;
      const CbmMCTrack* mctrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, i));
      TVector3 v;
      mctrack->GetStartVertex(v);
      fHM->H1("fh_vertex_z")->Fill(v.Z());
      fHM->H1("fh_vertex_z_sts")->Fill(v.Z());
      fHM->H2("fh_vertex_xy")->Fill(v.X(), v.Y());
      fHM->H2("fh_vertex_zy")->Fill(v.Z(), v.Y());
      fHM->H2("fh_vertex_zx")->Fill(v.Z(), v.X());
      if (v.Z() >= 60 && v.Z() <= 140) fHM->H2("fh_vertex_xy_z60_140")->Fill(v.X(), v.Y());
      if (v.Z() >= 140 && v.Z() <= 330) fHM->H2("fh_vertex_xy_z140_330")->Fill(v.X(), v.Y());
      if (v.Z() >= 140 && v.Z() <= 190) fHM->H2("fh_vertex_xy_z140_190")->Fill(v.X(), v.Y());

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
    fHM->Scale("fh_vertex_z", 1. / fEventNum);
    fHM->CreateCanvas("richurqmd_vertex_z", "richurqmd_vertex_z", 1000, 1000);
    DrawH1(fHM->H1("fh_vertex_z"), kLinear, kLog, "hist");
  }

  {
    fHM->Scale("fh_vertex_z_sts", 1. / fEventNum);
    fHM->CreateCanvas("richurqmd_vertex_z_sts", "richurqmd_vertex_z_sts", 1000, 1000);
    DrawH1(fHM->H1("fh_vertex_z_sts"), kLinear, kLog, "hist");
  }


  {
    fHM->Scale("fh_vertex_xy", 1. / fEventNum);
    fHM->Scale("fh_vertex_zy", 1. / fEventNum);
    fHM->Scale("fh_vertex_zx", 1. / fEventNum);
    TCanvas* c = fHM->CreateCanvas("richurqmd_vertex_xyz", "richurqmd_vertex_xyz", 1800, 600);
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

    TCanvas* c = fHM->CreateCanvas("richurqmd_vertex_sts_xyz", "richurqmd_vertex_sts_xyz", 1600, 1200);
    c->Divide(4, 3);
    int i = 1;
    for (auto pair : fVertexZStsSlices) {
      string name = "fh_vertex_xy_z" + to_string(pair.first) + "_" + to_string(pair.second);
      fHM->Scale(name, 1. / fEventNum);
      c->cd(i++);
      DrawH2(fHM->H2(name), kLinear, kLinear, kLog);
      DrawTextOnPad(to_string(pair.first) + " cm < Z < " + to_string(pair.second) + " cm", 0.3, 0.9, 0.7, 0.98);
    }

    gStyle->SetOptTitle(0);
  }

  vector<string> vertexZTypes {"z60_140", "z140_330", "z140_190"};
  for (const string& t : vertexZTypes) {
    string name = "richurqmd_vertex_xy_" + t;
    fHM->Scale("fh_vertex_xy_" + t, 1. / fEventNum);
    fHM->CreateCanvas(name, name, 1000, 1000);
    DrawH2(fHM->H2("fh_vertex_xy_" + t));
  }

  {
    fHM->Scale("fh_nof_points_per_event_src", 1. / fEventNum);
    fHM->CreateCanvas("richurqmd_nof_points_per_event_src", "richurqmd_nof_points_per_event_src", 1000, 1000);
    //gStyle->SetPaintTextFormat("4.1f");
    string labels[7] = {"all",
                        "e^{#pm}_{sec} other",
                        "e^{#pm}_{target} from #gamma",
                        "e^{#pm}_{not target} from #gamma",
                        "#pi^{#pm}",
                        "K^{#pm}",
                        "#mu^{#pm}"};
    for (int i = 1; i <= 7; i++) {
      fHM->H1("fh_nof_points_per_event_src")->GetXaxis()->SetBinLabel(i, labels[i - 1].c_str());
    }
    fHM->H1("fh_nof_points_per_event_src")->GetXaxis()->SetLabelSize(0.05);
    //fHM->H1("fh_nof_points_per_event_src")->SetMarkerSize(0.15);
    DrawH1(fHM->H1("fh_nof_points_per_event_src"), kLinear, kLog, "hist text0");
  }

  {
    vector<string> nofRingsTypes {"", "_prim", "_target"};
    for (const string& t : nofRingsTypes) {
      string cName  = "richurqmd_nof_rings" + t;
      string h1Name = "fh_nof_rings" + t + "_1hit";
      string h7Name = "fh_nof_rings" + t + "_7hits";
      fHM->CreateCanvas(cName, cName, 1000, 1000);
      fHM->NormalizeToIntegral(h1Name);
      fHM->NormalizeToIntegral(h7Name);
      stringstream ss1, ss2;
      ss1 << "At least 1 hit (" << fHM->H1(h1Name)->GetMean() << ")";
      ss2 << "At least 7 hits (" << fHM->H1(h7Name)->GetMean() << ")";
      DrawH1({fHM->H1(h1Name), fHM->H1(h7Name)}, {ss1.str(), ss2.str()}, kLinear, kLinear, true, 0.4, 0.85, 0.99, 0.99,
             "hist");
    }
  }

  {
    fHM->CreateCanvas("richurqmd_sources_mom", "richurqmd_sources_mom", 1000, 1000);
    fHM->Scale("fh_gamma_target_mom", 1. / fEventNum);
    fHM->Scale("fh_gamma_nontarget_mom", 1. / fEventNum);
    fHM->Scale("fh_secel_mom", 1. / fEventNum);
    fHM->Scale("fh_pi_mom", 1. / fEventNum);
    fHM->Scale("fh_kaon_mom", 1. / fEventNum);
    fHM->Scale("fh_mu_mom", 1. / fEventNum);
    stringstream ss1, ss2, ss3, ss4, ss5, ss6;
    ss1 << "e^{#pm}_{target} from #gamma (" << fHM->H1("fh_gamma_target_mom")->GetEntries() / fEventNum << ")";
    ss2 << "e^{#pm}_{not target} from #gamma (" << fHM->H1("fh_gamma_nontarget_mom")->GetEntries() / fEventNum << ")";
    ss3 << "e^{#pm}_{sec} other (" << fHM->H1("fh_secel_mom")->GetEntries() / fEventNum << ")";
    ss4 << "#pi^{#pm} (" << fHM->H1("fh_pi_mom")->GetEntries() / fEventNum << ")";
    ss5 << "K^{#pm} (" << fHM->H1("fh_kaon_mom")->GetEntries() / fEventNum << ")";
    ss6 << "#mu^{#pm} (" << fHM->H1("fh_mu_mom")->GetEntries() / fEventNum << ")";
    DrawH1(fHM->H1Vector({"fh_gamma_target_mom", "fh_gamma_nontarget_mom", "fh_secel_mom", "fh_pi_mom", "fh_kaon_mom",
                          "fh_mu_mom"}),
           {ss1.str(), ss2.str(), ss3.str(), ss4.str(), ss5.str(), ss6.str()}, kLinear, kLog, true, 0.5, 0.7, 0.99,
           0.99, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richurqmd_hits_xy", "richurqmd_hits_xy", 1000, 1000);
    TH2* clone = fHM->H2Clone("fh_hits_xy");
    clone->Scale(1. / (fEventNum));
    CbmRichDraw::DrawPmtH2(clone, c, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richurqmd_occupancy_xy", "richurqmd_occupancy_xy", 1000, 1000);
    TH2* clone = fHM->H2Clone("fh_hits_xy");
    clone->GetZaxis()->SetTitle("Occupancy:# hits/PMT/ev./64 [%]");
    clone->Scale(100. / (fEventNum * 64.));
    CbmRichDraw::DrawPmtH2(clone, c, true);
  }

  {
    vector<string> skippedPmtTypes {"10", "20", "30"};
    for (const string& t : skippedPmtTypes) {
      string name = "richurqmd_skipped_pmt_xy_" + t;
      TCanvas* c  = fHM->CreateCanvas(name, name, 1000, 1000);
      fHM->Scale("fh_skipped_pmt_xy_" + t, 100. / fEventNum);
      CbmRichDraw::DrawPmtH2(fHM->H2("fh_skipped_pmt_xy_" + t), c, true);
    }
  }

  {
    fHM->CreateCanvas("richurqmd_nof_hits_per_pmt", "richurqmd_nof_hits_per_pmt", 1000, 1000);
    fHM->NormalizeToIntegral("fh_nof_hits_per_pmt");
    DrawH1(fHM->H1("fh_nof_hits_per_pmt"), kLinear, kLog, "hist");
    fHM->H1("fh_nof_hits_per_pmt")->SetStats(true);
  }

  {
    vector<string> pointXYTypes {"", "_pions", "_gamma_target", "_gamma_nontarget"};
    for (const string& t : pointXYTypes) {
      string name = "richurqmd_points_xy" + t;
      TCanvas* c  = fHM->CreateCanvas(name, name, 1000, 1000);
      fHM->Scale("fh_points_xy" + t, 1. / fEventNum);
      CbmRichDraw::DrawPmtH2(fHM->H2("fh_points_xy" + t), c, true);
    }
  }

  {
    fHM->CreateCanvas("richurqmd_nof_hits_per_event", "richurqmd_nof_hits_per_event", 1000, 1000);
    fHM->NormalizeToIntegral("fh_nof_hits_per_event");
    DrawH1(fHM->H1("fh_nof_hits_per_event"), kLinear, kLinear, "hist");
    fHM->H1("fh_nof_hits_per_event")->SetStats(true);
    cout << "Mean number of hits per event = " << fHM->H1("fh_nof_hits_per_event")->GetMean() << endl;
  }

  {
    fHM->CreateCanvas("richurqmd_nof_points_per_event", "richurqmd_nof_points_per_event", 1000, 1000);
    fHM->NormalizeToIntegral("fh_nof_points_per_event");
    DrawH1(fHM->H1("fh_nof_points_per_event"), kLinear, kLinear, "hist");
    fHM->H1("fh_nof_points_per_event")->SetStats(true);
    cout << "Mean number of points per event = " << fHM->H1("fh_nof_points_per_event")->GetMean() << endl;
  }

  {
    TCanvas* c = fHM->CreateCanvas("richurqmd_hitrate_xy", "richurqmd_hitrate_xy", 1000, 1000);
    fHM->Scale("fh_hitrate_xy", 1e7 / (fEventNum * 64.));
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_hitrate_xy"), c, true);
    //DrawH2(fHM->H2("fh_hitrate_xy"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("richurqmd_proj_xy", "richurqmd_proj_xy", 1000, 1000);
    fHM->Scale("fh_proj_xy", 1. / fEventNum);
    CbmRichDraw::DrawPmtH2(fHM->H2("fh_proj_xy"), c);
  }

  {
    fHM->CreateCanvas("richurqmd_nof_proj_per_event", "richurqmd_nof_proj_per_event", 1000, 1000);
    fHM->Scale("fh_nof_proj_per_event", 1. / fEventNum);
    DrawH1(fHM->H1("fh_nof_proj_per_event"), kLinear, kLinear, "hist");
    fHM->H1("fh_nof_proj_per_event")->SetStats(true);
    cout << "Number of track projections per event = " << fHM->H1("fh_nof_proj_per_event")->GetMean() << endl;
  }
}

void CbmRichUrqmdTest::Finish()
{
  DrawHist();
  fHM->SaveCanvasToImage(fOutputDir);
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != nullptr) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
}

ClassImp(CbmRichUrqmdTest)
