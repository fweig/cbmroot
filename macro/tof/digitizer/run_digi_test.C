// --------------------------------------------------------------------------
//
// Macro for TOF digitzer testing
//
// Digitizer in TOF
// Clusterizer in TOF
// Test class for TOF (Point/Digi/Hit)
//
// P.-A. Loizeau   13/03/2015
// Version         13/03/2015 (P.-A. Loizeau)
//
// --------------------------------------------------------------------------
void run_digi_test(Int_t nEvents = 2, const char* setup = "sis100_electron")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");

  TString outDir  = "data/";

  // Input file (MC events)
  TString inFile = outDir + setup + "_test.mc.root";
  // Parameter file
  TString parFile = outDir + setup + "_params.root";

  // Output file
  TString outFile = outDir + setup + "_digitizerTest.eds.root";

  TString setupFile = inDir + "/geometry/setup/setup_"+ setup +".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setup + "()";
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);
  CbmSetup* cbmsetup = CbmSetup::Instance();

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters/";

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly 
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if ( cbmsetup->GetGeoTag(kTrd, geoTag) ) {
    TObjString* trdFile = new TObjString(inDir + "/parameters/trd/trd_" + geoTag + ".asic.par");
    parFileList->Add(trdFile);
    trdFile = new TObjString(inDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
    parFileList->Add(trdFile);
    trdFile = new TObjString(inDir + "/parameters/trd/trd_" + geoTag + ".gain.par");
    parFileList->Add(trdFile);
    trdFile = new TObjString(inDir + "/parameters/trd/trd_" + geoTag + ".gas.par");
    parFileList->Add(trdFile);
    std::cout << "-I- Using parameter file "
              << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if ( cbmsetup->GetGeoTag(kTof, geoTag) ) {
    TObjString* tofFile = new TObjString(inDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
    parFileList->Add(tofFile);
    std::cout << "-I- Using parameter file "
              << tofFile->GetString() << std::endl;
    TObjString* tofBdfFile = new TObjString(inDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- Using parameter file "
              << tofBdfFile->GetString() << std::endl;
  }

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
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) {
    FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  }
  // ------------------------------------------------------------------------


  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================

  // -----   TOF digitizer   -------------------------------------------------
  CbmTofDigitize* tofDigitizer = new CbmTofDigitize("TOF Digitizer",iVerbose);
  tofDigitizer->SetOutputBranchPersistent("TofDigi",            kFALSE);
  tofDigitizer->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
  tofDigitizer->SetInputFileName( paramDir + "tof/test_bdf_input.root"); // Required as input file name not read anymore by param class
  tofDigitizer->UseMcTrackMonitoring(); // Enable the Mc Track Usage in the digitizer
  run->AddTask(tofDigitizer);


  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================

  // Cluster/Hit builder
  CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer",iVerbose);
  tofSimpClust->SetOutputBranchPersistent("TofHit",          kFALSE);
  tofSimpClust->SetOutputBranchPersistent("TofDigiMatch",    kFALSE);
  tofSimpClust->UseMcTrackMonitoring(); // Enable the Mc Track Usage in the clusterizer
  run->AddTask(tofSimpClust);

  // ------   TOF hit producer   ---------------------------------------------
/*
  CbmTofHitProducerNew* tofHitProd = new CbmTofHitProducerNew("TOF HitProducerNew",iVerbose);
  //  tofHitProd->SetParFileName(std::string(TofGeoPar));
  tofHitProd->SetInitFromAscii(kFALSE);
//  tofHitProd->SetSigmaT(  0.070 );
//  tofHitProd->SetSigmaEl( 0.040 );
  run->AddTask(tofHitProd);
*/
  // -------------------------------------------------------------------------

  // ===                   End of TOF local reconstruction                 ===
  // =========================================================================
  
  // =========================================================================
  // ===                    Matching to Monte-carlo                        ===
  // =========================================================================
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // ===                  End of matching to Monte-Carlo                   ===
  // =========================================================================

  // =========================================================================
  // ===                     TOF evaluation                                ===
  // =========================================================================

  // Digitizer/custerizer testing
  CbmTofTests* tofTests = new CbmTofTests("TOF Tests",iVerbose);
  run->AddTask(tofTests);

  // ===                   End of TOF evaluation                           ===
  // =========================================================================
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
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

  if (hasFairMonitor) {
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory=sysInfo.GetMaxMemory();
    cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    cout << maxMemory;
    cout << "</DartMeasurement>" << endl;

    Float_t cpuUsage=ctime/rtime;
    cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    cout << cpuUsage;
    cout << "</DartMeasurement>" << endl;

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }
  //  delete run;

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
  
  RemoveGeoManager();
}
