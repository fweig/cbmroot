/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#ifndef CBMGPURECOSTS_H
#define CBMGPURECOSTS_H

#include "CbmStsDigi.h"
#include "CbmStsGpuHitFinder.h"
#include "CbmStsParModule.h"

#include <map>
#include <optional>
#include <vector>

#include <xpu/host.h>

class CbmStsPhysics;
class CbmStsRecoModule;
class TClonesArray;

namespace experimental
{

  class CbmGpuRecoSts {

  public:
    struct Config {
      std::vector<CbmStsParModule> parModules;
      std::vector<CbmStsRecoModule*> recoModules;
      std::vector<int> moduleAddrs;
      std::vector<CbmStsHitFinderConfig> hitfinderCfg;
      CbmStsPhysics* physics;
    };

    double timeIO          = 0;
    double timeSortDigi    = 0;
    double timeCluster     = 0;
    double timeSortCluster = 0;
    double timeHits        = 0;

    size_t nDigis     = 0;
    size_t nDigisUsed = 0;
    size_t nHits      = 0;
    size_t nCluster   = 0;

    void SetConfig(const Config& cfg) { fConfig = cfg; }

    void SetupConstants();
    void Setup(size_t maxDigisPerModule, size_t nDigitsTotal);

    void RunHitFinder();

    void ForwardClustersAndHits(TClonesArray* clusters, TClonesArray* hits);

    std::vector<CbmStsHit> GetHits(int module)
    {
      auto& hfc = fHitfinderCpu;
      auto* st  = &hfc.hitsPerModule.h()[module * hfc.maxHitsPerModule];
      auto* end = st + hfc.nHitsPerModule.h()[module];
      return std::vector<CbmStsHit> {st, end};
    }

  private:
    std::optional<Config> fConfig;  // Initialized late...

    // std::vector<int> fModuleAddrs;
    CbmStsHitFinder<xpu::side::host> fHitfinderCpu;
    CbmStsGpuHitFinder fHitfinderGpu;

    std::map<int, std::vector<CbmStsDigi>> fDigisByModuleF;
    std::map<int, std::vector<CbmStsDigi>> fDigisByModuleB;

    void FetchDigis(size_t& maxDigisPerModule, size_t& nDigisTotal);
  };

}  // namespace experimental

#endif
