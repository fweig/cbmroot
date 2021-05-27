void run_sim_cosy17(Int_t nEvents = 500)
{

  TTree::SetMaxTreeSize(90000000000);

  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_sim_geotest";              // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString richGeoTag = "cosy17_quartz";

  TString dir        = "/Users/slebedev/Development/cbm/data/sim/rich/richprototype/";
  TString asciiInput = "proton_1.25gevc.ascii.dat";
  TString parFile    = dir + "/param.00001.root";
  TString geoFile    = dir + "/geofilefull.00001.root";
  TString mcFile     = dir + "/mc.00001.root";

  remove(parFile.Data());
  remove(mcFile.Data());
  remove(geoFile.Data());


  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");
  run->SetOutputFile(mcFile);
  run->SetGenerateRunInfo(kTRUE);


  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // Initialize geometry setup
  CbmSetup* setup = CbmSetup::Instance();
  if (!setup->IsEmpty()) { setup->Clear(); }
  setup->SetTitle("RICH COSY17");
  setup->SetModule(kRich, richGeoTag);
  setup->Print();

  run->SetMaterials("media.geo");  // Materials


  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/registerSetup.C";
  std::cout << std::endl << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");


  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairAsciiGenerator* asciiGen  = new FairAsciiGenerator(asciiInput);
  primGen->AddGenerator(asciiGen);

  run->SetGenerator(primGen);
  //run->SetStoreTraj(true);

  run->Init();

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //    CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  //    fieldPar->SetParameters(magField);
  //    fieldPar->setChanged();
  //    fieldPar->setInputVersion(run->GetRunId(),1);
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  run->Run(nEvents);

  run->CreateGeometryFile(geoFile);

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
