/* Copyright (C) 2008-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmDrawHist.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 **/
#include "CbmDrawHist.h"

#include "CbmUtils.h"  // for NumberToString

#include <TAxis.h>        // for TAxis
#include <TF1.h>          // for TF1
#include <TGraph.h>       // for TGraph
#include <TGraph2D.h>     // for TGraph2D
#include <TH1.h>          // for TH1, TH1D
#include <TH2.h>          // for TH2, TH2D
#include <TH3.h>          // for TH3
#include <TLatex.h>       // for TLatex
#include <TLegend.h>      // for TLegend
#include <TStyle.h>       // for TStyle, gStyle
#include <TVirtualPad.h>  // for TVirtualPad, gPad

#include <algorithm>  // for max, min
#include <cassert>    // for assert
#include <iostream>   // for string, operator<<, basic_ostream, strin...
#include <limits>     // for numeric_limits
#include <string>     // for operator+, allocator, basic_string, char...

using std::string;
using std::stringstream;
using std::vector;

/* Set default styles for histograms. */
void SetDefaultDrawStyle()
{
  gStyle->SetOptStat("rm");
  gStyle->SetOptFit(1);
  gStyle->SetOptTitle(0);

  gStyle->SetCanvasColor(kWhite);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetStatColor(kWhite);
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetPalette(55, 0);
  // gStyle->SetPalette(1);
}

/* Draw TH1 histogram.*/
void DrawH1(TH1* hist, HistScale logx, HistScale logy, const string& drawOpt, Int_t color, Int_t lineWidth,
            Int_t lineStyle, Int_t markerSize, Int_t markerStyle)
{
  Double_t textSize = CbmDrawingOptions::TextSize();
  hist->SetLineColor(color);
  hist->SetLineWidth(lineWidth);
  hist->SetLineStyle(lineStyle);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize(markerSize);
  hist->SetMarkerStyle(markerStyle);
  if (logx == kLog) { gPad->SetLogx(); }
  if (logy == kLog) { gPad->SetLogy(); }
  hist->GetXaxis()->SetLabelSize(textSize);
  hist->GetXaxis()->SetNdivisions(505, kTRUE);
  hist->GetYaxis()->SetLabelSize(textSize);
  hist->GetXaxis()->SetTitleSize(textSize);
  hist->GetYaxis()->SetTitleSize(textSize);
  hist->GetXaxis()->SetTitleOffset(1.0);
  hist->GetYaxis()->SetTitleOffset(1.3);
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.15);
  gPad->SetTopMargin(0.12);
  gPad->SetTicks(1, 1);
  hist->Draw(drawOpt.c_str());
  gPad->SetGrid(true, true);
  hist->SetStats(false);
}

/* Draw TH2 histogram.*/
void DrawH2(TH2* hist, HistScale logx, HistScale logy, HistScale logz, const string& drawOpt)
{
  Double_t textSize = CbmDrawingOptions::TextSize();
  if (logx == kLog) { gPad->SetLogx(); }
  if (logy == kLog) { gPad->SetLogy(); }
  if (logz == kLog) { gPad->SetLogz(); }
  hist->GetXaxis()->SetLabelSize(textSize);
  hist->GetXaxis()->SetNdivisions(505, kTRUE);
  hist->GetYaxis()->SetLabelSize(textSize);
  hist->GetYaxis()->SetNdivisions(505, kTRUE);
  hist->GetZaxis()->SetLabelSize(textSize);
  // hist->GetZaxis()->SetNdivisions(505, kTRUE);
  hist->GetXaxis()->SetTitleSize(textSize);
  hist->GetYaxis()->SetTitleSize(textSize);
  hist->GetZaxis()->SetTitleSize(textSize);
  hist->GetXaxis()->SetTitleOffset(1.0);
  hist->GetYaxis()->SetTitleOffset(1.3);
  hist->GetZaxis()->SetTitleOffset(1.5);
  gPad->SetLeftMargin(0.17);
  gPad->SetRightMargin(0.30);
  gPad->SetBottomMargin(0.15);
  gPad->SetTicks(1, 1);
  hist->Draw(drawOpt.c_str());
  gPad->SetGrid(true, true);
  hist->SetStats(false);
}

/* Draw several TH1 histograms. */
void DrawH1(const vector<TH1*>& histos, const vector<string>& histLabels, HistScale logx, HistScale logy,
            Bool_t drawLegend, Double_t x1, Double_t y1, Double_t x2, Double_t y2, const string& drawOpt)
{
  assert(histos.size() != 0 && histLabels.size() == histos.size());
  Double_t max     = std::numeric_limits<Double_t>::min();
  UInt_t nofHistos = histos.size();
  TLegend* legend  = new TLegend(x1, y1, x2, y2);
  legend->SetFillColor(kWhite);
  for (UInt_t iHist = 0; iHist < nofHistos; iHist++) {
    TH1* hist  = histos[iHist];
    string opt = (iHist == 0) ? drawOpt : (iHist == nofHistos - 1) ? "SAME" + drawOpt : "SAME" + drawOpt;
    DrawH1(hist, logx, logy, opt, CbmDrawingOptions::Color(iHist), CbmDrawingOptions::LineWidth(),
           CbmDrawingOptions::LineStyle(0), CbmDrawingOptions::MarkerSize(), CbmDrawingOptions::MarkerStyle(iHist));
    max = std::max(max, hist->GetMaximum());
    legend->AddEntry(hist, histLabels[iHist].c_str(), "lp");
  }
  histos[0]->SetMaximum(max * 1.17);
  if (drawLegend) { legend->Draw(); }
}

void DrawGraph(TGraph* graph, HistScale logx, HistScale logy, const string& drawOpt, Int_t color, Int_t lineWidth,
               Int_t lineStyle, Int_t markerSize, Int_t markerStyle)
{
  Double_t textSize = CbmDrawingOptions::TextSize();
  graph->SetLineColor(color);
  graph->SetLineWidth(lineWidth);
  graph->SetLineStyle(lineStyle);
  graph->SetMarkerColor(color);
  graph->SetMarkerSize(markerSize);
  graph->SetMarkerStyle(markerStyle);
  if (drawOpt.find("A") != string::npos) {
    if (logx == kLog) { gPad->SetLogx(); }
    if (logy == kLog) { gPad->SetLogy(); }
    graph->GetXaxis()->SetLabelSize(textSize);
    graph->GetXaxis()->SetNdivisions(505, kTRUE);
    graph->GetYaxis()->SetLabelSize(textSize);
    graph->GetXaxis()->SetTitleSize(textSize);
    graph->GetYaxis()->SetTitleSize(textSize);
    graph->GetXaxis()->SetTitleOffset(1.0);
    graph->GetYaxis()->SetTitleOffset(1.3);
  }
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.15);
  graph->Draw(drawOpt.c_str());
  gPad->SetGrid(true, true);
}


/* Draw several TGraphs. */
void DrawGraph(const vector<TGraph*>& graphs, const vector<string>& graphLabels, HistScale logx, HistScale logy,
               Bool_t drawLegend, Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
  assert(graphs.size() != 0 && graphs.size() == graphLabels.size());

  Double_t max    = std::numeric_limits<Double_t>::min();
  Double_t min    = std::numeric_limits<Double_t>::max();
  TLegend* legend = new TLegend(x1, y1, x2, y2);
  legend->SetFillColor(kWhite);
  UInt_t nofGraphs = graphs.size();
  for (UInt_t iGraph = 0; iGraph < nofGraphs; iGraph++) {
    TGraph* graph = graphs[iGraph];
    string opt    = (iGraph == 0) ? "ACP" : "CP";
    DrawGraph(graph, logx, logy, opt, CbmDrawingOptions::Color(iGraph), CbmDrawingOptions::LineWidth(),
              CbmDrawingOptions::LineStyle(iGraph), CbmDrawingOptions::MarkerSize(),
              CbmDrawingOptions::MarkerStyle(iGraph));
    max = std::max(graph->GetYaxis()->GetXmax(), max);
    min = std::min(graph->GetYaxis()->GetXmin(), min);
    legend->AddEntry(graph, graphLabels[iGraph].c_str(), "lp");
  }
  graphs[0]->SetMaximum(max);
  graphs[0]->SetMinimum(min);
  if (drawLegend) { legend->Draw(); }
}

/* Draws 2D graph.*/
void DrawGraph2D(TGraph2D* graph, HistScale logx, HistScale logy, HistScale logz, const string& drawOpt)
{
  Double_t textSize = CbmDrawingOptions::TextSize();
  if (logx == kLog) { gPad->SetLogx(); }
  if (logy == kLog) { gPad->SetLogy(); }
  if (logz == kLog) { gPad->SetLogz(); }
  graph->GetXaxis()->SetLabelSize(textSize);
  graph->GetXaxis()->SetNdivisions(505, kTRUE);
  graph->GetYaxis()->SetLabelSize(textSize);
  graph->GetYaxis()->SetNdivisions(505, kTRUE);
  graph->GetZaxis()->SetLabelSize(textSize);
  //   graph->GetZaxis()->SetNdivisions(505, kTRUE);
  graph->GetXaxis()->SetTitleSize(textSize);
  graph->GetYaxis()->SetTitleSize(textSize);
  graph->GetZaxis()->SetTitleSize(textSize);
  graph->GetXaxis()->SetTitleOffset(1.0);
  graph->GetYaxis()->SetTitleOffset(1.3);
  graph->GetZaxis()->SetTitleOffset(1.7);
  gPad->SetLeftMargin(0.17);
  gPad->SetRightMargin(0.2);
  gPad->SetBottomMargin(0.15);
  gPad->SetTicks(1, 1);
  graph->Draw(drawOpt.c_str());
  gPad->SetGrid(true, true);
}

void DrawTextOnPad(const string& text, Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
  TLegend* leg = new TLegend(x1, y1, x2, y2);
  leg->AddEntry(new TH2D(), text.c_str(), "");
  leg->SetFillColor(kWhite);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();
}

void DrawH1andFitGauss(TH1* hist, Bool_t drawResults, Bool_t doScale, Double_t userRangeMin, Double_t userRangeMax)
{
  if (hist == nullptr) return;

  hist->GetYaxis()->SetTitle("Yield (a.u.)");
  DrawH1(hist, kLinear, kLinear, "hist");
  if (doScale) hist->Scale(1. / hist->Integral());
  if (!(userRangeMin == 0. && userRangeMax == 0.)) hist->GetXaxis()->SetRangeUser(userRangeMin, userRangeMax);
  hist->Fit("gaus", "Q");
  TF1* func = hist->GetFunction("gaus");
  if (func == nullptr) return;
  func->SetLineColor(kBlack);
  func->Draw("same");

  if (drawResults) {
    double m    = func->GetParameter(1);
    double s    = func->GetParameter(2);
    string txt1 = Cbm::NumberToString<Double_t>(m, 2) + " / " + Cbm::NumberToString<Double_t>(s, 2);
    TLatex text;
    text.SetTextAlign(21);
    text.SetTextSize(0.06);
    text.DrawTextNDC(0.5, 0.92, txt1.c_str());
  }
}

void DrawH2WithProfile(TH2* hist, Bool_t doGaussFit, Bool_t drawOnlyMean, const string& drawOpt2D, Int_t profileColor,
                       Int_t profileLineWidth)
{
  if (hist == nullptr) return;

  // TProfile does not allow to fit individual slice with gauss
  /*  TProfile* profX = (TProfile*)hist->ProfileX("_pfx", 1, -1, "s")->Clone();
    DrawH2(hist, kLinear, kLinear, kLinear, drawOpt);
    if (!drawOnlyMean) {
        DrawH1(profX, kLinear, kLinear, "same", profileColor, profileLineWidth, 1, 1, kOpenCircle);
    } else {
        DrawH1(profX, kLinear, kLinear, "same hist p", profileColor, profileLineWidth, 1, 1, kFullCircle);
    }*/

  TH1D* hMean   = (TH1D*) hist->ProjectionX((string(hist->GetName()) + "_mean").c_str())->Clone();
  string yTitle = (doGaussFit) ? "Mean and sigma. " : "Mean and RMS. ";
  hMean->GetYaxis()->SetTitle((yTitle + string(hist->GetYaxis()->GetTitle())).c_str());
  for (Int_t i = 1; i <= hist->GetXaxis()->GetNbins(); i++) {
    stringstream ss;
    ss << string(hist->GetName()) << "_py" << i;
    TH1D* pr = hist->ProjectionY(ss.str().c_str(), i, i);
    if (hMean == nullptr || pr == nullptr) continue;

    Double_t m = 0., s = 0.;
    if (doGaussFit) {
      pr->Fit("gaus", "QO");
      TF1* func = pr->GetFunction("gaus");
      if (func != nullptr) {
        m = func->GetParameter(1);
        s = func->GetParameter(2);
      }
    }
    else {
      m = pr->GetMean();
      s = pr->GetRMS();
    }

    hMean->SetBinContent(i, m);
    if (!drawOnlyMean) { hMean->SetBinError(i, s); }
    else {
      hMean->SetBinError(i, 0.);
    }
  }
  DrawH2(hist, kLinear, kLinear, kLinear, drawOpt2D);
  if (!drawOnlyMean) { DrawH1(hMean, kLinear, kLinear, "same", profileColor, profileLineWidth, 1, 1, kOpenCircle); }
  else {
    DrawH1(hMean, kLinear, kLinear, "same hist p", profileColor, profileLineWidth, 1, 1, kFullCircle);
  }
}

TH2D* DrawH3Profile(TH3* h, Bool_t drawMean, Bool_t doGaussFit, Double_t zMin, Double_t zMax)
{
  Int_t nBinsX = h->GetNbinsX();
  Int_t nBinsY = h->GetNbinsY();
  TH2D* h2     = (TH2D*) h->Project3D("yx")->Clone();

  for (Int_t x = 1; x <= nBinsX; x++) {
    for (Int_t y = 1; y <= nBinsY; y++) {
      stringstream ss;
      ss << h->GetName() << "_z_" << x << "_" << y;
      TH1D* hz    = h->ProjectionZ(ss.str().c_str(), x, x, y, y);
      Double_t ms = 0.;
      if (doGaussFit) {
        hz->Fit("gaus", "QO");
        TF1* func = hz->GetFunction("gaus");
        if (func != nullptr) { ms = (drawMean) ? func->GetParameter(1) : func->GetParameter(2); }
      }
      else {
        ms = (drawMean) ? hz->GetMean() : hz->GetRMS();
      }
      h2->SetBinContent(x, y, ms);
    }
  }

  string zAxisTitle = string(h->GetZaxis()->GetTitle());
  string sigmaRms   = (doGaussFit) ? "Sigma." : "RMS.";
  zAxisTitle        = (drawMean) ? "Mean." + zAxisTitle : sigmaRms + zAxisTitle;

  h2->GetZaxis()->SetTitle(zAxisTitle.c_str());
  if (zMin < zMax) h2->GetZaxis()->SetRangeUser(zMin, zMax);
  DrawH2(h2);

  return h2;
}
