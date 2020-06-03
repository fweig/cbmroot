enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis_wu_2_ip(Bool_t generateCalib = kFALSE, Int_t tdcN = 0, Int_t inChannel = 1)
{
   TStopwatch timer;
   timer.Start();
   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();

   TString outputDir = "output/";
   TString outRootFileName = outputDir + "testtest.root";
   TString runTitle;
   TString outHistoFile;

   TString script = TString(gSystem->Getenv("SCRIPT"));

   // --- Specify number of events to be produced.
   // --- -1 means run until the end of the input file.
   Int_t nEvents = -1;

   // --- Set log output levels
   FairLogger::GetLogger()->SetLogScreenLevel("INFO");
   FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

   // --- Set debug level
   gDebug = 0;

   CbmRichTrbUnpack2* source = new CbmRichTrbUnpack2();

   if (script == "yes") {
      TString hldFullFileName = TString(gSystem->Getenv("INPUT_HLD_FILE"));

      source->AddInputFile(hldFullFileName);

      outRootFileName = TString(gSystem->Getenv("OUTPUT_ROOT_FILE"));
      outputDir = TString(gSystem->Getenv("OUTPUT_DIR"));
      runTitle = TString(gSystem->Getenv("RUN_TITLE"));
      outHistoFile = TString(gSystem->Getenv("OUTPUT_HISTO_FILE"));
   } else {
      TString hldFileDir;

   	// --- Set the input files

      if (tdcN == 0) {
		   hldFileDir = "/data/TRBdata_May15/InvPol_Scan_direct_tdc_0010_refch_1/";
		   switch (inChannel) {
		   case 1: source->AddInputFile(hldFileDir + "te15134194201.hld"); break;	// Same file as 3!
		   case 3: source->AddInputFile(hldFileDir + "te15134194201.hld"); break;
		   case 5: source->AddInputFile(hldFileDir + "te15134194452.hld"); break;
		   case 7: source->AddInputFile(hldFileDir + "te15134194714.hld"); break;
		   case 9: source->AddInputFile(hldFileDir + "te15134194948.hld"); break;
		   case 11: source->AddInputFile(hldFileDir + "te15134195233.hld"); break;
		   case 13: source->AddInputFile(hldFileDir + "te15134195504.hld"); break;
		   case 15: source->AddInputFile(hldFileDir + "te15134195758.hld"); break;
		   case 17: source->AddInputFile(hldFileDir + "te15134200030.hld"); break;
		   case 19: source->AddInputFile(hldFileDir + "te15134200255.hld"); break;
		   case 21: source->AddInputFile(hldFileDir + "te15134200523.hld"); break;
		   case 23: source->AddInputFile(hldFileDir + "te15134200750.hld"); break;
		   case 25: source->AddInputFile(hldFileDir + "te15134201019.hld"); break;
		   case 27: source->AddInputFile(hldFileDir + "te15134201247.hld"); break;
		   case 29: source->AddInputFile(hldFileDir + "te15134201507.hld"); break;
		   case 31: source->AddInputFile(hldFileDir + "te15134201730.hld"); break;
		   default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		   }
	   } else if (tdcN == 1) {
		   hldFileDir = "/data/TRBdata_May15/InvPol_Scan_direct_tdc_0011_refch_1/";
		   switch (inChannel) {
		   case 1: source->AddInputFile(hldFileDir + "te15134202031.hld"); break;	// Same file as 3!
		   case 3: source->AddInputFile(hldFileDir + "te15134202031.hld"); break;
		   case 5: source->AddInputFile(hldFileDir + "te15134202256.hld"); break;
		   case 7: source->AddInputFile(hldFileDir + "te15134202522.hld"); break;
		   case 9: source->AddInputFile(hldFileDir + "te15134202812.hld"); break;
		   case 11: source->AddInputFile(hldFileDir + "te15134203033.hld"); break;
		   case 13: source->AddInputFile(hldFileDir + "te15134203300.hld"); break;
		   case 15: source->AddInputFile(hldFileDir + "te15134203612.hld"); break;
		   case 17: source->AddInputFile(hldFileDir + "te15134203844.hld"); break;
		   case 19: source->AddInputFile(hldFileDir + "te15134204123.hld"); break;
		   case 21: source->AddInputFile(hldFileDir + "te15134204404.hld"); break;
		   case 23: source->AddInputFile(hldFileDir + "te15134204643.hld"); break;
		   case 25: source->AddInputFile(hldFileDir + "te15134204917.hld"); break;
		   case 27: source->AddInputFile(hldFileDir + "te15134205159.hld"); break;
		   case 29: source->AddInputFile(hldFileDir + "te15134205424.hld"); break;
		   case 31: source->AddInputFile(hldFileDir + "te15134205653.hld"); break;
		   default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		   }
	   } else if (tdcN == 2) {
		   hldFileDir = "/data/TRBdata_May15/InvPol_Scan_direct_tdc_0012_refch_1/";
		   switch (inChannel) {
		   case 1: source->AddInputFile(hldFileDir + "te15134210142.hld"); break;	// Same file as 3!
		   case 3: source->AddInputFile(hldFileDir + "te15134210142.hld"); break;
		   case 5: source->AddInputFile(hldFileDir + "te15134210405.hld"); break;
		   case 7: source->AddInputFile(hldFileDir + "te15134210631.hld"); break;
		   case 9: source->AddInputFile(hldFileDir + "te15134210930.hld"); break;
		   case 11: source->AddInputFile(hldFileDir + "te15134211155.hld"); break;
		   case 13: source->AddInputFile(hldFileDir + "te15134211413.hld"); break;
		   case 15: source->AddInputFile(hldFileDir + "te15134211643.hld"); break;
		   case 17: source->AddInputFile(hldFileDir + "te15134211904.hld"); break;
		   case 19: source->AddInputFile(hldFileDir + "te15134212137.hld"); break;
		   case 21: source->AddInputFile(hldFileDir + "te15134212404.hld"); break;
		   case 23: source->AddInputFile(hldFileDir + "te15134212710.hld"); break;
		   case 25: source->AddInputFile(hldFileDir + "te15134212926.hld"); break;
		   case 27: source->AddInputFile(hldFileDir + "te15134213148.hld"); break;
		   case 29: source->AddInputFile(hldFileDir + "te15134213413.hld"); break;
		   case 31: source->AddInputFile(hldFileDir + "te15134213647.hld"); break;
		   default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		   }
	   } else if (tdcN == 3) {
		   hldFileDir = "/data/TRBdata_May15/InvPol_Scan_direct_tdc_0013_refch_1/";
		   switch (inChannel) {
		   case 1: source->AddInputFile(hldFileDir + "te15134214141.hld"); break;	// Same file as 3!
		   case 3: source->AddInputFile(hldFileDir + "te15134214141.hld"); break;
		   case 5: source->AddInputFile(hldFileDir + "te15134214409.hld"); break;
		   case 7: source->AddInputFile(hldFileDir + "te15134214636.hld"); break;
		   case 9: source->AddInputFile(hldFileDir + "te15134214905.hld"); break;
		   case 11: source->AddInputFile(hldFileDir + "te15134215155.hld"); break;
		   case 13: source->AddInputFile(hldFileDir + "te15134215430.hld"); break;
		   case 15: source->AddInputFile(hldFileDir + "te15134215650.hld"); break;
		   case 17: source->AddInputFile(hldFileDir + "te15134215922.hld"); break;
		   case 19: source->AddInputFile(hldFileDir + "te15134220141.hld"); break;
		   case 21: source->AddInputFile(hldFileDir + "te15134220400.hld"); break;
		   case 23: source->AddInputFile(hldFileDir + "te15134220622.hld"); break;
		   case 25: source->AddInputFile(hldFileDir + "te15134220843.hld"); break;
		   case 27: source->AddInputFile(hldFileDir + "te15134221058.hld"); break;
		   case 29: source->AddInputFile(hldFileDir + "te15134221321.hld"); break;
		   case 31: source->AddInputFile(hldFileDir + "te15134221537.hld"); break;
		   default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		   }
	   } else {
		   printf ("Wrong TDC defined as the macro parameter.\n");
		   return;
	   }

      runTitle = "Wuppertal_analysis";

      outHistoFile = runTitle + ".histo.root";
   }

   source->SetOutHistoFile(outputDir + outHistoFile);

   CbmTrbCalibrator* fgCalibrator = CbmTrbCalibrator::Instance();
   fgCalibrator->SetCalibrationPeriod(50000000);
   fgCalibrator->SetInputFilename("calibration_wu.root");    // does not actually import data - only defines
                                                             // the file that will be used if you specify mode etn_IMPORT
                                                             // Also note the (un)commented line in the end of the macro with export func
   if (generateCalib) {
      fgCalibrator->SetMode(etn_ONLINE);
   } else {
      fgCalibrator->SetMode(etn_IMPORT);
   }

   // --- Event header
   //FairEventHeader* event = new CbmTbEvent();
   //event->SetRunId(260);

   // --- Run
   FairRunOnline *run = new FairRunOnline(source);
   run->SetOutputFile(outRootFileName);
   //run->SetEventHeader(event);

   CbmTrbEdgeMatcher* matcher = new CbmTrbEdgeMatcher();
   matcher->SetDrawHits(kFALSE);
   run->AddTask(matcher);

   CbmRichTrbEventBuilder* unpack_qa = new CbmRichTrbEventBuilder();
   unpack_qa->SetDrawHist(kTRUE);
   unpack_qa->SetRunTitle(runTitle);
   unpack_qa->SetOutputDir(outputDir);
   unpack_qa->SetOutHistoFile(outputDir + outHistoFile);
   run->AddTask(unpack_qa);

/*
   CbmRichEventDebug* eventDebug = new CbmRichEventDebug();
   eventDebug->SetOutHistoFile(outputDir + runTitle + ".histo2.root");
   run->AddTask(eventDebug);
*/
/*
   CbmRichReconstruction* richReco = new CbmRichReconstruction();
   richReco->SetFinderName("hough_prototype");
   richReco->SetRunTrackAssign(false);
   richReco->SetRunExtrapolation(false);
   richReco->SetRunProjection(false);
   richReco->SetRunFitter(false);
   run->AddTask(richReco);

   CbmRichRingHitsAnalyser* richRingHitsAna = new CbmRichRingHitsAnalyser();
   richRingHitsAna->SetOutHistoFile(outputDir + runTitle + ".histo2.root");
   run->AddTask(richRingHitsAna);
*/
/*
   CbmRichTrbRecoQa* qaReco = new CbmRichTrbRecoQa();
   qaReco->SetMaxNofEventsToDraw(6);
   qaReco->SetOutputDir(outputDir);
   qaReco->SetRunTitle(runTitle);
   qaReco->SetDrawHist(true);
   run->AddTask(qaReco);
*/

   run->Init();

   // --- Start run
   TStopwatch timer;
   timer.Start();
   run->Run(nEvents, 0); // run until end of input file
   timer.Stop();

   // --- export calibration tables
   if (generateCalib) {
      fgCalibrator->ForceCalibration();
      fgCalibrator->Export("calibration_wu.root");
   }

   // You may try to draw the histograms showing which channels are calibrated
   //fgCalibrator->Draw();

   // --- End-of-run info
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   std::cout << std::endl << std::endl;
   std::cout << "Macro finished successfully." << std::endl;
   std::cout << "Output file is " << outRootFileName << std::endl;
   std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
   std::cout << " Test passed" << std::endl;
   std::cout << " All ok " << std::endl;
}

