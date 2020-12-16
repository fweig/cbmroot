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
// --------------------------------------------------------------------------
void mcbm_reco_nh(Int_t nEvents         = 1000,
                  TString cSys          = "lam",
                  TString cEbeam        = "2.5gev",
                  TString cCentr        = "-",
                  Int_t iRun            = 0,
                  const char* setupName = "sis18_mcbm") {
  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_nh";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString outDir = "data/";
  TString inFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr
                   + ".mc." + Form("%05d", iRun)
                   + ".root";  // Input file (MC events)
  TString parFile = outDir + setupName + "_" + cSys + "." + cEbeam + "."
                    + cCentr + ".params." + Form("%05d", iRun)
                    + ".root";  // Parameter file
  TString outFile = outDir + setupName + "_" + cSys + "." + cEbeam + "."
                    + cCentr + ".eds." + Form("%05d", iRun)
                    + ".root";  // Output file

  // -----   Remove old CTest runtime dependency file  ----------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_reco", setupName);

  FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG1");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (setup->GetGeoTag(kTrd, geoTag)) {
    TObjString* trdFile =
      new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
    parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file "
              << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if (setup->GetGeoTag(kTof, geoTag)) {
    TObjString* tofBdfFile =
      new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
              << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------

  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // -----   Digitisers   ---------------------------------------------------
  std::cout << std::endl;
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/digitize.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("digitize()");
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------
  std::cout << std::endl;
  macroName = srcDir + "/macro/mcbm/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct()");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName
              << std::endl;
    return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " excuted successfully"
            << std::endl;
  // ------------------------------------------------------------------------
  // =========================================================================
  // ===                    Matching to Monte-carlo                        ===
  // =========================================================================
  /*
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // Digitizer/custerizer testing
  CbmTofHitFinderQa* tofQa = new CbmTofHitFinderQa("TOF QA");
  tofQa->SetHistoFileName("TofQA.root");
  run->AddTask(tofQa);
  */
  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------

  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

  // -----   Resource monitoring   ------------------------------------------
  if (Has_Fair_Monitor()) {  // FairRoot Version >= 15.11
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory = sysInfo.GetMaxMemory();
    std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    std::cout << maxMemory;
    std::cout << "</DartMeasurement>" << std::endl;

    Float_t cpuUsage = ctime / rtime;
    std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    std::cout << cpuUsage;
    std::cout << "</DartMeasurement>" << std::endl;

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
  RemoveGeoManager();
}
