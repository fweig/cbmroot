/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Norbert Herrmann */

void dis_trks(Int_t nEvents = 10, Int_t iSel = 1, Int_t iGenCor = 1, TString cFileId = "831.50.3.0",
              TString cSet = "012022500_500", Int_t iSel2 = 500, Int_t iTrackingSetup = 1, Double_t dScalFac = 1.,
              Double_t dChi2Lim2 = 5., Double_t dDeadtime = 50, TString cCalId = "", Int_t iAnaCor = 1,
              Bool_t bUseSigCalib = kFALSE, Int_t iCalSet = 12022500, Int_t iCalOpt = 1, Int_t iTrkPar = 1,
              Int_t iMc = 0)
{

  Int_t iVerbose = 1;
  TString FId    = cFileId;
  Int_t iNLen    = FId.First(".");
  TString cRun(FId(0, iNLen));
  Int_t iRun = -1;
  if (cRun == "unp") {
    iRun    = 831;
    cCalId  = "831.100.4.0";
    iCalSet = 10020500;
  }
  else {
    iRun = cRun.Atoi();
    if (cCalId == "") cCalId = cFileId;
  }
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  //TString logLevel = "INFO";
  TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  TString workDir = gSystem->Getenv("VMCWORKDIR");
  //TString workDir          = "../../..";
  //TString paramDir       = workDir  + "/macro/beamtime/mcbm2020";
  TString paramDir      = "./";
  TString ParFile       = paramDir + "data/" + cFileId.Data() + ".params.root";
  TString InputFile     = paramDir + "data/" + cFileId.Data() + ".root";
  TString InputDigiFile = "";
  if (cRun == "unp") { InputDigiFile = paramDir + "data/" + cFileId.Data() + ".root"; }
  else {
    InputDigiFile = paramDir + "data/TofHits_" + cFileId.Data() + Form("_%s_%02.0f_Cal", cSet.Data(), dDeadtime)
                    + cCalId + ".out.root";
  }
  if (iMc == 1) {
    InputFile     = paramDir + "data/" + cFileId.Data() + ".raw.root";
    InputDigiFile = paramDir + "data/" + cFileId.Data() + ".rec.root";
    iRun          = 700;
  }
  TString OutputFile =
    paramDir + "data/distrks_" + cFileId.Data() + Form("_%s_%06d_%03d", cSet.Data(), iSel, iSel2) + ".out.root";
  TString cHstFile = paramDir
                     + Form("/hst/%s_%03.0f_%s_%06d_%03d_%03.1f_%03.1f_trk%03d_Cal%s_Dis.hst.root", cFileId.Data(),
                            dDeadtime, cSet.Data(), iSel, iSel2, dScalFac, dChi2Lim2, iTrackingSetup, cCalId.Data());
  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cCalId.Data());
  TString cAnaFile = Form("%s_TrkAnaTestBeam.hst.root", cCalId.Data());

  cout << " InputDigiFile = " << InputDigiFile << endl;

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();

  Int_t iGeo = 0;  //iMc;
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

    // -----   Load the geometry setup   -------------------------------------
    /*
  const char* setupName = "mcbm_beam_2020_03";
  TString setupFile = workDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- mcbm_reco: Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
*/

    TString geoDir      = gSystem->Getenv("VMCWORKDIR");
    TString geoFile     = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
    TFile* fgeo         = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile" << endl;
      return;
    }

    //TObjString *tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"); // TOF digi file
    //parFileList->Add(tofDigiFile);

    TObjString* tofDigiBdfFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
    parFileList->Add(tofDigiBdfFile);

    // -----   Reconstruction run   -------------------------------------------
    FairRunAna* run = new FairRunAna();
    cout << "InputFile:     " << InputFile.Data() << endl;
    cout << "InputDigiFile: " << InputDigiFile.Data() << endl;

    //run->SetInputFile(InputFile.Data());
    //run->AddFriend(InputDigiFile.Data());
    FairFileSource* fFileSource = new FairFileSource(InputFile.Data());
    fFileSource->AddFriend(InputDigiFile.Data());
    run->SetSource(fFileSource);
    run->SetUserOutputFileName(OutputFile.Data());
    run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

    FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
    //  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");
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
    cout << "<I> " << cCmd << endl;
    gInterpreter->ProcessLine(cCmd);

    CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
    Int_t iNStations                = tofFindTracks->GetNStations();


    // =========================================================================
    // ===                       Analysis                                    ===
    // =========================================================================
    Int_t iRSel = iCalSet % 1000;
    gROOT->LoadMacro("ini_AnaTestbeam.C");
    cCmd =
      Form("ini_AnaTestbeam(%d,\"%s\",%d,%d,%5.2f,%d,%d)", iSel, cFileId.Data(), iSel2in, iRSel, 0.9, iAnaCor, iMc);
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

    FairEventManager* fMan = new FairEventManager();

    CbmEvDisTracks* Tracks = new CbmEvDisTracks("Tof Tracks", 1, kFALSE,
                                                kTRUE);  //name, verbosity, RnrChildren points, RnrChildren track
    //  CbmEvDisTracks *Tracks =  new CbmEvDisTracks("Tof Tracks",1);
    fMan->AddTask(Tracks);
    CbmPixelHitSetDraw* TofUHits = new CbmPixelHitSetDraw("TofUHit", kRed, kOpenCross);
    fMan->AddTask(TofUHits);
    CbmPointSetArrayDraw* TofHits =
      new CbmPointSetArrayDraw("TofHit", 1, 1, 1,
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
      //TString name = vol->GetName();
      //cout << " GeoVolume "<<i<<" Name: "<< name << endl;
      //vol->SetLineColor(kRed);
      vol->SetTransparency(80);
    }
    fMan->Init(1, 5);

    cout << "customize TEveManager gEve " << gEve << endl;
    gEve->GetDefaultGLViewer()->SetClearColor(kYellow - 10);
    TGLViewer* v       = gEve->GetDefaultGLViewer();
    TGLAnnotation* ann = new TGLAnnotation(v, cFileId, 0.01, 0.98);
    ann->SetTextSize(0.03);  // % of window diagonal
    ann->SetTextColor(4);

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
  }
}
