/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Elena Lebedeva */

#include "LmvmDraw.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "cbm/elid/CbmLitGlobalElectronId.h"
#include "utils/CbmUtils.h"

#include "TCanvas.h"
#include "TClass.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TLine.h"
#include "TMath.h"
#include "TPad.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TText.h"
#include <TLegend.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

ClassImp(LmvmDraw);

using namespace std;
using namespace Cbm;
LmvmDraw::LmvmDraw() {}

void LmvmDraw::DrawHistFromFile(const string& fileName, const string& outputDir, bool useMvd)
{
  SetDefaultDrawStyle();
  fOutputDir        = outputDir;
  fUseMvd           = useMvd;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* file = new TFile(fileName.c_str());
  fH.fHM.ReadFromFile(file);
  fNofEvents = (int) fH.fHM.H1("hEventNumber")->GetEntries();
  cout << "File name = " << fileName << endl;
  cout << "Number of events = " << fNofEvents << endl;

  fH.fHM.ScaleByPattern(".*", 1. / fNofEvents);
  RebinMinvHist();

  DrawAnaStepMany("pty_pair_signal", [this](ELmvmAnaStep step) { DrawPtY("hPtYPairSignal", step); });
  for (const auto& cand : fH.fCandNames) {
    const string& cName = "hPtY_cands/" + cand;
    const string& hName = "hPtY_cands_" + cand;
    DrawAnaStepMany(cName, [this, hName](ELmvmAnaStep step) { DrawPtY(hName, step); });
  }
  DrawAnaStepMany("pair_rapidity", [this](ELmvmAnaStep step) { DrawRapidity(step); });
  //DrawAnaStepMany("pair_pty_efficiency", [this](ELmvmAnaStep step) { DrawPtYEfficiency(step); });  // TODO: causes segmentation violation error
  DrawAnaStepMany("minv_sbg", [this](ELmvmAnaStep step) { DrawMinvSBg(step); });
  //DrawAnaStepMany("minv_bgPairSrc", [this](ELmvmAnaStep step) { DrawMinvBgPairSrc(step); });	     // TODO: causes segmentation violation error
  //DrawAnaStepMany("minv_matching", [this](ELmvmAnaStep step) { DrawMinvMatching(step); });         // TODO: causes segmentation violation error
  DrawAnaStepMany("minv_pt", [this](ELmvmAnaStep step) { DrawMinvPt(step); });
  DrawAnaStepMany("anglePair", [this](ELmvmAnaStep step) { DrawSrcAnaStepH1("hAnglePair", step); });

  // draw momentum histograms
  for (const string& hName : {"hMom", "hMomPx", "hMomPy", "hMomPz", "hPt", "hRapidity"}) {
    DrawAnaStepMany(hName, [this, hName](ELmvmAnaStep step) { DrawSrcAnaStepH1(hName, step); });
    DrawAnaStepMany(hName + "EpEm", [this, hName](ELmvmAnaStep step) { DrawSrcAnaStepEpEmH1(hName, step); });
  }
  DrawElPurity();
  DrawMisc();
  DrawGammaVertex();
  DrawCuts();
  DrawMinvAll();
  DrawBgSourceTracks();
  DrawBgSourcePairsAll();
  DrawMismatchesAndGhosts();
  DrawMvdCutQa();
  DrawMvdAndStsHist();
  DrawAccRecVsMom();  // TODO: finish this method!
  DrawPmtXY();
  DrawMinvBg();  // TODO: do not extra method
  DrawBetaMomSpectra();
  DrawCombinatorialPairs();
  SaveCanvasToImage();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

bool LmvmDraw::SkipMvd(ELmvmAnaStep step)
{
  return (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut));
}

void LmvmDraw::RebinMinvHist()
{
  int nGroup      = 20;
  int nGroupCB    = 50;  // rebin for CB histos
  int nGroupMatch = 50;
  int nGroupBgSrc = 50;
  fH.Rebin("hMinv", fH.fSrcNames, fH.fAnaStepNames, nGroup);
  fH.Rebin("hMinvCombPM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPP", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombMM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPM_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPP_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombMM_pluto", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPM_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPP_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombMM_urqmd", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvBgMatch", {"trueMatch", "trueMatchEl", "trueMatchNotEl", "mismatch"}, fH.fAnaStepNames, nGroupMatch);
  fH.Rebin("hMinvBgSource", fH.fBgPairSrcNames, fH.fAnaStepNames, nGroupBgSrc);
}

void LmvmDraw::DrawBetaMomSpectra()
{
  TH2D* hPos = fH.H2Clone("hBetaMom_cands_plutoEl+");
  TH2D* hEl  = fH.H2Clone("hBetaMom_cands_plutoEl-");
  TCanvas* c = fH.fHM.CreateCanvas("betaMom/", "betaMom/", 1600, 800);
  c->Divide(2, 1);
  c->cd(1);
  DrawH2(hEl, kLinear, kLinear, kLog, "colz");
  c->cd(2);
  DrawH2(hPos, kLinear, kLinear, kLog, "colz");
}

void LmvmDraw::DrawCutEffH1(const string& hist, const string& option)
{
  vector<TH1*> effHist;
  for (ELmvmSrc src : fH.fSrcs) {
    TH1D* eff            = fH.H1Clone(hist, src);
    int nBins            = eff->GetNbinsX();
    double integralTotal = fH.H1(hist, src)->Integral(1, nBins, "width");

    if (option == "right") {
      for (int iB = 1; iB <= nBins; iB++) {
        eff->SetBinContent(iB, fH.H1(hist, src)->Integral(1, iB, "width") / integralTotal);
        eff->GetYaxis()->SetTitle("Cut Efficiency");
      }
    }
    else if (option == "left") {
      for (int iB = nBins; iB >= 1; iB--) {
        eff->SetBinContent(iB, fH.H1(hist, src)->Integral(iB, nBins, "width") / integralTotal);
        eff->GetYaxis()->SetTitle("Cut Efficiency");
      }
    }
    effHist.push_back(eff);
  }
  DrawH1(effHist, fH.fSrcLatex, kLinear, kLinear, true, 0.8, 0.8, 0.99, 0.99, "hist");
}

void LmvmDraw::DrawAnaStepMany(const string& cName, function<void(ELmvmAnaStep)> drawFunc)
{
  int hi          = 1;
  string newCName = cName + "/" + cName + "_all";
  TCanvas* c      = fH.fHM.CreateCanvas(newCName.c_str(), newCName.c_str(), 1600, 1200);
  c->Divide(4, 3);
  for (const auto step : fH.fAnaSteps) {
    if (SkipMvd(step)) continue;
    c->cd(hi++);
    drawFunc(step);
  }

  for (const auto step : fH.fAnaSteps) {
    if (SkipMvd(step)) continue;
    newCName = cName + "/" + cName + "_" + fH.fAnaStepNames[static_cast<int>(step)];
    fH.fHM.CreateCanvas(newCName.c_str(), newCName.c_str(), 800, 800);
    drawFunc(step);
  }
}

void LmvmDraw::DrawPtY(const string& hist, ELmvmAnaStep step)
{
  TH2D* h   = fH.H2(hist.c_str(), step);
  TH2D* hmc = fH.H2(hist.c_str(), ELmvmAnaStep::Mc);
  DrawH2(h, kLinear, kLinear, kLinear, "COLZ");
  bool drawAnaStep = true;
  if (drawAnaStep) fH.DrawEfficiency(h, hmc, 0.2, 1.8);
  if (drawAnaStep) fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawRapidity(ELmvmAnaStep step)
{
  DrawH1(fH.H2("hPtYPairSignal", step)->ProjectionX(), kLinear, kLinear, "hist");
  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawPtYEfficiency(ELmvmAnaStep step)
{
  TH2D* h   = fH.H2("hPtYPairSignal", step);
  TH2D* hmc = fH.H2("hPtYPairSignal", ELmvmAnaStep::Mc);
  TH2D* eff = Cbm::DivideH2(h, hmc, "", 100., "Efficiency [%]");
  DrawH2(eff);
  eff->SetMaximum(10.);
  bool drawAnaStep = true;
  if (drawAnaStep) fH.DrawEfficiency(h, hmc, 0.2, 1.8);
  if (drawAnaStep) fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawMinvPt(ELmvmAnaStep step)
{
  TH2D* h = fH.H2("hMinvPt", ELmvmSrc::Signal, step);
  DrawH2(h, kLinear, kLinear, kLinear, "COLZ");
  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawSrcAnaStepH1(const string& hName, ELmvmAnaStep step)
{
  DrawSrcH1(hName, step, false);
  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawSrcAnaStepEpEmH1(const string& hName, ELmvmAnaStep step)
{
  vector<TH1*> hVec;
  vector<string> latex;
  for (const ELmvmSrc src : {ELmvmSrc::Signal, ELmvmSrc::Pi0, ELmvmSrc::Gamma}) {
    for (const string& pm : {"+", "-"}) {
      hVec.push_back(fH.H1(hName + pm, src, step));
      latex.push_back(fH.fSrcLatex[static_cast<int>(src)] + " (e" + pm + ")");
    }
  }
  DrawH1(hVec, latex, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99, "hist");
  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawMisc()
{
  fH.fHM.CreateCanvas("mom_pairSignal", "mom_pairSignal", 800, 800);
  DrawAnaStepH1("hMomPairSignal", true);

  fH.fHM.CreateCanvas("mother_pdg", "mother_pdg", 800, 800);
  DrawH1({fH.H1("hMotherPdg_mc"), fH.H1("hMotherPdg_acc")}, {"MC", "acc"}, kLinear, kLog, true, 0.7, 0.7, 0.99, 0.99,
         "hist");

  fH.fHM.CreateCanvas("momVsAngle_pairSignal", "momVsAngle_pairSignal", 800, 800);
  DrawH2(fH.H2("hMomVsAnglePairSignalMc"));
}

void LmvmDraw::DrawCombinatorialPairs()
{
  // Draw pair yields // TODO: scale to bin width
  string cbName = "CombPairsPluto/";

  {
    TCanvas* c3 =
      fH.fHM.CreateCanvas((cbName + "pairYields_steps").c_str(), (cbName + "pairYields_steps").c_str(), 1800, 600);
    c3->Divide(3, 1);
    c3->cd(1);
    DrawAnaStepH1("hMinvCombPM_pluto_mixedEv", true);
    DrawTextOnPad("e^{+}e^{-} pairs ", 0.35, 0.9, 0.65, 0.99);
    c3->cd(2);
    DrawAnaStepH1("hMinvCombPP_pluto_mixedEv", true);
    DrawTextOnPad("e^{+}e^{+} pairs ", 0.35, 0.9, 0.65, 0.99);
    c3->cd(3);
    DrawAnaStepH1("hMinvCombMM_pluto_mixedEv", true);
    DrawTextOnPad("e^{-}e^{-} pairs ", 0.35, 0.9, 0.65, 0.99);
  }

  {
    TCanvas* c = fH.fHM.CreateCanvas((cbName + "pairYields").c_str(), (cbName + "pairYields").c_str(), 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fH.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1* pm = fH.H1Clone("hMinvCombPM_pluto_mixedEv", step);
      TH1* pp = fH.H1Clone("hMinvCombPP_pluto_mixedEv", step);
      TH1* mm = fH.H1Clone("hMinvCombMM_pluto_mixedEv", step);
      DrawH1({pm, pp, mm},
             {"e^{+}e^{-} pairs (mixed ev.)", "e^{+}e^{+} pairs (mixed ev.)", "e^{-}e^{-} pairs (mixed ev.)"}, kLinear,
             kLog, true, 0.57, 0.79, 0.99, 0.99, "hist");
      fH.DrawAnaStepOnPad(step);
    }
  }

  // Draw ratio of e+e+/e-e- pairs
  {
    TCanvas* c = fH.fHM.CreateCanvas((cbName + "ratio_PPMM").c_str(), (cbName + "ratio_PPMM").c_str(), 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fH.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1* rat = fH.H1Clone("hMinvCombPP_pluto_mixedEv", step);
      rat->Divide(fH.H1("hMinvCombMM_pluto_mixedEv", step));
      rat->GetYaxis()->SetTitle("Ratio");
      DrawH1(rat, kLinear, kLinear, "hist");
      fH.DrawAnaStepOnPad(step);
      DrawTextOnPad("Ratio e^{+}e^{+} / e^{-}e^{-} ", 0.4, 0.80, 0.89, 0.89);
    }
  }

  // Draw geometric mean
  {
    TCanvas* c = fH.fHM.CreateCanvas((cbName + "geomMean").c_str(), (cbName + "geomMean").c_str(), 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fH.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1* gm = fH.H1Clone("hMinvCombPP_pluto_mixedEv", step);
      for (int iB = 1; iB <= gm->GetNbinsX(); iB++) {
        double pp  = fH.H1("hMinvCombPP_pluto_mixedEv", step)->GetBinContent(iB);
        double mm  = fH.H1("hMinvCombMM_pluto_mixedEv", step)->GetBinContent(iB);
        double con = std::sqrt(pp * mm);
        gm->SetBinContent(iB, con);
      }
      DrawH1(gm, kLinear, kLog, "hist");
      fH.DrawAnaStepOnPad(step);
    }
  }

  // draw k Factor
  {
    TCanvas* c = fH.fHM.CreateCanvas((cbName + "kFactor").c_str(), (cbName + "kFactor").c_str(), 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fH.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1* k  = fH.H1Clone("hMinvCombPM_pluto_mixedEv", step);
      TH1* gm = fH.H1Clone("hMinvCombPP_pluto_mixedEv", step);
      for (int iB = 1; iB <= k->GetNbinsX(); iB++) {
        double con = std::sqrt(fH.H1("hMinvCombPP_pluto_mixedEv", step)->GetBinContent(iB)
                               * fH.H1("hMinvCombMM_pluto_mixedEv", step)->GetBinContent(iB));
        gm->SetBinContent(iB, con);
      }
      k->Divide(gm);
      k->Scale(0.5);
      k->GetYaxis()->SetTitle("k Factor");
      DrawH1(k, kLinear, kLinear, "p");
      fH.DrawAnaStepOnPad(step);
    }
  }
}

void LmvmDraw::DrawPmtXY()
{
  TCanvas* c = fH.fHM.CreateCanvas("pmtXY", "pmtXY", 1800, 600);
  c->Divide(3, 1);
  vector<ELmvmSrc> src {ELmvmSrc::Signal, ELmvmSrc::Pi0, ELmvmSrc::Gamma};
  for (size_t i = 0; i < src.size(); i++) {
    c->cd(i + 1);
    DrawH2(fH.H2("hPmtXY", src[i]));
    gPad->SetLogz(true);
    DrawTextOnPad(fH.fSrcLatex[static_cast<int>(src[i])], 0.40, 0.9, 0.60, 0.99);
  }
}

void LmvmDraw::DrawSrcH1(const string& hName, ELmvmAnaStep step, bool doScale)
{
  vector<TH1*> hVec;
  for (const ELmvmSrc src : fH.fSrcs) {
    TH1D* h = (step == ELmvmAnaStep::Undefined) ? fH.H1(hName, src) : fH.H1(hName, src, step);
    h->SetLineWidth(2);
    h->SetLineColor(fH.fSrcColors[static_cast<int>(src)]);
    if (doScale) h->Scale(1. / h->Integral());
    hVec.push_back(h);
  }
  DrawH1(hVec, fH.fSrcLatex, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99, "hist");
}

void LmvmDraw::Draw1DCut(const string& hist, const string& sigOption, double cut)
{
  int w      = 2400;
  int h = 800;
  TCanvas* c = fH.fHM.CreateCanvas(("cuts/" + hist).c_str(), ("cuts/" + hist).c_str(), w, h);
  c->Divide(3, 1);

  c->cd(1);
  DrawSrcH1(hist);
  if (cut != -999999.) {
    TLine* cutLine = new TLine(cut, 0.0, cut, 1.0);
    cutLine->SetLineWidth(2);
    cutLine->Draw();
  }

  c->cd(2);
  DrawCutEffH1(hist, sigOption);

  c->cd(3);
  TH1D* sign = fH.CreateSignificanceH1(fH.H1(hist, ELmvmSrc::Signal), fH.H1(hist, ELmvmSrc::Bg), hist + "_significance",
                                       sigOption);
  DrawH1(sign, kLinear, kLinear, "hist");
}

void LmvmDraw::DrawCuts()
{
  Draw1DCut("hAnnRich", "left", -0.4);  // CbmLitGlobalElectronId::GetInstance().GetRichAnnCut()
  //Draw1DCut("hAnnTrd", "left", 0.1);    // CbmLitGlobalElectronId::GetInstance().GetTrdAnnCut() // TODO: uncomment when Trd Ann works again
  Draw2DCut("hTrdLike_El");
  Draw2DCut("hTrdLike_Pi");
  Draw2DCut("hAnnRichVsMom");
  Draw2DCut("hTofM2");

  Draw1DCut("hChi2PrimVertex", "right", fCuts.fChi2PrimCut);
  //Draw1DCut("hPt", "left", fCuts.fPtCut);
  //Draw1DCut("hMom", "left");
  Draw1DCut("hChi2Sts", "right");

  for (const string& type : {"all", "pion", "truePair"}) {
    Draw2DCut("hStCut_" + type, fCuts.fStCutPP, fCuts.fStCutAngle);
    Draw2DCut("hTtCut_" + type, fCuts.fTtCutPP, fCuts.fTtCutAngle);
    Draw2DCut("hRtCut_" + type, fCuts.fRtCutPP, fCuts.fRtCutAngle);
  }

  if (fUseMvd) {
    Draw2DCut("hMvdCut_1", fCuts.fMvd1CutD, fCuts.fMvd1CutP);
    Draw2DCut("hMvdCut_2", fCuts.fMvd2CutD, fCuts.fMvd2CutP);
  }
}


void LmvmDraw::DrawBgSourcePairs(ELmvmAnaStep step, bool inPercent, bool drawAnaStep)
{
  TH2D* h = fH.H2Clone("hSrcBgPairsEpEm", step);
  gStyle->SetPaintTextFormat("4.1f");
  string labels[4] = {"#gamma", "#pi^{0}", "#pi^{#pm}", "oth"};
  for (int i = 1; i <= 4; i++) {
    h->GetYaxis()->SetBinLabel(i, labels[i - 1].c_str());
    h->GetXaxis()->SetBinLabel(i, labels[i - 1].c_str());
  }
  h->SetMarkerSize(2.5);
  if (inPercent) {
    h->Scale(100. / h->Integral());
    h->GetZaxis()->SetTitle("[%]");
  }
  else {
    h->Scale(1000.);
    h->GetZaxis()->SetTitle("Number of pairs/event x10^{3}");
  }
  DrawH2(h, kLinear, kLinear, kLinear, "text COLZ");
  h->GetXaxis()->SetLabelSize(0.1);
  h->GetYaxis()->SetLabelSize(0.1);
  if (drawAnaStep) fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawBgSourcePairsAll()
{
  int hi      = 1;
  TCanvas* c1 = fH.fHM.CreateCanvas("bg/srcPairs_abs", "bg/srcPairs_abs", 1500, 1500);
  c1->Divide(3, 3);
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc || SkipMvd(step)) continue;
    c1->cd(hi++);
    DrawBgSourcePairs(step, false);
  }

  hi          = 1;
  TCanvas* c2 = fH.fHM.CreateCanvas("bg/srcPairs_perc", "bg/srcPairs_perc", 1500, 1500);
  c2->Divide(3, 3);
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc || SkipMvd(step)) continue;
    c2->cd(hi++);
    DrawBgSourcePairs(step, true);
  }

  string elidCutName = fH.fAnaStepNames[static_cast<int>(ELmvmAnaStep::ElId)];
  fH.fHM.CreateCanvas("bg/srcPairs_abs_" + elidCutName, "bg/srcPairs_abs_" + elidCutName, 1100, 800);
  DrawBgSourcePairs(ELmvmAnaStep::ElId, false, false);

  fH.fHM.CreateCanvas("bg/srcPairs_perc_" + elidCutName, "bg/srcPairs_perc_" + elidCutName, 1100, 800);
  DrawBgSourcePairs(ELmvmAnaStep::ElId, true, false);

  DrawSource2D("bg/srcPairs_2d", "hSrcBgPairs", fH.fBgPairSrcLatex, 1000., "Pairs per event x10^{3}");
}

void LmvmDraw::Draw2DCutTriangle(double xCr, double yCr)
{
  if (xCr == -999999. || yCr == -999999.) return;
  vector<TLine*> lines {new TLine(0., 0., xCr, 0.), new TLine(0., 0., 0., yCr), new TLine(xCr, 0., 0., yCr)};
  for (size_t i = 0; i < lines.size(); i++) {
    lines[i]->SetLineWidth(2.);
    lines[i]->Draw();
  }
}

void LmvmDraw::DrawMinvBg()
{
  TH1D* gg     = fH.H1Clone("hMinvBgSource2_elid_gg");
  TH1D* pipi   = fH.H1Clone("hMinvBgSource2_elid_pipi");
  TH1D* pi0pi0 = fH.H1Clone("hMinvBgSource2_elid_pi0pi0");
  TH1D* oo     = fH.H1Clone("hMinvBgSource2_elid_oo");
  TH1D* gpi    = fH.H1Clone("hMinvBgSource2_elid_gpi");
  TH1D* gpi0   = fH.H1Clone("hMinvBgSource2_elid_gpi0");
  TH1D* go     = fH.H1Clone("hMinvBgSource2_elid_go");
  TH1D* pipi0  = fH.H1Clone("hMinvBgSource2_elid_pipi0");
  TH1D* pio    = fH.H1Clone("hMinvBgSource2_elid_pio");
  TH1D* pi0o   = fH.H1Clone("hMinvBgSource2_elid_pi0o");

  cout << "Entries gg: " << gg->GetEntries() << endl;
  cout << "Entries pipi: " << pipi->GetEntries() << endl;
  cout << "Entries pi0pi0: " << pi0pi0->GetEntries() << endl;
  cout << "Entries oo: " << oo->GetEntries() << endl;
  cout << "Entries gpi: " << gpi->GetEntries() << endl;
  cout << "Entries gpi0: " << gpi0->GetEntries() << endl;
  cout << "Entries go: " << go->GetEntries() << endl;
  cout << "Entries pipi0: " << pipi0->GetEntries() << endl;
  cout << "Entries pio: " << pio->GetEntries() << endl;
  cout << "Entries pi0o: " << pi0o->GetEntries() << endl;

  int reb = 50;

  gg->Rebin(reb);
  pi0pi0->Rebin(reb);
  gpi0->Rebin(reb);
  go->Rebin(reb);
  pi0o->Rebin(reb);

  gg->Scale(1. / reb);
  pi0pi0->Scale(1. / reb);
  gpi0->Scale(1. / reb);
  go->Scale(1. / reb);
  pi0o->Scale(1. / reb);

  string cName = "minvBgSrc/minvBgSrc";
  //vector<string> names = {"#gamma-#gamma", "#pi^{#pm}-#pi^{#pm}", "#pi^{0}-#pi^{0}", "o.-o.", "#gamma-#pi^{#pm}", "#gamma-#pi^{0}", "#gamma-o.", "#pi^{#pm}-#pi^{0}", "#pi^{#pm}-o.", "#pi^{0}-o.", "misid. #pi^{#pm}"};
  vector<string> names = {"#gamma-#gamma", "#pi^{0}-#pi^{0}", "#gamma-#pi^{0}", "#gamma-o.", "#pi^{0}-o."};
  fH.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1000, 1000);
  //DrawH1({gg, pipi, pi0pi0, oo, gpi, gpi0, go, pipi0, pio, pi0o}, names, kLinear, kLog, true, 0.85, 0.7, 0.99, 0.99, "hist");
  DrawH1({gg, pi0pi0, gpi0, go, pi0o}, names, kLinear, kLog, true, 0.85, 0.7, 0.99, 0.99, "hist");
}

void LmvmDraw::DrawElPurity()
{
  // All occuring PIDs
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) continue;
    TCanvas* c = fH.fHM.CreateCanvas("purity/pid_" + fH.fAnaStepNames[static_cast<int>(step)],
                                     "purity/pid_" + fH.fAnaStepNames[static_cast<int>(step)], 1600, 800);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fH.H1("hCandPdg_" + fH.fAnaStepNames[static_cast<int>(step)]), kLinear, kLog, "hist text40");
    c->cd(2);
    TH1D* pdgZoom = fH.H1Clone("hCandPdg_" + fH.fAnaStepNames[static_cast<int>(step)]);
    pdgZoom->GetXaxis()->SetRangeUser(-20., 20.);
    DrawH1(pdgZoom, kLinear, kLog, "hist text40");
  }

  // PID vs momentum
  vector<string> yLabel = {"e^{#pm}_{PLUTO}", "e^{#pm}_{UrQMD}", "#pi^{#pm}", "p", "K^{+}", "o."};
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) continue;
    fH.fHM.CreateCanvas("purity/PidVsMom_" + fH.fAnaStepNames[static_cast<int>(step)],
                        "purity/PidVsMom_" + fH.fAnaStepNames[static_cast<int>(step)], 800, 600);
    TH2D* hPidMom = fH.H2("hCandPdgVsMom_" + fH.fAnaStepNames[static_cast<int>(step)]);
    hPidMom->SetMinimum(5e-7);
    DrawH2(hPidMom, kLinear, kLinear, kLog, "COLZ");
    for (size_t y = 1; y <= yLabel.size(); y++) {
      hPidMom->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
    }
    double nEl = hPidMom->Integral(1, hPidMom->GetXaxis()->GetNbins(), 2, 2);  // do not count PLUTO particles
    double purity =
      (nEl / hPidMom->Integral(1, hPidMom->GetXaxis()->GetNbins(), 2, hPidMom->GetYaxis()->GetNbins())) * 100;
    DrawTextOnPad("Purity: " + Cbm::NumberToString(purity, 1) + " %", 0.1, 0.9, 0.45, 0.99);
  }

  // Purity vs momentum
  int nBins    = fH.H2("hCandPdgVsMom_elid")->GetXaxis()->GetNbins();
  double xMin  = fH.H2("hCandPdgVsMom_elid")->GetXaxis()->GetXmin();
  double xMax  = fH.H2("hCandPdgVsMom_elid")->GetXaxis()->GetXmax();
  TH1D* purity = new TH1D("purity_Mom", "purity_Mom; P [GeV/c]; Purity [%]", nBins, xMin, xMax);
  for (int i = 1; i <= purity->GetNbinsX(); i++) {
    double nEl  = fH.H2("hCandPdgVsMom", ELmvmAnaStep::ElId)->GetBinContent(i, 2);
    double nAll = fH.H2("hCandPdgVsMom", ELmvmAnaStep::ElId)
                    ->Integral(i, i, 2, fH.H2("hCandPdgVsMom_elid")->GetYaxis()->GetNbins());
    double val = (nAll != 0) ? 100 * nEl / nAll : 0.;
    purity->SetBinContent(i, val);
  }
  purity->Rebin(5);
  purity->Scale(1. / 5.);
  fH.fHM.CreateCanvas("purity/purity_mom_elid", "purity/purity_mom_elid", 800, 800);
  DrawH1(purity, kLinear, kLinear, "pe");

  // Source of electron (PDG = +-11) candidates
  DrawSource2D("purity/SrcTracksEl_2d", "hCandElSrc",
               {"#gamma", "#pi^{0}", "#pi^{#pm}", "p", "K", "e^{#pm}_{sec}", "oth.", "signal"}, 1000.,
               "Tracks per event x10^{3}");

  // Occurency of Electrons and not-Electrons for various cut categories
  for (const string& hName : {"hAnnRichVsMomPur", "hTrdElLikePur"}) {
    string cName = "purity/cuts_" + hName;
    TCanvas* c   = fH.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 800);
    c->Divide(3, 1);
    int hi = 1;
    for (const string& id : {"El", "Bg"}) {
      TH2D* hist = fH.H2Clone(hName + "_" + id);
      c->cd(hi);
      DrawH2(hist, kLinear, kLinear, kLog, "COLZ");
      DrawTextOnPad(id.c_str(), 0.6, 0.89, 0.7, 0.99);
      hi++;
    }
    c->cd(hi);
    TH2D* ratio = fH.H2Clone(hName + "_El");
    ratio->Divide(fH.H2(hName + "_Bg"));
    DrawH2(ratio, kLinear, kLinear, kLog, "COLZ");
    DrawTextOnPad("Ratio El/Bg", 0.4, 0.85, 0.8, 0.99);
  }
}

void LmvmDraw::Draw2DCut(const string& hist, double cutCrossX, double cutCrossY)
{
  TCanvas* c = fH.fHM.CreateCanvas(("cuts/" + hist).c_str(), ("cuts/" + hist).c_str(), 1000, 1500);
  c->Divide(2, 3);
  vector<TH1*> projX, projY;
  projX.clear();  // TODO: clearing needed?
  projY.clear();
  vector<string> latex;
  for (ELmvmSrc src : {ELmvmSrc::Signal, ELmvmSrc::Bg, ELmvmSrc::Pi0, ELmvmSrc::Gamma}) {
    int srcInt = static_cast<int>(src);
    c->cd(srcInt + 1);
    DrawH2(fH.H2(hist, src));
    double nofPerEvent = fH.H2(hist, src)->GetEntries() / (double) fNofEvents;
    DrawTextOnPad((Cbm::NumberToString(nofPerEvent, 2) + "/ev."), 0.1, 0.9, 0.5, 0.99);
    DrawTextOnPad(fH.fSrcLatex[srcInt], 0.6, 0.89, 0.7, 0.99);
    Draw2DCutTriangle(cutCrossX, cutCrossY);
    projX.push_back(fH.H2(hist, src)->ProjectionX((hist + fH.fSrcLatex[static_cast<int>(src)]).c_str(), 1,
                                                  fH.H2(hist, src)->GetYaxis()->GetNbins(), ""));
    projY.push_back(fH.H2(hist, src)->ProjectionY());
    latex.push_back(fH.fSrcLatex[srcInt]);
  }

  c->cd(5);
  DrawH1(projX, latex, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99, "hist");

  c->cd(6);
  DrawH1(projY, latex, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99, "hist");
}

void LmvmDraw::DrawGammaVertex()
{
  vector<TH2D*> xyz {fH.H2("hVertexGammaXZ", ELmvmAnaStep::Mc), fH.H2("hVertexGammaYZ", ELmvmAnaStep::Mc),
                     fH.H2("hVertexGammaXY", ELmvmAnaStep::Mc)};

  TCanvas* c = fH.fHM.CreateCanvas("vertexGamma_mc", "vertexGamma_mc", 1800, 600);
  c->Divide(3, 1);
  for (size_t i = 0; i < xyz.size(); i++) {
    c->cd(i + 1);
    DrawH2(xyz[i]);
    xyz[i]->SetMinimum(1e-3);
    gPad->SetLogz(true);
  }

  TCanvas* cZ = fH.fHM.CreateCanvas("vertexGamma_z", "vertexGamma_z", 1500, 750);
  cZ->Divide(2, 1);
  int counter = 1;
  for (ELmvmAnaStep step : {ELmvmAnaStep::Mc, ELmvmAnaStep::PtCut}) {
    cZ->cd(counter++);
    string name = fH.GetName("hVertexGammaXZ", step);
    TH1D* zProj = (TH1D*) fH.H2(name)->ProjectionX((name + "pz").c_str())->Clone();
    zProj->GetYaxis()->SetTitle("Counter per event");
    zProj->GetXaxis()->SetRangeUser(-2., 17.);
    DrawH1(zProj, kLinear, kLinear, "hist");
    fH.DrawAnaStepOnPad(step);
  }

  TCanvas* cZoom = fH.fHM.CreateCanvas("vertexGamma_mc_zoom", "vertexGamma_mc_zoom", 1800, 600);
  cZoom->Divide(3, 1);
  for (size_t i = 0; i < xyz.size(); i++) {
    TH2D* hZoom = (TH2D*) xyz[i]->Clone();
    cZoom->cd(i + 1);
    hZoom->GetXaxis()->SetRangeUser(-1., 11.);
    hZoom->GetYaxis()->SetRangeUser(-10., 10.);
    DrawH2(hZoom);
    hZoom->SetMinimum(1e-3);
    gPad->SetLogz(true);
  }

  fH.fHM.CreateCanvas("vertexGamma_rz_mc", "vertexGamma_rz_mc", 900, 900);
  DrawH2(fH.H2("hVertexGammaRZ", ELmvmAnaStep::Mc));
  fH.H2("hVertexGammaRZ", ELmvmAnaStep::Mc)->SetMinimum(1e-3);
  gPad->SetLogz(true);
}

void LmvmDraw::DrawAnaStepH1(const string& name, bool logy)
{
  double min   = std::numeric_limits<Double_t>::max();
  double max   = std::numeric_limits<Double_t>::min();
  TH1D* h0     = nullptr;
  TLegend* leg = new TLegend(0.80, 0.32, 0.99, 0.99);
  for (const auto step : fH.fAnaSteps) {
    if (SkipMvd(step)) continue;
    TH1D* h = fH.H1(name, step);
    LOG(info) << name << " " << h->GetEntries();
    if (h == nullptr || h->GetEntries() <= 0) continue;
    leg->AddEntry(h, fH.fAnaStepLatex[static_cast<int>(step)].c_str(), "l");
    DrawH1(h, kLinear, kLinear, (h0 == nullptr) ? "hist" : "hist,same", fH.fAnaStepColors[static_cast<int>(step)]);
    if (h0 == nullptr) h0 = h;
    min = std::min(h->GetMinimum(), min);
    max = std::max(h->GetMaximum(), max);
    LOG(info) << name << " min:" << h->GetMinimum() << " max:" << h->GetMaximum();
  }
  if (min == 0.) min = std::min(1e-8, max * 1e-6);
  if (h0 != nullptr) h0->SetMinimum(min);
  if (h0 != nullptr) h0->SetMaximum(1.1 * max);

  leg->SetFillColor(kWhite);
  leg->Draw();

  gPad->SetGridx(true);
  gPad->SetGridy(true);
  gPad->SetLogy(logy);
}

void LmvmDraw::DrawMinvAll()
{
  TCanvas* c1 = fH.fHM.CreateCanvas("minv_sbg_anaStep", "minv_sbg_anaStep", 1200, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Signal), true);
  c1->cd(2);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Bg), true);

  TCanvas* c2 = fH.fHM.CreateCanvas("minv_pi0_eta_gamma_anaStep", "minv_pi0_eta_gamma_anaStep", 1800, 600);
  c2->Divide(3, 1);
  c2->cd(1);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Pi0), true);
  c2->cd(2);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Eta), true);
  c2->cd(3);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Gamma), true);
}

void LmvmDraw::DrawMinvSBg(ELmvmAnaStep step)
{
  TH1D* s   = fH.H1Clone("hMinv", ELmvmSrc::Signal, step);
  TH1D* bg  = fH.H1Clone("hMinv", ELmvmSrc::Bg, step);
  TH1D* sbg = static_cast<TH1D*>(bg->Clone());
  sbg->Add(s);
  sbg->SetMinimum(1e-8);

  DrawH1({sbg, bg, s}, {"", "", ""}, kLinear, kLog, false, 0, 0, 0, 0, "Hist L");
  s->SetFillColor(kRed);
  s->SetLineColor(kBlack);
  s->SetLineWidth(1);
  s->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
  bg->SetFillColor(kYellow - 10);
  bg->SetLineColor(kBlack);
  bg->SetLineWidth(2);
  bg->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
  sbg->SetFillColor(kBlue);
  sbg->SetLineColor(kBlack);
  sbg->SetLineWidth(1);
  sbg->SetLineStyle(CbmDrawingOptions::MarkerStyle(1));
  s->SetMarkerStyle(1);
  bg->SetMarkerStyle(1);
  sbg->SetMarkerStyle(1);

  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawMinvBgPairSrc(ELmvmAnaStep step)
{
  double nofBg = fH.H1("hMinv", ELmvmSrc::Bg, step)->GetEntries();
  vector<TH1*> hists;
  vector<string> latex;
  for (int i = 0; i < fH.fNofBgPairSrc; i++) {
    hists.push_back(
      fH.H1("hMinvBgSource_" + fH.fBgPairSrcNames[i] + "_"
            + fH.fAnaStepNames[static_cast<int>(
              step)]));  // segmentation violation error is caused by this specific histogram; works with others
    string perc = Cbm::NumberToString(100. * hists[i]->GetEntries() / nofBg, 1);
    latex.push_back(fH.fBgPairSrcLatex[i] + "(" + perc + "%)");
  }
  DrawH1(hists, latex, kLinear, kLinear, true, 0.7, 0.45, 0.99, 0.9, "hist");
  bool drawAnaStep = true;
  if (drawAnaStep) fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawMinvMatching(ELmvmAnaStep step)
{
  double nofBg = fH.H1("hMinv", ELmvmSrc::Bg, step)->GetEntries();
  vector<TH1*> hists;
  vector<string> latex {"true match", "true match (e^{#pm})", "true match (not e^{#pm})", "mismatch "};
  int i = 0;
  for (const string& subName : {"trueMatch", "trueMatchEl", "trueMatchNotEl", "mismatch"}) {
    TH1D* h  = fH.H1("hMinvBgMatch_" + subName, step);
    latex[i] = latex[i] + " (" + Cbm::NumberToString(100. * h->GetEntries() / nofBg, 1) + "%)";
    hists.push_back(h);
    i++;
  }
  DrawH1(hists, latex, kLinear, kLinear, true, 0.4, 0.6, 0.99, 0.9, "hist");
  fH.DrawAnaStepOnPad(step);
}

void LmvmDraw::DrawAccRecVsMom()
{
  // Acceptance and reconstruction yields cs. momentum for various detector combinations
  for (const int& pdg : {11, 211, 2212, 321}) {
    vector<string> subNames {"mc", "acc", "recSts", "recStsRich", "recStsRichTrd", "recStsRichTrdTof"};
    vector<string> latex {
      "MC", "Acc", "Rec in STS", "Rec in STS-RICH", "Rec in STS-RICH-TRD", "Rec in STS-RICH-TRD-TOF"};
    vector<string> latexAll(latex.size()), latexPrim(latex.size());
    string ptcl = (pdg == 11) ? "hEl" : (pdg == 211) ? "hPi" : (pdg == 2212) ? "hProton" : "hKaon";

    vector<TH1*> histsAll, histsPrim;
    int i = 0;

    for (const string& subName : subNames) {
      TH1D* hAll = fH.H1(ptcl + "Mom_all_" + subName);
      hAll->SetMinimum(3e-6);
      hAll->SetMaximum(50);
      latexAll[i] = latex[i] + " (" + Cbm::NumberToString(hAll->GetEntries() / fNofEvents, 2) + "/ev.)";
      histsAll.push_back(hAll);

      TH1D* hPrim = fH.H1(ptcl + "Mom_prim_" + subName);
      hPrim->SetMinimum(3e-6);
      hPrim->SetMaximum(50);
      latexPrim[i] = latex[i] + " (" + Cbm::NumberToString(hPrim->GetEntries() / fNofEvents, 2) + "/ev.)";
      histsPrim.push_back(hPrim);
      i++;
    }

    //if (pdg == 321) continue; TODO: with kaons?
    double y1    = 0.17;  //(pdg == 211) ? 0.20 : 0.74;
    double y2 = 0.42;  //(pdg == 211) ? 0.45 : 0.99;
    string cName = "AccRecMom/" + ptcl + "Mom";
    fH.fHM.CreateCanvas(cName, cName, 900, 900);
    DrawH1(histsAll, latexAll, kLinear, kLog, true, 0.4, y1, 0.95, y2, "hist");

    fH.fHM.CreateCanvas(cName + "_prime", cName + "_prime", 900, 900);
    DrawH1(histsPrim, latexPrim, kLinear, kLog, true, 0.4, y1, 0.95, y2, "hist");
  }

  // Acceptance in single detectors
  for (const string& det : {"sts", "rich", "trd", "tof"}) {
    vector<TH1*> hVec;
    vector<string> latex;
    for (const ELmvmSrc src : {ELmvmSrc::Signal, ELmvmSrc::Pi0, ELmvmSrc::Gamma}) {
      for (const string& pm : {"+", "-"}) {
        hVec.push_back(fH.H1("hMomAcc" + pm + "_" + det, src));
        latex.push_back(fH.fSrcLatex[static_cast<int>(src)] + " (e" + pm + ")");
      }
    }
    fH.fHM.CreateCanvas("AccRecMom/momDetAcc_" + det, "AccRecMom/momDetAcc_" + det, 800, 800);
    DrawH1(hVec, latex, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99, "hist");
    DrawTextOnPad(det, 0.4, 0.9, 0.6, 0.999);
  }
}

void LmvmDraw::DrawSource2D(const string& cName, const string& hName, const vector<string>& yLabels, double scale,
                            const string& zTitle)
{
  fH.fHM.CreateCanvas((cName + "_abs").c_str(), (cName + "_abs").c_str(), 900, 600);
  TH2D* habs = fH.H2Clone(hName);
  habs->SetStats(false);
  habs->Scale(scale);
  habs->SetMinimum(1e-2);
  habs->GetZaxis()->SetTitle(zTitle.c_str());
  habs->SetMarkerSize(1.4);
  DrawH2(habs, kLinear, kLinear, kLog, "text COLZ");

  fH.fHM.CreateCanvas((cName + "_perc").c_str(), (cName + "_perc").c_str(), 900, 600);
  TH2D* hperc = fH.H2Clone(hName);
  hperc->SetStats(false);
  for (int x = 1; x <= hperc->GetNbinsX(); x++) {
    // calculate total number of BG tracks (pairs) for a current step
    double nbg = 0.;
    for (int y = 1; y <= hperc->GetNbinsY(); y++) {
      nbg += habs->GetBinContent(x, y);
    }
    double sc = 100. / (nbg / scale);
    for (int y = 1; y <= hperc->GetNbinsY(); y++) {
      double val = sc * hperc->GetBinContent(x, y);
      hperc->SetBinContent(x, y, val);
    }
  }
  hperc->GetZaxis()->SetTitle("[%]");
  hperc->GetYaxis()->SetLabelSize(0.06);
  hperc->SetMarkerColor(kBlack);
  hperc->SetMarkerSize(1.4);
  DrawH2(hperc, kLinear, kLinear, kLinear, "text COLZ");

  for (size_t y = 1; y <= yLabels.size(); y++) {
    hperc->GetYaxis()->SetBinLabel(y, yLabels[y - 1].c_str());
    habs->GetYaxis()->SetBinLabel(y, yLabels[y - 1].c_str());
  }

  SetAnalysisStepAxis(hperc);
  SetAnalysisStepAxis(habs);
}

void LmvmDraw::DrawBgSourceTracks()
{
  gStyle->SetPaintTextFormat("4.1f");

  fH.fHM.CreateCanvas("bg/nofBgTracks", "bg/nofBgTracks", 900, 900);
  TH1D* hbg = fH.H1Clone("hNofBgTracks");
  hbg->Scale(10);
  hbg->GetYaxis()->SetTitle("Tracks/event x10");
  DrawH1(hbg, kLinear, kLog, "hist text0");
  hbg->SetMarkerSize(2.);

  fH.fHM.CreateCanvas("signal/nofSignalTracks", "signal/nofSignalTracks", 900, 900);
  TH1D* hel = fH.H1("hNofSignalTracks");
  DrawH1(hel, kLinear, kLog, "hist");

  fH.fHM.CreateCanvas("purity", "purity", 900, 900);
  TH1D* purity = new TH1D("purity", "Purity;Analysis steps;Purity", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  purity->Divide(fH.H1("hNofBgTracks"), fH.H1("hNofSignalTracks"));
  DrawH1(purity, kLinear, kLog, "hist text30");
  purity->SetMarkerSize(1.9);

  SetAnalysisStepAxis(hbg);
  SetAnalysisStepAxis(hel);
  SetAnalysisStepAxis(purity);

  DrawSource2D("bg/SrcTracksBg_2d", "hBgSrcTracks",
               {"#gamma", "#pi^{0}", "#pi^{#pm}", "p", "K", "e^{#pm}_{sec}", "oth.", "signal"}, 1000.,
               "Tracks per event x10^{3}");

  TCanvas* c = fH.fHM.CreateCanvas("nofTopoPairs", "nofTopoPairs", 1600, 800);
  c->Divide(2, 1);
  int i = 1;
  for (const string& p : {"gamma", "pi0"}) {
    c->cd(i++);
    TH1D* hTopo = fH.H1Clone("hNofTopoPairs_" + p);
    hTopo->Scale(1. / hTopo->Integral());
    DrawH1(hTopo, kLinear, kLinear, "hist");
    hTopo->SetMarkerSize(1.);
  }
}

void LmvmDraw::DrawMismatchesAndGhosts()
{
  gStyle->SetPaintTextFormat("4.1f");
  TCanvas* c1 = fH.fHM.CreateCanvas("nofMismatches", "nofMismatches", 1500, 1500);
  c1->Divide(2, 2);
  vector<string> dets {"all", "rich", "trd", "tof"};
  for (size_t i = 0; i < dets.size(); i++) {
    c1->cd(i + 1);
    TH1D* h = fH.H1Clone("hNofMismatches_" + dets[i]);
    h->Scale(10);
    h->GetYaxis()->SetTitle(("Mismatch tracks (" + dets[i] + ")/event x10").c_str());
    DrawH1(h, kLinear, kLog, "hist text0");
    h->SetMarkerSize(2.);
    SetAnalysisStepAxis(h);
  }

  fH.fHM.CreateCanvas("nofGhosts", "nofGhosts", 900, 900);
  DrawH1(fH.H1("hNofGhosts"), kLinear, kLog, "hist");
  SetAnalysisStepAxis(fH.H1("hNofGhosts"));
}

void LmvmDraw::SetAnalysisStepAxis(TH1* h)
{
  // Shift histogram content by 2 bins if MVD was not used
  if (!fUseMvd) {
    for (int step = static_cast<int>(ELmvmAnaStep::Mvd1Cut) + 1; step <= fH.fNofAnaSteps - 2; step++) {
      if (h->IsA() == TH2D::Class()) {
        for (int y = 1; y <= h->GetYaxis()->GetNbins(); y++) {
          h->SetBinContent(step, y, h->GetBinContent(step + 2, y));
        }
      }
      else if (h->IsA() == TH1D::Class()) {
        h->SetBinContent(step, h->GetBinContent(step + 2));
      }
    }
  }

  int rangeMax = fH.fNofAnaSteps;
  if (!fUseMvd) { rangeMax = rangeMax - 2; }
  h->GetXaxis()->SetRange(static_cast<int>(ELmvmAnaStep::Reco) + 1, rangeMax);
  h->GetXaxis()->SetLabelSize(0.06);
  int x = 1;
  for (const auto step : fH.fAnaSteps) {
    if (SkipMvd(step)) continue;
    h->GetXaxis()->SetBinLabel(x, fH.fAnaStepLatex[static_cast<int>(step)].c_str());
    x++;
  }
}

void LmvmDraw::DrawMvdCutQa()
{
  if (!fUseMvd) return;
  TCanvas* c = fH.fHM.CreateCanvas("cuts/mvdCutQa", "cuts/mvd1cut_qa", 1600, 800);
  c->Divide(2, 1);
  int i = 1;
  for (const string& num : {"1", "2"}) {
    c->cd(i++);
    DrawSrcH1("hMvdCutQa_" + num);
    TH1D* h1 = fH.H1("hMvdCutQa_" + num + "_" + fH.fSrcNames[0]);
    h1->GetXaxis()->SetLabelSize(0.06);
    h1->GetXaxis()->SetBinLabel(1, "Correct");
    h1->GetXaxis()->SetBinLabel(2, "Wrong");
    gPad->SetLogy(false);
    DrawTextOnPad("MVD " + num, 0.50, 0.90, 0.70, 0.99);
  }
}

void LmvmDraw::DrawMvdAndStsHist()
{
  if (!fUseMvd) return;
  TCanvas* c1 = fH.fHM.CreateCanvas("nofHitsMvdSts", "nofHitsMvdSts", 1600, 800);
  c1->Divide(2, 1);
  c1->cd(1);
  DrawSrcH1("hNofMvdHits");
  c1->cd(2);
  DrawSrcH1("hNofStsHits");

  Draw2DCut("hMvdXY_1");
  fH.fHM.CreateCanvas("mvd1", "mvd1", 900, 900);
  DrawSrcH1("hMvdR_1");

  Draw2DCut("hMvdXY_2");
  fH.fHM.CreateCanvas("mvd2", "mvd2", 900, 900);
  DrawSrcH1("hMvdR_2");
}

void LmvmDraw::SaveCanvasToImage()
{
  fH.fHM.SaveCanvasToImage(fOutputDir, "png");  // fHM->SaveCanvasToImage(fOutputDir, "png;eps");
}
