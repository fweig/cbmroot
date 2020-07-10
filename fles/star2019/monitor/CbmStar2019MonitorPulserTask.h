// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmStar2019MonitorPulserTask                    -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019MonitorPulserTask_H
#define CbmStar2019MonitorPulserTask_H

#include "Timeslice.hpp"

#include "CbmMcbmUnpack.h"

#include "TString.h"

class CbmStar2019MonitorPulserAlgo;
class CbmTbDaqBuffer;
class TClonesArray;

class CbmStar2019MonitorPulserTask : public CbmMcbmUnpack {
public:
  CbmStar2019MonitorPulserTask();
  virtual ~CbmStar2019MonitorPulserTask();

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
  inline void SetEtofFeeIndexing(Bool_t bFlagIn = kTRUE) {
    fbEtofFeeIndexing = bFlagIn;
  }
  inline void SetUpdateFreqTs(UInt_t uFreq = 100) { fuUpdateFreqTs = uFreq; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax) {
    fuPulserMinTot = uMin;
    fuPulserMaxTot = uMax;
  }
  inline void SetPulserChannel(UInt_t uChan) { fuPulserChannel = uChan; }
  inline void SetSectorIndex(Int_t iSector = -1) { fiSectorIndex = iSector; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) {
    fuHistoryHistoSize = inHistorySizeSec;
  }

private:
  Bool_t SaveHistograms();

  /// Control flags
  Bool_t fbEtofFeeIndexing;

  /// User settings parameters
  TString fsHistoFileName;
  UInt_t fuUpdateFreqTs;
  UInt_t fuPulserMinTot;
  UInt_t fuPulserMaxTot;
  UInt_t fuPulserChannel;
  Int_t fiSectorIndex;
  UInt_t fuHistoryHistoSize;

  /// Parameters management
  TList* fParCList;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmStar2019MonitorPulserAlgo* fMonitorPulserAlgo;

  CbmStar2019MonitorPulserTask(const CbmStar2019MonitorPulserTask&);
  CbmStar2019MonitorPulserTask operator=(const CbmStar2019MonitorPulserTask&);

  ClassDef(CbmStar2019MonitorPulserTask, 1)
};

#endif  // CbmStar2019MonitorPulserTask_H
