/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void run_reco_mcbm_real_wToF_may22_Nickel_Bmon(const unsigned int runId = 2160,  // used for the output folder
                                               int nEvents = 50, const int taskId = 11,
                                               const string srcfolder = "/data/cbmroot/mcbmsource/macro/run/data/")
{
  // -----   File names   --------------------------------------------------
  const string& digiFile = Form("%s/%4d_bmonintof.digi.root", srcfolder.c_str(), runId);


  //TString setup          = "mcbm_beam_2021_07_surveyed";
  TString setup = "mcbm_beam_2022_03_22_iron";
  if (runId >= 2352) setup = "mcbm_beam_2022_05_23_nickel";

  const string& recoFile = Form("reco_%s_%d.root", setup.Data(), runId);
  // -----------------------------------------------------------------------


  // -----   EventBuilder Settings-----------------------------------------
  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {1};
  const Int_t eb_TriggerMaxNumberT0 {2};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {16};
  const Int_t eb_TriggerMinNumberTofLayers {4};
  const Int_t eb_TriggerMinNumberRich {5};
  // -----------------------------------------------------------------------


  // -----   TOF defaults --------------------------------------------------
  Int_t calMode      = 93;
  Int_t calSel       = 1;
  Int_t calSm        = 0;
  Int_t RefSel       = 0;
  Double_t dDeadtime = 50.;
  Int_t iSel2        = 20;

  // Tracking
  Int_t iSel           = 1;  // 910041;
  Int_t iTrackingSetup = 2;
  Int_t iGenCor        = 1;
  Double_t dScalFac    = 1.;
  Double_t dChi2Lim2   = 500.;
  Bool_t bUseSigCalib  = kFALSE;
  Int_t iCalOpt        = 0;
  Int_t iTrkPar        = 3;
  // -----------------------------------------------------------------------


  // -----   TOF Settings --------------------------------------------------
  TString cCalId = "490.100.5.0";
  if (runId >= 759) cCalId = "759.100.4.0";
  if (runId >= 812) cCalId = "831.100.4.0";
  if (runId >= 1588) cCalId = "1588.50.6.0";
  if (runId >= 2160) cCalId = "2160.50.4.0";
  if (runId >= 2352) cCalId = "2365.5.lxbk0600";
  Int_t iCalSet = 30040500;  // calibration settings
  if (runId >= 759) iCalSet = 10020500;
  if (runId >= 812) iCalSet = 10020500;
  if (runId >= 1588) iCalSet = 12002002;
  if (runId >= 2160) iCalSet = 700900500;
  if (runId >= 2352) iCalSet = 42032500;

  Double_t Tint           = 100.;  // coincidence time interval
  Int_t iTrackMode        = 2;     // 2 for TofTracker
  const Int_t iTofCluMode = 1;
  Bool_t doTofTracking    = kTRUE;
  // -----------------------------------------------------------------------


  // -----   Fair logger ---------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);
  // -----------------------------------------------------------------------

  TString myName  = "run_reco_mcbm_real";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup* geoSetup = CbmSetup::Instance();
  geoSetup->LoadSetup(setup);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  //CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  geoSetup->SetActive(ECbmModuleId::kMvd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kSts, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kMuch, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kRich, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kTrd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kTrd2d, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kPsd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kTof, kTRUE);
  // -----------------------------------------------------------------------

  //TString TofFileFolder = Form("/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/%s", cCalId.Data());
  //TString TofFileFolder = Form("/data/cbmroot/files/tofCal/mTofCriPar2/%s", cCalId.Data());
  TString TofFileFolder = Form("/data/cbmroot/files/tofCal/%s", cCalId.Data());

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();


  //-----------------------------------------------//
  //   TString FId    = cCalId;
  //   TString TofGeo = "v19b_mcbm"; //v18m_mCbm
  //   TString TofGeo = "v20a_mcbm"; //v18m_mCbm


  // ----- TOF digitisation parameters -------------------------------------
  TString geoTag;
  TString geoFile;
  if (geoSetup->IsActive(ECbmModuleId::kTof)) {
    geoSetup->GetGeoTag(ECbmModuleId::kTof, geoTag);
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    //TObjString* tofBdfFile = new TObjString("/lustre/cbm/users/adrian/cbmgit/cbmsource/parameters/tof/tof_v21c_mcbm.digibdf.par");
    parFileList->Add(tofBdfFile);
    //    parFileList->Add("/lustre/cbm/users/adrian/cbmgit/cbmsource/parameters/tof/tof_v21c_mcbm.digibdf.par");
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;

    geoFile = srcDir + "/macro/mcbm/data/" + setup + ".geo.root";
    //geoFile             = srcDir + "/macro/mcbm/data/mcbm_beam_2022_03_22_iron.geo.root";
    TFile* fgeo         = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile " << geoFile.Data() << endl;
      return;
    }
  }

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  gROOT->LoadMacro("save_hst.C");


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << digiFile << std::endl;
  // ------------------------------------------------------------------------

  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());


  // =========================================================================
  // ===                   Alignment Correction                            ===
  // =========================================================================
  // (Fairsoft Apr21p2 or newer is needed)


  TString alignmentMatrixFileName = "AlignmentMatrices_" + setup + ".root";
  if (alignmentMatrixFileName.Length() != 0) {
    std::cout << "-I- " << myName << ": Applying alignment for file " << alignmentMatrixFileName << std::endl;

    // Define the basic structure which needs to be filled with information
    // This structure is stored in the output file and later passed to the
    // FairRoot framework to do the (miss)alignment
    std::map<std::string, TGeoHMatrix>* matrices {nullptr};

    // read matrices from disk
    LOG(info) << "Filename: " << alignmentMatrixFileName;
    TFile* misalignmentMatrixRootfile = new TFile(alignmentMatrixFileName, "READ");
    if (misalignmentMatrixRootfile->IsOpen()) {
      gDirectory->GetObject("MisalignMatrices", matrices);
      misalignmentMatrixRootfile->Close();
    }
    else {
      LOG(error) << "Could not open file " << alignmentMatrixFileName << "\n Exiting";
      exit(1);
    }

    if (matrices) { run->AddAlignmentMatrices(*matrices); }
    else {
      LOG(error) << "Alignment required but no matrices found."
                 << "\n Exiting";
      exit(1);
    }
  }
  // ------------------------------------------------------------------------


  // --------------------event builder---------------------------------------
  CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

  //Choose between NoOverlap, MergeOverlap, AllowOverlap
  evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

  // Remove detectors where digis not found
  if (!geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  if (!geoSetup->IsActive(ECbmModuleId::kMuch)) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  if (!geoSetup->IsActive(ECbmModuleId::kPsd)) evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
  if (!geoSetup->IsActive(ECbmModuleId::kTrd)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  if (!geoSetup->IsActive(ECbmModuleId::kTrd2d)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd2D);
  if (!geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  if (!geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);

  // Set TOF as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetTof);
  // evBuildRaw->SetReferenceDetector(kRawEventBuilderDetT0);

  // evBuildRaw->AddDetector(kRawEventBuilderDetT0);
  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  evBuildRaw->SetTsParameters(0.0, 1.28e8, 0.0);

  if (geoSetup->IsActive(ECbmModuleId::kTof))
    evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);
  if (geoSetup->IsActive(ECbmModuleId::kTof))
    evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTofLayers);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0, eb_TriggerMinNumberT0);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0, eb_TriggerMaxNumberT0);
  if (geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);

  // evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, eb_TriggerMinNumberSts);
  // evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich, eb_TriggerMinNumberRich);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);

  evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0, -50, 150);
  if (geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, -50, 100);
  if (geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -50, 50);
  if (geoSetup->IsActive(ECbmModuleId::kTrd)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd, -200, 200);
  if (geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich, -50, 100);

  run->AddTask(evBuildRaw);
  // ------------------------------------------------------------------------


  // -----   Local reconstruction of RICH Hits ---------------
  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  hitProd->applyICDCorrection();
  run->AddTask(hitProd);
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in RICh -> Finding of Rings ---------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  TString cFname;
  switch (iTofCluMode) {
    case 1: {
      CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer", 0, 1);
      //      cFname=Form("/%s_set%09d_%02d_%01dtofClust.hst.root",cCalId.Data(),iCalSet,calMode,calSel);
      //      cFname = Form("/%s_set%09d_%02d_%01d_noWalk_tofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSel);
      cFname = Form("/%s_set%09d_%02d_%01dtofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSel);
      tofCluster->SetCalParFileName(TofFileFolder + cFname);
      tofCluster->SetCalMode(calMode);
      tofCluster->SetCalSel(calSel);
      tofCluster->SetCaldXdYMax(300.);            // geometrical matching window in cm
      tofCluster->SetCalCluMulMax(3.);            // Max Counter Cluster Multiplicity for filling calib histos
      tofCluster->SetCalRpc(calSm);               // select detector for calibration update
      tofCluster->SetTRefId(RefSel);              // reference trigger for offset calculation
      tofCluster->SetTotMax(20.);                 // Tot upper limit for walk corection
      tofCluster->SetTotMin(0.);                  // Tot lower limit for walk correction
      tofCluster->SetTotPreRange(5.);             // effective lower Tot limit  in ns from peak position
      tofCluster->SetTotMean(5.);                 // Tot calibration target value in ns
      tofCluster->SetMaxTimeDist(1.0);            // default cluster range in ns
      tofCluster->SetDelTofMax(50.);              // acceptance range for cluster distance in ns (!)
      tofCluster->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
      tofCluster->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
      tofCluster->SetEnableAvWalk(kFALSE);
      // tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
      tofCluster->SetYFitMin(1.E4);
      tofCluster->SetToDAv(0.04);
      tofCluster->SetIdMode(1);        // calibrate on module level
      tofCluster->SetTRefDifMax(2.0);  // in ns
      tofCluster->PosYMaxScal(0.75);   //in % of length
      Int_t iBRef    = iCalSet % 1000;
      Int_t iSet     = (iCalSet - iBRef) / 1000;
      Int_t iRSel    = 0;
      Int_t iRSelTyp = 0;
      Int_t iRSelSm  = 0;
      Int_t iRSelRpc = 0;
      iRSel          = iBRef;  // use diamond
      Int_t iRSelin  = iRSel;
      iRSelRpc       = iRSel % 10;
      iRSelTyp       = (iRSel - iRSelRpc) / 10;
      iRSelSm        = iRSelTyp % 10;
      iRSelTyp       = (iRSelTyp - iRSelSm) / 10;
      tofCluster->SetBeamRefId(iRSelTyp);  // define Beam reference counter
      tofCluster->SetBeamRefSm(iRSelSm);
      tofCluster->SetBeamRefDet(iRSelRpc);
      tofCluster->SetBeamAddRefMul(-1);
      tofCluster->SetBeamRefMulMax(3);
      Int_t iSel2in  = iSel2;
      Int_t iSel2Rpc = iSel2 % 10;
      iSel2          = (iSel2 - iSel2Rpc) / 10;
      Int_t iSel2Sm  = iSel2 % 10;
      iSel2          = (iSel2 - iSel2Sm) / 10;
      if (iSel2 > -1) {
        tofCluster->SetSel2Id(iSel2);
        tofCluster->SetSel2Sm(iSel2Sm);
        tofCluster->SetSel2Rpc(iSel2Rpc);
      }
      Int_t iRef    = iSet % 1000;
      Int_t iDut    = (iSet - iRef) / 1000;
      Int_t iDutRpc = iDut % 10;
      iDut          = (iDut - iDutRpc) / 10;
      Int_t iDutSm  = iDut % 10;
      iDut          = (iDut - iDutSm) / 10;
      tofCluster->SetDutId(iDut);
      tofCluster->SetDutSm(iDutSm);
      tofCluster->SetDutRpc(iDutRpc);

      Int_t iRefRpc = iRef % 10;
      iRef          = (iRef - iRefRpc) / 10;
      Int_t iRefSm  = iRef % 10;
      iRef          = (iRef - iRefSm) / 10;

      tofCluster->SetSelId(iRef);
      tofCluster->SetSelSm(iRefSm);
      tofCluster->SetSelRpc(iRefRpc);

      run->AddTask(tofCluster);
      std::cout << "-I- " << myName << ": Added task " << tofCluster->GetName() << std::endl;
    } break;

    default: {
      ;
    }
  }
  // -------------------------------------------------------------------------

  // =========================================================================
  // ===                   Tof Tracking                                    ===
  // =========================================================================

  cout << "<I> Initialize Tof tracker by ini_trks" << endl;
  TString cTrkFile = Form("%s/%s_tofFindTracks.hst.root", TofFileFolder.Data(), cCalId.Data());

  // -----   Local selection variables  --------------------------------------

  Int_t iRef    = iSel % 1000;
  Int_t iDut    = (iSel - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;
  Int_t iBucRpc = 0;

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================

  //if (doTofTracking)
  {
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
        tofTrackFinder->SetTxMean(0.21);  // mean slope dy/dz
        tofTrackFinder->SetTyMean(0.18);  // mean slope dy/dz
        tofTrackFinder->SetTxLIM(0.15);   // max slope dx/dz
        tofTrackFinder->SetTyLIM(0.18);   // max dev from mean slope dy/dz
        break;
    }

    CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
    TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
    tofTrackFinder->SetFitter(tofTrackFitter);
    CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
    tofFindTracks->UseFinder(tofTrackFinder);
    tofFindTracks->UseFitter(tofTrackFitter);
    tofFindTracks->SetCalOpt(iCalOpt);           // 1 - update offsets, 2 - update walk, 0 - bypass
    tofFindTracks->SetCorMode(iGenCor);          // valid options: 0,1,2,3,4,5,6, 10 - 19
    tofFindTracks->SetTtTarg(0.042);             // target value Mar2021, after T0 fix (double stack run 1058)
    tofFindTracks->SetCalParFileName(cTrkFile);  // Tracker parameter value file name
    tofFindTracks->SetBeamCounter(5, 0, 0);      // default beam counter
    tofFindTracks->SetR0Lim(20.);
    tofFindTracks->SetStationMaxHMul(30);  // Max Hit Multiplicity in any used station

    tofFindTracks->SetT0MAX(dScalFac);            // in ns
    tofFindTracks->SetSIGT(0.08);                 // default in ns
    tofFindTracks->SetSIGX(0.3);                  // default in cm
    tofFindTracks->SetSIGY(0.45);                 // default in cm
    tofFindTracks->SetSIGZ(0.05);                 // default in cm
    tofFindTracks->SetUseSigCalib(bUseSigCalib);  // ignore resolutions in CalPar file
    tofTrackFinder->SetSIGLIM(dChi2Lim2 * 2.);    // matching window in multiples of chi2
    tofTrackFinder->SetChiMaxAccept(dChi2Lim2);   // max tracklet chi2

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
        iNStations    = 32;
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
        tofFindTracks->SetStation(27, 9, 1, 0);
        tofFindTracks->SetStation(28, 9, 0, 1);
        tofFindTracks->SetStation(29, 9, 1, 1);
        tofFindTracks->SetStation(30, 6, 0, 0);
        tofFindTracks->SetStation(31, 6, 0, 1);
        break;

      case 11:  // for calibration mode of 2-stack & test counters
        iMinNofHits   = 4;
        iNStations    = 9;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 9, 0, 0);
        tofFindTracks->SetStation(2, 9, 1, 0);
        tofFindTracks->SetStation(3, 9, 0, 1);
        tofFindTracks->SetStation(4, 9, 1, 1);
        tofFindTracks->SetStation(5, 0, 3, 1);
        tofFindTracks->SetStation(6, 0, 4, 0);
        tofFindTracks->SetStation(7, 0, 3, 2);
        tofFindTracks->SetStation(8, 5, 0, 0);
        break;

      case 2:
        iMinNofHits   = 5;
        iNStations    = 28;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 0, 2, 2);
        tofFindTracks->SetStation(1, 0, 0, 2);
        tofFindTracks->SetStation(2, 0, 1, 2);
        tofFindTracks->SetStation(3, 0, 2, 1);
        tofFindTracks->SetStation(4, 0, 0, 1);
        tofFindTracks->SetStation(5, 0, 1, 1);
        tofFindTracks->SetStation(6, 0, 2, 3);
        tofFindTracks->SetStation(7, 0, 0, 3);
        tofFindTracks->SetStation(8, 0, 1, 3);
        tofFindTracks->SetStation(9, 0, 2, 0);
        tofFindTracks->SetStation(10, 0, 0, 0);
        tofFindTracks->SetStation(11, 0, 1, 0);
        tofFindTracks->SetStation(12, 0, 2, 4);
        tofFindTracks->SetStation(13, 0, 0, 4);
        tofFindTracks->SetStation(14, 0, 1, 4);
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

      case 4:  // for USTC evaluation (dut=910,911)
        iMinNofHits   = 4;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 0, 3, 1);
        tofFindTracks->SetStation(2, 9, 0, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 5, 0, 0);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 14:
        iMinNofHits   = 3;
        iNStations    = 15;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 0, 2, 2);
        tofFindTracks->SetStation(1, 0, 1, 2);
        tofFindTracks->SetStation(2, 0, 0, 2);
        tofFindTracks->SetStation(0, 0, 2, 1);
        tofFindTracks->SetStation(1, 0, 1, 1);
        tofFindTracks->SetStation(2, 0, 0, 1);
        tofFindTracks->SetStation(0, 0, 2, 0);
        tofFindTracks->SetStation(1, 0, 1, 0);
        tofFindTracks->SetStation(2, 0, 0, 0);
        tofFindTracks->SetStation(0, 0, 2, 3);
        tofFindTracks->SetStation(1, 0, 1, 3);
        tofFindTracks->SetStation(2, 0, 0, 3);
        tofFindTracks->SetStation(0, 0, 2, 4);
        tofFindTracks->SetStation(1, 0, 1, 4);
        tofFindTracks->SetStation(2, 0, 0, 4);
        break;

      case 5:  // for calibration of 2-stack and add-on counters (STAR2, BUC)
        iMinNofHits   = 3;
        iNStations    = 7;
        iNReqStations = 4;
        tofFindTracks->SetStation(6, 0, 4, 1);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 9, 0, 0);
        tofFindTracks->SetStation(3, 9, 0, 1);
        tofFindTracks->SetStation(4, 6, 0, 0);
        tofFindTracks->SetStation(5, 0, 3, 1);
        tofFindTracks->SetStation(0, 5, 0, 0);
        break;

      case 6:  // for double stack USTC counter evaluation
        iMinNofHits   = 5;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 0, 4, 1);
        tofFindTracks->SetStation(3, 6, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 7:  // for double stack USTC counter evaluation
        iMinNofHits   = 3;
        iNStations    = 4;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 6, 0, 0);
        tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
        break;

      case 8:  // evaluation of add-on counters (BUC)
        iMinNofHits   = 5;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 9, 0, 1);
        tofFindTracks->SetStation(2, 0, 4, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 9:  // calibration of Star2
        iMinNofHits   = 4;
        iNStations    = 5;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(2, 9, 0, 1);
        tofFindTracks->SetStation(1, 0, 4, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        break;

      case 10:
        iMinNofHits   = 3;
        iNStations    = 4;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(3, 0, 1, 2);
        tofFindTracks->SetStation(2, 0, 0, 2);
        tofFindTracks->SetStation(1, 0, 2, 2);
        break;

      default:
        cout << "Tracking setup " << iTrackingSetup << " not implemented " << endl;
        return 1;
        ;
    }
    tofFindTracks->SetMinNofHits(iMinNofHits);
    tofFindTracks->SetNStations(iNStations);
    tofFindTracks->SetNReqStations(iNReqStations);
    tofFindTracks->PrintSetup();
    std::cout << "MinNofHitsPerTrack: " << iMinNofHits << std::endl;
    run->AddTask(tofFindTracks);
  }
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===                           RICH QA                                 ===
  // =========================================================================

  CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
  if (taskId < 0) { qaTask->SetOutputDir(Form("result_run%d", runId)); }
  else {
    qaTask->SetOutputDir(Form("result_run%d_%05d", runId, taskId));
  }
  //qaTask->XOffsetHistos(+25.0);
  qaTask->XOffsetHistos(-4.1);
  if (runId > 2351) qaTask->XOffsetHistos(0.0);
  qaTask->SetMaxNofDrawnEvents(100);
  qaTask->SetTotRich(23.7, 30.0);
  qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  qaTask->SetTriggerTofHits(0);  // eb_TriggerMinNumberTof);
  qaTask->SetSEDisplayRingOnly();
  run->AddTask(qaTask);
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  Bool_t kParameterMerged    = kTRUE;
  FairParRootFileIo* parIo1  = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  //  parIo1->open(parFile.c_str(), "UPDATE");
  parIo2->open(parFileList, "in");
  parIo2->print();
  rtdb->setFirstInput(parIo2);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Database update   ----------------------------------------------
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  rtdb->printParamContexts();
  // ------------------------------------------------------------------------


  //--- House Keeping -------------------------------------------------------
  // print all important infos in a file
  std::ofstream outfile;
  if (taskId < 0) { outfile.open(Form("result_run%d/run_info.dat", runId)); }
  else {
    outfile.open(Form("result_run%d_%05d/run_info.dat", runId, taskId));
  }
  // write inputted data into the file.
  outfile << "Run: " << runId << std::endl;
  outfile << "Events: " << nEvents << std::endl;
  //  outfile << "parFile: " << parFile << std::endl;
  outfile << "digiFile: " << digiFile << std::endl;
  outfile << "recoFile: " << recoFile << std::endl;
  //outfile << "Geometry: " << geoFile << std::endl;
  outfile << "TrackCalParFile: " << TofFileFolder << cCalId << std::endl;
  outfile << "TofClusterFile :" << TofFileFolder + cFname << std::endl;
  outfile << "TofOutput :" << cFname << std::endl << std::endl;
  outfile << "Trigger:" << std::endl;
  outfile << "  fixedTimeWindow :" << eb_fixedTimeWindow << std::endl;
  outfile << "  MinNumberT0   :" << eb_TriggerMinNumberT0 << std::endl;
  outfile << "  MinNumberSts  :" << eb_TriggerMinNumberSts << std::endl;
  outfile << "  MinNumberMuch :" << eb_TriggerMinNumberMuch << std::endl;
  outfile << "  MinNumberTof  :" << eb_TriggerMinNumberTof << std::endl;
  outfile << "  MinNumberRich :" << eb_TriggerMinNumberRich << std::endl;
  outfile.close();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  //  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;

  // -----   Resource monitoring   ------------------------------------------
  //     if ( hasFairMonitor /*Has_Fair_Monitor()*/ ) {      // FairRoot Version >= 15.11
  //         // Extract the maximal used memory an add is as Dart measurement
  //         // This line is filtered by CTest and the value send to CDash
  //         FairSystemInfo sysInfo;
  //         Float_t maxMemory=sysInfo.GetMaxMemory();
  //         std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  //         std::cout << maxMemory;
  //         std::cout << "</DartMeasurement>" << std::endl;
  //
  //         Float_t cpuUsage=ctime/rtime;
  //         std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  //         std::cout << cpuUsage;
  //         std::cout << "</DartMeasurement>" << std::endl;
  //
  //         FairMonitor* tempMon = FairMonitor::GetMonitor();
  //         tempMon->Print();
  //     }
}

void save_hst(TString cstr = "status.hst.root", Bool_t bROOT = kFALSE)
{
  cout << "save all histograms to file " << cstr.Data() << endl;
  TList* tList(NULL);
  if (bROOT) { tList = gROOT->GetList(); }
  else {
    tList = gDirectory->GetList();
  }
  TIter next(tList);
  // Write objects to the file
  TFile* fHist = new TFile(cstr, "RECREATE");
  {
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class()) || obj->InheritsFrom(TEfficiency::Class())) { obj->Write(); }
    }
  }
  // fHist->ls();
  fHist->Close();
}
