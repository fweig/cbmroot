/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void run_aerogel_analysis(const string& parFile = "/lustre/cbm/users/adrian/cbmgitnew/cbmsource/macro/"
                                                  "beamtime/mcbm2020/data/unp_mcbm_params_598.root",
                          const string& digiFile = "/lustre/cbm/users/adrian/cbmgitnew/cbmsource/macro/"
                                                   "beamtime/mcbm2020/data/unp_mcbm_598.root",
                          const string& recoFile = "reco_aerogel_mcbm.root", int nEvents = 1000)
{
  const unsigned int runId = 598;

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName  = "run_reco_mcbm_real";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  //TString TofFileFolder = Form("/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/%s",cFileId.Data());

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  TString geoDir      = workDir;
  TString geoFile     = srcDir + "/macro/mcbm/data/mcbm_beam_2020_03.geo.root";
  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (NULL == geoMan) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }

  //-----------------------------------------------//

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  run->SetGenerateRunInfo(kTRUE);


  /// Add the Eventbuilder

  CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  //  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
  //  eventBuilder->SetMaximumTimeGap(100.);
  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  eventBuilder->SetFixedTimeWindow(200.);
  eventBuilder->SetTriggerMinNumberT0(1);
  eventBuilder->SetTriggerMinNumberSts(0);
  eventBuilder->SetTriggerMinNumberMuch(0);
  eventBuilder->SetTriggerMinNumberTof(10);
  eventBuilder->SetTriggerMinNumberRich(10);

  run->AddTask(eventBuilder);

  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  //hitProd->DoRestrictToAerogelAccDec2019();
  run->AddTask(hitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);


  //######################################################################

  CbmRichMCbmAerogelAna* qaTask = new CbmRichMCbmAerogelAna;
  qaTask->SetOutputDir(Form("result_run%d_Aerogel", runId));
  //qaTask->DoRestrictToAcc();//restrict to mRICH MAR2019 in histFilling
  qaTask->XOffsetHistos(-2.5);
  run->AddTask(qaTask);


  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;

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


  //  rtdb->setOutput(parIo1);
  rtdb->saveOutput();


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
