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
  int ExtIndex;
  int iStation;
  double x;
  double y;
  double time;
  double dx;
  double dy;
  double dt;
  double dxy;
  int Det;
  L1Vector<int> mcPointIds {"CbmL1HitDebugInfo::mcPointIds"};  ///< indices of CbmL1MCPoint in L1->fvMCPoints array
};

#endif
