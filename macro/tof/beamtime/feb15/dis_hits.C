void dis_hits(Int_t nEvents=10, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="Cern", Int_t iSet=0, Int_t iSel2=0, Int_t iTrackingSetup=6) 
{ // display hits in event display 

   Int_t iVerbose = 1;
   // Specify log level (INFO, DEBUG, DEBUG1, ...)
   TString logLevel = "FATAL";
   //TString logLevel = "ERROR";
   TString logLevel = "INFO";
   TString logLevel = "DEBUG"; 
   TString logLevel = "DEBUG1";
   //TString logLevel = "DEBUG2";
   TString logLevel = "DEBUG3";
   FairLogger* log;  

   TString workDir       = gSystem->Getenv("VMCWORKDIR");
   TString paramDir      = workDir  + "/macro/tof/beamtime/feb15";
   TString ParFile       = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile     = paramDir + "/unpack_" + cFileId + ".out.root";
   TString InputDigiFile = paramDir + "/digi_"   + cFileId + Form("_%03d",iSet) + ".out.root";
   TString OutputFile    = paramDir + "/dishits_" + cFileId + Form("_%03d%02d%1d",iSet,iSel,iSel2) + ".out.root";
   //   TString cAnaFile=Form("%s_%03d_%02d%1d_tofAnaTestBeam.hst.root",cFileId,iSet,iSel,iSel2);
   TString cAnaFile=""; 
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId);

   cout << " InputDigiFile = "
	<< InputDigiFile
	<< endl;

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernFeb2015.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v15b";  //default
   TString FId=cFileId;
   if(   FId.Contains("CernSps02Mar") 
      || FId.Contains("CernSps03Mar") ){
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

   TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   //   TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
   parFileList->Add(&tofDigiBdfFile);

   TString geoDir  = gSystem->Getenv("VMCWORKDIR");
   TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
   TFile* fgeo = new TFile(geoFile);
   TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
   if (NULL == geoMan){
     cout << "<E> FAIRGeom not found in geoFile"<<endl;
     return;
   }

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *run= new FairRunAna();
  cout << "InputFile:     "<<InputFile.Data()<<endl;
  cout << "InputDigiFile: "<<InputDigiFile.Data()<<endl;

  run->SetInputFile(InputFile.Data());
  run->AddFriend(InputDigiFile.Data());
  //run->SetInputFile(InputDigiFile.Data());
  //run->AddFriend(InputFile.Data());
  run->SetOutputFile(OutputFile);
  gLogger->SetLogScreenLevel(logLevel.Data());

   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   //   run->AddTask(tofMapping);

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
   case 0:                                       // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStations(3974);             // upper part of Feb15 setup 
     break;
   case 1:                                         //"Standard" for event analysis 
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStations(3974);             // upper part of Feb15 setup 
     break;
   case 2:                                         // for vertex
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStations(3974);             // upper part of Feb15 setup 
     break;
   case 3:                                         //"Standard" for timing resolution 
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStations(3974);             // upper part of Feb15 setup 
     break;
   case 4:                                         //"Standard" 
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStations(53974);             // upper part of Feb15 setup 
     break;
   case 5:                                         //for vertex analysis 
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStations(53974);             // upper part of Feb15 setup 
     break;
   case 6:                                         //for open vertex analysis 
     tofFindTracks->SetMinNofHits(1);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStations(53974);             // upper part of Feb15 setup 
     break;
   case 10:
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStations(5681);         // lower part: Buc2013, THUpad, BucRef  
     break;
   case 73:                                         //for Ana - class, Dut=7, sel2=3
   case 74:                                         //for Ana - class, Dut=7, sel2=4
   case 79:                                         //for Ana - class, Dut=7, sel2=9
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStations(394);             // upper part of Feb15 setup 
     break;
   case 93:                                         //for Ana - class, Dut=9, sel2=3 
   case 94:                                         //for Ana - class, Dut=9, sel2=4 
   case 97:                                         //for Ana - class, Dut=9, sel2=7 
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStations(374);             // upper part of Feb15 setup 
     break;
   case 34:                                       //for Ana - class, Dut=3, sel2=4 
   case 37:                                       //for Ana - class, Dut=3, sel2=3 
   case 39:                                       //for Ana - class, Dut=3, sel2=9 
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStations(974);             // upper part of Feb15 setup 
     break;
   default:
     cout << "Tracking setup "<<iTrackingSetup<<" not implemented "<<endl;
     return;
     ;
   }
   run->AddTask(tofFindTracks);
   // =========================================================================
   // ===                       Analysis                                    ===
   // =========================================================================
   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   tofAnaTestbeam->SetCorMode(iGenCor); // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp 
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization

   //CbmTofAnaTestbeam defaults  
   tofAnaTestbeam->SetDXMean(0.);
   tofAnaTestbeam->SetDYMean(0.);
   tofAnaTestbeam->SetDTMean(0.);      // in ps
   tofAnaTestbeam->SetDXWidth(0.5);
   tofAnaTestbeam->SetDYWidth(0.5);
   tofAnaTestbeam->SetDTWidth(90.);    // in ps
   tofAnaTestbeam->SetCalParFileName(cAnaFile);
   tofAnaTestbeam->SetPosY4Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetDTDia(0.);       // Time difference to additional diamond
   tofAnaTestbeam->SetMul0Max(20);     // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(10);     // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(20);     // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetTOffD4(10000.);  // initialization
   tofAnaTestbeam->SetDTD4MAX(6000.);  // initialization of Max time difference Ref - BRef

   //tofAnaTestbeam->SetTShift(-28000.);// initialization
   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetSel2TOff(450.);   // Shift Sel2 time peak to 0 

   Int_t iRSel=0;
   if(iSel2>=0){
     iRSel=5;   // use diamond
     tofAnaTestbeam->SetTShift(-2000.);  // initialization
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
     tofAnaTestbeam->SetTShift(50.);  // initialization
   }
   tofAnaTestbeam->SetMrpcSel2(iSel2); // initialization of second selector Mrpc 
   tofAnaTestbeam->SetChi2Lim(10000.);     // initialization of Chi2 selection limit  
   tofAnaTestbeam->SetBeamRefSmType(iRSel); // common reaction reference 
   tofAnaTestbeam->SetBeamRefSmId(0);
   tofAnaTestbeam->SetSIGLIM(3.);                // max matching chi2
   tofAnaTestbeam->SetSIGT(100.);                // in ps
   tofAnaTestbeam->SetSIGX(1.);                  // in cm
   tofAnaTestbeam->SetSIGY(1.);                  // in cm

   switch (iSel) {
   case 0:                                 // upper part of setup: P2 - P5
   case 34:
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(6.);   // Width  of channel selection window
	 //tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	 //	 tofAnaTestbeam->SetTShift(-2000.);  // initialization
	 switch(iSel2){
	 case 9:
	   tofAnaTestbeam->SetSel2TOff(-240.);   // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	   break;
	 case 7:
	   break;
	 default:
	   ;
	 }
	 break;

   case 74:
	 tofAnaTestbeam->SetDut(7);          // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);      // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(6.);     // Width  of channel selection window
	 tofAnaTestbeam->SetTOffD4(14000.);  // initialization
	 switch (iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(280.);   // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetPosYS2Sel(0.2);   // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetChS2Sel(16.);      // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(1.5);   // Width  of channel selection window
	   break;
	 case 9:
	   tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(130.);   // Shift Sel2 time peak to 0 
	   break;
	 default:
	   ;
	 }
	 break;

   case 94:
	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(6.);   // Width  of channel selection window
	 switch (iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(230.);   // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetPosYS2Sel(0.2);   // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetPosYS2SelOff(-1.5); // Y Position selection offset in cm 	
	   tofAnaTestbeam->SetChS2Sel(16.);      // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(1.5);   // Width  of channel selection window
	   break;

	 case 7:
	   tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(50.);      // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 1:                             // upper part of setup: Buc2013 - BucRef
   case 16:
	 tofAnaTestbeam->SetDut(6);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);    // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(38.5);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(35.);   // Width  of channel selection window
	 break;

   case 3:                                 // upper part of setup: P2 - P5

	 break;

   case 43:                                 // upper part of setup: P2 - THUstrip
	 tofAnaTestbeam->SetDut(4);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(3);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(14.5);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(1.);      // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length	
	 tofAnaTestbeam->SetPosY4SelOff(-2.); // Y Position selection offset in cm 	
	 switch(iSel2){
	 case 7:
	   tofAnaTestbeam->SetChi2Lim(6.);      // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(50.);    // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetChS2Sel(8.5);    // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(1.);     // Width  of channel selection window
	   tofAnaTestbeam->SetPosYS2Sel(0.1);   // Y Position selection in fraction of strip length	   
	   tofAnaTestbeam->SetPosYS2SelOff(-2.); // Y Position selection offset in cm 	
	   break;

	 case 9:
	   tofAnaTestbeam->SetChi2Lim(4.);      // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(100.);    // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetChS2Sel(11.5);    // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(1.);     // Width  of channel selection window
	   tofAnaTestbeam->SetPosYS2Sel(0.1);   // Y Position selection in fraction of strip length	
	   break;

	 }
	 break;

   case 73:                                 // upper part of setup: P2 - THUstrip
	 tofAnaTestbeam->SetDut(7);         // Device under test   
	 tofAnaTestbeam->SetMrpcRef(3);     // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(16.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(17.);   // Width  of channel selection window
	 //	 tofAnaTestbeam->SetPosY4Sel(0.3);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	 switch(iSel2){
	 case 4:
	   tofAnaTestbeam->SetTShift(3500.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(3500.);   // Shift Sel2 time peak to 0 
	   break;
	 case 9:  // cuts are the same as for 43-9
	   tofAnaTestbeam->SetCh4Sel(15.5);     // Center of channel selection window
	   tofAnaTestbeam->SetTShift(810.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(1080.);   // Shift Sel2 time peak to 0 
	   //	   tofAnaTestbeam->SetDCh4Sel(1.);      // Width  of channel selection window
	   //	   tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length	
	   //	   tofAnaTestbeam->SetPosY4SelOff(-2.); // Y Position selection offset in cm 	
	   //	   tofAnaTestbeam->SetChi2Lim(4.);      // initialization of Chi2 selection limit  
	   //	   tofAnaTestbeam->SetSel2TOff(100.);    // Shift Sel2 time peak to 0 
	   //	   tofAnaTestbeam->SetChS2Sel(11.5);    // Center of channel selection window
      	   //	   tofAnaTestbeam->SetDChS2Sel(1.);     // Width  of channel selection window
	   //	   tofAnaTestbeam->SetPosYS2Sel(0.1);   // Y Position selection in fraction of strip length	
	   break;
	 }
	 break;

   case 93:                                  // upper part of setup: THU - P2
	 tofAnaTestbeam->SetDut(9);          // Device under test   
	 tofAnaTestbeam->SetMrpcRef(3);      // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(16.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(18.);    // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.6);   // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChi2Lim(10.);    // initialization of Chi2 selection limit  
	
	 switch(iSel2){
	 case 4:
	   tofAnaTestbeam->SetTShift(3420.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(3500.);   // Shift Sel2 time peak to 0 
	   break;
	 case 7:
	   tofAnaTestbeam->SetChi2Lim(10.);      // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(3420.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(-170.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 37:                                 // upper part of setup: P2 - USTC
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 switch(iSel2){
	 case 4:
	   tofAnaTestbeam->SetTOffD4(12000.);  // initialization
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-150.);   // Shift Sel2 time peak to 0 
	   break;
	 case 9:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-400.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 47:                                 // upper part of setup: P2 - USTC
	 tofAnaTestbeam->SetDut(4);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(1.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);   // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(450.);   // Shift Sel2 time peak to 0 
	   break;

	 case 9:
	   tofAnaTestbeam->SetChS2Sel(11.5);    // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(1.);     // Width  of channel selection window
	   tofAnaTestbeam->SetPosYS2Sel(0.2);   // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(350.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 97:                                 // upper part of setup: THU - USTC
	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	 tofAnaTestbeam->SetChi2Lim(1000.);     // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetSel2TOff(450.);   // Shift Sel2 time peak to 0 
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-2500.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(-2180.);   // Shift Sel2 time peak to 0 
	   break;
	 case 4:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(1100.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(1300.);   // Shift Sel2 time peak to 0 	   
	   break;
	 }
	 break;

   case 39:                                 // upper part of setup: P2 - THUstrip
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);    // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(12.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(111.5);   // Width  of channel selection window
	 switch(iSel2){
	 case 4:
	   tofAnaTestbeam->SetTOffD4(12000.);  // initialization
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(2300.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(2400.);   // Shift Sel2 time peak to 0 
	   break;
	 case 7:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   if(iRSel==5){
	    tofAnaTestbeam->SetSel2TOff(1200.);   // Shift Sel2 time peak to 0 
	    tofAnaTestbeam->SetTShift(-9750.);     // Shift D vs 4 (BRef vs MRef)	  
	   }
	   else{
	    tofAnaTestbeam->SetSel2TOff(1200.);   // Shift Sel2 time peak to 0 
	    tofAnaTestbeam->SetTShift(800.);      // Shift D vs 4 (BRef vs MRef)
	   }
	   break;
	 }
	 break;

   case 49:                                 // upper part of setup: P5 - THUstrip
	 tofAnaTestbeam->SetDut(4);         // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);     // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(11.5);      // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(0.7);      // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);     // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetPosY4SelOff(-1.5); // Y Position selection offset in cm 	
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetDTD4MAX(2000.);     // initialization of Max time difference Ref - BRef
	   tofAnaTestbeam->SetChi2Lim(2.);        // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(180.);     // Shift Sel2 time peak to 0 
	   tofAnaTestbeam->SetPosYS2Sel(0.1);     // Y Position selection in fraction of strip length
	   tofAnaTestbeam->SetPosYS2SelOff(-1.5); // Y Position selection offset in cm 	
	   tofAnaTestbeam->SetChS2Sel(15.5);      // Center of channel selection window
	   tofAnaTestbeam->SetDChS2Sel(0.7);      // Width  of channel selection window
	   tofAnaTestbeam->SetTOffD4(12000.);     // tuning 
	   tofAnaTestbeam->SetDXWidth(0.8);
	   tofAnaTestbeam->SetDYWidth(0.6);
	   tofAnaTestbeam->SetDTWidth(100.);    // in ps
	   break;
	 case 7:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(-100.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;

   case 79:                                 // upper part of setup: P2 - THUstrip
	 tofAnaTestbeam->SetDut(7);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);    // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(12.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(12.);   // Width  of channel selection window
	 switch(iSel2){
	 case 4:
	   tofAnaTestbeam->SetTOffD4(12000.);  // initialization
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetSel2TOff(40.);   // Shift Sel2 time peak to 0 
	   break;
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(10.);       // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-1300.);     // Shift D vs 4 (BRef vs MRef)	  
	   tofAnaTestbeam->SetSel2TOff(-1080.);   // Shift Sel2 time peak to 0 
	   break;
	 }
	 break;
case 1:                                    // lower part of setup: Buc2013 - BucRef
   case 61:
	 tofAnaTestbeam->SetDut(6);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(30.0);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(15.1);    // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length	   
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetTOffD4(13000.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetChi2Lim(5.);      // initialization of Chi2 selection limit  
	 break;

   case 81:
	 tofAnaTestbeam->SetDut(8);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(30.0);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(15.1);    // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length	   
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetTOffD4(13000.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetSel2TOff(102.);   // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetChS2Sel(26.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDChS2Sel(12.);    // Width  of channel selection window
	 tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length	   
	 tofAnaTestbeam->SetChi2Lim(5.);      // initialization of Chi2 selection limit  
	 break;

   case 16:
	 tofAnaTestbeam->SetDut(1);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(6);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(26.5);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(10.);     // Width  of channel selection window
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetSel2TOff(-90.);  // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetTOffD4(11000.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChS2Sel(4.);      // Center of channel selection window
	 tofAnaTestbeam->SetDChS2Sel(1.1);    // Width  of channel selection window
	 tofAnaTestbeam->SetChi2Lim(5.);      // initialization of Chi2 selection limit  
	 break;

   case 86:
	 tofAnaTestbeam->SetDut(8);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(6);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(26.5);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(12.);     // Width  of channel selection window
	 tofAnaTestbeam->SetTShift(-9500.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetTOffD4(-500.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetSel2TOff(-115.);  // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetChi2Lim(5.);      // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChS2Sel(30.);     // Center of channel selection window
	 tofAnaTestbeam->SetDChS2Sel(15.1);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length	   
	 break;

   case 18:
	 tofAnaTestbeam->SetDut(1);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(8);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(4.0);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(1.1);     // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetSel2TOff(80.);    // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetTOffD4(11000.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetChS2Sel(26.);      // Center of channel selection window
	 tofAnaTestbeam->SetDChS2Sel(12.);     // Width  of channel selection window
	 tofAnaTestbeam->SetPosYS2Sel(0.5);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetChi2Lim(5.);       // initialization of Chi2 selection limit  
	 break;

   case 68:
	 tofAnaTestbeam->SetDut(6);           // Device under test   
	 tofAnaTestbeam->SetMrpcRef(8);       // Reference RPC     
	 tofAnaTestbeam->SetPlaSelect(2);     // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(4.0);     // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(1.1);     // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.1);    // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetTShift(2000.);    // Shift DTD4 to 0
	 tofAnaTestbeam->SetSel2TOff(-20.);    // Shift Sel2 time peak to 0 
	 tofAnaTestbeam->SetTOffD4(11000.);   // Shift DTD4 to physical value 	 
	 tofAnaTestbeam->SetChS2Sel(30.);     // Center of channel selection window
	 tofAnaTestbeam->SetDChS2Sel(15.1);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length	   
	 tofAnaTestbeam->SetChi2Lim(5.);       // initialization of Chi2 selection limit  
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
   cout << " Initialize TSHIFT to "<<tofAnaTestbeam->GetTShift()<<endl;
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

  FairEventManager *fMan= new FairEventManager();
  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle );// kFullSquare);
  fMan->AddTask(TofHits);   
  CbmEvDisTracks *Track =  new CbmEvDisTracks ("Tof Tracks",1);
  fMan->AddTask(Track);
  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(0);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for(Int_t i=0; i<allvolumes->GetEntries(); i++){
    TGeoVolume* vol     = (TGeoVolume*)allvolumes->At(i);
    TString name = vol->GetName();
    //cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    if( name == "counter"){
      //cout << " counter found " << i << " set transparent "<<endl;
      vol->SetTransparency(70);
    }
  }
  //  gGeoManager->SetVisLevel(3);
  //  top->SetTransparency(80); 
  //  top->Draw("ogl");

  //  fMan->Init(1,4,10000);                     
  fMan->Init(1); 
                    
  cout << "gEve "<< gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow-10);
  //  gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);

  // -----   Intialise and run   --------------------------------------------
  /*
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0, nEvents);

  // ------------------------------------------------------------------------
  // default display 
  TString Display_Status = "pl_over_Mat04D4best.C";
  TString Display_Funct = "pl_over_Mat04D4best()";  
  gROOT->LoadMacro(Display_Status);
  gInterpreter->ProcessLine(Display_Funct);

  gROOT->LoadMacro("pl_over_MatD4sel.C");
  gInterpreter->ProcessLine("pl_over_MatD4sel()");
   */
}
