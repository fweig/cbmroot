TString caveGeom="";
TString pipeGeom="";
TString magnetGeom="";
TString stsGeom="";
TString richGeom="";
TString shieldGeom="";
TString platformGeom="";

TString stsTag="";
TString richTag="";

TString stsDigi="";
TString richDigi="";
TString trdDigi="";
TString tofDigi="";
TString tofDigiBdf="";

TString mvdMatBudget="";
TString stsMatBudget="";

TString  fieldMap="";
Double_t fieldZ=0.;
Double_t fieldScale=0.;
Int_t    fieldSymType=0;

TString defaultInputFile="";


void run_reco(Int_t nEvents = 50000, Int_t studyName = 0, Int_t corrFlag = 0)
{
    TTree::SetMaxTreeSize(90000000000);

    Int_t iVerbose = 0;     // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
    FairLogger* logger = FairLogger::GetLogger();
    logger->SetLogScreenLevel("INFO");
    logger->SetLogVerbosityLevel("LOW");
    gRandom->SetSeed(10);

    // -----   Environment   --------------------------------------------------
    TString srcDir = gSystem->Getenv("VMCWORKDIR");	// top source directory
    TString myName = "run_reco";			// macro's name for screen output
    // ------------------------------------------------------------------------

    TString script = TString(gSystem->Getenv("SCRIPT"));
    TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters/");

    // -----   In- and output file names   ------------------------------------
    TString setupName = "", outDir = "", outDir2 = "";
    if ( studyName == 0 ) {
	outDir = "/data/Sim_Outputs/Align/";
	setupName = "setup_align";
	if ( corrFlag == 0 ) {
		outDir2 = "/data/Sim_Outputs/Align/Uncorrected/";
	}
	if ( corrFlag == 1 ) {
		outDir2 = "/data/Sim_Outputs/Align/Corrected/";
	}
    }
    else if ( studyName == 1 ) {
	outDir = "/data/Sim_Outputs/Gauss_sigma_1/";
	setupName = "setup_misalign_gauss_sigma_1"; // sigma_1
	if ( corrFlag == 0 ) {
		outDir2 = "/data/Sim_Outputs/Gauss_sigma_1/Uncorrected/";
	}
	if ( corrFlag == 1 ) {
		outDir2 = "/data/Sim_Outputs/Gauss_sigma_1/Corrected/";
	}
    }
    else if ( studyName == 2 ) {
	outDir = "/data/Sim_Outputs/Gauss_sigma_3/";
	setupName = "setup_misalign_gauss_sigma_3"; // sigma_3
	if ( corrFlag == 0 ) {
		outDir2 = "/data/Sim_Outputs/Gauss_sigma_3/Uncorrected/";
	}
	if ( corrFlag == 1 ) {
		outDir2 = "/data/Sim_Outputs/Gauss_sigma_3/Corrected/";
	}
    }

    if (script == "yes") {
	setupName = TString(gSystem->Getenv("SETUP_NAME"));
	outDir = TString(gSystem->Getenv("OUT_DIR"));
    }

    TString parFile = outDir + setupName + "_param.root";
    TString mcFile = outDir + setupName + "_mc.root";
    TString geoFile = outDir + setupName + "_geofilefull.root";
    TString recoFile = outDir2 + setupName + "_reco.root";
    TString resultDir = outDir2;

    TString geoSetupFile = "";
    geoSetupFile = "/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction/gauss_distrib/geosetup/" + setupName + ".C";
    // ------------------------------------------------------------------------


    // -----   Script initialization   ----------------------------------------
    if (script == "yes") {
	mcFile = TString(gSystem->Getenv("MC_FILE"));
	recoFile = TString(gSystem->Getenv("RECO_FILE"));
	parFile = TString(gSystem->Getenv("PAR_FILE"));
	resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));

	geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/matching/geosetup/" + TString(gSystem->Getenv("GEO_SETUP_FILE"));
	setupName = TString(gSystem->Getenv("SETUP_NAME"));
    }
    // ------------------------------------------------------------------------

    remove(recoFile.Data());


    // -----   Load the geometry setup   --------------------------------------
    const char* setupName2 = setupName;
    TString setupFunct = "";
    setupFunct = setupFunct + setupName2 + "()";
    std::cout << "-I- setupFile: " << geoSetupFile << std::endl
	<< "-I- setupFunct: " << setupFunct << std::endl;
    gROOT->LoadMacro(geoSetupFile);
    gROOT->ProcessLine(setupFunct);
    //gInterpreter->ProcessLine(setupFunct);
    // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList *parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if ( CbmSetup::Instance()->GetGeoTag(kTrd, geoTag) ) {
        TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
        parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file "
                      << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if ( CbmSetup::Instance()->GetGeoTag(kTof, geoTag) ) {
        TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
        parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file "
                      << tofFile->GetString() << std::endl;
        TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
        parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
                      << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------


    // ----    Debug option   -------------------------------------------------
    gDebug = 0;
    // ------------------------------------------------------------------------


    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();
    // ------------------------------------------------------------------------


    // -----   Reconstruction run   -------------------------------------------
    FairRunAna *run= new FairRunAna();
    if (mcFile != "") run->SetInputFile(mcFile);
    if (recoFile != "") run->SetOutputFile(recoFile);
    run->SetGenerateRunInfo(kTRUE);
    run->SetGeomFile(geoFile);
    // ------------------------------------------------------------------------


    // ----- MC Data Manager   ------------------------------------------------
    CbmMCDataManager* mcManager=new CbmMCDataManager;
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

//    CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
    CbmStsDigitize* stsDigi = new CbmStsDigitize();
//    stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
//    stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
    run->AddTask(stsDigi);
    std::cout << "-I- digitize: Added task " << stsDigi->GetName() << std::endl;
    // -------------------------------------------------------------------------


    // =========================================================================
    // ===                      STS local reconstruction                     ===
    // =========================================================================

    // -----   STS Cluster Finder   --------------------------------------------
    CbmStsFindClusters* stsClusterFinder = new CbmStsFindClusters();
    stsClusterFinder->UseEventMode();
    run->AddTask(stsClusterFinder);
    std::cout << "-I- : Added task " << stsClusterFinder->GetName() << std::endl;
    // -------------------------------------------------------------------------

    // -----   STS hit finder   ------------------------------------------------
    FairTask* stsFindHits = new CbmStsFindHitsEvents();
    run->AddTask(stsFindHits);
    std::cout << "-I- : Added task " << stsFindHits->GetName() << std::endl;
    // -------------------------------------------------------------------------

    // ---  STS track finding   ------------------------------------------------
    CbmKF* kalman = new CbmKF();
    run->AddTask(kalman);
    CbmL1* l1 = new CbmL1();
    //mvdMatBudget = "mvd_matbudget_v15a.root";
    stsMatBudget = "sts_matbudget_v17a.root";
    //TString mvdMatBudgetFileName = parDir + "sts/" + mvdMatBudget;
    TString stsMatBudgetFileName = parDir + "sts/" + stsMatBudget;
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
    //l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
    run->AddTask(l1);

    CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
    FairTask* stsFindTracks = new CbmStsFindTracks(iVerbose, stsTrackFinder);
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
    std::cout << "-I- : Added task " << finder->GetName() << std::endl;

    // ===                      End of global tracking                       ===
    // =========================================================================


    // =========================================================================
    // ===                        RICH reconstruction                        ===
    // =========================================================================

    CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
    //richDigitizer->SetNofNoiseHits(0);
    run->AddTask(richDigitizer);
    std::cout << "-I- digitize: Added task " << richDigitizer->GetName() << std::endl;

    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);
    std::cout << "-I- hitProducer: Added task " << richHitProd->GetName() << std::endl;

    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    richReco->SetRunExtrapolation(true);
    richReco->SetRunProjection(true);
//    richReco->SetMirrorMisalignmentCorrectionParameterFile("");
    if ( corrFlag == 0 ) { richReco->SetMirrorMisalignmentCorrectionParameterFile(""); }
    else if ( corrFlag == 1 ) { richReco->SetMirrorMisalignmentCorrectionParameterFile(outDir.Data()); }
    richReco->SetRunTrackAssign(true);
    //	richReco->SetFinderName("ideal");     // To do matching Sts-Ring
    //	richReco->SetProjectionName("analytical2"); // Set to analytical2 to test my class.
    //richReco->SetFitterName("circle_cop");;
    run->AddTask(richReco);
    std::cout << "-I- richReco: Added task " << richReco->GetName() << std::endl;

    CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
    run->AddTask(matchRecoToMc);

    // Reconstruction Qa
    Int_t minNofPointsTrd = 6;
    Double_t trdAnnCut = 0.85;
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
    if ( corrFlag == 0 ) { richRecoQa->SetCorrection("Uncorrected"); }
    else if ( corrFlag == 1 ) { richRecoQa->SetCorrection("Corrected"); }
    run->AddTask(richRecoQa);

    CbmRichGeoTest* geoTest = new CbmRichGeoTest();
    geoTest->SetOutputDir(std::string(resultDir));
//    run->AddTask(geoTest);

    CbmRichMirrorSortingAlignment* mirror = new CbmRichMirrorSortingAlignment();
    mirror->setOutputDir(outDir.Data());
    mirror->setThreshold(10);
//    TString studyName = "Full_Correction";
//    mirror->setStudyName(studyName);
//    run->AddTask(mirror);
//    std::cout << "-I- MirrorSortingAlignment: Added task " << std::endl;

    // ===                 End of RICH local reconstruction                  ===
    // =========================================================================


    // -----  Parameter database   --------------------------------------------
    std::cout << std::endl;
    std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
    parIo1->open(parFile.Data(), "UPDATE");
    rtdb->setFirstInput(parIo1);
    if ( ! parFileList->IsEmpty() ) {
        parIo2->open(parFileList, "in");
        rtdb->setSecondInput(parIo2);
    }
    // ------------------------------------------------------------------------


    // -----   Intialize and run   --------------------------------------------
    std::cout << std::endl;
    std::cout << "-I- " << myName << ": Initialise run" << std::endl;
    run->Init();

    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    rtdb->print();

    // -----   Start run   ----------------------------------------------------
    std::cout << std::endl << std::endl;
    std::cout << "-I- " << myName << ": Starting run" << std::endl;
    run->Run(0,nEvents);
    // ------------------------------------------------------------------------


    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Output file is " << recoFile << std::endl;
    std::cout << "Parameter file is " << parFile << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
    std::cout << std::endl;
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
}
