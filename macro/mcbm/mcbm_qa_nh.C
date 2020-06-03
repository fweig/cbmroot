// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
//
// HitProducers in MVD, RICH, TRD, TOF, ECAL
// Digitizer and HitFinder in STS
// FAST MC for ECAL
// STS track finding and fitting (L1 / KF)
// TRD track finding and fitting (L1 / KF)
// RICH ring finding (ideal) and fitting
// Global track finding (ideal), rich assignment
// Primary vertex finding (ideal)
// Matching of reconstructed and MC tracks in STS, RICH and TRD
//
// V. Friese   24/02/2006
// Version     04/03/2015 (V. Friese)
//
// --------------------------------------------------------------------------

void mcbm_qa_nh(Int_t nEvents = 1000, 
	       TString cSys="nini", 
	       TString cEbeam="1.93gev",
	       TString cCentr="mbias",
	       Int_t   iRun=986,
	       const char* setup = "sis18_mcbm_20deg_long")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  TString outDir  = "data/";
  TString inFile  = outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".mc." + Form("%05d",iRun) + ".root"; // Input file (MC events)
  TString parFile = outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".params." + Form("%05d",iRun) + ".root";  // Parameter file
  TString outFile = outDir + setup + "_" + cSys + "." + cEbeam + "." + cCentr + ".eds." + Form("%05d",iRun) + ".root";     // Output file

  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");


  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------
	
  CbmStsMCQa* stsQa = new CbmStsMCQa();
  run->AddTask(stsQa);
  // ------------------------------------------------------------------------
	
  CbmTrdMCQa* trdQa = new CbmTrdMCQa();
  run->AddTask(trdQa);
  // -------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
//  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
//  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------


  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  //  delete run;

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  RemoveGeoManager();
}
