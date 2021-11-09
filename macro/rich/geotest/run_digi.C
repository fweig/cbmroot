/* Copyright (C) 2018-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

void run_digi(const string& traFile, const string& parFile, const string& digiFile, int nEvents)
{
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  Double_t eventRate       = 1.e7;
  Double_t timeSliceLength = 1.e4;
  Bool_t eventMode         = true;
  Bool_t overwrite         = true;

  remove(digiFile.c_str());

  TStopwatch timer;
  timer.Start();

  CbmDigitization run;
  run.AddInput(traFile.c_str(), eventRate);
  run.SetOutputFile(digiFile.c_str(), overwrite);
  run.SetParameterRootFile(parFile.c_str());
  run.SetTimeSliceLength(timeSliceLength);
  run.SetEventMode(eventMode);
  run.SetMonitorFile("");

  CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  richDigitizer->SetMaxNofHitsPerPmtCut(65);
  richDigitizer->SetNoiseDigiRate(0.);
  run.SetDigitizer(ECbmModuleId::kRich, richDigitizer, "RichPoint", true);

  run.Run(nEvents);

  timer.Stop();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Digi file is " << digiFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
