/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmDevEventSink.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("OutFileName", bpo::value<std::string>()->default_value(""),
                        "Name (full or relative path) of the output .root file ");
  options.add_options()("ChannelNameDataInput", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for digi events");
  options.add_options()("ChannelNameCommands", bpo::value<std::string>()->default_value("commands"),
                        "MQ channel name for commands");
}

FairMQDevicePtr getDevice(const FairMQProgOptions&) { return new CbmDevEventSink(); }
