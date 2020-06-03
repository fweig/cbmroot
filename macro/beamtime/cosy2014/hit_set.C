/** @file hit_set.C
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 2015 
 ** Sets for hit building
 */

void hit_set(Int_t runID, Int_t fileID)
{  
	Int_t Version = 2014; // Cosy2014

	TString runN = TString::Itoa(runID,10);
	TString fileN = TString::Itoa(fileID,10);
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
	
	TString Dir = "data/Cosy" + version + "/";
	TString inFile = Dir + "clusters.run" + runN + "_" + fileN + ".root";

	// --- Specify output file name (this is just an example)
	TString outFile = Dir + "hit_sets.run" + runN + ".root";
	
	
	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> hit_set:  input file is " << inFile  << std::endl;
  std::cout << ">>> hit_set: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> hit_set: Initialising..." << std::endl;
  

  // --- Run
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  
  StsHitSet* set = new StsHitSet();
  run->AddTask(set);

  run->Init();
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> hit_set: Starting run..." << std::endl;
  run->Run(); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> hit_set: Macro finished successfully." << std::endl;
  std::cout << ">>> hit_set: Output file is " << outFile << std::endl;
  std::cout << ">>> hit_set: Real time " << rtime << " s, CPU time "
  					<< ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
