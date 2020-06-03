enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis3()
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
   Int_t nEvents = 2;

   // --- Set log output levels
   FairLogger::GetLogger()->SetLogScreenLevel("INFO");
   FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

   // --- Set debug level
   gDebug = 0;

   CbmFlibFileSourceNew* source = new CbmFlibFileSourceNew();

   if (script == "yes") {
      TString hldFullFileName = TString(gSystem->Getenv("INPUT_HLD_FILE"));

      source->SetFileName(hldFullFileName);

      outRootFileName = TString(gSystem->Getenv("OUTPUT_ROOT_FILE"));
      outputDir = TString(gSystem->Getenv("OUTPUT_DIR"));
      runTitle = TString(gSystem->Getenv("RUN_TITLE"));
      outHistoFile = TString(gSystem->Getenv("OUTPUT_HISTO_FILE"));
   } else {
      TString hldFileDir = "/store/cbm_rich_data_2014/flib/";
      source->SetFileName(hldFileDir + "1040_cern2014.tsa");

      runTitle = "flib_data";
      outHistoFile = runTitle + ".histo.root";
   }

   CbmTrbCalibrator* fgCalibrator = CbmTrbCalibrator::Instance();
   fgCalibrator->SetCalibrationPeriod(50000000);
   fgCalibrator->SetInputFilename("calibration_flib_1040.root");            // does not actually import data - only defines
                                                                  // the file that will be used if you specidy mode etn_IMPORT
   fgCalibrator->SetMode(etn_ONLINE);
                                                                  // Also note the (un)commented line in the end of the macro with export func

   CbmTSUnpackTrb2* trbUnpacker = new CbmTSUnpackTrb2();
   //source->AddUnpacker(trbUnpacker, 0x40);
   source->AddUnpacker(trbUnpacker, 0xe0);
   //source->AddUnpacker(trbUnpacker, 0xe1);

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

   CbmRichTrbUnpackQa* unpack_qa = new CbmRichTrbUnpackQa();
   unpack_qa->SetDrawHist(kTRUE);
   unpack_qa->SetRunTitle(runTitle);
   unpack_qa->SetOutputDir(outputDir);
   unpack_qa->SetOutHistoFile(outHistoFile);
   run->AddTask(unpack_qa);

/*
   CbmRichEventDebug* eventDebug = new CbmRichEventDebug();
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

   CbmRichTrbRecoQa* qaReco = new CbmRichTrbRecoQa();
   qaReco->SetMaxNofEventsToDraw(4);
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
   fgCalibrator->ForceCalibration();
   fgCalibrator->Export("calibration_flib_1040.root");

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

