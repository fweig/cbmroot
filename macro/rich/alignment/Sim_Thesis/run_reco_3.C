void run_reco_3(Int_t nEvents = 2500, Int_t geom = 3)
{
    TTree::SetMaxTreeSize(90000000000);
    TString script = TString(gSystem->Getenv("SCRIPT"));

    TString myName = "run_reco";
    TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

    TString setupName = "", outDir = "";
	if (geom == 0) {
        outDir = "/home/jordan/Desktop/Thesis_Results/HERA-B_Ana/4mrad_X_2mrad_Y_5_3/standard/";
        setupName = "setup_4mrad_X_2mrad_Y_5_3";
    }
    else if (geom == 1) {
    	string s1 = "Y";
        string s2 = "0pt1";
        outDir = "/home/jordan/Desktop/Thesis_Results/HERA-B_Ana/"+s1+"_5_3/"+s2+"mrad_"+s1+"_5_3/";
        setupName = "setup_"+s2+"mrad_"+s1+"_5_3";
    }
    else if (geom == 2) {
        outDir = "/home/jordan/Desktop/Thesis/HERA-B_Ana/12mrad_5_3/";
        setupName = "setup_12mrad_5_3";
    }
	else if (geom == 3) {
            outDir = "/home/jordan/Desktop/Thesis_Results/HERA-B_Ana/4mrad_5_3/";
            setupName = "setup_4mrad_5_3";
        }
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
