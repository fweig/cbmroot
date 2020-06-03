// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
// P.-A. Loizeau   09/05/2013 
// --------------------------------------------------------------------------

// Max nEvents: 198999999999
void unpack_run36_Fair(Int_t nEvents = 2) 
{
   // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
   Int_t iVerbose = 3;

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString workDir = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = workDir + "/macro/tof/beamtime";

   TObjString unpParFile = paramDir + "/parUnpackLmdRun36.txt";
   parFileList->Add(&unpParFile);
   TObjString calParFile = paramDir + "/parCalGsi12Run36.txt";
   parFileList->Add(&calParFile);
   TObjString mapParFile = paramDir + "/parMapGsi12Run36.txt";
   parFileList->Add(&mapParFile);
   TObjString convParFile = paramDir + "/parConvGsi12Run36.txt";
   parFileList->Add(&convParFile);
   
   // Parameter file output
   TString parFile = paramDir + "/unpack.params.root";
   
   // Output file
   TString outFile = paramDir+"/unpack.out.root";
   
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
   FairLmdSource* source = new FairLmdSource();
   source->AddFile("/buffalo/gsi2012/nov12/run36_20121105_0102.lmd");
   
   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof( iVerbose );
   source->AddUnpacker( tofTriglogUnpacker );
   
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof( iVerbose );
   source->AddUnpacker( tofMbsDataUnpacker );

      // -----   Online/Offline MBS run   -----------------------------------
   FairRunOnline *run = new FairRunOnline(source);
   run->SetOutputFile(outFile);
      // --------------------------------------------------------------------
   // ===                        End of Unpacker                            ===
   // =========================================================================
   
   // =========================================================================
   // ===                     Unpacker monitoring                           ===
   // =========================================================================
   TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", iVerbose);
   run->AddTask(tofUnpMonitor);
   // ===                 End of Unpacker monitoring                        ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   run->AddTask(tofMapping);
   // ===                      End of Mapping                               ===
   // =========================================================================
   
   // =========================================================================
   // ===                  GO4 like output formatting                       ===
   // =========================================================================
   TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
   run->AddTask(tofConversion);
   // ===                  End of output conversion                         ===
   // =========================================================================
   
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
   cout << "Starting run" << endl;
//   run->Run(0, nEvents);
   run->Run(nEvents, 0); // <= FAIRROOT guy style, crash for me on event finish
//   run->RunOnLmdFiles(0, nEvents);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished succesfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;
   // ------------------------------------------------------------------------

//   delete run;

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
}

