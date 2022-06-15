/* Copyright (C) 2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Etienne Bechtel [committer] */

//R__ADD_INCLUDE_PATH(/lustre/nyx/cbm/users/ebechtel/analysis)
R__ADD_INCLUDE_PATH($PWD)
#include "Config_dilepton_testing.C"

void run_analysis(Int_t nEvents = 0)
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
  // ------------------------------------------------------------------------

  // --- Logger settings ----------------------------------------------------
  gErrorIgnoreLevel = kFatal;  //kInfo, kWarning, kError, kFatal;
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  /// load local copy of setup file (same as used for simulation and reconstruction)
  std::cout << std::endl;
  TString setupName = gSystem->GetFromPipe(Form("echo $(basename $(ls %s/setup_*.C))", inDir.Data()));
  setupName.ReplaceAll("setup_", "");
  setupName.ReplaceAll(".C", "");

  TString setupFile  = inDir + "/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;

  //CbmSetup* setup = reinterpret_cast<CbmSetup*>(gInterpreter->ProcessLine(Form(".x %s",setupFile.Data())));

  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();


  // // ------------------------------------------------------------------------


  // // -----   Modify Cbm setup   ---------------------------------------------
  //   //  std::cout << std::endl;
  // // --- remove detector geometries
  // //  setup->RemoveModule(kPSD);      // remove psd from setup
  // // --- change default geomerties
  // //  setup->SetModule(kTRD, "v17n_1e", kTRUE); // 4 TRD layer
  // //  std::cout << "-I- " << myName << ": CbmSetup updated " << std::endl;
  // // ------------------------------------------------------------------------

  // -----   run manager + I/O   --------------------------------------------
  std::cout << std::endl;
  FairRunAna* run = new FairRunAna();
  run->SetOutputFile(outDir + "/" + setupName + "_analysis.root");
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

  /// loop over all file in list
  while (in >> file) {
    file += "/";

    // mc sim file
    if (!i) src = new FairFileSource(file + "tra.root");
    else
      src->AddFile(file + "tra.root");

    // (skimmed) reco file
    src->AddFriend(file + "event.raw.root");
    src->AddFriend(file + "rec.root");

    parFile = file + "par.root";

    i++;
  }

  // add source to run
  run->SetSource(src);


  // -----   L1/KF tracking + PID   -----------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading tasks " << std::endl;

  run->AddTask(new CbmTrackingDetectorInterfaceInit());
  //CbmKF is needed for Extrapolation
  CbmKF* kf = new CbmKF();
  run->AddTask(kf);
  std::cout << "-I- : Added task " << kf->GetName() << std::endl;

  CbmL1* l1 = new CbmL1();
  // --- Material budget file names
  if (setup->IsActive(kMvd)) {
    TString geoTag;
    setup->GetGeoTag(kMvd, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile         = matFile + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(matFile.Data());
  }
  if (setup->IsActive(kSts)) {
    TString geoTag;
    setup->GetGeoTag(kSts, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile         = matFile + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetStsMaterialBudgetFileName(matFile.Data());
  }
  run->AddTask(l1);
  std::cout << "-I- : Added task " << l1->GetName() << std::endl;


  //  --- TRD pid tasks
  if (setup->IsActive(kTrd)) {
    //  //    ----------- TRD track Pid Ann ----------------------
    // CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("Ann", "Ann");
    // run->AddTask(trdSetTracksPidAnnTask);
    // std::cout << "-I- : Added task " << trdSetTracksPidAnnTask->GetName() << std::endl;
    // //    ----------------------------------------------------

    // //    ----------- TRD track Pid Like ----------------------
    // FairTask *getTRDli = dynamic_cast<FairTask*>( gROOT->Macro( "$VMCWORKDIR/macro/papa/Config_trd_PidLI.C" ));
    // run->AddTask( getTRDli );
    // std::cout << "-I- : Added task " << getTRDli->GetName() << std::endl;

    CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
    trdLI->SetUseMCInfo(kTRUE);
    trdLI->SetUseMomDependence(kFALSE);
    run->AddTask(trdLI);
    std::cout << "-I- : Added task " << trdLI->GetName() << std::endl;
    // ------------------------------------------------------------------------
  }

  // -----   PAPA tasks   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading private tasks " << std::endl;

  TString cfgPath    = outDir + "/../";
  TString cfgFunc    = "Config_dilepton_";
  TString configName = "testing";
  TString cfgFile    = cfgPath + cfgFunc + configName + ".C";  //cfgPath + cfgFunc + configName + ".C";
  TString cfgFunct   = cfgFunc + configName + "(\"" + configName + "\")";
  TString cfg        = cfgFunc + configName + "()";

  gROOT->LoadMacro((cfgPath + cfgFunct + configName + ".C"));
  FairTask* task = reinterpret_cast<FairTask*>(gROOT->ProcessLine(cfg));
  run->AddTask(task);

  // // ------------------------------------------------------------------------

  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parList->IsEmpty()) {
    parIo2->open(parList, "in");
    rtdb->setSecondInput(parIo2);
  }

  // /// fair runtime database
  // FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // rtdb->setFirstInput(parIo1);
  // rtdb->setSecondInput(parIo2);
  // rtdb->setOutput(parIo1);
  // rtdb->saveOutput();

  /// intialize and run
  run->Init();
  run->Run(0, nEvents);

  timer.Stop();
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << " Output file is " << (outDir + setupName + "_analysis.root") << std::endl;
  //  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
