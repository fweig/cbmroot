/* Copyright (C) 2012-2016 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Timur Ablyazimov */

/**
 * \file CbmLitFitQaReport.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
#include "CbmLitFitQaReport.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmReportElement.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"

#include <boost/assign/list_of.hpp>

#include <map>

using boost::assign::list_of;
using Cbm::NumberToString;
using Cbm::Split;
using std::map;
using std::string;
using std::vector;

CbmLitFitQaReport::CbmLitFitQaReport() : CbmSimulationReport() { SetReportName("fit_qa"); }

CbmLitFitQaReport::~CbmLitFitQaReport() {}

void CbmLitFitQaReport::Create()
{
  Out().precision(3);
  Out() << R()->DocumentBegin();
  Out() << R()->Title(0, GetTitle());

  Out() << R()->TableBegin("Residuals and pulls (standard deviation)",
                           list_of("")("x")("y")("tx")("ty")("q/p")("x")("y")("tx")("ty")("q/p"));
  Out() << PrintResAndPullRow("STS first", "htf_Sts_FirstParam_(Res|Pull)_.+", "sigma");
  Out() << PrintResAndPullRow("STS last", "htf_Sts_LastParam_(Res|Pull)_.+", "sigma");
  Out() << PrintResAndPullRow("TRD first", "htf_Trd_FirstParam_(Res|Pull)_.+", "sigma");
  Out() << PrintResAndPullRow("TRD last", "htf_Trd_LastParam_(Res|Pull)_.+", "sigma");
  Out() << PrintResAndPullRow("MUCH first", "htf_Much_FirstParam_(Res|Pull)_.+", "sigma");
  Out() << PrintResAndPullRow("MUCH last", "htf_Much_LastParam_(Res|Pull)_.+", "sigma");
  Out() << R()->TableEnd();

  Out() << R()->TableBegin("Residuals and pulls (RMS)",
                           list_of("")("x")("y")("tx")("ty")("q/p")("x")("y")("tx")("ty")("q/p"));
  Out() << PrintResAndPullRow("STS first", "htf_Sts_FirstParam_(Res|Pull)_.+", "rms");
  Out() << PrintResAndPullRow("STS last", "htf_Sts_LastParam_(Res|Pull)_.+", "rms");
  Out() << PrintResAndPullRow("TRD first", "htf_Trd_FirstParam_(Res|Pull)_.+", "rms");
  Out() << PrintResAndPullRow("TRD last", "htf_Trd_LastParam_(Res|Pull)_.+", "rms");
  Out() << PrintResAndPullRow("MUCH first", "htf_Much_FirstParam_(Res|Pull)_.+", "rms");
  Out() << PrintResAndPullRow("MUCH last", "htf_Much_LastParam_(Res|Pull)_.+", "rms");
  Out() << R()->TableEnd();

  Out() << R()->TableBegin("Residuals and pulls (mean)",
                           list_of("")("x")("y")("tx")("ty")("q/p")("x")("y")("tx")("ty")("q/p"));
  Out() << PrintResAndPullRow("STS first", "htf_Sts_FirstParam_(Res|Pull)_.+", "mean");
  Out() << PrintResAndPullRow("STS last", "htf_Sts_LastParam_(Res|Pull)_.+", "mean");
  Out() << PrintResAndPullRow("TRD first", "htf_Trd_FirstParam_(Res|Pull)_.+", "mean");
  Out() << PrintResAndPullRow("TRD last", "htf_Trd_LastParam_(Res|Pull)_.+", "mean");
  Out() << PrintResAndPullRow("MUCH first", "htf_Much_FirstParam_(Res|Pull)_.+", "mean");
  Out() << PrintResAndPullRow("MUCH last", "htf_Much_LastParam_(Res|Pull)_.+", "mean");
  Out() << R()->TableEnd();

  PrintCanvases();

  Out() << R()->DocumentEnd();
}

string CbmLitFitQaReport::PrintResAndPullRow(const string& rowName, const string& pattern, const string& propertyName)
{
  // Maps parameter name to cell index
  static map<string, Int_t> ctc;
  ctc["X"]  = 0;
  ctc["Y"]  = 1;
  ctc["Tx"] = 2;
  ctc["Ty"] = 3;
  ctc["Qp"] = 4;

  vector<TH2*> histos = HM()->H2Vector(pattern);
  Int_t nofHistos     = histos.size();
  if (nofHistos == 0) return "";
  vector<string> parameters(nofHistos);
  for (UInt_t iHist = 0; iHist < histos.size(); iHist++) {
    vector<string> split = Split(histos[iHist]->GetName(), '_');
    TH1D* py             = histos[iHist]->ProjectionY();

    Int_t parIndex = (split[3] == "Res") ? (ctc[split[4]]) : (ctc[split[4]] + 5);
    if (propertyName == "mean") { parameters[parIndex] = NumberToString<Float_t>(py->GetMean(), 2); }
    else if (propertyName == "rms") {
      parameters[parIndex] = NumberToString<Float_t>(py->GetRMS(), 2);
    }
    else if (propertyName == "sigma") {
      py->Fit("gaus", "RQ");
      TF1* fit             = py->GetFunction("gaus");
      parameters[parIndex] = NumberToString<Float_t>((NULL != fit) ? fit->GetParameter(2) : 0., 2);
    }
  }
  return R()->TableRow(list_of(rowName).range(parameters));
}

void CbmLitFitQaReport::Draw()
{
  // 1D
  DrawResidualAndPullHistograms("Sts", false);
  DrawResidualAndPullHistograms("Trd", false);
  DrawResidualAndPullHistograms("Much", false);

  // 2D vs momentum
  DrawResidualAndPullHistograms("Sts", true);
  DrawResidualAndPullHistograms("Trd", true);
  DrawResidualAndPullHistograms("Much", true);

  DrawTrackParams("Sts");
  DrawTrackParams("Trd");
  DrawTrackParams("Much");

  DrawTrackParamsAtVertex();
  DrawTrackMomentumAtVertex();
}

void CbmLitFitQaReport::DrawHistSigmaRMS(Double_t sigma, Double_t rms)
{
  string txt1 = Cbm::NumberToString<Double_t>(sigma, 2) + " / " + Cbm::NumberToString<Double_t>(rms, 2);
  TLatex text;
  text.SetTextAlign(21);
  text.SetTextSize(0.08);
  text.DrawTextNDC(0.5, 0.83, txt1.c_str());
}

void CbmLitFitQaReport::DrawResidualAndPullHistograms(const string& detName, Bool_t draw2D)
{
  string parameterNames[] = {"X", "Y", "Tx", "Ty", "Qp"};
  string catNames[]       = {"Res", "Pull", "WrongCov"};

  if (!HM()->Exists("htf_" + detName + "_FirstParam_Res_X")) return;

  // [0] - for the first track parameter, [1] - for the last track parameter
  for (Int_t i = 0; i < 2; i++) {
    string trackParamName = (i == 0) ? "FirstParam" : "LastParam";

    string canvasName = "fit_qa_" + detName + "_" + trackParamName;
    if (draw2D) canvasName += "_2d";
    TCanvas* canvas = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1400, 900);
    canvas->Divide(5, 3);

    // [0] - "Res", [1] - "Pull", [2] - "WrongCov"
    for (Int_t iCat = 0; iCat < 3; iCat++) {
      for (Int_t iPar = 0; iPar < 5; iPar++) {
        string histName = "htf_" + detName + "_" + trackParamName + "_" + catNames[iCat] + "_" + parameterNames[iPar];
        if (!HM()->Exists(histName)) return;
        Int_t histId = iCat * 5 + iPar;
        canvas->cd(histId + 1);
        TH2* hist2D = HM()->H2(histName);

        if (draw2D) { DrawH2WithProfile(hist2D, false, false, "COLZ", kBlack, 3); }
        else {
          TH1* hist = (TH1D*) hist2D->ProjectionY()->Clone();
          DrawH1(hist, kLinear, kLog);

          if (histId < 10) {  // Fit only residual and pull histograms
            hist->Fit("gaus", "RQ");
            hist->SetMaximum(hist->GetMaximum() * 1.50);
            TF1* fit       = hist->GetFunction("gaus");
            Double_t sigma = (NULL != fit) ? fit->GetParameter(2) : 0.;
            Double_t rms   = hist->GetRMS();
            DrawHistSigmaRMS(sigma, rms);
          }
        }
      }
    }
  }
}

void CbmLitFitQaReport::DrawTrackParams(const string& detName)
{
  if (!HM()->Exists("htp_" + detName + "_FirstParam_X")) return;
  for (Int_t i = 0; i < 2; i++) {
    string trackParamName = (i == 0) ? "FirstParam" : "LastParam";
    string canvasName     = string("fit_qa_track_params_" + detName + trackParamName);
    TCanvas* canvas       = CreateCanvas(canvasName.c_str(), canvasName.c_str(), 2000, 1000);
    canvas->Divide(4, 2);
    string pattern      = string("htp_") + detName + "_" + trackParamName + "_.+";
    vector<TH1*> histos = HM()->H1Vector(pattern);
    Int_t nofHistos     = histos.size();
    for (Int_t iHist = 0; iHist < nofHistos; iHist++) {
      canvas->cd(iHist + 1);
      DrawH1(histos[iHist]);
      gPad->SetGridx(true);
      gPad->SetGridy(true);
    }
  }
}

void CbmLitFitQaReport::DrawTrackParamsAtVertex()
{
  TCanvas* canvas1 = CreateCanvas("fit_qa_momentum_momres_mom_2D", "fit_qa_momentum_momres_mom_2D", 600, 600);
  canvas1->cd(1);
  DrawH2(HM()->H2("htf_MomRes_Mom"));
  gPad->SetGridx(true);
  gPad->SetGridy(true);

  TCanvas* canvas2 = CreateCanvas("fit_qa_momentum_projection", "fit_qa_momentum_projection", 600, 600);
  canvas2->cd(1);
  TH1* projY = HM()->H2("htf_MomRes_Mom")->ProjectionY("htf_MomRes_Mom_ProjectionY");
  DrawH1(projY, kLinear, kLinear);
  projY->SetStats(true);
  projY->Fit("gaus", "RQ");
  projY->SetMaximum(projY->GetMaximum() * 1.25);
  gPad->SetGridx(true);
  gPad->SetGridy(true);

  TCanvas* canvas3 = CreateCanvas("fit_qa_momentum_momres_mom_sigma", "fit_qa_momentum_momres_mom_sigma", 600, 600);
  canvas3->cd(1);
  HM()->H2("htf_MomRes_Mom")->FitSlicesY();
  TH1* momslice = gDirectory->Get<TH1>("htf_MomRes_Mom_2");
  momslice->GetXaxis()->SetTitle("P [GeV/c]");
  momslice->GetYaxis()->SetTitle("dP/P, #sigma [%]");
  momslice->SetMinimum(0.);
  momslice->SetMaximum(3.);
  DrawH1(momslice, kLinear, kLinear);
  gPad->SetGridx(true);
  gPad->SetGridy(true);

  TCanvas* canvas4 = CreateCanvas("fit_qa_momentum_momres_mom_rms", "fit_qa_momentum_momres_mom_rms", 600, 600);
  canvas4->cd(1);
  TH2* hMomres   = HM()->H2("htf_MomRes_Mom");
  Int_t nBins    = hMomres->GetNbinsX();
  TH1* momResRms = hMomres->ProjectionX();
  for (Int_t i = 1; i < nBins; i++) {
    projY        = hMomres->ProjectionY("_py", i, i);
    Double_t rms = projY->GetRMS();
    momResRms->SetBinContent(i, rms);
    momResRms->SetBinError(i, momslice->GetBinError(i));
  }
  momResRms->GetXaxis()->SetTitle("P [GeV/c]");
  momResRms->GetYaxis()->SetTitle("dP/P, RMS [%]");
  momResRms->SetMinimum(0.);
  momResRms->SetMaximum(3.);
  DrawH1(momResRms, kLinear, kLinear, "P");
  gPad->SetGridx(true);
  gPad->SetGridy(true);

  TCanvas* canvas5 = CreateCanvas("fit_qa_chi_primary", "fit_qa_chi_primary", 600, 600);
  canvas5->cd(1);
  TH1* hChiprim = HM()->H1("htf_ChiPrimary");
  hChiprim->Scale(1. / hChiprim->Integral());
  DrawH1(hChiprim, kLinear, kLog);
  gPad->SetGridx(true);
  gPad->SetGridy(true);
}

void CbmLitFitQaReport::DrawTrackMomentumAtVertex()
{
  const char* histNames[] = {"htp_PrimaryVertexResidualPx", "htp_PrimaryVertexResidualPy",
                             "htp_PrimaryVertexResidualPz", "htp_PrimaryVertexPullPx",
                             "htp_PrimaryVertexPullPy",     "htp_PrimaryVertexPullPz"};
  TCanvas* canvas         = CreateCanvas("Momentum at primary vertex residuals and pulls",
                                 "Momentum at primary vertex residuals and pulls", 900, 600);
  canvas->Divide(3, 2);

  for (Int_t i = 0; i < 2; ++i) {
    for (Int_t j = 0; j < 3; ++j) {
      Int_t histId = i * 3 + j;
      canvas->cd(histId + 1);
      TH1* hist = HM()->H1(histNames[histId]);
      DrawH1(hist, kLinear, kLinear);
      hist->SetStats(true);
      hist->Fit("gaus");
      gStyle->SetOptFit(1);
    }
  }

  const char* histNames2[] = {"htp_PrimaryVertexResidualTx", "htp_PrimaryVertexResidualTy",
                              "htp_PrimaryVertexResidualQp", "htp_PrimaryVertexPullTx",
                              "htp_PrimaryVertexPullTy",     "htp_PrimaryVertexPullQp"};
  TCanvas* canvas2         = CreateCanvas("Tangents and Qp at primary vertex residuals and pulls",
                                  "Tangents and Qp at primary vertex residuals and pulls", 900, 600);
  canvas2->Divide(3, 2);

  for (Int_t i = 0; i < 2; ++i) {
    for (Int_t j = 0; j < 3; ++j) {
      Int_t histId = i * 3 + j;
      canvas2->cd(histId + 1);
      TH1* hist = HM()->H1(histNames2[histId]);
      DrawH1(hist, kLinear, kLinear);
      hist->SetStats(true);
      hist->Fit("gaus");
      gStyle->SetOptFit(1);
    }
  }
}

ClassImp(CbmLitFitQaReport)
