/* Copyright (C) 2012-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Valentina Akishina */

#include "L1HitsSortHelper.h"

#include <algorithm>
#include <vector>

#include "L1Grid.h"
#include "L1Hit.h"
#include "L1HitPoint.h"

using std::vector;

L1HitsSortHelper::L1HitsSortHelper(vector<L1Hit>& hits, vector<L1HitPoint>& points, vector<L1HitIndex_t>& indices,
                                   const L1Grid* grid, L1HitIndex_t* iStart, L1HitIndex_t* iStop, int nStations,
                                   int nDontUsedHits)
  : fD()
  , fHits(hits)
  , fPoints(points)
  , fIndices(indices)
  , fGrid(grid)
  , fHitsUnusedStartIndex(iStart)
  , fHitsUnusedStopIndex(iStop)
  , fNStations(nStations)
  , fnDontUsedHits(nDontUsedHits)
{
  L1_ASSERT(hits.size() == points.size(), hits.size() << " " << points.size());
  const int NHits = fnDontUsedHits;
  fD.resize(NHits);
  //float x,y=0;
  for (int iS = 0; iS < fNStations; ++iS)
    for (L1HitIndex_t i = fHitsUnusedStartIndex[iS]; i < fHitsUnusedStopIndex[iS]; i++) {
      fD[i].h = &(hits[i]);
      fD[i].p = &(points[i]);
      fD[i].i = indices[i];
      //      StripsToCoor(fD[i].p->U(),fD[i].p->V(), x, y, vStations[iS]);
      //      NOTE: Use vStations[iS].ConvUVtoXY() istead
      //      fD[i].bin = fGrid[iS].GetBinBounded(x[0], y[0]);
    }
}

void L1HitsSortHelper::Sort()
{
  for (int iS = 0; iS < fNStations; ++iS) {
    std::sort(fD.begin() + fHitsUnusedStartIndex[iS], fD.begin() + fHitsUnusedStopIndex[iS],
              L1HitsSortHelperData::compare);
  }

  vector<L1Hit> hits        = fHits;
  vector<L1HitPoint> points = fPoints;
  const int NHits           = fD.size();
  for (int i = 0; i < NHits; i++) {
    hits[i]     = *fD[i].h;
    points[i]   = *fD[i].p;
    fIndices[i] = fD[i].i;
  }
  fHits   = hits;
  fPoints = points;
}
