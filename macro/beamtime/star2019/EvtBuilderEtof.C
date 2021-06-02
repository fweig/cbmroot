/** @file EvtBuilderEtof.C
  * @copyright Copyright (C) 2018-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer] **/

/** @file EvtBuilderEtof.C
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 14.10.2019
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void EvtBuilderEtof(TString inFile = "", TString sHostname = "localhost", Int_t iServerRefreshRate = 100,
                    Int_t iServerHttpPort = 8081, Bool_t bSandbox = kFALSE, Bool_t bAddStatusToEvent = kTRUE,
                    UInt_t nrEvents = 0)
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

  TString paramFileTof        = paramDir + "etofEvtBuildPar.par";
  TObjString* parEtofFileName = new TObjString(paramFileTof);
  parFileList->Add(parEtofFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> EvtBuilderEtof: Initialising..." << std::endl;

  // Get4 Unpacker
  CbmStar2019EventBuilderEtof* etofEventBuilder = new CbmStar2019EventBuilderEtof();
  etofEventBuilder->SetSandboxMode(bSandbox);
  etofEventBuilder->SetAddStatusToEvent(bAddStatusToEvent);

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )
  source->SetSubscriberHwm(3000);
  source->AddUnpacker(etofEventBuilder, 0x60, 6);  //gDPBs

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
  std::cout << ">>> EvtBuilderEtof: Starting run..." << std::endl;
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
  std::cout << ">>> EvtBuilderEtof: Macro finished successfully." << std::endl;
  std::cout << ">>> EvtBuilderEtof: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
}
