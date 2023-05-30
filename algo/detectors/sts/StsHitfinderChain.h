/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#ifndef CBM_ALGO_STS_HITFINDER_CHAIN_H
#define CBM_ALGO_STS_HITFINDER_CHAIN_H

#include "CbmStsDigi.h"

#include <array>
#include <cstdint>
#include <gsl/span>
#include <map>
#include <optional>
#include <vector>

#include "StsHitfinder.h"
#include "SubChain.h"
#include "gpu/xpu_legacy.h"
#include "sts/HitfinderPars.h"
#include "sts/LandauTable.h"

namespace cbm::algo::sts
{

  struct HitfinderTimes {
    double timeIO          = 0;
    double timeSortDigi    = 0;
    double timeCluster     = 0;
    double timeSortCluster = 0;
    double timeHits        = 0;
  };

  /**
   * Sts Hitfinder Chain. This class is responsible for all memory allocations
   * on the GPU required by the hitfinder. It executes the hitfinder kernels and
   * handles memory transfer for input / output data and conversion to the
   * regular CBM STS types.
   */
  class HitfinderChain : public SubChain {

  public:
    void SetParameters(const sts::HitfinderPars& parameters);
    const sts::HitfinderPars& GetParameters() const { return *fPars; }

    /**
     * Teardown chain.
     */
    void Finalize();

    void operator()(gsl::span<const CbmStsDigi>);

  private:
    // Shorthands to map module-address onto digis in that module
    using DigiMapSide = std::map<int, std::vector<CbmStsDigi>>;
    struct DigiMap {
      DigiMapSide front;
      DigiMapSide back;
    };

    /**
       *  Factors used to estimate buffer sizes based on number of digis.
       *  TODO: These values might be wildly off. Look for better estimations.
       *  TODO: Should be configurable at runtime.
       */
    static constexpr float kClustersPerDigiFactor = 1.f;
    static constexpr float kHitsPerClustersFactor = 1.5f;

    /**
       * Ensure parameters were set. Raises log(fatal) otherwise.
       */
    void EnsureParameters();

    /**
       * Allocate memory that doesn't depend on input
       * Data stays on GPU for the entire duration the hitfinder is alive
       */
    void AllocateStatic();

    /**
       * Allocate memory that depends on input
       * Data stays on GPU until next timeslice is processed
       */
    void AllocateDynamic(size_t, size_t);

    /**
       * Sort Digis into buckets by module.
       */
    DigiMap SortDigisIntoModules(gsl::span<const CbmStsDigi> digis, size_t& maxNDigisPerModule);

    /**
       * Copy Digis into flat array that can be copied to the GPU.
       */
    void FlattenDigis(DigiMap& digiMap);
    void FlattenDigisSide(DigiMapSide& digis, bool isFront);

    /**
       * Transfer Hits / Clusters back to host and convert to common CBM types.
       */
    void AppendClustersModule(int module, bool isFront, std::vector<sts::Cluster>&);
    void AppendHitsModule(int module, std::vector<sts::Hit>&);

    // Debug functions, ensure reco produces sane results
    void EnsureDigiOffsets(DigiMap&);
    void EnsureDigisSorted();
    void EnsureChannelOffsets(span<u32>);
    void EnsureClustersSane(span<GpuClusterIdx>, span<int>);
    void EnsureClustersSorted();

    std::optional<const sts::HitfinderPars> fPars;

    Hitfinder fHitfinder;
  };

}  // namespace cbm::algo::sts

#endif  // #ifndef CBM_ALGO_STS_HITFINDER_CHAIN_H
