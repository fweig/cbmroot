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

void eDpbBeamMonitorDesy(Bool_t highP = false, TString inFile =
		     "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString inDir  = srcDir + "/input/";
//  TString inDir  = "/lustre/nyx/cbm/prod/beamtime/2016/11/cern/";
  if( "" != inFile )
   inFile = inDir + inFile;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/testBeam.root";
  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> eDpbMonitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> eDpbMonitor: Initialising..." << std::endl;

  // Spadic 2.0 unpacker
  //  CbmTSUnpackSpadic20DesyOnlineMonitor* spadic20_unpacker = new CbmTSUnpackSpadic20DesyOnlineMonitor(highP);
  //CbmTSUnpackSpadic20DesyOnlineMonitor* spadic20_unpacker = new CbmTSUnpackSpadic20DesyOnlineMonitor(false);
 CbmTSUnpackSpadic20DesyOnlineMonitor* spadic20_unpacker = new CbmTSUnpackSpadic20DesyOnlineMonitor(false);

  // --- Source task
  CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();
  source->SetHostName("localhost");
  source->AddUnpacker(spadic20_unpacker, 0x10); // FIXME: set correct values

  // --- Event header
  //FairEventHeader* event = new CbmTbEvent();
  //event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  //run->SetEventHeader(event);
   run->ActivateHttpServer(100); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  //  gDebug=2;
  //FairTask* spadicRawBeam = new CbmTrdTimeCorrel();
  //run->AddTask(spadicRawBeam);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> eDpbMonitor: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
   std::cout << ">>> eDpbMonitor: Starting run..test." << std::endl;
  timer.Stop();

  //run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> eDpbMonitor: Macro finished successfully." << std::endl;
  std::cout << ">>> eDpbMonitor: Output file is " << outFile << std::endl;
  std::cout << ">>> eDpbMonitor: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
