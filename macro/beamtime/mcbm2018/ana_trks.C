void ana_trks(Int_t nEvents        = 10000,
              Int_t iSel           = 1,
              Int_t iGenCor        = 1,
              TString cFileId      = "48.50.7.1",
              TString cSet         = "000010020",
              Int_t iSel2          = 20,
              Int_t iTrackingSetup = 2,
              Double_t dScalFac    = 1.,
              Double_t dChi2Lim2   = 500.,
              Double_t dDeadtime   = 50,
              TString cCalId       = "",
              Int_t iAnaCor        = 1,
              Bool_t bUseSigCalib  = kFALSE,
              Int_t iCalSet        = 30040500,
              Int_t iCalOpt        = 1,
              Int_t iMc            = 0) {
  Int_t iVerbose = 1;
  if (cCalId == "") cCalId = cFileId;
  TString FId = cFileId;
  TString cRun(FId(0, 3));
  Int_t iRun = cRun.Atoi();
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  TString workDir  = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/macro/beamtime/mcbm2018";
  //TString paramDir       = ".";

  TString ParFile       = paramDir + "/data/" + cFileId.Data() + ".params.root";
  TString InputFile     = paramDir + "/data/" + cFileId.Data() + ".root";
  TString InputDigiFile = paramDir + "/data/digidev_" + cFileId.Data()
                          + Form("_%s_%02.0f_Cal", cSet.Data(), dDeadtime)
                          + cCalId + ".out.root";
  if (iMc == 1) {
    InputFile     = paramDir + "/data/" + cFileId.Data() + ".raw.root";
    InputDigiFile = paramDir + "/data/" + cFileId.Data() + ".rec.root";
    iRun          = 700;
  }
  TString OutputFile = paramDir + "/data/hits_" + cFileId.Data()
                       + Form("_%s_%06d_%03d", cSet.Data(), iSel, iSel2)
                       + ".out.root";
  TString cHstFile =
    paramDir
    + Form(
      "/hst/%s_%03.0f_%s_%06d_%03d_%03.1f_%03.1f_trk%03d_Cal%s_Ana.hst.root",
      cFileId.Data(),
      dDeadtime,
      cSet.Data(),
      iSel,
      iSel2,
      dScalFac,
      dChi2Lim2,
      iTrackingSetup,
      cCalId.Data());
  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cCalId.Data());
  TString cAnaFile = Form("%s_TrkAnaTestBeam.hst.root", cFileId.Data());

  cout << " InputDigiFile = " << InputDigiFile << endl;

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();
  Int_t iGeo = 0;  //iMc;
  if (iGeo == 0) {
    TString TofGeo = "v18m_mcbm";  //default

    cout << "Geometry version " << TofGeo << endl;

    TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/tof_"
                                                + TofGeo + ".digibdf.par");
    parFileList->Add(tofDigiBdfFile);

    TString geoDir  = gSystem->Getenv("VMCWORKDIR");
    TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
    TFile* fgeo     = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile" << endl;
      return;
    }
  } else {
    TString setupName = "mcbm_beam_2019_03";
    // -----   Load the geometry setup   -------------------------------------
    TString setupFile =
      workDir + "/geometry/setup/setup_" + setupName.Data() + ".C";
    TString setupFunct = "setup_";
    setupFunct         = setupFunct + setupName + "()";
    std::cout << "-I- Loading macro " << setupFile << std::endl;
    gROOT->LoadMacro(setupFile);
    gROOT->ProcessLine(setupFunct);
    CbmSetup* setup = CbmSetup::Instance();
  }

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  cout << "InputFile:     " << InputFile.Data() << endl;
  cout << "InputDigiFile: " << InputDigiFile.Data() << endl;

  //run->SetInputFile(InputFile.Data());
  //run->AddFriend(InputDigiFile.Data());
  run->SetInputFile(InputDigiFile.Data());
  //run->AddFriend(InputFile.Data());
  //run->SetOutputFile(OutputFile);
  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  //FairLogger::GetLogger()->SetLogVerbosityLevel("VERYHIGH");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

  // -----   Local selection variables  -------------------------------------------

  Int_t iRef    = iSel % 1000;
  Int_t iDut    = (iSel - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;
  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;


  Int_t calMode = 93;
  Int_t calSel  = 1;
  Bool_t bOut   = kFALSE;

  CbmTofEventClusterizer* tofClust =
    new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);
  Int_t calSelRead = calSel;
  if (calSel < 0) calSelRead = 0;
  TString cFname = Form("%s_set%09d_%02d_%01dtofClust.hst.root",
                        cFileId.Data(),
                        iCalSet,
                        calMode,
                        calSelRead);
  if (cCalId != "XXX")
    cFname = Form("%s_set%09d_%02d_%01dtofClust.hst.root",
                  cCalId.Data(),
                  iCalSet,
                  calMode,
                  calSelRead);
  tofClust->SetCalParFileName(cFname);
  TString cOutFname =
    Form("tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================

  CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
  tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
  Int_t TrackerPar = 0;
  switch (TrackerPar) {
    case 0:                           // for full mTof setup
      tofTrackFinder->SetTxLIM(0.3);  // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);  // max dev from mean slope dy/dz
      tofTrackFinder->SetTxMean(0.);  // mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);  // mean slope dy/dz
      break;
    case 1:                             // for double stack test counters
      tofTrackFinder->SetTxMean(0.21);  // mean slope dx/dz
      tofTrackFinder->SetTyMean(0.18);  // mean slope dy/dz
      tofTrackFinder->SetTxLIM(0.15);   // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.18);   // max dev from mean slope dy/dz
      break;
    case 2:                             // for triple stack 012 counter evaluation
      tofTrackFinder->SetTxMean(0.0);  // mean slope dx/dz
      tofTrackFinder->SetTyMean(-0.03);   // mean slope dy/dz
      tofTrackFinder->SetTxLIM(0.06);  // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.03);  // max dev from mean slope dy/dz
      break;      
  }

  CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
  TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
  tofTrackFinder->SetFitter(tofTrackFitter);
  CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
  tofFindTracks->UseFinder(tofTrackFinder);
  tofFindTracks->UseFitter(tofTrackFitter);
  tofFindTracks->SetCalOpt(
    iCalOpt);  // 1 - update offsets, 2 - update walk, 0 - bypass
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  //tofFindTracks->SetTtTarg(0.041);     // Mar19, Run 159
  //tofFindTracks->SetTtTarg(0.0605);  // target value for Mar2020 triple stack -> betapeak ~ 0.95
  tofFindTracks->SetTtTarg(0.062);   // target value for Mar2020 triple stack -> betapeak ~ 0.95
  //tofFindTracks->SetTtTarg(0.058);   // target value for Mar2020 double stack
  //tofFindTracks->SetTtTarg(0.051);   // target value Nov2019
  //tofFindTracks->SetTtTarg(0.035);   // target value for inverse velocity, > 0.033 ns/cm!
  tofFindTracks->SetCalParFileName(
    cTrkFile);                             // Tracker parameter value file name
  tofFindTracks->SetBeamCounter(5, 0, 0);  // default beam counter
  tofFindTracks->SetR0Lim(0.); // for calibration
  tofFindTracks->SetStationMaxHMul(
    30);  // Max Hit Multiplicity in any used station

  tofFindTracks->SetT0MAX(dScalFac);  // in ns
  tofFindTracks->SetSIGT(0.08);       // default in ns
  tofFindTracks->SetSIGX(0.45);       // local-y default in cm
  tofFindTracks->SetSIGY(0.3);        // local-x default in cm
  tofFindTracks->SetSIGZ(10.05);       // default in cm
  tofFindTracks->SetUseSigCalib(
    bUseSigCalib);  // ignore resolutions in CalPar file
  tofTrackFinder->SetSIGLIM(dChi2Lim2
                            * 2.);  // matching window in multiples of chi2
  tofTrackFinder->SetSIGLIMMOD(1.5);   // search window modifier for last hit (DUT)
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
    {
      Double_t dTsig = dScalFac * 0.03;
      tofFindTracks->SetSIGT(dTsig);  // allow for variable deviations in ns
    }
      iMinNofHits   = 3;
      iNStations    = 39;
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
      tofFindTracks->SetStation(28, 7, 0, 0);
      tofFindTracks->SetStation(29, 6, 0, 0);
      tofFindTracks->SetStation(30, 6, 0, 1);
      tofFindTracks->SetStation(31, 8, 0, 0);
      tofFindTracks->SetStation(32, 8, 0, 1);
      tofFindTracks->SetStation(33, 8, 0, 2);
      tofFindTracks->SetStation(34, 8, 0, 3);
      tofFindTracks->SetStation(35, 8, 0, 4);
      tofFindTracks->SetStation(36, 8, 0, 5);
      tofFindTracks->SetStation(37, 8, 0, 6);
      tofFindTracks->SetStation(38, 8, 0, 7);
      break;

    case 2:
      iMinNofHits   = 3;
      iNStations    = 14;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 0, 4, 0);
      tofFindTracks->SetStation(4, 0, 3, 0);
      tofFindTracks->SetStation(5, 0, 4, 2);
      tofFindTracks->SetStation(6, 0, 3, 2);
      tofFindTracks->SetStation(7, 0, 4, 3);
      tofFindTracks->SetStation(8, 0, 3, 3);
      tofFindTracks->SetStation(9, 0, 4, 4);
      tofFindTracks->SetStation(10, 0, 3, 4);
      tofFindTracks->SetStation(11, 9, 0, 0);
      tofFindTracks->SetStation(12, 9, 0, 1);
      tofFindTracks->SetStation(13, 7, 0, 0);
      break;

    case 3:
      iMinNofHits   = 3;
      iNStations    = 16;
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

      /*
     tofFindTracks->SetStation(16, 0, 3, 2);         
     tofFindTracks->SetStation(17, 0, 4, 2);  
     tofFindTracks->SetStation(18, 0, 3, 1);         
     tofFindTracks->SetStation(19, 0, 4, 1);
     tofFindTracks->SetStation(20, 0, 3, 3);         
     tofFindTracks->SetStation(21, 0, 4, 3);
     tofFindTracks->SetStation(22, 0, 3, 0);         
     tofFindTracks->SetStation(23, 0, 4, 0);
     tofFindTracks->SetStation(24, 0, 3, 4);         
     tofFindTracks->SetStation(25, 0, 4, 4); 
     */
      break;

    case 4:
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 2, 2);
      tofFindTracks->SetStation(2, 0, 0, 2);
      tofFindTracks->SetStation(3, 0, 1, 2);
      break;
      
    case 14:    // for eval 012
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(3, 5, 0, 0);
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 0, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      break;
      
    case 24:    // for geometry tuning 012
      iMinNofHits   = 3;
      iNStations    = 3;
      iNReqStations = 3;
      tofFindTracks->SetStation(0, 0, 2, 2);
      tofFindTracks->SetStation(1, 0, 0, 2);
      tofFindTracks->SetStation(2, 0, 1, 2);
      break;


    case 5:  // for calibration of 2-stack and add-on counters (STAR2, CERN)
      iMinNofHits   = 5;
      iNStations    = 6;
      iNReqStations = 6;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 9, 0, 0);
      tofFindTracks->SetStation(4, 9, 0, 1);
      tofFindTracks->SetStation(5, 7, 0, 0);
      break;

    case 6:
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      //    tofFindTracks->SetStation(3, 9, 0, 0);
      //    tofFindTracks->SetStation(3, 9, 0, 1);
      //    tofFindTracks->SetStation(3, 7, 0, 0);
      break;

    case 7:  // for calibration of 2-stack and add-on counters (BUC)
      iMinNofHits   = 3;
      iNStations    = 5;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 3);
      tofFindTracks->SetStation(2, 0, 3, 3);
      tofFindTracks->SetStation(3, 6, 0, 0);
      tofFindTracks->SetStation(4, 6, 0, 1);
      break;

    case 8:  // evaluation of add-on counters (BUC)
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 3);
      tofFindTracks->SetStation(2, 0, 3, 3);
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      break;

    case 10:
      iMinNofHits   = 3;
      iNStations    = 4;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 1, 2);
      tofFindTracks->SetStation(2, 0, 0, 2);
      tofFindTracks->SetStation(3, 0, 2, 2);
      break;

    default:
      cout << "Tracking setup " << iTrackingSetup << " not implemented "
           << endl;
      return;
      ;
  }
  tofFindTracks->SetMinNofHits(iMinNofHits);
  tofFindTracks->SetNStations(iNStations);
  tofFindTracks->SetNReqStations(iNReqStations);
  tofFindTracks->PrintSetup();
  run->AddTask(tofFindTracks);

  // =========================================================================
  // ===                       Analysis                                    ===
  // =========================================================================
  CbmTofAnaTestbeam* tofAnaTestbeam =
    new CbmTofAnaTestbeam("TOF TestBeam Analysis", iVerbose);
  tofAnaTestbeam->SetCorMode(iAnaCor);  // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp
  tofAnaTestbeam->SetHitDistMin(30.);   // initialization
  tofAnaTestbeam->SetEnableMatchPosScaling(kTRUE);
  tofAnaTestbeam->SetSpillDuration(3.);
  if (iMc == 1) {
    tofAnaTestbeam->SetSpillDuration(0.);
    tofAnaTestbeam->SetSpillBreak(0.);
  }
  //CbmTofAnaTestbeam defaults
  tofAnaTestbeam->SetR0LimFit(
    0.);  // limit distance of fitted track to nominal vertex
  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);  // in ns
  tofAnaTestbeam->SetDXWidth(0.5);
  tofAnaTestbeam->SetDYWidth(1.0);
  tofAnaTestbeam->SetDTWidth(0.1);  // in ns
  tofAnaTestbeam->SetCalParFileName(cAnaFile);
  Double_t dScalFacA = 0.9;  // dScalFac is used for tracking
  tofAnaTestbeam->SetPosY4Sel(
    0.5 * dScalFacA);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);    // Time difference to additional diamond
  tofAnaTestbeam->SetMul0Max(20);  // Max Multiplicity in dut
  tofAnaTestbeam->SetMul4Max(30);  // Max Multiplicity in Ref - RPC
  tofAnaTestbeam->SetMulDMax(3);   // Max Multiplicity in Diamond / BeamRef
  tofAnaTestbeam->SetTOffD4(14.);  // initialization
  tofAnaTestbeam->SetDTD4MAX(
    6.);  // initialization of Max time difference Ref - BRef

  //tofAnaTestbeam->SetTShift(-28000.);// initialization
  tofAnaTestbeam->SetPosYS2Sel(
    0.55);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);     // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);  // Width  of channel selection window
  tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
  tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit
  tofAnaTestbeam->SetChi2Lim2(
    3.);  // initialization of Chi2 selection limit for Mref-Sel2 pair
  tofAnaTestbeam->SetDutDX(
    15.);  // limit inspection of tracklets to selected region
  tofAnaTestbeam->SetDutDY(
    15.);  // limit inspection of tracklets to selected region
  tofAnaTestbeam->SetSIGLIM(3.);  // max matching chi2
  tofAnaTestbeam->SetSIGT(0.08);  // in ns
  tofAnaTestbeam->SetSIGX(0.3);   // in cm
  tofAnaTestbeam->SetSIGY(0.6);   // in cm

  Int_t iRSel    = 500;
  Int_t iRSelTyp = 5;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  Int_t iRSelin  = iRSel;

  tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);  // common reaction reference
  tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
  tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);

  if (iSel2 >= -1) {
    tofAnaTestbeam->SetMrpcSel2(
      iSel2);  // initialization of second selector Mrpc Type
    tofAnaTestbeam->SetMrpcSel2Sm(
      iSel2Sm);  // initialization of second selector Mrpc SmId
    tofAnaTestbeam->SetMrpcSel2Rpc(
      iSel2Rpc);  // initialization of second selector Mrpc RpcId
  }

  cout << "AnaTestbeam init for Dut " << iDut << iDutSm << iDutRpc << ", Ref "
       << iRef << iRefSm << iRefRpc << endl;

  tofAnaTestbeam->SetDut(iDut);            // Device under test
  tofAnaTestbeam->SetDutSm(iDutSm);        // Device under test
  tofAnaTestbeam->SetDutRpc(iDutRpc);      // Device under test
  tofAnaTestbeam->SetMrpcRef(iRef);        // Reference RPC
  tofAnaTestbeam->SetMrpcRefSm(iRefSm);    // Reference RPC
  tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);  // Reference RPC

  cout << "dispatch iSel = " << iSel << ", iSel2in = " << iSel2in
       << ", iRSelin = " << iRSelin << ", iRSel = " << iRSel << endl;
  if (1) {
    switch (iSel) {

      case 10:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(2.5);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(18.);  // Shift DTD4 to physical value
            switch (iSel2in) {
              case 20:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              default:;
            }
            break;
          default:;
        }
        break;

      case 700040:
      case 900040:
      case 901040:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.3);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(18.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 30:
                tofAnaTestbeam->SetSel2TOff(-0.3);  // Shift Sel2 time peak to 0
                break;
              case 31:
                tofAnaTestbeam->SetSel2TOff(
                  -0.41);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 700041:
      case 900041:
      case 901041:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(0.8);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 30:
                tofAnaTestbeam->SetSel2TOff(-0.3);  // Shift Sel2 time peak to 0
                break;
              case 31:
                tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
                break;
              case 600:
                tofAnaTestbeam->SetSel2TOff(-0.2);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      case 600043:
      case 601043:
        switch (iRSelin) {
          case 500:
            tofAnaTestbeam->SetTShift(5.3);  // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);  // Shift DTD4 to physical value

            switch (iSel2in) {
              case 33:
                tofAnaTestbeam->SetSel2TOff(
                  -0.55);  // Shift Sel2 time peak to 0
                break;

              default:;
            }
            break;
          default:;
        }
        break;

      default:
        cout << "Better to define analysis setup! Running with default offset "
                "parameter... "
             << endl;
        // return;
    }  // end of different subsets

    cout << " Initialize TSHIFT to " << tofAnaTestbeam->GetTShift() << endl;
    run->AddTask(tofAnaTestbeam);
  }

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
  parIo2->open(ParFile.Data(), "UPDATE");
  parIo2->print();
  rtdb->setFirstInput(parIo2);

  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(parFileList, "in");
  parIo1->print();
  rtdb->setSecondInput(parIo1);
  rtdb->print();
  rtdb->printParamContexts();

  //  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  //  parInput1->open(ParFile.Data());
  //  rtdb->setFirstInput(parInput1);

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  //run->Run(nEvents-1, nEvents); //debugging single events for memory leak
  // ------------------------------------------------------------------------

  // default displays, plot results
  /*
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct;
  if (iGenCor<0) {
    Display_Funct = "pl_over_Mat04D4best(1)";
  }else{
    Display_Funct = "pl_over_Mat04D4best(0)";
  }
  gROOT->LoadMacro(Display_Status);

  cout << "Exec "<< Display_Funct.Data()<< endl;
  gInterpreter->ProcessLine(Display_Funct);
  */
  gROOT->LoadMacro("save_hst.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gROOT->LoadMacro("pl_eff_XY.C");
  gROOT->LoadMacro("pl_over_trk.C");
  gROOT->LoadMacro("pl_calib_trk.C");
  gROOT->LoadMacro("pl_XY_trk.C");
  gROOT->LoadMacro("pl_vert_trk.C");
  gROOT->LoadMacro("pl_pull_trk.C");
  gROOT->LoadMacro("pl_all_Track2D.C");
  gROOT->LoadMacro("pl_TIS.C");
  gROOT->LoadMacro("pl_TIR.C");
  gROOT->LoadMacro("pl_Eff_XY.C");
  gROOT->LoadMacro("pl_Eff_DTLH.C");
  gROOT->LoadMacro("pl_Eff_TIS.C");
  gROOT->LoadMacro("pl_Dut_Res.C");

  TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
  gInterpreter->ProcessLine(SaveToHstFile);

  //gInterpreter->ProcessLine("pl_over_MatD4sel()");
  //gInterpreter->ProcessLine("pl_TIS()");
  //gInterpreter->ProcessLine("pl_TIR()");
  //gInterpreter->ProcessLine("pl_eff_XY()");
  gInterpreter->ProcessLine("pl_calib_trk()");
  gInterpreter->ProcessLine("pl_vert_trk()");

  gInterpreter->ProcessLine("pl_all_Track2D(1)");
  gInterpreter->ProcessLine("pl_all_Track2D(2)");
  gInterpreter->ProcessLine("pl_all_Track2D(4)");

  TString over_trk = "pl_over_trk(" + (TString)(Form("%d", iNStations)) + ")";
  gInterpreter->ProcessLine(over_trk);

  TString XY_trk = "pl_XY_trk(" + (TString)(Form("%d", iNStations)) + ")";
  gInterpreter->ProcessLine(XY_trk);

  TString Pull0 = (TString)(Form("pl_pull_trk(%d,%d,1)", iNStations, 0));
  gInterpreter->ProcessLine(Pull0);
  TString Pull1 = (TString)(Form("pl_pull_trk(%d,%d,1)", iNStations, 1));
  gInterpreter->ProcessLine(Pull1);
  TString Pull3 = (TString)(Form("pl_pull_trk(%d,%d,1)", iNStations, 3));
  gInterpreter->ProcessLine(Pull3);
  TString Pull4 = (TString)(Form("pl_pull_trk(%d,%d,1)", iNStations, 4));
  gInterpreter->ProcessLine(Pull4);
}
