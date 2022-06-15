/* Copyright (C) 2010-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

// Run this macro with run_local.py for local test and with batch_send(job).py for large productions
void run_analysis(const std::string& traFile, const std::string& parFile, const std::string& digiFile,
                  const std::string& recoFile, const std::string& analysisFile, const std::string& plutoParticle,
                  const std::string& colSystem, const std::string& colEnergy, const std::string& geoSetup, int nEvents)
{
  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(analysisFile.c_str());

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(geoSetup.c_str());

  bool useMvd = geo->IsActive(ECbmModuleId::kMvd);

  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(recoFile.c_str());
  inputSource->AddFriend(traFile.c_str());
  inputSource->AddFriend(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(analysisFile.c_str());
  run->SetGenerateRunInfo(kFALSE);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile.c_str());
  run->AddTask(mcManager);
  run->AddTask(new CbmTrackingDetectorInterfaceInit());
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  run->AddTask(l1);

  // --- Material budget file names
  TString mvdGeoTag;
  if (useMvd && geo->GetGeoTag(ECbmModuleId::kMvd, mvdGeoTag)) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile += "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(parFile.Data());
  }
  TString stsGeoTag;
  if (geo->GetGeoTag(ECbmModuleId::kSts, stsGeoTag)) {
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile += "/parameters/sts/sts_matbudget_" + stsGeoTag + ".root";
    std::cout << "Using material budget file " << parFile << std::endl;
    l1->SetStsMaterialBudgetFileName(parFile.Data());
  }

  LmvmTask* task = new LmvmTask();
  task->SetEnergyAndPlutoParticle(colEnergy, plutoParticle);
  task->SetUseMvd(useMvd);
  // task->SetPionMisidLevel(pionMisidLevel);
  // task->SetTrdAnnCut(0.85);
  // task->SetRichAnnCut(-0.4);
  run->AddTask(task);


  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  run->Run(0, nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Analysis file is " << analysisFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
