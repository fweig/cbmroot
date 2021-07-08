/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoPsd                        -----
// -----              Created 26.09.2019 by N.Karpushkin                   -----
// -----      based on CbmMcbm2018MonitorAlgoT0 by P.-A. Loizeau           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorAlgoPsd_H
#define CbmMcbm2018MonitorAlgoPsd_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmPsdDigi.h"

#include "PronyFitter.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"
// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <map>
#include <numeric>
#include <vector>

class CbmMcbm2018PsdPar;
class TH1;
class TH2;
class TProfile;
class TGraph;

class CbmMcbm2018MonitorAlgoPsd : public CbmStar2019Algo<CbmPsdDigi> {
public:
  CbmMcbm2018MonitorAlgoPsd();
  ~CbmMcbm2018MonitorAlgoPsd();

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
  inline void SetMonitorChanMode(Bool_t bFlagIn = kTRUE) { fbMonitorChanMode = bFlagIn; }
  inline void SetMonitorWfmMode(Bool_t bFlagIn = kTRUE) { fbMonitorWfmMode = bFlagIn; }
  inline void SetMonitorFitMode(Bool_t bFlagIn = kTRUE) { fbMonitorFitMode = bFlagIn; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetChargeHistoArgs(std::vector<Int_t> inVec)
  {
    fviHistoChargeArgs = inVec;
    kvuWfmRanges.clear();
    for (uint8_t i = 0; i <= kuNbWfmRanges; ++i)
      kvuWfmRanges.push_back(fviHistoChargeArgs.at(1)
                             + i * (fviHistoChargeArgs.at(2) - fviHistoChargeArgs.at(1)) / kuNbWfmRanges);
  }
  inline void SetAmplHistoArgs(std::vector<Int_t> inVec) { fviHistoAmplArgs = inVec; }
  inline void SetZLHistoArgs(std::vector<Int_t> inVec) { fviHistoZLArgs = inVec; }


private:
  /// Control flags
  Bool_t fbMonitorMode                    = kTRUE;   //! Switch ON the filling of a minimal set of histograms
  Bool_t fbMonitorChanMode                = kFALSE;  //! Switch ON the filling channelwise histograms
  Bool_t fbMonitorWfmMode                 = kFALSE;  //! Switch ON the filling waveforms histograms
  Bool_t fbMonitorFitMode                 = kFALSE;  //! Switch ON the filling waveform fitting histograms
  Bool_t fbDebugMonitorMode               = kFALSE;  //! Switch ON the filling of a additional set of histograms
  Bool_t fbFirstPackageError              = kTRUE;
  Bool_t fbPsdMissedData                  = kFALSE;
  std::vector<Bool_t> fvbMaskedComponents = {};

  /// Settings from parameter file
  CbmMcbm2018PsdPar* fUnpackPar = nullptr;        //!
  UInt_t fuRawDataVersion       = 0;              //! Raw data versioning
                                                  /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs                       = 0;   //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap = {};  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb                  = 0;   //! Number of FEBs per GDPB
  UInt_t fuNrOfChannelsPerFee              = 0;   //! Number of channels in each FEE
  UInt_t fuNrOfChannelsPerGdpb             = 0;   //! Number of channels per GDPB

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanPsd       = 12;
  const float kfAdc_to_mV               = 16.5;

  static constexpr UInt_t GetNbChanPsd() { return kuNbChanPsd; }
  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx = 0;
  ULong64_t fulCurrentMsIdx = 0;
  Double_t fdTsStartTime    = -1.0;  //! Time in ns of current TS from the index of the first MS first component
  Double_t fdMsTime         = -1.0;  //! Start Time in ns of current MS from its index field in header
  Double_t fdPrevMsTime     = -1.0;  //! Start Time in ns of previous MS from its index field in header
  UInt_t fuMsIndex          = 0;     //! Index of current MS within the TS

  /// Current data properties
  UInt_t fuCurrentEquipmentId = 0;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId          = 0;  //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx         = 0;  //! Index of the DPB from which the MS currently unpacked is coming

  /// Starting state book-keeping
  Double_t fdStartTime = -1.0; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  std::chrono::steady_clock::time_point ftStartTimeUnix = std::chrono::steady_clock::
    now(); /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Histograms related variables
  UInt_t fuHistoryHistoSize             = 3600;                     /** Size in seconds of the evolution histograms **/
  std::vector<Int_t> fviHistoChargeArgs = std::vector<Int_t>(3, 0); /** Charge histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoAmplArgs   = std::vector<Int_t>(3, 0); /** Amplitude histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoZLArgs     = std::vector<Int_t>(3, 0); /** ZeroLevel histogram arguments in adc counts **/

  /// Histograms
  UInt_t fuMsgsCntInMs     = 0;
  UInt_t fuReadMsgsCntInMs = 0;
  UInt_t fuLostMsgsCntInMs = 0;
  UInt_t fuReadEvtCntInMs  = 0;

  /// Channel rate plots
  std::vector<TH2*> fvhHitZLChanEvo       = std::vector<TH2*>(kuNbChanPsd, nullptr);
  std::vector<TH2*> fvhHitLPChanEvo       = std::vector<TH2*>(kuNbChanPsd, nullptr);
  std::vector<TH2*> fvhHitFAChanEvo       = std::vector<TH2*>(kuNbChanPsd, nullptr);
  std::vector<TH1*> fvhHitChargeChan      = std::vector<TH1*>(kuNbChanPsd, nullptr);
  std::vector<TH1*> fvhHitZeroLevelChan   = std::vector<TH1*>(kuNbChanPsd, nullptr);
  std::vector<TH1*> fvhHitAmplChan        = std::vector<TH1*>(kuNbChanPsd, nullptr);
  std::vector<TH1*> fvhHitChargeByWfmChan = std::vector<TH1*>(kuNbChanPsd, nullptr);
  std::vector<TH1*> fvhHitWfmChan         = std::vector<TH1*>(kuNbChanPsd, nullptr);

  static const UInt_t kuNbWfmRanges   = 8;
  static const UInt_t kuNbWfmExamples = 8;
  std::vector<UInt_t> kvuWfmRanges              = std::vector<UInt_t>(kuNbWfmRanges, 0);
  std::vector<UInt_t> kvuWfmInRangeToChangeChan = std::vector<UInt_t>(kuNbChanPsd * kuNbWfmRanges, 0);
  std::vector<TH1*> fv3hHitWfmFlattenedChan     = std::vector<TH1*>(kuNbChanPsd * kuNbWfmRanges * kuNbWfmExamples, 0);

  /// Channels map
  Bool_t fbSpillOn                         = kTRUE;
  UInt_t fuCurrentSpillIdx                 = 0;
  UInt_t fuCurrentSpillPlot                = 0;
  Double_t fdStartTimeSpill                = -1.0;
  Double_t fdLastSecondTime                = -1.0;
  UInt_t fuCountsLastSecond                = 0;
  static const UInt_t kuOffSpillCountLimit = 200;

  const UInt_t kuPsdChanMap[kuNbChanPsd] = {
    0};  // = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; //! Map from electronics channel to PSD physical channel
  TH1* fhHitChargeMap  = nullptr;
  TH1* fhHitMapEvo     = nullptr;
  TH2* fhChanHitMapEvo = nullptr;

  /// Global Rate
  TH1* fhMissedData  = nullptr;
  TH1* fhAdcTime     = nullptr;
  TH2* fhMsLengthEvo = nullptr;

  TH2* fhMsgsCntPerMsEvo     = nullptr;
  TH2* fhReadMsgsCntPerMsEvo = nullptr;
  TH2* fhLostMsgsCntPerMsEvo = nullptr;
  TH2* fhReadEvtsCntPerMsEvo = nullptr;

  /// Waveform fitting
  std::vector<TH1*> fvhHitFitWfmChan    = std::vector<TH1*>(kuNbChanPsd, nullptr);
  std::vector<TH2*> fvhFitHarmonic1Chan = std::vector<TH2*>(kuNbChanPsd, nullptr);
  std::vector<TH2*> fvhFitHarmonic2Chan = std::vector<TH2*>(kuNbChanPsd, nullptr);
  std::vector<TH2*> fvhFitQaChan        = std::vector<TH2*>(kuNbChanPsd, nullptr);

  /// Canvases
  TCanvas* fcSummary                = nullptr;
  TCanvas* fcHitMaps                = nullptr;
  TCanvas* fcZLevo                  = nullptr;
  TCanvas* fcChargesFPGA            = nullptr;
  TCanvas* fcChargesWfm             = nullptr;
  TCanvas* fcAmplitudes             = nullptr;
  TCanvas* fcZeroLevels             = nullptr;
  TCanvas* fcGenCntsPerMs           = nullptr;
  TCanvas* fcWfmsAllChannels        = nullptr;
  std::vector<TCanvas*> fvcWfmsChan = std::vector<TCanvas*>(kuNbChanPsd, nullptr);
  TCanvas* fcPronyFit               = nullptr;

  CbmMcbm2018MonitorAlgoPsd(const CbmMcbm2018MonitorAlgoPsd&);
  CbmMcbm2018MonitorAlgoPsd operator=(const CbmMcbm2018MonitorAlgoPsd&);

  ClassDef(CbmMcbm2018MonitorAlgoPsd, 1)
};

#endif
