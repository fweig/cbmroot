/** @file Cern2017Monitor.C
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 26.07.2017
 **
 ** ROOT macro to read tsa files which have been produced with StsXyter + DPB + FLIB
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline *run = NULL;

void McbmPulserMonitor(TString inFile = "", TString sHostname = "pn05",
                 Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080,
                 UInt_t uRunId = 0, Bool_t bWithOffset = kFALSE,
                 Bool_t bWithTs = kTRUE )
{
   TString sFileTag = "";
   if( 0 < uRunId )
      sFileTag = Form("_%u_%s", uRunId, sHostname.Data() );
   if( kTRUE == bWithOffset )
      sFileTag += "_Offs";

   if( kTRUE == bWithTs )
     sFileTag += "_Ts";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/pulser_out" + sFileTag + ".root";
  TString parFile = "data/pulser_param" + sFileTag + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
//  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";

  TString paramFileSts = paramDir + "mStsMuchPar.par";
  TObjString* tutDetDigiFileSts = new TObjString(paramFileSts);
  parFileList->Add(tutDetDigiFileSts);

  TString paramFileTof = paramDir + "mTofPar.par";
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
  CbmMcbm2018MonitorMcbmPulser* monitorPulser = new CbmMcbm2018MonitorMcbmPulser();
  monitorPulser->SetHistoFileName( "data/McbmPulserHistos" + sFileTag + ".root" );
  monitorPulser->SetIgnoreMsOverlap();
  monitorPulser->SetDiamondDpbIdx();
  monitorPulser->SetMuchDpbIdx( 0 );

   if( kTRUE == bWithOffset )
   {
      switch( uRunId )
      {
         case 48:
            monitorPulser->SetStsTofOffsetNs(   43900 ); // Run 48
            monitorPulser->SetMuchTofOffsetNs(  12000 ); // Run 48
            break;
         case 49:
            monitorPulser->SetStsTofOffsetNs(   11900 ); // Run 49
            monitorPulser->SetMuchTofOffsetNs(  -2300 ); // Run 49
            break;
         case 51:
            monitorPulser->SetStsTofOffsetNs(  165450 ); // Run 51, no peak in same MS, peak at ~162 us in same TS
            monitorPulser->SetMuchTofOffsetNs(    850 ); // Run 51, no peak in same MS for full run, peak around -850 ns in last spills
            break;
         case 52:
            monitorPulser->SetStsTofOffsetNs(  141500 ); // Run 52, no peak in same MS, peak at ~104 us in same TS
            monitorPulser->SetMuchTofOffsetNs(  18450 ); // Run 52
            break;
         case 53:
            monitorPulser->SetStsTofOffsetNs(  101500 ); // Run 53
            monitorPulser->SetMuchTofOffsetNs(   2400 ); // Run 53
            break;
         default:
            break;
      } // switch( uRunId )
   } // if( kTRUE == bWithOffset )

   monitorPulser->SetTsLevelAna( bWithTs );

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if( "" != inFile )
  {
    source->SetFileName(inFile);
  } // if( "" != inFile )
      else
      {
         source->SetHostName( sHostname );
      } // else of if( "" != inFile )

  source->AddUnpacker(monitorPulser,  0x10, 6); // sDPBs
  source->AddUnpacker(monitorPulser,  0x60, 6); // gDPB
  source->AddUnpacker(monitorPulser,  0x90, 6); // gDPB

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  run->ActivateHttpServer( iServerRefreshRate, iServerHttpPort ); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
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
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> Cern2017Monitor: Macro finished successfully." << std::endl;
  std::cout << ">>> Cern2017Monitor: Output file is " << outFile << std::endl;
  std::cout << ">>> Cern2017Monitor: Real time " << rtime << " s, CPU time "
	         << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
