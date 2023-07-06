/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Sebastian Heinemann */
#ifndef CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H
#define CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H

#include <optional>
#include <unordered_map>
#include <vector>

#include <xpu/host.h>
#include <xpu/defines.h>
#include <yaml-cpp/yaml.h>

#include "MsUnpacker.h"  // STS
#include "Prelude.h"
#include "SubChain.h"
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
    sts::MsUnpacker fStsUnpacker;
    std::optional<sts::ReadoutMapping> fConfig;
    std::unordered_map<u16, u16> eqIdToIndex;  // Map equiptment id to linear index [0, nComponents)


    size_t fNumTs    = 0;
    size_t fNumMs    = 0;
    size_t fNumBytes = 0;
    size_t fNumDigis = 0;
    size_t fSumDigis = 0;
    size_t fVecDigis = 0;
    double fTime     = 0.;
    double fSortTime = 0.;

    static constexpr u64 fEpochLength = stsxyter::kuHitNbTsBinsBinning;
    static constexpr u32 fClockCycleNom = stsxyter::kulClockCycleNom;
    static constexpr u32 fClockCycleDen = stsxyter::kulClockCycleDen;
    static constexpr u64 fEpochLengthInNs = fEpochLength * fClockCycleNom / fClockCycleDen;

    std::vector<sts::UnpackPar> fUnpackPar;
    std::vector<sts::UnpackElinkPar> fUnpackElinkPar;
    size_t fnComponents;
    size_t fnumAllElinks;
  };

}  // namespace cbm::algo

#endif  //CBM_ALGO_GLOBAL_PARALLEL_UNPACK_CHAIN_H
