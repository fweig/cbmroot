/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file TofTestFee
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 13.05.2019
 **
 ** ROOT macro to test TOF FEE with pulser
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void TofTestFee(TString inFile = "", TString sHostname = "localhost", TString sHistoFile = "data/TofTestFeeHistos.root",
                Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080, TString sFileTag = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/tof_test_fee" + sFileTag + ".root";
  TString parFile = "data/tof_test_fee_param" + sFileTag + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  //  gLogger->SetLogScreenLevel("DEBUG2"); // Print raw messages
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileTof          = paramDir + "mTofPar.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> TofTestFee: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> TofTestFee: Initialising..." << std::endl;

  // Get4 Unpacker
  CbmMcbm2018TofTestFee* test_monitor_tof = new CbmMcbm2018TofTestFee();
  test_monitor_tof->SetFitZoomWidthPs();
  test_monitor_tof->SetHistoryHistoSize(4000);
  /*
  test_monitor_tof->SetMsOverlap();
*/
  test_monitor_tof->SetIgnoreMsOverlap();
  test_monitor_tof->SetHistoFileName(sHistoFile);
  test_monitor_tof->SelectTestFees(1, 0,
                                   0,  // DPB GBTx FEE
                                   1, 0,
                                   1);  // DPB GBTx FEE

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }

  source->AddUnpacker(test_monitor_tof, 0x60, 6);  //gDPBs TOF
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
  std::cout << ">>> TofTestFee: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> TofTestFee: Macro finished successfully." << std::endl;
  std::cout << ">>> TofTestFee: Output file is " << outFile << std::endl;
  std::cout << ">>> TofTestFee: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
}
