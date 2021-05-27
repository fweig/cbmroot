/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void BuildEventStar2018(TString inFile = "", TString inDir = "", Bool_t bEventBuildingMode = kFALSE,
                        Bool_t bTimeSort = kTRUE, Bool_t bEventBinaryDump = kFALSE, Int_t iServerRefreshRate = 100,
                        Int_t iServerHttpPort = 8081, size_t uAcceptBoundaryPct = 100)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
  /*
  TString inDir  = srcDir + "/input/";
  if( "" != inFile )
   inFile = inDir + inFile;
*/
  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = "data/event_build.root";
  TString parFile = "data/event_build_param.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  //  gLogger->SetLogScreenLevel("DEBUG2"); // Print raw messages
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileTof          = paramDir + "BuildEtof_v18b.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitorLab: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Initialising..." << std::endl;

  // eTOF event builder
  CbmTofStarEventBuilder2018* etofEventBuilder = new CbmTofStarEventBuilder2018();
  etofEventBuilder->SetMsLimitLevel(uAcceptBoundaryPct);
  etofEventBuilder->SetEventBuildingMode(bEventBuildingMode);
  etofEventBuilder->SetTimeSortOutput(bTimeSort);
  etofEventBuilder->SetEventDumpEnable(bEventBinaryDump);
  etofEventBuilder->SetHistoryHistoSize(600.);
  etofEventBuilder->SetHistoryHistoSizeLong(1800.);
  etofEventBuilder->SetPrintoutInterval(30.0);
  etofEventBuilder->SetHistSaveToPrintRatio(10);

  // --- Source task
  CbmTofStar2018Source* source  = new CbmTofStar2018Source();
  CbmFlibTestSource* sourceFile = new CbmFlibTestSource();
  if ("" != inFile) {
    sourceFile->AddPath(inDir, inFile);
    sourceFile->AddUnpacker(etofEventBuilder, 0x60, 6);  //gDPBs
  }
  //      source->SetFileName(inFile);
  else {
    source->SetHostName("localhost");
    source->SetPortNumber(5556);
  }

  source->AddUnpacker(etofEventBuilder, 0x60, 6);  //gDPBs for eTOF

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  //  run = new FairRunOnline(source);
  if ("" != inFile) run = new FairRunOnline(sourceFile);
  else
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
  std::cout << ">>> ngDpbMonitorLab: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  timer.Stop();

  run->Finish();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Macro finished successfully." << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Output file is " << outFile << std::endl;
  std::cout << ">>> ngDpbMonitorLab: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
