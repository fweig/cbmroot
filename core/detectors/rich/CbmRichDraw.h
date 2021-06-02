/** @file CbmRichDraw.h
  * @copyright Copyright (C) 2016-2020 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev, Andrey Lebedev [committer], Florian Uhlig **/

#ifndef RICH_CbmRichDraw
#define RICH_CbmRichDraw

#include "CbmRichDetectorData.h"    // for CbmRichPmtData, CbmRichPixelData
#include "CbmRichDigiMapManager.h"  // for CbmRichDigiMapManager
#include "CbmRichGeoManager.h"      // for CbmRichGeoManager

#include <RtypesCore.h>  // for ROOT data types
#include <TCanvas.h>     // for TCanvas
#include <TGraph2D.h>    // for TGraph2D
#include <TH2.h>         // for TH2D
#include <TPad.h>        // for gPad
#include <TVector3.h>    // for TVector3

#include <vector>  // for vector

class CbmRichDraw {

public:
  static void DrawPmtH2(TH2* h, TCanvas* c, Bool_t usePmtBins = false)
  {
    if (c == nullptr) return;
    c->Divide(1, 2);
    c->cd(1);
    TH2D* hUp = (TH2D*) h->Clone();
    DrawH2(hUp);
    if (usePmtBins) {
      std::vector<Double_t> yPmtBins = CbmRichDraw::GetPmtHistYbins();
      hUp->GetYaxis()->SetRange(yPmtBins.size() / 2 + 1, yPmtBins.size());
    }
    else {
      hUp->GetYaxis()->SetRangeUser(120, 210);
    }
    hUp->GetYaxis()->SetTitleOffset(0.75);
    hUp->GetZaxis()->SetTitleOffset(0.87);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.15);
    c->cd(2);
    TH2D* hDown = (TH2D*) h->Clone();
    if (usePmtBins) {
      std::vector<Double_t> yPmtBins = CbmRichDraw::GetPmtHistYbins();
      hDown->GetYaxis()->SetRange(0, yPmtBins.size() / 2 - 1);
    }
    else {
      hDown->GetYaxis()->SetRangeUser(-210, -120);
    }
    DrawH2(hDown);
    hDown->GetYaxis()->SetTitleOffset(0.75);
    hDown->GetZaxis()->SetTitleOffset(0.87);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.15);
  }

  static void DrawPmtGraph2D(TGraph2D* gUp, TGraph2D* gDown, TCanvas* c)
  {
    if (c == nullptr) return;
    c->Divide(1, 2);
    c->cd(1);
    DrawGraph2D(gUp);
    gUp->GetYaxis()->SetTitleOffset(0.75);
    gUp->GetZaxis()->SetTitleOffset(0.87);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.15);
    c->cd(2);
    DrawGraph2D(gDown);
    gDown->GetYaxis()->SetTitleOffset(0.75);
    gDown->GetZaxis()->SetTitleOffset(0.87);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.15);
  }

  static std::vector<Double_t> GetPmtHistXbins() { return CbmRichDraw::GetPmtHistBins(true); }

  static std::vector<Double_t> GetPmtHistYbins() { return CbmRichDraw::GetPmtHistBins(false); }

private:
  static std::vector<Double_t> GetPmtHistBins(Bool_t isX)
  {
    std::vector<Double_t> initVec;
    std::vector<Int_t> pmts = CbmRichDigiMapManager::GetInstance().GetPmtIds();
    for (Int_t pmtId : pmts) {
      CbmRichPmtData* pmtData = CbmRichDigiMapManager::GetInstance().GetPmtDataById(pmtId);
      TVector3 inPos(pmtData->fX, pmtData->fY, pmtData->fZ);
      TVector3 outPos;
      CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
      if (isX) {
        initVec.push_back(outPos.X() - 0.5 * pmtData->fWidth);
        initVec.push_back(outPos.X() + 0.5 * pmtData->fWidth);
      }
      else {
        initVec.push_back(outPos.Y() - 0.5 * pmtData->fHeight);
        initVec.push_back(outPos.Y() + 0.5 * pmtData->fHeight);
      }
    }
    sort(initVec.begin(), initVec.end());

    std::vector<Double_t> uniVec;
    for (unsigned int i = 0; i < initVec.size(); i++) {
      if (i == 0) uniVec.push_back(initVec[i]);
      if (initVec[i] - uniVec[uniVec.size() - 1] > 0.000001) uniVec.push_back(initVec[i]);
    }

    //        cout << "uniVec.size():" << uniVec.size() << endl;
    //        for (int i = 0; i < uniVec.size(); i++) {
    //            cout << std::setprecision(9);
    //            cout << fixed;
    //            cout << uniVec[i] << " " ;
    //        }
    //        cout << endl;

    return uniVec;
  }
};

#endif
