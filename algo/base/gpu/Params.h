/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_GPU_CONFIG_H
#define CBM_ALGO_GPU_CONFIG_H

#include <xpu/device.h>

#include "DeviceImage.h"
#include "Prelude.h"
#include "RecoParams.h"

namespace cbm::algo
{
  struct Params : xpu::constant<GPUReco, RecoParams> {
  };
}  // namespace cbm::algo

#endif
