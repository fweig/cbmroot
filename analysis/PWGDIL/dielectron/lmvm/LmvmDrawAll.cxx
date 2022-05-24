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
  DrawMinvCombBgAndSignal();
  DrawMinvPtAll();
  DrawMinvBgSourcesAll();
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
  if (nofRebins > 1) {
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
      for (const string& cat : {"", "_urqmd"}) {
        for (const string& ev : {"_sameEv", "_mixedEv"}) {
          CreateMeanHist<TH1D>(fHMean.GetName("hMinvComb" + comb + cat + ev, step), nofEvents, fRebMinv);
        }
      }
    }
  }
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_gg", nofEvents, fRebMinv);  // TODO: automatize this
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_pipi", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_pi0pi0", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_oo", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_gpi", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_gpi0", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_go", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_pipi0", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_pio", nofEvents, fRebMinv);
  CreateMeanHist<TH1D>("hMinvBgSource2_elid_pi0o", nofEvents, fRebMinv);
}

TH1D* LmvmDrawAll::GetCocktailMinvH1(ELmvmAnaStep step) { return GetCocktailMinv<TH1D>("hMinv", step); }

template<class T>
T* LmvmDrawAll::GetCocktailMinv(const string& name, ELmvmAnaStep step)
{
  T* sEta = dynamic_cast<T*>(fHMean.GetObject(fHMean.GetName(name, ELmvmSrc::Eta, step)));
  T* sPi0 = dynamic_cast<T*>(fHMean.GetObject(fHMean.GetName(name, ELmvmSrc::Pi0, step)));

  T* cocktail = nullptr;
  for (ELmvmSignal signal : fHMean.fSignals) {
    string nameFull = fHMean.GetName(name, ELmvmSrc::Signal, step);
    T* sHist        = dynamic_cast<T*>(H(signal)->GetObject(nameFull)->Clone());
    int nofEvents   = (int) H(signal)->H1("hEventNumber")->GetEntries();
    sHist->Scale(1. / nofEvents);
    // Rebinning only for 1D histograms
    if (dynamic_cast<T*>(fHMean.GetObject(fHMean.GetName(name, ELmvmSrc::Eta, step)))->GetDimension() == 1) {
      double binWidth = sEta->GetBinWidth(1);
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
    if (step < ELmvmAnaStep::ElId) continue; // TODO: activate also earlier histos in Task.cxx
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
  TH1D* cb        = fHMean.H1Clone("hMinvCombBg", step);
  TH1D* cbSig 	  = fHMean.H1Clone("hMinvCombSignalMc", step);
  TH1D* cbU       = fHMean.H1Clone("hMinvCombBg_urqmd", step);
  TH1D* cbSigU	  = fHMean.H1Clone("hMinvCombSignalMc_urqmd", step);
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

  drawData.emplace_back(pi0, kGreen - 3, kGreen + 3, 2, -1, "#pi^{0} #rightarrow #gammae^{+}e^{-}");
  drawData.emplace_back(eta, kRed - 4, kRed + 2, 2, -1, "#eta #rightarrow #gammae^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::OmegaD)], kCyan + 2, kCyan + 4, 2, -1,
                        "#omega #rightarrow #pi^{0}e^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Omega)], kOrange + 7, kOrange + 4, 2, -1,
                        "#omega #rightarrow e^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Phi)], kAzure + 2, kAzure + 3, 2, -1,
                        "#phi #rightarrow e^{+}e^{-}");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Qgp)], kOrange - 2, kOrange - 3, 4, 3112, "QGP radiation");
  drawData.emplace_back(sHists[static_cast<int>(ELmvmSignal::Inmed)], kMagenta - 3, kMagenta - 2, 4, 3018,
                        "in-medium #rho");
  drawData.emplace_back(cocktail, -1, kRed + 2, 4, -1, "Cocktail");
  drawData.emplace_back(cb, -1, kCyan + 2, 4, -1, "CB");
  drawData.emplace_back(cbSig, -1, kCyan, 3, -1, "CB-Signal");
  drawData.emplace_back(cbU, -1, kMagenta + 3, 4, -1, "CB (UrQMD-El)");
  drawData.emplace_back(cbSigU, -1, kMagenta, 3, -1, "CB-Signal (UrQMD-El)");


  double min      = std::numeric_limits<Double_t>::max();
  double max      = std::numeric_limits<Double_t>::min();
  TH1D* h0        = nullptr;
  TLegend* leg    = new TLegend(0.7, 0.57, 0.99, 0.97);
  for (size_t i = 0; i < drawData.size(); i++) {
    const auto& d = drawData[i];
    d.fH->GetYaxis()->SetLabelSize(0.05);

    if (d.fFillColor != -1) d.fH->SetFillColor(d.fFillColor);
    if (d.fFillStyle != -1) d.fH->SetFillStyle(d.fFillStyle);
    leg->AddEntry(d.fH, d.fLegend.c_str(), "f");
    DrawH1(d.fH, kLinear, kLinear, (h0 == nullptr) ? "hist" : "hist,same", d.fLineColor, d.fLineWidth, 0);
    if (h0 == nullptr) h0 = d.fH;
    //min = std::min(d.fH->GetMinimum(), min);
    min = 1e-8;
    max = std::max(d.fH->GetMaximum(), max);
    //h0->SetMinimum(1e-7);
    //h0->SetMaximum(50);
  }
  if (min == 0.) min = std::min(1e-4, max * 1e-7);
  if (h0 != nullptr) h0->SetMinimum(min);
  if (h0 != nullptr) h0->SetMaximum(1.1 * max);

  leg->SetFillColor(kWhite);
  leg->Draw();
  gPad->SetLogy(true);
  //DrawH1(hists, legendStr, kLinear, kLog, true, 0.7, 0.6, 0.99, 0.99, "HIST L");
}

void LmvmDrawAll::DrawMinvBgSourcesAll()
{  
  TH1D* gg     = fHMean.H1Clone("hMinvBgSource2_elid_gg");
  TH1D* pipi   = fHMean.H1Clone("hMinvBgSource2_elid_pipi");
  TH1D* pi0pi0 = fHMean.H1Clone("hMinvBgSource2_elid_pi0pi0");
  TH1D* oo     = fHMean.H1Clone("hMinvBgSource2_elid_oo");
  TH1D* gpi    = fHMean.H1Clone("hMinvBgSource2_elid_gpi");
  TH1D* gpi0   = fHMean.H1Clone("hMinvBgSource2_elid_gpi0");
  TH1D* go     = fHMean.H1Clone("hMinvBgSource2_elid_go");
  TH1D* pipi0  = fHMean.H1Clone("hMinvBgSource2_elid_pipi0");
  TH1D* pio    = fHMean.H1Clone("hMinvBgSource2_elid_pio");
  TH1D* pi0o   = fHMean.H1Clone("hMinvBgSource2_elid_pi0o");

  TH1D* physBg = fHMean.H1Clone("hMinvBgSource2_elid_gg");
  physBg->Add(fHMean.H1("hMinvBgSource2_elid_gpi0"));
  physBg->Add(fHMean.H1("hMinvBgSource2_elid_pi0pi0"));
  
  TH1D* cPi = fHMean.H1Clone("hMinvBgSource2_elid_pipi");
  cPi->Add(fHMean.H1Clone("hMinvBgSource2_elid_gpi"));
  cPi->Add(fHMean.H1Clone("hMinvBgSource2_elid_pipi0"));
  cPi->Add(fHMean.H1Clone("hMinvBgSource2_elid_pio"));

  TH1D* rest = fHMean.H1Clone("hMinvBgSource2_elid_oo");
  rest->Add(fHMean.H1Clone("hMinvBgSource2_elid_go"));
  rest->Add(fHMean.H1Clone("hMinvBgSource2_elid_pi0o"));
  

  //physBg->SetMinimum(1e-6);
  //physBg->SetMaximum(1e-2);

  vector<LmvmDrawMinvData> drawData;

  drawData.emplace_back(gpi0, kBlack, kBlack, 1, -1, "#gamma - #pi^{0}");
  drawData.emplace_back(pi0pi0, kGray + 1, kGray + 1, 1, -1, "#pi^{0} - #pi^{0}");
  drawData.emplace_back(gg, kCyan + 2, kCyan + 4, 2, -1, "#gamma - #gamma");
  drawData.emplace_back(pipi0, kGreen -3 , kGreen + 3, 2, -1, "#pi^{#pm}_{misid} - #pi^{0}");
  drawData.emplace_back(pi0o, kRed - 4, kRed + 2, 1, -1, "#pi^{0} - o.");
  drawData.emplace_back(gpi, kOrange + 7, kOrange + 4, 2, -1, "#gamma - #pi^{#pm}_{misid}");
  drawData.emplace_back(go, kAzure + 2, kAzure + 3, 2, -1, "#gamma - o.");
  drawData.emplace_back(pipi, kOrange - 2, kOrange - 3, 4, 3112, "#pi^{#pm}_{misid} - #pi^{#pm}_{misid}");
  drawData.emplace_back(pio, kMagenta - 3, kMagenta - 2, 4, 3018, "#pi^{#pm}_{misid} - o.");
  drawData.emplace_back(oo, -1, kRed + 2, 4, -1, "o. - o.");

  string cName = "minvBgSrc/minvBgSrc";
  fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1000, 1000);

  TH1D* h0 = nullptr;
  TLegend* leg    = new TLegend(0.65, 0.55, 0.95, 0.95);
  for (size_t i = 0; i < drawData.size(); i++) {
    const auto& d = drawData[i];
    d.fH->GetYaxis()->SetLabelSize(0.05);

    if (d.fFillColor != -1) d.fH->SetFillColor(d.fFillColor);
    if (d.fFillStyle != -1) d.fH->SetFillStyle(d.fFillStyle);
    leg->AddEntry(d.fH, d.fLegend.c_str(), "f");
    DrawH1(d.fH, kLinear, kLinear, (h0 == nullptr) ? "hist" : "hist,same", d.fLineColor, d.fLineWidth, 0);
    if (h0 == nullptr) h0 = d.fH;
  }

  leg->SetFillColor(kWhite);
  leg->Draw();
  gPad->SetLogy(true);

  fHMean.fHM.CreateCanvas((cName + "_misidPi").c_str(), (cName + "_misidPi").c_str(), 1000, 1000);
  DrawH1({physBg, cPi, rest}, {"Phys. BG", "BG w. misid. #pi^{#pm}", "Rest"}, kLinear, kLog, true, 0.7, 0.8, 0.95, 0.91, "p");

  
  //check if all bg pair combinations are considered
  TH1D* bgRat = static_cast<TH1D*>(physBg->Clone());
  bgRat->Add(cPi);
  bgRat->Add(rest);
  
  int nBins = bgRat->GetNbinsX();
  for (int i = 1; i <= nBins; i++) {
    if (fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->GetBinContent(i) == 0 && bgRat->GetBinContent(i) == 0) bgRat->SetBinContent(i, 1);
    else {
      double r = bgRat->GetBinContent(i) / fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->GetBinContent(i);
      bgRat->SetBinContent(i, r);
    }
  }

  bgRat->SetMinimum(0.9);
  bgRat->SetMaximum(1.1);

  fHMean.fHM.CreateCanvas((cName + "_compWithBg").c_str(), (cName + "_compWithBg").c_str(), 800, 800);
  DrawH1(bgRat, kLinear, kLinear, "hist");
}

void LmvmDrawAll::DrawMinvPtAll()
{
  for (const ELmvmAnaStep step : fHMean.fAnaSteps) {
    string name = fHMean.GetName("minvPt/lmvmAll_minvPt", step);
    fHMean.fHM.CreateCanvas(name.c_str(), name.c_str(), 1000, 1000);
    DrawH2(GetCocktailMinv<TH2D>("hMinvPt", step));
  }
}

void LmvmDrawAll::DrawMinvCombBgAndSignal()
{
  // double yMin   = 1e-7;
  // double yMax   = 5e-2;
  // string yTitle = "dN/dM_{ee}/N [GeV/c^{2}]^{-1}";
  // string xTitle = "M_{ee} [GeV/c^2]";

  // draw MM/PP ratio for same events
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/RatioMMPP_same", "cb/RatioMMPP_same", 1800, 1800);
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
      string cName = "cb/pairYield_" + ev;
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

  // Draw same and mixed for PP, MM, PM cases; normalize to 300 - 1000 MeV/c2 interval
  {
    for (const string& comb : {"PM", "PP", "MM"}) {
      string cName = "cb/pairYield_" + comb + "_sameMixed";
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* same    = fHMean.H1Clone("hMinvComb" + comb + "_sameEv", step);
        TH1D* mixed   = fHMean.H1Clone("hMinvComb" + comb + "_mixedEv", step);
        TH1D* sameU   = fHMean.H1Clone("hMinvComb" + comb + "_urqmd_sameEv", step);
        TH1D* mixedU  = fHMean.H1Clone("hMinvComb" + comb + "_urqmd_mixedEv", step);
        int minBin    = same->FindBin(0.3);
        int maxBin    = same->FindBin(1.0);
        double scale  = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
        double scaleU = sameU->Integral(minBin, maxBin) / mixedU->Integral(minBin, maxBin);
        mixed->Scale(scale);
        mixedU->Scale(scaleU);
        same->SetMinimum(1e-8);
        DrawH1({same, mixed, sameU, mixedU}, {"Same " + comb, "Mixed " + comb, "Same " + comb + " (UrQMD-El)", "Mixed " + comb + " (UrQMD-El)"}, kLinear, kLog, true, 0.65, 0.7, 0.99, 0.99, "p");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  // Draw ratio mixed/same for PP, MM, PM cases
  {
    for (const string& comb : {"PM", "PP", "MM"}) {
      string cName = "cb/mixedOverSame" + comb;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* same          = fHMean.H1Clone("hMinvComb" + comb + "_sameEv", step); // TODO: check if this does not change origin histo
        TH1D* mixedOverSame = fHMean.H1Clone("hMinvComb" + comb + "_mixedEv", step);
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
      string cName = "cb/N+-VsCocBg_" + fHMean.fAnaStepNames[static_cast<int>(step)];
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
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/geomMean", "cb/geomMean", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* same    = fHMean.H1Clone("hMinvCombGeom_sameEv", step);
      TH1D* mixed   = fHMean.H1Clone("hMinvCombGeom_mixedEv", step);
      TH1D* sameU   = fHMean.H1Clone("hMinvCombGeom_urqmd_sameEv", step);
      TH1D* mixedU  = fHMean.H1Clone("hMinvCombGeom_urqmd_mixedEv", step);
      int minBin    = same->FindBin(0.4);
      int maxBin    = same->FindBin(0.7);
      double scale  = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
      double scaleU = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
      mixed->Scale(scale);
      mixedU->Scale(scaleU);
      same->GetXaxis()->SetTitle("M_{ee}");
      same->SetMinimum(1e-8);
      mixed->SetMinimum(1e-8);
      DrawH1({same, mixed, sameU, mixedU}, {"geom. mean (same)", "geom. mean (mixed)", "geom. mean (same, UrQMD-El)", "geom. mean (mixed, UrQMD-El)"}, kLinear, kLog, true, 0.52, 0.8, 0.99, 0.99, "p");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Draw k factor
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/k", "cb/k", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* k   = fHMean.H1Clone("hMinvCombK", step);
      TH1D* kU = fHMean.H1Clone("hMinvCombK_urqmd", step);
      double min = 0.6;
      double max = 1.4;
      k->SetMinimum(min);
      kU->SetMinimum(min);
      k->SetMaximum(max);
      kU->SetMaximum(max);
      DrawH1({k, kU}, {"k Factor", "k Factor (UrQMD electrons)"}, kLinear, kLinear, true, 0.65, 0.82, 0.99, 0.95, "p");
      fHMean.DrawAnaStepOnPad(step);
    }
  }
  
  // Draw CB vs MC-BG
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/cbVsMcBg", "cb/cbVsMcBg", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* cb = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* bg = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
      DrawH1({bg, cb}, {"BG from MC input", "comb. BG (B_{C})"}, kLinear, kLog, true, 0.7, 0.75, 0.99, 0.90, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Draw ratio of CB and MC-BG
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/cbVsMcBg_ratio", "cb/cbVsMcBg_ratio", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* cb = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* bg = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);     
      TH1D* ratio = static_cast<TH1D*>(bg->Clone());
      ratio->Divide(cb);
      DrawH1({ratio}, {"BG_{MC}/B_{C}"}, kLinear, kLinear, true, 0.65, 0.75, 0.92, 0.9, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Draw common CB vs. CB from pure UrQMD electrons
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/cbVsUrqmdCb", "cb/cbVsUrqmdCb", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* cb  = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* cbU = fHMean.H1Clone("hMinvCombBg_urqmd", step);
      DrawH1({cb, cbU}, {"B_{C}", "B_{C} from UrQMD electrons"}, kLinear, kLog, true, 0.55, 0.8, 0.99, 0.95);
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  //Draw Combinatorial Signal from N+-same
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/cbVsInput_Npm", "cb/cbVsInput_Npm", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* pmSame        = fHMean.H1Clone("hMinvCombPM_sameEv", step);
      TH1D* combBg        = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* combSignalNpm = fHMean.H1Clone("hMinvCombSignalNpm", step);
      TH1D* cocktail      = GetCocktailMinvH1(step);
      pmSame->SetMaximum(1e-2);
      pmSame->SetMinimum(4e-9);
      DrawH1({pmSame, combBg, combSignalNpm, cocktail},
             {"N_{same}^{+-}", "B_{c}", "Signal (N_{same}^{+-} - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.53,
             0.75, 0.99, 0.92, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  //Draw Combinatorial Signal from Cocktail+BG
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/cbVsInput_cocBg", "cb/cbVsInput_cocBg", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* sbg2 = fHMean.H1Clone("hMinv_bg", step);
      sbg2->Add(GetCocktailMinvH1(step));
      TH1D* combBg    = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* sBgSignal = fHMean.H1Clone("hMinvCombSignalMc", step);
      sbg2->SetMaximum(2e-2);
      sbg2->SetMinimum(4e-9);
      TH1D* cocktail     = GetCocktailMinvH1(step);
      DrawH1({sbg2, combBg, sBgSignal, cocktail},
             {"Cocktail + BG", "B_{C}", "Signal (Cock+BG - B_{C})", "Cocktail"}, kLinear, kLog, true, 0.53,
             0.72, 0.99, 0.92, "p");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Draw ratio of CB-signal and cocktail
  {
    TCanvas* c = fHMean.fHM.CreateCanvas("cb/SigVsCoc", "cb/SigVsCoc", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* sig = fHMean.H1("hMinvCombSignalMc", step);
      TH1D* rat = GetCocktailMinvH1(step);
      rat->Divide(sig);
      rat->SetMaximum(3.);
      DrawH1({rat}, {"Cocktail/S_{CB}"}, kLinear, kLinear, true, 0.5, 0.8, 0.85, 0.86, "hist");
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  // Compare common CB and signal with versions with pure UrQMD electrons
  {
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      string name = fHMean.GetName("cb/cbAndSignal_urqmd", step);
      TCanvas* c = fHMean.fHM.CreateCanvas(name.c_str(), name.c_str(), 1800, 900);
      TH1D* cb   = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* cbU  = fHMean.H1Clone("hMinvCombBg_urqmd", step);
      TH1D* sig  = fHMean.H1Clone("hMinvCombSignalMc", step);
      TH1D* sigU = fHMean.H1Clone("hMinvCombSignalMc_urqmd", step);
      cb->SetMinimum(1e-8);
      c->Divide(2, 1);
      c->cd(1);
      DrawH1({cb, cbU}, {"B_{C}", "B_{C} (UrQMD electrons)"}, kLinear, kLog, true, 0.6, 0.85, 0.95, 0.95, "p");
      fHMean.DrawAnaStepOnPad(step);
      c->cd(2);
      DrawH1({sig, sigU}, {"Signal (BG+Coc - B_{C})", "Signal (UrQMD electrons)"}, kLinear, kLog, true, 0.55, 0.85, 0.95, 0.95, "p");
      fHMean.DrawAnaStepOnPad(step);
    }
  }
}

void LmvmDrawAll::DrawSBgVsMinv()
{
  vector<TH1*> hists;
  for (ELmvmAnaStep step : {ELmvmAnaStep::TtCut, ELmvmAnaStep::PtCut}) {
    TH1D* bg = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
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
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    for (const string& cat : {"", "_urqmd"}) { // common CB or from pure UrQMD electrons
      if (step < ELmvmAnaStep::ElId) continue;
    
      // Geometrical mean
      for (const string& ev : {"_sameEv", "_mixedEv"}) {
        TH1D* pp   = fHMean.H1Clone("hMinvCombPP" + cat + ev, step);
        TH1D* mm   = fHMean.H1Clone("hMinvCombMM" + cat + ev, step);
        TH1D* geom = fHMean.CreateHByClone<TH1D>("hMinvCombMM" + cat + ev, "hMinvCombGeom" + cat + ev, step);

        for (int i = 1; i <= geom->GetNbinsX(); i++) {
          double cpp     = pp->GetBinContent(i);
          double cmm     = mm->GetBinContent(i);
          double content = std::sqrt(cpp * cmm);  // TODO: mind case if one of these values is 0
          geom->SetBinContent(i, content);
        }
      }

      // calculate k factor
      TH1D* k = fHMean.CreateHByClone<TH1D>("hMinvCombPM" + cat + "_mixedEv", "hMinvCombK" + cat, step);
      k->Divide(fHMean.H1("hMinvCombGeom" + cat + "_mixedEv", step));
      k->Scale(0.5);
          
      // calculate combinatorial BG from same (<= 0.3 GeV) and mixed (> 0.3 GeV) events data
        // at first, calculate norm. factor between same and mixed events in 300 - 1000 MeV/c2 interval
      TH1D* hGeomSame  = fHMean.H1("hMinvCombGeom" + cat + "_sameEv", step);
      TH1D* hGeomMixed = fHMean.H1("hMinvCombGeom" + cat + "_mixedEv", step);
      int minBin       = hGeomSame->FindBin(0.3);
      int maxBin       = hGeomSame->FindBin(1.0);
      double normGM    = hGeomSame->Integral(minBin, maxBin) / hGeomMixed->Integral(minBin, maxBin);

        // combinatorial BG
      TH1D* hBc =  fHMean.CreateHByClone<TH1D>("hMinvCombGeom" + cat + "_sameEv", "hMinvCombBg" + cat, step);
      for (int i = 0; i <= hBc->GetNbinsX(); i++) {
        double val = (i <= minBin) ? 2 * fHMean.H1("hMinvCombK" + cat, step)->GetBinContent(i) 
            * fHMean.H1("hMinvCombGeom" + cat + "_sameEv", step)->GetBinContent(i) 
                : normGM * fHMean.H1("hMinvCombPM" + cat + "_mixedEv", step)->GetBinContent(i);
        hBc->SetBinContent(i, val);
      }
    
      // calculate signal from CB subtraction
        // from 'N+-same': Signal = N+-same - B_{C}
      TH1D* hSigNpm = fHMean.CreateHByClone<TH1D>("hMinvCombPM" + cat + "_sameEv", "hMinvCombSignalNpm" + cat, step);
      hSigNpm->Add(fHMean.H1("hMinvCombBg" + cat, step), -1.);

        // from MC 'Cocktail + BG': Signal = Coc+BG - B_{C}
      TH1D* hSigCoc = fHMean.CreateHByClone<TH1D>("hMinv_bg", "hMinvCombSignalMc" + cat, step);
      hSigCoc->Add(GetCocktailMinvH1(step));
      hSigCoc->Add(fHMean.H1("hMinvCombBg" + cat, step), -1.);

      // calculate errors via error propagation formula
      int nofEvents = GetNofTotalEvents();
      double bW = fHMean.H1("hMinvCombPM" + cat + "_sameEv", step)->GetBinWidth(1);
      int nofBins = fHMean.H1("hMinvCombPM" + cat + "_sameEv", step)->GetNbinsX();
      for (int i = 1; i <= nofBins; i++) {
        //s_ for same, m_ for mixed
        double s_pm = fHMean.H1("hMinvCombPM" + cat + "_sameEv", step)->GetBinContent(i) * nofEvents * bW;
        double s_pp = fHMean.H1("hMinvCombPP" + cat + "_sameEv", step)->GetBinContent(i) * nofEvents * bW;
        double s_mm = fHMean.H1("hMinvCombMM" + cat + "_sameEv", step)->GetBinContent(i) * nofEvents * bW;
        double m_pm = fHMean.H1("hMinvCombPM" + cat + "_mixedEv", step)->GetBinContent(i) * nofEvents * bW;
        double m_pp = fHMean.H1("hMinvCombPP" + cat + "_mixedEv", step)->GetBinContent(i) * nofEvents * bW;
        double m_mm = fHMean.H1("hMinvCombMM" + cat + "_mixedEv", step)->GetBinContent(i) * nofEvents * bW;
        
	      // derivatives of B_{C} w.r. to according value
        double d_m_pm = std::sqrt(s_pp * s_mm / (m_pp * m_mm));
        double d_m_pp = -0.5 * m_pm * m_mm * std::sqrt(s_pp * s_mm) / (std::pow(std::sqrt(m_pp * m_mm), 3));
        double d_m_mm = -0.5 * m_pm * m_pp * std::sqrt(s_pp * s_mm) / (std::pow(std::sqrt(m_pp * m_mm), 3));
        double d_s_pp =  0.5 * m_pm * s_mm / std::sqrt(m_pp * m_mm * s_pp * s_mm);
        double d_s_mm =  0.5 * m_pm * s_pp / std::sqrt(m_pp * m_mm * s_pp * s_mm);

	      // contributions to error propagation
        double f_m_pm = std::pow(d_m_pm * std::sqrt(m_pm), 2);
        double f_m_pp = std::pow(d_m_pp * std::sqrt(m_pp), 2);
        double f_m_mm = std::pow(d_m_mm * std::sqrt(m_mm), 2);
        double f_s_pp = std::pow(d_s_pp * std::sqrt(s_pp), 2);
        double f_s_mm = std::pow(d_s_mm * std::sqrt(s_mm), 2);

	      // final error propagation values
        double errorBc   = std::sqrt(f_m_pm + f_m_pp + f_m_mm + f_s_pp + f_s_mm) / (nofEvents * bW);
        double errorBc2  = normGM * std::sqrt(m_pm) / (nofEvents * bW);  // for range > 0.3 GeV
        double errorSig  = std::sqrt(s_pm + std::pow(errorBc, 2)) / (nofEvents * bW);
        double errorSig2 = std::sqrt(s_pm + std::pow(errorBc2, 2)) / (nofEvents * bW);

        if (i <= minBin) fHMean.H1("hMinvCombBg" + cat, step)->SetBinError(i, errorBc);
        if (i >  minBin) fHMean.H1("hMinvCombBg" + cat, step)->SetBinError(i, errorBc2);
        if (i <= minBin) fHMean.H1("hMinvCombSignalNpm" + cat, step)->SetBinError(i, errorSig);
        if (i >  minBin) fHMean.H1("hMinvCombSignalNpm" + cat, step)->SetBinError(i, errorSig2);
        if (i <= minBin) fHMean.H1("hMinvCombSignalMc" + cat, step)->SetBinError(i, errorSig);
        if (i >  minBin) fHMean.H1("hMinvCombSignalMc" + cat, step)->SetBinError(i, errorSig2);
      }  // error calculation
    }  // cat ("" or "_urqmd")
  } // steps
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
