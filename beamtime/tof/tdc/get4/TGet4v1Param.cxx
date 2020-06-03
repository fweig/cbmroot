// ------------------------------------------------------------------
// -----                     TGet4v1Rec.h                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// -----                 Adapted from TGet4v1Rec.h              -----
// -----         in RocLib/beamtime/tof-tdctest rev.4862        -----
// ------------------------------------------------------------------
#include "TGet4v1Param.h"

#include "Riostream.h"

#include <iostream>

using std::cout;
using std::endl;

TGet4v1Param::TGet4v1Param():
   TNamed("", "TGet4v1Param"),
   numRocs            (0),
   noTrigger          (kTRUE),
   triggerSignal      (0),
   globalTrigger      (kFALSE),
   masterRoc          (77777),
   maxBufferTriggers  (10),
   maxBufferLoops     (10),
   doSorting          (kFALSE),
   bGet4Debug         (kFALSE),
   bTriggerAfterData  (kFALSE),
   iEventNbGap        (-1),
   uEventNbCycle      (0),
   uSyncCycleSize     (0),
   bSuppressedEpochs  (kFALSE),
   bLongTimeHistos    (kFALSE),
   uRocCycleSize      (0),
   uMainSyncCycleSize (0),
   uNbRocsGet4        (0),
   uNbGet4            (0),
   uSilentMode        (0),
   uDebugHistoOn      (0),
   uDebugPrintOn      (0),
   uGet4Print         (0),
   uNbTriggerChan     (0),
   uCoincidenceWinSize(0),
   b32bReadoutModeGet4v1   (kFALSE),
   dMaxTot                 (300000.0),
   bTotHistoEnable         (kFALSE),
   bDebugHistoEnable       (kFALSE),
   uNbEventsDnlUpdate      (1000),
   uGet4TimeDiffChip1      (0),
   uGet4TimeDiffChip2      (0),
   bChannelRateHistoEnable (kFALSE),
   bChipRateHistoEnable    (kFALSE),
   bRawDataMode            (kFALSE),
   bFreeStreaming          (kFALSE),
   uNbTriggers             (0),
   dDeadTime               (0.0),
   bAllowEventMixing       (kFALSE),
   iMainReferenceTdc       (-1),
   iReference1Channel      (-1),
   iReference2Channel      (-1)
{
//   numRocs = 0;
   for (UInt_t roc=0;roc<get4v10::kuMaxRoc;roc++) {
      activeRoc[roc] = kFALSE;
      timeScale[roc] = 1.0;
      timeShift[roc] = 0;
      uGet4RocsMapping[roc] = 0;
   }
/*
   noTrigger = kTRUE;  // ignore trigger window and take all messages with initial event division
   triggerSignal = 0; // 0 .. 3 is AUXs, 10-11 is SYNCs
   globalTrigger = kFALSE; // true for selecting messages under global corrected time window, false for using separate windows for each roc
   masterRoc = 77777; // id number of ROC which is time/trigger reference, big number is disable master roc
   maxBufferTriggers=10;
   maxBufferLoops=10;
   doSorting = kFALSE;  // perform time sorting of messages
   bGet4Debug = kFALSE;

   bTriggerAfterData = kFALSE;
*/
   /** GET4 unpacking & pre-processing **/
/*
   iEventNbGap    = -1;      // -1 = check Off, Gap in event nember between get4 events
   uEventNbCycle  = 0;       // No Online Change - Size of the event nember cycle
   uSyncCycleSize = 0;       // No Online Change - Size of the synchronization cycle in epochs
   bSuppressedEpochs = kFALSE;// No Online Change - kFALSE = Normal epoch operation, kTRUE = 24b readout suppressed epoch mode, epoch only just after data
   bLongTimeHistos = kFALSE; // Enable some Long duration counts monitoring histos (10 days)
   uRocCycleSize  = 0;       // No Online Change - Size of roc epoch counter cycle in epochs
   uMainSyncCycleSize = 0;   // No Online Change - Size of the synchronization cycle in Main clock (250MHz) epochs
   uNbRocsGet4 = 0;          // No Online Change - Nb of Rocs "GET4" in setup
   uNbGet4 = 0;             // No Online Change - Total Nb of active FEET boards in setup
   uSilentMode = 0;          // Remove all printouts => Blind mode!
   uDebugHistoOn = 0;        // No Online Change - Enable/Disable general debug histograms
   uDebugPrintOn = 0;        // Number of data messages to be printed on screen for debug
   uGet4Print = 0;           // Number of data messages to be printed on screen for debug
*/
   /************* GET4 v1.x debug *******/
/*
   dMaxTot            = 300000.0;  // Maximal allowed Tot for 24 bits hit building, in ps
   bTotHistoEnable    = kFALSE;         // Enable Tot histograms
   bDebugHistoEnable  = kFALSE;       // Enable the time difference, finetime and DNL histograms
   uNbEventsDnlUpdate = 1000;      // Nb of events between 2 updates of the DNL histograms
   uGet4TimeDiffChip1 = 0;   // Index of first chip to be used for time difference histos
   uGet4TimeDiffChip2 = 0;   // Index of first chip to be used for time difference histos
   bChannelRateHistoEnable = kFALSE; // Enable Rate histograms for each channel
   bChipRateHistoEnable    = kFALSE; // Enable Rate histograms for each chip
*/
   /*************************************/
   /*********** free-streaming **********/
/*
   bRawDataMode   = kFALSE;          // kFALSE = DABC data format, 1 SYNC/Event, kTRUE = RAW DATA format, all messages
   bFreeStreaming = kFALSE;          // kFALSE = Sync trigger/no trigger, kTRUE= online detection/selection from data
*/
   /*************************************/

   for (UInt_t n=0;n<get4v10::kuMaxGet4;n++) {
      uGet4Mapping[n] = 0; // No Online Change - Get4 Mapping: there should be always at least 2*nbfeets entries here
      uGet4Active[n] = 0; // Activated Get4 chips (indexes after remapping => not hardware map !)
      uGet4EdgeInversion[n] = 0;   // Edges inversion: affect all channels of a GET4 chip
   }
/*
   iMainReferenceTdc  = -1;  // No Online Change - TDC used for the reference signals profiles in Detector classes, -1 disable it
   iReference1Channel = -1;  // No Online Change - Channel used for the 1st reference histogram in Detector classes, -1 disable it
   iReference2Channel = -1;  // No Online Change - Channel used for the 2nd reference histogram in Detector classes, -1 disable it
*/
}



TGet4v1Param::TGet4v1Param(const char* name):
   TNamed(name, "TGet4v1Param"),
   numRocs            (0),
   noTrigger          (kTRUE),
   triggerSignal      (0),
   globalTrigger      (kFALSE),
   masterRoc          (77777),
   maxBufferTriggers  (10),
   maxBufferLoops     (10),
   doSorting          (kFALSE),
   bGet4Debug         (kFALSE),
   bTriggerAfterData  (kFALSE),
   iEventNbGap        (-1),
   uEventNbCycle      (0),
   uSyncCycleSize     (0),
   bSuppressedEpochs  (kFALSE),
   bLongTimeHistos    (kFALSE),
   uRocCycleSize      (0),
   uMainSyncCycleSize (0),
   uNbRocsGet4        (0),
   uNbGet4            (0),
   uSilentMode        (0),
   uDebugHistoOn      (0),
   uDebugPrintOn      (0),
   uGet4Print         (0),
   uNbTriggerChan     (0),
   uCoincidenceWinSize(0),
   b32bReadoutModeGet4v1   (kFALSE),
   dMaxTot                 (300000.0),
   bTotHistoEnable         (kFALSE),
   bDebugHistoEnable       (kFALSE),
   uNbEventsDnlUpdate      (1000),
   uGet4TimeDiffChip1      (0),
   uGet4TimeDiffChip2      (0),
   bChannelRateHistoEnable (kFALSE),
   bChipRateHistoEnable    (kFALSE),
   bRawDataMode            (kFALSE),
   bFreeStreaming          (kFALSE),
   uNbTriggers             (0),
   dDeadTime               (0.0),
   bAllowEventMixing       (kFALSE),
   iMainReferenceTdc       (-1),
   iReference1Channel      (-1),
   iReference2Channel      (-1)
{
//   numRocs = 4;
   for (UInt_t rocid=0;rocid<get4v10::kuMaxRoc; rocid++) {
      if(rocid<numRocs)
         activeRoc[rocid]=kTRUE;
      else
         activeRoc[rocid]=kFALSE;

      timeScale[rocid]=1.0;
      timeShift[rocid]=0;
   }
/*
   globalTrigger = true;
   
   // use AUX2 as default trigger signal, while it is external trigger input for ROC
   triggerSignal = 2;
   noTrigger = kFALSE;
   masterRoc = 0;
   maxBufferTriggers=10;
   maxBufferLoops=10;
   doSorting = kTRUE;
   bGet4Debug = kFALSE;

   bTriggerAfterData = kFALSE;
*/
   /** GET4 unpacking & pre-processing **/
/*
   iEventNbGap    = -1;      // -1 = check Off, Gap in event nember between get4 events
   uSyncCycleSize = 0;       // No Online Change - Size of the synchronization cycle in epochs
   bSuppressedEpochs = kFALSE;// No Online Change - kFALSE = Normal epoch operation, kTRUE = 24b readout suppressed epoch mode, epoch only just after data
   bLongTimeHistos = kFALSE; // Enable some Long duration counts monitoring histos (10 days)
   uRocCycleSize  = 0;       // No Online Change - Size of roc epoch counter cycle in epochs
   uMainSyncCycleSize = 0;   // No Online Change - Size of the synchronization cycle in Main clock (250MHz) epochs
   uNbRocsGet4 = 0;          // No Online Change - Nb of Rocs "GET4" in setup
   uNbGet4 = 0;             // No Online Change - Total Nb of active FEET boards in setup
   uSilentMode = 0;          // Remove all printouts => Blind mode!
   uDebugHistoOn = 0;        // No Online Change - Enable/Disable general debug histograms
   uDebugPrintOn = 0;        // Number of data messages to be printed on screen for debug
   uGet4Print = 0;           // Number of data messages to be printed on screen for debug
*/
   /************* GET4 v1.x debug *******/
/*
   dMaxTot            = 300000.0;  // Maximal allowed Tot for 24 bits hit building, in ps
   bTotHistoEnable    = kFALSE;         // Enable Tot histograms
   bDebugHistoEnable  = kFALSE;       // Enable the time difference, finetime and DNL histograms
   uNbEventsDnlUpdate = 1000;      // Nb of events between 2 updates of the DNL histograms
   uGet4TimeDiffChip1 = 0;   // Index of first chip to be used for time difference histos
   uGet4TimeDiffChip2 = 0;   // Index of first chip to be used for time difference histos
   bChannelRateHistoEnable = kFALSE; // Enable Rate histograms for each channel
   bChipRateHistoEnable    = kFALSE; // Enable Rate histograms for each chip
*/
   /*************************************/
   /*********** free-streaming **********/
/*
   bRawDataMode   = kFALSE;          // kFALSE = DABC data format, 1 SYNC/Event, kTRUE = RAW DATA format, all messages
   bFreeStreaming = kFALSE;          // kFALSE = Sync trigger/no trigger, kTRUE= online detection/selection from data
*/
   /*************************************/

   for (UInt_t n=0;n<get4v10::kuMaxGet4;n++) {
      uGet4Mapping[n] = 0; // No Online Change - Get4 Mapping: there should be always at least 2*nbfeets entries here
      uGet4Active[n] = 0; // Activated Get4 chips (indexes after remapping => not hardware map !)
      uGet4EdgeInversion[n] = 0;   // Edges inversion: affect all channels of a GET4 chip
   }
/*
   iMainReferenceTdc  = -1;  // No Online Change - TDC used for the reference signals profiles in Detector classes, -1 disable it
   iReference1Channel = -1;  // No Online Change - Channel used for the 1st reference histogram in Detector classes, -1 disable it
   iReference2Channel = -1;  // No Online Change - Channel used for the 2nd reference histogram in Detector classes, -1 disable it
*/
}

TGet4v1Param::~TGet4v1Param()
{
   SetNbRocsGet4(0);

   SetNbGet4(0);
}

// Roc functions
Bool_t TGet4v1Param::SetConfigRocs()
{
   /*
   for (UInt_t rocid=0;rocid<get4v10::kuMaxRoc; rocid++)
   {
      TGet4v1Event::ConfigRocs[rocid] = rocid < numRocs ? (UInt_t) activeRoc[rocid] : 0;
      cout <<"TGet4v1Param::SetConfigRocs sets rocid " << rocid << " to active:" << TGet4v1Event::ConfigRocs[rocid] << endl;
   }
   */
   return kTRUE;
}

// Get4 functions
void TGet4v1Param::SetNbRocsGet4(UInt_t num)
{
   uNbRocsGet4 = num;

   if (uNbRocsGet4 > get4v10::kuMaxRoc) uNbRocsGet4 = get4v10::kuMaxRoc;

   if (uNbRocsGet4==0) return;
   for (unsigned n=0;n<uNbRocsGet4;n++)
      uGet4RocsMapping[n] = n;
}

Int_t TGet4v1Param::FindGet4RocId(UInt_t rocid) const
{
   for(Int_t iGet4Roc=0; iGet4Roc < (Int_t)uNbRocsGet4; iGet4Roc++)
      if(rocid == uGet4RocsMapping[iGet4Roc]) return iGet4Roc;

   return -1;
}

UInt_t TGet4v1Param::DefineGet4IndexOffset(UInt_t rocid) const
{
   Int_t id = FindGet4RocId(rocid);
   return id>=0 ? id*get4v10::kuMaxGet4Roc : 0;
}

void TGet4v1Param::SetNbGet4(UInt_t num)
{

   uNbGet4  = num;
   if (uNbGet4>get4v10::kuMaxGet4) uNbGet4 = get4v10::kuMaxGet4;

   if (uNbGet4==0) return;

   for(UInt_t uGet4Index = 0; uGet4Index < uNbGet4; uGet4Index++) {
      // No Online Change - Get4 Mapping: there should be always at least 2*nbfeets entries here
      uGet4Mapping[uGet4Index]       = uGet4Index;
      // Activated Get4 chips (indexes after remapping => not hardware map !)
      uGet4Active[uGet4Index]        = 0;
      // Edges inversion: affect all channels of a GET4 chip
      uGet4EdgeInversion[uGet4Index] = 0;
   }
}

UInt_t TGet4v1Param::RemapGet4Chip(UInt_t uChip) const
{
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
      if( uChip == uGet4Mapping[temp_index]) return temp_index;
   return uNbGet4;
}
UInt_t TGet4v1Param::RemapGet4Chip(UInt_t roc, UInt_t uChip) const
{
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
      if( DefineGet4IndexOffset(roc)+uChip == uGet4Mapping[temp_index]) return temp_index;
   return uNbGet4;
}

// Data Triggering
/** Return -1 if not a main channel for any trigger, trigger number otherwise */
Int_t  TGet4v1Param::IsTriggerMainChannel( UInt_t tdc, UInt_t channel )
{
   for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
   {
      if( tdc == uMainSelectionTdc[uTrigger] &&
            channel == uMainSelectionChannel[uTrigger])
         return uTrigger;
   } // for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
   return -1;
}
/** Return -1 if not a secondary channel for any trigger, trigger number otherwise */
Int_t  TGet4v1Param::IsTriggerSecChannel( UInt_t tdc, UInt_t channel )
{
   for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
   {
      if( 0 < uNbSecondarySelectionCh[uTrigger])
      {
         for(UInt_t uSecondary = 0; uSecondary < uNbSecondarySelectionCh[uTrigger]; uSecondary++)
            if( tdc == uSecondarySelectionTdc[uTrigger][uSecondary] &&
                  channel == uSecondarySelectionChannel[uTrigger][uSecondary] )
               return uTrigger;
      } // if( 0 < uNbSecondarySelectionCh[iTrigger])
   } // for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
   return -1;
}
/** Return secondary channel index if the group (trigger, tdc, channel) match, -1 otherwise */
Int_t  TGet4v1Param::GetSecChannelIndex( UInt_t uTrigger,  UInt_t tdc, UInt_t channel )
{
   if( 0 < uNbSecondarySelectionCh[uTrigger])
   {
      for(UInt_t iSecondary = 0; iSecondary < uNbSecondarySelectionCh[uTrigger]; iSecondary++)
         if( tdc == uSecondarySelectionTdc[uTrigger][iSecondary] &&
               channel == uSecondarySelectionChannel[uTrigger][iSecondary] )
            return iSecondary;
   } // if( 0 < uNbSecondarySelectionCh[uTrigger])
   return -1;
}
Bool_t TGet4v1Param::IsMainOfThisTrigger( UInt_t uTrigger,  UInt_t tdc, UInt_t channel )
{
   if( tdc == uMainSelectionTdc[uTrigger] &&
         channel == uMainSelectionChannel[uTrigger])
      return kTRUE;
      else return kFALSE;
}
Bool_t TGet4v1Param::IsSecOfThisTrigger(  UInt_t uTrigger,  UInt_t tdc, UInt_t channel )
{
   for(UInt_t iSecondary = 0; iSecondary < uNbSecondarySelectionCh[uTrigger]; iSecondary++)
      if( tdc == uSecondarySelectionTdc[uTrigger][iSecondary] &&
            channel == uSecondarySelectionChannel[uTrigger][iSecondary] )
         return kTRUE;
   return kFALSE;
}


Bool_t TGet4v1Param::PrintGet4Options()
{
   cout<<"*************************************************"<<endl;
   cout<<"       Option values"<<endl;
   cout<<"Trigger after data:      "<<bTriggerAfterData<<endl;
   cout<<"Event index gap:         "<<iEventNbGap<<endl;
   cout<<"Sync cycle size:         "<<uSyncCycleSize<<" Epochs (1 synch + "<<(uSyncCycleSize-1)<<" Unsynched)"<<endl;
   cout<<"Sync cycle size (Main):  "<<uMainSyncCycleSize<<" Epochs (1 synch + "<<(uMainSyncCycleSize-1)<<" Unsynched)"<<endl;
   cout<<"Nb ROC-GET4   :          "<<uNbRocsGet4<<endl;
   cout<<"Rocs Mapping  :  |-      ";
   for(UInt_t temp_index = 0; temp_index < uNbRocsGet4; temp_index++)
   {
      cout.width(2);
      cout<<uGet4RocsMapping[temp_index]<<" ";
   }
   cout<<endl<<"                 |->     ";
   for(UInt_t temp_index = 0; temp_index < uNbRocsGet4; temp_index++)
   {
      cout.width(2);
      cout<<temp_index<<" ";
   }
   cout<<endl;
   cout<<"Nb GET4 chips:           "<<uNbGet4<<endl;
   cout<<"Get4 Mapping  :  |-      ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<uGet4Mapping[temp_index]<<" ";
   }
   cout<<endl<<"                 |->     ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<temp_index<<" ";
   }
   cout<<endl;
   cout<<"Get4 Activated:  |-      ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<temp_index<<" ";
   }
   cout<<endl<<"                 |->     ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<uGet4Active[temp_index]<<" ";
   }
   cout<<endl;
   cout<<"Get4 Inversion:  |-      ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<temp_index<<" ";
   }
   cout<<endl<<"                 |->     ";
   for(UInt_t temp_index = 0; temp_index < uNbGet4; temp_index++)
   {
      cout.width(2);
      cout<<uGet4EdgeInversion[temp_index]<<" ";
   }
   cout<<endl;
   cout<<"Nb Get4 message printed: "<<uGet4Print<<endl;
   if( 1 == uSilentMode )
      cout<<"Silent Mode                 :    ON "<<endl;
      else cout<<"Silent Mode                 :    OFF"<<endl;
   if( 1 == uDebugHistoOn )
      cout<<"Debug Histograms:                ON "<<endl;
      else cout<<"Debug Histograms:                OFF"<<endl;
   if( 1 == uDebugPrintOn )
      cout<<"Debug Printout:                  ON "<<endl;
      else cout<<"Debug Printout:                  OFF"<<endl;
   cout<<"Chips used for time diff: "<<uGet4TimeDiffChip1<<" and "<<uGet4TimeDiffChip2<<endl;
   cout<<"Nb Get4 message printed: "<<uGet4Print<<endl;
   cout<<"*************************************************"<<endl;

   /************* GET4 v1.0 *************/
   cout<<"       Get4 v1.0 Option values"<<endl;
   if( kTRUE == b32bReadoutModeGet4v1 )
      cout<<"Readout Mode:                    32 bits "<<endl;
      else
      {
         cout<<"Readout Mode:                    24 bit"<<endl;
         if( kTRUE == bSuppressedEpochs )
            cout<<"Suppressed epoch mode:           ON "<<endl;
            else cout<<"Suppressed epoch mode:           OFF"<<endl;
      } // else of if( kTRUE == b32bReadoutModeGet4v1 )
   if( kTRUE == bLongTimeHistos )
      cout<<"Long duration counts mon. hist.: ON "<<endl;
      else cout<<"Long duration counts mon. hist.: OFF"<<endl;
   cout<<"*************************************************"<<endl;
   /*************************************/

   /*********** free-streaming **********/
   if( kTRUE == bFreeStreaming )
   {
      cout<<"       Free-streaming Option values"<<endl;
      for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
      {
         cout<<"Tdc main trigger chan:   "<<uMainSelectionTdc[uTrigger]<<endl;
         cout<<"Channel main trigger:    "<<uMainSelectionChannel[uTrigger]<<endl;
         if( 0 < uNbSecondarySelectionCh[uTrigger])
         {
            cout<<"Sec. trigger chans: |-      ";
            for(UInt_t temp_index = 0; temp_index < uNbSecondarySelectionCh[uTrigger]; temp_index++)
            {
               cout.width(3);
               cout<<temp_index<<" ";
            }
            cout<<endl<<" TDC                |->     ";
            for(UInt_t temp_index = 0; temp_index < uNbSecondarySelectionCh[uTrigger]; temp_index++)
            {
               cout.width(3);
               cout<<uSecondarySelectionTdc[uTrigger][temp_index]<<" ";
            }
            cout<<endl<<" Channel            |->     ";
            for(UInt_t temp_index = 0; temp_index < uNbSecondarySelectionCh[uTrigger]; temp_index++)
            {
               cout.width(3);
               cout<<uSecondarySelectionChannel[uTrigger][temp_index]<<" ";
            }
            cout<<endl;
         }
         cout<<"Channel main trigger:    "<<uMainSelectionChannel[uTrigger]<<endl;
         cout<<"Coincidenc Window start: "<<dCoincidenceWindowStart[uTrigger]<<" ns relative to main channel hits"<<endl;
         cout<<"Coincidenc Window stop:  "<<dCoincidenceWindowStop[uTrigger]<<" ns relative to main channel hits"<<endl;
      } // for( UInt_t uTrigger = 0; uTrigger < uNbTriggers; uTrigger++)
      cout<<"Deadtime:                "<<dDeadTime<<" ns"<<endl;
      if( kTRUE == bAllowEventMixing )
         cout<<"Event mixing/superpos.:  ON"<<endl;
         else cout<<"Event mixing/superpos.:  OFF"<<endl;
   } // if( kTRUE == bFreeStreaming )// if( kTRUE == bFreeStreaming )
      else if( 9 < triggerSignal )
         cout<<"       Use SYNC "<<(triggerSignal- 10)<<" as trigger"<<endl;
      else cout<<"       Use AUX "<<triggerSignal<<" as trigger"<<endl;
   cout<<"*************************************************"<<endl;
   /*************************************/
   /************** Detectors ************/
   if(-1 < iMainReferenceTdc )
   {
      cout<<"       Detectors Option values"<<endl;
      cout<<"Tdc reference signals:   "<<iMainReferenceTdc<<endl;
      if( -1 < iReference1Channel )
         cout<<"Channel reference 1:     "<<iReference1Channel<<endl;
      else cout<<"Reference 1:             OFF"<<endl;
      if( -1 < iReference2Channel )
         cout<<"Channel reference 2:     "<<iReference2Channel<<endl;
      else cout<<"Reference 2:             OFF"<<endl;
   } // if(-1 < iMainReferenceTdc )
   else cout<<"Reference hist. in det.: OFF"<<endl;
   cout<<"*************************************************"<<endl;
   /*************************************/

   return kTRUE;
}
