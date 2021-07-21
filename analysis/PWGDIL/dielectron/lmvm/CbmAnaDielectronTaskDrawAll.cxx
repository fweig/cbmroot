/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva, Andrey Lebedev, Semen Lebedev [committer] */

/** CbmAnaDielectronTaskDrawAll.cxx
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2011
 * @version 2.0
 **/

#include "CbmAnaDielectronTaskDrawAll.h"

#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmUtils.h"

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

using namespace std;
using namespace Cbm;

void CbmAnaDielectronTaskDrawAll::DrawHistosFromFile(const string& fileNameInmed, const string& fileNameQgp,
                                                     const string& fileNameOmega, const string& fileNamePhi,
                                                     const string& fileNameOmegaDalitz, const string& outputDir,
                                                     Bool_t useMvd)
{
  fOutputDir = outputDir;
  fUseMvd    = useMvd;
  fDrawQgp   = (fileNameQgp != "");

  nRebin = 100;

  //SetDefaultDrawStyle();
  vector<string> fileNames = {fileNameInmed, fileNameQgp, fileNameOmega, fileNamePhi, fileNameOmegaDalitz};

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fHM.resize(fNofSignals);
  for (int i = 0; i < fNofSignals; i++) {
    fHM[i] = new CbmHistManager();
    if (!fDrawQgp && i == kQgp) continue;
    TFile* file = new TFile(fileNames[i].c_str());
    fHM[i]->ReadFromFile(file);
    Int_t nofEvents = (int) H1(i, "fh_event_number")->GetEntries();
    //fHM[i]->ScaleByPattern(".*", 1. / nofEvents); // TODO: keep this commented?
    cout << "nofEvents = " << nofEvents << endl;
  }

  // index: AnalysisSteps
  fh_mean_bg_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_eta_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_pi0_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_sum_s_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_eta_minv_pt.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_pi0_minv_pt.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_sbg_vs_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsPM_sameEvent_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsPP_sameEvent_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsMM_sameEvent_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsPM_mixedEvents_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsPP_mixedEvents_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combPairsMM_mixedEvents_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_raw_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_assemb_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_GeomMeanSame_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_GeomMeanMixed_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_k_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combSignalNpm_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combSignalNpm_assemb_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combSignalBCoc_assemb_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combBg_errProp_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combSignal_errProp_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_combSBg_vs_minv.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_nof_plutoElectrons.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_nof_plutoPositrons.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_nof_urqmdElectrons.resize(CbmLmvmHist::fNofAnaSteps);
  fh_mean_nof_urqmdPositrons.resize(CbmLmvmHist::fNofAnaSteps);

  FillMeanHist();  // TODO: Add method RebinHist() after this to rebin all histograms there to have better overview??
  FillSumSignalsHist();
  CalcCutEffRange(0.0, 0.2);
  CalcCutEffRange(0.2, 0.6);
  CalcCutEffRange(0.6, 1.2);
  CalcCombBGHistos();
  SBgRangeAll();
  DrawSBgSignals();
  DrawMinvAll();
  DrawMinvCombSignalAndBg();
  DrawMinvPtAll();
  DrawSBgVsMinv();
  //CompareSTSversions();
  SaveHist();
  SaveCanvasToImage();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}


TH1D* CbmAnaDielectronTaskDrawAll::H1(int signalType, const string& name) { return (TH1D*) fHM[signalType]->H1(name); }

TH2D* CbmAnaDielectronTaskDrawAll::H2(int signalType, const string& name) { return (TH2D*) fHM[signalType]->H1(name); }

TH1D* CbmAnaDielectronTaskDrawAll::GetCoctailMinv(CbmLmvmAnalysisSteps step)
{
  TH1D* sInmed = (TH1D*) H1(kInmed, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* sQgp   = (fDrawQgp) ? (TH1D*) H1(kQgp, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone() : nullptr;
  TH1D* sOmega = (TH1D*) H1(kOmega, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* sPhi   = (TH1D*) H1(kPhi, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* sEta   = fh_mean_eta_minv[step];
  TH1D* sPi0   = fh_mean_pi0_minv[step];
  TH1D* sOmegaDalitz = (TH1D*) H1(kOmegaD, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();

  Int_t nofEvents = 0;
  for (int i = 0; i < fNofSignals; i++) {
    if (!fDrawQgp && i == kQgp) continue;
    Int_t nofEventsS = (int) H1(i, "fh_event_number")->GetEntries();
    if (i == 0) sInmed->Scale(1. / nofEventsS);
    if (i == 1) sQgp->Scale(1. / nofEventsS);
    if (i == 2) sOmega->Scale(1. / nofEventsS);
    if (i == 3) sPhi->Scale(1. / nofEventsS);
    if (i == 4) sOmegaDalitz->Scale(1. / nofEventsS);

    nofEvents += nofEventsS;
    if (i == 4) cout << "GetCoctailMinv(...): nofEvents = " << nofEvents << endl;
  }

  TH1D* coctail = (TH1D*) sInmed->Clone();
  if (fDrawQgp) coctail->Add(sQgp);
  coctail->Add(sOmega);
  coctail->Add(sPhi);
  coctail->Add(sEta);
  coctail->Add(sPi0);
  coctail->Add(sOmegaDalitz);

  //coctail->Scale(1. / (fNofSignals + 2));	// '+2' because except signals there are two particles (eta and pi0)

  return coctail;
}

void CbmAnaDielectronTaskDrawAll::DrawMinvAll()
{
  fHM[0]->CreateCanvas("minv_all_mc", "minv_all_mc", 800, 800);
  DrawMinv(kMc);

  fHM[0]->CreateCanvas("minv_all_acc", "minv_all_acc", 800, 800);
  DrawMinv(kAcc);

  fHM[0]->CreateCanvas("minv_all_ptcut", "minv_all_ptcut", 800, 800);
  DrawMinv(kPtCut);

  fHM[0]->CreateCanvas("minv_all_ttcut", "minv_all_ttcut", 800, 800);
  DrawMinv(kTtCut);

  fHM[0]->CreateCanvas("minv_all_elid", "minv_all_elid", 800, 800);
  DrawMinv(kElId);
}

void CbmAnaDielectronTaskDrawAll::DrawMinv(CbmLmvmAnalysisSteps step)
{
  TH1D* sInmed = (TH1D*) H1(kInmed, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* sQgp   = (fDrawQgp) ? (TH1D*) H1(kQgp, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone() : nullptr;
  TH1D* sOmega = (TH1D*) H1(kOmega, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* sPhi   = (TH1D*) H1(kPhi, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH1D* bg     = (TH1D*) fh_mean_bg_minv[step]->Clone();
  TH1D* sEta   = (TH1D*) fh_mean_eta_minv[step]->Clone();
  TH1D* sPi0   = (TH1D*) fh_mean_pi0_minv[step]->Clone();
  TH1D* sOmegaDalitz = (TH1D*) H1(kOmegaD, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();

  Int_t nofEvents = 0;
  for (int i = 0; i < fNofSignals; i++) {
    if (!fDrawQgp && i == kQgp) continue;
    Int_t nofEventsS = (int) H1(i, "fh_event_number")->GetEntries();
    if (i == 0) sInmed->Scale(1. / nofEventsS);
    if (i == 1) sQgp->Scale(1. / nofEventsS);
    if (i == 2) sOmega->Scale(1. / nofEventsS);
    if (i == 3) sPhi->Scale(1. / nofEventsS);
    if (i == 4) sOmegaDalitz->Scale(1. / nofEventsS);

    nofEvents += nofEventsS;
    cout << "DrawMinv(...): nofEvents = " << nofEvents << endl;
  }

  TH1D* coctail = GetCoctailMinv(step);

  TH1D* sbg = (TH1D*) bg->Clone();
  sbg->Add(sInmed);  // Why here not added coctail instead (is same procedure)?
  if (fDrawQgp) sbg->Add(sQgp);
  sbg->Add(sOmega);
  sbg->Add(sPhi);
  sbg->Add(sEta);
  sbg->Add(sPi0);
  sbg->Add(sOmegaDalitz);

  sbg->Rebin(nRebin);
  coctail->Rebin(nRebin);
  bg->Rebin(nRebin);
  sPi0->Rebin(nRebin);
  sEta->Rebin(nRebin);
  sOmegaDalitz->Rebin(nRebin);
  sOmega->Rebin(nRebin);
  sInmed->Rebin(nRebin);
  if (fDrawQgp) sQgp->Rebin(nRebin);
  sPhi->Rebin(nRebin);

  double binWidth = sbg->GetBinWidth(1);
  sbg->Scale(1. / binWidth);
  coctail->Scale(1. / binWidth);
  bg->Scale(1. / binWidth);
  sPi0->Scale(1. / binWidth);
  sEta->Scale(1. / binWidth);
  sOmegaDalitz->Scale(1. / binWidth);
  sOmega->Scale(1. / binWidth);
  sInmed->Scale(1. / binWidth);
  if (fDrawQgp) sQgp->Scale(1. / binWidth);
  sPhi->Scale(1. / binWidth);

  sbg->SetMinimum(5e-8);
  sbg->SetMaximum(2e-2);
  sbg->GetXaxis()->SetRangeUser(0, 2.);
  bg->GetXaxis()->SetRangeUser(0, 2.);
  coctail->GetXaxis()->SetRangeUser(0, 2.);
  sPi0->GetXaxis()->SetRangeUser(0, 2.);
  sEta->GetXaxis()->SetRangeUser(0, 2.);
  sOmegaDalitz->GetXaxis()->SetRangeUser(0, 2.);
  sOmega->GetXaxis()->SetRangeUser(0, 2.);
  sInmed->GetXaxis()->SetRangeUser(0, 2.);
  if (fDrawQgp) sQgp->GetXaxis()->SetRangeUser(0, 2.);
  sPhi->GetXaxis()->SetRangeUser(0, 2.);

  /*
    if (step == kMc) {
        DrawH1({coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sQgp, sPhi},
                {"", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99);
    } else {
        DrawH1({sbg, bg, coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sQgp, sPhi},
                {"", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99);
    }
  */

  if (step == kMc) {
    if (fDrawQgp) {
      DrawH1({coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sQgp, sPhi}, {"", "", "", "", "", "", "", ""}, kLinear,
             kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
    }
    else {
      DrawH1({coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sPhi}, {"", "", "", "", "", "", ""}, kLinear, kLog,
             false, 0.8, 0.8, 0.99, 0.99, "HIST L");
    }
  }
  else {
    if (fDrawQgp) {
      DrawH1({sbg, bg, coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sQgp, sPhi},
             {"", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
    }
    else {
      DrawH1({sbg, bg, coctail, sPi0, sEta, sOmegaDalitz, sOmega, sInmed, sPhi}, {"", "", "", "", "", "", "", "", ""},
             kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
    }
  }

  string yTitle = "dN/dM_{ee} [GeV/c^{2}]^{-1}";
  coctail->GetYaxis()->SetTitle(yTitle.c_str());
  sbg->GetYaxis()->SetTitle(yTitle.c_str());
  coctail->GetYaxis()->SetLabelSize(0.05);
  sbg->GetYaxis()->SetLabelSize(0.05);

  sInmed->SetFillColor(kMagenta - 3);
  sInmed->SetLineColor(kMagenta - 2);
  sInmed->SetLineStyle(0);
  sInmed->SetLineWidth(3);
  sInmed->SetFillStyle(3344);

  if (fDrawQgp) {
    sQgp->SetFillColor(kOrange - 2);
    sQgp->SetLineColor(kOrange - 3);
    sQgp->SetLineStyle(0);
    sQgp->SetLineWidth(3);
    sQgp->SetFillStyle(3444);
  }

  sOmega->SetFillColor(kOrange + 7);
  sOmega->SetLineColor(kOrange + 4);
  sOmega->SetLineStyle(0);
  sOmega->SetLineWidth(2);

  sPhi->SetFillColor(kAzure + 2);
  sPhi->SetLineColor(kAzure + 3);
  sPhi->SetLineStyle(0);
  sPhi->SetLineWidth(2);
  sPhi->SetFillStyle(3112);
  gStyle->SetHatchesLineWidth(1);
  gStyle->SetHatchesSpacing(1.);


  bg->SetFillColor(kGray);
  bg->SetLineColor(kBlack);
  bg->SetLineStyle(0);
  bg->SetLineWidth(1);


  sEta->SetFillColor(kRed - 4);
  sEta->SetLineColor(kRed + 2);
  sEta->SetLineStyle(0);
  sEta->SetLineWidth(2);


  sPi0->SetFillColor(kGreen - 3);
  sPi0->SetLineColor(kGreen + 3);
  sPi0->SetLineStyle(0);
  sPi0->SetLineWidth(2);

  sOmegaDalitz->SetFillColor(kCyan + 2);
  sOmegaDalitz->SetLineColor(kCyan + 4);
  sOmegaDalitz->SetLineStyle(0);
  sOmegaDalitz->SetLineWidth(2);

  sbg->SetFillColor(kBlack);
  sbg->SetLineColor(kBlack);
  sbg->SetLineStyle(0);
  sbg->SetLineWidth(1);

  coctail->SetLineColor(kRed + 2);
  coctail->SetFillStyle(0);
  coctail->SetLineWidth(3);

  if (step != kMc) {
    TLegend* legend = new TLegend(0.7, 0.6, 0.99, 0.99);
    legend->SetFillColor(kWhite);
    legend->SetTextSize(0.04);
    legend->AddEntry(sOmega, "#omega #rightarrow e^{+}e^{-}", "f");
    legend->AddEntry(sOmegaDalitz, "#omega #rightarrow #pi^{0}e^{+}e^{-}", "f");
    legend->AddEntry(sPhi, "#phi #rightarrow e^{+}e^{-}", "f");
    legend->AddEntry(sInmed, "in-medium #rho", "f");
    if (fDrawQgp) legend->AddEntry(sQgp, "QGP radiation", "f");
    legend->AddEntry(sEta, "#eta #rightarrow #gammae^{+}e^{-}", "f");
    legend->AddEntry(sPi0, "#pi^{0} #rightarrow #gammae^{+}e^{-}", "f");
    legend->AddEntry(coctail, "Cocktail", "f");
    legend->AddEntry(bg, "Background", "f");
    legend->AddEntry(sbg, "Cocktail+BG", "f");
    legend->Draw();
  }
  else {
    TLegend* legend = new TLegend(0.7, 0.7, 0.99, 0.99);
    legend->SetFillColor(kWhite);
    legend->SetTextSize(0.04);
    legend->AddEntry(sOmega, "#omega #rightarrow e^{+}e^{-}", "f");
    legend->AddEntry(sOmegaDalitz, "#omega #rightarrow #pi^{0}e^{+}e^{-}", "f");
    legend->AddEntry(sPhi, "#phi #rightarrow e^{+}e^{-}", "f");
    legend->AddEntry(sInmed, "in-medium #rho", "f");
    if (fDrawQgp) legend->AddEntry(sQgp, "QGP radiation", "f");
    legend->AddEntry(sEta, "#eta #rightarrow #gammae^{+}e^{-}", "f");
    legend->AddEntry(sPi0, "#pi^{0} #rightarrow #gammae^{+}e^{-}", "f");
    legend->AddEntry(coctail, "Cocktail", "f");
    legend->Draw();
  }
  gPad->SetLogy(true);
}

void CbmAnaDielectronTaskDrawAll::DrawMinvCombSignalAndBg()
{
  /*******************************************************************************
 * MIND: 																	    *
 * 'B' (capital) stands for same event, 'b' (not-capital) for mixed events!    	*
 *																			  	*
 * Order of draw functions:													  	*
 * 1) Yields of single e- and e+ per event					  				  	*
 * 2) e+e- | e+e+ | e-e- pairs from same and mixed events					  	*
 * 3) geometric mean (conv. and assembled)									  	*
 * 4) B_comb with MC BG and B+-												  	*
 * 5) k factor																  	*
 * 6) comb. signal															  	*
 *******************************************************************************/

  double yMin   = 1e-7;
  double yMax   = 5e-2;
  string yTitle = "dN/dM_{ee} [GeV/c^{2}]^{-1}";
  string xTitle = "M_{ee} [GeV/c^2]";

  bool setMinMax = true;

  // Rebin factor for single particle and CB histograms. Their nBin in Task.cxx differ from each other

  /* 1) draw number of e+ and e- per event */
  // vs. momentum
  TH1D* nPlutoElMc    = (TH1D*) fh_mean_nof_plutoElectrons[kMc]->Clone();
  TH1D* nPlutoPosMc   = (TH1D*) fh_mean_nof_plutoPositrons[kMc]->Clone();
  TH1D* nPlutoElAcc   = (TH1D*) fh_mean_nof_plutoElectrons[kAcc]->Clone();
  TH1D* nPlutoPosAcc  = (TH1D*) fh_mean_nof_plutoPositrons[kAcc]->Clone();
  TH1D* nPlutoElReco  = (TH1D*) fh_mean_nof_plutoElectrons[kReco]->Clone();
  TH1D* nPlutoPosReco = (TH1D*) fh_mean_nof_plutoPositrons[kReco]->Clone();
  TH1D* nPlutoElElid  = (TH1D*) fh_mean_nof_plutoElectrons[kElId]->Clone();
  TH1D* nPlutoPosElid = (TH1D*) fh_mean_nof_plutoPositrons[kElId]->Clone();
  TH1D* nPlutoElTt    = (TH1D*) fh_mean_nof_plutoElectrons[kTtCut]->Clone();
  TH1D* nPlutoPosTt   = (TH1D*) fh_mean_nof_plutoPositrons[kTtCut]->Clone();
  TH1D* nPlutoElPt    = (TH1D*) fh_mean_nof_plutoElectrons[kPtCut]->Clone();
  TH1D* nPlutoPosPt   = (TH1D*) fh_mean_nof_plutoPositrons[kPtCut]->Clone();
  TH1D* nUrqmdElMc    = (TH1D*) fh_mean_nof_urqmdElectrons[kMc]->Clone();
  TH1D* nUrqmdPosMc   = (TH1D*) fh_mean_nof_urqmdPositrons[kMc]->Clone();
  TH1D* nUrqmdElAcc   = (TH1D*) fh_mean_nof_urqmdElectrons[kAcc]->Clone();
  TH1D* nUrqmdPosAcc  = (TH1D*) fh_mean_nof_urqmdPositrons[kAcc]->Clone();
  TH1D* nUrqmdElReco  = (TH1D*) fh_mean_nof_urqmdElectrons[kReco]->Clone();
  TH1D* nUrqmdPosReco = (TH1D*) fh_mean_nof_urqmdPositrons[kReco]->Clone();
  TH1D* nUrqmdElElid  = (TH1D*) fh_mean_nof_urqmdElectrons[kElId]->Clone();
  TH1D* nUrqmdPosElid = (TH1D*) fh_mean_nof_urqmdPositrons[kElId]->Clone();
  TH1D* nUrqmdElTt    = (TH1D*) fh_mean_nof_urqmdElectrons[kTtCut]->Clone();
  TH1D* nUrqmdPosTt   = (TH1D*) fh_mean_nof_urqmdPositrons[kTtCut]->Clone();
  TH1D* nUrqmdElPt    = (TH1D*) fh_mean_nof_urqmdElectrons[kPtCut]->Clone();
  TH1D* nUrqmdPosPt   = (TH1D*) fh_mean_nof_urqmdPositrons[kPtCut]->Clone();

  double min1 = 1e-9;
  double max1 = 10;
  nPlutoElMc->SetMinimum(min1);
  nPlutoElMc->SetMaximum(max1);
  nPlutoPosMc->SetMinimum(min1);
  nPlutoPosMc->SetMaximum(max1);
  nPlutoElAcc->SetMinimum(min1);
  nPlutoElAcc->SetMaximum(max1);
  nPlutoPosAcc->SetMinimum(min1);
  nPlutoPosAcc->SetMaximum(max1);
  nPlutoElReco->SetMinimum(min1);
  nPlutoElReco->SetMaximum(max1);
  nPlutoPosReco->SetMinimum(min1);
  nPlutoPosReco->SetMaximum(max1);
  nPlutoElElid->SetMinimum(min1);
  nPlutoElElid->SetMaximum(max1);
  nPlutoPosElid->SetMinimum(min1);
  nPlutoPosElid->SetMaximum(max1);
  nPlutoElTt->SetMinimum(min1);
  nPlutoElTt->SetMaximum(max1);
  nPlutoPosTt->SetMinimum(min1);
  nPlutoPosTt->SetMaximum(max1);
  nPlutoElPt->SetMinimum(min1);
  nPlutoElPt->SetMaximum(max1);
  nPlutoPosPt->SetMinimum(min1);
  nPlutoPosPt->SetMaximum(max1);

  nUrqmdElMc->SetMinimum(min1);
  nUrqmdElMc->SetMaximum(max1);
  nUrqmdPosMc->SetMinimum(min1);
  nUrqmdPosMc->SetMaximum(max1);
  nUrqmdElAcc->SetMinimum(min1);
  nUrqmdElAcc->SetMaximum(max1);
  nUrqmdPosAcc->SetMinimum(min1);
  nUrqmdPosAcc->SetMaximum(max1);
  nUrqmdElReco->SetMinimum(min1);
  nUrqmdElReco->SetMaximum(max1);
  nUrqmdPosReco->SetMinimum(min1);
  nUrqmdPosReco->SetMaximum(max1);
  nUrqmdElElid->SetMinimum(min1);
  nUrqmdElElid->SetMaximum(max1);
  nUrqmdPosElid->SetMinimum(min1);
  nUrqmdPosElid->SetMaximum(max1);
  nUrqmdElTt->SetMinimum(min1);
  nUrqmdElTt->SetMaximum(max1);
  nUrqmdPosTt->SetMinimum(min1);
  nUrqmdPosTt->SetMaximum(max1);
  nUrqmdElPt->SetMinimum(min1);
  nUrqmdElPt->SetMaximum(max1);
  nUrqmdPosPt->SetMinimum(min1);
  nUrqmdPosPt->SetMaximum(max1);

  nPlutoElMc->GetYaxis()->SetTitle("per event");
  nPlutoElMc->SetTitle("PLUTO electrons");
  nUrqmdElMc->GetYaxis()->SetTitle("per event");
  nUrqmdElMc->SetTitle("UrQMD electrons");

  fHM[0]->CreateCanvas("fh_nof_plutoElPos", "fh_nof_plutoElPos", 800, 800);
  DrawH1({nPlutoElMc, nPlutoPosMc, nPlutoElAcc, nPlutoPosAcc, nPlutoElReco, nPlutoPosReco, nPlutoElElid, nPlutoPosElid,
          nPlutoElTt, nPlutoPosTt, nPlutoElPt, nPlutoPosPt},
         {"", "", "", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofPP = new TLegend(0.65, 0.6, 0.88, 0.93);
  legendNofPP->SetFillColor(kWhite);
  legendNofPP->AddEntry(nPlutoElMc, "electrons kMc");
  legendNofPP->AddEntry(nPlutoPosMc, "positrons kMc");
  legendNofPP->AddEntry(nPlutoElAcc, "electrons kAcc");
  legendNofPP->AddEntry(nPlutoPosAcc, "positrons kAcc");
  legendNofPP->AddEntry(nPlutoElReco, "electrons kReco");
  legendNofPP->AddEntry(nPlutoPosReco, "positrons kReco");
  legendNofPP->AddEntry(nPlutoElElid, "electrons kElId");
  legendNofPP->AddEntry(nPlutoPosElid, "positrons kElId");
  legendNofPP->AddEntry(nPlutoElTt, "electrons kTt");
  legendNofPP->AddEntry(nPlutoPosTt, "positrons kTt");
  legendNofPP->AddEntry(nPlutoElPt, "electrons kPt");
  legendNofPP->AddEntry(nPlutoPosPt, "positrons kPt");
  legendNofPP->Draw();

  fHM[0]->CreateCanvas("fh_nof_urqmdElPos", "fh_nof_urqmdElPos", 800, 800);
  DrawH1({nUrqmdElMc, nUrqmdPosMc, nUrqmdElAcc, nUrqmdPosAcc, nUrqmdElReco, nUrqmdPosReco, nUrqmdElElid, nUrqmdPosElid,
          nUrqmdElTt, nUrqmdPosTt, nUrqmdElPt, nUrqmdPosPt},
         {"", "", "", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofUP = new TLegend(0.65, 0.6, 0.88, 0.93);
  legendNofUP->SetFillColor(kWhite);
  legendNofUP->AddEntry(nUrqmdElMc, "electrons kMc");
  legendNofUP->AddEntry(nUrqmdPosMc, "positrons kMc");
  legendNofUP->AddEntry(nUrqmdElAcc, "electrons kAcc");
  legendNofUP->AddEntry(nUrqmdPosAcc, "positrons kAcc");
  legendNofUP->AddEntry(nUrqmdElReco, "electrons kReco");
  legendNofUP->AddEntry(nUrqmdPosReco, "positrons kReco");
  legendNofUP->AddEntry(nUrqmdElElid, "electrons kElId");
  legendNofUP->AddEntry(nUrqmdPosElid, "positrons kElId");
  legendNofUP->AddEntry(nUrqmdElTt, "electrons kTt");
  legendNofUP->AddEntry(nUrqmdPosTt, "positrons kTt");
  legendNofUP->AddEntry(nUrqmdElPt, "electrons kPt");
  legendNofUP->AddEntry(nUrqmdPosPt, "positrons kPt");
  legendNofUP->Draw();

  /* 2) Draw Pair Yields */

  // draw ratio e-e-/e+e+
  TH1D* ratPairsElid = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[kElId]->Clone();
  TH1D* ratPairsPt   = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[kPtCut]->Clone();

  ratPairsElid->Divide(fh_mean_combPairsPP_sameEvent_minv[kElId]);
  ratPairsPt->Divide(fh_mean_combPairsPP_sameEvent_minv[kPtCut]);

  ratPairsElid->GetXaxis()->SetRangeUser(0, 2.);
  ratPairsElid->GetXaxis()->SetTitle(xTitle.c_str());
  ratPairsElid->GetYaxis()->SetTitle("Ratio");
  ratPairsElid->SetTitle("Ratio e^{-}e^{-}/e^{+}e^{+}");

  fHM[0]->CreateCanvas("minv_CB_1_Ratio_eMeP", "minv_CB_1_Ratio_eMeP", 800, 800);
  DrawH1({ratPairsElid, ratPairsPt}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "");

  TLegend* legendRatio = new TLegend(0.2, 0.2, 0.7, 0.35);
  legendRatio->SetFillColor(kWhite);
  legendRatio->AddEntry(ratPairsElid, "ratio e^{-}e^{-}/e^{+}e^{+} pairs after El ID");
  legendRatio->AddEntry(ratPairsPt, "ratio e^{-}e^{-}/e^{+}e^{+} pairs after Pt Cut");
  legendRatio->Draw();

  // calculate ratio of e-e-/e+e+ pairs
  double nElPairsElid  = fh_mean_combPairsMM_sameEvent_minv[kElId]->GetEntries();
  double nPosPairsElid = fh_mean_combPairsPP_sameEvent_minv[kElId]->GetEntries();
  double nElPairsPt    = fh_mean_combPairsMM_sameEvent_minv[kPtCut]->GetEntries();
  double nPosPairsPt   = fh_mean_combPairsPP_sameEvent_minv[kPtCut]->GetEntries();

  double ratioEmEpElid = nElPairsElid / nPosPairsElid;
  double ratioEmEpPt   = nElPairsPt / nPosPairsPt;
  cout << "Ratio e-e-/e+e+ (pairs, El ID)  = " << ratioEmEpElid << endl;
  cout << "Ratio e-e-/e+e+ (pairs, Pt cut) = " << ratioEmEpPt << endl;

  // draw raw pairs
  TH1D* h21PMElidRaw = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();
  TH1D* h21PPElidRaw = (TH1D*) fh_mean_combPairsPP_sameEvent_minv[kElId]->Clone();
  TH1D* h21MMElidRaw = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[kElId]->Clone();
  TH1D* h21pmElidRaw = (TH1D*) fh_mean_combPairsPM_mixedEvents_minv[kElId]->Clone();
  TH1D* h21ppElidRaw = (TH1D*) fh_mean_combPairsPP_mixedEvents_minv[kElId]->Clone();
  TH1D* h21mmElidRaw = (TH1D*) fh_mean_combPairsMM_mixedEvents_minv[kElId]->Clone();

  double bW = h21PMElidRaw->GetBinWidth(1);  // MIND: 'bW' is used throughout this draw method!
  cout << "DrawMinvCombSignalAndBg(): bW = " << bW << endl;

  Int_t nofEvents = 0;
  for (int i = 0; i < fNofSignals; i++) {
    if (!fDrawQgp && i == kQgp) continue;
    nofEvents += (int) H1(i, "fh_event_number")->GetEntries();
    cout << "DrawMinvCombSignalAndBg: nofEvents = " << nofEvents << endl;
  }

  h21PMElidRaw->Scale(bW * nofEvents);
  h21PPElidRaw->Scale(bW * nofEvents);
  h21MMElidRaw->Scale(bW * nofEvents);
  h21pmElidRaw->Scale(bW * nofEvents);
  h21ppElidRaw->Scale(bW * nofEvents);
  h21mmElidRaw->Scale(bW * nofEvents);

  h21PMElidRaw->GetXaxis()->SetTitle(xTitle.c_str());
  h21PMElidRaw->GetYaxis()->SetTitle("absolute number");
  h21PMElidRaw->SetTitle("same events");
  h21pmElidRaw->GetXaxis()->SetTitle(xTitle.c_str());
  h21pmElidRaw->GetYaxis()->SetTitle("absolute number");
  h21pmElidRaw->SetTitle("mixed events");

  fHM[0]->CreateCanvas("minv_CB_2_rawPairs_same", "minv_CB_2_rawPairs_same", 800, 800);
  DrawH1({h21PMElidRaw, h21PPElidRaw, h21MMElidRaw}, {"", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "pe1");

  TLegend* legend21 = new TLegend(0.5, 0.8, 0.92, 0.92);
  legend21->SetFillColor(kWhite);
  legend21->AddEntry(h21PMElidRaw, "e+e- pairs after El ID");
  legend21->AddEntry(h21PPElidRaw, "e+e+ pairs after El ID");
  legend21->AddEntry(h21MMElidRaw, "e-e- pairs after El ID");
  legend21->Draw();

  fHM[0]->CreateCanvas("minv_CB_2_rawPairs_mixed", "minv_CB_2_rawPairs_mixed", 800, 800);
  DrawH1({h21pmElidRaw, h21ppElidRaw, h21mmElidRaw}, {"", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "pe1");

  TLegend* legend21b = new TLegend(0.5, 0.8, 0.92, 0.92);
  legend21b->SetFillColor(kWhite);
  legend21b->AddEntry(h21pmElidRaw, "e+e- pairs after El ID");
  legend21b->AddEntry(h21ppElidRaw, "e+e+ pairs after El ID");
  legend21b->AddEntry(h21mmElidRaw, "e-e- pairs after El ID");
  legend21b->Draw();

  // draw scaled pairs (same event)
  TH1D* Bpm = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();
  TH1D* Bpp = (TH1D*) fh_mean_combPairsPP_sameEvent_minv[kElId]->Clone();
  TH1D* Bmm = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[kElId]->Clone();


  Bpm->GetXaxis()->SetRangeUser(0, 2.);
  Bpm->GetXaxis()->SetTitle(xTitle.c_str());
  Bpm->GetYaxis()->SetTitle(yTitle.c_str());
  Bpm->SetTitle("same event");

  fHM[0]->CreateCanvas("minv_CB_2_pairsSameEvent", "minv_CB_2_pairsSameEvent", 800, 800);
  DrawH1({Bpm, Bpp, Bmm}, {"", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendB = new TLegend(0.5, 0.75, 0.95, 0.93);
  legendB->SetFillColor(kWhite);
  legendB->AddEntry(Bpm, "e+e- pairs (Elid)");
  legendB->AddEntry(Bpp, "e+e+ pairs (Elid)");
  legendB->AddEntry(Bmm, "e-e- pairs (Elid)");
  legendB->Draw();

  // draw scaled pairs (mixed events)
  TH1D* bpm = (TH1D*) fh_mean_combPairsPM_mixedEvents_minv[kElId]->Clone();
  TH1D* bpp = (TH1D*) fh_mean_combPairsPP_mixedEvents_minv[kElId]->Clone();
  TH1D* bmm = (TH1D*) fh_mean_combPairsMM_mixedEvents_minv[kElId]->Clone();

  bpm->GetXaxis()->SetRangeUser(0, 2.);
  bpm->GetXaxis()->SetTitle(xTitle.c_str());
  bpm->GetYaxis()->SetTitle(yTitle.c_str());
  bpm->SetTitle("mixed events");

  fHM[0]->CreateCanvas("minv_CB_2_pairsMixedEvents", "minv_CB_2_pairsMixedEvents", 800, 800);
  DrawH1({bpm, bpp, bmm}, {"", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendb = new TLegend(0.5, 0.75, 0.95, 0.93);
  legendb->SetFillColor(kWhite);
  legendb->AddEntry(bpm, "e+e- pairs (Elid)");
  legendb->AddEntry(bpp, "e+e+ pairs (Elid)");
  legendb->AddEntry(bmm, "e-e- pairs (Elid)");
  legendb->Draw();

  // compare B++ with b++ (and -- and +-, resp.); therefor normalize to integral(400,700)
  // convert MeV into Bin
  int intFrom    = 400;  // lower and upper range to normalize (in Mev/c^2)
  int intTo      = 700;
  int nBinsOrig  = 4000;  // was set in CbmAnaDielectronTask.cxx
  int upperLimit = 4000;  // in MeV (assumed, that lower limit = 0)
  double bin400  = (double) (1. * nBinsOrig / nRebin) * (1. * intFrom / upperLimit);
  double bin700  = (double) (1. * nBinsOrig / nRebin) * (1. * intTo / upperLimit);
  cout << "bin400 = " << bin400 << endl;
  cout << "bin700 = " << bin700 << endl;

  // e+e- pairs
  TH1D* BpmNormed = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();  // norm. factor = 1
  TH1D* bpmNormed = (TH1D*) fh_mean_combPairsPM_mixedEvents_minv[kElId]->Clone();

  BpmNormed->GetXaxis()->SetRangeUser(0, 2.);
  BpmNormed->GetXaxis()->SetTitle(xTitle.c_str());
  BpmNormed->GetYaxis()->SetTitle(yTitle.c_str());

  double intSamePM  = BpmNormed->Integral(bin400, bin700);
  double intMixedPM = bpmNormed->Integral(bin400, bin700);
  double normPM     = (double) intSamePM / intMixedPM;  // mixed histograms will be scaled with this factor

  int nofBins = BpmNormed->GetNbinsX();  // MIND: 'nofBins' is used throughout this draw method!

  for (int iBin = 1; iBin <= nofBins; iBin++) {
    double content = bpmNormed->GetBinContent(iBin);
    content *= normPM;
    bpmNormed->SetBinContent(iBin, content);
  }

  // e+e+ pairs
  TH1D* BppNormed = (TH1D*) fh_mean_combPairsPP_sameEvent_minv[kElId]->Clone();  // norm. factor = 1
  TH1D* bppNormed = (TH1D*) fh_mean_combPairsPP_mixedEvents_minv[kElId]->Clone();

  BppNormed->GetXaxis()->SetRangeUser(0, 2.);
  BppNormed->GetXaxis()->SetTitle(xTitle.c_str());
  BppNormed->GetYaxis()->SetTitle(yTitle.c_str());

  double intSamePP  = BppNormed->Integral(bin400, bin700);
  double intMixedPP = bppNormed->Integral(bin400, bin700);
  double normPP     = (double) intSamePP / intMixedPP;  // mixed histograms will be scaled with this factor

  for (int iBin = 1; iBin <= nofBins; iBin++) {
    double content = bppNormed->GetBinContent(iBin);
    content *= normPP;
    bppNormed->SetBinContent(iBin, content);
  }

  // e-e- pairs
  TH1D* BmmNormed = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[kElId]->Clone();  // norm. factor = 1
  TH1D* bmmNormed = (TH1D*) fh_mean_combPairsMM_mixedEvents_minv[kElId]->Clone();

  BmmNormed->GetXaxis()->SetRangeUser(0, 2.);
  BmmNormed->GetXaxis()->SetTitle(xTitle.c_str());
  BmmNormed->GetYaxis()->SetTitle(yTitle.c_str());

  double intSameMM  = BmmNormed->Integral(bin400, bin700);
  double intMixedMM = bmmNormed->Integral(bin400, bin700);
  double normMM     = (double) intSameMM / intMixedMM;  // mixed histograms will be scaled with this factor

  for (int iBin = 1; iBin <= nofBins; iBin++) {
    double content = bmmNormed->GetBinContent(iBin);
    content *= normMM;
    bmmNormed->SetBinContent(iBin, content);
  }

  BpmNormed->SetTitle("compare same/mixed e+e- events");
  BppNormed->SetTitle("compare same/mixed e+e+ events");
  BmmNormed->SetTitle("compare same/mixed e-e- events");

  TCanvas* c = fHM[0]->CreateCanvas("minv_CB_2_compare_same-mixed", "minv_CB_2_compare_same-mixed", 1800, 600);
  c->Divide(3, 1);

  c->cd(1);
  if (setMinMax) {
    BpmNormed->SetMinimum(yMin);
    BpmNormed->SetMaximum(2 * yMax);
  }
  DrawH1({BpmNormed, bpmNormed}, {"p", "p"}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
  TLegend* leg1 = new TLegend(0.5, 0.78, 0.92, 0.9);
  leg1->SetFillColor(kWhite);
  leg1->AddEntry(BpmNormed, "e^{+}-e^{-} same event (Elid)");
  leg1->AddEntry(bpmNormed, "e^{+}-e^{-} mixed events (Elid)");
  leg1->Draw();

  c->cd(2);
  if (setMinMax) {
    BppNormed->SetMinimum(yMin);
    BppNormed->SetMaximum(2 * yMax);
  }
  DrawH1({BppNormed, bppNormed}, {"p", "p"}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
  TLegend* leg2 = new TLegend(0.5, 0.78, 0.92, 0.9);
  leg2->SetFillColor(kWhite);
  leg2->AddEntry(BppNormed, "e^{+}-e^{+} same event (Elid)");
  leg2->AddEntry(bppNormed, "e^{+}-e^{+} mixed events (Elid)");
  leg2->Draw();

  c->cd(3);
  if (setMinMax) {
    BmmNormed->SetMinimum(yMin);
    BmmNormed->SetMaximum(2 * yMax);
  }
  DrawH1({BmmNormed, bmmNormed}, {"p", "p"}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");
  TLegend* leg3 = new TLegend(0.5, 0.78, 0.92, 0.9);
  leg3->SetFillColor(kWhite);
  leg3->AddEntry(BmmNormed, "e^{-}-e^{-} same event (Elid)");
  leg3->AddEntry(bmmNormed, "e^{-}-e^{-} mixed events (Elid)");
  leg3->Draw();

  // compare B+- with (Cocktail + BG)
  TH1D* Bpm1Elid   = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId];
  TH1D* Bpm1Pt     = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kPtCut];
  TH1D* cocBg1Elid = (TH1D*) GetCoctailMinv(kElId);
  TH1D* cocBg1Pt   = (TH1D*) GetCoctailMinv(kPtCut);

  cocBg1Elid->Add(fh_mean_bg_minv[kElId]);
  cocBg1Pt->Add(fh_mean_bg_minv[kPtCut]);

  cocBg1Elid->Rebin(nRebin);
  double bWCoc = cocBg1Elid->GetBinWidth(1);
  cocBg1Elid->Scale(1. / bWCoc);
  cocBg1Pt->Rebin(nRebin);
  cocBg1Pt->Scale(1. / bWCoc);

  Bpm1Elid->GetXaxis()->SetRangeUser(0, 2.);
  Bpm1Elid->GetXaxis()->SetTitle(xTitle.c_str());
  Bpm1Elid->GetYaxis()->SetTitle(yTitle.c_str());

  fHM[0]->CreateCanvas("minv_CB_2_BpmVsMC", "minv_CB_2_BpmVsMC", 800, 800);
  DrawH1({Bpm1Elid, cocBg1Elid, Bpm1Pt, cocBg1Pt}, {"", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99,
         "HIST L");

  TLegend* legendCocBg = new TLegend(0.5, 0.7, 0.92, 0.91);
  legendCocBg->SetFillColor(kWhite);
  legendCocBg->AddEntry(Bpm1Elid, "N_{same}^{+-} (Elid)");
  legendCocBg->AddEntry(cocBg1Elid, "Cocktail + BG (Elid)");
  legendCocBg->AddEntry(Bpm1Pt, "N_{same}^{+-} (P_{t} cut)");
  legendCocBg->AddEntry(cocBg1Pt, "Cocktail + BG (P_{t} cut)");
  legendCocBg->Draw();

  TH1D* ratCocBgBpmElid = (TH1D*) cocBg1Elid->Clone();
  TH1D* ratCocBgBpmPt   = (TH1D*) cocBg1Pt->Clone();

  ratCocBgBpmElid->Divide(Bpm1Elid);
  ratCocBgBpmPt->Divide(Bpm1Pt);

  ratCocBgBpmElid->GetXaxis()->SetRangeUser(0, 2.);
  ratCocBgBpmElid->GetYaxis()->SetTitle(0);
  ratCocBgBpmElid->SetTitle("Ratio (Cocktail + BG) / N_{same}^{+-}");

  fHM[0]->CreateCanvas("minv_CB_2_BpmVsMC2", "minv_CB_2_BpmVsMC2", 800, 800);
  DrawH1({ratCocBgBpmElid, ratCocBgBpmPt}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendCocBg2 = new TLegend(0.55, 0.75, 0.95, 0.91);
  legendCocBg2->SetFillColor(kWhite);
  legendCocBg2->AddEntry(ratCocBgBpmElid, "El ID cut");
  legendCocBg2->AddEntry(ratCocBgBpmPt, "P_{t} cut");
  legendCocBg2->Draw();

  /* 3) Draw Geometric Mean */
  TH1D* geomMeanSameElid  = (TH1D*) fh_mean_combBg_GeomMeanSame_minv[kElId]->Clone();
  TH1D* geomMeanMixedElid = (TH1D*) fh_mean_combBg_GeomMeanMixed_minv[kElId]->Clone();
  TH1D* geomMeanSamePt    = (TH1D*) fh_mean_combBg_GeomMeanSame_minv[kPtCut]->Clone();
  TH1D* geomMeanMixedPt   = (TH1D*) fh_mean_combBg_GeomMeanMixed_minv[kPtCut]->Clone();

  double intGMsameElid  = geomMeanSameElid->Integral(bin400, bin700);
  double intGMmixedElid = geomMeanMixedElid->Integral(bin400, bin700);
  double normGMElid     = (double) intGMsameElid / intGMmixedElid;  // mixed histograms will be scaled with this factor

  double intGMsamePt  = geomMeanSamePt->Integral(bin400, bin700);
  double intGMmixedPt = geomMeanMixedPt->Integral(bin400, bin700);
  double normGMPt     = (double) intGMsamePt / intGMmixedPt;

  for (int iBin = 1; iBin <= nofBins; iBin++) {
    double contentElid = geomMeanMixedElid->GetBinContent(iBin);
    double contentPt   = geomMeanMixedPt->GetBinContent(iBin);
    contentElid *= normGMElid;
    contentPt *= normGMPt;
    geomMeanMixedElid->SetBinContent(iBin, contentElid);
    geomMeanMixedPt->SetBinContent(iBin, contentPt);
  }
  if (setMinMax) {
    geomMeanSameElid->SetMinimum(yMin);
    geomMeanSameElid->SetMaximum(yMax);
  }

  geomMeanSameElid->GetXaxis()->SetRangeUser(0, 2.);
  geomMeanSameElid->GetXaxis()->SetTitle(xTitle.c_str());
  geomMeanSameElid->GetYaxis()->SetTitle(yTitle.c_str());
  geomMeanSameElid->SetTitle("geometric mean (normalized)");
  geomMeanMixedElid->SetTitle(0);

  fHM[0]->CreateCanvas("minv_CB_3_geomMean", "minv_CB_3_geomMean", 800, 800);
  DrawH1({geomMeanSameElid, geomMeanMixedElid, geomMeanSamePt, geomMeanMixedPt}, {"", "", "", ""}, kLinear, kLog, false,
         0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendgeomMean = new TLegend(0.55, 0.8, 0.92, 0.91);
  legendgeomMean->SetFillColor(kWhite);
  legendgeomMean->AddEntry(geomMeanSameElid, "same event (elid)");
  legendgeomMean->AddEntry(geomMeanMixedElid, "mixed events (elid)");
  legendgeomMean->AddEntry(geomMeanSamePt, "same event (Pt)");
  legendgeomMean->AddEntry(geomMeanMixedPt, "mixed events (Pt)");
  legendgeomMean->Draw();

  //draw geom. mean, assembled of same (<= 0.3 GeV) and normed mixed (> 0.3 GeV) data

  // convert MeV into Bin
  int rFrom      = 300;  // lower and upper range to normalize (in Mev/c^2)
  int rTo        = 1000;
  double binFrom = (double) (1. * nBinsOrig / nRebin) * (1. * rFrom / upperLimit);
  double binTo   = (double) (1. * nBinsOrig / nRebin) * (1. * rTo / upperLimit);

  TH1D* geomMeanAss = (TH1D*) fh_mean_combBg_GeomMeanSame_minv[kElId]->Clone();

  double intSameGMsp  = geomMeanAss->Integral(binFrom, binTo);
  double intMixedGMsp = geomMeanMixedElid->Integral(binFrom, binTo);
  double normGMsp     = (double) intSameGMsp / intMixedGMsp;

  for (int iBin = binFrom + 1; iBin <= nofBins; iBin++) {  // from 300 MeV on normalized data from mixed event
    double cont = geomMeanMixedElid->GetBinContent(iBin);
    cont *= normGMsp;
    geomMeanAss->SetBinContent(iBin, cont);
  }
  if (setMinMax) {
    geomMeanAss->SetMinimum(yMin);
    geomMeanAss->SetMaximum(yMax);
  }

  geomMeanAss->GetXaxis()->SetRangeUser(0, 2.);
  geomMeanAss->GetXaxis()->SetTitle(xTitle.c_str());
  geomMeanAss->GetYaxis()->SetTitle(yTitle.c_str());
  geomMeanAss->SetTitle("geometric mean (splitted)");

  fHM[0]->CreateCanvas("minv_CB_3_geomMean_assembled", "minv_CB_3_geomMean_assembled", 800, 800);
  DrawH1({geomMeanAss}, {""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist l");

  TLegend* legendgeomMeanAss = new TLegend(0.55, 0.8, 0.91, 0.91);
  legendgeomMeanAss->SetFillColor(kWhite);
  legendgeomMeanAss->AddEntry(geomMeanAss, "geom. mean");
  legendgeomMeanAss->Draw();

  /* 4) Draw B_comb with Coc+BG and with Bpm */
  TH1D* cbgElid  = (TH1D*) fh_mean_combBg_minv[kElId]->Clone();
  TH1D* cbgPtCut = (TH1D*) fh_mean_combBg_minv[kPtCut]->Clone();

  TH1D* bgCocElid = (TH1D*) GetCoctailMinv(kElId);
  TH1D* bgCocPt   = (TH1D*) GetCoctailMinv(kPtCut);

  bgCocElid->Add(fh_mean_bg_minv[kElId]);
  bgCocPt->Add(fh_mean_bg_minv[kPtCut]);

  TH1D* BpmElid = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();  // MIND: BpmElid is also used below!
  TH1D* BpmPt   = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kPtCut]->Clone();

  bgCocElid->Rebin(nRebin);  // these histos have to be rebinned and scaled here because not done yet as with CB histos
  bgCocPt->Rebin(nRebin);
  bgCocElid->Scale(1. / bW);
  bgCocPt->Scale(1. / bW);

  cbgElid->GetXaxis()->SetRangeUser(0, 2.);
  cbgElid->GetXaxis()->SetTitle(xTitle.c_str());
  cbgElid->GetYaxis()->SetTitle(yTitle.c_str());
  cbgElid->SetTitle(0);

  if (setMinMax) {
    cbgElid->SetMinimum(yMin);
    cbgElid->SetMaximum(yMax);
  }

  fHM[0]->CreateCanvas("minv_CB_4_bComb", "minv_CB_4_bComb", 800, 800);
  DrawH1({cbgElid, bgCocElid}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendBg = new TLegend(0.55, 0.7, 0.95, 0.9);
  legendBg->SetFillColor(kWhite);
  legendBg->AddEntry(cbgElid, "comb. BG (2 * k * geomMean, elid)");
  legendBg->AddEntry(bgCocElid, "Cocktail + BG (elid)");
  legendBg->Draw();

  TH1D* ratCocBgCbgElid = (TH1D*) bgCocElid->Clone();
  TH1D* ratCocBgCbgPt   = (TH1D*) bgCocPt->Clone();

  ratCocBgCbgElid->Divide(cbgElid);
  ratCocBgCbgPt->Divide(cbgPtCut);

  if (setMinMax) {
    ratCocBgCbgElid->SetMinimum(2e-1);
    ratCocBgCbgElid->SetMaximum(50);
    ratCocBgCbgPt->SetMinimum(2e-1);
    ratCocBgCbgPt->SetMaximum(50);
    cbgElid->SetMinimum(yMin);
    cbgElid->SetMaximum(yMax);
  }

  fHM[0]->CreateCanvas("minv_CB_4_bCombVsBpm", "minv_CB_4_bCombVsBpm", 800, 800);
  DrawH1({cbgElid, BpmElid, cbgPtCut, BpmPt}, {"", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendBg2 = new TLegend(0.55, 0.7, 0.92, 0.91);
  legendBg2->SetFillColor(kWhite);
  legendBg2->AddEntry(cbgElid, "comb. BG, Elid");
  legendBg2->AddEntry(BpmElid, "N_{same}^{+-}, Elid");
  legendBg2->AddEntry(cbgPtCut, "comb. BG, PtCut");
  legendBg2->AddEntry(BpmPt, "N_{same}^{+-}, PtCut");
  legendBg2->Draw();

  ratCocBgCbgElid->GetXaxis()->SetRangeUser(0, 2.);
  ratCocBgCbgElid->SetTitle("Ratio (Cocktail + BG) / B_{c}");

  fHM[0]->CreateCanvas("minv_CB_4_bComb_rat", "minv_CB_4_bComb_rat", 800, 800);
  DrawH1({ratCocBgCbgElid, ratCocBgCbgPt}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendBg1 = new TLegend(0.55, 0.8, 0.92, 0.9);
  legendBg1->SetFillColor(kWhite);
  legendBg1->AddEntry(ratCocBgCbgElid, "El ID cut");
  legendBg1->AddEntry(ratCocBgCbgPt, "Pt cut");
  legendBg1->Draw();

  // draw ass. CB with all steps
  TH1D* cbgAssElid  = (TH1D*) fh_mean_combBg_assemb_minv[kElId]->Clone();
  TH1D* cbgAssGamma = (TH1D*) fh_mean_combBg_assemb_minv[kGammaCut]->Clone();
  TH1D* cbgAssMvd1  = (TH1D*) fh_mean_combBg_assemb_minv[kMvd1Cut]->Clone();
  TH1D* cbgAssSt    = (TH1D*) fh_mean_combBg_assemb_minv[kStCut]->Clone();
  TH1D* cbgAssRt    = (TH1D*) fh_mean_combBg_assemb_minv[kRtCut]->Clone();
  TH1D* cbgAssTt    = (TH1D*) fh_mean_combBg_assemb_minv[kTtCut]->Clone();
  TH1D* cbgAssPt    = (TH1D*) fh_mean_combBg_assemb_minv[kPtCut]->Clone();

  TH1D* ratCocBgCbgAssElid = (TH1D*) bgCocElid->Clone();
  TH1D* ratCocBgCbgAssPt   = (TH1D*) bgCocPt->Clone();

  ratCocBgCbgAssElid->Divide(cbgAssElid);
  ratCocBgCbgAssPt->Divide(cbgAssPt);

  if (setMinMax) {
    ratCocBgCbgAssElid->SetMinimum(2e-1);
    ratCocBgCbgAssElid->SetMaximum(50);
    ratCocBgCbgAssPt->SetMinimum(2e-1);
    ratCocBgCbgAssPt->SetMaximum(50);
  }

  ratCocBgCbgAssElid->GetXaxis()->SetRangeUser(0, 2.);
  ratCocBgCbgAssElid->SetTitle("Ratio (Cocktail + BG) / B_{c_ass}");

  fHM[0]->CreateCanvas("minv_CB_4_bComb_rat2", "minv_CB_4_bComb_rat2", 800, 800);
  DrawH1({ratCocBgCbgAssElid, ratCocBgCbgAssPt}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendBg4 = new TLegend(0.55, 0.8, 0.92, 0.9);
  legendBg4->SetFillColor(kWhite);
  legendBg4->AddEntry(ratCocBgCbgAssElid, "El ID cut");
  legendBg4->AddEntry(ratCocBgCbgAssPt, "Pt cut");
  legendBg4->Draw();

  // conv. vs assembled CB
  TH1D* h44cbgElid    = (TH1D*) fh_mean_combBg_minv[kElId]->Clone();
  TH1D* h44cbgAssElid = (TH1D*) fh_mean_combBg_assemb_minv[kElId]->Clone();

  if (setMinMax) {
    h44cbgElid->SetMinimum(yMin);
    h44cbgElid->SetMaximum(yMax);
  }

  h44cbgElid->GetXaxis()->SetRangeUser(0, 2.);
  h44cbgElid->GetXaxis()->SetTitle(xTitle.c_str());
  h44cbgElid->GetYaxis()->SetTitle(yTitle.c_str());
  h44cbgElid->SetTitle("Combinatorial BG");

  fHM[0]->CreateCanvas("minv_CB_4_bComb_convVsAss", "minv_CB_4_bComb_convVsAss", 800, 800);
  DrawH1({h44cbgElid, h44cbgAssElid}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "pe1");

  TLegend* legendBg3 = new TLegend(0.55, 0.7, 0.92, 0.91);
  legendBg3->SetFillColor(kWhite);
  legendBg3->AddEntry(h44cbgElid, "conv. CB, Elid");
  legendBg3->AddEntry(h44cbgAssElid, "assembled CB, Elid");
  legendBg3->Draw();

  // assembled CB with all steps
  if (setMinMax) {
    cbgAssElid->SetMinimum(yMin);
    cbgAssElid->SetMaximum(yMax);
  }
  cbgAssElid->GetXaxis()->SetRangeUser(0, 2.);
  cbgAssElid->GetXaxis()->SetTitle(xTitle.c_str());
  cbgAssElid->GetYaxis()->SetTitle(yTitle.c_str());
  cbgAssElid->SetTitle("Combinatorial BG");

  fHM[0]->CreateCanvas("minv_CB_4_bComb_assembled_steps", "minv_CB_4_bComb_assembled_steps", 800, 800);
  DrawH1({cbgAssElid, cbgAssGamma, cbgAssMvd1, cbgAssSt, cbgAssRt, cbgAssTt, cbgAssPt}, {"", "", "", "", "", "", ""},
         kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "pe1");

  TLegend* legendBg5 = new TLegend(0.7, 0.65, 0.92, 0.91);
  legendBg5->SetFillColor(kWhite);
  legendBg5->AddEntry(cbgAssElid, "elid");
  legendBg5->AddEntry(cbgAssGamma, "gamma cut");
  legendBg5->AddEntry(cbgAssMvd1, "MVD1 cut");
  legendBg5->AddEntry(cbgAssSt, "ST cut");
  legendBg5->AddEntry(cbgAssRt, "RT cut");
  legendBg5->AddEntry(cbgAssTt, "TT cut");
  legendBg5->AddEntry(cbgAssPt, "P_{t} cut");
  legendBg5->Draw();

  /* 5) Draw k Factor */
  TH1D* kReconst = (TH1D*) fh_mean_combBg_k_minv[kReco]->Clone();
  TH1D* kChi2    = (TH1D*) fh_mean_combBg_k_minv[kChi2Prim]->Clone();
  TH1D* kElid    = (TH1D*) fh_mean_combBg_k_minv[kElId]->Clone();
  TH1D* kGamma   = (TH1D*) fh_mean_combBg_k_minv[kGammaCut]->Clone();
  TH1D* kMvd1    = (TH1D*) fh_mean_combBg_k_minv[kMvd1Cut]->Clone();
  TH1D* kSt      = (TH1D*) fh_mean_combBg_k_minv[kStCut]->Clone();
  TH1D* kRt      = (TH1D*) fh_mean_combBg_k_minv[kRtCut]->Clone();
  TH1D* kTt      = (TH1D*) fh_mean_combBg_k_minv[kTtCut]->Clone();
  TH1D* kPt      = (TH1D*) fh_mean_combBg_k_minv[kPtCut]->Clone();

  if (setMinMax) {
    kReconst->SetMinimum(0.7);
    kReconst->SetMaximum(1.3);
    kChi2->SetMinimum(0.7);
    kChi2->SetMaximum(1.3);
    kElid->SetMinimum(0.7);
    kElid->SetMaximum(1.3);
    kGamma->SetMinimum(0.7);
    kGamma->SetMaximum(1.3);
    kMvd1->SetMinimum(0.7);
    kMvd1->SetMaximum(1.3);
    kSt->SetMinimum(0.7);
    kSt->SetMaximum(1.3);
    kRt->SetMinimum(0.7);
    kRt->SetMaximum(1.3);
    kTt->SetMinimum(0.7);
    kTt->SetMaximum(1.3);
    kPt->SetMinimum(0.7);
    kPt->SetMaximum(1.3);
  }

  kReconst->GetXaxis()->SetRangeUser(0, 2.);
  kReconst->GetXaxis()->SetTitle(xTitle.c_str());
  kReconst->GetYaxis()->SetTitle(0);
  kReconst->SetTitle("k factor");

  fHM[0]->CreateCanvas("minv_CB_5_k", "minv_CB_5_k", 800, 800);
  DrawH1({kReconst, kChi2, kElid, kGamma, kMvd1, kSt, kRt, kTt, kPt}, {"", "", "", "", "", "", "", "", ""}, kLinear,
         kLinear, false, 0.8, 0.8, 0.99, 0.99, "HIST L");

  TLegend* legendK = new TLegend(0.7, 0.7, 0.92, 0.91);
  legendK->SetFillColor(kWhite);
  legendK->AddEntry(kReconst, "reco");
  legendK->AddEntry(kChi2, "#chi^{2} ");
  legendK->AddEntry(kElid, "elid");
  legendK->AddEntry(kGamma, "gamma cut");
  legendK->AddEntry(kMvd1, "MVD1 cut");
  legendK->AddEntry(kSt, "ST cut");
  legendK->AddEntry(kRt, "RT cut");
  legendK->AddEntry(kTt, "TT cut");
  legendK->AddEntry(kPt, "P_{t} cut");
  legendK->Draw();

  /* 6) Draw Combinatorial Signal (all from assembled comb. BG) */
  // from 'N+-same'

  // elid, draw also components
  TH1D* h61NpmElid    = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();
  TH1D* h61cbAssElid  = (TH1D*) fh_mean_combBg_assemb_minv[kElId]->Clone();
  TH1D* h61SigNpmElid = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kElId]->Clone();
  TH1D* h61coc1Elid   = (TH1D*) GetCoctailMinv(kElId);

  h61coc1Elid->Add(fh_mean_bg_minv[kElId]);
  h61coc1Elid->Add(fh_mean_bg_minv[kElId], -1.);

  h61coc1Elid->Rebin(nRebin);
  h61coc1Elid->Scale(1. / bW);

  if (setMinMax) {
    h61NpmElid->SetMinimum(yMin);
    h61NpmElid->SetMaximum(yMax);
    h61cbAssElid->SetMinimum(yMin);
    h61cbAssElid->SetMaximum(yMax);
    h61SigNpmElid->SetMinimum(yMin);
    h61SigNpmElid->SetMaximum(yMax);
    h61coc1Elid->SetMinimum(yMin);
    h61coc1Elid->SetMaximum(yMax);
  }

  h61NpmElid->GetXaxis()->SetRangeUser(0, 2.);
  h61NpmElid->GetYaxis()->SetTitle(yTitle.c_str());
  h61NpmElid->SetTitle(0);
  h61cbAssElid->GetXaxis()->SetRangeUser(0, 2.);
  h61SigNpmElid->GetXaxis()->SetRangeUser(0, 2.);
  h61coc1Elid->GetXaxis()->SetRangeUser(0, 2.);

  fHM[0]->CreateCanvas("minv_CB_6_signal_Npm_elid", "minv_CB_6_signal_Npm_elid", 800, 800);
  DrawH1({h61NpmElid, h61cbAssElid, h61SigNpmElid, h61coc1Elid}, {"", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99,
         0.99, "hist p");

  TLegend* legend61 = new TLegend(0.55, 0.7, 0.98, 0.9);
  legend61->SetFillColor(kWhite);
  legend61->AddEntry(h61NpmElid, "N_{same}^{+-} (Elid)");
  legend61->AddEntry(h61cbAssElid, "B_{c} (assembled, Elid)");
  legend61->AddEntry(h61SigNpmElid, "Signal (N_{same}^{+-} - B_{c}, Elid)");
  legend61->AddEntry(h61coc1Elid, "Cocktail, Elid)");
  legend61->Draw();

  // all steps, draw with and wo error
  TH1D* h63SigNpmElid  = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kElId]->Clone();
  TH1D* h63SigNpmGamma = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kGammaCut]->Clone();
  TH1D* h63SigNpmMvd1  = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kMvd1Cut]->Clone();
  TH1D* h63SigNpmSt    = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kStCut]->Clone();
  TH1D* h63SigNpmRt    = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kRtCut]->Clone();
  TH1D* h63SigNpmTt    = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kTtCut]->Clone();
  TH1D* h63SigNpmPt    = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kPtCut]->Clone();

  TH1D* h63SigNpmPt2 = (TH1D*) fh_mean_combSignalNpm_assemb_minv[kPtCut]->Clone();

  if (setMinMax) {
    h63SigNpmElid->SetMinimum(yMin);  // setting min/max somehow makes the histos empty
    h63SigNpmElid->SetMaximum(yMax);
    h63SigNpmGamma->SetMinimum(yMin);
    h63SigNpmGamma->SetMaximum(yMax);
    h63SigNpmMvd1->SetMinimum(yMin);
    h63SigNpmMvd1->SetMaximum(yMax);
    h63SigNpmSt->SetMinimum(yMin);
    h63SigNpmSt->SetMaximum(yMax);
    h63SigNpmRt->SetMinimum(yMin);
    h63SigNpmRt->SetMaximum(yMax);
    h63SigNpmTt->SetMinimum(yMin);
    h63SigNpmTt->SetMaximum(yMax);
    h63SigNpmPt->SetMinimum(yMin);
    h63SigNpmPt->SetMaximum(yMax);
    h63SigNpmPt2->SetMinimum(yMin);
    h63SigNpmPt2->SetMaximum(yMax);
  }

  h63SigNpmElid->GetXaxis()->SetRangeUser(0, 2.);
  h63SigNpmElid->GetXaxis()->SetTitle(xTitle.c_str());
  h63SigNpmElid->GetYaxis()->SetTitle(yTitle.c_str());
  h63SigNpmElid->SetTitle("Signal");
  h63SigNpmPt2->GetXaxis()->SetRangeUser(0, 2.);
  h63SigNpmPt2->GetXaxis()->SetTitle(xTitle.c_str());
  h63SigNpmPt2->GetYaxis()->SetTitle(yTitle.c_str());
  h63SigNpmPt2->SetTitle("Signal");

  fHM[0]->CreateCanvas("minv_CB_6_signal_Npm_steps", "minv_CB_6_signal_Npm_steps", 800, 800);
  DrawH1({h63SigNpmElid, h63SigNpmGamma, h63SigNpmMvd1, h63SigNpmSt, h63SigNpmRt, h63SigNpmTt, h63SigNpmPt},
         {"", "", "", "", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist p");

  TLegend* legend63 = new TLegend(0.7, 0.6, 0.92, 0.9);
  legend63->SetFillColor(kWhite);
  legend63->AddEntry(h63SigNpmElid, "elid");
  legend63->AddEntry(h63SigNpmGamma, "gamma cut");
  legend63->AddEntry(h63SigNpmMvd1, "MVD1 cut");
  legend63->AddEntry(h63SigNpmSt, "ST cut");
  legend63->AddEntry(h63SigNpmRt, "Rt cut");
  legend63->AddEntry(h63SigNpmTt, "Tt cut");
  legend63->AddEntry(h63SigNpmPt, "P_{t} cut");
  legend63->Draw();

  fHM[0]->CreateCanvas("minv_CB_6_signal_Npm_steps2", "minv_CB_6_signal_Npm_steps2", 800, 800);
  DrawH1({h63SigNpmElid, h63SigNpmGamma, h63SigNpmMvd1, h63SigNpmSt, h63SigNpmRt, h63SigNpmTt, h63SigNpmPt},
         {"", "", "", "", "", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist pe1");

  TLegend* legend63e = new TLegend(0.7, 0.6, 0.92, 0.9);
  legend63e->SetFillColor(kWhite);
  legend63e->AddEntry(h63SigNpmElid, "elid");
  legend63e->AddEntry(h63SigNpmGamma, "gamma cut");
  legend63e->AddEntry(h63SigNpmMvd1, "MVD1 cut");
  legend63e->AddEntry(h63SigNpmSt, "ST cut");
  legend63e->AddEntry(h63SigNpmRt, "Rt cut");
  legend63e->AddEntry(h63SigNpmTt, "Tt cut");
  legend63e->AddEntry(h63SigNpmPt, "P_{t} cut");
  legend63e->Draw();

  fHM[0]->CreateCanvas("minv_CB_6_signal_Npm_steps3", "minv_CB_6_signal_Npm_steps3", 800, 800);
  DrawH1({h63SigNpmElid}, {""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist pe1");

  TLegend* legend63e3 = new TLegend(0.7, 0.8, 0.92, 0.9);
  legend63e3->SetFillColor(kWhite);
  legend63e3->AddEntry(h63SigNpmElid, "elid");
  legend63e3->Draw();

  fHM[0]->CreateCanvas("minv_CB_6_signal_Npm_steps4", "minv_CB_6_signal_Npm_steps4", 800,
                       800);  // Wenn aktiviert, sind Elid- und Pt-Kurven oben beide gleichfarbig
  DrawH1({h63SigNpmPt2}, {""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist pe1");

  TLegend* legend63e4 = new TLegend(0.7, 0.8, 0.92, 0.9);
  legend63e4->SetFillColor(kWhite);
  legend63e4->AddEntry(h63SigNpmPt2, "P_{t} cut");
  legend63e4->Draw();

  // from 'Cocktail + BG'
  // elid
  TH1D* h65SigCocBgElid = (TH1D*) fh_mean_combSignalBCoc_assemb_minv[kElId]->Clone();
  TH1D* h65cbAssElid    = (TH1D*) fh_mean_combBg_assemb_minv[kElId]->Clone();
  TH1D* h65cocBgElid    = (TH1D*) GetCoctailMinv(kElId);
  TH1D* h65coc1Elid     = (TH1D*) GetCoctailMinv(kElId);

  h65cocBgElid->Add(fh_mean_bg_minv[kElId]);

  h65coc1Elid->Add(fh_mean_bg_minv[kElId]);
  h65coc1Elid->Add(fh_mean_bg_minv[kElId], -1.);

  h65cocBgElid->Rebin(nRebin);
  h65cocBgElid->Scale(1. / bW);
  h65coc1Elid->Rebin(nRebin);
  h65coc1Elid->Scale(1. / bW);

  if (setMinMax) {
    h65cocBgElid->SetMinimum(yMin);
    h65cocBgElid->SetMaximum(yMax);
  }

  h65cocBgElid->GetXaxis()->SetRangeUser(0, 2.);
  h65cocBgElid->GetYaxis()->SetTitle(yTitle.c_str());
  h65cocBgElid->SetTitle(0);

  fHM[0]->CreateCanvas("minv_CB_6_signal_bgCoc_elid", "minv_CB_6_signal_bgCoc_elid", 800, 800);
  DrawH1({h65cocBgElid, h65cbAssElid, h65SigCocBgElid, h65coc1Elid}, {"", "", "", ""}, kLinear, kLog, false, 0.8, 0.8,
         0.99, 0.99, "");

  /*h65coc1Elid->SetFillColor(kMagenta - 3);
  h65coc1Elid->SetLineColor(kMagenta - 2);
  h65coc1Elid->SetLineStyle(1);
  h65coc1Elid->SetLineWidth(3);
  h65coc1Elid->SetFillStyle(3344);*/

  h65cocBgElid->SetLineStyle(0);
  h65cbAssElid->SetLineStyle(2);
  h65SigCocBgElid->SetLineStyle(3);
  h65coc1Elid->SetLineStyle(1);

  TLegend* legend65 = new TLegend(0.55, 0.75, 0.98, 0.9);
  //legend65->SetFillColor(kWhite);
  legend65->AddEntry(h65cocBgElid, "Cocktail + BG (Elid)");
  legend65->AddEntry(h65cbAssElid, "B_{c} (assembled, Elid)");
  legend65->AddEntry(h65SigCocBgElid, "Signal (Coc + BG - B_{c}, Elid)");
  legend65->AddEntry(h65coc1Elid, "Cocktail (Elid)");
  legend65->Draw();

  // Pt cut
  TH1D* h66SigCocBgPt = (TH1D*) fh_mean_combSignalBCoc_assemb_minv[kPtCut]->Clone();
  TH1D* h66cbAssPt    = (TH1D*) fh_mean_combBg_assemb_minv[kPtCut]->Clone();
  TH1D* h66cocBgPt    = (TH1D*) GetCoctailMinv(kPtCut);
  h66cocBgPt->Add(fh_mean_bg_minv[kPtCut]);

  h66cocBgPt->Rebin(nRebin);
  h66cocBgPt->Scale(1. / bW);

  if (setMinMax) {
    h66cocBgPt->SetMinimum(yMin);
    h66cocBgPt->SetMaximum(yMax);
  }

  h66cocBgPt->GetXaxis()->SetRangeUser(0, 2.);
  h66cocBgPt->GetYaxis()->SetTitle(yTitle.c_str());
  h66cocBgPt->SetTitle(0);

  fHM[0]->CreateCanvas("minv_CB_6_signal_bgCoc_pt", "minv_CB_6_signal_bgCoc_pt", 800, 800);
  DrawH1({h66cocBgPt, h66cbAssPt, h66SigCocBgPt}, {"", "", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist p");

  TLegend* legend66 = new TLegend(0.55, 0.75, 0.98, 0.9);
  legend66->SetFillColor(kWhite);
  legend66->AddEntry(h66cocBgPt, "Cocktail + BG (P_{t})");
  legend66->AddEntry(h66cbAssPt, "B_{c} (assembled, P_{t})");
  legend66->AddEntry(h66SigCocBgPt, "Signal (Coc + BG - B_{c}, P_{t})");
  legend66->Draw();

  /* 7) Error */
  TH1D* h71NpmElid = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[kElId]->Clone();
  TH1D* h71sigElid = (TH1D*) fh_mean_combSignal_errProp_minv[kElId]->Clone();

  h71NpmElid->GetXaxis()->SetRangeUser(0, 2.);
  h71NpmElid->GetXaxis()->SetTitle(xTitle.c_str());
  h71NpmElid->GetYaxis()->SetTitle(yTitle.c_str());
  h71NpmElid->SetTitle(0);

  for (int iBin = 1; iBin <= nofBins; iBin++) {
    double errNpm = h71NpmElid->GetBinError(iBin);
    h71NpmElid->SetBinContent(iBin, errNpm);
  }

  fHM[0]->CreateCanvas("minv_CB_7_error", "minv_CB_7_error", 800, 800);
  DrawH1({h71NpmElid, h71sigElid}, {"", ""}, kLinear, kLog, false, 0.8, 0.8, 0.99, 0.99, "hist p");

  TLegend* legend71 = new TLegend(0.55, 0.75, 0.98, 0.9);
  legend71->SetFillColor(kWhite);
  legend71->AddEntry(h71NpmElid, "error N^{+-}_{same} (Elid)");
  legend71->AddEntry(h71sigElid, "error Signal = N^{+-}_{same} - B_{c} (Elid)");
  legend71->Draw();
}

void CbmAnaDielectronTaskDrawAll::DrawSBgVsMinv()
{
  TH1D* bg      = (TH1D*) fh_mean_bg_minv[kTtCut]->Clone();
  TH1D* combBg  = (TH1D*) fh_mean_combBg_minv[kTtCut]->Clone();
  TH1D* coctail = GetCoctailMinv(kTtCut);

  fh_mean_sbg_vs_minv[kTtCut] =
    new TH1D(("fh_sbg_vs_minv_" + CbmLmvmHist::fAnaSteps[kTtCut]).c_str(),
             ("fh_sbg_vs_minv_" + CbmLmvmHist::fAnaSteps[kTtCut] + ";M_{ee} [GeV/c^{2}];Cocktail/Background").c_str(),
             bg->GetNbinsX(), bg->GetXaxis()->GetXmin(), bg->GetXaxis()->GetXmax());
  fh_mean_sbg_vs_minv[kTtCut]->Divide(coctail, bg, 1., 1., "B");
  fh_mean_sbg_vs_minv[kTtCut]->Rebin(20);
  fh_mean_sbg_vs_minv[kTtCut]->Scale(1. / 20.);
  fh_mean_sbg_vs_minv[kTtCut]->GetXaxis()->SetRangeUser(0, 2.);

  fh_mean_combSBg_vs_minv[kAcc] =
    new TH1D(("fh_combSBg_vs_minv_" + CbmLmvmHist::fAnaSteps[kAcc]).c_str(),
             ("fh_combSBg_vs_minv_" + CbmLmvmHist::fAnaSteps[kAcc] + ";M_{ee} [GeV/c^{2}];Cocktail/comb. BG").c_str(),
             bg->GetNbinsX(), bg->GetXaxis()->GetXmin(), bg->GetXaxis()->GetXmax());
  fh_mean_combSBg_vs_minv[kAcc]->Divide(coctail, combBg, 1., 1., "B");
  fh_mean_combSBg_vs_minv[kAcc]->Rebin(20);
  fh_mean_combSBg_vs_minv[kAcc]->Scale(1. / 20.);
  fh_mean_combSBg_vs_minv[kAcc]->GetXaxis()->SetRangeUser(0, 2.);

  bg      = (TH1D*) fh_mean_bg_minv[kPtCut]->Clone();
  combBg  = (TH1D*) fh_mean_combBg_minv[kPtCut]->Clone();
  coctail = GetCoctailMinv(kPtCut);

  fh_mean_sbg_vs_minv[kPtCut] =
    new TH1D(("fh_sbg_vs_minv_" + CbmLmvmHist::fAnaSteps[kPtCut]).c_str(),
             ("fh_sbg_vs_minv_" + CbmLmvmHist::fAnaSteps[kPtCut] + ";M_{ee} [GeV/c^{2}];Cocktail/Background").c_str(),
             bg->GetNbinsX(), bg->GetXaxis()->GetXmin(), bg->GetXaxis()->GetXmax());
  fh_mean_sbg_vs_minv[kPtCut]->Divide(coctail, bg, 1., 1., "B");
  fh_mean_sbg_vs_minv[kPtCut]->Rebin(20);
  fh_mean_sbg_vs_minv[kPtCut]->Scale(1. / 20.);
  fh_mean_sbg_vs_minv[kPtCut]->GetXaxis()->SetRangeUser(0, 2.);

  /*fh_mean_combSBg_vs_minv[kPtCut] = new TH1D(
    ("fh_combSBg_vs_minv_" + CbmLmvmHist::fAnaSteps[kPtCut]).c_str(),
    ("fh_combSBg_vs_minv_" + CbmLmvmHist::fAnaSteps[kPtCut] + ";M_{ee} [GeV/c^{2}];Cocktail/comb. Background").c_str(),
    bg->GetNbinsX(), bg->GetXaxis()->GetXmin(), bg->GetXaxis()->GetXmax());
  fh_mean_combSBg_vs_minv[kPtCut]->Divide(coctail, combBg, 1., 1., "B");
  fh_mean_combSBg_vs_minv[kPtCut]->Rebin(20);
  fh_mean_combSBg_vs_minv[kPtCut]->Scale(1. / 20.);
  fh_mean_combSBg_vs_minv[kPtCut]->GetXaxis()->SetRangeUser(0, 2.);*/

  fHM[0]->CreateCanvas("lmvm_sbg_vs_minv", "lmvm_sbg_vs_minv", 800, 800);
  DrawH1({fh_mean_sbg_vs_minv[kTtCut], fh_mean_sbg_vs_minv[kPtCut]}, {"Without Pt cut", "With Pt cut"}, kLinear, kLog,
         true, 0.6, 0.85, 0.99, 0.99);
  gPad->SetLogy(true);

  /*fHM[0]->CreateCanvas("lmvm_sbgComb_vs_minv", "lmvm_sbgComb_vs_minv", 800, 800);
  DrawH1({fh_mean_combSBg_vs_minv[kAcc], fh_mean_combSBg_vs_minv[kPtCut]}, {"Without Pt cut", "With Pt cut"}, kLinear,
         kLog, true, 0.6, 0.85, 0.99, 0.99);*/
  gPad->SetLogy(true);
}

void CbmAnaDielectronTaskDrawAll::DrawMinvPtAll()
{
  fHM[0]->CreateCanvas("minv_pt_ptcut", "minv_pt_ptcut", 800, 800);
  DrawMinvPt(kPtCut);
}

void CbmAnaDielectronTaskDrawAll::DrawMinvPt(CbmLmvmAnalysisSteps step)
{
  TH2D* sInmed = (TH2D*) H2(kInmed, "fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH2D* sQgp   = (fDrawQgp) ? (TH2D*) H2(kQgp, "fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone() : nullptr;
  TH2D* sOmega = (TH2D*) H2(kOmega, "fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH2D* sOmegaDalitz = (TH2D*) H2(kOmegaD, "fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH2D* sPhi         = (TH2D*) H2(kPhi, "fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  TH2D* sEta         = fh_mean_eta_minv_pt[step];
  TH2D* sPi0         = fh_mean_pi0_minv_pt[step];
  TH2D* coctail      = (TH2D*) sInmed->Clone();
  if (fDrawQgp) coctail->Add(sQgp);
  coctail->Add(sOmega);
  coctail->Add(sPhi);
  coctail->Add(sOmegaDalitz);
  coctail->Add(sEta);
  coctail->Add(sPi0);
  DrawH2(coctail);
}

void CbmAnaDielectronTaskDrawAll::FillMeanHist()
{
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    for (int iS = 0; iS < fNofSignals; iS++) {
      if (!fDrawQgp && iS == kQgp) continue;
      if (iS == 0) {
        fh_mean_bg_minv[step]     = (TH1D*) H1(iS, "fh_bg_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_eta_minv[step]    = (TH1D*) H1(iS, "fh_eta_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_pi0_minv[step]    = (TH1D*) H1(iS, "fh_pi0_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_eta_minv_pt[step] = (TH2D*) H2(iS, "fh_eta_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_pi0_minv_pt[step] = (TH2D*) H2(iS, "fh_pi0_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsPM_sameEvent_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsPM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsPP_sameEvent_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsPP_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsMM_sameEvent_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsMM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsPM_mixedEvents_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsPM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsPP_mixedEvents_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsPP_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_combPairsMM_mixedEvents_minv[step] =
          (TH1D*) H1(iS, "fh_combPairsMM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_nof_plutoElectrons[step] =
          (TH1D*) H1(iS, "fh_nof_plutoElectrons_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_nof_plutoPositrons[step] =
          (TH1D*) H1(iS, "fh_nof_plutoPositrons_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_nof_urqmdElectrons[step] =
          (TH1D*) H1(iS, "fh_nof_urqmdElectrons_" + CbmLmvmHist::fAnaSteps[step])->Clone();
        fh_mean_nof_urqmdPositrons[step] =
          (TH1D*) H1(iS, "fh_nof_urqmdPositrons_" + CbmLmvmHist::fAnaSteps[step])->Clone();
      }
      else {
        fh_mean_bg_minv[step]->Add((TH1D*) H1(iS, "fh_bg_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_eta_minv[step]->Add((TH1D*) H1(iS, "fh_eta_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_pi0_minv[step]->Add((TH1D*) H1(iS, "fh_pi0_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_eta_minv_pt[step]->Add((TH2D*) H2(iS, "fh_eta_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_pi0_minv_pt[step]->Add((TH2D*) H2(iS, "fh_pi0_minv_pt_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsPM_sameEvent_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsPM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsPP_sameEvent_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsPP_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsMM_sameEvent_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsMM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsPM_mixedEvents_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsPM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsPP_mixedEvents_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsPP_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_combPairsMM_mixedEvents_minv[step]->Add(
          (TH1D*) H1(iS, "fh_combPairsMM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_nof_plutoElectrons[step]->Add(
          (TH1D*) H1(iS, "fh_nof_plutoElectrons_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_nof_plutoPositrons[step]->Add(
          (TH1D*) H1(iS, "fh_nof_plutoPositrons_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_nof_urqmdElectrons[step]->Add(
          (TH1D*) H1(iS, "fh_nof_urqmdElectrons_" + CbmLmvmHist::fAnaSteps[step])->Clone());
        fh_mean_nof_urqmdPositrons[step]->Add(
          (TH1D*) H1(iS, "fh_nof_urqmdPositrons_" + CbmLmvmHist::fAnaSteps[step])->Clone());
      }
    }

    Int_t nofEvents = 0;
    for (int i = 0; i < fNofSignals; i++) {
      if (!fDrawQgp && i == kQgp) continue;
      nofEvents += (int) H1(i, "fh_event_number")->GetEntries();
      cout << "FillMeanHist: nofEvents = " << nofEvents << endl;
    }
    cout << "FillMeanHist: final nofEvents = " << nofEvents << endl;

    // Scaling; Comb. BG histograms are scaled in CalcCombBGHistos()
    fh_mean_bg_minv[step]->Scale(1. / (double) (nofEvents));
    fh_mean_eta_minv[step]->Scale(1. / (double) (nofEvents));
    fh_mean_pi0_minv[step]->Scale(1. / (double) (nofEvents));
    fh_mean_eta_minv_pt[step]->Scale(1. / (double) (nofEvents));
    fh_mean_pi0_minv_pt[step]->Scale(1. / (double) (nofEvents));
    fh_mean_nof_plutoElectrons[step]->Scale(1. / (double) (nofEvents));
    fh_mean_nof_plutoPositrons[step]->Scale(1. / (double) (nofEvents));
    fh_mean_nof_urqmdElectrons[step]->Scale(1. / (double) (nofEvents));
    fh_mean_nof_urqmdPositrons[step]->Scale(1. / (double) (nofEvents));
  }  // steps
}

void CbmAnaDielectronTaskDrawAll::SaveHist()
{
  if (fOutputDir != "") {
    gSystem->mkdir(fOutputDir.c_str(), true);
    TFile* f = TFile::Open(string(fOutputDir + "/draw_all_hist.root").c_str(), "RECREATE");
    for (int i = 0; i < CbmLmvmHist::fNofAnaSteps; i++) {
      fh_mean_bg_minv[i]->Write();
      fh_mean_combBg_minv[i]->Write();
      fh_mean_combBg_GeomMeanSame_minv[i]->Write();
      fh_mean_combBg_GeomMeanMixed_minv[i]->Write();
      //fh_mean_combSignalNpm_minv[i]->Write();
      fh_mean_eta_minv[i]->Write();
    }
    fh_mean_sbg_vs_minv[kTtCut]->Write();
    fh_mean_sbg_vs_minv[kPtCut]->Write();
    f->Close();
  }
}

void CbmAnaDielectronTaskDrawAll::FillSumSignalsHist()
{
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    fh_sum_s_minv[step] = (TH1D*) H1(kInmed, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
    if (fDrawQgp) fh_sum_s_minv[step]->Add((TH1D*) H1(kQgp, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
    fh_sum_s_minv[step]->Add((TH1D*) H1(kOmega, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
    fh_sum_s_minv[step]->Add((TH1D*) H1(kPhi, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
    fh_sum_s_minv[step]->Add((TH1D*) H1(kOmegaD, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone());
    fh_sum_s_minv[step]->Add((TH1D*) fh_mean_eta_minv[step]->Clone());
    fh_sum_s_minv[step]->Add((TH1D*) fh_mean_pi0_minv[step]->Clone());
  }
}

void CbmAnaDielectronTaskDrawAll::CalcCutEffRange(Double_t minMinv, Double_t maxMinv)
{
  stringstream ss1;
  ss1 << minMinv << "_" << maxMinv;
  TH1D* grS = new TH1D(("grS_" + ss1.str()).c_str(), ";Analysis step;Efficiency [%]", CbmLmvmHist::fNofAnaSteps, 0,
                       CbmLmvmHist::fNofAnaSteps);
  TH1D* grB = new TH1D(("grB_" + ss1.str()).c_str(), ";Analysis step;Efficiency [%]", CbmLmvmHist::fNofAnaSteps, 0,
                       CbmLmvmHist::fNofAnaSteps);
  int x     = 1;
  for (int step = kElId; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) { continue; }
    Int_t x1 = fh_sum_s_minv[step]->FindBin(minMinv);
    Int_t x2 = fh_sum_s_minv[step]->FindBin(maxMinv);

    double yS = 100. * fh_sum_s_minv[step]->Integral(x1, x2) / fh_sum_s_minv[kElId]->Integral(x1, x2);
    double yB = 100. * fh_mean_bg_minv[step]->Integral(x1, x2) / fh_mean_bg_minv[kElId]->Integral(x1, x2);

    grB->GetXaxis()->SetBinLabel(x, CbmLmvmHist::fAnaStepsLatex[step].c_str());
    grB->SetBinContent(x, yB);
    grS->SetBinContent(x, yS);
    x++;
  }

  grB->GetXaxis()->SetLabelSize(0.06);
  grB->GetXaxis()->SetRange(1, x - 1);
  grS->GetXaxis()->SetRange(1, x - 1);

  stringstream ss;
  ss << "lmvm_cut_eff_" << minMinv << "_" << maxMinv;
  fHM[0]->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 700, 700);
  DrawH1({grB, grS}, {"BG", "Signal"}, kLinear, kLinear, true, 0.75, 0.85, 1.0, 1.0);
  grS->SetLineWidth(4);
  grB->SetLineWidth(4);
  grB->SetMinimum(1);
  grB->SetMaximum(105);

  stringstream ss2;
  ss2 << minMinv << "<M [GeV/c^2]<" << maxMinv;
  TText* t = new TText(0.5, 110, ss2.str().c_str());
  t->Draw();
}

void CbmAnaDielectronTaskDrawAll::CalcCombBGHistos()
{
  int nofBins;
  double bW       = 0;
  Int_t nofEvents = 0;
  for (int i = 0; i < fNofSignals; i++) {
    if (!fDrawQgp && i == kQgp) continue;
    nofEvents += (int) H1(i, "fh_event_number")->GetEntries();
    cout << "CalcCombBGHistos: nofEvents = " << nofEvents << endl;
  }

  // first convert MeV into Bin for later normalization
  int nBinsOrig  = 4000;  // was set in CbmAnaDielectronTask.cxx
  int upperLimit = 4000;  // in MeV (assumed, that lower limit of histo = 0)

  int rFrom      = 300;  // lower and upper range to normalize (in Mev/c^2)
  int rTo        = 1000;
  double binFrom = (double) (1. * nBinsOrig / nRebin) * (1. * rFrom / upperLimit);
  double binTo   = (double) (1. * nBinsOrig / nRebin) * (1. * rTo / upperLimit);

  for (Int_t step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    fh_mean_combPairsPM_sameEvent_minv[step]->Rebin(nRebin);
    fh_mean_combPairsPP_sameEvent_minv[step]->Rebin(nRebin);
    fh_mean_combPairsMM_sameEvent_minv[step]->Rebin(nRebin);
    fh_mean_combPairsPM_mixedEvents_minv[step]->Rebin(nRebin);
    fh_mean_combPairsPP_mixedEvents_minv[step]->Rebin(nRebin);
    fh_mean_combPairsMM_mixedEvents_minv[step]->Rebin(nRebin);

    /*fh_mean_combPairsPM_sameEvent_minv_raw[step]->Rebin(nRebin);
    fh_mean_combPairsPP_sameEvent_minv_raw[step]->Rebin(nRebin);
    fh_mean_combPairsMM_sameEvent_minv_raw[step]->Rebin(nRebin);
    fh_mean_combPairsPM_mixedEvents_minv_raw[step]->Rebin(nRebin);
    fh_mean_combPairsPP_mixedEvents_minv_raw[step]->Rebin(nRebin);
    fh_mean_combPairsMM_mixedEvents_minv_raw[step]->Rebin(nRebin);*/

    //int nofBins = hBpp->GetNbinsX();  	TODO: test; delete
    //nofBinsRaw = fh_mean_combPairsPM_sameEvent_minv_raw[step]->GetNbinsX(); _RAW_
    nofBins = fh_mean_combPairsPM_sameEvent_minv[step]->GetNbinsX();  // MIND: 'nofBins' is used throughout this method!

    // calculate geom. mean of same events
    TH1D* hBpp = (TH1D*) fh_mean_combPairsPP_sameEvent_minv[step]->Clone();
    TH1D* hBmm = (TH1D*) fh_mean_combPairsMM_sameEvent_minv[step]->Clone();

    fh_mean_combBg_GeomMeanSame_minv[step] = (TH1D*) hBpp->Clone();

    for (int iBin = 1; iBin <= nofBins; iBin++) {
      double m1      = hBpp->GetBinContent(iBin);
      double m2      = hBmm->GetBinContent(iBin);
      double content = TMath::Sqrt(m1 * m2);
      fh_mean_combBg_GeomMeanSame_minv[step]->SetBinContent(iBin, content);
    }

    // calculate geom. mean of mixed events
    TH1D* hbpp = (TH1D*) fh_mean_combPairsPP_mixedEvents_minv[step]->Clone();
    TH1D* hbmm = (TH1D*) fh_mean_combPairsMM_mixedEvents_minv[step]->Clone();

    fh_mean_combBg_GeomMeanMixed_minv[step] = (TH1D*) hbpp->Clone();

    for (int iBin = 1; iBin <= nofBins; iBin++) {
      double m1      = hbpp->GetBinContent(iBin);
      double m2      = hbmm->GetBinContent(iBin);
      double content = 0;
      if (m1 == 0 && m2 != 0) content = m2;
      else if (m1 != 0 && m2 == 0)
        content = m1;
      else
        content = TMath::Sqrt(m1 * m2);
      fh_mean_combBg_GeomMeanMixed_minv[step]->SetBinContent(iBin, content);
    }

    // normalization factor for same/mixed geom. mean
    double intSameGM  = fh_mean_combBg_GeomMeanSame_minv[step]->Integral(binFrom, binTo);
    double intMixedGM = fh_mean_combBg_GeomMeanMixed_minv[step]->Integral(binFrom, binTo);
    double normGM     = (double) intSameGM / intMixedGM;
    cout << "step " << step << ": normGM = " << normGM << endl;

    // calculate k factor
    TH1D* k = (TH1D*) fh_mean_combPairsPM_mixedEvents_minv[step]->Clone();
    k->Divide(fh_mean_combBg_GeomMeanMixed_minv[step]);
    k->Scale(1. / 2);
    fh_mean_combBg_k_minv[step] = (TH1D*) k->Clone();

    // calculate combinatorial BG
    TH1D* Bc = (TH1D*) k->Clone();
    Bc->Multiply(fh_mean_combBg_GeomMeanSame_minv[step]);
    Bc->Scale(2.);
    fh_mean_combBg_minv[step]     = (TH1D*) Bc->Clone();
    fh_mean_combBg_raw_minv[step] = (TH1D*) Bc->Clone();
    fh_mean_combBg_raw_minv[step]->Scale(fNofSignals * nofEvents);

    // calculate assembled combinatorial BG from same (<= 0.3 GeV) and mixed (> 0.3 GeV) events data
    fh_mean_combBg_assemb_minv[step] = (TH1D*) fh_mean_combBg_minv[step]->Clone();

    for (int iBin = binFrom + 1; iBin <= nofBins; iBin++) {  // from > 300 MeV on normalized data from mixed event
      double k2      = fh_mean_combBg_k_minv[step]->GetBinContent(iBin);
      double gm2     = fh_mean_combBg_GeomMeanMixed_minv[step]->GetBinContent(iBin);
      double content = 2 * k2 * gm2 * normGM;
      fh_mean_combBg_assemb_minv[step]->SetBinContent(iBin, content);
    }

    // calculate comb. signal
    // from 'N+-same'
    fh_mean_combSignalNpm_minv[step] = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[step]->Clone();
    fh_mean_combSignalNpm_minv[step]->Add(fh_mean_combBg_minv[step], -1.);


    // from 'Cocktail + BG'
    TH1D* cock;
    if (step == kMc) cock = (TH1D*) GetCoctailMinv(kMc);
    else if (step == kAcc)
      cock = (TH1D*) GetCoctailMinv(kAcc);
    else if (step == kReco)
      cock = (TH1D*) GetCoctailMinv(kReco);
    else if (step == kChi2Prim)
      cock = (TH1D*) GetCoctailMinv(kChi2Prim);
    else if (step == kElId)
      cock = (TH1D*) GetCoctailMinv(kElId);
    else if (step == kGammaCut)
      cock = (TH1D*) GetCoctailMinv(kGammaCut);
    else if (step == kMvd1Cut)
      cock = (TH1D*) GetCoctailMinv(kMvd1Cut);
    else if (step == kMvd2Cut)
      cock = (TH1D*) GetCoctailMinv(kMvd2Cut);
    else if (step == kStCut)
      cock = (TH1D*) GetCoctailMinv(kStCut);
    else if (step == kRtCut)
      cock = (TH1D*) GetCoctailMinv(kRtCut);
    else if (step == kTtCut)
      cock = (TH1D*) GetCoctailMinv(kTtCut);
    else if (step == kPtCut)
      cock = (TH1D*) GetCoctailMinv(kPtCut);

    if (cock != NULL) cock->Rebin(nRebin);
    fh_mean_combSignalNpm_minv[step] = (TH1D*) fh_mean_bg_minv[step]->Clone();
    fh_mean_combSignalNpm_minv[step]->Add(cock);
    fh_mean_combSignalNpm_minv[step]->Add(fh_mean_combBg_minv[step], -1.);

    // calculate assembled comb. signal from same (<= 0.3 GeV) and mixed (> 0.3 GeV) events data
    // from 'N+-same'
    fh_mean_combSignalNpm_assemb_minv[step] = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[step]->Clone();
    fh_mean_combSignalNpm_assemb_minv[step]->Add(fh_mean_combBg_assemb_minv[step], -1.);
    fh_mean_combSignalNpm_assemb_minv[step]->SetTitle("Signal (N^{+-}_{same} - B_{C})");

    // from 'Cocktail + BG'
    fh_mean_combSignalBCoc_assemb_minv[step] = (TH1D*) fh_mean_bg_minv[step]->Clone();
    fh_mean_combSignalBCoc_assemb_minv[step]->Add(cock);
    TH1D* cbAss = (TH1D*) fh_mean_combBg_assemb_minv[step]->Clone();
    cbAss->Scale(1. / nofEvents);
    fh_mean_combSignalBCoc_assemb_minv[step]->Add(cbAss, -1.);

    // error calculation
    TH1D* err = (TH1D*) fh_mean_combPairsPM_sameEvent_minv[step]->Clone();
    err->Reset("ice");
    fh_mean_combBg_errProp_minv[step]     = (TH1D*) err->Clone();
    fh_mean_combSignal_errProp_minv[step] = (TH1D*) err->Clone();

    for (int iBin = 1; iBin <= nofBins; iBin++) {  // calculate error propagation
      double Npm = fh_mean_combPairsPM_sameEvent_minv[step]->GetBinContent(iBin);
      double Bpp = fh_mean_combPairsPP_sameEvent_minv[step]->GetBinContent(iBin);
      double Bmm = fh_mean_combPairsMM_sameEvent_minv[step]->GetBinContent(iBin);
      double bpm = fh_mean_combPairsPM_mixedEvents_minv[step]->GetBinContent(iBin);
      double bpp = fh_mean_combPairsPP_mixedEvents_minv[step]->GetBinContent(iBin);
      double bmm = fh_mean_combPairsMM_mixedEvents_minv[step]->GetBinContent(iBin);

      // calculation of error propagation
      double DNpm = TMath::Sqrt(Npm);  // Δ<B+->
      double DBpp = TMath::Sqrt(Bpp);  // Δ<B++>
      double DBmm = TMath::Sqrt(Bmm);  // Δ<B-->
      double Dbpm = TMath::Sqrt(bpm);  // Δ<b+->
      double Dbpp = TMath::Sqrt(bpp);  // Δ<b++>
      double Dbmm = TMath::Sqrt(bmm);  // Δ<b-->

      double dNpm = (double) 1.;  // derivatives of B_c and signal resp. to single contributions
      double dBpp = (double) (1. / 2) * bpm * Bmm * (1. / TMath::Sqrt(Bpp * Bmm * bpp * bmm));
      double dBmm = (double) (1. / 2) * bpm * Bpp * (1. / TMath::Sqrt(Bpp * Bmm * bpp * bmm));
      double dbpm = (double) TMath::Sqrt((Bpp * Bmm) / (bpp * bmm));
      double dbpp = (double) ((-1.) / 2) * bpm * bmm * TMath::Sqrt(Bpp * Bmm)
                    / (TMath::Sqrt(bpp * bmm) * TMath::Sqrt(bpp * bmm) * TMath::Sqrt(bpp * bmm));
      double dbmm = (double) ((-1.) / 2) * bpm * bpp * TMath::Sqrt(Bpp * Bmm)
                    / (TMath::Sqrt(bpp * bmm) * TMath::Sqrt(bpp * bmm) * TMath::Sqrt(bpp * bmm));
      double dbpm2 = (double) normGM;  // for > 300 MeV

      double fNpm  = (DNpm * dNpm) * (DNpm * dNpm);
      double fBpp  = (DBpp * dBpp) * (DBpp * dBpp);  // single contribution factors to error propagation
      double fBmm  = (DBmm * dBmm) * (DBmm * dBmm);
      double fbpm  = (Dbpm * dbpm) * (Dbpm * dbpm);
      double fbpp  = (Dbpp * dbpp) * (Dbpp * dbpp);  // for > 300 MeV
      double fbmm  = (Dbmm * dbmm) * (Dbmm * dbmm);
      double fbpm2 = (Dbpm * dbpm2) * (Dbpm * dbpm2);  // for > 300 MeV

      double errorBc  = TMath::Sqrt(fBpp + fBmm + fbpm + fbpp + fbmm);  // final error propagation value
      double errorBc2 = TMath::Sqrt(fbpm2);

      double errorSig  = TMath::Sqrt(fNpm + fBpp + fBmm + fbpm + fbpp + fbmm);
      double errorSig2 = TMath::Sqrt(fNpm + fbpm2);

      // fill error histograms (histos with error information only)
      if (iBin <= binFrom) fh_mean_combBg_errProp_minv[step]->SetBinContent(iBin, errorBc);
      if (iBin > binFrom) fh_mean_combBg_errProp_minv[step]->SetBinContent(iBin, errorBc2);
      if (iBin <= binFrom) fh_mean_combSignal_errProp_minv[step]->SetBinContent(iBin, errorSig);
      if (iBin > binFrom) fh_mean_combSignal_errProp_minv[step]->SetBinContent(iBin, errorSig2);

      // set error value in CB histograms
      fh_mean_combBg_minv[step]->SetBinError(iBin, errorBc);
      if (iBin <= binFrom) fh_mean_combBg_assemb_minv[step]->SetBinError(iBin, errorBc);
      if (iBin > binFrom) fh_mean_combBg_assemb_minv[step]->SetBinError(iBin, errorBc2);
      if (iBin <= binFrom) fh_mean_combSignalNpm_assemb_minv[step]->SetBinError(iBin, errorSig);
      if (iBin > binFrom) fh_mean_combSignalNpm_assemb_minv[step]->SetBinError(iBin, errorSig2);

      if (iBin == 3 || iBin == 4 || ((iBin % (500 / nRebin) == 0 && iBin <= (2000 / nRebin)))) {
        cout << "step    = " << step << endl;
        cout << "iBin    = " << iBin << endl;
        cout << "Npm     = " << Npm << endl;
        cout << "bpm     = " << bpm << endl;
        cout << "bp      = " << bpp << endl;
        cout << "bm      = " << bmm << endl;
        cout << "Bp      = " << Bpp << endl;
        cout << "Bm      = " << Bmm << endl;
        cout << "Dbpm    = " << Dbpm << endl;
        cout << "Dbp     = " << Dbpp << endl;
        cout << "Dbm     = " << Dbmm << endl;
        cout << "DBp     = " << DBpp << endl;
        cout << "DBm     = " << DBmm << endl;
        cout << "dbpm    = " << dbpm << endl;
        cout << "dbp     = " << dbpp << endl;
        cout << "dbm     = " << dbmm << endl;
        cout << "dBp     = " << dBpp << endl;
        cout << "dBm     = " << dBmm << endl;
        cout << "fbpm    = " << fbpm << endl;
        cout << "fbp     = " << fbpp << endl;
        cout << "fbm     = " << fbmm << endl;
        cout << "fBp     = " << fBpp << endl;
        cout << "fBm     = " << fBmm << endl;
        cout << "errPropBc  = " << errorBc << endl;
        cout << "errPropBc2 = " << errorBc2 << endl;
        cout << "errorSig   = " << errorSig << endl;
        cout << "errorSig2  = " << errorSig2 << endl;
        cout << "Bin error CB     = " << fh_mean_combBg_errProp_minv[step]->GetBinContent(iBin) << endl;
        cout << "Bin error Signal = " << fh_mean_combSignal_errProp_minv[step]->GetBinContent(iBin) << endl;
        cout << "Bin error Npm    = " << fh_mean_combPairsPM_sameEvent_minv[step]->GetBinError(iBin) << endl;
      }
    }  // error propagation

    // scale histograms
    bW = fh_mean_combPairsPM_sameEvent_minv[step]->GetBinWidth(1);
    cout << "bW = " << bW << endl;
    fh_mean_combPairsPM_sameEvent_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combPairsPP_sameEvent_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combPairsMM_sameEvent_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combPairsPM_mixedEvents_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combPairsPP_mixedEvents_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combPairsMM_mixedEvents_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combBg_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combBg_assemb_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combBg_GeomMeanSame_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combBg_GeomMeanMixed_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combSignalNpm_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combSignalNpm_assemb_minv[step]->Scale(1. / (nofEvents * bW));
    fh_mean_combSignalBCoc_assemb_minv[step]->Scale(1. / (bW));  // had been normalized earlier due to Cocktail

    for (int iBin = 1; iBin <= nofBins; iBin++) {  // TODO: only to check error value after sacling
      if (iBin == 3 || iBin == 4 || (iBin % (500 / nRebin) == 0 && iBin <= (2000 / nRebin))) {
        double Npm = fh_mean_combPairsPM_sameEvent_minv[step]->GetBinContent(iBin);
        double Bm  = fh_mean_combPairsMM_sameEvent_minv[step]->GetBinContent(iBin);
        cout << "CHECK: step    = " << step << endl;
        cout << "CHECK: iBin    = " << iBin << endl;
        cout << "CHECK: Npm = " << Npm * nofEvents * bW << endl;
        cout << "CHECK: Bm  = " << Bm * nofEvents * bW << endl;
        cout << "CHECK: Bin error Npm              = " << fh_mean_combPairsPM_sameEvent_minv[step]->GetBinError(iBin)
             << endl;
        cout << "CHECK: Bin error Sig_ass (Npm)    = " << fh_mean_combSignalNpm_assemb_minv[step]->GetBinError(iBin)
             << endl;
      }
    }
  }  // steps
}

/*void CbmAnaDielectronTaskDrawAll::CompareSTSversions()
{
  TFile* _Input_file_v16g = TFile::Open("/lustre/cbm/users/criesen/cbm/data/lmvm/inmed/analysis.all.root");
  TFile* _Input_file_v19a = TFile::Open("/lustre/cbm/users/criesen/cbm/data/lmvm_sts-v19a_5M/inmed/analysis.all.root");

  TH1D* hv16g = (TH1D*) _Input_file_v16g->Get("fh_signal_minv_mc");
  TH1D* hv19a = (TH1D*) _Input_file_v19a->Get("fh_signal_minv_mc");
  
  hv16g->Divide(hv19a);

  hv16g->Rebin(nRebin);
  hv16g->Scale(1. / nRebin);

  hv16g->GetXaxis()->SetRangeUser(0, 2.);
  hv16g->GetXaxis()->SetTitle("M_{ee} [GeV/c^{2}]");

  hv16g->SetTitle(0);

  DrawH1({hv16g}, {""}, kLinear, kLog, false, 0.8, 0.8, 0.95, 0.95, "HIST L");
  hv16g->GetYaxis()->SetTitle("ratio v16g/v19a");
  hv16g->GetYaxis()->SetLabelSize(0.05);

  hv16g->Draw();
}*/

TH1D* CbmAnaDielectronTaskDrawAll::SBgRange(Double_t min, Double_t max)
{
  stringstream ss;
  ss << "lmvm_s_bg_region_" << min << "_" << max;
  TH1D* h_s_bg = new TH1D(ss.str().c_str(), string(ss.str() + ";Analysis steps;S/BG").c_str(),
                          CbmLmvmHist::fNofAnaSteps, 0, CbmLmvmHist::fNofAnaSteps);
  h_s_bg->GetXaxis()->SetLabelSize(0.06);
  int x = 1;
  for (int step = kElId; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) { continue; }
    Int_t bin1 = fh_sum_s_minv[step]->FindBin(min);
    Int_t bin2 = fh_sum_s_minv[step]->FindBin(max);
    double y   = fh_sum_s_minv[step]->Integral(bin1, bin2) / fh_mean_bg_minv[step]->Integral(bin1, bin2);

    h_s_bg->GetXaxis()->SetBinLabel(x, CbmLmvmHist::fAnaStepsLatex[step].c_str());
    h_s_bg->SetBinContent(x, y);
    // replace "." with "_"
    string str = ss.str();
    for (string::iterator it = str.begin(); it < str.end(); it++) {
      if (*it == '.') *it = '_';
    }
    x++;
  }
  h_s_bg->GetXaxis()->SetRange(1, x - 1);
  return h_s_bg;
}

void CbmAnaDielectronTaskDrawAll::SBgRangeAll()
{
  TH1D* h_00_02 = SBgRange(0.0, 0.2);
  TH1D* h_02_06 = SBgRange(0.2, 0.6);
  TH1D* h_06_12 = SBgRange(0.6, 1.2);

  fHM[0]->CreateCanvas("lmvm_s_bg_ranges", "lmvm_s_bg_ranges", 700, 700);
  DrawH1({h_00_02, h_02_06, h_06_12}, {"0.0<M [GeV/c^{2}]<0.2", "0.2<M [GeV/c^{2}]<0.6", "0.6<M [GeV/c^{2}]<1.2"},
         kLinear, kLog, true, 0.25, 0.8, 0.75, 0.99);

  h_00_02->SetMinimum(1e-3);
  h_00_02->SetMaximum(3);
  h_00_02->SetLineWidth(4);
  h_02_06->SetLineWidth(4);
  h_06_12->SetLineWidth(4);

  TH1D* h_05_06 = SBgRange(0.5, 0.6);
  fHM[0]->CreateCanvas("lmvm_s_bg_ranges_05_06", "lmvm_s_bg_ranges_05_06", 700, 700);
  DrawH1(h_05_06, kLinear, kLinear);
  h_05_06->SetMinimum(1e-3);
  h_05_06->SetMaximum(2e-2);
  h_05_06->SetLineWidth(4);
}

void CbmAnaDielectronTaskDrawAll::DrawSBgSignals()
{
  //   Double_t y[CbmLmvmHist::fNofAnaSteps];
  TCanvas* cFit       = fHM[0]->CreateCanvas("lmvm_signal_fit", "lmvm_signal_fit", 600, 600);
  TCanvas* cDashboard = fHM[0]->CreateCanvas("lmvm_dashboard", "lmvm_dashboard", 1000, 900);
  int iDash           = 2;
  TLatex* latex       = new TLatex();
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.05, 0.95, "signal");
  latex->DrawLatex(0.2, 0.95, "step");
  latex->DrawLatex(0.4, 0.95, "eff, %");
  latex->DrawLatex(0.55, 0.95, "S/BG");
  latex->DrawLatex(0.7, 0.95, "mean");
  latex->DrawLatex(0.85, 0.95, "sigma");
  TString str;
  for (int iF = 0; iF < fNofSignals - 1; iF++) {
    if (!fDrawQgp && iF == kQgp) continue;
    string signalName = CbmLmvmHist::fSignalNames[iF];
    cout << "Signal: " << signalName << endl;
    stringstream ss;
    ss << "lmvm_s_bg_cuts_" << signalName;

    TH1D* h_s_bg = new TH1D(ss.str().c_str(), string(ss.str() + ";Analysis steps;S/BG").c_str(),
                            CbmLmvmHist::fNofAnaSteps, 0, CbmLmvmHist::fNofAnaSteps);
    h_s_bg->GetXaxis()->SetLabelSize(0.06);
    h_s_bg->SetLineWidth(4);
    int x = 1;
    iDash++;  // empty string after each signal
    for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
      if (step < kElId) continue;
      if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) { continue; }

      TH1D* s         = (TH1D*) H1(iF, "fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step])->Clone();
      TH1D* bg = (TH1D*) fh_mean_bg_minv[step]->Clone();  // TODO: not better to take BG of PP intead of mean BG??
      Int_t nofEvents = (int) H1(iF, "fh_event_number")->GetEntries();
      s->Scale(1. / nofEvents);
      cFit->cd();
      if (iF == kPhi) {
        if (s->GetEntries() > 0) s->Fit("gaus", "Q", "", 0.95, 1.05);
      }
      else if (iF == kOmega) {
        if (s->GetEntries() > 0) s->Fit("gaus", "Q", "", 0.69, 0.81);
      }
      else {
        if (s->GetEntries() > 0) s->Fit("gaus", "Q");
      }

      TF1* func      = s->GetFunction("gaus");
      Double_t mean  = (func != NULL) ? func->GetParameter("Mean") : 0.;
      Double_t sigma = (func != NULL) ? func->GetParameter("Sigma") : 0.;
      Int_t minInd   = s->FindBin(mean - 2. * sigma);
      Int_t maxInd   = s->FindBin(mean + 2. * sigma);

      Double_t sumSignal = 0.;
      Double_t sumBg     = 0.;
      for (Int_t i = minInd + 1; i <= maxInd - 1; i++) {
        sumSignal += s->GetBinContent(i);
        sumBg += bg->GetBinContent(i);
      }
      Double_t sbg = sumSignal / sumBg;
      double eff   = 100. * H1(iF, "fh_signal_pty_" + CbmLmvmHist::fAnaSteps[step])->GetEntries()
                   / H1(iF, "fh_signal_pty_" + CbmLmvmHist::fAnaSteps[kMc])->GetEntries();

      bool isOmegaOrPhi = (iF == kPhi || iF == kOmega);
      cDashboard->cd();
      latex->DrawLatex(0.05, 1.0 - iDash * 0.033, signalName.c_str());
      latex->DrawLatex(0.2, 1.0 - iDash * 0.033, CbmLmvmHist::fAnaSteps[step].c_str());
      str.Form("%.2f", eff);
      latex->DrawLatex(0.4, 1.0 - iDash * 0.033, str.Data());
      str.Form("%.3f", sumSignal / sumBg);
      latex->DrawLatex(0.55, 1.0 - iDash * 0.033, (isOmegaOrPhi) ? str.Data() : "-");
      str.Form("%.1f", 1000. * mean);
      latex->DrawLatex(0.7, 1.0 - iDash * 0.033, (isOmegaOrPhi) ? str.Data() : "-");
      str.Form("%.1f", 1000. * sigma);
      latex->DrawLatex(0.85, 1.0 - iDash * 0.033, (isOmegaOrPhi) ? str.Data() : "-");

      h_s_bg->GetXaxis()->SetBinLabel(x, CbmLmvmHist::fAnaStepsLatex[step].c_str());
      if (sbg < 1000.) h_s_bg->SetBinContent(x, sbg);
      x++;
      iDash++;
    }
    h_s_bg->GetXaxis()->SetRange(1, x - 1);
    fHM[0]->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
    DrawH1(h_s_bg);
    h_s_bg->SetLineWidth(4);

    cDashboard->Draw();
  }
}

void CbmAnaDielectronTaskDrawAll::SaveCanvasToImage()
{
  cout << "Images output dir:" << fOutputDir << endl;
  fHM[0]->SaveCanvasToImage(fOutputDir, "png");  // fHM[0]->SaveCanvasToImage(fOutputDir, "eps;png");
}

ClassImp(CbmAnaDielectronTaskDrawAll);
