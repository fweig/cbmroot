/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void unpack_tsa_cosy2019_sts(TString inFile  = "",
                             UInt_t uRunId   = 0,
                             UInt_t nrEvents = 0,
                             TString outDir  = "data") {
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%04u", uRunId);
  TString outFile = outDir + "/unp_cosy_" + runId + "_sts.root";
  TString parFile = outDir + "/unp_cosy_params_" + runId + "_sts.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/cosy2019/";

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);
  /*
  TString paramFileTof = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);
*/
  /*
  TString paramFileHodo = paramDir + "mHodoPar.par";
  TObjString* parHodoFileName = new TObjString(paramFileHodo);
  parFileList->Add(parHodoFileName);
*/
  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskSts* unpacker_sts = new CbmMcbm2018UnpackerTaskSts();
  //  CbmMcbm2018UnpackerTaskTof  * unpacker_tof  = new CbmMcbm2018UnpackerTaskTof();
  //  CbmMcbm2018UnpackerTaskHodo * unpacker_hodo = new CbmMcbm2018UnpackerTaskHodo();

  unpacker_sts->SetMonitorMode();
  //  unpacker_tof ->SetMonitorMode();
  //  unpacker_hodo->SetMonitorMode();

  unpacker_sts->SetIgnoreOverlapMs();
  //  unpacker_tof ->SetIgnoreOverlapMs();
  //  unpacker_hodo->SetIgnoreOverlapMs();

  //  unpacker_tof ->SetSeparateArrayT0();

  switch (uRunId) {
      /*
     case 159:
     {
        /// General System offsets (= offsets between sub-systems)
        unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 159
        unpacker_tof ->SetTimeOffsetNs(   -50 ); // Run 159

        /// ASIC specific offsets (= offsets inside sub-system)
        unpacker_sts ->SetTimeOffsetNsAsic(  0,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  1,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  2,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  3,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  4,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  5,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  6,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  7,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  8,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic(  9,      18.75 ); // Run 160, Ladder 0, Module 1, N, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 10,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 11,      25.0  ); // Run 160, Ladder 0, Module 1, N, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 12,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 13,      56.25 ); // Run 160, Ladder 0, Module 1, N, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 14,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 15,      37.5  ); // Run 160, Ladder 0, Module 1, N, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 16,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 17,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 18,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 19,      50.0  ); // Run 160, Ladder 0, Module 1, P, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 20,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 21,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 22,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 23,      25.0  ); // Run 160, Ladder 0, Module 1, P, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 24,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 25,      25.0  ); // Run 160, Ladder 0, Module 0, N, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 26,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 27,      31.25 ); // Run 160, Ladder 0, Module 0, N, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 28,       0.0  ); // Run 160, Ladder 0, Module 0, N, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 29,       6.25 ); // Run 160, Ladder 0, Module 0, N, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 30,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 31,      31.25 ); // Run 160, Ladder 0, Module 0, N, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 32,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 33,      31.25 ); // Run 160, Ladder 0, Module 0, P, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 34,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 35,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 36,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 37,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 38,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 39,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 7

        break;
     } // 159
*/
    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  source->AddUnpacker(unpacker_sts, 0x10, kSts);  //STS xyter
  //  source->AddUnpacker(unpacker_tof,  0x90, kTof  );//gDPB T0
  //  source->AddUnpacker(unpacker_hodo, 0x10, kHodo );//HODO xyter

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
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  } else {
    run->Run(0, nrEvents);  // process  N Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices"
            << std::endl;

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
