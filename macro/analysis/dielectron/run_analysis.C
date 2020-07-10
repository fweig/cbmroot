void run_analysis(
  const string& mcFile =
    "/lustre/nyx/cbm/users/slebedev/cbm/data/mc.00000.root",
  const string& parFile =
    "/lustre/nyx/cbm/users/slebedev/cbm/data/param.00000.root",
  const string& digiFile =
    "/lustre/nyx/cbm/users/slebedev/cbm/data/digi.00000.root",
  const string& recoFile =
    "/lustre/nyx/cbm/users/slebedev/cbm/data/reco.00000.root",
  const string& analysisFile =
    "/lustre/nyx/cbm/users/slebedev/cbm/data/analysis.00000.root",
  const string& plutoParticle = "omegaepem",
  const string& energy        = "8gev",
  const string& geoSetup      = "sis100_electron",
  int nEvents                 = 100) {

  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_analysis";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(analysisFile.c_str());

  TString setupFile  = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  TString setupFunct = "setup_" + geoSetup + "()";
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);

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
  run->SetOutputFile(analysisFile.c_str());
  run->SetGenerateRunInfo(kTRUE);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile.c_str());
  run->AddTask(mcManager);

  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  run->AddTask(l1);

  CbmAnaDielectronTask* task = new CbmAnaDielectronTask();
  task->SetEnergyAndPlutoParticle(energy, plutoParticle);
  task->SetUseMvd(false);
  task->SetUseRich(true);
  task->SetUseTrd(true);
  task->SetUseTof(true);
  // task->SetPionMisidLevel(pionMisidLevel);
  // task->SetTrdAnnCut(0.85);
  // task->SetRichAnnCut(-0.4);
  run->AddTask(task);

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

  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time "
            << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}