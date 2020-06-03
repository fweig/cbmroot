enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

void run_analysis_wu(Bool_t generateCalib = kFALSE, Int_t inChannel = 1)
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
/*
      // Set of data from tdc 0x0010 ref ch 31
      hldFileDir = "/data/TRBdata_May15/Direct_TDC_scan_tdc0010/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127153937.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127154139.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127154321.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127154515.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15127155034.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15127155224.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15127155407.hld"); break;
      case 15: source->AddInputFile(hldFileDir + "te15127155551.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15127155750.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15127155929.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15127160129.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15127160307.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15127160559.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15127160755.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15127161011.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15127161011.hld"); break;  // same file as for ch 29, just for consistency
      }
*/
/*
      // Set of data from tdc 0x0010 ref ch 13
      hldFileDir = "/data/TRBdata_May15/Direct_TDC_scan_tdc0010/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127161716.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127161936.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127162115.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127162249.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15127162425.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15127162624.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15127162624.hld"); break;  // same file as for ch 11, just for consistency
      case 15: source->AddInputFile(hldFileDir + "te15127162806.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15127163014.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15127163221.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15127163417.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15127163606.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15127163808.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15127163952.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15127164202.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15127164351.hld"); break;
      }
*/
/*
      // Set of data from tdc 0x0011 ref ch 31
      hldFileDir = "/data/TRBdata_May15/Direct_TDC_scan_tdc0011/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127164839.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127165356.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127165641.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127165849.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15127170057.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15127171016.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15127171209.hld"); break;
      case 15: source->AddInputFile(hldFileDir + "te15127171357.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15127171538.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15127171745.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15127171921.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15127172128.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15127172401.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15127172637.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15127172858.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15127172858.hld"); break;  // same file as for ch 29, just for consistency
      }
*/
/*
      // Set of data from tdc 0x0011 ref ch 13
      hldFileDir = "/data/TRBdata_May15/Direct_TDC_scan_tdc0011/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127173132.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127173419.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127173621.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127173816.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15127173957.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15127174132.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15127174132.hld"); break;  // same file as for ch 11, just for consistency
      case 15: source->AddInputFile(hldFileDir + "te15127174314.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15127174459.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15127174637.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15127174818.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15127175000.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15127175136.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15127175314.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15127175452.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15127175653.hld"); break;
      }
*/
/*
      // Set of data from PADIWA + tdc 0x0010 ref ch 31
      hldFileDir = "/data/TRBdata_May15/PADIWA_TDC_scan_ref_D6/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127141520.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127141401.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127141647.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127141240.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15127142946.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15127142714.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15127142830.hld"); break;
      case 15: source->AddInputFile(hldFileDir + "te15127142557.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15127143759.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15127143918.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15127144037.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15127144154.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15127145403.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15127145124.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15127145245.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15127145245.hld"); break;  // same file as for ch 29, just for consistency
      }
*/

      // Set of data from PADIWA + tdc 0x0011 ref ch 31
      hldFileDir = "/data/TRBdata_May15/PADIWA_TDC_scan_ref_A1/";
      switch (inChannel)
      {
      case 1: source->AddInputFile(hldFileDir + "te15127134650.hld"); break;
      case 3: source->AddInputFile(hldFileDir + "te15127134848.hld"); break;
      case 5: source->AddInputFile(hldFileDir + "te15127135031.hld"); break;
      case 7: source->AddInputFile(hldFileDir + "te15127135208.hld"); break;
      case 9: source->AddInputFile(hldFileDir + "te15126180928.hld"); break;
      case 11: source->AddInputFile(hldFileDir + "te15126181105.hld"); break;
      case 13: source->AddInputFile(hldFileDir + "te15126182959.hld"); break;
      case 15: source->AddInputFile(hldFileDir + "te15126183139.hld"); break;
      case 17: source->AddInputFile(hldFileDir + "te15126180114.hld"); break;
      case 19: source->AddInputFile(hldFileDir + "te15126175824.hld"); break;
      case 21: source->AddInputFile(hldFileDir + "te15126175700.hld"); break;
      case 23: source->AddInputFile(hldFileDir + "te15126175949.hld"); break;
      case 25: source->AddInputFile(hldFileDir + "te15126174820.hld"); break;
      case 27: source->AddInputFile(hldFileDir + "te15126174700.hld"); break;
      case 29: source->AddInputFile(hldFileDir + "te15126174246.hld"); break;
      case 31: source->AddInputFile(hldFileDir + "te15126174246.hld"); break;  // same file as for ch 29, just for consistency
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

