// --------------------------------------------------------------------------
//
// Macro for reconstruction "TOF tracks" in mcbm data (2020)
// Runs on output of event-based combined hit reconstruction for all systems.
//
// --------------------------------------------------------------------------

Bool_t mcbm_tof_tracking(UInt_t uRunId     = 831,
                         Bool_t bEventWin  = kFALSE,
                         Int_t iTrackMode  = 2,
                         Int_t iCalOpt     = 0,
                         Int_t nTimeslices = 0,
                         TString sInpDir   = "./data",
                         TString sOutDir   = "./data",
                         TString cCalId    = "831.50.3.0") {

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName   = "mcbm_tof_tracking";  // this macro's name for screen outp
  TString srcDir   = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString paramDir = srcDir + "/macro/beamtime/mcbm2020/";
  TString parDir   = srcDir + "/parameters";
  // ------------------------------------------------------------------------

  /// FIXME: Disable clang formatting around parameters initial value setting
  /// due to problem with real file path length
  /* clang-format off */
  // -----   In- and output file names   ------------------------------------
  TString inFile     = sInpDir + Form("/reco_mcbm_event_%03u.root", uRunId);
  TString parFileIn  = sInpDir + Form("/reco_mcbm_event_params_%03u.root",
                                      uRunId);
  TString parFileOut = sOutDir + Form("/tracking_mcbm_event_params_%03u.root",
                                      uRunId);
  TString outFile    = sOutDir + Form("/tracking_mcbm_event_%03u.root", uRunId);
  if (bEventWin) {
     inFile     = sInpDir + Form("/reco_mcbm_evt_win_%03u.root", uRunId);
     parFileIn  = sInpDir + Form("/reco_mcbm_evt_win_params_%03u.root",
                                 uRunId);
     parFileOut = sOutDir + Form("/tracking_mcbm_evt_win_params_%03u.root",
                                 uRunId);
     outFile    = sOutDir + Form("/tracking_mcbm_evt_win_%03u.root", uRunId);
  }  // if( bEventWin )
  // ------------------------------------------------------------------------
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  /*
  std::cout << sInpDir << std::endl << sOutDir << std::endl;
  std::cout << inFile << std::endl
            << parFileIn << std::endl
            << parFileOut << std::endl
            << outFile << std::endl;
  std::cout << uRunId << " " << nTimeslices << std::endl;

  return kTRUE;
  */

  // --- Load the geometry setup ----
  // This is currently only required by the TRD (parameters)
  std::string geoSetupTag = "mcbm_beam_2020_03";
  TString geoFile =
    paramDir + geoSetupTag.data() + ".geo.root";  // Created in sim. run
  CbmSetup* geoSetup = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag.data());
  TList* parFileList = new TList();
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);

  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  run->SetSink(outputSink);
  run->SetGeomFile(geoFile);

  // Define output file for FairMonitor histograms
  TString monitorFile {outFile};
  monitorFile.ReplaceAll("reco", "reco.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Track reconstruction   ------------------------------------------
  switch (iTrackMode) {
    case 2: {
      Int_t iGenCor      = 1;
      Double_t dScalFac  = 1.;
      Double_t dChi2Lim2 = 3.5;
      TString cTrkFile =
        parDir + "/tof/" + Form("%s_tofFindTracks.hst.root", cCalId.Data());
      Int_t iTrackingSetup = 1;

      CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
      tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
      tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);  // max dev from mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);  // mean slope dy/dz
      CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
      TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
      tofTrackFinder->SetFitter(tofTrackFitter);

      CbmTofFindTracks* tofFindTracks =
        new CbmTofFindTracks("TOF Track Finder");
      tofFindTracks->UseFinder(tofTrackFinder);
      tofFindTracks->UseFitter(tofTrackFitter);
      tofFindTracks->SetCalOpt(iCalOpt);
      // 1 - update offsets, 2 - update walk, 0 - bypass
      tofFindTracks->SetCorMode(
        iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
      tofFindTracks->SetTtTarg(
        0.065);  // target value for Mar2020 triple stack -> betapeak ~ 0.95
      //tofFindTracks->SetTtTarg(0.041);  // target value for inverse velocity, > 0.033 ns/cm!
      //tofFindTracks->SetTtTarg(0.035);  // target value for inverse velocity, > 0.033 ns/cm!
      tofFindTracks->SetCalParFileName(
        cTrkFile);  // Tracker parameter value file name
      tofFindTracks->SetBeamCounter(5, 0, 0);  // default beam counter
      tofFindTracks->SetStationMaxHMul(
        30);  // Max Hit Multiplicity in any used station

      tofFindTracks->SetT0MAX(dScalFac);  // in ns
      tofFindTracks->SetSIGT(0.08);       // default in ns
      tofFindTracks->SetSIGX(0.3);        // default in cm
      tofFindTracks->SetSIGY(0.45);       // default in cm
      tofFindTracks->SetSIGZ(0.05);       // default in cm
      tofFindTracks->SetUseSigCalib(
        kFALSE);  // ignore resolutions in CalPar file
      tofTrackFinder->SetSIGLIM(dChi2Lim2
                                * 2.);  // matching window in multiples of chi2
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
      run->AddTask(tofFindTracks);
    } break;
    case 1: {
    }
    case 0:
    default:;
  }


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  FairParRootFileIo* parIo3  = new FairParRootFileIo();
  parIo1->open(parFileIn.Data(), "READ");
  rtdb->setFirstInput(parIo1);
  parIo2->open(parFileList, "in");
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

  return kTRUE;
}
