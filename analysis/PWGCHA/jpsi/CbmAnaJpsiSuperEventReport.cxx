
#include "CbmAnaJpsiSuperEventReport.h"
#include "CbmAnaJpsiCuts.h"
#include "CbmAnaJpsiHist.h"
#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmReportElement.h"
#include "CbmUtils.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMarker.h"
#include "TPad.h"
#include "TStyle.h"
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <map>

using boost::assign::list_of;
using Cbm::NumberToString;
using Cbm::Split;
using std::cout;
using std::endl;
using std::map;
using std::string;

CbmAnaJpsiSuperEventReport::CbmAnaJpsiSuperEventReport()
  : CbmSimulationReport(), fHMSuperEvent(), fHMEventByEvent() {
  SetReportName("jpsi_superevent_qa");
}

CbmAnaJpsiSuperEventReport::~CbmAnaJpsiSuperEventReport() {}

void CbmAnaJpsiSuperEventReport::Create(const string& fileEventByEvent,
                                        const string& fileSuperEvent,
                                        const string& outputDir) {
  fHMSuperEvent = new CbmHistManager();
  TFile* fileSE = new TFile(fileSuperEvent.c_str());
  fHMSuperEvent->ReadFromFile(fileSE);

  fHMEventByEvent = new CbmHistManager();
  TFile* fileEbE  = new TFile(fileEventByEvent.c_str());
  fHMEventByEvent->ReadFromFile(fileEbE);

  SetOutputDir(outputDir);
  CreateReports();
}
void CbmAnaJpsiSuperEventReport::Create() {
  //Out().precision(3);
  Out() << R()->DocumentBegin();
  Out() << R()->Title(0, GetTitle());

  PrintCanvases();

  Out() << R()->DocumentEnd();
}

void CbmAnaJpsiSuperEventReport::Draw() {
  SetDefaultDrawStyle();

  long nofSEEvents = fHMSuperEvent->H1("fh_se_event_number")->GetEntries();
  long nofSEEventsSquared = nofSEEvents * nofSEEvents;
  cout << "Number of SE events = " << nofSEEventsSquared << endl;
  fHMSuperEvent->ScaleByPattern(".*", 1. / (nofSEEventsSquared));

  int nofEEEvents = fHMEventByEvent->H1("fh_event_number")->GetEntries();
  cout << "Number of EE events = " << nofEEEvents << endl;
  fHMEventByEvent->ScaleByPattern(".*", 1. / nofEEEvents);


  Int_t nRebins = 20;
  fHMEventByEvent->RebinByPattern("fh_signal_minv.+", nRebins);
  fHMEventByEvent->RebinByPattern("fh_bg_minv.+", nRebins);
  fHMEventByEvent->RebinByPattern("fh_ee_signal_minv_diff_ptcuts.+", nRebins);
  fHMSuperEvent->RebinByPattern("fh_se_bg_participants_minv.+", nRebins);
  fHMSuperEvent->RebinByPattern("fh_se_bg_truematch.+", nRebins);
  fHMSuperEvent->RebinByPattern("fh_se_bg_mismatch.+", nRebins);


  fHMEventByEvent->ScaleByPattern("fh_signal_minv.+", nRebins);
  fHMEventByEvent->ScaleByPattern("fh_bg_minv.+", nRebins);
  fHMEventByEvent->ScaleByPattern("fh_ee_signal_minv_diff_ptcuts.+", nRebins);
  fHMSuperEvent->ScaleByPattern("fh_se_bg_participants_minv.+", nRebins);
  fHMSuperEvent->ScaleByPattern("fh_se_bg_truematch.+", nRebins);
  fHMSuperEvent->ScaleByPattern("fh_se_bg_mismatch.+", nRebins);

  DrawComparison();

  DrawMinvSignalBg();

  SignalOverBgAllSteps();

  DrawPairSource();

  {
    CreateCanvas("jpsi_fh_SE_PdgCode_of Others_BG",
                 "jpsi_fh_SE_PdgCode_of Others_BG",
                 1000,
                 1000);
    DrawH1(fHMSuperEvent->H1("fh_SE_PdgCode_of Others_BG"));
    DrawTextOnPad(
      CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.4, 0.89, 0.5, 0.99);
  }

  DrawMinvMismatchPt();

  DrawMinvDiffPtBins();
}

void CbmAnaJpsiSuperEventReport::DrawComparison() {
  CreateCanvas(
    "jpsi_se_fh_se_ee_bg_minv_reco", "jpsi_fh_se_ee_bg_minv_reco", 900, 900);
  DrawH1(list_of((TH1*) fHMEventByEvent->H1("fh_bg_minv_reco")->Clone())(
           (TH1*) fHMSuperEvent
             ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiReco])
             ->Clone()),
         list_of("Event-by-Event")("SuperEvent"),
         kLinear,
         kLinear,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiReco], 0.4, 0.89, 0.5, 0.99);


  CreateCanvas("jpsi_se_fh_se_ee_bg_minv_chi2prim",
               "jpsi_fh_se_ee_bg_minv_chi2prim",
               900,
               900);
  DrawH1(list_of((TH1*) fHMEventByEvent->H1("fh_bg_minv_chi2prim")->Clone())(
           (TH1*) fHMSuperEvent
             ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiChi2Prim])
             ->Clone()),
         list_of("Event-by-Event")("SuperEvent"),
         kLinear,
         kLinear,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiChi2Prim], 0.4, 0.89, 0.5, 0.99);


  CreateCanvas(
    "jpsi_se_fh_se_ee_bg_minv_elid", "jpsi_fh_se_ee_bg_minv_elid", 900, 900);
  DrawH1(list_of((TH1*) fHMEventByEvent->H1("fh_bg_minv_elid")->Clone())(
           (TH1*) fHMSuperEvent
             ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiElId])
             ->Clone()),
         list_of("Event-by-Event")("SuperEvent"),
         kLinear,
         kLinear,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiElId], 0.4, 0.89, 0.5, 0.99);


  CreateCanvas(
    "jpsi_se_fh_se_ee_bg_minv_ptcut", "jpsi_fh_se_ee_bg_minv_ptcut", 900, 900);
  DrawH1(list_of((TH1*) fHMEventByEvent->H1("fh_bg_minv_ptcut")->Clone())(
           (TH1*) fHMSuperEvent
             ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
             ->Clone()),
         list_of("Event-by-Event")("SuperEvent"),
         kLinear,
         kLinear,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.4, 0.89, 0.5, 0.99);
}

void CbmAnaJpsiSuperEventReport::DrawMinvSignalBg() {
  CreateCanvas("jpsi_se_minv_reco", "jpsi_se_minv_reco", 900, 900);
  fHMSuperEvent->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiReco])
    ->SetMinimum(1e-9);
  DrawH1(list_of((TH1*) fHMSuperEvent
                   ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiReco])
                   ->Clone())(
           (TH1*) fHMEventByEvent
             ->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiReco])
             ->Clone()),
         list_of("Background SuperEvent")("Signal Event-By-Event"),
         kLinear,
         kLog,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiReco], 0.4, 0.89, 0.5, 0.99);

  CreateCanvas("jpsi_se_minv_chi2prim", "jpsi_se_minv_chi2prim", 900, 900);
  fHMSuperEvent->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiChi2Prim])
    ->SetMinimum(1e-9);
  DrawH1(
    list_of((TH1*) fHMSuperEvent
              ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiChi2Prim])
              ->Clone())(
      (TH1*) fHMEventByEvent
        ->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiChi2Prim])
        ->Clone()),
    list_of("Background SuperEvent")("Signal Event-By-Event"),
    kLinear,
    kLog,
    true,
    0.7,
    0.9,
    0.99,
    0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiChi2Prim], 0.4, 0.89, 0.5, 0.99);

  CreateCanvas("jpsi_se_minv_elid", "jpsi_se_minv_elid", 900, 900);
  fHMSuperEvent->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiElId])
    ->SetMinimum(1e-9);
  DrawH1(list_of((TH1*) fHMSuperEvent
                   ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiElId])
                   ->Clone())(
           (TH1*) fHMEventByEvent
             ->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiElId])
             ->Clone()),
         list_of("Background SuperEvent")("Signal Event-By-Event"),
         kLinear,
         kLog,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiElId], 0.4, 0.89, 0.5, 0.99);

  CreateCanvas("jpsi_se_minv_ptcut", "jpsi_se_minv_ptcut", 900, 900);
  fHMSuperEvent->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
    ->SetMinimum(1e-9);
  DrawH1(
    list_of((TH1*) fHMSuperEvent
              ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
              ->Clone())(
      (TH1*) fHMEventByEvent
        ->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
        ->Clone()),
    list_of("Background SuperEvent")("Signal Event-By-Event"),
    kLinear,
    kLog,
    true,
    0.7,
    0.9,
    0.99,
    0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.4, 0.89, 0.5, 0.99);

  CreateCanvas("jpsi_se_ee_minv_ptcut", "jpsi_se_ee_minv_ptcut", 900, 900);
  TH1D* fhBg = (TH1D*) fHMSuperEvent
                 ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
                 ->Clone();
  TH1D* fhSignal =
    (TH1D*) fHMEventByEvent
      ->H1("fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
      ->Clone();
  TH1D* fhBgSignal = (TH1D*) fhBg->Clone();
  fhBgSignal->Add(fhSignal);
  fhBgSignal->SetMinimum(1e-9);
  DrawH1(list_of(fhBg)(fhSignal)(fhBgSignal),
         list_of("Background SuperEvent")("Signal Event-By-Event")(
           "Signal and Background"),
         kLinear,
         kLog,
         true,
         0.7,
         0.9,
         0.99,
         0.95);
  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.4, 0.89, 0.5, 0.99);
}

double CbmAnaJpsiSuperEventReport::SignalOverBg(int step) {
  TH1D* signal_ee = (TH1D*) fHMEventByEvent->H1(
    "fh_signal_minv_" + CbmAnaJpsiHist::fAnaSteps[step]);
  TH1D* bg_se = (TH1D*) fHMSuperEvent->H1("fh_se_bg_minv_"
                                          + CbmAnaJpsiHist::fAnaSteps[step]);

  //Create Histogram for the Gaus-Fit
  TH1D* signalFit_ee = (TH1D*) signal_ee->Clone();
  signalFit_ee->Fit("gaus", "", "", 2.9, 3.3);

  //Calculate sigma and Mean
  Double_t sigmaSignal =
    signalFit_ee->GetFunction("gaus")->GetParameter("Sigma");
  Double_t meanSignal = signalFit_ee->GetFunction("gaus")->GetParameter("Mean");

  //Get the number of the Bins of Min and Max
  int signalMin = signal_ee->FindBin(meanSignal - 2. * sigmaSignal);
  int signalMax = signal_ee->FindBin(meanSignal + 2. * sigmaSignal);

  double NOfSignalEntries = 0.;
  double NOfBgEntries     = 0.;

  //sum up all the bins
  for (int i = signalMin; i <= signalMax; i++) {
    NOfSignalEntries += signal_ee->GetBinContent(i);
    NOfBgEntries += bg_se->GetBinContent(i);
  }

  //Calculate Signal/Background
  if (NOfBgEntries <= 0.) {
    return 0.;
  } else {
    double sOverBg = NOfSignalEntries / NOfBgEntries;
    return sOverBg;
  }
}

void CbmAnaJpsiSuperEventReport::SignalOverBgAllSteps() {
  CreateCanvas("jpsi_fh_se_SignalOverBg_allAnaSteps",
               "jpsi_fh_se_SignalOverBg_allAnaSteps",
               600,
               600);
  fHMSuperEvent->Create1<TH1D>("fh_se_SignalOverBg_allAnaSteps",
                               "fh_se_SignalOverBg_allAnaSteps;AnaSteps;S/Bg",
                               4,
                               0,
                               4);


  for (int i = 0; i < CbmAnaJpsiHist::fNofAnaSteps - 2; i++) {
    double nstep   = i + 0.5;
    double SOverBg = SignalOverBg(i + 2);
    fHMSuperEvent->H1("fh_se_SignalOverBg_allAnaSteps")->Fill(nstep, SOverBg);
    gPad->SetLogy();
  }

  fHMSuperEvent->H1("fh_se_SignalOverBg_allAnaSteps")
    ->GetXaxis()
    ->SetLabelSize(0.06);
  int x = 1;
  for (Int_t step = 2; step < CbmAnaJpsiHist::fNofAnaSteps; step++) {
    fHMSuperEvent->H1("fh_se_SignalOverBg_allAnaSteps")
      ->GetXaxis()
      ->SetBinLabel(x, CbmAnaJpsiHist::fAnaStepsLatex[step].c_str());
    x++;
  }

  DrawH1(fHMSuperEvent->H1("fh_se_SignalOverBg_allAnaSteps"),
         kLinear,
         kLinear,
         "Text COLZ");
}

void CbmAnaJpsiSuperEventReport::DrawPairSource() {
  CreateCanvas("jpsi_fh_bg_pair_source", "jpsi_fh_bg_pair_source", 800, 800);

  DrawH1(
    list_of((TH1*) fHMSuperEvent
              ->H1("fh_se_bg_minv_" + CbmAnaJpsiHist::fAnaSteps[kJpsiPtCut])
              ->Clone())(fHMSuperEvent->H1("fh_se_bg_participants_minv_gg"))(
      fHMSuperEvent->H1("fh_se_bg_participants_minv_gp"))(
      fHMSuperEvent->H1("fh_se_bg_participants_minv_go"))(
      fHMSuperEvent->H1("fh_se_bg_participants_minv_pp"))(
      fHMSuperEvent->H1("fh_se_bg_participants_minv_po"))(
      fHMSuperEvent->H1("fh_se_bg_participants_minv_oo")),
    list_of("whole BG")("#gamma + #gamma")("#gamma + #pi^{0}")(
      "#gamma + others")("#pi^{0} + #pi^{0}")("#pi^{0} + others")(
      "others + others"),
    kLinear,
    kLog,
    true,
    0.85,
    0.6,
    0.99,
    0.99);

  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.6, 0.89, 0.7, 0.99);
}

void CbmAnaJpsiSuperEventReport::DrawMinvMismatchPt() {
  CreateCanvas("jpsi_fh_se_bg_Minv_Mismatch_pt",
               "jpsi_fh_se_bg_Minv_Mismatch_pt",
               800,
               800);

  double trueMatch =
    fHMSuperEvent->H1("fh_se_bg_truematch_minv_ptCut")->GetEntries();
  double trueMatchEl =
    fHMSuperEvent->H1("fh_se_bg_truematch_el_minv_ptCut")->GetEntries();
  double trueMatchNotEl =
    fHMSuperEvent->H1("fh_se_bg_truematch_notel_minv_ptCut")->GetEntries();
  double misMatch =
    fHMSuperEvent->H1("fh_se_bg_mismatch_minv_ptCut")->GetEntries();
  double nofBg = fHMSuperEvent->H1("fh_se_bg_minv_ptcut")->GetEntries();

  DrawH1(list_of(fHMSuperEvent->H1("fh_se_bg_truematch_minv_ptCut"))(
           fHMSuperEvent->H1("fh_se_bg_truematch_el_minv_ptCut"))(
           fHMSuperEvent->H1("fh_se_bg_truematch_notel_minv_ptCut"))(
           fHMSuperEvent->H1("fh_se_bg_mismatch_minv_ptCut")),
         list_of("true match ("
                 + Cbm::NumberToString(100. * trueMatch / nofBg, 1) + "%)")(
           "true match (e^{#pm}) ("
           + Cbm::NumberToString(100. * trueMatchEl / nofBg, 1) + "%)")(
           "true match (not e^{#pm}) ("
           + Cbm::NumberToString(100. * trueMatchNotEl / nofBg, 1) + "%)")(
           "mismatch (" + Cbm::NumberToString(100. * misMatch / nofBg) + "%)"),
         kLinear,
         kLog,
         true,
         0.65,
         0.8,
         0.99,
         0.99);

  DrawTextOnPad(
    CbmAnaJpsiHist::fAnaStepsLatex[kJpsiPtCut], 0.15, 0.9, 0.35, 0.99);
}

void CbmAnaJpsiSuperEventReport::DrawMinvDiffPtBins() {
  TCanvas* c6 = CreateCanvas(
    "jpsi_se_ee_minv_diff_ptCuts", "jpsi_se_ee_minv_diff_ptCuts", 1600, 800);
  c6->Divide(4, 2);
  for (int i = 0; i < 8; i++) {
    c6->cd(i + 1);
    TH1D* fhBgDiffPtCuts =
      (TH1D*) fHMSuperEvent
        ->H1("fh_se_bg_minv_diff_ptcuts_" + Cbm::NumberToString(i))
        ->Clone();
    TH1D* fhSignalDiffPtCuts =
      (TH1D*) fHMEventByEvent
        ->H1("fh_ee_signal_minv_diff_ptcuts_" + Cbm::NumberToString(i))
        ->Clone();
    TH1D* fhBgSignalDiffPtCuts = (TH1D*) fhBgDiffPtCuts->Clone();
    fhBgSignalDiffPtCuts->Add(fhSignalDiffPtCuts);
    fhBgSignalDiffPtCuts->SetMinimum(1e-9);

    double lowerCut = 0;
    double upperCut = 0;
    if (i < 6) {
      upperCut = (0.4 * (i + 1));
      lowerCut = (upperCut - 0.4);
    } else if (i == 6) {
      upperCut = 3.0;
      lowerCut = 2.4;
    } else if (i == 7) {
      upperCut = 6.0;
      lowerCut = 3.0;
    }
    string text = Cbm::NumberToString(lowerCut, 2) + "<P_t<"
                  + Cbm::NumberToString(upperCut, 2);

    //SignalOverBackground
    //Create Histogram for the Gaus-Fit
    TH1D* signalFit_DiffPt_ee = (TH1D*) fhSignalDiffPtCuts->Clone();
    signalFit_DiffPt_ee->Fit("gaus", "", "", 2.9, 3.3);

    //Calculate sigma and Mean
    Double_t sigmaSignal =
      signalFit_DiffPt_ee->GetFunction("gaus")->GetParameter("Sigma");
    Double_t meanSignal =
      signalFit_DiffPt_ee->GetFunction("gaus")->GetParameter("Mean");

    //Get the number of the Bins of Min and Max
    int signalMin = signalFit_DiffPt_ee->FindBin(meanSignal - 2. * sigmaSignal);
    int signalMax = signalFit_DiffPt_ee->FindBin(meanSignal + 2. * sigmaSignal);

    double NOfSignalEntries = 0.;
    double NOfBgEntries     = 0.;

    //sum up all the bins
    for (int j = signalMin; j <= signalMax; j++) {
      NOfSignalEntries += signalFit_DiffPt_ee->GetBinContent(j);
      NOfBgEntries += fhBgDiffPtCuts->GetBinContent(j);
    }

    //Calculate Signal/Background
    double SOverBg = 0.;
    if (NOfBgEntries <= 0.) {
      SOverBg = 0;
    } else {
      SOverBg = NOfSignalEntries / NOfBgEntries;
    }
    string textSOverBg =
      "S/Bg = " + Cbm::NumberToString(SOverBg, 3) + "    " + text;

    DrawH1(list_of(fhBgDiffPtCuts)(fhSignalDiffPtCuts)(fhBgSignalDiffPtCuts),
           list_of("Background")("Signal")("Signal and Background"),
           kLinear,
           kLog,
           true,
           0.65,
           0.8,
           0.99,
           0.95);
    DrawTextOnPad(textSOverBg, 0.07, 0.88, 0.65, 0.95);
  }
}

ClassImp(CbmAnaJpsiSuperEventReport)
