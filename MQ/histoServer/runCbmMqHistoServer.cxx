/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMqHistoServer.h"

#include <memory>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
  options.add_options()("ChNameHistCfg", bpo::value<std::string>()->default_value("histo-conf"),
                        "MQ channel name for histos config");
  options.add_options()("ChNameCanvCfg", bpo::value<std::string>()->default_value("canvas-conf"),
                        "MQ channel name for canvases config");
  options.add_options()("HistoFileName", bpo::value<std::string>()->default_value("HistosMonitorPulser.root"),
                        ".root File name for histo saving");
  options.add_options()("histport", bpo::value<uint32_t>()->default_value(8080), "port for histos http server");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
  CbmMqHistoServer* histoServer = new CbmMqHistoServer();

  return histoServer;
}
