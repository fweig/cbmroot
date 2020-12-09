// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2020)
// Combined event based reconstruction (Event building + cluster + hit finder)
// for different subsystems.
//
// --------------------------------------------------------------------------


void mcbm_build_and_reco(UInt_t uRunId     = 28,
                         Int_t nTimeslices = 300,
                         TString outDir    = "data/") {
  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName   = "mcbm_reco";  // this macro's name for screen output
  TString srcDir   = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString paramDir = srcDir + "/macro/beamtime/mcbm2020/";
  //    ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString inFile    = Form("./data/unp_mcbm_%i.root", uRunId);
  TString parFileIn = Form("./data/unp_mcbm_params_%i.root", uRunId);
  TString geoFile = paramDir + "mcbm2020_reco.geo.root";  // Created in sim. run
  TString parFileOut = Form("./data/reco_mcbm_evt_win_params_%u.root", uRunId);
  TString outFile    = Form("./data/reco_mcbm_evt_win_%u.root", uRunId);
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetEventHeaderPersistence(kFALSE);

  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);

  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  run->SetSink(outputSink);
  run->SetGeomFile(geoFile);

  // Define output file for FairMonitor histograms
  TString monitorFile {outFile};
  monitorFile.ReplaceAll("rec", "rec.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  //-------- Event builder --------------------------------------------------
  //  CbmMcbm2019TimeWinEventBuilder* eventBuilder = new CbmMcbm2019TimeWinEventBuilder();
  CbmMcbm2019TimeWinEventBuilderTask* eventBuilder =
    new CbmMcbm2019TimeWinEventBuilderTask();

  eventBuilder->SetFillHistos(kTRUE);

  eventBuilder->SetEventOverlapMode(EOverlapMode::NoOverlap);
  //  eventBuilder->SetEventOverlapMode(EOverlapMode::MergeOverlap);
  //  eventBuilder->SetEventOverlapMode(EOverlapMode::AllowOverlap);

  /*
 * Available Pre-defined detectors:
 * kEventBuilderDetSts
 * kEventBuilderDetMuch
 * kEventBuilderDetTrd
 * kEventBuilderDetTof
 * kEventBuilderDetRich
 * kEventBuilderDetPsd
 * kEventBuilderDetT0
 */

  /// Change the selection window limits for T0 as ref
  eventBuilder->SetTriggerWindow(ECbmModuleId::kSts, -50, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kMuch, -150, 50);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTrd, -250, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTof, -150, 10);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kRich, -50, 50);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kPsd, -50, 50);
  /// To get T0 Digis (seed + close digis) in the event
  eventBuilder->SetTriggerWindow(ECbmModuleId::kT0, -1, 10);

  /*
  /// Use TOF as reference
  eventBuilder->SetReferenceDetector( kEventBuilderDetTof );
  eventBuilder->AddDetector( kEventBuilderDetT0 );

  /// Change the selection window limits for TOF as ref
  /// => Should always be after changes of detector lists!
  eventBuilder->SetTriggerWindow(ECbmModuleId::kT0, -150, 0);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kSts, -50, 100);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kMuch, -50, 200);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTrd, -50, 300);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kTof, 0, 60);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kRich, -100, 150);
  eventBuilder->SetTriggerWindow(ECbmModuleId::kPsd, -200, 50);
*/

  /// Change the trigger requirements
  /// => Should always be after changes of detector lists!
  /// --- Minimum
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kT0, 1);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kSts, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kMuch, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kTrd, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kTof, 10);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kRich, 0);
  eventBuilder->SetTriggerMinNumber(ECbmModuleId::kPsd, 0);
  /// --- Maximum  (-1 to disable cut)
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kT0, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kMuch, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kTrd, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);
  eventBuilder->SetTriggerMaxNumber(ECbmModuleId::kPsd, -1);


  if (0 < uRunId)
    eventBuilder->SetOutFilename(
      Form("%sHistosEvtWin_%03u.root", outDir.Data(), uRunId));

  run->AddTask(eventBuilder);
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------

  // -----   Local reconstruction in STS   ----------------------------------
  CbmRecoSts* recoSts = new CbmRecoSts();
  recoSts->SetMode(kCbmRecoEvent);

  //recoSts->SetTimeCutDigisAbs( 20 );// cluster finder: time cut in ns
  //recoSts->SetTimeCutClustersAbs(20.); // hit finder: time cut in ns

  // ASIC params: #ADC channels, dyn. range, threshold, time resol., dead time,
  // noise RMS, zero-threshold crossing rate
  auto parAsic =
    new CbmStsParAsic(32, 75000., 3000., 5., 800., 1000., 3.9789e-3);

  // Module params: number of channels, number of channels per ASIC
  auto parMod = new CbmStsParModule(2048, 128);
  parMod->SetAllAsics(*parAsic);
  recoSts->UseModulePar(parMod);

  // Sensor params
  auto sensorPar = new CbmStsParSensor(CbmStsSensorClass::kDssdStereo);
  sensorPar->SetPar(0, 6.2092);  // Extension in x
  sensorPar->SetPar(1, 6.2);     // Extension in y
  sensorPar->SetPar(2, 0.03);    // Extension in z
  sensorPar->SetPar(3, 5.9692);  // Active size in y
  sensorPar->SetPar(4, 1024.);   // Number of strips front side
  sensorPar->SetPar(5, 1024.);   // Number of strips back side
  sensorPar->SetPar(6, 0.0058);  // Strip pitch front side
  sensorPar->SetPar(7, 0.0058);  // Strip pitch back side
  sensorPar->SetPar(8, 7.5);     // Stereo angle front side
  sensorPar->SetPar(9, 0.0);     // Stereo angle back side
  recoSts->UseSensorPar(sensorPar);

  // Sensor conditions: full depletion voltage, bias voltage, temperature,
  // coupling capacitance, inter-strip capacitance
  auto sensorCond = new CbmStsParSensorCond(70., 140., 268., 17.5, 1.);
  recoSts->UseSensorCond(sensorCond);

  run->AddTask(recoSts);
  std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  Int_t flag = 1;
  TString parDir =
    TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
  TString muchDigiFile(
    parDir + "/much/much_v19c_mcbm_digi_sector.root");  // MUCH digi file
  CbmMuchFindHitsGem* muchFindHits =
    new CbmMuchFindHitsGem(muchDigiFile.Data(), flag);
  muchFindHits->SetBeamTimeDigi(kTRUE);
  run->AddTask(muchFindHits);
  std::cout << "-I- : Added task " << muchFindHits->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TRD   ----------------------------------
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  // ------------------------------------------------------------------------


  // -----   Local reconstruction of RICH Hits ------------------------------
  CbmRichMCbmHitProducer* hitProdRich = new CbmRichMCbmHitProducer();
  hitProdRich->setToTLimits(23.7, 30.0);
  hitProdRich->applyToTCut();
  TString sRichMapFile =
    srcDir + "/macro/rich/mcbm/beamtime/mRICH_Mapping_vert_20190318_elView.geo";
  hitProdRich->SetMappingFile(sRichMapFile.Data());
  run->AddTask(hitProdRich);
  // ------------------------------------------------------------------------

  // -----   Local reconstruction in RICh -> Finding of Rings ---------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  // ------------------------------------------------------------------------


  // -----  Psd hit producer   ----------------------------------------------
  CbmPsdMCbmHitProducer* hitProdPsd = new CbmPsdMCbmHitProducer();
  run->AddTask(hitProdPsd);
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  FairParRootFileIo* parIo3  = new FairParRootFileIo();
  parIo1->open(parFileIn.Data(), "READ");
  parIo3->open(parFileOut.Data(), "RECREATE");
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo3);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nTimeslices);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFileOut << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl;
  std::cout << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
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
  // ------------------------------------------------------------------------


  // -----   Function needed for CTest runtime dependency   -----------------
  //  RemoveGeoManager();
  // ------------------------------------------------------------------------

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}