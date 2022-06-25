/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmDevTrigger.h"

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
  options.add_options()("TriggerDet", bpo::value<std::string>()->default_value(""),
                        "Set the trigger detector, use string matching an ECbmModuleId ");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("unpts_0"),
                        "MQ channel name for unpacked TS data");
  options.add_options()("TriggerNameOut", bpo::value<std::string>()->default_value("trigger"),
                        "MQ channel name for trigger times");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDevTrigger(); }
