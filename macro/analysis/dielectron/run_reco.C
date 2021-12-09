/* Copyright (C) 2011-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer]*/

// Run this macro with run_local.py for local test and with batch_send(job).py for large productions
void run_reco(const string& traFile, const string& parFile, const string& digiFile, const string& recoFile,
              const string& setup, int nEvents)
{
  TString sEvBuildRaw = "ideal";

  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(setup.c_str());

  Bool_t eventBased = !(sEvBuildRaw == "");
  Bool_t useMC      = true;
  Bool_t useMvd     = geo->IsActive(ECbmModuleId::kMvd);
  Bool_t useSts     = geo->IsActive(ECbmModuleId::kSts);
  Bool_t useRich    = geo->IsActive(ECbmModuleId::kRich);
  Bool_t useTrd     = geo->IsActive(ECbmModuleId::kTrd);
  Bool_t useTof     = geo->IsActive(ECbmModuleId::kTof);

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

  if (useMC) {
    CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 0);
    mcManager->AddFile(traFile.c_str());
    run->AddTask(mcManager);
  }

  // only eventBased Ideal
  if (eventBased) {
    if (sEvBuildRaw.EqualTo("Ideal", TString::ECaseCompare::kIgnoreCase)) {
      FairTask* evBuildRaw = new CbmBuildEventsIdeal();
      run->AddTask(evBuildRaw);
      eventBased = kTRUE;
    }
  }

  if (eventBased && useMC) {
    CbmBuildEventsQa* evBuildQA = new CbmBuildEventsQa();
    run->AddTask(evBuildQA);
  }

  if (useMvd) {
    CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
    run->AddTask(mvdCluster);

    CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
    mvdHit->UseClusterfinder(kTRUE);
    run->AddTask(mvdHit);
  }

  if (useSts) {
    CbmRecoSts* stsReco = new CbmRecoSts(ECbmRecoMode::kCbmRecoTimeslice);
    if (eventBased) stsReco->SetMode(ECbmRecoMode::kCbmRecoEvent);
    run->AddTask(stsReco);
  }

  if (useRich) {
    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);
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

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
  }

  if (useTof) {
    CbmTofSimpClusterizer* tofCluster = new CbmTofSimpClusterizer("TofSimpClusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
    run->AddTask(tofCluster);
  }


  if (useMvd || useSts) {
    CbmKF* kalman = new CbmKF();
    run->AddTask(kalman);
    CbmL1* l1 = 0;
    if (useMC) { l1 = new CbmL1("L1", 2, 3); }
    else {
      l1 = new CbmL1("L1", 0);
    }

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

    CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
    if (eventBased) {
      FairTask* stsFindTracks = new CbmStsFindTracksEvents(stsTrackFinder, useMvd);
      run->AddTask(stsFindTracks);
    }
    else {
      FairTask* stsFindTracks = new CbmStsFindTracks(0, stsTrackFinder, useMvd);
      run->AddTask(stsFindTracks);
    }
  }

  // Support only event-based reco

  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);

  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  finder->SetTrackingType("branch");
  finder->SetMergerType("nearest_hit");
  run->AddTask(finder);

  if (useTrd) {
    CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
    trdLI->SetUseMCInfo(kTRUE);
    trdLI->SetUseMomDependence(kTRUE);
    run->AddTask(trdLI);
    std::cout << "-I- : Added task " << trdLI->GetName() << std::endl;
    // ------------------------------------------------------------------------
  }


  if (useRich) {
    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    run->AddTask(richReco);
  }

  if (useMC) {
    CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
    run->AddTask(match1);
  }

  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();


  // -----   Register light ions (d, t, He3, He4)   -------------------------
  std::cout << std::endl;
  TString registerLightIonsMacro = gSystem->Getenv("VMCWORKDIR");
  registerLightIonsMacro += "/macro/KF/registerLightIons.C";
  std::cout << "Loading macro " << registerLightIonsMacro << std::endl;
  gROOT->LoadMacro(registerLightIonsMacro);
  gROOT->ProcessLine("registerLightIons()");
  // ------------------------------------------------------------------------

  run->Run(0, nEvents);

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is    " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
}
