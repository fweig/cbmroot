/* Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2019)
// Only STS local reconstruction (cluster + hit finder) for the time being
//
//
//
// --------------------------------------------------------------------------


void mcbm_reco(Int_t nTimeslices = 10)
{


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_reco";                    // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // TString srcDir1 = gSystem->Getenv("SLURM_INDEX");  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString inFile  = "data/unp_mcbm_4.root";
  TString parFile = "data/unp_mcbm_params_4.root";
  TString outFile = "data/reco_mcbm_4.root";
  //  outFile = Form("%s_%i.root",outFile.Data(),(int)(srcDir1.Atoi()*100));
  TString geoFile = "data/test.geo.root";  // to be created by a simulation run
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run     = new FairRunAna();
  CbmStsFindHits* hit = new CbmStsFindHits();

  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);

  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGeomFile(geoFile);

  TString monitorFile {outFile};
  monitorFile.ReplaceAll("rec", "rec.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // -----------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  CbmStsReco* stsReco = new CbmStsReco();
  // --- The parameter file for the STS Setup sensors needs to be explicitely set as
  // --- 1) By default the CbmStsSetup creates only Stereo sensors with a 58 um pitch
  // --- 2) The only place where this can be set is in the init of the CbmStsReco

  // -----  Geometry Tags  --------------------------------------------------
  TString hodoGeoTag  = "hodo_v19a_mcbm";  // 2019 LAB test
  TString sHodoGeoPar = hodoGeoTag + ".par";
  // ------------------------------------------------------------------------
  stsReco->SetSensorsParFile(sHodoGeoPar);

  run->AddTask(stsReco);
  std::cout << "-I- : Added task " << stsReco->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nTimeslices);
  //run->Run(0,nEvents);
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
  //  RemoveGeoManager();
  // ------------------------------------------------------------------------
}
