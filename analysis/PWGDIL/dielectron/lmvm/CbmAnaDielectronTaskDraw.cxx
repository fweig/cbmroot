/* Copyright (C) 2011-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Elena Lebedeva */

/** CbmAnaDielectronTaskDraw.cxx
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2011
 * @version 1.0
 **/

#include "CbmAnaDielectronTaskDraw.h"

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

ClassImp(CbmAnaDielectronTaskDraw);

using namespace std;
using namespace Cbm;
CbmAnaDielectronTaskDraw::CbmAnaDielectronTaskDraw()
  : TObject()
  , fNofEvents(0)
  , fUseMvd(kFALSE)
  , fDrawSignificance(kFALSE)
  , fCuts()
  , fHM(NULL)
  , fOutputDir("")
{
}

void CbmAnaDielectronTaskDraw::DrawHistFromFile(const string& fileName, const string& outputDir, Bool_t useMvd,
                                                Bool_t drawSig)
{
  SetDefaultDrawStyle();
  fOutputDir        = outputDir;
  fUseMvd           = useMvd;
  fDrawSignificance = drawSig;

  fCuts.SetDefaultCuts();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);
  fNofEvents = (Int_t) H1("fh_event_number")->GetEntries();
  cout << "File name = " << fileName << endl;
  cout << "Number of events = " << fNofEvents << endl;

  fHM->ScaleByPattern(".*", 1. / fNofEvents);
  SOverBgAll();
  RebinMinvHist();
  if (!fUseMvd) RemoveMvdCutBins();
  DrawPtYDistributionAll();
  DrawPtYEfficiencyAll();
  DrawRapidityDistributionAll();
  DrawMomentumDistributionAll();
  DrawMomentumEfficiencyAll();
  DrawMotherPdg();
  DrawPPAngleMCSignal();
  DrawSourcesBgPairsAll();
  DrawGammaVertex();
  DrawCutDistributions();
  DrawMinvForEachAnalysisStep();
  DrawMinvSandBgAll();
  DrawMinvSourceAll();
  DrawBgSourceTracks();
  DrawMismatchesAndGhosts();
  DrawMinvPtAll();
  DrawBgSourcesVsMomentum();
  DrawMvdCutQa();
  DrawMvdAndStsHist();
  DrawElPiMomHis();
  DrawPmtXY();
  DrawMomLikeHist();
  DrawSingleParticleYield();
  SaveCanvasToImage();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmAnaDielectronTaskDraw::DrawMomLikeHist()
{
  TCanvas* c = fHM->CreateCanvas("lmvm_mom_likelihood", "lmvm_mom_likelihood", 1000, 500);

  c->Divide(2, 1);
  c->cd(1);
  DrawH2(H2("fh_mom_likelihood_El"));
  c->cd(2);
  DrawH2(H2("fh_mom_likelihood_Pi"));
}

void CbmAnaDielectronTaskDraw::DrawSingleParticleYield()
{
  // Draw 1D Histos: Yield vs. Momentum
  TCanvas* c2 = fHM->CreateCanvas("fh_mom_ElPos_pluto", "fh_mom_ElPos_pluto", 1000, 500);
  c2->Divide(2, 1);
  c2->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_nof_plutoElectrons", true);
  c2->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_nof_plutoPositrons", true);

  TCanvas* c3 = fHM->CreateCanvas("fh_mom_ElPos_urqmd", "fh_mom_ElPos_urqmd", 1000, 500);
  c3->Divide(2, 1);
  c3->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_nof_urqmdElectrons", true);
  c3->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_nof_urqmdPositrons", true);

  // draw electron and positron yield vs. momentum
  TH1D* nPlutoElMc    = (TH1D*) H1("fh_nof_plutoElectrons_mc")->Clone();
  TH1D* nPlutoPosMc   = (TH1D*) H1("fh_nof_plutoPositrons_mc")->Clone();
  TH1D* nPlutoElAcc   = (TH1D*) H1("fh_nof_plutoElectrons_acc")->Clone();
  TH1D* nPlutoPosAcc  = (TH1D*) H1("fh_nof_plutoPositrons_acc")->Clone();
  TH1D* nPlutoElReco  = (TH1D*) H1("fh_nof_plutoElectrons_reco")->Clone();
  TH1D* nPlutoPosReco = (TH1D*) H1("fh_nof_plutoPositrons_reco")->Clone();
  TH1D* nPlutoElElid  = (TH1D*) H1("fh_nof_plutoElectrons_elid")->Clone();
  TH1D* nPlutoPosElid = (TH1D*) H1("fh_nof_plutoPositrons_elid")->Clone();
  TH1D* nPlutoElTt    = (TH1D*) H1("fh_nof_plutoElectrons_ttcut")->Clone();
  TH1D* nPlutoPosTt   = (TH1D*) H1("fh_nof_plutoPositrons_ttcut")->Clone();
  TH1D* nPlutoElPt    = (TH1D*) H1("fh_nof_plutoElectrons_ptcut")->Clone();
  TH1D* nPlutoPosPt   = (TH1D*) H1("fh_nof_plutoPositrons_ptcut")->Clone();
  TH1D* nUrqmdElMc    = (TH1D*) H1("fh_nof_urqmdElectrons_mc")->Clone();
  TH1D* nUrqmdPosMc   = (TH1D*) H1("fh_nof_urqmdPositrons_mc")->Clone();
  TH1D* nUrqmdElAcc   = (TH1D*) H1("fh_nof_urqmdElectrons_acc")->Clone();
  TH1D* nUrqmdPosAcc  = (TH1D*) H1("fh_nof_urqmdPositrons_acc")->Clone();
  TH1D* nUrqmdElReco  = (TH1D*) H1("fh_nof_urqmdElectrons_reco")->Clone();
  TH1D* nUrqmdPosReco = (TH1D*) H1("fh_nof_urqmdPositrons_reco")->Clone();
  TH1D* nUrqmdElElid  = (TH1D*) H1("fh_nof_urqmdElectrons_elid")->Clone();
  TH1D* nUrqmdPosElid = (TH1D*) H1("fh_nof_urqmdPositrons_elid")->Clone();
  TH1D* nUrqmdElTt    = (TH1D*) H1("fh_nof_urqmdElectrons_ttcut")->Clone();
  TH1D* nUrqmdPosTt   = (TH1D*) H1("fh_nof_urqmdPositrons_ttcut")->Clone();
  TH1D* nUrqmdElPt    = (TH1D*) H1("fh_nof_urqmdElectrons_ptcut")->Clone();
  TH1D* nUrqmdPosPt   = (TH1D*) H1("fh_nof_urqmdPositrons_ptcut")->Clone();

  double min1 = 5e-8;
  double max1 = 50;
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

  fHM->CreateCanvas("fh_nof_plutoElPos", "fh_nof_plutoElPos", 800, 800);
  DrawH1({nPlutoElMc, nPlutoPosMc, nPlutoElAcc, nPlutoPosAcc, nPlutoElReco, nPlutoPosReco}, {"", "", "", "", "", ""},
         kLinear, kLog, false, 0, 0, 0, 0, "Hist p");

  TLegend* legendNofPP = new TLegend(0.65, 0.6, 0.88, 0.93);
  legendNofPP->SetFillColor(kWhite);
  legendNofPP->AddEntry(nPlutoElMc, "electrons kMc");
  legendNofPP->AddEntry(nPlutoPosMc, "positrons kMc");
  legendNofPP->AddEntry(nPlutoElAcc, "electrons kAcc");
  legendNofPP->AddEntry(nPlutoPosAcc, "positrons kAcc");
  legendNofPP->AddEntry(nPlutoElReco, "electrons kReco");
  legendNofPP->AddEntry(nPlutoPosReco, "positrons kReco");
  legendNofPP->Draw();

  fHM->CreateCanvas("fh_nof_urqmdElPos", "fh_nof_urqmdElPos", 800, 800);
  DrawH1({nUrqmdElMc, nUrqmdPosMc, nUrqmdElAcc, nUrqmdPosAcc, nUrqmdElReco, nUrqmdPosReco}, {"", "", "", "", "", ""},
         kLinear, kLog, false, 0, 0, 0, 0, "Hist p");

  TLegend* legendNofUP = new TLegend(0.65, 0.6, 0.88, 0.93);
  legendNofUP->SetFillColor(kWhite);
  legendNofUP->AddEntry(nUrqmdElMc, "electrons kMc");
  legendNofUP->AddEntry(nUrqmdPosMc, "positrons kMc");
  legendNofUP->AddEntry(nUrqmdElAcc, "electrons kAcc");
  legendNofUP->AddEntry(nUrqmdPosAcc, "positrons kAcc");
  legendNofUP->AddEntry(nUrqmdElReco, "electrons kReco");
  legendNofUP->AddEntry(nUrqmdPosReco, "positrons kReco");
  legendNofUP->Draw();

  // Acc. of single particle yield vs. momentum for various detector combinations
  TH1D* nPlutoElMc2   = (TH1D*) H1("fh_nof_particles_acc_pEl_mc")->Clone();
  TH1D* nPlutoPosMc2  = (TH1D*) H1("fh_nof_particles_acc_pPos_mc")->Clone();
  TH1D* nPlutoElSts   = (TH1D*) H1("fh_nof_particles_acc_pEl_sts")->Clone();
  TH1D* nPlutoPosSts  = (TH1D*) H1("fh_nof_particles_acc_pPos_sts")->Clone();
  TH1D* nPlutoElRich  = (TH1D*) H1("fh_nof_particles_acc_pEl_rich")->Clone();
  TH1D* nPlutoPosRich = (TH1D*) H1("fh_nof_particles_acc_pPos_rich")->Clone();
  TH1D* nPlutoElTrd   = (TH1D*) H1("fh_nof_particles_acc_pEl_trd")->Clone();
  TH1D* nPlutoPosTrd  = (TH1D*) H1("fh_nof_particles_acc_pPos_trd")->Clone();
  TH1D* nPlutoElTof   = (TH1D*) H1("fh_nof_particles_acc_pEl_tof")->Clone();
  TH1D* nPlutoPosTof  = (TH1D*) H1("fh_nof_particles_acc_pPos_tof")->Clone();
  TH1D* nUrqmdElMc2   = (TH1D*) H1("fh_nof_particles_acc_uEl_mc")->Clone();
  TH1D* nUrqmdPosMc2  = (TH1D*) H1("fh_nof_particles_acc_uPos_mc")->Clone();
  TH1D* nUrqmdElSts   = (TH1D*) H1("fh_nof_particles_acc_uEl_sts")->Clone();
  TH1D* nUrqmdPosSts  = (TH1D*) H1("fh_nof_particles_acc_uPos_sts")->Clone();
  TH1D* nUrqmdElRich  = (TH1D*) H1("fh_nof_particles_acc_uEl_rich")->Clone();
  TH1D* nUrqmdPosRich = (TH1D*) H1("fh_nof_particles_acc_uPos_rich")->Clone();
  TH1D* nUrqmdElTrd   = (TH1D*) H1("fh_nof_particles_acc_uEl_trd")->Clone();
  TH1D* nUrqmdPosTrd  = (TH1D*) H1("fh_nof_particles_acc_uPos_trd")->Clone();
  TH1D* nUrqmdElTof   = (TH1D*) H1("fh_nof_particles_acc_uEl_tof")->Clone();
  TH1D* nUrqmdPosTof  = (TH1D*) H1("fh_nof_particles_acc_uPos_tof")->Clone();

  int n         = 1;
  int nRebinAcc = 4;
  nPlutoElMc2->Rebin(n * nRebinAcc);
  nPlutoElMc2->Scale(1. / (n * nRebinAcc));
  nPlutoPosMc2->Rebin(n * nRebinAcc);
  nPlutoPosMc2->Scale(1. / (n * nRebinAcc));
  nPlutoElSts->Rebin(n * nRebinAcc);
  nPlutoElSts->Scale(1. / (n * nRebinAcc));
  nPlutoPosSts->Rebin(n * nRebinAcc);
  nPlutoPosSts->Scale(1. / (n * nRebinAcc));
  nPlutoElRich->Rebin(n * nRebinAcc);
  nPlutoElRich->Scale(1. / (n * nRebinAcc));
  nPlutoPosRich->Rebin(n * nRebinAcc);
  nPlutoPosRich->Scale(1. / (n * nRebinAcc));
  nPlutoElTrd->Rebin(n * nRebinAcc);
  nPlutoElTrd->Scale(1. / (n * nRebinAcc));
  nPlutoPosTrd->Rebin(n * nRebinAcc);
  nPlutoPosTrd->Scale(1. / (n * nRebinAcc));
  nPlutoElTof->Rebin(n * nRebinAcc);
  nPlutoElTof->Scale(1. / (n * nRebinAcc));
  nPlutoPosTof->Rebin(n * nRebinAcc);
  nPlutoPosTof->Scale(1. / (n * nRebinAcc));

  nUrqmdElMc2->Rebin(n * nRebinAcc);
  nUrqmdElMc2->Scale(1. / (n * nRebinAcc));
  nUrqmdPosMc2->Rebin(n * nRebinAcc);
  nUrqmdPosMc2->Scale(1. / (n * nRebinAcc));
  nUrqmdElSts->Rebin(n * nRebinAcc);
  nUrqmdElSts->Scale(1. / (n * nRebinAcc));
  nUrqmdPosSts->Rebin(n * nRebinAcc);
  nUrqmdPosSts->Scale(1. / (n * nRebinAcc));
  nUrqmdElRich->Rebin(n * nRebinAcc);
  nUrqmdElRich->Scale(1. / (n * nRebinAcc));
  nUrqmdPosRich->Rebin(n * nRebinAcc);
  nUrqmdPosRich->Scale(1. / (n * nRebinAcc));
  nUrqmdElTrd->Rebin(n * nRebinAcc);
  nUrqmdElTrd->Scale(1. / (n * nRebinAcc));
  nUrqmdPosTrd->Rebin(n * nRebinAcc);
  nUrqmdPosTrd->Scale(1. / (n * nRebinAcc));
  nUrqmdElTof->Rebin(n * nRebinAcc);
  nUrqmdElTof->Scale(1. / (n * nRebinAcc));
  nUrqmdPosTof->Rebin(n * nRebinAcc);
  nUrqmdPosTof->Scale(1. / (n * nRebinAcc));

  nPlutoElMc2->SetMinimum(min1);
  nPlutoElMc2->SetMaximum(max1);
  nPlutoPosMc2->SetMinimum(min1);
  nPlutoPosMc2->SetMaximum(max1);
  nPlutoElSts->SetMinimum(min1);
  nPlutoElSts->SetMaximum(max1);
  nPlutoPosSts->SetMinimum(min1);
  nPlutoPosSts->SetMaximum(max1);
  nPlutoElRich->SetMinimum(min1);
  nPlutoElRich->SetMaximum(max1);
  nPlutoPosRich->SetMinimum(min1);
  nPlutoPosRich->SetMaximum(max1);
  nPlutoElTrd->SetMinimum(min1);
  nPlutoElTrd->SetMaximum(max1);
  nPlutoPosTrd->SetMinimum(min1);
  nPlutoPosTrd->SetMaximum(max1);
  nPlutoElTof->SetMinimum(min1);
  nPlutoElTof->SetMaximum(max1);
  nPlutoPosTof->SetMinimum(min1);
  nPlutoPosTof->SetMaximum(max1);

  nUrqmdElMc2->SetMinimum(min1);
  nUrqmdElMc2->SetMaximum(max1);
  nUrqmdPosMc2->SetMinimum(min1);
  nUrqmdPosMc2->SetMaximum(max1);
  nUrqmdElSts->SetMinimum(min1);
  nUrqmdElSts->SetMaximum(max1);
  nUrqmdPosSts->SetMinimum(min1);
  nUrqmdPosSts->SetMaximum(max1);
  nUrqmdElRich->SetMinimum(min1);
  nUrqmdElRich->SetMaximum(max1);
  nUrqmdPosRich->SetMinimum(min1);
  nUrqmdPosRich->SetMaximum(max1);
  nUrqmdElTrd->SetMinimum(min1);
  nUrqmdElTrd->SetMaximum(max1);
  nUrqmdPosTrd->SetMinimum(min1);
  nUrqmdPosTrd->SetMaximum(max1);
  nUrqmdElTof->SetMinimum(min1);
  nUrqmdElTof->SetMaximum(max1);
  nUrqmdPosTof->SetMinimum(min1);
  nUrqmdPosTof->SetMaximum(max1);

  nPlutoElMc2->GetYaxis()->SetTitle("Yield");
  nPlutoElMc2->SetTitle("Acceptance PLUTO particles");
  nUrqmdElMc2->GetYaxis()->SetTitle("Yield");
  nUrqmdElMc2->SetTitle("Acceptance UrQMD particles");

  fHM->CreateCanvas("fh_nof_pluto_det", "fh_nof_pluto_det", 800, 800);
  DrawH1({nPlutoElMc2, nPlutoPosMc2, nPlutoElSts, nPlutoPosSts, nPlutoElRich, nPlutoPosRich, nPlutoElTrd, nPlutoPosTrd,
          nPlutoElTof, nPlutoPosTof},
         {"", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofPP2 = new TLegend(0.45, 0.6, 0.88, 0.93);
  legendNofPP2->SetFillColor(kWhite);
  legendNofPP2->AddEntry(nPlutoElMc2, "electrons kMc");
  legendNofPP2->AddEntry(nPlutoPosMc2, "positrons kMc");
  legendNofPP2->AddEntry(nPlutoElSts, "electrons kAcc STS");
  legendNofPP2->AddEntry(nPlutoPosSts, "positrons kAcc STS");
  legendNofPP2->AddEntry(nPlutoElRich, "electrons kAcc STS + RICH");
  legendNofPP2->AddEntry(nPlutoPosRich, "positrons kAcc STS + RICH");
  legendNofPP2->AddEntry(nPlutoElTrd, "electrons kAcc STS + RICH + TRD");
  legendNofPP2->AddEntry(nPlutoPosTrd, "positrons kAcc STS + RICH + TRD");
  legendNofPP2->AddEntry(nPlutoElTof, "electrons kAcc STS + RICH + TRD + ToF");
  legendNofPP2->AddEntry(nPlutoPosTof, "positrons kAcc STS + RICH + TRD + ToF");
  legendNofPP2->Draw();

  fHM->CreateCanvas("fh_nof_urqmd_det", "fh_nof_urqmd_det", 800, 800);
  DrawH1({nUrqmdElMc2, nUrqmdPosMc2, nUrqmdElSts, nUrqmdPosSts, nUrqmdElRich, nUrqmdPosRich, nUrqmdElTrd, nUrqmdPosTrd,
          nUrqmdElTof, nUrqmdPosTof},
         {"", "", "", "", "", "", "", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofUP2 = new TLegend(0.45, 0.6, 0.88, 0.93);
  legendNofUP2->SetFillColor(kWhite);
  legendNofUP2->AddEntry(nUrqmdElMc2, "electrons kMc");
  legendNofUP2->AddEntry(nUrqmdPosMc2, "positrons kMc");
  legendNofUP2->AddEntry(nUrqmdElSts, "electrons kAcc STS");
  legendNofUP2->AddEntry(nUrqmdPosSts, "positrons kAcc STS");
  legendNofUP2->AddEntry(nUrqmdElRich, "electrons kAcc STS + RICH");
  legendNofUP2->AddEntry(nUrqmdPosRich, "positrons kAcc STS + RICH");
  legendNofUP2->AddEntry(nUrqmdElTrd, "electrons kAcc STS + RICH + TRD");
  legendNofUP2->AddEntry(nUrqmdPosTrd, "positrons kAcc STS + RICH + TRD");
  legendNofUP2->AddEntry(nUrqmdElTof, "electrons kAcc STS + RICH + TRD + ToF");
  legendNofUP2->AddEntry(nUrqmdPosTof, "positrons kAcc STS + RICH + TRD + ToF");
  legendNofUP2->Draw();

  // Draw nofPoints of electrons and positrons in various detectors
  TH1D* pElSts   = (TH1D*) H1("fh_nof_points_pEl_sts")->Clone();
  TH1D* pPosSts  = (TH1D*) H1("fh_nof_points_pPos_sts")->Clone();
  TH1D* pElRich  = (TH1D*) H1("fh_nof_points_pEl_rich")->Clone();
  TH1D* pPosRich = (TH1D*) H1("fh_nof_points_pPos_rich")->Clone();
  TH1D* pElTrd   = (TH1D*) H1("fh_nof_points_pEl_trd")->Clone();
  TH1D* pPosTrd  = (TH1D*) H1("fh_nof_points_pPos_trd")->Clone();
  TH1D* pElTof   = (TH1D*) H1("fh_nof_points_pEl_tof")->Clone();
  TH1D* pPosTof  = (TH1D*) H1("fh_nof_points_pPos_tof")->Clone();
  TH1D* uElSts   = (TH1D*) H1("fh_nof_points_uEl_sts")->Clone();
  TH1D* uPosSts  = (TH1D*) H1("fh_nof_points_uPos_sts")->Clone();
  TH1D* uElRich  = (TH1D*) H1("fh_nof_points_uEl_rich")->Clone();
  TH1D* uPosRich = (TH1D*) H1("fh_nof_points_uPos_rich")->Clone();
  TH1D* uElTrd   = (TH1D*) H1("fh_nof_points_uEl_trd")->Clone();
  TH1D* uPosTrd  = (TH1D*) H1("fh_nof_points_uPos_trd")->Clone();
  TH1D* uElTof   = (TH1D*) H1("fh_nof_points_uEl_tof")->Clone();
  TH1D* uPosTof  = (TH1D*) H1("fh_nof_points_uPos_tof")->Clone();

  pElSts->SetTitle("STS");
  pElRich->SetTitle("RICH");
  pElTrd->SetTitle("TRD");
  pElTrd->GetXaxis()->SetRangeUser(0., 15.);
  pElTof->SetTitle("ToF");
  pElTof->GetXaxis()->SetRangeUser(0., 20.);

  fHM->CreateCanvas("fh_nof_points_sts", "fh_nof_points_sts", 800, 800);
  DrawH1({pElSts, pPosSts, uElSts, uPosSts}, {"", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofPSts = new TLegend(0.45, 0.7, 0.88, 0.93);
  legendNofPSts->SetFillColor(kWhite);
  legendNofPSts->AddEntry(pElSts, "PLUTO electrons");
  legendNofPSts->AddEntry(pPosSts, "PLUTO positrons");
  legendNofPSts->AddEntry(uElSts, "UrQMD electrons");
  legendNofPSts->AddEntry(uPosSts, "UrQMD positrons");
  legendNofPSts->Draw();

  fHM->CreateCanvas("fh_nof_points_rich", "fh_nof_points_rich", 800, 800);
  DrawH1({pElRich, pPosRich, uElRich, uPosRich}, {"", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99,
         "hist p");

  TLegend* legendNofPRich = new TLegend(0.45, 0.7, 0.88, 0.93);
  legendNofPRich->SetFillColor(kWhite);
  legendNofPRich->AddEntry(pElRich, "PLUTO electrons");
  legendNofPRich->AddEntry(pPosRich, "PLUTO positrons");
  legendNofPRich->AddEntry(uElRich, "UrQMD electrons");
  legendNofPRich->AddEntry(uPosRich, "UrQMD positrons");
  legendNofPRich->Draw();

  fHM->CreateCanvas("fh_nof_points_trd", "fh_nof_points_trd", 800, 800);
  DrawH1({pElTrd, pPosTrd, uElTrd, uPosTrd}, {"", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofPTrd = new TLegend(0.45, 0.7, 0.88, 0.93);
  legendNofPTrd->SetFillColor(kWhite);
  legendNofPTrd->AddEntry(pElTrd, "PLUTO electrons");
  legendNofPTrd->AddEntry(pPosTrd, "PLUTO positrons");
  legendNofPTrd->AddEntry(uElTrd, "UrQMD electrons");
  legendNofPTrd->AddEntry(uPosTrd, "UrQMD positrons");
  legendNofPTrd->Draw();

  fHM->CreateCanvas("fh_nof_points_tof", "fh_nof_points_tof", 800, 800);
  DrawH1({pElTof, pPosTof, uElTof, uPosTof}, {"", "", "", ""}, kLinear, kLog, false, 0.9, 0.8, 0.99, 0.99, "hist p");

  TLegend* legendNofPTof = new TLegend(0.45, 0.7, 0.88, 0.93);
  legendNofPTof->SetFillColor(kWhite);
  legendNofPTof->AddEntry(pElTof, "PLUTO electrons");
  legendNofPTof->AddEntry(pPosTof, "PLUTO positrons");
  legendNofPTof->AddEntry(uElTof, "UrQMD electrons");
  legendNofPTof->AddEntry(uPosTof, "UrQMD positrons");
  legendNofPTof->Draw();

  //Draw 2D Histos: Yield vs. Momentum and Pt
  TH2D* pElMc   = H2("fh_nof_plutoElectrons_p_pt_mc");
  TH2D* pPosMc  = H2("fh_nof_plutoPositrons_p_pt_mc");
  TH2D* pElAcc  = H2("fh_nof_plutoElectrons_p_pt_acc");
  TH2D* pPosAcc = H2("fh_nof_plutoPositrons_p_pt_acc");

  TH2D* uElMc   = H2("fh_nof_urqmdElectrons_p_pt_mc");
  TH2D* uPosMc  = H2("fh_nof_urqmdPositrons_p_pt_mc");
  TH2D* uElAcc  = H2("fh_nof_urqmdElectrons_p_pt_acc");
  TH2D* uPosAcc = H2("fh_nof_urqmdPositrons_p_pt_acc");

  TCanvas* cPMc = fHM->CreateCanvas("fh_nof_plutoParticles_p_pt_mc", "fh_nof_plutoParticles_p_pt_mc", 1000, 500);
  cPMc->Divide(2, 1);
  cPMc->cd(1);
  DrawH2(pElMc);
  pElMc->SetMinimum(1e-6);
  pElMc->SetMaximum(1e-1);
  gPad->SetLogz(true);
  cPMc->cd(2);
  DrawH2(pPosMc);
  pPosMc->SetMinimum(1e-6);
  pPosMc->SetMaximum(1e-1);
  gPad->SetLogz(true);

  TCanvas* cPAcc = fHM->CreateCanvas("fh_nof_plutoParticles_p_pt_acc", "fh_nof_plutoParticles_p_pt_acc", 1000, 500);
  cPAcc->Divide(2, 1);
  cPAcc->cd(1);
  DrawH2(pElAcc);
  pElAcc->SetMinimum(1e-6);
  pElAcc->SetMaximum(5e-2);
  gPad->SetLogz(true);
  cPAcc->cd(2);
  DrawH2(pPosAcc);
  pPosAcc->SetMinimum(1e-6);
  pPosAcc->SetMaximum(5e-2);
  gPad->SetLogz(true);

  TCanvas* cUMc = fHM->CreateCanvas("fh_nof_urqmdParticles_p_pt_mc", "fh_nof_urqmdParticles_p_pt_mc", 1000, 500);
  cUMc->Divide(2, 1);
  cUMc->cd(1);
  DrawH2(uElMc);
  uElMc->SetMinimum(1e-6);
  uElMc->SetMaximum(250);
  gPad->SetLogz(true);
  cUMc->cd(2);
  DrawH2(uPosMc);
  uPosMc->SetMinimum(1e-6);
  uPosMc->SetMaximum(250);
  gPad->SetLogz(true);

  TCanvas* cUAcc = fHM->CreateCanvas("fh_nof_urqmdParticles_p_pt_acc", "fh_nof_urqmdParticles_p_pt_acc", 1000, 500);
  cUAcc->Divide(2, 1);
  cUAcc->cd(1);
  DrawH2(uElAcc);
  uElAcc->SetMinimum(1e-6);
  uElAcc->SetMaximum(1e-1);
  gPad->SetLogz(true);
  cUAcc->cd(2);
  DrawH2(uPosAcc);
  uPosAcc->SetMinimum(1e-6);
  uPosAcc->SetMaximum(1e-1);
  gPad->SetLogz(true);
}

void CbmAnaDielectronTaskDraw::RebinMinvHist()
{
  int nRebin = 20;  // general rebin
  int nRebCB = 10;  // rebin for CB histos
  int nRebSP = 4;   // rebin for single particle histos
  for (int i = 0; i < CbmLmvmHist::fNofAnaSteps; i++) {
    H1("fh_signal_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebin);
    H1("fh_bg_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebin);
    H1("fh_combPairsPM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_combPairsPP_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_combPairsMM_minv_sameEvent_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_combPairsPM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_combPairsPP_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_combPairsMM_minv_mixedEvents_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebCB);
    H1("fh_pi0_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebin);
    H1("fh_eta_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebin);
    H1("fh_bg_truematch_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(2 * nRebin);
    H1("fh_bg_mismatch_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(2 * nRebin);
    H1("fh_bg_truematch_el_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(2 * nRebin);
    H1("fh_bg_truematch_notel_minv_" + CbmLmvmHist::fAnaSteps[i])->Rebin(2 * nRebin);

    H1("fh_nof_plutoElectrons_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebSP);
    H1("fh_nof_plutoPositrons_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebSP);
    H1("fh_nof_urqmdElectrons_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebSP);
    H1("fh_nof_urqmdPositrons_" + CbmLmvmHist::fAnaSteps[i])->Rebin(nRebSP);
    H1("fh_nof_plutoElectrons_" + CbmLmvmHist::fAnaSteps[i])->Scale(1. / nRebSP);
    H1("fh_nof_plutoPositrons_" + CbmLmvmHist::fAnaSteps[i])->Scale(1. / nRebSP);
    H1("fh_nof_urqmdElectrons_" + CbmLmvmHist::fAnaSteps[i])->Scale(1. / nRebSP);
    H1("fh_nof_urqmdPositrons_" + CbmLmvmHist::fAnaSteps[i])->Scale(1. / nRebSP);

    for (int iP = 0; iP < CbmLmvmHist::fNofBgPairSources; iP++) {
      stringstream ss;
      ss << "fh_source_bg_minv_" << iP << "_" << CbmLmvmHist::fAnaSteps[i];
      H1(ss.str())->Rebin(8 * nRebin);
    }
  }
}

TH1D* CbmAnaDielectronTaskDraw::H1(const string& name) { return (TH1D*) fHM->H1(name); }
/*
TH2D* CbmAnaDielectronTaskDraw::H2(
      const string& name)
{
   return (TH2D*) fHM->H1(name);
}
*/
TH2D* CbmAnaDielectronTaskDraw::H2(const string& name) { return (TH2D*) fHM->H2(name); }

void CbmAnaDielectronTaskDraw::DrawEfficiencyOnHist(TH1* h1, TH1* h2, Double_t xPos, Double_t yPos)
{
  string effTxt = "";
  if (h2->GetEntries() != 0) {
    effTxt = Cbm::NumberToString<Double_t>(((Double_t) h1->GetEntries() / h2->GetEntries() * 100.), 1);
  }
  TText* t = new TText(xPos, yPos, effTxt.c_str());
  t->SetTextSize(0.1);
  t->Draw();
}


TH1D* CbmAnaDielectronTaskDraw::CreateSignificanceH1D(TH1D* s, TH1D* bg, const string& name, const string& option)
{
  Int_t nBins   = s->GetNbinsX();
  Double_t xmin = s->GetXaxis()->GetXmin();
  Double_t xmax = s->GetXaxis()->GetXmax();
  TH1D* hsig    = new TH1D(name.c_str(), name.c_str(), nBins, xmin, xmax);
  hsig->GetXaxis()->SetTitle(s->GetXaxis()->GetTitle());

  Double_t sumSignal = 0., sumBg = 0., significance = 0.;
  // "right" - one wants to reject right part of the histogram.
  // value > cut -> reject
  if (option == "right") {
    for (Int_t i = 1; i <= nBins; i++) {
      sumSignal     = s->Integral(1, i, "width");
      sumBg         = bg->Integral(1, i, "width");
      Double_t prov = TMath::Sqrt(sumSignal + sumBg);
      if (prov != 0.) significance = sumSignal / prov;
      else
        significance = 0.;
      hsig->SetBinContent(i, significance);
    }
    // "left" - one wants to reject left part of the histogram.
    // value < cut -> reject
  }
  else if (option == "left") {
    for (Int_t i = nBins; i >= 1; i--) {
      sumSignal     = s->Integral(i, nBins, "width");
      sumBg         = bg->Integral(i, nBins, "width");
      Double_t prov = TMath::Sqrt(sumSignal + sumBg);
      if (prov != 0.) significance = sumSignal / prov;
      else
        significance = 0.;
      hsig->SetBinContent(i, significance);
    }
  }
  return hsig;
}

TH2D* CbmAnaDielectronTaskDraw::CreateSignificanceH2D(TH2D* signal, TH2D* bg, const string& name, const string& title)
{

  Double_t xmin  = 1.0;
  Double_t xmax  = 5.0;
  Double_t ymin  = 1.0;
  Double_t ymax  = 5.0;
  Double_t delta = 0.1;
  Int_t nStepsX  = (Int_t)((xmax - xmin) / delta);
  Int_t nStepsY  = (Int_t)((ymax - ymin) / delta);
  Int_t nBinsX   = signal->GetNbinsX();
  Int_t nBinsY   = signal->GetNbinsY();

  TH2D* hsig = new TH2D(name.c_str(), title.c_str(), nStepsX, xmin, xmax, nStepsY, ymin, ymax);

  Double_t sumSignal = 0;
  Double_t sumBg     = 0;

  Int_t binX = 1;
  for (Double_t xcut = xmin; xcut <= xmax; xcut += delta, binX++) {
    Int_t binY = 1;
    cout << "x " << xcut << endl;
    for (Double_t ycut = ymin; ycut <= ymax; ycut += delta, binY++) {
      sumSignal = 0;
      sumBg     = 0;
      for (Int_t ix = 1; ix <= nBinsX; ix++) {
        for (Int_t iy = 1; iy <= nBinsY; iy++) {
          Double_t xcenter = signal->GetXaxis()->GetBinCenter(ix);
          Double_t ycenter = signal->GetYaxis()->GetBinCenter(iy);
          Double_t val     = -1 * (ycut / xcut) * xcenter + ycut;

          if (!(xcenter < xcut && ycenter < val)) {
            sumSignal += signal->GetBinContent(ix, iy);
            sumBg += bg->GetBinContent(ix, iy);
          }
        }
      }
      Double_t prov         = TMath::Sqrt(sumSignal + sumBg);
      Double_t significance = 0.;
      if (prov != 0) significance = sumSignal / prov;
      hsig->SetBinContent(binX, binY, significance);
    }
  }
  return hsig;
}

void CbmAnaDielectronTaskDraw::SOverBg(CbmLmvmAnalysisSteps step)
{
  TH1D* s  = H1("fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step]);
  TH1D* bg = H1("fh_bg_minv_" + CbmLmvmHist::fAnaSteps[step]);
  //H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[step]);	// TODO: commented these lines, what do they do??
  //H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[0]);

  if (s->GetEntries() < 1) return;

  TH1D* sClone = (TH1D*) s->Clone();
  sClone->Fit("gaus", "Q");

  Double_t mean  = sClone->GetFunction("gaus")->GetParameter("Mean");
  Double_t sigma = sClone->GetFunction("gaus")->GetParameter("Sigma");

  Int_t minInd = s->FindBin(mean - 2. * sigma);
  Int_t maxInd = s->FindBin(mean + 2. * sigma);

  Double_t sumSignal = 0.;
  Double_t sumBg     = 0.;
  for (Int_t i = minInd + 1; i <= maxInd - 1; i++) {
    sumSignal += s->GetBinContent(i);
    sumBg += bg->GetBinContent(i);
  }
}

void CbmAnaDielectronTaskDraw::SOverBgAll()
{
  fHM->CreateCanvas("lmvm_signal_fitting", "lmvm_signal_fitting", 600, 600);
  SOverBg(kReco);
  SOverBg(kChi2Prim);
  SOverBg(kElId);
  SOverBg(kGammaCut);
  if (fUseMvd) SOverBg(kMvd1Cut);
  if (fUseMvd) SOverBg(kMvd2Cut);
  SOverBg(kStCut);
  SOverBg(kTtCut);
  SOverBg(kPtCut);
}

void CbmAnaDielectronTaskDraw::DrawPtYDistribution(int step, bool drawAnaStep)
{
  TH2D* h   = H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[step]);
  TH2D* hmc = H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[0]);

  DrawH2(h, kLinear, kLinear, kLinear, "COLZ");
  if (drawAnaStep) DrawEfficiencyOnHist(h, hmc, 0.2, 1.8);
  if (drawAnaStep) DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.78, 0.70, 0.9);
}

void CbmAnaDielectronTaskDraw::DrawPtYDistributionAll()
{
  Int_t hi   = 1;
  TCanvas* c = fHM->CreateCanvas("lmvm_pty", "lmvm_pty", 750, 1000);
  c->Divide(3, 4);
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c->cd(hi++);
    DrawPtYDistribution(step);
  }

  fHM->CreateCanvas("lmvm_pty_" + CbmLmvmHist::fAnaSteps[kAcc], "lmvm_pty_" + CbmLmvmHist::fAnaSteps[kAcc], 800, 800);
  DrawPtYDistribution(kAcc, true);

  fHM->CreateCanvas("lmvm_pty_" + CbmLmvmHist::fAnaSteps[kPtCut], "lmvm_pty_" + CbmLmvmHist::fAnaSteps[kPtCut], 800,
                    800);
  DrawPtYDistribution(kPtCut, true);
}


void CbmAnaDielectronTaskDraw::DrawRapidityDistributionAll()
{
  Int_t hi   = 1;
  TCanvas* c = fHM->CreateCanvas("lmvm_signal_rapidity", "lmvm_signal_rapidity", 750, 1000);
  c->Divide(3, 4);
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c->cd(hi++);
    TH1D* proj = H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[step])->ProjectionX();
    DrawH1(proj);
    DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.70, 0.78, 0.90, 0.9);
  }
}


void CbmAnaDielectronTaskDraw::DrawPtYEfficiency(int step, bool drawAnaStep)
{
  TH2D* h = H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[step]);
  // efficiency is normalized to the previous step (step - 1)
  TH2D* hmc = H2("fh_signal_pty_" + CbmLmvmHist::fAnaSteps[kMc]);

  TH2D* eff = Cbm::DivideH2(h, hmc);
  eff->GetZaxis()->SetTitle("Efficiency [%]");
  DrawH2(eff);
  eff->SetMaximum(10.);
  if (drawAnaStep) DrawEfficiencyOnHist(h, hmc, 0.2, 1.8);
  if (drawAnaStep) DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.78, 0.70, 0.9);
}

void CbmAnaDielectronTaskDraw::DrawPtYEfficiencyAll()
{
  Int_t hi   = 1;
  TCanvas* c = fHM->CreateCanvas("lmvm_pty_efficiency", "lmvm_pty_efficiency", 1000, 1000);
  c->Divide(3, 3);
  for (int step = kAcc; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c->cd(hi++);
    DrawPtYEfficiency(step);
  }

  fHM->CreateCanvas("lmvm_pty_efficiency_ptcut", "lmvm_pty_efficiency_ptcut", 600, 600);
  DrawPtYEfficiency(kPtCut, true);
}

void CbmAnaDielectronTaskDraw::DrawMomentumDistributionAll()
{
  fHM->CreateCanvas("lmvm_signal_momentum_distribution", "lmvm_signal_momentum_distribution", 600, 600);
  Draw1DHistoForEachAnalysisStep("fh_signal_mom", true);
}

void CbmAnaDielectronTaskDraw::DrawMomentumEfficiencyAll()
{
  //EFFICIENCY vs. MOMENTUM
  /*    TCanvas *c5 = CreateCanvas("signal_momentum_efficiency","signal_momentum_efficiency", 600, 600);
    Draw1DHistoForEachAnalysisStep(
          NULL,
          NULL, //DivideHisto1D(H1("fh_acc_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_reco_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_chi_prim_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_el_id_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_gammacut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_dstscut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_dsts2cut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_stcut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_ttcut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_ptcut_signal_mom"), H1("fh_mc_signal_mom")),
          DivideHisto1D(H1("fh_anglecut_signal_mom"), H1("fh_mc_signal_mom")),
          false
    );*/
}

void CbmAnaDielectronTaskDraw::DrawMotherPdg()
{
  fHM->CreateCanvas("lmvm_mother_pdg", "lmvm_mother_pdg", 500, 500);
  DrawH1({H1("fh_mc_mother_pdg"), H1("fh_acc_mother_pdg")}, {"MC", "acc"}, kLinear, kLog, true, 0.7, 0.7, 0.99, 0.99);
}

void CbmAnaDielectronTaskDraw::DrawPPAngleMCSignal()
{
  fHM->CreateCanvas("mc_signal_mom_angle", "mc_signal_mom_angle", 500, 500);
  DrawH2(H2("fh_mc_signal_mom_angle"));
}

void CbmAnaDielectronTaskDraw::DrawPmtXY()
{
  TCanvas* c = fHM->CreateCanvas("lmvm_pmt_xy", "lmvm_pmt_xy", 500, 1800);
  c->Divide(1, 3);
  c->cd(1);
  DrawH2(H2("fh_signal_pmtXY"));
  gPad->SetLogz(true);
  DrawTextOnPad(CbmLmvmHist::fSourceTypesLatex[kSignal], 0.50, 0.78, 0.70, 0.9);
  c->cd(2);
  DrawH2(H2("fh_pi0_pmtXY"));
  gPad->SetLogz(true);
  DrawTextOnPad(CbmLmvmHist::fSourceTypesLatex[kPi0], 0.50, 0.78, 0.70, 0.9);
  c->cd(3);
  DrawH2(H2("fh_gamma_pmtXY"));
  gPad->SetLogz(true);
  DrawTextOnPad(CbmLmvmHist::fSourceTypesLatex[kGamma], 0.50, 0.78, 0.70, 0.9);
}

void CbmAnaDielectronTaskDraw::Draw1DSourceTypes(const string& hName, bool doScale)
{
  vector<TH1*> h;
  vector<string> hLegend;
  for (int i = 0; i < CbmLmvmHist::fNofSourceTypes; i++) {
    string fullName = hName + "_" + CbmLmvmHist::fSourceTypes[i];
    h.push_back(H1(fullName));
    h[i]->SetLineWidth(2);
    h[i]->SetLineColor(CbmLmvmHist::fSourceTypesColor[i]);
    if (doScale) h[i]->Scale(1. / h[i]->Integral());
    hLegend.push_back(CbmLmvmHist::fSourceTypesLatex[i]);
  }
  DrawH1(h, hLegend, kLinear, kLog, true, 0.90, 0.7, 0.99, 0.99);
}

void CbmAnaDielectronTaskDraw::Draw1DCut(const string& hName, const string& sigOption, double cutValue)
{
  Int_t w = 600;
  Int_t h = 600;
  if (fDrawSignificance) w = 1200;
  TCanvas* c = fHM->CreateCanvas(("lmvm_" + hName).c_str(), ("lmvm_" + hName).c_str(), w, h);
  if (fDrawSignificance) {
    c->Divide(2, 1);
    c->cd(1);
  }
  Draw1DSourceTypes(hName);
  if (cutValue != -999999.) {
    TLine* cutLine = new TLine(cutValue, 0.0, cutValue, 1.0);
    cutLine->SetLineWidth(2);
    cutLine->Draw();
  }
  if (fDrawSignificance) {
    c->cd(2);
    string sName  = hName + "_" + CbmLmvmHist::fSourceTypes[kSignal];
    string bgName = hName + "_" + CbmLmvmHist::fSourceTypes[kBg];
    TH1D* sign    = CreateSignificanceH1D(H1(sName), H1(bgName), hName + "_significance", sigOption);
    DrawH1(sign);
  }
}

void CbmAnaDielectronTaskDraw::DrawCutDistributions()
{
  //Draw1DCut("fh_richann", "left", CbmLitGlobalElectronId::GetInstance().GetRichAnnCut());
  //Draw1DCut("fh_trdann", "left", CbmLitGlobalElectronId::GetInstance().GetTrdAnnCut());

  Draw1DCut("fh_richann", "left", -0.4);
  Draw1DCut("fh_trdann", "left", 0.85);

  Draw2DCut("fh_tofm2");

  Draw1DCut("fh_chi2prim", "right", fCuts.fChiPrimCut);
  Draw1DCut("fh_pt", "left", fCuts.fPtCut);
  Draw1DCut("fh_mom", "left");
  Draw1DCut("fh_chi2sts", "right");

  Draw2DCut("fh_stcut", fCuts.fStCutPP, fCuts.fStCutAngle);
  Draw2DCut("fh_ttcut", fCuts.fTtCutPP, fCuts.fTtCutAngle);
  Draw2DCut("fh_rtcut", fCuts.fRtCutPP, fCuts.fRtCutAngle);

  Draw2DCut("fh_stcut_pion", fCuts.fStCutPP, fCuts.fStCutAngle);
  Draw2DCut("fh_ttcut_pion", fCuts.fTtCutPP, fCuts.fTtCutAngle);
  Draw2DCut("fh_rtcut_pion", fCuts.fRtCutPP, fCuts.fRtCutAngle);
  Draw2DCut("fh_stcut_truepair", fCuts.fStCutPP, fCuts.fStCutAngle);
  Draw2DCut("fh_ttcut_truepair", fCuts.fTtCutPP, fCuts.fTtCutAngle);
  Draw2DCut("fh_rtcut_truepair", fCuts.fRtCutPP, fCuts.fRtCutAngle);

  /*  TH2D* st = H2("fh_stcut_signal");
   double sumT = 0.;
   double sumAll = 0;
   for (int x = 1; x <= st->GetNbinsX(); x++){
      for (int y = 1; y <= st->GetNbinsY(); y++){
         double c = st->GetBinContent(x, y);
         double xc = (st->GetXaxis()->GetBinLowEdge(x) + st->GetXaxis()->GetBinUpEdge(x))/2.0;
         double yc = (st->GetYaxis()->GetBinLowEdge(y) + st->GetXaxis()->GetBinUpEdge(y))/2.0;

         Double_t val = -1.*(fStCutAngle/fStCutPP)*xc + fStCutAngle;
         if ( (xc < fStCutPP && val > yc) ) {
         // Double_t val = -1.*(fTtCutAngle/fTtCutPP)*xc + fTtCutAngle;
         // if ( (xc < fTtCutPP && val > yc) ) {
            sumT += c;
         }
         sumAll += c;
      }
   }
   cout << endl << endl << endl << "sumT/sumAll = " << 100*sumT/sumAll << endl;
*/

  if (fUseMvd) {
    Draw2DCut("fh_mvd1cut", fCuts.fMvd1CutD, fCuts.fMvd1CutP);
    Draw2DCut("fh_mvd2cut", fCuts.fMvd2CutD, fCuts.fMvd2CutP);
  }
}

void CbmAnaDielectronTaskDraw::DrawSourcesBgPairsEpEm(int step, bool inPercent, bool drawAnaStep)
{
  TH2D* h = (TH2D*) H2("fh_source_pairs_epem_" + CbmLmvmHist::fAnaSteps[step])->Clone();
  gStyle->SetPaintTextFormat("4.1f");
  string labels[3] = {"#gamma", "#pi^{0}", "oth"};
  for (Int_t i = 1; i <= 3; i++) {
    h->GetYaxis()->SetBinLabel(i, labels[i - 1].c_str());
    h->GetXaxis()->SetBinLabel(i, labels[i - 1].c_str());
  }
  //h->SetMarkerColor(0);
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
  if (drawAnaStep) DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.90, 0.70, 0.99);
}

void CbmAnaDielectronTaskDraw::DrawSourcesBgPairsAll()
{
  Int_t hi    = 1;
  TCanvas* c1 = fHM->CreateCanvas("lmvm_bg_sources_pairs_epem_abs", "lmvm_bg_sources_pairs_epem_abs", 900, 900);
  c1->Divide(3, 3);
  for (int step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c1->cd(hi++);
    DrawSourcesBgPairsEpEm(step, false);
  }

  hi          = 1;
  TCanvas* c2 = fHM->CreateCanvas("lmvm_bg_sources_pairs_epem_percent", "lmvm_bg_sources_pairs_epem_percent", 900, 900);
  c2->Divide(3, 3);
  for (int step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c2->cd(hi++);
    DrawSourcesBgPairsEpEm(step, true);
  }

  //Draw pair source histogram for the las step (ptcut)
  fHM->CreateCanvas("lmvm_bg_sources_pairs_epem_abs_" + CbmLmvmHist::fAnaSteps[kPtCut],
                    "lmvm_bg_sources_pairs_epem_abs_" + CbmLmvmHist::fAnaSteps[kPtCut], 600, 600);
  DrawSourcesBgPairsEpEm(kPtCut, false, false);

  fHM->CreateCanvas("lmvm_bg_sources_pairs_epem_percent_" + CbmLmvmHist::fAnaSteps[kPtCut],
                    "lmvm_bg_sources_pairs_epem_percent_" + CbmLmvmHist::fAnaSteps[kPtCut], 600, 600);
  DrawSourcesBgPairsEpEm(kPtCut, true, false);

  // Draw 2D histogram for sources of BG pairs
  DrawBgSource2D("lmvm_source_pairs_2d", "fh_source_pairs", CbmLmvmHist::fBgPairSourceLatex, 1000.,
                 "Pairs per event x10^{3}");
}

void CbmAnaDielectronTaskDraw::Draw2DCutTriangle(double xCross, double yCross)
{
  if (xCross == -999999. || yCross == -999999.) return;
  TLine* line1 = new TLine(0., 0., xCross, 0.);
  line1->SetLineWidth(2.);
  line1->Draw();
  TLine* line2 = new TLine(0., 0., 0., yCross);
  line2->SetLineWidth(2.);
  line2->Draw();
  TLine* line3 = new TLine(xCross, 0., 0., yCross);
  line3->SetLineWidth(2.);
  line3->Draw();
}


void CbmAnaDielectronTaskDraw::Draw2DCut(const string& hist, double cutCrossX, double cutCrossY)
{
  TCanvas* c = fHM->CreateCanvas(("lmvm_" + hist).c_str(), ("lmvm_" + hist).c_str(), 600, 900);
  c->Divide(2, 3);
  vector<TH1*> projX, projY;
  for (int i = 0; i < CbmLmvmHist::fNofSourceTypes; i++) {
    c->cd(i + 1);
    DrawH2(H2(hist + "_" + CbmLmvmHist::fSourceTypes[i]));
    double nofPerEvent = H2(hist + "_" + CbmLmvmHist::fSourceTypes[i])->GetEntries() / (double) fNofEvents;
    cout << hist << "_" << CbmLmvmHist::fSourceTypes[i] << " = " << nofPerEvent << endl;
    DrawTextOnPad((Cbm::NumberToString(nofPerEvent, 2) + "/ev."), 0.1, 0.9, 0.5, 0.99);
    DrawTextOnPad(CbmLmvmHist::fSourceTypesLatex[i], 0.6, 0.89, 0.7, 0.99);
    Draw2DCutTriangle(cutCrossX, cutCrossY);
    projX.push_back(H2(hist + "_" + CbmLmvmHist::fSourceTypes[i])->ProjectionX());
    projY.push_back(H2(hist + "_" + CbmLmvmHist::fSourceTypes[i])->ProjectionY());
  }

  //Draw X projection
  c->cd(5);
  DrawH1(projX, CbmLmvmHist::fSourceTypesLatex, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99);

  //Draw Y projection
  c->cd(6);
  DrawH1(projY, CbmLmvmHist::fSourceTypesLatex, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99);

  fHM->CreateCanvas(("lmvm_" + hist + "_signal").c_str(), ("lmvm_" + hist + "_signal").c_str(), 800, 800);
  DrawH2(H2(hist + "_" + CbmLmvmHist::fSourceTypes[kSignal]));
  Draw2DCutTriangle(cutCrossX, cutCrossY);
  fHM->CreateCanvas(("lmvm_" + hist + "_gamma").c_str(), ("lmvm_" + hist + "_gamma").c_str(), 800, 800);
  DrawH2(H2(hist + "_" + CbmLmvmHist::fSourceTypes[kGamma]));
  Draw2DCutTriangle(cutCrossX, cutCrossY);
  //c->cd(9);
  //TH2D* fh_significance = CalculateSignificance2D(fh_stcut_signal, fh_stcut_bg, "stcut_2dsignificance", "significance");
  //fh_significance->Draw("COLZ");
}

void CbmAnaDielectronTaskDraw::DrawGammaVertex()
{
  TH2D* xz   = H2("fh_vertex_el_gamma_xz_mc");
  TH2D* yz   = H2("fh_vertex_el_gamma_yz_mc");
  TH2D* xy   = H2("fh_vertex_el_gamma_xy_mc");
  TCanvas* c = fHM->CreateCanvas("lmvm_vertex_el_gamma_mc", "lmvm_vertex_el_gamma_mc", 1200, 400);
  c->Divide(3, 1);
  c->cd(1);
  DrawH2(xz);
  xz->SetMinimum(1e-3);
  gPad->SetLogz(true);
  c->cd(2);
  DrawH2(yz);
  yz->SetMinimum(1e-3);
  gPad->SetLogz(true);
  c->cd(3);
  DrawH2(xy);
  xy->SetMinimum(1e-3);
  gPad->SetLogz(true);

  // number of e+- from gamma vs Z.
  fHM->CreateCanvas("lmvm_vertex_el_gamma_z_mc", "lmvm_vertex_el_gamma_z_mc", 600, 600);
  TH1D* zProj = (TH1D*) xz->ProjectionX("fh_vertex_el_gamma_pz_mc")->Clone();
  zProj->GetYaxis()->SetTitle("Counter per event");
  zProj->GetXaxis()->SetRangeUser(-2., 17.);
  DrawH1(zProj);

  fHM->CreateCanvas("lmvm_vertex_el_gamma_z_ptcut", "lmvm_vertex_el_gamma_z_ptcut", 600, 600);
  TH1D* zProjPtCut = (TH1D*) (H2("fh_vertex_el_gamma_xz_ptcut")->ProjectionX("fh_vertex_el_gamma_ptcut_pz")->Clone());
  zProjPtCut->GetYaxis()->SetTitle("Counter per event");
  zProjPtCut->GetXaxis()->SetRangeUser(-2., 17.);
  DrawH1(zProjPtCut);

  TH2D* xzZoom = (TH2D*) xz->Clone();
  TH2D* yzZoom = (TH2D*) yz->Clone();
  TH2D* xyZoom = (TH2D*) xy->Clone();
  TCanvas* cZoom =
    fHM->CreateCanvas("lmvm_vertex_el_gamma_mc_target_zoom", "lmvm_vertex_el_gamma_mc_target_zoom", 1200, 400);
  cZoom->Divide(3, 1);
  cZoom->cd(1);
  xzZoom->GetXaxis()->SetRangeUser(-1., 11.);
  xzZoom->GetYaxis()->SetRangeUser(-10., 10.);
  DrawH2(xzZoom);
  xzZoom->SetMinimum(1e-3);
  gPad->SetLogz(true);
  cZoom->cd(2);
  yzZoom->GetXaxis()->SetRangeUser(-1., 11.);
  yzZoom->GetYaxis()->SetRangeUser(-10., 10.);
  DrawH2(yzZoom);
  yzZoom->SetMinimum(1e-3);
  gPad->SetLogz(true);
  cZoom->cd(3);
  xyZoom->GetXaxis()->SetRangeUser(-20., 20.);
  xyZoom->GetYaxis()->SetRangeUser(-20., 20.);
  DrawH2(xyZoom);
  xyZoom->SetMinimum(1e-3);
  gPad->SetLogz(true);

  fHM->CreateCanvas("lmvm_vertex_el_gamma_rz_mc", "lmvm_vertex_el_gamma_rz_mc", 600, 600);
  DrawH2(H2("fh_vertex_el_gamma_rz_mc"));
  H2("fh_vertex_el_gamma_rz_mc")->SetMinimum(1e-3);
  gPad->SetLogz(true);
}

void CbmAnaDielectronTaskDraw::Draw1DHistoForEachAnalysisStep(const string& hist, Bool_t logy)
{
  string drOpt     = "";  // First histogram will be drawn without options.
  Double_t min     = 9999999999.;
  TH1D* firstHisto = NULL;
  TLegend* leg     = new TLegend(0.80, 0.32, 0.99, 0.99);
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    string fullName = hist + "_" + CbmLmvmHist::fAnaSteps[step];
    TH1D* h         = H1(fullName);
    if (h == NULL || h->GetEntries() <= 0) continue;
    leg->AddEntry(h, CbmLmvmHist::fAnaStepsLatex[step].c_str(), "l");
    int color = CbmLmvmHist::fAnaStepsColor[step];
    DrawH1(h, kLinear, kLinear, drOpt, color);
    if (firstHisto == NULL) firstHisto = h;
    drOpt = "same";  // If the histogram is drawn, next histograms must be drawn with "same" option.
    if (min > h->GetMinimum()) { min = h->GetMinimum(); }
  }
  if (min <= 0.0) min = 1e-9;
  if (firstHisto != NULL) firstHisto->SetMinimum(min);

  leg->SetFillColor(kWhite);
  leg->Draw();

  gPad->SetGridx(true);
  gPad->SetGridy(true);
  gPad->SetLogy(logy);
}

void CbmAnaDielectronTaskDraw::DrawMinvForEachAnalysisStep()
{
  TCanvas* c1 =
    fHM->CreateCanvas("lmvm_minv_for_each_analysis_step_s_bg", "lmvm_minv_for_each_analysis_step_s_bg", 1200, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_signal_minv", true);
  c1->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_bg_minv", true);


  TCanvas* c2 = fHM->CreateCanvas("lmvm_minv_for_each_analysis_step_pi0_eta",
                                  "lmvm_minv_for_each_analysis_step_pi0_eta", 1200, 600);
  c2->Divide(2, 1);
  c2->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_pi0_minv", true);
  c2->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_eta_minv", true);

  TCanvas* c3 = fHM->CreateCanvas("lmvm_minv_for_each_analysis_step_combinatorial_Pairs_same_Event",
                                  "lmvm_minv_for_each_analysis_step_combinatorial_Pairs_same_event", 1800, 600);
  c3->Divide(3, 1);
  c3->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_combPairsPM_minv_sameEvent", true);
  c3->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_combPairsPP_minv_sameEvent", true);
  c3->cd(3);
  Draw1DHistoForEachAnalysisStep("fh_combPairsMM_minv_sameEvent", true);

  TCanvas* c4 = fHM->CreateCanvas("lmvm_minv_for_each_analysis_step_combinatorial_Pairs_mixed_Events",
                                  "lmvm_minv_for_each_analysis_step_combinatorial_Pairs_mixed_Events", 1800, 600);
  c4->Divide(3, 1);
  c4->cd(1);
  Draw1DHistoForEachAnalysisStep("fh_combPairsPM_minv_mixedEvents", true);
  c4->cd(2);
  Draw1DHistoForEachAnalysisStep("fh_combPairsPP_minv_mixedEvents", true);
  c4->cd(3);
  Draw1DHistoForEachAnalysisStep("fh_combPairsMM_minv_mixedEvents", true);

  fHM->CreateCanvas("lmvm_minv_for_each_analysis_step_gamma", "lmvm_minv_for_each_analysis_step_gamma", 600, 600);
  // H1("fh_gamma_minv_mc")->GetXaxis()->SetRangeUser(0., 0.05);
  Draw1DHistoForEachAnalysisStep("fh_gamma_minv", true);
}

void CbmAnaDielectronTaskDraw::DrawMinvSandBg(int step)
{
  TH1* s1  = H1("fh_signal_minv_" + CbmLmvmHist::fAnaSteps[step]);
  TH1* bg1 = H1("fh_bg_minv_" + CbmLmvmHist::fAnaSteps[step]);

  TH1D* s   = (TH1D*) s1->Clone();
  TH1D* bg  = (TH1D*) bg1->Clone();
  TH1D* sbg = (TH1D*) bg->Clone();
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

  DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.65, 0.78, 0.85, 0.9);
}

void CbmAnaDielectronTaskDraw::DrawMinvSandBgAll()
{
  Int_t hi   = 1;
  TCanvas* c = fHM->CreateCanvas("lmvm_minv_both_s_and_bg", "lmvm_minv_both_s_and_bg", 900, 900);
  c->Divide(3, 3);
  for (int step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c->cd(hi++);
    DrawMinvSandBg(step);
  }

  fHM->CreateCanvas("lmvm_minv_both_s_and_bg_ptcut", "lmvm_minv_both_s_and_bg_ptcut", 900, 900);
  DrawMinvSandBg(kPtCut);
}

void CbmAnaDielectronTaskDraw::DrawMinvSource(int step, bool drawAnaStep)
{
  double nofBg = H1("fh_bg_minv_" + CbmLmvmHist::fAnaSteps[step])->GetEntries();

  vector<TH1*> hists;
  for (int i = 0; i < CbmLmvmHist::fNofBgPairSources; i++) {
    stringstream ss;
    ss << "fh_source_bg_minv_" << i << "_" << CbmLmvmHist::fAnaSteps[step];
    H1(ss.str())->GetXaxis()->SetRangeUser(0, 2.);
    hists.push_back(H1(ss.str()));
  }

  DrawH1(hists, CbmLmvmHist::fBgPairSourceLatex, kLinear, kLinear, false, 0.85, 0.15, 0.99, 0.80);

  TLegend* legend = new TLegend(0.78, 0.15, 0.99, 0.90);
  for (int i = 0; i < CbmLmvmHist::fNofBgPairSources; i++) {
    hists[i]->SetMinimum(1e-8);
    legend->AddEntry(
      hists[i],
      (CbmLmvmHist::fBgPairSourceLatex[i] + "(" + Cbm::NumberToString(100. * hists[i]->GetEntries() / nofBg, 1) + "%)")
        .c_str(),
      "f");
  }
  legend->SetFillColor(kWhite);
  legend->Draw();
  if (drawAnaStep) DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.65, 0.78, 0.85, 0.9);
}

void CbmAnaDielectronTaskDraw::DrawMinvSourceAll()
{
  {
    Int_t hi   = 1;
    TCanvas* c = fHM->CreateCanvas("lmvm_minv_source", "lmvm_minv_source", 900, 900);
    c->Divide(3, 3);
    for (int step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
      if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
      c->cd(hi++);
      DrawMinvSource(step);
    }

    // Draw histogram for the last step (ptcut) on one histogram
    fHM->CreateCanvas("lmvm_minv_source_" + CbmLmvmHist::fAnaSteps[kPtCut],
                      "lmvm_minv_source_" + CbmLmvmHist::fAnaSteps[kPtCut], 600, 600);
    DrawMinvSource(kPtCut, false);

    fHM->CreateCanvas("lmvm_minv_source_" + CbmLmvmHist::fAnaSteps[kElId],
                      "lmvm_minv_source_" + CbmLmvmHist::fAnaSteps[kElId], 600, 600);
    DrawMinvSource(kElId, false);
  }

  // Draw mismatches and true matches minv
  {
    Int_t hi   = 1;
    TCanvas* c = fHM->CreateCanvas("lmvm_minv_mismatches", "lmvm_minv_mismatches", 900, 900);
    c->Divide(3, 3);
    for (int step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
      if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
      c->cd(hi++);
      DrawH1({H1("fh_bg_truematch_minv_" + CbmLmvmHist::fAnaSteps[step]),
              H1("fh_bg_truematch_el_minv_" + CbmLmvmHist::fAnaSteps[step]),
              H1("fh_bg_truematch_notel_minv_" + CbmLmvmHist::fAnaSteps[step]),
              H1("fh_bg_mismatch_minv_" + CbmLmvmHist::fAnaSteps[step])},
             {"true match", "true match (e^{#pm})", "true match (not e^{#pm})", "mismatch"}, kLinear, kLinear, true,
             0.5, 0.7, 0.99, 0.99);
    }

    // Draw minv after PtCut
    double trueMatch      = H1("fh_bg_truematch_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])->GetEntries();
    double trueMatchEl    = H1("fh_bg_truematch_el_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])->GetEntries();
    double trueMatchNotEl = H1("fh_bg_truematch_notel_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])->GetEntries();
    double misMatch       = H1("fh_bg_mismatch_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])->GetEntries();
    double nofBg          = H1("fh_bg_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])->GetEntries();

    fHM->CreateCanvas("lmvm_minv_mismatches_" + CbmLmvmHist::fAnaSteps[kPtCut],
                      "lmvm_minv_mismatches_" + CbmLmvmHist::fAnaSteps[kPtCut], 700, 700);
    DrawH1({H1("fh_bg_truematch_minv_" + CbmLmvmHist::fAnaSteps[kPtCut]),
            H1("fh_bg_truematch_el_minv_" + CbmLmvmHist::fAnaSteps[kPtCut]),
            H1("fh_bg_truematch_notel_minv_" + CbmLmvmHist::fAnaSteps[kPtCut]),
            H1("fh_bg_mismatch_minv_" + CbmLmvmHist::fAnaSteps[kPtCut])},
           {"true match (" + Cbm::NumberToString(100. * trueMatch / nofBg, 1) + "%)",
            "true match (e^{#pm}) (" + Cbm::NumberToString(100. * trueMatchEl / nofBg, 1) + "%)",
            "true match (not e^{#pm}) (" + Cbm::NumberToString(100. * trueMatchNotEl / nofBg, 1) + "%)",
            "mismatch (" + Cbm::NumberToString(100. * misMatch / nofBg) + "%)"},
           kLinear, kLinear, true, 0.4, 0.7, 0.99, 0.99);
  }
}

void CbmAnaDielectronTaskDraw::DrawElPiMomHis()
{
  double binWEl = H1("fh_pi_mom_mc")->GetBinWidth(1);

  fHM->CreateCanvas("lmvm_pi_mom", "lmvm_pi_mom", 800, 800);
  H1("fh_pi_mom_mc")->Scale(1 / binWEl);
  H1("fh_pi_mom_acc")->Scale(1 / binWEl);
  H1("fh_pi_mom_rec")->Scale(1 / binWEl);
  H1("fh_pi_mom_rec_only_sts")->Scale(1 / binWEl);
  H1("fh_pi_mom_rec_sts_rich_trd")->Scale(1 / binWEl);
  H1("fh_pi_mom_rec_sts_rich_trd_tof")->Scale(1 / binWEl);
  H1("fh_pi_mom_mc")->SetMinimum(2);

  DrawH1(
    {H1("fh_pi_mom_mc"), H1("fh_pi_mom_acc"), H1("fh_pi_mom_rec"), H1("fh_pi_mom_rec_only_sts"),
     H1("fh_pi_mom_rec_sts_rich_trd"), H1("fh_pi_mom_rec_sts_rich_trd_tof")},
    {"MC (" + Cbm::NumberToString(H1("fh_pi_mom_mc")->GetEntries() / fNofEvents, 2) + " per event)",
     "Acc (" + Cbm::NumberToString(H1("fh_pi_mom_acc")->GetEntries() / fNofEvents, 2) + " per event)",
     "Rec (" + Cbm::NumberToString(H1("fh_pi_mom_rec")->GetEntries() / fNofEvents, 2) + " per event)",
     "Rec only STS (" + Cbm::NumberToString(H1("fh_pi_mom_rec_only_sts")->GetEntries() / fNofEvents, 2) + " per event)",
     "Rec STS-RICH-TRD (" + Cbm::NumberToString(H1("fh_pi_mom_rec_sts_rich_trd")->GetEntries() / fNofEvents, 2)
       + " per event)",
     "Rec STS-RICH-TRD-TOF (" + Cbm::NumberToString(H1("fh_pi_mom_rec_sts_rich_trd_tof")->GetEntries() / fNofEvents, 2)
       + " per event)"},
    kLinear, kLog, true, 0.1, 0.7, 0.99, 0.99);

  //primary pions vertex < 0.1 cm
  double binWElPrim = H1("fh_piprim_mom_mc")->GetBinWidth(1);
  fHM->CreateCanvas("lmvm_piprim_mom", "lmvm_piprim_mom", 800, 800);
  H1("fh_piprim_mom_mc")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_acc")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_rec")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_rec_only_sts")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_rec_sts_rich_trd")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_rec_sts_rich_trd_tof")->Scale(1 / binWElPrim);
  H1("fh_piprim_mom_mc")->SetMinimum(2);
  DrawH1({H1("fh_piprim_mom_mc"), H1("fh_piprim_mom_acc"), H1("fh_piprim_mom_rec"), H1("fh_piprim_mom_rec_only_sts"),
          H1("fh_piprim_mom_rec_sts_rich_trd"), H1("fh_piprim_mom_rec_sts_rich_trd_tof")},
         {"MC (" + Cbm::NumberToString(H1("fh_piprim_mom_mc")->GetEntries() / fNofEvents, 2) + " per event)",
          "Acc (" + Cbm::NumberToString(H1("fh_piprim_mom_acc")->GetEntries() / fNofEvents, 2) + " per event)",
          "Rec (" + Cbm::NumberToString(H1("fh_piprim_mom_rec")->GetEntries() / fNofEvents, 2) + " per event)",
          "Rec only STS (" + Cbm::NumberToString(H1("fh_piprim_mom_rec_only_sts")->GetEntries() / fNofEvents, 2)
            + " per event)",
          "Rec STS-RICH-TRD (" + Cbm::NumberToString(H1("fh_piprim_mom_rec_sts_rich_trd")->GetEntries() / fNofEvents, 2)
            + " per event)",
          "Rec STS-RICH-TRD-TOF ("
            + Cbm::NumberToString(H1("fh_piprim_mom_rec_sts_rich_trd_tof")->GetEntries() / fNofEvents, 2)
            + " per event)"},
         kLinear, kLog, true, 0.1, 0.7, 0.99, 0.99);

  fHM->CreateCanvas("lmvm_pi_mom_notacc", "lmvm_pi_mom_notacc", 800, 800);
  TH1D* h1 = ((TH1D*) H1("fh_pi_mom_mc")->Clone());
  h1->Add(H1("fh_pi_mom_acc"), -1.);
  DrawH1(h1);
  fHM->CreateCanvas("lmvm_piprim_mom_notacc", "lmvm_piprim_mom_notacc", 800, 800);
  TH1D* h2 = ((TH1D*) H1("fh_piprim_mom_mc")->Clone());
  h2->Add(H1("fh_piprim_mom_acc"), -1.);
  DrawH1(h2);
  DrawH1(h2);

  cout << "Number of primary pions minus at rapidity 2 = "
       << H1("fh_piprim_minus_rapidity_mc")->GetBinContent(H1("fh_piprim_minus_rapidity_mc")->FindFixBin(2)) << endl;
  cout << "Number of primary pions minus at rapidity (1, 3) = "
       << H1("fh_piprim_minus_rapidity_mc")
            ->Integral(H1("fh_piprim_minus_rapidity_mc")->FindFixBin(1),
                       H1("fh_piprim_minus_rapidity_mc")->FindFixBin(3))
       << endl;

  cout << "Number of primary pions plus at rapidity 2 = "
       << H1("fh_piprim_plus_rapidity_mc")->GetBinContent(H1("fh_piprim_plus_rapidity_mc")->FindFixBin(2)) << endl;
  cout << "Number of primary pions plus at rapidity (1, 3) = "
       << H1("fh_piprim_plus_rapidity_mc")
            ->Integral(H1("fh_piprim_plus_rapidity_mc")->FindFixBin(1), H1("fh_piprim_plus_rapidity_mc")->FindFixBin(3))
       << endl;


  double binWRapidity = H1("fh_piprim_minus_rapidity_mc")->GetBinWidth(1);
  H1("fh_pi_rapidity_mc")->Scale(1 / binWRapidity);
  H1("fh_piprim_minus_rapidity_mc")->Scale(1 / binWRapidity);
  H1("fh_piprim_plus_rapidity_mc")->Scale(1 / binWRapidity);
  H1("fh_pi0prim_rapidity_mc")->Scale(1 / binWRapidity);
  H1("fh_etaprim_rapidity_mc")->Scale(1 / binWRapidity);

  fHM->CreateCanvas("lmvm_piprim_plus_rapidity", "lmvm_piprim_plus_rapidity", 800, 800);
  DrawH1(H1("fh_piprim_plus_rapidity_mc"));
  fHM->CreateCanvas("lmvm_piprim_minus_rapidity", "lmvm_piprim_minus_rapidity", 800, 800);
  DrawH1(H1("fh_piprim_minus_rapidity_mc"));
  fHM->CreateCanvas("lmvm_pi0prim_rapidity", "lmvm_pi0prim_rapidity", 800, 800);
  DrawH1(H1("fh_pi0prim_rapidity_mc"));
  fHM->CreateCanvas("lmvm_etaprim_rapidity", "lmvm_etaprim_rapidity", 800, 800);
  DrawH1(H1("fh_etaprim_rapidity_mc"));
}

void CbmAnaDielectronTaskDraw::RemoveMvdCutBins()
{
  for (int step = kMvd2Cut + 1 + 1; step < CbmLmvmHist::fNofAnaSteps + 1; step++) {
    H1("fh_nof_bg_tracks")->SetBinContent(step - 2, H1("fh_nof_bg_tracks")->GetBinContent(step));
    H1("fh_nof_el_tracks")->SetBinContent(step - 2, H1("fh_nof_el_tracks")->GetBinContent(step));

    H1("fh_nof_mismatches")->SetBinContent(step - 2, H1("fh_nof_mismatches")->GetBinContent(step));
    H1("fh_nof_ghosts")->SetBinContent(step - 2, H1("fh_nof_ghosts")->GetBinContent(step));

    int ny = H2("fh_source_tracks")->GetYaxis()->GetNbins();
    for (int y = 1; y <= ny; y++) {
      H2("fh_source_tracks")->SetBinContent(step - 2, y, H2("fh_source_tracks")->GetBinContent(step, y));
    }

    ny = H2("fh_source_pairs")->GetYaxis()->GetNbins();
    for (int y = 1; y <= ny; y++) {
      H2("fh_source_pairs")->SetBinContent(step - 2, y, H2("fh_source_pairs")->GetBinContent(step, y));
    }
  }
}

void CbmAnaDielectronTaskDraw::DrawBgSource2D(const string& canvasName, const string& histName,
                                              const vector<string>& yLabels, double scale, const string& zTitle)
{
  int rangeMax = CbmLmvmHist::fNofAnaSteps;
  if (!fUseMvd) { rangeMax = CbmLmvmHist::fNofAnaSteps - 2; }

  fHM->CreateCanvas(string(canvasName + "_abs").c_str(), string(canvasName + "_abs").c_str(), 900, 600);
  TH2D* habs = (TH2D*) H2(histName)->Clone();
  habs->SetStats(false);
  habs->Scale(scale);
  habs->GetZaxis()->SetTitle(zTitle.c_str());
  habs->GetXaxis()->SetRange(kReco + 1, rangeMax);
  habs->SetMarkerSize(1.4);
  DrawH2(habs, kLinear, kLinear, kLog, "text COLZ");

  fHM->CreateCanvas(string(canvasName + "_percent").c_str(), string(canvasName + "_percent").c_str(), 900, 600);
  TH2D* hperc = (TH2D*) H2(histName)->Clone();
  hperc->SetStats(false);
  Int_t nBinsX = hperc->GetNbinsX();
  Int_t nBinsY = hperc->GetNbinsY();
  for (Int_t x = 1; x <= nBinsX; x++) {
    // calculate total number of BG tracks (pairs) for a current step
    double nbg = 0.;
    for (Int_t y = 1; y <= nBinsY; y++) {
      nbg += habs->GetBinContent(x, y);
    }
    Double_t sc = 100. / (nbg / scale);
    for (Int_t y = 1; y <= nBinsY; y++) {
      Double_t val = sc * hperc->GetBinContent(x, y);
      hperc->SetBinContent(x, y, val);
    }
  }
  hperc->GetZaxis()->SetTitle("[%]");
  hperc->GetXaxis()->SetLabelSize(0.06);
  hperc->GetYaxis()->SetLabelSize(0.06);
  hperc->SetMarkerColor(kBlack);
  hperc->SetMarkerSize(1.8);
  hperc->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH2(hperc, kLinear, kLinear, kLinear, "text COLZ");

  for (UInt_t y = 1; y <= yLabels.size(); y++) {
    hperc->GetYaxis()->SetBinLabel(y, yLabels[y - 1].c_str());
    habs->GetYaxis()->SetBinLabel(y, yLabels[y - 1].c_str());
  }

  SetAnalysisStepLabels(hperc);
  SetAnalysisStepLabels(habs);
}

void CbmAnaDielectronTaskDraw::DrawBgSourceTracks()
{
  gStyle->SetPaintTextFormat("4.1f");

  int rangeMax = CbmLmvmHist::fNofAnaSteps;
  if (!fUseMvd) { rangeMax = CbmLmvmHist::fNofAnaSteps - 2; }

  fHM->CreateCanvas("lmvm_nof_bg_tracks", "lmvm_nof_bg_tracks", 600, 600);
  TH1D* hbg = (TH1D*) H1("fh_nof_bg_tracks")->Clone();
  hbg->Scale(10);
  hbg->GetYaxis()->SetTitle("Tracks/event x10");
  hbg->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hbg, kLinear, kLog, "hist text0");
  hbg->SetMarkerSize(2.);

  fHM->CreateCanvas("lmvm_nof_el_tracks", "lmvm_nof_el_tracks", 600, 600);
  TH1D* hel = H1("fh_nof_el_tracks");
  hel->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hel, kLinear, kLog);

  fHM->CreateCanvas("lmvm_purity", "lmvm_purity", 600, 600);
  TH1D* purity =
    new TH1D("purity", "purity;Analysis steps;Purity", CbmLmvmHist::fNofAnaSteps, 0., CbmLmvmHist::fNofAnaSteps);
  purity->Divide(H1("fh_nof_bg_tracks"), H1("fh_nof_el_tracks"));
  purity->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(purity, kLinear, kLog, "hist text30");
  purity->SetMarkerSize(1.9);

  SetAnalysisStepLabels(hbg);
  SetAnalysisStepLabels(hel);
  SetAnalysisStepLabels(purity);

  DrawBgSource2D("lmvm_source_tracks_2d", "fh_source_tracks",
                 {"#gamma", "#pi^{0}", "#pi^{#pm}", "p", "K", "e^{#pm}_{sec}", "oth."}, 100.,
                 "Tracks per event x10^{2}");


  fHM->CreateCanvas("fh_nof_topology_pairs_gamma", "fh_nof_topology_pairs_gamma", 600, 600);
  TH1D* htopologyGamma = (TH1D*) H1("fh_nof_topology_pairs_gamma")->Clone();
  htopologyGamma->Scale(1. / htopologyGamma->Integral());
  DrawH1(htopologyGamma, kLinear, kLinear);
  htopologyGamma->SetMarkerSize(1.);

  fHM->CreateCanvas("fh_nof_topology_pairs_pi0", "fh_nof_topology_pairs_pi0", 600, 600);
  TH1D* htopologyPi0 = (TH1D*) H1("fh_nof_topology_pairs_pi0")->Clone();
  htopologyPi0->Scale(1. / htopologyPi0->Integral());
  DrawH1(htopologyPi0, kLinear, kLinear);
  htopologyPi0->SetMarkerSize(1.);
}

void CbmAnaDielectronTaskDraw::DrawMismatchesAndGhosts()
{
  gStyle->SetPaintTextFormat("4.1f");

  int rangeMax = CbmLmvmHist::fNofAnaSteps;
  if (!fUseMvd) { rangeMax = CbmLmvmHist::fNofAnaSteps - 2; }

  TCanvas* c1 = fHM->CreateCanvas("lmvm_nof_mismatches", "lmvm_nof_mismatches", 600, 600);
  c1->Divide(2, 2);
  c1->cd(1);
  TH1D* hmismatches = (TH1D*) H1("fh_nof_mismatches")->Clone();
  hmismatches->Scale(10);
  hmismatches->GetYaxis()->SetTitle("Mismatch tracks/event x10");
  hmismatches->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hmismatches, kLinear, kLog, "hist text0");

  hmismatches->SetMarkerSize(2.);
  SetAnalysisStepLabels(hmismatches);
  c1->cd(2);
  TH1D* hmismatches_rich = (TH1D*) H1("fh_nof_mismatches_rich")->Clone();
  hmismatches_rich->Scale(10);
  hmismatches_rich->GetYaxis()->SetTitle("Mismatch tracks (RICH)/event x10");
  hmismatches_rich->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hmismatches_rich, kLinear, kLog, "hist text0");
  hmismatches_rich->SetMarkerSize(2.);
  SetAnalysisStepLabels(hmismatches_rich);
  c1->cd(3);
  TH1D* hmismatches_trd = (TH1D*) H1("fh_nof_mismatches_trd")->Clone();
  hmismatches_trd->Scale(10);
  hmismatches_trd->GetYaxis()->SetTitle("Mismatch tracks (TRD)/event x10");
  hmismatches_trd->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hmismatches_trd, kLinear, kLog, "hist text0");
  hmismatches_trd->SetMarkerSize(2.);
  SetAnalysisStepLabels(hmismatches_trd);
  c1->cd(4);
  TH1D* hmismatches_tof = (TH1D*) H1("fh_nof_mismatches")->Clone();
  hmismatches_tof->Scale(10);
  hmismatches_tof->GetYaxis()->SetTitle("Mismatch tracks (TOF)/event x10");
  hmismatches_tof->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hmismatches_tof, kLinear, kLog, "hist text0");
  hmismatches_tof->SetMarkerSize(2.);
  SetAnalysisStepLabels(hmismatches_tof);

  fHM->CreateCanvas("lmvm_nof_ghosts", "lmvm_nof_ghosts", 600, 600);
  TH1D* hghosts = H1("fh_nof_ghosts");
  hghosts->GetXaxis()->SetRange(kReco + 1, rangeMax);
  DrawH1(hghosts, kLinear, kLog);
  SetAnalysisStepLabels(hghosts);
}

void CbmAnaDielectronTaskDraw::SetAnalysisStepLabels(TH1* h)
{
  h->GetXaxis()->SetLabelSize(0.06);
  int x = 1;
  for (Int_t step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) { continue; }
    h->GetXaxis()->SetBinLabel(x, CbmLmvmHist::fAnaStepsLatex[step].c_str());
    x++;
  }
}

void CbmAnaDielectronTaskDraw::DrawMinvPtAll()
{
  Int_t hi   = 1;
  TCanvas* c = fHM->CreateCanvas("lmvm_fh_signal_minv_pt", "lmvm_fh_signal_minv_pt", 750, 1000);
  c->Divide(3, 4);
  for (int step = 0; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c->cd(hi++);

    TH2D* h = H2("fh_signal_minv_pt_" + CbmLmvmHist::fAnaSteps[step]);
    DrawH2(h, kLinear, kLinear, kLinear, "COLZ");
    DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.78, 0.70, 0.9);
  }
}

void CbmAnaDielectronTaskDraw::DrawBgSourcesVsMomentum()
{
  fHM->CreateCanvas("lmvm_source_mom_mc_signal", "lmvm_source_mom_mc_signal", 600, 600);
  DrawH1(H1("fh_source_mom_mc_signal"));

  int hi      = 1;
  TCanvas* c1 = fHM->CreateCanvas("lmvm_source_mom", "lmvm_source_mom", 900, 900);
  c1->Divide(3, 3);
  for (Int_t step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c1->cd(hi++);
    Draw1DSourceTypes("fh_source_mom_" + CbmLmvmHist::fAnaSteps[step], false);
    DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.90, 0.70, 0.99);
  }
  fHM->CreateCanvas("lmvm_source_mom_ttcut", "lmvm_source_mom_ttcut", 600, 600);
  Draw1DSourceTypes("fh_source_mom_" + CbmLmvmHist::fAnaSteps[kTtCut], false);

  hi          = 1;
  TCanvas* c3 = fHM->CreateCanvas("lmvm_source_pt", "lmvm_source_pt", 900, 900);
  c3->Divide(3, 3);
  for (Int_t step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c3->cd(hi++);
    Draw1DSourceTypes("fh_source_pt_" + CbmLmvmHist::fAnaSteps[step], false);
    DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.90, 0.70, 0.99);
  }
  fHM->CreateCanvas("lmvm_source_pt_ttcut", "lmvm_source_pt_ttcut", 600, 600);
  Draw1DSourceTypes("fh_source_pt_" + CbmLmvmHist::fAnaSteps[kTtCut], false);

  hi          = 1;
  TCanvas* c2 = fHM->CreateCanvas("lmvm_opening_angle", "lmvm_opening_angle", 900, 900);
  c2->Divide(3, 3);
  for (Int_t step = kReco; step < CbmLmvmHist::fNofAnaSteps; step++) {
    //cout << "fh_opening_angle_" << step << endl;
    if (!fUseMvd && (step == kMvd1Cut || step == kMvd2Cut)) continue;
    c2->cd(hi++);
    Draw1DSourceTypes("fh_opening_angle_" + CbmLmvmHist::fAnaSteps[step]);
    DrawTextOnPad(CbmLmvmHist::fAnaStepsLatex[step], 0.50, 0.90, 0.70, 0.99);
  }
  fHM->CreateCanvas("lmvm_opening_angle_ttcut", "lmvm_opening_angle_ttcut", 600, 600);
  Draw1DSourceTypes("fh_opening_angle_" + CbmLmvmHist::fAnaSteps[kTtCut], false);
}

void CbmAnaDielectronTaskDraw::DrawMvdCutQa()
{
  if (fUseMvd) {
    fHM->CreateCanvas("lmvm_mvd1cut_qa", "lmvm_mvd1cut_qa", 600, 600);
    Draw1DSourceTypes("fh_mvd1cut_qa");
    TH1D* h1 = H1("fh_mvd1cut_qa_" + CbmLmvmHist::fSourceTypes[0]);
    h1->GetXaxis()->SetLabelSize(0.06);
    h1->GetXaxis()->SetBinLabel(1, "Correct");
    h1->GetXaxis()->SetBinLabel(2, "Wrong");
    gPad->SetLogy(false);

    fHM->CreateCanvas("lmvm_mvd2cut_qa", "lmvm_mvd2cut_qa", 600, 600);
    Draw1DSourceTypes("fh_mvd2cut_qa");
    TH1D* h2 = H1("fh_mvd2cut_qa_" + CbmLmvmHist::fSourceTypes[0]);
    h2->GetXaxis()->SetLabelSize(0.07);
    h2->GetXaxis()->SetBinLabel(1, "Correct");
    h2->GetXaxis()->SetBinLabel(2, "Wrong");
    gPad->SetLogy(false);
  }
}

void CbmAnaDielectronTaskDraw::DrawMvdAndStsHist()
{
  if (!fUseMvd) return;
  TCanvas* c1 = fHM->CreateCanvas("lmvm_nofhits_mvd_sts", "lmvm_nofhits_mvd_sts", 900, 450);
  c1->Divide(2, 1);
  c1->cd(1);
  Draw1DSourceTypes("fh_nofMvdHits");
  c1->cd(2);
  Draw1DSourceTypes("fh_nofStsHits");

  Draw2DCut("fh_mvd1xy");
  fHM->CreateCanvas("lmvm_mvd1r", "lmvm_mvd1r", 600, 600);
  Draw1DSourceTypes("fh_mvd1r");


  Draw2DCut("fh_mvd2xy");
  fHM->CreateCanvas("lmvm_mvd2r", "lmvm_mvd2r", 600, 600);
  Draw1DSourceTypes("fh_mvd2r");
}


void CbmAnaDielectronTaskDraw::SaveCanvasToImage()
{
  fHM->SaveCanvasToImage(fOutputDir, "png");  // fHM->SaveCanvasToImage(fOutputDir, "png;eps");
}
