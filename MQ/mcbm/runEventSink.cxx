/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmDeviceEventSink.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("StoreFullTs", bpo::value<bool>()->default_value(false),
                        "Store digis vectors with full TS in addition to selected events if true");
  options.add_options()("OutFileName", bpo::value<std::string>()->default_value("mcbm_digis_events.root"),
                        "Name (full or relative path) of the output .root file ");
  options.add_options()("EvtNameIn", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for built events");
  options.add_options()("FillHistos", bpo::value<bool>()->default_value(false),
                        "Fill histograms and send them to histo server if true");

  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceEventSink(); }