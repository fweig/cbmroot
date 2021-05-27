void run_digi(const string& mcFile   = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/mc.2.root",
              const string& parFile  = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/param.2.root",
              const string& digiFile = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/inmed/digi.2.root",
              int nEvents            = 1000)
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
  run.AddInput(mcFile.c_str(), eventRate);
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
