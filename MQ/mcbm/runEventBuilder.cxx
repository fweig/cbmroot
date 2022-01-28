/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmDeviceEventBuilder.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("TriggerWin", bpo::value<double>()->default_value(0.0), "Time window for trigger algorithm");
  options.add_options()("TriggerMinDigis", bpo::value<int32_t>()->default_value(1),
                        "Minimum digi count for trigger algorithm");
  options.add_options()("TriggerDeadTime", bpo::value<double>()->default_value(0.0), "Dead time for trigger algorithm");
  options.add_options()("FillHistos", bpo::value<bool>()->default_value(true),
                        "Fill histograms and send them to histo server if true");
  options.add_options()("IgnTsOver", bpo::value<bool>()->default_value(false), "Ignore TS overlap if true");
  options.add_options()("TriggerDet", bpo::value<std::string>()->default_value("kT0"),
                        "Set the trigger detector, use string matching an ECbmModuleId ");
  options.add_options()("AddDet", bpo::value<std::vector<std::string>>()->multitoken()->composing(),
                        "Add a detector for digis selection, use string matching an ECbmModuleId ");
  options.add_options()("SetEvbuildWin", bpo::value<std::vector<std::string>>()->multitoken()->composing(),
                        "Set event builder window for selected detector, use string matching "
                        "ECbmModuleId,dWinBeg,dWinEnd e.g. kSts,-10.5,100.0");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("unpts_0"),
                        "MQ channel name for unpacked TS data");
  options.add_options()("EvtNameOut", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for built events");
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

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceEventBuilder(); }
