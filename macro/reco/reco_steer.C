/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


// --- Includes needed for IDE code analyser
#if !defined(__CLING__)
#include "CbmReco.h"

#include <TStopwatch.h>
#include <TSystem.h>

#include <iostream>
#endif

#include <FairLogger.h>


using std::string;

/** @brief Macro for CBM reconstruction from FLES timeslices
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  12 March 2022
 ** @param tsaFile    Name of input file (w/o extension .tsa)
 ** @param outFile    Name of output file (w/o extension .digi.root)
 ** @param numTs      Number of timeslices to process. If not specified, all available will be used.
 **
 ** Reconstruction from timeslice level, making use of the steering class CbmReco.
 ** Currently included stages:
 ** - Unpacking (STS only)
 ** - Event trigger based on STS digis (CbmTaskDigiTrigger)
 ** - Event building (CbmTaskBuildEvents) (STS only)
 **
 ** If the tsaFile name is not specified, a default file from the repository will be used.
 ** If the outFile name is not specified, the input file name will be used, replacing
 ** the extension .tsa by .digi.root
 **/

void reco_steer(TString tsaFile = "", TString outFile = "", int32_t numTs = std::numeric_limits<int32_t>::max())
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // --- Configuration  -----------------------------------------------------
  // Digi trigger
  CbmRecoConfig config;
  config.fTriggerDet       = ECbmModuleId::kSts;  // trigger detector
  config.fTriggerWin       = 10.;                 // trigger window in [ns]
  config.fTriggerThreshold = 100;                 // trigger threshold in number of digis
  config.fTriggerDeadTime  = 50.;                 // trigger dead time [ns]

  // Event builder
  config.fEvtbuildWindows[ECbmModuleId::kSts] = std::make_pair(-20., 30.);  // Event window for STS

  // Branch persistence
  config.fStoreTimeslice = false;  /// Store branch DigiTimeSlice
  config.fStoreTrigger   = false;  /// Store branch Trigger
  config.fStoreEvents    = true;   /// Store branch DigiEvent
  // ------------------------------------------------------------------------


  // ------------------------------------------------------------------------
  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Environment   --------------------------------------------------
  TString myName = "reco_steer";                   // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // Tested with file 1588_node8_1_0000.tsa
  // TODO: Would need a small up-to-date default input file; the one distributed with
  // the code is outdated.


  // ----- Default file names   ---------------------------------------------
  if (tsaFile.IsNull()) tsaFile = srcDir + "/input/mcbm_run399_first20Ts";
  TString inFile = tsaFile + ".tsa";
  if (outFile.IsNull()) outFile = tsaFile;
  outFile += ".digi.root";
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Run reconstruction   -------------------------------------------
  TStopwatch timer;
  timer.Start();
  CbmReco run(inFile.Data(), outFile.Data(), numTs, config);
  run.Run();
  timer.Stop();
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << myName << ": Macro finished successfully." << std::endl;
  std::cout << myName << ": CPU time = " << timer.CpuTime() << " s, real time = " << timer.RealTime() << " s."
            << std::endl;
  // ------------------------------------------------------------------------

}  // End of main macro function
