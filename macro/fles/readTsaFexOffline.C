

void readTsaFexOffline(TString inFile =
  //"data/43_sps2016.tsa"
  //"/Users/fairbanks/fairbanks/dev/data/tsa/14_debug_spadic1p1.tsa"
        "/Volumes/fairbanksNAS-1/iri/data/testbeams/2016/sps/118_sps2016.tsa"
        ,Bool_t highP = false
)
{

  // --- Specify input file name (this is just an example)
  //TString inFile = "spadic_dlm_trigger_2014-11-15_noepoch.tsa";
  //TString inFile1 = "spadic_noise_trigger_2014-11-15_withepoch.tsa";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = inFile;//"data/test_online.root";
  outFile.ReplaceAll(".tsa",".root");

  // --- Set log output levels
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> readTsa:  input file is " << inFile  << std::endl;
  std::cout << ">>> readTsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> readTsa: Initialising..." << std::endl;

  // Spadic Unpacker
  //CbmTSUnpackSpadic11OnlineMonitor* spadic_unpacker = new CbmTSUnpackSpadic11OnlineMonitor(highP);
  CbmTSUnpackSpadicOnlineFex* spadic_unpacker = new CbmTSUnpackSpadicOnlineFex(highP);

  // NXyter Unpacker
  CbmTSUnpackNxyter* nxyter_unpacker = new CbmTSUnpackNxyter();

  CbmTSUnpackTrb* trb_unpacker = new CbmTSUnpackTrb();

  // --- Source task
  CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();
  //source->SetHostName("cbmflib01");
  source->SetFileName(inFile);
  //source->AddFile(inFile1);
  source->AddUnpacker(trb_unpacker, 0xE0);// RICH + REF
  //source->AddUnpacker(nxyter_unpacker, 0x10);//fhodo or cherenkov or pb glass???
  source->AddUnpacker(nxyter_unpacker, 0xE1);//HODO 1 + 2
  source->AddUnpacker(spadic_unpacker, 0x40);// test beam 2014
  //source->AddUnpacker(spadic_unpacker, 0xE0);  // Lab münster

  // --- Event header
  //  FairEventHeader* event = new CbmTbEvent();
  //  event->SetRunId(260);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->ActivateHttpServer(100);
  run->SetAutoFinish(kFALSE);
  //  run->SetEventHeader(event);

  //  gDebug=2;
  //FairTask* spadicRawBeam = new CbmTrdTimeCorrel();
  //run->AddTask(spadicRawBeam);

  run->Init();


  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> readTsa: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> readTsa: Macro finished successfully." << std::endl;
  std::cout << ">>> readTsa: Output file is " << outFile << std::endl;
  std::cout << ">>> readTsa: Real time " << rtime << " s, CPU time "
            << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
