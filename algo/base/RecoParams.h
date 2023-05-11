/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_RECOPARAMS_H
#define CBM_ALGO_BASE_RECOPARAMS_H

#include <xpu/defines.h>

#include "Prelude.h"
#include "config/Property.h"

namespace cbm::algo
{

  /**
   * @brief RecoParams contains all parameters to configure reconstruction
   */
  struct RecoParams {
    enum SortMode : u8
    {
      SortBlockSort        = 0,
      SortCUBSegmentedSort = 1,
    };
    enum UnpackMode : u8
    {
      CPU,
      XPU,
    };

    struct STS {
      SortMode digiSortMode;
      SortMode clusterSortMode;

      UnpackMode unpackMode;
      u8 findClustersMultiKernels;

      f32 timeCutDigiAbs;
      f32 timeCutDigiSig;
      f32 timeCutClusterAbs;
      f32 timeCutClusterSig;

      struct MemoryLimits {
        u64 maxDigisPerTS;
        u64 maxDigisPerMS;
        u64 maxDigisPerModule;
        f64 clustersPerDigiTS;
        f64 clustersPerDigiModule;
        f64 hitsPerClusterTS;
        f64 hitsPerClusterModule;

        XPU_D u64 maxClustersPerTS() const { return maxDigisPerTS * clustersPerDigiTS; }
        XPU_D u64 maxClustersPerModule() const { return maxDigisPerModule * clustersPerDigiModule; }
        XPU_D u64 maxHitsPerClusterTS() const { return maxClustersPerTS() * hitsPerClusterTS; }
        XPU_D u64 maxHitsPerClusterModule() const { return maxClustersPerModule() * hitsPerClusterModule; }

        static constexpr auto Properties = std::make_tuple(
          config::Property(&MemoryLimits::maxDigisPerTS, "maxDigisPerTS", "Maximal number of digis per time slice"),
          config::Property(&MemoryLimits::maxDigisPerMS, "maxDigisPerMS", "Maximal number of digis per micro slice"),
          config::Property(&MemoryLimits::maxDigisPerModule, "maxDigisPerModule", "Maximal number of digis per module"),
          config::Property(&MemoryLimits::clustersPerDigiTS, "clustersPerDigiTS",
                           "Number of clusters per digi in a time slice"),
          config::Property(&MemoryLimits::clustersPerDigiModule, "clustersPerDigiModule",
                           "Number of clusters per digi in a module"),
          config::Property(&MemoryLimits::hitsPerClusterTS, "hitsPerClusterTS",
                           "Number of hits per cluster in a time slice"),
          config::Property(&MemoryLimits::hitsPerClusterModule, "hitsPerClusterModule",
                           "Number of hits per cluster in a module"));
      } memoryLimits;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&STS::digiSortMode, "digiSortMode",
                         "Digi sort mode (0 = block sort, 1 = cub segmented sort))"),
        config::Property(&STS::clusterSortMode, "clusterSortMode", "Cluster sort mode"),

        config::Property(&STS::unpackMode, "unpackMode", "Unpack mode (0 = legacy, 1 = XPU)"),
        config::Property(&STS::findClustersMultiKernels, "findClustersMultiKernels",
                         "Split cluster finding into multiple kernels"),

        config::Property(&STS::timeCutDigiAbs, "timeCutDigiAbs",
                         "Time delta for neighboring digis to be considered for the same cluster. [ns]"),
        config::Property(
          &STS::timeCutDigiSig, "timeCutDigiSig",
          "Used if timeCutDigiAbs is negative. Time delta must be < 'value * sqrt2 * timeResolution'. [ns]"),
        config::Property(&STS::timeCutClusterAbs, "timeCutClusterAbs",
                         "Maximal time difference between two clusters in a hit [ns]."
                         " Setting to a positive value will override timeCutClustersSig."),
        config::Property(
          &STS::timeCutClusterSig, "timeCutClusterSig",
          "Time cut for clusters."
          " Two clusters are considered it their time difference is below 'value * sqrt(terr1**2 + terr2*+2)'"),
        config::Property(&STS::memoryLimits, "memoryLimits", "Memory limits for STS reco"));
    };

    STS sts;

    static constexpr auto Properties = std::make_tuple(config::Property(&RecoParams::sts, "sts", "STS reco settings"));
  };

};  // namespace cbm::algo

#endif  // CBM_ALGO_BASE_RECOPARAMS_H
