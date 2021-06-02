/** @file CbmMcbm2018UnpackerTaskRich.h
  * @copyright Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Egor Ovcharenko [committer], Pierre-Alain Loizeau **/

/**
 * CbmMcbm2018UnpackerTaskRich
 * E. Ovcharenko, Mar 2019
 * based on other detectors' classes by P.-A. Loizeau
 */

#ifndef CbmMcbm2018UnpackerTaskRich_H
#define CbmMcbm2018UnpackerTaskRich_H

#include "CbmMcbmUnpack.h"  // mother class
#include "CbmRichDigi.h"

//class TList; // Needed?
//class TClonesArray;
class CbmMcbm2018UnpackerAlgoRich;

class CbmMcbm2018UnpackerTaskRich : public CbmMcbmUnpack {
public:
  CbmMcbm2018UnpackerTaskRich();

  CbmMcbm2018UnpackerTaskRich(const CbmMcbm2018UnpackerTaskRich&) = delete;
  CbmMcbm2018UnpackerTaskRich operator=(const CbmMcbm2018UnpackerTaskRich&) = delete;

  virtual ~CbmMcbm2018UnpackerTaskRich();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  virtual void SetParContainers();

  virtual Bool_t InitContainers();

  virtual Bool_t ReInitContainers();

  virtual void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);

  /// Algo settings setters
  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  void SetTimeOffsetNs(Double_t dOffsetIn = 0.0);
  void DoTotCorr(Bool_t bDoToTCorr = kTRUE);

  /// Task settings
  void SetWriteOutputFlag(Bool_t bFlagIn) { fbWriteOutput = bFlagIn; }

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  Bool_t fbWriteOutput;       //! If ON the output TClonesArray of digi is written to disk

  /// Statistics & first TS rejection
  uint64_t fulTsCounter;

  /// Algo implementation of the unpacking
  CbmMcbm2018UnpackerAlgoRich* fUnpackerAlgo;

  /// Output vectors
  std::vector<CbmRichDigi>* fpvDigiRich = nullptr;

  ClassDef(CbmMcbm2018UnpackerTaskRich, 1);
};

#endif  // CbmMcbm2018UnpackerTaskRich_H
