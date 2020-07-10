void mrich_reco(
  const string srcfolder = "/lustre/cbm/users/adrian/mcbmbeamtime/cbmsource/"
                           "macro/beamtime/mcbm2020/data",
  const unsigned int runId = 759,  // used for the output folder
  int nEvents              = 1000,
  const int taskId         = -1) {
  const string& parFile =
    Form("%s/unp_mcbm_params_%d.root", srcfolder.c_str(), runId);
  const string& digiFile =
    Form("%s/unp_mcbm_%d.root", srcfolder.c_str(), runId);
  const string& recoFile = Form("reco_mcbm_mar20_%d.root", runId);

  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {1};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {10};
  const Int_t eb_TriggerMinNumberRich {5};

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName  = "mrich_reco";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());


  std::cout << std::endl
            << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  // -----   Load the geometry setup   -------------------------------------
  TString geoDir      = workDir;  // gSystem->Getenv("VMCWORKDIR");
  TString geoFile     = srcDir + "/macro/mcbm/data/mcbm_beam_2020_03.geo.root";
  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (NULL == geoMan) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }
  // -----------------------------------------------------------------------


  // -----   FairRunAna   -------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(kTRUE);
  // -----------------------------------------------------------------------


  // -----   Cbm EventBuilder   --------------------------------------------
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

  run->AddTask(eventBuilder);
  // -----------------------------------------------------------------------


  // -----   Local reconstruction of RICH Hits -----------------------------
  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  run->AddTask(hitProd);
  // -----------------------------------------------------------------------


  // -----   Local reconstruction in RICh -> Finding of Rings --------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  // -----  DO your QA -> For rich-Tof  I use CbmRichMCbmQaReal       ------
  // -----                For rich Only I use CbmRichMCbmQaRichOnly   ------
  // -----------------------------------------------------------------------
  /*CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
    if (taskId < 0) {
        qaTask->SetOutputDir(Form("result_run%d",runId));
    } else {
        qaTask->SetOutputDir(Form("result_run%d_%05d",runId,taskId));
    }
    //    qaTask->DoRestrictToAcc();//restrict to mRICH MAR2019 in histFilling
    //    qaTask->XOffsetHistos(12);
    qaTask->XOffsetHistos(-2.5);
    run->AddTask(qaTask);*/
  // -----------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl
            << std::endl
            << "-I- " << myName << ": Set runtime DB" << std::endl;

  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  Bool_t kParameterMerged    = kTRUE;
  FairParRootFileIo* parIo1  = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  parIo1->print();

  parIo2->open(parFileList, "in");
  parIo2->print();
  rtdb->setFirstInput(parIo2);
  rtdb->setSecondInput(parIo1);

  rtdb->print();
  rtdb->printParamContexts();

  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  //rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // -----------------------------------------------------------------------


  //--- House Keeping -------------------------------------------------------
  // print all important infos in a file
  std::ofstream outfile;
  if (taskId < 0) {
    outfile.open(Form("result_run%d/run_info.dat", runId));
  } else {
    outfile.open(Form("result_run%d_%05d/run_info.dat", runId, taskId));
  }
  // write inputted data into the file.
  outfile << "Run: " << runId << std::endl;
  outfile << "Events: " << nEvents << std::endl;
  outfile << "parFile: " << parFile << std::endl;
  outfile << "digiFile: " << digiFile << std::endl;
  outfile << "recoFile: " << recoFile << std::endl;
  outfile << "Geometry: " << geoFile << std::endl;
  outfile << "Trigger:" << std::endl;
  outfile << "  fixedTimeWindow :" << eb_fixedTimeWindow << std::endl;
  outfile << "  MinNumberT0   :" << eb_TriggerMinNumberT0 << std::endl;
  outfile << "  MinNumberSts  :" << eb_TriggerMinNumberSts << std::endl;
  outfile << "  MinNumberMuch :" << eb_TriggerMinNumberMuch << std::endl;
  outfile << "  MinNumberTof  :" << eb_TriggerMinNumberTof << std::endl;
  outfile << "  MinNumberRich :" << eb_TriggerMinNumberRich << std::endl;
  outfile.close();
  // -----------------------------------------------------------------------


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
