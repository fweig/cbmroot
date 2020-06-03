#ifndef TGET4EVENT_H
#define TGET4EVENT_H

#include <vector>

/*
 * Put all definitions here and use them in processor,
 * since they are necessary for data structure of event
 */

#include "TObject.h"

#include "roc/Message.h"
//#include "Message.h"

#include "TofGet4Def.h"
/*
 * Conversion of definitions to constants:
 *
MAX_ROC               get4v10::kuMaxRoc
MAX_GET4              get4v10::kuMaxGet4
MAX_GET4_PER_ROC      get4v10::kiMaxGet4Roc
MAX_AUX               get4v10::kiMaxAux
MAX_SYNC              get4v10::kiMaxSync
GET4_PRINT            get4v10::kiGet4MessPr
GET4_EPOCH_CYCLE_SIZE get4v10::kulGet4EpochCycleSz
MAIN_EPOCH_CYCLE_SIZE get4v10::kulMainEpochCycleSz
MAIN_CLK_SYNC_SIZE    get4v10::kiSyncCycleSzMain
GET4_SYNC_CYCLE_SIZE  get4v10::kiSyncCycleSzGet4
MAX_DATA_TRIGG        get4v10::kiMaxNbDataTriggs
MAX_SEC_TRIG          get4v10::kiMaxNbSecTriggCh

NB_BIN_GET4_FTS       get4tdc::kiFineTime
GET4_BIN_SIZE_IN_PS   get4tdc::kdBinSize
GET4_EPOCH_IN_BINS    get4tdc::kiEpochInBins
GET4_EPOCH_IN_PS      get4tdc::kdEpochInPs
GET4_CYCLE_IN_EPOCHS  get4v10::kulGet4EpochCycleSz

NB_CHAN_GET4          get4v10::kuNbChan instead of get4tdc::kuNbChan to accomodate 2tdc ch per input ch

psubevt->GetSubcrate()  fcSubCrate
psubevt->GetControl()   fcControl
psubevt->GetDlen()      uLength ???
psubevt->GetDataField() pMbsData

GetTriggerNumber()      Not available (TCBMBeamtimeProc method)
GetEventNumber()        Not available (TCBMBeamtimeProc method)
*/

#ifdef HAVE_UINT8_T_HEADER_FILE
#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#else
#include <stdint.h>
#endif

/*
 * Extended message container. Keeps original ROC message, but adds full timestamp and
 * optionally corrected adc valules.
 * Note that extended messages inside the vector will be sorted after full timestamp
 * by the TRocProc::FinalizeEvent
 *
 * TODO: Recalculation of full time when setting RocEpoch and/or RocCycle !!!
 */
class TGet4v1MessageExtended : public TObject
{
   public:

      TGet4v1MessageExtended(const roc::Message& msg, ULong64_t fulltime=0 ) :
         TObject(),
         fxMessage(msg),
         fuFulltime(fulltime),
         fdFullTime((Double_t)fulltime),
         ffTriggerDeltaT(0),
         fbIs32Bit(kFALSE),
         uRocEpoch( 0 ),
         uRocEpochCycle( 0 ),
         fbIsEmpty(kFALSE)
      {
      }

      TGet4v1MessageExtended() :
         TObject(),
         fxMessage(),
         fuFulltime(0),
         fdFullTime(0),
         ffTriggerDeltaT(0),
         fbIs32Bit(kFALSE),
         uRocEpoch( 0 ),
         uRocEpochCycle( 0 ),
         fbIsEmpty(kFALSE)
      {
      }

      virtual ~TGet4v1MessageExtended(){}

      // copy constructor, used by vector to copy content
      TGet4v1MessageExtended(const TGet4v1MessageExtended& src) : TObject(),
         fxMessage(src.fxMessage), fuFulltime(src.fuFulltime), fdFullTime(src.fdFullTime),
         ffTriggerDeltaT(src.ffTriggerDeltaT), fbIs32Bit(src.fbIs32Bit),
         uRocEpoch(src.uRocEpoch), uRocEpochCycle(src.uRocEpochCycle),
         fbIsEmpty(src.fbIsEmpty){}

      /* this is used for timesorting the messages in the filled vectors */
      /*
      bool operator<(const TGet4v1MessageExtended &rhs) const
         { return (fuFulltime < rhs.fuFulltime); }
         */
      /* Move to double version to insure GET4 messages ordering */
      bool operator<(const TGet4v1MessageExtended &rhs) const
         { return (fdFullTime < rhs.fdFullTime); }

      void Clear(Option_t* ="")
      {
         fxMessage       = roc::Message();
         fuFulltime      = 0;
         fdFullTime      = 0;
         ffTriggerDeltaT = 0;
         fbIs32Bit       = kFALSE;
         uRocEpoch       = 0;
         uRocEpochCycle  = 0;
         SetEmptyFlag();
      }

      void SetRocMessage(const roc::Message & source)
      {
         fxMessage=source;
      }

      const roc::Message& GetRocMessage() const
      {
         return fxMessage;
      }

      UChar_t GetMessageType() const
      {
    	  return fxMessage.getMessageType();
      }
      UChar_t GetSysMesType() const
      {
        return fxMessage.getSysMesType();
      }

      void SetFullTime(ULong64_t ts)
      {
         fuFulltime=ts;
         fdFullTime = (Double_t) ts;
      }
      void SetFullTimeD(Double_t ts)
      {
         fdFullTime = ts;
         fuFulltime = (ULong64_t)ts;
      }
      ULong64_t GetFullTime() const
      {
         return fuFulltime;
      }
      Double_t GetFullTimeD() const
      {
         return fdFullTime;
      }

      uint16_t GetRocNumber() const
      {
    	  return fxMessage.getRocNumber();
      }

      uint8_t GetGet4Number() const
      {
         return fxMessage.getGet4Number();
      }

      uint8_t GetGet4ChNum() const
      {
         return fxMessage.getGet4ChNum();
      }

      uint32_t GetGet4Ts() const
      {
         return fxMessage.getGet4Ts();
      }

      Bool_t IsRisingGet4Edge() const
      {
         return fxMessage.getGet4Edge();
      }

      void SetTriggerDeltaT(float dt)
      {
         ffTriggerDeltaT=dt;
      }

      float GetTriggerDeltaT() const
      {
        return ffTriggerDeltaT;
      }

      Bool_t SetGet4MessageFull( const roc::Message & DataMessIn, UInt_t rocEpochIn,
            UInt_t rocCycleIn)
      {
         fxMessage        = DataMessIn;
         uRocEpoch        = rocEpochIn;
         uRocEpochCycle   = rocCycleIn;
/*
         if( roc::MSG_GET4 == fxMessage.getMessageType() )
            fuFulltime = fxMessage.getMsgFullTime( uRocEpoch );
         else if(roc::MSG_SYS == fxMessage.getMessageType() &&
                 roc::SYSMSG_GET4_EVENT == fxMessage.getSysMesType() )
            fuFulltime = fxMessage.FullTimeStamp2(uRocEpoch, (fxMessage.getField(23,12)<<7) )
                         / 20 + 512;
*/
         return kTRUE;
      }

      Bool_t SetRocEpoch( UInt_t rocEpochIn)
      {
         uRocEpoch        = rocEpochIn;
         /*
         if( roc::MSG_GET4 == fxMessage.getMessageType() )
            fuFulltime = fxMessage.getMsgFullTime( uRocEpoch );
         else if(roc::MSG_SYS == fxMessage.getMessageType() &&
                 roc::SYSMSG_GET4_EVENT == fxMessage.getSysMesType() )
            fuFulltime = fxMessage.FullTimeStamp2(uRocEpoch, (fxMessage.getField(23,12)<<7) )
                         / 20 + 512;
                         */
         return kTRUE;
      }
      Bool_t SetRocCycle( UInt_t rocCycleIn)
      {
         uRocEpochCycle   = rocCycleIn;
         return kTRUE;
      }

      UInt_t GetRocEpoch() const
      {
         return uRocEpoch;
      }
      UInt_t GetRocCycle() const
      {
         return uRocEpochCycle;
      }

      Bool_t RocOrder( const TGet4v1MessageExtended &messageToCompare )const;
      Double_t RocSpacing( const TGet4v1MessageExtended &messageToCompare) const;

      void Print( unsigned kind = roc::msg_print_Human ) const;
      void Print32Bit(  unsigned kind = roc::msg_print_Human ) const;

      // Implement to remove warning
      void        Print(Option_t*) const {;}
      
      // All Get4 v1.0 24 bit messages
      inline void SetGet4Number( UChar_t ucGet4Index) { fxMessage.setGet4Number(ucGet4Index); }
      inline void SetEpoch2ChipNumber(UChar_t ucGet4Index) { fxMessage.setEpoch2ChipNumber(ucGet4Index); }

      // Get4 v1,0 32bits message function (later  general?)
      inline UInt_t getFieldBE(roc::Message rawMessage, UInt_t shift, UInt_t len) const
         { return (dataBE(rawMessage) >> shift) & ((((UInt_t)1) << len) - 1); }
      inline UChar_t getBitBE(roc::Message rawMessage, UInt_t shift) const
         { return (dataBE(rawMessage) >> shift) & 1; }
      inline ULong64_t dataBE( roc::Message rawMessage) const
         { return (((ULong64_t)rawMessage.getField(0,  8) &0x00000000000000FF)<<56)+
                  ( ((ULong64_t)rawMessage.getField(0, 16) &0x000000000000FF00)<<40)+
                  ( ((ULong64_t)rawMessage.getField(0, 24) &0x0000000000FF0000)<<24)+
                  ( ( ((ULong64_t)rawMessage.getField( 8, 24)  <<8 ) &0x00000000FF000000)<< 8)+
                  ( ( ((ULong64_t)rawMessage.getField(16, 24)) << 8)&0x00000000FF000000)+
                  ( ((ULong64_t)rawMessage.getField(24, 24) )&0x0000000000FF0000)+
                  ( ((ULong64_t)rawMessage.getField(40, 16) )&0x000000000000FF00)+
                  ( ((ULong64_t)rawMessage.getField(56,  8) )&0x00000000000000FF);
                  }

      inline UChar_t getGet4V10R32ChipId() const { return ( (fxMessage.getSysMesType() )&0xF ); }
      inline UChar_t getGet4V10R32MessageType() const { return getFieldBE(fxMessage, 46, 2); }
         // type 0 => Epoch message
      inline UInt_t  getGet4V10R32EpochNumber() const { return getFieldBE(fxMessage, 17,24); }
      inline Bool_t  getGet4V10R32SyncFlag()    const { return getBitBE(fxMessage, 16); }
         // type 1 => Slow control
      inline UInt_t  getGet4V10R32SlData()    const { return getFieldBE(fxMessage, 16,24); }
      inline UChar_t getGet4V10R32SlType()    const { return getFieldBE(fxMessage, 40, 2); }
      inline Bool_t  getGet4V10R32SlEdge()    const { return getBitBE(fxMessage, 42); }
      inline UChar_t getGet4V10R32SlChan()    const { return getFieldBE(fxMessage, 43, 2); }
         // type 2 => Error message
      inline UChar_t getGet4V10R32ErrorData() const { return getFieldBE(fxMessage, 16, 7); }
      inline UChar_t getGet4V10R32ErrorChan() const { return getFieldBE(fxMessage, 42, 2); }
      inline Bool_t  getGet4V10R32ErrorEdge() const { return getBitBE(fxMessage, 44); }
         // type 3 => Hit Data
      inline UChar_t getGet4V10R32HitTot()     const { return getFieldBE(fxMessage, 16, 8); }
      inline UChar_t getGet4V10R32HitFt()      const { return getFieldBE(fxMessage, 24, 7); }
      inline UInt_t  getGet4V10R32HitTs()      const { return getFieldBE(fxMessage, 31,12); }
      inline UInt_t  getGet4V10R32HitTimeBin()const { return getFieldBE(fxMessage, 24,19); }
      inline UChar_t getGet4V10R32HitChan()    const { return getFieldBE(fxMessage, 43, 2); }
      inline Bool_t  getGet4V10R32HitDllFlag() const { return getBitBE(fxMessage, 45); }

      void Set32BitFlag(){ fbIs32Bit = kTRUE; };
      Bool_t Is32Bit(){ return fbIs32Bit; };
      void SetEmptyFlag(){ fbIsEmpty = kTRUE; };
      Bool_t IsEmpty(){ return fbIsEmpty; };
   protected:

      /* original roc message*/
      roc::Message fxMessage;

      /* full time stamp without correction*/
      ULong64_t fuFulltime;
      Double_t  fdFullTime;

      /* corrected time difference to trigger time of master ROC
       * float range should be sufficient here*/
      float ffTriggerDeltaT;

      Bool_t         fbIs32Bit;
      UInt_t         uRocEpoch;
      UInt_t         uRocEpochCycle;

      Bool_t         fbIsEmpty;

      ClassDef(TGet4v1MessageExtended,1)
};

class Get4v1Hit : public TObject {
   public:
      Get4v1Hit();
      void Clear(Option_t *t="");
      void SetFullHit24Bits( TGet4v1MessageExtended& mesTime, TGet4v1MessageExtended& mesTot);
      void SetFullHit32Bits( TGet4v1MessageExtended& mesHit, Double_t d32bTotRange = 25500.0 );
      void SetTime24Bits( TGet4v1MessageExtended& mesTime );
      void SetTot24Bits(  TGet4v1MessageExtended& mesTot );

      TGet4v1MessageExtended GetTimeMessage() { return fTimeMessage; };
      TGet4v1MessageExtended GetTotMessage()  { return fTotMessage; };

      UChar_t  GetChan(){ return (kTRUE == fbIs32Bit? fTimeMessage.getGet4V10R32HitChan() : fTimeMessage.GetGet4ChNum()); };
      ULong_t  GetHitFullTime(){  return fTimeMessage.GetFullTime(); };
      Double_t GetHitFullTimeD(){ return fTimeMessage.GetFullTimeD();};
      UInt_t   GetHitEpoch(){ return fTimeMessage.GetRocEpoch(); };
      UInt_t   GetHitCycle(){ return fTimeMessage.GetRocCycle(); };

      Double_t GetHitTime();
      Double_t GetTot(){ return fdTot; };
      Int_t   GetHitLeadingFTS();
      Int_t   GetHitTrailingFTS();
      Double_t GetTimeDiff( Get4v1Hit& hitToCompare );

      Bool_t IsTimeSet(){   return fbTimeSet;};
      Bool_t IsTotSet(){    return fbTotSet;};
      Bool_t IsComplete(){  return fbTimeSet && fbTotSet;};
      Bool_t Is32Bit(){     return fbIs32Bit; };

      /*
      // copy constructor, used by vector to copy content for sorting
      Get4v1Hit(const Get4v1Hit& src) :
         fTimeMessage(src.fTimeMessage), fbTimeSet(src.fbTimeSet),
         fTotMessage(src.fTotMessage), fbTotSet(src.fbTotSet),
         fdTot(src.fdTot) {};
         */

   protected:
      Bool_t fbIs32Bit;
      void CalculateTot( Double_t d32bTotRange = 25500.0 );

      TGet4v1MessageExtended fTimeMessage;
      Bool_t fbTimeSet;
      TGet4v1MessageExtended fTotMessage;
      Bool_t fbTotSet;
      Double_t fdTot;

      ClassDef(Get4v1Hit,1)
};

class Get4v1Tdc : public TObject {
   public:
      Get4v1Tdc() : TObject() { Clear(); }
      ~Get4v1Tdc();
      void Clear(Option_t *t="");
      UInt_t Size();

      std::vector<Get4v1Hit> fHits[get4v10::kuNbChan];
/*
      // copy constructor
      Get4v1Tdc(const Get4v1Tdc& src);
      // Assignment operator
      Get4v1Tdc & operator=(const Get4v1Tdc& src);
*/
      ClassDef(Get4v1Tdc,1)
};

class Get4v1Event : public TObject {
   public:
      Get4v1Event();
      ~Get4v1Event();
      void Clear(Option_t *t="");
      UInt_t Size();
      Bool_t IsEmpty();
      Bool_t HasTrigger();
      Bool_t IsMbsSynced(){ return fbMbsSyncedEvent;};
      /*************************************/
      Get4v1Tdc fGet4Boards[get4v10::kuMaxGet4];

      std::vector<TGet4v1MessageExtended> fPureRocMessages[get4v10::kuMaxRoc];

      /* last time of trigger signal (defined by parameter)
       * can be sync or aux message, depending on TGet4Parameter::triggerSignal setup*/
      Double_t fdTriggerFullTime[get4v10::kuMaxRoc];

      // Event number of the corresponding MBS event
      UInt_t   fuMbsEventNumber;
      // Index of the real event inside MBS event
      UInt_t   fuEventNbInsideMbsEvt;
      // Total event number
      UInt_t   fuGlobalEventNumber;

      // Flag indicating that this real event is the one synchronized to MBS
      Bool_t   fbMbsSyncedEvent;
      // Event Nmber from the sync message
      UInt_t   fuSyncEventNumber;

      // Flag indicating that this real event was made after finding coincidence in data
      Bool_t   fbDataTriggered;
      // Index of the Data trigger making this event
      UInt_t   fuTriggerIndex;

      ClassDef(Get4v1Event,1)
};

#endif //TGET4V1EVENT_H



