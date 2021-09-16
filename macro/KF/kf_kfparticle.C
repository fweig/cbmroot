/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of short-lived particles with KF Particle Finder
//
// M. Zyzak   28/11/2018
//
// Version 2018-22-28
//
// 1 parameter - number of events to be processed
// 2 parameter - geometry setup to be tested
// 3 parameter - the prefix for the input and
// 3 parameter - the PID method: kTRUE - real pid, kFALSE - mc pid
// 4 parameter - defines if super event analysis should be run
// 5 parameter - defines, which signal is being analysed, the number should
//               correspond to the scheme of KFPartEfficiencies.h. If "-1"
//               the analysis of
//
// The output files are:
// [dataset].phys.root - a general output, by default is not filled
// [dataset].KFParticleFinder.root - a set of histograms
// [dataset].Efficiency_KFParticleFinder.txt - a file with efficiencies
// --------------------------------------------------------------------------

void kf_kfparticle(Int_t nEvents = 2, const TString setupName = "sis100_electron", const TString dataset = "test",
                   const Bool_t useDetectorPID = kTRUE, const Bool_t superEvent = kFALSE, const int iDecay = -1)
{
  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString macroName = "run_kfparticle";               // this macro's name for screen output
  TString srcDir    = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString paramDir  = srcDir + "/parameters";
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  TString mcFile    = dataset + ".tra.root";
  TString parFile   = dataset + ".par.root";
  TString rawFile   = dataset + ".event.raw.root";
  TString recFile   = dataset + ".rec.root";
  TString outFile   = dataset + ".phys.root";
  TString effFile   = dataset + ".Efficiency_KFParticleFinder.txt";
  TString histoFile = dataset + ".KFParticleFinder.root";
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << macroName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  TString geoTag;
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ------------------------------------------------------------------------

  // ----- Check if the simulation and reconstruction are complited ---------
  TFile* fileMC = new TFile(mcFile);
  if (fileMC->IsOpen()) {
    TTree* treeMC = (TTree*) fileMC->Get("cbmsim");
    if (!treeMC) {
      std::cout << "[FATAL  ]  No MC tree available." << std::endl;
      return;
    }
    if (treeMC->GetEntriesFast() < nEvents) {
      std::cout << "[FATAL  ]  Simulation is incomplete. N mc events = " << treeMC->GetEntriesFast() << std::endl;
      return;
    }
  }
  else {
    std::cout << "[FATAL  ]  MC file does not exist." << std::endl;
    return;
  }

  TFile* fileReco = new TFile(recFile);
  if (fileReco->IsOpen()) {
    TTree* treeReco = (TTree*) fileReco->Get("cbmsim");
    if (!treeReco) {
      std::cout << "[FATAL  ]  No Reco tree available." << std::endl;
      return;
    }
    if (treeReco->GetEntriesFast() < nEvents) {
      std::cout << "[FATAL  ]  Reconstruction is incomplete. N reco events = " << treeReco->GetEntriesFast()
                << std::endl;
      return;
    }
  }
  else {
    std::cout << "[FATAL  ]  Reco file does not exist." << std::endl;
    return;
  }
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  inputSource->AddFriend(mcFile);
  inputSource->AddFriend(recFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------

  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // ----- KF and L1 are needed for field and material   --------------------
  CbmKF* KF = new CbmKF();
  run->AddTask(KF);
  CbmL1* l1 = new CbmL1("CbmL1", 1, 3);
  if (setup->IsActive(ECbmModuleId::kMvd)) {
    setup->GetGeoTag(ECbmModuleId::kMvd, geoTag);
    const TString mvdMatBudgetFileName = paramDir + "/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if (setup->IsActive(ECbmModuleId::kSts)) {
    setup->GetGeoTag(ECbmModuleId::kSts, geoTag);
    const TString stsMatBudgetFileName = paramDir + "/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);
  // ------------------------------------------------------------------------

  // ----- PID for KF Particle Finder ---------------------------------------
  CbmKFParticleFinderPID* kfParticleFinderPID = new CbmKFParticleFinderPID();
  kfParticleFinderPID->SetSIS100();
  if (useDetectorPID) {
    kfParticleFinderPID->UseDetectorPID();
    if (setup->IsActive(ECbmModuleId::kMuch)) {
      kfParticleFinderPID->UseMuch();
      kfParticleFinderPID->SetNMinStsHitsForMuon(7);
      kfParticleFinderPID->SetNMinMuchHitsForLMVM(10);
      kfParticleFinderPID->SetNMinMuchHitsForJPsi(11);
      kfParticleFinderPID->SetMaxChi2ForStsMuonTrack(3);
      kfParticleFinderPID->SetMaxChi2ForMuchMuonTrack(3);
    }
    else {
      kfParticleFinderPID->UseTRDANNPID();
      kfParticleFinderPID->UseRICHRvspPID();
    }
  }
  else
    kfParticleFinderPID->UseMCPID();
  run->AddTask(kfParticleFinderPID);
  // ------------------------------------------------------------------------

  // ----- KF Particle Finder -----------------------------------------------
  CbmKFParticleFinder* kfParticleFinder = new CbmKFParticleFinder();
  kfParticleFinder->SetPIDInformation(kfParticleFinderPID);
  if (iDecay > -1) kfParticleFinder->UseMCPV();
  if (superEvent) kfParticleFinder->SetSuperEventAnalysis();  // SuperEvent
  run->AddTask(kfParticleFinder);
  // ------------------------------------------------------------------------

  // ----- KF Particle Finder QA --------------------------------------------
  CbmKFParticleFinderQa* kfParticleFinderQA =
    new CbmKFParticleFinderQa("CbmKFParticleFinderQa", 0, kfParticleFinder->GetTopoReconstructor(), histoFile.Data());
  kfParticleFinderQA->SetPrintEffFrequency(nEvents);
  if (superEvent) kfParticleFinderQA->SetSuperEventAnalysis();  // SuperEvent
  kfParticleFinderQA->SetEffFileName(effFile.Data());
  if (iDecay > -1) {
    TString referenceResults = srcDir + "/input/qa/KF/reference/";
    if (useDetectorPID) referenceResults += "realpid/";
    else
      referenceResults += "mcpid/";
    kfParticleFinderQA->SetReferenceResults(referenceResults);
    kfParticleFinderQA->SetDecayToAnalyse(iDecay);
    kfParticleFinderQA->SetCheckDecayQA();
  }
  run->AddTask(kfParticleFinderQA);
  // ------------------------------------------------------------------------

  // ----- KF Track QA ------------------------------------------------------
  // The module is under development.
  //   CbmKFTrackQa* kfTrackQA = new CbmKFTrackQa();
  //   run->AddTask(kfTrackQA);
  // ------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------

  // -----   Intialise and run   --------------------------------------------
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  KFPartEfficiencies eff;
  for (int jParticle = eff.fFirstStableParticleIndex + 10; jParticle <= eff.fLastStableParticleIndex; jParticle++) {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if (!pdgDB->GetParticle(eff.partPDG[jParticle])) {
      pdgDB->AddParticle(eff.partTitle[jParticle].data(), eff.partTitle[jParticle].data(), eff.partMass[jParticle],
                         kTRUE, 0, eff.partCharge[jParticle] * 3, "Ion", eff.partPDG[jParticle]);
    }
  }
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  if (hasFairMonitor) FairMonitor::GetMonitor()->Print();
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  if (iDecay > -1) {
    if (kfParticleFinderQA->IsTestPassed()) {
      std::cout << " Test passed" << std::endl;
      std::cout << " All ok " << std::endl;
    }
  }
  // ------------------------------------------------------------------------

  // -----   Resource monitoring   ------------------------------------------
  if (Has_Fair_Monitor()) {  // FairRoot Version >= 15.11
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory = sysInfo.GetMaxMemory();
    std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    std::cout << maxMemory;
    std::cout << "</DartMeasurement>" << std::endl;

    Float_t cpuUsage = ctime / rtime;
    std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    std::cout << cpuUsage;
    std::cout << "</DartMeasurement>" << std::endl;
  }
  // ------------------------------------------------------------------------

  // -----   Function needed for CTest runtime dependency   -----------------
  RemoveGeoManager();
  // ------------------------------------------------------------------------
}
