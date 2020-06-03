// --------------------------------------------------------------------------
//
// Macro for TOF digitzer testing
//
// Digitizer in TOF
// Clusterizer in TOF
// Test class for TOF (Point/Digi/Hit)
//
// P.-A. Loizeau   13/03/2015
// Version         13/03/2015 (P.-A. Loizeau)
//
// --------------------------------------------------------------------------


void tof_map_norm_gen_ana(TString geoVersion, Int_t iCoordType = 0, Int_t iPartType = 0, Int_t nEvents = 10000000)
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
//  TString logLevel = "WARNING";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");
  TString outDir  = "data/";

  // Input file (MC events)
  TString inFile  = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + ".mc.root";
  // Parameter file
  TString parFile = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + ".params.root";

  // Output file
  TString outFile = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + "_qa.eds.root";
  TString digiOutFile  = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + "_DigiBdf.hst.root";
  TString clustOutFile = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + "_SimpClust.hst.root"; 
  TString qaOutFile    = outDir + "/norm." + geoVersion + "_" + sCoordType + "_" + sPartType + "_qa.hst.root"; 

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
  TString tofDigi    = "tof/tof_" + geoVersion + ".digi.par";

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters/";

  TObjString tofDigiFile = paramDir + tofDigi;
  parFileList->Add(&tofDigiFile);

  TObjString tofDigiBdfFile = "./tof.digibdf_norm.par";
  if( "v14-0a" == geoVersion  || "v14-0b" == geoVersion)
      tofDigiBdfFile = "./tof.digibdf_norm_v14_0.par";
  parFileList->Add(&tofDigiBdfFile);


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
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  gLogger->SetLogScreenLevel(logLevel.Data());
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================

  // -----   TOF digitizer   -------------------------------------------------
  CbmTofDigitizerBDF* tofDigitizerBdf = new CbmTofDigitizerBDF("TOF Digitizer BDF",iVerbose);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigi",            kFALSE);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
  tofDigitizerBdf->SetInputFileName( paramDir + "tof/test_bdf_input.root"); // Required as input file name not read anymore by param class
  tofDigitizerBdf->SetHistoFileName( digiOutFile );
  run->AddTask(tofDigitizerBdf);


  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================
  // Cluster/Hit builder
  CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer",iVerbose);
  tofSimpClust->SetOutputBranchPersistent("TofHit",          kFALSE);
  tofSimpClust->SetOutputBranchPersistent("TofDigiMatch",    kFALSE);
  tofSimpClust->SetHistoFileName( clustOutFile );
  run->AddTask(tofSimpClust);
  // -------------------------------------------------------------------------

  // ===                   End of TOF local reconstruction                 ===
  // =========================================================================

  // =========================================================================
  // ===                     TOF evaluation                                ===
  // =========================================================================

  // Digitizer/custerizer testing
  CbmTofHitFinderQa* tofQa = new CbmTofHitFinderQa("TOF QA",iVerbose);
  tofQa->SetHistoFileName( qaOutFile );
  tofQa->SetNormHistGenerationMode();
  // TODO: add the position for the other versions
  if( "v13-5a" == geoVersion || "v14-0a" == geoVersion )
      tofQa->SetWallPosZ(  550 );
  else if( "v13-5d" == geoVersion || "v14-0b" == geoVersion)
      tofQa->SetWallPosZ(  900 );
      else tofQa->SetWallPosZ( 1000 ); // default position of the wall
  run->AddTask(tofQa);

  // ===                   End of TOF evaluation                           ===
  // =========================================================================
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


  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

//  delete run;

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
}
