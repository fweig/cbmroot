// --------------------------------------------------------------------------
//
// Macro for transport simulation with the TOF
// to generate the normalization histograms for the TOF points/digi/hits maps.
// ROOTinos will be transported either parallel to the Z axis or in straight 
// tracks from origin.
// Efficiency of TOF must be set to 100% and cluster size to 0 (1 channel fired 
// at most) in the digitizer.
// From the output MC file, the number of tracks per bin is determined, which 
// allow to get the "nominal" number of points/digi/hit per track in this bin.
// Thus we can compensate detectors/channels overlaps.
//
// P.-A. Loizeau, 31.08.2015
// Based on macro/mcbm/matbudget_mc_mcbm_sts.C and matbudget_ana_mcbm_sts.C
//
// --------------------------------------------------------------------------

void tof_map_norm_gen_mc(TString geoVersion, Int_t iCoordType = 0, Int_t iPartType = 0, Int_t nEvents = 1000000 )
{
   TString sCoordType = ""; 
   if( 0 == iCoordType )
      sCoordType = "xyz"; 
   else if( 1 == iCoordType )
      sCoordType = "ang"; 
   else if( 2 == iCoordType )
      sCoordType = "sph";
      else return; // No reason to enter other values!

   TString sPartType = ""; 
   if( 0 == iPartType )
      sPartType = "geantino"; 
   else if( 1 == iPartType )
      sPartType = "proton"; 
      else return; // No reason to enter other values!

//  TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "WARNING";
//  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  
  // ========================================================================
  //          Adjust this part according to your requirements
  	

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString inFile  = inDir + "/input/urqmd.ftn14";
  TString outDir  = "data";
  TString outFile = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + ".mc.root";
  TString parFile = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + ".params.root";
  
  // -----  Geometries  -----------------------------------------------------
  TString caveGeom   = "cave.geo";
  TString targetGeom = "";
  TString pipeGeom   = "";
  TString magnetGeom = "";
  TString mvdGeom    = "";
  TString stsGeom    = "";
  TString richGeom   = "";
  TString trdGeom    = "";
  TString tofGeom    = "tof/tof_" + geoVersion + ".geo.root";
  
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
  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  gLogger->SetLogScreenLevel(logLevel.Data());
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------


  // -----   Create detectors and passive volumes   -------------------------
  if ( caveGeom != "" ) {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    run->AddModule(cave);
  }

  if ( pipeGeom != "" ) {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    run->AddModule(pipe);
  }
  
  if ( targetGeom != "" ) {
    FairModule* target = new CbmTarget("Target");
    target->SetGeometryFileName(targetGeom);
    run->AddModule(target);
  }

  if ( magnetGeom != "" ) {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    run->AddModule(magnet);
  }
  
  if ( mvdGeom != "" ) {
    FairDetector* mvd = new CbmMvd("MVD", kTRUE);
    mvd->SetGeometryFileName(mvdGeom);
    run->AddModule(mvd);
  }

  if ( stsGeom != "" ) {
    FairDetector* sts = new CbmStsMC(kTRUE);
    sts->SetGeometryFileName(stsGeom);
    run->AddModule(sts);
  }

  if ( richGeom != "" ) {
    FairDetector* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    run->AddModule(rich);
  }

  if ( trdGeom != "" ) {
    FairDetector* trd = new CbmTrd("TRD",kTRUE );
    trd->SetGeometryFileName(trdGeom);
    run->AddModule(trd);
  }

  if ( tofGeom != "" ) {
    FairDetector* tof = new CbmTof("TOF", kTRUE);
    tof->SetGeometryFileName(tofGeom);
    run->AddModule(tof);
  }
  
  // ------------------------------------------------------------------------



  // -----   Create magnetic field   ----------------------------------------
  // Zero field
  CbmFieldConst *magField = new CbmFieldConst();
  magField->SetField(0, 0 ,0 ); // values are in kG
  magField->SetFieldRegion(-74, -39 ,-22 , 74, 39 , 160 );  
  run->SetField(magField);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  run->SetGenerator(primGen);

  // --- Primary particles
  FairBoxGenerator* boxGen;
  if( 0 == iPartType )
      // Generated are ROOTinos (PDG=0), 100 per events
      boxGen = new FairBoxGenerator(0,  99);
   else if( 1 == iPartType )
      // Generated are protons (PDG=2212), 100 per events.
      boxGen = new FairBoxGenerator(2212,  99);
  // The starting points in x and y or the angles are chosen such as to 
  // illuminate the standard TOF wall and match the histo ranges in the 
  // TofHitFinderQa class.
  if( 0 == iCoordType )
  {
     // Cartesian coord: in z direction, starting at z = 0.
     boxGen->SetBoxXYZ(-750.,-500.,750.,500.,0.);
     boxGen->SetPRange( 0.1, 10.0); // arbitrary, not sure if any effect to expect here
     boxGen->SetThetaRange(0.,0.);
     boxGen->SetPhiRange(0.,360.);
  } // if( 0 == iCoordType )
  else if( 1 == iCoordType )
  {
     // angular coord: in all directions, starting at (0,0,0) => (ThetaX, ThetaY).
     boxGen->SetPRange( 0.1, 10.0); // arbitrary, not sure if any effect to expect here
     boxGen->SetThetaRange(0.,  70.); // approx. sqrt(2)*60 (maximum in thetaX)
     boxGen->SetPhiRange(  0., 360.);
  } // if( 1 == iCoordType )
  else if( 2 == iCoordType )
  {
     // Spherical coord: in all directions, starting at (0,0,0) => (Theta, Phi).
     boxGen->SetPRange( 0.1, 10.0); // arbitrary, not sure if any effect to expect here
     boxGen->SetThetaRange(0.,  90.);
     boxGen->SetPhiRange(  0., 360.);
  } // if( 2 == iCoordType )
	
  primGen->AddGenerator(boxGen);

  // ------------------------------------------------------------------------

  // -----   Run initialisation   -------------------------------------------
  run->Init();
  // ------------------------------------------------------------------------
  
  // -----   Runtime database   ---------------------------------------------
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
  run->Run(nEvents);
  // ------------------------------------------------------------------------

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

