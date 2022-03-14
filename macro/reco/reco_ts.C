/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


// --- Includes needed for IDE code analyser
#if !defined(__CLING__)
#include "CbmSourceTs.h"
#include "CbmTaskUnpack.h"
#include "CbmTsEventHeader.h"

#include <FairRootFileSink.h>
#include <FairRunOnline.h>

#include <TStopwatch.h>
#include <TSystem.h>

#include <iostream>
#include <memory>
#endif

#include <FairLogger.h>


using std::string;

/** @brief Macro for CBM reconstruction from FLES timeslices
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  12 March 2022
 ** @param tsaFile    Name of input file (w/o extension .tsa)
 ** @param outFile    Name of output file (w/o extension .digi.root)
 **
 ** Reconstruction from timeslice level. Currently included stages:
 ** - Unpacking (STS only)
 ** - Event trigger based on STS digis (CbmTaskDigiTrigger)
 ** - Event building (CbmTaskBuildEvents) (STS only)
 **
 ** TODO: To be expanded with the progress of the algo project.
 **
 ** If the tsaFile name is not specified, a default file from the reporistory will be used.
 ** If the outFile name is not specified, the input file name will be used, replacing
 ** the extension .tsa by .digi.root
 **/

void reco_ts(TString tsaFile = "", TString outFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "reco_ts";                      // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  // Tested with file 1588_node8_1_0000.tsa
  // TODO: Would need a small up-to-date default input file; the one distributed with
  // the code is outdated.

  // ----- Default file names   ---------------------------------------------
  if (tsaFile.IsNull()) tsaFile = srcDir + "/input/mcbm_run399_first20Ts";
  TString inFile = tsaFile + ".tsa";
  if (outFile.IsNull()) outFile = tsaFile;
  outFile += ".digi.root";
  std::cout << std::endl << std::endl;
  LOG(info) << myName << ": Using input file " << inFile;
  LOG(info) << myName << ": Output file is   " << outFile;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----   Run configuration   ---------------------------------------------
  auto source = new CbmSourceTs(inFile.Data());
  auto run    = new FairRunOnline(source);
  auto sink   = new FairRootFileSink(outFile.Data());
  run->SetSink(sink);
  auto eventheader = new CbmTsEventHeader();
  run->SetEventHeader(new CbmTsEventHeader());
  // ------------------------------------------------------------------------


  // -----   Unpacking   ----------------------------------------------------
  auto unpack = std::make_unique<CbmTaskUnpack>();
  unpack->SetOutputBranchPersistent("DigiTimeslice.", kFALSE);
  LOG(info) << myName << ": Added task " << unpack->GetName();
  run->AddTask(unpack.release());
  // ------------------------------------------------------------------------


  // -----   Digi trigger   -------------------------------------------------
  auto trigger         = std::make_unique<CbmTaskTriggerDigi>();
  double triggerWindow = 10.;  // Trigger window size in ns
  int32_t minNumDigis  = 100;  // Trigger threshold in number of digis
  double deadTime      = 50.;  // Minimum time between two triggers
  trigger->SetAlgoParams(triggerWindow, minNumDigis, deadTime);
  trigger->AddSystem(ECbmModuleId::kSts);
  trigger->SetOutputBranchPersistent("Trigger", kFALSE);
  LOG(info) << myName << ": Added task " << trigger->GetName();
  run->AddTask(trigger.release());
  // ------------------------------------------------------------------------


  // -----   Event building   -----------------------------------------------
  auto evtBuild = std::make_unique<CbmTaskBuildEvents>();
  evtBuild->SetEventWindow(ECbmModuleId::kSts, -20., 30.);  // event building time window for STS
  LOG(info) << myName << ": Added task " << evtBuild->GetName();
  run->AddTask(evtBuild.release());
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  LOG(info) << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  LOG(info) << myName << ": Starting run" << std::endl;
  run->Run(-1, 0);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file: " << outFile << std::endl;
  std::cout << "CPU time = " << timer.CpuTime() << " s, real time = " << timer.RealTime() << " s." << std::endl;
  // ------------------------------------------------------------------------

}  // End of main macro function
