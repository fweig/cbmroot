// -----------------------------------------------------------------------------
// ----- $Id::                                                           $ -----
// -----                                                                   ----- 
// ----- intended use: code development and debugging                      -----
// -----               of TRB raw-data unpacking and calibration routines  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

FairRunOnline *run = new FairRunOnline ();

// Max nEvents: 198999999999
void tdcref_monitoring_auto(Int_t nEvents = 100000, char *cFileId="CbmTofSps_28Nov0041", Bool_t bCalibRun = kFALSE) 
{
   // Specify log level (INFO, DEBUG, DEBUG1, ...)
   TString logLevel = "FATAL";
   logLevel = "ERROR";
   logLevel = "INFO";
//   logLevel = "DEBUG";
//   logLevel = "DEBUG1";
//   logLevel = "DEBUG2";
//   logLevel = "DEBUG3";

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString paramDir = gSystem->Getenv("PWD");

//   TObjString unpParFile = paramDir + "/parUnpCernNov2015_old.txt";
   TObjString unpParFile = paramDir + "/parUnpCernNov2015.txt";
   parFileList->Add(&unpParFile);
   TObjString calParFile = paramDir + "/parCalib_tdcres.txt";
   parFileList->Add(&calParFile);
   TObjString mapParFile = paramDir + "/parMapCernNov2015.txt";
//   parFileList->Add(&mapParFile);

   // Parameter file output
   TString parFile = paramDir + "/unpack.params.root";
   
   // Output file
   TString outFile = paramDir + "/unpack.out.root";
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
//   FairLmdSource* source = new FairLmdSource();
//   source->AddFile("/home/cbmlyserv/DataTemp/CbmTofSps_24Nov2047_0001.lmd");
//   source->AddFile("/home/cbmlyserv/DataTemp/CbmTofSps_26Nov0022_0000.lmd");
//   source->AddFile("/home/cbmlyserv/DataTemp/CbmTofSps_26Nov0043_0065.lmd");
//   FairMbsStreamSource* source = new FairMbsStreamSource("trbdaq:21704");

   CbmHldSource* source = new CbmHldSource();
   source->AddPath("/hera/cbm/users/tofGsiApr14/cern-nov15/production/",Form("%s*.hld",cFileId));
//   source->AddFile("/home/cbmlyserv/DataTemp/CbmTofSps_26Nov1421_15330142142.hld");

   // = 1 TRB3: TTrbUnpackTof(0,0,31,0,0)
   // > 1 TRB3: TTrbUnpackTof(10,1,31,0,0)

   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );

   // -----   Online/Offline MBS run   -----------------------------------------
   FairRunOnline *run = FairRunOnline::Instance();
   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   gLogger->SetLogScreenLevel(logLevel);

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
//   tofCalibration->SetSaveTdcs(kTRUE);
   tofCalibration->EnaTdcRefMoniMode(kTRUE);
   tofCalibration->SetTdcCalibOutFoldername("./");
   tofCalibration->SetTdcCalibFilename("auto_calib_tdcref");
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================

   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", 2);
//   run->AddTask(tofMapping);
   // ===                      End of Mapping                               ===
   // =========================================================================

   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->MonitorTdcOccupancy(kTRUE);
   display->SetNumberOfTDC(38);
   display->SetNumberOfSEB(5);
   display->SetUpdateInterval(50000);
   display->MonitorCts(kTRUE);
   display->MonitorSebStatus(kFALSE);
   display->MonitorTdcStatus(kFALSE);
   display->MonitorFSMockup(kFALSE);
   display->MonitorDigiStatus(kFALSE);
   display->SetResMonitorEna(kTRUE);
   display->SetRateMonitorEna(kFALSE);
   if( kFALSE == bCalibRun )
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

   run->Run(nEvents);
   if( kTRUE == bCalibRun )
      run->Finish();
}

