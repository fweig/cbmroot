// -----------------------------------------------------------------------------
// ----- test_init.C                                                       -----
// -----                                                                   -----
// ----- created by C. Simon on 2016-02-04                                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

void test_init() 
{
  FairRunOnline* run = new FairRunOnline();

  gLogger->SetLogScreenLevel("ERROR");
  gLogger->SetLogVerbosityLevel("MEDIUM");

  TList *parFileList = new TList();

  TString workDir = gSystem->Getenv("VMCWORKDIR");
  TString paramDir = workDir + "/macro/tof/beamtime/nov15";

  TObjString unpParFile = paramDir + "/parUnpCernNov2015.txt";					
  parFileList->Add(&unpParFile);

  TString parFile = paramDir + "/test_init.params.root";

  TString outFile = paramDir + "/test_init.out.root";

  CbmHldSource* source = new CbmHldSource();
  source->AddFile("/hera/cbm/users/tofGsiApr14/cern-nov15/production/CbmTofSps_01Dec0427_15335042707.hld");
//  source->AddFile("/mnt/nas-herrmann2/cern-nov15/production/CbmTofSps_01Dec0427_15335042707.hld");

  TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
  source->AddUnpacker( tofTrbDataUnpacker );

  run->SetSource(source);
  run->SetOutputFile(outFile);
  run->SetAutoFinish(kFALSE);

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);

  rtdb->getContainer("TMbsUnpackTofPar");

  run->Init();
}

