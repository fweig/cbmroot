// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMuchLite                        -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                Modified 07/12/18  by Ajit Kumar                 -----
// -----                Modified 05/03/19  by Vikas Singhal               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MonitorMuchLite_H
#define CBMMCBM2018MonitorMuchLite_H

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
//class CbmCern2017UnpackParSts;

class CbmMcbm2018MonitorMuchLite : public CbmMcbmUnpack {
public:
  CbmMcbm2018MonitorMuchLite();
  virtual ~CbmMcbm2018MonitorMuchLite();

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
  void SaveHistos(TString sFileName = "");
  void SetHistoFileName(TString sFileName = "data/SetupHistos.root") {
    fsHistoFileFullname = sFileName;
  }

  void SetPrintMessage(Bool_t bPrintMessOn = kTRUE,
                       stsxyter::MessagePrintMask ctrl =
                         stsxyter::MessagePrintMask::msg_print_Hex
                         | stsxyter::MessagePrintMask::msg_print_Human) {
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
  void UseDaqBuffer(
    Bool_t) {};  //Virtual function in Mother Class, Need to update accordingly. VS

  /// => Quick and dirty hack for binning FW!!!
  void SetBinningFwFlag(Bool_t bEnable = kTRUE) { fbBinningFw = bEnable; }

private:
  // Parameters
  // Control flags
  Bool_t fbMuchMode;
  std::vector<Bool_t> fvbMaskedComponents;
  /// => Quick and dirty hack for binning FW!!!
  Bool_t fbBinningFw = kFALSE;

  // FLES containers
  std::vector<size_t> fvMsComponentsList;  //!
  size_t fuNbCoreMsPerTs;                  //!
  size_t fuNbOverMsPerTs;                  //!
  Bool_t
    fbIgnoreOverlapMs;  //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
                        // Unpacking and mapping
  CbmMcbm2018MuchPar* fUnpackParMuch;  //!
  UInt_t fuNrOfDpbs;                   //! Total number of STS DPBs in system
  std::map<UInt_t, UInt_t>
    fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  std::vector<std::vector<Bool_t>>
    fvbCrobActiveFlag;  //! Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  UInt_t fuNbFebs;       //! Number of StsXyter ASICs
  UInt_t fuNbStsXyters;  //! Number of StsXyter ASICs
  //std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcGain;  //! ADC gain in e-/b, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]
  //std::vector< std::vector< std::vector< Double_t > > > fvdFebAdcOffs;  //! ADC offset in e-, [ NbDpb ][ NbCrobPerDpb ][ NbFebsPerCrob ]

  // Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 4;

  /// Internal Control/status of monitor
  /// Histo File name and path
  TString fsHistoFileFullname;
  /// Task configuration values
  Bool_t fbPrintMessages;
  stsxyter::MessagePrintMask fPrintMessCtrl;
  // Bool_t                fbEnableCoincidenceMaps;
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  /// Current data properties
  std::map<stsxyter::MessType, UInt_t> fmMsgCounter;
  UInt_t
    fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t
    fuCurrDpbId;  //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t
    fuCurrDpbIdx;  //! Index of the DPB from which the MS currently unpacked is coming
  Int_t
    fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;  //! Bin size in s for the plots with date as X axis

  /// Data format control
  std::vector<ULong64_t> fvulCurrentTsMsb;   //! Current TS MSB for each DPB
  std::vector<UInt_t> fvuCurrentTsMsbCycle;  //! Current TS MSB cycle for DPB
  std::vector<UInt_t>
    fvuInitialHeaderDone;  //! Flag set after seeing MS header in 1st MS for DPB
  std::vector<UInt_t>
    fvuInitialTsMsbCycleHeader;  //! TS MSB cycle from MS header in 1st MS for DPB
  std::vector<UInt_t> fvuElinkLastTsHit;  //! TS from last hit for DPB
                                          /// Hits comparison
  std::vector<std::vector<ULong64_t>>
    fvulChanLastHitTime;  //! Last hit time in bins for each Channel
  //std::vector< std::vector< ULong64_t  > > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
  std::vector<std::vector<Double_t>>
    fvdChanLastHitTime;  //! Last hit time in ns   for each Channel
  std::vector<Double_t> fvdPrevMsTime;  //! Header time of previous MS per link
  std::vector<Double_t> fvdMsTime;      //! Header time of each MS
  std::vector<std::vector<std::vector<UInt_t>>>
    fvuChanNbHitsInMs;  //! Number of hits in each MS for each Channel
  std::vector<std::vector<std::vector<Double_t>>>
    fvdChanLastHitTimeInMs;  //! Last hit time in bins in each MS for each Channel
  std::vector<std::vector<std::vector<UShort_t>>>
    fvusChanLastHitAdcInMs;  //! Last hit ADC in bins in each MS for each Channel
  //   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
  /// Starting state book-keeping
  //Long64_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Long64_t prevtime_new; /** previous time for consecutive hit time**/
  Long64_t prevTime;     /** previous time for consecutive hit time**/
  UInt_t prevAsic;       /** previous asic**/
  UInt_t prevChan;       /** previous channel**/
  Double_t
    fdStartTime; /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t
    fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
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
  TH1* fhMsSz[kiMaxNbFlibLinks];
  TProfile* fhMsSzTime[kiMaxNbFlibLinks];

  // Methods later going into Algo
  Bool_t InitMuchParameters();
  void CreateHistograms();
  Bool_t
  ProcessMuchMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx);

  void FillHitInfo(stsxyter::Message mess,
                   const UShort_t& usElinkIdx,
                   const UInt_t& uAsicIdx,
                   const UInt_t& uMsIdx);
  void
  FillTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
  void FillEpochInfo(stsxyter::Message mess);

  void ResetMuchHistos();
  void SaveMuchHistos(TString sFileName = "");
  Bool_t ScanForNoisyChannels(Double_t dNoiseThreshold = 1e3);

  CbmMcbm2018MonitorMuchLite(const CbmMcbm2018MonitorMuchLite&);
  CbmMcbm2018MonitorMuchLite operator=(const CbmMcbm2018MonitorMuchLite&);

  ClassDef(CbmMcbm2018MonitorMuchLite, 1)
};

#endif  // CBMMCBM2018MonitorMuchLite_H
