void ana_digi_nodia(Int_t nEvents = 10000, Int_t calMode=0, Int_t calSel=-1, Int_t calSmType=2, Int_t RefSel=0, char *cFileId="Trb24Feb2028", Int_t iSet=0, Bool_t bOut=0, Int_t iSel2=0) 
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
   TString OutputFile = paramDir + "/digi_"   + cFileId + Form("_%02d%1d",iSet,iSel2) + ".out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernFeb2015.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v14c";
   TString FId=cFileId;
   TString FPar="";
   if(FId.Contains("tsu")) {
     cout << FId << ": Analyse TSU counter "<< endl;
     TofGeo="v14c_tsu";
     FPar="tsu.";
   }

   TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   parFileList->Add(&tofDigiFile);   

   TObjString tofDigiBdfFile =  paramDir + "/tof." + FPar + "digibdf.par";
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
   tofTestBeamClust->SetCaldXdYMax(3.);         // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(20.);       // Max Counter Cluster Multiplicity for filling calib histos  
   tofTestBeamClust->SetCalSmType(calSmType);    // select detectors for walk correction 
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10000.);          // Tot upper limit for walk corection
   tofTestBeamClust->SetTotMin(1.); //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(5000.);      // effective lower Tot limit  in ps from peak position
   tofTestBeamClust->SetTotMean(2000.);          // Tot calibration target value in ps 
   tofTestBeamClust->SetMaxTimeDist(500.);       // default cluster range in ps 
   //tofTestBeamClust->SetMaxTimeDist(0.);         //Deb// default cluster range in ps 
   tofTestBeamClust->SetSel2Id(iSel2);

   tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
   tofTestBeamClust->SetBeamRefSm(0);
   tofTestBeamClust->SetBeamRefDet(0);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_set%1d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iSet,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s_set%1d.hst.root",cFileId,iSet);
   tofTestBeamClust->SetOutHstFileName(cOutFname);

   TString cAnaFile=Form("%s_%02d%1d_tofAnaTestBeam.hst.root",cFileId,iSet,iSel2);

   switch (calMode) {
   case 0:                                      // initial calibration 
     tofTestBeamClust->SetTotMax(100000.);      // 100 ns
     //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(2000000.); // in ps 
     tofTestBeamClust->PosYMaxScal(2000.);      // in % of length 
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 1:                                      // save offsets, update walks 
     tofTestBeamClust->SetTRefDifMax(5000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
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
     tofTestBeamClust->SetTRefDifMax(13000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10.);        //in % of length
     break;
   case 52:
   case 53:
     tofTestBeamClust->SetTRefDifMax(6000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4.);        //in % of length
     break;
   case 62:
   case 63:
     tofTestBeamClust->SetTRefDifMax(3000.);   // in ps 
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

   switch (iSet) {
   case 0:                                       // upper part of setup: P2 - P5
         tofTestBeamClust->SetDutId(3);
         tofTestBeamClust->SetSelId(4);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // default case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);
	 break;

   case 1:
	 tofTestBeamClust->SetDutId(6);
         tofTestBeamClust->SetSelId(1);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // default case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);
	 break;
   default:
     ;
   }
   run->AddTask(tofTestBeamClust);

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
   tofAnaTestbeam->SetMul0Max(10);   // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(3);    // Max Multiplicity in HDRef - RPC 
   tofAnaTestbeam->SetMulDMax(3);    // Max Multiplicity in Diamond    tofAnaTestbeam->SetHitDistMin(30.);  // initialization
   tofAnaTestbeam->SetTOffD4(15000.);   // initialization
   tofAnaTestbeam->SetMrpcSel2(iSel2);  // initialization of second selector Mrpc 
   tofAnaTestbeam->SetChi2Lim(20.);    // initialization of Chi2 selection limit  

   switch (iSet) {
   case 0:                                 // upper part of setup: P2 - P5
   case 3:                                 // upper part of setup: P2 - P5
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(4);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);

	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); // 
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(8.5);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window

	 break;

     case 1:                             // upper part of setup: Buc2013 - BucRef
     case 6:                             // lower part of setup: Buc2013 - BucRef
	 tofTestBeamClust->SetDutId(6);
	 tofTestBeamClust->SetSelId(1);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);

	 tofAnaTestbeam->SetDut(6);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); // P2
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(38.5);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(35.);  // Width  of channel selection window
	 break;

   case 7:                                 // upper part of setup: USTC - P5
	 tofTestBeamClust->SetDutId(7);
	 tofTestBeamClust->SetSelId(4);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);

	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); //
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(0);   // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(12.5);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);   // Width  of channel selection window
	 break;

   case 9:                                 // upper part of setup: THUstrip - P5
	 tofTestBeamClust->SetDutId(9);
	 tofTestBeamClust->SetSelId(4);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);
	 
	 tofAnaTestbeam->SetDut(9);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); // P2
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(0);   // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(12.5);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);   // Width  of channel selection window
	 break;

   case 37:                                 // upper part of setup: P2 - USTC
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(7);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);

	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(7);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); // P2
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(8.5);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 break;

   case 39:                                 // upper part of setup: P2 - THUstrip
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(9);
	 tofTestBeamClust->SetBeamRefType(iSel2);    // Test case 
	 tofTestBeamClust->SetBeamRefSm(0);
	 tofTestBeamClust->SetBeamRefDet(0);
	 tofTestBeamClust->SetBeamAddRefMul(-1);

	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(9);    // Reference RPC     
	 tofAnaTestbeam->SetBeamRefSmType(iSel2); // P2
	 tofAnaTestbeam->SetBeamRefSmId(0);
	 tofAnaTestbeam->SetPlaSelect(0);   // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 tofAnaTestbeam->SetCh4Sel(12.5);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(11.);   // Width  of channel selection window
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
  gInterpreter->ProcessLine(Display_Funct);

  gROOT->LoadMacro("pl_over_cluSel.C");
  gROOT->LoadMacro("pl_over_clu.C");

  switch(iSet){
  case 0:
  case 3:
  case 9:
  case 37:
  case 39:
    gInterpreter->ProcessLine("pl_over_clu(3)");
    gInterpreter->ProcessLine("pl_over_clu(4)");
    gInterpreter->ProcessLine("pl_over_clu(5)");
    gInterpreter->ProcessLine("pl_over_clu(7)");
    gInterpreter->ProcessLine("pl_over_clu(9)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,7)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,9)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,7)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,9)");
    break;

  case 1:
    gInterpreter->ProcessLine("pl_over_clu(1)");
    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(8)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
    break;

  default:
    ;
  }
}
