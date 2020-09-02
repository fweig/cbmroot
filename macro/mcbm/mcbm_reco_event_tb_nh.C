// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated mCBM events with standard settings
//
// Event-by-event reconstruction; requires appropriate digitization before
// (see mcbm_digi.C)
//
// Local reconstruction in MVD, STS, MUCH, TRD and TOF
// Binned tracker for track reconstruction
//
// V. Friese   11.06.2018
//
// --------------------------------------------------------------------------

void mcbm_reco_event_tb_nh(
    Int_t nEvents = 10,
    TString RunId  = "test",
    TString InDir  = "./data/",
    TString OutDir = "./data/",
    TString setupName  = "mcbm_beam_2021_03",	
    bool timebased = kTRUE,
	Double_t eventRate = 1.e5,        // Interaction rate [1/s]
	Double_t timeSliceLength = 1.e4,  // Length of time-slice [ns]    
    Double_t Tint=100., 
    Double_t ReqTofMul=2.
)
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  //TString logVerbosity = "VERYHIGH";
  //TString logVerbosity = "HIGH";
  //TString logVerbosity = "MEDIUM";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_reco";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  //TString TraDir ="../../../../../../uhlig/mcbm_proposal/data"; 
  TString TraDir  = InDir;
  TString traFile = TraDir + "/" + RunId + ".tra.root";  
  TString dataset = InDir + "/" + RunId;
  TString parFile = dataset + ".par.root";
  TString rawFile = dataset + ".event.raw.root";
  TString recFile = OutDir + "/" + RunId + ".rec.root";
  if (timebased) {
    rawFile = dataset + Form(".%2.1e",eventRate) + ".raw.root";
    recFile = OutDir + "/" + RunId + Form(".%2.1e.%d.%d",eventRate,(Int_t)Tint,(Int_t)ReqTofMul) + ".rec.root";
  }

  // ------------------------------------------------------------------------

  Int_t iTofCluMode=1; // 1 - CbmTofEventClusterizer
  Int_t iTrackMode=0;

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  TString setupFile = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  setup->RemoveModule(ECbmModuleId::kTrd);
//  setup->RemoveModule(ECbmModuleId::kTof);
//  setup->RemoveModule(ECbmModuleId::kSts);
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList *parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if ( setup->GetGeoTag(ECbmModuleId::kTrd, geoTag) ) {
  	TObjString* trdFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
  	parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if ( setup->GetGeoTag(ECbmModuleId::kTof, geoTag) ) {
  	TObjString* tofFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digi.par");
  	parFileList->Add(tofFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << tofFile->GetString() << std::endl;
  	TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
  	parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << rawFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna *run = new FairRunAna();
  run->SetInputFile(rawFile);
  run->AddFriend(traFile);
  run->SetOutputFile(recFile);
  run->SetGenerateRunInfo(kFALSE);
  Bool_t hasFairMonitor = kFALSE; //Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  // ----- MC Data Manager   ------------------------------------------------
  //CbmMCDataManager* mcManager=new CbmMCDataManager("MCManager", 1);
  //mcManager->AddFile(rawFile);
  //run->AddTask(mcManager);
  // ------------------------------------------------------------------------  
  
    CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
    //  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
    //  eventBuilder->SetMaximumTimeGap(100.);
    eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
    eventBuilder->SetFixedTimeWindow(200.);
    eventBuilder->SetTriggerMinNumberT0(0);
    eventBuilder->SetTriggerMinNumberSts(0);
    eventBuilder->SetTriggerMinNumberMuch(0);
    eventBuilder->SetTriggerMinNumberTof(1);
    eventBuilder->SetTriggerMinNumberRich(0);
    eventBuilder->SetFillHistos(kTRUE);
  if (timebased)  run->AddTask(eventBuilder);

  // -----   Local reconstruction in MVD   ----------------------------------
  if ( setup->IsActive(ECbmModuleId::kMvd) ) {

    CbmMvdClusterfinder* mvdCluster =
            new CbmMvdClusterfinder("MVD Cluster Finder", 0, 0);
    run->AddTask(mvdCluster);
    std::cout << "-I- " << myName << ": Added task "
        << mvdCluster->GetName() << std::endl;

    CbmMvdHitfinder* mvdHit = new CbmMvdHitfinder("MVD Hit Finder", 0, 0);
    mvdHit->UseClusterfinder(kTRUE);
    run->AddTask(mvdHit);
    std::cout << "-I- " << myName << ": Added task "
        << mvdHit->GetName() << std::endl;

  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  if ( setup->IsActive(ECbmModuleId::kSts) ) {
    CbmRecoSts* stsReco=NULL;
    if (timebased) {     
      stsReco = new CbmRecoSts(kCbmRecoEvent,kFALSE,kFALSE);
//      stsReco = new CbmRecoSts();
    }else {
      stsReco = new CbmRecoSts();
    }
    if(kFALSE && timebased) {
      // ASIC params: #ADC channels, dyn. range, threshold, time resol., dead time,
      // noise RMS, zero-threshold crossing rate
      auto parAsic = new CbmStsParAsic(32, 75000., 3000., 5., 800., 1000., 3.9789e-3);
  
      // Module params: number of channels, number of channels per ASIC
      auto parMod = new CbmStsParModule(2048, 128);
      parMod->SetAllAsics(*parAsic);
      stsReco->UseModulePar(parMod);
      
      // Sensor params
      auto sensorPar = new CbmStsParSensor(CbmStsSensorClass::kDssdStereo);
      sensorPar->SetPar(0, 6.2092);  // Extension in x
      sensorPar->SetPar(1, 6.2);     // Extension in y
      sensorPar->SetPar(2, 0.03);    // Extension in z
      sensorPar->SetPar(3, 5.9692);  // Active size in y
      sensorPar->SetPar(4, 1024.);   // Number of strips front side
      sensorPar->SetPar(5, 1024.);   // Number of strips back side
      sensorPar->SetPar(6, 0.0058);  // Strip pitch front side 
      sensorPar->SetPar(7, 0.0058);  // Strip pitch back side
      sensorPar->SetPar(8, 7.5);      // Stereo angle front side
      sensorPar->SetPar(9, 0.0);     // Stereo angle back side
      stsReco->UseSensorPar(sensorPar);
  
      // Sensor conditions: full depletion voltage, bias voltage, temperature,
      // coupling capacitance, inter-strip capacitance
      auto sensorCond = new CbmStsParSensorCond(70., 140., 268., 17.5, 1.);
      stsReco->UseSensorCond(sensorCond);
    }
    run->AddTask(stsReco);
    std::cout << "-I- : Added task " << stsReco->GetName() << std::endl;

  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in MUCH   ---------------------------------
  if ( setup->IsActive(ECbmModuleId::kMuch) ) {
  /*
        // --- Parameter file name
        TString geoTag;
        setup->GetGeoTag(ECbmModuleId::kMuch, geoTag);
        Int_t muchFlag=0;
        if (geoTag.Contains("mcbm")) muchFlag=1;

        TString parFile = gSystem->Getenv("VMCWORKDIR");

        if (muchFlag) {
           std::cout << geoTag << std::endl;
           parFile = parFile + "/parameters/much/much_" + geoTag
                             + "_digi_sector.root";
           std::cout << "Using parameter file " << parFile << std::endl;
         } else {
           std::cout << geoTag(0,4) << std::endl;
           parFile = parFile + "/parameters/much/much_" + geoTag(0,4)
                             + "_digi_sector.root";
           std::cout << "Using parameter file " << parFile << std::endl;
         }


        // --- Hit finder for GEMs
        FairTask* muchHitGem = new CbmMuchFindHitsGem(parFile.Data(),muchFlag);
        run->AddTask(muchHitGem);
        std::cout << "-I- " << myName << ": Added task "
            << muchHitGem->GetName() << FairLogger::endl;
*/
  }
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TRD   ----------------------------------
  if ( setup->IsActive(ECbmModuleId::kTrd) && !timebased && kFALSE ) {

    Double_t triggerThreshold = 0.5e-6;   // SIS100
    Bool_t   triangularPads = false;      // Bucharest triangular pad-plane layout
    CbmTrdClusterFinder* trdCluster = new CbmTrdClusterFinder();
    trdCluster->SetNeighbourEnable(true);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetNeighbourEnable(false);
    trdCluster->SetRowMerger(true);
    run->AddTask(trdCluster);
    std::cout << "-I- " << myName << ": Added task "
        << trdCluster->GetName() << std::endl;

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- " << myName << ": Added task "
        << trdHit->GetName() << std::endl;

  }
  // ------------------------------------------------------------------------
  // TOF defaults

	Int_t calMode=93;
	Int_t calSel=1;
	Int_t calSm=0;
	Int_t RefSel=0;
	Double_t dDeadtime=50.;
	Int_t iCalSet=30040500;
	Int_t iSel2=500;
	TString cCalId="MCdefault";

  // -----   Local reconstruction in TOF   ----------------------------------
  if ( setup->IsActive(ECbmModuleId::kTof) ) {
    switch(iTofCluMode) {
    case 1:
      {
	CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer",0,1);

	tofCluster->SetCalMode(calMode);
	tofCluster->SetCalSel(calSel);
	tofCluster->SetCaldXdYMax(3.);            // geometrical matching window in cm 
	tofCluster->SetCalCluMulMax(5.);        // Max Counter Cluster Multiplicity for filling calib histos  
	tofCluster->SetCalRpc(calSm);              // select detector for calibration update  
	tofCluster->SetTRefId(RefSel);              // reference trigger for offset calculation 
	tofCluster->SetTotMax(20.);                  // Tot upper limit for walk corection
	tofCluster->SetTotMin(0.01);                 //(12000.);  // Tot lower limit for walk correction
	tofCluster->SetTotPreRange(5.);           // effective lower Tot limit  in ns from peak position
	tofCluster->SetTotMean(5.);                  // Tot calibration target value in ns 
	tofCluster->SetMaxTimeDist(0.4);        // default cluster range in ns 
	tofCluster->SetDelTofMax(5.);            // acceptance range for cluster distance in ns (!)  
	tofCluster->SetSel2MulMax(3);           // limit Multiplicity in 2nd selector
	tofCluster->SetChannelDeadtime(dDeadtime);    // artificial deadtime in ns 
	tofCluster->SetEnableAvWalk(kFALSE);
	//tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
	tofCluster->SetYFitMin(1.E8);
	tofCluster->SetToDAv(0.04);
	tofCluster->SetIdMode(1);                    // calibrate on module level
	tofCluster->SetTRefDifMax(2.0);        // in ns 
	tofCluster->PosYMaxScal(0.75);        //in % of length
    TString cOutFname=OutDir + Form("/%s_set%09d_%02d_%01dtofClust.hst.root",RunId.Data(),iCalSet,calMode,calSel);
    tofCluster->SetOutHstFileName(cOutFname);
    
	Int_t iBRef=iCalSet%1000;
	Int_t iSet = (iCalSet - iBRef)/1000;
	Int_t iRSel=0;
	Int_t iRSelTyp=0;
	Int_t iRSelSm=0;
	Int_t iRSelRpc=0;

	iRSel=iBRef;     // use diamond
	if(iSel2==0){
	  // iSel2=iBRef;
	}else{
	  if(iSel2<0) iSel2=-iSel2;
	}

	Int_t iRSelin=iRSel;
	iRSelRpc=iRSel%10;
	iRSelTyp = (iRSel-iRSelRpc)/10;
	iRSelSm=iRSelTyp%10;
	iRSelTyp = (iRSelTyp-iRSelSm)/10;

	tofCluster->SetBeamRefId(iRSelTyp);    // define Beam reference counter 
	tofCluster->SetBeamRefSm(iRSelSm);
	tofCluster->SetBeamRefDet(iRSelRpc);
	tofCluster->SetBeamAddRefMul(-1);
	tofCluster->SetBeamRefMulMax(3);

	Int_t iSel2in=iSel2;
	Int_t iSel2Rpc= iSel2%10;
	iSel2=(iSel2-iSel2Rpc)/10;
	Int_t iSel2Sm=iSel2%10;
	iSel2=(iSel2-iSel2Sm)/10;
	if(iSel2 > -1) {
	  tofCluster->SetSel2Id(iSel2); 
	  tofCluster->SetSel2Sm(iSel2Sm); 
	  tofCluster->SetSel2Rpc(iSel2Rpc);
	}

	Int_t iRef = iSet %1000;
	Int_t iDut = (iSet - iRef)/1000;
	Int_t iDutRpc = iDut%10;
	iDut = (iDut - iDutRpc)/10;
	Int_t iDutSm = iDut%10;
	iDut = (iDut - iDutSm)/10;

	tofCluster->SetDutId(iDut);
	tofCluster->SetDutSm(iDutSm);
	tofCluster->SetDutRpc(iDutRpc);
	
	Int_t iRefRpc = iRef%10;
	iRef = (iRef - iRefRpc)/10;
	Int_t iRefSm = iRef%10;
	iRef = (iRef - iRefSm)/10;
	
	tofCluster->SetSelId(iRef);
	tofCluster->SetSelSm(iRefSm);
	tofCluster->SetSelRpc(iRefRpc);
	
	run->AddTask(tofCluster);
	std::cout << "-I- " << myName << ": Added task "
		  << tofCluster->GetName() << std::endl;   
      }
      break;
    default:
      {
	CbmTofSimpClusterizer* tofCluster
	  = new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
	tofCluster->SetOutputBranchPersistent("TofHit",          kTRUE);
	tofCluster->SetOutputBranchPersistent("TofDigiMatch",    kTRUE);
	run->AddTask(tofCluster);
	std::cout << "-I- " << myName << ": Added task "
		  << tofCluster->GetName() << std::endl;
      }
    }
  }
  // -------------------------------------------------------------------------

  
  // -----   Local reconstruction in RICH   ----------------------------------
  if ( setup->IsActive(ECbmModuleId::kRich) ) {
  /*
    CbmRichMCbmHitProducer *richHitProd = new CbmRichMCbmHitProducer();
    //richHitProd->setToTLimits(23.7,30.0);
    //richHitProd->applyToTCut();
    //richHitProd->DoRestrictToAcc();
    run->AddTask(richHitProd);
    cout << "-I- hitProducer: Added task " << richHitProd->GetName() << endl;

    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    richReco->UseMCbmSetup();
    run->AddTask(richReco);
    cout << "-I- richReco: Added task " << richReco->GetName() << endl;
*/
  }
  // -------------------------------------------------------------------------
  
  
  // -----   Track reconstruction   ------------------------------------------
  Double_t beamWidthX = 0.1;
  Double_t beamWidthY = 0.1;
  switch(iTrackMode) {
  case 2: 
    {
      Int_t iGenCor=1;
      Double_t dScalFac=1.;
      Double_t dChi2Lim2=3.5;
      TString cTrkFile=Form("%s_tofFindTracks.hst.root","MC");
      Int_t iTrackingSetup=1;

      CbmTofTrackFinder* tofTrackFinder= new CbmTofTrackFinderNN();
      tofTrackFinder->SetMaxTofTimeDifference(0.2);    // in ns/cm 
      tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);                 // max dev from mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);                 // mean slope dy/dz
      CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
      TFitter *MyFit = new TFitter(1);                // initialize Minuit
      tofTrackFinder->SetFitter(tofTrackFitter);
      CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
      tofFindTracks->UseFinder(tofTrackFinder);
      tofFindTracks->UseFitter(tofTrackFitter);
      tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2,3,4,5,6, 10 - 19
      tofFindTracks->SetTtTarg(0.041);                   // target value for inverse velocity, > 0.033 ns/cm!
      //tofFindTracks->SetTtTarg(0.035);                // target value for inverse velocity, > 0.033 ns/cm!
      tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name
      tofFindTracks->SetBeamCounter(5,0,0);         // default beam counter 
      tofFindTracks->SetStationMaxHMul(30);          // Max Hit Multiplicity in any used station
      
      tofFindTracks->SetT0MAX(dScalFac);            // in ns
      tofFindTracks->SetSIGT(0.08);                 // default in ns
      tofFindTracks->SetSIGX(0.3);                  // default in cm
      tofFindTracks->SetSIGY(0.45);                  // default in cm 
      tofFindTracks->SetSIGZ(0.05);                 // default in cm 
      tofFindTracks->SetUseSigCalib(kFALSE);        // ignore resolutions in CalPar file
      tofTrackFinder->SetSIGLIM(dChi2Lim2*2.);      // matching window in multiples of chi2
      tofTrackFinder->SetChiMaxAccept(dChi2Lim2);   // max tracklet chi2

      Int_t iMinNofHits=-1;
      Int_t iNStations=0;
      Int_t iNReqStations=3;
      switch (iTrackingSetup){
      case 0:            // bypass mode
	iMinNofHits=-1;   
	iNStations=1;        
	tofFindTracks->SetStation(0, 5, 0, 0);       // Diamond 
	break;
      case 1:                                           // for calibration mode of full setup
	iMinNofHits=3;
	iNStations=26;
	iNReqStations=3;
	tofFindTracks->SetStation(0, 5, 0, 0);          
	tofFindTracks->SetStation(1, 0, 2, 2);          
	tofFindTracks->SetStation(2, 0, 1, 2);
	tofFindTracks->SetStation(3, 0, 0, 2);         
	tofFindTracks->SetStation(4, 0, 2, 1);     
	tofFindTracks->SetStation(5, 0, 1, 1);
	tofFindTracks->SetStation(6, 0, 0, 1);         
	tofFindTracks->SetStation(7, 0, 2, 3);          
	tofFindTracks->SetStation(8, 0, 1, 3);
	tofFindTracks->SetStation(9, 0, 0, 3);         
	tofFindTracks->SetStation(10, 0, 2, 0);          
	tofFindTracks->SetStation(11, 0, 1, 0);
	tofFindTracks->SetStation(12, 0, 0, 0);         
	tofFindTracks->SetStation(13, 0, 2, 4);          
	tofFindTracks->SetStation(14, 0, 1, 4);
	tofFindTracks->SetStation(15, 0, 0, 4);    
	tofFindTracks->SetStation(16, 0, 4, 0);          
	tofFindTracks->SetStation(17, 0, 3, 0);
	tofFindTracks->SetStation(18, 0, 4, 1);          
	tofFindTracks->SetStation(19, 0, 3, 1);
	tofFindTracks->SetStation(20, 0, 4, 2);          
	tofFindTracks->SetStation(21, 0, 3, 2);
	tofFindTracks->SetStation(22, 0, 4, 3);          
	tofFindTracks->SetStation(23, 0, 3, 3);
	tofFindTracks->SetStation(24, 0, 4, 4);          
	tofFindTracks->SetStation(25, 0, 3, 4);               
	break;
      }
      tofFindTracks->SetMinNofHits(iMinNofHits);
      tofFindTracks->SetNStations(iNStations);
      tofFindTracks->SetNReqStations(iNReqStations);
      tofFindTracks->PrintSetup();
      run->AddTask(tofFindTracks);
    }
    break;
  case 1:
    {
      CbmBinnedTrackerTask* trackerTask = new CbmBinnedTrackerTask(kTRUE,
								   beamWidthX,
								   beamWidthY);
      trackerTask->SetUse(ECbmModuleId::kTrd, kFALSE);
      run->AddTask(trackerTask);
    }
  case 0:
  default:
    ;
  }
  // ------------------------------------------------------------------------
  
  
  // =========================================================================
  // ===                               Your QA                             ===
  // =========================================================================
  
    //CbmRichMCbmQaReal* mRichQa = new CbmRichMCbmQaReal();
    //mRichQa->SetOutputDir(string(resultDir));
    //run->AddTask(mRichQa);
  // =========================================================================
    
    
    
  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
//  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
//  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
//  rtdb->setSecondInput(parIo2);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Database update   ----------------------------------------------
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << recFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
  		      << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------
  // save all historgrams 
  gROOT->LoadMacro("save_hst.C");
  TString FSave=Form("save_hst(\"%s.reco_hst.root\")",dataset.Data());
  gInterpreter->ProcessLine(FSave.Data());
  // -----   Resource monitoring   ------------------------------------------
  if ( hasFairMonitor /*Has_Fair_Monitor()*/ ) {      // FairRoot Version >= 15.11
    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory=sysInfo.GetMaxMemory();
    std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    std::cout << maxMemory;
    std::cout << "</DartMeasurement>" << std::endl;

    Float_t cpuUsage=ctime/rtime;
    std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    std::cout << cpuUsage;
    std::cout << "</DartMeasurement>" << std::endl;

    FairMonitor* tempMon = FairMonitor::GetMonitor();
    tempMon->Print();
  }

 // RemoveGeoManager();
}
