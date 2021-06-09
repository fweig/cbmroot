/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorTaskPsd                        -----
// -----              Created 26.09.2019 by N.Karpushkin                   -----
// -----      based on CbmMcbm2018MonitorTaskT0 by P.-A. Loizeau           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorTaskPsd_H
#define CbmMcbm2018MonitorTaskPsd_H

#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

#include "TString.h"

class CbmMcbm2018MonitorAlgoPsd;

class CbmMcbm2018MonitorTaskPsd : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorTaskPsd();
  virtual ~CbmMcbm2018MonitorTaskPsd();

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
  inline void SetMonitorChanMode(Bool_t bFlagIn = kTRUE) { fbMonitorChanMode = bFlagIn; }
  inline void SetMonitorWfmMode(Bool_t bFlagIn = kTRUE) { fbMonitorWfmMode = bFlagIn; }
  inline void SetMonitorFitMode(Bool_t bFlagIn = kTRUE) { fbMonitorFitMode = bFlagIn; }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetChargeHistoArgs(std::vector<Int_t> inVec) { fviHistoChargeArgs = inVec; }
  inline void SetAmplHistoArgs(std::vector<Int_t> inVec) { fviHistoAmplArgs = inVec; }
  inline void SetZLHistoArgs(std::vector<Int_t> inVec) { fviHistoZLArgs = inVec; }
  inline void SetHistoFilename(TString sNameIn) { fsHistoFileName = sNameIn; }

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbMonitorChanMode;   //! Switch ON the filling channelwise histograms
  Bool_t fbMonitorWfmMode;    //! Switch ON the filling waveforms histograms
  Bool_t fbMonitorFitMode;    //! Switch ON the filling waveform fitting histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms

  /// User settings parameters
  UInt_t fuHistoryHistoSize;
  std::vector<Int_t> fviHistoChargeArgs; /** Charge histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoAmplArgs;   /** Amplitude histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoZLArgs;     /** ZeroLevel histogram arguments in adc counts **/
  TString fsHistoFileName;

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Processing algo
  CbmMcbm2018MonitorAlgoPsd* fMonitorAlgo;

  CbmMcbm2018MonitorTaskPsd(const CbmMcbm2018MonitorTaskPsd&);
  CbmMcbm2018MonitorTaskPsd operator=(const CbmMcbm2018MonitorTaskPsd&);

  ClassDef(CbmMcbm2018MonitorTaskPsd, 1)
};

#endif
