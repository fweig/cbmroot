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


/** @brief Macro for CBM reconstruction
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  14 November 2020
 ** @param input          Name of input file (w/o extension .raw.root)
 ** @param nTimeSlices    Number of time-slices to process
 ** @param firstTimeSlice First time-slice (entry) to be processed
 ** @param output         Name of output file (w/o extension .rec.root)
 ** @param sEvBuildRaw    Option for raw event building
 ** @param setup          Name of predefined geometry setup
 ** @param paramFile      Parameter ROOT file (w/o extension .par.root)
 ** @param useMC          Option to provide the trackfinder with MC information
 **
 ** This macro performs from the digis in a time-slice. It can be used
 ** for simulated data (result of run_digi.C) or real data after unpacking.
 **
 ** The macro covers both time-based reconstruction and event-based
 ** reconstruction using raw events build from digis. This can be selected
 ** by the forth argument. If left empty, no raw event builder will be
 ** employed and reconstruction will be time-based. The option "Ideal"
 ** selects the ideal raw event builder, which associates digis to events
 ** based on the MC truth. The option "Real" selects a real raw event builder
 ** (latest version, for older versions use "Real2018" or "Real2019").
 ** 
 **
 ** The file names must be specified without extensions. The convention is
 ** that the raw (input) file is [input].raw.root. The output file
 ** will be [input].rec.root if not specified by the user. The parameter file
 ** has the extension .par.root. It is assumed to be [input].par.root if
 ** not specified by the user.
 **
 ** If no argument is specified, the input will be set to "test". This allows
 ** to execute the macro chain (run_tra_file.C, run_digi.C and run_reco.C)
 ** from the ROOT prompt without user intervention.
 **
 **/
void run_unpack_tsa(std::string infile = "test.tsa", UInt_t runid = 0, const char* setupName = "mcbm_beam_2021_03",
                    std::int32_t nevents = -1, std::string outpath = "")
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
  std::string outfilename = infile;
  auto filenamepos        = infile.find_last_of("/");
  filenamepos++;
  std::string filename = infile.substr(filenamepos);
  if (filename.find("*") != infile.npos) filename = std::to_string(runid) + ".tsa";
  if (filename.find(";") != infile.npos) filename = std::to_string(runid) + "_merged" + ".tsa";
  if (outpath.empty()) { outpath = infile.substr(0, filenamepos); }
  outfilename = outpath + filename;
  outfilename.replace(outfilename.find(".tsa"), 4, ".digi.root");
  // ------------------------------------------------------------------------


  // -----   CbmSetup   -----------------------------------------------------
  auto cbmsetup = CbmSetup::Instance();
  cbmsetup->LoadSetup(setupName);
  // ------------------------------------------------------------------------

  // -----   UnpackerConfigs   ----------------------------------------------

  // ---- TRD ----
  TString trdsetuptag = "";
  cbmsetup->GetGeoTag(ECbmModuleId::kTrd, trdsetuptag);
  // auto trdconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), runid);
  auto trdconfig = std::make_shared<CbmTrdUnpackConfig>(trdsetuptag.Data(), 3);
  // trdconfig->SetDebugState();
  trdconfig->SetDoWriteOutput();
  trdconfig->SetDoWriteOptOutA(CbmTrdRawMessageSpadic::GetBranchName());
  // trdconfig->SetDoWriteOptOutB("SpadicInfoMessages"); // SpadicInfoMessages

  std::string parfilesbasepath = Form("%s/parameters/trd", srcDir.Data());
  trdconfig->SetParFilesBasePath(parfilesbasepath);
  trdconfig->SetMonitor(GetTrdMonitor(outfilename));
  // Get the spadic configuration true = avg baseline active / false plain sample 0
  trdconfig->SetSpadicObject(GetTrdSpadic(true));
  // -------------

  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   CbmSourceTsArchive   -------------------------------------------
  auto source = new CbmSourceTsArchive(infile.data());
  auto unpack = source->GetRecoUnpack();
  unpack->SetUnpackConfig(trdconfig);
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  auto run  = new FairRunOnline(source);
  auto sink = new FairRootFileSink(outfilename.data());
  run->SetSink(sink);
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
  std::cout << "Macro finished successfully after CpuTime = " << timer.CpuTime() << " s RealTime = " << timer.RealTime()
            << " s." << std::endl;
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
    CbmTrdUnpackMonitor::eDigiHistos::kMap,         CbmTrdUnpackMonitor::eDigiHistos::kMap_St,
    CbmTrdUnpackMonitor::eDigiHistos::kMap_Nt,      CbmTrdUnpackMonitor::eDigiHistos::kCharge,
    CbmTrdUnpackMonitor::eDigiHistos::kCharge_St,   CbmTrdUnpackMonitor::eDigiHistos::kCharge_Nt,
    CbmTrdUnpackMonitor::eDigiHistos::kTriggerType, CbmTrdUnpackMonitor::eDigiHistos::kDigiDeltaT};

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