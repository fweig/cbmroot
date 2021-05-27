/** @file CbmL1StsHit.h
  * @copyright Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Igor Kulakov [orginator], Valentina Akishina, Maxim Zyzak **/

#ifndef _CbmL1StsHit_h_
#define _CbmL1StsHit_h_

#include <vector>
using std::vector;

// hits with hit-mcpoint match information
class CbmL1StsHit {
public:
  CbmL1StsHit()
    : hitId(0)
    , extIndex(0)
    , Det(0)
    , mcPointIds()
    , x(0.)
    , y(0.)
    , t(0.)
    , f(0.)
    , b(0.)
    , ID(0.)
    , file(0)
    , event(0) {};
  CbmL1StsHit(int hitId_, int extIndex_, int Det_)
    : hitId(hitId_)
    , extIndex(extIndex_)
    , Det(Det_)
    , mcPointIds()
    , x(0.)
    , y(0.)
    , t(0.)
    , f(0.)
    , b(0.)
    , ID(0.)
    , file(0)
    , event(0) {};

  int hitId;     // index of L1StsHit in algo->vStsHits array. Should be equal to index of this in L1->vStsHits
  int extIndex;  // index of hit in the TClonesArray array
  int Det;
  vector<int> mcPointIds;  // indices of CbmL1MCPoint in L1->vMCPoints array
  float x, y, t;
  int f, b;
  int ID;
  int file;
  int event;
};

#endif
