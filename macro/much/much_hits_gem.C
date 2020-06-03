// flag =0 for sis100 flag =1 for mini cbm geometry
void much_hits_gem(Int_t flag=1){
  Int_t nEvents = 10;
  TString inFile   = "data/mc.test.mcbm.root";
  TString outFile  = "data/hits.test.mcbm.root";
  TString digiFile = "data/much_digi_sector_mcbm.root";
  TString parFile  = "data/params.test.mcbm.root";

  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  //  basiclibs();
  gROOT->LoadMacro("$VMCWORKDIR/macro/much/muchlibs.C");
  //muchlibs();
  
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);

  CbmMuchDigitizeGem* digitize = new CbmMuchDigitizeGem(digiFile,flag);
  CbmMuchFindHitsGem* findHits = new CbmMuchFindHitsGem(digiFile,flag);
  fRun->AddTask(digitize);
  fRun->AddTask(findHits);
  fRun->Init();
  fRun->Run(0,nEvents);
}
