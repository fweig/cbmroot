//#include "variables.h" //patch

void run_treemaker
(
  Int_t nEvents = 2,
  TString dataSet = "test",
  TString setupName = "sis100_electron"
)
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_treemaker";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString paramDir = srcDir + "/parameters";
  // ------------------------------------------------------------------------
  
  
  // -----   In- and output file names   ------------------------------------
  TString traFile = dataSet + ".tra.root";
  TString rawFile = dataSet + ".event.raw.root";
  TString recFile = dataSet + ".rec.root";
  TString geoFile = dataSet + ".geo.root";
  TString parFile = dataSet + ".par.root";
  TString outFile = dataSet + ".tree.root";
  TString KFeffFile = dataSet + ".KFeff.txt";
  TString KFqaFile = dataSet + ".KFQA.root";
  // ------------------------------------------------------------------------


  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
		
    
  TString geoTag;
  TList *parFileList = new TList();
  
  std::cout << "-I- " << myName << ": Using raw file " << rawFile << std::endl;
  std::cout << "-I- " << myName << ": Using parameter file " << parFile << std::endl;
  std::cout << "-I- " << myName << ": Using reco file " << recFile << std::endl;
  std::cout << "-I- " << myName << ": Using geo file " << geoFile << std::endl;
  
  
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(recFile);
  inputSource->AddFriend(traFile);
  inputSource->AddFriend(rawFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  // ------------------------------------------------------------------------
  
    
  // ----- Mc Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(traFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------
    
  // ---   STS track matching   ----------------------------------------------
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();    
  run->AddTask(matchTask);
  // ------------------------------------------------------------------------

  CbmKF *KF = new CbmKF();
  run->AddTask(KF);
    
  CbmL1* l1 = new CbmL1("CbmL1",1, 3);
  if( setup->IsActive(ECbmModuleId::kMvd) )
  {
    setup->GetGeoTag(ECbmModuleId::kMvd, geoTag);
    const TString mvdMatBudgetFileName = srcDir + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if( setup->IsActive(ECbmModuleId::kSts) )
  {
    setup->GetGeoTag(ECbmModuleId::kSts, geoTag);
    const TString stsMatBudgetFileName = srcDir + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);
    
  //BEGIN finder
  CbmKFParticleFinderPID* kfParticleFinderPID_TOF = new CbmKFParticleFinderPID("fkfParticleFinderPID_TOF");
  kfParticleFinderPID_TOF->SetSIS100();
  kfParticleFinderPID_TOF->SetPIDMode(2); //0 - topology, 1 - mc, 2 - tof
  run->AddTask(kfParticleFinderPID_TOF);
  
  CbmKFParticleFinder* kfParticleFinder_TOF = new CbmKFParticleFinder("fCbmKFParticleFinder_TOF");
  kfParticleFinder_TOF->SetPIDInformation(kfParticleFinderPID_TOF);
  kfParticleFinder_TOF->AddDecayToReconstructionList(3122);
  kfParticleFinder_TOF->AddDecayToReconstructionList(-3122);
  kfParticleFinder_TOF->AddDecayToReconstructionList(310);
  run->AddTask(kfParticleFinder_TOF);
    
  CbmKFParticleFinderPID* kfParticleFinderPID_MC = new CbmKFParticleFinderPID("fkfParticleFinderPID_MC");
  kfParticleFinderPID_MC->SetSIS100();
  kfParticleFinderPID_MC->SetPIDMode(1); //0 - topology, 1 - mc, 2 - tof
  run->AddTask(kfParticleFinderPID_MC);
    
  CbmKFParticleFinder* kfParticleFinder_MC = new CbmKFParticleFinder("fCbmKFParticleFinder_MC");
  kfParticleFinder_MC->SetPIDInformation(kfParticleFinderPID_MC);
  kfParticleFinder_MC->AddDecayToReconstructionList(3122);
  kfParticleFinder_MC->AddDecayToReconstructionList(-3122);
  kfParticleFinder_MC->AddDecayToReconstructionList(310);
  run->AddTask(kfParticleFinder_MC);
  
  // ----- KF Particle Finder QA --------------------------------------------
  CbmKFParticleFinderQA* kfParticleFinderQA = new CbmKFParticleFinderQA("CbmKFParticleFinderQA", 0, kfParticleFinder_MC->GetTopoReconstructor(),KFqaFile.Data());
  kfParticleFinderQA->SetPrintEffFrequency(100);//nEvents);
//  kfParticleFinderQA->SetSuperEventAnalysis(); // SuperEvent
  kfParticleFinderQA->SetEffFileName(KFeffFile.Data());
  run->AddTask(kfParticleFinderQA);

  //END finder
  cout << "KF done" << endl;

  // ----- KF Particle Finder QA --------------------------------------------
  DataTreeCbmInterface* fInterface = new DataTreeCbmInterface();
  fInterface->LoadGeo(geoFile);
    
  fInterface -> SetKFParticleFinderTOF(kfParticleFinder_TOF) ;
  fInterface -> SetKFParticleFinderMC(kfParticleFinder_MC) ;
  fInterface -> SetOutputFile(outFile);
    
  run -> AddTask(fInterface);
  cout << "DataTreeCbmInterface set" << endl;
  // ------------------------------------------------------------------------
    
  // ----- KF Track QA --------------------------------------------
  CbmKFTrackQA* kfTrackQA = new CbmKFTrackQA();
  run->AddTask(kfTrackQA);
  // ------------------------------------------------------------------------
    
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
  // ------------------------------------------------------------------------
    
  // -----   Intialise and run   --------------------------------------------
  run->Init();
    
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
    
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);

  // -----   CTest resource monitoring   ------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory=sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  std::cout << "<DartMeasurement name=\"WallTime\" type=\"numeric/double\">";
  std::cout << rtime;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage=ctime/rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  RemoveGeoManager();
  // ------------------------------------------------------------------------
}
