/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#include "StsHitfinderChain.h"

#include <log.hpp>
#include <numeric>

using namespace cbm::algo;

void sts::HitfinderChain::SetParameters(const sts::HitfinderPars& parameters)
{
  fPars.emplace(parameters);
  AllocateStatic();
}

void sts::HitfinderChain::Finalize()
{
  // Explicitly free buffers in constant memory.
  // This avoids an issue in xpu with teardown order of static variables when using CPU.
  fHitfinder = {};
  xpu::set<TheHitfinder>(fHitfinder);
}

void sts::HitfinderChain::operator()(gsl::span<const CbmStsDigi> digis)
{
  EnsureParameters();

  xpu::scoped_timer t_("STS Hitfinder");

  int nModules       = fPars->modules.size();
  int nModuleSides   = nModules * 2;
  size_t nDigisTotal = digis.size();
  xpu::t_add_bytes(nDigisTotal * sizeof(CbmStsDigi));

  // Getting the digis on the GPU requires 3 steps
  // 1. Sort digis into buckets by module
  size_t maxNDigisPerModule;
  DigiMap digiMap = SortDigisIntoModules(digis, maxNDigisPerModule);
  // 2. Once we know number of digis per module, we can allocate
  //    the dynamic buffers on the gpu, as the buffer sizes depend on that value
  AllocateDynamic(maxNDigisPerModule, nDigisTotal);
  // 3. Copy digis into flat array with offsets per module
  FlattenDigis(digiMap);

  if (Opts().LogLevel() == trace) EnsureDigiOffsets(digiMap);

  xpu::queue queue;

  // Clear buffers
  // Not all buffers have to initialized, but useful for debugging

  L_(debug) << "STS Hitfinder Chain: Clearing buffers...";
  auto& hfc = fHitfinder;
  // xpu::memset(hfc.digisPerModule, 0);
  // xpu::memset(hfc.digisPerModuleTmp, 0);
  // xpu::memset(hfc.digisSortedPerModule, 0);
  // xpu::memset(hfc.digiOffsetPerModule, 0);
  queue.memset(hfc.digiConnectorsPerModule, 0);
  queue.memset(hfc.channelOffsetPerModule, 0);
  queue.memset(hfc.clusterIdxPerModule, 0);
  // xpu::memset(hfc.clusterIdxPerModuleTmp, 0);
  // xpu::memset(hfc.clusterIdxSortedPerModule, 0);
  // xpu::memset(hfc.clusterDataPerModule, 0);
  queue.memset(hfc.nClustersPerModule, 0);
  // xpu::memset(hfc.hitsPerModule, 0);
  queue.memset(hfc.nHitsPerModule, 0);
  queue.memset(hfc.maxClusterTimeErrorByModuleSide, 0);

  L_(debug) << "STS Hitfinder Chain: Copy digis buffers...";
  xpu::set<TheHitfinder>(fHitfinder);
  queue.copy(hfc.digisPerModule, xpu::h2d);
  queue.copy(hfc.digiOffsetPerModule, xpu::h2d);

  L_(debug) << "STS Hitfinder Chain: Sort Digis...";
  // TODO: skip temporary buffers and sort directly into digisSortedPerModule

  queue.launch<SortDigis>(xpu::n_blocks(nModuleSides));
  xpu::k_add_bytes<SortDigis>(nDigisTotal * sizeof(CbmStsDigi));
  if (Opts().LogLevel() == trace) {
    L_(trace) << "Ensuring STS digis are sorted...";
    queue.copy(hfc.digisPerModule, xpu::d2h);
    queue.copy(hfc.digiOffsetPerModule, xpu::d2h);
    queue.wait();
    EnsureDigisSorted();
  }

  L_(debug) << "STS Hitfinder Chain: Find Clusters...";
  if (!Params().sts.findClustersMultiKernels) { queue.launch<FindClusters>(xpu::n_blocks(nModuleSides)); }
  else {
    queue.launch<ChannelOffsets>(xpu::n_blocks(nModuleSides));
    xpu::k_add_bytes<ChannelOffsets>(nDigisTotal * sizeof(CbmStsDigi));
    queue.launch<CreateDigiConnections>(xpu::n_threads(nDigisTotal));
    xpu::k_add_bytes<CreateDigiConnections>(nDigisTotal * sizeof(CbmStsDigi));
    queue.launch<CreateClusters>(xpu::n_blocks(nModuleSides));
    xpu::k_add_bytes<CreateClusters>(nDigisTotal * sizeof(CbmStsDigi));
  }
  if (Opts().LogLevel() == trace) {
    L_(trace) << "Ensuring STS channel offsets correct...";
    xpu::buffer_prop propsOffset {hfc.channelOffsetPerModule};
    std::vector<u32> channelOffsetPerModule;
    channelOffsetPerModule.resize(propsOffset.size());
    queue.copy(hfc.channelOffsetPerModule.get(), channelOffsetPerModule.data(), propsOffset.size_bytes());
    queue.wait();
    EnsureChannelOffsets(channelOffsetPerModule);

    L_(trace) << "Ensuring STS clusters are ok...";
    xpu::buffer_prop props {hfc.clusterIdxPerModule};

    std::vector<GpuClusterIdx> clusterIdxPerModule;
    clusterIdxPerModule.resize(props.size());
    std::vector<int> nClustersPerModule;
    nClustersPerModule.resize(fPars->modules.size() * 2);

    queue.copy(hfc.clusterIdxPerModule.get(), clusterIdxPerModule.data(), props.size_bytes());
    queue.copy(hfc.nClustersPerModule.get(), nClustersPerModule.data(), nClustersPerModule.size() * sizeof(int));
    queue.wait();
    EnsureClustersSane(clusterIdxPerModule, nClustersPerModule);
  }

  // Run cluster finding steps in indivual kernels, useful for debugging / profiling
  // xpu::run_kernel<CalculateOffsets>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClusters>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  L_(debug) << "STS Hitfinder Chain: Sort Clusters...";
  queue.launch<SortClusters>(xpu::n_blocks(nModuleSides));  // FIXME n_blocks(nModuleSides) once debugging is done

  L_(debug) << "STS Hitfinder Chain: Find Hits...";
  queue.copy(hfc.nClustersPerModule, xpu::d2h);
  queue.wait();
  xpu::h_view nClusters {hfc.nClustersPerModule};
  size_t nClustersFront = std::accumulate(nClusters.begin(), nClusters.begin() + nModules, 0);
  bool isCpu            = xpu::device::active().backend() == xpu::cpu;
  xpu::grid findHitsG   = isCpu ? xpu::n_blocks(nModules) : xpu::n_threads(nClustersFront);
  queue.launch<FindHits>(findHitsG);

  // Transfer Hits / Cluster back to host
  // TODO: Hits and Clusters are stored in buckets. Currently we transfer the entire
  // bucket arrays back to the host. This includes a lot of unused bytes.
  // Better to either flatten both arrays on the GPU and transfer flat array back.
  // Or do multiple copies to copy contents of individual buckets.
  queue.copy(hfc.clusterDataPerModule, xpu::d2h);
  queue.copy(hfc.nClustersPerModule, xpu::d2h);
  queue.copy(hfc.hitsPerModule, xpu::d2h);
  queue.copy(hfc.nHitsPerModule, xpu::d2h);

  queue.wait();

  xpu::h_view nHits {hfc.nHitsPerModule};
  // xpu::h_view nClusters{hfc.nClustersPerModule};
  size_t nHitsTotal     = std::accumulate(nHits.begin(), nHits.end(), 0);
  size_t nClustersTotal = std::accumulate(nClusters.begin(), nClusters.end(), 0);

  xpu::k_add_bytes<SortClusters>(nClustersTotal * sizeof(GpuClusterIdx));
  xpu::k_add_bytes<FindHits>(nClustersTotal * sizeof(sts::Cluster));

  L_(info) << "Timeslice contains " << nHitsTotal << " STS hits and " << nClustersTotal << " STS clusters";
}

void sts::HitfinderChain::EnsureParameters()
{
  if (fPars == std::nullopt) throw std::runtime_error("sts::HitfinderChain: Parameters not set. Can't continue!");
}

void sts::HitfinderChain::AllocateStatic()
{
  EnsureParameters();

  // Shorthands for common constants
  const int nChannels = fPars->nChannels / 2;  // Only count channels on one side of the module
  const int nModules  = fPars->modules.size();
  const int nModuleSides = 2 * nModules;  // Number of module front + backsides

  xpu::queue q;

  // Set GPU constants
  fHitfinder.nModules  = nModules;
  fHitfinder.nChannels = nChannels;
  fHitfinder.asic      = fPars->asic;

  // Copy landau table
  size_t nLandauTableEntries = fPars->landauTable.values.size();
  fHitfinder.landauTableSize = nLandauTableEntries;
  fHitfinder.landauStepSize  = fPars->landauTable.stepSize;
  fHitfinder.landauTable.reset(nLandauTableEntries, xpu::buf_io);
  q.copy(fPars->landauTable.values.data(), fHitfinder.landauTable.get(), nLandauTableEntries * sizeof(float));

  // Copy transformation matrix
  fHitfinder.localToGlobalRotationByModule.reset(9 * nModules, xpu::buf_io);
  fHitfinder.localToGlobalTranslationByModule.reset(3 * nModules, xpu::buf_io);

  // - Copy matrix into flat array
  xpu::h_view hRot {fHitfinder.localToGlobalRotationByModule};
  xpu::h_view hTrans {fHitfinder.localToGlobalTranslationByModule};
  for (int m = 0; m < nModules; m++) {
    const auto& module = fPars->modules.at(m);
    std::copy_n(module.localToGlobal.rotation.data(), 9, &hRot[m * 9]);
    std::copy_n(module.localToGlobal.translation.data(), 3, &hTrans[m * 3]);
  }

  // - Then copy to GPU
  q.copy(fHitfinder.localToGlobalRotationByModule, xpu::h2d);
  q.copy(fHitfinder.localToGlobalTranslationByModule, xpu::h2d);

  // Copy Sensor Parameteres
  fHitfinder.sensorPars.reset(nModules, xpu::buf_io);
  xpu::h_view hSensorPars {fHitfinder.sensorPars};
  for (int m = 0; m < nModules; m++) {
    const auto& module = fPars->modules.at(m);
    auto& gpuPars      = hSensorPars[m];
    gpuPars.dY         = module.dY;
    gpuPars.pitch      = module.pitch;
    gpuPars.stereoF    = module.stereoF;
    gpuPars.stereoB    = module.stereoB;
    gpuPars.lorentzF   = module.lorentzF;
    gpuPars.lorentzB   = module.lorentzB;
  }
  xpu::copy(fHitfinder.sensorPars, xpu::h2d);

  // Time errors
  fHitfinder.maxClusterTimeErrorByModuleSide.reset(nModuleSides, xpu::buf_device);

  q.wait();
}

void sts::HitfinderChain::AllocateDynamic(size_t maxNDigisPerModule, size_t nDigisTotal)
{
  L_(debug) << "STS Hitfinder Chain: Allocating dynamic memory for " << maxNDigisPerModule << " digis per module and "
            << nDigisTotal << " digis in total";
  EnsureParameters();

  xpu::scoped_timer t_ {"Allocate"};

  // TODO: some of these buffers have a constant size and can be allocated statically.
  // Just the data they contain is static.

  // Shorthands for common constants
  const int nChannels    = fPars->nChannels / 2;  // Only count channels on one side of the module
  const int nModules     = fPars->modules.size();
  const int nModuleSides = 2 * nModules;  // Number of module front + backsides

  const size_t maxNClustersPerModule = maxNDigisPerModule * kClustersPerDigiFactor;
  const size_t maxNHitsPerModule     = maxNClustersPerModule * kHitsPerClustersFactor;

  // Allocate Digi Buffers
  fHitfinder.digiOffsetPerModule.reset(nModuleSides + 1, xpu::buf_io);
  fHitfinder.digisPerModule.reset(nDigisTotal, xpu::buf_io);

  fHitfinder.digisPerModuleTmp.reset(nDigisTotal, xpu::buf_device);
  fHitfinder.digiConnectorsPerModule.reset(nDigisTotal, xpu::buf_device);

  fHitfinder.channelOffsetPerModule.reset(nModuleSides * nChannels, xpu::buf_device);

  // Allocate Cluster Buffers
  fHitfinder.maxClustersPerModule = maxNClustersPerModule;

  fHitfinder.clusterIdxPerModule.reset(maxNClustersPerModule * nModuleSides, xpu::buf_device);
  fHitfinder.clusterIdxPerModuleTmp.reset(maxNClustersPerModule * nModuleSides, xpu::buf_device);
  fHitfinder.clusterIdxSortedPerModule.reset(nModuleSides, xpu::buf_device);

  fHitfinder.clusterDataPerModule.reset(maxNClustersPerModule * nModuleSides, xpu::buf_io);
  fHitfinder.nClustersPerModule.reset(nModuleSides, xpu::buf_io);

  // Allocate Hit Buffers
  fHitfinder.maxHitsPerModule = maxNHitsPerModule;
  fHitfinder.hitsPerModule.reset(maxNHitsPerModule * nModules, xpu::buf_io);
  fHitfinder.nHitsPerModule.reset(nModules, xpu::buf_io);
}

sts::HitfinderChain::DigiMap sts::HitfinderChain::SortDigisIntoModules(gsl::span<const CbmStsDigi> digis,
                                                                       size_t& maxNDigisPerModule)
{
  L_(debug) << "STS Hitfinder Chain: Sorting " << digis.size() << " digis into modules";
  xpu::scoped_timer t_ {"Sort Digis"};

  DigiMap digiMap;

  int nChannelsPerSide = fPars->nChannels / 2;

  auto isPulser = [&](const CbmStsDigi& digi) {
    return std::find_if(fPars->modules.begin(), fPars->modules.end(),
                        [&](const auto& mod) { return mod.address == digi.GetAddress(); })
           == fPars->modules.end();
  };

  size_t nPulsers = 0;

  for (const auto& digi : digis) {

    if (isPulser(digi)) {
      nPulsers++;
      continue;
    }

    int moduleAddr = CbmStsAddress::GetMotherAddress(digi.GetAddress(), kStsModule);
    bool isFront   = digi.GetChannel() < fPars->nChannels / 2;
    if (isFront) digiMap.front[moduleAddr].emplace_back(digi);
    else {
      CbmStsDigi tmpdigi = digi;
      tmpdigi.SetChannel(tmpdigi.GetChannel() - nChannelsPerSide);
      digiMap.back[moduleAddr].emplace_back(tmpdigi);
    }
  }

  if (nPulsers > 0) L_(warning) << "STS Hitfinder: Discarded " << nPulsers << " pulser digis";

  // Print digi counts per module
  for (const auto& mod : fPars->modules) {
    i32 moduleAddr = mod.address;
    L_(debug) << "Module " << moduleAddr << " has " << digiMap.front[moduleAddr].size() << " front digis and "
              << digiMap.back[moduleAddr].size() << " back digis";
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

void sts::HitfinderChain::FlattenDigis(DigiMap& digiMap)
{
  L_(debug) << "STS Hitfinder Chain: Flattening digis";
  xpu::scoped_timer t_ {"Flatten Digis"};
  FlattenDigisSide(digiMap.front, true);
  FlattenDigisSide(digiMap.back, false);
}

void sts::HitfinderChain::FlattenDigisSide(DigiMapSide& digis, bool isFront)
{
  EnsureParameters();

  int nModules           = fPars->modules.size();
  xpu::h_view pMdigiOffset {fHitfinder.digiOffsetPerModule};
  xpu::h_view pDigisFlat {fHitfinder.digisPerModule};

  int sideOffset = (isFront ? 0 : nModules);

  if (isFront) pMdigiOffset[0] = 0;
  for (int m = 0; m < nModules; m++) {
    size_t moduleOffset     = pMdigiOffset[sideOffset + m];
    const auto& moduleDigis = digis[fPars->modules[m].address];
    std::copy(moduleDigis.begin(), moduleDigis.end(), &pDigisFlat[moduleOffset]);
    pMdigiOffset[sideOffset + m + 1] = moduleOffset + moduleDigis.size();
  }
}

void sts::HitfinderChain::AppendClustersModule(int module, bool isFront, std::vector<sts::Cluster>& clusters)
{
  EnsureParameters();

  auto& hfc      = fHitfinder;
  int moduleSide = (isFront ? module : 2 * module);
  xpu::h_view hNClusters {hfc.nClustersPerModule};
  xpu::h_view hClusters {hfc.clusterDataPerModule};

  int nClusters     = hNClusters[moduleSide];
  auto* gpuClusters = &hClusters[moduleSide * hfc.maxClustersPerModule];

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

void sts::HitfinderChain::AppendHitsModule(int module, std::vector<sts::Hit>& hits)
{
  EnsureParameters();

  xpu::h_view hHits {fHitfinder.hitsPerModule};
  xpu::h_view hNHits {fHitfinder.nHitsPerModule};

  auto* gpuHits = &hHits[module * fHitfinder.maxHitsPerModule];
  int nHitsGpu  = hNHits[module];

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

void sts::HitfinderChain::EnsureDigiOffsets(DigiMap& digi)
{
  xpu::h_view digiOffset {fHitfinder.digiOffsetPerModule};

  size_t nModules = fPars->modules.size();
  size_t offset   = 0;
  // Front
  for (size_t m = 0; m < nModules; m++) {
    const auto& moduleDigis = digi.front[fPars->modules[m].address];
    if (digiOffset[m] != offset) {
      L_(fatal) << "Module " << m << ": Digi offset mismatch: " << digiOffset[m] << " != " << offset;
      std::abort();
    }
    offset += moduleDigis.size();
  }

  // Back
  for (size_t m = 0; m < nModules; m++) {
    const auto& moduleDigis = digi.back[fPars->modules[m].address];

    if (digiOffset[nModules + m] != offset) {
      L_(fatal) << "Module " << nModules + m << ": Digi offset mismatch: " << digiOffset[nModules + m]
                << " != " << offset;
      std::abort();
    }
    offset += moduleDigis.size();
  }

  if (offset != digiOffset[2 * nModules]) {
    L_(fatal) << "Digi offset mismatch: " << digiOffset[2 * nModules] << " != " << offset;
    std::abort();
  }
}

void sts::HitfinderChain::EnsureDigisSorted()
{
  xpu::h_view digiOffset {fHitfinder.digiOffsetPerModule};
  xpu::h_view digis {fHitfinder.digisPerModule};

  bool isSorted = true;

  for (size_t m = 0; m < fPars->modules.size(); m++) {
    int nDigis = digiOffset[m + 1] - digiOffset[m];
    if (nDigis == 0) continue;

    auto* digisModule = &digis[digiOffset[m]];

    for (int i = 0; i < nDigis - 1; i++) {
      const auto &digi1 = digisModule[i], &digi2 = digisModule[i + 1];

      if ((digi1.GetChannel() < digi2.GetChannel())
          || (digi1.GetChannel() == digi2.GetChannel()
              && digi1.GetTime() <= digi2.GetTime())  // Unpacker sometimes produces multiple
                                                      // digis with the same time and channel, FIXME!
      ) {
        continue;
      }

      isSorted = false;
      L_(error) << "Module " << m << " not sorted: " << digi1.ToString() << " " << digi2.ToString();
      break;
    }
  }

  if (!isSorted) {
    L_(fatal) << "Digis are not sorted";
    std::abort();
  }
}

void sts::HitfinderChain::EnsureChannelOffsets(span<u32> channelOffsetsByModule)
{
  xpu::h_view digisPerModule {fHitfinder.digisPerModule};
  xpu::h_view digiOffsetPerModule {fHitfinder.digiOffsetPerModule};
  for (size_t m = 0; m < fPars->modules.size() * 2; m++) {
    int nChannels = fPars->nChannels / 2;  // Consider module sides

    std::vector<u32> expectedChannelOffsets(nChannels, 0);

    int offset = digiOffsetPerModule[m];
    int nDigis = digiOffsetPerModule[m + 1] - offset;
    span<CbmStsDigi> digis(&digisPerModule[offset], nDigis);
    span<u32> channelOffsets = channelOffsetsByModule.subspan(m * nChannels, nChannels);

    if (nDigis == 0) continue;

    if (channelOffsets[0] != 0) {
      L_(fatal) << "Module " << m << ": First channel offset is not 0";
      std::abort();
    }

    int chan = digis[0].GetChannel();
    for (int i = 0; i < nDigis; i++) {
      if (digis[i].GetChannel() != chan) {
        while (chan < digis[i].GetChannel()) {
          chan++;
          expectedChannelOffsets[chan] = i;
        }
      }
    }
    while (chan < nChannels) {
      chan++;
      expectedChannelOffsets[chan] = nDigis;
    }

    for (int i = 0; i < nChannels; i++) {
      if (channelOffsets[i] != expectedChannelOffsets[i]) {
        L_(fatal) << "Module " << m << ": Channel offset for channel " << i << " is " << channelOffsets[i]
                  << " but should be " << expectedChannelOffsets[i];
        std::abort();
      }
    }
  }
}

void sts::HitfinderChain::EnsureClustersSane(span<GpuClusterIdx> hClusterIdx, span<int> hNClusters)
{
  for (size_t m = 0; m < 2 * fPars->modules.size(); m++) {
    int nClusters = hNClusters[m];

    L_(trace) << "Module " << m << " has " << nClusters << " clusters of " << fHitfinder.maxClustersPerModule;

    if (nClusters == 0) continue;

    if (nClusters < 0) {
      L_(fatal) << "Module " << m << " has negative number of clusters " << nClusters;
      std::abort();
    }
    if (size_t(nClusters) > fHitfinder.maxClustersPerModule) {
      L_(fatal) << "Module " << m << " has " << nClusters << " clusters, but only " << fHitfinder.maxClustersPerModule
                << " are allowed";
      std::abort();
    }

    auto* clusterIdx = &hClusterIdx[m * fHitfinder.maxClustersPerModule];

    for (int i = 0; i < nClusters; i++) {
      auto& cidx = clusterIdx[i];

      if (cidx.fIdx < 0 || size_t(cidx.fIdx) >= fHitfinder.maxClustersPerModule) {
        L_(fatal) << "Cluster " << i << " of module " << m << " has invalid index " << cidx.fIdx;
        std::abort();
      }

      if (cidx.fTime == 0xFFFFFFFF) {
        L_(fatal) << "Cluster " << i << " of module " << m << " has invalid time " << cidx.fTime;
        std::abort();
      }
    }
  }

  L_(trace) << "Clusters ok";
}
