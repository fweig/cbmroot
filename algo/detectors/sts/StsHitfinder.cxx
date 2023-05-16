/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */

#include "StsHitfinder.h"

using namespace cbm::algo;

// Export Constants
XPU_EXPORT(sts::TheHitfinder);


// Kernel Definitions
XPU_EXPORT(sts::SortDigis);
XPU_D void sts::SortDigis::operator()(context& ctx) { ctx.cmem<sts::TheHitfinder>().SortDigisInSpaceAndTime(ctx); }

XPU_EXPORT(sts::FindClusters);
XPU_D void sts::FindClusters::operator()(context& ctx) { ctx.cmem<sts::TheHitfinder>().FindClustersSingleStep(ctx); }

XPU_EXPORT(sts::ChannelOffsets);
XPU_D void sts::ChannelOffsets::operator()(context& ctx)
{
  ctx.cmem<sts::TheHitfinder>().CalculateOffsetsParallel(ctx);
}

XPU_EXPORT(sts::CreateDigiConnections);
XPU_D void sts::CreateDigiConnections::operator()(context& ctx)
{
  ctx.cmem<sts::TheHitfinder>().FindClustersParallel(ctx);
}

XPU_EXPORT(sts::CreateClusters);
XPU_D void sts::CreateClusters::operator()(context& ctx)
{
  ctx.cmem<sts::TheHitfinder>().CalculateClustersParallel(ctx);
}

XPU_EXPORT(sts::SortClusters);
XPU_D void sts::SortClusters::operator()(context& ctx) { ctx.cmem<sts::TheHitfinder>().SortClusters(ctx); }

XPU_EXPORT(sts::FindHits);
XPU_D void sts::FindHits::operator()(context& ctx) { ctx.cmem<sts::TheHitfinder>().FindHits(ctx); }

/**
  * sts::Hitfinder::sortDigisKhun
  * Sorts digis channelwise. Inside a channel all digis are sorted in time.
  *
  * @param smem is the shared memory it is worked in
  * @param iBlock is the Block/Module that is currenty running
  *
*/
XPU_D void sts::Hitfinder::SortDigisInSpaceAndTime(SortDigis::context& ctx) const
{
  int iModule          = ctx.block_idx_x();
  CbmStsDigi* digis    = &digisPerModule[digiOffsetPerModule[iModule]];
  CbmStsDigi* digisTmp = &digisPerModuleTmp[digiOffsetPerModule[iModule]];
  int nDigis           = GetNDigis(iModule);

  SortDigis::sort_t digiSort(ctx.pos(), ctx.smem());

  CbmStsDigi* digisOut = digiSort.sort(digis, nDigis, digisTmp, [](const CbmStsDigi a) {
    return ((unsigned long int) a.GetChannel()) << 32 | (unsigned long int) (a.GetTimeU32());
  });

  if (digisOut == digisTmp) {
    // Copy back to digis
    for (int i = ctx.thread_idx_x(); i < nDigis; i += ctx.block_dim_x()) {
      digis[i] = digisTmp[i];
    }
  }
}

XPU_D void sts::Hitfinder::FindClustersSingleStep(FindClusters::context& ctx) const
{
  CalculateOffsetsParallel(ctx);
  xpu::barrier(ctx.pos());
  FindClustersParallel(ctx);
  xpu::barrier(ctx.pos());
  CalculateClustersParallel(ctx);
}

/**
  * sts::Hitfinder::calculateChannelOffsets
  * Calculates the Offsest of every channel. digis-Array is sorted in Channel and Time.
  * If a channelChange is detected it is stored in channelOffsetsPerModule.
  *
  * @param digis All Digis that are relevant for this Block
  * @param channelOffsets The Array where all offsets are written to
  * @param nDigis Amount of digis in digis-Array
  *
*/
XPU_D void sts::Hitfinder::CalculateChannelOffsets(FindClusters::context& ctx, CbmStsDigi* digis,
                                                   unsigned int* channelOffsets, unsigned int const nDigis) const
{
  channelOffsets[0] = 0;

  for (unsigned int pos = ctx.thread_idx_x(); pos < nDigis - 1; pos += (unsigned int) ctx.block_dim_x()) {
    auto const currChannel = digis[pos].GetChannel();
    auto const nextChannel = digis[pos + 1].GetChannel();
    if (currChannel != nextChannel) {
      for (int i = currChannel + 1; i <= nextChannel; i++) {
        channelOffsets[i] = pos + 1;
      }
    }
    XPU_ASSERT(digis[pos].GetChannel()
               <= digis[pos + 1].GetChannel());  //channel are supposed to be sorted increasingly
  }

  for (int c = digis[nDigis - 1].GetChannel() + 1; c < nChannels; c++) {
    channelOffsets[c] = nDigis;
  }
}

/**
  * sts::Hitfinder::calculateOffsetsParallel
  * This function calculates the channeloffsets in a certain module.
  * An Offset is the Startingpoint of a Channel in a sorted array of Digis.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void sts::Hitfinder::CalculateOffsetsParallel(FindClusters::context& ctx) const
{
  int const iModule = ctx.block_idx_x();
  CbmStsDigi* digis = &digisPerModule[digiOffsetPerModule[iModule]];
  auto const nDigis = GetNDigis(iModule);

  if (nDigis == 0) return;

  auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  CalculateChannelOffsets(ctx, digis, channelOffsets, nDigis);
}

/**
  * sts::Hitfinder::findClustersParallel
  * This function finds clusters form an sts-Digi Array inserted.
  * It runs the finding process highly parallel.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void sts::Hitfinder::FindClustersParallel(FindClusters::context& ctx) const
{
  const i32 iGThread    = ctx.block_dim_x() * ctx.block_idx_x() + ctx.thread_idx_x();
  const i32 nDigisTotal = digiOffsetPerModule[2 * nModules];
  if (iGThread >= nDigisTotal) { return; };

  i32 iModule = 0;
  while (size_t(iGThread) >= digiOffsetPerModule[iModule + 1]) {
    iModule++;
  }

  i32 iLocal = iGThread - digiOffsetPerModule[iModule];

  const CbmStsDigi myDigi = digisPerModule[iGThread];

  u32 nDigis = GetNDigis(iModule);
  if (nDigis == 0) return;

  xpu::view digis(&digisPerModule[digiOffsetPerModule[iModule]], nDigis);
  xpu::view digiConnector(&digiConnectorsPerModule[digiOffsetPerModule[iModule]], nDigis);
  xpu::view channelOffsets(&channelOffsetPerModule[iModule * nChannels], nChannels);

  u16 channel = myDigi.GetChannel();
  if (channel == nChannels - 1) return;

  //DeltaCalculation
  const f32 tResol              = GetTimeResolution(iModule, channel);
  const RecoParams::STS& params = ctx.cmem<Params>().sts;
  const f32 deltaT = (params.timeCutDigiAbs > 0.f ? params.timeCutDigiAbs : params.timeCutDigiSig * 1.4142f * tResol);

  const u32 nextChannelEnd = (channel + 2 < nChannels) ? channelOffsets[channel + 2] : nDigis;

  float firstPossibleTime = float(myDigi.GetTimeU32()) - deltaT;

  // Binary search for the first possible digi
  u32 start = channelOffsets[channel + 1];
  u32 end   = nextChannelEnd;
  while (start < end) {
    u32 mid = (start + end) / 2;
    if (float(digis[mid].GetTimeU32()) < firstPossibleTime) { start = mid + 1; }
    else {
      end = mid;
    }
  }

  for (auto nextIter = start; nextIter < nextChannelEnd; nextIter++) {

    const CbmStsDigi otherDigi = digis[nextIter];

    // This should never happen?
    if (myDigi.GetChannel() >= otherDigi.GetChannel()) continue;

    if (float(myDigi.GetTimeU32()) + deltaT < float(otherDigi.GetTimeU32())) { break; }
    if (float(myDigi.GetTimeU32()) - deltaT > float(otherDigi.GetTimeU32())) { continue; }

    digiConnector[iLocal].SetNext(nextIter);
    digiConnector[nextIter].SetHasPrevious(true);
    break;
  }
}

/**
  * sts::Hitfinder::calculateClustersParallel
  * This function calculates clusters form an sts-Digi Array inserted.
  * It runs the calculating process highly parallel.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void sts::Hitfinder::CalculateClustersParallel(FindClusters::context& ctx) const
{
  int const iModule = ctx.block_idx_x();
  CbmStsDigi* digis = &digisPerModule[digiOffsetPerModule[iModule]];
  ;
  auto const nDigis = GetNDigis(iModule);

  if (nDigis == 0) return;

  auto* digiConnector = &digiConnectorsPerModule[digiOffsetPerModule[iModule]];
  // auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  // calculateClustersChannelWise(digis, digiConnector, channelOffsets, iModule, threadId, nDigis);
  CalculateClustersDigiWise(ctx, digis, digiConnector, nDigis);
}

/**
  * sts::Hitfinder::calculateClustersDigiWise
  *
  * Each Thread one Channel
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  01234 56789
  *
  * Calculates the Clustercharges of all found ClusterConnections.
  * Each Thread takes on digi and looks for connections. When the thread is done, it takes the next digi.
  * If one Digi does not have a previous one it's a cluster start and may be the
  * start of either a single-element-cluster, double-element-cluster or
  * multi-element-cluster.
  *
  * @param digis All Digis that are relevant for this Block
  * @param digiConnector Array where the connection between 2 digis is sotred in.(next Digi; has Previous)
  * @param iModule The Module that the curren Block is working on
  * @param threadId Id of the thrad currently working
  *
**/
XPU_D void sts::Hitfinder::CalculateClustersDigiWise(FindClusters::context& ctx, CbmStsDigi* digis,
                                                     sts::DigiConnector* digiConnector, unsigned int const nDigis) const
{
  int const iModule = ctx.block_idx_x();
  for (unsigned int currIter = ctx.thread_idx_x(); currIter < nDigis; currIter += (unsigned int) ctx.block_dim_x()) {

    if (digiConnector[currIter].HasPrevious()) continue;

    if (!digiConnector[currIter].HasNext()) {
      //if Cluster has 1 element
      CreateClusterFromConnectors1(iModule, digis, currIter);
    }
    else if (!digiConnector[digiConnector[currIter].next()].HasNext()) {
      //if Cluster has 2 elements
      CreateClusterFromConnectors2(iModule, digis, digiConnector, currIter);
    }
    else {
      //if Cluster has N elements
      CreateClusterFromConnectorsN(iModule, digis, digiConnector, currIter);
    }
  }
}

XPU_D void sts::Hitfinder::CreateClusterFromConnectors1(int const iModule, CbmStsDigi* digis, int digiIndex) const
{
  const CbmStsDigi& digi = digis[digiIndex];

  uint32_t time = digi.GetTimeU32();

  const float timeError = asic.timeResolution;

  sts::Cluster cluster {
    .fCharge        = asic.AdcToCharge(digi.GetChargeU16()),
    .fSize          = 1,
    .fPosition      = float(digi.GetChannel()) + (IsBackside(iModule) ? nChannels : 0.f),
    .fPositionError = 1.f / xpu::sqrt(24.f),
    .fTime          = time,
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);

  AddCluster(iModule, time, cluster);
}

XPU_D void sts::Hitfinder::CreateClusterFromConnectors2(int const iModule, CbmStsDigi* digis,
                                                        sts::DigiConnector* digiConnector, int const digiIndex) const
{

  const CbmStsDigi& digi1 = digis[digiIndex];
  const CbmStsDigi& digi2 = digis[digiConnector[digiIndex].next()];

  float eNoiseSq = asic.noise * asic.noise;

  float chargePerAdc = asic.dynamicRange / float(asic.nAdc);
  float eDigitSq     = chargePerAdc * chargePerAdc / 12.f;

  float x1 = float(digi1.GetChannel());
  float q1 = asic.AdcToCharge(digi1.GetChargeU16());
  float q2 = asic.AdcToCharge(digi2.GetChargeU16());

  // Periodic position for clusters round the edge
  if (digi1.GetChannel() > digi2.GetChannel()) { x1 -= float(nChannels); }

  // Uncertainties of the charge measurements
  float width1 = LandauWidth(q1);
  float eq1sq  = width1 * width1 + eNoiseSq + eDigitSq;
  float width2 = LandauWidth(q2);
  float eq2sq  = width2 * width2 + eNoiseSq + eDigitSq;

  // Cluster time
  float time      = 0.5f * (digi1.GetTimeU32() + digi2.GetTimeU32());
  float timeError = asic.timeResolution * 0.70710678f;  // 1/sqrt(2)

  // Cluster position
  float x = x1 + 0.5f + (q2 - q1) / 3.f / xpu::max(q1, q2);

  // Correct negative position for clusters around the edge
  if (x < -0.5f) { x += float(nChannels); }

  // Uncertainty on cluster position. See software note.
  float ex0sq = 0.f;
  float ex1sq = 0.f;
  float ex2sq = 0.f;
  if (q1 < q2) {
    ex0sq = (q2 - q1) * (q2 - q1) / q2 / q2 / 72.f;
    ex1sq = eq1sq / q2 / q2 / 9.f;
    ex2sq = eq2sq * q1 * q1 / q2 / q2 / q2 / q2 / 9.f;
  }
  else {
    ex0sq = (q2 - q1) * (q2 - q1) / q1 / q1 / 72.f;
    ex1sq = eq1sq * q2 * q2 / q1 / q1 / q1 / q1 / 9.f;
    ex2sq = eq2sq / q1 / q1 / 9.f;
  }
  float xError = xpu::sqrt(ex0sq + ex1sq + ex2sq);

  // Cluster charge
  float charge = q1 + q2;

  if (IsBackside(iModule)) x += nChannels;

  sts::Cluster cls {
    .fCharge        = charge,
    .fSize          = 2,
    .fPosition      = x,
    .fPositionError = xError,
    .fTime          = uint32_t(time),
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);
  AddCluster(iModule, time, cls);
}

XPU_D void sts::Hitfinder::CreateClusterFromConnectorsN(int iModule, CbmStsDigi* digis,
                                                        sts::DigiConnector* digiConnector, int digiIndex) const
{
  ClusterCalculationProperties cProps;

  //This Lambda calculates all charges for a single digi inside of a cluster
  auto calculateClusterCharges = [this, &cProps, &digis, &digiConnector](int index) {
    CbmStsDigi digi    = digis[index];
    float eNoiseSq     = asic.noise * asic.noise;
    float chargePerAdc = asic.dynamicRange / float(asic.nAdc);
    float eDigitSq     = chargePerAdc * chargePerAdc / 12.f;
    cProps.tResolSum += asic.timeResolution;
    cProps.tSum += digi.GetTimeU32();
    float charge    = asic.AdcToCharge(digi.GetChargeU16());
    float lWidth    = LandauWidth(charge);
    float eChargeSq = lWidth * lWidth + eNoiseSq + eDigitSq;

    if (!digiConnector[index].HasPrevious()) {
      //begin of Cluster (most left Element)
      cProps.chanF = digi.GetChannel();
      cProps.qF    = charge;
      cProps.eqFsq = eChargeSq;
    }
    else if (!digiConnector[index].HasNext()) {
      //end of Cluster (most right Element)
      cProps.chanL = digi.GetChannel();
      cProps.qL    = charge;
      cProps.eqLsq = eChargeSq;
    }
    else {
      cProps.qM += charge;
      cProps.eqMsq += eChargeSq;
    }
    cProps.xSum += charge * float(digi.GetChannel());
  };

  calculateClusterCharges(digiIndex);

  // Calculate ClusterCharges
  while (digiConnector[digiIndex].HasNext()) {
    digiIndex = digiConnector[digiIndex].next();
    calculateClusterCharges(digiIndex);
  }

  // Periodic channel position for clusters round the edge
  if (cProps.chanF > cProps.chanL) cProps.chanF -= nChannels;

  // Cluster time and total charge
  float nDigis    = ChanDist(cProps.chanF, cProps.chanL) + 1;
  cProps.tSum     = cProps.tSum / nDigis;
  float timeError = (cProps.tResolSum / nDigis) / xpu::sqrt(nDigis);
  float qSum      = cProps.qF + cProps.qM + cProps.qL;

  // Average charge in middle strips
  cProps.qM /= (nDigis - 2.f);
  cProps.eqMsq /= (nDigis - 2.f);

  // Cluster position
  float x = 0.5f * (float(cProps.chanF + cProps.chanL) + (cProps.qL - cProps.qF) / cProps.qM);

  // Correct negative cluster position for clusters round the edge
  if (x < -0.5f) { x += float(nChannels); }

  // Cluster position error
  float exFsq = cProps.eqFsq / cProps.qM / cProps.qM / 4.f;  // error from first charge
  float exMsq = cProps.eqMsq * (cProps.qL - cProps.qF) * (cProps.qL - cProps.qF) / cProps.qM / cProps.qM / cProps.qM
                / cProps.qM / 4.f;
  float exLsq  = cProps.eqLsq / cProps.qM / cProps.qM / 4.f;
  float xError = xpu::sqrt(exFsq + exMsq + exLsq);

  // Correction for corrupt clusters
  if (x < cProps.chanF || x > cProps.chanL) { x = cProps.xSum / qSum; }

  XPU_ASSERT(x >= cProps.chanF && x <= cProps.chanL);
  XPU_ASSERT(nDigis > 2);

  if (IsBackside(iModule)) { x += nChannels; }

  sts::Cluster cls {
    .fCharge        = qSum,
    .fSize          = int(nDigis),
    .fPosition      = x,
    .fPositionError = xError,
    .fTime          = uint32_t(cProps.tSum),
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);
  AddCluster(iModule, cProps.tSum, cls);
}

XPU_D void sts::Hitfinder::SortClusters(SortClusters::context& ctx) const
{
  // By default sort all modules in parallel,
  // but in debug mode sort only one module at a time
  // to narrow down the error source
  int firstModule = ctx.block_idx_x();
  int lastModule  = ctx.block_idx_x();
  if (ctx.grid_dim_x() == 1) {
    firstModule = 0;
    lastModule  = 2 * nModules - 1;
  }

  for (int iModule = firstModule; iModule <= lastModule; iModule++) {
    size_t offset                = iModule * maxClustersPerModule;
    GpuClusterIdx* clusterIdx    = &clusterIdxPerModule[offset];
    GpuClusterIdx* clusterIdxTmp = &clusterIdxPerModuleTmp[offset];
    int nClusters                = nClustersPerModule[iModule];

    // if (nClusters == 0 && ctx.thread_idx_x() == 0) {
    //   printf("Module %d: No clusters found, exit early\n", iModule);
    // }
    if (nClusters == 0) return;

    // if (ctx.thread_idx_x() == 0) {
    //   printf("Module %d: Start sorting %d clusters\n", iModule, nClusters);
    // }

    SortClusters::sort_t clsSort(ctx.pos(), ctx.smem());
    clusterIdx = clsSort.sort(clusterIdx, nClusters, clusterIdxTmp, [](const GpuClusterIdx a) { return a.fTime; });

    // if (ctx.thread_idx_x() == 0) {
    //   printf("Module %d: Finished sorting %d clusters\n", iModule, nClusters);
    // }

    if (ctx.thread_idx_x() == 0) clusterIdxSortedPerModule[iModule] = clusterIdx;
  }
}

XPU_D void sts::Hitfinder::FindHits(FindHits::context& ctx) const
{

// On CPU process all modules in parallel (one thread per module).
// On GPU process all front clusters in parallel instead (one thread per cluster)
//   to fully utilize the GPU.
// Currently use option 2 for both as it is faster on CPU as well.
#if XPU_IS_CPU
  int iModule = ctx.block_idx_x();
#else
  int iModule = 0;
  int iThread = ctx.block_dim_x() * ctx.block_idx_x() + ctx.thread_idx_x();

  for (; iModule < nModules; iModule++) {
    if (iThread < nClustersPerModule[iModule]) break;
    iThread -= nClustersPerModule[iModule];
  }

  if (iModule >= nModules) return;
#endif

  int iModuleF               = iModule;
  int iModuleB               = iModuleF + nModules;
  size_t offsetF             = iModuleF * maxClustersPerModule;
  size_t offsetB             = iModuleB * maxClustersPerModule;
  GpuClusterIdx* clusterIdxF = clusterIdxSortedPerModule[iModuleF];
  GpuClusterIdx* clusterIdxB = clusterIdxSortedPerModule[iModuleB];
  sts::Cluster* clusterDataF = &clusterDataPerModule[offsetF];
  sts::Cluster* clusterDataB = &clusterDataPerModule[offsetB];
  int nClustersF             = nClustersPerModule[iModuleF];
  int nClustersB             = nClustersPerModule[iModuleB];

  if (nClustersF == 0 || nClustersB == 0) return;

  HitfinderCache pars;
  {
    SensorPar cfg = sensorPars[iModule];

    pars.dY         = cfg.dY;
    pars.pitch      = cfg.pitch;
    pars.stereoF    = cfg.stereoF;
    pars.stereoB    = cfg.stereoB;
    pars.lorentzF   = cfg.lorentzF;
    pars.lorentzB   = cfg.lorentzB;
    pars.tanStereoF = xpu::tan(pars.stereoF * xpu::deg_to_rad());
    pars.tanStereoB = xpu::tan(pars.stereoB * xpu::deg_to_rad());
    pars.errorFac   = 1.f / (pars.tanStereoB - pars.tanStereoF) / (pars.tanStereoB - pars.tanStereoF);
    pars.dX         = float(nChannels) * pars.pitch;
  }

  float maxTerrF = maxClusterTimeErrorByModuleSide[iModuleF];
  float maxTerrB = maxClusterTimeErrorByModuleSide[iModuleB];

  float maxSigmaBoth = 4.f * xpu::sqrt(maxTerrF * maxTerrF + maxTerrB * maxTerrB);

  int startB = 0;
#if XPU_IS_CPU
  for (int iClusterF = ctx.thread_idx_x(); iClusterF < nClustersF; iClusterF += ctx.block_dim_x()) {
#else
  int iClusterF = iThread;
  if (iClusterF < nClustersF) {

    float firstPossibleTime = float(clusterIdxF[iClusterF].fTime) - maxSigmaBoth;

    // Use binary search to find the first cluster on back side that can be matched
    // with the current cluster on front side
    int start = startB;
    int end   = nClustersB;
    while (end - start > 1) {
      int mid = (start + end) / 2;
      if (float(clusterIdxB[mid].fTime) < firstPossibleTime) { start = mid; }
      else {
        end = mid;
      }
    }

    startB = start;
#endif
    GpuClusterIdx clsIdxF = clusterIdxF[iClusterF];
    sts::Cluster clsDataF = clusterDataF[clsIdxF.fIdx];

    float maxSigma = 4.f * xpu::sqrt(clsDataF.fTimeError * clsDataF.fTimeError + maxTerrB * maxTerrB);

    for (int iClusterB = startB; iClusterB < nClustersB; iClusterB++) {
      GpuClusterIdx clsIdxB = clusterIdxB[iClusterB];
      sts::Cluster clsDataB = clusterDataB[clsIdxB.fIdx];

      float timeDiff = float(clsIdxF.fTime) - float(clsIdxB.fTime);

      if (timeDiff > 0 && timeDiff > maxSigmaBoth) {
        startB++;
        continue;
      }
      else if (timeDiff > 0 && timeDiff > maxSigma) {
        continue;
      }
      else if (timeDiff < 0 && xpu::abs(timeDiff) > maxSigma) {
        break;
      }

      float timeCut = -1.f;
      const RecoParams::STS& params = ctx.cmem<Params>().sts;
      if (params.timeCutClusterAbs > 0.f) timeCut = params.timeCutClusterAbs;
      else if (params.timeCutClusterSig > 0.f) {
        float eF = clsDataF.fTimeError;
        float eB = clsDataB.fTimeError;
        timeCut  = params.timeCutClusterSig * xpu::sqrt(eF * eF + eB * eB);
      }

      if (xpu::abs(float(clsIdxF.fTime) - float(clsIdxB.fTime)) > timeCut) continue;

      IntersectClusters(iModule, pars, clsIdxF.fTime, clsDataF, clsIdxB.fTime, clsDataB);
    }
  }
}

XPU_D void sts::Hitfinder::IntersectClusters(int iBlock, const HitfinderCache& pars, uint32_t timeF,
                                             const sts::Cluster& clsF, uint32_t timeB, const sts::Cluster& clsB) const
{
  // --- Calculate cluster centre position on readout edge

  float xF  = GetClusterPosition(pars, clsF.fPosition, true);
  float exF = clsF.fPositionError * pars.pitch;
  float du  = exF * xpu::cos(xpu::deg_to_rad() * pars.stereoF);
  float xB  = GetClusterPosition(pars, clsB.fPosition, false);
  float exB = clsB.fPositionError * pars.pitch;
  float dv  = exB * xpu::cos(xpu::deg_to_rad() * pars.stereoB);

  // // --- Should be inside active area
  if (xF < 0.f || xF > pars.dX || xB < 0.f || xB > pars.dX) { return; }

  // // --- Calculate number of line segments due to horizontal
  // // --- cross-connection. If x(y=0) does not fall on the bottom edge,
  // // --- the strip is connected to the one corresponding to the line
  // // --- with top edge coordinate xF' = xF +/- Dx. For odd combinations
  // // --- of stereo angle and sensor dimensions, this could even happen
  // // --- multiple times. For each of these lines, the intersection with
  // // --- the line on the other side is calculated. If inside the active area,
  // // --- a hit is created.
  int nF = (xF + pars.dY * pars.tanStereoF) / pars.dX;
  int nB = (xB + pars.dY * pars.tanStereoB) / pars.dX;

  // --- If n is positive, all lines from 0 to n must be considered,
  // --- if it is negative (phi negative), all lines from n to 0.
  int nF1 = xpu::min(0, nF);
  int nF2 = xpu::max(0, nF);
  int nB1 = xpu::min(0, nB);
  int nB2 = xpu::max(0, nB);

  // --- Double loop over possible lines
  float xC    = -1.f;
  float yC    = -1.f;
  float varX  = 0.f;
  float varY  = 0.f;
  float varXY = 0.f;
  for (int iF = nF1; iF <= nF2; iF++) {
    float xFi = xF - float(iF) * pars.dX;
    for (int iB = nB1; iB <= nB2; iB++) {
      float xBi = xB - float(iB) * pars.dX;

      // --- Intersect the two lines
      bool found = Intersect(pars, xFi, exF, xBi, exB, xC, yC, varX, varY, varXY);
      if (not found) continue;

      // --- Transform into sensor system with origin at sensor centre

      xC -= 0.5f * pars.dX;
      yC -= 0.5f * pars.dY;

      CreateHit(iBlock, xC, yC, varX, varY, varXY, timeF, clsF, timeB, clsB, du, dv);
    }
  }
}

XPU_D float sts::Hitfinder::GetClusterPosition(const HitfinderCache& pars, float centre, bool isFront) const
{
  // Take integer channel

  int iChannel = int(centre);
  float xDif   = centre - float(iChannel);

  // Calculate corresponding strip on sensor

  int iStrip = iChannel - (isFront ? 0 : nChannels);

  // Re-add difference to integer channel. Convert channel to
  // coordinate
  float xCluster = (float(iStrip) + xDif + 0.5f) * pars.pitch;

  // // Correct for Lorentz-Shift
  // // Simplification: The correction uses only the y component of the
  // // magnetic field. The shift is calculated using the mid-plane of the
  // // sensor, which is not correct for tracks not traversing the entire
  // // sensor thickness (i.e., are created or stopped somewhere in the sensor).
  // // However, this is the best one can do in reconstruction.
  xCluster -= (isFront ? pars.lorentzF : pars.lorentzB);

  return xCluster;
}

XPU_D bool sts::Hitfinder::Intersect(const HitfinderCache& pars, float xF, float exF, float xB, float exB, float& x,
                                     float& y, float& varX, float& varY, float& varXY) const
{

  // In the coordinate system with origin at the bottom left corner,
  // a line along the strips with coordinate x0 at the top edge is
  // given by the function y(x) = Dy - ( x - x0 ) / tan(phi), if
  // the stereo angle phi does not vanish. Two lines yF(x), yB(x) with top
  // edge coordinates xF, xB intersect at
  // x = ( tan(phiB)*xF - tan(phiF)*xB ) / (tan(phiB) - tan(phiF)
  // y = Dy + ( xB - xF ) / ( tan(phiB) - tan(phiF) )
  // For the case that one of the stereo angles vanish (vertical strips),
  // the calculation of the intersection is straightforward.

  // --- First check whether stereo angles are different. Else there is
  // --- no intersection.
  // TODO: if this is true, no hits can be found? So just do this once at the beginning?
  if (xpu::abs(pars.stereoF - pars.stereoB) < 0.5f) return false;

  // --- Now treat vertical front strips
  if (xpu::abs(pars.stereoF) < 0.001f) {
    x     = xF;
    y     = pars.dY - (xF - xB) / pars.tanStereoB;
    varX  = exF * exF;
    varY  = (exF * exF + exB * exB) / pars.tanStereoB / pars.tanStereoB;
    varXY = -1.f * exF * exF / pars.tanStereoB;
    return IsInside(pars, x - pars.dX / 2.f, y - pars.dY / 2.f);
  }

  // --- Maybe the back side has vertical strips?
  if (xpu::abs(pars.stereoB) < 0.001f) {
    x     = xB;
    y     = pars.dY - (xB - xF) / pars.tanStereoF;
    varX  = exB * exB;
    varY  = (exF * exF + exB * exB) / pars.tanStereoF / pars.tanStereoF;
    varXY = -1.f * exB * exB / pars.tanStereoF;
    return IsInside(pars, x - pars.dX / 2.f, y - pars.dY / 2.f);
  }

  // --- OK, both sides have stereo angle

  x = (pars.tanStereoB * xF - pars.tanStereoF * xB) / (pars.tanStereoB - pars.tanStereoF);
  y = pars.dY + (xB - xF) / (pars.tanStereoB - pars.tanStereoF);
  varX =
    pars.errorFac * (exF * exF * pars.tanStereoB * pars.tanStereoB + exB * exB * pars.tanStereoF * pars.tanStereoF);
  varY  = pars.errorFac * (exF * exF + exB * exB);
  varXY = -1.f * pars.errorFac * (exF * exF * pars.tanStereoB + exB * exB * pars.tanStereoF);

  return IsInside(pars, x - pars.dX / 2.f, y - pars.dY / 2.f);
}

XPU_D bool sts::Hitfinder::IsInside(const HitfinderCache& pars, float x, float y) const
{
  // clang-format off
  return x >= -pars.dX / 2.f
      && x <= pars.dX / 2.f
      && y >= -pars.dY / 2.f
      && y <= pars.dY / 2.f;
  // clang-format on
}

XPU_D void sts::Hitfinder::CreateHit(int iModule, float xLocal, float yLocal, float varX, float varY, float varXY,
                                     uint32_t timeF, const sts::Cluster& clsF, uint32_t timeB, const sts::Cluster& clsB,
                                     float du, float dv) const
{
  // --- Transform into global coordinate system
  float globalX, globalY, globalZ;
  ToGlobal(iModule, xLocal, yLocal, 0.f, globalX, globalY, globalZ);

  // We assume here that the local-to-global transformations is only translation
  // plus maybe rotation upside down or front-side back. In that case, the
  // global covariance matrix is the same as the local one.
  float errX = xpu::sqrt(varX);
  float errY = xpu::sqrt(varY);
  float errZ = 0.f;

  // --- Calculate hit time (average of cluster times)
  float hitTime      = 0.5f * (float(timeF) + float(timeB));
  float etF          = clsF.fTimeError;
  float etB          = clsB.fTimeError;
  float hitTimeError = 0.5f * xpu::sqrt(etF * etF + etB * etB);

  sts::Hit hit {
    .fX         = globalX,
    .fY         = globalY,
    .fZ         = globalZ,
    .fTime      = static_cast<uint32_t>(hitTime),
    .fDx        = errX,
    .fDy        = errY,
    .fDz        = errZ,
    .fDxy       = varXY,
    .fTimeError = hitTimeError,
    .fDu        = du,
    .fDv        = dv,
  };

  int idx = xpu::atomic_add(&nHitsPerModule[iModule], 1);

  assert(size_t(idx) < maxHitsPerModule);
  if (size_t(idx) < maxHitsPerModule) { hitsPerModule[iModule * maxHitsPerModule + idx] = hit; }
}

XPU_D float sts::Hitfinder::LandauWidth(float charge) const
{
  if (charge <= landauStepSize) return landauTable[0];
  if (charge > landauStepSize * (landauTableSize - 1)) return landauTable[landauTableSize - 1];

  int tableIdx = xpu::ceil(charge / landauStepSize);
  float e2     = tableIdx * landauStepSize;
  float v2     = landauTable[tableIdx];
  tableIdx--;
  float e1 = tableIdx * landauStepSize;
  float v1 = landauTable[tableIdx];
  return v1 + (charge - e1) * (v2 - v1) / (e2 - e1);
}

XPU_D void sts::Hitfinder::ToGlobal(int iModule, float lx, float ly, float lz, float& gx, float& gy, float& gz) const
{
  const float* tr  = &localToGlobalTranslationByModule[iModule * 3];
  const float* rot = &localToGlobalRotationByModule[iModule * 9];

  gx = tr[0] + lx * rot[0] + ly * rot[1] + lz * rot[2];
  gy = tr[1] + lx * rot[3] + ly * rot[4] + lz * rot[5];
  gz = tr[2] + lx * rot[6] + ly * rot[7] + lz * rot[8];
}
