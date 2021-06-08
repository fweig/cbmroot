/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018MonitorDataRates                    -----
// -----               Created 26.03.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorDataRates_H
#define CbmMcbm2018MonitorDataRates_H

#include "CbmMcbmUnpack.h"

#include "Timeslice.hpp"

#include "Rtypes.h"
#include "TClonesArray.h"

#include <chrono>
#include <map>
#include <vector>

class CbmMcbm2018TofPar;

class TCanvas;
class TH1;
class TH2;
class TProfile;
class TProfile2D;

class CbmMcbm2018MonitorDataRates : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorDataRates();
  virtual ~CbmMcbm2018MonitorDataRates();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  void SetMsLimitLevel(size_t uAcceptBoundaryPct = 100) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
  size_t GetMsLimitLevel() { return fuMsAcceptsPercent; }

  virtual void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);
  void SetIgnoreOverlapMs(Bool_t bEnaFlag = kTRUE) { fbIgnoreOverlapMs = bEnaFlag; }
  void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
  size_t GetMsOverlap() { return fuOverlapMsNb; }

  inline void SetHistoFilename(TString sNameIn) { fsHistoFilename = sNameIn; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }

  inline void AddEqIdChannelNumber(UInt_t uEqId, UInt_t uNbCh) { fmChannelsPerEqId[uEqId] = uNbCh; }

  void SaveAllHistos(TString sFileName = "");
  void ResetAllHistos();
  void ResetEvolutionHistograms();
  void UseDaqBuffer(Bool_t) {};

private:
  /// FLES containers
  std::vector<size_t> fvMsComponentsList;  //!
  size_t fuNbCoreMsPerTs;                  //!
  size_t fuNbOverMsPerTs;                  //!
  Bool_t fbIgnoreOverlapMs;                //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

  /// OLD, to be cleaned out !!!!!
  size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
  size_t fuTotalMsNb;        /** Total nb of MS per link in timeslice **/
  size_t fuOverlapMsNb;      /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  size_t fuCoreMs;           /** Number of non overlap MS at beginning of TS **/
  Double_t fdMsSizeInNs;
  Double_t fdTsCoreSizeInNs;

  /** Control Flags **/
  TString fsHistoFilename;
  UInt_t fuNbFlimLinks;

  /// Parameters
  std::map<UInt_t, UInt_t> fmChannelsPerEqId;

  /** Running indices **/
  uint64_t fulCurrentTsIndex;  // Idx of the current TS
  size_t fuCurrentMs;          // Idx of the current MS in TS (0 to fuTotalMsNb)
  size_t fuCurrentMsSysId;     // SysId of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdMsIndex;          // Time in ns of current MS from its index
  Int_t fiEquipmentId;

  /// Constants
  static const UInt_t kuSysIdSts            = 10;
  static const UInt_t kuSysIdRich           = 30;
  static const UInt_t kuSysIdMuch           = 40;
  static const UInt_t kuSysIdTof            = 60;
  static const UInt_t kuSysIdT0             = 90;
  static const UInt_t kuBytesPerMessageSts  = 4;
  static const UInt_t kuBytesPerMessageRich = 4;
  static const UInt_t kuBytesPerMessageMuch = 4;
  static const UInt_t kuBytesPerMessageTof  = 8;
  static const UInt_t kuBytesPerMessageT0   = 8;

  /// Histograms and histogram control variables
  // Evolution plots control
  Double_t fdStartTimeMsSz;  /** Time of first microslice, used as reference for evolution plots**/
  UInt_t fuHistoryHistoSize; /** Size in seconds of the evolution histograms **/
                             // Counters
  std::vector<UInt_t> fvuTsSzLink;
  // Flesnet
  TCanvas* fcDataRateTimeAll;
  TCanvas* fcTsSizeAll;
  TCanvas* fcTsSizeTimeAll;
  TCanvas* fcMsSizeAll;
  TCanvas* fcMsSizeTimeAll;
  TCanvas* fcMsMessAll;
  TCanvas* fcMsMessTimeAll;
  TCanvas* fcMsDataChAll;
  TCanvas* fcMsDataChTimeAll;
  TH1* fhDataRateTimeAllLinks;
  std::vector<TH1*> fvhDataRateTimePerLink;
  std::vector<TH1*> fvhTsSzPerLink;
  std::vector<TProfile*> fvhTsSzTimePerLink;
  std::vector<TH1*> fvhMsSzPerLink;
  std::vector<TProfile*> fvhMsSzTimePerLink;
  std::vector<TH1*> fvhMsMessPerLink;
  std::vector<TProfile*> fvhMsMessTimePerLink;
  std::vector<TH1*> fvhMsMeanChDataPerLink;
  std::vector<TH1*> fvhMsMeanChDataTimePerLink;

  void CreateHistograms();

  CbmMcbm2018MonitorDataRates(const CbmMcbm2018MonitorDataRates&);
  CbmMcbm2018MonitorDataRates operator=(const CbmMcbm2018MonitorDataRates&);

  ClassDef(CbmMcbm2018MonitorDataRates, 1)
};

#endif
