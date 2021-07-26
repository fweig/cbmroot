// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStsUnpackMonitor                       -----
// -----               Created 26.01.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStsUnpackMonitor_H
#define CbmStsUnpackMonitor_H

#include "CbmMcbm2018StsPar.h"

#include "Rtypes.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include <cstdint>

#include "StsXyterMessage.h"

class CbmQaCanvas;

class CbmStsUnpackMonitor {
public:
  CbmStsUnpackMonitor();

  virtual ~CbmStsUnpackMonitor();

  //Variables for debugging info
  std::vector<uint32_t> vNbMessType;
  std::string sMessPatt = "";
  bool bError           = false;

  void ResetDebugInfo();
  void PrintDebugInfo(const uint64_t MsStartTime, const size_t NrProcessedTs, const uint16_t msDescriptorFlags,
                      const uint32_t uSize);

  /** @brief Init all required parameter informations and histograms */
  Bool_t Init(CbmMcbm2018StsPar* digiParSet);

  Bool_t CreateHistograms(CbmMcbm2018StsPar* pUnpackPar);
  Bool_t ResetHistograms();

  Bool_t CreateDebugHistograms(CbmMcbm2018StsPar* pUnpackPar);
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
  void FillStsAllFebsHitRateEvo(Double_t dTimeSinceStartSec, UInt_t uFebIdx)
  {
    fhStsAllFebsHitRateEvo->Fill(dTimeSinceStartSec, uFebIdx);
  }
  void FillStsAllAsicsHitRateEvo(Double_t dTimeSinceStartSec, UInt_t uAsicIdx)
  {
    fhStsAllAsicsHitRateEvo->Fill(dTimeSinceStartSec, uAsicIdx);
  }
  void FillStsFebAsicHitCounts(UInt_t uFebIdx, UInt_t uAsicInFeb) { fhStsFebAsicHitCounts->Fill(uFebIdx, uAsicInFeb); }
  void FillStsStatusMessType(UInt_t uAsicIdx, UShort_t usStatusField)
  {
    fhStsStatusMessType->Fill(uAsicIdx, usStatusField);
  }

  ///Fill general "per Feb" histogram vectors
  void FillStsFebChanCntRaw(UInt_t uFebIdx, UInt_t uChanInFeb) { fvhStsFebChanCntRaw[uFebIdx]->Fill(uChanInFeb); }
  void FillStsFebChanAdcRaw(UInt_t uFebIdx, UInt_t uChanInFeb, UShort_t usRawAdc)
  {
    fvhStsFebChanAdcRaw[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  }
  void FillStsFebChanAdcRawProf(UInt_t uFebIdx, UInt_t uChanInFeb, UShort_t usRawAdc)
  {
    fvhStsFebChanAdcRawProf[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  }
  void FillStsFebChanAdcCal(UInt_t uFebIdx, UInt_t uChanInFeb, Double_t dCalAdc)
  {
    fvhStsFebChanAdcCal[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  }
  void FillStsFebChanAdcCalProf(UInt_t uFebIdx, UInt_t uChanInFeb, Double_t dCalAdc)
  {
    fvhStsFebChanAdcCalProf[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  }
  void FillStsFebChanRawTs(UInt_t uFebIdx, UInt_t uChan, UShort_t usRawTs)
  {
    fvhStsFebChanRawTs[uFebIdx]->Fill(uChan, usRawTs);
  }
  void FillStsFebChanMissEvt(UInt_t uFebIdx, UInt_t uChan, bool missEvtFlag)
  {
    fvhStsFebChanMissEvt[uFebIdx]->Fill(uChan, missEvtFlag);
  }
  void FillStsFebChanMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uChanInFeb)
  {
    fvhStsFebChanMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
  }
  void FillStsFebAsicMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uAsicInFeb)
  {
    fvhStsFebAsicMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
  }
  void FillStsFebMissEvtEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec)
  {
    fvhStsFebMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  }
  void FillStsFebChanHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uChanInFeb)
  {
    fvhStsFebChanHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
  }
  void FillStsFebAsicHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec, UInt_t uAsicInFeb)
  {
    fvhStsFebAsicHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
  }
  void FillStsFebHitRateEvo(UInt_t uFebIdx, Double_t dTimeSinceStartSec)
  {
    fvhStsFebHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  }
  void FillStsFebChanHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin, UInt_t uChanInFeb)
  {
    fvhStsFebChanHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, uChanInFeb, 1.0 / 60.0);
  }
  void FillStsFebAsicHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin, UInt_t uAsicInFeb)
  {
    fvhStsFebAsicHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, uAsicInFeb, 1.0 / 60.0);
  }
  void FillStsFebHitRateEvoLong(UInt_t uFebIdx, Double_t dTimeSinceStartMin)
  {
    fvhStsFebHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, 1.0 / 60.0);
  }

  ///Fill debugging histograms
  void FillStsMessType(uint16_t typeMess) { fhStsMessType->Fill(typeMess); }
  void FillStsMessTypePerDpb(UInt_t DpbIdx, uint16_t typeMess) { fhStsMessTypePerDpb->Fill(DpbIdx, typeMess); }
  void FillStsMessTypePerElink(UInt_t ElinkIdx, uint16_t typeMess) { fhStsMessTypePerElink->Fill(ElinkIdx, typeMess); }
  void FillStsHitsElinkPerDpb(UInt_t DpbIdx, UInt_t ElinkIdx) { fhStsHitsElinkPerDpb->Fill(DpbIdx, ElinkIdx); }
  void FillStsDpbRawTsMsb(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhStsDpbRawTsMsb->Fill(uCurrDpbIdx, ulCurrentTsMsb);
  }
  void FillStsDpbRawTsMsbSx(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhStsDpbRawTsMsbSx->Fill(uCurrDpbIdx, (ulCurrentTsMsb & 0x1F));
  }
  void FillStsDpbRawTsMsbDpb(UInt_t uCurrDpbIdx, ULong_t ulCurrentTsMsb)
  {
    fhStsDpbRawTsMsbDpb->Fill(uCurrDpbIdx, (ulCurrentTsMsb >> 5));
  }

  ///Fill debugging "per Asic" histogram vectors
  void FillStsChanCntRaw(UInt_t uAsicIdx, UShort_t usChan) { fvhStsChanCntRaw[uAsicIdx]->Fill(usChan); }
  void FillStsChanAdcRaw(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawAdc)
  {
    fvhStsChanAdcRaw[uAsicIdx]->Fill(usChan, usRawAdc);
  }
  void FillStsChanAdcRawProf(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawAdc)
  {
    fvhStsChanAdcRawProf[uAsicIdx]->Fill(usChan, usRawAdc);
  }
  void FillStsChanRawTs(UInt_t uAsicIdx, UShort_t usChan, UShort_t usRawTs)
  {
    fvhStsChanRawTs[uAsicIdx]->Fill(usChan, usRawTs);
  }
  void FillStsChanMissEvt(UInt_t uAsicIdx, UShort_t usChan, bool missedEvtFlag)
  {
    fvhStsChanMissEvt[uAsicIdx]->Fill(usChan, missedEvtFlag);
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

  /** @brief Activate the debug mode */
  bool GetDebugMode() { return fDebugMode; }

  void ProcessDebugInfo(const stsxyter::Message& Mess, const UInt_t& uCurrDpbIdx);

  /** @brief Activate the debug mode */
  void SetDebugMode(bool value) { fDebugMode = value; }

private:
  TString fHistoFileName = "HistosUnpackerSts.root";

  /// Rate evolution histos
  //Bool_t fbLongHistoEnable;
  UInt_t fuLongHistoNbSeconds  = 3600;
  UInt_t fuLongHistoBinSizeSec = 10;
  UInt_t fuLongHistoBinNb;

  /// Canvases
  std::vector<CbmQaCanvas*> fvcStsSumm;
  std::vector<CbmQaCanvas*> fvcStsSmxErr;

  ///General histograms
  TH1* fhDigisTimeInRun        = nullptr;
  TH1* fhVectorSize            = nullptr;
  TH1* fhVectorCapacity        = nullptr;
  TH1* fhMsCntEvo              = nullptr;
  TH2* fhMsErrorsEvo           = nullptr;
  TH2* fhStsAllFebsHitRateEvo  = nullptr;
  TH2* fhStsAllAsicsHitRateEvo = nullptr;
  TH2* fhStsFebAsicHitCounts   = nullptr;
  TH2* fhStsStatusMessType     = nullptr;

  ///General "per Feb" histogram vectors
  std::vector<TH1*> fvhStsFebChanCntRaw;
  std::vector<TH2*> fvhStsFebChanAdcRaw;
  std::vector<TProfile*> fvhStsFebChanAdcRawProf;
  std::vector<TH2*> fvhStsFebChanAdcCal;
  std::vector<TProfile*> fvhStsFebChanAdcCalProf;
  std::vector<TH2*> fvhStsFebChanRawTs;
  std::vector<TH2*> fvhStsFebChanMissEvt;
  std::vector<TH2*> fvhStsFebChanMissEvtEvo;
  std::vector<TH2*> fvhStsFebAsicMissEvtEvo;
  std::vector<TH1*> fvhStsFebMissEvtEvo;
  std::vector<TH2*> fvhStsFebChanHitRateEvo;
  std::vector<TH2*> fvhStsFebAsicHitRateEvo;
  std::vector<TH1*> fvhStsFebHitRateEvo;
  std::vector<TH2*> fvhStsFebChanHitRateEvoLong;
  std::vector<TH2*> fvhStsFebAsicHitRateEvoLong;
  std::vector<TH1*> fvhStsFebHitRateEvoLong;

  /** @brief Flag if debug mode is active or not */
  bool fDebugMode = false;

  ///Debugging histograms
  TH1* fhStsMessType         = nullptr;
  TH2* fhStsMessTypePerDpb   = nullptr;
  TH2* fhStsMessTypePerElink = nullptr;
  TH2* fhStsHitsElinkPerDpb  = nullptr;
  TH2* fhStsDpbRawTsMsb      = nullptr;
  TH2* fhStsDpbRawTsMsbSx    = nullptr;
  TH2* fhStsDpbRawTsMsbDpb   = nullptr;

  static const UInt_t kiMaxNbFlibLinks = 32;
  TH1* fvhMsSize[kiMaxNbFlibLinks];
  TProfile* fvhMsSizeTime[kiMaxNbFlibLinks];

  ///Debugging "per Asic" histogram vectors
  std::vector<TH1*> fvhStsChanCntRaw;
  std::vector<TH2*> fvhStsChanAdcRaw;
  std::vector<TProfile*> fvhStsChanAdcRawProf;
  std::vector<TH2*> fvhStsChanRawTs;
  std::vector<TH2*> fvhStsChanMissEvt;

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

  CbmStsUnpackMonitor(const CbmStsUnpackMonitor&);
  CbmStsUnpackMonitor operator=(const CbmStsUnpackMonitor&);

  ClassDef(CbmStsUnpackMonitor, 1)
};

#endif
