#ifndef PAIRANALYSISHELPER_H
#define PAIRANALYSISHELPER_H
/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
// PairAnalysis helpers                                                                    //
//                                                                                       //
//                                                                                       //
// Authors:                                                                              //
//   Julian Book <Julian.Book@cern.ch>                                                   //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////


#include <TVectorDfwd.h>
#include <TArrayD.h>

#include "CbmDefs.h"

//class AliKFParticle;
/* class AliVEvent; */
/* class AliMCEvent; */
///class PairAnalysisEventNEW;
class TFormula;
class TH1;
class TH2;
class TObjArray;

namespace PairAnalysisHelper
{

  TVectorD* MakeLogBinning(Int_t nbinsX, Double_t xmin, Double_t xmax);
  TVectorD* MakeLinBinning(Int_t nbinsX, Double_t xmin, Double_t xmax);
  TVectorD* MakeArbitraryBinning(const char* bins);
  TVectorD* MakeGausBinning(Int_t nbinsX, Double_t mean, Double_t sigma);
  TVectorD* CombineBinning(TVectorD *low, TVectorD *high);

  TArrayD* MakeStatBinLimits(TH1* h, Double_t stat);

  TVectorD* MakePdgBinning();

  //  void RotateKFParticle(AliKFParticle * kfParticle,Double_t angle/*, const PairAnalysisEventNEW * const ev=0x0*/);

  // tformula functions
  Double_t EvalFormula(    TFormula *form, const Double_t *values);
  TString  GetFormulaTitle(TFormula *form);
  TString  GetFormulaName( TFormula *form);
  TFormula *GetFormula(const char *name, const char* formula);

  // pdg labels
  void SetPDGBinLabels( TH1 *hist, Bool_t clean);
  TString GetPDGlabel(Int_t pdg);
  void SetGEANTBinLabels( TH1 *hist);

  // misc
  TString GetDetName(ECbmModuleId det);
  Double_t GetContentMinimum(TH1 *h, Bool_t inclErr=kTRUE);
  Double_t GetContentMaximum(TH1 *h, Bool_t inclErr=kTRUE);
  Double_t GetQuantile(TH1* h1, Double_t p=0.5);

  void     NormalizeSlicesY(TH2* h);
  void     CumulateSlicesX(TH2* h, Bool_t reverse=kFALSE, Bool_t norm=kFALSE);
  void     Cumulate(TH1* h, Bool_t reverse=kFALSE, Bool_t norm=kFALSE);

  TObject* FindObjectByTitle(TObjArray *arrhist, TString ref);

  Int_t GetPrecision(Double_t value);

} // Helper class (static)

#endif
