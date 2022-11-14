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
#include "TGraph.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TText.h"
#include <TLegend.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "LmvmDef.h"
#include "LmvmUtils.h"

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
  DrawCutEffSignal();
  DrawPionSuppression();
  CalcCombBGHistos();
  SBgRangeAll();
  DrawSBgResults();
  DrawMinvAll();
  DrawMinvCombBgAndSignal();
  DrawMinvOfficialStyle();
  DrawMinvPtAll();
  DrawMinvBgSourcesAll();
  DrawSBgVsMinv();
  InvestigateMisid();  // TODO: move some procedures in here into seperate methods?
  DrawBetaMomSpectra();
  DrawMomRecoPrecision();
  DrawMomPluto();
  DrawPurity();
  DrawSignificancesAll();
  DrawTofM2();
  DrawGTrackVertex();
  DrawTofHitXY();
  DrawMinvScaleValues();
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

  // Global Track Loop
  for (const string& ptcl : fHMean.fGTrackNames) {
    CreateMeanHist<TH1D>("hMom_gTracks_" + ptcl, nofEvents);
    CreateMeanHist<TH1D>("hPtY_gTracks_" + ptcl, nofEvents);
    CreateMeanHist<TH1D>("hTofM2_gTracks_" + ptcl, nofEvents);
    /*CreateMeanHist<TH2D>("hIndexStsRich_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hIndexStsTrd_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hIndexStsTof_" + ptcl, nofEvents);*/
    CreateMeanHist<TH2D>("hRichRingTrackDist_gTracks_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hMatchId_gTracks_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hTofM2Calc_gTracks_" + ptcl, nofEvents);

    for (const string& cat : {"trueid", "misid", "truematch", "mismatch"}) {
      CreateMeanHist<TH2D>("hTofHitXY_" + cat + "_" + ptcl, nofEvents);
      CreateMeanHist<TH2D>("hTofPointXY_" + cat + "_" + ptcl, nofEvents);
      CreateMeanHist<TH1D>("hTofHitPointDist_" + cat + "_" + ptcl, nofEvents);
      CreateMeanHist<TH2D>("hTofTimeVsMom_gTracks_" + cat + "_" + ptcl, nofEvents);
      CreateMeanHist<TH2D>("hTofHitTrackDist_gTracks_" + cat + "_" + ptcl, nofEvents);
    }

    for (const string& cat : {"all", "complete"}) {
      CreateMeanHist<TH1D>("hNofMismatches_gTracks_" + cat + "_" + ptcl, nofEvents);
      CreateMeanHist<TH1D>("hNofMismatchedTrackSegments_" + cat + "_" + ptcl, nofEvents);
      for (const string& match : {"truematch", "mismatch"}) {
        for (const string& det : {"rich", "trd", "tof"}) {
          CreateMeanHist<TH1D>("hChi2_" + match + "_" + cat + "_" + det + "_" + ptcl, nofEvents);
        }
      }
    }
  }  // fGTrackNames

  // Candidate Loop
  for (const string& ptcl : fHMean.fCandNames) {
    CreateMeanHist<TH2D>("hTofPileHitXY_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hTofPilePointXY_" + ptcl, nofEvents);
    CreateMeanHist<TH1D>("hTofPileHitPointDist_" + ptcl, nofEvents);
    CreateMeanHist<TH1D>("hTofPilePty_cands_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hTofTimeVsMom_cands_" + ptcl, nofEvents);
    CreateMeanHist<TH2D>("hTofHitTrackDist_cands_" + ptcl, nofEvents);

    for (auto step : fHMean.fAnaSteps) {
      CreateMeanHist<TH1D>(fHMean.GetName("hMom_cands_" + ptcl, step), nofEvents);
      CreateMeanHist<TH1D>(fHMean.GetName("hMomRatio_cands_" + ptcl, step), nofEvents);
      CreateMeanHist<TH1D>(fHMean.GetName("hMomRatioVsMom_cands_" + ptcl, step), 1);
      CreateMeanHist<TH2D>(fHMean.GetName("hPtY_cands_" + ptcl, step), nofEvents);
      CreateMeanHist<TH2D>(fHMean.GetName("hTofM2_cands_" + ptcl, step), nofEvents);
      CreateMeanHist<TH2D>(fHMean.GetName("hRichRingTrackDist_cands_" + ptcl, step), nofEvents);
    }  // steps

    for (const string& cat : {"gTracks", "cands"}) {
      CreateMeanHist<TH1D>("hBetaMom_" + cat + "_" + ptcl, nofEvents);
    }


    for (const string det : {"sts", "rich", "trd", "tof"}) {
      CreateMeanHist<TH2D>("hChi2VsMom_" + det + "_" + ptcl, nofEvents);
      CreateMeanHist<TH2D>("hTofTimeVsChi2_" + det + "_" + ptcl, nofEvents);
    }

    for (const string det : {"StsRich", "StsTrd", "RichTrd"}) {
      CreateMeanHist<TH2D>("hChi2Comb_" + det + "_" + ptcl, nofEvents);
    }


  }  // fCandNames

  // Step Loop
  for (auto step : fHMean.fAnaSteps) {
    for (auto src : {ELmvmSrc::Bg, ELmvmSrc::Eta, ELmvmSrc::Pi0}) {
      CreateMeanHist<TH1D>(fHMean.GetName("hMinv", src, step), nofEvents, fRebMinv);
      CreateMeanHist<TH1D>(fHMean.GetName("hMinv_urqmdAll", src, step), nofEvents, fRebMinv);
      CreateMeanHist<TH1D>(fHMean.GetName("hMinv_urqmdEl", src, step), nofEvents, fRebMinv);
      CreateMeanHist<TH2D>(fHMean.GetName("hMinvPt", src, step), nofEvents);
    }

    for (const string& comb : {"PM", "PP", "MM"}) {
      for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
        for (const string& ev : {"_sameEv", "_mixedEv"}) {
          CreateMeanHist<TH1D>(fHMean.GetName("hMinvComb" + comb + cat + ev, step), nofEvents, fRebMinv);
        }
      }
    }

    CreateMeanHist<TH1D>(fHMean.GetName("hPionSupp_idEl", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hPionSupp_pi", step), nofEvents);
    CreateMeanHist<TH2D>(fHMean.GetName("hCandPdgVsMom", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hTofPilePdgs_cands", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hVertexXZ_misidTof", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hVertexYZ_misidTof", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hVertexXY_misidTof", step), nofEvents);
    CreateMeanHist<TH1D>(fHMean.GetName("hVertexRZ_misidTof", step), nofEvents);
  }  // steps

  string hBgSrc = "hMinvBgSource2_elid_";
  for (const string& pair : {"gg", "pipi", "pi0pi0", "oo", "gpi", "gpi0", "go", "pipi0", "pio", "pi0o"}) {
    string hPairFull = hBgSrc + pair;
    CreateMeanHist<TH1D>(hPairFull, nofEvents, fRebMinv);
  }

  for (const string& cat : {"true", "misid"}) {
    for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {
      CreateMeanHist<TH1D>("hMom_" + fHMean.fCandNames[iP] + "_" + cat, nofEvents);
      CreateMeanHist<TH1D>("hPtY_" + fHMean.fCandNames[iP] + "_" + cat, nofEvents);
      CreateMeanHist<TH1D>("hTofM2_" + fHMean.fCandNames[iP] + "_" + cat, nofEvents);
    }
  }

  for (const string& det : {"rich", "trd", "tof"}) {
    for (const string& cat : {"all", "complete"}) {
      CreateMeanHist<TH2D>("hPdgVsMom_gTracks_" + det + "_" + cat, nofEvents);
    }
  }

  for (const string& cat : {"El", "Bg"}) {
    CreateMeanHist<TH2D>("hAnnRichVsMomPur_" + cat, nofEvents);
    CreateMeanHist<TH2D>("hTrdElLikePur_" + cat, nofEvents);
  }

  CreateMeanHist<TH1D>("hBetaMom_allGTracks", nofEvents);
  CreateMeanHist<TH1D>("hTofPilePdgs_gTracks", nofEvents);
  CreateMeanHist<TH2D>("hVertexGTrackRZ", nofEvents);
}

TH1D* LmvmDrawAll::GetCocktailMinvH1(const string& name, ELmvmAnaStep step)
{
  return GetCocktailMinv<TH1D>(name, step);
}

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

void LmvmDrawAll::DrawMinvScaleValues()
{
  TH1D* inmed = new TH1D("inmed", "inmed; M_{ee} [GeV/c^{2}]; Scale Value", 3400., 0., 3.4);
  TH1D* qgp   = new TH1D("qgp", "qgp; M_{ee} [GeV/c^{2}]; Scale Value", 3400., 0., 3.4);
  for (int iB = 1; iB <= 3400; iB++) {
    double binCenter = inmed->GetBinCenter(iB);
    double sInmed    = LmvmUtils::GetMassScaleInmed(binCenter);
    double sQgp      = LmvmUtils::GetMassScaleQgp(binCenter);
    inmed->SetBinContent(iB, sInmed);
    qgp->SetBinContent(iB, sQgp);
  }
  fHMean.fHM.CreateCanvas("MinvScaleValues", "MinvScaleValues", 2400, 800);
  DrawH1({inmed, qgp}, {"inmed", "QGP"}, kLinear, kLog, true, 0.7, 0.7, 0.9, 0.9, "hist");

  TH1D* inmed2 = (TH1D*) inmed->Clone();
  TH1D* qgp2   = (TH1D*) qgp->Clone();
  inmed2->GetXaxis()->SetRangeUser(1., 1.1);
  qgp2->GetXaxis()->SetRangeUser(1., 1.1);
  inmed2->GetYaxis()->SetRangeUser(3e-3, 5e-1);
  qgp2->GetYaxis()->SetRangeUser(3e-3, 5e-1);

  fHMean.fHM.CreateCanvas("MinvScaleValues2", "MinvScaleValues2", 2400, 800);
  DrawH1({inmed2, qgp2}, {"inmed", "QGP"}, kLinear, kLog, true, 0.7, 0.7, 0.9, 0.9, "p");
}

void LmvmDrawAll::DrawSignificance(TH2D* hEl, TH2D* hBg, const string& name, double minZ, double maxZ,
                                   const string& option)
{
  string hElProjName = name + "_yProjEl";
  string hBgProjName = name + "_yProjBg";
  TH1D* el           = hEl->ProjectionY(hElProjName.c_str(), 1, hEl->GetXaxis()->GetNbins(), "");
  TH1D* bg           = hBg->ProjectionY(hBgProjName.c_str(), 1, hBg->GetXaxis()->GetNbins(), "");
  TH2D* rat          = (TH2D*) hEl->Clone();
  rat->Divide(hBg);

  const string hName = name + "_sign";
  const string cName = "Significance/" + name;

  TH1D* sign = fHMean.CreateSignificanceH1(el, bg, hName.c_str(), option);
  TCanvas* c = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 1600);
  c->Divide(3, 2);
  c->cd(1);
  DrawH2(hEl, kLinear, kLinear, kLog, "colz");
  hEl->GetZaxis()->SetRangeUser(minZ, maxZ);
  DrawTextOnPad("Electrons", 0.25, 0.9, 0.75, 0.99);
  c->cd(2);
  DrawH2(hBg, kLinear, kLinear, kLog, "colz");
  hBg->GetZaxis()->SetRangeUser(minZ, maxZ);
  DrawTextOnPad("Other", 0.25, 0.9, 0.75, 0.99);
  c->cd(3);
  DrawH2(rat, kLinear, kLinear, kLog, "colz");
  DrawTextOnPad("Ratio Electrons/Other", 0.2, 0.9, 0.8, 0.99);
  c->cd(4);
  DrawH1({el, bg}, {"Electrons", "Other"}, kLinear, kLog, true, 0.75, 0.75, 0.91, 0.91);
  DrawTextOnPad("Yield / Event", 0.25, 0.9, 0.75, 0.99);
  c->cd(5);
  DrawH1(sign, kLinear, kLinear, "hist");
  double maxX = -999999.;
  double maxY = -999999.;
  for (int iB = 1; iB <= sign->GetNbinsX(); iB++) {
    if (option == "right" && sign->GetBinContent(iB) >= maxY) {
      maxX = sign->GetBinCenter(iB);
      maxY = sign->GetBinContent(iB);
    }
    if (option == "left" && sign->GetBinContent(iB) > maxY) {
      maxX = sign->GetBinCenter(iB);
      maxY = sign->GetBinContent(iB);
    }
  }
  DrawTextOnPad("max. at: " + Cbm::NumberToString(maxX, 1), 0.55, 0.2, 0.85, 0.5);
  DrawTextOnPad("Significance", 0.25, 0.9, 0.75, 0.99);
}

void LmvmDrawAll::
  DrawSignificancesAll()  // TODO: implement automatization to seperate electrons from not-electrons in gTracks and cands
{
  // RICH ANN and Electron Likelihood
  {
    for (const string& hist : {"hAnnRichVsMomPur", "hTrdElLikePur"}) {
      TH2D* el = fHMean.H2Clone(hist + "_El");
      TH2D* bg = fHMean.H2Clone(hist + "_Bg");
      DrawSignificance(el, bg, hist, 1e-9, 1e-3, "left");
    }
  }

  // X2 in TRD (filled after ElID step)
  {
    TH2D* el = fHMean.H2Clone("hChi2VsMom_trd_plutoEl+");
    el->Add(fHMean.H2("hChi2VsMom_trd_plutoEl-"));

    TH2D* bg = fHMean.H2Clone("hChi2VsMom_trd_pion+");
    for (size_t iP = 2; iP < fHMean.fCandNames.size(); iP++) {  // consider UrQMD-electrons as "BG"
      string ptcl   = fHMean.fCandNames[iP];
      string hName1 = "hChi2VsMom_trd_" + ptcl;
      bg->Add(fHMean.H2(hName1.c_str()));
    }

    DrawSignificance(el, bg, "hChi2VsMom_trd", 1e-9, 1e-3, "right");
  }

  // Time in ToF (filled after ElID step)
  {
    TH2D* el = fHMean.H2Clone("hTofTimeVsMom_cands_plutoEl+");
    TH2D* bg = fHMean.H2Clone("hTofTimeVsMom_cands_pion+");

    for (size_t iP = 1; iP < fHMean.fCandNames.size(); iP++) {
      string hName = "hTofTimeVsMom_cands_" + fHMean.fCandNames[iP];
      if (iP <= 3) el->Add(fHMean.H2(hName.c_str()));
      else if (iP == 4)
        continue;
      else
        bg->Add(fHMean.H2(hName.c_str()));
    }

    DrawSignificance(el, bg, "hTofTimeVsMom", 1e-9, 1e-2, "right");
  }

  // Hit-Track-Distance in ToF (filled after ElID step)
  {
    TH2D* el = fHMean.H2Clone("hTofHitTrackDist_cands_plutoEl+");
    TH2D* bg = fHMean.H2Clone("hTofHitTrackDist_cands_pion+");

    for (size_t iP = 1; iP < fHMean.fCandNames.size(); iP++) {
      string hName = "hTofHitTrackDist_cands_" + fHMean.fCandNames[iP];
      if (iP <= 1) el->Add(fHMean.H2(hName.c_str()));  // consider UrQMD-electrons as "BG"
      else if (iP == 4)
        continue;
      else
        bg->Add(fHMean.H2(hName.c_str()));
    }

    DrawSignificance(el, bg, "hTofHitTrackDist", 1e-9, 1e-2, "right");
  }
}

void LmvmDrawAll::DrawGTrackVertex()
{
  TCanvas* c = fHMean.fHM.CreateCanvas("Vertex/globalTrackRZ", "Vertex/globalTrackRZ", 1600, 800);
  c->Divide(2, 1);
  c->cd(1);
  TH2D* h = fHMean.H2Clone("hVertexGTrackRZ");
  h->GetYaxis()->SetRangeUser(0., 50);
  h->GetZaxis()->SetRangeUser(1e-7, 10);
  DrawH2(h, kLinear, kLinear, kLog, "colz");
  c->cd(2);
  TH2D* h2 = fHMean.H2Clone("hVertexGTrackRZ");
  h2->GetXaxis()->SetRangeUser(-49., -39.);
  h2->GetYaxis()->SetRangeUser(0., 10.);
  h2->GetZaxis()->SetRangeUser(1e-7, 10);
  DrawH2(h2, kLinear, kLinear, kLog, "colz");
}

void LmvmDrawAll::DrawTofHitXY()
{
  //double min = 1e-7;
  //double max = 2e-3;
  double minD = 1e-7;
  double maxD = 2e-2;

  /*TCanvas* c = fHMean.fHM.CreateCanvas("ToF/point-hit-dist_gTracks", "ToF/point-hit-dist_gTracks", 2400, 2400); // TODO: do these as loop for pile and not-pile histograms
  c->Divide(4, 4);
  int i = 1;
  for (const string& ptcl : fHMean.fGTrackNames) {
    c->cd(i++);
    string hName = "hTofHitPointDist_" + ptcl;
    TH1D* h = fHMean.H1Clone(hName.c_str());
    h->Fit("gaus", "Q", "", 0., 2.);
    h->GetYaxis()->SetRangeUser(minD, maxD);
    DrawH1(h, kLinear, kLog, "hist");
    DrawTextOnPad(fHMean.fGTrackLatex[i-2], 0.25, 0.9, 0.75, 0.999);
    TF1* func = h->GetFunction("gaus");
    double mean = (func != nullptr) ? func->GetParameter("Mean") : 0.;
    DrawTextOnPad("mean: " + Cbm::NumberToString(mean, 2), 0.2, 0.65, 0.6, 0.89);
  }

  for (const string& ptcl : fHMean.fGTrackNames) {
    TCanvas* c2 = fHMean.fHM.CreateCanvas("ToF/HitXY/" + ptcl, "ToF/HitXY/" + ptcl, 2400, 800);
    c2->Divide(3,1);
    c2->cd(1);
    TH2D* point = fHMean.H2Clone("hTofPointXY_" + ptcl);
    point->GetZaxis()->SetRangeUser(min, max);
    DrawH2(point, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF Points", 0.3, 0.9, 0.7, 0.99);
    c2->cd(2);
    TH2D* hit = fHMean.H2Clone("hTofHitXY_" + ptcl);
    hit->GetZaxis()->SetRangeUser(min, max);
    DrawH2(hit, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF Hits", 0.3, 0.9, 0.7, 0.99);
    c2->cd(3);
    TH1D* dist = fHMean.H1Clone("hTofHitPointDist_" + ptcl);
    dist->GetYaxis()->SetRangeUser(minD, maxD);
    dist->Fit("gaus", "Q", "", 0., 2.);
    DrawH1(dist, kLinear, kLog, "hist");
    DrawTextOnPad("Distance Hit-Point", 0.3, 0.9, 0.7, 0.99);
    TF1* func = dist->GetFunction("gaus");
    double mean = (func != nullptr) ? func->GetParameter("Mean") : 0.;
    DrawTextOnPad("mean: " + Cbm::NumberToString(mean, 1), 0.2, 0.65, 0.5, 0.89);
  }*/

  // draw only for particles in ToF pile that are identified as electrons in ToF
  double minH = 2e-6;
  double maxH = 1e-4;

  TCanvas* c3 = fHMean.fHM.CreateCanvas("ToF/HitXY/MisidsInTofPile/point-hit-dist_all",
                                        "ToF/HitXY/MisidsInTofPile/point-hit-dist_all", 2400, 1600);
  c3->Divide(3, 2);
  int iC = 1;
  for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {  // only draw for not-electrons
    c3->cd(iC++);
    string ptcl  = fHMean.fCandNames[iP];
    string hName = "hTofPileHitPointDist_" + ptcl;
    TH1D* h      = fHMean.H1Clone(hName.c_str());
    h->GetYaxis()->SetRangeUser(minD, maxD);
    h->Fit("gaus", "Q", "", 0., 2.);
    DrawH1(h, kLinear, kLog, "hist");
    DrawTextOnPad(fHMean.fCandLatex[iP], 0.4, 0.9, 0.54, 0.99);
    TF1* func   = h->GetFunction("gaus");
    double mean = (func != nullptr) ? func->GetParameter("Mean") : 0.;
    DrawTextOnPad("mean: " + Cbm::NumberToString(mean, 2), 0.2, 0.65, 0.6, 0.89);
  }

  for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {  // only draw for not-electrons
    string ptcl = fHMean.fCandNames[iP];
    TCanvas* c4 =
      fHMean.fHM.CreateCanvas("ToF/HitXY/MisidsInTofPile/" + ptcl, "ToF/HitXY/MisidsInTofPile/" + ptcl, 2400, 800);
    c4->Divide(3, 1);
    c4->cd(1);
    TH2D* hit = fHMean.H2Clone("hTofPileHitXY_" + ptcl);
    hit->GetZaxis()->SetRangeUser(minH, maxH);
    DrawH2(hit, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF Hits", 0.3, 0.9, 0.7, 0.99);
    c4->cd(2);
    TH2D* point = fHMean.H2Clone("hTofPilePointXY_" + ptcl);
    point->GetZaxis()->SetRangeUser(minH, maxH);
    DrawH2(point, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF Points", 0.3, 0.9, 0.7, 0.99);
    c4->cd(3);
    TH1D* dist = fHMean.H1Clone("hTofPileHitPointDist_" + ptcl);
    dist->Fit("gaus", "Q", "", 0., 2.);
    dist->GetYaxis()->SetRangeUser(minD, maxD);
    DrawH1(dist, kLinear, kLog, "hist");
    DrawTextOnPad("Distance Hit-Point", 0.3, 0.9, 0.7, 0.99);
    TF1* func   = dist->GetFunction("gaus");
    double mean = (func != nullptr) ? func->GetParameter("Mean") : 0.;
    DrawTextOnPad("mean: " + Cbm::NumberToString(mean, 1), 0.2, 0.65, 0.5, 0.89);
  }
}

void LmvmDrawAll::DrawBetaMomSpectra()
{
  double min = 1e-5;
  double max = 10;
  for (const string& cat : {"gTracks", "cands"}) {
    for (size_t i = 0; i < fHMean.fCandNames.size(); i++) {
      string cName = "BetaMom/" + cat + "/" + fHMean.fCandNames[i];
      string hName = "hBetaMom_" + cat + "_" + fHMean.fCandNames[i];
      fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 800, 800);
      TH2D* h = fHMean.H2Clone(hName.c_str());
      h->GetZaxis()->SetRangeUser(min, max);
      DrawH2(h, kLinear, kLinear, kLog, "colz");
      DrawTextOnPad((fHMean.fCandLatex[i]).c_str(), 0.25, 0.9, 0.75, 0.999);
    }
  }
  fHMean.fHM.CreateCanvas("BetaMom/gTracks/all", "BetaMom/gTracks/all", 800, 800);
  TH2D* h = fHMean.H2Clone("hBetaMom_allGTracks");
  h->GetZaxis()->SetRangeUser(min, max);
  DrawH2(h, kLinear, kLinear, kLog, "colz");
}

void LmvmDrawAll::DrawMomPluto()
{
  for (const string& pi : {"Px", "Py", "Pz"}) {
    string cName = "MomDistPluto/" + pi;
    TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 1600);
    c->Divide(3, 2);
    int i = 1;
    for (ELmvmSignal signal : fHMean.fSignals) {
      c->cd(i++);
      string hName = "hMom" + pi;
      TH1D* ep     = H(signal)->H1Clone((hName + "+_signal_mc").c_str());
      TH1D* em     = H(signal)->H1Clone((hName + "-_signal_mc").c_str());
      double bW    = ep->GetBinWidth(1);
      ep->Scale(1. / (H(signal)->H1("hEventNumber")->GetEntries() * bW));
      em->Scale(1. / (H(signal)->H1("hEventNumber")->GetEntries() * bW));
      ep->GetYaxis()->SetTitle("1/N dN/dP [GeV/c]^{-1}");
      DrawH1({ep, em}, {"e^{+}", "e^{-}"}, kLinear, kLog, true, 0.75, 0.75, 0.91, 0.91);
      DrawTextOnPad(fHMean.fSignalNames[static_cast<int>(signal)].c_str(), 0.23, 0.8, 0.48, 0.89);
    }
  }
}

void LmvmDrawAll::DrawMomentum()
{
  // draw momentum of all global tracks
  fHMean.DrawAllGTracks(1, "hMom/gTracks", "hMom_gTracks", {""}, {""}, 1e-7, 10.);

  // draw momentum for misidentified and not-misidentified particles (from global tracks)
  {
    double min = 5e-8;
    double max = 10;
    TCanvas* c = fHMean.fHM.CreateCanvas("hMom/misid_vs_True", "hMom/misid_vs_True", 2400, 1600);
    c->Divide(3, 2);
    int iC = 1;
    for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {  // only draw for not-electrons
      c->cd(iC++);
      string ptcl = fHMean.fCandNames[iP];
      TH1D* hMis  = fHMean.H1Clone("hMom_" + ptcl + "_misid");
      TH1D* hTrue = fHMean.H1Clone("hMom_" + ptcl + "_true");
      hMis->GetYaxis()->SetRangeUser(min, max);
      hTrue->GetYaxis()->SetRangeUser(min, max);
      DrawH1({hTrue, hMis}, {"not misidentified", "misidentified as electron"}, kLinear, kLog, true, 0.55, 0.8, 0.95,
             0.91, "hist");
      DrawTextOnPad(fHMean.fCandLatex[iP], 0.4, 0.9, 0.54, 0.99);
    }
  }
}

void LmvmDrawAll::DrawCutEffSignal()
{
  TH1D* mc = fHMean.H1Clone("hMom_cands_" + fHMean.fCandNames[0], ELmvmAnaStep::Mc);
  mc->Add(fHMean.H1("hMom_cands_" + fHMean.fCandNames[1], ELmvmAnaStep::Mc));

  TH1D* elid = fHMean.H1Clone("hMom_cands_" + fHMean.fCandNames[0], ELmvmAnaStep::ElId);
  elid->Add(fHMean.H1("hMom_cands_" + fHMean.fCandNames[1], ELmvmAnaStep::ElId));
  elid->Divide(mc);

  TH1D* gamma = fHMean.H1Clone("hMom_cands_" + fHMean.fCandNames[0], ELmvmAnaStep::GammaCut);
  gamma->Add(fHMean.H1("hMom_cands_" + fHMean.fCandNames[1], ELmvmAnaStep::GammaCut));
  gamma->Divide(mc);

  TH1D* tt = fHMean.H1Clone("hMom_cands_" + fHMean.fCandNames[0], ELmvmAnaStep::TtCut);
  tt->Add(fHMean.H1("hMom_cands_" + fHMean.fCandNames[1], ELmvmAnaStep::TtCut));
  tt->Divide(mc);

  TH1D* pt = fHMean.H1Clone("hMom_cands_" + fHMean.fCandNames[0], ELmvmAnaStep::PtCut);
  pt->Add(fHMean.H1("hMom_cands_" + fHMean.fCandNames[1], ELmvmAnaStep::PtCut));
  pt->Divide(mc);

  fHMean.fHM.CreateCanvas("CutEff_signal", "CutEff_signal", 900, 900);
  elid->GetYaxis()->SetTitle("Efficiency");
  DrawH1({elid, gamma, tt, pt}, {"El-ID", "Gamma cut", "TT cut", "P_{t} cut"}, kLinear, kLinear, true, 0.6, 0.75, 0.95,
         0.91, "hist");
  DrawTextOnPad("Signal Efficiency", 0.3, 0.9, 0.7, 0.99);
}

void LmvmDrawAll::DrawPionSuppression()
{
  for (auto step : {ELmvmAnaStep::Mc, ELmvmAnaStep::Acc}) {
    TH1D* elidMc = fHMean.H1Clone("hPionSupp_pi", step);
    elidMc->Divide(fHMean.H1("hPionSupp_idEl", ELmvmAnaStep::ElId));

    TH1D* gammaMc = fHMean.H1Clone("hPionSupp_pi", step);
    gammaMc->Divide(fHMean.H1("hPionSupp_idEl", ELmvmAnaStep::GammaCut));

    TH1D* ttMc = fHMean.H1Clone("hPionSupp_pi", step);
    ttMc->Divide(fHMean.H1("hPionSupp_idEl", ELmvmAnaStep::TtCut));

    TH1D* ptMc = fHMean.H1Clone("hPionSupp_pi", step);
    ptMc->Divide(fHMean.H1("hPionSupp_idEl", ELmvmAnaStep::PtCut));

    string text = fHMean.fAnaStepNames[static_cast<int>(step)];
    fHMean.fHM.CreateCanvas("PionSuppression_" + text, "PionSuppression_" + text, 900, 900);
    elidMc->GetYaxis()->SetTitle("Pion Suppression");
    DrawH1({elidMc, gammaMc, ttMc, ptMc}, {"El-ID", "Gamma cut", "TT cut", "P_{t} cut"}, kLinear, kLog, true, 0.7, 0.8,
           0.95, 0.91, "hist");
    DrawTextOnPad("Pion Suppression (norm. to " + text + ")", 0.25, 0.88, 0.75, 0.99);
  }
}

void LmvmDrawAll::DrawTofM2()
{
  // for all global tracks
  {
    TH2D* hEl = fHMean.H2Clone("hTofM2_gTracks_" + fHMean.fGTrackNames[0]);
    for (size_t iP = 1; iP < 4; iP++) {
      string hName = "hTofM2_gTracks_" + fHMean.fGTrackNames[iP];
      hEl->Add(fHMean.H2(hName.c_str()));
    }
    TH2D* hBg = fHMean.H2Clone("hTofM2_gTracks_" + fHMean.fGTrackNames[4]);
    for (size_t iP = 5; iP < fHMean.fGTrackNames.size(); iP++) {
      string hName = "hTofM2_gTracks_" + fHMean.fGTrackNames[iP];
      hBg->Add(fHMean.H2(hName.c_str()));
    }

    double minX = 0.;
    double maxX = 2.5;
    double minY = -0.05;
    double maxY = 0.05;
    double minZ = 1e-8;
    double maxZ = 1;
    vector<TLine*> lines {new TLine(0., 0.01, 1.3, 0.01), new TLine(1.3, 0.01, 2.5, 0.022)};  // set by hand
    TCanvas* c = fHMean.fHM.CreateCanvas("ToF/Purity/gTracks", "ToF/Purity/gTracks", 1600,
                                         800);  // TODO: check: is this all gTracks?
    c->Divide(2, 1);
    c->cd(1);
    hEl->GetXaxis()->SetRangeUser(minX, maxX);
    hEl->GetYaxis()->SetRangeUser(minY, maxY);
    hEl->GetZaxis()->SetRangeUser(minZ, maxZ);
    DrawH2(hEl, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("Electrons", 0.35, 0.89, 0.65, 0.99);
    for (size_t i = 0; i < lines.size(); i++) {
      lines[i]->SetLineWidth(2.);
      lines[i]->Draw();
    }
    c->cd(2);
    hBg->GetXaxis()->SetRangeUser(minX, maxX);
    hBg->GetYaxis()->SetRangeUser(minY, maxY);
    hBg->GetZaxis()->SetRangeUser(minZ, maxZ);
    DrawH2(hBg, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("Other", 0.35, 0.89, 0.65, 0.99);
    for (size_t i = 0; i < lines.size(); i++) {
      lines[i]->SetLineWidth(2.);
      lines[i]->Draw();
    }
  }

  // for candidates
  {
    double minX = 0.;
    double maxX = 2.5;
    double minY = -0.05;
    double maxY = 0.05;
    double minZ = 1e-8;
    double maxZ = 10;
    vector<TLine*> lines {new TLine(0., 0.01, 1.3, 0.01), new TLine(1.3, 0.01, 2.5, 0.022)};  // set by hand

    for (auto step : fHMean.fAnaSteps) {
      TH2D* hEl = fHMean.H2Clone(("hTofM2_cands_" + fHMean.fCandNames[0]), step);
      for (size_t iP = 1; iP < 4; iP++) {
        string hName = "hTofM2_cands_" + fHMean.fCandNames[iP] + "_" + fHMean.fAnaStepNames[static_cast<int>(step)];
        hEl->Add(fHMean.H2(hName.c_str()));
      }
      TH2D* hBg = fHMean.H2Clone(("hTofM2_cands_" + fHMean.fCandNames[4]), step);
      for (size_t iP = 5; iP < fHMean.fCandNames.size(); iP++) {
        string hName = "hTofM2_cands_" + fHMean.fCandNames[iP] + "_" + fHMean.fAnaStepNames[static_cast<int>(step)];
        hBg->Add(fHMean.H2(hName.c_str()));
      }
      string cName = "ToF/Purity/" + fHMean.fAnaStepNames[static_cast<int>(step)];
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1600, 800);
      c->Divide(2, 1);
      c->cd(1);
      hEl->GetXaxis()->SetRangeUser(minX, maxX);
      hEl->GetYaxis()->SetRangeUser(minY, maxY);
      hEl->GetZaxis()->SetRangeUser(minZ, maxZ);
      DrawH2(hEl, kLinear, kLinear, kLog, "colz");
      DrawTextOnPad("Electrons (" + fHMean.fAnaStepNames[static_cast<int>(step)] + ")", 0.3, 0.89, 0.6, 0.99);
      for (size_t i = 0; i < lines.size(); i++) {
        lines[i]->SetLineWidth(2.);
        lines[i]->Draw();
      }
      c->cd(2);
      hBg->GetXaxis()->SetRangeUser(minX, maxX);
      hBg->GetYaxis()->SetRangeUser(minY, maxY);
      hBg->GetZaxis()->SetRangeUser(minZ, maxZ);
      DrawH2(hBg, kLinear, kLinear, kLog, "colz");
      DrawTextOnPad("Other (" + fHMean.fAnaStepNames[static_cast<int>(step)] + ")", 0.3, 0.89, 0.55, 0.99);
      for (size_t i = 0; i < lines.size(); i++) {
        lines[i]->SetLineWidth(2.);
        lines[i]->Draw();
      }
    }
  }
}

void LmvmDrawAll::DrawPtYAndTofM2Elid()
{
  double x[200], y[200];
  int n     = 200;
  double m  = 511e-6;
  double m2 = m * m;
  double p  = 6;
  double p2 = p * p;
  for (int i = 0; i < n; i++) {
    x[i] = 0.02 * i;
    double counter =
      std::sqrt(2 * m2 * TMath::Exp(2 * x[i]) - m2 * TMath::Exp(4 * x[i]) + 4 * p2 * TMath::Exp(2 * x[i]) - m2);
    double denom = std::sqrt(1 + 2 * TMath::Exp(2 * x[i]) + TMath::Exp(4 * x[i]));
    double r     = counter / denom;
    y[i]         = r;
  }
  auto pLine = new TGraph(n, x, y);  // draw momentum line into Pt-Y histogram

  for (const string& cat : {"hPtY", "hTofM2"}) {
    // draw all global tracks seperate for diff. particles
    {
      double min   = 1e-8;
      double max   = 10;
      string cName = cat + "/globalTracks/all";
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 2400);
      c->Divide(4, 4);
      int i = 1;
      for (auto ptcl : fHMean.fGTrackNames) {
        c->cd(i++);
        string hName = cat + "_gTracks_" + ptcl;
        TH2D* h      = fHMean.H2Clone(hName.c_str());
        h->GetZaxis()->SetRangeUser(min, max);
        DrawH2(h, kLinear, kLinear, kLog, "colz");
        DrawTextOnPad(fHMean.fGTrackLatex[i - 2], 0.25, 0.9, 0.75, 0.999);
        if (cat == "hPtY") pLine->Draw("C");
      }
    }

    // draw zoomed in TofM2
    {
      if (cat == "hTofM2") {
        double min   = 1e-8;
        double max   = 10;
        string cName = cat + "/globalTracks/all_zoom";
        TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 2400);
        c->Divide(4, 4);
        int i = 1;
        for (auto ptcl : fHMean.fGTrackNames) {
          c->cd(i++);
          string hName = cat + "_gTracks_" + ptcl;
          TH2D* h      = fHMean.H2Clone(hName.c_str());
          h->GetXaxis()->SetRangeUser(0., 2.5);
          h->GetYaxis()->SetRangeUser(-0.05, 0.05);
          h->GetZaxis()->SetRangeUser(min, max);
          DrawH2(h, kLinear, kLinear, kLog, "colz");
          DrawTextOnPad(fHMean.fGTrackLatex[i - 2], 0.25, 0.9, 0.75, 0.999);
        }
      }
    }

    // draw misidentified and not-misidentified particles and ratio of both (all from global tracks)
    {
      double min = (cat == "hPtY") ? 1e-7 : 1e-8;
      double max = (cat == "hPtY") ? 10 : 10;
      for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {  // only draw for not-electrons
        string ptcl  = fHMean.fCandNames[iP];
        string cName = cat + "/candidates/" + ptcl + "_misid";
        TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 2400, 800);
        c->Divide(3, 1);
        c->cd(1);
        TH2D* hTrue = fHMean.H2Clone(cat + "_" + ptcl + "_true");
        if (cat == "hTofM2") {
          hTrue->GetXaxis()->SetRangeUser(0., 2.5);
          hTrue->GetYaxis()->SetRangeUser(-0.05, 0.05);
        }
        hTrue->GetZaxis()->SetRangeUser(min, max);
        DrawH2(hTrue, kLinear, kLinear, kLog, "colz");
        DrawTextOnPad(fHMean.fCandLatex[iP] + " (not misidentified)", 0.2, 0.9, 0.75, 0.99);
        if (cat == "hPtY") pLine->Draw("C");

        c->cd(2);
        TH2D* hMis = fHMean.H2Clone(cat + "_" + ptcl + "_misid");
        if (cat == "hTofM2") {
          hMis->GetXaxis()->SetRangeUser(0., 2.5);
          hMis->GetYaxis()->SetRangeUser(-0.05, 0.05);
        }
        hMis->GetZaxis()->SetRangeUser(min, max);
        DrawH2(hMis, kLinear, kLinear, kLog, "colz");
        DrawTextOnPad(fHMean.fCandLatex[iP] + " (misidentified as electron)", 0.15, 0.9, 0.75, 0.99);
        if (cat == "hPtY") pLine->Draw("C");

        c->cd(3);
        TH2D* hRat = fHMean.H2Clone(cat + "_" + ptcl + "_misid");
        hRat->Divide(hTrue);
        if (cat == "hTofM2") {
          hRat->GetXaxis()->SetRangeUser(0., 2.5);
          hRat->GetYaxis()->SetRangeUser(-0.05, 0.05);
        }
        hRat->GetZaxis()->SetRangeUser(1e-5, 1);  // TODO: check best min value
        hRat->GetZaxis()->SetTitle("Ratio #misid./#not misid.");
        DrawH2(hRat, kLinear, kLinear, kLog, "colz");
        DrawTextOnPad(fHMean.fCandLatex[iP] + ":  Ratio #misid./#not misid.", 0.1, 0.9, 0.6, 0.99);
        if (cat == "hPtY") pLine->Draw("C");
      }
    }

    // draw misidentified candidates for each step
    {
      double min = (cat == "hPtY") ? 2e-8 : 2e-8;
      double max = (cat == "hPtY") ? 5e-3 : 5e-3;
      for (size_t iP = 4; iP < fHMean.fCandNames.size(); iP++) {
        string ptcl   = fHMean.fCandNames[iP];
        string cName  = cat + "/candidates/" + ptcl + "_misid_steps";
        TCanvas* cPty = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
        cPty->Divide(3, 3);
        int i = 1;
        for (auto step : fHMean.fAnaSteps) {
          if (step < ELmvmAnaStep::ElId) continue;
          cPty->cd(i++);
          string hName = cat + "_cands_" + ptcl;
          TH2D* h      = fHMean.H2Clone(hName.c_str(), step);
          if (cat == "hTofM2") {
            h->GetXaxis()->SetRangeUser(0., 2.5);
            h->GetYaxis()->SetRangeUser(-0.05, 0.05);
          }
          h->GetZaxis()->SetRangeUser(min, max);
          DrawH2(h, kLinear, kLinear, kLog, "colz");
          fHMean.DrawAnaStepOnPad(step);
          if (cat == "hPtY") pLine->Draw("C");
        }
      }
    }
  }  // cat: "hPtY", "hTofM2"
}

void LmvmDrawAll::DrawTofPilePids()
{
  // draw particles that occur in "ToF Pile"
  vector<string> xLabel = {"e^{-}_{PLUTO}",
                           "e^{+}_{PLUTO}",
                           "e^{-}_{UrQMD_prim}",
                           "e^{+}_{UrQMD_prim}",
                           "e^{-}_{UrQMD_sec}",
                           "e^{+}_{UrQMD_sec}",
                           "#pi^{+}",
                           "#pi^{-}",
                           "p",
                           "K^{+}",
                           "K^{-}",
                           "o."};
  double max            = 3;
  double min            = 1e-5;
  TCanvas* cTofPile     = fHMean.fHM.CreateCanvas("ToF/TofPilePids", "ToF/TofPilePids", 1800, 1800);
  cTofPile->Divide(4, 4);

  cTofPile->cd(1);
  TH1D* h0 = fHMean.H1Clone("hTofPilePdgs_gTracks");
  for (size_t iL = 0; iL < xLabel.size(); iL++) {
    h0->GetXaxis()->SetBinLabel(iL + 1, xLabel[iL].c_str());
  }
  h0->GetYaxis()->SetRangeUser(min, max);
  DrawH1(h0, kLinear, kLog, "hist");
  DrawTextOnPad("Global Tracks", 0.35, 0.9, 0.65, 0.999);

  int i = 2;
  for (auto step : fHMean.fAnaSteps) {
    cTofPile->cd(i++);
    TH1D* h = fHMean.H1Clone("hTofPilePdgs_cands", step);
    for (size_t iL = 0; iL < xLabel.size(); iL++) {
      h->GetXaxis()->SetBinLabel(iL + 1, xLabel[iL].c_str());
    }
    h->GetYaxis()->SetRangeUser(min, max);
    DrawH1(h, kLinear, kLog, "hist");
    fHMean.DrawAnaStepOnPad(step);
  }
}

void LmvmDrawAll::DrawPurity()
{
  // Misidentified Particles vs. Momentum
  {
    vector<string> yLabel = {"e^{#pm}_{PLUTO}", "e^{#pm}_{UrQMD}", "#pi^{#pm}", "p", "K^{+}", "o."};
    for (ELmvmAnaStep step : fHMean.fAnaSteps) {
      if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc) continue;
      string cName = "Purity/purity_" + fHMean.fAnaStepNames[static_cast<int>(step)];
      string hName = "hCandPdgVsMom_" + fHMean.fAnaStepNames[static_cast<int>(step)];
      fHMean.fHM.CreateCanvas(cName, cName, 800, 600);
      TH2D* h = fHMean.H2Clone(hName);
      h->GetZaxis()->SetRangeUser(5e-7, 1e-2);
      DrawH2(h, kLinear, kLinear, kLog, "colz");
      for (size_t y = 1; y <= yLabel.size(); y++) {
        h->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      }
      double nEl    = h->Integral(1, h->GetXaxis()->GetNbins(), 2, 2);  // do not count PLUTO particles
      double purity = (nEl / h->Integral(1, h->GetXaxis()->GetNbins(), 2, h->GetYaxis()->GetNbins())) * 100;
      DrawTextOnPad("Purity: " + Cbm::NumberToString(purity, 1) + " %", 0.1, 0.9, 0.45, 0.99);
    }
  }

  // Purity vs. Momentum after ElID step
  {
    int nBins    = fHMean.H2("hCandPdgVsMom_elid")->GetXaxis()->GetNbins();
    double xMin  = fHMean.H2("hCandPdgVsMom_elid")->GetXaxis()->GetXmin();
    double xMax  = fHMean.H2("hCandPdgVsMom_elid")->GetXaxis()->GetXmax();
    TH1D* purity = new TH1D("purity_Mom", "purity_Mom; P [GeV/c]; Purity [%]", nBins, xMin, xMax);
    for (int i = 1; i <= purity->GetNbinsX(); i++) {
      double nEl  = fHMean.H2("hCandPdgVsMom", ELmvmAnaStep::ElId)->GetBinContent(i, 2);
      double nAll = fHMean.H2("hCandPdgVsMom", ELmvmAnaStep::ElId)
                      ->Integral(i, i, 2, fHMean.H2("hCandPdgVsMom_elid")->GetYaxis()->GetNbins());
      double val = (nAll != 0) ? 100 * nEl / nAll : 0.;
      purity->SetBinContent(i, val);
    }
    purity->Rebin(5);
    purity->Scale(1. / 5.);
    fHMean.fHM.CreateCanvas("Purity/purityVsMom_elid", "Purity/purityVsMom_elid", 800, 800);
    DrawH1(purity, kLinear, kLinear, "p");
  }

  // Purity seperate for ID Detectors
  {
    vector<string> yLabel = {"#pi^{+}", "#pi^{-}", "p", "K^{+}", "K^{-}", "o."};
    double min            = 1e-7;
    double max            = 10;

    TH2D* rich1 = fHMean.H2Clone("hPdgVsMom_gTracks_rich_all");
    TH2D* trd1  = fHMean.H2Clone("hPdgVsMom_gTracks_trd_all");
    TH2D* tof1  = fHMean.H2Clone("hPdgVsMom_gTracks_tof_all");
    TH2D* rich2 = fHMean.H2Clone("hPdgVsMom_gTracks_rich_complete");
    TH2D* trd2  = fHMean.H2Clone("hPdgVsMom_gTracks_trd_complete");
    TH2D* tof2  = fHMean.H2Clone("hPdgVsMom_gTracks_tof_complete");

    rich1->GetZaxis()->SetRangeUser(min, max);
    trd1->GetZaxis()->SetRangeUser(min, max);
    tof1->GetZaxis()->SetRangeUser(min, max);
    rich2->GetZaxis()->SetRangeUser(min, max);
    trd2->GetZaxis()->SetRangeUser(min, max);
    tof2->GetZaxis()->SetRangeUser(min, max);

    for (size_t y = 1; y <= yLabel.size(); y++) {
      rich1->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      trd1->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      tof1->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      rich2->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      trd2->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
      tof2->GetYaxis()->SetBinLabel(y, yLabel[y - 1].c_str());
    }

    TCanvas* c1 = fHMean.fHM.CreateCanvas("Misid/misid_gTracks_all", "Misid/misid_gTracks_all", 2400, 800);
    c1->Divide(3, 1);
    c1->cd(1);
    DrawH2(rich1, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("RICH", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(rich1);
    c1->cd(2);
    DrawH2(trd1, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("TRD", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(trd1);
    c1->cd(3);
    DrawH2(tof1, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(tof1);

    TCanvas* c2 = fHMean.fHM.CreateCanvas("Misid/misid_gTracks_complete", "Misid/misid_gTracks_complete", 2400, 800);
    c2->Divide(3, 1);
    c2->cd(1);
    DrawH2(rich2, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("RICH", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(rich2);

    c2->cd(2);
    DrawH2(trd2, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("TRD", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(trd2);

    c2->cd(3);
    DrawH2(tof2, kLinear, kLinear, kLog, "colz");
    DrawTextOnPad("ToF", 0.25, 0.9, 0.75, 0.999);
    DrawPurityHistText(tof2);
  }
}

void LmvmDrawAll::DrawPurityHistText(TH2D* h)
{
  int nX = h->GetXaxis()->GetNbins();

  double xMin = 0.4;
  double xMax = 0.75;
  double piP  = h->Integral(1, nX, 1, 1);
  double piM  = h->Integral(1, nX, 2, 2);
  double p    = h->Integral(1, nX, 3, 3);
  double KP   = h->Integral(1, nX, 4, 4);
  double KM   = h->Integral(1, nX, 5, 5);
  double o    = h->Integral(1, nX, 6, 6);

  DrawTextOnPad("#pi^{+}: " + Cbm::NumberToString(piP, 1) + " /Ev", xMin, 0.12, xMax, 0.3);
  DrawTextOnPad("#pi^{-}: " + Cbm::NumberToString(piM, 1) + " /Ev", xMin, 0.28, xMax, 0.37);
  DrawTextOnPad("p: " + Cbm::NumberToString(p, 1) + " /Ev", xMin, 0.43, xMax, 0.48);
  DrawTextOnPad("K^{+}: " + Cbm::NumberToString(KP, 1) + " /Ev", xMin, 0.53, xMax, 0.65);
  DrawTextOnPad("K^{-}: " + Cbm::NumberToString(KM, 1) + " /Ev", xMin, 0.66, xMax, 0.76);
  DrawTextOnPad("o.: " + Cbm::NumberToString(o, 1) + " /Ev", xMin, 0.77, xMax, 0.9);
}

void LmvmDrawAll::InvestigateMisid()
{
  DrawMomentum();
  DrawPtYAndTofM2Elid();
  DrawTofPilePids();
  DrawChi2();

  // Connection between Misidentification and Mismatches
  {
    vector<string> xLabel = {"0", "1", "2", "3"};
    vector<string> yLabel = {"true-ID", "mis-ID"};
    fHMean.DrawAllGTracks(2, "hMatchId_gTracks", "hMatchId_gTracks", {xLabel}, {yLabel}, 1e-7, 1e3);
  }

  // RICH: Ring-Track Distance
  fHMean.DrawAllGTracks(2, "hRichRingTrackDist/gTracks_all", "hRichRingTrackDist_gTracks", {""}, {""}, 1e-7,
                        1e-2);  // TODO: move this anywhere else?
  fHMean.DrawAllCandsAndSteps(2, "hRichRingTrackDist/", "hRichRingTrackDist_cands", {""}, {""}, 1e-7, 1e-2);

  // ToF
  {
    double minXY = 1e-6;
    double maxXY = 1e-3;
    double minD  = 3e-6;
    double maxD  = 3e-1;
    for (const string& cat : {"trueid", "misid", "truematch", "mismatch"}) {
      fHMean.DrawAllGTracks(2, "ToF/HitTrackDist/HitPointDist_" + cat, "hTofHitTrackDist_gTracks_" + cat, {""}, {""},
                            1e-9, 1.);
      fHMean.DrawAllGTracks(2, "ToF/HitXY/PointXY_" + cat, "hTofPointXY_" + cat, {""}, {""}, minXY, maxXY);
      fHMean.DrawAllGTracks(2, "ToF/HitXY/HitXY_" + cat, "hTofHitXY_" + cat, {""}, {""}, minXY, maxXY);
      fHMean.DrawAllGTracks(1, "ToF/HitXY/HitPointDist_" + cat, "hTofHitPointDist_" + cat, {""}, {""}, minD, maxD);
    }
    fHMean.DrawAllCands(2, "ToF/Time/HitTrackDist_cands", "hTofHitTrackDist_cands", {""}, {""}, 1e-9, 1.);
    fHMean.DrawAllCands(2, "ToF/Time/TimeVsMom_", "hTofTimeVsMom_cands", {""}, {""}, 1e-9, 1e-2);
    fHMean.DrawAllGTracks(2, "ToF/Time/IdAndMatch", "hTofM2Calc_gTracks", {""},
                          {"true-ID", "mis-ID", "truematch", "mismatch"}, 1e-9, 10);

    TCanvas* c1 = fHMean.fHM.CreateCanvas("ToF/HitXY/HitPointDist_all", "ToF/HitXY/HitPointDist_all", 2400, 2400);
    c1->Divide(4, 4);
    int iC = 1;
    for (size_t iP = 0; iP < fHMean.fGTrackNames.size(); iP++) {
      c1->cd(iC++);
      string hName = (iP <= 3) ? "hTofHitPointDist_trueid_" + fHMean.fGTrackNames[iP]
                               : "hTofHitPointDist_misid_" + fHMean.fGTrackNames[iP];
      string text  = (iP <= 3) ? "true-ID" : "mis-ID";
      TH1D* h      = fHMean.H1Clone(hName);
      h->GetYaxis()->SetRangeUser(minD, maxD);
      h->Fit("gaus", "Q", "", 0., 2.);
      DrawH1(h, kLinear, kLog, "hist");
      DrawTextOnPad(text, 0.5, 0.7, 0.8, 0.9);
      DrawTextOnPad(fHMean.fGTrackLatex[iP], 0.25, 0.9, 0.75, 0.99);
      TF1* func   = h->GetFunction("gaus");
      double mean = (func != nullptr) ? func->GetParameter("Mean") : 0.;
      DrawTextOnPad("mean: " + Cbm::NumberToString(mean, 2), 0.2, 0.65, 0.6, 0.89);
    }
  }

  // draw chi2 of true-matched vs. mismatched
  for (const string& match : {"truematch", "mismatch"}) {
    for (const string& cat : {"all", "complete"}) {
      for (const string& det : {"rich", "trd", "tof"}) {
        string cName = "hChi2/" + match + "_" + det + "_" + cat;
        string hName = "hChi2_" + match + "_" + cat + "_" + det;
        fHMean.DrawAllGTracks(1, cName, hName, {""}, {""}, 2e-7, 10);
      }
    }
  }

  // draw vertex of ToF-misidentifications
  {
    double min = 1e-7;
    for (auto step : fHMean.fAnaSteps) {
      string cName = "ToF/Vertex/tofMisid_" + fHMean.fAnaStepNames[static_cast<int>(step)];
      vector<TH2D*> xyz {fHMean.H2("hVertexXZ_misidTof", step), fHMean.H2("hVertexYZ_misidTof", step),
                         fHMean.H2("hVertexXY_misidTof", step)};

      TCanvas* c = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 600);
      c->Divide(3, 1);
      for (size_t i = 0; i < xyz.size(); i++) {
        c->cd(i + 1);
        DrawH2(xyz[i]);
        xyz[i]->SetMinimum(min);
        gPad->SetLogz(true);
      }

      TCanvas* cZoom = fHMean.fHM.CreateCanvas((cName + "_zoom").c_str(), (cName + "_zoom").c_str(), 1800, 600);
      cZoom->Divide(3, 1);
      for (size_t i = 0; i < xyz.size(); i++) {
        TH2D* hZoom = (TH2D*) xyz[i]->Clone();
        cZoom->cd(i + 1);
        if (i == 2) {
          hZoom->GetXaxis()->SetRangeUser(-10., 10.);
          hZoom->GetYaxis()->SetRangeUser(-10., 10.);
        }
        else {
          hZoom->GetXaxis()->SetRangeUser(fZ - 1., fZ + 10.);
          hZoom->GetYaxis()->SetRangeUser(-10., 10.);
        }
        DrawH2(hZoom);
        hZoom->SetMinimum(min);
        gPad->SetLogz(true);
      }

      fHMean.fHM.CreateCanvas((cName + "_RZ").c_str(), (cName + "_RZ").c_str(), 900, 900);
      TH2D* hRZ = fHMean.H2Clone("hVertexRZ_misidTof", step);
      hRZ->SetMinimum(min);
      DrawH2(hRZ, kLinear, kLinear, kLog);
    }
  }

  // draw RICH/TRD/ToF index vs. STS index
  /*{
    for (const string& det : {"Rich", "Trd", "Tof"}) {
      string cName = "hIndex/" + det;
      string hName = "hIndexSts" + det;
      fHMean.DrawAllGTracks(2, cName, hName, {""}, {""}, 1e-7, 2e-3);
    }
  }*/

  double minMism = 1e-4;
  double maxMism = 100.;

  vector<string> xLabelDet = {"STS_{all}", "RICH_{all}", "RICH_{mis}", "TRD_{all}",
                              "TRD_{mis}", "ToF_{all}",  "ToF_{mis}"};
  fHMean.DrawAllGTracks(1, "Mismatches/hNofMismatches_all", "hNofMismatches_gTracks_all", xLabelDet, {""}, minMism,
                        maxMism);
  fHMean.DrawAllGTracks(1, "Mismatches/hNofMismatches_complete", "hNofMismatches_gTracks_complete", xLabelDet, {""},
                        minMism, maxMism);

  vector<string> xLabelSeg = {"0", "1", "2", "3"};
  fHMean.DrawAllGTracks(1, "Mismatches/hNofMismatchedTrackSegments_all", "hNofMismatchedTrackSegments_all", xLabelSeg,
                        {""}, minMism, maxMism);
  fHMean.DrawAllGTracks(1, "Mismatches/hNofMismatchedTrackSegments_complete", "hNofMismatchedTrackSegments_complete",
                        xLabelSeg, {""}, minMism, maxMism);
}

void LmvmDrawAll::DrawChi2()
{
  double min = 1e-9;
  double max = 5e-3;
  for (const string det : {"sts", "rich", "trd", "tof"}) {
    fHMean.DrawAllCands(2, "hChi2/Detectors/hChi2VsMom_" + det, "hChi2VsMom_" + det, {""}, {""}, min, max);
    fHMean.DrawAllCands(2, "hChi2/Detectors/hTofTimeVsChi2_" + det, "hTofTimeVsChi2_" + det, {""}, {""}, min, max);
  }

  for (const string det : {"StsRich", "StsTrd", "RichTrd"}) {
    fHMean.DrawAllCands(2, "hChi2/Detectors/hChi2Comb" + det, "hChi2Comb_" + det, {""}, {""}, min, max);
  }
}

void LmvmDrawAll::DrawMinvAll()
{
  for (const ELmvmAnaStep step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    string cName = "Minv/" + fHMean.fAnaStepNames[static_cast<int>(step)];
    fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1000, 1000);
    DrawMinv(step);
    fHMean.DrawAnaStepOnPad(step);
  }
}

void LmvmDrawAll::DrawMinv(ELmvmAnaStep step)
{
  TH1D* bg       = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
  TH1D* pi0      = fHMean.H1Clone("hMinv", ELmvmSrc::Pi0, step);
  TH1D* eta      = fHMean.H1Clone("hMinv", ELmvmSrc::Eta, step);
  TH1D* cocktail = GetCocktailMinvH1("hMinv", step);
  TH1D* sbg      = static_cast<TH1D*>(bg->Clone());
  sbg->Add(cocktail);
  TH1D* cb        = fHMean.H1Clone("hMinvCombBg", step);
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
    drawData.emplace_back(bg, kGray, kBlack, 1, 3001, "Background");
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
    min = 1e-9;
    max = std::max(d.fH->GetMaximum(), max);
  }
  if (min == 0.) min = std::min(1e-4, max * 1e-7);
  if (h0 != nullptr) h0->SetMinimum(min);
  if (h0 != nullptr) h0->SetMaximum(1.1 * max);

  leg->SetFillColor(kWhite);
  leg->Draw();
  gPad->SetLogy(true);
}

void LmvmDrawAll::DrawMomRecoPrecision()
{
  // draw ratio P_rec/P_MC
  {
    for (const string& cat : {"", "_zoom", "_zoom2"}) {
      string cName = "hMom/precisionReco/mom_ratioRecOverMc" + cat;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        vector<TH1*> hists;
        vector<string> legend;
        c->cd(i++);
        for (size_t iP = 0; iP < fHMean.fCandNames.size(); iP++) {
          TH1D* h = fHMean.H1Clone("hMomRatio_cands_" + fHMean.fCandNames[iP], step);
          if (cat == "_zoom") h->GetXaxis()->SetRangeUser(0.9, 1.1);
          if (cat == "_zoom2") h->GetXaxis()->SetRangeUser(7., 7.5);
          hists.push_back(h);
          legend.push_back((fHMean.fCandLatex[iP]).c_str());
        }
        DrawH1(hists, legend, kLinear, kLog, true, 0.8, 0.65, 0.99, 0.99, "hist");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  // draw ratio P_rec/P_MC vs P
  {
    ELmvmAnaStep step = ELmvmAnaStep::ElId;
    double min        = 0.5;
    double max        = 1.5;
    TCanvas* c        = fHMean.fHM.CreateCanvas("hMom/precisionReco/mom_ratioRecOverMcVsMom",
                                         "hMom/precisionReco/mom_ratioRecOverMcVsMom", 2700, 1800);
    c->Divide(4, 3);
    int i = 1;
    for (auto ptcl : fHMean.fCandNames) {
      int iL = i - 1;
      c->cd(i++);
      string hName = "hMomRatioVsMom_cands_" + ptcl;
      TH2D* h      = fHMean.H2Clone(hName.c_str(), step);
      h->GetYaxis()->SetRangeUser(min, max);
      DrawH2(h, kLinear, kLinear, kLog, "colz");
      string text = fHMean.fCandLatex[iL] + ", " + fHMean.fAnaStepLatex[static_cast<int>(step)];
      DrawTextOnPad(text.c_str(), 0.25, 0.9, 0.75, 0.999);
    }
  }
}

void LmvmDrawAll::DrawMinvOfficialStyle()
{
  // variable binning
  TH1D* h            = fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::Mc);  // template for values
  double ChangeValue = 1.2;
  int f              = 2;  // factor larger/smaller binsize
  int binChange      = h->GetXaxis()->FindBin(ChangeValue);
  double bW          = h->GetXaxis()->GetBinWidth(1);
  const int nBins    = (int) binChange + (h->GetNbinsX() - binChange) / f;
  vector<double> binEdges(nBins + 1, 0.);

  for (int iB = 1; iB <= nBins; iB++) {
    binEdges[iB] = (iB < binChange) ? binEdges[iB - 1] + bW : binEdges[iB - 1] + f * bW;
  }
  binEdges[nBins + 1] = h->GetBinCenter(h->GetNbinsX()) + bW / f;

  int nofInmed  = H(ELmvmSignal::Inmed)->H1("hEventNumber")->GetEntries();
  int nofQgp    = H(ELmvmSignal::Qgp)->H1("hEventNumber")->GetEntries();
  int nofOmega  = H(ELmvmSignal::Omega)->H1("hEventNumber")->GetEntries();
  int nofOmegaD = H(ELmvmSignal::OmegaD)->H1("hEventNumber")->GetEntries();
  int nofPhi    = H(ELmvmSignal::Phi)->H1("hEventNumber")->GetEntries();

  for (auto step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    TH1D* h_npm    = new TH1D("h_npm", "h_npm", nBins, binEdges.data());
    TH1D* h_bc     = new TH1D("h_bc", "h_bc", nBins, binEdges.data());
    TH1D* h_coc    = new TH1D("h_coc", "h_coc", nBins, binEdges.data());
    TH1D* h_eta    = new TH1D("h_eta", "h_eta", nBins, binEdges.data());
    TH1D* h_pi0    = new TH1D("h_pi0", "h_pi0", nBins, binEdges.data());
    TH1D* h_inmed  = new TH1D("h_inmed", "h_inmed", nBins, binEdges.data());
    TH1D* h_qgp    = new TH1D("h_qgp", "h_qgp", nBins, binEdges.data());
    TH1D* h_omega  = new TH1D("h_omega", "h_omega", nBins, binEdges.data());
    TH1D* h_omegaD = new TH1D("h_omegaD", "h_omegaD", nBins, binEdges.data());
    TH1D* h_phi    = new TH1D("h_phi", "h_phi", nBins, binEdges.data());

    TH1D* hInmed0  = H(ELmvmSignal::Inmed)->H1Clone("hMinv", ELmvmSrc::Signal, step);
    TH1D* hQgp0    = H(ELmvmSignal::Qgp)->H1Clone("hMinv", ELmvmSrc::Signal, step);
    TH1D* hOmega0  = H(ELmvmSignal::Omega)->H1Clone("hMinv", ELmvmSrc::Signal, step);
    TH1D* hOmegaD0 = H(ELmvmSignal::OmegaD)->H1Clone("hMinv", ELmvmSrc::Signal, step);
    TH1D* hPhi0    = H(ELmvmSignal::Phi)->H1Clone("hMinv", ELmvmSrc::Signal, step);

    hInmed0->Rebin(fRebMinv);
    hQgp0->Rebin(fRebMinv);
    hOmega0->Rebin(fRebMinv);
    hOmegaD0->Rebin(fRebMinv);
    hPhi0->Rebin(fRebMinv);

    hInmed0->Scale(1. / (nofInmed * bW));
    hQgp0->Scale(1. / (nofQgp * bW));
    hOmega0->Scale(1. / (nofOmega * bW));
    hOmegaD0->Scale(1. / (nofOmegaD * bW));
    hPhi0->Scale(1. / (nofPhi * bW));

    for (int iB = 1; iB <= h->GetNbinsX(); iB++) {
      if (iB < binChange) {
        h_npm->SetBinContent(iB, fHMean.H1("hMinvCombPM_sameEv", step)->GetBinContent(iB));
        h_bc->SetBinContent(iB, fHMean.H1("hMinvCombBg", step)->GetBinContent(iB));
        h_coc->SetBinContent(iB, GetCocktailMinvH1("hMinv", step)->GetBinContent(iB));
        h_eta->SetBinContent(iB, fHMean.H1("hMinv", ELmvmSrc::Eta, step)->GetBinContent(iB));
        h_pi0->SetBinContent(iB, fHMean.H1("hMinv", ELmvmSrc::Pi0, step)->GetBinContent(iB));
        h_inmed->SetBinContent(iB, hInmed0->GetBinContent(iB));
        h_qgp->SetBinContent(iB, hQgp0->GetBinContent(iB));
        h_omega->SetBinContent(iB, hOmega0->GetBinContent(iB));
        h_omegaD->SetBinContent(iB, hOmegaD0->GetBinContent(iB));
        h_phi->SetBinContent(iB, hPhi0->GetBinContent(iB));
      }
      else {
        int iB2 = (int) binChange + (iB - binChange) / f;
        h_npm->SetBinContent(iB2,
                             h_npm->GetBinContent(iB2) + fHMean.H1("hMinvCombPM_sameEv", step)->GetBinContent(iB) / f);
        h_bc->SetBinContent(iB2, h_bc->GetBinContent(iB2) + fHMean.H1("hMinvCombBg", step)->GetBinContent(iB) / f);
        h_coc->SetBinContent(iB2, h_coc->GetBinContent(iB2) + GetCocktailMinvH1("hMinv", step)->GetBinContent(iB) / f);
        h_eta->SetBinContent(iB2, h_eta->GetBinContent(iB2)
                                    + fHMean.H1("hMinv", ELmvmSrc::Eta, step)->GetBinContent(iB) / f);
        h_pi0->SetBinContent(iB2, h_pi0->GetBinContent(iB2)
                                    + fHMean.H1("hMinv", ELmvmSrc::Pi0, step)->GetBinContent(iB) / f);
        h_inmed->SetBinContent(iB2, h_inmed->GetBinContent(iB2) + hInmed0->GetBinContent(iB) / f);
        h_qgp->SetBinContent(iB2, h_qgp->GetBinContent(iB2) + hQgp0->GetBinContent(iB) / f);
        h_omega->SetBinContent(iB2, h_omega->GetBinContent(iB2) + hOmega0->GetBinContent(iB) / f);
        h_omegaD->SetBinContent(iB2, h_omegaD->GetBinContent(iB2) + hOmegaD0->GetBinContent(iB) / f);
        h_phi->SetBinContent(iB2, h_phi->GetBinContent(iB2) + hPhi0->GetBinContent(iB) / f);
      }
    }

    // set errors
    int nEv = GetNofTotalEvents();
    for (int iB = 1; iB <= h_npm->GetNbinsX(); iB++) {
      double bW2 = h_npm->GetBinLowEdge(iB + 1) - h_npm->GetBinLowEdge(iB);
      //cout << "iB = " << iB <<  " | bW2 = " << bW2 << endl;
      h_npm->SetBinError(iB, std::sqrt(h_npm->GetBinContent(iB) * bW2 * nEv) / (bW2 * nEv));
      h_bc->SetBinError(iB, std::sqrt(h_bc->GetBinContent(iB) * bW2 * nEv) / (bW2 * nEv));
    }

    TH1D* h_sum = (TH1D*) h_eta->Clone();
    h_sum->Add(h_pi0);
    h_sum->Add(h_inmed);
    h_sum->Add(h_qgp);
    h_sum->Add(h_omega);
    h_sum->Add(h_omegaD);
    h_sum->Add(h_phi);

    TH1D* h_sum2 = (TH1D*) h_sum->Clone();

    fHMean.SetOptH1(h_npm, "#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]",
                    "1/N_{ev} dN/d#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]^{-1}", 510, 21, 1.3, kPink, "marker");
    fHMean.SetOptH1(h_bc, "#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]",
                    "1/N_{ev} dN/d#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]^{-1}", 510, 22, 1.3, kBlue - 2, "marker");
    fHMean.SetOptH1(h_sum, "#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]",
                    "1/N_{ev} dN/d#font[52]{M}_{ee} [GeV/#font[52]{c}^{2}]^{-1}", 510, 20, 1.1, kBlack, "marker");

    fHMean.SetOptH1(h_sum2, "", "", 510, 1, 1, kBlack, "line");
    fHMean.SetOptH1(h_eta, "", "", 510, 1, 1, kBlue + 2, "line");
    fHMean.SetOptH1(h_pi0, "", "", 510, 1, 1, kAzure - 4, "line");
    fHMean.SetOptH1(h_inmed, "", "", 510, 1, 1, kPink, "line");
    fHMean.SetOptH1(h_qgp, "", "", 510, 1, 1, kOrange, "line");
    fHMean.SetOptH1(h_omega, "", "", 510, 1, 1, kGreen + 1, "line");
    fHMean.SetOptH1(h_omegaD, "", "", 510, 1, 1, kGreen + 3, "line");
    fHMean.SetOptH1(h_phi, "", "", 510, 1, 1, kMagenta + 2, "line");

    //h_npm->GetXaxis()->SetRangeUser(0.,2.);
    h_npm->GetYaxis()->SetRangeUser(1e-9, 10);

    string cName = "MinvOfficialStyle/minv_" + fHMean.fAnaStepNames[static_cast<int>(step)];
    TCanvas* can = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 900, 918);
    fHMean.SetOptCanvas(can);
    can->SetLogy();
    can->cd();

    h_npm->Draw("peist");
    h_bc->Draw("peistsame");
    h_sum->Draw("phistsame");

    h_sum2->Draw("histsame");
    h_eta->Draw("histsame");
    h_pi0->Draw("histsame");
    h_inmed->Draw("histsame");
    h_qgp->Draw("histsame");
    h_omega->Draw("histsame");
    h_omegaD->Draw("histsame");
    h_phi->Draw("histsame");

    vector<LmvmLegend> legend3;
    legend3.emplace_back(h_npm, "N^{#pm}_{same}", "p");
    legend3.emplace_back(h_bc, "CB_{calc}", "p");
    legend3.emplace_back(h_sum, "MC Cocktail", "p");

    vector<LmvmLegend> legend8;
    legend8.emplace_back(h_sum2, "MC Cocktail", "l");
    legend8.emplace_back(h_eta, "#eta#rightarrow#gammae^{+}e^{-}", "l");
    legend8.emplace_back(h_pi0, "#pi^{0}#rightarrow#gammae^{+}e^{-}", "l");
    legend8.emplace_back(h_omegaD, "#omega#rightarrow#pi^{0}e^{+}e^{-}", "l");
    legend8.emplace_back(h_omega, "#omega#rightarrowe^{+}e^{-}", "l");
    legend8.emplace_back(h_phi, "#phi#rightarrowe^{+}e^{-}", "l");
    legend8.emplace_back(h_inmed, "Rapp in-medium SF", "l");
    legend8.emplace_back(h_qgp, "Rapp QGP", "l");

    fHMean.SetLegend(legend3, 0.035, 0.37, 0.64, 0.57, 0.77);
    fHMean.SetLegend(legend8, 0.025, 0.66, 0.57, 0.82, 0.87);

    TLatex* tex = new TLatex(0.15, 2.5, "CBM Simulations");
    tex->SetTextColor(1);  //kAzure+6);
    tex->SetTextSize(0.035);
    tex->SetTextFont(42);
    tex->Draw();
    TLatex* tex2 = new TLatex(0.15, 0.7, "0 fm Au+Au, #sqrt{s_{NN}}=4.1 GeV");
    tex2->SetTextColor(1);  //kAzure+6);
    tex2->SetTextSize(0.035);
    tex2->SetTextFont(42);
    tex2->Draw();

    /*delete h_npm;
    delete h_bc;
    delete h_coc;
    delete h_eta;
    delete h_pi0;
    delete h_inmed;
    delete h_qgp;
    delete h_omega;
    delete h_omegaD;
    delete h_phi;
    delete h_sum;
    delete h_sum2;*/
  }  // steps
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
  drawData.emplace_back(pipi0, kGreen - 3, kGreen + 3, 2, -1, "#pi^{#pm}_{misid} - #pi^{0}");
  drawData.emplace_back(pi0o, kRed - 4, kRed + 2, 1, -1, "#pi^{0} - o.");
  drawData.emplace_back(gpi, kOrange + 7, kOrange + 4, 2, -1, "#gamma - #pi^{#pm}_{misid}");
  drawData.emplace_back(go, kAzure + 2, kAzure + 3, 2, -1, "#gamma - o.");
  drawData.emplace_back(pipi, kOrange - 2, kOrange - 3, 4, 3112, "#pi^{#pm}_{misid} - #pi^{#pm}_{misid}");
  drawData.emplace_back(pio, kMagenta - 3, kMagenta - 2, 4, 3018, "#pi^{#pm}_{misid} - o.");
  drawData.emplace_back(oo, -1, kRed + 2, 4, -1, "o. - o.");

  string cName = "minvBgSrc/minvBgSrc";
  fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1000, 1000);

  TH1D* h0     = nullptr;
  TLegend* leg = new TLegend(0.65, 0.55, 0.95, 0.95);
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
  DrawH1({physBg, cPi, rest}, {"Phys. BG", "BG w. misid. #pi^{#pm}", "Rest"}, kLinear, kLog, true, 0.7, 0.8, 0.95, 0.91,
         "p");


  //check if all bg pair combinations are considered
  TH1D* bgRat = static_cast<TH1D*>(physBg->Clone());
  bgRat->Add(cPi);
  bgRat->Add(rest);

  int nBins = bgRat->GetNbinsX();
  for (int i = 1; i <= nBins; i++) {
    if (fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->GetBinContent(i) == 0) bgRat->SetBinContent(i, 2);
    else {
      double r = bgRat->GetBinContent(i) / fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->GetBinContent(i);
      bgRat->SetBinContent(i, r);
    }
  }

  bgRat->SetMinimum(0.9);
  bgRat->SetMaximum(2.1);

  fHMean.fHM.CreateCanvas((cName + "_compWithBg").c_str(), (cName + "_compWithBg").c_str(), 800, 800);
  DrawH1(bgRat, kLinear, kLinear, "hist");
}

void LmvmDrawAll::DrawMinvPtAll()
{
  for (const ELmvmAnaStep step : fHMean.fAnaSteps) {
    string name = fHMean.GetName("minvPt/lmvmAll_minvPt", step);
    fHMean.fHM.CreateCanvas(name.c_str(), name.c_str(), 1000, 1000);
    DrawH2(GetCocktailMinv<TH2D>("hMinvPt", step), kLinear, kLinear, kLog, "colz");
  }
}

void LmvmDrawAll::DrawMinvCombBgAndSignal()
{
  string folder     = "CombinatorialBackground/";
  string folderUAll = folder + "UrQMD/All/";
  string folderUEl  = folder + "UrQMD/Electrons/";

  // Draw PM, MM, PP in one plot
  {
    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
      for (const string& ev : {"sameEv", "mixedEv"}) {
        string cName = (cat == "_urqmdAll")  ? folderUAll + "PairYields_" + ev
                       : (cat == "_urqmdEl") ? folderUEl + "PairYields_" + ev
                                             : folder + "PairYields_" + ev;
        TCanvas* c   = fHMean.fHM.CreateCanvas(cName.c_str(), cName.c_str(), 1800, 1800);
        c->Divide(3, 3);
        int i = 1;
        for (auto step : fHMean.fAnaSteps) {
          if (step < ELmvmAnaStep::ElId) continue;
          c->cd(i++);
          TH1D* pp = fHMean.H1Clone("hMinvCombPP" + cat + "_" + ev, step);
          TH1D* mm = fHMean.H1Clone("hMinvCombMM" + cat + "_" + ev, step);
          TH1D* pm = fHMean.H1Clone("hMinvCombPM" + cat + "_" + ev, step);
          pm->GetYaxis()->SetTitle("Yield");
          if (ev == "sameEv") pm->GetYaxis()->SetRangeUser(1e-6, 2e-2);
          DrawH1({pm, pp, mm}, {"e+e-", "e+e+", "e-e-"}, kLinear, kLog, true, 0.85, 0.7, 0.99, 0.99, "HIST");
          fHMean.DrawAnaStepOnPad(step);
        }
      }
    }
  }

  // draw MM/PP ratio for same events
  {
    for (const string& cat : {"", "_urqmdEl"}) {
      string cName = (cat == "_urqmdEl") ? folderUEl : folder;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName + "RatioMMPP_same", cName + "RatioMMPP_same", 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* pp = fHMean.H1Clone("hMinvCombPP" + cat + "_sameEv", step);
        TH1D* mm = fHMean.H1Clone("hMinvCombMM" + cat + "_sameEv", step);
        mm->Divide(pp);
        mm->GetYaxis()->SetTitle("Ratio e^{-}e^{-}/e^{+}e^{+}");
        DrawH1(mm, kLinear, kLinear, "hist");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  //Draw Geometric Mean for same and mixed (mixed normalized)
  {
    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
      string cName = (cat == "_urqmdAll") ? folderUAll : (cat == "_urqmdEl") ? folderUEl : folder;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName + "geomMean", cName + "geomMean", 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* same    = fHMean.H1Clone("hMinvCombGeom" + cat + "_sameEv", step);
        TH1D* mixed   = fHMean.H1Clone("hMinvCombGeom" + cat + "_mixedEv", step);
        int minBin    = same->FindBin(0.4);
        int maxBin    = same->FindBin(0.7);
        double scale  = same->Integral(minBin, maxBin) / mixed->Integral(minBin, maxBin);
        mixed->Scale(scale);
        same->GetXaxis()->SetTitle("M_{ee} [GeV/c^{2}]");
        same->SetMinimum(1e-8);
        mixed->SetMinimum(1e-8);
        DrawH1({same, mixed}, {"geom. mean (same)", "geom. mean (mixed)"}, kLinear, kLog, true, 0.52, 0.8, 0.99, 0.99,
               "p");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  // Draw k factor
  {  // draw all three k factors in one histo  // TODO: keep this or next method or both?
    ELmvmAnaStep step = ELmvmAnaStep::ElId;
    fHMean.fHM.CreateCanvas(folder + "k_all", folder + "k_all", 1800, 1800);
    TH1D* kAll  = fHMean.H1Clone("hMinvCombK", step);
    TH1D* kUAll = fHMean.H1Clone("hMinvCombK_urqmdAll", step);
    TH1D* kUEl  = fHMean.H1Clone("hMinvCombK_urqmdEl", step);
    DrawH1({kAll, kUAll, kUEl}, {"k (all)", "k (UrQMD all)", "k (UrQMD El"}, kLinear, kLinear, true, 0.65, 0.8, 0.99,
           0.95, "p");
    kAll->GetYaxis()->SetTitle("k Factor");
    kAll->GetYaxis()->SetRangeUser(0.8, 1.2);
    fHMean.DrawAnaStepOnPad(step);
  }

  // Compare N+-, BG+Coc, BG and CB after ElID step
  {
    ELmvmAnaStep step = ELmvmAnaStep::ElId;

    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
      string cName = (cat == "_urqmdAll") ? folderUAll : (cat == "_urqmdEl") ? folderUEl : folder;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName + "N+-VsBgCoc", cName + "N+-VsBgCoc", 1800, 900);
      c->Divide(2, 1);
      TH1D* npm      = fHMean.H1Clone("hMinvCombPM" + cat + "_sameEv", step);
      TH1D* cb       = fHMean.H1Clone("hMinvCombBg" + cat, step);
      TH1D* bg       = fHMean.H1Clone("hMinv" + cat, ELmvmSrc::Bg, step);
      TH1D* cocktail = GetCocktailMinvH1("hMinv" + cat, step);
      TH1D* sbg      = static_cast<TH1D*>(bg->Clone());
      sbg->Add(cocktail);
      TH1D* ratS = (TH1D*) npm->Clone();
      ratS->Divide(sbg);
      TH1D* ratB = (TH1D*) cb->Clone();
      ratB->Divide(bg);
      c->cd(1);
      npm->GetYaxis()->SetRangeUser(1e-6, 2e-2);
      DrawH1({npm, sbg, cb, bg}, {"N^{+-}_{same}", "BG + Cocktail", "CB_{calc}", "Background"}, kLinear, kLog, true,
             0.7, 0.7, 0.99, 0.95, "p");
      fHMean.DrawAnaStepOnPad(step);
      c->cd(2);
      ratS->GetYaxis()->SetTitle("Ratio");
      ratS->GetYaxis()->SetRangeUser(0., 2.);
      ratB->GetYaxis()->SetRangeUser(0., 2.);
      DrawH1({ratS, ratB}, {"N^{+-}_{same} / BG+Coc", "CB_{calc} / BG"}, kLinear, kLinear, true, 0.7, 0.8, 0.99, 0.95,
             "p");
    }
  }

  // Draw Ratios N+-/CocBg and CB/BG for all three categories
  {
    ELmvmAnaStep step = ELmvmAnaStep::ElId;

    TH1D* rsAll  = fHMean.H1Clone("hMinvCombPM_sameEv", step);
    TH1D* rsUAll = fHMean.H1Clone("hMinvCombPM_urqmdAll_sameEv", step);
    TH1D* rsUEl  = fHMean.H1Clone("hMinvCombPM_urqmdEl_sameEv", step);

    TH1D* cbgAll = GetCocktailMinvH1("hMinv", step);
    cbgAll->Add(fHMean.H1("hMinv", ELmvmSrc::Bg, step));
    TH1D* cbgUAll = GetCocktailMinvH1("hMinv_urqmdAll", step);
    cbgUAll->Add(fHMean.H1("hMinv_urqmdAll", ELmvmSrc::Bg, step));
    TH1D* cbgUEl = GetCocktailMinvH1("hMinv_urqmdEl", step);
    cbgUEl->Add(fHMean.H1("hMinv_urqmdEl", ELmvmSrc::Bg, step));

    rsAll->Divide(cbgAll);
    rsUAll->Divide(cbgUAll);
    rsUEl->Divide(cbgUEl);

    TH1D* rbAll  = fHMean.H1Clone("hMinvCombBg", step);
    TH1D* rbUAll = fHMean.H1Clone("hMinvCombBg_urqmdAll", step);
    TH1D* rbUEl  = fHMean.H1Clone("hMinvCombBg_urqmdEl", step);

    rbAll->Divide(fHMean.H1("hMinv", ELmvmSrc::Bg, step));
    rbUAll->Divide(fHMean.H1("hMinv_urqmdAll", ELmvmSrc::Bg, step));
    rbUEl->Divide(fHMean.H1("hMinv_urqmdEl", ELmvmSrc::Bg, step));

    rsAll->GetYaxis()->SetTitle("N^{+-}_{same} / BG+Coc");
    rsAll->GetYaxis()->SetRangeUser(0., 2.);
    rbAll->GetYaxis()->SetTitle("CB_{calc} / BG");
    rbAll->GetYaxis()->SetRangeUser(0., 2.);

    TCanvas* c = fHMean.fHM.CreateCanvas(folder + "CbVsMc_ratio", folder + "CbVsMc_ratio", 1800, 900);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1({rsAll, rsUAll, rsUEl}, {"UrQMD + PLUTO", "UrQMD", "UrQMD electrons"}, kLinear, kLinear, true, 0.55, 0.78,
           0.99, 0.95, "hist");
    fHMean.DrawAnaStepOnPad(step);
    c->cd(2);
    DrawH1({rbAll, rbUAll, rbUEl}, {"UrQMD + PLUTO", "UrQMD", "UrQMD electrons"}, kLinear, kLinear, true, 0.55, 0.78,
           0.99, 0.95, "hist");
    fHMean.DrawAnaStepOnPad(step);
  }

  // Draw common CB vs. CB from pure UrQMD electrons
  {
    TCanvas* c = fHMean.fHM.CreateCanvas(folder + "CbVsUrqmdCb", folder + "CbVsUrqmdCb", 1800, 1800);
    c->Divide(3, 3);
    int i = 1;
    for (auto step : fHMean.fAnaSteps) {
      if (step < ELmvmAnaStep::ElId) continue;
      c->cd(i++);
      TH1D* cb  = fHMean.H1Clone("hMinvCombBg", step);
      TH1D* cbU = fHMean.H1Clone("hMinvCombBg_urqmdEl", step);
      DrawH1({cb, cbU}, {"B_{C}", "B_{C} from UrQMD electrons"}, kLinear, kLog, true, 0.55, 0.8, 0.99, 0.95);
      fHMean.DrawAnaStepOnPad(step);
    }
  }

  //Draw Combinatorial Signal with N+-same, CB, Cocktail
  {
    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
      string cName = (cat == "_urqmdEl") ? folderUEl : (cat == "_urqmdAll") ? folderUAll : folder;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName + "CbVsInput_Npm", cName + "CbVsInput_Npm", 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* pmSame        = fHMean.H1Clone("hMinvCombPM" + cat + "_sameEv", step);
        TH1D* combBg        = fHMean.H1Clone("hMinvCombBg" + cat, step);
        TH1D* combSignalNpm = fHMean.H1Clone("hMinvCombSignalNpm" + cat, step);
        TH1D* cocktail      = GetCocktailMinvH1("hMinv" + cat, step);
        pmSame->SetMaximum(1e-2);
        pmSame->SetMinimum(4e-9);
        DrawH1({pmSame, combBg, combSignalNpm, cocktail},
               {"N_{same}^{+-}", "B_{c}", "Signal (N_{same}^{+-} - B_{c})", "Cocktail"}, kLinear, kLog, true, 0.53,
               0.75, 0.99, 0.92, "p");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }

  //Draw Combinatorial Signal from Cocktail+BG
  {
    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {
      string cName = (cat == "_urqmdEl") ? folderUEl : (cat == "_urqmdAll") ? folderUAll : folder;
      TCanvas* c   = fHMean.fHM.CreateCanvas(cName + "CbVsInput_cocBg", cName + "CbVsInput_cocBg", 1800, 1800);
      c->Divide(3, 3);
      int i = 1;
      for (auto step : fHMean.fAnaSteps) {
        if (step < ELmvmAnaStep::ElId) continue;
        c->cd(i++);
        TH1D* sbg2 = fHMean.H1Clone("hMinv" + cat, ELmvmSrc::Bg, step);
        sbg2->Add(GetCocktailMinvH1("hMinv" + cat, step));
        TH1D* combBg    = fHMean.H1Clone("hMinvCombBg" + cat, step);
        TH1D* sBgSignal = fHMean.H1Clone("hMinvCombSignalMc" + cat, step);
        sbg2->SetMaximum(2e-2);
        sbg2->SetMinimum(4e-9);
        TH1D* cocktail = GetCocktailMinvH1("hMinv" + cat, step);
        DrawH1({sbg2, combBg, sBgSignal, cocktail}, {"Cocktail + BG", "B_{C}", "Signal (Cock+BG - B_{C})", "Cocktail"},
               kLinear, kLog, true, 0.53, 0.72, 0.99, 0.92, "p");
        fHMean.DrawAnaStepOnPad(step);
      }
    }
  }
}

void LmvmDrawAll::DrawSBgVsMinv()
{
  vector<TH1*> hists;
  for (ELmvmAnaStep step : {ELmvmAnaStep::TtCut, ELmvmAnaStep::PtCut}) {
    TH1D* bg = fHMean.H1Clone("hMinv", ELmvmSrc::Bg, step);
    TH1D* cocktail       = GetCocktailMinvH1("hMinv", step);
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
    for (const string& cat : {"", "_urqmdEl", "_urqmdAll"}) {  // common CB or from pure UrQMD (all or electrons)
      if (step < ELmvmAnaStep::ElId) continue;

      // Geometric mean
      for (const string& ev : {"_sameEv", "_mixedEv"}) {
        TH1D* pp   = fHMean.H1Clone("hMinvCombPP" + cat + ev, step);
        TH1D* mm   = fHMean.H1Clone("hMinvCombMM" + cat + ev, step);
        TH1D* geom = fHMean.CreateHByClone<TH1D>("hMinvCombMM" + cat + ev, "hMinvCombGeom" + cat + ev, step);

        for (int i = 1; i <= geom->GetNbinsX(); i++) {
          double cpp     = pp->GetBinContent(i);
          double cmm     = mm->GetBinContent(i);
          double content = std::sqrt(cpp * cmm);
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
      TH1D* hBc = fHMean.CreateHByClone<TH1D>("hMinvCombGeom" + cat + "_sameEv", "hMinvCombBg" + cat, step);
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
      TH1D* hSigCoc = fHMean.CreateHByClone<TH1D>("hMinv" + cat + "_bg", "hMinvCombSignalMc" + cat, step);
      hSigCoc->Add(GetCocktailMinvH1("hMinv" + cat, step));
      hSigCoc->Add(fHMean.H1("hMinvCombBg" + cat, step), -1.);

      // calculate errors via error propagation formula
      int nofEvents = GetNofTotalEvents();
      double bW     = fHMean.H1("hMinvCombPM" + cat + "_sameEv", step)->GetBinWidth(1);
      int nofBins   = fHMean.H1("hMinvCombPM" + cat + "_sameEv", step)->GetNbinsX();
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
        double d_s_pp = 0.5 * m_pm * s_mm / std::sqrt(m_pp * m_mm * s_pp * s_mm);
        double d_s_mm = 0.5 * m_pm * s_pp / std::sqrt(m_pp * m_mm * s_pp * s_mm);

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
        if (i > minBin) fHMean.H1("hMinvCombBg" + cat, step)->SetBinError(i, errorBc2);
        if (i <= minBin) fHMean.H1("hMinvCombSignalNpm" + cat, step)->SetBinError(i, errorSig);
        if (i > minBin) fHMean.H1("hMinvCombSignalNpm" + cat, step)->SetBinError(i, errorSig2);
        if (i <= minBin) fHMean.H1("hMinvCombSignalMc" + cat, step)->SetBinError(i, errorSig);
        if (i > minBin) fHMean.H1("hMinvCombSignalMc" + cat, step)->SetBinError(i, errorSig2);
      }  // error calculation
    }    // cat ("" or "_urqmdEl")
  }      // steps
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
  TH1D* cocktail   = GetCocktailMinvH1("hMinv", ELmvmAnaStep::ElId);
  int binMin       = cocktail->FindBin(minMinv);
  int binMax       = cocktail->FindBin(maxMinv);
  double sIntElId  = cocktail->Integral(binMin, binMax);
  double bgIntElId = fHMean.H1("hMinv", ELmvmSrc::Bg, ELmvmAnaStep::ElId)->Integral(binMin, binMax);
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    if (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut)) continue;

    double effS = 100. * GetCocktailMinvH1("hMinv", step)->Integral(binMin, binMax) / sIntElId;
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
  TH1D* cocktail = GetCocktailMinvH1("hMinv", ELmvmAnaStep::ElId);
  int binMin     = cocktail->FindBin(minMinv);
  int binMax     = cocktail->FindBin(maxMinv);
  for (ELmvmAnaStep step : fHMean.fAnaSteps) {
    if (step < ELmvmAnaStep::ElId) continue;
    if (!fUseMvd && (step == ELmvmAnaStep::Mvd1Cut || step == ELmvmAnaStep::Mvd2Cut)) continue;

    double intS  = 100. * GetCocktailMinvH1("hMinv", step)->Integral(binMin, binMax);
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
