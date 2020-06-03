/** @file readLmd_calibr.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since December 2013
 ** @date 25.02.2014
 **
 ** @author Anna Senger <a.senger@gsi.de>
 ** @date 2015
 ** ROOT macro to read lmd files from beamtime DEC2013 - DEC2014 and convert the data
 ** into cbmroot format & data calibration.
 ** Uses CbmSourceLmd as source task and CbmStsCosyBL for calibration
 */

void readLmd_calibr(Int_t runID, Int_t fileID)
{
	Int_t Version = 2014; // Cosy
		
	TString runN = TString::Itoa(runID,10); // run ID
	TString fileN = TString::Itoa(fileID,10); // 10 files
	TString version = TString::Itoa(Version,10); // Cosy

	// --- Specify input file name (this is just an example)
		
	TString inDir = "/hera/cbm/prod/beamtime/2014/12/cosy/";
	if(Version==2013)inDir = "/hera/cbm/prod/beamtime/2013/12/cosy/run" + runN + "/";
	
	TString setup = "";
	if(runID > 0 && runID < 12)setup = "_hodo_only";
	else if(runID > 11 && runID < 15)setup = "_hodo_sts";
	else if(runID == 15)setup = "_hodo_sts1";  
	else if(runID == 16)setup = "_hodo_sts2";
	else if(runID == 20)setup = "_nobeam";
	else if(runID == 21)setup = "_nobeam_baselines_first";
	else if(runID > 21 && runID < 32 || runID == 54 || runID == 78 || runID == 79 || runID == 81)setup = "";
	else if(runID == 32 || runID == 45)setup = "_0mm_0deg";
	else if(runID == 33 || runID == 42)setup = "_0mm_30deg";
	else if(runID == 34)setup = "_0mm_20deg";
	else if(runID == 35)setup = "_0mm_15deg";
	else if(runID == 36)setup = "_0mm_10deg";
	else if(runID == 37)setup = "_0mm_5deg";
	else if(runID == 38)setup = "_0mm_3deg";
	else if(runID == 39)setup = "_0mm_-5deg";
	else if(runID == 40)setup = "_0mm_-10deg";
	else if(runID == 41)setup = "_0mm_-15deg";
	else if(runID == 43)setup = "_4mm_30deg";
	else if(runID == 44)setup = "_-4mm_30deg";
	else if(runID == 46)setup = "_-2mm_0deg";
	else if(runID == 47)setup = "_0mm_25deg";
	else if(runID == 48)setup = "_0mm_18deg";
	else if(runID == 49)setup = "_-6mm_18deg";
	else if(runID == 50)setup = "_-12mm_18deg";
	else if(runID == 51)setup = "_-12mm_25deg";
	else if(runID == 52)setup = "_-18mm_30deg";
	else if(runID == 53)setup = "_0mm_8deg";
	else if(runID == 55)setup = "_-15mm";
	else if(runID == 56)setup = "_-10mm";
	else if(runID == 57 || runID == 86)setup = "_-5mm";
	else if(runID == 58)setup = "_-2.5mm";
	else if(runID == 59)setup = "_-2.5mm";  
	else if(runID == 60)setup = "_10mm";  
	else if(runID == 61)setup = "_-7.5mm";  
	else if(runID == 62)setup = "_sts3exttrig";  
	else if(runID == 63)setup = "_sts3exttrig_polswap";  
	else if(runID > 63 && runID < 67)setup = "_0mm";  
	else if(runID > 67 && runID < 72 || runID == 74)setup = "_0mm";  
	else if(runID == 72)setup = "_3mm";  
	else if(runID == 73 || runID == 87)setup = "_2mm";  
	else if(runID == 75)setup = "_sts3exttrig_0mm";  
	else if(runID == 76)setup = "_sts3exttrig_0mm_nobeam";  
	else if(runID == 77 || runID == 80)setup = "_0mm_selftriggered";  
	else if(runID == 82 || runID == 84)setup = "_-1mm";  
	else if(runID == 83)setup = "_-3mm";  
	else if(runID == 85)setup = "_-2mm";  
	else if(runID == 88)setup = "_1.5mm";  
	else if(runID == 89 || runID == 105 || runID == 123)setup = "_production_selftrigg";  
	else if(runID == 90 || runID == 89)setup = "_production_sts3exttrig";
	else if(runID == 92)setup = "_-450V_selftrigg";  
	else if(runID == 93)setup = "_-450V_sts3exttrig";  
	else if(runID == 94)setup = "_-400V_sts3exttrig";  
	else if(runID == 95)setup = "_-400V_selftrigg";  
	else if(runID == 96)setup = "_-350V_selftrigg";  
	else if(runID == 97)setup = "_-350V_sts3exttrig";  
	else if(runID == 98)setup = "_-300V_sts3exttrig";  
	else if(runID == 99)setup = "_-300V_selftrigg";  
	else if(runID == 100)setup = "_-250V_selftrigg";  
	else if(runID == 101)setup = "_-250V_sts3exttrig";  
	else if(runID == 102 || runID == 104 || runID == 119 || runID == 120)setup = "_selftrigg";  
	else if(runID == 103 || runID == 121)setup = "_-0.5mm_selftrigg";  
	else if(runID == 106 || runID == 124 || runID == 127)setup = "_production_sts3exttrig";  
	else if(runID == 107)setup = "_voltagescan470_selftrigg";  
	else if(runID == 108)setup = "_voltagescan470_sts3exttrig";  
	else if(runID == 109)setup = "_voltagescan450_sts3exttrig";  
	else if(runID == 110)setup = "_voltagescan450_selftrigg";  
	else if(runID == 111)setup = "_voltagescan400_selftrigg";  
	else if(runID == 112)setup = "_voltagescan400_sts3exttrig";  
	else if(runID == 113)setup = "_voltagescan350_sts3exttrig";  
	else if(runID == 114)setup = "_voltagescan350_selftrigg";  
	else if(runID == 115)setup = "_voltagescan300_selftrigg";  
	else if(runID == 116)setup = "_voltagescan300_sts3exttrig";  
	else if(runID == 117)setup = "_voltagescan251_sts3exttrig";  
	else if(runID == 118)setup = "_voltagescan251_selftrigg";  
	else if(runID == 122)setup = "_0.25mm_selftrigg";  
	else if(runID == 125)setup = "_nobeam_selftrigg_150V";  
	else if(runID == 126)setup = "_production_selftrigg_150V";  
	else if(runID == 128)setup = "_lowvth_selftrigg_150V";  
	  
	if(runID < 10)inFile = inDir + "run00" + runN + setup + "_00" + fileN + "?.lmd";
	else if(runID > 9 && runID < 100)inFile = inDir + "run0" + runN + setup + "_00" + fileN + "?.lmd";
	else if(runID > 99)inFile = inDir + "run" + runN + setup + "_00" + fileN + "?.lmd";
	  
	if(Version==2013)inFile = inDir + "run" + runN + "_00" + fileN + "?.lmd";
	
	// --- Specify number of events to be produced.
	// --- -1 means run until the end of the input file.
	Int_t nEvents = -1;

	// --- Specify output file name (this is just an example)
	TString outDir = "data/Cosy" + version + "/";
	
	TString outFile = outDir + "calib.run" + runN + "_" + fileN + ".root";	

	// --- Set log output levels
	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

	// --- Set debug level
  gDebug = 0;
  
  std::cout << std::endl;
  std::cout << ">>> readLmd:  input file is " << inFile  << std::endl;
  std::cout << ">>> readLmd: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> readLmd: Initialising..." << std::endl;
  
  // --- Sets
  Bool_t trigger = kTRUE;   // kFALSE - self-trigger; kTRUE - external trigger
  
  //if(setup->Contains("selftrigg"))trigger = kFALSE; else trigger = kTRUE; 
  
  Int_t triggered_station = 1;  // Cosy 2014: first (0) - STS1, second (1) - STS2/3, third (2) - STS0
  Int_t RocNr1 = 6;
  Int_t RocNr2 = 7;
  
   if(Version==2013){triggered_station = 1; RocNr1 = 10; RocNr2 = 11;}// Cosy 2013: first (0) - STS0, second (1) - STS2, third (2) - STS1
 
  // Create the correct mapping for the beamtime
  if(Version==2014)
  {
      CbmDaqMapCosy2014* daqMap = new CbmDaqMapCosy2014(runID);
  }
  else if(Version==2013)CbmDaqMapCosy2013* daqMap = new CbmDaqMapCosy2013();

  // --- Source task
  
  CbmSourceLmd* source = new CbmSourceLmd();
  source->SetDaqMap(daqMap);
  source->SetTriggeredMode(trigger);
  source->SetTriggeredRocNumber(RocNr1,RocNr2); 
  source->AddFile(inFile);
  
  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(runID);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  
  // --- Calibration  
  StsCosyBL* hist = new StsCosyBL();
  hist->SetTriggeredMode(trigger);
  hist->SetTriggeredStation(triggered_station);
  run->AddTask(hist);
  
  run->Init();
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> readLmd: Starting run..." << std::endl;
  run->Run(nEvents, 0); // run until end of input file
  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> readLmd: Macro finished successfully." << std::endl;
  std::cout << ">>> readLmd: Output file is " << outFile << std::endl;
  std::cout << ">>> readLmd: Real time " << rtime << " s, CPU time "
  					 << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
