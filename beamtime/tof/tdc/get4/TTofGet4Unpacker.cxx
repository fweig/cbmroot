// ------------------------------------------------------------------
// -----                     TTofGet4Unpacker                   -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

/*
 * Conversion of definitions to constants:
 *
MAX_ROC               get4v10::kuMaxRoc
MAX_GET4              get4v10::kuMaxGet4
MAX_GET4_PER_ROC      get4v10::kuMaxGet4Roc
MAX_AUX               get4v10::kuMaxAux
MAX_SYNC              get4v10::kuMaxSync
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

NB_CHAN_GET4          get4v10::kuNbChan instead of get4tdc::kuNbChan to accomodate 2tdc ch per input ch

psubevt->GetSubcrate()  fcSubCrate
psubevt->GetControl()   fcControl
psubevt->GetDlen()      uLength ???
psubevt->GetDataField() pMbsData

GetTriggerNumber()      Not available (TCBMBeamtimeProc method)
GetEventNumber()        Not available (TCBMBeamtimeProc method)
*/

#include "TTofGet4Unpacker.h"

// TOF headers
#include "TofDef.h"
#include "TMbsUnpackTofPar.h"
#include "TofTdcDef.h"
#include "TofGet4Def.h"
#include "TTofGet4Data.h"
#include "TTofGet4Board.h"

// FAIR headers
#include "FairRootManager.h"
#include "FairLogger.h"
#include "TString.h"

// ROOT headers
#include "TClonesArray.h"
#include "TH1.h"
#include "TROOT.h"
#include "TDirectory.h"

// Includes from old GO4 unpacker
   // ROOT
#include "Riostream.h"
#include "TH1.h"
#include "TH2.h"
#include "TTimeStamp.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TLatex.h"
   // GO4
//#include "TGo4MbsEvent.h"
//#include "TGo4WinCond.h"
//#include "TGo4Log.h"
//#include "TGo4Version.h"
   // GET4
#include "TGet4v1Param.h"
   // ROC
#include "roc/Message.h"
#include "roc/Board.h"
   // C++
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

TTofGet4Unpacker::TTofGet4Unpacker():
   fParUnpack(0),
   fiProcId( roc3587::proc_RocEvent ),
   fcSubCrate( 0 ),
   fcControl( roc::formatOptic2 ),
   fuNbTdc(0),
   fGet4BoardCollection(NULL),
   fbDataSavedForThisEvent(kFALSE),
   fb24bitsReadoutDetected(kFALSE),
   fb32bitsReadoutDetected(kFALSE),
   fParam(NULL),
   fCurrentGet4Event (),
   fIsTimeSorted(kFALSE),
   fEvntSize(NULL),
   fEvtPerProc(NULL),
   fEvtIndexRoc(NULL),
   fEvtIndexTrig(NULL),
   fEvtIndexVme(NULL),
   uInitialEvent(0),
   uEventCountsRoc(0),
   uEventCountsTrig(0),
   uEventCountsVme(0),
   fTriggerNumber(NULL),
   fMsgsPerRoc(NULL),
   fTriggerPerRoc(NULL),
   fDeltaTriggerTime(NULL),
   fGlobalTriggerWind(NULL),
   fGlobalAUXWind(NULL),
   fGlobalSelfTriggWind(NULL),
   fTotaldatasize(0),
   fCurrEvntTm(0),
   fLastRateTm(0.),
   fRate(0.),
   ROC(),
   fFirstSubEvent(kFALSE),
   fHasNewGlobalTrigger(kFALSE),
   fTriggerCounter(0),
   printData(0),
   fHitTemp()
{
}
TTofGet4Unpacker::TTofGet4Unpacker( TMbsUnpackTofPar * parIn, Short_t sProcId, Short_t sSubCrate, Short_t sControl  ):
   fParUnpack( parIn ),
   fiProcId( sProcId ),
   fcSubCrate( sSubCrate ),
   fcControl( sControl ),
   fuNbTdc( parIn->GetNbActiveBoards( tofMbs::get4 ) ),
   fGet4BoardCollection(NULL),
   fbDataSavedForThisEvent(kFALSE),
   fb24bitsReadoutDetected(kFALSE),
   fb32bitsReadoutDetected(kFALSE),
   fParam(NULL),
   fCurrentGet4Event (),
   fIsTimeSorted(kFALSE),
   fEvntSize(NULL),
   fEvtPerProc(NULL),
   fEvtIndexRoc(NULL),
   fEvtIndexTrig(NULL),
   fEvtIndexVme(NULL),
   uInitialEvent(0),
   uEventCountsRoc(0),
   uEventCountsTrig(0),
   uEventCountsVme(0),
   fTriggerNumber(NULL),
   fMsgsPerRoc(NULL),
   fTriggerPerRoc(NULL),
   fDeltaTriggerTime(NULL),
   fGlobalTriggerWind(NULL),
   fGlobalAUXWind(NULL),
   fGlobalSelfTriggWind(NULL),
   fTotaldatasize(0),
   fCurrEvntTm(0),
   fLastRateTm(0.),
   fRate(0.),
   ROC(),
   fFirstSubEvent(kFALSE),
   fHasNewGlobalTrigger(kFALSE),
   fTriggerCounter(0),
   printData(0),
   fHitTemp()
{
}
TTofGet4Unpacker::~TTofGet4Unpacker()
{
   // TODO: Histos file
//   WriteHistos();

   // not sure if it will not make problems for seeing them
//   DeleteHistos();
   ROC.clear();
}

void TTofGet4Unpacker::Clear(Option_t * /*option*/)
{
   fParUnpack = NULL;
   fuNbTdc    = 0;
   fGet4BoardCollection = NULL;
}
void TTofGet4Unpacker::ProcessGet4( UInt_t* pMbsData, UInt_t uLength, Char_t cSubCrate, Char_t cControl )
{
   fcSubCrate = cSubCrate;
   fcControl = cControl;
   ProcessGet4( pMbsData, uLength );
}


void TTofGet4Unpacker::ProcessGet4( UInt_t* pMbsData, UInt_t uLength )
{
//   if( (iTdcIndex<0) || (fuNbTdc <= iTdcIndex) || 
//       (fGet4BoardCollection->GetEntriesFast() <= iTdcIndex ))

   // Use a flag to insure only the first detected event is saved in each MBS event
   fbDataSavedForThisEvent = kFALSE;

/*   uint32_t g4id(0);*/ // Commented out as unused to remove warning
   uint32_t auxid(0);
/*   uint32_t syncid(0);*/ // Commented out as unused to remove warning
   uint64_t fulltm(0);
   uint32_t typ(0), rocid(0);
/*   uint32_t lastevrocid(0);*/ // Commented out as unused to remove warning
/*   static int cnt=0;*/ // Commented out as unused to remove warning
   unsigned msgcount=0;


   // TODO: Replace it with a parameter option
   // => cannot be detected in FAIRROOT scheme, so it is responsability
   // of the user to insure the proper sub-event type is provided to the unpacker
   bool is_dabc_evt = (fiProcId == roc3587::proc_RocEvent) ||
                      (fiProcId == roc3587::proc_ErrEvent) ||
                      (fiProcId == roc3587::proc_MergedEvent);
   bool is_raw_evt = (fiProcId == roc3587::proc_RawData);
   if( 1 == is_raw_evt && kFALSE == fParam->bRawDataMode )
   {
      fParam->bRawDataMode = kTRUE;
      cout<<" Found a RAW DATA event => switch RAW DATA mode ON"<<endl;
   }

   unsigned rid = fcSubCrate;

// BEGIN INITIAL EVENT PART
   int datasize = (uLength - 2) * 2;
//   int datasize = uLength * 2;
//   int msg_fmt = fcControl;
   roc::MessageFormat msg_fmt = ConvertToMsgFormat(fcControl);
   ROC[rid].fIter.setFormat(msg_fmt);
   ROC[rid].fIter.setRocNumber(rid);

   /** GET4 unpacking & pre-processing **/
   if( -1 != fParam->FindGet4RocId( fcSubCrate ) )
   {
      Int_t iFeetRocIndex = fParam->FindGet4RocId( fcSubCrate );
      if(uNbEvents[iFeetRocIndex] == 0) {
//         uFirstEventIndex[iFeetRocIndex] = GetEventNumber();
         TString sTemp = Form("***** First event on Get4 roc %d: %d ", iFeetRocIndex, uFirstEventIndex[iFeetRocIndex]);
         LOG(info)<<sTemp;
      } // if(uNbEvents[iFeetRocIndex] == 0)
      /*
      else {
         if( 0 < fParam->iEventNbGap &&
             (Int_t)uPrevEventIndex[iFeetRocIndex] + fParam->iEventNbGap != GetEventNumber() &&
             ( uPrevEventIndex[iFeetRocIndex] !=  fParam->uEventNbCycle || 0 != GetEventNumber() ) ) {
            TString sTemp = Form("***** Missing event in file for Get4 roc %d: Event %d, Previous event index %d, Current event Index %d, expected %d",
                  iFeetRocIndex, uNbEvents[iFeetRocIndex], uPrevEventIndex[iFeetRocIndex], GetEventNumber(),
                  (Int_t)uPrevEventIndex[iFeetRocIndex] + fParam->iEventNbGap);
            LOG(error)<<sTemp;
         } // if not matching expected event gap
      } // else of if(uNbEvents[iFeetRocIndex] == 0)
      uPrevEventIndex[iFeetRocIndex]  = GetEventNumber();
      */
      uNbEvents[iFeetRocIndex]++;
   } // if( -1 != fParam->FindGet4RocId( fcSubCrate ) )
   /*************************************/

   // we can exclude messages (SYNC and may be EPOCH) which are added by DABC at the end of subevent
   if (is_dabc_evt)
   {
      // ignore all irrelevant events
      /*
      if (GetTriggerNumber()>11)
      {
         cout<<"OUPS, trigger too high: "<<GetTriggerNumber()<<endl;
         return;
      }
      */

      roc::Message msg1, msg2;
      roc::Message msgA, msgB;

      char* ptr = (char*) pMbsData;
      int msglen = roc::Message::RawSize(msg_fmt);

      if (ROC[rid].fIter.assign(ptr + datasize - msglen, msglen)) {
         if (ROC[rid].fIter.next()) msg1.assign(ROC[rid].fIter.msg());
      }

      if (!msg1.isSyncMsg()) {
         LOG(error)<<"Non-SYNC message at the end of DABC-produced subevent";
      } else {
         rocid = msg1.getRocNumber();
         /*
          * TODO: Option to choose between removing final epoch+SYNC or initial epoch+SYNC
          */
/*         UInt_t uSyncId    = msg1.getSyncChNum();*/ // Commented out as unused to remove warning
/*         UInt_t uSyncTs    = msg1.getSyncTs();*/ // Commented out as unused to remove warning
/*         UInt_t uSyncEpLsb = msg1.getSyncEpochLSB();*/ // Commented out as unused to remove warning
/*         UInt_t uSyncData  = msg1.getSyncData();*/ // Commented out as unused to remove warning
/*         UInt_t uSyncFlag  = msg1.getSyncStFlag();*/ // Commented out as unused to remove warning
         ULong64_t ulFulltime = msg1.getMsgFullTime( ROC[rocid].GetFullEpochNumber( ) );
         ulFulltime += ROC[rocid].GetSuperCycleEpOffset();
/*
         cout<<"End Event "<<GetEventNumber()<<" Sync Id "<<uSyncId<<" Ts "<<uSyncTs;
         cout<<" EpLsb "<<uSyncEpLsb<<" Data "<<uSyncData<<" Flag "<<uSyncFlag;
         cout<<" FullT "<<ulFulltime<<endl;
         */
         /*
          *
          */

         // kFALSE = SYNC arriving before the event data, kTRUE = after
         // => if kTRUE, keep last 2 message and throw 2 first!
         if( kFALSE == fParam->bTriggerAfterData )
            datasize -= msglen;
         int shift = msglen;
         while (shift<=datasize) {

            if (ROC[rid].fIter.assign(ptr + datasize - shift, msglen)) {
               if (ROC[rid].fIter.next())
                  msg2.assign(ROC[rid].fIter.msg());
               else
                  break;

               if (msg2.isEpochMsg())
               {
                  /*
                  cout<<"End Event "<<GetEventNumber()<<" Epoch "<<msg2.getEpochNumber()<<endl;
                  */
                  break;
               }
            }
            shift+=msglen;
         }
         // exclude special epoch message for sync only
         // kFALSE = SYNC arriving before the event data, kTRUE = after
         // => if kTRUE, keep last 2 message and throw 2 first!
         if( kFALSE == fParam->bTriggerAfterData )
            if ((shift==msglen) && msg2.isEpochMsg()) datasize -= msglen;
      } // else of if (!msg1.isSyncMsg())
   } // if (is_dabc_evt)

   fTotaldatasize += datasize;

   if (!ROC[rid].fIter.assign(pMbsData, datasize))
   {
      cout<<"OUPS, probblem: "<<pMbsData<<" "<<datasize<<endl;
      return;
   } // if (!ROC[rid].fIter.assign(pMbsData, datasize))

   fEvntSize->Fill(datasize);

   //cout << "AnlProc: found subevent subcrate="<<(int) fcSubCrate<<", procid=";
   //cout<<(int)psubevt->GetProcid()<<", control="<<(int) psubevt->GetControl()<< endl;
// END INITIAL EVENT PART

   /* TODO FILL ME
    * Buffer processing
    */

   roc::Message* data = & ROC[rid].fIter.msg();
   while (ROC[rid].fIter.next()) {
      msgcount++;

      rocid = data->getRocNumber();
      fMsgsPerRoc->Fill(rocid);

      // Process data only from active ROCs
      if( kFALSE == fParam->IsActiveRoc(rocid) )
         continue;

      typ = data->getMessageType();
      if(typ==roc::MSG_GET4)
      {
         fulltm = data->getMsgFullTime(
               ROC[rocid].GetFullEpoch2Number(
                     fParam->RemapGet4Chip( data->getGet4Number() ) ));
         fulltm +=  ROC[rocid].GetSuperCycleEp2Offset(
                     fParam->RemapGet4Chip( data->getGet4Number() ) );
      }
      else
      {
         fulltm = data->getMsgFullTime(
               ROC[rocid].GetFullEpochNumber() );
         fulltm +=  ROC[rocid].GetSuperCycleEpOffset( );

      }

      ROC[rocid].fMsgTypes->Fill(typ);

      ULong64_t  ulMsgTm = fulltm / 100000000L;
      ROC[rocid].fALLt->Fill( (ulMsgTm % 36000) * 0.1 );

      // Long duration time distribution (1 min. bin, 10 days length)
      if( kTRUE == fParam->bLongTimeHistos )
         ROC[rocid].fAllLongT->Fill( ( ulMsgTm * 0.1 ) /3600);

      TGet4v1MessageExtended exmess(*data,fulltm);

      switch (typ)
      {
         case roc::MSG_NOP:
         {
            break;
         } // case roc::MSG_NOP:
         case roc::MSG_HIT:
         {
            break;
         } // case roc::MSG_HIT:
         case roc::MSG_GET4:
         {
            Double_t dFullTimeD = data->getMsgFullTimeD(
                                    ROC[rocid].GetFullEpoch2Number(
                                             fParam->RemapGet4Chip( data->getGet4Number() ) ) );
            dFullTimeD += ROC[rocid].GetSuperCycleEp2OffsetD( fParam->RemapGet4Chip( data->getGet4Number() ) );
            exmess.SetFullTimeD( dFullTimeD );
            // In case of suppressed epochs, we will have no epoch2 message for all empty epochs
            // However, when a chip has data, its next epoch message should be present!!!
            // => Specific function to wait for next epoch2
            if( kTRUE == fParam->bSuppressedEpochs)
               ProcessGet4SuppDataMessage(rocid, exmess);
            else if( kTRUE == ProcessGet4DataMessage(rocid, exmess) )
               ProcessExtendedMessage(rocid, exmess);
            break;
         } // case roc::MSG_GET4:
         case roc::MSG_EPOCH:
         {
            /*
            if((msgcount<3) && !is_raw_evt)
            {
               cout<<"Beg Event "<<GetEventNumber()<<" Epoch "<<data->getEpochNumber()<<endl;
            }
            */
            if( !is_dabc_evt || kFALSE == fParam->bTriggerAfterData ||
                 2 < msgcount)
               ProcessRocEpochMessage(rocid, exmess);
            break;
         } // case roc::MSG_EPOCH:
         case roc::MSG_EPOCH2:
         {
            if( kTRUE == fParam->bSuppressedEpochs)
               ProcessGet4SuppEpochMessage(rocid, exmess);
               else ProcessGet4EpochMessage(rocid, exmess);
            break;
         } // case roc::MSG_EPOCH2:
         case roc::MSG_SYNC:
         {
/*            UInt_t uSyncId    = data->getSyncChNum();*/ // Commented out as unused to remove warning
/*            UInt_t uSyncTs    = data->getSyncTs();*/ // Commented out as unused to remove warning
/*            UInt_t uSyncEpLsb = data->getSyncEpochLSB();*/ // Commented out as unused to remove warning
/*            UInt_t uSyncData  = data->getSyncData();*/ // Commented out as unused to remove warning
/*            UInt_t uSyncFlag  = data->getSyncStFlag();*/ // Commented out as unused to remove warning
            ULong64_t ulFulltime = data->getMsgFullTime( ROC[rocid].GetFullEpochNumber( ) );
            ulFulltime += ROC[rocid].GetSuperCycleEpOffset();

            if( kFALSE == fParam->bTriggerAfterData )
            {
               if((msgcount<3) && !is_raw_evt)
               {
                  ROC[rocid].fDabcSeparator = data->getSyncChNum()+10;
               }
               ProcessRocSyncMessage(rocid, exmess);
            } // if( kFALSE == fParam->bTriggerAfterData )
               else if( (2 < msgcount ) || !is_dabc_evt)
               {
                  if( ( (datasize - 3 ) < (Int_t)msgcount) && !is_raw_evt)
                  {
                     ROC[rocid].fDabcSeparator = data->getSyncChNum()+10;
                  }
                  ProcessRocSyncMessage(rocid, exmess);
               } // else if( (2 < msgcount ) || !is_dabc_evt) of if( kFALSE == fParam->bTriggerAfterData )
            break;
         } // case roc::MSG_SYNC:
         case roc::MSG_AUX:
         {
            auxid = data->getAuxChNum();
            ROC[rocid].fAUXch->Fill(auxid);
            if (auxid < get4v10::kuMaxAux)
            {
               ROC[rocid].fLastAuxTm[auxid] = fulltm;

               // Recalculate the full Time to take into account also the epoch cycles
               ULong64_t ulFulltime ;
               ulFulltime  = (exmess.GetRocMessage()).getMsgFullTime(
                                   ROC[rocid].GetFullEpochNumber( ));
               ulFulltime += ROC[rocid].GetSuperCycleEpOffset();
               exmess.SetFullTime(ulFulltime);

               ULong64_t auxtm = fulltm / 100000000L;
               ROC[rocid].fAUXt[auxid]->Fill((auxtm % 10000) * 0.1);

               if( kFALSE ==  fParam->bFreeStreaming || 0 == fParam->uNbTriggers )
                  if (fParam->triggerSignal == (Int_t) auxid)
                  {
                     ProcessTriggerMessage(rocid, exmess);
                  }
            } // if (auxid < get4v10::kuMaxAux)

            break;
         } // case roc::MSG_AUX:
         case roc::MSG_SYS:
         {
            ROC[rocid].fSysTypes->Fill(data->getSysMesType());

            UShort_t ucSysMesType = data->getSysMesType(); // change type to avoid warning if last Chip id reach char type size
      
            if (data->getSysMesType() == roc::SYSMSG_USER)
            {
               ROC[rocid].fSysUserTypes->Fill(data->getSysMesData());

               switch (data->getSysMesData())
               {
                  case roc::SYSMSG_USER_CALIBR_ON:
                     ROC[rocid].bIgnoreData = kFALSE;
                     break;
                  case roc::SYSMSG_USER_CALIBR_OFF:
                     ROC[rocid].bIgnoreData = kFALSE;
                     break;
                  case roc::SYSMSG_USER_RECONFIGURE:
                     ROC[rocid].bIgnoreData = kTRUE;
                     ROC[rocid].fIgnoreCnt = 0;
                     ROC[rocid].fIgnoreTime = fulltm;
                     break;
                  default:
                     break;
               } // switch (data->getSysMesData())
            } // if (data->getSysMesType() == roc::SYSMSG_USER)
            else if( data->getSysMesType() == roc::SYSMSG_GET4_EVENT )
            {
/*               UInt_t uGet4SysMessType = (data->getSysMesData()>>6) & 0x1;*/ // Commented out as unused to remove warning

               // Get4 system message types histogram
               /*
               if (ROC[rocid].fGet4SysTypes)
                  ROC[rocid].fGet4SysTypes->Fill(uGet4SysMessType);
                  */

               UInt_t uGet4IndexOffset = fParam->DefineGet4IndexOffset(rocid);
               // Remap the Get4 chip index
               UInt_t uChip = data->getField(40, 8) + uGet4IndexOffset;
               uChip = fParam->RemapGet4Chip(uChip);
               // Set Remapped Get4 index
               data->setField(40, 8, uChip);

               // Get4 system message type = Error
               if (!fParam->IsValidGet4Chip(uChip)) {
                   cout << "Error: Bad chip nb in Error message = " << uChip << endl;
                   cout << " => This message will be skipped!!!! "<<endl;
                   continue;
               }
/*
               fGet4ErrorChip->Fill( uChip );
               fGet4ErrorPatt->Fill( uChip, (data->getSysMesData()) & 0x7f);
               if( 3 < ( (data->getSysMesData()) & 0x7f ) && ( (data->getSysMesData()) & 0x7f ) < 7 )
                  fGet4ErrorChan->Fill( (((data->getSysMesData()) & 0x300)>>16) + 0.5*(((data->getSysMesData()) & 0x80)>>15),
                       ((data->getSysMesData()) & 0x7f )>>2 );
*/
               UInt_t get4_24b_er_code = exmess.getGet4V10R32ErrorData();
               UInt_t get4_24b_er_chan = exmess.getGet4V10R32ErrorChan();
               UInt_t get4_24b_er_edge = exmess.getGet4V10R32ErrorEdge();

               ROC[rocid].fGet4ErrorChip->Fill( uChip );
               ROC[rocid].fGet4ErrorPatt->Fill( uChip, get4_24b_er_code);
               if( (0x03 < get4_24b_er_code && get4_24b_er_code < 0x07) )
                  ROC[rocid].fGet4ErrorChan->Fill(
                        get4v10::kuNbChan*uChip + get4_24b_er_chan + 0.5*get4_24b_er_edge,
                        get4_24b_er_code - 0x04 );

               ULong64_t uFullTimeBins = ((ULong64_t)  ROC[rocid].GetFullEpoch2Number( uChip )) << 19;
               uFullTimeBins /= 20; // now in ns
               uFullTimeBins += 512; // 250MHz offset
               uFullTimeBins /= 100000000L;
               ROC[rocid].fERRORt->Fill( (uFullTimeBins % 36000) * 0.1 );

               // Long duration time distribution (1 min. bin, 10 days length)
               if( kTRUE == fParam->bLongTimeHistos )
                  ROC[rocid].fErrorLongT->Fill( (uFullTimeBins * 0.1 ) / 3600 );

            } // if( data->getSysMesType() == roc::SYSMSG_GET4_EVENT )
            else if( get4v10::SYSMSG_GET4V1_32BIT_0 <= data->getSysMesType()   &&
                     ucSysMesType <= get4v10::SYSMSG_GET4V1_32BIT_15 )
            {
               if( 3 == Process32BitGet4Message(rocid, exmess) )
                  ProcessExtendedMessage(rocid, exmess);
            } // if GET4 32 bits message
            break;
         } //  case roc::MSG_SYS:
         default:
            break;
      } // switch (typ)

      if(printData<fParam->uGet4Print  )
      {
         exmess.Print(roc::msg_print_Human);
         printData++;
      }
   } // while (ROC[rid].fIter.next()) {
   /*
    * Buffer processing End
    */

   fFirstSubEvent = kFALSE;

   if( kFALSE == fbDataSavedForThisEvent )
   {
      // First event found in this MBS event
      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
      {
         TTofGet4Board * fGet4Board = (TTofGet4Board*) fGet4BoardCollection->ConstructedAt(uGet4);

         // Check if there is an unfinished event and save its data if it is the case
         if( kFALSE == fCurrentGet4Event.IsEmpty() )
         {
            if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
            {
               fGet4Board->SetTriggerFullTime( fCurrentGet4Event.fdTriggerFullTime[0] ); // Use only first ROC!!!!
               fGet4Board->SetSyncEventFlag( fCurrentGet4Event.fbMbsSyncedEvent );
               fGet4Board->SetSyncEvtNb( fCurrentGet4Event.fuSyncEventNumber );
               fGet4Board->SetDataEventFlag( fCurrentGet4Event.fbDataTriggered );
               fGet4Board->SetTriggIndx( fCurrentGet4Event.fuTriggerIndex );

               // Some Data found for this TDC Chip
               for( UInt_t uCh = 0; uCh < get4v10::kuNbChan; uCh++ )
                  for( UInt_t uHit = 0;
                       uHit < ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh]).size();
                       uHit++ )
                  {
                     TGet4v1MessageExtended extTime = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTimeMessage();
                     TGet4v1MessageExtended extTot  = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTotMessage();

                     if( kFALSE == extTime.IsEmpty() )
                     {
                        TTofGet4Data dataTime;
                        dataTime.SetEdge( 0 );
                        if( kTRUE == extTime.Is32Bit() )
                        {
                           dataTime.SetChannel( uCh );
                           dataTime.SetFineTime(   extTime.getGet4V10R32HitFt() );
                           dataTime.SetCoarseTime( extTime.getGet4V10R32HitTs() );
                           dataTime.SetTot(        extTime.getGet4V10R32HitTot() );
                        } // if( kTRUE == extTime.Is32Bit() )
                           else
                           {
                              // Time saved in "odd channels" as example of VFTX
                              dataTime.SetChannel( 2*uCh + 1 );
                              dataTime.SetFineTime(    extTime.GetGet4Ts() & get4tdc::kiFineTime );
                              dataTime.SetCoarseTime( (extTime.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                           } // else of if( kTRUE == extTime.Is32Bit() )
                        dataTime.SetEpoch( extTime.GetRocEpoch() );
                        dataTime.SetEpochCycle( extTime.GetRocCycle() );
                        dataTime.SetFullTimeD( extTime.GetFullTimeD() );
                        dataTime.Set32bFlag( extTime.Is32Bit() );
                        fGet4Board->AddData( dataTime );
                     } // if( kFALSE == extTime.IsEmpty() )

                     if( kFALSE == extTot.IsEmpty() )
                     {
                        // Will appear only in case of 24bit mode !!!
                        TTofGet4Data dataTot;
                        // Tot saved in "even channels" as example of VFTX
                        dataTot.SetChannel( 2*uCh );
                        dataTot.SetEdge( 1 );
                        dataTot.SetFineTime(    extTot.GetGet4Ts() & get4tdc::kiFineTime );
                        dataTot.SetCoarseTime( (extTot.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                        dataTot.SetEpoch( extTot.GetRocEpoch() );
                        dataTot.SetEpochCycle( extTot.GetRocCycle() );
                        dataTot.SetFullTimeD( extTot.GetFullTimeD() );
                        dataTot.Set32bFlag( extTot.Is32Bit() );
                        fGet4Board->AddData( dataTot );
                     } // if( kFALSE == extTot.IsEmpty() )
                  } // scan over all hits in this board object

            } // if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
         } // if( kFALSE == fCurrentGet4Event.IsEmpty() )
      } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
      fbDataSavedForThisEvent = kTRUE;
   } // if( kFALSE == fbDataSavedForThisEvent )
   return;
} 

Bool_t TTofGet4Unpacker::Init( )
{
   if( kFALSE == InitParameters() )
      return kFALSE;
   if( kFALSE == InitOutput() )
      return kFALSE;
   if( kFALSE == CreateHistos() )
      return kFALSE;
   if( kFALSE == InitConditions() )
      return kFALSE;
      
   return kTRUE;   
}
Bool_t TTofGet4Unpacker::InitParameters( )
{

//   fParam = (TGet4v1Param *) MakeParameter("Get4v1Par", "TGet4v1Param" /*, "set_RocPar.C" */);
   fParam = new TGet4v1Param("Get4v1Par");
   gROOT->GetListOfSpecials()->Add(fParam); // Register param so it can be retrieved by gROOT->FindObject
   

   fLastRateTm = TTimeStamp().AsDouble();
   fTotaldatasize = 0;
   fRate = 0.;
   
   // here we optionally override parameter values with setup macro, if existing:
   TString setupmacro = "set_Get4v1Par.C";
   if (!gSystem->AccessPathName(setupmacro.Data())) {
      TString sTemp = Form("Executing ROC parameter setup script %s", setupmacro.Data());
      LOG(info)<<sTemp;
      gROOT->ProcessLine(Form(".x %s( )", setupmacro.Data()));
      // need to copy parameter setup to static event array,
      // when restored from Autosave file, executed in UpdateFrom method
      fParam->SetConfigRocs();
   } else {
      TString sTemp = Form("NO ROC parameter setup script %s. Use previous values!", setupmacro.Data());
      LOG(info)<<sTemp;
   }

   fParam->PrintGet4Options();
   if (fParam->numRocs > get4v10::kuMaxRoc) fParam->numRocs = get4v10::kuMaxRoc;
   if (fParam->uNbGet4 > get4v10::kuMaxGet4) fParam->uNbGet4 = get4v10::kuMaxGet4;

   printData = 0;


   ResetEndOfBuffer();
   fCurrentGet4Event.Clear();

   uInitialEvent = 0;
   uEventCountsRoc = 0;
   uEventCountsTrig = 0;
   uEventCountsVme = 0;

   return kTRUE;
}
Bool_t TTofGet4Unpacker::InitConditions()
{
//   fGlobalTriggerWind = MakeWinCond("TriggerWindow", -100, 2000., fDeltaTriggerTime->GetName());
//   fGlobalAUXWind     = MakeWinCond("AUXWindow",     -100, 100.,  fDeltaTriggerTime->GetName());
   fGlobalTriggerWind = new TimeWindowCond( "TriggerWindow", -100., 2000.);
   gROOT->GetListOfSpecials()->Add(fGlobalTriggerWind); // Register Condition so it can be retrieved by gROOT->FindObject
   fGlobalAUXWind     = new TimeWindowCond( "AUXWindow",     -100.,  100.);
   gROOT->GetListOfSpecials()->Add(fGlobalAUXWind); // Register Condition so it can be retrieved by gROOT->FindObject

   TString setupmacro = "set_Get4v1Cond.C";
   if (!gSystem->AccessPathName(setupmacro.Data())) {
      TString sTemp = Form("Executing ROC condition setup script %s", setupmacro.Data());
      LOG(info)<<sTemp;
      gROOT->ProcessLine(Form(".x %s()", setupmacro.Data() ) ); //
//      gROOT->ProcessLine(Form(".x %s( %p, %p, %p, %p )", setupmacro.Data(),
//            fGlobalTriggerWind, fGlobalAUXWind, ROC[0].fTriggerWind, ROC[0].fAUXWind)); // Only ROC00!!
   } else {
      TString sTemp = Form("NO ROC condition setup script %s. Use previous values!", setupmacro.Data());
      LOG(info)<<sTemp;
   }

   return kTRUE;
}

Bool_t TTofGet4Unpacker::InitOutput()
{
   // Recover first the GET4 board objects created in general unpacker class
   FairRootManager* rootMgr = FairRootManager::Instance();

   fGet4BoardCollection = (TClonesArray*) rootMgr->GetObject("TofGet4Tdc");
   if(NULL == fGet4BoardCollection)
   {
      LOG(warn)<<"TTofGet4Unpacker::TTofGet4Unpacker : no TOF GET4 TDC array! ";
      fuNbTdc = 0;
      return kFALSE;
   } // if(NULL == fGet4BoardCollection)
   return kTRUE;
}

// Histogram management
Bool_t TTofGet4Unpacker::CreateHistos()
{
   LOG(debug)<<" TTofGet4Unpacker::CreateHistos Nb boards: "<<fuNbTdc<<" Directory ";

   TDirectory * oldir = gDirectory; // <= To prevent histos from being sucked in by the param file of the TRootManager!
   gROOT->cd(); // <= To prevent histos from being sucked in by the param file of the TRootManager !

   for (unsigned n=0; n < fParam->numRocs;n++) {
         ROC.push_back(TGet4v1Rec());
         ROC[n].fRocId = n;
   }

   fEvntSize = new TH1I("EvntSize", "Number of messages in event",
         250, 1., 2000.);

   fEvtPerProc= new TH1I("SubEventsPerProc", "Number of subevents per Proc index",
         64, 0., 64);

   fMsgsPerRoc = new TH1I("MsgsPerRoc", "Number of messages per ROC",
         fParam->numRocs, 0., fParam->numRocs);

   fTriggerPerRoc = new TH1I("TriggersPerRoc", "Number of Trigger messages per ROC",
         fParam->numRocs, 0., fParam->numRocs);

   fDeltaTriggerTime = new TH1I("TriggerDeltaTime", "Corrected hits time difference of all ROCs; Delta t",
         5000, -5000., 5000.);

   for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
   {
      // do not build histograms for non-used ROC
      if (!fParam->IsActiveRoc(uRoc)) continue;

      char folder[30];
      sprintf(folder,"ROC%u/",uRoc);

      ROC[uRoc].fMsgTypes = new TH1I( Form("MsgTypes%u", uRoc),
            Form("ROC%u Distribution of messages types", uRoc),
            8, 0., 8.);

//      if (IsObjMade())
      {
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_NOP, "NOP");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_HIT, "HIT");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_EPOCH, "EPOCH");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_SYNC, "SYNC");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_AUX, "AUX");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_EPOCH2, "EPOCH2");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_GET4, "GET4");
         ROC[uRoc].fMsgTypes->GetXaxis()->SetBinLabel(1 + roc::MSG_SYS, "SYS");
      }

      ROC[uRoc].fSysTypes = new TH1I( Form("SysTypes%u", uRoc),
            Form("ROC%u Distribution of system messages", uRoc),
            roc::SYSMSG_GET4_EVENT+1, 0., roc::SYSMSG_GET4_EVENT+1);

//      if (IsObjMade())
      {
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_DAQ_START,  "DAQ_START");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_DAQ_FINISH, "DAQ_FINISH");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_NX_PARITY,  "NX_PARITY");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_SYNC_PARITY,"SYNC_PARITY");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_DAQ_RESUME, "DAQ_RESUME");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_FIFO_RESET, "FIFO_RESET");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_USER,       "USER_MSG");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_PCTIME,     "PCTIME");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_ADC,        "ADC");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_PACKETLOST, "PACKETLOST");
         ROC[uRoc].fSysTypes->GetXaxis()->SetBinLabel(1 + roc::SYSMSG_GET4_EVENT, "GET4_EVENT");
      }

      ROC[uRoc].fSysUserTypes = new TH1I( Form("SysUserTypes%u", uRoc),
            Form("ROC%u Distribution of user messages", uRoc),
            8, 7., 15.);
//      if (IsObjMade())
      {
         ROC[uRoc].fSysUserTypes->GetXaxis()->SetBinLabel(roc::SYSMSG_USER_CALIBR_ON - 6, "CALIBR_ON");
         ROC[uRoc].fSysUserTypes->GetXaxis()->SetBinLabel(roc::SYSMSG_USER_CALIBR_OFF - 6, "CALIBR_OFF");
         ROC[uRoc].fSysUserTypes->GetXaxis()->SetBinLabel(roc::SYSMSG_USER_RECONFIGURE - 6, "RECONFIGURE");
      }

      ROC[uRoc].fAUXch = new TH1I( Form("Roc%u_aux_chs", uRoc),
            "Number events per aux channel",
            8, 0., 8.);

      ROC[uRoc].fTrigger_All = new TH1I( Form("Roc%u_Trigger", uRoc),
            Form("Time difference between all hits and last trigger signal on ROC%u", uRoc),
            5000, -1000., 4000.);
      ROC[uRoc].fTrigger_All_100 = new TH1I( Form("Roc%u_Trigger_100", uRoc),
            Form("Time difference between all hits and last trigger signal on ROC%u", uRoc),
            50005, -100000., 1e6);
      ROC[uRoc].fTrigger_All_us = new TH1I( Form("Roc%u_Trigger_us", uRoc),
            Form("Time difference between all hits and last trigger signal on ROC%u", uRoc),
            1000, -1000., 999.*1e3);
      ROC[uRoc].fTrigger_All_ms = new TH1I( Form("Roc%u_Trigger_ms", uRoc),
            Form("Time difference between all hits and last trigger signal on ROC%u", uRoc),
            1000, -1e6, 999*1e6);
      ROC[uRoc].fTrigger_All_s = new TH1I( Form("Roc%u_Trigger_s", uRoc),
            Form("Time difference between all hits and last trigger signal on ROC%u", uRoc),
            1000, -1e9, 999*1e9);

      ROC[uRoc].fTrigger_AUX = new TH1I( Form("Roc%u_Trigger_AUX", uRoc),
            Form("Time difference between all AUXs and last trigger signal on ROC%u", uRoc),
            5000, -5000., 5000.);

      ROC[uRoc].fEvntMultipl = new TH1I( Form("Roc%u_Multiplicity", uRoc),
            Form("Event multiplicity for ROC%u in time window", uRoc),
            16, 0., 16.);

      ROC[uRoc].fALLt = new TH1I( Form("Roc%u_ALL_t",uRoc), Form("Time distribution of ALL signals on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fDATAt = new TH1I( Form("Roc%u_DATA_t",uRoc), Form("Time distribution of DATA hits on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fERRORt = new TH1I( Form("Roc%u_ERROR_t",uRoc), Form("Time distribution of ERROR signals on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fSLOWCt = new TH1I( Form("Roc%u_SLOWC_t",uRoc), Form("Time distribution of SLOW CONTROL signals on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fSelfTriggT = new TH1I( Form("Roc%u_SelfTrigg_t",uRoc), Form("Time distribution of self triggers on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fEPOCHt = new TH1I( Form("Roc%u_EPOCH_t",uRoc), Form("Time distribution of Epochs on ROC%u; s", uRoc),
            36000, 0., 3600.);

      ROC[uRoc].fAllSelectedT = new TH1I( Form("Roc%u_AllSel_t",uRoc), Form("Time distribution of all selected hits on ROC%u; s", uRoc),
            36000, 0., 3600.);

      for (unsigned nsync=0; nsync<get4v10::kuMaxSync; nsync++)
         ROC[uRoc].fSYNCt[nsync] = new TH1I( Form("Roc%u_Sync%u_t", uRoc, nsync),
               Form("Time distribution of SYNC%u signal on ROC%u; s", nsync, uRoc),
               36000, 0., 3600.);

      for (unsigned naux=0; naux<get4v10::kuMaxAux; naux++)
         ROC[uRoc].fAUXt[naux] = new TH1I( Form("Roc%u_Aux%u_t", uRoc, naux),
               Form("Time distribution of AUX%u signal on ROC%u; s", naux, uRoc),
               36000, 0., 3600.);

      // Long duration time distribution (1 min. bin, 10 days length)
      if( kTRUE == fParam->bLongTimeHistos )
      {
         ROC[uRoc].fAllLongT = new TH1I( Form("Roc%u_All_lt",uRoc), Form("Time distribution of ALL signals on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fDataLongT = new TH1I( Form("Roc%u_Data_lt",uRoc), Form("Time distribution of DATA hits on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fErrorLongT = new TH1I( Form("Roc%u_Error_lt",uRoc), Form("Time distribution of ERROR signals on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fSlowCLongT = new TH1I( Form("Roc%u_Slowc_lt",uRoc), Form("Time distribution of SLOW CONTROL signals on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fSelfTriggLongT = new TH1I( Form("Roc%u_SelfTrigg_lt",uRoc), Form("Time distribution of self triggers on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fEpochLongT = new TH1I( Form("Roc%u_Epoch_lt",uRoc), Form("Time distribution of Epochs on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fAllEpoch2LongT = new TH1I( Form("Roc%u_AllEp2_lt",uRoc), Form("Time distribution of all epoch2 on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         ROC[uRoc].fAllSelLongT = new TH1I( Form("Roc%u_AllSel_lt",uRoc), Form("Time distribution of all selected hits on ROC%u; t [h]", uRoc),
               18720, 0., 312.);

         for (unsigned nsync=0; nsync<get4v10::kuMaxSync; nsync++)
            ROC[uRoc].fSyncLongT[nsync] = new TH1I( Form("Roc%u_Sync%u_lt", uRoc, nsync),
                  Form("Time distribution of SYNC%u signal on ROC%u; t [h]", nsync, uRoc),
                  18720, 0., 312.);
      } // if( kTRUE == fParam->bLongTimeHistos )

      /** GET4 unpacking & pre-processing **/
      uNbEvents[uRoc]        = 0;
      uFirstEventIndex[uRoc] = 0;
      uPrevEventIndex[uRoc]  = 0;

      // Real events counters
      uEventInsideMbsEvtCount[uRoc] = 0;
      uRealEventsCount[uRoc] = 0;
      uNotEmptyEventInsideMbsEvtCount[uRoc] = 0;
      uNotEmptyRealEventsCount[uRoc] = 0;

//      ROC[uRoc].fTriggerWind = MakeWinCond(Form("Roc%u_EventWindow",uRoc),
//            485., 885., ROC[uRoc].fTrigger_All->GetName());
//      ROC[uRoc].fAUXWind = MakeWinCond(Form("Roc%u_AUXWindow",uRoc),
//            -100., 100., ROC[uRoc].fTrigger_AUX->GetName());

      ROC[uRoc].fTriggerWind = new TimeWindowCond( Form("Roc%u_EventWindow",uRoc), 485., 885. );
      gROOT->GetListOfSpecials()->Add(ROC[uRoc].fTriggerWind); // Register Condition so it can be retrieved by gROOT->FindObject
      ROC[uRoc].fAUXWind     = new TimeWindowCond( Form("Roc%u_AUXWindow",uRoc),  -100., 100. );
      gROOT->GetListOfSpecials()->Add(ROC[uRoc].fAUXWind); // Register Condition so it can be retrieved by gROOT->FindObject

      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      {
         char folderGet4[30];
         sprintf(folderGet4,"%sGet4_%u/",folder, uGet4);
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRoc, uGet4);
         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
         {
            ROC[uRoc].fEPOCH2t[uGet4] = new TH1I( Form("Roc%u_EPOCH2_t_%u",uRoc, uGet4),
                  Form("Time distribution of GET4 Epochs on ROC%u Get4 %u; s", uRoc, uGet4),
                  36000, 0., 3600.);
            for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
            {
               ROC[uRoc].fSelectedT[uGet4][uGet4Chan] = new TH1I( Form("Roc%u_Selected_t_%u_Ch%u",uRoc, uGet4, uGet4Chan),
                     Form("Time distribution of selected hits on ROC%u Get4 %u Channel %u; s", uRoc, uGet4, uGet4Chan),
                     36000, 0., 3600.);

               ROC[uRoc].fTrigger_Get4Channel[uGet4][uGet4Chan] =
                     new TH1I( Form("Roc%u_Get%u_Ch%u_Trigger",
                                    uRoc, uGet4, uGet4Chan),
                              Form("Time difference between all hits on GET4 %u Channel %u and last trigger signal on ROC%u; [ns]",
                                    uGet4, uGet4Chan, uRoc),
                              10000, -5000., 5000.);
               ROC[uRoc].fTriggerMs_Get4Channel[uGet4][uGet4Chan] =
                     new TH1I( Form("Roc%u_Get%u_Ch%u_TriggerMs",
                                    uRoc, uGet4, uGet4Chan),
                              Form("Time difference between all hits on GET4 %u Channel %u and last trigger signal on ROC%u; [ns]",
                                    uGet4, uGet4Chan, uRoc),
                              20000, -100*1e3, 100*1e3);
               ROC[uRoc].fTriggerS_Get4Channel[uGet4][uGet4Chan] =
                     new TH1I( Form("Roc%u_Get%u_Ch%u_TriggerS",
                                    uRoc, uGet4, uGet4Chan),
                              Form("Time difference between all hits on GET4 %u Channel %u and last trigger signal on ROC%u; [ns]",
                                    uGet4, uGet4Chan, uRoc),
                              20000, -100*1e9, 1900*1e9);

               /*
                * Debug Histograms for GET4 v1.x
                */
               if( 1 == fParam->bTotHistoEnable )
                  ROC[uRoc].fTot[uGet4][uGet4Chan] =
                        new TH1I( Form("Roc%u_Get%u_Ch%u_Tot",
                              uRoc, uGet4, uGet4Chan),
                        Form("Tot for channel %u in chip %u on ROC %u;Tot [ps];Counts [1]", uGet4Chan, uGet4, uRoc),
                        1200, -25, 59975 );
               if( 1 == fParam->bDebugHistoEnable )
               {
                  // Change Name to make it compatible with calibration step
                  ROC[uRoc].fLeadingFTS[uGet4][uGet4Chan] =
//                        new TH1I( Form("Roc%u_Get%d_Ch%d_FTS_Leading",
//                                    uRoc, uGet4, uGet4Chan),
                        new TH1I( Form("tof_%s_ft_b%03u_ch%03u",
                                    toftdc::ksTdcHistName[ toftdc::get4 ].Data(),
                                    uGet4, 2*uGet4Chan + 1),
                              Form("FTS of Leading edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, 0., get4tdc::kiFineTime);
                  // Change Name to make it compatible with calibration step
                  ROC[uRoc].fTrailingFTS[uGet4][uGet4Chan] =
//                        new TH1I( Form("Roc%u_Get%d_Ch%d_FTS_Trailing",
//                                    uRoc, uGet4, uGet4Chan),
                        new TH1I( Form("tof_%s_ft_b%03u_ch%03u",
                                    toftdc::ksTdcHistName[ toftdc::get4 ].Data(),
                                    uGet4, 2*uGet4Chan),
                              Form("FTS of Trailing edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, 0., get4tdc::kiFineTime);
                  ROC[uRoc].fLeadingDnl[uGet4][uGet4Chan] =
                        new TH1D( Form("Roc%u_Get%u_Ch%u_Dnl_Leading",
                                    uRoc, uGet4, uGet4Chan),
                              Form("DNL of Leading edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);
                  ROC[uRoc].fLeadingDnlSum[uGet4][uGet4Chan] =
                        new TH1D( Form("Roc%u_Get%u_Ch%u_DnlSum_Leading",
                                    uRoc, uGet4, uGet4Chan),
                              Form("DNL Integral of Leading edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);
                  ROC[uRoc].fTrailingDnl[uGet4][uGet4Chan] =
                        new TH1D( Form("Roc%u_Get%u_Ch%u_Dnl_Trailing",
                                    uRoc, uGet4, uGet4Chan),
                              Form("DNL of Trailing edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);
                  ROC[uRoc].fTrailingDnlSum[uGet4][uGet4Chan] =
                        new TH1D( Form("Roc%u_Get%u_Ch%u_DnlSum_Trailing",
                                    uRoc, uGet4, uGet4Chan),
                              Form("DNL Integral of Trailing edge of channel %u in chip %u on ROC %u",
                                    uGet4Chan, uGet4, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);
               } // if( 1 == fParam->bDebugHistoEnable )

               // Event statistics
               if( kTRUE == fParam->bChannelRateHistoEnable )
                  ROC[uRoc].fChannelRateEvolution[uGet4][uGet4Chan] = new TH1I(
                        Form("Roc%u_Get%u_Ch%u_RateEvolution",
                              uRoc, uGet4, uGet4Chan),
                        Form("Time distribution of hits of channel %u in chip %u on ROC %u; Time [s]; (Nb hits)/(bin size in s) []",
                              uGet4Chan, uGet4, uRoc),
                        36000, 0., 7200.);

            } // for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)

            // Event statistics
            if( kTRUE == fParam->bChipRateHistoEnable )
            {
               ROC[uRoc].fChipRateEvolution[uGet4] = new TH1I(
                     Form("Roc%u_Get%u_RateEvolution",
                           uRoc, uGet4 ),
                     Form("Time distribution of hits for all channels of chip %u on ROC %u; Time [s]; (Nb hits)/(bin size in s) []",
                           uGet4, uRoc),
                     36000, 0., 7200.);
               ROC[uRoc].fdRateEvolutionBinSize = ROC[uRoc].fChipRateEvolution[uGet4]->GetBinWidth( 1 );
            } // if( kTRUE == fParam->bChipRateHistoEnable )
            ROC[uRoc].fuNbHitsChipEpoch[uGet4] = 0;

            // Epochs
            ROC[uRoc].fEpochShiftsDuration[uGet4] = new TH2I( Form("Roc%u_Get%u_EpochShiftsDuration",
                        uRoc, uGet4 ),
                  Form("Time distribution of hits for all channels of chip %u on ROC %u; Shift [GET4 epochs]; Duration [GET4 epochs]",
                        uGet4, uRoc),
                  52, -26., 26.,
                  5000, 0., 5000.);
         } // if remapped chip is valid and active
      } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      ROC[uRoc].fDistribEpochs = new TH1I( Form("Roc%u_DistribEpochs", uRoc),
            "Distribution of epoch counter value for 250MHz epoch messages; Value of Local Epoch Cnt []; Counts []",
            get4v10::kulMainEpochCycleSz/100000, 0, get4v10::kulMainEpochCycleSz );
      ROC[uRoc].fDistribEpochs2 = new TH2I( Form("Roc%u_DistribEpochs2", uRoc),
            "Distribution of epoch counter value for Roc epoch messages; chip []; Value of Roc Epoch Cnt []; Counts []",
            get4v10::kuMaxGet4Roc, -0.5, get4v10::kuMaxGet4Roc -0.5,
            get4v10::kulGet4EpochCycleSz/100000, 0, get4v10::kulGet4EpochCycleSz );
      ROC[uRoc].fEpochShiftsPerChip = new TH2I( Form("Roc%u_EpochShiftsPerChip", uRoc),
            "Value of shift in Epochs when non consecutive epoch index are detected, per chip; chip []; Epoch indec Shift in GET4 epochs []; Counts []",
            get4v10::kuMaxGet4Roc, -0.5, get4v10::kuMaxGet4Roc -0.5,
            512, -256, 256 );
      ROC[uRoc].fEpochShiftsDurationPerChip = new TH2I( Form("Roc%u_EpochShiftsDurationPerChip", uRoc ),
            Form("Duration of epoch shifts for all chips on ROC %d; chip []; Duration [GET4 epochs]", uRoc),
            get4v10::kuMaxGet4Roc, -0.5, get4v10::kuMaxGet4Roc -0.5,
            5000, 0., 5000.);

      ROC[uRoc].fNbEventsPerMbsEvent = new TH1I( Form("Roc%u_EventsCount", uRoc),
            "Nb of detected events per Mbs event; # Events []; Counts []",
            50, 0, 50  );
      ROC[uRoc].fNbNotEmptyEventsPerMbsEvent = new TH1I( Form("Roc%u_NotEmptyEvtCount", uRoc),
            "Nb of detected events per Mbs event; # Events []; Counts []",
            50, 0, 50 );
      ROC[uRoc].fChannelsMapping = new TH2I( Form("Roc%u_ChanMap", uRoc),
            "Events with both channels present; 1st channel []; 2nd channel []; Counts []",
            get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc ,
            get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc );
      ROC[uRoc].fChannelMultiplicity = new TH2I( Form("Roc%u_ChanMul", uRoc),
            "Channel multiplicity per event; Channel []; # Hits []; events []",
            get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc ,
            20, 0, 20 );

      if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
      {
         ROC[uRoc].fDataSelfTriggerPerEvent = new TH2I( Form("Roc%u_DataSelfTrigg", uRoc),
               "Channel multiplicity per event; # Triggers/event []; Trigger index[]; events []",
               50, 0, 50 ,
               fParam->uNbTriggers, 0, fParam->uNbTriggers );

         ROC[uRoc].fDataSelfTrigDistanceNs  = new TH2I( Form("Roc%u_DataTrigDistanceNs", uRoc),
               "Time difference since last trigger from data ROC; [ns]; Trigger index[]; events []",
               1002, -2., 1000.,
               fParam->uNbTriggers, 0, fParam->uNbTriggers );
         ROC[uRoc].fDataSelfTrigDistanceUs   = new TH2I( Form("Roc%u_DataTrigDistanceUs", uRoc),
               "Time difference since last trigger from data ROC; [us]; Trigger index[]; events []",
               999, 1., 1000.,
               fParam->uNbTriggers, 0, fParam->uNbTriggers );
         ROC[uRoc].fDataSelfTrigDistanceMs   = new TH2I( Form("Roc%u_DataTrigDistanceMs", uRoc),
               "Time difference since last trigger from data ROC;  [ms]; Trigger index[]; events []",
               999, 1., 1000.,
               fParam->uNbTriggers, 0, fParam->uNbTriggers );
         ROC[uRoc].fDataSelfTrigDistanceS    = new TH2I( Form("sRoc%u_DataTrigDistanceS", uRoc),
               "Time difference since last trigger from data ROC; [s]; Trigger index[]; events []",
               999, 1., 1000.,
               fParam->uNbTriggers, 0, fParam->uNbTriggers);
      }

      // Error messages
      ROC[uRoc].fGet4ErrorChip = new TH1I( Form("Roc%u_Get4ErrorChip", uRoc),
                              "Get4 chip sending Error message; Error Chip; Entries [1]",
                              fParam->uNbGet4, -0.5, fParam->uNbGet4 -0.5);
      ROC[uRoc].fGet4ErrorChan = new TH2I( Form("Roc%u_Get4ErrorChan", uRoc),
                              "Get4 channel/edge sending Error message; Error chan;;Entries [1]",
                              2*(fParam->uNbGet4)*get4v10::kuNbChan, 0., (fParam->uNbGet4)*get4v10::kuNbChan,
                              6, 0, 6 );

      ROC[uRoc].fGet4ErrorChan->GetYaxis()->SetBinLabel(1, "0x04: FIFO Write      ");
      ROC[uRoc].fGet4ErrorChan->GetYaxis()->SetBinLabel(2, "0x05: Lost event      ");
      ROC[uRoc].fGet4ErrorChan->GetYaxis()->SetBinLabel(3, "0x06: Channel state   ");
      ROC[uRoc].fGet4ErrorChan->GetYaxis()->SetBinLabel(4, "0x11: Overwrite       ");
      ROC[uRoc].fGet4ErrorChan->GetYaxis()->SetBinLabel(5, "0x12: ToT out of range");

      ROC[uRoc].fGet4ErrorPatt = new TH2I( Form("Roc%u_Get4ErrorPatt", uRoc),
                              "Pattern of Get4 Error message;Error chip;;Entries [1]",
                              fParam->uNbGet4, -0.5, fParam->uNbGet4 -0.5, 128, 0., 128);
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(1,  "0x00: Readout Init    ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(2,  "0x01: Sync            ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(3,  "0x02: Epoch count sync");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(4,  "0x03: Epoch           ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(5,  "0x04: FIFO Write      ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(6,  "0x05: Lost event      ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(7,  "0x06: Channel state   ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(8,  "0x07: Token Ring state");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(9,  "0x08: Token           ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invok."); // <- From GET4 v1.2
//      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(14, "0x0d:                 ");
//      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(15, "0x0e:                 ");
//      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(16, "0x0f:                 ");
//      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(17, "0x10:                 ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(18, "0x11: Overwrite       ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(19, "0x12: ToT out of range");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(20, "0x13: Event Discarded ");
      ROC[uRoc].fGet4ErrorPatt->GetYaxis()->SetBinLabel(128, "0x7f: Unknown         ");

      /*
       * Debug Histograms for GET4 v1.x
       */
      if( 1 == fParam->bDebugHistoEnable )
      {
         UInt_t uHistosIndexSingleChip = 0;
         UInt_t uHistosIndexChipVsChip = 0;
         for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < get4v10::kuNbChan; uFirstGet4Channel++)
         {
            for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < get4v10::kuNbChan; uSecondGet4Channel++)
            {
               if( uFirstGet4Channel < uSecondGet4Channel )
               {
                  ROC[uRoc].fTimeDiffInsideChip[0][uHistosIndexSingleChip] =
                        new TH1I( Form("Roc%d_TimeDiff1_%d_%d",
                                    uRoc, uFirstGet4Channel, uSecondGet4Channel),
                              Form("Time difference between channel %d and %d in chip %d ROC %d; dT [ps]; Counts [1]",
                                    uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip1, uRoc),
                              601, -15025, 15025 );
                  ROC[uRoc].fTimeDiffInsideChip[1][uHistosIndexSingleChip] =
                        new TH1I( Form("Roc%d_TimeDiff2_%d_%d",
                              uRoc, uFirstGet4Channel, uSecondGet4Channel),
                        Form("Time difference between channel %d and %d in chip %d ROC %d; dT [ps]; Counts [1]",
                              uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip2, uRoc),
                        601, -15025, 15025 );

                  // 1D FineTime correlation
                  ROC[uRoc].fFTCorrel[0][uHistosIndexSingleChip] =
                        new TH1I( Form("Roc%d_FTCorrel1_%d_%d",
                                    uRoc, uFirstGet4Channel, uSecondGet4Channel),
                              Form("FTS Correlation between channels %d and %d in chip %d ROC %d",
                                    uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip1, uRoc),
                              2*get4tdc::kiFineTime + 1, -get4tdc::kiFineTime-0.5, get4tdc::kiFineTime+0.5);
                  ROC[uRoc].fFTCorrel[1][uHistosIndexSingleChip] =
                        new TH1I( Form("Roc%d_FTCorrel2_%d_%d",
                                    uRoc, uFirstGet4Channel, uSecondGet4Channel),
                              Form("FTS Correlation between channels %d and %d in chip %d ROC %d",
                                    uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip2, uRoc),
                              2*get4tdc::kiFineTime + 1, -get4tdc::kiFineTime-0.5, get4tdc::kiFineTime+0.5);

                  // 2D FineTime correlation
                  ROC[uRoc].fFTCorrel2D[0][uHistosIndexSingleChip] =
                        new TH2I( Form("Roc%d_2dFTCorr1_%d_%d",
                                    uRoc, uFirstGet4Channel, uSecondGet4Channel),
                              Form("FTS Correlation between channels %d and %d in chip %d ROC %d",
                                    uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip1, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5,
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);
                  ROC[uRoc].fFTCorrel2D[1][uHistosIndexSingleChip] =
                        new TH2I( Form("Roc%d_2dFTCorr2_%d_%d",
                                    uRoc, uFirstGet4Channel, uSecondGet4Channel),
                              Form("FTS Correlation between channels %d and %d in chip %d ROC %d",
                                    uFirstGet4Channel, uSecondGet4Channel, fParam->uGet4TimeDiffChip2, uRoc),
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5,
                              get4tdc::kiFineTime, -0.5, get4tdc::kiFineTime-0.5);

                  uHistosIndexSingleChip++;
               } // if( uFirstGet4Channel < uSecondGet4Channel )
               ROC[uRoc].fTimeDiffBetweenChips[uHistosIndexChipVsChip] =
                     new TH1I( Form("Roc%d_dT_%d_%d_Vs_%d_%d",
                                 uRoc,
                                 fParam->uGet4TimeDiffChip1, uFirstGet4Channel,
                                 fParam->uGet4TimeDiffChip2, uSecondGet4Channel),
                           Form("Time difference between channel %d in chip %d and channel %d in chip %d; dT [ps]; Counts [1]",
                                 uFirstGet4Channel, fParam->uGet4TimeDiffChip1, uSecondGet4Channel, fParam->uGet4TimeDiffChip2),
                           961, -24025, 24025 );
               ROC[uRoc].fFTCorrelChipToChip[uHistosIndexChipVsChip] =
                     new TH1I( Form("Roc%d_FTCorrel_%d_%d_Vs_%d_%d",
                                 uRoc,
                                 fParam->uGet4TimeDiffChip1, uFirstGet4Channel,
                                 fParam->uGet4TimeDiffChip2, uSecondGet4Channel),
                           Form("FTS Correlation between channel %d in chip %d and channel %d in chip %d",
                                 uFirstGet4Channel, fParam->uGet4TimeDiffChip1, uSecondGet4Channel, fParam->uGet4TimeDiffChip2),
                           2*get4tdc::kiFineTime + 1, -get4tdc::kiFineTime-0.5, get4tdc::kiFineTime+0.5);
               uHistosIndexChipVsChip++;
            } // for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < MAX_GET4_CH; uSecondGet4Channel++)
         } // for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < MAX_GET4_CH; uFirstGet4Channel++)
      } // if( 1 == fParam->bDebugHistoEnable )
   } // for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
   
   gDirectory->cd( oldir->GetPath() ); // <= To prevent histos from being sucked in by the param file of the TRootManager!

   return kTRUE;
}
void TTofGet4Unpacker::FillHistos()
{
   // Most Histos are filled inside processing functions

   // Fill histos which were normally filled in Finish function
   for( UInt_t  uRocTempId =0; uRocTempId < fParam->numRocs; uRocTempId++ )
   {
      ROC[uRocTempId].fNbEventsPerMbsEvent->Fill(uEventInsideMbsEvtCount[uRocTempId]);
      ROC[uRocTempId].fNbNotEmptyEventsPerMbsEvent->Fill(uNotEmptyEventInsideMbsEvtCount[uRocTempId]);
      uEventInsideMbsEvtCount[uRocTempId] = 0;
      uNotEmptyEventInsideMbsEvtCount[uRocTempId] = 0;

      if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
         for( UInt_t uTrigger = 0; uTrigger < fParam->uNbTriggers; uTrigger++)
         {
            ROC[uRocTempId].fDataSelfTriggerPerEvent->Fill( ROC[uRocTempId].fuDataSelfTriggerCount[uTrigger],
                  uTrigger );
            ROC[uRocTempId].fuDataSelfTriggerCount[uTrigger] = 0;
         } // for( UInt_t uTrigger = 0; uTrigger < fParam->uNbTriggers; uTrigger++)
   } // for( UInt_t  uRocTempId =0; uRocTempId < fParam->numRocs; uRocTempId++ )
}
void TTofGet4Unpacker::WriteHistos( TDirectory* inDir)
{
   TDirectory * oldir = gDirectory;

   // ROCS and base Dir
   TDirectory *cdAllRocs;
   TDirectory *cdRoc[fParam->numRocs];
   TDirectory *cdRocLong[fParam->numRocs];

   // GET4
   TDirectory *cdRocGet4[fParam->numRocs][get4v10::kuMaxGet4Roc];
   TDirectory *cdRocGet4FtL[fParam->numRocs][get4v10::kuMaxGet4Roc];
   TDirectory *cdRocGet4FtT[fParam->numRocs][get4v10::kuMaxGet4Roc];
   TDirectory *cdRocGet4Rate[fParam->numRocs][get4v10::kuMaxGet4Roc];

   // DEBUG
   TDirectory *cdRocDebug[fParam->numRocs];
   TDirectory *cdRocDebugChip1[fParam->numRocs];
   TDirectory *cdRocDebugChip2[fParam->numRocs];
   TDirectory *cdRocDebugCvsC[fParam->numRocs];

   // All Sub-Events histo
   fEvtPerProc->Write();

   // All ROCs histos
   cdAllRocs = inDir->mkdir( "ROCs" );
   cdAllRocs->cd();    // make the "ROCs" directory the current directory
   
   fEvntSize->Write();
   fMsgsPerRoc->Write();
   fTriggerPerRoc->Write();
   fDeltaTriggerTime->Write();

   // ROC Specific histos
   for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
   {
      cdRoc[uRoc] = inDir->mkdir( Form("ROC%02u",uRoc) );
      cdRoc[uRoc]->cd();    // make the "ROCXX" directory the current directory

      ROC[uRoc].fMsgTypes->Write();
      ROC[uRoc].fSysTypes->Write();
      ROC[uRoc].fSysUserTypes->Write();
      ROC[uRoc].fAUXch->Write();

      ROC[uRoc].fTrigger_All->Write();
      ROC[uRoc].fTrigger_All_100->Write();
      ROC[uRoc].fTrigger_All_us->Write();
      ROC[uRoc].fTrigger_All_ms->Write();
      ROC[uRoc].fTrigger_All_s->Write();

      ROC[uRoc].fTrigger_AUX->Write();

      ROC[uRoc].fEvntMultipl->Write();

      ROC[uRoc].fALLt->Write();
      ROC[uRoc].fDATAt->Write();
      ROC[uRoc].fERRORt->Write();
      ROC[uRoc].fSLOWCt->Write();
      ROC[uRoc].fSelfTriggT->Write();
      ROC[uRoc].fEPOCHt->Write();
      ROC[uRoc].fAllSelectedT->Write();

      for (unsigned nsync=0; nsync<get4v10::kuMaxSync; nsync++)
         ROC[uRoc].fSYNCt[nsync]->Write();

      for (unsigned naux=0; naux<get4v10::kuMaxAux; naux++)
         ROC[uRoc].fAUXt[naux]->Write();

      // Long duration time distribution (1 min. bin, 10 days length)
      if( kTRUE == fParam->bLongTimeHistos )
      {
         cdRocLong[uRoc] = cdRoc[uRoc]->mkdir( "Long" );
         cdRocLong[uRoc]->cd();    // make the "ROCXX/Long" directory the current directory

         ROC[uRoc].fAllLongT->Write();
         ROC[uRoc].fDataLongT->Write();
         ROC[uRoc].fErrorLongT->Write();
         ROC[uRoc].fSlowCLongT->Write();
         ROC[uRoc].fSelfTriggLongT->Write();
         ROC[uRoc].fEpochLongT->Write();
         ROC[uRoc].fAllEpoch2LongT->Write();
         ROC[uRoc].fAllSelLongT->Write();

         for (unsigned nsync=0; nsync<get4v10::kuMaxSync; nsync++)
            ROC[uRoc].fSyncLongT[nsync]->Write();
      } // if( kTRUE == fParam->bLongTimeHistos )

      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      {
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRoc, uGet4);

         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
         {
            cdRocGet4[uRoc][uGet4] = cdRoc[uRoc]->mkdir( Form("Get4_%u", uGet4) );
            cdRocGet4[uRoc][uGet4]->cd();    // make the "ROCXX/Get4_YY" directory the current directory
            if( 1 == fParam->bDebugHistoEnable )
            {
               cdRocGet4FtL[uRoc][uGet4] = cdRocGet4[uRoc][uGet4]->mkdir( "FTS_L" );
               cdRocGet4FtT[uRoc][uGet4] = cdRocGet4[uRoc][uGet4]->mkdir( "FTS_T" );
            } // if( 1 == fParam->bDebugHistoEnable )
            if( kTRUE == fParam->bChannelRateHistoEnable )
               cdRocGet4Rate[uRoc][uGet4] = cdRocGet4[uRoc][uGet4]->mkdir( "RateCh" );

            ROC[uRoc].fEPOCH2t[uGet4]->Write();
            for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
            {
               ROC[uRoc].fSelectedT[uGet4][uGet4Chan]->Write();

               ROC[uRoc].fTrigger_Get4Channel[uGet4][uGet4Chan]->Write();
               ROC[uRoc].fTriggerMs_Get4Channel[uGet4][uGet4Chan]->Write();
               ROC[uRoc].fTriggerS_Get4Channel[uGet4][uGet4Chan]->Write();

               /*
                * Debug Histograms for GET4 v1.x
                */
               if( 1 == fParam->bTotHistoEnable )
                  ROC[uRoc].fTot[uGet4][uGet4Chan]->Write();
               if( 1 == fParam->bDebugHistoEnable )
               {
                  cdRocGet4FtL[uRoc][uGet4]->cd();    // make the "ROCXX/Get4_YY/FTS_L" directory the current directory

                  ROC[uRoc].fLeadingFTS[uGet4][uGet4Chan]->Write();
                  ROC[uRoc].fLeadingDnl[uGet4][uGet4Chan]->Write();
                  ROC[uRoc].fLeadingDnlSum[uGet4][uGet4Chan]->Write();

                  cdRocGet4FtT[uRoc][uGet4]->cd();    // make the "ROCXX/Get4_YY/FTS_T" directory the current directory

                  ROC[uRoc].fTrailingFTS[uGet4][uGet4Chan]->Write();
                  ROC[uRoc].fTrailingDnl[uGet4][uGet4Chan]->Write();
                  ROC[uRoc].fTrailingDnlSum[uGet4][uGet4Chan]->Write();
               } // if( 1 == fParam->bDebugHistoEnable )

               // Event statistics
               if( kTRUE == fParam->bChannelRateHistoEnable )
               {
                  cdRocGet4Rate[uRoc][uGet4]->cd();    // make the "ROCXX/Get4_YY/RateCh" directory the current directory
                  ROC[uRoc].fChannelRateEvolution[uGet4][uGet4Chan]->Write();
               } // if( kTRUE == fParam->bChannelRateHistoEnable )

            } // for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)

            cdRocGet4[uRoc][uGet4]->cd();    // make the "ROCXX/Get4_YY" directory the current directory
            // Event statistics
            if( kTRUE == fParam->bChipRateHistoEnable )
               ROC[uRoc].fChipRateEvolution[uGet4]->Write();

            // Epochs
            ROC[uRoc].fEpochShiftsDuration[uGet4]->Write();
         } // if remapped chip is valid and active
      } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)

      cdRoc[uRoc]->cd();    // make the "ROCXX" directory the current directory
      ROC[uRoc].fDistribEpochs->Write();
      ROC[uRoc].fDistribEpochs2->Write();
      ROC[uRoc].fEpochShiftsPerChip->Write();
      ROC[uRoc].fEpochShiftsDurationPerChip->Write();

      ROC[uRoc].fNbEventsPerMbsEvent->Write();
      ROC[uRoc].fNbNotEmptyEventsPerMbsEvent->Write();
      ROC[uRoc].fChannelsMapping->Write();
      ROC[uRoc].fChannelMultiplicity->Write();

      if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
      {
         ROC[uRoc].fDataSelfTriggerPerEvent->Write();

         ROC[uRoc].fDataSelfTrigDistanceNs->Write();
         ROC[uRoc].fDataSelfTrigDistanceUs->Write();
         ROC[uRoc].fDataSelfTrigDistanceMs->Write();
         ROC[uRoc].fDataSelfTrigDistanceS->Write();
      }

      // Error messages
      ROC[uRoc].fGet4ErrorChip->Write();
      ROC[uRoc].fGet4ErrorChan->Write();

      ROC[uRoc].fGet4ErrorPatt->Write();

      /*
       * Debug Histograms for GET4 v1.x
       */
      if( 1 == fParam->bDebugHistoEnable )
      {
         cdRocDebug[uRoc] = cdRoc[uRoc]->mkdir( "Debug" );
         cdRocDebug[uRoc]->cd();    // make the "ROCXX/Debug" directory the current directory
         cdRocDebugChip1[uRoc] = cdRocDebug[uRoc]->mkdir( "Chip1" );
         cdRocDebugChip2[uRoc] = cdRocDebug[uRoc]->mkdir( "Chip2" );
         cdRocDebugCvsC[uRoc] = cdRocDebug[uRoc]->mkdir( "ChipVsChip" );

         UInt_t uHistosIndexSingleChip = 0;
         UInt_t uHistosIndexChipVsChip = 0;
         for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < get4v10::kuNbChan; uFirstGet4Channel++)
         {
            for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < get4v10::kuNbChan; uSecondGet4Channel++)
            {
               if( uFirstGet4Channel < uSecondGet4Channel )
               {
                  cdRocDebugChip1[uRoc]->cd();    // make the "ROCXX/Debug/Chip1" directory the current directory
                  ROC[uRoc].fTimeDiffInsideChip[0][uHistosIndexSingleChip]->Write();
                  // 1D FineTime correlation
                  ROC[uRoc].fFTCorrel[0][uHistosIndexSingleChip]->Write();
                  // 2D FineTime correlation
                  ROC[uRoc].fFTCorrel2D[0][uHistosIndexSingleChip]->Write();

                  cdRocDebugChip2[uRoc]->cd();    // make the "ROCXX/Debug/Chip2" directory the current directory
                  ROC[uRoc].fTimeDiffInsideChip[1][uHistosIndexSingleChip]->Write();
                  // 1D FineTime correlation
                  ROC[uRoc].fFTCorrel[1][uHistosIndexSingleChip]->Write();
                  // 2D FineTime correlation
                  ROC[uRoc].fFTCorrel2D[1][uHistosIndexSingleChip]->Write();

                  uHistosIndexSingleChip++;
               } // if( uFirstGet4Channel < uSecondGet4Channel )

               cdRocDebugCvsC[uRoc]->cd();    // make the "ROCXX/Debug/ChipVsChip" directory the current directory
               ROC[uRoc].fTimeDiffBetweenChips[uHistosIndexChipVsChip]->Write();
               ROC[uRoc].fFTCorrelChipToChip[uHistosIndexChipVsChip]->Write();

               uHistosIndexChipVsChip++;
            } // for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < get4v10::kuNbChan; uSecondGet4Channel++)
         } // for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < get4v10::kuNbChan; uFirstGet4Channel++)
      } // if( 1 == fParam->bDebugHistoEnable )
   } // for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
   
   oldir->cd();
}
void TTofGet4Unpacker::DeleteHistos()
{
   /*
   fh1VftxRawChMap.clear();
   for( Int_t iBoardIndex = 0; iBoardIndex < fuNbTdc; iBoardIndex++)
      (fh1VftxChFt[iBoardIndex]).clear();
   fh1VftxChFt.clear();
   */
}

/*********************************************************************************/
/****************** Internal Functions *******************************************/

void TTofGet4Unpacker::ProcessTriggerMessage(UInt_t uRocId, TGet4v1MessageExtended& extMess,
                                            Int_t iTriggerIndex)
{
   /* TODO FILL ME
    * Trigger processing
    */

   /*
    * Check if current event already got a trigger
    */
   if( kTRUE == fCurrentGet4Event.HasTrigger() )
   {
      Double_t dLastTriggerDistance =
              extMess.GetFullTimeD()
            - fCurrentGet4Event.fdTriggerFullTime[uRocId] ;

      /*
       * If incoming trigger has lower priority than current trigger or if
       * current trigger was a SYNC trigger and a data trigger comes
       * And it is inside the deatime window
       * => Ignore incoming trigger
       */
      if( dLastTriggerDistance <= fParam->dDeadTime  &&
          iTriggerIndex < (Int_t)(fCurrentGet4Event.fuTriggerIndex)  )
         return;
      /*
       * If incoming trigger has higher priority than current trigger or if
       * current trigger was a SYNC trigger and a data trigger comes
       * And it is inside the deatime window
       * => erase current event, start new event using incoming trigger
       */
      else if( dLastTriggerDistance <= fParam->dDeadTime  &&
                iTriggerIndex > (Int_t)(fCurrentGet4Event.fuTriggerIndex) )
      {
         fCurrentGet4Event.Clear();

      } // else if( iTriggerIndex > (Int_t)(fCurrentGet4Event.fuTriggerIndex) )
      /*
       * If incoming trigger has same priority than current trigger or if
       * current trigger was a SYNC trigger and a SYNC trigger comes
       * => depend on bAllowEventMixing flag and distance
       * kTRUE  = save the partially filled old one, start the new one => TODO MIXING!!!
       * kFALSE = ignore incoming trigger
       * Distance between them bigger than trigger window size => save the partially filled old one, start the new one
       */
      else
      {
         if( fParam->dDeadTime < dLastTriggerDistance || kTRUE == fParam->bAllowEventMixing)
         {
            /*
             * TODO real event mixing or parallel buffers
             */

            // Check flag indicating the extra epoch after trigger is passed
            for( UInt_t  uRocTempId =0; uRocTempId<fParam->numRocs; uRocTempId++ )
            {
               // do not check for non-used ROC
               if (!fParam->IsActiveRoc(uRocTempId)) continue;

               for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
               {
                  UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRocTempId, uGet4);
                  if( kFALSE == fParam->IsValidGet4Chip(uRemappedGet4Index) ||
                      kFALSE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
                      continue;

                  // Finish previous event
//                  BuildHitsAfterTrigg( uRocTempId,  uRemappedGet4Index);

                  // If chip not finished to process this event, build hits from current
                  // buffer (safe as we anyway build hits on epoch end when necessary)
                  if( kFALSE == ROC[uRocTempId].fbSelectionDone[uRemappedGet4Index]  )
                  {
                     // If This is the first epoch after the trigger and
                     // we did not process the epoch before the one where the
                     // trigger came, we need to initialize the 24 bit temp hits
                     if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uRemappedGet4Index] &&
                         kFALSE == ROC[uRocId].bLookInPreviousEpoch[uRemappedGet4Index] )
                        for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                              (ROC[uRocId].fHitTemp24[uRemappedGet4Index][uChan]).Clear();

                     BuildHits(uRocTempId, uRemappedGet4Index, ROC[uRocTempId].fbBufferWithLastFullEpoch2[uRemappedGet4Index]);
                     ROC[uRocTempId].fbSelectionDone[uRemappedGet4Index] = kTRUE;
                  }
               } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
            } // for( UInt_t  uRocId =0; uRocId<fParam->numRocs; uRocId++ )

            // Save previous event if not empty
            if( kFALSE == fCurrentGet4Event.IsEmpty() )
            {
               AnalyzeAllGet4Channels( fCurrentGet4Event );

               // Replaced with cbmroot output format
               // Only 1 event used per MBS event!!
//               (fOutputEvent->fEvents).push_back( fCurrentGet4Event );
               if( kFALSE == fbDataSavedForThisEvent )
               {
                  // First event found in this MBS event
                  for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
                  {
                     TTofGet4Board * fGet4Board = (TTofGet4Board*) fGet4BoardCollection->ConstructedAt(uGet4);
                     if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
                     {
                        fGet4Board->SetTriggerFullTime( fCurrentGet4Event.fdTriggerFullTime[0] ); // Use only first ROC!!!!
                        fGet4Board->SetSyncEventFlag( fCurrentGet4Event.fbMbsSyncedEvent );
                        fGet4Board->SetSyncEvtNb( fCurrentGet4Event.fuSyncEventNumber );
                        fGet4Board->SetDataEventFlag( fCurrentGet4Event.fbDataTriggered );
                        fGet4Board->SetTriggIndx( fCurrentGet4Event.fuTriggerIndex );

                        // Some Data found for this TDC Chip
                        for( UInt_t uCh = 0; uCh < get4v10::kuNbChan; uCh++ )
                           for( UInt_t uHit = 0;
                                uHit < ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh]).size();
                                uHit++ )
                           {
                              TGet4v1MessageExtended extTime = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTimeMessage();
                              TGet4v1MessageExtended extTot  = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTotMessage();

                              if( kFALSE == extTime.IsEmpty() )
                              {
                                 TTofGet4Data dataTime;
                                 dataTime.Set32bFlag( extTime.Is32Bit() );

                                 dataTime.SetEdge( 0 );
                                 if( kTRUE == extTime.Is32Bit() )
                                 {
                                    dataTime.SetChannel( uCh );
                                    dataTime.SetFineTime(   extTime.getGet4V10R32HitFt() );
                                    dataTime.SetCoarseTime( extTime.getGet4V10R32HitTs() );
                                    dataTime.SetTot(        extTime.getGet4V10R32HitTot() );
                                 } // if( kTRUE == extTime.Is32Bit() )
                                    else
                                    {
                                       // Time saved in "odd channels" as example of VFTX
                                       dataTime.SetChannel( 2*uCh + 1 );
                                       dataTime.SetFineTime(    extTime.GetGet4Ts() & get4tdc::kiFineTime );
                                       dataTime.SetCoarseTime( (extTime.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                                    } // else of if( kTRUE == extTime.Is32Bit() )
                                 dataTime.SetEpoch( extTime.GetRocEpoch() );
                                 dataTime.SetEpochCycle( extTime.GetRocCycle() );
                                 dataTime.SetFullTimeD( extTime.GetFullTimeD() );
                                 fGet4Board->AddData( dataTime );
                              } // if( kFALSE == extTime.IsEmpty() )

                              if( kFALSE == extTot.IsEmpty() )
                              {
                                 // Will appear only in case of 24bit mode !!!
                                 TTofGet4Data dataTot;
                                 // Tot saved in "even channels" as example of VFTX
                                 dataTot.SetChannel( 2*uCh );
                                 dataTot.SetEdge( 1 );
                                 dataTot.SetFineTime(    extTot.GetGet4Ts() & get4tdc::kiFineTime );
                                 dataTot.SetCoarseTime( (extTot.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                                 dataTot.SetEpoch( extTot.GetRocEpoch() );
                                 dataTot.SetEpochCycle( extTot.GetRocCycle() );
                                 dataTot.SetFullTimeD( extTot.GetFullTimeD() );
                                 dataTot.Set32bFlag( extTot.Is32Bit() );
                                 fGet4Board->AddData( dataTot );
                              } // if( kFALSE == extTot.IsEmpty() )
                           } // scan over all hits in this board object

                     } // if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
                  } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
                  fbDataSavedForThisEvent = kTRUE;
               } // if( kFALSE == fbDataSavedForThisEvent )
               ///////////////////////////////////////////////

               for( UInt_t  uRocTempId =0; uRocTempId<fParam->numRocs; uRocTempId++ )
               {
                  // do not check for non-used ROC
                  if (!fParam->IsActiveRoc(uRocTempId)) continue;

                  uNotEmptyEventInsideMbsEvtCount[uRocTempId]++;
                  uNotEmptyRealEventsCount[uRocTempId]++;
               } // for( UInt_t  uRocTempId =0; uRocTempId<fParam->numRocs; uRocTempId++ )
            } // if( kFALSE == fCurrentGet4Event.IsEmpty() )
         } // if( kTRUE == fParam->bAllowEventMixing)
            else return;
      } // else of else if( trigger > currentTrig) of if( trigger < currentTrig)
   } // if( kTRUE == fCurrentGet4Event.HasTrigger() )

   // anyway clear again for safety
   fCurrentGet4Event.Clear();

   // Assign new trigger time to current event holder
   fCurrentGet4Event.fdTriggerFullTime[uRocId] = extMess.GetFullTimeD();
   if( fCurrentGet4Event.fdTriggerFullTime[uRocId] <= 0.0)
   {
      TString sTemp = Form("Assigning 0 time as trigger time? event %d time %f", uNbEvents[uRocId], extMess.GetFullTimeD());
      LOG(error)<<sTemp;
   } // if( fCurrentGet4Event.fdTriggerFullTime[uRocId] <= 0.0)

   // Event number of the corresponding MBS event
//   fCurrentGet4Event.fuMbsEventNumber      = GetEventNumber();
   // Index of the real event inside MBS event
   fCurrentGet4Event.fuEventNbInsideMbsEvt = uEventInsideMbsEvtCount[uRocId];
   // Total event number
   fCurrentGet4Event.fuGlobalEventNumber   = uRealEventsCount[uRocId];
   if( roc::MSG_SYNC == extMess.GetMessageType() )
   {
      // Flag indicating that this real event is the one synchronized to MBS
      fCurrentGet4Event.fbMbsSyncedEvent = kTRUE;
      // Event Nmber from the sync message
      fCurrentGet4Event.fuSyncEventNumber = (extMess.GetRocMessage()).getSyncData();
   } // if( roc::MSG_SYNC == extMess.GetMessageType() )
      else if( -1 < iTriggerIndex )
      {
         // Flag indicating that this real event was made after finding coincidence in data
         fCurrentGet4Event.fbDataTriggered = kTRUE;
         // Index of the Data trigger making this event
         fCurrentGet4Event.fuTriggerIndex = (UInt_t)iTriggerIndex;
      } // else if( -1 < iTriggerIndex ) of if( roc::MSG_SYNC == extMess.GetMessageType() )
   uEventInsideMbsEvtCount[uRocId]++;
   uRealEventsCount[uRocId]++;

   /*
    * TODO: ROC self (NX/SYNC/AUX) messages bulding/checking
    */
   ROC[uRocId].fbSelectionRocDone = kFALSE;
   ROC[uRocId].fbEpochSinceTrigger = kFALSE;

   // Good solution is following:
   // When trigger: Process PrevBuffer Epoch -1 and remove older data if no event mixing
   // When current epoch finishes or when new trigger out of deadtime => process current epoch and remove older data if no event mixing
   // When future epoch finishes or when new trigger out of deadtime => process future epoch (which is stored in current buffer)
   for( UInt_t uGet4Index = 0; uGet4Index < fParam->uNbGet4; uGet4Index++)
      if( fParam->IsActiveGet4Chip( uGet4Index ) )
      {
         // Check if necessary to build hits from buffer of previous epoch
         // Is the case if the trigger window begins before beginning of current epoch
         Double_t dFullTimeCurrentEpoch = (((ULong64_t)ROC[uRocId].GetFullEpoch2Number(uGet4Index)) << 19);
         dFullTimeCurrentEpoch = dFullTimeCurrentEpoch / 20. + 512.;

         if( extMess.GetFullTimeD() + fParam->dCoincidenceWindowStart[iTriggerIndex] < dFullTimeCurrentEpoch)
         {
            ROC[uRocId].bLookInPreviousEpoch[uGet4Index] = kTRUE;
            for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                  (ROC[uRocId].fHitTemp24[uGet4Index][uChan]).Clear();
            Bool_t bOlderBuffer = ( kTRUE == ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Index] ? 0 : 1 );
            BuildHits(uRocId, uGet4Index, bOlderBuffer);
         } // if( extMess.GetFullTimeD() + fParam->dCoincidenceWindowStart[iTriggerIndex] < dFullTimeCurrentEpoch)
            else ROC[uRocId].bLookInPreviousEpoch[uGet4Index] = kFALSE;

         // Check if necessary to build hits from buffer of epoch after current one
         // Is the case if the trigger window ends after the end of current epoch
//         Double_t dFullTimeNextEpoch    = (Double_t)get4tdc::kdEpochInPs*(Double_t)( 1.0 + ROC[uRocId].GetFullEpoch2Number(uGet4Index) )/ 1000.0;
         Double_t dFullTimeNextEpoch = 1.0 + (((ULong64_t)ROC[uRocId].GetFullEpoch2Number(uGet4Index)) << 19);
         dFullTimeNextEpoch = dFullTimeCurrentEpoch / 20. + 512.;
         if( dFullTimeNextEpoch < extMess.GetFullTimeD() + fParam->dCoincidenceWindowStop[iTriggerIndex] )
            ROC[uRocId].bLookInNextEpoch[uGet4Index] = kTRUE;
            else ROC[uRocId].bLookInNextEpoch[uGet4Index] = kFALSE;

         // Initialize the flag indicating how many epoch2 finished since this trigger came
         ROC[uRocId].fbEpoch2SinceTrigger[uGet4Index] = kFALSE;
         ROC[uRocId].fbSelectionDone[uGet4Index]   = kFALSE;

         for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
         {
            ROC[uRocId].fbDataTimeOut[uGet4Index][uChanTest] = kFALSE;
            ROC[uRocId].fbDataFallingOut[uGet4Index][uChanTest] = kFALSE;
         }
      }
   return;
}

void TTofGet4Unpacker::ProcessExtendedMessage(UInt_t uRocId, TGet4v1MessageExtended& extMess)
{
   /* TODO FILL ME
    * Generic Extended message processing
    * If Previous trigger done => just put in buffer
    * If not done => put in buffer, build hit if possible and
    * check if it does not match current trigger
    */
   UInt_t uType = extMess.GetMessageType();

   // 24 Bits data message => need to be built
   if( roc::MSG_GET4 == uType )
   {
      UInt_t uGet4Id =  extMess.GetGet4Number() ;

      // Check if chip is activated
      if( fParam->IsActiveGet4Chip(uGet4Id) )
      {
         // Save message anyway in previous epochs buffer in case two triggers come close
         ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][
             ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]].push_back(extMess);
      } // if( fParam->IsActiveGet4Chip(uChip) )
   }
   // 32 Bits data message => already contains a full hit!
   else if( roc::MSG_SYS == uType )
   {
      UShort_t ucSysMesType = extMess.GetSysMesType(); // change type to avoid warning if last Chip id reach char type size
      if( get4v10::SYSMSG_GET4V1_32BIT_0 <= ucSysMesType   &&
          ucSysMesType <= get4v10::SYSMSG_GET4V1_32BIT_15 )
      {
         UInt_t uGet4Id =  extMess.getGet4V10R32ChipId() ;
         uGet4Id = fParam->RemapGet4Chip( uRocId, uGet4Id);

         // Check if chip is activated
         if( fParam->IsActiveGet4Chip(uGet4Id) )
         {
            // Save message anyway in previous epochs buffer in case two triggers come close
            ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][
                ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]].push_back(extMess);
         } // if( fParam->IsActiveGet4Chip(uChip) )
      }
      else
      {
         cout<<"Rejected message because type not recognised in ProcessExtendedMessage"<<endl;
      }// else of else if( Get4 32 bits) of if( Get4 24 bits )
   } // else if( Get4 32 bits) of if( Get4 24 bits )
   // Other ROC data messages => Saved in their own vector for each event if needed
   else
   {
      cout<<"Rejected message because type not recognised in ProcessExtendedMessage"<<endl;
   }// else of else if( Get4 32 bits) of if( Get4 24 bits )
}
/**********************************************************************/
Bool_t TTofGet4Unpacker::BuildHits(UInt_t uRocId, UInt_t uGet4Id, Bool_t bBufferToBuild)
{
   /*
    * 32 bit hit building from all raw message => easy => done
    * 24bit hit building from all raw edges messages
    * -> Check edges order? => ordering messages should solve
    * -> Check multiple rising edges case  => Histos?
    * -> Check multiple falling edges case => Histos?
    */
   UInt_t uLastAcceptedMessageIndex = 0;

   std::sort( ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild].begin(),
              ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild].end() );
   for( UInt_t uMessageIndex = 0;
         uMessageIndex < ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild].size();
         uMessageIndex ++)
   {
      if( kTRUE == (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex].Is32Bit() )
      {
         // 32 Bits data message => already contains a full hit!
         fHitTemp.Clear();
         fHitTemp.SetFullHit32Bits( (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex] );
         if( kTRUE == CheckHit( uRocId, uGet4Id, fHitTemp ) )
            uLastAcceptedMessageIndex = uMessageIndex;

      } // if( kTRUE == (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex].Is32Bit() )
         else
         {
            UChar_t ucChan = (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex].GetGet4ChNum();

            // 24 Bits data message => Rising and falling edge need to be associated to get a hit
            if( 1 == (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex].IsRisingGet4Edge() )
            {
               // Trailing Edge
               // If no leading edge there, as messages are time ordered, means that
               // the leading edge of this hit is missing => no need to save it
               if( kTRUE == (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).IsTimeSet() )
               {
                  (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).SetTot24Bits(
                        (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex] );

                  // Check if hit is not too big
                  // If it is we can clear this one as anyway messages are time ordered!
                  if( (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTot() < 0)
                  {
                     cout<<"Negative 24b ToT in Build Older: Mess "<<uMessageIndex;
                     cout<<" Tot "<<(ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTot();
                     cout<<" Test "<<( ((ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTimeMessage()) < ((ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTotMessage()) )<<endl;
                     ((ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTimeMessage()).Print(roc::msg_print_Human);
                     ((ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTotMessage()).Print(roc::msg_print_Human);
                     cout<<endl;
                     if( kTRUE == CheckHit( uRocId, uGet4Id, (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]) ) )
                        uLastAcceptedMessageIndex = uMessageIndex;
                  }
                     else if( (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).GetTot() < fParam->dMaxTot )
                     {
                        if( kTRUE == CheckHit( uRocId, uGet4Id, (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]) ) )
                           uLastAcceptedMessageIndex = uMessageIndex;
                     }
                     else (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).Clear();
               } // if( kTRUE == fHitTemp.IsTimeSet() )
            }
               else
               {
                  // Leading edge
                  // clear anyway as time ordered, worst case there was another leading edge
                  // whose trailing edge is missing
                  (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).Clear();
                  (ROC[uRocId].fHitTemp24[uGet4Id][ucChan]).SetTime24Bits(
                        (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild])[uMessageIndex] );
               }
         } // else of if( kTRUE == (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bOlderBuffer])[uMessageIndex].Is32Bit() )
   } // for loop over older buffer = full epoch one

   // Erase all messages until last accepted one
   // This can be done only if we don't mix event (always strictly consecutive events)

   if( kFALSE == fParam->bAllowEventMixing)
      (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild]).erase(
         (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild]).begin(),
         (ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild]).begin() + uLastAcceptedMessageIndex );

   /*
   cout<<"Building hits, size old = "<<(ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bBufferToBuild]).size()<<" (buffer "<<bBufferToBuild<<" )";
   cout<<" size new = "<<(ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][bCurrentBuffer]).size()<<" (buffer "<<bCurrentBuffer<<" )"<<endl;
   */

   return kTRUE;
}


Bool_t TTofGet4Unpacker::CheckHit(UInt_t uRocId, UInt_t uGet4Id, Get4v1Hit& hit)
{
   /* TODO FILL ME
    * Check if Get4 Hit matches the current trigger and
    * if yes save it in output event
    */
   Double_t dTimeToTrigger = 0.0;
   Bool_t bSelectedHit = kFALSE;

   if ( 0.0 < fCurrentGet4Event.fdTriggerFullTime[uRocId] )
   {
      dTimeToTrigger = hit.GetHitFullTimeD() - fCurrentGet4Event.fdTriggerFullTime[uRocId];

      fDeltaTriggerTime->Fill( dTimeToTrigger );
      ROC[uRocId].fTrigger_All->Fill( dTimeToTrigger );
      ROC[uRocId].fTrigger_All_100->Fill( dTimeToTrigger );
      ROC[uRocId].fTrigger_All_us->Fill( dTimeToTrigger );
      ROC[uRocId].fTrigger_All_ms->Fill( dTimeToTrigger );
      ROC[uRocId].fTrigger_All_s->Fill( dTimeToTrigger );

      UChar_t uGet4Chan = hit.GetChan();
      ROC[uRocId].fTrigger_Get4Channel[uGet4Id][uGet4Chan]->Fill( dTimeToTrigger );
      ROC[uRocId].fTriggerMs_Get4Channel[uGet4Id][uGet4Chan]->Fill( dTimeToTrigger );
      ROC[uRocId].fTriggerS_Get4Channel[uGet4Id][uGet4Chan]->Fill( dTimeToTrigger );


      if( kTRUE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
      {
         bSelectedHit =kTRUE;
         // Hit is in trigger selection window => add it to the event
         ( ( fCurrentGet4Event.fGet4Boards[uGet4Id] ).fHits[ hit.GetChan() ]).push_back( hit );

         // Count per global time unit in s
         Double_t dSyncTm = hit.GetHitFullTimeD() / 100000000.0;
         ROC[uRocId].fSelectedT[uGet4Id][uGet4Chan]->Fill(( dSyncTm ) * 0.1);

         ROC[uRocId].fAllSelectedT->Fill(( dSyncTm ) * 0.1 );
         // Long duration time distribution (1 min. bin, 10 days length)
         if( kTRUE == fParam->bLongTimeHistos )
            ROC[uRocId].fAllSelLongT->Fill( (dSyncTm * 0.1 ) / 3600 );

         // Filling histos:
         if( 1 == fParam->bTotHistoEnable )
         {
            ROC[uRocId].fTot[uGet4Id][hit.GetChan()]->Fill( hit.GetTot() );
            if( hit.GetTot() < 0)
            {
               cout<<"Negative 24b ToT in Check: "<<hit.GetTot()<<endl;
               (hit.GetTimeMessage()).Print(roc::msg_print_Human);
               (hit.GetTotMessage()).Print(roc::msg_print_Human);
               cout<<endl;
            }
         }
         if( 1 == fParam->bDebugHistoEnable )
         {
            ROC[uRocId].fLeadingFTS[uGet4Id][hit.GetChan()]->Fill( hit.GetHitLeadingFTS() );
            if( kFALSE == hit.Is32Bit() )
               ROC[uRocId].fTrailingFTS[uGet4Id][hit.GetChan()]->Fill( hit.GetHitTrailingFTS() );
         } // if( 1 == fParam->bDebugHistoEnable )
      }
   } // if ( 0.0 < fCurrentGet4Event.fdTriggerFullTime[uRocId])
      else LOG(error)<<"Trying to check a hit validity while no trigger found for current event";

   return bSelectedHit;
}
void TTofGet4Unpacker::CheckEventClosure()
{
   /* TODO FILL ME
    * Checking if event finiched for all ROCs/GET4s
    * saving it
    */

   // If not trigger yet => no need to check!
   if( kFALSE == fCurrentGet4Event.HasTrigger() )
      return;

   Bool_t bAllFutureEpochsFinished = kTRUE;
   // Check flag indicating the extra epoch after trigger is passed
   for( UInt_t  uRocId =0; uRocId<fParam->numRocs; uRocId++ )
   {
      // do not check for non-used ROC
      if (!fParam->IsActiveRoc(uRocId)) continue;

      if( kFALSE == ROC[uRocId].fbSelectionRocDone )
         bAllFutureEpochsFinished = kFALSE;

      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      {
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRocId, uGet4);
         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
            if( kFALSE == ROC[uRocId].fbSelectionDone[uGet4] )
               bAllFutureEpochsFinished = kFALSE;
      }
   }

   // extra epoch passed for all => save event
   if( kTRUE == bAllFutureEpochsFinished )
   {
      if( kFALSE == fCurrentGet4Event.IsEmpty() )
      {
         AnalyzeAllGet4Channels( fCurrentGet4Event );
         ///////////////////////////////////////////////
         // Replaced with cbmroot output format
         // Only 1 event used per MBS event!
//         (fOutputEvent->fEvents).push_back( fCurrentGet4Event );
         if( kFALSE == fbDataSavedForThisEvent )
         {
            // First event found in this MBS event
            for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
            {
               TTofGet4Board * fGet4Board = (TTofGet4Board*) fGet4BoardCollection->ConstructedAt(uGet4);
               if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
               {
                  fGet4Board->SetTriggerFullTime( fCurrentGet4Event.fdTriggerFullTime[0] ); // Use only first ROC!!!!
                  fGet4Board->SetSyncEventFlag( fCurrentGet4Event.fbMbsSyncedEvent );
                  fGet4Board->SetSyncEvtNb( fCurrentGet4Event.fuSyncEventNumber );
                  fGet4Board->SetDataEventFlag( fCurrentGet4Event.fbDataTriggered );
                  fGet4Board->SetTriggIndx( fCurrentGet4Event.fuTriggerIndex );

                  // Some Data found for this TDC Chip
                  for( UInt_t uCh = 0; uCh < get4v10::kuNbChan; uCh++ )
                     for( UInt_t uHit = 0;
                          uHit < ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh]).size();
                          uHit++ )
                     {
                        TGet4v1MessageExtended extTime = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTimeMessage();
                        TGet4v1MessageExtended extTot  = ((fCurrentGet4Event.fGet4Boards[uGet4]).fHits[uCh])[uHit].GetTotMessage();

                        if( kFALSE == extTime.IsEmpty() )
                        {
                           TTofGet4Data dataTime;
                           dataTime.SetEdge( 0 );
                           if( kTRUE == extTime.Is32Bit() )
                           {
                              dataTime.SetChannel( uCh );
                              dataTime.SetFineTime(   extTime.getGet4V10R32HitFt() );
                              dataTime.SetCoarseTime( extTime.getGet4V10R32HitTs() );
                              dataTime.SetTot(        extTime.getGet4V10R32HitTot() );
                           } // if( kTRUE == extTime.Is32Bit() )
                              else
                              {
                                 // Time saved in "odd channels" as example of VFTX
                                 dataTime.SetChannel( 2*uCh + 1 );
                                 dataTime.SetFineTime(    extTime.GetGet4Ts() & get4tdc::kiFineTime );
                                 dataTime.SetCoarseTime( (extTime.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                              } // else of if( kTRUE == extTime.Is32Bit() )
                           dataTime.SetEpoch( extTime.GetRocEpoch() );
                           dataTime.SetEpochCycle( extTime.GetRocCycle() );
                           dataTime.SetFullTimeD( extTime.GetFullTimeD() );
                           dataTime.Set32bFlag( extTime.Is32Bit() );
                           fGet4Board->AddData( dataTime );
                        } // if( kFALSE == extTime.IsEmpty() )

                        if( kFALSE == extTot.IsEmpty() )
                        {
                           // Will appear only in case of 24bit mode !!!
                           TTofGet4Data dataTot;
                           // Tot saved in "even channels" as example of VFTX
                           dataTot.SetChannel( 2*uCh );
                           dataTot.SetEdge( 1 );
                           dataTot.SetFineTime(    extTot.GetGet4Ts() & get4tdc::kiFineTime );
                           dataTot.SetCoarseTime( (extTot.GetGet4Ts() & get4tdc::kiCoarseTime )>>get4tdc::kiCtShift );
                           dataTot.SetEpoch( extTot.GetRocEpoch() );
                           dataTot.SetEpochCycle( extTot.GetRocCycle() );
                           dataTot.SetFullTimeD( extTot.GetFullTimeD() );
                           dataTot.Set32bFlag( extTot.Is32Bit() );
                           fGet4Board->AddData( dataTot );
                        } // if( kFALSE == extTot.IsEmpty() )
                     } // scan over all hits in this board object

               } // if( 0 < (fCurrentGet4Event.fGet4Boards[uGet4]).Size() )
            } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4; uGet4++)
            fbDataSavedForThisEvent = kTRUE;
         } // if( kFALSE == fbDataSavedForThisEvent )
         ///////////////////////////////////////////////

         for( UInt_t  uRocId =0; uRocId<fParam->numRocs; uRocId++ )
         {
            uNotEmptyEventInsideMbsEvtCount[uRocId]++;
            uNotEmptyRealEventsCount[uRocId]++;
         }
      } // if( kFALSE == fCurrentGet4Event.IsEmpty() )

      fCurrentGet4Event.Clear();
   } // if( kTRUE == bAllFutureEpochsFinished )
}

void TTofGet4Unpacker::ResetTrigger()
{
   /* TODO FILL ME
    * Trigger resetting => Needed?
    */
}


void TTofGet4Unpacker::ResetEndOfBuffer()
{
   /* TODO FILL ME
    * Buffer resetting => Needed?
    */
}

/**********************************************************************/
/*
 * This function process the data message when it is a ROC Sync message
 */
Bool_t TTofGet4Unpacker::ProcessRocSyncMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * ROC Sync message specific processing
    * TODO: Check on Sync number from current event to avoid
    * multiple events created for same sync when using more than 1 ROC
    */
   UInt_t uSyncId = (extMess.GetRocMessage()).getSyncChNum();
   ULong64_t ulFulltime = extMess.GetFullTime();

   // Recalculate the full Time to take into account also the epoch cycles
   ulFulltime  = (extMess.GetRocMessage()).getMsgFullTime(
                       ROC[uRocId].GetFullEpochNumber( ));
   ulFulltime += ROC[uRocId].GetSuperCycleEpOffset();
   extMess.SetFullTime(ulFulltime);

   if( uSyncId < get4v10::kuMaxSync )
   {
      ROC[uRocId].fLastSyncTm[uSyncId] = ulFulltime;
      ROC[uRocId].fLastSyncId[uSyncId] = (extMess.GetRocMessage()).getSyncData();
      /* TODO ?
      if (rocevent)
         rocevent->fLastSyncTm[uSyncId] = ulFulltime;
         */

      // Count per global time unit in s
      ULong64_t ulSyncTm = ulFulltime / 100000000L;
      ROC[uRocId].fSYNCt[uSyncId]->Fill((ulSyncTm % 36000) * 0.1);

      // Long duration time distribution (1 min. bin, 10 days length)
      if( kTRUE == fParam->bLongTimeHistos )
         ROC[uRocId].fSyncLongT[uSyncId]->Fill( (ulSyncTm * 0.1 ) / 3600 );

      if( kFALSE ==  fParam->bFreeStreaming || 0 == fParam->uNbTriggers )
         if( fParam->triggerSignal == (Int_t) (uSyncId + 10) ) // then may use it for new trigger
            ProcessTriggerMessage( uRocId, extMess );
   } // if( uSyncId < get4v10::kuMaxSync )
   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a ROC epoch message
 */
Bool_t TTofGet4Unpacker::ProcessRocEpochMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * Roc epoch message specific processing
    */
   if( ROC[uRocId].fuCurrEpoch > (extMess.GetRocMessage()).getEpochNumber() )
   {
      ROC[uRocId].fuEpochCycle++;
      TString sTemp = Form("Roc Epoch cycle change: %u", ROC[uRocId].fuEpochCycle);
      LOG(info)<<sTemp;
   }
   ROC[uRocId].fuCurrEpoch = (extMess.GetRocMessage()).getEpochNumber();

   // Recalculate the full Time to take into account also the epoch cycles
   ULong64_t ulFulltime = ((ULong64_t)  ROC[uRocId].GetFullEpochNumber() ) << 14;
   extMess.SetFullTime(ulFulltime);

   // Count per global time unit in s
   ULong64_t ulEpochTm = ulFulltime / 100000000L;
   ROC[uRocId].fEPOCHt->Fill((ulEpochTm % 36000) * 0.1);

   // Long duration time distribution (1 min. bin, 10 days length)
   if( kTRUE == fParam->bLongTimeHistos )
      ROC[uRocId].fEpochLongT->Fill( (ulEpochTm * 0.1 ) / 3600 );

   ROC[uRocId].fDistribEpochs->Fill( ROC[uRocId].fuCurrEpoch );

   if(  kTRUE == fCurrentGet4Event.HasTrigger() &&
       kFALSE == ROC[uRocId].fbSelectionRocDone  )
   {
      Double_t dTimeToTrigger = 0.0;
      dTimeToTrigger = extMess.GetFullTimeD() - fCurrentGet4Event.fdTriggerFullTime[uRocId];
      // If flag is kTRUE: we are at the second epoch since trigger
      // => rest of trigger epoch + 1 full epoch elapsed, enough for ordering issues
      // => close hit selection checks for ROC self message
      // If Epoch is out of the trigger window started on trigger
      // => all hits always contained inside epoch even if not time ordered
      // => No need to look at the next epoch!!!
      if( kTRUE == ROC[uRocId].fbEpochSinceTrigger ||
          kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger)  )
      {
         ROC[uRocId].fbSelectionRocDone  = kTRUE;
         ROC[uRocId].fbEpochSinceTrigger = kFALSE;
         CheckEventClosure();
      }
      else ROC[uRocId].fbEpochSinceTrigger = kTRUE;
   } // If selection of hits after trigger detection not over
/*
   // TODO: Maybe necessary?
   // As no empty epoch there after data anymore, try to use ROC epoch as
   // empty epochs... unsafe because of ordering, but epochs with data
   // should anyway have the flag ON and an epoch2 message coming after,
   // thus keeping the ordering containment
   if( kTRUE == fParam->bSuppressedEpochs && kTRUE == fCurrentGet4Event.HasTrigger())
      for( UInt_t uGet4Id = 0; uGet4Id < get4v10::kuMaxGet4Roc; uGet4Id ++)
         if( kTRUE == fParam->IsActiveGet4Chip(uGet4Id) &&
              0 == ROC[uRocId].fuEpochWithData[uGet4Id] &&
              kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )
         {
            // A trigger is ongoing on this chip!
            if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
            {
               // This is the first epoch after the trigger
               // => time to look for the data in the epoch which was
               // ongoing when the trigger was found

               // If we did not process the epoch before the one where the
               // trigger came, we need to initialize the 24 bit temp hits
               if( kFALSE == ROC[uRocId].bLookInPreviousEpoch[uGet4Id] )
                  for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                        (ROC[uRocId].fHitTemp24[uGet4Id][uChan]).Clear();

               BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
               if( kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id] )
               {
                  ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
                  CheckEventClosure();
               }
               ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] = kTRUE;
            } // if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
            else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] &&
                      kTRUE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
            {
               // This is the second epoch after the trigger and we detected on trigger
               // that the trigger window extend in epoch after the one where trigger
               // was found => time to look for the data in the epoch which just finished
               // and was the one just after the trigger epoch
               BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
               ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
               CheckEventClosure();
            } // else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] && kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
         } // if chip active and not over
*/
   // Swap the flag indicating which buffer contains currently happening epoch
   ROC[uRocId].fbBufferWithLastFullEpoch = ( kTRUE == ROC[uRocId].fbBufferWithLastFullEpoch ? kFALSE: kTRUE );
   // Clear this buffer (Data more than 1 full epoch old => either already selected or junk)
   ROC[uRocId].fPrevEpochsBuffer[ ROC[uRocId].fbBufferWithLastFullEpoch ].clear();

   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a GET4 epoch message
 */
Bool_t TTofGet4Unpacker::ProcessGet4EpochMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * 24bit Get4 epoch message specific processing
    */

   UInt_t uGet4Id(0);

   // Remap the Get4 chip index => Go in Get4 indexing common to all ROCs
   uGet4Id = (extMess.GetRocMessage()).getEpoch2ChipNumber();
   uGet4Id = fParam->RemapGet4Chip( fParam->DefineGet4IndexOffset(uRocId) +  uGet4Id);
   extMess.SetEpoch2ChipNumber(uGet4Id);

   // Check if active chip
   if( kFALSE == fParam->IsValidGet4Chip(uGet4Id) )
   {
       cout << "Error: Bad chip nb in Epoch message = " << uGet4Id << endl;
       cout << " => This message will be skipped!!!! "<<endl;
       return kFALSE;
   }
   if( kFALSE == fParam->IsActiveGet4Chip(uGet4Id) )
   {
      // Masked chip, hopefully on purpose, don't complain & return
      return kFALSE;
   }

   if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )
   {
      // A trigger is ongoing on this chip!
      if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
      {
         // This is the first epoch after the trigger
         // => time to look for the data in the epoch which was
         // ongoing when the trigger was found

         // If we did not process the epoch before the one where the
         // trigger came, we need to initialize the 24 bit temp hits
         if( kFALSE == ROC[uRocId].bLookInPreviousEpoch[uGet4Id] )
            for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                  (ROC[uRocId].fHitTemp24[uGet4Id][uChan]).Clear();

         BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
         if( kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id] )
         {
            ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
            CheckEventClosure();
         }
         ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] = kTRUE;
      } // if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
      else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] &&
                kTRUE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
      {
         // This is the second epoch after the trigger and we detected on trigger
         // that the trigger window extend in epoch after the one where trigger
         // was found => time to look for the data in the epoch which just finished
         // and was the one just after the trigger epoch
         BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
         ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
         CheckEventClosure();
      } // else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] && kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
   } // if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )

   UInt_t get4_24b_ep_epoch = (extMess.GetRocMessage()).getEpoch2Number();
   if( ROC[uRocId].fuCurrEpoch2[uGet4Id] + 1 != get4_24b_ep_epoch &&
       ( 0 < ROC[uRocId].fuCurrEpoch2[uGet4Id] || 0 < ROC[uRocId].fuEpoch2Cycle[uGet4Id] ) &&
       !( get4v10::kulGet4EpochCycleSz == ROC[uRocId].fuCurrEpoch2[uGet4Id]  && 0 == get4_24b_ep_epoch ) )
   {
      LOG(info)<<"Roc Get4 Epoch2 error in 24b mode";
      ROC[uRocId].fEpochShiftsPerChip->Fill( uGet4Id, (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4Id]+1) );

      // Correction attempt: just calculating, not using it
      if( (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4Id]+1) < 10 )
      {
         // close epochs ( < 250us) => probable not a full event missing
         if( 0 != ROC[uRocId].fiEpochShift[uGet4Id])
            ROC[uRocId].fEpochShiftsDuration[uGet4Id]->Fill(ROC[uRocId].fiEpochShift[uGet4Id],
                  ROC[uRocId].fuNbShiftedEpochs[uGet4Id] );
         ROC[uRocId].fiEpochShift[uGet4Id] += (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4Id]+1) ;
         ROC[uRocId].fuNbShiftedEpochs[uGet4Id] = 0;
      }
         else
         {
            // far epochs ( > 250us) => probable a full event is missing
            if( 0 != ROC[uRocId].fiEpochShift[uGet4Id])
               ROC[uRocId].fEpochShiftsDuration[uGet4Id]->Fill(ROC[uRocId].fiEpochShift[uGet4Id],
                     ROC[uRocId].fuNbShiftedEpochs[uGet4Id] );
            ROC[uRocId].fiEpochShift[uGet4Id]      = 0 ;
            ROC[uRocId].fuNbShiftedEpochs[uGet4Id] = 0;
         }
   } // if non consecutive epoch indexes
      else
      {
         if( 0 != ROC[uRocId].fiEpochShift[uGet4Id] )
            ROC[uRocId].fuNbShiftedEpochs[uGet4Id]++;
      } // else of if non consecutive epoch indexes

   // Event statistics
   ROC[uRocId].fuNbHitsChipEpoch[uGet4Id] = 0;

   if( ROC[uRocId].fuCurrEpoch2[uGet4Id] > get4_24b_ep_epoch )
   {
      TString sTemp = Form("Roc Epoch2 cycle (24b mode) change: %u", ROC[uRocId].fuEpoch2Cycle[uGet4Id]);
      LOG(info)<<sTemp;
      ROC[uRocId].fuEpoch2Cycle[uGet4Id]++;
   }
   ROC[uRocId].fuCurrEpoch2[uGet4Id] = get4_24b_ep_epoch;

   // In case of suppressed epochs, we will have only chip 0 epochs for all empty epochs
   // We then use these epochs for all other active chips
   // However, when a chip has data, its next epoch message should be present!!!
   if( kTRUE == fParam->bSuppressedEpochs )
   {
      // Assume Chip 0 are always there, even if disabled
      // TODO: test this and at some other behavior if needed
      if( 0 == uGet4Id )
         for( UInt_t uGet4IdBis = uGet4Id + 1; uGet4IdBis < get4v10::kuMaxGet4Roc; uGet4IdBis ++)
            if( kTRUE == fParam->IsActiveGet4Chip(uGet4IdBis) )
            {
               // In case no data was found in previous epoch for this chip, we use the epoch info
               // from chip 0
               if( 0 == ROC[uRocId].fuEpochWithData[uGet4IdBis])
               {
                  if( ROC[uRocId].fuCurrEpoch2[uGet4IdBis] + 1 != get4_24b_ep_epoch &&
                      ( 0 < ROC[uRocId].fuCurrEpoch2[uGet4IdBis] || 0 < ROC[uRocId].fuEpoch2Cycle[uGet4IdBis] ) &&
                      !( get4v10::kulGet4EpochCycleSz == ROC[uRocId].fuCurrEpoch2[uGet4IdBis]  && 0 == get4_24b_ep_epoch ) )
                  {
                     LOG(info)<<"Roc Get4 Epoch2 error in 24b mode";
                     ROC[uRocId].fEpochShiftsPerChip->Fill( uGet4IdBis, (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4IdBis]+1) );

                     // Correction attempt: just calculating, not using it
                     if( (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4IdBis]+1) < 10 )
                     {
                        // close epochs ( < 250us) => probable not a full event missing
                        if( 0 != ROC[uRocId].fiEpochShift[uGet4IdBis])
                           ROC[uRocId].fEpochShiftsDuration[uGet4IdBis]->Fill(ROC[uRocId].fiEpochShift[uGet4IdBis],
                                 ROC[uRocId].fuNbShiftedEpochs[uGet4IdBis] );
                        ROC[uRocId].fiEpochShift[uGet4IdBis] += (Int_t)get4_24b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[uGet4IdBis]+1) ;
                        ROC[uRocId].fuNbShiftedEpochs[uGet4IdBis] = 0;
                     }
                        else
                        {
                           // far epochs ( > 250us) => probable a full event is missing
                           if( 0 != ROC[uRocId].fiEpochShift[uGet4IdBis])
                              ROC[uRocId].fEpochShiftsDuration[uGet4IdBis]->Fill(ROC[uRocId].fiEpochShift[uGet4IdBis],
                                    ROC[uRocId].fuNbShiftedEpochs[uGet4IdBis] );
                           ROC[uRocId].fiEpochShift[uGet4IdBis]      = 0 ;
                           ROC[uRocId].fuNbShiftedEpochs[uGet4IdBis] = 0;
                        }
                  } // if non consecutive epoch indexes
                     else
                     {
                        if( 0 != ROC[uRocId].fiEpochShift[uGet4IdBis] )
                           ROC[uRocId].fuNbShiftedEpochs[uGet4IdBis]++;
                     } // else of if non consecutive epoch indexes

                  // Event statistics
                  ROC[uRocId].fuNbHitsChipEpoch[uGet4IdBis] = 0;

                  if( ROC[uRocId].fuCurrEpoch2[uGet4IdBis] > get4_24b_ep_epoch )
                  {
                     TString sTemp = Form("Roc Epoch2 cycle (24b mode) change: %u", ROC[uRocId].fuEpoch2Cycle[uGet4Id]);
                     LOG(info)<<sTemp;
                     ROC[uRocId].fuEpoch2Cycle[uGet4IdBis]++;
                  }
                  ROC[uRocId].fuCurrEpoch2[uGet4IdBis] = get4_24b_ep_epoch;
               } // if( 0 == ROC[uRocId].fuEpochWithData[uGet4IdBis])
                  // If data just increase the flag, should not hurt,
                  // just in case the epoch messages are not always coming ordered the way I expect
                  else ROC[uRocId].fuEpochWithData[uGet4IdBis]++;
            } // if( kTRUE == fParam->IsActiveGet4Chip(uGet4IdBis) )

      ROC[uRocId].fuEpochWithData[uGet4Id] = 0;
   } // if( kTRUE == fParam->bSuppressedEpochs)

   // Recalculate the full Time to take into account also the epoch2 cycles
   ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( uGet4Id )) << 19;
   ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
   Double_t  dFullTime = uFullTimeBins / 20. + 512.;
   extMess.SetFullTime(  uFullTime );
   extMess.SetFullTimeD( dFullTime );

   // Set The ROC epoch2 counter and cycle counter
   extMess.SetRocEpoch( ROC[uRocId].fuCurrEpoch2[uGet4Id] );
   extMess.SetRocCycle( ROC[uRocId].fuEpoch2Cycle[uGet4Id] );

   // Count per global time unit in s
   ULong64_t ulEpochTm = uFullTime / 100000000L;
   ROC[uRocId].fEPOCH2t[uGet4Id]->Fill((ulEpochTm % 36000) * 0.1);

   // Long duration time distribution (1 min. bin, 10 days length)
   if( kTRUE == fParam->bLongTimeHistos )
      ROC[uRocId].fAllEpoch2LongT->Fill( (ulEpochTm * 0.1 ) / 3600 );

   ROC[uRocId].fDistribEpochs2->Fill(uGet4Id, ROC[uRocId].fuCurrEpoch2[uGet4Id]);

   // Swap the flag indicating which buffer contains currently happening epoch
   ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] = ( kTRUE == ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] ? kFALSE: kTRUE );
   // Clear this buffer (Data more than 1 full epoch old => either already selected or junk)
   ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][ ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] ].clear();

   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a GET4 data message
 */
Bool_t TTofGet4Unpacker::ProcessGet4DataMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * 24bit Get4 hit message specific processing
    *
    * TODO
    * Make the get4_index consistent everywhere between global get4 index and inside roc get4 index
    */

   if (ROC[uRocId].bIgnoreData) return kFALSE;

   if( kFALSE == ROC[uRocId].fb24bitsReadoutDetected )
   {
      ROC[uRocId].fb24bitsReadoutDetected = kTRUE;
      fb24bitsReadoutDetected = kTRUE;
      // As it is first 24bit message, let's create
      // 24bit specific histograms
      char folder[30];
      sprintf(folder,"ROC%u/",uRocId);

      /*
       * 24 bits specific histograms
       */
      ROC[uRocId].fChannelInputMessCount = new TH1I( Form("Roc%u_ChanCounts", uRocId),
            "Channel multiplicity per event; Channel []; # Mess []" ,
            2*get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc);
      ROC[uRocId].fb24bitsReadoutDetected = kTRUE;
   } // if( kFALSE == ROC[uRocId].fb24bitsReadoutDetected )

   UInt_t    uGet4Id(0), uGet4Ch(0), uGet4Edge(0);
   uGet4Id   = (extMess.GetRocMessage()).getGet4Number();
   uGet4Ch   = (extMess.GetRocMessage()).getGet4ChNum();
   uGet4Edge = (extMess.GetRocMessage()).getGet4Edge();

   // Remap the Get4 chip index
   uGet4Id = (extMess.GetRocMessage()).getGet4Number();
   uGet4Id = fParam->RemapGet4Chip( fParam->DefineGet4IndexOffset(uRocId) + uGet4Id);
   extMess.SetGet4Number(uGet4Id);

   // Check if valid chip
   if( kFALSE == fParam->IsValidGet4Chip(uGet4Id) )
   {
       cout << "Error: Bad chip nb in Data message = " << uGet4Id << endl;
       cout << " => This message will be skipped!!!! "<<endl;
       return kFALSE;
   }
   if( kFALSE == fParam->IsActiveGet4Chip(uGet4Id) )
   {
      // Masked chip, hopefully on purpose, don't complain & return
      return kFALSE;
   }

   // Recalculate the full Time to take into account also the epoch2 cycles
   ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( uGet4Id ) << 19) |
                              (ULong64_t)extMess.GetGet4Ts();
   ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
   Double_t  dFullTime = ((Double_t)uFullTimeBins)*0.05 + 512.;
   extMess.SetFullTime(  uFullTime );
   extMess.SetFullTimeD( dFullTime );

   // Set The ROC epoch2 counter and cycle counter
   extMess.SetRocEpoch( ROC[uRocId].fuCurrEpoch2[uGet4Id] );
   extMess.SetRocCycle( ROC[uRocId].fuEpoch2Cycle[uGet4Id] );


   // Count per global time unit in s
   ULong64_t ulDataTm = uFullTime / 100000000L;
   ROC[uRocId].fDATAt->Fill((ulDataTm % 36000) * 0.1);

   // Long duration time distribution (1 min. bin, 10 days length)
   if( kTRUE == fParam->bLongTimeHistos )
      ROC[uRocId].fDataLongT->Fill( (ulDataTm * 0.1 ) / 3600 );

   // Event statistics
   ROC[uRocId].fChannelInputMessCount->Fill( uGet4Id + (Double_t)uGet4Ch/(Double_t)get4v10::kuNbChan
                                                     + (Double_t)uGet4Edge/(Double_t)(2*get4v10::kuNbChan)  );
   if( kTRUE == fParam->bChipRateHistoEnable )
      ROC[uRocId].fChipRateEvolution[uGet4Id]->Fill(
            (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
   if( kTRUE == fParam->bChannelRateHistoEnable )
      ROC[uRocId].fChannelRateEvolution[uGet4Id][uGet4Ch]->Fill(
            (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
   ROC[uRocId].fuNbHitsChipEpoch[uGet4Id] ++;

   // Early event closure check
   if( kTRUE == fCurrentGet4Event.HasTrigger() &&
       kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )
   {
      Double_t dTimeToTrigger = 0.0;
      dTimeToTrigger = extMess.GetFullTimeD() - fCurrentGet4Event.fdTriggerFullTime[uRocId];
      if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
      {
         // Message after the end of trigger window!!
         Bool_t bAllChanInChipOver = kTRUE;

         // If there was already a message from the same channel & edge out of the trigger
         //  window, then the token ring made at least one full turn => ok for all
         // Otherwise: need to check other channels/edge status
         if( (kFALSE == ROC[uRocId].fbDataTimeOut[uGet4Id][uGet4Ch] && 0 == uGet4Edge ) ||
             (kFALSE == ROC[uRocId].fbDataFallingOut[uGet4Id][uGet4Ch] && 1 == uGet4Edge ) )
         {
            // 24b = always a edge not a hit => need to check both !
            if( 0 == uGet4Edge )
               ROC[uRocId].fbDataTimeOut[uGet4Id][uGet4Ch] = kTRUE;
               else ROC[uRocId].fbDataFallingOut[uGet4Id][uGet4Ch] = kTRUE;
            // Check if chip fully finished
            for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
            {
               if( kFALSE ==  ROC[uRocId].fbDataTimeOut[uGet4Id][uChanTest] )
                  bAllChanInChipOver = kFALSE;
               if( kFALSE ==  ROC[uRocId].fbDataFallingOut[uGet4Id][uChanTest] )
                  bAllChanInChipOver = kFALSE;
            }
         } // if same edge not already out of trigger once

         // If chip fully finished: don't wait for the next epoch to close future buffer
         // of current event for this chip
         if( kTRUE == bAllChanInChipOver)
         {
            // If This is the first epoch after the trigger and
            // we did not process the epoch before the one where the
            // trigger came, we need to initialize the 24 bit temp hits
            if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] &&
                kFALSE == ROC[uRocId].bLookInPreviousEpoch[uGet4Id] )
               for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                     (ROC[uRocId].fHitTemp24[uGet4Id][uChan]).Clear();

            BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
            ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
            CheckEventClosure();

            for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
            {
               ROC[uRocId].fbDataTimeOut[uGet4Id][uChanTest] = kFALSE;
               ROC[uRocId].fbDataFallingOut[uGet4Id][uChanTest] = kFALSE;
            }
         } // if( kTRUE == bAllChanInChipOver)
      } // if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
   } // Event ongoing with a detected trigger and chip did not finished all its buffers

   /** Triggering on Data **/
   if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
   {
      /** Rising edge: Triggering on Data **/
      if( 0 == uGet4Edge )
      {
         for( Int_t iMainTriggerIndex = 0; iMainTriggerIndex < (Int_t)(fParam->uNbTriggers); iMainTriggerIndex++ )
            if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )
         {
            ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex] = extMess;
            /*
             * TODO: call to ProcessTrigger
             * TEMP: histo Filling + counting
             */
            Bool_t bAllInCoinc = kTRUE;
            for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
            {
               Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iMainTriggerIndex][uSecChannel]
                                        - extMess.GetFullTimeD();
               if( dDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                     fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dDistanceToMain      )
               {
                  bAllInCoinc = kFALSE;
                  break;
               } // if out of coincidence window
               Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iMainTriggerIndex][uSecChannel]
                                        - extMess.GetFullTimeD();
               if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                     fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotDistanceToMain      )
               {
                  bAllInCoinc = kFALSE;
                  break;
               } // if out of coincidence window
            } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)

            // Check if tot edge of the main channel is in trigger window
            Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex]
                              - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
            if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                  fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotMainDistanceToMain      )
            {
               bAllInCoinc = kFALSE;
            } // if out of coincidence window

            if( kTRUE == bAllInCoinc )
            {
               Double_t dSameTriggerDistance =
                       ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD()
                     - ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] ;

               ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iMainTriggerIndex );
               ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iMainTriggerIndex );
               ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iMainTriggerIndex );
               ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iMainTriggerIndex );

               if( fParam->dDeadTime <= dSameTriggerDistance )
               {
                  // Here we have a DATA self-trigger!!!!!!
                  ROC[uRocId].fuDataSelfTriggerCount[iMainTriggerIndex] ++;

                  ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] =
                        ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();

                  ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTime();
                  uFullTimeSelfTrigBins /= 100000000L;
                  ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                  // Long duration time distribution (1 min. bin, 10 days length)
                  if( kTRUE == fParam->bLongTimeHistos )
                     ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                  ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex], iMainTriggerIndex);
               } // if( fParam->dDeadTime <= dSameTriggerDistance )
            } // if( kTRUE == bAllInCoinc )
         } // if( -1 < iMainTriggerIndex )

         for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
         {
            if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
            {
               Int_t iSecondaryIndex = fParam->GetSecChannelIndex(iSecTriggerIndex, uGet4Id, uGet4Ch);
               if( -1 < iSecondaryIndex )
               {
                  ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][iSecondaryIndex] = extMess.GetFullTimeD();

                  /*
                   * TODO: call to ProcessTrigger
                   * TEMP: histo Filling + counting
                   */
                  Bool_t bAllInCoinc = kTRUE;
                  for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                  {
                     Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][uSecChannel]
                                       - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                     if( dDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                        break;
                     } // if out of coincidence window

                     // Check if tot edge is in trigger window, only needed in 24b mode
                     Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][uSecChannel]
                                       - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                     if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                        break;
                     } // if out of coincidence window
                  } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)

                  // Check if tot edge of the main channel is in trigger window, only needed in 24b mode
                  Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iSecTriggerIndex]
                                    - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                  if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotMainDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                  } // if out of coincidence window

                  if( kTRUE == bAllInCoinc  )
                  {
                     Double_t dSameTriggerDistance =
                             ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD()
                           - ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] ;

                     ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iSecTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iSecTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iSecTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iSecTriggerIndex );

                     if( fParam->dDeadTime <= dSameTriggerDistance )
                     {
                        // Here we have a DATA self-trigger!!!!!!
                        ROC[uRocId].fuDataSelfTriggerCount[iSecTriggerIndex] ++;

                        ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] =
                              ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();

                        ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTime();
                        uFullTimeSelfTrigBins /= 100000000L;
                        ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                        // Long duration time distribution (1 min. bin, 10 days length)
                        if( kTRUE == fParam->bLongTimeHistos )
                           ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                        ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex], iSecTriggerIndex);
                     } // if( fParam->dDeadTime <= dSameTriggerDistance )
                  } // if( kTRUE == bAllInCoinc  )
               } // if( -1 < iSecondaryIndex )
            } //  if( -1 < iSecTriggerIndex )
         } // for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
      }// if( 0 == uGet4Edge )
         /** Falling edge: Triggering on Data, only needed in 24b mode **/
         else
         {
            // Falling edge
            for( Int_t iMainTriggerIndex = 0; iMainTriggerIndex < (Int_t)(fParam->uNbTriggers); iMainTriggerIndex++ )
               if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )
            {
               ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex] = extMess.GetFullTimeD();

               Bool_t bAllInCoinc = kTRUE;
               for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
               {
                  // Check if time edge is in trigger window
                  Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iMainTriggerIndex][uSecChannel]
                                    - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
                  if( dDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                     break;
                  } // if out of coincidence window

                  // Check if tot edge is in trigger window, only needed in 24b mode
                  Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iMainTriggerIndex][uSecChannel]
                                    - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
                  if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                     break;
                  } // if out of coincidence window
               } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)

               // Check if tot edge of the main channel is in trigger window, only needed in 24b mode
               Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex]
                                 - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
               if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                     fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotMainDistanceToMain      )
               {
                  bAllInCoinc = kFALSE;
               } // if out of coincidence window

               if( kTRUE == bAllInCoinc  )
               {
                  Double_t dSameTriggerDistance =
                          ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD()
                        - ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] ;

                  ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iMainTriggerIndex );

                  if( fParam->dDeadTime <= dSameTriggerDistance )
                  {
                     // Here we have a DATA self-trigger!!!!!!
                     ROC[uRocId].fuDataSelfTriggerCount[iMainTriggerIndex] ++;

                     ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] =
                           ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();

                     ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTime();
                     uFullTimeSelfTrigBins /= 100000000L;
                     ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                     // Long duration time distribution (1 min. bin, 10 days length)
                     if( kTRUE == fParam->bLongTimeHistos )
                        ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                     ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex], iMainTriggerIndex);
                  } // if( fParam->dDeadTime <= dSameTriggerDistance )
               } // if( kTRUE == bAllInCoinc  )
            } // if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )

            for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
            {
               if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
               {
                  Int_t iSecondaryIndex = fParam->GetSecChannelIndex(iSecTriggerIndex, uGet4Id, uGet4Ch);
                  if( -1 < iSecondaryIndex )
                  {
                     ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][iSecondaryIndex] = extMess.GetFullTimeD();

                     Bool_t bAllInCoinc = kTRUE;
                     for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                     {
                        // Check if time edge is in trigger window
                        Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][uSecChannel]
                                          - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                        if( dDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                              fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dDistanceToMain      )
                        {
                           bAllInCoinc = kFALSE;
                           break;
                        } // if out of coincidence window

                        // Check if tot edge is in trigger window
                        Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][uSecChannel]
                                          - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                        if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                              fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotDistanceToMain      )
                        {
                           bAllInCoinc = kFALSE;
                           break;
                        } // if out of coincidence window
                     } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)

                     // Check if tot edge of the main channel is in trigger window
                     Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iSecTriggerIndex]
                                       - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                     if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotMainDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                     } // if out of coincidence window

                     if( kTRUE == bAllInCoinc  )
                     {
                        Double_t dSameTriggerDistance =
                                ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD()
                              - ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] ;

                        ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iSecTriggerIndex );

                        if( fParam->dDeadTime <= dSameTriggerDistance )
                        {
                           // Here we have a DATA self-trigger!!!!!!
                           ROC[uRocId].fuDataSelfTriggerCount[iSecTriggerIndex] ++;

                           ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] =
                                 ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();

                           ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTime();
                           uFullTimeSelfTrigBins /= 100000000L;
                           ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                           // Long duration time distribution (1 min. bin, 10 days length)
                           if( kTRUE == fParam->bLongTimeHistos )
                              ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                           ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex], iSecTriggerIndex);
                        } // if( fParam->dDeadTime <= dSameTriggerDistance )
                     } // if( kTRUE == bAllInCoinc  )
                  } // if( -1 < iSecondaryIndex )
               } // if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
            } // for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
         } // else if( 0 == uGet4Edge )
   } // if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers && 0 == uGet4Edge )

   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a GET4 External Synch
 */
Bool_t TTofGet4Unpacker::ProcessGet4ExtSyncMessage( UInt_t /*uRocId*/, TGet4v1MessageExtended& /*extMess*/ )
{
   /* TODO FILL ME
    * 24bit Get4 external sync message specific processing
    */
   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a GET4 epoch message
 * In suppressed empty epochs 2 mode
 */
Bool_t TTofGet4Unpacker::ProcessGet4SuppEpochMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * 24bit Get4 epoch message specific processing
    */

   UInt_t uGet4Id(0);

   // Remap the Get4 chip index => Go in Get4 indexing common to all ROCs
   uGet4Id = (extMess.GetRocMessage()).getEpoch2ChipNumber();
   uGet4Id = fParam->RemapGet4Chip( fParam->DefineGet4IndexOffset(uRocId) +  uGet4Id);
   extMess.SetEpoch2ChipNumber(uGet4Id);

   // Check if active chip
   if( kFALSE == fParam->IsValidGet4Chip(uGet4Id) )
   {
       cout << "Error: Bad chip nb in Epoch message = " << uGet4Id << endl;
       cout << " => This message will be skipped!!!! "<<endl;
       return kFALSE;
   }
   if( kFALSE == fParam->IsActiveGet4Chip(uGet4Id) )
   {
      // Masked chip, hopefully on purpose, don't complain & return
      return kFALSE;
   }

   UInt_t get4_24b_ep_epoch = (extMess.GetRocMessage()).getEpoch2Number();
   // No "non-consecutive epoch check" as in this mode they are by definition non consecutive

   // Event statistics
   ROC[uRocId].fuNbHitsChipEpoch[uGet4Id] = 0;

   if( ROC[uRocId].fuCurrEpoch2[uGet4Id] > get4_24b_ep_epoch )
   {
      TString sTemp = Form("Roc Epoch2 cycle (24b mode) change: %u", ROC[uRocId].fuEpoch2Cycle[uGet4Id]);
      LOG(info)<<sTemp;
      ROC[uRocId].fuEpoch2Cycle[uGet4Id]++;
   }
   ROC[uRocId].fuCurrEpoch2[uGet4Id] = get4_24b_ep_epoch;

   // Reprocess all data messages which where waiting for their epoch
   if( 1 == ROC[uRocId].fuEpochWithData[uGet4Id] )
   {
      ProcessGet4ReprocessSuppData( uRocId, uGet4Id );
      ROC[uRocId].fuEpochWithData[uGet4Id] = 0;
   }

   // Self triggering
   if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )
   {
      // A trigger is ongoing on this chip!
      if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
      {
         // This is the first epoch after the trigger
         // => time to look for the data in the epoch which was
         // ongoing when the trigger was found

         // If we did not process the epoch before the one where the
         // trigger came, we need to initialize the 24 bit temp hits
         if( kFALSE == ROC[uRocId].bLookInPreviousEpoch[uGet4Id] )
            for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                  (ROC[uRocId].fHitTemp24[uGet4Id][uChan]).Clear();

         BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
         if( kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id] )
         {
            ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
            CheckEventClosure();
         }
         ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] = kTRUE;
      } // if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] )
      else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] &&
                kTRUE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
      {
         // This is the second epoch after the trigger and we detected on trigger
         // that the trigger window extend in epoch after the one where trigger
         // was found => time to look for the data in the epoch which just finished
         // and was the one just after the trigger epoch
         BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
         ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
         CheckEventClosure();
      } // else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] && kFALSE == ROC[uRocId].bLookInNextEpoch[uGet4Id])
   } // if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )

   // Recalculate the full Time to take into account also the epoch2 cycles
   ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( uGet4Id )) << 19;
   ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
   Double_t  dFullTime = uFullTimeBins / 20. + 512.;
   extMess.SetFullTime(  uFullTime );
   extMess.SetFullTimeD( dFullTime );

   // Set The ROC epoch2 counter and cycle counter
   extMess.SetRocEpoch( ROC[uRocId].fuCurrEpoch2[uGet4Id] );
   extMess.SetRocCycle( ROC[uRocId].fuEpoch2Cycle[uGet4Id] );

   // Count per global time unit in s
   ULong64_t ulEpochTm = uFullTime / 100000000L;
   ROC[uRocId].fEPOCH2t[uGet4Id]->Fill((ulEpochTm % 36000) * 0.1);

   // Long duration time distribution (1 min. bin, 10 days length)
   if( kTRUE == fParam->bLongTimeHistos )
      ROC[uRocId].fAllEpoch2LongT->Fill( (ulEpochTm * 0.1 ) / 3600 );

   ROC[uRocId].fDistribEpochs2->Fill(uGet4Id, ROC[uRocId].fuCurrEpoch2[uGet4Id]);

   // Swap the flag indicating which buffer contains currently happening epoch
   ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] = ( kTRUE == ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] ? kFALSE: kTRUE );
   // Clear this buffer (Data more than 1 full epoch old => either already selected or junk)
   ROC[uRocId].fPrevEpochs2Buffer[uGet4Id][ ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id] ].clear();

   return kTRUE;
}
/**********************************************************************/
/*
 * This function process the data message when it is a GET4 data message
 */
Bool_t TTofGet4Unpacker::ProcessGet4SuppDataMessage( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * 24bit Get4 hit message specific processing
    * In case of suppressed epochs, we will have no epoch2 message for all empty epochs
    * However, when a chip has data, its next epoch message should be present!!!
    * => need a flag to use the proper epoch message when its the case
    *
    * TODO
    * Make the get4_index consistent everywhere between global get4 index and inside roc get4 index
    */

   if (ROC[uRocId].bIgnoreData) return kFALSE;

   if( kFALSE == ROC[uRocId].fb24bitsReadoutDetected )
   {
      ROC[uRocId].fb24bitsReadoutDetected = kTRUE;
      fb24bitsReadoutDetected = kTRUE;
      // As it is first 24bit message, let's create
      // 24bit specific histograms
      char folder[30];
      sprintf(folder,"ROC%u/",uRocId);

      /*
       * 24 bits specific histograms
       */
      ROC[uRocId].fChannelInputMessCount = new TH1I( Form("Roc%u_ChanCounts", uRocId),
            "Channel multiplicity per event; Channel []; # Mess []",
            2*get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc );
      ROC[uRocId].fb24bitsReadoutDetected = kTRUE;
   } // if( kFALSE == ROC[uRocId].fb24bitsReadoutDetected )

   UInt_t    uGet4Id(0); 
//   UInt_t    uGet4Ch(0); // -> Comment to remove warning because set but never used
//   UInt_t    uGet4Edge(0); // -> Comment to remove warning because set but never used
   uGet4Id   = (extMess.GetRocMessage()).getGet4Number();
//   uGet4Ch   = (extMess.GetRocMessage()).getGet4ChNum(); // -> Comment to remove warning because set but never used
//   uGet4Edge = (extMess.GetRocMessage()).getGet4Edge(); // -> Comment to remove warning because set but never used

   // Remap the Get4 chip index
   uGet4Id = (extMess.GetRocMessage()).getGet4Number();
   uGet4Id = fParam->RemapGet4Chip( fParam->DefineGet4IndexOffset(uRocId) + uGet4Id);
   extMess.SetGet4Number(uGet4Id);

   // Check if valid chip
   if( kFALSE == fParam->IsValidGet4Chip(uGet4Id) )
   {
       cout << "Error: Bad chip nb in Data message = " << uGet4Id << endl;
       cout << " => This message will be skipped!!!! "<<endl;
       return kFALSE;
   }
   if( kFALSE == fParam->IsActiveGet4Chip(uGet4Id) )
   {
      // Masked chip, hopefully on purpose, don't complain & return
      return kFALSE;
   }

   // => need a flag to use the proper epoch message when its the case
   ROC[uRocId].fuEpochWithData[uGet4Id] = 1;

   // Store data message in temporary buffer to wait for the next epoch message
   ROC[uRocId].fEpSuppBuffer[uGet4Id].push_back( extMess );

   return kTRUE;
}

/**********************************************************************/
/*
 * This function reprocess the data message from a GET4 in empty epoch suppression
 * mode once its next epoch message is found
 */
Bool_t TTofGet4Unpacker::ProcessGet4ReprocessSuppData( UInt_t uRocId, UInt_t uGet4Id )
{
   /*
    * 24bit Get4 hit message specific processing
    * In case of suppressed epochs, we will have no epoch2 message for all empty epochs
    * However, when a chip has data, its next epoch message should be present just after all data!!!
    * => there should not be more than 1 epoch waiting time before reprocessing
    *
    * TODO
    * Make the get4_index consistent everywhere between global get4 index and inside roc get4 index
    */
   // Loop over the temporary buffer
   for( UInt_t uDataIndex = 0; uDataIndex < ROC[uRocId].fEpSuppBuffer[uGet4Id].size(); uDataIndex++ )
   {
      TGet4v1MessageExtended extMess = ROC[uRocId].fEpSuppBuffer[uGet4Id][uDataIndex];
      UInt_t   uGet4Ch(0), uGet4Edge(0);
      uGet4Ch   = (extMess.GetRocMessage()).getGet4ChNum();
      uGet4Edge = (extMess.GetRocMessage()).getGet4Edge();

      // Recalculate the full Time to take into account also the epoch2 cycles
      ULong64_t uFullTimeBins = ((ULong64_t)  ( ROC[uRocId].GetFullEpoch2Number( uGet4Id ) - 1 ) << 19) |
                                 (ULong64_t)extMess.GetGet4Ts();
      ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
      Double_t  dFullTime = ((Double_t)uFullTimeBins)*0.05 + 512.;
      extMess.SetFullTime(  uFullTime );
      extMess.SetFullTimeD( dFullTime );

      // Set The ROC epoch2 counter and cycle counter
      extMess.SetRocEpoch( ROC[uRocId].fuCurrEpoch2[uGet4Id] );
      extMess.SetRocCycle( ROC[uRocId].fuEpoch2Cycle[uGet4Id] );


      // Count per global time unit in s
      ULong64_t ulDataTm = uFullTime / 100000000L;
      ROC[uRocId].fDATAt->Fill((ulDataTm % 36000) * 0.1);

      // Long duration time distribution (1 min. bin, 10 days length)
      if( kTRUE == fParam->bLongTimeHistos )
         ROC[uRocId].fDataLongT->Fill( (ulDataTm * 0.1 ) / 3600 );

      // Event statistics
      ROC[uRocId].fChannelInputMessCount->Fill( uGet4Id + (Double_t)uGet4Ch/(Double_t)get4v10::kuNbChan
                                                        + (Double_t)uGet4Edge/(Double_t)(2*get4v10::kuNbChan)  );
      if( kTRUE == fParam->bChipRateHistoEnable )
         ROC[uRocId].fChipRateEvolution[uGet4Id]->Fill(
               (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
      if( kTRUE == fParam->bChannelRateHistoEnable )
         ROC[uRocId].fChannelRateEvolution[uGet4Id][uGet4Ch]->Fill(
               (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
      ROC[uRocId].fuNbHitsChipEpoch[uGet4Id] ++;

      // Early event closure check
      if( kTRUE == fCurrentGet4Event.HasTrigger() &&
          kFALSE == ROC[uRocId].fbSelectionDone[uGet4Id]  )
      {
         Double_t dTimeToTrigger = 0.0;
         dTimeToTrigger = extMess.GetFullTimeD() - fCurrentGet4Event.fdTriggerFullTime[uRocId];
         if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
         {
            // Message after the end of trigger window!!
            Bool_t bAllChanInChipOver = kTRUE;

            // If there was already a message from the same channel & edge out of the trigger
            //  window, then the token ring made at least one full turn => ok for all
            // Otherwise: need to check other channels/edge status
            if( (kFALSE == ROC[uRocId].fbDataTimeOut[uGet4Id][uGet4Ch] && 0 == uGet4Edge ) ||
                (kFALSE == ROC[uRocId].fbDataFallingOut[uGet4Id][uGet4Ch] && 1 == uGet4Edge ) )
            {
               // 24b = always a edge not a hit => need to check both !
               if( 0 == uGet4Edge )
                  ROC[uRocId].fbDataTimeOut[uGet4Id][uGet4Ch] = kTRUE;
                  else ROC[uRocId].fbDataFallingOut[uGet4Id][uGet4Ch] = kTRUE;
               // Check if chip fully finished
               for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
               {
                  if( kFALSE ==  ROC[uRocId].fbDataTimeOut[uGet4Id][uChanTest] )
                     bAllChanInChipOver = kFALSE;
                  if( kFALSE ==  ROC[uRocId].fbDataFallingOut[uGet4Id][uChanTest] )
                     bAllChanInChipOver = kFALSE;
               }
            } // if same edge not already out of trigger once

            // If chip fully finished: don't wait for the next epoch to close future buffer
            // of current event for this chip
            if( kTRUE == bAllChanInChipOver)
            {
               // If This is the first epoch after the trigger and
               // we did not process the epoch before the one where the
               // trigger came, we need to initialize the 24 bit temp hits
               if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[uGet4Id] &&
                   kFALSE == ROC[uRocId].bLookInPreviousEpoch[uGet4Id] )
                  for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                        (ROC[uRocId].fHitTemp24[uGet4Id][uChan]).Clear();

               BuildHits(uRocId, uGet4Id, ROC[uRocId].fbBufferWithLastFullEpoch2[uGet4Id]);
               ROC[uRocId].fbSelectionDone[uGet4Id] = kTRUE;
               CheckEventClosure();

               for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
               {
                  ROC[uRocId].fbDataTimeOut[uGet4Id][uChanTest] = kFALSE;
                  ROC[uRocId].fbDataFallingOut[uGet4Id][uChanTest] = kFALSE;
               }
            } // if( kTRUE == bAllChanInChipOver)
         } // if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
      } // Event ongoing with a detected trigger and chip did not finished all its buffers

      /** Triggering on Data **/
      if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
      {
         /** Rising edge: Triggering on Data **/
         if( 0 == uGet4Edge )
         {
            for( Int_t iMainTriggerIndex = 0; iMainTriggerIndex < (Int_t)(fParam->uNbTriggers); iMainTriggerIndex++ )
               if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )
            {
               ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex] = extMess;
               /*
                * TODO: call to ProcessTrigger
                * TEMP: histo Filling + counting
                */
               Bool_t bAllInCoinc = kTRUE;
               for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
               {
                  Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iMainTriggerIndex][uSecChannel]
                                           - extMess.GetFullTimeD();
                  if( dDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                     break;
                  } // if out of coincidence window
                  Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iMainTriggerIndex][uSecChannel]
                                           - extMess.GetFullTimeD();
                  if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                     break;
                  } // if out of coincidence window
               } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)

               // Check if tot edge of the main channel is in trigger window
               Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex]
                                 - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
               if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                     fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotMainDistanceToMain      )
               {
                  bAllInCoinc = kFALSE;
               } // if out of coincidence window

               if( kTRUE == bAllInCoinc )
               {
                  Double_t dSameTriggerDistance =
                          ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD()
                        - ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] ;

                  ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iMainTriggerIndex );

                  if( fParam->dDeadTime <= dSameTriggerDistance )
                  {
                     // Here we have a DATA self-trigger!!!!!!
                     ROC[uRocId].fuDataSelfTriggerCount[iMainTriggerIndex] ++;

                     ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] =
                           ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();

                     ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTime();
                     uFullTimeSelfTrigBins /= 100000000L;
                     ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                     // Long duration time distribution (1 min. bin, 10 days length)
                     if( kTRUE == fParam->bLongTimeHistos )
                        ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                     ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex], iMainTriggerIndex);
                  } // if( fParam->dDeadTime <= dSameTriggerDistance )
               } // if( kTRUE == bAllInCoinc )
            } // if( -1 < iMainTriggerIndex )

            for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
            {
               if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
               {
                  Int_t iSecondaryIndex = fParam->GetSecChannelIndex(iSecTriggerIndex, uGet4Id, uGet4Ch);
                  if( -1 < iSecondaryIndex )
                  {
                     ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][iSecondaryIndex] = extMess.GetFullTimeD();

                     /*
                      * TODO: call to ProcessTrigger
                      * TEMP: histo Filling + counting
                      */
                     Bool_t bAllInCoinc = kTRUE;
                     for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                     {
                        Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][uSecChannel]
                                          - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                        if( dDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                              fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dDistanceToMain      )
                        {
                           bAllInCoinc = kFALSE;
                           break;
                        } // if out of coincidence window

                        // Check if tot edge is in trigger window, only needed in 24b mode
                        Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][uSecChannel]
                                          - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                        if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                              fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotDistanceToMain      )
                        {
                           bAllInCoinc = kFALSE;
                           break;
                        } // if out of coincidence window
                     } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)

                     // Check if tot edge of the main channel is in trigger window, only needed in 24b mode
                     Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iSecTriggerIndex]
                                       - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                     if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotMainDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                     } // if out of coincidence window

                     if( kTRUE == bAllInCoinc  )
                     {
                        Double_t dSameTriggerDistance =
                                ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD()
                              - ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] ;

                        ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iSecTriggerIndex );
                        ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iSecTriggerIndex );

                        if( fParam->dDeadTime <= dSameTriggerDistance )
                        {
                           // Here we have a DATA self-trigger!!!!!!
                           ROC[uRocId].fuDataSelfTriggerCount[iSecTriggerIndex] ++;

                           ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] =
                                 ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();

                           ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTime();
                           uFullTimeSelfTrigBins /= 100000000L;
                           ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                           // Long duration time distribution (1 min. bin, 10 days length)
                           if( kTRUE == fParam->bLongTimeHistos )
                              ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                           ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex], iSecTriggerIndex);
                        } // if( fParam->dDeadTime <= dSameTriggerDistance )
                     } // if( kTRUE == bAllInCoinc  )
                  } // if( -1 < iSecondaryIndex )
               } //  if( -1 < iSecTriggerIndex )
            } // for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
         }// if( 0 == uGet4Edge )
            /** Falling edge: Triggering on Data, only needed in 24b mode **/
            else
            {
               // Falling edge
               for( Int_t iMainTriggerIndex = 0; iMainTriggerIndex < (Int_t)(fParam->uNbTriggers); iMainTriggerIndex++ )
                  if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )
               {
                  ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex] = extMess.GetFullTimeD();

                  Bool_t bAllInCoinc = kTRUE;
                  for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
                  {
                     // Check if time edge is in trigger window
                     Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iMainTriggerIndex][uSecChannel]
                                       - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
                     if( dDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                        break;
                     } // if out of coincidence window

                     // Check if tot edge is in trigger window, only needed in 24b mode
                     Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iMainTriggerIndex][uSecChannel]
                                       - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
                     if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                           fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotDistanceToMain      )
                     {
                        bAllInCoinc = kFALSE;
                        break;
                     } // if out of coincidence window
                  } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)

                  // Check if tot edge of the main channel is in trigger window, only needed in 24b mode
                  Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iMainTriggerIndex]
                                    - ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();
                  if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dTotMainDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                  } // if out of coincidence window

                  if( kTRUE == bAllInCoinc  )
                  {
                     Double_t dSameTriggerDistance =
                             ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD()
                           - ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] ;

                     ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iMainTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iMainTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iMainTriggerIndex );
                     ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iMainTriggerIndex );

                     if( fParam->dDeadTime <= dSameTriggerDistance )
                     {
                        // Here we have a DATA self-trigger!!!!!!
                        ROC[uRocId].fuDataSelfTriggerCount[iMainTriggerIndex] ++;

                        ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] =
                              ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();

                        ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTime();
                        uFullTimeSelfTrigBins /= 100000000L;
                        ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                        // Long duration time distribution (1 min. bin, 10 days length)
                        if( kTRUE == fParam->bLongTimeHistos )
                           ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                        ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex], iMainTriggerIndex);
                     } // if( fParam->dDeadTime <= dSameTriggerDistance )
                  } // if( kTRUE == bAllInCoinc  )
               } // if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, uGet4Id, uGet4Ch ) )

               for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
               {
                  if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
                  {
                     Int_t iSecondaryIndex = fParam->GetSecChannelIndex(iSecTriggerIndex, uGet4Id, uGet4Ch);
                     if( -1 < iSecondaryIndex )
                     {
                        ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][iSecondaryIndex] = extMess.GetFullTimeD();

                        Bool_t bAllInCoinc = kTRUE;
                        for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                        {
                           // Check if time edge is in trigger window
                           Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][uSecChannel]
                                             - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                           if( dDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                                 fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dDistanceToMain      )
                           {
                              bAllInCoinc = kFALSE;
                              break;
                           } // if out of coincidence window

                           // Check if tot edge is in trigger window
                           Double_t dTotDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannelTot[iSecTriggerIndex][uSecChannel]
                                             - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                           if( dTotDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                                 fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotDistanceToMain      )
                           {
                              bAllInCoinc = kFALSE;
                              break;
                           } // if out of coincidence window
                        } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)

                        // Check if tot edge of the main channel is in trigger window
                        Double_t dTotMainDistanceToMain = ROC[uRocId].fdLastFullTimeMainChannelTot[iSecTriggerIndex]
                                          - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                        if( dTotMainDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                              fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dTotMainDistanceToMain      )
                        {
                           bAllInCoinc = kFALSE;
                        } // if out of coincidence window

                        if( kTRUE == bAllInCoinc  )
                        {
                           Double_t dSameTriggerDistance =
                                   ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD()
                                 - ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] ;

                           ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iSecTriggerIndex );

                           if( fParam->dDeadTime <= dSameTriggerDistance )
                           {
                              // Here we have a DATA self-trigger!!!!!!
                              ROC[uRocId].fuDataSelfTriggerCount[iSecTriggerIndex] ++;

                              ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] =
                                    ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();

                              ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTime();
                              uFullTimeSelfTrigBins /= 100000000L;
                              ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                              // Long duration time distribution (1 min. bin, 10 days length)
                              if( kTRUE == fParam->bLongTimeHistos )
                                 ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                              ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex], iSecTriggerIndex);
                           } // if( fParam->dDeadTime <= dSameTriggerDistance )
                        } // if( kTRUE == bAllInCoinc  )
                     } // if( -1 < iSecondaryIndex )
                  } // if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, uGet4Id, uGet4Ch ) )
               } // for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
            } // else if( 0 == uGet4Edge )
      } // if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers && 0 == uGet4Edge )

      ProcessExtendedMessage(uRocId, extMess);
   } // for( UInt_t uDataIndex = 0; uDataIndex < ROC[uRocId].fEpSuppBuffer[uGet4Id].size(); uDataIndex++ )
   ROC[uRocId].fEpSuppBuffer[uGet4Id].clear();

   return kTRUE;
}
/**********************************************************************/
Int_t TTofGet4Unpacker::Process32BitGet4Message( UInt_t uRocId, TGet4v1MessageExtended& extMess )
{
   /*
    * 32bit Get4 hit message specific processing
    * TODO FILL ME
    * Make the get4_index consistent everywhere between global get4 index and inside roc get4 index
    */

   UInt_t get4_index = extMess.getGet4V10R32ChipId() ;
   get4_index = fParam->RemapGet4Chip(get4_index);

   // Check if valid chip
   if( kFALSE == fParam->IsValidGet4Chip(get4_index) )
   {
       cout << "Error: Bad chip nb in 32b Data message = " << get4_index <<" < "<<fParam->uNbGet4<< endl;
       cout << " => This message will be skipped!!!! "<<endl;
       return -1;
   }
   if( kFALSE == fParam->IsActiveGet4Chip(get4_index) )
   {
      // Masked chip, hopefully on purpose, don't complain & return
      return -1;
   }

   // If 1 message is 32bits, all are!
   if( kFALSE == ROC[uRocId].fb32bitsReadoutDetected )
   {
      ROC[uRocId].fb32bitsReadoutDetected = kTRUE;
      fb32bitsReadoutDetected = kTRUE;
      // As it is first 32bit message, let's create
      // 32bit specific histograms
      char folder[30];
      sprintf(folder,"ROC%u/",uRocId);

      /*
       * 32 bits specific histograms
       */
      ROC[uRocId].fChannelInputMessCount = new TH1I( Form("Roc%u_ChanCounts", uRocId),
            "Channel multiplicity per event; Channel []; # Mess []",
            get4v10::kuNbChan*get4v10::kuMaxGet4Roc, 0, get4v10::kuMaxGet4Roc );

         // Slow control messages
      ROC[uRocId].fGet4V1SlowControlType = new TH2I( Form("Roc%u_Get4SlowControl", uRocId),
            "SlowControl message type per Get4; Chip; ; Entries [1]",
            fParam->uNbGet4, -0.5, fParam->uNbGet4 -0.5, 5, 0., 5);
      ROC[uRocId].fGet4V1SlowControlType->GetYaxis()->SetBinLabel(1, "Scaler");
      ROC[uRocId].fGet4V1SlowControlType->GetYaxis()->SetBinLabel(2, "Dead time");
      ROC[uRocId].fGet4V1SlowControlType->GetYaxis()->SetBinLabel(3, "SPI");
      ROC[uRocId].fGet4V1SlowControlType->GetYaxis()->SetBinLabel(4, "Start");
      ROC[uRocId].fGet4V1SlowControlType->GetYaxis()->SetBinLabel(5, "Hamming");
      ROC[uRocId].fGet4V1SlowControlScaler = new TH2I( Form("Roc%u_Get4SlScaler", uRocId),
                              "Scaler in Get4 slow control message; Channel; Scaler [hits]; Entries [1]",
                              2*(fParam->uNbGet4)*get4v10::kuNbChan, 0., (fParam->uNbGet4)*get4v10::kuNbChan, 820, 0, 8200);
      ROC[uRocId].fGet4V1SlowControlDeadTime = new TH2I( Form("Roc%u_Get4SlDeadTime", uRocId),
                              "Dead time in Get4 slow control message; Channel; Dead Time [Clk cycle]; Entries [1]",
                              2*(fParam->uNbGet4)*get4v10::kuNbChan, 0., (fParam->uNbGet4)*get4v10::kuNbChan, 410, 0, 4100);
      ROC[uRocId].fGet4V1SlowControlHamming = new TH2I( Form("Roc%u_Get4SlSeu", uRocId),
            "Hamming error counter for SEU in Get4 slow control message; Chip; Scaler [hits]; Entries [1]",
            fParam->uNbGet4, 0., fParam->uNbGet4, 512, 0, 512);
      ROC[uRocId].fGet4V1SlowControlSeuEvo = new TH2I( Form("Roc%u_Get4SlSeuEvo", uRocId),
            "Hamming error counter for SEU in Get4 slow control message; Time [s]; Chip; Scaler [hits]",
            3600, 0, 3600, fParam->uNbGet4, 0., fParam->uNbGet4);
         // Data messages
      ROC[uRocId].fGet4V1DllLockBit =
            new TH2I( Form("Roc%u_Get4V1DllLockBit", uRocId),
            "Dll lock bit status per channel; Channel []; Dll lock bit []; Counts [1]",
            fParam->uNbGet4*get4v10::kuNbChan, -0.5 , fParam->uNbGet4*get4v10::kuNbChan -0.5,
            2, -0.5, 1.5 );

      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      {
         char folderGet4[30];
         sprintf(folderGet4,"%sGet4_%u/",folder, uGet4);
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRocId, uGet4);
         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
         {
            ROC[uRocId].fGet4V1HitsDistanceNs[uGet4] =
                  new TH2I( Form("Roc%u_Get%u_HitDistNs",
                                 uRocId, uGet4),
                           Form("Time difference since last hit on same channel in GET4 %u Channels on ROC%u; [ns]",
                                 uGet4, uRocId),
                           1002, -2., 1000.,
                           get4v10::kuNbChan, 0, get4v10::kuNbChan );
            ROC[uRocId].fGet4V1HitsDistanceUs[uGet4] =
                  new TH2I( Form("Roc%u_Get%u_HitDistUs",
                                 uRocId, uGet4),
                           Form("Time difference since last hit on same channel in GET4 %u Channels on ROC%u; [us]",
                                 uGet4, uRocId),
                           999, 1., 1000.,
                           get4v10::kuNbChan, 0, get4v10::kuNbChan );
            ROC[uRocId].fGet4V1HitsDistanceMs[uGet4] =
                  new TH2I( Form("Roc%u_Get%u_HitDistMs",
                                 uRocId, uGet4),
                           Form("Time difference since last hit on same channel in GET4 %u Channels on ROC%u; [ms]",
                                 uGet4, uRocId),
                           999, 1., 1000.,
                           get4v10::kuNbChan, 0, get4v10::kuNbChan );
            ROC[uRocId].fGet4V1HitsDistanceS[uGet4]  =
                  new TH2I( Form("Roc%u_Get%u_HitDistS",
                                 uRocId, uGet4),
                           Form("Time difference since last hit on same channel in GET4 %u Channels on ROC%u; [s]",
                                 uGet4, uRocId),
                           999, 1., 1000.,
                           get4v10::kuNbChan, 0, get4v10::kuNbChan );

            if( 1 == fParam->bTotHistoEnable )
               for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
                  ROC[uRocId].fRawTot[uGet4][uGet4Chan] =
                        new TH1I( Form("RawTot_Ch%u_Get%u_Roc%u",
                              uGet4Chan, uGet4, uRocId ),
                        Form("Tot for channel %u in chip %u on ROC %u; Tot [ps]; Counts [1]", uGet4Chan, uGet4, uRocId),
                        1200, -25, 59975 );
         }
      } // for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
   } // if( kFALSE == fb32bitsReadoutDetected )
   extMess.Set32BitFlag();

   UInt_t get4_32b_type  = extMess.getGet4V10R32MessageType();
   switch(get4_32b_type)
   {
      case 0: // epoch message
      {
         UInt_t get4_32b_ep_epoch = extMess.getGet4V10R32EpochNumber();
/*         UInt_t get4_32b_ep_sync  = extMess.getGet4V10R32SyncFlag();*/ // Commented out as unused to remove warning

         if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[get4_index]  )
         {
            // A trigger is ongoing on this chip!
            if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[get4_index] )
            {
               // This is the first epoch after the trigger
               // => time to look for the data in the epoch which was
               // ongoing when the trigger was found

               // If we did not process the epoch before the one where the
               // trigger came, we need to initialize the 24 bit temp hits
               if( kFALSE == ROC[uRocId].bLookInPreviousEpoch[get4_index] )
                  for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                        (ROC[uRocId].fHitTemp24[get4_index][uChan]).Clear();

               BuildHits(uRocId, get4_index, ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index]);
               if( kFALSE == ROC[uRocId].bLookInNextEpoch[get4_index] )
               {
                  ROC[uRocId].fbSelectionDone[get4_index] = kTRUE;
                  CheckEventClosure();
               }
            } // if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[get4_index] )
            else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[get4_index] &&
                      kTRUE == ROC[uRocId].bLookInNextEpoch[get4_index])
            {
               // This is the second epoch after the trigger and we detected on trigger
               // that the trigger window extend in epoch after the one where trigger
               // was found => time to look for the data in the epoch which just finished
               // and was the one just after the trigger epoch
               BuildHits(uRocId, get4_index, ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index]);
               ROC[uRocId].fbSelectionDone[get4_index] = kTRUE;
               CheckEventClosure();
            } // else if( kTRUE == ROC[uRocId].fbEpoch2SinceTrigger[get4_index] && kFALSE == ROC[uRocId].bLookInNextEpoch[get4_index])
         } // if( kTRUE == fCurrentGet4Event.HasTrigger() && kFALSE == ROC[uRocId].fbSelectionDone[get4_index]  )

         if( ROC[uRocId].fuCurrEpoch2[get4_index] + 1 != get4_32b_ep_epoch &&
               ( 0 < ROC[uRocId].fuCurrEpoch2[get4_index] || 0 < ROC[uRocId].fuEpoch2Cycle[get4_index] ) &&
               !( get4v10::kulGet4EpochCycleSz == ROC[uRocId].fuCurrEpoch2[get4_index]  && 0 == get4_32b_ep_epoch ) )
         {

            // Count per global time unit in s for previous epoch as should not be too far away
            // anyway this is just for selection, not actual display
            ULong64_t ulPrevEpochTm = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( get4_index )) << 19;
            ulPrevEpochTm = ulPrevEpochTm/ 20 + 512;
            ulPrevEpochTm = ulPrevEpochTm / 100000000L;

            if( 311 < (ulPrevEpochTm % 36000) * 0.1 &&
              (ulPrevEpochTm % 36000) * 0.1 < 312 )
            {
               cout<<(ulPrevEpochTm % 36000) * 0.1<<" Non consecutive epoch message for chip "<<get4_index<<" Roc Epoch "<<ROC[uRocId].fuCurrEpoch<<" : ";
               cout<<ROC[uRocId].fuCurrEpoch2[get4_index]<<" "<<get4_32b_ep_epoch<<" ";
               cout<<(Int_t)get4_32b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[get4_index]+1) <<" ";
               cout<<ROC[uRocId].fiEpochShift[get4_index]<<" ";
               cout<<ROC[uRocId].fuNbShiftedEpochs[get4_index]<<endl;
            }

            ROC[uRocId].fEpochShiftsPerChip->Fill( get4_index, (Int_t)get4_32b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[get4_index]+1) );

            if( ROC[uRocId].fuCurrEpoch2[get4_index] > get4_32b_ep_epoch &&
                get4v10::kulGet4EpochCycleSz < ROC[uRocId].fuCurrEpoch2[get4_index] + 100ul &&
                100 < get4_32b_ep_epoch )
               // Epoch close to a cycle change on both sides
               ROC[uRocId].fuEpoch2Cycle[get4_index]++;

            // Correction attempt:
            if( (Int_t)get4_32b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[get4_index]+1) < 10 )
            {
               // close epochs ( < 250us) => probable not a full event missing
               if( 0 != ROC[uRocId].fiEpochShift[get4_index])
               {
                  ROC[uRocId].fEpochShiftsDuration[get4_index]->Fill(ROC[uRocId].fiEpochShift[get4_index],
                        ROC[uRocId].fuNbShiftedEpochs[get4_index] );
                  ROC[uRocId].fEpochShiftsDurationPerChip->Fill( get4_index,
                        ROC[uRocId].fuNbShiftedEpochs[get4_index] );
               }
               ROC[uRocId].fiEpochShift[get4_index] += (Int_t)get4_32b_ep_epoch - (Int_t)(ROC[uRocId].fuCurrEpoch2[get4_index]+1) ;
               ROC[uRocId].fuNbShiftedEpochs[get4_index] = 0;
            }
               else
               {
                  // far epochs ( > 250us) => probable a full event is missing
                  if( 0 != ROC[uRocId].fiEpochShift[get4_index])
                  {
                     ROC[uRocId].fEpochShiftsDuration[get4_index]->Fill(ROC[uRocId].fiEpochShift[get4_index],
                           ROC[uRocId].fuNbShiftedEpochs[get4_index] );
                     ROC[uRocId].fEpochShiftsDurationPerChip->Fill( get4_index,
                           ROC[uRocId].fuNbShiftedEpochs[get4_index] );
                  }
                  ROC[uRocId].fiEpochShift[get4_index]      = 0 ;
                  ROC[uRocId].fuNbShiftedEpochs[get4_index] = 0;
               }

            ROC[uRocId].fuCurrEpoch2[get4_index] = get4_32b_ep_epoch;
         } // if non consecutive epoch indexes
            else
            {
               if( ROC[uRocId].fuCurrEpoch2[get4_index] > get4_32b_ep_epoch )
                  ROC[uRocId].fuEpoch2Cycle[get4_index]++;
               ROC[uRocId].fuCurrEpoch2[get4_index] = get4_32b_ep_epoch;

               if( 0 != ROC[uRocId].fiEpochShift[get4_index] )
                  ROC[uRocId].fuNbShiftedEpochs[get4_index]++;
            } // else of if non consecutive epoch indexes

         // Event statistics
         ROC[uRocId].fuNbHitsChipEpoch[get4_index] = 0;

         /*
          * Temp fix for nov 2012 epoch problem
          */
/*
         if( 0 == get4_index)
         {
            if( ROC[uRocId].fuCurrEpoch2[get4_index] > get4_32b_ep_epoch )
            {
               ROC[uRocId].fuEpoch2Cycle[1]++;
               ROC[uRocId].fuEpoch2Cycle[5]++;
            }
            ROC[uRocId].fuCurrEpoch2[1] = get4_32b_ep_epoch;
            ROC[uRocId].fuCurrEpoch2[5] = get4_32b_ep_epoch;
         }
         if( 1 != get4_index && 5 != get4_index)
         {
            if( ROC[uRocId].fuCurrEpoch2[get4_index] > get4_32b_ep_epoch )
               ROC[uRocId].fuEpoch2Cycle[get4_index]++;
            ROC[uRocId].fuCurrEpoch2[get4_index] = get4_32b_ep_epoch;
         }
*/

         ROC[uRocId].fDistribEpochs2->Fill(get4_index, ROC[uRocId].fuCurrEpoch2[get4_index]);

         ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( get4_index )) << 19;
         ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
         Double_t  dFullTime = uFullTimeBins / 20. + 512.;
         extMess.SetFullTime(  uFullTime );
         extMess.SetFullTimeD( dFullTime );

         // Count per global time unit in s
         ULong64_t ulEpochTm = uFullTime / 100000000L;
         ROC[uRocId].fEPOCH2t[get4_index]->Fill((ulEpochTm % 36000) * 0.1);

         // Long duration time distribution (1 min. bin, 10 days length)
         if( kTRUE == fParam->bLongTimeHistos )
            ROC[uRocId].fAllEpoch2LongT->Fill( (ulEpochTm * 0.1 ) / 3600 );

         // Swap the flag indicating which buffer contains currently happening epoch
         ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index] = ( kTRUE == ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index] ? kFALSE: kTRUE );
         // Clear this buffer (Data more than 1 full epoch old => either already selected or junk)
         ROC[uRocId].fPrevEpochs2Buffer[get4_index][ ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index] ].clear();

         break;
      }
      case 1: // slow control message
      {
         UInt_t get4_32b_sl_data = extMess.getGet4V10R32SlData();
         UInt_t get4_32b_sl_type = extMess.getGet4V10R32SlType();
         UInt_t get4_32b_sl_edge = extMess.getGet4V10R32SlEdge();
         UInt_t get4_32b_sl_chan = extMess.getGet4V10R32SlChan();

         if( get4_32b_sl_type < 3)
            ROC[uRocId].fGet4V1SlowControlType->Fill(get4_index, get4_32b_sl_type);

         ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( get4_index )) << 19;
         ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
         Double_t  dFullTime = uFullTimeBins / 20. + 512.;
         extMess.SetFullTime(  uFullTime );
         extMess.SetFullTimeD( dFullTime );

         uFullTimeBins /= 20;
         uFullTimeBins += 512;
         uFullTimeBins /= 100000000L;
         ROC[uRocId].fSLOWCt->Fill( (uFullTimeBins % 36000) * 0.1 );

         // Long duration time distribution (1 min. bin, 10 days length)
         if( kTRUE == fParam->bLongTimeHistos )
            ROC[uRocId].fSlowCLongT->Fill( (uFullTimeBins * 0.1 ) / 3600 );

         switch(get4_32b_sl_type)
         {
            case 0:
               ROC[uRocId].fGet4V1SlowControlScaler->Fill(
                     get4v10::kuNbChan*get4_index + get4_32b_sl_chan + 0.5*get4_32b_sl_edge, get4_32b_sl_data );
               break;
            case 1:
               ROC[uRocId].fGet4V1SlowControlDeadTime->Fill(
                     get4v10::kuNbChan*get4_index + get4_32b_sl_chan + 0.5*get4_32b_sl_edge, get4_32b_sl_data );
               break;
            case 2:
            {
               TString sTemp = Form("Get4 V1.0 32bit, chip %02d, SPI message: %06x", get4_index, get4_32b_sl_data);
               LOG(info)<<sTemp;
               break;
            }
            case 3:
            {
               if( 0 == get4_32b_sl_chan && 1 == get4_32b_sl_edge )
               {
                  ROC[uRocId].fGet4V1SlowControlType->Fill(get4_index, 4);
                  ROC[uRocId].fGet4V1SlowControlHamming->Fill( get4_index, get4_32b_sl_data );
                  ROC[uRocId].fGet4V1SlowControlSeuEvo->Fill(
                                ( ROC[uRocId].GetFullEpoch2Number(get4_index) * get4tdc::kdEpochInPs ) / 1e12,
                                get4_index, get4_32b_sl_data );
               }
                  else
                  {
                     ROC[uRocId].fGet4V1SlowControlType->Fill(get4_index, get4_32b_sl_type);
                     TString sTemp = Form("Get4 V1.0 32bit, chip %02d, Start message: %c%c%c (%6X)",
                           get4_index,
                           (get4_32b_sl_data>>16)&0xFF, (get4_32b_sl_data>>8)&0xFF,
                           get4_32b_sl_data&0xFF, get4_32b_sl_data);
                     LOG(info)<<sTemp;
                  }

               break;
            }
            default:
               break;
         }

         break;
      }
      case 2: // error event
      {
         UInt_t get4_32b_er_code = extMess.getGet4V10R32ErrorData();
         UInt_t get4_32b_er_chan = extMess.getGet4V10R32ErrorChan();
         UInt_t get4_32b_er_edge = extMess.getGet4V10R32ErrorEdge();

         ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( get4_index )) << 19;
         ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
         Double_t  dFullTime = uFullTimeBins / 20. + 512.;
         extMess.SetFullTime(  uFullTime );
         extMess.SetFullTimeD( dFullTime );

         uFullTimeBins /= 20;
         uFullTimeBins += 512;
         uFullTimeBins /= 100000000L;
         ROC[uRocId].fERRORt->Fill( (uFullTimeBins % 36000) * 0.1 );

         // Long duration time distribution (1 min. bin, 10 days length)
         if( kTRUE == fParam->bLongTimeHistos )
            ROC[uRocId].fErrorLongT->Fill( (uFullTimeBins * 0.1 ) / 3600 );

         ROC[uRocId].fGet4ErrorChip->Fill( get4_index );
         ROC[uRocId].fGet4ErrorPatt->Fill( get4_index, get4_32b_er_code);
         if( (0x03 < get4_32b_er_code && get4_32b_er_code < 0x07) )
            ROC[uRocId].fGet4ErrorChan->Fill(
                  get4v10::kuNbChan*get4_index + get4_32b_er_chan + 0.5*get4_32b_er_edge,
                  get4_32b_er_code - 0x04 );
         else if( 0x11 == get4_32b_er_code ||
                   0x12 == get4_32b_er_code )
            ROC[uRocId].fGet4ErrorChan->Fill(
                  get4v10::kuNbChan*get4_index + get4_32b_er_chan + 0.5*get4_32b_er_edge,
                  get4_32b_er_code - 0x0e );

         break;
      }
      case 3: // data event
      {
         UInt_t get4_32b_dat_tot  = extMess.getGet4V10R32HitTot();
/*         UInt_t get4_32b_dat_ft   = extMess.getGet4V10R32HitFt();*/ // Commented out as unused to remove warning
/*         UInt_t get4_32b_dat_ts   = extMess.getGet4V10R32HitTs();*/ // Commented out as unused to remove warning
         UInt_t get4_32b_dat_chan = extMess.getGet4V10R32HitChan();
         UInt_t get4_32b_dat_dll  = extMess.getGet4V10R32HitDllFlag();
         if( get4v10::kuNbChan <= get4_32b_dat_chan )
            return -1;

         ULong64_t uFullTimeBins = ((ULong64_t)  ROC[uRocId].GetFullEpoch2Number( get4_index ) << 19) |
                                    (ULong64_t)extMess.getGet4V10R32HitTimeBin();
         ULong64_t uFullTime = uFullTimeBins/ 20 + 512;
         Double_t  dFullTime = ((Double_t)uFullTimeBins)*0.05 + 512.;
         extMess.SetFullTime(  uFullTime );
         extMess.SetFullTimeD( dFullTime );

         uFullTimeBins /= 20;
         uFullTimeBins += 512;
         uFullTimeBins /= 100000000L;
         ROC[uRocId].fDATAt->Fill( (uFullTimeBins % 36000) * 0.1 );

         // Long duration time distribution (1 min. bin, 10 days length)
         if( kTRUE == fParam->bLongTimeHistos )
            ROC[uRocId].fDataLongT->Fill( (uFullTimeBins * 0.1 ) / 3600 );

         if( 0 < ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan].GetFullTimeD() )
         {
            ROC[uRocId].fGet4V1HitsDistanceNs[get4_index]->Fill( extMess.GetFullTimeD()
                  - ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan].GetFullTimeD(),
                  get4_32b_dat_chan);
            ROC[uRocId].fGet4V1HitsDistanceUs[get4_index]->Fill( ( extMess.GetFullTimeD()
                  - ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan].GetFullTimeD() )/1e3,
                  get4_32b_dat_chan);
            ROC[uRocId].fGet4V1HitsDistanceMs[get4_index]->Fill( ( extMess.GetFullTimeD()
                  - ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan].GetFullTimeD() )/1e6,
                  get4_32b_dat_chan);
            ROC[uRocId].fGet4V1HitsDistanceS[get4_index]->Fill( ( extMess.GetFullTimeD()
                  - ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan].GetFullTimeD() )/1e9,
                  get4_32b_dat_chan);
         }

         // Event statistics
         ROC[uRocId].fLastExtMess[get4_index][get4_32b_dat_chan] = extMess;
         ROC[uRocId].fChannelInputMessCount->Fill( get4_index + (Double_t)get4_32b_dat_chan/(Double_t)get4v10::kuNbChan  );
         if( kTRUE == fParam->bChipRateHistoEnable )
            ROC[uRocId].fChipRateEvolution[get4_index]->Fill(
                  (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
         if( kTRUE == fParam->bChannelRateHistoEnable )
            ROC[uRocId].fChannelRateEvolution[get4_index][get4_32b_dat_chan]->Fill(
                  (uFullTimeBins ) * 0.1, 1.0/ROC[uRocId].fdRateEvolutionBinSize );
         ROC[uRocId].fuNbHitsChipEpoch[get4_index] ++;

         // TODO: bin size for Tot as option!!!
         if( 1 == fParam->bTotHistoEnable )
            ROC[uRocId].fRawTot[get4_index][get4_32b_dat_chan]->Fill( get4_32b_dat_tot*25500.0/255.0 );

         ROC[uRocId].fGet4V1DllLockBit->Fill( get4v10::kuNbChan*get4_index + get4_32b_dat_chan,
                                  get4_32b_dat_dll);
         if( kTRUE == ROC[uRocId].fbDllFlag[get4_index][get4_32b_dat_chan] && 1 == get4_32b_dat_dll)
         {
            ROC[uRocId].fbDllFlag[get4_index][get4_32b_dat_chan] = kFALSE;
            cout<<"DLL lock recovery, chip "<<get4_index<<" chan "<<get4_32b_dat_chan<<" at "<<(Double_t)uFullTimeBins*0.1<<" s"<<endl;
         }
         if( kFALSE == ROC[uRocId].fbDllFlag[get4_index][get4_32b_dat_chan] && 0 == get4_32b_dat_dll)
         {
            ROC[uRocId].fbDllFlag[get4_index][get4_32b_dat_chan] = kTRUE;
            cout<<"DLL lock loss, chip "<<get4_index<<" chan "<<get4_32b_dat_chan<<" at "<<(Double_t)uFullTimeBins*0.1<<" s"<<endl;
         }

         // Early event closure check
         if( kTRUE == fCurrentGet4Event.HasTrigger() &&
             kFALSE == ROC[uRocId].fbSelectionDone[get4_index]  )
         {
            Double_t dTimeToTrigger = 0.0;
            dTimeToTrigger = extMess.GetFullTimeD() - fCurrentGet4Event.fdTriggerFullTime[uRocId];
            if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
            {
               // Message after the end of trigger window!!
               Bool_t bAllChanInChipOver = kTRUE;

               // If there was already a message from the same channel out of the trigger
               //  window, then the token ring made at least one full turn => ok for all
               // Otherwise: need to check other channels status
               if( kFALSE == ROC[uRocId].fbDataTimeOut[get4_index][get4_32b_dat_chan] )
               {
                  // 32b = always a hit, not just edge !
                  ROC[uRocId].fbDataTimeOut[get4_index][get4_32b_dat_chan] = kTRUE;
                  // Check if chip fully finished
                  for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
                     if( kFALSE ==  ROC[uRocId].fbDataTimeOut[get4_index][uChanTest] )
                        bAllChanInChipOver = kFALSE;
               }
               // If chip fully finished: don't wait for the next epoch to close future buffer
               // of current event for this chip
               if( kTRUE == bAllChanInChipOver)
               {
                  // If This is the first epoch after the trigger and
                  // we did not process the epoch before the one where the
                  // trigger came, we need to initialize the 24 bit temp hits
                  if( kFALSE == ROC[uRocId].fbEpoch2SinceTrigger[get4_index] &&
                      kFALSE == ROC[uRocId].bLookInPreviousEpoch[get4_index] )
                     for( UInt_t uChan = 0; uChan < get4v10::kuNbChan; uChan++)
                           (ROC[uRocId].fHitTemp24[get4_index][uChan]).Clear();

                  BuildHits(uRocId, get4_index, ROC[uRocId].fbBufferWithLastFullEpoch2[get4_index]);
                  ROC[uRocId].fbSelectionDone[get4_index] = kTRUE;
                  CheckEventClosure();

                  for(UInt_t uChanTest = 0; uChanTest < get4v10::kuNbChan; uChanTest ++)
                     ROC[uRocId].fbDataTimeOut[get4_index][uChanTest] = kFALSE;
               } // if( kTRUE == bAllChanInChipOver)
            } // if( kFALSE == ROC[uRocId].fTriggerWind->Test( dTimeToTrigger) )
         } // Event ongoing with a detected trigger and future buffer not closed

         /** Triggering on Data **/
         if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )
         {
            for( Int_t iMainTriggerIndex = 0; iMainTriggerIndex < (Int_t)(fParam->uNbTriggers); iMainTriggerIndex++ )
               if( kTRUE == fParam->IsMainOfThisTrigger( iMainTriggerIndex, get4_index, get4_32b_dat_chan ) )
            {
               ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex] = extMess;
               /*
                * TODO: call to ProcessTrigger
                * TEMP: histo Filling + counting
                */
               Bool_t bAllInCoinc = kTRUE;
               for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
               {
                  Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iMainTriggerIndex][uSecChannel]
                                           - extMess.GetFullTimeD();
                  if( dDistanceToMain < fParam->dCoincidenceWindowStart[iMainTriggerIndex] ||
                        fParam->dCoincidenceWindowStop[iMainTriggerIndex] < dDistanceToMain      )
                  {
                     bAllInCoinc = kFALSE;
                     break;
                  } // if out of coincidence window
               } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iMainTriggerIndex]; uSecChannel++)
               if( kTRUE == bAllInCoinc )
               {
                  Double_t dSameTriggerDistance =
                          ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD()
                        - ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] ;

                  ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iMainTriggerIndex );
                  ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iMainTriggerIndex );

                  if( fParam->dDeadTime <= dSameTriggerDistance )
                  {
                     // Here we have a DATA self-trigger!!!!!!
                     ROC[uRocId].fuDataSelfTriggerCount[iMainTriggerIndex] ++;

                     ROC[uRocId].fdLastFullTimeSelfTrigger[iMainTriggerIndex] =
                           ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTimeD();

                     ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex].GetFullTime();
                     uFullTimeSelfTrigBins /= 100000000L;
                     ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                     // Long duration time distribution (1 min. bin, 10 days length)
                     if( kTRUE == fParam->bLongTimeHistos )
                        ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                     ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iMainTriggerIndex], iMainTriggerIndex);
                  } // if( fParam->dDeadTime <= dSameTriggerDistance )
               } // if( kTRUE == bAllInCoinc )
            } // if( -1 < iMainTriggerIndex )
            for( Int_t iSecTriggerIndex = 0; iSecTriggerIndex < (Int_t)(fParam->uNbTriggers); iSecTriggerIndex++ )
            {
               if( kTRUE == fParam->IsSecOfThisTrigger( iSecTriggerIndex, get4_index, get4_32b_dat_chan ) )
                  {
                     Int_t iSecondaryIndex = fParam->GetSecChannelIndex(iSecTriggerIndex, get4_index, get4_32b_dat_chan);
                     if( -1 < iSecondaryIndex )
                     {
                        ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][iSecondaryIndex] = extMess.GetFullTimeD();

                        /*
                         * TODO: call to ProcessTrigger
                         * TEMP: histo Filling + counting
                         */
                        Bool_t bAllInCoinc = kTRUE;
                        for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                        {
                           Double_t dDistanceToMain = ROC[uRocId].fdLastFullTimeSecChannel[iSecTriggerIndex][uSecChannel]
                                             - ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();
                           if( dDistanceToMain < fParam->dCoincidenceWindowStart[iSecTriggerIndex] ||
                                 fParam->dCoincidenceWindowStop[iSecTriggerIndex] < dDistanceToMain      )
                           {
                              bAllInCoinc = kFALSE;
                              break;
                           } // if out of coincidence window
                        } // for( UInt_t uSecChannel = 0; uSecChannel < fParam->uNbSecondarySelectionCh[iSecTriggerIndex]; uSecChannel++)
                        if( kTRUE == bAllInCoinc  )
                        {
                           Double_t dSameTriggerDistance =
                                   ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD()
                                 - ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] ;

                           ROC[uRocId].fDataSelfTrigDistanceNs->Fill( dSameTriggerDistance,     iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceUs->Fill( dSameTriggerDistance/1e3, iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceMs->Fill( dSameTriggerDistance/1e6, iSecTriggerIndex );
                           ROC[uRocId].fDataSelfTrigDistanceS->Fill(  dSameTriggerDistance/1e9, iSecTriggerIndex );

                           if( fParam->dDeadTime <= dSameTriggerDistance )
                           {
                              // Here we have a DATA self-trigger!!!!!!
                              ROC[uRocId].fuDataSelfTriggerCount[iSecTriggerIndex] ++;

                              ROC[uRocId].fdLastFullTimeSelfTrigger[iSecTriggerIndex] =
                                    ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTimeD();

                              ULong64_t uFullTimeSelfTrigBins = ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex].GetFullTime();
                              uFullTimeSelfTrigBins /= 100000000L;
                              ROC[uRocId].fSelfTriggT->Fill( (uFullTimeSelfTrigBins % 36000) * 0.1 );

                              // Long duration time distribution (1 min. bin, 10 days length)
                              if( kTRUE == fParam->bLongTimeHistos )
                                 ROC[uRocId].fSelfTriggLongT->Fill( (uFullTimeSelfTrigBins * 0.1 ) / 3600 );

                              ProcessTriggerMessage(uRocId, ROC[uRocId].fextMessLastMainChannel[iSecTriggerIndex], iSecTriggerIndex);
                           } // if( fParam->dDeadTime <= dSameTriggerDistance )
                        } // if( kTRUE == bAllInCoinc  )
                     } // if( -1 < iSecondaryIndex )
                  } //  if( -1 < iSecTriggerIndex )
               } // else of if( -1 < iMainTriggerIndex )
         } // if( kTRUE ==  fParam->bFreeStreaming && 0 < fParam->uNbTriggers )

         break;
      }
      default:
         break;
   } // switch(get4_32b_type)

   return get4_32b_type;
}
/**********************************************************************/

Bool_t TTofGet4Unpacker::AnalyzeAllGet4Channels( Get4v1Event &eventFull )
{

   /* TODO FILL ME
    * both readout mode histo filling
    */
   for( UInt_t  uRocId =0; uRocId<fParam->numRocs; uRocId++ )
   {
      if( !fParam->IsActiveRoc(uRocId) ) continue;

      for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
         for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
         {
            if( 0 < ( (eventFull.fGet4Boards[uGet4]).fHits[uGet4Chan]).size() )
            {
               for( UInt_t uGet4Chan_2 = uGet4Chan+1; uGet4Chan_2 < get4v10::kuNbChan; uGet4Chan_2++)
                  if( 0 < ( (eventFull.fGet4Boards[uGet4]).fHits[uGet4Chan_2]).size() )
                     ROC[uRocId].fChannelsMapping->Fill( uGet4 + (Double_t)uGet4Chan/(Double_t)get4v10::kuNbChan,
                                             uGet4 + (Double_t)uGet4Chan_2/(Double_t)get4v10::kuNbChan );

               for( UInt_t uGet4_2 = uGet4+1; uGet4_2 < get4v10::kuMaxGet4Roc; uGet4_2++)
                  for( UInt_t uGet4Chan_2 = 0; uGet4Chan_2 < get4v10::kuNbChan; uGet4Chan_2++)
                     if( 0 < ( (eventFull.fGet4Boards[uGet4_2]).fHits[uGet4Chan_2]).size() )
                        ROC[uRocId].fChannelsMapping->Fill( uGet4 + (Double_t)uGet4Chan/(Double_t)get4v10::kuNbChan,
                                                uGet4_2 + (Double_t)uGet4Chan_2/(Double_t)get4v10::kuNbChan );
            }

            ROC[uRocId].fChannelMultiplicity->Fill( uGet4 + (Double_t)uGet4Chan/(Double_t)get4v10::kuNbChan,
                  ( (eventFull.fGet4Boards[uGet4]).fHits[uGet4Chan]).size());
         }

      if( 1 == fParam->bDebugHistoEnable )
      {
         UInt_t uHistosIndexSingleChip = 0;
         UInt_t uHistosIndexChipVsChip = 0;
         for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < get4v10::kuNbChan; uFirstGet4Channel++)
         {
            for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < get4v10::kuNbChan; uSecondGet4Channel++)
            {
               if( uFirstGet4Channel < uSecondGet4Channel )
               {
                  if( 1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel]).size() &&
                      1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uSecondGet4Channel]).size() )
                  {
                     ROC[uRocId].fTimeDiffInsideChip[0][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel])[0].GetTimeDiff(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uSecondGet4Channel])[0]) );
                     // 1D FineTime correlation
                     ROC[uRocId].fFTCorrel[0][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uSecondGet4Channel])[0].GetHitLeadingFTS()
                          -( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel])[0].GetHitLeadingFTS() );
                     // 2D FineTime correlation
                     ROC[uRocId].fFTCorrel2D[0][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel])[0].GetHitLeadingFTS(),
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uSecondGet4Channel])[0].GetHitLeadingFTS() );
                  }
                  if( 1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uFirstGet4Channel]).size() &&
                      1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel]).size() )
                  {
                     ROC[uRocId].fTimeDiffInsideChip[1][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uFirstGet4Channel])[0].GetTimeDiff(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel])[0]) );
                     // 1D FineTime correlation
                     ROC[uRocId].fFTCorrel[1][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel])[0].GetHitLeadingFTS()
                          -( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uFirstGet4Channel])[0].GetHitLeadingFTS() );
                     // 2D FineTime correlation
                     ROC[uRocId].fFTCorrel2D[1][uHistosIndexSingleChip]->Fill(
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uFirstGet4Channel])[0].GetHitLeadingFTS(),
                           ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel])[0].GetHitLeadingFTS() );
                  }
                  uHistosIndexSingleChip++;
               } // if( uFirstGet4Channel < uSecondGet4Channel )
               if( 1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel]).size() &&
                   1 == ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel]).size() )
               {
                  ROC[uRocId].fTimeDiffBetweenChips[uHistosIndexChipVsChip]->Fill(
                        ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel])[0].GetTimeDiff(
                        ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel])[0]) );
                  ROC[uRocId].fFTCorrelChipToChip[uHistosIndexChipVsChip]->Fill(
                        ( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip2]).fHits[uSecondGet4Channel])[0].GetHitLeadingFTS()
                       -( (eventFull.fGet4Boards[fParam->uGet4TimeDiffChip1]).fHits[uFirstGet4Channel])[0].GetHitLeadingFTS() );
               }
               uHistosIndexChipVsChip++;
            } // for(UInt_t uSecondGet4Channel = 0; uSecondGet4Channel < MAX_GET4_CH; uSecondGet4Channel++)
         } // for(UInt_t uFirstGet4Channel = 0; uFirstGet4Channel < MAX_GET4_CH; uFirstGet4Channel++)
      } // if( 1 == fParam->bDebugHistoEnable )
   } // for( UInt_t  uRocId =0; uRocId<fParam->numRocs; uRocId++ )

   if( kTRUE == fb32bitsReadoutDetected )
      AnalyzeAllGet4Channels32Bits( fCurrentGet4Event );
      else AnalyzeAllGet4Channels24Bits( fCurrentGet4Event );

   return kTRUE;
}
Bool_t TTofGet4Unpacker::AnalyzeAllGet4Channels24Bits( Get4v1Event &eventFull )
{

   /* TODO FILL ME
    * 24 bit mode histo filling
    */

   if( 1 == fParam->bDebugHistoEnable )
      for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
         if( fParam->IsActiveRoc(uRoc) )
            if( 0 == uNbEvents[uRoc]%fParam->uNbEventsDnlUpdate )
            {
               UpdateLeadingDnlHistograms( eventFull, uRoc );
               UpdateTrailingDnlHistograms( eventFull, uRoc );
            }

   return kTRUE;
}
Bool_t TTofGet4Unpacker::AnalyzeAllGet4Channels32Bits( Get4v1Event &eventFull )
{

   /* TODO FILL ME
    * 32 bit mode histo filling
    */

   if( 1 == fParam->bDebugHistoEnable )
      for( UInt_t  uRoc =0; uRoc<fParam->numRocs; uRoc++ )
         if( fParam->IsActiveRoc(uRoc) )
            if( 0 == uNbEvents[uRoc]%fParam->uNbEventsDnlUpdate )
               UpdateLeadingDnlHistograms( eventFull, uRoc );

   return kTRUE;
}
/**********************************************************************/

/*
Bool_t TTofGet4Unpacker::PrintRocEpochIndexes(UInt_t uRocId, Int_t uMessagePriority )
{
   TString sOutput = "";

   for( UInt_t uGet4 = 0; uGet4 < fParam->uNbGet4; uGet4++)
   {
      sOutput += Form(" %7d", ROC[uRocId].fuCurrEpoch2[uGet4]);
   }
   LOG((FairLogLevel)uMessagePriority)<<"Current ROC epoch indexes: "<<sOutput.Data();

   return kTRUE;
}
Bool_t TTofGet4Unpacker::PrintRocEpochCycles(UInt_t uRocId, Int_t uMessagePriority )
{
   TString sOutput = "";

   for( UInt_t uGet4 = 0; uGet4 < fParam->uNbGet4; uGet4++)
   {
      sOutput += Form(" %6d", ROC[uRocId].fuEpoch2Cycle[uGet4]);
   }
   LOG((FairLogLevel)uMessagePriority)<<"Current ROC cycle indexes: "<<sOutput.Data();

   return kTRUE;
}
*/
/**********************************************************************/
void TTofGet4Unpacker::UpdateLeadingDnlHistograms( Get4v1Event & /*eventFull*/, UInt_t uRocId )
{
   for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
      {
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRocId, uGet4);
         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
         {
            Double_t dDnlLeading = 0;
            Double_t dSumLeading = 0;

            // First Resets everything
            ROC[uRocId].fLeadingDnl[uGet4][uGet4Chan]->Reset();
            ROC[uRocId].fLeadingDnlSum[uGet4][uGet4Chan]->Reset();

            // First bin
            dDnlLeading = ( ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetBinContent(1)
                           - (ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime) ) /
                            (ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime);
            ROC[uRocId].fLeadingDnl[uGet4][uGet4Chan]->Fill( 0.0, dDnlLeading);
            dSumLeading += dDnlLeading;
            ROC[uRocId].fLeadingDnlSum[uGet4][uGet4Chan]->Fill( 0.0, dSumLeading );

            for( int j = 2; j <= get4tdc::kiFineTime; j++)
            {
             dDnlLeading = ( ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetBinContent(j)
                             - (ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime) ) /
                            (ROC[uRocId].fLeadingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime);
             ROC[uRocId].fLeadingDnl[uGet4][uGet4Chan]->Fill( (Double_t)(j-1), dDnlLeading );
             dSumLeading += dDnlLeading;
             ROC[uRocId].fLeadingDnlSum[uGet4][uGet4Chan]->Fill( (Double_t)j-1, dSumLeading );
            }
         }
      } // For get4 in ROC, for channel in GET4
}
void TTofGet4Unpacker::UpdateTrailingDnlHistograms( Get4v1Event & /*eventFull*/, UInt_t uRocId )
{
   for( UInt_t uGet4 = 0; uGet4 < get4v10::kuMaxGet4Roc; uGet4++)
      for( UInt_t uGet4Chan = 0; uGet4Chan < get4v10::kuNbChan; uGet4Chan++)
      {
         UInt_t uRemappedGet4Index = fParam->RemapGet4Chip(uRocId, uGet4);
         if( kTRUE == fParam->IsValidGet4Chip(uRemappedGet4Index) &&
             kTRUE == fParam->IsActiveGet4Chip(uRemappedGet4Index) )
         {
            Double_t dDnlTrailing = 0;
            Double_t dSumTrailing = 0;

            // First Resets everything
            ROC[uRocId].fTrailingDnl[uGet4][uGet4Chan]->Reset();
            ROC[uRocId].fTrailingDnlSum[uGet4][uGet4Chan]->Reset();

            // First bin
            dDnlTrailing = ( ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetBinContent(1)
                            - (ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime) ) /
                            (ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime);
            ROC[uRocId].fTrailingDnl[uGet4][uGet4Chan]->Fill( 0.0, dDnlTrailing );
            dSumTrailing += dDnlTrailing;
            ROC[uRocId].fTrailingDnlSum[uGet4][uGet4Chan]->Fill( 0.0, dSumTrailing );

            for( int j = 2; j <= get4tdc::kiFineTime; j++)
            {
             dDnlTrailing = ( ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetBinContent(j)
                             - (ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime) ) /
                            (ROC[uRocId].fTrailingFTS[uGet4][uGet4Chan]->GetEntries()/(Double_t)get4tdc::kiFineTime);
             ROC[uRocId].fTrailingDnl[uGet4][uGet4Chan]->Fill( (Double_t)(j-1), dDnlTrailing );
             dSumTrailing += dDnlTrailing;
             ROC[uRocId].fTrailingDnlSum[uGet4][uGet4Chan]->Fill( (Double_t)(j-1), dSumTrailing );
            }
         }
      } // For get4 in ROC, for channel in GET4
}

roc::MessageFormat TTofGet4Unpacker::ConvertToMsgFormat(const Char_t& cNumFormat)
{
  roc::MessageFormat tmp_fmt;

  switch(cNumFormat)
  {
    case 0:
      tmp_fmt = roc::formatEth1;
      break;
    case 1:
      tmp_fmt = roc::formatOptic1;
      break;
    case 2:
      tmp_fmt = roc::formatEth2;
      break;
    case 3:
      tmp_fmt = roc::formatOptic2;
      break;
    case 4:
    default:
      tmp_fmt = roc::formatNormal;
      break;
  }

  return tmp_fmt;
}
