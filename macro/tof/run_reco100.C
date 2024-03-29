/* Copyright (C) 2012 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
//
// HitProducers in MVD, RICH, TRD, TOF, ECAL
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
//
// --------------------------------------------------------------------------

void run_reco100(Int_t nEvents = 2)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // Input file (MC events)
  TString inFile = "data/auaumbias.root";

  // Parameter file
  TString parFile = "data/testparams.root";

  // Output file
  TString outFile = "data/auaumbias.eds.root";

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList* parFileList = new TList();

  TString paramDir = gSystem->Getenv("VMCWORKDIR");
  paramDir += "/parameters";

  TObjString stsDigiFile = paramDir + "/sts/sts_v11a.digi.par";
  parFileList->Add(&stsDigiFile);

  TObjString trdDigiFile = paramDir + "/trd/trd_v11c.digi.par";
  //parFileList->Add(&trdDigiFile);


  // In general, the following parts need not be touched
  // ========================================================================


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libCbmBase");
  gSystem->Load("libCbmData");
  gSystem->Load("libField");
  gSystem->Load("libGen");
  gSystem->Load("libPassive");
  gSystem->Load("libEcal");
  gSystem->Load("libKF");
  gSystem->Load("libMvd");
  gSystem->Load("libSts");
  gSystem->Load("libLittrack");
  gSystem->Load("libRich");
  gSystem->Load("libTrd");
  gSystem->Load("libTof");
  gSystem->Load("libGlobal");
  gSystem->Load("libL1");
  gSystem->Load("libMinuit2");  // Needed for rich ellipse fitter
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================


  // -----   MVD Digitiser   -------------------------------------------------
  //CbmMvdDigitizeL* mvdDigi =
  //		new CbmMvdDigitizeL("MVD Digitiser", 0, iVerbose);
  //  run->AddTask(mvdDigi);
  // -------------------------------------------------------------------------

  // -----   STS digitizer   -------------------------------------------------
  Double_t threshold          = 4;
  Double_t noiseWidth         = 0.01;
  Int_t nofBits               = 12;
  Double_t electronsPerAdc    = 10;
  Double_t StripDeadTime      = 0.1;
  CbmStsDigitize* stsDigitize = new CbmStsDigitize("STS Digitiser", iVerbose);
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


  // =========================================================================
  // ===                     MVD local reconstruction                      ===
  // =========================================================================


  // -----   MVD Hit Finder   ------------------------------------------------
  //CbmMvdFindHits* mvdHitFinder = new CbmMvdFindHits("MVD Hit Finder", 0,
  //		iVerbose);
  //run->AddTask(mvdHitFinder);
  // -------------------------------------------------------------------------


  // ===                 End of MVD local reconstruction                   ===
  // =========================================================================

  // =========================================================================
  // ===                      STS local reconstruction                     ===
  // =========================================================================

  // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsClusterFinder("STS Cluster Finder", iVerbose);
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------

  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHits("STS Hit Finder", iVerbose);
  run->AddTask(stsFindHits);
  // -------------------------------------------------------------------------

  // -----  STS hit matching   -----------------------------------------------
  FairTask* stsMatchHits = new CbmStsMatchHits("STS Hit Matcher", iVerbose);
  run->AddTask(stsMatchHits);
  // -------------------------------------------------------------------------

  // ---  STS track finding   ------------------------------------------------
  run->AddTask(new CbmTrackingDetectorInterfaceInit());
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  run->AddTask(l1);
  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks           = new CbmStsFindTracks(iVerbose, stsTrackFinder);
  run->AddTask(stsFindTracks);
  // -------------------------------------------------------------------------

  // ---   STS track matching   ----------------------------------------------
  FairTask* stsMatchTracks = new CbmStsMatchTracks(iVerbose);
  run->AddTask(stsMatchTracks);
  // -------------------------------------------------------------------------

  // ---   STS track fitting   -----------------------------------------------
  CbmStsTrackFitter* stsTrackFitter = new CbmStsKFTrackFitter();
  FairTask* stsFitTracks            = new CbmStsFitTracks(stsTrackFitter, iVerbose);
  run->AddTask(stsFitTracks);
  // -------------------------------------------------------------------------
  // ===                 End of STS local reconstruction                   ===
  // =========================================================================

  // =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

  // Update of the values for the radiator F.U. 17.08.07
  Int_t trdNFoils   = 130;     // number of polyetylene foils
  Float_t trdDFoils = 0.0013;  // thickness of 1 foil [cm]
  Float_t trdDGap   = 0.02;    // thickness of gap between foils [cm]
  Bool_t simpleTR   = kTRUE;   // use fast and simple version for TR
  // production
  /*
  CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR, trdNFoils,
  		trdDFoils, trdDGap);

  CbmTrdHitProducerSmearing* trdHitProd = new CbmTrdHitProducerSmearing(radiator);
  run->AddTask(trdHitProd);
  */
  // -------------------------------------------------------------------------
  // ===                 End of TRD local reconstruction                   ===
  // =========================================================================


  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================


  // ------   TOF hit producer   ---------------------------------------------
  CbmTofHitProducer* tofHitProd = new CbmTofHitProducer("TOF HitProducer", iVerbose);
  run->AddTask(tofHitProd);
  // -------------------------------------------------------------------------

  // ===                   End of TOF local reconstruction                 ===
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


  // -----   TRD track matching   --------------------------------------------
  CbmTrdMatchTracks* trdMatchTracks = new CbmTrdMatchTracks(iVerbose);
  //run->AddTask(trdMatchTracks);
  // ----------------------------------------------------

  // ----------- TRD track Pid Wkn ----------------------
  /*
  CbmTrdSetTracksPidWkn* trdSetTracksPidTask = new CbmTrdSetTracksPidWkn(
  		"trdFindTracks", "trdFindTracks");
  run->AddTask(trdSetTracksPidTask);
  */
  // ----------------------------------------------------

  // ----------- TRD track Pid Ann ----------------------
  CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("Ann", "Ann");
  //run->AddTask(trdSetTracksPidAnnTask);
  // ----------------------------------------------------

  // ----------- TRD track Pid Like ----------------------
  // Since in the newest version of this method depends on the global
  // track the task has to move after the global tracking
  // FU 08.02.12 Switch the task off since the input file needed for the new geometry has to be generated first.
  //  CbmTrdSetTracksPidLike* trdSetTracksPidLikeTask =
  //  		new CbmTrdSetTracksPidLike("Likelihood", "Likelihood");
  //  run->AddTask(trdSetTracksPidLikeTask);
  // ----------------------------------------------------


  // =========================================================================
  // ===                        RICH reconstruction                        ===
  // =========================================================================

  // ---------------------RICH Hit Producer ----------------------------------
  /*
  CbmRichHitProducer* richHitProd  = new CbmRichHitProducer();
  richHitProd->SetDetectorType(4);
  richHitProd->SetNofNoiseHits(220);
  richHitProd->SetCollectionEfficiency(1.0);
  richHitProd->SetSigmaMirror(0.06);
  run->AddTask(richHitProd);
  //--------------------------------------------------------------------------

  //--------------------- RICH Reconstruction ----------------------------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  //run->AddTask(richReco);

  // ------------------- RICH Ring matching  ---------------------------------
  CbmRichMatchRings* matchRings = new CbmRichMatchRings();
  //run->AddTask(matchRings);
  */
  // -------------------------------------------------------------------------
  // ===                 End of RICH local reconstruction                  ===
  // =========================================================================


  /*
  // =========================================================================
  // ===                        ECAL reconstruction                        ===
  // =========================================================================

  // -----   ECAL hit producer  ----------------------------------------------
  CbmEcalHitProducerFastMC* ecalHitProd = new CbmEcalHitProducerFastMC(
  		"ECAL Hitproducer");
  run->AddTask(ecalHitProd);
  // -------------------------------------------------------------------------

  // ===                      End of ECAL reconstruction                   ===
  // =========================================================================
*/

  // -----  Parameter database   --------------------------------------------
  //  TString stsDigi = gSystem->Getenv("VMCWORKDIR");
  // stsDigi += "/parameters/sts/";
  //  stsDigi += stsDigiFile;
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
