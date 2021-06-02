/** @file CbmMcbmSpillFindAlgo.h
  * @copyright Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbmSpillFindAlgo                         -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbmSpillFindAlgo_H
#define CbmMcbmSpillFindAlgo_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmTofDigi.h"

#include "gDpbMessv100.h"

// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018TofPar;
/*
class TCanvas;
class THttpServer;
*/
class TH1;
class TH2;
class TProfile;

class CbmMcbmSpillFindAlgo : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmMcbmSpillFindAlgo();
  ~CbmMcbmSpillFindAlgo();

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
  Bool_t ResetHistograms(Bool_t bResetTime = kTRUE);

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }
  inline void SetSpillThreshold(UInt_t uCntLimit) { fuOffSpillCountLimit = uCntLimit; }
  inline void SetSpillCheckIntervalSec(Double_t dInterval) { fdSpillCheckInterval = dInterval; }

private:
  /// Control flags
  Bool_t fbMonitorMode                    = kFALSE;  //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode               = kFALSE;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents = {};

  /// Settings from parameter file
  CbmMcbm2018TofPar* fUnpackPar = nullptr;  //!
    /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs                       = 0;   //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap = {};  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb                  = 0;   //! Number of FEBs per GDPB
  UInt_t fuNrOfGet4PerFee                  = 0;   //! Number of GET4s per FEE
  UInt_t fuNrOfChannelsPerGet4             = 0;   //! Number of channels in each GET4
  UInt_t fuNrOfChannelsPerFee              = 0;   //! Number of channels in each FEE
  UInt_t fuNrOfGet4                        = 0;   //! Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb                 = 0;   //! Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb             = 0;   //! Number of channels per GDPB

  /// User settings: Data correction parameters
  UInt_t fuMinTotPulser         = 90;
  UInt_t fuMaxTotPulser         = 100;
  UInt_t fuOffSpillCountLimit   = 100;
  Double_t fdSpillCheckInterval = 0.5;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanDiamond   = 8;

  /// Running indices
  /// TS/MS info
  ULong64_t fulFirstTsIdx   = 9999999999999;  //! First TS index, forward point set ~30 years...
  ULong64_t fulCurrentTsIdx = 0;
  ULong64_t fulCurrentMsIdx = 0;
  Double_t fdTsStartTime    = -1.0;  //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore =
    -1.0;                    //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime = -1.0;  //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex  = 0;     //! Index of current MS within the TS
                             /// Current data properties
  std::map<gdpbv100::MessageTypes, UInt_t> fmMsgCounter = {};
  UInt_t fuCurrentEquipmentId = 0;   //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId          = 0;   //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx         = 0;   //! Index of the DPB from which the MS currently unpacked is coming
  Int_t fiRunStartDateTimeSec = -1;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots    = -1;  //! Bin size in s for the plots with date as X axis
  UInt_t fuGet4Id =
    0;  //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr = 0;  //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message

  /// Starting state book-keeping
  Double_t fdStartTime     = -1.0;  //! Time of first valid hit (epoch available), used as reference for evolution plots
  Double_t fdStartTimeMsSz = 0.0;   //! Time of first microslice, used as reference for evolution plots
  std::chrono::steady_clock::time_point ftStartTimeUnix = std::chrono::steady_clock::
    now();  //! Time of run Start from UNIX system, used as reference for long evolution plots against reception time

  /// Spill detection
  Bool_t fbSpillOn                          = kTRUE;
  UInt_t fuCurrentSpillIdx                  = 0;
  Double_t fdStartTimeSpill                 = -1.0;
  Double_t fdLastSecondTime                 = -1.0;
  UInt_t fuCountsLastInterval               = 0;
  UInt_t fuCountsLastSpill                  = 0;
  std::vector<ULong64_t> fvuSpillBreakBegTs = {};
  std::vector<ULong64_t> fvuSpillBreakEndTs = {};
  std::vector<ULong64_t> fvuSpillBreakMidTs = {};

  /// Histograms related variables
  UInt_t fuHistoryHistoSize = 3600; /** Size in seconds of the evolution histograms **/

  /// Histograms
  TH1* fhHitsEvo            = nullptr;
  TH1* fhHitsPerSpill       = nullptr;
  TH1* fhSpillBreakDuration = nullptr;
  TH1* fhSpillDuration      = nullptr;

  /// Canvases
  /*
  TCanvas* fcSummary            = nullptr;
  TCanvas* fcHitMaps            = nullptr;
  TCanvas* fcGenCntsPerMs       = nullptr;
  TCanvas* fcSpillCounts        = nullptr;
  TCanvas* fcSpillCountsHori    = nullptr;
  TCanvas* fcSpillDpbCountsHori = nullptr;
*/

  CbmMcbmSpillFindAlgo(const CbmMcbmSpillFindAlgo&);
  CbmMcbmSpillFindAlgo operator=(const CbmMcbmSpillFindAlgo&);

  ClassDef(CbmMcbmSpillFindAlgo, 1)
};

#endif
