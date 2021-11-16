/* Copyright (C) 2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#include "LmvmHist.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "utils/CbmUtils.h"

#include "Logger.h"

#include "TText.h"


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

const vector<std::string> LmvmHist::fSignalNames = {"immed", "qgp", "omega", "phi", "omegaD"};
const vector<ELmvmSignal> LmvmHist::fSignals     = {ELmvmSignal::Inmed, ELmvmSignal::Qgp, ELmvmSignal::Omega,
                                                ELmvmSignal::Phi, ELmvmSignal::OmegaD};

const vector<std::string> LmvmHist::fBgPairSrcNames = {"GG", "PP", "OO", "GP", "GO", "PO"};
const vector<std::string> LmvmHist::fBgPairSrcLatex = {"#gamma-#gamma",  "#pi^{0}-#pi^{0}", "o.-o.",
                                                       "#gamma-#pi^{0}", "#gamma-o.",       "#pi^{0}-o."};


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
  double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.;
  H1(name, src)->Fill(x, myWeight);
}

void LmvmHist::FillH2(const string& name, ELmvmSrc src, double x, double y, double wSignal)
{
  if (src == ELmvmSrc::Undefined) return;
  double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.;
  H2(name, src)->Fill(x, y, myWeight);
}

void LmvmHist::FillH1(const string& name, ELmvmAnaStep step, double x, double w) { H1(name, step)->Fill(x, w); }

void LmvmHist::FillH2(const string& name, ELmvmAnaStep step, double x, double y, double w)
{
  H2(name, step)->Fill(x, y, w);
}

void LmvmHist::FillH1(const string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double wSignal)
{
  if (src == ELmvmSrc::Undefined || step == ELmvmAnaStep::Undefined) return;
  double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.;
  FillH1(GetName(name, src, step), x, myWeight);
}

void LmvmHist::FillH2(const string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double y, double wSignal)
{
  if (src == ELmvmSrc::Undefined || step == ELmvmAnaStep::Undefined) return;
  double myWeight = (src == ELmvmSrc::Signal) ? wSignal : 1.;
  FillH2(GetName(name, src, step), x, y, myWeight);
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
