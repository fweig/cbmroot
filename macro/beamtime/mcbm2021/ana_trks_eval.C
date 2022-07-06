/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void ana_trks_eval(Int_t nEvents = 10000, Int_t iSel = 1, Int_t iGenCor = 1, TString cFileId = "48.50.7.1",
                   TString cSet = "000010020", Int_t iSel2 = 20, Int_t iTrackingSetup = 2, Double_t dScalFac = 1.,
                   Double_t dChi2Lim2 = 500., Double_t dDeadtime = 50, TString cCalId = "", Int_t iAnaCor = 1,
                   Bool_t bUseSigCalib = kFALSE, Int_t iCalSet = 30040500, Int_t iCalOpt = 1, Int_t iTrkPar = 0,
                   Int_t iMc = 0)
{
  Int_t iVerbose = 1;
  if (cCalId == "") cCalId = cFileId;
  TString FId = cFileId;
  Int_t iNLen = FId.First(".");
  TString cRun(FId(0, iNLen));
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
  TString paramDir = workDir + "/macro/beamtime/mcbm2021";
  //TString paramDir       = ".";

  TString ParFile       = paramDir + "/data/" + cFileId.Data() + ".params.root";
  TString InputFile     = paramDir + "/data/" + cFileId.Data() + ".root";
  TString InputDigiFile = paramDir + "/data/TofHits_" + cFileId.Data() + Form("_%s_%02.0f_Cal", cSet.Data(), dDeadtime)
                          + cCalId + ".out.root";
  if (iMc == 1) {
    InputFile     = paramDir + "/data/" + cFileId.Data() + ".raw.root";
    InputDigiFile = paramDir + "/data/" + cFileId.Data() + ".rec.root";
    iRun          = 700;
  }
  TString InputTrklFile =
    paramDir + "/data/TofTrks_" + cFileId.Data() + Form("_%s_%06d_%03d", cSet.Data(), iSel, iSel2) + ".out.root";
  TString OutputFile =
    paramDir + "/data/ana_" + cFileId.Data() + Form("_%s_%06d_%03d", cSet.Data(), iSel, iSel2) + ".out.root";
  TString cHstFile = paramDir
                     + Form("/hst/%s_%03.0f_%s_%06d_%03d_%03.1f_%03.1f_trk%03d_Cal%s_Ana.hst.root", cFileId.Data(),
                            dDeadtime, cSet.Data(), iSel, iSel2, dScalFac, dChi2Lim2, iTrackingSetup, cCalId.Data());
  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cCalId.Data());
  TString cAnaFile = Form("%s_TrkAnaTestBeam.hst.root", cFileId.Data());

  cout << " InputDigiFile = " << InputDigiFile << endl;
  cout << " InputTrklFile = " << InputTrklFile << endl;
  cout << " FindTrkFile   = " << cTrkFile << endl;

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();

  Int_t iGeo = 0;
  if (iGeo == 0) {
    TString TofGeo = "";
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
  }
  else {
    TString setupName = "mcbm_beam_2021_01";
    // -----   Load the geometry setup   -------------------------------------
    TString setupFile  = workDir + "/geometry/setup/setup_" + setupName.Data() + ".C";
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
  //run->SetInputFile(InputDigiFile.Data());
  //run->AddFriend(InputTrklFile.Data());
  //run->SetInputFile(InputTrklFile.Data());
  FairFileSource* fFileSource = new FairFileSource(InputDigiFile.Data());
  run->SetSource(fFileSource);

  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel("VERYHIGH");

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

  CbmTofEventClusterizer* tofClust = new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);
  Int_t calSelRead                 = calSel;
  if (calSel < 0) calSelRead = 0;
  TString cFname = Form("%s_set%09d_%02d_%01dtofClust.hst.root", cFileId.Data(), iCalSet, calMode, calSelRead);
  if (cCalId != "XXX")
    cFname = Form("%s_set%09d_%02d_%01dtofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSelRead);
  tofClust->SetCalParFileName(cFname);
  TString cOutFname = Form("tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================
  gROOT->LoadMacro("ini_trks.C");
  Char_t* cCmd = Form("ini_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d)", iSel, iTrackingSetup, iGenCor, dScalFac,
                      dChi2Lim2, cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar);
  gInterpreter->ProcessLine(cCmd);

  CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
  Int_t iNStations                = tofFindTracks->GetNStations();

  // =========================================================================
  // ===                       Analysis                                    ===
  // =========================================================================
  gROOT->LoadMacro("ini_AnaTestbeam.C");
  case 1051:
  case 1058:
    tofAnaTestbeam->SetTShift(-3.);   // Shift DTD4 to 0
    tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
    break;

    Int_t iRSel = iCalSet % 1000;
    cCmd =
      Form("ini_AnaTestbeam(%d,\"%s\",%d,%d,%5.2f,%d,%d)", iSel, cFileId.Data(), iSel2in, iRSel, 0.9, iAnaCor, iMc);
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
    run->Run(0, nEvents);
    //run->Run(nEvents-1, nEvents); //debugging single events for memory leak
    // ------------------------------------------------------------------------
    TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
    gROOT->LoadMacro("save_hst.C");
    gInterpreter->ProcessLine(SaveToHstFile);

    // default displays, plot results

    TString Display_Status = "pl_over_Mat04D4best.C";
    TString Display_Funct;
    if (iGenCor < 0) { Display_Funct = "pl_over_Mat04D4best(1)"; }
    else {
      Display_Funct = "pl_over_Mat04D4best(0)";
    }
    gROOT->LoadMacro(Display_Status);

    cout << "Exec " << Display_Funct.Data() << endl;
    gInterpreter->ProcessLine(Display_Funct);

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
    gROOT->LoadMacro("pl_Dut_Vel.C");

    cout << "Plotting for Dut " << iDut << iDutSm << iDutRpc << ", Ref " << iRef << iRefSm << iRefRpc << endl;

    gInterpreter->ProcessLine("pl_over_MatD4sel()");
    gInterpreter->ProcessLine("pl_TIS()");
    gInterpreter->ProcessLine("pl_TIR()");
    gInterpreter->ProcessLine(Form("pl_Dut_Vel(\"%d%d%d\")", iDut, iDutSm, iDutRpc));
    gInterpreter->ProcessLine("pl_eff_XY()");
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
