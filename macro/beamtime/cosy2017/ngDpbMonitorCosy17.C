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

void ngDpbMonitorCosy17(TString inFile = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString inDir  = srcDir + "/input/";
  if( "" != inFile )
   inFile = inDir + inFile;

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

  TString paramFileSts = paramDir + "StsUnpackPar.par";
  TObjString* tutDetDigiFileSts = new TObjString(paramFileSts);
  parFileList->Add(tutDetDigiFileSts);

/*
  TString paramFileMuch = paramDir + "MuchUnpackPar.par";
  TObjString* tutDetDigiFileMuch = new TObjString(paramFileMuch);
  parFileList->Add(tutDetDigiFileMuch);
*/
/*
  TString paramFileTof = paramDir + "MapTofLab2017.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);
*/
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
  CbmTsMonitorSts* test_monitor_sts = new CbmTsMonitorSts();
//  test_monitor_sts->SetRunStart( 20160206, 1610);

  // Much Monitor
//  CbmTSMonitorMuch* test_monitor_much = new CbmTSMonitorMuch();
//  test_monitor_much->SetRunStart( 20160206, 1610);

  // Get4 Unpacker
//  CbmTSMonitorTofStar* test_monitor_tof = new CbmTSMonitorTofStar();
//  test_monitor_tof->SetPulserMode();
//  test_monitor_tof->SetPulserFee(0, 1);
//  test_monitor_tof->SetPulserChans(   0,   8,  16,  24,  32,  40,  48,  56, 
//                                     64,  72,  80,  88,  96, 104, 112, 120 );

  // --- Source task
  CbmFlibCern2016Source* source = new CbmFlibCern2016Source();
  if( "" != inFile )
      source->SetFileName(inFile);
      else
      {
//         source->SetHostName( "localhost");
         source->SetHostName( "cbmflib20");
         source->SetPortNumber( 5556 );
      }

  source->AddUnpacker(test_monitor_sts, 0x10, 4); //nDPBs
//  source->AddUnpacker(test_monitor_tof,  0x60, 6); //gDPBs
//  source->AddUnpacker(test_monitor_much, 0x10, 4); //nDPBs

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
  std::cout << ">>> ngDpbMonitorLab: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Macro finished successfully." << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Output file is " << outFile << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
