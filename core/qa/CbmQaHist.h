/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/// \file   CbmQaHist.h
/// \brief  Definition of the CbmQaHist class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   22.11.2020

#ifndef CbmQaHist_H
#define CbmQaHist_H

#include "CbmQaCanvas.h"

#include <FairLogger.h>

#include <TFitResultPtr.h>
#include <TPaveStats.h>
#include <TStyle.h>
#include <TVirtualPad.h>

// The TH* headers are needed here for the ROOT linker

#include <TF1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TProfile.h>
#include <TProfile2D.h>

/// A modification of TH* and TProfile* classes that keeps statistics & fit drawing options
/// and resizes the stat window accordingly without the actual drawing.
/// In the original classes, hist->Draw() & canv->Update() must be called
/// for resetting Stat/Fit window.
///
template<class HistTypeT>
class CbmQaHist : public HistTypeT {
public:
  /// Default constructor only for the streamer
  CbmQaHist() : HistTypeT()
  {
    // we don't call SetOptStatFit() here since it will call Clone()
    // which calls this default constructor in an endless recursion
    if (gStyle) {
      fOptStat = gStyle->GetOptStat();
      fOptFit  = gStyle->GetOptFit();
    }
    this->SetLineWidth(2);
  }

  /// Copy constructor
  CbmQaHist(const CbmQaHist& h) : HistTypeT(h) { SetOptStatFit(h.fOptStat, h.fOptFit); }

  /// Reimplementation of all other HistTypeT constructors
  /// that creates a stat window with current gStyle options.
  template<typename... Types>
  CbmQaHist(Types... args) : HistTypeT(args...)
  {
    if (gStyle) { SetOptStatFit(gStyle->GetOptStat(), gStyle->GetOptFit()); }
    this->SetLineWidth(2);
  }

  /// Destructor
  ~CbmQaHist() {}

  /// Reimplementation of Fit()
  /// that suppresses an immediate drawing in the active window
  ///
  template<typename... Types>
  TFitResultPtr Fit(Types... args)
  {
    TVirtualPad* padsav = gPad;
    GetDummyCanvas().cd();
    this->Sumw2();
    auto ret = HistTypeT::Fit(args...);
    GetDummyCanvas().Clear();

    // make the output look nice

    if (padsav) padsav->cd();
    auto* f = this->GetFunction("gaus");
    if (f) {
      f->SetParNames("Peak", "#mu", "#sigma");
      f->SetLineColor(kRed);
      f->SetLineWidth(3);
      TPaveStats* st = (TPaveStats*) this->FindObject("stats");
      if (!st) { LOG(fatal) << "CbmQaHist: can not access statistics of histogram with name \"" << this->GetName() << '\"'; }
      else {
        st->SetX1NDC(0.6);
        st->SetX2NDC(0.940);
        st->SetY1NDC(0.5);
        st->SetY2NDC(0.930);
        st->SetOptStat(111110);
        st->SetOptFit(10001);
      }
    }
    return ret;
  }

  /// Set stat drawing options and autoresize the stat window
  void SetOptStat(Int_t stat = 1) { SetOptStatFit(stat, fOptFit); }

  /// Set fit drawing options and autoresize the stat window
  void SetOptFit(Int_t fit = 1) { SetOptStatFit(fOptStat, fit); }

  /// Set stat & fit drawing options and autoresize the stat window
  void SetOptStatFit(int stat, int fit)
  {
    // the only way to create and auto-size the stat window is to draw the histogram

    fOptStat = stat;
    fOptFit  = fit;
    if (!gStyle) { return; }  // should not happen

    TVirtualPad* savePad = gPad;
    int saveStat         = gStyle->GetOptStat();
    int saveFit          = gStyle->GetOptFit();

    GetDummyCanvas().cd();
    gStyle->SetOptStat(fOptStat);
    gStyle->SetOptFit(fOptFit);

    this->SetStats(0);  // remove the old stat window
    this->SetStats(1);  // set the flag to create the stat window during Draw()
    this->Draw();
    GetDummyCanvas().Update();
    GetDummyCanvas().Clear();

    // restore the environment
    gStyle->SetOptStat(saveStat);
    gStyle->SetOptFit(saveFit);
    if (savePad) savePad->cd();
  }

private:
  /// a static canvas for temporary drawing
  static CbmQaCanvas& GetDummyCanvas()
  {
    /// the static variable will be initialised at the first call;
    /// deleted at the application end (c++11)
    static CbmQaCanvas dummy("CbmQaTempCanvas", "CbmQaTempCanvas", 1, 1);
    return dummy;
  }

  int fOptStat = 1;
  int fOptFit  = 0;

  ClassDefNV(CbmQaHist, 1);
};

#endif
