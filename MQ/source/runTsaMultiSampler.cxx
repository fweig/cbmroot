#include "runFairMQDevice.h"
#include "CbmMQTsaMultiSampler.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
    options.add_options()
        ("filename", bpo::value<std::string>()->default_value(""), "Filename of the input file")
        ("dirname", bpo::value<std::string>()->default_value(""), "Directory name where to find the input files")
        ("flib-host", bpo::value<std::string>()->default_value(""), "Host where the timeslice server is running")
        ("max-timeslices", bpo::value<uint64_t>()->default_value(0), "Maximum number of timeslices to process for Run/ConditionalRun/OnData (0 - infinite)")
        ("high-water-mark", bpo::value<uint64_t>()->default_value(1), "High water mark for ZeroMQ")
        ("no-split-ts", bpo::value<bool>()->default_value(0), "Send a copy of the full TS to all enabled channels")
        ("send-ts-per-sysid", bpo::value<bool>()->default_value(0), "Send a single TS per SysId with all matching components")
        ("send-ts-per-channel", bpo::value<bool>()->default_value(0), "Send a single TS per channel with all matching components")
        ("sysid-chan", bpo::value< std::vector< std::string > >(), "Pair a SysId in hex + channel name, separated by :, unique SysId!")
        ("flib-port", bpo::value<uint64_t>()->default_value(0), "Port where the timeslice server is running");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
    return new CbmMQTsaMultiSampler();
}
