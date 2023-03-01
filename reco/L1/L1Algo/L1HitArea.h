/* Copyright (C) 2012-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Igor Kulakov [committer] */

#ifndef L1HitArea_H
#define L1HitArea_H

#include "L1Def.h"
#include "L1Grid.h"

class L1Row;
class L1SliceData;

class L1HitArea {
public:
  L1HitArea(const L1Grid& grid, float x, float y, float dx, float dy);

  /**
     * look up the next hit in the requested area.
     * Sets h to the coordinates and returns the index for the hit data
     */
  bool GetNext(L1HitIndex_t& i);

protected:
  const L1Grid& fGrid;

  unsigned short fBYmax;  // maximal Y bin index
  unsigned short fBDX;    // X distance of bin indexes
  unsigned int fIndXmin;  // minimum index for
  unsigned short fIy;     // current Y bin index (incremented while iterating)
  L1HitIndex_t fHitXlst;  // last possible hit index in current y-line
  L1HitIndex_t fIh;       // hit index iterating inside the bins
  int fNx;                // Number of bins in X direction
};

inline L1HitArea::L1HitArea(const L1Grid& grid, float x, float y, float dx, float dy)
  : fGrid(grid)
  , fBYmax(0)
  , fBDX(0)
  , fIndXmin(0)
  , fIy(0)
  , fHitXlst(0)
  , fIh(0)
  , fNx(fGrid.Nx())
{
  const float minX = x - dx;
  const float maxX = x + dx;
  const float minY = y - dy;
  const float maxY = y + dy;
  unsigned short bXmin, bYmin, bXmax;  // boundary bin indexes
  fGrid.GetBinBounded(minX, minY, bXmin, bYmin);
  fGrid.GetBinBounded(maxX, maxY, bXmax, fBYmax);

  fBDX = (bXmax - bXmin + 1);  // bin index span in x direction

  fIndXmin = (bYmin * fNx + bXmin);  // same as grid.GetBin(fminX, fMinY), i.e. the smallest bin index of interest
  // fIndXmin + fBDX - 1 then is the largest bin index of interest with the same Y

  fGrid.GetBinBounds(fIndXmin, x, dx, y, dy);

  fIy = bYmin;

  fIh      = fGrid.FirstHitInBin(fIndXmin);
  fHitXlst = fGrid.FirstHitInBin(fIndXmin + fBDX);
}

inline bool L1HitArea::GetNext(L1HitIndex_t& i)
{
  bool xIndexOutOfRange     = fIh >= fHitXlst;  // current x is not in the area
  bool nextYIndexOutOfRange = (fIy >= fBYmax);  // there isn't any new y-line

  if (xIndexOutOfRange && nextYIndexOutOfRange) {  // all iterators are over the end
    return false;
  }

  // at least one entry in the vector has (fIh >= fHitXlst && fIy < fBYmax)
  bool needNextY = xIndexOutOfRange && !nextYIndexOutOfRange;

  // skip as long as fIh is outside of the interesting bin x-index
  while (ISLIKELY(needNextY)) {  //ISLIKELY to speed the programm and optimise the use of registers
    fIy++;                       // get new y-line
      // get next hit
    fIndXmin += fNx;
    fIh      = fGrid.FirstHitInBin(fIndXmin);  // get first hit in cell, if y-line is new
    fHitXlst = fGrid.FirstHitInBin(fIndXmin + fBDX);

    xIndexOutOfRange     = fIh >= fHitXlst;
    nextYIndexOutOfRange = (fIy >= fBYmax);
    needNextY            = xIndexOutOfRange && !nextYIndexOutOfRange;
  }

  L1_ASSERT(fIh < fGrid.FirstHitInBin(fGrid.N()) || xIndexOutOfRange,
            fIh << " < " << fGrid.FirstHitInBin(fGrid.N()) << " || " << xIndexOutOfRange);
  i = fIh;  // return

  fIh++;  // go to next
  return !xIndexOutOfRange;
}

class L1HitAreaTime {
public:
  L1HitAreaTime(const L1Grid& grid, float x, float y, float dx, float dy, float t, float dt);

  /**
     * look up the next hit in the requested area.
     * Sets h to the coordinates and returns the index for the hit data
     */
  bool GetNext(L1HitIndex_t& i);

protected:
  const L1Grid& fGrid;

  unsigned short fBYmax;  // maximal Y bin index
  unsigned short fBDX;    // X distance of bin indexes
  unsigned int fIndXmin;  // minimum index for
  unsigned short fIy;     // current Y bin index (incremented while iterating)
  L1HitIndex_t fHitXlst;  // last possible hit index in current y-line
  L1HitIndex_t fIh;       // hit index iterating inside the bins
  int fNx;                // Number of bins in X direction
  int fNy;

  unsigned short fBTmax;  // maximal Y bin index
  unsigned short fIt;     // current Y bin index (incremented while iterating)

  unsigned short fBYmin;
  unsigned short fBXmin;


  unsigned int fIndYmin;  // minimum index for
};

inline L1HitAreaTime::L1HitAreaTime(const L1Grid& grid, float x, float y, float dx, float dy, float t, float dt)
  : fGrid(grid)
  , fBYmax(0)
  , fBDX(0)
  , fIndXmin(0)
  , fIy(0)
  , fHitXlst(0)
  , fIh(0)
  , fNx(fGrid.Nx())
  , fNy(fGrid.Ny())
  , fBTmax(0)
  , fIt(0)
  , fBYmin(0)
  , fBXmin(0)
  , fIndYmin(0)
{
  const float minX = x - dx;
  const float maxX = x + dx;
  const float minY = y - dy;
  const float maxY = y + dy;
  const float minT = t - dt;


  const float maxT = t + dt;

  unsigned short bXmin, bYmin, bXmax, bTmin;  // boundary bin indexes
  fGrid.GetBinBounded(minX, minY, minT, bXmin, bYmin, bTmin);
  fGrid.GetBinBounded(maxX, maxY, maxT, bXmax, fBYmax, fBTmax);
  fBYmin = bYmin;
  fBXmin = bXmin;


  fBDX = (bXmax - bXmin + 1);  // bin index span in x direction

  fIndXmin = (bTmin * fNx * fNy + bYmin * fNx + bXmin);

  //fGrid.GetBinBounds(fIndXmin, x, dx, y, dy, t, dt);

  fIy = bYmin;

  fIt = bTmin;

  fIh = fGrid.FirstHitInBin(fIndXmin);

  fHitXlst = fGrid.FirstHitInBin(fIndXmin + fBDX);
}

inline bool L1HitAreaTime::GetNext(L1HitIndex_t& i)
{
  bool xIndexOutOfRange     = fIh >= fHitXlst;  // current x is not in the area
  bool nextYIndexOutOfRange = (fIy >= fBYmax);  // there isn't any new y-line
  bool nextTIndexOutOfRange = (fIt >= fBTmax);  // there isn't any new y-line


  if (xIndexOutOfRange && nextYIndexOutOfRange && nextTIndexOutOfRange) {  // all iterators are over the end
    return false;
  }

  // at least one entry in the vector has (fIh >= fHitXlst && fIy < fBYmax)
  bool needNextY = xIndexOutOfRange && !nextYIndexOutOfRange;
  bool needNextT = xIndexOutOfRange && nextYIndexOutOfRange && !nextTIndexOutOfRange;


  while (ISLIKELY((needNextY) || (needNextT))) {  //ISLIKELY to speed the programm and optimise the use of registers

    if (needNextT) {
      fIt++;
      fIy = fBYmin;

      fIndXmin = (fIt * fNx * fNy + fBYmin * fNx + fBXmin);
      fIh      = fGrid.FirstHitInBin(fIndXmin);  // get first hit in cell, if y-line is new
      fHitXlst = fGrid.FirstHitInBin(fIndXmin + fBDX);
    }
    else {

      fIy++;  // get new y-line
      // get next hit
      fIndXmin += fNx;
      fIh = fGrid.FirstHitInBin(fIndXmin);  // get first hit in cell, if y-line is new

      fHitXlst = fGrid.FirstHitInBin(fIndXmin + fBDX);
    }

    xIndexOutOfRange     = fIh >= fHitXlst;
    nextYIndexOutOfRange = (fIy >= fBYmax);
    needNextY            = xIndexOutOfRange && !nextYIndexOutOfRange;

    nextTIndexOutOfRange = (fIt >= fBTmax);
    needNextT            = xIndexOutOfRange && nextYIndexOutOfRange && !nextTIndexOutOfRange;
  }

  L1_ASSERT(fIh < fGrid.FirstHitInBin(fGrid.N() + 1) || xIndexOutOfRange,
            fIh << " < " << fGrid.FirstHitInBin(fGrid.N() + 1) << " || " << xIndexOutOfRange);
  i = fIh;  // return

  fIh++;  // go to next
  return !xIndexOutOfRange;
}

#endif
