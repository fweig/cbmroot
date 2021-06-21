/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerAlgoPsd                    -----
// -----                 Created 09.10.2019 by N.Karpushkin                -----
// -----        based on CbmMcbm2018UnpackerAlgoTof by P.-A. Loizeau       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerAlgoPsd_H
#define CbmMcbm2018UnpackerAlgoPsd_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmPsdDigi.h"

#include "PronyFitter.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"

// CbmRoot

// C++11
#include <chrono>
#include <numeric>

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018PsdPar;

class CbmMcbm2018UnpackerAlgoPsd : public CbmStar2019Algo<CbmPsdDigi> {
public:
  CbmMcbm2018UnpackerAlgoPsd();
  ~CbmMcbm2018UnpackerAlgoPsd();

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  Bool_t ProcessTs(const fles::Timeslice& ts);
  Bool_t ProcessTs(const fles::Timeslice& ts, size_t /*component*/) { return ProcessTs(ts); }
  Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  Bool_t CreateHistograms();
  Bool_t FillHistograms();
  Bool_t ResetHistograms();

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetTimeOffsetNs(Double_t dOffsetIn = 0.0) { fdTimeOffsetNs = dOffsetIn; }
  inline void SetDiamondDpbIdx(UInt_t uIdx = 2) { fuDiamondDpbIdx = uIdx; }


private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents;

  /// Settings from parameter file
  CbmMcbm2018PsdPar* fUnpackPar;             //!
  UInt_t fuRawDataVersion;                   //! Raw data versioning
                                             /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs;                        //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb;                   //! Number of FEBs per GDPB
  UInt_t fuNrOfChannelsPerFee;               //! Number of channels in each FEE
  UInt_t fuNrOfChannelsPerGdpb;              //! Number of channels per GDPB

  /// Detector Mapping
  UInt_t fuNrOfGbtx;
  UInt_t fuNrOfModules;
  std::vector<Int_t> fviNrOfRpc;
  std::vector<Int_t> fviRpcType;
  std::vector<Int_t> fviRpcSide;
  std::vector<Int_t> fviModuleId;
  std::vector<Int_t> fviPsdChUId;

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs;
  UInt_t fuDiamondDpbIdx;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuDetMask         = 0x0001FFFF;

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;  //! Idx of the current TS
  ULong64_t fulCurrentMsIdx;  //! Idx of the current MS in TS (0 to fuTotalMsNb)
  size_t fuCurrentMsSysId;    //! SysId of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdTsStartTime;     //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;  //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime;          //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;           //! Index of current MS within the TS

  /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  Int_t fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;     //! Bin size in s for the plots with date as X axis

  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch;       //! Current epoch index, per DPB
  std::vector<ULong64_t> fvulCurrentEpochCycle;  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull;   //! Epoch + Epoch Cycle

  /// Starting state book-keeping
  Double_t fdStartTime;     /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  CbmMcbm2018UnpackerAlgoPsd(const CbmMcbm2018UnpackerAlgoPsd&);
  CbmMcbm2018UnpackerAlgoPsd operator=(const CbmMcbm2018UnpackerAlgoPsd&);

  ClassDef(CbmMcbm2018UnpackerAlgoPsd, 1)
};

#endif
