//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 1.0
// @since   15.08.14
// 
// macro to simulate signal events for KFParticleFinder
//_________________________________________________________________________________


void simSignal(int iParticle = 0, Int_t nEvents = 1000,
               const char* setupName = "sis100_electron",
               const char* inputFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_mc";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString inFile = "Signal.txt";

  TString outDir  = "";
  TString outFile = "mc.root";
  TString parFile = "params.root";
  TString geoFile = "geofile.root";
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
//   Bool_t smearVertexXY = kTRUE;
//   Bool_t smearVertexZ  = kTRUE;
//   Double_t beamWidthX   = 1.;  // Gaussian sigma of the beam profile in x [cm]
//   Double_t beamWidthY   = 1.;  // Gaussian sigma of the beam profile in y [cm]
  Bool_t smearVertexXY = 0;
  Bool_t smearVertexZ  = 0;
  Double_t beamWidthX   = 0.;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY   = 0.;  // Gaussian sigma of the beam profile in y [cm]
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


  // -----   Create PrimaryGenerator   --------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Registering event generators" << std::endl;
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
//  primGen->SmearGausVertexXY(smearVertexXY);
//  primGen->SmearVertexZ(smearVertexZ);

  FairAsciiGenerator*  asciiGen = new FairAsciiGenerator(inFile);
  primGen->AddGenerator(asciiGen);

  run->SetGenerator(primGen);
  
  KFPartEfficiencies eff;
  for(int jParticle=eff.fFirstStableParticleIndex+10; jParticle<=eff.fLastStableParticleIndex; jParticle++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if(!pdgDB->GetParticle(eff.partPDG[jParticle])){
        pdgDB->AddParticle(eff.partTitle[jParticle].data(),eff.partTitle[jParticle].data(), eff.partMass[jParticle], kTRUE,
                           0, eff.partCharge[jParticle]*3,"Ion",eff.partPDG[jParticle]);
    }
  }
    
  Double_t lifetime = eff.partLifeTime[iParticle]; // lifetime
  Double_t mass = eff.partMass[iParticle];
  Int_t PDG = eff.partPDG[iParticle];
  Double_t charge = eff.partCharge[iParticle];
  
  if(iParticle == 56 || iParticle == 57)
  {
    for(int iPall=56; iPall<58; iPall++)
    {
      Double_t lifetime = eff.partLifeTime[iPall]; // lifetime
      Double_t mass = eff.partMass[iPall];
      Int_t PDG = eff.partPDG[iPall];
      Double_t charge = eff.partCharge[iPall];
    
      FairParticle* newParticle = new FairParticle(PDG, eff.partTitle[iPall].data(), kPTHadron, mass, charge,
              lifetime, "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);
      run->AddNewParticle(newParticle);
    }
    TString pythia6Config = "/u/mzyzak/cbmtrunk/macro/KF/KFParticleFinderSignalTest/Signal/DecayConfig.C()";
    run->SetPythiaDecayer(pythia6Config);
  }
   
   // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

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
  
  if(!(iParticle == 56 || iParticle == 57))
  {
    TVirtualMC::GetMC()->DefineParticle(PDG, eff.partTitle[iParticle].data(), kPTHadron, mass, charge,
                                        lifetime, "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);
    
    Int_t mode[6][3];
    Float_t bratio[6];

    for (Int_t kz = 0; kz < 6; kz++) {
      bratio[kz] = 0.;
      mode[kz][0] = 0;
      mode[kz][1] = 0;
      mode[kz][2] = 0;
    }
    bratio[0] = 100.;
    for(int iD=0; iD<eff.GetNDaughters(iParticle); iD++)
    {
      if(iD>2)
        continue;
      mode[0][iD] = eff.GetDaughterPDG(iParticle, iD); //pi+
    }
    
    TVirtualMC::GetMC()->SetDecayMode(PDG,bratio,mode);
  }
  for(int iP=eff.fFirstHypernucleusIndex; iP<=eff.fLastHypernucleusIndex; iP++)
  {
    Double_t lifetime = eff.partLifeTime[iP]; // lifetime
    Double_t mass = eff.partMass[iP];
    Int_t PDG = eff.partPDG[iP];
    Double_t charge = eff.partCharge[iP];
   
    TVirtualMC::GetMC()->DefineParticle(PDG, eff.partTitle[iP].data(), kPTHadron, mass, charge,
                                        lifetime, "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);
    
    Int_t mode[6][3];
    Float_t bratio[6];

    for (Int_t kz = 0; kz < 6; kz++) {
      bratio[kz] = 0.;
      mode[kz][0] = 0;
      mode[kz][1] = 0;
      mode[kz][2] = 0;
    }
    bratio[0] = 100.;
    for(int iD=0; iD<eff.GetNDaughters(iP); iD++)
    {
      if(iD>2)
        continue;
      mode[0][iD] = eff.GetDaughterPDG(iP, iD); //pi+
    }
    
    TVirtualMC::GetMC()->SetDecayMode(PDG,bratio,mode);
  }

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
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // ------------------------------------------------------------------------

}

