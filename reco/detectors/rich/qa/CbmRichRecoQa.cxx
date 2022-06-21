/* Copyright (C) 2016-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Andrey Lebedev [committer] */

#include "CbmRichRecoQa.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"
#include "elid/CbmLitGlobalElectronId.h"

#include "FairMCPoint.h"
#include "FairTrackParam.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMCProcess.h"
#include "TMarker.h"
#include "TStyle.h"
#include <TFile.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace Cbm;

CbmRichRecoQa::CbmRichRecoQa() : FairTask("CbmRichRecoQa") {}


InitStatus CbmRichRecoQa::Init()
{
  fMcTracks        = InitOrFatalMc("MCTrack", "CbmRichRecoQa::Init");
  fRichPoints      = InitOrFatalMc("RichPoint", "CbmRichRecoQa::Init");
  fRichHits        = GetOrFatal<TClonesArray>("RichHit", "CbmRichRecoQa::Init");
  fRichRings       = GetOrFatal<TClonesArray>("RichRing", "CbmRichRecoQa::Init");
  fRichRingMatches = GetOrFatal<TClonesArray>("RichRingMatch", "CbmRichRecoQa::Init");
  fRichProjections = GetOrFatal<TClonesArray>("RichProjection", "CbmRichRecoQa::Init");
  fGlobalTracks    = GetOrFatal<TClonesArray>("GlobalTrack", "CbmRichRecoQa::Init");
  fStsTracks       = GetOrFatal<TClonesArray>("StsTrack", "CbmRichRecoQa::Init");
  fStsTrackMatches = GetOrFatal<TClonesArray>("StsTrackMatch", "CbmRichRecoQa::Init");
  fEventList       = GetOrFatal<CbmMCEventList>("MCEventList.", "CbmRichUrqmdTest::Init");

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  InitHistograms();

  // CbmLitGlobalElectronId::GetInstance();

  return kSUCCESS;
}

void CbmRichRecoQa::InitHistograms()
{
  fHM = new CbmHistManager();

  // 2D histogramm XY
  double xMin = -120.;
  double xMax = 120.;
  int nBinsX  = 30;
  double yMin = -208;
  double yMax = 208.;
  int nBinsY  = 52;

  // 1D histogram X or Y
  int nBinsX1 = 60;
  int xMin1   = -120;
  int xMax1   = 120;
  int nBinsY1 = 25;
  int yMin1   = 100;
  int yMax1   = 200;

  vector<string> matchTypes {"Primel", "Pi", "PrimelPlus", "PrimelMinus"};
  for (const string& t : matchTypes) {
    fHM->Create2<TH2D>("fhRTDistVsMomTruematch" + t,
                       "fhRTDistVsMomTruematch" + t + ";P [GeV/c];Ring-track distance [cm];Yield", 20, 0., 10., 100, 0.,
                       5.);
    fHM->Create2<TH2D>("fhRTDistVsMomWrongmatch" + t,
                       "fhRTDistVsMomWrongmatch" + t + ";P [GeV/c];Ring-track distance [cm];Yield", 20, 0., 10., 100,
                       0., 5.);

    fHM->Create2<TH2D>("fhRTDistVsNofHitsTruematch" + t,
                       "fhRTDistVsNofHitsTruematch" + t + ";# hits/ring;Ring-track distance [cm];Yield", 40, -.5, 39.5,
                       100, 0., 5.);
    fHM->Create3<TH3D>("fhRTDistVsXYTruematch" + t,
                       "fhRTDistVsXYTruematch" + t + ";X [cm];Y [cm];Ring-track distance [cm]", nBinsX, xMin, xMax,
                       nBinsY, yMin, yMax, 100, 0., 5.);
    fHM->Create2<TH2D>("fhRTDistVsXTruematch" + t, "fhRTDistVsXTruematch" + t + ";X [cm];Ring-track distance [cm]",
                       nBinsX1, xMin1, xMax1, 100, 0., 5.);
    fHM->Create2<TH2D>("fhRTDistVsYTruematch" + t, "fhRTDistVsYTruematch" + t + ";Abs(Y) [cm];Ring-track distance [cm]",
                       nBinsY1, yMin1, yMax1, 100, 0., 5.);
  }

  // after electron identification
  fHM->Create2<TH2D>("fhRTDistVsMomTruematchElId",
                     "fhRTDistVsMomTruematchElId;P [GeV/c];Ring-track distance [cm];Yield", 20, 0., 10., 100, 0., 5.);

  fHM->Create1<TH1D>("fhMismatchSrc", "fhMismatchSrc;Global track category;% from MC", 13, -0.5, 12.5);

  vector<string> mismatchTypes {
    "Mc",          "Sts",           "StsAccRich",      "StsRich",      "StsRichTrue",    "StsNoRich",     "StsNoRichRF",
    "StsNoRichRM", "StsNoRichNoRF", "StsNoRichNoProj", "StsRichWrong", "StsRichWrongRF", "StsRichWrongRM"};
  for (const string& t : mismatchTypes) {
    fHM->Create1<TH1D>("fhMismatchSrcMom" + t, "fhMismatchSrcMom" + t + ";P [GeV/c];Yield", 40, 0., 10.);
  }
}

void CbmRichRecoQa::Exec(Option_t* /*option*/)
{
  fEventNum++;
  LOG(info) << "CbmRichRecoQa, event No. " << fEventNum;
  FillRichRingNofHits();
  FillRingTrackDistance();
  RingTrackMismatchSource();
}

void CbmRichRecoQa::FillRichRingNofHits()
{
  fNofHitsInRingMap.clear();
  int nofRichHits = fRichHits->GetEntriesFast();
  for (int iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iHit));
    if (nullptr == hit) continue;
    vector<pair<int, int>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, hit, fRichPoints, fMcTracks, fEventNum);

    for (size_t i = 0; i < motherIds.size(); i++) {
      fNofHitsInRingMap[motherIds[i]]++;
    }
  }
}

void CbmRichRecoQa::RingTrackMismatchSource()
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
      const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, i));
      if (IsMcPrimaryElectron(mcTrack)) {
        fHM->H1("fhMismatchSrc")->Fill(0);  // MC
        fHM->H1("fhMismatchSrcMomMc")->Fill(mcTrack->GetP());
      }
    }
  }

  int nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (int iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    int stsId = globalTrack->GetStsTrackIndex();
    if (stsId < 0) continue;
    const CbmTrackMatchNew* stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));
    if (stsTrackMatch == nullptr) continue;
    int stsMcTrackId          = stsTrackMatch->GetMatchedLink().GetIndex();
    int stsMcEventId          = stsTrackMatch->GetMatchedLink().GetEntry();
    const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(0, stsMcEventId, stsMcTrackId));
    if (!IsMcPrimaryElectron(mcTrack)) continue;
    double mom = mcTrack->GetP();
    //STS
    fHM->H1("fhMismatchSrc")->Fill(1);
    fHM->H1("fhMismatchSrcMomSts")->Fill(mom);

    pair<int, int> val = std::make_pair(stsMcEventId, stsMcTrackId);
    if (fNofHitsInRingMap[val] >= 7) {
      //Sts-AccRich
      fHM->H1("fhMismatchSrc")->Fill(2);
      fHM->H1("fhMismatchSrcMomStsAccRich")->Fill(mcTrack->GetP());
    }

    int richId = globalTrack->GetRichRingIndex();
    // No RICH segment
    if (richId < 0) {
      fHM->H1("fhMismatchSrc")->Fill(5);  //STS-noRICH
      fHM->H1("fhMismatchSrcMomStsNoRich")->Fill(mom);
      bool ringFound   = WasRingFound(stsMcTrackId);
      bool ringMatched = WasRingMatched(stsMcTrackId);
      bool hasProj     = HasRichProjection(stsId);
      if (ringFound) {
        fHM->H1("fhMismatchSrc")->Fill(6);  //STS-NoRich RF
        fHM->H1("fhMismatchSrcMomStsNoRichRF")->Fill(mom);
      }
      else {
        fHM->H1("fhMismatchSrc")->Fill(8);  //STS-NoRich NoRF
        fHM->H1("fhMismatchSrcMomStsNoRichNoRF")->Fill(mom);
      }

      if (ringMatched) {
        fHM->H1("fhMismatchSrc")->Fill(7);  //STS-NoRich RM
        fHM->H1("fhMismatchSrcMomStsNoRichRM")->Fill(mom);
      }

      if (!hasProj) {
        fHM->H1("fhMismatchSrc")->Fill(9);  //STS-NoRich NoProj
        fHM->H1("fhMismatchSrcMomStsNoRichNoProj")->Fill(mom);
      }
    }
    else {

      //STS-RICH
      fHM->H1("fhMismatchSrc")->Fill(3);
      fHM->H1("fhMismatchSrcMomStsRich")->Fill(mom);
      const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
      if (richRingMatch == nullptr) continue;
      int richMcTrackId       = richRingMatch->GetMatchedLink().GetIndex();
      const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
      if (nullptr == ring) continue;

      if (stsMcTrackId == richMcTrackId) {
        fHM->H1("fhMismatchSrc")->Fill(4);  //STS-RICH true
        fHM->H1("fhMismatchSrcMomStsRichTrue")->Fill(mom);
      }
      else {
        fHM->H1("fhMismatchSrc")->Fill(10);  //STS-RICH wrong
        fHM->H1("fhMismatchSrcMomStsRichWrong")->Fill(mom);
        if (WasRingFound(stsMcTrackId)) {
          fHM->H1("fhMismatchSrc")->Fill(11);  //STS-RICH wrong RF
          fHM->H1("fhMismatchSrcMomStsRichWrongRF")->Fill(mom);
        }

        if (WasRingFound(stsMcTrackId)) {
          fHM->H1("fhMismatchSrc")->Fill(12);  //STS-RICH wrong RM
          fHM->H1("fhMismatchSrcMomStsRichWrongRM")->Fill(mom);
        }
      }
    }
  }
}

bool CbmRichRecoQa::WasRingFound(int mcTrackId)
{
  int nofRings = fRichRings->GetEntriesFast();
  for (int iR = 0; iR < nofRings; iR++) {
    const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(iR));
    if (ring == nullptr) continue;
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId = richRingMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId == mcTrackId) return true;
  }
  return false;
}

bool CbmRichRecoQa::WasRingMatched(int mcTrackId)
{
  int nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (int iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    int richId = globalTrack->GetRichRingIndex();
    if (richId < 0) continue;
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId = richRingMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId == mcTrackId) { return true; }
  }

  return false;
}


bool CbmRichRecoQa::HasRichProjection(int stsTrackId)
{
  if (stsTrackId < 0) return false;
  const FairTrackParam* pTrack = static_cast<FairTrackParam*>(fRichProjections->At(stsTrackId));
  if (pTrack == nullptr) return false;

  if (pTrack->GetX() == 0. && pTrack->GetY() == 0.) return false;

  return true;
}

void CbmRichRecoQa::FillRingTrackDistance()
{
  int nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (int iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    int stsId  = globalTrack->GetStsTrackIndex();
    int richId = globalTrack->GetRichRingIndex();
    if (stsId < 0 || richId < 0) continue;

    const CbmTrackMatchNew* stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));
    if (stsTrackMatch == nullptr) continue;
    int stsMcTrackId = stsTrackMatch->GetMatchedLink().GetIndex();
    int stsMcEventId = stsTrackMatch->GetMatchedLink().GetEntry();

    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId       = richRingMatch->GetMatchedLink().GetIndex();
    const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
    if (nullptr == ring) continue;

    double rtDistance = CbmRichUtil::GetRingTrackDistance(iTrack);
    double xc         = ring->GetCenterX();
    double yc         = ring->GetCenterY();
    int nofHits       = ring->GetNofHits();

    const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(0, stsMcEventId, stsMcTrackId));
    if (mcTrack == nullptr) continue;
    double mom = mcTrack->GetP();
    int charge = mcTrack->GetCharge();

    bool isEl = IsMcPrimaryElectron(mcTrack);
    bool isPi = IsMcPion(mcTrack);

    if (!isEl && !isPi) continue;

    vector<string> matchTypes {"Primel", "Pi", "PrimelPlus", "PrimelMinus"};
    for (const string& t : matchTypes) {
      //cout << "t:" << t << " iT:" << iTrack << " dist:" << rtDistance << " if:" << (!(t == "Primel" && isEl)?"true":"false")<< endl;
      bool isGood = false;
      if (t == "Primel" && isEl) isGood = true;
      if (t == "Pi" && isPi) isGood = true;
      if (t == "PrimelPlus" && isEl && charge > 0) isGood = true;
      if (t == "PrimelMinus" && isEl && charge < 0) isGood = true;
      if (!isGood) continue;
      if (stsMcTrackId == richMcTrackId) {
        fHM->H2("fhRTDistVsMomTruematch" + t)->Fill(mom, rtDistance);
        fHM->H3("fhRTDistVsXYTruematch" + t)->Fill(xc, yc, rtDistance);
        fHM->H2("fhRTDistVsXTruematch" + t)->Fill(xc, rtDistance);
        fHM->H2("fhRTDistVsYTruematch" + t)->Fill(abs(yc), rtDistance);
        fHM->H2("fhRTDistVsNofHitsTruematch" + t)->Fill(nofHits, rtDistance);

        if (t == "Primel" && isEl) {
          //if (CbmLitGlobalElectronId::GetInstance().IsRichElectron(iTrack, mom)){
          fHM->H2("fhRTDistVsMomTruematchElId")->Fill(mom, rtDistance);
          //}
        }
      }
      else {
        fHM->H2("fhRTDistVsMomWrongmatch" + t)->Fill(mom, rtDistance);
      }
    }
  }
}


void CbmRichRecoQa::DrawHist()
{
  SetDefaultDrawStyle();

  {
    fHM->CreateCanvas("richqa_RTDist_truematch_epem", "richqa_RTDist_truematch_epem", 1000, 1000);
    TH1D* py_minus = (TH1D*) (fHM->H2("fhRTDistVsMomTruematchPrimelMinus")
                                ->ProjectionY("fhRTDistVsMomTruematchPrimelMinus_py")
                                ->Clone());
    py_minus->Scale(1. / py_minus->Integral());
    TH1D* py_plus = (TH1D*) (fHM->H2("fhRTDistVsMomTruematchPrimelPlus")
                               ->ProjectionY("fhRTDistVsMomTruematchPrimelPlus_py")
                               ->Clone());
    py_plus->Scale(1. / py_plus->Integral());
    DrawH1({py_minus, py_plus},
           {"e_{prim}^{-} (" + GetMeanRmsOverflowString(py_minus, false) + ")",
            "e_{prim}^{+} (" + GetMeanRmsOverflowString(py_plus, false) + ")"},
           kLinear, kLog, true, 0.5, 0.85, 0.99, 0.99, "hist");
  }

  {
    fHM->CreateCanvas("richqa_RTDist_truematch_elpi", "richqa_RTDist_truematch_elpi", 800, 800);
    TH1D* py_el =
      (TH1D*) (fHM->H2("fhRTDistVsMomTruematchPrimel")->ProjectionY("fhRTDistVsMomTruematchPrimel_py")->Clone());
    py_el->Scale(1. / py_el->Integral());
    TH1D* py_pi = (TH1D*) (fHM->H2("fhRTDistVsMomTruematchPi")->ProjectionY("fhRTDistVsMomTruematchPi_py")->Clone());
    py_pi->Scale(1. / py_pi->Integral());
    DrawH1({py_el, py_pi},
           {"e_{prim}^{#pm} (" + GetMeanRmsOverflowString(py_el, false) + ")",
            "#pi^{#pm} (" + GetMeanRmsOverflowString(py_pi, false) + ")"},
           kLinear, kLog, true, 0.5, 0.85, 0.99, 0.99, "hist");
  }

  {
    gStyle->SetPaintTextFormat("4.1f");
    fHM->CreateCanvas("richqa_mismatch_src", "richqa_mismatch_src", 1000, 800);
    double nofMcEl = fHM->H1("fhMismatchSrc")->GetBinContent(1);
    fHM->Scale("fhMismatchSrc", 100. / nofMcEl);
    DrawH1(fHM->H1("fhMismatchSrc"), kLinear, kLog, "hist text");
    fHM->H1("fhMismatchSrc")->SetMarkerSize(1.9);

    vector<string> labels {"MC",
                           "STS",
                           "STS-Acc RICH",
                           "STS-RICH",
                           "STS-RICH true",
                           "STS-NoRICH",
                           "STS-NoRICH RF",
                           "STS-NoRICH RM",
                           "STS-NoRICH NoRF",
                           "STS-NoRICH NoPrj",
                           "STS-RICH wrong",
                           "STS-RICH wrong RF",
                           "STS-RICH wrong RM"};

    for (size_t i = 0; i < labels.size(); i++) {
      fHM->H1("fhMismatchSrc")->GetXaxis()->SetBinLabel(i + 1, labels[i].c_str());
    }
    fHM->H1("fhMismatchSrc")->GetXaxis()->SetLabelSize(0.03);
  }

  {
    fHM->CreateCanvas("richqa_mismatch_src_mom", "richqa_mismatch_src_mom", 1000, 800);
    vector<string> labels {"MC",
                           "STS",
                           "STS-Acc RICH",
                           "STS-RICH",
                           "STS-RICH true",
                           "STS-NoRICH",
                           "STS-NoRICH RF",
                           "STS-NoRICH RM",
                           "STS-NoRICH NoRF",
                           "STS-NoRICH NoPrj",
                           "STS-RICH wrong",
                           "STS-RICH wrong RF",
                           "STS-RICH wrong RM"};
    vector<TH1*> hists = fHM->H1Vector(
      {"fhMismatchSrcMomMc", "fhMismatchSrcMomSts", "fhMismatchSrcMomStsAccRich", "fhMismatchSrcMomStsRich",
       "fhMismatchSrcMomStsRichTrue", "fhMismatchSrcMomStsNoRich", "fhMismatchSrcMomStsNoRichRF",
       "fhMismatchSrcMomStsNoRichRM", "fhMismatchSrcMomStsNoRichNoRF", "fhMismatchSrcMomStsNoRichNoProj",
       "fhMismatchSrcMomStsRichWrong", "fhMismatchSrcMomStsRichWrongRF", "fhMismatchSrcMomStsRichWrongRM"});

    DrawH1(hists, labels, kLinear, kLog, true, 0.8, 0.35, 0.99, 0.99);
    fHM->H1("fhMismatchSrcMomMc")->SetMinimum(0.9);
  }

  DrawRingTrackDist("Primel");
  DrawRingTrackDist("PrimelPlus");
  DrawRingTrackDist("PrimelMinus");
  DrawRingTrackDist("Pi");

  // before and after electron identification
  {
    TCanvas* c = fHM->CreateCanvas("richqa_RTDist_truematch_elid", "richqa_RTDist_truematch_elid", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhRTDistVsMomTruematchPrimel"), false, true);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRTDistVsMomTruematchElId"), false, true);
    c->cd(3);
    TH1D* py     = (TH1D*) (fHM->H2("fhRTDistVsMomTruematchPrimel")
                          ->ProjectionY(string("fhRTDistVsMomTruematchPrimel_py2").c_str())
                          ->Clone());
    TH1D* pyElId = (TH1D*) (fHM->H2("fhRTDistVsMomTruematchElId")
                              ->ProjectionY(string("fhRTDistVsMomTruematchElId_py").c_str())
                              ->Clone());
    DrawH1(
      {py, pyElId},
      {"before ElId (" + GetMeanRmsOverflowString(py) + ")", "after ElId (" + GetMeanRmsOverflowString(pyElId) + ")"},
      kLinear, kLog, true, 0.3, 0.75, 0.99, 0.99);
    fHM->H2("fhRTDistVsMomTruematchPrimel")->GetYaxis()->SetRangeUser(0., 2.);
    fHM->H2("fhRTDistVsMomTruematchElId")->GetYaxis()->SetRangeUser(0., 2.);
  }
}

string CbmRichRecoQa::GetMeanRmsOverflowString(TH1* h, bool withOverflow)
{
  if (withOverflow) {
    double overflow = h->GetBinContent(h->GetNbinsX() + 1);
    return Cbm::NumberToString<double>(h->GetMean(), 2) + " / " + Cbm::NumberToString<double>(h->GetRMS(), 2) + " / "
           + Cbm::NumberToString<double>(100. * overflow / h->Integral(0, h->GetNbinsX() + 1), 2) + "%";
  }
  else {
    return Cbm::NumberToString<double>(h->GetMean(), 2) + " / " + Cbm::NumberToString<double>(h->GetRMS(), 2);
  }
}


void CbmRichRecoQa::DrawRingTrackDist(const string& opt)
{
  {
    TCanvas* c = fHM->CreateCanvas("richqa_RTDist_truematch_" + opt, "richqa_RTDist_truematch_" + opt, 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhRTDistVsMomTruematch" + opt), false, true);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRTDistVsNofHitsTruematch" + opt), false, true);
    c->cd(3);
    TH1D* py = (TH1D*) (fHM->H2("fhRTDistVsMomTruematch" + opt)
                          ->ProjectionY(("fhRTDistVsMomTruematch_py" + opt).c_str())
                          ->Clone());
    DrawH1(py);
    DrawTextOnPad(this->GetMeanRmsOverflowString(py), 0.2, 0.9, 0.8, 0.99);
    gPad->SetLogy(true);

    fHM->H2("fhRTDistVsMomTruematch" + opt)->GetYaxis()->SetRangeUser(0., 2.);
    fHM->H2("fhRTDistVsNofHitsTruematch" + opt)->GetYaxis()->SetRangeUser(0., 2.);
  }

  {
    fHM->CreateCanvas("richqa_RTDist_wrongmatch" + opt, "richqa_RTDist_wrongmatch" + opt, 600, 600);
    TH1D* py = (TH1D*) (fHM->H2("fhRTDistVsMomWrongmatch" + opt)
                          ->ProjectionY(("fhRTDistVsMomWrongmatch_py" + opt).c_str())
                          ->Clone());
    DrawH1(py);
    DrawTextOnPad(this->GetMeanRmsOverflowString(py), 0.2, 0.9, 0.8, 0.99);
    gPad->SetLogy(true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richqa_RTDist_xy_truematch" + opt, "richqa_RTDist_xy_truematch" + opt, 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRTDistVsXYTruematch" + opt), true, false, 0., .4);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRTDistVsXTruematch" + opt), false, true);
    fHM->H2("fhRTDistVsXTruematch" + opt)->GetYaxis()->SetRangeUser(0., 2.);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhRTDistVsYTruematch" + opt), false, true);
    fHM->H2("fhRTDistVsYTruematch" + opt)->GetYaxis()->SetRangeUser(0., 2.);
  }
}

bool CbmRichRecoQa::IsMcPrimaryElectron(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr) return false;
  if (mctrack->GetGeantProcessId() == kPPrimary && std::abs(mctrack->GetPdgCode()) == 11) return true;
  return false;
}

bool CbmRichRecoQa::IsMcPion(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr || std::abs(mctrack->GetPdgCode()) != 211) return false;
  return true;
}

void CbmRichRecoQa::Finish()
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

void CbmRichRecoQa::DrawFromFile(const string& fileName, const string& outputDir)
{
  fOutputDir = outputDir;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  if (fHM != nullptr) delete fHM;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);

  DrawHist();

  fHM->SaveCanvasToImage(fOutputDir);

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmRichRecoQa)
