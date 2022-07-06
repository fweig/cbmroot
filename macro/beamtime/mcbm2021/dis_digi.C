/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void dis_digi(Int_t nEvents = 100, Int_t calMode = 93, Int_t calSel = 1, Int_t calSm = 0, Int_t RefSel = 1,
              TString cFileId = "68.50.7.1", Int_t iCalSet = 10500, Bool_t bOut = 0, Int_t iSel2 = 20,
              Double_t dDeadtime = 50, Int_t iGenCor = 1, Int_t iTrackingSetup = 1, Double_t dScalFac = 5.,
              Double_t dChi2Lim2 = 10., TString cCalId = "XXX", Bool_t bUseSigCalib = kFALSE, Int_t iCalOpt = 1,
              Int_t iTrkPar = 3)
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
  //TString logLevel = "INFO";
  TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel(logLevel);
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("VERYHIGH");

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  /*
   TString workDir    = (TString)gInterpreter->ProcessLine(".! pwd");
   cout << "workdir = "<< workDir.Data() << endl;
   return;
  */
  //   TString paramDir   = workDir + "/macro/beamtime/mcbm2019/";
  //TString paramDir   = "./";
  TString paramDir   = "/home/nh/LUSTRE/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/";
  TString ParFile    = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile  = paramDir + "data/" + cFileId + ".root";
  TString OutputFile = paramDir + "data/disdigi_" + cFileId + Form("_%09d%03d", iCalSet, iSel2) + ".out.root";

  TString cTrkFile = Form("%s_tofFindTracks.hst.root", cFileId.Data());

  TList* parFileList = new TList();


  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TString TofGeo;
  Int_t iGeo = 0;  //iMc;
  if (iGeo == 0) {
    if (iRun < 690) TofGeo = "v20a_mcbm";
    else {
      if (iRun < 1112) { TofGeo = "v21a_mcbm"; }
        if (iRun < 1400) { TofGeo = "v21b_mcbm"; }
        else {
          if (iRun < 1400) { TofGeo = "v21b_mcbm"; }
          else {
            if (iRun < 2000) { TofGeo = "v21d_mcbm"; }
            else {
              if (iRun < 2100) { TofGeo = "v21e_mcbm"; }
              else {
                if (iRun < 2176) { TofGeo = "v21f_mcbm"; }
                else {
                  TofGeo = "v21g_mcbm";
                }
              }
            }
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
  FairFileSource* fFileSource = new FairFileSource(InputFile.Data());
  run->SetSource(fFileSource);
  run->SetUserOutputFileName(OutputFile.Data());
  run->SetSink(new FairRootFileSink(run->GetUserOutputFileName()));

  gROOT->LoadMacro("ini_Clusterizer.C");
  Char_t* cCmd = Form("ini_Clusterizer(%d,%d,%d,%d,\"%s\",%d,%d,%d,%f,\"%s\")", calMode, calSel, calSm, RefSel,
                      cFileId.Data(), iCalSet, (Int_t) bOut, iSel2, dDeadtime, cCalId.Data());
  cout << "<I> " << cCmd << endl;
  gInterpreter->ProcessLine(cCmd);

  CbmTofEventClusterizer* tofClust = new CbmTofEventClusterizer("TOF Event Clusterizer", iVerbose, bOut);

  // =========================================================================
  // ===                       Tracking                                    ===
  // =========================================================================
  gROOT->LoadMacro("ini_trks.C");
  cCmd = Form("ini_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d)", iCalSet, iTrackingSetup, iGenCor, dScalFac, dChi2Lim2,
              cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar);
  gInterpreter->ProcessLine(cCmd);

  CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
  Int_t iNStations                = tofFindTracks->GetNStations();

  // =========================================================================
  // ===                       Analysis                                    ===
  // =========================================================================
  /*
  gROOT->LoadMacro("ini_AnaTestbeam.C");
  cCmd=Form("ini_AnaTestbeam(%d,\"%s\",%d,%6.2f,%d,%d)",
   iCalSel, cFileId.Data(), iSel2, 0.9, iAnaCor, iMc);
  */
  //gInterpreter->ProcessLine(cCmd);

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
}
