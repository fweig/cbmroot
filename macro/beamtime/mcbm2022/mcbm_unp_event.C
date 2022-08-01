/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Adrian Weber */


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

std::shared_ptr<CbmTrdSpadic> GetTrdSpadic(bool useAvgBaseline = false);
std::string defaultSetupName = "mcbm_beam_2021_07_surveyed";

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t mcbm_unp_event(std::string infile,
                      UInt_t uRunId,
                      uint32_t uTriggerSet = 3,
                      std::int32_t nTimeslices = -1,
                      std::string setupName = defaultSetupName,
                      std::string sOutDir = "data/",
                      bool bBmoninTof = false)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  std::vector<std::string> vInFile = {infile};


  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_unp_event";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   EventBuilder Settings----------------
  /// FIXME: Disable clang formatting to keep easy parameters overview
  /* clang-format off */
  UInt_t eb_TriggerMinNumberBmon  { 0 };
  UInt_t eb_TriggerMinNumberSts   { 0 };
  UInt_t eb_TriggerMinNumberMuch  { 0 };
  UInt_t eb_TriggerMinNumberTrd1d { 0 };
  UInt_t eb_TriggerMinNumberTrd2d { 0 };
  UInt_t eb_TriggerMinNumberTof   { 4 };
  UInt_t eb_TriggerMinNumberRich  { 0 };

  Int_t  eb_TriggerMaxNumberBMon  { -1 };
  Int_t  eb_TriggerMaxNumberSts   { -1 };
  Int_t  eb_TriggerMaxNumberMuch  { -1 };
  Int_t  eb_TriggerMaxNumberTrd1d { -1 };
  Int_t  eb_TriggerMaxNumberTrd2d { -1 };
  Int_t  eb_TriggerMaxNumberTof   { -1 };
  Int_t  eb_TriggerMaxNumberRich  { -1 };

  UInt_t eb_TriggerMinLayersNumberTof  { 0 };
  UInt_t eb_TriggerMinLayersNumberSts  { 0 };

  Double_t eb_TrigWinMinBMon  { -20};
  Double_t eb_TrigWinMaxBMon  {  20};
  Double_t eb_TrigWinMinSts   { -40};
  Double_t eb_TrigWinMaxSts   {  40};
  Double_t eb_TrigWinMinMuch  { -50};
  Double_t eb_TrigWinMaxMuch  { 500};
  Double_t eb_TrigWinMinTrd1d { -50};
  Double_t eb_TrigWinMaxTrd1d { 400};
  Double_t eb_TrigWinMinTrd2d { -60};
  Double_t eb_TrigWinMaxTrd2d { 350};
  Double_t eb_TrigWinMinTof   { -10};
  Double_t eb_TrigWinMaxTof   {  70};
  Double_t eb_TrigWinMinRich  { -10};
  Double_t eb_TrigWinMaxRich  {  40};


  bool bTrigSet = true;
  switch (uTriggerSet) {
    case 0: {
      // default, any Tof hit
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       =    1;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -80;
      eb_TrigWinMaxTof             =  120;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 1: {
      // default,  Tof - T0 concidences (pulser)
      eb_TriggerMinNumberBmon      =    1;
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       =    2;
      eb_TriggerMinLayersNumberTof =    1;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             = -180;
      eb_TrigWinMaxTof             =  220;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 2: {
      // Tof standalone track trigger (cosmic)
      eb_TriggerMaxNumberBMon      = 1000;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -30;
      eb_TrigWinMaxTof             =   70;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 3: {
      // # Tof track trigger with T0
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 4: {
      // mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinLayersNumberSts = 1;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 5: {
      // mCbm lambda trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 2;

      eb_TriggerMinNumberSts       = 8;
      eb_TriggerMinLayersNumberSts = 2;

      eb_TriggerMinNumberTof       = 16;
      eb_TriggerMinLayersNumberTof = 8; // # PAL 07/04/2022: Not sure here if it should be 4 or 8 (2 tracks in same det. stack excluded?)

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 6: {
      // One hit per detector system w/ big acceptance = mCbm full track trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMaxNumberBMon      = 1;

      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinLayersNumberSts = 0;

      eb_TriggerMinNumberMuch      = 2;

      eb_TriggerMinNumberTrd1d     = 2;

      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;

      eb_TrigWinMinBMon            =  -50;
      eb_TrigWinMaxBMon            =   50;
      eb_TrigWinMinSts             =  -60;
      eb_TrigWinMaxSts             =   60;
      eb_TrigWinMinTrd1d           = -300;
      eb_TrigWinMaxTrd1d           =  300;
      eb_TrigWinMinTrd2d           = -200;
      eb_TrigWinMaxTrd2d           =  200;

      eb_TrigWinMinTof             =  -20;
      eb_TrigWinMaxTof             =   60;

      eb_TrigWinMinRich            =  -60;
      eb_TrigWinMaxRich            =   60;
      break;
    }
    case 7: {
      /// PAL default: T0 + STS + TOF, only digi cut
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinNumberTof       = 4;
      break;
    }
    case 8: {
      // default,  Tof - T0 concidences (pulser)
      eb_TriggerMinNumberBmon      = 4;
      eb_TriggerMinNumberTof       = 2;
      eb_TriggerMinLayersNumberTof = 1;
      break;
    }
    case 9: {
      // Tof standalone track trigger (cosmic)
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 10: {
      // # Tof track trigger with T0
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 11: {
      // mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 2;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      break;
    }
    case 12: {
      // mCbm lambda trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 8;
      eb_TriggerMinNumberTof       = 16;
      eb_TriggerMinLayersNumberTof = 8; // # PAL 07/04/2022: Not sure here if it should be 4 or 8 (2 tracks in same det. stack excluded?)
      break;
    }
    case 13: {
      // One hit per detector system w/ big acceptance = mCbm full track trigger
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinNumberTrd1d     = 2;
      eb_TriggerMinNumberTrd1d     = 1;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinNumberRich      = 1;
      break;
    }
    case 14: {
      /// PAL mCbm track trigger Tof, T0 & STS
      eb_TriggerMinNumberBmon      = 1;
      eb_TriggerMinNumberSts       = 4;
      eb_TriggerMinNumberTof       = 8;
      eb_TriggerMinLayersNumberTof = 4;
      eb_TriggerMinLayersNumberSts = 2;
      break;
    }
    default: {
      bTrigSet = false;
      break;
    }
  }
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */
  // ------------------------------------------------------------------------


  // -----   Output filename   ----------------------------------------------
  std::string filename    = Form("%d%s.digi_event.root", uRunId, (bTrigSet ? Form("_%u", uTriggerSet) : ""));
  std::string outfilename = sOutDir + "/" + filename;
  std::cout << "-I- " << myName << ": Output file will be " << outfilename << std::endl;
  std::string histosfilename = sOutDir + "/" + filename;
  histosfilename.replace(histosfilename.find(".digi_event.root"), 16, ".hist.root");
  std::cout << "-I- " << myName << ": Histos file will be " << histosfilename << std::endl;
  // ------------------------------------------------------------------------


  // -----   Performance profiling   ----------------------------------------
  // Set to true if you want some minimal performance profiling output
  bool doPerfProfiling = true;
  // Define if you want a special path and name for the performance profiling output file
  std::string perfProfFileName = sOutDir + "/" + filename;
  perfProfFileName.replace(perfProfFileName.find(".digi_event.root"), 16, ".perf.root");
  std::cout << "-I- " << myName << ": Unpack perf file will be " << perfProfFileName << std::endl;
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
    else if (2350 <= uRunId && uRunId <= 2397) {
      /// Nickel runs: 2350 - 2397 = 23/05/2022 - 25/05/2022
      setupName = "mcbm_beam_2022_05_23_nickel";
    }
    else if (2454 <= uRunId && uRunId <= 2497) {
      /// Lambda Benchmark Gold runs: 2454 - 2497 = 16/06/2022 - 18/06/2022
      setupName = "mcbm_beam_2022_06_16_gold";
    }
    if (defaultSetupName != setupName) {
      std::cout << "Automatic setup choice for run " << uRunId << ": " << setupName << std::endl;
    }
  }
  auto cbmGeoSetup = CbmSetup::Instance();
  cbmGeoSetup->LoadSetup(setupName.c_str());
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

      if (2350 <= uRunId) {
        bmonconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
      }
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
    stsconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= uRunId) {
      stsconfig->SetSystemTimeOffset(-1075);  // [ns] value to be updated
    }
    if (2350 <= uRunId) {
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
    else if (2163 <= uRunId && uRunId <= 2291) {
      ///
      muchconfig->SetParFileName("mMuchParUpto03042022.par");
    }
    else if (2311 <= uRunId && uRunId <= 2315) {
      ///
      muchconfig->SetParFileName("mMuchParUpto10042022.par");
    }
    else if (2316 <= uRunId && uRunId <= 2366) {
      ///
      muchconfig->SetParFileName("mMuchParUpto23052022.par");
    }
    else if (2367 <= uRunId && uRunId <= 2397) {
      /// Starting to use GEM 2 moved to CRI 0 on 24/05/2022
      muchconfig->SetParFileName("mMuchParUpto26052022.par");
    }
    else {
      /// Default file for all other runs (including 06/2022 Gold runs)
      muchconfig->SetParFileName("mMuchPar.par");
    }

    /// Enable duplicates rejection, Ignores the ADC for duplicates check
    muchconfig->SetDuplicatesRejection(true, true);
    muchconfig->SetSystemTimeOffset(-2221);  // [ns] value to be updated
    if (2160 <= uRunId) {
      muchconfig->SetSystemTimeOffset(-1020);  // [ns] value to be updated
    }
    if (2350 <= uRunId) {
      muchconfig->SetSystemTimeOffset(-980);  // [ns] value to be updated
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
    // Get the spadic configuration true = avg baseline active / false plain sample 0
    trd1Dconfig->SetSpadicObject(GetTrdSpadic(true));
    trd1Dconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    if (2160 <= uRunId) {
      trd1Dconfig->SetSystemTimeOffset(1140);  // [ns] value to be updated
    }
    if (2350 <= uRunId) {
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
    if (uRunId <= 1588) {
      const size_t nfasps = 12;
      uint8_t map21[] = {9, 2, 3, 11, 10, 7, 8, 0, 1, 4, 6, 5};
      for (uint32_t i(0); i < nfasps; i++)
        map[i] = map21[i];
      uint16_t crob_map21[] = {0x00f0, 0, 0, 0, 0};
      for (uint32_t i(0); i < NCROBMOD; i++)
        crob_map[i] = crob_map21[i];
    }
    else if (uRunId >= 2335) {
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
    if (2160 <= uRunId) {
      trdfasp2dconfig->SetSystemTimeOffset(-570);  // [ns] value to be updated
    }
    if (2350 <= uRunId) {
      trdfasp2dconfig->SetSystemTimeOffset(-510);  // [ns] value to be updated
    }
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
        if (bBmoninTof) {
          /// Map the BMon components in the TOF par file
          parFileNameTof = "mTofCriParIron_withBmon.par";
        }
      }
      else if (2176 <= uRunId && uRunId <= 2310) {
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
    if (2160 <= uRunId) {
      tofconfig->SetSystemTimeOffset(0);  // [ns] value to be updated
    }
    if (2350 <= uRunId) {
      tofconfig->SetSystemTimeOffset(40);  // [ns] value to be updated
    }
    if (uRunId <= 1659) {
      /// Switch ON the -4 offset in epoch count (hack for Spring-Summer 2021)
      tofconfig->SetFlagEpochCountHack2021();
    }
  }
  // -------------

  // ---- RICH ----
  std::shared_ptr<CbmRichUnpackConfig> richconfig = nullptr;

  richconfig = std::make_shared<CbmRichUnpackConfig>("", uRunId);
  if (richconfig) {
    if (1904 < uRunId) {
      /// Switch to new unpacking algo starting from first combined cosmics run in 2022
      richconfig->SetUnpackerVersion(CbmRichUnpackerVersion::v03);
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
    if (2350 <= uRunId) {
      richconfig->SetSystemTimeOffset(100);  // [ns] value to be updated
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
  evBuildRaw->AddDetector(kRawEventBuilderDetT0);

  // Set T0 as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetT0);
  evBuildRaw->AddDetector(kRawEventBuilderDetTof);

  // Remove detectors not there in 2022
  evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);

  // Add all 2022 detectors in the right order
  evBuildRaw->AddDetector(kRawEventBuilderDetSts);
  //  evBuildRaw->AddDetector(kRawEventBuilderDetMuch);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd);
  evBuildRaw->AddDetector(kRawEventBuilderDetTrd2D);
  evBuildRaw->AddDetector(kRawEventBuilderDetRich);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength):
  // => TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  //evBuildRaw->SetTsParameters(0.0, 2.56e8, 0.0);, 0.0);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength):
  // => TsStartTime=0, TsLength=102.4ms in 2022, TsOverLength=TS overlap not used in mCBM2022?
  evBuildRaw->SetTsParameters(0.0, 1.28e8, 1.12928e8);

  /// FIXME: Disable clang formatting to keep easy parameters overview
  /* clang-format off */
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0,    eb_TriggerMinNumberBmon);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0,    eb_TriggerMaxNumberBMon);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts,   eb_TriggerMinNumberSts);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts,   eb_TriggerMaxNumberSts);
  evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kSts, eb_TriggerMinLayersNumberSts);
/*
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kMuch,  eb_TriggerMinNumberMuch);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kMuch,  eb_TriggerMaxNumberMuch);
*/
  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTrd,   eb_TriggerMinNumberTrd1d);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTrd,   eb_TriggerMaxNumberTrd1d);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTrd2d, eb_TriggerMinNumberTrd2d);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTrd2d, eb_TriggerMaxNumberTrd2d);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, eb_TriggerMaxNumberTof);
  evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kTof, eb_TriggerMinLayersNumberTof);

  evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich,  eb_TriggerMinNumberRich);
  evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich,  eb_TriggerMaxNumberRich);


  evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0,    eb_TrigWinMinBMon,  eb_TrigWinMaxBMon);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts,   eb_TrigWinMinSts,   eb_TrigWinMaxSts);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kMuch,  eb_TrigWinMinMuch,  eb_TrigWinMaxMuch);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd,   eb_TrigWinMinTrd1d, eb_TrigWinMaxTrd1d);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd2d, eb_TrigWinMinTrd2d, eb_TrigWinMaxTrd2d);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof,   eb_TrigWinMinTof,   eb_TrigWinMaxTof);
  evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich,  eb_TrigWinMinRich,  eb_TrigWinMaxRich);

  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kT0,
                                    (0 < eb_TriggerMaxNumberBMon ? eb_TriggerMaxNumberBMon : 50));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kSts,
                                    (0 < eb_TriggerMaxNumberSts ? eb_TriggerMaxNumberSts : 2000));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kMuch,
                                    (0 < eb_TriggerMaxNumberMuch ? eb_TriggerMaxNumberMuch : 1000));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTrd,
                                    (0 < eb_TriggerMaxNumberTrd1d ? eb_TriggerMaxNumberTrd1d : 1500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTrd2d,
                                    (0 < eb_TriggerMaxNumberTrd2d ? eb_TriggerMaxNumberTrd2d : 500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kTof,
                                    (0 < eb_TriggerMaxNumberTof ? eb_TriggerMaxNumberTof : 500));
  evBuildRaw->SetHistogramMaxDigiNb(ECbmModuleId::kRich,
                                    (0 < eb_TriggerMaxNumberRich ? eb_TriggerMaxNumberRich : 600));

  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // Use standard MUCH digis
  evBuildRaw->ChangeMuchBeamtimeDigiFlag();

  // Set Det type to find T0 in TOF digis = Select storage of BMon digis
  if (bBmoninTof) { evBuildRaw->SetT0InTofDetType(); }

  evBuildRaw->SetOutFilename(histosfilename);
  // evBuildRaw->SetOutputBranchPersistent("CbmEvent", kFALSE);
  evBuildRaw->SetWriteHistosToFairSink(kFALSE);
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
  unpack->SetDoPerfProfilingPerTs(doPerfProfiling);
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
  if (nTimeslices < 0) {
    std::cout << "-I- " << myName << ": Starting run over all timeslices in input" << std::endl;
    run->Run(-1, 0);
  }
  else {
    std::cout << "-I- " << myName << ": Starting run over " << nTimeslices
              << " timeslices (or less if not enough in input)" << std::endl;
    run->Run(0, nTimeslices);
  }
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
