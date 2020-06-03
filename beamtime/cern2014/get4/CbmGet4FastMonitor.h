// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4FastMonitor                           -----
// -----                    Created 15.12.2014 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                  Based on CbmTSUnpackNxyter                       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMFASTGET4MONITOR_H
#define CBMFASTGET4MONITOR_H

#include "Timeslice.hpp"

// Specific headers
#include "CbmGet4v1xHackDef.h"
#include "CbmTSUnpack.h"

// ROOT headers
#include "TClonesArray.h"

// C++ std headers
#include <vector>
//#include <map>
//#include <set>

class TH1;
class TH2;
class TProfile;

const UInt_t kuNbChanFee  = 32;
const UInt_t kuNbChipFee  =  8;
const UInt_t kuNbChanTest = 16;
const UInt_t kuNbChanComb =  4;

class CbmGet4FastMonitor : public CbmTSUnpack
{
public:

  CbmGet4FastMonitor();
  virtual ~CbmGet4FastMonitor();

  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers() {;}
  
  virtual void FillOutput(boost::any){;}

  inline void SetRocNb(  UInt_t uNbRocsIn ) { fuNbRocs = uNbRocsIn; }
  inline void SetGet4Nb( UInt_t uNbChipsIn) { fuNbGet4 = uNbChipsIn; }
  inline void SetMsOverlapTs( UInt_t uMsOverIn) { fuMsOverlapTs = uMsOverIn; }
         void SetActiveGet4( UInt_t uChipsIndex, Bool_t bActiveFlag = kTRUE);

  inline void SetVerbose( Bool_t inVerb = kTRUE ) { fbVerbose = inVerb; }
  inline void SetDebug( Bool_t inDeb = kTRUE ) { fbDebug = inDeb; }
  inline void SetDebugStartTs( Int_t inDebStart = -1 ) { fiDebugTsStart = inDebStart; }
  inline void SetDebugStopTs(  Int_t inDebStop  = -1 ) { fiDebugTsStop = inDebStop; }

  inline void SetMode( Int_t inMode = 2 ) { fiMode = inMode; }

  inline void SetBinSizeEvoHistos( Double_t dBinSzIn ) {  fdEvoBinSizeS  = ( 0.0 < dBinSzIn ? dBinSzIn : 1.0);};
  inline void SetLengthEvoHistos(  Double_t dLengthIn ) { fdEvoLengthSec = dLengthIn;};
  inline void SetMicroSliceLength( Double_t dMsIn ) {     fdMsLengthSec   = dMsIn;};
  inline void SetPulserMode( Bool_t inPulserMode = kTRUE ) { fbPulserMode = inPulserMode; }
  inline void SetPulserFee( UInt_t inPulserFee = 0 ) { fuPulserFee = inPulserFee; }
  inline void SetPulserFeeB( UInt_t inPulserFeeB = 0 ) { fuPulserFeeB = inPulserFeeB; fbPulserFeeB = kTRUE;}
         void SetPulserChans( UInt_t inPulserChanA = 0, UInt_t inPulserChanB = 1, UInt_t inPulserChanC = 2,
        UInt_t inPulserChanD =  3, UInt_t inPulserChanE =  4, UInt_t inPulserChanF =  5,
        UInt_t inPulserChanG =  6, UInt_t inPulserChanH =  7, UInt_t inPulserChanI =  8,
        UInt_t inPulserChanJ =  9, UInt_t inPulserChanK = 10, UInt_t inPulserChanL = 11,
        UInt_t inPulserChanM = 12, UInt_t inPulserChanN = 13, UInt_t inPulserChanO = 14,
        UInt_t inPulserChanP = 15 );
  inline void SetOldReadoutSupp( Bool_t inReadoutAllowed = kTRUE ) { fbOldReadoutOk = inReadoutAllowed; }
  inline void SetMissEdgeCheck( Bool_t inMissCheckAllowed = kTRUE )
     { fbOldReadoutOk = inMissCheckAllowed; fbEnableMissingEdgeCheck = inMissCheckAllowed; }

  inline void SetMaxCoincDist( Double_t dMaxDistIn) { fdMaxCoincDist = dMaxDistIn;}

  inline void SetMaxDtMultiHit( Double_t dMaxDt ) { fdMaxDtMultiHit = dMaxDt; };

  void SetHistoFilename( TString sNameIn );

  void SetCalibOutOn( Bool_t bCalibOn = kTRUE) { fbEnaCalibOutput = bCalibOn;};
  void SetCalibOutFolder( TString sCalibOUtFolder = "./") { fsCalibOutFoldername = sCalibOUtFolder;};
  void SetCalibFilename( TString sCalibFileName = "") { fsCalibFilename = sCalibFileName;};

protected:
  //  virtual void Register();
  void FinishUnpack();

private:
  // Behavior control variables
  Bool_t fbVerbose;
  Bool_t fbDebug;
  Int_t  fiDebugTsStart;
  Int_t  fiDebugTsStop;
  Int_t  fiMode;
  TString fsHistoFilename;

  // Hardware related variables => MUST be set on startup by user!
  UInt_t fuNbRocs;
  UInt_t fuNbGet4;
  UInt_t fuNbFee;
  UInt_t fuMsOverlapTs;
  std::vector< Bool_t  > fvbActiveChips; // Active flag to avoid mapping need (one per GET4 chip, def. kTRUE)

  // TS counter
  ULong64_t fulTsNb;
  ULong64_t fulMsNb;
  size_t    fsMaxMsNb;
  UInt_t    fuOffset;
  UInt_t    fuMaxOffset;
  UInt_t    fuLocalOffset;
  UInt_t    fuMaxLocalOffset;

  // SYNC handling
  std::vector< Bool_t > fvbRocFeetFirstSyncDlm;  // Keep track whether the 1st SYNC DLM was processed  (one per ROC)
  std::vector< Bool_t > fvbRocFeetSyncStart;  // Keep track whether the SYNC DLM was processed  (one per ROC)
  std::vector< Bool_t > fvbGet4WaitFirstSync; // True until the 1st SYNC Ep2 after DLM is found (one per GET4)

  // Epochs book-keeping variables
  std::vector< UInt_t > fvuCurrEpoch;       // Current epoch  (one per ROC)
  std::vector< UInt_t > fvuCurrEpochCycle;  // Current cycle of the epoch counter (one per ROC)
  std::vector< UInt_t > fvuCurrEpoch2;      // Current epoch2 (one per GET4 chip)
  std::vector< UInt_t > fvuCurrEpoch2Cycle; // Current cycle of the epoch2 counter (one per GET4 chip)

  // Monitoring related variables
     // Base, always on
  TH2 * fhMessageTypePerRoc;
  TH2 * fhRocSyncTypePerRoc;
  TH2 * fhRocAuxTypePerRoc;
  TH2 * fhSysMessTypePerRoc;
  TH2 * fhMessTypePerGet4;
  TH2 * fhGet4EpochFlags;
  TH2 * fhGet4EpochSyncDist;
  TH2 * fhGet4EpochJumps;
  TH1 * fhGet4BadEpochRatio;
  TH2 * fhGet4BadEpRatioChip;
  TH1 * fhGet4ChanDataCount;
  TH2 * fhGet4ChanDllStatus;
  TH2 * fhGet4ChanTotMap;
  TH2 * fhGet4ChanErrors;
  TProfile * fhGet4ChanTotOwErrorsProp;
  TProfile * fhGet4ChanTotOrErrorsProp;
  TProfile * fhGet4ChanTotEdErrorsProp;
  TH2 * fhGet4ChanSlowContM;
  TH2 * fhGet4ChanEdgesCounts;
  TH2 * fhGet4ChanDeadtime;
  TH2 * fhGet4SeuCounter;
     // Time evolution of data
  Double_t fdEvoBinSizeS;
  Double_t fdEvoLengthSec;
  Double_t fdMsLengthSec;
  TProfile *           fhMsSizeEvo;
  std::vector< TH1 * > fvhRocDataCntEvo;
  std::vector< TH1 * > fvhRocEpochCntEvo;
  std::vector< TH1 * > fvhRocErrorCntEvo;
  std::vector< TH1 * > fvhRocTotErrorCntEvo;
  std::vector< TH1 * > fvhRocEpochMissmCntEvo;
  std::vector< TH1 * > fvhRocDataLossCntEvo;
  std::vector< TH1 * > fvhRocEpochLossCntEvo;
  std::vector< TH1 * > fvhGet4DataCntEvo;
  std::vector< TH1 * > fvhGet4ErrorCntEvo;
  std::vector< TH1 * > fvhGet4EpochMissmCntEvo;
  std::vector< TH1 * > fvhGet4DataLossCntEvo;
  std::vector< TH1 * > fvhGet4EpochLossCntEvo;
  std::vector< TH2 * > fvhGet4ChDataCntEvo;
  std::vector< TH2 * > fvhGet4ChErrorCntEvo;
  std::vector< TH1 * > fvhDetDataCntEvo;
  std::vector< TH1 * > fvhDetErrorCntEvo;
  std::vector< TH1 * > fvhDetEpochMissmCntEvo;
  std::vector< TH1 * > fvhDetDataLossCntEvo;
  std::vector< TH1 * > fvhDetEpochLossCntEvo;
  TProfile *           fhClosedEpSizeEvo;
     // TDC pulser test, works up to 1 hits per 2 epoch
  Bool_t fbPulserMode;
  UInt_t fuPulserFee;
  UInt_t fuPulserChan[kuNbChanTest]; //!
  std::vector< UInt_t >           fvuLastHitEp; // Epoch of Last hit message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmLastHit;   // Last hit message (one per GET4 chip & channel)
  TH1 * fhTimeResFee[kuNbChanFee*(kuNbChanFee-1)/2]; //!
  TH2 * fhTimeRmsAllFee;
  TH2 * fhTimeResAllFee;
  //--------------------------------------------------------------//
  Bool_t fbPulserFeeB;
  UInt_t fuPulserFeeB;
  TH1 * fhTimeResFeeB[kuNbChanFee*(kuNbChanFee-1)/2]; //!
  TH2 * fhTimeRmsAllFeeB;
  TH2 * fhTimeResAllFeeB;
  TH1 * fhTimeResFeeAB[kuNbChanFee*kuNbChanFee]; //!
  TH2 * fhTimeRmsAllFeeAB;
  TH2 * fhTimeResAllFeeAB;
  //--------------------------------------------------------------//
  TH1 * fhTimeRmsAllPairs;
  TH1 * fhTimeResAllPairs;
  TH2 * fhTimeRmsAllCombi;
  TH2 * fhTimeResAllCombi;
  TH1 * fhTimeResPairs[kuNbChanTest - 1]; //!
  TH1 * fhTimeResCombi[kuNbChanComb*(kuNbChanComb-1)/2]; //!
  TH2 * fhTimeResPairsTot[(kuNbChanTest - 1)*2]; //!
  TH2 * fhTimeResCombiTot[(kuNbChanComb*(kuNbChanComb-1)/2)*2]; //!
  TH1 * fhTimeResCombiEncA[kuNbChanComb*(kuNbChanComb-1)/2]; //!
  TH1 * fhTimeResCombiEncB[kuNbChanComb*(kuNbChanComb-1)/2]; //!
  TH2 * fhTimeRmsAllCombiEncA;
  TH2 * fhTimeResAllCombiEncA;
  TH2 * fhTimeRmsAllCombiEncB;
  TH2 * fhTimeResAllCombiEncB;
  TH2 * fhPulserHitDistNs;
  TH2 * fhPulserHitDistUs;
  TH2 * fhPulserHitDistMs;
  TH2 * fhPulserFeeDnl;
  TH2 * fhPulserFeeInl;
  std::vector<TH2 *> fhPulserFeeDistCT;
  //--------------------------------------------------------------//
  std::vector< UInt_t >           fvuPrevHitEp; //! Epoch of previous hit message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmPrevHit;   //! Previous hit message (one per GET4 chip & channel)
     // Finetime monitoring for hits with normal pulser time difference and too big difference
  TH2 * fhFtSmallDtFeeA;
  TH2 * fhFtSmallDtFeeB;
  TH2 * fhFtBigDtFeeA;
  TH2 * fhFtBigDtFeeB;
  TH2 * fhFtPrevSmallDtFeeA;
  TH2 * fhFtPrevSmallDtFeeB;
  TH2 * fhFtPrevBigDtFeeA;
  TH2 * fhFtPrevBigDtFeeB;
     // Coarse time and Tot full coarse time of last hits in 32b mode when epoch jumps found
  std::vector<TH2 *> fhFullCtEpJumpFeeA;
  std::vector<TH2 *> fhFullCtEpJumpFeeACh;
  std::vector<TH2 *> fhFullCtEpJumpFeeAChSort;
  TH2 *              fhFullCtEpJumpFeeAChOrder;
  std::vector<std::vector<UInt_t>> fvChanOrder;
  std::vector<TH2 *> fhFullCtChOrderCh;
  std::vector<TH2 *> fhFullCtEpQualityCh;
  std::vector<TH2 *> fhFullCtEpQualityChZoom;
  std::vector<TH2 *> fhEpQualityFirstChFeeA;
     // Monitoring of 24b mode for debugging purpose
  Bool_t fbOldReadoutOk;
  TH1 * fhGet4ChanTotCount;
  std::vector< UInt_t >           fvuLastOldTotEp; //! Epoch of Last TOT message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmLastOldTot;   //! Last TOT message (one per GET4 chip & channel)
     //
  TH2 * fhPulserFeeTotDnl;
  TH2 * fhPulserFeeTotInl;
  TH2 * fhPulserFeeRisCtWideBins;
  TH2 * fhPulserFeeFalCtWideBins;
  Bool_t fbEnableMissingEdgeCheck;
  std::vector< UInt_t >           fvuLastTotInFtBins;
  TH2 * fhPulserFeeFtRecoMissRis;
  TH2 * fhPulserFeeFtRecoMissFal;
  std::vector< UInt_t >           fvuNbRisEdgeEpoch;
  std::vector< UInt_t >           fvuNbFalEdgeEpoch;
  TH2 * fhPulserFeeExtraRisEp;
  TH2 * fhPulserFeeExtraFalEp;
  TH2 * fhPulserFeeExtraEdgesEp;
  std::vector< UInt_t >           fvuFeePrevRisEp; //! Epoch of previous to last Rising edge message (one per GET4 channel in Fee)
  std::vector< get4v1x::Message > fvmFeePrevRis;   //! Previous to last Rising edge message (one per GET4 channel in fee)
  std::vector< UInt_t >           fvuFeePrevFalEp; //! Epoch of previous to last Falling edge message (one per GET4 channel in Fee)
  std::vector< get4v1x::Message > fvmFeePrevFal;   //! Previous to last Falling edge message (one per GET4 channel in fee)
  TH2 * fhPulserFeeFtExtraEdgeRisA;
  TH2 * fhPulserFeeFtExtraEdgeFalA;
  TH2 * fhPulserFeeFtExtraEdgeRisB;
  TH2 * fhPulserFeeFtExtraEdgeFalB;
  TH2 * fhPulserFeeGoodTot;
  TH2 * fhPulserFeeExtraRecoTot;
  std::vector<TH2 *> fhPulserFeeTotDistCT;
  //--------------------------------------------------------------//
  std::vector< UInt_t >           fvuPrevOldTotEp; //! Epoch of previous TOT message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmPrevOldTot;   //! Previous TOT message (one per GET4 chip & channel)
     // TOT Finetime monitoring for hits with normal pulser time difference and too big difference
  TH2 * fhFtTotSmallDtFeeA;
  TH2 * fhFtTotSmallDtFeeB;
  TH2 * fhFtTotBigDtFeeA;
  TH2 * fhFtTotBigDtFeeB;
  TH2 * fhFtPrevTotSmallDtFeeA;
  TH2 * fhFtPrevTotSmallDtFeeB;
  TH2 * fhFtPrevTotBigDtFeeA;
  TH2 * fhFtPrevTotBigDtFeeB;
     // Channel coincidences
  std::vector< std::vector< TH2 * > > fvvhChannelsCoinc; //!
  Double_t fdMaxCoincDist;

  // Multiple hits (reflections, ...) detection
  std::vector< TH2 * > fvhGet4MultipleHits; //!
  std::vector< TH2 * > fvhGet4DistDoubleHits; //!
  std::vector< TH2 * > fvhGet4DistTripleHits; //!
  std::vector< TH2 * > fvhGet4DistMultipleHits; //!
  std::vector< TH2 * > fvhGet4MultipleHitsVsTot; //!
  Double_t fdMaxDtMultiHit;
  std::vector< std::vector< Bool_t > > fvbChanSecondHit; //!
  std::vector< std::vector< Bool_t > > fvbChanThirdHit; //!
  std::vector< std::vector< Double_t > > fvdChanFirstHitTot; //!

  // Monitoring functions
  void InitMonitorHistograms();
  void MonitorMessage_epoch(  get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_sync(   get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_aux(    get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_sys(    get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_epoch2( get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_get4(   get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_Get4v1( get4v1x::Message mess, uint16_t EqID);
  void FillMonitorHistograms();
  void WriteMonitorHistograms();
  void DeleteMonitorHistograms();
  Bool_t fbHistoWriteDone;

  // Finetime calibration members and method
  Bool_t  fbEnaCalibOutput;
  TString fsCalibOutFoldername;
  TString fsCalibFilename;
  std::vector< std::vector< TH1 * > >  fvhFineTime; //!
  Bool_t WriteCalibrationFile();

  CbmGet4FastMonitor(const CbmGet4FastMonitor&);
  CbmGet4FastMonitor operator=(const CbmGet4FastMonitor&);

  void PrintOptions();

  ClassDef(CbmGet4FastMonitor, 1)
};

#endif // CBMFASTGET4MONITOR_H
