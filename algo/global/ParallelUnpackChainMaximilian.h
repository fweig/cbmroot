/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#ifndef CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H
#define CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H

#include <optional>
#include <unordered_map>
#include <vector>

#include <xpu/defines.h>
#include <yaml-cpp/yaml.h>

#include "Prelude.h"
#include "SubChain.h"
#include "UnpackSts.h"
#include "sts/ReadoutSetup.h"
#include "util/NoInitAlloc.h"

class CbmStsDigi;
namespace fles
{
  class Timeslice;
}

namespace cbm::algo
{

  class ParallelUnpackChain : public SubChain {

  public:
    void Init(sts::ReadoutMapping config);
    std::vector<CbmStsDigi> Run(const fles::Timeslice& ts);

  private:
    std::unordered_map<u16, UnpackSts> fAlgoSts;
    std::optional<sts::ReadoutMapping> fConfig;

  };

}  // namespace cbm::algo

#endif  //CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H