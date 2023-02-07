/* Copyright (C) 2020-2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Oleg Golosov [committer] */

/** @file run_digi_json_config.C
 ** @author Oleg Golosov <oleg.golosov@gmail.com>
 ** @since 31 March 2020
 **/

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDigitization.h"

#include "FairSystemInfo.h"

#include "TStopwatch.h"
#endif


/** @brief Macro for detector response simulation in CBM
 ** @author Oleg Golosov <oleg.golosov@gmail.com>
 ** @since  31 March 2021
 ** @param config      Name of config file
 ** @param nEvents     Number of events to process
 **
 ** This macro performs the detector response simulation
 ** for multiple transport files using test-based config.
 ** Additions to the CbmDigitization settings can be made after
 ** the main config is applied. 
 **/

void run_digi_json_config(std::string config = "", int nEvents = 2)
{
  if (config == "") config = Form("%s/macro/run/config.json", gSystem->Getenv("VMCWORKDIR"));

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  CbmDigitization run;
  if (CbmDigitizationConfig::Load(run, config)) run.Run(nEvents);
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
