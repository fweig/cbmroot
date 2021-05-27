/** @file run_rich_digi_tb.C
  * @copyright Copyright (C) 2017-2018 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [orginator] **/


void run_rich_digi_tb(Int_t nEvents = 1000)
{

  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  Bool_t overwrite   = kTRUE;
  Double_t eventRate = 1.e7;  // Interaction rate [1/s]
  // workaround for the moment QA works if only one time slice exists for one mc file
  Double_t timeSliceLength = 1.e4;    // 1.e4;   // Length of time-slice [ns]
  Bool_t eventMode         = kFALSE;  // Event-by-event mode

  TString outDir   = "/Users/slebedev/Development/cbm/data/sim/rich/tb/";
  TString mcFile   = outDir + "mc.00001.root";
  TString parFile  = outDir + "param.00001.root";
  TString digiFile = outDir + "digi.00001.root";

  TStopwatch timer;
  timer.Start();

  // Run digitization
  CbmDigitization run;

  run.AddInput(mcFile, eventRate);
  run.SetOutputFile(digiFile, overwrite);
  run.SetParameterRootFile(parFile);
  run.SetTimeSliceLength(timeSliceLength);
  run.SetEventMode(eventMode);

  CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  //richDigitizer->SetPixelDeadTime(0.);
  run.SetDigitizer(kRich, richDigitizer);

  run.Deactivate(kSts);

  run.Run(nEvents);


  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Digi file is " << digiFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
