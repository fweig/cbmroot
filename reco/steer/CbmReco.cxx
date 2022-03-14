/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmReco.h"

#include "CbmSourceTs.h"
#include "CbmTaskBuildEvents.h"
#include "CbmTaskTriggerDigi.h"
#include "CbmTaskUnpack.h"
#include "CbmTsEventHeader.h"

#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunOnline.h>

#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::make_unique;


// -----   Constructor   ------------------------------------------------------
CbmReco::CbmReco(TString source, TString outFile, int32_t numTs, const CbmRecoConfig& config)
  : fInputFileName(source)
  , fOutputFileName(outFile)
  , fNumTs(numTs)
  , fConfig(config)
{
}
// ----------------------------------------------------------------------------


// -----   Configure and execute run   ----------------------------------------
int32_t CbmReco::Run()
{

  // --- Timing
  TStopwatch timer;
  timer.Start();

  // --- Input source
  auto source = make_unique<CbmSourceTs>(fInputFileName.Data());
  if (source) LOG(info) << "Reco: Using source " << fInputFileName.Data();
  else {
    LOG(error) << "Reco: Could not open source " << fInputFileName.Data() << "; aborting.";
    return -1;
  }

  // --- Output file
  auto sink = make_unique<FairRootFileSink>(fOutputFileName.Data());
  if (sink) LOG(info) << "Reco: Using output file " << fOutputFileName.Data();
  else {
    LOG(error) << "Reco: Could not open output " << fOutputFileName.Data() << "; aborting.";
    return -1;
  }

  // --- Event header
  auto header = make_unique<CbmTsEventHeader>();

  // --- Unpacking
  auto unpack = make_unique<CbmTaskUnpack>();
  unpack->SetOutputBranchPersistent("DigiTimeslice.", fConfig.fStoreTimeslice);

  // --- Digi trigger
  auto trigger = make_unique<CbmTaskTriggerDigi>();
  trigger->AddSystem(fConfig.fTriggerDet);
  trigger->SetAlgoParams(fConfig.fTriggerWin, fConfig.fTriggerThreshold, fConfig.fTriggerDeadTime);
  trigger->SetOutputBranchPersistent("Trigger", fConfig.fStoreTrigger);

  // --- Event building
  auto evtBuild = make_unique<CbmTaskBuildEvents>();
  for (auto& entry : fConfig.fEvtbuildWindows)
    evtBuild->SetEventWindow(entry.first, entry.second.first, entry.second.second);
  evtBuild->SetOutputBranchPersistent("DigiEvent", fConfig.fStoreEvents);

  // --- Run configuration
  FairRunOnline run(source.release());
  run.SetSink(sink.release());
  run.SetEventHeader(header.release());
  run.AddTask(unpack.release());
  run.AddTask(trigger.release());
  run.AddTask(evtBuild.release());

  // --- Initialise and start run
  timer.Stop();
  double timeSetup = timer.RealTime();
  timer.Start();
  cout << endl << endl;
  LOG(info) << "Reco: Initialising...";
  run.Init();
  timer.Stop();
  double timeInit = timer.RealTime();

  // --- Start run
  timer.Start();
  cout << endl << endl;
  run.Run(0, fNumTs);
  timer.Stop();
  double timeRun = timer.RealTime();

  // --- Run log
  std::cout << std::endl;
  double timeTotal = timeSetup + timeInit + timeRun;
  LOG(info) << "=====================================";
  LOG(info) << "Reco: Run summary";
  LOG(info) << "Timeslices  : " << fNumTs;
  LOG(info) << "Time setup  : " << timeSetup << " s";
  LOG(info) << "Time init   : " << timeInit << " s";
  LOG(info) << "Time run    : " << timeRun << " s";
  LOG(info) << "Time total  : " << timeTotal << " s"
            << " (" << timeTotal / fNumTs << " s/ts)";
  LOG(info) << "Output file : " << fOutputFileName;
  LOG(info) << "=====================================";

  return fNumTs;
}
// ----------------------------------------------------------------------------

ClassImp(CbmReco)
