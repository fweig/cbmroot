
void run_reco_mcbm_real(const string& parFile  = "/lustre/nyx/cbm/users/adrian/data/159/10kTS/unp_mcbm_params_159.root",
                        const string& digiFile = "/lustre/nyx/cbm/users/adrian/data/159/10kTS/unp_mcbm_159.root",
                        const string& recoFile = "reco_mcbm.root", int nEvents = 10)
{
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName = "run_reco_mcbm_real";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  remove(recoFile.c_str());

  //    TString setupFile = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  //    TString setupFunct = "setup_" + geoSetup + "()";
  //    gROOT->LoadMacro(setupFile);
  //    gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

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
  eventBuilder->SetFixedTimeWindow(200.);
  eventBuilder->SetTriggerMinNumberT0(0);
  eventBuilder->SetTriggerMinNumberSts(0);
  eventBuilder->SetTriggerMinNumberMuch(0);
  eventBuilder->SetTriggerMinNumberTof(10);
  eventBuilder->SetTriggerMinNumberRich(1);

  run->AddTask(eventBuilder);

  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  run->AddTask(hitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);

  CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
  run->AddTask(qaTask);

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
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
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
