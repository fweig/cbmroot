/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak */

#ifndef _CbmL1Hit_h_
#define _CbmL1Hit_h_

#include "L1Vector.h"

///
/// Identificator for cbm hits with their detector and index in cbm arrays
///
class CbmL1HitId {
public:
  CbmL1HitId() = default;
  CbmL1HitId(int det, int index) : detId(det), hitId(index) {};

  int detId {0};  ///< detector ID (mvd/sts/etc)
  int hitId {0};  ///< index of hit in the TClonesArray array
};


///
/// a helper class for performance evaluation that contains useful info about cbm hits with hit-mcpoint match information
///
class CbmL1HitDebugInfo {  // TODO: SZh 21.09.2022: Replace instances of this class with L1Hit
public:
  int ExtIndex;  ///< index of hit in the external branch
  int iStation;  ///< index of station in active stations array
  double x;      ///< x coordinate of position [cm]
  double y;      ///< y coordinate of position [cm]
  double time;   ///< hit time [ns]
  double dx;     ///< x coordinate error [cm]
  double dy;     ///< y coordinate error [cm]
  double dt;     ///< time error [ns]
  double dxy;    ///< covariance between x and y [cm2]
  int Det;       ///< detector subsystem ID
  L1Vector<int> mcPointIds {"CbmL1HitDebugInfo::mcPointIds"};  ///< indices of CbmL1MCPoint in L1->fvMCPoints array
};

#endif
