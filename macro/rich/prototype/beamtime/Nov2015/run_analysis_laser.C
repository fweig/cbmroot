enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis_laser(Bool_t generateCalib = kTRUE,
									TString inputFile,
									TString outputFile,
									TString calibFile,
									TString histoFile,
									TString histo2File,
									TString fileBasename,
									Int_t inNevents = -1)
{
   TStopwatch timer;
   timer.Start();
   gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
   loadlibs();

   TString outputDir = "output/";
   TString runTitle;
   TString outHistoFile;

   TString script = TString(gSystem->Getenv("SCRIPT"));

   // --- Specify number of events to be produced.
   // --- -1 means run until the end of the input file.
   Int_t nEvents = inNevents;

   // --- Set log output levels
   FairLogger::GetLogger()->SetLogScreenLevel("INFO");
   FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

   // --- Set debug level
   gDebug = 0;

   CbmRichTrbUnpack2* source = new CbmRichTrbUnpack2();

   source->AddInputFile(inputFile);
   source->SetOutHistoFile(histoFile);

   CbmTrbCalibrator* fgCalibrator = CbmTrbCalibrator::Instance();
   fgCalibrator->SetCalibrationPeriod(50000000);
   fgCalibrator->SetInputFilename(calibFile);
                                                             // does not actually import data - only defines
                                                             // the file that will be used if you specify mode etn_IMPORT
                                                             // Also note the (un)commented line in the end of the macro with export func
   //fgCalibrator->SetCorrInputFilename("/store/cbm_rich_data_2014/Corrections_WLS_off_no29channels.txt");
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
   run->SetOutputFile(outputFile);
   //run->SetEventHeader(event);

   CbmTrbEdgeMatcher* matcher = new CbmTrbEdgeMatcher();
   matcher->SetDrawHits(kFALSE);
   run->AddTask(matcher);

   CbmRichTrbEventBuilder* unpack_qa = new CbmRichTrbEventBuilder();
   //unpack_qa->SetWlsState("off");
   unpack_qa->SetDrawHist(kTRUE);
   unpack_qa->SetRunTitle(runTitle);
   unpack_qa->SetOutputDir(outputDir);
   unpack_qa->SetOutHistoFile(histoFile);
   run->AddTask(unpack_qa);

   run->Init();

   // --- Start run
   TStopwatch timer;
   timer.Start();
   run->Run(nEvents, 0); // run until end of input file
   timer.Stop();

   // --- export calibration tables
   if (generateCalib) {
      fgCalibrator->ForceCalibration();
      fgCalibrator->Export(calibFile);
   }

   // You may try to draw the histograms showing which channels are calibrated
   //fgCalibrator->Draw();

   // --- End-of-run info
   Double_t rtime = timer.RealTime();
   Double_t ctime = timer.CpuTime();
   std::cout << std::endl << std::endl;
   std::cout << "Macro finished successfully." << std::endl;
   std::cout << "Output file is " << outputFile << std::endl;
   std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
   std::cout << " Test passed" << std::endl;
   std::cout << " All ok " << std::endl;
}
