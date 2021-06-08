/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMcbmPulser                    -----
// -----                Created 20/02/19  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MONITORMCBMPULSER_H
#define CBMMCBM2018MONITORMCBMPULSER_H

#ifndef __CINT__
#include "Timeslice.hpp"
#endif

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

class CbmMcbm2018MonitorMcbmPulser : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorMcbmPulser();
  virtual ~CbmMcbm2018MonitorMcbmPulser();

  virtual Bool_t Init();
#ifndef __CINT__
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
#endif
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
  inline void SetMuchDpbIdx(UInt_t uIdx = 2) { fuMuchDpbIdx = uIdx; }
  inline void SetStsTofOffsetNs(Double_t dOffsetIn = 0.0) { fdStsTofOffsetNs = dOffsetIn; }
  inline void SetMuchTofOffsetNs(Double_t dOffsetIn = 0.0) { fdMuchTofOffsetNs = dOffsetIn; }
  inline void SetUseBestPair(Bool_t bInFLag = kTRUE) { fbUseBestPair = bInFLag; }
  inline void SetTsLevelAna(Bool_t bInFLag = kTRUE) { fbTsLevelAna = bInFLag; }

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

  std::vector<TH2*> fvhMcbmTimeDiffToSelDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToSelDpbWide;
  std::vector<TH2*> fvhMcbmTimeDiffToSelDpbTs;

  std::vector<TH2*> fvhMcbmTimeDiffToDiamondEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToDiamondWideEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToDiamondTsEvoDpb;

  std::vector<TH2*> fvhMcbmTimeDiffToTofEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToTofWideEvoDpb;
  std::vector<TH2*> fvhMcbmTimeDiffToTofTsEvoDpb;


  void CreateMcbmHistograms();
  /****************** mCBM Sync *****************************************/


  CbmMcbm2018MonitorMcbmPulser(const CbmMcbm2018MonitorMcbmPulser&);
  CbmMcbm2018MonitorMcbmPulser operator=(const CbmMcbm2018MonitorMcbmPulser&);

  ClassDef(CbmMcbm2018MonitorMcbmPulser, 1)
};

#endif  // CBMMCBM2018MONITORMCBMPULSER_H
