/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer] */

#include "CbmDeviceMonitorT0.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("IgnOverMs", bpo::value<bool>()->default_value(true), "Ignore overlap MS if true");
  options.add_options()("HistEvoSz", bpo::value<uint32_t>()->default_value(1800),
                        "Size of evolution histos in seconds");
  options.add_options()("PulsTotMin", bpo::value<uint32_t>()->default_value(185), "Minimal TOT for pulser cut");
  options.add_options()("PulsTotMax", bpo::value<uint32_t>()->default_value(195), "Maximal TOT for pulser cut");
  options.add_options()("SpillThr", bpo::value<uint32_t>()->default_value(25), "Hits Nb Thr for spill detection");
  options.add_options()("SpillThrNonPuls", bpo::value<uint32_t>()->default_value(10),
                        "Non pulser Hits Nb Thr for spill detection");
  options.add_options()("SpillCheckInt", bpo::value<double>()->default_value(0.128),
                        "Interval in seconds between count checks for spill detection");
  options.add_options()("ChanMap", bpo::value<std::string>()->default_value("0,1,2,3,4,5,6,7"),
                        "Set T0 channel map e.g. 0,1,2,3,4,5,6,7");
  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("t0component"),
                        "MQ channel name for TS data");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceMonitorT0(); }
