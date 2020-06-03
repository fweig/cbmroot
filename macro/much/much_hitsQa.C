// --------------------------------------------------------------------------
// Macro for qualitycheck of the MUCH hit producer
// E.Kryshen 22.11.2007
// P. P. Bhaduri on 23.11.2017
// flag = 0 for sis100 & flag = 1 for mcbm
// --------------------------------------------------------------------------
void much_hitsQa(Int_t flag=1){
  Int_t nEvents = 2;
  Int_t iVerbose = 1;

  TString  dir            = "data/";
  TString  mcFile         = dir + "mc.sis100b.test.root";
  TString  parFile        = dir + "params.sis100b.test.root";
  TString  digiFile       = dir + "much_digi_sector_sis100b.root";
  TString  muchHitsFile   = dir + "hits.sis100b.test.root";
  TString  outFile        = dir + "hits_gem.qa.sis100b.test.root";

  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  //basiclibs();
  gROOT->LoadMacro("$VMCWORKDIR/macro/much/muchlibs.C");
  //muchlibs();
  gROOT->LoadMacro("$VMCWORKDIR/much/much_histo_style.C");
  //much_histo_style();

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(mcFile);
  fRun->AddFriend(muchHitsFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  TString muchDigiFile = gSystem->Getenv("VMCWORKDIR");
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  // ---  MuCh hit finder QA-------------------------------------------------
  CbmMuchHitFinderQa* qa = new CbmMuchHitFinderQa("HitQa",2);
  qa->SetGeoFileName(digiFile);
  qa->SetPerformanceFileName(outFile);
  qa-> SetGeometryID(flag);
  qa->SetPrintToFile(0);
  qa->SetPullsQa(0);
  qa->SetOccupancyQa(0);
  qa->SetDigitizerQa(0);
  qa->SetStatisticsQa(1);
  qa->SetClusterDeconvQa(0);

  fRun->AddTask(qa);
  // ------------------------------------------------------------------------

  // -----   Initialize and run --------------------------------------------
  fRun->Init();
  fRun->Run(0,nEvents);
  // ------------------------------------------------------------------------
}
