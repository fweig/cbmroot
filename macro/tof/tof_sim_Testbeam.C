// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// Standard CBM setup with MVD, STS, RICH, TRD, TOF and ECAL
//
// V. Friese   22/02/2007
// processing more than 1 input file does not work. - nh
//
// --------------------------------------------------------------------------

void tof_sim_Testbeam(Int_t nEvents = 10, Int_t iSys=0)
{
  // iSys=25 for particle with UrQMD
  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  //TString inFile  = inDir + "/input/urqmd.auau.8gev.mbias.0000.ftn14";
  //TString inFile  = inDir + "/input/urqmd.AuAu.10gev.centr.0000.ftn14";
  TString outDir  = "data";
  switch(iSys){
  case 0:
    TString outFile = outDir + "/1p.mc.root";
    TString parFile = outDir + "/1p.params.root";
    break;
  case 25:
    TString inFile  = inDir + "/input/urqmd.auau.25gev.centr.0000.ftn14";
    TString outFile = outDir + "/U25cen_1p.mc.root";
    TString parFile = outDir + "/U25cen_1p.params.root";
    break;
  default:
    ;
  }
  // -----  Geometries  -----------------------------------------------------
  TString caveGeom   = "cave.geo";
  //TString targetGeom = "target_au_250mu.geo";
  //TString pipeGeom   = "pipe_standard.geo";
  TString targetGeom = "";
  TString pipeGeom   = "";
  TString magnetGeom = ""; //passive/magnet_v12a.geo";
  TString stsGeom    = ""; //sts/sts_v12b.geo.root";
  //  TString tofGeom    = "tof/tof_V13-2b.geo";
  TString tofGeom    = "tof/tof_v15c.root";
  
  // -----   Magnetic field   -----------------------------------------------
  TString fieldMap    = "";//"field_v12a";   // name of field map
  Double_t fieldZ     = 50.;             // field centre z position
  Double_t fieldScale = 0.001;             // field scaling factor
  
  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // -----------------------------------------------------------------------

  //  gLogger->SetLogScreenLevel("DEBUG");

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");             // Transport engine
  fRun->SetTrackingDebugMode(kFALSE);   // Geant3 debug output can be set in ./gconfig/g3Config.C 
  fRun->SetOutputFile(outFile);         // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create detectors and passive volumes   -------------------------
  if ( caveGeom != "" ) {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    fRun->AddModule(cave);
  }

  if ( pipeGeom != "" ) {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    fRun->AddModule(pipe);
  }
  
  if ( targetGeom != "" ) {
    FairModule* target = new CbmTarget("Target");
    target->SetGeometryFileName(targetGeom);
    fRun->AddModule(target);
  }

  if ( magnetGeom != "" ) {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    fRun->AddModule(magnet);
  }
  
  if ( stsGeom != "" ) {
    FairDetector* sts = new CbmSts("STS", kTRUE);
    sts->SetGeometryFileName(stsGeom);
    fRun->AddModule(sts);
  }

  if ( tofGeom != "" ) {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    fRun->AddModule(tof);
  }
  
  // ------------------------------------------------------------------------



  // -----   Create magnetic field   ----------------------------------------
  /*
  CbmFieldMap* magField = new CbmFieldMapSym2(fieldMap);
  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  */
  // ------------------------------------------------------------------------

  // Use the experiment specific MC Event header instead of the default one
  // This one stores additional information about the reaction plane
  //CbmMCEventHeader* mcHeader = new CbmMCEventHeader();
  //fRun->SetMCEventHeader(mcHeader);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  // Ion Generator
  //  FairIonGenerator *fIongen= new FairIonGenerator(79, 197,79,1, 0.,0., 25, 0.,0.,-1.);
  //  FairParticleGenerator *fPartGen= new FairParticleGenerator(2212, 1.,0.,5., 25., 0.,0.,0.);
  //primGen->AddGenerator(fPartGen);
  Int_t iMode=0;
  switch (iMode) {
    case 0:                                                   //(pdg,mul,px, py, pz, vx,vy,vz)
      FairParticleGenerator *fPartGen= new FairParticleGenerator(2212, 1,0.03,0.38, 3.5, 0.,0.,0.); //proton
      //FairParticleGenerator *fPartGen= new FairParticleGenerator(13, 1,0.03,0.38, 3.5, 0.,0.,0.); //mu-
      break;
    case 1:
      FairBoxGenerator *fPartGen= new FairBoxGenerator(2212, 10);
      fPartGen->SetPtRange(0.,2.5);
      //      fPartGen->SetYRange(0.5,4.);
      fPartGen->SetYRange(2.8,3.2);
      break;
    default:
  }

  primGen->AddGenerator(fPartGen);
  fRun->SetGenerator(primGen);
  // Use the CbmUrqmdGenrator which calculates a reaction plane and
  // rotate all particles accordingly
  if (iSys>0){
  FairUrqmdGenerator*  urqmdGen = new FairUrqmdGenerator(inFile);
  primGen->SetEventPlane(-TMath::Pi(), TMath::Pi());
  //(CbmUrqmdGenerator *)urqmdGen->SetVertex(0.,0.,20.);   // test shifted vertex
  //urqmdGen->SetEventPlane(-180., 180.);
  primGen->AddGenerator(urqmdGen);
  primGen->SetTarget(0., 0.025);
  fRun->SetGenerator(primGen);
  }
  // -Trajectories Visualization (TGeoManager only )
  // Switch this on if you want to visualize tracks in the
  // eventdisplay.
  // This is normally switch off, because of the huge files created
  // when it is switched on. 
  fRun->SetStoreTraj(kTRUE);

  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------
  
  // Set cuts for storing the trajectories.
  // Switch this on only if trajectories are stored.
  // Choose this cuts according to your needs, but be aware
  // that the file size of the output file depends on these cuts

  //FairTrajFilter* trajFilter = FairTrajFilter::Instance();
  //trajFilter->SetStepSizeCut(0.01); // 1 cm
  //trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
  //trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
  //trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
  //trajFilter->SetStorePrimaries(kTRUE);
  //trajFilter->SetStoreSecondaries(kTRUE);

  // -----   Runtime database   ---------------------------------------------
  /*
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
  */
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
  //  fRun->CreateGeometryFile("data/geofile_1p.root");

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

