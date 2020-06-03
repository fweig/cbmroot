// -----------------------------------------------------------------------------
// ----- analysis.C                                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-08-22                                 -----
// -----                                                                   -----
// ----- based on unpack_trb.C by C. Simon                                 -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/csimon/    -----
// -----   beamtimeApr14/macro/tof/beamtime/unpack_trb.C                   -----
// ----- revision 24438, 2014-04-13                                        -----
// -----------------------------------------------------------------------------

// Max nEvents: 198999999999
void batch_ana( Int_t iRunId = 3, Bool_t bCalib = kTRUE, Bool_t bMap = kFALSE, Int_t nEvents = -1)
{
   TString script = TString(gSystem->Getenv("ANA_SCRIPT"));
   Bool_t bScript = kFALSE;
   if( script != "yes" )
      bScript = kFALSE;
      else
      {
         bScript  = kTRUE;
         iRunId--; // Job Id on batch farm runs from 1 to N instead of 0 to N-1
      } // else of if( script != "yes" )

   // Check if run is in "database" and if yes prepare the strings for parameter file name,
   // LMD files list name and output file name
   // To add new run, just create the corresponding AddFileGsi_XXX.C file and add a new case
   TString sRunName   = "";
   TString sCalibName = "";
   switch( iRunId )
   {
      case 0:
      {
         sRunName   = "Thu09Test";
         sCalibName = "Thu09Test";
         break;
      } // case 0:
      case 1:
      {
         sRunName   = "Fri10Night";
         sCalibName = "Fri10Night";
         break;
      } // case 1:
      case 2:
      {
         sRunName   = "Fri10Day";
         sCalibName = "Fri10Day";
         break;
      } // case 2:
      case 3:
      {
         sRunName   = "Fri10Late";
         sCalibName = "Fri10Late";
         break;
      } // case 3:
      case 4:
      {
         sRunName   = "Sat11Night";
         sCalibName = "Sat11Night";
         break;
      } // case 4:
      case 5:
      {
         sRunName   = "Sat11Day_broken";
         sCalibName = "Sat11Day_broken";
         break;
      } // case 5:
      case 6:
      {
         sRunName   = "Sat11Late";
         sCalibName = "Sat11Late";
         break;
      } // case 6:
      case 7:
      {
         sRunName   = "Sun12Night";
         sCalibName = "Sun12Night";
         break;
      } // case 7:
      case 8:
      {
         sRunName   = "Sun12Day";
         sCalibName = "Sun12Day";
         break;
      } // case 8:
      case 9:
      {
         sRunName   = "Sun12Late";
         sCalibName = "Sun12Late";
         break;
      } // case 9:
      case 10:
      {
         sRunName   = "Mon13Night";
         sCalibName = "Mon13Night";
         break;
      } // case 10:
      default:
      {
         cout << "Unknown run Id: "<< iRunId <<" => Exit macro!!!" << endl;
         return;
      } // default:
   } // switch( iRunId )
   cout << "Input run Id: "<< iRunId <<" Run Name: "<< sRunName <<" Calib Name: "<< sCalibName << endl;

   //  Parameter files.
   // Add TObjectString containing the different file names to
   // a TList which is passed as input to the FairParAsciiFileIo.
   // The FairParAsciiFileIo will take care to create on the fly
   // a concatenated input parameter file.
   TList *parFileList = new TList();

   TString paramDir  = "/hera/cbm/users/tofGsiApr14/beamtimeSep14/source/macro/tof/beamtime/sep14/";
   TString outputDir = "/hera/cbm/users/tofGsiApr14/sep14/calib/";

   TObjString unpParFile = paramDir + "/parUnpackTower.txt";
   if( kTRUE == bCalib && kFALSE == bMap )
      unpParFile = paramDir + "/parUnpackTowerNoOut.txt";
   parFileList->Add(&unpParFile);

   TObjString calParFile = paramDir + "/parCalib.txt";
   if( kTRUE == bCalib && kFALSE == bMap)
      calParFile = paramDir + "/parCalibNoOut.txt";
      else calParFile = paramDir + Form("/parCalib_%s.txt", sRunName.Data() );
   parFileList->Add(&calParFile);

   TObjString mapParFile = paramDir + "/parMapGsiSep14.txt";
   parFileList->Add(&mapParFile);

   // Parameter file output
   TString parFile = outputDir + Form("/unpack_%s.params.root", sRunName.Data() );

   // Output file
   TString outFile = outputDir + Form("/unpack_%s.out.root", sRunName.Data() );

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


  // Add all files of chosen run
  TString sFileListName = paramDir + Form("AddFileGsi_%s.C", sRunName.Data() );
  ifstream filesList( sFileListName.Data() );
  if( filesList.fail() == true)
  {
     cout<<"Error opening the LMD file list "<< sFileListName <<endl;
     return;
  } // if( filesList.fail() == true)
  else
  {
     TString line;
     line.ReadLine(filesList, kFALSE);
     while( !filesList.eof() )
     {
        gROOT->ProcessLineFast(line);
        line.ReadLine(filesList, kFALSE);
     } // while( !filesList.eof() )

     filesList.close();
  } // else of if( filesList.fail() == true)

     // Add the TRIGLOG unpacker
   TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();
   source->AddUnpacker( tofTriglogUnpacker );

      // Add the MBS boards unpacker
   TMbsUnpackTof* tofMbsDataUnpacker = new TMbsUnpackTof();
   source->AddUnpacker( tofMbsDataUnpacker );

      // Add the TRB unpacker
   // = 1 TRB3: TTrbUnpackTof(0,0,31,0,0)
   // > 1 TRB3: TTrbUnpackTof(10,1,31,0,0)
   TTrbUnpackTof* tofTrbDataUnpacker = new TTrbUnpackTof(10,1,31,0,0);
   source->AddUnpacker( tofTrbDataUnpacker );

      // -----   Online/Offline MBS run   -----------------------------------
   FairRunOnline *run;
   if( kTRUE == bScript )
      run = new FairRunOnline();
      else run = FairRunOnline::Instance();

   run->SetSource(source);
   run->SetOutputFile(outFile);
   run->SetAutoFinish(kFALSE);

   gLogger->SetLogScreenLevel("INFO");
//   gLogger->SetLogScreenLevel("DEBUG");

      // --------------------------------------------------------------------
   // ===                        End of Unpacker                            ===
   // =========================================================================

   // =========================================================================
   // ===                     Unpacker monitoring                           ===
   // =========================================================================
   TMbsUnpTofMonitor* tofUnpMonitor = new TMbsUnpTofMonitor("Tof Unp Moni", 2);
   run->AddTask(tofUnpMonitor);
   // ===                 End of Unpacker monitoring                        ===
   // =========================================================================

   // =========================================================================
   // ===                         Calibration                               ===
   // =========================================================================
   TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", 2);
   if( kTRUE == bCalib )
   {
      tofCalibration->SetSaveTdcs(kFALSE);
      tofCalibration->SetTdcCalibFilename( sCalibName);
      tofCalibration->SetTdcCalibOutFoldername( "/hera/cbm/users/tofGsiApr14/sep14/calib/" );
   } // if( kTRUE == bCalib )
   else tofCalibration->SetSaveTdcs(kTRUE);
   run->AddTask(tofCalibration);
   // ===                      End of Calibration                           ===
   // =========================================================================

   // =========================================================================
   // ===                         Mapping                                   ===
   // =========================================================================
   TMbsMappingTof* tofMapping;
   if( kTRUE == bMap)
   {
      tofMapping = new TMbsMappingTof("Tof Mapping", 2);
      run->AddTask(tofMapping);
   } // if( kTRUE == bMap)
   // ===                      End of Mapping                               ===
   // =========================================================================


   // =========================================================================
   // ===                         Monitoring                                ===
   // =========================================================================
   CbmTofOnlineDisplay* display;
   if( kFALSE == bScript )
   {
      display = new CbmTofOnlineDisplay();
      display->SetUpdateInterval(10000);
      display->SetNumberOfTDC(38);
   //   display->SetResMonitorEna( kTRUE );
      run->AddTask(display);
   } // if( kFALSE == bScript )
   // ===                      End of Monitoring                            ===
   // =========================================================================

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

   if( kTRUE == bScript )
   {
      cout << "Starting run" << endl;
      run->Run(nEvents);
      run->Finish();
   } //if( kTRUE == bScript )
   // ------------------------------------------------------------------------

//   gLogger->SetLogScreenLevel("FATAL");

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
}

