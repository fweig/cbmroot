/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */


void FHodoLabSetup(TString inFile = "hodoTop_source_1000ts_20160422.tsa")
{

  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString inDir  = srcDir + "/input/";
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
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = srcDir + "/macro/beamtime/tests/";
  TString paramFile = paramDir + "FHodoUnpackPar.par";
  TObjString* tutDetDigiFile = new TObjString(paramFile);
  parFileList->Add(tutDetDigiFile);
  
  TString paramFileTof = paramDir + "TofUnpackPar.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> FHodoLabSetup: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> FHodoLabSetup: Initialising..." << std::endl;

  // NXyter Unpacker
  CbmTSUnpackFHodo*    test_unpacker     = new CbmTSUnpackFHodo();
  //  test_unpacker->CreateRawMessageOutput(kTRUE);
  
  // Get4 Unpacker
  CbmTSUnpackTof* test_unpacker_tof = new CbmTSUnpackTof();

  // --- Source task
  CbmFlibTestSource* source = new CbmFlibTestSource();
  source->SetFileName(inFile);
  source->AddUnpacker(test_unpacker_tof, 0x60, 20);//gDPB A & B
  source->AddUnpacker(test_unpacker,     0x10, 10);//nDPB A & B = HODO 1 + 2

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // -----   Runtime database   ---------------------------------------------
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
  std::cout << ">>> FHodoLabSetup: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;
    
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> FHodoLabSetup: Macro finished successfully." << std::endl;
  std::cout << ">>> FHodoLabSetup: Output file is " << outFile << std::endl;
  std::cout << ">>> FHodoLabSetup: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
