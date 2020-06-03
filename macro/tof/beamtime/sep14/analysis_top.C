// -----------------------------------------------------------------------------
// ----- analysis.C                                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-08-22                                 -----
// -----                                                                   -----
// ----- based on unpack_trb.C by C. Simon                                 -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/csimon/    -----
// -----   beamtimeApr14/macro/tof/beamtime/unpack_trb.C                   -----
// ----- revision 24438, 2014-04-13                                        -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void analysis_top(Int_t nEvents = 2) 
{
   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString paramDir = gSystem->Getenv("PWD");

   TObjString unpParFile = paramDir + "/parUnpackTop.txt";
   parFileList->Add(&unpParFile);

   TObjString calParFile = paramDir + "/parCalibTop.txt";
   parFileList->Add(&calParFile);

   TObjString mapParFile = paramDir + "/parMapTop.txt";
   parFileList->Add(&mapParFile);

   // Parameter file output
   TString parFile = paramDir + "/unpack.params.root";
   
   // Output file
   TString outFile = paramDir + "/unpack.out.root";
   
   // ----    Debug option   -------------------------------------------------
   gDebug = 0;
   // ------------------------------------------------------------------------

   // -----   Timer   --------------------------------------------------------
   TStopwatch timer;
   timer.Start();
   // ------------------------------------------------------------------------
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
//  FairLmdSource* source = new FairLmdSource();
//  source->AddFile("/local/beam/sep14/macro/tof/beamtime/sep14/data/MbsTrbSat1805_0003.lmd"); // V

   FairMbsStreamSource* source = new FairMbsStreamSource("tofrepeater");
//   FairMbsStreamSource* source = new FairMbsStreamSource("tofdaq");
   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
   source->AddUnpacker( tofTriglogUnpacker );

   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
   source->AddUnpacker( tofMbsDataUnpacker );

   // = 1 TRB3: TTrbUnpackTof(0,0,31,0,0)
   // > 1 TRB3: TTrbUnpackTof(10,1,31,0,0)

   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );

      // -----   Online/Offline MBS run   -----------------------------------
   FairRunOnline *run = FairRunOnline::Instance();
   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   gLogger->SetLogScreenLevel("INFO");
//   gLogger->SetLogScreenLevel("DEBUG");

      // --------------------------------------------------------------------
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
   display->SetUpdateInterval(2000);
   display->SetNumberOfTDC(38);
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
//   cout << "Starting run" << endl;
//   run->Run(nEvents, 0);
   // ------------------------------------------------------------------------

//   gLogger->SetLogScreenLevel("FATAL");

   // -----   Finish   -------------------------------------------------------
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished successfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;
   // ------------------------------------------------------------------------

//   delete run;
}

