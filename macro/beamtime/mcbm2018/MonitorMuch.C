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

void MonitorMuch(TString inFile = "", TString sHostname = "en02",
                 Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080,
                 Int_t iStartFile = -1, Int_t iStopFile = -1 )
{

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/much_out.root";
  TString parFile = "data/much_param.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";

  TString paramFileHodo = paramDir + "mMuchPar.par";
  TObjString* tutDetDigiFileHodo = new TObjString(paramFileHodo);
  parFileList->Add(tutDetDigiFileHodo);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: Initialising..." << std::endl;

  // MUCH Gem Monitor
  CbmMcbm2018MonitorMuchLite* monitorMuch = new CbmMcbm2018MonitorMuchLite();
  //CbmMcbm2018MonitorMuchLite* monitorMuch = new CbmMcbm2018MonitorMuchLite();
  monitorMuch->SetHistoFileName( "data/MuchHistos.root" );
//  monitorSts->SetPrintMessage();
  monitorMuch->SetMsOverlap( 1 );
//  monitorSts->SetLongDurationLimits( 3600, 10 );
 //  monitorSts->SetLongDurationLimits( 7200, 60 );
//  monitorSts->SetEnableCoincidenceMaps();
 // monitorSts->SetCoincidenceBorder(   0.0,  200 );
//  monitorSts->SetMuchMode();

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  if( "" != inFile )
  {
      if( 0 <= iStartFile && iStartFile < iStopFile )
      {
         for( Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx )
         {
            TString sFilePath = Form( "%s_%04u.tsa", inFile.Data(), iFileIdx );
            source->AddFile( sFilePath  );
            std::cout << "Added " << sFilePath <<std::endl;
         } // for( Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx )
      } // if( 0 < iStartFile && 0 < iStopFile )
         else source->SetFileName(inFile);
  } // if( "" != inFile )
      else
      {
         source->SetHostName( sHostname);
         source->SetPortNumber( 5556 );
      }

  //source->AddUnpacker(monitorMuch,  0x10, 6); // stsXyter DPBs
  source->AddUnpacker(monitorMuch,  0x10, kMuch); // stsXyter DPBs

  // --- Run
  run = new FairRunOnline(source);
  run->ActivateHttpServer( iServerRefreshRate, iServerHttpPort ); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorSts: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorSts: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorSts: Output file is " << outFile << std::endl;
  std::cout << ">>> MonitorSts: Real time " << rtime << " s, CPU time "
	         << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
