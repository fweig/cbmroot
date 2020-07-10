/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include <memory>

#include "runFairMQDevice.h"

#include "CbmMqHistoServer.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options) {
  options.add_options()(
    "ChNameIn",
    bpo::value<std::string>()->default_value("histogram-in"),
    "MQ channel name for histos");
  options.add_options()("ChNameHistCfg",
                        bpo::value<std::string>()->default_value("histo-conf"),
                        "MQ channel name for histos config");
  options.add_options()("ChNameCanvCfg",
                        bpo::value<std::string>()->default_value("canvas-conf"),
                        "MQ channel name for canvases config");
  options.add_options()(
    "HistoFileName",
    bpo::value<std::string>()->default_value("HistosMonitorPulser.root"),
    ".root File name for histo saving");
  options.add_options()("histport",
                        bpo::value<uint32_t>()->default_value(8080),
                        "port for histos http server");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) {
  CbmMqHistoServer* histoServer = new CbmMqHistoServer();

  return histoServer;
}
