/* Copyright (C) 2019 Variable Energy Cyclotron Centre, Kolkata
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Vikas Singhal [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMuch                        -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                Modified 07/12/18  by Ajit Kumar                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MONITORMUCH_H
#define CBMMCBM2018MONITORMUCH_H

#ifndef __CINT__
#include "Timeslice.hpp"
#endif

// Data
#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot
#include "CbmHistManager.h"
#include "CbmMcbmUnpack.h"

// C++11
#include <chrono>

// C/C++
#include <map>
#include <set>
#include <vector>

class CbmMcbm2018MuchPar;
class CbmCern2017UnpackParSts;

class CbmMcbm2018MonitorMuch : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorMuch();
  virtual ~CbmMcbm2018MonitorMuch();

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
  virtual void SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb);
  void SetMsOverlap(size_t uOverlapMsNb = 1) { fuNbOverMsPerTs = uOverlapMsNb; }
  size_t GetMsOverlap() { return fuNbOverMsPerTs; }

  void SetRunStart(Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5);

  void ResetAllHistos();
  void SaveAllHistos(TString sFileName = "");
  void SavePulserHistos(TString sFileName = "");
  void SetHistoFileName(TString sFileName = "data/SetupHistos.root") { fsHistoFileFullname = sFileName; }

  void SetPrintMessage(Bool_t bPrintMessOn             = kTRUE,
                       stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex
                                                         | stsxyter::MessagePrintMask::msg_print_Human)
  {
    fbPrintMessages = bPrintMessOn;
    fPrintMessCtrl  = ctrl;
  }
  void SetLongDurationLimits(UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1);
  void SetEnableCoincidenceMaps(Bool_t bEnableCoincidenceMapsOn = kTRUE)
  {
    fbEnableCoincidenceMaps = bEnableCoincidenceMapsOn;
  }
  void SetCoincidenceBorder(Double_t dCenterPos, Double_t dBorderVal);
  void SetFebChanCoincidenceLimitNs(Double_t dLimitIn) { fdFebChanCoincidenceLimit = dLimitIn; }
  void UseNoiseLimitsSmx2LogicError(Bool_t bUseNoise = kTRUE) { fbSmx2ErrorUseNoiseLevels = bUseNoise; }

  void SetMuchMode(Bool_t bMuchMode = kTRUE) { fbMuchMode = bMuchMode; }

  //   void SetTimeBin( size_t uTimeBin );
  void UseDaqBuffer(Bool_t) {};  //Virtual function in Mother Class, Need to update accordingly. VS
private:
  // Parameters
  // Control flags
  Bool_t fbMuchMode;
  // FLES containers
  std::vector<size_t> fvMsComponentsList;   //!
  size_t fuNbCoreMsPerTs;                   //!
  size_t fuNbOverMsPerTs;                   //!
  Bool_t fbIgnoreOverlapMs;                 //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
                                            // Unpacking and mapping
  CbmMcbm2018MuchPar* fUnpackParMuch;       //!
  UInt_t fuNrOfDpbs;                        //! Total number of STS DPBs in system
  std::map<UInt_t, UInt_t> fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  std::vector<std::vector<Bool_t>>
    fvbCrobActiveFlag;   //! Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  UInt_t fuNbFebs;       //! Number of StsXyter ASICs
  UInt_t fuNbStsXyters;  //! Number of StsXyter ASICs
  std::vector<std::vector<std::vector<Double_t>>>
    fvdFebAdcGain;  //! ADC gain in e-/b, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]
  std::vector<std::vector<std::vector<Double_t>>>
    fvdFebAdcOffs;  //! ADC offset in e-, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]

  // Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 4;

  /// Internal Control/status of monitor
  /// Histo File name and path
  TString fsHistoFileFullname;
  /// Task configuration values
  Bool_t fbPrintMessages;
  stsxyter::MessagePrintMask fPrintMessCtrl;
  Bool_t fbEnableCoincidenceMaps;
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  /// Current data properties
  std::map<stsxyter::MessType, UInt_t> fmMsgCounter;
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
  std::vector<std::vector<Double_t>> fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
  //std::vector< std::vector< Double_t  > > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
  std::vector<Double_t> fvdPrevMsTime;                              //! Header time of previous MS per link
  std::vector<Double_t> fvdMsTime;                                  //! Header time of each MS
  std::vector<std::vector<std::vector<UInt_t>>> fvuChanNbHitsInMs;  //! Number of hits in each MS for each Channel
  std::vector<std::vector<std::vector<Double_t>>>
    fvdChanLastHitTimeInMs;  //! Last hit time in bins in each MS for each Channel
  std::vector<std::vector<std::vector<UShort_t>>>
    fvusChanLastHitAdcInMs;  //! Last hit ADC in bins in each MS for each Channel
  //   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
  /// Starting state book-keeping
  Long64_t fdStartTime; /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Long64_t prevtime;    /** previous time for consecutive hit time**/
  //Double_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Hits time-sorting
  std::vector<stsxyter::FinalHit>
    fvmHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator
  std::vector<std::vector<stsxyter::FinalHit>>
    fvmAsicHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, per ASIC, sorted with "<" operator
  std::vector<std::vector<stsxyter::FinalHit>>
    fvmFebHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last TS, per FEB, sorted with "<" operator
                     /// Coincidence histos
  UInt_t fuMaxNbMicroslices;
  /// Mean Rate per channel plots
  Int_t fiTimeIntervalRateUpdate;
  std::vector<Int_t> fviFebTimeSecLastRateUpdate;
  std::vector<Int_t> fviFebCountsSinceLastRateUpdate;
  std::vector<std::vector<Double_t>> fvdFebChanCountsSinceLastRateUpdate;
  /// Rate evolution histos
  Bool_t fbLongHistoEnable;
  UInt_t fuLongHistoNbSeconds;
  UInt_t fuLongHistoBinSizeSec;
  UInt_t fuLongHistoBinNb;
  UInt_t Counter;
  UInt_t Counter1;
  /// Coincidences in sorted hits
  Double_t fdCoincCenter;  // ns
  Double_t fdCoincBorder;  // ns, +/-
  Double_t fdCoincMin;     // ns
  Double_t fdCoincMax;     // ns

  /// Histograms
  CbmHistManager* fHM;  //! Histogram manager
  TH1* fhStsMessType;
  TH1* fhStsSysMessType;
  TH1* fhStsFebChanAdcRaw_combined;
  TH2* fhStsMessTypePerDpb;
  TH2* fhStsSysMessTypePerDpb;
  TH2* fhPulserStatusMessType;
  TH2* fhPulserMsStatusFieldType;
  TH2* fhStsHitsElinkPerDpb;

  TH1* fhRate;
  TH1* fhRateAdcCut;
  // std::vector<TH1 *>     fhStsFebChanRateEvo;

  /// Plots per FEB-8
  //   UInt_t   kuNbAsicPerFeb = 8;

  std::vector<TH2*> fHistPadDistr;
  std::vector<TH2*> fRealHistPadDistr;

  Double_t fdFebChanCoincidenceLimit;  // ns
  std::vector<TH1*> fhStsFebChanCntRaw;
  std::vector<TH1*> fhStsFebChanCntRawGood;
  std::vector<TH2*> fhStsFebChanAdcRaw;
  std::vector<TProfile*> fhStsFebChanAdcRawProf;
  std::vector<TH2*> fhStsFebChanAdcCal;
  std::vector<TProfile*> fhStsFebChanAdcCalProf;
  std::vector<TH2*> fhStsFebChanRawTs;
  std::vector<TH2*> fhStsFebChanMissEvt;
  std::vector<TH2*> fhStsFebChanMissEvtEvo;
  std::vector<TH2*> fhStsFebAsicMissEvtEvo;
  std::vector<TH1*> fhStsFebMissEvtEvo;
  std::vector<TH2*> fhStsFebChanHitRateEvo;
  std::vector<TProfile*> fhStsFebChanHitRateProf;
  std::vector<TH2*> fhStsFebAsicHitRateEvo;
  std::vector<TH1*> fhStsFebHitRateEvo;
  std::vector<TH1*> fhStsFebHitRateEvo_mskch;
  std::vector<TH1*> fhStsFebHitRateEvo_mskch_adccut;
  std::vector<TH2*> fhStsFebChanHitRateEvoLong;
  std::vector<TH2*> fhStsFebAsicHitRateEvoLong;
  std::vector<TH1*> fhStsFebHitRateEvoLong;
  std::vector<std::vector<Double_t>> fdStsFebChanLastTimeForDist;
  std::vector<TH2*> fhStsFebChanDistT;
  std::vector<std::vector<TH1*>> fhStsFebChanDtCoinc;
  std::vector<std::vector<TH2*>> fhStsFebChanCoinc;

  /// SXM 2.0 logic error detection and tagging, 1 eLink case
  Bool_t fbSmx2ErrorUseNoiseLevels;
  static const UInt_t kuSmxErrCoincWinNbHits          = 11;
  constexpr static const Double_t kdSmxErrCoincWinBeg = 0;  //ns, true for time sorted hits!
  /// Coincidence windows for 99.0% tagging (up to 1% of corruption not detected)
  constexpr static const Double_t kdSmxErrCoincWinMainM07 = 320;  //ns
  constexpr static const Double_t kdSmxErrCoincWinMainM08 = 380;  //ns
  constexpr static const Double_t kdSmxErrCoincWinMainM09 = 450;  //ns
  constexpr static const Double_t kdSmxErrCoincWinMainM10 = 510;  //ns
  constexpr static const Double_t kdSmxErrCoincWinMainM11 = 570;  //ns
    /// Coincidence windows 99.9% tagging (up to 0.1% of corruption not detected)
  constexpr static const Double_t kdSmxErrCoincWinNoiseM07 = 390;  //ns
  constexpr static const Double_t kdSmxErrCoincWinNoiseM08 = 460;  //ns
  constexpr static const Double_t kdSmxErrCoincWinNoiseM09 = 540;  //ns
  constexpr static const Double_t kdSmxErrCoincWinNoiseM10 = 600;  //ns
  constexpr static const Double_t kdSmxErrCoincWinNoiseM11 = 660;  //ns
    /// Tagging variables
  Double_t fdSmxErrCoincWinM07;
  Double_t fdSmxErrCoincWinM08;
  Double_t fdSmxErrCoincWinM09;
  Double_t fdSmxErrCoincWinM10;
  Double_t fdSmxErrCoincWinM11;
  std::vector<std::vector<std::vector<Double_t>>>
    fvdSmxErrTimeLastHits;                                   //! [ NbFebs ][ NbSmxPerFeb ][ kuSmxErrCoincWinNbHits ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxFirstHitM07;  //! [ NbFebs ][ NbSmxPerFeb ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxFirstHitM08;  //! [ NbFebs ][ NbSmxPerFeb ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxFirstHitM09;  //! [ NbFebs ][ NbSmxPerFeb ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxFirstHitM10;  //! [ NbFebs ][ NbSmxPerFeb ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxFirstHitM11;  //! [ NbFebs ][ NbSmxPerFeb ]
  std::vector<std::vector<UInt_t>> fvuSmxErrIdxLastHit;
  /// Methods
  void SmxErrInitializeVariables();
  Bool_t SmxErrCheckCoinc(UInt_t uFebIdx, UInt_t uAsicIdx, Double_t dNewHitTime);
  /// Histograms
  std::vector<TProfile*> fhStsFebSmxErrRatioEvo;
  std::vector<TProfile2D*> fhStsFebSmxErrRatioEvoAsic;
  std::vector<TProfile*> fhStsFebSmxErrRatioCopyEvo;
  std::vector<TProfile2D*> fhStsFebSmxErrRatioCopyEvoAsic;
  std::vector<TProfile*> fhStsFebSmxErrRatioCopySameAdcEvo;
  std::vector<TProfile2D*> fhStsFebSmxErrRatioCopySameAdcEvoAsic;


  TCanvas* fcMsSizeAll;
  TH1* fhMsSz[kiMaxNbFlibLinks];
  TProfile* fhMsSzTime[kiMaxNbFlibLinks];
  /*
   /// Pulser time difference histos
   Double_t                          fdStartTs; //! Last hits in each ASIC
   std::vector< stsxyter::FinalHit > fvmLastHitAsic; //! Last hits in each ASIC
   std::vector<TH2 *>                fhStsTimeDiffPerAsic;
   std::vector< std::vector<TH1 *> > fhStsTimeDiffPerAsicPair;
   std::vector< std::vector<TH1 *> > fhStsTimeDiffClkPerAsicPair;
   std::vector< std::vector<TH2 *> > fhStsTimeDiffEvoPerAsicPair;
   std::vector< std::vector<TProfile *> > fhStsTimeDiffEvoPerAsicPairProf;
   std::vector< std::vector<TProfile *> > fhStsRawTimeDiffEvoPerAsicPairProf;
   std::vector< std::vector<TH2 *> > fhStsTsLsbMatchPerAsicPair;
   std::vector< std::vector<TH2 *> > fhStsTsMsbMatchPerAsicPair;
   std::vector<TH1 *>                fhStsIntervalAsic;
   std::vector<TH1 *>                fhStsIntervalLongAsic;

   static const UInt_t kuNbValuesForTimeDiffMean = 100;
   constexpr static const double kdPulserPeriod = 154000; /// ns
   constexpr static const double kdTimeDiffToMeanMargin = 50;
   std::vector< std::vector< std::vector< Double_t > > > fvdLastTimeDiffValuesAsicPair;
   std::vector< std::vector< UInt_t > >  fvuLastTimeDiffSlotAsicPair;
   std::vector< std::vector< Double_t > > fvdMeanTimeDiffAsicPair;

   void UpdatePairMeanValue( UInt_t uAsicA, UInt_t uAsicB, Double_t dNewValue );
*/

  // Methods later going into Algo
  Bool_t InitStsParameters();
  void CreateHistograms();
  Bool_t ProcessStsMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx);

  void FillHitInfo(stsxyter::Message mess, const UShort_t& usElinkIdx, const UInt_t& uAsicIdx, const UInt_t& uMsIdx);
  void FillTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillEpochInfo(stsxyter::Message mess);

  void ResetStsHistos();
  void SaveStsHistos(TString sFileName = "");
  Bool_t ScanForNoisyChannels(Double_t dNoiseThreshold = 1e3);

  CbmMcbm2018MonitorMuch(const CbmMcbm2018MonitorMuch&);
  CbmMcbm2018MonitorMuch operator=(const CbmMcbm2018MonitorMuch&);

  ClassDef(CbmMcbm2018MonitorMuch, 1)
};

#endif  // CBMMCBM2018MONITORMUCH_H
