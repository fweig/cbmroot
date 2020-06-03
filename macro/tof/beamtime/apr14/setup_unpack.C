// -----------------------------------------------------------------------------
// ----- setup_analysis FairRoot macro                                     -----
// -----                                                                   -----
// ----- created by N. Herrmann on 2014-04-18                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void setup_unpack(Int_t nEvents = 10, Int_t start_run=1, Int_t end_run=100, Int_t calMode=0, char *cFileId="MbsTrbBeamFee6") 
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

   TObjString unpParFile = paramDir + "/parUnpackLmdMbsTrbBeam.txt";
   parFileList->Add(&unpParFile);

   TObjString calParFile = paramDir + "/parCalApr2014MbsTrbBeam.txt";
   parFileList->Add(&calParFile);

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
   TString parFile = paramDir + "/unpack_" + cFileId + ".params.root";
   
   // Output file
   TString outFile = paramDir + "/unpack_" + cFileId + ".out.root";
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
     cout << "<I> Inputdata file(s) " << lmdfile << endl;
     source->AddFile(lmdfile); 
   }

   
   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
   source->AddUnpacker( tofTriglogUnpacker );
  
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
   source->AddUnpacker( tofMbsDataUnpacker );

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
   /*
    CbmTofTestBeamClusterizer* tofTestBeamClust = new CbmTofTestBeamClusterizer("TOF TestBeam Clusterizer",iVerbose, kFALSE);
    //tofTestBeamClust->SetCalParFileName("MbsTrbBeamtofTestBeamClust.hst.root");
   
   tofTestBeamClust->SetCalMode(calMode);
   tofTestBeamClust->SetTRefDetId(20486);        //0x00005006; big Diamond
   //tofTestBeamClust->SetTRefDetId(8214);       //0x00002016; Plastic 1
   //tofTestBeamClust->SetTRefDetId(8198);       //0x00002006;  Plastic 0 
   switch (calMode) {
   case 0: 
     tofTestBeamClust->SetTRefDifMax(200000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(200.);         //in % of length 
     break;
   case 1:
     tofTestBeamClust->SetCalParFileName("MbsTrbBeam03tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(10000.);    // in ps 
     tofTestBeamClust->PosYMaxScal(2.);         //in % of length 
     break;
   case 2:                                      // time difference calibration
     //	tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
     // tofTestBeamClust->SetCalParFileName("MbsTrbThu0138_01tofTestBeamClust.hst.root");
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_00tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(10.);         //in % of length
     break;
   case 12:
     //	tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
     // tofTestBeamClust->SetCalParFileName("MbsTrbThu0138_01tofTestBeamClust.hst.root");
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_10tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(5.);         //in % of length
     break;
   case 22:
     //	tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
     // tofTestBeamClust->SetCalParFileName("MbsTrbThu0138_02tofTestBeamClust.hst.root");
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_20tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(300000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);         //in % of length
     break;
   case 3:
     //	 tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
     // tofTestBeamClust->SetCalParFileName("MbsTrbThu0138_01tofTestBeamClust.hst.root");
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_03tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(200000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);       //in % of length
     break;
   case 13:
     tofTestBeamClust->SetCalParFileName("MbsTrbThu224_13tofTestBeamClust.hst.root");
     tofTestBeamClust->SetTRefDifMax(50000.);   // in ps 
     tofTestBeamClust->PosYMaxScal(2.);       //in % of length
     break;
   default:
     cout << "<E> Calib mode not implemented! stop execution of script"<<endl;
     return;
   } 

   run->AddTask(tofTestBeamClust);
   CbmTofAnaTestbeam* tofAnaTestbeam = new CbmTofAnaTestbeam("TOF TestBeam Analysis",iVerbose);
   //run->AddTask(tofAnaTestbeam);
   */
   // =========================================================================
   // ===                  GO4 like output formatting                       ===
   // =========================================================================
//   TMbsConvTof* tofConversion = new TMbsConvTof("Tof Conversion", iVerbose);
//   run->AddTask(tofConversion);
   // ===                  End of output conversion                         ===
   // =========================================================================
   CbmTofOnlineDisplay* display = new CbmTofOnlineDisplay();
   display->SetUpdateInterval(1000);
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

