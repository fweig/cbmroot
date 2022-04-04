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
  options.add_options()("UnpBmon", bpo::value<bool>()->default_value(false), "Enable Bmon unpacking if true");
  options.add_options()("UnpSts", bpo::value<bool>()->default_value(true), "Enable STS unpacking if true");
  options.add_options()("UnpMuch", bpo::value<bool>()->default_value(false), "Enable MUCH unpacking if true");
  options.add_options()("UnpTrd1d", bpo::value<bool>()->default_value(true), "Enable TRD 1D unpacking if true");
  options.add_options()("UnpTrd2d", bpo::value<bool>()->default_value(true), "Enable TRD 2D unpacking if true");
  options.add_options()("UnpTof", bpo::value<bool>()->default_value(true), "Enable TOF unpacking if true");
  options.add_options()("UnpRich", bpo::value<bool>()->default_value(true), "Enable RICH unpacking if true");
  options.add_options()("UnpPsd", bpo::value<bool>()->default_value(true), "Enable PSD unpacking if true");
  options.add_options()("IgnOverMs", bpo::value<bool>()->default_value(true), "Ignore overlap MS if true");
  options.add_options()("FullTimeSort", bpo::value<bool>()->default_value(true),
                        "Full time sorting per detector before sending output array");
  options.add_options()("SetTimeOffs", bpo::value<std::vector<std::string>>()->multitoken()->composing(),
                        "Set time offset in ns for selected detector, use string matching "
                        "ECbmModuleId,dOffs e.g. kTof,-35.2");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("ts-request"),
                        "MQ channel name for raw TS data");
  options.add_options()("TsNameOut", bpo::value<std::string>()->default_value("unpts_0"),
                        "MQ channel name for unpacked TS data");

  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(0), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceUnpack(); }
