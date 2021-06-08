/* Copyright (C) 2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void ana_digi_cos(Int_t nEvents = 10000000, Int_t calMode = 53, Int_t calSel = 0, Int_t calSm = 900, Int_t RefSel = 1,
                  TString cFileId = "Test", Int_t iCalSet = 910601600, Bool_t bOut = 0, Int_t iSel2 = 0,
                  Double_t dDeadtime = 50, TString cCalId = "XXX", Int_t iBugCor = 0)
{
  Int_t iVerbose = 1;
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
  gLogger->SetLogVerbosityLevel("MEDIUM");

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  /*
   TString workDir    = (TString)gInterpreter->ProcessLine(".! pwd");
   cout << "workdir = "<< workDir.Data() << endl;
   return;
  */
  TString paramDir   = workDir + "/macro/beamtime/mcbm2018/";
  TString ParFile    = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile  = paramDir + "data/" + cFileId + ".root";
  TString OutputFile = paramDir + "data/digidev_" + cFileId + Form("_%09d_%03d_%02.0f_Cal", iCalSet, iSel2, dDeadtime)
                       + cCalId + ".out.root";

  TList* parFileList = new TList();

  TString FId    = cFileId;
  TString TofGeo = "v18j_cosmicHD";
  cout << "Geometry version " << TofGeo << endl;

  TObjString* tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par");  // TOF digi file
  parFileList->Add(tofDigiFile);

  //   TObjString tofDigiBdfFile = new TObjString( paramDir + "/tof." + FPar + "digibdf.par");
  TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/" + TofGeo + ".digibdf.par");
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

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(InputFile.Data());
  //run->AddFriend(InputFile.Data());
  run->SetOutputFile(OutputFile);

  CbmTofCosmicClusterizer* tofCosmicClust = new CbmTofCosmicClusterizer("TOF Cosmic Clusterizer", iVerbose, bOut);

  tofCosmicClust->SetCalMode(calMode);
  tofCosmicClust->SetCalSel(calSel);
  tofCosmicClust->SetCaldXdYMax(30.);   // geometrical matching window in cm
  tofCosmicClust->SetCalCluMulMax(3.);  // Max Counter Cluster Multiplicity for filling calib histos
  tofCosmicClust->SetCalRpc(calSm);     // select detector for calibration update
  tofCosmicClust->SetTRefId(RefSel);    // reference trigger for offset calculation
  tofCosmicClust->SetTotMax(20.);       // Tot upper limit for walk corection
  tofCosmicClust->SetTotMin(0.01);      //(12000.);  // Tot lower limit for walk correction
  tofCosmicClust->SetTotPreRange(5.);   // effective lower Tot limit  in ns from peak position
  tofCosmicClust->SetTotMean(5.);       // Tot calibration target value in ns
  tofCosmicClust->SetMaxTimeDist(1.0);  // default cluster range in ns
  //tofCosmicClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ns
  tofCosmicClust->SetDelTofMax(60.);              // acceptance range for cluster distance in cm (!)
  tofCosmicClust->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
  tofCosmicClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofCosmicClust->SetEnableAvWalk(kFALSE);
  tofCosmicClust->SetYFitMin(1.E4);
  //   tofCosmicClust->SetTimePeriod(25600.);       // ignore coarse time
  tofCosmicClust->SetCorMode(iBugCor);  // correct missing hits

  Int_t calSelRead = calSel;
  if (calSel < 0) calSelRead = 0;
  TString cFname = Form("%s_set%09d_%02d_%01dtofCosmicClust.hst.root", cFileId.Data(), iCalSet, calMode, calSelRead);
  tofCosmicClust->SetCalParFileName(cFname);
  TString cOutFname = Form("tofCosmicClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofCosmicClust->SetOutHstFileName(cOutFname);

  TString cAnaFile = Form("%s_%09d%03d_tofAnaCosmic.hst.root", cFileId.Data(), iCalSet, iSel2);

  switch (calMode) {
    case -1:                            // initial check of raw data
      tofCosmicClust->SetTotMax(256.);  // range in bin number
      tofCosmicClust->SetTotPreRange(256.);
      //tofCosmicClust->SetTotMin(1.);
      tofCosmicClust->SetTRefDifMax(26000.);  // in ns
      tofCosmicClust->PosYMaxScal(10000.);    // in % of length
      tofCosmicClust->SetMaxTimeDist(0.);     // no cluster building
      //tofCosmicClust->SetTimePeriod(25600.);       // inspect coarse time
      break;
    case 0:                             // initial calibration
      tofCosmicClust->SetTotMax(256.);  // range in bin number
      tofCosmicClust->SetTotPreRange(256.);
      //tofCosmicClust->SetTotMin(1.);
      tofCosmicClust->SetTRefDifMax(50.);  // in ns
      tofCosmicClust->PosYMaxScal(10.);    // in % of length
      tofCosmicClust->SetMaxTimeDist(0.);  // no cluster building
      break;
    case 1:                             // save offsets, update walks, for diamonds
      tofCosmicClust->SetTotMax(256.);  // range in bin number
      tofCosmicClust->SetTotPreRange(256.);
      tofCosmicClust->SetTRefDifMax(6.25);  // in ns
      tofCosmicClust->SetTimePeriod(6.25);  // inspect coarse time
      tofCosmicClust->PosYMaxScal(10.);     // in % of length
      break;
    case 11:
      tofCosmicClust->SetTRefDifMax(5.);  // in ns
      tofCosmicClust->PosYMaxScal(3.0);   // in % of length
      break;
    case 21:
      tofCosmicClust->SetTRefDifMax(2.5);  // in ns
      tofCosmicClust->PosYMaxScal(2.0);    // in % of length
      break;
    case 31:
      tofCosmicClust->SetTRefDifMax(2.);  // in ns
      tofCosmicClust->PosYMaxScal(1.5);   // in % of length
      break;
    case 41:
      tofCosmicClust->SetTRefDifMax(1.);  // in ns
      tofCosmicClust->PosYMaxScal(0.8);   // in % of length
      break;
    case 51:
      tofCosmicClust->SetTRefDifMax(0.7);  // in ns
      tofCosmicClust->PosYMaxScal(0.7);    // in % of length
      break;
    case 61:
      tofCosmicClust->SetTRefDifMax(0.9);  // in ns
      tofCosmicClust->PosYMaxScal(0.75);   // in % of length
      break;
    case 71:
      tofCosmicClust->SetTRefDifMax(0.4);  // in ns
      tofCosmicClust->PosYMaxScal(0.6);    // in % of length
      break;

    case 2:                                 // time difference calibration
      tofCosmicClust->SetTRefDifMax(300.);  // in ns
      tofCosmicClust->PosYMaxScal(1000.);   //in % of length
      break;

    case 3:                                 // time offsets
      tofCosmicClust->SetTRefDifMax(200.);  // in ns
      tofCosmicClust->PosYMaxScal(100.);    //in % of length
      tofCosmicClust->SetMaxTimeDist(0.);   // no cluster building
      break;
    case 12:
    case 13:
      tofCosmicClust->SetTRefDifMax(100.);  // in ns
      tofCosmicClust->PosYMaxScal(50.);     //in % of length
      break;
    case 22:
    case 23:
      tofCosmicClust->SetTRefDifMax(50.);  // in ns
      tofCosmicClust->PosYMaxScal(20.);    //in % of length
      break;
    case 32:
    case 33:
      tofCosmicClust->SetTRefDifMax(25.);  // in ns
      tofCosmicClust->PosYMaxScal(10.);    //in % of length
      break;
    case 42:
    case 43:
      tofCosmicClust->SetTRefDifMax(12.);  // in ns
      tofCosmicClust->PosYMaxScal(5.);     //in % of length
      break;
    case 52:
    case 53:
      tofCosmicClust->SetTRefDifMax(5.);  // in ns
      tofCosmicClust->PosYMaxScal(3.);    //in % of length
      break;
    case 62:
    case 63:
      tofCosmicClust->SetTRefDifMax(3.);  // in ns
      tofCosmicClust->PosYMaxScal(2.);    //in % of length
      break;
    case 72:
    case 73:
      tofCosmicClust->SetTRefDifMax(2.);  // in ns
      tofCosmicClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 82:
    case 83:
      tofCosmicClust->SetTRefDifMax(1.);  // in ns
      tofCosmicClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 92:
    case 93:
      tofCosmicClust->SetTRefDifMax(0.9);  // in ns
      tofCosmicClust->PosYMaxScal(0.75);   //in % of length
      break;

    case 4:                               // velocity dependence (DelTOF)
      tofCosmicClust->SetTRefDifMax(6.);  // in ns
      tofCosmicClust->PosYMaxScal(1.5);   //in % of length
      break;
    case 14:
      tofCosmicClust->SetTRefDifMax(5.);  // in ns
      tofCosmicClust->PosYMaxScal(1.);    //in % of length
      break;
    case 24:
      tofCosmicClust->SetTRefDifMax(3.);  // in ns
      tofCosmicClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 34:
      tofCosmicClust->SetTRefDifMax(2.);  // in ns
      tofCosmicClust->PosYMaxScal(1.0);   //in % of length
      break;
    case 44:
      tofCosmicClust->SetTRefDifMax(1.0);  // in ns
      tofCosmicClust->PosYMaxScal(0.75);   //in % of length
      break;
    case 54:
      tofCosmicClust->SetTRefDifMax(0.7);  // in ns
      tofCosmicClust->PosYMaxScal(0.7);    //in % of length
      break;
    case 64:
      tofCosmicClust->SetTRefDifMax(0.9);  // in ns
      tofCosmicClust->PosYMaxScal(0.75);   //in % of length
      break;
    default: cout << "<E> Calib mode not implemented! stop execution of script" << endl; return;
  }

  run->AddTask(tofCosmicClust);

  CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF Cosmic Analysis", iVerbose);

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
  tofAnaTestbeam->SetEnableMatchPosScaling(kFALSE);

  tofAnaTestbeam->SetPosYS2Sel(0.5);  // Y Position selection in fraction of strip length
  tofAnaTestbeam->SetChS2Sel(0.);     // Center of channel selection window
  tofAnaTestbeam->SetDChS2Sel(100.);  // Width  of channel selection window
  tofAnaTestbeam->SetTShift(0.);      // Shift DTD4 to 0
  tofAnaTestbeam->SetSel2TOff(0.);    // Shift Sel2 time peak to 0
  tofAnaTestbeam->SetTOffD4(13.);     // Shift DTD4 to physical value

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
  if (iSel2 > 0) {
    tofCosmicClust->SetSel2Id(iSel2);
    tofCosmicClust->SetSel2Sm(iSel2Sm);
    tofCosmicClust->SetSel2Rpc(iSel2Rpc);

    tofAnaTestbeam->SetMrpcSel2(iSel2);        // initialization of second selector Mrpc Type
    tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);    // initialization of second selector Mrpc SmId
    tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);  // initialization of second selector Mrpc RpcId
  }

  Int_t iRef    = iSet % 1000;
  Int_t iDut    = (iSet - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;

  tofCosmicClust->SetDutId(iDut);
  tofCosmicClust->SetDutSm(iDutSm);
  tofCosmicClust->SetDutRpc(iDutRpc);

  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  tofCosmicClust->SetSelId(iRef);
  tofCosmicClust->SetSelSm(iRefSm);
  tofCosmicClust->SetSelRpc(iRefRpc);

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

        tofAnaTestbeam->SetChi2Lim(30.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(1.5);
        tofAnaTestbeam->SetDYWidth(1.5);
        tofAnaTestbeam->SetDTWidth(120.);   // in ps
        tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
        tofAnaTestbeam->SetDCh4Sel(3);      // Width  of channel selection window
        tofAnaTestbeam->SetPosY4Sel(0.25);  // Y Position selection in fraction of strip length
        tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(4);      // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(4);      // Max Multiplicity in Ref - RPC
        break;

      case 920300:
      case 921300:
        switch (iRSel) {
          case 5:
            tofAnaTestbeam->SetMulDMax(1);    // Max Multiplicity in BeamRef // Diamond
            tofAnaTestbeam->SetTShift(0.2);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 9:
            tofAnaTestbeam->SetTShift(0.1);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);    // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.5);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(30.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(2.);
        tofAnaTestbeam->SetDYWidth(2.);
        tofAnaTestbeam->SetDTWidth(0.2);   // in ps
        tofAnaTestbeam->SetCh4Sel(15);     // Center of channel selection window
        tofAnaTestbeam->SetDCh4Sel(10);    // Width  of channel selection window
        tofAnaTestbeam->SetPosY4Sel(0.5);  // Y Position selection in fraction of strip length
        tofAnaTestbeam->SetMulDMax(1);     // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(3);     // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(3);     // Max Multiplicity in Ref - RPC
        break;

      case 920400:
      case 921400:
        switch (iRSel) {
          case 5:
            tofAnaTestbeam->SetMulDMax(1);    // Max Multiplicity in BeamRef // Diamond
            tofAnaTestbeam->SetTShift(0.2);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 9:
            tofAnaTestbeam->SetTShift(0.1);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);    // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.5);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(30.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(2.);
        tofAnaTestbeam->SetDYWidth(2.);
        tofAnaTestbeam->SetDTWidth(0.2);   // in ps
        tofAnaTestbeam->SetCh4Sel(8);      // Center of channel selection window
        tofAnaTestbeam->SetDCh4Sel(10);    // Width  of channel selection window
        tofAnaTestbeam->SetPosY4Sel(0.5);  // Y Position selection in fraction of strip length
        tofAnaTestbeam->SetMulDMax(1);     // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(30);    // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(30);    // Max Multiplicity in Ref - RPC
        break;

      case 300920:
        switch (iRSel) {
          case 9:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 5:
            tofAnaTestbeam->SetTShift(0.);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(1.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(2.);
        tofAnaTestbeam->SetDYWidth(2.);
        tofAnaTestbeam->SetDTWidth(0.5);  // in ps
        tofAnaTestbeam->SetMulDMax(1);    // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(5);    // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(5);    // Max Multiplicity in Ref - RPC
        break;

      case 900920:

        switch (iRSel) {
          case 4:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 5:
            tofAnaTestbeam->SetTShift(0.);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(10.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(1.);
        tofAnaTestbeam->SetDYWidth(2.);
        tofAnaTestbeam->SetDTWidth(0.1);  // in ps
        tofAnaTestbeam->SetMulDMax(1);    // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(15);   // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(15);   // Max Multiplicity in Ref - RPC
        break;

      case 901911:
      case 910911:
      case 920911:
      case 921911:
      case 600911:
      case 601911:
      case 400911:
      case 300911:
        switch (iRSel) {
          case 400:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 500:
            //tofTestBeamClust->SetBeamAddRefMul(1);
            tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);      // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(-0.17);  // Shift Sel2 time peak to 0
            break;

          case 910:
            tofAnaTestbeam->SetTShift(0.0);    // Shift DTD4 to 0
            tofAnaTestbeam->SetSel2TOff(0.0);  // Shift Sel2 time peak to 0
            tofAnaTestbeam->SetTOffD4(20.);    // Shift DTD4 to physical value
            break;

          case 921:
            tofAnaTestbeam->SetTShift(0.02);     // Shift DTD4 to 0
            tofAnaTestbeam->SetSel2TOff(0.020);  // Shift Sel2 time peak to 0
            tofAnaTestbeam->SetTOffD4(40.);      // Shift DTD4 to physical value
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit
        tofAnaTestbeam->SetChi2Lim2(3.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(0.5);
        tofAnaTestbeam->SetDYWidth(0.8);
        tofAnaTestbeam->SetDTWidth(0.08);  // in ns
        break;


      case 300921:
      case 400921:
      case 900921:
      case 901921:
      case 910921:
      case 911921:
      case 920921:
        switch (iRSel) {
          case 300:
            tofAnaTestbeam->SetTShift(-0.8);     // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);      // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(-150.);  // Shift Sel2 time peak to 0
            break;

          case 500:                            // sel2 = 920
            tofAnaTestbeam->SetTShift(-5.);    // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);    // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.1);  // Shift Sel2 time peak to 0
            break;

          case 911:
            tofAnaTestbeam->SetTShift(0.);       // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(34.);      // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(-0.01);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(10.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(1.0);
        tofAnaTestbeam->SetDYWidth(2.0);
        tofAnaTestbeam->SetDTWidth(0.15);  // in ps
        break;

      case 901900:
      case 400900:
      case 300900:
        switch (iRSel) {
          case 4:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 5:
            tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);      // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(-0.17);  // Shift Sel2 time peak to 0
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(40.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(1.0);
        tofAnaTestbeam->SetDYWidth(2.0);
        tofAnaTestbeam->SetDTWidth(0.15);  // in ps
        break;

      case 901910:
      case 911910:
      case 920910:
      case 921910:
      case 600910:
      case 601910:
      case 400910:
      case 300910:
        switch (iRSel) {
          case 400:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 500:
            tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);      // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(-0.17);  // Shift Sel2 time peak to 0
            break;

          case 911:
            tofAnaTestbeam->SetTShift(0.);       // Shift DTD4 to 0
            tofAnaTestbeam->SetSel2TOff(0.014);  // Shift Sel2 time peak to 0
            tofAnaTestbeam->SetTOffD4(36.);      // Shift DTD4 to physical value
            break;

          case 921:
            tofAnaTestbeam->SetTShift(-0.3);  // Shift DTD4 to 0
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            tofAnaTestbeam->SetTOffD4(36.);   // Shift DTD4 to physical value
            break;

          default:;
        }
        tofAnaTestbeam->SetChi2Lim(10.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetChi2Lim2(3.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(1.0);
        tofAnaTestbeam->SetDYWidth(2.0);
        tofAnaTestbeam->SetDTWidth(0.15);  // in ns
        break;

      case 300400:
        switch (iRSel) {
          case 9:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;
          default:;
        }
        break;

      case 210200:
        switch (iRSel) {
          case 9:
            tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;
          default:;
        }
        break;

      case 700600:
      case 701600:
      case 702600:
      case 703600:
      case 100600:
      case 200600:
      case 210600:
      case 601600:
      case 900600:
      case 901600:
      case 910600:
      case 911600:
      case 920600:
      case 921600:

        switch (iRSel) {
          case 500:
            tofAnaTestbeam->SetTShift(-5.5);  //24.5);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(30.);   // Shift DTD4 to physical value
            switch (iSel2in) {
              case 910:
                tofAnaTestbeam->SetSel2TOff(-5.);  // Shift Sel2 time peak to 0
                break;

              default: cout << " setup not implemented " << endl; return;
            }
            break;

          case 601:
            tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 921:
            tofAnaTestbeam->SetTShift(0.);      // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(11.);     // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.13);  // Shift Sel2 time peak to 0
            break;
        }
        tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit
        tofAnaTestbeam->SetChi2Lim2(50.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(0.5);
        tofAnaTestbeam->SetDYWidth(1.);
        tofAnaTestbeam->SetDTWidth(0.1);  // in ps

        tofAnaTestbeam->SetMulDMax(10);  // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(10);  // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(10);  // Max Multiplicity in Ref - RPC
        break;

      case 900601:
      case 901601:
      case 910601:
      case 911601:
      case 920601:
      case 921601:
      case 600601:
        switch (iRSel) {
          case 5:
            tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;

          case 6:
            tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
            tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
            tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
            break;
        }
        tofAnaTestbeam->SetChi2Lim(5.);  // initialization of Chi2 selection limit
        tofAnaTestbeam->SetDXWidth(0.5);
        tofAnaTestbeam->SetDYWidth(1.);
        tofAnaTestbeam->SetDTWidth(0.1);  // in ps

        tofAnaTestbeam->SetMulDMax(10);  // Max Multiplicity in Diamond
        tofAnaTestbeam->SetMul0Max(10);  // Max Multiplicity in dut
        tofAnaTestbeam->SetMul4Max(10);  // Max Multiplicity in Ref - RPC
        break;

      default:
        cout << "<E> detector setup " << iSet << " unknown, stop!" << endl;
        return;
        ;
    }  // end of different subsets

  //run->AddTask(tofAnaTestbeam);
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

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  //tofCosmicClust->Finish();
  // ------------------------------------------------------------------------
  // default display
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct  = "pl_over_Mat04D4best()";
  gROOT->LoadMacro(Display_Status);

  gROOT->LoadMacro("fit_ybox.h");
  gROOT->LoadMacro("pl_all_CluMul.C");
  gROOT->LoadMacro("pl_all_CluRate.C");
  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_over_Walk2.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gROOT->LoadMacro("save_hst.C");

  switch (iSet) {
    default:

      gInterpreter->ProcessLine("pl_over_clu(0,0,0)");
      gInterpreter->ProcessLine("pl_over_clu(0,0,1)");
      gInterpreter->ProcessLine("pl_over_clu(0,1,0)");
      gInterpreter->ProcessLine("pl_over_clu(0,1,1)");
      gInterpreter->ProcessLine("pl_over_clu(0,2,0)");
      gInterpreter->ProcessLine("pl_over_clu(0,2,1)");

      gInterpreter->ProcessLine("pl_over_cluSel(0,0,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,0,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,0,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,0,1,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,0,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(0,0,2,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,0,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,0,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,1,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,1,1)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,2,0)");
      gInterpreter->ProcessLine("pl_over_cluSel(1,0,2,1)");

      gInterpreter->ProcessLine("pl_over_Walk2(0,0,0,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(0,0,0,1)");
      gInterpreter->ProcessLine("pl_over_Walk2(0,0,1,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(0,0,1,1)");
      gInterpreter->ProcessLine("pl_over_Walk2(0,0,2,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(0,0,2,1)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,0,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,0,1)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,1,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,1,1)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,2,0)");
      gInterpreter->ProcessLine("pl_over_Walk2(1,0,2,1)");

      gInterpreter->ProcessLine("pl_all_CluMul()");
      gInterpreter->ProcessLine("pl_all_CluRate()");
      gInterpreter->ProcessLine("pl_all_dTSel()");
      TString FSave = Form("save_hst(\"cosdev-status%d_%d_Cal_%s.hst.root\")", iCalSet, iSel2in, cCalId.Data());
      gInterpreter->ProcessLine(FSave.Data());
      //gInterpreter->ProcessLine("pl_over_MatD4sel()");
      break;
      ;
  }
  gInterpreter->ProcessLine(Display_Funct.Data());
}
