#include "CbmDeviceMonitorPsd.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("IgnOverMs", bpo::value<bool>()->default_value(true), "Ignore overlap MS if true");
  options.add_options()("MonitorMode", bpo::value<bool>()->default_value(true), "Monitor mode ON/OFF");
  options.add_options()("MonitorChanMode", bpo::value<bool>()->default_value(false), "Monitor channelwise mode ON/OFF");
  options.add_options()("MonitorWfmMode", bpo::value<bool>()->default_value(false), "Monitor waveform mode ON/OFF");
  options.add_options()("MonitorFitMode", bpo::value<bool>()->default_value(false), "Monitor fit waveform mode ON/OFF");
  options.add_options()("HistEvoSz", bpo::value<uint32_t>()->default_value(1800),
                        "Size of evolution histos in seconds");
  options.add_options()("HistChrgArgs", bpo::value<vector<int>>()->multitoken(), "Charge histos arguments");
  options.add_options()("HistAmplArgs", bpo::value<vector<int>>()->multitoken(), "Ampl histos arguments");
  options.add_options()("HistZlArgs", bpo::value<vector<int>>()->multitoken(), "ZL histos arguments");
  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("TsNameIn", bpo::value<std::string>()->default_value("psdcomponent"),
                        "MQ channel name for TS data");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
  options.add_options()("ChNameHistCfg", bpo::value<std::string>()->default_value("histo-conf"),
                        "MQ channel name for histos config");
  options.add_options()("ChNameCanvCfg", bpo::value<std::string>()->default_value("canvas-conf"),
                        "MQ channel name for canvases config");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceMonitorPsd(); }
