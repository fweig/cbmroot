/* Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018MonitorTaskT0                      -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorTaskT0_H
#define CbmMcbm2018MonitorTaskT0_H

#include "CbmMcbm2018MonitorAlgoT0.h"
#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

#include "TString.h"

class CbmMcbm2018MonitorTaskT0 : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorTaskT0();
  CbmMcbm2018MonitorTaskT0(const CbmMcbm2018MonitorTaskT0&) = delete;
  CbmMcbm2018MonitorTaskT0 operator=(const CbmMcbm2018MonitorTaskT0&) = delete;

  virtual ~CbmMcbm2018MonitorTaskT0();

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
  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetHistoFilename(TString sNameIn) { fsHistoFileName = sNameIn; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }
  inline void SetSpillThreshold(UInt_t uCntLimit) { fuOffSpillCountLimit = uCntLimit; }
  inline void SetSpillThresholdNonPulser(UInt_t uCntLimit) { fuOffSpillCountLimitNonPulser = uCntLimit; }
  inline void SetSpillCheckInterval(Double_t dIntervalSec) { fdSpillCheckInterval = dIntervalSec; }
  void SetChannelMap(UInt_t uChan0, UInt_t uChan1, UInt_t uChan2, UInt_t uChan3, UInt_t uChan4, UInt_t uChan5,
                     UInt_t uChan6, UInt_t uChan7);

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms

  /// User settings parameters
  UInt_t fuHistoryHistoSize;
  TString fsHistoFileName;
  UInt_t fuMinTotPulser;
  UInt_t fuMaxTotPulser;
  UInt_t fuOffSpillCountLimit;
  UInt_t fuOffSpillCountLimitNonPulser;
  Double_t fdSpillCheckInterval;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmMcbm2018MonitorAlgoT0* fMonitorAlgo;

  ClassDef(CbmMcbm2018MonitorTaskT0, 1)
};

#endif
