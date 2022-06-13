/* Copyright (C) 2021-2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#include "CbmStsGpuHitFinder.h"

using namespace experimental;

/**
  * CbmStsGpuHitFinder::sortDigisKhun
  * Sorts digis channelwise. Inside a channel all digis are sorted in time.
  *
  * @param smem is the shared memory it is worked in
  * @param iBlock is the Block/Module that is currenty running
  *
*/
XPU_D void CbmStsGpuHitFinder::SortDigisInSpaceAndTime(CbmStsSortDigiSmem& smem, int iBlock) const
{
  int iModule          = iBlock;
  CbmStsDigi* digis    = &digisPerModule[digiOffsetPerModule[iModule]];
  CbmStsDigi* digisTmp = &digisPerModuleTmp[digiOffsetPerModule[iModule]];
  int nDigis           = GetNDigis(iModule);

  SortDigisT digiSort(smem.sortBuf);

  digis = digiSort.sort(digis, nDigis, digisTmp, [](const CbmStsDigi a) {
    return ((unsigned long int) a.GetChannel()) << 32 | (unsigned long int) (a.GetTimeU32());
  });

  if (xpu::thread_idx::x() == 0) { digisSortedPerModule[iModule] = digis; }
}

XPU_D void CbmStsGpuHitFinder::FindClustersSingleStep(int iBlock) const
{
  CalculateOffsetsParallel(iBlock);
  FindClustersParallel(iBlock);
  CalculateClustersParallel(iBlock);
}

/**
  * CbmStsGpuHitFinder::calculateChannelOffsets
  * Calculates the Offsest of every channel. digis-Array is sorted in Channel and Time.
  * If a channelChange is detected it is stored in channelOffsetsPerModule.
  *
  * @param digis All Digis that are relevant for this Block
  * @param channelOffsets The Array where all offsets are written to
  * @param nDigis Amount of digis in digis-Array
  *
*/
XPU_D void CbmStsGpuHitFinder::CalculateChannelOffsets(CbmStsDigi* digis, unsigned int* channelOffsets,
                                                       unsigned int const nDigis) const
{
  channelOffsets[0] = 0;

  //Parallel
  for (unsigned int pos = xpu::thread_idx::x(); pos < nDigis - 1; pos += (unsigned int) xpu::block_dim::x()) {
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
  * CbmStsGpuHitFinder::calculateOffsetsParallel
  * This function calculates the channeloffsets in a certain module.
  * An Offset is the Startingpoint of a Channel in a sorted array of Digis.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void CbmStsGpuHitFinder::CalculateOffsetsParallel(int iBlock) const
{
  int const iModule = iBlock;
  CbmStsDigi* digis = digisSortedPerModule[iModule];
  auto const nDigis = GetNDigis(iModule);

  if (nDigis == 0) return;

  auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  CalculateChannelOffsets(digis, channelOffsets, nDigis);
}

/**
  * CbmStsGpuHitFinder::findClustersParallel
  * This function finds clusters form an sts-Digi Array inserted.
  * It runs the finding process highly parallel.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void CbmStsGpuHitFinder::FindClustersParallel(int iBlock) const
{
  int const iModule           = iBlock;
  CbmStsDigi* digis           = digisSortedPerModule[iModule];
  auto const nDigis           = GetNDigis(iModule);
  unsigned int const threadId = xpu::thread_idx::x();

  if (nDigis == 0) return;

  auto* digiConnector  = &digiConnectorPerModule[digiOffsetPerModule[iModule]];
  auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  // findClusterConnectionsChannelWise(digis, digiConnector, channelOffsets, iModule, threadId);
  FindClusterConnectionsDigiWise(digis, digiConnector, channelOffsets, iModule, threadId, nDigis);
}


/**
  * CbmStsGpuHitFinder::calculateClustersParallel
  * This function calculates clusters form an sts-Digi Array inserted.
  * It runs the calculating process highly parallel.
  *
  * @param iBlock is the Block/Module that is currently worked on
  *
*/
XPU_D void CbmStsGpuHitFinder::CalculateClustersParallel(int iBlock) const
{
  int const iModule  = iBlock;
  CbmStsDigi* digis  = digisSortedPerModule[iModule];
  auto const nDigis  = GetNDigis(iModule);
  int const threadId = xpu::thread_idx::x();

  if (nDigis == 0) { return; }

  auto* digiConnector = &digiConnectorPerModule[digiOffsetPerModule[iModule]];
  // auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  // calculateClustersChannelWise(digis, digiConnector, channelOffsets, iModule, threadId, nDigis);
  CalculateClustersDigiWise(digis, digiConnector, iModule, threadId, nDigis);
}


/**
  * CbmStsGpuHitFinder::findClusterConnectionsBasic
  *
  * Each Thread one Channel
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  00000 00000
  *
  * Finds Clusters in a basic way. One thread takes care of the whole calculation.
  *
  * @param digis All Digis that are relevant for this Block
  * @param digiConnector Array where the connection between 2 digis is sotred in.(next Digi; has Previous)
  * @param channelOffsets The Array where all offsets are written to
  * @param iModule The Module that the curren Block is working on
  * @param threadId Id of the thrad currently working
  *
**/
XPU_D void CbmStsGpuHitFinder::FindClusterConnectionsBasic(int iBlock) const
{
  int iModule          = iBlock;
  int threadId         = xpu::thread_idx::x();
  auto nDigis          = GetNDigis(iModule);
  CbmStsDigi* digis    = digisSortedPerModule[iModule];
  auto* digiConnector  = &digiConnectorPerModule[digiOffsetPerModule[iModule]];
  auto* channelOffsets = &channelOffsetPerModule[iModule * nChannels];

  if (threadId != 0) return;

  for (unsigned int currIter = 0; currIter < nDigis; currIter++) {
    const CbmStsDigi& digi = digis[currIter];
    uint16_t channel       = digi.GetChannel();

    if (channel == nChannels - 1) break;

    //DeltaCalculation
    float const tResol = GetTimeResolution(iModule, channel);
    float const deltaT = (timeCutDigiAbs > 0.f ? timeCutDigiAbs : timeCutDigiSig * 1.4142f * tResol);

    unsigned int nextChannelStart = channelOffsets[channel + 1];
    unsigned int nextChannelEnd   = (channel + 2 < nChannels) ? channelOffsets[channel + 2] : nDigis;

    // printf("Comparing digis (deltaT=%f, nextChannelStart=%u, nextChannelEnd=%u, nDigis=%u):\n", deltaT,
    //        nextChannelStart, nextChannelEnd, nDigis);
    // printf(" channel=%d, time=%d, charge=%d\n", digi.GetChannel(), digi.GetTimeU32(), digi.GetChargeU16());

    //Calculate DigiConnections
    for (unsigned int nextIter = nextChannelStart; nextIter < nextChannelEnd; nextIter++) {

      const CbmStsDigi& nextDigi = digis[nextIter];

      // printf("> channel=%d, time=%d, charge=%d\n", nextDigi.GetChannel(), nextDigi.GetTimeU32(),
      //        nextDigi.GetChargeU16());

      if (digi.GetChannel() >= nextDigi.GetChannel()) continue;

      if (float(digis[currIter].GetTimeU32() + deltaT) <= float(digis[nextIter].GetTimeU32())) break;
      if (float(digis[currIter].GetTimeU32() - deltaT) >= float(digis[nextIter].GetTimeU32())) continue;

      digiConnector[currIter].SetNext(nextIter);
      digiConnector[nextIter].SetHasPrevious(true);
      break;
    }
  }
}


/**
  * CbmStsGpuHitFinder::findClusterConnectionsChannelWise
  *
  * Each Thread one Channel
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  00000 11111
  *
  * Finds Clusters highly parallel in a basic way. Each thread gets the same anmount of Channels to work with.
  * Every Thread checks all digis in its channel, if there is a pendant to the current one in the channel to its right.
  * If there is another digi, the digi will be connected inside the digiConnector-Array.
  *
  * @param digis All Digis that are relevant for this Block
  * @param digiConnector Array where the connection between 2 digis is sotred in.(next Digi; has Previous)
  * @param channelOffsets The Array where all offsets are written to
  * @param iModule The Module that the curren Block is working on
  * @param threadId Id of the thrad currently working
  *
**/
XPU_D void CbmStsGpuHitFinder::FindClusterConnectionsChannelWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                                                 unsigned int* channelOffsets, int const iModule,
                                                                 unsigned int const threadId) const
{
  // Expl: here we need to be smaller than nChannels because the last channel is the most Right.
  for (unsigned int channel = threadId; channel < (unsigned int) (nChannels - 1); channel += xpu::block_dim::x()) {
    unsigned int const currChannelBegin = channelOffsets[channel];      //Begin of current Channel
    unsigned int const nextChannelBegin = channelOffsets[channel + 1];  //Begin of next (right) channel
    auto const nextChannelEnd =
      (channel + 2 < (unsigned int) nChannels) ? channelOffsets[channel + 2] : GetNDigis(iModule);
    unsigned int nextIter = nextChannelBegin;

    //Check if first Digi of Channel belongs to Channel
    if (channel != digis[currChannelBegin].GetChannel()) continue;

    //DeltaCalculation
    float const tResol = GetTimeResolution(iModule, channel);
    float const deltaT = (timeCutDigiAbs > 0.f ? timeCutDigiAbs : timeCutDigiSig * 1.4142f * tResol);

    //Calculate DigiConnections
    for (auto currIter = currChannelBegin; currIter < nextChannelBegin; currIter++) {
      while (nextIter < nextChannelEnd
             && ((digis[currIter].GetTimeU32() + deltaT) >= float(digis[nextIter].GetTimeU32()))) {
        if (digis[currIter].GetChannel() >= digis[nextIter].GetChannel()) { continue; }
        if (digis[currIter].GetTimeU32() + deltaT < float(digis[nextIter].GetTimeU32())) {
          nextIter++;
          break;
        }
        if (digis[currIter].GetTimeU32() - deltaT > float(digis[nextIter].GetTimeU32())) {
          nextIter++;
          continue;
        }

        digiConnector[currIter].SetNext(nextIter);
        digiConnector[nextIter].SetHasPrevious(true);
        nextIter++;
        break;
      }
    }
  }
}

/**
  * CbmStsGpuHitFinder::findClusterConnectionsDigiWise
  *
  * Each thread one Digi
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  01234 01234
  *
  * Finds Clusters highly parallel in a basic way.
  * The Threads work on Data thats stored nearby, so it's been taken advantage of the data locality.
  * Each thread takes one digi and calculates if there is a neighbour.
  * This is repeated until all digis have been processed
  *
  * @param digis All Digis that are relevant for this Block
  * @param digiConnector Array where the connection between 2 digis is sotred in.(next Digi; has Previous)
  * @param channelOffsets The Array where all offsets are written to
  * @param iModule The Module that the curren Block is working on
  * @param threadId Id of the thrad currently working
  * @param nDigis Amount of digis in digis-Array
  *
**/
XPU_D void CbmStsGpuHitFinder::FindClusterConnectionsDigiWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                                              unsigned int* channelOffsets, int const iModule,
                                                              unsigned int const threadId,
                                                              unsigned int const nDigis) const
{

  for (unsigned int currIter = threadId; currIter < nDigis; currIter += xpu::block_dim::x()) {
    auto const digi    = digis[currIter];
    auto const channel = digi.GetChannel();

    if (channel == nChannels - 1) break;

    //DeltaCalculation
    float const tResol = GetTimeResolution(iModule, channel);
    float const deltaT = (timeCutDigiAbs > 0.f ? timeCutDigiAbs : timeCutDigiSig * 1.4142f * tResol);

    auto const nextChannelEnd = (channel + 2 < nChannels) ? channelOffsets[channel + 2] : nDigis;
    //Calculate DigiConnections
    for (auto nextIter = channelOffsets[channel + 1]; nextIter < nextChannelEnd; nextIter++) {
      if (digis[currIter].GetChannel() >= digis[nextIter].GetChannel()) continue;
      if (float(digis[currIter].GetTimeU32()) + deltaT < float(digis[nextIter].GetTimeU32())) break;
      if (float(digis[currIter].GetTimeU32()) - deltaT > float(digis[nextIter].GetTimeU32())) continue;

      digiConnector[currIter].SetNext(nextIter);
      digiConnector[nextIter].SetHasPrevious(true);
      break;
    }
  }
}


/**
  * CbmStsGpuHitFinder::calculateClustersBasic
  *
  * One Thread all Digis
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  00000 00000
  *
  * Calculates the Clustercharges of all found ClusterConnections.
  * One thread walks through all Digis and looks for connections. If one Digi does not have a previous one
  * it's a cluster start and may be the start of either a single-element-cluster, double-element-cluster
  * or multi-element-cluster.
  * All Other Threads are canceled
  *
  * @param digis All Digis that are relevant for this Block
  * @param digiConnector Array where the connection between 2 digis is sotred in.(next Digi; has Previous)
  * @param iModule The Module that the curren Block is working on
  * @param threadId Id of the thrad currently working
  *
**/
XPU_D void CbmStsGpuHitFinder::CalculateClustersBasic(int iBlock) const
{
  int iModule         = iBlock;
  int threadId        = xpu::thread_idx::x();
  CbmStsDigi* digis   = digisSortedPerModule[iModule];
  auto* digiConnector = &digiConnectorPerModule[digiOffsetPerModule[iModule]];

  if (threadId != 0) return;

  for (unsigned int i = 0; i < GetNDigis(iModule); i++) {
    if (digiConnector[i].HasPrevious()) { continue; }
    if (!digiConnector[i].HasNext()) {
      //if Cluster has 1 element
      CreateClusterFromConnectors1(iModule, digis, i);
    }
    else if (!digiConnector[digiConnector[i].next()].HasNext()) {
      //if Cluster has 2 elements
      CreateClusterFromConnectors2(iModule, digis, digiConnector, i);
    }
    else {
      //if Cluster has N elements
      CreateClusterFromConnectorsN(iModule, digis, digiConnector, i);
    }
  }
}


/**
  * CbmStsGpuHitFinder::calculateClustersChannelWise
  *
  * Each Thread one Channel
  *
  * ChannelId: 00000 11111
  * DigiIndex: 01234 56789
  * ThreadId:  00000 11111
  *
  * Calculates the Clustercharges of all found ClusterConnections.
  * Each Thread walks through all digis of one channel and looks for connections.
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
XPU_D void CbmStsGpuHitFinder::CalculateClustersChannelWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                                            unsigned int* channelOffsets, int const iModule,
                                                            unsigned int const threadId,
                                                            unsigned int const nDigis) const
{

  for (unsigned int channel = threadId; channel < (unsigned int) nChannels;
       channel += (unsigned int) xpu::block_dim::x()) {
    unsigned int const currChannelBegin = channelOffsets[channel];
    unsigned int const nextChannelBegin =
      (channel + 1 < (unsigned int) nChannels) ? channelOffsets[channel + 1] : nDigis;

    //Check if first Digi of Channel belongs to Channel
    if (channel != digis[currChannelBegin].GetChannel()) { continue; }

    //Calculate DigiConnections
    for (auto currIter = currChannelBegin; currIter < nextChannelBegin; currIter++) {
      if (digiConnector[currIter].HasPrevious()) { continue; }
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
}


/**
  * CbmStsGpuHitFinder::calculateClustersDigiWise
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
XPU_D void CbmStsGpuHitFinder::CalculateClustersDigiWise(CbmStsDigi* digis, CbmStsDigiConnector* digiConnector,
                                                         int const iModule, unsigned int const threadId,
                                                         unsigned int const nDigis) const
{

  for (unsigned int currIter = threadId; currIter < nDigis; currIter += (unsigned int) xpu::block_dim::x()) {

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


XPU_D void CbmStsGpuHitFinder::CreateClusterFromConnectors1(int const iModule, CbmStsDigi* digis, int digiIndex) const
{
  const CbmStsDigi& digi = digis[digiIndex];

  CbmStsTimeNs time = digi.GetTimeU32();

  const float timeError = asic.timeResolution;

  CbmStsClusterData cluster {
    .fCharge        = asic.AdcToCharge(digi.GetChargeU16()),
    .fSize          = 1,
    .fPosition      = float(digi.GetChannel()) + (IsBackside(iModule) ? nChannels : 0.f),
    .fPositionError = 1.f / xpu::sqrt(24.f),
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);

  AddCluster(iModule, time, cluster);
}

XPU_D void CbmStsGpuHitFinder::CreateClusterFromConnectors2(int const iModule, CbmStsDigi* digis,
                                                            CbmStsDigiConnector* digiConnector,
                                                            int const digiIndex) const
{

  const CbmStsDigi& digi1 = digis[digiIndex];
  const CbmStsDigi& digi2 = digis[digiConnector[digiIndex].next()];

  float eNoiseSq = asic.noise * asic.noise;

  float chargePerAdc = asic.dynRange / float(asic.nAdc);
  float eDigitSq     = chargePerAdc * chargePerAdc / 12.f;

  float x1 = float(digi1.GetChannel());
  float q1 = asic.AdcToCharge(digi1.GetChargeU16());
  float q2 = asic.AdcToCharge(digi2.GetChargeU16());

  // Periodic position for clusters round the edge
  if (digi1.GetChannel() > digi2.GetChannel()) { x1 -= float(nChannels); }

  // Uncertainties of the charge measurements
  XErrorT width1 = LandauWidth(q1);
  XErrorT eq1sq  = width1 * width1 + eNoiseSq + eDigitSq;
  XErrorT width2 = LandauWidth(q2);
  XErrorT eq2sq  = width2 * width2 + eNoiseSq + eDigitSq;

  // Cluster time
  float time      = 0.5f * (digi1.GetTimeU32() + digi2.GetTimeU32());
  float timeError = asic.timeResolution * 0.70710678f;  // 1/sqrt(2)

  // Cluster position
  float x = x1 + 0.5f + (q2 - q1) / 3.f / xpu::max(q1, q2);

  // Correct negative position for clusters around the edge
  if (x < -0.5f) { x += float(nChannels); }

  // Uncertainty on cluster position. See software note.
  XErrorT ex0sq = 0.f;
  XErrorT ex1sq = 0.f;
  XErrorT ex2sq = 0.f;
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
  XErrorT xError = xpu::sqrt(ex0sq + ex1sq + ex2sq);

  // Cluster charge
  float charge = q1 + q2;

  if (IsBackside(iModule)) x += nChannels;

  CbmStsClusterData cls {
    .fCharge        = charge,
    .fSize          = 2,
    .fPosition      = x,
    .fPositionError = xError,
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);
  AddCluster(iModule, time, cls);
}


XPU_D void CbmStsGpuHitFinder::CreateClusterFromConnectorsN(int iModule, CbmStsDigi* digis,
                                                            CbmStsDigiConnector* digiConnector, int digiIndex) const
{
  CbmStsClusterCalculationProperties cProps;

  //This Lambda calculates all charges for a single digi inside of a cluster
  auto calculateClusterCharges = [this, &cProps, &digis, &digiConnector](int index) {
    CbmStsDigi digi    = digis[index];
    float eNoiseSq     = asic.noise * asic.noise;
    float chargePerAdc = asic.dynRange / float(asic.nAdc);
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

  CbmStsClusterData cls {
    .fCharge        = qSum,
    .fSize          = int(nDigis),
    .fPosition      = x,
    .fPositionError = xError,
    .fTimeError     = timeError,
  };

  SaveMaxError(timeError, iModule);
  AddCluster(iModule, cProps.tSum, cls);
}

XPU_D void CbmStsGpuHitFinder::SortClusters(CbmStsSortClusterSmem& smem, int iBlock) const
{
  int iModule                     = iBlock;
  size_t offset                   = iModule * maxClustersPerModule;
  CbmStsClusterIdx* clusterIdx    = &clusterIdxPerModule[offset];
  CbmStsClusterIdx* clusterIdxTmp = &clusterIdxPerModuleTmp[offset];
  int nClusters                   = nClustersPerModule[iModule];

  SortClustersT clsSort(smem.sortBuf);
  clusterIdx = clsSort.sort(clusterIdx, nClusters, clusterIdxTmp, [](const CbmStsClusterIdx a) { return a.fTime; });

  if (xpu::thread_idx::x() == 0) clusterIdxSortedPerModule[iModule] = clusterIdx;
}

XPU_D void CbmStsGpuHitFinder::FindHits(int iBlock) const
{
  int iModuleF                    = iBlock;
  int iModuleB                    = iBlock + nModules;
  size_t offsetF                  = iModuleF * maxClustersPerModule;
  size_t offsetB                  = iModuleB * maxClustersPerModule;
  CbmStsClusterIdx* clusterIdxF   = clusterIdxSortedPerModule[iModuleF];
  CbmStsClusterIdx* clusterIdxB   = clusterIdxSortedPerModule[iModuleB];
  CbmStsClusterData* clusterDataF = &clusterDataPerModule[offsetF];
  CbmStsClusterData* clusterDataB = &clusterDataPerModule[offsetB];
  int nClustersF                  = nClustersPerModule[iModuleF];
  int nClustersB                  = nClustersPerModule[iModuleB];


  CbmStsHitFinderParams pars;
  {
    CbmStsHitFinderConfig cfg = hitfinderConfig[iBlock];

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

  float maxTerrF = *maxErrorFront;
  float maxTerrB = *maxErrorBack;

  float maxSigmaBoth = 4.f * xpu::sqrt(maxTerrF * maxTerrF + maxTerrB * maxTerrB);

  int startB = 0;
  for (int iClusterF = xpu::thread_idx::x(); iClusterF < nClustersF; iClusterF += xpu::block_dim::x()) {
    CbmStsClusterIdx clsIdxF   = clusterIdxF[iClusterF];
    CbmStsClusterData clsDataF = clusterDataF[clsIdxF.fIdx];

    float maxSigma = 4.f * xpu::sqrt(clsDataF.fTimeError * clsDataF.fTimeError + maxTerrF * maxTerrF);

    for (int iClusterB = startB; iClusterB < nClustersB; iClusterB++) {
      CbmStsClusterIdx clsIdxB   = clusterIdxB[iClusterB];
      CbmStsClusterData clsDataB = clusterDataB[clsIdxB.fIdx];

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
      if (timeCutClusterAbs > 0.f) timeCut = timeCutClusterAbs;
      else if (timeCutClusterSig > 0.f) {
        float eF = clsDataF.fTimeError;
        float eB = clsDataB.fTimeError;
        timeCut  = timeCutClusterSig * xpu::sqrt(eF * eF + eB * eB);
      }

      if (xpu::abs(float(clsIdxF.fTime) - float(clsIdxB.fTime)) > timeCut) continue;

      IntersectClusters(iBlock, pars, clsIdxF.fTime, clsDataF, clsIdxB.fTime, clsDataB);
    }
  }
}

XPU_D void CbmStsGpuHitFinder::IntersectClusters(int iBlock, const CbmStsHitFinderParams& pars, CbmStsTimeNs timeF,
                                                 const CbmStsClusterData& clsF, CbmStsTimeNs timeB,
                                                 const CbmStsClusterData& clsB) const
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

XPU_D float CbmStsGpuHitFinder::GetClusterPosition(const CbmStsHitFinderParams& pars, float centre, bool isFront) const
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

XPU_D bool CbmStsGpuHitFinder::Intersect(const CbmStsHitFinderParams& pars, float xF, float exF, float xB, float exB,
                                         float& x, float& y, float& varX, float& varY, float& varXY) const
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

XPU_D bool CbmStsGpuHitFinder::IsInside(const CbmStsHitFinderParams& pars, float x, float y) const
{
  // clang-format off
  return x >= -pars.dX / 2.f
      && x <= pars.dX / 2.f
      && y >= -pars.dY / 2.f
      && y <= pars.dY / 2.f;
  // clang-format on
}

XPU_D void CbmStsGpuHitFinder::CreateHit(int iModule, float xLocal, float yLocal, float varX, float varY, float varXY,
                                         CbmStsTimeNs timeF, const CbmStsClusterData& clsF, CbmStsTimeNs timeB,
                                         const CbmStsClusterData& clsB, float du, float dv) const
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

  CbmStsHit hit {
    .fX         = globalX,
    .fY         = globalY,
    .fZ         = globalZ,
    .fTime      = static_cast<CbmStsTimeNs>(hitTime),
    .fDx        = errX,
    .fDy        = errY,
    .fDz        = errZ,
    .fDxy       = varXY,
    .fTimeError = hitTimeError,
    .fDu        = du,
    .fDv        = dv,
  };

  int idx = xpu::atomic_add_block(&nHitsPerModule[iModule], 1);

  assert(size_t(idx) < maxHitsPerModule);

  hitsPerModule[iModule * maxHitsPerModule + idx] = hit;
}


XPU_D XErrorT CbmStsGpuHitFinder::LandauWidth(float charge) const
{
  if (charge <= landauStepSize) return landauTable[0];
  if (charge > landauStepSize * (landauTableSize - 1)) return landauTable[landauTableSize - 1];

  int tableIdx = xpu::ceil(charge / landauStepSize);
  XErrorT e2   = tableIdx * landauStepSize;
  XErrorT v2   = landauTable[tableIdx];
  tableIdx--;
  XErrorT e1 = tableIdx * landauStepSize;
  XErrorT v1 = landauTable[tableIdx];
  return v1 + (charge - e1) * (v2 - v1) / (e2 - e1);
}

XPU_D void CbmStsGpuHitFinder::ToGlobal(int iModule, float lx, float ly, float lz, float& gx, float& gy,
                                        float& gz) const
{
  const float* tr  = &localToGlobalTranslationByModule[iModule * 3];
  const float* rot = &localToGlobalRotationByModule[iModule * 9];

  gx = tr[0] + lx * rot[0] + ly * rot[1] + lz * rot[2];
  gy = tr[1] + lx * rot[3] + ly * rot[4] + lz * rot[5];
  gz = tr[2] + lx * rot[6] + ly * rot[7] + lz * rot[8];
}
