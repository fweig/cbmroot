/* Copyright (C) 2013-2021 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Andrey Lebedev [committer] */

/**
 * \file radlength_ana.C
 * \brief Macro runs radiathin length QA task.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 */
void radlength_ana(const string& mcFile    = "/Users/slebedev/Development/cbm/data/sim/rich/radlen/mc.ac.root",
                   const string& parFile   = "/Users/slebedev/Development/cbm/data/sim/rich/radlen/param.ac.root",
                   const string& radqaFile = "/Users/slebedev/Development/cbm/data/sim/rich/radlen/radqa.ac.root",
                   const string& resultDir = "results_radlen_ac/",
                   const string& geoSetup = "sis100_electron_rich_pal_bcarb", Int_t nEvents = 12100000)
{

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  remove(radqaFile.c_str());

  TString setupFile  = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  TString setupFunct = "setup_" + geoSetup + "()";
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);

  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(mcFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(radqaFile.c_str());
  run->SetGenerateRunInfo(kTRUE);


  CbmLitRadLengthQa* radLengthQa = new CbmLitRadLengthQa();
  radLengthQa->SetOutputDir(resultDir);
  run->AddTask(radLengthQa);

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
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Radqa file is " << radqaFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
