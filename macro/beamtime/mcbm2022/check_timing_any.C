/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

void check_timing_any(TString fileName, UInt_t uRunId = 0, Int_t nEvents = 0, TString outDir = "data/")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 3;

  // MC file

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
  FairRunOnline* fRun = new FairRunOnline();
  fRun->ActivateHttpServer(100, 8080);  // refresh each 100 events
  fRun->SetSink(new FairRootFileSink("SinkFile.root"));
  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);

  // Define output file for FairMonitor histograms
  //  TString monitorFile{outFile};
  //  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

  CbmMcbmCheckTimingTask* timeChecker = new CbmMcbmCheckTimingTask();
  /// Default is using T0 as reference
  /// With Pulser rejection
  timeChecker->SetReferenceDetector( ECbmModuleId::kT0, "T0",
                                     -1000., 1000., 320.,
                                     182, 190 );

  /// With pulser selection
  /*
  timeChecker->SetReferenceDetector( ECbmModuleId::kT0, "T0",
                                     -1000., 1000., 320.,
                                     190, 182 );
*/
  /// Here swapping with TOF
  //   timeChecker->SetReferenceDetector(ECbmModuleId::kTof, "Tof", -3000, 3000, 3000);
  //   timeChecker->RemoveCheckDetector(ECbmModuleId::kTof);
  //   timeChecker->AddCheckDetector(ECbmModuleId::kT0, "T0");

  /// Here swapping with MUCH
  /*
  timeChecker->SetReferenceDetector(ECbmModuleId::kMuch, "Much");
  timeChecker->RemoveCheckDetector(ECbmModuleId::kMuch);
  timeChecker->AddCheckDetector(ECbmModuleId::kT0, "T0");
  */

  /// Remove detectors not present in 2021
  //timeChecker->RemoveCheckDetector(ECbmModuleId::kT0);
  timeChecker->RemoveCheckDetector(ECbmModuleId::kMuch);
  //timeChecker->SetReferenceDetector(ECbmModuleId::kPsd, "Psd", -300000, 300000, 320 * 300);
  timeChecker->RemoveCheckDetector(ECbmModuleId::kPsd);

  /// Add detectors with wider range
  /*
  timeChecker->RemoveCheckDetector(ECbmModuleId::kSts);
  timeChecker->AddCheckDetector(ECbmModuleId::kSts, "Sts");
  timeChecker->RemoveCheckDetector(ECbmModuleId::kTrd);
  timeChecker->AddCheckDetector(ECbmModuleId::kTrd, "Trd");
  timeChecker->RemoveCheckDetector(ECbmModuleId::kTof);
  //timeChecker->AddCheckDetector(ECbmModuleId::kTof, "Tof", -150000, 150000, 320*150);
  timeChecker->AddCheckDetector(ECbmModuleId::kTof, "Tof", -2000, 2000, 320 * 2);
  */

  /// Add extra differential analysis on specific detectors
  timeChecker->AddCheckDetector(ECbmModuleId::kSts, "Sts", -100., 100., 4000);
  timeChecker->AddCheckDetector(ECbmModuleId::kRich, "Rich", -100., 100., 4000);
  timeChecker->AddCheckDetector(ECbmModuleId::kTrd, "Trd", -500., 500., 250);
  timeChecker->AddCheckDetector(ECbmModuleId::kTof, "Tof", -100, 100, 4000);
  timeChecker->SetDetectorDifferential(ECbmModuleId::kTrd, {"5", "21", "37", "53"});

  TString outFileName = fileName(0, fileName.Index(".digi.root"));
  if (0 < uRunId) timeChecker->SetOutFilename(Form("%s/%s.tck.root", outDir.Data(), outFileName.Data()));
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
  }
  else {
    fRun->Run(0, nEvents);  // process  N Events
  }
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
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
  /*
  FairMonitor* tempMon = FairMonitor::GetMonitor();
  tempMon->Print();
*/
  //  RemoveGeoManager();
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
