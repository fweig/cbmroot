#include "CbmRichMCbmQaRichOnly.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TH1.h"
#include "TH1D.h"
#include "TLine.h"
#include "TMarker.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"

#include <TBox.h>
#include <TLegend.h>

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDigi.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingFinderHoughImpl.h"
#include "CbmRichUtil.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdTrack.h"
#include "TLatex.h"

#include "CbmRichMCbmSEDisplay.h"

#include "CbmEvent.h"

#include "CbmRichConverter.h"

#include "CbmHistManager.h"
#include "CbmUtils.h"

#include <boost/assign/list_of.hpp>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


using namespace std;
using boost::assign::list_of;

#define RichZPos 348.

CbmRichMCbmQaRichOnly::CbmRichMCbmQaRichOnly()
  : FairTask("CbmRichMCbmQaRichOnly")
  , fRichHits(nullptr)
  , fRichRings(nullptr)
  , fCbmEvent(nullptr)
  , fHM(nullptr)
  , fEventNum(0)
  , fNofDrawnRings(0)
  , fNofDrawnRichTofEv(0)
  , fNofDrawnEvents(0)
  , fMaxNofDrawnEvents(100)
  , fTriggerRichHits(0)
  , fOutputDir("result") {}

InitStatus CbmRichMCbmQaRichOnly::Init() {
  cout << "CbmRichMCbmQaRichOnly::Init" << endl;

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    Fatal("CbmRichMCbmQaRichOnly::Init", "RootManager not instantised!");
  }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  if (!fDigiMan->IsPresent(ECbmModuleId::kRich))
    Fatal("CbmRichMCbmQaReal::Init", "No Rich Digis!");

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) {
    Fatal("CbmRichMCbmQaRichOnly::Init", "No Rich Hits!");
  }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) {
    Fatal("CbmRichMCbmQaRichOnly::Init", "No Rich Rings!");
  }

  fCbmEvent = (TClonesArray*) ioman->GetObject("CbmEvent");
  if (nullptr == fCbmEvent) {
    Fatal("CbmRichMCbmQaRichOnly::Init", "No Event!");
  }

  InitHistograms();


  fSeDisplay = new CbmRichMCbmSEDisplay(fHM);
  fSeDisplay->SetRichHits(fRichHits);
  fSeDisplay->SetRichRings(fRichRings);
  ///fSeDisplay->SetTofTracks(fTofTracks);
  fSeDisplay->SetTotRich(23.7, 30.);
  fSeDisplay->SetMaxNofDrawnEvents(fMaxNofDrawnEvents);
  fSeDisplay->SetOutDir(fOutputDir);

  //Init OffsetCorrection ICD
  for (auto& a : ICD_offset_read)
    a = 0.;
  for (auto& a : ICD_offset)
    a = 0.;
  for (auto& a : ICD_offset_cnt)
    a = 0;
  read_ICD(ICD_offset_read, 0);

  return kSUCCESS;
}

void CbmRichMCbmQaRichOnly::InitHistograms() {
  fHM = new CbmHistManager();

  fHM->Create1<TH1D>("fhNofEvents", "fhNofEvents;Entries", 1, 0.5, 1.5);
  fHM->Create1<TH1D>("fhNofCbmEvents", "fhNofCbmEvents;Entries", 1, 0.5, 1.5);
  fHM->Create1<TH1D>(
    "fhNofCbmEventsRing", "fhNofCbmEventsRing;Entries", 1, 0.5, 1.5);

  fHM->Create1<TH1D>("fhNofBlobEvents", "fhNofBlobEvents;Entries", 1, 0.5, 1.5);
  fHM->Create1<TH1D>(
    "fhNofBlobsInEvent", "fhNofBlobsInEvent;Entries", 36, 0.5, 36.5);

  // RICH hits
  fHM->Create2<TH2D>("fhRichHitXY",
                     "fhRichHitXY;RICH hit X [cm];RICH hit Y [cm];Entries",
                     67,
                     -20.1 + fXOffsetHisto,
                     20.1 + fXOffsetHisto,
                     84,
                     -25.2,
                     25.2);
  fHM->Create2<TH2D>(
    "fhRichHitXY_fromRing",
    "fhRichHitXY_fromRing;RICH hit X [cm];RICH hit Y [cm];Entries",
    67,
    -20.1 + fXOffsetHisto,
    20.1 + fXOffsetHisto,
    84,
    -25.2,
    25.2);

  //ToT
  fHM->Create1<TH1D>(
    "fhRichDigisToT", "fhRichDigisToT;ToT [ns];Entries", 601, 9.975, 40.025);
  fHM->Create1<TH1D>(
    "fhRichHitToT", "fhRichHitToT;ToT [ns];Entries", 601, 9.975, 40.025);

  // RICH rings
  fHM->Create2<TH2D>(
    "fhRichRingXY",
    "fhRichRingXY;Ring center X [cm];Ring center Y [cm];Entries",
    67,
    -20.1 + fXOffsetHisto,
    20.1 + fXOffsetHisto,
    84,
    -25.2,
    25.2);
  fHM->Create1<TH1D>("fhRichRingRadius",
                     "fhRichRingRadius;Ring radius [cm];Entries",
                     100,
                     0.,
                     7.);
  fHM->Create1<TH1D>("fhNofHitsInRing",
                     "fhNofHitsInRing;# hits in ring;Entries",
                     50,
                     -0.5,
                     49.5);
  fHM->Create2<TH2D>(
    "fhICD", "fhICD;channel;DeltaTime", 2305, -0.5, 2304.5, 31, -15.5, 15.5);

  fHM->Create2<TH2D>(
    "fhRichRingRadiusY",
    "fhRichRingRadiusY;Ring Radius [cm]; Y position[cm];Entries",
    70,
    -0.05,
    6.95,
    84,
    -25.2,
    25.2);
  fHM->Create2<TH2D>(
    "fhRichHitsRingRadius",
    "fhRichHitsRingRadius;#Rich Hits/Ring; Ring Radius [cm];Entries",
    50,
    -0.5,
    49.5,
    70,
    -0.05,
    6.95);

  fHM->Create1<TH1D>("fhRingDeltaTime",
                     "fhRingDeltaTime; \\Delta Time/ns;Entries",
                     101,
                     -10.1,
                     10.1);
  fHM->Create1<TH1D>(
    "fhRingChi2", "fhRingChi2; \\Chi^2 ;Entries", 101, 0.0, 10.1);

  fHM->Create2<TH2D>(
    "fhRichRingCenterXChi2",
    "fhRichRingCenterXChi2;Ring Center X [cm];\\Chi^2 ;;Entries",
    67,
    -20.1 + fXOffsetHisto,
    20.1 + fXOffsetHisto,
    101,
    0.0,
    10.1);
  fHM->Create2<TH2D>(
    "fhRichRingCenterYChi2",
    "fhRichRingCenterYChi2;Ring Center Y [cm];\\Chi^2 ;;Entries",
    84,
    -25.2,
    25.2,
    101,
    0.0,
    10.1);
  fHM->Create2<TH2D>(
    "fhRichRingRadiusChi2",
    "fhRichRingRadiusChi2; Ring Radius   [cm];\\Chi^2 ;;Entries",
    70,
    -0.05,
    6.95,
    101,
    0.0,
    10.1);


  // Digis
  fHM->Create2<TH2D>("fhDigisInChnl",
                     "fhDigisInChnl;channel;#Digis;",
                     2304,
                     -0.5,
                     2303.5,
                     50,
                     -0.5,
                     49.5);
  fHM->Create2<TH2D>("fhDigisInDiRICH",
                     "fhDigisInDiRICH;DiRICH;#Digis;",
                     72,
                     -0.5,
                     71.5,
                     300,
                     -0.5,
                     299.5);
}


void CbmRichMCbmQaRichOnly::Exec(Option_t* /*option*/) {
  fEventNum++;
  fHM->H1("fhNofEvents")->Fill(1);

  cout << "CbmRichMCbmQaRichOnly, event No. " << fEventNum << endl;

  std::array<unsigned int, 2304> chnlDigis;
  for (auto& c : chnlDigis)
    c = 0;
  for (int i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
    const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(i);
    fHM->H1("fhRichDigisToT")->Fill(richDigi->GetToT());

    uint16_t addrDiRICH = (richDigi->GetAddress() >> 16) & 0xFFFF;
    uint16_t addrChnl   = richDigi->GetAddress() & 0xFFFF;
    uint16_t dirichNmbr = ((addrDiRICH >> 8) & 0xF) * 18
                          + ((addrDiRICH >> 4) & 0xF) * 2
                          + ((addrDiRICH >> 0) & 0xF);
    uint32_t fullNmbr = (dirichNmbr << 5) | (addrChnl - 1);
    chnlDigis[fullNmbr]++;
  }

  {
    unsigned int sum = 0;
    for (uint16_t i = 0; i < 2304; ++i) {
      if (chnlDigis[i] != 0) fHM->H1("fhDigisInChnl")->Fill(i, chnlDigis[i]);
      sum += chnlDigis[i];
      if (i % 32 == 31) {
        uint16_t dirich = i / 32;
        if (sum != 0) fHM->H1("fhDigisInDiRICH")->Fill(dirich, sum);
        sum = 0;
      }
    }
  }

  int nofRichHits = fRichHits->GetEntries();
  for (int iH = 0; iH < nofRichHits; iH++) {
    CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    fHM->H2("fhRichHitXY")->Fill(richHit->GetX(), richHit->GetY());
    fHM->H1("fhRichHitToT")->Fill(richHit->GetToT());
  }


  //CBMEVENT
  auto fNCbmEvent = fCbmEvent->GetEntriesFast();

  for (int i = 0; i < fNCbmEvent; i++) {
    fHM->H1("fhNofCbmEvents")->Fill(1);
    CbmEvent* ev = static_cast<CbmEvent*>(fCbmEvent->At(i));

    if (fTriggerRichHits != 0
        && (ev->GetNofData(ECbmDataType::kRichHit) < fTriggerRichHits))
      continue;


    std::vector<int> ringIndx;
    std::vector<int> evRichHitIndx;
    std::array<uint32_t, 36> pmtHits;
    for (auto& a : pmtHits)
      a = 0;

    // Map Rings to CbmEvent
    for (int j = 0; j < ev->GetNofData(ECbmDataType::kRichHit); j++) {
      auto iRichHit = ev->GetIndex(ECbmDataType::kRichHit, j);
      evRichHitIndx.push_back(iRichHit);
      CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));

      uint32_t pmtId = (((richHit->GetAddress()) >> 20) & 0xF)
                       + (((richHit->GetAddress()) >> 24) & 0xF) * 9;
      pmtHits[pmtId]++;

      int nofRichRings = fRichRings->GetEntries();
      for (int l = 0; l < nofRichRings; l++) {
        CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(l));

        auto NofRingHits = ring->GetNofHits();
        for (int m = 0; m < NofRingHits; m++) {
          auto RingHitIndx = ring->GetHit(m);
          if (RingHitIndx == iRichHit) {
            Bool_t used = false;
            for (auto check : ringIndx) {
              if (check == l) {
                used = true;
                break;
              }
            }
            if (used == false) ringIndx.push_back(l);
            break;
          }
        }
      }
    }

    uint16_t blob = 0;
    for (auto a : pmtHits) {
      if (a > 30) { blob++; }
    }
    if (blob > 0) {
      fHM->H1("fhNofBlobEvents")->Fill(1);
      fHM->H1("fhNofBlobsInEvent")->Fill(blob);
    }

    if (ringIndx.size() != 0) fHM->H1("fhNofCbmEventsRing")->Fill(1);

    //Ring Loop
    for (unsigned int k = 0; k < ringIndx.size(); ++k) {
      //Rigs in this CbmEvent
      CbmRichRing* ring =
        static_cast<CbmRichRing*>(fRichRings->At(ringIndx[k]));
      if (ring == nullptr) continue;
      fHM->H1("fhRingChi2")->Fill(ring->GetChi2());
      fHM->H2("fhRichRingCenterXChi2")
        ->Fill(ring->GetCenterX(), ring->GetChi2());
      fHM->H2("fhRichRingCenterYChi2")
        ->Fill(ring->GetCenterY(), ring->GetChi2());
      fHM->H2("fhRichRingRadiusChi2")->Fill(ring->GetRadius(), ring->GetChi2());

      fHM->H2("fhRichRingXY")->Fill(ring->GetCenterX(), ring->GetCenterY());
      fHM->H1("fhRichRingRadius")->Fill(ring->GetRadius());
      fHM->H1("fhNofHitsInRing")->Fill(ring->GetNofHits());
      fHM->H2("fhRichRingRadiusY")->Fill(ring->GetRadius(), ring->GetCenterY());
      fHM->H2("fhRichHitsRingRadius")
        ->Fill(ring->GetNofHits(), ring->GetRadius());
      for (int j = 0; j < ring->GetNofHits(); ++j) {
        Int_t hitIndx   = ring->GetHit(j);
        CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitIndx);
        if (nullptr == hit) continue;
        fHM->H2("fhRichHitXY_fromRing")->Fill(hit->GetX(), hit->GetY());
      }
    }
    fSeDisplay->DrawEvent(
      static_cast<CbmEvent*>(fCbmEvent->At(i)), ringIndx, 1);

  }  //End CbmEvent loop

  // Loop over all Rings
  RichRings();
}

void CbmRichMCbmQaRichOnly::RichRings() {
  int nofRichRings = fRichRings->GetEntries();
  for (int i = 0; i < nofRichRings; i++) {
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(i));
    if (ring == nullptr) continue;

    for (int j = 1; j < ring->GetNofHits(); ++j) {
      Int_t hitIndx   = ring->GetHit(j);
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitIndx);
      if (nullptr == hit) continue;

      //Read Address
      uint32_t DiRICH_Addr = hit->GetAddress();
      unsigned int addr    = (((DiRICH_Addr >> 24) & 0xF) * 18 * 32)
                          + (((DiRICH_Addr >> 20) & 0xF) * 2 * 32)
                          + (((DiRICH_Addr >> 16) & 0xF) * 32)
                          + ((DiRICH_Addr & 0xFFFF) - 0x1);
      ICD_offset.at(addr) +=
        hit->GetTime() - ring->GetTime() - ICD_offset_read.at(addr);
      fHM->H2("fhICD")->Fill(
        addr, hit->GetTime() - ring->GetTime() - ICD_offset_read.at(addr));
      fHM->H1("fhRingDeltaTime")
        ->Fill(hit->GetTime() - ring->GetTime() - ICD_offset_read.at(addr));
      ICD_offset_cnt.at(addr)++;
    }

    //DrawRing(ring);
    /*fHM->H2("fhRichRingXY")->Fill(ring->GetCenterX(), ring->GetCenterY());
        fHM->H1("fhRichRingRadius")->Fill(ring->GetRadius());
        fHM->H1("fhNofHitsInRing")->Fill(ring->GetNofHits());
        */
  }
}


void CbmRichMCbmQaRichOnly::DrawHist() {
  cout.precision(4);

  //SetDefaultDrawStyle();
  double nofEvents = fHM->H1("fhNofCbmEvents")->GetEntries();
  fHM->ScaleByPattern("fh_.*", 1. / nofEvents);

  {
    fHM->CreateCanvas(
      "rich_mcbm_fhNofCbmEvents", "rich_mcbm_fhNofCbmEvents", 600, 600);
    DrawH1(fHM->H1("fhNofCbmEvents"));
  }

  {
    fHM->CreateCanvas(
      "rich_mcbm_fhNofCbmEventsRing", "rich_mcbm_fhNofCbmEventsRing", 600, 600);
    DrawH1(fHM->H1("fhNofCbmEventsRing"));
  }

  {
    fHM->CreateCanvas(
      "rich_mcbm_fhNofEvents", "rich_mcbm_fhNofEvents", 600, 600);
    DrawH1(fHM->H1("fhNofEvents"));
  }

  {
    fHM->CreateCanvas(
      "rich_mcbm_fhBlobEvents", "rich_mcbm_fhBlobEvents", 600, 600);
    DrawH1(fHM->H1("fhNofBlobEvents"));
  }

  {
    fHM->CreateCanvas(
      "rich_mcbm_fhBlobsInCbmEvent", "rich_mcbm_fhBlobsInCbmEvent", 600, 600);
    DrawH1(fHM->H1("fhNofBlobsInEvent"));
  }

  {
    fHM->CreateCanvas("inner_channel_delay", "inner_channel_delay", 1200, 600);
    DrawH2(fHM->H2("fhICD"));
  }


  {
    fHM->CreateCanvas("RingDelta", "RingDelta", 600, 600);
    DrawH1(fHM->H1("fhRingDeltaTime"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_ToT", "rich_ToT", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRichDigisToT"));
    c->cd(2);
    DrawH1(fHM->H1("fhRichHitToT"));
  }

  {
    fHM->CreateCanvas("DigisInChnl", "DigisInChnl", 1200, 600);
    DrawH2(fHM->H2("fhDigisInChnl"));
  }

  {
    fHM->CreateCanvas("DigisInDiRICH", "DigisInDiRICH", 1200, 600);
    DrawH2(fHM->H2("fhDigisInDiRICH"));
  }


  {
    TCanvas* c = fHM->CreateCanvas("rich_Hits", "rich_Hits", 1200, 1200);
    c->Divide(2, 2);
    c->cd(1);
    DrawH2(fHM->H2("fhRichHitXY"));
    c->cd(2);
    DrawH2(fHM->H2("fhRichHitXY_fromRing"));
    c->cd(3);
    TH2F* hitsBg = (TH2F*) fHM->H2("fhRichHitXY")->Clone();
    hitsBg->SetName("fhRichHitXY_bg");
    hitsBg->SetTitle("fhRichHitXY_bg");
    hitsBg->Add(fHM->H2("fhRichHitXY_fromRing"), -1);
    //hitsBg->Draw("colz");
    DrawH2(hitsBg);
    c->cd(4);
    DrawH2(fHM->H2("fhRichRingXY"));
  }

  {
    TCanvas* c =
      fHM->CreateCanvas("rich_mcbm_rings", "rich_mcbm_rings", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRichRingRadius"));
    c->cd(2);
    DrawH1(fHM->H1("fhNofHitsInRing"));
  }

  {
    fHM->CreateCanvas("RichRingRadiusVsY", "RichRingRadiusVsY", 800, 800);
    //c->SetLogy();
    DrawH2(fHM->H2("fhRichRingRadiusY"));
  }

  {
    fHM->CreateCanvas("RichRingHitsVsRadius", "RichRingHitsVsRadius", 800, 800);
    //c->SetLogy();
    DrawH2(fHM->H2("fhRichHitsRingRadius"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("RichRingChi2", "RichRingChi2", 1600, 800);
    //c->SetLogy();
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRingChi2"));
    c->cd(2);
    DrawH2(fHM->H2("fhRichRingRadiusChi2"));
  }

  {
    TCanvas* c =
      fHM->CreateCanvas("RichRingCenterChi2", "RichRingCenterChi2", 1600, 800);
    //c->SetLogy();
    c->Divide(2, 1);
    c->cd(1);
    DrawH2(fHM->H2("fhRichRingCenterXChi2"));
    c->cd(2);
    DrawH2(fHM->H2("fhRichRingCenterYChi2"));
  }
}

void CbmRichMCbmQaRichOnly::DrawRing(CbmRichRing* ring, bool full) {
  //std::cout<<"#!#DRAW!!!"<<std::endl;
  if (fNofDrawnRings > 20) return;
  fNofDrawnRings++;
  stringstream ss;
  ss << "Event" << fNofDrawnRings;
  //fNofDrawnRings++;
  TCanvas* c = nullptr;
  if (full == true) {
    c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
  } else {
    c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 500, 500);
  }
  c->SetGrid(true, true);
  TH2D* pad = nullptr;
  if (full == true) {
    pad = new TH2D(ss.str().c_str(),
                   (ss.str() + ";X [cm];Y [cm]").c_str(),
                   1,
                   -15.,
                   10.,
                   1,
                   -5.,
                   20);
  } else {
    pad = new TH2D(ss.str().c_str(),
                   (ss.str() + ";X [cm];Y [cm]").c_str(),
                   1,
                   -5.,
                   5.,
                   1,
                   -5.,
                   5);
  }

  pad->SetStats(false);
  pad->Draw();

  if (full == true) {
    //rough Drawing of RichDetectorAcceptance
    TLine* line0 = new TLine(-6.25, 8, -6.25, 15.9);
    line0->Draw();
    TLine* line1 = new TLine(-6.25, 15.9, -1.05, 15.9);
    line1->Draw();
    TLine* line2 = new TLine(-1.05, 15.9, -1.05, 13.2);
    line2->Draw();
    TLine* line3 = new TLine(-1.05, 13.2, +4.25, 13.2);
    line3->Draw();
    TLine* line4 = new TLine(4.25, 13.2, +4.25, 8);
    line4->Draw();
    TLine* line5 = new TLine(4.25, 8, -6.25, 8);
    line5->Draw();
  }

  // find min and max x and y positions of the hits
  // in order to shift drawing
  double xCur = 0.;
  double yCur = 0.;

  if (full == false) {
    double xmin = 99999., xmax = -99999., ymin = 99999., ymax = -99999.;
    for (int i = 0; i < ring->GetNofHits(); i++) {
      Int_t hitInd    = ring->GetHit(i);
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
      if (nullptr == hit) continue;
      if (xmin > hit->GetX()) xmin = hit->GetX();
      if (xmax < hit->GetX()) xmax = hit->GetX();
      if (ymin > hit->GetY()) ymin = hit->GetY();
      if (ymax < hit->GetY()) ymax = hit->GetY();
    }
    xCur = (xmin + xmax) / 2.;
    yCur = (ymin + ymax) / 2.;
  }

  //Draw circle and center
  TEllipse* circle = new TEllipse(
    ring->GetCenterX() - xCur, ring->GetCenterY() - yCur, ring->GetRadius());
  circle->SetFillStyle(0);
  circle->SetLineWidth(3);
  circle->Draw();
  TEllipse* center =
    new TEllipse(ring->GetCenterX() - xCur, ring->GetCenterY() - yCur, .1);
  center->Draw();


  double hitZ      = 0;
  uint nofDrawHits = 0;

  // Draw hits
  for (int i = 0; i < ring->GetNofHits(); i++) {
    Int_t hitInd    = ring->GetHit(i);
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
    if (nullptr == hit) continue;
    TEllipse* hitDr = new TEllipse(hit->GetX() - xCur, hit->GetY() - yCur, .25);
    //std::cout<<"LE of Hit: "<< hit->GetTime()- fCbmEventStartTime << "\t" << hit->GetTime() << "\t" << fCbmEventStartTime <<std::endl;
    if (doToT(hit)) {  // Good ToT selection
      hitDr->SetFillColor(kRed);
    } else {
      hitDr->SetFillColor(kBlue);
    }
    hitZ += hit->GetZ();
    nofDrawHits++;
    hitDr->Draw();
  }
  hitZ /= nofDrawHits;


  //Draw information
  stringstream ss2;
  ss2 << "(x,y,r,n)=(" << setprecision(3) << ring->GetCenterX() << ", "
      << ring->GetCenterY() << ", " << ring->GetRadius() << ", "
      << ring->GetNofHits() << ")";
  TLatex* latex = nullptr;
  if (full == true) {
    latex = new TLatex(
      ring->GetCenterX() - 13., ring->GetCenterY() + 5., ss2.str().c_str());
  } else {
    latex = new TLatex(-4., 4., ss2.str().c_str());
  }
  latex->Draw();
}


void CbmRichMCbmQaRichOnly::Finish() {

  for (unsigned int i = 0; i < ICD_offset.size(); ++i) {
    if (ICD_offset_cnt.at(i) == 0) {
      ICD_offset.at(i) = 0.0;
    } else {
      ICD_offset.at(i) /= ICD_offset_cnt.at(i);
    }
    ICD_offset.at(i) += ICD_offset_read.at(i);
  }
  save_ICD(ICD_offset, 0);

  //std::cout<<"Address: "<<InterChannel[0].first << std::endl;
  //std::cout<<"Tracks:  "<< fTofTracks->GetEntriesFast() <<std::endl;
  std::cout << "Drawing Hists...";
  DrawHist();
  std::cout << "DONE!" << std::endl;

  if (this->fDoDrawCanvas) {
    fHM->SaveCanvasToImage(fOutputDir, "png");
    std::cout << "Canvas saved to Images!" << std::endl;
  }

  if (this->fDoWriteHistToFile) {
    TDirectory* oldir = gDirectory;
    std::string s     = fOutputDir + "/RecoHists.root";
    TFile* outFile    = new TFile(s.c_str(), "RECREATE");
    if (outFile->IsOpen()) {
      fHM->WriteToFile();
      std::cout << "Written to Root-file \"" << s << "\"  ...";
      outFile->Close();
      std::cout << "Done!" << std::endl;
    }
    gDirectory->cd(oldir->GetPath());
  }
}


void CbmRichMCbmQaRichOnly::DrawFromFile(const string& fileName,
                                         const string& outputDir) {
  fOutputDir = outputDir;

  if (fHM != nullptr) delete fHM;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);
  DrawHist();

  fHM->SaveCanvasToImage(fOutputDir);
}

bool CbmRichMCbmQaRichOnly::doToT(CbmRichHit* hit) {
  bool check = false;
  if ((hit->GetToT() > 23.7) && (hit->GetToT() < 30.0)) check = true;

  return check;
}

void CbmRichMCbmQaRichOnly::analyseRing(CbmRichRing* ring, CbmEvent* ev) {

  std::cout << "analyse a Ring" << std::endl;

  Double_t meanTime   = 0.;
  unsigned int hitCnt = 0;
  Double_t minRHit2   = std::numeric_limits<Double_t>::max();
  for (int i = 0; i < ring->GetNofHits(); i++) {
    Int_t hitInd    = ring->GetHit(i);
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
    if (nullptr == hit) continue;

    meanTime += hit->GetTime();
    hitCnt++;

    const Float_t diffX         = hit->GetX() - ring->GetCenterX();
    const Float_t diffY         = hit->GetY() - ring->GetCenterY();
    const Float_t tmpHitRadius2 = (diffX * diffX + diffY * diffY);

    if (tmpHitRadius2 < minRHit2) { minRHit2 = tmpHitRadius2; }
  }
  meanTime = meanTime / hitCnt;

  //std::cout<<"mean: "<<meanTime<<std::endl;
  for (int i = 0; i < ring->GetNofHits(); i++) {
    Int_t hitInd    = ring->GetHit(i);
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
    if (nullptr == hit) continue;
    //std::cout<<"DeltatTime: "<< meanTime - hit->GetTime()<<std::endl;
    fHM->H1("fhRingDeltaTime")
      ->Fill(static_cast<Double_t>(meanTime - hit->GetTime()));

    fHM->H1("fhRingToTs")->Fill(hit->GetToT());
    fHM->H1("fhRingLE")
      ->Fill(static_cast<Double_t>(hit->GetTime() - ev->GetStartTime()));
    fHM->H2("fhRingLEvsToT")
      ->Fill(static_cast<Double_t>(hit->GetTime() - ev->GetStartTime()),
             hit->GetToT());
    //std::vector<int> tmpRingIndx;
    //tmpRingIndx.push_back(ring->GetIndex);
    const Double_t Tdiff_ring = (hit->GetTime() - ev->GetStartTime());
    if ((Tdiff_ring > 20.) && (Tdiff_ring < 30.)) {
      std::cout << ev->GetNumber() << " Address_ring: " << std::hex
                << CbmRichUtil::GetDirichId(hit->GetAddress()) << std::dec
                << "  " << CbmRichUtil::GetDirichChannel(hit->GetAddress())
                << "  " << hit->GetToT() << "  " << ring->GetRadius()
                << std::endl;
      //fHM->H1("fhDiRICHsInRegion")->Fill(CbmRichUtil::GetDirichId(hit->GetAddress()));
    }
  }

  int InnerHitCnt     = 0;
  int InnerHitCnt_cut = 0;
  for (int j = 0; j < ev->GetNofData(ECbmDataType::kRichHit); j++) {
    auto iRichHit       = ev->GetIndex(ECbmDataType::kRichHit, j);
    CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
    if (nullptr == richHit) continue;
    const Float_t diffX = richHit->GetX() - ring->GetCenterX();
    const Float_t diffY = richHit->GetY() - ring->GetCenterY();
    //select inner Part of Ring
    if (diffX * diffX + diffY * diffY < minRHit2) {
      InnerHitCnt++;
      const Double_t Tdiff_inner = (richHit->GetTime() - ev->GetStartTime());
      if ((Tdiff_inner > 20.) && (Tdiff_inner < 30.)) {
        InnerHitCnt_cut++;
        //if (InnerHitCnt_cut == 1) {DrawRing(ring);}
        std::cout << ev->GetNumber() << " Address_inner: " << std::hex
                  << CbmRichUtil::GetDirichId(richHit->GetAddress()) << std::dec
                  << "  "
                  << CbmRichUtil::GetDirichChannel(richHit->GetAddress())
                  << "  " << richHit->GetToT() << "  " << ring->GetRadius()
                  << std::endl;
        fHM->H1("fhDiRICHsInRegion")
          ->Fill(CbmRichUtil::GetDirichId(richHit->GetAddress()));
      }

      fHM->H1("fhInnerRingDeltaTime")
        ->Fill(static_cast<Double_t>(meanTime - richHit->GetTime()));
      fHM->H1("fhInnerRingToTs")->Fill(richHit->GetToT());
      fHM->H1("fhInnerRingLE")
        ->Fill(static_cast<Double_t>(richHit->GetTime() - ev->GetStartTime()));
    }
  }
  if (InnerHitCnt == 0) {
    fHM->H1("fhInnerRingFlag")->Fill(1);
  } else {
    fHM->H1("fhInnerRingFlag")->Fill(0);
  }
  fHM->H1("fhNofInnerHits")->Fill(InnerHitCnt);
}


Bool_t CbmRichMCbmQaRichOnly::cutRadius(CbmRichRing* ring) {
  if (ring->GetRadius() > 2. && ring->GetRadius() < 4.2) return true;

  return false;
}

void CbmRichMCbmQaRichOnly::save_ICD(std::array<Double_t, 2304>& ICD_offsets,
                                     unsigned int iteration) {
  std::ofstream icd_file;
  icd_file.open(Form("icd_offset_it_%u.data", iteration));
  if (icd_file.is_open()) {
    for (unsigned int i = 0; i < ICD_offsets.size(); ++i) {
      //loop over all Offsets
      icd_file << i << "\t" << std::setprecision(25) << ICD_offsets.at(i)
               << "\n";
    }
    icd_file.close();
  } else
    std::cout << "Unable to open inter channel delay file icd_offset_it_"
              << iteration << ".data\n";
}


void CbmRichMCbmQaRichOnly::read_ICD(std::array<Double_t, 2304>& ICD_offsets,
                                     unsigned int iteration) {
  std::cout << gSystem->pwd() << std::endl;
  std::string line;
  std::ifstream icd_file(Form("icd_offset_it_%u.data", iteration));
  unsigned int lineCnt = 0;
  if (icd_file.is_open()) {
    while (getline(icd_file, line)) {
      if (line[0] == '#') continue;  // just a comment
      std::istringstream iss(line);
      unsigned int addr = 0;
      Double_t value;
      if (!(iss >> addr >> value)) {
        std::cout << "A Problem accured in line " << lineCnt << "\n";
        break;
      }  // error
      lineCnt++;
      ICD_offsets.at(addr) += value;
    }
    icd_file.close();
  } else
    std::cout << "Unable to open inter channel delay file icd_offset_it_"
              << iteration << ".data\n";
}

ClassImp(CbmRichMCbmQaRichOnly)
