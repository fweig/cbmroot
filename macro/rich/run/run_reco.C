void run_reco(const string& traFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/tra.00000.root",
              const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/par.00000.root",
              const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/raw.00000.root",
              const string& recoFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/reco.00000.root",
              const string& geoSetup = "sis100_electron", const string& resultDir = "results_recoqa_newqa/",
              int nofTimeSlices = 3)
{

  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  remove(recoFile.c_str());

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(geoSetup.c_str());

  TString sEvBuildRaw = "";
  Bool_t useMC        = true;

  Bool_t eventBased = (sEvBuildRaw != "");
  Bool_t useMvd     = geo->IsActive(ECbmModuleId::kMvd);
  Bool_t useSts     = geo->IsActive(ECbmModuleId::kSts);
  Bool_t useRich    = geo->IsActive(ECbmModuleId::kRich);
  Bool_t useMuch    = geo->IsActive(ECbmModuleId::kMuch);
  Bool_t useTrd     = geo->IsActive(ECbmModuleId::kTrd);
  Bool_t useTof     = geo->IsActive(ECbmModuleId::kTof);
  Bool_t usePsd     = geo->IsActive(ECbmModuleId::kPsd);

  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- "
                << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- "
              << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }

  TStopwatch timer;
  timer.Start();

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  if (useMC) { inputSource->AddFriend(traFile.c_str()); }
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(true);

  if (useMC) {
    CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 0);
    mcManager->AddFile(traFile.c_str());
    run->AddTask(mcManager);
  }

  if (eventBased) {
    if (sEvBuildRaw.EqualTo("Ideal", TString::ECaseCompare::kIgnoreCase)) {
      FairTask* evBuildRaw = new CbmBuildEventsIdeal();
      run->AddTask(evBuildRaw);
      std::cout << "-I- "
                << ": Added task " << evBuildRaw->GetName() << std::endl;
      eventBased = kTRUE;
    }
    else if (sEvBuildRaw.EqualTo("Real", TString::ECaseCompare::kIgnoreCase)) {
      CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

      //Choose between NoOverlap, MergeOverlap, AllowOverlap
      evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

      // Remove detectors where digis not found
      if (!useRich) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
      if (!useMuch) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
      if (!usePsd) evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
      if (!useTof) evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);
      if (!useTrd) evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
      if (!useSts) {
        std::cerr << "-E- "
                  << ": Sts must be present for raw event "
                  << "building using ``Real2019'' option. Terminating macro." << std::endl;
        return;
      }
      // Set STS as reference detector
      evBuildRaw->SetReferenceDetector(kRawEventBuilderDetSts);
      evBuildRaw->SetTsParameters(0.0, 1.e7, 0.0);

      // Use CbmMuchDigi instead of CbmMuchBeamtimeDigi
      evBuildRaw->ChangeMuchBeamtimeDigiFlag(kFALSE);

      evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, 1000);
      evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -500, 500);

      run->AddTask(evBuildRaw);
      std::cout << "-I- "
                << ": Added task " << evBuildRaw->GetName() << std::endl;
      eventBased = kTRUE;
    }  //? Real raw event building
    else {
      std::cerr << "-E- "
                << ": Unknown option " << sEvBuildRaw << " for raw event building! Terminating macro execution."
                << std::endl;
      return;
    }
  }


  if (eventBased && useMC) {
    CbmBuildEventsQA* evBuildQA = new CbmBuildEventsQA();
    run->AddTask(evBuildQA);
  }

  if (useMvd) {

    CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
    run->AddTask(mvdCluster);
    std::cout << "-I- : Added task " << mvdCluster->GetName() << std::endl;

    CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
    mvdHit->UseClusterfinder(kTRUE);
    run->AddTask(mvdHit);
    std::cout << "-I- "
              << ": Added task " << mvdHit->GetName() << std::endl;
  }


  if (useSts) {
    CbmRecoSts* stsReco = new CbmRecoSts(kCbmRecoTimeslice);
    if (eventBased) stsReco->SetMode(kCbmRecoEvent);
    run->AddTask(stsReco);
    std::cout << "-I- "
              << ": Added task " << stsReco->GetName() << std::endl;
  }


  if (useRich) {
    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);
    std::cout << "-I- "
              << ": Added task " << richHitProd->GetName() << std::endl;
  }


  if (useTrd) {

    Double_t triggerThreshold       = 0.5e-6;  // SIS100
    CbmTrdClusterFinder* trdCluster = new CbmTrdClusterFinder();
    if (eventBased) trdCluster->SetTimeBased(kFALSE);
    else
      trdCluster->SetTimeBased(kTRUE);
    trdCluster->SetNeighbourEnable(true, false);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetRowMerger(true);

    // Uncomment if you want to use all available digis.
    // In that case clusters hits will not be added to the CbmEvent
    // trdCluster->SetUseOnlyEventDigis(kFALSE);

    run->AddTask(trdCluster);
    std::cout << "-I- "
              << ": Added task " << trdCluster->GetName() << std::endl;

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- "
              << ": Added task " << trdHit->GetName() << std::endl;
  }

  if (useTof) {
    CbmTofSimpClusterizer* tofCluster = new CbmTofSimpClusterizer("TofSimpClusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
    run->AddTask(tofCluster);
    std::cout << "-I- "
              << ": Added task " << tofCluster->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------

  if (useMC) {
    CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
    run->AddTask(match1);
  }


  if (useMvd || useSts) {
    CbmKF* kalman = new CbmKF();
    run->AddTask(kalman);
    CbmL1* l1 = 0;
    if (useMC) { l1 = new CbmL1("L1", 2, 3); }
    else {
      l1 = new CbmL1("L1", 0);
    }
    l1->SetDataMode(!eventBased);

    // --- Material budget file names
    TString mvdGeoTag;
    if (geo->GetGeoTag(ECbmModuleId::kMvd, mvdGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile += "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetMvdMaterialBudgetFileName(parFile.Data());
    }
    TString stsGeoTag;
    if (geo->GetGeoTag(ECbmModuleId::kSts, stsGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile += "/parameters/sts/sts_matbudget_" + stsGeoTag + ".root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetStsMaterialBudgetFileName(parFile.Data());
    }
    run->AddTask(l1);
    std::cout << "-I- "
              << ": Added task " << l1->GetName() << std::endl;

    CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
    if (eventBased) {
      FairTask* stsFindTracks = new CbmStsFindTracksEvents(stsTrackFinder, useMvd);
      run->AddTask(stsFindTracks);
      std::cout << "-I- "
                << ": Added task " << stsFindTracks->GetName() << std::endl;
    }
    else {
      FairTask* stsFindTracks = new CbmStsFindTracks(0, stsTrackFinder, useMvd);
      run->AddTask(stsFindTracks);
      std::cout << "-I- "
                << ": Added task " << stsFindTracks->GetName() << std::endl;
    }
  }
  // ------------------------------------------------------------------------


  // ---   Global track finding   -----------------------------------------
  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  finder->SetTrackingType("branch");
  finder->SetMergerType("nearest_hit");
  run->AddTask(finder);
  std::cout << "-I- : Added task " << finder->GetName() << std::endl;
  // ----------------------------------------------------------------------


  // -----   RICH reconstruction   ----------------------------------------
  if (useRich) {
    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    run->AddTask(richReco);
    std::cout << "-I- : Added task " << richReco->GetName() << std::endl;
  }
  // ----------------------------------------------------------------------

  // ==== From here on, the time-based and the event-based reconstruction
  // ==== chains differ, since time-based version of primary vertex finding
  // ==== and global tracking are not yet available. For time-based
  // ==== reconstruction, a track-based event finder is used; no global
  // ==== tracks are produced.

  if (eventBased) {

    //-----   Primary vertex finding   -------------------------------------
    CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
    CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
    run->AddTask(findVertex);
    std::cout << "-I- "
              << ": Added task " << findVertex->GetName() << std::endl;
    // ----------------------------------------------------------------------


  }  //? event-based reco

  else {

    // -----   Event building from STS tracks   -----------------------------
    run->AddTask(new CbmBuildEventsFromTracksReal());
    // ----------------------------------------------------------------------

  }  //? time-based reco


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- "
            << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- "
            << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Register light ions (d, t, He3, He4)   -------------------------
  std::cout << std::endl;
  TString registerLightIonsMacro = gSystem->Getenv("VMCWORKDIR");
  registerLightIonsMacro += "/macro/KF/registerLightIons.C";
  std::cout << "Loading macro " << registerLightIonsMacro << std::endl;
  gROOT->LoadMacro(registerLightIonsMacro);
  gROOT->ProcessLine("registerLightIons()");
  // ------------------------------------------------------------------------

  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- "
            << ": Starting run" << std::endl;
  run->Run(0, nofTimeSlices);
  // ------------------------------------------------------------------------

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
