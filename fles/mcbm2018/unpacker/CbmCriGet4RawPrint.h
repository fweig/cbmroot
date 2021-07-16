/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CbmCriGet4RawPrint_H
#define CbmCriGet4RawPrint_H

// Data
#include "Timeslice.hpp"

#include "CriGet4Mess001.h"

// CbmRoot
#include "CbmMcbmUnpack.h"

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018TofPar;

class CbmCriGet4RawPrint : public CbmMcbmUnpack {
public:
  CbmCriGet4RawPrint();

  CbmCriGet4RawPrint(const CbmCriGet4RawPrint&) = delete;
  CbmCriGet4RawPrint operator=(const CbmCriGet4RawPrint&) = delete;

  virtual ~CbmCriGet4RawPrint();

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

  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE) { fbIgnoreOverlapMs = bFlagIn; }

private:
  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;

  /// Parameters related to FLES containers
  std::vector<size_t> fvMsComponentsList;  //!
  size_t fuNbCoreMsPerTs;                  //!
  size_t fuNbOverMsPerTs;                  //!
  size_t fuNbMsLoop;                       //!
  Bool_t fbIgnoreOverlapMs;                //! /** Ignore Overlap Ms: all fuNbOverMsPerTs MS at the end of timeslice **/
  Double_t fdMsSizeInNs;                   //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  Double_t fdTsStartTime;       //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;    //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime;            //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;             //! Index of current MS within the TS
                                /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating

  ClassDef(CbmCriGet4RawPrint, 1)
};

#endif
