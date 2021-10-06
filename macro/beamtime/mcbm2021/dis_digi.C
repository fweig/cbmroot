/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void dis_digi(Int_t nEvents = 100, Int_t calMode = 93, Int_t calSel = 1, Int_t calSm = 0, Int_t RefSel = 1,
              TString cFileId = "68.50.7.1", Int_t iCalSet = 10500, Bool_t bOut = 0, Int_t iSel2 = 20,
              Double_t dDeadtime = 50, Int_t iGenCor = 1, Int_t iTrackingSetup = 1, Double_t dScalFac = 5.,
              Double_t dChi2Lim2 = 10., TString cCalId = "XXX", Bool_t bUseSigCalib = kFALSE, Int_t iCalOpt = 1)
{

  Int_t iVerbose = 1;
  if (cCalId == "") cCalId = cFileId;
  TString FId = cFileId;
  Int_t iNLen = FId.First(".");
  TString cRun(FId(0, iNLen));
  Int_t iRun = cRun.Atoi();
  cout << "dis_digi for Run " << iRun << endl;

  //Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel);
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  /*
   TString workDir    = (TString)gInterpreter->ProcessLine(".! pwd");
   cout << "workdir = "<< workDir.Data() << endl;
   return;
  */
  //   TString paramDir   = workDir + "/macro/beamtime/mcbm2019/";
  TString paramDir   = "./";
  TString ParFile    = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile  = paramDir + "data/" + cFileId + ".root";
  TString OutputFile = paramDir + "data/disdigi_" + cFileId + Form("_%09d%03d", iCalSet, iSel2) + ".out.root";

  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cFileId.Data());

  TList* parFileList = new TList();


  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  Int_t iGeo = 0;  //iMc;
  if (iGeo == 0) {
    if (iRun < 690) TofGeo = "v20a_mcbm";
    else {
      if (iRun < 1112) { TofGeo = "v21a_mcbm"; }
      else {
        if (iRun < 1400) { TofGeo = "v21b_mcbm"; }
        else {
          TofGeo = "v21d_mcbm";
        }
      }
    }	  

    cout << "Geometry version " << TofGeo << endl;
    /*  
    TObjString* tofDigiFile = new TObjString(
      workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par");  // TOF digi file
    parFileList->Add(tofDigiFile);
*/
    TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
    parFileList->Add(tofDigiBdfFile);

    TString geoDir      = gSystem->Getenv("VMCWORKDIR");
    TString geoFile     = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
    TFile* fgeo         = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile" << endl;
      return;
    }

    if (0) {
      TGeoVolume* master = geoMan->GetTopVolume();
      master->SetVisContainers(1);
      master->Draw("ogl");
    }
  }

  // Local steering variables
  Int_t iBRef    = iCalSet % 1000;
  Int_t iSet     = (iCalSet - iBRef) / 1000;
  Int_t iRSel    = 0;
  Int_t iRSelTyp = 0;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  if (iSel2 == 0) {
    iRSel = iBRef;  // use diamond
    iSel2 = iBRef;
  }
  else {
    if (iSel2 < 0) iSel2 = -iSel2;
    iRSel = iSel2;
  }

  iRSelRpc = iRSel % 10;
  iRSelTyp = (iRSel - iRSelRpc) / 10;
  iRSelSm  = iRSelTyp % 10;
  iRSelTyp = (iRSelTyp - iRSelSm) / 10;

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;

  Int_t iRef    = iSet % 1000;
  Int_t iDut    = (iSet - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;

  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(InputFile.Data());
  //run->SetOutputFile(OutputFile);
  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

  CbmTofEventClusterizer* tofClust = new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);

  tofClust->SetCalMode(calMode);
  tofClust->SetCalSel(calSel);
  tofClust->SetCaldXdYMax(50.);   // geometrical matching window in cm
  tofClust->SetCalCluMulMax(4.);  // Max Counter Cluster Multiplicity for filling calib histos
  tofClust->SetCalRpc(calSm);     // select detector for calibration update
  tofClust->SetTRefId(RefSel);    // reference trigger for offset calculation
  tofClust->SetTotMax(20.);       // Tot upper limit for walk corection
  tofClust->SetTotMin(0.01);      //(12000.);  // Tot lower limit for walk correction
  tofClust->SetTotPreRange(2.);   // effective lower Tot limit  in ns from peak position
  tofClust->SetTotMean(2.);       // Tot calibration target value in ns
  tofClust->SetMaxTimeDist(1.0);  // default cluster range in ns
  //tofClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ns
  tofClust->SetDelTofMax(60.);              // acceptance range for cluster correlation in cm (!)
  tofClust->SetSel2MulMax(4);               // limit Multiplicity in 2nd selector
  tofClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofClust->SetEnableAvWalk(kTRUE);
  tofClust->SetYFitMin(1.E4);
  //tofClust->SetTimePeriod(6.25);       // ignore coarse time
  //tofClust->SetCorMode(2);              // correct missing hits

  Int_t calSelRead = calSel;
  if (calSel < 0) calSelRead = 0;
  TString cFname = Form("%s_set%09d_%02d_%01dtofClust.hst.root", cFileId.Data(), iCalSet, calMode, calSelRead);
  tofClust->SetCalParFileName(cFname);
  TString cOutFname = Form("tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  TString cAnaFile = Form("%s_%09d%03d_tofAna.hst.root", cFileId.Data(), iCalSet, iSel2);

  switch (calMode) {
    case -1:                      // initial calibration
      tofClust->SetTotMax(256.);  // range in bin number
      tofClust->SetTotPreRange(256.);
      //tofClust->SetTotMin(1.);
      tofClust->SetTRefDifMax(50000.);  // in ns
      tofClust->PosYMaxScal(10000.);    // in % of length
      tofClust->SetMaxTimeDist(0.);     // no cluster building
      //tofClust->SetTimePeriod(0.);       // inspect coarse time
      break;
    case 0:                       // initial calibration
      tofClust->SetTotMax(256.);  // range in bin number
      tofClust->SetTotPreRange(256.);
      //tofClust->SetTotMin(1.);
      tofClust->SetTRefDifMax(50.);  // in ns
      tofClust->PosYMaxScal(10.);    // in % of length
      tofClust->SetMaxTimeDist(0.);  // no cluster building
      //tofClust->SetTimePeriod(0.);       // inspect coarse time
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
      tofClust->SetTRefDifMax(1.);  // in ns
      tofClust->PosYMaxScal(0.8);   // in % of length
      break;
    case 51:
      tofClust->SetTRefDifMax(0.7);  // in ns
      tofClust->PosYMaxScal(0.7);    // in % of length
      break;
    case 61:
      tofClust->SetTRefDifMax(0.5);  // in ns
      tofClust->PosYMaxScal(0.7);    // in % of length
      break;
    case 71:
      tofClust->SetTRefDifMax(0.4);  // in ns
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
      tofClust->PosYMaxScal(50.);     //in % of length
      break;
    case 22:
    case 23:
      tofClust->SetTRefDifMax(50.);  // in ns
      tofClust->PosYMaxScal(20.);    //in % of length
      break;
    case 32:
    case 33:
      tofClust->SetTRefDifMax(25.);  // in ns
      tofClust->PosYMaxScal(10.);    //in % of length
      break;
    case 42:
    case 43:
      tofClust->SetTRefDifMax(12.);  // in ns
      tofClust->PosYMaxScal(5.);     //in % of length
      break;
    case 52:
    case 53:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(3.);    //in % of length
      break;
    case 62:
    case 63:
      tofClust->SetTRefDifMax(3.);  // in ns
      tofClust->PosYMaxScal(2.);    //in % of length
      break;
    case 72:
    case 73:
      tofClust->SetTRefDifMax(2.);  // in ns
      tofClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 82:
    case 83:
      tofClust->SetTRefDifMax(1.);  // in ns
      tofClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 92:
    case 93:
      tofClust->SetTRefDifMax(0.6);  // in ns
      tofClust->PosYMaxScal(1.0);    //in % of length
      break;

    case 4:                         // velocity dependence (DelTOF)
      tofClust->SetTRefDifMax(6.);  // in ns
      tofClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 14:
      tofClust->SetTRefDifMax(5.);  // in ns
      tofClust->PosYMaxScal(1.);    //in % of length
      break;
    case 24:
      tofClust->SetTRefDifMax(3.);  // in ns
      tofClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 34:
      tofClust->SetTRefDifMax(2.);  // in ns
      tofClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 44:
      tofClust->SetTRefDifMax(1.);  // in ns
      tofClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 54:
      tofClust->SetTRefDifMax(0.7);  // in ns
      tofClust->PosYMaxScal(0.7);    //in % of length
      break;
    case 64:
      tofClust->SetTRefDifMax(0.5);  // in ns
      tofClust->PosYMaxScal(0.7);    //in % of length
      break;
    default: cout << "<E> Calib mode not implemented! stop execution of script" << endl; return;
  }

  run->AddTask(tofClust);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================

  CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
  tofTrackFinder->SetMaxTofTimeDifference(0.4);  // in ns/cm
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
  tofFindTracks->SetCalOpt(iCalOpt);   // 1 - update offsets, 2 - update walk, 0 - bypass
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  tofFindTracks->SetTtTarg(0.057);     // target value for inverse velocity, > 0.033 ns/cm!
  //tofFindTracks->SetTtTarg(0.035);            // target value for inverse velocity, > 0.033 ns/cm!
  tofFindTracks->SetCalParFileName(cTrkFile);  // Tracker parameter value file name
  tofFindTracks->SetBeamCounter(5, 0, 0);      // default beam counter
  tofFindTracks->SetR0Lim(100.);

  tofFindTracks->SetStationMaxHMul(30);  // Max Hit Multiplicity in any used station

  tofFindTracks->SetT0MAX(dScalFac);            // in ns
  tofFindTracks->SetSIGT(0.08);                 // default in ns
  tofFindTracks->SetSIGX(0.3);                  // default in cm
  tofFindTracks->SetSIGY(0.6);                  // default in cm
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
      iNStations    = 30;
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
      //tofFindTracks->SetStation(28, 6, 0, 0);
      //tofFindTracks->SetStation(29, 6, 0, 1);
      break;

    case 11:  // for calibration mode of 2-stack & test counters
      iMinNofHits   = 4;
      iNStations    = 9;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 0, 4, 0);
      tofFindTracks->SetStation(4, 0, 3, 2);
      tofFindTracks->SetStation(5, 9, 0, 0);
      tofFindTracks->SetStation(6, 9, 1, 0);
      tofFindTracks->SetStation(7, 9, 0, 1);
      tofFindTracks->SetStation(8, 9, 1, 1);
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

      tofFindTracks->SetStation(15, 5, 0, 0);
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
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
      break;

    case 5:  // for calibration of 2-stack and add-on counters (STAR2, BUC)
      iMinNofHits   = 3;
      iNStations    = 5;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 9, 0, 0);
      tofFindTracks->SetStation(4, 9, 0, 1);
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
      iMinNofHits   = 4;
      iNStations    = 5;
      iNReqStations = 5;
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

    default:
      cout << "Tracking setup " << iTrackingSetup << " not implemented " << endl;
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

  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis", iVerbose);

  //CbmTofAnaTestbeam defaults
  tofAnaTestbeam->SetReqTrg(0);  // 0 - no selection
  tofAnaTestbeam->SetDXMean(0.);
  tofAnaTestbeam->SetDYMean(0.);
  tofAnaTestbeam->SetDTMean(0.);  // in ps
  tofAnaTestbeam->SetDXWidth(0.4);
  tofAnaTestbeam->SetDYWidth(0.4);
  tofAnaTestbeam->SetDTWidth(80.);  // in ps
  tofAnaTestbeam->SetCalParFileName(cAnaFile);
  tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetDTDia(0.);        // Time difference to additional diamond
  tofAnaTestbeam->SetCorMode(RefSel);  // 1 - DTD4, 2 - X4
  tofAnaTestbeam->SetMul0Max(30);      // Max Multiplicity in dut
  tofAnaTestbeam->SetMul4Max(30);      // Max Multiplicity in Ref - RPC
  tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond
  tofAnaTestbeam->SetHitDistMin(30.);  // initialization
  tofAnaTestbeam->SetEnableMatchPosScaling(kTRUE);

  tofAnaTestbeam->SetPosYS2Sel(0.5);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);     // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);  // Width  of channel selection window
  tofAnaTestbeam->SetTShift(0.);      // Shift DTD4 to 0
  tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
  tofAnaTestbeam->SetTOffD4(13.);     // Shift DTD4 to physical value

  tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);  // common reaction reference
  tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
  tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);

  if (iSel2 > -1) {
    tofClust->SetSel2Id(iSel2);
    tofClust->SetSel2Sm(iSel2Sm);
    tofClust->SetSel2Rpc(iSel2Rpc);

    tofAnaTestbeam->SetMrpcSel2(iSel2);        // initialization of second selector Mrpc Type
    tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);    // initialization of second selector Mrpc SmId
    tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);  // initialization of second selector Mrpc RpcId
  }

  tofClust->SetDutId(iDut);
  tofClust->SetDutSm(iDutSm);
  tofClust->SetDutRpc(iDutRpc);

  tofClust->SetSelId(iRef);
  tofClust->SetSelSm(iRefSm);
  tofClust->SetSelRpc(iRefRpc);

  tofAnaTestbeam->SetDut(iDut);            // Device under test
  tofAnaTestbeam->SetDutSm(iDutSm);        // Device under test
  tofAnaTestbeam->SetDutRpc(iDutRpc);      // Device under test
  tofAnaTestbeam->SetMrpcRef(iRef);        // Reference RPC
  tofAnaTestbeam->SetMrpcRefSm(iRefSm);    // Reference RPC
  tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);  // Reference RPC

  tofAnaTestbeam->SetChi2Lim(10.);  // initialization of Chi2 selection limit
  cout << "Run with iRSel = " << iRSel << ", iSel2 = " << iSel2in << endl;

  if (0) switch (iSet) {
      case 0:   // upper part of setup: P2 - P5
      case 3:   // upper part of setup: P2 - P5
      case 34:  // upper part of setup: P2 - P5
      case 400300:
        switch (iRSel) {
          case 4:
            tofAnaTestbeam->SetTShift(0.);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 5:
            tofAnaTestbeam->SetTShift(-3.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 9:
            tofAnaTestbeam->SetChi2Lim(100.);  // initialization of Chi2 selection limit
            tofAnaTestbeam->SetMulDMax(3);     // Max Multiplicity in BeamRef // Diamond
            tofAnaTestbeam->SetTShift(0.1);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);    // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.5);  // Shift Sel2 time peak to 0
            break;

          default:;
        }

      default:
        cout << "<E> detector setup " << iSet << " unknown, stop!" << endl;
        return;
        ;
    }  // end of different subsets

  run->AddTask(tofAnaTestbeam);
  // =========================================================================
  /*
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000);
   run->AddTask(display);   
   */
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

  FairEventManager* fMan = new FairEventManager();

  CbmEvDisTracks* Tracks = new CbmEvDisTracks("Tof Tracks", 1, kFALSE,
                                              kTRUE);  //name, verbosity, RnrChildren points, RnrChildren track
                                                       //  CbmEvDisTracks *Tracks =  new CbmEvDisTracks("Tof Tracks",1);
  fMan->AddTask(Tracks);
  CbmPixelHitSetDraw* TofUHits = new CbmPixelHitSetDraw("TofUHit", kRed, kOpenCross);
  fMan->AddTask(TofUHits);
  CbmPointSetArrayDraw* TofHits =
    new CbmPointSetArrayDraw("TofHit", 1, 1, 4,
                             kTRUE);  //name, colorMode, markerMode, verbosity, RnrChildren
  //  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle, 4);// kFullSquare);
  fMan->AddTask(TofHits);


  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(1);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for (Int_t i = 0; i < allvolumes->GetEntries(); i++) {
    TGeoVolume* vol = (TGeoVolume*) allvolumes->At(i);
    TString name    = vol->GetName();
    //    cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    vol->SetTransparency(90);
    /* switch (char *) not allowed any more in root 6 :(
    switch(name.Data()) {
    case "counter":
      vol->SetTransparency(95);
      break;

    case "tof_glass":
    case "Gap":
    case "Cell":
      vol->SetTransparency(99);
      break;

    case "pcb":
      vol->SetTransparency(30);
      break;

    default:
      vol->SetTransparency(96);
    }
    */
  }
  //  gGeoManager->SetVisLevel(3);
  //  top->SetTransparency(80);
  //  top->Draw("ogl");

  //  fMan->Init(1,4,10000);
  fMan->Init(1, 5);

  cout << "customize TEveManager gEve " << gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow - 10);
  TGLViewer* v       = gEve->GetDefaultGLViewer();
  TGLAnnotation* ann = new TGLAnnotation(v, cFileId, 0.01, 0.98);
  ann->SetTextSize(0.03);  // % of window diagonal
  ann->SetTextColor(4);

  //  gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);

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

  // -----   Intialise and run   --------------------------------------------
  // run->Init();
  //  cout << "Starting run" << endl;
  //  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  // default display
  /*
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct = "pl_over_Mat04D4best()";  
  gROOT->LoadMacro(Display_Status);

  gROOT->LoadMacro("fit_ybox.h");
  gROOT->LoadMacro("pl_all_CluMul.C");
  gROOT->LoadMacro("pl_all_CluRate.C");
  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gROOT->LoadMacro("save_hst.C");

  switch(iSet){
    default:
  case 0:
  case 3:
  case 49:
  case 79:
  case 34:
  case 94:
  case 37:
  case 97:
  case 39:
  case 99:
  case 93:
  case 300400:
  case 400300:
  case 910900:
  case 300900:
  case 400900:
  case 901900:
  case 921920:
  case 300921:
  case 920921:
  case 920300:
  case 921300:

    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(6,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(9,0,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(9,1,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,1,1)");
    gInterpreter->ProcessLine("pl_over_clu(9,2,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,2,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,1)");
    gInterpreter->ProcessLine("pl_all_CluMul()");
    gInterpreter->ProcessLine("pl_all_CluRate()");
    gInterpreter->ProcessLine("pl_all_dTSel()");
    TString FSave=Form("save_hst(\"cosdev-status%d_%d_Cal_%s.hst.root\")",iCalSet,iSel2in,cCalId.Data());
    gInterpreter->ProcessLine(FSave.Data());
    //gInterpreter->ProcessLine("pl_over_MatD4sel()");
    break;
    ;
  }
  gInterpreter->ProcessLine(Display_Funct);
  */
}
