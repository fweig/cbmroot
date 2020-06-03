// -------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// CBM setup with MVD only
//
// V. Friese   06/02/2007
// updated P.Sitzmann 22/11/2016

// --------------------------------------------------------------------------
void mvd_sim(Int_t nEvents = 100, TString setup = "sis100_electron")
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // Input file
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile  = "";


  // Output file name
  TString outDir="data/";
  TString outFile = outDir+ "mvd.mc.root";

  // Parameter file name
  TString parFile = outDir+ "params.root";

  TString geoFile = outDir + "geo.root";

  TString setupFile = inDir + "/geometry/setup/setup_"+ setup +".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setup + "()";
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);
  CbmSetup* cbmsetup = CbmSetup::Instance();
  cbmsetup->RemoveModule(kSTS);
  cbmsetup->RemoveModule(kRICH);
  cbmsetup->RemoveModule(kTRD);
  cbmsetup->RemoveModule(kTOF);
  cbmsetup->RemoveModule(kPSD);

  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

    // --- Logger settings ----------------------------------------------------
  TString logLevel = "INFO";   // "DEBUG";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
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
  



  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");              // Transport engine
  fRun->SetOutputFile(outFile);          // Output file
  fRun->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Logger settings   ----------------------------------------------
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  // -----   Create and register modules   ----------------------------------
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/registerSetup.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");
  // ------------------------------------------------------------------------


  // -----   Create and register the target   -------------------------------
  CbmTarget* target = new CbmTarget(targetElement.Data(),
                                              targetThickness,
                                              targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  target->Print();
  fRun->AddModule(target);
  // ------------------------------------------------------------------------

  // -----   Create magnetic field   ----------------------------------------
  CbmFieldMap* magField = CbmSetup::Instance()->CreateFieldMap();
  if ( ! magField ) {
        std::cout << "-E- run_sim_new: No valid field!";
        return;
  }
  fRun->SetField(magField);
  // ------------------------------------------------------------------------

  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  TString defaultInputFile = inDir + "/input/urqmd.auau.10gev.centr.root";
  if ( inFile.IsNull() ) {  // Not defined in the macro explicitly
//        if ( strcmp(inFile, "") == 0 ) {  // not given as argument to the macro
                inFile = defaultInputFile;
//        }
//        else inFile = inputFile;
  }
  // ------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // --- Uniform distribution of event plane angle
  primGen->SetEventPlane(0., 2. * TMath::Pi());
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
  //
  // TODO: Currently, there is no guaranteed consistency of the beam profile
  // and the transversal target dimension, i.e., that the sampled primary
  // vertex falls into the target volume. This would require changes
  // in the FairPrimaryGenerator class.
  // ------------------------------------------------------------------------

  // Use the CbmUnigenGenrator for the input
  CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
  primGen->AddGenerator(uniGen);
  fRun->SetGenerator(primGen);
  // ------------------------------------------------------------------------

    // Visualisation of trajectories (TGeoManager Only)
  // Switch this on if you want to visualise tracks in the event display.
  // This is normally switch off, because of the huge files created
  // when it is switched on. 
  fRun->SetStoreTraj(kFALSE);

 
  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------

  
  
  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


 
  // -----   Start run   ----------------------------------------------------
  fRun->Run(nEvents);
  // ------------------------------------------------------------------------
  fRun->CreateGeometryFile(geoFile);
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

}

