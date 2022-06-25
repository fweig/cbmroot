/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "CbmDevBuildEvents.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("OutFileName", bpo::value<std::string>()->default_value(""),
                        "Name (full or relative path) of the output .root file ");
  options.add_options()("SetEvbuildWin", bpo::value<std::vector<std::string>>()->multitoken()->composing(),
                        "Set event builder window for selected detector, use string matching "
                        "ECbmModuleId,dWinBeg,dWinEnd e.g. kSts,-10.5,100.0");
  options.add_options()("TrigNameIn", bpo::value<std::string>()->default_value("trigger"),
                        "MQ channel name for TS data and triggers");
  options.add_options()("EvtNameOut", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for built events");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDevBuildEvents(); }
