/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#include "ProgramOptions.h"

#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

#include "log.hpp"

namespace po = boost::program_options;

void ProgramOptions::ParseOptions(int argc, char* argv[])
{
  unsigned log_level  = 2;
  unsigned log_syslog = 2;
  std::string log_file;
  std::string options_file;

  // --- Define generic options
  po::options_description generic("Generic options");
  auto generic_add = generic.add_options();
  generic_add("options-file,f", po::value<std::string>(&options_file)->value_name("<filename>"),
              "read program options from file");
  generic_add("log-level,l", po::value<unsigned>(&log_level)->default_value(log_level)->value_name("<n>"),
              "set the file log level (all:0)");
  generic_add("log-file,L", po::value<std::string>(&log_file)->value_name("<filename>"), "write log output to file");
  generic_add("log-syslog,S", po::value<unsigned>(&log_syslog)->implicit_value(log_syslog)->value_name("<n>"),
              "enable logging to syslog at given log level");
  generic_add("help,h", "display this help and exit");
  generic_add("version,V", "output version information and exit");

  // --- Define configuration options
  po::options_description config("Configuration");
  auto config_add = config.add_options();
  config_add(
    "input,i",
    po::value<std::vector<std::string>>()->multitoken()->value_name("scheme://host/path?param=value ... | <filename>"),
    "uri of a timeslice source");
  config_add("output-root,o",
             po::value<std::string>(&fOutputRootFile)->default_value(fOutputRootFile)->value_name("<filename>"),
             "name of an output root file to write");
  config_add("config,c", po::value<std::string>(&fConfigYamlFile)->value_name("<filename>"),
             "name of a yaml config file containing the reconstruction chain configuration");
  config_add("save-config", po::value<std::string>(&fSaveConfigYamlFile)->value_name("<filename>"),
             "save configuration to yaml file (mostly for debugging)");
  config_add("max-timeslice-number,n", po::value<int32_t>(&fMaxNumTs)->value_name("<n>"),
             "quit after processing given number of timeslices (default: unlimited)");
  config_add("http-server-port,p", po::value<uint16_t>(&fHttpServerPort)->value_name("<n>"),
             "port number for the HTTP server. If 0, server will not be activated (default)");

  po::options_description cmdline_options("Allowed options");
  cmdline_options.add(generic).add(config);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
  po::notify(vm);

  // --- Read program options from file if requested
  if (!options_file.empty()) {
    std::ifstream ifs(options_file.c_str());
    if (!ifs) { throw ProgramOptionsException("cannot open options file: " + options_file); }
    po::store(po::parse_config_file(ifs, config), vm);
    notify(vm);
  }

  if (vm.count("help") != 0u) {
    std::cout << cmdline_options << std::endl;
    exit(EXIT_SUCCESS);
  }

  if (vm.count("version") != 0u) {
    std::cout << "cbmreco, version (unspecified)" << std::endl;
    exit(EXIT_SUCCESS);
  }

  // --- Set up logging
  logging::add_console(static_cast<severity_level>(log_level));
  if (vm.count("log-file") != 0u) {
    L_(info) << "Logging output to " << log_file;
    logging::add_file(log_file, static_cast<severity_level>(log_level));
  }
  if (vm.count("log-syslog") != 0u) {
    logging::add_syslog(logging::syslog::local0, static_cast<severity_level>(log_syslog));
  }

  if (vm.count("input") == 0u) { throw ProgramOptionsException("no input source specified"); }
  fInputUri = vm["input"].as<std::vector<std::string>>();

  if (vm.count("config") == 0u) { throw ProgramOptionsException("no configuration file specified"); }

  L_(debug) << "input sources (" << fInputUri.size() << "):";
  for (auto& inputUri : fInputUri) {
    L_(debug) << "  " << inputUri;
  }
}
