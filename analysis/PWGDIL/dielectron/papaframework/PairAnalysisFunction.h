#ifndef PAIRANALYSISFUNCTION_H
#define PAIRANALYSISFUNCTION_H

//#############################################################
//#                                                           #
//#         Class PairAnalysisFunction                        #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TNamed.h>
#include <TString.h>
#include <TVectorT.h>
#include <TMath.h>
#include <TH1F.h>
#include <TF1.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>

class PairAnalysisFunction : public TNamed {

 public:

  enum EFunction { kBoltzmann=0, kPtExp, kHagedorn, kLevi };

  PairAnalysisFunction();
  PairAnalysisFunction(const char*name, const char* title);
  PairAnalysisFunction(const PairAnalysisFunction &c);
  /* PairAnalysisFunction &operator=(const PairAnalysisFunction &c); */

  virtual ~PairAnalysisFunction();

  //  virtual void Fit(Option_t *opt);

  // Setter
  void SetParticleOfInterest(Int_t pdgcode)      { fPOIpdg=pdgcode;  fPOI=TDatabasePDG::Instance()->GetParticle(fPOIpdg); }
  void SetFitRange(Double_t min, Double_t max)   { fFitMin=min; fFitMax=max;  }
  void SetUseIntegral(Bool_t flag=kTRUE)         { fUseIntegral = flag; }
  void SetFitOption(const char* opt) {
    fFitOpt=opt;    fFitOpt.ToLower();
    if(!fFitOpt.Contains("s")) fFitOpt += "s";
  }

  void SetMCSignalShape(TH1F* hist) { fgHistSimPM=hist; }
  void SetFunctions(TF1 * const combined, TF1 * const sig=0, TF1 * const back=0, Int_t parM=1, Int_t parMres=2);
  void SetDefault(EFunction predefinedFunc);
  void SetDefaults(Int_t type);

  void CombineFunc(TF1 * const peak=0, TF1 * const bgnd=0);

  // predefined peak functions
  Double_t PeakFunMC(const Double_t *x, const Double_t *par); // peak function from a mc histo
  Double_t PeakFunCB(const Double_t *x, const Double_t *par); // crystal ball function
  Double_t PeakFunGauss(const Double_t *x, const Double_t *par);       // gaussian
  Double_t PeakFunPowGaussPow(const Double_t *x, const Double_t *par); // pow gaussian pow
  Double_t PeakFunExpGaussExp(const Double_t *x, const Double_t *par); // exp gaussian exp

  // TODO: predefined other functions (tsallis, boltzman, levi, powerlaw)
  TF1*  GetBoltzmann();
  TF1*  GetPtExp();
  TF1*  GetHagedorn();
  TF1*  GetLevi();

  // Getter
  TF1*  GetSignalFunction()     const { return fFuncSignal;        }
  TF1*  GetBackgroundFunction() const { return fFuncBackground;    }
  TF1*  GetCombinedFunction()   const { return fFuncSigBack;       }

  Int_t GetParticleOfInterest() const { return fPOIpdg;            }
  Int_t GetDof()                const { return fDof;               }
  Double_t GetChi2Dof()         const { return fChi2Dof;           }
  Double_t GetFitMin()          const { return fFitMin;            }
  Double_t GetFitMax()          const { return fFitMax;            }
  
  //virtual void Draw(const Option_t* option = "");
  //  virtual void Print(Option_t *option="") const;


protected:

  static TH1F* fgHistSimPM;          // simulated peak shape

  Double_t PeakBgndFun(const Double_t *x, const Double_t *par); // combine any bgrd and any peak function

  TF1 *fFuncSignal        = NULL;    // Function for the signal description
  TF1 *fFuncBackground    = NULL;    // Function for the background description
  TF1 *fFuncSigBack       = NULL;    // Combined function signal plus background

  Double_t fFitMin        = 0.;      // fit range lowest inv. mass
  Double_t fFitMax        = 0.;      // fit range highest inv. mass

  TParticlePDG *fPOI      = NULL;    // MC particle of interest
  Int_t  fPOIpdg          = 443;     // pdg code particle of interest
  Int_t fParMass          = 1;       // the index of the parameter corresponding to the resonance mass
  Int_t fParMassWidth     = 2;       // the index of the parameter corresponding to the resonance mass width

  TString fFitOpt         = "SMNQE"; // fit option used
  Bool_t fUseIntegral     = kFALSE;  // use the integral of the fitted functions to extract signal and background

  Int_t    fDof           = 0;       // degrees of freedom
  Double_t fChi2Dof       = 0.;      // chi2/dof of the fitted inv mass spectra

  Int_t    fNparPeak      = 0;       // number of parameters for peak function
  Int_t    fNparBgnd      = 0;       // number of parameters for background function




  //  PairAnalysisFunction(const PairAnalysisFunction &c);
  PairAnalysisFunction &operator=(const PairAnalysisFunction &c);

  ClassDef(PairAnalysisFunction,1)         // Combine functions for e.g. signal extractions

};

#endif
