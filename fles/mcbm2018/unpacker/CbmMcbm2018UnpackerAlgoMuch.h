// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018UnpackerAlgoMuch                      -----
// -----               Created 02.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018UnpackerAlgoMuch_H
#define CbmMcbm2018UnpackerAlgoMuch_H

#include "CbmStar2019Algo.h"

// Data
#include "CbmMuchBeamTimeDigi.h"
#include "StsXyterFinalHit.h"
#include "StsXyterMessage.h"

// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018MuchPar;
/*
class TCanvas;
class THttpServer;
*/
class TH1;
class TH2;
class TProfile;

class CbmMcbm2018UnpackerAlgoMuch :
  public CbmStar2019Algo<CbmMuchBeamTimeDigi> {
public:
  CbmMcbm2018UnpackerAlgoMuch();
  ~CbmMcbm2018UnpackerAlgoMuch();

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  Bool_t ProcessTs(const fles::Timeslice& ts);
  Bool_t ProcessTs(const fles::Timeslice& ts, size_t /*component*/) {
    return ProcessTs(ts);
  }
  Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx);

  void AddMsComponentToList(size_t component, UShort_t usDetectorId);

  Bool_t CreateHistograms();
  Bool_t FillHistograms();
  Bool_t ResetHistograms();

  inline void SetMonitorMode(Bool_t bFlagIn = kTRUE) {
    fbMonitorMode = bFlagIn;
  }
  inline void SetTimeOffsetNs(Double_t dOffsetIn = 0.0) {
    fdTimeOffsetNs = dOffsetIn;
  }
  void SetTimeOffsetNsAsic(UInt_t uAsicIdx, Double_t dOffsetIn = 0.0);
  void MaskNoisyChannel(UInt_t uFeb, UInt_t uChan, Bool_t bMasked = kTRUE);
  void EnableAsicType(Int_t flag = 0) { fiFlag = flag; }

  /// => Quick and dirty hack for binning FW!!!
  void SetBinningFwFlag(Bool_t bEnable = kTRUE) { fbBinningFw = bEnable; }

  inline void SetVectCapInc(Double_t dIncFact) {
    fdCapacityIncFactor = dIncFact;
  }

  void SetAdcCut(UInt_t uAdc) { fdAdcCut = uAdc; }


  CbmMuchBeamTimeDigi* CreateMuchDigi(stsxyter::FinalHit*);


private:
  /// Control flags
  Bool_t
    fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms
  Bool_t
    fbDebugMonitorMode;  //! Switch ON the filling of a additional set of histograms
  std::vector<Bool_t> fvbMaskedComponents;
  /// => Quick and dirty hack for binning FW!!!
  Bool_t fbBinningFw = kFALSE;

  Int_t
    fiFlag;  //! Switch to smx2.0/smx2.1 data-> fiFlag = 0 for 2.0 and fiFlag = 1 for 2.1

  /// Settings from parameter file
  CbmMcbm2018MuchPar* fUnpackPar;  //!
                                   /// Readout chain dimensions and mapping
  UInt_t fuNrOfDpbs;               //! Total number of STS DPBs in system
  std::map<UInt_t, UInt_t>
    fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
  std::vector<std::vector<Bool_t>>
    fvbCrobActiveFlag;  //! Array to hold the active flag for all CROBs, [ NbDpb ][ NbCrobPerDpb ]
  UInt_t fuNbFebs;       //! Number of FEBs with StsXyter ASICs
  UInt_t fuNbStsXyters;  //! Number of StsXyter ASICs

  /// User settings: Data correction parameters
  Double_t fdTimeOffsetNs;
  std::vector<Double_t> fvdTimeOffsetNsAsics;
  //std::vector< Int_t >     fviFebAddress;  //! Much address for each FEB, [ NbDpb * NbCrobPerDpb * NbFebsPerCrob ]

  Bool_t fbUseChannelMask;
  std::vector<std::vector<bool>>
    fvvbMaskedChannels;  //! Vector of channel masks, [ NbFeb ][ NbCHanInFeb ], used only if fbUseChannelMask is true
  UInt_t fdAdcCut;
  /// Constants
  static const Int_t kiMaxNbFlibLinks = 32;

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  Double_t
    fdTsStartTime;  //! Time in ns of current TS from the index of the first MS first component
  Double_t
    fdTsStopTimeCore;  //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t
    fdMsTime;  //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;  //! Index of current MS within the TS
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
                                             /// Starting state book-keeping
  Double_t
    fdStartTime; /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
  Double_t
    fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// Hits time-sorting
  std::vector<stsxyter::FinalHit>
    fvmHitsInMs;  //! All hits (time in bins, ADC in bins, asic, channel) in last MS, sorted with "<" operator

  /// Duplicate hits suppression
  static const UInt_t kuMaxTsMsbDiffDuplicates =
    32;  //! Limit how many different TS_MSB are checked for same duplicate hit => set to 1 uS
  std::vector<std::vector<UShort_t>>
    fvvusLastTsChan;  //! TS of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<UShort_t>>
    fvvusLastAdcChan;  //! ADC of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<UShort_t>>
    fvvusLastTsMsbChan;  //! TS MSB of last hit message for each channel, [ AsicIdx ][ Chan ]
  std::vector<std::vector<UShort_t>>
    fvvusLastTsMsbCycleChan;  //! TS MSB cycle of last hit message for each channel, [ AsicIdx ][ Chan ]

  /// Histograms
  TH1* fhDigisTimeInRun;  //!
                          /*
      std::vector< TH1* > fvhHitsTimeToTriggerRaw;       //! [sector]
      std::vector< TH1* > fvhMessDistributionInMs;       //! [sector], extra monitor for debug
      TH1 *               fhEventNbPerTs;                //!
      TCanvas *           fcTimeToTrigRaw;               //! All sectors
*/
  TH1* fhVectorSize            = nullptr;
  TH1* fhVectorCapacity        = nullptr;
  size_t fuTsMaxVectorSize     = 0;
  Double_t fdCapacityIncFactor = 1.1;

  void ProcessHitInfo(const stsxyter::Message& mess,
                      const UShort_t& usElinkIdx,
                      const UInt_t& uAsicIdx,
                      const UInt_t& uMsIdx);
  void ProcessTsMsbInfo(const stsxyter::Message& mess,
                        UInt_t uMessIdx = 0,
                        UInt_t uMsIdx   = 0);
  void ProcessEpochInfo(const stsxyter::Message& mess);
  void ProcessStatusInfo(const stsxyter::Message& mess, const UInt_t& uAsicIdx);

  CbmMcbm2018UnpackerAlgoMuch(const CbmMcbm2018UnpackerAlgoMuch&);
  CbmMcbm2018UnpackerAlgoMuch operator=(const CbmMcbm2018UnpackerAlgoMuch&);

  ClassDef(CbmMcbm2018UnpackerAlgoMuch, 1)
};

#endif
