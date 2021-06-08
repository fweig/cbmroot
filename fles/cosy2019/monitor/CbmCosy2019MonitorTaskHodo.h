/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: P.-A. Loizeau, Florian Uhlig [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2019MonitorTaskHodo                    -----
// -----               Created 03.07.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmCosy2019MonitorTaskHodo_H
#define CbmCosy2019MonitorTaskHodo_H

#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

class CbmCosy2019MonitorAlgoHodo;

class CbmCosy2019MonitorTaskHodo : public CbmMcbmUnpack {
public:
  CbmCosy2019MonitorTaskHodo(UInt_t uNbSdpb = 1);

  CbmCosy2019MonitorTaskHodo(const CbmCosy2019MonitorTaskHodo&) = delete;
  CbmCosy2019MonitorTaskHodo operator=(const CbmCosy2019MonitorTaskHodo&) = delete;

  virtual ~CbmCosy2019MonitorTaskHodo();

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

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  void SetDpbId(UInt_t uDpbId = 0x5b75);
  void SetHodoElinkIdx(UInt_t uElinkHodoA = 5, UInt_t uElinkHodoB = 10);
  void SetHodoSwapXY(Bool_t bSwapHodoA = kFALSE, Bool_t bSwapHodoB = kTRUE);
  void SetHodoInvertX(Bool_t bInvHodoA = kFALSE, Bool_t bInvHodoB = kTRUE);
  void SetHodoInvertY(Bool_t bInvHodoA = kFALSE, Bool_t bInvHodoB = kTRUE);

  void SetTimeOffsetNs(Double_t dOffsetIn = 0.0);

  /// Task settings
  void SetWriteOutputFlag(Bool_t bFlagIn) { fbWriteOutput = bFlagIn; }

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  Bool_t fbWriteOutput;       //! If ON the output TClonesArray of digi is written to disk

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmCosy2019MonitorAlgoHodo* fMonitorAlgo;

  Bool_t SaveHistograms();

  ClassDef(CbmCosy2019MonitorTaskHodo, 1)
};

#endif
