/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmHistoServer.h"

#include <memory>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/) {}

//std::unique_ptr<FairMQExHistoCanvasDrawer> getCanvasDrawer();

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
  CbmHistoServer* histoServer = new CbmHistoServer();

  //    histoServer->SetCanvasDrawer(getCanvasDrawer());

  return histoServer;
}
