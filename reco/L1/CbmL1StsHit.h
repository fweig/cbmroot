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
    , file(0)
    , event(0) {};

  int
    hitId;  // index of L1StsHit in algo->vStsHits array. Should be equal to index of this in L1->vStsHits
  int extIndex;  // index of hit in the TClonesArray array
  int Det;
  vector<int> mcPointIds;  // indices of CbmL1MCPoint in L1->vMCPoints array
  float x, y, t;

  int file;
  int event;
};

#endif
