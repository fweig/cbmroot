/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmTsConsumerReqDevExample.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("IgnOverMs", bpo::value<bool>()->default_value(true), "Ignore overlap MS if true");
  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("ts-request"),
                        "MQ channel name for TS data");
  options.add_options()("TsBlockName", bpo::value<std::string>()->default_value("exampleblock"),
                        "Block name for requesting TS data, TOF SysId request if empty");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmTsConsumerReqDevExample(); }
