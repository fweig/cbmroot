// --------------------------------------------------------------------------
//
// Macro for analyzing test setup data
// nh   14/02/2014 
// -------------------------------------------------------------------------

// Max nEvents: 198999999999
void ana_lmd(Int_t nEvents = 10, Int_t start_run=1, Int_t end_run=100, Int_t calMode=0, char *cFileId="MbsTrbBeamFee6") 
{
   // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug, 4=raw debug)
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

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly 
   // a concatenated input parameter file.
   TList *parFileList = new TList();
   
   TString workDir = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = workDir + "/macro/tof/beamtime/apr14";

   //   TObjString unpParFile = paramDir + "/parUnpackLmdLab2014.txt";
   //TObjString unpParFile = paramDir + "/parUnpackLmdTrb2014.txt";
   //TObjString unpParFile = paramDir + "/parUnpackLmdMbsTrbCosPul.txt";
   TObjString unpParFile = paramDir + "/parUnpackLmdMbsTrbBeam.txt";
   parFileList->Add(&unpParFile);
   //TObjString calParFile = paramDir + "/parCalLab2014.txt";
   TObjString calParFile = paramDir + "/parCalApr2014MbsTrbBeam.txt";
   parFileList->Add(&calParFile);
   //TObjString mapParFile = paramDir + "/parMapLab2014.txt";
   //TObjString mapParFile = paramDir + "/parMapApr2014MbsTrbCosPul.txt";
   TObjString mapParFile = paramDir + "/parMapApr2014MbsTrbBeam.txt";
   parFileList->Add(&mapParFile);
   TObjString convParFile = paramDir + "/parConvApr2014.txt";
   parFileList->Add(&convParFile);

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
   // Parameter file output
   TString parFile = paramDir + "/unpack.params.root";
   
   // Output file
   TString outFile = paramDir+"/unpack.out.root";
   
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
  
   for (Int_t irun=start_run; irun<end_run; irun++)
   {   
     TString lmdfile = Form("./LMD/%s_%04d.lmd",cFileId,irun);   
     //TString lmdfile = Form("./LMD/MbsTrbBeamFee6_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbCosFee6_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbHdFee6_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbHdFee_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbHdTest_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbHDall_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbCosPul_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrbPrep_%04d.lmd",irun);   
     //TString lmdfile = Form("./LMD/MbsTrb2Pul_%04d.lmd",irun);   
    cout << "<I> Inputdata file(s) " << lmdfile << endl;
    source->AddFile(lmdfile); 
   }

   // -----   Online/Offline MBS run   -----------------------------------

   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof( iVerbose );
   source->AddUnpacker( tofTriglogUnpacker );
   
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof( iVerbose );
   source->AddUnpacker( tofMbsDataUnpacker );

   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );

   FairRunOnline *run = new FairRunOnline(source);
   //   run->SetGeomFile(geoFile);
   run->SetOutputFile(outFile);
   // ===                        End of Unpacker                            ===
   // =========================================================================
   
   // =========================================================================
   // ===                     Unpacker monitoring                           ===
   // =========================================================================
   TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", iVerbose);
   run->AddTask(tofUnpMonitor);       // do not remove!! 
   // ===                 End of Unpacker monitoring                        ===
   // =========================================================================
   // ---- Set the log level 	
   gLogger->SetLogScreenLevel(logLevel.Data());
   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================
   
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================
   
   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
   run->AddTask(tofMapping);
   // ===                      End of Mapping                               ===
   // =========================================================================

   // Cluster/Hit builder
   //CbmTofSimpClusterizer* tofSimpClust = new CbmTofSimpClusterizer("TOF Simple Clusterizer",iVerbose);
   //run->AddTask(tofSimpClust);
   
   CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, kTRUE);
   //tofTestBeamClust->SetCalParFileName("CalMbsTrbCosPul0tofTestBeamClust.hst.root");
   tofTestBeamClust->SetCalParFileName("MbsTrbBeamtofTestBeamClust.hst.root");
   
   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetTRefDetId(8214);         //0x00002016; Plastic 1
   //tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0  
   if (calMode==0){
     tofTestBeamClust->SetTRefDifMax(100000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(200.);         //in % of length 
   }
   else{
     if(calMode==1){
       tofTestBeamClust->SetCalParFileName("MbsTrbBeam0tofTestBeamClust.hst.root");
       tofTestBeamClust->SetTRefDifMax(10000.);    // in ps 
       tofTestBeamClust->PosYMaxScal(20.);         //in % of length 
     }
     else{
       if(calMode==2){
	 tofTestBeamClust->SetCalParFileName("MbsTrbBeam0tofTestBeamClust.hst.root");
	 tofTestBeamClust->SetTRefDifMax(10000.);   // in ps 
	 tofTestBeamClust->PosYMaxScal(2.);         //in % of length
       } 
     }
   }
   run->AddTask(tofTestBeamClust);

   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   //run->AddTask(tofAnaTestbeam);
   
   // =========================================================================
   // ===                  GO4 like output formatting                       ===
   // =========================================================================
   /*
   TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
   run->AddTask(tofConversion);
   */
   // ===                  End of output conversion                         ===
   // =========================================================================
   
   // -----  Parameter database   --------------------------------------------
   FairRuntimeDb* rtdb = run->GetRuntimeDb();
   
   Bool_t kParameterMerged = kTRUE;
   FairParRootFileIo* parIo1 = new FairParRootFileIo(kParameterMerged);
   parIo1->open(parFile.Data()); // "UPDATE");
   parIo1->print();
   rtdb->setFirstInput(parIo1);
   
   FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
   parIo2->open(parFileList, "in");
   parIo2->print();
//   rtdb->setFirstInput(parIo1);
   rtdb->setSecondInput(parIo2);
   
   rtdb->print();
   rtdb->printParamContexts();

   rtdb->setOutput(parIo1);
   rtdb->saveOutput();
  
   // ------------------------------------------------------------------------

   // -----   Intialise and run   --------------------------------------------
   run->Init();
   cout << "Starting run" << endl;
//   run->Run(0, nEvents);
   run->Run(nEvents, 0); // <= FAIRROOT guy style, crash for me on event finish
//   run->RunOnLmdFiles(0, nEvents);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------

   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished succesfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;
   // ------------------------------------------------------------------------

//   delete run;

   cout << " Test passed" << endl;
   cout << " All ok " << endl;
}

