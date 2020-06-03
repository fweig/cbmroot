// -----------------------------------------------------------------------------
// ----- ana_calToDigi_get4 FairRoot macro                                 -----
// -----                                                                   -----
// ----- created by P.-A. Loizeau on 2015-04-30                            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void ana_calToDigi_get4(Int_t nEvents = 10, TString sFileId="Trb23Feb0930", Int_t calMode=1, Bool_t bGenCal = kFALSE)
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

   TString workDir  = gSystem->Getenv("VMCWORKDIR");
   TString paramDir = workDir + "/macro/fles/get4";
   TString dataDir  = workDir + "/macro/fles/get4/data";

   TObjString unpParFile = paramDir + "/parUnpack_get4.txt";
   parFileList->Add(&unpParFile);

   TObjString calParFile;
   calParFile = paramDir + "/parCalib_get4.txt";
   parFileList->Add(&calParFile);

   TObjString mapParFile;
   mapParFile = paramDir + "/parMapCernFeb2015_get4.txt";
   parFileList->Add(&mapParFile);

   TString sOutfileId;
   sOutfileId = sFileId;
   cout << " Output File tag "<< sOutfileId << endl;

   TString TofGeo="v14c";
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

   // Load geometry
   TGeoVolume* master=geoMan->GetTopVolume();
   master->SetVisContainers(1);
   master->Draw("ogl");

   // Input File
   TString sInputFile = dataDir + "/" + sOutfileId + ".root"; ;

   // Parameter file output
   TString parFile  = dataDir + "/calToDigi_" + sOutfileId + ".params.root";

   // Output file
   TString sOutFile = dataDir + "/calToDigi_" + sOutfileId + ".out.root";
   // ----    Debug option   -------------------------------------------------
   gDebug = 0;
   // ------------------------------------------------------------------------

   // -----   Timer   --------------------------------------------------------
   TStopwatch timer;
   timer.Start();
   // ------------------------------------------------------------------------

   gLogger->SetLogScreenLevel(logLevel.Data());

   // -----   Reconstruction run   -------------------------------------------
   FairRunAna *run= new FairRunAna();
   run->SetInputFile(  sInputFile);
   run->SetOutputFile( sOutFile);

   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================

   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================

   if( kFALSE == bGenCal )
   {
      // =========================================================================
      // ===                         Mapping                                   ===
      // =========================================================================
      TMbsMappingTof* tofMapping = new TMbsMappingTof("Tof Mapping", iVerbose);
      tofMapping->SetSaveDigis(kTRUE);
      run->AddTask(tofMapping);
      // ===                      End of Mapping                               ===
      // =========================================================================


      // =========================================================================
      // ===                     Cluster Building                              ===
      // =========================================================================
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

      tofTestBeamClust->SetBeamRefType(4);    // Test case
      tofTestBeamClust->SetBeamRefSm(0);
      tofTestBeamClust->SetBeamRefDet(0);
      tofTestBeamClust->SetBeamAddRefMul(-1);

      switch( calMode )
      {
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
            //   tofTestBeamClust->SetCalParFileName("MbsTrbBeam00tofTestBeamClust.hst.root");
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
      } // switch (calMode)
      run->AddTask(tofTestBeamClust);
      // ===                  End of Cluster Building                          ===
      // =========================================================================
   } // if( kFALSE == bGenCal )

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

   // -----   Initialize and run   --------------------------------------------
   run->Init();

   cout << "Starting run" << endl;
   run->Run(nEvents, 0);
   // ------------------------------------------------------------------------

   // -----   Finish   -------------------------------------------------------

   timer.Stop();
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   cout << endl << endl;
   cout << "Macro finished successfully." << endl;
   cout << "Output file is " << outFile << endl;
   cout << "Parameter file is " << parFile << endl;
   cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
   cout << endl;

   // ------------------------------------------------------------------------

   delete run;
}

