/** @file MonitorStar2017
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 22.01.2017
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline *run = NULL;

void MonitorHdCosmics2017(TString inFile = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
//  TString inDir  = srcDir + "/input/";

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
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";

  TString paramFileTof = paramDir + "MapTofHdCosmics2017.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> MonitorStar2017: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> MonitorStar2017: Initialising..." << std::endl;

  // Get4 Unpacker
//  CbmTSMonitorTofStar* test_monitor_tof = new CbmTSMonitorTofStar();
  CbmTSMonitorTofLab* test_monitor_tof = new CbmTSMonitorTofLab();
//  test_monitor_tof->SetCoincidenceMapOn();
/*
  test_monitor_tof->SetPulserMode();
  test_monitor_tof->SetPulserFee(0, 1);
  test_monitor_tof->SetPulserChans(   0,   8,  16,  24,  32,  40,  48,  56,
                                     64,  72,  80,  88,  96, 104, 112, 120 );
*/
   test_monitor_tof->SetMsOverlap(1);
   test_monitor_tof->SetEpochSuppressedMode();

  // --- Source task
  CbmFlibCern2016Source* source = new CbmFlibCern2016Source();
  if( "" != inFile )
      source->SetFileName(inFile);
      else
      {
         source->SetHostName( "localhost");
         source->SetPortNumber( 5556 );
      }

  source->AddUnpacker(test_monitor_tof,  0x60, 6); //gDPBs

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  run->ActivateHttpServer(100); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorStar2017: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();

//  run->Finish();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorStar2017: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorStar2017: Output file is " << outFile << std::endl;
  std::cout << ">>> MonitorStar2017: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
