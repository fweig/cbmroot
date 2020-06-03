// --------------------------------------------------------------------------
//
// Macro for STS-only simulation, digitisation and reconstruction
// of single-track events
//
// Setup: STS only
// Field: v12b
// Input: one proton per event with p = 2 GeV, theta = 15 degrees
//        and random azimuth
// Physics: standard GEANT3
//
// Arguments:
//    - number of events (default 10)
//    - STS geometry version (default v15b)
//
// Output file name: sts_single_raw.root
//
// V. Friese   08/01/2016
//
// --------------------------------------------------------------------------

void sts_sim_single(Int_t nEvents = 10,
		                const char* geoTag = "v15b")
{

  // =====   Logger settings   ==============================================
  TString logLevel   = "INFO";
  TString logVerbose = "LOW";
  // ========================================================================


	// =====   Transport Geometry   ===========================================
  FairModule* cave= new CbmCave("CAVE");
  cave->SetGeometryFileName("cave.geo");

  TString stsGeom = Form("sts/sts_%s.geo.root", geoTag);
  FairDetector* sts= new CbmStsMC(kTRUE);
  sts->SetGeometryFileName(stsGeom);
  // ========================================================================


  // =====   Magnetic field   ===============================================
  TString  fieldMap = "field_v12b";
  Double_t fieldZ   = 40.;
  CbmFieldMap* magField = new CbmFieldMapSym3(fieldMap);
  magField->SetPosition(0., 0., fieldZ);
  // ========================================================================



  // =====   Input generator   ==============================================
  Int_t    pdgId        = 2212;  // proton
  Int_t    multiplicity = 1;     // particles per event
  Double_t p            = 2.;    // momentum magnitude
  Double_t theta        = 15.;   // polar angle (degrees)
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, multiplicity);
  boxGen->SetPRange(p, p);
  boxGen->SetThetaRange(theta, theta);
  boxGen->SetPhiRange();

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  primGen->AddGenerator(boxGen);
  // ========================================================================
  
  

  // =====   STS Digitizer   ================================================
  Int_t digiModel         =       1;   // User sensor type DSSD
  CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
  stsDigi->SetProcessSecondaries(kFALSE);
  // ========================================================================


  // =====   STS Cluster and Hit Finder   ===================================
  FairTask* stsFindClusters = new CbmStsFindClusters(1,0);
  FairTask* stsFindHits     = new CbmStsFindHits();
  // ========================================================================



  // =====   Output files   =================================================
  TString outFile = "sts_single.raw.root";
  TString parFile = "sts_single.params.root";
  // ========================================================================
  
  

  // =====   Run   ==========================================================
  FairRunSim* run = new FairRunSim();
  gLogger->SetLogScreenLevel(logLevel.Data());
  gLogger->SetLogVerbosityLevel(logVerbose.Data());
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  run->SetMaterials("media.geo");       // Materials
  run->AddModule(cave);                 // Geometry: cave
  run->AddModule(sts);                  // Geometry: sts
  run->SetField(magField);              // Magnetic field
  run->SetGenerator(primGen);           // Input generator
  run->AddTask(stsDigi);
  run->AddTask(stsFindClusters);
  run->AddTask(stsFindHits);
  run->Init();
  // ========================================================================

  

  // =====   Runtime database   =============================================
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
  // ========================================================================



  // =====   Run execution   ================================================
  TStopwatch timer;
  timer.Start();
  run->Run(nEvents);
  timer.Stop();
  // ========================================================================

  

  // =====   Finish   =======================================================
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
       << "s" << std::endl << std::endl;
  // ========================================================================
}

