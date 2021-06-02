/** @file CbmMcbm2018MonitorAlgoPsd.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors N.Karpushkin, David Emschermann [committer] **/

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
#include "PsdGbtReader.h"

// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <map>
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
  Bool_t fbMonitorMode;       //! Switch ON the filling of a minimal set of histograms
  Bool_t fbMonitorChanMode;   //! Switch ON the filling channelwise histograms
  Bool_t fbMonitorWfmMode;    //! Switch ON the filling waveforms histograms
  Bool_t fbMonitorFitMode;    //! Switch ON the filling waveform fitting histograms
  Bool_t fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents;
  Bool_t fbFirstPackageError;
  Bool_t fbPsdMissedData;

  /// Settings from parameter file
  CbmMcbm2018PsdPar* fUnpackPar;             //!
                                             /// Readout chain dimensions and mapping
  UInt_t fuNrOfGdpbs;                        //! Total number of GDPBs in the system
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;  //! gDPB ID to index map
  UInt_t fuNrOfFeePerGdpb;                   //! Number of FEBs per GDPB
  UInt_t fuNrOfChannelsPerFee;               //! Number of channels in each FEE
  UInt_t fuNrOfChannelsPerGdpb;              //! Number of channels per GDPB

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanPsd       = 10;

  static constexpr UInt_t GetNbChanPsd() { return kuNbChanPsd; }
  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  Double_t fdTsStartTime;  //! Time in ns of current TS from the index of the first MS first component
  Double_t fdMsTime;       //! Start Time in ns of current MS from its index field in header
  Double_t fdPrevMsTime;   //! Start Time in ns of previous MS from its index field in header
  UInt_t fuMsIndex;        //! Index of current MS within the TS

  /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming

  /// Starting state book-keeping
  Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Histograms related variables
  UInt_t fuHistoryHistoSize;             /** Size in seconds of the evolution histograms **/
  std::vector<Int_t> fviHistoChargeArgs; /** Charge histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoAmplArgs;   /** Amplitude histogram arguments in adc counts **/
  std::vector<Int_t> fviHistoZLArgs;     /** ZeroLevel histogram arguments in adc counts **/

  /// Histograms
  UInt_t fuMsgsCntInMs;
  UInt_t fuReadMsgsCntInMs;
  UInt_t fuLostMsgsCntInMs;
  UInt_t fuReadEvtCntInMs;

  /// Channel rate plots
  std::vector<UInt_t> fvuHitCntChanMs;
  std::vector<TH1*> fvhHitChargeChan;
  std::vector<TH1*> fvhHitZeroLevelChan;
  std::vector<TH1*> fvhHitAmplChan;
  std::vector<TH1*> fvhHitChargeByWfmChan;
  std::vector<TH1*> fvhHitWfmChan;

  static const UInt_t kuNbWfmRanges   = 8;
  static const UInt_t kuNbWfmExamples = 8;
  std::vector<UInt_t> kvuWfmRanges;
  std::vector<UInt_t> kvuWfmInRangeToChangeChan;
  std::vector<TH1*> fv3hHitWfmFlattenedChan;

  /// Channels map
  Bool_t fbSpillOn;
  UInt_t fuCurrentSpillIdx;
  UInt_t fuCurrentSpillPlot;
  Double_t fdStartTimeSpill;
  Double_t fdLastSecondTime;
  UInt_t fuCountsLastSecond;
  static const UInt_t kuOffSpillCountLimit = 200;

  const UInt_t kuPsdChanMap[kuNbChanPsd] = {
    0};  // = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; //! Map from electronics channel to PSD physical channel
  TH1* fhHitChargeMap;
  TH1* fhHitMapEvo;
  TH2* fhChanHitMapEvo;

  /// Global Rate
  TH1* fhMissedData;
  TH1* fhAdcTime;
  TH2* fhMsLengthEvo;

  TH2* fhMsgsCntPerMsEvo;
  TH2* fhReadMsgsCntPerMsEvo;
  TH2* fhLostMsgsCntPerMsEvo;
  TH2* fhReadEvtsCntPerMsEvo;

  /// Waveform fitting
  std::vector<TH1*> fvhHitFitWfmChan;
  std::vector<TH2*> fvhFitHarmonic1Chan;
  std::vector<TH2*> fvhFitHarmonic2Chan;
  std::vector<TH2*> fvhFitQaChan;

  /// Canvases
  TCanvas* fcSummary;
  TCanvas* fcHitMaps;
  TCanvas* fcChargesFPGA;
  TCanvas* fcChargesWfm;
  TCanvas* fcAmplitudes;
  TCanvas* fcZeroLevels;
  TCanvas* fcGenCntsPerMs;
  TCanvas* fcWfmsAllChannels;
  std::vector<TCanvas*> fvcWfmsChan;
  TCanvas* fcPronyFit;

  CbmMcbm2018MonitorAlgoPsd(const CbmMcbm2018MonitorAlgoPsd&);
  CbmMcbm2018MonitorAlgoPsd operator=(const CbmMcbm2018MonitorAlgoPsd&);

  ClassDef(CbmMcbm2018MonitorAlgoPsd, 1)
};

#endif
