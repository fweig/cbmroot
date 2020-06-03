#include "runFairMQDevice.h"
#include "CbmTsaComponentSink.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/)
{
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
    return new CbmTsaComponentSink();
}
