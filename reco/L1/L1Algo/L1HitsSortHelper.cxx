
#include "L1HitsSortHelper.h"

#include <algorithm>
#include <vector>

#include "L1Grid.h"
#include "L1HitPoint.h"
#include "L1StsHit.h"

using std::vector;

L1HitsSortHelper::L1HitsSortHelper(vector<L1StsHit>& hits, vector<L1HitPoint>& points, vector<THitI>& indices,
                                   const L1Grid* grid, THitI* iStart, THitI* iStop, int nStations, int nDontUsedHits)
  : fD()
  , fHits(hits)
  , fPoints(points)
  , fIndices(indices)
  , fGrid(grid)
  , fStsHitsUnusedStartIndex(iStart)
  , fStsHitsUnusedStopIndex(iStop)
  , fNStations(nStations)
  , fnDontUsedHits(nDontUsedHits)
{
  L1_ASSERT(hits.size() == points.size(), hits.size() << " " << points.size());
  const int NHits = fnDontUsedHits;
  fD.resize(NHits);
  //float x,y=0;
  for (int iS = 0; iS < fNStations; ++iS)
    for (THitI i = fStsHitsUnusedStartIndex[iS]; i < fStsHitsUnusedStopIndex[iS]; i++) {
      fD[i].h = &(hits[i]);
      fD[i].p = &(points[i]);
      fD[i].i = indices[i];
      //      StripsToCoor(fD[i].p->U(),fD[i].p->V(), x, y, vStations[iS]);
      //      fD[i].bin = fGrid[iS].GetBinBounded(x[0], y[0]);
    }
}

void L1HitsSortHelper::Sort()
{
  for (int iS = 0; iS < fNStations; ++iS) {
    std::sort(fD.begin() + fStsHitsUnusedStartIndex[iS], fD.begin() + fStsHitsUnusedStopIndex[iS],
              L1HitsSortHelperData::compare);
  }

  vector<L1StsHit> hits     = fHits;
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
