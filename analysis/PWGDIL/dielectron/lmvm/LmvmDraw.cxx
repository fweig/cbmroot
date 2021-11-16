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

void LmvmDraw::DrawHistFromFile(const string& fileName, const string& outputDir, bool useMvd, bool drawSig)
{
  SetDefaultDrawStyle();
  fOutputDir        = outputDir;
  fUseMvd           = useMvd;
  fDrawSignificance = drawSig;

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
  DrawAnaStepMany("lmvm_pair_pty", [this](ELmvmAnaStep step) { DrawPtY(step); });
  DrawAnaStepMany("lmvm_pair_rapidity", [this](ELmvmAnaStep step) { DrawRapidity(step); });
  //DrawAnaStepMany("lmvm_pair_pty_efficiency", [this](ELmvmAnaStep step) { DrawPtYEfficiency(step); });  // TODO: causes segmentation violation error
  DrawAnaStepMany("lmvm_minv_sbg", [this](ELmvmAnaStep step) { DrawMinvSBg(step); });
  //DrawAnaStepMany("lmvm_minv_bgPairSrc", [this](ELmvmAnaStep step) { DrawMinvBgPairSrc(step); });	      // TODO: causes segmentation violation error
  //DrawAnaStepMany("lmvm_minv_matching", [this](ELmvmAnaStep step) { DrawMinvMatching(step); });         // TODO: causes segmentation violation error
  DrawAnaStepMany("lmvm_minv_pt", [this](ELmvmAnaStep step) { DrawMinvPt(step); });
  DrawAnaStepMany("lmvm_anglePair", [this](ELmvmAnaStep step) { DrawSrcAnaStepH1("hAnglePair", step); });

  // draw momentum histograms
  for (const string& hName : {"hMom", "hMomPx", "hMomPy", "hMomPz", "hPt", "hRapidity"}) {
    DrawAnaStepMany("lmvm_" + hName, [this, hName](ELmvmAnaStep step) { DrawSrcAnaStepH1(hName, step); });
    DrawAnaStepMany("lmvm_" + hName + "EpEm", [this, hName](ELmvmAnaStep step) { DrawSrcAnaStepEpEmH1(hName, step); });
  }
  DrawMomAccEpEm();
  DrawMisc();
  DrawGammaVertex();
  DrawCuts();
  DrawMinvAll();
  DrawBgSourceTracks();
  DrawMismatchesAndGhosts();
  DrawMvdCutQa();
  DrawMvdAndStsHist();
  DrawPiMom();
  DrawPmtXY();
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
  int nGroupCB    = 10;  // rebin for CB histos
  int nGroupMatch = 50;
  int nGroupBgSrc = 50;
  fH.Rebin("hMinv", fH.fSrcNames, fH.fAnaStepNames, nGroup);
  fH.Rebin("hMinvCombPM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombPP", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvCombMM", {"sameEv", "mixedEv"}, fH.fAnaStepNames, nGroupCB);
  fH.Rebin("hMinvBgMatch", {"trueMatch", "trueMatchEl", "trueMatchNotEl", "mismatch"}, fH.fAnaStepNames, nGroupMatch);
  fH.Rebin("hMinvBgSource", fH.fBgPairSrcNames, fH.fAnaStepNames, nGroupBgSrc);
}

TH1D* LmvmDraw::CreateSignificanceH1(TH1D* s, TH1D* bg, const string& name, const string& option)
{
  int nBins  = s->GetNbinsX();
  TH1D* hsig = new TH1D(name.c_str(), name.c_str(), nBins, s->GetXaxis()->GetXmin(), s->GetXaxis()->GetXmax());
  hsig->GetXaxis()->SetTitle(s->GetXaxis()->GetTitle());

  // "right" - reject right part of the histogram. value > cut -> reject
  if (option == "right") {
    for (int i = 1; i <= nBins; i++) {
      double sumSignal = s->Integral(1, i, "width");
      double sumBg     = bg->Integral(1, i, "width");
      double sign      = (sumSignal + sumBg != 0.) ? sumSignal / std::sqrt(sumSignal + sumBg) : 0.;
      hsig->SetBinContent(i, sign);
    }
  }
  // "left" - reject left part of the histogram. value < cut -> reject
  else if (option == "left") {
    for (int i = nBins; i >= 1; i--) {
      double sumSignal = s->Integral(i, nBins, "width");
      double sumBg     = bg->Integral(i, nBins, "width");
      double sign      = (sumSignal + sumBg != 0.) ? sumSignal / std::sqrt(sumSignal + sumBg) : 0.;
      hsig->SetBinContent(i, sign);
    }
  }
  return hsig;
}

TH2D* LmvmDraw::CreateSignificanceH2(TH2D* signal, TH2D* bg, const string& name, const string& title)
{
  double xmin  = 1.0;
  double xmax  = 5.0;
  double ymin  = 1.0;
  double ymax  = 5.0;
  double delta = 0.1;
  int nStepsX  = (int) ((xmax - xmin) / delta);
  int nStepsY  = (int) ((ymax - ymin) / delta);

  TH2D* hsig = new TH2D(name.c_str(), title.c_str(), nStepsX, xmin, xmax, nStepsY, ymin, ymax);

  int binX = 1;
  for (double xcut = xmin; xcut <= xmax; xcut += delta, binX++) {
    int binY = 1;
    for (double ycut = ymin; ycut <= ymax; ycut += delta, binY++) {
      double sumSignal = 0;
      double sumBg     = 0;
      for (int ix = 1; ix <= signal->GetNbinsX(); ix++) {
        for (int iy = 1; iy <= signal->GetNbinsY(); iy++) {
          double xc  = signal->GetXaxis()->GetBinCenter(ix);
          double yc  = signal->GetYaxis()->GetBinCenter(iy);
          double val = -1 * (ycut / xcut) * xc + ycut;

          if (!(xc < xcut && yc < val)) {
            sumSignal += signal->GetBinContent(ix, iy);
            sumBg += bg->GetBinContent(ix, iy);
          }
        }
      }
      double sign = (sumSignal + sumBg != 0.) ? sumSignal / std::sqrt(sumSignal + sumBg) : 0.;
      hsig->SetBinContent(binX, binY, sign);
    }
  }
  return hsig;
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

void LmvmDraw::DrawPtY(ELmvmAnaStep step)
{
  TH2D* h   = fH.H2("hPtYPairSignal", step);
  TH2D* hmc = fH.H2("hPtYPairSignal", ELmvmAnaStep::Mc);
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

void LmvmDraw::DrawMomAccEpEm()
{
  for (const string& det : {"sts", "rich", "trd", "tof"}) {
    vector<TH1*> hVec;
    vector<string> latex;
    for (const ELmvmSrc src : {ELmvmSrc::Signal, ELmvmSrc::Pi0, ELmvmSrc::Gamma}) {
      for (const string& pm : {"+", "-"}) {
        hVec.push_back(fH.H1("hMomAcc" + pm + "_" + det, src));
        latex.push_back(fH.fSrcLatex[static_cast<int>(src)] + " (e" + pm + ")");
      }
    }
    fH.fHM.CreateCanvas("lmvm_momDetAcc/lmvm_momDetAcc_" + det, "lmvm_momDetAcc/lmvm_momDetAcc_" + det, 800, 800);
    DrawH1(hVec, latex, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99, "hist");
    DrawTextOnPad(det, 0.4, 0.9, 0.6, 0.999);
  }
}

void LmvmDraw::DrawMisc()
{
  fH.fHM.CreateCanvas("lmvm_mom_pairSignal", "lmvm_mom_pairSignal", 800, 800);
  DrawAnaStepH1("hMomPairSignal", true);

  fH.fHM.CreateCanvas("lmvm_mother_pdg", "lmvm_mother_pdg", 800, 800);
  DrawH1({fH.H1("hMotherPdg_mc"), fH.H1("hMotherPdg_acc")}, {"MC", "acc"}, kLinear, kLog, true, 0.7, 0.7, 0.99, 0.99,
         "hist");

  fH.fHM.CreateCanvas("lmvm_momVsAngle_pairSignal", "lmvm_momVsAngle_pairSignal", 800, 800);
  DrawH2(fH.H2("hMomVsAnglePairSignalMc"));
}

void LmvmDraw::DrawPmtXY()
{
  TCanvas* c = fH.fHM.CreateCanvas("lmvm_pmtXY", "lmvm_pmtXY", 1800, 600);
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
  int w = 800;
  int h = 800;
  if (fDrawSignificance) w = 1600;
  TCanvas* c = fH.fHM.CreateCanvas(("lmvm_" + hist).c_str(), ("lmvm_" + hist).c_str(), w, h);
  if (fDrawSignificance) {
    c->Divide(2, 1);
    c->cd(1);
  }
  DrawSrcH1(hist);
  if (cut != -999999.) {
    TLine* cutLine = new TLine(cut, 0.0, cut, 1.0);
    cutLine->SetLineWidth(2);
    cutLine->Draw();
  }
  if (fDrawSignificance) {
    c->cd(2);
    TH1D* sign =
      CreateSignificanceH1(fH.H1(hist, ELmvmSrc::Signal), fH.H1(hist, ELmvmSrc::Bg), hist + "_significance", sigOption);
    DrawH1(sign, kLinear, kLinear, "hist");
  }
}

void LmvmDraw::DrawCuts()
{
  Draw1DCut("hRichAnn", "left", -0.4);  // CbmLitGlobalElectronId::GetInstance().GetRichAnnCut()
  Draw1DCut("hTrdAnn", "left", 0.85);   // CbmLitGlobalElectronId::GetInstance().GetTrdAnnCut()
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

void LmvmDraw::DrawSrcBgPairs(ELmvmAnaStep step, bool inPercent, bool drawAnaStep)
{
  TH2D* h = fH.H2Clone("hSrcBgPairsEpEm", step);
  gStyle->SetPaintTextFormat("4.1f");
  string labels[3] = {"#gamma", "#pi^{0}", "oth"};
  for (int i = 1; i <= 3; i++) {
    h->GetYaxis()->SetBinLabel(i, labels[i - 1].c_str());
    h->GetXaxis()->SetBinLabel(i, labels[i - 1].c_str());
  }
  h->SetMarkerSize(3);
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

void LmvmDraw::DrawSrcBgPairsAll()
{
  int hi      = 1;
  TCanvas* c1 = fH.fHM.CreateCanvas("lmvm_srcBgPairs_abs", "lmvm_srcBgPairs_abs", 1500, 1500);
  c1->Divide(3, 3);
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc || SkipMvd(step)) continue;
    c1->cd(hi++);
    DrawSrcBgPairs(step, false);
  }

  hi          = 1;
  TCanvas* c2 = fH.fHM.CreateCanvas("lmvm_srcBgPairs_perc", "lmvm_srcBgPairs_perc", 1500, 1500);
  c2->Divide(3, 3);
  for (ELmvmAnaStep step : fH.fAnaSteps) {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc || SkipMvd(step)) continue;
    c1->cd(hi++);
    DrawSrcBgPairs(step, true);
  }

  string ptCutName = fH.fAnaStepNames[static_cast<int>(ELmvmAnaStep::PtCut)];
  fH.fHM.CreateCanvas("lmvm_srcBgPairs_abs_" + ptCutName, "lmvm_srcBgPairs_abs_" + ptCutName, 900, 900);
  DrawSrcBgPairs(ELmvmAnaStep::PtCut, false, false);

  fH.fHM.CreateCanvas("lmvm_srcBgPairs_perc_" + ptCutName, "lmvm_srcBgPairs_perc_" + ptCutName, 900, 900);
  DrawSrcBgPairs(ELmvmAnaStep::PtCut, true, false);

  DrawBgSource2D("lmvm_srcBgPairs_2d", "hSrcBgPairs", fH.fBgPairSrcLatex, 1000., "Pairs per event x10^{3}");
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

void LmvmDraw::Draw2DCut(const string& hist, double cutCrossX, double cutCrossY)
{
  TCanvas* c = fH.fHM.CreateCanvas(("lmvm_" + hist).c_str(), ("lmvm_" + hist).c_str(), 1000, 1500);
  c->Divide(2, 3);
  vector<TH1*> projX, projY;
  vector<string> latex;
  for (ELmvmSrc src : {ELmvmSrc::Signal, ELmvmSrc::Bg, ELmvmSrc::Pi0, ELmvmSrc::Gamma}) {
    int srcInt = static_cast<int>(src);
    c->cd(srcInt + 1);
    DrawH2(fH.H2(hist, src));
    double nofPerEvent = fH.H2(hist, src)->GetEntries() / (double) fNofEvents;
    DrawTextOnPad((Cbm::NumberToString(nofPerEvent, 2) + "/ev."), 0.1, 0.9, 0.5, 0.99);
    DrawTextOnPad(fH.fSrcLatex[srcInt], 0.6, 0.89, 0.7, 0.99);
    Draw2DCutTriangle(cutCrossX, cutCrossY);
    projX.push_back(fH.H2(hist, src)->ProjectionX());
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

  TCanvas* c = fH.fHM.CreateCanvas("lmvm_vertexGamma_mc", "lmvm_vertexGamma_mc", 1800, 600);
  c->Divide(3, 1);
  for (size_t i = 0; i < xyz.size(); i++) {
    c->cd(i + 1);
    DrawH2(xyz[i]);
    xyz[i]->SetMinimum(1e-3);
    gPad->SetLogz(true);
  }

  TCanvas* cZ = fH.fHM.CreateCanvas("lmvm_vertexGamma_z", "lmvm_vertexGamma_z", 1500, 750);
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

  TCanvas* cZoom = fH.fHM.CreateCanvas("lmvm_vertexGamma_mc_zoom", "lmvm_vertexGamma_mc_zoom", 1800, 600);
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

  fH.fHM.CreateCanvas("lmvm_vertexGamma_rz_mc", "lmvm_vertexGamma_rz_mc", 900, 900);
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
  TCanvas* c1 = fH.fHM.CreateCanvas("lmvm_minv_sbg_anaStep", "lmvm_minv_sbg_anaStep", 1200, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Signal), true);
  c1->cd(2);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Bg), true);

  TCanvas* c2 = fH.fHM.CreateCanvas("lmvm_minv_pi0_eta_gamma_anaStep", "lmvm_minv_pi0_eta_gamma_anaStep", 1800, 600);
  c2->Divide(3, 1);
  c2->cd(1);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Pi0), true);
  c2->cd(2);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Eta), true);
  c2->cd(3);
  DrawAnaStepH1(fH.GetName("hMinv", ELmvmSrc::Gamma), true);

  for (const string& name : {"sameEv", "mixedEv"}) {
    string cName = "lmvm_minv_combPairs_" + name;
    TCanvas* c3  = fH.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 600);
    c3->Divide(3, 1);
    c3->cd(1);
    DrawAnaStepH1("hMinvCombPM_" + name, true);
    c3->cd(2);
    DrawAnaStepH1("hMinvCombPP_" + name, true);
    c3->cd(3);
    DrawAnaStepH1("hMinvCombMM_" + name, true);
  }
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
    stringstream ss;
    ss << "hMinvBgSource_" << fH.fBgPairSrcNames[i] << "_" << fH.fAnaStepNames[static_cast<int>(step)];
    hists.push_back(fH.H1(ss.str()));
    hists[i]->SetMinimum(1e-8);
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

void LmvmDraw::DrawPiMom()
{
  vector<string> subNames {"mc", "acc", "rec", "recOnlySts", "recStsRichTrd", "recStsRichTrdTof"};
  vector<string> latex {"MC", "Acc", "Rec", "Rec only STS", "Rec STS-RICH-TRD", "Rec STS-RICH-TRD-TOF"};
  vector<string> latexAll(latex.size()), latexPrim(latex.size());
  vector<TH1*> histsAll, histsPrim;

  int i = 0;
  for (const string& subName : subNames) {
    TH1D* hAll  = fH.H1("hPiMom_all_" + subName);
    latexAll[i] = latex[i] + " (" + Cbm::NumberToString(hAll->GetEntries() / fNofEvents, 2) + "/ev.)";
    histsAll.push_back(hAll);

    TH1D* hPrim  = fH.H1("hPiMom_prim_" + subName);
    latexPrim[i] = latex[i] + " (" + Cbm::NumberToString(hPrim->GetEntries() / fNofEvents, 2) + "/ev.)";
    histsPrim.push_back(hPrim);
    i++;
  }

  fH.fHM.CreateCanvas("lmvm_piMom", "lmvm_piMom", 900, 900);
  DrawH1(histsAll, latexAll, kLinear, kLog, true, 0.45, 0.75, 0.99, 0.99, "hist");

  fH.fHM.CreateCanvas("lmvm_piMomPrim", "lmvm_piMomPrim", 900, 900);
  DrawH1(histsPrim, latexPrim, kLinear, kLog, true, 0.45, 0.75, 0.99, 0.99, "hist");
}

void LmvmDraw::DrawBgSource2D(const string& cName, const string& hName, const vector<string>& yLabels, double scale,
                              const string& zTitle)
{
  fH.fHM.CreateCanvas((cName + "_abs").c_str(), (cName + "_abs").c_str(), 900, 600);
  TH2D* habs = fH.H2Clone(hName);
  habs->SetStats(false);
  habs->Scale(scale);
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
  hperc->SetMarkerSize(1.8);
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

  fH.fHM.CreateCanvas("lmvm_nofBgTracks", "lmvm_nofBgTracks", 900, 900);
  TH1D* hbg = fH.H1Clone("hNofBgTracks");
  hbg->Scale(10);
  hbg->GetYaxis()->SetTitle("Tracks/event x10");
  DrawH1(hbg, kLinear, kLog, "hist text0");
  hbg->SetMarkerSize(2.);

  fH.fHM.CreateCanvas("lmvm_nofSignalTracks", "lmvm_nofSignalTracks", 900, 900);
  TH1D* hel = fH.H1("hNofSignalTracks");
  DrawH1(hel, kLinear, kLog, "hist");

  fH.fHM.CreateCanvas("lmvm_purity", "lmvm_purity", 900, 900);
  TH1D* purity = new TH1D("purity", "Purity;Analysis steps;Purity", fH.fNofAnaSteps, 0., fH.fNofAnaSteps);
  purity->Divide(fH.H1("hNofBgTracks"), fH.H1("hNofSignalTracks"));
  DrawH1(purity, kLinear, kLog, "hist text30");
  purity->SetMarkerSize(1.9);

  SetAnalysisStepAxis(hbg);
  SetAnalysisStepAxis(hel);
  SetAnalysisStepAxis(purity);

  DrawBgSource2D("lmvm_bgSrcTracks_2d", "hBgSrcTracks",
                 {"#gamma", "#pi^{0}", "#pi^{#pm}", "p", "K", "e^{#pm}_{sec}", "oth."}, 100.,
                 "Tracks per event x10^{2}");

  TCanvas* c = fH.fHM.CreateCanvas("lmvm_nofTopoPairs", "lmvm_nofTopoPairs", 1600, 800);
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
  TCanvas* c1 = fH.fHM.CreateCanvas("lmvm_nofMismatches", "lmvm_nofMismatches", 1500, 1500);
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

  fH.fHM.CreateCanvas("lmvm_nofGhosts", "lmvm_nofGhosts", 900, 900);
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
  TCanvas* c = fH.fHM.CreateCanvas("lmvm_mvdCutQa", "lmvm_mvd1cut_qa", 1600, 800);
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
  TCanvas* c1 = fH.fHM.CreateCanvas("lmvm_nofHitsMvdSts", "lmvm_nofHitsMvdSts", 1600, 800);
  c1->Divide(2, 1);
  c1->cd(1);
  DrawSrcH1("hNofMvdHits");
  c1->cd(2);
  DrawSrcH1("hNofStsHits");

  Draw2DCut("hMvdXY_1");
  fH.fHM.CreateCanvas("lmvm_mvd1", "lmvm_mvd1", 900, 900);
  DrawSrcH1("hMvdR_1");

  Draw2DCut("hMvdXY_2");
  fH.fHM.CreateCanvas("lmvm_mvd2", "lmvm_mvd2", 900, 900);
  DrawSrcH1("hMvdR_2");
}

void LmvmDraw::SaveCanvasToImage()
{
  fH.fHM.SaveCanvasToImage(fOutputDir, "png");  // fHM->SaveCanvasToImage(fOutputDir, "png;eps");
}
