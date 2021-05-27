/** @file check_much_digis.C
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

void check_much_digis(UInt_t uRunId, UInt_t uTsJump, Double_t dFirstTsOffset, Double_t dDigiDistPlotStartTime = 0.0,
                      Int_t nrEvents = 0, TString sDir = "data")
{
  if (uRunId < 353) return kFALSE;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = 1e9;
  // --- Specify output file name (this is just an example)
  TString runId    = TString::Format("%03u", uRunId);
  TString fileName = sDir + "/unp_mcbm_" + runId + ".root";

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

  // Define output file for FairMonitor histograms
  //  TString monitorFile{outFile};
  //  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------

  CbmMcbm2019CheckDigisMuch* muchChecker = new CbmMcbm2019CheckDigisMuch();
  muchChecker->SetTimeWindow(uTsJump, dFirstTsOffset, 2, 3, 10240000);
  muchChecker->SetDigiDistPlotStartTime(dDigiDistPlotStartTime);
  muchChecker->SetMuchPulseradcLimits(5, 15);
  if (0 < uRunId) muchChecker->SetOutFilename(Form("data/HistosMuchCheck_%03u.root", uRunId));
  fRun->AddTask(muchChecker);

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
  if (0 == nrEvents) {
    fRun->Run(0, nEvents);  // run until end of input file
  }
  else {
    fRun->Run(0, nrEvents);  // process  N Events
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
