/** @file hit.C
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 2015 
 ** Hits
 */

void hit(Int_t runID, Int_t fileID)
{  
	Int_t Version = 2014; // Cosy2014

	TString runN = TString::Itoa(runID,10);
	TString fileN = TString::Itoa(fileID,10);
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
	TString Dir = "data/Cosy" + version + "/";
	TString inFile = Dir + "clusters.run" + runN + "_" + fileN + ".root";

	// --- Specify output file name (this is just an example)
	TString outFile = Dir + "hits.run" + runN + "_" + fileN + ".root";

	// --- Specify file with time parameters for hit building produced after StsHitSet (if used)
	TString cutFile = Dir + "hit_sets.run" + runN + ".root";
	//cutFile = "";

	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> readLmd:  input file is " << inFile  << std::endl;
  std::cout << ">>> readLmd: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> readLmd: Initialising..." << std::endl;
  
  // --- Sets
  Int_t sensorID = 18;

  // --- Time parameterts if Set-function will be used here (for run 105)
  Double_t t_limits[]={18., 10., 20.};
  Double_t t_shifts[]={-4., -5., -6.};
  
  // --- Run
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);

  // --- Hits
  StsCosyHitFinder* hits= new StsCosyHitFinder();
  hits->SetZ(25.3, 99.3, 140.7); 
  if(cutFile != "" )hits->SetCutFileName(cutFile); 
  else
  {
    hits->SetTimeLimit(t_limits);
    hits->SetTimeShift(t_shifts);
  }
  hits->SetSensorId(sensorID);
  run->AddTask(hits);
  
  run->Init();
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> readLmd: Starting run..." << std::endl;
  run->Run(); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> readLmd: Macro finished successfully." << std::endl;
  std::cout << ">>> readLmd: Output file is " << outFile << std::endl;
  std::cout << ">>> readLmd: Real time " << rtime << " s, CPU time "
  					<< ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
