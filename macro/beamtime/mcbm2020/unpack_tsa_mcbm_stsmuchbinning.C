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

void unpack_tsa_mcbm_stsmuchbinning(TString inFile  = "",
                                    UInt_t uRunId   = 0,
                                    UInt_t nrEvents = 0,
                                    TString outDir  = "data",
                                    TString inDir   = "") {
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString outFile = outDir + "/unp_mcbm_" + runId + ".root";
  TString parFile = outDir + "/unp_mcbm_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2020/";

  // --- Load the geometry setup ----
  // This is currently only required by the TRD
  std::string geoSetupTag = "mcbm_beam_2020_03";
  CbmSetup* geoSetup      = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag.data());

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch       = paramDir + "mMuchPar.par";
  TObjString* parMuchFileName = new TObjString(paramFileMuch);
  parFileList->Add(parMuchFileName);

  // ---- Trd ----
  TString geoTagTrd = "";
  if (geoSetup->GetGeoTag(ECbmModuleId::kTrd, geoTagTrd)) {
    TString paramFilesTrd(
      Form("%s/parameters/trd/trd_%s", srcDir.Data(), geoTagTrd.Data()));
    std::vector<std::string> paramFilesVecTrd;
    CbmTrdParManager::GetParFileExtensions(&paramFilesVecTrd);
    for (auto parIt : paramFilesVecTrd) {
      parFileList->Add(
        new TObjString(Form("%s.%s.par", paramFilesTrd.Data(), parIt.data())));
    }
  }

  TString paramFileTof       = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  TString paramFileRich = paramDir + "mRichPar.par";
  if (uRunId > 698) paramFileRich = paramDir + "mRichPar_70.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  TString paramFilePsd       = paramDir + "mPsdPar.par";
  TObjString* parPsdFileName = new TObjString(paramFilePsd);
  parFileList->Add(parPsdFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskSts* unpacker_sts = new CbmMcbm2018UnpackerTaskSts();
  CbmMcbm2018UnpackerTaskMuch* unpacker_much =
    new CbmMcbm2018UnpackerTaskMuch();
  CbmMcbm2018UnpackerTaskTrdR* unpacker_trdR =
    new CbmMcbm2018UnpackerTaskTrdR();
  CbmMcbm2018UnpackerTaskTof* unpacker_tof = new CbmMcbm2018UnpackerTaskTof();
  CbmMcbm2018UnpackerTaskRich* unpacker_rich =
    new CbmMcbm2018UnpackerTaskRich();
  CbmMcbm2018UnpackerTaskPsd* unpacker_psd = new CbmMcbm2018UnpackerTaskPsd();

  unpacker_sts->SetMonitorMode();
  unpacker_much->SetMonitorMode();
  unpacker_trdR->SetMonitorMode();
  unpacker_tof->SetMonitorMode();
  unpacker_rich->SetMonitorMode();
  unpacker_psd->SetMonitorMode();

  unpacker_sts->SetIgnoreOverlapMs();
  unpacker_much->SetIgnoreOverlapMs();
  //  unpacker_trdR ->SetIgnoreOverlapMs(); /// Default is kTRUE
  unpacker_tof->SetIgnoreOverlapMs();
  unpacker_rich->SetIgnoreOverlapMs();
  unpacker_psd->SetIgnoreOverlapMs();

  unpacker_sts->SetBinningFwFlag(kTRUE);
  unpacker_much->SetBinningFwFlag(kTRUE);

  unpacker_tof->SetSeparateArrayT0();

  // ------------------------------ //
  // Enable Asic type for MUCH data.
  // fFlag = 0 ==> Asic type 2.0 (20) ---> December 2018 and March 2019 Data
  // fFlag = 1 ==> Asic type 2.1 (21) ---> December 2019 Data
  // This is to correct the channel fliping problem in smx 2.1 chip
  Int_t fFlag = 1;
  unpacker_much->EnableAsicType(fFlag);
  // ------------------------------ //

  switch (uRunId) {
      /*
     case 159:
     {
        /// General System offsets (= offsets between sub-systems)
        unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 159
        unpacker_much->SetTimeOffsetNs( -1750 ); // Run 159
        unpacker_tof ->SetTimeOffsetNs(   -50 ); // Run 159
        unpacker_rich->SetTimeOffsetNs( -1090 ); // Run 159

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

        unpacker_much->SetTimeOffsetNsAsic(  0,       0.0 ); // Run 159, DPB 0 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic(  1,     109.0 ); // Run 159, DPB 0 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic(  2,     142.0 ); // Run 159, DPB 0 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic(  3,      84.0 ); // Run 159, DPB 0 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic(  4,     109.0 ); // Run 159, DPB 0 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic(  5,       0.0 ); // Run 159, DPB 0 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic(  6, 2820915.0 ); // Run 159, DPB 1 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic(  7, 2820905.0 ); // Run 159, DPB 1 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic(  8, 2820785.0 ); // Run 159, DPB 1 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic(  9, 2820915.0 ); // Run 159, DPB 1 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 10,       0.0 ); // Run 159, DPB 1 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 11, 2820805.0 ); // Run 159, DPB 1 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic( 12,    8144.0 ); // Run 159, DPB 2 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic( 13,    8133.0 ); // Run 159, DPB 2 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic( 14,       0.0 ); // Run 159, DPB 2 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic( 15,       0.0 ); // Run 159, DPB 2 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 16,       0.0 ); // Run 159, DPB 2 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 17,       0.0 ); // Run 159, DPB 2 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic( 18,     136.0 ); // Run 159, DPB 3 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic( 19,     119.0 ); // Run 159, DPB 3 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic( 20,     141.0 ); // Run 159, DPB 3 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic( 21,       0.0 ); // Run 159, DPB 3 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 22,       0.0 ); // Run 159, DPB 3 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 23,       0.0 ); // Run 159, DPB 3 ASIC 5

        break;
     } // 159
*/
    case 384: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 384
      //unpacker_much->SetTimeOffsetNs( -1750 ); // Run 384
      //unpacker_tof ->SetTimeOffsetNs(    40 ); // Run 384
      //unpacker_rich->SetTimeOffsetNs(  -273 ); // Run 384

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 2429.0);   // Run 384, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 2417.0);   // Run 384, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 2418.0);   // Run 384, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 384, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 2404.0);   // Run 384, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 2415.0);   // Run 384, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, -772.7);   // Run 384, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, -779.3);   // Run 384, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 384, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, -806.6);   // Run 384, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, -784.2);  // Run 384, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, -786.4);  // Run 384, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, -788.9);  // Run 384, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 384, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 384, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, -785.9);  // Run 384, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, -784.5);  // Run 384, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, -775.6);  // Run 384, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 2404.0);  // Run 384, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 2400.0);  // Run 384, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 2413.0);  // Run 384, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 2407.0);  // Run 384, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 384, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 384, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 2377.0);  // Run 384, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 2375.0);  // Run 384, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 2378.0);  // Run 384, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 2394.0);  // Run 384, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 2401.0);  // Run 384, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 2405.0);  // Run 384, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 5575.0);  // Run 384, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 5599.0);  // Run 384, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, 5597.0);  // Run 384, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, 5583.0);  // Run 384, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 384, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 384, DPB 5 ASIC 5

      break;
    }  // 384

    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  //  source->SetInputDir(inDir);
  source->AddUnpacker(unpacker_sts, 0x10, ECbmModuleId::kSts);    //STS xyter
  source->AddUnpacker(unpacker_much, 0x50, ECbmModuleId::kMuch);  //MUCH xyter
  source->AddUnpacker(
    unpacker_trdR,
    0x40,
    ECbmModuleId::
      kTrd);  // Trd flibId (0x40) as at desy2019. kTrd defined in CbmDefs.h
  source->AddUnpacker(unpacker_tof, 0x60, ECbmModuleId::kTof);  //gDPB A & B & C
  source->AddUnpacker(unpacker_tof, 0x90, ECbmModuleId::kTof);  //gDPB T0 A & B
  source->AddUnpacker(unpacker_rich, 0x30, ECbmModuleId::kRich);  //RICH trb
  source->AddUnpacker(unpacker_psd, 0x80, ECbmModuleId::kPsd);    //PSD

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
