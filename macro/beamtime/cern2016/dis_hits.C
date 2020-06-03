void dis_hits(Int_t nEvents=10, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="Cern", char *cSet="345", Int_t iSel2=0, Int_t iTrackingSetup=0, Double_t dScalFac=1., Double_t dChi2Lim2=50.) 
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
   //TString logLevel = "DEBUG4";
   FairLogger* log;  

   TString workDir       = gSystem->Getenv("VMCWORKDIR");
   TString paramDir      = workDir  + "/macro/beamtime/cern2016";
   TString ParFile       = paramDir + "/data/" + cFileId + ".params.root";
   TString InputFile     = paramDir + "/data/" + cFileId + ".root";
   TString InputDigiFile = paramDir + "/data/digi_" + cFileId + Form("_%s",cSet) + ".out.root";
   TString OutputFile    = paramDir + "/data/dishits_" + cFileId + Form("_%s_%06d_%03d",cSet,iSel,iSel2) + ".out.root";
   TString cAnaFile=Form("%s_%s_%06d_%03d_tofAnaTestBeam.hst.root",cFileId,cSet,iSel,iSel2);
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",cFileId);

   cout << " InputDigiFile = "
	<< InputDigiFile
	<< endl;

   TList *parFileList = new TList();

   TString TofGeo="v16b";  //default

   cout << "Geometry version "<<TofGeo<<endl;

   TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   parFileList->Add(&tofDigiFile);   

   // TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   // TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
   TObjString tofDigiBdfFile = workDir  + "/parameters/tof/" + TofGeo +".digibdf.par";
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

  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

   // =========================================================================
   // ===                       Tracking                                    ===
   // =========================================================================
   CbmStsDigitize* stsDigitize = new CbmStsDigitize(); //necessary for kalman !!
   CbmKF* kalman = new CbmKF();
   if(1){
   CbmTofTrackFinder* tofTrackFinder= new CbmTofTrackFinderNN();
   tofTrackFinder->SetMaxTofTimeDifference(5.);    // in ns/cm 
   tofTrackFinder->SetTxLIM(0.05);                 // max slope dx/dz
   tofTrackFinder->SetTyLIM(0.05);                 // max dev from mean slope dy/dz
   tofTrackFinder->SetTyMean(-0.13);               // mean slope dy/dz

   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);                // initialize Minuit
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
   tofTrackFinder->SetSIGLIM(dChi2Lim2);         // matching window in multiples of chi2
   tofTrackFinder->SetChiMaxAccept(dChi2Lim2);   // max matching chi2

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
     tofFindTracks->SetStation(1, 4, 0, 0);           // HD 
     tofFindTracks->SetStation(2, 9, 0, 1);           // USTC 
     tofFindTracks->SetStation(3, 9, 1, 0);           // USTC 
     tofFindTracks->SetStation(4, 9, 1, 1);           // USTC
     tofFindTracks->SetStation(5, 9, 2, 1);           // THU
     tofFindTracks->SetStation(6, 6, 0, 0);           // BUC
     tofFindTracks->SetStation(7, 6, 0, 1);           // BUC
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


   default:
     cout << "Tracking setup "<<iTrackingSetup<<" not implemented "<<endl;
     return;
     ;
   }
   tofFindTracks->PrintSetup();

   run->AddTask(tofFindTracks);
   }
   // =========================================================================
   // ===                       Analysis                                    ===
   // =========================================================================

   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   tofAnaTestbeam->SetCorMode(iGenCor); // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp 
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization

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

   //tofAnaTestbeam->SetTShift(-28000.);// initialization
   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetSel2TOff(0.);   // Shift Sel2 time peak to 0 
   tofAnaTestbeam->SetChi2Lim(0.5*dChi2Lim2);    // initialization of Chi2 selection limit  
   tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);   // initialization of Chi2 selection limit for Mref-Sel2 pair   

   Int_t iRSel=0;
   Int_t iRSelSm=0;
   Int_t iRSelRpc=0;
   Int_t iRSelin=iRSel;
   if(iSel2>=0){
     iRSel=5;      // use diamond
     iRSelSm=0;
     iRSelin=500;
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
     iRSelin=iRSel;
     iRSelRpc=iRSel%10;
     iRSel = (iRSel-iRSelRpc)/10;
     iRSelSm=iRSel%10;
     iRSel = (iRSel-iRSelSm)/10;
   }

   tofAnaTestbeam->SetBeamRefSmType(iRSel); // common reaction reference 
   tofAnaTestbeam->SetBeamRefSmId(iRSelSm);
   tofAnaTestbeam->SetBeamRefRpc(iRSelRpc);

   Int_t iSel2in = iSel2;
   Int_t iSel2Rpc= iSel2%10;
   iSel2=(iSel2-iSel2Rpc)/10;
   Int_t iSel2Sm=iSel2%10;
   iSel2=(iSel2-iSel2Sm)/10;
   if(iSel2 > 0) {
     tofAnaTestbeam->SetMrpcSel2(iSel2);           // initialization of second selector Mrpc Type 
     tofAnaTestbeam->SetMrpcSel2Sm(iSel2Sm);       // initialization of second selector Mrpc SmId
     tofAnaTestbeam->SetMrpcSel2Rpc(iSel2Rpc);     // initialization of second selector Mrpc RpcId
   }

   tofAnaTestbeam->SetSIGLIM(30.);               // max matching chi2
   tofAnaTestbeam->SetSIGT(0.2);                 // in ns
   tofAnaTestbeam->SetSIGX(1.);                  // in cm
   tofAnaTestbeam->SetSIGY(1.5);                 // in cm
   tofAnaTestbeam->SetChi2Lim(0.5*dChi2Lim2);    // initialization of Chi2 selection limit  
   tofAnaTestbeam->SetChi2Lim2(dChi2Lim2);       // initialization of Chi2 selection limit for Mref-Sel2 pair 

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

   cout<< "dispatch iSel = "<<iSel<<", iSel2in = "<<iSel2in<<" iRSelin = "<<iRSelin<<endl;

   switch (iSel) {
   case 200910:  
   case 201910:  
   case 400910:  
   case 600910:  
   case 601910:  
   case 800910:  
   case 810910:  
   case 900910:  
   case 901910:  
   case 911910:  
   case 921910:  
     switch (iRSelin){
       case 500:	 
	   tofAnaTestbeam->SetTShift(-0.6);    // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(14.);   // Shift DTD4 to physical value
	   switch(iSel2in){
	   case 600:
	     tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	     break;
	   case 901:
	     tofAnaTestbeam->SetSel2TOff(0.62);     // Shift Sel2 time peak to 0
	     break;
	   case 921:
	     tofAnaTestbeam->SetSel2TOff(-0.24);     // Shift Sel2 time peak to 0
	     break;

	   default:
	     ;   
	   }
	   break;

        default:
       ;
     }
     tofAnaTestbeam->SetCh4Sel(15);                // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(15*dScalFac);      // Width  of channel selection window
     break;

   default:
     cout << "Better to define analysis setup! Running with default offset parameter... "<< endl;
     // return;
   }  // end of different subsets

   cout << " Initialize TSHIFT to "<<tofAnaTestbeam->GetTShift()<<endl;
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
  CbmPixelHitSetDraw *TofUHits = new CbmPixelHitSetDraw ("TofUHit", kRed, kOpenCross );
  fMan->AddTask(TofUHits); 
  CbmEvDisTracks *Tracks =  new CbmEvDisTracks ("Tof Tracks",1);
  Tracks->SetVerbose(4);
  fMan->AddTask(Tracks);
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
  //gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);
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
