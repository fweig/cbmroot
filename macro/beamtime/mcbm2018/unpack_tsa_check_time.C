/** @file unpack_tsa_check_time.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer] **/

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

void unpack_tsa_check_time(TString inFile = "", TString sHostname = "", UInt_t uRunId = 0, UInt_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%u", uRunId);
  TString outFile = "data/unp_mcbm_checktime_" + runId + ".root";
  TString parFile = "data/unp_mcbm_params_checktime_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2018/";

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch       = paramDir + "mMuchPar.par";
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
  ////unpacker_rich->SetIgnoreOverlapMs();

  unpacker_sts->SetWriteOutputFlag(kFALSE);
  unpacker_much->SetWriteOutputFlag(kFALSE);
  unpacker_tof->SetWriteOutputFlag(kFALSE);
  unpacker_rich->SetWriteOutputFlag(kFALSE);

  //  unpacker_tof ->SetDiamondDpbIdx( 2 );
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
    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  //  Bool_t _usedaqbuffer = kFALSE;
  //  source->UseDaqBuffer(_usedaqbuffer);

  if ("" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
  }  // else of if( "" != inFile )

  source->AddUnpacker(unpacker_sts, 0x10, kSts);    //STS xyter
  source->AddUnpacker(unpacker_much, 0x10, kMuch);  //MUCH xyter
  source->AddUnpacker(unpacker_tof, 0x60, kTof);    //gDPB A & B
  source->AddUnpacker(unpacker_tof, 0x90, kTof);    //gDPB C & D
  source->AddUnpacker(unpacker_rich, 0x30, kRich);  //RICH trb
  //  source->EnableDataOutput();

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

  CbmCheckTiming* timeChecker = new CbmCheckTiming();
  timeChecker->SetCheckInterSystemOffset(kTRUE);
  timeChecker->SetCheckTimeOrder(kTRUE);
  timeChecker->SetStsOffsetSearchRange(10000);
  timeChecker->SetMuchOffsetSearchRange(10000);
  timeChecker->SetTofOffsetSearchRange(10000);
  timeChecker->SetRichOffsetSearchRange(10000);
  ;
  if (uRunId < 87) timeChecker->SetT0PulserTotLimits(90, 100);  // for runs <= 86
  else                                                          //if( uRunId < )
    timeChecker->SetT0PulserTotLimits(180, 210);                // for runs  >  86
  //    else timeChecker->SetT0PulserTotLimits( 90, 100 ); // for runs <= 86
  if (0 < uRunId) timeChecker->SetOutFilename(Form("HistosTimeCheck_%03u.root", uRunId));
  run->AddTask(timeChecker);

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
  std::cout << ">>> unpack_tsa_sts: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  2000 Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa_sts: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa_sts: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa_sts: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
