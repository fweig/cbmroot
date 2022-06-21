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

#include <FairFileSource.h>
#include <FairLogger.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairRunOnline.h>

#include <THttpServer.h>
#include <TRootSniffer.h>

#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>

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
  // --- QA publishing
  fHttpServerRefreshRate = config["qa"]["refreshrate"].as<int32_t>(fHttpServerRefreshRate);
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
  // --- QA publishing
  config["qa"]["refreshrate"] = fHttpServerRefreshRate;
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

  // --- Check if the input is a ROOT file. In that case, digis are already present and
  // --- the unpacking stage must be skipped. The digis are in direct branches of the ROOT
  // --- tree when coming from simulation, or in form of CbmDigiTimeslice if produced
  // --- by a previous unpacking run. This variety is caught by the tasks and need not be
  // --- considered here.
  bool isRootInput =
    fSourceNames.size() == 1 && fSourceNames.at(0).compare(fSourceNames.at(0).size() - 5, 5, ".root") == 0;

  // --- Run instance
  FairRunOnline run;

  // --- Input source
  if (isRootInput) {
    auto source = make_unique<FairFileSource>(fSourceNames.at(0));
    LOG(info) << "Reco: Using ROOT input " << fSourceNames.at(0);
    run.SetSource(source.release());
  }
  else {
    auto source = make_unique<CbmSourceTs>(fSourceNames);
    if (source) LOG(info) << "Reco: Using sources " << ListSources();
    else {
      LOG(error) << "Reco: Could not open sources " << ListSources() << "; aborting.";
      return -1;
    }
    run.SetSource(source.release());
  }

  // --- Output file
  auto sink = make_unique<FairRootFileSink>(fOutputFileName.Data());
  if (sink) LOG(info) << "Reco: Using output file " << fOutputFileName.Data();
  else {
    LOG(error) << "Reco: Could not open output " << fOutputFileName.Data() << "; aborting.";
    return -1;
  }
  run.SetSink(sink.release());

  // --- Event header
  auto header = make_unique<CbmTsEventHeader>();
  run.SetEventHeader(header.release());

  // --- Unpacking
  if (!isRootInput) {
    auto unpack = make_unique<CbmTaskUnpack>();
    unpack->SetOutputBranchPersistent("DigiTimeslice.", fConfig.fStoreTimeslice);
    run.AddTask(unpack.release());
  }

  // --- Digi trigger
  auto trigger = make_unique<CbmTaskTriggerDigi>();
  trigger->AddSystem(fConfig.fTriggerDet);
  trigger->SetAlgoParams(fConfig.fTriggerWin, fConfig.fTriggerThreshold, fConfig.fTriggerDeadTime);
  trigger->SetOutputBranchPersistent("Trigger", fConfig.fStoreTrigger);
  run.AddTask(trigger.release());

  // --- Event building
  auto evtBuild = make_unique<CbmTaskBuildEvents>();
  for (auto& entry : fConfig.fEvtbuildWindows)
    evtBuild->SetEventWindow(entry.first, entry.second.first, entry.second.second);
  evtBuild->SetOutputBranchPersistent("DigiEvent", fConfig.fStoreEvents);
  run.AddTask(evtBuild.release());

  // --- Event QA
  auto evtQa = make_unique<CbmTaskDigiEventQa>();
  evtQa->Config(fConfig);
  run.AddTask(evtQa.release());

  // ----- HttpServer for online monitoring
  if (fHttpServerPort) {
    run.ActivateHttpServer(fConfig.fHttpServerRefreshRate, fHttpServerPort);
    run.GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  }

  // --- Initialise and start run
  timer.Stop();
  double timeSetup = timer.RealTime();
  timer.Start();
  LOG(info) << "Reco: Initialising...";
  run.Init();
  timer.Stop();
  double timeInit = timer.RealTime();

  // --- Start run
  timer.Start();
  run.Run(0, fNumTs);
  timer.Stop();
  double timeRun = timer.RealTime();

  // --- Run log
  size_t numTs = 1;
  if (!isRootInput) {
    auto src = dynamic_cast<CbmSourceTs*>(run.GetSource());
    assert(src);
    numTs = src->GetNumTs();
  }
  // TODO: Don't know how to get the number of processed timeslices for ROOT input.
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
