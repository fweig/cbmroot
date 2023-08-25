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
#include "SubChain.h"
#include "UnpackSts.h"
#include "sts/ReadoutSetup.h"

class CbmStsDigi;
namespace fles
{
  class Timeslice;
}

namespace cbm::algo::sts
{

  class UnpackChain : public SubChain {

  public:
    void Init(sts::ReadoutMapping config);
    std::vector<CbmStsDigi> Run(const fles::Timeslice& ts);

  private:
    std::unordered_map<u16, UnpackSts> fAlgoSts;
    std::optional<sts::ReadoutMapping> fConfig;
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_DETECTORS_STS_UNPACKCHAIN_H
