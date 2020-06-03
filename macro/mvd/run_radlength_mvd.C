// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// Standard CBM setup with MVD, STS, RICH, TRD, TOF and ECAL
//
// V. Friese   22/02/2007
//
// --------------------------------------------------------------------------

void run_radlength_mvd(Int_t nEvents = 10000000)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile  = inDir + "/input/urqmd.ftn14";
  TString outDir  = "data";
  TString outFile = outDir + "/radlength.mvd_v15a_vacuum.mc.root";
  TString parFile = outDir + "/params.mvd_v15a_vacuum.root";
  
  // -----  Geometries  -----------------------------------------------------
  TString caveGeom   = "cave.geo";

  TString pipeGeom = "pipe/pipe_v14l.root";
  TString mvdGeom    = "mvd/MVD_v15a_vacuum.root";

  
  // Magnet geometry and field map
  TString magnetGeom  = "magnet/magnet_v12b.geo.root";
  TString fieldMap    = "field_v12b";   // name of field map
  Int_t fieldZ        = 40.;             // field centre z position
  Int_t fieldScale    =  1.;             // field scaling factor
  Int_t fieldSymType  = 3;

  
  // In general, the following parts need not be touched
  // ========================================================================




  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------



  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3");              // Transport engine
  fRun->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  fRun->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create detectors and passive volumes   -------------------------
  if ( caveGeom != "" ) 
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    fRun->AddModule(cave);

  FairModule* pipe= new CbmPipe("PIPE");
  pipe->SetGeometryFileName(pipeGeom);
  fRun->AddModule(pipe);

    FairDetector* mvd = new CbmMvd("MVD", kTRUE);
    mvd->SetGeometryFileName(mvdGeom);
    mvd->SetMotherVolume("pipevac1");
    fRun->AddModule(mvd);

  // ------------------------------------------------------------------------



  // -----   Create magnetic field   ---------------------------------------
  if ( 2 == fieldSymType ) {
    CbmFieldMap* magField = new CbmFieldMapSym2(fieldMap);
  }  else if ( 3 == fieldSymType ) {
    CbmFieldMap* magField = new CbmFieldMapSym3(fieldMap);
  } 
  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  // ------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);

  //ROOTino
  FairBoxGenerator *fBox1 = new FairBoxGenerator(0, 1);
     fBox1->SetBoxXYZ(-15.,-15.,15.,15.,0.);
     fBox1->SetPRange(0.1,0.5);
     fBox1->SetThetaRange(0.,0.);
     fBox1->SetPhiRange(0.,360.);
     primGen->AddGenerator(fBox1);

   fRun->SetStoreTraj(kFALSE);
   fRun->SetRadLenRegister(kTRUE);
  // ------------------------------------------------------------------------

  // -----   Run initialisation   -------------------------------------------
  fRun->Init();
  // ------------------------------------------------------------------------
  
  // -----   Runtime database   ---------------------------------------------
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
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
  fRun->CreateGeometryFile("data/geo_rad_mvd15a_vacuum.root");


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

