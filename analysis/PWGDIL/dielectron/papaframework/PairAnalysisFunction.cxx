///////////////////////////////////////////////////////////////////////////
//                PairAnalysis Function                                  //
//                                                                       //
//                                                                       //
/*

  1) add any background function you like
  TF1 *fB = new TF1("fitBgrd","pol3",minFit,maxFit);

  2) configure the signal extraction

  2.1) chose one of the signal functions (MCshape, CrystalBall, Gauss, PowGaussPow, ExpGaussExp)
  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunCB,minFit,maxFit,5); // has 5 parameters
  //  sig->SetMCSignalShape(hMCsign);
  //  TF1 *fS = new TF1("fitSign",PairAnalysisFunction::PeakFunMC,minFit,maxFit,1); // requires a MC shape

  OR

  2.2) one of the other predefined function (Boltzmann, PtExp, Hagedorn, Levi)


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

#include <TNamed.h>
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

TH1F* PairAnalysisFunction::fgHistSimPM=0x0;

PairAnalysisFunction::PairAnalysisFunction() :
  PairAnalysisFunction("PairAnalysisFunction","fcttitle")
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
PairAnalysisFunction::PairAnalysisFunction(const PairAnalysisFunction &c) :
  TNamed(c.GetName(), c.GetTitle()),
  fFitMin(c.GetFitMin()),
  fFitMax(c.GetFitMax()),
   fPOIpdg(c.GetParticleOfInterest())
 {
  //
  // Copy Constructor
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
    fitval = nn * TMath::Exp(-.5*arg*arg); //gaussian part
  } else {
    fitval = nn * a * TMath::Power((b-arg), (-1*n));
  }

  return fitval;
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunPowGaussPow(const Double_t *x, const Double_t *par) {
  // PowGaussPow function fit function (both sided Crystall Ball)

  Double_t     n = par[0];
  Double_t alpha = par[1];
  Double_t    nn = par[4];
  Double_t meanx = par[2];
  Double_t sigma = par[3];

  Double_t a = TMath::Power((n/TMath::Abs(alpha)), n) * TMath::Exp(-.5*alpha*alpha);
  Double_t b = n/TMath::Abs(alpha) - TMath::Abs(alpha);

  Double_t arg = (x[0] - meanx)/sigma;
  Double_t fitval = 0;

  if (arg > alpha) {
    fitval = nn * a * TMath::Power((b+arg), (-1*n));
  } else if (arg < -alpha) {
    fitval = nn * a * TMath::Power((b-arg), (-1*n));
  } else {
    fitval = nn * TMath::Exp(-0.5*arg*arg); //gaussian part
  }

  return fitval;
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunExpGaussExp(const Double_t *x, const Double_t *par) {
  // ExpGaussExp function fit function

  Double_t     n = par[0];
  Double_t alpha = par[1];

  Double_t meanx = par[2];
  Double_t sigma = par[3];

  Double_t arg = (x[0] - meanx)/sigma;
  Double_t fitval = 0;

  if (arg > alpha) {
    fitval = n * TMath::Exp(-0.5*alpha*alpha - alpha*arg);
  } else if (arg < -alpha) {
    fitval = n * TMath::Exp(-0.5*alpha*alpha + alpha*arg);
  } else {
    fitval = n * TMath::Exp(-0.5*arg*arg); //gaussian part
  }

  return fitval;
}

//______________________________________________________________________________
Double_t PairAnalysisFunction::PeakFunGauss(const Double_t *x, const Double_t *par) {
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
void PairAnalysisFunction::SetDefault(EFunction predefinedFunc)
{
  ///
  /// use a predefined function, that internally sets the "fFuncSigBack"
  ///
  switch(predefinedFunc)
    {
    case kBoltzmann: GetBoltzmann(); break;
    case kPtExp:     GetPtExp();     break;
    case kHagedorn:  GetHagedorn();  break;
    case kLevi:      GetLevi();      break;
    default: Error("SetDefault","predefined function not yet implemented");
    }

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

  if (!peak) {
    Error("CombineFunc","signal function need to be set!");
    return;
  }
  fFuncSignal=peak;
  fFuncBackground=bgnd;

  fNparPeak     = fFuncSignal->GetNpar();
  fNparBgnd     = (bgnd?fFuncBackground->GetNpar():0);

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
//void PairAnalysisFunction::Print(Option_t */*option*/) const
//{
  //
  // Print the statistics
  //
//  printf("Fit int.  :  %.5g - %.5g \n",fFitMin,fFitMax);
//  printf("Fit chi/%d:  %.5g \n",fDof,fChi2Dof);
//}


//______________________________________________
TF1 * PairAnalysisFunction::GetBoltzmann(){
  // Boltzmann (exp in 1/mt*dNdmT times mt) as a function of dNdpt
  fFuncSigBack=new TF1("Boltzmann","[1]*x*sqrt(x*x+[0]*[0])*exp(-sqrt(x*x+[0]*[0])/[2])",0.,10.);
  //    fFuncSigBack->SetParameters(fPOI->Mass(), norm, temp);
  if(fPOI) fFuncSigBack->FixParameter(0,fPOI->Mass());
  fFuncSigBack->SetParLimits(2, 0.01, 10);
  fFuncSigBack->SetParNames("mass","norm", "T");
  return   fFuncSigBack;
}

//______________________________________________
TF1 * PairAnalysisFunction::GetPtExp(){
  // Simple exponential in 1/pt*dNdpT, as a function of dNdpt
  fFuncSigBack=new TF1("Exponential","[0]*x*exp(-x/[1])",0.,10.);
  //  fFuncSigBack->SetParameters(norm, temp);
  fFuncSigBack->SetParLimits(1, 0.01, 10);
  fFuncSigBack->SetParNames("norm", "T");
  return fFuncSigBack;
}

//______________________________________________
TF1 * PairAnalysisFunction::GetHagedorn(){
  // PowerLaw function, dNdpt
  // power law Nuclear Physics B, Vol. 335, No. 2. (7 May 1990), pp. 261-287.
  // This is sometimes also called Hagedorn or modified Hagedorn
  fFuncSigBack=new TF1("Hagedorn","x*[0]*( 1 + x/[1] )^(-[2])",0.,10.);
  //  fFuncSigBack->SetParameters(norm, pt0, n);
  fFuncSigBack->SetParLimits(1, 0.01, 10);
  //fFuncSigBack->SetParLimits(2, 0.01, 50);
  fFuncSigBack->SetParNames("norm", "pt0", "n");
  return fFuncSigBack;
}

//______________________________________________
TF1 * PairAnalysisFunction::GetLevi(){
  // Levi function (aka Tsallis), dNdpt
  fFuncSigBack=new TF1("Levi-Tsallis","( x*[0]*([1]-1)*([1]-2)  )/( [1]*[2]*( [1]*[2]+[3]*([1]-2) )  ) * ( 1 + (sqrt([3]*[3]+x*x) -[3])/([1]*[2])  )^(-[1])",0.,10.);
  //  fFuncSigBack->SetParameters(norm, n, temp,mass);
  if(fPOI) fFuncSigBack->FixParameter(3,fPOI->Mass());
  fFuncSigBack->SetParLimits(2, 0.01, 10);
  fFuncSigBack->SetParNames("norm (dN/dy)", "n", "T", "mass");
  return fFuncSigBack;
}
