/* Copyright (C) 2018-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmMcbm2018Source                     -----
// -----                    Created 19.01.2018 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018SOURCE_H
#define CBMMCBM2018SOURCE_H

#include "CbmDefs.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"
#include "TimesliceSource.hpp"

#include "FairSource.h"

#include "TList.h"
#include "TObjString.h"
#include "TStopwatch.h"
#include "TString.h"

#include <map>
#include <memory>
#include <unordered_set>

class CbmMcbmUnpack;

class TH1I;
class TProfile;
class TClonesArray;

class CbmMcbm2018Source : public FairSource {
public:
  CbmMcbm2018Source();

  CbmMcbm2018Source(const CbmMcbm2018Source& source) = delete;
  CbmMcbm2018Source& operator=(const CbmMcbm2018Source&) = delete;

  virtual ~CbmMcbm2018Source();

  Bool_t Init();
  Int_t ReadEvent(UInt_t);

  Source_Type GetSourceType() { return kONLINE; }

  virtual void SetParUnpackers();

  virtual Bool_t InitUnpackers();

  virtual Bool_t ReInitUnpackers();

  virtual Bool_t SpecifyRunId() { return true; };

  void Close();
  void Reset();

  void SetFileName(TString name)
  {
    fFileName = name;
    fInputFileList.Add(new TObjString(name));
  }
  void SetInputDir(TString name) { fDirName = name; }
  void SetHostName(TString name) { fHost = name; }

  void AddUnpacker(CbmMcbmUnpack* unpacker, Int_t flibId, ECbmModuleId /*detId*/)
  {
    fUnpackers.insert(std::pair<Int_t, CbmMcbmUnpack*>(flibId, unpacker));
  }

  void AddFile(const char* name) { fInputFileList.Add(new TObjString(name)); }

  UInt_t GetTsCount() { return fTSCounter; }
  UInt_t GetNofTSSinceLastTS() { return fNofTSSinceLastTS; }

  void SetDataReductionMuch(UInt_t uTsReduction = 15) { fuTsReduction = uTsReduction; }

  void UnpackSingleSpill(Int_t uSpillIdx, UInt_t uSpillStart = 1)
  {
    fiUnpSpillIdxStart = uSpillIdx;
    fiUnpSpillIdxStop  = uSpillIdx;
    fuFlagSpillStart   = uSpillStart;
  }

  void UnpackSelectSpills(Int_t uSpillIdxStart, Int_t uSpillIdxStop, UInt_t uSpillStart = 1)
  {
    fiUnpSpillIdxStart = uSpillIdxStart;
    fiUnpSpillIdxStop  = uSpillIdxStop;
    fuFlagSpillStart   = uSpillStart;
  }

  void LoadTsListSpillBreakBegin(std::vector<ULong64_t> vTsBeg)
  {
    fvuSpillBreakBegTs.assign(vTsBeg.begin(), vTsBeg.end());
  }
  void LoadTsListSpillBreakEnd(std::vector<ULong64_t> vTsEnd)
  {
    fvuSpillBreakEndTs.assign(vTsEnd.begin(), vTsEnd.end());
  }
  void LoadTsListSpillBreakMiddle(std::vector<ULong64_t> vTsMid)
  {
    fvuSpillBreakMidTs.assign(vTsMid.begin(), vTsMid.end());
  }

  void SetSubscriberHwm(UInt_t val = 1) { fuSubscriberHwm = val; }

  void SetWriteOutputFlag(Bool_t bFlagIn) { fbWriteOutput = bFlagIn; }

private:
  TString fFileName;
  TString fDirName;
  TList fInputFileList;  ///< List of input files
  TString fHost;

  std::multimap<Int_t, CbmMcbmUnpack*>
    fUnpackers;  //! List pairs of system ID and unpacker pointer (unpacker can appear multiple times)
  std::unordered_set<CbmMcbmUnpack*>
    fUnpackersToRun;  //! List of all unpackers for which at least one matching container was found

  UInt_t fTSNumber;
  UInt_t fTSCounter;
  ULong64_t fTSLength         = 0;  // used for the creation of the TimesliceMetaData
  ULong64_t fTSOverlappLength = 0;  // used for the creation of the TimesliceMetaData
  TStopwatch fTimer;

  TH1I* fHistoMissedTS;         //!
  TProfile* fHistoMissedTSEvo;  //!
  Int_t fNofTSSinceLastTS;

  UInt_t fuTsReduction;

  Int_t fiUnpSpillIdxStart = -1;  //! >= 0 means unpack only from this spill
  Int_t fiUnpSpillIdxStop  = -1;  //! >= 0 means unpack only up to this spill (included)
  UInt_t fuFlagSpillStart  = 0;   //! 0 = Break begin, 1 = Break middle, 2 = Break end
  UInt_t fuSpillBegTs      = 0;   //!
  UInt_t fuSpillEndTs      = 0;   //!

  std::vector<ULong64_t> fvuSpillBreakBegTs = {};  //!
  std::vector<ULong64_t> fvuSpillBreakEndTs = {};  //!
  std::vector<ULong64_t> fvuSpillBreakMidTs = {};  //!

  std::unique_ptr<fles::TimesliceSource> fSource;  //!

  UInt_t fuSubscriberHwm;

  Bool_t CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);

  Int_t FillBuffer();

  /// Output ClonesArray
  Bool_t fbWriteOutput;  //! If ON the output TClonesArray of meta-data is written to disk
  TClonesArray* fTimeSliceMetaDataArray;

  ClassDef(CbmMcbm2018Source, 2)
};


#endif
