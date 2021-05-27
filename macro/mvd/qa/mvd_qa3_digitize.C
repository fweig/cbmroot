/** @file mvd_qa3_digitize.C
  * @copyright Copyright (C) 2010-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [orginator], Florian Uhlig **/

// --------------------------------------------------------------------------
//
// Macro for local MVD reconstruction from MC data
//
// Tasks:  CbmMvdHitProducer
//
//
// V. Friese   06/02/2007
//
// --------------------------------------------------------------------------
void mvd_qa3_digitize(const char* setup = "sis100_electron")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  TString inDir = gSystem->Getenv("VMCWORKDIR");

  TString outDir = "data/";
  // Input file (MC events)
  TString inFile = outDir + "mvd.mcQA.root";

  // Parameter file name
  TString parFile = outDir + "params.root";

  // Output file
  TString outFile = outDir + "mvd.rawQA.root";

  // Background file (MC events, for pile-up)
  TString bgFile = inFile;

  // Delta file (Au Ions)
  TString deltaFile = outDir + "mvd.mcDelta.root";

  // Number of events to process
  Int_t nEvents = 5;

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");


  TString setupFile  = inDir + "/geometry/setup/setup_" + setup + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setup + "()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -------   MVD Digitiser   ----------------------------------------------
  CbmMvdDigitizer* digi = new CbmMvdDigitizer("MVDDigitiser", 0, iVerbose);
  std::cout << "Adding Task:  CbmMvdDigitiser... " << std::endl;

  //--- Pile Up -------

  Int_t pileUpInMVD = 2;  // use 1 or higher

  digi->SetBgFileName(bgFile);
  digi->SetBgBufferSize(5);  //for simulation this buffer should contain > 2 * pileUpInMVD mBias events.
  //default = few 100 (increase for high pile-up, reduce to limit memory consumption)
  digi->SetPileUp(pileUpInMVD - 1);


  //--- Delta electrons -------
  digi->SetDeltaName(deltaFile);
  digi->SetDeltaBufferSize(50);  //for simulation, this buffer must contain at least pileUpInMVD*200 delta electrons
  digi->SetDeltaEvents(2);       //valid for QA-purposes only, use next line for simulations
  //digi->SetDeltaEvents(pileUpInMVD*100); // for simulation assumes 1% target

  //digi->ShowDebugHistograms();
  // ------------------------------------------------------------------------


  // -----   Digitization run   ---------------------------------------------
  CbmDigitization run;

  run.AddInput(inFile);
  run.SetOutputFile(outFile, kTRUE);
  run.SetParameterRootFile(parFile);
  run.SetEventMode();

  run.SetDigitizer(ECbmModuleId::kMvd, digi);

  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   ----------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  // ---------------------------------------------------------------------------

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
