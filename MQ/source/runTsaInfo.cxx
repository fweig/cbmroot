/* Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmMQTsaInfo.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("filename", bpo::value<std::string>()->default_value(""), "Filename of the input file")(
    "flib-host", bpo::value<std::string>()->default_value(""), "Host where the timeslice server is running")(
    "max-timeslices", bpo::value<uint64_t>()->default_value(0),
    "Maximum number of timeslices to process for Run/ConditionalRun/OnData (0 "
    "- infinite)")("flib-port", bpo::value<uint64_t>()->default_value(0), "Port where the timeslice server is running");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmMQTsaInfo(); }
