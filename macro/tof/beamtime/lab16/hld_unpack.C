// -----------------------------------------------------------------------------
// ----- $Id::                                                           $ -----
// -----                                                                   ----- 
// ----- intended use: code development and debugging                      -----
// -----               of TRB raw-data unpacking and calibration routines  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void hld_unpack(Int_t nEvents = 2) 
{
   // Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString paramDir = gSystem->Getenv("PWD");

   TObjString unpParFile = paramDir + "/parUnpack_basic.txt";
   parFileList->Add(&unpParFile);
   TObjString calParFile = paramDir + "/parCalib_basic.txt";
   parFileList->Add(&calParFile);
   TObjString mapParFile = paramDir + "/parMapLab16.txt";
//   parFileList->Add(&mapParFile);

   // Parameter file output
   TString parFile = paramDir + "/unpack.params.root";
   
   // Output file
   TString outFile = paramDir + "/unpack.out.root";
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
   CbmHldSource* source = new CbmHldSource();
   source->AddFile("./HLD/CbmTofPiHd_19Feb1812_16050181255.hld");
   source->AddFile("./HLD/CbmTofPiHd_19Feb1812_16051054603.hld");
   source->AddFile("./HLD/CbmTofPiHd_19Feb1812_16051111623.hld");
   source->AddFile("./HLD/CbmTofPiHd_19Feb1812_16051165102.hld");

   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );

   // --------------------   Online/Offline MBS run   -------------------------
   FairRunOnline *run = FairRunOnline::Instance();
   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   gLogger->SetLogScreenLevel("INFO");

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
   tofCalibration->SetSaveTdcs(kTRUE);
   run->AddTask(tofCalibration);
   
   // ===                      End of Calibration                           ===
   // =========================================================================



   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", 2);
   run->AddTask(tofMapping);
   
   // ===                      End of Mapping                               ===
   // =========================================================================



   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetNumberOfTDC(38);
   display->SetNumberOfSEB(5);
   display->SetUpdateInterval(1000);
   display->MonitorCts(kTRUE);
   display->MonitorSebStatus(kTRUE);
   display->MonitorTdcStatus(kTRUE);
   // run->AddTask(display);   



   // =========================================================================
   // ===                       Parameter Database                          ===
   // =========================================================================
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   Bool_t kParameterMerged = kTRUE;
   FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
   parIo2->open(parFile.Data(), "UPDATE");
   parIo2->print();
   rtdb->setFirstInput(parIo2);
   
   FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
   parIo1->open(parFileList, "in");
   parIo1->print();
   // rtdb->setFirstInput(parIo1);
   rtdb->setSecondInput(parIo1);
   
   rtdb->print();
   rtdb->printParamContexts();
   
   // ===                      				                                ===
   // =========================================================================




   // =========================================================================
   // ===                       Intialise and run                           ===
   // =========================================================================

   run->Init();
   
   // ------------------------------------------------------------------------

}

