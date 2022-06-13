/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#include "CbmGpuRecoSts.h"

#include "CbmAddress.h"
#include "CbmDigiManager.h"
#include "CbmStsAddress.h"
#include "CbmStsCluster.h"
#include "CbmStsGpuRecoDevice.h"
#include "CbmStsHit.h"
#include "CbmStsPhysics.h"
#include "CbmStsRecoModule.h"

#include <TCanvas.h>
#include <TClonesArray.h>
#include <TGeoMatrix.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <vector>

using namespace experimental;

void CbmGpuRecoSts::SetupConstants()
{
  assert(fConfig != std::nullopt);
  assert(fConfig->recoModules.size() == fConfig->parModules.size());
  assert(fConfig->moduleAddrs.size() == fConfig->parModules.size());
  assert(fConfig->hitfinderCfg.size() == fConfig->parModules.size());

  size_t nModules = fConfig->parModules.size();

  auto& hfc = fHitfinderCpu;
  auto& hfg = fHitfinderGpu;

  LOG(info) << "STS GPU RECO: setting constants";

  size_t nChannels     = fConfig->parModules[0].GetNofChannels();
  ::CbmStsParAsic asic = fConfig->parModules[0].GetParAsic(0);  // TODO values are currently identical for all asics??
  // for (auto &mod : modules) {
  //     assert(nChannels == mod.GetNofChannels());
  //     for (size_t c = 0; c < nChannels; c++) {
  //         ::CbmStsParAsic otherAsic = mod.GetParAsic(c);
  //         assert(asic.GetNofAdc() == otherAsic.GetNofAdc());
  //         assert(asic.GetDynRange() == otherAsic.GetDynRange());
  //         assert(asic.GetThreshold() == otherAsic.GetThreshold());
  //         assert(asic.GetTimeResol() == otherAsic.GetTimeResol());
  //         assert(asic.GetDeadTime() == otherAsic.GetDeadTime());
  //         assert(asic.GetNoise() == otherAsic.GetNoise());
  //         assert(asic.GetZeroNoiseRate() == otherAsic.GetZeroNoiseRate());
  //     }
  // }

  nChannels /= 2;
  hfc.nModules = hfg.nModules = nModules;
  hfc.nChannels = hfg.nChannels = nChannels;

  hfc.timeCutDigiAbs = hfg.timeCutDigiAbs = -1.f;
  hfc.timeCutDigiSig = hfg.timeCutDigiSig = 3.f;
  hfc.timeCutClusterAbs = hfg.timeCutClusterAbs = -1.f;
  hfc.timeCutClusterSig = hfg.timeCutClusterSig = 4.f;

  hfc.asic.nAdc           = asic.GetNofAdc();
  hfc.asic.dynRange       = asic.GetDynRange();
  hfc.asic.threshold      = asic.GetThreshold();
  hfc.asic.timeResolution = asic.GetTimeResol();
  hfc.asic.deadTime       = asic.GetDeadTime();
  hfc.asic.noise          = asic.GetNoise();
  hfc.asic.zeroNoiseRate  = asic.GetZeroNoiseRate();
  hfg.asic                = hfc.asic;
}

void CbmGpuRecoSts::Setup(size_t maxDigisPerModule, size_t nDigitsTotal)
{
  assert(fConfig != std::nullopt);

  // static constexpr size_t maxDigisPerModule    = 60000;
  // static constexpr size_t maxClustersPerModule = 30000;
  // static constexpr size_t maxHitsPerModule     = 120000;

  const size_t maxClustersPerModule = maxDigisPerModule * 0.5;
  const size_t maxHitsPerModule     = maxClustersPerModule * 4;

  size_t nModules  = fConfig->parModules.size();
  size_t nChannels = fConfig->parModules[0].GetNofChannels() / 2;


  size_t nModulesTotal = nModules * 2;  // Front- and backside

  auto& hfc = fHitfinderCpu;
  auto& hfg = fHitfinderGpu;

  auto landauTable    = fConfig->physics->GetLandauWidthTable();
  hfc.landauTableSize = hfg.landauTableSize = landauTable.size();
  hfc.landauTable                           = xpu::hd_buffer<float> {size_t(hfc.landauTableSize)};
  hfg.landauTable                           = hfc.landauTable.d();
  auto landauEntry                          = landauTable.begin();
  auto prevLandauEntry                      = landauEntry;
  hfc.landauTable.h()[0]                    = landauEntry->second;
  landauEntry++;
  hfc.landauStepSize = hfg.landauStepSize = landauEntry->first - prevLandauEntry->first;
  for (size_t i = 1; landauEntry != landauTable.end(); i++, landauEntry++) {
    assert(hfc.landauStepSize == landauEntry->first - prevLandauEntry->first);
    assert(i < hfc.landauTable.size());
    hfc.landauTable.h()[i] = landauEntry->second;
    prevLandauEntry        = landauEntry;
  }

  LOG(info) << "STS GPU RECO: finished landau";

  hfc.localToGlobalTranslationByModule = xpu::hd_buffer<float> {nModules * 3};
  hfg.localToGlobalTranslationByModule = hfc.localToGlobalTranslationByModule.d();
  hfc.localToGlobalRotationByModule    = xpu::hd_buffer<float> {nModules * 9};
  hfg.localToGlobalRotationByModule    = hfc.localToGlobalRotationByModule.d();
  for (size_t m = 0; m < fConfig->recoModules.size(); m++) {
    auto& recoMod       = fConfig->recoModules[m];
    TGeoHMatrix* matrix = recoMod->getMatrix();

    const double* rot = matrix->GetRotationMatrix();
    float* tgt        = &hfc.localToGlobalRotationByModule.h()[m * 9];
    std::copy_n(rot, 9, tgt);

    const double* tr = matrix->GetTranslation();
    tgt              = &hfc.localToGlobalTranslationByModule.h()[m * 3];
    std::copy_n(tr, 3, tgt);
  }

  LOG(info) << "STS GPU RECO: finished transformation matrix";

  hfc.hitfinderConfig = xpu::hd_buffer<CbmStsHitFinderConfig>(nModules);
  hfg.hitfinderConfig = hfc.hitfinderConfig.d();

  std::copy_n(fConfig->hitfinderCfg.begin(), nModules, hfc.hitfinderConfig.h());

  LOG(info) << "STS GPU RECO: finished setting hitfinder config";

  // FIXME: allocate by total number of digis instead. This array is flat.
  hfc.digiOffsetPerModule    = xpu::hd_buffer<size_t> {nModulesTotal + 1};
  hfg.digiOffsetPerModule    = hfc.digiOffsetPerModule.d();
  hfc.digisPerModule         = xpu::hd_buffer<CbmStsDigi> {nDigitsTotal};
  hfg.digisPerModule         = hfc.digisPerModule.d();
  hfc.digisPerModuleTmp      = xpu::d_buffer<CbmStsDigi> {nDigitsTotal};
  hfg.digisPerModuleTmp      = hfc.digisPerModuleTmp.d();
  hfc.digisSortedPerModule   = xpu::d_buffer<CbmStsDigi*> {nModulesTotal};
  hfg.digisSortedPerModule   = hfc.digisSortedPerModule.d();
  hfc.digiConnectorPerModule = xpu::d_buffer<CbmStsDigiConnector> {nDigitsTotal};
  hfg.digiConnectorPerModule = hfc.digiConnectorPerModule.d();

  hfc.channelOffsetPerModule = xpu::d_buffer<unsigned int> {nModulesTotal * nChannels};
  hfg.channelOffsetPerModule = hfc.channelOffsetPerModule.d();

  hfc.maxErrorFront = xpu::d_buffer<float> {1};
  hfg.maxErrorFront = hfc.maxErrorFront.d();
  hfc.maxErrorBack  = xpu::d_buffer<float> {1};
  hfg.maxErrorBack  = hfc.maxErrorBack.d();

  LOG(info) << "STS GPU RECO: finished digis";

  hfc.maxClustersPerModule = hfg.maxClustersPerModule = maxClustersPerModule;
  hfc.channelStatusPerModule                          = xpu::d_buffer<int> {nChannels * nModulesTotal};
  hfg.channelStatusPerModule                          = hfc.channelStatusPerModule.d();
  hfc.clusterIdxPerModule       = xpu::hd_buffer<CbmStsClusterIdx> {maxClustersPerModule * nModulesTotal};
  hfg.clusterIdxPerModule       = hfc.clusterIdxPerModule.d();
  hfc.clusterIdxPerModuleTmp    = xpu::hd_buffer<CbmStsClusterIdx> {maxClustersPerModule * nModulesTotal};
  hfg.clusterIdxPerModuleTmp    = hfc.clusterIdxPerModuleTmp.d();
  hfc.clusterIdxSortedPerModule = xpu::hd_buffer<CbmStsClusterIdx*> {nModulesTotal};
  hfg.clusterIdxSortedPerModule = hfc.clusterIdxSortedPerModule.d();
  hfc.clusterDataPerModule      = xpu::hd_buffer<CbmStsClusterData> {maxClustersPerModule * nModulesTotal};
  hfg.clusterDataPerModule      = hfc.clusterDataPerModule.d();
  hfc.nClustersPerModule        = xpu::hd_buffer<int> {nModulesTotal};
  hfg.nClustersPerModule        = hfc.nClustersPerModule.d();

  LOG(info) << "STS GPU RECO: finished cluster";

  hfc.maxHitsPerModule = hfg.maxHitsPerModule = maxHitsPerModule;
  hfc.hitsPerModule                           = xpu::hd_buffer<CbmStsHit>(maxHitsPerModule * nModules);
  hfg.hitsPerModule                           = hfc.hitsPerModule.d();
  hfc.nHitsPerModule                          = xpu::hd_buffer<int>(nModules);
  hfg.nHitsPerModule                          = hfc.nHitsPerModule.d();

  LOG(info) << "STS GPU RECO: finished setup";
  LOG(info) << "- nModules = " << nModules;
  LOG(info) << "- nChannels = " << nChannels;
}

void CbmGpuRecoSts::RunHitFinder()
{
  setenv("XPU_PROFILE", "1", 1);
  xpu::initialize();

  auto& hfc = fHitfinderCpu;

  // ROCm bug: Mi100 name is an emtpy string...
  auto deviceName = (xpu::device_properties().name.empty()) ? "AMD Mi100" : xpu::device_properties().name;

  LOG(info) << "STS GPU RECO: running GPU hit finder on device " << deviceName;
  LOG(info) << "STS GPU RECO: Allocate and fetch digis";


  // FIXME: This is ugly, setup has to be split in two parts, constants required by FetchDigis
  // And FetchDigis required by Setup...
  SetupConstants();
  size_t maxDigisPerModule = 0;
  size_t nDigisTotal       = 0;
  FetchDigis(maxDigisPerModule, nDigisTotal);
  Setup(maxDigisPerModule, nDigisTotal);

  LOG(info) << "STS GPU RECO: finished digis / running clusterizer now";
  LOG(info) << "STS GPU RECO: largest module has " << maxDigisPerModule << " digis";

  // Not all buffers have to initialized, but useful for debugging
  // xpu::memset(hfc.digisPerModule, 0);
  // xpu::memset(hfc.digisPerModuleTmp, 0);
  // xpu::memset(hfc.digisSortedPerModule, 0);
  // xpu::memset(hfc.digiOffsetPerModule, 0);
  xpu::memset(hfc.digiConnectorPerModule, 0);
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

  hfc.digiOffsetPerModule.h()[0] = 0;
  for (int m = 0; m < hfc.nModules; m++) {
    size_t offset = hfc.digiOffsetPerModule.h()[m];
    auto& md      = fDigisByModuleF[fConfig->moduleAddrs[m]];
    std::copy(md.begin(), md.end(), &hfc.digisPerModule.h()[offset]);
    hfc.digiOffsetPerModule.h()[m + 1] = offset + md.size();
  }
  for (int m = 0; m < hfc.nModules; m++) {
    size_t offset = hfc.digiOffsetPerModule.h()[m + hfc.nModules];
    auto& md      = fDigisByModuleB[fConfig->moduleAddrs[m]];
    std::copy(md.begin(), md.end(), &hfc.digisPerModule.h()[offset]);
    hfc.digiOffsetPerModule.h()[m + hfc.nModules + 1] = offset + md.size();
  }
  assert(nDigisTotal == hfc.digiOffsetPerModule.h()[2 * hfc.nModules]);

  TStopwatch timer;

  // constants only need to be transferred once. should be excluded from time measurement
  xpu::copy(hfc.landauTable, xpu::host_to_device);
  xpu::copy(hfc.hitfinderConfig, xpu::host_to_device);
  xpu::copy(hfc.localToGlobalRotationByModule, xpu::host_to_device);
  xpu::copy(hfc.localToGlobalTranslationByModule, xpu::host_to_device);

  xpu::set_constant<HitFinder>(fHitfinderGpu);
  xpu::copy(hfc.digisPerModule, xpu::host_to_device);
  xpu::copy(hfc.digiOffsetPerModule, xpu::host_to_device);

  xpu::run_kernel<SortDigis>(xpu::grid::n_blocks(hfc.nModules * 2));
  xpu::run_kernel<FindClustersSingleStep>(xpu::grid::n_blocks(hfc.nModules * 2));
  // Run cluster finding steps in indivual kernels, useful for debugging / profiling
  // xpu::run_kernel<CalculateOffsets>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClusters>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<FindClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  // xpu::run_kernel<CalculateClustersBasic>(xpu::grid::n_blocks(hfc.nModules * 2));
  xpu::run_kernel<SortClusters>(xpu::grid::n_blocks(hfc.nModules * 2));
  xpu::run_kernel<FindHits>(xpu::grid::n_blocks(hfc.nModules));

  // Check if profiling is enabled
  if (not xpu::get_timing<SortDigis>().empty()) {
    timeSortDigi    = xpu::get_timing<SortDigis>().back();
    timeCluster     = xpu::get_timing<FindClustersSingleStep>().back();
    timeSortCluster = xpu::get_timing<SortClusters>().back();
    timeHits        = xpu::get_timing<FindHits>().back();
  }

  xpu::copy(hfc.clusterDataPerModule, xpu::device_to_host);
  xpu::copy(hfc.nClustersPerModule, xpu::device_to_host);
  xpu::copy(hfc.hitsPerModule, xpu::device_to_host);
  xpu::copy(hfc.nHitsPerModule, xpu::device_to_host);


  size_t mostCluster = 0, mostHits = 0;
  for (int m = 0; m < hfc.nModules; m++) {
    mostCluster = std::max<size_t>(hfc.nClustersPerModule.h()[m], mostCluster);
    mostCluster = std::max<size_t>(hfc.nClustersPerModule.h()[m + hfc.nModules], mostCluster);
    mostHits    = std::max<size_t>(hfc.nHitsPerModule.h()[m], mostHits);

    nCluster += hfc.nClustersPerModule.h()[m];
    nCluster += hfc.nClustersPerModule.h()[m + hfc.nModules];

    nHits += hfc.nHitsPerModule.h()[m];
  }

  LOG(info) << "STS GPU RECO: finished hitfinder";
}

void CbmGpuRecoSts::ForwardClustersAndHits(TClonesArray* clusters, TClonesArray* hits)
{
  auto& hfc = fHitfinderCpu;

  for (int module = 0; module < hfc.nModules; module++) {

    auto* gpuClusterF    = &hfc.clusterDataPerModule.h()[module * hfc.maxClustersPerModule];
    auto* gpuClusterIdxF = &hfc.clusterIdxPerModule.h()[module * hfc.maxClustersPerModule];
    int nClustersFGpu    = hfc.nClustersPerModule.h()[module];

    for (int i = 0; i < nClustersFGpu; i++) {
      auto& cidx       = gpuClusterIdxF[i];
      auto& clusterGpu = gpuClusterF[cidx.fIdx];

      unsigned int outIdx = clusters->GetEntriesFast();
      auto* clusterOut    = new ((*clusters)[outIdx])::CbmStsCluster {};

      clusterOut->SetAddress(fConfig->moduleAddrs[module]);
      clusterOut->SetProperties(clusterGpu.fCharge, clusterGpu.fPosition, clusterGpu.fPositionError, cidx.fTime,
                                clusterGpu.fTimeError);
      clusterOut->SetSize(clusterGpu.fSize);
    }

    auto* gpuClusterB    = &hfc.clusterDataPerModule.h()[(module + hfc.nModules) * hfc.maxClustersPerModule];
    auto* gpuClusterIdxB = &hfc.clusterIdxPerModule.h()[(module + hfc.nModules) * hfc.maxClustersPerModule];
    int nClustersBGpu    = hfc.nClustersPerModule.h()[module + hfc.nModules];

    for (int i = 0; i < nClustersBGpu; i++) {
      auto& cidx          = gpuClusterIdxB[i];
      auto& clusterGpu    = gpuClusterB[cidx.fIdx];
      unsigned int outIdx = clusters->GetEntriesFast();
      auto* clusterOut    = new ((*clusters)[outIdx])::CbmStsCluster {};

      clusterOut->SetAddress(fConfig->moduleAddrs[module]);
      clusterOut->SetProperties(clusterGpu.fCharge, clusterGpu.fPosition, clusterGpu.fPositionError, cidx.fTime,
                                clusterGpu.fTimeError);
      clusterOut->SetSize(clusterGpu.fSize);
    }

    auto* gpuHits = &hfc.hitsPerModule.h()[module * hfc.maxHitsPerModule];
    int nHitsGpu  = hfc.nHitsPerModule.h()[module];

    for (int i = 0; i < nHitsGpu; i++) {
      auto& hitGpu = gpuHits[i];

      unsigned int outIdx = hits->GetEntriesFast();
      new ((*hits)[outIdx])::CbmStsHit {fConfig->moduleAddrs[module],
                                        TVector3 {hitGpu.fX, hitGpu.fY, hitGpu.fZ},
                                        TVector3 {hitGpu.fDx, hitGpu.fDy, hitGpu.fDz},
                                        hitGpu.fDxy,
                                        0,
                                        0,
                                        double(hitGpu.fTime),
                                        hitGpu.fTimeError,
                                        hitGpu.fDu,
                                        hitGpu.fDv};
    }

  }  // for (int module = 0; module < hfc.nModules; module++)
}

void CbmGpuRecoSts::FetchDigis(size_t& maxDigisPerModule, size_t& nDigisTotal)
{

  // FIXME: This function is a crime against humanity.
  // Digis should be presorted by module already...

  CbmDigiManager* digis = CbmDigiManager::Instance();
  auto& hfc             = fHitfinderCpu;

  // FIXME: GPU reco should use regular digi class too
  nDigis      = digis->GetNofDigis(ECbmModuleId::kSts);
  nDigisTotal = 0;
  for (size_t i = 0; i < nDigis; i++) {
    const ::CbmStsDigi* digi = digis->Get<::CbmStsDigi>(i);
    Int_t systemId           = CbmAddress::GetSystemId(digi->GetAddress());
    // TODO: is this check still needed?
    if (systemId != ToIntegralType(ECbmModuleId::kSts)) { continue; }
    int moduleAddr = CbmStsAddress::GetMotherAddress(digi->GetAddress(), kStsModule);
    // int module     = addrToModuleId(moduleAddr);
    // assert(module < hfc.nModules);
    bool isFront = digi->GetChannel() < hfc.nChannels;

    if (isFront) { fDigisByModuleF[moduleAddr].push_back(*digi); }
    else {
      CbmStsDigi tmpdigi = *digi;
      tmpdigi.SetChannel(digi->GetChannel() - hfc.nChannels);
      fDigisByModuleB[moduleAddr].push_back(tmpdigi);
    }

    nDigisTotal++;
  }

  nDigisUsed = nDigisTotal;

  maxDigisPerModule = 0;
  for (const auto& mod : fDigisByModuleF) {
    maxDigisPerModule = std::max(maxDigisPerModule, mod.second.size());
  }

  for (const auto& mod : fDigisByModuleB) {
    maxDigisPerModule = std::max(maxDigisPerModule, mod.second.size());
  }
}
