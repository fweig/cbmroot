/* Copyright (C) 2021 Variable Energy Cyclotron Centre, Kolkata
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Vikas Singhal [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMuchLite                        -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                Modified 07/12/18  by Ajit Kumar                 -----
// -----                Modified 05/03/19  by Vikas Singhal               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorAlgoMuchLite_H
#define CbmMcbm2018MonitorAlgoMuchLite_H

#ifndef __CINT__
#include "Timeslice.hpp"
#endif

// Data
#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot
#include "CbmHistManager.h"
#include "CbmMcbmUnpack.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmStar2019Algo.h"

// C++11
#include <chrono>

// C/C++
#include <map>
#include <set>
#include <vector>

class CbmMcbm2018MuchPar;
//class CbmCern2017UnpackParSts;

class CbmMcbm2018MonitorAlgoMuchLite : public CbmStar2019Algo<CbmMuchBeamTimeDigi> {
public:
  CbmMcbm2018MonitorAlgoMuchLite();
  CbmMcbm2018MonitorAlgoMuchLite(const CbmMcbm2018MonitorAlgoMuchLite&);
  CbmMcbm2018MonitorAlgoMuchLite operator=(const CbmMcbm2018MonitorAlgoMuchLite&);
  ~CbmMcbm2018MonitorAlgoMuchLite();


  virtual Bool_t Init();

  virtual void Reset();

  virtual void Finish();

  Bool_t InitContainers();

  Bool_t ReInitContainers();
  TList* GetParList();
  Bool_t InitParameters();
  Bool_t InitMuchParameters();

  Bool_t ProcessTs(const fles::Timeslice& ts);
  Bool_t ProcessTs(const fles::Timeslice& ts, size_t /*component*/) { return ProcessTs(ts); }
  //Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  Bool_t CreateHistograms();
  void SaveMuchHistos(TString sFileName = "");
  void ResetMuchHistos();
  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) { fbMonitorMode = bFlagIn; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetSpillThreshold(UInt_t uCntLimit) { fuOffSpillCountLimit = uCntLimit; }
  //void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);
  //void SetMsOverlap(size_t uOverlapMsNb = 1) { fuNbOverMsPerTs = uOverlapMsNb; }
  //size_t GetMsOverlap() { return fuNbOverMsPerTs; }
  Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void SetRunStart(Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5);
  void FillHitInfo(stsxyter::Message mess, const UShort_t& usElinkIdx, const UInt_t& uAsicIdx, const UInt_t& uMsIdx);

  /*void
  FillTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillEpochInfo(stsxyter::Message mess);
  
  void
  FillTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillEpochInfo(stsxyter::Message mess);

  
  
  Bool_t ScanForNoisyChannels(Double_t dNoiseThreshold = 1e3); */

  void ResetAllHistos();
  void SaveAllHistos(TString sFileName = "");
  void SaveHistos(TString sFileName = "");
  void SetHistoFileName(TString sFileName = "data/SetupHistos.root") { fsHistoFileFullname = sFileName; }

  void SetPrintMessage(Bool_t bPrintMessOn             = kTRUE,
                       stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex
                                                         | stsxyter::MessagePrintMask::msg_print_Human)
  {
    fbPrintMessages = bPrintMessOn;
    fPrintMessCtrl  = ctrl;
  }
  //void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );
  //void SetEnableCoincidenceMaps( Bool_t bEnableCoincidenceMapsOn = kTRUE ) { fbEnableCoincidenceMaps = bEnableCoincidenceMapsOn; }
  //void SetCoincidenceBorder( Double_t dCenterPos, Double_t dBorderVal );
  //void SetFebChanCoincidenceLimitNs( Double_t dLimitIn ) { fdFebChanCoincidenceLimit = dLimitIn; }
  //void UseNoiseLimitsSmx2LogicError( Bool_t bUseNoise = kTRUE ) { fbSmx2ErrorUseNoiseLevels = bUseNoise; }

  void SetMuchMode(Bool_t bMuchMode = kTRUE) { fbMuchMode = bMuchMode; }

  //   void SetTimeBin( size_t uTimeBin );
  void UseDaqBuffer(Bool_t) {};  //Virtual function in Mother Class, Need to update accordingly. VS

  /// => Quick and dirty hack for binning FW!!!
  void SetBinningFwFlag(Bool_t bEnable = kTRUE) { fbBinningFw = bEnable; }
  Bool_t ScanForNoisyChannels(Double_t dNoiseThreshold = 1e3);

private:
  // Parameters
  // Control flags
  Bool_t fbMonitorMode = kFALSE;  //! Switch ON the filling of a minimal set of histograms
  Bool_t fbMuchMode;
  std::vector<Bool_t> fvbMaskedComponents;
  /// Hits time-sorting
  std::vector<stsxyter::FinalHit>
    fvmHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator
  TString fsHistoFilename;
  /// Task configuration values
  Bool_t fbPrintMessages;
  stsxyter::MessagePrintMask fPrintMessCtrl;
  ULong64_t fulCurrentTsIdx;
  size_t fuNbCoreMsPerTs;                   //!
  UInt_t fuNrOfDpbs;                        //! Total number of STS DPBs in system
  std::map<UInt_t, UInt_t> fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  std::vector<std::vector<Bool_t>>
    fvbCrobActiveFlag;   //! Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  UInt_t fuNbFebs;       //! Number of StsXyter ASICs
  UInt_t fuNbStsXyters;  //! Number of StsXyter ASICs
  std::map<stsxyter::MessType, UInt_t> fmMsgCounter;
  CbmMcbm2018MuchPar* fUnpackParMuch;  //!

  /// => Quick and dirty hack for binning FW!!!
  Bool_t fbBinningFw = kTRUE;

  // CbmMcbm2018MuchPar* fUnpackPar = nullptr;  //!

  Double_t fdTsStartTime = -1.0;  //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore =
    -1.0;  //! End Time in ns of current TS Core from the index of the first MS first component

  UInt_t fuMsIndex = 0;  //! Index of current MS within the TS

  /// Histograms related variables
  UInt_t fuHistoryHistoSize   = 3600; /** Size in seconds of the evolution histograms **/
  UInt_t fuOffSpillCountLimit = 200;

  Bool_t fbIgnoreOverlapMs;  //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
                             // Unpacking and mapping
  //std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcGain;  //! ADC gain in e-/b, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]
  //std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcOffs;  //! ADC offset in e-, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]

  // Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 4;

  /// Internal Control/status of monitor
  /// Histo File name and path
  TString fsHistoFileFullname;
  //Bool_t                fbEnableCoincidenceMaps;
  /// TS/MS info
  ULong64_t fulCurrentMsIdx;
  /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  Int_t fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;     //! Bin size in s for the plots with date as X axis

  /// Data format control
  std::vector<ULong64_t> fvulCurrentTsMsb;                  //! Current TS MSB for each DPB
  std::vector<UInt_t> fvuCurrentTsMsbCycle;                 //! Current TS MSB cycle for DPB
  std::vector<UInt_t> fvuInitialHeaderDone;                 //! Flag set after seeing MS header in 1st MS for DPB
  std::vector<UInt_t> fvuInitialTsMsbCycleHeader;           //! TS MSB cycle from MS header in 1st MS for DPB
  std::vector<UInt_t> fvuElinkLastTsHit;                    //! TS from last hit for DPB
                                                            /// Hits comparison
  std::vector<std::vector<ULong64_t>> fvulChanLastHitTime;  //! Last hit time in bins for each Channel
  //std::vector< std::vector< ULong64_t  > > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
  std::vector<std::vector<Double_t>> fvdChanLastHitTime;  //! Last hit time in ns   for each Channel
  std::vector<Double_t> fvdPrevMsTime;                    //! Header time of previous MS per link
  std::vector<Double_t> fvdMsTime;                        //! Header time of each MS
  std::vector<std::vector<std::vector<Double_t>>>
    fvdChanLastHitTimeInMs;  //! Last hit time in bins in each MS for each Channel
  std::vector<std::vector<std::vector<UShort_t>>>
    fvusChanLastHitAdcInMs;  //! Last hit ADC in bins in each MS for each Channel

  std::vector<std::vector<stsxyter::FinalHit>>
    fvmAsicHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, per ASIC, sorted with "<" operator
  std::vector<std::vector<stsxyter::FinalHit>>
    fvmFebHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, per FEB, sorted with "<" operator
  /// Mean Rate per channel plots
  Int_t fiTimeIntervalRateUpdate;
  std::vector<Int_t> fviFebTimeSecLastRateUpdate;
  std::vector<Int_t> fviFebCountsSinceLastRateUpdate;
  std::vector<std::vector<Double_t>> fvdFebChanCountsSinceLastRateUpdate;
  /// Rate evolution histos
  //Bool_t fbLongHistoEnable;
  //UInt_t fuLongHistoNbSeconds;
  //UInt_t fuLongHistoBinSizeSec;
  //UInt_t fuLongHistoBinNb;
  UInt_t Counter;
  UInt_t Counter1;
  /// Coincidences in sorted hits
  //Double_t fdCoincCenter;  // ns
  //Double_t fdCoincBorder;  // ns, +/-
  //Double_t fdCoincMin;     // ns
  //Double_t fdCoincMax;     // ns

  /// Histograms
  CbmHistManager* fHM;  //! Histogram manager
  TH1* fhMuchMessType;
  TH1* fhMuchSysMessType;
  TH1* fhMuchFebChanAdcRaw_combined;
  TH2* fhMuchMessTypePerDpb;
  TH2* fhMuchSysMessTypePerDpb;
  TH2* fhStatusMessType;
  TH2* fhMsStatusFieldType;
  TH2* fhMuchHitsElinkPerDpb;

  TH1* fhRate;
  TH1* fhRateAdcCut;
  TH1* fhFEBcount = nullptr;
  // std::vector<TH1 *>     fhMuchFebChanRateEvo;

  /// Plots per FEB-8
  //UInt_t   kuNbAsicPerFeb = 1;

  std::vector<TH2*> fHistPadDistr;
  std::vector<TH2*> fRealHistPadDistr;

  //Double_t fdFebChanCoincidenceLimit; // ns
  std::vector<TH1*> fhMuchFebChanCntRaw;
  std::vector<TH1*> fhMuchFebSpill = {};
  std::vector<TH2*> fhMuchFebADC   = {};
  //std::vector<TH1 *>     fhMuchFebChanCntRawGood;
  std::vector<TH2*> fhMuchFebChanAdcRaw;
  std::vector<TProfile*> fhMuchFebChanAdcRawProf;
  //std::vector<TH2 *>     fhMuchFebChanAdcCal;
  //std::vector<TProfile*> fhMuchFebChanAdcCalProf;
  std::vector<TH2*> fhMuchFebChanRawTs;
  std::vector<TH2*> fhMuchChannelTime = {};
  //std::vector<TH2*>      fhMuchFebChanMissEvt;
  //std::vector<TH2*>      fhMuchFebChanMissEvtEvo;
  //std::vector<TH2*>      fhMuchFebAsicMissEvtEvo;
  //std::vector<TH1*>      fhMuchFebMissEvtEvo;
  std::vector<TH2*> fhMuchFebChanHitRateEvo;
  std::vector<TProfile*> fhMuchFebChanHitRateProf;
  //std::vector<TH2*>      fhMuchFebAsicHitRateEvo;
  std::vector<TH1*> fhMuchFebHitRateEvo;
  std::vector<TH1*> fhMuchFebHitRateEvo_mskch;
  std::vector<TH1*> fhMuchFebHitRateEvo_mskch_adccut;
  std::vector<TH1*> fhMuchFebHitRateEvo_WithoutDupli;
  //std::vector<TH2*>      fhMuchFebChanHitRateEvoLong;
  //std::vector<TH2*>      fhMuchFebAsicHitRateEvoLong;
  //std::vector<TH1*>      fhMuchFebHitRateEvoLong;
  std::vector<std::vector<Double_t>> fdMuchFebChanLastTimeForDist;
  std::vector<TH2*> fhMuchFebChanDistT;
  //std::vector< std::vector<TH1*> > fhMuchFebChanDtCoinc;
  //std::vector< std::vector<TH2*> > fhMuchFebChanCoinc;
  std::vector<TProfile*> fhMuchFebDuplicateHitProf;

  /// Binning FW error flag
  TH2* fhDpbMsErrors = nullptr;

  TCanvas* fcMsSizeAll;

  // FLES containers
  std::vector<size_t> fvMsComponentsList;  //!

  /// Coincidence histos
  UInt_t fuMaxNbMicroslices;

  TH1* fhMsSz[kiMaxNbFlibLinks];
  TProfile* fhMsSzTime[kiMaxNbFlibLinks];

  //   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
  /// Starting state book-keeping
  //Long64_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Long64_t prevtime_new;    /** previous time for consecutive hit time**/
  Long64_t prevTime;        /** previous time for consecutive hit time**/
  UInt_t prevAsic;          /** previous asic**/
  UInt_t prevChan;          /** previous channel**/
  Double_t fdStartTime;     /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  std::vector<std::vector<std::vector<UInt_t>>> fvuChanNbHitsInMs;  //! Number of hits in each MS for each Channel

  // Methods later going into Algo
  size_t fuNbOverMsPerTs;  //!


  void FillTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillEpochInfo(stsxyter::Message mess);


  ClassDef(CbmMcbm2018MonitorAlgoMuchLite, 1)
};

#endif  // CbmMcbm2018MonitorAlgoMuchLite_H
