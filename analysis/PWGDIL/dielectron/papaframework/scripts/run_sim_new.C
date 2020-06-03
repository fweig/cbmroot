// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
//
// V. Friese   22/02/2007
//
// Version 2016-02-05
//
// For the setup (geometry and field), predefined setups can be chosen
// by the second argument. A list of available setups is given below.
// The input file can be defined explicitly in this macro or by the
// third argument. If none of these options are chosen, a default
// input file distributed with the source code is selected.
//
// 2014-06-30 - DE - available setups from geometry/setup:
// 2014-06-30 - DE - sis100_hadron
// 2014-06-30 - DE - sis100_electron
// 2014-06-30 - DE - sis100_muon
// 2014-06-30 - DE - sis300_electron
// 2014-06-30 - DE - sis300_muon
//
// --------------------------------------------------------------------------




void AddUrqmdGenerator(   FairPrimaryGenerator *primGen, Int_t idx, TString inputFile);
void AddLMVMCocktail(     FairPrimaryGenerator *primGen, Int_t idx, TString inputFile);
void AddRadiationCocktail(FairPrimaryGenerator *primGen, Int_t idx, TString inputFile);
void AddCharmoniaCocktail(FairPrimaryGenerator *primGen, Int_t idx, TString inputFile);
void AddFragmentsCocktail(FairPrimaryGenerator *primGen, Int_t idx);
void AddBoxGenerator(     FairPrimaryGenerator *primGen, Int_t mult);

void SetParticleDecays();
void ConfigureMCStack();


void run_sim_new(Int_t nEvents = 2,
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
  TString myName = "run_sim_new";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString outDir  = gSystem->Getenv("OUTDIR");
  if(outDir.IsNull())  outDir = "data/";
  TString geoFile = outDir + setupName + "_geofile_full.root";
  TString outFile = outDir + setupName + "_mc.root";
  TString parFile = outDir + setupName + "_params.root";
  // ------------------------------------------------------------------------


  // -----   Remove old CTest runtime dependency file   ---------------------
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_sim" , setupName);
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
  std::cout << std::endl;
  setup->SetActive(kPsd, kFALSE); // dont write psd points
  // --- remove detector geometries
  setup->RemoveModule(kPsd);      // remove psd from setup
  //  setup->RemoveModule(kMvd);  // remove mvd and its material budget
  // setup->RemoveModule(kTrd);   // e.g. for sts-tof-matching study
  // --- change default geomerties
  //  setup->SetModule(kTrd, "v15d_1e", kTRUE); // 5 TRD layer
  std::cout << "-I- " << myName << ": CbmSetup updated " << std::endl;
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
  Double_t targetThickness = 0.0025; // full thickness in cm
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


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  TString inFile = ""; // give here or as argument; otherwise default is taken
  TString defaultInputFile = srcDir + "/input/urqmd.auau.10gev.centr.root";
  if ( inFile.IsNull() ) {  // Not defined in the macro explicitly
  	if ( strcmp(inputFile, "") == 0 ) {  // not given as argument to the macro
  		inFile = defaultInputFile;
  	}
  	else inFile = inputFile;
  }
  //  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger* gLogger = FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
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
  	std::cout << "-E- " << myName << ": No valid field!";
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

  // ---   Create particle cocktail
  TObjArray *arr = outDir.Tokenize("/");
  Int_t idx = 1;
  if(arr->GetEntriesFast()>3) {
    TString tmp=(static_cast<TObjString*>(arr->Last()))->GetString();
    idx=tmp.Atoi();
  }
  delete arr;

  // NOTE: for random file selection set idx to -1
  idx=-1;
  gRandom->SetSeed(0);

  TString location="/lustre/nyx"; //"/hera"; //"/scratch"

  TString charmoniaFile =
    location + "/cbm/users/jbook/pluto/train/";
  /// NOTE: charmonia generator needs to be added before the urqmd, because of evtgen
  //AddCharmoniaCocktail( primGen, idx, charmoniaFile );

  /// AA urqmd
  TString urqmdFile =
    // location + "/cbm/prod/gen/urqmd/auau/8gev/mbias/urqmd.auau.8gev.mbias.";
    // location + "/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr.";
    location + "/cbm/users/jbook/urqmd/auau/8gev/centr010/";                    //selfmade UrQMD
  /// pA urqmd
    // location + "/cbm/users/jbook/urqmd/pau/30gev/centr/";                    //selfmade UrQMD
  //  AddUrqmdGenerator(    primGen, idx, urqmdFile );

  TString plutoFile =
    location + "/cbm/users/jbook/pluto/train/";
    // location + "/cbm/users/ekrebs/pluto/jun15/auau/cktA/8gev/";             //for mumu
  //  AddLMVMCocktail(      primGen, idx, plutoFile );

  TString radiationFile =
    location + "/cbm/users/jbook/pluto/inmed/out_rapp_pluto_ee_inmed_";
    // location + "/cbm/users/ekrebs/pluto/jun15/auau/cktRapp/8gev/";         // for mumu
  //AddRadiationCocktail( primGen, idx, radiationFile );

  //  AddFragmentsCocktail( primGen, 1 );

  AddBoxGenerator(      primGen, 2);


  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init(); // this inits gMC and the stack
  // ------------------------------------------------------------------------

  // -----   Particle decays in geant3   ------------------------------------
  SetParticleDecays();
  // ------------------------------------------------------------------------

  // -----   Configure MC stack   -------------------------------------------
  ConfigureMCStack();
  // ------------------------------------------------------------------------


  // -----   Parameter database   ---------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* rootIo = new FairParRootFileIo(kParameterMerged);
  rootIo->open(parFile.Data());
  rtdb->setOutput(rootIo);
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

// ------------------------------------------------------------------------
void AddUrqmdGenerator(FairPrimaryGenerator* primGen, Int_t idx, TString inputFile)
{
  /// UrQMD version 3.3 - 5k files with 1k events each
  /// checkout https://lxcbmredmine01.gsi.de/projects/cbmroot/wiki/Mass_Production_at_GSI
  ///
  Bool_t isAA   = inputFile.Contains("auau");
  Bool_t isOWN  = inputFile.Contains("jbook");

  Int_t module = (isAA ? (isOWN ? 10000: 5000) : 10000);
	Int_t idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );

  TString file="";
  if(isOWN)     file = inputFile + Form("/%05d",idy+1) + "/urqmd.root";
  else if(isAA) file = inputFile + Form("%05d",idy+1)  + ".root";        /// AA
  else          file = inputFile + Form("%05d",idy+1)  + ".urqmd.f14";   /// pA
  printf("urqmd file: %s \n",file.Data());

  if(isAA || isOWN) {
    CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(file);
    primGen->AddGenerator( uniGen );
  }
  else {
    FairUrqmdGenerator*  urqmdGen = new FairUrqmdGenerator(file);
    urqmdGen->SetEventPlane(0. , 360.);
    primGen->AddGenerator( urqmdGen );
  }
  return;
}

// ------------------------------------------------------------------------
void AddLMVMCocktail(     FairPrimaryGenerator *primGen, Int_t idx, TString inputFile)
{
  ///
  ///  DIELECTRON-COCKTAIL: add particles via pluto generator
  ///

  Int_t module = 10000;
  /*
  TString idy="";
  if(inputFile.Contains("ekrebs")) {
    // dimuon
    idy=Form("%04d",idx+1);
    inputFile += "/rho0/mpmm/pluto.auau.8gev.rho0.mpmm." +idy + ".root";
  } else {
    // dielectron
    idy=Form("%05d",(idx%10000)+1);
    inputFile += idy + "/pluto.auau.8gev.rho0.epem.root";
  }
  printf("pluto vec.meson: %s \n",inputFile.Data());
  */

  Int_t idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  TString file = inputFile + Form("%05d",idy+1) + "/pluto.auau.8gev.rho0.epem.root";
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoRho= new CbmPlutoGenerator(file);
  //  primGen->AddGenerator(plutoRho); // not added, included in inmed spectral func

  idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  file = inputFile + Form("%05d",idy+1) + "/pluto.auau.8gev.rho0.epem.root";
  file.ReplaceAll("rho0","omega");
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoOmega= new CbmPlutoGenerator(file);
  primGen->AddGenerator(plutoOmega);

  idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  file = inputFile + Form("%05d",idy+1) + "/pluto.auau.8gev.rho0.epem.root";
  file.ReplaceAll("rho0","phi");
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoPhi= new CbmPlutoGenerator(file);
  primGen->AddGenerator(plutoPhi);

  idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  file = inputFile + Form("%05d",idy+1) + "/pluto.auau.8gev.rho0.epem.root";
  file.ReplaceAll("rho0","omega");
  file.ReplaceAll("epem","pi0epem");
  file.ReplaceAll("mpmm","pi0mpmm");
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoOmegaDalitz= new CbmPlutoGenerator(file);
  primGen->AddGenerator(plutoOmegaDalitz);

  // idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  // file = inputFile + Form("%05d",idy+1) + "/pluto.auau.8gev.rho0.epem.root";
  // file.ReplaceAll("rho0","phi");
  // file.ReplaceAll("epem","kpkm");
  // file.ReplaceAll("mpmm","kpkm");
  // printf("pluto vec.meson: %s \n",file.Data());
  //  CbmPlutoGenerator *plutoPhiKK= new CbmPlutoGenerator(file);
  //  primGen->AddGenerator(plutoPhiKK);

  return;
}

// ------------------------------------------------------------------------
void AddRadiationCocktail(FairPrimaryGenerator *primGen, Int_t idx, TString inputFile)
{
  ///
  /// in-medium and qgp radiation
  ///

  Int_t module = 500;
  /*
  TString idy="";
  if(inputFile.Contains("ekrebs")) {
    // dimuon
    idy=Form("%04d",(idx%5000)+1);
    inputFile  += "rapp.inmed/mpmm/pluto.auau.25gev.rapp.inmed.mpmm." + idy + ".root";
  } else {
    // dielectron
    idy=Form("%04d",(idx%500)+1);
    inputFile += idy + ".root";
  }
  */
  printf("pluto radiation: %s \n",inputFile.Data());

  Int_t idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  TString file = inputFile + Form("%04d",idy+1) + ".root";
  printf("pluto radiation: %s \n",file.Data());
  CbmPlutoGenerator *plutoRadInMed= new CbmPlutoGenerator(file);
  plutoRadInMed->SetManualPDG(99009011);
  primGen->AddGenerator(plutoRadInMed);

  if(!inputFile.Contains("ekrebs")) {
    idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
    file = inputFile + Form("%04d",idy+1) + ".root";
    file.ReplaceAll("inmed","qgp");
    printf("pluto radiation: %s \n",file.Data());
    CbmPlutoGenerator *plutoRadQGP= new CbmPlutoGenerator(file);
    plutoRadQGP->SetManualPDG(99009111);
    primGen->AddGenerator(plutoRadQGP);
  }
  return;
}

// ------------------------------------------------------------------------
void AddFragmentsCocktail(FairPrimaryGenerator *primGen, Int_t mult)
{
  ///
  ///  HeavyFragments-COCKTAIL: add particles via fair box generator
  ///

  ///NOTE: the heavy fragements are not in the pdg table by default, so we have to add them:
  TDatabasePDG *pdgDB = TDatabasePDG::Instance();
  // PDG nuclear states are 10-digit numbers
  // 10LZZZAAAI e.g. deuteron is 1000010020
  // add IONS
  const Int_t kion=1000000000;
  const Double_t khSlash = 1.0545726663e-27;
  const Double_t kErg2Gev = 1/1.6021773349e-3;
  const Double_t khShGev = khSlash*kErg2Gev;
  const Double_t kYear2Sec = 3600*24*365.25;
  // Done by default now from Pythia6 table
  // check if already defined

  Int_t ionCode = kion+10020;
  if(!pdgDB->GetParticle(ionCode))  pdgDB->AddParticle("Deuteron","Deuteron", 1.875613, kTRUE, 0,3,"Ion",ionCode);
  pdgDB->AddAntiParticle("AntiDeuteron", - ionCode);

  ionCode = kion+10030;
  if(!pdgDB->GetParticle(ionCode))  pdgDB->AddParticle("Triton","Triton", 2.80925, kFALSE, khShGev/(12.33*kYear2Sec),3,"Ion",ionCode);
  pdgDB->AddAntiParticle("AntiTriton", - ionCode);

  ionCode = kion+20030;
  if(!pdgDB->GetParticle(ionCode))  pdgDB->AddParticle("HE3","HE3", 2.80923,kFALSE, 0,6,"Ion",ionCode);
  pdgDB->AddAntiParticle("AntiHE3", - ionCode);

  ionCode = kion+20040;
  if(!pdgDB->GetParticle(ionCode))  pdgDB->AddParticle("Alpha","Alpha", 3.727379, kTRUE, khShGev/(12.33*kYear2Sec), 6, "Ion", ionCode);
  pdgDB->AddAntiParticle("AntiAlpha", - ionCode);

  // Use box generators for defined particles and per event multiplcities (pdg,mult)
  FairBoxGenerator* boxGenDeu = new FairBoxGenerator(1000010020, mult);
  boxGenDeu->SetPtRange(0.,3.);
  boxGenDeu->SetPhiRange(0.,360.);
  boxGenDeu->SetThetaRange(2.5,25.);
  boxGenDeu->SetCosTheta();
  boxGenDeu->SetDebug(kTRUE);
  boxGenDeu->Init();
  primGen->AddGenerator(boxGenDeu);

  FairBoxGenerator* boxGenTri = new FairBoxGenerator(1000010030, mult);
  boxGenTri->SetPtRange(0.,3.);
  boxGenTri->SetPhiRange(0.,360.);
  boxGenTri->SetThetaRange(2.5,25.);
  boxGenTri->SetCosTheta();
  boxGenTri->SetDebug(kTRUE);
  boxGenTri->Init();
  primGen->AddGenerator(boxGenTri);

  FairBoxGenerator* boxGenHe3 = new FairBoxGenerator(1000020030, mult);
  boxGenHe3->SetPtRange(0.,3.);
  boxGenHe3->SetPhiRange(0.,360.);
  boxGenHe3->SetThetaRange(2.5,25.);
  boxGenHe3->SetCosTheta();
  boxGenHe3->SetDebug(kTRUE);
  boxGenHe3->Init();
  primGen->AddGenerator(boxGenHe3);

  FairBoxGenerator* boxGenAlp = new FairBoxGenerator(1000020040, mult);
  boxGenAlp->SetPtRange(0.,3.);
  boxGenAlp->SetPhiRange(0.,360.);
  boxGenAlp->SetThetaRange(2.5,25.);
  boxGenAlp->SetCosTheta();
  boxGenAlp->SetDebug(kTRUE);
  boxGenAlp->Init();
  primGen->AddGenerator(boxGenAlp);

  return;
}

// ------------------------------------------------------------------------
void AddCharmoniaCocktail(FairPrimaryGenerator *primGen, Int_t idx, TString inputFile)
{
  ///
  ///  CHARMONIA: add particles via pluto generator, decay them via TEvtGen
  ///

  Int_t module = 10000;

  ///NOTE: this is for pAu collisisons simulations, either jpsi OR psi2S produced in the event
  // TString idu=Form("%05d",idx+1);
  // inputFile += idu + "/pluto.pau.30gev.jpsi.epem.root";   // OR "/pluto.pau.30gev.jpsi.root"

  Int_t idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  TString file = inputFile + Form("%05d",idy+1) + "/pluto.pau.30gev.jpsi.epem.root";
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoJpsi= new CbmPlutoGenerator(file);
  if(idy%2) primGen->AddGenerator(plutoJpsi);

  /// for radiative decay
  CbmGenEvtGen *evtgen = new CbmGenEvtGen();
  evtgen->SetForceDecay(kBJpsiDiElectron);
  evtgen->SetParticleSwitchedOff(CbmGenEvtGen::kJPsiPart);
  if(idy%2) primGen->AddGenerator(evtgen);

  /// NOTE: do NOT decay with evtgen
  //  idy = (idx >= 0 ? idx%module : gRandom->Integer(module) );
  file = inputFile + Form("%05d",idy+1) + "/pluto.pau.30gev.jpsi.epem.root";
  file.ReplaceAll("jpsi","psi2s");
  //inputFile.ReplaceAll("jpsi","psi2s.epem");
  printf("pluto vec.meson: %s \n",file.Data());
  CbmPlutoGenerator *plutoPsi2S= new CbmPlutoGenerator(file);
  plutoPsi2S->SetManualPDG(100443);
  if(!(idy%2)) primGen->AddGenerator(plutoPsi2S);

  return;
}

// ------------------------------------------------------------------------
void AddBoxGenerator(     FairPrimaryGenerator *primGen, Int_t mult)
{
  ///
  /// Use box generators for defined particles and per event multiplcities (pdg,mult)
  ///

  FairBoxGenerator* boxGenEle = new FairBoxGenerator(11, mult);
  boxGenEle->SetPtRange(0.,3.);
  boxGenEle->SetPhiRange(0.,360.);
  boxGenEle->SetThetaRange(2.5,25.);
  boxGenEle->SetCosTheta();
  boxGenEle->SetDebug(kTRUE);
  boxGenEle->Init();
  primGen->AddGenerator(boxGenEle);

  FairBoxGenerator* boxGenPiM = new FairBoxGenerator(-211, mult);
  boxGenPiM->SetPtRange(0.,3.);
  boxGenPiM->SetPhiRange(0.,360.);
  boxGenPiM->SetThetaRange(2.5,25.);
  boxGenPiM->SetCosTheta();
  boxGenPiM->SetDebug(kTRUE);
  boxGenPiM->Init();
  primGen->AddGenerator(boxGenPiM);

  FairBoxGenerator* boxGenPosi = new FairBoxGenerator(-11, mult);
  boxGenPosi->SetPtRange(0.,3.);
  boxGenPosi->SetPhiRange(0.,360.);
  boxGenPosi->SetThetaRange(2.5,25.);
  boxGenPosi->SetCosTheta();
  boxGenPosi->SetDebug(kTRUE);
  boxGenPosi->Init();
  primGen->AddGenerator(boxGenPosi);

  FairBoxGenerator* boxGenPiP = new FairBoxGenerator(+211, mult);
  boxGenPiP->SetPtRange(0.,3.);
  boxGenPiP->SetPhiRange(0.,360.);
  boxGenPiP->SetThetaRange(2.5,25.);
  boxGenPiP->SetCosTheta();
  boxGenPiP->SetDebug(kTRUE);
  boxGenPiP->Init();
  primGen->AddGenerator(boxGenPiP);

  return;
}

// ------------------------------------------------------------------------
void SetParticleDecays()
{
  ///
  /// set and unset particle decays for geant3
  ///
  TGeant3* gMC3 = (TGeant3*) gMC;

  /// switch off certain decays
  gMC3->SetUserDecay(443);   // Force the decay to be done w/external decayer
  gMC3->SetUserDecay(100443);// Force the decay to be done w/external decayer
  std::cout << "----------------- switch OFF: jpsi decay by geant3 -----------------" << std::endl;

  /// set branching ratios by hand
  Float_t bratioEta[6];
  Int_t modeEta[6];

  for (Int_t kz = 0; kz < 6; ++kz) {
    bratioEta[kz] = 0.;
    modeEta[kz]   = 0;
  }

  Int_t ipa    = 17;
  bratioEta[0] = 39.38;  //2gamma
  bratioEta[1] = 32.20;  //3pi0
  bratioEta[2] = 22.70;  //pi+pi-pi0
  bratioEta[3] = 4.69;   //pi+pi-gamma
  bratioEta[4] = 0.60;   //e+e-gamma
  bratioEta[5] = 4.4e-2; //pi02gamma

  modeEta[0] = 101;    //2gamma
  modeEta[1] = 70707;  //3pi0
  modeEta[2] = 80907;  //pi+pi-pi0
  modeEta[3] = 80901;  //pi+pi-gamma
  modeEta[4] = 30201;  //e+e-gamma
  modeEta[5] = 10107;  //pi02gamma
  gMC3->Gsdk(ipa, bratioEta, modeEta);

  Float_t bratioPi0[6];
  Int_t modePi0[6];

  for (Int_t kz = 0; kz < 6; ++kz) {
    bratioPi0[kz] = 0.;
    modePi0[kz] = 0;
  }

  ipa = 7;
  bratioPi0[0] = 98.798;
  bratioPi0[1] = 1.198;

  modePi0[0] = 101;
  modePi0[1] = 30201;

  gMC3->Gsdk(ipa, bratioPi0, modePi0);

  /*
  TDatabasePDG *pdg = TDatabasePDG::Instance();
  Int_t mother    = 0;
  Float_t br[10]  = {0.};
  Int_t  mode[10] = {0};

  ////////////  pi0 decay //////////
  mother = 111;
  TParticlePDG *p1 = pdg->GetParticle(mother);
  p1->Print(); //decay info

  // loop over all decay channels
  TDecayChannel *dc=0x0;
  for(Int_t i=0; i<10; i++) { br[i]=0.; mode[i]=0; } //reset
  for(Int_t i=0; i<p1->NDecayChannels(); i++) {
    dc      = p1->DecayChannel(i);
    br[i]   = dc->BranchingRatio();
    mode[i] = 0;
    // loop over all daughters
    for(Int_t d=0; d<dc->NDaughters(); d++) {
      mode[i] += pdg->ConvertPdgToGeant3(dc->DaughterPdgCode(d)) * TMath::Power(100,d);
    }
  }
  for(Int_t i=0; i<10; i++)
    Printf("i%d --> BR:%.3e \t mode:%d ",i,br[i],mode[i]);
  gMC3->Gsdk(pdg->ConvertPdgToGeant3(mother), br, mode);


  ////////////  eta decay //////////
  mother = 221;
  TParticlePDG *p1 = pdg->GetParticle(mother);
  p1->Print(); //decay info

  // loop over all decay channels
  TDecayChannel *dc=0x0;
  for(Int_t i=0; i<10; i++) { br[i]=0.; mode[i]=0; } //reset
  for(Int_t i=0; i<p1->NDecayChannels(); i++) {
    dc      = p1->DecayChannel(i);
    br[i]   = dc->BranchingRatio();
    mode[i] = 0;
    // loop over all daughters
    for(Int_t d=0; d<dc->NDaughters(); d++) {
      mode[i] += pdg->ConvertPdgToGeant3(dc->DaughterPdgCode(d)) * TMath::Power(100,d);
    }
  }
  for(Int_t i=0; i<10; i++)
    Printf("i%d --> BR:%.3e \t mode:%d ",i,br[i],mode[i]);
  gMC3->Gsdk(pdg->ConvertPdgToGeant3(mother), br, mode);
  */

  // set randomizer to gMC
  TRandom3 *rnd = new TRandom3(0); // computed via a TUUID object (unique!)
  gMC->SetRandom(rnd);

}

void ConfigureMCStack()
{
  ///
  /// configure the cbm stack, apply/release cuts
  ///
  CbmStack *stack = dynamic_cast<CbmStack*>(gMC->GetStack());
  if(stack) {
    //    printf("[USER MODIFICATION]: set MC stack cuts by hand! \n");
    //    stack->StoreSecondaries(kTRUE); // default: kTRUE
    stack->SetMinPoints(0);         // default: 1
    //    stack->SetEnergyCut(0.00001);   // default: 0.
    //    stack->StoreMothers(kTRUE);     // default: kTRUE
  }

}
