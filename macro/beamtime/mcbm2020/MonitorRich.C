/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file MCBM RICH DATA monitoring
 ** Based on MonitorT0 by P.-A. Loizeau
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorRich(TString inFile    = "/Users/slebedev/Development/cbm/data/mcbm18/2021179204256_0.tsa",
                 TString sHostname = "localhost", Int_t iServerHttpPort = 8080, Int_t iServerRefreshRate = 100,
                 UInt_t uRunId = 0, UInt_t nrEvents = 1000000)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString parFile = TString::Format("data/moni_rich_params_%u.root", uRunId);

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("DEBUG4");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2020/";

  TString paramFileRich       = paramDir + "mRichPar_70.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  // --- Set debug level
  gDebug = 0;

  CbmMcbm2018UnpackerTaskRich2020* unpacker_rich = new CbmMcbm2018UnpackerTaskRich2020();
  unpacker_rich->SetIgnoreOverlapMs();
  unpacker_rich->SetMonitorMode();
  unpacker_rich->DoTotCorr(kFALSE);
  unpacker_rich->SetWriteOutputFlag(kFALSE);  /// Needed to avoid bug with FairRoot not checking if Data Sink exists
  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if ("" != inFile) { source->SetFileName(inFile); }
  else {
    source->SetHostName(sHostname);
  }

  source->AddUnpacker(unpacker_rich, 0x30, ECbmModuleId::kRich);  //RICH trb

  source->SetSubscriberHwm(1000);

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
  std::cout << ">>> MonitorRich: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  2000 Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorRich: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorRich: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
