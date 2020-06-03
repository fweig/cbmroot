// ------------------------------------------------------------------
// -----                     TGet4v1Rec.h                       -----
// -----              Created 22/07/2013 by P.-A. Loizeau       -----
// -----                 Adapted from TGet4v1Rec.h              -----
// -----         in RocLib/beamtime/tof-tdctest rev.4862        -----
// ------------------------------------------------------------------

#ifndef TGET4V1PARAM_H
#define TGET4V1PARAM_H

#include "TofGet4Def.h"
#include "TNamed.h"

class TGet4v1Param : public TNamed
{
   public:

      // default constructor - important for streamer
      TGet4v1Param();

      TGet4v1Param(const char* name);

      virtual ~TGet4v1Param();

      /* implemented to copy setup of active roc to roc event */
//      virtual Bool_t UpdateFrom(TGo4Parameter* rhs);

      /* Actual set roc event configuration. To be used from setup macro*/
      Bool_t SetConfigRocs();

      UInt_t numRocs;  // maximum roc id in use +1

      Bool_t activeRoc[get4v10::kuMaxRoc]; //enable roc of id

      Bool_t noTrigger;  // ignore trigger window and take all messages with initial event division

      Int_t triggerSignal; // 0 .. 3 is AUXs, 10-11 is SYNCs

      Bool_t globalTrigger; // true for selecting messages under global corrected time window, false for using separate windows for each roc

      UInt_t masterRoc; // id number of ROC which is time/trigger reference for global roc

      UInt_t maxBufferTriggers; // maximum number of triggers in same mbs input buffer until retrieving next
      UInt_t maxBufferLoops; // maximum number of loops over same mbs input buffer until retrieving next

      Double_t timeScale[get4v10::kuMaxRoc]; // scaling factor for time correction

      Int_t timeShift[get4v10::kuMaxRoc]; // instrumental time shift between triggers of each ROC

      Bool_t doSorting;  // perform time sorting of messages

      Bool_t         bGet4Debug;           // provide debug information about Get4 errors
      
      Bool_t bTriggerAfterData;       // kFALSE = SYNC arriving before the event data, kTRUE = after

      /** GET4 unpacking & pre-processing **/
      Int_t     iEventNbGap;          // -1 = check Off, Gap in event nember between get4 events
      Int_t     uEventNbCycle;        // No Online Change - Size of the event nember cycle
      UInt_t    uSyncCycleSize;       // No Online Change - Size of the synchronization cycle in epochs
      // No Online Change - kFALSE = Normal epoch operation,
      //                    kTRUE = 24b readout suppressed epoch mode, epoch only just after data
      Bool_t    bSuppressedEpochs;    // No Online Change - For suppressed epoch mode in 24b readout
      Bool_t    bLongTimeHistos;      // Enable some Long duration counts monitoring histos (10 days)
      ULong64_t uRocCycleSize;        // No Online Change - Size of roc epoch counter cycle in epochs
      UInt_t    uMainSyncCycleSize;   // No Online Change - Size of the synchronization cycle in Main clock (250MHz) epochs
      UInt_t    uNbRocsGet4;          // No Online Change - Nb of Rocs "GET4" in setup
      UInt_t    uGet4RocsMapping[get4v10::kuMaxRoc]; // No Online Change - Roc-GET4 id among indexes of all rocs
      UInt_t    uNbGet4;                   // No Online Change - Total Nb of active FEET boards in setup
      UInt_t    uGet4Mapping[get4v10::kuMaxGet4];    // No Online Change - Get4 Mapping: there should be always at least 2*nbfeets entries here
      UInt_t    uGet4Active[get4v10::kuMaxGet4];          // Activated Get4 chips (indexes after remapping => not hardware map !)
      UInt_t    uGet4EdgeInversion[get4v10::kuMaxGet4];   // Edges inversion: affect all channels of a GET4 chip
      UInt_t    uSilentMode;          // Remove all printouts => Blind mode!
      UInt_t    uDebugHistoOn;        // No Online Change - Enable/Disable general debug histograms
      UInt_t    uDebugPrintOn;        // Number of data messages to be printed on screen for debug
      UInt_t    uGet4Print;           // Number of data messages to be printed on screen for debug

      UInt_t    uNbTriggerChan;       // Number of Get4 channels to use for the coincidence trigger, max 8
      UInt_t    uCoincidenceWinSize;  // Size of the coincidence window for trigger building
      UInt_t    uTriggerChannels[8];  // Channels to be used for building a coincidence trigger
      /*************************************/

      /************* GET4 v1.0 *************/
      Bool_t    b32bReadoutModeGet4v1;   // Get4 v1.0 readout mode: kFALSE = 24 bits, kTRUE = 32 bits
      /*************************************/
      /************* GET4 v1.x debug *******/
      Double_t  dMaxTot;                 // Maximal allowed Tot for 24 bits hit building, in ps
      Bool_t    bTotHistoEnable;         // Enable Tot histograms
      Bool_t    bDebugHistoEnable;       // Enable the time difference, finetime and DNL histograms
      UInt_t    uNbEventsDnlUpdate;      // Nb of events between 2 updates of the DNL histograms
      UInt_t    uGet4TimeDiffChip1;      // Index of first chip to be used for time difference histos
      UInt_t    uGet4TimeDiffChip2;      // Index of first chip to be used for time difference histos
      Bool_t    bChannelRateHistoEnable; // Enable Rate histograms for each channel
      Bool_t    bChipRateHistoEnable;    // Enable Rate histograms for each chip
      /*************************************/
      /*********** free-streaming **********/
      Bool_t    bRawDataMode;            // kFALSE = DABC data format, 1 SYNC/Event, kTRUE = RAW DATA format, all messages
      Bool_t    bFreeStreaming;          // kFALSE = Sync trigger/no trigger, kTRUE= online detection/selection from data
      UInt_t    uNbTriggers;             // Number of user defined data triggers
      UInt_t    uNbSecondarySelectionCh[get4v10::kiMaxNbDataTriggs]; // Nb of secondary channels to use for event detection/selection
      UInt_t    uMainSelectionTdc[get4v10::kiMaxNbDataTriggs];       // TDC index of the main signal for online selection. Selection window is relative to this
      UInt_t    uMainSelectionChannel[get4v10::kiMaxNbDataTriggs];   // Channel of the main signal for online selection. Selection window is relative to this
      UInt_t    uSecondarySelectionTdc[get4v10::kiMaxNbDataTriggs][get4v10::kiMaxNbSecTriggCh];     // TDC index of the secondary signals for online selection.
      UInt_t    uSecondarySelectionChannel[get4v10::kiMaxNbDataTriggs][get4v10::kiMaxNbSecTriggCh]; // Channel of the secondary signals for online selection.
      Double_t  dCoincidenceWindowStart[get4v10::kiMaxNbDataTriggs]; // Beginning of time window in which secondary channels have to be present, relative to main channel
      Double_t  dCoincidenceWindowStop[get4v10::kiMaxNbDataTriggs];  // End of time window in which secondary channels have to be present, relative to main channel
      Double_t  dDeadTime;               // Minimal time between the main channel of two identical triggers in ns

      // kFALSE = 1 hit => max 1 event of same trigger, 2nd event ignored ( ~Deadtime )
      // kTRUE  = 1 hit => all events matching trigger window inside MBS event as long as same trigger
      Bool_t    bAllowEventMixing;       // kFALSE = 1 hit => max 1 event, kTRUE = 1 hit => all trigger window matching events inside MBS event
      /*************************************/

      /************** Detectors ************/
      Int_t   iMainReferenceTdc;         // No Online Change - TDC used for the reference signals profiles in Detector classes, -1 disable it
      Int_t   iReference1Channel;        // No Online Change - Channel used for the 1st reference histogram in Detector classes, -1 disable it
      Int_t   iReference2Channel;        // No Online Change - Channel used for the 2nd reference histogram in Detector classes, -1 disable it
      /*************************************/

      /** Set number of ROCs for Get4 readout, automatically sets default uFeetRocsMapping */
      void SetNbRocsGet4(UInt_t num);

      /** Find feet rocid, using uFeetRocsMapping. Return -1 if not exists in the map */
      Int_t FindGet4RocId(UInt_t rocid) const;

      /** Calculate offset for Get4 chip using rocid */
      UInt_t DefineGet4IndexOffset(UInt_t rocid) const;

      /** Change uNbGet4, automatically sets defaults for Get4Mapping, Get4Active and Get4EdgeInversion */
      void SetNbGet4(UInt_t num);

      /** Find Get4 chip in current map, returns uNbGet4 if not found */
      UInt_t RemapGet4Chip(UInt_t chip) const;
      UInt_t RemapGet4Chip(UInt_t roc, UInt_t chip) const;

      /** Returns true if Roc id is inside mapping index */
      Bool_t IsValidRoc(UInt_t board) const { return board < numRocs; }

      /** Returns true if chip id is inside mapping index */
      Bool_t IsValidGet4Chip(UInt_t chip) const { return chip < uNbGet4; }

      /** Returns true if Roc id is activated */
      Bool_t IsActiveRoc(UInt_t board) const { return (board < numRocs)? (kTRUE == activeRoc[board]): kFALSE ; }

      /** Returns true if chip id is activated */
      Bool_t IsActiveGet4Chip(UInt_t chip) const { return (chip < uNbGet4)? (1 == uGet4Active[chip]): kFALSE; }

      /** Return -1 if not a main channel for any trigger, trigger number otherwise */
      Int_t  IsTriggerMainChannel( UInt_t tdc, UInt_t channel );
      /** Return -1 if not a secondary channel for any trigger, trigger number otherwise */
      Int_t  IsTriggerSecChannel( UInt_t tdc, UInt_t channel );
      /** Return secondary channel index if the group (trigger, tdc, channel) match, -1 otherwise */
      Int_t  GetSecChannelIndex( UInt_t uTrigger,  UInt_t tdc, UInt_t channel );

      Bool_t IsMainOfThisTrigger( UInt_t uTrigger,  UInt_t tdc, UInt_t channel );
      Bool_t IsSecOfThisTrigger(  UInt_t uTrigger,  UInt_t tdc, UInt_t channel );

      Bool_t PrintGet4Options();


   ClassDef(TGet4v1Param, 1)
};

#endif // TGET4V1PARAM_H
