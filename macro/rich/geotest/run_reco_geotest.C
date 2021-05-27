
void run_reco_geotest(const string& mcFile   = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/mc.00000.root",
                      const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/param.00000.root",
                      const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/digi.00000.root",
                      const string& recoFile = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/reco.00000.root",
                      const string& geoSetup = "sis100_electron",
                      const string& resultDir = "rich_pipe_v1",  // "results_geotest_test/",
                      int nEvents             = 10000)
{
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_reco_geotest";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  remove(recoFile.c_str());

  TString setupFile  = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  TString setupFunct = "setup_" + geoSetup + "()";
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(mcFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(kTRUE);


  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile.c_str());
  run->AddTask(mcManager);

  CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
  run->AddTask(richHitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->SetRunExtrapolation(false);
  richReco->SetRunProjection(false);
  richReco->SetRunTrackAssign(false);
  richReco->SetFinderName("ideal");
  run->AddTask(richReco);

  CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
  run->AddTask(matchRecoToMc);

  // CbmRichGeoTest* geoTest = new CbmRichGeoTest();
  // geoTest->SetOutputDir(resultDir);
  // run->AddTask(geoTest);

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }


  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();


  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();


  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);


  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
