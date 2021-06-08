/* Copyright (C) 2015-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer] */

///////////////////////////////////////////////////////////////////////////
//                Dielectron SignalExt                                  //
//                                                                       //
//                                                                       //
/*
Ext class for signal extraction from a histogram or an array of histograms
The histogram is assumed to be an inv. mass spectrum,
the array of histograms is assumed to be an array with inv. mass histograms
resulting from single and mixed events, as defined in PairAnalysis.cxx

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include <TCollection.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TH1.h>
#include <TList.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TVectorT.h>
//#include <TH1.h>
#include <TCanvas.h>
#include <TGaxis.h>
#include <TH2F.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TMath.h>
#include <TString.h>

#include "PairAnalysis.h"
#include "PairAnalysisFunction.h"
#include "PairAnalysisHelper.h"
#include "PairAnalysisSignalExt.h"
#include "PairAnalysisSignalMC.h"
#include "PairAnalysisStyler.h"

ClassImp(PairAnalysisSignalExt)

  TH1F* PairAnalysisSignalExt::fgHistSimPM                      = 0x0;
TObject* PairAnalysisSignalExt::fgPeakShape                     = 0x0;
const char* PairAnalysisSignalExt::fgkValueNames[7]             = {"S",    "B",         "S/#sqrt{S+B}", "S/B",
                                                       "Mass", "MassWidth", "ChiSqNDFmatch"};
const char* PairAnalysisSignalExt::fgkBackgroundMethodNames[11] = {"FittedMC",
                                                                   "Fitted",
                                                                   "like-sign",
                                                                   "like-sign (arithm.)",
                                                                   "like-sign #times R(#Deltam)",
                                                                   "like-sign (arithm.) #times R(#Deltam)",
                                                                   "fitted like-sign",
                                                                   "mixed event",
                                                                   "fitted mixed event",
                                                                   "track rotation",
                                                                   "cocktail"};

PairAnalysisSignalExt::PairAnalysisSignalExt() : PairAnalysisSignalExt("PairAnalysisSignalExt", "title")
{
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisSignalExt::PairAnalysisSignalExt(const char* name, const char* title)
  : PairAnalysisFunction(name, title)
  , fValues(7)
  , fErrors(7)
{
  //
  // Named Constructor
  //
}

//______________________________________________
PairAnalysisSignalExt::PairAnalysisSignalExt(const PairAnalysisSignalExt& c)
  : PairAnalysisFunction(c.GetName(), c.GetTitle())
  , fArrHists(c.fArrHists)
  , fArrCocktail(c.fArrCocktail)
  , fHistSignal(c.GetSignalHistogram())
  , fHistSB(c.GetSoverBHistogram())
  , fHistSgn(c.GetSignificanceHistogram())
  , fHistBackground(c.GetBackgroundHistogram())
  , fHistCocktail(c.GetCocktailHistogram())
  , fHistDataPM(c.GetUnlikeSignHistogram())
  , fHistRfactor(c.GetRfactorHistogram())
  , fHistSignalMC(c.GetMCSignalShape())
  , fValues(c.GetValues())
  , fErrors(c.GetErrors())
  , fIntMin(c.GetIntegralMin())
  , fIntMax(c.GetIntegralMax())
  , fRebin(c.GetRebin())
  ,
  // fRebinStat(1.), //TODO
  // fBinLimits(0x0), //TODO
  fMethod(c.GetMethod())
  , fScaleMin(c.GetScaleMin())
  , fScaleMax(c.GetScaleMax())
  , fScaleMin2(c.GetScaleMin2())
  , fScaleMax2(c.GetScaleMax2())
  , fScaleFactor(c.GetScaleFactor())
  , fPeakMethod(c.GetExtractionMethod())
// fExtrFunc(0x0) //TODO: needed
{
  //
  // Copy Constructor
  //
}

//______________________________________________
PairAnalysisSignalExt::~PairAnalysisSignalExt()
{
  //
  // Default Destructor
  //
  // TODO: add new datamembers
  if (fArrHists) delete fArrHists;
  if (fArrCocktail) delete fArrCocktail;
  if (fHistSignal) delete fHistSignal;
  if (fHistSB) delete fHistSB;
  if (fHistSgn) delete fHistSgn;
  if (fHistBackground) delete fHistBackground;
  if (fHistCocktail) delete fHistCocktail;
  if (fHistDataPP) delete fHistDataPP;
  if (fHistDataPM) delete fHistDataPM;
  if (fHistDataMM) delete fHistDataMM;
  if (fHistDataME) delete fHistDataME;
  if (fHistRfactor) delete fHistRfactor;
  if (fHistSignalMC) delete fHistSignalMC;
  if (fExtrFunc) delete fExtrFunc;
  if (fBinLimits) delete fBinLimits;
}

//______________________________________________
TPaveText* PairAnalysisSignalExt::DrawStats(Double_t x1 /*=0.*/, Double_t y1 /*=0.*/, Double_t x2 /*=0.*/,
                                            Double_t y2 /*=0.*/, TString opt /*="pRnbsSmrc"*/)
{
  //
  // Draw extracted values in a TPaveText
  //
  // with the corners x1,y2,x2,y2
  // use option string to select information displayed:
  // p := Particle name using fPOIpdg
  // R := integration Range
  // n := Number of signal counts
  // b := Background
  // s := signal-to-bgrd
  // S := Significance
  // m := mass position
  // r := mass resolution (sigma or fwhm)
  // c := matching Chi2/ndf
  //
  if (TMath::Abs(x1) < 1e-20 && TMath::Abs(x2) < 1e-20) {
    x1 = .6;
    x2 = .9;
    y1 = .7;
    y2 = .9;
  }
  if (y1 < 0.5) y2 = y1 + 0.025 * opt.Length();
  else
    y1 = y2 - 0.025 * opt.Length();

  // particle of interest
  //  TParticlePDG *fPOI = TDatabasePDG::Instance()->GetParticle(fPOIpdg);

  TPaveText* t = new TPaveText(x1, y1, x2, y2, "brNDC");
  t->SetFillColor(0);
  t->SetFillStyle(kFEmpty);
  t->SetBorderSize(0);
  t->SetTextAlign(12);
  if (opt.Contains("p")) t->AddText(Form("#bf{%s}", (fPOI ? fPOI->GetName() : "particle not found")));
  if (opt.Contains("R")) t->AddText(Form("%.2f < %s < %.2f GeV/c^{2}", fIntMin, "m_{inv}", fIntMax));
  if (opt.Contains("n")) t->AddText(Form("%s: %.4g #pm %.4g", fgkValueNames[0], fValues(0), fErrors(0)));
  if (opt.Contains("b")) t->AddText(Form("%s: %.4g #pm %.4g", fgkValueNames[1], fValues(1), fErrors(1)));
  Int_t smallS2B = (fValues(3) < 0.1 ? 1 : 100);
  if (opt.Contains("s"))
    t->AddText(Form("%s: %.2f #pm %.2f%s", fgkValueNames[3], fValues(3) * 100 / smallS2B, fErrors(3) * 100 / smallS2B,
                    smallS2B > 1 ? "" : "%"));
  if (opt.Contains("S")) t->AddText(Form("%s: %.1f #pm %.1f", fgkValueNames[2], fValues(2), fErrors(2)));
  if (opt.Contains("m") && fValues(4) > 0) t->AddText(Form("Mass: %.2f #pm %.2f GeV/c^{2}", fValues(4), fErrors(4)));
  if (opt.Contains("r") && fValues(5) > 0)
    t->AddText(Form("Mass res.: %.1f #pm %.1f MeV/c^{2}", 1000 * fValues(5), 1000 * fErrors(5)));
  if (opt.Contains("c") && fValues(6) > 0) t->AddText(Form("Match chi2/ndf.: %.1f", fValues(6)));
  t->Draw();

  return t;
}

//______________________________________________
void PairAnalysisSignalExt::Print(Option_t* /*option*/) const
{
  //
  // Print the statistics
  //
  printf("Signal : %.5g #pm %.5g (+-%.3g%%)\n", fValues(0), fErrors(0), 100 * fErrors(0) / fValues(0));
  printf("Backgnd: %.5g #pm %.5g (+-%.3g%%)\n", fValues(1), fErrors(1), 100 * fErrors(1) / fValues(1));
  printf("Signif : %.5g #pm %.5g (+-%.3g%%)\n", fValues(2), fErrors(2), 100 * fErrors(2) / fValues(2));
  printf("SoB    : %.5g #pm %.5g (+-%.3g%%)\n", fValues(3), fErrors(3), 100 * fErrors(3) / fValues(3));
  if (fValues(4) > 0) printf("Mass: %.5g #pm %.5g\n", fValues(4), fErrors(4));
  if (fValues(5) > 0) printf("Mass res.: %.5g #pm %.5g\n", fValues(5), fErrors(5));
  if (fValues(6) > 0) printf("Match chi2/ndf: %.5g \n", fValues(6));
  printf("Scale  : %.5g \n", fScaleFactor);
  printf("Mass int.: %.5g - %.5g \n", fIntMin, fIntMax);
}

//______________________________________________
Double_t PairAnalysisSignalExt::ScaleHistograms(TH1* histRaw, TH1* histBackground, Double_t intMin, Double_t intMax)
{
  //
  // scale histBackground to match the integral of histRaw in the interval intMin, intMax
  //

  //protect using over and underflow bins in normalisation calculation
  if (intMin < histRaw->GetXaxis()->GetXmin()) intMin = histRaw->GetXaxis()->GetXmin();
  if (intMin < histBackground->GetXaxis()->GetXmin()) intMin = histBackground->GetXaxis()->GetXmin();

  if (intMax > histRaw->GetXaxis()->GetXmax())
    intMax = histRaw->GetXaxis()->GetXmax() - histRaw->GetBinWidth(histRaw->GetNbinsX()) / 2.;
  if (intMax > histBackground->GetXaxis()->GetXmax())
    intMax = histBackground->GetXaxis()->GetXmax() - histBackground->GetBinWidth(histBackground->GetNbinsX()) / 2.;

  Double_t intRaw  = histRaw->Integral(histRaw->FindBin(intMin), histRaw->FindBin(intMax));
  Double_t intBack = histBackground->Integral(histBackground->FindBin(intMin), histBackground->FindBin(intMax));
  fScaleFactor     = intBack > 0 ? intRaw / intBack : 1.;
  // scale
  if (histBackground->GetDefaultSumw2()) histBackground->Sumw2();
  histBackground->Scale(fScaleFactor);

  return fScaleFactor;
}

//______________________________________________
Double_t PairAnalysisSignalExt::ScaleHistograms(TH1* histRaw, TH1* histBackground, Double_t intMin, Double_t intMax,
                                                Double_t intMin2, Double_t intMax2)
{
  //
  // scale histBackground to match the integral of "histRaw" in the interval "intMin", "intMax" and "intMin2", "intMax2"
  //

  if (TMath::Abs(intMin2 - intMax2) < 0.00001) return (ScaleHistograms(histRaw, histBackground, intMin, intMax));

  //protect using over and underflow bins in normalisation calculation
  if (intMin < histRaw->GetXaxis()->GetXmin()) intMin = histRaw->GetXaxis()->GetXmin();
  if (intMin < histBackground->GetXaxis()->GetXmin()) intMin = histBackground->GetXaxis()->GetXmin();

  if (intMax2 > histRaw->GetXaxis()->GetXmax())
    intMax2 = histRaw->GetXaxis()->GetXmax() - histRaw->GetBinWidth(histRaw->GetNbinsX()) / 2.;
  if (intMax2 > histBackground->GetXaxis()->GetXmax())
    intMax2 = histBackground->GetXaxis()->GetXmax() - histBackground->GetBinWidth(histBackground->GetNbinsX()) / 2.;

  Double_t intRaw  = histRaw->Integral(histRaw->FindBin(intMin), histRaw->FindBin(intMax));
  Double_t intBack = histBackground->Integral(histBackground->FindBin(intMin), histBackground->FindBin(intMax));
  intRaw += histRaw->Integral(histRaw->FindBin(intMin2), histRaw->FindBin(intMax2));
  intBack += histBackground->Integral(histBackground->FindBin(intMin2), histBackground->FindBin(intMax2));

  fScaleFactor = intBack > 0 ? intRaw / intBack : 1.;
  // scale
  if (histBackground->GetDefaultSumw2()) histBackground->Sumw2();
  histBackground->Scale(fScaleFactor);

  return fScaleFactor;
}

//______________________________________________
TH1* PairAnalysisSignalExt::MergeObjects(TH1* obj1, TH1* obj2, Double_t val)
{
  //
  // function to merge all TH1 inherited objects
  // (needed because TProfile::Add with negative 'val' does not what is needed)
  // what about TProfile2D (inherits from TH2D) ?
  //
  if (!obj1) return obj2;
  if (!obj2) return obj1;

  TString key = Form("%s_%s", obj1->GetName(), "Signal");
  if (obj1->IsA() == TProfile2D::Class() && obj2->IsA() == TProfile2D::Class()) {
    if (val >= 0.) {
      //summation
      ((TProfile2D*) obj1)->Add(((TProfile2D*) obj2), val);
      return (TH1*) obj1->Clone(key.Data());  //TOCHECK: clone needed??
    }
    else
      return 0x0;
  }
  else if (obj1->IsA() == TProfile::Class() && obj2->IsA() == TProfile::Class()) {
    if (val >= 0.) {
      //summation
      ((TProfile*) obj1)->Add(((TProfile*) obj2), val);
      return (TH1*) obj1->Clone(key.Data());  //TOCHECK: clone needed??
    }
    else {
      // ONLY 1D subtraction
      TH1D* histSign = new TH1D(key.Data(), "", obj1->GetXaxis()->GetNbins(), obj1->GetXaxis()->GetXmin(),
                                obj1->GetXaxis()->GetXmax());
      if (histSign->GetDefaultSumw2()) histSign->Sumw2();
      histSign->SetDirectory(0);

      for (Int_t i = 0; i <= obj1->GetNbinsX(); i++) {
        histSign->SetBinContent(i, obj1->GetBinContent(i) - obj2->GetBinContent(i));
        histSign->SetBinError(
          i, TMath::Sqrt(obj1->GetBinError(i) * obj1->GetBinError(i) - obj2->GetBinError(i) * obj2->GetBinError(i)));
      }
      return histSign;
    }

    /*
      TList listH;
    TString listHargs;
    listHargs.Form("((TCollection*)0x%lx)", (ULong_t)&listH);
    Int_t error = 0;
    listH.Add(obj2);
    obj1->Execute("Merge", listHargs.Data(), &error);
    */
  }
  else {             // Histograms
    if (val < 0.) {  // subtraction
      TH1* histSign = (TH1*) obj1->Clone(key.Data());
      histSign->Add(obj2, val);
      return histSign;
    }
    else {  // merge
      obj1->Add(obj2, val);
      return obj1;
    }
  }
}

//______________________________________________
TObject* PairAnalysisSignalExt::DescribePeakShape(ESignalExtractionMethod method, Bool_t replaceValErr, TH1F* mcShape)
{
  //
  // Describe the extracted peak by the selected "method" and overwrite signal etc if activated
  //

  if (replaceValErr) fPeakMethod = method;
  Double_t data     = 0.;
  Double_t mc       = 0.;
  Double_t massPOI  = TDatabasePDG::Instance()->GetParticle(fPOIpdg)->Mass();
  Double_t sigPOI   = massPOI * 0.02;
  Double_t nPOI     = fHistSignal->GetBinContent(fHistSignal->FindBin(massPOI));
  Double_t binWidth = fHistSignal->GetBinWidth(fHistSignal->FindBin(massPOI));
  TF1* fit          = 0x0;
  Int_t fitResult   = 0;
  Int_t parMass     = -1;
  Int_t parSigma    = -1;
  if (!fExtrFunc) fExtrFunc = new PairAnalysisFunction();
  //PairAnalysisSignalFit *fExtrFunc = new PairAnalysisSignalFit();
  //  PairAnalysisSignalFunc fct;// = 0;//new PairAnalysisSignalFunc();

  Info("DescribePeakShape", "Signal extraction method: %d", (Int_t) fPeakMethod);

  // do the scaling/fitting
  switch (method) {
    case kBinCounting: /*nothing needs to be done*/ break;

    case kMCScaledMax:
      if (!mcShape) {
        Error("DescribePeakShape", "No MC histogram passed. Returning.");
        return 0x0;
      }
      data = fHistSignal->GetBinContent(fHistSignal->FindBin(massPOI));
      mc   = mcShape->GetBinContent(fHistSignal->FindBin(massPOI));
      mcShape->Scale(data / mc);
      break;

    case kMCScaledInt:
      if (!mcShape) {
        Error("DescribePeakShape", "No MC histogram passed. Returning.");
        return 0x0;
      }
      if (mcShape->GetBinWidth(1) != fHistSignal->GetBinWidth(1))
        printf(" WARNING: MC and signal histogram have different bin widths. \n");
      data = fHistSignal->Integral(fHistSignal->FindBin(fIntMin), fHistSignal->FindBin(fIntMax));
      mc   = mcShape->Integral(mcShape->FindBin(fIntMin), mcShape->FindBin(fIntMax));
      mcShape->Scale(data / mc);
      break;

    case kMCFitted:
      if (!mcShape && !fgHistSimPM) {
        printf(" ERROR: No MC histogram passed or set. Returning. \n");
        return 0x0;
      }
      if (!fgHistSimPM) fgHistSimPM = mcShape;
      if (fgHistSimPM->GetBinWidth(1) != fHistSignal->GetBinWidth(1)) fgHistSimPM->Rebin(fRebin);
      //    fit = new TF1("fitMC",PairAnalysisSignalFunc::PeakFunMC,fFitMin,fFitMax,1);
      fit = new TF1("MC", fExtrFunc, &PairAnalysisFunction::PeakFunMC, fFitMin, fFitMax, 1);
      fit->SetParNames("N");
      fitResult = fHistSignal->Fit(fit, "RNI0Q");
      break;

    case kCrystalBall:
      fit = new TF1("Crystal Ball", fExtrFunc, &PairAnalysisFunction::PeakFunCB, fFitMin, fFitMax, 5);
      fit->SetParNames("alpha", "n", "meanx", "sigma", "N");
      //  fit->SetParameters(-.2,5.,gMjpsi,.06,20);
      //  fit->SetParameters(1.,3.6,gMjpsi,.08,700);
      fit->SetParameters(0.4, 4.0, massPOI, sigPOI, 1.3 * nPOI);
      fit->SetParLimits(0, 0.0, 1.);
      fit->SetParLimits(1, 0.01, 10.);
      fit->SetParLimits(2, massPOI - sigPOI, massPOI + sigPOI);
      fit->SetParLimits(3, sigPOI / 5, 5 * sigPOI);
      fit->SetParLimits(4, 0.2 * nPOI, 2.0 * nPOI);
      parMass   = 2;
      parSigma  = 3;
      fitResult = fHistSignal->Fit(fit, "RNI0Q");
      break;

    case kGaus:
      fit = new TF1("Gaussisan", fExtrFunc, &PairAnalysisFunction::PeakFunGauss, fFitMin, fFitMax, 3);
      //fit = new TF1("fitGaus","gaus",fFitMin,fFitMax);
      fit->SetParNames("N", "meanx", "sigma");
      fit->SetParameters(1.3 * nPOI, massPOI, sigPOI);
      fit->SetParLimits(0, 0.2 * nPOI, 2.0 * nPOI);
      fit->SetParLimits(1, massPOI - sigPOI, massPOI + sigPOI);
      fit->SetParLimits(2, sigPOI / 5, 5 * sigPOI);
      parMass  = 1;
      parSigma = 2;
      //    fit->Print("V");
      fitResult = fHistSignal->Fit(fit, "RNI0");
      break;

    case kUserFunc:
      if (!fExtrFunc || !fExtrFunc->GetCombinedFunction()) {
        Fatal("DescribePeakShape", "Function class not added!");
        return 0x0;
      }
      fit       = fExtrFunc->GetCombinedFunction();
      fitResult = fHistSignal->Fit(fit, "RNI0Q");
      break;
  }

  // warning in case of fit issues
  if (fitResult != 0) Warning("DescripePeakShape", "fit has error/issue (%d)", fitResult);

  // store chi2/ndf of the fit
  if (fit) fValues(6) = (fit->GetNDF() ? fit->GetChisquare() / fit->GetNDF() : -1.);

  // overwrite values and errors if requested
  if (replaceValErr) {
    switch (method) {
      case kBinCounting:
        fValues(0) =
          fHistSignal->IntegralAndError(fHistSignal->FindBin(fIntMin), fHistSignal->FindBin(fIntMax), fErrors(0));
        SetSignificanceAndSOB();
        break;
      case kMCScaledMax:
      case kMCScaledInt:
        fValues(0) = mcShape->IntegralAndError(mcShape->FindBin(fIntMin), mcShape->FindBin(fIntMax), fErrors(0));
        SetSignificanceAndSOB();
        break;

      case kMCFitted:
      case kCrystalBall:
      case kGaus:
      case kUserFunc:
        fValues(0) = fit->Integral(fIntMin, fIntMax) / binWidth;
        fErrors(0) = fit->IntegralError(fIntMin, fIntMax) / binWidth;
        SetSignificanceAndSOB();
        break;
    }

    // set mass position and width
    if (parMass >= 0) {
      fValues(4) = fit->GetParameter(parMass);
      fErrors(4) = fit->GetParError(parMass);
    }
    if (parSigma >= 0) {
      fValues(5) = fit->GetParameter(parSigma);
      fErrors(5) = fit->GetParError(parSigma);
    }
    else {
      // calculate FWHM
      SetFWHM();
    }
  }

  // set the peak method obj
  switch (method) {
    case kBinCounting:
      if (replaceValErr) fgPeakShape = (TH1F*) fHistSignal->Clone("BinCount");
      //    delete fExtrFunc;
      return (TH1F*) fHistSignal->Clone("BinCountReturn");
      break;
    case kMCScaledMax:
    case kMCScaledInt:
      if (replaceValErr) fgPeakShape = mcShape;
      //    delete fExtrFunc;
      return mcShape;
      break;
    case kMCFitted:
    case kCrystalBall:
    case kGaus:
      if (fgHistSimPM) fit->SetName(Form("mcShapeFunc-%s", fgHistSimPM->GetName()));
      if (replaceValErr) fgPeakShape = fit;
      //    delete fExtrFunc;
      fPeakIsTF1 = kTRUE;
      return fit;
    case kUserFunc:
      fit->SetName(Form("UserFunc"));
      if (replaceValErr) fgPeakShape = fit;
      fit->Print();
      fPeakIsTF1 = kTRUE;
      break;
  }

  // printf("true integration range: %f %f \n",
  //  	 fHistSignal->GetBinLowEdge(fHistSignal->FindBin(fIntMin)),
  // 	 fHistSignal->GetBinLowEdge(fHistSignal->FindBin(fIntMax))+fHistSignal->GetBinWidth(fHistSignal->FindBin(fIntMax)));
  //delete fExtrFunc;
  //  if(fgPeakShape->IsA()==TF1::Class()) fPeakIsTF1=kTRUE;
  if (replaceValErr && fgPeakShape->IsA() == TF1::Class()) fPeakIsTF1 = kTRUE;
  return fgPeakShape;
}

//______________________________________________
void PairAnalysisSignalExt::Process(TObjArray* const arrhist)
{
  //
  // process signal subtraction
  //
  fArrHists = arrhist;
  fArrHists->SetOwner(kFALSE);

  // calculate optimal binning if configured
  if (fRebinStat < 1. && fBinLimits == 0x0) {
    fBinLimits =
      PairAnalysisHelper::MakeStatBinLimits((TH1*) FindObject(arrhist, PairAnalysis::EPairType::kSEPM), fRebinStat);
  }


  // clean up spectra
  if (fProcessed && 0) {  //TODO: not needed??
    if (fHistDataPP) {
      delete fHistDataPP;
      fHistDataPP = 0x0;
    }
    if (fHistDataPM) {
      delete fHistDataPM;
      fHistDataPM = 0x0;
    }
    if (fHistDataMM) {
      delete fHistDataMM;
      fHistDataMM = 0x0;
    }

    if (fHistDataME) {
      delete fHistDataME;
      fHistDataME = 0x0;
    }
    if (fHistRfactor) {
      delete fHistRfactor;
      fHistRfactor = 0x0;
    }

    if (fHistMixPP) {
      delete fHistMixPP;
      fHistMixPP = 0x0;
    }
    if (fHistMixPM) {
      delete fHistMixPM;
      fHistMixPM = 0x0;
    }
    if (fHistMixMM) {
      delete fHistMixMM;
      fHistMixMM = 0x0;
    }
    if (fHistMixMP) {
      delete fHistMixMP;
      fHistMixMP = 0x0;
    }

    if (fHistDataTR) {
      delete fHistDataTR;
      fHistDataTR = 0x0;
    }

    if (fHistSignal) {
      delete fHistSignal;
      fHistSignal = 0x0;
    }
    if (fHistBackground) {
      delete fHistBackground;
      fHistBackground = 0x0;
    }
    if (fHistSB) {
      delete fHistSB;
      fHistSB = 0x0;
    }
    if (fHistSgn) {
      delete fHistSgn;
      fHistSgn = 0x0;
    }
    if (fHistCocktail) {
      delete fHistCocktail;
      fHistCocktail = 0x0;
    }
  }

  //// get histograms and rebin
  // SE ++
  fHistDataPP = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kSEPP);
  if (fHistDataPP) {
    if (fBinLimits) {
      fHistDataPP = fHistDataPP->Rebin(fBinLimits->GetSize() - 1, "histPP", fBinLimits->GetArray());
      fHistDataPP->Scale(1., "width");
    }
    else
      fHistDataPP->Clone("histPP");
    if (fHistDataPP->GetDefaultSumw2()) fHistDataPP->Sumw2();
    fHistDataPP->SetDirectory(0);
    if (fRebin > 1) fHistDataPP->Rebin(fRebin);
  }
  // SE +-
  fHistDataPM = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kSEPM);
  if (fHistDataPM) {
    if (fBinLimits) {
      fHistDataPM = fHistDataPM->Rebin(fBinLimits->GetSize() - 1, "histPM", fBinLimits->GetArray());
      fHistDataPM->Scale(1., "width");
    }
    else
      fHistDataPM->Clone("histPM");
    if (fHistDataPM->GetDefaultSumw2()) fHistDataPM->Sumw2();
    fHistDataPM->SetDirectory(0);
    if (fRebin > 1) fHistDataPM->Rebin(fRebin);
    fHistDataPM->SetYTitle((fBinLimits ? "dN/dm" : "Counts"));
  }
  // SE --
  fHistDataMM = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kSEMM);
  if (fHistDataMM) {
    if (fBinLimits) {
      fHistDataMM = fHistDataMM->Rebin(fBinLimits->GetSize() - 1, "histMM", fBinLimits->GetArray());
      fHistDataMM->Scale(1., "width");
    }
    else
      fHistDataMM->Clone("histMM");
    if (fHistDataMM->GetDefaultSumw2()) fHistDataMM->Sumw2();
    fHistDataMM->SetDirectory(0);
    if (fRebin > 1) fHistDataMM->Rebin(fRebin);
  }
  // ME ++
  fHistMixPP = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kMEPP);
  if (fHistMixPP) {
    if (fBinLimits) {
      fHistMixPP = fHistMixPP->Rebin(fBinLimits->GetSize() - 1, "mixPP", fBinLimits->GetArray());
      fHistMixPP->Scale(1., "width");
    }
    else
      fHistMixPP->Clone("mixPP");
    if (fHistMixPP->GetDefaultSumw2()) fHistMixPP->Sumw2();
    fHistMixPP->SetDirectory(0);
    if (fRebin > 1) fHistMixPP->Rebin(fRebin);
  }
  // ME +-
  fHistMixPM = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kMEPM);
  if (fHistMixPM) {
    if (fBinLimits) {
      fHistMixPM = fHistMixPM->Rebin(fBinLimits->GetSize() - 1, "mixPM", fBinLimits->GetArray());
      fHistMixPM->Scale(1., "width");
    }
    else
      fHistMixPM->Clone("mixPM");
    if (fHistMixPM->GetDefaultSumw2()) fHistMixPM->Sumw2();
    fHistMixPM->SetDirectory(0);
    if (fRebin > 1) fHistMixPM->Rebin(fRebin);
  }
  // ME -+
  fHistMixMP = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kMEMP);
  if (fHistMixMP) {
    if (fBinLimits) {
      fHistMixMP = fHistMixMP->Rebin(fBinLimits->GetSize() - 1, "mixMP", fBinLimits->GetArray());
      fHistMixMP->Scale(1., "width");
    }
    else
      fHistMixMP->Clone("mixMP");
    if (fHistMixMP->GetDefaultSumw2()) fHistMixMP->Sumw2();
    fHistMixMP->SetDirectory(0);
    if (fRebin > 1) fHistMixMP->Rebin(fRebin);

    if (fHistMixPM) fHistMixMP->Add(fHistMixPM);  // merge ME +- and -+
  }
  // ME --
  fHistMixMM = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kMEMM);
  if (fHistMixMM) {
    if (fBinLimits) {
      fHistMixMM = fHistMixMM->Rebin(fBinLimits->GetSize() - 1, "mixMM", fBinLimits->GetArray());
      fHistMixMM->Scale(1., "width");
    }
    else
      fHistMixMM->Clone("mixMM");
    if (fHistMixMM->GetDefaultSumw2()) fHistMixMM->Sumw2();
    fHistMixMM->SetDirectory(0);
    if (fRebin > 1) fHistMixMM->Rebin(fRebin);
  }
  // TR +-
  fHistDataTR = (TH1*) FindObject(arrhist, PairAnalysis::EPairType::kSEPMRot);
  if (fHistDataTR) {
    if (fBinLimits) {
      fHistDataTR = fHistDataTR->Rebin(fBinLimits->GetSize() - 1, "histTR", fBinLimits->GetArray());
      fHistDataTR->Scale(1., "width");
    }
    else
      fHistDataTR->Clone("histTR");
    if (fHistDataTR->GetDefaultSumw2()) fHistDataTR->Sumw2();
    fHistDataTR->SetDirectory(0);
    if (fRebin > 1) fHistDataTR->Rebin(fRebin);
  }

  // init histograms for R-factor, subtracted signal, background
  fHistSignal = new TH1D("HistSignal", "Substracted signal", fHistDataPM->GetXaxis()->GetNbins(),
                         fHistDataPM->GetXaxis()->GetXbins()->GetArray());
  fHistSignal->SetXTitle(fHistDataPM->GetXaxis()->GetTitle());
  fHistSignal->SetYTitle(fHistDataPM->GetYaxis()->GetTitle());
  if (fHistSignal->GetDefaultSumw2()) fHistSignal->Sumw2();
  fHistSignal->SetDirectory(0);
  fHistBackground = new TH1D("HistBackground", "Background contribution", fHistDataPM->GetXaxis()->GetNbins(),
                             fHistDataPM->GetXaxis()->GetXbins()->GetArray());
  if (fHistBackground->GetDefaultSumw2()) fHistBackground->Sumw2();
  fHistBackground->SetDirectory(0);
  fHistRfactor = new TH1D("HistRfactor", "Rfactor;;N^{mix}_{+-}/2#sqrt{N^{mix}_{++} N^{mix}_{--}}",
                          fHistDataPM->GetXaxis()->GetNbins(), fHistDataPM->GetXaxis()->GetXbins()->GetArray());
  if (fHistRfactor->GetDefaultSumw2()) fHistRfactor->Sumw2();
  fHistRfactor->SetDirectory(0);

  // cocktail
  if (fArrCocktail && fArrCocktail->GetEntriesFast()) {
    //    printf("rebin %d cocktail histograms\n",fArrCocktail->GetEntriesFast());
    fHistCocktail = new TH1D("HistCocktail", "Cocktail contribution", fHistDataPM->GetXaxis()->GetNbins(),
                             fHistDataPM->GetXaxis()->GetXbins()->GetArray());
    fHistCocktail->SetXTitle(fHistDataPM->GetXaxis()->GetTitle());
    fHistCocktail->SetYTitle(fHistDataPM->GetYaxis()->GetTitle());
    if (fHistCocktail->GetDefaultSumw2()) fHistCocktail->Sumw2();
    fHistCocktail->SetDirectory(0);

    // loop over all ingredients
    for (Int_t i = 0; i < fArrCocktail->GetEntriesFast(); i++) {
      TH1* htmp = static_cast<TH1*>(fArrCocktail->UncheckedAt(i));
      if (fBinLimits) {
        htmp = htmp->Rebin(fBinLimits->GetSize() - 1, htmp->GetTitle(), fBinLimits->GetArray());
        if (htmp->GetDefaultSumw2()) htmp->Sumw2();
        htmp->SetDirectory(0);
        htmp->Scale(1., "width");
        fArrCocktail->AddAt(htmp, i);
      }
      //      else               htmp->Clone("histTR");
      // if(fHistDataTR->GetDefaultSumw2()) fHistDataTR->Sumw2();
      // fHistDataTR->SetDirectory(0);
      if (fRebin > 1) htmp->Rebin(fRebin);
      fHistCocktail->Add(htmp, +1.);
    }
  }

  /// rebin all other individualy
  /*
  for(Int_t i=0; i<arrhist->GetEntriesFast(); i++) {
    TH1* htmp = static_cast<TH1*>(arrhist->UncheckedAt(i));
    if( htmp->GetNbinsX()!=fHistSignal->GetNbinsX() ) {
      TArrayD *limits = PairAnalysisHelper::MakeStatBinLimits( htmp , fRebinStat);
      htmp = htmp->Rebin(limits->GetSize()-1,htmp->GetTitle(),limits->GetArray());
      if(htmp->GetDefaultSumw2()) htmp->Sumw2();
      htmp->SetDirectory(0);
      htmp->Scale(1.,"width");
      arrhist->AddAt(htmp,i);
    }
  }
  */

  // process method
  switch (fMethod) {
    case kLikeSign:
    case kLikeSignArithm:
    case kLikeSignRcorr:
    case kLikeSignArithmRcorr:
      ProcessLS();  // process like-sign subtraction method
      break;

    case kEventMixing:
      ProcessEM();  // process event mixing method
      break;

    case kRotation: ProcessTR(); break;

    case kCocktail:
      fCocktailSubtr = kTRUE;
      ProcessCocktail();
      break;

    default: Warning("Process", "Subtraction method not supported. Please check SetMethod() function.");
  }


  // set S/B and Significance histos
  if (!fPeakIsTF1) fHistSB = (TH1*) fgPeakShape->Clone("histSB");
  else {
    fHistSB = (TH1*) fHistSignal->Clone("histSB");
    fHistSB->Reset("CEIS");
    fHistSB->Eval((TF1*) fgPeakShape);
  }
  fHistSB->Divide(fHistBackground);
  fHistSB->SetYTitle("S/B");

  //significance
  fHistSgn = (TH1*) fHistSignal->Clone("histSB");
  fHistSgn->Reset("CEIS");
  Double_t s = 0.;
  Double_t b = 0.;
  for (Int_t i = 1; i <= fHistSgn->GetNbinsX(); i++) {

    if (!fPeakIsTF1) s = static_cast<TH1*>(fgPeakShape)->GetBinContent(i);
    else
      s = static_cast<TF1*>(fgPeakShape)->Eval(fHistSgn->GetBinCenter(i));
    b = fHistBackground->GetBinContent(i);

    if (s + b < 1.e-6) continue;
    fHistSgn->SetBinContent(i, s / TMath::Sqrt(s + b));
  }
  fHistSgn->SetYTitle("S/#sqrt{S+B}");
  //  fErrors(2) = ((s+b)>0 ? fValues(2)*TMath::Sqrt(be*be + TMath::Power(se*(s+2*b)/s, 2)) / 2 / (s+b) : 0);
}

//______________________________________________
void PairAnalysisSignalExt::ProcessLS()
{
  //
  // signal subtraction suing the like-sign method
  //

  /// TODO: set bin error of empty bins to 0.5*TMath::ChisquareQuantile(0.6827,2)
  /// according to PDG and RooFit (http://pdg.lbl.gov/2011/reviews/rpp2011-rev-statistics.pdf)
  /// The lower limit should of course be 0, use TGraphAsymmError of when fitting the distributions

  // protections
  if (!fHistDataPP || !fHistDataMM) return;

  // fill background histogram
  for (Int_t ibin = 1; ibin <= fHistDataPM->GetXaxis()->GetNbins(); ibin++) {
    Float_t pp  = fHistDataPP->GetBinContent(ibin);
    Float_t ppe = fHistDataPP->GetBinError(ibin);
    Float_t mm  = fHistDataMM->GetBinContent(ibin);
    Float_t mme = fHistDataMM->GetBinError(ibin);

    Float_t background = 2 * TMath::Sqrt(pp * mm);
    // Float_t ebackground = TMath::Sqrt(mm+pp); // do not use it since LS could be weighted err!=sqrt(entries)
    Float_t ebackground = TMath::Sqrt(mm / pp * ppe * ppe + pp / mm * mme * mme);
    // Arithmetic mean instead of geometric
    if (fMethod == kLikeSignArithm || fMethod == kLikeSignArithmRcorr) {
      background = (pp + mm);
      //      ebackground=TMath::Sqrt(pp+mm);
      ebackground = TMath::Sqrt(ppe * ppe + mme * mme);
      if (TMath::Abs(ebackground) < 1e-30) ebackground = 1;
    }
    // set contents
    fHistBackground->SetBinContent(ibin, background);
    fHistBackground->SetBinError(ibin, ebackground);
  }

  //correct LS spectrum bin-by-bin with R factor obtained in mixed events
  if (fMixingCorr || fMethod == kLikeSignRcorr || fMethod == kLikeSignArithmRcorr) {

    // protections
    if (!fHistMixPM || fHistMixPP || !fHistMixMM) return;

    // fill R-factor histogram
    for (Int_t ibin = 1; ibin <= fHistMixPM->GetXaxis()->GetNbins(); ibin++) {
      Float_t pp  = fHistMixPP->GetBinContent(ibin);
      Float_t ppe = fHistMixPP->GetBinError(ibin);
      Float_t mm  = fHistMixMM->GetBinContent(ibin);
      Float_t mme = fHistMixMM->GetBinError(ibin);
      Float_t pm  = fHistMixPM->GetBinContent(ibin);
      Float_t pme = fHistMixPM->GetBinError(ibin);

      Float_t background = 2 * TMath::Sqrt(pp * mm);
      // do not use it since ME could be weighted err!=sqrt(entries)
      // Float_t ebackground = TMath::Sqrt(mm+pp);
      Float_t ebackground = TMath::Sqrt(mm / pp * ppe * ppe + pp / mm * mme * mme);
      //Arithmetic mean instead of geometric
      if (fMethod == kLikeSignArithm) {
        background  = (pp + mm);
        ebackground = TMath::Sqrt(ppe * ppe + mme * mme);
        if (TMath::Abs(ebackground) < 1e-30) ebackground = 1;
      }

      Float_t rcon = 1.0;
      Float_t rerr = 0.0;
      if (background > 0.0) {
        rcon = pm / background;
        rerr = TMath::Sqrt((1. / background) * (1. / background) * pme * pme
                           + (pm / (background * background)) * (pm / (background * background)) * ebackground
                               * ebackground);
      }
      fHistRfactor->SetBinContent(ibin, rcon);
      fHistRfactor->SetBinError(ibin, rerr);
    }
    // correction
    fHistBackground->Multiply(fHistRfactor);
  }

  //scale histograms to match integral between fScaleMin and fScaleMax
  // or if fScaleMax <  fScaleMin use fScaleMin as scale factor
  ScaleBackground();

  //subtract background
  fHistSignal->Add(fHistDataPM);
  fHistSignal->Add(fHistBackground, -1);

  //subtract cocktail (if added)
  if (fCocktailSubtr) fHistSignal->Add(fHistCocktail, -1);

  // background
  fValues(1) =
    fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin), fHistBackground->FindBin(fIntMax), fErrors(1));

  // signal depending on peak description method
  DescribePeakShape(fPeakMethod, kTRUE, fgHistSimPM);
  //printf("%f  %f\n",fValues(0),fValues(1));
  //  SetSignificanceAndSOB();

  fProcessed = kTRUE;
}

//______________________________________________
void PairAnalysisSignalExt::ProcessEM()
{
  //
  // signal subtraction using event mixing (+-,-+) method
  //

  if (!fHistMixMP) {
    Error("ProcessEM", "Mixed event histogram missing");
    return;
  }


  // fill background histogram
  for (Int_t ibin = 1; ibin <= fHistDataPM->GetXaxis()->GetNbins(); ibin++) {
    Float_t pm  = fHistMixMP->GetBinContent(ibin);
    Float_t pme = fHistMixMP->GetBinError(ibin);

    Float_t background  = pm;
    Float_t ebackground = TMath::Sqrt(pme * pme);
    //TODO:      Float_t ebackground = TMath::Power(pm, 3./4);

    // set contents
    fHistBackground->SetBinContent(ibin, background);
    fHistBackground->SetBinError(ibin, ebackground);
  }

  //scale histograms to match integral between fScaleMin and fScaleMax
  // or if fScaleMax <  fScaleMin use fScaleMin as scale factor
  ScaleBackground();

  //subtract background
  fHistSignal->Add(fHistDataPM);
  fHistSignal->Add(fHistBackground, -1);
  //  fHistSignal = MergeObjects(fHistDataPM,fHistBackground,-1.);

  //subtract cocktail (if added)
  if (fCocktailSubtr) fHistSignal->Add(fHistCocktail, -1);

  // background
  fValues(1) =
    fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin), fHistBackground->FindBin(fIntMax), fErrors(1));

  // signal depending on peak description method
  DescribePeakShape(fPeakMethod, kTRUE, fgHistSimPM);

  fProcessed = kTRUE;
}

//______________________________________________
void PairAnalysisSignalExt::ProcessTR()
{
  //
  // signal subtraction using the track-rotation method
  //

  if (!fHistDataTR) {
    Error("ProcessTR", "Track rotation histogram missing");
    return;
  }

  // fill background histogram
  for (Int_t ibin = 1; ibin <= fHistDataPM->GetXaxis()->GetNbins(); ibin++) {
    Float_t pm  = fHistDataTR->GetBinContent(ibin);
    Float_t pme = fHistDataTR->GetBinError(ibin);

    Float_t background  = pm;
    Float_t ebackground = TMath::Sqrt(pme * pme);

    // set contents
    fHistBackground->SetBinContent(ibin, background);
    fHistBackground->SetBinError(ibin, ebackground);
  }

  // scale according to number of interations used in track rotation
  fHistBackground->Scale(1. / fNiterTR);

  //scale histograms to match integral between fScaleMin and fScaleMax
  // or if fScaleMax <  fScaleMin use fScaleMin as scale factor
  ScaleBackground();

  //subtract background
  fHistSignal->Add(fHistDataPM);
  fHistSignal->Add(fHistBackground, -1);

  //subtract cocktail (if added)
  if (fCocktailSubtr) fHistSignal->Add(fHistCocktail, -1);

  // background
  fValues(1) =
    fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin), fHistBackground->FindBin(fIntMax), fErrors(1));

  // signal depending on peak description method
  DescribePeakShape(fPeakMethod, kTRUE, fgHistSimPM);

  fProcessed = kTRUE;
}

//______________________________________________
void PairAnalysisSignalExt::ProcessCocktail()
{
  //
  // signal subtraction using the cocktail method
  //

  if (!fArrCocktail) {
    Error("ProcessCocktail", "Cocktail histograms missing");
    return;
  }

  // fill background histogram
  if (fArrCocktail) fHistBackground = fHistCocktail;

  //scale histograms to match integral between fScaleMin and fScaleMax
  // or if fScaleMax <  fScaleMin use fScaleMin as scale factor
  ScaleBackground();

  //subtract background
  fHistSignal->Add(fHistDataPM);
  fHistSignal->Add(fHistBackground, -1);

  // background
  fValues(1) =
    fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin), fHistBackground->FindBin(fIntMax), fErrors(1));

  // signal depending on peak description method
  DescribePeakShape(fPeakMethod, kTRUE, fgHistSimPM);

  fProcessed = kTRUE;
}

//______________________________________________
void PairAnalysisSignalExt::Draw(const Option_t* option)
{
  ///
  /// Draw signal extraction histograms into a canvas
  ///
  /// additional plotting options to TH1::Draw():
  ///
  /// "noMc":       no mc signals are plotted
  /// "onlyMc":     only mc signals are plotted
  /// "onlySig":    only background subtracted signals are plotted
  /// "onlyRaw":    only the raw unlike-sign spectra are plotted
  /// "cocktail":   plot the sum of cocktail contributors added via SetCocktailContribution
  ///
  /// "can":        canvas is created with name: "cSignalExtraction" or re-used
  /// "logx,y,z":   the axis are plotted in log-scale (labels are added automatically according to the range)
  /// "leg(f)":     a ("filled") legend will be created with caption=className ,
  ///               can be modified by PairAnalysisHistos::SetName("mycaption"),
  ///               change of legend position: see PairAnalysisStyler::SetLegendAlign
  ///
  /// "line":      draws a dashed line at zero
  /// "stat":      draws a statistics box with default entries at top-right position, see DrawStats
  ///
  /// "sb":        calculates and plots the signal-over-background ratio instead of counts
  /// "sgn":       calculates and plots the significance instead of counts

  Info("Draw", "Signal extraction results for '%s'", fgkBackgroundMethodNames[fMethod]);
  TString optString(option);
  optString.ToLower();
  printf("Plot extraction: bgrd. estimator: '%s' \t options: '%s' \n", fgkBackgroundMethodNames[fMethod],
         optString.Data());
  Bool_t optTask = optString.Contains("same");
  optString.ReplaceAll("same", "");
  Bool_t optLegFull = optString.Contains("legf");
  optString.ReplaceAll("legf", "");
  Bool_t optLeg = optString.Contains("leg");
  optString.ReplaceAll("leg", "");
  Bool_t optCan = optString.Contains("can");
  optString.ReplaceAll("can", "");
  Bool_t optLine = optString.Contains("line");
  optString.ReplaceAll("line", "");
  Bool_t optStat = optString.Contains("stat");
  optString.ReplaceAll("stat", "");
  Bool_t optSSB = optString.Contains("ssb");
  optString.ReplaceAll("ssb", "");
  Bool_t optSB = optString.Contains("sb");
  optString.ReplaceAll("sb", "");
  Bool_t optSgn = optString.Contains("sgn");
  optString.ReplaceAll("sgn", "");
  Bool_t optOnlyRaw = optString.Contains("onlyraw");
  optString.ReplaceAll("onlyraw", "");
  Bool_t optOnlySig = optString.Contains("onlysig");
  optString.ReplaceAll("onlysig", "");
  Bool_t optNoSig = optString.Contains("nosig");
  optString.ReplaceAll("nosig", "");
  Bool_t optNoBgrd = optString.Contains("nobgrd");
  optString.ReplaceAll("nobgrd", "");
  Bool_t optOnlyMC = optString.Contains("onlymc");
  optString.ReplaceAll("onlymc", "");
  Bool_t optNoMC = optString.Contains("nomc");
  optString.ReplaceAll("nomc", "");
  Bool_t optCocktail = optString.Contains("cocktail");
  optString.ReplaceAll("cocktail", "");
  optString.ReplaceAll(" ", "");

  /// load style
  PairAnalysisStyler::LoadStyle();

  /// activate std option for legend
  if (optLegFull) optLeg = kTRUE;

  /// add canvas
  TCanvas* c = 0;
  if (optCan) {
    TString key = Form("cSignalExtraction%s", (optSB ? "SB" : (optSgn ? "SGN" : (optSSB ? "SSB" : ""))));
    c           = (TCanvas*) gROOT->FindObject(key.Data());
    if (!c) c = new TCanvas(key.Data(), key.Data());
    //    c->Clear();
    c->cd();
  }

  Int_t nobj = 0;
  TObject* obj;
  /// count number of drawn objects in pad
  TList* prim = gPad->GetListOfPrimitives();
  for (Int_t io = 0; io < prim->GetSize(); io++) {
    obj = prim->At(io);
    if (obj->InheritsFrom(TH1::Class()) && obj != prim->At(io + 1)) nobj++;
  }

  // add or get legend
  TLegend* leg = 0;
  if ((optLeg && optTask && !nobj) || (optLeg && !optTask)) {
    leg = new TLegend(.75, .3, 1. - gPad->GetTopMargin() - gStyle->GetTickLength("X"),
                      1. - gPad->GetRightMargin() - gStyle->GetTickLength("Y"), fArrHists->GetName(), "nbNDC");
    if (optTask) leg->SetHeader("");
  }
  else if (optLeg && nobj) {
    leg = (TLegend*) prim->FindObject("TPave");
  }

  // logaritmic style
  gPad->SetLogx(optString.Contains("logx"));
  gPad->SetLogy(optString.Contains("logy"));
  gPad->SetLogz(optString.Contains("logz"));
  optString.ReplaceAll("logx", "");
  optString.ReplaceAll("logy", "");
  optString.ReplaceAll("logz", "");

  /// error style
  if (optString.Contains("e2") || optString.Contains("e3") || optString.Contains("e4")) {
    //    printf("set x-error style \n");
    gStyle->SetErrorX(0.5);
    //    PairAnalysisStyler::SetForceFillStyle(1);
  }

  Int_t i = nobj;  // TODO: obsolete?
  Info("Draw", "this is object number: %d", nobj);


  TString ytitle = fHistDataPM->GetYaxis()->GetTitle();
  // add bin width to y-axis title
  if (!(fHistDataPM->GetXaxis()->IsVariableBinSize())) {
    ytitle += Form("/%.0f MeV/#it{c}^{2}", fHistDataPM->GetBinWidth(1) * 1e3);
  }
  fHistDataPM->GetYaxis()->SetTitle(ytitle.Data());
  fHistSignal->GetYaxis()->SetTitle(ytitle.Data());

  // styling
  fHistDataPM->SetNameTitle(Form("unlike-sign%s", GetName()), "unlike-sign");
  fHistDataPM->UseCurrentStyle();
  PairAnalysisStyler::Style(fHistDataPM, static_cast<Int_t>(PairAnalysisStyler::Eidx::kRaw));
  if (fPlotMin != fPlotMax) fHistDataPM->SetAxisRange(fPlotMin, fPlotMax, "X");

  fHistBackground->SetNameTitle(fgkBackgroundMethodNames[fMethod], fgkBackgroundMethodNames[fMethod]);
  fHistBackground->UseCurrentStyle();
  PairAnalysisStyler::Style(fHistBackground, static_cast<Int_t>(PairAnalysisStyler::Eidx::kBgrd));

  fHistSignal->SetNameTitle(Form("signal%s", GetName()), "signal");
  fHistSignal->UseCurrentStyle();
  PairAnalysisStyler::Style(fHistSignal, static_cast<Int_t>(PairAnalysisStyler::Eidx::kSig));
  if (fPlotMin != fPlotMax) fHistSignal->SetAxisRange(fPlotMin, fPlotMax, "X");

  if (fHistCocktail) {
    fHistCocktail->SetNameTitle(Form("cocktail%s", GetName()), "cocktail");
    fHistCocktail->UseCurrentStyle();
    PairAnalysisStyler::Style(fHistCocktail, static_cast<Int_t>(PairAnalysisStyler::Eidx::kCocktail));
    if (fPlotMin != fPlotMax) fHistCocktail->SetAxisRange(fPlotMin, fPlotMax, "X");
  }

  if (optSB) {
    fHistSB->SetNameTitle(Form("s2b%s", GetName()), "signal");
    fHistSB->UseCurrentStyle();
    PairAnalysisStyler::Style(fHistSB, static_cast<Int_t>(PairAnalysisStyler::Eidx::kSig));
    if (fPlotMin != fPlotMax) fHistSB->SetAxisRange(fPlotMin, fPlotMax, "X");
  }
  else if (optSgn) {
    fHistSgn->SetNameTitle(Form("sgn%s", GetName()), "signal");
    fHistSgn->UseCurrentStyle();
    PairAnalysisStyler::Style(fHistSgn, static_cast<Int_t>(PairAnalysisStyler::Eidx::kSig));
    if (fPlotMin != fPlotMax) fHistSgn->SetAxisRange(fPlotMin, fPlotMax, "X");
  }

  // fHistRfactor->UseCurrentStyle();
  // fHistRfactor->SetTitle("");
  PairAnalysisStyler::Style(fHistRfactor, 3);

  fgPeakShape->UseCurrentStyle();
  //  fgPeakShape->SetTitle("");
  PairAnalysisStyler::Style(fgPeakShape, static_cast<Int_t>(PairAnalysisStyler::Eidx::kFit));

  // draw stuff
  if (c) c->cd(1);
  Info("Draw", "Start plotting stuff with option -%s-", optString.Data());
  TString drawOpt = (optString.IsNull() ? "EP" : optString);
  // draw spectra
  if (!optOnlyMC) {

    if (optSB && !optOnlyRaw && !optNoSig) {
      fHistSB->Draw(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
      i++;
    }
    else if (optSgn && !optOnlyRaw && !optNoSig) {
      fHistSgn->Draw(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
      i++;
    }
    else if (optOnlySig) {
      drawOpt = (optString.IsNull() ? "EP0" : optString);
      fHistSignal->DrawCopy(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
      i++;
      drawOpt = (optString.IsNull() ? "L same" : optString + "same");
      if (fPeakIsTF1) {
        static_cast<TF1*>(fgPeakShape)->DrawCopy(drawOpt.Data());
        i++;
      }
    }
    else if (!optSB && !optSgn) {
      fHistDataPM->DrawCopy(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
      i++;
      if (fMethod == kRotation || fMethod == kEventMixing || fMethod == kCocktail)
        drawOpt = (optString.IsNull() ? "HIST" : optString);
      if (!optNoBgrd) {
        fHistBackground->DrawCopy(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
        i++;
      }
      if (!optOnlyRaw && !optNoSig) {
        drawOpt = (optString.IsNull() ? "EP0" : optString);
        fHistSignal->DrawCopy(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
        i++;
        drawOpt = (optString.IsNull() ? "L same" : optString + "same");
        if (fPeakIsTF1) {
          static_cast<TF1*>(fgPeakShape)->DrawCopy(drawOpt.Data());
          i++;
        }
      }
    }
    // add cocktail
    if (optCocktail && fHistCocktail) {
      drawOpt = (optString.IsNull() ? "HIST" : optString);
      if (optSSB) {
        fHistCocktail->Divide(fHistDataPM);
        fHistCocktail->SetYTitle(Form("S/(S+B)"));
      }
      if (optSB) {
        fHistCocktail->Divide(fHistBackground);
        fHistCocktail->SetYTitle(Form("%s", GetValueName(3)));
      }
      if (optSgn) { FillSignificance(fHistCocktail, fHistCocktail, fHistBackground); }
      fHistCocktail->DrawCopy(i > 0 ? (drawOpt + "same").Data() : drawOpt.Data());
      i++;  //TODO: needed?
      //      fHistCocktail->DrawCopy((drawOpt+"same").Data()); i++;
    }
  }

  // draw MC signals
  if (!optNoMC) {
    if (!optSgn) {
      TIter nextObj(fArrHists);
      TH1* hmc;
      Int_t isty = 0;
      while ((hmc = dynamic_cast<TH1*>(nextObj()))) {
        TString key = hmc->GetTitle();  //hmc->GetName();
        TString tit = hmc->GetTitle();
        if (key.CountChar('_') != 1) continue;  // only reconstr. MC signals
        // remove cocktail subtracted signals
        if (optOnlySig && fCocktailSubtr && FindObjectByTitle(fArrCocktail, key)) continue;
        else if (optOnlySig && !fCocktailSubtr && fArrCocktail && !FindObjectByTitle(fArrCocktail, key))
          continue;
        else if (optCocktail && FindObjectByTitle(fArrCocktail, key))
          continue;
        PairAnalysisStyler::Style(hmc, isty++);
        // check if rebinning is necessary
        if (1 && fHistSignal->GetNbinsX() != hmc->GetNbinsX()) {
          if (fBinLimits) {
            hmc = hmc->Rebin(fBinLimits->GetSize() - 1, key.Data(), fBinLimits->GetArray());
            hmc->SetTitle(tit.Data());
            hmc->Scale(1., "width");
          }
          if (fRebin > 1) hmc->Rebin(fRebin);
        }
        if (optSSB) {
          hmc->Divide(fHistDataPM);
          hmc->SetYTitle(Form("S/(S+B)"));
        }
        if (optSB) {
          hmc->Divide(fHistBackground);
          hmc->SetYTitle(Form("%s", GetValueName(3)));
        }
        if (fPlotMin != fPlotMax) hmc->SetAxisRange(fPlotMin, fPlotMax, "X");
        hmc->Draw(i > 0 ? "HISTsame" : "HIST");
        i++;
      }
    }
  }

  /// automatic axis minimum and maximum
  Double_t max = -1e10;
  Double_t min = +1e10;
  //TListIter nextObj(gPad->GetListOfPrimitives(),kIterForward);
  TListIter nextObj(gPad->GetListOfPrimitives(), kIterBackward);
  //  TObject *obj;
  while ((obj = nextObj())) {
    if (obj->InheritsFrom(TH1::Class())) {
      TH1* hobj       = static_cast<TH1*>(obj);
      max             = TMath::Max(max,
                       PairAnalysisHelper::GetContentMaximum(hobj));  //hobj->GetMaximum();
      Double_t tmpmax = max * (gPad->GetLogy() ? 5. : 1.1);
      hobj->SetMaximum(tmpmax);

      Double_t objmin = PairAnalysisHelper::GetContentMinimum(hobj, kTRUE);
      if (gPad->GetLogy() && objmin < 0.) objmin = 0.5;
      min             = TMath::Min(min, objmin);
      Double_t tmpmin = min * (min < 0. ? 1.1 : 0.9);
      hobj->SetMinimum(tmpmin);

      // Printf("after %s max%f \t min%f \t for logy %.3e >? %.3e",
      // hobj->GetTitle(),tmpmax,tmpmin, tmpmax/(tmpmin>0.?tmpmin:1.),TMath::Power(10.,TGaxis::GetMaxDigits()));
      // automatically set log option
      if (gPad->GetLogy()
          && (tmpmax / (tmpmin > 0. ? tmpmin : 1.) > TMath::Power(10., TGaxis::GetMaxDigits())
              || tmpmin < TMath::Power(10., -TGaxis::GetMaxDigits()))) {
        hobj->GetYaxis()->SetMoreLogLabels(kFALSE);
        hobj->GetYaxis()->SetNoExponent(kFALSE);
      }
      //      if(gPad->GetLogy()) break;
    }
  }

  // add legend entries
  TListIter nextObjFwd(gPad->GetListOfPrimitives(), kIterForward);
  if (optLeg && leg) {
    nextObjFwd.Reset();
    Int_t ileg = 0;
    while ((obj = nextObjFwd())) {
      if (obj->InheritsFrom(TH1::Class()) || obj->InheritsFrom(TF1::Class())) {
        //       	TH1 *hleg = static_cast<TH1*>(obj);
        if (nobj && ileg < nobj) {
          ileg++;
          continue;
        }

        TString histClass = obj->GetTitle();  //hleg->GetName();
        if (histClass.IsNull()) histClass = obj->GetName();
        histClass.ReplaceAll("Pair.", "");
        histClass.ReplaceAll("Pair_", "");
        // change default signal names to titles
        for (Int_t isig = 0; isig < static_cast<Int_t>(PairAnalysisSignalMC::EDefinedSignal::kNSignals); isig++) {
          histClass.ReplaceAll(PairAnalysisSignalMC::fgkSignals[isig][0], PairAnalysisSignalMC::fgkSignals[isig][1]);
        }
        // remove trailing and leading spaces
        histClass.Remove(TString::kBoth, ' ');
        // histClass.Remove(TString::kBoth,'.');
        // histClass.Remove(TString::kBoth,'_');

        // modify legend option
        TString legOpt = obj->GetDrawOption();
        legOpt.ToLower();
        legOpt += "l";
        legOpt.ReplaceAll("hist", "");
        legOpt.ReplaceAll("same", "");
        legOpt.ReplaceAll("scat", "");
        legOpt.ReplaceAll("col", "");
        legOpt.ReplaceAll("z", "");
        legOpt.ReplaceAll("text", "");
        legOpt.ReplaceAll("e", "");
        //	Printf("hist %s legopt %s",histClass.Data(),legOpt.Data());
        if (ileg == nobj && optTask) leg->AddEntry((TObject*) 0x0, fArrHists->GetName(), "");
        leg->AddEntry(obj, histClass.Data(), legOpt.Data());
        ileg++;
      }
    }
  }

  /// draw legend only once
  /// set legend coordinates, margins, fillstyle, fontsize
  if (leg) {
    PairAnalysisStyler::SetLegendAttributes(leg, optLegFull);
    if (!nobj) leg->Draw();  // was w/o !nobj
  }

  // baseline
  TLine* line = new TLine();
  line->SetLineColor(kBlack);
  line->SetLineStyle(kDashed);
  if (optLine) line->DrawLine(fPlotMin, 0., fPlotMax, 0.);

  // draw statistics box
  if (optStat)
    DrawStats(0.7, gPad->GetBottomMargin() + gStyle->GetTickLength("Y"),
              1. - gPad->GetRightMargin() - gStyle->GetTickLength("X"),
              gPad->GetBottomMargin() + gStyle->GetTickLength("Y") + 5 * 0.025);


  // styling
  //  gPad->RedrawAxis();
}

void PairAnalysisSignalExt::ScaleBackground()
{
  //
  // scale histograms according to method and integral limits
  //

  TH1* scalingRef;
  //  scalingRef->Sumw2();
  switch (fSclMethod) {
    case kSclToRaw: scalingRef = fHistDataPM; break;
    case kSclToLikeSign:
      scalingRef = (TH1*) fHistDataPP->Clone();
      scalingRef->Add(fHistDataMM);
      break;  //arithmetic mean
    default: scalingRef = nullptr; break;
  }
  if (fScaleMax > fScaleMin && fScaleMax2 > fScaleMin2)
    fScaleFactor = ScaleHistograms(scalingRef, fHistBackground, fScaleMin, fScaleMax, fScaleMin2, fScaleMax2);
  else if (fScaleMax > fScaleMin)
    fScaleFactor = ScaleHistograms(scalingRef, fHistBackground, fScaleMin, fScaleMax);
  else if (fScaleMin > 0.) {
    fScaleFactor = fScaleMin;
    fHistBackground->Scale(fScaleFactor);
  }

  // clean up
  if (fSclMethod == kSclToLikeSign && scalingRef) delete scalingRef;
}
