/** @file run_testing.C
  * @copyright Copyright (C) 2021 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Etienne Bechtel [orginator] **/

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


void run_testing(Int_t nEvents = 0, Bool_t test = true, Int_t id = 1, TString TASK = "",
                 TString setup = "sis100_electron")
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


  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup::Instance()->LoadSetup(setup);

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
      // mc sim file
      if (!i) src = new FairFileSource(file + "/tra.root");
      src->AddFriend(file + "/raw.root");
      src->AddFriend(file + "/rec.root");
      parFile = file + "/par.root";

      mcManager->AddFile(traFile);

      i++;
    }
  }
  if (!test) {
    ifstream in(outDir + "/" + inFile);
    while (in >> file) {

      if (!i) src = new FairFileSource(file + "/tra.root");
      src->AddFriend(file + "/raw.root");
      src->AddFriend(file + "/rec.root");
      parFile = file + "/par.root";
      //      parFile = outDir + "/../../par.root";

      mcManager->AddFile(traFile);

      i++;
    }
  }

  //  parList->Dump();
  // add source to run
  run->SetSource(src);

  // // ------------------------------------------------------------------------

  //   -----   MCDataManager (legacy mode)  -----------------------------------
  run->AddTask(mcManager);
  //  ------------------------------------------------------------------------

  // CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
  // run->AddTask(match1);


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
  TString cfg      = cfgFunc + configName + "(" + std::to_string(id) + ",\"" + TASK + "\")";
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
