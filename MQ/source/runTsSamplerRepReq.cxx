/** @file runTsSamplerRepReq.cxx
  * @copyright Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

#include "CbmMQTsSamplerRepReq.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("filename", bpo::value<std::string>()->default_value(""), "Filename of the input file");
  options.add_options()("dirname", bpo::value<std::string>()->default_value(""),
                        "Directory name where to find the input files");
  options.add_options()("fles-host", bpo::value<std::string>()->default_value(""),
                        "Host where the timeslice server is running");
  options.add_options()("fles-port", bpo::value<uint16_t>()->default_value(0),
                        "Port where the timeslice server is running");

  options.add_options()("max-timeslices", bpo::value<uint64_t>()->default_value(0),
                        "Maximum number of timeslices to process for Run/ConditionalRun/OnData (0 - infinite)");
  options.add_options()("high-water-mark", bpo::value<uint64_t>()->default_value(1), "High water mark for ZeroMQ");

  options.add_options()("ChNameTsReq", bpo::value<std::string>()->default_value("ts-request"),
                        "MQ channel name for TS requests");
  options.add_options()("no-split-ts", bpo::value<bool>()->default_value(0),
                        "Send a copy of the full TS to single consummer");
  options.add_options()("send-ts-per-sysid", bpo::value<bool>()->default_value(0),
                        "Send a single TS upon request of a SysId with all matching components");
  options.add_options()("send-ts-per-block", bpo::value<bool>()->default_value(0),
                        "Send a single TS upon request of a block name with all matching components");
  options.add_options()("block-sysid", bpo::value<std::vector<std::string>>(),
                        "Pair a block name and SysId in hex, separated by :, unique use of SysId for all blocks!");

  options.add_options()("ChNameMissTs", bpo::value<std::string>()->default_value(""),
                        "MQ channel name for missed TS indices");
  options.add_options()("ChNameCmds", bpo::value<std::string>()->default_value(""),
                        "MQ channel name for commands to slaves");

  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(0), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmMQTsSamplerRepReq(); }
