/** @file ana.C
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 09.12.2014 
 ** Analysis
 */

void ana(Int_t runID, Int_t fileID)
{
	Int_t Version = 2014; // Cosy2014

	TString runN = TString::Itoa(runID,10);
	TString fileN = TString::Itoa(fileID,10);
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
	/*
	TString inDir = "/hera/cbm/users/anna/beamtime/2014/cbmroot/macro/beamtime/data/";
	TString inFile = inDir + "all_1stripCluster_20-28channelsSTS1.run" + runN + "_" + fileN + "_Cosy" + version + ".root";
	TString algnFile = inDir + "run" + runN + "_alignment_1stripCluster_20-28channelsSTS1" + "_Cosy" + version + ".root";
	algnFile = inDir + "run105_alignment_1stripCluster_20-28channelsSTS1" + "_Cosy" + version + ".root";	
	*/	
	TString Dir = "data/Cosy" + version + "/";
	TString inFile = Dir + "hits.run" + runN + "_" + fileN + ".root";
	
	// --- Specify output file name (this is just an example)
	TString outFile = Dir + "ana_50chi2_50x50bin.run" + runN + "_" + fileN + ".root";
	TString algnFile = Dir + "alignment.run" + runN + ".root";
	//outFile = Dir + "ana_1stripCluster_20-28channelsSTS1_58x116bin.run" + runN + "_" + fileN + ".root";
	
	// --- Specify file with time parameters for hit selection produced after StsHitSet (if used)
	TString cutFile = Dir + "hit_sets.run" + runN + ".root";
	cutFile = "";
	
	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> analysis:  input file is " << inFile  << std::endl;
  std::cout << ">>> analysis: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> analysis: Initialising..." << std::endl;
  
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

  // --- Residuals & correlations
  StsCosyAnalysis* analysis= new StsCosyAnalysis();
  analysis->SetAlignFileName(algnFile);
  if(cutFile != "" )analysis->SetCutFileName(cutFile); 
  else
  {
    analysis->SetTimeLimit(t_limits);
    analysis->SetTimeShift(t_shifts);
  }
  analysis->SetTrackSelectType(1);
  analysis->SetEventSelectType(1);
  analysis->SetNofDetectors(3); 
  analysis->SetChi2Cut(50.,50.);  
  analysis->SetCoordPrecision(50, 50);
  run->AddTask(analysis);
   
  run->SetWriteRunInfoFile(kFALSE);
  run->Init();

  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> analysis: Starting run..." << std::endl;
  run->Run(); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> analysis: Macro finished successfully." << std::endl;
  std::cout << ">>> analysis: Output file is " << outFile << std::endl;
  std::cout << ">>> analysis: Real time " << rtime << " s, CPU time "
  					<< ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
