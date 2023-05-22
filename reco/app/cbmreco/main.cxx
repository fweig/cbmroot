/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include <TimesliceAutoSource.hpp>

#include <log.hpp>
#include <sstream>

#include <xpu/host.h>

#include "Options.h"
#include "Reco.h"

using namespace cbm::algo;

int main(int argc, char** argv)
{
  Options opts {argc, argv};

  logging::add_console(opts.LogLevel());

  // XPU
  xpu::settings settings;
  settings.profile = opts.CollectKernelTimes();
  settings.device  = opts.Device();
  if (opts.LogLevel() == trace) {
    settings.verbose      = true;
    settings.logging_sink = [](std::string_view msg) { L_(trace) << msg; };
  }
  xpu::initialize(settings);
  xpu::preload<GPUReco>();

  // LOG(info) << "CORE buildType=" << BuildInfo::BUILD_TYPE << " gpuDebug=" << BuildInfo::GPU_DEBUG << " commit=" << BuildInfo::GIT_HASH;
  std::stringstream ss;
  for (int i = 0; i < argc; i++) {
    ss << argv[i] << " ";
  }
  L_(info) << ss.str();

  Reco reco;
  reco.Init(opts);

  fles::TimesliceAutoSource source {opts.InputLocator()};
  int tsIdx  = 0;
  int num_ts = opts.NumTimeslices();
  if (num_ts > 0) { num_ts += opts.SkipTimeslices(); }
  while (auto ts = source.get()) {
    if (tsIdx < opts.SkipTimeslices()) {
      tsIdx++;
      continue;
    }
    reco.Run(*ts);
    tsIdx++;

    if (num_ts > 0 && tsIdx >= num_ts) { break; }
  }

  reco.Finalize();

  return 0;
}
