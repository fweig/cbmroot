/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#include "StsHitfinderChain.h"

#include "Logger.h"

using namespace cbm::algo;

// Shorthands to set data / allocate buffers on GPU
// FIXME: Eventually we'll need a better way to do this. Preferably supplied by xpu
#define SET_GPU_CONSTANT(constant, val) fHitfinderHost.constant = fHitfinderGpu.constant = val
#define GPU_ALLOC(field, nEntries)                                                                                     \
  fHitfinderHost.field = decltype(fHitfinderHost.field)(nEntries);                                                     \
  fHitfinderGpu.field  = fHitfinderHost.field.d()

void StsHitfinderChain::SetParameters(const StsHitfinderPar& parameters)
{
  fPars.emplace(parameters);
  AllocateStatic();
}

void StsHitfinderChain::operator()(gsl::span<const CbmStsDigi> digis)
{
  EnsureParameters();

  // FIXME: this has to be called only once
  // and should happen during initialization not in reco loop
  setenv("XPU_PROFILE", "1", 1);
  xpu::initialize();

  int nModules       = fPars->modules.size();
  int nModuleSides   = nModules * 2;
  size_t nDigisTotal = digis.size();

  // Getting the digis on the GPU requires 3 steps
  // 1. Sort digis into buckets by module
  size_t maxNDigisPerModule;
  DigiMap digiMap = SortDigisIntoModules(digis, maxNDigisPerModule);
  // 2. Once we know number of digis per module, we can allocate
  //    the dynamic buffers on the gpu, as the buffer sizes depend on that value
  AllocateDynamic(maxNDigisPerModule, nDigisTotal);
  // 3. Copy digis into flat array with offsets per module
  FlattenDigis(digiMap);

  // Clear buffers
  // Not all buffers have to initialized, but useful for debugging

  auto& hfc = fHitfinderHost;
  // xpu::memset(hfc.digisPerModule, 0);
  // xpu::memset(hfc.digisPerModuleTmp, 0);
  // xpu::memset(hfc.digisSortedPerModule, 0);
  // xpu::memset(hfc.digiOffsetPerModule, 0);
  xpu::memset(hfc.digiConnectorsPerModule, 0);
  // xpu::memset(hfc.channelOffsetPerModule, 0);
  xpu::memset(hfc.channelStatusPerModule, -1);
  xpu::memset(hfc.clusterIdxPerModule, 0);
  // xpu::memset(hfc.clusterIdxPerModuleTmp, 0);
  // xpu::memset(hfc.clusterIdxSortedPerModule, 0);
  // xpu::memset(hfc.clusterDataPerModule, 0);
  xpu::memset(hfc.nClustersPerModule, 0);
  // xpu::memset(hfc.hitsPerModule, 0);
  xpu::memset(hfc.nHitsPerModule, 0);
  xpu::memset(hfc.maxErrorFront, 0);
  xpu::memset(hfc.maxErrorBack, 0);

  xpu::set_constant<TheStsHitfinder>(fHitfinderGpu);
  xpu::copy(hfc.digisPerModule, xpu::host_to_device);
  xpu::copy(hfc.digiOffsetPerModule, xpu::host_to_device);

  xpu::run_kernel<SortDigis>(xpu::grid::n_blocks(nModuleSides));
  xpu::run_kernel<FindClustersSingleStep>(xpu::grid::n_blocks(nModuleSides));
  // Run cluster finding steps in indivual kernels, useful for debugging / profiling
  // xpu::run_kernel<CalculateOffsets>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClusters>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  xpu::run_kernel<SortClusters>(xpu::grid::n_blocks(nModuleSides));
  xpu::run_kernel<FindHits>(xpu::grid::n_blocks(nModules));

  CollectTimingInfo();

  // Transfer Hits / Cluster back to host
  // TODO: Hits and Clusters are stored in buckets. Currently we transfer the entire
  // bucket arrays back to the host. This includes a lot of unused bytes.
  // Better to either flatten both arrays on the GPU and transfer flat array back.
  // Or do multiple copies to copy contents of individual buckets.
  xpu::copy(hfc.clusterDataPerModule, xpu::device_to_host);
  xpu::copy(hfc.nClustersPerModule, xpu::device_to_host);
  xpu::copy(hfc.hitsPerModule, xpu::device_to_host);
  xpu::copy(hfc.nHitsPerModule, xpu::device_to_host);
}

void StsHitfinderChain::EnsureParameters()
{
  LOG_IF(fatal, fPars == std::nullopt) << "StsHitfinderChain: Parameters not set. Can't continue!";
}

void StsHitfinderChain::AllocateStatic()
{
  EnsureParameters();

  // Shorthands for common constants
  const int nChannels = fPars->nChannels / 2;  // Only count channels on one side of the module
  const int nModules  = fPars->modules.size();
  // const int nModuleSides = 2 * nModules; // Number of module front + backsides

  // Set GPU constants
  SET_GPU_CONSTANT(nModules, nModules);
  SET_GPU_CONSTANT(nChannels, nChannels);

  SET_GPU_CONSTANT(timeCutDigiAbs, -1.f);
  SET_GPU_CONSTANT(timeCutDigiSig, 3.f);
  SET_GPU_CONSTANT(timeCutClusterAbs, -1.f);
  SET_GPU_CONSTANT(timeCutClusterSig, 4.f);

  SET_GPU_CONSTANT(asic, fPars->asic);

  // Copy landau table
  size_t nLandauTableEntries = fPars->landauTable.values.size();
  SET_GPU_CONSTANT(landauTableSize, nLandauTableEntries);
  SET_GPU_CONSTANT(landauStepSize, fPars->landauTable.stepSize);
  GPU_ALLOC(landauTable, nLandauTableEntries);
  xpu::copy(fHitfinderGpu.landauTable, fPars->landauTable.values.data(), nLandauTableEntries);

  // Copy transformation matrix
  GPU_ALLOC(localToGlobalRotationByModule, 9 * nModules);
  GPU_ALLOC(localToGlobalTranslationByModule, 3 * nModules);

  // - Copy matrix into flat array
  for (int m = 0; m < nModules; m++) {
    const auto& module = fPars->modules.at(m);
    std::copy_n(module.localToGlobal.rotation.data(), 9, &fHitfinderHost.localToGlobalRotationByModule.h()[m * 9]);
    std::copy_n(module.localToGlobal.translation.data(), 3, &fHitfinderHost.localToGlobalRotationByModule.h()[m * 3]);
  }

  // - Then copy to GPU
  xpu::copy(fHitfinderHost.localToGlobalRotationByModule, xpu::host_to_device);
  xpu::copy(fHitfinderHost.localToGlobalTranslationByModule, xpu::host_to_device);

  // Copy Sensor Parameteres
  GPU_ALLOC(sensorPars, nModules);
  for (int m = 0; m < nModules; m++) {
    const auto& module = fPars->modules.at(m);
    auto& gpuPars      = fHitfinderHost.sensorPars.h()[m];
    gpuPars.dY         = module.dY;
    gpuPars.pitch      = module.pitch;
    gpuPars.stereoF    = module.stereoF;
    gpuPars.stereoB    = module.stereoB;
    gpuPars.lorentzF   = module.lorentzF;
    gpuPars.lorentzB   = module.lorentzB;
  }
  xpu::copy(fHitfinderHost.sensorPars, xpu::host_to_device);
}

void StsHitfinderChain::AllocateDynamic(size_t maxNDigisPerModule, size_t nDigisTotal)
{
  EnsureParameters();

  // TODO: some of these buffers have a constant size and can be allocated statically.
  // Just the data they contain is static.

  // Shorthands for common constants
  const int nChannels    = fPars->nChannels / 2;  // Only count channels on one side of the module
  const int nModules     = fPars->modules.size();
  const int nModuleSides = 2 * nModules;  // Number of module front + backsides

  const size_t maxNClustersPerModule = maxNDigisPerModule * kClustersPerDigiFactor;
  const size_t maxNHitsPerModule     = maxNClustersPerModule * kHitsPerClustersFactor;


  // Allocate Digi Buffers
  GPU_ALLOC(digiOffsetPerModule, nModuleSides + 1);
  GPU_ALLOC(digisPerModule, nDigisTotal);
  GPU_ALLOC(digisPerModuleTmp, nDigisTotal);
  GPU_ALLOC(digisSortedPerModule, nModuleSides);
  GPU_ALLOC(digiConnectorsPerModule, nDigisTotal);

  GPU_ALLOC(channelOffsetPerModule, nModuleSides * nChannels);

  GPU_ALLOC(maxErrorFront, 1);
  GPU_ALLOC(maxErrorBack, 1);

  // Allocate Cluster Buffers
  SET_GPU_CONSTANT(maxClustersPerModule, maxNClustersPerModule);
  GPU_ALLOC(channelStatusPerModule, nChannels * nModules);
  GPU_ALLOC(clusterIdxPerModule, maxNClustersPerModule * nModuleSides);
  GPU_ALLOC(clusterIdxPerModuleTmp, maxNClustersPerModule * nModuleSides);
  GPU_ALLOC(clusterIdxSortedPerModule, nModuleSides);
  GPU_ALLOC(clusterDataPerModule, maxNClustersPerModule * nModuleSides);
  GPU_ALLOC(nClustersPerModule, nModuleSides);

  // Allocate Hit Buffers
  SET_GPU_CONSTANT(maxHitsPerModule, maxNHitsPerModule);
  GPU_ALLOC(hitsPerModule, maxNHitsPerModule * nModules);
  GPU_ALLOC(nHitsPerModule, nModules);
}

StsHitfinderChain::DigiMap StsHitfinderChain::SortDigisIntoModules(gsl::span<const CbmStsDigi> digis,
                                                                   size_t& maxNDigisPerModule)
{
  DigiMap digiMap;

  int nChannelsPerSide = fPars->nChannels / 2;

  for (const auto& digi : digis) {
    int moduleAddr = CbmStsAddress::GetMotherAddress(digi.GetAddress(), kStsModule);
    bool isFront   = digi.GetChannel() < fPars->nChannels / 2;
    if (isFront) digiMap.front[moduleAddr].emplace_back(digi);
    else {
      CbmStsDigi tmpdigi = digi;
      tmpdigi.SetChannel(tmpdigi.GetChannel() - nChannelsPerSide);
      digiMap.back[moduleAddr].emplace_back(tmpdigi);
    }
  }

  maxNDigisPerModule = 0;
  for (const auto& mod : digiMap.front) {
    maxNDigisPerModule = std::max(maxNDigisPerModule, mod.second.size());
  }

  for (const auto& mod : digiMap.back) {
    maxNDigisPerModule = std::max(maxNDigisPerModule, mod.second.size());
  }

  return digiMap;
}

void StsHitfinderChain::FlattenDigis(DigiMap& digiMap)
{
  FlattenDigisSide(digiMap.front, true);
  FlattenDigisSide(digiMap.back, false);
}

void StsHitfinderChain::FlattenDigisSide(DigiMapSide& digis, bool isFront)
{
  EnsureParameters();

  int nModules           = fPars->modules.size();
  size_t* pMdigiOffset   = fHitfinderHost.digiOffsetPerModule.h();
  CbmStsDigi* pDigisFlat = fHitfinderHost.digisPerModule.h();

  int sideOffset = (isFront ? 0 : nModules);

  if (isFront) pMdigiOffset[0] = 0;
  for (int m = 0; m < nModules; m++) {
    size_t moduleOffset     = pMdigiOffset[sideOffset + m];
    const auto& moduleDigis = digis[fPars->modules[m].address];
    std::copy(moduleDigis.begin(), moduleDigis.end(), &pDigisFlat[moduleOffset]);
    pMdigiOffset[sideOffset + m + 1] = moduleOffset + moduleDigis.size();
  }
}

void StsHitfinderChain::CollectTimingInfo()
{
  // Check if profiling is enabled
  // TODO: xpu should have a nicer way to query if profiling is enabled
  if (xpu::get_timing<SortDigis>().empty()) return;

  // TODO: Add time counters
  fHitfinderTimes.timeSortDigi    = xpu::get_timing<SortDigis>().back();
  fHitfinderTimes.timeCluster     = xpu::get_timing<FindClustersSingleStep>().back();
  fHitfinderTimes.timeSortCluster = xpu::get_timing<SortClusters>().back();
  fHitfinderTimes.timeHits        = xpu::get_timing<FindHits>().back();
}

void StsHitfinderChain::AppendClustersModule(int module, bool isFront, std::vector<StsGpuCluster>& clusters)
{
  EnsureParameters();

  auto& hfc      = fHitfinderHost;
  int moduleSide = (isFront ? module : 2 * module);

  int nClusters     = hfc.nClustersPerModule.h()[moduleSide];
  auto* gpuClusters = &hfc.clusterDataPerModule.h()[moduleSide * hfc.maxClustersPerModule];

  clusters.reserve(clusters.size() + nClusters);

  for (int i = 0; i < nClusters; i++) {
    clusters.emplace_back(gpuClusters[i]);

    //   auto& clusterGpu = gpuClusters[i];

    //   auto& clusterOut = clusters.emplace_back();

    //   clusterOut.SetAddress(fPars->modules[module].address);
    //   clusterOut.SetProperties(clusterGpu.fCharge, clusterGpu.fPosition, clusterGpu.fPositionError, clusterGpu.fTime,
    //                             clusterGpu.fTimeError);
    //   clusterOut.SetSize(clusterGpu.fSize);
  }
}

void StsHitfinderChain::AppendHitsModule(int module, std::vector<StsGpuHit>& hits)
{
  EnsureParameters();

  auto& hfc = fHitfinderHost;

  auto* gpuHits = &hfc.hitsPerModule.h()[module * hfc.maxHitsPerModule];
  int nHitsGpu  = hfc.nHitsPerModule.h()[module];

  hits.reserve(hits.size() + nHitsGpu);

  for (int i = 0; i < nHitsGpu; i++) {
    hits.emplace_back(gpuHits[i]);
    // auto& hitGpu = gpuHits[i];

    // hits.emplace_back(fPars->modules[module].address,
    //                   TVector3 {hitGpu.fX, hitGpu.fY, hitGpu.fZ},
    //                   TVector3 {hitGpu.fDx, hitGpu.fDy, hitGpu.fDz},
    //                   hitGpu.fDxy,
    //                   0,
    //                   0,
    //                   double(hitGpu.fTime),
    //                   hitGpu.fTimeError,
    //                   hitGpu.fDu,
    //                   hitGpu.fDv);
  }
}
