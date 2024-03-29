/* Copyright (C) 2014-2015 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Pierre-Alain Loizeau [committer] */

/** @file readTsa.C
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since August 2014
 ** @date 23.01.2015
 **
 ** Adapted to GET4 v1.23 unpacker in 32b mode by P.-A. Loizeau
 ** ROOT macro to read tsa files which have been generated by a flib board and convert the data
 ** into cbmroot format.
 ** Uses CbmFlibFileSource as source task.
 */


void evtBld_file_get4(Int_t nEvents = -1, TString inFolder = "data/", TString inFile = "get4Test",
                      Int_t iUnpackMode = 2, Bool_t bWithUstc = kFALSE, Bool_t bSmallMs = kTRUE)
{

  //   FairRunOnline* run = new FairRunOnline();
  CbmRunOnline* run = new CbmRunOnline();

  // --- Specify input file name (this is just an example)
  //TString inFile = "spadic_dlm_trigger_2014-11-15_noepoch.tsa";
  //   TString inFile = "nxdata.tsa";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  //  Int_t nEvents = -1;

  // Create pattern for histo output file name
  TString sHistoFile = "./" + inFile + Form("_%1u_%1u_%1u", iUnpackMode, bWithUstc, bSmallMs);

  // Create pattern for calib output file name
  TString sCalibFile = inFile + Form("_%1u_%1u_%1u", iUnpackMode, bWithUstc, bSmallMs);


  // --- Specify output file name (this is just an example)
  TString outFile = "data/" + inFile + Form("_%1u_%1u_%1u", iUnpackMode, bWithUstc, bSmallMs) + ".root";

  // Create input filename
  inFile = inFolder + inFile + ".tsa";

  // --- Set log output levels
  //  FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  //  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG2");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> readTsa:  input file is " << inFile << std::endl;
  std::cout << ">>> readTsa: output file is " << outFile << std::endl;
  std::cout << ">>> readTsa:  histo file is " << sHistoFile << std::endl;
  std::cout << ">>> readTsa:  Calib file is " << sCalibFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> readTsa: Initialising..." << std::endl;

  // NXyter Unpacker
  //  CbmTSUnpackNxyter* nxyter_unpacker = new CbmTSUnpackNxyter();

  // Spadic Unpacker
  //  CbmTSUnpackSpadic* spadic_unpacker = new CbmTSUnpackSpadic();

  // GET4 Unpacker
  CbmTSUnpackGet4v1x* get4_unpacker = new CbmTSUnpackGet4v1x();
  get4_unpacker->SetRocNb(2);           // Min 1
  get4_unpacker->SetGet4Nb(88);         // Min 1 64 + 24
  get4_unpacker->SetMsOverlapTs(0);     // Min 1
  get4_unpacker->SetMode(iUnpackMode);  // 0 = debug, 1 = moni, 2 = unpack
                                        // Disable unconnected chips
  for (UInt_t uChipIndex = 24; uChipIndex < 64; uChipIndex++)
    get4_unpacker->SetActiveGet4(uChipIndex, kFALSE);
  // Disable unread USTC RPC chips
  if (kFALSE == bWithUstc)
    for (UInt_t uChipIndex = 80; uChipIndex < 88; uChipIndex++)
      get4_unpacker->SetActiveGet4(uChipIndex, kFALSE);

  //  get4_unpacker->SetBinSizeEvoHistos(  10.0 );
  get4_unpacker->SetBinSizeEvoHistos(1.0);
  //  get4_unpacker->SetBinSizeEvoHistos(   0.1 );
  get4_unpacker->SetLengthEvoHistos(1200.0);
  if (kTRUE == bSmallMs) get4_unpacker->SetMicroSliceLength(16384 * 8 * (1e-9));  // s, From to 01/03 15:00
  else
    get4_unpacker->SetMicroSliceLength(1e-3);  // s, Up to 01/03 15:00

  get4_unpacker->SetPulserMode(kFALSE);  // kTRUE = ON, kFALSE = OFF (default is ON)
  get4_unpacker->SetPulserFee();         // 1 value (default is 0)
  get4_unpacker->SetPulserChans(0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56,
                                60);  // 1-16 values (default is 0-15)
  get4_unpacker->SetOldReadoutSupp();
  get4_unpacker->SetMaxCoincDist(200000.0);  // ps
  get4_unpacker->SetHistoFilename(sHistoFile + "_unpack.hst.root");
  get4_unpacker->SetCalibOutOn();
  get4_unpacker->SetCalibOutFolder("./data/");
  get4_unpacker->SetCalibFilename(sCalibFile);
  get4_unpacker->SetMaxDtMultiHit(12800.0);  // ps

  // Obtain (create) epoch buffer singleton and configure it
  CbmGet4EpochBuffer* epochBufferSing = CbmGet4EpochBuffer::Instance();
  epochBufferSing->SetBufferMaxSize(250);
  epochBufferSing->SetBufferDropEpochSpread(100);
  epochBufferSing->SetBufferHighWater(200);

  // --- Source task
  CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();
  source->SetFileName(inFile);
  //  source->AddUnpacker(nxyter_unpacker, 0x10);
  //  source->AddUnpacker(spadic_unpacker, 0x40);
  source->AddUnpacker(get4_unpacker, 0x60);

  // --- Event header
  //  FairEventHeader* event = new CbmTbEvent();
  //  event->SetRunId(260);

  // --- Run
  //  FairRunOnline *run = FairRunOnline::Instance();
  run->SetSource(source);
  run->SetOutputFile(outFile);
  run->SetAutoFinish(kFALSE);
  run->SetAsync();

  //  FairRunOnline *run = new FairRunOnline(source);
  //  run->SetOutputFile(outFile);
  //  run->SetEventHeader(event);

  // Obtain (create) event buffer singleton and configure it
  CbmGet4EventBuffer* eventBufferSing = CbmGet4EventBuffer::Instance();
  if (kTRUE == bSmallMs) {
    eventBufferSing->SetBufferMaxSize(1400);
    eventBufferSing->SetBufferDropEventSpread(100);
    eventBufferSing->SetBufferHighWater(1200);
  }  // if( kTRUE == bSmallMs )
  else {
    eventBufferSing->SetBufferMaxSize(10000);
    eventBufferSing->SetBufferDropEventSpread(100);
    eventBufferSing->SetBufferHighWater(9200);
  }

  // Create event builder
  CbmGet4EventBuilder* eventBuilder = new CbmGet4EventBuilder();
  eventBuilder->SetMaxTimePerLoop(0.5);  // [s]
  eventBuilder->SetTriggerMode(kFALSE);  // Multiplicity trigger
  eventBuilder->SetTriggerWinNeg(-16);   // Clock cycles (6.4ns)
  eventBuilder->SetTriggerWinPos(16);    // Clock cycles (6.4ns)
  eventBuilder->SetTriggerDeadT(16);     // Clock cycles (6.4ns)
  eventBuilder->SetTriggerType(0);
  eventBuilder->SetMultiThreshold(6);     // Hits per bin
  eventBuilder->SetMultiHistBinSzClk(8);  // Clock cycles (6.4ns) per bin
  eventBuilder->SetHistoFilename(sHistoFile + "_builder.hst.root");
  run->AddTask(eventBuilder);

  // Create event dumper
  CbmGet4EventDumper* eventDumper = new CbmGet4EventDumper();
  eventDumper->SetMaxTimePerLoop(0.5);  // [s]
  eventDumper->SetGet4Nb(88);           // Min 1
  // Disable unconnected chips
  for (UInt_t uChipIndex = 24; uChipIndex < 64; uChipIndex++)
    eventDumper->SetActiveGet4(uChipIndex, kFALSE);
  // Disable unread USTC RPC chips
  for (UInt_t uChipIndex = 80; uChipIndex < 88; uChipIndex++)
    eventDumper->SetActiveGet4(uChipIndex, kFALSE);
  eventDumper->SetNbEvtBuffEmptyCall(50);
  //  eventDumper->SetDataWriting( kFALSE ); // kFALSE = throw out all mode
  eventDumper->SetDataWriting(kTRUE);  // kTRUE = 1 entry per event if everything works
  run->AddTask(eventDumper);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  //  std::cout << ">>> Start run from the command line by calling Run(<events>)" << std::endl;
  run->Run(nEvents, 0);  // run until end of input file

  get4_unpacker->Finish();  // Should be called first as histo owned by analysis file :-(
  run->Finish();
  timer.Stop();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> readTsa: Macro finished successfully." << std::endl;
  std::cout << ">>> readTsa: Output file is " << outFile << std::endl;
  std::cout << ">>> readTsa: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
