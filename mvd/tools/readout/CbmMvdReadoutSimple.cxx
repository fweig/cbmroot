// -------------------------------------------------------------------------
// -----                  CbmMvdReadoutSimple source file              -----
// -----                  Created 17/10/16 by P. Sitzmann              -----
// -------------------------------------------------------------------------

#include "CbmMvdReadoutSimple.h"

#include "CbmMCTrack.h"
#include "CbmMvdPoint.h"

#include "CbmMvdDetector.h"
#include "CbmMvdSensor.h"

#include "tools/CbmMvdGeoHandler.h"


//-- Include from Fair --//
#include "FairLogger.h"

#include "FairTrackParam.h"


//-- Include from Root --//
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include <TFile.h>

#include <iostream>

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
CbmMvdReadoutSimple::CbmMvdReadoutSimple()
  : CbmMvdReadoutSimple::CbmMvdReadoutSimple("MvdReadoutSimple", 0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdReadoutSimple::CbmMvdReadoutSimple(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , foutFile(nullptr)
  , fshow(kFALSE)
  , fMvdMCBank()
  , fMvdMCHitsStations()
  , fWordsPerRegion(nullptr)
  , fWordsPerRegion2(nullptr)
  , fWordsPerWorstRegion(nullptr)
  , fWordsPerSuperRegion(nullptr)
  , fWorstSuperPerEvent(nullptr)
  , fMvdBankDist(nullptr)
  , fMvdMCWorst(nullptr)
  , fMvdMCWorstDelta(nullptr)
  , fMvdDataLoadPerSensor(nullptr)
  , fMvdDataLoadHotSensor(nullptr)
  , fMvdDataPerRegion()
  , fMvdDataPerSuperRegion()
  , fMcPoints()
  , fListMCTracks()
  , fEventNumber(0) {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdReadoutSimple::~CbmMvdReadoutSimple() { ; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
InitStatus CbmMvdReadoutSimple::Init() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) { LOG(fatal) << "RootManager not instantised!"; }

  fMcPoints = (TClonesArray*) ioman->GetObject("MvdPoint");  // PileUp Mc points
  fListMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");

  if (!fMcPoints) LOG(fatal) << "Mvd Pile Up Mc array missing";

  SetupHistograms();

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutSimple::SetupHistograms() {
  for (Int_t i = 0; i < 63; i++) {
    fMvdMCBank[i] =
      new TH2F(Form("fMvdMCBank%d", i),
               "Mvd mc distribution worst spot only delta electrons",
               300,
               -2,
               0,
               1500,
               3.5,
               0);
  }

  fMvdMCHitsStations[0] = new TH2F(
    "fMvdMCStation0", "Mvd mc distribution", 2, -2.5, -0.5, 3, -0.5, 2.5);
  fMvdMCHitsStations[1] = new TH2F(
    "fMvdMCStation1", "Mvd mc distribution", 4, -4.5, -0.5, 6, -0.5, 5.5);
  fMvdMCHitsStations[2] = new TH2F(
    "fMvdMCStation2", "Mvd mc distribution", 6, -7.5, -1.5, 9, -1.5, 7.5);
  fMvdMCHitsStations[3] = new TH2F(
    "fMvdMCStation3", "Mvd mc distribution", 8, -9.5, -1.5, 12, -1.5, 10.5);

  fWordsPerRegion =
    new TH1F("fWordsPerRegion", "Words send to a region", 65, 0, 64);
  fWordsPerRegion->GetXaxis()->SetTitle("Regionnumber");
  fWordsPerRegion->GetYaxis()->SetTitle("Average Entries per Region");

  fWordsPerRegion2 =
    new TH2F("fWordsPerRegion2",
             "Words send to a region, errors sigma of gauss fit",
             64,
             0,
             63,
             150,
             0,
             15);
  fWordsPerRegion2->GetXaxis()->SetTitle("Regionnumber");
  fWordsPerRegion2->GetYaxis()->SetTitle("Average Entries per Region");

  fWordsPerWorstRegion = new TH1F(
    "fWordsPerWorstRegion", "Most worst send to a region per Event", 65, 0, 64);
  fWordsPerWorstRegion->GetXaxis()->SetTitle("words");
  fWordsPerWorstRegion->GetYaxis()->SetTitle("Entries");

  fWordsPerSuperRegion = new TH1F(
    "fWordsPerSuperRegion", "Words send to a super region", 1000, 0, 400);
  fWordsPerSuperRegion->GetXaxis()->SetTitle("words");
  fWordsPerSuperRegion->GetYaxis()->SetTitle("Entries");

  fWorstSuperPerEvent = new TH1F("fWorstSuperRegion",
                                 "Most words send to super region per Event",
                                 1000,
                                 0,
                                 400);
  fWorstSuperPerEvent->GetXaxis()->SetTitle("words");
  fWorstSuperPerEvent->GetYaxis()->SetTitle("Entries");

  fMvdMCWorst = new TH2F(
    "fMvdMCWorst", "Mvd mc distribution worst spot", 300, -2, 0, 1500, 3.5, 0);
  fMvdMCWorst->GetYaxis()->SetTitle("y [cm]");
  fMvdMCWorst->GetXaxis()->SetTitle("x [cm]");

  fMvdMCWorstDelta =
    new TH2F("fMvdMCWorstDelta",
             "Mvd mc distribution worst spot only delta electrons",
             300,
             -2,
             0,
             1500,
             3.5,
             0);
  fMvdMCWorstDelta->GetYaxis()->SetTitle("y [cm]");
  fMvdMCWorstDelta->GetXaxis()->SetTitle("x [cm]");

  fMvdDataLoadPerSensor =
    new TH1I("fMvdDataLoadPerSensor", "Mvd Dataload per Sensor", 300, 0, 300);
  fMvdDataLoadPerSensor->GetXaxis()->SetTitle("Sensor number");
  fMvdDataLoadPerSensor->GetYaxis()->SetTitle("Entries");

  fMvdDataLoadHotSensor = new TH1I(
    "fMvdDataLoadHotSensor", "Mvd Dataload in worst Sensor", 2000, 0, 2000);
  fMvdDataLoadHotSensor->GetXaxis()->SetTitle("number of words");
  fMvdDataLoadHotSensor->GetYaxis()->SetTitle("Entries");

  fMvdBankDist =
    new TH2I("fMvdBankDist", "Avarage Hits per Region", 63, 0, 63, 50, 0, 50);
  fMvdBankDist->GetXaxis()->SetTitle("Region number");
  fMvdBankDist->GetYaxis()->SetTitle("Entries");

  for (Int_t i = 0; i < 64; i++) {
    fMvdDataPerRegion[i] = new TH1F(Form("fMvdDataPerRegion[%d]", i),
                                    Form("Words send to region %d", i),
                                    100,
                                    0,
                                    100);
    fMvdDataPerRegion[i]->GetXaxis()->SetTitle("Words");
    fMvdDataPerRegion[i]->GetYaxis()->SetTitle("Entries");
  }


  for (Int_t i = 0; i < 16; i++) {
    fMvdDataPerSuperRegion[i] =
      new TH1F(Form("fMvdDataPerSuperRegion[%d]", i),
               Form("Words send to superregion %d", i),
               400,
               0,
               400);
    fMvdDataPerSuperRegion[i]->GetXaxis()->SetTitle("Words");
    fMvdDataPerSuperRegion[i]->GetYaxis()->SetTitle("Entries");
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdReadoutSimple::Exec(Option_t* /*opt*/) {
  fEventNumber++;

  Float_t wordsPerRegion[64] = {0};
  Float_t wordsPerSuper[16]  = {0};

  Float_t yPosMin = -0.73;
  Float_t yPosMax = 2.5;

  for (Int_t k = 0; k < fMcPoints->GetEntriesFast(); k++) {
    CbmMvdPoint* curPoint = (CbmMvdPoint*) fMcPoints->At(k);
    fMvdDataLoadPerSensor->Fill(curPoint->GetStationNr(), 1.5);

    if (curPoint->GetZ() < 8) {
      if (curPoint->GetX() > -1.93 && curPoint->GetX() <= -0.55
          && curPoint->GetY() >= yPosMin && curPoint->GetY() <= yPosMax) {
        fMvdMCWorst->Fill(curPoint->GetX(), curPoint->GetY());
        if (curPoint->GetTrackID() == -3)
          fMvdMCWorstDelta->Fill(curPoint->GetX(), curPoint->GetY());
        for (Int_t nRegion = 0; nRegion < 64; nRegion++) {
          if (curPoint->GetY() >= (yPosMin + (nRegion * 0.05))
              && curPoint->GetY() < (yPosMin + ((nRegion + 1) * 0.05))) {
            fMvdMCBank[nRegion]->Fill(curPoint->GetX(), curPoint->GetY());
            wordsPerRegion[nRegion] = wordsPerRegion[nRegion] + 1.5;
            if (wordsPerRegion[nRegion] > 100)
              LOG(info) << " Region " << nRegion << " has an overflow";
            fWordsPerRegion->Fill(nRegion, 1.5);
            break;
          }
        }
      }
    }
    if (curPoint->GetZ() < 8) {
      if (curPoint->GetX() < -0.5 && curPoint->GetY() > -0.5)
        fMvdMCHitsStations[0]->Fill(curPoint->GetX(), curPoint->GetY());
    } else if (curPoint->GetZ() < 13) {
      if (curPoint->GetX() < -0.5 && curPoint->GetY() > -1.5)
        fMvdMCHitsStations[1]->Fill(curPoint->GetX(), curPoint->GetY());
    } else if (curPoint->GetZ() < 18) {
      if (curPoint->GetX() < -1.5 && curPoint->GetY() > -1.5)
        fMvdMCHitsStations[2]->Fill(curPoint->GetX(), curPoint->GetY());
    } else {
      if (curPoint->GetX() < -1.5 && curPoint->GetY() > -1.5)
        fMvdMCHitsStations[3]->Fill(curPoint->GetX(), curPoint->GetY());
    }
  }
  LOG(info) << "//--------------- New Event " << fEventNumber
            << " -----------------------\\";

  Int_t i               = 0;
  Int_t wordsInWorst    = 0;
  Int_t wordsInWorstReg = 0;
  Int_t wordsTotal      = 0;

  for (Int_t supReg = 0; supReg < 16; supReg++) {
    for (Int_t k = 0; k < 4; k++) {
      wordsPerSuper[supReg] += wordsPerRegion[i];
      fMvdDataPerRegion[i]->Fill(wordsPerRegion[i]);
      if (wordsPerRegion[i] > wordsInWorstReg)
        wordsInWorstReg = wordsPerRegion[i];
      LOG(debug) << "Words in Region " << i << ": " << wordsPerRegion[i];
      i++;
    }

    LOG(debug) << " Words in super region " << supReg << ": "
               << wordsPerSuper[supReg];
    if (wordsPerSuper[supReg] > 400)
      LOG(info) << "SuperRegion " << supReg << " has an overflow";
    fWordsPerSuperRegion->Fill(wordsPerSuper[supReg]);
    fMvdDataPerSuperRegion[supReg]->Fill(wordsPerSuper[supReg]);

    if (wordsPerSuper[supReg] > wordsInWorst)
      wordsInWorst = wordsPerSuper[supReg];

    wordsTotal += wordsPerSuper[supReg];
  }

  fWorstSuperPerEvent->Fill(wordsInWorst);
  fWordsPerWorstRegion->Fill(wordsInWorstReg);
  fMvdDataLoadHotSensor->Fill(wordsTotal);

  LOG(info) << "//--------------- End Event -----------------------\\";
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutSimple::Finish() {
  foutFile->cd();

  Float_t scale = 1. / (Float_t) fEventNumber;
  for (Int_t iPad = 0; iPad < 4; iPad++) {
    fMvdMCHitsStations[iPad]->Scale(scale);
  }
  fMvdDataLoadPerSensor->Scale(scale);
  fWordsPerRegion->Scale(scale);


  if (fshow) DrawHistograms();
  WriteHistograms();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutSimple::WriteHistograms() {

  for (Int_t iPad = 0; iPad < 4; iPad++) {
    fMvdMCHitsStations[iPad]->Write();
  }

  fMvdDataLoadPerSensor->Write();

  fMvdDataLoadHotSensor->Write();

  fWordsPerSuperRegion->Write();

  fWorstSuperPerEvent->Write();

  fWordsPerRegion->Write();

  fWordsPerWorstRegion->Write();

  for (Int_t i = 0; i < 64; i++) {
    fMvdDataPerRegion[i]->Fit("gaus");
    TF1* func       = fMvdDataPerRegion[i]->GetFunction("gaus");
    Double_t param0 = func->GetParameter(0);
    Double_t param1 = func->GetParameter(1);
    Double_t param2 = func->GetParameter(2);
    Double_t chi2   = func->GetChisquare();
    cout << " // - " << i << " -- param 0 = " << param0
         << " -- param 1 = " << param1 << " -- param 2 = " << param2
         << " -- chi2 = " << chi2 << endl;
    fWordsPerRegion2->Fill(i, param1);
    fWordsPerRegion2->SetBinError(i, 0, param1, param2);
    fMvdDataPerRegion[i]->Write();
  }
  fWordsPerRegion2->Write();

  for (Int_t i = 0; i < 16; i++)
    fMvdDataPerSuperRegion[i]->Write();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutSimple::DrawHistograms() {
  TCanvas* mcCanvas1 = new TCanvas();
  mcCanvas1->Divide(2, 2);
  for (Int_t iPad = 0; iPad < 4; iPad++) {
    mcCanvas1->cd(iPad + 1);
    fMvdMCHitsStations[iPad]->Draw("COLZ");
  }

  TCanvas* dataLoad = new TCanvas();
  dataLoad->Divide(2, 1);
  dataLoad->cd(1);
  fMvdDataLoadPerSensor->Draw("BAR");
  dataLoad->cd(2);
  fMvdDataLoadHotSensor->Draw("BAR");

  TCanvas* mcCanvas2 = new TCanvas();
  mcCanvas2->Divide(1, 2);
  mcCanvas2->cd(1);
  fWordsPerSuperRegion->Draw();
  mcCanvas2->cd(2);
  fWorstSuperPerEvent->Draw();

  /*
TCanvas* mcCanvas3 = new TCanvas();
fMvdMCHitsStations[0]->Draw("COLZ");

TCanvas* mcCanvas4 = new TCanvas();
mcCanvas4->Divide(8,8);
for(Int_t pad = 0; pad < 63; pad++)
  {
      mcCanvas4->cd(pad+1);
      fMvdMCBank[pad]->Draw("COL");
      fMvdMCBank[pad]->Write();
      cout << "Bank " << pad << " avarage Entries " << fMvdMCBank[pad]->GetEntries()/fEventNumber << endl;
      fMvdBankDist->Fill(pad, fMvdMCBank[pad]->GetEntries()/fEventNumber);
  }
TCanvas* mcCanvas5 = new TCanvas();
fMvdBankDist->Draw();

TCanvas* mcCanvas6 = new TCanvas();
fMvdMCWorst->Draw("COL");
fMvdMCWorst->Write();
*/

  TCanvas* regionCanvas = new TCanvas();
  regionCanvas->Divide(1, 2);
  regionCanvas->cd(1);
  fWordsPerRegion->Draw();
  regionCanvas->cd(2);
  fWordsPerRegion2->Draw();

  TCanvas* regionsCanvas[64];
  TCanvas* supregionsCanvas[16];

  for (Int_t i = 0; i < 64; i++) {
    regionsCanvas[i] = new TCanvas();
    regionsCanvas[i]->cd();
    fMvdDataPerRegion[i]->Draw();
  }

  for (Int_t i = 0; i < 16; i++) {
    supregionsCanvas[i] = new TCanvas();
    supregionsCanvas[i]->cd();
    fMvdDataPerSuperRegion[i]->Draw();
  }
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdReadoutSimple);
