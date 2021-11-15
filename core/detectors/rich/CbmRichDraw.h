/* Copyright (C) 2016-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Andrey Lebedev [committer], Florian Uhlig */

#ifndef RICH_CbmRichDraw
#define RICH_CbmRichDraw


#include "CbmRichUtil.h"

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
      std::vector<Double_t> yPmtBins = CbmRichUtil::GetPmtHistYbins();
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
      std::vector<Double_t> yPmtBins = CbmRichUtil::GetPmtHistYbins();
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
};

#endif
