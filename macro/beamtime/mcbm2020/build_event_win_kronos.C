void build_event_win_kronos( UInt_t uRunIdx = 0, Int_t nEvents = 0, TString outDir="data/")
{
  UInt_t uRunId = 0;
  TString fileName = "data/unp_mcbm_0.root";
  if( 99999 != uRunIdx )
  {
      std::vector< UInt_t > vuListRunId = {
             692, 698, 702, 704, 705, 706, 707,                //  7 =>  0 -  6
             744, 750, 759, 760, 761, 762, 799,                //  7 =>  7 - 13
             811, 812, 816, 817, 819,                     //  5 => 14 - 18
             820, 821, 822, 824, 826, 827, 828, 829,      //  8 => 19 - 26
             830, 831, 836,                               //  3 => 27 - 29
             841, 846, 849,                               //  3 => 30 - 32
             850, 851, 852, 854, 855, 856, 857, 858, 859, //  9 => 33 - 41
             860, 861, 862, 863, 864, 865, 866            //  7 => 42 - 48
/*
 /// With runs < 1 min due to missmatch!
             811, 812, 816, 817, 818, 819,                     //  6 => 14 - 19
             820, 821, 822, 824, 826, 827, 828, 829,           //  8 => 20 - 27
             830, 831, 836, 839,                               //  4 => 28 - 31
             840, 841, 842, 844, 845, 846, 848, 849,           //  8 => 32 - 39
             850, 851, 852, 854, 855, 856, 857, 858, 859,      //  9 => 40 - 48
             860, 861, 862, 863, 864, 865, 866                 //  7 => 49 - 55
*/
         };
      if( vuListRunId.size() <= uRunIdx )
         return kFALSE;
      uRunId = vuListRunId[ uRunIdx ];
      fileName = Form( "data/unp_mcbm_%03u.root", uRunId );
  } // if( 99999 != uRunIdx )

  if( uRunId < 692 && 0 != uRunId )
    return kFALSE;


  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
//  Int_t iVerbose = 1;

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
//  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // MC file

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
//  FairRunOnline *fRun= new FairRunOnline();
  FairRunAna *fRun= new FairRunAna();
  fRun->SetEventHeaderPersistence(kFALSE);

  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);

  TString runId = TString::Format("%03u", uRunId);
  TString outFile = outDir + "/events_win_" + runId + ".root";
  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  fRun->SetSink(outputSink);

  // Define output file for FairMonitor histograms
//  TString monitorFile{outFile};
//  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
//  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

//  CbmMcbm2019TimeWinEventBuilder* eventBuilder = new CbmMcbm2019TimeWinEventBuilder();
  CbmMcbm2019TimeWinEventBuilderTask* eventBuilder = new CbmMcbm2019TimeWinEventBuilderTask();

  eventBuilder->SetFillHistos( kTRUE );

  eventBuilder->SetEventOverlapMode( EOverlapMode::NoOverlap );

  eventBuilder->SetTriggerWindow( ECbmModuleId::kSts,   -50, 100 );
  eventBuilder->SetTriggerWindow( ECbmModuleId::kMuch, -150,  50 );
  eventBuilder->SetTriggerWindow( ECbmModuleId::kTrd,  -250, 100 );
  eventBuilder->SetTriggerWindow( ECbmModuleId::kTof,  -150,  10 );
  eventBuilder->SetTriggerWindow( ECbmModuleId::kRich, -150,  20 );
  eventBuilder->SetTriggerWindow( ECbmModuleId::kPsd,   -50,  10 );
//  eventBuilder->SetT0PulserTotLimits(   185, 191 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kT0,    1 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kSts,   0 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kMuch,  0 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kTrd,   0 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kTof,  10 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kRich,  0 );
  eventBuilder->SetTriggerMinNumber( ECbmModuleId::kPsd,   0 );
  if( 0 < uRunId )
    eventBuilder->SetOutFilename( Form( "%sHistosEvtWin_%03u.root", outDir.Data(), uRunId ) );

  // To get T0 Digis (seed + close digis) in the event
  eventBuilder->SetTriggerWindow( ECbmModuleId::kT0,   -1, 10 );

  fRun->AddTask(eventBuilder);

  // -----  Parameter database   --------------------------------------------
//  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
//  FairParRootFileIo* parIo1 = new FairParRootFileIo();
//  parIo1->open(parFile.Data(),"UPDATE");
//  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------


  // -----   Intialise and run   --------------------------------------------
  fRun->Init();

//  rtdb->setOutput(parIo1);
//  rtdb->saveOutput();
//  rtdb->print();

  cout << "Starting run" << endl;
  if ( 0 == nEvents) {
    fRun->Run(0, 0); // run until end of input file
  } else {
    fRun->Run(0, nEvents); // process  N Events
  }
  // ------------------------------------------------------------------------



  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory=sysInfo.GetMaxMemory();
  cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  cout << maxMemory;
  cout << "</DartMeasurement>" << endl;

  Float_t cpuUsage=ctime/rtime;
  cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  cout << cpuUsage;
  cout << "</DartMeasurement>" << endl;

  FairMonitor* tempMon = FairMonitor::GetMonitor();
  tempMon->Print();

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
