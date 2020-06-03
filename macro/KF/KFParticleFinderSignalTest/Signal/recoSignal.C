//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 2.0
// @since   2.08.17
// 
// macro to reconstruct signal events for KFParticleFinder
//_________________________________________________________________________________

void recoSignal(Int_t nEvents = 1000, const char* setupName = "sis100_electron")
{    
  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;
  FairLogger* logger = FairLogger::GetLogger();
  logger->SetLogScreenLevel("INFO");
  logger->SetLogVerbosityLevel("LOW");
  
  TString inFile  = "mc.root";
  TString parFile = "params.root";
  TString outFile = "reco.root";

  //  Digitisation files.
  // Add TObjectString containing the different file names to
  // a TList which is passed as input to the FairParAsciiFileIo.
  // The FairParAsciiFileIo will take care to create on the fly 
  // a concatenated input parameter file which is then used during
  // the reconstruction.
  TList *parFileList = new TList();
  
  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters";

  TString rootalias = workDir + TString("/macro/include/rootalias.C");
  gROOT->LoadMacro(rootalias.Data());

  TString setupFile = workDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = TString("setup_") + setupName;
  setupFunct += "()";

  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  
  TString geoTag;
  if( setup->GetGeoTag(kTrd, geoTag) )
  {
    TObjString* trdDigiFile = new TObjString(paramDir + "/trd/trd_" + geoTag.Data() + ".digi.par");
    parFileList->Add(trdDigiFile);
  }
  
  if( setup->GetGeoTag(kTof, geoTag) )
  {
    TObjString* tofDigiFile = new TObjString(paramDir + "/tof/tof_" + geoTag.Data() + ".digi.par");
    parFileList->Add(tofDigiFile);
    std::cout << ": Using ToF parameter file "
    		      << tofDigiFile->GetString() << std::endl;

    TObjString* tofDigiBdfFile = new TObjString(paramDir + "/tof/tof_" + geoTag.Data() +".digibdf.par");
    parFileList->Add(tofDigiBdfFile);
    std::cout << ": Using Bdf parameter file "
    		      << tofDigiBdfFile->GetString() << std::endl;

  }  
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
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===             Detector Response Simulation (Digitiser)              ===
  // ===                          (where available)                        ===
  // =========================================================================


  if( setup->IsActive(kMvd) )
  {
    // -----   MVD Digitiser   -------------------------------------------------
    CbmMvdDigitizer* mvdDigitise = new CbmMvdDigitizer("MVD Digitiser", 0, iVerbose);
    run->AddTask(mvdDigitise);
    // -------------------------------------------------------------------------

//     // -----   MVD Clusterfinder   ---------------------------------------------
//     CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose);
//     run->AddTask(mvdCluster);
//     // -------------------------------------------------------------------------
  }

  // -----   STS digitizer   -------------------------------------------------
  // -----   The parameters of the STS digitizer are set such as to match
  // -----   those in the old digitizer. Change them only if you know what you
  // -----   are doing.
  if ( setup->IsActive(kSts) ) 
  {
    Double_t dynRange       =   40960.;  // Dynamic range [e]
    Double_t threshold      =    4000.;  // Digitisation threshold [e]
    Int_t nAdc              =    4096;   // Number of ADC channels (12 bit)
    Double_t timeResolution =       5.;  // time resolution [ns]
    Double_t deadTime       = 9999999.;  // infinite dead time (integrate entire event)
    Double_t noise          =       0.;  // ENC [e]
    Int_t digiModel         =       1;   // Model: 1 = uniform charge distribution along track

    Int_t  eLossModel       = 1;         // Energy loss model: uniform
    Bool_t useLorentzShift  = kFALSE;    // Deactivate Lorentz shift
    Bool_t useDiffusion     = kFALSE;    // Deactivate diffusion
    Bool_t useCrossTalk     = kFALSE;    // Deactivate cross talk
  
    CbmStsDigitize* stsDigi = new CbmStsDigitize();
//     stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
//     stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution,
//                        deadTime, noise);
    run->AddTask(stsDigi);
  }


  // =========================================================================
  // ===                     MVD local reconstruction                      ===
  // =========================================================================

  // -----   MVD Hit Finder   ------------------------------------------------
  if( setup->IsActive(kMvd) )
  {
    CbmMvdClusterfinder* mvdClusterFinder = new CbmMvdClusterfinder("MVD Cluster Finder", 0, iVerbose);
    run->AddTask(mvdClusterFinder);
    CbmMvdHitfinder* mvdHitfinder = new CbmMvdHitfinder("MVD Hit Finder", 0, iVerbose);
    mvdHitfinder->UseClusterfinder(kTRUE);
    run->AddTask(mvdHitfinder);
  }
  // -------------------------------------------------------------------------

  // ===                 End of MVD local reconstruction                   ===
  // =========================================================================




  // =========================================================================
  // ===                      STS local reconstruction                     ===
  // =========================================================================


 /* // -----   STS Cluster Finder   --------------------------------------------
  FairTask* stsClusterFinder = new CbmStsFindClusters();
  run->AddTask(stsClusterFinder);
  // -------------------------------------------------------------------------


  // -----   STS hit finder   ------------------------------------------------
  FairTask* stsFindHits = new CbmStsFindHits();
  run->AddTask(stsFindHits);
 */ // -------------------------------------------------------------------------
 
  CbmStsFindClusters* stsCluster = new CbmStsFindClusters();
  stsCluster->UseEventMode();
  run->AddTask(stsCluster);
    
  FairTask* stsHit = new CbmStsFindHitsEvents();
  run->AddTask(stsHit);

  // ---   STS track matching   ----------------------------------------------
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();    
  run->AddTask(matchTask);
  // -------------------------------------------------------------------------
   
  // ---  STS track finding   ------------------------------------------------
  CbmKF* kalman = new CbmKF();
  run->AddTask(kalman);
  CbmL1* l1 = new CbmL1("CbmL1",1, 3);
  TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
  if( setup->IsActive(kMvd) )
  {
    cout<<"****************  MVD IS ACTIVE ***************"<<endl;
    setup->GetGeoTag(kMvd, geoTag);
    const TString mvdMatBudgetFileName = parDir + "/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if( setup->IsActive(kSts) )
  {
    setup->GetGeoTag(kSts, geoTag);
    const TString stsMatBudgetFileName = parDir + "/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);
  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks = new CbmStsFindTracks(iVerbose, stsTrackFinder);
  run->AddTask(stsFindTracks);
  // -------------------------------------------------------------------------

  if( setup->IsActive(kTrd) ) {
    Bool_t  simpleTR  = kTRUE;  // use fast and simple version for TR production
    CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR,"K++");
    //"K++" : micro structured POKALON
    //"H++" : PE foam foils
    //"G30" : ALICE fibers 30 layers

    Bool_t triangularPads = false;// Bucharest triangular pad-plane layout
    Double_t triggerThreshold = 0.5e-6;//SIS100
//     Double_t triggerThreshold = 1.0e-6;//SIS300
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
  }

  if ( setup->IsActive(kTof) ) {  
    // -----   TOF digitizer   -------------------------------------------------
    CbmTofDigitizerBDF* tofDigi = new CbmTofDigitizerBDF("TOF Digitizer BDF",iVerbose);
    tofDigi->SetOutputBranchPersistent("TofDigi",            kFALSE);
    tofDigi->SetOutputBranchPersistent("TofDigiMatchPoints", kFALSE);
    TString paramDir = gSystem->Getenv("VMCWORKDIR");
    tofDigi->SetInputFileName( paramDir + "/parameters/tof/test_bdf_input.root"); // Required as input file name not read anymore by param class
//      tofDigi->SetHistoFileName( digiOutFile ); // Uncomment to save control histograms
    run->AddTask(tofDigi);
    // -------------------------------------------------------------------------
  
    // ------ TOF hits --------------------------------------------------------
    CbmTofSimpClusterizer* tofCluster
          = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit",          kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch",    kTRUE);
    run->AddTask(tofCluster);
    // ------------------------------------------------------------------------
  }

  if ( setup->IsActive(kRich) || setup->IsActive(kTrd) || setup->IsActive(kTof)) {
    // ------ Global track reconstruction -------------------------------------
    CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
    //CbmLitFindGlobalTracksParallel* finder = new CbmLitFindGlobalTracksParallel();
    // Tracking method to be used
    // "branch" - branching tracking
    // "nn" - nearest neighbor tracking
    // "nn_parallel" - nearest neighbor parallel tracking
    TString globalTrackingType = "nn";
    finder->SetTrackingType(std::string(globalTrackingType));

    // Hit-to-track merger method to be used
    // "nearest_hit" - assigns nearest hit to track
    // "all_hits" - assigns all hits in the searching area to track
    finder->SetMergerType("nearest_hit");

    run->AddTask(finder);
  }
  
  if ( setup->IsActive(kRich) ) {
    CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
    run->AddTask(richDigitizer);
    
    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);
    
    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    run->AddTask(richReco);
  }
  
  // ---   STS track matching   ----------------------------------------------
  CbmMatchRecoToMC* matchTask2 = new CbmMatchRecoToMC();
  if( setup->IsActive(kMvd) )
    matchTask2->SetIncludeMvdHitsInStsTrack(1);
  run->AddTask(matchTask2);
  // -------------------------------------------------------------------------
  
  if( setup->IsActive(kTrd) )
  {
    // ----------- TRD track Pid ModWkn ----------------------
    CbmTrdSetTracksPidModWkn* trdSetTracksPidModWknTask = new CbmTrdSetTracksPidModWkn("Wkn", "Wkn");
    run->AddTask(trdSetTracksPidModWknTask);
    // ----------------------------------------------------

    // ----------- TRD track Pid Ann ----------------------
    CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("Ann", "Ann");
    run->AddTask(trdSetTracksPidAnnTask);
  }
  
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(),"UPDATE");
  rtdb->setFirstInput(parIo1);
  if ( ! parFileList->IsEmpty() ) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------


  // -----   Intialise and run   --------------------------------------------
  run->Init();
  
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  
  //add ions to the TDatabasePDG
  KFPartEfficiencies eff;
  for(int jParticle=eff.fFirstStableParticleIndex+10; jParticle<=eff.fLastStableParticleIndex; jParticle++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if(!pdgDB->GetParticle(eff.partPDG[jParticle])){
        pdgDB->AddParticle(eff.partTitle[jParticle].data(),eff.partTitle[jParticle].data(), eff.partMass[jParticle], kTRUE,
                           0, eff.partCharge[jParticle]*3,"Ion",eff.partPDG[jParticle]);
    }
  }
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
  
  gROOT->ProcessLine("RemoveGeoManager()");
}
