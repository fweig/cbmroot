using namespace std;

void run_mirror_reco_correction(Int_t nEvents = 2000)
{
   TTree::SetMaxTreeSize(90000000000);

	Int_t iVerbose = 0;

	TString script = TString(gSystem->Getenv("SCRIPT"));
	TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters/");

	gRandom->SetSeed(10);

	TString outDir = "/data/misalignment_correction/Sim_Outputs/Mirror_Sorting/First/";
	TString runTitle = "Mirror_Sorting";
        TString parFile = outDir + "param." + "test" + ".root";
        TString mcFile = outDir + "mc." + "test" + ".root";
        TString recoFile = outDir + "reco." + "test" + ".root";

	TString geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/geosetup_25gev.C";

	std::string resultDir = "recqa_0001/";

	if (script == "yes") {
		mcFile = TString(gSystem->Getenv("MC_FILE"));
		recoFile = TString(gSystem->Getenv("RECO_FILE"));
		parFile = TString(gSystem->Getenv("PAR_FILE"));
		resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));
		geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + "/macro/rich/run/geosetup/" + TString(gSystem->Getenv("GEO_SETUP_FILE"));
	}

	remove(recoFile.Data());

	//setup all geometries from macro
	cout << "geoSetupName:" << geoSetupFile << endl;
	gROOT->LoadMacro(geoSetupFile);
	init_geo_setup();

	// digi parameters
	TList *parFileList = new TList();
	TObjString trdDigiFile = parDir + "/" + trdDigi;
	TObjString tofDigiFile = parDir + "/" + tofDigi;
	if (trdDigiFile.GetString() != "") parFileList->Add(&trdDigiFile);
	parFileList->Add(&tofDigiFile);

	// material budget for STS and MVD
	TString mvdMatBudgetFileName = "";
	TString stsMatBudgetFileName = parDir + "/" + stsMatBudget;

	gDebug = 0;

	TStopwatch timer;
	timer.Start();

	// ----  Load libraries   -------------------------------------------------
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
	loadlibs();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/determine_setup.C");

	// -----   Reconstruction run   -------------------------------------------
	FairRunAna *run= new FairRunAna();
	if (mcFile != "") run->SetInputFile(mcFile);
	if (recoFile != "") run->SetOutputFile(recoFile);

	// ----- MC Data Manager   ------------------------------------------------
	CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
	mcManager->AddFile(mcFile);
	run->AddTask(mcManager);

	Bool_t isMvd = IsMvd(parFile);
	Bool_t isTrd = IsTrd(parFile);
	Bool_t isRich = IsRich(parFile);
	Bool_t isTof = IsTof(parFile);

	Bool_t useMvdInTracking = kFALSE;
	if (isMvd) {
        CbmMvdDigitizer* mvdDigitise = new CbmMvdDigitizer("MVD Digitiser", 0, iVerbose);
        run->AddTask(mvdDigitise);

        CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose);
        run->AddTask(mvdCluster);


        CbmMvdHitfinder* mvdHitfinder = new CbmMvdHitfinder("MVD Hit Finder", 0, iVerbose);
        mvdHitfinder->UseClusterfinder(kTRUE);
        run->AddTask(mvdHitfinder);

        useMvdInTracking = kTRUE;
        mvdMatBudgetFileName = parDir + "/" + mvdMatBudget;
	}

	// =========================================================================
	// ===                      STS local reconstruction                     ===
	// =========================================================================
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

    CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
    stsDigi->SetProcesses(eLossModel, useLorentzShift, useDiffusion, useCrossTalk);
    stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
    run->AddTask(stsDigi);

	FairTask* stsClusterFinder = new CbmStsFindClusters();
	run->AddTask(stsClusterFinder);

	FairTask* stsFindHits = new CbmStsFindHits();
	run->AddTask(stsFindHits);

	CbmKF* kalman = new CbmKF();
	run->AddTask(kalman);
	CbmL1* l1 = new CbmL1();
	l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
	if (mvdMatBudgetFileName != "") l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
	run->AddTask(l1);

	CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
	FairTask* stsFindTracks = new CbmStsFindTracks(1, stsTrackFinder);
	run->AddTask(stsFindTracks);

	// =========================================================================
	// ===                     TRD local reconstruction                      ===
	// =========================================================================
/*	if (isTrd) {
		Bool_t simpleTR = kTRUE; // use fast and simple version for TR production
		CbmTrdRadiator *radiator = new CbmTrdRadiator(simpleTR , "K++");

		if (trdHitProducerType == "smearing") {
			CbmTrdHitProducerSmearing* trdHitProd = new CbmTrdHitProducerSmearing(radiator);
			trdHitProd->SetUseDigiPar(false);
			run->AddTask(trdHitProd);
		} else if (trdHitProducerType == "digi") {
			CbmTrdDigitizer* trdDigitizer = new CbmTrdDigitizer(radiator);
			run->AddTask(trdDigitizer);

			CbmTrdHitProducerDigi* trdHitProd = new CbmTrdHitProducerDigi();
			run->AddTask(trdHitProd);
		} else if (trdHitProducerType == "clustering") {
			Bool_t triangularPads = false;// Bucharest triangular pad-plane layout
			//Double_t triggerThreshold = 0.5e-6;//SIS100
			Double_t triggerThreshold = 1.0e-6;//SIS300
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
	}// isTRD
*/
	// =========================================================================
	// ===                     TOF local reconstruction                      ===
	// =========================================================================
/*	if (isTof) {
		CbmTofHitProducerNew* tofHitProd = new CbmTofHitProducerNew("CbmTofHitProducer", 1);
		tofHitProd->SetInitFromAscii(kFALSE);
		run->AddTask(tofHitProd);
	} //isTof
*/
	// =========================================================================
	// ===                        Global tracking                            ===
	// =========================================================================

	CbmLitFindGlobalTracks* finder = new CbmLitFindGlobalTracks();
	finder->SetTrackingType(std::string("branch"));
	finder->SetMergerType("nearest_hit");
	run->AddTask(finder);

	CbmPrimaryVertexFinder* pvFinder = new CbmPVFinderKF();
	CbmFindPrimaryVertex* findVertex = new CbmFindPrimaryVertex(pvFinder);
	run->AddTask(findVertex);


	if (isTrd) {
		CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("CbmTrdSetTracksPidANN","CbmTrdSetTracksPidANN");
		trdSetTracksPidAnnTask->SetTRDGeometryType("h++");
		run->AddTask(trdSetTracksPidAnnTask);
	}//isTrd

    // =========================================================================
    // ===                        RICH reconstruction                        ===
    // =========================================================================
	if (isRich){
	CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
        //richDigitizer->SetNofNoiseHits(0);
	run->AddTask(richDigitizer);

	CbmRichHitProducer* richHitProd	= new CbmRichHitProducer();
	run->AddTask(richHitProd);

	CbmRichReconstruction* richReco = new CbmRichReconstruction();
	richReco->SetRunExtrapolation(true);
	richReco->SetRunProjection(true);
	richReco->SetRunTrackAssign(true);
//	richReco->SetFinderName("ideal");     // To do matching Sts-Ring
//	richReco->SetProjectionName("analytical2"); // Set to analytical2 to test my class.
	//richReco->SetFitterName("circle_cop");;
	run->AddTask(richReco);
	}//isRich

	CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
	run->AddTask(matchRecoToMc);
/*
	// Reconstruction Qa
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
	trackingQa->SetOutputDir(resultDir);
	trackingQa->SetPRange(20, 0., 10.);
	trackingQa->SetTrdAnnCut(trdAnnCut);
	std::vector<std::string> trackCat, richCat;
	trackCat.push_back("All");
	trackCat.push_back("Electron");
	richCat.push_back("Electron");
	richCat.push_back("ElectronReference");
	trackingQa->SetTrackCategories(trackCat);
	trackingQa->SetRingCategories(richCat);
	run->AddTask(trackingQa);

	CbmLitFitQa* fitQa = new CbmLitFitQa();
	fitQa->SetMvdMinNofHits(0);
	fitQa->SetStsMinNofHits(4);
	fitQa->SetMuchMinNofHits(10);
	fitQa->SetTrdMinNofHits(minNofPointsTrd);
	fitQa->SetOutputDir(resultDir);
	//run->AddTask(fitQa);

	CbmLitClusteringQa* clusteringQa = new CbmLitClusteringQa();
	clusteringQa->SetOutputDir(resultDir);
	//run->AddTask(clusteringQa);

	CbmLitTofQa* tofQa = new CbmLitTofQa();
	tofQa->SetOutputDir(std::string(resultDir));
	//run->AddTask(tofQa);
*/
	CbmRichMirrorSortingCorrection* mirror = new CbmRichMirrorSortingCorrection();
        mirror->setOutputDir(outDir);
	mirror->setStudyName("test");
	run->AddTask(mirror);

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

    run->Init();
    cout << "Starting run" << endl;
    run->Run(0,nEvents);

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished successfully." << endl;
    cout << "Output file is "    << recoFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
    cout << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
