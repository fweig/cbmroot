// --------------------------------------------------------------------------
//
// Macro for standard simulation (transport + digitisation)
// The transport uses UrQMD input and GEANT3.
// The default digitisation tasks are used.
//
// V. Friese   12/02/2016
//
// For the setup (geometry and field), predefined setups can be chosen
// by the second argument. A list of available setups is given below.
// The input file can be defined explicitly in this macro or by the
// third argument. If none of these options are chosen, a default
// input file distributed with the source code is selected.
//
// The output file (.raw.root) contains raw data (digis) as well as
// Monte-Carlo data.
//
// 2014-06-30 - DE - available setups from geometry/setup:
// 2014-06-30 - DE - sis100_hadron
// 2014-06-30 - DE - sis100_electron
// 2014-06-30 - DE - sis100_muon
// 2014-06-30 - DE - sis300_electron
// 2014-06-30 - DE - sis300_muon
//
// --------------------------------------------------------------------------

#include "../include/rootalias.C"


void run_sim_maf(Int_t nEvents = 10,
		         const char* setupName = "sis100_electron",
		         const char* inputFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Environment   --------------------------------------------------
  TString myName = "run_sim";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString inFile = inputFile; // give here or as argument; otherwise default is taken
  //TString outDir  = "/gluster2/cbm/sim/data/";
  TString outDir  = inFile;//"/opt/CBM/Daten/";
  outDir.Resize(outDir.Last('/'));
  TRegexp Nr ("[.][0-9][0-9][0-9][0-9][0-9][.]");
  TString outFile = outDir + setupName + TString(inputFile)(Nr) + "_test.raw.root";
  TString parFile = outDir + setupName + TString(inputFile)(Nr) + "_params.root";
  TString geoFile = outDir + setupName + TString(inputFile)(Nr) + "_geofile_full.root";
 
  // ------------------------------------------------------------------------


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
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


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList *parFileList = new TList();
  TString geoTag;
  CbmSetup* setup = CbmSetup::Instance();

  setupFunct = setupFunct + setupName + "()";
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
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  run->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
  // ------------------------------------------------------------------------



  // -----   Logger settings   ----------------------------------------------
  FairLogger* gLogger = FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  TString defaultInputFile = srcDir + "/input/urqmd.auau.10gev.centr.root";
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

  // Use the CbmUnigenGenrator for the input
  CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
  primGen->AddGenerator(uniGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------


  // -----   Digitisers   ---------------------------------------------------
  std::cout << std::endl;
  macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/digitize.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("digitize()");
  // ------------------------------------------------------------------------


  // -----   Runtime database   ---------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(),1);
  // ASCII I/O
  FairParAsciiFileIo* asciiIo = new FairParAsciiFileIo();
  asciiIo->open(parFileList, "in");
  rtdb->setFirstInput(asciiIo);
  // ROOT parameter I/O
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* rootIo = new FairParRootFileIo(kParameterMerged);
  rootIo->open(parFile.Data());
  rtdb->setOutput(rootIo);
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

