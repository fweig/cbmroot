/** Macro for detector response simulation (digitisation)
 **
 ** The detector response produces a raw data file from the transport
 ** data, which serves as input for reconstruction. Raw data will
 ** be delivered in time-slice format (one tree entry per time-slice).
 **
 ** The first five arguments to the macro call represent the minimal 
 ** information to be passed to the digitization run. In addition, 
 ** the mode (time-based as default, or event-by-event) can be specified.
 ** By default, already existing output files will not be overwritten
 ** to prevent accidental data loss.
 **
 ** If the time-slice length is negative, all data will be in one
 ** time-slice.
 **
 ** For the file names, the following conventions are applied:
 ** Input file:     [dataSet].tra.root
 ** Parameter file: [dataSet].par.root
 ** Output file:    [dataSet].raw.root
 ** If different names are wanted, they have to be specified
 ** explicitely in the macro.
 **
 ** For further options to modify the run settings, consult
 ** the documentation of the CbmDigitization class.
 ** 
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 8 June 2018
 **/


void run_digi(Int_t nEvents            = 2,
              const char* setupName    = "sis100_electron",
              const char* index        = "00001",
              TString dataSet          = "data/test",
              Bool_t eventMode         = kTRUE,  // Event-by-event mode
              Double_t eventRate       = 1.e7,   // Interaction rate [1/s]
              Double_t timeSliceLength = 1.e4    // Length of time-slice [ns]
) {

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Allow overwriting of output file   -----------------------------
  Bool_t overwrite = kTRUE;
  // ------------------------------------------------------------------------


  // -----   File names   ---------------------------------------------------
  TString inFile  = dataSet + "_" + setupName + ".tra." + index + ".root";
  TString parFile = dataSet + "_" + setupName + ".par." + index + ".root";
  TString outFile = dataSet + "_" + setupName + ".raw." + index + ".root";
  TString monFile = dataSet + "_" + setupName + ".raw.moni." + index + ".root";
  if (eventMode) {
    outFile = dataSet + "_" + setupName + ".event.raw." + index + ".root";
    monFile = dataSet + "_" + setupName + ".event.raw.moni." + index + ".root";
  }

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Digitization run   ---------------------------------------------
  CbmDigitization run;

  run.AddInput(inFile, eventRate);
  run.SetOutputFile(outFile, overwrite);
  // run.SetMonitorFile(monFile);
  run.SetParameterRootFile(parFile);
  run.SetTimeSliceLength(timeSliceLength);
  run.SetEventMode(eventMode);
  run.GenerateRunInfo(kFALSE);

  // test with collection efficiency
  // CbmRichDigitizer* richDigitizer = new CbmRichDigitizer();
  // richDigitizer->SetCollectionEfficiency(0.0);
  // run.SetDigitizer(kRich, richDigitizer, "RichPoint", true);

  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl
            << std::endl;
  // ------------------------------------------------------------------------


  // -----   CTest resource monitoring   ------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  std::cout << "<DartMeasurement name=\"WallTime\" type=\"numeric/double\">";
  std::cout << rtime;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------


}  // End of macro
