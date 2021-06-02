/** @file run_digi.C
  * @copyright Copyright (C) 2018-2020 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [committer], Andrey Lebedev, Semen Lebedev [committer] **/

void run_digi(const string& traFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/tra.00000.root",
              const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/par.00000.root",
              const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/raw.00000.root",
              int nEvents            = 3)
{
  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

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
  run.SetProduceNoise(false);
  run.SetMonitorFile("");

  CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  richDigitizer->SetMaxNofHitsPerPmtCut(65);
  run.SetDigitizer(ECbmModuleId::kRich, richDigitizer, "RichPoint", true);

  run.Run(nEvents);

  timer.Stop();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Digi file is " << digiFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
