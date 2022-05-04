/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */

#include "CbmDevUnpack.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("ts-request"),
                        "MQ channel name for raw TS data");
  options.add_options()("TsNameOut", bpo::value<std::string>()->default_value("unpts_0"),
                        "MQ channel name for unpacked TS data");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDevUnpack(); }
