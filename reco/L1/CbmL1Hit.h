/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak */

#ifndef _CbmL1Hit_h_
#define _CbmL1Hit_h_

#include <vector>
using std::vector;

///
/// hits with hit-mcpoint match information
///
class CbmL1Hit {
public:
  CbmL1Hit() {};

  CbmL1Hit(int hitId_, int extIndex_, int Det_) : hitId(hitId_), extIndex(extIndex_), Det(Det_) {};

  int hitId    = 0;  // index of L1Hit in algo->vStsHits array. Should be equal to index of this in L1->vStsHits
  int extIndex = 0;  // index of hit in the TClonesArray array
  int Det      = 0;  // station index
  vector<int> mcPointIds {};  // indices of CbmL1MCPoint in L1->vMCPoints array
  float x   = 0.f;            // measured X coordinate
  float y   = 0.f;            // measured Y coordinate
  float t   = 0.f;            // measured time
  int f     = 0;              // front strip index
  int b     = 0;              // back strip index
  int ID    = 0;              // TODO: check if this ID is redundant
  int file  = 0;              // TODO: ??
  int event = 0;              // TODO: ??
};

#endif
