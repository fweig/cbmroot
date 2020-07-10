#ifndef PAIRANALYSISSPECTRUM_H
#define PAIRANALYSISSPECTRUM_H
//#############################################################
//#                                                           #
//#         Class PairAnalysisSpectrum                        #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################


//#include <TTree.h>
#include <TGraphErrors.h>
#include <TObjArray.h>
#include <TString.h>
#include <TVectorT.h>

#include "PairAnalysisHF.h"
#include "PairAnalysisHistos.h"

#include "PairAnalysisFunction.h"
#include "PairAnalysisSignalExt.h"

#include "PairAnalysis.h"

class TList;
class TObjArray;
class TFormula;
class TTree;
class TH1F;

class Extraction : public TObject {
public:
  TString setup                 = "";    // indentifier key
  Int_t setupId                 = -1;    // identifier idx
  Int_t poi                     = 0;     // particle of interest
  Double_t var                  = 0.;    // value of variable
  Double_t varE                 = 0.;    // error of variable
  Double_t s                    = 0.;    // raw signal
  Double_t sE                   = 0.;    // raw signal error
  Double_t b                    = 0.;    // background
  Double_t bE                   = 0.;    // background error
  Double_t sb                   = 0.;    // SB
  Double_t sbE                  = 0.;    // SBError
  Double_t sgn                  = 0.;    // Significance
  Double_t sgnE                 = 0.;    // SignificanceError
  TH1F* HistSignal              = NULL;  // SignalHistogram
  Double_t eff                  = 0.;    // efficiency
  Double_t effE                 = 0.;    // efficiency error
  PairAnalysisSignalExt* signal = NULL;  // Signal extraction
  Double_t sref                 = 0.;    // mc truth signal
  Double_t srefE                = 0.;    // mc truth signal error
  ClassDef(
    Extraction,
    1)  // mini object that holds members of the PairAnalysisSpectrum TTree
};
ClassImp(Extraction)

  class PairAnalysisSpectrum : public PairAnalysisFunction {

public:
  enum ESystMethod { kBarlow = 0, kSystMax, kSystRMS };

  PairAnalysisSpectrum();
  PairAnalysisSpectrum(const char* name, const char* title);

  virtual ~PairAnalysisSpectrum();

  // General Setter

  void SetVariable(TString varType, TVectorD* const binLimits) {
    fVar        = varType;
    fVarBinning = binLimits;
  }
  void SetSystMethod(ESystMethod mthd) { fSystMthd = mthd; }

  // Input
  void AddInput(TObjArray* raw,
                TString identifier,
                TObjArray* mc    = NULL,
                TObjArray* truth = NULL);
  void AddMCInput(PairAnalysisHistos* hf) { fMCInput.Add(hf); }
  void AddExtractor(PairAnalysisSignalExt* sig) {
    fExtractor.Add((PairAnalysisSignalExt*) sig->Clone());
  }

  // Spectrum
  virtual void DrawSpectrum(const char* varexp,
                            const char* selection = "",
                            Option_t* option      = "");
  Int_t Write(const char*, Int_t, Int_t) { return -1; }
  Int_t Write(const char*, Int_t, Int_t) const { return -1; }

  void Fit(TString drawoption = "L");

  // Processing
  void Init();
  void Process();

  // output
  virtual void Write();

private:
  // settings
  Int_t fIdx            = 0;     // index
  TString fVar          = "";    // variable looked at
  TVectorD* fVarBinning = NULL;  // variable binning

  // calculation
  ESystMethod fSystMthd =
    kSystMax;  // method for systematic uncertainty calculation

  // input
  TString fInputKeys[100];  // keys to identify the extraction
  TList fRawInput;  // list of input objects for signals (HF, Ntuple, THnSparse)
  TList fMCInput;   // list of input objects for mc (HF, Ntuple, THnSparse)
  TList fMCTruth;   // list of input objects for mc truth
  TList
    fExtractor;  // list of input objects for signal extraction objects (Ext,Func)

  // output
  TTree* fTree            = NULL;  // tree output
  TList* fResults         = NULL;  // final list of inv. mass spectra
  TObjArray* fExtractions = NULL;  // final canvases
  Extraction* fExt        = NULL;  // extraction

  TGraphErrors* fSignal = NULL;  // signal graph

  PairAnalysisSpectrum(const PairAnalysisSpectrum& c);
  PairAnalysisSpectrum& operator=(const PairAnalysisSpectrum& c);

  ClassDef(PairAnalysisSpectrum,
           1)  // Build spectra from many signal extractions
};

#endif
