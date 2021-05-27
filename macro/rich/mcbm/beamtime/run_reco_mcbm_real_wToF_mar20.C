/** @file run_reco_mcbm_real_wToF_mar20.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void run_reco_mcbm_real_wToF_mar20(const string srcfolder = "/lustre/cbm/users/adrian/mcbmbeamtime/cbmsource/"
                                                            "macro/beamtime/mcbm2020/data",
                                   const unsigned int runId = 831,  // used for the output folder
                                   int nEvents = 5000, const int taskId = 3)
{
  // -----   File names   --------------------------------------------------
  const string& parFile  = Form("%s/unp_mcbm_params_%d.root", srcfolder.c_str(), runId);
  const string& digiFile = Form("%s/unp_mcbm_%d.root", srcfolder.c_str(), runId);
  const string& recoFile = Form("reco_mcbm_mar20_%d.root", runId);
  TString setup          = "mcbm_beam_2020_03";
  // -----------------------------------------------------------------------


  // -----   EventBuilder Settings-----------------------------------------
  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {1};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {10};
  const Int_t eb_TriggerMinNumberRich {5};
  // -----------------------------------------------------------------------


  // -----   TOF defaults --------------------------------------------------
  Int_t calMode      = 93;
  Int_t calSel       = 1;
  Int_t calSm        = 0;
  Int_t RefSel       = 0;
  Double_t dDeadtime = 50.;
  Int_t iSel2        = 500;
  // -----------------------------------------------------------------------


  // -----   TOF Settings --------------------------------------------------
  TString cCalId = "490.100.5.0";
  if (runId >= 759) cCalId = "759.100.4.0";
  if (runId >= 812) cCalId = "831.100.4.0";

  Int_t iCalSet = 30040500;  // calibration settings
  if (runId >= 759) iCalSet = 10020500;
  if (runId >= 812) iCalSet = 10020500;

  Double_t Tint           = 100.;  // coincidence time interval
  Int_t iTrackMode        = 2;     // 2 for TofTracker
  const Int_t iTofCluMode = 1;
  // -----------------------------------------------------------------------


  // -----   Fair logger ---------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  TTree::SetMaxTreeSize(90000000000);
  // -----------------------------------------------------------------------


  TString myName  = "run_reco_mcbm_real";
  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  remove(recoFile.c_str());

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup* pSetup = CbmSetup::Instance();
  pSetup->LoadSetup(setup);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  //CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  pSetup->SetActive(ECbmModuleId::kMvd, kFALSE);
  pSetup->SetActive(ECbmModuleId::kSts, kFALSE);
  pSetup->SetActive(ECbmModuleId::kMuch, kFALSE);
  pSetup->SetActive(ECbmModuleId::kRich, kTRUE);
  pSetup->SetActive(ECbmModuleId::kTrd, kFALSE);
  pSetup->SetActive(ECbmModuleId::kPsd, kFALSE);
  // -----------------------------------------------------------------------


  //TString TofFileFolder = Form("/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/%s",cFileId.Data());
  //    TString TofFileFolder = Form("/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2019/%s",cFileId.Data());
  TString TofFileFolder = Form("/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/%s", cCalId.Data());
  //    TString setupFile = srcDir + "/geometry/setup/setup_" + geoSetup + ".C";
  //    TString setupFunct = "setup_" + geoSetup + "()";
  //    gROOT->LoadMacro(setupFile);
  //    gROOT->ProcessLine(setupFunct);

  std::cout << std::endl << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();


  //-----------------------------------------------//
  //   TString FId    = cCalId;
  //   TString TofGeo = "v19b_mcbm"; //v18m_mCbm
  //   TString TofGeo = "v20a_mcbm"; //v18m_mCbm


  // ----- TOF digitisation parameters -------------------------------------
  TString geoTag;
  TString geoFile;
  if (pSetup->IsActive(ECbmModuleId::kTof)) {
    pSetup->GetGeoTag(ECbmModuleId::kTof, geoTag);
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;

    //TString geoFile = srcDir + "/geometry/tof/geofile_tof_" + geoTag + ".root";
    geoFile             = srcDir + "/macro/mcbm/data/mcbm_beam_2020_03.geo.root";
    TFile* fgeo         = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile " << geoFile.Data() << endl;
      return;
    }
  }

  // ------------------------------------------------------------------------

  //   TObjString *tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"); // TOF digi file
  //   std::cout << std::endl<< "-I- digi.par file : "<<workDir << "/parameters/tof/tof_" << TofGeo << ".digi.par"<< std::endl;
  //   parFileList->Add(tofDigiFile);
  //
  //   TObjString *tofDigiBdfFile = new TObjString( workDir  + "/parameters/tof/tof_" + TofGeo +".digibdf.par");
  //   std::cout << std::endl<< "-I- digibdf.par file : "<<workDir << "/parameters/tof/tof_" << TofGeo << ".digibdf.par"<< std::endl;
  //   parFileList->Add(tofDigiBdfFile);

  //    TString geoDir  = workDir;  // gSystem->Getenv("VMCWORKDIR");
  //    //TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
  //    //TString geoFile = "/lustre/nyx/cbm/users/adrian/data/sim/rich/mcbm/sis18_mcbm_25deg_long_geofile_full.root"; //18m
  // //   TString geoFile = srcDir + "/macro/mcbm/data/mcbm_beam_2019_11.geo.root";
  //    TString geoFile = srcDir + "/macro/mcbm/data/mcbm_beam_2020_03.geo.root";
  //    TFile* fgeo = new TFile(geoFile);
  //    TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  //    if (NULL == geoMan){
  //      cout << "<E> FAIRGeom not found in geoFile"<<endl;
  //      return;
  //    }


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  gROOT->LoadMacro("save_hst.C");


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << digiFile << std::endl;
  // ------------------------------------------------------------------------

  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(digiFile.c_str());
  run->SetSource(inputSource);
  run->SetOutputFile(recoFile.c_str());
  //     run->SetGenerateRunInfo(kFALSE);
  //if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);


  // -----   Cbm EventBuilder   ---------------------------------------------------
  CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  //	eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::MaximumTimeGap);
  eventBuilder->SetMaximumTimeGap(200.);
  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  eventBuilder->SetFixedTimeWindow(eb_fixedTimeWindow);
  eventBuilder->SetTriggerMinNumberT0(eb_TriggerMinNumberT0);
  eventBuilder->SetTriggerMinNumberSts(eb_TriggerMinNumberSts);
  eventBuilder->SetTriggerMinNumberMuch(eb_TriggerMinNumberMuch);
  eventBuilder->SetTriggerMinNumberTof(eb_TriggerMinNumberTof);
  eventBuilder->SetTriggerMinNumberRich(eb_TriggerMinNumberRich);
  eventBuilder->SetFillHistos(kFALSE);  // to prevent memory leak???

  run->AddTask(eventBuilder);


  // -----   Local reconstruction of RICH Hits ---------------
  CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
  hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
  hitProd->setToTLimits(23.7, 30.0);
  hitProd->applyToTCut();
  run->AddTask(hitProd);
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in RICh -> Finding of Rings ---------------
  CbmRichReconstruction* richReco = new CbmRichReconstruction();
  richReco->UseMCbmSetup();
  run->AddTask(richReco);
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in TOF   ----------------------------------
  TString cFname;
  switch (iTofCluMode) {
    case 1: {
      CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer", 0, 1);
      //		cFname=Form("/%s_set%09d_%02d_%01dtofClust.hst.root",cCalId.Data(),iCalSet,calMode,calSel);
      cFname = Form("/%s_set%09d_%02d_%01d_noWalk_tofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSel);
      tofCluster->SetCalParFileName(TofFileFolder + cFname);
      tofCluster->SetCalMode(calMode);
      tofCluster->SetCalSel(calSel);
      tofCluster->SetCaldXdYMax(3.);              // geometrical matching window in cm
      tofCluster->SetCalCluMulMax(5.);            // Max Counter Cluster Multiplicity for filling calib histos
      tofCluster->SetCalRpc(calSm);               // select detector for calibration update
      tofCluster->SetTRefId(RefSel);              // reference trigger for offset calculation
      tofCluster->SetTotMax(20.);                 // Tot upper limit for walk corection
      tofCluster->SetTotMin(0.01);                //(12000.);  // Tot lower limit for walk correction
      tofCluster->SetTotPreRange(5.);             // effective lower Tot limit  in ns from peak position
      tofCluster->SetTotMean(5.);                 // Tot calibration target value in ns
      tofCluster->SetMaxTimeDist(1.0);            // default cluster range in ns
      tofCluster->SetDelTofMax(15.);              // acceptance range for cluster distance in ns (!)
      tofCluster->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
      tofCluster->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
      tofCluster->SetEnableAvWalk(kFALSE);
      //tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
      tofCluster->SetYFitMin(1.E4);
      tofCluster->SetToDAv(0.04);
      tofCluster->SetIdMode(1);        // calibrate on module level
      tofCluster->SetTRefDifMax(2.0);  // in ns
      tofCluster->PosYMaxScal(0.75);   //in % of length
      Int_t iBRef    = iCalSet % 1000;
      Int_t iSet     = (iCalSet - iBRef) / 1000;
      Int_t iRSel    = 0;
      Int_t iRSelTyp = 0;
      Int_t iRSelSm  = 0;
      Int_t iRSelRpc = 0;
      iRSel          = iBRef;  // use diamond
      Int_t iRSelin  = iRSel;
      iRSelRpc       = iRSel % 10;
      iRSelTyp       = (iRSel - iRSelRpc) / 10;
      iRSelSm        = iRSelTyp % 10;
      iRSelTyp       = (iRSelTyp - iRSelSm) / 10;
      tofCluster->SetBeamRefId(iRSelTyp);  // define Beam reference counter
      tofCluster->SetBeamRefSm(iRSelSm);
      tofCluster->SetBeamRefDet(iRSelRpc);
      tofCluster->SetBeamAddRefMul(-1);
      tofCluster->SetBeamRefMulMax(3);
      Int_t iSel2in  = iSel2;
      Int_t iSel2Rpc = iSel2 % 10;
      iSel2          = (iSel2 - iSel2Rpc) / 10;
      Int_t iSel2Sm  = iSel2 % 10;
      iSel2          = (iSel2 - iSel2Sm) / 10;
      if (iSel2 > -1) {
        tofCluster->SetSel2Id(iSel2);
        tofCluster->SetSel2Sm(iSel2Sm);
        tofCluster->SetSel2Rpc(iSel2Rpc);
      }
      Int_t iRef    = iSet % 1000;
      Int_t iDut    = (iSet - iRef) / 1000;
      Int_t iDutRpc = iDut % 10;
      iDut          = (iDut - iDutRpc) / 10;
      Int_t iDutSm  = iDut % 10;
      iDut          = (iDut - iDutSm) / 10;
      tofCluster->SetDutId(iDut);
      tofCluster->SetDutSm(iDutSm);
      tofCluster->SetDutRpc(iDutRpc);

      Int_t iRefRpc = iRef % 10;
      iRef          = (iRef - iRefRpc) / 10;
      Int_t iRefSm  = iRef % 10;
      iRef          = (iRef - iRefSm) / 10;

      tofCluster->SetSelId(iRef);
      tofCluster->SetSelSm(iRefSm);
      tofCluster->SetSelRpc(iRefRpc);

      run->AddTask(tofCluster);
      std::cout << "-I- " << myName << ": Added task " << tofCluster->GetName() << std::endl;
    } break;

    default: {
      ;
    }
  }
  // -------------------------------------------------------------------------


  // -----   Track reconstruction   ------------------------------------------
  Double_t beamWidthX = 0.1;
  Double_t beamWidthY = 0.1;
  switch (iTrackMode) {
    case 2: {
      Int_t iGenCor                     = 1;
      Double_t dScalFac                 = 1.;
      Double_t dChi2Lim2                = 3.5;
      TString cTrkFile                  = Form("/%s_tofFindTracks.hst.root", cCalId.Data());
      Int_t iTrackingSetup              = 1;
      CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
      tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
      tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
      tofTrackFinder->SetTyLIM(0.3);                 // max dev from mean slope dy/dz
      tofTrackFinder->SetTyMean(0.);                 // mean slope dy/dz
      CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
      TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
      tofTrackFinder->SetFitter(tofTrackFitter);
      CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
      tofFindTracks->UseFinder(tofTrackFinder);
      tofFindTracks->UseFitter(tofTrackFitter);
      tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
      tofFindTracks->SetTtTarg(0.041);     // target value for inverse velocity, > 0.033 ns/cm!
      //tofFindTracks->SetTtTarg(0.035);            // target value for inverse velocity, > 0.033 ns/cm!
      tofFindTracks->SetCalParFileName(TofFileFolder + cTrkFile);  // Tracker parameter value file name
      tofFindTracks->SetBeamCounter(5, 0, 0);                      // default beam counter
      tofFindTracks->SetStationMaxHMul(30);                        // Max Hit Multiplicity in any used station


      tofFindTracks->SetT0MAX(dScalFac);           // in ns
      tofFindTracks->SetSIGT(0.08);                // default in ns
      tofFindTracks->SetSIGX(0.3);                 // default in cm
      tofFindTracks->SetSIGY(0.45);                // default in cm
      tofFindTracks->SetSIGZ(0.05);                // default in cm
      tofFindTracks->SetUseSigCalib(kFALSE);       // ignore resolutions in CalPar file
      tofTrackFinder->SetSIGLIM(dChi2Lim2 * 2.);   // matching window in multiples of chi2
      tofTrackFinder->SetChiMaxAccept(dChi2Lim2);  // max tracklet chi2

      Int_t iMinNofHits   = -1;
      Int_t iNStations    = 0;
      Int_t iNReqStations = 3;

      switch (iTrackingSetup) {
        case 0:  // bypass mode
          iMinNofHits = -1;
          iNStations  = 1;
          tofFindTracks->SetStation(0, 5, 0, 0);  // Diamond
          break;

        case 1:  // for calibration mode of full setup
          iMinNofHits   = 3;
          iNStations    = 30;
          iNReqStations = 4;
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
    } break;

    case 1: {
    }

    case 0:

    default:;
  }
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===                           RICH QA                                 ===
  // =========================================================================

  CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
  if (taskId < 0) { qaTask->SetOutputDir(Form("result_run%d", runId)); }
  else {
    qaTask->SetOutputDir(Form("result_run%d_%05d", runId, taskId));
  }
  //    qaTask->DoRestrictToAcc();//restrict to mRICH MAR2019 in histFilling
  //    qaTask->XOffsetHistos(12);
  qaTask->XOffsetHistos(+2.5);
  qaTask->SetMaxNofDrawnEvents(100);
  qaTask->SetTotRich(23.7, 30.0);
  qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  qaTask->SetTriggerTofHits(eb_TriggerMinNumberTof);
  run->AddTask(qaTask);
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  Bool_t kParameterMerged    = kTRUE;
  FairParRootFileIo* parIo1  = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  parIo2->open(parFileList, "in");
  parIo1->print();
  parIo2->print();
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Database update   ----------------------------------------------
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  rtdb->printParamContexts();
  // ------------------------------------------------------------------------


  //--- House Keeping -------------------------------------------------------
  // print all important infos in a file
  std::ofstream outfile;
  if (taskId < 0) { outfile.open(Form("result_run%d/run_info.dat", runId)); }
  else {
    outfile.open(Form("result_run%d_%05d/run_info.dat", runId, taskId));
  }
  // write inputted data into the file.
  outfile << "Run: " << runId << std::endl;
  outfile << "Events: " << nEvents << std::endl;
  outfile << "parFile: " << parFile << std::endl;
  outfile << "digiFile: " << digiFile << std::endl;
  outfile << "recoFile: " << recoFile << std::endl;
  outfile << "Geometry: " << geoFile << std::endl;
  outfile << "TrackCalParFile: " << TofFileFolder << cCalId << std::endl;
  outfile << "TofClusterFile :" << TofFileFolder + cFname << std::endl;
  outfile << "TofOutput :" << cFname << std::endl << std::endl;
  outfile << "Trigger:" << std::endl;
  outfile << "  fixedTimeWindow :" << eb_fixedTimeWindow << std::endl;
  outfile << "  MinNumberT0   :" << eb_TriggerMinNumberT0 << std::endl;
  outfile << "  MinNumberSts  :" << eb_TriggerMinNumberSts << std::endl;
  outfile << "  MinNumberMuch :" << eb_TriggerMinNumberMuch << std::endl;
  outfile << "  MinNumberTof  :" << eb_TriggerMinNumberTof << std::endl;
  outfile << "  MinNumberRich :" << eb_TriggerMinNumberRich << std::endl;
  outfile.close();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << recoFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;

  // -----   Resource monitoring   ------------------------------------------
  //     if ( hasFairMonitor /*Has_Fair_Monitor()*/ ) {      // FairRoot Version >= 15.11
  //         // Extract the maximal used memory an add is as Dart measurement
  //         // This line is filtered by CTest and the value send to CDash
  //         FairSystemInfo sysInfo;
  //         Float_t maxMemory=sysInfo.GetMaxMemory();
  //         std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  //         std::cout << maxMemory;
  //         std::cout << "</DartMeasurement>" << std::endl;
  //
  //         Float_t cpuUsage=ctime/rtime;
  //         std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  //         std::cout << cpuUsage;
  //         std::cout << "</DartMeasurement>" << std::endl;
  //
  //         FairMonitor* tempMon = FairMonitor::GetMonitor();
  //         tempMon->Print();
  //     }
}

void save_hst(TString cstr = "status.hst.root", Bool_t bROOT = kFALSE)
{
  cout << "save all histograms to file " << cstr.Data() << endl;
  TList* tList(NULL);
  if (bROOT) { tList = gROOT->GetList(); }
  else {
    tList = gDirectory->GetList();
  }
  TIter next(tList);
  // Write objects to the file
  TFile* fHist = new TFile(cstr, "RECREATE");
  {
    TObject* obj;
    while ((obj = (TObject*) next())) {
      if (obj->InheritsFrom(TH1::Class()) || obj->InheritsFrom(TEfficiency::Class())) { obj->Write(); }
    }
  }
  // fHist->ls();
  fHist->Close();
}
