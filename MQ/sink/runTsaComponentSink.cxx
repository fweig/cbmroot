/* Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmTsaComponentSink.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/) {}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmTsaComponentSink(); }
