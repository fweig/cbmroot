/* Copyright (C) 2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Etienne Bechtel [committer] */

R__ADD_INCLUDE_PATH($PWD)

#include "Config_dilepton_testing.C"
TString cfgFunc    = "Config_dilepton_";
TString configName = "testing";

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDefs.h"
#include "CbmMCDataManager.h"
#include "CbmSetup.h"

#include "FairSystemInfo.h"
#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

#include <TStopwatch.h>
#endif


void run_common_analysis(Int_t nEvents = 0, Bool_t test = true, TString setup = "sis100_electron")
{

  // -----   Environment   --------------------------------------------------
  TString myName = "run_analysis";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output directory names   -------------------------------
  TString inDir  = gSystem->Getenv("INDIR");
  TString inFile = gSystem->Getenv("INFILE");  // input file list
  TString outDir = gSystem->Getenv("OUTDIR");
  //  if(outDir.IsNull()) outFile = ".";

  // --- Logger settings ----------------------------------------------------
  gErrorIgnoreLevel = kFatal;  //kInfo, kWarning, kError, kFatal;
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  /// load local copy of setup file (same as used for simulation and reconstruction)
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup::Instance()->LoadSetup(setup);

  CbmSetup::Instance()->Print();


  // -----   run manager + I/O   --------------------------------------------
  std::cout << std::endl;
  FairRunAna* run = new FairRunAna();
  run->SetOutputFile(outDir + "/analysis.root");
  FairFileSource* src = NULL;

  /// stopwatch
  TStopwatch timer;
  timer.Start();

  Int_t i = 0;
  TString file;
  //  char filename[300];
  ifstream in(inFile);
  TList* parList  = new TList();
  TString parFile = "";

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 1);

  TString traFile = "";
  /// loop over all file in list
  if (test) {
    ifstream in(inFile);
    while (in >> file) {
      Int_t n     = 1;
      TString num = "";
      while (true) {
        TString temp = (TString) file(file.Length() - n, file.Length() - 0);
        //      std::cout<<temp<<std::endl;
        if (temp.Contains("/")) break;
        num = temp;
        n++;
      }

      // // mc sim file
      src = new FairFileSource(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".tra.root");
      src->AddFriend(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".event.raw.root");
      src->AddFriend(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".rec.root");

      parFile = file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".par.root";
      //      parFile = "/lustre/cbm/users/ogolosov/mc/cbmsim/commonParFiles/"+file(60,file.Length()-n+1)+"/" + file(file.Length()-n+1,file.Length()-0)+".par.root";

      traFile = file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".tra.root";

      mcManager->AddFile(traFile);

      i++;
    }
  }
  if (!test) {
    ifstream in(outDir + "/" + inFile);
    while (in >> file) {
      Int_t n     = 1;
      TString num = "";
      while (true) {
        TString temp = (TString) file(file.Length() - n, file.Length() - 0);
        //      std::cout<<temp<<std::endl;
        if (temp.Contains("/")) break;
        num = temp;
        n++;
      }

      // mc sim file
      if (!i) src = new FairFileSource(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".tra.root");
      else
        src->AddFile(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".tra.root");
      src->AddFriend(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".event.raw.root");
      src->AddFriend(file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".rec.root");

      parFile = file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".par.root";
      //parFile = "/lustre/cbm/users/ogolosov/mc/cbmsim/commonParFiles/"+file(60,file.Length()-n+1)+"/" + file(file.Length()-n+1,file.Length()-0)+".par.root";

      traFile = file + "/" + file(file.Length() - n + 1, file.Length() - 0) + ".tra.root";

      mcManager->AddFile(traFile);

      i++;
    }
  }

  //  parList->Dump();
  // add source to run
  run->SetSource(src);

  // // ------------------------------------------------------------------------

  //  -----   MCDataManager (legacy mode)  -----------------------------------
  run->AddTask(mcManager);
  //  ------------------------------------------------------------------------

  CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
  run->AddTask(match1);


  // -----   L1/KF tracking + PID   -----------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading tasks " << std::endl;

  //CbmKF is needed for Extrapolation
  CbmKF* kf = new CbmKF();
  run->AddTask(kf);
  std::cout << "-I- : Added task " << kf->GetName() << std::endl;

  CbmL1* l1 = new CbmL1();
  // --- Material budget file names
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kMvd)) {
    TString geoTag;
    CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMvd, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile         = matFile + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(matFile.Data());
  }
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kSts)) {
    TString geoTag;
    CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kSts, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile         = matFile + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    //    matFile = matFile + "/parameters/sts/sts_matbudget_v19a.root";
    //    matFile = matFile + "/parameters/sts/sts_matbudget_v19i.root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetStsMaterialBudgetFileName(matFile.Data());
    //  }
    run->AddTask(l1);
    std::cout << "-I- : Added task " << l1->GetName() << std::endl;
  }

  //  --- TRD pid tasks
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd)) {
    CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
    trdLI->SetUseMCInfo(kTRUE);
    trdLI->SetUseMomDependence(kTRUE);
    run->AddTask(trdLI);
    std::cout << "-I- : Added task " << trdLI->GetName() << std::endl;
    // ------------------------------------------------------------------------
  }

  // -----   PAPA tasks   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading private tasks " << std::endl;

  TString cfgPath  = outDir + "/../";
  TString cfgFile  = cfgPath + cfgFunc + configName + ".C";  //cfgPath + cfgFunc + configName + ".C";
  TString cfgFunct = cfgFunc + configName + "(\"" + configName + "\")";
  TString cfg      = cfgFunc + configName + "()";
  gROOT->LoadMacro((cfgPath + cfgFunct + configName + ".C"));
  FairTask* task = reinterpret_cast<FairTask*>(gROOT->ProcessLine(cfg));
  run->AddTask(task);

  // // ------------------------------------------------------------------------

  // set parameter list
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  //  parIo1->open(parFile.Data(),"UPDATE");
  parIo1->open(parFile.Data(), "READ");
  //  parIo1->open(parList);
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  //  rtdb->clearRunList();
  rtdb->saveOutput();

  /// intialize and run
  run->Init();
  run->Run(0, nEvents);

  timer.Stop();
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << " Output file is " << (outDir + "analysis.root") << std::endl;
  //  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
