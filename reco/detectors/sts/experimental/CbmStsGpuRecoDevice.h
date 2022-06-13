/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer], Kilian Hunold */
#ifndef CBMSTSGPURECODEVICE_H
#define CBMSTSGPURECODEVICE_H 1

#include <xpu/device.h>

// TODO: Tune these values for various gpus
#if XPU_IS_CUDA
#define CBM_STS_SORT_DIGIS_BLOCK_SIZE 32
#define CBM_STS_SORT_DIGIS_ITEMS_PER_THREAD 32
#define CBM_STS_SORT_CLUSTERS_BLOCK_SIZE 32
#define CBM_STS_SORT_CLUSTERS_ITEMS_PER_THREAD 32
#define CBM_STS_FIND_CLUSTERS_BLOCK_SIZE 32
#define CBM_STS_FIND_HITS_BLOCK_SIZE 32
#else  // HIP, values ignored on CPU
#define CBM_STS_SORT_DIGIS_BLOCK_SIZE 256
#define CBM_STS_SORT_DIGIS_ITEMS_PER_THREAD 6
#define CBM_STS_SORT_CLUSTERS_BLOCK_SIZE 256
#define CBM_STS_SORT_CLUSTERS_ITEMS_PER_THREAD 6
#define CBM_STS_FIND_CLUSTERS_BLOCK_SIZE 256
#define CBM_STS_FIND_HITS_BLOCK_SIZE 256
#endif

struct CbmStsGpuRecoDevice {
};

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, SortDigis);
XPU_BLOCK_SIZE(SortDigis, CBM_STS_SORT_DIGIS_BLOCK_SIZE);

// Combine substeps for finding clusters into a single kernel
XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, FindClustersSingleStep);
XPU_BLOCK_SIZE(FindClustersSingleStep, CBM_STS_FIND_CLUSTERS_BLOCK_SIZE);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, CalculateOffsets);
XPU_BLOCK_SIZE(CalculateOffsets, CBM_STS_FIND_CLUSTERS_BLOCK_SIZE);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, FindClusters);
XPU_BLOCK_SIZE(FindClusters, CBM_STS_FIND_CLUSTERS_BLOCK_SIZE);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, FindClustersBasic);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, CalculateClusters);
XPU_BLOCK_SIZE(CalculateClusters, CBM_STS_FIND_CLUSTERS_BLOCK_SIZE);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, CalculateClustersBasic);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, SortClusters);
XPU_BLOCK_SIZE(SortClusters, CBM_STS_SORT_CLUSTERS_BLOCK_SIZE);

XPU_EXPORT_KERNEL(CbmStsGpuRecoDevice, FindHits);
XPU_BLOCK_SIZE(FindHits, CBM_STS_FIND_HITS_BLOCK_SIZE);

#endif
