/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#ifndef APP_CBMRECO_PROGRAMOPTIONS_H
#define APP_CBMRECO_PROGRAMOPTIONS_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

/** @class ProgramOptionsException
 ** @brief Program options exception class
 ** @author Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>
 ** @since 16 March 2022
 **/
class ProgramOptionsException : public std::runtime_error {
public:
  explicit ProgramOptionsException(const std::string& what_arg = "") : std::runtime_error(what_arg) {}
};

/** @class ProgramOptions
 ** @brief Program options class for the "cbmreco_fairrun" application
 ** @author Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>
 ** @since 16 March 2022
 **/
class ProgramOptions {
public:
  /** @brief Standard constructor with command line arguments **/
  ProgramOptions(int argc, char* argv[]) { ParseOptions(argc, argv); }

  /** @brief Copy constructor forbidden **/
  ProgramOptions(const ProgramOptions&) = delete;

  /** @brief Assignment operator forbidden **/
  void operator=(const ProgramOptions&) = delete;

  /** @brief Get URI specifying the monitoring server interface **/
  [[nodiscard]] const std::string& MonitorUri() const { return fMonitorUri; }

  /** @brief Get URI specifying input timeslice stream source(s) **/
  [[nodiscard]] const std::vector<std::string>& InputUri() const { return fInputUri; }

  /** @brief Get output file name (.root format) **/
  [[nodiscard]] const std::string& OutputRootFile() const { return fOutputRootFile; }

  /** @brief Get configuration file name (.yaml format) **/
  [[nodiscard]] const std::string& ConfigYamlFile() const { return fConfigYamlFile; }

  /** @brief Get save configuration file name (.yaml format) **/
  [[nodiscard]] const std::string& SaveConfigYamlFile() const { return fSaveConfigYamlFile; }

  /** @brief Get maximum number of timeslices to process **/
  [[nodiscard]] int32_t MaxNumTs() const { return fMaxNumTs; }

  /** @brief Get the port number for the HTTP server **/
  [[nodiscard]] uint16_t HttpServerPort() const { return fHttpServerPort; }

private:
  /** @brief Parse command line arguments using boost program_options **/
  void ParseOptions(int argc, char* argv[]);

  std::string fMonitorUri;  ///< URI specifying the monitoring server interface

  std::vector<std::string> fInputUri;         ///< URI(s) specifying input timeslice stream source(s)
  std::string fOutputRootFile = "/dev/null";  ///< Output file name (.root format)
  std::string fConfigYamlFile;                ///< Configuration file name (.yaml format)
  std::string fSaveConfigYamlFile;            ///< Save configuration file name (.yaml format)
  int32_t fMaxNumTs        = INT32_MAX;       ///< Maximum number of timeslices to process
  uint16_t fHttpServerPort = 0;               ///< Port number for the HTTP server. If 0, server will not be activated.
};

#endif /* APP_CBMRECO_PROGRAMOPTIONS_H */
