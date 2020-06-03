void ana_digi(Int_t nEvents = 1000000, Int_t calMode=0, Int_t calSel=-1, Int_t calSmType=2, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iSet=0, Bool_t bOut=0) 
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
   TString paramDir   = workDir + "/macro/tof/beamtime/sep14";
   TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/digi_"   + cFileId + ".out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapGsiSep14.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v14a";
   TString FId=cFileId;
   TString FPar="";
   if(FId.Contains("tsu")) {
     cout << FId << ": Analyse TSU counter "<< endl;
     TofGeo="v14a_tsu";
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
   //tofTestBeamClust->SetCalParFileName("MbsTrbBeamtofTestBeamClust.hst.root");

   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(3.);         // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(4.);        // Max Counter Cluster Multiplicity for filling calib histos  
   tofTestBeamClust->SetdTRefMax(50000.);        // obsolete, not used any more
   tofTestBeamClust->SetCalSmType(calSmType);    // select detectors for walk correction 
   //tofTestBeamClust->SetTRefDetId(20486);      //0x00005006; big Diamond
   //tofTestBeamClust->SetTRefDetId(8214);       //0x00002016; Plastic 1
   //tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0 
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10000.);          // Tot upper limit for walk corection
   tofTestBeamClust->SetTotMin(1.); //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(5000.);      // effective lower Tot limit  in ps from peak position
   tofTestBeamClust->SetTotMean(2000.);          // Tot calibration target value in ps 
   tofTestBeamClust->SetMaxTimeDist(500.);       // default cluster range in ps 
   //tofTestBeamClust->SetMaxTimeDist(0.);         //Deb// default cluster range in ps 
   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_%02d_%01dtofTestBeamClust.hst.root",cFileId,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s.hst.root",cFileId);
   tofTestBeamClust->SetOutHstFileName(cOutFname);

   TString cAnaFile=Form("%s_%1d_tofAnaTestBeam.hst.root",cFileId,iSet);

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
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization
   tofAnaTestbeam->SetTOffD4(12000.);  // initialization

   switch(cFileId){
     case "MbsTrbSat1805":
     case "MbsTrbSat1900":
     case "MbsTrbSat2035":
     case "MbsTrbSat2300":
       switch (iSet) {
       case 0: // lower part of setup: HD-P2
	 tofTestBeamClust->SetDutId(3);
	 tofTestBeamClust->SetSelId(4);
	 tofAnaTestbeam->SetDXMean(0.);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);     // in ps
	 tofAnaTestbeam->SetDXWidth(0.45);
	 tofAnaTestbeam->SetDYWidth(0.45);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul0Max(100);  // Max Multiplicity in dut 
	 tofAnaTestbeam->SetMul4Max(1);    // Max Multiplicity in HDRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1);    // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(3);        // Device under test    
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1:                             // upper part of setup: Buc2013 - BucRef
	 tofTestBeamClust->SetDutId(6.);
	 tofTestBeamClust->SetSelId(1.);

	 tofAnaTestbeam->SetTOffD4(10000.);  // initialization
	 tofAnaTestbeam->SetDXMean(-0.04);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);    // in ps
	 tofAnaTestbeam->SetDXWidth(0.45);
	 tofAnaTestbeam->SetDYWidth(0.45);
	 tofAnaTestbeam->SetDTWidth(90.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(37.);   // Width  of channel selection window
	 //tofAnaTestbeam->SetCh4Sel(34.);    //Deb// Center of channel selection window
 	 //tofAnaTestbeam->SetDCh4Sel(0.1);   //Deb// Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
	 tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp 
	 tofAnaTestbeam->SetDut(6);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013)
	 break;
         default:
	 ;
       }  // end of different subsets for Sat1805
       break;
     case "MbsTrbSun1205_tsu":
     case "MbsTrbSun1555_tsu":
       switch (iSet) {
       case 0: // lower part of setup: TSU strip
	 tofTestBeamClust->SetDutId(3.);
	 tofTestBeamClust->SetSelId(4.);

	 tofAnaTestbeam->SetHitDistMin(28.);  // initialization
	 tofAnaTestbeam->SetDXMean(-0.03);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);     // in ps
	 tofAnaTestbeam->SetDXWidth(0.45);
	 tofAnaTestbeam->SetDYWidth(0.45);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in HDRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1: // TSU pad
	 tofTestBeamClust->SetDutId(8.);
	 tofTestBeamClust->SetSelId(1.);

	 tofAnaTestbeam->SetHitDistMin(27.5);  // initialization
	 tofAnaTestbeam->SetDXMean(0.);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);  // in ps
	 tofAnaTestbeam->SetDXWidth(0.63);
	 tofAnaTestbeam->SetDYWidth(0.49);
	 tofAnaTestbeam->SetDTWidth(90.); // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul0Max(100.);  // Max Multiplicity in THU - Pad - RPC 
	 tofAnaTestbeam->SetMul4Max(1.);  // Max Multiplicity in BucRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(35.);  // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);  // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetCorMode(1);   // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDTDia(0.);  // Time difference to additional diamond
	 tofAnaTestbeam->SetDut(8);       // Device under test  
	 tofAnaTestbeam->SetMrpcRef(1);   // Reference RPC
	 tofAnaTestbeam->SetPlaSelect(2); // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       default:
	 ;
       }
       break;
     default:
       tofAnaTestbeam->SetDXMean(0.);
       tofAnaTestbeam->SetDYMean(0.);
       tofAnaTestbeam->SetDTMean(0.);  // in ps
       tofAnaTestbeam->SetDXWidth(0.5);
       tofAnaTestbeam->SetDYWidth(0.5);
       tofAnaTestbeam->SetDTWidth(100.); // in ps
       tofAnaTestbeam->SetCalParFileName(cAnaFile);
       tofAnaTestbeam->SetMul4Max(10.);  // Max Multiplicity in HDRef - RPC 
       tofAnaTestbeam->SetMulDMax(10.);  // Max Multiplicity in Diamond 
       tofAnaTestbeam->SetCorMode(1);   // 1 - DTD4, 2 - X4 
       tofAnaTestbeam->SetDut(2);       // Device under test  
       tofAnaTestbeam->SetMrpcRef(4);   // Reference RPC
       tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013)    
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
    gInterpreter->ProcessLine("pl_over_clu(3)");
    gInterpreter->ProcessLine("pl_over_clu(4)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
    break;
  case 1:
    gInterpreter->ProcessLine("pl_over_clu(1)");
    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(8)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
    break;
  default:
    ;
  }
}
