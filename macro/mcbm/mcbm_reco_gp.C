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


void mcbm_reco_gp(Int_t nEvents = 100, 
               TString cSys="nini", 
               TString cEbeam="1.93gev",
               TString cCentr="mbias",
               Int_t   iRun=1,
//             const char* setupName = "sis18_mcbm_25deg")
//             const char* setupName = "sis18_mcbm_20deg_short")
               const char* setupName = "sis18_mcbm_20deg_long")
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
//  TString outDir  = "data/";

	cout << "directories set!" << endl;
  TString outDir = "/home/aghoehne/Documents/CbmRoot/Gregor/";	

  TString inFile  = outDir + setupName + "_test.mc.root";       // Input file (MC events)
  TString parFile = outDir + setupName + "_params.root";        // Parameter file
  TString outFile = outDir + setupName + "_test.eds.root";      // Output file

  TString geoFile = outDir + setupName + "_geofile_full.root";		

  TString resultDir ="/home/aghoehne/Documents/CbmRoot/Gregor/results_mcbm_rich";

//  TString inFile  = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr + ".mc." + Form("%05d",iRun) + ".root"; // Input file (MC events)
//  TString parFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr + ".params." + Form("%05d",iRun) + ".root";  // Parameter file
//  TString outFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr + ".eds." + Form("%05d",iRun) + ".root";     // Output file
  // ------------------------------------------------------------------------


  // -----   Remove old CTest runtime dependency file  ----------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_reco" , setupName);
  // ------------------------------------------------------------------------


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
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


      // -----   Reconstruction run   -------------------------------------------
    FairRunAna *run= new FairRunAna();
    if (inFile != "") run->SetInputFile(inFile);
    if (outFile != "") run->SetOutputFile(outFile);
    run->SetGenerateRunInfo(kTRUE);
    run->SetGeomFile(geoFile);
    Bool_t hasFairMonitor = Has_Fair_Monitor();
    if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  
  // ------------------------------------------------------------------------


    // ----- MC Data Manager   ------------------------------------------------
    CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager",1);
    mcManager->AddFile(inFile);
    run->AddTask(mcManager);
    // ------------------------------------------------------------------------

    // =========================================================================
    // ===                        RICH reconstruction                        ===
    // =========================================================================

    CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
    richDigitizer->SetNofNoiseHits(0);
    run->AddTask(richDigitizer);
    cout << "-I- digitize: Added task " << richDigitizer->GetName() << endl;


    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    richHitProd->SetRotationNeeded(false);
    run->AddTask(richHitProd);
    cout << "-I- hitProducer: Added task " << richHitProd->GetName() << endl;


    	

	CbmRichReconstruction* richReco = new CbmRichReconstruction();
	richReco->SetRunExtrapolation(false);
	richReco->SetRunProjection(false);
	richReco->SetRunTrackAssign(false);
	richReco->SetFinderName("ideal");
    	richReco->SetFitterName("circle_cop");
	run->AddTask(richReco);
    	cout << "-I- richReco: Added task " << richReco->GetName() << endl;

	CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
    	run->AddTask(matchRecoToMc);

	
	cout << "next: class calling" << endl;

	CbmRichMCbmQa* mRichQa = new CbmRichMCbmQa();
    	mRichQa->SetOutputDir(string(resultDir));
    	run->AddTask(mRichQa);

	cout << "class called!" << endl;
	

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


/* // -----   FairRunAna   ---------------------------------------------------
  FairRunAna *run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
*/
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------
/*
  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------
*/
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
  if ( ! recoSuccess ) {
  	std::cerr << "-E-" << myName << ": error in executing " << macroName
  			<< std::endl;
  	return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " excuted successfully"
  		<< std::endl;
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

  // -----   Resource monitoring   ------------------------------------------
  if ( Has_Fair_Monitor() ) {      // FairRoot Version >= 15.11
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory=sysInfo.GetMaxMemory();
    std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    std::cout << maxMemory;
    std::cout << "</DartMeasurement>" << std::endl;

    Float_t cpuUsage=ctime/rtime;
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
