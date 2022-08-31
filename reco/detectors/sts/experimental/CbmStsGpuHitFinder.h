/* Copyright (C) 2021-2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#ifndef CBMSTSGPUHITFINDER_H
#define CBMSTSGPUHITFINDER_H

#include "CbmStsDigi.h"
#include "CbmStsGpuRecoDevice.h"

#include <xpu/device.h>

// experimental namespace to avoid name collisions with original data types
namespace experimental
{
  using CbmStsTimeNs = uint32_t;

  using XErrorT = float;  // FIXME: remove and replace by float

  struct CbmStsClusterIdx {
    CbmStsTimeNs fTime;  ///< Cluster time (average of digi times) [ns]
    int fIdx;
  };

  struct CbmStsClusterData {
    float fCharge;           ///< Total charge
    int fSize;               ///< Difference between first and last channel
    float fPosition;         ///< Cluster centre in channel number units
    XErrorT fPositionError;  ///< Cluster centre error (r.m.s.) in channel number units
    float fTimeError;        ///< Error of cluster time [ns]
  };

  struct CbmStsHit {
    float fX, fY;        ///< X, Y positions of hit [cm]
    float fZ;            ///< Z position of hit [cm]
    CbmStsTimeNs fTime;  ///< Hit time [ns]
    float fDx, fDy;      ///< X, Y errors [cm]
    float fDz;           ///< Z position error [cm]
    float fDxy;          ///< XY correlation
    float fTimeError;    ///< Error of hit time [ns]
    float fDu;           ///< Error of coordinate across front-side strips [cm]
    float fDv;           ///< Error of coordinate across back-side strips [cm]
  };

  struct CbmStsParAsic {
    int nAdc;
    float dynRange;
    float threshold;
    float timeResolution;
    float deadTime;
    float noise;
    float zeroNoiseRate;

    XPU_D float AdcToCharge(unsigned short adc) const
    {
      return threshold + dynRange / float(nAdc) * (float(adc) + 0.5f);
    }
  };

  struct CbmStsHitFinderConfig {
    float dY;
    float pitch;
    float stereoF;
    float stereoB;
    float lorentzF;
    float lorentzB;
  };

  // cache of various parameters used by the hit finder
  struct CbmStsHitFinderParams : public CbmStsHitFinderConfig {
    float tanStereoF;
    float tanStereoB;
    float errorFac;
    float dX;
  };

  using SortDigisT = xpu::block_sort<unsigned long int, ::CbmStsDigi, xpu::block_size<SortDigis>::value.x,
                                     CBM_STS_SORT_DIGIS_ITEMS_PER_THREAD>;

  struct CbmStsSortDigiSmem {
    typename SortDigisT::storage_t sortBuf;
  };

  using SortClustersT = xpu::block_sort<CbmStsTimeNs, CbmStsClusterIdx, xpu::block_size<SortClusters>::value.x,
                                        CBM_STS_SORT_CLUSTERS_ITEMS_PER_THREAD>;

  struct CbmStsSortClusterSmem {
    typename SortClustersT::storage_t sortBuf;
  };

  struct CbmStsClusterCalculationProperties {
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

  class CbmStsDigiConnector {
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

  template<xpu::side S>
  class CbmStsHitFinder {

  public:
    // Constants
    int nModules;
    int nChannels;

    // calibration data
    float timeCutDigiAbs;
    float timeCutDigiSig;
    float timeCutClusterAbs;
    float timeCutClusterSig;
    CbmStsParAsic asic;
    int landauTableSize;
    float landauStepSize;
    xpu::cmem_io_t<float, S> landauTable;

    xpu::cmem_device_t<float, S> maxErrorFront;
    xpu::cmem_device_t<float, S> maxErrorBack;

    // TODO fill this array
    xpu::cmem_io_t<CbmStsHitFinderConfig, S> hitfinderConfig;

    // input
    // Store all digis in a flat array with a header that contains the offset for every module (front and back)
    xpu::cmem_io_t<size_t, S> digiOffsetPerModule;  // 2 * nModules + 1 entries, last entry contains total digi count
    xpu::cmem_io_t<CbmStsDigi, S> digisPerModule;   // TODO this is a flat array now, should be renamed
    // Two digi arrays needed, as sorting can't sort in place right now
    xpu::cmem_device_t<CbmStsDigi, S> digisPerModuleTmp;
    xpu::cmem_device_t<CbmStsDigi*, S> digisSortedPerModule;

    xpu::cmem_io_t<float, S> localToGlobalTranslationByModule;
    xpu::cmem_io_t<float, S> localToGlobalRotationByModule;

    xpu::cmem_device_t<CbmStsDigiConnector, S> digiConnectorPerModule;
    xpu::cmem_device_t<unsigned int, S> channelOffsetPerModule;

    // intermediate results
    size_t maxClustersPerModule;
    xpu::cmem_device_t<int, S> channelStatusPerModule;
    xpu::cmem_io_t<CbmStsClusterIdx, S> clusterIdxPerModule;
    xpu::cmem_io_t<CbmStsClusterIdx, S> clusterIdxPerModuleTmp;
    xpu::cmem_io_t<CbmStsClusterIdx*, S> clusterIdxSortedPerModule;
    xpu::cmem_io_t<CbmStsClusterData, S> clusterDataPerModule;
    xpu::cmem_io_t<int, S> nClustersPerModule;

    // output
    size_t maxHitsPerModule;
    xpu::cmem_io_t<CbmStsHit, S> hitsPerModule;
    xpu::cmem_io_t<int, S> nHitsPerModule;
  };

  class CbmStsGpuHitFinder : public CbmStsHitFinder<xpu::side::device> {

  public:
    XPU_D void SortDigisInSpaceAndTime(CbmStsSortDigiSmem& smem, int iBlock) const;
    XPU_D void FindClustersSingleStep(int iBlock) const;
    XPU_D void SortClusters(CbmStsSortClusterSmem& smem, int iBlock) const;
    XPU_D void FindHits(int iBlock) const;

    // Individual steps of cluster finder, for more granular time measurement
    XPU_D void CalculateOffsetsParallel(int iBlock) const;
    XPU_D void FindClustersParallel(int iBlock) const;
    XPU_D void CalculateClustersParallel(int iBlock) const;

    XPU_D void FindClusterConnectionsBasic(int iBlock) const;
    XPU_D void CalculateClustersBasic(int iBlock) const;

  private:
    XPU_D void CalculateChannelOffsets(CbmStsDigi* digis, unsigned int* channelOffsets, unsigned int nDigis) const;

    XPU_D void FindClusterConnectionsChannelWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                                 unsigned int* channelOffsets, int iModule,
                                                 unsigned int threadId) const;
    XPU_D void FindClusterConnectionsDigiWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                              unsigned int* channelOffsets, int iModule, unsigned int threadId,
                                              unsigned int nDigis) const;

    XPU_D void CalculateClustersBasic(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector, int iModule,
                                      unsigned int const threadId) const;
    XPU_D void CalculateClustersChannelWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                            unsigned int* channelOffsets, int iModule, unsigned int const threadId,
                                            unsigned int const nDigis) const;
    XPU_D void CalculateClustersDigiWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector, int iModule,
                                         unsigned int const threadId, unsigned int const nDigis) const;

    XPU_D void CreateClusterFromConnectors1(int const iModule, CbmStsDigi* digis, int const digiIndex) const;
    XPU_D void CreateClusterFromConnectors2(int const iModule, CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                            int const digiIndex) const;
    XPU_D void CreateClusterFromConnectorsN(int const iModule, CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
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

    XPU_D void AddCluster(int iModule, CbmStsTimeNs time, const CbmStsClusterData& cls) const
    {
      CbmStsClusterIdx* tgtIdx   = &clusterIdxPerModule[iModule * maxClustersPerModule];
      CbmStsClusterData* tgtData = &clusterDataPerModule[iModule * maxClustersPerModule];

      int pos = xpu::atomic_add_block(&nClustersPerModule[iModule], 1);
      XPU_ASSERT(size_t(pos) < maxClustersPerModule);

      CbmStsClusterIdx idx {time, pos};
      tgtIdx[idx.fIdx]  = idx;
      tgtData[idx.fIdx] = cls;
    }

    XPU_D bool IsBackside(int iModule) const { return iModule >= nModules; }

    XPU_D XErrorT LandauWidth(float charge) const;

    XPU_D void ToGlobal(int iModule, float lx, float ly, float lz, float& gx, float& gy, float& gz) const;

    XPU_D void IntersectClusters(int iBlock, const CbmStsHitFinderParams& pars, CbmStsTimeNs timeF,
                                 const CbmStsClusterData& clsF, CbmStsTimeNs timeB,
                                 const CbmStsClusterData& clsB) const;
    XPU_D float GetClusterPosition(const CbmStsHitFinderParams& pars, float centre, bool isFront) const;
    XPU_D bool Intersect(const CbmStsHitFinderParams& pars, float xF, float exF, float xB, float exB, float& x,
                         float& y, float& varX, float& varY, float& varXY) const;
    XPU_D bool IsInside(const CbmStsHitFinderParams& pars, float x, float y) const;
    XPU_D void CreateHit(int iBlocks, float xLocal, float yLocal, float varX, float varY, float varXY,
                         CbmStsTimeNs timeF, const CbmStsClusterData& clsF, CbmStsTimeNs timeB,
                         const CbmStsClusterData& clsB, float du, float dv) const;

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

}  // namespace experimental

XPU_EXPORT_CONSTANT(CbmStsGpuRecoDevice, ::experimental::CbmStsGpuHitFinder, HitFinder);

#endif
