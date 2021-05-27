void run_digi_geotest(const string& mcFile   = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/mc.00000.root",
                      const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/param.00000.root",
                      const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/digi.00000.root",
                      int nEvents            = 10000)
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
  run.AddInput(mcFile.c_str(), eventRate);
  run.SetOutputFile(digiFile.c_str(), overwrite);
  run.SetParameterRootFile(parFile.c_str());
  run.SetTimeSliceLength(timeSliceLength);
  run.SetEventMode(eventMode);
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
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
