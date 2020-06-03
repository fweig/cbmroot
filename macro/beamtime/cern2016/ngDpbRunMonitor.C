/** @file FHodoLabSetup
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 **
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline *run = NULL;

void ngDpbRunMonitor(Int_t iRun)
{   
   const Int_t kiNbRuns = 87;
   Int_t iPhase[kiNbRuns]   =
      {
               2, // Run 1 named following different pattern => not usable yet!!
               2,        2,        2,        2,        2,        2,
               2,        2,        2,        2,        2,        2,
               2,        2,        2,        2,        2,        2,
               2,        2,        2,        2,        2,        2,
               2,        2,        2,        2,        2,        2,
               2,        2,        2,        2,
               2,        3,        3,        3, // Run 36 failed test by MUCH, 37-39 ???
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3,
               3,        3,        3,        3,        3,        3
      };
   Int_t iRunId[kiNbRuns]   =
      {
               1, // Run 1 named following different pattern => not usable yet!!
               2,        3,        4,        5,        6,        7,
               8,        9,       10,       11,       12,       13,
              14,       15,       16,       17,       18,       19,
              20,       21,       22,       23,       24,       25,
              26,       27,       28,       29,       30,       31,
              32,       33,       34,       35,
              36,       37,       38,       39, // Run 36 failed test by MUCH, 37-39 ???
              40,       41,       42,       43,       44,       45,
              46,       47,       48,       49,       50,       51,
              52,       53,       54,       55,       56,       57,
              58,       59,       60,       61,       62,       63,
              64,       65,       66,       67,       68,       69,
              70,       71,       72,       73,       74,       75,
              76,       77,       78,       79,       80,       81,
              82,       83,       84,       85,       86,       87
      };
   Int_t iDate[kiNbRuns]    =
      {
        20161122, // Run 1 named following different pattern => not usable yet!!
        20161122, 20161122, 20161122, 20161122, 20161122, 20161122,
        20161122, 20161122, 20161122, 20161122, 20161122, 20161122,
        20161122, 20161122, 20161122, 20161122, 20161123, 20161123,
        20161123, 20161123, 20161123, 20161123, 20161123, 20161123,
        20161123, 20161123, 20161123, 20161123, 20161123, 20161123,
        20161123, 20161123, 20161123, 20161123,
        20161127, 20161130, 20161130, 20161130, // Run 36 failed test by MUCH, 37-39 ???
        20161201, 20161201, 20161201, 20161201, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202,
        20161202, 20161202, 20161202, 20161202, 20161202, 20161202
      };
   Int_t iTime[kiNbRuns]    =
      {
             944, // Run 1 named following different pattern => not usable yet!!
             954,     1036,     1112,     1519,     1529,     1608,
            1642,     1749,     1815,     1844,     1917,     2112,
            2136,     2212,     2249,     2326,        8,       45,
             122,      152,      227,      300,      334,      346,
             350,      353,      356,      406,      444,      516,
             538,      600,      606,      610,
            1426,     1200,     1300,     1400, // Run 36 failed test by MUCH, 37-39 ???
            1839,     2008,     2053,     2335,       17,       51,
             130,      211,      253,      323,      336,      409,
             437,      503,      535,      538,      629,      728,
             749,      756,      828,      832,      849,      903,
             922,      928,      933,      945,      948,      950,
             953,      954,      957,      958,     1000,     1002,
            1004,     1006,     1011,     1013,     1015,     1018,
            1021,     1023,     1025,     1027,     1031,     1033
      };

   // Empty or current files listed as runs with 
   Int_t iNbFiles[kiNbRuns] =
      {
               0, // Run 1 named following different pattern => not usable yet!!
               2,        1,        0,        2,        6,        7,
               5,        6,        6,        7,       22,        5,
               7,        8,        6,        8,        7,        7,
               6,        7,        7,        6,        2,        1,
               0,        0,        0,        7,        6,        6,
               1,        0,        0,        0,
               0,        0,        0,        0, // Run 36 failed test by MUCH, 37-39 ???
               7,       10,       17,        9,        8,        8,
               8,        9,        7,        2,        7,        7,
               6,        6,        1,       11,        5,        4,
               2,        8,        1,        4,        3,        4,
               1,        1,        1,        1,        1,        1, // Probably pedestal runs from MUCH with pulser trigger?
               1,        1,        1,        1,        1,        1, // Probably pedestal runs from MUCH with pulser trigger?
               1,        1,        1,        1,        1,        1, // Probably pedestal runs from MUCH with pulser trigger?
               1,        1,        1,        1,        1,        1  // Probably pedestal runs from MUCH with pulser trigger?
      };
   
   if( iRun < 0 || kiNbRuns <= iRun )
   {
      std::cout << ">>> ngDpbRunMonitor: Invalid run index: " << iRun << std::endl;
      std::cout << ">>>>>>>>>>>>>>>>>>>> Valid indices are within [0; " << kiNbRuns << "]" << std::endl;
      return;
   } // if( iRun < 0 || kiNbRuns <= iRun )
   
   if( 0 == iNbFiles[iRun] )
   {
      std::cout << ">>> ngDpbRunMonitor: 0 tsa files listed for run index: " << iRun << std::endl;
      std::cout << ">>>>>>>>>>>>>>>>>>>> Could be either a bad run or a not listed yet run, please check!" << std::endl;
      return;
   } // if( 0 == iNbFiles[iRun] )
   
   std::cout << ">>> ngDpbRunMonitor: Analysing run " 
             << Form( "phase%1d r%04d_%08d_%04d", iPhase[iRun], iRunId[iRun], iDate[iRun], iTime[iRun])
             << std::endl;
   
  TString inFile = "";
  TString srcDir = gSystem->Getenv("VMCWORKDIR");
//  TString inDir  = srcDir + "/input/";
  TString inDir  = "/lustre/nyx/cbm/prod/beamtime/2016/11/cern/";
  if( "" != inFile )
   inFile = inDir + inFile;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
//  Int_t nEvents = 10000;
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
//  TString outFile = "data/test.root";
//  TString parFile = "data/testparam.root";
  TString outFile = Form( "data/phase%1d_r%04d_%08d_%04d.root",
                  iPhase[iRun], iRunId[iRun], iDate[iRun], iTime[iRun] );
  TString parFile = Form( "data/phase%1d_r%04d_%08d_%04d_param.root",
                  iPhase[iRun], iRunId[iRun], iDate[iRun], iTime[iRun] );

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList *parFileList = new TList();
  TString paramDir = "./";

  TString paramFileMuch = paramDir + "MuchUnpackPar.par";
  TObjString* tutDetDigiFileMuch = new TObjString(paramFileMuch);
  parFileList->Add(tutDetDigiFileMuch);

  TString paramFileTof = paramDir + "MapCern2016.par";
  TObjString* tutDetDigiFileTof = new TObjString(paramFileTof);
  parFileList->Add(tutDetDigiFileTof);

  // --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> ngDpbRunMonitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> ngDpbRunMonitor: Initialising..." << std::endl;

  // Dummy Unpacker
//  CbmTSUnpackDummy*    dummy_unpacker     = new CbmTSUnpackDummy();
  
  // Much Monitor
  CbmTSMonitorMuch* test_monitor_much = new CbmTSMonitorMuch();
  test_monitor_much->SetRunStart( iDate[iRun], iTime[iRun] * 100 ); // Add two 0s to Time for seconds
  
  // Get4 Unpacker
  CbmTSMonitorTof* test_monitor_tof = new CbmTSMonitorTof();
  test_monitor_tof->SetDiamondChannels();
  test_monitor_tof->SetDiamondPerTsSpillOnThr(  2 );
  test_monitor_tof->SetDiamondPerTsSpillOffThr( 1 );
  test_monitor_tof->SetDiamondTsNbSpillOffThr( 50 );
  test_monitor_tof->SetRunStart( iDate[iRun], iTime[iRun] * 100 ); // Add two 0s to Time for seconds

  // --- Source task
  CbmFlibCern2016Source* source = new CbmFlibCern2016Source();

  TString sFileBase = inDir + Form( "phase%1d/r%04d_%08d_%04d/r%04d_%08d_%04d_",
              iPhase[iRun], iRunId[iRun], iDate[iRun], iTime[iRun], iRunId[iRun], iDate[iRun], iTime[iRun]);
  for( Int_t iFile = 0; iFile < iNbFiles[iRun]; ++iFile)
  {
     inFile = sFileBase + Form( "%04d.tsa", iFile );
     source->AddFile(inFile);
  }

  source->AddUnpacker(test_monitor_tof,  0x60, 6); //gDPBs
  source->AddUnpacker(test_monitor_much, 0x10, 4); //nDPBs
//   source->AddUnpacker(dummy_unpacker, 0x10, 4);//gDPB A & B
//   source->AddUnpacker(dummy_unpacker, 0x60, 6);//gDPB A & B

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  run->ActivateHttpServer(100); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> ngDpbRunMonitor: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();

  run->Finish();
  
  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;
    
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> ngDpbRunMonitor: Macro finished successfully." << std::endl;
  std::cout << ">>> ngDpbRunMonitor: Output file is " << outFile << std::endl;
  std::cout << ">>> ngDpbRunMonitor: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
