#ifndef PAIRANALYSISFUNCTION_H
#define PAIRANALYSISFUNCTION_H

/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. */

//#############################################################
//#                                                           #
//#         Class PairAnalysisFunction                      #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TVectorT.h>
#include <TString.h>
#include <TH1F.h>

class PairAnalysisFunction : public TNamed {
public:
  PairAnalysisFunction();
  PairAnalysisFunction(const char*name, const char* title);
  PairAnalysisFunction(const PairAnalysisFunction &c);
  PairAnalysisFunction &operator=(const PairAnalysisFunction &c);

  virtual ~PairAnalysisFunction();

  //  virtual void Fit(Option_t *opt);

  // Setter
  void SetUseIntegral(Bool_t flag=kTRUE) {fUseIntegral = flag;};
  void SetFitOption(const char* opt) {
    fFitOpt=opt;    fFitOpt.ToLower();
    if(!fFitOpt.Contains("s")) fFitOpt += "s";
  }

  void SetFunction(TF1 * const combined, TF1 * const sig=0, TF1 * const back=0, Int_t parM=1, Int_t parMres=2);
  void SetDefaults(Int_t type);

  void CombineFunc(TF1 * const peak=0, TF1 * const bgnd=0);

  // predefined peak functions
  Double_t PeakFunMC(const Double_t *x, const Double_t *par); // peak function from a mc histo
  Double_t PeakFunCB(const Double_t *x, const Double_t *par); // crystal ball function
  Double_t PeakFunGaus(const Double_t *x, const Double_t *par); // gaussian

  // Getter
  TF1*  GetSignalFunction()     const { return fFuncSignal;        }
  TF1*  GetBackgroundFunction() const { return fFuncBackground;    }
  TF1*  GetCombinedFunction()   const { return fFuncSigBack;       }

  Int_t GetDof()                const { return fDof;               }
  Double_t GetChi2Dof()         const { return fChi2Dof;           }

  virtual void Draw(const Option_t* option = "");
  virtual void Print(Option_t *option="") const;


protected:

  Double_t PeakBgndFun(const Double_t *x, const Double_t *par); // combine any bgrd and any peak function

  TF1 *fFuncSignal        = NULL;    // Function for the signal description
  TF1 *fFuncBackground    = NULL;    // Function for the background description
  TF1 *fFuncSigBack       = NULL;    // Combined function signal plus background

  Int_t fParMass          = 1;       // the index of the parameter corresponding to the resonance mass
  Int_t fParMassWidth     = 2;       // the index of the parameter corresponding to the resonance mass width

  TString fFitOpt         = "SMNQE"; // fit option used
  Bool_t fUseIntegral     = kFALSE;  // use the integral of the fitted functions to extract signal and background

  Int_t    fDof           = 0;       // degrees of freedom
  Double_t fChi2Dof       = 0;       // chi2/dof of the fitted inv mass spectra

  Int_t    fNparPeak      = 0;       // number of parameters for peak function
  Int_t    fNparBgnd      = 0;       // number of parameters for background function

  ClassDef(PairAnalysisFunction,1)         // Signal extraction using a combined bgrd+signal fit
};

#endif
