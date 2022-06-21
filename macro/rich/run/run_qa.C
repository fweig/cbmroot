/* Copyright (C) 2019-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

void run_qa(const string& traFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/tra.0.root",
            const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/reco/par.0.root",
            const string& digiFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/digi.0.root",
            const string& recoFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/reco.0.root",
            const string& qaFile   = "/Users/slebedev/Development/cbm/data/sim/rich/reco/qa.0.root",
            const string& geoSetup = "sis100_electron", const string& resultDir = "results_recoqa_apr21plus/",
            int nEvents = 100)
{

  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  remove(qaFile.c_str());

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(geoSetup.c_str());

  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
  }

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  inputSource->AddFriend(traFile.c_str());
  inputSource->AddFriend(recoFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(qaFile.c_str());
  run->SetGenerateRunInfo(kTRUE);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile.c_str());
  run->AddTask(mcManager);

  //    // RICH reco QA
  CbmRichRecoQa* richRecoQa = new CbmRichRecoQa();
  richRecoQa->SetOutputDir(resultDir);
  run->AddTask(richRecoQa);

  //    // Reconstruction Qa
  CbmLitTrackingQa* trackingQa = new CbmLitTrackingQa();
  trackingQa->SetMinNofPointsSts(4);
  trackingQa->SetUseConsecutivePointsInSts(true);
  trackingQa->SetMinNofPointsTrd(2);
  trackingQa->SetMinNofPointsMuch(10);
  trackingQa->SetMinNofPointsTof(1);
  trackingQa->SetQuota(0.7);
  trackingQa->SetMinNofHitsTrd(2);
  trackingQa->SetMinNofHitsMuch(10);
  trackingQa->SetVerbose(0);
  trackingQa->SetMinNofHitsRich(7);
  trackingQa->SetQuotaRich(0.6);
  trackingQa->SetOutputDir(resultDir);
  trackingQa->SetPRange(12, 0., 6.);
  // trackingQa->SetTrdAnnCut(trdAnnCut);
  std::vector<std::string> trackCat, richCat;
  trackCat.push_back("All");
  trackCat.push_back("Electron");
  richCat.push_back("Electron");
  richCat.push_back("ElectronReference");
  trackingQa->SetTrackCategories(trackCat);
  trackingQa->SetRingCategories(richCat);
  run->AddTask(trackingQa);

  CbmLitFitQa* fitQa = new CbmLitFitQa();
  fitQa->SetMvdMinNofHits(0);
  fitQa->SetStsMinNofHits(6);
  fitQa->SetMuchMinNofHits(10);
  fitQa->SetTrdMinNofHits(2);
  fitQa->SetOutputDir(resultDir);
  //run->AddTask(fitQa);

  CbmLitClusteringQa* clusteringQa = new CbmLitClusteringQa();
  clusteringQa->SetOutputDir(resultDir);
  //run->AddTask(clusteringQa);

  CbmLitTofQa* tofQa = new CbmLitTofQa();
  tofQa->SetOutputDir(std::string(resultDir));
  //run->AddTask(tofQa);


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
  std::cout << "Output file is " << qaFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
