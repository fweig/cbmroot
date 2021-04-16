// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMuchLite                        -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                Modified 07/12/18  by Ajit Kumar                 -----
// -----                Modified 05/03/19  by Vikas Singhal               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorTaskMuchLite_H
#define CbmMcbm2018MonitorTaskMuchLite_H

#ifndef __CINT__
#include "Timeslice.hpp"
#endif

// Data
#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot
#include "CbmHistManager.h"
#include "CbmMcbm2018MonitorAlgoMuchLite.h"
#include "CbmMcbmUnpack.h"

// C++11
#include <chrono>

// C/C++
#include <map>
#include <set>
#include <vector>

class CbmMcbm2018MuchPar;
//class CbmCern2017UnpackParSts;

class CbmMcbm2018MonitorTaskMuchLite : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorTaskMuchLite();
  //CbmMcbm2018MonitorTaskMuchLite(const CbmMcbm2018MonitorTaskMuchLite&) = delete;
  //CbmMcbm2018MonitorTaskMuchLite operator=(const CbmMcbm2018MonitorTaskMuchLite&) = delete;
  virtual ~CbmMcbm2018MonitorTaskMuchLite();


  virtual Bool_t Init();
  // #ifndef __CINT__
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  //#endif
  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};
  // Algo settings setters
  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  //void SetMsOverlap(size_t uOverlapMsNb = 1) { fuNbOverMsPerTs = uOverlapMsNb; }
  //size_t GetMsOverlap() { return fuNbOverMsPerTs; }

  //void SetRunStart(Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5);

  //void ResetAllHistos();
  //void SaveAllHistos(TString sFileName = "");
  //void SaveHistos(TString sFileName = "");
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetHistoFileName(TString sFileName = "data/SetupHistos.root") { fsHistoFilename = sFileName; }
  inline void SetSpillThreshold(UInt_t uCntLimit) { fuOffSpillCountLimit = uCntLimit; }

  void SetPrintMessage(Bool_t bPrintMessOn             = kTRUE,
                       stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex
                                                         | stsxyter::MessagePrintMask::msg_print_Human)
  {
    fbPrintMessages = bPrintMessOn;
    fPrintMessCtrl  = ctrl;
  }
  //void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );
  //void SetEnableCoincidenceMaps( Bool_t bEnableCoincidenceMapsOn = kTRUE ) { fbEnableCoincidenceMaps = bEnableCoincidenceMapsOn; }
  //void SetCoincidenceBorder( Double_t dCenterPos, Double_t dBorderVal );
  //void SetFebChanCoincidenceLimitNs( Double_t dLimitIn ) { fdFebChanCoincidenceLimit = dLimitIn; }
  //void UseNoiseLimitsSmx2LogicError( Bool_t bUseNoise = kTRUE ) { fbSmx2ErrorUseNoiseLevels = bUseNoise; }

  void SetMuchMode(Bool_t bMuchMode = kTRUE) { fbMuchMode = bMuchMode; }

  //   void SetTimeBin( size_t uTimeBin );
  // void UseDaqBuffer(
  // Bool_t) {};  //Virtual function in Mother Class, Need to update accordingly. VS

  /// => Quick and dirty hack for binning FW!!!
  void SetBinningFwFlag(Bool_t bEnable = kTRUE) { fbBinningFw = bEnable; }

private:
  // Parameters
  // Control flags
  Bool_t fbMonitorMode;
  Bool_t fbDebugMonitorMode;
  Bool_t fbMuchMode;

  /// => Quick and dirty hack for binning FW!!!
  Bool_t fbBinningFw = kFALSE;


  /// Histograms related variables
  UInt_t fuHistoryHistoSize = 3600; /** Size in seconds of the evolution histograms **/
  TString fsHistoFilename;
  UInt_t fuOffSpillCountLimit = 200;
  uint64_t fulTsCounter;
  Bool_t fbPrintMessages;
  //size_t fuNbOverMsPerTs;
  stsxyter::MessagePrintMask fPrintMessCtrl;

  //CbmMcbm2018MonitorTaskMuchLite(const CbmMcbm2018MonitorTaskMuchLite&);
  //CbmMcbm2018MonitorTaskMuchLite operator=(const CbmMcbm2018MonitorTaskMuchLite&);

  CbmMcbm2018MonitorAlgoMuchLite* fMonitorAlgo;

  ClassDef(CbmMcbm2018MonitorTaskMuchLite, 1)
};

#endif  // CbmMcbm2018MonitorTaskMuchLite_H
