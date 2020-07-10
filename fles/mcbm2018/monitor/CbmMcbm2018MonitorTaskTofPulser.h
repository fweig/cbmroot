// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorTaskTofPulser                  -----
// -----               Created 27.11.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorTaskTofPulser_H
#define CbmMcbm2018MonitorTaskTofPulser_H

#include "Timeslice.hpp"

#include "CbmMcbmUnpack.h"

#include "TString.h"

class CbmMcbm2018MonitorAlgoTofPulser;

class CbmMcbm2018MonitorTaskTofPulser : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorTaskTofPulser();
  CbmMcbm2018MonitorTaskTofPulser(const CbmMcbm2018MonitorTaskTofPulser&) =
    delete;
  CbmMcbm2018MonitorTaskTofPulser
  operator=(const CbmMcbm2018MonitorTaskTofPulser&) = delete;
  virtual ~CbmMcbm2018MonitorTaskTofPulser();

  virtual Bool_t Init();
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  /// Temp until we change from CbmMcbmUnpack to something else
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};

  /// Algo settings setters
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  inline void SetHistoFilename(TString sNameIn) { fsHistoFileName = sNameIn; }
  inline void SetUpdateFreqTs(UInt_t uFreq = 100) { fuUpdateFreqTs = uFreq; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax) {
    fuPulserMinTot = uMin;
    fuPulserMaxTot = uMax;
  }
  inline void SetPulserChannel(UInt_t uChan) { fuPulserChannel = uChan; }
  inline void SetGdpbIndex(Int_t iGdpb = -1) { fiGdpbIndex = iGdpb; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) {
    fuHistoryHistoSize = inHistorySizeSec;
  }

private:
  Bool_t SaveHistograms();

  /// Control flags

  /// User settings parameters
  TString fsHistoFileName;
  UInt_t fuUpdateFreqTs;
  UInt_t fuPulserMinTot;
  UInt_t fuPulserMaxTot;
  UInt_t fuPulserChannel;
  Int_t fiGdpbIndex;
  UInt_t fuHistoryHistoSize;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmMcbm2018MonitorAlgoTofPulser* fMonitorPulserAlgo;

  ClassDef(CbmMcbm2018MonitorTaskTofPulser, 1)
};

#endif  // CbmMcbm2018MonitorTaskTofPulser_H
