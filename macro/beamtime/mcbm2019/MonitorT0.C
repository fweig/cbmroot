/** @file MonitorT0.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer] **/

/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorT0(TString inFile = "", TString sHostname = "localhost", Int_t iServerHttpPort = 8080,
               Int_t iServerRefreshRate = 100, UInt_t uRunId = 0, UInt_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString parFile = "data/moni_t0_params_" + runId + ".root";
  TString outFile = "data/moni_t0__" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2019/";

  TString paramFileTof       = paramDir + "mT0Par.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> MonitorT0: Initialising..." << std::endl;
  CbmMcbm2018MonitorTaskT0* monitor_t0 = new CbmMcbm2018MonitorTaskT0();

  monitor_t0->SetIgnoreOverlapMs();
  monitor_t0->SetHistoryHistoSize(1800);
  if (0 < uRunId) monitor_t0->SetHistoFilename(Form("data/HistosMonitorT0_%03u.root", uRunId));
  monitor_t0->SetPulserTotLimits(180, 210);  // for runs  >  86

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )

  // Use kHodo since there is no entry for T0 in the enum yet
  //  source->AddUnpacker(monitor_t0,  0x60, 9  );//gDPB TOF
  //  source->AddUnpacker(monitor_t0,  0x90, 9  );//gDPB T0
  source->AddUnpacker(monitor_t0, 0x90, ECbmModuleId::kHodo);  //gDPB T0

  source->SetSubscriberHwm(1000);

  // --- Run
  run = new FairRunOnline(source);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  /// ===> Need to explicitely add the canvases to the server in the task!
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  run->SetAutoFinish(kFALSE);

  FairRootFileSink* sink = new FairRootFileSink(outFile);
  run->SetSink(sink);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorT0: Starting run..." << std::endl;
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
  std::cout << ">>> MonitorT0: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorT0: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
