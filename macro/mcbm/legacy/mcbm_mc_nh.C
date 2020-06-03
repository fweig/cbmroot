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

void mcbm_mc_nh(Int_t nEvents = 2,  Int_t iMode=3, 
	     TString cSys="lam", 
	     TString cEbeam="2.5gev",
	     TString cCentr="-",
	     Int_t iRun=0, 
	     const char* setupName = "sis18_mcbm_20deg_long",
             const char* inputFile = "")

// available input files
// inputFile = "/input/urqmd.agag.1.65gev.centr.00001.root";
// inputFile = "/input/urqmd.agag.1.65gev.mbias.00001.root";
// inputFile = "/input/urqmd.niau.1.93gev.centr.00001.root";
// inputFile = "/input/urqmd.niau.1.93gev.mbias.00001.root";
// inputFile = "/input/urqmd.nini.1.93gev.centr.00001.root";
// inputFile = "/input/urqmd.nini.1.93gev.mbias.00001.root";
// inputFile = "/input/urqmd.pau.4.5gev.mbias.00001.root";

{

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_mc";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------
  // -----   In- and output file names   ------------------------------------
  //TString inFile  = srcDir + "/input/urqmd." + cSys + "." + cEbeam + "." + cCentr + "." + Form("%05d",iRun) + ".root";
  TString inFile  = "/lustre/nyx/cbm/prod/gen/urqmd/" + cSys + "/" + cEbeam + "/" + cCentr + "/urqmd." + cSys + "." + cEbeam + "." + cCentr + "." + Form("%05d",iRun) + ".root";
  TString outDir  = "data/";
  TString outFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr + ".mc." + Form("%05d",iRun) + ".root";
  TString parFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr + ".params." + Form("%05d",iRun) + ".root";
  TString geoFile = outDir + setupName + "_geofile_full.root";
  // ------------------------------------------------------------------------

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "WARNING";
  //TString logLevel     = "INFO";
  TString logVerbosity = "MEDIUM";
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
  Double_t beamRotY        = 25.;    // with 8 degree wrt R3B beam - beam rotation angle around the y axis [deg]
  //  Double_t beamRotY        = -20.;   // with 15 degree magnet - beam rotation angle around the y axis [deg]
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

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Remove old CTest runtime dependency file   ---------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "mcbm_mc" , setupName);
  // ------------------------------------------------------------------------


  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
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
  //TString defaultInputFile = srcDir + "/input/urqmd.agag.1.65gev.centr.00001.root";
  TString defaultInputFile = srcDir + "/input/urqmd.nini.1.93gev.mbias.00001.root";
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
  macroName += "/macro/mcbm/registerSetup.C";
  //  macroName += "/macro/run/modules/registerSetup.C";
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

     // choose generator / source 
  if(iMode>0)
  {
    switch (iMode) {
        case 1:                  
	  {                                 //                          (pdg, mul,px,     py, pz, vx,vy,vz)
            FairParticleGenerator *fPartGen = new FairParticleGenerator  (13,   1,0.0,-0.228, 1.5, 0.,0.,0.); //mu-
	    primGen->AddGenerator(fPartGen);
	  }
            break;

        case 2:                  
	  {                                 //(pdg,mul,px, py, pz, vx,vy,vz)
            FairParticleGenerator *fPartGen= new FairParticleGenerator(2212, 1,0.0,-0.228, 1.5, 0.,0.,0.); //proton
	    primGen->AddGenerator(fPartGen);
	  }
            break;

        case 3:                  
	  {                                 //(pdg,mul,px, py, pz, vx,vy,vz)
	    Double_t pz;
	    sscanf(cEbeam,"%lfgev",&pz);
	    cout<<"simulate single lambda with pz = "<<pz<<endl;
            FairParticleGenerator *fPartGen= new FairParticleGenerator(3122, 1,0.0,0., pz, 0.,0.,0.); //lambda
	    primGen->AddGenerator(fPartGen);
	  }
            break;
        case 4:
	  {
	    FairBoxGenerator *fPartGen= new FairBoxGenerator(3122, 1);
	    fPartGen->SetPtRange(0.,0.);
            fPartGen->SetYRange(0.9,2.);
	    primGen->AddGenerator(fPartGen);
	  }	  
            break;

        default:
	  ;
    }
  }
  // Use the CbmUrqmdGenrator which calculates a reaction plane and
  // rotate all particles accordingly
  else { //if (iMode>0){
  // Use the CbmUnigenGenrator for the input
  CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
  uniGen->SetEventPlane(0. , 360.);
  primGen->AddGenerator(uniGen);
  primGen->SetBeamAngle(beamRotY * TMath::Pi()/180.,0,0,0);  // set direction of beam
  }
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
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
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
  RemoveGeoManager();
  // ------------------------------------------------------------------------
}

