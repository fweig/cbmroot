/* Copyright (C) 2019-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated mCBM events with standard settings
//
// Event-by-event reconstruction; requires appropriate digitization before
// (see mcbm_digi.C)
//
// Local reconstruction in MVD, STS, MUCH, TRD and TOF
// Binned tracker for track reconstruction
//
// V. Friese   11.06.2018
//
// --------------------------------------------------------------------------


void mcbm_display_event(Int_t nEvents = 3, TString dataset = "data/test", const char* setupName = "mcbm_beam_2022_03")
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel = "INFO";
  //TString logLevel     = "DEBUG";
  //  TString logVerbosity = "LOW";
  TString logVerbosity = "MEDIUM";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_display";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  TString inFile  = dataset + ".tra.root";
  TString rawFile = dataset + ".event.raw.root";
  TString parFile = dataset + ".par.root";
  TString recFile = dataset + ".dis.root";
  // ------------------------------------------------------------------------

  Int_t iTofCluMode = 1;

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  setup->RemoveModule(kTrd);
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (setup->GetGeoTag(kTrd, geoTag)) {
    TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
    parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file " << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if (setup->GetGeoTag(kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << rawFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->AddFriend(rawFile);
  run->SetOutputFile(recFile);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = kFALSE;  //Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MVD   ----------------------------------
  if (setup->IsActive(kMvd)) {

    CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
    run->AddTask(mvdCluster);
    std::cout << "-I- " << myName << ": Added task " << mvdCluster->GetName() << std::endl;

    CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
    mvdHit->UseClusterfinder(kTRUE);
    run->AddTask(mvdHit);
    std::cout << "-I- " << myName << ": Added task " << mvdHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  if (setup->IsActive(kSts)) {

    FairTask* stsReco = new CbmStsReco();
    run->AddTask(stsReco);
    std::cout << "-I- : Added task " << stsReco->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  if (0) {  // setup->IsActive(kMuch) ) {

    // --- Parameter file name
    TString geoTag;
    setup->GetGeoTag(kMuch, geoTag);
    Int_t muchFlag = 0;
    if (geoTag.Contains("mcbm")) muchFlag = 1;

    TString parFile = gSystem->Getenv("VMCWORKDIR");

    if (muchFlag) {
      std::cout << geoTag << std::endl;
      parFile = parFile + "/parameters/much/much_" + geoTag + "_digi_sector.root";
      std::cout << "Using parameter file " << parFile << std::endl;
    }
    else {
      std::cout << geoTag(0, 4) << std::endl;
      parFile = parFile + "/parameters/much/much_" + geoTag(0, 4) + "_digi_sector.root";
      std::cout << "Using parameter file " << parFile << std::endl;
    }


    // --- Hit finder for GEMs
    FairTask* muchHitGem = new CbmMuchFindHitsGem(parFile.Data(), muchFlag);
    run->AddTask(muchHitGem);
    std::cout << "-I- " << myName << ": Added task " << muchHitGem->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TRD   ----------------------------------
  if (0) {  //setup->IsActive(kTrd) ) {

    Double_t triggerThreshold       = 0.5e-6;  // SIS100
    Bool_t triangularPads           = false;   // Bucharest triangular pad-plane layout
    CbmTrdClusterFinder* trdCluster = new CbmTrdClusterFinder();
    trdCluster->SetNeighbourEnable(true);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetNeighbourEnable(false);
    trdCluster->SetRowMerger(true);
    run->AddTask(trdCluster);
    std::cout << "-I- " << myName << ": Added task " << trdCluster->GetName() << std::endl;

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- " << myName << ": Added task " << trdHit->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  if (setup->IsActive(kTof)) {
    switch (iTofCluMode) {
      case 1: {
        CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer", 0, 1);
        Int_t calMode                      = 93;
        Int_t calSel                       = 0;
        Int_t calSm                        = 0;
        Int_t RefSel                       = 0;
        Double_t dDeadtime                 = 50.;

        tofCluster->SetCalMode(calMode);
        tofCluster->SetCalSel(calSel);
        tofCluster->SetCaldXdYMax(3.);              // geometrical matching window in cm
        tofCluster->SetCalCluMulMax(5.);            // Max Counter Cluster Multiplicity for filling calib histos
        tofCluster->SetCalRpc(calSm);               // select detector for calibration update
        tofCluster->SetTRefId(RefSel);              // reference trigger for offset calculation
        tofCluster->SetTotMax(20.);                 // Tot upper limit for walk corection
        tofCluster->SetTotMin(0.01);                //(12000.);  // Tot lower limit for walk correction
        tofCluster->SetTotPreRange(5.);             // effective lower Tot limit  in ns from peak position
        tofCluster->SetTotMean(5.);                 // Tot calibration target value in ns
        tofCluster->SetMaxTimeDist(1.0);            // default cluster range in ns
        tofCluster->SetDelTofMax(5.);               // acceptance range for cluster distance in ns (!)
        tofCluster->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
        tofCluster->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
        tofCluster->SetEnableAvWalk(kFALSE);
        //tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
        tofCluster->SetYFitMin(1.E4);
        tofCluster->SetToDAv(0.04);
        tofCluster->SetIdMode(1);        // calibrate on module level
        tofCluster->SetTRefDifMax(2.0);  // in ns
        tofCluster->PosYMaxScal(0.75);   //in % of length
        run->AddTask(tofCluster);
        std::cout << "-I- " << myName << ": Added task " << tofCluster->GetName() << std::endl;
      } break;
      default: {
        CbmTofSimpClusterizer* tofCluster = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
        tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
        tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
        run->AddTask(tofCluster);
        std::cout << "-I- " << myName << ": Added task " << tofCluster->GetName() << std::endl;
      }
    }
  }
  // -------------------------------------------------------------------------


  // -----   Track reconstruction   ------------------------------------------
  Double_t beamWidthX = 0.1;
  Double_t beamWidthY = 0.1;
  Int_t iGenCor        = 1;
  Double_t dScalFac    = 1.;
  Double_t dChi2Lim2   = 3.5;
  TString cTrkFile     = Form("%s_tofFindTracks.hst.root", "MC");
  Int_t iTrackingSetup = 1;

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
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  tofFindTracks->SetTtTarg(0.041);     // target value for inverse velocity, > 0.033 ns/cm!
  //tofFindTracks->SetTtTarg(0.035);                // target value for inverse velocity, > 0.033 ns/cm!
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
      iNStations    = 39;
      iNReqStations = 3;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 0);
      tofFindTracks->SetStation(2, 0, 3, 0);
      tofFindTracks->SetStation(3, 0, 4, 1);
      tofFindTracks->SetStation(4, 0, 3, 1);
      tofFindTracks->SetStation(5, 0, 4, 2);
      tofFindTracks->SetStation(6, 0, 3, 2);
      tofFindTracks->SetStation(7, 0, 4, 3);
      tofFindTracks->SetStation(8, 0, 3, 3);
      tofFindTracks->SetStation(9, 0, 4, 4);
      tofFindTracks->SetStation(10, 0, 3, 4);
      tofFindTracks->SetStation(11, 9, 0, 0);
      tofFindTracks->SetStation(12, 9, 0, 1);
      tofFindTracks->SetStation(13, 7, 0, 0);
      tofFindTracks->SetStation(14, 6, 0, 0);
      tofFindTracks->SetStation(15, 6, 0, 1);
      tofFindTracks->SetStation(16, 8, 0, 0);
      tofFindTracks->SetStation(17, 8, 0, 1);
      tofFindTracks->SetStation(18, 8, 0, 2);
      tofFindTracks->SetStation(19, 8, 0, 3);
      tofFindTracks->SetStation(20, 8, 0, 4);
      tofFindTracks->SetStation(21, 8, 0, 5);
      tofFindTracks->SetStation(22, 8, 0, 6);
      tofFindTracks->SetStation(23, 8, 0, 7);
      tofFindTracks->SetStation(24, 0, 2, 2);
      tofFindTracks->SetStation(25, 0, 1, 2);
      tofFindTracks->SetStation(26, 0, 0, 2);
      tofFindTracks->SetStation(27, 0, 2, 1);
      tofFindTracks->SetStation(28, 0, 1, 1);
      tofFindTracks->SetStation(29, 0, 0, 1);
      tofFindTracks->SetStation(30, 0, 2, 3);
      tofFindTracks->SetStation(31, 0, 1, 3);
      tofFindTracks->SetStation(32, 0, 0, 3);
      tofFindTracks->SetStation(33, 0, 2, 0);
      tofFindTracks->SetStation(34, 0, 1, 0);
      tofFindTracks->SetStation(35, 0, 0, 0);
      tofFindTracks->SetStation(36, 0, 2, 4);
      tofFindTracks->SetStation(37, 0, 1, 4);
      tofFindTracks->SetStation(38, 0, 0, 4);
      break;
  }
  tofFindTracks->SetMinNofHits(iMinNofHits);
  tofFindTracks->SetNStations(iNStations);
  tofFindTracks->SetNReqStations(iNReqStations);
  tofFindTracks->PrintSetup();
  run->AddTask(tofFindTracks);
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  /*
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  */
  // ------------------------------------------------------------------------


  // -----   Database update   ----------------------------------------------
  /*
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  */
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  /*
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  */
  // ------------------------------------------------------------------------

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track    = new FairMCTracks("Monte-Carlo Tracks");

  FairMCPointDraw* MvdPoint      = new FairMCPointDraw("MvdPoint", kBlack, kFullSquare);
  FairMCPointDraw* StsPoint      = new FairMCPointDraw("StsPoint", kGreen, kFullSquare);
  FairMCPointDraw* MuchPoint     = new FairMCPointDraw("MuchPoint", kOrange, kFullSquare);
  FairMCPointDraw* RichPoint     = new FairMCPointDraw("RichPoint", kGreen, kFullSquare);
  FairMCPointDraw* TrdPoint      = new FairMCPointDraw("TrdPoint", kBlue, kFullSquare);
  FairMCPointDraw* TofPoint      = new FairMCPointDraw("TofPoint", kRed, kFullSquare);
  FairMCPointDraw* EcalPoint     = new FairMCPointDraw("EcalPoint", kYellow, kFullSquare);
  FairMCPointDraw* RefPlanePoint = new FairMCPointDraw("RefPlanePoint", kPink, kFullSquare);

  fMan->AddTask(Track);

  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(MuchPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);
  fMan->AddTask(EcalPoint);
  fMan->AddTask(RefPlanePoint);


  CbmPixelHitSetDraw* StsHits = new CbmPixelHitSetDraw("StsHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(StsHits);
  CbmPixelHitSetDraw* TrdHits = new CbmPixelHitSetDraw("TrdHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(TrdHits);
  CbmPixelHitSetDraw* TofHits = new CbmPixelHitSetDraw("TofHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(TofHits);
  CbmPixelHitSetDraw* TofUHits = new CbmPixelHitSetDraw("TofUHit", kRed, kOpenCross);
  fMan->AddTask(TofUHits);
  CbmEvDisTracks* Tracks = new CbmEvDisTracks("Tof Tracks", 1);
  Tracks->SetVerbose(4);
  fMan->AddTask(Tracks);

  fMan->Init(1, 7, 10000);  // make MVD visible by default

  cout << "gEve " << gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow - 10);
  {  // from readCurrentCamera(const char* fname)
    TGLCamera& c      = gEve->GetDefaultGLViewer()->CurrentCamera();
    const char* fname = "Cam.sav";
    TFile* f          = TFile::Open(fname, "READ");
    if (!f) return;
    if (f->GetKey(c.ClassName())) {
      f->GetKey(c.ClassName())->Read(&c);
      c.IncTimeStamp();
      gEve->GetDefaultGLViewer()->RequestDraw();
    }
  }
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << recFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  if (hasFairMonitor) {  // FairRoot Version >= 15.11
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

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }

  //  RemoveGeoManager();
}
