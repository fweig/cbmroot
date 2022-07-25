/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#include "Application.h"

#include <chrono>
#include <thread>

Application::Application(ProgramOptions const& opt) : fOpt(opt)
{
  // start up monitoring
  if (!fOpt.MonitorUri().empty()) { fMonitor = std::make_unique<cbm::Monitor>(fOpt.MonitorUri()); }

  CbmRecoConfig config;
  config.LoadYaml(fOpt.ConfigYamlFile());
  if (!fOpt.SaveConfigYamlFile().empty()) { config.SaveYaml(fOpt.SaveConfigYamlFile()); }

  fCbmReco =
    std::make_unique<CbmReco>(fOpt.InputUri(), fOpt.OutputRootFile(), fOpt.MaxNumTs(), config, fOpt.HttpServerPort());
}

void Application::Run() { fCbmReco->Run(); }

Application::~Application()
{
  // delay to allow monitor to process pending messages
  constexpr auto destruct_delay = std::chrono::milliseconds(200);
  std::this_thread::sleep_for(destruct_delay);
}
