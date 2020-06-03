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

void ngDpbMonitorLab(TString inFile = "", Bool_t bGet4v2Mode = kTRUE, Bool_t b24bModeOn = kFALSE,
                     Bool_t bMergedEpochsOn = kFALSE )
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
//  gLogger->SetLogScreenLevel("DEBUG2"); // Print raw messages
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";
/*
  TString paramFileMuch = paramDir + "MuchUnpackPar.par";
  TObjString* tutDetDigiFileMuch = new TObjString(paramFileMuch);
  parFileList->Add(tutDetDigiFileMuch);
*/
  TString paramFileTof = paramDir + "MapTofLab2017.par";
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
  CbmTSMonitorTofStar* test_monitor_tof = new CbmTSMonitorTofStar();
  test_monitor_tof->SetPulserMode();
  test_monitor_tof->SetPulserFee(0, 0);
  test_monitor_tof->SetPulserChans(3, 35, 67, 99, 1, 127 );
  test_monitor_tof->SetGet4v20Mode( bGet4v2Mode );
  test_monitor_tof->SetGet4Mode24b( b24bModeOn );
  test_monitor_tof->SetMergedEpochs( bMergedEpochsOn );
//  if( kTRUE == bMergedEpochsOn )
//   test_monitor_tof->SetEpochSuppressedMode( kTRUE );
  test_monitor_tof->SetFitZoomWidthPs( );
  test_monitor_tof->SetMsOverlap();
  test_monitor_tof->SetHistoryHistoSize( 600. );

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
//  source->AddUnpacker(test_monitor_much, 0x10, 4); //nDPBs
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
  std::cout << ">>> ngDpbMonitorLab: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;
    
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
