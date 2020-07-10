/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019CHECKTIMINGPAIRS_H
#define CBMMCBM2019CHECKTIMINGPAIRS_H

/// CBMROOT headers
#include "CbmDefs.h"
#include "TimesliceMetaData.h"

#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

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

class CbmMcbm2019CheckTimingPairs : public FairTask {
public:
  CbmMcbm2019CheckTimingPairs();

  CbmMcbm2019CheckTimingPairs(const CbmMcbm2019CheckTimingPairs&) = delete;
  CbmMcbm2019CheckTimingPairs
  operator=(const CbmMcbm2019CheckTimingPairs&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbm2019CheckTimingPairs(Int_t verbose);


  /** Destructor **/
  ~CbmMcbm2019CheckTimingPairs();


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

  void SetStsOffsetSearchRange(Double_t val = 1000) { fdStsTimeWin = val; }

  void SetMuchOffsetSearchRange(Double_t val = 1000) { fdMuchTimeWin = val; }

  void SetTrdOffsetSearchRange(Double_t val = 1000) { fdTrdTimeWin = val; }

  void SetTofOffsetSearchRange(Double_t val = 1000) { fdTofTimeWin = val; }

  void SetRichOffsetSearchRange(Double_t val = 1000) { fdRichTimeWin = val; }

  void SetPsdOffsetSearchRange(Double_t val = 1000) { fdPsdTimeWin = val; }

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
  void CreateHistos();
  void WriteHistos();

  template<class Digi>
  UInt_t FillCorrBuffer(Double_t dTimeT0,
                        UInt_t uIndexStart,
                        Double_t dWinStartTime,
                        Double_t dWinStopTime,
                        std::vector<std::vector<Digi>>& vDigi,
                        ECbmModuleId iDetId = ECbmModuleId::kLastModule);
  template<class DigiA>
  void FillHistosInter(UInt_t uIndexT0,
                       UInt_t uIndexA,
                       UInt_t uIndexB,
                       std::vector<DigiA>& vCorrDigA,
                       UInt_t uHistoIdx);
  template<class DigiA, class DigiB>
  void FillHistos(UInt_t uIndexT0,
                  UInt_t uIndexA,
                  UInt_t uIndexB,
                  std::vector<DigiA>& vCorrDigA,
                  std::vector<DigiB>& vCorrDigB,
                  UInt_t uHistoIdx);

  /** Digi data **/
  CbmDigiManager* fDigiMan                     = nullptr;  //!
  const std::vector<CbmTofDigi>* fT0DigiVector = nullptr;  //!
  TClonesArray* fT0DigiArray                   = nullptr;  //!
  TClonesArray* fTimeSliceMetaDataArray        = nullptr;  //!
  const TimesliceMetaData* pTsMetaData         = nullptr;

  /// Constants
  static const UInt_t kuNbChanSMX        = 128;
  static const UInt_t kuMaxNbStsDpbs     = 2;
  static const UInt_t kuMaxNbMuchDpbs    = 6;
  static const UInt_t kuMaxNbMuchAsics   = 36;
  static const UInt_t kuDefaultAddress   = 0xFFFFFFFF;
  static const UInt_t kuMaxChannelSts    = 3000;
  static const UInt_t kuNbBinsDefault    = 2000;
  static constexpr Double_t kdClockCycle = 3.125;
  static constexpr Double_t kdDefaultTimeWin =
    kdClockCycle * kuNbBinsDefault / 2;

  /// List of detectors
  std::vector<std::string> fvsDetectors =
    {"STS", "MUCH", "TRD", "TOF", "RICH", "PSD"};  //!
  UInt_t fuNbDetectors = fvsDetectors.size();

  /// Variables to store the previous digi time
  Double_t fPrevTimeT0 = 0.;
  std::vector<Double_t> fvPrevTimeDet =
    std::vector<Double_t>(fuNbDetectors, 0.0);  //!

  /// Variables to store the first digi fitting the previous T0 hits
  /// => Time-order means the time window for following one can only be in a later digi
  std::vector<UInt_t> fvuPrevT0FirstDigiDet =
    std::vector<UInt_t>(fuNbDetectors, 0);  //!

  /// Variable to store correlated Digis
  std::vector<CbmTofDigi> fvDigisT0                         = {};  //!
  std::vector<std::vector<CbmStsDigi>> fvDigisSts           = {};  //!
  std::vector<std::vector<CbmMuchBeamTimeDigi>> fvDigisMuch = {};  //!
  std::vector<std::vector<CbmTrdDigi>> fvDigisTrd           = {};  //!
  std::vector<std::vector<CbmTofDigi>> fvDigisTof           = {};  //!
  std::vector<std::vector<CbmRichDigi>> fvDigisRich         = {};  //!
  std::vector<std::vector<CbmPsdDigi>> fvDigisPsd           = {};  //!

  /// Variable to store counts of T0 with at least one coincidence
  UInt_t fuNbDigisWithCoincT0 = 0;
  /// Variable to store counts of T0 with at least one coincidence
  UInt_t fuNbCoincDigisSts  = 0;
  UInt_t fuNbCoincDigisMuch = 0;
  UInt_t fuNbCoincDigisTrd  = 0;
  UInt_t fuNbCoincDigisTof  = 0;
  UInt_t fuNbCoincDigisRich = 0;
  UInt_t fuNbCoincDigisPsd  = 0;

  /// User settings: Data correction parameters
  /// Charge cut
  UInt_t fuMinTotPulserT0     = 182;
  UInt_t fuMaxTotPulserT0     = 190;
  UInt_t fuMinAdcPulserSts    = 90;
  UInt_t fuMaxAdcPulserSts    = 100;
  UInt_t fuMinAdcPulserMuch   = 5;
  UInt_t fuMaxAdcPulserMuch   = 15;
  UInt_t fuMinChargePulserTrd = 700000;  /// Default: No cut
  UInt_t fuMaxChargePulserTrd = 0;       /// Default: No cut
  UInt_t fuMinTotPulserTof    = 182;
  UInt_t fuMaxTotPulserTof    = 190;
  UInt_t fuMinTotPulserRich   = 700000;  /// Default: No cut
  UInt_t fuMaxTotPulserRich   = 0;       /// Default: No cut
  UInt_t fuMinAdcPulserPsd    = 700000;  /// Default: No cut
  UInt_t fuMaxAdcPulserPsd    = 0;       /// Default: No cut
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

  Double_t fdStsTimeWin  = kdDefaultTimeWin;
  Double_t fdMuchTimeWin = kdDefaultTimeWin;
  Double_t fdTrdTimeWin  = kdDefaultTimeWin;
  Double_t fdTofTimeWin  = kdDefaultTimeWin;
  Double_t fdRichTimeWin = kdDefaultTimeWin;
  Double_t fdPsdTimeWin  = kdDefaultTimeWin;

  Int_t fBinWidth = 1;

  std::vector<TH1*> fhDtADtB = {};  //!

  TString fOutFileName {"data/HistosTimingPairs.root"};

  ClassDef(CbmMcbm2019CheckTimingPairs, 1);
};

#endif  // CBMMCBM2019CHECKTIMINGPAIRS_H
