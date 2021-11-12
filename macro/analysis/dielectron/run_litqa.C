/* Copyright (C) 2014-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Semen Lebedev, Elena Lebedeva [committer] */

// Run this macro with run_local.py for local test and with batch_send(job).py for large productions
void run_litqa(const std::string& traFile, const std::string& parFile, const std::string& digiFile,
               const std::string& recoFile, const std::string& qaFile, const std::string& geoSetup, int nEvents)
{
  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  remove(qaFile.c_str());

  CbmSetup::Instance()->LoadSetup(geoSetup.c_str());

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
  inputSource->AddFriend(traFile.c_str());
  inputSource->AddFriend(recoFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(qaFile.c_str());
  run->SetGenerateRunInfo(kFALSE);

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile.c_str());
  run->AddTask(mcManager);

  // RICH reco QA
  CbmRichRecoQa* richRecoQa = new CbmRichRecoQa();
  richRecoQa->SetOutputDir("");
  //run->AddTask(richRecoQa);

  // Reconstruction Qa
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
  trackingQa->SetOutputDir("");
  trackingQa->SetPRange(12, 0., 6.);
  //trackingQa->SetTrdAnnCut(trdAnnCut);  // removed comment
  std::vector<std::string> trackCat, richCat;
  trackCat.push_back("All");
  trackCat.push_back("Electron");
  richCat.push_back("Electron");
  richCat.push_back("ElectronReference");
  trackingQa->SetTrackCategories(trackCat);
  trackingQa->SetRingCategories(richCat);
  run->AddTask(trackingQa);

  CbmLitFitQa* fitQa = new CbmLitFitQa();
  fitQa->SetMvdMinNofHits(3);
  fitQa->SetStsMinNofHits(6);
  fitQa->SetMuchMinNofHits(10);
  fitQa->SetTrdMinNofHits(2);
  fitQa->SetOutputDir("");
  // run->AddTask(fitQa);

  CbmLitClusteringQa* clusteringQa = new CbmLitClusteringQa();
  clusteringQa->SetOutputDir("");
  run->AddTask(clusteringQa);

  CbmLitTofQa* tofQa = new CbmLitTofQa();
  tofQa->SetOutputDir(std::string(""));
  // run->AddTask(tofQa);

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
  std::cout << "Qa file is " << qaFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
