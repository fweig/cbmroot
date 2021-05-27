
void run_sim_mcbm(Int_t nEvents = 10)
{
  TTree::SetMaxTreeSize(90000000000);

  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_sim_mcbm";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString geoSetupFile = srcDir + "/macro/rich/geosetup/rich_setup_sis18_mcbm_20deg_long.C";
  //    TString geoSetupFile = srcDir +"/geometry/setup/setup_sis18_mcbm_20deg_long.C";

  //TString inFile = srcDir + "/input/urqmd.auau.1.24gev.centr.00000.root";
  TString inFile = srcDir + "/input/urqmd.agag.1.65gev.centr.00001.root";
  //   TString outDir = "/home/aghoehne/Documents/CbmRoot/Gregor/";
  TString outDir  = "/Users/slebedev/Development/cbm/data/sim/rich/mcbm/";
  TString parFile = outDir + "param.00000.root";
  TString mcFile  = outDir + "mc.00000.root";
  TString geoFile = outDir + "sis18_mcbm_20deg_long_geofile_full.root";
  //    TString geoFile = outDir + "geosim.00000.root";

  remove(parFile.Data());
  remove(mcFile.Data());
  remove(geoFile.Data());

  if (script == "yes") {
    inFile       = TString(gSystem->Getenv("IN_FILE"));
    mcFile       = TString(gSystem->Getenv("MC_FILE"));
    parFile      = TString(gSystem->Getenv("PAR_FILE"));
    geoFile      = TString(gSystem->Getenv("GEOSIM_FILE"));
    geoSetupFile = srcDir + TString(gSystem->Getenv("GEO_SETUP_FILE"));
  }


  // Target geometry
  TString targetElement    = "Gold";
  Double_t targetThickness = 0.1;  // full thickness in cm
  Double_t targetDiameter  = 0.5;  // diameter in cm
  Double_t targetPosX      = 0.;   // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;   // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;   // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;   // target rotation angle around the y axis [deg]
  Double_t beamRotY        = 20.;  // the primary beam is at 25 degrees to the left of the mCBM setup
  //  Double_t beamRotY        = 25.;    // the primary beam is at 25 degrees to the left of the mCBM setup

  //primary vertex
  Bool_t smearVertexXY = kTRUE;
  Bool_t smearVertexZ  = kTRUE;
  Double_t beamWidthX  = 0.1;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY  = 0.1;  // Gaussian sigma of the beam profile in y [cm]


  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");
  run->SetOutputFile(mcFile);
  run->SetGenerateRunInfo(kTRUE);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");


  TString setupFunct = "do_setup()";
  std::cout << "-I- " << myName << ": Loading macro " << geoSetupFile << std::endl;
  gROOT->LoadMacro(geoSetupFile);
  gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Setting media file" << std::endl;
  run->SetMaterials("media.geo");  // Materials

  // Register setup
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/mcbm/modules/registerSetup.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");


  // Create and register the target
  std::cout << "-I- " << myName << ": Registering target" << std::endl;
  CbmTarget* target = new CbmTarget(targetElement.Data(), targetThickness, targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  target->Print();
  run->AddModule(target);

  // magnetic field
  std::cout << "-I- " << myName << ": Registering magnetic field" << std::endl;
  CbmFieldMap* magField = CbmSetup::Instance()->CreateFieldMap();
  if (!magField) {
    std::cout << "-E- No valid field!";
    return;
  }
  run->SetField(magField);


  // Create PrimaryGenerator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // --- Uniform distribution of event plane angle
  primGen->SetEventPlane(0., 2. * TMath::Pi());
  // --- Get target parameters
  Double_t tX  = 0.;
  Double_t tY  = 0.;
  Double_t tZ  = 0.;
  Double_t tDz = 0.;
  if (target) {
    target->GetPosition(tX, tY, tZ);
    tDz = target->GetThickness();
  }
  primGen->SetTarget(tZ, tDz);
  primGen->SetBeam(0., 0., beamWidthX, beamWidthY);
  primGen->SmearGausVertexXY(smearVertexXY);
  primGen->SmearVertexZ(smearVertexZ);


  // Use the CbmUnigenGenrator for the input
  CbmUnigenGenerator* uniGen = new CbmUnigenGenerator(inFile);
  uniGen->SetEventPlane(0., 360.);
  primGen->AddGenerator(uniGen);
  primGen->SetBeamAngle(beamRotY * TMath::Pi() / 180., 0, 0, 0);  // set direction of beam
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


  // Store trajectories for event display
  run->SetStoreTraj(kTRUE);
  //  // Set cuts for storing the trajectories.
  //   FairTrajFilter* trajFilter = FairTrajFilter::Instance();
  //   if ( trajFilter ) {
  //       trajFilter->SetStepSizeCut(0.01); // 1 cm
  //       trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
  //       trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
  //       trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
  //       trajFilter->SetStorePrimaries(kTRUE);
  //       trajFilter->SetStoreSecondaries(kTRUE);
  //   }

  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  // Runtime database
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb   = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(), 1);
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();


  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(nEvents);

  run->CreateGeometryFile(geoFile);
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  //    gGeoManager->CheckOverlaps();
  //    gGeoManager->PrintOverlaps();
}
