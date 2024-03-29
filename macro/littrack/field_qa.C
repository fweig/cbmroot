/* Copyright (C) 2009-2012 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file field_qa.C
 * \brief Macro runs CbmLitFieldQa task which checks field map and its approximation.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 *
 **/

#include <iostream>

void field_qa(Int_t nEvents = 1)
{
  TString script = TString(gSystem->Getenv("LIT_SCRIPT"));

  TString dir         = "./commit_tests/events_electron/";  // Output directory
  TString mcFile      = dir + "mc.0000.root";               // MC transport file
  TString parFile     = dir + "param.0000.root";            // Parameter file
  TString fieldQaFile = dir + "field.qa.0000.root";         // Output file
  TString resultDir   = "./test";

  if (script == "yes") {
    mcFile      = TString(gSystem->Getenv("LIT_MC_FILE"));
    parFile     = TString(gSystem->Getenv("LIT_PAR_FILE"));
    fieldQaFile = TString(gSystem->Getenv("LIT_FIELD_QA_FILE"));
    resultDir   = TString(gSystem->Getenv("LIT_RESULT_DIR"));
  }

  TStopwatch timer;
  timer.Start();

  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
  loadlibs();

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(mcFile);
  run->SetOutputFile(fieldQaFile);
  // ------------------------------------------------------------------------

  CbmLitFieldQa* fieldQa = new CbmLitFieldQa();

  std::vector<Double_t> zPos;
  zPos.push_back(0.);
  zPos.push_back(50.);
  zPos.push_back(100.);
  zPos.push_back(150.);
  zPos.push_back(200.);
  fieldQa->SetSliceZPosition(zPos);
  fieldQa->SetAcceptanceAngleX(35.);
  fieldQa->SetAcceptanceAngleY(35.);
  fieldQa->SetNofBinsX(30);
  fieldQa->SetNofBinsY(30);
  fieldQa->SetZMin(-10);
  fieldQa->SetZMax(300);
  fieldQa->SetZStep(5);
  fieldQa->SetOutputDir(std::string(resultDir));
  run->AddTask(fieldQa);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  // -----   Initialize and run   --------------------------------------------
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << fieldQaFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  // ------------------------------------------------------------------------
}
