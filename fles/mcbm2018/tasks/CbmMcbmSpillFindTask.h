/* Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbmSpillFindTaskzz                      -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbmSpillFindTask_H
#define CbmMcbmSpillFindTask_H

#include "CbmMcbmSpillFindAlgo.h"
#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

#include "TString.h"

class CbmMcbmSpillFindTask : public CbmMcbmUnpack {
public:
  CbmMcbmSpillFindTask();
  CbmMcbmSpillFindTask(const CbmMcbmSpillFindTask&) = delete;
  CbmMcbmSpillFindTask operator=(const CbmMcbmSpillFindTask&) = delete;

  virtual ~CbmMcbmSpillFindTask();

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
  inline void SetSpillCheckIntervalSec(Double_t dInterval) { fdSpillCheckInterval = dInterval; }

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
  Double_t fdSpillCheckInterval = 0.5;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmMcbmSpillFindAlgo* fMonitorAlgo;

  ClassDef(CbmMcbmSpillFindTask, 1)
};

#endif
