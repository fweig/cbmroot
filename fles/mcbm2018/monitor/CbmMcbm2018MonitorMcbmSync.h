/** @file CbmMcbm2018MonitorMcbmSync.h
  * @copyright Copyright (C) 2018-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMcbmSync                      -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MONITORMCBMSYNC_H
#define CBMMCBM2018MONITORMCBMSYNC_H

#include "Timeslice.hpp"

// Data
#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"
#include "gDpbMessv100.h"

// CbmRoot
#include "CbmHistManager.h"
#include "CbmMcbmUnpack.h"

// C++11
#include <chrono>

// C/C++
#include <map>
#include <set>
#include <vector>

class CbmMcbm2018StsPar;

class CbmMcbm2018TofPar;

class CbmMcbm2018MonitorMcbmSync : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorMcbmSync();
  virtual ~CbmMcbm2018MonitorMcbmSync();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  virtual void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void AddMsComponentToListSts(size_t component);
  void AddMsComponentToListTof(size_t component);

  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);
  void SetMsOverlap(size_t uOverlapMsNb = 1) { fuNbOverMsPerTs = uOverlapMsNb; }
  size_t GetMsOverlap() { return fuNbOverMsPerTs; }
  void SetIgnoreMsOverlap(Bool_t bIgnoreOver = kTRUE) { fbIgnoreOverlapMs = bIgnoreOver; }

  void ResetAllHistos();
  void SaveAllHistos(TString sFileName = "");
  void SavePulserHistos(TString sFileName = "");
  void SetHistoFileName(TString sFileName = "data/SetupHistos.root") { fsHistoFileFullname = sFileName; }

  inline void SetDiamondDpbIdx(UInt_t uIdx = 2) { fuDiamondDpbIdx = uIdx; }
  inline void SetMuchDpbIdx(UInt_t uIdx = 1) { fuMuchDpbIdx = uIdx; }
  inline void SetStsTofOffsetNs(Double_t dOffsetIn = 0.0) { fdStsTofOffsetNs = dOffsetIn; }
  inline void SetMuchTofOffsetNs(Double_t dOffsetIn = 0.0) { fdMuchTofOffsetNs = dOffsetIn; }
  inline void SetUseBestPair(Bool_t bInFLag = kTRUE) { fbUseBestPair = bInFLag; }
  inline void SetTsLevelAna(Bool_t bInFLag = kTRUE) { fbTsLevelAna = bInFLag; }
  inline void SetSpillLimits(Double_t dSpillA, Double_t dSpillB, Double_t dSpillC)
  {
    fdSpillStartA = dSpillA;
    fdSpillStartB = dSpillB;
    fdSpillStartC = dSpillC;
  }

  void UseDaqBuffer(Bool_t) {};

private:
  /// Parameters
  /// FLES containers
  std::vector<size_t> fvMsComponentsListSts;  //!
  std::vector<size_t> fvMsComponentsListTof;  //!
  size_t fuNbCoreMsPerTs;                     //!
  size_t fuNbOverMsPerTs;                     //!
  Bool_t fbIgnoreOverlapMs;          //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
                                     /// Unpacking and mapping parameters for STS
  CbmMcbm2018StsPar* fUnpackParSts;  //!
  UInt_t fuStsNrOfDpbs;              //! Total number of Sts DPBs in system
  std::map<UInt_t, UInt_t> fmStsDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  UInt_t fuMuchDpbIdx;
  /// Unpacking and mapping parameters for TOF
  CbmMcbm2018TofPar* fUnpackParTof;  //!
  UInt_t fuTofNrOfDpbs;              //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fmTofDpbIdIndexMap;
  UInt_t fuDiamondDpbIdx;
  /// Global parameters
  UInt_t fuTotalNrOfDpb;
  Double_t fdStsTofOffsetNs;
  Double_t fdMuchTofOffsetNs;
  Bool_t fbUseBestPair;
  Bool_t fbTsLevelAna;

  // Constants
  static const UInt_t kuStsBytesPerMessage = 4;  //! TODO => move to the message class!!
  static const UInt_t kuTofBytesPerMessage = 8;

  // Internal Control/status of monitor
  // Histo File name and path
  TString fsHistoFileFullname;
  // Task configuration values
  Bool_t fbPrintMessages;
  stsxyter::MessagePrintMask fPrintMessCtrlSts;
  // TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  // Current data properties
  std::map<stsxyter::MessType, UInt_t> fmMsgCounter;
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  Int_t fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;     //! Bin size in s for the plots with date as X axis

  /****************** STS Sync ******************************************/
  // Data format control
  std::vector<ULong64_t> fvulStsCurrentTsMsb;   //! Current TS MSB for each DPB
  std::vector<UInt_t> fvuStsCurrentTsMsbCycle;  //! Current TS MSB cycle for DPB

  // Hits time-sorting
  std::vector<std::vector<stsxyter::FinalHit>>
    fvmStsSdpbHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator
  std::vector<std::vector<stsxyter::FinalHit>>
    fvmStsSdpbHitsInTs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator

  /// Processing methods
  void CreateStsHistograms();

  Bool_t ProcessStsMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx);

  void FillStsHitInfo(stsxyter::Message mess, const UInt_t& uMsIdx);
  void FillStsTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillStsEpochInfo(stsxyter::Message mess);
  /****************** STS Sync ******************************************/

  /****************** TOF Sync ******************************************/

  /// Running indices
  uint64_t fulTofCurrentTsIndex;  // Idx of the current TS
  size_t fuTofCurrentMs;          // Idx of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdTofMsIndex;          // Time in ns of current MS from its index
  UInt_t fuTofGdpbId;             // Id (hex number) of the GDPB for current message
  UInt_t fuTofGdpbNr;             // running number (0 to fuNrOfGdpbs) of the GDPB for current message
  Int_t fiTofEquipmentId;
  std::vector<int> fviTofMsgCounter;

  /** Current epoch marker for each GDPB and GET4
     * (first epoch in the stream initializes the map item)
     * pointer points to an array of size fuNrOfGdpbs * fuNrOfGet4PerGdpb
     * The correct array index is calculated using the function
     * GetArrayIndex(gdpbId, get4Id)
     **/
  std::vector<ULong64_t> fvulTofCurrentEpoch;       //!
  std::vector<ULong64_t> fvulTofCurrentEpochCycle;  //!
  std::vector<ULong64_t> fvulTofCurrentEpochFull;   //!

  ULong64_t fulTofCurrentEpochTime; /** Time stamp of current epoch **/

  /// Buffer for suppressed epoch processing
  std::vector<std::vector<gdpbv100::Message>> fvmTofEpSupprBuffer;

  /// Buffer for system sync check
  std::vector<std::vector<gdpbv100::FullMessage>> fvmTofGdpbHitsInMs;
  std::vector<std::vector<gdpbv100::FullMessage>> fvmTofGdpbHitsInTs;

  /// Processing methods
  void CreateTofHistograms();

  Bool_t ProcessTofMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx);

  void FillTofEpochCycle(uint64_t ulCycleData);
  void FillTofHitInfo(gdpbv100::Message);
  void FillTofEpochInfo(gdpbv100::Message);
  /****************** TOF Sync ******************************************/

  /****************** mCBM Sync *****************************************/
  TH2* fhMcbmHitsNbPerMs;

  TH2* fhMcbmTimeDiffToDiamond;
  TH2* fhMcbmTimeDiffToDiamondWide;
  TH2* fhMcbmTimeDiffToDiamondTs;

  TH2* fhMcbmTimeDiffToMuch;
  TH2* fhMcbmTimeDiffToMuchWide;
  TH2* fhMcbmTimeDiffToMuchTs;

  TH2* fhMcbmStsTimeDiffToMuchVsAdc;
  TH2* fhMcbmStsTimeDiffToMuchWideVsAdc;
  TH2* fhMcbmStsTimeDiffToMuchTsVsAdc;

  std::vector<TH2*> fvhMcbmTimeDiffToDiamondEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToDiamondWideEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToDiamondTsEvoDpb;

  Double_t fdSpillStartA;
  Double_t fdSpillStartB;
  Double_t fdSpillStartC;
  std::vector<TH1*> fvhHitsTimeEvoSpillA;
  std::vector<TH1*> fvhHitsTimeEvoSpillB;

  std::vector<TH2*> fvhMcbmTimeDiffToDiamondEvoSpillA;
  std::vector<TH2*> fvhMcbmTimeDiffToDiamondEvoSpillB;

  std::vector<TH2*> fvhMcbmTimeDiffToMuchEvoSpillA;
  std::vector<TH2*> fvhMcbmTimeDiffToMuchEvoSpillB;

  void CreateMcbmHistograms();
  /****************** mCBM Sync *****************************************/


  CbmMcbm2018MonitorMcbmSync(const CbmMcbm2018MonitorMcbmSync&);
  CbmMcbm2018MonitorMcbmSync operator=(const CbmMcbm2018MonitorMcbmSync&);

  ClassDef(CbmMcbm2018MonitorMcbmSync, 1)
};

#endif  // CBMMCBM2018MONITORMCBMSYNC_H
