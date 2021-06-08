/* Copyright (C) 2018-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Pierre-Alain Loizeau [committer] */

/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void MonitorTof(TString inFile = "", TString sHostname = "localhost", Int_t iServerRefreshRate = 100,
                Int_t iServerHttpPort = 8080, TString sHistoFile = "data/HistosMonitorTof.root")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  //  TString inDir  = srcDir + "/input/";
  //  if( "" != inFile )
  //   inFile = inDir + inFile;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/test.root";
  TString parFile = "data/testparam.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  //  gLogger->SetLogScreenLevel("DEBUG2"); // Print raw messages
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";
  /*
  TString paramFileMuch = paramDir + "MuchUnpackPar.par";
  TObjString* tutDetDigiFileMuch = new TObjString(paramFileMuch);
  parFileList->Add(tutDetDigiFileMuch);
*/
  TString paramFileTof          = paramDir + "mTofPar.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitorLab: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Initialising..." << std::endl;

  // Dummy Unpacker
  //  CbmTSUnpackDummy*    dummy_unpacker     = new CbmTSUnpackDummy();

  // Much Monitor
  //  CbmTSMonitorMuch* test_monitor_much = new CbmTSMonitorMuch();

  // Get4 Unpacker
  CbmMcbm2018MonitorTof* test_monitor_tof = new CbmMcbm2018MonitorTof();
  test_monitor_tof->SetFitZoomWidthPs();
  test_monitor_tof->SetHistoryHistoSize(600);
  test_monitor_tof->SetHistoryHistoSizeLong(1000.);  // Night: 6 + 10 H
  //  test_monitor_tof->SetHistoryHistoSizeLong( 3840. ); // WE:    6 + 24 + 24 + 10 H
  /*
  test_monitor_tof->SetMsOverlap();
  test_monitor_tof->SetHistoryHistoSize( 600. );
  test_monitor_tof->SetRawDataPrintMsgNb( 100 );
*/
  test_monitor_tof->SetIgnoreMsOverlap();
  test_monitor_tof->EnablePulserMode();
  //test_monitor_tof->EnableCoincidenceMaps();
  //  test_monitor_tof->SetDiamondDpbIdx();
  test_monitor_tof->SetHistoFilename(sHistoFile);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }

  source->AddUnpacker(test_monitor_tof, 0x60, 6);  //gDPBs
  source->AddUnpacker(test_monitor_tof, 0x90, 6);  //gDPBs T0

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
  std::cout << ">>> ngDpbMonitorLab: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Macro finished successfully." << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Output file is " << outFile << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
