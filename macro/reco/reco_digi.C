/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */


// --- Includes needed for IDE
#include <RtypesCore.h>
#if !defined(__CLING__)
#include "CbmDefs.h"
#include "CbmSetup.h"
#include "CbmTaskBuildEvents.h"
#include "CbmTaskTriggerDigi.h"

#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSystemInfo.h>

#include <TStopwatch.h>

#include <memory>
#endif

#include <FairLogger.h>

/** @brief Macro for CBM reconstruction from digi level
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  15 November 2021
 ** @param input          Name of input file (w/o extension .raw.root)
 ** @param nTimeSlices    Number of time-slices to process
 ** @param firstTimeSlice First time-slice (entry) to be processed
 ** @param output         Name of output file (w/o extension .rec.root)
 ** @param setup          Name of predefined geometry setup
 ** @param paramFile      Parameter ROOT file (w/o extension .par.root)
 **
 ** Reconstruction from digi level. Currently included stages:
 ** - Event building (CbmTaskBuildEvents) (STS only)
 **
 ** TODO: To be expanded with the progress of the algo project.
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
void reco_digi(TString input = "", Int_t nTimeSlices = -1, Int_t firstTimeSlice = 0, TString output = "",
               TString setup = "sis100_electron", TString paramFile = "")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "reco_digi";                    // this macro's name for screen log
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  if (input.IsNull()) input = "test";
  TString rawFile = input + ".raw.root";
  TString traFile = input + ".tra.root";
  if (output.IsNull()) output = input;
  TString outFile = output + ".events.root";
  TString monFile = output + ".moni_reco.root";
  if (paramFile.IsNull()) paramFile = input;
  TString parFile = paramFile + ".par.root";
  LOG(info) << myName << ": Input file is     " << rawFile;
  LOG(info) << myName << ": Output file is    " << outFile;
  LOG(info) << myName << ": Parameter file is " << parFile;

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(setup);
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Digi trigger   -------------------------------------------------
  auto trigger         = std::make_unique<CbmTaskTriggerDigi>();
  double triggerWindow = 10.;  // Trigger window size in ns
  int32_t minNumDigis  = 100;  // Trigger threshold in number of digis
  double deadTime      = 50.;  // Minimum time between two triggers
  trigger->SetAlgoParams(triggerWindow, minNumDigis, deadTime);
  trigger->AddSystem(ECbmModuleId::kSts);
  LOG(info) << myName << ": Added task " << trigger->GetName();
  run->AddTask(trigger.release());
  // ------------------------------------------------------------------------


  // -----   Event building   -----------------------------------------------
  auto evtBuild = std::make_unique<CbmTaskBuildEvents>();
  evtBuild->SetEventWindow(ECbmModuleId::kSts, -20., 30.);  // event building time window for STS
  LOG(info) << myName << ": Added task " << evtBuild->GetName();
  run->AddTask(evtBuild.release());
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb      = run->GetRuntimeDb();
  FairParRootFileIo* parIo = new FairParRootFileIo();
  parIo->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo);
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo);
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
  RemoveGeoManager();
  // ------------------------------------------------------------------------

}  // End of main macro function
