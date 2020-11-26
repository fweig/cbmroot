void check_events(Int_t nEvents      = 10,
                  UInt_t uRunId      = 0,
                  TString inDir      = "data/",
                  TString friendFile = "",
                  TString inFile     = "") {
  Int_t iVerbose = 1;
  Int_t iBugCor  = 0;
  //Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel);
  gLogger->SetLogVerbosityLevel("VERYHIGH");

  TString workDir = gSystem->Getenv("VMCWORKDIR");

  TString runId = TString::Format("%03u", uRunId);

  TString ParFile = inDir + "/unp_mcbm_params_" + runId + ".root";

  TString InputFile      = inDir + "/unp_mcbm_" + runId + ".root";
  TString InputFileEvent = "";
  if ("" == friendFile) {
    InputFileEvent = inDir + "/events_" + runId + ".root";
  }  // if( "" == friendFile )
  else {
    InputFileEvent = inDir + friendFile;
  }  // else of if( "" == friendFile )

  TString OutputFile = inDir + "/test_" + runId + ".out.root";

  TList* parFileList = new TList();

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();

  FairFileSource* inputSource = new FairFileSource(InputFile);
  inputSource->AddFriend(InputFileEvent);
  run->SetSource(inputSource);

  FairRootFileSink* outputSink = new FairRootFileSink(OutputFile);
  run->SetSink(outputSink);


  run->SetEventHeaderPersistence(kFALSE);

  // ------------------------------------------------------------------------
  // Enable FairMonitor
  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

  CbmCheckEvents* checker = new CbmCheckEvents();
  run->AddTask(checker);

  // -----  Parameter database   --------------------------------------------

  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
  parIo2->open(ParFile.Data(), "UPDATE");
  parIo2->print();
  rtdb->setFirstInput(parIo2);

  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(parFileList, "in");
  parIo1->print();
  rtdb->setSecondInput(parIo1);
  rtdb->print();
  rtdb->printParamContexts();

  //  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  //  parInput1->open(ParFile.Data());
  //  rtdb->setFirstInput(parInput1);

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  //tofClust->Finish();
  // ------------------------------------------------------------------------
  // default display

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
