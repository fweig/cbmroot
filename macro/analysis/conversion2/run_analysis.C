/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated raw data
//
// Default event-by-event reconstruction chain,
// defined in macro/run/modules/reconstruct.C
//
// The following naming conventions are assumed:
// Raw data file:  [dataset].event.raw.root
// Transport file: [dataset].tra.root
// Parameter file: [dataset].par.root
// Output file:    [dataset].rec.root
//
// V. Friese   10/06/2018
//
// --------------------------------------------------------------------------


void run_analysis(Int_t nEvents = 2, TString setupName = "sis100_electron", const char* index = "00001",
                  TString dataset = "data/test")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_event";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString rawFile = dataset + "_" + setupName + ".event.raw." + index + ".root";
  TString traFile = dataset + "_" + setupName + ".tra." + index + ".root";
  TString parFile = dataset + "_" + setupName + ".par." + index + ".root";
  TString recFile = dataset + "_" + setupName + ".rec." + index + ".root";
  TString outFile = dataset + "_" + setupName + ".analysis." + index + ".root";
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(kTof, geoTag)) {
    TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
    parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofFile->GetString() << std::endl;
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  inputSource->AddFriend(traFile);
  inputSource->AddFriend(recFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kFALSE);
  // ------------------------------------------------------------------------

  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);

  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);

  CbmL1* l1 = new CbmL1();
  if (CbmSetup::Instance()->IsActive(kMvd)) {
    CbmSetup::Instance()->GetGeoTag(kMvd, geoTag);
    const TString mvdMatBudgetFileName = srcDir + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if (CbmSetup::Instance()->IsActive(kSts)) {
    CbmSetup::Instance()->GetGeoTag(kSts, geoTag);
    const TString stsMatBudgetFileName = srcDir + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);

  // —----------------------------------------------------------------------
  ///// add you task here
  CbmKresConversionMain* conversionKres = new CbmKresConversionMain();
  run->AddTask(conversionKres);
  // —----------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------


  // -----   Function needed for CTest runtime dependency   -----------------
  // RemoveGeoManager();
  // ------------------------------------------------------------------------
}
