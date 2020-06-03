void dis_hits(Int_t nEvents=10, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="Cern", char *cSet="345", Int_t iSel2=0, Int_t iTrackingSetup=0) 
{
   Int_t iVerbose = 1;
   // Specify log level (INFO, DEBUG, DEBUG1, ...)
   TString logLevel = "FATAL";
   //TString logLevel = "ERROR";
   TString logLevel = "INFO";
   TString logLevel = "DEBUG"; 
   TString logLevel = "DEBUG1";
   //TString logLevel = "DEBUG2";
   //TString logLevel = "DEBUG3";
   FairLogger* log;  

   TString workDir       = gSystem->Getenv("VMCWORKDIR");
   TString paramDir      = workDir  + "/macro/tof/beamtime/nov15";
   TString ParFile       = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile     = paramDir + "/unpack_" + cFileId + ".out.root";
   TString InputDigiFile = paramDir + "/digi_" + cFileId + Form("_%s",cSet) + ".out.root";
   TString OutputFile    = paramDir + "/dishits_" + cFileId + Form("_%s_%06d_%03d",cSet,iSel,iSel2) + ".out.root";
   TString cAnaFile=Form("%s_%s_%06d_%03d_tofAnaTestBeam.hst.root",cFileId,cSet,iSel,iSel2);
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
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(9);
     tofFindTracks->SetStation(0, 5, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 0, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 9, 1, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(5, 9, 1, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(6, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(7, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(8, 3, 0, 0);           // upper part of Nov15 setup 
     tofTrackFinder->SetSIGT(100000.);                 // in ps
     break;

   case 1:                                       // calibration mode
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 9, 0, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 1, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 3, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetT0MAX(50000.);                 // in ps
     tofTrackFinder->SetSIGT(10000.);                  // in ps
     break;

   case 2:                                       // calibration mode
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(5);
     tofFindTracks->SetStation(0, 5, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 3, 0, 0);           // upper part of Nov15 setup 
     tofTrackFinder->SetSIGT(2000.);                 // in ps
     break;

   case 3:                                       // calibration mode
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStation(0, 5, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 3, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetT0MAX(50000.);                 // in ps
     tofTrackFinder->SetSIGT(1000.);                 // in ps
     break;

   case 4:                                       // calibration mode
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(3);
     tofFindTracks->SetStation(0, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 3, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetT0MAX(2000.);                 // in ps
     tofTrackFinder->SetSIGT(200.);                  // in ps
     break;

   case 5:                                       // calibration mode
     tofFindTracks->SetMinNofHits(2);
     tofFindTracks->SetNStations(7);
     tofFindTracks->SetStation(0, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 9, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 0, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 1, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 9, 1, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(5, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(6, 9, 2, 1);           // upper part of Nov15 setup 
     tofTrackFinder->SetSIGT(100000.);                 // in ps
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
   case 921920:  
	 tofAnaTestbeam->SetCh4Sel(16.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(20.);   // Width  of channel selection window
	 //tofAnaTestbeam->SetChi2Lim(6.);     // initialization of Chi2 selection limit  
	 tofAnaTestbeam->SetTOffD4(13000.);  // initialization
	 //	 tofAnaTestbeam->SetTShift(-2000.);  // initialization
	 switch(iSel2){
	 case 3:
	   tofAnaTestbeam->SetChi2Lim(10.);     // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetTShift(-600.);       // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16000.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);  // Shift Sel2 time peak to 0
	   break;

	 case 4:
	   break
	     ;
	 default:
	   ;
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
