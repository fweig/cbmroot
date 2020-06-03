void ana_trks(Int_t nEvents=10, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="Cern", char *cSet="345", Int_t iSel2=0, Int_t iTrackingSetup=0, Double_t dScalFac=10.) 
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

   TString workDir       = gSystem->Getenv("VMCWORKDIR");
   TString paramDir      = workDir  + "/macro/tof/beamtime/nov15";
   TString ParFile       = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile     = paramDir + "/unpack_" + cFileId + ".out.root";
   TString InputDigiFile = paramDir + "/digi_" + cFileId + Form("_%s",cSet) + ".out.root";
   TString OutputFile    = paramDir + "/hits_" + cFileId + Form("_%s_%06d_%03d",cSet,iSel,iSel2) + ".out.root";
   TString cAnaFile=Form("%s_%s_%06d_%03d_TrkAnaTestBeam.hst.root",cFileId,cSet,iSel,iSel2);
   TString cHstFile=paramDir + Form("/hst/%s_%s_%06d_%03d_trk%03dAna.hst.root",cFileId,cSet,iSel,iSel2,iTrackingSetup);
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId);

   cout << " InputDigiFile = "
	<< InputDigiFile
	<< endl;

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernNov2015.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v15c";  //default
   TString FId=cFileId;
   if(FId.Contains("CernSps02Mar")){
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

   // TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   // TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
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
   tofTrackFinder->SetMaxTofTimeDifference(5000.); // in ps/cm 
   tofTrackFinder->SetTxLIM(0.05);                 // max slope dx/dz
   tofTrackFinder->SetTyLIM(0.05);                  // max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(0.1);                 // mean slope dy/dz

   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);                // initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2,3,4,5,6, 10 - 19
   tofFindTracks->SetTtTarg(37.);                // target value for inverse velocity, > 33.3 !
   tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name
     
   tofFindTracks->SetT0MAX(dScalFac*1000.);       // in ps
   tofFindTracks->SetSIGT(100.);                  // default in ps
   tofFindTracks->SetSIGX(0.5);                   // default in cm
   tofFindTracks->SetSIGY(1.5);                   // default in cm 
   tofFindTracks->SetSIGZ(0.5);                   // default in cm 
   tofTrackFinder->SetSIGLIM(500.);                // matching window in multiples of chi2
   tofTrackFinder->SetChiMaxAccept(40.);          // max matching chi2

   switch (iTrackingSetup){
   case 0:                                       // bypass mode
     tofFindTracks->SetMinNofHits(-1);
     tofFindTracks->SetNStations(1);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     break;

   case 1:                                       // calibration mode
     tofFindTracks->SetMinNofHits(6);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 4, 0, 0);           // P5 
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(4, 9, 1, 0);           // 
     //tofFindTracks->SetStation(4, 9, 1, 1);         // broken
     tofFindTracks->SetStation(5, 9, 0, 0);           //  
     tofFindTracks->SetStation(6, 9, 0, 1);           //  
     tofFindTracks->SetStation(7, 3, 0, 0);           // P2 
     tofTrackFinder->SetSIGT(200.);                   // in ps
     break;

   case 2:                                       // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(6);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(2, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(3, 9, 1, 0);           // 
     tofFindTracks->SetStation(4, 9, 0, 0);           //  
     tofFindTracks->SetStation(5, 9, 0, 1);           // 
     tofTrackFinder->SetSIGT(150.);                  // in ps
     break;

   case 3:                                            // calibration debugging mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStation(0, 4, 0, 0);           //  
     tofFindTracks->SetStation(1, 9, 2, 0);           //  
     tofFindTracks->SetStation(2, 9, 2, 1);           // 
     break;

   case 4:                                            // calibration mode
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 9, 0, 0);           //  
     tofFindTracks->SetStation(1, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(2, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     break;

   case 5:                                            // calibration mode
     tofFindTracks->SetMinNofHits(5);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 4, 0, 0);           // P2 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofFindTracks->SetStation(4, 9, 0, 0);           //  
     break;

   case 6:                                            // calibration mode
     tofFindTracks->SetMinNofHits(6);
     tofFindTracks->SetNStations(6);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 0, 1);           // 
     tofFindTracks->SetStation(4, 9, 0, 0);           //  
     tofFindTracks->SetStation(5, 4, 0, 0);           // P2 
     break;

   case 10:                                       // full lower setup
     tofFindTracks->SetMinNofHits(6);
     tofFindTracks->SetNStations(16);
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
     tofFindTracks->SetStation(14, 2, 0, 0);           // Ceramic      
     tofFindTracks->SetStation(15, 2, 1, 0);           // Ceramic  
     break;

   case 11:                                       // partial lower setup
     tofFindTracks->SetMinNofHits(4);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(2, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(3, 1, 0, 0);           // BucRef  
     break;

   case 111:                                         // show case 
     tofTrackFinder->SetTyLIM(0.2);                  // max dev from mean slope dy/dz
     tofFindTracks->SetMinNofHits(5);
     tofFindTracks->SetNStations(23);
     tofFindTracks->SetStation(0, 5, 0, 0);           // Diamond 
     tofFindTracks->SetStation(1, 4, 0, 0);           // P5 
     tofFindTracks->SetStation(2, 9, 2, 0);           // USTC 
     tofFindTracks->SetStation(3, 9, 2, 1);           // USTC
     tofFindTracks->SetStation(4, 9, 1, 0);           // 
     //tofFindTracks->SetStation(4, 9, 1, 1);           // broken
     tofFindTracks->SetStation(5, 9, 0, 0);           //  
     tofFindTracks->SetStation(6, 9, 0, 1);           //  
     tofFindTracks->SetStation(7, 3, 0, 0);           // P2 

     tofFindTracks->SetStation(8, 8, 0, 2);           // THUpad 
     tofFindTracks->SetStation(9, 8, 0, 3);           // THUpad
     tofFindTracks->SetStation(10, 8, 0, 0);           // THUpad
     tofFindTracks->SetStation(11, 8, 0, 1);           // THUpad
     tofFindTracks->SetStation(12, 8, 0, 4);           // THUpad
     tofFindTracks->SetStation(13, 8, 0, 5);           // THUpad
     tofFindTracks->SetStation(14, 6, 0, 0);           // Buc2015
     tofFindTracks->SetStation(15, 6, 0, 1);           // Buc2015
     tofFindTracks->SetStation(16, 7, 0, 0);           // Buc2012
     tofFindTracks->SetStation(17, 7, 0, 1);           // Buc2012
     tofFindTracks->SetStation(18, 7, 0, 2);           // Buc2012
     tofFindTracks->SetStation(19, 7, 0, 3);           // Buc2012
     tofFindTracks->SetStation(20, 1, 0, 0);           // BucRef      
     tofFindTracks->SetStation(21, 2, 0, 0);           // Ceramic      
     tofFindTracks->SetStation(22, 2, 1, 0);           // Ceramic     
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
   tofAnaTestbeam->SetMul4Max(20);     // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(20);     // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetTOffD4(10000.);  // initialization
   tofAnaTestbeam->SetDTD4MAX(6000.);  // initialization of Max time difference Ref - BRef

   //tofAnaTestbeam->SetTShift(-28000.);// initialization
   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetSel2TOff(450.);   // Shift Sel2 time peak to 0 

   Int_t iRSel=0;
   Int_t iRSelSm=0;
   Int_t iRSelRpc=0;
   if(iSel2>=0){
     iRSel=5;      // use diamond
     iRSelSm=0;
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
     iRSelRpc=iRSel%10;
     iRSel = (iRSel-iRSelRpc)/10;
     iRSelSm=iRSel%10;
     iRSel = (iRSel-iRSelSm)/10;
   }

   tofAnaTestbeam->SetBeamRefSmType(iRSel); // common reaction reference 
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);

   Int_t iSel2Rpc= iSel2%10;
   iSel2=(iSel2-iSel2Rpc)/10;
   Int_t iSel2Sm=iSel2%10;
   iSel2=(iSel2-iSel2Sm)/10;
   if(iSel2 > 0) {
     tofAnaTestbeam->SetMrpcSel2(iSel2);           // initialization of second selector Mrpc Type 
     tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);       // initialization of second selector Mrpc SmId
     tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);     // initialization of second selector Mrpc RpcId
   }

   tofAnaTestbeam->SetSIGLIM(3.);                // max matching chi2
   tofAnaTestbeam->SetSIGT(100.);                // in ps
   tofAnaTestbeam->SetSIGX(1.);                  // in cm
   tofAnaTestbeam->SetSIGY(1.);                  // in cm

   Int_t iRef = iSel %1000;
   Int_t iDut = (iSel - iRef)/1000;
   Int_t iDutRpc = iDut%10;
   iDut = (iDut - iDutRpc)/10;
   Int_t iDutSm = iDut%10;
   iDut = (iDut - iDutSm)/10;
   Int_t iRefRpc = iRef%10;
   iRef = (iRef - iRefRpc)/10;
   Int_t iRefSm = iRef%10;
   iRef = (iRef - iRefSm)/10;

   tofAnaTestbeam->SetDut(iDut);              // Device under test   
   tofAnaTestbeam->SetDutSm(iDutSm);          // Device under test   
   tofAnaTestbeam->SetDutRpc(iDutRpc);        // Device under test   
   tofAnaTestbeam->SetMrpcRef(iRef);          // Reference RPC     
   tofAnaTestbeam->SetMrpcRefSm(iRefSm);      // Reference RPC     
   tofAnaTestbeam->SetMrpcRefRpc(iRefRpc);    // Reference RPC  

   cout<< "dispatch iSel = "<<iSel<<", iSel2 = "<<iSel2<<endl;

   switch (iSel) {
   case 0:                                 // upper part of setup: P2 - P5
   case 400300:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	   tofAnaTestbeam->SetTShift(-3000.);     // Shift DTD4 to 0
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

     tofAnaTestbeam->SetChi2Lim(30.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.5);
     tofAnaTestbeam->SetDYWidth(1.5);
     tofAnaTestbeam->SetDTWidth(120.);   // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(3);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.25);  // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMul0Max(1);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(3);      // Max Multiplicity in Ref - RPC 
     break;

   case 921300:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	   tofAnaTestbeam->SetTShift(-17000.);     // Shift DTD4 to 0
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

     tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.5);
     tofAnaTestbeam->SetDYWidth(1.5);
     tofAnaTestbeam->SetDTWidth(120.);   // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(20);     // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(10.5);   // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(3);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(300);      // Max Multiplicity in Ref - RPC 
     break;

   case 300920:  
   case 921920:  
     tofAnaTestbeam->SetCh4Sel(16.5);    // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(20.);   // Width  of channel selection window
     tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetTOffD4(13000.);  // initialization
     //	 tofAnaTestbeam->SetTShift(-2000.);  // initialization
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){
       case 5:	 
	 tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	 tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetTShift(-14000.);       // Shift DTD4 to 0
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim2(4.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2230.);     // Shift Sel2 time peak to 0
	   break;

	 case 4:
	   break
	     ;
	 default:
	   ;
	 }
	 break;

       case 3:
	 tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Bref    
	 tofAnaTestbeam->SetTShift(1950.);  // initialization
	 tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	 tofAnaTestbeam->SetSel2TOff(2250.);  // Shift Sel2 time peak to 0
	 break;
       default:
	 cout << "Define setup! "<< endl;
	 return;
	 ;
     }
     break;


   case 300921:  
   case 400921:  
   case 900921:  
   case 920921:  
	 tofAnaTestbeam->SetCh4Sel(16.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(20.);   // Width  of channel selection window
	 //tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  

	 cout << "Run with iRSel = "<<iRSel<<endl;
	 switch (iRSel){
	 case 5:
	   tofAnaTestbeam->SetTShift(2500.);  // initialization
	   tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	   switch(iSel2){
	   case 3:
	     tofAnaTestbeam->SetChi2Lim2(4.);     // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(2000.);  // Shift Sel2 time peak to 0
	     break;

	   case 4:
	     tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetTShift(-700.);       // Shift DTD4 to 0
	     tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	     tofAnaTestbeam->SetSel2TOff(-700.);  // Shift Sel2 time peak to 0
	     break;

	   case 9:
	     tofAnaTestbeam->SetChi2Lim(4.);      // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(-170.);  // Shift Sel2 time peak to 0
	     break;

	   default:
	     ;
	   }
	   break;

	 case 3:
	   tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
	   tofAnaTestbeam->SetTShift(1950.);  // initialization
	   tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	   tofAnaTestbeam->SetSel2TOff(2070.);  // Shift Sel2 time peak to 0
	   break;

	 default:
	   cout << "Define setup! "<< endl;
	   return;
	   ;
	 }
	 tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetDXWidth(1.5);
	 tofAnaTestbeam->SetDYWidth(1.5);
	 tofAnaTestbeam->SetDTWidth(120.);   // in ps
	 break;

   case 900400:
   case 901400:
   case 910400:
   case 920400:
   case 921400:
     //defaults first
     tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.5);
     tofAnaTestbeam->SetDYWidth(1.5);
     tofAnaTestbeam->SetDTWidth(120.);   // in ps
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond / Bref   
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){
         case 3:
	   tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(2500.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(0.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2760.);     // Shift Sel2 time peak to 0
	   break;
	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	   tofAnaTestbeam->SetTShift(-7700.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
	   switch(iSel2){
	   case 3:
	     tofAnaTestbeam->SetChi2Lim2(4.);   // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(2840.);     // Shift Sel2 time peak to 0
	     break;
	   default:
	     ;
	   }
	   break;

         case 9:
	   tofAnaTestbeam->SetChi2Lim(100.);   // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetMulDMax(3);      // Max Multiplicity in BeamRef // Diamond    
	   tofAnaTestbeam->SetTShift(100.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(500.);     // Shift Sel2 time peak to 0
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   break;

         default:
	   cout << "Define setup! "<< endl;
	   return;
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetCh4Sel(8);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(10);     // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.5);   // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;

   case 100600:
   case 601600:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(0.) //-18300.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   switch(iSel2){
	   case 0:
	     break;
	   case 1:
	     tofAnaTestbeam->SetChi2Lim2(10.);   // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(3000.);     // Shift Sel2 time peak to 0
	     break;
	   case 6:
	     tofAnaTestbeam->SetChi2Lim2(10.);   // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(85.);     // Shift Sel2 time peak to 0
	     break;
	   default:
	     ;
	   }      // iSel2 switch end 
	   break;
         case 1:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(2500.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   break;
         case 6:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(2500.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   cout << "Define setup! "<< endl;
	   return;
     }
     tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(0.5);
     tofAnaTestbeam->SetDYWidth(1.);
     tofAnaTestbeam->SetDTWidth(100.);   // in ps
     //tofAnaTestbeam->SetCh4Sel(8);      // Center of channel selection window
     //tofAnaTestbeam->SetDCh4Sel(10);     // Width  of channel selection window
     //tofAnaTestbeam->SetPosY4Sel(0.5);   // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;

   case 100601:
   case 600601:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-18300.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   switch(iSel2){
	   case 0:
	     break;
	   case 1:
	     tofAnaTestbeam->SetChi2Lim(4.);   // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(3000.);     // Shift Sel2 time peak to 0
	     break;
	   case 6:
	     tofAnaTestbeam->SetChi2Lim2(4.);   // initialization of Chi2 selection limit  
	     tofAnaTestbeam->SetSel2TOff(-50.);     // Shift Sel2 time peak to 0
	     break;
	   default:
	     ;
	   }      // iSel2 switch end 
	   break;
         case 1:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(2500.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   break;
         case 6:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(4.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-500.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   cout << "Define setup! "<< endl;
	   return;
     }
     tofAnaTestbeam->SetDXWidth(0.5);
     tofAnaTestbeam->SetDYWidth(1.);
     tofAnaTestbeam->SetDTWidth(100.);   // in ps
     //tofAnaTestbeam->SetCh4Sel(8);      // Center of channel selection window
     //tofAnaTestbeam->SetDCh4Sel(10);     // Width  of channel selection window
     //tofAnaTestbeam->SetPosY4Sel(0.5);   // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;

   case 700100:
   case 701100:
   case 702100:
   case 703100:
   case 800100:
   case 801100:
   case 802100:
   case 803100:
   case 804100:
   case 805100:
   case 600100:
   case 601100:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-21300.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(2900.);     // Shift Sel2 time peak to 0
	   switch(iSel2){
	   case 0:
	     break;
	   case 6:
	     tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
	     switch(iSel2Rpc){
	     case 0:
	       tofAnaTestbeam->SetSel2TOff(-3000.);     // Shift Sel2 time peak to 0 // 601
	     case 1:
	       tofAnaTestbeam->SetSel2TOff(-2920.);     // Shift Sel2 time peak to 0 // 601
	       break;
	     }
	     break;
	   default:
	     ;
	   }      // iSel2 switch end 
	   break;
         case 6:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetChi2Lim(5.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-3000.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(17000.);      // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-2900.);   // Shift Sel2 time peak to 0 for 601
	   break;
         default:
	   cout << "Define setup! "<< endl;
	   return;
     }
     tofAnaTestbeam->SetDXWidth(0.7);
     tofAnaTestbeam->SetDYWidth(1.2);
     tofAnaTestbeam->SetDTWidth(100.);   // in ps
     //tofAnaTestbeam->SetCh4Sel(8);      // Center of channel selection window
     //tofAnaTestbeam->SetDCh4Sel(10);     // Width  of channel selection window
     //tofAnaTestbeam->SetPosY4Sel(0.5);   // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;
   }  // end of different subsets

   switch(cFileId){
     case "CernSps01Mar2203":
	 break;

     default:
	 ;
   }
   cout << " Initialize TSHIFT to "<<tofAnaTestbeam->GetTShift()<<endl;
   cout << " Initialize MinNofHits to "<<tofFindTracks->GetMinNofHits()<<endl;
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

   /*
  FairEventManager *fMan= new FairEventManager();
  CbmPixelHitSetDraw *TofHits =   new CbmPixelHitSetDraw ("TofHit", kRed, kFullSquare);
  fMan->AddTask(TofHits);   
  fMan->Init();                     
   */

  // -----   Intialise and run   --------------------------------------------
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
  gROOT->LoadMacro("pl_over_trk.C");
  gROOT->LoadMacro("pl_calib_trk.C");
  gROOT->LoadMacro("save_hst.C");

  gInterpreter->ProcessLine("pl_over_MatD4sel()");
  gInterpreter->ProcessLine("pl_calib_trk()");
  gInterpreter->ProcessLine("pl_over_trk(5)");
  TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
  gInterpreter->ProcessLine(SaveToHstFile);
}
