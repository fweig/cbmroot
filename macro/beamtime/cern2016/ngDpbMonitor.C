/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline *run = NULL;

void ngDpbMonitor(TString inFile = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString inDir  = srcDir + "/input/";
  if( "" != inFile )
   inFile = inDir + inFile;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
//  Int_t nEvents = 10000;
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

  TString paramFileMuch = paramDir + "MuchUnpackPar.par";
  TObjString* tutDetDigiFileMuch = new TObjString(paramFileMuch);
  parFileList->Add(tutDetDigiFileMuch);

  TString paramFileTof = paramDir + "MapCern2016.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitor: Initialising..." << std::endl;

  // Dummy Unpacker
//  CbmTSUnpackDummy*    dummy_unpacker     = new CbmTSUnpackDummy();
  
  // Much Monitor
  CbmTSMonitorMuch* test_monitor_much = new CbmTSMonitorMuch();
  
  // Get4 Unpacker
  CbmTSMonitorTof* test_monitor_tof = new CbmTSMonitorTof();
//  test_monitor_tof->SetDiamondChannels(); // Up to 10/12/2016 17:30
  test_monitor_tof->SetDiamondChannels( 0, 2, 77,  70, 94, 86); // Starting from 10/12/2016 17:30
  test_monitor_tof->SetDiamondPerTsSpillOnThr(  50 ); // 2 at 600 mV, 10 at 500 mV, 50 at 400 mV
  test_monitor_tof->SetDiamondPerTsSpillOffThr( 10 ); // 1 at 600-500 mV, 10 at 400 mV
  test_monitor_tof->SetDiamondTsNbSpillOffThr(  50 );
  test_monitor_tof->SetEpochSuppressedMode();

  // --- Source task
  CbmFlibCern2016Source* source = new CbmFlibCern2016Source();
  if( "" != inFile )
      source->SetFileName(inFile);
      else
      {
         source->SetHostName( "cbmflib20d");
         source->SetPortNumber( 5556 );
//         source->SetPortNumber( 5560 );
      }

  source->AddUnpacker(test_monitor_tof,  0x60, 6); //gDPBs
  source->AddUnpacker(test_monitor_much, 0x10, 4); //nDPBs
  source->SetDataReductionMuch( 20 );
//   source->AddUnpacker(dummy_unpacker, 0x10, 4);//gDPB A & B
//   source->AddUnpacker(dummy_unpacker, 0x60, 6);//gDPB A & B

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
  std::cout << ">>> ngDpbMonitor: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;
    
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> ngDpbMonitor: Macro finished successfully." << std::endl;
  std::cout << ">>> ngDpbMonitor: Output file is " << outFile << std::endl;
  std::cout << ">>> ngDpbMonitor: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
