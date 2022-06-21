/* Copyright (C) 2019-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

void run_qa(const string& testType,  // "geotest" or "urqmdtest"
            const string& traFile, const string& parFile, const string& digiFile, const string& recoFile,
            const string& qaFile, const string& geoSetup, const string& resultDir, int nEvents)
{
  if (testType != "urqmdtest" && testType != "geotest") {
    std::cout << "ERROR testType is not correct. It must be urqmdtest or geotest" << std::endl;
    return;
  }

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  remove(qaFile.c_str());

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(geoSetup.c_str());

  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(recoFile.c_str());
  inputSource->AddFriend(traFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(qaFile.c_str());
  run->SetGenerateRunInfo(false);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile.c_str());
  run->AddTask(mcManager);

  if (testType == "geotest") {
    CbmRichGeoTest* geoTest = new CbmRichGeoTest();
    geoTest->SetDrawPmts(false);
    //geoTest->SetDrawEventDisplay(false);
    geoTest->SetOutputDir(resultDir);
    run->AddTask(geoTest);
  }
  else if (testType == "urqmdtest") {
    CbmRichUrqmdTest* urqmdTest = new CbmRichUrqmdTest();
    urqmdTest->SetOutputDir(resultDir);
    run->AddTask(urqmdTest);
  }

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
  std::cout << "Output file is " << qaFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
