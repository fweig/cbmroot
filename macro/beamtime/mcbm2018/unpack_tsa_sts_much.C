/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Pierre-Alain Loizeau [committer] */

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

void unpack_tsa_sts_much(TString inFile = "", UInt_t uRunId = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString outFile = "data/unp_sts_much.root";
  TString parFile = "data/unp_sts_much_params.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";
  /*
  TString paramFile = paramDir + "FHodoUnpackPar.par";
  TObjString* tutDetDigiFile = new TObjString(paramFile);
  parFileList->Add(tutDetDigiFile);
  */
  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch       = paramDir + "mMuchPar.par";
  TObjString* parMuchFileName = new TObjString(paramFileMuch);
  parFileList->Add(parMuchFileName);

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

  unpacker_sts->SetMonitorMode();
  unpacker_much->SetMonitorMode();

  unpacker_sts->SetIgnoreOverlapMs();
  unpacker_much->SetIgnoreOverlapMs();

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
  source->SetFileName(inFile);
  source->AddUnpacker(unpacker_sts, 0x10, kSts);    //STS xyter
  source->AddUnpacker(unpacker_much, 0x10, kMuch);  //MUCH xyter
  source->EnableDataOutput();

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
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
  std::cout << ">>> unpack_tsa_sts: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  //run->Run(0, nEvents); // process nEvents

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
