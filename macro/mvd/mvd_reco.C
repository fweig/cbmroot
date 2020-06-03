// --------------------------------------------------------------------------
//
// Macro for local MVD reconstruction from MC data
//
// Tasks:  CbmMvdDigitiser
//         CbmMvdClusterfinder
//         CbmMvdHitfinder
// 
//
// V. Friese   23/04/2009
// Update: P. Sitzmann 22/11/2016
// --------------------------------------------------------------------------


void mvd_reco(Int_t nEvents = 100, TString setup = "sis100_electron")
{

     // ========================================================================
    //          Adjust this part according to your requirements

    TString inDir   = gSystem->Getenv("VMCWORKDIR");

    TString outDir="data/";
    // Input file (MC events)
    TString inFile = outDir+ "mvd.mc.root";

    // Parameter file name
    TString parFile =outDir+ "params.root";

    // Output file
    TString outFile =outDir+ "mvd.reco.root";

    // Background file (MC events, for pile-up)
    TString bgFile = inFile;

    // Delta file (Au Ions)
    TString deltaFile =outDir+ "mvd.mcDelta.root";

    Int_t iVerbose = 0;

    FairLogger* logger = FairLogger::GetLogger();
    logger->SetLogScreenLevel("INFO");
    logger->SetLogVerbosityLevel("LOW");


    TString setupFile = inDir + "/geometry/setup/setup_" + setup + ".C";
    TString setupFunct = "setup_";
    setupFunct = setupFunct + setup + "()";

    gROOT->LoadMacro(setupFile);
    gInterpreter->ProcessLine(setupFunct);

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
    FairRunAna *fRun= new FairRunAna();
    fRun->SetInputFile(inFile);
    fRun->SetOutputFile(outFile);
    // ------------------------------------------------------------------------

    // ----- Mc Data Manager   ------------------------------------------------
    CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
    mcManager->AddFile(inFile);
    fRun->AddTask(mcManager);
    // ------------------------------------------------------------------------
  

    // -------   MVD Digitiser   ----------------------------------------------
    CbmMvdDigitizer* digi = new CbmMvdDigitizer("MVDDigitiser", 0, iVerbose);
    cout << "Adding Task:  CbmMvdDigitiser... " << endl;
    fRun->AddTask(digi);
    
    //--- Pile Up -------
    
    Int_t pileUpInMVD=3; // use 1 or higher
    
    digi->SetBgFileName(bgFile);
    digi->SetBgBufferSize(300);
    digi->SetPileUp(pileUpInMVD-1);


    //--- Delta electrons -------
    digi->SetDeltaName(deltaFile);
    digi->SetDeltaBufferSize(1000);
    digi->SetDeltaEvents(pileUpInMVD*100); // for simulation assumes 1% target

    //digi->ShowDebugHistograms();
    

  // -----   MVD Clusterfinder   ---------------------------------------------
  CbmMvdClusterfinder* mvdCluster = new CbmMvdClusterfinder("MVD Clusterfinder", 0, iVerbose);
  fRun->AddTask(mvdCluster);
  // -------------------------------------------------------------------------

    
    CbmMvdHitfinder* mvd_hit   = new CbmMvdHitfinder("MVDFindHits", 0, iVerbose);
    mvd_hit->UseClusterfinder(kTRUE);
    fRun->AddTask(mvd_hit);

    //----------------------------------------------------------------------------
    // -----  Parameter database   -----------------------------------------------
    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo*  parIo1 = new FairParRootFileIo();
    parIo1->open(parFile.Data());

    rtdb->setFirstInput(parIo1);
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    rtdb->print();
    // ---------------------------------------------------------------------------



    // -----   Run initialisation   ----------------------------------------------
    fRun->Init();
    // ---------------------------------------------------------------------------



    // -----   Start run   -------------------------------------------------------
    fRun->Run(0,nEvents);
    // ---------------------------------------------------------------------------



    // -----   Finish   ----------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is "    << outFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
    cout << endl;
    // ---------------------------------------------------------------------------


  //  delete run;

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
 

}
