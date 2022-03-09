/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland, Volker Friese, Pierre-Alain Loizeau, Pascal Raisig [committer], Dominik Smith, Adrian A. Weber  */

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
std::shared_ptr<CbmRichUnpackMonitor> GetRichMonitor(std::string treefilename, bool bDebugMode = false);
const char* defaultSetupName = "mcbm_beam_2021_07_surveyed";

void run_unpack_online(std::vector<std::string> publisher = {"tcp://localhost:5556"}, Int_t serverHttpPort = 8080,
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
  auto cbmsetup = CbmSetup::Instance();
  cbmsetup->LoadSetup(setupName);
  // ------------------------------------------------------------------------

  // -----   UnpackerConfigs   ----------------------------------------------

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
    if (runid == 1588) richconfig->MaskDiRICH(0x7150);
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
  }
  // -------------

  // ---- TRDFASP2D ----
  std::shared_ptr<CbmTrdUnpackFaspConfig> trdfasp2dconfig = nullptr;

  trdfasp2dconfig = std::make_shared<CbmTrdUnpackFaspConfig>(trdsetuptag.Data());
  if (trdfasp2dconfig) {
    // trdfasp2dconfig->SetDebugState();
    trdfasp2dconfig->SetDoWriteOutput();
    // Activate the line below to write Trd1D digis to a separate "TrdFaspDigi" branch. Can be used to separate between Fasp and Spadic digis
    //trdfasp2dconfig->SetOutputBranchName("TrdFaspDigi");
    uint8_t map[NFASPMOD];
    if (runid <= 1588) {
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
    trdfasp2dconfig->SetMonitor(dynamic_pointer_cast<CbmTrdUnpackFaspMonitor>(GetTrdMonitor(outfilename, 1)));
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
  auto source = new CbmSourceTsArchive(publisher);
  auto unpack = source->GetRecoUnpack();
  unpack->SetDoPerfProfiling(doPerfProfiling);
  unpack->SetOutputFilename(perfProfFileName);
  // Enable full time sorting instead sorting per FLIM link
  unpack->SetTimeSorting(true);

  if (psdconfig) unpack->SetUnpackConfig(psdconfig);
  if (richconfig) unpack->SetUnpackConfig(richconfig);
  if (stsconfig) unpack->SetUnpackConfig(stsconfig);
  if (trd1Dconfig) unpack->SetUnpackConfig(trd1Dconfig);
  if (trdfasp2dconfig) unpack->SetUnpackConfig(trdfasp2dconfig);
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

void run_unpack_online(std::string publisher = "tcp://localhost:5556", Int_t serverHttpPort = 8080,
                       Int_t serverRefreshRate = 100, std::int32_t nevents = -1, UInt_t runid = 1905,
                       const char* setupName = defaultSetupName, std::string outpath = "data/")
{
  std::vector<std::string> vPublisher = {publisher};
  return run_unpack_online(vPublisher, serverHttpPort, serverRefreshRate, nevents, runid, setupName, outpath);
}
