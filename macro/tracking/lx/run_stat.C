void run_stat(int index = -1)
{
   Int_t nEvents = 1000;
   const char* setupName = "sis100_electron";
   //const char* setupName = "sis100_muon_jpsi";
   TString system  = "auau";
   TString beam    = "10gev";
   TString trigger = "mbias";//"centr";
   TString part = "jpsi";
   TString channel = "epem";
   //TString channel = "mpmm";

   bool useSig = true;
   bool useBg = false;
   bool sigAscii = false;

   if (!useSig && !useBg)
   {
      cout << "At least one of the signal or background must be set!" << endl;
      return;
   }

   TString partDir = "charm";

   if (part != "jpsi")
     partDir = "cktA";

   char str[5];
   sprintf(str, "%05d", index);

   char str2[4];
   sprintf(str2, "%04d", index);

   TString suffix = "";

   if (useBg)
     suffix += "." + trigger;

   if (useSig)
   {
      if (sigAscii)
         suffix += ".ascii";
      else
         suffix += "." + part;
   }

   TString inOutDir;
   
   if (index >= 0)
      inOutDir= "/lustre/nyx/cbm/users/tablyaz/Lx/runs/data" + TString(str) + "/";
   else
      inOutDir = "/data.local/cbmrootdata/";
   
   TString inFile = inOutDir + setupName + ".mc." + system + "." + beam + suffix + ".root";
   TString globalParFile = inOutDir + setupName + ".param." + system + "." + beam + suffix + ".root";
   TString outFile = inOutDir + setupName + ".reco." + system + "." + beam + suffix + ".root";

   TString srcDir = gSystem->Getenv("VMCWORKDIR");

   Int_t iVerbose = 1;
   TStopwatch timer;
   timer.Start();

   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();
   gSystem->Load("libLxTrack.so");

   FairRunAna* run = new FairRunAna;
   run->SetUseFairLinks(1);
   run->SetInputFile(inFile);
   run->SetOutputFile(outFile);
   
   CbmMCDataManager* mcManager = new CbmMCDataManager;
   mcManager->AddFile(inFile);
   run->AddTask(mcManager);
   
   // -----   Load the geometry setup   -------------------------------------
   std::cout << std::endl;
   TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
   TString setupFunct = "setup_";
   setupFunct = setupFunct + setupName + "()";
   std::cout << "-I- : Loading macro " << setupFile << std::endl;
   gROOT->LoadMacro(setupFile);
   gROOT->ProcessLine(setupFunct);
   CbmSetup* setup = CbmSetup::Instance();
   // ------------------------------------------------------------------------
   
   TList* parFileList = new TList();
   /*TString geoTag;

   if (setup->GetGeoTag(kTrd, geoTag))
   {
      TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
      parFileList->Add(trdFile);
      std::cout << "-I- : Using parameter file " << trdFile->GetString() << std::endl;
   }

   // -----   MUCH Digitiser   ------------------------------------------------
   if (setup->IsActive(kMuch))
   {
      // --- Parameter file name
      TString geoTag;
      setup->GetGeoTag(kMuch, geoTag);
      std::cout << geoTag(0, 4) << std::endl;
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile = parFile + "/parameters/much/much_" + geoTag(0, 4)
         + "_digi_sector.root";
      std::cout << "Using parameter file " << parFile << std::endl;

      // --- Digitiser for GEM
      FairTask* gemDigi = new CbmMuchDigitizeGem(parFile.Data());
      run->AddTask(gemDigi);
      std::cout << "-I- digitize: Added task " << gemDigi->GetName()
         << std::endl;

      // --- Digitiser for Straws
      FairTask* strawDigi = new CbmMuchDigitizeStraws(parFile.Data());
      run->AddTask(strawDigi);
      std::cout << "-I- digitize: Added task " << strawDigi->GetName()
         << std::endl;
   }
   // -------------------------------------------------------------------------


   // -----   TRD Digitiser   -------------------------------------------------
   if (setup->IsActive(kTrd))
   {
      CbmTrdRadiator *radiator = new CbmTrdRadiator(kTRUE, "K++");
      FairTask* trdDigi = new CbmTrdDigitizerPRF(radiator);
      run->AddTask(trdDigi);
      std::cout << "-I- digitize: Added task " << trdDigi->GetName()
         << std::endl;
   }
   // -------------------------------------------------------------------------

   // -----   Local reconstruction in MUCH   ---------------------------------
   if (setup->IsActive(kMuch))
   {
      // --- Parameter file name
      TString geoTag;
      setup->GetGeoTag(kMuch, geoTag);
      std::cout << geoTag(0, 4) << std::endl;
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile = parFile + "/parameters/much/much_" + geoTag(0, 4)
         + "_digi_sector.root";
      std::cout << "Using parameter file " << parFile << std::endl;

      // --- Hit finder for GEMs
      FairTask* muchHitGem = new CbmMuchFindHitsGem(parFile.Data());
      run->AddTask(muchHitGem);

      // --- Hit finder for Straws
      CbmMuchFindHitsStraws* strawFindHits =
         new CbmMuchFindHitsStraws(parFile.Data());
      run->AddTask(strawFindHits);

   }



   // -----   Local reconstruction in TRD   ----------------------------------
   if (setup->IsActive(kTrd))
   {

      Double_t triggerThreshold = 0.5e-6; // SIS100
      Bool_t triangularPads = false; // Bucharest triangular pad-plane layout
      CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
      trdCluster->SetNeighbourTrigger(true);
      trdCluster->SetTriggerThreshold(triggerThreshold);
      trdCluster->SetNeighbourRowTrigger(false);
      trdCluster->SetPrimaryClusterRowMerger(true);
      trdCluster->SetTriangularPads(triangularPads);
      run->AddTask(trdCluster);
      std::cout << "-I- : Added task " << trdCluster->GetName() << std::endl;

      CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
      trdHit->SetTriangularPads(triangularPads);
      run->AddTask(trdHit);
      std::cout << "-I- : Added task " << trdHit->GetName() << std::endl;

   }
   // ------------------------------------------------------------------------
   */

   LxCalcStats* calcStats = new LxCalcStats;
   run->AddTask(calcStats);
    

   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   FairParRootFileIo* parIo1 = new FairParRootFileIo();
   parIo1->open(globalParFile.Data());
   rtdb->setFirstInput(parIo1);
   rtdb->setOutput(parIo1);   
   rtdb->saveOutput();
   // ------------------------------------------------------------------------

   // -----   Initialize and run   --------------------------------------------
   run->Init();
   //run->Run(900, 1000);
   run->Run(0, nEvents);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------
   timer.Stop();
   cout << "Macro finished successfully." << endl;
   cout << "Output file is "    << inFile << endl;
   cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << endl;
   cout << endl;
   // ------------------------------------------------------------------------

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
}
