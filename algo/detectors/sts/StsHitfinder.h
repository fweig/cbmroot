/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#ifndef CBM_ALGO_STS_HITFINDER_H
#define CBM_ALGO_STS_HITFINDER_H

#include "CbmStsDigi.h"

#include <xpu/device.h>

#include "Prelude.h"
#include "gpu/DeviceImage.h"
#include "gpu/Params.h"
#include "sts/Cluster.h"
#include "sts/Hit.h"
#include "sts/HitfinderPars.h"

namespace cbm::algo
{

  // Block sizes / other compile time constants that need tuning
  enum GpuConstants
  {
#if XPU_IS_CUDA
    kSortDigisBlockSize         = 512,
    kSortDigisItemsPerThread    = 8,
    kSortClustersBlockSize      = 512,
    kSortClustersItemsPerThread = 8,
    kFindClusterBlockSize       = 1024,
    kFindHitsBlockSize          = 512,
#else  // HIP, values ignored on CPU
    kSortDigisBlockSize         = 256,
    kSortDigisItemsPerThread    = 6,
    kSortClustersBlockSize      = 256,
    kSortClustersItemsPerThread = 6,
    kFindClusterBlockSize       = 1024,
    kFindHitsBlockSize          = 1024,
#endif
  };
}  // namespace cbm::algo

namespace cbm::algo::sts
{

  class Hitfinder;  // forward declaration

  // GPU Data structures
  // TODO: Replace with regular StsCluster / StsHit once the changes
  // land to make them fit for GPU
  struct GpuClusterIdx {
    uint32_t fTime;  ///< Cluster time (average of digi times) [ns]
    int fIdx;
  };

  // Declare Constant Memory
  struct TheHitfinder : xpu::constant<GPUReco, Hitfinder> {
  };

  // Declare Kernels
  struct SortDigis : xpu::kernel<GPUReco> {
    using block_size    = xpu::block_size<kSortDigisBlockSize>;
    using sort_t        = xpu::block_sort<u64, CbmStsDigi, kSortDigisBlockSize, kSortDigisItemsPerThread>;
    using shared_memory = sort_t::storage_t;
    using constants     = xpu::cmem<TheHitfinder, Params>;
    using context       = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct FindClusters : xpu::kernel<GPUReco> {
    using block_size = xpu::block_size<kFindClusterBlockSize>;
    using constants  = xpu::cmem<TheHitfinder, Params>;
    using context    = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct ChannelOffsets : xpu::kernel<GPUReco> {
    using block_size = xpu::block_size<kFindClusterBlockSize>;
    using constants  = xpu::cmem<TheHitfinder, Params>;
    using context    = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct CreateDigiConnections : xpu::kernel<GPUReco> {
    using block_size = xpu::block_size<kFindClusterBlockSize>;
    using constants  = xpu::cmem<TheHitfinder, Params>;
    using context    = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct CreateClusters : xpu::kernel<GPUReco> {
    using block_size = xpu::block_size<kFindClusterBlockSize>;
    using constants  = xpu::cmem<TheHitfinder, Params>;
    using context    = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct SortClusters : xpu::kernel<GPUReco> {
    using block_size    = xpu::block_size<kSortClustersBlockSize>;
    using sort_t        = xpu::block_sort<u32, GpuClusterIdx, kSortClustersBlockSize, kSortClustersItemsPerThread>;
    using shared_memory = sort_t::storage_t;
    using constants     = xpu::cmem<TheHitfinder, Params>;
    using context       = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  struct FindHits : xpu::kernel<GPUReco> {
    using block_size = xpu::block_size<kFindHitsBlockSize>;
    using constants  = xpu::cmem<TheHitfinder, Params>;
    using context    = xpu::kernel_context<shared_memory, constants>;
    XPU_D void operator()(context&);
  };

  // Calibration data structures
  struct SensorPar {
    float dY;
    float pitch;
    float stereoF;
    float stereoB;
    float lorentzF;
    float lorentzB;
  };

  // Cache for various parameters of the hitfindiner
  // Used internally to avoid computing values multiple times
  // TODO: is this really needed? Overhead from additional computations should be miniscule.
  // TODO: Also store in shared memory, not registers. Values identical for all threads.
  struct HitfinderCache : SensorPar {
    float tanStereoF;
    float tanStereoB;
    float errorFac;
    float dX;
  };

  struct ClusterCalculationProperties {
    float tSum      = 0.;       // sum of digi times
    int chanF       = 9999999;  // first channel in cluster
    int chanL       = -1;       // last channel in cluster
    float qF        = 0.f;      // charge in first channel
    float qM        = 0.f;      // sum of charges in middle channels
    float qL        = 0.f;      // charge in last cluster
    float eqFsq     = 0.f;      // uncertainty of qF
    float eqMsq     = 0.f;      // uncertainty of qMid
    float eqLsq     = 0.f;      // uncertainty of qL
    float tResolSum = 0.f;

    float xSum = 0.f;  // weighted charge sum, used to correct corrupt clusters
  };

  class DigiConnector {
  private:
    unsigned int hasPreviousAndNext;

    XPU_D unsigned int UnpackNext(unsigned int val) const { return val & ~(1u << 31); }

  public:
    XPU_D bool HasPrevious() const { return (hasPreviousAndNext >> 31) & 1u; }

    XPU_D void SetHasPrevious(bool hasPrevious)
    {
      unsigned int old          = hasPreviousAndNext;
      unsigned int hasPreviousI = ((unsigned int) hasPrevious) << 31;
      unsigned int assumed;

      do {
        assumed = old;
        old     = xpu::atomic_cas(&hasPreviousAndNext, assumed, UnpackNext(assumed) | hasPreviousI);
      } while (old != assumed);
    }

    XPU_D unsigned int next() const { return UnpackNext(hasPreviousAndNext); }

    XPU_D void SetNext(unsigned int next)
    {
      unsigned int old = hasPreviousAndNext;
      unsigned int assumed;

      next = xpu::min((1u << 31) - 1, next);

      do {
        assumed = old;
        old     = xpu::atomic_cas(&hasPreviousAndNext, assumed, (assumed & (1u << 31)) | next);
      } while (old != assumed);
    }

    XPU_D bool HasNext() const { return UnpackNext(hasPreviousAndNext) != 0; }
  };

  // Hitfinder class. Holds pointers to buffers + algorithm.
  class Hitfinder {

  public:
    // calibration / configuration data
    // Only set once

    int nModules;   // Number of modules
    int nChannels;  // Number of channels per module

    sts::HitfinderPars::Asic asic;

    int landauTableSize;
    float landauStepSize;
    // Entries of landauTable. size = landauTableSize
    xpu::buffer<float> landauTable;

    // transformation matrix to convert local to global coordinate space
    // size = nModules
    xpu::buffer<float> localToGlobalTranslationByModule;
    xpu::buffer<float> localToGlobalRotationByModule;

    // Sensor Parameters
    // size = nModules
    xpu::buffer<SensorPar> sensorPars;

    // input
    // Store all digis in a flat array with a header that contains the offset for every module (front and back)
    xpu::buffer<size_t> digiOffsetPerModule;  // size = 2 * nModules + 1 entries, last entry contains total digi count
    xpu::buffer<CbmStsDigi> digisPerModule;   // Flat array of input digis. size = nDigisTotal

    // Temporary Digi-Array used for sorting as sorting is not in place. size = nDigisTotal
    xpu::buffer<CbmStsDigi> digisPerModuleTmp;

    // DigiConnectors used internally by StsClusterizer
    // Connects digis that belong to the same cluster via linked-list.
    // size = nDigisTotal
    xpu::buffer<DigiConnector> digiConnectorsPerModule;

    // Digis are sorted by channel + within channel by time
    // Contains the offset for each channel within each channel
    // size = 2 * nModules * nChannels
    xpu::buffer<unsigned int> channelOffsetPerModule;

    // intermediate results
    size_t maxClustersPerModule;

    // Cluster Index by module. Produced by cluster finder.
    // Stored as buckets with a max size of maxClustersPerModule
    // Actual number of entries in each bucket is stored in nClustersPerModule
    // size = 2 * nModules * maxClustersPerModule
    xpu::buffer<GpuClusterIdx> clusterIdxPerModule;

    // Temporary cluster index array used for sorting as sorting is not in place.
    // size =  2 * nModules * maxClustersPerModule
    xpu::buffer<GpuClusterIdx> clusterIdxPerModuleTmp;

    // Pointer to sorted cluster idx for every module side.
    // Either points to clusterIdxPerModule or clusterIdxPerModuleTmp.
    // size = 2 * nModules
    xpu::buffer<GpuClusterIdx*> clusterIdxSortedPerModule;

    // Clusters stored by modules. Stored as buckets with a max size of maxClustersPerModule
    // Actual number of entries in each bucket is stored in nClustersPerModule
    // size = 2 * nModules * maxClustersPerModule
    xpu::buffer<sts::Cluster> clusterDataPerModule;

    // Number of clusters in each module
    // size = 2 * nModules
    xpu::buffer<int> nClustersPerModule;

    // Max time error of clusters on front- and backside of a module
    // size = 1 (???)
    // FIXME: size should be 2 * nModules? And only one array!
    xpu::buffer<float> maxClusterTimeErrorByModuleSide;

    // output

    // Max number of Hits that can be stored in each module
    size_t maxHitsPerModule;

    // Hits sorted by module. Stored in buckets of size maxHitsPerModule.
    // Actual number of hits is stored in nHitsPerModule
    // size = maxHitsPerModule * nModules
    xpu::buffer<sts::Hit> hitsPerModule;

    // Number of hits in each module
    // size = nModules
    xpu::buffer<int> nHitsPerModule;

  public:
    XPU_D void SortDigisInSpaceAndTime(SortDigis::context&) const;
    XPU_D void FindClustersSingleStep(FindClusters::context&) const;
    XPU_D void SortClusters(SortClusters::context&) const;
    XPU_D void FindHits(FindHits::context&) const;

    // Individual steps of cluster finder, for more granular time measurement
    XPU_D void CalculateOffsetsParallel(FindClusters::context&) const;
    XPU_D void FindClustersParallel(FindClusters::context&) const;
    XPU_D void CalculateClustersParallel(FindClusters::context&) const;

  private:
    XPU_D void CalculateChannelOffsets(FindClusters::context& ctx, CbmStsDigi* digis, unsigned int* channelOffsets,
                                       unsigned int nDigis) const;

    XPU_D void FindClusterConnectionsDigiWise(FindClusters::context& ctx, CbmStsDigi* digis,
                                              DigiConnector* digiConnector, unsigned int* channelOffsets,
                                              unsigned int nDigis) const;

    XPU_D void CalculateClustersDigiWise(FindClusters::context& ctx, CbmStsDigi* digis, DigiConnector* digiConnector,
                                         unsigned int const nDigis) const;

    XPU_D void CreateClusterFromConnectors1(int const iModule, CbmStsDigi* digis, int const digiIndex) const;
    XPU_D void CreateClusterFromConnectors2(int const iModule, CbmStsDigi* digis, DigiConnector* digiConnector,
                                            int const digiIndex) const;
    XPU_D void CreateClusterFromConnectorsN(int const iModule, CbmStsDigi* digis, DigiConnector* digiConnector,
                                            int const digiIndex) const;

  private:
    XPU_D unsigned int GetNDigis(int iModule) const
    {
      return digiOffsetPerModule[iModule + 1] - digiOffsetPerModule[iModule];
    }

    XPU_D float GetTimeResolution(int /* iModule */, int /* channel */) const { return asic.timeResolution; }

    XPU_D bool IsActive(int* channelStatus, int channel) const
    {
      // TODO add padding to channels to remove this?
      if (channel < 0 || channel >= nChannels) { return false; }
      return channelStatus[channel] > -1;
    }

    XPU_D int ChanLeft(int channel) const { return channel - 1; }

    XPU_D int ChanRight(int channel) const { return channel + 1; }

    XPU_D int ChanDist(int c1, int c2) const
    {
      int diff = c2 - c1;
      // TODO handle wrap around
      return diff;
    }

    XPU_D void AddCluster(int iModule, uint32_t time, const sts::Cluster& cls) const
    {
      GpuClusterIdx* tgtIdx = &clusterIdxPerModule[iModule * maxClustersPerModule];
      sts::Cluster* tgtData = &clusterDataPerModule[iModule * maxClustersPerModule];

      int pos = xpu::atomic_add_block(&nClustersPerModule[iModule], 1);
      XPU_ASSERT(size_t(pos) < maxClustersPerModule);

      if (size_t(pos) < maxClustersPerModule) {
        GpuClusterIdx idx {time, pos};
        tgtIdx[idx.fIdx]  = idx;
        tgtData[idx.fIdx] = cls;
      }
    }

    XPU_D bool IsBackside(int iModule) const { return iModule >= nModules; }

    XPU_D float LandauWidth(float charge) const;

    XPU_D void ToGlobal(int iModule, float lx, float ly, float lz, float& gx, float& gy, float& gz) const;

    XPU_D void IntersectClusters(int iBlock, const HitfinderCache& pars, uint32_t timeF, const sts::Cluster& clsF,
                                 uint32_t timeB, const sts::Cluster& clsB) const;
    XPU_D float GetClusterPosition(const HitfinderCache& pars, float centre, bool isFront) const;
    XPU_D bool Intersect(const HitfinderCache& pars, float xF, float exF, float xB, float exB, float& x, float& y,
                         float& varX, float& varY, float& varXY) const;
    XPU_D bool IsInside(const HitfinderCache& pars, float x, float y) const;
    XPU_D void CreateHit(int iBlocks, float xLocal, float yLocal, float varX, float varY, float varXY, uint32_t timeF,
                         const sts::Cluster& clsF, uint32_t timeB, const sts::Cluster& clsB, float du, float dv) const;

    XPU_D void SaveMaxError(float errorValue, int iModule) const
    {
      float* maxError = &maxClusterTimeErrorByModuleSide[iModule];
      float old {};
      do {
        old = *maxError;
        if (old >= errorValue) { break; }
      } while (!xpu::atomic_cas_block(maxError, *maxError, xpu::max(errorValue, *maxError)));
    }
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_STS_HITFINDER_H
