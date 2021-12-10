/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Andrey Lebedev, Semen Lebedev [committer] */

#include "LmvmDrawAll.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmUtils.h"

#include "Logger.h"

#include "TCanvas.h"
#include "TClass.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TLatex.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TText.h"
#include <TLegend.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "LmvmDef.h"

using namespace std;
using namespace Cbm;

LmvmHist* LmvmDrawAll::H(ELmvmSignal signal) { return fH[static_cast<int>(signal)]; }

void LmvmDrawAll::DrawHistFromFile(const string& fileInmed, const string& fileQgp, const string& fileOmega,
                                   const string& filePhi, const string& fileOmegaD, const string& outputDir,
                                   bool useMvd)
{
  SetDefaultDrawStyle();
  fOutputDir = outputDir;
  fUseMvd    = useMvd;

  // order in vector is important, see ELmvmSignal enum.
  vector<string> fileNames {fileInmed, fileQgp, fileOmega, filePhi, fileOmegaD};

  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fH.resize(fHMean.fNofSignals);
  for (size_t i = 0; i < fH.size(); i++) {
    fH[i]       = new LmvmHist();
    TFile* file = new TFile(fileNames[i].c_str());
    fH[i]->fHM.ReadFromFile(file);
    int nofEvents = (int) fH[i]->H1("hEventNumber")->GetEntries();
    LOG(info) << "Signal:" << fHMean.fSignalNames[i] << " nofEvents:" << nofEvents << endl;
  }

  CreateMeanHistAll();
  CalcCutEffRange(0.0, 0.2);
  CalcCutEffRange(0.2, 0.6);
  CalcCutEffRange(0.6, 1.2);
  CalcCombBGHistos();
  SBgRangeAll();
  DrawSBgResults();
  DrawMinvAll();
  DrawMinvCombSignalAndBg();
  DrawMinvPtAll();
  DrawSBgVsMinv();
  SaveHist();
  SaveCanvasToImage();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

int LmvmDrawAll::GetNofTotalEvents()
{
  int nofEvents = 0;
  for (ELmvmSignal sig : fHMean.fSignals) {
    nofEvents += H(sig)->H1("hEventNumber")->GetEntries();
  }
  return nofEvents;
}

template<class T>
void LmvmDrawAll::CreateMeanHist(const string& name, int nofEvents, int nofRebins)
{
  for (ELmvmSignal sig : fHMean.fSignals) {
    if (static_cast<int>(sig) == 0) fHMean.fHM.Add(name, static_cast<T*>(H(sig)->GetObject(name)->Clone()));
    else
      static_cast<T*>(fHMean.GetObject(name))->Add(static_cast<T*>(H(sig)->GetObject(name)->Clone()));
  }
  static_cast<T*>(fHMean.GetObject(name))->Scale(1. / (double) nofEvents);
  if (nofRebins > 0) {
    static_cast<T*>(fHMean.GetObject(name))->Rebin(nofRebins);
    double binWidth = static_cast<T*>(fHMean.GetObject(name))->GetBinWidth(1);
    static_cast<T*>(fHMean.GetObject(name))->Scale(1. / binWidth);
  }
}

void LmvmDrawAll::CreateMeanHistAll()
{
  int nofEvents = GetNofTotalEvents();

  for (auto step : fHMean.fAnaSteps) {
    for (auto src : {ELmvmSrc::Bg, ELmvmSrc::Eta, ELmvmSrc::Pi0}) {
      CreateMeanHist<TH1D>(fHMean.GetName("hMinv", src, step), nofEvents, fRebMinv);
      CreateMeanHist<TH2D>(fHMean.GetName("hMinvPt", src, step), nofEvents);
    }

    for (const string& comb : {"PM", "PP", "MM"}) {
      for (const string& ev : {"sameEv", "mixedEv"}) {
        CreateMeanHist<TH1D>(fHMean.GetName("hMinvComb" + comb + "_" + ev, step), nofEvents, fRebMinv);
      }
    }
  }
}

TH1D* LmvmDrawAll::GetCocktailMinvH1(ELmvmAnaStep step) { return GetCocktailMinv<TH1D>("hMinv", step); }

template<class T>
T* LmvmDrawAll::GetCocktailMinv(const string& name, ELmvmAnaStep step)
{
  T* sEta = dynamic_cast<T*>(fHMean.GetObject(fHMean.GetName(name, ELmvmSrc::Eta, step)));
  T* sPi0 = dynamic_cast<T*>(fHMean.GetObject(fHMean.GetName(name, ELmvmSrc::Pi0, step)));
  double binWidth = sEta->GetBinWidth(1);

  T* cocktail = nullptr;
  for (ELmvmSignal signal : fHMean.fSignals) {
    string nameFull = fHMean.GetName(name, ELmvmSrc::Signal, step);
    T* sHist        = dynamic_cast<T*>(H(signal)->GetObject(nameFull)->Clone());
    int nofEvents   = (int) H(signal)->H1("hEventNumber")->GetEntries();
    sHist->Scale(1. / nofEvents);
    if (name != "hMinvPt") {
      sHist->Rebin(fRebMinv);
      sHist->Scale(1. / binWidth);
    }
    if (cocktail == nullptr) cocktail = sHist;
    else
      cocktail->Add(sHist);
  }
  cocktail->Add(sEta);
  cocktail->Add(sPi0);

  return cocktail;
}

void LmvmDrawAll::DrawMinvAll()
{
  for (const ELmvmAnaStep step : fHMean.fAnaSteps) {
    string name = fHMean.GetName("lmvmAll_minv_", step);
    fHMean.fHM.CreateCanvas(name.c_str(), name.c_str(), 1000, 1000);
    DrawMinv(step);
  }
}

void LmvmDrawAll::DrawMinv(ELmvmAnaStep step)
{
  TH1D* bg       = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
  TH1D* pi0      = fHMean.H1Clone("hMinv", ELmvmSrc::Pi0, step);
  TH1D* eta      = fHMean.H1Clone("hMinv", ELmvmSrc::Eta, step);
  TH1D* cocktail = GetCocktailMinvH1(step);
  TH1D* sbg      = static_cast<TH1D*>(bg->Clone());
  sbg->Add(cocktail);
  double binWidth = bg->GetBinWidth(1);

  vector<TH1D*> sHists(fHMean.fNofSignals);
  for (ELmvmSignal signal : fHMean.fSignals) {
    TH1D* sHist = H(signal)->H1Clone("hMinv", ELmvmSrc::Signal, step);
    sHist->Scale(1. / H(signal)->H1("hEventNumber")->GetEntries());
    sHist->Rebin(fRebMinv);
    sHist->Scale(1. / binWidth);
    sHists[static_cast<int>(signal)] = sHist;
  }

  vector<LmvmDrawMinvData> drawData;
  if (step != ELmvmAnaStep::Mc) {
    drawData.emplace_back(sbg, kBlack, kBlack, 1, -1, "Cocktail+BG");
    drawData.emplace_back(bg, kGray, kBlack, 1, -1, "Background");
  }
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::OmegaD)], kCyan + 2, kCyan + 4, 2, -1,
                        "#omega #rightarrow #pi^{0}e^{+}e^{-}");
  drawData.emplace_back(pi0, kGreen - 3, kGreen + 3, 2, -1, "#pi^{0} #rightarrow #gammae^{+}e^{-}");
  drawData.emplace_back(eta, kRed - 4, kRed + 2, 2, -1, "#eta #rightarrow #gammae^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Omega)], kOrange + 7, kOrange + 4, 2, -1,
                        "#omega #rightarrow e^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Phi)], kAzure + 2, kAzure + 3, 2, -1,
                        "#phi #rightarrow e^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Qgp)], kOrange - 2, kOrange - 3, 4, 3112, "QGP radiation");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Inmed)], kMagenta - 3, kMagenta - 2, 4, 3018,
                        "in-medium #rho");
  drawData.emplace_back(cocktail, -1, kRed + 2, 4, -1, "Cocktail");


  double min      = std::numeric_limits<Double_t>::max();
  double max      = std::numeric_limits<Double_t>::min();
  TH1D* h0        = nullptr;
  TLegend* leg    = new TLegend(0.7, 0.6, 0.99, 0.99);
  for (size_t i = 0; i < drawData.size(); i++) {
    const auto& d = drawData[i];
    d.fH->GetYaxis()->SetTitle("dN/dM_{ee} [GeV/c^{2}]^{-1}");
    d.fH->GetYaxis()->SetLabelSize(0.05);

    if (d.fFillColor != -1) d.fH->SetFillColor(d.fFillColor);
    if (d.fFillStyle != -1) d.fH->SetFillStyle(d.fFillStyle);
    leg->AddEntry(d.fH, d.fLegend.c_str(), "f");
    DrawH1(d.fH, kLinear, kLinear, (h0 == nullptr) ? "hist" : "hist,same", d.fLineColor, d.fLineWidth, 0);
    if (h0 == nullptr) h0 = d.fH;
    min = std::min(d.fH->GetMinimum(), min);
    max = std::max(d.fH->GetMaximum(), max);
  }
  if (min == 0.) min = std::min(1e-4, max * 1e-7);
  if (h0 != nullptr) h0->SetMinimum(min);
  if (h0 != nullptr) h0->SetMaximum(1.1 * max);

  leg->SetFillColor(kWhite);
  leg->Draw();
  gPad->SetLogy(true);
  //DrawH1(hists, legendStr, kLinear, kLog, true, 0.7, 0.6, 0.99, 0.99, "HIST L");
}

void LmvmDrawAll::DrawMinvPtAll()
{
  for (const ELmvmAnaStep step : fHMean.fAnaSteps) {
    string name = fHMean.GetName("lmvmAll_minvPt", step);
    fHMean.fHM.CreateCanvas(name.c_str(), name.c_str(), 1000, 1000);
    DrawH2(GetCocktailMinv<TH2D>("hMinvPt", step));
  }
}

void LmvmDrawAll::DrawMinvCombSignalAndBg()
{
  // double yMin   = 1e-7;
  // double yMax   = 5e-2;
  // string yTitle = "dN/dM_{ee} [GeV/c^{2}]^{-1}";
  // string xTitle = "M_{ee} [GeV/c^2]";

  // draw MM/PP ratio for same events
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_CBsameEv_RatioMMPP", "lmvmAll_CBsameEv_RatioMMPP", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* pp = fHMean.H1Clone("hMinvCombPP_sameEv", step);
      TH1D* mm = fHMean.H1Clone("hMinvCombMM_sameEv", step);
      mm->Divide(pp);
      mm->GetYaxis()->SetTitle("Ratio e^{-}e^{-}/e^{+}e^{+}");
      DrawH1(mm, kLinear, kLinear, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Draw PM, MM, PP in one plot
  {
    for (const string& ev : {"sameEv", "mixedEv"}) {
      string cName = "lmvmAll_CB" + ev;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* pp = fHMean.H1Clone("hMinvCombPP_" + ev, step);
        TH1D* mm = fHMean.H1Clone("hMinvCombMM_" + ev, step);
        TH1D* pm = fHMean.H1Clone("hMinvCombPM_" + ev, step);
        pm->GetYaxis()->SetTitle("M_{ee}");
        DrawH1({pm, pp, mm}, {"e+e-", "e+e+", "e-e-"}, kLinear, kLog, true, 0.85, 0.7, 0.99, 0.99, "HIST");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  // Draw same and mixed for PP, MM, PM cases; normalize to 400 - 700 MeV/c2 interval
  {
    for (const string& comb : {"PM", "PP", "MM"}) {
      string cName = "lmvmAll_CB_sameMixed" + comb;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* same   = fHMean.H1Clone("hMinvComb" + comb + "_sameEv", step);
        TH1D* mixed  = fHMean.H1Clone("hMinvComb" + comb + "_mixedEv", step);
        int minBin   = same->FindBin(0.4);
        int maxBin   = same->FindBin(0.7);
        double scale = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
        mixed->Scale(scale);
        DrawH1({same, mixed}, {"Same " + comb, "Mixed " + comb}, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99, "HIST");
        fHMean.DrawAnaStepOnPad(step);
        int nofSamePairs  = same->GetEntries();
        int nofMixedPairs = mixed->GetEntries();
        DrawTextOnPad("same: " + Cbm::NumberToString(nofSamePairs, 5), 0.4, 0.7, 0.8, 0.9);
        DrawTextOnPad("mixed: " + Cbm::NumberToString(nofMixedPairs, 5), 0.4, 0.5, 0.8, 0.7);
      }
    }
  }

  // Draw ratio mixed/same for PP, MM, PM cases
  {
    for (const string& comb : {"PM", "PP", "MM"}) {
      string cName = "lmvmAll_CB_mixedOverSame" + comb;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* same          = fHMean.H1Clone("hMinvComb" + comb + "_sameEv", step);
        TH1D* mixedOverSame = fHMean.CreateHByClone<TH1D>("hMinvComb" + comb + "_mixedEv", "hMinvMixedOverSame", step);
        mixedOverSame->Divide(mixedOverSame, same, 1., 1., "B");
        DrawH1(mixedOverSame, kLinear, kLog, "hist");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  // compare PM with sbg (Cocktail + BG)
  {
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      TH1D* pmSame = fHMean.H1Clone("hMinvCombPM_sameEv", step);
      TH1D* sbg    = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
      sbg->Add(GetCocktailMinvH1(step));
      TH1D* ratio = (TH1D*) pmSame->Clone();

      string cName = "lmvmAll_CB_SameVsSbg_" + fHMean.fAnaStepNames[static_cast<int>(step)];
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 900);
      c->Divide(2, 1);
      c->cd(1);
      DrawH1({pmSame, sbg}, {"N_{same}^{+-}", "Cocktail + BG"}, kLinear, kLog, true, 0.7, 0.8, 0.99, 0.99, "HIST");
      pmSame->SetMinimum(1e-5 * sbg->GetMaximum());
      c->cd(2);
      ratio->Divide(sbg);
      DrawH1(ratio, kLinear, kLog, "hist");
      ratio->SetMinimum(0.01);
    }
  }


  //Draw Geometric Mean
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_minvCombGeom", "lmvmAll_minvCombGeom", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* same   = fHMean.H1Clone("hMinvCombGeom_sameEv", step);
      TH1D* mixed  = fHMean.H1Clone("hMinvCombGeom_mixedEv", step);
      int minBin   = same->FindBin(0.4);
      int maxBin   = same->FindBin(0.7);
      double scale = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
      mixed->Scale(scale);
      same->GetXaxis()->SetTitle("M_{ee}");
      DrawH1({same, mixed}, {"same", "mixed"}, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99, "HIST");
      fHMean.DrawAnaStepOnPad(step);
    }
  }


  // Draw CombBg vs Cocktail+Bg vs combBg(Asm)
  {
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      TH1D* cbg    = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* cbgAsm = fHMean.H1Clone("hMinvCombBgAsm", step);
      TH1D* sbg    = GetCocktailMinvH1(step);
      sbg->Add(fHMean.H1("hMinv", ELmvmSrc::Bg, step));

      TH1D* ratio1 = static_cast<TH1D*>(sbg->Clone());
      TH1D* ratio2 = static_cast<TH1D*>(sbg->Clone());

      string cName = "lmvmAll_minvCombBgVsSBgVsCombBgAsm_" + fHMean.fAnaStepNames[static_cast<int>(step)];
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 900);
      c->Divide(2, 1);
      c->cd(1);
      DrawH1({cbg, sbg, cbgAsm}, {"CombBG", "Cocktail + BG", "CombBG(Asm)"}, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99,
             "HIST");
      c->cd(2);
      ratio1->Divide(cbg);
      ratio2->Divide(cbgAsm);
      DrawH1({ratio1, ratio2}, {"Cocktail+BG/CombBG", "Cocktail+BG/CombBG(Asm)"}, kLinear, kLog, true, 0.65, 0.85, 0.99,
             0.99, "HIST");

      fHMean.DrawAnaStepOnPad(step);
    }
  }


  //Draw k factor
  {
    fHMean.fHM.CreateCanvas("lmvmAll_minvCombK", "lmvmAll_minvCombK", 1000, 1000);
    vector<string> legend;
    vector<TH1*> hists;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::Reco) continue;
      hists.push_back(fHMean.H1Clone("hMinvCombK", step));
      legend.push_back(fHMean.fAnaStepLatex[static_cast<int>(step)]);
    }
    hists[0]->GetXaxis()->SetTitle("M_{ee}");
    hists[0]->GetYaxis()->SetTitle("k factor");
    DrawH1(hists, legend, kLinear, kLinear, true, 0.8, 0.6, 0.99, 0.99, "HIST");
  }

  //Draw Combinatorial Signal from N+-same (without mixed geom. mean)      // TODO: without mixed: needed or delete?
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_minvCombPMSignal", "lmvmAll_minvCombPMSignal", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* pmSame          = fHMean.H1Clone("hMinvCombPM_sameEv", step);
      TH1D* combPMSignalAsm = fHMean.H1Clone("hMinvCombPMSignal", step);
      TH1D* combBgAsm       = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* cocktail        = GetCocktailMinvH1(step);
      pmSame->SetMaximum(3e-1);
      pmSame->SetMinimum(4e-8);
      DrawH1({pmSame, combBgAsm, combPMSignalAsm, cocktail},
             {"N_{same}^{+-}", "B_{c}", "Signal (N_{same}^{+-} - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.8, 0.8,
             0.99, 0.99, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  //Draw Combinatorial Signal from N+-same
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_minvCombPMSignalAsm", "lmvmAll_minvCombPMSignalAsm", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* pmSame          = fHMean.H1Clone("hMinvCombPM_sameEv", step);
      TH1D* combPMSignalAsm = fHMean.H1Clone("hMinvCombPMSignalAsm", step);
      TH1D* combBgAsm       = fHMean.H1Clone("hMinvCombBgAsm", step);
      TH1D* cocktail        = GetCocktailMinvH1(step);
      pmSame->SetMaximum(3e-1);
      pmSame->SetMinimum(4e-8);
      DrawH1({pmSame, combBgAsm, combPMSignalAsm, cocktail},
             {"N_{same}^{+-}", "B_{c} (asm)", "Signal (N_{same}^{+-} - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.8,
             0.8, 0.99, 0.99, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  //Draw Combinatorial Signal from Cocktail+BG
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_minvSbgSignalAsm", "lmvmAll_minvSbgSignalAsm", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* sbg = fHMean.H1Clone("hMinv_bg", step);
      sbg->Add(GetCocktailMinvH1(step));
      TH1D* combBgAsm    = fHMean.H1Clone("hMinvCombBgAsm", step);
      TH1D* sBgSignalAsm = fHMean.H1Clone("hMinvSBgSignalAsm", step);
      sbg->SetMaximum(3e-1);
      sbg->SetMinimum(4e-8);
      TH1D* cocktail     = GetCocktailMinvH1(step);
      DrawH1({sbg, combBgAsm, sBgSignalAsm, cocktail},
             {"Cocktail + BG", "B_{c} (asm)", "Signal (Cocktail+BG - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.8,
             0.8, 0.99, 0.99, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }


  //Draw Combinatorial Signal from Cocktail+BG (without mixed geom. mean)      // TODO: without mixed: needed or delete?
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("lmvmAll_minvSbgSignal", "lmvmAll_minvSbgSignal", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* sbg = fHMean.H1Clone("hMinv_bg", step);
      sbg->Add(GetCocktailMinvH1(step));
      TH1D* combBgAsm    = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* sBgSignalAsm = fHMean.H1Clone("hMinvSBgSignal", step);
      sbg->SetMaximum(3e-1);
      sbg->SetMinimum(4e-8);
      TH1D* cocktail = GetCocktailMinvH1(step);
      DrawH1({sbg, combBgAsm, sBgSignalAsm, cocktail},
             {"Cocktail + BG", "B_{c}", "Signal (Cocktail+BG - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.8, 0.8,
             0.99, 0.99, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }
}

void LmvmDrawAll::DrawSBgVsMinv()
{
  vector<TH1*> hists;
  for (ELmvmAnaStep step : {ELmvmAnaStep::TtCut, ELmvmAnaStep::PtCut}) {
    TH1D* bg = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
    //TH1D* combBg  = fHMean.H1Clone("hMinvCombBg", step);
    TH1D* cocktail       = GetCocktailMinvH1(step);
    TH1D* cocktailOverBg = fHMean.CreateHByClone<TH1D>("hMinv_bg", "hMinvCocktailOverBg", step);
    cocktailOverBg->GetYaxis()->SetTitle("Cocktail/Background");
    cocktailOverBg->Divide(cocktail, bg, 1., 1., "B");
    hists.push_back(cocktailOverBg);
  }

  fHMean.fHM.CreateCanvas("lmvmAll_minvCocktailOverBg", "lmvmAll_minvCocktailOverBg", 1000, 1000);
  DrawH1(hists, {"Without Pt cut", "With Pt cut"}, kLinear, kLog, true, 0.6, 0.85, 0.99, 0.99, "hist");
}

void LmvmDrawAll::SaveHist()
{
  if (fOutputDir != "") {
    gSystem->mkdir(fOutputDir.c_str(), true);
    TFile* f = TFile::Open(string(fOutputDir + "/draw_all_hist.root").c_str(), "RECREATE");
    fHMean.WriteToFile();
    f->Close();
  }
}

void LmvmDrawAll::CalcCombBGHistos()
{
  // TODO : think about normalization
  //int nofEvents = GetNofTotalEvents();

  // Geometrical mean
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    for (const string& ev : {"sameEv", "mixedEv"}) {
      TH1D* pp   = fHMean.H1Clone("hMinvCombPP_" + ev, step);
      TH1D* mm   = fHMean.H1Clone("hMinvCombMM_" + ev, step);
      TH1D* geom = fHMean.CreateHByClone<TH1D>("hMinvCombMM_" + ev, "hMinvCombGeom_" + ev, step);

      for (int i = 1; i <= geom->GetNbinsX(); i++) {
        double cpp     = pp->GetBinContent(i);
        double cmm     = mm->GetBinContent(i);
        double content = std::sqrt(
          cpp
          * cmm);  // TODO: what, if one of these values is 0, even after rebinning? Should then last result be applied with a warning?
        geom->SetBinContent(i, content);
      }
    }

    // 300 - 1000 MeV/c2 interval
    TH1D* hGeomSame  = fHMean.H1("hMinvCombGeom_sameEv", step);
    TH1D* hGeomMixed = fHMean.H1("hMinvCombGeom_mixedEv", step);
    int minBin       = hGeomSame->FindBin(0.3);
    int maxBin       = hGeomSame->FindBin(1.0);
    double normGM    = hGeomSame->Integral(minBin, maxBin) / hGeomMixed->Integral(minBin, maxBin);

    // calculate k factor
    //fh_mean_combBg_k_minv
    TH1D* hK = fHMean.CreateHByClone<TH1D>("hMinvCombPM_mixedEv", "hMinvCombK", step);
    hK->Divide(fHMean.H1("hMinvCombGeom_mixedEv", step));
    hK->Scale(0.5);

    // calculate combinatorial BG
    // fh_mean_combBg_minv
    TH1D* hCBg = fHMean.CreateHByClone<TH1D>("hMinvCombK", "hMinvCombBg", step);
    hCBg->Multiply(fHMean.H1("hMinvCombGeom_sameEv", step));
    hCBg->Scale(2.);

    // calculate assembled combinatorial BG from same (<= 0.3 GeV) and mixed (> 0.3 GeV) events data
    //fh_mean_combBg_assemb_minv
    {
      TH1D* hAsm = fHMean.CreateHByClone<TH1D>("hMinvCombBg", "hMinvCombBgAsm", step);
      // from > 300 MeV on normalized data from mixed event
      for (int i = minBin + 1; i <= hAsm->GetNbinsX(); i++) {
        double k  = fHMean.H1("hMinvCombK", step)->GetBinContent(i);
        double gm = fHMean.H1("hMinvCombGeom_mixedEv", step)->GetBinContent(i);
        hAsm->SetBinContent(i, 2 * k * gm * normGM);
      }
    }

    // calculate comb. signal from same events data (S = N^{+-}_{same} - B_{C})   // TODO: without mixed: needed?
    TH1D* hCombPMSignal = fHMean.CreateHByClone<TH1D>("hMinvCombPM_sameEv", "hMinvCombPMSignal", step);
    hCombPMSignal->Add(fHMean.H1("hMinvCombBg", step), -1.);

    // calculate assembled comb. signal from same (<= 0.3 GeV) and mixed (> 0.3 GeV) events data
    // from 'N+-same', Signal (N^{+-}_{same} - B_{C})
    //fh_mean_combSignalNpm_assemb_minv
    TH1D* hCombPMSignalAsm = fHMean.CreateHByClone<TH1D>("hMinvCombPM_sameEv", "hMinvCombPMSignalAsm", step);
    hCombPMSignalAsm->Add(fHMean.H1("hMinvCombBgAsm", step), -1.);

    // from 'Cocktail + BG
    TH1D* hBCSignal = fHMean.CreateHByClone<TH1D>("hMinv_bg", "hMinvSBgSignal", step);
    hBCSignal->Add(GetCocktailMinvH1(step));
    hBCSignal->Add(fHMean.H1("hMinvCombBg", step), -1.);

    // from 'Cocktail + BG' (S = Coc+BG - B_{C})     // TODO: without mixed: needed?
    //fh_mean_combSignalBCoc_assemb_minv
    TH1D* hBCSignalAsm = fHMean.CreateHByClone<TH1D>("hMinv_bg", "hMinvSBgSignalAsm", step);
    hBCSignalAsm->Add(GetCocktailMinvH1(step));
    hBCSignalAsm->Add(fHMean.H1("hMinvCombBgAsm", step), -1.);

    // TODO: @Cornelius check if calculations are correct
    int nofBins = fHMean.H1("hMinvCombPM_sameEv", step)->GetNbinsX();
    for (int i = 1; i <= nofBins; i++) {
      //s_ for same, m_ for mixed
      double s_pm = fHMean.H1("hMinvCombPM_sameEv", step)->GetBinContent(i);
      double s_pp = fHMean.H1("hMinvCombPP_sameEv", step)->GetBinContent(i);
      double s_mm = fHMean.H1("hMinvCombMM_sameEv", step)->GetBinContent(i);
      double m_pm = fHMean.H1("hMinvCombPM_mixedEv", step)->GetBinContent(i);
      double m_pp = fHMean.H1("hMinvCombPP_mixedEv", step)->GetBinContent(i);
      double m_mm = fHMean.H1("hMinvCombMM_mixedEv", step)->GetBinContent(i);

      double s_dpm  = 1.;  // derivatives of B_c and signal resp. to single contributions
      double d1     = 1. / std::sqrt(s_pp * s_mm * m_pp * m_mm);
      double s_dpp  = 0.5 * m_pm * s_mm * d1;
      double s_dmm  = 0.5 * m_pm * s_pp * d1;
      double m_dpm  = std::sqrt((s_pp * s_mm) / (m_pp * m_mm));
      double d2     = std::sqrt(s_pp * s_mm) / std::pow(std::sqrt(m_pp * m_mm), 3);
      double m_dpp  = -0.5 * m_pm * m_mm * d2;
      double m_dmm  = -0.5 * m_pm * m_pp * d2;
      double m_dpm2 = normGM;  // for > 300 MeV

      double s_fpm = std::pow(std::sqrt(s_pm) * s_dpm, 2);
      double s_fpp = std::pow(std::sqrt(s_pp) * s_dpp, 2);  // single contribution factors to error propagation
      double s_fmm = std::pow(std::sqrt(s_mm) * s_dmm, 2);
      double m_fpm = std::pow(std::sqrt(m_pm) * m_dpm, 2);
      double m_fpp =
        std::pow(std::sqrt(m_pp) * m_dpp, 2);  //TODO: @Cornelius check, changed from s_pp2 to m_pp2  // for > 300 MeV
      double m_fmm  = std::pow(std::sqrt(m_mm) * m_dmm, 2);   //TODO: @Cornelius check, changed from s_mm2 to m_mm2
      double m_fpm2 = std::pow(std::sqrt(m_pm) * m_dpm2, 2);  // for > 300 MeV

      double errorBc   = std::sqrt(s_fpp + s_fmm + m_fpm + m_fpp + m_fmm);  // final error propagation value
      double errorBc2  = std::sqrt(m_fpm2);
      double errorSig  = std::sqrt(s_fpm + s_fpp + s_fmm + m_fpm + m_fpp + m_fmm);
      double errorSig2 = std::sqrt(s_fpm + m_fpm2);

      fHMean.H1("hMinvCombBg", step)->SetBinError(i, errorBc);
      if (i <= minBin) fHMean.H1("hMinvCombBgAsm", step)->SetBinError(i, errorBc);
      if (i > minBin) fHMean.H1("hMinvCombBgAsm", step)->SetBinError(i, errorBc2);
      if (i <= minBin) fHMean.H1("hMinvCombPMSignalAsm", step)->SetBinError(i, errorSig);
      if (i > minBin) fHMean.H1("hMinvCombPMSignalAsm", step)->SetBinError(i, errorSig2);
    }
  }
}

void LmvmDrawAll::CalcCutEffRange(double minMinv, double maxMinv)
{
  stringstream ss1;
  ss1 << "hCutEff_" << minMinv << "to" << maxMinv;
  fHMean.CreateH1(ss1.str() + "_bg", "Analysis step", "Efficiency [%]", fHMean.fNofAnaSteps, 0, fHMean.fNofAnaSteps);
  fHMean.CreateH1(ss1.str() + "_s", "Analysis step", "Efficiency [%]", fHMean.fNofAnaSteps, 0, fHMean.fNofAnaSteps);
  TH1D* hS         = fHMean.H1(ss1.str() + "_s");
  TH1D* hBg        = fHMean.H1(ss1.str() + "_bg");
  int x            = 1;
  TH1D* cocktail   = GetCocktailMinvH1(ELmvmAnaStep::ElId);
  int binMin       = cocktail->FindBin(minMinv);
  int binMax       = cocktail->FindBin(maxMinv);
  double sIntElId  = cocktail->Integral(binMin, binMax);
  double bgIntElId = fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->Integral(binMin, binMax);
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    if (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut)) continue;

    double effS = 100. * GetCocktailMinvH1(step)->Integral(binMin, binMax) / sIntElId;
    double effB = 100. * fHMean.H1("hMinv", ELmvmSrc::Bg, step)->Integral(binMin, binMax) / bgIntElId;

    hBg->GetXaxis()->SetBinLabel(x, fHMean.fAnaStepLatex[static_cast<int>(step)].c_str());
    hBg->SetBinContent(x, effB);
    hS->SetBinContent(x, effS);
    x++;
  }

  hBg->GetXaxis()->SetLabelSize(0.06);
  hBg->GetXaxis()->SetRange(1, x - 1);
  hS->GetXaxis()->SetRange(1, x - 1);

  stringstream ss;
  ss << "lmvmAll_cutEff_" << minMinv << "to" << maxMinv;
  fHMean.fHM.CreateCanvas(ss.str(), ss.str(), 1000, 1000);
  DrawH1({hBg, hS}, {"BG", "Cocktail"}, kLinear, kLinear, true, 0.75, 0.85, 1.0, 1.0, "hist");
  hBg->SetLineWidth(4);
  hS->SetLineWidth(4);
  hBg->SetMinimum(1);
  hBg->SetMaximum(110);

  stringstream ss2;
  ss2 << minMinv << "<M [GeV/c^2]<" << maxMinv;
  TText* t = new TText(0.5, hBg->GetMaximum() + 5, ss2.str().c_str());
  t->Draw();
}

TH1D* LmvmDrawAll::SBgRange(double minMinv, double maxMinv)
{
  stringstream ss;
  ss << "hSBgRatio_" << minMinv << "to" << maxMinv;
  fHMean.CreateH1(ss.str(), "Analysis step", "Cocktail/BG", fHMean.fNofAnaSteps, 0, fHMean.fNofAnaSteps);
  TH1D* hSBg = fHMean.H1(ss.str());
  hSBg->GetXaxis()->SetLabelSize(0.06);
  int x          = 1;
  TH1D* cocktail = GetCocktailMinvH1(ELmvmAnaStep::ElId);
  int binMin     = cocktail->FindBin(minMinv);
  int binMax     = cocktail->FindBin(maxMinv);
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    if (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut)) continue;

    double intS  = 100. * GetCocktailMinvH1(step)->Integral(binMin, binMax);
    double intBg = 100. * fHMean.H1("hMinv", ELmvmSrc::Bg, step)->Integral(binMin, binMax);
    double sbg   = intS / intBg;

    hSBg->GetXaxis()->SetBinLabel(x, fHMean.fAnaStepLatex[static_cast<int>(step)].c_str());
    hSBg->SetBinContent(x, sbg);
    x++;
  }
  hSBg->GetXaxis()->SetRange(1, x - 1);
  return hSBg;
}

void LmvmDrawAll::SBgRangeAll()
{
  TH1D* h1 = SBgRange(0.0, 0.2);
  TH1D* h2 = SBgRange(0.2, 0.6);
  TH1D* h3 = SBgRange(0.6, 1.2);

  fHMean.fHM.CreateCanvas("lmvmAll_sBgRatio_Ranges", "lmvmAll_sBgRatio_Ranges", 1000, 1000);
  DrawH1({h1, h2, h3}, {"0.0<M [GeV/c^{2}]<0.2", "0.2<M [GeV/c^{2}]<0.6", "0.6<M [GeV/c^{2}]<1.2"}, kLinear, kLog, true,
         0.25, 0.83, 0.75, 0.99, "hist");

  h1->SetMinimum(0.9 * std::min({h1->GetMinimum(), h2->GetMinimum(), h3->GetMinimum()}));
  h1->SetMaximum(1.1 * std::max({h1->GetMaximum(), h2->GetMaximum(), h3->GetMaximum()}));
  h1->SetLineWidth(4);
  h2->SetLineWidth(4);
  h3->SetLineWidth(4);

  TH1D* h4 = SBgRange(0.5, 0.6);
  fHMean.fHM.CreateCanvas("lmvmAll_sBgRatio_Range05to06", "lmvmAll_sBgRatio_Range05to06", 1000, 1000);
  DrawH1(h4, kLinear, kLinear);
  h4->SetLineWidth(4);
}

void LmvmDrawAll::DrawSBgResults()
{
  TCanvas* cFit = fHMean.fHM.CreateCanvas("lmvmAll_signalFit", "lmvmAll_signalFit", 1000, 1000);
  ofstream resultFile(fOutputDir + "/lmvmAll_results.txt");
  for (auto signal : fHMean.fSignals) {
    string signalName = fHMean.fSignalNames[static_cast<int>(signal)];
    fHMean.CreateH1("hSBgRatio_" + signalName, "Analysis steps", "S/BG", fHMean.fNofAnaSteps, 0, fHMean.fNofAnaSteps);
    TH1D* hSBg = fHMean.H1("hSBgRatio_" + signalName);
    hSBg->GetXaxis()->SetLabelSize(0.06);
    hSBg->SetLineWidth(4);
    int x = 1;
    for (ELmvmAnaStep step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      if (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut)) continue;
      cFit->cd();
      LmvmSBgResultData result = CalculateSBgResult(signal, step);

      hSBg->GetXaxis()->SetBinLabel(x, fHMean.fAnaStepLatex[static_cast<int>(step)].c_str());
      if (result.fSBgRatio < 1000.) hSBg->SetBinContent(x, result.fSBgRatio);
      x++;
      resultFile << signalName << " " << fHMean.fAnaStepNames[static_cast<int>(step)] << " " << result.fSignallEff
                 << " " << result.fSBgRatio << " " << result.fFitMean << " " << result.fFitSigma;
    }
    hSBg->GetXaxis()->SetRange(1, x - 1);
    fHMean.fHM.CreateCanvas("lmvmAll_sBgRatio_" + signalName, "lmvmAll_sBgRatio_" + signalName, 1000, 1000);
    DrawH1(hSBg);
    hSBg->SetLineWidth(4);
  }
  resultFile.close();
}

LmvmSBgResultData LmvmDrawAll::CalculateSBgResult(ELmvmSignal signal, ELmvmAnaStep step)
{
  TH1D* s  = H(signal)->H1("hMinv", ELmvmSrc::Signal, step);
  TH1D* bg = H(signal)->H1("hMinv", ELmvmSrc::Bg, step);

  if (s->GetEntries() < 10) return LmvmSBgResultData(0., 0., 0., 0.);

  TH1D* sClone = static_cast<TH1D*>(s->Clone());
  if (signal == ELmvmSignal::Phi) sClone->Fit("gaus", "Q", "", 0.95, 1.05);
  else if (signal == ELmvmSignal::Omega)
    sClone->Fit("gaus", "Q", "", 0.69, 0.81);
  else
    sClone->Fit("gaus", "Q");

  TF1* func    = sClone->GetFunction("gaus");
  double mean  = (func != nullptr) ? func->GetParameter("Mean") : 0.;
  double sigma = (func != nullptr) ? func->GetParameter("Sigma") : 0.;
  int minInd   = s->FindBin(mean - 2. * sigma);
  int maxInd   = s->FindBin(mean + 2. * sigma);

  double sumSignal = 0.;
  double sumBg     = 0.;
  for (int i = minInd + 1; i <= maxInd - 1; i++) {
    sumSignal += s->GetBinContent(i);
    sumBg += bg->GetBinContent(i);
  }
  double sbg = (sumBg != 0.) ? sumSignal / sumBg : 0.;

  double eff = 100. * H(signal)->H1("hPtYPairSignal", step)->GetEntries()
               / H(signal)->H1("hPtYPairSignal", ELmvmAnaStep::Mc)->GetEntries();

  return LmvmSBgResultData(sbg, eff, mean, sigma);
}

void LmvmDrawAll::SaveCanvasToImage()
{
  cout << "Images output dir:" << fOutputDir << endl;
  fHMean.fHM.SaveCanvasToImage(fOutputDir, "png");  // fHM[0]->SaveCanvasToImage(fOutputDir, "eps;png");
}

ClassImp(LmvmDrawAll);
