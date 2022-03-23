/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CbmMuchUnpackMonitor_H
#define CbmMuchUnpackMonitor_H

#include "CbmMuchUnpackPar.h"

#include "Rtypes.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <cstdint>

#include "StsXyterMessage.h"

class CbmQaCanvas;

class CbmMuchUnpackMonitor {
public:
  CbmMuchUnpackMonitor();

  virtual ~CbmMuchUnpackMonitor();

  //Variables for debugging info
  std::vector<uint32_t> vNbMessType;
  std::string sMessPatt = "";
  bool bError           = false;

  void ResetDebugInfo();
  void PrintDebugInfo(const uint64_t MsStartTime, const size_t NrProcessedTs, const uint16_t msDescriptorFlags,
                      const uint32_t uSize);

  /** @brief Init all required parameter informations and histograms */
  Bool_t Init(CbmMuchUnpackPar* digiParSet);

  Bool_t CreateHistograms(CbmMuchUnpackPar* pUnpackPar);
  Bool_t ResetHistograms();

  Bool_t CreateDebugHistograms(CbmMuchUnpackPar* pUnpackPar);
  Bool_t ResetDebugHistograms();

  Bool_t CreateMsComponentSizeHistos(UInt_t component);
  Bool_t ResetMsComponentSizeHistos(UInt_t component);

  /** @brief Write all histograms and canvases to file */
  void Finish();

  void SetHistoFileName(TString nameIn) { fHistoFileName = nameIn; }

  void DrawCanvases();

  void AddHistoToVector(TNamed* pointer, std::string sFolder = "")
  {
    fvpAllHistoPointers.push_back(std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() { return fvpAllHistoPointers; }

  void AddCanvasToVector(CbmQaCanvas* pointer, std::string sFolder = "")
  {
    fvpAllCanvasPointers.push_back(std::pair<CbmQaCanvas*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<CbmQaCanvas*, std::string>> GetCanvasVector() { return fvpAllCanvasPointers; }

  void SetLongDurationLimits(UInt_t uDurationSeconds, UInt_t uBinSize)
  {
    //fbLongHistoEnable     = kTRUE;
    fuLongHistoNbSeconds  = uDurationSeconds;
    fuLongHistoBinSizeSec = uBinSize;
  }

  UInt_t GetMaxNbFlibLinks() { return kiMaxNbFlibLinks; }

  ///Fill general histograms
  void FillVectorSize(ULong64_t TsIdx, UInt_t Size) { fhVectorSize->Fill(TsIdx, Size); }
  void FillVectorCapacity(ULong64_t TsIdx, UInt_t Capacity) { fhVectorCapacity->Fill(TsIdx, Capacity); }
  void FillMsCntEvo(ULong64_t MsIdx) { fhMsCntEvo->Fill(MsIdx * 1e-9); }
  void FillMsErrorsEvo(ULong64_t MsIdx, uint16_t MsErrorType) { fhMsErrorsEvo->Fill(1e-9 * MsIdx, MsErrorType); }
  void FillDigisTimeInRun(Double_t Time) { fhDigisTimeInRun->Fill(Time * 1e-9); }
  void FillMuchAllFebsHitRateEvo(Double_t dTimeSinceStartSec, UInt_t uFebIdx)
  {
    fhMuchAllFebsHitRateEvo->Fill(dTimeSinceStartSec, uFebIdx);
  }
  void FillMuchAllAsicsHitRateEvo(Double_t dTimeSinceStartSec, UInt_t uAsicIdx)
  {
    fhMuchAllAsicsHitRateEvo->Fill(dTimeSinceStartSec, uAsicIdx);
  }
  void FillMuchFebAsicHitCounts(UInt_t uFebIdx, UInt_t uAsicInFeb)
  {
    fhMuchFebAsicHitCounts->Fill(uFebIdx, uAsicInFeb);
  }
  void FillMuchStatusMessType(UInt_t uAsicIdx, UShort_t usStatusField)
  {
    fhMuchStatusMessType->Fill(uAsicIdx, usStatusField);
  }

  ///Fill general "per Feb" histogram vectors
  void FillMuchFebChanCntRaw(UInt_t uFebIdx, UInt_t uChanInFeb) { fvhMuchFebChanCntRaw[uFebIdx]->Fill(uChanInFeb); }
  void FillMuchFebChanAdcRaw(UInt_t uFebIdx, UInt_t uChanInFeb, UShort_t usRawAdc)
  {
    fvhMuchFebChanAdcRaw[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  }
  void FillMuchFebChanAdcRawProf(UInt_t uFebIdx, UInt_t uChanInFeb, UShort_t usRawAdc)
  {
    fvhMuchFebChanAdcRawProf[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  }
  void FillMuchFebChanAdcCal(UInt_t uFebIdx, UInt_t uChanInFeb, Double_t dCalAdc)
  {
    fvhMuchFebChanAdcCal[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  }
  void FillMuchFebChanAdcCalProf(UInt_t uFebIdx, UInt_t uChanInFeb, Double_t dCalAdc)
  {
    fvhMuchFebChanAdcCalProf[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  }
  void FillMuchFebChanRawTs(UInt_t uFebIdx, UInt_t uChan, UShort_t usRawTs)
  {
    fvhMuchFebChanRawTs[uFebIdx]->Fill(uChan, usRawTs);
  }
  void FillMuchFebChanMissEvt(UInt_t uFebIdx, UInt_t uChan, bool missEvtFlag)
  {
    fvhMuchFebChanMissEvt[uFebIdx]->Fill(uChan, missEvtFlag);
  }
  void FillMuchFebChanMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uChanInFeb)
  {
    fvhMuchFebChanMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
  }
  void FillMuchFebAsicMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uAsicInFeb)
  {
    fvhMuchFebAsicMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
  }
  void FillMuchFebMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec)
  {
    fvhMuchFebMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  }
  void FillMuchFebChanHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uChanInFeb)
  {
    fvhMuchFebChanHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
  }
  void FillMuchFebAsicHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uAsicInFeb)
  {
    fvhMuchFebAsicHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
  }
  void FillMuchFebHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec)
  {
    fvhMuchFebHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  }
  void FillMuchFebChanHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin, UInt_t uChanInFeb)
  {
    fvhMuchFebChanHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, uChanInFeb, 1.0 / 60.0);
  }
  void FillMuchFebAsicHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin, UInt_t uAsicInFeb)
  {
    fvhMuchFebAsicHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, uAsicInFeb, 1.0 / 60.0);
  }
  void FillMuchFebHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin)
  {
    fvhMuchFebHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, 1.0 / 60.0);
  }

  ///Fill debugging histograms
  void FillMuchMessType(uint16_t typeMess) { fhMuchMessType->Fill(typeMess); }
  void FillMuchMessTypePerDpb(UInt_t DpbIdx, uint16_t typeMess) { fhMuchMessTypePerDpb->Fill(DpbIdx, typeMess); }
  void FillMuchMessTypePerElink(UInt_t ElinkIdx, uint16_t typeMess)
  {
    fhMuchMessTypePerElink->Fill(ElinkIdx, typeMess);
  }
  void FillMuchHitsElinkPerDpb(UInt_t DpbIdx, UInt_t ElinkIdx) { fhMuchHitsElinkPerDpb->Fill(DpbIdx, ElinkIdx); }
  void FillMuchDpbRawTsMsb(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhMuchDpbRawTsMsb->Fill(uCurrDpbIdx, ulCurrentTsMsb);
  }
  void FillMuchDpbRawTsMsbSx(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhMuchDpbRawTsMsbSx->Fill(uCurrDpbIdx, (ulCurrentTsMsb & 0x1F));
  }
  void FillMuchDpbRawTsMsbDpb(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhMuchDpbRawTsMsbDpb->Fill(uCurrDpbIdx, (ulCurrentTsMsb >> 5));
  }

  ///Fill debugging "per Asic" histogram vectors
  void FillMuchChanCntRaw(UInt_t uAsicIdx, UShort_t usChan) { fvhMuchChanCntRaw[uAsicIdx]->Fill(usChan); }
  void FillMuchChanAdcRaw(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawAdc)
  {
    fvhMuchChanAdcRaw[uAsicIdx]->Fill(usChan, usRawAdc);
  }
  void FillMuchChanAdcRawProf(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawAdc)
  {
    fvhMuchChanAdcRawProf[uAsicIdx]->Fill(usChan, usRawAdc);
  }
  void FillMuchChanRawTs(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawTs)
  {
    fvhMuchChanRawTs[uAsicIdx]->Fill(usChan, usRawTs);
  }
  void FillMuchChanMissEvt(UInt_t uAsicIdx, UShort_t usChan, bool missedEvtFlag)
  {
    fvhMuchChanMissEvt[uAsicIdx]->Fill(usChan, missedEvtFlag);
  }

  //Fill Ms Component Size Histos
  void FillMsSize(UInt_t uMsComp, UInt_t uSize) { fvhMsSize[uMsComp]->Fill(uSize); }
  void FillMsSizeTime(UInt_t uMsComp, Double_t dTime, UInt_t uSize) { fvhMsSizeTime[uMsComp]->Fill(dTime, uSize); }

  void FillHitMonitoringHistos(const UInt_t& uFebIdx, const UShort_t& usChan, const UInt_t& uChanInFeb,
                               const UShort_t& usRawAdc, const Double_t& dCalAdc, const UShort_t& usRawTs,
                               const bool& isHitMissedEvts);

  void FillHitDebugMonitoringHistos(const UInt_t& uAsicIdx, const UShort_t& usChan, const UShort_t& usRawAdc,
                                    const UShort_t& usRawTs, const bool& isHitMissedEvts);

  void FillHitEvoMonitoringHistos(const UInt_t& uFebIdx, const UInt_t& uAsicIdx, const UInt_t& uAsicInFeb,
                                  const UInt_t& uChanInFeb, const Double_t& dTimeSinceStartSec,
                                  const bool& isHitMissedEvts);

  void CountRawHit(uint32_t uFebIdx, uint32_t uChanInFeb)
  {
    fvuNbRawTsFeb[uFebIdx]++;
    fvvuNbRawTsChan[uFebIdx][uChanInFeb]++;
  }
  void CountDigi(uint32_t uFebIdx, uint32_t uChanInFeb)
  {
    fvuNbDigisTsFeb[uFebIdx]++;
    fvvuNbDigisTsChan[uFebIdx][uChanInFeb]++;
  }

  void FillPerTimesliceCountersHistos(double_t dTsStartTime);
  void FillDuplicateHitsAdc(const uint32_t& uFebIdx, const uint32_t& uChanInFeb, const uint16_t& usAdc);

  /** @brief Activate the debug mode */
  bool GetDebugMode() { return fDebugMode; }

  void ProcessDebugInfo(const stsxyter::Message& Mess, const UInt_t& uCurrDpbIdx);

  /** @brief Activate the debug mode */
  void SetDebugMode(bool value) { fDebugMode = value; }

private:
  TString fHistoFileName = "HistosUnpackerMuch.root";

  /// Rate evolution histos
  double_t dFirstTsStartTime = 0;
  //Bool_t fbLongHistoEnable;
  UInt_t fuLongHistoNbSeconds  = 3600;
  UInt_t fuLongHistoBinSizeSec = 10;
  UInt_t fuLongHistoBinNb      = 1;

  /// Per timeslice counters to evaluate the eventual raw messages rejection per FEB
  std::vector<uint32_t> fvuNbRawTsFeb   = {};
  std::vector<uint32_t> fvuNbDigisTsFeb = {};
  /// Per timeslice counters to evaluate the eventual raw messages rejection per [FEB, chan] pairs
  std::vector<std::vector<uint32_t>> fvvuNbRawTsChan   = {};
  std::vector<std::vector<uint32_t>> fvvuNbDigisTsChan = {};

  /// Canvases
  std::vector<CbmQaCanvas*> fvcMuchSumm;
  std::vector<CbmQaCanvas*> fvcMuchSmxErr;

  ///General histograms
  TH1* fhDigisTimeInRun         = nullptr;
  TH1* fhVectorSize             = nullptr;
  TH1* fhVectorCapacity         = nullptr;
  TH1* fhMsCntEvo               = nullptr;
  TH2* fhMsErrorsEvo            = nullptr;
  TH2* fhMuchAllFebsHitRateEvo  = nullptr;
  TH2* fhMuchAllAsicsHitRateEvo = nullptr;
  TH2* fhMuchFebAsicHitCounts   = nullptr;
  TH2* fhMuchStatusMessType     = nullptr;
  TProfile* fhRawHitRatioPerFeb = nullptr;

  ///General "per Feb" histogram vectors
  std::vector<TProfile*> fvhRawChRatio         = {};
  std::vector<TProfile*> fvhHitChRatio         = {};
  std::vector<TProfile*> fvhDupliChRatio       = {};
  std::vector<TProfile*> fvhRawHitRatioPerCh   = {};
  std::vector<TProfile*> fvhRawDupliRatioPerCh = {};
  std::vector<TH1*> fvhMuchFebChanCntRaw;
  std::vector<TH2*> fvhMuchFebChanAdcRaw;
  std::vector<TProfile*> fvhMuchFebChanAdcRawProf;
  std::vector<TH2*> fvhMuchFebChanAdcCal;
  std::vector<TProfile*> fvhMuchFebChanAdcCalProf;
  std::vector<TH2*> fvhMuchFebChanRawTs;
  std::vector<TH2*> fvhMuchFebChanMissEvt;
  std::vector<TH2*> fvhMuchFebChanMissEvtEvo;
  std::vector<TH2*> fvhMuchFebAsicMissEvtEvo;
  std::vector<TH1*> fvhMuchFebMissEvtEvo;
  std::vector<TH2*> fvhMuchFebChanHitRateEvo;
  std::vector<TH2*> fvhMuchFebAsicHitRateEvo;
  std::vector<TH1*> fvhMuchFebHitRateEvo;
  std::vector<TH2*> fvhMuchFebChanHitRateEvoLong;
  std::vector<TH2*> fvhMuchFebAsicHitRateEvoLong;
  std::vector<TH1*> fvhMuchFebHitRateEvoLong;

  /** @brief Flag if debug mode is active or not */
  bool fDebugMode = false;

  /// Debugging histograms
  TH1* fhMuchMessType         = nullptr;
  TH2* fhMuchMessTypePerDpb   = nullptr;
  TH2* fhMuchMessTypePerElink = nullptr;
  TH2* fhMuchHitsElinkPerDpb  = nullptr;
  TH2* fhMuchDpbRawTsMsb      = nullptr;
  TH2* fhMuchDpbRawTsMsbSx    = nullptr;
  TH2* fhMuchDpbRawTsMsbDpb   = nullptr;

  TProfile2D* fhRawHitRatioEvoPerFeb                = nullptr;
  std::vector<TH2*> fvhChDupliAdc                   = {};
  std::vector<TProfile2D*> fvhRawChRatioEvo         = {};
  std::vector<TProfile2D*> fvhHitChRatioEvo         = {};
  std::vector<TProfile2D*> fvhDupliChRatioEvo       = {};
  std::vector<TProfile2D*> fvhRawHitRatioEvoPerCh   = {};
  std::vector<TProfile2D*> fvhRawDupliRatioEvoPerCh = {};

  static const UInt_t kiMaxNbFlibLinks = 32;
  TH1* fvhMsSize[kiMaxNbFlibLinks];
  TProfile* fvhMsSizeTime[kiMaxNbFlibLinks];

  ///Debugging "per Asic" histogram vectors
  std::vector<TH1*> fvhMuchChanCntRaw;
  std::vector<TH2*> fvhMuchChanAdcRaw;
  std::vector<TProfile*> fvhMuchChanAdcRawProf;
  std::vector<TH2*> fvhMuchChanRawTs;
  std::vector<TH2*> fvhMuchChanMissEvt;

  /** @brief Number of elinks per dpb, extracted from the parset */
  uint32_t fNrElinksPerDpb = 0;


  /// For monitoring of internal processes.
  /// => Pointers should be filled with TH1*, TH2*, TProfile*, ...
  /// ==> To check if object N is of type T, use "T ObjectPointer = dynamic_cast<T>( fvpAllHistoPointers[N].first );" and check for nullptr
  /// ==> To get back the original class name use "fvpAllHistoPointers[N].first->ClassName()" which returns a const char * (e.g. "TH1I")
  /// ===> Usage example with feeding a THttpServer:
  /// ===> #include "TH2.h"
  /// ===> std::string sClassName = vHistos[ uHisto ].first.ClassName();
  /// ===> if( !strncmp( sClassName, "TH1", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH1 * >(vHistos[ uHisto ].first) );
  /// ===> else if( !strncmp( sClassName, "TH2", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH2 * >(vHistos[ uHisto ].first) );
  std::vector<std::pair<TNamed*, std::string>>
    fvpAllHistoPointers;  //! Vector of pointers to histograms + optional folder name
  std::vector<std::pair<CbmQaCanvas*, std::string>>
    fvpAllCanvasPointers;  //! Vector of pointers to canvases + optional folder name

  CbmMuchUnpackMonitor(const CbmMuchUnpackMonitor&);
  CbmMuchUnpackMonitor operator=(const CbmMuchUnpackMonitor&);

  ClassDef(CbmMuchUnpackMonitor, 1)
};

#endif
