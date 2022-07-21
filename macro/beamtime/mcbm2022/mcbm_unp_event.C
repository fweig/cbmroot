/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland, Volker Friese, Pierre-Alain Loizeau, Pascal Raisig [committer], Dominik Smith, Adrian A. Weber  */


// --- Includes needed for IDE
#include <RtypesCore.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <math.h>
#include <stdio.h>
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
const char* defaultSetupName = "mcbm_beam_2021_07_surveyed";

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t mcbm_unp_event(bool bBmoninTof = false,
                      std::int32_t nTimeslices = -1,
                      std::string infile = "/scratch/mcbm_data/2022_iron/2160_node8_0000.tsa;/scratch/mcbm_data/2022_iron/2160_node9_0000.tsa",
                      UInt_t uRunId = 2160,
                      const char* setupName = defaultSetupName,
                      std::string sOutDir = "data/")
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  std::vector<std::string> vInFile = {infile};


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_eventmcbm_unp_event_reco";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");    // top source directory
  // ------------------------------------------------------------------------


  // -----   EventBuilder Settings----------------
  /// FIXME: Disable clang formatting to keep easy parameters overview
  /* clang-format off */
  const UInt_t eb_TriggerMinNumberBmon { 1 };
  const UInt_t eb_TriggerMinNumberSts  { 0 };
  const UInt_t eb_TriggerMinNumberMuch { 0 };
  const UInt_t eb_TriggerMinNumberTof  { 4 };
  const UInt_t eb_TriggerMinNumberRich { 0 };

  const Int_t  eb_TriggerMaxNumberBMon { -1 };
  const Int_t  eb_TriggerMaxNumberSts  { -1 };
  const Int_t  eb_TriggerMaxNumberMuch { -1 };
  const Int_t  eb_TriggerMaxNumberTof  { -1 };
  const Int_t  eb_TriggerMaxNumberRich { -1 };
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */
  // ------------------------------------------------------------------------


  // -----   Output filename   ----------------------------------------------
  std::string filename    = Form("%d_%s.digi.root", uRunId, (bBmoninTof ? "BmonInTof" : "BmonSepar"));
  std::string outfilename = sOutDir + "/" + filename;
  std::cout << "-I- " << myName << ": Output file will be " << outfilename << std::endl;
  std::string histosfilename = sOutDir + "/" + filename;
  histosfilename.replace(histosfilename.find(".digi.root"), 10, ".hist.root");
  std::cout << "-I- " << myName << ": Histos file will be " << histosfilename << std::endl;
  // ------------------------------------------------------------------------


  // -----   Performance profiling   ----------------------------------------
  // Set to true if you want some minimal performance profiling output
  bool doPerfProfiling = true;
  // Define if you want a special path and name for the performance profiling output file
  std::string perfProfFileName = sOutDir + "/" + filename;
  perfProfFileName.replace(perfProfFileName.find(".digi.root"), 10, ".perf.root");
  // ------------------------------------------------------------------------

  // -----   CbmSetup   -----------------------------------------------------
  if (2060 <= uRunId && defaultSetupName == setupName) {
    /// Setup changed multiple times between the 2022 carbon and uranium runs
    if (uRunId <= 2065) {
      /// Carbon runs: 2060 - 2065 = 10/03/2022
      setupName = "mcbm_beam_2022_03_09_carbon";
    }
    else if (2150 <= uRunId && uRunId <= 2160) {
      /// Iron runs: 2150 - 2160 = 24-25/03/2022
      setupName = "mcbm_beam_2022_03_22_iron";
    }
    else if (2176 <= uRunId && uRunId <= 2310) {
      /// Uranium runs: 2176 - 2310 = 30/03/2022 - 01/04/2022
      setupName = "mcbm_beam_2022_03_28_uranium";
    }
  }
  auto cbmGeoSetup = CbmSetup::Instance();
  cbmGeoSetup->LoadSetup(setupName);
  // ------------------------------------------------------------------------

  // -----   UnpackerConfigs   ----------------------------------------------

  // ---- BMON ----
  std::shared_ptr<CbmBmonUnpackConfig> bmonconfig = nullptr;

  if (!bBmoninTof) {
    bmonconfig = std::make_shared<CbmBmonUnpackConfig>("", uRunId);
    if (bmonconfig) {
      // bmonconfig->SetDebugState();
      bmonconfig->SetDoWriteOutput();
      // bmonconfig->SetDoWriteOptOutA("CbmBmonErrors");
      std::string parfilesbasepathBmon = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
      bmonconfig->SetParFilesBasePath(parfilesbasepathBmon);
      bmonconfig->SetParFileName("mBmonCriPar.par");
      bmonconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
      if (2160 <= uRunId) {
        bmonconfig->SetSystemTimeOffset(-80);  // [ns] value to be updated
      }
      /// Enable Monitor plots
      // bmonconfig->SetMonitor(GetTofMonitor(outfilename, true));
    }
  }
  // -------------

  // ---- STS ----
  std::shared_ptr<CbmStsUnpackConfig> stsconfig = nullptr;

  stsconfig = std::make_shared<CbmStsUnpackConfig>(std::string(setupName), uRunId);
  if (stsconfig) {
    // stsconfig->SetDebugState();
    stsconfig->SetDoWriteOutput();
    stsconfig->SetDoWriteOptOutA("StsDigiPulser");
    std::string parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    if (2060 <= uRunId) {
      /// Starting to readout the U3 since 10/03/2022 Carbon run
      parfilesbasepathSts = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
    }
    stsconfig->SetParFilesBasePath(parfilesbasepathSts);
    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    stsconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //stsconfig->SetMonitor(GetStsMonitor(outfilename, true));
    stsconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= uRunId) {
      stsconfig->SetSystemTimeOffset(-1075);  // [ns] value to be updated
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
      std::cout << Form("Setting time-walk parametersfor: module %x, ASIC %u\n", sensor, asic);
      parAsic->SetWalkCoef({p0, p1, p2, p3});

      if (walkMap.find(sensor) == walkMap.end()) { walkMap[sensor] = CbmStsParModule(*parMod); }
      walkMap[sensor].SetAsic(asic, *parAsic);
    }

    stsconfig->SetWalkMap(walkMap);
    walkMap.clear();
    delete parMod;
    delete parAsic;
  }
  // -------------

  // ---- MUCH ----
  std::shared_ptr<CbmMuchUnpackConfig> muchconfig = nullptr;

  muchconfig = std::make_shared<CbmMuchUnpackConfig>(std::string(setupName), uRunId);
  if (muchconfig) {
    // muchconfig->SetDebugState();
    muchconfig->SetDoWriteOutput();
    muchconfig->SetDoWriteOptOutA("MuchDigiPulser");
    std::string parfilesbasepathMuch = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
    muchconfig->SetParFilesBasePath(parfilesbasepathMuch);
    if (2060 <= uRunId && uRunId <= 2162) {
      /// Starting to use CRI Based MUCH setup with 2GEM and 1 RPC since 09/03/2022 Carbon run
      muchconfig->SetParFileName("mMuchParUpto26032022.par");
    }
    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    muchconfig->SetDuplicatesRejection(true, true);
    /// Enable Monitor plots
    //muchconfig->SetMonitor(GetMuchMonitor(outfilename, true));
    muchconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= uRunId) {
      muchconfig->SetSystemTimeOffset(-1020);  // [ns] value to be updated
    }

    // muchconfig->SetMinAdcCut(1, 1);

    // muchconfig->MaskNoisyChannel(3, 56);
  }
  // -------------

  // ---- TRD ----
  std::shared_ptr<CbmTrdUnpackConfig> trd1Dconfig = nullptr;

  TString trdsetuptag = "";
  cbmGeoSetup->GetGeoTag(ECbmModuleId::kTrd, trdsetuptag);
  // trd1Dconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), uRunId);
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
    if (2160 <= uRunId) {
      trd1Dconfig->SetSystemTimeOffset(1140);  // [ns] value to be updated
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
    if (uRunId <= 1588) {
      uint8_t map21[] = {9, 2, 3, 11, 10, 7, 8, 0, 1, 4, 6, 5};
      for (int i(0); i < NFASPMOD; i++)
        map[i] = (i < 12 ? map21[i] : i);
    }
    else
      for (int i(0); i < NFASPMOD; i++)
        map[i] = i;
    trdfasp2dconfig->SetFaspMapping(5, map);
    std::string parfilesbasepathTrdfasp2d = Form("%s/parameters/trd", srcDir.Data());
    trdfasp2dconfig->SetParFilesBasePath(parfilesbasepathTrdfasp2d);
    trdfasp2dconfig->SetSystemTimeOffset(-1800);  // [ns] value to be updated
    if (2160 <= uRunId) {
      trdfasp2dconfig->SetSystemTimeOffset(-570);  // [ns] value to be updated
    }
    // trdfasp2dconfig->SetMonitor(dynamic_pointer_cast<CbmTrdUnpackFaspMonitor>(GetTrdMonitor(outfilename, 1)));
  }
  // -------------

  // ---- TOF ----
  std::shared_ptr<CbmTofUnpackConfig> tofconfig = nullptr;

  tofconfig = std::make_shared<CbmTofUnpackConfig>("", uRunId);
  if (tofconfig) {
    // tofconfig->SetDebugState();
    tofconfig->SetDoWriteOutput();
    // tofconfig->SetDoWriteOptOutA("CbmTofErrors");
    std::string parfilesbasepathTof = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    std::string parFileNameTof      = "mTofCriPar.par";
    if (2060 <= uRunId) {
      /// Additional modules added just before the 10/03/2022 Carbon run
      parfilesbasepathTof = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
      /// Setup changed multiple times between the 2022 carbon and uranium runs
      if (uRunId <= 2065) {
        /// Carbon runs: 2060 - 2065
        parFileNameTof = "mTofCriParCarbon.par";
      }
      else if (2150 <= uRunId && uRunId <= 2160) {
        /// Iron runs: 2150 - 2160
        parFileNameTof = "mTofCriParIron.par";
        if (bBmoninTof) { parFileNameTof = "mTofCriParIron_withBmon.par"; }
      }
      else if (2176 <= uRunId && uRunId <= 2310) {
        /// Uranium runs: 2176 - 2310
        parFileNameTof = "mTofCriParUranium.par";
      }
    }
    tofconfig->SetParFilesBasePath(parfilesbasepathTof);
    tofconfig->SetParFileName(parFileNameTof);
    tofconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated
    if (2160 <= uRunId) {
      tofconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
    if (uRunId <= 1659) {
      /// Switch ON the -4 offset in epoch count (hack for Spring-Summer 2021)
      tofconfig->SetFlagEpochCountHack2021();
    }
    /// Enable Monitor plots
    // tofconfig->SetMonitor(GetTofMonitor(outfilename, false));
  }
  // -------------

  // ---- RICH ----
  std::shared_ptr<CbmRichUnpackConfig> richconfig = nullptr;

  richconfig = std::make_shared<CbmRichUnpackConfig>("", uRunId);
  if (richconfig) {
    if (1904 < uRunId) {
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
    if (1904 < uRunId) richconfig->SetSystemTimeOffset(-1200);
    if (2160 <= uRunId) {
      richconfig->SetSystemTimeOffset(50);  // [ns] value to be updated
    }
    if (uRunId == 1588) richconfig->MaskDiRICH(0x7150);
  }
  // -------------

  // ------------------------------------------------------------------------

  // --------------------event builder---------------------------------------
  CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

  //Choose between NoOverlap, MergeOverlap, AllowOverlap
  evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

  // Set TOF as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetTof);

  // Remove detectors not there in 2022
  evBuildRaw->RemoveDetector(kRawEventBuilderDetT0);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);

  // Add all 2022 detectors in the right order
  evBuildRaw->AddDetector(kRawEventBuilderDetT0);
  evBuildRaw->AddDetector(kRawEventBuilderDetSts);
  evBuildRaw->AddDetector(kRawEventBuilderDetMuch);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->AddDetector(kRawEventBuilderDetRich);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  //evBuildRaw->SetTsParameters(0.0, 2.56e8, 0.0);, 0.0);
  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=102.4ms in 2022, TsOverLength=TS overlap not used in mCBM202?
  evBuildRaw->SetTsParameters(0.0, 1.28e8, 1.12928e8);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, eb_TriggerMaxNumberTof);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0, eb_TriggerMinNumberBmon);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0, eb_TriggerMaxNumberBMon);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, eb_TriggerMinNumberSts);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, eb_TriggerMaxNumberSts);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kMuch, eb_TriggerMinNumberMuch);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kMuch, eb_TriggerMaxNumberMuch);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich, eb_TriggerMinNumberRich);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich, eb_TriggerMaxNumberRich);

  if (bBmoninTof) { evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0, -180, 20); }
  else {
    evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0, -100, 100);
  }
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -60, 60);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kMuch, -100, 500);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd, -100, 350);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd2d, -200, 200);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, -60, 60);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich, -60, 60);

  // Use standard MUCH digis
  evBuildRaw->ChangeMuchBeamtimeDigiFlag();

  // Set Det type to find T0 in TOF digis = Select storage of BMon digis
  if (bBmoninTof) { evBuildRaw->SetT0InTofDetType(); }

  evBuildRaw->SetOutFilename(histosfilename);
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   CbmSourceTsArchive   -------------------------------------------
  auto source = new CbmSourceTsArchive(vInFile);
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
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  auto run  = new FairRunOnline(source);
  auto sink = new FairRootFileSink(outfilename.data());
  run->SetSink(sink);
  auto eventheader = new CbmTsEventHeader();
  run->SetRunId(uRunId);
  run->SetEventHeader(eventheader);

  run->AddTask(evBuildRaw);
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
  if (nTimeslices < 0) run->Run(-1, 0);
  else
    run->Run(0, nTimeslices);
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

  return kTRUE;
}  // End of main macro function


/**
 * @brief Get the Trd Monitor. Extra function to keep default macro part more silent.
 * @return std::shared_ptr<CbmTrdUnpackMonitor>
*/
std::shared_ptr<CbmTrdUnpackMonitor> GetTrdMonitor(std::string treefilename, bool fasp = false)
{
  // -----   Output filename and path   -------------------------------------
  std::string sOutDir  = "";
  std::string filename = "";
  auto filenamepos     = treefilename.find_last_of("/");
  if (filenamepos != treefilename.npos) {
    sOutDir  = treefilename.substr(0, filenamepos);
    filename = treefilename.substr(filenamepos++);
  }
  if (sOutDir.empty()) sOutDir = gSystem->GetWorkingDirectory();
  std::string mydir = "/qa";
  sOutDir += mydir;

  auto currentdir = gSystem->GetWorkingDirectory();

  if (!gSystem->cd(sOutDir.data())) gSystem->MakeDirectory(sOutDir.data());
  else
    gSystem->cd(currentdir.data());

  std::string outfilename = sOutDir + filename;
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
