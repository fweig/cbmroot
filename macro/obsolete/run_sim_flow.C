// --------------------------------------------------------------------------
//
// Macro for transport simulation for the sake of collective flow studies, using UrQMD or SHIELD input and GEANT4
// CBM setup with (MVD,) STS, (RICH, TRD, TOF and ECAL), PSD (detector in brackets not included by default)
//
// 3 output files, with: 1) transported events 2) run parameters 3) PSD module XY coordinates in LAB (target) system (ASCII)
//
// Selim, 22/10/14
//
// --------------------------------------------------------------------------

// TO CHECK in the code: IMPORTANT NOTE

void run_sim_flow(Int_t nEvents = 2, Int_t En=25, const char* setupName = "sis300_electron_flow", const char* inputFile ="")
{
  // The energy is needed in the setup file to define the field scale
  // and the psd position
  TString energy = Form("%i", En);
  gSystem->Setenv("BEAM_ENERGY",energy);
  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_sim";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString inFile = ""; // give here or as argument; otherwise default is taken
  TString outDir  = "data/";
  TString outFile = outDir + setupName + "_mc_evt.root";
  TString parFile = outDir + setupName + "_params_evt.root";
  TString geoFile = outDir + setupName + "_geofile_full_evt.root";
  // ------------------------------------------------------------------------


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";  
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // --- Define the target geometry -----------------------------------------
  //
  // The target is not part of the setup, since one and the same setup can
  // and will be used with different targets.
  // The target is constructed as a tube in z direction with the specified
  // diameter (in x and y) and thickness (in z). It will be placed at the
  // specified position as daughter volume of the volume present there. It is
  // in the responsibility of the user that no overlaps or extrusions are
  // created by the placement of the target.
  //
  TString  targetElement   = "Gold";
  Double_t targetThickness = 0.025;  // full thickness in cm
  Double_t targetDiameter  = 2.5;    // diameter in cm
  Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]
  // ------------------------------------------------------------------------


  // --- Define the creation of the primary vertex   ------------------------
  //
  // By default, the primary vertex point is sampled from a Gaussian
  // distribution in both x and y with the specified beam profile width,
  // and from a flat distribution in z over the extension of the target.
  // By setting the respective flags to kFALSE, the primary vertex will always
  // at the (0., 0.) in x and y and in the z centre of the target, respectively.
  //
  Bool_t smearVertexXY = kTRUE;
  Bool_t smearVertexZ  = kTRUE;
  Double_t beamWidthX   = 1.;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY   = 1.;  // Gaussian sigma of the beam profile in y [cm]
  // ------------------------------------------------------------------------


  // In general, the following parts need not be touched
  // ========================================================================


  // if kbeam==kTRUE, transport the beam for estimating required PSD x-shift
  // if kbeam==kFALSE, transport particles from input models, 
  // gen=0 : UrQMD
  bool kbeam = kFALSE;
  Int_t gen = 0;

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  
  // -----   Remove old CTest runtime dependency file   ---------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_sim" , setupName);
  // ------------------------------------------------------------------------



  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant4");              // Transport engine
                                         // IMPORTANT NOTE: need G4 for hadronic calorimetry in PSD including projectile fragments (produced in SHIELD)
                                         // IMPORTANT NOTE: change physics list (in gconfig/g4Config.C) to either FTFP_BERT or QGSP_BIC_HP (both tested)
  run->SetOutputFile(outFile);          // Output file
  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  TString defaultInputFile = srcDir + "/input/urqmd.auau.25gev.centr.root";
  if ( inFile.IsNull() ) {  // Not defined in the macro explicitly
  	if ( strcmp(inputFile, "") == 0 ) {  // not given as argument to the macro
  		inFile = defaultInputFile;
  	}
  	else inFile = inputFile;
  }
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Setting media file" << std::endl;
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create and register modules   ----------------------------------
  std::cout << std::endl;
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/registerSetup.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");
  // ------------------------------------------------------------------------


  // -----   Create and register the target   -------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering target" << std::endl;
  CbmTarget* target = new CbmTarget(targetElement.Data(),
  		                              targetThickness,
  		                              targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  target->Print();
  run->AddModule(target);
  // ------------------------------------------------------------------------


  // -----   Create magnetic field   ----------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering magnetic field" << std::endl;
  CbmFieldMap* magField = CbmSetup::Instance()->CreateFieldMap();
  if ( ! magField ) {
  	std::cout << "-E- run_sim_new: No valid field!";
  	return;
  }
  run->SetField(magField);
  // ------------------------------------------------------------------------


  /*
  CbmPsdv1_44mods_hole6cm* psd= new CbmPsdv1_44mods_hole6cm("PSD", kTRUE);

  // ========= Acceptance PSD & FD method
  //CbmPsdWoutv1_for44mods_full_holes* psd= new CbmPsdWoutv1_for44mods_full_holes("PSD", kTRUE);
  //CbmPsdWoutv1_for44mods_sub1* psd= new CbmPsdWoutv1_for44mods_sub1("PSD", kTRUE);

  psd->SetZposition(psdZpos); // in cm
  psd->SetXshift(psdXshift);  // in cm    
  TString geoFileNamePsd = outDir + "psd_geo_xy_" + numEvt + "evt.txt";
  psd->SetGeoFile(geoFileNamePsd);
  fRun->AddModule(psd);
  */
  // ------------------------------------------------------------------------


  // -----   Create PrimaryGenerator   --------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering event generators" << std::endl;
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // --- Uniform distribution of event plane angle
  primGen->SetEventPlane(-TMath::Pi(), TMath::Pi());
  // --- Get target parameters
  Double_t tX = 0.;
  Double_t tY = 0.;
  Double_t tZ = 0.;
  Double_t tDz = 0.;
  if ( target ) {
  	target->GetPosition(tX, tY, tZ);
  	tDz = target->GetThickness();
  }
  primGen->SetTarget(tZ, tDz);
  primGen->SetBeam(0., 0., beamWidthX, beamWidthY);
  primGen->SmearGausVertexXY(smearVertexXY);
  primGen->SmearVertexZ(smearVertexZ);
   // Include beam emittance
   //primGen->SmearVertexZ(kTRUE);
   //primGen->SmearVertexXY(kTRUE);
   //primGen->SetBeam(0., 0., 0.15, 0.06, 2.2e-3, 2e-3); // emittance (SIS100) @ 10 AGeV ~ 2.2 mm.mrad (X) -> deltaX = +/- 1 mm && thetaX = +/- 2.2 mrad

  //
  // TODO: Currently, there is no guaranteed consistency of the beam profile
  // and the transversal target dimension, i.e., that the sampled primary
  // vertex falls into the target volume. This would require changes
  // in the FairPrimaryGenerator class.
  // ------------------------------------------------------------------------

  if (kbeam == kFALSE) {
    
    CbmUnigenGenerator*  urqmdGen = new CbmUnigenGenerator(inFile);
    // IMPORTANT NOTE: event plane angle in [-pi, pi] by convention
    // rotation is done in FairPrimaryGenerator    
    primGen->AddGenerator(urqmdGen);
  } else {
    
    Double_t bMom;
    if (En == 35) bMom = 35.926;
    if (En == 25) bMom = 25.92;
    if (En == 15) bMom = 15.91;
    if (En == 10) bMom = 10.898; 
    if (En == 8) bMom = 8.88889;
    if (En == 6) bMom = 6.87454;
    if (En == 4) bMom = 4.84832;
    if (En == 2) bMom = 2.78444;
    
    int Nion;
    int pileup = 100;
    Nion = nEvents*pileup;
    
    FairIonGenerator *fIongen= new FairIonGenerator(79, 197, 79, Nion, 0., 0., bMom, 0., 0., -1.); 
    primGen->AddGenerator(fIongen);   
    
    nEvents = 1;
  }

  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

 
  // -Trajectories Visualization (TGeoManager Only )
  // Switch this on if you want to visualize tracks in the
  // eventdisplay.
  // This is normally switch off, because of the huge files created
  // when it is switched on. 
  // run->SetStoreTraj(kTRUE);

  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------
  
  // Set cuts for storing the trajectories.
  // Switch this on only if trajectories are stored.
  // Choose this cuts according to your needs, but be aware
  // that the file size of the output file depends on these cuts

  // FairTrajFilter* trajFilter = FairTrajFilter::Instance();
  // trajFilter->SetStepSizeCut(0.01); // 1 cm
  // trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
  // trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
  // trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
  // trajFilter->SetStorePrimaries(kTRUE);
  // trajFilter->SetStoreSecondaries(kTRUE);

  CbmStack* stack = (CbmStack*) FairMCApplication::Instance()->GetStack();
  //stack->SetMinPoints(0);
  //stack->StoreSecondaries(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

 
  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(nEvents);
  // ------------------------------------------------------------------------

  
  // -----   Finish   -------------------------------------------------------
  run->CreateGeometryFile(geoFile);
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is "  << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime 
	    << "s" << std::endl << std::endl;
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
  }

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
  // ------------------------------------------------------------------------

}

