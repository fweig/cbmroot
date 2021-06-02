/** @file CbmRichMCbmSEDisplay.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmRichMCbmSEDisplay.h"

#include "CbmRichHit.h"
#include "CbmRichRing.h"
#include "CbmTofTracklet.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TH1D.h"
#include "TLine.h"
#include "TMarker.h"
#include "TPad.h"
#include "TSystem.h"
#include <TBox.h>

#include <sstream>
#include <string>


CbmRichMCbmSEDisplay::CbmRichMCbmSEDisplay()
  : fRichHits(nullptr)
  , fRichRings(nullptr)
  , fTofTracks(nullptr)
  , fXOffsetHisto(0.)
  , fTotRichMin(23.7)
  , fTotRichMax(30.)
  , fNofDrawnEvents(0)
  , fMaxNofDrawnEvents(100)
  , fOutputDir("result")
  , fHM(nullptr)
{
}

CbmRichMCbmSEDisplay::CbmRichMCbmSEDisplay(CbmHistManager* manager)
  : fRichHits(nullptr)
  , fRichRings(nullptr)
  , fTofTracks(nullptr)
  , fXOffsetHisto(0.)
  , fTotRichMin(23.7)
  , fTotRichMax(30.)
  , fNofDrawnEvents(0)
  , fMaxNofDrawnEvents(100)
  , fOutputDir("result")
  , fHM(manager)
{
}


void CbmRichMCbmSEDisplay::DrawEvent(CbmEvent* ev, std::vector<int>& ringIndx, bool full = true)
{

  // ---- General Size of PMTs [cm] -------------------------------------------------
  double pmtWidth  = 5.20;
  double pmtHeight = 5.20;
  double pmtGap    = 0.10;
  // --------------------------------------------------------------------------------


  // ---- Positioning ---------------------------------------------------------------
  double left = 0.0;
  double top  = 0.0;
  if (full == true) {
    left = -16.85 + fXOffsetHisto + 6.225;
    top  = 23.7;
  }
  // -------------------------------------------------------------------------------


  // ---- Limiting Output ----------------------------------------------------------
  if (fNofDrawnEvents > fMaxNofDrawnEvents) return;
  // -------------------------------------------------------------------------------


  // ---- Throw away too clean events ----------------------------------------------
  if (ev->GetNofData(ECbmDataType::kRichHit) <= 4) return;
  // -------------------------------------------------------------------------------


  // ---- Init Canvas Layout for Event ---------------------------------------------
  fNofDrawnEvents++;
  std::string dir = fOutputDir + "/png/" + fFileName + "/";
  gSystem->mkdir(dir.c_str(), true);  // create directory if it does not exist
  //std::cout<<"makeDir: "<<dir.c_str() << "   "<<gSystem->mkdir(dir.c_str(), true)<<std::endl;

  stringstream ss;
  ss << fFileName << "/CbmEvent" << fNofDrawnEvents;
  TCanvas* c = nullptr;
  if (full == true) { c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 600, 1250); }
  else {
    c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
  }
  c->SetGrid(true, true);
  TPad* pad_event = new TPad("pad_event", "event", 0, 0.20, 1, 1);
  TH2D* pad       = nullptr;
  if (full == true) {
    pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -18. + fXOffsetHisto + 6.225,
                   5. + fXOffsetHisto + 6.225, 1, -26., 26.);
  }
  else {
    pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -15. + fXOffsetHisto + 6.225,
                   10. + fXOffsetHisto + 6.225, 1, -5., 20);
  }

  TPad* pad_time           = new TPad("pad_time", "timeDist", 0, 0, 1, 0.20);
  TH1D* timeDistRichHit    = new TH1D((ss.str() + "timeDistRichHit").c_str(), ";LE [ns];Entries", 200, 0.0, 200.);
  TH1D* timeDistRichHitToT = new TH1D((ss.str() + "timeDistRichHitToT").c_str(), ";LE [ns];Entries", 200, 0.0, 200.);
  TH1D* timeDistTofTrack   = new TH1D((ss.str() + "timeDistTofTrack").c_str(), ";LE [ns];Entries", 200, 0.0, 200.);
  pad_event->Draw();
  pad_time->Draw();
  pad_event->cd();
  //pad_event->SetBottomMargin(0);
  pad->SetStats(false);
  pad->Draw();

  if (full == true) {
    for (unsigned int x = 0; x < 4; ++x) {
      for (unsigned int y = 0; y < 9; ++y) {
        double pmtLeft = left + (pmtWidth + pmtGap) * x;
        double pmtTop  = top - (pmtHeight + pmtGap) * y;
        TBox* box      = new TBox(pmtLeft, pmtTop, pmtLeft + pmtWidth, pmtTop - pmtHeight);
        //box->SetFillColorAlpha(8,0.2);
        box->Draw();

        pmtLeft += 0.175;
        pmtTop -= 0.175;
        for (unsigned int pX = 0; pX < 8; ++pX) {
          for (unsigned int pY = 0; pY < 8; ++pY) {
            double xStart = 0.0;
            double xEnd   = 0.0;
            double yStart = 0.0;
            double yEnd   = 0.0;
            if (pX == 0) {
              xStart = pmtLeft;
              xEnd   = pmtLeft + 0.625;
            }
            else if (pX < 7) {
              xStart = pmtLeft + 0.625 + 0.6 * (pX - 1);
              xEnd   = pmtLeft + 0.625 + 0.6 * (pX);
            }
            else if (pX == 7) {
              xStart = pmtLeft + 0.625 + 0.6 * 6;
              xEnd   = pmtLeft + 0.625 * 2 + 0.6 * 6;
            }

            if (pY == 0) {
              yStart = pmtTop;
              yEnd   = pmtTop - 0.625;
            }
            else if (pY < 7) {
              yStart = pmtTop - 0.625 - 0.6 * (pY - 1);
              yEnd   = pmtTop - 0.625 - 0.6 * (pY);
            }
            else if (pY == 7) {
              yStart = pmtTop - 0.625 - 0.6 * 6;
              yEnd   = pmtTop - 0.625 * 2 - 0.6 * 6;
            }

            TBox* box1 = new TBox(xStart, yStart, xEnd, yEnd);
            box1->SetLineWidth(1.);
            //box1->SetFillColorAlpha(30.,0.1);
            //box1->SetLineColorAlpha(30.,0.5);

            box1->Draw("l");
          }
        }
      }
    }
  }

  if (full == true) {
    //rough Drawing of RichDetectorAcceptance
    /*
        TLine *line0 = new TLine( left ,    top, right,    top);
        line0->Draw();
        TLine *line1 = new TLine( right,    top, right, bottom);
        line1->Draw();
        TLine *line2 = new TLine( right, bottom,  left, bottom);
        line2->Draw();
        TLine *line3 = new TLine( left , bottom,  left,    top);
        line3->Draw();
        */
  }
  // -------------------------------------------------------------------------------


  // find min and max x and y positions of the hits
  // in order to shift drawing


  double hitZ      = 0;
  uint nofDrawHits = 0;

  // ---- Draw Rich Hits -----------------------------------------------------------
  for (int j = 0; j < ev->GetNofData(ECbmDataType::kRichHit); j++) {
    auto iRichHit   = ev->GetIndex(ECbmDataType::kRichHit, j);
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iRichHit));
    if (nullptr == hit) continue;
    TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), .25);
    timeDistRichHit->Fill(hit->GetTime() - ev->GetStartTime());
    if (doToT(hit)) {  // Good ToT selection
      hitDr->SetFillColor(kCyan);
      timeDistRichHitToT->Fill(hit->GetTime() - ev->GetStartTime());
    }
    else {
      hitDr->SetFillColor(kBlue);
    }
    hitZ += hit->GetZ();
    nofDrawHits++;
    hitDr->Draw();
  }
  // -------------------------------------------------------------------------------


  // ---- Draw Rich Rings and Ring Hits --------------------------------------------
  for (unsigned int rings = 0; rings < ringIndx.size(); rings++) {
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(ringIndx[rings]));

    //Draw Ring
    TEllipse* circle = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), ring->GetRadius());
    circle->SetFillStyle(0);
    circle->SetLineWidth(3);
    circle->Draw();
    TEllipse* center = new TEllipse(ring->GetCenterX(), ring->GetCenterY(), .1);
    center->Draw();

    // Draw Ring Hits
    for (int i = 0; i < ring->GetNofHits(); i++) {
      Int_t hitInd    = ring->GetHit(i);
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
      if (nullptr == hit) continue;
      TEllipse* hitDr = new TEllipse(hit->GetX(), hit->GetY(), .125);
      if (doToT(hit)) {  // Good ToT selection
        hitDr->SetFillColor(kMagenta);
      }
      else {
        hitDr->SetFillColor(kRed);
      }
      //hitZ += hit->GetZ();
      //nofDrawHits++;
      hitDr->Draw();
    }
  }
  hitZ /= nofDrawHits;
  // -------------------------------------------------------------------------------

  // ---- Draw Tracks in RICH Plane ------------------------------------------------
  auto nofTofTracks = ev->GetNofData(ECbmDataType::kTofTrack);
  for (int j = 0; j < nofTofTracks; j++) {
    auto iTofTrack        = ev->GetIndex(ECbmDataType::kTofTrack, j);
    CbmTofTracklet* track = static_cast<CbmTofTracklet*>(fTofTracks->At(iTofTrack));
    if (nullptr == track) continue;
    timeDistTofTrack->Fill(track->GetTime() - ev->GetStartTime());
    TEllipse* hitDr = new TEllipse(track->GetFitX(hitZ), track->GetFitY(hitZ), .25);
    hitDr->SetFillColor(kGreen);
    hitDr->Draw();
  }
  // -------------------------------------------------------------------------------


  // ---- Draw LE time distribution ------------------------------------------------
  pad_time->cd();
  pad_time->SetTitle("");
  pad_time->SetTopMargin(0);
  pad_time->SetBottomMargin(0.25);
  timeDistRichHit->SetFillColor(kBlue);
  timeDistRichHit->SetStats(false);
  timeDistRichHit->GetXaxis()->SetLabelSize(0.06);
  timeDistRichHit->GetXaxis()->SetTitleSize(0.08);
  timeDistRichHit->GetYaxis()->SetLabelSize(0.06);
  timeDistRichHit->GetYaxis()->SetTitleSize(0.08);
  timeDistRichHit->GetYaxis()->SetTitleOffset(0.52);
  timeDistRichHit->Draw("HIST");
  timeDistRichHitToT->SetFillColor(kCyan);
  timeDistRichHitToT->Draw("HIST SAME");
  timeDistTofTrack->SetFillColor(kGreen);
  timeDistTofTrack->Draw("HIST SAME");
  // -------------------------------------------------------------------------------
}
