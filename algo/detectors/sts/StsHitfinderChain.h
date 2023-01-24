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

#include <xpu/host.h>

#include "StsHitfinder.h"

namespace cbm::algo
{

  struct StsModuleTransformationMatrix {
    // Rotation + translation matrix to transform
    // local module coordinates into global coordinate system.
    // No need for fancy math types here. These values are just copied
    // and moved to the GPU.
    std::array<float, 9> rotation;  // 3x3 matrix
    std::array<float, 3> translation;
  };

  struct StsModulePar {
    int32_t address;
    float dY;
    float pitch;
    float stereoF;
    float stereoB;
    float lorentzF;
    float lorentzB;
    StsModuleTransformationMatrix localToGlobal;
  };

  struct StsLandauTable {
    std::vector<float> values;
    float stepSize;
  };

  struct StsHitfinderPar {
    StsAsicPar asic;  // Asic definitions. Currently assumes same parameters for all asics.
    int nChannels;    // Total number of channels per module. Hitfinder assumes nChannels / 2 channels per side.
    std::vector<StsModulePar> modules;
    StsLandauTable landauTable;
  };

  struct StsHitfinderTimes {
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
  class StsHitfinderChain {

  public:
    void SetParameters(const StsHitfinderPar& parameters);
    const StsHitfinderPar& GetParameters() const { return *fPars; }

    void operator()(gsl::span<const CbmStsDigi>);

    StsHitfinderTimes GetHitfinderTimes() const { return fHitfinderTimes; }

    /**
       * Returns access to (host copies of) the raw buffers used by the gpu hitfinder.
       * This is currently the only way to access the produced hits and cluster.
       */
    const StsHitfinderHost& GetHitfinderBuffers() const { return fHitfinderHost; }

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
    static constexpr float kClustersPerDigiFactor = 0.5f;
    static constexpr float kHitsPerClustersFactor = 4.f;

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

    void CollectTimingInfo();

    /**
       * Transfer Hits / Clusters back to host and convert to common CBM types.
       */
    void AppendClustersModule(int module, bool isFront, std::vector<StsGpuCluster>&);
    void AppendHitsModule(int module, std::vector<StsGpuHit>&);

    std::optional<const StsHitfinderPar> fPars;
    StsHitfinderTimes fHitfinderTimes;

    StsHitfinderHost fHitfinderHost;
    StsHitfinderGpu fHitfinderGpu;
  };

}  // namespace cbm::algo

#endif  // #ifndef CBM_ALGO_STS_HITFINDER_CHAIN_H
