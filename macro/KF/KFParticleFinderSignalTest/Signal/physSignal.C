//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 2.0
// @since   2.08.17
// 
// macro to reconstruct particles from signal events by KFParticleFinder
//_________________________________________________________________________________

void physSignal(Int_t nEvents = 1000, const char* setupName = "sis100_electron")
{
  TStopwatch timer;
  timer.Start();

  const int firstEventToRun = 0;
  const int lastEventToRun = firstEventToRun + nEvents - 1;

  TString inFile  = "mc.root";
  TString parFile = "params.root";
  TString recFile = "reco.root";
  TString outFile = "phys.root";
  TString effFile = "Efficiency_KFParticleFinder.txt";
  TString histoFile = "KFParticleFinder.root";
  
  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters";

  TString rootalias = workDir + TString("/macro/include/rootalias.C");
  gROOT->LoadMacro(rootalias.Data());
  
  TString setupFile = workDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = TString("setup_") + setupName;
  setupFunct += "()";
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  TString geoTag;
  
  TList *parFileList = new TList();

  //check if the simulation and reconstruction are complited
  TFile *fileMC = new TFile(inFile);
  if(fileMC->IsOpen())
  {
    TTree *treeMC = (TTree*) fileMC->Get("cbmsim");
    if(!treeMC) { std::cout << "[FATAL  ]  No MC tree available." << std::endl; return; }
    if(treeMC->GetEntriesFast() < nEvents)
    {
      std::cout << "[FATAL  ]  Simulation is incomplete. N mc events = " << treeMC->GetEntriesFast() << std::endl;
      return;
    }
  }
  else
  {
    std::cout << "[FATAL  ]  MC file does not exist." << std::endl;
    return;
  }

  TFile *fileReco = new TFile(recFile);
  if(fileReco->IsOpen())
  {
    TTree *treeReco = (TTree*) fileReco->Get("cbmsim");
    if(!treeReco) { std::cout << "[FATAL  ]  No Reco tree available." << std::endl; return; }
    if(treeReco->GetEntriesFast() < nEvents)
    {
      std::cout << "[FATAL  ]  Reconstruction is incomplete. N reco events = " << treeReco->GetEntriesFast() << std::endl;
      return;
    }
  }
  else
  {
    std::cout << "[FATAL  ]  Reco file does not exist." << std::endl;
    return;
  }

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->AddFriend(recFile);
  run->SetOutputFile(outFile);
  
    // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);

  // ------------------------------------------------------------------------
 //          Adjust this part according to your requirements
  CbmKF *KF = new CbmKF();
  run->AddTask(KF);
  CbmL1* l1 = new CbmL1("CbmL1",1, 3);
  if( setup->IsActive(kMvd) )
  {
    setup->GetGeoTag(kMvd, geoTag);
    const TString mvdMatBudgetFileName = paramDir + "/mvd/mvd_matbudget_" + geoTag + ".root";
    l1->SetMvdMaterialBudgetFileName(mvdMatBudgetFileName.Data());
  }
  if( setup->IsActive(kSts) )
  {
    setup->GetGeoTag(kSts, geoTag);
    const TString stsMatBudgetFileName = paramDir + "/sts/sts_matbudget_" + geoTag + ".root";
    l1->SetStsMaterialBudgetFileName(stsMatBudgetFileName.Data());
  }
  run->AddTask(l1);
  
    // ----- PID for KF Particle Finder --------------------------------------------
  CbmKFParticleFinderPID* kfParticleFinderPID = new CbmKFParticleFinderPID();
  kfParticleFinderPID->SetSIS100(); 
  kfParticleFinderPID->SetPIDMode(1);
//   kfParticleFinderPID->SetPIDMode(2);
  kfParticleFinderPID->UseTRDANNPID();
  kfParticleFinderPID->UseRICHRvspPID();
  run->AddTask(kfParticleFinderPID);
  
  // ----- KF Particle Finder --------------------------------------------
  CbmKFParticleFinder* kfParticleFinder = new CbmKFParticleFinder();
  kfParticleFinder->SetPIDInformation(kfParticleFinderPID);
  kfParticleFinder->SetPVToZero();
//  kfParticleFinder->SetSuperEventAnalysis(); // SuperEvent
  run->AddTask(kfParticleFinder);

// ----- KF Particle Finder QA --------------------------------------------
  CbmKFParticleFinderQA* kfParticleFinderQA = new CbmKFParticleFinderQA("CbmKFParticleFinderQA", 0, kfParticleFinder->GetTopoReconstructor(),histoFile.Data());
  kfParticleFinderQA->SetPrintEffFrequency(100);//nEvents);
//  kfParticleFinderQA->SetSuperEventAnalysis(); // SuperEvent
  kfParticleFinderQA->SetEffFileName(effFile.Data());
  run->AddTask(kfParticleFinderQA);

  // ----- KF Track QA --------------------------------------------
//  CbmKFTrackQA* kfTrackQA = new CbmKFTrackQA();
//  run->AddTask(kfTrackQA);
  
// -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(),"UPDATE");
  rtdb->setFirstInput(parIo1);
  if ( ! parFileList->IsEmpty() ) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------

// -----   Intialise and run   --------------------------------------------
  run->Init();
  
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  
  KFPartEfficiencies eff;
  for(int jParticle=eff.fFirstStableParticleIndex+10; jParticle<=eff.fLastStableParticleIndex; jParticle++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    if(!pdgDB->GetParticle(eff.partPDG[jParticle])){
        pdgDB->AddParticle(eff.partTitle[jParticle].data(),eff.partTitle[jParticle].data(), eff.partMass[jParticle], kTRUE,
                           0, eff.partCharge[jParticle]*3,"Ion",eff.partPDG[jParticle]);
    }
  }
  run->Run(firstEventToRun,lastEventToRun+1);
  // ------------------------------------------------------------------------

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);

  gROOT->ProcessLine("RemoveGeoManager()");
}
