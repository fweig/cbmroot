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


void run_reco_new(Int_t nEvents = 2,
		              const char* setupName = "sis100_electron",
		              const char* inputFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_new";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString outDir   = gSystem->Getenv("OUTDIR");
  if(outDir.IsNull())  outDir = "data/";
  TString inFile  = outDir + setupName + "_mc.root";       // Input file (MC events)
  TString outFile = outDir + setupName + "_reco.root";     // reco file
  TString parFile = outDir + setupName + "_params.root";   // Parameter file
  // ------------------------------------------------------------------------


  // -----   Remove old CTest runtime dependency file  ----------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_reco" , setupName);
  // ------------------------------------------------------------------------


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = outDir + "/../setup_" + setupName + ".C";
  //  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // ------------------------------------------------------------------------


  // -----   Modify Cbm setup   ---------------------------------------------
  setup->SetActive(kPsd, kFALSE); // dont write psd points
  // --- remove detector geometries
  setup->RemoveModule(kPsd);      // remove psd from setup
  // setup->RemoveModule(kMvd);   // remove mvd and its material budget
  // setup->RemoveModule(kTrd);   // e.g. for sts-tof-matching study
  // --- change default geomerties
  //  setup->SetModule(kTrd, "v15d_1e", kTRUE); // 5 TRD layer
  std::cout << "-I- " << myName << ": CbmSetup updated " << std::endl;
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList *parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if ( setup->GetGeoTag(kTrd, geoTag) ) {
  	TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
  	parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if ( setup->GetGeoTag(kTof, geoTag) ) {
  	TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
  	parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << tofFile->GetString() << std::endl;
  	TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
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
  TString defaultInputFile = "data/";
  defaultInputFile = defaultInputFile + setupName + "_test.mc.root";
  if ( inFile.IsNull() ) {  // Not defined in the macro explicitly
  	if ( strcmp(inputFile, "") == 0 ) {  // not given as argument to the macro
  		inFile = defaultInputFile;
  	}
  	else inFile = inputFile;
  }
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna *run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger* gLogger = FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
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
  macroName = srcDir + "/macro/run/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("reconstruct()");
  // ------------------------------------------------------------------------


  // -----   Matching to Monte-Carlo tasks   --------------------------------
  std::cout << std::endl;
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  std::cout << "-I- : Added task " << mcManager->GetName() << std::endl;
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  std::cout << "-I- : Added task " << matchTask->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   PID tasks   ----------------------------------------------------
  if ( setup->IsActive(kTrd) ) {

    // ----------- TRD track Pid Wkn ----------------------
    CbmTrdSetTracksPidWkn* trdSetTracksPidTask = new CbmTrdSetTracksPidWkn("trdFindTracks", "trdFindTracks");
    run->AddTask(trdSetTracksPidTask);
    std::cout << "-I- : Added task " << trdSetTracksPidTask->GetName() << std::endl;
    // ----------------------------------------------------

    // ----------- TRD track Pid Ann ----------------------
    CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("Ann", "Ann");
    run->AddTask(trdSetTracksPidAnnTask);
    std::cout << "-I- : Added task " << trdSetTracksPidAnnTask->GetName() << std::endl;
    // ----------------------------------------------------

    // ----------- TRD track Pid Like ----------------------
    // Since in the newest version of this method depends on the global
    // track the task has to move after the global tracking
    CbmTrdSetTracksPidLike* trdSetTracksPidLikeTask = new CbmTrdSetTracksPidLike("Likelihood", "Likelihood");
    run->AddTask(trdSetTracksPidLikeTask);
    std::cout << "-I- : Added task " << trdSetTracksPidLikeTask->GetName() << std::endl;
    // ----------------------------------------------------

  }
  // ------------------------------------------------------------------------


  // -----   control data persistency   -------------------------------------
  std::cout << std::endl;
  if ( setup->IsActive(kMvd) ) {
    run->GetTask("MVD Digitiser")->SetOutputBranchPersistent("MvdDigi",      kFALSE);
    run->GetTask("MVD Digitiser")->SetOutputBranchPersistent("MvdDigiMatch", kFALSE);
  }
  if ( setup->IsActive(kSts) ) {
    run->GetTask("StsDigitize")->SetOutputBranchPersistent("StsDigi",      kFALSE);
    run->GetTask("StsDigitize")->SetOutputBranchPersistent("StsDigiMatch", kFALSE);
  }
  if ( setup->IsActive(kRich) ) {
    run->GetTask("CbmRichDigitizer")->SetOutputBranchPersistent("RichDigi",      kFALSE);
  }
  if ( setup->IsActive(kMuch) ) {
    run->GetTask("MuchDigitizeGem")      ->SetOutputBranchPersistent("MuchDigi",      kFALSE); //gem
    run->GetTask("MuchDigitizeGem")      ->SetOutputBranchPersistent("MuchDigiMatch", kFALSE);
    run->GetTask("CbmMuchDigitizeStraws")->SetOutputBranchPersistent("MuchStrawDigi",      kFALSE); //straw
    run->GetTask("CbmMuchDigitizeStraws")->SetOutputBranchPersistent("MuchStrawDigiMatch", kFALSE);
  }
  if ( setup->IsActive(kTrd) ) {
    run->GetTask("CbmTrdDigitizerPRF")   ->SetOutputBranchPersistent("TrdDigi",      kFALSE);
    run->GetTask("CbmTrdDigitizerPRF")   ->SetOutputBranchPersistent("TrdDigiMatch", kFALSE);
  }
  if ( setup->IsActive(kTof) ) {
    run->GetTask("TOF Digitizer BDF")      ->SetOutputBranchPersistent("TofDigi",            kFALSE);
    run->GetTask("TOF Digitizer BDF")      ->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
    run->GetTask("TOF Simple Clusterizer") ->SetOutputBranchPersistent("TofDigiMatch",       kFALSE);
    matchTask->SetOutputBranchPersistent("TofDigiMatch",     kFALSE);
  }
  std::cout << "-I- : Data persistency controlled " << std::endl;
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
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

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
}
