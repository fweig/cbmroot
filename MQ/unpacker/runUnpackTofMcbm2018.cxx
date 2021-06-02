/** @file runUnpackTofMcbm2018.cxx
  * @copyright Copyright (C) 2018-2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

#include "CbmDeviceUnpackTofMcbm2018.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("ReqMode", bpo::value<uint64_t>()->default_value(0), "Time intervall selector");
  options.add_options()("SelectComponents", bpo::value<uint64_t>()->default_value(1),
                        "Select components for transport");
  options.add_options()("ReqTint", bpo::value<uint64_t>()->default_value(100), "Time intervall length in ns");
  options.add_options()("ReqBeam", bpo::value<uint64_t>()->default_value(-1), "Mandatory beam counter in event");
  options.add_options()("PulserMode", bpo::value<int64_t>()->default_value(0), "Choose pulser configuration");
  options.add_options()("PulMulMin", bpo::value<uint64_t>()->default_value(0), "Min number of pulsed detectors");
  options.add_options()("PulTotMin", bpo::value<uint64_t>()->default_value(0), "Min pulser TimeOverThreshold");
  options.add_options()("PulTotMax", bpo::value<uint64_t>()->default_value(1000), "Max pulser TimeOverThreshold");
  options.add_options()("ToffTof", bpo::value<double_t>()->default_value(0.),
                        "Time offset of Tof digis with respect to T0");
  options.add_options()("RefModType", bpo::value<int64_t>()->default_value(5), "Module type of reference counter");
  options.add_options()("RefModId", bpo::value<int64_t>()->default_value(0), "Module Id of reference counter");
  options.add_options()("RefCtrType", bpo::value<int64_t>()->default_value(4), "Counter type of reference counter");
  options.add_options()("RefCtrId", bpo::value<int64_t>()->default_value(0), "Counter Id of reference counter");
  options.add_options()("MaxAsicInactive", bpo::value<uint64_t>()->default_value(0), "Max number of disabled ASICs");

  int iNDet = 36;
  for (int i = 0; i < iNDet; i++)
    options.add_options()(Form("ReqDet%d", i), bpo::value<uint64_t>()->default_value(0), Form("ReqDet%d", i));
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceUnpackTofMcbm2018(); }
