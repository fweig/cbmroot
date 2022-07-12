/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void run_reco_mcbm_richOnly2022(const string srcfolder   = "/data/cbmroot/cbmsource/macro/run/data/",
                                const unsigned int runId = 2160,  // used for the output folder
                                int nEvents = 100, const int taskId = 9)
{
  const string& digiFile = Form("%s/%u.digi.root", srcfolder.c_str(), runId);
  const string& recoFile = Form("reco_mcbm_mar22_%d.root", runId);

  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {0};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {0};
  const Int_t eb_TriggerMinNumberRich {10};

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName  = "run_reco_mcbm_richOnly2022";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  //    TString setupFile = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  //    TString setupFunct = "setup_" + geoSetup + "()";
  //    gROOT->LoadMacro(setupFile);
  //    gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TString geoFile = srcDir + "/macro/mcbm/data/mcbm_beam_2022_03_22_iron.geo.root";

  if (runId >= 2352) geoFile = srcDir + "/macro/mcbm/data/mcbm_beam_2022_05_23_nickel.geo.root";

  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (geoMan == nullptr) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(kTRUE);

  // CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  // //  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
  // //  eventBuilder->SetMaximumTimeGap(100.);
  // eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  // eventBuilder->SetFixedTimeWindow(eb_fixedTimeWindow);
  // eventBuilder->SetTriggerMinNumberT0(eb_TriggerMinNumberT0);
  // eventBuilder->SetTriggerMinNumberSts(eb_TriggerMinNumberSts);
  // eventBuilder->SetTriggerMinNumberMuch(eb_TriggerMinNumberMuch);
  // eventBuilder->SetTriggerMinNumberTof(eb_TriggerMinNumberTof);
  // eventBuilder->SetTriggerMinNumberRich(eb_TriggerMinNumberRich);
  // eventBuilder->SetFillHistos(kFALSE);  // to prevent memory leak???


  // run->AddTask(eventBuilder);

  CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

  //Choose between NoOverlap, MergeOverlap, AllowOverlap
  evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

  // Remove detectors where digis not found
  //if (!geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetT0);
  // Set TOF as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetRich);
  //evBuildRaw->SetReferenceDetector(kRawEventBuilderDetT0);

  //evBuildRaw->AddDetector(kRawEventBuilderDetT0);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  evBuildRaw->SetTsParameters(0.0, 1.28e8, 0.0);

  //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);

  // evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0, eb_TriggerMinNumberT0);
  // evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0, -1);

  //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);

  //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, eb_TriggerMinNumberSts);
  //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich, eb_TriggerMinNumberRich);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);

  //evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0, -100, 100);
  //if (geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, -50, 50);
  //if (geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -50, 50);
  //if (geoSetup->IsActive(ECbmModuleId::kTrd)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd, -200, 200);
  //if (geoSetup->IsActive(ECbmModuleId::kTrd2d)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd2d, -200, 200);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich, -50, 100);

  run->AddTask(evBuildRaw);

  //---------------------------------------------------------------


  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  hitProd->applyICDCorrection();
  run->AddTask(hitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  ;

  CbmRichMCbmQaRichOnly* qaTask = new CbmRichMCbmQaRichOnly();
  if (taskId < 0) { qaTask->SetOutputDir(Form("result_richOnly_run%d", runId)); }
  else {
    qaTask->SetOutputDir(Form("result_richOnly_run%d_%05d", runId, taskId));
  }
  //    qaTask->DoRestrictToAcc();//restrict to mRICH MAR2019 in histFilling
  qaTask->XOffsetHistos(+5.5);
  qaTask->SetMaxNofDrawnEvents(100);
  qaTask->SetTotRich(23.7, 30.0);
  // qaTask->SetTotRich(0.0, 60.0);
  qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  // qaTask->DoWriteHistToFile(false);
  // qaTask->SetIcdGeneration(); // comment out to stop generation in ICD files.
  run->AddTask(qaTask);

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  //parIo1->open(parFile.c_str(), "UPDATE");
  //rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }


  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();


  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();


  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);


  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
