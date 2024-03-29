/* Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

TString caveGeom     = "";
TString pipeGeom     = "";
TString magnetGeom   = "";
TString stsGeom      = "";
TString richGeom     = "";
TString shieldGeom   = "";
TString platformGeom = "";

TString stsTag  = "";
TString richTag = "";

TString stsDigi    = "";
TString richDigi   = "";
TString trdDigi    = "";
TString tofDigi    = "";
TString tofDigiBdf = "";

TString mvdMatBudget = "";
TString stsMatBudget = "";

TString fieldMap    = "";
Double_t fieldZ     = 0.;
Double_t fieldScale = 0.;
Int_t fieldSymType  = 0;

TString defaultInputFile = "";


void run_reco_alignment(Int_t nEvents = 100000)
{
  TTree::SetMaxTreeSize(90000000000);

  Int_t iVerbose     = 0;  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");
  gRandom->SetSeed(10);

  TString script = TString(gSystem->Getenv("SCRIPT"));
  TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters/");

  // -----   In- and output file names   ------------------------------------
  TString setupName = "", outDir = "";
  //    setupName = "setup_align";
  setupName = "setup_v17a_1e_3mrad_minusX";
  //    setupName = "setup_v17a_1e_3mrad_plusX";
  //    setupName = "setup_v17a_1e_3mrad_minusY";
  //    setupName = "setup_v17a_1e_3mrad_plusY";

  //    outDir = "/data/Sim_Outputs/Correction_test/old_code/";
  outDir = "/data/Sim_Outputs/Correction_test/minusX/with_corr/";
  //    outDir = "/data/Sim_Outputs/w_Brems/minusY/";
  //    outDir = "/data/Sim_Outputs/Event_Display/plusY/";
  //    outDir = "/data/Sim_Outputs/Position/Aligned/";
  //    outDir = "/data/Sim_Outputs/Position/minusX/";
  //    outDir = "/data/Sim_Outputs/Position/plusX/";
  //    outDir = "/data/Sim_Outputs/Position/minusY/";
  //    outDir = "/data/Sim_Outputs/Position/plusY/";

  if (script == "yes") {
    setupName = TString(gSystem->Getenv("SETUP_NAME"));
    outDir    = TString(gSystem->Getenv("OUT_DIR"));
  }

  TString parFile  = outDir + setupName + "_param.root";
  TString mcFile   = outDir + setupName + "_mc.root";
  TString recoFile = outDir + setupName + "_reco.root";
  //TString resultDir = "recqa_0001/";
  TString resultDir = outDir;

  TString geoSetupFile = "";
  geoSetupFile         = "/data/ROOT6/trunk/macro/rich/alignment/"
                 "misalignment_correction/single_tile/geosetup/"
                 + setupName + ".C";
  //    geoSetupFile = "/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction/single_tile/geosetup/setup_align.C";
  // ------------------------------------------------------------------------


  // -----   Script initialization   ----------------------------------------
  if (script == "yes") {
    mcFile    = TString(gSystem->Getenv("MC_FILE"));
    recoFile  = TString(gSystem->Getenv("RECO_FILE"));
    parFile   = TString(gSystem->Getenv("PAR_FILE"));
    resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));

    geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/matching/geosetup/"
                   + TString(gSystem->Getenv("GEO_SETUP_FILE"));
    setupName = TString(gSystem->Getenv("SETUP_NAME"));
  }
  // ------------------------------------------------------------------------


  remove(recoFile.Data());


  // -----   Load the geometry setup   --------------------------------------
  const char* setupName2 = setupName;
  TString setupFunct     = "";
  setupFunct             = setupFunct + setupName2 + "()";
  std::cout << "setupFile: " << geoSetupFile << " and setupFunct: " << setupFunct << std::endl;
  gROOT->LoadMacro(geoSetupFile);
  gInterpreter->ProcessLine(setupFunct);
  // ------------------------------------------------------------------------


  // -----   Digi parameters   ----------------------------------------------
  TList* parFileList = new TList();

  TObjString trdDigiFile(parDir + trdDigi);
  parFileList->Add(&trdDigiFile);
  TObjString tofDigiFile(parDir + tofDigi);
  parFileList->Add(&tofDigiFile);
  TObjString tofDigiBdfFile(parDir + tofDigiBdf);
  parFileList->Add(&tofDigiBdfFile);
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run = new FairRunAna();
  if (mcFile != "") run->SetInputFile(mcFile);
  if (recoFile != "") run->SetOutputFile(recoFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGeomFile("/data/Sim_Outputs/setup_align_geofilefull.root");
  // ------------------------------------------------------------------------


  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(mcFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================

  // -----   STS digitizer   -------------------------------------------------
  // -----   The parameters of the STS digitizer are set such as to match
  // -----   those in the old digitizer. Change them only if you know what you
  // -----   are doing.
  Double_t dynRange       = 40960.;    // Dynamic range [e]
  Double_t threshold      = 4000.;     // Digitisation threshold [e]
  Int_t nAdc              = 4096;      // Number of ADC channels (12 bit)
  Double_t timeResolution = 5.;        // time resolution [ns]
  Double_t deadTime       = 9999999.;  // infinite dead time (integrate entire event)
  Double_t noise          = 0.;        // ENC [e]
  Int_t digiModel         = 1;         // User sensor type DSSD

  // The following settings correspond to a validated implementation.
  // Changing them is on your own risk.
  Int_t eLossModel       = 1;       // Energy loss model: uniform
  Bool_t useLorentzShift = kFALSE;  // Deactivate Lorentz shift
  Bool_t useDiffusion    = kFALSE;  // Deactivate diffusion
  Bool_t useCrossTalk    = kFALSE;  // Deactivate cross talk

  CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
  stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
  stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
  run->AddTask(stsDigi);
  // -------------------------------------------------------------------------


  // =========================================================================
  // ===                      STS local reconstruction                     ===
  // =========================================================================

  // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsFindClustersEvents();
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------

  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHitsEvents();
  run->AddTask(stsFindHits);
  // -------------------------------------------------------------------------

  // ---  STS track finding   ------------------------------------------------
  run->AddTask(new CbmTrackingDetectorInterfaceInit());
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1                    = new CbmL1();
  mvdMatBudget                 = "mvd_matbudget_v15a.root";
  stsMatBudget                 = "sts_matbudget_v17a.root";
  TString mvdMatBudgetFileName = parDir + "sts/" + mvdMatBudget;
  TString stsMatBudgetFileName = parDir + "sts/" + stsMatBudget;
  l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  run->AddTask(l1);

  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks           = new CbmStsFindTracks(iVerbose, stsTrackFinder);
  run->AddTask(stsFindTracks);
  // -------------------------------------------------------------------------

  // ===                 End of STS local reconstruction                   ===
  // =========================================================================


  // =========================================================================
  // ===                        Global tracking                            ===
  // =========================================================================

  // -----   Primary vertex finding   ---------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);
  // ------------------------------------------------------------------------

  CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
  finder->SetTrackingType("branch");
  finder->SetMergerType("nearest_hit");
  run->AddTask(finder);

  // ===                      End of global tracking                       ===
  // =========================================================================


  // =========================================================================
  // ===                        RICH reconstruction                        ===
  // =========================================================================

  CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  //richDigitizer->SetNofNoiseHits(0);
  run->AddTask(richDigitizer);

  CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
  run->AddTask(richHitProd);

  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->SetRunExtrapolation(true);
  richReco->SetRunProjection(true);
  richReco->SetTxtFile(outDir.Data());
  richReco->SetRunTrackAssign(true);
  //	richReco->SetFinderName("ideal");     // To do matching Sts-Ring
  //	richReco->SetProjectionName("analytical2"); // Set to analytical2 to test my class.
  //richReco->SetFitterName("circle_cop");;
  run->AddTask(richReco);

  CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
  run->AddTask(matchRecoToMc);

  // Reconstruction Qa
  Int_t minNofPointsTrd        = 6;
  trdAnnCut                    = 0.85;
  CbmLitTrackingQa* trackingQa = new CbmLitTrackingQa();
  trackingQa->SetMinNofPointsSts(4);
  trackingQa->SetUseConsecutivePointsInSts(true);
  trackingQa->SetMinNofPointsTrd(minNofPointsTrd);
  trackingQa->SetMinNofPointsMuch(10);
  trackingQa->SetMinNofPointsTof(1);
  trackingQa->SetQuota(0.7);
  trackingQa->SetMinNofHitsTrd(minNofPointsTrd);
  trackingQa->SetMinNofHitsMuch(10);
  trackingQa->SetVerbose(0);
  trackingQa->SetMinNofHitsRich(7);
  trackingQa->SetQuotaRich(0.6);
  trackingQa->SetOutputDir(std::string(resultDir));
  trackingQa->SetPRange(20, 0., 10.);
  trackingQa->SetTrdAnnCut(trdAnnCut);
  std::vector<std::string> trackCat, richCat;
  trackCat.push_back("All");
  trackCat.push_back("Electron");
  richCat.push_back("Electron");
  richCat.push_back("ElectronReference");
  trackingQa->SetTrackCategories(trackCat);
  trackingQa->SetRingCategories(richCat);
  //    run->AddTask(trackingQa);

  // RICH reco QA
  CbmRichRecoQa* richRecoQa = new CbmRichRecoQa();
  richRecoQa->SetOutputDir(std::string(resultDir));
  //    run->AddTask(richRecoQa);

  CbmRichGeoTest* geoTest = new CbmRichGeoTest();
  geoTest->SetOutputDir(std::string(resultDir));
  //    run->AddTask(geoTest);

  CbmRichMirrorSortingAlignment* mirror = new CbmRichMirrorSortingAlignment();
  mirror->setOutputDir(outDir + "/corr_params/");
  mirror->setThreshold(700);
  //    TString studyName = "Full_Correction";
  //    mirror->setStudyName(studyName);
  run->AddTask(mirror);

  CbmLitFitQa* fitQa = new CbmLitFitQa();
  fitQa->SetMvdMinNofHits(0);
  fitQa->SetStsMinNofHits(4);
  fitQa->SetMuchMinNofHits(10);
  fitQa->SetTrdMinNofHits(minNofPointsTrd);
  fitQa->SetOutputDir(std::string(resultDir));
  //run->AddTask(fitQa);

  CbmLitClusteringQa* clusteringQa = new CbmLitClusteringQa();
  clusteringQa->SetOutputDir(std::string(resultDir));
  //run->AddTask(clusteringQa);

  CbmLitTofQa* tofQa = new CbmLitTofQa();
  tofQa->SetOutputDir(std::string(resultDir));
  //run->AddTask(tofQa);

  // ===                 End of RICH local reconstruction                  ===
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


  // -----   Intialize and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  //    run->Run(182100,182200);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
  cout << "Output file is " << recoFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------


  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
