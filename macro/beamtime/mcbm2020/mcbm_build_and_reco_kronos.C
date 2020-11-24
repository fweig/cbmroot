// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2020)
// Combined reconstruction (Event building + cluster + hit finder) for different subsystems.
//
// --------------------------------------------------------------------------


void mcbm_build_and_reco_kronos(UInt_t uRunIdx = 28,
                                Int_t nEvents  = 300,
                                TString outDir = "data/") {
  UInt_t uRunId    = 0;
  TString fileName = "data/unp_mcbm_0.root";
  if (99999 != uRunIdx) {
    std::vector<UInt_t> vuListRunId = {
      692, 698, 702, 704, 705, 706, 707,            //  7 =>  0 -  6
      744, 750, 759, 760, 761, 762, 799,            //  7 =>  7 - 13
      811, 812, 816, 817, 819,                      //  5 => 14 - 18
      820, 821, 822, 824, 826, 827, 828, 829,       //  8 => 19 - 26
      830, 831, 836,                                //  3 => 27 - 29
      841, 846, 849,                                //  3 => 30 - 32
      850, 851, 852, 854, 855, 856, 857, 858, 859,  //  9 => 33 - 41
      860, 861, 862, 863, 864, 865, 866             //  7 => 42 - 48
                                                    /*
 /// With runs < 1 min due to missmatch!
             811, 812, 816, 817, 818, 819,                     //  6 => 14 - 19
             820, 821, 822, 824, 826, 827, 828, 829,           //  8 => 20 - 27
             830, 831, 836, 839,                               //  4 => 28 - 31
             840, 841, 842, 844, 845, 846, 848, 849,           //  8 => 32 - 39
             850, 851, 852, 854, 855, 856, 857, 858, 859,      //  9 => 40 - 48
             860, 861, 862, 863, 864, 865, 866                 //  7 => 49 - 55
*/
    };
    if (vuListRunId.size() <= uRunIdx) return kFALSE;
    uRunId = vuListRunId[uRunIdx];
    //fileName = Form("data/unp_mcbm_%03u.root", uRunId);
    fileName = Form("/lustre/cbm/users/ploizeau/mcbm2020/"
                    "unp_evt_data_7f229b3f_20201103/unp_mcbm_%i.root",
                    uRunId);
  }  // if( 99999 != uRunIdx )

  if (uRunId < 692 && 0 != uRunId) return kFALSE;


  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  //  Int_t iVerbose = 1;

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // MC file

  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = srcDir + "/macro/beamtime/mcbm2020/";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
  //  FairRunOnline *fRun= new FairRunOnline();
  FairRunAna* fRun = new FairRunAna();
  fRun->SetEventHeaderPersistence(kFALSE);

  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);

  TString runId                = TString::Format("%03u", uRunId);
  TString outFile              = outDir + "/events_win_" + runId + ".root";
  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  fRun->SetSink(outputSink);

  // Define output file for FairMonitor histograms
  //  TString monitorFile{outFile};
  //  monitorFile.ReplaceAll("qa","qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  //  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

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

  fRun->AddTask(eventBuilder);

  // -----  Parameter database   --------------------------------------------
  
  TString parFileIn =
    Form("/lustre/cbm/users/ploizeau/mcbm2020/unp_evt_data_7f229b3f_20201103/"
         "unp_mcbm_params_%i.root",
         uRunId);
  TString parFileOut = Form("reco_mcbm_params_%i.root", uRunId);


  FairRuntimeDb* rtdb       = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParRootFileIo* parIo3  = new FairParRootFileIo();
  parIo1->open(parFileIn.Data(), "READ");
  parIo3->open(parFileOut.Data(), "RECREATE");
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo3);

  //----------------------------------Reconstruction-------------------------------------


  
  // ------------------------------------------------------------------------
  TString geoFileSts = paramDir + "mcbm2020_reco.geo.root";  
  //TString geoFileSts =
  //  "/lustre/cbm/users/alberica/cbmroot/macro/beamtime/mcbm2020/data/test.geo.root";  // to be created by a simulation run
  fRun->SetGeomFile(geoFileSts);

  // -----   Local reconstruction in STS   ----------------------------------
  CbmRecoSts* recoSts = new CbmRecoSts();
  //  recoSts->SetMode(kCbmRecoEvent);

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

  fRun->AddTask(recoSts);
  std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  Int_t flag = 1;
  TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
  TString muchDigiFile(parDir + "/much/much_v19c_mcbm_digi_sector.root"); // MUCH digi file
  CbmMuchFindHitsGem* muchFindHits = new CbmMuchFindHitsGem(muchDigiFile.Data(), flag);
  muchFindHits->SetBeamTimeDigi(kTRUE);
  fRun->AddTask(muchFindHits);
  std::cout << "-I- : Added task " << muchFindHits->GetName() << std::endl;

  //--------------------------------------------------------



  // -----   Intialise and run   --------------------------------------------
  fRun->Init();

  //  rtdb->setOutput(parIo1);
  //  rtdb->saveOutput();
  //  rtdb->print();

  cout << "Starting run" << endl;
  if (0 == nEvents) {
    fRun->Run(0, 0);  // run until end of input file
  } else {
    fRun->Run(0, nEvents);  // process  N Events
  }
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  cout << maxMemory;
  cout << "</DartMeasurement>" << endl;

  Float_t cpuUsage = ctime / rtime;
  cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  cout << cpuUsage;
  cout << "</DartMeasurement>" << endl;

  FairMonitor* tempMon = FairMonitor::GetMonitor();
  tempMon->Print();

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
