// -----------------------------------------------------------------------------
// ----- unpack_trb FairRoot macro                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-08                                 -----
// -----                                                                   -----
// ----- based on unpack_run36_Fair.C by P.-A. Loizeau                     -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/macro/tof/beamtime/unpack_run36_Fair.C                     -----
// ----- revision 21398, 2013-08-26                                        -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void setup_trb() 
{
   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString workDir = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = workDir + "/macro/tof/beamtime/apr14";

   TObjString unpParFile = paramDir + "/parUnpackLmdTrb.txt";
   parFileList->Add(&unpParFile);
   TObjString calParFile = paramDir + "/parCalGsiApr14.txt";
   parFileList->Add(&calParFile);
/*
   TObjString mapParFile = paramDir + "/parMapGsi12Run36.txt";
   parFileList->Add(&mapParFile);
   TObjString convParFile = paramDir + "/parConvGsi12Run36.txt";
   parFileList->Add(&convParFile);
*/ 
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
   source->AddFile("./MbsTrbHdFee6_0003.lmd");

/*
   Int_t iNbTestFiles = 85;
   Int_t iStartIndex = 1;
   for(Int_t iFileIndex = 0; iFileIndex < iNbTestFiles; iFileIndex++)
      source->AddFile( Form("/home/palsec/trbData/MbsTrb2Pul_%04d.lmd", iStartIndex+iFileIndex));
*/

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
   // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
   Int_t iVerbose = 3; // Not working, to remove from classes!

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
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
//   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
//   run->AddTask(tofMapping);
   // ===                      End of Mapping                               ===
   // =========================================================================
   
   // =========================================================================
   // ===                  GO4 like output formatting                       ===
   // =========================================================================
//   TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
//   run->AddTask(tofConversion);
   // ===                  End of output conversion                         ===
   // =========================================================================
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000);
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
   cout << "Starting run" << endl;
//   run->Run(nEvents, 0);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------
/*
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished successfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;
*/
   // ------------------------------------------------------------------------

//   delete run;
}

