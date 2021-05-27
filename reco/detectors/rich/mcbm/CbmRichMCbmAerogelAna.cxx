/** @file CbmRichMCbmAerogelAna.cxx
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmRichMCbmAerogelAna.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichConverter.h"
#include "CbmRichDigi.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingFinderHoughImpl.h"
#include "CbmRichUtil.h"
#include "CbmTofDigi.h"
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdTrack.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TH1.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMarker.h"
#include "TMath.h"
#include "TStyle.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include <cmath>

using namespace std;
using boost::assign::list_of;

#define RichZPos 348.

CbmRichMCbmAerogelAna::CbmRichMCbmAerogelAna()
  : FairTask("CbmRichMCbmAerogelAna")
  , fRichHits(nullptr)
  , fRichRings(nullptr)
  , fCbmEvent(nullptr)
  , fHM(nullptr)
  , fXOffsetHisto(12.)
  , fEventNum(0)
  , fNofDrawnRings(0)
  , fNofDrawnRichTofEv(0)
  , fNofDrawnEvents(0)
  , fOutputDir("result")
{
}

InitStatus CbmRichMCbmAerogelAna::Init()
{
  cout << "CbmRichMCbmAerogelAna::Init" << endl;

  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmRichMCbmQaReal::Init", "RootManager not instantised!"); }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) Fatal("CbmRichMCbmQaReal::Init", "No Rich Digis!");

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) Fatal("CbmRichMCbmQaReal::Init", "No Tof Digis!");

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmRichMCbmAerogelAna::Init", "No Rich Hits!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmRichMCbmAerogelAna::Init", "No Rich Rings!"); }


  //    fTofHits =(TClonesArray*) ioman->GetObject("TofHit");
  //    if (nullptr == fTofHits) { Fatal("CbmRichMCbmQaReal::Init", "No Tof Hits!");}

  //    fTofTracks =(TClonesArray*) ioman->GetObject("TofTracks");
  //    if (nullptr == fTofTracks) { Fatal("CbmRichMCbmQaReal::Init", "No Tof Tracks!");}

  //     fT0Digis =(TClonesArray*) ioman->GetObject("CbmT0Digi");
  //     if (nullptr == fT0Digis) { Fatal("CbmRichMCbmQaReal::Init", "No T0 Digis!");}

  //fT0Digis = ioman->InitObjectAs<std::vector<CbmTofDigi> const *>("T0Digi");

  fCbmEvent = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fCbmEvent) { Fatal("fTofDigis::Init", "No Event!"); }

  InitHistograms();

  return kSUCCESS;
}

void CbmRichMCbmAerogelAna::InitHistograms()
{
  fHM = new CbmHistManager();

  fHM->Create1<TH1D>("fhNofEvents", "fhNofEvents;Entries", 1, 0.5, 1.5);
  fHM->Create1<TH1D>("fhNofCbmEvents", "fhNofCbmEvents;Entries", 1, 0.5, 1.5);
  fHM->Create1<TH1D>("fhNofCbmEventsRing", "fhNofCbmEventsRing;Entries", 1, 0.5, 1.5);

  fHM->Create1<TH1D>("fhHitsInTimeslice", "fhHitsInTimeslice;Timeslice;#Hits", 200, 1, 200);

  // nof objects per timeslice
  fHM->Create1<TH1D>("fhNofRichDigisInTimeslice", "fhNofRichDigisInTimeslice;# RICH digis / timeslice;Entries", 100,
                     -0.5, 999.5);
  fHM->Create1<TH1D>("fhNofRichHitsInTimeslice", "fhNofRichHitsInTimeslice;# RICH hits / timeslice;Entries", 100, -0.5,
                     999.5);
  fHM->Create1<TH1D>("fhNofRichRingsInTimeslice", "fhNofRichRingsInTimeslice;# RICH rings / timeslice;Entries", 10,
                     -0.5, 9.5);

  // RICH hits
  fHM->Create2<TH2D>("fhRichHitXY", "fhRichHitXY;RICH hit X [cm];RICH hit Y [cm];Entries", 67, -20.1 + fXOffsetHisto,
                     20.1 + fXOffsetHisto, 84, -25.2, 25.2);
  fHM->Create2<TH2D>("fhEventRichHitXY", "fhEventRichHitXY;RICH hit X [cm];RICH hit Y [cm];Entries", 67,
                     -20.1 + fXOffsetHisto, 20.1 + fXOffsetHisto, 84, -25.2, 25.2);
  fHM->Create1<TH1D>("fhEventRichHitX", "fhEventRichHitX;RICH hit X [cm];Entries", 67, -20.1 + fXOffsetHisto,
                     20.1 + fXOffsetHisto);
  fHM->Create1<TH1D>("fhEventRichHitY", "fhEventRichHitY;RICH hit Y [cm];Entries", 84, -25.2, 25.2);
  fHM->Create1<TH1D>("fhRichHitX", "fhRichHitX;RICH hit X [cm];Entries", 67, -20.1 + fXOffsetHisto,
                     20.1 + fXOffsetHisto);
  fHM->Create1<TH1D>("fhRichHitY", "fhRichHitY;RICH hit Y [cm];Entries", 84, -25.2, 25.2);

  //ToT
  fHM->Create1<TH1D>("fhRichDigisToT", "fhRichDigisToT;ToT [ns];Entries", 601, 9.975, 40.025);
  fHM->Create1<TH1D>("fhRichHitToT", "fhRichHitToT;ToT [ns];Entries", 601, 9.975, 40.025);

  // RICH rings
  fHM->Create2<TH2D>("fhRichRingXY", "fhRichRingXY;Ring center X [cm];Ring center Y [cm];Entries", 100,
                     -20 + fXOffsetHisto, 20 + fXOffsetHisto, 100, -20, 20);
  fHM->Create1<TH1D>("fhRichRingRadius", "fhRichRingRadius;Ring radius [cm];Entries", 100, 0., 7.);
  fHM->Create1<TH1D>("fhNofHitsInRing", "fhNofHitsInRing;# hits in ring;Entries", 50, -0.5, 49.5);

  // RICH rings aerogel/ Event
  fHM->Create2<TH2D>("fhEventRichRingXY", "fhEventRichRingXY;Ring center X [cm];Ring center Y [cm];Entries", 100,
                     -20 + fXOffsetHisto, 20 + fXOffsetHisto, 100, -20, 20);
  fHM->Create1<TH1D>("fhEventRichRingRadius", "fhEventRichRingRadius;Ring radius [cm];Entries", 100, 0., 7.);
  fHM->Create1<TH1D>("fhEventNofHitsInRing", "fhEventNofHitsInRing;# hits in ring;Entries", 50, -0.5, 49.5);
  fHM->Create1<TH1D>("fhEventNofHitsInRingTop", "fhEventNofHitsInRingTop;# hits in ring;Entries", 50, -0.5, 49.5);
  fHM->Create1<TH1D>("fhEventNofHitsInRingBottom", "fhEventNofHitsInRingBottom;# hits in ring;Entries", 50, -0.5, 49.5);

  fHM->Create1<TH1D>("fhEventRichHitToT", "fhRichHitToT;ToT [ns];Entries", 601, 9.975, 40.025);

  fHM->Create1<TH1D>("fhEventRichRingRadiusTop", "fhEventRichRingRadiusTop;Ring radius [cm];Entries", 100, 0., 7.);
  fHM->Create1<TH1D>("fhEventRichRingRadiusBottom", "fhEventRichRingRadiusBottom;Ring radius [cm];Entries", 100, 0.,
                     7.);

  fHM->Create1<TH1D>("fhNofRingsTopBottom", "fhNofRingsTopBottom;Entries", 2, -0.5, 1.5);
}


void CbmRichMCbmAerogelAna::Exec(Option_t* /*option*/)
{
  fEventNum++;
  fHM->H1("fhNofEvents")->Fill(1);
  cout << "CbmRichMCbmAerogelAna, event No. " << fEventNum << endl;

  {
    for (int i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
      const CbmRichDigi* richDigi = fDigiMan->Get<CbmRichDigi>(i);
      fHM->H1("fhRichDigisToT")->Fill(richDigi->GetToT());
    }
  }

  int nofRichHits = fRichHits->GetEntriesFast();
  fHM->H1("fhNofRichHitsInTimeslice")->Fill(nofRichHits);
  fHM->H1("fhHitsInTimeslice")->Fill(fEventNum, nofRichHits);
  for (int iH = 0; iH < nofRichHits; iH++) {
    CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    fHM->H2("fhRichHitXY")->Fill(richHit->GetX(), richHit->GetY());
    fHM->H1("fhRichHitToT")->Fill(richHit->GetToT());
    fHM->H1("fhRichHitX")->Fill(richHit->GetX());
    fHM->H1("fhRichHitY")->Fill(richHit->GetY());
    //printf("HitToT: %f \n", richHit->GetToT());
  }


  //CBMEVENT
  auto fNCbmEvent = fCbmEvent->GetEntriesFast();

  for (int i = 0; i < fNCbmEvent; i++) {
    fHM->H1("fhNofCbmEvents")->Fill(1);
    CbmEvent* ev = static_cast<CbmEvent*>(fCbmEvent->At(i));
    std::vector<int> ringIndx;  // Rings in CbmEvent
    std::vector<int> evRichHitIndx;


    for (int j = 0; j < ev->GetNofData(ECbmDataType::kRichHit); j++) {
      auto iRichHit = ev->GetIndex(ECbmDataType::kRichHit, j);
      evRichHitIndx.push_back(iRichHit);

      int nofRichRings = fRichRings->GetEntriesFast();
      for (int l = 0; l < nofRichRings; l++) {
        CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(l));
        auto NofRingHits  = ring->GetNofHits();
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


    //DrawEvent(ev, ringIndx, 1);


    if (ringIndx.size() > 0) {  // Ring in CbmEvent
      //loop over all Hits in a CbmEvent with Ring:
      for (int j = 0; j < ev->GetNofData(ECbmDataType::kRichHit); j++) {
        auto iRichHit       = ev->GetIndex(ECbmDataType::kRichHit, j);
        CbmRichHit* richHit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
        fHM->H1("fhEventRichHitToT")->Fill(richHit->GetToT());
        fHM->H2("fhEventRichHitXY")->Fill(richHit->GetX(), richHit->GetY());
        fHM->H1("fhEventRichHitX")->Fill(richHit->GetX());
        fHM->H1("fhEventRichHitY")->Fill(richHit->GetY());
      }


      //loop over rings in CbmEvent
      for (unsigned int rings = 0; rings < ringIndx.size(); rings++) {
        CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(ringIndx[rings]));
        if (ring == nullptr) continue;

        fHM->H2("fhEventRichRingXY")->Fill(ring->GetCenterX(), ring->GetCenterY());
        fHM->H1("fhEventRichRingRadius")->Fill(ring->GetRadius());
        fHM->H1("fhEventNofHitsInRing")->Fill(ring->GetNofHits());

        // for now ignore overlap of hits in crossover region!
        if (ring->GetCenterY() >= 0.) {  // new Aerogel Block
          fHM->H1("fhEventNofHitsInRingTop")->Fill(ring->GetNofHits());
          fHM->H1("fhEventRichRingRadiusTop")->Fill(ring->GetRadius());
          fHM->H1("fhNofRingsTopBottom")->Fill(0);
        }
        else {  // Aerogel from Mar2019
          fHM->H1("fhEventNofHitsInRingBottom")->Fill(ring->GetNofHits());
          fHM->H1("fhEventRichRingRadiusBottom")->Fill(ring->GetRadius());
          fHM->H1("fhNofRingsTopBottom")->Fill(1);
        }
      }

      //DrawRichTofEv(RichTofEv);
      fHM->H1("fhNofCbmEventsRing")->Fill(1);
    }
  }  //End CbmEvent loop


  RichRings();
}

void CbmRichMCbmAerogelAna::RichRings()
{
  int nofRichRings = fRichRings->GetEntriesFast();
  //fHM->H1("fhNofRichRingsInTimeslice")->Fill(nofRichRings);
  for (int i = 0; i < nofRichRings; i++) {
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(i));
    if (ring == nullptr) continue;
    //DrawRing(ring);
    fHM->H2("fhRichRingXY")->Fill(ring->GetCenterX(), ring->GetCenterY());
    fHM->H1("fhRichRingRadius")->Fill(ring->GetRadius());
    fHM->H1("fhNofHitsInRing")->Fill(ring->GetNofHits());
  }
}


void CbmRichMCbmAerogelAna::DrawHist()
{
  cout.precision(4);

  //SetDefaultDrawStyle();
  double nofEvents = fHM->H1("fhNofCbmEvents")->GetEntries();
  fHM->ScaleByPattern("fh_.*", 1. / nofEvents);

  {
    fHM->CreateCanvas("rich_mcbm_fhNofCbmEvents", "rich_mcbm_fhNofCbmEvents", 600, 600);
    DrawH1(fHM->H1("fhNofCbmEvents"));
  }

  {
    fHM->CreateCanvas("rich_mcbm_fhNofCbmEventsRing", "rich_mcbm_fhNofCbmEventsRing", 600, 600);
    DrawH1(fHM->H1("fhNofCbmEventsRing"));
  }

  {
    fHM->CreateCanvas("rich_mcbm_fhNofEvents", "rich_mcbm_fhNofEvents", 600, 600);
    DrawH1(fHM->H1("fhNofEvents"));
  }


  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_XY", "rich_mcbm_XY", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH2(fHM->H2("fhRichHitXY"));
    c->cd(2);
    DrawH2(fHM->H2("fhRichRingXY"));
  }


  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_XY_inEvent", "rich_mcbm_XY_inEvent", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH2(fHM->H2("fhEventRichHitXY"));
    c->cd(2);
    DrawH2(fHM->H2("fhEventRichRingXY"));
  }


  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_X_and_Y_inEvent", "rich_mcbm_X_and_Y_inEvent", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRichHitX"));
    c->cd(2);
    DrawH1(fHM->H1("fhRichHitY"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_X_and_Y_inEventWithRing", "rich_mcbm_X_and_Y_inEventWithRing", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhEventRichHitX"));
    c->cd(2);
    DrawH1(fHM->H1("fhEventRichHitY"));
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
    TCanvas* c = fHM->CreateCanvas("rich_ToT_Event", "rich_ToT_Event", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRichHitToT"));
    c->cd(2);
    DrawH1(fHM->H1("fhEventRichHitToT"));
  }


  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_rings", "rich_mcbm_rings", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhRichRingRadius"));
    c->cd(2);
    DrawH1(fHM->H1("fhNofHitsInRing"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_mcbm_rings_inEvent", "rich_mcbm_rings_inEvent", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhEventRichRingRadius"));
    c->cd(2);
    DrawH1(fHM->H1("fhEventNofHitsInRing"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_Aerogel_Top_Bottom_Hits", "rich_Aerogel_Top_Bottom_Hits", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhEventNofHitsInRingTop"));
    unsigned int sumHitsTop = 0;
    for (unsigned int i = 1; i <= 50; i++) {
      sumHitsTop += fHM->H1("fhEventNofHitsInRingTop")->GetBinContent(i) * (i - 1);
    }
    std::cout << "Sum Hits Top: " << sumHitsTop << std::endl;

    c->cd(2);
    DrawH1(fHM->H1("fhEventNofHitsInRingBottom"));
    unsigned int sumHitsBottom = 0;
    for (unsigned int i = 1; i <= 50; i++) {
      sumHitsBottom += fHM->H1("fhEventNofHitsInRingBottom")->GetBinContent(i) * (i - 1);
    }
    std::cout << "Sum Hits Bottom: " << sumHitsBottom << std::endl;
  }

  {
    TCanvas* c = fHM->CreateCanvas("rich_Aerogel_Top_Bottom_Radius", "rich_Aerogel_Top_Bottom_Radius", 1200, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhEventRichRingRadiusTop"));
    c->cd(2);
    DrawH1(fHM->H1("fhEventRichRingRadiusBottom"));
  }


  {
    fHM->CreateCanvas("rich_Aerogel_#RingsTopVsBottom", "rich_Aerogel_#RingsTopVsBottom", 1200, 600);
    fHM->H1("fhNofRingsTopBottom")->Draw("HIST TEXT");
  }
}


void CbmRichMCbmAerogelAna::Finish()
{
  //std::cout<<"Tracks:  "<< fTofTracks->GetEntriesFast() <<std::endl;
  std::cout << "Drawing Hists...";
  DrawHist();
  std::cout << "DONE!" << std::endl;

  if (this->fDoDrawCanvas) {
    fHM->SaveCanvasToImage(fOutputDir, "png");
    std::cout << "Canvas saved to Images!" << std::endl;
  }

  if (this->fDoWriteHistToFile) {
    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile    = gFile;
    TDirectory* oldir = gDirectory;

    std::string s  = fOutputDir + "/RecoHists.root";
    TFile* outFile = new TFile(s.c_str(), "RECREATE");
    if (outFile->IsOpen()) {
      fHM->WriteToFile();
      std::cout << "Written to Root-file \"" << s << "\"  ...";
      outFile->Close();
      std::cout << "Done!" << std::endl;
    }
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile = oldFile;
    gDirectory->cd(oldir->GetPath());
  }
}


void CbmRichMCbmAerogelAna::DrawFromFile(const string& fileName, const string& outputDir)
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


bool CbmRichMCbmAerogelAna::doToT(CbmRichHit* hit)
{
  bool check = false;
  if ((hit->GetToT() > 23.7) && (hit->GetToT() < 30.0)) check = true;

  return check;
}


Bool_t CbmRichMCbmAerogelAna::cutRadius(CbmRichRing* ring)
{
  if (ring->GetRadius() > 1. && ring->GetRadius() < 100.) return true;

  return false;
}


ClassImp(CbmRichMCbmAerogelAna)
