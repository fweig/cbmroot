/** @file unpack_tsa_mcbm.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer], Alberica Toia **/

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

void unpack_tsa_mcbm(TString inFile = "", UInt_t uRunId = 0, UInt_t nrEvents = 0, TString outDir = "data",
                     TString inDir = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString outFile = outDir + "/unp_mcbm_withoffset_" + runId + ".root";
  TString parFile = outDir + "/unp_mcbm_params_withoffset_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2018/";

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch = paramDir + "mMuchPar.par";
  /// Special parameter files for runs 80-141 (16_23_28March19) and 142-182 (30March19)
  if (142 < uRunId) paramFileMuch = paramDir + "mMuchPar_30March19.par";
  else if (79 < uRunId)
    paramFileMuch = paramDir + "mMuchPar_16_23_28March19.par";

  TObjString* parMuchFileName = new TObjString(paramFileMuch);
  parFileList->Add(parMuchFileName);

  TString paramFileTof       = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  TString paramFileRich       = paramDir + "mRichPar.par";
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

  CbmMcbm2018UnpackerTaskSts* unpacker_sts   = new CbmMcbm2018UnpackerTaskSts();
  CbmMcbm2018UnpackerTaskMuch* unpacker_much = new CbmMcbm2018UnpackerTaskMuch();
  CbmMcbm2018UnpackerTaskTof* unpacker_tof   = new CbmMcbm2018UnpackerTaskTof();
  CbmMcbm2018UnpackerTaskRich* unpacker_rich = new CbmMcbm2018UnpackerTaskRich();

  unpacker_sts->SetMonitorMode();
  unpacker_much->SetMonitorMode();
  unpacker_tof->SetMonitorMode();
  unpacker_rich->SetMonitorMode();

  unpacker_sts->SetIgnoreOverlapMs();
  unpacker_much->SetIgnoreOverlapMs();
  unpacker_tof->SetIgnoreOverlapMs();
  unpacker_rich->SetIgnoreOverlapMs();

  //  unpacker_tof ->SetDiamondDpbIdx( 2 ); /// Only for Dec 2018 data
  unpacker_tof->SetSeparateArrayT0();

  switch (uRunId) {
    case 48:
      unpacker_sts->SetTimeOffsetNs(43900);   // Run 48
      unpacker_much->SetTimeOffsetNs(12000);  // Run 48
      break;
    case 49:
      unpacker_sts->SetTimeOffsetNs(11900);   // Run 49
      unpacker_much->SetTimeOffsetNs(-2300);  // Run 49
      break;
    case 51:
      unpacker_sts->SetTimeOffsetNs(165450);  // Run 51, no peak in same MS, peak at ~162 us in same TS
      unpacker_much->SetTimeOffsetNs(
        850);  // Run 51, no peak in same MS for full run, peak around -850 ns in last spills
      break;
    case 52:
      unpacker_sts->SetTimeOffsetNs(141500);  // Run 52, no peak in same MS, peak at ~104 us in same TS
      unpacker_much->SetTimeOffsetNs(18450);  // Run 52
      break;
    case 53:
      unpacker_sts->SetTimeOffsetNs(101500);  // Run 53
      unpacker_much->SetTimeOffsetNs(2400);   // Run 53
      break;
    case 81:
      unpacker_sts->SetTimeOffsetNs(-1700);   // Run 81
      unpacker_much->SetTimeOffsetNs(-1600);  // Run 81
      break;
    case 159: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_sts->SetTimeOffsetNs(-1750);   // Run 159
      unpacker_much->SetTimeOffsetNs(-1750);  // Run 159
      unpacker_tof->SetTimeOffsetNs(-50);     // Run 159
      unpacker_rich->SetTimeOffsetNs(-1090);  // Run 159

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_sts->SetTimeOffsetNsAsic(0, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(1, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(2, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(3, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(4, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(5, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(6, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(7, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(8, 0.0);     // Run 159, Ladder 0, Module 1, N, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(9, 25.2);    // Run 159, Ladder 0, Module 1, N, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(10, 0.0);    // Run 159, Ladder 0, Module 1, N, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(11, 17.5);   // Run 159, Ladder 0, Module 1, N, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(12, 0.0);    // Run 159, Ladder 0, Module 1, N, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(13, -13.2);  // Run 159, Ladder 0, Module 1, N, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(14, 0.0);    // Run 159, Ladder 0, Module 1, N, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(15, 5.9);    // Run 159, Ladder 0, Module 1, N, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(16, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(17, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(18, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(19, -10.7);  // Run 159, Ladder 0, Module 1, P, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(20, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(21, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(22, 0.0);    // Run 159, Ladder 0, Module 1, P, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(23, 17.4);   // Run 159, Ladder 0, Module 1, P, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(24, -12.2);  // Run 159, Ladder 0, Module 0, N, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(25, 14.);    // Run 159, Ladder 0, Module 0, N, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(26, -12.5);  // Run 159, Ladder 0, Module 0, N, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(27, 5.7);    // Run 159, Ladder 0, Module 0, N, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(28, 0.0);    // Run 159, Ladder 0, Module 0, N, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(29, 31.0);   // Run 159, Ladder 0, Module 0, N, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(30, -12.4);  // Run 159, Ladder 0, Module 0, N, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(31, 7.6);    // Run 159, Ladder 0, Module 0, N, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(32, 0.0);    // Run 159, Ladder 0, Module 0, P, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(33, 9.5);    // Run 159, Ladder 0, Module 0, P, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(34, 0.0);    // Run 159, Ladder 0, Module 0, P, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(35, 18.8);   // Run 159, Ladder 0, Module 0, P, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(36, 17.);    // Run 159, Ladder 0, Module 0, P, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(37, 19.);    // Run 159, Ladder 0, Module 0, P, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(38, 0.0);    // Run 159, Ladder 0, Module 0, P, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(39, 0.0);    // Run 159, Ladder 0, Module 0, P, Asic 7

      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);     // Run 159, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 109.83);  // Run 159, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 151.6);   // Run 159, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 82.4);    // Run 159, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 108.6);   // Run 159, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);     // Run 159, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6,
                                         2820920.34);  // Run 159, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7,
                                         2820909.9);  // Run 159, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8,
                                         2820888.5);  // Run 159, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9,
                                         2820919.5);  // Run 159, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);    // Run 159, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11,
                                         2820913.8);    // Run 159, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 8151.18);  // Run 159, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 8143.07);  // Run 159, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);      // Run 159, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);      // Run 159, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);      // Run 159, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);      // Run 159, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 142.8);    // Run 159, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 122.9);    // Run 159, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 142.75);   // Run 159, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 0.0);      // Run 159, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);      // Run 159, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);      // Run 159, DPB 3 ASIC 5
      break;
    }  // 159
    case 160: {
      /// General System offsets (= offsets between sub-systems)
      unpacker_sts->SetTimeOffsetNs(-1750);   // Run 160
      unpacker_much->SetTimeOffsetNs(-1750);  // Run 160
      unpacker_tof->SetTimeOffsetNs(-50);     // Run 160
      unpacker_rich->SetTimeOffsetNs(-1090);  // Run 160

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_sts->SetTimeOffsetNsAsic(0, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(1, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(2, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(3, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(4, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(5, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(6, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(7, 0.0);     // Unused
      unpacker_sts->SetTimeOffsetNsAsic(8, 0.0);     // Run 160, Ladder 0, Module 1, N, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(9, 25.2);    // Run 160, Ladder 0, Module 1, N, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(10, 0.0);    // Run 160, Ladder 0, Module 1, N, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(11, 17.5);   // Run 160, Ladder 0, Module 1, N, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(12, 0.0);    // Run 160, Ladder 0, Module 1, N, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(13, -13.2);  // Run 160, Ladder 0, Module 1, N, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(14, 0.0);    // Run 160, Ladder 0, Module 1, N, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(15, 5.9);    // Run 160, Ladder 0, Module 1, N, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(16, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(17, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(18, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(19, -10.7);  // Run 160, Ladder 0, Module 1, P, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(20, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(21, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(22, 0.0);    // Run 160, Ladder 0, Module 1, P, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(23, 17.4);   // Run 160, Ladder 0, Module 1, P, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(24, -12.2);  // Run 160, Ladder 0, Module 0, N, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(25, 14.);    // Run 160, Ladder 0, Module 0, N, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(26, -12.5);  // Run 160, Ladder 0, Module 0, N, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(27, 5.7);    // Run 160, Ladder 0, Module 0, N, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(28, 0.0);    // Run 160, Ladder 0, Module 0, N, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(29, 31.0);   // Run 160, Ladder 0, Module 0, N, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(30, -12.4);  // Run 160, Ladder 0, Module 0, N, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(31, 7.6);    // Run 160, Ladder 0, Module 0, N, Asic 7
      unpacker_sts->SetTimeOffsetNsAsic(32, 0.0);    // Run 160, Ladder 0, Module 0, P, Asic 0
      unpacker_sts->SetTimeOffsetNsAsic(33, 9.5);    // Run 160, Ladder 0, Module 0, P, Asic 1
      unpacker_sts->SetTimeOffsetNsAsic(34, 0.0);    // Run 160, Ladder 0, Module 0, P, Asic 2
      unpacker_sts->SetTimeOffsetNsAsic(35, 18.8);   // Run 160, Ladder 0, Module 0, P, Asic 3
      unpacker_sts->SetTimeOffsetNsAsic(36, 17.);    // Run 160, Ladder 0, Module 0, P, Asic 4
      unpacker_sts->SetTimeOffsetNsAsic(37, 19.);    // Run 160, Ladder 0, Module 0, P, Asic 5
      unpacker_sts->SetTimeOffsetNsAsic(38, 0.0);    // Run 160, Ladder 0, Module 0, P, Asic 6
      unpacker_sts->SetTimeOffsetNsAsic(39, 0.0);    // Run 160, Ladder 0, Module 0, P, Asic 7

      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);       // Run 160, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 109.0);     // Run 160, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 142.0);     // Run 160, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 84.0);      // Run 160, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 109.0);     // Run 160, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);       // Run 160, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 832150.0);  // Run 160, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 832150.0);  // Run 160, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 832150.0);  // Run 160, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 832150.0);  // Run 160, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);      // Run 160, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11,
                                         832150.0);   // Run 160, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 148.0);  // Run 160, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 140.0);  // Run 160, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);    // Run 160, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);    // Run 160, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);    // Run 160, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);    // Run 160, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 136.0);  // Run 160, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 119.0);  // Run 160, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 141.0);  // Run 160, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 0.0);    // Run 160, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);    // Run 160, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);    // Run 160, DPB 3 ASIC 5

      break;
    }  // 160
    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  source->SetInputDir(inDir);
  source->AddUnpacker(unpacker_sts, 0x10, kSts);    //STS xyter
  source->AddUnpacker(unpacker_much, 0x10, kMuch);  //MUCH xyter
  source->AddUnpacker(unpacker_tof, 0x60, kTof);    //gDPB A & B & C
  source->AddUnpacker(unpacker_tof, 0x90, kTof);    //gDPB T0 A & B
  source->AddUnpacker(unpacker_rich, 0x30, kRich);  //RICH trb

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
  }
  else {
    run->Run(0, nrEvents);  // process  N Events
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
  std::cout << ">>> unpack_tsa_mcbm: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
