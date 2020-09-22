void run_qa_urqmdtest(
  const string& mcFile =
    "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/mc.00000.root",
  const string& parFile =
    "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/param.00000.root",
  const string& digiFile =
    "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/digi.00000.root",
  const string& recoFile =
    "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/reco.00000.root",
  const string& qaFile =
    "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/qa.00000.root",
  const string& geoSetup  = "sis100_electron",
  const string& resultDir = "results_urqmdtest_geant4/",
  int nEvents             = 1000) {
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_reco_urqmdtest";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(qaFile.c_str());

  TString setupFile  = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  TString setupFunct = "setup_" + geoSetup + "()";
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);

  CbmSetup* setup = CbmSetup::Instance();

  std::cout << std::endl
            << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();


  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(mcFile.c_str());
  inputSource->AddFriend(recoFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(qaFile.c_str());
  run->SetGenerateRunInfo(kTRUE);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile.c_str());
  run->AddTask(mcManager);


  CbmRichUrqmdTest* urqmdTest = new CbmRichUrqmdTest();
  urqmdTest->SetOutputDir(resultDir);
  run->AddTask(urqmdTest);


  std::cout << std::endl
            << std::endl
            << "-I- " << myName << ": Set runtime DB" << std::endl;
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
  std::cout << "Reco file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time "
            << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
