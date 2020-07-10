//#include <FairParIo.h>
#include <FairRuntimeDb.h>

void build_correlations_tof(
  const string& digiFile = "/home/nikolay/FairRoot/cbmroot_trunk/macro/"
                           "beamtime/mcbm2020/data/unp_mcbm_582.root",
  const string& parFile = "/home/nikolay/FairRoot/cbmroot_trunk/macro/beamtime/"
                          "mcbm2020/data/unp_mcbm_params_582.root",
  const string& outDir =
    "/home/nikolay/FairRoot/cbmroot_trunk/macro/psd/Results/",
  const string& outFile     = "reco_mcbm_mar20.root",
  const unsigned int uRunId = 582,  // used for the output folder
  int nEvents               = 100) {

  // ==========================   Adjustments   =============================

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  TTree::SetMaxTreeSize(90000000000);

  TString srcDir  = gSystem->Getenv("VMCWORKDIR");  // top source directory
  TString workDir = gSystem->Getenv("VMCWORKDIR");

  std::cout << std::endl << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();

  const Double_t eb_fixedTimeWindow {200.};
  const Int_t eb_TriggerMinNumberT0 {0};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {0};
  const Int_t eb_TriggerMinNumberPsd {1};

  //ToF Settings
  Int_t calMode        = 93;
  Int_t calSel         = 1;
  Int_t calSelRead     = 0;
  Int_t calSm          = 40;
  Int_t RefSel         = 1;
  TString cFileId      = "385.100.-192.0";
  Int_t iCalSet        = 900041901;
  Int_t iTrackingSetup = 1;
  Int_t iSel2          = 0;
  Double_t dDeadtime   = 50;

  //void save_hst(TString cstr, Bool_t bROOT);

  //TString TofFileFolder = Form("/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2019/%s",cFileId.Data());
  TString TofFileFolder =
    Form("/home/nikolay/FairRoot/cbmroot_trunk/macro/beamtime/mcbm2019/%s",
         cFileId.Data());

  TString TofGeo = "v19b_mcbm";
  TObjString* tofDigiFile =
    new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par");
  std::cout << std::endl
            << "-I- digi.par file : " << workDir << "/parameters/tof/tof_"
            << TofGeo << ".digi.par" << std::endl;
  parFileList->Add(tofDigiFile);

  TObjString* tofDigiBdfFile =
    new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digibdf.par");
  std::cout << std::endl
            << "-I- digibdf.par file : " << workDir << "/parameters/tof/tof_"
            << TofGeo << ".digibdf.par" << std::endl;
  parFileList->Add(tofDigiBdfFile);

  TString geoFile =
    workDir
    + "/macro/mcbm/data/mcbm_beam_2019_12.geo.root";  //has to be generated
  TFile* fgeo         = new TFile(geoFile);
  TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
  if (NULL == geoMan) {
    cout << "<E> FAIRGeom not found in geoFile" << endl;
    return;
  }
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----  Analysis run   --------------------------------------------------
  FairRunAna* fRun = new FairRunAna();
  fRun->SetEventHeaderPersistence(kFALSE);
  FairFileSource* inputSource = new FairFileSource(digiFile);
  fRun->SetSource(inputSource);
  TString runId       = TString::Format("%03u", uRunId);
  TString QaDir       = outDir + "result_run" + runId;
  TString outFileName = QaDir + "/events_" + runId + "_" + outFile;
  //remove(outFileName.c_str());
  gSystem->Exec(Form("mkdir -p %s", QaDir.Data()));
  FairRootFileSink* outputSink = new FairRootFileSink(outFileName);
  fRun->SetSink(outputSink);
  //fRun->SetGenerateRunInfo(kTRUE);
  // ------------------------------------------------------------------------

  // Define output file for FairMonitor histograms
  FairMonitor::GetMonitor()->EnableMonitor(kFALSE);
  // ------------------------------------------------------------------------


  // -----  Event builder   -------------------------------------------------
  CbmMcbm2018EventBuilder* eventBuilder = new CbmMcbm2018EventBuilder();
  eventBuilder->SetEventBuilderAlgo(EventBuilderAlgo::FixedTimeWindow);
  eventBuilder->SetFixedTimeWindow(eb_fixedTimeWindow);
  eventBuilder->SetTriggerMinNumberT0(eb_TriggerMinNumberT0);
  eventBuilder->SetTriggerMinNumberSts(eb_TriggerMinNumberSts);
  eventBuilder->SetTriggerMinNumberMuch(eb_TriggerMinNumberMuch);
  eventBuilder->SetTriggerMinNumberTof(eb_TriggerMinNumberTof);
  eventBuilder->SetTriggerMinNumberPsd(eb_TriggerMinNumberPsd);
  fRun->AddTask(eventBuilder);
  // ------------------------------------------------------------------------


  // -----  Psd hit producer   ----------------------------------------------
  CbmPsdMCbmHitProducer* hitProd = new CbmPsdMCbmHitProducer();
  fRun->AddTask(hitProd);
  // ------------------------------------------------------------------------


  // Starting with ToF

  CbmTofEventClusterizer* tofClust =
    new CbmTofEventClusterizer("TOF Event Clusterizer", 1, 0);
  tofClust->SetCalMode(calMode);
  tofClust->SetCalSel(calSel);
  tofClust->SetCaldXdYMax(10.);  // geometrical matching window in cm
  tofClust->SetCalCluMulMax(
    3.);  // Max Counter Cluster Multiplicity for filling calib histos
  tofClust->SetCalRpc(calSm);   // select detector for calibration update
  tofClust->SetTRefId(RefSel);  // reference trigger for offset calculation
  tofClust->SetTotMax(20.);     // Tot upper limit for walk corection
  tofClust->SetTotMin(
    0.01);  //(12000.);  // Tot lower limit for walk correction
  tofClust->SetTotPreRange(
    5.);  // effective lower Tot limit  in ns from peak position
  tofClust->SetTotMean(5.);       // Tot calibration target value in ns
  tofClust->SetMaxTimeDist(1.0);  // default cluster range in ns
  tofClust->SetDelTofMax(
    10.);  // acceptance range for cluster distance in ns (!)
  tofClust->SetSel2MulMax(3);  // limit Multiplicity in 2nd selector
  tofClust->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
  tofClust->SetEnableAvWalk(kFALSE);
  //tofClust->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
  tofClust->SetYFitMin(1.E4);
  tofClust->SetIdMode(1);  // calibrate on module level
  //tofClust->SetDeadStrips(15,23);   // declare dead strip for T0M3,Rpc0,Strip 23


  TString cCalibFname = Form("/%s_set%09d_%02d_%01dtofClust.hst.root",
                             cFileId.Data(),
                             iCalSet,
                             calMode,
                             calSelRead);
  tofClust->SetCalParFileName(TofFileFolder + cCalibFname);
  TString cOutFname =
    Form("tofClust_%s_set%09d.hst.root", cFileId.Data(), iCalSet);
  tofClust->SetOutHstFileName(cOutFname);

  switch (calMode) {
    case 93:
      tofClust->SetTRefDifMax(1.5);  // in ns
      tofClust->PosYMaxScal(0.75);   //in % of length
      break;
    default:
      cout << "<E> Calib mode not implemented! stop execution of script"
           << endl;
      return;
  }

  fRun->AddTask(tofClust);

  Int_t iBRef    = iCalSet % 1000;
  Int_t iSet     = (iCalSet - iBRef) / 1000;
  Int_t iRSel    = 0;
  Int_t iRSelTyp = 0;
  Int_t iRSelSm  = 0;
  Int_t iRSelRpc = 0;
  iRSel          = iBRef;  // use diamond
  if (iSel2 == 0) {
    // iSel2=iBRef;
  } else {
    if (iSel2 < 0) iSel2 = -iSel2;
  }

  Int_t iRSelin = iRSel;
  iRSelRpc      = iRSel % 10;
  iRSelTyp      = (iRSel - iRSelRpc) / 10;
  iRSelSm       = iRSelTyp % 10;
  iRSelTyp      = (iRSelTyp - iRSelSm) / 10;

  tofClust->SetBeamRefId(iRSelTyp);  // define Beam reference counter
  tofClust->SetBeamRefSm(iRSelSm);
  tofClust->SetBeamRefDet(iRSelRpc);
  tofClust->SetBeamAddRefMul(-1);
  tofClust->SetBeamRefMulMax(3);

  Int_t iSel2in  = iSel2;
  Int_t iSel2Rpc = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Rpc) / 10;
  Int_t iSel2Sm  = iSel2 % 10;
  iSel2          = (iSel2 - iSel2Sm) / 10;
  if (iSel2 > 0) {
    tofClust->SetSel2Id(iSel2);
    tofClust->SetSel2Sm(iSel2Sm);
    tofClust->SetSel2Rpc(iSel2Rpc);
  }

  Int_t iRef    = iSet % 1000;
  Int_t iDut    = (iSet - iRef) / 1000;
  Int_t iDutRpc = iDut % 10;
  iDut          = (iDut - iDutRpc) / 10;
  Int_t iDutSm  = iDut % 10;
  iDut          = (iDut - iDutSm) / 10;

  tofClust->SetDutId(iDut);
  tofClust->SetDutSm(iDutSm);
  tofClust->SetDutRpc(iDutRpc);

  Int_t iRefRpc = iRef % 10;
  iRef          = (iRef - iRefRpc) / 10;
  Int_t iRefSm  = iRef % 10;
  iRef          = (iRef - iRefSm) / 10;

  tofClust->SetSelId(iRef);
  tofClust->SetSelSm(iRefSm);
  tofClust->SetSelRpc(iRefRpc);

  cout << "Run mTof Clusterizer with iRSel = " << iRSel
       << ", iSel2 = " << iSel2in << endl;


  //----------------------------------------------------------------------

  // mTof Tracker Initialization
  TString cTrkFile = Form("/%s_tofFindTracks.hst.root", cFileId.Data());
  CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
  tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
  tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
  tofTrackFinder->SetTyLIM(0.3);  // max dev from mean slope dy/dz
  tofTrackFinder->SetTyMean(0.);  // mean slope dy/dz

  CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
  TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
  tofTrackFinder->SetFitter(tofTrackFitter);
  CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
  tofFindTracks->UseFinder(tofTrackFinder);
  tofFindTracks->UseFitter(tofTrackFitter);
  Int_t iGenCor = 1;
  tofFindTracks->SetCorMode(iGenCor);  // valid options: 0,1,2,3,4,5,6, 10 - 19
  tofFindTracks->SetTtTarg(
    0.057);  // target value for inverse velocity, > 0.033 ns/cm!
  //tofFindTracks->SetTtTarg(0.035);                // target value for inverse velocity, > 0.033 ns/cm!
  tofFindTracks->SetCalParFileName(
    TofFileFolder + cTrkFile);  // Tracker parameter value file name
  std::cout << "TrackCalParFile: " << TofFileFolder << cTrkFile << std::endl;
  tofFindTracks->SetBeamCounter(5, 0, 0);  // default beam counter
  tofFindTracks->SetStationMaxHMul(
    30);  // Max Hit Multiplicity in any used station

  tofFindTracks->SetT0MAX(1.);            // in ns
  tofFindTracks->SetSIGT(0.08);           // default in ns
  tofFindTracks->SetSIGX(0.3);            // default in cm
  tofFindTracks->SetSIGY(0.6);            // default in cm
  tofFindTracks->SetSIGZ(0.05);           // default in cm
  tofFindTracks->SetUseSigCalib(kFALSE);  // ignore resolutions in CalPar file
  Double_t dChi2Lim2 = 3.5;               //100;//3.5;
  tofTrackFinder->SetSIGLIM(dChi2Lim2
                            * 2.);  // matching window in multiples of chi2
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
      iNStations    = 39;
      iNReqStations = 4;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 0);
      tofFindTracks->SetStation(2, 0, 3, 0);
      tofFindTracks->SetStation(3, 0, 4, 1);
      tofFindTracks->SetStation(4, 0, 3, 1);
      tofFindTracks->SetStation(5, 0, 4, 2);
      tofFindTracks->SetStation(6, 0, 3, 2);
      tofFindTracks->SetStation(7, 0, 4, 3);
      tofFindTracks->SetStation(8, 0, 3, 3);
      tofFindTracks->SetStation(9, 0, 4, 4);
      tofFindTracks->SetStation(10, 0, 3, 4);
      tofFindTracks->SetStation(11, 9, 0, 0);
      tofFindTracks->SetStation(12, 9, 0, 1);
      tofFindTracks->SetStation(13, 7, 0, 0);
      tofFindTracks->SetStation(14, 6, 0, 0);
      tofFindTracks->SetStation(15, 6, 0, 1);
      tofFindTracks->SetStation(16, 8, 0, 0);
      tofFindTracks->SetStation(17, 8, 0, 1);
      tofFindTracks->SetStation(18, 8, 0, 2);
      tofFindTracks->SetStation(19, 8, 0, 3);
      tofFindTracks->SetStation(20, 8, 0, 4);
      tofFindTracks->SetStation(21, 8, 0, 5);
      tofFindTracks->SetStation(22, 8, 0, 6);
      tofFindTracks->SetStation(23, 8, 0, 7);

      tofFindTracks->SetStation(24, 0, 2, 2);
      tofFindTracks->SetStation(25, 0, 1, 2);
      tofFindTracks->SetStation(26, 0, 0, 2);
      tofFindTracks->SetStation(27, 0, 2, 1);
      tofFindTracks->SetStation(28, 0, 1, 1);
      tofFindTracks->SetStation(29, 0, 0, 1);
      tofFindTracks->SetStation(30, 0, 2, 3);
      tofFindTracks->SetStation(31, 0, 1, 3);
      tofFindTracks->SetStation(32, 0, 0, 3);
      tofFindTracks->SetStation(33, 0, 2, 0);
      tofFindTracks->SetStation(34, 0, 1, 0);
      tofFindTracks->SetStation(35, 0, 0, 0);
      tofFindTracks->SetStation(36, 0, 2, 4);
      tofFindTracks->SetStation(37, 0, 1, 4);
      tofFindTracks->SetStation(38, 0, 0, 4);
      break;

    case 2:
      iMinNofHits   = 3;
      iNStations    = 14;
      iNReqStations = 5;
      tofFindTracks->SetStation(0, 5, 0, 0);
      tofFindTracks->SetStation(1, 0, 4, 1);
      tofFindTracks->SetStation(2, 0, 3, 1);
      tofFindTracks->SetStation(3, 0, 4, 0);
      tofFindTracks->SetStation(4, 0, 3, 0);
      tofFindTracks->SetStation(5, 0, 4, 2);
      tofFindTracks->SetStation(6, 0, 3, 2);
      tofFindTracks->SetStation(7, 0, 4, 3);
      tofFindTracks->SetStation(8, 0, 3, 3);
      tofFindTracks->SetStation(9, 0, 4, 4);
      tofFindTracks->SetStation(10, 0, 3, 4);
      tofFindTracks->SetStation(11, 9, 0, 0);
      tofFindTracks->SetStation(12, 9, 0, 1);
      tofFindTracks->SetStation(13, 7, 0, 0);
      break;

    default:;
  }

  tofFindTracks->SetMinNofHits(iMinNofHits);
  tofFindTracks->SetNStations(iNStations);
  tofFindTracks->SetNReqStations(iNReqStations);
  tofFindTracks->PrintSetup();
  fRun->AddTask(tofFindTracks);

  //######################################################################


  CbmPsdMCbmQaReal* qaTask = new CbmPsdMCbmQaReal();
  qaTask->SetOutputDir(QaDir.Data());
  fRun->AddTask(qaTask);


  std::cout << std::endl
            << std::endl
            << "-I- "
            << ": Set runtime DB" << std::endl;

  FairRuntimeDb* rtdb        = fRun->GetRuntimeDb();
  Bool_t kParameterMerged    = kTRUE;
  FairParRootFileIo* parIo1  = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.c_str(), "UPDATE");
  parIo1->print();


  parIo2->open(parFileList, "in");
  parIo2->print();
  rtdb->setFirstInput(parIo2);
  rtdb->setSecondInput(parIo1);


  // -----   Intialise and run   --------------------------------------------
  std::cout << std::endl
            << "-I- "
            << ": Initialise run" << std::endl;
  fRun->Init();

  //  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  rtdb->printParamContexts();

  cout << "Starting run" << endl;
  if (0 == nEvents) {
    fRun->Run(0, 0);  // run until end of input file
  } else {
    fRun->Run(0, nEvents);  // process  2000 Events
  }
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  cout << maxMemory;
  cout << "</DartMeasurement>" << endl;

  Float_t cpuUsage = ctime / rtime;
  cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  cout << cpuUsage;
  cout << "</DartMeasurement>" << endl;

  FairMonitor* tempMon = FairMonitor::GetMonitor();
  tempMon->Print();

  //  RemoveGeoManager();
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
