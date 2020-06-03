#include "runFairMQDevice.h"
#include "CbmDeviceUnpackTofStar2018.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/)
{
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
    return new CbmDeviceUnpackTofStar2018();
}
