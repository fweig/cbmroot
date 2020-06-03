// -----------------------------------------------------------------------------
// -----                                                                   ----- 
// ----- intended use: unpacking of rate data acquired with MBS            -----
// -----               during the CERN Feb. 2015 beamtime                  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void rates_unpacking(TString sLmdFolder = "", TString sLmdName = "", Int_t nEvents = -1)
{
   if( "" == sLmdName )
   {
      cout<<"Empty LMD filename!!!"<<endl;
      return;
   }

   // Specify log level (INFO, DEBUG, DEBUG1, ...)
//   TString logLevel = "FATAL";
   //TString logLevel = "ERROR";
   TString logLevel = "INFO";
   //TString logLevel = "DEBUG";
   //TString logLevel = "DEBUG1";
   //TString logLevel = "DEBUG2";
   //TString logLevel = "DEBUG3";

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString paramDir = gSystem->Getenv("PWD");

   TObjString * unpParFile = new TObjString( paramDir + "/parUnpack_rates.txt" );
   parFileList->Add(unpParFile);
   TObjString * calParFile = new TObjString( paramDir + "/parCalib_rates.txt" );
   parFileList->Add(calParFile);

   // Parameter file output
   TString parFile = paramDir + "/out/unpack_" + sLmdName + ".params.root";
   
   // Output file
   TString outFile = paramDir + "/out/unpack_" + sLmdName + ".out.root";
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
   FairLmdSource* source = new FairLmdSource();
   source->AddFile( sLmdFolder + sLmdName + ".lmd" );
//   FairMbsStreamSource* source = new FairMbsStreamSource("localhost");

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
   FairRunOnline *run = new FairRunOnline();
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
   tofCalibration->SetSaveScalers(kTRUE);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
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
   // ------------------------------------------------------------------------


   cout << "Starting run" << endl;
   run->Run(nEvents, 0);

   run->Finish();

   TString sMoveCmd = "mv tofMbs";
   gSystem->Exec( sMoveCmd + "Cal.hst.root out/tofMbsCal_" + sLmdName + ".hst.root" );
   gSystem->Exec( sMoveCmd + "Unp.hst.root out/tofMbsUnp_" + sLmdName + ".hst.root" );

//   delete run;
   return;
}

