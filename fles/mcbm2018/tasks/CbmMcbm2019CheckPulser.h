/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019CHECKPULSER_H
#define CBMMCBM2019CHECKPULSER_H

/// CBMROOT headers
#include "CbmDefs.h"
#include "CbmTofDigi.h"
#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairTask.h"

/// External headers (ROOT, boost, ...)
#include "TString.h"

/// C/C++ headers
#include <vector>

class TClonesArray;
class TH1;
class TH2;
class TProfile;
class CbmDigiManager;

class CbmMcbm2019CheckPulser : public FairTask {
public:
  CbmMcbm2019CheckPulser();

  CbmMcbm2019CheckPulser(const CbmMcbm2019CheckPulser&) = delete;
  CbmMcbm2019CheckPulser operator=(const CbmMcbm2019CheckPulser&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbm2019CheckPulser(Int_t verbose);


  /** Destructor **/
  ~CbmMcbm2019CheckPulser();


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

  void SetOffsetSearchRange(Int_t val = 1000) { fOffsetRange = val; }

  void SetStsOffsetSearchRange(Int_t val = 1000) { fStsOffsetRange = val; }

  void SetMuchOffsetSearchRange(Int_t val = 1000) { fMuchOffsetRange = val; }

  void SetTrdOffsetSearchRange(Int_t val = 1000) { fTrdOffsetRange = val; }

  void SetTofOffsetSearchRange(Int_t val = 1000) { fTofOffsetRange = val; }

  void SetRichOffsetSearchRange(Int_t val = 1000) { fRichOffsetRange = val; }

  void SetPsdOffsetSearchRange(Int_t val = 1000) { fPsdOffsetRange = val; }

  inline void SetT0PulserTotLimits(UInt_t uMin, UInt_t uMax) {
    fuMinTotPulserT0 = uMin;
    fuMaxTotPulserT0 = uMax;
  }
  inline void SetStsPulserAdcLimits(UInt_t uMin, UInt_t uMax) {
    fuMinAdcPulserSts = uMin;
    fuMaxAdcPulserSts = uMax;
  }
  inline void SetMuchPulserAdcLimits(UInt_t uMin, UInt_t uMax) {
    fuMinAdcPulserMuch = uMin;
    fuMaxAdcPulserMuch = uMax;
  }
  inline void SetTrdPulserChargeLimits(UInt_t uMin, UInt_t uMax) {
    fuMinChargePulserTrd = uMin;
    fuMaxChargePulserTrd = uMax;
  }
  inline void SetTofPulserTotLimits(UInt_t uMin, UInt_t uMax) {
    fuMinTotPulserTof = uMin;
    fuMaxTotPulserTof = uMax;
  }
  inline void SetRichPulserTotLimits(UInt_t uMin, UInt_t uMax) {
    fuMinTotPulserRich = uMin;
    fuMaxTotPulserRich = uMax;
  }
  inline void SetPsdPulserAdcLimits(UInt_t uMin, UInt_t uMax) {
    fuMinAdcPulserPsd = uMin;
    fuMaxAdcPulserPsd = uMax;
  }

  inline void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }

  inline void SetStsAddress(UInt_t uAddress) { fuStsAddress = uAddress; }
  inline void SetMuchAsic(UInt_t uAsic) { fuMuchAsic = uAsic; }
  inline void SetMuchChanRange(UInt_t uFirstChan,
                               UInt_t uLastChan = kuNbChanSMX) {
    fuMuchFirstCha = uFirstChan;
    fuMuchLastChan = uLastChan;
  }
  inline void SetTrdAddress(UInt_t uAddress) { fuTrdAddress = uAddress; }
  inline void SetPsdAddress(UInt_t uAddress) { fuPsdAddress = uAddress; }

private:
  void CheckTimeOrder();
  Int_t CheckIfSorted(TClonesArray*, TH1*, Double_t&, TString);

  void CheckInterSystemOffset();

  template<class Digi>
  Int_t FillSystemOffsetHistos(TH1* histo,
                               TH2* histoEvo,
                               TH2* histoEvoLong,
                               TProfile* profMeanEvo,
                               TH2* histoAFCK,
                               const Double_t T0Time,
                               const Int_t offsetRange,
                               Int_t iStartDigi,
                               ECbmModuleId iDetId = ECbmModuleId::kLastModule);

  Int_t CalcNrBins(Int_t);
  void CreateHistos();
  void WriteHistos();


  /** Digi data **/
  CbmDigiManager* fDigiMan                     = nullptr;  //!
  const std::vector<CbmTofDigi>* fT0DigiVector = nullptr;  //!
  TClonesArray* fT0DigiArray                   = nullptr;  //!
  TClonesArray* fTimeSliceMetaDataArray        = nullptr;  //!
  const TimesliceMetaData* pTsMetaData         = nullptr;

  /// Constants
  static const UInt_t kuNbChanSMX      = 128;
  static const UInt_t kuMaxNbStsDpbs   = 2;
  static const UInt_t kuMaxNbMuchDpbs  = 6;
  static const UInt_t kuMaxNbMuchAsics = 36;
  static const UInt_t kuDefaultAddress = 0xFFFFFFFF;
  static const UInt_t kuMaxChannelSts  = 3000;

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
  /// Charge cut
  UInt_t fuMinTotPulserT0     = 182;
  UInt_t fuMaxTotPulserT0     = 190;
  UInt_t fuMinAdcPulserSts    = 90;
  UInt_t fuMaxAdcPulserSts    = 100;
  UInt_t fuMinAdcPulserMuch   = 5;
  UInt_t fuMaxAdcPulserMuch   = 15;
  UInt_t fuMinChargePulserTrd = 0;
  UInt_t fuMaxChargePulserTrd = 70000;
  UInt_t fuMinTotPulserTof    = 182;
  UInt_t fuMaxTotPulserTof    = 190;
  UInt_t fuMinTotPulserRich   = 90;
  UInt_t fuMaxTotPulserRich   = 105;
  UInt_t fuMinAdcPulserPsd    = 90;
  UInt_t fuMaxAdcPulserPsd    = 100;
  /// Channel selection
  UInt_t fuStsAddress   = kuDefaultAddress;
  UInt_t fuStsFirstCha  = kuMaxChannelSts;
  UInt_t fuStsLastChan  = kuMaxChannelSts;
  UInt_t fuMuchAsic     = kuMaxNbMuchAsics;
  UInt_t fuMuchFirstCha = kuNbChanSMX;
  UInt_t fuMuchLastChan = kuNbChanSMX;
  UInt_t fuTrdAddress   = kuDefaultAddress;
  UInt_t fuPsdAddress   = kuDefaultAddress;

  //
  Int_t fNrTs = 0;

  Int_t fOffsetRange     = 1000;
  Int_t fStsOffsetRange  = 1000;
  Int_t fMuchOffsetRange = 1000;
  Int_t fTrdOffsetRange  = 1000;
  Int_t fTofOffsetRange  = 1000;
  Int_t fRichOffsetRange = 1000;
  Int_t fPsdOffsetRange  = 1000;

  Int_t fBinWidth = 1;

  TH1* fT0StsDiff       = nullptr;
  TH1* fT0MuchDiff      = nullptr;
  TH1* fT0TrdDiff       = nullptr;
  TH1* fT0TofDiff       = nullptr;
  TH1* fT0RichDiff      = nullptr;
  TH1* fT0PsdDiff       = nullptr;
  TH2* fT0PsdDiffCharge = nullptr;

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

  Double_t fdStartTime     = -1;
  TProfile* fT0StsMeanEvo  = nullptr;
  TProfile* fT0MuchMeanEvo = nullptr;
  TProfile* fT0TrdMeanEvo  = nullptr;
  TProfile* fT0TofMeanEvo  = nullptr;
  TProfile* fT0RichMeanEvo = nullptr;
  TProfile* fT0PsdMeanEvo  = nullptr;

  TH1* fT0T0Diff     = nullptr;
  TH1* fStsStsDiff   = nullptr;
  TH1* fMuchMuchDiff = nullptr;
  TH1* fTrdTrdDiff   = nullptr;
  TH1* fTofTofDiff   = nullptr;
  TH1* fRichRichDiff = nullptr;
  TH1* fPsdPsdDiff   = nullptr;

  TH2* fT0StsNb  = nullptr;
  TH2* fT0MuchNb = nullptr;
  TH2* fT0TrdNb  = nullptr;
  TH2* fT0TofNb  = nullptr;
  TH2* fT0RichNb = nullptr;
  TH2* fT0PsdNb  = nullptr;

  Int_t fiT0Nb   = 0;
  Int_t fiStsNb  = 0;
  Int_t fiMuchNb = 0;
  Int_t fiTrdNb  = 0;
  Int_t fiTofNb  = 0;
  Int_t fiRichNb = 0;
  Int_t fiPsdNb  = 0;

  TH1* fT0Address = nullptr;
  TH1* fT0Channel = nullptr;

  TH2* fT0StsDpbDiff = nullptr;
  TH2* fT0StsDpbDiffEvo[kuMaxNbStsDpbs];
  TH1* fStsDpbCntsEvo[kuMaxNbStsDpbs];

  TH2* fT0MuchRocDiff  = nullptr;
  TH2* fT0MuchAsicDiff = nullptr;
  TH2* fT0MuchAsicDiffEvo[kuMaxNbMuchAsics];

  TH2* fDigisPerAsicEvo = nullptr;
  Double_t fdLastMuchDigi[kuMaxNbMuchAsics][kuNbChanSMX];
  Double_t fdLastMuchDigiPulser[kuMaxNbMuchAsics][kuNbChanSMX];
  TH2* fSameChanDigisDistEvo = nullptr;

  Double_t fdLastT0DigiPulser = 0;

  TH2* fDigiTimeEvoT0   = nullptr;
  TH2* fDigiTimeEvoSts  = nullptr;
  TH2* fDigiTimeEvoMuch = nullptr;
  TH2* fDigiTimeEvoTof  = nullptr;


  TString fOutFileName {"data/HistosPulserChecker.root"};

  ClassDef(CbmMcbm2019CheckPulser, 1);
};

#endif  // CBMMCBM2019CHECKPULSER_H
