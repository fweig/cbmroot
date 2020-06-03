void ana_digi(Int_t nEvents = 100000, Int_t calMode=0, Int_t calSel=-1, Int_t calSm=300, Int_t RefSel=1, char *cFileId="MbsTrbThu1715", Int_t iCalSet=0, Bool_t bOut=0, Int_t iSel2=0) 
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
   TString paramDir   = workDir + "/macro/tof/beamtime/nov15";
   TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/digi_"   + cFileId + Form("_%09d_%03d",iCalSet,iSel2) + ".out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapCernNov2015.txt";
   parFileList->Add(&mapParFile);
   TString FId=cFileId;
   TString TofGeo="v15c";
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

   tofTestBeamClust->SetPs2Ns(kTRUE);           // work with Raw Digis in ps, output will be ns 
   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetCalSel(calSel);
   tofTestBeamClust->SetCaldXdYMax(2.);          // geometrical matching window in cm 
   tofTestBeamClust->SetCalCluMulMax(10.);       // Max Counter Cluster Multiplicity for filling calib histos  
   tofTestBeamClust->SetCalRpc(calSm);           // select detector for calibration update  
   tofTestBeamClust->SetTRefId(RefSel);          // reference trigger for offset calculation 
   tofTestBeamClust->SetTotMax(10.);             // Tot upper limit for walk corection
   tofTestBeamClust->SetTotMin(0.01);            //(12000.);  // Tot lower limit for walk correction
   tofTestBeamClust->SetTotPreRange(5.);         // effective lower Tot limit  in ps from peak position
   tofTestBeamClust->SetTotMean(2000.);          // Tot calibration target value in ps 
   tofTestBeamClust->SetMaxTimeDist(500.);       // default cluster range in ps 
   //tofTestBeamClust->SetMaxTimeDist(0.);       //Deb// default cluster range in ps 
   tofTestBeamClust->SetDelTofMax(6.);        // acceptance range for cluster correlation  
   tofTestBeamClust->SetBeamRefMulMax(40);       // limit Multiplicity in beam counter

   Int_t calSelRead = calSel;
   if (calSel<0) calSelRead=0;
   TString cFname=Form("%s_set%09d_%02d_%01dtofTestBeamClust.hst.root",cFileId,iCalSet,calMode,calSelRead);
   tofTestBeamClust->SetCalParFileName(cFname);
   TString cOutFname=Form("tofTestBeamClust_%s_set%09d.hst.root",cFileId,iCalSet);
   tofTestBeamClust->SetOutHstFileName(cOutFname);

   TString cAnaFile=Form("%s_%09d%03d_tofAnaTestBeam.hst.root",cFileId,iCalSet,iSel2);

   switch (calMode) {
   case 0:                                      // initial calibration 
     tofTestBeamClust->SetTotMax(10.);      // 100 ns
     //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(2000.); // in ps 
     tofTestBeamClust->PosYMaxScal(2000.);      // in % of length 
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 1:                                       // save offsets, update walks, for diamonds 
     tofTestBeamClust->SetTRefDifMax(50.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.1);         // in % of length 
     break;
   case 11:
     tofTestBeamClust->SetTRefDifMax(4.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;   
   case 21:
     tofTestBeamClust->SetTRefDifMax(3.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 31:
     tofTestBeamClust->SetTRefDifMax(2.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 41:
     tofTestBeamClust->SetTRefDifMax(1.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        // in % of length 
     break;   
   case 51:
     tofTestBeamClust->SetTRefDifMax(0.7);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;
   case 61:
     tofTestBeamClust->SetTRefDifMax(0.5);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        // in % of length 
     break;   
   case 71:
     tofTestBeamClust->SetTRefDifMax(0.4);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.6);        // in % of length 
     break;

   case 2:                                      // time difference calibration
     tofTestBeamClust->SetTRefDifMax(300.);  // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);      //in % of length
     break;

   case 3:                                       // time offsets 
     tofTestBeamClust->SetTRefDifMax(200.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);       //in % of length
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 12:
   case 13:
     tofTestBeamClust->SetTRefDifMax(100.);   // in ps 
     tofTestBeamClust->PosYMaxScal(100.);        //in % of length
     break;
   case 22:
   case 23:
     tofTestBeamClust->SetTRefDifMax(50.);    // in ps 
     tofTestBeamClust->PosYMaxScal(50.);         //in % of length
     break;
   case 32:
   case 33:
     tofTestBeamClust->SetTRefDifMax(25.);    // in ps 
     tofTestBeamClust->PosYMaxScal(20.);         //in % of length
     break;
   case 42:
   case 43:
     tofTestBeamClust->SetTRefDifMax(12.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10.);        //in % of length
     break;
   case 52:
   case 53:
     tofTestBeamClust->SetTRefDifMax(5.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4.);        //in % of length
     break;
   case 62:
   case 63:
     tofTestBeamClust->SetTRefDifMax(2.5);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);        //in % of length
     break;
   case 72:
   case 73:
     tofTestBeamClust->SetTRefDifMax(2.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        //in % of length
     break;
   case 82:
   case 83:
     tofTestBeamClust->SetTRefDifMax(1.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length   
     break;
   case 92:
   case 93:
     tofTestBeamClust->SetTRefDifMax(0.6);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length   
     break;

   case 4:                                      // velocity dependence (DelTOF)
     tofTestBeamClust->SetTRefDifMax(6.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.5);        //in % of length
     break;
   case 14:
     tofTestBeamClust->SetTRefDifMax(3.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 24:
     tofTestBeamClust->SetTRefDifMax(1.5);   // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length
     break;
   case 44:
     tofTestBeamClust->SetTRefDifMax(1.);   // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length
     break;
   case 54:
     tofTestBeamClust->SetTRefDifMax(0.7);     // in ps 
     tofTestBeamClust->PosYMaxScal(0.7);        //in % of length
     break;
   case 64:
     tofTestBeamClust->SetTRefDifMax(0.5);     // in ps 
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
   tofAnaTestbeam->SetDTWidth(0.08); // in ps
   tofAnaTestbeam->SetCalParFileName(cAnaFile);
   tofAnaTestbeam->SetPosY4Sel(0.5); // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetDTDia(0.);   // Time difference to additional diamond
   tofAnaTestbeam->SetCorMode(RefSel); // 1 - DTD4, 2 - X4 
   tofAnaTestbeam->SetMul0Max(20);      // Max Multiplicity in dut 
   tofAnaTestbeam->SetMul4Max(20);      // Max Multiplicity in Ref - RPC 
   tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
   tofAnaTestbeam->SetHitDistMin(30.);  // initialization

   tofAnaTestbeam->SetPosYS2Sel(0.5);   // Y Position selection in fraction of strip length
   tofAnaTestbeam->SetChS2Sel(0.);      // Center of channel selection window
   tofAnaTestbeam->SetDChS2Sel(100.);   // Width  of channel selection window
   tofAnaTestbeam->SetTShift(0.);       // Shift DTD4 to 0
   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0 
   tofAnaTestbeam->SetTOffD4(13.);   // Shift DTD4 to physical value

   Int_t iBRef=iCalSet%1000;
   Int_t iSet = (iCalSet - iBRef)/1000;
   Int_t iRSel=0;
   Int_t iRSelTyp=0;
   Int_t iRSelSm=0;
   Int_t iRSelRpc=0;
   if(iSel2>=0){
     iRSel=iBRef;     // use diamond
   }else{
     iSel2=-iSel2;
     iRSel=iSel2;
   }
   iRSelRpc=iRSel%10;
   iRSelTyp = (iRSel-iRSelRpc)/10;
   iRSelSm=iRSelTyp%10;
   iRSelTyp = (iRSelTyp-iRSelSm)/10;

   tofTestBeamClust->SetBeamRefId(iRSelTyp);    // define Beam reference counter 
   tofTestBeamClust->SetBeamRefSm(iRSelSm);
   tofTestBeamClust->SetBeamRefDet(iRSelRpc);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   tofAnaTestbeam->SetBeamRefSmType(iRSelTyp);
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

   tofAnaTestbeam->SetChi2Lim(10.);           // initialization of Chi2 selection limit  

   switch (iSet) {
   case 0:                                 // upper part of setup: P2 - P5
   case 3:                                 // upper part of setup: P2 - P5
   case 34:                                // upper part of setup: P2 - P5
   case 400300:
     cout << "Run with iRSel = "<<iRSel<<endl;
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(0.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
	   
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-3.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
	   tofAnaTestbeam->SetChi2Lim(100.);   // initialization of Chi2 selection limit  
	   tofAnaTestbeam->SetMulDMax(3);      // Max Multiplicity in BeamRef // Diamond    
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }

     tofAnaTestbeam->SetChi2Lim(30.);     // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.5);
     tofAnaTestbeam->SetDYWidth(1.5);
     tofAnaTestbeam->SetDTWidth(0.12);   // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(3);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.25);  // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(4);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(4);      // Max Multiplicity in Ref - RPC 
     break;

   case 920300:
   case 921300:
     switch (iRSel){
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	   tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.2); // in ps
     tofAnaTestbeam->SetCh4Sel(15);      // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.5);  // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(3);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(3);      // Max Multiplicity in Ref - RPC 
     break; 

   case 920400:
   case 921400:
     switch (iRSel){
         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in BeamRef // Diamond    
	   tofAnaTestbeam->SetTShift(0.2);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 9:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.1);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.5);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(30.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.2);    // in ps
     tofAnaTestbeam->SetCh4Sel(8);        // Center of channel selection window
     tofAnaTestbeam->SetDCh4Sel(10);      // Width  of channel selection window
     tofAnaTestbeam->SetPosY4Sel(0.5);    // Y Position selection in fraction of strip length
     tofAnaTestbeam->SetMulDMax(1);       // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(30);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(30);      // Max Multiplicity in Ref - RPC 
     break; 

   case 300920:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(1000.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(2.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.5); // in ps
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(5);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(5);      // Max Multiplicity in Ref - RPC 
     break; 

   case 900920:

     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(0.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.);
     tofAnaTestbeam->SetDYWidth(2.);
     tofAnaTestbeam->SetDTWidth(0.1); // in ps
     tofAnaTestbeam->SetMulDMax(1);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(15);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(15);      // Max Multiplicity in Ref - RPC 
     break; 

   case 300921:
   case 400921:
   case 900921:
   case 910921:
   case 920921:
     switch (iRSel){
         case 3:
	   tofAnaTestbeam->SetTShift(-8.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.15);     // Shift Sel2 time peak to 0
	   break;

         case 5: // sel2 = 920
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-5.);  // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.1);  // Shift Sel2 time peak to 0
	   break;

         case 9:   // sel2 = -920
	   tofAnaTestbeam->SetTShift(1.2);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.15);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(10.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.0);
     tofAnaTestbeam->SetDYWidth(2.0);
     tofAnaTestbeam->SetDTWidth(0.15); // in ps
     Bool_t BEff=kFALSE;
     //Bool_t BEff=kTRUE;
     if(BEff) {
       tofAnaTestbeam->SetCh4Sel(16);      // Center of channel selection window
       tofAnaTestbeam->SetDCh4Sel(13);      // Width  of channel selection window
       tofAnaTestbeam->SetPosY4Sel(0.4);   // Y Position selection in fraction of strip length
     }
     break; 

   case 901900:
   case 400900:
   case 300900:
     switch (iRSel){
         case 4:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 5:
           //tofTestBeamClust->SetBeamAddRefMul(1);
	   tofAnaTestbeam->SetTShift(-20.);     // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(-0.17);     // Shift Sel2 time peak to 0
	   break;

         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(40.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(1.0);
     tofAnaTestbeam->SetDYWidth(2.0);
     tofAnaTestbeam->SetDTWidth(0.15); // in ps
     break; 

   case 300400:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

   case 210200:
     switch (iRSel){
         case 9:
	   tofAnaTestbeam->SetTShift(-2.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     break;

   case 700600:
   case 701600:
   case 702600:
   case 703600:
   case 100600:
   case 200600:
   case 210600:
   case 601600:
     switch (iRSel){
         case 5:
	   tofAnaTestbeam->SetTShift(-11.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;

         case 6:
	   tofAnaTestbeam->SetTShift(-6.);   // Shift DTD4 to 0
	   tofAnaTestbeam->SetTOffD4(16.);   // Shift DTD4 to physical value
	   tofAnaTestbeam->SetSel2TOff(0.);     // Shift Sel2 time peak to 0
	   break;
         default:
	   ;
     }
     tofAnaTestbeam->SetChi2Lim(5.);   // initialization of Chi2 selection limit  
     tofAnaTestbeam->SetDXWidth(0.5);
     tofAnaTestbeam->SetDYWidth(1.);
     tofAnaTestbeam->SetDTWidth(0.1); // in ps
     Bool_t BEff=kFALSE;
     if(BEff) {
       tofAnaTestbeam->SetCh4Sel(20);      // Center of channel selection window
       tofAnaTestbeam->SetDCh4Sel(8);      // Width  of channel selection window
       tofAnaTestbeam->SetPosY4Sel(0.4);   // Y Position selection in fraction of strip length
     }
     tofAnaTestbeam->SetMulDMax(10);      // Max Multiplicity in Diamond    
     tofAnaTestbeam->SetMul0Max(10);      // Max Multiplicity in dut 
     tofAnaTestbeam->SetMul4Max(10);      // Max Multiplicity in Ref - RPC 
     break;

     default:
	   cout<<"<E> detector setup "<<iSet<<" unknown, stop!"<<endl;
	   return;
	 ;
   }  // end of different subsets

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
  case 910900:
  case 300900:
  case 400900:
  case 901900:
  case 921920:
  case 300921:
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
  case 61:
  case 81:
  case 16:
  case 86:
  case 18:
  case 68:
  case 601600:
  case 100600:
  case 200600:
  case 700600:
  case 600601:
  case 100601:
  case 200601:
  case 700601:

    gInterpreter->ProcessLine("pl_over_clu(1)");
    gInterpreter->ProcessLine("pl_over_clu(5)");
    gInterpreter->ProcessLine("pl_over_clu(6)");
    gInterpreter->ProcessLine("pl_over_clu(6,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(2,0,0)");
    gInterpreter->ProcessLine("pl_over_clu(2,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(7,0,0)");
    gInterpreter->ProcessLine("pl_over_clu(7,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(7,0,2)");
    gInterpreter->ProcessLine("pl_over_clu(7,0,3)");
    gInterpreter->ProcessLine("pl_over_clu(8)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,1)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,2)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,3)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,4)");
    gInterpreter->ProcessLine("pl_over_clu(8,0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,7,0,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,2)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,3)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,4)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,8,0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,6,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,6,0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5)");
    gInterpreter->ProcessLine("pl_over_cluSel(0,5,1)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,2,0,0)");
    gInterpreter->ProcessLine("pl_over_cluSel(1,2,1,0)");
    gInterpreter->ProcessLine("pl_all_dTSel()");
    gInterpreter->ProcessLine("pl_over_MatD4sel()");
    break;

  default:
    ;
  }
  gInterpreter->ProcessLine(Display_Funct);
}
