void run_reco_Jan2019(Int_t nEvents = 10000)
{
  TTree::SetMaxTreeSize(90000000000);
  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_reco";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString setupName = "setup_misalign_gauss_sigma_1",
          outDir    = "/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV/Misaligned_1mrad_new/";

  TString parFile  = outDir + "merge_param.root";
  TString mcFile   = outDir + "merge_mc.root";
  TString geoFile  = outDir + "_geofilefull.root";
  TString recoFile = outDir + setupName + "merge_reco.root";
  TString outDir2  = outDir;

  TString geoSetupFile = "";
  geoSetupFile         = "/data/ROOT6/trunk/macro/rich/alignment/"
                 "misalignment_correction/Sim_Thesis/geosetup/"
                 + setupName + ".C";

  if (script == "yes") {
    setupName    = TString(gSystem->Getenv("SETUP_NAME"));
    geoSetupFile = TString(gSystem->Getenv("VMCWORKDIR")) + TString(gSystem->Getenv("GEO_SETUP_FILE"));
    outDir       = TString(gSystem->Getenv("DIR"));
    mcFile       = TString(gSystem->Getenv("MC_FILE"));
    parFile      = TString(gSystem->Getenv("PAR_FILE"));
    recoFile     = TString(gSystem->Getenv("RECO_FILE"));
  }

  remove(recoFile.Data());

  // -----   Load the geometry setup   --------------------------------------
  const char* setupName2 = setupName;
  TString setupFunct     = "";
  setupFunct             = setupFunct + setupName2 + "()";
  std::cout << "-I- setupFile: " << geoSetupFile << std::endl << "-I- setupFunct: " << setupFunct << std::endl;
  gROOT->LoadMacro(geoSetupFile);
  gROOT->ProcessLine(setupFunct);
  //gInterpreter->ProcessLine(setupFunct);
  // ------------------------------------------------------------------------


  CbmSetup* setup = CbmSetup::Instance();

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  TStopwatch timer;
  timer.Start();
  gDebug = 0;


  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(mcFile);
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile);
  run->SetGenerateRunInfo(kTRUE);

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
  macroName = srcDir + "/macro/run/modules/reconstruct_batch_Jan2019.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct_batch_Jan2019()");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName << std::endl;
    return;
  }
  std::cout << "-I-" << myName << ": " << macroName << " executed successfully" << std::endl;


  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }

  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);


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
