// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
// Standard CBM setup with MVD, STS, RICH, TRD, TOF and ECAL
//
// V. Friese   22/02/2007
//
// 2014-06-30 - DE - available setups from geometry/setup:
// 2014-06-30 - DE - sis100_hadron
// 2014-06-30 - DE - sis100_electron
// 2014-06-30 - DE - sis100_muon
// 2014-06-30 - DE - sis300_electron
// 2014-06-30 - DE - sis300_muon
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

void tof_qa_sim(Int_t nEvents = 2, const char* setup = "sis100_electron", 
                const char* geoOption = "", Int_t iRandSeed = 0)
{

   // ========================================================================
   //          Adjust this part according to your requirements
      // Random seed for random generator!
   gRandom->SetSeed(iRandSeed);

   // ----- Paths and file names  --------------------------------------------
   TString inDir   = gSystem->Getenv("VMCWORKDIR");

   TString outDir  = "data/";
   TString outFile = outDir + setup + "_test.mc.root";
   TString parFile = outDir + setup + "_params.root";
   TString geoFile = outDir + setup + "_geofile_full.root";

   TString setupFile = inDir + "/geometry/setup/" + setup + "_setup.C";
   TString setupFunct = setup;
   setupFunct += "_setup()";

   gROOT->LoadMacro(setupFile);
   gInterpreter->ProcessLine(setupFunct);

   TString inFile  = inDir + defaultInputFile;

   CbmTarget* target = new CbmTarget("Gold", 0.025);

   if( geoOption=="noTRD" ) trdGeom  = "";

   // Disable PSD for testing the effect of TRD on TOF
   psdGeom = "";

   // If SCRIPT environment variable is set to "yes", i.e. macro is run via script
   TString script = TString(gSystem->Getenv("LIT_SCRIPT"));
   if (script == "yes") 
   {
      inFile  = TString(gSystem->Getenv("LIT_URQMD_FILE"));
      outFile = TString(gSystem->Getenv("LIT_MC_FILE"));
      parFile = TString(gSystem->Getenv("LIT_PAR_FILE"));
      geoFile = TString(gSystem->Getenv("LIT_FULLGEO_FILE"));
   } // if (script == "yes")


   // In general, the following parts need not be touched
   // ========================================================================

   cout << "[INFO   ] Setup: " << setup << endl;

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
   fRun->SetGenerateRunInfo(kTRUE);       // Create FairRunInfo file
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

   if ( target ) fRun->AddModule(target);

   if ( magnetGeom != "" ) {
      FairModule* magnet = new CbmMagnet("MAGNET");
      magnet->SetGeometryFileName(magnetGeom);
      fRun->AddModule(magnet);
   }

   if ( platformGeom != "" ) {
      FairModule* platform = new CbmPlatform("PLATFORM");
      platform->SetGeometryFileName(platformGeom);
      fRun->AddModule(platform);
   }

   if ( mvdGeom != "" ) {
      FairDetector* mvd = new CbmMvd("MVD", kTRUE);
      mvd->SetGeometryFileName(mvdGeom);
      mvd->SetMotherVolume("pipevac1");
      fRun->AddModule(mvd);
   }

   if ( stsGeom != "" ) {
      FairDetector* sts = new CbmStsMC(kTRUE);
      sts->SetGeometryFileName(stsGeom);
      fRun->AddModule(sts);
   }

   if ( richGeom != "" ) {
      FairDetector* rich = new CbmRich("RICH", kTRUE);
      rich->SetGeometryFileName(richGeom);
      fRun->AddModule(rich);
   }

   if ( muchGeom != "" ) {
      FairDetector* much = new CbmMuch("MUCH", kTRUE);
      much->SetGeometryFileName(muchGeom);
      fRun->AddModule(much);
   }

   if ( shieldGeom != "" ) {
      FairModule* shield = new CbmShield("SHIELD");
      shield->SetGeometryFileName(shieldGeom);
      fRun->AddModule(shield);
   }

   if ( trdGeom != "" ) {
      FairDetector* trd = new CbmTrd("TRD",kTRUE );
      trd->SetGeometryFileName(trdGeom);
      fRun->AddModule(trd);
   }

   if ( tofGeom != "" ) {
      FairDetector* tof = new CbmTof("TOF", kTRUE);
      tof->SetGeometryFileName(tofGeom);
      fRun->AddModule(tof);
   }

   if ( ecalGeom != "" ) {
      FairDetector* ecal = new CbmEcal("ECAL", kTRUE, ecalGeom.Data()); 
      fRun->AddModule(ecal);
   }

   if ( psdGeom != "" ) {
      cout << "Constructing PSD" << endl;
      CbmPsdv1* psd= new CbmPsdv1("PSD", kTRUE);  
      psd->SetZposition(psdZpos); // in cm
      psd->SetXshift(psdXpos);  // in cm    
      psd->SetGeoFile(psdGeom);  
      fRun->AddModule(psd);
   }

   // ------------------------------------------------------------------------

   // -----   Create magnetic field   ----------------------------------------
   CbmFieldMap* magField = NULL;
   if ( 2 == fieldSymType ) {
      magField = new CbmFieldMapSym2(fieldMap);
   }  else if ( 3 == fieldSymType ) {
      magField = new CbmFieldMapSym3(fieldMap);
   } 
   magField->SetPosition(0., 0., fieldZ);
   magField->SetScale(fieldScale);
   fRun->SetField(magField);
   // ------------------------------------------------------------------------

   // Use the experiment specific MC Event header instead of the default one
   // This one stores additional information about the reaction plane
   CbmMCEventHeader* mcHeader = new CbmMCEventHeader();
   fRun->SetMCEventHeader(mcHeader);

   // -----   Create PrimaryGenerator   --------------------------------------
   FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
   // Use the CbmUnigenGenrator which calculates a reaction plane and
   // rotate all particles accordingly
   CbmUnigenGenerator*  uniGen = new CbmUnigenGenerator(inFile);
   uniGen->SetEventPlane(0. , 360.);
   primGen->AddGenerator(uniGen);
   fRun->SetGenerator(primGen);       
   // ------------------------------------------------------------------------


   // Trajectories Visualization (TGeoManager Only)
   // Switch this on if you want to visualize tracks in the
   // eventdisplay.
   // This is normally switch off, because of the huge files created
   // when it is switched on. 
   fRun->SetStoreTraj(kFALSE);

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
//   fRun->CreateGeometryFile(geoFile);

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

