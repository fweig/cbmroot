/** @file unpack_tsa_much.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Pierre-Alain Loizeau [committer] **/

/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by Ajit Kumar
 ** @date 03.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
void unpack_tsa_much(TString inFile = "")
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString outFile = "data/unp_much.root";
  TString parFile = "data/unp_much_params.root";

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

  CbmMcbm2018MuchUnpacker* unpacker_much = new CbmMcbm2018MuchUnpacker();

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  source->SetFileName(inFile);
  source->AddUnpacker(unpacker_much, 0x10, 4);  //MUCH xyter
  source->EnableDataOutput();

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
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
  std::cout << ">>> unpack_tsa: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  //run->Run(0, nEvents); // process nEvents

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
