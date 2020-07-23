void build_event_win(UInt_t uRunId  = 0,
                     Int_t nEvents  = 0,
                     TString outDir = "data/") {
  TString fileName = Form("data/unp_mcbm_%03u.root", uRunId);

  if (uRunId < 692) return kFALSE;


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
  FairRunAna* fRun = new FairRunAna();
  fRun->SetEventHeaderPersistence(kFALSE);

  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);

  TString runId                = TString::Format("%03u", uRunId);
  TString outFile              = outDir + "/events_win_" + runId + ".root";
  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  fRun->SetSink(outputSink);

  // Define output file for FairMonitor histograms
  //  TString monitorFile{outFile};
  //  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  //  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

  //  CbmMcbm2019TimeWinEventBuilder* eventBuilder = new CbmMcbm2019TimeWinEventBuilder();
  CbmMcbm2019TimeWinEventBuilderTask* eventBuilder =
    new CbmMcbm2019TimeWinEventBuilderTask();

  eventBuilder->SetFillHistos(kTRUE);

  eventBuilder->SetEventOverlapMode(EOverlapMode::NoOverlap);
  //  eventBuilder->SetEventOverlapMode(EOverlapMode::MergeOverlap);
  //  eventBuilder->SetEventOverlapMode(EOverlapMode::AllowOverlap);

  /*
 * Available Pre-defined detectors:
 * kEventBuilderDetSts
 * kEventBuilderDetMuch
 * kEventBuilderDetTrd
 * kEventBuilderDetTof
 * kEventBuilderDetRich
 * kEventBuilderDetPsd
 * kEventBuilderDetT0
 */

  /// Change the selection window limits for T0 as ref
  eventBuilder->SetTriggerWindow(ECbmModuleId::kSts, -50, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kMuch, -150, 50);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTrd, -250, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTof, -150, 10);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kRich, -150, 20);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kPsd, -50, 10);
  /// To get T0 Digis (seed + close digis) in the event
  eventBuilder->SetTriggerWindow(ECbmModuleId::kT0, -1, 10);

  /*
  /// Use TOF as reference
  eventBuilder->SetReferenceDetector( kEventBuilderDetTof );
  eventBuilder->AddDetector( kEventBuilderDetT0 );

  /// Change the selection window limits for TOF as ref
  /// => Should always be after changes of detector lists!
  eventBuilder->SetTriggerWindow(ECbmModuleId::kT0, -150, 0);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kSts, -50, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kMuch, -50, 200);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTrd, -50, 300);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTof, 0, 60);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kRich, -100, 150);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kPsd, -200, 50);
*/

  /// Change the trigger requirements
  /// => Should always be after changes of detector lists!
  /// --- Minimum
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kT0, 1);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kSts, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kMuch, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kTrd, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kTof, 10);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kRich, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kPsd, 0);
  /// --- Maximum  (-1 to disable cut)
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kT0, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kMuch, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kTrd, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kPsd, -1);


  if (0 < uRunId)
    eventBuilder->SetOutFilename(
      Form("%sHistosEvtWin_%03u.root", outDir.Data(), uRunId));

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
  if (0 == nEvents) {
    fRun->Run(0, 0);  // run until end of input file
  } else {
    fRun->Run(0, nEvents);  // process  N Events
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
  Float_t maxMemory = sysInfo.GetMaxMemory();
  cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  cout << maxMemory;
  cout << "</DartMeasurement>" << endl;

  Float_t cpuUsage = ctime / rtime;
  cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  cout << cpuUsage;
  cout << "</DartMeasurement>" << endl;

  FairMonitor* tempMon = FairMonitor::GetMonitor();
  tempMon->Print();

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
