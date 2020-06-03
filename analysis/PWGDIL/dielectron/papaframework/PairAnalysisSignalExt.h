#ifndef PAIRANALYSISSIGNALEXT_H
#define PAIRANALYSISSIGNALEXT_H

//#############################################################
//#                                                           # 
//#         Class PairAnalysisSignalExt                       #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################


#include <TNamed.h>
#include <TVectorD.h>
#include <TMath.h>
#include <TH1F.h>
#include <TF1.h>

#include "PairAnalysis.h"
#include "PairAnalysisFunction.h"

class TObjArray;
class TPaveText;

class PairAnalysisSignalExt : public PairAnalysisFunction {

public:
  enum EBackgroundMethod {
    kFittedMC = 0,
    kFitted,
    kLikeSign,
    kLikeSignArithm,
    kLikeSignRcorr,
    kLikeSignArithmRcorr,
    kLikeSignFit,
    kEventMixing,
    kEventMixingFit,
    kRotation,
    kCocktail
  };

  enum ESignalExtractionMethod {
    kBinCounting = 0,
    kMCScaledMax,
    kMCScaledInt,
    kMCFitted,
    kCrystalBall,
    kGaus,
    kUserFunc
  };

  enum EScalingMethod {
    kSclToRaw = 0,
    kSclToLikeSign
  };

  PairAnalysisSignalExt();
  PairAnalysisSignalExt(const char*name, const char* title);
  
  ~PairAnalysisSignalExt();

  //// Setter ////
  // signal

  void SetMCSignalShape(TH1F* hist)                        { fgHistSimPM=hist; fHistSignalMC=hist; }
  void SetIntegralRange(Double_t min, Double_t max)        { fIntMin=min; fIntMax=max; }
  void SetPlotRange(Double_t min, Double_t max)            { fPlotMin=min; fPlotMax=max; }
  void SetRebin(Int_t factor)                              { fRebin=factor; }
  void SetStatRebin(Double_t stat)                         { fRebinStat=stat; }
  void SetRebin(TArrayD *limits)                           { fBinLimits=limits; }
  void SetRebin(TVectorD *limits)                          { fBinLimits=new TArrayD(limits->GetNrows()-1,limits->GetMatrixArray()); }
  void SetExtractionMethod(ESignalExtractionMethod method, PairAnalysisFunction *sigF=0x0) {
    fPeakMethod=method; fExtrFunc=sigF;}
  void SetMixingCorrection(Bool_t mixcorr=kTRUE)           { fMixingCorr=mixcorr; }

  // background

  void SetMethod(EBackgroundMethod method)                 { fMethod = method;}
  void SetNTrackRotations(Int_t iterations)                { fNiterTR =iterations; }
  void SetScaleBackgroundTo(EScalingMethod method, Double_t intMin, Double_t intMax, Double_t intMin2=0., Double_t intMax2=0.) { fSclMethod=method; fScaleMin=intMin; fScaleMax=intMax; fScaleMin2=intMin2; fScaleMax2=intMax2;}
  void SetCocktailContribution(TObjArray *arr, Bool_t subtract=kTRUE)       { fArrCocktail=arr; fCocktailSubtr=subtract; }

  // Getter

  Bool_t IsCocktailSubtracted()      const { return fCocktailSubtr; }
  Double_t GetIntegralMin()          const { return fIntMin; }
  Double_t GetIntegralMax()          const { return fIntMax; }
  Int_t GetRebin()                   const { return fRebin;  }
  TArrayD *GetRebinLimits()          const { return fBinLimits; }
  ESignalExtractionMethod GetExtractionMethod() const      { return fPeakMethod; }
  EBackgroundMethod GetMethod()      const { return fMethod; }
  Double_t GetScaleMin()             const { return fScaleMin;   }
  Double_t GetScaleMax()             const { return fScaleMax;   }
  Double_t GetScaleMin2()            const { return fScaleMin2;  }
  Double_t GetScaleMax2()            const { return fScaleMax2;  }

  // values of results

  Double_t GetScaleFactor()          const { return fScaleFactor; }
  const TVectorD& GetValues()        const { return fValues; }
  const TVectorD& GetErrors()        const { return fErrors; }
  Double_t GetSignal()               const { return fValues(0); }
  Double_t GetSignalError()          const { return fErrors(0); }
  Double_t GetBackground()           const { return fValues(1); }
  Double_t GetBackgroundError()      const { return fErrors(1); }
  Double_t GetSignificance()         const { return fValues(2); }
  Double_t GetSignificanceError()    const { return fErrors(2); }
  Double_t GetSB()                   const { return fValues(3); }
  Double_t GetSBError()              const { return fErrors(3); }
  Double_t GetMass()                 const { return fValues(4); }
  Double_t GetMassError()            const { return fErrors(4); }
  Double_t GetMassWidth()            const { return fValues(5); }
  Double_t GetMassWidthError()       const { return fErrors(5); }
  Double_t GetMatchChi2NDF()         const { return fValues(6); }
  Double_t GetMatchChi2NDFError()    const { return fErrors(6); }
  static const char* GetValueName(Int_t i) { return (i>=0&&i<7)?fgkValueNames[i]:""; }

  // objects

  TH1* GetMCSignalShape()            const { return fHistSignalMC; }
  TH1* GetSignalHistogram()          const { return fHistSignal; }
  TH1* GetSoverBHistogram()          const { return fHistSB; }
  TH1* GetSignificanceHistogram()    const { return fHistSgn; }
  TH1* GetBackgroundHistogram()      const { return fHistBackground; }
  TH1* GetUnlikeSignHistogram()      const { return fHistDataPM; }
  TH1* GetCocktailHistogram()        const { return fHistCocktail; }
  TH1* GetRfactorHistogram()         const { return fHistRfactor; }
  TObject* GetPeakShape()            const { return fgPeakShape; }


  TObject* DescribePeakShape(ESignalExtractionMethod method=kMCFitted, Bool_t replaceValErr=kFALSE,  TH1F *mcShape=0x0);
  TPaveText* DrawStats(Double_t x1=0., Double_t y1=0., Double_t x2=0., Double_t y2=0., TString opt="pRnbsSmrc");
  Double_t ScaleHistograms(TH1* histRaw, TH1* histBackground, Double_t intMin, Double_t intMax);
  Double_t ScaleHistograms(TH1* histRaw, TH1* histBackground, Double_t intMin, Double_t intMax, Double_t intMin2, Double_t intMax2);

  static TH1* MergeObjects(TH1* obj1, TH1* obj2, Double_t val=+1.);
  void Print(Option_t *option="") const;

  /**
  This function needs to be implemented by the signal extraction classes.
  Here all the work should be done.

  The signal extraction is done on the mass spectra.
  The TObjArray should contain the Inv. Mass spectra of the 8 possible combinations
  for single and mixed events defined in PairAnalysis.cxx
  */
  //virtual void Process(TObjArray * const /*arrhist*/) = 0;
  void Process(TObjArray* const arrhist);
  void ProcessLS();        // like-sign method
  void ProcessEM();        // event mixing method
  void ProcessTR();        // track rotation method
  void ProcessCocktail();  // cocktail method

  void Draw(const Option_t* option = "");

  TObject* FindObject(TObjArray *arrhist, PairAnalysis::EPairType type) const;
  TObject* FindObjectByTitle(TObjArray *arrhist, TString ref);

  // implemented to remove warnings
  TObject* FindObject(const char *) const {TObject* bla = new TObject(); return bla;}
  TObject* FindObject(const TObject *) const {TObject* bla = new TObject(); return bla;}

protected:
  TObjArray *fArrHists    = NULL; // array of input histograms
  TObjArray *fArrCocktail = NULL; // array of cocktail histograms
  TH1 *fHistSignal        = NULL; // histogram of pure signal
  TH1 *fHistSB            = NULL; // histogram of signal to bgrd
  TH1 *fHistSgn           = NULL; // histogram of significance
  TH1 *fHistBackground    = NULL; // histogram of background (fitted=0, like-sign=1, event mixing=2)
  TH1 *fHistCocktail      = NULL; // histogram of cocktail
  TH1 *fHistDataPM        = NULL; // histogram of selected +- pair candidates
  TH1 *fHistDataPP        = NULL; // histogram of selected ++ pair candidates
  TH1 *fHistDataMM        = NULL; // histogram of selected -- pair candidates
  TH1 *fHistDataME        = NULL; // histogram of selected +- pair candidates from mixed event
  TH1 *fHistRfactor       = NULL; // histogram of R factors
  TH1 *fHistSignalMC      = NULL; // histogram of signal MC shape

  TH1 *fHistMixPM         = NULL; // histogram of selected +- pair candidates
  TH1 *fHistMixPP         = NULL; // histogram of selected ++ pair candidates
  TH1 *fHistMixMM         = NULL; // histogram of selected -- pair candidates
  TH1 *fHistMixMP         = NULL; // histogram of selected +- pair candidates
  TH1 *fHistDataTR        = NULL; // histogram of selected +- pair candidates

  TVectorD fValues;               // values
  TVectorD fErrors;               // value errors

  Double_t fIntMin        = 0.;   // signal extraction range min
  Double_t fIntMax        = 0.;   // signal extraction range max
  Double_t fPlotMin       = 0.;   // plot range lowest inv. mass
  Double_t fPlotMax       = 0.;   // plot range highest inv. mass

  Int_t fRebin            = 1;    // histogram rebin factor
  Double_t fRebinStat     = 1.;   // rebin until bins have max. stat. error
  TArrayD *fBinLimits     = NULL; // bin limits from stat. rebinning

  void ScaleBackground();
  EBackgroundMethod fMethod = kLikeSign; // method for background substraction
  EScalingMethod fSclMethod = kSclToRaw; // method for background normalization
  Double_t fScaleMin      = 0.;   // min for scaling
  Double_t fScaleMax      = 0.;   // max for scaling
  Double_t fScaleMin2     = 0.;   // min2 for scaling
  Double_t fScaleMax2     = 0.;   // max2 for scaling
  Int_t    fNiterTR       = 1;    // track rotation scale factor according to number of rotations
  Double_t fScaleFactor   = 1.;   // scale factor of histogram scaling
  Bool_t fMixingCorr      = kFALSE; // switch for bin by bin correction with R factor
  Bool_t fCocktailSubtr   = kFALSE; // switch for cocktail subtraction

  PairAnalysisFunction *fExtrFunc     = NULL; // signal extraction function
  ESignalExtractionMethod fPeakMethod = kBinCounting; // method for peak description and signal extraction
  static TObject *fgPeakShape;      // histogram or function used to describe the extracted signal
  Bool_t fPeakIsTF1       = kFALSE; // flag

  Bool_t fProcessed       = kFALSE; // flag
  static TH1F* fgHistSimPM;         // simulated peak shape

  void FillSignificance(TH1* hfill, TObject* signal, TH1* hbgrd);
  void SetSignificanceAndSOB();      // calculate the significance and S/B
  void SetFWHM();                    // calculate the fwhm

  static const char* fgkValueNames[7];             //value names
  static const char* fgkBackgroundMethodNames[11]; // background estimator names

  PairAnalysisSignalExt(const PairAnalysisSignalExt &c);
  PairAnalysisSignalExt &operator=(const PairAnalysisSignalExt &c);

  ClassDef(PairAnalysisSignalExt,3) // Class for signal extraction
};

inline TObject* PairAnalysisSignalExt::FindObject(TObjArray *arrhist, PairAnalysis::EPairType type) const
{
  //
  // shortcut to find a certain pair type object in array
  //

  //  return ( arrhist->FindObject(Form("Pair.%s",PairAnalysis::PairClassName(type))) );
  TString ref=Form("Pair.%s",PairAnalysis::PairClassName(type));
  for(Int_t i=0; i<arrhist->GetEntriesFast(); i++) {
    if( !ref.CompareTo(arrhist->UncheckedAt(i)->GetTitle()) )
      return arrhist->UncheckedAt(i);
  }
  return 0x0;
}

inline TObject* PairAnalysisSignalExt::FindObjectByTitle(TObjArray *arrhist, TString ref)
{
  //
  // shortcut to find a certain pair type object in array
  //
  if(!arrhist) return 0x0;
  //  return ( arrhist->FindObject(Form("Pair.%s",PairAnalysis::PairClassName(type))) );
  //  TString ref=Form("Pair.%s",PairAnalysis::PairClassName(type));
  for(Int_t i=0; i<arrhist->GetEntriesFast(); i++) {
    if( !ref.CompareTo(arrhist->UncheckedAt(i)->GetTitle()) ) {
      return arrhist->UncheckedAt(i);
    }
  }
  return 0x0;
}

inline void PairAnalysisSignalExt::SetSignificanceAndSOB()
{
  //
  // Calculate S/B and significance
  //

  // Signal/Background
  fValues(3) = (fValues(1)>0 ? fValues(0)/fValues(1) : 0);
  Float_t epsSig = (fValues(0)>0 ? fErrors(0)/fValues(0) : 0);
  Float_t epsBknd = (fValues(1)>0 ? fErrors(1)/fValues(1) : 0);
  fErrors(3) = fValues(3)*TMath::Sqrt(epsSig*epsSig + epsBknd*epsBknd);
  // Significance
  fValues(2) = ((fValues(0)+fValues(1))>0 ? fValues(0)/TMath::Sqrt(fValues(0)+fValues(1)) : 0);
  Float_t s = (fValues(0)>0?fValues(0):0); Float_t b = fValues(1);
  Float_t se = fErrors(0); Float_t be = fErrors(1);
  // fErrors(2) = ((s+b)>0 ? TMath::Sqrt((s*(s+2*b)*(s+2*b)+b*s*s)/(4*TMath::Power(s+b,3))) : 0); // old implementation
  fErrors(2) = ((s+b)>0 ? fValues(2)*TMath::Sqrt(be*be + TMath::Power(se*(s+2*b)/s, 2)) / 2 / (s+b) : 0);
}

inline void PairAnalysisSignalExt::SetFWHM()
{
  //
  // calculate the full width at half maximum (fwhm)
  //

  if(!fgPeakShape) return;

  // case for TF1
  if(fgPeakShape->IsA() == TF1::Class()) {
    TF1* fit  = (TF1*) fgPeakShape->Clone("fit");
    TF1* pfit = (TF1*) fit->Clone("pfit");
    TF1* mfit = (TF1*) fit->Clone("mfit");
    for (Int_t i=0; i<fit->GetNpar(); i++) {
      pfit->SetParameter(i,fit->GetParameter(i) + fit->GetParError(i));
      mfit->SetParameter(i,fit->GetParameter(i) - fit->GetParError(i));
    }
    Double_t maxX   = fit->GetMaximumX();
    Double_t maxY   = fit->GetHistogram()->GetMaximum();
    Double_t xAxMin = fit->GetXmin();
    Double_t xAxMax = fit->GetXmax();
    // fwhms 
    Double_t fwhmMin  = fit->GetX(.5*maxY, xAxMin, maxX);
    Double_t fwhmMax  = fit->GetX(.5*maxY, maxX, xAxMax);
    Double_t pfwhmMin = pfit->GetX(.5*maxY, xAxMin, maxX);
    Double_t pfwhmMax = pfit->GetX(.5*maxY, maxX, xAxMax);
    Double_t mfwhmMin = mfit->GetX(.5*maxY, xAxMin, maxX);
    Double_t mfwhmMax = mfit->GetX(.5*maxY, maxX, xAxMax);
    Double_t pError = TMath::Abs( (fwhmMax-fwhmMin) - (pfwhmMax-pfwhmMin) );
    Double_t mError = TMath::Abs( (fwhmMax-fwhmMin) - (mfwhmMax-mfwhmMin) );
    fValues(5) = (fwhmMax-fwhmMin);
    fErrors(5) = (pError>=mError ? pError : mError);
    delete fit;
    delete pfit;
    delete mfit;
  }
  else if(fgPeakShape->IsA() == TH1F::Class()) {
    // th1 calculation
    TH1F *hist = (TH1F*) fgPeakShape->Clone("hist");
    Int_t bin1 = hist->FindFirstBinAbove(hist->GetMaximum()/2);
    Int_t bin2 = hist->FindLastBinAbove(hist->GetMaximum()/2);
    fValues(5) = hist->GetBinCenter(bin2) - hist->GetBinCenter(bin1);
    fErrors(5) = 0.0; // not defined
    delete hist;
  }

}

inline void PairAnalysisSignalExt::FillSignificance(TH1* hfill, TObject* signal, TH1* hbgrd) {
  ///
  /// fill significance histogram
  ///
  /// TODO: signal error for TF1


  /* hfill->Reset("CEIS"); */
  hfill->SetYTitle( GetValueName(2) );

  Double_t s=0.;  Double_t b=0.;
  Double_t se=0.; Double_t be=0.;
  for(Int_t i=1; i<=hfill->GetNbinsX(); i++) {

    if(signal->IsA()==TF1::Class()) s=static_cast<TF1*>(signal)->Eval(hfill->GetBinCenter(i));
    else                            {
      s   =static_cast<TH1*>(signal)->GetBinContent(i);
      se  =static_cast<TH1*>(signal)->GetBinError(i);
    }

    b=hbgrd->GetBinContent(i);
    be=hbgrd->GetBinError(i);

    Double_t sgn = ((s+b)>0. ? s/TMath::Sqrt(s+b) : 0.);
    //    printf("s %.3e b %.3e \t s/b: %.3e sgn %.3e \n",s,b,s/b,sgn);
    hfill->SetBinContent(i, sgn );
    hfill->SetBinError(  i,((s+b)>0. ? sgn*TMath::Sqrt(be*be + TMath::Power(se*(s+2*b)/s, 2)) / 2 / (s+b) : 0) );
  }

}


#endif
