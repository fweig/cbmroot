/** @file convert_tsa_sdpb.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/** @file MCBM DATA raw message conversion to ROOT for sDPB
 ** @author P.-A. Loizeau
 ** @date 28.06.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert tsa data to root data while keeping raw message format (loses the microslide meta data!).
 ** Uses CbmMcbm2018Source as source task.
 */
// --- Specify number of TS to be converted.
// --- -1 means run until the end of the input file.
void convert_tsa_sdpb(TString inFile = "", Int_t nrEvents = 0)
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString outFile = "data/raw_sdpb.root";
  TString parFile = "data/raw_sdpb_params.root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> convert_tsa_sdpb: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> convert_tsa_sdpb: Initialising..." << std::endl;

  CbmMcbm2018RawConverterSdpb* raw_conv_sdpb = new CbmMcbm2018RawConverterSdpb();

  raw_conv_sdpb->SetIgnoreOverlapMs();

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  source->SetFileName(inFile);
  source->AddUnpacker(raw_conv_sdpb, 0x10, ECbmModuleId::kSts);  //STS xyter from STS and MUCH

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
  std::cout << ">>> convert_tsa_sdpb: Starting run..." << std::endl;
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
  std::cout << ">>> convert_tsa_sdpb: Macro finished successfully." << std::endl;
  std::cout << ">>> convert_tsa_sdpb: Output file is " << outFile << std::endl;
  std::cout << ">>> convert_tsa_sdpb: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
