#include "CbmMCPointSource.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("filename", bpo::value<std::string>()->default_value(""), "Filename of the input file")(
    "max-events", bpo::value<uint64_t>()->default_value(0),
    "Maximum number of timeslices to process for Run/ConditionalRun/OnData (0 "
    "- infinite)");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmMCPointSource(); }
