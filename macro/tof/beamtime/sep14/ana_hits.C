void ana_hits(Int_t nEvents = 10000000, Int_t iSel=1, Int_t iGenCor=1, char *cFileId="MbsTrbSat1805", Int_t iSet=0) 
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
   TString InputDigiFile = paramDir + "/digi_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/hits_" + cFileId + ".out.root";

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
  run->AddFriend(InputDigiFile.Data());
  run->SetOutputFile(OutputFile);
  gLogger->SetLogScreenLevel(logLevel.Data());

   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   //   run->AddTask(tofMapping);

   TString cAnaFile=Form("%s_%1d_%02d_tofAnaTestBeam.hst.root",cFileId,iSet,iSel);

   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   tofAnaTestbeam->SetCorMode(iGenCor); // 1 - DTD4, 2 - X4, 3 - Y4, 4 - Texp 
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization
   tofAnaTestbeam->SetTOffD4(20000.);   // initialization

   switch(cFileId){
     case "MbsTrbSat1805":
     case "MbsTrbSat2035":
       switch (iSet) {
       case 0: // lower part of setup: HD-P2
	 tofAnaTestbeam->SetHitDistMin(27.5);  // initialization
	 tofAnaTestbeam->SetTOffD4(12000.);    // initialization
	 tofAnaTestbeam->SetDXMean(0.);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);     // in ps
	 tofAnaTestbeam->SetDXWidth(0.40);
	 tofAnaTestbeam->SetDYWidth(0.40);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1:                             // upper part of setup: Buc2013 - BucRef
	 tofAnaTestbeam->SetTOffD4(12000.);  // initialization
	 tofAnaTestbeam->SetDXMean(-0.04);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);    // in ps
	 tofAnaTestbeam->SetDXWidth(0.45);
	 tofAnaTestbeam->SetDYWidth(0.45);
	 tofAnaTestbeam->SetDTWidth(80.);  // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 tofAnaTestbeam->SetMul4Max(2.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(5.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.2); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
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
	 tofAnaTestbeam->SetTOffD4(12000.);   // initialization
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
	 tofAnaTestbeam->SetDut(3);        // Device under test   
	 tofAnaTestbeam->SetMrpcRef(4);    // Reference RPC       
	 tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013) 
	 break;
       case 1: // TSU pad
	 tofAnaTestbeam->SetHitDistMin(27.5);  // initialization
	 tofAnaTestbeam->SetTOffD4(12000.);  // initialization
	 tofAnaTestbeam->SetDXMean(0.);
	 tofAnaTestbeam->SetDYMean(0.);
	 tofAnaTestbeam->SetDTMean(0.);  // in ps
	 tofAnaTestbeam->SetDXWidth(0.8);
	 tofAnaTestbeam->SetDYWidth(0.8);
	 tofAnaTestbeam->SetDTWidth(90.); // in ps
	 tofAnaTestbeam->SetCalParFileName(cAnaFile);
	 //	 tofAnaTestbeam->SetMul0Max(1.);  // Max Multiplicity in THU - Pad - RPC 
	 tofAnaTestbeam->SetMul4Max(1.);  // Max Multiplicity in BucRef - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);   // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(5.);  // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.4); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);  // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(500.);  // Time difference to additional diamond
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
       tofAnaTestbeam->SetDut(2);       // Device under test  
       tofAnaTestbeam->SetMrpcRef(4);   // Reference RPC
       tofAnaTestbeam->SetPlaSelect(0);  // Select attached plastics (0 - HD-P2, 2 - Buc2013)    
   }
   switch(iSet){
   case 0:
     switch(iSel){   // selection cuts 
     case 0:
	 tofAnaTestbeam->SetMul4Max(10);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(70.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(10.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(10.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 break;
     case 1:
	 tofAnaTestbeam->SetMul0Max(10);    // Max Multiplicity in dut - RPC 
	 tofAnaTestbeam->SetMul4Max(1);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 break;
     case 2:
	 tofAnaTestbeam->SetMul4Max(1);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 break;
     case 3:
	 tofAnaTestbeam->SetMul4Max(1);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(4.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.3); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 break;
     case 20:
	 tofAnaTestbeam->SetMul4Max(2);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(70.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(1.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 break;
     case 21:
	 tofAnaTestbeam->SetMul4Max(2);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(0.);     // Time difference to additional diamond
	 break;
     case 22:
	 tofAnaTestbeam->SetMul4Max(2);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(7.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 break;
     case 23:
	 tofAnaTestbeam->SetMul4Max(2);    // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(8.);    // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(4.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.3); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 break;
     default :
       ;
     }
     break;
   case 1:           // upper system 
     switch(iSel){   // selection cuts
     case 0:                               // for optimization of corrections 
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(40.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
	 break;
     case 1:
	 tofAnaTestbeam->SetMul0Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(5.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond       
	 break;
     case 2:
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(5.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.2); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
	 break;
     case 3:
	 tofAnaTestbeam->SetMul4Max(1.);   // Max Multiplicity in Ref - RPC 
	 tofAnaTestbeam->SetCh4Sel(38.);  // Center of channel selection window
	 tofAnaTestbeam->SetDCh4Sel(5.);   // Width  of channel selection window
	 tofAnaTestbeam->SetPosY4Sel(0.2); // Y Position selection in fraction of strip length
	 tofAnaTestbeam->SetMulDMax(1.);   // Max Multiplicity in Diamond 
	 tofAnaTestbeam->SetDTDia(500.);   // Time difference to additional diamond
	 break;
     default:
       ;
     } 
     break;
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
}
