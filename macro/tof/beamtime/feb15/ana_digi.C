void ana_digi(Int_t nEvents = 100000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=300, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iSet=0, Bool_t bOut=0, Int_t iSel2=0) 
{
  Int_t iVerbose = 1;
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger* log;  

   TString workDir    = gSystem->Getenv("VMCWORKDIR");
   TString paramDir   = workDir + "/macro/tof/beamtime/feb15";
   TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/digi_"   + cFileId + Form("_%06d_%03d",iSet,iSel2) + ".out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernFeb2015.txt";
   parFileList->Add(&mapParFile);
   TString FId=cFileId;
   TString TofGeo="v15b";
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

   //   TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
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
   //   run->AddTask(tofMapping);

   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, bOut);

   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(2.);         // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(10.);       // Max Counter Cluster Multiplicity for filling calib histos  
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

   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);

   //CbmTofAnaTestbeam defaults  
   tofAnaTestbeam->SetReqTrg(0);   // 0 - no selection
   tofAnaTestbeam->SetDXMean(0.);
   tofAnaTestbeam->SetDYMean(0.);
   tofAnaTestbeam->SetDTMean(0.);  // in ps
   tofAnaTestbeam->SetDXWidth(0.4);
   tofAnaTestbeam->SetDYWidth(0.4);
   tofAnaTestbeam->SetDTWidth(80.); // in ps
   tofAnaTestbeam->SetCalParFileName(cAnaFile);
   tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
   tofAnaTestbeam->SetCorMode(RefSel); // 1 - DTD4, 2 - X4 
   tofAnaTestbeam->SetMul0Max(3);      // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(3);      // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization

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
     iRSel=5;     // use diamond
     iRSelSm=0;   // -1 -> any one ok
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
   case 300400:                                // upper part of setup: P2 - P5
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(4);

	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;

   case 900400:                                // upper part of setup: P2 - P5
	 tofTestBeamClust->SetDutId(9);
	 tofTestBeamClust->SetSelId(4);

	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 break;

   case 1:                                 // upper part of setup: Buc2013 - BucRef
   case 600100:
	 tofTestBeamClust->SetDutId(6);
	 tofTestBeamClust->SetSelId(1);

	 tofAnaTestbeam->SetDut(6);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);    // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(38.5);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(40.);  // Width  of channel selection window
	 tofAnaTestbeam->SetTShift(1000.);    // initialization
	 tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0 

	 break;

     case 6:                             // lower part of setup: Buc2013 - BucRef
     case 100600:
	 tofTestBeamClust->SetDutId(1);
	 tofTestBeamClust->SetSelId(6);

	 tofAnaTestbeam->SetDut(1);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(6);    // Reference RPC     

	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(38.5);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(45.);  // Width  of channel selection window
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetSel2TOff(-90.);   // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetTOffD4(11000.);   // Shift DTD4 to physical value 

	 break;

     case 800600:
	 tofTestBeamClust->SetDutId(8);
	 tofTestBeamClust->SetSelId(6);

	 tofAnaTestbeam->SetDut(8);        // Device under test   
	 tofAnaTestbeam->SetDutRpc(2);     // Device under test   
	 tofAnaTestbeam->SetMrpcRef(6);    // Reference RPC     

	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(38.5);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(45.);  // Width  of channel selection window
	 tofAnaTestbeam->SetSel2TOff(-230.);  // Shift Sel2 time peak to 0 

	 tofAnaTestbeam->SetChi2Lim(3.);      // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetPosY4Sel(0.2);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length	   

	 break;

   case 7:                                 // upper part of setup: USTC - P5
   case 700400:
	 tofTestBeamClust->SetDutId(7);
	 tofTestBeamClust->SetSelId(4);

	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(12.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);   // Width  of channel selection window
	 break;

   case 9:                                 // upper part of setup: THUstrip - P5
	 tofTestBeamClust->SetDutId(9);
	 tofTestBeamClust->SetSelId(4);

	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(12.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);   // Width  of channel selection window
	 break;

   case 300700:                                 // upper part of setup: P2 - USTC
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(7);

	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 break;

   case 400700:                                 // upper part of setup: P2 - USTC
	 tofTestBeamClust->SetDutId(4);
	 tofTestBeamClust->SetSelId(7);

	 tofAnaTestbeam->SetDut(4);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 break;

   case 900700:                                 // upper part of setup: P2 - USTC
	 tofTestBeamClust->SetDutId(9);
	 tofTestBeamClust->SetSelId(7);

	 tofAnaTestbeam->SetChi2Lim(600.);    // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(100.);   // Width  of channel selection window
	 break;

   case 300900:                                 // upper part of setup: P2 - THUstrip
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(9);

	 tofAnaTestbeam->SetDut(3);          // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);      // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(12.);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);    // Width  of channel selection window
	 tofAnaTestbeam->SetSel2TOff(-210.);  // Shift Sel2 time peak to 0 
	 break;

   case 400900:                                 // upper part of setup: P5 - THUstrip
	 tofTestBeamClust->SetDutId(4);
	 tofTestBeamClust->SetSelId(9);

	 tofAnaTestbeam->SetDut(4);         // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);     // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(11.5);      // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(0.7);      // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);     // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetPosY4SelOff(-1.5); // Y Position selection offset in cm 	
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(6.);        // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-100.);     // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetPosYS2Sel(0.15);     // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetPosYS2SelOff(-1.5); // Y Position selection offset in cm 	
	   tofAnaTestbeam->SetChS2Sel(15.5);      // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(10.7);      // Width  of channel selection window
	   break;
	 case 7:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-100.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 700900:                                 // upper part of setup: P5 - THUstrip
	 tofTestBeamClust->SetDutId(7);
	 tofTestBeamClust->SetSelId(9);

	 tofAnaTestbeam->SetDut(7);         // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);     // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(11.5);      // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(10.7);      // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(1.1);     // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetPosY4SelOff(-1.5); // Y Position selection offset in cm 	
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(6.);        // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-100.);     // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetPosYS2Sel(0.5);     // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetPosYS2SelOff(0.); // Y Position selection offset in cm 	
	   tofAnaTestbeam->SetChS2Sel(15.5);      // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(10.7);      // Width  of channel selection window
	   break;
	 case 4:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-100.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

         default:
	 ;
   }  // end of different subsets

   switch(cFileId){
     case "CernSps01Mar2203_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_0c0_nb":

	 break;

     default:
	 ;
   }

   run->AddTask(tofAnaTestbeam);
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

  // -----   Intialise and run   --------------------------------------------
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  // default display 
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct = "pl_over_Mat04D4best()";  
  gROOT->LoadMacro(Display_Status);

  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  gROOT->LoadMacro("pl_over_MatD4sel.C");

  switch(iSet){
  case 0:
  case 3:
  case 49:
  case 79:
  case 34:
  case 94:
  case 37:
  case 97:
  case 39:
  case 99:
  case 93:
  case 300400:
  case 400300:
  case 921920:
  case 920921:
  case 920300:
  case 921300:
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
  case 600100:
  case 800100:
  case 100600:
  case 800600:
  case 100800:
  case 600800:

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
}
