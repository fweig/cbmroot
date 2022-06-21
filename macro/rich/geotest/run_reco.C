/* Copyright (C) 2013-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

void run_reco(const string& testType,  // "geotest" or "urqmdtest"
              const string& traFile, const string& parFile, const string& digiFile, const string& recoFile,
              const string& geoSetup, int nEvents)
{

  if (testType != "urqmdtest" && testType != "geotest") {
    std::cout << "ERROR testType:" << testType << " is not correct. It must be urqmdTest or geoTest" << std::endl;
    return;
  }

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(geoSetup.c_str());

  Bool_t eventBased = false;
  Bool_t useMC      = true;
  Bool_t useMvd     = geo->IsActive(ECbmModuleId::kMvd);
  Bool_t useSts     = geo->IsActive(ECbmModuleId::kSts);

  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(traFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(false);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile.c_str());
  run->AddTask(mcManager);

  if (testType == "urqmdtest") {
    if (useMvd) {
      CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
      run->AddTask(mvdCluster);

      CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
      mvdHit->UseClusterfinder(kTRUE);
      run->AddTask(mvdHit);
    }

    if (useSts) {
      CbmRecoSts* stsReco = new CbmRecoSts(ECbmRecoMode::kCbmRecoTimeslice);
      run->AddTask(stsReco);
    }

    if (useMvd || useSts) {
      run->AddTask(new CbmTrackingDetectorInterfaceInit());
      CbmKF* kalman = new CbmKF();
      run->AddTask(kalman);
      CbmL1* l1 = new CbmL1("L1", 0);

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
      FairTask* stsFindTracks           = new CbmStsFindTracks(0, stsTrackFinder, useMvd);
      run->AddTask(stsFindTracks);
    }

    CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
    finder->SetTrackingType("branch");
    finder->SetMergerType("nearest_hit");
    run->AddTask(finder);
  }

  CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
  run->AddTask(richHitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  if (testType == "geotest") {
    richReco->SetRunExtrapolation(false);
    richReco->SetRunProjection(false);
  }
  richReco->SetRunTrackAssign(false);
  richReco->SetFinderName("ideal");
  run->AddTask(richReco);

  CbmMatchRecoToMC* match = new CbmMatchRecoToMC();
  run->AddTask(match);

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

  run->Run(0, nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Reco file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
