void check_timing(TString fileName,
                  UInt_t uRunId  = 0,
                  Int_t nEvents  = 0,
                  TString outDir = "data/") {

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 1;

  // MC file

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
  FairRunOnline* fRun = new FairRunOnline();
  fRun->ActivateHttpServer(100, 8080);  // refresh each 100 events

  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);
  fRun->SetSink(new FairRootFileSink( "SinkFile.root" ));
  // Define output file for FairMonitor histograms
  //  TString monitorFile{outFile};
  //  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------

  CbmCheckTiming* timeChecker = new CbmCheckTiming();
  timeChecker->SetCheckInterSystemOffset(kTRUE);
  timeChecker->SetCheckTimeOrder(kTRUE);
  timeChecker->SetStsOffsetSearchRange(10000);
  timeChecker->SetMuchOffsetSearchRange(50000);
  timeChecker->SetTofOffsetSearchRange(2000);
  timeChecker->SetRichOffsetSearchRange(1000);
  timeChecker->SetPsdOffsetSearchRange(10000);
  timeChecker->SetT0PulserTotLimits(185, 191);
  timeChecker->SetNrTsForFit(-1); //Positive values: iterative peak fitting 
  if (0 < uRunId)
    timeChecker->SetOutFilename(
      Form("%sHistosTimeCheck_%03u.root", outDir.Data(), uRunId));
  fRun->AddTask(timeChecker);

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

  //  RemoveGeoManager();
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
