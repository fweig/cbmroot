/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmDeviceUnpack.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("Setup", bpo::value<std::string>()->default_value("mcbm_beam_2021_07_surveyed"),
                        "Name/tag of the geomatry setup");
  options.add_options()("RunId", bpo::value<uint32_t>()->default_value(1588), "Run ID");
  options.add_options()("IgnOverMs", bpo::value<bool>()->default_value(true), "Ignore overlap MS if true");
  options.add_options()("FullTimeSort", bpo::value<bool>()->default_value(true),
                        "Full time sorting per detector before sending output array");
  options.add_options()("SetTimeOffs", bpo::value<std::vector<std::string>>()->multitoken()->composing(),
                        "Set time offset in ns for selected detector, use string matching "
                        "ECbmModuleId,dOffs e.g. kTof,-35.2");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("fullts"),
                        "MQ channel name for raw TS data");
  options.add_options()("TsNameOut", bpo::value<std::string>()->default_value("unpts_0"),
                        "MQ channel name for unpacked TS data");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceUnpack(); }
