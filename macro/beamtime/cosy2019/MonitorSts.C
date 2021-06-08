/* Copyright (C) 2017-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file Cern2017Monitor.C
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 26.07.2017
 **
 ** ROOT macro to read tsa files which have been produced with StsXyter + DPB + FLIB
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorSts(TString inFile = "", TString sHostname = "localhost", Int_t iServerRefreshRate = 100,
                Int_t iServerHttpPort = 8080, TString sHistoFile = "data/StsHistos.root", Int_t nEvents = -1)
{

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  //  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/sts_out.root";
  TString parFile = "data/sts_param.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileHodo          = paramDir + "mStsPar.par";
  TObjString* tutDetDigiFileHodo = new TObjString(paramFileHodo);
  parFileList->Add(tutDetDigiFileHodo);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: Initialising..." << std::endl;

  // Hodoscopes Monitor
  CbmMcbm2018MonitorSts* monitorSts = new CbmMcbm2018MonitorSts();
  monitorSts->SetHistoFileName(sHistoFile);
  //  monitorSts->SetPrintMessage();
  monitorSts->SetMsOverlap(1);
  monitorSts->SetIgnoreOverlapMs();
  //  monitorSts->SetLongDurationLimits( 3600, 10 );
  monitorSts->SetLongDurationLimits(7200, 60);
  //  monitorSts->SetEnableCoincidenceMaps();
  //  monitorSts->SetCoincidenceBorder(   0.0,  200 );
  monitorSts->SetEnableCheckBugSmx20(kTRUE);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )

  source->AddUnpacker(monitorSts, 0x10, 6);  // stsXyter DPBs

  source->SetSubscriberHwm(10);

  // --- Run
  run = new FairRunOnline(source);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  /// ===> Need to explicitely add the canvases to the server in the task!
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorSts: Starting run..." << std::endl;
  //  run->Run(nEvents, 0); // run until end of input file
  if (nEvents <= 0) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nEvents);  // process  N Events
  }
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorSts: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorSts: Output file is " << outFile << std::endl;
  std::cout << ">>> MonitorSts: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
