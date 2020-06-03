void run_mc(int index = -1)
{
   Int_t nEvents = 1000;
   //const char* setupName = "sis100_electron";
   const char* setupName = "sis100_muon_jpsi";
   TString system  = "auau";
   TString beam    = "8gev";
   TString trigger = "mbias";//"centr";
   TString part = "omega";
   //TString channel = "epem";
   TString channel = "mpmm";

   bool useSig = false;
   bool useBg = true;
   bool sigAscii = false;
   Bool_t storeTraj = kFALSE;

   if (!useSig && !useBg)
   {
      cout << "At least one of the signal or background must be set!" << endl;
      return;
   }
   
   if (storeTraj)
      nEvents = 10;

   TString partDir = "charm";

   if (part != "jpsi")
     partDir = "cktA";

   char str[5];
   sprintf(str, "%05d", index >= 0 ? index : 1);

   char str2[4];
   sprintf(str2, "%04d", index >= 0 ? index : 1);

   TString suffix = "";

   if (useBg)
     suffix += "." + trigger;

   if (useSig)
   {
      if (sigAscii)
         suffix += ".ascii";
      else
         suffix += "." + part;
   }
   
   TString fsPrefix;
   
   if (index >= 0)
      fsPrefix = "/lustre/nyx";
   else
      fsPrefix = "/SAT/nyx";//"/SAT/hera";

   TString bgFile = fsPrefix + "/cbm/prod/gen/urqmd/" + system + "/" + beam + "/" + trigger + "/" + "urqmd." + system + "." + beam + "." + trigger + "." + TString(str) + ".root";
   TString outDir;
   
   if (index >= 0)
      outDir = "/lustre/nyx/cbm/users/tablyaz/Lx/runs/data" + TString(str) + "/";
   else
      outDir = "/data.local/cbmrootdata/";
   
   TString sigFile;
   
   if (sigAscii)
      sigFile = outDir + "muon_pairs.txt";
   else
      sigFile = fsPrefix + "/cbm/prod/gen/pluto/" + system + "/" + partDir + "/" + beam + "/" + part + "/" + channel + "/pluto." + system + "." + beam + "." + part + "." + channel + "." + TString(str2) + ".root";

   TString outFile = outDir + setupName + ".mc." + system + "." + beam + suffix + ".root";
   TString parFile = outDir + setupName + ".param." + system + "." + beam + suffix + ".root";
   TString geoFile = outDir + setupName + "_geofile.root";

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_sim";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
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
  Double_t beamWidthX   = 1.0;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY   = 1.0;  // Gaussian sigma of the beam profile in y [cm]
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
  //TString depFile = Remove_CTest_Dependency_File(outDir, "run_mc" , setupName);
  // ------------------------------------------------------------------------



  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  run->SetStoreTraj(storeTraj);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
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
  primGen->SmearGausVertexXY(smearVertexXY);
  primGen->SmearVertexZ(smearVertexZ);
  //
  // TODO: Currently, there is no guaranteed consistency of the beam profile
  // and the transversal target dimension, i.e., that the sampled primary
  // vertex falls into the target volume. This would require changes
  // in the FairPrimaryGenerator class.
  // ------------------------------------------------------------------------
  
  if (useSig)
  {
    if (sigAscii)
    {
      FairAsciiGenerator* signalGen = new FairAsciiGenerator(sigFile);
      primGen->AddGenerator(signalGen);
    }
    else
    {
      CbmPlutoGenerator* plutoGen= new CbmPlutoGenerator(sigFile);
      primGen->AddGenerator(plutoGen);
    }
  }

  if (useBg)
  {
    CbmUnigenGenerator* uniGen = new CbmUnigenGenerator(bgFile);
    primGen->AddGenerator(uniGen);
  }
  
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

 
  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------
  

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
  /*if ( Has_Fair_Monitor() ) {      // FairRoot Version >= 15.11
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
  }*/

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
  //Generate_CTest_Dependency_File(depFile);
  // ------------------------------------------------------------------------

}

