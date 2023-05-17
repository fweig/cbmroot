/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_OPTIONS_H
#define CBM_ALGO_BASE_OPTIONS_H

#include <string>

#include "compat/Filesystem.h"

namespace cbm::algo
{

  class Options {

  public:
    Options() = default;
    Options(int argc, char** argv);

    fs::path ParamsDir() const { return fParamsDir; }
    const std::string& InputLocator() const { return fInputLocator; }
    const std::string& LogLevel() const { return fLogLevel; }
    const std::string& Device() const { return fDevice; }
    bool CollectKernelTimes() const { return fCollectKernelTimes; }
    int NumTimeslices() const { return fNumTimeslices; }
    int SkipTimeslices() const { return fSkipTimeslices; }

  private:
    std::string fParamsDir;  // TODO: can we make this a std::path?
    std::string fInputLocator;
    std::string fLogLevel;
    std::string fDevice;
    bool fCollectKernelTimes = false;
    int fNumTimeslices       = -1;
    int fSkipTimeslices      = 0;
  };

}  // namespace cbm::algo

#endif
