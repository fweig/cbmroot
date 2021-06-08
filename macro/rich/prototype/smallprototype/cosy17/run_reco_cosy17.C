/* Copyright (C) 2017 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

void run_reco_cosy17(Int_t nEvents = 500)
{
  TTree::SetMaxTreeSize(90000000000);
  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_reco_geotest";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString dir      = "/Users/slebedev/Development/cbm/data/sim/rich/richprototype/";
  TString parFile  = dir + "/param.00001.root";
  TString recoFile = dir + "/reco.00001.root";
  TString mcFile   = dir + "/mc.00001.root";
  string resultDir = "results_cosy17/";

  remove(recoFile.Data());

  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(mcFile);
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile);
  run->SetGenerateRunInfo(kTRUE);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);

  CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  richDigitizer->SetNofNoiseHits(0);
  run->AddTask(richDigitizer);

  CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
  richHitProd->SetRotationNeeded(false);
  run->AddTask(richHitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->SetRunExtrapolation(false);
  richReco->SetRunProjection(false);
  richReco->SetRunTrackAssign(false);
  richReco->SetFinderName("ideal");
  richReco->SetFitterName("circle_cop");
  run->AddTask(richReco);

  CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
  run->AddTask(matchRecoToMc);

  CbmRichSmallPrototypeQa* richQa = new CbmRichSmallPrototypeQa();
  richQa->SetOutputDir(std::string(resultDir));
  run->AddTask(richQa);


  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
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
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
