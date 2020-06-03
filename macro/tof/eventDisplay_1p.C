eventDisplay_1p(Int_t iTrackingSetup=1, Int_t iSys=0, Int_t iSel=1, Int_t iSel2=-3)
{
  switch(iSys){
  case 0:
    TString  InputFile     ="data/1p.mc.root";
    TString  InputFile2    ="data/1p.reco.root";
    TString  ParFile       ="data/1p.params.root";
    break;
  case 25:
    TString  InputFile     ="data/U25cen_1p.mc.root";
    TString  InputFile2    ="data/U25cen_1p.reco.root";
    TString  ParFile       ="data/U25cen_1p.params.root";
    break;
  default:
    ;
  }
  Int_t iVerbose = 0;
  TString logLevel = "INFO";
  TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";

   TString workDir = gSystem->Getenv("VMCWORKDIR");
   TString geoDir  = gSystem->Getenv("VMCWORKDIR");
   TString TofGeo="v15c";  //default
   TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
   TFile* fgeo = new TFile(geoFile);
   TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
   if (NULL == geoMan){
     cout << "<E> FAIRGeom not found in geoFile"<<endl;
     return;
   }
   TString cTrkFile=Form("%s_tofFindTracks.hst.root",TofGeo);
   TString cAnaFile=Form("%s_tofAnaTestBeam.hst.root",TofGeo);

   TList *parFileList = new TList();

   TString paramDir = gSystem->Getenv("VMCWORKDIR");
   paramDir += "/parameters";

   TObjString tofDigiFile = paramDir + "/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   parFileList->Add(&tofDigiFile);

   TObjString tofDigiBdfFile =  paramDir + "/tof/tof_" + TofGeo + ".digibdf.par";
   parFileList->Add(&tofDigiBdfFile);

   TString TofDigitizerBDFInputFile = paramDir + "/tof/test_bdf_input.root";

                                   
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  fRun->SetInputFile(InputFile.Data());
  fRun->AddFriend(InputFile2.Data());
  gLogger->SetLogScreenLevel(logLevel.Data());
  fRun->SetOutputFile("data/evDis.root");

  // -----   TOF digitizer   -------------------------------------------------
  /*
  CbmTofDigitizerBDF* tofDigitizerBdf = new CbmTofDigitizerBDF("TOF Digitizer BDF",iVerbose);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigi",            kTRUE);
  tofDigitizerBdf->SetOutputBranchPersistent("TofDigiMatchPoints", kTRUE);
  tofDigitizerBdf->SetInputFileName(TofDigitizerBDFInputFile);
  fRun->AddTask(tofDigitizerBdf);

  // Cluster/Hit builder
  CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer",iVerbose);
  tofSimpClust->SetOutputBranchPersistent("TofHit",          kTRUE);
  tofSimpClust->SetOutputBranchPersistent("TofDigiMatch",    kTRUE);
  fRun->AddTask(tofSimpClust);
  */
  // -----   TOF reconstruction   -------------------------------------------------

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
   tofTrackFinder->SetSIGLIM(40.);                 // max matching chi2
   tofTrackFinder->SetSIGT(100.);                // in ps
   tofTrackFinder->SetSIGX(1.);                  // in cm
   tofTrackFinder->SetSIGY(1.);                  // in cm
   CbmTofTrackFitter* tofTrackFitter= new CbmTofTrackFitterKF(0,211);
   TFitter *MyFit = new TFitter(1);              // initialize Minuit
   tofTrackFinder->SetFitter(tofTrackFitter);
   CbmTofFindTracks* tofFindTracks  = new CbmTofFindTracks("TOF Track Finder");
   tofFindTracks->UseFinder(tofTrackFinder);
   tofFindTracks->UseFitter(tofTrackFitter);
   Int_t iGenCor=1;
   tofFindTracks->SetCorMode(iGenCor);           // valid options: 0,1,2
   tofFindTracks->SetTtTarg(33.7);               // target value for inverse velocity, > 33.3 !
   tofFindTracks->SetCalParFileName(cTrkFile);   // Tracker parameter value file name  
   switch (iTrackingSetup){
   case 0:                                       // tracking mode
     tofFindTracks->SetMinNofHits(3);
     tofFindTracks->SetNStations(4);
     tofFindTracks->SetStations(3974);           // upper part of Nov15 setup 
     break;
   case 1:                                       // tracking mode
     tofFindTracks->SetMinNofHits(6);
     tofFindTracks->SetNStations(8);
     tofFindTracks->SetStation(0, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 9, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 0, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 1, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 9, 1, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(5, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(6, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(7, 3, 0, 0);           // upper part of Nov15 setup 
     break;
   case 2:                                       // tracking mode
     tofFindTracks->SetMinNofHits(7);
     tofFindTracks->SetNStations(9);
     tofFindTracks->SetStation(0, 5, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(1, 4, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(2, 9, 0, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(3, 9, 0, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(4, 9, 1, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(5, 9, 1, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(6, 9, 2, 0);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(7, 9, 2, 1);           // upper part of Nov15 setup 
     tofFindTracks->SetStation(8, 3, 0, 0);           // upper part of Nov15 setup 
     break;
   default:
     ;
   }
   tofFindTracks->PrintSetup();
   fRun->AddTask(tofFindTracks);

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
   tofAnaTestbeam->SetDTD4MAX(6000.);  // initialization of Max time difference Ref - BRef

   //tofAnaTestbeam->SetTShift(-28000.);// initialization
   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetSel2TOff(0.);   // Shift Sel2 time peak to 0 
   tofAnaTestbeam->SetTOffD4(0.);  // initialization

   Int_t iRSel=0;
   if(iSel2>=0){
     iRSel=5;   // use diamond
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
     //     tofAnaTestbeam->SetTShift(50.);  // initialization
   }
   tofAnaTestbeam->SetMrpcSel2(iSel2);     // initialization of second selector Mrpc 
   tofAnaTestbeam->SetBeamRefSmType(iRSel); // common reaction reference 
   tofAnaTestbeam->SetBeamRefSmId(0);
   tofAnaTestbeam->SetSIGLIM(30.);                // max matching chi2
   tofAnaTestbeam->SetSIGT(100.);                // in ps
   tofAnaTestbeam->SetSIGX(1.);                  // in cm
   tofAnaTestbeam->SetSIGY(1.);                  // in cm
   tofAnaTestbeam->SetChi2Lim(10000.);     // initialization of Chi2 selection limit  

  switch (iSel) {
   case 0:                                 // upper part of setup: P2 - P5
   case 1:
   case 900911:
	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetDutSm(0);      // Device under test   
	 tofAnaTestbeam->SetDutRpc(0);     // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);    // Reference RPC     
	 tofAnaTestbeam->SetMrpcRefSm(1);    // Reference RPC     
	 tofAnaTestbeam->SetMrpcRefRpc(1);   // Reference RPC     
	 tofAnaTestbeam->SetCh4Sel(8.5);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(60.);   // Width  of channel selection window
	 //	 tofAnaTestbeam->SetTShift(-2000.);  // initialization
	 switch(iSel2){
	 case 9:
	   tofAnaTestbeam->SetSel2TOff(-240.);   // Shift Sel2 time peak to 0 
	   break;
	 case 3:
	   //tofAnaTestbeam->SetTShift(-14000.);  // initialization of MrpcRef - Dia shift
	   if(iRSel == iSel2) {
	     tofAnaTestbeam->SetTShift(1420.);  // initialization of MrpcRef - Dia shift
	     tofAnaTestbeam->SetTOffD4(0.);   // shift TOF to phsical values 
	     tofAnaTestbeam->SetSel2TOff(1460.);   // Shift Sel2 time peak to 0 
	   }else {
	     tofAnaTestbeam->SetTShift(-13400.);  // initialization of MrpcRef - Dia shift
	     tofAnaTestbeam->SetTOffD4(0.);   // shift TOF to phsical values 
	     tofAnaTestbeam->SetSel2TOff(1460.);   // Shift Sel2 time peak to 0 
	   }
	   break;
	 default:
	   ;
	 }
	 break;



         default:
	 ;
   }  // end of different subsets

   cout << " Initialize TSHIFT to "<<tofAnaTestbeam->GetTShift()<<endl;
   fRun->AddTask(tofAnaTestbeam);

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(ParFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();

  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(1);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for(Int_t i=0; i<allvolumes->GetEntries(); i++){
    TGeoVolume* vol     = (TGeoVolume*)allvolumes->At(i);
    TString name = vol->GetName();
    cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    switch(name) {
    case "counter":
      vol->SetTransparency(99);
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
      vol->SetTransparency(99);
    }
  }
  /*
    if( name == "counter"){
      //cout << " counter found " << i <<", transparency "<<vol->GetTransparency()<<", set 70 "<<endl;
      vol->SetTransparency(70);
    }
    if (name.Contains("module")) {
      //cout << " module found " << i <<", transparency "<<vol->GetTransparency()<< " set 90 "<<endl;
      vol->SetTransparency(90);    
    } 
    if (name.Contains("glass")) {
      //cout << " glass found " << i <<", transparency "<<vol->GetTransparency()<< " set 90 "<<endl;
      vol->SetTransparency(90);    
    } 
    if (name.Contains("gas")) {
      //cout << " gas found " << i <<", transparency "<<vol->GetTransparency()<< " set 99 "<<endl;
      vol->SetTransparency(99);    
    } 
  }
  */   
  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  //  FairMCPointDraw *RichPoint =   new FairMCPointDraw ("RichPoint",kOrange,  kFullSquare);
  FairMCPointDraw *TofPoint =    new FairMCPointDraw ("TofPoint",kBlue,  kFullSquare);
//  FairMCPointDraw *TrdPoint= new FairMCPointDraw ("TrdPoint",kTeal,  kFullSquare);
  //FairMCPointDraw *EcalPoint =    new FairMCPointDraw ("EcalPoint",kYellow,  kFullSquare);
  //FairMCPointDraw *RefPlanePoint = new FairMCPointDraw ("RefPlanePoint",kPink,  kFullSquare);
  //FairMCPointDraw *StsPoint = new FairMCPointDraw ("StsPoint",kCyan,  kFullSquare);
   
  CbmPixelHitSetDraw *TofHits =   new CbmPixelHitSetDraw ("TofHit", kRed,  kFullCircle);
                                                              
  fMan->AddTask(Track);
  
  //fMan->AddTask(RichPoint);
  //fMan->AddTask(EcalPoint);   
  fMan->AddTask(TofPoint);   
  fMan->AddTask(TofHits);   
  //fMan->AddTask(TrdPoint);
  //fMan->AddTask(RefPlanePoint);
  //fMan->AddTask(StsPoint);
  CbmEvDisTracks *TTrack =  new CbmEvDisTracks ("Tof Tracks",1);
  fMan->AddTask(TTrack);

  fMan->Init(1,5,10000);
  //fMan->Init(1,5);
                
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow-10);

}
