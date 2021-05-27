/** @file runUnpackTofStar2018.cxx
  * @copyright Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmDeviceUnpackTofStar2018.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/) {}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceUnpackTofStar2018(); }
