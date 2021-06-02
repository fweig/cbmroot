/** @file CbmStar2019MonitorPulserAlgo.h
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStar2019MonitorPulserAlgo                     -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019MonitorPulserAlgo_H
#define CbmStar2019MonitorPulserAlgo_H

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

class CbmStar2019TofPar;
class TH1;
class TH2;
class TProfile;

class CbmStar2019MonitorPulserAlgo : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmStar2019MonitorPulserAlgo();
  ~CbmStar2019MonitorPulserAlgo();

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
  Bool_t UpdateStats();
  Bool_t ResetHistograms();

  inline void SetEtofFeeIndexing(Bool_t bFlagIn = kTRUE) { fbEtofFeeIndexing = bFlagIn; }
  inline void SetSectorIndex(Int_t iSector = -1) { fiSectorIndex = iSector; }
  inline void SetUpdateFreqTs(UInt_t uFreq = 100) { fuUpdateFreqTs = uFreq; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuPulserMinTot = uMin;
    fuPulserMaxTot = uMax;
  }
  inline void SetPulserChannel(UInt_t uChan) { fuPulserChannel = uChan; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }

private:
  /// Control flags
  Bool_t fbEtofFeeIndexing;
  std::vector<Bool_t> fvbMaskedComponents;
  Int_t fiSectorIndex;
  UInt_t fuUpdateFreqTs;

  /// Settings from parameter file
  CbmStar2019TofPar* fUnpackPar;             //!
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

  /// User settings: Data correction parameters
  UInt_t fuPulserMinTot;
  UInt_t fuPulserMaxTot;
  UInt_t fuPulserChannel;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;

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
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  UInt_t fuGet4Id;  //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch;       //! Current epoch index, per DPB
  std::vector<ULong64_t> fvulCurrentEpochCycle;  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull;   //! Epoch + Epoch Cycle

  /// Buffer for suppressed epoch processing
  std::vector<gdpbv100::Message> fvmEpSupprBuffer;

  /// Storing the time of the last hit for each MS in each of the FEE
  std::vector<std::vector<Bool_t>> fvvbFeeHitFound;  //! [ Sector ][ FEE ]
  std::vector<std::vector<Double_t>> fvvdFeeHits;    //! [ Sector ][ FEE ]

  /// Histograms and histogram control variables
  /// Default value for nb bins in Pulser time difference histos
  const Double_t kdMaxDtPulserPs = 300e3;
  const UInt_t kuNbBinsDt        = 2000;
  Double_t dMinDt;
  Double_t dMaxDt;
  const Double_t kdFitZoomWidthPs = 500.0;
  /// Starting time and time evolution book-keeping
  Double_t fdStartTime;       //! Time of first MS
  UInt_t fuHistoryHistoSize;  //! Size in seconds of the evolution histograms

  /// Histograms
  /// Pulser dT within sector
  std::vector<std::vector<TH1*>> fvvhFeePairPulserTimeDiff;  //! [ FEE A ][ FEE B ]
  TH2* fhPulserTimeDiffMean;                                 //!
  TH2* fhPulserTimeDiffRms;                                  //!
  TH2* fhPulserTimeDiffRmsZoom;                              //!
  TH2* fhPulserRmsGdpbToRefEvo;                              //!
  TH2* fhPulserRmsGbtxToRefEvo;                              //!

  /// Canvases
  TCanvas* fcSummary;  //!

  void ProcessEpochCycle(uint64_t ulCycleData);
  void ProcessEpoch(gdpbv100::Message mess);

  void ProcessEpSupprBuffer();

  void ProcessHit(gdpbv100::FullMessage mess);

  CbmStar2019MonitorPulserAlgo(const CbmStar2019MonitorPulserAlgo&);
  CbmStar2019MonitorPulserAlgo operator=(const CbmStar2019MonitorPulserAlgo&);

  ClassDef(CbmStar2019MonitorPulserAlgo, 1)
};

#endif  // CbmStar2019MonitorPulserAlgo_H
