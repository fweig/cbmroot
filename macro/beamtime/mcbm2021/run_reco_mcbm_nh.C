/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */

/** @file run_reco.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 14 November 2020
 **/


// --- Includes needed for IDE
#include <RtypesCore.h>
#if !defined(__CLING__)
#include "CbmBuildEventsFromTracksReal.h"
#include "CbmBuildEventsIdeal.h"
#include "CbmBuildEventsQa.h"
#include "CbmDefs.h"
#include "CbmEvbuildRawTask.h"
#include "CbmFindPrimaryVertex.h"
#include "CbmKF.h"
#include "CbmL1.h"
#include "CbmL1StsTrackFinder.h"
#include "CbmLitFindGlobalTracks.h"
#include "CbmMCDataManager.h"
#include "CbmMatchRecoToMC.h"
#include "CbmMuchFindHitsGem.h"
#include "CbmMvdClusterfinder.h"
#include "CbmMvdHitfinder.h"
#include "CbmPVFinderKF.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmPsdHitProducer.h"
#include "CbmRecoSts.h"
#include "CbmRichHitProducer.h"
#include "CbmRichReconstruction.h"
#include "CbmSetup.h"
#include "CbmStsFindTracks.h"
#include "CbmStsFindTracksEvents.h"
#include "CbmStsTrackFinder.h"
#include "CbmTaskBuildRawEvents.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"

#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSystemInfo.h>

#include <TStopwatch.h>
#endif


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
void run_reco_mcbm_nh(TString input = "data/1588_node8_1_0000", Int_t nTimeSlices = 1000, Int_t firstTimeSlice = 0,
                      TString output = "", TString sEvBuildRaw = "Real", TString setup = "mcbm_beam_2021_03",
                      TString paramFile = "", Bool_t useMC = false)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "info";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco";                     // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  if (input.IsNull()) input = "test";
  TString rawFile = input + ".digi.root";
  TString traFile = input + ".tra.root";
  if (output.IsNull()) output = input;
  TString outFile = output + ".reco.root";
  TString monFile = output + ".moni_reco.root";
  if (paramFile.IsNull()) paramFile = input;
  TString parFile = paramFile + ".par.root";
  std::cout << "Inputfile " << rawFile << std::endl;
  std::cout << "Outfile " << outFile << std::endl;
  std::cout << "Parfile " << parFile << std::endl;

  TString shcmd = "rm -v " + parFile;
  gSystem->Exec(shcmd.Data());

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(setup);
  // ------------------------------------------------------------------------


  // -----   Some global switches   -----------------------------------------
  Bool_t eventBased = !sEvBuildRaw.IsNull();
  Bool_t useMvd     = geo->IsActive(ECbmModuleId::kMvd);
  Bool_t useSts     = geo->IsActive(ECbmModuleId::kSts);
  Bool_t useRich    = geo->IsActive(ECbmModuleId::kRich);
  Bool_t useMuch    = geo->IsActive(ECbmModuleId::kMuch);
  Bool_t useTrd     = geo->IsActive(ECbmModuleId::kTrd);
  Bool_t useTof     = geo->IsActive(ECbmModuleId::kTof);
  Bool_t usePsd     = geo->IsActive(ECbmModuleId::kPsd);

  useMvd  = kFALSE;
  useMuch = kFALSE;
  useRich = kFALSE;
  useTrd  = kFALSE;
  useTof  = kFALSE;
  usePsd  = kFALSE;
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  if (useMC) { inputSource->AddFriend(traFile); }
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monFile);
  // ------------------------------------------------------------------------

  // -----   MCDataManager  -----------------------------------
  if (useMC) {
    CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 0);
    mcManager->AddFile(traFile);
    run->AddTask(mcManager);
  }
  // ------------------------------------------------------------------------

  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Raw event building from digis   --------------------------------
  if (eventBased) {
    if (sEvBuildRaw.EqualTo("Ideal", TString::ECaseCompare::kIgnoreCase)) {
      FairTask* evBuildRaw = new CbmBuildEventsIdeal();
      run->AddTask(evBuildRaw);
      std::cout << "-I- " << myName << ": Added task " << evBuildRaw->GetName() << std::endl;
      eventBased = kTRUE;
    }  //? Ideal raw event building
    else if (sEvBuildRaw.EqualTo("Real", TString::ECaseCompare::kIgnoreCase)) {
      CbmEvbuildRawTask* evBuildRaw = new CbmEvbuildRawTask();

      //Choose between NoOverlap, MergeOverlap, AllowOverlap
      evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

      // Remove detectors where digis not found
      if (!useRich) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
      if (!useMuch) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
      if (!usePsd) evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
      if (!useTof) evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);
      if (!useTrd) evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
      if (!useSts) {
        std::cerr << "-E- " << myName << ": Sts must be present for raw event "
                  << "building using ``Real2019'' option. Terminating macro." << std::endl;
        return;
      }
      // Set STS as reference detector
      evBuildRaw->SetReferenceDetector(kRawEventBuilderDetTof);
      evBuildRaw->SetTsParameters(0.0, 1.e7, 0.0);

      // Use CbmMuchDigi instead of CbmMuchBeamtimeDigi
      // evBuildRaw->ChangeMuchBeamtimeDigiFlag(kFALSE);

      //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, 1000);
      evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, 10);
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, -5, 50);
      //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -500, 500);

      run->AddTask(evBuildRaw);
      std::cout << "-I- " << myName << ": Added task " << evBuildRaw->GetName() << std::endl;
      eventBased = kTRUE;
    }  //? Real raw event building
    else {
      std::cerr << "-E- " << myName << ": Unknown option " << sEvBuildRaw
                << " for raw event building! Terminating macro execution." << std::endl;
      return;
    }
  }  //? event-based reco
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(firstTimeSlice, nTimeSlices);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is    " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
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


  // -----   This is to prevent a malloc error when exiting ROOT   ----------
  // The source of the error is unknown. Related to TGeoManager.
  //RemoveGeoManager();
  // ------------------------------------------------------------------------

}  // End of main macro function
