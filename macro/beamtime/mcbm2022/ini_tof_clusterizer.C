/* Copyright (C) 2022 Physikalisches Institut, Universitaet Heidelberg, Heidelberg 
   SPDX-License-Identifier: GPL-3.0-only
   Authors:  Norbert Herrmann [committer]*/

void ini_tof_clusterizer(Int_t calMode = 53, Int_t calSel = 0, Int_t calSm = 900, Int_t RefSel = 1,
                         TString cFileId = "Test", Int_t iCalSet = 910601600, Bool_t bOut = 0, Int_t iSel2 = 0,
                         Double_t dDeadtime = 50, TString cCalId = "XXX", TString parPath = "")
{
  Int_t iVerbose                   = 1;
  CbmTofEventClusterizer* tofClust = new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);
  cout << "Instantiate  CbmTofEventClusterizer " << endl;
  tofClust->SetCalMode(calMode);
  tofClust->SetCalSel(calSel);
  tofClust->SetCaldXdYMax(3000.);  // geometrical matching window in cm
  tofClust->SetCalCluMulMax(3.);   // Max Counter Cluster Multiplicity for filling calib histos
  tofClust->SetCalRpc(calSm);      // select detector for calibration update
  tofClust->SetTRefId(RefSel);     // reference trigger for offset calculation
  tofClust->SetTotMax(20.);        // Tot upper limit for walk corection
  tofClust->SetTotMin(0.);         //(12000.);  // Tot lower limit for walk correction
  tofClust->SetTotPreRange(5.);    // effective lower Tot limit  in ns from peak position
  tofClust->SetTotMean(5.);        // Tot calibration target value in ns
  tofClust->SetMaxTimeDist(0.5);   // default cluster range in ns
  //tofClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ns
  tofClust->SetDelTofMax(50.);              // acceptance range for cluster distance in ns (!)
  tofClust->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
  tofClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofClust->SetEnableAvWalk(kFALSE);
  //tofClust->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
  tofClust->SetYFitMin(1.E3);
  tofClust->SetToDAv(0.04);
  // tofClust->SetTimePeriod(25600.);       // ignore coarse time
  // tofClust->SetCorMode(iBugCor);         // correct missing hits
  //tofClust->SetIdMode(0);                  // calibrate on counter level
  tofClust->SetIdMode(1);  // calibrate on module level
  //   tofClust->SetDeadStrips(15,23);   // declare dead strip for T0M3,Rpc0,Strip 23
  //tofClust->SetDeadStrips(25,16);   // declare non-existant diamond strip (#5) dead

  Int_t calSelRead = calSel;
  TString cFname =
    Form("%s/%s_set%09d_%02d_%01dtofClust.hst.root", parPath.Data(), cFileId.Data(), iCalSet, calMode, calSelRead);
  if (calSel < 0) calSelRead = 1;  // get default calibration
  if (cCalId != "XXX")
    cFname =
      Form("%s/%s_set%09d_%02d_%01dtofClust.hst.root", parPath.Data(), cCalId.Data(), iCalSet, calMode, calSelRead);
  tofClust->SetCalParFileName(cFname);
  TString cOutFname = Form("data/tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  TString cAnaFile = Form("data/%s_%09d%03d_tofAna.hst.root", cFileId.Data(), iCalSet, iSel2);

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
      tofClust->SetTRefDifMax(3.0);  // in ns
      tofClust->PosYMaxScal(2.0);    // in % of length
      break;
    case 31:
      tofClust->SetTRefDifMax(3.);  // in ns
      tofClust->PosYMaxScal(1.);    // in % of length
      break;
    case 41:
      tofClust->SetTRefDifMax(2.0);  // in ns
      tofClust->PosYMaxScal(0.9);    // in % of length
      break;
    case 51:
      tofClust->SetTRefDifMax(2.0);  // in ns
      tofClust->PosYMaxScal(0.8);    // in % of length
      break;
    case 61:
      tofClust->SetTRefDifMax(1.5);  // in ns
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
      tofClust->SetTRefDifMax(10.);  // in ns
      tofClust->PosYMaxScal(1.5);    //in % of length
      break;
    case 62:
    case 63:
      tofClust->SetTRefDifMax(10.);  // in ns
      tofClust->PosYMaxScal(1.);     //in % of length
      break;
    case 72:
    case 73:
    case 75:
      tofClust->SetTRefDifMax(10.);  // in ns
      tofClust->PosYMaxScal(0.9);    //in % of length
      break;
    case 82:
    case 83:
    case 85:
      tofClust->SetTRefDifMax(10.);  // in ns
      tofClust->PosYMaxScal(0.8);    //in % of length
      break;
    case 92:
    case 93:
    case 95:
      tofClust->SetTRefDifMax(10.);  // in ns
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
      tofClust->SetTRefDifMax(3.5);  // in ns
      tofClust->PosYMaxScal(1.0);    //in % of length
      break;
    case 54:
      tofClust->SetTRefDifMax(3.0);  // in ns
      tofClust->PosYMaxScal(0.9);    //in % of length
      break;
    case 64:
      tofClust->SetTRefDifMax(2.5);  // in ns
      tofClust->PosYMaxScal(0.8);    //in % of length
      break;
    case 74:
      tofClust->SetTRefDifMax(2.0);  // in ns
      tofClust->PosYMaxScal(0.7);    //in % of length
      break;
    case 19:
    case 29:
    case 39:
    case 49:
    case 59:
    case 69:
    case 79:
    case 89:
    case 99:
      tofClust->SetTRefDifMax(20.0);  // in ns
      tofClust->PosYMaxScal(2.);      //in % of length
      break;
    default: cout << "<E> Calib mode not implemented! stop execution of script" << endl; return;
  }

  Int_t iBRef    = iCalSet % 1000;
  Int_t iSet     = (iCalSet - iBRef) / 1000;
  Int_t iRSel    = 0;
  Int_t iRSelTyp = 0;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  iRSel          = iBRef;  // use diamond

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

  Int_t iSel2in = iSel2;
  if (iSel2in > -1) {
    Int_t iSel2Rpc = iSel2 % 10;
    iSel2          = (iSel2 - iSel2Rpc) / 10;
    Int_t iSel2Sm  = iSel2 % 10;
    iSel2          = (iSel2 - iSel2Sm) / 10;

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

  FairRunAna* run = FairRunAna::Instance();
  if (NULL != run) run->AddTask(tofClust);

  cout << "Clusterizer Run with iRSel = " << iRSel << ", iSel2 = " << iSel2in << endl;
}
