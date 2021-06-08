/* Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmDevNullSink.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/)
{
  //    options.add_options()
  //        ("max-iterations", bpo::value<uint64_t>()->default_value(0), "Maximum number of iterations of Run/ConditionalRun/OnData (0 - infinite)");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDevNullSink(); }
