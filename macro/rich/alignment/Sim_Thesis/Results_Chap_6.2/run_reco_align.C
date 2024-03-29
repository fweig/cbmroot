/* Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

void run_reco_align(Int_t nEvents)
{
  TTree::SetMaxTreeSize(90000000000);
  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_reco";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString setupName = "setup_misalign_3mrad_minusX", outDir = "/data/Sim_Outputs/Results_Chap_6.2/3mrad_minusX/";
  //    TString setupName = "setup_align", outDir = "/data/Sim_Outputs/Results_Chap_6.2/Align/";

  TString parFile   = outDir + setupName + "_param.root";
  TString mcFile    = outDir + setupName + "_mc.root";
  TString geoFile   = outDir + setupName + "_geofilefull.root";
  TString recoFile  = outDir + setupName + "_reco.root";
  TString resultDir = outDir;
  TString outDir2   = outDir;

  TString geoSetupFile = "";
  geoSetupFile         = "/data/ROOT6/trunk/macro/rich/alignment/"
                 "misalignment_correction/Sim_Thesis/Results_Chap_6.2/geosetup/"
                 + setupName + ".C";

  if (script == "yes") {
    setupName    = TString(gSystem->Getenv("SETUP_NAME"));
    geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + TString(gSystem->Getenv("GEO_SETUP_FILE"));
    outDir       = TString(gSystem->Getenv("DIR"));

    mcFile    = TString(gSystem->Getenv("MC_FILE"));
    parFile   = TString(gSystem->Getenv("PAR_FILE"));
    recoFile  = TString(gSystem->Getenv("RECO_FILE"));
    resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));
  }

  remove(recoFile.Data());

  // -----   Load the geometry setup   --------------------------------------
  const char* setupName2 = setupName;
  TString setupFunct     = "";
  setupFunct             = setupFunct + setupName2 + "()";
  std::cout << "-I- setupFile: " << geoSetupFile << std::endl << "-I- setupFunct: " << setupFunct << std::endl;
  gROOT->LoadMacro(geoSetupFile);
  gROOT->ProcessLine(setupFunct);
  //gInterpreter->ProcessLine(setupFunct);
  // ------------------------------------------------------------------------


  CbmSetup* setup = CbmSetup::Instance();

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(mcFile);
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile);
  run->SetGenerateRunInfo(kTRUE);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");


  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);


  // Digitisers
  std::cout << std::endl;
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/digitize.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("digitize()");


  // Reconstruction tasks
  std::cout << std::endl;
  macroName = srcDir + "/macro/run/modules/reconstruct_align.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct_align()");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName << std::endl;
    return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " executed successfully" << std::endl;

  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);


  // Finish
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
