// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoTof                         -----
// -----               Created 27.11.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorAlgoTof_H
#define CbmMcbm2018MonitorAlgoTof_H

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

class CbmMcbm2018TofPar;
class TH1;
class TH2;
class TProfile;
class TProfile2D;

class CbmMcbm2018MonitorAlgoTof : public CbmStar2019Algo<CbmTofDigi> {
public:
  CbmMcbm2018MonitorAlgoTof();
  ~CbmMcbm2018MonitorAlgoTof();

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
  void ResetEvolutionHistograms();

  inline void SetDebugMonitorMode(Bool_t bFlagIn = kTRUE) { fbDebugMonitorMode = bFlagIn; }
  inline void SetIgnoreCriticalErrors(Bool_t bFlagIn = kTRUE) { fbIgnoreCriticalErrors = bFlagIn; }
  inline void SetHistoryHistoSize(UInt_t inHistorySizeSec = 1800) { fuHistoryHistoSize = inHistorySizeSec; }
  inline void SetPulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulser = uMin;
    fuMaxTotPulser = uMax;
  }

  inline void SetGdpbIndex(Int_t iGdpb = -1) { fiGdpbIndex = iGdpb; }

  inline void UseAbsoluteTime(Bool_t bFlagIn = kTRUE) { fbUseAbsoluteTime = bFlagIn; }

private:
  /// Control flags
  Bool_t fbDebugMonitorMode;      //! Switch ON the filling of a additional set of histograms
  Bool_t fbIgnoreCriticalErrors;  //! If ON not printout at all for critical errors
  std::vector<Bool_t> fvbMaskedComponents;
  Int_t fiGdpbIndex;
  Bool_t fbUseAbsoluteTime = kFALSE;  //! Switch ON the usage of abolute time scale for the evo histo (no start time)

  /// Settings from parameter file
  CbmMcbm2018TofPar* fUnpackPar;             //!
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
  UInt_t fuMinTotPulser;
  UInt_t fuMaxTotPulser;

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 8;
  static const UInt_t kuNbChanDiamond   = 8;

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  uint32_t fuCurrentMsSysId;  //! SysId of the current MS in TS (0 to fuTotalMsNb)
  Double_t fdTsStartTime;     //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;  //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime;          //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;           //! Index of current MS within the TS
                              /// Current data properties
  std::map<gdpbv100::MessageTypes, UInt_t> fmMsgCounter;
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
  Int_t fiRunStartDateTimeSec;  //! Start of run time since "epoch" in s, for the plots with date as X axis
  Int_t fiBinSizeDatePlots;     //! Bin size in s for the plots with date as X axis
  UInt_t fuGet4Id;  //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
  /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
  std::vector<ULong64_t> fvulCurrentEpoch;       //! Current epoch index, per DPB
  std::vector<ULong64_t> fvulCurrentEpochCycle;  //! Epoch cycle from the Ms Start message and Epoch counter flip
  std::vector<ULong64_t> fvulCurrentEpochFull;   //! Epoch + Epoch Cycle

  /// Buffer for suppressed epoch processing
  std::vector<gdpbv100::Message> fvmEpSupprBuffer;

  /// Starting state book-keeping
  Double_t fdStartTime;     /** Time of first valid hit (epoch available), used as reference for evolution plots**/
  Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
  std::chrono::steady_clock::time_point
    ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

  /// STAR TRIGGER detection
  std::vector<ULong64_t> fvulGdpbTsMsb;
  std::vector<ULong64_t> fvulGdpbTsLsb;
  std::vector<ULong64_t> fvulStarTsMsb;
  std::vector<ULong64_t> fvulStarTsMid;
  std::vector<ULong64_t> fvulGdpbTsFullLast;
  std::vector<ULong64_t> fvulStarTsFullLast;
  std::vector<UInt_t> fvuStarTokenLast;
  std::vector<UInt_t> fvuStarDaqCmdLast;
  std::vector<UInt_t> fvuStarTrigCmdLast;

  /// Duplicate Hits detection
  Bool_t fbEpochSinceLastHit;
  UInt_t fuDuplicatesCount;
  gdpbv100::Message fmLastHit;

  /// Histograms related variables
  UInt_t fuHistoryHistoSize;  /// Size in seconds of the evolution histograms

  /// Histograms
  /// Global Counts
  /// In System
  TH1* fhMessType;                                   //!
  TH1* fhSysMessType;                                //!
                                                     /// Per GET4 in system
  TH2* fhGet4MessType;                               //!
  TH2* fhGet4ChanScm;                                //!
  TH2* fhGet4ChanErrors;                             //!
  TH2* fhGet4EpochFlags;                             //!
  TH2* fhGdpbAsicSpiCounts;                          //!
                                                     /// Per Gdpb
  TH2* fhGdpbMessType;                               //!
  TH2* fhGdpbSysMessType;                            //!
  TH2* fhGdpbSysMessPattType;                        //!
  TH2* fhGdpbEpochFlags;                             //!
  TH2* fhGdpbEpochSyncEvo;                           //!
  TH2* fhGdpbEpochMissEvo;                           //!
  TH1* fhGdpbEndMsBufferNotEmpty;                    //!
  TH2* fhGdpbEndMsDataLost;                          //!
  TH2* fhGdpbHitRate;                                //!
                                                     /// Per GET4 in gDPB
  std::vector<TH2*> fvhGdpbGet4MessType;             //!
  std::vector<TH2*> fvhGdpbGet4ChanScm;              //!
  std::vector<TH2*> fvhGdpbGet4ChanErrors;           //!
                                                     /// Global Rates
  TH1* fhMsgCntEvo;                                  //!
  TH1* fhHitCntEvo;                                  //!
  TH1* fhErrorCntEvo;                                //!
  TH1* fhLostEvtCntEvo;                              //!
  TProfile* fhErrorFractEvo;                         //!
  TProfile* fhLostEvtFractEvo;                       //!
  TH2* fhMsgCntPerMsEvo;                             //!
  TH2* fhHitCntPerMsEvo;                             //!
  TH2* fhErrorCntPerMsEvo;                           //!
  TH2* fhLostEvtCntPerMsEvo;                         //!
  TH2* fhErrorFractPerMsEvo;                         //!
  TH2* fhLostEvtFractPerMsEvo;                       //!
                                                     /// Raw data per channel
  std::vector<TH2*> fvhRawFt_gDPB;                   //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhRawCt_gDPB;                   //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhRemapTot_gDPB;                //!
  std::vector<TH1*> fvhRemapChCount_gDPB;            //!
  std::vector<TH2*> fvhRemapChRate_gDPB;             //!
  std::vector<TProfile2D*> fvhRemapChErrFract_gDPB;  //!
                                                     /// Pattern Messages
  /// Pattern messages per gDPB
  std::vector<UInt_t> fuNbMissmatchPattern;
  TH2* fhNbMissPatternPerMs;                                    //! Debug histo, only in DebugMonitorMode
  TH2* fhPatternMissmatch;                                      //! Debug histo, only in DebugMonitorMode
  TH2* fhPatternEnable;                                         //! Debug histo, only in DebugMonitorMode
  TH2* fhPatternResync;                                         //! Debug histo, only in DebugMonitorMode
                                                                /// Per MS in gDPB
  std::vector<TH2*> fvhGdpbPatternMissmatchEvo;                 //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbPatternEnableEvo;                    //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbPatternResyncEvo;                    //! Debug histo, only in DebugMonitorMode
                                                                /// State Per TS
  std::vector<std::vector<bool>> fvvbGdpbLastMissmatchPattern;  //! Exclude from dictionnary
  std::vector<std::vector<bool>> fvvbGdpbLastEnablePattern;     //! Exclude from dictionnary
  std::vector<std::vector<bool>> fvvbGdpbLastResyncPattern;     //! Exclude from dictionnary
  std::vector<TH2*> fvhGdpbMissmatchEvoPerTs;                   //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbMissmatchEnaEvoPerTs;                //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbEnableEvoPerTs;                      //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbResyncEvoPerTs;                      //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbResyncEnaEvoPerTs;                   //! Debug histo, only in DebugMonitorMode
  std::vector<TH2*> fvhGdpbStateEvoPerTs;                       //! Debug histo, only in DebugMonitorMode
                                                                /// STAR TRIGGER detection
  std::vector<TH1*> fvhTokenMsgType;                            //!
  std::vector<TH1*> fvhTriggerRate;                             //!
  std::vector<TH2*> fvhCmdDaqVsTrig;                            //!
  std::vector<TH2*> fvhStarTokenEvo;                            //!
  std::vector<TProfile*> fvhStarTrigGdpbTsEvo;                  //!
  std::vector<TProfile*> fvhStarTrigStarTsEvo;                  //!
                                                                //!

  /// Canvases
  TCanvas* fcSummary;                    //!
  TCanvas* fcSummaryGdpb;                //!
  std::vector<TCanvas*> fvcSumGdpbGet4;  //!
  TCanvas* fcStarTrigTokenType;          //!
  TCanvas* fcStarTriggerRate;            //!
  TCanvas* fcStarTrigCmdDaqVsTrig;       //!
  TCanvas* fcStarTrigStarTokenEvo;       //!
  TCanvas* fcStarTrigGdpbTsEvo;          //!
  TCanvas* fcStarTrigStarTsEvo;          //!

  void ProcessEpochCycle(uint64_t ulCycleData);
  void ProcessEpoch(gdpbv100::Message mess);

  void ProcessEpSupprBuffer();

  void ProcessHit(gdpbv100::FullMessage mess);

  void ProcessSysMess(gdpbv100::FullMessage mess);
  void ProcessError(gdpbv100::FullMessage mess);
  void ProcessPattern(gdpbv100::Message mess);

  void ProcessSlowCtrl(gdpbv100::Message mess);

  void ProcessStarTrig(gdpbv100::Message mess);

  CbmMcbm2018MonitorAlgoTof(const CbmMcbm2018MonitorAlgoTof&);
  CbmMcbm2018MonitorAlgoTof operator=(const CbmMcbm2018MonitorAlgoTof&);

  ClassDef(CbmMcbm2018MonitorAlgoTof, 1)
};

#endif  // CbmMcbm2018MonitorAlgoTof_H
