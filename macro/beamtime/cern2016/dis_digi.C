void dis_digi(Int_t nEvents = 100000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=300, Int_t RefSel=1, TString cFileId="MbsTrbThu1715", Int_t iCalSet=0, Bool_t bOut=0, Int_t iSel2=0, Double_t dDeadtime=50., Int_t iGenCor=1, Int_t iTrackingSetup=0,Double_t dScalFac=5., Double_t dChi2Lim2=50.) 
{
  Int_t iVerbose = 1;
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  //TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  //TString logLevel = "INFO";
  TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger* log;  

   TString workDir    = gSystem->Getenv("VMCWORKDIR");
   TString paramDir   = workDir + "/macro/beamtime/cern2016/";
   TString ParFile    = paramDir + "data/" + cFileId + ".params.root";
   TString InputFile  = paramDir + "data/" + cFileId + ".root";
   TString OutputFile = paramDir + "data/disdigi_"   + cFileId + Form("_%09d%03d",iCalSet,iSel2) + ".out.root";

   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId.Data());

   TList *parFileList = new TList();

   TString TofGeo="v16b";  //default

   cout << "Geometry version "<<TofGeo<<endl;

   TObjString *tofDigiFile = new TObjString(workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"); // TOF digi file
   parFileList->Add(tofDigiFile);   

   // TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   // TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
   TObjString *tofDigiBdfFile = new TObjString(workDir  + "/parameters/tof/" + TofGeo +".digibdf.par");
   parFileList->Add(tofDigiBdfFile);

   TString geoDir  = gSystem->Getenv("VMCWORKDIR");
   TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
   TFile* fgeo = new TFile(geoFile);
   TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
   if (NULL == geoMan){
     cout << "<E> FAIRGeom not found in geoFile"<<endl;
     return;
   }
   
   if(0){
   TGeoVolume* master=geoMan->GetTopVolume();
   master->SetVisContainers(1); 
   master->Draw("ogl"); 
   }

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(InputFile.Data());
  //run->AddFriend(InputFile.Data());
  run->SetOutputFile(OutputFile);
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, bOut);

   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(3.);         // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(20.);       // Max Counter Cluster Multiplicity for filling calib histos  
   tofTestBeamClust->SetCalRpc(calSm);           // select detector for calibration update  
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10.);             // Tot upper limit for walk corection
   tofTestBeamClust->SetTotMin(0.01);            //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(2.);         // effective lower Tot limit  in ns from peak position
   tofTestBeamClust->SetTotMean(2.);             // Tot calibration target value in ns 
   tofTestBeamClust->SetMaxTimeDist(0.5);        // default cluster range in ns 
   //tofTestBeamClust->SetMaxTimeDist(0.);       // Deb// default cluster range in ns 
   tofTestBeamClust->SetDelTofMax(3.);           // acceptance range for cluster correlation  
   tofTestBeamClust->SetBeamRefMulMax(10);       // limit Multiplicity in beam counter
   tofTestBeamClust->SetChannelDeadtime(dDeadtime);    // artificial deadtime in ns 
   tofTestBeamClust->SetMemoryTime(0.);//1000000.);    // internal storage time of hits in ns

   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_set%09d_%02d_%01dtofTestBeamClust.hst.root",cFileId.Data(),iCalSet,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s_set%09d.hst.root",cFileId.Data(),iCalSet);
   tofTestBeamClust->SetOutHstFileName(cOutFname);

   TString cAnaFile=Form("%s_%09d%03d_tofAnaTestBeam.hst.root",cFileId.Data(),iCalSet,iSel2);

   switch (calMode) {
   case 0:                                      // initial calibration 
     tofTestBeamClust->SetTotMax(100.);      // 100 ns 
     tofTestBeamClust->SetTotPreRange(100.);
    //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(1000.); // in ns 
     tofTestBeamClust->PosYMaxScal(1000.);      // in % of length 
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 1:                                       // save offsets, update walks, for diamonds 
     tofTestBeamClust->SetTRefDifMax(50.);    // in ns 
     tofTestBeamClust->PosYMaxScal(0.1);         // in % of length 
     break;
   case 11:
     tofTestBeamClust->SetTRefDifMax(4.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;   
   case 21:
     tofTestBeamClust->SetTRefDifMax(3.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 31:
     tofTestBeamClust->SetTRefDifMax(2.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 41:
     tofTestBeamClust->SetTRefDifMax(1.);    // in ns 
     tofTestBeamClust->PosYMaxScal(0.8);        // in % of length 
     break;   
   case 51:
     tofTestBeamClust->SetTRefDifMax(0.7);     // in ns 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;
   case 61:
     tofTestBeamClust->SetTRefDifMax(0.5);     // in ns 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;   
   case 71:
     tofTestBeamClust->SetTRefDifMax(0.4);     // in ns 
     tofTestBeamClust->PosYMaxScal(0.6);        // in % of length 
     break;

   case 2:                                      // time difference calibration
     tofTestBeamClust->SetTRefDifMax(300.);  // in ns 
     tofTestBeamClust->PosYMaxScal(1000.);      //in % of length
     break;

   case 3:                                       // time offsets 
     tofTestBeamClust->SetTRefDifMax(200.);   // in ns 
     tofTestBeamClust->PosYMaxScal(100.);       //in % of length
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 12:
   case 13:
     tofTestBeamClust->SetTRefDifMax(100.);   // in ns 
     tofTestBeamClust->PosYMaxScal(50.);        //in % of length
     break;
   case 22:
   case 23:
     tofTestBeamClust->SetTRefDifMax(50.);    // in ns 
     tofTestBeamClust->PosYMaxScal(20.);         //in % of length
     break;
   case 32:
   case 33:
     tofTestBeamClust->SetTRefDifMax(25.);    // in ns 
     tofTestBeamClust->PosYMaxScal(10.);         //in % of length
     break;
   case 42:
   case 43:
     tofTestBeamClust->SetTRefDifMax(12.);   // in ns 
     tofTestBeamClust->PosYMaxScal(5.);        //in % of length
     break;
   case 52:
   case 53:
     tofTestBeamClust->SetTRefDifMax(5.);   // in ns 
     tofTestBeamClust->PosYMaxScal(3.);        //in % of length
     break;
   case 62:
   case 63:
     tofTestBeamClust->SetTRefDifMax(2.5);   // in ns 
     tofTestBeamClust->PosYMaxScal(2.);        //in % of length
     break;
   case 72:
   case 73:
     tofTestBeamClust->SetTRefDifMax(2.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.5);        //in % of length
     break;
   case 82:
   case 83:
     tofTestBeamClust->SetTRefDifMax(1.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length   
     break;
   case 92:
   case 93:
     tofTestBeamClust->SetTRefDifMax(0.6);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length   
     break;

   case 4:                                      // velocity dependence (DelTOF)
     tofTestBeamClust->SetTRefDifMax(6.);    // in ns 
     tofTestBeamClust->PosYMaxScal(1.5);        //in % of length
     break;
   case 14:
     tofTestBeamClust->SetTRefDifMax(5.);   // in ns 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 24:
     tofTestBeamClust->SetTRefDifMax(3.);   // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length
     break;
   case 34:
     tofTestBeamClust->SetTRefDifMax(2.);   // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length
     break;
   case 44:
     tofTestBeamClust->SetTRefDifMax(1.);   // in ns 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length
     break;
   case 54:
     tofTestBeamClust->SetTRefDifMax(0.7);     // in ns 
     tofTestBeamClust->PosYMaxScal(0.7);        //in % of length
     break;
   case 64:
     tofTestBeamClust->SetTRefDifMax(0.5);     // in ns 
     tofTestBeamClust->PosYMaxScal(0.7);        //in % of length
     break;
   default:
     cout << "<E> Calib mode not implemented! stop execution of script"<<endl;
     return;
   } 

   run->AddTask(tofTestBeamClust);

   // =========================================================================
   // ===                       Tracking                                    ===
   // =========================================================================
   CbmStsDigitize* stsDigitize = new CbmStsDigitize(); //necessary for kalman !!
   CbmKF* kalman = new CbmKF();

   CbmTofTrackFinder* tofTrackFinder= new CbmTofTrackFinderNN();
   tofTrackFinder->SetMaxTofTimeDifference(1.);   // in ns/cm 
   tofTrackFinder->SetTxLIM(0.3);                 // max slope dx/dz
   tofTrackFinder->SetTyLIM(0.3);                 // max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(-0.12);                // mean slope dy/dz
   tofTrackFinder->SetSIGLIM(dChi2Lim2);          // matching window in multiples of chi2
   tofTrackFinder->SetChiMaxAccept(dChi2Lim2);    // max matching chi2

   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);              // initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2,3,4,5,6, 10 - 19
   tofFindTracks->SetTtTarg(0.037);                // target value for inverse velocity, > 0.033 ns/cm!
   tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name
   tofFindTracks->SetBeamCounter(5,0,0);         // default beam counter 
  
   tofFindTracks->SetT0MAX(dScalFac);            // in ns
   tofFindTracks->SetSIGT(0.1);                  // default in ns
   tofFindTracks->SetSIGX(0.5);                  // default in cm
   tofFindTracks->SetSIGY(1.5);                  // default in cm 
   tofFindTracks->SetSIGZ(0.5);                  // default in cm 


   switch (iTrackingSetup){
   case 0:                                       // bypass mode
     tofFindTracks->SetMinNofHits(-1);
     tofFindTracks->SetNStations(1);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     break;

   case 1:                                           // track presentation mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(7, 6, 0, 1);           // BUC
     tofFindTracks->SetStation(1, 4, 0, 0);           // HD 
     tofFindTracks->SetStation(2, 9, 0, 1);           // USTC 
     tofFindTracks->SetStation(3, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(4, 9, 1, 1);           // USTC
     tofFindTracks->SetStation(5, 9, 2, 1);           // THU
     tofFindTracks->SetStation(6, 6, 0, 0);           // BUC
     tofTrackFinder->SetSIGT(0.08);                    // in ns
     break;

   case 2:                                           // debugging mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 0, 1);           // USTC 
     tofFindTracks->SetStation(2, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 1, 1);           // USTC
     break;

   case 3:                                           // debugging mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC 
     tofFindTracks->SetStation(2, 6, 0, 0);           // Buc2015a
     tofFindTracks->SetStation(3, 6, 0, 1);           // Buc2015b
     break;

   case 4:                                           // debugging mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond
     tofFindTracks->SetStation(1, 6, 0, 1);           // Buc2015b
     tofFindTracks->SetStation(2, 6, 0, 0);           // Buc2015a 
     tofFindTracks->SetStation(3, 9, 2, 1);           // USTC 
     break;

   case 5:                                           // debugging mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond
     tofFindTracks->SetStation(1, 6, 0, 1);           // Buc2015b
     tofFindTracks->SetStation(2, 6, 0, 0);           // Buc2015a 
     tofFindTracks->SetStation(3, 9, 2, 1);           // THU
     tofFindTracks->SetStation(4, 9, 1, 1);           // USTC 
     break;

   case 7:                                           // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(7);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 6, 0, 1);           // BUC
     tofFindTracks->SetStation(2, 6, 0, 0);           // BUC
     tofFindTracks->SetStation(3, 9, 2, 1);           // THU
     tofFindTracks->SetStation(4, 9, 1, 1);           // USTC
     tofFindTracks->SetStation(5, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(6, 9, 0, 1);           // USTC 
     tofTrackFinder->SetSIGT(0.08);                   // in ns
     break;

   case 8:                                           // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 6, 0, 1);           // BUC
     tofFindTracks->SetStation(2, 6, 0, 0);           // BUC
     tofFindTracks->SetStation(3, 9, 2, 1);           // THU
     tofFindTracks->SetStation(4, 9, 1, 1);           // USTC
     tofFindTracks->SetStation(5, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(6, 9, 0, 1);           // USTC 
     tofFindTracks->SetStation(7, 4, 0, 0);           // HD 
     tofTrackFinder->SetSIGT(0.08);                   // in ns
     break;

   case 100:                                           // track presentation mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 6, 0, 1);           // BUC
     tofFindTracks->SetStation(1, 6, 0, 0);           // BUC
     tofFindTracks->SetStation(2, 9, 2, 1);           // THU
     tofFindTracks->SetStation(3, 9, 1, 1);           // USTC
     tofFindTracks->SetStation(4, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(5, 9, 0, 1);           // USTC 
     tofFindTracks->SetStation(6, 4, 0, 0);           // HD 
     tofFindTracks->SetStation(7, 5, 0, 0);           // Diamond 
     tofTrackFinder->SetSIGT(0.08);                    // in ns
     break;

   default:
     cout << "Tracking setup "<<iTrackingSetup<<" not implemented "<<endl;
     return;
     ;
   }
   tofFindTracks->PrintSetup();

   //run->AddTask(tofFindTracks);

   //
   // Analysis 
   //
   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);

   //CbmTofAnaTestbeam defaults  
   tofAnaTestbeam->SetDXMean(0.);
   tofAnaTestbeam->SetDYMean(0.);
   tofAnaTestbeam->SetDTMean(0.);      // in ns
   tofAnaTestbeam->SetDXWidth(1.0);
   tofAnaTestbeam->SetDYWidth(1.5);
   tofAnaTestbeam->SetDTWidth(0.1);    // in ns
   tofAnaTestbeam->SetCalParFileName(cAnaFile);
   tofAnaTestbeam->SetPosY4Sel(0.5*dScalFac);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetDTDia(0.);       // Time difference to additional diamond
   tofAnaTestbeam->SetMul0Max(20);     // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(20);     // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(20);     // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetTOffD4(10.);  // initialization
   tofAnaTestbeam->SetDTD4MAX(6.);  // initialization of Max time difference Ref - BRef

   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetSel2TOff(0.);   // Shift Sel2 time peak to 0 
   tofAnaTestbeam->SetChi2Lim(0.5*dChi2Lim2);    // initialization of Chi2 selection limit  
   tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);   // initialization of Chi2 selection limit for Mref-Sel2 pair   

   Int_t iBRef=iCalSet%1000;
   Int_t iSet = (iCalSet - iBRef)/1000;
   Int_t iRSel=0;
   Int_t iRSelTyp=0;
   Int_t iRSelSm=0;
   Int_t iRSelRpc=0;
   Int_t iRSelin=iRSel;
   if(iSel2>=0){
     iRSel=iBRef;     // use diamond
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
   }

   iRSelRpc=iRSel%10;
   iRSelTyp = (iRSel-iRSelRpc)/10;
   iRSelSm=iRSelTyp%10;
   iRSelTyp = (iRSelTyp-iRSelSm)/10;

   tofTestBeamClust->SetBeamRefId(iRSelTyp);    // define Beam reference counter 
   tofTestBeamClust->SetBeamRefSm(iRSelSm);
   tofTestBeamClust->SetBeamRefDet(iRSelRpc);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
   tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);

   Int_t iSel2in=iSel2;
   Int_t iSel2Rpc= iSel2%10;
   iSel2=(iSel2-iSel2Rpc)/10;
   Int_t iSel2Sm=iSel2%10;
   iSel2=(iSel2-iSel2Sm)/10;
   if(iSel2 > 0) {
     tofTestBeamClust->SetSel2Id(iSel2); 
     tofTestBeamClust->SetSel2Sm(iSel2Sm); 
     tofTestBeamClust->SetSel2Rpc(iSel2Rpc);

     tofAnaTestbeam->SetMrpcSel2(iSel2);           // initialization of second selector Mrpc Type 
     tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);       // initialization of second selector Mrpc SmId
     tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);     // initialization of second selector Mrpc RpcId
   }

   Int_t iRef = iSet %1000;
   Int_t iDut = (iSet - iRef)/1000;
   Int_t iDutRpc = iDut%10;
   iDut = (iDut - iDutRpc)/10;
   Int_t iDutSm = iDut%10;
   iDut = (iDut - iDutSm)/10;

   tofTestBeamClust->SetDutId(iDut);
   tofTestBeamClust->SetDutSm(iDutSm);
   tofTestBeamClust->SetDutRpc(iDutRpc);

   Int_t iRefRpc = iRef%10;
   iRef = (iRef - iRefRpc)/10;
   Int_t iRefSm = iRef%10;
   iRef = (iRef - iRefSm)/10;

   tofTestBeamClust->SetSelId(iRef);
   tofTestBeamClust->SetSelSm(iRefSm);
   tofTestBeamClust->SetSelRpc(iRefRpc);

   tofAnaTestbeam->SetDut(iDut);              // Device under test   
   tofAnaTestbeam->SetDutSm(iDutSm);          // Device under test   
   tofAnaTestbeam->SetDutRpc(iDutRpc);        // Device under test   
   tofAnaTestbeam->SetMrpcRef(iRef);          // Reference RPC     
   tofAnaTestbeam->SetMrpcRefSm(iRefSm);      // Reference RPC     
   tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    // Reference RPC     

   tofAnaTestbeam->SetChi2Lim(10.);           // initialization of Chi2 selection limit  
   cout << "Run with iRSel = "<<iRSel<<", iSel2 = "<<iSel2in<<endl;

   switch (iSet) {
   case 0:                                 // upper part of setup: P2 - P5
   case 3:                                 // upper part of setup: P2 - P5
   case 34:                                // upper part of setup: P2 - P5
   case 400300:
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-3.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
	   tofAnaTestbeam->SetChi2Lim(100.);   // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetMulDMax(3);      // Max Multiplicity in BeamRef // Diamond    
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }

     tofAnaTestbeam->SetChi2Lim(30.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.5);
     tofAnaTestbeam->SetDYWidth(1.5);
     tofAnaTestbeam->SetDTWidth(120.);   // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(3);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.25);  // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(4);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(4);      // Max Multiplicity in Ref - RPC 
     break;

   case 920300:
   case 921300:
     switch (iRSel){
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	   tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.2); // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.5);  // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(3);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(3);      // Max Multiplicity in Ref - RPC 
     break; 

   case 920400:
   case 921400:
     switch (iRSel){
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	   tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.2);    // in ps
     tofAnaTestbeam->SetCh4Sel(8);        // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);       // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(30);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(30);      // Max Multiplicity in Ref - RPC 
     break; 

   case 300920:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(1.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.5); // in ps
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(5);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(5);      // Max Multiplicity in Ref - RPC 
     break; 

   case 900920:

     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.1); // in ps
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(15);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(15);      // Max Multiplicity in Ref - RPC 
     break; 

   case 300921:
   case 400921:
   case 900921:
   case 901921:
   case 910921:
   case 920921:
     switch (iRSel){
         case 3:
	   tofAnaTestbeam->SetTShift(-0.8);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-150.);     // Shift Sel2 time peak to 0
	   break;

         case 5: // sel2 = 920
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-5.);  // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.1);  // Shift Sel2 time peak to 0
	   break;

         case 9:   // sel2 = -920
	   tofAnaTestbeam->SetTShift(1.2);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.150);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.0);
     tofAnaTestbeam->SetDYWidth(2.0);
     tofAnaTestbeam->SetDTWidth(0.15); // in ps
     break; 

   case 901900:
   case 400900:
   case 300900:
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.17);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(40.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.0);
     tofAnaTestbeam->SetDYWidth(2.0);
     tofAnaTestbeam->SetDTWidth(0.15); // in ps
     break; 

   case 901910:
   case 911910:
   case 920910:
   case 921910:
   case 600910:
   case 601910:
   case 400910:
   case 300910:
     switch (iRSel){
         case 400:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 500:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(10.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.17);     // Shift Sel2 time peak to 0
	   break;

         case 921:
	   tofAnaTestbeam->SetTShift(-1.);    // Shift DTD4 to 0
	   tofAnaTestbeam->SetSel2TOff(-0.5);  // Shift Sel2 time peak to 0
	   tofAnaTestbeam->SetTOffD4(12.);   // Shift DTD4 to physical value
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(10.);    // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetChi2Lim2(3.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.0);
     tofAnaTestbeam->SetDYWidth(2.0);
     tofAnaTestbeam->SetDTWidth(0.15); // in ns
     break;
     
   case 300400:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

   case 210200:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

   case 700600:
   case 701600:
   case 702600:
   case 703600:
   case 100600:
   case 200600:
   case 210600:
   case 601600:
   case 900600:
   case 901600:
   case 910600:
   case 911600:
   case 920600:
   case 921600:
     
     switch (iRSel){
         case 500:
	   tofAnaTestbeam->SetTShift(-5.5); //24.5);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(30.);    // Shift DTD4 to physical value
	   switch (iSel2in){
	   case 910:
	     tofAnaTestbeam->SetSel2TOff(-5.);  // Shift Sel2 time peak to 0
	     break;
	   case 921:
	     tofAnaTestbeam->SetSel2TOff(-4.5);  // Shift Sel2 time peak to 0
	     break;

	   default:
	     cout << " setup not implemented " << endl;
	     return;
	   }
	   break;

         case 601:
	   tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
	   break;

         case 921:
	   tofAnaTestbeam->SetTShift(0.);      // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(11.);     // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.13);  // Shift Sel2 time peak to 0
	   break;

     }
     tofAnaTestbeam->SetChi2Lim(10.);    // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetChi2Lim2(50.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(0.5);
     tofAnaTestbeam->SetDYWidth(1.);
     tofAnaTestbeam->SetDTWidth(0.1); // in ps

     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;

   case 900601:
   case 901601:
   case 910601:
   case 911601:
   case 920601:
   case 921601:
   case 600601:
     switch (iRSel){
         case 5:
	   tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 6:
	   tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

     }
     tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(0.5);
     tofAnaTestbeam->SetDYWidth(1.);
     tofAnaTestbeam->SetDTWidth(0.1); // in ps

     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;
     
     default:
     cout<<"<W> detector setup "<<iSet<<" unknown, run with default analysis parameter values "<<endl;
      //	   return;
	 ;
   }  // end of different subsets

   //   run->AddTask(tofAnaTestbeam);
   // =========================================================================
   /*
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000);
   run->AddTask(display);   
   */
  // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   Bool_t kParameterMerged = kTRUE;
   FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
   parIo2->open(ParFile.Data(), "UPDATE");
   parIo2->print();
   rtdb->setFirstInput(parIo2);
   
   FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
   parIo1->open(parFileList, "in");
   parIo1->print();
   rtdb->setSecondInput(parIo1);
   rtdb->print();
   rtdb->printParamContexts();

   //  FairParRootFileIo* parInput1 = new FairParRootFileIo();
   //  parInput1->open(ParFile.Data());
   //  rtdb->setFirstInput(parInput1);
  FairEventManager *fMan= new FairEventManager();
 
  CbmEvDisTracks *Tracks =  new CbmEvDisTracks("Tof Tracks",1);
  fMan->AddTask(Tracks);
  CbmPixelHitSetDraw *TofUHits = new CbmPixelHitSetDraw ("TofUHit", kRed, kOpenCross );
  fMan->AddTask(TofUHits);  
  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle, 4);// kFullSquare);
  fMan->AddTask(TofHits); 


  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(1);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for(Int_t i=0; i<allvolumes->GetEntries(); i++){
    TGeoVolume* vol     = (TGeoVolume*)allvolumes->At(i);
    TString name = vol->GetName();
    //    cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    vol->SetTransparency(99);
    /* switch (char *) not allowed any more in root 6 :(
    switch(name.Data()) {
    case "counter":
      vol->SetTransparency(95);
      break;

    case "tof_glass":
    case "Gap":
    case "Cell":
      vol->SetTransparency(99);
      break;

    case "pcb":
      vol->SetTransparency(30);
      break;

    default:
      vol->SetTransparency(96);
    }
    */
  }
  //  gGeoManager->SetVisLevel(3);
  //  top->SetTransparency(80); 
  //  top->Draw("ogl");

  //  fMan->Init(1,4,10000);                     
  fMan->Init(1,5); 
                    
  cout << "customize TEveManager gEve "<< gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow-10);
  TGLViewer* v = gEve->GetDefaultGLViewer();
  TGLAnnotation* ann = new TGLAnnotation(v,cFileId,0.01,0.98);
  ann->SetTextSize(0.03);// % of window diagonal
  ann->SetTextColor(4);

  //  gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);

  {   // from readCurrentCamera(const char* fname)
  TGLCamera& c = gEve->GetDefaultGLViewer()->CurrentCamera();
  const char* fname="Cam.sav";
  TFile* f = TFile::Open(fname, "READ");
  if (!f) 
    return;
  if (f->GetKey(c.ClassName())) {
    f->GetKey(c.ClassName())->Read(&c);
    c.IncTimeStamp();
    gEve->GetDefaultGLViewer()->RequestDraw();
  }
  }

  // -----   Intialise and run   --------------------------------------------
  // run->Init();
  //  cout << "Starting run" << endl;
  //  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  // default display 
  /*
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct = "pl_over_Mat04D4best()";  
  gROOT->LoadMacro(Display_Status);

  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");
  iSet=1000;
  switch(iSet){
  case 0:
  case 3:
  case 49:
  case 79:
  case 94:
  case 37:
  case 97:
  case 39:
  case 93:
    gInterpreter->ProcessLine("pl_over_clu(3)");
    gInterpreter->ProcessLine("pl_over_clu(4)");
    gInterpreter->ProcessLine("pl_over_clu(5)");
    gInterpreter->ProcessLine("pl_over_clu(5,1)");
    gInterpreter->ProcessLine("pl_over_clu(5,2)");
    gInterpreter->ProcessLine("pl_over_clu(9,0,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(9,1,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,1,1)");
    gInterpreter->ProcessLine("pl_over_clu(9,2,0)");
    gInterpreter->ProcessLine("pl_over_clu(9,2,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9,2,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,4)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9,2,1)");
    gInterpreter->ProcessLine("pl_all_dTSel()");
    gInterpreter->ProcessLine("pl_over_MatD4sel()");
    break;

  case 1:
  case 61:
  case 81:
  case 16:
  case 86:
  case 18:
  case 68:

    gInterpreter->ProcessLine("pl_over_clu(1)");
    gInterpreter->ProcessLine("pl_over_clu(5)");
    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(8)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,2)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,3)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,4)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,4)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,6)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5,1)");
    gInterpreter->ProcessLine("pl_all_dTSel()");
    gInterpreter->ProcessLine("pl_over_MatD4sel()");
    break;

  default:
    ;
  }
  gInterpreter->ProcessLine(Display_Funct);
  */
}
