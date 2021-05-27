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

void McbmSyncMonitor(TString inFile = "", TString sHostname = "pn05", Int_t iStartFile = -1, Int_t iStopFile = -1,
                     Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080, UInt_t uRunId = 0,
                     Bool_t bWithOffset = kFALSE, Bool_t bWithTs = kFALSE, Bool_t bSpillAna = kFALSE)
{
  TString sFileTag = "";
  if (0 < uRunId) sFileTag = Form("_%u_%s", uRunId, sHostname.Data());
  if (kTRUE == bWithOffset) sFileTag += "_Offs";

  if (kTRUE == bSpillAna) {
    sFileTag += "_Spill";
    bWithTs = kTRUE;
  }  // if( kTRUE == bSpillAna )
  else if (kTRUE == bWithTs)
    sFileTag += "_Ts";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/sync_out" + sFileTag + ".root";
  TString parFile = "data/sync_param" + sFileTag + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileSts          = paramDir + "mStsMuchPar.par";
  TObjString* tutDetDigiFileSts = new TObjString(paramFileSts);
  parFileList->Add(tutDetDigiFileSts);

  TString paramFileTof          = paramDir + "mTofPar.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: Initialising..." << std::endl;

  // Hodoscopes Monitor
  CbmMcbm2018MonitorMcbmSync* monitorPulser = new CbmMcbm2018MonitorMcbmSync();
  monitorPulser->SetHistoFileName("data/McbmSyncHistos" + sFileTag + ".root");
  monitorPulser->SetIgnoreMsOverlap();

  if (kTRUE == bWithOffset) {
    switch (uRunId) {
      case 48:
        monitorPulser->SetStsTofOffsetNs(43900);   // Run 48
        monitorPulser->SetMuchTofOffsetNs(12000);  // Run 48
        break;
      case 49:
        monitorPulser->SetStsTofOffsetNs(11900);   // Run 49
        monitorPulser->SetMuchTofOffsetNs(-2300);  // Run 49
        break;
      case 51:
        monitorPulser->SetStsTofOffsetNs(165450);  // Run 51, no peak in same MS, peak at ~162 us in same TS
        monitorPulser->SetMuchTofOffsetNs(
          850);  // Run 51, no peak in same MS for full run, peak around -850 ns in last spills
        break;
      case 52:
        monitorPulser->SetStsTofOffsetNs(141500);  // Run 52, no peak in same MS, peak at ~104 us in same TS
        monitorPulser->SetMuchTofOffsetNs(18450);  // Run 52
        break;
      case 53:
        monitorPulser->SetStsTofOffsetNs(101500);  // Run 53
        monitorPulser->SetMuchTofOffsetNs(2400);   // Run 53
        break;
      default: break;
    }  // switch( uRunId )
  }    // if( kTRUE == bWithOffset )

  monitorPulser->SetTsLevelAna(bWithTs);

  if (kTRUE == bSpillAna) {
    switch (uRunId) {
      case 48:
        monitorPulser->SetSpillLimits(41.8, 44.6, 47.4);  // 48
        break;
      case 49:
        monitorPulser->SetSpillLimits(48.6, 51.3, 54.1);  // 49
        break;
      case 51:
        //            monitorPulser->SetSpillLimits( 29.4, 32.2, 34.8 ); // 51
        monitorPulser->SetSpillLimits(3572.3, 3575.0, 3577.8);  // 51
        break;
      case 52:
        monitorPulser->SetSpillLimits(58.6, 61.4, 64.1);  // 52
        break;
      case 53:
        //            monitorPulser->SetSpillLimits( 56.7, 59.2, 62.2 ); // 53
        monitorPulser->SetSpillLimits(885.6, 888.4, 891.2);  // 53
        break;
      default: break;
    }  // switch( uRunId )
  }    // if( kTRUE == bSpillAna )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
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
    source->SetHostName(sHostname);
    source->SetPortNumber(5556);
  }

  source->AddUnpacker(monitorPulser, 0x10, 6);  // sDPBs
  source->AddUnpacker(monitorPulser, 0x60, 6);  // gDPB

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
