/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#include "Application.h"

Application::Application(ProgramOptions const& opt) : fOpt(opt)
{
  CbmRecoConfig config;
  config.LoadYaml(fOpt.ConfigYamlFile());
  if (!fOpt.SaveConfigYamlFile().empty()) { config.SaveYaml(fOpt.SaveConfigYamlFile()); }

  fCbmReco =
    std::make_unique<CbmReco>(fOpt.InputUri(), fOpt.OutputRootFile(), fOpt.MaxNumTs(), config, fOpt.HttpServerPort());
}

void Application::Run() { fCbmReco->Run(); }
