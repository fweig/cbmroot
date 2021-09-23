/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Pierre-Alain Loizeau, Andreas Redelbach */

#ifndef CBMCHECKTIMING_H
#define CBMCHECKTIMING_H

#include "CbmModuleList.h"
#include "CbmTofDigi.h"

#include "FairTask.h"

#include "TString.h"

#include <vector>

class TClonesArray;
class TH1;
class TH2;
class CbmDigiManager;

class CbmCheckTiming : public FairTask {
public:
  CbmCheckTiming();

  CbmCheckTiming(const CbmCheckTiming&) = delete;
  CbmCheckTiming operator=(const CbmCheckTiming&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmCheckTiming(Int_t verbose);


  /** Destructor **/
  ~CbmCheckTiming();


  /** Initiliazation of task at the beginning of a run **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes **/
  virtual InitStatus ReInit();


  /** Executed for each event. **/
  virtual void Exec(Option_t*);

  /** Load the parameter container from the runtime database **/
  virtual void SetParContainers();

  /** Finish task called at the end of the run **/
  virtual void Finish();

  /** Switch OFF check for correct time order of the digis **/
  void SetCheckTimeOrder(Bool_t val = kFALSE) { fCheckTimeOrdering = val; }

  /** Switch ON check for inter system offsets **/
  void SetCheckInterSystemOffset(Bool_t val = kTRUE) { fCheckInterSystemOffset = val; }

  void SetOffsetSearchRange(Int_t val = 1000) { fOffsetRange = val; }

  void SetStsOffsetSearchRange(Int_t val = 1000) { fStsOffsetRange = val; }

  void SetMuchOffsetSearchRange(Int_t val = 1000) { fMuchOffsetRange = val; }

  void SetTrdOffsetSearchRange(Int_t val = 1000) { fTrdOffsetRange = val; }

  void SetTofOffsetSearchRange(Int_t val = 1000) { fTofOffsetRange = val; }

  void SetRichOffsetSearchRange(Int_t val = 1000) { fRichOffsetRange = val; }

  void SetPsdOffsetSearchRange(Int_t val = 1000) { fPsdOffsetRange = val; }

  void SetNrTsForFit(Int_t val = 200) { fNrTsForFit = val; }
  void SetTrdPeakWidthNs(Double_t val = 120.) { fTrdPeakWidthNs = val; }
  void SetStsPeakWidthNs(Double_t val = 30.) { fStsPeakWidthNs = val; }
  void SetMuchPeakWidthNs(Double_t val = 100.) { fMuchPeakWidthNs = val; }
  void SetTofPeakWidthNs(Double_t val = 20.) { fTofPeakWidthNs = val; }
  void SetRichPeakWidthNs(Double_t val = 40.) { fRichPeakWidthNs = val; }
  void SetPsdPeakWidthNs(Double_t val = 20.) { fPsdPeakWidthNs = val; }

  inline void SetT0PulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulserT0 = uMin;
    fuMaxTotPulserT0 = uMax;
  }
  inline void SetTofPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulserTof = uMin;
    fuMaxTotPulserTof = uMax;
  }

  inline void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }

private:
  void CheckTimeOrder();
  template<class Digi>
  Int_t CheckIfSorted(TH1*, Double_t&, TString);
  Int_t CheckIfSortedT0(TH1*, Double_t&, TString);

  void CheckInterSystemOffset();

  /*
    Int_t FillSystemOffsetHistos(TClonesArray* array,
                                 TH1* histo, TH2* histoCharge,
                                 TH2* histoEvo, TH2* histoEvoLong, TH2* histoAFCK,
                                 const Double_t T0Time, const Int_t offsetRange, Int_t iStartDigi,
                                 Bool_t bSts = kFALSE, Bool_t bMuch = kFALSE, Bool_t bTof = kFALSE );
    */
  template<class Digi>
  Int_t FillSystemOffsetHistos(TH1* histo, TH2* histoCharge, TH2* histoEvo, TH2* histoEvoLong, TH2* histoAFCK,
                               const Double_t T0Time, const Int_t offsetRange, Int_t iStartDigi, Bool_t bSts = kFALSE,
                               Bool_t bMuch = kFALSE, Bool_t bTof = kFALSE, Bool_t bPsd = kFALSE);

  template<class Digi>
  Int_t FillHistosSelT0(TH1* histo, const Double_t T0Time, const Int_t offsetRange, Int_t iStartDigi,
                        Bool_t bSts = kFALSE, Bool_t bMuch = kFALSE, Bool_t bTof = kFALSE, Bool_t bPsd = kFALSE);

  Int_t CalcNrBins(Int_t);
  void CreateHistos();
  void WriteHistos();
  void FitPeaks();

  /** Input array from previous already existing data level **/
  CbmDigiManager* fDigiMan = nullptr;  //!

  /** T0 is not included in CbmDigiManager, so add it explicitly here **/
  const std::vector<CbmTofDigi>* fT0DigiVec = nullptr;  //!
  TClonesArray* fT0DigiArr                  = nullptr;  //!

  /** Peak position of time difference histograms **/
  Double_t trd_peak_pos;
  Double_t sts_peak_pos;
  Double_t much_peak_pos;
  Double_t tof_peak_pos;
  Double_t rich_peak_pos;
  Double_t psd_peak_pos;

  /** Peak position of time difference histograms with TOF coincidence**/
  Double_t trd_coin_peak_pos;
  Double_t sts_coin_peak_pos;
  Double_t much_coin_peak_pos;
  Double_t rich_coin_peak_pos;
  Double_t psd_coin_peak_pos;

  /** Average height of bins **/
  Double_t trd_average;
  Double_t sts_average;
  Double_t much_average;
  Double_t tof_average;
  Double_t rich_average;
  Double_t psd_average;

  /** Typical width for Gaussian peaks**/
  Double_t trd_width0_ns;
  Double_t sts_width0_ns;
  Double_t much_width0_ns;
  Double_t tof_width0_ns;
  Double_t rich_width0_ns;
  Double_t psd_width0_ns;

  /// Variables to store the previous digi time
  Double_t fPrevTimeT0   = 0.;
  Double_t fPrevTimeSts  = 0.;
  Double_t fPrevTimeMuch = 0.;
  Double_t fPrevTimeTrd  = 0.;
  Double_t fPrevTimeTof  = 0.;
  Double_t fPrevTimeRich = 0.;
  Double_t fPrevTimePsd  = 0.;

  /// Variables to store the first digi fitting the previous T0 hits
  /// => Time-order means the time window for following one can only be in a later digi
  Int_t fPrevT0FirstDigiSts  = 0;
  Int_t fPrevT0FirstDigiMuch = 0;
  Int_t fPrevT0FirstDigiTrd  = 0;
  Int_t fPrevT0FirstDigiTof  = 0;
  Int_t fPrevT0FirstDigiRich = 0;
  Int_t fPrevT0FirstDigiPsd  = 0;

  /// User settings: Data correction parameters
  UInt_t fuMinTotPulserT0  = 90;
  UInt_t fuMaxTotPulserT0  = 100;
  UInt_t fuMinTotPulserTof = 92;
  UInt_t fuMaxTotPulserTof = 96;

  //
  Int_t fNrTs = 0;

  Int_t fNrOfT0Errors   = 0;
  Int_t fNrOfT0Digis    = 0;
  Int_t fNrOfStsErrors  = 0;
  Int_t fNrOfStsDigis   = 0;
  Int_t fNrOfMuchErrors = 0;
  Int_t fNrOfMuchDigis  = 0;
  Int_t fNrOfTrdErrors  = 0;
  Int_t fNrOfTrdDigis   = 0;
  Int_t fNrOfTofErrors  = 0;
  Int_t fNrOfTofDigis   = 0;
  Int_t fNrOfRichErrors = 0;
  Int_t fNrOfRichDigis  = 0;
  Int_t fNrOfPsdErrors  = 0;
  Int_t fNrOfPsdDigis   = 0;

  Bool_t fCheckTimeOrdering      = kTRUE;
  Bool_t fCheckInterSystemOffset = kTRUE;

  Int_t fOffsetRange     = 1000;
  Int_t fStsOffsetRange  = 1000;
  Int_t fMuchOffsetRange = 1000;
  Int_t fTrdOffsetRange  = 1000;
  Int_t fTofOffsetRange  = 1000;
  Int_t fRichOffsetRange = 1000;
  Int_t fPsdOffsetRange  = 1000;

  Int_t fNrTsForFit         = 200;
  Double_t fTrdPeakWidthNs  = 120.;
  Double_t fStsPeakWidthNs  = 30.;
  Double_t fMuchPeakWidthNs = 100.;
  Double_t fTofPeakWidthNs  = 20.;
  Double_t fRichPeakWidthNs = 40.;
  Double_t fPsdPeakWidthNs  = 20.;

  Int_t fBinWidth = 1;

  TH1* fT0StsDiff  = nullptr;
  TH1* fT0MuchDiff = nullptr;
  TH1* fT0TrdDiff  = nullptr;
  TH1* fT0TofDiff  = nullptr;
  TH1* fT0RichDiff = nullptr;
  TH1* fT0PsdDiff  = nullptr;

  TH2* fT0StsDiffCharge  = nullptr;
  TH2* fT0MuchDiffCharge = nullptr;
  TH2* fT0TrdDiffCharge  = nullptr;
  TH2* fT0TofDiffCharge  = nullptr;
  TH2* fT0RichDiffCharge = nullptr;
  TH2* fT0PsdDiffCharge  = nullptr;

  TH2* fT0StsDiffEvo  = nullptr;
  TH2* fT0MuchDiffEvo = nullptr;
  TH2* fT0TrdDiffEvo  = nullptr;
  TH2* fT0TofDiffEvo  = nullptr;
  TH2* fT0RichDiffEvo = nullptr;
  TH2* fT0PsdDiffEvo  = nullptr;

  TH2* fT0StsDiffEvoLong  = nullptr;
  TH2* fT0MuchDiffEvoLong = nullptr;
  TH2* fT0TrdDiffEvoLong  = nullptr;
  TH2* fT0TofDiffEvoLong  = nullptr;
  TH2* fT0RichDiffEvoLong = nullptr;
  TH2* fT0PsdDiffEvoLong  = nullptr;

  TH1* fT0T0Diff     = nullptr;
  TH1* fStsStsDiff   = nullptr;
  TH1* fMuchMuchDiff = nullptr;
  TH1* fTrdTrdDiff   = nullptr;
  TH1* fTofTofDiff   = nullptr;
  TH1* fRichRichDiff = nullptr;
  TH1* fPsdPsdDiff   = nullptr;

  TH1* fT0Address = nullptr;
  TH1* fT0Channel = nullptr;

  TH2* fT0StsDpbDiff                 = nullptr;
  static const UInt_t kuMaxNbStsDpbs = 2;
  TH2* fT0StsDpbDiffEvo[kuMaxNbStsDpbs];
  TH1* fStsDpbCntsEvo[kuMaxNbStsDpbs];

  static const UInt_t kuMaxNbMuchDpbs  = 6;
  TH2* fT0MuchRocDiff                  = nullptr;
  static const UInt_t kuMaxNbMuchAsics = 36;
  TH2* fT0MuchAsicDiff                 = nullptr;
  TH2* fT0MuchAsicDiffEvo[kuMaxNbMuchAsics];

  UInt_t fuNbTofDigisSel   = 6;
  UInt_t fuNbTofDigiInSync = 0;
  std::vector<Double_t> fvdTimeSelectedT0;  //!
  TH1* fSelT0StsDiff  = nullptr;
  TH1* fSelT0MuchDiff = nullptr;
  TH1* fSelT0TrdDiff  = nullptr;
  TH1* fSelT0TofDiff  = nullptr;
  TH1* fSelT0RichDiff = nullptr;
  TH1* fSelT0PsdDiff  = nullptr;

  TString fOutFileName {"test.root"};

  ClassDef(CbmCheckTiming, 2);
};

#endif
