enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis(Bool_t generateCalib = kTRUE)
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

/*
      // --- Set the input files
      hldFileDir = "/store/Wuppertal_data_jul2015/threshold_scan_2/";
      source->AddInputFile(hldFileDir + "te15204202729.hld");
      source->AddInputFile(hldFileDir + "te15204202941.hld");
      source->AddInputFile(hldFileDir + "te15204203152.hld");
      source->AddInputFile(hldFileDir + "te15204203404.hld");
      source->AddInputFile(hldFileDir + "te15204203615.hld");
      source->AddInputFile(hldFileDir + "te15204203827.hld");
      source->AddInputFile(hldFileDir + "te15204204038.hld");
      source->AddInputFile(hldFileDir + "te15204204250.hld");
      source->AddInputFile(hldFileDir + "te15204204501.hld");
      source->AddInputFile(hldFileDir + "te15204204713.hld");
      source->AddInputFile(hldFileDir + "te15204204924.hld");
      source->AddInputFile(hldFileDir + "te15204205135.hld");
      source->AddInputFile(hldFileDir + "te15204205347.hld");
      source->AddInputFile(hldFileDir + "te15204205558.hld");
      source->AddInputFile(hldFileDir + "te15204205809.hld");
      source->AddInputFile(hldFileDir + "te15204210021.hld");
      source->AddInputFile(hldFileDir + "te15204210232.hld");
      source->AddInputFile(hldFileDir + "te15204210443.hld");
      source->AddInputFile(hldFileDir + "te15204210655.hld");
      source->AddInputFile(hldFileDir + "te15204210906.hld");
      source->AddInputFile(hldFileDir + "te15204211117.hld");
      source->AddInputFile(hldFileDir + "te15204211329.hld");
      source->AddInputFile(hldFileDir + "te15204211540.hld");
      source->AddInputFile(hldFileDir + "te15204211751.hld");
      source->AddInputFile(hldFileDir + "te15204212003.hld");
      source->AddInputFile(hldFileDir + "te15204212214.hld");

      hldFileDir = "/store/Wuppertal_data_jul2015/new_PADIWA_thr_scan_3/";
      source->AddInputFile(hldFileDir + "te15205220547.hld");
      source->AddInputFile(hldFileDir + "te15205222548.hld");
      source->AddInputFile(hldFileDir + "te15205224549.hld");
      source->AddInputFile(hldFileDir + "te15205230550.hld");
      source->AddInputFile(hldFileDir + "te15205232551.hld");
      source->AddInputFile(hldFileDir + "te15205234552.hld");
      source->AddInputFile(hldFileDir + "te15206000553.hld");
      source->AddInputFile(hldFileDir + "te15206002555.hld");
      source->AddInputFile(hldFileDir + "te15206004556.hld");
      source->AddInputFile(hldFileDir + "te15206010557.hld");
      source->AddInputFile(hldFileDir + "te15206012558.hld");

		hldFileDir = "/store/Wuppertal_data_jul2015/new_PADIWA_thr_scan_3/small/";
		source->AddInputFile(hldFileDir + "te15206000335.hld");
*/

      hldFileDir = "/store/cbm_rich_data_2014/WLS/WLS_off/nonstretched/ringD1/offset00100/";
		source->AddInputFile(hldFileDir + "te14326170143.hld");
		source->AddInputFile(hldFileDir + "te14326170350.hld");
		source->AddInputFile(hldFileDir + "te14326170559.hld");
		source->AddInputFile(hldFileDir + "te14326170755.hld");
		source->AddInputFile(hldFileDir + "te14326171000.hld");
      source->AddInputFile(hldFileDir + "te14326171202.hld");
		source->AddInputFile(hldFileDir + "te14326171408.hld");
		source->AddInputFile(hldFileDir + "te14326171613.hld");
		source->AddInputFile(hldFileDir + "te14326171818.hld");

//      source->AddInputFile(hldFileDir + "");

      runTitle = "WLS_analysis";
      outHistoFile = runTitle + ".histo.root";
   }

   source->SetOutHistoFile(outputDir + outHistoFile);

   CbmTrbCalibrator* fgCalibrator = CbmTrbCalibrator::Instance();
   fgCalibrator->SetCalibrationPeriod(50000000);
   fgCalibrator->SetInputFilename("calibration.root");
                                                             // does not actually import data - only defines
                                                             // the file that will be used if you specify mode etn_IMPORT
                                                             // Also note the (un)commented line in the end of the macro with export func
   //fgCalibrator->SetCorrInputFilename("/store/Wuppertal_data_jul2015/Corrections3.txt");
                                                             // Corrections are imported only in IMPORT mode

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

   CbmRichProtoAnalysis* unpack_qa = new CbmRichProtoAnalysis(); // or CbmRichTrbEventBuilder
   //unpack_qa->SetDrawHist(kTRUE);
   unpack_qa->SetRunTitle(runTitle);
   unpack_qa->SetOutputDir(outputDir);
   unpack_qa->SetOutHistoFile(outputDir + outHistoFile);
   run->AddTask(unpack_qa);

/*
   CbmRichEventDebug* eventDebug = new CbmRichEventDebug();
   eventDebug->SetOutHistoFile(outputDir + runTitle + ".histo2.root");
   run->AddTask(eventDebug);
*/

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
      fgCalibrator->Export("calibration.root");
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
