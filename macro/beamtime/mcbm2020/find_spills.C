/** @file find_spills.C
  * @copyright Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/** @file MCBM spill detection with T0
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 16.02.2021
 ** ROOT macro to read tsa files which have been produced in mCBM and use the T0 detector to
 ** find the spill breaks beginning, middle and end TS index
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

Bool_t find_spills(TString inFile = "", UInt_t uRunId = 0, TString sOutDir = "data", TString sHostname = "localhost",
                   Int_t iServerHttpPort = 8080, Int_t iServerRefreshRate = 100)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString parFile = "data/spill_finder_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2020/";

  TString paramFileTof       = paramDir + "mT0Par.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> SpillFinder: Initialising..." << std::endl;
  CbmMcbmSpillFindTask* spill_finder_t0 = new CbmMcbmSpillFindTask();

  spill_finder_t0->SetIgnoreOverlapMs();
  if (0 < uRunId) spill_finder_t0->SetHistoFilename(Form("data/HistosSpillFinder_%03u.root", uRunId));
  spill_finder_t0->SetPulserTotLimits(180, 210);    // for runs  >  86
  spill_finder_t0->SetSpillCheckIntervalSec(0.05);  // ~every 4 TS
  spill_finder_t0->SetSpillThreshold(40);           // ~10 hits per TS

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )

  // Use kHodo since there is no entry for T0 in the enum yet
  source->AddUnpacker(spill_finder_t0, 0x90, ECbmModuleId::kHodo);  //gDPB T0

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
  std::cout << ">>> SpillFinder: Starting run..." << std::endl;
  run->Run(-1, 0);  // process  full file(s)!

  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> SpillFinder: Macro finished successfully." << std::endl;
  std::cout << ">>> SpillFinder: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  return kTRUE;
}
