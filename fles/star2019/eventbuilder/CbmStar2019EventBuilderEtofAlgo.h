/** @file CbmStar2019EventBuilderEtofAlgo.h
  * @copyright Copyright (C) 2018-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStar2019EventBuilderEtofAlgo                  -----
// -----               Created 03.11.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019EventBuilderEtofAlgo_H
#define CbmStar2019EventBuilderEtofAlgo_H

#include "CbmStar2019Algo.h"
#include "CbmTofDigi.h"
#include "CbmTofStarData2019.h"

#include <bitset>
#include <chrono>
#include <map>
#include <vector>

#include "gDpbMessv100.h"

class CbmStar2019TofPar;
/*
class TCanvas;
class THttpServer;
*/
class TH1;
class TH2;
class TProfile;

class CbmStar2019EventBuilderEtofAlgo : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmStar2019EventBuilderEtofAlgo();
  ~CbmStar2019EventBuilderEtofAlgo();

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

  Bool_t BuildEvents();
  std::vector<CbmTofStarSubevent2019>& GetEventBuffer() { return fvEventsBuffer; }

  Bool_t CreateHistograms();
  Bool_t FillHistograms();
  Bool_t ResetHistograms();

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetDebugMonitorMode(Bool_t bFlagIn = kTRUE) { fbDebugMonitorMode = bFlagIn; }
  inline void SetStoreLostEventMsg(Bool_t bFlagIn = kTRUE) { fbStoreLostEventMsg = bFlagIn; }
  inline void SetAddStatusToEvent(Bool_t bFlagIn = kTRUE) { fbAddStatusToEvent = bFlagIn; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  Bool_t
    fbStoreLostEventMsg;  //! Switch ON the insertion of the LostEvent messages from GET4s with the critical errors (default is false)
  Bool_t fbAddStatusToEvent;  //! Switch ON the readout and insertion of STATUS pattern message (default is true)

  /// Constants
  static const UInt_t kuMaxNbErrorsPerGdpbPerEvent =
    500;  //! Correspond to ~6000 error messages max per event, leaving 2000 for hits and epoch

  /// Settings from parameter file
  CbmStar2019TofPar* fUnpackPar;             //!
                                             /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs;                        // Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;  // gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb;                   // Number of FEBs per GDPB
  UInt_t fuNrOfGet4PerFee;                   // Number of GET4s per FEE
  UInt_t fuNrOfChannelsPerGet4;              // Number of channels in each GET4

  UInt_t fuNrOfChannelsPerFee;   // Number of channels in each FEE
  UInt_t fuNrOfGet4;             // Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb;      // Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb;  // Number of channels per GDPB
                                 /// Detector Mapping
  UInt_t fuNrOfGbtx;
  UInt_t fuNrOfModules;
  std::vector<Int_t> fviNrOfRpc;
  std::vector<Int_t> fviRpcType;
  std::vector<Int_t> fviRpcSide;
  std::vector<Int_t> fviModuleId;
  /// Event window limits
  Double_t fdAllowedTriggersSpread;
  std::vector<Double_t> fdStarTriggerDeadtime;  //! [sector]
  std::vector<Double_t> fdStarTriggerDelay;     //! [sector]
  std::vector<Double_t> fdStarTriggerWinSize;   //! [sector]

  /// User settings: Data selection parameters
  UInt_t fuMinTotPulser;
  UInt_t fuMaxTotPulser;

  /// Running indices
  ULong64_t fulCurrentTsIndex;  // Idx of the current TS
  Double_t fdTsStartTime;       // Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;    // End Time in ns of current TS Core from the index of the first MS first component
  size_t fuCurrentMs;           // Idx of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdMsTime;            // Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;             // Index of current MS within the TS
  UInt_t fuGdpbId;              // Id (hex number) of the GDPB for current message
  UInt_t fuGdpbNr;              // running number (0 to fuNrOfGdpbs) of the GDPB for current message
  UInt_t fuGet4Id;  // running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  // running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
  Int_t fiEquipmentId;

  /// Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch;       //!
  std::vector<ULong64_t> fvulCurrentEpochCycle;  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull;   //! Epoch + Epoch Cycle

  /// Buffers
  std::vector<std::vector<gdpbv100::Message>> fvvmEpSupprBuffer;  //! [sector]
  std::vector<std::vector<gdpbv100::FullMessage>>
    fvvBufferMajorAsicErrors;  //! [sector], buffer to make sure GET4 errors 0-12 are always transmitted
  std::vector<std::vector<gdpbv100::FullMessage>> fvvBufferMessages;  //! [sector]
  std::vector<std::vector<CbmTofStarTrigger2019>> fvvBufferTriggers;  //! [sector]
  std::vector<CbmTofStarSubevent2019> fvEventsBuffer;                 //! Event buffer

  /// STAR TRIGGER detection
  Bool_t fbTriggerFoundA;
  Bool_t fbTriggerFoundB;
  Bool_t fbTriggerFoundC;
  std::vector<ULong64_t> fvulGdpbTsMsb;           //! [sector]
  std::vector<ULong64_t> fvulGdpbTsLsb;           //! [sector]
  std::vector<ULong64_t> fvulStarTsMsb;           //! [sector]
  std::vector<ULong64_t> fvulStarTsMid;           //! [sector]
  std::vector<ULong64_t> fvulGdpbTsFullLast;      //! [sector]
  std::vector<ULong64_t> fvulStarTsFullLast;      //! [sector]
  std::vector<UInt_t> fvuStarTokenLast;           //! [sector]
  std::vector<UInt_t> fvuStarDaqCmdLast;          //! [sector]
  std::vector<UInt_t> fvuStarTrigCmdLast;         //! [sector]
  std::vector<ULong64_t> fvulGdpbTsFullLastCore;  //! [sector]
  std::vector<ULong64_t> fvulStarTsFullLastCore;  //! [sector]
  std::vector<UInt_t> fvuStarTokenLastCore;       //! [sector]
  std::vector<UInt_t> fvuStarDaqCmdLastCore;      //! [sector]
  std::vector<UInt_t> fvuStarTrigCmdLastCore;     //! [sector]

  /// Buffer insertion limits
  std::vector<Double_t> fvdMessCandidateTimeStart;  //! [sector]
  std::vector<Double_t> fvdMessCandidateTimeStop;   //! [sector]
  std::vector<Double_t> fvdTrigCandidateTimeStart;  //! [sector]
  std::vector<Double_t> fvdTrigCandidateTimeStop;   //! [sector]
  Bool_t fbEpochAfterCandWinFound;
  Bool_t fbTriggerAfterCandWinFound;

  /// ASIC status monitoring
  static const uint32_t kuNbMsgPerPattern = 8;
  static const uint32_t kuNbAsicPerGdpb   = 240;
  std::vector<std::bitset<kuNbAsicPerGdpb>> fvbGdpbLastMissmatchPattern;  //! [sector][asic] Exclude from dictionnary
  std::vector<std::bitset<kuNbAsicPerGdpb>> fvbGdpbLastEnablePattern;     //! [sector][asic] Exclude from dictionnary
  std::vector<std::bitset<kuNbAsicPerGdpb>> fvbGdpbLastResyncPattern;     //! [sector][asic] Exclude from dictionnary
  std::vector<std::vector<std::pair<uint64_t,
                                    std::bitset<kuNbAsicPerGdpb>>>>
    fvSectorStatusPattern;  //! [sector][Update]<time, [asic]> Exclude from dictionnary


  /// Histograms
  std::vector<TH1*> fvhHitsTimeToTriggerRaw;        //! [sector]
  std::vector<TH2*> fvhHitsTimeToTriggerRawPulser;  //! [sector], extra monitor for debug
  std::vector<TH1*> fvhHitsTimeToTriggerSel;        //! [sector]
  std::vector<TH2*> fvhHitsTimeToTriggerSelVsDaq;   //! [sector], extra monitor for debug
  std::vector<TH2*> fvhHitsTimeToTriggerSelVsTrig;  //! [sector], extra monitor for debug
  std::vector<TH1*> fvhTriggerDt;                   //! [sector], extra monitor for debug
  std::vector<TH1*> fvhTriggerDistributionInTs;     //! [sector], extra monitor for debug
  std::vector<TH1*> fvhTriggerDistributionInMs;     //! [sector], extra monitor for debug
  std::vector<TH1*> fvhMessDistributionInMs;        //! [sector], extra monitor for debug
  TH1* fhEventNbPerTs;                              //!
  TH1* fhEventSizeDistribution;                     //!
  TProfile* fhEventSizeEvolution;                   //!
  TH1* fhEventNbEvolution;                          //!
  TH1* fhEventNbDistributionInTs;                   //! extra monitor for debug
  TProfile* fhEventSizeDistributionInTs;            //! extra monitor for debug
  TH2* fhRawTriggersStats;                          //! extra monitor for debug
  TH2* fhRawTriggersStatsCore;                      //! extra monitor for debug
  TH2* fhRawTriggersStatsOver;                      //! extra monitor for debug
  TH2* fhRawTriggersStatsSel;                       //! extra monitor for debug
  TH2* fhMissingTriggersEvolution;                  //! extra monitor for debug
  TCanvas* fcTimeToTrigRaw;                         //! All sectors
  TCanvas* fcTimeToTrigSel;                         //! All sectors
  TCanvas* fcTrigDistMs;                            //! All sectors, extra monitor for debug
  TCanvas* fcMessDistMs;                            //! All sectors, extra monitor for debug
  TCanvas* fcEventBuildStats;                       //!
  TCanvas* fcTriggerStats;                          //! All sectors, extra monitor for debug

  void ProcessEpochCycle(uint64_t ulCycleData);
  void ProcessEpoch(gdpbv100::Message mess);
  void ProcessStarTrigger(gdpbv100::Message mess);

  void ProcessEpSupprBuffer(uint32_t uGdpbNr);
  void StoreMessageInBuffer(gdpbv100::FullMessage fullMess, uint32_t uGdpbNr);

  void ProcessHit(gdpbv100::Message mess, uint64_t ulCurEpochGdpbGet4);
  void ProcessSlCtrl(gdpbv100::Message mess, uint64_t ulCurEpochGdpbGet4);
  void ProcessSysMess(gdpbv100::Message mess, uint64_t ulCurEpochGdpbGet4);
  void ProcessPattern(gdpbv100::Message mess, uint64_t ulCurEpochGdpbGet4);

  void UpdateStatusPatternCurrGdpb();
  gdpbv100::FullMessage CreateStatusMessage(uint16_t uGdpbId, uint32_t uIndex,
                                            std::pair<uint64_t, std::bitset<kuNbAsicPerGdpb>> statusIn);

  CbmStar2019EventBuilderEtofAlgo(const CbmStar2019EventBuilderEtofAlgo&);
  CbmStar2019EventBuilderEtofAlgo operator=(const CbmStar2019EventBuilderEtofAlgo&);

  ClassDef(CbmStar2019EventBuilderEtofAlgo, 1)
};

#endif
