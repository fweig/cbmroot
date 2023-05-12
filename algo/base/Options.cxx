/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Options.h"

#include <boost/program_options.hpp>

#include <iostream>

using namespace cbm::algo;

Options::Options(int argc, char** argv)
{
  namespace po = boost::program_options;

  po::options_description required("Required options");
  // clang-format off
  required.add_options()
    ("param-dir,p", po::value<std::string>(&fParamsDir)->value_name("<folder>")->required(),
      "read program options from this folder")
    ("input-locator,i", po::value<std::string>(&fInputLocator)->value_name("<locator>")->required(),
      "URI specifying input timeslice source")
  ;
  // clang-format on

  po::options_description generic("Other options");
  // clang-format off
  generic.add_options()
    ("device,d", po::value<std::string>(&fDevice)->default_value("cpu")->value_name("<device>"),
      "select device (cpu, cuda0, cuda1, hip0, ...)")
    ("log-level,l", po::value<std::string>(&fLogLevel)->default_value("info")->value_name("<level>"),
      "set log level (debug, info, warning, error, fatal)")
    ("num-ts,n", po::value<int>(&fNumTimeslices)->default_value(-1)->value_name("<num>"),
      "Stop after <num> timeslices (-1 = all)")
    ("skip-ts,s", po::value<int>(&fSkipTimeslices)->default_value(0)->value_name("<num>"),
      "Skip first <num> timeslices")
    ("times,t", po::bool_switch(&fCollectKernelTimes)->default_value(false),
      "print kernel times")("help,h", "produce help message")
  ;
  // clang-format on

  po::options_description cmdline_options;
  cmdline_options.add(required).add(generic);

  po::variables_map vm;
  po::command_line_parser parser {argc, argv};
  parser.options(cmdline_options);
  po::store(parser.run(), vm);

  if (vm.count("help")) {
    std::cerr << cmdline_options << std::endl;
    std::exit(0);
  }

  try {
    po::notify(vm);
  }
  catch (const po::required_option& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << cmdline_options << std::endl;
    std::exit(1);
  }
}
