/** @file SetupMonitorCosmics.C
  * @copyright Copyright (C) 2017-2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/** @file Cern2017Monitor.C
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 26.07.2017
 **
 ** ROOT macro to read tsa files which have been produced with StsXyter + DPB + FLIB
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void SetupMonitorCosmics(TString inFile = "", Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8083,
                         Int_t iStartFile = -1, Int_t iStopFile = -1)
{
  /*
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  TString inDir  = srcDir + "/input/";
  if( "" != inFile )
   inFile = inDir + inFile;
*/
  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  //  Int_t nEvents = 10000;
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/setup_cosmics_out.root";
  TString parFile = "data/setup_cosmics_param.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileHodo          = paramDir + "HodoUnpackPar.par";
  TObjString* tutDetDigiFileHodo = new TObjString(paramFileHodo);
  parFileList->Add(tutDetDigiFileHodo);

  TString paramFileSts          = paramDir + "StsUnpackPar.par";
  TObjString* tutDetDigiFileSts = new TObjString(paramFileSts);
  parFileList->Add(tutDetDigiFileSts);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: Initialising..." << std::endl;

  // Hodoscopes Monitor
  CbmCosy2018MonitorSetup* monitorSetup = new CbmCosy2018MonitorSetup();
  monitorSetup->SetHistoFileName("data/SetupCosmicsHistos.root");
  //  monitorSetup->SetPrintMessage();
  monitorSetup->SetMsOverlap();
  monitorSetup->EnableDualStsMode(kTRUE);
  //  monitorSetup->SetLongDurationLimits( 3600, 10 );
  monitorSetup->SetLongDurationLimits(288000, 60);
  monitorSetup->SetCoincidenceBorder(150);

  // --- Source task
  CbmTofStar2018Source* source = new CbmTofStar2018Source();
  if ("" != inFile) {
    if (0 <= iStartFile && iStartFile < iStopFile) {
      for (Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx) {
        TString sFilePath = Form("%s_%04u.tsa", inFile.Data(), iFileIdx);
        source->AddFile(sFilePath);
        std::cout << "Added " << sFilePath << std::endl;
      }  // for( Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx )
    }    // if( 0 < iStartFile && 0 < iStopFile )
    else
      source->SetFileName(inFile);
  }  // if( "" != inFile )
  else {
    source->SetHostName("cbmin002");
    source->SetPortNumber(5556);
  }

  source->AddUnpacker(monitorSetup, 0x10, 6);  // stsXyter DPBs

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> Cern2017Monitor: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> Cern2017Monitor: Macro finished successfully." << std::endl;
  std::cout << ">>> Cern2017Monitor: Output file is " << outFile << std::endl;
  std::cout << ">>> Cern2017Monitor: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
