/** @file eval_raw.C
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbrt Herrmann [orginator] **/

void eval_raw(Int_t nEvents = 10000000, Int_t calMode = 33, Int_t calSel = 1, Int_t calSm = 900, Int_t RefSel = 1,
              TString cFileId = "Test", Int_t iCalSet = 910601600, Bool_t bOut = 0, Int_t iSel2 = 0,
              Double_t dDeadtime = 50, TString cCalId = "XXX", Int_t iSel = 910041, Int_t iSel22 = 31,
              Int_t iTrackingSetup = 4, Int_t iGenCor = 1, Double_t dScalFac = 1., Double_t dChi2Lim2 = 3.,
              Bool_t bUseSigCalib = kFALSE, Int_t iCalOpt = 1, Int_t iAnaCor = 1, Int_t iTrkPar = 0, Int_t iMc = 0,
              Int_t iPlot = 0)
{
  Int_t iVerbose    = 1;
  Int_t iBugCor     = 0;
  Int_t iFirstEvent = 0;  //500000;


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
  gLogger->SetLogVerbosityLevel("VERYHIGH");
  //gLogger->SetLogVerbosityLevel("MEDIUM");

  TString workDir  = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/macro/beamtime/mcbm2021/";
  //TString paramDir   = "../";

  TString ParFile   = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile = paramDir + "data/" + cFileId + ".root";
  // TString InputFile  =  "./data/" + cFileId + ".root";
  TString OutputFile = paramDir + "data/EvalRaw_" + cFileId + Form("_%09d_%03d_%02.0f_Cal", iCalSet, iSel2, dDeadtime)
                       + cCalId + Form("_%d_%03d_trk%03d", iSel, iSel22, iTrackingSetup) + ".out.root";

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();

  TString FId = cFileId;
  Int_t iNLen = FId.First(".");
  TString cRun(FId(0, iNLen));
  Int_t iRun = cRun.Atoi();

  Int_t iGeo     = 0;
  TString TofGeo = "";
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
  }
  cout << "Geometry version " << TofGeo << endl;

  if (nEvents > -1) {
    if (iRun > 10000) {
      iFirstEvent = 2000000;  // late start of Buc ...
      if (iRun > 1050) iFirstEvent = 10000000;
      nEvents += iFirstEvent;
    }
  }

  //   TObjString *tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"); // TOF digi file
  //   parFileList->Add(tofDigiFile);

  //   TObjString tofDigiBdfFile = new TObjString( paramDir + "/tof." + FPar + "digibdf.par");
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

  Int_t iRSelin = iCalSet % 1000;

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  //run->SetInputFile(InputFile.Data());
  //run->AddFriend(InputFile.Data());
  FairFileSource* fFileSource = new FairFileSource(InputFile.Data());
  run->SetSource(fFileSource);
  // run->SetOutputFile(OutputFile);
  //run->SetSink( new FairRootFileSink( OutputFile.Data() ) );
  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

  gROOT->LoadMacro("ini_Clusterizer.C");
  Char_t* cCmd = Form("ini_Clusterizer(%d,%d,%d,%d,\"%s\",%d,%d,%d,%f,\"%s\")", calMode, calSel, calSm, RefSel,
                      cFileId.Data(), iCalSet, (Int_t) bOut, iSel2in, dDeadtime, cCalId.Data());
  cout << "<I> " << cCmd << endl;
  gInterpreter->ProcessLine(cCmd);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================
  gROOT->LoadMacro("ini_trks.C");
  cCmd = Form("ini_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d)", iSel, iTrackingSetup, iGenCor, dScalFac, dChi2Lim2,
              cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar);
  gInterpreter->ProcessLine(cCmd);
  LOG(info) << cCmd;
  CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
  Int_t iNStations                = tofFindTracks->GetNStations();

  // =========================================================================
  // ===                       Analysis                                    ===
  // =========================================================================
  gROOT->LoadMacro("ini_AnaTestbeam.C");
  cCmd =
    Form("ini_AnaTestbeam(%d,\"%s\",%d,%d,%5.2f,%d,%d)", iSel, cFileId.Data(), iSel2in, iRSelin, 0.9, iAnaCor, iMc);
  LOG(info) << cCmd;
  gInterpreter->ProcessLine(cCmd);


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
  run->Run(iFirstEvent, nEvents);

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

  TString FSave = Form("save_hst(\"EvalRaw_%d_%d_Cal_%s_%06d_%03d_trk_%03d.hst.root\")", iCalSet, iSel2in,
                       cCalId.Data(), iSel, iSel22, iTrackingSetup);
  gInterpreter->ProcessLine(FSave.Data());

  if (iPlot) {

    switch (iCalSet) {
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
        gInterpreter->ProcessLine("pl_all_CluMul()");
        gInterpreter->ProcessLine("pl_all_CluRate()");
        gInterpreter->ProcessLine("pl_all_CluRate(5,1)");
        gInterpreter->ProcessLine("pl_all_CluPosEvol()");
        gInterpreter->ProcessLine("pl_all_CluTimeEvol()");
        gInterpreter->ProcessLine("pl_all_dTSel()");

        //  gInterpreter->ProcessLine("pl_over_MatD4sel()");
        //  gInterpreter->ProcessLine(Display_Funct.Data());
      */
        break;
        ;
    }
  }
}
