/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

/** @file run_unpack_tsa.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since May 2021
 **/


// --- Includes needed for IDE
#include <FairSource.h>

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
const char* defaultSetupName = "mcbm_beam_2022_03_09_carbon";

void run_unpack_online_bmon(std::vector<std::string> publisher = {"tcp://localhost:5556"}, Int_t serverHttpPort = 8080,
                            Int_t serverRefreshRate = 100, std::int32_t nevents = -1, UInt_t runid = 1905,
                            const char* setupName = defaultSetupName, std::string outpath = "data/")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_unpack_online";            // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   Output filename   ----------------------------------------------
  std::string filename    = Form("online_%05d.digi.root", runid);
  std::string outfilename = outpath + filename;
  std::cout << "-I- " << myName << ": Output file will be " << outfilename << std::endl;
  // ------------------------------------------------------------------------


  // -----   Performance profiling   ----------------------------------------
  // Set to true if you want some minimal performance profiling output
  bool doPerfProfiling = true;
  // Define if you want a special path and name for the performance profiling output file
  std::string perfProfFileName = outpath + filename;
  perfProfFileName.replace(perfProfFileName.find(".digi.root"), 10, ".perf.root");
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
  }
  auto cbmsetup = CbmSetup::Instance();
  cbmsetup->LoadSetup(setupName);
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
    /// Enable Monitor plots
    bmonconfig->SetMonitor(GetTofMonitor(outfilename, true));
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
  auto source = new CbmSourceTsArchive(publisher);
  auto unpack = source->GetRecoUnpack();
  unpack->SetDoPerfProfiling(doPerfProfiling);
  unpack->SetOutputFilename(perfProfFileName);
  // Enable full time sorting instead sorting per FLIM link
  unpack->SetTimeSorting(true);
  unpack->SetMonitoringOnly(true);

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


  // -----   HttpServer for online monitoring   -----------------------------
  run->ActivateHttpServer(serverRefreshRate, serverHttpPort);
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  // run->SetAutoFinish(kFALSE);
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

void run_unpack_online_bmon(std::string publisher = "tcp://localhost:5556", Int_t serverHttpPort = 8080,
                            Int_t serverRefreshRate = 100, std::int32_t nevents = -1, UInt_t runid = 1905,
                            const char* setupName = defaultSetupName, std::string outpath = "data/")
{
  std::vector<std::string> vPublisher = {publisher};
  return run_unpack_online_bmon(vPublisher, serverHttpPort, serverRefreshRate, nevents, runid, setupName, outpath);
}
