/* Copyright (C) 2010-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Semen Lebedev [committer] */

void run_analysis(const string& mcFile        = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/mc.1.root",
                  const string& parFile       = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/param.1.root",
                  const string& digiFile      = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/digi.1.root",
                  const string& recoFile      = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/reco.1.root",
                  const string& analysisFile  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/analysis.1.root",
                  const string& plutoParticle = "inmed", const string& colSystem = "auau",
                  const string& colEnergy = "8gev", const string& geoSetup = "sis100_electron", int nEvents = 1000)
{
  /*cout << "Here: just entered run_ana.C" << endl;
  Int_t Interval=10;
  Int_t PID=gSystem->GetPid();
  cout<<"PID: "<<PID<<endl;
  TString cmdline="$VMCWORKDIR/macro/analysis/dielectron/check_memory.sh ";
  cmdline+= PID;
  cmdline+= " ";
  cmdline+= Interval;
  cmdline+= "  &";
  cout<<cmdline<<endl;
  gSystem->Exec(cmdline);*/


  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_analysis";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(analysisFile.c_str());

  CbmSetup::Instance()->LoadSetup(geoSetup.c_str());

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
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
  run->SetGenerateRunInfo(kFALSE);

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
  task->SetEnergyAndPlutoParticle(colEnergy, plutoParticle);
  task->SetUseMvd(false);
  task->SetUseRich(true);
  task->SetUseTrd(true);
  task->SetUseTof(true);
  // task->SetPionMisidLevel(pionMisidLevel);
  // task->SetTrdAnnCut(0.85);
  // task->SetRichAnnCut(-0.4);
  run->AddTask(task);
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

  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Analysis file is " << analysisFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
