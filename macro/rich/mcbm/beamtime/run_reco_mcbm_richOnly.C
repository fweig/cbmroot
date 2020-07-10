
void run_reco_mcbm_richOnly(
  const string srcfolder =
    "/lustre/cbm/users/adrian/cbmgitnew/cbmsource/macro/beamtime/mcbm2020/data",
  const unsigned int runId = 790,  // used for the output folder
  int nEvents              = 0,
  const int taskId         = -1) {
  const string& parFile =
    Form("%s/unp_mcbm_params_%d.root", srcfolder.c_str(), runId);
  const string& digiFile =
    Form("%s/unp_mcbm_%d.root", srcfolder.c_str(), runId);
  const string& recoFile = Form("reco_mcbm_mar20_%d.root", runId);

  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {0};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {0};
  const Int_t eb_TriggerMinNumberRich {5};

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName  = "run_reco_mcbm_reichOnly";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  //    TString setupFile = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  //    TString setupFunct = "setup_" + geoSetup + "()";
  //    gROOT->LoadMacro(setupFile);
  //    gROOT->ProcessLine(setupFunct);

  std::cout << std::endl
            << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TString geoFile     = srcDir + "/macro/mcbm/data/mcbm_beam_2020_03.geo.root";
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

  CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  //  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
  //  eventBuilder->SetMaximumTimeGap(100.);
  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  eventBuilder->SetFixedTimeWindow(eb_fixedTimeWindow);
  eventBuilder->SetTriggerMinNumberT0(eb_TriggerMinNumberT0);
  eventBuilder->SetTriggerMinNumberSts(eb_TriggerMinNumberSts);
  eventBuilder->SetTriggerMinNumberMuch(eb_TriggerMinNumberMuch);
  eventBuilder->SetTriggerMinNumberTof(eb_TriggerMinNumberTof);
  eventBuilder->SetTriggerMinNumberRich(eb_TriggerMinNumberRich);
  eventBuilder->SetFillHistos(kFALSE);  // to prevent memory leak???


  run->AddTask(eventBuilder);


  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  run->AddTask(hitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  ;

  CbmRichMCbmQaRichOnly* qaTask = new CbmRichMCbmQaRichOnly();
  if (taskId < 0) {
    qaTask->SetOutputDir(Form("result_richOnly_run%d", runId));
  } else {
    qaTask->SetOutputDir(Form("result_richOnly_run%d_%05d", runId, taskId));
  }
  //    qaTask->DoRestrictToAcc();//restrict to mRICH MAR2019 in histFilling
  qaTask->XOffsetHistos(+2.5);
  qaTask->SetMaxNofDrawnEvents(100);
  //qaTask->SetTotRich(23.7,30.0);
  qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  //qaTask->DoWriteHistToFile(false);
  run->AddTask(qaTask);

  std::cout << std::endl
            << std::endl
            << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  rtdb->setFirstInput(parIo1);
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
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time "
            << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
