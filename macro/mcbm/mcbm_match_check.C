void mcbm_match_check(Int_t nEvents = 10, TString setup = "mcbm_beam_2020_03", const char* output = "data/test")
{
  TString dataset(output);
  TString InputFile = dataset + ".tra.root";
  TString DigiFile  = dataset + ".event.raw.root";
  TString RecoFile  = dataset + ".rec.root";
  TString ParFile   = dataset + ".par.root";
  TString OutFile   = dataset + ".match_check.root";

  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("HIGH");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairFileSource* inputFiles = new FairFileSource(InputFile.Data());
  inputFiles->AddFriend(DigiFile.Data());
  inputFiles->AddFriend(RecoFile.Data());

  FairRootFileSink* sink = new FairRootFileSink(OutFile.Data());

  FairRunAna* fRun = new FairRunAna();
  fRun->SetSource(inputFiles);
  fRun->SetSink(sink);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 1);
  mcManager->AddFile(InputFile);
  fRun->AddTask(mcManager);

  CbmMatchRecoToMC* match = new CbmMatchRecoToMC();
  fRun->AddTask(match);

  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  cout << "Starting run" << endl;
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // save histos to file
  TFile* fHist = fRun->GetOutputFile();
  fHist->Write();

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << OutFile << std::endl;
  std::cout << "Parameter file is " << ParFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------

  // -----   Resource monitoring   ------------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;


  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

  RemoveGeoManager();
}
