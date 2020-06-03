// ------------------------------------------------------------------
// -----                     TGet4v1Rec.h                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// -----                 Adapted from TGet4v1Rec.h              -----
// -----         in RocLib/beamtime/tof-tdctest rev.4862        -----
// ------------------------------------------------------------------
#ifndef TGET4V1REC_H
#define TGET4V1REC_H

// GET4 Headers
#include "TofGet4Def.h"
#include "TGet4v1Event.h"

// ROClib headers
#include "roc/Iterator.h"
#include "roc/Message.h"
#include "roc/Board.h"

//class TGo4WinCond;

// ROOT headers
#include "TNamed.h"
#include "TH1.h"
#include "TH2.h"

// C++ headers
#include <vector>

#ifdef HAVE_UINT8_T_HEADER_FILE
#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#else
#include <stdint.h>
#endif

class TimeWindowCond : public TNamed
{
   public:
      TimeWindowCond();
      TimeWindowCond( const char* name, Double_t tLow, Double_t tHigh );
      ~TimeWindowCond(){};
      void Clear(Option_t *t="");

      void SetValues( Double_t tLow, Double_t tHigh );

      Bool_t Test( Double_t dValToTest);
   private:
      Double_t fdLowT;
      Double_t fdHighT;

   ClassDef(TimeWindowCond, 1)
};

struct TGet4v1Rec {

   Int_t fRocId;
   roc::Iterator fIter; // iterator of current roc input buffer

   TH1* fMsgTypes; // messages types
   TH1* fSysTypes; // system messages types
   TH1* fSysUserTypes; // user system messages
   TH1* fAUXch; // filling of AUX channels

   TH1* fTrigger_All;          // time shift between last trigger and hit
   TH1* fTrigger_AUX;          // time shift between last trigger and all AUX
   TH1* fTrigger_All_100;      // time shift between last trigger and hit (100ns bin)
   TH1* fTrigger_All_us;      // time shift between last trigger and hit (1 us bin)
   TH1* fTrigger_All_ms;      // time shift between last trigger and hit (1 ms bin)
   TH1* fTrigger_All_s;      // time shift between last trigger and hit (1 s bin)
//   TGo4WinCond* fTriggerWind;  // time window to assign hits to event (relative to last trigger)
//   TGo4WinCond* fAUXWind;      // time window to assign AUX to event (relative to last trigger, differs from NX messages)
//   TGo4WinCond* fSelfTrigWind; // time window to detect trigger inside data themselves
   TimeWindowCond* fTriggerWind;  // time window to assign hits to event (relative to last trigger)
   TimeWindowCond* fAUXWind;      // time window to assign AUX to event (relative to last trigger, differs from NX messages)
   TimeWindowCond* fSelfTrigWind; // time window to detect trigger inside data themselves
   TH1* fEvntMultipl;          // event multiplicity (how many messages selected)

   TH1* fALLt; // time distribution of all signals
   TH1* fDATAt; // time distribution of data signals
   TH1* fERRORt; // time distribution of error signals
   TH1* fSLOWCt; // time distribution of slow control signals
   TH1* fSelfTriggT;      // time distribution of triggers from data
   TH1* fAUXt[get4v10::kuMaxAux];   // time distribution of aux signals
   TH1* fSYNCt[get4v10::kuMaxSync]; // time distribution of sync signals
   TH1* fEPOCHt; // time distribution of epoch signals
   TH1* fEPOCH2t[get4v10::kuMaxGet4Roc]; // time distribution of epoch signals
   TH1* fAllSelectedT; // time distribution of all selected hits in an event
   TH1* fSelectedT[get4v10::kuMaxGet4Roc][get4v10::kuNbChan]; // time distribution of hits selected in an event

   // Long duration time distribution (1 min. bin, 10 days length)
   TH1* fAllLongT;            // Long time distribution of all signals
   TH1* fDataLongT;           // Long time distribution of data signals
   TH1* fErrorLongT;          // Long time distribution of error signals
   TH1* fSlowCLongT;          // Long time distribution of slow control signals
   TH1* fSelfTriggLongT;      // Long time distribution of triggers from data
   TH1* fSyncLongT[get4v10::kuMaxSync]; // Long time distribution of sync signals
   TH1* fEpochLongT;          // Long time distribution of epoch signals
   TH1* fAllEpoch2LongT;      // Long time distribution of all epoch2 signals
   TH1* fAllSelLongT;         // Long time distribution of all selected hits in an event

   uint64_t fLastTm;               //
   uint64_t fLastTriggerTm;        // last time of trigger signal (defined by parameter)
   uint64_t fLastAuxTm[get4v10::kuMaxAux];   // last time of rising edge of AUX signals
   uint64_t fLastSyncTm[get4v10::kuMaxSync]; // last time of SYNC signals
   uint32_t fLastSyncId[get4v10::kuMaxSync]; // last ID of SYNC signals
   uint64_t fStartSyncTm;          // time of SYNC signal in the beginning of subevent
   uint64_t fStopSyncTm;           // time of SYNC signal at the end of subevent

   std::vector<TGet4v1MessageExtended> fUnprocessedMsg; // buffer of not accounted hit messages of previous mbs evnts

   Int_t fDabcSeparator; // sync id of the dabc event separator. For event finalizing condition

   Bool_t   bIgnoreData;       // true when data should be ignored during reconfiguration
   Int_t    fIgnoreCnt;        // counter used to process data when ignored, reset when ignore activated
   uint64_t fIgnoreTime;       // time when ignore message was generated
   uint64_t fCollectTime;      // time when collect message was generated

   /* Modified part */

      // Epochs
   UInt_t   fuCurrEpoch;             // current epoch on the roc
   UInt_t   fuEpochCycle;            // Number of time the epoch counter cycled
   UInt_t   fuCurrEpoch2[get4v10::kuMaxGet4Roc];  // current epoch2 for each get4 chip
   UInt_t   fuEpoch2Cycle[get4v10::kuMaxGet4Roc]; // Number of time the epoch2 counter cycled
   TH1 *fDistribEpochs;
   TH2 *fDistribEpochs2;
   TH2 *fEpochShiftsPerChip;
   TH2 *fEpochShiftsDurationPerChip;
   Int_t    fiEpochShift[get4v10::kuMaxGet4Roc];
   UInt_t   fuNbShiftedEpochs[get4v10::kuMaxGet4Roc];
   TH2 *fEpochShiftsDuration[get4v10::kuMaxGet4Roc];

      // Buffers
         // ROC
   Bool_t fbBufferWithLastFullEpoch;
   std::vector<TGet4v1MessageExtended> fPrevEpochsBuffer[2]; // buffer of all ROC messages in current epoch and previous one
   Bool_t fbEpochSinceTrigger;
         // GET4s
   Bool_t fbBufferWithLastFullEpoch2[get4v10::kuMaxGet4Roc];
   std::vector<TGet4v1MessageExtended> fPrevEpochs2Buffer[get4v10::kuMaxGet4Roc][2]; // buffer of all get4 messages in current epoch and previous one
   Bool_t bLookInPreviousEpoch[get4v10::kuMaxGet4Roc];
   Bool_t bLookInNextEpoch[get4v10::kuMaxGet4Roc];
   Bool_t fbEpoch2SinceTrigger[get4v10::kuMaxGet4Roc];

      // HitsSelection
   Bool_t fbSelectionRocDone;
   Bool_t fbSelectionDone[get4v10::kuMaxGet4Roc];
   Bool_t fbDataTimeOut[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   Bool_t fbDataFallingOut[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1* fTrigger_Get4Channel[get4v10::kuMaxGet4Roc][get4v10::kuNbChan]; // time shift between last trigger and hit in this Get4 channel
   TH1* fTriggerMs_Get4Channel[get4v10::kuMaxGet4Roc][get4v10::kuNbChan]; // time shift between last trigger and hit in this Get4 channel
   TH1* fTriggerS_Get4Channel[get4v10::kuMaxGet4Roc][get4v10::kuNbChan]; // time shift between last trigger and hit in this Get4 channel

      // Triggering on data themselves
   TH2* fDataSelfTriggerPerEvent;
   TH2* fDataSelfTrigDistanceNs;
   TH2* fDataSelfTrigDistanceUs;
   TH2* fDataSelfTrigDistanceMs;
   TH2* fDataSelfTrigDistanceS;

   UInt_t fuDataSelfTriggerCount[get4v10::kiMaxNbDataTriggs];
   Double_t fdLastFullTimeSelfTrigger[get4v10::kiMaxNbDataTriggs];
   TGet4v1MessageExtended fextMessLastMainChannel[get4v10::kiMaxNbDataTriggs];
   Double_t fdLastFullTimeMainChannelTot[get4v10::kiMaxNbDataTriggs];
   Double_t fdLastFullTimeSecChannel[get4v10::kiMaxNbDataTriggs][get4v10::kiMaxNbSecTriggCh];
   Double_t fdLastFullTimeSecChannelTot[get4v10::kiMaxNbDataTriggs][get4v10::kiMaxNbSecTriggCh];

      // Event statistics
   TH1* fNbEventsPerMbsEvent;
   TH1* fNbNotEmptyEventsPerMbsEvent;
   TH2 *fChannelsMapping;
   TH1 *fChannelInputMessCount;
   TH2 *fChannelMultiplicity;
   TH1 *fChipRateEvolution[get4v10::kuMaxGet4Roc];
   TH1 *fChannelRateEvolution[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   Double_t fdRateEvolutionBinSize;
   UInt_t fuNbHitsChipEpoch[get4v10::kuMaxGet4Roc];


   /*
    * 24 & 32 bits variables & histograms
    */
   TH1 *fRawTot[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fTot[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];

   /*
    * 24 bits specific variables & histograms
    */
   Bool_t fb24bitsReadoutDetected;
   Get4v1Hit fHitTemp24[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   UInt_t   fuEpochWithData[get4v10::kuMaxGet4Roc]; // Data flag for suppressed epoch mode
   std::vector<TGet4v1MessageExtended> fEpSuppBuffer[get4v10::kuMaxGet4Roc]; // temp buffer in supp epoch mode to wait for epoch message

   /*
    * 32 bits specific variables & histograms
    */
   Bool_t fb32bitsReadoutDetected;
   TH2 *fGet4V1SlowControlType;
   TH2 *fGet4V1SlowControlScaler;
   TH2 *fGet4V1SlowControlDeadTime;
   TH2 *fGet4V1SlowControlHamming;
   TH2 *fGet4V1SlowControlSeuEvo;
   TH1 *fGet4ErrorChip;           // Get4 System Error chip
   TH2 *fGet4ErrorChan;           // Get4 System Error channel/edge
   TH2 *fGet4ErrorPatt;           // Get4 System Error pattern
   Bool_t fbDllFlag[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH2 *fGet4V1DllLockBit;

   TGet4v1MessageExtended fLastExtMess[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH2 *fGet4V1HitsDistanceNs[get4v10::kuMaxGet4Roc];
   TH2 *fGet4V1HitsDistanceUs[get4v10::kuMaxGet4Roc];
   TH2 *fGet4V1HitsDistanceMs[get4v10::kuMaxGet4Roc];
   TH2 *fGet4V1HitsDistanceS[get4v10::kuMaxGet4Roc];

   /*
    * Debug Histograms for GET4 v1.x
    */
   TH1 *fLeadingFTS[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fTrailingFTS[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fLeadingDnl[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fLeadingDnlSum[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fTrailingDnl[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];
   TH1 *fTrailingDnlSum[get4v10::kuMaxGet4Roc][get4v10::kuNbChan];

   TH1 *fTimeDiffInsideChip[2][get4v10::kuNbChan*(get4v10::kuNbChan-1)/2];
   TH1 *fTimeDiffBetweenChips[get4v10::kuNbChan*get4v10::kuNbChan];

   TH1 *fFTCorrel[2][get4v10::kuNbChan*(get4v10::kuNbChan-1)/2];
   TH2 *fFTCorrel2D[2][get4v10::kuNbChan*(get4v10::kuNbChan-1)/2];
   TH1 *fFTCorrelChipToChip[get4v10::kuNbChan*get4v10::kuNbChan];

   TGet4v1Rec();
   // no copy constructor required to add object in std::vector
   TGet4v1Rec(const TGet4v1Rec&);

   ULong64_t GetFullEpochNumber();
   ULong64_t GetFullEpoch2Number( UInt_t uGet4Id);
   // Functions to overcome the 32 bit limit in epoch number as input to
   // GetMessageFullTime function
   ULong64_t GetSuperCycleEpOffset( );
   ULong64_t GetSuperCycleEp2Offset( UInt_t uGet4Id);
   Double_t GetSuperCycleEp2OffsetD( UInt_t uGet4Id);
   
   private:
      // no default = OP as object not meant to be copied
      TGet4v1Rec& operator=(const TGet4v1Rec&);
};


#endif


