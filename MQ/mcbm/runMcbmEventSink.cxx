/** @file runMcbmEventSink.cxx
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

#include "CbmDeviceMcbmEventSink.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("OutFileName", bpo::value<std::string>()->default_value("mcbm_digis_events.root"),
                        "Name (full or relative path) of the output .root file ");
  options.add_options()("EvtNameIn", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for built events");
  options.add_options()("FillHistos", bpo::value<bool>()->default_value(false),
                        "Fill histograms and send them to histo server if true");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
  options.add_options()("ChNameHistCfg", bpo::value<std::string>()->default_value("histo-conf"),
                        "MQ channel name for histos config");
  options.add_options()("ChNameCanvCfg", bpo::value<std::string>()->default_value("canvas-conf"),
                        "MQ channel name for canvases config");
  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceMcbmEventSink(); }
