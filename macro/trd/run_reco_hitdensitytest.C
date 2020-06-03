// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
//
// HitProducers in MVD, RICH, TRD, TOF, ECAL
// Digitizer and HitFinder in STS
// FAST MC for ECAL
// STS track finding and fitting (L1 / KF)
// TRD track finding and fitting (L1 / KF)
// RICH ring finding (ideal) and fitting
// Global track finding (ideal), rich assignment
// Primary vertex finding (ideal)
// Matching of reconstructed and MC tracks in STS, RICH and TRD
//
// V. Friese   24/02/2006
// Version     04/03/2015 (V. Friese)
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
TString myName="run_reco_hitdensity_new";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;


TString defaultInputFile="";
#include "../include/rootalias.C"


void run_reco_hitdensitytest(Int_t nEvents = 2, const char* setupName = "sis100_electron",
	      const char* rawfile="")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;
  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");


  TString outDir  = "/gluster2/cbm/sim/data/";
  //TString outDir  = "/opt/CBM/Daten/";
  TRegexp Nr ("[.][0-9][0-9][0-9][0-9][0-9][.]");
  TString inFile  = outDir + setupName + TString(rawfile)(Nr) + "_test.raw.root";
  TString parFile = outDir + setupName + TString(rawfile)(Nr) + "_params.root";
  TString outFile = outDir + setupName + TString(rawfile)(Nr) + "_test.esd.root";      // Output file
  TString geoFile = outDir + setupName + TString(rawfile)(Nr) + "_geofile_full.root";

  // Function needed for CTest runtime dependency
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_reco" , setupName);

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString inDir = gSystem->Getenv("VMCWORKDIR");
  TString srcDir = inDir;  // top source directory
  TString paramDir = inDir + "/parameters/";

  TString setupFile = inDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = TString("setup_")+setupName;
  setupFunct += "()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  // --- STS digipar file is there only for L1. It is no longer required
  // --- for STS digitisation and should be eventually removed.
  //TObjString stsDigiFile(paramDir + stsDigi);
  //parFileList->Add(&stsDigiFile);
  //std::cout << "macro/run/run_reco.C using: " << stsDigi << std::endl;

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


  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run = new FairRunAna();
  FairFileSource *inputFile = new FairFileSource(inFile);
  run->SetSource(inputFile);
  run->SetGeomFile(geoFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) {
    FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  }

  // ------------------------------------------------------------------------

  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================


  // =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

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

  /*

  Bool_t  simpleTR  = kTRUE;  // use fast and simple version for TR production
  CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR,"K++");
  //"K++" : micro structured POKALON
  //"H++" : PE foam foils
  //"G30" : ALICE fibers 30 layers

  Bool_t triangularPads = false;// Bucharest triangular pad-plane layout
  Double_t triggerThreshold = 0.5e-6;//SIS100
  //Double_t triggerThreshold = 1.0e-6;//SIS300
  Double_t trdNoiseSigma_keV = 0.1; //default best matching to test beam PRF

  CbmTrdDigitizerPRF* trdDigiPrf = new CbmTrdDigitizerPRF(radiator);
  trdDigiPrf->SetTriangularPads(triangularPads);
  trdDigiPrf->SetNoiseLevel(trdNoiseSigma_keV);
  run->AddTask(trdDigiPrf);

  CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
  trdCluster->SetNeighbourTrigger(true);
  trdCluster->SetTriggerThreshold(triggerThreshold);
  trdCluster->SetNeighbourRowTrigger(false);
  trdCluster->SetPrimaryClusterRowMerger(true);
  trdCluster->SetTriangularPads(triangularPads);
  run->AddTask(trdCluster);

  CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
  trdHit->SetTriangularPads(triangularPads);
  run->AddTask(trdHit);
*/

  printf("CbmTrdHitDensityQa\n");
  CbmTrdHitDensityQa *trdDensityTest = new CbmTrdHitDensityQa(/*"HitDensityTest","Hit Density Test",radiator*/);
  run->AddTask(trdDensityTest);

  // -------------------------------------------------------------------------
  // ===                 End of TRD local reconstruction                   ===
  // =========================================================================



/*
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------
*/

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


  // -----   Intialise and run   --------------------------------------------
  run->Init();
  std::cout << "Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  // ------------------------------------------------------------------------

  if (hasFairMonitor) {
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

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
}
