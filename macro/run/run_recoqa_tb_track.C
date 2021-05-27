// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated time slices
// Using time-based reconstruction in the STS
//
// STS cluster finder (time-based)
// STS hit finder (time-based)
// STS track finder (time-based)
// Event building from STS tracks
// Simple QA
//
// V. Friese   14/03/2017
//
// --------------------------------------------------------------------------

using std::cout;
using std::endl;

void run_recoqa_tb_track(TString dataSet = "test", Int_t nSlices = -1, TString setup = "sis100_electron")
{

  // =========================================================================
  // ===                      Settings                                     ===
  // =========================================================================


  // --- File names
  TString outDir  = "data/";
  TString inFile  = dataSet + ".raw.root";     // Input file (digis)
  TString parFile = dataSet + ".par.root";     // Parameter file
  TString recFile = dataSet + ".tb.rec.root";  // Output file
  TString outFile = dataSet + ".tb.qa.root";   // Output file
  TString traFile = dataSet + ".tra.root";     // Output file

  // Log level
  TString logLevel     = "INFO";  // switch to DEBUG or DEBUG1,... for more info
  TString logVerbosity = "LOW";   // switch to MEDIUM or HIGH for more info


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;

  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  inputSource->AddFriend(recFile);
  run->SetSource(inputSource);
  FairRootFileSink* sink = new FairRootFileSink(outFile);
  run->SetSink(sink);
  run->SetGenerateRunInfo(kTRUE);


  TString monitorFile {outFile};
  monitorFile.ReplaceAll("qa", "qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);

  // ------------------------------------------------------------------------


  // ---- Set the log level   -----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------

  // --- QA

  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 0);
  mcManager->AddFile(traFile);
  run->AddTask(mcManager);

  // ----- Reco to MC Matching
  CbmMatchRecoToMC* match1 = new CbmMatchRecoToMC();
  run->AddTask(match1);

  // -- CbmRecoQa
  CbmRecoQa* recoqa = new CbmRecoQa(
    {{"sts", {5, 50, 500, 20}}, {"mvd", {5, 100, 1000, 40}}, {"much", {10, 50, 50, 50}}}, std::string(dataSet.Data()));
  run->AddTask(recoqa);

  // -----   Initialise and run   --------------------------------------------
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  cout << "Starting run " << gGeoManager << endl;
  if (nSlices < 0) run->Run();
  else
    run->Run(nSlices);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
  //  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------


  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  RemoveGeoManager();
}
