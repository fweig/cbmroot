/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Pierre-Alain Loizeau [committer] */

/** @file MonitorTofPulser.C
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 13.10.2019
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorTofPulser(TString inFile = "", TString sHostname = "etofin001", Int_t iServerRefreshRate = 100,
                      Int_t iServerHttpPort = 8083, UInt_t nrEvents = 0, Int_t iGdpbIndex = -1, )
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  //  gLogger->SetLogScreenLevel("DEBUG2"); // Print raw messages
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileTof       = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> MonitorTofPulser: Initialising..." << std::endl;

  // Get4 Unpacker
  CbmMcbm2018MonitorTaskTofPulser* monitor_tof = new CbmMcbm2018MonitorTaskTofPulser();
  monitor_tof->SetUpdateFreqTs(1000);
  monitor_tof->SetPulserTotLimits(85, 95);  // Auto pulser
  monitor_tof->SetPulserChannel(0);
  monitor_tof->SetGdpbIndex(iGdpbIndex);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )
  source->SetSubscriberHwm(10);
  source->AddUnpacker(monitor_tof, 0x60, ECbmModuleId::kTof);  //gDPBs
  source->AddUnpacker(monitor_tof, 0x90, ECbmModuleId::kTof);  //gDPBs T0

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
  std::cout << ">>> MonitorTofPulser: Starting run..." << std::endl;
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
  std::cout << ">>> MonitorTofPulser: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorTofPulser: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
}
