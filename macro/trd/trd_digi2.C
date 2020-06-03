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
// Version     24/04/2007 (V. Friese)
//
// 20130605 - checked by DE
// --------------------------------------------------------------------------
void trd_digi2(Int_t nEvents = 1,
             const char* setupName = "sis100_electron")
//             const char* setupName = "sis100_hadron")
//               const char* setupName = "sis100_debug")
{

  gStyle->SetPalette(1,0);
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  FairLogger *logger = FairLogger::GetLogger();
  logger->SetLogFileName("MyLog.log");
  logger->SetLogToScreen(kTRUE);
  //  logger->SetLogToFile(kFALSE);
  //  logger->SetLogVerbosityLevel("HIGH");
  //  logger->SetLogFileLevel("DEBUG4");
  //  logger->SetLogScreenLevel("DEBUG2");
  logger->SetLogScreenLevel("INFO");

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // Input file (MC events)
  TString inFile = "data/test.mc.root";

  // Parameter file
  TString parFile = "data/params.root";

  // Output directory
  TString outDir = "data";

  // Output file
  TString outFile = "data/test.eds.root";

  TString inDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = inDir + "/parameters";

  TString setupFile = inDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);
  CbmSetup* cbmsetup = CbmSetup::Instance();

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
    TObjString* trdFile = new TObjString(inDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
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

   // Function needed for CTest runtime dependency
   TString depFile = Remove_CTest_Dependency_File(outDir, "trd_digi1");

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

  // =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

  // Update of the values for the radiator F.U. 17.08.07
  Int_t   trdNFoils = 130;    // number of polyethylene foils
  Float_t trdDFoils = 0.0013; // thickness of 1 foil [cm]
  Float_t trdDGap   = 0.02;   // thickness of gap between foils [cm]
  Bool_t  simpleTR  = kTRUE;  // use fast and simple version for TR production

  CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR, trdNFoils, trdDFoils, trdDGap);

  // -----   TRD hit producer   ----------------------------------------------
  CbmTrdDigitizerPRF* trdDigiPrf = new CbmTrdDigitizerPRF(radiator);
  run->AddTask(trdDigiPrf);

  CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
  run->AddTask(trdCluster);

  CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
  run->AddTask(trdHit);

//CbmTrdHitProducerSmearing* trdHitProd = new CbmTrdHitProducerSmearing(radiator);
  //run->AddTask(trdHitProd);

  //  CbmTrdDigitizer* trdDigitizer = new CbmTrdDigitizer(radiator);
  //  run->AddTask(trdDigitizer);
  //
  //  CbmTrdHitProducerDigi* trdHitProd = new CbmTrdHitProducerDigi();
  //  run->AddTask(trdHitProd);

  // -------------------------------------------------------------------------
  // ===                 End of TRD local reconstruction                   ===
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
