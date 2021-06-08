/* Copyright (C) 2017-2018 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

void run_rich_reco_tb()
{
  TTree::SetMaxTreeSize(90000000000);
  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_rich_reco_tb";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString geoSetupFile = srcDir + "/macro/rich/geosetup/rich_setup_sis100_tb.C";

  TString outDir        = "/Users/slebedev/Development/cbm/data/sim/rich/tb/";
  TString mcFile        = outDir + "mc.00001.root";
  TString parFile       = outDir + "param.00001.root";
  TString digiFile      = outDir + "digi.00001.root";
  TString recoFile      = outDir + "reco.00001.root";
  std::string resultDir = "results_geotest/";


  // Specify interaction rate in 1/s
  Double_t eventRate = 1.e4;
  // Specify duration of time slices in output [ns]
  Double_t timeSliceSize = 1.e8;

  remove(recoFile.Data());

  //   TString setupFunct = "do_setup()";
  //   std::cout << "-I- " << myName << ": Loading macro " << geoSetupFile << std::endl;
  //   gROOT->LoadMacro(geoSetupFile);
  //   gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();


  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  CbmRunAna* run              = new CbmRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile);
  run->SetSource(inputSource);
  run->SetAsync();
  run->AddFriend(mcFile);
  run->SetOutputFile(recoFile);
  //run->SetEventMeanTime(1.e9 / eventRate);
  FairRootManager::Instance()->SetUseFairLinks(kTRUE);

  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", false);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);

  //   run->AddTask(new CbmBuildEventsIdeal());

  CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
  run->AddTask(richHitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->SetRunExtrapolation(false);
  richReco->SetRunProjection(false);
  richReco->SetRunFinder(true);
  richReco->SetRunFitter(true);
  richReco->SetRunTrackAssign(false);
  run->AddTask(richReco);

  CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
  run->AddTask(matchRecoToMc);

  CbmRichRecoTbQa* richRecoTbQa = new CbmRichRecoTbQa();
  run->AddTask(richRecoTbQa);

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
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
  run->Run();


  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
