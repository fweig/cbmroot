void ana_digi_cal(Int_t nEvents      = 10000000,
                  Int_t calMode      = 53,
                  Int_t calSel       = 0,
                  Int_t calSm        = 900,
                  Int_t RefSel       = 1,
                  TString cFileId    = "Test",
                  Int_t iCalSet      = 910601600,
                  Bool_t bOut        = 0,
                  Int_t iSel2        = 0,
                  Double_t dDeadtime = 50,
                  TString cCalId     = "XXX",
                  Int_t iPlot        = 1) {
  Int_t iVerbose = 1;
  Int_t iBugCor  = 0;
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
  TString paramDir = workDir + "/macro/beamtime/mcbm2020/";
  //TString paramDir   = "./";
  TString ParFile   = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile = paramDir + "data/" + cFileId + ".root";
  // TString InputFile  =  "./data/" + cFileId + ".root";
  TString OutputFile =
    paramDir + "data/digidev_" + cFileId
    + Form("_%09d_%03d_%02.0f_Cal", iCalSet, iSel2, dDeadtime) + cCalId
    + ".out.root";

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();

  TString FId    = cFileId;
  TString TofGeo = "v20b_mcbm";
  cout << "Geometry version " << TofGeo << endl;

  //   TObjString *tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"); // TOF digi file
  //   parFileList->Add(tofDigiFile);

  //   TObjString tofDigiBdfFile = new TObjString( paramDir + "/tof." + FPar + "digibdf.par");
  TObjString* tofDigiBdfFile =
    new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
  parFileList->Add(tofDigiBdfFile);

  TString geoDir  = gSystem->Getenv("VMCWORKDIR");
  TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
  TFile* fgeo     = new TFile(geoFile);
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
  // run->SetOutputFile(OutputFile);
  //run->SetSink( new FairRootFileSink( OutputFile.Data() ) );
  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));
  CbmTofEventClusterizer* tofClust =
    new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);

  tofClust->SetCalMode(calMode);
  tofClust->SetCalSel(calSel);
  tofClust->SetCaldXdYMax(3.);  // geometrical matching window in cm
  tofClust->SetCalCluMulMax(
    5.);  // Max Counter Cluster Multiplicity for filling calib histos
  tofClust->SetCalRpc(calSm);   // select detector for calibration update
  tofClust->SetTRefId(RefSel);  // reference trigger for offset calculation
  tofClust->SetTotMax(20.);     // Tot upper limit for walk corection
  tofClust->SetTotMin(
    0.01);  //(12000.);  // Tot lower limit for walk correction
  tofClust->SetTotPreRange(
    5.);  // effective lower Tot limit  in ns from peak position
  tofClust->SetTotMean(5.);       // Tot calibration target value in ns
  tofClust->SetMaxTimeDist(1.0);  // default cluster range in ns
  //tofClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ns
  tofClust->SetDelTofMax(
    5.);  // acceptance range for cluster distance in ns (!)
  tofClust->SetSel2MulMax(3);  // limit Multiplicity in 2nd selector
  tofClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofClust->SetEnableAvWalk(kFALSE);
  //tofClust->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
  tofClust->SetYFitMin(1.E4);
  tofClust->SetToDAv(0.04);
  // tofClust->SetTimePeriod(25600.);       // ignore coarse time
  // tofClust->SetCorMode(iBugCor);         // correct missing hits
  tofClust->SetIdMode(0);  // calibrate on counter level
  //tofClust->SetIdMode(1);                  // calibrate on module level
  //   tofClust->SetDeadStrips(15,23);   // declare dead strip for T0M3,Rpc0,Strip 23
  //tofClust->SetDeadStrips(25,16);   // declare non-existant diamond strip (#5) dead

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

  TString cAnaFile =
    Form("%s_%09d%03d_tofAna.hst.root", cFileId.Data(), iCalSet, iSel2);

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
      tofClust->SetTRefDifMax(2.0);  // in ns
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
    default:
      cout << "<E> Calib mode not implemented! stop execution of script"
           << endl;
      return;
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

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;

  tofClust->SetSel2Id(iSel2);
  tofClust->SetSel2Sm(iSel2Sm);
  tofClust->SetSel2Rpc(iSel2Rpc);

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

  run->AddTask(tofClust);

  cout << "Run with iRSel = " << iRSel << ", iSel2 = " << iSel2in << endl;


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
  //tofClust->Finish();
  // ------------------------------------------------------------------------
  // default display
  /*
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct = "pl_over_Mat04D4best()";  
  gROOT->LoadMacro(Display_Status);
  */

  gROOT->LoadMacro("save_hst.C");

  gROOT->LoadMacro("fit_ybox.h");
  gROOT->LoadMacro("pl_all_CluMul.C");
  gROOT->LoadMacro("pl_all_CluRate.C");
  gROOT->LoadMacro("pl_all_CluPosEvol.C");
  gROOT->LoadMacro("pl_all_CluTimeEvol.C");
  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_over_Walk2.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gROOT->LoadMacro("pl_all_Sel2D.C");
  gROOT->LoadMacro("pl_all_2D.C");

  if (iPlot) {

    switch (iSet) {
      default:
        for (Int_t iOpt = 0; iOpt < 8; iOpt++) {
          for (Int_t iSel = 0; iSel < 2; iSel++) {
            gInterpreter->ProcessLine(Form("pl_all_Sel2D(%d,%d)", iOpt, iSel));
          }
        }

        for (Int_t iOpt = 0; iOpt < 12; iOpt++) {
          gInterpreter->ProcessLine(Form("pl_all_2D(%d)", iOpt));
        }
        /*
	gInterpreter->ProcessLine("pl_over_clu(0,0,0)");
	gInterpreter->ProcessLine("pl_over_clu(0,0,1)");
	gInterpreter->ProcessLine("pl_over_clu(0,0,2)");
	gInterpreter->ProcessLine("pl_over_clu(0,0,3)");
	gInterpreter->ProcessLine("pl_over_clu(0,0,4)");
	gInterpreter->ProcessLine("pl_over_clu(0,1,0)");
	gInterpreter->ProcessLine("pl_over_clu(0,1,1)");
	gInterpreter->ProcessLine("pl_over_clu(0,1,2)");
	gInterpreter->ProcessLine("pl_over_clu(0,1,3)");
	gInterpreter->ProcessLine("pl_over_clu(0,1,4)");
	gInterpreter->ProcessLine("pl_over_clu(0,2,0)");
	gInterpreter->ProcessLine("pl_over_clu(0,2,1)");
	gInterpreter->ProcessLine("pl_over_clu(0,2,2)");
	gInterpreter->ProcessLine("pl_over_clu(0,2,3)");
	gInterpreter->ProcessLine("pl_over_clu(0,2,4)");
	gInterpreter->ProcessLine("pl_over_clu(0,3,0)");
	gInterpreter->ProcessLine("pl_over_clu(0,3,1)");
	gInterpreter->ProcessLine("pl_over_clu(0,3,2)");
	gInterpreter->ProcessLine("pl_over_clu(0,3,3)");
	gInterpreter->ProcessLine("pl_over_clu(0,3,4)");
	gInterpreter->ProcessLine("pl_over_clu(0,4,0)");
	gInterpreter->ProcessLine("pl_over_clu(0,4,1)");
	gInterpreter->ProcessLine("pl_over_clu(0,4,2)");
	gInterpreter->ProcessLine("pl_over_clu(0,4,3)");
	gInterpreter->ProcessLine("pl_over_clu(0,4,4)");
	
	gInterpreter->ProcessLine("pl_over_clu(5,0,0)");
	gInterpreter->ProcessLine("pl_over_cluSel(0,5,0,0)");
	gInterpreter->ProcessLine("pl_over_cluSel(1,5,0,0)");
	
	for(Int_t iSm=0; iSm<3; iSm++)
	for (Int_t iRpc=0; iRpc<5; iRpc++)
	for (Int_t iSel=0; iSel<2; iSel++){
	gInterpreter->ProcessLine(Form("pl_over_cluSel(%d,0,%d,%d)",iSel,iSm,iRpc));
	gInterpreter->ProcessLine(Form("pl_over_Walk2(%d,0,%d,%d)",iSel,iSm,iRpc));
	}
      */
        gInterpreter->ProcessLine("pl_all_CluMul()");
        gInterpreter->ProcessLine("pl_all_CluRate()");
        gInterpreter->ProcessLine("pl_all_CluRate(5,1)");
        gInterpreter->ProcessLine("pl_all_CluPosEvol()");
        gInterpreter->ProcessLine("pl_all_CluTimeEvol()");
        gInterpreter->ProcessLine("pl_all_dTSel()");

        //  gInterpreter->ProcessLine("pl_over_MatD4sel()");
        //  gInterpreter->ProcessLine(Display_Funct.Data());
        break;
        ;
    }
  }
  TString FSave = Form("save_hst(\"CluStatus%d_%d_Cal_%s.hst.root\")",
                       iCalSet,
                       iSel2in,
                       cCalId.Data());
  gInterpreter->ProcessLine(FSave.Data());
}
