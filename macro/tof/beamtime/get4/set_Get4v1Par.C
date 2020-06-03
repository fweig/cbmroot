void set_Get4v1Par(  )
//void set_Get4v1Par( TGet4v1Param* param1 )
{
   TGet4v1Param* param1 = 0;
   param1 = (TGet4v1Param*)gROOT->FindObject("Get4v1Par");

   if (param1==0) {
      cout<<"Could not find parameter Get4v1Par"<<endl;
      return;
   }

   if (strcmp(param1->ClassName(), "TGet4v1Param") != 0) {
      cout<<"Parameter Get4v1Par has wrong class "<<param1->ClassName()<<endl;
      return;
   }

   cout << "Set parameter Get4Par values" << endl;

   param1->numRocs = 1;
   param1->activeRoc[0] = kTRUE;
   param1->activeRoc[1] = kFALSE;
   param1->activeRoc[2] = kFALSE;
   param1->activeRoc[3] = kFALSE;
   param1->activeRoc[4] = kFALSE;
   param1->activeRoc[5] = kFALSE;
   param1->activeRoc[6] = kFALSE;
   param1->activeRoc[7] = kFALSE;
   param1->activeRoc[8] = kFALSE;

   param1->noTrigger = kFALSE;
   param1->triggerSignal = 10;
   param1->globalTrigger = kFALSE;
   param1->masterRoc = 0;

   param1->timeScale[0] = 1.000000;
   param1->timeScale[1] = 1.000000;
   param1->timeScale[2] = 1.000000;
   param1->timeScale[3] = 1.000000;
   param1->timeScale[4] = 1.000000;
   param1->timeScale[5] = 1.000000;
   param1->timeScale[6] = 1.000000;
   param1->timeScale[7] = 1.000000;
   param1->timeScale[8] = 1.000000;
   param1->timeShift[0] = 0;
   param1->timeShift[1] = 0;
   param1->timeShift[2] = 0;
   param1->timeShift[3] = 0;
   param1->timeShift[4] = 0;
   param1->timeShift[5] = 0;
   param1->timeShift[6] = 0;
   param1->timeShift[7] = 0;
   param1->timeShift[8] = 0;
   param1->doSorting = kTRUE;
   

   param1->bGet4Debug = kFALSE;

   // kFALSE = SYNC arriving before the event data, kTRUE = after
   param1->bTriggerAfterData = kTRUE;

      /** GET4 unpacking & pre-processing **/
   // No Online Change - Size of the synchronization cycle in epochs
//   param1->iEventNbGap         = 64;
   param1->iEventNbGap         = 1;
   // No Online Change - Size of the event nember cycle
   param1->uEventNbCycle       = 16777216; // ROC auto SYNC limit
   // No Online Change - Size of the synchronization cycle in epochs
   param1->uSyncCycleSize      = 25;
   // No Online Change - kFALSE = Normal epoch operation,
   //                    kTRUE = 24b readout suppressed epoch mode, epoch only just after data
   param1->bSuppressedEpochs = kTRUE;
   // Enable some Long duration counts monitoring histos (10 days)
   param1->bLongTimeHistos = kTRUE;
   // No Online Change - Size of roc epoch counter cycle in epochs
//   param1->uRocCycleSize       = 1048576;  // old value, can be used to have graph with ~27s bin
   param1->uRocCycleSize       = 4294967295;  // New value, can be used to have graph with ~31h16m29s bin
   // No Online Change - Size of the synchronization cycle in Main clock (250MHz) epochs
   param1->uMainSyncCycleSize  = 40;
   // No Online Change - Nb of Rocs "GET4" in setup
   param1->SetNbRocsGet4(1);
   // No Online Change - Roc-GET4 id among indexes of all rocs
   // No Online Change - Total Nb of active FEET boards in setup
   param1->SetNbGet4(8);
   param1->uGet4Mapping[ 0] =  0;
   param1->uGet4Mapping[ 1] =  1;
   param1->uGet4Mapping[ 2] =  2;
   param1->uGet4Mapping[ 3] =  3;
   param1->uGet4Mapping[ 4] =  4;
   param1->uGet4Mapping[ 5] =  5;
   param1->uGet4Mapping[ 6] =  6;
   param1->uGet4Mapping[ 7] =  7;
   param1->uGet4Mapping[ 8] =  8;
   param1->uGet4Mapping[ 9] =  9;
   param1->uGet4Mapping[10] = 10;
   param1->uGet4Mapping[11] = 11;
   param1->uGet4Mapping[12] = 12;
   param1->uGet4Mapping[13] = 13;
   param1->uGet4Mapping[14] = 14;
   param1->uGet4Mapping[15] = 15;
   // Activated Get4 chips (indexes after remapping => not hardware map !)
   param1->uGet4Active[ 0] = 1;
   param1->uGet4Active[ 1] = 1;
   param1->uGet4Active[ 2] = 1;
   param1->uGet4Active[ 3] = 1;
   param1->uGet4Active[ 4] = 1;
   param1->uGet4Active[ 5] = 1;
   param1->uGet4Active[ 6] = 1;
   param1->uGet4Active[ 7] = 1;
   param1->uGet4Active[ 8] = 0;
   param1->uGet4Active[ 9] = 0;
   param1->uGet4Active[10] = 0;
   param1->uGet4Active[11] = 0;
   param1->uGet4Active[12] = 0;
   param1->uGet4Active[13] = 0;
   param1->uGet4Active[14] = 0;
   param1->uGet4Active[15] = 0;
   // Edges inversion: affect all channels of a GET4 chip
   param1->uGet4EdgeInversion[ 0] = 0;
   param1->uGet4EdgeInversion[ 1] = 0;
   param1->uGet4EdgeInversion[ 2] = 0;
   param1->uGet4EdgeInversion[ 3] = 0;
   param1->uGet4EdgeInversion[ 4] = 0;
   param1->uGet4EdgeInversion[ 5] = 0;
   param1->uGet4EdgeInversion[ 6] = 0;
   param1->uGet4EdgeInversion[ 7] = 0;
   param1->uGet4EdgeInversion[ 8] = 0;
   param1->uGet4EdgeInversion[ 9] = 0;
   param1->uGet4EdgeInversion[10] = 0;
   param1->uGet4EdgeInversion[11] = 0;
   param1->uGet4EdgeInversion[12] = 0;
   param1->uGet4EdgeInversion[13] = 0;
   param1->uGet4EdgeInversion[14] = 0;
   param1->uGet4EdgeInversion[15] = 0;
   // Remove all printouts => Blind mode!
   param1->uSilentMode        = 0;
    // No Online Change - Enable/Disable general debug histograms
   param1->uDebugHistoOn      = 1;
   // Activate lots of messages for debug
   param1->uDebugPrintOn      = 0;
   // Number of data messages to be printed on screen for debug
   param1->uGet4Print         = 300;

   // Enable Tot histograms
   param1->bTotHistoEnable    = kTRUE;
   // Enable the time difference, finetime and DNL histograms
   param1->bDebugHistoEnable  = kTRUE;
   // Nb of events between 2 updates of the DNL histograms
   param1->uNbEventsDnlUpdate = 100;
   // Index of first chip to be used for time difference histos
   param1->uGet4TimeDiffChip1 = 0;
   // Index of first chip to be used for time difference histos
   param1->uGet4TimeDiffChip2 = 5;


   // kFALSE = Sync trigger/no trigger, kTRUE= online detection/selection from data
   param1->bFreeStreaming = kFALSE;
   // Number of user defined data triggers
   param1->uNbTriggers = 1;
   // Nb of secondary channels to use for event detection/selection
   param1->uNbSecondarySelectionCh[ 0] = 3; // PMT Full AND
   param1->uNbSecondarySelectionCh[ 1] = 1; // RPC ch0 AND
   param1->uNbSecondarySelectionCh[ 2] = 1; // RPC ch1 AND
   param1->uNbSecondarySelectionCh[ 3] = 1; // RPC ch2 AND
   param1->uNbSecondarySelectionCh[ 4] = 1; // RPC ch3 AND
   param1->uNbSecondarySelectionCh[ 5] = 1; // RPC ch4 AND
   param1->uNbSecondarySelectionCh[ 6] = 1; // RPC ch5 AND
   param1->uNbSecondarySelectionCh[ 7] = 1; // RPC ch6 AND
   param1->uNbSecondarySelectionCh[ 8] = 1; // RPC ch7 AND
   param1->uNbSecondarySelectionCh[ 9] = 1; // PMT1 AND
   param1->uNbSecondarySelectionCh[10] = 1; // PMT2 AND
   // TDC index of the main signal for online selection. Selection window is relative to this
   param1->uMainSelectionTdc[ 0] =  7; // PMT Full AND
   param1->uMainSelectionTdc[ 1] =  0; // RPC ch0 AND
   param1->uMainSelectionTdc[ 2] =  0; // RPC ch1 AND
   param1->uMainSelectionTdc[ 3] =  0; // RPC ch2 AND
   param1->uMainSelectionTdc[ 4] =  0; // RPC ch3 AND
   param1->uMainSelectionTdc[ 5] =  1; // RPC ch4 AND
   param1->uMainSelectionTdc[ 6] =  1; // RPC ch5 AND
   param1->uMainSelectionTdc[ 7] =  1; // RPC ch6 AND
   param1->uMainSelectionTdc[ 8] =  1; // RPC ch7 AND
   param1->uMainSelectionTdc[ 9] =  7; // PMT1 AND
   param1->uMainSelectionTdc[10] =  7; // PMT2 AND
   // Channel of the main signal for online selection. Selection window is relative to this
   param1->uMainSelectionChannel[ 0] = 0; // PMT Full AND
   param1->uMainSelectionChannel[ 1] = 0; // RPC ch0 AND
   param1->uMainSelectionChannel[ 2] = 1; // RPC ch1 AND
   param1->uMainSelectionChannel[ 3] = 2; // RPC ch2 AND
   param1->uMainSelectionChannel[ 4] = 3; // RPC ch3 AND
   param1->uMainSelectionChannel[ 5] = 0; // RPC ch4 AND
   param1->uMainSelectionChannel[ 6] = 1; // RPC ch5 AND
   param1->uMainSelectionChannel[ 7] = 2; // RPC ch6 AND
   param1->uMainSelectionChannel[ 8] = 3; // RPC ch7 AND
   param1->uMainSelectionChannel[ 9] = 0; // PMT1 AND
   param1->uMainSelectionChannel[10] = 2; // PMT2 AND
   // TDC index of the secondary signals for online selection.
   param1->uSecondarySelectionTdc[ 0][0] =  7; // PMT Full AND
   param1->uSecondarySelectionTdc[ 0][1] =  7; // PMT Full AND
   param1->uSecondarySelectionTdc[ 0][2] =  7; // PMT Full AND
   param1->uSecondarySelectionTdc[ 1][0] =  3; // RPC ch0 AND
   param1->uSecondarySelectionTdc[ 2][0] =  3; // RPC ch1 AND
   param1->uSecondarySelectionTdc[ 3][0] =  3; // RPC ch2 AND
   param1->uSecondarySelectionTdc[ 4][0] =  3; // RPC ch3 AND
   param1->uSecondarySelectionTdc[ 5][0] =  2; // RPC ch4 AND
   param1->uSecondarySelectionTdc[ 6][0] =  2; // RPC ch5 AND
   param1->uSecondarySelectionTdc[ 7][0] =  2; // RPC ch6 AND
   param1->uSecondarySelectionTdc[ 8][0] =  2; // RPC ch7 AND
   param1->uSecondarySelectionTdc[ 9][0] =  7;
   param1->uSecondarySelectionTdc[10][0] =  7;
   // Channel of the secondary signals for online selection.
   param1->uSecondarySelectionChannel[ 0][0] = 1; // PMT Full AND
   param1->uSecondarySelectionChannel[ 0][1] = 2; // PMT Full AND
   param1->uSecondarySelectionChannel[ 0][2] = 3; // PMT Full AND
   param1->uSecondarySelectionChannel[ 1][0] = 3; // RPC ch0 AND
   param1->uSecondarySelectionChannel[ 2][0] = 2; // RPC ch1 AND
   param1->uSecondarySelectionChannel[ 3][0] = 1; // RPC ch2 AND
   param1->uSecondarySelectionChannel[ 4][0] = 0; // RPC ch3 AND
   param1->uSecondarySelectionChannel[ 5][0] = 3; // RPC ch4 AND
   param1->uSecondarySelectionChannel[ 6][0] = 2; // RPC ch5 AND
   param1->uSecondarySelectionChannel[ 7][0] = 1; // RPC ch6 AND
   param1->uSecondarySelectionChannel[ 8][0] = 0; // RPC ch7 AND
   param1->uSecondarySelectionChannel[ 9][0] = 1; // PMT1 AND
   param1->uSecondarySelectionChannel[10][0] = 3; // PMT2 AND
   // Beginning of time window in which secondary channels have to be present, relative to main channel
   param1->dCoincidenceWindowStart[ 0] = -150;
   param1->dCoincidenceWindowStart[ 1] = -150;
   param1->dCoincidenceWindowStart[ 2] = -150;
   param1->dCoincidenceWindowStart[ 3] = -150;
   param1->dCoincidenceWindowStart[ 4] = -150;
   param1->dCoincidenceWindowStart[ 5] = -150;
   param1->dCoincidenceWindowStart[ 6] = -150;
   param1->dCoincidenceWindowStart[ 7] = -150;
   param1->dCoincidenceWindowStart[ 8] = -150;
   param1->dCoincidenceWindowStart[ 9] = -150;
   param1->dCoincidenceWindowStart[10] = -150;
   // End of time window in which secondary channels have to be present, relative to main channel
   param1->dCoincidenceWindowStop[ 0] = 150;
   param1->dCoincidenceWindowStop[ 1] = 150;
   param1->dCoincidenceWindowStop[ 2] = 150;
   param1->dCoincidenceWindowStop[ 3] = 150;
   param1->dCoincidenceWindowStop[ 4] = 150;
   param1->dCoincidenceWindowStop[ 5] = 150;
   param1->dCoincidenceWindowStop[ 6] = 150;
   param1->dCoincidenceWindowStop[ 7] = 150;
   param1->dCoincidenceWindowStop[ 8] = 150;
   param1->dCoincidenceWindowStop[ 9] = 150;
   param1->dCoincidenceWindowStop[10] = 150;
   // Minimal time between the main channel of two identical triggers in ns
   param1->dDeadTime = 300;
   // kFALSE = 1 hit => max 1 event of same trigger, 2nd event ignored ( ~Deadtime )
   // kTRUE  = 1 hit => all events matching trigger window inside MBS event as long as same trigger
   // TODO: implement this flag usage
   param1->bAllowEventMixing = kFALSE;
      /*************************************/

   /************** Detectors ************/
   // No Online Change - TDC used for the reference signals profiles in Detector classes, -1 disable it
   param1->iMainReferenceTdc  = -1;
   // No Online Change - Channel used for the 1st reference histogram in Detector classes, -1 disable it
   param1->iReference1Channel = -1;
   // No Online Change - Channel used for the 2nd reference histogram in Detector classes, -1 disable it
   param1->iReference2Channel = -1;
   /*************************************/
}
