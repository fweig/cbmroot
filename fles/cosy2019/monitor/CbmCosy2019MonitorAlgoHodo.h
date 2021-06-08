/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: P.-A. Loizeau, Florian Uhlig [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmCosy2019MonitorAlgoHodo                       -----
// -----               Created 03.07.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmCosy2019MonitorAlgoHodo_H
#define CbmCosy2019MonitorAlgoHodo_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmStsDigi.h"

#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot

// ROOT
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

// C++11
#include <chrono>

// C/C++
#include <map>
#include <vector>

class TProfile;

class CbmCosy2019MonitorAlgoHodo : public CbmStar2019Algo<CbmStsDigi> {
public:
  CbmCosy2019MonitorAlgoHodo();
  ~CbmCosy2019MonitorAlgoHodo();

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

  inline void SetDpbId(UInt_t uDpbId = 0x5b75) { fuDpbId = uDpbId; }
  inline void SetHodoElinkIdx(UInt_t uElinkHodoA = 5, UInt_t uElinkHodoB = 10)
  {
    fvuElinkIdxHodo[0] = uElinkHodoA;
    fvuElinkIdxHodo[1] = uElinkHodoB;
  }  /// Default set for mMUCH FMC slots 8 and 9
  inline void SetHodoSwapXY(Bool_t bSwapHodoA = kFALSE, Bool_t bSwapHodoB = kTRUE)
  {
    fvbHodoSwapXY[0] = bSwapHodoA;
    fvbHodoSwapXY[1] = bSwapHodoB;
  }  /// Default set closest cosmic setup stack
  inline void SetHodoInvertX(Bool_t bInvHodoA = kFALSE, Bool_t bInvHodoB = kTRUE)
  {
    fvbHodoInvertX[0] = bInvHodoA;
    fvbHodoInvertX[1] = bInvHodoB;
  }  /// Default set closest cosmic setup stack
  inline void SetHodoInvertY(Bool_t bInvHodoA = kFALSE, Bool_t bInvHodoB = kTRUE)
  {
    fvbHodoInvertY[0] = bInvHodoA;
    fvbHodoInvertY[1] = bInvHodoB;
  }  /// Default set closest cosmic setup stack

private:
  /// Control flags
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents;

  /// Settings from parameter file => For now use only accessors!
  /*
      CbmCosy2019HodoPar* fUnpackPar;      //!
         /// Readout chain dimensions and mapping
      UInt_t                   fuNbModules;       //! Total number of STS modules in the setup
      std::vector< Int_t    >  fviModuleType;     //! Type of each module: 0 for connectors on the right, 1 for connectors on the left
      std::vector< Int_t    >  fviModAddress;     //! STS address for the first strip of each module
      UInt_t                   fuNrOfDpbs;        //! Total number of STS DPBs in system
      std::map<UInt_t, UInt_t> fDpbIdIndexMap;    //! Map of DPB Identifier to DPB index
      std::vector< std::vector< Bool_t > > fvbCrobActiveFlag; //! Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
      UInt_t                   fuNbFebs;          //! Number of FEBs with HodoXyter ASICs
      UInt_t                   fuNbStsXyters;     //! Number of StsXyter ASICs
      std::vector< std::vector< std::vector< Int_t > > > fviFebModuleIdx;   //! Idx of the STS module for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], -1 if inactive
      std::vector< std::vector< std::vector< Int_t > > > fviFebModuleSide;  //! STS module side for each FEB, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = P, 1 = N, -1 if inactive
      std::vector< std::vector< std::vector< Int_t > > > fviFebType;  //! FEB type, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ], 0 = A, 1 = B, -1 if inactive
      std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcGain;  //! ADC gain in e-/b, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]
      std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcOffs;  //! ADC offset in e-, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]
      std::vector< Int_t >     fviFebAddress;  //! STS address for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]
*/
  UInt_t fuDpbId;
  std::vector<UInt_t> fvuElinkIdxHodo;
  std::vector<Bool_t> fvbHodoSwapXY;
  std::vector<Bool_t> fvbHodoInvertX;
  std::vector<Bool_t> fvbHodoInvertY;
  std::vector<UInt_t> fvuAdcGainHodo;
  std::vector<UInt_t> fvuAdcOffsHodo;

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 4;
  static const UInt_t kuNbElinksDpb     = 2 * 42;  //! Max Nb of eLinks for this sDPB: 2 GBTx with 42 eLinks each
  static const UInt_t kuNbHodos         = 2;       //! Nb of hodoscopes = Nb FEBs = Nb ASICs
  static const UInt_t kuNbAxis          = 2;       //! Per hodoscope = X and Y
  static const UInt_t kuNbChanPerAsic   = 128;
  /*
      const UInt_t   kuChannelToFiberMap[ kuNbChanPerAsic ] = {
            0, 32, 16, 48, 15, 47, 31, 63,  1, 33, 17, 49, 14, 46, 30, 62,
            2, 34, 18, 50, 13, 45, 29, 61,  3, 35, 19, 51, 12, 44, 28, 60,
            4, 36, 20, 52, 11, 43, 27, 59,  5, 37, 21, 53, 10, 42, 26, 58,
            6, 38, 22, 54,  9, 41, 25, 57,  7, 39, 23, 55,  8, 40, 24, 56,
            0, 32, 16, 48, 15, 47, 31, 63,  1, 33, 17, 49, 14, 46, 30, 62,
            2, 34, 18, 50, 13, 45, 29, 61,  3, 35, 19, 51, 12, 44, 28, 60,
            4, 36, 20, 52, 11, 43, 27, 59,  5, 37, 21, 53, 10, 42, 26, 58,
            6, 38, 22, 54,  9, 41, 25, 57,  7, 39, 23, 55,  8, 40, 24, 56
         }; //! Map from channel index to Hodoscope Fiber
*/
  const UInt_t kuChannelToFiberMap[kuNbChanPerAsic] = {32, 0,  48, 16, 47, 15, 63, 31, 33, 1,  49, 17, 46, 14, 62,
                                                       30, 34, 2,  50, 18, 45, 13, 61, 29, 35, 3,  51, 19, 44, 12,
                                                       60, 28, 36, 4,  52, 20, 43, 11, 59, 27, 37, 5,  53, 21, 42,
                                                       10, 58, 26, 38, 6,  54, 22, 41, 9,  57, 25, 39, 7,  55, 23,
                                                       40, 8,  56, 24, 32, 0,  48, 16, 47, 15, 63, 31, 33, 1,  49,
                                                       17, 46, 14, 62, 30, 34, 2,  50, 18, 45, 13, 61, 29, 35, 3,
                                                       51, 19, 44, 12, 60, 28, 36, 4,  52, 20, 43, 11, 59, 27, 37,
                                                       5,  53, 21, 42, 10, 58, 26, 38, 6,  54, 22, 41, 9,  57, 25,
                                                       39, 7,  55, 23, 40, 8,  56, 24};  //! Map from channel index to Hodoscope Fiber
  const UInt_t kuChannelToPixelMap[kuNbChanPerAsic] = {1,  5,  3,  7,  2,  6,  4,  8,  9,  13, 11, 15, 10, 14, 12,
                                                       16, 17, 21, 19, 23, 18, 22, 20, 24, 25, 29, 27, 31, 26, 30,
                                                       28, 32, 33, 37, 35, 39, 34, 38, 36, 40, 41, 45, 43, 47, 42,
                                                       46, 44, 48, 49, 53, 51, 55, 50, 54, 52, 56, 57, 61, 59, 63,
                                                       58, 62, 60, 64, 1,  5,  3,  7,  2,  6,  4,  8,  9,  13, 11,
                                                       15, 10, 14, 12, 16, 17, 21, 19, 23, 18, 22, 20, 24, 25, 29,
                                                       27, 31, 26, 30, 28, 32, 33, 37, 35, 39, 34, 38, 36, 40, 41,
                                                       45, 43, 47, 42, 46, 44, 48, 49, 53, 51, 55, 50, 54, 52, 56,
                                                       57, 61, 59, 63, 58, 62, 60, 64};  //! Map from channel index to PMT pixel
  const UInt_t kuChannelToPlaneMap[kuNbChanPerAsic] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  //! Map from channel index to Hodoscope Axis (X or Y)

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  Double_t fdTsStartTime;     //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;  //! End Time in ns of current TS Core from the index of the first MS first component
  std::vector<Double_t> fvdPrevMsTime;  //! Header time of previous MS per link
  Double_t fdMsTime;                    //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;                     //! Index of current MS within the TS
                                        /// Current data properties
  std::map<stsxyter::MessType, UInt_t> fmMsgCounter;
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  Int_t fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;     //! Bin size in s for the plots with date as X axis
                                /// Data format control
  ULong64_t fvulCurrentTsMsb;   //! Current TS MSB for each DPB
  UInt_t fvuCurrentTsMsbCycle;  //! Current TS MSB cycle for DPB
                                /// Starting state book-keeping
  Double_t fdStartTime;         /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz;     /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Hits time-sorting
  std::vector<stsxyter::FinalHit>
    fvmHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last MS, sorted with "<" operator

  /// Histograms
  /// General plots
  TH1* fhHodoMessType;
  TH2* fhHodoStatusMessType;
  TH2* fhHodoMsStatusFieldType;
  TH2* fhHodoMessTypePerElink;
  /// Raw = ASIC channels
  std::vector<TH1*> fhHodoChanCntRaw;                      //! [ Hodo ]
  std::vector<TH2*> fhHodoChanAdcRaw;                      //! [ Hodo ]
  std::vector<TProfile*> fhHodoChanAdcRawProf;             //! [ Hodo ]
  std::vector<TH2*> fhHodoChanAdcCal;                      //! [ Hodo ]
  std::vector<TProfile*> fhHodoChanAdcCalProf;             //! [ Hodo ]
  std::vector<TH2*> fhHodoChanRawTs;                       //! [ Hodo ]
  std::vector<TH2*> fhHodoChanMissEvt;                     //! [ Hodo ]
  std::vector<TH2*> fhHodoChanMissEvtEvo;                  //! [ Hodo ]
  std::vector<TH2*> fhHodoChanHitRateEvo;                  //! [ Hodo ]
  std::vector<TProfile*> fhHodoChanHitRateProf;            //! [ Hodo ]
  std::vector<TH2*> fhHodoChanDistT;                       //! [ Hodo ]
                                                           /// Remapped = Hodos fibers
  std::vector<std::vector<TH1*>> fhHodoFiberCnt;           //! [ Hodo ][ Axis ]
  std::vector<std::vector<TH2*>> fhHodoFiberAdc;           //! [ Hodo ][ Axis ]
  std::vector<std::vector<TProfile*>> fhHodoFiberAdcProf;  //! [ Hodo ][ Axis ]
  std::vector<std::vector<TH2*>> fhHodoFiberHitRateEvo;    //! [ Hodo ][ Axis ]
                                                           /// Clusters
  /// =====> Limits for clustering to be defined, maybe use instead STS SsdOrtho reconstruction class
  /// Coincidences
  Double_t fdTimeCoincLimit   = 100.0;                                                /// +/- in clock cycles
  Double_t fdTimeCoincLimitNs = stsxyter::kdClockCycleNs * (fdTimeCoincLimit + 0.5);  /// +/- in clock cycles
  std::vector<TH2*> fhHodoFiberCoincMapXY;                                            //! [ Hodo ]
  std::vector<TH1*> fhHodoFiberCoincTimeXY;                                           //! [ Hodo ]
  std::vector<TH2*> fhHodoFiberCoincWalkXY_X;                                         //! [ Hodo ]
  std::vector<TH2*> fhHodoFiberCoincWalkXY_Y;                                         //! [ Hodo ]
  std::vector<TH2*> fhHodoFiberCoincMapSameAB;                                        //! [ Axis ]
  std::vector<TH1*> fhHodoFiberCoincTimeSameAB;                                       //! [ Axis ]
  std::vector<TH2*> fhHodoFiberCoincMapDiffAB;                                        //! [ Axis ]
  std::vector<TH1*> fhHodoFiberCoincTimeDiffAB;                                       //! [ Axis ]
  TH2* fhHodoFullCoincPosA;                                                           //!
  TH2* fhHodoFullCoincPosB;                                                           //!
  TH2* fhHodoFullCoincCompX;                                                          //!
  TH2* fhHodoFullCoincCompY;                                                          //!
  TH2* fhHodoFullCoincResidualXY;                                                     //!
  TH1* fhHodoFullCoincTimeDiff;                                                       //!
  std::vector<std::vector<TH2*>> fhHodoFullCoincTimeWalk;                             //! [ Hodo ][ Axis ]
  TH1* fhHodoFullCoincRateEvo;                                                        //!
  std::vector<std::vector<TH2*>> fhHodoFullCoincPosEvo;                               //! [ Hodo ][ Axis ]
  /// Setup debugging
  TH1* fhPrevHitDtAllAsics;
  TH1* fhPrevHitDtAsicA;
  TH1* fhPrevHitDtAsicB;
  TH1* fhPrevHitDtAsicsAB;

  /// Histogramming variables
  /// Mean Rate per channel plots
  Int_t fiTimeIntervalRateUpdate;
  std::vector<Int_t> fviTimeSecLastRateUpdate;                          //! [ Hodo ]
  std::vector<std::vector<Double_t>> fvdChanCountsSinceLastRateUpdate;  //! [ Hodo ][ Chan ]
  std::vector<std::vector<Double_t>> fdHodoChanLastTimeForDist;         //! [ Hodo ][ Chan ]
  UInt_t fuPreviousHitAsic;
  std::vector<Double_t> fvdPreviousHitTimePerAsic;  //! [ Asic ]

  /// Canvases
  TCanvas* fcSummary;
  std::vector<TCanvas*> fcHodoSummaryRaw;
  std::vector<TCanvas*> fcHodoSummaryFiber;
  TCanvas* fcHodoFiberCoinc;
  TCanvas* fcHodoFiberCoincAB;
  TCanvas* fcHodoFullCoinc;
  TCanvas* fcHodoFullCoincPos;
  TCanvas* fcHodoFullCoincPosEvo;
  TCanvas* fcHodoPrevHitDt;

  void ProcessHitInfo(stsxyter::Message mess, const UInt_t& uHodoIdx, const UInt_t& uMsIdx);
  void ProcessTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void ProcessEpochInfo(stsxyter::Message mess);
  void ProcessStatusInfo(stsxyter::Message mess);

  CbmCosy2019MonitorAlgoHodo(const CbmCosy2019MonitorAlgoHodo&);
  CbmCosy2019MonitorAlgoHodo operator=(const CbmCosy2019MonitorAlgoHodo&);

  ClassDef(CbmCosy2019MonitorAlgoHodo, 1)
};

#endif
