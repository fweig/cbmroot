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
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"

#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <TStopwatch.h>
#include <TSystem.h>
#endif

std::shared_ptr<CbmTrdUnpackMonitor> GetTrdMonitor(std::string treefilename, bool fasp = false);
std::shared_ptr<CbmTrdSpadic> GetTrdSpadic(bool useAvgBaseline = false);
std::shared_ptr<CbmStsUnpackMonitor> GetStsMonitor(std::string treefilename, bool bDebugMode = false);
std::shared_ptr<CbmMuchUnpackMonitor> GetMuchMonitor(std::string treefilename, bool bDebugMode = false);
std::shared_ptr<CbmRichUnpackMonitor> GetRichMonitor(std::string treefilename, bool bDebugMode = false);
std::shared_ptr<CbmTofUnpackMonitor> GetTofMonitor(std::string treefilename, bool bBmonMode = false);
std::string defaultSetupName = "mcbm_beam_2021_07_surveyed";

void run_unpack_tsa(std::vector<std::string> infile = {"test.tsa"}, UInt_t runid = 0,
                    std::string setupName = defaultSetupName, std::int32_t nevents = -1, bool bBmoninTof = false,
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
  if (bBmoninTof) {  //
    outfilename.replace(outfilename.find(".tsa"), 4, "_bmonintof.digi.root");
  }
  else {
    outfilename.replace(outfilename.find(".tsa"), 4, ".digi.root");
  }
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
  /// Do automatic mapping only if not overridden by user
  if (defaultSetupName == setupName) {
    cbm::mcbm::ToForceLibLoad dummy;  /// Needed to trigger loading of the library as no fct dict in ROOT6 and CLING
    try {
      setupName = cbm::mcbm::GetSetupFromRunId(runid);
    }
    catch (const std::invalid_argument& e) {
      std::cout << "Error in mapping from runID to setup name: " << e.what() << std::endl;
      return;
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

  if (!bBmoninTof) {
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
      // bmonconfig->SetMonitor(GetTofMonitor(outfilename, true));
    }
  }
  // -------------

  // ---- STS ----
  std::shared_ptr<CbmStsUnpackConfig> stsconfig = nullptr;

  stsconfig = std::make_shared<CbmStsUnpackConfig>(std::string(setupName), runid);
  if (stsconfig) {
    // stsconfig->SetDebugState();
    stsconfig->SetDoWriteOutput();
    stsconfig->SetDoWriteOptOutA("StsDigiPulser");
    std::string parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    if (2060 <= runid) {
      /// Starting to readout the U3 since 10/03/2022 Carbon run
      parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
    }
    stsconfig->SetParFilesBasePath(parfilesbasepathSts);
    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    stsconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //stsconfig->SetMonitor(GetStsMonitor(outfilename, true));
    stsconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= runid) {
      stsconfig->SetSystemTimeOffset(-1075);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      stsconfig->SetSystemTimeOffset(-970);  // [ns] value to be updated
    }

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
      // std::cout << Form("Setting time-walk parameters for: module %x, ASIC %u\n", sensor, asic);
      parAsic->SetWalkCoef({p0, p1, p2, p3});

      if (walkMap.find(sensor) == walkMap.end()) { walkMap[sensor] = CbmStsParModule(*parMod); }
      walkMap[sensor].SetAsic(asic, *parAsic);
      // std::cout << Form("Done with time-walk parameters for: module %x, ASIC %u\n", sensor, asic);
    }

    stsconfig->SetWalkMap(walkMap);
    walkMap.clear();
    delete parMod;
    delete parAsic;
  }
  // -------------

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
    else if (2163 <= runid && runid <= 2291) {
      ///
      muchconfig->SetParFileName("mMuchParUpto03042022.par");
    }
    else if (2311 <= runid && runid <= 2315) {
      ///
      muchconfig->SetParFileName("mMuchParUpto10042022.par");
    }
    else if (2316 <= runid && runid <= 2366) {
      ///
      muchconfig->SetParFileName("mMuchParUpto23052022.par");
    }
    else if (2367 <= runid && runid <= 2397) {
      /// Starting to use GEM 2 moved to CRI 0 on 24/05/2022
      muchconfig->SetParFileName("mMuchParUpto26052022.par");
    }
    else {
      /// Default file for all other runs (including 06/2022 Gold runs)
      muchconfig->SetParFileName("mMuchPar.par");
    }


    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    muchconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //muchconfig->SetMonitor(GetMuchMonitor(outfilename, true));
    muchconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= runid) {
      muchconfig->SetSystemTimeOffset(-1020);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      muchconfig->SetSystemTimeOffset(-980);  // [ns] value to be updated
    }

    // muchconfig->SetMinAdcCut(1, 1);

    // muchconfig->MaskNoisyChannel(3, 56);
  }
  // -------------

  // ---- TRD ----
  std::shared_ptr<CbmTrdUnpackConfig> trd1Dconfig = nullptr;

  TString trdsetuptag = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kTrd, trdsetuptag);
  // trd1Dconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), runid);
  trd1Dconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data());
  if (trd1Dconfig) {
    trd1Dconfig->SetDoWriteOutput();
    // Activate the line below to write Trd1D digis to a separate "TrdSpadicDigi" branch. Can be used to separate between Fasp and Spadic digis
    // trd1Dconfig->SetOutputBranchName("TrdSpadicDigi");
    // trd1Dconfig->SetDoWriteOptOutA(CbmTrdRawMessageSpadic::GetBranchName());
    // trd1Dconfig->SetDoWriteOptOutB("SpadicInfoMessages"); // SpadicInfoMessages

    std::string parfilesbasepathTrd = Form("%s/parameters/trd", srcDir.Data());
    trd1Dconfig->SetParFilesBasePath(parfilesbasepathTrd);
    // trd1Dconfig->SetMonitor(GetTrdMonitor(outfilename));
    // Get the spadic configuration true = avg baseline active / false plain sample 0
    trd1Dconfig->SetSpadicObject(GetTrdSpadic(true));
    trd1Dconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    if (2160 <= runid) {
      trd1Dconfig->SetSystemTimeOffset(1140);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      trd1Dconfig->SetSystemTimeOffset(1300);  // [ns] value to be updated
    }
  }
  // -------------

  // ---- TRDFASP2D ----
  std::shared_ptr<CbmTrdUnpackFaspConfig> trdfasp2dconfig = nullptr;

  trdfasp2dconfig = std::make_shared<CbmTrdUnpackFaspConfig>(trdsetuptag.Data());
  if (trdfasp2dconfig) {
    // trdfasp2dconfig->SetDebugState();
    trdfasp2dconfig->SetDoWriteOutput();
    // Activate the line below to write Trd1D digis to a separate "TrdFaspDigi" branch. Can be used to separate between Fasp and Spadic digis
    // trdfasp2dconfig->SetOutputBranchName("TrdFaspDigi");
    uint8_t map[NFASPMOD];
    uint16_t crob_map[NCROBMOD];
    for (uint32_t i(0); i < NFASPMOD; i++)
      map[i] = i;
    if (runid <= 1588) {
      const size_t nfasps = 12;
      uint8_t map21[] = {9, 2, 3, 11, 10, 7, 8, 0, 1, 4, 6, 5};
      for (uint32_t i(0); i < nfasps; i++)
        map[i] = map21[i];
      uint16_t crob_map21[] = {0x00f0, 0, 0, 0, 0};
      for (uint32_t i(0); i < NCROBMOD; i++)
        crob_map[i] = crob_map21[i];
    }
    else if (runid >= 2335) {
      const size_t nfasp0 = 72;
      const size_t nfasps = 36;
      uint8_t map22[]     = {
        84,  85,  86,  87,  88,  89,   // FEB14/0xffc1
        90,  91,  92,  93,  94,  95,   // FEB17/0xffc1
        96,  97,  98,  99,  100, 101,  // FEB18/0xffc1
        102, 103, 104, 105, 106, 107,  // FEB16/0xffc1
        72,  73,  74,  75,  76,  77,   // FEB9/0xffc1
        78,  79,  80,  81,  82,  83    // FEB8/0xffc1
      };
      for (uint32_t i(0); i < nfasps; i++)
        map[i + nfasp0] = map22[i];
      uint16_t crob_map22[] = {0xffc2, 0xffc5, 0xffc1, 0, 0};
      for (uint32_t i(0); i < NCROBMOD; i++)
        crob_map[i] = crob_map22[i];
    }
    trdfasp2dconfig->SetFaspMapping(5, map);
    trdfasp2dconfig->SetCrobMapping(5, crob_map);
    std::string parfilesbasepathTrdfasp2d = Form("%s/parameters/trd", srcDir.Data());
    trdfasp2dconfig->SetParFilesBasePath(parfilesbasepathTrdfasp2d);
    trdfasp2dconfig->SetSystemTimeOffset(-1800);  // [ns] value to be updated
    if (2160 <= runid) {
      trdfasp2dconfig->SetSystemTimeOffset(-570);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      trdfasp2dconfig->SetSystemTimeOffset(-510);  // [ns] value to be updated
    }
    // trdfasp2dconfig->SetMonitor(dynamic_pointer_cast<CbmTrdUnpackFaspMonitor>(GetTrdMonitor(outfilename, 1)));
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
    std::string parFileNameTof      = "mTofCriPar.par";
    if (2060 <= runid) {
      /// Additional modules added just before the 10/03/2022 Carbon run
      parfilesbasepathTof = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
      /// Setup changed multiple times between the 2022 carbon and uranium runs
      if (runid <= 2065) {
        /// Carbon runs: 2060 - 2065
        parFileNameTof = "mTofCriParCarbon.par";
      }
      else if (2150 <= runid && runid <= 2160) {
        /// Iron runs: 2150 - 2160
        parFileNameTof = "mTofCriParIron.par";
        if (bBmoninTof) {
          /// Map the BMon components in the TOF par file
          parFileNameTof = "mTofCriParIron_withBmon.par";
        }
      }
      else if (2176 <= runid && runid <= 2310) {
        /// Uranium runs: 2176 - 2310
        parFileNameTof = "mTofCriParUranium.par";
      }
      else if (2335 <= runid && runid <= 2497) {
        /// Nickel runs: 2335 - 2397
        /// Gold runs: 2400 - 2497
        parFileNameTof = "mTofCriParNickel.par";
        if (bBmoninTof) {
          /// Map the BMon components in the TOF par file
          parFileNameTof = "mTofCriParNickel_withBmon.par";
        }
      }
      else {
        parFileNameTof = "mTofCriPar.par";
      }
    }
    tofconfig->SetParFilesBasePath(parfilesbasepathTof);
    tofconfig->SetParFileName(parFileNameTof);
    tofconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
    if (2160 <= runid) {
      tofconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      tofconfig->SetSystemTimeOffset(40);  // [ns] value to be updated
    }
    if (runid <= 1659) {
      /// Switch ON the -4 offset in epoch count (hack for Spring-Summer 2021)
      tofconfig->SetFlagEpochCountHack2021();
    }
    /// Enable Monitor plots
    // tofconfig->SetMonitor(GetTofMonitor(outfilename, false));
  }
  // -------------

  // ---- RICH ----
  std::shared_ptr<CbmRichUnpackConfig> richconfig = nullptr;

  richconfig = std::make_shared<CbmRichUnpackConfig>("", runid);
  if (richconfig) {
    if (1904 < runid) {
      /// Switch to new unpacking algo starting from first combined cosmics run in 2022
      richconfig->SetUnpackerVersion(CbmRichUnpackerVersion::v03);
      richconfig->SetMonitor(GetRichMonitor(outfilename, true));
    }

    richconfig->DoTotOffsetCorrection();  // correct ToT offset
    richconfig->SetDebugState();
    richconfig->SetDoWriteOutput();
    std::string parfilesbasepathRich = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    richconfig->SetParFilesBasePath(parfilesbasepathRich);
    richconfig->SetSystemTimeOffset(256000 - 1200);  // [ns] 1 MS and additional correction
    if (1904 < runid) richconfig->SetSystemTimeOffset(-1200);
    if (2160 <= runid) {
      richconfig->SetSystemTimeOffset(50);  // [ns] value to be updated
    }
    if (2350 <= runid) {
      richconfig->SetSystemTimeOffset(100);  // [ns] value to be updated
    }
    if (runid == 1588) richconfig->MaskDiRICH(0x7150);
  }
  // -------------

  // ---- PSD ----
  std::shared_ptr<CbmPsdUnpackConfig> psdconfig = nullptr;

  psdconfig = std::make_shared<CbmPsdUnpackConfig>("", runid);
  if (psdconfig) {
    // psdconfig->SetDebugState();
    psdconfig->SetDoWriteOutput();
    // psdconfig->SetDoWriteOptOutA("CbmPsdDsp");
    std::string parfilesbasepathPsd = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    psdconfig->SetParFilesBasePath(parfilesbasepathPsd);
    psdconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
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
  if (stsconfig) unpack->SetUnpackConfig(stsconfig);
  if (muchconfig) unpack->SetUnpackConfig(muchconfig);
  if (trd1Dconfig) unpack->SetUnpackConfig(trd1Dconfig);
  if (trdfasp2dconfig) unpack->SetUnpackConfig(trdfasp2dconfig);
  if (tofconfig) unpack->SetUnpackConfig(tofconfig);
  if (richconfig) unpack->SetUnpackConfig(richconfig);
  if (psdconfig) unpack->SetUnpackConfig(psdconfig);
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
  stsconfig.reset();
  muchconfig.reset();
  trd1Dconfig.reset();
  trdfasp2dconfig.reset();
  tofconfig.reset();
  richconfig.reset();
  psdconfig.reset();
  // ------------------------------------------------------------------------

}  // End of main macro function


/**
 * @brief Get the Trd Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmTrdUnpackMonitor>
*/
std::shared_ptr<CbmTrdUnpackMonitor> GetTrdMonitor(std::string treefilename, bool fasp = false)
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
  std::string mydir = "/qa";
  outpath += mydir;

  auto currentdir = gSystem->GetWorkingDirectory();

  if (!gSystem->cd(outpath.data())) gSystem->MakeDirectory(outpath.data());
  else
    gSystem->cd(currentdir.data());

  std::string outfilename = outpath + filename;
  auto filetypepos        = outfilename.find(".digi.root");
  if (filetypepos != outfilename.npos) outfilename.replace(filetypepos, 10, ".mon.trd.root");
  else
    outfilename += ".mon.trd.root";
  // ------------------------------------------------------------------------

  std::vector<CbmTrdUnpackMonitor::eDigiHistos> digihistovec = {
    CbmTrdUnpackMonitor::eDigiHistos::kMap,        CbmTrdUnpackMonitor::eDigiHistos::kMap_St,
    CbmTrdUnpackMonitor::eDigiHistos::kMap_Nt,     CbmTrdUnpackMonitor::eDigiHistos::kCharge,
    CbmTrdUnpackMonitor::eDigiHistos::kCharge_St,  CbmTrdUnpackMonitor::eDigiHistos::kCharge_Nt,
    CbmTrdUnpackMonitor::eDigiHistos::kChannel,    CbmTrdUnpackMonitor::eDigiHistos::kChannel_St,
    CbmTrdUnpackMonitor::eDigiHistos::kChannel_Nt, CbmTrdUnpackMonitor::eDigiHistos::kTriggerType,
    CbmTrdUnpackMonitor::eDigiHistos::kDigiDeltaT};

  std::vector<CbmTrdUnpackMonitor::eRawHistos> rawhistovec = {
    CbmTrdUnpackMonitor::eRawHistos::kSignalshape,      CbmTrdUnpackMonitor::eRawHistos::kSignalshape_St,
    CbmTrdUnpackMonitor::eRawHistos::kSignalshape_Nt,   CbmTrdUnpackMonitor::eRawHistos::kElinkId,
    CbmTrdUnpackMonitor::eRawHistos::kSampleDistStdDev, CbmTrdUnpackMonitor::eRawHistos::kSample0perChannel,
    CbmTrdUnpackMonitor::eRawHistos::kHitType};

  std::vector<CbmTrdUnpackMonitor::eOtherHistos> otherhistovec = {CbmTrdUnpackMonitor::eOtherHistos::kSpadic_Info_Types,
                                                                  CbmTrdUnpackMonitor::eOtherHistos::kMs_Flags};
  std::shared_ptr<CbmTrdUnpackMonitor> monitor(nullptr);
  if (!fasp) {  // SPADIC monitor
    monitor = std::make_shared<CbmTrdUnpackMonitor>();
    monitor->SetActiveHistos(digihistovec);
    monitor->SetActiveHistos(rawhistovec);
    monitor->SetActiveHistos(otherhistovec);
    monitor->SetWriteToFile(outfilename.data());
  }
  else {  // FASP monitoring settings
    monitor = std::make_shared<CbmTrdUnpackFaspMonitor>();
    monitor->SetActiveHistos(digihistovec);
    monitor->SetWriteToFile(outfilename.data());
  }
  return monitor;
}


/**
 * @brief Get the Trd Spadic
 * @return std::shared_ptr<CbmTrdSpadic>
*/
std::shared_ptr<CbmTrdSpadic> GetTrdSpadic(bool useAvgBaseline)
{
  auto spadic = std::make_shared<CbmTrdSpadic>();
  spadic->SetUseBaselineAverage(useAvgBaseline);
  spadic->SetMaxAdcToEnergyCal(1.0);

  return spadic;
}

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

/**
 * @brief Get the Rich Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmRichUnpackMonitor>
*/
std::shared_ptr<CbmRichUnpackMonitor> GetRichMonitor(std::string treefilename, bool bDebugMode = false)
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
  if (filetypepos != outfilename.npos) outfilename.replace(filetypepos, 10, ".mon.rich.root");
  else
    outfilename += ".mon.rich.root";
  // ------------------------------------------------------------------------

  auto monitor = std::make_shared<CbmRichUnpackMonitor>();
  monitor->SetHistoFileName(outfilename);
  monitor->SetDebugMode(bDebugMode);

  return monitor;
}

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

void run_unpack_tsa(std::string infile = "test.tsa", UInt_t runid = 0, std::string setupName = defaultSetupName,
                    std::int32_t nevents = -1, bool bBmoninTof = false, std::string outpath = "data/")
{
  std::vector<std::string> vInFile = {infile};
  return run_unpack_tsa(vInFile, runid, setupName, nevents, bBmoninTof, outpath);
}
