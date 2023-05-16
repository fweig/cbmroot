/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#ifndef CBM_ALGO_DATA_STS_CLUSTER_H
#define CBM_ALGO_DATA_STS_CLUSTER_H

#include "Prelude.h"

namespace cbm::algo::sts
{

  struct Cluster {
    real fCharge;         ///< Total charge
    i32 fSize;            ///< Difference between first and last channel
    real fPosition;       ///< Cluster centre in channel number units
    real fPositionError;  ///< Cluster centre error (r.m.s.) in channel number units
    u32 fTime;            ///< cluster time [ns]
    real fTimeError;      ///< Error of cluster time [ns]
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_DATA_STS_CLUSTER_H
