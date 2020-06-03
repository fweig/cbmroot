/** @file cluster.C
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 2015 
 ** Clusters
 */

void cluster(Int_t runID, Int_t fileID)
{  
	Int_t Version = 2014; // Cosy2014

	TString runN = TString::Itoa(runID,10);
	TString fileN = TString::Itoa(fileID,10);
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
	
	TString Dir = "data/Cosy" + version + "/";
	TString inFile = Dir + "calib.run" + runN + "_" + fileN + ".root";
	
	// --- Specify file with time parameters for cluster building produced after StsClusterSet (if used)
	TString cutFile = Dir + "clust_sets.run" + runN + ".root";
	//TString cutFile = "";
	
	// --- Specify output file name (this is just an example)
	TString outFile = Dir + "clusters.run" + runN + "_" + fileN + ".root";
	
	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> cluster:  input file is " << inFile  << std::endl;
  std::cout << ">>> cluster: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> cluster: Initialising..." << std::endl;
  
  // --- Sets
  Bool_t trigger = kTRUE;   // kFALSE - self-trigger; kTRUE - external trigger
  Int_t triggered_station = 1;
  
  // --- Time parameterts if Set-function will be used here (for run 105)
  Double_t t_limits[]={25., 10., 24.}; 
  Double_t t_shifts[]={0., 0., 0.};
 
  // --- Charge value for signal strips
  Double_t min_charge[] = {50., 70., 50.}; // min value for strip charge
  Double_t max_charge[] = {500., 500., 500.}; // max value for strip charge
  
  // --- Min value for cluster charge (additional threshold)
  Double_t min_charge_cluster[] = {100., 100., 100.}; 
  
  // --- Run
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);

  // --- Clusters
  StsCosyClusterFinder* sts_clust= new StsCosyClusterFinder();
  sts_clust->SetTriggeredMode(trigger);
  sts_clust->SetTriggeredStation(triggered_station);
  
  if(cutFile != "" )sts_clust->SetCutFileName(cutFile); 
  else
  {
    sts_clust->SetTimeLimit(t_limits);
    sts_clust->SetTimeShift(t_shifts);  
  }
  
  sts_clust->SetChargeLimitsStrip(min_charge, max_charge);
  sts_clust->SetChargeMinCluster(min_charge_cluster);
  run->AddTask(sts_clust);
  
  run->Init();
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> cluster: Starting run..." << std::endl;
  run->Run(); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> cluster: Macro finished successfully." << std::endl;
  std::cout << ">>> cluster: Output file is " << outFile << std::endl;
  std::cout << ">>> cluster: Real time " << rtime << " s, CPU time "
  					<< ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
