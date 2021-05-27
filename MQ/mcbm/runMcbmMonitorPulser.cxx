/** @file runMcbmMonitorPulser.cxx
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

#include "CbmDeviceMcbmMonitorPulser.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("DebugMoni", bpo::value<bool>()->default_value(false), "Debug Monitor Mode");
  options.add_options()("HistEvoSz", bpo::value<uint32_t>()->default_value(1800),
                        "Size of evolution histos in seconds");
  options.add_options()("PulsTotMin", bpo::value<uint32_t>()->default_value(185), "Minimal TOT for pulser cut");
  options.add_options()("PulsTotMax", bpo::value<uint32_t>()->default_value(195), "Maximal TOT for pulser cut");
  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("unpts_0"), "MQ channel name for TS data");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
  options.add_options()("ChNameHistCfg", bpo::value<std::string>()->default_value("histo-conf"),
                        "MQ channel name for histos config");
  options.add_options()("ChNameCanvCfg", bpo::value<std::string>()->default_value("canvas-conf"),
                        "MQ channel name for canvases config");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceMcbmMonitorPulser(); }
