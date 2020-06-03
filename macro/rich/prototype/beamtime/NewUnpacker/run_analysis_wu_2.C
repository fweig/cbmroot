enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis_wu_2(Bool_t generateCalib = kFALSE, Int_t tdcN = 0, Int_t inChannel = 1)
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
		hldFileDir = "/data/TRBdata_May15/Scan_direct_tdc_0010_refch_1/";
		switch (inChannel) {
		case 1: source->AddInputFile(hldFileDir + "te15134132956.hld"); break;	// Same file as 3!
		case 3: source->AddInputFile(hldFileDir + "te15134132956.hld"); break;
		case 5: source->AddInputFile(hldFileDir + "te15134133240.hld"); break;
		case 7: source->AddInputFile(hldFileDir + "te15134133516.hld"); break;
		case 9: source->AddInputFile(hldFileDir + "te15134133929.hld"); break;
		case 11: source->AddInputFile(hldFileDir + "te15134134214.hld"); break;
		case 13: source->AddInputFile(hldFileDir + "te15134134504.hld"); break;
		case 15: source->AddInputFile(hldFileDir + "te15134134751.hld"); break;
		case 17: source->AddInputFile(hldFileDir + "te15134135106.hld"); break;
		case 19: source->AddInputFile(hldFileDir + "te15134135351.hld"); break;
		case 21: source->AddInputFile(hldFileDir + "te15134135619.hld"); break;
		case 23: source->AddInputFile(hldFileDir + "te15134135918.hld"); break;
		case 25: source->AddInputFile(hldFileDir + "te15134140253.hld"); break;
		case 27: source->AddInputFile(hldFileDir + "te15134140548.hld"); break;
		case 29: source->AddInputFile(hldFileDir + "te15134140825.hld"); break;
		case 31: source->AddInputFile(hldFileDir + "te15134141138.hld"); break;
		default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		}
	} else if (tdcN == 1) {
		hldFileDir = "/data/TRBdata_May15/Scan_direct_tdc_0011_refch_1/";
		switch (inChannel) {
		case 1: source->AddInputFile(hldFileDir + "te15134141709.hld"); break;		// Same file as 3!
		case 3: source->AddInputFile(hldFileDir + "te15134141709.hld"); break;
		case 5: source->AddInputFile(hldFileDir + "te15134142119.hld"); break;
		case 7: source->AddInputFile(hldFileDir + "te15134142411.hld"); break;
		case 9: source->AddInputFile(hldFileDir + "te15134142658.hld"); break;
		case 11: source->AddInputFile(hldFileDir + "te15134143002.hld"); break;
		case 13: source->AddInputFile(hldFileDir + "te15134143248.hld"); break;
		case 15: source->AddInputFile(hldFileDir + "te15134143522.hld"); break;
		case 17: source->AddInputFile(hldFileDir + "te15134143746.hld"); break;
		case 19: source->AddInputFile(hldFileDir + "te15134144040.hld"); break;
		case 21: source->AddInputFile(hldFileDir + "te15134144321.hld"); break;
		case 23: source->AddInputFile(hldFileDir + "te15134144552.hld"); break;
		case 25: source->AddInputFile(hldFileDir + "te15134144821.hld"); break;
		case 27: source->AddInputFile(hldFileDir + "te15134145107.hld"); break;
		case 29: source->AddInputFile(hldFileDir + "te15134145347.hld"); break;
		case 31: source->AddInputFile(hldFileDir + "te15134145633.hld"); break;
		default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		}
	} else if (tdcN == 2) {
		hldFileDir = "/data/TRBdata_May15/Scan_direct_tdc_0012_refch_1/";
		switch (inChannel) {
		case 1: source->AddInputFile(hldFileDir + "te15134150452.hld"); break;		// Same file as 3!
		case 3: source->AddInputFile(hldFileDir + "te15134150452.hld"); break;
		case 5: source->AddInputFile(hldFileDir + "te15134150739.hld"); break;
		case 7: source->AddInputFile(hldFileDir + "te15134151002.hld"); break;
		case 9: source->AddInputFile(hldFileDir + "te15134151240.hld"); break;
		case 11: source->AddInputFile(hldFileDir + "te15134151517.hld"); break;
		case 13: source->AddInputFile(hldFileDir + "te15134151751.hld"); break;
		case 15: source->AddInputFile(hldFileDir + "te15134152031.hld"); break;
		case 17: source->AddInputFile(hldFileDir + "te15134152323.hld"); break;
		case 19: source->AddInputFile(hldFileDir + "te15134152549.hld"); break;
		case 21: source->AddInputFile(hldFileDir + "te15134152845.hld"); break;
		case 23: source->AddInputFile(hldFileDir + "te15134153118.hld"); break;
		case 25: source->AddInputFile(hldFileDir + "te15134153349.hld"); break;
		case 27: source->AddInputFile(hldFileDir + "te15134153623.hld"); break;
		case 29: source->AddInputFile(hldFileDir + "te15134153853.hld"); break;
		case 31: source->AddInputFile(hldFileDir + "te15134154125.hld"); break;
		default: printf ("Wrong channel defined as the macro parameter.\n"); return;
		}
	} else if (tdcN == 3) {
		hldFileDir = "/data/TRBdata_May15/Scan_direct_tdc_0013_refch_1/";
		switch (inChannel) {
		case 1: source->AddInputFile(hldFileDir + "te15134154818.hld"); break;		// Same file as 3!
		case 3: source->AddInputFile(hldFileDir + "te15134154818.hld"); break;
		case 5: source->AddInputFile(hldFileDir + "te15134155112.hld"); break;
		case 7: source->AddInputFile(hldFileDir + "te15134155423.hld"); break;
		case 9: source->AddInputFile(hldFileDir + "te15134155703.hld"); break;
		case 11: source->AddInputFile(hldFileDir + "te15134155944.hld"); break;
		case 13: source->AddInputFile(hldFileDir + "te15134160221.hld"); break;
		case 15: source->AddInputFile(hldFileDir + "te15134160446.hld"); break;
		case 17: source->AddInputFile(hldFileDir + "te15134160757.hld"); break;
		case 19: source->AddInputFile(hldFileDir + "te15134161049.hld"); break;
		case 21: source->AddInputFile(hldFileDir + "te15134161344.hld"); break;
		case 23: source->AddInputFile(hldFileDir + "te15134161719.hld"); break;
		case 25: source->AddInputFile(hldFileDir + "te15134161949.hld"); break;
		case 27: source->AddInputFile(hldFileDir + "te15134162218.hld"); break;
		case 29: source->AddInputFile(hldFileDir + "te15134162511.hld"); break;
		case 31: source->AddInputFile(hldFileDir + "te15134162818.hld"); break;
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

