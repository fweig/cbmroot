// --------------------------------------------------------------------------
//
// Macro for TOF digitzer and clusterizer QA
//
// Digitizer in TOF
// Clusterizer in TOF
// QA class for TOF (Point/Digi/Hit)
//
// P.-A. Loizeau   08/09/2015
// Version         08/09/2015 (P.-A. Loizeau)
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
TString trdDigi="";
TString tofDigi="";

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";


void tof_qa_ana(Int_t nEvents = 2, const char* setup = "sis100_electron", Int_t iRandSeed = 0)
{

  // ========================================================================
  //          Adjust this part according to your requirements
      // Random seed for random generator!
   gRandom->SetSeed(iRandSeed);

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

//  TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
//  TString logLevel = "WARNING";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";

  // ----- Paths and file names  --------------------------------------------
  TString inDir   = gSystem->Getenv("VMCWORKDIR");

  TString outDir  = "data/";

  // Input file (MC events)
  TString inFile = outDir + setup + "_test.mc.root";
  // Parameter file
  TString parFile = outDir + setup + "_params.root";

  // -----  Geometries  -----------------------------------------------------
  TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";
  
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  // TOF maps normalization
  TString normCartInFile = inDir + "/macro/tof/qa/data/norm." + tofTag + "_xyz_geantino_qa.hst.root"; 
  TString normAngInFile  = inDir + "/macro/tof/qa/data/norm." + tofTag + "_ang_geantino_qa.hst.root"; 
  TString normSphInFile  = inDir + "/macro/tof/qa/data/norm." + tofTag + "_sph_geantino_qa.hst.root"; 

  // Output file
  TString outFile = outDir + "/tofqa." + setup + "_qa.eds.root";
  TString digiOutFile  = outDir + "/tofqa." + setup + "_DigiBdf.hst.root";
  TString clustOutFile = outDir + "/tofqa." + setup + "_SimpClust.hst.root"; 
  TString qaOutFile    = outDir + "/tofqa." + setup + "_qa.hst.root"; 
  
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

  TObjString tofDigiBdfFile = paramDir + "/tof/tof_" + tofTag + ".digibdf.par";
  parFileList->Add(&tofDigiBdfFile);

   // If SCRIPT environment variable is set to "yes", i.e. macro is run via script
   TString script = TString(gSystem->Getenv("LIT_SCRIPT"));
   if (script == "yes") 
   {
      inFile       = TString(gSystem->Getenv("LIT_MC_FILE"));
      parFile      = TString(gSystem->Getenv("LIT_PAR_FILE"));
      outFile      = TString(gSystem->Getenv("LIT_RECO_FILE"));
      digiOutFile  = TString(gSystem->Getenv("LIT_HSTDIGI_FILE"));
      clustOutFile = TString(gSystem->Getenv("LIT_HSTCLUST_FILE"));
      qaOutFile    = TString(gSystem->Getenv("LIT_HSTTOFQA_FILE"));
   } // if (script == "yes")

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

  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
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
  // ==========================================================================

  // =========================================================================
  // ===                    Matching to Monte-carlo                        ===
  // =========================================================================
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // ===                  End of matching to Monte-Carlo                   ===
  // =========================================================================

  // =========================================================================
  // ===                     TOF evaluation                                ===
  // =========================================================================

  // Digitizer/custerizer testing
  CbmTofHitFinderQa* tofQa = new CbmTofHitFinderQa("TOF QA",iVerbose);
  tofQa->SetHistoFileNameCartCoordNorm( normCartInFile );
  tofQa->SetHistoFileNameAngCoordNorm(  normAngInFile );
  tofQa->SetHistoFileNameSphCoordNorm(  normSphInFile );
  tofQa->SetHistoFileName( qaOutFile );
  // TODO: add the position for the other versions
  //       or recover it from the geometry?
   if( "v16a_1h" == tofTag )
   {
      tofQa->SetWallPosZ(  500 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  500 cm (v16a_1h)." << endl;
   }
   else if( "v16a_1e" == tofTag )
   {
      tofQa->SetWallPosZ(  650 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  650 cm (v16a_1e)." << endl;
   }
   else if( "v16a_1m" == tofTag )
   {
      tofQa->SetWallPosZ(  730 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  730 cm (v16a_1m)." << endl;
   }
   else if( "v16a_3e" == tofTag )
   {
      tofQa->SetWallPosZ(  930 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  930 cm (v16a_3e)." << endl;
   }
   else if( "v16a_3m" == tofTag )
   {
      tofQa->SetWallPosZ(  1070 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of 1070 cm (v16a_3m)." << endl;
   }
   else if( "v13-5a" == tofTag || "v14-0a" == tofTag )
   {
      tofQa->SetWallPosZ(  550 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  550 cm (v14-0a)." << endl;
   }
   else if( "v13-5d" == tofTag  || "v14-0b" == tofTag )
   {
      tofQa->SetWallPosZ(  900 );
      cout << "Setting TOF QA histograms for a TOF wall Z position of  900 cm (v14-0b)." << endl;
   }
   else
   {
      tofQa->SetWallPosZ( 1000 ); // default position of the wall
      cout << "Setting TOF QA histograms for a TOF wall Z position of 1000 cm (default). tofTag = "
           <<  tofTag << endl;
   }
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
