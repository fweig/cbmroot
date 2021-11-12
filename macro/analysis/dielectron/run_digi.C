/* Copyright (C) 2020-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

// Run this macro with run_local.py for local test and with batch_send(job).py for large productions
void run_digi(const std::string& traFile, const std::string& parFile, const std::string& digiFile, int nEvents)
{
  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  double eventRate       = 1.e7;
  double timeSliceLength = 1.e4;
  bool eventMode         = true;
  bool overwrite         = true;

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
  run.Run(nEvents);

  timer.Stop();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Digi file is " << digiFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
