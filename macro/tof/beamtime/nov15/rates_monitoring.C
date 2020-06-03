// -----------------------------------------------------------------------------
// -----                                                                   ----- 
// ----- intended use: unpacking of rate data acquired with MBS            -----
// -----               during the CERN Feb. 2015 beamtime                  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void rates_monitoring()
{
   // Specify log level (INFO, DEBUG, DEBUG1, ...)
//   TString logLevel = "FATAL";
   //TString logLevel = "ERROR";
   TString logLevel = "INFO";
//   TString logLevel = "DEBUG";
   //TString logLevel = "DEBUG1";
//   TString logLevel = "DEBUG2";
   //TString logLevel = "DEBUG3";

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString paramDir = gSystem->Getenv("PWD");

   TObjString unpParFile = paramDir + "/parUnpack_rates.txt";
   parFileList->Add(&unpParFile);
   TObjString calParFile = paramDir + "/parCalib_rates.txt";
   parFileList->Add(&calParFile);

   // Parameter file output
   TString parFile = paramDir + "/out/unpack_rate_moni.params.root";
   
   // Output file
   TString outFile = paramDir + "/out/unpack_rate_moni.out.root";
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
//   FairLmdSource* source = new FairLmdSource();
//   source->AddFile( sLmdFolder + sLmdName + ".lmd" );
//   FairMbsStreamSource* source = new FairMbsStreamSource("localhost");
//   FairMbsStreamSource* source = new FairMbsStreamSource("rio4-1");
   FairRemoteSource* source = new FairRemoteSource("mbsdaq");

   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
   //tofTriglogUnpacker->SetSaveTriglog(kTRUE);
   tofTriglogUnpacker->SetSaveTriglog(kFALSE);
   cout<<tofTriglogUnpacker<<endl;
   source->AddUnpacker( tofTriglogUnpacker );
  
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
   tofMbsDataUnpacker->SetSaveScalers(kFALSE);
   cout<<tofMbsDataUnpacker<<endl;
   source->AddUnpacker( tofMbsDataUnpacker );

   // -----   Online/Offline MBS run   -----------------------------------------
//   FairRunOnline *run = new FairRunOnline();
//   FairRunOnline *run = FairRunOnline::Instance();
   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   gLogger->SetLogScreenLevel(logLevel.Data());

   // -------------------------------------------------------------------------
   // ===                        End of Unpacker                            ===
   // =========================================================================
   
   // =========================================================================
   // ===                     Unpacker monitoring                           ===
   // =========================================================================
   TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", 2);
   run->AddTask(tofUnpMonitor);
   // ===                 End of Unpacker monitoring                        ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", 2);
   tofCalibration->SetSaveScalers(kFALSE);
   tofCalibration->SetScalHistoUserAxis( 3600.0, 1.0 ); // duration [s], bin size [s]
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // ========================================================================= 

   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->MonitorTdcOccupancy(kFALSE);
   display->SetNumberOfTDC(0);
   display->SetNumberOfSEB(0);
   display->SetUpdateInterval(2);
   display->MonitorCts(kFALSE);
   display->MonitorSebStatus(kFALSE);
   display->MonitorTdcStatus(kFALSE);
   display->MonitorFSMockup(kFALSE);
   display->MonitorDigiStatus(kFALSE);
   display->SetResMonitorEna(kFALSE);
   display->SetRateMonitorEna(kTRUE);
   display->SetFreeTrloNames( "HD-P5", "HD-P2", "PAD  ", "BREF ", 
                              "B12-1", "B12-2", "B12-3", "B12-4", 
                              "BUC15", "DIAM ", "BOX31", "BOX32", 
                              "BOX21", "BOX22", "BOX11", "BOX12" );
   display->RatesSlidingScaleEna(kTRUE);
   run->AddTask(display); 

   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   
   
   Bool_t kParameterMerged = kTRUE;
   FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
   parIo2->open(parFile.Data(), "UPDATE");
   parIo2->print();
   rtdb->setFirstInput(parIo2);
   
   FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
   parIo1->open(parFileList, "in");
   parIo1->print();
//   rtdb->setFirstInput(parIo1);
   rtdb->setSecondInput(parIo1);
   
   rtdb->print();
   rtdb->printParamContexts();
   // ------------------------------------------------------------------------

   // -----   Intialise and run   --------------------------------------------
   run->Init();
   // ------------------------------------------------------------------------

/*
   cout << "Starting run" << endl;
   run->Run(nEvents, 0);

   run->Finish();

   TString sMoveCmd = "mv tofMbs";
   gSystem->Exec( sMoveCmd + "Cal.hst.root out/tofMbsCal_" + sLmdName + ".hst.root" );
   gSystem->Exec( sMoveCmd + "Unp.hst.root out/tofMbsUnp_" + sLmdName + ".hst.root" );

//   delete run;
*/
   return;
}

