void run_reco_mcbm(Int_t nEvents = 1000)
{

  TTree::SetMaxTreeSize(90000000000);
  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_reco_mcbm";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  //   TString geoSetupFile = srcDir + "/macro/rich/geosetup/rich_setup_sis18_mcbm_20deg_long.C";
  TString geoSetupFile = srcDir + "/geometry/setup/setup_sis18_mcbm_20deg_long.C";

  TString outDir = "/Users/slebedev/Development/cbm/data/sim/rich/mcbm/";
  // TString outDir = "/home/aghoehne/Documents/CbmRoot/Gregor/";
  TString mcFile   = outDir + "mc.00000.root";
  TString parFile  = outDir + "param.00000.root";
  TString recoFile = outDir + "reco.00000.root";
  TString geoFile  = outDir + "sis18_mcbm_20deg_long_geofile_full.root";
  //   TString geoFile = outDir + "geosim.00000.root";
  std::string resultDir = "results_mcbm_rich/";

  if (script == "yes") {
    mcFile       = TString(gSystem->Getenv("MC_FILE"));
    recoFile     = TString(gSystem->Getenv("RECO_FILE"));
    parFile      = TString(gSystem->Getenv("PAR_FILE"));
    resultDir    = TString(gSystem->Getenv("RESULT_DIR"));
    geoFile      = TString(gSystem->Getenv("GEOSIM_FILE"));
    geoSetupFile = srcDir + TString(gSystem->Getenv("GEO_SETUP_FILE"));
  }

  remove(recoFile.Data());
  /*
    //Load geometry setup
    TString setupFunct = "do_setup()";
    std::cout << "-I- " << myName << ": Loading macro " << geoSetupFile << std::endl;
    gROOT->LoadMacro(geoSetupFile);
    gROOT->ProcessLine(setupFunct);
    CbmSetup* setup = CbmSetup::Instance();
    
    */

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile  = srcDir + "/geometry/setup/setup_sis18_mcbm_20deg_long.C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + "sis18_mcbm_20deg_long()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // ------------------------------------------------------------------------


  // Parameter files
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // TRD digitisation parameters
  if (setup->GetGeoTag(kTrd, geoTag)) {
    TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
    parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file " << trdFile->GetString() << std::endl;
  }

  // TOF digitisation parameters
  if (setup->GetGeoTag(kTof, geoTag)) {
    TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
    parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofFile->GetString() << std::endl;
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunAna* run = new FairRunAna();
  if (mcFile != "") run->SetInputFile(mcFile);
  if (recoFile != "") run->SetOutputFile(recoFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGeomFile(geoFile);

  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");


  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
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
  macroName = srcDir + "/macro/mcbm/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct()");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName << std::endl;
    return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " excuted successfully" << std::endl;

  CbmMatchRecoToMC* match = new CbmMatchRecoToMC();
  run->AddTask(match);

  FairTask* richMCbmQa = new CbmRichMCbmQa();
  run->AddTask(richMCbmQa);

  // Parameter database
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();


  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();


  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);

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
