/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#ifndef CBM_ALGO_STS_HITFINDER_H
#define CBM_ALGO_STS_HITFINDER_H

#include "CbmStsDigi.h"

#include <xpu/device.h>

namespace cbm::algo
{
  struct GpuOnline {
  };  // xpu image identifier, has to be moved when more steps are moved to GPU

  // Block sizes / other compile time constants that need tuning
  enum GpuConstants
  {
#if XPU_IS_CUDA
    kSortDigisBlockSize         = 32,
    kSortDigisItemsPerThread    = 32,
    kSortClustersBlockSize      = 32,
    kSortClustersItemsPerThread = 32,
    kFindClusterBlockSize       = 32,
    kFindHitsBlockSize          = 32,
#else  // HIP, values ignored on CPU
    kSortDigisBlockSize         = 256,
    kSortDigisItemsPerThread    = 6,
    kSortClustersBlockSize      = 256,
    kSortClustersItemsPerThread = 6,
    kFindClusterBlockSize       = 256,
    kFindHitsBlockSize          = 256,
#endif
  };

  // Kernel declarations
  XPU_EXPORT_KERNEL(GpuOnline, SortDigis);
  // Combine substeps for finding clusters into a single kernel
  XPU_EXPORT_KERNEL(GpuOnline, FindClustersSingleStep);
  XPU_EXPORT_KERNEL(GpuOnline, CalculateOffsets);
  XPU_EXPORT_KERNEL(GpuOnline, FindClusters);
  XPU_EXPORT_KERNEL(GpuOnline, FindClustersBasic);
  XPU_EXPORT_KERNEL(GpuOnline, CalculateClusters);
  XPU_EXPORT_KERNEL(GpuOnline, CalculateClustersBasic);
  XPU_EXPORT_KERNEL(GpuOnline, SortClusters);
  XPU_EXPORT_KERNEL(GpuOnline, FindHits);

}  // namespace cbm::algo

// Set block sizes. Currently this needs to be done in toplevel namespace.
XPU_BLOCK_SIZE_1D(cbm::algo::SortClusters, cbm::algo::kSortClustersBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::FindClustersSingleStep, cbm::algo::kFindClusterBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::CalculateOffsets, cbm::algo::kFindClusterBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::FindClusters, cbm::algo::kFindClusterBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::CalculateClusters, cbm::algo::kFindClusterBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::SortDigis, cbm::algo::kSortClustersBlockSize);
XPU_BLOCK_SIZE_1D(cbm::algo::FindHits, cbm::algo::kFindClusterBlockSize);

namespace cbm::algo
{
  // GPU Data structures
  // TODO: Replace with regular StsCluster / StsHit once the changes
  // land to make them fit for GPU
  struct StsGpuClusterIdx {
    uint32_t fTime;  ///< Cluster time (average of digi times) [ns]
    int fIdx;
  };

  struct StsGpuCluster {
    float fCharge;         ///< Total charge
    int fSize;             ///< Difference between first and last channel
    float fPosition;       ///< Cluster centre in channel number units
    float fPositionError;  ///< Cluster centre error (r.m.s.) in channel number units
    uint32_t fTime;        ///< cluster time [ns]
    float fTimeError;      ///< Error of cluster time [ns]
  };

  struct StsGpuHit {
    float fX, fY;      ///< X, Y positions of hit [cm]
    float fZ;          ///< Z position of hit [cm]
    uint32_t fTime;    ///< Hit time [ns]
    float fDx, fDy;    ///< X, Y errors [cm]
    float fDz;         ///< Z position error [cm]
    float fDxy;        ///< XY correlation
    float fTimeError;  ///< Error of hit time [ns]
    float fDu;         ///< Error of coordinate across front-side strips [cm]
    float fDv;         ///< Error of coordinate across back-side strips [cm]
  };

  // Calibration data structures

  struct StsAsicPar {
    int nAdc;
    float dynamicRange;
    float threshold;
    float timeResolution;
    float deadTime;
    float noise;
    float zeroNoiseRate;

    XPU_D float AdcToCharge(unsigned short adc) const
    {
      return threshold + dynamicRange / float(nAdc) * (float(adc) + 0.5f);
    }
  };

  struct StsSensorPar {
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
  struct StsHitfinderCache : StsSensorPar {
    float tanStereoF;
    float tanStereoB;
    float errorFac;
    float dX;
  };

  struct StsClusterCalculationProperties {
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

  class StsDigiConnector {
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
        old     = xpu::atomic_cas_block(&hasPreviousAndNext, assumed, UnpackNext(assumed) | hasPreviousI);
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
        old     = xpu::atomic_cas_block(&hasPreviousAndNext, assumed, (assumed & (1u << 31)) | next);
      } while (old != assumed);
    }

    XPU_D bool HasNext() const { return UnpackNext(hasPreviousAndNext) != 0; }
  };

  // Shared Memory defintions
  using SortDigisT =
    xpu::block_sort<unsigned long int, ::CbmStsDigi, xpu::block_size<SortDigis>::value.x, kSortDigisItemsPerThread>;

  struct StsSortDigiSmem {
    typename SortDigisT::storage_t sortBuf;
  };

  using SortClustersT =
    xpu::block_sort<uint32_t, StsGpuClusterIdx, xpu::block_size<SortClusters>::value.x, kSortClustersItemsPerThread>;

  struct StsSortClusterSmem {
    typename SortClustersT::storage_t sortBuf;
  };

  // Hitfinder buffers
  template<xpu::side S>
  class StsHitfinderBase {

  public:
    // calibration / configuration data
    // Only set once

    int nModules;   // Number of modules
    int nChannels;  // Number of channels per module

    // Time cuts for Digis / Clusters
    // TODO: currently not used!
    float timeCutDigiAbs;
    float timeCutDigiSig;
    float timeCutClusterAbs;
    float timeCutClusterSig;

    StsAsicPar asic;

    int landauTableSize;
    float landauStepSize;
    // Entries of landauTable. size = landauTableSize
    xpu::cmem_device_t<float, S> landauTable;

    // transformation matrix to convert local to global coordinate space
    // size = nModules
    xpu::cmem_io_t<float, S> localToGlobalTranslationByModule;
    xpu::cmem_io_t<float, S> localToGlobalRotationByModule;

    // Sensor Parameters
    // size = nModules
    xpu::cmem_io_t<StsSensorPar, S> sensorPars;

    // input
    // Store all digis in a flat array with a header that contains the offset for every module (front and back)
    xpu::cmem_io_t<size_t, S>
      digiOffsetPerModule;  // size = 2 * nModules + 1 entries, last entry contains total digi count
    xpu::cmem_io_t<CbmStsDigi, S> digisPerModule;  // Flat array of input digis. size = nDigisTotal

    // Temporary Digi-Array used for sorting as sorting is not in place. size = nDigisTotal
    xpu::cmem_device_t<CbmStsDigi, S> digisPerModuleTmp;

    // Pointer to sorted of a module side. Points either to digisPerModule or digisPerModuleTmp
    // size = 2 * nModules
    xpu::cmem_device_t<CbmStsDigi*, S> digisSortedPerModule;

    // DigiConnectors used internally by StsClusterizer
    // Connects digis that belong to the same cluster via linked-list.
    // size = nDigisTotal
    xpu::cmem_device_t<StsDigiConnector, S> digiConnectorsPerModule;

    // Digis are sorted by channel + within channel by time
    // Contains the offset for each channel within each channel
    // size = 2 * nModules * nChannels
    xpu::cmem_device_t<unsigned int, S> channelOffsetPerModule;

    // intermediate results
    size_t maxClustersPerModule;

    // FIXME: Not used anymore?
    xpu::cmem_device_t<int, S> channelStatusPerModule;

    // Cluster Index by module. Produced by cluster finder.
    // Stored as buckets with a max size of maxClustersPerModule
    // Actual number of entries in each bucket is stored in nClustersPerModule
    // size = 2 * nModules * maxClustersPerModule
    xpu::cmem_io_t<StsGpuClusterIdx, S> clusterIdxPerModule;

    // Temporary cluster index array used for sorting as sorting is not in place.
    // size =  2 * nModules * maxClustersPerModule
    xpu::cmem_io_t<StsGpuClusterIdx, S> clusterIdxPerModuleTmp;

    // Pointer to sorted cluster idx for every module side.
    // Either points to clusterIdxPerModule or clusterIdxPerModuleTmp.
    // size = 2 * nModules
    xpu::cmem_io_t<StsGpuClusterIdx*, S> clusterIdxSortedPerModule;

    // Clusters stored by modules. Stored as buckets with a max size of maxClustersPerModule
    // Actual number of entries in each bucket is stored in nClustersPerModule
    // size = 2 * nModules * maxClustersPerModule
    xpu::cmem_io_t<StsGpuCluster, S> clusterDataPerModule;

    // Number of clusters in each module
    // size = 2 * nModules
    xpu::cmem_io_t<int, S> nClustersPerModule;

    // Max time error of clusters on front- and backside of a module
    // size = 1 (???)
    // FIXME: size should be 2 * nModules? And only one array!
    xpu::cmem_device_t<float, S> maxErrorFront;
    xpu::cmem_device_t<float, S> maxErrorBack;

    // output

    // Max number of Hits that can be stored in each module
    size_t maxHitsPerModule;

    // Hits sorted by module. Stored in buckets of size maxHitsPerModule.
    // Actual number of hits is stored in nHitsPerModule
    // size = maxHitsPerModule * nModules
    xpu::cmem_io_t<StsGpuHit, S> hitsPerModule;

    // Number of hits in each module
    // size = nModules
    xpu::cmem_io_t<int, S> nHitsPerModule;
  };

  // Host-side hitfinder. Only holds memory buffers.
  using StsHitfinderHost = StsHitfinderBase<xpu::side::host>;

  // Device-side hitfinder. Holds pointers to buffers + algorithm.
  class StsHitfinderGpu : public StsHitfinderBase<xpu::side::device> {

  public:
    XPU_D void SortDigisInSpaceAndTime(StsSortDigiSmem& smem, int iBlock) const;
    XPU_D void FindClustersSingleStep(int iBlock) const;
    XPU_D void SortClusters(StsSortClusterSmem& smem, int iBlock) const;
    XPU_D void FindHits(int iBlock) const;

    // Individual steps of cluster finder, for more granular time measurement
    XPU_D void CalculateOffsetsParallel(int iBlock) const;
    XPU_D void FindClustersParallel(int iBlock) const;
    XPU_D void CalculateClustersParallel(int iBlock) const;

    XPU_D void FindClusterConnectionsBasic(int iBlock) const;
    XPU_D void CalculateClustersBasic(int iBlock) const;

  private:
    XPU_D void CalculateChannelOffsets(CbmStsDigi* digis, unsigned int* channelOffsets, unsigned int nDigis) const;

    XPU_D void FindClusterConnectionsChannelWise(CbmStsDigi* digis, StsDigiConnector* digiConnector,
                                                 unsigned int* channelOffsets, int iModule,
                                                 unsigned int threadId) const;
    XPU_D void FindClusterConnectionsDigiWise(CbmStsDigi* digis, StsDigiConnector* digiConnector,
                                              unsigned int* channelOffsets, int iModule, unsigned int threadId,
                                              unsigned int nDigis) const;

    XPU_D void CalculateClustersBasic(CbmStsDigi* digis, StsDigiConnector* digiConnector, int iModule,
                                      unsigned int const threadId) const;
    XPU_D void CalculateClustersChannelWise(CbmStsDigi* digis, StsDigiConnector* digiConnector,
                                            unsigned int* channelOffsets, int iModule, unsigned int const threadId,
                                            unsigned int const nDigis) const;
    XPU_D void CalculateClustersDigiWise(CbmStsDigi* digis, StsDigiConnector* digiConnector, int iModule,
                                         unsigned int const threadId, unsigned int const nDigis) const;

    XPU_D void CreateClusterFromConnectors1(int const iModule, CbmStsDigi* digis, int const digiIndex) const;
    XPU_D void CreateClusterFromConnectors2(int const iModule, CbmStsDigi* digis, StsDigiConnector* digiConnector,
                                            int const digiIndex) const;
    XPU_D void CreateClusterFromConnectorsN(int const iModule, CbmStsDigi* digis, StsDigiConnector* digiConnector,
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

    XPU_D void AddCluster(int iModule, uint32_t time, const StsGpuCluster& cls) const
    {
      StsGpuClusterIdx* tgtIdx = &clusterIdxPerModule[iModule * maxClustersPerModule];
      StsGpuCluster* tgtData   = &clusterDataPerModule[iModule * maxClustersPerModule];

      int pos = xpu::atomic_add_block(&nClustersPerModule[iModule], 1);
      XPU_ASSERT(size_t(pos) < maxClustersPerModule);

      StsGpuClusterIdx idx {time, pos};
      tgtIdx[idx.fIdx]  = idx;
      tgtData[idx.fIdx] = cls;
    }

    XPU_D bool IsBackside(int iModule) const { return iModule >= nModules; }

    XPU_D float LandauWidth(float charge) const;

    XPU_D void ToGlobal(int iModule, float lx, float ly, float lz, float& gx, float& gy, float& gz) const;

    XPU_D void IntersectClusters(int iBlock, const StsHitfinderCache& pars, uint32_t timeF, const StsGpuCluster& clsF,
                                 uint32_t timeB, const StsGpuCluster& clsB) const;
    XPU_D float GetClusterPosition(const StsHitfinderCache& pars, float centre, bool isFront) const;
    XPU_D bool Intersect(const StsHitfinderCache& pars, float xF, float exF, float xB, float exB, float& x, float& y,
                         float& varX, float& varY, float& varXY) const;
    XPU_D bool IsInside(const StsHitfinderCache& pars, float x, float y) const;
    XPU_D void CreateHit(int iBlocks, float xLocal, float yLocal, float varX, float varY, float varXY, uint32_t timeF,
                         const StsGpuCluster& clsF, uint32_t timeB, const StsGpuCluster& clsB, float du,
                         float dv) const;

    XPU_D void SaveMaxError(float errorValue, int iModule) const
    {
      float* maxError = IsBackside(iModule) ? maxErrorBack : maxErrorFront;
      float old {};
      do {
        old = *maxError;
        if (old >= errorValue) { break; }
      } while (!xpu::atomic_cas_block(maxError, *maxError, xpu::max(errorValue, *maxError)));
    }
  };

  // StsHitfinder lives in constant memory
  XPU_EXPORT_CONSTANT(GpuOnline, StsHitfinderGpu, TheStsHitfinder);

}  // namespace cbm::algo

#endif  // #ifndef CBM_ALGO_STS_HITFINDER_H
