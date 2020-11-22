/// \file   CbmQaHist.h
/// \brief  Definition of the CbmQaHist class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   22.11.2020

#ifndef CbmQaHist_H
#define CbmQaHist_H

#include "CbmQaCanvas.h"
#include "TFitResultPtr.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TVirtualPad.h"
/// A modification of TH* classes that keeps statistics & fit drawing options
/// and resizes the stat window accordingly without actual drawing.
/// In the original classes, hist->Draw() & canv->Update() must be called
/// for resetting Stat/Fit window.
///
template<class HistTypeT>
class CbmQaHist : public HistTypeT {
public:
  /// Default constructor only for the streamer
  CbmQaHist() : HistTypeT() {
    // we don't call SetOptStatFit() here since it will call Clone()
    // which calls this default constructor in an endless recursion
    if (gStyle) {
      fOptStat = gStyle->GetOptStat();
      fOptFit  = gStyle->GetOptFit();
    }
  }

  /// Copy constructor
  CbmQaHist(const CbmQaHist& h) : HistTypeT(h) {
    SetOptStatFit(h.fOptStat, h.fOptFit);
  }

  /// Reimplementation of all other HistTypeT constructors
  /// that creates a stat window with current gStyle options.
  template<typename... Types>
  CbmQaHist(Types... args) : HistTypeT(args...) {
    if (gStyle) { SetOptStatFit(gStyle->GetOptStat(), gStyle->GetOptFit()); }
  }

  /// Destructor
  ~CbmQaHist() {}

  /// Reimplementation of Fit()
  /// that suppresses an immediate drawing in the active window
  ///
  template<typename... Types>
  TFitResultPtr Fit(Types... args) {
    TVirtualPad* padsav = gPad;
    GetCanvas().cd();
    auto ret = HistTypeT::Fit(args...);
    GetCanvas().Clear();
    if (padsav) padsav->cd();
    auto* f = this->GetFunction("gaus");
    if (f) {
      f->SetParName(0, "Peak");
      f->SetParName(1, "#mu");
      f->SetParName(2, "#sigma");
      f->SetLineColor(kRed);
      f->SetLineWidth(3);
    }
    return ret;
  }

  /// Set stat drawing options and resize the stat window
  void SetOptStat(Int_t stat = 1) { SetOptStatFit(stat, fOptFit); }

  /// Set fit drawing options and resize the stat window
  void SetOptFit(Int_t fit = 1) { SetOptStatFit(fOptStat, fit); }

  /// Set stat & fit drawing options and resize the stat window
  void SetOptStatFit(int stat, int fit) {
    fOptStat = stat;
    fOptFit  = fit;
    if (!gStyle) { return; }
    TVirtualPad* savePad = gPad;
    int saveStat         = gStyle->GetOptStat();
    int saveFit          = gStyle->GetOptFit();

    this->SetStats(0);  // remove the old stat window
    this->SetStats(1);  // set the flag to create thes stat window during Draw()
    CbmQaHist* tmp = (CbmQaHist*) this->Clone("myClone");
    GetCanvas().cd();
    gStyle->SetOptStat(fOptStat);
    gStyle->SetOptFit(fOptFit);
    // the only way to create and auto-size the stat window
    tmp->Draw();
    GetCanvas().Update();
    // move the stat window to *this
    TObject* obj = tmp->FindObject("stats");
    // the list of functions seems always to exist, but let's check it
    if (obj && this->GetListOfFunctions()) {
      tmp->GetListOfFunctions()->Remove(obj);
      this->GetListOfFunctions()->Add(obj);
    }
    GetCanvas().Clear();
    gStyle->SetOptStat(saveStat);
    gStyle->SetOptFit(saveFit);
    if (savePad) savePad->cd();
  }

private:
  /// a static canvas for temporary drawing
  static CbmQaCanvas& GetCanvas() {
    /// the static variable will be initialised at the first call;
    /// deleted at the application end (c++11)
    static CbmQaCanvas tmp("CbmQaTempCanvas", "CbmQaTempCanvas", 1, 1);
    return tmp;
  }

  int fOptStat = 1;
  int fOptFit  = 0;

  ClassDefNV(CbmQaHist, 1);
};

// shortcuts
typedef CbmQaHist<TH1F> CbmQaH1F;
typedef CbmQaHist<TH1D> CbmQaH1D;

#endif
