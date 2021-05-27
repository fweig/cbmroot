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

void EfficiencyMonitor(TString inFile = "", Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080,
                       Int_t iStartFile = -1, Int_t iStopFile = -1, Bool_t bEnableDeadCorr = kFALSE)
{

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/eff_out.root";
  TString parFile = "data/eff_param.root";

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
  CbmCosy2018MonitorEfficiency* monitorEff = new CbmCosy2018MonitorEfficiency();
  monitorEff->SetHistoFileName("data/EfficiencyHistos.root");
  //  monitorEff->SetPrintMessage();
  monitorEff->SetMsOverlap();
  monitorEff->EnableDualStsMode(kTRUE);
  //  monitorEff->SetLongDurationLimits( 3600, 10 );
  monitorEff->SetLongDurationLimits(7200, 60);
  /*
  monitorEff->SetCoincidenceBorderHodo(   0.0,  50 );
  monitorEff->SetCoincidenceBorderSts1(   0.0,  75 );
  monitorEff->SetCoincidenceBorderSts2(   0.0,  75 );
*/
  monitorEff->SetCoincidenceBorderHodo(0.0, 25);
  monitorEff->SetCoincidenceBorderHodoBoth(12.0, 50);
  monitorEff->SetCoincidenceBorderSts1(-10.0, 40);
  monitorEff->SetCoincidenceBorderSts2(-27.5, 40);
  monitorEff->SetCoincidenceBorderHodoSts1(0.0, 20);
  monitorEff->SetCoincidenceBorderHodoSts2(-30.0, 20);
  monitorEff->SetCoincidenceBorder(0.0, 150);
  monitorEff->SetStripsOffset1(-64, -61);
  monitorEff->SetStripsOffset2(-64, -61);
  monitorEff->SetPositionsMmZ(1230.0, 1620.0, 1350.0, 1470.0);
  monitorEff->SetPositionOffsetSts1(-3.97, -17.85);
  monitorEff->SetPositionOffsetSts2(-1.57, -6.68);
  monitorEff->EnableDeadCorr(bEnableDeadCorr);

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

  source->AddUnpacker(monitorEff, 0x10, 6);  // stsXyter DPBs

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
