/* Copyright (C) 2012-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef _L1HitsL1HitsSortHelper_h_
#define _L1HitsL1HitsSortHelper_h_

#include <algorithm>
#include <vector>

#include "L1Grid.h"
#include "L1Hit.h"
#include "L1HitPoint.h"

using std::vector;

struct L1HitsSortHelperData {
  L1Hit* h;
  L1HitPoint* p;
  unsigned int bin;
  L1HitIndex_t i;

  static bool compare(const L1HitsSortHelperData& a, const L1HitsSortHelperData& b)
  {
    return a.bin < b.bin || (a.bin == b.bin && a.p->V() < b.p->V());
  }
};

class L1HitsSortHelper {
public:
  L1HitsSortHelper(vector<L1Hit>& hits, vector<L1HitPoint>& points, vector<L1HitIndex_t>& indices, const L1Grid* grid,
                   L1HitIndex_t* iStart, L1HitIndex_t* iStop, int nStations, int nDontUsedHits);

  void Sort();

private:
  vector<L1HitsSortHelperData> fD;
  vector<L1Hit>& fHits;
  vector<L1HitPoint>& fPoints;
  vector<L1HitIndex_t>& fIndices;
  const L1Grid* fGrid;
  L1HitIndex_t *fStsHitsUnusedStartIndex, *fStsHitsUnusedStopIndex;
  int fNStations;
  int fnDontUsedHits;

  L1HitsSortHelper(const L1HitsSortHelper&);
  L1HitsSortHelper& operator=(const L1HitsSortHelper&);
};

#endif
