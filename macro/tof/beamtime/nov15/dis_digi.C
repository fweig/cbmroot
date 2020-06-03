void dis_digi(Int_t nEvents = 100000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=300, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iSet=0, Bool_t bOut=0, Int_t iSel2=0, Int_t iGenCor=1, Int_t iTrackingSetup=0) 
{
  Int_t iVerbose = 1;
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  TString logLevel = "FATAL";
  TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger* log;  

   TString workDir    = gSystem->Getenv("VMCWORKDIR");
   TString paramDir   = workDir + "/macro/tof/beamtime/nov15";
   TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/disdigi_"   + cFileId + Form("_%06d%03d",iSet,iSel2) + ".out.root";
   TString cAnaFile=""; 
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId);

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernNov2015.txt";
   parFileList->Add(&mapParFile);
   TString FId=cFileId;
   TString TofGeo="v15c";
   if( FId.Contains("CernSps02Mar") || FId.Contains("CernSps03Mar") ){
     TofGeo="v15b";
   }
   if(FId.Contains("CernSps28Feb")){
     TofGeo="v15a";
   }
   cout << "Geometry version "<<TofGeo<<endl;

   TString FPar="";
   if(FId.Contains("tsu")) {
     cout << FId << ": Analyse TSU counter "<< endl;
     TofGeo="v14c_tsu";
     FPar="tsu.";
   }

   TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   parFileList->Add(&tofDigiFile);   

   TObjString tofDigiBdfFile = workDir  + "/parameters/tof/tof_" + TofGeo +".digibdf.par";
   parFileList->Add(&tofDigiBdfFile);

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
  gLogger->SetLogScreenLevel(logLevel.Data());

   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   //run->AddTask(tofMapping);

   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, bOut);

   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(3.);         // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(20.);       // Max Counter Cluster Multiplicity for filling calib histos  
   tofTestBeamClust->SetCalRpc(calSm);           // select detector for calibration update  
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10000.);          // Tot upper limit for walk corection
   tofTestBeamClust->SetTotMin(1.); //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(5000.);      // effective lower Tot limit  in ps from peak position
   tofTestBeamClust->SetTotMean(2000.);          // Tot calibration target value in ps 
   tofTestBeamClust->SetMaxTimeDist(500.);       // default cluster range in ps 
   //tofTestBeamClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ps 

   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_set%06d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iSet,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s_set%06d.hst.root",cFileId,iSet);
   tofTestBeamClust->SetOutHstFileName(cOutFname);

   TString cAnaFile=Form("%s_%06d%03d_tofAnaTestBeam.hst.root",cFileId,iSet,iSel2);

   switch (calMode) {
   case 0:                                      // initial calibration 
     tofTestBeamClust->SetTotMax(100000.);      // 100 ns
     //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(2000000.); // in ps 
     tofTestBeamClust->PosYMaxScal(2000.);      // in % of length 
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 1:                                       // save offsets, update walks, for diamonds 
     tofTestBeamClust->SetTRefDifMax(50000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.1);         // in % of length 
     break;
   case 11:
     tofTestBeamClust->SetTRefDifMax(4000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;   
   case 21:
     tofTestBeamClust->SetTRefDifMax(3000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 31:
     tofTestBeamClust->SetTRefDifMax(2000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 41:
     tofTestBeamClust->SetTRefDifMax(1000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        // in % of length 
     break;   
   case 51:
     tofTestBeamClust->SetTRefDifMax(700.);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;
   case 61:
     tofTestBeamClust->SetTRefDifMax(500.);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;   
   case 71:
     tofTestBeamClust->SetTRefDifMax(400.);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.6);        // in % of length 
     break;

   case 2:                                      // time difference calibration
     tofTestBeamClust->SetTRefDifMax(300000.);  // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);      //in % of length
     break;

   case 3:                                       // time offsets 
     tofTestBeamClust->SetTRefDifMax(200000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);       //in % of length
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 12:
   case 13:
     tofTestBeamClust->SetTRefDifMax(100000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(100.);        //in % of length
     break;
   case 22:
   case 23:
     tofTestBeamClust->SetTRefDifMax(50000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(50.);         //in % of length
     break;
   case 32:
   case 33:
     tofTestBeamClust->SetTRefDifMax(25000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(20.);         //in % of length
     break;
   case 42:
   case 43:
     tofTestBeamClust->SetTRefDifMax(12000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10.);        //in % of length
     break;
   case 52:
   case 53:
     tofTestBeamClust->SetTRefDifMax(5000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4.);        //in % of length
     break;
   case 62:
   case 63:
     tofTestBeamClust->SetTRefDifMax(2500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);        //in % of length
     break;
   case 72:
   case 73:
     tofTestBeamClust->SetTRefDifMax(2000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length
     break;
   case 82:
   case 83:
     tofTestBeamClust->SetTRefDifMax(1000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length   
     break;
   case 92:
   case 93:
     tofTestBeamClust->SetTRefDifMax(600.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length   
     break;

   case 4:                                      // velocity dependence (DelTOF)
     tofTestBeamClust->SetTRefDifMax(6000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.5);        //in % of length
     break;
   case 14:
     tofTestBeamClust->SetTRefDifMax(2500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 24:
     tofTestBeamClust->SetTRefDifMax(1500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length
     break;
   case 44:
     tofTestBeamClust->SetTRefDifMax(1000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length
     break;
   case 54:
     tofTestBeamClust->SetTRefDifMax(700.);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        //in % of length
     break;
   case 64:
     tofTestBeamClust->SetTRefDifMax(500.);     // in ps 
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
   tofTrackFinder->SetMaxTofTimeDifference(5000.);// in ps/cm 
   tofTrackFinder->SetTxLIM(0.3);                  // max slope dx/dz
   tofTrackFinder->SetTyLIM(0.2);                  // max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(0.1);                // mean slope dy/dz
   tofTrackFinder->SetSIGLIM(4.);                 // max matching chi2
   tofTrackFinder->SetSIGT(100.);                // in ps
   tofTrackFinder->SetSIGX(1.);                  // in cm
   tofTrackFinder->SetSIGY(1.);                  // in cm
   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);              // initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2
   tofFindTracks->SetTtTarg(33.7);               // target value for inverse velocity, > 33.3 !
   tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name  
   switch (iTrackingSetup){
   case 1:                                       // calibration mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 4, 0, 0);           // P5 
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 2, 1);           // USTC
     //tofFindTracks->SetStation(4, 9, 1, 0);           // broken
     tofFindTracks->SetStation(4, 9, 1, 1);           //
     tofFindTracks->SetStation(5, 9, 0, 0);           //  
     tofFindTracks->SetStation(6, 9, 0, 1);           //  
     tofFindTracks->SetStation(7, 3, 0, 0);           // P2 
     break;

   case 10:                                       // full lower setup
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(14);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 8, 0, 2);           // THUpad 
     tofFindTracks->SetStation(2, 8, 0, 3);           // THUpad
     tofFindTracks->SetStation(3, 8, 0, 0);           // THUpad
     tofFindTracks->SetStation(4, 8, 0, 1);           // THUpad
     tofFindTracks->SetStation(5, 8, 0, 4);           // THUpad
     tofFindTracks->SetStation(6, 8, 0, 5);           // THUpad
     tofFindTracks->SetStation(7, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(8, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(9, 7, 0, 0);           // Buc2012
     tofFindTracks->SetStation(10, 7, 0, 1);           // Buc2012
     tofFindTracks->SetStation(11, 7, 0, 2);           // Buc2012
     tofFindTracks->SetStation(12, 7, 0, 3);           // Buc2012
     tofFindTracks->SetStation(13, 1, 0, 0);           // BucRef  
     break;

   case 11:                                       // full lower setup
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(2, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(3, 1, 0, 0);           // BucRef  
     break;

   default:
     cout << "Tracking setup "<<iTrackingSetup<<" not implemented "<<endl;
     return;
     ;
   }
   run->AddTask(tofFindTracks);
   //
   // Analysis 
   //
   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);

   //CbmTofAnaTestbeam defaults  
   tofAnaTestbeam->SetDXMean(0.);
   tofAnaTestbeam->SetDYMean(0.);
   tofAnaTestbeam->SetDTMean(0.);  // in ps
   tofAnaTestbeam->SetDXWidth(0.4);
   tofAnaTestbeam->SetDYWidth(0.4);
   tofAnaTestbeam->SetDTWidth(80.); // in ps
   tofAnaTestbeam->SetCalParFileName(cAnaFile);
   tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
   tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4 
   tofAnaTestbeam->SetMul0Max(20);   // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(10);    // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(20);      // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization
   tofAnaTestbeam->SetTOffD4(13000.);   // initialization

   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetTShift(0.);       // Shift DTD4 to 0
   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0 
   tofAnaTestbeam->SetTOffD4(13000.);   // Shift DTD4 to physical value

   Int_t iRSel=0;
   Int_t iRSelSm=0;
   Int_t iRSelRpc=0;
   if(iSel2>=0){
     iRSel=5;   // use diamond
     iRSelSm=0;
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
     iRSelRpc=iRSel%10;
     iRSel = (iRSel-iRSelRpc)/10;
     iRSelSm=iRSel%10;
     iRSel = (iRSel-iRSelSm)/10;
   }

   tofTestBeamClust->SetBeamRefId(iRSel);    // define Beam reference counter 
   tofTestBeamClust->SetBeamRefSm(iRSelSm);
   tofTestBeamClust->SetBeamRefDet(iRSelRpc);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   tofAnaTestbeam->SetBeamRefSmType(iRSel);
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);

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

   tofAnaTestbeam->SetChi2Lim(20.);             // initialization of Chi2 selection limit  

   switch (iSet) {
   case 0:                                 // upper part of setup: P2 - P5
   case 3:                                 // upper part of setup: P2 - P5
   case 34:                                // upper part of setup: P2 - P5
   case 400300:
   case 921300:
   case 920300:
   case 921920:
   case 920921:
     switch (iRSel){
         case 3:
	   tofAnaTestbeam->SetTShift(-2000.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
	   tofAnaTestbeam->SetChi2Lim(1000.);   // initialization of Chi2 selection limit  
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(200.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
	   tofAnaTestbeam->SetChi2Lim(100.);   // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetMulDMax(3);      // Max Multiplicity in BeamRef // Diamond    
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(100.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(500.);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetCh4Sel(15.);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(12.);     // Width  of channel selection window
     break;
 
   case 300400:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2000.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

   case 601600:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2000.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

     default:
	   cout<<"<E> detector setup "<<iSet<<" unknown, stop!"<<endl;
	   return;
	 ;
   }  // end of different subsets

   switch(cFileId){
     case " ":

	 break;

     default:
	 ;
   }

   //run->AddTask(tofAnaTestbeam);

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
  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle );// kFullSquare);
  fMan->AddTask(TofHits);   
  CbmEvDisTracks *Track =  new CbmEvDisTracks ("Tof Tracks",1);
  fMan->AddTask(Track);

  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(1);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for(Int_t i=0; i<allvolumes->GetEntries(); i++){
    TGeoVolume* vol     = (TGeoVolume*)allvolumes->At(i);
    TString name = vol->GetName();
    //    cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    switch(name) {
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
  }
  //  gGeoManager->SetVisLevel(3);
  //  top->SetTransparency(80); 
  //  top->Draw("ogl");

  //  fMan->Init(1,4,10000);                     
  fMan->Init(1,5); 
                    
  cout << "gEve "<< gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow-10);
  //  gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);

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
