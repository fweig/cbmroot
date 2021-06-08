/* Copyright (C) 2018-2019 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [orginator] */

#include "CbmDeviceHitBuilderTof.h"

#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options)
{
  options.add_options()("OutRootFile", bpo::value<std::string>()->default_value(""), "Root output file name");
  options.add_options()("MaxEvent", bpo::value<int64_t>()->default_value(-1), "Nr of Event for ROOT file");
  options.add_options()("RunId", bpo::value<int64_t>()->default_value(11), "Run identifier for ROOT geometry file");
  options.add_options()("Mode", bpo::value<int64_t>()->default_value(0), "Choose device operation mode");
  options.add_options()("PulserMode", bpo::value<int64_t>()->default_value(0), "Choose pulser configuration");
  options.add_options()("PulMulMin", bpo::value<uint64_t>()->default_value(0), "Min number of pulsed detectors");
  options.add_options()("PulDetRef", bpo::value<uint64_t>()->default_value(0), "Pulser Reference Detector");
  options.add_options()("PulTotMin", bpo::value<uint64_t>()->default_value(0), "Min Tot value");
  options.add_options()("PulTotMax", bpo::value<uint64_t>()->default_value(0), "Max Tot value");

  options.add_options()("DutType", bpo::value<uint64_t>()->default_value(0), "Dut type identifier");
  options.add_options()("DutSm", bpo::value<uint64_t>()->default_value(0), "Dut module number");
  options.add_options()("DutRpc", bpo::value<uint64_t>()->default_value(1), "Dut counter number");

  options.add_options()("SelType", bpo::value<uint64_t>()->default_value(0), "Sel type identifier");
  options.add_options()("SelSm", bpo::value<uint64_t>()->default_value(1), "Sel module number");
  options.add_options()("SelRpc", bpo::value<uint64_t>()->default_value(1), "Sel counter number");

  options.add_options()("Sel2Type", bpo::value<uint64_t>()->default_value(0), "Sel2 type identifier");
  options.add_options()("Sel2Sm", bpo::value<uint64_t>()->default_value(2), "Sel2 module number");
  options.add_options()("Sel2Rpc", bpo::value<uint64_t>()->default_value(1), "Sel2 counter number");

  options.add_options()("BRefType", bpo::value<uint64_t>()->default_value(0), "Beam Reference type identifier");
  options.add_options()("BRefSm", bpo::value<uint64_t>()->default_value(3), "Beam Reference module number");
  options.add_options()("BRefDet", bpo::value<uint64_t>()->default_value(1), "Beam Reference counter number");

  options.add_options()("CalMode", bpo::value<uint64_t>()->default_value(0), "Calibration Mode");
  options.add_options()("CalSel", bpo::value<uint64_t>()->default_value(0), "Calibration Selector");
  options.add_options()("CaldXdYMax", bpo::value<double_t>()->default_value(30.), "Matching Window");
  options.add_options()("CalCluMulMax", bpo::value<uint64_t>()->default_value(3), "Max Counter Multiplicity");
  options.add_options()("CalRefSel", bpo::value<uint64_t>()->default_value(0), "Reference Selector");
  options.add_options()("CalTotMax", bpo::value<double_t>()->default_value(20.), "Time Over Threhold Maximum");
  options.add_options()("CalTotMean", bpo::value<double_t>()->default_value(5.), "Time Over Threhold Mean");
  options.add_options()("MaxTimeDist", bpo::value<double_t>()->default_value(1.), "Cluster range in ns");
  options.add_options()("DelTofMax", bpo::value<double_t>()->default_value(60.),
                        "acceptance range for cluster distance");
  options.add_options()("Sel2MulMax", bpo::value<double_t>()->default_value(3.), "Limit of 2nd selector multiplicity");
  options.add_options()("ChannelDeadtime", bpo::value<double_t>()->default_value(50.), "channel deadtime in ns");
  options.add_options()("CalYFitMin", bpo::value<double_t>()->default_value(1.E4), "Min counts for box fit");

  options.add_options()("OutHstFile", bpo::value<std::string>()->default_value(""), "monitor histogram file name");
  options.add_options()("OutParFile", bpo::value<std::string>()->default_value(""), "parameter histogram file name");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/) { return new CbmDeviceHitBuilderTof(); }
