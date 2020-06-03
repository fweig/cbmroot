// -----------------------------------------------------------------------------
// ----- setup_analysis FairRoot macro                                     -----
// -----                                                                   -----
// ----- created by N. Herrmann on 2014-04-18                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void setup_unpack(Int_t calMode=1, char *cFileId="CbmTofSps_01Dec0128", Int_t iSet=0) 
{
   // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
  Int_t iVerbose = 2;
  // Specify log level (INFO, DEBUG, DEBUG1, ...)
  TString logLevel = "FATAL";
  //TString logLevel = "ERROR";
  TString logLevel = "INFO";
  //TString logLevel = "DEBUG";
  //TString logLevel = "DEBUG1";
  //TString logLevel = "DEBUG2";
  //TString logLevel = "DEBUG3";
  FairLogger* log;  

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString workDir = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = workDir + "/macro/tof/beamtime/nov15";

   //   TObjString unpParFile = paramDir + "/parUnpack_basic.txt";					
   TObjString unpParFile = paramDir + "/parUnpCernNov2015.txt";					
   parFileList->Add(&unpParFile);

   TObjString calParFile;
   TString cOutfileId;
   //TString TofGeo="v14b";
   TString TofGeo="v15c";
   TObjString mapParFile;


   switch(iSet){
   case 0: 
     calParFile = paramDir + "/parCalib_batch.txt";										
     cOutfileId = Form("%s",cFileId);
     mapParFile = paramDir + "/parMapCernNov2015.txt";						
     break;

   case 1:
   /*
     calParFile = paramDir + "/parCalib_basic.txt";
     cOutfileId = Form("%s%s",cFileId,"_nopla");
     mapParFile = paramDir + "/parMapCosmicsThuMay2015_nopla.txt";
 	int mapping =1;   
 	*/  
	break;

   default:
     ; 
   }
   parFileList->Add(&calParFile);
 
   cout << " Output File tag "<< cOutfileId << endl;

   parFileList->Add(&mapParFile);
   //TObjString convParFile = paramDir + "/parConvFeb2015.txt";
   //parFileList->Add(&convParFile);

   TObjString tofDigiFile = workDir + "/parameters/tof/tof_" + TofGeo + ".digi.par"; // TOF digi file
   //   parFileList->Add(&tofDigiFile);   

   TObjString tofDigiBdfFile = workDir  + "/parameters/tof/tof_" + TofGeo +".digibdf.par";
   //   TObjString tofDigiBdfFile =  paramDir + "/tof.digibdf.par";
   parFileList->Add(&tofDigiBdfFile);

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
   TString parFile = paramDir + "/unpack_" + cOutfileId + ".params.root";
   
   // Output file
   TString outFile = paramDir + "/unpack_" + cOutfileId + ".out.root";
   // ----    Debug option   -------------------------------------------------
   gDebug = 0;
   // ------------------------------------------------------------------------

   // -----   Timer   --------------------------------------------------------
   TStopwatch timer;
   timer.Start();
   // ------------------------------------------------------------------------
   
 
   gLogger->SetLogScreenLevel(logLevel.Data());

   // -----   Online/Offline MBS run   -----------------------------------

   FairRunOnline *run = FairRunOnline::Instance();
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   
   Bool_t kParameterMerged = kTRUE;
   
   FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
   parIo1->open(parFileList, "in");
   parIo1->print();
//   rtdb->setFirstInput(parIo1);

   FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
   parIo2->open(parFile.Data(), "UPDATE");
   parIo2->print();

   rtdb->setFirstInput(parIo1);
   rtdb->setSecondInput(parIo2);
   
   rtdb->print();
   rtdb->printParamContexts();
   // ------------------------------------------------------------------------
   // =========================================================================
   // ===                           Unpacker                                ===
   // =========================================================================
   /*
   FairLmdSource* source = new FairLmdSource();
   for (Int_t irun=start_run; irun<end_run; irun++)
   {   
     TString lmdfile = paramDir+Form("/LMD/%s_%04d.lmd",cFileId,irun);   
     cout << "<I> Inputdata file(s) " << lmdfile << endl;
     source->AddFile(lmdfile); 
   }
   */
   CbmHldSource* source = new CbmHldSource();
   source->AddPath("/hera/cbm/users/tofGsiApr14/cern-nov15/production/",Form("%s*.hld",cFileId));
   //source->AddPath("./LMD/",Form("%s*.hld",cFileId));
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
   run->SetSource(source);
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
   tofCalibration->SetTdcCalibFilename("Tof_calib_batch");
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
   if(0){  
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

   tofTestBeamClust->SetBeamRefId(5);    // define Beam reference counter 
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

