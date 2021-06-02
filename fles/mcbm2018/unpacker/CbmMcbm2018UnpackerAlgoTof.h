/** @file CbmMcbm2018UnpackerAlgoTof.h
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018UnpackerAlgoTof                       -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerAlgoTof_H
#define CbmMcbm2018UnpackerAlgoTof_H

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

class CbmMcbm2018UnpackerAlgoTof : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmMcbm2018UnpackerAlgoTof();
  ~CbmMcbm2018UnpackerAlgoTof();

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

  inline void SetVectCapInc(Double_t dIncFact) { fdCapacityIncFactor = dIncFact; }
  inline void SetMaxAsicInactive(Int_t ival) { fMaxAsicInactive = ival; }

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents;

  /// Settings from parameter file
  CbmMcbm2018TofPar* fUnpackPar;             //!
                                             /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs;                        //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb;                   //! Number of FEBs per GDPB
  UInt_t fuNrOfGet4PerFee;                   //! Number of GET4s per FEE
  UInt_t fuNrOfChannelsPerGet4;              //! Number of channels in each GET4
  UInt_t fuNrOfChannelsPerFee;               //! Number of channels in each FEE
  UInt_t fuNrOfGet4;                         //! Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb;                  //! Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb;              //! Number of channels per GDPB
                                             /// Detector Mapping
  UInt_t fuNrOfGbtx;
  UInt_t fuNrOfModules;
  std::vector<Int_t> fviNrOfRpc;
  std::vector<Int_t> fviRpcType;
  std::vector<Int_t> fviRpcSide;
  std::vector<Int_t> fviModuleId;
  std::vector<Int_t> fviRpcChUId;

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs;
  UInt_t fuDiamondDpbIdx;

  /// Constants
  static const Int_t kiMaxNbFlibLinks = 32;
  static const UInt_t kuDetMask       = 0x0001FFFF;

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;    //! Idx of the current TS
  ULong64_t fulCurrentMsIdx;    //! Idx of the current MS in TS (0 to fuTotalMsNb)
  size_t fuCurrentMsSysId;      //! SysId of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdTsStartTime;       //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;    //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime;            //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;             //! Index of current MS within the TS
                                /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  UInt_t fuGet4Id;  //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch;       //! Current epoch index, per DPB
  std::vector<ULong64_t> fvulCurrentEpochCycle;  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull;   //! Epoch + Epoch Cycle

  /// Starting state book-keeping
  Double_t fdStartTime;     /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Buffers
  std::vector<std::vector<gdpbv100::Message>>
    fvvmEpSupprBuffer;  //! [DPB], FIXME: dimension to be removed as not needed with real MS

  /// STAR TRIGGER detection
  std::vector<ULong64_t> fvulGdpbTsMsb;
  std::vector<ULong64_t> fvulGdpbTsLsb;
  std::vector<ULong64_t> fvulStarTsMsb;
  std::vector<ULong64_t> fvulStarTsMid;
  std::vector<ULong64_t> fvulGdpbTsFullLast;
  std::vector<ULong64_t> fvulStarTsFullLast;
  std::vector<UInt_t> fvuStarTokenLast;
  std::vector<UInt_t> fvuStarDaqCmdLast;
  std::vector<UInt_t> fvuStarTrigCmdLast;

  /// Histograms
  Double_t fdRefTime;
  Double_t fdLastDigiTime;
  Double_t fdFirstDigiTimeDif;
  Double_t fdEvTime0;
  TH1* fhRawTDigEvT0;
  TH1* fhRawTDigRef0;
  TH1* fhRawTDigRef;
  TH1* fhRawTRefDig0;
  TH1* fhRawTRefDig1;
  TH1* fhRawDigiLastDigi;
  std::vector<TH2*> fhRawTotCh;
  std::vector<TH1*> fhChCount;
  std::vector<TH1*> fhChCountRemap;
  std::vector<Bool_t> fvbChanThere;
  std::vector<TH2*> fhChanCoinc;
  Bool_t fbDetChanThere[64];
  TH2* fhDetChanCoinc;
  /*
      std::vector< TH1* > fvhHitsTimeToTriggerRaw;       //! [sector]
      std::vector< TH1* > fvhMessDistributionInMs;       //! [sector], extra monitor for debug
      TH1 *               fhEventNbPerTs;                //!
      TCanvas *           fcTimeToTrigRaw;               //! All sectors
*/
  TH1* fhVectorSize            = nullptr;
  TH1* fhVectorCapacity        = nullptr;
  size_t fuTsMaxVectorSize     = 0;
  Double_t fdCapacityIncFactor = 1.1;
  Int_t fMaxAsicInactive       = 0;

  void ProcessEpochCycle(const uint64_t& ulCycleData);
  void ProcessEpoch(const gdpbv100::Message& mess, uint32_t uMesgIdx);
  void ProcessEndOfMsEpoch();
  void ProcessStarTrigger(const gdpbv100::Message& mess);

  void ProcessEpSupprBuffer();

  void ProcessHit(const gdpbv100::FullMessage& mess);
  void ProcessSlCtrl(const gdpbv100::FullMessage& mess);
  void ProcessSysMess(const gdpbv100::FullMessage& mess);

  void ProcessError(const gdpbv100::FullMessage& mess);
  void ProcessPattern(const gdpbv100::FullMessage& mess);

  inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) { return gdpbId * fuNrOfGet4PerGdpb + get4Id; }

  CbmMcbm2018UnpackerAlgoTof(const CbmMcbm2018UnpackerAlgoTof&);
  CbmMcbm2018UnpackerAlgoTof operator=(const CbmMcbm2018UnpackerAlgoTof&);

  ClassDef(CbmMcbm2018UnpackerAlgoTof, 1)
};

#endif
