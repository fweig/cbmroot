// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with sts and tof allone
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
// F. Uhlig   19/11/2012
//
// --------------------------------------------------------------------------

void tof_reco_Testbeam (Int_t nEvents = 100, Int_t iSys=0) 
{
  // is v15c

  // ========================================================================
  //          Adjust this part according to your requirements
  Int_t sel=0;
  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;
  TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger* log; 

  TString TofGeo="v15c";
  cout << "run global_digi with Tof Geometry "<<TofGeo<<endl;

  TString numEvt = "";
  if(nEvents<10) numEvt="000";
  if(nEvents>=10 && nEvents<100) numEvt="00";
  if(nEvents>=100 && nEvents<1000) numEvt="0";
  numEvt += nEvents;

  TString numEvti="10000"; //for running
  //TString numEvti=numEvt;  // for debugging reco script 

  switch(iSys){
  case 0:  // Input file (MC events)
    TString inFile =  "data/1p.mc.root";
    // Parameter file
    TString parFile = "data/1p.params.root";
    // Output filex
    TString outFile ="data/1p.reco.root"; 
    break;
  case 25:
    TString inFile =  "data/U25cen_1p.mc.root";
    TString parFile = "data/U25cen_1p.params.root";
    TString outFile ="data/U25cen_1p.reco.root"; 
    break;
  default:
    ;
  }
  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly 
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString paramDir = gSystem->Getenv("VMCWORKDIR");
  paramDir += "/parameters";

  TObjString tofDigiFile = paramDir + "/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
  parFileList->Add(&tofDigiFile);

  TObjString tofDigiBdfFile =  paramDir + "/tof/tof_" + TofGeo + ".digibdf.par";
  parFileList->Add(&tofDigiBdfFile);

  TString TofDigitizerBDFInputFile = paramDir + "/tof/test_bdf_input.root";

  TString geoDir  = gSystem->Getenv("VMCWORKDIR");
  TString geoFile = geoDir + "/geometry/tof/tof_" + TofGeo + ".geo.root";

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
  FairRunAna *run = new FairRunAna();
  run->SetInputFile(inFile);
  //  run->SetGeomFile(geoFile);
  run->SetOutputFile(outFile);
  gLogger->SetLogScreenLevel(logLevel.Data());
  TFile *fHist = run->GetOutputFile(); //nh - addition 
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================

  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================
  // -----   TOF digitizer   -------------------------------------------------
   
  CbmTofDigitizerBDF* tofDigitizerBdf = new CbmTofDigitizerBDF("TOF Digitizer BDF",iVerbose);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigi",            kTRUE);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigiMatchPoints", kTRUE);
  tofDigitizerBdf->SetInputFileName(TofDigitizerBDFInputFile);
  tofDigitizerBdf->SetHistoFileName("DigitizerBDF_Control.root");
  run->AddTask(tofDigitizerBdf);

  // Cluster/Hit builder
  CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer",iVerbose);
  tofSimpClust->SetOutputBranchPersistent("TofHit",          kTRUE);
  tofSimpClust->SetOutputBranchPersistent("TofDigiMatch",    kTRUE);
  tofSimpClust->SetHistoFileName("SimpClusterizer_Control.root");
  tofSimpClust->SetOutTimeFactor(1000.);  /* convert ns -> ps */
  run->AddTask(tofSimpClust);
  // ------   TOF hit producer   ---------------------------------------------
  //    CbmTofHitProducerNew* tofHitProd = new CbmTofHitProducerNew("TOF HitProducer",0);
  //1//tofHitProd->SetParFileName(std::string(TofGeoPar));
  //tofHitProd->SetInitFromAscii(kFALSE);
  //run->AddTask(tofHitProd);
  //
  // ===                   End of TOF local reconstruction                 ===
  // =========================================================================
  // =========================================================================
  // ===                     TOF evaluation                                ===
  // =========================================================================

  // Digitizer/custerizer testing
  /*
  CbmTofTests* tofTests = new CbmTofTests("TOF Tests",iVerbose);
  run->AddTask(tofTests);
  */
  // =========================================================================
  // ===                        Global tracking                            ===
  // =========================================================================
  if(0) { // enable / disable global tracking 
  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  // Tracking method to be used
  // "branch" - branching tracking
  // "nn" - nearest neighbor tracking
  // "weight" - weighting tracking
  finder->SetTrackingType("branch");

  // Hit-to-track merger method to be used
  // "nearest_hit" - assigns nearest hit to the track
  //finder->SetMergerType("nearest_hit");
  finder->SetMergerType("all_hits");

  run->AddTask(finder);

  // -----   Primary vertex finding   ---------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);
  // ------------------------------------------------------------------------
  // Global track fitting 
  // (taken from hadron/produceDST.C
  //
  CbmGlobalTrackFitterKF *globalTrackFitter = new CbmGlobalTrackFitterKF();
  CbmFitGlobalTracks *fitGlobal = new CbmFitGlobalTracks("FitGlobalTracks", 1,
                                                          globalTrackFitter);
  run->AddTask(fitGlobal);
  
  //  CbmProduceDst *produceDst = new CbmProduceDst(); // in hadron
  // run->AddTask(produceDst);

  // ===                      End of global tracking                       ===
  // =========================================================================
  // Analysis
  if(1){
   CbmHadronAnalysis *HadronAna = new CbmHadronAnalysis(); // in hadron
   HadronAna->SetBeamMomentum(10.);  // beam momentum
   HadronAna->SetBSelMax(11.);      // maximum impact parameter to be analyzed  
   HadronAna->SetDY(0.5);           // flow analysis exclusion window  

   run->AddTask(HadronAna);
   }
  } 
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
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
  cout << "Initialize run" << endl;  
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  cout << "Processing done, save control histos to output file " << endl;
  fHist->Write();
  //  fHist->ls();
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
