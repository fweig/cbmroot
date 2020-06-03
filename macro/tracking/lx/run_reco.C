void run_reco(int index = -1)
{
   Int_t nEvents = 1000;
   const char* setupName = "sis100_muon_jpsi";
   //const char* setupName = "sis100_electron";
   TString system  = "auau";
   TString beam    = "8gev";
   TString trigger = "mbias";
   TString part = "omega";
   TString channel = "mpmm";
   //TString channel = "epem";

   bool useSig = true;
   bool useBg = false;
   bool sigAscii = false;
   bool useIdeal = false;
   bool isEvByEv = true;
   bool isML = true;

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

   TString myName = "run_reco_lx";  // this macro's name for screen output
   TString srcDir = gSystem->Getenv("VMCWORKDIR");

   Int_t iVerbose = 1;
   TStopwatch timer;
   timer.Start();

   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();
   gSystem->Load("libLxTrack.so");
   gSystem->Load("libLittrack.so");

   FairRunAna* run = new FairRunAna;
   run->SetUseFairLinks(1);
   run->SetInputFile(inFile);
   run->SetOutputFile(outFile);
   run->SetGenerateRunInfo(kTRUE);
   
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
   
   //if (nofNoiseE > 0)
   //{
      //LxGenNoiseElectrons* elGenerator = new LxGenNoiseElectrons;
      //elGenerator->SetNofNoiseE(nofNoiseE);
      //run->AddTask(elGenerator);
   //}
   
   TList* parFileList = new TList();
   
   if (useIdeal)
   {
      TString muchDigiFileName = srcDir + "/parameters/much/much_v15c_digi_sector.root";
      CbmMuchHitProducerIdeal* muchHPIdeal = new CbmMuchHitProducerIdeal(muchDigiFileName.Data());
      run->AddTask(muchHPIdeal);
   
      CbmTrdHitProducerIdeal* trdHPIdeal = new CbmTrdHitProducerIdeal;
      run->AddTask(trdHPIdeal);
   }
   else
   {
      TString geoTag;
      
      if (setup->GetGeoTag(kTrd, geoTag))
      {
         TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
         parFileList->Add(trdFile);
         std::cout << "-I- : Using parameter file " << trdFile->GetString() << std::endl;
      }
      
      // - TOF digitisation parameters
      if (setup->GetGeoTag(kTof, geoTag)) {
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
      
      // -----   MVD Digitiser   -------------------------------------------------
      if (setup->IsActive(kMvd)) {
         FairTask* mvdDigi = new CbmMvdDigitizer("MVD Digitiser", 0);
         run->AddTask(mvdDigi);
         std::cout << "-I- digitize: Added task " << mvdDigi->GetName()
            << std::endl;
      }
      // -------------------------------------------------------------------------


      // -----   STS Digitiser   -------------------------------------------------
      if (setup->IsActive(kSts)) {

         Double_t dynRange = 40960.; // Dynamic range [e]
         Double_t threshold = 4000.; // Digitisation threshold [e]
         Int_t nAdc = 4096; // Number of ADC channels (12 bit)
         Double_t timeResolution = 5.; // time resolution [ns]
         Double_t deadTime = 9999999.; // infinite dead time (integrate entire event)
         Double_t noise = 0.; // ENC [e]
         Int_t digiModel = 1; // User sensor type DSSD

         // The following settings correspond to a validated implementation.
         // Changing them is on your own risk.
         Int_t eLossModel = 1; // Energy loss model: uniform
         Bool_t useLorentzShift = kFALSE; // Deactivate Lorentz shift
         Bool_t useDiffusion = kFALSE; // Deactivate diffusion
         Bool_t useCrossTalk = kFALSE; // Deactivate cross talk

         CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
         stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
         stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
         run->AddTask(stsDigi);
      }
      // -------------------------------------------------------------------------
      
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
      
      // -----   TOF Digitiser   -------------------------------------------------
      if (setup->IsActive(kTof)) {
         Int_t iVerbose = 0;
         CbmTofDigitizerBDF* tofDigi = new CbmTofDigitizerBDF("TOF Digitizer BDF", iVerbose);
         tofDigi->SetOutputBranchPersistent("TofDigi", kFALSE);
         tofDigi->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
         TString paramDir = gSystem->Getenv("VMCWORKDIR");
         tofDigi->SetInputFileName(paramDir + "/parameters/tof/test_bdf_input.root"); // Required as input file name not read anymore by param class
         run->AddTask(tofDigi);

         std::cout << "-I- digitize: Added task " << tofDigi->GetName()
            << std::endl;
      }
      // -------------------------------------------------------------------------
      
      // -----   Local reconstruction in MVD   ----------------------------------
      if (setup->IsActive(kMvd)) {

         CbmMvdClusterfinder* mvdCluster =
            new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
         run->AddTask(mvdCluster);
         std::cout << "-I- : Added task " << mvdCluster->GetName() << std::endl;

         CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
         mvdHit->UseClusterfinder(kTRUE);
         run->AddTask(mvdHit);
         std::cout << "-I- : Added task " << mvdHit->GetName() << std::endl;

      }
      // ------------------------------------------------------------------------


      // -----   Local reconstruction in STS   ----------------------------------
      if (setup->IsActive(kSts)) {

         FairTask* stsCluster = new CbmStsFindClusters();
         run->AddTask(stsCluster);
         std::cout << "-I- : Added task " << stsCluster->GetName() << std::endl;

         FairTask* stsHit = new CbmStsFindHits();
         run->AddTask(stsHit);
         std::cout << "-I- : Added task " << stsHit->GetName() << std::endl;

      }
      // ------------------------------------------------------------------------
      
      // -----   Local reconstruction in MUCH   ---------------------------------
      if (setup->IsActive(kMuch)) {

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
      if (setup->IsActive(kTrd)) {

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
      
      // -----   Local reconstruction in TOF   ----------------------------------
      if (setup->IsActive(kTof)) {
         CbmTofSimpClusterizer* tofCluster
            = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
         tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
         tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
         run->AddTask(tofCluster);
         std::cout << "-I- : Added task " << tofCluster->GetName() << std::endl;
      }
      // -------------------------------------------------------------------------
   }
   
   // -----   Track finding in (MVD+) STS    -----------------------------------------
   CbmKF* kalman = new CbmKF();
   run->AddTask(kalman);
   CbmL1* l1 = new CbmL1();
   // --- Material budget file names
   if (setup->IsActive(kMvd)) {
      TString geoTag;
      setup->GetGeoTag(kMvd, geoTag);
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile = parFile + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetMvdMaterialBudgetFileName(parFile.Data());
   }
   if (setup->IsActive(kSts)) {
      TString geoTag;
      setup->GetGeoTag(kSts, geoTag);
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile = parFile + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetStsMaterialBudgetFileName(parFile.Data());
   }
   run->AddTask(l1);
   std::cout << "-I- : Added task " << l1->GetName() << std::endl;

   CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
   FairTask* stsFindTracks = new CbmStsFindTracks(0, stsTrackFinder, setup->IsActive(kMvd));
   run->AddTask(stsFindTracks);
   std::cout << "-I- : Added task " << stsFindTracks->GetName() << std::endl;
   // -------------------------------------------------------------------------

   TString setupString(setupName);
   
   if (setupString.Contains("electron"))
   {
      LxTBTrdFinder* lxTbFinder = new LxTBTrdFinder;
      run->AddTask(lxTbFinder);
   }
   else
   {
      if (isML)
      {
         LxTBMLFinder* lxTbFinder = new LxTBMLFinder;
         lxTbFinder->SetEvByEv(isEvByEv);
         run->AddTask(lxTbFinder);
      }
      else
      {
         LxTBFinder* lxTbFinder = new LxTBFinder;
         lxTbFinder->SetEvByEv(isEvByEv);
         lxTbFinder->SetSignalParticle(part.Data());
         lxTbFinder->SetUseIdeal(useIdeal);
         lxTbFinder->SetUseAsciiSig(sigAscii);
         lxTbFinder->SetNEvents(nEvents);
         run->AddTask(lxTbFinder);
      }
   }
    

   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   FairParRootFileIo* parIo1 = new FairParRootFileIo();
   parIo1->open(globalParFile.Data());
   rtdb->setFirstInput(parIo1);
   rtdb->setOutput(parIo1); 
   
   if (!useIdeal)
   {
      FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
      parIo2->open(parFileList, "in");
      rtdb->setSecondInput(parIo2);
   }
   
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
   cout << "Parameter file is " << globalParFile << endl;
   cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << endl;
   cout << endl;
   // ------------------------------------------------------------------------

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
}
