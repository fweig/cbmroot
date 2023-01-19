/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

// --------------------------------------------------------------------------
//
// Macro for local MVD reconstruction from MC data
//
// Tasks:  CbmMvdHitProducer
//
//
// V. Friese   06/02/2007
//
// --------------------------------------------------------------------------
void mvd_qa4_reco(const char* setup = "sis100_electron")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  TString inDir = gSystem->Getenv("VMCWORKDIR");

  TString outDir = "data/";
  // Input file (MC events)
  TString inFile = outDir + "mvd.rawQA.root";

  // Parameter file name
  TString parFile = outDir + "params.root";

  // Output file
  TString outFile = outDir + "mvd.recoQA.root";

  // Background file (MC events, for pile-up)
  TString bgFile = inFile;

  // Delta file (Au Ions)
  TString deltaFile = outDir + "mvd.mcDelta.root";

  // Number of events to process
  Int_t nEvents = 5;

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");


  TString setupFile  = inDir + "/geometry/setup/setup_" + setup + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setup + "()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 1;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun = new FairRunAna();

  FairFileSource* inputSource = new FairFileSource(inFile);
  fRun->SetSource(inputSource);

  fRun->SetOutputFile(outFile);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) { FairMonitor::GetMonitor()->EnableMonitor(kTRUE); }
  // ------------------------------------------------------------------------

  // ----- Mc Data Manager   ------------------------------------------------
  //CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  //mcManager->AddFile(inFile);
  //fRun->AddTask(mcManager);
  // ------------------------------------------------------------------------


  // -----   MVD Clusterfinder   ---------------------------------------------
  CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose);
  fRun->AddTask(mvdCluster);
  // -------------------------------------------------------------------------


  CbmMvdHitfinder* mvd_hit = new CbmMvdHitfinder("MVDFindHits", 0, iVerbose);
  mvd_hit->UseClusterfinder(kTRUE);
  fRun->AddTask(mvd_hit);

  //----------------------------------------------------------------------------
  // -----  Parameter database   -----------------------------------------------
  FairRuntimeDb* rtdb       = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());

  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ---------------------------------------------------------------------------


  // -----   Run initialisation   ----------------------------------------------
  fRun->Init();
  // ---------------------------------------------------------------------------


  // -----   Start run   -------------------------------------------------------
  fRun->Run(0, nEvents);
  // ---------------------------------------------------------------------------


  // -----   Finish   ----------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ---------------------------------------------------------------------------

  if (hasFairMonitor) {
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory = sysInfo.GetMaxMemory();
    cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    cout << maxMemory;
    cout << "</DartMeasurement>" << endl;

    Float_t cpuUsage = ctime / rtime;
    cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    cout << cpuUsage;
    cout << "</DartMeasurement>" << endl;

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }
  //  delete run;

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

  RemoveGeoManager();
}
