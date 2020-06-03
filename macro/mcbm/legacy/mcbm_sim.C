// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// Standard CBM setup with MVD, STS, RICH, TRD, TOF and ECAL
//
// V. Friese   22/02/2007
//
// 2017-03-30 - DE - add mcbm_sim.C to CTests
// 2014-06-30 - DE - available setups from geometry/setup:
// 2014-06-30 - DE - sis100_hadron
// 2014-06-30 - DE - sis100_electron
// 2014-06-30 - DE - sis100_muon
// 2014-06-30 - DE - sis300_electron
// 2014-06-30 - DE - sis300_muon
//
// --------------------------------------------------------------------------


TString caveGeom="";
TString pipeGeom="";
TString magnetGeom="";
TString mvdGeom="";
TString stsGeom="";
TString richGeom="";
TString muchGeom="";
TString shieldGeom="";
TString trdGeom="";
TString tofGeom="";
TString ecalGeom="";
TString platformGeom="";
TString psdGeom="";
Double_t psdZpos=0.;
Double_t psdXpos=0.;

TString mvdTag="";
TString stsTag="";
TString trdTag="";
TString tofTag="";  

TString stsDigi="";
TString muchDigi="";
TString trdDigi="";
TString tofDigi="";
TString tofDigiBdf="";

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";

//void mcbm_sim(Int_t nEvents = 20, const char* setup = "sis18_mcbm")
void mcbm_sim(Int_t nEvents = 2, const char* setup = "sis18_mcbm")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");


  TString outDir  = "data/";
  TString outFile = outDir + setup + "_test.mc.root";
  TString parFile = outDir + setup + "_params.root";
  TString geoFile = outDir + setup + "_geofile_full.root";

  TString setupFile = inDir + "/geometry/setup/legacy/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";
  
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  TString inFile  = inDir + defaultInputFile;

  // Function needed for CTest runtime dependency
  TString depFile = Remove_CTest_Dependency_File(outDir, "mcbm_sim" , setup);

//  Bool_t hasFairMonitor = Has_Fair_Monitor();

  // --- Logger settings ----------------------------------------------------
  TString logLevel = "INFO";   // "DEBUG";
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
  Double_t targetThickness = 0.1;    // full thickness in cm
  Double_t targetDiameter  = 0.5;    // diameter in cm
  Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]
  Double_t beamRotY        = -20.;   // beam rotation angle around the y axis [deg]
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
  Double_t beamWidthX  = 0.1;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY  = 0.1;  // Gaussian sigma of the beam profile in y [cm]
  // ------------------------------------------------------------------------
  

  // In general, the following parts need not be touched
  // ========================================================================

  cout << "[INFO   ] Setup: " << setup << endl;

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create detectors and passive volumes   -------------------------
  if ( caveGeom != "" ) {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    run->AddModule(cave);
  }

  if ( pipeGeom != "" ) {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    run->AddModule(pipe);
  }
  
  // --- Target
  CbmTarget* target = new CbmTarget(targetElement.Data(),
  		                              targetThickness,
  		                              targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  //  target->SetRotation(targetRotY);
  run->AddModule(target);

  if ( magnetGeom != "" ) {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    run->AddModule(magnet);
  }
  
  if ( platformGeom != "" ) {
    FairModule* platform = new CbmPlatform("PLATFORM");
    platform->SetGeometryFileName(platformGeom);
    run->AddModule(platform);
  }

  if ( mvdGeom != "" ) {
    FairDetector* mvd = new CbmMvd("MVD", kTRUE);
    mvd->SetGeometryFileName(mvdGeom);
    mvd->SetMotherVolume("pipevac1");
    run->AddModule(mvd);
  }

  if ( stsGeom != "" ) {
    FairDetector* sts = new CbmStsMC(kTRUE);
    sts->SetGeometryFileName(stsGeom);
    run->AddModule(sts);
  }

  if ( richGeom != "" ) {
    FairDetector* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    run->AddModule(rich);
  }
  
  if ( muchGeom != "" ) {
    FairDetector* much = new CbmMuch("MUCH", kTRUE);
    much->SetGeometryFileName(muchGeom);
    run->AddModule(much);
  }
  
  if ( shieldGeom != "" ) {
  	FairModule* shield = new CbmShield("SHIELD");
  	shield->SetGeometryFileName(shieldGeom);
  	run->AddModule(shield);
  }

  if ( trdGeom != "" ) {
    FairDetector* trd = new CbmTrd("TRD",kTRUE );
    trd->SetGeometryFileName(trdGeom);
    run->AddModule(trd);
  }

  if ( tofGeom != "" ) {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    run->AddModule(tof);
  }
  
  if ( ecalGeom != "" ) {
    FairDetector* ecal = new CbmEcal("ECAL", kTRUE, ecalGeom.Data()); 
    run->AddModule(ecal);
  }
  
  if ( psdGeom != "" ) {
  	cout << "Constructing PSD" << endl;
  	CbmPsdv1* psd= new CbmPsdv1("PSD", kTRUE);  
 	psd->SetZposition(psdZpos); // in cm
 	psd->SetXshift(psdXpos);  // in cm    
 	psd->SetGeoFile(psdGeom);  
	run->AddModule(psd);
  }
  
  // ------------------------------------------------------------------------

  // -----   Create magnetic field   ----------------------------------------
//  CbmFieldMap* magField = NULL;
//  if ( 2 == fieldSymType ) {
//    magField = new CbmFieldMapSym2(fieldMap);
//  }  else if ( 3 == fieldSymType ) {
//    magField = new CbmFieldMapSym3(fieldMap);
//  } 
//  magField->SetPosition(0., 0., fieldZ);
//  magField->SetScale(fieldScale);
  FairConstField* magField = new FairConstField();
  run->SetField(magField);
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
  //  primGen->SetTargetRotAngle(0., beamRotY * TMath::Pi()/180.);
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
  uniGen->SetEventPlane(0. , 360.);
  primGen->AddGenerator(uniGen);
  primGen->SetBeamAngle(beamRotY * TMath::Pi()/180.,0,0,0);  // set direction of beam to 30 degrees
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //  // -----   Create Electron gun as alternative -----------------------------                  
  //  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();                                  
  //  // Use the FairBoxGenerator which generates a soingle electron                               
  //  FairBoxGenerator *eminus = new FairBoxGenerator();                                           
  //  eminus->SetPDGType(11);                                                                      
  //  eminus->SetMultiplicity(1000);                                                               
  //  //  eminus->SetBoxXYZ(32.,-32.,32.,-32.,0.);  // shoot at corner of diagonal modules         
  //  //  eminus->SetBoxXYZ(0., 0., 0., 0., 0.);  // shoot at corner of diagonal modules           
  //  //  eminus->SetBoxXYZ(57.,-57., 0., 0.,0.);  // shoot at corner of diagonal modules          
  //  //  eminus->SetBoxXYZ(-57.,-57., 57., 57.,0.);  // shoot at corner of diagonal modules       
  //  eminus->SetBoxXYZ(-180.,-15.,-150.,15.,0.);  // shoot at corner of diagonal modules          
  //  eminus->SetPRange(2.,2.);                                                                    
  //  eminus->SetPhiRange(0.,360.);                                                                
  //  eminus->SetThetaRange(0.,0.);                                                                
  //  primGen->AddGenerator(eminus);                                                               
  //                                                                                               
  //  //  primGen->SetBeamAngle(30*TMath::Pi()/180.,0,0,0);  // set direction of beam to 30 degrees
  //                                                                                               
  //  fRun->SetGenerator(primGen);                                                                 
  //  // ------------------------------------------------------------------------                  


   // Visualisation of trajectories (TGeoManager Only)
  // Switch this on if you want to visualise tracks in the event display.
  // This is normally switch off, because of the huge files created
  // when it is switched on. 
  run->SetStoreTraj(kTRUE);

  // -----   Run initialisation   -------------------------------------------
  run->Init();
  // ------------------------------------------------------------------------
  
//  // Set cuts for storing the trajectories.
//  // Switch this on only if trajectories are stored.
//  // Choose this cuts according to your needs, but be aware
//  // that the file size of the output file depends on these cuts
//
//   FairTrajFilter* trajFilter = FairTrajFilter::Instance();
//   if ( trajFilter ) {
//  	 trajFilter->SetStepSizeCut(0.01); // 1 cm
//  	 trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
//  	 trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
//  	 trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
//  	 trajFilter->SetStorePrimaries(kTRUE);
//  	 trajFilter->SetStoreSecondaries(kTRUE);
//   }

  // -----   Runtime database   ---------------------------------------------
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
  run->Run(nEvents);
  // ------------------------------------------------------------------------
  run->CreateGeometryFile(geoFile);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
       << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------

//  if (hasFairMonitor) {
//    // Extract the maximal used memory an add is as Dart measurement
//    // This line is filtered by CTest and the value send to CDash
//    FairSystemInfo sysInfo;
//    Float_t maxMemory=sysInfo.GetMaxMemory();
//    cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
//    cout << maxMemory;
//    cout << "</DartMeasurement>" << endl;
//
//    Float_t cpuUsage=ctime/rtime;
//    cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
//    cout << cpuUsage;
//    cout << "</DartMeasurement>" << endl;
//  }

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
  RemoveGeoManager();
}

