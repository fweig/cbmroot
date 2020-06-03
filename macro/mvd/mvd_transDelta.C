 // --------------------------------------------------------------
//
// Macro for standard transport delta simulation using GEANT3
// CBM setup with MVD only
//
// updated P.Sitzmann  22/11/2016
//
// --------------------------------------------------------------------------

void mvd_transDelta(Int_t nEvents = 5000, TString setup = "sis100_electron")
{
  // ========================================================================
  //          Adjust this part according to your requirements

  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");

  TString inDir   = gSystem->Getenv("VMCWORKDIR");

  // Output path
  TString outDir="./data/";   
  TString outpath ="./data/";

  //output file
  TString outfile = "mvd.mcDelta.root";

  //output filename
  TString outFile = outpath + outfile;

  // Parameter file name
  TString parFile = outpath + "params.root";

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

  Int_t iVerbose = 0;


  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
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

  FairIon *fIon = new FairIon("My_Au", 79, 197, 79, 10.,183.47324);      // 10 GeV Gold
  fRun->AddNewIon(fIon);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen   = new FairPrimaryGenerator();
  FairIonGenerator*     fIongen   = new FairIonGenerator(79, 197,79,1, 0.,0., 10, 0.,0.,-1.);     // 10 GeV Gold
  primGen->AddGenerator(fIongen);

  fRun->SetGenerator(primGen);


  // ------------------------------------------------------------------------


 
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

