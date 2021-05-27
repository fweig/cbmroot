#include "CbmDeviceTriggerHandlerEtof.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("SandboxMode", bpo::value<bool>()->default_value(1), "Test mode switch");
  ;
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceTriggerHandlerEtof(); }
