// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmTSUnpackGet4v1x                           -----
// -----                    Created 15.12.2014 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                  Based on CbmTSUnpackNxyter                       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKGET4V1X_H
#define CBMTSUNPACKGET4V1X_H

#include "Timeslice.hpp"

// Specific headers
#include "CbmGet4v1xHackDef.h"
#include "CbmTSUnpack.h"

// ROOT headers
#include "TClonesArray.h"

// C++ std headers
#include <vector>
#include <map>
#include <set>

class TH1;
class TH2;
class TProfile;

class CbmGet4EpochBuffer;
/*
const UInt_t kuNbChanFee  = 32;
const UInt_t kuNbChanTest = 16;
const UInt_t kuNbChanComb = 4;
*/
class CbmTSUnpackGet4v1x : public CbmTSUnpack
{
public:

  CbmTSUnpackGet4v1x();
  virtual ~CbmTSUnpackGet4v1x();

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

  inline void SetMode( Int_t inMode = 2 ) { fiMode = inMode; }

  inline void SetBinSizeEvoHistos( Double_t dBinSzIn ) {  fdEvoBinSizeS  = ( 0.0 < dBinSzIn ? dBinSzIn : 1.0);};
  inline void SetLengthEvoHistos(  Double_t dLengthIn ) { fdEvoLengthSec = dLengthIn;};
  inline void SetMicroSliceLength( Double_t dMsIn ) {     fdMsLengthSec   = dMsIn;};
/*
  inline void SetPulserMode( Bool_t inPulserMode = kTRUE ) { fbPulserMode = inPulserMode; }
  inline void SetPulserFee( UInt_t inPulserFee = 0 ) { fuPulserFee = inPulserFee; }
         void SetPulserChans( UInt_t inPulserChanA = 0, UInt_t inPulserChanB = 1, UInt_t inPulserChanC = 2,
        UInt_t inPulserChanD =  3, UInt_t inPulserChanE =  4, UInt_t inPulserChanF =  5,
        UInt_t inPulserChanG =  6, UInt_t inPulserChanH =  7, UInt_t inPulserChanI =  8,
        UInt_t inPulserChanJ =  9, UInt_t inPulserChanK = 10, UInt_t inPulserChanL = 11,
        UInt_t inPulserChanM = 12, UInt_t inPulserChanN = 13, UInt_t inPulserChanO = 14,
        UInt_t inPulserChanP = 15 );
  inline void SetOldReadoutSupp( Bool_t inReadoutAllowed = kTRUE ) { fbOldReadoutOk = inReadoutAllowed; }
*/

  inline void SetMaxCoincDist( Double_t dMaxDistIn) { fdMaxCoincDist = dMaxDistIn;}

//  inline void SetMaxDtMultiHit( Double_t dMaxDt ) { fdMaxDtMultiHit = dMaxDt; };

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
  TH2 * fhGet4EpochJumps; // TODO
  TH1 * fhGet4ChanDataCount;
  TH2 * fhGet4ChanDllStatus;
  TH2 * fhGet4ChanTotMap;
  TH2 * fhGet4ChanErrors;
  TH2 * fhGet4ChanSlowContM;
     // Time evolution of data
  Double_t fdEvoBinSizeS;
  Double_t fdEvoLengthSec;
  Double_t fdMsLengthSec;
  TProfile *           fhMsSizeEvo;
  std::vector< TH1 * > fvhRocDataCntEvo;
  std::vector< TH1 * > fvhRocEpochCntEvo;
  std::vector< TH1 * > fvhRocErrorCntEvo;
  std::vector< TH1 * > fvhRocDataLossCntEvo;
  std::vector< TH1 * > fvhRocEpochLossCntEvo;
  std::vector< TH1 * > fvhGet4DataCntEvo;
  std::vector< TH1 * > fvhGet4ErrorCntEvo;
  std::vector< TH1 * > fvhGet4DataLossCntEvo;
  std::vector< TH1 * > fvhGet4EpochLossCntEvo;
  std::vector< TH2 * > fvhGet4ChDataCntEvo;
  std::vector< TH2 * > fvhGet4ChErrorCntEvo;
  std::vector< TH1 * > fvhDetDataCntEvo;
  std::vector< TH1 * > fvhDetErrorCntEvo;
  std::vector< TH1 * > fvhDetDataLossCntEvo;
  std::vector< TH1 * > fvhDetEpochLossCntEvo;
  TProfile *           fhClosedEpSizeEvo;

/*
     // TDC pulser test, works up to 1 hits per 2 epoch
  Bool_t fbPulserMode;
  UInt_t fuPulserFee;
  UInt_t fuPulserChan[kuNbChanTest];
  TH1 * fhTimeResFee[kuNbChanFee*(kuNbChanFee-1)/2];
  TH2 * fhTimeRmsAllFee;
  TH2 * fhTimeResAllFee;
  TH1 * fhTimeResPairs[kuNbChanTest - 1];
  TH1 * fhTimeResCombi[kuNbChanComb*(kuNbChanComb-1)/2];
  TH2 * fhPulserHitDistNs;
  TH2 * fhPulserHitDistUs;
  TH2 * fhPulserHitDistMs;
  TH2 * fhPulserFeeDnl;
  TH2 * fhPulserFeeInl;
     // Monitoring of 24b mode for debugging purpose
  Bool_t fbOldReadoutOk;
  TH1 * fhGet4ChanTotCount;
     //
  TH2 * fhPulserFeeTotDnl;
  TH2 * fhPulserFeeTotInl;

  // Multiple hits (reflections, ...) detection
  std::vector< TH2 * > fvhGet4MultipleHits;
  std::vector< TH2 * > fvhGet4DistDoubleHits;
  std::vector< TH2 * > fvhGet4DistTripleHits;
  std::vector< TH2 * > fvhGet4DistMultipleHits;
  std::vector< TH2 * > fvhGet4MultipleHitsVsTot;
  Double_t fdMaxDtMultiHit;
  std::vector< std::vector< Bool_t > > fvbChanSecondHit;
  std::vector< std::vector< Bool_t > > fvbChanThirdHit;
  std::vector< std::vector< Double_t > > fvdChanFirstHitTot;

  // Monitoring functions
  void MonitorMessage_epoch(  get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_sync(   get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_aux(    get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_sys(    get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_epoch2( get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_get4(   get4v1x::Message mess, uint16_t EqID);
  void MonitorMessage_Get4v1( get4v1x::Message mess, uint16_t EqID);
*/
    // Channel coincidences
  std::vector< std::vector< TH2 * > > fvvhChannelsCoinc;
  Double_t fdMaxCoincDist;
  std::vector< UInt_t >           fvuLastHitEp; // Epoch of Last hit message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmLastHit;   // Last hit message (one per GET4 chip & channel)
  std::vector< UInt_t >           fvuLastOldTotEp; // Epoch of Last TOT message (one per GET4 chip & channel)
  std::vector< get4v1x::Message > fvmLastOldTot;   // Last TOT message (one per GET4 chip & channel)
  void InitMonitorHistograms();
  void FillMonitorHistograms();
  void WriteMonitorHistograms();
  void DeleteMonitorHistograms();

  // Unpacking related variables
     // Map of following pairs: full epoch +  Multiset containers for ordered data (1/epoch)
//  std::map< ULong64_t, std::multiset< get4v1x::FullMessage > > fmsOrderedEpochsData; // OLD
  std::map< ULong64_t, std::multiset< get4v1x::FullMessage >* > fmsOrderedEpochsData;
     // Map of following pairs: full epoch +  Closed buffer flag (1/epoch)
  std::map< ULong64_t, Bool_t > fmsOrderedEpochsBuffStat;
     // FULL epoch2 key of the Current buffer (one per GET4 chip)
  std::vector< ULong64_t >      fvuCurrEpochBuffer;
     // iterator of the Current buffer (one per GET4 chip)
  std::vector<
     std::map< ULong64_t, std::multiset<
//           get4v1x::FullMessage > // OLD
           get4v1x::FullMessage > *
        >::iterator >  fvuCurrEpochBufferIt;
     // Last epoch buffer for which all chips were done
  ULong64_t fuLastEpBufferReady;
     // Current buffer for read (first unread close epoch, to be erased after read?)
  ULong64_t fuCurrEpReadBuffer;

  CbmGet4EpochBuffer * fEpochBuffer;

  // Unpacking functions
  // EqID = Equipment identifier from ums
  void ProcessMessage_epoch(  get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_sync(   get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_aux(    get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_sys(    get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_epoch2( get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_get4(   get4v1x::Message mess, uint16_t EqID);
  void ProcessMessage_Get4v1( get4v1x::Message mess, uint16_t EqID);

  // Finetime calibration members and method
  Bool_t  fbEnaCalibOutput;
  TString fsCalibOutFoldername;
  TString fsCalibFilename;
  std::vector< std::vector< TH1 * > >  fvhFineTime;
  Bool_t WriteCalibrationFile();

  CbmTSUnpackGet4v1x(const CbmTSUnpackGet4v1x&);
  CbmTSUnpackGet4v1x operator=(const CbmTSUnpackGet4v1x&);

  ClassDef(CbmTSUnpackGet4v1x, 1)
};

#endif // CBMTSUNPACKGET4V1X_H
