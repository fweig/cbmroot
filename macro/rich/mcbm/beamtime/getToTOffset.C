/** @file MCBM DATA unpacking and ToT Shift calculation
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** Modified by A. Weber
 ** @date 05.08.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Calculate the ToT Shifts in all channels of diriches for Parameter File.
 */
// In order to call later Finish, we make this global
FairRunOnline *run = nullptr;

void getToTOffset(TString inFile = "/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn02_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn04_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn05_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn06_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn08_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn10_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn11_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn12_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn13_*.tsa;/lustre/nyx/cbm/users/ploizeau/mcbm2019/370_pn15_*.tsa", UInt_t uRunId = 370, UInt_t nrEvents=10000, TString outDir="/lustre/nyx/cbm/users/adrian/data/ToTOffset/", TString inDir="") //1Event is 1TS
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents=-1;
  // --- Specify output file name (this is just an example)
  TString runId = TString::Format("%u", uRunId);
  TString outFile = outDir + "/unp_mcbm_" + runId + ".root";
  TString parFile = outDir + "/unp_mcbm_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = srcDir + "/macro/beamtime/mcbm2019/";

  TString paramFileRich = paramDir + "mRichPar.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskRich * unpacker_rich = new CbmMcbm2018UnpackerTaskRich();

  unpacker_rich->SetMonitorMode();
  unpacker_rich->SetIgnoreOverlapMs();

  
  // Deactivate ToT correction with kFALSE. Use it only, if you
  // whant to create a new ToT Correction Set
  unpacker_rich->DoTotCorr(kFALSE);
  
  
  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  source->SetInputDir(inDir);
  source->AddUnpacker(unpacker_rich, 0x30, kRich );//RICH trb

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(uRunId);

  // --- RootFileSink
  // --- Open next outputfile after 4GB
  FairRootFileSink* sink = new FairRootFileSink(outFile);
//  sink->GetOutTree()->SetMaxTreeSize(4294967295LL);

  // --- Run
  run = new FairRunOnline(source);
  run->SetSink(sink);
  run->SetEventHeader(event);
  run->SetAutoFinish(kFALSE);
  
  
  
  // Add ToT Correction Finder
  CbmRichMCbmToTShifter *tot = new CbmRichMCbmToTShifter();
  tot->GeneratePDF();
  tot->ShowTdcId(false);
  run->AddTask(tot);


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
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
  if ( 0 == nrEvents) {
    run->Run(nEvents, 0); // run until end of input file
  } else {
    run->Run(0, nrEvents); // process  2000 Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
