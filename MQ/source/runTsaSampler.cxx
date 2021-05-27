#include "CbmMQTsaSampler.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("filename", bpo::value<std::string>()->default_value(""), "Filename of the input file")(
    "dirname", bpo::value<std::string>()->default_value(""), "Directory name where to find the input files")(
    "flib-host", bpo::value<std::string>()->default_value(""), "Host where the timeslice server is running")(
    "max-timeslices", bpo::value<uint64_t>()->default_value(0),
    "Maximum number of timeslices to process for Run/ConditionalRun/OnData (0 "
    "- infinite)")("flib-port", bpo::value<uint64_t>()->default_value(0), "Port where the timeslice server is running");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmMQTsaSampler(); }
