/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland, Volker Friese, Pierre-Alain Loizeau, Pascal Raisig [committer], Dominik Smith, Adrian A. Weber  */

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

std::shared_ptr<CbmMuchUnpackMonitor> GetMuchMonitor(std::string treefilename, bool bDebugMode = false);
const char* defaultSetupName = "mcbm_beam_2021_07_surveyed";

void much_run_unpack_tsa(std::vector<std::string> infile = {"test.tsa"}, UInt_t runid = 0,
                         const char* setupName = defaultSetupName, std::int32_t nevents = -1,
                         std::string outpath = "data/withTof/")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel = "INFO";
  //TString logLevel     = "DEBUG";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_unpack_tsa";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
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
  auto cbmsetup = CbmSetup::Instance();
  cbmsetup->LoadSetup(setupName);
  // ------------------------------------------------------------------------

  // -----   UnpackerConfigs   ----------------------------------------------

  /*
  // ---- STS ----
  std::shared_ptr<CbmStsUnpackConfig> stsconfig = nullptr;

  stsconfig = std::make_shared<CbmStsUnpackConfig>(std::string(setupName), runid);
  if (stsconfig) {
    // stsconfig->SetDebugState();
    stsconfig->SetDoWriteOutput();
    stsconfig->SetDoWriteOptOutA("StsDigiPulser");
    std::string parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    stsconfig->SetParFilesBasePath(parfilesbasepathSts);
    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    stsconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //stsconfig->SetMonitor(GetStsMonitor(outfilename, true));
    stsconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated

    stsconfig->SetMinAdcCut(1, 1);
    stsconfig->SetMinAdcCut(2, 1);
    stsconfig->SetMinAdcCut(3, 1);
    stsconfig->SetMinAdcCut(4, 1);

    stsconfig->MaskNoisyChannel(3, 56);
    stsconfig->MaskNoisyChannel(3, 75);
    stsconfig->MaskNoisyChannel(3, 79);
    stsconfig->MaskNoisyChannel(3, 85);
    stsconfig->MaskNoisyChannel(7, 123);
    stsconfig->MaskNoisyChannel(7, 124);
    stsconfig->MaskNoisyChannel(7, 125);
    stsconfig->MaskNoisyChannel(7, 158);
    stsconfig->MaskNoisyChannel(7, 159);
    stsconfig->MaskNoisyChannel(7, 162);
    stsconfig->MaskNoisyChannel(7, 715);
    stsconfig->MaskNoisyChannel(9, 709);
    stsconfig->MaskNoisyChannel(12, 119);

    // Time Walk correction
    std::map<uint32_t, CbmStsParModule> walkMap;
    auto parAsic = new CbmStsParAsic(128, 31, 31., 1., 5., 800., 1000., 3.9789e-3);

    // Module params: number of channels, number of channels per ASIC
    auto parMod = new CbmStsParModule(2048, 128);

    // default
    double p0 = 0, p1 = 0, p2 = 0, p3 = 0;
    parAsic->SetWalkCoef({p0, p1, p2, p3});
    parMod->SetAllAsics(*parAsic);

    walkMap[0x10107C02] = CbmStsParModule(*parMod);  // Make a copy for storage
    walkMap[0x101FFC02] = CbmStsParModule(*parMod);  // Make a copy for storage

    /// To be replaced by a storage in a new parameter class later
    int sensor, asic;
    std::ifstream asicTimeWalk_par(Form("%s/mStsAsicTimeWalk.par", parfilesbasepathSts.data()));
    while (asicTimeWalk_par >> std::hex >> sensor >> std::dec >> asic >> p0 >> p1 >> p2 >> p3) {
      std::cout << Form("Setting time-walk parametersfor: module %x, ASIC %u\n", sensor, asic);
      parAsic->SetWalkCoef({p0, p1, p2, p3});

      if (walkMap.find(sensor) == walkMap.end()) { walkMap[sensor] = CbmStsParModule(*parMod); }
      walkMap[sensor].SetAsic(asic, *parAsic);
    }

    stsconfig->SetWalkMap(walkMap);
  }
  // -------------
   */
  // ---- MUCH ----
  std::shared_ptr<CbmMuchUnpackConfig> muchconfig = nullptr;

  muchconfig = std::make_shared<CbmMuchUnpackConfig>(std::string(setupName), runid);
  if (muchconfig) {
    // muchconfig->SetDebugState();
    muchconfig->SetDoWriteOutput();
    muchconfig->SetDoWriteOptOutA("MuchDigiPulser");
    std::string parfilesbasepathMuch = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
    muchconfig->SetParFilesBasePath(parfilesbasepathMuch);
    if (2060 <= runid && runid <= 2162) {
      /// Starting to use CRI Based MUCH setup with 2GEM and 1 RPC since 09/03/2022 Carbon run
      muchconfig->SetParFileName("mMuchParUpto26032022.par");
    }
    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    muchconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //muchconfig->SetMonitor(GetMuchMonitor(outfilename, true));
    muchconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated

    // muchconfig->SetMinAdcCut(1, 1);

    // muchconfig->MaskNoisyChannel(3, 56);
  }
  // -------------


  // ---- TOF ----
  std::shared_ptr<CbmTofUnpackConfig> tofconfig = nullptr;

  tofconfig = std::make_shared<CbmTofUnpackConfig>("", runid);
  if (tofconfig) {
    // tofconfig->SetDebugState();
    tofconfig->SetDoWriteOutput();
    // tofconfig->SetDoWriteOptOutA("CbmTofErrors");
    std::string parfilesbasepathTof = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    tofconfig->SetParFilesBasePath(parfilesbasepathTof);
    tofconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
    if (runid <= 1659) {
      /// Switch ON the -4 offset in epoch count (hack for Spring-Summer 2021)
      tofconfig->SetFlagEpochCountHack2021();
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

  //if (stsconfig) unpack->SetUnpackConfig(stsconfig);
  if (muchconfig) unpack->SetUnpackConfig(muchconfig);
  if (tofconfig) unpack->SetUnpackConfig(tofconfig);
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

}  // End of main macro function


/**
 * @brief Get the Sts Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmStsUnpackMonitor>
*/
std::shared_ptr<CbmStsUnpackMonitor> GetStsMonitor(std::string treefilename, bool bDebugMode = false)
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

  std::string outfilename = outpath + filename;
  auto filetypepos        = outfilename.find(".digi.root");
  if (filetypepos != outfilename.npos) outfilename.replace(filetypepos, 10, ".mon.sts.root");
  else
    outfilename += ".mon.sts.root";
  // ------------------------------------------------------------------------

  auto monitor = std::make_shared<CbmStsUnpackMonitor>();
  monitor->SetHistoFileName(outfilename);
  monitor->SetDebugMode(bDebugMode);
  return monitor;
}

/**
 * @brief Get the Much Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmMuchUnpackMonitor>
*/
std::shared_ptr<CbmMuchUnpackMonitor> GetMuchMonitor(std::string treefilename, bool bDebugMode = false)
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

  auto currentdir = gSystem->GetWorkingDirectory();

  if (!gSystem->cd(outpath.data())) gSystem->MakeDirectory(outpath.data());
  else
    gSystem->cd(currentdir.data());

  std::string outfilename = outpath + filename;
  auto filetypepos        = outfilename.find(".digi.root");
  if (filetypepos != outfilename.npos) outfilename.replace(filetypepos, 10, ".mon.much.root");
  else
    outfilename += ".mon.much.root";
  // ------------------------------------------------------------------------

  auto monitor = std::make_shared<CbmMuchUnpackMonitor>();
  monitor->SetHistoFileName(outfilename);
  monitor->SetDebugMode(bDebugMode);
  return monitor;
}


void much_run_unpack_tsa(std::string infile = "test.tsa", UInt_t runid = 0, const char* setupName = defaultSetupName,
                         std::int32_t nevents = -1, std::string outpath = "data/")
{
  std::vector<std::string> vInFile = {infile};
  return much_run_unpack_tsa(vInFile, runid, setupName, nevents, outpath);
}
