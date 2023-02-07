/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov, Frederic Linz [committer] */

// -----   Check output file name   -------------------------------------------
bool CheckOutFileName(TString fileName, Bool_t overwrite)
{
  string fName = "run_reco_json_config";
  // --- Protect against overwriting an existing file
  if ((!gSystem->AccessPathName(fileName.Data())) && (!overwrite)) {
    cout << fName << ": output file " << fileName << " already exists!";
    return false;
  }

  // --- If the directory does not yet exist, create it
  const char* directory = gSystem->DirName(fileName.Data());
  if (gSystem->AccessPathName(directory)) {
    Int_t success = gSystem->mkdir(directory, kTRUE);
    if (success == -1) {
      cout << fName << ": output directory " << directory << " does not exist and cannot be created!";
      return false;
    }
    else
      cout << fName << ": created directory " << directory;
  }
  return true;
}

void run_analysis_tree_maker_json_config(TString traPath = "test", TString rawPath = "", TString recPath = "",
                                         TString unigenFile = "", TString outPath = "", bool overwrite = true,
                                         std::string config = "", int nEvents = 0)
{
  const std::string system = "Au+Au";  // TODO can we read it automatically?
  const float beam_mom     = 12.;
  const bool is_event_base = false;

  // --- Logger settings ----------------------------------------------------
  const TString logLevel     = "INFO";
  const TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  const TString myName = "run_analysis_tree_maker";
  const TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  if (rawPath == "") rawPath = traPath;
  if (recPath == "") recPath = traPath;
  if (outPath == "") outPath = traPath;
  TString traFile = traPath + ".tra.root";
  TString geoFile = traPath + ".geo.root";
  TString rawFile = rawPath + ".raw.root";
  TString recFile = recPath + ".reco.root";
  TString parFile = rawPath + ".par.root";
  TString outFile = outPath + ".analysistree.root";
  // ------------------------------------------------------------------------

  // -----   Remove old CTest runtime dependency file  ----------------------
  const TString dataDir  = gSystem->DirName(outPath);
  const TString dataName = gSystem->BaseName(outPath);
  const TString testName = ("run_treemaker");
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  CbmSetup* setup = CbmSetup::Instance();
  if (config == "") config = Form("%s/macro/run/config.json", gSystem->Getenv("VMCWORKDIR"));
  boost::property_tree::ptree pt;
  CbmTransportConfig::LoadFromFile(config, pt);
  CbmTransportConfig::SetGeometry(setup, pt.get_child(CbmTransportConfig::GetModuleTag()));
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  TString geoTag;
  auto* parFileList = new TList();

  if (!CheckOutFileName(outFile, overwrite)) return;
  std::cout << "-I- " << myName << ": Using raw file " << rawFile << std::endl;
  std::cout << "-I- " << myName << ": Using parameter file " << parFile << std::endl;
  std::cout << "-I- " << myName << ": Using reco file " << recFile << std::endl;
  if (unigenFile.Length() > 0) std::cout << "-I- " << myName << ": Using unigen file " << unigenFile << std::endl;

  // -----   Reconstruction run   -------------------------------------------
  auto* run         = new FairRunAna();
  auto* inputSource = new FairFileSource(recFile);
  inputSource->AddFriend(traFile);
  inputSource->AddFriend(rawFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  // ------------------------------------------------------------------------

  // ----- Mc Data Manager   ------------------------------------------------
  auto* mcManager = new CbmMCDataManager("MCManager", is_event_base);
  mcManager->AddFile(traFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // ---   STS track matching   ----------------------------------------------
  //  auto* matchTask = new CbmMatchRecoToMC();
  //  run->AddTask(matchTask);
  // ------------------------------------------------------------------------

  run->AddTask(new CbmTrackingDetectorInterfaceInit());

  auto* KF = new CbmKF();
  run->AddTask(KF);
  // needed for tracks extrapolation
  auto* l1 = new CbmL1("CbmL1", 1, 3);
  if (setup->IsActive(ECbmModuleId::kMvd)) {
    setup->GetGeoTag(ECbmModuleId::kMvd, geoTag);
    const TString mvdMatBudgetFileName = srcDir + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if (setup->IsActive(ECbmModuleId::kSts)) {
    setup->GetGeoTag(ECbmModuleId::kSts, geoTag);
    const TString stsMatBudgetFileName = srcDir + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);

  // --- TRD pid tasks
  if (setup->IsActive(ECbmModuleId::kTrd)) {
    CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
    trdLI->SetUseMCInfo(kTRUE);
    trdLI->SetUseMomDependence(kTRUE);
    run->AddTask(trdLI);
    std::cout << "-I- : Added task " << trdLI->GetName() << std::endl;
    //     ------------------------------------------------------------------------
  }

  // AnalysisTree converter
  auto* man = new CbmConverterManager();
  man->SetSystem(system);
  man->SetBeamMomentum(beam_mom);

  man->SetOutputName(outFile.Data(), "rTree");

  if (!is_event_base) { man->AddTask(new CbmMatchEvents()); }

  man->AddTask(new CbmSimEventHeaderConverter("SimEventHeader"));
  man->AddTask(new CbmRecEventHeaderConverter("RecEventHeader"));
  man->AddTask(new CbmSimTracksConverter("SimParticles"));

  CbmStsTracksConverter* taskCbmStsTracksConverter = new CbmStsTracksConverter("VtxTracks", "SimParticles");
  taskCbmStsTracksConverter->SetIsWriteKFInfo();
  taskCbmStsTracksConverter->SetIsReproduceCbmKFPF();
  man->AddTask(taskCbmStsTracksConverter);

  man->AddTask(new CbmRichRingsConverter("RichRings", "VtxTracks"));
  man->AddTask(new CbmTofHitsConverter("TofHits", "VtxTracks"));
  man->AddTask(new CbmTrdTracksConverter("TrdTracks", "VtxTracks"));
  man->AddTask(new CbmPsdModulesConverter("PsdModules"));

  run->AddTask(man);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  auto* parIo1        = new FairParRootFileIo();
  auto* parIo2        = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  // -----   Intialise and run   --------------------------------------------
  run->Init();

  std::cout << "Starting run" << std::endl;
  run->Run(nEvents);
  // ------------------------------------------------------------------------

  timer.Stop();
  const Double_t rtime = timer.RealTime();
  const Double_t ctime = timer.CpuTime();
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;

  printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);

  // -----   CTest resource monitoring   ------------------------------------
  FairSystemInfo sysInfo;
  const Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << R"(<DartMeasurement name="MaxMemory" type="numeric/double">)";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  std::cout << R"(<DartMeasurement name="WallTime" type="numeric/double">)";
  std::cout << rtime;
  std::cout << "</DartMeasurement>" << std::endl;
  const Float_t cpuUsage = ctime / rtime;
  std::cout << R"(<DartMeasurement name="CpuLoad" type="numeric/double">)";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  //   Generate_CTest_Dependency_File(depFile);
  // ------------------------------------------------------------------------

  // -----   This is to prevent a malloc error when exiting ROOT   ----------
  // The source of the error is unknown. Related to TGeoManager.
  RemoveGeoManager();
  // ------------------------------------------------------------------------
}
