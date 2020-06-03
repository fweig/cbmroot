///////////////////////////////////////////////////////////////////////////
//                Dielectron Function                                  //
//                                                                       //
//                                                                       //
/*

  1) add any background function you like
  TF1 *fB = new TF1("fitBgrd","pol3",minFit,maxFit);

  2) configure the signal extraction

  2.1) chose one of the signal functions (MCshape, CrystalBall, Gauss)
  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunCB,minFit,maxFit,5); // has 5 parameters
  //  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunGaus,minFit,maxFit,3); // has 3 parameters
  //  sig->SetMCSignalShape(hMCsign);
  //  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunMC,minFit,maxFit,1); // requires a MC shape


  3) combined fit of bgrd+signal

  3.1) combine the two functions
  sig->CombineFunc(fS,fB);
  3.2) apply fitting ranges and the fit options
  sig->SetFitRange(minFit, maxFit);
  sig->SetFitOption("NR");


  6) access the spectra and values created

  6.1) fit function
  TF1 *fFitSign  = sig->GetCombinedFunction();                // combined fit function
  TF1 *fFitExtr  = sig->GetSignalFunction();                  // signal function
  TF1 *fFitBgrd  = sig->GetBackgroundFunction();              // background function

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

#include "PairAnalysisFunction.h"

ClassImp(PairAnalysisFunction)


PairAnalysisFunction::PairAnalysisFunction() :
  TNamed("function","function")
{
  //
  // Default Constructor
  //

}

//______________________________________________
PairAnalysisFunction::PairAnalysisFunction(const char* name, const char* title) :
  TNamed(name, title)
{
  //
  // Named Constructor
  //
}

//______________________________________________
PairAnalysisFunction::~PairAnalysisFunction()
{
  //
  // Default Destructor
  //
  if(fFuncSigBack) delete fFuncSigBack;
  if(fFuncSignal) delete fFuncSignal;
  if(fFuncBackground) delete fFuncBackground;
}


//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunMC(const Double_t *x, const Double_t *par) {
  // Fit MC signal shape
  // parameters
  // [0]:   scale for simpeak

  Double_t xx  = x[0];

  TH1F *hPeak = fgHistSimPM;
  if (!hPeak) {
    printf("E-PairAnalysisFunction::PeakFun: No histogram for peak fit defined!\n");
    return 0.0;
  }

  Int_t idx = hPeak->FindBin(xx);
  if ((idx <= 1) ||
      (idx >= hPeak->GetNbinsX())) {
    return 0.0;
  }

  return (par[0] * hPeak->GetBinContent(idx));
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunCB(const Double_t *x, const Double_t *par) {
  // Crystal Ball fit function

  Double_t     n = par[0];
  Double_t alpha = par[1];
  Double_t meanx = par[2];
  Double_t sigma = par[3];
  Double_t    nn = par[4];

  Double_t a = TMath::Power((n/TMath::Abs(alpha)), n) * TMath::Exp(-.5*alpha*alpha);
  Double_t b = n/TMath::Abs(alpha) - TMath::Abs(alpha);

  Double_t arg = (x[0] - meanx)/sigma;
  Double_t fitval = 0;

  if (arg > -1.*alpha) {
    fitval = nn * TMath::Exp(-.5*arg*arg);
  } else {
    fitval = nn * a * TMath::Power((b-arg), (-1*n));
  }

  return fitval;
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunGaus(const Double_t *x, const Double_t *par) {
  // Gaussian fit function
  //printf("fNparBgrd %d \n",fNparBgnd);
  Double_t     n = par[0];
  Double_t  mean = par[1];
  Double_t sigma = par[2];
  Double_t    xx = x[0];

  return ( n*TMath::Exp(-0.5*TMath::Power((xx-mean)/sigma,2)) );
}

//______________________________________________
void PairAnalysisFunction::SetFunctions(TF1 * const combined, TF1 * const sig, TF1 * const back,
					  Int_t parM, Int_t parMres)
{
  //
  // Set the signal, background functions and combined fit function
  // Note: The process method assumes that the first n parameters in the
  //       combined fit function correspond to the n parameters of the signal function
  //       and the n+1 to n+m parameters to the m parameters of the background function!!!

  if (!sig||!back||!combined) {
    Error("SetFunctions","Both, signal and background function need to be set!");
    return;
  }
  fFuncSignal=sig;
  fFuncBackground=back;
  fFuncSigBack=combined;
  fParMass=parM;
  fParMassWidth=parMres;

}

//______________________________________________
void PairAnalysisFunction::SetDefaults(Int_t type)
{
  //
  // Setup some default functions:
  // type = 0: gaus signal + linear background in 2.5 - 4 GeV inv. mass
  // type = 1: gaus signal + exponential background in 2.5 - 4 GeV inv. mass
  // type = 2: half gaussian, half exponential signal function
  // type = 3: Crystal-Ball function
  // type = 4: Crystal-Ball signal + exponential background
  //
  // TODO: use PDG mass and width + fPOI for parameter settings

  if (type==0){
    fFuncSignal=new TF1("DieleSignal","gaus",2.5,4);
    fFuncBackground=new TF1("DieleBackground","pol1",2.5,4);
    fFuncSigBack=new TF1("DieleCombined","gaus+pol1(3)",2.5,4);

    fFuncSigBack->SetParameters(1,3.1,.05,2.5,1);
    fFuncSigBack->SetParLimits(0,0,10000000);
    fFuncSigBack->SetParLimits(1,3.05,3.15);
    fFuncSigBack->SetParLimits(2,.02,.1);
  }
  else if (type==1){
    fFuncSignal=new TF1("DieleSignal","gaus",2.5,4);
    fFuncBackground=new TF1("DieleBackground","[0]*exp(-(x-[1])/[2])",2.5,4);
    fFuncSigBack=new TF1("DieleCombined","gaus+[3]*exp(-(x-[4])/[5])",2.5,4);

    fFuncSigBack->SetParameters(1,3.1,.05,1,2.5,1);
    fFuncSigBack->SetParLimits(0,0,10000000);
    fFuncSigBack->SetParLimits(1,3.05,3.15);
    fFuncSigBack->SetParLimits(2,.02,.1);
  }
  else if (type==2){
    // half gaussian, half exponential signal function
    // exponential background
    fFuncSignal = new TF1("DieleSignal","(x<[1])*([0]*(exp(-0.5*((x-[1])/[2])^2)+exp((x-[1])/[3])*(1-exp(-0.5*((x-[1])/[2])^2))))+(x>=[1])*([0]*exp(-0.5*((x-[1])/[2])^2))",2.5,4);
    fFuncBackground = new TF1("DieleBackground","[0]*exp(-(x-[1])/[2])+[3]",2.5,4);
    fFuncSigBack = new TF1("DieleCombined","(x<[1])*([0]*(exp(-0.5*((x-[1])/[2])^2)+exp((x-[1])/[3])*(1-exp(-0.5*((x-[1])/[2])^2))))+(x>=[1])*([0]*exp(-0.5*((x-[1])/[2])^2))+[4]*exp(-(x-[5])/[6])+[7]",2.5,4);
    fFuncSigBack->SetParameters(1.,3.1,.05,.1,1,2.5,1,0);

    fFuncSigBack->SetParLimits(0,0,10000000);
    fFuncSigBack->SetParLimits(1,3.05,3.15);
    fFuncSigBack->SetParLimits(2,.02,.1);
    fFuncSigBack->FixParameter(6,2.5);
    fFuncSigBack->FixParameter(7,0);
  }
}

//______________________________________________________________________________
void PairAnalysisFunction::CombineFunc(TF1 * const peak, TF1 * const bgnd) {
  //
  // combine the bgnd and the peak function
  //

  if (!peak||!bgnd) {
    Error("CombineFunc","Both, signal and background function need to be set!");
    return;
  }
  fFuncSignal=peak;
  fFuncBackground=bgnd;

  fNparPeak     = fFuncSignal->GetNpar();
  fNparBgnd     = fFuncBackground->GetNpar();

  fFuncSigBack = new TF1("BgndPeak",this,&PairAnalysisFunction::PeakBgndFun, fFitMin,fFitMax, fNparPeak+fNparBgnd);
  return;
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakBgndFun(const Double_t *x, const Double_t *par) {
  //
  // merge peak and bgnd functions
  //
  return (fFuncSignal->EvalPar(x,par) + (fFuncBackground?fFuncBackground->EvalPar(x,par+fNparPeak):0.));
}

//______________________________________________
void PairAnalysisFunction::Print(Option_t */*option*/) const
{
  //
  // Print the statistics
  //
  printf("Fit int.  :  %.5g - %.5g \n",fFitMin,fFitMax);
  printf("Fit chi/%d:  %.5g \n",fDof,fChi2Dof);

}
