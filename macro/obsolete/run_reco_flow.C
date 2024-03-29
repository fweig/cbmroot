/* Copyright (C) 2015-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
//
// HitProducers in (MVD, RICH, TRD, TOF, ECAL,) PSD (detector in brackets not included by default)
// Digitizer and HitFinder in STS
// FAST MC for ECAL
// STS track finding and fitting (L1 / KF)
// TRD track finding and fitting (L1 / KF)
// RICH ring finding (ideal) and fitting
// Global track finding (ideal), rich assignment
// Primary vertex finding (ideal)
// Matching of reconstructed and MC tracks in STS, RICH and TRD
//
// V. Friese   24/02/2006
// Version     24/04/2007 (V. Friese)
// Version     22/10/2014 (Selim)
// --------------------------------------------------------------------------


void run_reco_flow(Int_t nEvents = 2, Int_t En = 10, const char* setup = "sis300_electron")
{
  Int_t gen = 0;

  TString numEvt = "";
  numEvt.Form("%04i", nEvents);

  TString sEn = "";
  sEn += En;

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  TString dir     = "./data/";
  TString inFile  = dir + "mc_" + numEvt + "evt.root";
  TString parFile = dir + "params_" + numEvt + "evt.root";
  TString outFile = dir + "reco_" + numEvt + "evt.root";


  TString inDir    = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = inDir + "/parameters/";

  TString setupFile  = inDir + "/geometry/setup/" + setup + "_setup.C";
  TString setupFunct = setup;
  setupFunct += "_setup()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList* parFileList = new TList();

  TObjString stsDigiFile = paramDir + stsDigi;
  parFileList->Add(&stsDigiFile);
  cout << "macro/run/run_reco.C using: " << stsDigi << endl;

  TObjString trdDigiFile = paramDir + trdDigi;
  parFileList->Add(&trdDigiFile);
  cout << "macro/run/run_reco.C using: " << trdDigi << endl;

  TObjString tofDigiFile = paramDir + tofDigi;
  parFileList->Add(&tofDigiFile);
  cout << "macro/run/run_reco.C using: " << tofDigi << endl;

  //TODO: Don't hardcode the file here. This file should be also defined
  //      in the setup files
  TString stsMatBudgetFileName = paramDir + "/sts/sts_matbudget_v13d.root";
  cout << "STS MB : " << stsMatBudgetFileName << endl;

  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ------------------------------------------------------------------------

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================

  // -----   STS digitizer   -------------------------------------------------
  Double_t threshold              = 4;
  Double_t noiseWidth             = 0.01;
  Int_t nofBits                   = 12;
  Double_t electronsPerAdc        = 10;
  Double_t StripDeadTime          = 0.1;
  CbmStsDigitize_old* stsDigitize = new CbmStsDigitize_old();
  stsDigitize->SetRealisticResponse();
  stsDigitize->SetFrontThreshold(threshold);
  stsDigitize->SetBackThreshold(threshold);
  stsDigitize->SetFrontNoiseWidth(noiseWidth);
  stsDigitize->SetBackNoiseWidth(noiseWidth);
  stsDigitize->SetFrontNofBits(nofBits);
  stsDigitize->SetBackNofBits(nofBits);
  stsDigitize->SetFrontNofElPerAdc(electronsPerAdc);
  stsDigitize->SetBackNofElPerAdc(electronsPerAdc);
  stsDigitize->SetStripDeadTime(StripDeadTime);
  run->AddTask(stsDigitize);
  // -------------------------------------------------------------------------

  // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsClusterFinder_old();
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------

  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHits_old();
  run->AddTask(stsFindHits);
  // -------------------------------------------------------------------------

  // ---  STS track finding   ------------------------------------------------
  run->AddTask(new CbmTrackingDetectorInterfaceInit());
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);

  CbmL1* l1 = new CbmL1();
  l1->SetMaterialBudgetFileName(stsMatBudgetFileName);
  run->AddTask(l1);

  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks           = new CbmStsFindTracks(iVerbose, stsTrackFinder);
  run->AddTask(stsFindTracks);
  // -------------------------------------------------------------------------

  // ---   STS track fitting   -----------------------------------------------
  CbmStsTrackFitter* stsTrackFitter = new CbmStsKFTrackFitter();
  FairTask* stsFitTracks            = new CbmStsFitTracks(stsTrackFitter, iVerbose);
  run->AddTask(stsFitTracks);
  // -------------------------------------------------------------------------

  // ===                 End of STS local reconstruction                   ===
  // =========================================================================

  // =========================================================================
  // ===                        Global tracking                            ===
  // =========================================================================

  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  // Tracking method to be used
  // "branch" - branching tracking
  // "nn" - nearest neighbor tracking
  // "weight" - weighting tracking
  finder->SetTrackingType("branch");

  // Hit-to-track merger method to be used
  // "nearest_hit" - assigns nearest hit to the track
  finder->SetMergerType("nearest_hit");

  run->AddTask(finder);

  // -----   Primary vertex finding   ---------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);
  // ------------------------------------------------------------------------

  // ===                      End of global tracking                       ===
  // =========================================================================

  // -----   RECO-to-MC matching   --------------------------------------------
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // ----------------------------------------------------

  // =========================================================================
  // ===                        PSD reconstruction                        ===
  // =========================================================================

  // -----   PSD hit producer  ----------------------------------------------
  CbmPsdIdealDigitizer* psdDigi = new CbmPsdIdealDigitizer();
  run->AddTask(psdDigi);

  CbmPsdHitProducer* psdHit = new CbmPsdHitProducer();
  run->AddTask(psdHit);

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
