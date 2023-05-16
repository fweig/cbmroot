/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#ifndef CBM_ALGO_DETECTORS_STS_UNPACKCHAIN_H
#define CBM_ALGO_DETECTORS_STS_UNPACKCHAIN_H

#include <optional>
#include <unordered_map>
#include <vector>

#include <xpu/defines.h>
#include <yaml-cpp/yaml.h>

#include "Prelude.h"
#include "StsReadoutConfig.h"
#include "SubChain.h"
#include "UnpackSts.h"

class CbmStsDigi;
namespace fles
{
  class Timeslice;
}

namespace cbm::algo::sts
{

  class UnpackChain : public SubChain {

  public:
    void Init(StsReadoutConfig config);
    std::vector<CbmStsDigi> Run(const fles::Timeslice& ts);

  private:
    std::unordered_map<u16, UnpackSts> fAlgoSts;
    std::optional<StsReadoutConfig> fConfig;
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_DETECTORS_STS_UNPACKCHAIN_H
