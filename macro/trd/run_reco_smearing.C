/* Copyright (C) 2012 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Cyrano Bergmann [committer] */

// --------------------------------------------------------------------------
//
// Macro for testing the trd digitizer and hit producer
//
// F. Uhlig    02/06/2010
// Version     02/06/2010 (F. Uhlig)
//
// --------------------------------------------------------------------------

/// extra Doxygen comment 10/11/2011 by DE

void run_reco_smearing(Int_t nEvents = 2)
{
  gStyle->SetPalette(1, 0);
  gROOT->SetStyle("Plain");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  // ========================================================================
  // geometry selection for sim + reco  by Cyrano
  // ========================================================================
  ifstream whichTrdGeo;
  whichTrdGeo.open("whichTrdGeo", ios::in);
  TString digipar;
  if (whichTrdGeo) whichTrdGeo >> digipar;
  cout << "selected geometry : >> " << digipar
       << " << (to select a different geometry, edit macro/trd/whichTrdGeo file)" << endl;
  whichTrdGeo.close();
  if (digipar.Length() == 0) digipar = "trd_standard";

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  // Input file (MC events)
  TString inFile = "data/test.mc.root";

  // Parameter file
  TString parFile = "data/params.root";

  // Output file
  TString outFile = "data/test.esd.root";

  //  Digitisation files.
  // The sts digi file is not needed. The code is only here to
  // show how one can load more than one digi file.
  TList* parFileList = new TList();

  TString paramDir = gSystem->Getenv("VMCWORKDIR");
  paramDir += "/parameters";

  TObjString stsDigiFile = paramDir + "/sts/sts_v09a.digi.par";
  parFileList->Add(&stsDigiFile);

  //  TObjString trdDigiFile =  paramDir + "/trd/trd_v10b.digi.par";
  //  parFileList->Add(&trdDigiFile);

  TObjString trdDigiFile = paramDir + "/trd/" + digipar + ".digi.par";  //"./trd.digi.par";
  parFileList->Add(&trdDigiFile);

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
  //  gSystem->Load("libLittrack");
  gSystem->Load("libMinuit2");  // Nedded for rich ellipse fitter
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

  // Update of the values for the radiator F.U. 17.08.07
  Int_t trdNFoils   = 130;     // number of polyetylene foils
  Float_t trdDFoils = 0.0013;  // thickness of 1 foil [cm]
  Float_t trdDGap   = 0.02;    // thickness of gap between foils [cm]
  Bool_t simpleTR   = kTRUE;   // use fast and simple version for TR
                               // production

  CbmTrdRadiator* radiator = new CbmTrdRadiator(simpleTR, trdNFoils, trdDFoils, trdDGap);

  CbmTrdHitProducerSmearing* trdHitProd = new CbmTrdHitProducerSmearing(radiator);
  run->AddTask(trdHitProd);

  /*
  CbmTrdHitProducerQa* trdHitProdQa =
                   new CbmTrdHitProducerQa("TRD Hit Producer Qa","TRD task");

  run->AddTask(trdHitProdQa);
  */
  // -------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  //  rtdb->setOutput(parIo1);
  //  rtdb->saveOutput();
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

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
// Output file
TString outFile = "data/test.esd.root";
