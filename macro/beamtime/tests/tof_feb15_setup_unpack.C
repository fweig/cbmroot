// -----------------------------------------------------------------------------
// ----- setup_analysis FairRoot macro                                     -----
// -----                                                                   -----
// ----- created by N. Herrmann on 2014-04-18                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void setup_unpack(Int_t calMode=1, TString sFileDir="/data.local1/cdash/data", TString sFileId="CernSps05Mar0041", Int_t iSet=0) 
{
   // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
  Int_t iVerbose = 2;
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  TString logLevel = "FATAL";
  //logLevel = "ERROR";
  logLevel = "INFO";
  //logLevel = "DEBUG";
  //logLevel = "DEBUG1";
  //logLevel = "DEBUG2";
  //logLevel = "DEBUG3";
  FairLogger* log;  

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString workDir  = gSystem->Getenv("PWD");
   TString cbmDir = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = cbmDir + "/macro/tof/beamtime/feb15";

   TObjString*  unpParFile = new TObjString(paramDir + "/parUnpack_basic.txt");
   parFileList->Add(unpParFile);

   TObjString*  calParFile;
   TString cOutfileId;
   TString TofGeo="v15b";
   TObjString*  mapParFile;


   switch(iSet){
   case 0: 
     calParFile = new TObjString(paramDir + "/parCalib_batch.txt");
     cOutfileId = Form("%s",sFileId.Data());
     mapParFile = new TObjString(paramDir + "/parMapCernFeb2015.txt");
     break;

   case 1:
     calParFile = new TObjString(paramDir + "/parCalib_basic.txt");
     cOutfileId = Form("%s%s",sFileId.Data(),"_up");
     mapParFile = new TObjString(paramDir + "/parMapCernFeb2015_up.txt");
     break;

   case 2:
     calParFile = new TObjString(paramDir + "/parCalib_basic.txt");
     cOutfileId = Form("%s%s",sFileId.Data(),"_do");
     mapParFile = new TObjString(paramDir + "/parMapCernFeb2015_do.txt");
     break;

   case 10:
     calParFile = new TObjString(paramDir + "/parCalib_tsu.txt");
     cOutfileId = Form("%s%s",sFileId.Data(),"_tsu");
     TofGeo="v14b_tsu";
     break;
   default:
     ;
   }
   parFileList->Add(calParFile);

   cout << " Output File tag "<< cOutfileId << endl;

   parFileList->Add(mapParFile);

   TString geoDir  = gSystem->Getenv("VMCWORKDIR");
   TString geoFile = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
   TFile* fgeo = new TFile(geoFile);

   TGeoManager *geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
   if (NULL == geoMan){
     cout << "<E> FAIRGeom not found in geoFile"<<endl;
     //     return;
   }
   if(0){
   TGeoVolume* master=geoMan->GetTopVolume();
   master->SetVisContainers(1); 
   master->Draw("ogl"); 
   }
   // Parameter file output
   TString parFile = workDir + "/data/unpack_" + cOutfileId + ".params.root";
   
   // Output file
   TString outFile = workDir + "/data/unpack_" + cOutfileId + ".out.root";
   // ----    Debug option   -------------------------------------------------
   gDebug = 0;
   // ------------------------------------------------------------------------

   // -----   Timer   --------------------------------------------------------
   TStopwatch timer;
   timer.Start();
   // ------------------------------------------------------------------------
   
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
   FairLmdSource* source = new FairLmdSource();
   Int_t start_run=0;
   Int_t end_run  =100;
   for (Int_t irun=start_run; irun<end_run; irun++)
   {   
      TString lmdfile = Form("%s/%s_%04d.lmd",sFileDir.Data(),sFileId.Data(),irun);   
      TString sTmpSearchFile = Form("%s_%04d.lmd",sFileId.Data(),irun);
      if ( !gSystem->FindFile( sFileDir, sTmpSearchFile ) )
      {
         cout << "<W> Stop adding files after not finding " << lmdfile << endl;
         break; // Stop looping on first file not found
      } // if ( !gSystem->FindFile( sFileDir, sTmpSearchFile ) )
         
     cout << "<I> Inputdata file(s) " << lmdfile << endl;
     source->AddFile(lmdfile); 
   }
   
   /*   
   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
   //   tofTriglogUnpacker->SetSaveTriglog(kTRUE);
   tofTriglogUnpacker->SetSaveTriglog(kFALSE);
   source->AddUnpacker( tofTriglogUnpacker );
  
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
   source->AddUnpacker( tofMbsDataUnpacker );
   */
   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );
   
   gLogger->SetLogScreenLevel(logLevel.Data());

   // -----   Online/Offline MBS run   -----------------------------------

   FairRunOnline *run = FairRunOnline::Instance();
   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   // --------------------------------------------------------------------
   // ===                        End of Unpacker                            ===
   // =========================================================================
   
   // =========================================================================
   // ===                     Unpacker monitoring                           ===
   // =========================================================================
   TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", iVerbose);
   run->AddTask(tofUnpMonitor);
   // ===                 End of Unpacker monitoring                        ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================
   
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
   tofCalibration->SetTdcCalibOutFoldername("./");
   tofCalibration->SetTdcCalibFilename("batch");
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   if(calMode>0){
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   tofMapping->SetSaveDigis(kTRUE);
   run->AddTask(tofMapping);
   }
   // ===                      End of Mapping                               ===
   // =========================================================================
   if(0){  // activation leads to crash in FairRootManager !? 
   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, kFALSE);
   //tofTestBeamClust->SetCalParFileName("MbsTrbBeamtofTestBeamClust.hst.root");
   Int_t calTrg=-1;
   Int_t calSmType = 0;
   Int_t RefTrg=0;

   tofTestBeamClust->SetCalMode(0);
   tofTestBeamClust->SetCalSel(calTrg);
   tofTestBeamClust->SetCaldXdYMax(150.);         // geometrical matching window
   tofTestBeamClust->SetdTRefMax(500000.);
   tofTestBeamClust->SetCalSmType(calSmType);    // select detectors for walk correction 
   //tofTestBeamClust->SetTRefDetId(20486);        //0x00005006; big Diamond
   //tofTestBeamClust->SetTRefDetId(8214);       //0x00002016; Plastic 1
   //tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0 
   tofTestBeamClust->SetTRefId(RefTrg);          //  reference trigger 5 -> Diamond  
   tofTestBeamClust->SetTotMax(100000.);
   tofTestBeamClust->SetTotMin(1.); 

   tofTestBeamClust->SetBeamRefId(4);    // Test case 
   tofTestBeamClust->SetBeamRefSm(0);
   tofTestBeamClust->SetBeamRefDet(0);
   tofTestBeamClust->SetBeamAddRefMul(-1);

   switch (calMode) {
   case 0: 
     tofTestBeamClust->SetTotMax(100000.);      // 100 ns
     //tofTestBeamClust->SetTotMin(1.);
     tofTestBeamClust->SetTRefDifMax(2000000.); // in ps 
     tofTestBeamClust->PosYMaxScal(2000.);      // in % of length 
     tofTestBeamClust->SetMaxTimeDist(0.);      // no cluster building  
     break;
   case 1:
     tofTestBeamClust->SetTRefDifMax(100000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);         //in % of length 
     break;
   case 2:                                      // time difference calibration
     tofTestBeamClust->SetCalParFileName("MbsTrbBeam02tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(1000.);         //in % of length
     break;
   case 3:
     //	 tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
     // tofTestBeamClust->SetCalParFileName("MbsTrbThu0138_01tofTestBeamClust.hst.root");
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_03tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(200000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);       //in % of length
     break;
   case 13:
     tofTestBeamClust->SetTRefDifMax(50000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);       //in % of length
     break;
   default:
     cout << "<E> Calib mode not implemented! stop execution of script"<<endl;
     return;
   } 
   run->AddTask(tofTestBeamClust);
   }
   
   // =========================================================================
   // ===                  GO4 like output formatting                       ===
   // =========================================================================
//   TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
//   run->AddTask(tofConversion);
   // ===                  End of output conversion                         ===
   // =========================================================================
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000000);
   run->AddTask(display);   

   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   
   Bool_t kParameterMerged = kTRUE;
   FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
   parIo2->open(parFile.Data(), "UPDATE");
   parIo2->print();
   rtdb->setFirstInput(parIo2);
   
   FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
   parIo1->open(parFileList, "in");
   parIo1->print();
//   rtdb->setFirstInput(parIo1);
   rtdb->setSecondInput(parIo1);
   
   rtdb->print();
   rtdb->printParamContexts();
   // ------------------------------------------------------------------------

   // -----   Intialise and run   --------------------------------------------
   run->Init();

   cout << "Starting run" << endl;
//   run->Run(nEvents, 0);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------
/*
   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished successfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;
*/
   // ------------------------------------------------------------------------

//   delete run;
}

