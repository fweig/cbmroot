/* Copyright (C) 2019 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

#include "CbmDeviceEventBuilderEtofStar2019.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options) { ; }

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceEventBuilderEtofStar2019(); }
