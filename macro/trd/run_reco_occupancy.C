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
// Version     04/03/2015 (V. Friese)
//
// --------------------------------------------------------------------------

TString caveGeom="";
TString pipeGeom="";
TString magnetGeom="";
TString mvdGeom="";
TString stsGeom="";
TString richGeom="";
TString muchGeom="";
TString shieldGeom="";
TString trdGeom="";
TString tofGeom="";
TString ecalGeom="";
TString platformGeom="";
TString psdGeom="";
Double_t psdZpos=0.;
Double_t psdXpos=0.;

TString mvdTag="";
TString stsTag="";
TString trdTag="";
TString tofTag="";

TString stsDigi="";
TString muchDigi="";
TString trdDigi="trd_v15a_3e.digi.par";
TString tofDigi="";
TString tofDigiBdf="";

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";

#include "../include/rootalias.C"


void run_reco_occupancy(Int_t nEvents = 1000, const char* setup = "sis100_electron",
	      const char* rawfile="")
{

  // ========================================================================
  // geometry selection for sim + reco  by Cyrano
  // ========================================================================
  std::ifstream whichTrdGeo;
  whichTrdGeo.open("whichTrdGeo",std::ios::in);
  TString digipar;
  if (whichTrdGeo) whichTrdGeo >> digipar;
  std::cout << "selected geometry : >> " << digipar << " << (to select a different geometry, edit macro/trd/whichTrdGeo file)" << std::endl;
  whichTrdGeo.close();
  if (digipar.Length() == 0) digipar = "trd_standard";

  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;
  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");

  TString outDir  = "/gluster2/cbm/sim/data/";
  //TString outDir  = "/opt/CBM/Daten/";
  TRegexp Nr ("[.][0-9][0-9][0-9][0-9][0-9][.]");
  TString inFile  = outDir + setup + TString(rawfile)(Nr) + "_test.raw.root";
  TString parFile = outDir + setup + TString(rawfile)(Nr) + "_params.root";
  TString outFile = outDir + setup + TString(rawfile)(Nr) + "_test.eds.root";      // Output file
  TString geoFile = outDir + setup + TString(rawfile)(Nr) + "_geofile_full.root";


  // Function needed for CTest runtime dependency
  TString depFile = Remove_CTest_Dependency_File(outDir, "run_reco" , setup);

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();

  TString inDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = inDir + "/parameters/";

  TString setupFile = inDir + "/geometry/setup/setup_" + setup + ".C";
  TString setupFunct = TString("setup_")+setup;
  setupFunct += "()";

  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);

  // --- STS digipar file is there only for L1. It is no longer required
  // --- for STS digitisation and should be eventually removed.
  //TObjString stsDigiFile(paramDir + stsDigi);
  //parFileList->Add(&stsDigiFile);
  //std::cout << "macro/run/run_reco.C using: " << stsDigi << std::endl;

  TObjString trdDigiFile(paramDir + "trd/" + trdDigi);
  parFileList->Add(&trdDigiFile);
  std::cout << "macro/run/run_reco.C using: " << trdDigi << std::endl;

/*
  TObjString tofDigiFile(paramDir + tofDigi);
  parFileList->Add(&tofDigiFile);
  std::cout << "macro/run/run_reco.C using: " << tofDigi << std::endl;

  TObjString tofDigiBdfFile(paramDir + tofDigiBdf);
  parFileList->Add(&tofDigiBdfFile);
  std::cout << "macro/run/run_reco.C using: " << tofDigiBdf << std::endl;
*/

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
  FairSource *inputFile = new FairFileSource(inFile);
  run->SetSource(inputFile);
  //run->SetInputFile(inFile);
  run->SetGeomFile(geoFile);
  run->SetOutputFile(outFile);

  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) {
    FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  }

  // ------------------------------------------------------------------------

  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================
/*


  // -----   MVD Digitiser   -------------------------------------------------
  CbmMvdDigitizer* mvdDigitise = new CbmMvdDigitizer("MVD Digitiser", 0, iVerbose);
  run->AddTask(mvdDigitise);
  // -------------------------------------------------------------------------

  // -----   MVD Clusterfinder   ---------------------------------------------
  CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose);
  run->AddTask(mvdCluster);
  // -------------------------------------------------------------------------

*/
/*

  // -----   STS digitizer   -------------------------------------------------
  // -----   The parameters of the STS digitizer are set such as to match
  // -----   those in the old digitizer. Change them only if you know what you
  // -----   are doing.
  Double_t dynRange       =   40960.;  // Dynamic range [e]
  Double_t threshold      =    4000.;  // Digitisation threshold [e]
  Int_t nAdc              =    4096;   // Number of ADC channels (12 bit)
  Double_t timeResolution =       5.;  // time resolution [ns]
  Double_t deadTime       = 9999999.;  // infinite dead time (integrate entire event)
  Double_t noise          =       0.;  // ENC [e]
  Int_t digiModel         =       1;   // User sensor type DSSD

  // The following settings correspond to a validated implementation.
  // Changing them is on your own risk.
  Int_t  eLossModel       = 1;         // Energy loss model: uniform
  Bool_t useLorentzShift  = kFALSE;    // Deactivate Lorentz shift
  Bool_t useDiffusion     = kFALSE;    // Deactivate diffusion
  Bool_t useCrossTalk     = kFALSE;    // Deactivate cross talk

  CbmStsDigitize* stsDigitize = new CbmStsDigitize(digiModel);
  stsDigitize->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
  stsDigitize->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
  run->AddTask(stsDigitize);
  // -------------------------------------------------------------------------
*/
/*

  // -----   TOF digitizer   -------------------------------------------------
  CbmTofDigitizerBDF* tofDigitizerBdf = new CbmTofDigitizerBDF("TOF Digitizer BDF", 0);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigi",            kFALSE);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
  tofDigitizerBdf->SetInputFileName( paramDir + "tof/test_bdf_input.root"); // Required as input file name not read anymore by param class
//  tofDigitizerBdf->SetHistoFileName( digiOutFile ); // Uncomment to save control histograms
  run->AddTask(tofDigitizerBdf);
  // -------------------------------------------------------------------------
*/


/*

  // =========================================================================
  // ===                     MVD local reconstruction                      ===
  // =========================================================================

  // -----   MVD Hit Finder   ------------------------------------------------
  CbmMvdHitfinder* mvdHitfinder = new CbmMvdHitfinder("MVD Hit Finder", 0, iVerbose);
  mvdHitfinder->UseClusterfinder(kTRUE);
  run->AddTask(mvdHitfinder);
  // -------------------------------------------------------------------------

  // ===                 End of MVD local reconstruction                   ===
  // =========================================================================
*/




/*
  // =========================================================================
  // ===                      STS local reconstruction                     ===
  // =========================================================================


  // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsFindClusters();
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------


  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHits();
  run->AddTask(stsFindHits);
  // -------------------------------------------------------------------------


  // -----  STS hit matching   -----------------------------------------------
//  FairTask* stsMatchHits = new CbmStsMatchHits();
//  run->AddTask(stsMatchHits);
  // -------------------------------------------------------------------------


  // ---  STS track finding   ------------------------------------------------
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1();
  TString mvdMatBudgetFileName = paramDir + mvdMatBudget;
  TString stsMatBudgetFileName = paramDir + stsMatBudget;
  l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  run->AddTask(l1);

  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks = new CbmStsFindTracks(iVerbose, stsTrackFinder);
  run->AddTask(stsFindTracks);
  // -------------------------------------------------------------------------


  // ---   STS track matching   ----------------------------------------------
//  FairTask* stsMatchTracks = new CbmStsMatchTracks(iVerbose);
//  run->AddTask(stsMatchTracks);
  // -------------------------------------------------------------------------


  // ---   STS track fitting   -----------------------------------------------
//  CbmStsTrackFitter* stsTrackFitter = new CbmStsKFTrackFitter();
//  FairTask* stsFitTracks = new CbmStsFitTracks(stsTrackFitter, iVerbose);
//  run->AddTask(stsFitTracks);
  // -------------------------------------------------------------------------

  // ===                 End of STS local reconstruction                   ===
  // =========================================================================
*/




  // =========================================================================
  // ===                     TRD local reconstruction                      ===
  // =========================================================================

  Bool_t  simpleTR  = kTRUE;  // use fast and simple version for TR production
  CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR,"K++");
  //"K++" : micro structured POKALON
  //"H++" : PE foam foils
  //"G30" : ALICE fibers 30 layers

  Bool_t triangularPads = false;// Bucharest triangular pad-plane layout
  Double_t triggerThreshold = 0.5e-6;//SIS100
  //Double_t triggerThreshold = 1.0e-6;//SIS300
  Double_t trdNoiseSigma_keV = 0.1; //default best matching to test beam PRF

  CbmTrdDigitizerPRF* trdDigiPrf = new CbmTrdDigitizerPRF(radiator);
  trdDigiPrf->SetTriangularPads(triangularPads);
  trdDigiPrf->SetNoiseLevel(trdNoiseSigma_keV);
  run->AddTask(trdDigiPrf);

  CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
  trdCluster->SetNeighbourTrigger(true);
  trdCluster->SetTriggerThreshold(triggerThreshold);
  trdCluster->SetNeighbourRowTrigger(false);
  trdCluster->SetPrimaryClusterRowMerger(true);
  trdCluster->SetTriangularPads(triangularPads);
  run->AddTask(trdCluster);

  CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
  trdHit->SetTriangularPads(triangularPads);
  run->AddTask(trdHit);

  CbmTrdOccupancyQa* trdOccupancy = new CbmTrdOccupancyQa("TRD Occupancy", "TRD task", digipar);
  run->AddTask(trdOccupancy);


  // -------------------------------------------------------------------------
  // ===                 End of TRD local reconstruction                   ===
  // =========================================================================

/*

  // =========================================================================
  // ===                     TOF local reconstruction                      ===
  // =========================================================================


  // ------   TOF Cluster/Hit builder   ---------------------------------------
  CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
  tofSimpClust->SetOutputBranchPersistent("TofHit",          kTRUE);
  tofSimpClust->SetOutputBranchPersistent("TofDigiMatch",    kTRUE);
//  tofSimpClust->SetHistoFileName( clustOutFile ); // Uncomment to save control histograms
  run->AddTask(tofSimpClust);
  // -------------------------------------------------------------------------

  // ===                   End of TOF local reconstruction                 ===
  // =========================================================================
*/


  if (muchGeom.Length() != 0)  // if RICH is defined
  {
   TString muchDigiFile(paramDir + muchDigi);
   std::cout << "MuchDigiFile: " << muchDigiFile << std::endl;

   // ----- MUCH hits----------   --------4--------------------------------------
   CbmMuchDigitizeGem* muchDigitize = new CbmMuchDigitizeGem(muchDigiFile.Data());
   run->AddTask(muchDigitize);
   CbmMuchDigitizeStraws* strawDigitize = new CbmMuchDigitizeStraws(muchDigiFile.Data());
   run->AddTask(strawDigitize);

   CbmMuchFindHitsGem* muchFindHits = new CbmMuchFindHitsGem(muchDigiFile.Data());
   run->AddTask(muchFindHits);
   CbmMuchFindHitsStraws* strawFindHits = new CbmMuchFindHitsStraws(muchDigiFile.Data());
   run->AddTask(strawFindHits);
   // --------------------------------------------------------------------------
  }

/*

  // =========================================================================
  // ===                        Global tracking                            ===
  // =========================================================================

  // -----   Primary vertex finding   ---------------------------------------
  CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
  CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
  run->AddTask(findVertex);

  // ------------------------------------------------------------------------
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


  // ===                      End of global tracking                       ===
  // =========================================================================
*/

/*

  // ----------- TRD track Pid Wkn ----------------------
  CbmTrdSetTracksPidWkn* trdSetTracksPidTask = new CbmTrdSetTracksPidWkn(
  		"trdFindTracks", "trdFindTracks");
  run->AddTask(trdSetTracksPidTask);
  // ----------------------------------------------------

  // ----------- TRD track Pid Ann ----------------------
  CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN(
  		"Ann", "Ann");
  run->AddTask(trdSetTracksPidAnnTask);
  // ----------------------------------------------------

  // ----------- TRD track Pid Like ----------------------
  // Since in the newest version of this method depends on the global
  // track the task has to move after the global tracking
  CbmTrdSetTracksPidLike* trdSetTracksPidLikeTask =
  		new CbmTrdSetTracksPidLike("Likelihood", "Likelihood");
  run->AddTask(trdSetTracksPidLikeTask);
  // ----------------------------------------------------

*/
/*
  // =========================================================================
  // ===                        RICH reconstruction                        ===
  // =========================================================================

  if (richGeom.Length() != 0)  // if RICH is defined
    {
        CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
        run->AddTask(richDigitizer);

        CbmRichHitProducer* richHitProd	= new CbmRichHitProducer();
        run->AddTask(richHitProd);

        CbmRichReconstruction* richReco = new CbmRichReconstruction();
        run->AddTask(richReco);
    }*/
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

  // =========================================================================
  // ===                    Matching to Monte-carlo                        ===
  // =========================================================================
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // ===                  End of matching to Monte-Carlo                   ===
  // =========================================================================

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
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
  std::cout << "Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
  // ------------------------------------------------------------------------

  if (hasFairMonitor) {
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory=sysInfo.GetMaxMemory();
    std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    std::cout << maxMemory;
    std::cout << "</DartMeasurement>" << std::endl;

    Float_t cpuUsage=ctime/rtime;
    std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    std::cout << cpuUsage;
    std::cout << "</DartMeasurement>" << std::endl;

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // Function needed for CTest runtime dependency
  Generate_CTest_Dependency_File(depFile);
}
