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

void MonitorTofSync(TString inFile               = "",
                    TString sHostname            = "localhost",
                    Int_t iServerRefreshRate     = 100,
                    Int_t iServerHttpPort        = 8090,
                    UInt_t nrEvents              = 500,
                    Bool_t bIgnoreCriticalErrors = kTRUE,
                    TString sHistoFile = "data/HistosMonitorTofSync.root") {
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
  std::cout << ">>> MonitorTof: Initialising..." << std::endl;

  // Get4 Unpacker
  CbmStar2019MonitorTask* monitorEtof = new CbmStar2019MonitorTask();
  monitorEtof->SetIgnoreOverlapMs();
  monitorEtof->SetHistoryHistoSize(300);
  monitorEtof->SetDebugMonitorMode();
  monitorEtof->SetIgnoreCriticalErrors(bIgnoreCriticalErrors);
  //  monitorEtof->SetHistoryHistoSizeLong( 1000. ); // Night: 6 + 10 H
  //  monitorEtof->SetHistoryHistoSizeLong( 3840. ); // WE:    6 + 24 + 24 + 10 H
  //  monitorEtof->SetHistoFilename( sHistoFile );

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if ("" != inFile) {
    source->SetFileName(inFile);
  }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )
  source->SetSubscriberHwm(100);
  source->AddUnpacker(monitorEtof, 0x60, 6);  //gDPBs

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
  std::cout << ">>> MonitorTofSync: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  } else {
    run->Run(0, nrEvents);  // process  2000 Events
  }
  //  run->Finish();
  monitorEtof->SaveLatencyHistograms(sHistoFile);

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices"
            << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorTof: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorTof: Real time " << rtime << " s, CPU time " << ctime
            << " s" << std::endl;
  std::cout << std::endl;
}
