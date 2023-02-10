/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmDeviceDigiEventSink.h"

#include <iomanip>
#include <string>

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;
using namespace std;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("StoreFullTs", bpo::value<bool>()->default_value(false),
                        "Store digis vectors with full TS in addition to selected events if true");
  options.add_options()("OutFileName", bpo::value<std::string>()->default_value("mcbm_digis_events.root"),
                        "Name (full or relative path) of the output .root file ");
  options.add_options()("EvtNameIn", bpo::value<std::string>()->default_value("events"),
                        "MQ channel name for built events");

  options.add_options()("BypassConsecutiveTs", bpo::value<bool>()->default_value(false),
                        "Do not wait for having consecutive TS in buffer before writing to file if true");
  options.add_options()("WriteMissingTs", bpo::value<bool>()->default_value(false),
                        "Write empty TS to file for the missing ones if true (irrelevant if bypass ON)");

  options.add_options()("DisableCompression", bpo::value<bool>()->default_value(false),
                        "Disable the root file compression if true");
  options.add_options()("TreeFileMaxSize", bpo::value<int64_t>()->default_value(10000000000LL),
                        "Set the maximum output tree size (~file size) in bytes");

  options.add_options()("DigiEventInput", bpo::value<bool>()->default_value(false),
                        "Enable the input of CbmDigiEvents instead of raw data + CbmEvents if true");
  options.add_options()("ExclusiveTrdExtract", bpo::value<bool>()->default_value(true),
                        "Enable loop based extraction of TRD digis to handle different 1D/2D sel windows if true");

  options.add_options()("FillHistos", bpo::value<bool>()->default_value(false),
                        "Fill histograms and send them to histo server if true");

  options.add_options()("PubFreqTs", bpo::value<uint32_t>()->default_value(100), "Histo publishing frequency in TS");
  options.add_options()("PubTimeMin", bpo::value<double_t>()->default_value(1.0),
                        "Minimal time between two publishing");
  options.add_options()("PubTimeMax", bpo::value<double_t>()->default_value(10.0),
                        "Maximal time between two publishing");
  options.add_options()("HistosSuffix", bpo::value<std::string>()->default_value(""),
                        "Suffix added to folders, histos and canvases names, e.g. for multiple nodes usages");
  options.add_options()("ChNameIn", bpo::value<std::string>()->default_value("histogram-in"),
                        "MQ channel name for histos");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceDigiEventSink(); }
