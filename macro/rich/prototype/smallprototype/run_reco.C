using namespace std;

void run_reco(Int_t nEvents = 100000)
{
    TTree::SetMaxTreeSize(90000000000);

    Int_t iVerbose = 0;     // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)

    FairLogger* logger = FairLogger::GetLogger();
    logger->SetLogScreenLevel("INFO");
    logger->SetLogVerbosityLevel("LOW");

    gRandom->SetSeed(10);

    // -----   Environment   --------------------------------------------------

    TString myName = "run_reco";			// macro's name for screen output

    TString script = TString(gSystem->Getenv("SCRIPT"));

    // -----   In- and output file names   ------------------------------------
    TString outDir = "";

/*
    if (script == "yes") {
	setupName = TString(gSystem->Getenv("SETUP_NAME"));
	outDir = TString(gSystem->Getenv("OUT_DIR"));
    }
    else {
	outDir = "/home/aghoehne/Documents/CbmRoot/Gregor/";
    }
*/
	outDir = "/home/aghoehne/Documents/CbmRoot/Gregor/";


    	TString parFile = outDir + "param.00001.root";
    	TString mcFile = outDir +   "mc.00001.root";
    	TString geoFile = outDir  + "geofilefull.00001.root";
	TString recoFile = outDir  + "reco.00001.root";
    

    	TString geoSetupFile = "";
    	geoSetupFile = "/home/aghoehne/Documents/CbmRoot/trunk/macro/rich/prototype/smallprototype/geo.C";
	TString resultDir ="/home/aghoehne/Documents/CbmRoot/Gregor/results_smallprotype";


    // -----   Script initialization   ----------------------------------------
/*    if (script == "yes") {
	mcFile = TString(gSystem->Getenv("MC_FILE"));
	recoFile = TString(gSystem->Getenv("RECO_FILE"));
	parFile = TString(gSystem->Getenv("PAR_FILE"));
	resultDir = TString(gSystem->Getenv("LIT_RESULT_DIR"));
    }
*/
    // ------------------------------------------------------------------------

    remove(recoFile.Data());


    // -----   Load the geometry setup   --------------------------------------
    TString setupFunct = "geo()";
    cout << "-I- setupFile: " << geoSetupFile << endl
	<< "-I- setupFunct: " << setupFunct << endl;
    gROOT->LoadMacro(geoSetupFile);
    gROOT->ProcessLine(setupFunct);

    // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  cout << endl;
  cout << "-I- " << myName << ": Defining parameter files " << endl;
  TList *parFileList = new TList();
  TString geoTag;



    // ----    Debug option   -------------------------------------------------
    gDebug = 0;


    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();


    // -----   Reconstruction run   -------------------------------------------
    FairRunAna *run= new FairRunAna();
    if (mcFile != "") run->SetInputFile(mcFile);
    if (recoFile != "") run->SetOutputFile(recoFile);
    run->SetGenerateRunInfo(kTRUE);
    run->SetGeomFile(geoFile);
  
  // ------------------------------------------------------------------------


    // ----- MC Data Manager   ------------------------------------------------
    CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager",1);
    mcManager->AddFile(mcFile);
    run->AddTask(mcManager);
    // ------------------------------------------------------------------------


    // =========================================================================
    // ===                        RICH reconstruction                        ===
    // =========================================================================

    CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
    richDigitizer->SetNofNoiseHits(0);
    run->AddTask(richDigitizer);
    cout << "-I- digitize: Added task " << richDigitizer->GetName() << endl;


    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    richHitProd->SetRotationNeeded(false);
    run->AddTask(richHitProd);
    cout << "-I- hitProducer: Added task " << richHitProd->GetName() << endl;


    	

	CbmRichReconstruction* richReco = new CbmRichReconstruction();
	richReco->SetRunExtrapolation(false);
	richReco->SetRunProjection(false);
	richReco->SetRunTrackAssign(false);
	richReco->SetFinderName("ideal");
    	richReco->SetFitterName("circle_cop");
	run->AddTask(richReco);
    	cout << "-I- richReco: Added task " << richReco->GetName() << endl;

	CbmMatchRecoToMC* matchRecoToMc = new CbmMatchRecoToMC();
    	run->AddTask(matchRecoToMc);

	CbmRichSmallPrototypeQa* richQa = new CbmRichSmallPrototypeQa();
    	richQa->SetOutputDir(string(resultDir));
    	run->AddTask(richQa);
	


  


    // ===                 End of RICH local reconstruction                  ===
    // =========================================================================


	
// -----  Parameter database   --------------------------------------------
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	FairParRootFileIo* parIo1 = new FairParRootFileIo();
	FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
	parIo1->open(parFile.Data());
	//parIo2->open(parFileList, "in");
	rtdb->setFirstInput(parIo1);
	rtdb->setSecondInput(parIo2);
	rtdb->setOutput(parIo1);
	rtdb->saveOutput();
	//rtdb->print();

// -----   Intialize and run   --------------------------------------------
    	run->Init();
    	cout << "Starting run" << endl;
    	run->Run(0,nEvents);
  

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
    cout << " Test passed" << endl;
    cout << " All ok " << endl;


}
