//
// N.Herrmann   02.05.2020
//
// --------------------------------------------------------------------------

void mtof_reco(
    Int_t nEvents   = 100,  // number of Timeslices
    TString dataset ="data/unp_mcbm_759",
    TString setup   ="mcbm_beam_2020_03",
    TString cCalId  ="759.100.4.0",
	Int_t iCalSet   =10020500, // calibration settings
    Double_t ReqTofMul=2.,  // requested TOF digi multiplicity
    Double_t Tint   =100.,  // coincidence time interval
    Int_t    iTrackMode=0,  // 2 for TofTracker
    Double_t ReqT0Mul=0.        
)
{
  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "VERYHIGH";
  // ------------------------------------------------------------------------

  Int_t iTofCluMode=1;

  
  // -----   Environment   --------------------------------------------------
  TString myName = "mtof_reco";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  TString rawFile = dataset + ".root";
  TString parFile = dataset + ".par.root";
  TString recFile = dataset + Form(".%d.%d",(Int_t)Tint,(Int_t)ReqTofMul) + ".rec.root";
  TString hstFile = "./hst/" + dataset + Form(".%d.%d",(Int_t)Tint,(Int_t)ReqTofMul) + ".rec.hst.root";
  // ------------------------------------------------------------------------
  TString shcmd = "rm -v " + parFile;
  gSystem->Exec( shcmd.Data()  );
   
  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup *pSetup=CbmSetup::Instance();
  pSetup->LoadSetup(setup);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  //CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  pSetup->SetActive(ECbmModuleId::kMvd, kFALSE);
  pSetup->SetActive(ECbmModuleId::kSts, kFALSE);
  pSetup->SetActive(ECbmModuleId::kMuch, kFALSE);
  pSetup->SetActive(ECbmModuleId::kRich, kFALSE);
  pSetup->SetActive(ECbmModuleId::kTrd, kFALSE);
  pSetup->SetActive(ECbmModuleId::kPsd, kFALSE);

  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList *parFileList = new TList();
  TString geoTag;

  // - TOF digitisation parameters
  if ( pSetup->IsActive(ECbmModuleId::kTof) ) {
    
    pSetup->GetGeoTag(ECbmModuleId::kTof, geoTag);

  	TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/" + geoTag + ".digibdf.par");
  	parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
    		      << tofBdfFile->GetString() << std::endl;
                  
    TString geoFile = srcDir + "/geometry/tof/geofile_tof_" + geoTag + ".root";
    TFile* fgeo = new TFile(geoFile);
    TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan){
     cout << "<E> FAIRGeom not found in geoFile "<< geoFile.Data() <<endl;
     return;
    }                 
                  
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
  run->SetOutputFile(recFile);
  run->SetGenerateRunInfo(kFALSE);
  Bool_t hasFairMonitor = kFALSE; //Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------
  
    CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
    //  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
    //  eventBuilder->SetMaximumTimeGap(100.);
    eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
    eventBuilder->SetFixedTimeWindow(Tint);
    eventBuilder->SetTriggerMinNumberT0(ReqT0Mul);
    eventBuilder->SetTriggerMinNumberSts(0);
    eventBuilder->SetTriggerMinNumberMuch(0);
    eventBuilder->SetTriggerMinNumberTof(ReqTofMul);
    eventBuilder->SetTriggerMinNumberRich(0);
    eventBuilder->SetFillHistos(kFALSE); // to prevent memory leak???
    
  run->AddTask(eventBuilder);

  // ------------------------------------------------------------------------
  // TOF defaults

	Int_t calMode=93;
	Int_t calSel=1;
	Int_t calSm=0;
	Int_t RefSel=0;
	Double_t dDeadtime=50.;
	Int_t iSel2=500;

  // -----   Local reconstruction in TOF   ----------------------------------
  if ( pSetup->IsActive(ECbmModuleId::kTof) ) {
    switch(iTofCluMode) {
    case 1:
      {
	CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer",0,1);

    TString cFname=Form("%s_set%09d_%02d_%01dtofClust.hst.root",cCalId.Data(),iCalSet,calMode,calSel);
    tofCluster->SetCalParFileName(cFname);
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
	tofCluster->SetMaxTimeDist(1.0);        // default cluster range in ns 
	tofCluster->SetDelTofMax(15.);            // acceptance range for cluster distance in ns (!)  
	tofCluster->SetSel2MulMax(3);             // limit Multiplicity in 2nd selector
	tofCluster->SetChannelDeadtime(dDeadtime);    // artificial deadtime in ns 
	tofCluster->SetEnableAvWalk(kFALSE);
	//tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
	tofCluster->SetYFitMin(1.E4);
	tofCluster->SetToDAv(0.04);
	tofCluster->SetIdMode(1);                    // calibrate on module level
	tofCluster->SetTRefDifMax(2.0);        // in ns 
	tofCluster->PosYMaxScal(0.75);        //in % of length

	Int_t iBRef=iCalSet%1000;
	Int_t iSet = (iCalSet - iBRef)/1000;
	Int_t iRSel=0;
	Int_t iRSelTyp=0;
	Int_t iRSelSm=0;
	Int_t iRSelRpc=0;

	iRSel=iBRef;     // use diamond
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
       ;
      }   
    }
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
      TString cTrkFile=Form("%s_tofFindTracks.hst.root",cCalId.Data());
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
	iNStations=30;
	iNReqStations=4;
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
	tofFindTracks->SetStation(26, 9, 0, 0);         
	tofFindTracks->SetStation(27, 9, 0, 1);         
	tofFindTracks->SetStation(28, 6, 0, 0);         
	tofFindTracks->SetStation(29, 6, 0, 1);              
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

    }
  case 0:
  default:
    ;
  }
  // ------------------------------------------------------------------------
  
  
  // =========================================================================
  // ===                               Your QA                             ===
  // =========================================================================

  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parIo1 = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
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
  // save all historgrams 
  gROOT->LoadMacro("save_hst.C");
  TString FSave=Form("save_hst(\"%s\")",hstFile.Data());
  gInterpreter->ProcessLine(FSave.Data());
  
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

}
