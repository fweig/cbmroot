/** @file check_coinc_Hodo.C
  * @copyright Copyright (C) 2021 GSI, IKF-UFra
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alberica Toia [orginator] **/

void check_coinc_Hodo(Int_t nEvents = 10, UInt_t run = 25, UInt_t prefix = 0)
{
  TString runId    = TString::Format("%04u", run);
  TString prefixId = TString::Format("%04u", prefix);
  // TString fileName="/lustre/cbm/users/alberica/cosy2019/rec/TESTrec_cosy_";
  //   fileName = fileName + runId + "_" + prefixId + ".root";
  TString fileName = "0025.rec.root";
  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 1;

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
  FairRunOnline* fRun = new FairRunOnline();
  fRun->ActivateHttpServer(100, 8080);  // refresh each 100 events

  FairFileSource* inputSource = new FairFileSource(fileName);
  fRun->SetSource(inputSource);

  // Define output file for FairMonitor histograms
  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------

  CbmStsCoincHodo* timeChecker = new CbmStsCoincHodo();
  timeChecker->SetTsStart(0);
  timeChecker->SetTsStop(5000);
  timeChecker->SetOutFilename(Form("testCoincHodo_%i_%i_2_8.root", run, prefix));
  fRun->AddTask(timeChecker);


  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  cout << "Starting run" << endl;
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

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

  //  RemoveGeoManager();
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
