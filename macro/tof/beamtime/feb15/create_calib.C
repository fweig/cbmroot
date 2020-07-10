// -----------------------------------------------------------------------------
// ----- create_calib.C                                                    -----
// -----                                                                   -----
// ----- created by C. Simon on 2016-04-09                                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

void create_calib(Long64_t nEvents = 100000000,
                  TString cFileId  = "CernSps05Mar0041") {
  TStopwatch timer;
  timer.Start();

  FairLogger::GetLogger();

  //  gLogger->SetLogScreenLevel("FATAL");
  gLogger->SetLogScreenLevel("ERROR");
  //  gLogger->SetLogScreenLevel("WARNING");
  //  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  //  gLogger->SetLogScreenLevel("DEBUG1");
  //  gLogger->SetLogScreenLevel("DEBUG2");
  //  gLogger->SetLogScreenLevel("DEBUG3");

  //  gLogger->SetLogVerbosityLevel("LOW");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //  gLogger->SetLogVerbosityLevel("HIGH");

  TString srcDir   = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = srcDir + "/macro/tof/beamtime/feb15";

  TObjString unpParFile = (paramDir + "/parUnpack_basic.txt").Data();
  TObjString calParFile = (paramDir + "/parCalib_basic.txt").Data();

  TList* parFileList = new TList();
  parFileList->Add(&unpParFile);
  parFileList->Add(&calParFile);

  FairRunOnline* run = new FairRunOnline();
  //  run->SetAutoFinish(kFALSE);

  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);

  TString cOutfileId = Form("%s", cFileId.Data());

  TString outFile = "./unpack_" + cOutfileId + ".calib.root";

  FairLmdSource* source = new FairLmdSource();
  source->AddPath("/lustre/nyx/cbm/prod/beamtime/2015/02/cern/data/",
                  Form("%s*.lmd", cFileId.Data()));

  TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10, 1, 31, 0, 0);
  tofTrbDataUnpacker->SetInspection(kTRUE);
  tofTrbDataUnpacker->SetSaveRawData(kFALSE);
  source->AddUnpacker(tofTrbDataUnpacker);

  TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni");

  TTofTrbTdcCalib* tofCalibTrb = new TTofTrbTdcCalib();
  tofCalibTrb->CreateCalibration(kTRUE);
  tofCalibTrb->SaveCalibData(kFALSE);
  tofCalibTrb->SetFineTimeMethod(0);
  tofCalibTrb->SetToTMethod(0);
  tofCalibTrb->SetMinEntriesBinByBin(100000);
  tofCalibTrb->SetMinEntriesLocalFineLimits(10000);
  tofCalibTrb->SetTimeContinuum(kTRUE);
  tofCalibTrb->SetTrailingOffsetCycles(2);
  tofCalibTrb->SetLowerLinearFineLimit(31);   // feb15
  tofCalibTrb->SetUpperLinearFineLimit(490);  // feb15
  //  tofCalibTrb->SetLowerLinearFineLimit(16);  // nov15
  //  tofCalibTrb->SetUpperLinearFineLimit(490); // nov15
  tofCalibTrb->SetReferenceBoard(0);
  tofCalibTrb->SetToTSingleLeading(-100.);

  CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
  display->MonitorTdcOccupancy(kTRUE);
  display->MonitorCts(kTRUE);
  display->MonitorSebStatus(kTRUE);
  display->MonitorTdcStatus(kTRUE);
  display->MonitorFSMockup(kTRUE);
  display->MonitorInspection(kTRUE);
  display->MonitorCalibration(kTRUE);
  //  display->SetUpdateInterval(10000);
  display->SetUpdateInterval(100000000);  // disables live updates (speed-up)


  run->AddTask(tofUnpMonitor);
  run->AddTask(tofCalibTrb);
  run->AddTask(display);

  run->SetSource(source);
  run->SetOutputFile(outFile);
  run->SetAutoFinish(kFALSE);

  run->Init();

  cout << "Starting run" << endl;
  run->Run(0, nEvents);

  cout << "Finishing run" << endl;
  run->Finish();

  tofTrbDataUnpacker->WriteHistograms();

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Run finished successfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
}
