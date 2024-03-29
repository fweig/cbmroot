/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig, Norbert Herrmann */

/** @file MonitorTof.C
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

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
void MonitorTof(TString inFile               = "",
                TString sHostname            = "node8",
                Int_t iServerRefreshRate     = 100,
                Int_t iServerHttpPort        = 8060,
                UInt_t nrEvents              = 100,
                Bool_t bIgnoreCriticalErrors = kTRUE,
                Int_t iGdpbIndex             = -1,
                TString sHistoFile           = "data/HistosMonitorTof.root")
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

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
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2021/";
  //TString paramDir = "/scratch/cbmroot_macro/macro/beamtime/mcbm2020/";

  TString paramFileTof       = paramDir + "mTofCriPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> MonitorTof: Initialising..." << std::endl;

  // Get4 Unpacker
  CbmMcbm2018MonitorTaskTof* monitor_tof = new CbmMcbm2018MonitorTaskTof();
  monitor_tof->SetIgnoreOverlapMs();
  monitor_tof->SetHistoryHistoSize(600);
  //  monitor_tof->SetDebugMonitorMode();
  monitor_tof->SetIgnoreCriticalErrors(bIgnoreCriticalErrors);
  monitor_tof->SetGdpbIndex(iGdpbIndex);
  monitor_tof->SetHistoFilename(sHistoFile);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) {
    // Empty comment to avoid collapsing by clang-format
    source->SetFileName(inFile);
  }  // if( "" != inFile )
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
  std::cout << ">>> MonitorTof: Starting run..." << std::endl;
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
  std::cout << ">>> MonitorTof: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorTof: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
