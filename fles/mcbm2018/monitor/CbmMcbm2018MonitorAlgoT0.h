/* Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoT0                         -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorAlgoT0_H
#define CbmMcbm2018MonitorAlgoT0_H

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

class CbmMcbm2018MonitorAlgoT0 : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmMcbm2018MonitorAlgoT0();
  ~CbmMcbm2018MonitorAlgoT0();

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
  inline void SetSpillThresholdNonPulser(UInt_t uCntLimit) { fuOffSpillCountLimitNonPulser = uCntLimit; }
  inline void SetSpillCheckInterval(Double_t dIntervalSec) { fdSpillCheckInterval = dIntervalSec; }
  inline void SetChannelMap(UInt_t uChan0, UInt_t uChan1, UInt_t uChan2, UInt_t uChan3, UInt_t uChan4, UInt_t uChan5,
                            UInt_t uChan6, UInt_t uChan7)
  {
    fuDiamChanMap[0] = uChan0;
    fuDiamChanMap[1] = uChan1;
    fuDiamChanMap[2] = uChan2;
    fuDiamChanMap[3] = uChan3;
    fuDiamChanMap[4] = uChan4;
    fuDiamChanMap[5] = uChan5;
    fuDiamChanMap[6] = uChan6;
    fuDiamChanMap[7] = uChan7;
  }

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
  UInt_t fuMinTotPulser                = 90;
  UInt_t fuMaxTotPulser                = 100;
  UInt_t fuOffSpillCountLimit          = 200;
  UInt_t fuOffSpillCountLimitNonPulser = 80;
  Double_t fdSpillCheckInterval        = 1.0;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanDiamond   = 8;

  /// Running indices
  /// TS/MS info
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
  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch      = {};  //! Current epoch index, per DPB
  std::vector<ULong64_t> fvulCurrentEpochCycle = {};  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull  = {};  //! Epoch + Epoch Cycle

  /// Starting state book-keeping
  Double_t fdStartTime = -1.0; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz = 0.0; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point ftStartTimeUnix = std::chrono::steady_clock::
    now(); /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Buffers
  std::vector<std::vector<gdpbv100::Message>> fvvmEpSupprBuffer = {};  //! [DPB]
  std::vector<gdpbv100::FullMessage> fvmHitsInMs =
    {};  //! All hits (time in bins, TOT in bins, asic, channel) in last MS, sorted with "<" operator

  /// Spill detection
  Bool_t fbSpillOn                  = kTRUE;
  UInt_t fuCurrentSpillIdx          = 0;
  UInt_t fuCurrentSpillPlot         = 0;
  Double_t fdStartTimeSpill         = -1.0;
  Double_t fdLastInterTime          = -1.0;
  UInt_t fuCountsLastInter          = 0;
  UInt_t fuNonPulserCountsLastInter = 0;


  /// Histograms related variables
  UInt_t fuHistoryHistoSize = 3600; /** Size in seconds of the evolution histograms **/

  /// Histograms
  /// Channel rate plots
  std::vector<UInt_t> fvuHitCntChanMs           = std::vector<UInt_t>(kuNbChanDiamond, 0);
  std::vector<UInt_t> fvuErrorCntChanMs         = std::vector<UInt_t>(kuNbChanDiamond, 0);
  std::vector<UInt_t> fvuEvtLostCntChanMs       = std::vector<UInt_t>(kuNbChanDiamond, 0);
  std::vector<TH1*> fvhMsgCntEvoChan            = std::vector<TH1*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhMsgCntPerMsEvoChan       = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  std::vector<TH1*> fvhHitCntEvoChan            = std::vector<TH1*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhHitCntPerMsEvoChan       = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  std::vector<TH1*> fvhErrorCntEvoChan          = std::vector<TH1*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhErrorCntPerMsEvoChan     = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  std::vector<TH1*> fvhEvtLostCntEvoChan        = std::vector<TH1*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhEvtLostCntPerMsEvoChan   = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  std::vector<TProfile*> fvhErrorFractEvoChan   = std::vector<TProfile*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhErrorFractPerMsEvoChan   = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  std::vector<TProfile*> fvhEvtLostFractEvoChan = std::vector<TProfile*>(kuNbChanDiamond, nullptr);
  std::vector<TH2*> fvhEvtLostFractPerMsEvoChan = std::vector<TH2*>(kuNbChanDiamond, nullptr);
  /// Channels map
  static const UInt_t kuNbSpillPlots = 5;
  //      UInt_t  kuDiamChanMap[ kuNbChanDiamond ] = { 2, 3, 4, 5, 0, 1, 6, 7 }; //! Map from electronics channel to Diamond strip
  UInt_t fuDiamChanMap[kuNbChanDiamond] = {0, 1, 2, 3, 4, 5, 6, 7};  //! Map from electronics channel to Diamond strip
  TH1* fhDpbMap                         = nullptr;
  TH1* fhChannelMap                     = nullptr;
  TH2* fhHitMapEvo                      = nullptr;
  TH2* fhHitTotEvo                      = nullptr;
  TH1* fhChanHitMap                     = nullptr;
  TH2* fhChanHitMapEvo                  = nullptr;
  std::vector<TH1*> fvhDpbMapSpill      = {};
  std::vector<TH1*> fvhChannelMapSpill  = {};
  TH1* fhHitsPerSpill                   = nullptr;
  /// Global Rate
  TH1* fhMsgCntEvo            = nullptr;
  TH1* fhHitCntEvo            = nullptr;
  TH1* fhErrorCntEvo          = nullptr;
  TH1* fhLostEvtCntEvo        = nullptr;
  TProfile* fhErrorFractEvo   = nullptr;
  TProfile* fhLostEvtFractEvo = nullptr;

  TH2* fhMsgCntPerMsEvo       = nullptr;
  TH2* fhHitCntPerMsEvo       = nullptr;
  TH2* fhErrorCntPerMsEvo     = nullptr;
  TH2* fhLostEvtCntPerMsEvo   = nullptr;
  TH2* fhErrorFractPerMsEvo   = nullptr;
  TH2* fhLostEvtFractPerMsEvo = nullptr;

  /// Pulser
  TH1* fhChannelMapPulser = nullptr;
  TH2* fhHitMapEvoPulser  = nullptr;

  /// Canvases
  TCanvas* fcSummary            = nullptr;
  TCanvas* fcSummaryMap         = nullptr;
  TCanvas* fcHitMaps            = nullptr;
  TCanvas* fcGenCntsPerMs       = nullptr;
  TCanvas* fcSpillCounts        = nullptr;
  TCanvas* fcSpillCountsHori    = nullptr;
  TCanvas* fcSpillDpbCountsHori = nullptr;

  /*
      void ProcessEpochCycle( uint64_t ulCycleData );
      void ProcessEpoch(       gdpbv100::Message mess );

      void ProcessEpSupprBuffer();

      void ProcessHit(     gdpbv100::FullMessage mess );
      void ProcessSysMess( gdpbv100::FullMessage mess );

      void ProcessError(   gdpbv100::FullMessage mess );

      inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
      {
         return gdpbId * fuNrOfGet4PerGdpb + get4Id;
      }
*/
  CbmMcbm2018MonitorAlgoT0(const CbmMcbm2018MonitorAlgoT0&);
  CbmMcbm2018MonitorAlgoT0 operator=(const CbmMcbm2018MonitorAlgoT0&);

  ClassDef(CbmMcbm2018MonitorAlgoT0, 1)
};

#endif
