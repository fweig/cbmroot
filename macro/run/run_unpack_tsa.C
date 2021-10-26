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

std::shared_ptr<CbmTrdUnpackMonitor> GetTrdMonitor(std::string treefilename);
std::shared_ptr<CbmTrdSpadic> GetTrdSpadic(bool useAvgBaseline = false);
std::shared_ptr<CbmStsUnpackMonitor> GetStsMonitor(std::string treefilename, bool bDebugMode = false);
const char* defaultSetupName = "mcbm_beam_2021_07_surveyed";

void run_unpack_tsa(std::vector<std::string> infile = {"test.tsa"}, UInt_t runid = 0,
                    const char* setupName = defaultSetupName, std::int32_t nevents = -1, std::string outpath = "data/")
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
    richconfig->SetDebugState();
    richconfig->SetDoWriteOutput();
    std::string parfilesbasepathRich = Form("%s/macro/beamtime/mcbm2021/", srcDir.Data());
    richconfig->SetParFilesBasePath(parfilesbasepathRich);
    richconfig->SetSystemTimeOffset(256000 - 1200);  // [ns] 1 MS and additional correction
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

    stsconfig->MaskNoisyChannel(7, 715);
    stsconfig->MaskNoisyChannel(7, 162);
    stsconfig->MaskNoisyChannel(7, 159);
    stsconfig->MaskNoisyChannel(7, 158);
    stsconfig->MaskNoisyChannel(7, 125);
    stsconfig->MaskNoisyChannel(7, 124);
    stsconfig->MaskNoisyChannel(7, 123);
    stsconfig->MaskNoisyChannel(12, 119);
    stsconfig->MaskNoisyChannel(3, 85);
    stsconfig->MaskNoisyChannel(3, 79);
    stsconfig->MaskNoisyChannel(3, 75);
    stsconfig->MaskNoisyChannel(3, 56);
    stsconfig->MaskNoisyChannel(9, 709);

    // Time OffSet
    // U2 MOD1
    stsconfig->SetAsicTimeOffset(104, 0.235097);
    stsconfig->SetAsicTimeOffset(105, -0.919854);
    stsconfig->SetAsicTimeOffset(106, 0.470871);
    stsconfig->SetAsicTimeOffset(107, -0.302136);
    stsconfig->SetAsicTimeOffset(108, 0.0538701);
    stsconfig->SetAsicTimeOffset(109, -0.726945);
    stsconfig->SetAsicTimeOffset(110, -0.474958);
    stsconfig->SetAsicTimeOffset(111, -0.426105);
    stsconfig->SetAsicTimeOffset(112, -0.642128);
    stsconfig->SetAsicTimeOffset(113, 3.90217);
    stsconfig->SetAsicTimeOffset(114, 4.50206);
    stsconfig->SetAsicTimeOffset(115, 4.16447);
    stsconfig->SetAsicTimeOffset(116, 5.00525);
    stsconfig->SetAsicTimeOffset(117, 4.2554);
    stsconfig->SetAsicTimeOffset(118, 5.51313);
    stsconfig->SetAsicTimeOffset(119, 3.38058);
    // U2 MOD0
    stsconfig->SetAsicTimeOffset(88, 5.32785);
    stsconfig->SetAsicTimeOffset(89, 5.10337);
    stsconfig->SetAsicTimeOffset(90, 6.63223);
    stsconfig->SetAsicTimeOffset(91, 4.44318);
    stsconfig->SetAsicTimeOffset(92, 6.5406);
    stsconfig->SetAsicTimeOffset(93, 3.82063);
    stsconfig->SetAsicTimeOffset(94, 5.8673);
    stsconfig->SetAsicTimeOffset(95, 4.41679);
    stsconfig->SetAsicTimeOffset(96, 0.917994);
    stsconfig->SetAsicTimeOffset(97, 2.7819);
    stsconfig->SetAsicTimeOffset(98, 5.61836);
    stsconfig->SetAsicTimeOffset(99, 5.9533);
    stsconfig->SetAsicTimeOffset(100, 5.9789);
    stsconfig->SetAsicTimeOffset(101, 5.24167);
    stsconfig->SetAsicTimeOffset(102, 7.14848);
    stsconfig->SetAsicTimeOffset(103, 6.12267);
    // U1 MOD1
    stsconfig->SetAsicTimeOffset(64, 6.94614);
    stsconfig->SetAsicTimeOffset(65, 6.79433);
    stsconfig->SetAsicTimeOffset(66, 5.57188);
    stsconfig->SetAsicTimeOffset(67, 4.78256);
    stsconfig->SetAsicTimeOffset(68, 4.47773);
    stsconfig->SetAsicTimeOffset(69, 3.70702);
    stsconfig->SetAsicTimeOffset(70, 4.02281);
    stsconfig->SetAsicTimeOffset(71, 4.74402);
    stsconfig->SetAsicTimeOffset(72, 2.13448);
    stsconfig->SetAsicTimeOffset(73, 3.28798);
    stsconfig->SetAsicTimeOffset(74, 4.13604);
    stsconfig->SetAsicTimeOffset(76, 5.02552);
    stsconfig->SetAsicTimeOffset(77, 3.55128);
    stsconfig->SetAsicTimeOffset(78, 4.66596);
    stsconfig->SetAsicTimeOffset(79, 3.76298);
    // U1 MOD0
    stsconfig->SetAsicTimeOffset(48, 10.0742);
    stsconfig->SetAsicTimeOffset(49, 9.31221);
    stsconfig->SetAsicTimeOffset(50, 6.09788);
    stsconfig->SetAsicTimeOffset(51, 2.85866);
    stsconfig->SetAsicTimeOffset(52, 5.45111);
    stsconfig->SetAsicTimeOffset(53, 3.18376);
    stsconfig->SetAsicTimeOffset(54, 4.77448);
    stsconfig->SetAsicTimeOffset(55, 3.20388);
    stsconfig->SetAsicTimeOffset(56, 3.37812);
    stsconfig->SetAsicTimeOffset(57, 4.08165);
    stsconfig->SetAsicTimeOffset(58, 5.06481);
    stsconfig->SetAsicTimeOffset(59, 4.39388);
    stsconfig->SetAsicTimeOffset(60, 5.63931);
    stsconfig->SetAsicTimeOffset(61, 4.15354);
    stsconfig->SetAsicTimeOffset(62, 6.67062);
    stsconfig->SetAsicTimeOffset(63, 6.33579);
    // U0 MOD1
    stsconfig->SetAsicTimeOffset(24, 5.01505);
    stsconfig->SetAsicTimeOffset(25, 3.84039);
    stsconfig->SetAsicTimeOffset(26, 5.36344);
    stsconfig->SetAsicTimeOffset(27, 2.96807);
    stsconfig->SetAsicTimeOffset(28, 4.14668);
    stsconfig->SetAsicTimeOffset(29, 2.4282);
    stsconfig->SetAsicTimeOffset(30, 4.02055);
    stsconfig->SetAsicTimeOffset(31, 4.13309);
    stsconfig->SetAsicTimeOffset(32, 1.2073);
    stsconfig->SetAsicTimeOffset(33, 4.02747);
    stsconfig->SetAsicTimeOffset(34, 5.03306);
    stsconfig->SetAsicTimeOffset(35, 3.5234);
    stsconfig->SetAsicTimeOffset(36, 5.41946);
    stsconfig->SetAsicTimeOffset(37, 4.82807);
    stsconfig->SetAsicTimeOffset(38, 6.33436);
    stsconfig->SetAsicTimeOffset(39, 4.71413);
    // U0 MOD0
    stsconfig->SetAsicTimeOffset(8, 8.15031);
    stsconfig->SetAsicTimeOffset(9, 6.99897);
    stsconfig->SetAsicTimeOffset(10, 8.14826);
    stsconfig->SetAsicTimeOffset(11, 3.29095);
    stsconfig->SetAsicTimeOffset(12, 3.04337);
    stsconfig->SetAsicTimeOffset(14, 3.71628);
    stsconfig->SetAsicTimeOffset(15, 3.82512);
    stsconfig->SetAsicTimeOffset(16, 4.05862);
    stsconfig->SetAsicTimeOffset(17, 4.53291);
    stsconfig->SetAsicTimeOffset(18, 5.22825);
    stsconfig->SetAsicTimeOffset(19, 4.93144);
    stsconfig->SetAsicTimeOffset(20, 6.33821);
    stsconfig->SetAsicTimeOffset(21, 5.66979);
    stsconfig->SetAsicTimeOffset(22, 9.33802);
    stsconfig->SetAsicTimeOffset(23, 7.41302);
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
  std::shared_ptr<CbmTrdUnpackConfigFasp2D> trdfasp2dconfig = nullptr;

  trdfasp2dconfig = std::make_shared<CbmTrdUnpackConfigFasp2D>(trdsetuptag.Data());
  if (trdfasp2dconfig) {
    // trdfasp2dconfig->SetDebugState();
    trdfasp2dconfig->SetDoWriteOutput();
    // Activate the line below to write Trd1D digis to a separate "TrdFaspDigi" branch. Can be used to separate between Fasp and Spadic digis
    trdfasp2dconfig->SetOutputBranchName("TrdFaspDigi");
    std::string parfilesbasepathTrdfasp2d = Form("%s/parameters/trd", srcDir.Data());
    trdfasp2dconfig->SetParFilesBasePath(parfilesbasepathTrdfasp2d);
    trdfasp2dconfig->SetSystemTimeOffset(-1800);  // [ns] value to be updated
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
  /// Enable full time sorting instead of time sorting per FLIM link
  //unpack->SetTimeSorting(true);
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
std::shared_ptr<CbmTrdUnpackMonitor> GetTrdMonitor(std::string treefilename)
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

  auto monitor = std::make_shared<CbmTrdUnpackMonitor>();
  monitor->SetActiveHistos(digihistovec);
  monitor->SetActiveHistos(rawhistovec);
  monitor->SetActiveHistos(otherhistovec);
  monitor->SetWriteToFile(outfilename.data());

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

void run_unpack_tsa(std::string infile = "test.tsa", UInt_t runid = 0, const char* setupName = defaultSetupName,
                    std::int32_t nevents = -1, std::string outpath = "data/")
{
  std::vector<std::string> vInFile = {infile};
  return run_unpack_tsa(vInFile, runid, setupName, nevents, outpath);
}
