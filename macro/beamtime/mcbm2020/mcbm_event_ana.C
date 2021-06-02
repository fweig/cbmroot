/** @file mcbm_event_ana.C
  * @copyright Copyright (C) 2021 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer], Pierre-Alain Loizeau **/

// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2020)
// Combined Event based local reconstruction (Event Building (Florian one) +
// cluster + hit finder) for different subsystems.
//
// --------------------------------------------------------------------------

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
void mcbm_event_ana(UInt_t uRunId         = 831,
                    Int_t nTimeslices     = 1000,
                    Bool_t bUseEvtWin     = kFALSE,
                    TString sInpDir       = "./data",
                    TString sOutDir       = "./data",
                    Int_t iUnpFileIndex   = -1)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "WARN";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName   = "mcbm_event_ana";               // this macro's name for screen output
  TString srcDir   = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString paramDir = srcDir + "/macro/beamtime/mcbm2020/";
  TString parDir   = srcDir + "/parameters";
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  /// Standardized RUN ID
  TString sRunId = TString::Format("%03u", uRunId);
  /// Initial pattern
  TString inFile     = sInpDir + "/reco_mcbm_event_" + sRunId;
  TString trkFile    = sInpDir + "/tracking_mcbm_event_" + sRunId;
  TString parFileIn  = sInpDir + "/unp_mcbm_params_" + sRunId;
  TString parFileOut = sOutDir + "/mcbm_event_ana_params_" + sRunId;
  TString geoFile    = paramDir + "mcbm_beam_2020_03.geo.root";
  TString outFile    = sOutDir + "/mcbm_event_ana__" + sRunId;
  /// Initial pattern if using event builder with time window
  if (bUseEvtWin) {
    inFile     = sInpDir + "/reco_mcbm_evt_win_" + sRunId;
    trkFile    = sInpDir + "/tracking_mcbm_evt_win_" + sRunId;
    parFileOut = sOutDir + "/mcbm_event_ana_evt_win_params_" + sRunId;
    outFile    = sOutDir + "/mcbm_event_ana_evt_win_" + sRunId;
  }  // if( bUseEvtWin )
  /// Add index of splitting at unpacking level if needed
  if (0 <= iUnpFileIndex) {
    inFile += TString::Format("_%02u", iUnpFileIndex);
    trkFile += TString::Format("_%02u", iUnpFileIndex);
    // the input par file is from unpacking and not split during it!
    parFileOut += TString::Format("_%02u", iUnpFileIndex);
    outFile += TString::Format("_%02u", iUnpFileIndex);
  }  // if ( 0 <= iUnpFileIndex )
  /// Add ROOT file suffix
  inFile += ".root";
  trkFile += ".root";
  parFileIn += ".root";
  parFileOut += ".root";
  outFile += ".root";
  // ------------------------------------------------------------------------

  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  inputSource->AddFriend(trkFile);
  run->SetSource(inputSource);

  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  run->SetSink(outputSink);
  run->SetGeomFile(geoFile);

  // Define output file for FairMonitor histograms
  TString monitorFile {outFile};
  monitorFile.ReplaceAll("ana", "ana.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  //--------------------event builder-------------------//
  CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  // eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
  //eventBuilder->SetMaximumTimeGap(50.);
  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  eventBuilder->SetFixedTimeWindow(200.);
  eventBuilder->SetTriggerMinNumberT0(1);
  //eventBuilder->SetTriggerMinNumberSts(0);
  eventBuilder->SetTriggerMinNumberMuch(1);
  eventBuilder->SetTriggerMinNumberTof(10);
  //  run->AddTask(eventBuilder);
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------

  // -----   Local reconstruction in MUCH   ---------------------------------
  Int_t flag = 1;
  TString muchDigiFile(parDir + "/much/much_v19c_mcbm_digi_sector.root");  // MUCH digi file
  CbmMuchFindHitsGem* muchFindHits = new CbmMuchFindHitsGem(muchDigiFile.Data(), flag);
  muchFindHits->SetBeamTimeDigi(kTRUE);
  //  run->AddTask(muchFindHits);
  std::cout << "-I- : Added task " << muchFindHits->GetName() << std::endl;
  //-------------------------------------------------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  CbmRecoSts* recoSts = new CbmRecoSts();
  recoSts->SetMode(kCbmRecoEvent);

  //recoSts->SetTimeCutDigisAbs( 20 );// cluster finder: time cut in ns
  //recoSts->SetTimeCutClustersAbs(20.); // hit finder: time cut in ns

  // ASIC params: #ADC channels, dyn. range, threshold, time resol., dead time,
  // noise RMS, zero-threshold crossing rate
  auto parAsic = new CbmStsParAsic(128, 32, 75000., 3000., 5., 800., 1000., 3.9789e-3);

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

  //  run->AddTask(recoSts);
  //  std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TRD   ----------------------------------
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  // ------------------------------------------------------------------------
  // TOF defaults
  Int_t calMode      = 93;
  Int_t calSel       = 1;
  Int_t calSm        = 0;
  Int_t RefSel       = 0;
  Double_t dDeadtime = 50.;
  Int_t iSel2        = 500;
  TString TofGeoTag  = "v20f_mcbm";
  TString cCalId     = "831.50.3.0";
  Int_t iCalSet      = 12022500;  // calibration settings

  TObjString* tofBdfFile = new TObjString(parDir + "/tof/tof_" + TofGeoTag + ".digibdf.par");
  parFileList->Add(tofBdfFile);
  std::cout << "-I- Using parameter file " << tofBdfFile->GetString() << std::endl;

  CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer", 0, 1);
  TString cFname =
    parDir + "/tof/" + Form("%s_set%09d_%02d_%01dtofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSel);
  tofCluster->SetCalParFileName(cFname);
  tofCluster->SetCalMode(calMode);
  tofCluster->SetCalSel(calSel);
  tofCluster->PosYMaxScal(0.75);              //in % of 2*length
  tofCluster->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns

  //  run->AddTask(tofCluster);
  //  std::cout << "-I- Added task " << tofCluster->GetName() << std::endl;

  // -----   Track reconstruction   ------------------------------------------
  Int_t iTrackMode = 2;
  switch (iTrackMode) {
    case 2: {
      Int_t iGenCor        = 1;
      Double_t dScalFac    = 1.;
      Double_t dChi2Lim2   = 3.5;
      TString cTrkFile     = parDir + "/tof/" + Form("%s_tofFindTracks.hst.root", cCalId.Data());
      Int_t iTrackingSetup = 1;
      Int_t iCalOpt        = 0;

      CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
      tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
      tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);                 // max dev from mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);                 // mean slope dy/dz
      CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
      TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
      tofTrackFinder->SetFitter(tofTrackFitter);

      CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
      tofFindTracks->UseFinder(tofTrackFinder);
      tofFindTracks->UseFitter(tofTrackFitter);
      tofFindTracks->SetCalOpt(iCalOpt);
      // 1 - update offsets, 2 - update walk, 0 - bypass
      tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
      tofFindTracks->SetTtTarg(0.065);     // target value for Mar2020 triple stack -> betapeak ~ 0.95
      //tofFindTracks->SetTtTarg(0.041);  // target value for inverse velocity, > 0.033 ns/cm!
      //tofFindTracks->SetTtTarg(0.035);  // target value for inverse velocity, > 0.033 ns/cm!
      tofFindTracks->SetCalParFileName(cTrkFile);  // Tracker parameter value file name
      tofFindTracks->SetBeamCounter(5, 0, 0);      // default beam counter
      tofFindTracks->SetStationMaxHMul(30);        // Max Hit Multiplicity in any used station

      tofFindTracks->SetT0MAX(dScalFac);           // in ns
      tofFindTracks->SetSIGT(0.08);                // default in ns
      tofFindTracks->SetSIGX(0.3);                 // default in cm
      tofFindTracks->SetSIGY(0.45);                // default in cm
      tofFindTracks->SetSIGZ(0.05);                // default in cm
      tofFindTracks->SetUseSigCalib(kFALSE);       // ignore resolutions in CalPar file
      tofTrackFinder->SetSIGLIM(dChi2Lim2 * 2.);   // matching window in multiples of chi2
      tofTrackFinder->SetChiMaxAccept(dChi2Lim2);  // max tracklet chi2

      Int_t iMinNofHits   = -1;
      Int_t iNStations    = 0;
      Int_t iNReqStations = 3;
      switch (iTrackingSetup) {
        case 0:  // bypass mode
          iMinNofHits = -1;
          iNStations  = 1;
          tofFindTracks->SetStation(0, 5, 0, 0);  // Diamond
          break;

        case 1:  // for calibration mode of full setup
          iMinNofHits   = 3;
          iNStations    = 28;
          iNReqStations = 4;
          tofFindTracks->SetStation(0, 5, 0, 0);
          tofFindTracks->SetStation(1, 0, 2, 2);
          tofFindTracks->SetStation(2, 0, 1, 2);
          tofFindTracks->SetStation(3, 0, 0, 2);
          tofFindTracks->SetStation(4, 0, 2, 1);
          tofFindTracks->SetStation(5, 0, 1, 1);
          tofFindTracks->SetStation(6, 0, 0, 1);
          tofFindTracks->SetStation(7, 0, 2, 3);
          tofFindTracks->SetStation(8, 0, 1, 3);
          tofFindTracks->SetStation(9, 0, 0, 3);
          tofFindTracks->SetStation(10, 0, 2, 0);
          tofFindTracks->SetStation(11, 0, 1, 0);
          tofFindTracks->SetStation(12, 0, 0, 0);
          tofFindTracks->SetStation(13, 0, 2, 4);
          tofFindTracks->SetStation(14, 0, 1, 4);
          tofFindTracks->SetStation(15, 0, 0, 4);
          tofFindTracks->SetStation(16, 0, 4, 0);
          tofFindTracks->SetStation(17, 0, 3, 0);
          tofFindTracks->SetStation(18, 0, 4, 1);
          tofFindTracks->SetStation(19, 0, 3, 1);
          tofFindTracks->SetStation(20, 0, 4, 2);
          tofFindTracks->SetStation(21, 0, 3, 2);
          tofFindTracks->SetStation(22, 0, 4, 3);
          tofFindTracks->SetStation(23, 0, 3, 3);
          tofFindTracks->SetStation(24, 0, 4, 4);
          tofFindTracks->SetStation(25, 0, 3, 4);
          tofFindTracks->SetStation(26, 9, 0, 0);
          tofFindTracks->SetStation(27, 9, 0, 1);
          break;

        case 2:  // for geometry check mode of full setup
          iMinNofHits   = 3;
          iNStations    = 27;
          iNReqStations = 4;
          tofFindTracks->SetStation(0, 0, 2, 2);
          tofFindTracks->SetStation(1, 0, 1, 2);
          tofFindTracks->SetStation(2, 0, 0, 2);
          tofFindTracks->SetStation(3, 0, 2, 1);
          tofFindTracks->SetStation(4, 0, 1, 1);
          tofFindTracks->SetStation(5, 0, 0, 1);
          tofFindTracks->SetStation(6, 0, 2, 3);
          tofFindTracks->SetStation(7, 0, 1, 3);
          tofFindTracks->SetStation(8, 0, 0, 3);
          tofFindTracks->SetStation(9, 0, 2, 0);
          tofFindTracks->SetStation(10, 0, 1, 0);
          tofFindTracks->SetStation(11, 0, 0, 0);
          tofFindTracks->SetStation(12, 0, 2, 4);
          tofFindTracks->SetStation(13, 0, 1, 4);
          tofFindTracks->SetStation(14, 0, 0, 4);
          tofFindTracks->SetStation(15, 0, 4, 0);
          tofFindTracks->SetStation(16, 0, 3, 0);
          tofFindTracks->SetStation(17, 0, 4, 1);
          tofFindTracks->SetStation(18, 0, 3, 1);
          tofFindTracks->SetStation(19, 0, 4, 2);
          tofFindTracks->SetStation(20, 0, 3, 2);
          tofFindTracks->SetStation(21, 0, 4, 3);
          tofFindTracks->SetStation(22, 0, 3, 3);
          tofFindTracks->SetStation(23, 0, 4, 4);
          tofFindTracks->SetStation(24, 0, 3, 4);
          tofFindTracks->SetStation(25, 9, 0, 0);
          tofFindTracks->SetStation(26, 9, 0, 1);
          break;

        case 3:  // for reduced bias tracking of full setup
          iMinNofHits   = 3;
          iNStations    = 28;
          iNReqStations = 4;
          tofFindTracks->SetStation(0, 0, 2, 2);
          tofFindTracks->SetStation(1, 0, 1, 2);
          tofFindTracks->SetStation(2, 0, 0, 2);
          tofFindTracks->SetStation(3, 0, 2, 1);
          tofFindTracks->SetStation(4, 0, 1, 1);
          tofFindTracks->SetStation(5, 0, 0, 1);
          tofFindTracks->SetStation(6, 0, 2, 3);
          tofFindTracks->SetStation(7, 0, 1, 3);
          tofFindTracks->SetStation(8, 0, 0, 3);
          tofFindTracks->SetStation(9, 0, 2, 0);
          tofFindTracks->SetStation(10, 0, 1, 0);
          tofFindTracks->SetStation(11, 0, 0, 0);
          tofFindTracks->SetStation(12, 0, 2, 4);
          tofFindTracks->SetStation(13, 0, 1, 4);
          tofFindTracks->SetStation(14, 0, 0, 4);
          tofFindTracks->SetStation(15, 0, 4, 0);
          tofFindTracks->SetStation(16, 0, 3, 0);
          tofFindTracks->SetStation(17, 0, 4, 1);
          tofFindTracks->SetStation(18, 0, 3, 1);
          tofFindTracks->SetStation(19, 0, 4, 2);
          tofFindTracks->SetStation(20, 0, 3, 2);
          tofFindTracks->SetStation(21, 0, 4, 3);
          tofFindTracks->SetStation(22, 0, 3, 3);
          tofFindTracks->SetStation(23, 0, 4, 4);
          tofFindTracks->SetStation(24, 0, 3, 4);
          tofFindTracks->SetStation(25, 9, 0, 0);
          tofFindTracks->SetStation(26, 9, 0, 1);
          tofFindTracks->SetStation(27, 5, 0, 0);
          break;
      }
      tofFindTracks->SetMinNofHits(iMinNofHits);
      tofFindTracks->SetNStations(iNStations);
      tofFindTracks->SetNReqStations(iNReqStations);
      //tofFindTracks->PrintSetup();
      //      run->AddTask(tofFindTracks);
    } break;
    case 1: {
    }
    case 0:
    default:;
  }

  // -----   Local reconstruction of RICH Hits ------------------------------
  CbmRichMCbmHitProducer* hitProdRich = new CbmRichMCbmHitProducer();
  hitProdRich->setToTLimits(23.7, 30.0);
  hitProdRich->applyToTCut();
  TString sRichMapFile = srcDir + "/macro/rich/mcbm/beamtime/mRICH_Mapping_vert_20190318_elView.geo";
  hitProdRich->SetMappingFile(sRichMapFile.Data());
  //  run->AddTask(hitProdRich);
  // ------------------------------------------------------------------------

  // -----   Local reconstruction in RICh -> Finding of Rings ---------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  //  run->AddTask(richReco);
  // ------------------------------------------------------------------------


  // -----  Psd hit producer   ----------------------------------------------
  CbmPsdMCbmHitProducer* hitProdPsd = new CbmPsdMCbmHitProducer();
  //  run->AddTask(hitProdPsd);
  // ------------------------------------------------------------------------

  // --- Analysis by TOF track extension
  CbmTofExtendTracks* tofExtendTracks = new CbmTofExtendTracks("TofExtAna");
  tofExtendTracks->SetCalParFileName("TofExtTracksPar.root");
  tofExtendTracks->SetCalOutFileName("TofExtTracksOut.root");
  tofExtendTracks->SetStationUT(1);
  //iLev: 0 update alignment with deviation from original tracklet
  //iLev: 1 update alignment with deviation from extended and refitted tracklet
  tofExtendTracks->SetCorSrc(0);     // [iLev]0 - all hits, [ilev]1 - pulls,
  tofExtendTracks->SetCorMode(210);  // 2 - Y coordinate, 1 - X coordinat, 0 Time offset
  tofExtendTracks->SetTrkHitsMin(4);
  tofExtendTracks->SetAddStations(1);
  tofExtendTracks->SetReqStations(509);
  tofExtendTracks->SetCutDX(10.);
  tofExtendTracks->SetCutDY(10.);
  tofExtendTracks->SetCutDT(50.);
  tofExtendTracks->SetChi2Max(1000.);
  tofExtendTracks->SetCutStationMaxHitMul(100.);
  tofExtendTracks->SetNTrkTofMax(50);
  run->AddTask(tofExtendTracks);

  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  FairParRootFileIo* parIo3  = new FairParRootFileIo();
  parIo1->open(parFileIn.Data(), "READ");
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  parIo3->open(parFileOut.Data(), "RECREATE");
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
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
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
