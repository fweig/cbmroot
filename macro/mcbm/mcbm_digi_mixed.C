/** @file mcbm_digi_mixed.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 7 August 2020
 **/



// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDigitization.h"
#include "FairSystemInfo.h"
#endif

/** @brief Macro for detector response simulation (digitisation)
 ** with two different MC inputs at different rates
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 8 August 2020
 **
 ** The detector response produces a raw data file from the transport
 ** data, which serves as input for reconstruction. Raw data will
 ** be delivered in time-slice format (one tree entry per time-slice).
 **
 ** For options to modify the run settings, consult
 ** the documentation of the CbmDigitization class.
 **/
void mcbm_digi_mixed()
{

  // ===> User settings

  // --- Specify collision input (e.g., UrQMD)
  TString collInput = "collision.tra.root";            // MC file name
  Double_t collRate = 1.e7;                            // Collision rate [1/s]
  ECbmTreeAccess collMode = ECbmTreeAccess::kRegular;  // Take events one after one

  // --- Specify beam input
  TString beamInput = "beam.tra.root";                 // MC file name
  Double_t beamRate = 1.e9;                            // Beam rate [1/s]
  ECbmTreeAccess beamMode = ECbmTreeAccess::kRandom;   // Take random events

  // --- Specify parameter file
  TString paramFile = "collision.par.root";

  // --- Specify time-slice length [ns]
  // --- Negative values mean that all data go into one time slice
  Double_t tsLength = -1.;

  // --- Specify output file
  TString outFile = "combined.raw.root";

  // <=== User settings



  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Allow overwriting of output file   -----------------------------
  Bool_t overwrite = kTRUE;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Digitization run   ---------------------------------------------
  CbmDigitization run;
  run.AddInput(0, collInput, collRate, collMode);
  run.AddInput(1, beamInput, beamRate, beamMode);
  run.SetParameterRootFile(paramFile);
  run.SetOutputFile(outFile, overwrite);
  run.SetTimeSliceLength(tsLength);
  run.Deactivate(ECbmModuleId::kTof);
  run.Run();
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << paramFile << std::endl;
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
