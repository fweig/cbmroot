/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void ana_digi_cal_all(Int_t nEvents = 10000000, Int_t calMode = 93, Int_t calSel = 0, Int_t calSm = 900,
                      Int_t RefSel = 1, TString cFileId = "Test", Int_t iCalSet = 910601600, Bool_t bOut = 0,
                      Int_t iSel2 = 0, Double_t dDeadtime = 50, TString cCalId = "XXX", Int_t iPlot = 1)
{
  Int_t iVerbose = 1;
  Int_t iBugCor  = 0;
  Int_t iFirstEvent = 0;

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
  //gLogger->SetLogVerbosityLevel("VERYHIGH");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  /*
   TString workDir    = (TString)gInterpreter->ProcessLine(".! pwd");
   cout << "workdir = "<< workDir.Data() << endl;
   return;
  */
  TString paramDir = workDir + "/macro/beamtime/mcbm2021/";
  //TString paramDir   = "./";
  TString ParFile   = paramDir + "data/" + cFileId + ".params.root";
  TString InputFile = paramDir + "RawDataIn/" + cFileId + ".root";
  // TString InputFile  =  "./data/" + cFileId + ".root";
  TString OutputFile = paramDir + "data/TofHits_" + cFileId + Form("_%09d_%03d_%02.0f_Cal", iCalSet, iSel2, dDeadtime)
                       + cCalId + ".out.root";

  TString shcmd = "rm -v " + ParFile;
  gSystem->Exec(shcmd.Data());

  TList* parFileList = new TList();

  TString FId = cFileId;
  Int_t iNLen = FId.First(".");
  if (iNLen <= 0) iNLen = FId.Length();
  TString cRun(FId(0, iNLen));
  Int_t iRun     = cRun.Atoi();
  cout << "FileId " << cFileId << ", Run " << iRun << endl;
  TString TofGeo = "";
  if (iRun < 690) TofGeo = "v20a_mcbm";
  else {
    if (iRun < 1112) { TofGeo = "v21a_mcbm"; }
      if (iRun < 1400) { TofGeo = "v21b_mcbm"; }
      else {
        if (iRun < 1400) { TofGeo = "v21b_mcbm"; }
        else {
          if (iRun < 2000) { TofGeo = "v21d_mcbm"; }
          else {
            if (iRun < 2150) { TofGeo = "v21e_mcbm"; }
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

  if (nEvents > -1) {
    if (iRun > 10000) {
      iFirstEvent = 2000000;  // late start of Buc ...
      if (iRun > 1050) iFirstEvent = 10000000;
      nEvents += iFirstEvent;
    }
  }


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


  gROOT->LoadMacro("save_hst.C");
  TString FSave = Form("save_hst(\"CluStatus%d_%d_Cal_%s.hst.root\")", iCalSet, iSel2, cCalId.Data());
  gInterpreter->ProcessLine(FSave.Data());

  //if(calMode%10 >7) return;

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
  gROOT->LoadMacro("pl_all_Cal2D.C");

  if (iPlot) {

    switch (calMode % 10) {
      case 9:
        for (Int_t iOpt = 0; iOpt < 7; iOpt++) {
          gInterpreter->ProcessLine(Form("pl_all_Cal2D(%d)", iOpt));
        }
        break;

      default:
        for (Int_t iOpt = 0; iOpt < 7; iOpt++) {
          for (Int_t iSel = 0; iSel < 2; iSel++) {
            gInterpreter->ProcessLine(Form("pl_all_Sel2D(%d,%d)", iOpt, iSel));
          }
        }

        for (Int_t iOpt = 6; iOpt < 10; iOpt++) {
          gInterpreter->ProcessLine(Form("pl_all_2D(%d)", iOpt));
        }

        /*	
        gInterpreter->ProcessLine("pl_all_CluMul()");
        gInterpreter->ProcessLine("pl_all_CluRate()");
        gInterpreter->ProcessLine("pl_all_CluRate(5,1)");
        gInterpreter->ProcessLine("pl_all_CluPosEvol()");
        gInterpreter->ProcessLine("pl_all_CluTimeEvol()");
        gInterpreter->ProcessLine("pl_all_dTSel()");
        */
        break;
        ;
    }
  }
}
