/** @file run_reco_mcbm_real_wToF.C
  * @copyright Copyright (C) 2019-2021 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Adrian Amatus Weber [committer] **/

void run_reco_mcbm_real_wToF(
  //     const string& parFile = "/lustre/nyx/cbm/users/adrian/data/159/10kTS/unp_mcbm_params_159.root",
  //const string& digiFile = "/lustre/nyx/cbm/users/adrian/data/159/10kTS/unp_mcbm_159.root",
  const string& parFile = "/lustre/nyx/cbm/users/adrian/data/159_corr/10kTS/unp_mcbm_params_159.root",
  //const string& digiFile = "/lustre/nyx/cbm/users/adrian/data/159_corr/10kTS/unp_mcbm_159.root",  //this was always used
  const string& digiFile = "/lustre/nyx/cbm/users/adrian/data/159_corr/all/unp_mcbm_159.root",
  //     const string& digiFile = "/lustre/nyx/cbm/users/adrian/data/159_corr_final/1kTS/unp_mcbm_159.root",     //Test 30.7.2019
  //const string& digiFile = "/lustre/nyx/cbm/users/ploizeau/mcbm2018/mcbm_all/data/unpacked/unp_mcbm_159_10kTs_withoffsets.root",
  //const string& parFile  = "/lustre/nyx/cbm/users/adrian/data/159_test/unp_mcbm_params_159.root",
  //const string& digiFile = "/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/data/159.200.10.1.0.root",
  const string& recoFile = "reco_mcbm.root", int nEvents = 0)
{

  //ToF Settings
  Int_t calMode        = 93;
  Int_t calSel         = 1;
  Int_t calSm          = 40;
  Int_t RefSel         = 1;
  TString cFileId      = "159.200.10.0.0";
  Int_t iCalSet        = 10020500;  //30040500;
  Int_t iTrackingSetup = 1;
  Int_t iSel2          = 0;
  Double_t dDeadtime   = 50;

  void save_hst(TString cstr, Bool_t bROOT);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);

  TString myName  = "run_reco_mcbm_real";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  TString TofFileFolder = Form("/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/%s", cFileId.Data());

  //    TString setupFile = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  //    TString setupFunct = "setup_" + geoSetup + "()";
  //    gROOT->LoadMacro(setupFile);
  //    gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  //-----------------------------------------------//
  TString FId    = cFileId;
  TString TofGeo = "v19a_mcbm";  //v18m_mCbm

  TObjString* tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par");  // TOF digi file
  std::cout << std::endl
            << "-I- digi.par file : " << workDir << "/parameters/tof/tof_" << TofGeo << ".digi.par" << std::endl;
  parFileList->Add(tofDigiFile);

  TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
  std::cout << std::endl
            << "-I- digibdf.par file : " << workDir << "/parameters/tof/" << TofGeo << ".digibdf.par" << std::endl;
  parFileList->Add(tofDigiBdfFile);

  TString geoDir = workDir;  // gSystem->Getenv("VMCWORKDIR");
  //TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
  //TString geoFile = "/lustre/nyx/cbm/users/adrian/data/sim/rich/mcbm/sis18_mcbm_25deg_long_geofile_full.root"; //18m
  TString geoFile = "/lustre/nyx/cbm/users/adrian/cbmsource/macro/mcbm/data/"
                    "mcbm_beam_2019_03.geo.root";
  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (NULL == geoMan) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }

  //-----------------------------------------------//

  gROOT->LoadMacro("save_hst.C");

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
  eventBuilder->SetTriggerMinNumberRich(1);

  run->AddTask(eventBuilder);

  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  run->AddTask(hitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);

  // Starting with ToF

  CbmTofEventClusterizer* tofClust = new CbmTofEventClusterizer("TOF Event Clusterizer", 1, 0);
  tofClust->SetCalMode(calMode);
  tofClust->SetCalSel(calSel);
  tofClust->SetCaldXdYMax(10.);             // geometrical matching window in cm
  tofClust->SetCalCluMulMax(3.);            // Max Counter Cluster Multiplicity for filling calib histos
  tofClust->SetCalRpc(calSm);               // select detector for calibration update
  tofClust->SetTRefId(RefSel);              // reference trigger for offset calculation
  tofClust->SetTotMax(20.);                 // Tot upper limit for walk corection
  tofClust->SetTotMin(0.01);                //(12000.);  // Tot lower limit for walk correction
  tofClust->SetTotPreRange(5.);             // effective lower Tot limit  in ns from peak position
  tofClust->SetTotMean(5.);                 // Tot calibration target value in ns
  tofClust->SetMaxTimeDist(1.0);            // default cluster range in ns
  tofClust->SetDelTofMax(10.);              // acceptance range for cluster distance in ns (!)
  tofClust->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
  tofClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofClust->SetEnableAvWalk(kFALSE);
  //tofClust->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
  tofClust->SetYFitMin(1.E4);
  tofClust->SetIdMode(1);  // calibrate on module level
  //   tofClust->SetDeadStrips(15,23);   // declare dead strip for T0M3,Rpc0,Strip 23


  Int_t calSelRead = calSel;
  if (calSel < 0) calSelRead = 0;
  TString cCalibFname = Form("/%s_set%09d_%02d_%01dtofClust.hst.root", cFileId.Data(), iCalSet, calMode, calSelRead);
  tofClust->SetCalParFileName(TofFileFolder + cCalibFname);
  TString cOutFname = Form("tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  switch (calMode) {
    case -1:                      // initial check of raw data
      tofClust->SetTotMax(256.);  // range in bin number
      tofClust->SetTotPreRange(256.);
      //tofClust->SetTotMin(1.);
      tofClust->SetTRefDifMax(26000.);  // in ns
      tofClust->PosYMaxScal(10000.);    // in % of length
      tofClust->SetMaxTimeDist(0.);     // no cluster building
      //tofClust->SetTimePeriod(25600.);       // inspect coarse time
      break;
    case 0:                       // initial calibration
      tofClust->SetTotMax(256.);  // range in bin number
      tofClust->SetTotPreRange(256.);
      //tofClust->SetTotMin(1.);
      tofClust->SetTRefDifMax(1000.);  // in ns
      tofClust->PosYMaxScal(10.);      // in % of length
      tofClust->SetMaxTimeDist(0.);    // no cluster building
      break;
    case 1:                       // save offsets, update walks, for diamonds
      tofClust->SetTotMax(256.);  // range in bin number
      tofClust->SetTotPreRange(256.);
      tofClust->SetTRefDifMax(6.25);  // in ns
      //tofClust->SetTimePeriod(6.25);       // inspect coarse time
      tofClust->PosYMaxScal(10.);  // in % of length
      break;
    case 11:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(3.0);   // in % of length
      break;
    case 21:
      tofClust->SetTRefDifMax(2.5);  // in ns
      tofClust->PosYMaxScal(2.0);    // in % of length
      break;
    case 31:
      tofClust->SetTRefDifMax(2.);  // in ns
      tofClust->PosYMaxScal(1.5);   // in % of length
      break;
    case 41:
      tofClust->SetTRefDifMax(1.5);  // in ns
      tofClust->PosYMaxScal(0.8);    // in % of length
      break;
    case 51:
      tofClust->SetTRefDifMax(1.2);  // in ns
      tofClust->PosYMaxScal(0.7);    // in % of length
      break;
    case 61:
      tofClust->SetTRefDifMax(1.0);  // in ns
      tofClust->PosYMaxScal(0.75);   // in % of length
      break;
    case 71:
      tofClust->SetTRefDifMax(0.8);  // in ns
      tofClust->PosYMaxScal(0.6);    // in % of length
      break;

    case 2:                           // time difference calibration
      tofClust->SetTRefDifMax(300.);  // in ns
      tofClust->PosYMaxScal(1000.);   //in % of length
      break;

    case 3:                           // time offsets
      tofClust->SetTRefDifMax(200.);  // in ns
      tofClust->PosYMaxScal(100.);    //in % of length
      tofClust->SetMaxTimeDist(0.);   // no cluster building
      break;
    case 12:
    case 13:
      tofClust->SetTRefDifMax(100.);  // in ns
      tofClust->PosYMaxScal(10.);     //in % of length
      break;
    case 22:
    case 23:
      tofClust->SetTRefDifMax(50.);  // in ns
      tofClust->PosYMaxScal(5.);     //in % of length
      break;
    case 32:
    case 33:
      tofClust->SetTRefDifMax(25.);  // in ns
      tofClust->PosYMaxScal(4.);     //in % of length
      break;
    case 42:
    case 43:
      tofClust->SetTRefDifMax(12.);  // in ns
      tofClust->PosYMaxScal(2.);     //in % of length
      break;
    case 52:
    case 53:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 62:
    case 63:
      tofClust->SetTRefDifMax(3.);  // in ns
      tofClust->PosYMaxScal(1.);    //in % of length
      break;
    case 72:
    case 73:
      tofClust->SetTRefDifMax(2.5);  // in ns
      tofClust->PosYMaxScal(0.9);    //in % of length
      break;
    case 82:
    case 83:
      tofClust->SetTRefDifMax(2.0);  // in ns
      tofClust->PosYMaxScal(0.8);    //in % of length
      break;
    case 92:
    case 93:
      tofClust->SetTRefDifMax(1.5);  // in ns
      tofClust->PosYMaxScal(0.75);   //in % of length
      break;

    case 4:  // velocity dependence (DelTOF)
    case 14:
      tofClust->SetTRefDifMax(25.);  // in ns
      tofClust->PosYMaxScal(2.0);    //in % of length
      break;
    case 24:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 34:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(1.2);   //in % of length
      break;
    case 44:
      tofClust->SetTRefDifMax(3.);  // in ns
      tofClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 54:
      tofClust->SetTRefDifMax(2.5);  // in ns
      tofClust->PosYMaxScal(0.9);    //in % of length
      break;
    case 64:
      tofClust->SetTRefDifMax(2.5);  // in ns
      tofClust->PosYMaxScal(0.8);    //in % of length
      break;
    case 74:
      tofClust->SetTRefDifMax(1.5);  // in ns
      tofClust->PosYMaxScal(0.7);    //in % of length
      break;
    default: cout << "<E> Calib mode not implemented! stop execution of script" << endl; return;
  }

  run->AddTask(tofClust);

  Int_t iBRef    = iCalSet % 1000;
  Int_t iSet     = (iCalSet - iBRef) / 1000;
  Int_t iRSel    = 0;
  Int_t iRSelTyp = 0;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  iRSel          = iBRef;  // use diamond
  if (iSel2 == 0) {
    // iSel2=iBRef;
  }
  else {
    if (iSel2 < 0) iSel2 = -iSel2;
  }

  Int_t iRSelin = iRSel;
  iRSelRpc      = iRSel % 10;
  iRSelTyp      = (iRSel - iRSelRpc) / 10;
  iRSelSm       = iRSelTyp % 10;
  iRSelTyp      = (iRSelTyp - iRSelSm) / 10;

  tofClust->SetBeamRefId(iRSelTyp);  // define Beam reference counter
  tofClust->SetBeamRefSm(iRSelSm);
  tofClust->SetBeamRefDet(iRSelRpc);
  tofClust->SetBeamAddRefMul(-1);
  tofClust->SetBeamRefMulMax(3);

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;
  if (iSel2 > 0) {
    tofClust->SetSel2Id(iSel2);
    tofClust->SetSel2Sm(iSel2Sm);
    tofClust->SetSel2Rpc(iSel2Rpc);
  }

  Int_t iRef    = iSet % 1000;
  Int_t iDut    = (iSet - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;

  tofClust->SetDutId(iDut);
  tofClust->SetDutSm(iDutSm);
  tofClust->SetDutRpc(iDutRpc);

  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  tofClust->SetSelId(iRef);
  tofClust->SetSelSm(iRefSm);
  tofClust->SetSelRpc(iRefRpc);

  cout << "Run mTof Clusterizer with iRSel = " << iRSel << ", iSel2 = " << iSel2in << endl;

  //######################################################################

  // mTof Tracker Initialization
  TString cTrkFile                  = Form("/%s_tofFindTracks.hst.root", cFileId.Data());
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
  Int_t iGenCor = 1;
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  tofFindTracks->SetTtTarg(0.057);     // target value for inverse velocity, > 0.033 ns/cm!
  //tofFindTracks->SetTtTarg(0.035);                // target value for inverse velocity, > 0.033 ns/cm!
  tofFindTracks->SetCalParFileName(TofFileFolder + cTrkFile);  // Tracker parameter value file name
  tofFindTracks->SetBeamCounter(5, 0, 0);                      // default beam counter
  tofFindTracks->SetStationMaxHMul(30);                        // Max Hit Multiplicity in any used station

  tofFindTracks->SetT0MAX(1.);                 // in ns
  tofFindTracks->SetSIGT(0.08);                // default in ns
  tofFindTracks->SetSIGX(0.3);                 // default in cm
  tofFindTracks->SetSIGY(0.6);                 // default in cm
  tofFindTracks->SetSIGZ(0.05);                // default in cm
  tofFindTracks->SetUseSigCalib(kFALSE);       // ignore resolutions in CalPar file
  Double_t dChi2Lim2 = 3.5;                    //100;//3.5;
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
      iNReqStations = 4;
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

    default:;
  }

  tofFindTracks->SetMinNofHits(iMinNofHits);
  tofFindTracks->SetNStations(iNStations);
  tofFindTracks->SetNReqStations(iNReqStations);
  tofFindTracks->PrintSetup();
  run->AddTask(tofFindTracks);

  //######################################################################

  CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
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
