/** @file cosy2019_calib.C
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2019)
// Only STS local reconstruction (cluster + hit finder) for the time being
//
//
//
// --------------------------------------------------------------------------


void cosy2019_calib(Int_t nTimeslices = -1, UInt_t uRunId = 0, TString outDir = "data",
                    TString dataSet = "data/cosy2019"  // Data set for file names
)
{
  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "cosy2019_calib";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // TString srcDir1 = gSystem->Getenv("SLURM_INDEX");
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString runId   = TString::Format("%04u", uRunId);
  TString inFile  = outDir + "/unp_cosy_" + runId + ".root";
  TString parFile = outDir + "/unp_cosy_params_" + runId + "_sts.root";
  TString outFile = dataSet + "_" + runId + ".cal.root";
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

  TString monitorFile {outFile};
  monitorFile.ReplaceAll("rec", "rec.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // -----------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Calibration tasks   --------------------------------------------
  CbmOffsetDigiTime<CbmStsDigi>* pCalSts = new CbmOffsetDigiTime<CbmStsDigi>("StsDigi", "StsDigi", "Calib. STS Digis");

  /// List of time offset to apply
  pCalSts->AddOffsetPoint(3916, -169593);
  ///                         ^          ^
  ///                        TS        Offset

  /// Apply the offsets only to STS
  pCalSts->AddAddressToOffset(0x10008012);  // Front side
  pCalSts->AddAddressToOffset(0x12008012);  // Back side

  run->AddTask(pCalSts);
  std::cout << "-I- : Added task " << pCalSts->GetName() << std::endl;
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
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
