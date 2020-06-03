//---------------------------------------------------------------------------------
// @author  M. Zyzak
// @version 1.0
// @since   15.08.14
// 
// macro to reconstruct particles from signal events by KFParticleFinder
//_________________________________________________________________________________

void run_phys(TString setupName, Int_t nEvents = 10, TString inputDir = "")
{
  TStopwatch timer;
  timer.Start();

  const int firstEventToRun = 0;
  const int lastEventToRun = firstEventToRun + nEvents - 1;

  TString inFile  = inputDir + TString("/sim.root");
  TString parFile = inputDir + TString("/param.root");
  TString recFile = inputDir + TString("/reco.root");
  TString outFile = "phys.root";
  TString effFile = "Efficiency_KFParticleFinder.txt";
  TString histoFile = "KFParticleFinder.root";
  
  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/parameters";

  TString setupFile = workDir + "/geometry/setup/setup_" + setupName.Data() + ".C";
  TString setupFunct = TString("setup_") + setupName;
  setupFunct += "()";
  gROOT->LoadMacro(setupFile);
  gInterpreter->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  TString geoTag;
  
  TList *parFileList = new TList();

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->AddFriend(recFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  
  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);

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
//  kfParticleFinderPID->SetPIDMode(1);
  kfParticleFinderPID->SetPIDMode(2);
  kfParticleFinderPID->UseTRDANNPID();
  kfParticleFinderPID->UseRICHRvspPID();
  run->AddTask(kfParticleFinderPID);
  
  // ----- KF Particle Finder --------------------------------------------
  CbmKFParticleFinder* kfParticleFinder = new CbmKFParticleFinder();
  kfParticleFinder->SetPIDInformation(kfParticleFinderPID);
//   kfParticleFinder->SetPVToZero();
//  kfParticleFinder->SetSuperEventAnalysis(); // SuperEvent
  run->AddTask(kfParticleFinder);

  // ----- KF Particle Finder QA --------------------------------------------
  CbmKFParticleFinderQA* kfParticleFinderQA = new CbmKFParticleFinderQA("CbmKFParticleFinderQA", 0, kfParticleFinder->GetTopoReconstructor(),histoFile.Data());
  kfParticleFinderQA->SetPrintEffFrequency(nEvents);
//  kfParticleFinderQA->SetSuperEventAnalysis(); // SuperEvent
  kfParticleFinderQA->SetEffFileName(effFile.Data());
  run->AddTask(kfParticleFinderQA);

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
  cout << "Starting run" << endl;
  run->Run(firstEventToRun,lastEventToRun+1);
  // ------------------------------------------------------------------------

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);  
}
