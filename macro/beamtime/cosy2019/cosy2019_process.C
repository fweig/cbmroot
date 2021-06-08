/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Alberica Toia */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of cosy real data (2019)
// Only STS local reconstruction (cluster + hit finder) for the time being
//
//
//
// --------------------------------------------------------------------------


void cosy2019_process()
{

  Int_t nTimeslices = -1;
  TString inFile    = "unp_cosy_0025.root";
  TString parFile   = "unp_cosy_params_0025.root";
  TString outFile   = "0025.rec.root";
  TString geoFile   = "sts_hodo_v19a_cosy_geo.root";


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "cosy2019_process";             // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // TString srcDir1 = gSystem->Getenv("SLURM_INDEX");  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  //TString runId  = TString::Format("%04u", uRunId);
  //TString inFile = outDir + "/unp_cosy_" + runId + ".root";
  //if (eventMode) inFile = dataSet + ".event.raw.root";
  //TString parFile = outDir + "/unp_cosy_params_" + runId + "_sts.root";
  //TString outFile = dataSet + "_" + runId + ".rec.root";
  //TString geoFile = dataSet + ".geo.root";  // to be created by a simulation run
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
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
  CbmRecoSts* recoSts = new CbmRecoSts();
  recoSts->SetTimeCutDigisAbs(20);      // cluster finder: time cut in ns
  recoSts->SetTimeCutClustersAbs(20.);  // hit finder: time cut in ns

  // ----   Here we define the sensor parameters since they are not available
  // -----  from the runtimeDb.(#hateRuntimeDb)

  // ---- Hodoscope "sensor"
  CbmStsParSensor hodoSensPar(CbmStsSensorClass::kDssdOrtho);
  hodoSensPar.SetPar(0, 6.5);  // extension in x
  hodoSensPar.SetPar(1, 6.5);  // extension in y
  hodoSensPar.SetPar(2, 0.4);  // extension in z
  hodoSensPar.SetPar(3, 6.4);  // active size in y
  hodoSensPar.SetPar(4, 64);   // number of strips front side
  hodoSensPar.SetPar(5, 64);   // number of strips back side
  hodoSensPar.SetPar(6, 0.1);  // strip pitch front side
  hodoSensPar.SetPar(7, 0.1);  // strip pitch back side

  // ---- STS sensor (DUT)
  CbmStsParSensor stsSensPar(CbmStsSensorClass::kDssdStereo);
  stsSensPar.SetPar(0, 6.2092);  // extension in x
  stsSensPar.SetPar(1, 6.2000);  // extension in y
  stsSensPar.SetPar(2, 0.0300);  // extension in z
  stsSensPar.SetPar(3, 5.9600);  // active size in y
  stsSensPar.SetPar(4, 1024);    // number of strips front side
  stsSensPar.SetPar(5, 1024);    // number of strips back side
  stsSensPar.SetPar(6, 0.0058);  // strip pitch front side
  stsSensPar.SetPar(7, 0.0058);  // strip pitch back side
  stsSensPar.SetPar(8, -7.5);    // strip pitch front side
  stsSensPar.SetPar(9, 0.0);     // strip pitch back side

  // --- Addresses for sensors
  // --- They are defined in each station as sensor 1, module 1, halfladderD (2), ladder 1
  Int_t hodo1Address = CbmStsAddress::GetAddress(0, 0, 1, 0, 0);  // station 1
  Int_t stsAddress   = CbmStsAddress::GetAddress(1, 0, 1, 0, 0);  // station 2
  Int_t hodo2Address = CbmStsAddress::GetAddress(2, 0, 1, 0, 0);  // station 3
  std::cout << "Hodo 1 address " << std::dec << hodo1Address << " " << std::hex << hodo1Address << std::endl;
  std::cout << "STS    address " << std::dec << stsAddress << " " << std::hex << stsAddress << std::endl;
  std::cout << "Hodo 2 address " << std::dec << hodo2Address << " " << std::hex << hodo2Address << std::endl;

  // --- Now we can define the sensor parameter set and tell recoSts to use it
  auto sensorParSet = new CbmStsParSetSensor("CbmStsParSetSensor", "STS sensor parameters"
                                                                   "cosy2019");
  sensorParSet->SetParSensor(hodo1Address, hodoSensPar);
  sensorParSet->SetParSensor(stsAddress, stsSensPar);
  sensorParSet->SetParSensor(hodo2Address, hodoSensPar);
  recoSts->UseSensorParSet(sensorParSet);

  // --- Next we define the module and ASIC parameters. The same can be used for all sensors.
  CbmStsParAsic asicPar(32, 75000., 3000., 5., 800., 1000., 3.9789e-3);
  auto modulePar = new CbmStsParModule(2048, 128);
  modulePar->SetAllAsics(asicPar);
  recoSts->UseModulePar(modulePar);

  // --- We also need the sensor conditions, although they are of no relevance here
  // --- without magnetic field (they are used to determine the Lorentz shift).
  auto sensorCond = new CbmStsParSensorCond(70., 140., 268., 17.5, 1.);
  recoSts->UseSensorCond(sensorCond);

  run->AddTask(recoSts);
  std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
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
