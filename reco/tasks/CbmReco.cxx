/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmReco.h"

#include "CbmSourceTs.h"
#include "CbmTaskBuildEvents.h"
#include "CbmTaskDigiEventQa.h"
#include "CbmTaskTriggerDigi.h"
#include "CbmTaskUnpack.h"
#include "CbmTsEventHeader.h"

#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunOnline.h>

#include <THttpServer.h>
#include <TRootSniffer.h>

#include <iostream>
#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>

using std::cout;
using std::endl;
using std::make_unique;
using std::string;


// -----   Load configuration from YAML file   --------------------------------
void CbmRecoConfig::LoadYaml(const std::string& filename)
{
  YAML::Node config = YAML::LoadFile(filename);
  // --- Digi trigger
  fTriggerDet       = ToCbmModuleIdCaseInsensitive(config["trigger"]["detector"].as<std::string>());
  fTriggerWin       = config["trigger"]["window"].as<double>();
  fTriggerThreshold = config["trigger"]["threshold"].as<size_t>();
  fTriggerDeadTime  = config["trigger"]["deadtime"].as<double>();
  // --- Event builder: (detector -> (tMin, tMax))
  if (auto eventbuilder = config["eventbuilder"]) {
    if (auto windows = eventbuilder["windows"]) {
      for (YAML::const_iterator it = windows.begin(); it != windows.end(); ++it) {
        auto det              = ToCbmModuleIdCaseInsensitive(it->first.as<std::string>());
        auto lower            = it->second[0].as<double>();
        auto upper            = it->second[1].as<double>();
        fEvtbuildWindows[det] = std::make_pair(lower, upper);
      }
    }
  }
  // --- Branch persistence in output file
  fStoreTimeslice = config["store"]["timeslice"].as<bool>();
  fStoreTrigger   = config["store"]["triggers"].as<bool>();
  fStoreEvents    = config["store"]["events"].as<bool>();
}
// ----------------------------------------------------------------------------


// -----   Save configuration to YAML file   ----------------------------------
void CbmRecoConfig::SaveYaml(const std::string& filename)
{
  YAML::Node config;
  // --- Digi trigger
  config["trigger"]["detector"]  = ToString(fTriggerDet);
  config["trigger"]["window"]    = fTriggerWin;
  config["trigger"]["threshold"] = fTriggerThreshold;
  config["trigger"]["deadtime"]  = fTriggerDeadTime;
  // --- Event builder: (detector -> (tMin, tMax))
  for (const auto& [key, value] : fEvtbuildWindows) {
    auto det = ToString(key);
    config["eventbuilder"]["windows"][det].push_back(value.first);
    config["eventbuilder"]["windows"][det].push_back(value.second);
  };
  // --- Branch persistence in output file
  config["store"]["timeslice"] = fStoreTimeslice;
  config["store"]["triggers"]  = fStoreTrigger;
  config["store"]["events"]    = fStoreEvents;
  // ---
  std::ofstream fout(filename);
  fout << config;
}
// ----------------------------------------------------------------------------


// -----   Constructor from single source   -----------------------------------
CbmReco::CbmReco(string source, TString outFile, int32_t numTs, const CbmRecoConfig& config, uint16_t port)
  : fSourceNames {source}
  , fOutputFileName(outFile)
  , fNumTs(numTs)
  , fConfig(config)
  , fHttpServerPort(port)
{
}
// ----------------------------------------------------------------------------


// -----   Constructor from multiple sources   --------------------------------
CbmReco::CbmReco(std::vector<string> sources, TString outFile, int32_t numTs, const CbmRecoConfig& config,
                 uint16_t port)
  : fSourceNames(sources)
  , fOutputFileName(outFile)
  , fNumTs(numTs)
  , fConfig(config)
  , fHttpServerPort(port)
{
}
// ----------------------------------------------------------------------------


// -----   List of source names   ---------------------------------------------
std::string CbmReco::ListSources() const
{
  std::string result = "{";
  for (auto& source : fSourceNames) {
    result += source + ", ";
  }
  result += "}";
  return result;
}
// ----------------------------------------------------------------------------


// -----   Configure and execute run   ----------------------------------------
int32_t CbmReco::Run()
{

  // --- Timing
  TStopwatch timer;
  timer.Start();

  // --- Input source
  auto source = make_unique<CbmSourceTs>(fSourceNames);
  if (source) LOG(info) << "Reco: Using sources " << ListSources();
  else {
    LOG(error) << "Reco: Could not open sources " << ListSources() << "; aborting.";
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

  // --- Event QA
  auto evtQa = make_unique<CbmTaskDigiEventQa>();
  evtQa->Config(fConfig);

  // --- Run configuration
  FairRunOnline run(source.release());
  run.SetSink(sink.release());
  run.SetEventHeader(header.release());
  run.AddTask(unpack.release());
  run.AddTask(trigger.release());
  run.AddTask(evtBuild.release());
  run.AddTask(evtQa.release());

  // ----- HttpServer for online monitoring
  if (fHttpServerPort) {
    Int_t serverRefreshRate = 100;  // timeslices
    run.ActivateHttpServer(serverRefreshRate, fHttpServerPort);
    run.GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  }

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
  auto src = dynamic_cast<CbmSourceTs*>(run.GetSource());
  assert(src);
  size_t numTs     = src->GetNumTs();
  double timeTotal = timeSetup + timeInit + timeRun;
  LOG(info) << "=====================================";
  LOG(info) << "Reco: Run summary";
  LOG(info) << "Timeslices  : " << numTs;
  LOG(info) << "Time setup  : " << timeSetup << " s";
  LOG(info) << "Time init   : " << timeInit << " s";
  LOG(info) << "Time run    : " << timeRun << " s";
  LOG(info) << "Time total  : " << timeTotal << " s"
            << " (" << timeTotal / numTs << " s/ts)";
  LOG(info) << "Output file : " << fOutputFileName;
  LOG(info) << "=====================================";

  return numTs;
}
// ----------------------------------------------------------------------------

ClassImp(CbmReco)
