// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// Standard CBM setup with MVD, STS, RICH, TRD, TOF and ECAL
//
// V. Friese   22/02/2007
//
// --------------------------------------------------------------------------

void trd_sim(Int_t nEvents = 1, 
             const char* setupName = "sis100_electron")
//             const char* setupName = "sis100_hadron")
//               const char* setupName = "sis100_debug")
{


  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile = ""; // give here or as argument; otherwise default is taken
  TString outDir  = "data";
  TString outFile = outDir + "/test.mc.root";
  TString parFile = outDir + "/params.root";


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

  
  // Function needed for CTest runtime dependency
  TString depFile = Remove_CTest_Dependency_File(outDir, "trd_sim");

  TString setupFile = inDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

//  if (CbmSetup <= 3)
//    CbmSetup::Instance()->SetModule(kPlatform, "v13a");
//  else
//    CbmSetup::Instance()->SetModule(kPlatform, "v13b");

  // In general, the following parts need not be touched
  // ========================================================================




  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------



  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");              // Transport engine
  fRun->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
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
 
  // -Trajectories Visualization (TGeoManager Only )
  // Switch this on if you want to visualize tracks in the
  // eventdisplay.
  // This is normally switch off, because of the huge files created
  // when it is switched on. 
  fRun->SetStoreTraj(kTRUE);

  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------
  
  // Set cuts for storing the trajectories.
  // Switch this on only if trajectories are stored.
  // Choose this cuts according to your needs, but be aware
  // that the file size of the output file depends on these cuts

   FairTrajFilter* trajFilter = FairTrajFilter::Instance();
   trajFilter->SetStepSizeCut(0.01); // 1 cm
   trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
   trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
   trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
   trajFilter->SetStorePrimaries(kTRUE);
   trajFilter->SetStoreSecondaries(kTRUE);

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
  fRun->CreateGeometryFile("data/geofile_full.root");


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

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
  RemoveGeoManager();
}

