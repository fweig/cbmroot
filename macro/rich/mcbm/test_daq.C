/** @file test_daq.C
  * @copyright Copyright (C) 2018-2019 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Egor Ovcharenko [committer] **/

void test_daq(Int_t startEvent = 0, Int_t nEvents = 10000, TString inFile = "/home/evovch/TSA_files/delay_2_2_0000.tsa",
              TString outFile = "~/TSA_files/result/delay_2_2_0000_3.root")
{
  //Int_t startEvent = 0;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  //Int_t nEvents = 0;

  // --- Set log output levels
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> test_daq:  input file is " << inFile << std::endl;
  std::cout << ">>> test_daq: output file is " << outFile << std::endl;
  std::cout << std::endl;
  std::cout << ">>> test_daq: Initialising..." << std::endl;

  // ========================================================================

  //CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();
  CbmFlibCern2016Source* source = new CbmFlibCern2016Source();
  source->SetFileName(inFile);

  CbmTSUnpackTestMiniRich* unp = new CbmTSUnpackTestMiniRich();
  source->AddUnpacker(unp, 0x30, 0);

  // --- Run
  FairRunOnline* run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  /*
	CbmTestMiniRichPairBuilder* pairBuilder = new CbmTestMiniRichPairBuilder();
	run->AddTask(pairBuilder);
*/
  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> test_daq: Starting run..." << std::endl;
  run->Run(startEvent, startEvent + nEvents);
  timer.Stop();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> test_daq: Macro finished successfully." << std::endl;
  std::cout << ">>> test_daq: Input file is " << inFile << std::endl;
  std::cout << ">>> test_daq: Output file is " << outFile << std::endl;
  std::cout << ">>> test_daq: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
