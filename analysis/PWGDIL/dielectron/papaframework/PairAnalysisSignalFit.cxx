///////////////////////////////////////////////////////////////////////////
//                Dielectron SignalFit                                  //
//                                                                       //
//                                                                       //
/*

  Class used for extracting the signal from an invariant mass spectrum.
  It implements the PairAnalysisSignalBase and -Ext classes and it uses user provided
  functions to fit the spectrum with a combined signa+background fit.
  Used invariant mass spectra are provided via an array of histograms. There are serveral method
  to estimate the background and to extract the raw yield from the background subtracted spectra.

  Example usage:

  PairAnalysisSignalFit *sig = new PairAnalysisSignalFit();


  1) invariant mass input spectra

  1.1) Assuming a PairAnalysisCF container as data format (check class for more details)
  PairAnalysisCFdraw *cf = new PairAnalysisCFdraw("path/to/the/output/file.root");
  TObjArray *arrHists = cf->CollectMinvProj(cf->FindStep("Config"));

  1.2) Assuming a PairAnalysisHF grid as data format (check class for more details)
  PairAnalysisHFhelper *hf = new PairAnalysisHFhelper("path/to/the/output/file.root", "ConfigName");
  TObjArray *arrHists = hf->CollectHistos(PairAnalysisVarManager::kM);

  1.3) Assuming a single histograms
  TObjArray *histoArray = new TObjArray();
  arrHists->Add(signalPP);            // add the spectrum histograms to the array
  arrHists->Add(signalPM);            // the order is important !!!
  arrHists->Add(signalMM);


  2) background estimation

  2.1) set the method for the background estimation (methods can be found in PairAnalysisSignalBase)
  sig->SetMethod(PairAnalysisSignalBase::kFitted);
  2.2) rebin the spectras if needed
  //  sig->SetRebin(2);
  2.3) add any background function you like
  TF1 *fB = new TF1("fitBgrd","pol3",minFit,maxFit);


  3) configure the signal extraction

  3.1) chose one of the signal functions (MCshape, CrystalBall, Gauss)
  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunCB,minFit,maxFit,5); // has 5 parameters
  //  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunGaus,minFit,maxFit,3); // has 3 parameters
  //  sig->SetMCSignalShape(hMCsign);
  //  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunMC,minFit,maxFit,1); // requires a MC shape
  3.2) set the method for the signal extraction (methods can be found in PairAnalysisSignalBase)
  depending on the method serveral inputs are needed (e.g. MC shape, PDG code of the particle of interest)
  //  sig->SetParticleOfInterest(443); //default is jpsi
  //  sig->SetMCSignalShape(signalMC);
  //  sig->SetIntegralRange(minInt, maxInt);
  sig->SetExtractionMethod(PairAnalysisSignal::BinCounting); // this is the default


  4) combined fit of bgrd+signal

  4.1) combine the two functions
  sig->CombineFunc(fS,fB);
  4.2) apply fitting ranges and the fit options
  sig->SetFitRange(minFit, maxFit);
  sig->SetFitOption("NR");


  5) start the processing

  sig->Process(arrHists);
  sig->Print(""); // print values and errors extracted


  6) access the spectra and values created

  6.1) standard spectra as provided filled in PairAnalysisSignalExt
  TH1F *hsign = (TH1F*) sig->GetUnlikeSignHistogram();  // same as the input (rebinned)
  TH1F *hbgrd = (TH1F*) sig->GetBackgroundHistogram();  // filled histogram with fitBgrd
  TH1F *hextr = (TH1F*) sig->GetSignalHistogram();      // after backgound extraction (rebinned)
  TObject *oPeak = (TObject*) sig->GetPeakShape();      // can be a TF1 or TH1 depending on the method
  6.2) flow spectra
  TF1 *fFitSign  = sig->GetCombinedFunction();                // combined fit function
  TF1 *fFitExtr  = sig->GetSignalFunction();                  // signal function
  TF1 *fFitBgrd  = sig->GetBackgroundFunction();              // background function
  6.3) access the extracted values and errors
  sig->GetValues();     or GetErrors();                 // yield extraction

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMath.h>
#include <TString.h>
#include <TPaveText.h>
#include <TList.h>
#include <TFitResult.h>
//#include <../hist/hist/src/TF1Helper.h>

#include "PairAnalysisSignalFit.h"

ClassImp(PairAnalysisSignalFit)

//TH1F* PairAnalysisSignalFunc::fgHistSimPM=0x0;
// // TF1*  PairAnalysisSignalFunc::fFuncSignal=0x0;
// // TF1*  PairAnalysisSignalFunc::fFuncBackground=0x0;
// // Int_t PairAnalysisSignalFunc::fNparPeak=0;
// // Int_t PairAnalysisSignalFunc::fNparBgnd=0;


PairAnalysisSignalFit::PairAnalysisSignalFit() :
  PairAnalysisSignalExt()
//  ,PairAnalysisFunction()
{
  //
  // Default Constructor
  //

}

//______________________________________________
PairAnalysisSignalFit::PairAnalysisSignalFit(const char* name, const char* title) :
  PairAnalysisSignalExt(name, title)//,
  //  PairAnalysisFunction()
{
  //
  // Named Constructor
  //
}

//______________________________________________
PairAnalysisSignalFit::~PairAnalysisSignalFit()
{
  //
  // Default Destructor
  //

}


//______________________________________________
void PairAnalysisSignalFit::Process(TObjArray * const arrhist)
{
  //
  // Fit the invariant mass histograms and retrieve the signal and background
  //
  switch(fMethod) {
  case kFitted :
    ProcessFit(arrhist);
    break;

  case kLikeSignFit :
    ProcessFitLS(arrhist);
    break;

  case kEventMixingFit :
    ProcessFitEM(arrhist);
    break;

  case kLikeSign :
  case kLikeSignArithm :
  case kLikeSignRcorr:
  case kLikeSignArithmRcorr:
  case kEventMixing :
  case kRotation:
    PairAnalysisSignalExt::Process(arrhist);
    break;

  default :
    Error("Process","Background substraction method not known!");
  }
}

//______________________________________________
void PairAnalysisSignalFit::ProcessFit(TObjArray * const arrhist) {
  //
  // Fit the +- invariant mass distribution only
  // Here we assume that the combined fit function is a sum of the signal and background functions
  //    and that the signal function is always the first term of this sum
  //

  fHistDataPM = (TH1F*)(arrhist->At(1))->Clone("histPM");  // +-    SE
  fHistDataPM->Sumw2();
  if(fRebin>1)
    fHistDataPM->Rebin(fRebin);

  fHistSignal = new TH1F("HistSignal", "Fit substracted signal",
                         fHistDataPM->GetXaxis()->GetNbins(),
                         fHistDataPM->GetXaxis()->GetXmin(), fHistDataPM->GetXaxis()->GetXmax());
  fHistBackground = new TH1F("HistBackground", "Fit contribution",
                             fHistDataPM->GetXaxis()->GetNbins(),
                             fHistDataPM->GetXaxis()->GetXmin(), fHistDataPM->GetXaxis()->GetXmax());

  // the starting parameters of the fit function and their limits can be tuned
  // by the user in its macro
  fHistDataPM->Fit(fFuncSigBack, fFitOpt.Data(), "", fFitMin, fFitMax);
  TFitResultPtr pmFitPtr = fHistDataPM->Fit(fFuncSigBack, fFitOpt.Data(), "", fFitMin, fFitMax);
  //TFitResult *pmFitResult = pmFitPtr.Get(); // used only with TF1Helper
  //fFuncBackground->SetParameters(fFuncSigBack->GetParameters());
  fFuncSignal->SetParameters(fFuncSigBack->GetParameters());
  fFuncBackground->SetParameters(fFuncSigBack->GetParameters()+fFuncSignal->GetNpar());

  // fill the background spectrum
  fHistBackground->Eval(fFuncBackground);
  // set the error for the background histogram
  fHistBackground->Fit(fFuncBackground,"0qR","",fFitMin,fFitMax);
  Double_t inte  = fFuncBackground->IntegralError(fIntMin, fIntMax)/fHistDataPM->GetBinWidth(1);
  Double_t binte = inte / TMath::Sqrt((fHistDataPM->FindBin(fIntMax)-fHistDataPM->FindBin(fIntMin))+1);
  for(Int_t iBin=fHistDataPM->FindBin(fIntMin); iBin<=fHistDataPM->FindBin(fIntMax); iBin++) {
    fHistBackground->SetBinError(iBin, binte);
  }

  for(Int_t iBin=1; iBin<=fHistDataPM->GetXaxis()->GetNbins(); iBin++) {
    //    Double_t m = fHistDataPM->GetBinCenter(iBin);
    Double_t pm = fHistDataPM->GetBinContent(iBin);
    Double_t epm = fHistDataPM->GetBinError(iBin);
    Double_t bknd = fHistBackground->GetBinContent(iBin);
    Double_t ebknd = fHistBackground->GetBinError(iBin);
    Double_t signal = pm-bknd;
    Double_t error = TMath::Sqrt(epm*epm+ebknd);
    // theres no signal extraction outside the fit region
    if(fHistDataPM->GetBinLowEdge(iBin) > fFitMax ||
       fHistDataPM->GetBinLowEdge(iBin)+fHistDataPM->GetBinWidth(iBin) < fFitMin ) {
      signal=0.0;
      error=0.0;
    }
    fHistSignal->SetBinContent(iBin, signal);
    fHistSignal->SetBinError(iBin, error);
  }

  if(fUseIntegral) {
    // signal
    fValues(0) = fFuncSignal->Integral(fIntMin, fIntMax)/fHistDataPM->GetBinWidth(1);
    fErrors(0) = fFuncSignal->IntegralError(fIntMin, fIntMax)/fHistDataPM->GetBinWidth(1);
    //    fErrors(0) = 0;
    // background
    fValues(1) = fFuncBackground->Integral(fIntMin, fIntMax)/fHistDataPM->GetBinWidth(1);
    fErrors(1) = fFuncBackground->IntegralError(fIntMin, fIntMax)/fHistDataPM->GetBinWidth(1);
  }
  else {
    // signal
    fValues(0) = fHistSignal->IntegralAndError(fHistSignal->FindBin(fIntMin),
                                               fHistSignal->FindBin(fIntMax), fErrors(0));
    // background
    fValues(1) = fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin),
						   fHistBackground->FindBin(fIntMax),
						   fErrors(1));
  }
  // S/B and significance
  SetSignificanceAndSOB();
  fValues(4) = fFuncSigBack->GetParameter(fParMass);
  fErrors(4) = fFuncSigBack->GetParError(fParMass);
  fValues(5) = fFuncSigBack->GetParameter(fParMassWidth);
  fErrors(5) = fFuncSigBack->GetParError(fParMassWidth);

  // flag
  fProcessed = kTRUE;

  fHistBackground->GetListOfFunctions()->Add(fFuncBackground);

}

//______________________________________________
void PairAnalysisSignalFit::ProcessFitLS(TObjArray * const arrhist) {
  //
  // Substract background using the like-sign spectrum
  //
  fHistDataPP = (TH1F*)(arrhist->At(0))->Clone("histPP");  // ++    SE
  fHistDataPM = (TH1F*)(arrhist->At(1))->Clone("histPM");  // +-    SE
  fHistDataMM = (TH1F*)(arrhist->At(2))->Clone("histMM");  // --    SE
  if (fRebin>1) {
    fHistDataPP->Rebin(fRebin);
    fHistDataPM->Rebin(fRebin);
    fHistDataMM->Rebin(fRebin);
  }
  fHistDataPP->Sumw2();
  fHistDataPM->Sumw2();
  fHistDataMM->Sumw2();
  
  fHistSignal = new TH1F("HistSignal", "Like-Sign substracted signal",
                         fHistDataPM->GetXaxis()->GetNbins(),
                         fHistDataPM->GetXaxis()->GetXmin(), fHistDataPM->GetXaxis()->GetXmax());
  fHistBackground = new TH1F("HistBackground", "Like-sign contribution",
                             fHistDataPM->GetXaxis()->GetNbins(),
                             fHistDataPM->GetXaxis()->GetXmin(), fHistDataPM->GetXaxis()->GetXmax());
  
  // fit the +- mass distribution
  fHistDataPM->Fit(fFuncSigBack, fFitOpt.Data(), "", fFitMin, fFitMax);
  fHistDataPM->Fit(fFuncSigBack, fFitOpt.Data(), "", fFitMin, fFitMax);
  // declare the variables where the like-sign fit results will be stored
  //   TFitResult *ppFitResult = 0x0;
  //   TFitResult *mmFitResult = 0x0;
  // fit the like sign background
  TF1 *funcClonePP = (TF1*)fFuncBackground->Clone("funcClonePP");
  TF1 *funcCloneMM = (TF1*)fFuncBackground->Clone("funcCloneMM");
  fHistDataPP->Fit(funcClonePP, fFitOpt.Data(), "", fFitMin, fFitMax);
  //   TFitResultPtr ppFitPtr = fHistDataPP->Fit(funcClonePP, fFitOpt.Data(), "", fFitMin, fFitMax);
  //   ppFitResult = ppFitPtr.Get();
  fHistDataMM->Fit(funcCloneMM, fFitOpt.Data(), "", fFitMin, fFitMax);
  //   TFitResultPtr mmFitPtr = fHistDataMM->Fit(funcCloneMM, fFitOpt.Data(), "", fFitMin, fFitMax);
  //   mmFitResult = mmFitPtr.Get();

  for(Int_t iBin=1; iBin<=fHistDataPM->GetXaxis()->GetNbins(); iBin++) {
    Double_t m = fHistDataPM->GetBinCenter(iBin);
    Double_t pm = fHistDataPM->GetBinContent(iBin);
    Double_t pp = funcClonePP->Eval(m);
    Double_t mm = funcCloneMM->Eval(m);
    Double_t epm = fHistDataPM->GetBinError(iBin);
    // TODO: use TFitResults for errors?
    Double_t epp = 0;
    Double_t emm = 0;

    Double_t signal = pm-2.0*TMath::Sqrt(pp*mm);
    Double_t background = 2.0*TMath::Sqrt(pp*mm);

    // error propagation on the signal calculation above
    Double_t esignal = TMath::Sqrt(epm*epm+(mm/pp)*epp+(pp/mm)*emm);
    Double_t ebackground = TMath::Sqrt((mm/pp)*epp+(pp/mm)*emm);
    fHistSignal->SetBinContent(iBin, signal);
    fHistSignal->SetBinError(iBin, esignal);
    fHistBackground->SetBinContent(iBin, background);
    fHistBackground->SetBinError(iBin, ebackground);
  }

  // signal
  fValues(0) = fHistSignal->IntegralAndError(fHistSignal->FindBin(fIntMin),
                                             fHistSignal->FindBin(fIntMax), fErrors(0));
  // background
  fValues(1) = fHistBackground->IntegralAndError(fHistBackground->FindBin(fIntMin),
                                                 fHistBackground->FindBin(fIntMax),
                                                 fErrors(1));
  // S/B and significance
  SetSignificanceAndSOB();
  fValues(4) = fFuncSigBack->GetParameter(fParMass);
  fErrors(4) = fFuncSigBack->GetParError(fParMass);
  fValues(5) = fFuncSigBack->GetParameter(fParMassWidth);
  fErrors(5) = fFuncSigBack->GetParError(fParMassWidth);

  // flag
  fProcessed = kTRUE;

}

//______________________________________________
void PairAnalysisSignalFit::ProcessFitEM(TObjArray * const arrhist) {
  //
  // Substract background with the event mixing technique
  //
  arrhist->GetEntries();   // just to avoid the unused parameter warning
  Error("ProcessFitEM","Event mixing for background substraction method not yet implemented!");
}

//______________________________________________
void PairAnalysisSignalFit::Draw(const Option_t* option)
{
  //
  // Draw the fitted function
  //
  // TODO: update
  TString drawOpt(option);
  drawOpt.ToLower();
  
  Bool_t optStat=drawOpt.Contains("stat");
  
  fFuncSigBack->SetNpx(200);
  fFuncSigBack->SetRange(fIntMin,fIntMax);
  fFuncBackground->SetNpx(200);
  fFuncBackground->SetRange(fIntMin,fIntMax);
  
  TGraph *grSig=new TGraph(fFuncSigBack);
  grSig->SetFillColor(kGreen);
  grSig->SetFillStyle(3001);
  
  TGraph *grBack=new TGraph(fFuncBackground);
  grBack->SetFillColor(kRed);
  grBack->SetFillStyle(3001);
  
  grSig->SetPoint(0,grBack->GetX()[0],grBack->GetY()[0]);
  grSig->SetPoint(grSig->GetN()-1,grBack->GetX()[grBack->GetN()-1],grBack->GetY()[grBack->GetN()-1]);
  
  grBack->SetPoint(0,grBack->GetX()[0],0.);
  grBack->SetPoint(grBack->GetN()-1,grBack->GetX()[grBack->GetN()-1],0.);
  
  fFuncSigBack->SetRange(fFitMin,fFitMax);
  fFuncBackground->SetRange(fFitMin,fFitMax);
  
  if (!drawOpt.Contains("same")){
    if (fHistDataPM){
      fHistDataPM->Draw();
      grSig->Draw("f");
    } else {
      grSig->Draw("af");
    }
  } else {
    grSig->Draw("f");
  }
  if(fMethod==kFitted || fMethod==kFittedMC) grBack->Draw("f");
  fFuncSigBack->Draw("same");
  fFuncSigBack->SetLineWidth(2);
  if(fMethod==kLikeSign) {
    fHistDataPP->SetLineWidth(2);
    fHistDataPP->SetLineColor(6);
    fHistDataPP->Draw("same");
    fHistDataMM->SetLineWidth(2);
    fHistDataMM->SetLineColor(8);
    fHistDataMM->Draw("same");
  }
  
  if(fMethod==kFitted || fMethod==kFittedMC)
    fFuncBackground->Draw("same");
  
  if (optStat) DrawStats();
  
}

//______________________________________________
void PairAnalysisSignalFit::Print(Option_t */*option*/) const
{
  //
  // Print the statistics
  //
  PairAnalysisSignalBase::Print();
  printf("Fit int.  :  %.5g - %.5g \n",fFitMin,fFitMax);
  printf("Fit chi/%d:  %.5g \n",fDof,fChi2Dof);

}
