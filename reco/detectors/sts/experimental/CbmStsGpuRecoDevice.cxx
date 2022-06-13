/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#include "CbmStsGpuRecoDevice.h"

#include "CbmStsGpuHitFinder.h"

using namespace experimental;

XPU_IMAGE(CbmStsGpuRecoDevice);

XPU_CONSTANT(HitFinder);

XPU_KERNEL(SortDigis, CbmStsSortDigiSmem) { xpu::cmem<HitFinder>().SortDigisInSpaceAndTime(smem, xpu::block_idx::x()); }

XPU_KERNEL(FindClustersSingleStep, xpu::no_smem) { xpu::cmem<HitFinder>().FindClustersSingleStep(xpu::block_idx::x()); }

XPU_KERNEL(CalculateOffsets, xpu::no_smem) { xpu::cmem<HitFinder>().CalculateOffsetsParallel(xpu::block_idx::x()); }

XPU_KERNEL(FindClusters, xpu::no_smem) { xpu::cmem<HitFinder>().FindClustersParallel(xpu::block_idx::x()); }

XPU_KERNEL(FindClustersBasic, xpu::no_smem) { xpu::cmem<HitFinder>().FindClusterConnectionsBasic(xpu::block_idx::x()); }

XPU_KERNEL(CalculateClusters, xpu::no_smem) { xpu::cmem<HitFinder>().CalculateClustersParallel(xpu::block_idx::x()); }

XPU_KERNEL(CalculateClustersBasic, xpu::no_smem) { xpu::cmem<HitFinder>().CalculateClustersBasic(xpu::block_idx::x()); }

XPU_KERNEL(SortClusters, CbmStsSortClusterSmem) { xpu::cmem<HitFinder>().SortClusters(smem, xpu::block_idx::x()); }

XPU_KERNEL(FindHits, xpu::no_smem) { xpu::cmem<HitFinder>().FindHits(xpu::block_idx::x()); }
