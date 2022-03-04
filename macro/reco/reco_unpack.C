/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
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
#endif

using std::string;

void reco_unpack(TString tsaFile = "", TString outFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "reco_unpack";                  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  //tsaFile = "/Users/vfriese/Cbmroot/data/1588_node8_1_0000";   Used for testing
  //outFile = "test";

  // ----- Default file names   ---------------------------------------------
  if (tsaFile.IsNull()) tsaFile = srcDir + "/input/mcbm_run399_first20Ts";
  TString inFile = tsaFile + ".tsa";
  if (outFile.IsNull()) outFile = tsaFile;
  outFile += ".digi.root";
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


  // -----   Unpacking task   -----------------------------------------------
  auto unpack = new CbmTaskUnpack();
  run->AddTask(unpack);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(-1, 0);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << "\nMacro finished successfully." << std::endl;
  std::cout << "Output file: " << outFile << std::endl;
  std::cout << "CPU time = " << timer.CpuTime() << " s, real time = " << timer.RealTime() << " s." << std::endl;
  // ------------------------------------------------------------------------

}  // End of main macro function
