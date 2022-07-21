/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

/** @file run_unpack_tsa.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since May 2021
 **/


// --- Includes needed for IDE
#include <RtypesCore.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#if !defined(__CLING__)
#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <TStopwatch.h>
#include <TSystem.h>
#endif

std::shared_ptr<CbmTofUnpackMonitor> GetTofMonitor(std::string treefilename, bool bBmonMode = false);
std::string defaultSetupName = "mcbm_beam_2022_03_09_carbon";

void run_unpack_tsa_bmon(std::vector<std::string> infile = {"test.tsa"}, UInt_t runid = 0,
                         std::string setupName = defaultSetupName, std::int32_t nevents = -1,
                         std::string outpath = "data/")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_unpack_tsa";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   Potentially hardcode the infile   ------------------------------

  // std::string infile = srcDir + "/input/mcbm_run399_first20Ts.tsa";

  // ------------------------------------------------------------------------


  // -----   Output filename   ----------------------------------------------
  std::string outfilename = infile[0];
  auto filenamepos        = infile[0].find_last_of("/");
  filenamepos++;
  std::string filename = infile[0].substr(filenamepos);
  if (filename.find("*") != infile[0].npos) filename = std::to_string(runid) + ".tsa";
  if (filename.find(";") != infile[0].npos) filename = std::to_string(runid) + "_merged" + ".tsa";
  if (outpath.empty()) { outpath = infile[0].substr(0, filenamepos); }
  outfilename = outpath + filename;
  outfilename.replace(outfilename.find(".tsa"), 4, ".digi.root");
  std::cout << "-I- " << myName << ": Output file will be " << outfilename << std::endl;
  // ------------------------------------------------------------------------


  // -----   Performance profiling   ----------------------------------------
  // Set to true if you want some minimal performance profiling output
  bool doPerfProfiling = true;
  // Define if you want a special path and name for the performance profiling output file
  std::string perfProfFileName = outpath + filename;
  perfProfFileName.replace(perfProfFileName.find(".tsa"), 4, ".perf.root");
  // ------------------------------------------------------------------------


  // -----   CbmSetup   -----------------------------------------------------
  if (2060 <= runid && defaultSetupName == setupName) {
    /// Setup changed multiple times between the 2022 carbon and uranium runs
    if (runid <= 2065) {
      /// Carbon runs: 2060 - 2065 = 10/03/2022
      setupName = "mcbm_beam_2022_03_09_carbon";
    }
    else if (2150 <= runid && runid <= 2160) {
      /// Iron runs: 2150 - 2160 = 24-25/03/2022
      setupName = "mcbm_beam_2022_03_22_iron";
    }
    else if (2176 <= runid && runid <= 2310) {
      /// Uranium runs: 2176 - 2310 = 30/03/2022 - 01/04/2022
      setupName = "mcbm_beam_2022_03_28_uranium";
    }
    else if (2350 <= runid && runid <= 2397) {
      /// Nickel runs: 2350 - 2397 = 23/05/2022 - 25/05/2022
      setupName = "mcbm_beam_2022_05_23_nickel";
    }
    else if (2454 <= runid && runid <= 2497) {
      /// Lambda Benchmark Gold runs: 2454 - 2497 = 16/06/2022 - 18/06/2022
      setupName = "mcbm_beam_2022_06_16_gold";
    }
    if (defaultSetupName != setupName) {
      std::cout << "Automatic setup choice for run " << runid << ": " << setupName << std::endl;
    }
  }
  auto cbmsetup = CbmSetup::Instance();
  cbmsetup->LoadSetup(setupName.c_str());
  // ------------------------------------------------------------------------

  // -----   UnpackerConfigs   ----------------------------------------------

  // ---- BMON ----
  std::shared_ptr<CbmBmonUnpackConfig> bmonconfig = nullptr;

  bmonconfig = std::make_shared<CbmBmonUnpackConfig>("", runid);
  if (bmonconfig) {
    // bmonconfig->SetDebugState();
    bmonconfig->SetDoWriteOutput();
    // bmonconfig->SetDoWriteOptOutA("CbmBmonErrors");
    std::string parfilesbasepathBmon = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
    bmonconfig->SetParFilesBasePath(parfilesbasepathBmon);
    bmonconfig->SetParFileName("mBmonCriPar.par");
    bmonconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
    if (2160 <= runid) {
      bmonconfig->SetSystemTimeOffset(-80);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      bmonconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
    /// Enable Monitor plots
    bmonconfig->SetMonitor(GetTofMonitor(outfilename, true));
    if (2337 <= runid) {
      bmonconfig->GetMonitor()->SetSpillThreshold(250);
      bmonconfig->GetMonitor()->SetSpillThresholdNonPulser(100);
    }
  }
  // -------------


  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   CbmSourceTsArchive   -------------------------------------------
  auto source = new CbmSourceTsArchive(infile);
  auto unpack = source->GetRecoUnpack();
  unpack->SetDoPerfProfiling(doPerfProfiling);
  unpack->SetOutputFilename(perfProfFileName);
  // Enable full time sorting instead sorting per FLIM link
  unpack->SetTimeSorting(true);

  if (bmonconfig) unpack->SetUnpackConfig(bmonconfig);
  // ------------------------------------------------------------------------

  // -----   FairRunAna   ---------------------------------------------------
  auto run  = new FairRunOnline(source);
  auto sink = new FairRootFileSink(outfilename.data());
  run->SetSink(sink);
  auto eventheader = new CbmTsEventHeader();
  run->SetRunId(runid);
  run->SetEventHeader(eventheader);
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
  if (nevents < 0) run->Run(-1, 0);
  else
    run->Run(0, nevents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "After CpuTime = " << timer.CpuTime() << " s RealTime = " << timer.RealTime() << " s." << std::endl;
  // ------------------------------------------------------------------------

  // --   Release all shared pointers to config before ROOT destroys things -
  // => We need to destroy things by hand because run->Finish calls (trhought the FairRootManager) Source->Close which
  //    does call the Source destructor, so due to share pointer things stay alive until out of macro scope...
  run->SetSource(nullptr);
  delete run;
  delete source;

  bmonconfig.reset();
  // ------------------------------------------------------------------------

}  // End of main macro function


/**
 * @brief Get the Tof Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmTofUnpackMonitor>
*/
std::shared_ptr<CbmTofUnpackMonitor> GetTofMonitor(std::string treefilename, bool bBmonMode = false)
{
  // -----   Output filename and path   -------------------------------------
  std::string outpath  = "";
  std::string filename = "";
  auto filenamepos     = treefilename.find_last_of("/");
  if (filenamepos != treefilename.npos) {
    outpath  = treefilename.substr(0, filenamepos);
    filename = treefilename.substr(filenamepos++);
  }
  if (outpath.empty()) outpath = gSystem->GetWorkingDirectory();
  //std::string mydir = "/qa";
  //outpath += mydir;

  auto currentdir = gSystem->GetWorkingDirectory();

  if (!gSystem->cd(outpath.data())) gSystem->MakeDirectory(outpath.data());
  else
    gSystem->cd(currentdir.data());

  std::string sSystemType = ".mon.tof.root";
  if (bBmonMode) {
    //
    sSystemType = ".mon.bmon.root";
  }

  std::string outfilename = outpath + filename;
  auto filetypepos        = outfilename.find(".digi.root");
  if (filetypepos != outfilename.npos) outfilename.replace(filetypepos, 10, sSystemType);
  else
    outfilename += sSystemType;
  // ------------------------------------------------------------------------

  auto monitor = std::make_shared<CbmTofUnpackMonitor>();
  monitor->SetHistoFileName(outfilename);
  monitor->SetBmonMode(bBmonMode);
  return monitor;
}

void run_unpack_tsa_bmon(std::string infile = "test.tsa", UInt_t runid = 0, std::string setupName = defaultSetupName,
                         std::int32_t nevents = -1, std::string outpath = "data/")
{
  std::vector<std::string> vInFile = {infile};
  return run_unpack_tsa_bmon(vInFile, runid, setupName, nevents, outpath);
}
