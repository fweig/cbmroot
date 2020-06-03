void run_reco_jpsi(Int_t nEvents = 50)
{
   TTree::SetMaxTreeSize(90000000000);

	Int_t iVerbose = 0;

	TString script = TString(gSystem->Getenv("SCRIPT"));
	TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
    TString stsMatBudgetFileName = parDir + "/sts/sts_matbudget_v13d.root"; // Material budget file for L1 STS tracking

	gRandom->SetSeed(10);

	//TString mcFile = "/hera/cbm/users/adrian/data/mc.0001.root";
	//TString parFile = "/hera/cbm/users/adrian/data/param.0001.root";
	//TString recoFile ="/hera/cbm/users/adrian/data/reco.0001.root";

	TString parFile = "/hera/cbm/users/slebedev/data/jpsi/jun15_25gev/urqmd_jpsi/params.auau.25gev.centr.00697.root";
	TString recoFile = "/hera/cbm/users/slebedev/data/jpsi/jun15_25gev/urqmd_jpsi/temp.reco.auau.25gev.centr.00697.root";
	TString mcFile = "/hera/cbm/users/slebedev/data/jpsi/jun15_25gev/urqmd_jpsi/mc.auau.25gev.centr.00697.root";

	TString trdHitProducerType = "smearing";
	TObjString stsDigiFile = parDir + "/sts/sts_v13d_std.digi.par"; // STS digi file
    TObjString trdDigiFile = parDir + "/trd/trd_v14a_3e.digi.par"; // TRD digi file
    TObjString tofDigiFile = parDir + "/tof/tof_v13b.digi.par"; // TOF digi file
	if (script == "yes") {
		mcFile = TString(gSystem->Getenv("MC_FILE"));
		recoFile = TString(gSystem->Getenv("RECO_FILE"));
		parFile = TString(gSystem->Getenv("PAR_FILE"));
        trdHitProducerType = TString(gSystem->Getenv("TRD_HIT_PRODUCER_TYPE"));
		stsDigiFile = TString(gSystem->Getenv("STS_DIGI"));
		trdDigiFile = TString(gSystem->Getenv("TRD_DIGI"));
		tofDigiFile = TString(gSystem->Getenv("TOF_DIGI"));
		stsMatBudgetFileName = TString(gSystem->Getenv("STS_MATERIAL_BUDGET_FILE"));
	}

   TString parDir = TString(gSystem->Getenv("VMCWORKDIR")) + TString("/parameters");
   TList *parFileList = new TList();
   if (stsDigiFile.GetString() != "") parFileList->Add(&stsDigiFile);
   if (trdDigiFile.GetString() != "") parFileList->Add(&trdDigiFile);
   if (tofDigiFile.GetString() != "") parFileList->Add(&tofDigiFile);
   gDebug = 0;

    TStopwatch timer;
    timer.Start();

	// ----  Load libraries   -------------------------------------------------
   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();
	gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/determine_setup.C");

   // Remove output reco file
   remove(recoFile.Data());

	// -----   Reconstruction run   -------------------------------------------
	FairRunAna *run= new FairRunAna();
	if (mcFile != "") run->SetInputFile(mcFile);
	if (recoFile != "") run->SetOutputFile(recoFile);

	  // ----- MC Data Manager   ------------------------------------------------
	  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
	  mcManager->AddFile(mcFile);
	  run->AddTask(mcManager);

	// =========================================================================
	// ===                      STS local reconstruction                     ===
	// =========================================================================

	Double_t dynRange       =   40960.;  // Dynamic range [e]
	Double_t threshold      =    4000.;  // Digitisation threshold [e]
	Int_t nAdc              =    4096;   // Number of ADC channels (12 bit)
	Double_t timeResolution =       5.;  // time resolution [ns]
	Double_t deadTime       = 9999999.;  // infinite dead time (integrate entire event)
	Double_t noise          =       0.;  // ENC [e]
	Int_t digiModel         = 1;  // Model: 1 = uniform charge distribution along track

	CbmStsDigitize* stsDigi = new CbmStsDigitize(digiModel);
	stsDigi->SetParameters(dynRange, threshold, nAdc, timeResolution, deadTime, noise);
	run->AddTask(stsDigi);

	FairTask* stsCluster = new CbmStsFindClusters();
	run->AddTask(stsCluster);

	FairTask* stsHit = new CbmStsFindHits();
	run->AddTask(stsHit);

	CbmKF* kalman = new CbmKF();
	run->AddTask(kalman);
	CbmL1* l1 = new CbmL1();
         l1->SetMaterialBudgetFileName(stsMatBudgetFileName);
	run->AddTask(l1);

	CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
	Bool_t useMvd = kFALSE;
	FairTask* stsFindTracks = new CbmStsFindTracks(iVerbose, stsTrackFinder, useMvd);
	run->AddTask(stsFindTracks);

       //   CbmStsTrackFitter* stsTrackFitter = new CbmStsKFTrackFitter();
       //   FairTask* stsFitTracks = new CbmStsFitTracks(stsTrackFitter, iVerbose);
       //   run->AddTask(stsFitTracks);

	// =========================================================================
	// ===                     TRD local reconstruction                      ===
	// =========================================================================
	if (IsTrd(parFile)) {
		CbmTrdRadiator *radiator = new CbmTrdRadiator(kTRUE , "H++");

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
			CbmTrdDigitizerPRF* trdDigiPrf = new CbmTrdDigitizerPRF(radiator);
			run->AddTask(trdDigiPrf);

			CbmTrdClusterFinderFast* trdCluster = new CbmTrdClusterFinderFast();
			run->AddTask(trdCluster);

			CbmTrdHitProducerCluster* trdHit = new CbmTrdHitProducerCluster();
			run->AddTask(trdHit);
		}
	}// isTRD

	// =========================================================================
	// ===                     TOF local reconstruction                      ===
	// =========================================================================
	if (IsTof(parFile)) {
		CbmTofHitProducerNew* tofHitProd = new CbmTofHitProducerNew("CbmTofHitProducerNew",iVerbose);
		tofHitProd->SetInitFromAscii(kFALSE);
		run->AddTask(tofHitProd);
	} //isTof

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


	if (IsTrd(parFile)) {
		CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("CbmTrdSetTracksPidANN","CbmTrdSetTracksPidANN");
		trdSetTracksPidAnnTask->SetTRDGeometryType("h++");
		run->AddTask(trdSetTracksPidAnnTask);
	}//isTrd

    // =========================================================================
    // ===                        RICH reconstruction                        ===
    // =========================================================================
	if (IsRich(parFile)){
		CbmRichHitProducer* richHitProd	= new CbmRichHitProducer();
		richHitProd->SetDetectorType(4);
		richHitProd->SetNofNoiseHits(220);
		richHitProd->SetCollectionEfficiency(1.0);
		richHitProd->SetSigmaMirror(0.06);
		richHitProd->SetCrossTalkHitProb(0.02);
		run->AddTask(richHitProd);

		CbmRichReconstruction* richReco = new CbmRichReconstruction();
		run->AddTask(richReco);

		CbmRichMatchRings* matchRings = new CbmRichMatchRings();
		run->AddTask(matchRings);
	}//isRich


	CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
	run->AddTask(matchRecoToMc);


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

    run->SetGenerateRunInfo(true);
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
