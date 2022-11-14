/* Copyright (C) 2012-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Semen Lebedev [committer] */

#include "LmvmHist.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "utils/CbmUtils.h"

#include "Logger.h"

#include "TLegend.h"
#include "TStyle.h"
#include "TText.h"

#include "LmvmUtils.h"


using std::string;
using std::vector;

const vector<ELmvmSrc> LmvmHist::fSrcs        = {ELmvmSrc::Signal, ELmvmSrc::Bg, ELmvmSrc::Pi0, ELmvmSrc::Gamma,
                                          ELmvmSrc::Eta};
const vector<std::string> LmvmHist::fSrcNames = {"signal", "bg", "pi0", "gamma", "eta"};
const vector<std::string> LmvmHist::fSrcLatex = {"S", "BG_{oth}", "#pi^{0}", "#gamma", "#eta"};
const vector<int> LmvmHist::fSrcColors        = {kRed, kBlue, kGreen, kOrange, kMagenta};

const vector<ELmvmAnaStep> LmvmHist::fAnaSteps    = {ELmvmAnaStep::Mc,       ELmvmAnaStep::Acc,     ELmvmAnaStep::Reco,
                                                  ELmvmAnaStep::Chi2Prim, ELmvmAnaStep::ElId,    ELmvmAnaStep::GammaCut,
                                                  ELmvmAnaStep::Mvd1Cut,  ELmvmAnaStep::Mvd2Cut, ELmvmAnaStep::StCut,
                                                  ELmvmAnaStep::RtCut,    ELmvmAnaStep::TtCut,   ELmvmAnaStep::PtCut};
const vector<std::string> LmvmHist::fAnaStepNames = {"mc",      "acc",     "reco",  "chi2prim", "elid",  "gammacut",
                                                     "mvd1cut", "mvd2cut", "stcut", "rtcut",    "ttcut", "ptcut"};
const vector<std::string> LmvmHist::fAnaStepLatex = {
  "MC", "ACC", "REC", "#chi^{2}_{prim}", "ID", "m_{#gamma}", "mvd1", "mvd2", "ST", "RT", "TT", "P_{t}"};
const vector<int> LmvmHist::fAnaStepColors = {kGreen + 3,   kOrange + 3, kBlack,   kOrange + 7,
                                              kRed,         kPink - 6,   kGreen,   kOrange - 3,
                                              kViolet + 10, kGreen - 3,  kMagenta, kYellow + 1};

const vector<std::string> LmvmHist::fSignalNames = {"inmed", "qgp", "omega", "phi", "omegaD"};
const vector<ELmvmSignal> LmvmHist::fSignals     = {ELmvmSignal::Inmed, ELmvmSignal::Qgp, ELmvmSignal::Omega,
                                                ELmvmSignal::Phi, ELmvmSignal::OmegaD};

const vector<std::string> LmvmHist::fBgPairSrcNames = {"GG", "PP", "OO", "GP", "GO", "PO"};
const vector<std::string> LmvmHist::fBgPairSrcLatex = {"#gamma-#gamma",  "#pi^{0}-#pi^{0}", "o.-o.",
                                                       "#gamma-#pi^{0}", "#gamma-o.",       "#pi^{0}-o."};

// the following candidates and global track vectors are mainly set up to investigate misidentifications
const vector<std::string> LmvmHist::fGTrackNames = {
  "el+",    "el+_prim",    "el-",   "el-_prim",   "pion+", "pion+_prim", "pion-", "pion-_prim",
  "proton", "proton_prim", "kaon+", "kaon+_prim", "kaon-", "kaon-_prim", "other"};
const vector<std::string> LmvmHist::fGTrackLatex = {
  "e^{+}", "e^{+}_{prim}", "e^{-}", "e^{-}_{prim}", "#pi^{+}", "#pi^{+}_{prim}", "#pi^{-}", "#pi^{-}_{prim}",
  "p",     "p_{prim}",     "K^{+}", "K^{+}_{prim}", "K^{-}",   "K^{-}_{prim}",   "other"};

const vector<std::string> LmvmHist::fCandNames = {"plutoEl+", "plutoEl-", "urqmdEl+", "urqmdEl-", "pion+",
                                                  "pion-",    "proton",   "kaon+",    "kaon-",    "other"};
const vector<std::string> LmvmHist::fCandLatex = {"e^{+}_{PLUTO}",
                                                  "e^{-}_{PLUTO}",
                                                  "e^{+}_{UrQMD}",
                                                  "e^{-}_{UrQMD}",
                                                  "#pi^{+}",
                                                  "#pi^{-}",
                                                  "p",
                                                  "K^{+}",
                                                  "K^{-}",
                                                  "o."};


LmvmHist::LmvmHist() {}

vector<string> LmvmHist::CombineNames(const string& name, const vector<string>& subNames)
{
  vector<string> result;
  for (const auto& subName : subNames) {
    result.push_back(name + "_" + subName);
  }
  return result;
}

vector<string> LmvmHist::CombineNames(const string& name, const vector<string>& subNames1,
                                      const vector<string>& subNames2)
{
  vector<string> result;
  for (const auto& subName1 : subNames1) {
    for (const auto& subName2 : subNames2) {
      result.push_back(name + "_" + subName1 + "_" + subName2);
    }
  }
  return result;
}

void LmvmHist::CreateH1(const string& name, const string& axisX, const string& axisY, double nBins, double min,
                        double max)
{
  string title = name + ";" + axisX + ";" + axisY;
  fHM.Create1<TH1D>(name, title, nBins, min, max);
}

void LmvmHist::CreateH2(const string& name, const string& axisX, const string& axisY, const string& axisZ,
                        double nBinsX, double minX, double maxX, double nBinsY, double minY, double maxY)
{
  string title = name + ";" + axisX + ";" + axisY + ";" + axisZ;
  fHM.Create2<TH2D>(name, title, nBinsX, minX, maxX, nBinsY, minY, maxY);
}


void LmvmHist::CreateH1(const string& name, const vector<string>& subNames, const string& axisX, const string& axisY,
                        double nBins, double min, double max)
{
  vector<string> names = CombineNames(name, subNames);
  for (const auto& curName : names) {
    string title = curName + ";" + axisX + ";" + axisY;
    fHM.Create1<TH1D>(curName, title, nBins, min, max);
  }
}

void LmvmHist::CreateH2(const string& name, const vector<string>& subNames, const string& axisX, const string& axisY,
                        const string& axisZ, double nBinsX, double minX, double maxX, double nBinsY, double minY,
                        double maxY)
{
  vector<string> names = CombineNames(name, subNames);
  for (const auto& curName : names) {
    string title = curName + ";" + axisX + ";" + axisY + ";" + axisZ;
    fHM.Create2<TH2D>(curName, title, nBinsX, minX, maxX, nBinsY, minY, maxY);
  }
}

void LmvmHist::CreateH1(const string& name, const vector<string>& subNames1, const vector<string>& subNames2,
                        const string& axisX, const string& axisY, double nBins, double min, double max)
{
  vector<string> names = CombineNames(name, subNames1, subNames2);
  for (const auto& curName : names) {
    string title = curName + ";" + axisX + ";" + axisY;
    fHM.Create1<TH1D>(curName, title, nBins, min, max);
  }
}

void LmvmHist::CreateH2(const string& name, const vector<string>& subNames1, const vector<string>& subNames2,
                        const string& axisX, const string& axisY, const string& axisZ, double nBinsX, double minX,
                        double maxX, double nBinsY, double minY, double maxY)
{
  vector<string> names = CombineNames(name, subNames1, subNames2);
  for (const auto& curName : names) {
    string title = curName + ";" + axisX + ";" + axisY + ";" + axisZ;
    fHM.Create2<TH2D>(curName, title, nBinsX, minX, maxX, nBinsY, minY, maxY);
  }
}

void LmvmHist::FillH1(const string& name, double x, double w) { H1(name)->Fill(x, w); }

void LmvmHist::FillH2(const string& name, double x, double y, double w) { H2(name)->Fill(x, y, w); }

void LmvmHist::FillH1(const string& name, ELmvmSrc src, double x, double wSignal)
{
  if (src == ELmvmSrc::Undefined) return;
  //double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.; // TODO: delete commented lines
  //H1(name, src)->Fill(x, myWeight);
  H1(name, src)->Fill(x, wSignal);
}

void LmvmHist::FillH2(const string& name, ELmvmSrc src, double x, double y, double wSignal)
{
  if (src == ELmvmSrc::Undefined) return;
  //double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.; // TODO: delete commented lines
  //H2(name, src)->Fill(x, y, myWeight);
  H2(name, src)->Fill(x, y, wSignal);
}

void LmvmHist::FillH1(const string& name, ELmvmAnaStep step, double x, double w) { H1(name, step)->Fill(x, w); }

void LmvmHist::FillH2(const string& name, ELmvmAnaStep step, double x, double y, double w)
{
  H2(name, step)->Fill(x, y, w);
}

void LmvmHist::FillH1(const string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double wSignal)
{
  if (src == ELmvmSrc::Undefined || step == ELmvmAnaStep::Undefined) return;
  //double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.; // TODO: delete commented lines
  //FillH1(GetName(name, src, step), x, myWeight);
  FillH1(GetName(name, src, step), x, wSignal);
}

void LmvmHist::FillH2(const string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double y, double wSignal)
{
  if (src == ELmvmSrc::Undefined || step == ELmvmAnaStep::Undefined) return;
  //double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.; // TODO: delete commented lines
  //FillH2(GetName(name, src, step), x, y, myWeight);
  FillH2(GetName(name, src, step), x, y, wSignal);
}


string LmvmHist::GetName(const string& name, ELmvmAnaStep step)
{
  if (step == ELmvmAnaStep::Undefined) {
    LOG(error) << "LmvmHist::GetName step == ELmvmAnaStep::Undefined";
    return name;
  }
  return name + "_" + fAnaStepNames[static_cast<int>(step)];
}

string LmvmHist::GetName(const string& name, ELmvmSrc src)
{
  if (src == ELmvmSrc::Undefined) {
    LOG(error) << "LmvmHist::GetName src == ELmvmSrc::Undefined";
    return name;
  }
  return name + "_" + fSrcNames[static_cast<int>(src)];
}

string LmvmHist::GetName(const string& name, ELmvmSrc src, ELmvmAnaStep step)
{
  return GetName(GetName(name, src), step);
}

void LmvmHist::SetOptH1(TH1D* hist, TString xAxisTitle = " ", TString yAxisTitle = " ", Int_t Ndevision = 510,
                        Int_t style = 1, Float_t size = 2, Int_t color = 1, string opt)
{
  hist->GetXaxis()->SetTitle(xAxisTitle);
  hist->GetYaxis()->SetTitle(yAxisTitle);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetNdivisions(Ndevision);
  hist->GetYaxis()->SetTitleOffset(1.4);
  hist->GetXaxis()->SetTitleOffset(1.);
  //hist->GetXaxis()->CenterTitle();
  //hist->GetYaxis()->CenterTitle();
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.04);
  hist->GetYaxis()->SetLabelSize(0.04);
  if (opt == "marker") {
    hist->SetMarkerStyle(style);
    hist->SetMarkerSize(size);
    hist->SetMarkerColor(color);
    hist->SetLineColor(color);
    hist->SetLineWidth(2);
    hist->SetTitle("");
    //hist->SetLineWidth(2);
    //hist->SetMinimum(0.1);
    //hist->SetMaximum(1e5);
    hist->GetXaxis()->SetLabelColor(1);
    hist->GetYaxis()->SetLabelColor(1);
    hist->GetXaxis()->SetTitleColor(1);
    hist->GetYaxis()->SetTitleColor(1);
    hist->SetFillColor(1);
  }
  else if (opt == "line") {
    hist->SetLineStyle(style);
    hist->SetLineColor(color);
    hist->SetTitle("");  // TODO: with ""?
    hist->SetLineWidth(size);
  }
  else
    LOG(error) << "Option '" << opt << "' undefined. Choose 'marker' or 'line'." << std::endl;
}

void LmvmHist::SetOptCanvas(TCanvas* canvas)
{
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);
  //gStyle->SetErrorX(0);    // X errors of the data points set to be 0
  gStyle->SetLineStyleString(22, "80 18 12 18 12 12");  // special style for the line
  gStyle->SetEndErrorSize(5);                           // define end width of error bars
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(10);
  canvas->SetLeftMargin(0.15);
  canvas->SetRightMargin(0.04);
  canvas->SetTopMargin(0.05);
  canvas->SetBottomMargin(0.12);
  canvas->ToggleEventStatus();
  canvas->Range(-200, -10, 1000, -2);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetBorderSize(0);
  canvas->SetTickx();
  canvas->SetTicky();
  canvas->SetLogy();
  canvas->SetFrameLineWidth(2);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(0);
}

void LmvmHist::SetLegend(vector<LmvmLegend> legendV, double textsize, Double_t x1, Double_t y1, Double_t x2,
                         Double_t y2)
{
  auto leg = new TLegend(x1, y1, x2, y2);
  leg->SetLineColor(10);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(10);
  leg->SetTextSize(textsize);
  leg->SetTextFont(42);
  leg->SetMargin(0.4);
  leg->SetBorderSize(0);
  for (size_t i = 0; i < legendV.size(); i++) {
    const auto& l = legendV[i];
    leg->AddEntry(l.fH, l.fName, l.fOpt);
  }
  leg->Draw("same");
}

void LmvmHist::Rebin(const string& name, int nGroup) { fHM.Rebin(name, nGroup); }

void LmvmHist::Rebin(const string& name, const vector<string>& subNames, int nGroup)
{
  vector<string> names = CombineNames(name, subNames);
  for (const auto& curName : names) {
    fHM.Rebin(curName, nGroup);
  }
}

void LmvmHist::Rebin(const string& name, const vector<string>& subNames1, const vector<string>& subNames2, int nGroup)
{
  vector<string> names = CombineNames(name, subNames1, subNames2);
  for (const auto& curName : names) {
    fHM.Rebin(curName, nGroup);
  }
}

TH1D* LmvmHist::CreateSignificanceH1(TH1D* s, TH1D* bg, const string& name, const string& option)
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
      hsig->GetYaxis()->SetTitle("Significance");
    }
  }
  // "left" - reject left part of the histogram. value < cut -> reject
  else if (option == "left") {
    for (int i = nBins; i >= 1; i--) {
      double sumSignal = s->Integral(i, nBins, "width");
      double sumBg     = bg->Integral(i, nBins, "width");
      double sign      = (sumSignal + sumBg != 0.) ? sumSignal / std::sqrt(sumSignal + sumBg) : 0.;
      hsig->SetBinContent(i, sign);
      hsig->GetYaxis()->SetTitle("Significance");
    }
  }
  return hsig;
}

TH2D* LmvmHist::CreateSignificanceH2(TH2D* signal, TH2D* bg, const string& name, const string& title)
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

void LmvmHist::DrawAllGTracks(int dim, const string& cName, const string& hName, vector<string> xLabel,
                              vector<string> yLabel, double min, double max)
{
  TCanvas* c = fHM.CreateCanvas(cName, cName, 2400, 2400);
  c->Divide(4, 4);
  int i = 1;
  for (const string& ptcl : fGTrackNames) {
    c->cd(i++);
    string hFullname = hName + "_" + ptcl;
    DrawAll(dim, hFullname, fGTrackLatex[i - 2], xLabel, yLabel, min, max);
  }
}

void LmvmHist::DrawAllCands(int dim, const string& cName, const string& hName, vector<string> xLabel,
                            vector<string> yLabel, double min, double max)
{
  TCanvas* c = fHM.CreateCanvas(cName, cName, 2400, 1800);
  c->Divide(4, 3);
  int i = 1;
  for (const string& ptcl : fCandNames) {
    c->cd(i++);
    string hFullname = hName + "_" + ptcl;
    DrawAll(dim, hFullname, fCandLatex[i - 2], xLabel, yLabel, min, max);
  }
}

void LmvmHist::DrawAllCandsAndSteps(int dim, const string& cName, const string& hName, vector<string> xLabel,
                                    vector<string> yLabel, double min, double max)
{
  for (auto step : fAnaSteps) {
    TCanvas* c = fHM.CreateCanvas(GetName(cName + "cands_all", step), GetName(cName + "cands_all", step), 2400, 1800);
    c->Divide(4, 3);
    int i = 1;
    for (const string& ptcl : fCandNames) {
      c->cd(i++);
      string hFullname = GetName(hName + "_" + ptcl, step);
      DrawAll(dim, hFullname.c_str(), fCandLatex[i - 2], xLabel, yLabel, min, max);
    }
  }

  for (const string& ptcl : fCandNames) {
    TCanvas* c = fHM.CreateCanvas(cName + ptcl + "_steps", cName + ptcl + "_steps", 2400, 1800);
    c->Divide(4, 3);
    int i = 1;
    for (auto step : fAnaSteps) {
      c->cd(i++);
      string hFullname = GetName(hName + "_" + ptcl, step);
      DrawAll(dim, hFullname.c_str(), fAnaStepNames[static_cast<int>(step)], xLabel, yLabel, min, max);
    }
  }
}

void LmvmHist::DrawAll(int dim, const string& hFullname, const string& text, vector<string> xLabel,
                       vector<string> yLabel, double min, double max)
{
  if (dim == 1) {
    TH1D* h = H1Clone(hFullname.c_str());
    h->GetYaxis()->SetRangeUser(min, max);
    DrawH1(h, kLinear, kLog, "hist");
    if (xLabel.size() > 1) {
      for (size_t iL = 0; iL < xLabel.size(); iL++) {
        h->GetXaxis()->SetBinLabel(iL + 1, xLabel[iL].c_str());
      }
    }
  }
  else if (dim == 2) {
    TH2D* h = H2Clone(hFullname.c_str());
    h->GetZaxis()->SetRangeUser(min, max);
    DrawH2(h, kLinear, kLinear, kLog, "colz");
    if (xLabel.size() > 1) {
      for (size_t iL = 0; iL < xLabel.size(); iL++) {
        h->GetXaxis()->SetBinLabel(iL + 1, xLabel[iL].c_str());
      }
    }
    if (yLabel.size() > 1) {
      for (size_t iL = 0; iL < yLabel.size(); iL++) {
        h->GetYaxis()->SetBinLabel(iL + 1, yLabel[iL].c_str());
      }
    }
  }
  else
    LOG(error) << "LmvmHist::DrawAll: Choose dimension 1 or 2";

  DrawTextOnPad(text, 0.25, 0.9, 0.75, 0.999);
}

void LmvmHist::WriteToFile() { fHM.WriteToFile(); }

void LmvmHist::DrawEfficiency(TH1* h1, TH1* h2, double xPos, double yPos)
{
  string effTxt =
    (h2->GetEntries() != 0.) ? Cbm::NumberToString<double>((h1->GetEntries() / h2->GetEntries() * 100.), 1) : "";
  TText* t = new TText(xPos, yPos, effTxt.c_str());
  t->SetTextSize(0.1);
  t->Draw();
}

void LmvmHist::DrawAnaStepOnPad(ELmvmAnaStep step)
{
  DrawTextOnPad(LmvmHist::fAnaStepLatex[static_cast<int>(step)], 0.4, 0.9, 0.6, 0.999);
}

ClassImp(LmvmHist);
