/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file MCBM DATA raw message conversion to ROOT for sDPB
 ** @author P.-A. Loizeau
 ** @date 01.07.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert tsa data to root data while keeping raw message format (loses the microslide meta data!).
 ** Uses CbmMcbm2018Source as source task.
 */
// --- Specify number of TS to be converted.
// --- -1 means run until the end of the input file.
void convert_tsa_gdpb(TString inFile = "", Int_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString outFile = "data/raw_gdpb.root";
  TString parFile = "data/raw_gdpb_params.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileSts       = paramDir + "mTofPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> convert_tsa_gdpb: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> convert_tsa_gdpb: Initialising..." << std::endl;

  CbmMcbm2018RawConverterGdpb* raw_conv_gdpb = new CbmMcbm2018RawConverterGdpb();

  raw_conv_gdpb->SetIgnoreOverlapMs();

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  source->SetFileName(inFile);
  source->AddUnpacker(raw_conv_gdpb, 0x60, ECbmModuleId::kTof);  //gDPB A & B & C
  source->AddUnpacker(raw_conv_gdpb, 0x90, ECbmModuleId::kTof);  //gDPB T0 A & B

  // --- Event header
  FairEventHeader* event = new FairEventHeader();
  event->SetRunId(1);

  // --- Run
  FairRunOnline* run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  //  run->SetEventHeader(event);


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
  std::cout << ">>> convert_tsa_gdpb: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  N Events
  }

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> convert_tsa_gdpb: Macro finished successfully." << std::endl;
  std::cout << ">>> convert_tsa_gdpb: Output file is " << outFile << std::endl;
  std::cout << ">>> convert_tsa_gdpb: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
