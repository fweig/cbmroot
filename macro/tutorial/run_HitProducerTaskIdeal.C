/** @file run_HitProducerTaskIdeal.C
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

// --------------------------------------------------------------------------
// Macro to demonstrate the algorith based task
//
// --------------------------------------------------------------------------


void run_HitProducerTaskIdeal(Int_t nEvents = 10, TString inFile = "data/test.mc.root",
                              TString parFile = "data/params.root", TString outFile = "data/test.eds.root")
{

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList* parFileList = new TList();

  TString workDir  = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters/";

  TObjString* stsDigiFile = new TObjString(paramDir + "/trd/trd_v17n_1m.gas.par");
  parFileList->Add(stsDigiFile);

  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===                     STS ideal reconstruction                      ===
  // =========================================================================

  // -----   TRD hit producer   ----------------------------------------------
  // Original version of the tasked based ideal STS hit producer
  // CbmStsHitProducerIdeal* stsHitProd = new CbmStsHitProducerIdeal();

  // Intermediate version with algorithm moved to a separate function
  //CbmStsHitProducerTaskIdeal* stsHitProd = new CbmStsHitProducerTaskIdeal();

  // New algorithm based version of the ideal STS hit producer
  CbmStsHitProducerIdealWrapper* stsHitProd = new CbmStsHitProducerIdealWrapper();

  run->AddTask(stsHitProd);

  // -------------------------------------------------------------------------
  // ===                 End of STS ideal reconstruction                   ===
  // =========================================================================


  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------


  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  //  delete run;

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
