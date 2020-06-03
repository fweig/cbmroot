/** @file readTsa.C
 ** @author Martin Kohn <martin.kohn@cern.ch>
 ** @since June 2017
 ** @date 02.06.2017
 **
 ** adapted original File from Florian Uhlig <f.uhlig@gsi.de> for new analysis chain and bucharest spadic data.
 */


void readTsaQA_buch(TString inFile = 
	      "/opt/cbm/testdata/237_spa11_2016.tsa"
)
{

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
  CbmTSUnpackSpadic* spadic_unpacker = new CbmTSUnpackSpadic();

  // --- Source task
  CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();
  //source->SetHostName("cbmflib01");
  source->SetFileName(inFile);
  source->AddUnpacker(spadic_unpacker, 0x40);  // Lab münster
  // --- Event header
  //  FairEventHeader* event = new CbmTbEvent();
  //  event->SetRunId(260);


  // this the very crucial part, choose here the right settings for
  CbmTrdTestBeamTools::Instance(new CbmTrdTestBeamToolsBuch2016);
  //CbmTrdTestBeamTools::Instance(new CbmTrdLabTools); 

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  //  run->SetEventHeader(event);

  FairTask* spadicRawBeam = new CbmTrdQABase();
  run->AddTask(spadicRawBeam);

  FairTask* HitAnalysis = new CbmTrdQAHit();
  run->AddTask(HitAnalysis);
  /*  
  FairTask* Baseline=new CbmTrdQABaseline();
  run->AddTask(Baseline);
  */
   FairTask* digitize=new CbmTrdSimpleDigitizer();
  run->AddTask(digitize);
  /*
  FairTask* Clusterrize=new CbmTrdSimpleClusterizer();
  //  run->AddTask(Clusterrize);

  FairTask* ClusterAnalysis=new CbmTrdClusterAnalysis();
  //  run->AddTask(ClusterAnalysis);
  */

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
}
