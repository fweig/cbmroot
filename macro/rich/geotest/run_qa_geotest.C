
void run_qa_geotest(const string& mcFile   = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/mc.00000.root",
                    const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/param.00000.root",
                    const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/digi.00000.root",
                    const string& recoFile = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/reco.00000.root",
                    const string& qaFile   = "/Users/slebedev/Development/cbm/data/sim/rich/"
                                           "geotest/qa.g3new_media0.00000.root",
                    const string& geoSetup  = "sis100_electron",
                    const string& resultDir = "results_geotest_geant3_new_media0/", int nEvents = 10000)
{
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_qa_geotest";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  remove(qaFile.c_str());

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
  inputSource->AddFriend(recoFile.c_str());
  inputSource->AddFriend(mcFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(qaFile.c_str());
  run->SetGenerateRunInfo(kTRUE);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile.c_str());
  run->AddTask(mcManager);

  CbmRichGeoTest* geoTest = new CbmRichGeoTest();
  geoTest->SetDrawPmts(false);
  //geoTest->SetDrawEventDisplay(false);
  geoTest->SetOutputDir(resultDir);
  run->AddTask(geoTest);

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
  std::cout << "Output file is " << qaFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
