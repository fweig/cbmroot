/* Copyright (C) 2020-2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Oleg Golosov [committer] */

/** @file run_transport_json_config.C
 ** @author Oleg Golosov <oleg.golosov@gmail.com>
 ** @since 31 March 2020
 **/

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TStopwatch.h"
#endif


/** @brief Macro for transport simulation of events from file
 ** @author Oleg Golosov <oleg.golosov@gmail.com>
 ** @since  31 March 2021
 ** @param config      Name of config file
 ** @param nEvents     Number of events to transport
 **
 ** This macro performs the transport simulation of externally generated
 ** events from an input file using test-based config.
 ** Additions to the CbmTransport settings can be made after
 ** the main config is applied. 
 **/

void run_transport_json_config(std::string config = "", int nEvents = 2, int randomSeed = 0)
{
  if (config == "") config = Form("%s/macro/run/config.json", gSystem->Getenv("VMCWORKDIR"));

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  CbmTransport run;
  run.SetRandomSeed(randomSeed);
  if (CbmTransportConfig::Load(run, config)) run.Run(nEvents);
  else
    return;

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl;
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------
}
