/** @file alignment.C
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 26.01.2015 
 ** Alignment
 */

void alignment(Int_t runID, Int_t fileID)
{  
	Int_t Version = 2014; // Cosy2014

	TString runN = TString::Itoa(runID,10);
	TString fileN = TString::Itoa(fileID,10);
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
	
	TString Dir = "data/Cosy" + version + "/";
	TString inFile = Dir + "hits.run" + runN + "_" + fileN + ".root";
	
	// --- Specify output file name (this is just an example)
	TString outFile = Dir + "alignment.run" + runN + ".root";
	
	// --- Specify file with time parameters for hit selection produced after StsHitSet (if used)
	TString cutFile = Dir + "hit_sets.run" + runN + ".root";
	cutFile = "";
	
	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> alignment:  input file is " << inFile  << std::endl;
  std::cout << ">>> alignment: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> alignment: Initialising..." << std::endl;
  
  // --- Time parameterts if Set-function will be used here (for run 105)
  /*
  Double_t t_limits[]={20., 18., 20.};
  Double_t t_shifts[]={-15., 0., -15.};
*/
  // for run 106
  Double_t t_limits[]={10., 18., 12.};
  Double_t t_shifts[]={26., 0., 26.};

  // --- Run
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
 
  StsAlignment* align = new StsAlignment();
  // align->SetChi2Cut(200.,200.); 
   align->SetNofDetectors(3);
  if(cutFile != "" )align->SetCutFileName(cutFile); 
  else
  {
    align->SetTimeLimit(t_limits);
    align->SetTimeShift(t_shifts);
  }
  run->AddTask(align);

  run->Init();
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> alignment: Starting run..." << std::endl;
  run->Run(); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> alignment: Macro finished successfully." << std::endl;
  std::cout << ">>> alignment: Output file is " << outFile << std::endl;
  std::cout << ">>> alignment: Real time " << rtime << " s, CPU time "
  					<< ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}