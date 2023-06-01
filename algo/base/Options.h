/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_OPTIONS_H
#define CBM_ALGO_BASE_OPTIONS_H

#include "Types.h"

#include <set>
#include <string>
#include <vector>

#include "compat/Filesystem.h"
#include "log.hpp"

namespace cbm::algo
{

  class Options {

  public:
    Options() = default;
    Options(int argc, char** argv);

    fs::path ParamsDir() const { return fParamsDir; }
    const std::string& InputLocator() const { return fInputLocator; }
    severity_level LogLevel() const { return fLogLevel; }
    const std::string& Device() const { return fDevice; }
    bool CollectKernelTimes() const { return fCollectKernelTimes; }
    bool ParallelUnpacker() const { return fParallelUnpacker; }
    int NumTimeslices() const { return fNumTimeslices; }
    int SkipTimeslices() const { return fSkipTimeslices; }

    const std::vector<Step>& Steps() const { return fRecoSteps; }
    bool HasStep(Step step) const { return std::find(fRecoSteps.begin(), fRecoSteps.end(), step) != fRecoSteps.end(); }
    const std::vector<RecoData>& OutputTypes() const { return fOutputTypes; }
    bool HasOutput(RecoData recoData) const
    {
      return std::find(fOutputTypes.begin(), fOutputTypes.end(), recoData) != fOutputTypes.end();
    }

    const std::vector<Detector>& Detectors() const { return fDetectors; }
    bool HasDetector(Detector detector) const
    {
      return std::find(fDetectors.begin(), fDetectors.end(), detector) != fDetectors.end();
    }

    const std::vector<std::string>& ConfigKeys() const { return fConfigKeys; }


  private:
    std::string fParamsDir;  // TODO: can we make this a std::path?
    std::string fInputLocator;
    severity_level fLogLevel;
    std::string fDevice;
    bool fCollectKernelTimes = false;
    bool fParallelUnpacker   = false;
    int fNumTimeslices       = -1;
    int fSkipTimeslices      = 0;
    std::vector<Step> fRecoSteps;
    std::vector<RecoData> fOutputTypes;
    std::vector<Detector> fDetectors;
    std::vector<std::string> fConfigKeys;
  };

}  // namespace cbm::algo

#endif
