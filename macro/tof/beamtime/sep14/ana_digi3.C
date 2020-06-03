void ana_digi3(Int_t nEvents = 1000000, Int_t calMode=0, Int_t calSel=-1, Int_t calSmType=2, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iSet=0) 
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
   TString OutputFile = paramDir + "/digi_" + cFileId + ".out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapGsiSep14.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v14a";
   TString FId=cFileId;
   if(FId.Contains("tsu")) {
     cout << FId << ": Analyse TSU counter "<< endl;
     TofGeo="v14a_tsu";
   }

   TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   parFileList->Add(&tofDigiFile);   

   TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
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

   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, kFALSE);
   //tofTestBeamClust->SetCalParFileName("MbsTrbBeamtofTestBeamClust.hst.root");

   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(2.);          // geometrical matching window in cm 
   tofTestBeamClust->SetdTRefMax(50000.);        // obsolete, not used any more
   tofTestBeamClust->SetCalSmType(calSmType);    // select detectors for walk correction 
   //tofTestBeamClust->SetTRefDetId(20486);      //0x00005006; big Diamond
   //tofTestBeamClust->SetTRefDetId(8214);       //0x00002016; Plastic 1
   //tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0 
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10000.);          // Tot upper limit for walk corection
   //tofTestBeamClust->SetTotMin(1.); //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(5000.);      // effective lower Tot limit  in ps from peak position
   tofTestBeamClust->SetTotMean(2000.);          // Tot calibration target value in ps 
   tofTestBeamClust->SetMaxTimeDist(500.);       // default cluster range in ps 
   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_%02d_%01dtofTestBeamClust.hst.root",cFileId,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s.hst.root",cFileId);
   tofTestBeamClust->SetOutHstFileName(cOutFname);
   TString cAnaFile=Form("%s_%1d_tofAnaTestBeam.hst.root",cFileId,iSet);
   if(iSet==0){
     tofTestBeamClust->SetDutId(6.);
   }
   else
   {
     tofTestBeamClust->SetDutId(8.);
   }
   tofTestBeamClust->SetSel2Id(0.);
   tofTestBeamClust->SetSelId(1);

   switch (calMode) {
   case 0:                                      // initial calibration 
     tofTestBeamClust->SetTotMax(100000.);
     //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(3000000000000.); // in ps  // special setting for SmType=1 
     tofTestBeamClust->PosYMaxScal(2000000000.);      // in % of length 
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
     tofTestBeamClust->SetTRefDifMax(2000000000000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1000000000.);       //in % of length
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 12:
   case 13:
     tofTestBeamClust->SetTRefDifMax(1000000000000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(100000000.);        //in % of length
     break;
   case 22:
   case 23:
     tofTestBeamClust->SetTRefDifMax(50000000000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(5000000.);         //in % of length
     break;
   case 32:
   case 33:
     tofTestBeamClust->SetTRefDifMax(2500000000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(200000.);         //in % of length
     break;
   case 42:
   case 43:
     tofTestBeamClust->SetTRefDifMax(160000000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10000.);        //in % of length
     break;
   case 52:
   case 53:
     tofTestBeamClust->SetTRefDifMax(8000000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4000.);        //in % of length
     break;
   case 62:
   case 63:
     tofTestBeamClust->SetTRefDifMax(4000000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4000.);        //in % of length
     break;
   case 72:
   case 73:
     tofTestBeamClust->SetTRefDifMax(2000000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(2000.0);        //in % of length
     break;
   case 82:
   case 83:
     tofTestBeamClust->SetTRefDifMax(1000000.);    // in ps  
     tofTestBeamClust->PosYMaxScal(1500.);        //in % of length   
     break;
   case 92:
   case 93:
     tofTestBeamClust->SetTRefDifMax(500000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);        //in % of length   
     break;

   case 4:                                      // velocity dependence (DelTOF)
     tofTestBeamClust->SetTRefDifMax(5000.);    // in ps 
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
   switch(cFileId){
     case "MbsTrbSun2325":
       tofAnaTestbeam->SetDXMean(0.09);
       tofAnaTestbeam->SetDYMean(-0.06);
       tofAnaTestbeam->SetDTMean(0.);  // in ps
       tofAnaTestbeam->SetDXWidth(0.59);
       tofAnaTestbeam->SetDYWidth(0.47);
       tofAnaTestbeam->SetDTWidth(220.); // in ps
       break;
     case "MbsTrbMon0153":
       tofAnaTestbeam->SetDXMean(0.54);
       tofAnaTestbeam->SetDYMean(-0.02);
       tofAnaTestbeam->SetDTMean(0.);  // in ps
       tofAnaTestbeam->SetDXWidth(0.6);
       tofAnaTestbeam->SetDYWidth(0.55);
       tofAnaTestbeam->SetDTWidth(100.); // in ps
       break;
     case "MbsTrbSat1805":
       switch (iSet) {
       case 0: // lower part of setup: HD-P2
	 tofAnaTestbeam->SetDXMean(0.13);
	 tofAnaTestbeam->SetDYMean(-0.78);
	 tofAnaTestbeam->SetDTMean(6.);     // in ps
	 tofAnaTestbeam->SetDXWidth(0.40);
	 tofAnaTestbeam->SetDYWidth(0.40);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1);   // Max Multiplicity in HDRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(4.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.2); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1:                             // upper part of setup: Buc2013 - BucRef
	 tofAnaTestbeam->SetDXMean(-0.04);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);    // in ps
	 tofAnaTestbeam->SetDXWidth(0.1);
	 tofAnaTestbeam->SetDYWidth(0.5);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(4.);   // Width  of channel selection window
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetCorMode(2);    // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(6);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(1);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013)
	 break;
         default:
	 ;
       }  // end of different subsets for Sat1805
       break;
     case "MbsTrbSun1205_tsu":
     case "MbsTrbSun1525_tsu":
       switch (iSet) {
       case 0: // lower part of setup: TSU strip
	 tofAnaTestbeam->SetDXMean(-0.03);
	 tofAnaTestbeam->SetDYMean( 0.);
	 tofAnaTestbeam->SetDTMean(0.);     // in ps
	 tofAnaTestbeam->SetDXWidth(0.40);
	 tofAnaTestbeam->SetDYWidth(0.40);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in HDRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.4); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);     // Time difference to additional diamond
	 tofAnaTestbeam->SetCorMode(1);    // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1: // TSU pad
	 tofAnaTestbeam->SetDXMean(-0.15);
	 tofAnaTestbeam->SetDYMean(-2.7);
	 tofAnaTestbeam->SetDTMean(13.9);  // in ps
	 tofAnaTestbeam->SetDXWidth(2.63);
	 tofAnaTestbeam->SetDYWidth(2.49);
	 tofAnaTestbeam->SetDTWidth(93.); // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(1.);  // Max Multiplicity in HDRef - RPC 
	 tofAnaTestbeam->SetMulDMax(1.);  // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetCorMode(1);   // 1 - DTD4, 2 - X4 
	 tofAnaTestbeam->SetDut(8);       // Device under test  
	 tofAnaTestbeam->SetMrpcRef(1);   // Reference RPC
	 tofAnaTestbeam->SetPlaSelect(2);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
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
  TString Display_Status = "pl_over_cluSel.C";
  gROOT->LoadMacro(Display_Status);
  gROOT->LoadMacro("pl_over_clu.C");
  gROOT->LoadMacro("pl_all_dTSel.C");
  switch (iSet) {
  case 0: 
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,4)");
    break;
  case 1:
    gInterpreter->ProcessLine("pl_over_clu(1)");
    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
    gInterpreter->ProcessLine("pl_all_dTSel()");
    break;
  }
}
