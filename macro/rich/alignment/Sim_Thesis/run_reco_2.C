void run_reco_2(Int_t nEvents = 1000, Int_t input = 3, Int_t geom = 0)
{
    TTree::SetMaxTreeSize(90000000000);
    TString script = TString(gSystem->Getenv("SCRIPT"));

    TString myName = "run_reco";
    TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

    TString setupName = "";
    if (geom == 0) {setupName = "setup_align";}
    else if (geom == 1) {setupName = "setup_misalign_gauss_sigma_1";}
    else if (geom == 2) {setupName = "setup_misalign_gauss_sigma_2";}
    else if (geom == 3) {setupName = "setup_misalign_gauss_sigma_3";}
    else if (geom == 4) {setupName = "setup_misalign_gauss_sigma_5";}

    TString outDir = "";
	if (geom == 0) {outDir = "/data/Sim_Outputs/Sim_Thesis/Misalignment_Effect/Only_e_p/Aligned/";}
	else if (geom == 1) {outDir = "/data/Sim_Outputs/Sim_Thesis/Misalignment_Effect/Only_e_p/1mrad/";}
	else if (geom == 2) {outDir = "/data/Sim_Outputs/Sim_Thesis/Misalignment_Effect/Only_e_p/2mrad/";}
	else if (geom == 3) {outDir = "/data/Sim_Outputs/Sim_Thesis/Misalignment_Effect/Only_e_p/3mrad/";}
	else if (geom == 4) {outDir = "/data/Sim_Outputs/Sim_Thesis/Misalignment_Effect/Only_e_p/5mrad/";}
    TString outDir2 = outDir;
    cout << endl << "outDir & outDir2: " << outDir << " ; " << outDir2 << endl << endl;

    TString parFile = outDir + setupName + "_param.root";
    TString mcFile = outDir + setupName + "_mc.root";
    TString geoFile = outDir + setupName + "_geofilefull.root";
    TString recoFile = outDir2 + setupName + "_reco.root";
    TString resultDir = outDir2;

    TString geoSetupFile = "";
    geoSetupFile = "/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction/Sim_Thesis/geosetup/" + setupName + ".C";

    if (script == "yes") {
        mcFile = TString(gSystem->Getenv("MC_FILE"));
        recoFile = TString(gSystem->Getenv("RECO_FILE"));
        parFile = TString(gSystem->Getenv("PAR_FILE"));
        resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));
        geoSetupFile = srcDir + TString(gSystem->Getenv("GEO_SETUP_FILE"));
    }

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


    CbmSetup* setup = CbmSetup::Instance();

    std::cout << std::endl<< "-I- " << myName << ": Defining parameter files " << std::endl;
    TList *parFileList = new TList();
//    TString geoTag;
//
//    // - TRD digitisation parameters
//    if ( CbmSetup::Instance()->GetGeoTag(kTrd, geoTag) ) {
//        TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
//        parFileList->Add(trdFile);
//        std::cout << "-I- " << myName << ": Using parameter file " << trdFile->GetString() << std::endl;
//    }
//    // - TOF digitisation parameters
//    if ( CbmSetup::Instance()->GetGeoTag(kTof, geoTag) ) {
//        TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
//        parFileList->Add(tofFile);
//        std::cout << "-I- " << myName << ": Using parameter file " << tofFile->GetString() << std::endl;
//        TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
//        parFileList->Add(tofBdfFile);
//        std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
//    }

    TStopwatch timer;
    timer.Start();
    gDebug = 0;


    FairRunAna *run = new FairRunAna();
    FairFileSource* inputSource = new FairFileSource(mcFile);
    run->SetSource(inputSource);
    run->SetOutputFile(recoFile);
    run->SetGenerateRunInfo(kTRUE);

    FairLogger::GetLogger()->SetLogScreenLevel("INFO");
    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");


    CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
    mcManager->AddFile(mcFile);
    run->AddTask(mcManager);


    // Digitisers
    std::cout << std::endl;
    TString macroName = gSystem->Getenv("VMCWORKDIR");
    macroName += "/macro/run/modules/digitize.C";
    std::cout << "Loading macro " << macroName << std::endl;
    gROOT->LoadMacro(macroName);
    gROOT->ProcessLine("digitize()");


    // Reconstruction tasks
    std::cout << std::endl;
    macroName = srcDir + "/macro/run/modules/reconstruct_align.C";
    std::cout << "Loading macro " << macroName << std::endl;
    gROOT->LoadMacro(macroName);
    Bool_t recoSuccess = gROOT->ProcessLine("reconstruct_align()");
    if ( ! recoSuccess ) {
        std::cerr << "-E-" << myName << ": error in executing " << macroName << std::endl;
        return;
    }
    std::cout << "-I-" << myName << ": " << macroName << " excuted successfully" << std::endl;


    CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
    run->AddTask(matchRecoToMc);

    // RICH reco QA
    CbmRichRecoQa* richRecoQa = new CbmRichRecoQa();
    richRecoQa->SetOutputDir(std::string(resultDir));
//    run->AddTask(richRecoQa);

    // CbmRichTrbRecoQa* richTrbReco = new CbmRichTrbRecoQa();
    // richTrbReco->SetOutputDir(std::string(resultDir));
    // run->AddTask(richTrbReco);

    CbmRichRecoMisEffect* recoMisEffect = new CbmRichRecoMisEffect();
    run->AddTask(recoMisEffect);

//    // Reconstruction Qa
    CbmLitTrackingQa* trackingQa = new CbmLitTrackingQa();
    trackingQa->SetMinNofPointsSts(4);
    trackingQa->SetUseConsecutivePointsInSts(true);
   // trackingQa->SetMinNofPointsTrd(minNofPointsTrd);
    trackingQa->SetMinNofPointsMuch(10);
    trackingQa->SetMinNofPointsTof(1);
    trackingQa->SetQuota(0.7);
   // trackingQa->SetMinNofHitsTrd(minNofPointsTrd);
    trackingQa->SetMinNofHitsMuch(10);
    trackingQa->SetVerbose(0);
    trackingQa->SetMinNofHitsRich(7);
    trackingQa->SetQuotaRich(0.6);
    trackingQa->SetOutputDir(std::string(resultDir));
    trackingQa->SetPRange(12, 0., 6.);
   // trackingQa->SetTrdAnnCut(trdAnnCut);
    std::vector<std::string> trackCat, richCat;
    trackCat.push_back("All");
    trackCat.push_back("Electron");
    richCat.push_back("Electron");
    richCat.push_back("ElectronReference");
    trackingQa->SetTrackCategories(trackCat);
    trackingQa->SetRingCategories(richCat);
   //  run->AddTask(trackingQa);

   CbmLitFitQa* fitQa = new CbmLitFitQa();
   fitQa->SetMvdMinNofHits(0);
   fitQa->SetStsMinNofHits(6);
   fitQa->SetMuchMinNofHits(10);
  // fitQa->SetTrdMinNofHits(minNofPointsTrd);
   fitQa->SetOutputDir(std::string(resultDir));
   //run->AddTask(fitQa);

   CbmLitClusteringQa* clusteringQa = new CbmLitClusteringQa();
   clusteringQa->SetOutputDir(std::string(resultDir));
   //run->AddTask(clusteringQa);

    //CbmLitTofQa* tofQa = new CbmLitTofQa();
    //tofQa->SetOutputDir(std::string(resultDir));
    //run->AddTask(tofQa);


    std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
    parIo1->open(parFile.Data(),"UPDATE");
    rtdb->setFirstInput(parIo1);
    if ( ! parFileList->IsEmpty() ) {
        parIo2->open(parFileList, "in");
        rtdb->setSecondInput(parIo2);
    }

    std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
    run->Init();

    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    rtdb->print();

    std::cout << "-I- " << myName << ": Starting run" << std::endl;
    run->Run(0,nEvents);



    // Finish
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << recoFile << std::endl;
    std::cout << "Parameter file is " << parFile << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
    std::cout << std::endl;
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
}
