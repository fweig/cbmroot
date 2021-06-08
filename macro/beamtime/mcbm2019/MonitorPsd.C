/* Copyright (C) 2019-2020 Institute for Nuclear Research, Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin [committer] */

/** @file MCBM PSD DATA monitoring
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 09.10.2019
 ** Based on MonitorT0 by P.-A. Loizeau
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorPsd(TString inFile = "", TString sHostname = "localhost", Int_t iServerHttpPort = 8080,
                Int_t iServerRefreshRate = 100, UInt_t uRunId = 0, TString sHistoFilePrefix = "", UInt_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId             = TString::Format("%u", uRunId);
  TString outFile           = "data/moni_psd_" + runId + ".root";
  TString parFile           = "data/moni_psd_params_" + runId + ".root";
  TString outFileNameHistos = Form("data/HistosMonitorPsd_%03u_", uRunId) + sHistoFilePrefix + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2019/";

  TString paramFilePsd       = paramDir + "mPsdPar.par";
  TObjString* parPsdFileName = new TObjString(paramFilePsd);
  parFileList->Add(parPsdFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> MonitorPsd: output file is " << outFile << std::endl;

  // ------------------------------------------------------------------------

  std::cout << std::endl;
  std::cout << ">>> MonitorPsd: Initialising..." << std::endl;
  CbmMcbm2018MonitorTaskPsd* monitor_psd = new CbmMcbm2018MonitorTaskPsd();

  monitor_psd->SetIgnoreOverlapMs();
  monitor_psd->SetHistoryHistoSize(1000);
  monitor_psd->SetHistoFilename(outFileNameHistos);

  std::vector<Int_t> fviHistoChargeArgs {500, 0, 40000};
  std::vector<Int_t> fviHistoAmplArgs {100, 0, 3000};
  std::vector<Int_t> fviHistoZLArgs {9000, 0, 9000};
  monitor_psd->SetChargeHistoArgs(fviHistoChargeArgs);
  monitor_psd->SetAmplHistoArgs(fviHistoAmplArgs);
  monitor_psd->SetZLHistoArgs(fviHistoZLArgs);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )


  source->AddUnpacker(monitor_psd, 0x80, ECbmModuleId::kPsd);  //gDPB T0

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
  std::cout << ">>> MonitorPsd: Starting run..." << std::endl;
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
  std::cout << ">>> MonitorPsd: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorPsd: Output file is " << outFile << std::endl;
  std::cout << ">>> MonitorPsd: Output histos file is " << outFileNameHistos << std::endl;
  std::cout << ">>> MonitorPsd: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
