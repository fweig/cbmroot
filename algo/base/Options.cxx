/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Options.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <iterator>

using namespace cbm::algo;
namespace po = boost::program_options;


namespace std
{
  template<class T>
  std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
  {
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
  }
}  // namespace std


void validate(boost::any& v, const std::vector<std::string>& values, severity_level*, int)
{

  static const std::unordered_map<std::string, severity_level> levels {
    {"trace", severity_level::trace}, {"debug", severity_level::debug},     {"status", severity_level::status},
    {"info", severity_level::info},   {"warning", severity_level::warning}, {"error", severity_level::error},
    {"fatal", severity_level::fatal}};

  po::validators::check_first_occurrence(v);

  const std::string& s = po::validators::get_single_string(values);

  auto it = levels.find(s);

  if (it == levels.end()) throw po::validation_error(po::validation_error::invalid_option_value);

  v = it->second;
}

Options::Options(int argc, char** argv)
{

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
    ("log-level,l", po::value(&fLogLevel)->default_value(info)->value_name("<level>"),
      "set log level (debug, info, warning, error, fatal)")
    ("output,o", po::value<std::vector<RecoData>>(&fOutputTypes)->multitoken()->default_value({RecoData::Hit})->value_name("<types>"),
      "comma seperated list of reconstruction output types (hit, digi, ...)")
    ("steps", po::value<std::vector<Step>>(&fRecoSteps)->multitoken()->default_value({Step::Unpack, Step::LocalReco})->value_name("<steps>"),
      "comma seperated list of reconstruction steps (upack, digitrigger, localreco, ...)")
    ("systems,s", po::value(&fDetectors)->multitoken()->default_value({Detector::STS})->value_name("<detectors>"),
      "comma seperated list of detectors to process (sts, mvd, ...)")
    ("num-ts,n", po::value<int>(&fNumTimeslices)->default_value(-1)->value_name("<num>"),
      "Stop after <num> timeslices (-1 = all)")
    ("skip-ts", po::value<int>(&fSkipTimeslices)->default_value(0)->value_name("<num>"),
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
