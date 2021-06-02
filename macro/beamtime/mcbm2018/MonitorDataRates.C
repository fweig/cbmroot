/** @file MonitorDataRates.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
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

void MonitorDataRates(TString inFile = "", TString sHostname = "localhost", Int_t iServerHttpPort = 8080,
                      Int_t iServerRefreshRate = 100, UInt_t uRunId = 0, UInt_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString outFile = "data/moni_t0_" + runId + ".root";
  TString parFile = "data/moni_t0_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> MonitorDataRates: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> MonitorDataRates: Initialising..." << std::endl;
  CbmMcbm2018MonitorDataRates* monitor = new CbmMcbm2018MonitorDataRates();

  monitor->SetIgnoreOverlapMs();
  monitor->SetHistoryHistoSize(1800);
  if (0 < uRunId) monitor->SetHistoFilename(Form("data/HistosDataRates_%03u.root", uRunId));

  monitor->AddEqIdChannelNumber(0x193d, 18 * 128);  // STS 0
  monitor->AddEqIdChannelNumber(0x5c0b, 14 * 128);  // STS 1
  monitor->AddEqIdChannelNumber(0x18e3, 3 * 128);   // MUCH M1 A
  monitor->AddEqIdChannelNumber(0x4f19, 3 * 128);   // MUCH M1 B
  monitor->AddEqIdChannelNumber(0x1931, 3 * 128);   // MUCH M2 A
  monitor->AddEqIdChannelNumber(0x6601, 3 * 128);   // MUCH M2 B
  monitor->AddEqIdChannelNumber(0x1980, 960);       // TOF 3 stack
  monitor->AddEqIdChannelNumber(0x1922, 960);       // TOF 2 stack
  monitor->AddEqIdChannelNumber(0x1949, 48 + 8);    // TOF guests
  monitor->AddEqIdChannelNumber(0x18c5, 4);         // T0 A
  monitor->AddEqIdChannelNumber(0x1925, 3);         // T0 B

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )

  source->AddUnpacker(monitor, 0x10, 9);  //sDPB STS + MUCH
  source->AddUnpacker(monitor, 0x30, 9);  //rDPB RICH
  source->AddUnpacker(monitor, 0x60, 9);  //gDPB TOF
  source->AddUnpacker(monitor, 0x90, 9);  //gDPB T0

  source->SetSubscriberHwm(10000);

  // --- Run
  run = new FairRunOnline(source);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  /// ===> Need to explicitely add the canvases to the server in the task!
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  run->SetAutoFinish(kFALSE);


  // -----   Runtime database   ---------------------------------------------
  /*
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
*/
  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorDataRates: Starting run..." << std::endl;
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
  std::cout << ">>> MonitorDataRates: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorDataRates: Output file is " << outFile << std::endl;
  std::cout << ">>> MonitorDataRates: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
