#include "CbmDeviceStsHitProducerIdeal.h"

#include <iostream>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("max-events", bpo::value<uint64_t>()->default_value(0),
                        "Maximum number of events to process (0 - infinite)")(
    "vmcworkdir", bpo::value<std::string>()->default_value("."), "Directory where to find needed input data")(
    "run-id", bpo::value<std::string>()->default_value("0"),
    "Rund ID which is needed to retrieve the proper parameters from Parameter "
    "Manager");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceStsHitProducerIdeal(); }
