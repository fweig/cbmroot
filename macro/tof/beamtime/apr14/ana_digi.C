void ana_digi(Int_t nEvents = 1000000, Int_t calMode=0, Int_t calTrg=-1, Int_t calSmType=2, Int_t RefTrg=5, char *cFileId="MbsTrbSun2325") 
{
  Int_t iVerbose = 3;
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
   TString paramDir   = workDir + "/macro/tof/beamtime/apr14";
   TString ParFile    = paramDir + "/unpack_" + cFileId + ".params.root";
   TString InputFile  = paramDir + "/unpack_" + cFileId + ".out.root";
   TString OutputFile = paramDir + "/digi.out.root";

   TList *parFileList = new TList();

   TObjString mapParFile = paramDir + "/parMapApr2014MbsTrbBeam.txt";
   parFileList->Add(&mapParFile);

   TString TofGeo="v14a";
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
   tofTestBeamClust->SetCalTrg(calTrg);
   tofTestBeamClust->SetCaldXdYMax(1.5);         // geometrical matching window
   tofTestBeamClust->SetdTRefMax(5000.);
   tofTestBeamClust->SetCalSmType(calSmType);    // select detectors for walk correction 
   //tofTestBeamClust->SetTRefDetId(20486);        //0x00005006; big Diamond
   //tofTestBeamClust->SetTRefDetId(8214);       //0x00002016; Plastic 1
   // tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0 
   tofTestBeamClust->SetTRefId(RefTrg);             //  Plastic 0 - trigger 
   tofTestBeamClust->SetTotMax(30000.);
   tofTestBeamClust->SetTotMin(15000.);
   TString cFname=Form("%s_%02dtofTestBeamClust.hst.root",cFileId,calMode);
   tofTestBeamClust->SetCalParFileName(cFname);
   switch (calMode) {
   case 0: 
     tofTestBeamClust->SetTotMax(100000.);
     tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(200000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(200.);         //in % of length 
     break;
   case 1:
     tofTestBeamClust->SetTRefDifMax(5000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 11:
     tofTestBeamClust->SetTRefDifMax(5000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;   
   case 21:
     tofTestBeamClust->SetTRefDifMax(4000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(2.0);        // in % of length 
     break;
   case 31:
     tofTestBeamClust->SetTRefDifMax(2000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1.0);        // in % of length 
     break;
   case 41:
     tofTestBeamClust->SetTRefDifMax(1000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        // in % of length 
     break;
   case 2:                                      // time difference calibration
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10.);         //in % of length
     break;
   case 12:
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(5.);         //in % of length
     break;
   case 22:
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(4.);         //in % of length
     break;
   case 32:
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(3.);         //in % of length
     break;
   case 42:
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);         //in % of length
     break;
   case 52:
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);         //in % of length
     break;
   case 62:
     tofTestBeamClust->SetTRefDifMax(5000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.5);         //in % of length
     break;
   case 3:
     tofTestBeamClust->SetTRefDifMax(200000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(5.);       //in % of length
     break;
   case 13:
     tofTestBeamClust->SetTRefDifMax(100000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.5);         //in % of length
     break;
   case 23:
     tofTestBeamClust->SetTRefDifMax(50000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.5);         //in % of length
     break;
   case 33:
     tofTestBeamClust->SetTRefDifMax(25000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);         //in % of length
     break;
   case 43:
     tofTestBeamClust->SetTRefDifMax(12000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 53:
     tofTestBeamClust->SetTRefDifMax(5000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 63:
     tofTestBeamClust->SetTRefDifMax(2500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 73:
     tofTestBeamClust->SetCalParFileName(cFname);
     tofTestBeamClust->SetTRefDifMax(2000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 83:
     tofTestBeamClust->SetTRefDifMax(1500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length   
     break;
   case 93:
     tofTestBeamClust->SetTRefDifMax(1000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(0.8);        //in % of length   
     break;
   case 4:
     tofTestBeamClust->SetTRefDifMax(2500.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 14:
     tofTestBeamClust->SetTRefDifMax(2000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
     break;
   case 24:
     tofTestBeamClust->SetTRefDifMax(1000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1.);        //in % of length
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
       tofAnaTestbeam->SetDXMean(0.024);
       tofAnaTestbeam->SetDYMean(-0.015);
       tofAnaTestbeam->SetDTMean(0.);  // in ps
       tofAnaTestbeam->SetDXWidth(0.58);
       tofAnaTestbeam->SetDYWidth(0.40);
       tofAnaTestbeam->SetDTWidth(100.); // in ps
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
}
