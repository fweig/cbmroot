FairTask * addpapa(TString configPath, TString configFunct, TString configName) {

  TString myName = "run_analysis";
  std::cout << "-I- " << myName << ": Loading task " << (configFunct + configName) << std::endl;

  TString cfgFile  = configPath + configFunct + configName + ".C";
  //  std::cout << "-I- " << myName << ": Loading macro " << cfgFile << std::endl;
  gROOT->LoadMacro( (configPath + configFunct + configName + ".C") );

  TString cfgFunct = configFunct + configName + "(\""+ configName +"\")";
  //  std::cout << "-I- " << myName << ": process line " << cfgFunct << std::endl;
  return ( dynamic_cast<FairTask*>(gROOT->ProcessLine( cfgFunct )) );


}

void run_analysis(Int_t nEvents = 0)
{

  // -----   Environment   --------------------------------------------------
  TString myName = "run_analysis";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output directory names   -------------------------------
  TString inDir   = gSystem->Getenv("INDIR");
  TString inFile  = gSystem->Getenv("INFILE"); // input file list
  TString outDir  = gSystem->Getenv("OUTDIR");
  if(outDir.IsNull()) outFile = ".";
  // ------------------------------------------------------------------------

  // --- Logger settings ----------------------------------------------------
  gErrorIgnoreLevel = kFatal; //kInfo, kWarning, kError, kFatal;
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  /// load local copy of setup file (same as used for simulation and reconstruction)
  std::cout << std::endl;
  TString setupName = gSystem->GetFromPipe( Form("echo $(basename $(ls %s/setup_*.C))",inDir.Data()) );
  setupName.ReplaceAll("setup_","");
  setupName.ReplaceAll(".C","");

  TString setupFile = inDir + "/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // ------------------------------------------------------------------------


  // -----   Modify Cbm setup   ---------------------------------------------
  std::cout << std::endl;
  // --- remove detector geometries
  // setup->RemoveModule(kMvd);   // remove mvd and its material budget
  // setup->RemoveModule(kTrd);   // e.g. for sts-tof-matching study
  setup->RemoveModule(kPsd);      // remove psd from setup
  // --- change default geomerties
  //  setup->SetModule(kTrd, "v15d_1e", kTRUE); // 5 TRD layer
  std::cout << "-I- " << myName << ": CbmSetup updated " << std::endl;
  // ------------------------------------------------------------------------


  // -----   Run manager + I/O   --------------------------------------------
  std::cout << std::endl;
  FairRunAna* run = new FairRunAna();
  run->SetOutputFile( outDir + "/" + setupName + "_analysis.root");
  FairFileSource *src = NULL;

  /// stopwatch
  TStopwatch timer;
  timer.Start();

  Int_t i=0;
  TString file;
  //  char filename[300];
  ifstream in(inFile);
  TList *parList = new TList();
  /// loop over all file in list
  while ( in >> file ) {
    //    TString file(filename);// + "/" + setupName;
    file += "/";
    file += setupName;

    // mc sim file
    if(!i) src = new FairFileSource(file + "_mc.root");
    else   src->AddFile(     file + "_mc.root");

    // (skimmed) reco file
    src->AddFriend(file + "_reco.root");

    // parameter files
    TObjString *parFile = new TObjString((file + "_params.root").Data());
    parList->Add(parFile);
    //  parIo1->open(parFile.GetString().Data());

    i++;
  }

  // add source to run
  run->SetSource(src);

  // set parameter list
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parList);
  std::cout << "-I- " << myName << ": in/output added " << std::endl;
  // ------------------------------------------------------------------------


  // -----   L1/KF tracking + PID   -----------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading tasks " << std::endl;

  //CbmKF is needed for Extrapolation
  CbmKF* kf = new CbmKF();
  run->AddTask(kf);
  std::cout << "-I- : Added task " << kf->GetName() << std::endl;

  CbmL1* l1 = new CbmL1();
  // --- Material budget file names
  if ( setup->IsActive(kMvd) ) {
    TString geoTag;
    setup->GetGeoTag(kMvd, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile = matFile + "/parameters/mvd/mvd_matbudget_" + geoTag + ".root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetMvdMaterialBudgetFileName(matFile.Data());
  }
  if ( setup->IsActive(kSts) ) {
    TString geoTag;
    setup->GetGeoTag(kSts, geoTag);
    TString matFile = gSystem->Getenv("VMCWORKDIR");
    matFile = matFile + "/parameters/sts/sts_matbudget_" + geoTag + ".root";
    std::cout << "Using material budget file " << matFile << std::endl;
    l1->SetStsMaterialBudgetFileName(matFile.Data());
  }
  run->AddTask(l1);
  std::cout << "-I- : Added task " << l1->GetName() << std::endl;

  // --- TRD pid tasks
  if ( setup->IsActive(kTrd) ) {
    // ----------- TRD track Pid Ann ----------------------
    CbmTrdSetTracksPidANN* trdSetTracksPidAnnTask = new CbmTrdSetTracksPidANN("Ann", "Ann");
    run->AddTask(trdSetTracksPidAnnTask);
    std::cout << "-I- : Added task " << trdSetTracksPidAnnTask->GetName() << std::endl;
    // ----------------------------------------------------

    // ----------- TRD track Pid Like ----------------------
    FairTask *getTRDli = dynamic_cast<FairTask*>( gROOT->Macro( "$VMCWORKDIR/macro/papa/Config_trd_PidLI.C" ));
    run->AddTask( getTRDli );
    std::cout << "-I- : Added task " << getTRDli->GetName() << std::endl;

    // CbmTrdSetTracksPidLike* trdLI = new CbmTrdSetTracksPidLike("TRDLikelihood", "TRDLikelihood");
    // trdLI->SetUseMCInfo(kTRUE);
    // trdLI->SetUseMomDependence(kFALSE);
    // run->AddTask(trdLI);
    // std::cout << "-I- : Added task " << trdLI->GetName() << std::endl;
    // ------------------------------------------------------------------------
  }

  // -----   PAPA tasks   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading private tasks " << std::endl;

  // gSystem->Exec(Form("cp -u %s/*.C $OUTDIR/../.",configPath.Data()));
  TString cfgPath = outDir + "/../";
  TString cfgFunc = "Config_jbook_";

  // run->AddTask( addpapa(cfgPath,cfgFunc, "MC")    );
  // run->AddTask( addpapa(cfgPath,cfgFunc, "URQMD") );
  // run->AddTask( addpapa(cfgPath,cfgFunc, "MVD")   );
  // run->AddTask( addpapa(cfgPath,cfgFunc, "MUCH")  );
  //  run->AddTask( addpapa(cfgPath,cfgFunc, "TOF")   );

  // run->AddTask( addpapa(cfgPath,cfgFunc, "QA")    );
  // run->AddTask( addpapa(cfgPath,cfgFunc, "StsQA") );

  //run->AddTask( addpapa(cfgPath,cfgFunc, "AA")    );
  // run->AddTask( addpapa(cfgPath,cfgFunc, "pA")    );
  //  run->AddTask( addpapa(cfgPath,cfgFunc, "RF")    );

  // run->AddTask( addtask(cfgPath,cfgFunc, "PID")   );
  // run->AddTask( addtask(cfgPath,cfgFunc, "Perf")  );
  // run->AddTask( addtask(cfgPath,cfgFunc, "Phi")   );

  //  run->AddTask( addpapa(cfgPath,cfgFunc, "AAfast")    );
  //  run->AddTask( addpapa(cfgPath,cfgFunc, "RFfast")    );
  //  run->AddTask( addpapa(cfgPath,cfgFunc, "Test")   );

  // ------------------------------------------------------------------------

  /// fair runtime database
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();

  /// intialize and run
  run->Init();
  run->Run(0, nEvents);

  timer.Stop();
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << " Output file is " << (outDir + setupName + "_analysis.root") << std::endl;
  //  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
