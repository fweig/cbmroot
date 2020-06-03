#include "LxEff.h"
#include "Lx.h"
#include <cmath>
#include <set>

//#define LXDRAW

#ifdef LXDRAW
#include "LxDraw.h"
//#define LXDRAWSAVE
//#define LXDRAWWAIT
#endif//LXDRAW

using namespace std;

LxEff::LxEff(LxFinderTriplet& owner) : finder(owner)
{
}

#ifdef CLUSTER_MODE
void LxEff::AnalyzeNotMatched(ofstream& out, list<LxPoint*> hits[LXSTATIONS][LXLAYERS])
{
  out << "AnalyzeNotMatched:" << endl;

  for (Int_t stNum = LXFIRSTSTATION; stNum < LXSTATIONS - 1; ++stNum)
  {
    out << "Segments #" << stNum << endl;
    list<LxPoint*> lHits[LXLAYERS] = hits[stNum];
    list<LxPoint*> rHits[LXLAYERS] = hits[stNum + 1];

    Double_t maxDx = 0;
    Double_t maxDy = 0;
    Double_t maxDtx = 0;
    Double_t maxDty = 0;
    Double_t minDx = 1000;
    Double_t minDy = 1000;
    Double_t minDtx = 1000;
    Double_t minDty = 1000;

    for (list<LxPoint*>::iterator l0 = lHits[0].begin(); l0 != lHits[0].end(); ++l0)
    {
      for (list<LxPoint*>::iterator l1 = lHits[1].begin(); l1 != lHits[1].end(); ++l1)
      {
        for (list<LxPoint*>::iterator l2 = lHits[2].begin(); l2 != lHits[2].end(); ++l2)
        {
          for (list<LxPoint*>::iterator r0 = rHits[0].begin(); r0 != rHits[0].end(); ++r0)
          {
            for (list<LxPoint*>::iterator r1 = rHits[1].begin(); r1 != rHits[1].end(); ++r1)
            {
              for (list<LxPoint*>::iterator r2 = rHits[2].begin(); r2 != rHits[2].end(); ++r2)
              {
                LxPoint* lPoints[LXLAYERS] = { *l0, *l1, *l2 };
                LxPoint* rPoints[LXLAYERS] = { *r0, *r1, *r2 };
                LxRay* rays[LXLAYERS * LXLAYERS] = {};

                for (Int_t i = 0; i < LXLAYERS; ++i)
                {
                  for (Int_t j = 0; j < LXLAYERS; ++j)
                    rays[j * LXLAYERS + i] = new LxRay(rPoints[j], lPoints[i], j * LXLAYERS + i);
                }

                for (Int_t i = 0; i < LXLAYERS * LXLAYERS; ++i)
                {
                  LxRay* ray1 = rays[i];
                  Double_t tx1 = ray1->tx;
                  Double_t ty1 = ray1->ty;
                  Double_t z1 = ray1->source->z;
                  Double_t x1 = ray1->source->x;
                  Double_t y1 = ray1->source->y;

                  if (minDx > ray1->source->dx)
                    minDx = ray1->source->dx;

                  if (minDy > ray1->source->dy)
                    minDy = ray1->source->dy;

                  if (minDtx > ray1->dtx)
                    minDtx = ray1->dtx;

                  if (minDty > ray1->dty)
                    minDty = ray1->dty;

                  for (Int_t j = i; j < LXLAYERS * LXLAYERS; ++j)
                  {
                    LxRay* ray2 = rays[j];
                    Double_t tx2 = ray2->tx;
                    Double_t ty2 = ray2->ty;
                    Double_t z2 = ray2->source->z;
                    Double_t diffZ = z1 - z2;
                    Double_t x2 = ray2->source->x + tx2 * diffZ;
                    Double_t y2 = ray2->source->y + ty2 * diffZ;

                    Double_t dtx = abs(tx2 - tx1);
                    Double_t dty = abs(ty2 - ty1);
                    Double_t dx = abs(x2 - x1);
                    Double_t dy = abs(y2 - y1);

                    if (maxDx < dx)
                      maxDx = dx;

                    if (maxDy < dy)
                      maxDy = dy;

                    if (maxDtx < dtx)
                      maxDtx = dtx;

                    if (maxDty < dty)
                      maxDty = dty;
                  }
                }

                for (Int_t i = 0; i < LXLAYERS * LXLAYERS; ++i)
                  delete rays[i];
              }
            }
          }
        }
      }
    }

    out << 4.0 * sqrt(minDx * minDx + finder.caSpace.stations[stNum + 1]->clusterXLimit2) - maxDx << " " <<
        4.0 * sqrt(minDy * minDy + finder.caSpace.stations[stNum + 1]->clusterYLimit2) - maxDy << " " <<
        4.0 * sqrt(minDtx * minDtx + finder.caSpace.stations[stNum + 1]->clusterTxLimit2) - maxDtx << " " <<
        4.0 * sqrt(minDty * minDty + finder.caSpace.stations[stNum + 1]->clusterTyLimit2) - maxDty << endl << endl;
  }
}

void LxEff::AnalyzeNotMatched2(ofstream& out, list<LxPoint*> hits[LXSTATIONS][LXLAYERS])
{
  out << "AnalyzeNotMatched2:" << endl;

  for (Int_t stNum = LXFIRSTSTATION; stNum < LXSTATIONS - 1; ++stNum)
  {
    out << "Segments #" << stNum << " has ";
    list<LxPoint*> lHits[LXLAYERS] = hits[stNum];
    list<LxPoint*> rHits[LXLAYERS] = hits[stNum + 1];

    Int_t maxOccupiedLevels = 0;
    Int_t maxNumPointsWithRays = 0;

    for (list<LxPoint*>::iterator l0 = lHits[0].begin(); l0 != lHits[0].end(); ++l0)
    {
      for (list<LxPoint*>::iterator l1 = lHits[1].begin(); l1 != lHits[1].end(); ++l1)
      {
        for (list<LxPoint*>::iterator l2 = lHits[2].begin(); l2 != lHits[2].end(); ++l2)
        {
          for (list<LxPoint*>::iterator r0 = rHits[0].begin(); r0 != rHits[0].end(); ++r0)
          {
            for (list<LxPoint*>::iterator r1 = rHits[1].begin(); r1 != rHits[1].end(); ++r1)
            {
              for (list<LxPoint*>::iterator r2 = rHits[2].begin(); r2 != rHits[2].end(); ++r2)
              {
                LxPoint* lPoints[LXLAYERS] = { *l0, *l1, *l2 };
                LxPoint* rPoints[LXLAYERS] = { *r0, *r1, *r2 };
                Int_t occupiedLevels = 0;
                Int_t numPointsWithRays = 0;

                for (Int_t i = 0; i < LXLAYERS; ++i)
                {
                  LxPoint* rPoint = rPoints[i];

                  if (!rPoint->rays.empty())
                    ++numPointsWithRays;

                  for (Int_t j = 0; j < LXLAYERS; ++j)
                  {
                    LxPoint* lPoint = lPoints[j];

                    for (list<LxRay*>::iterator k = rPoint->rays.begin(); k != rPoint->rays.end(); ++k)
                    {
                      LxRay* ray = *k;

                      if (ray->end == lPoint)
                      {
                        ++occupiedLevels;
                        break;
                      }
                    }
                  }
                }

                if (occupiedLevels > maxOccupiedLevels)
                  maxOccupiedLevels = occupiedLevels;

                if (numPointsWithRays > maxNumPointsWithRays)
                  maxNumPointsWithRays = numPointsWithRays;
              }
            }
          }
        }
      }
    }// for (list<LxPoint*>::iterator l0 = lHits[0].begin(); l0 != lHits[0].end(); ++l0)

    out << maxOccupiedLevels << " of occupied levels and " << maxNumPointsWithRays << " points with rays" << endl << endl;
  }
}

void LxEff::DumpNotMatchedMC(list<LxPoint*> hits[LXSTATIONS][LXLAYERS])
{
  AnalyzeNotMatched(finder.incomplete_events, hits);
  AnalyzeNotMatched2(finder.incomplete_events, hits);
  finder.incomplete_events << "Dumping not matched MC track" << endl;

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    finder.incomplete_events << "S" << i << " ";

    for (Int_t j = 0; j < LXLAYERS; ++j)
    {
      finder.incomplete_events << "L" << j << " ";

      for (list<LxPoint*>::iterator k = hits[i][j].begin(); k != hits[i][j].end(); ++k)
      {
        LxPoint* point = *k;
        finder.incomplete_events << "[";

        if (point->track)
          finder.incomplete_events << "t";
        else
        {
          if (point->leftClusteredRay)
            finder.incomplete_events << "r";

          finder.incomplete_events << "/";

          if (point->rightClusteredRay)
            finder.incomplete_events << "r";

        }

        finder.incomplete_events << "] ";
      }

      finder.incomplete_events << " ";
    }

    finder.incomplete_events << " ";
  }

  finder.incomplete_events << endl << endl;
}
#endif//CLUSTER_MODE

void LxEff::AnalyzeNotMatchedMC(ofstream& out, list<LxPoint*> hits[LXSTATIONS][LXLAYERS])
{
  out << "Not matched MC track:" << endl;
  out << "All triplets:   ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    Int_t tripletsCount = 0;

    for (list<LxPoint*>::iterator j = hits[i][1].begin(); j != hits[i][1].end(); ++j)
    {
      LxPoint* point = *j;
      tripletsCount += point->triplets.size();
    }

    out << "[" << tripletsCount << "]";
  }

  out << endl << "True triplets:  ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    Int_t trueTripletsCount = 0;

    for (list<LxPoint*>::iterator j = hits[i][1].begin(); j != hits[i][1].end(); ++j)
    {
      LxPoint* point = *j;

      for (list<LxTriplet*>::iterator k = point->triplets.begin(); k != point->triplets.end(); ++k)
      {
        LxTriplet* triplet = *k;
        LxPoint* lPoint = triplet->left;
        LxPoint* rPoint = triplet->right;
        bool trueLPoint = false;
        bool trueRPoint = false;

        for (list<LxPoint*>::iterator l = hits[i][0].begin(); l != hits[i][0].end(); ++l)
        {
          if (*l == lPoint)
            trueLPoint = true;
        }

        for (list<LxPoint*>::iterator l = hits[i][2].begin(); l != hits[i][2].end(); ++l)
        {
          if (*l == rPoint)
            trueRPoint = true;
        }

        if (trueLPoint && trueRPoint)
          ++trueTripletsCount;
      }
    }

    out << "[" << trueTripletsCount << "]";
  }

  out << endl << "Have neighbors: ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    out << "[";

    if (0 == i)
      out << " ";
    else
    {
      set<LxTriplet*> neighbours;

      for (list<LxPoint*>::iterator j = hits[i][1].begin(); j != hits[i][1].end(); ++j)
      {
        LxPoint* rPoint = *j;

        for (list<LxTriplet*>::iterator k = rPoint->triplets.begin(); k != rPoint->triplets.end(); ++k)
        {
          LxTriplet* rTriplet = *k;

          for (list<LxPoint*>::iterator l = hits[i - 1][1].begin(); l != hits[i - 1][1].end(); ++l)
          {
            LxPoint* lPoint = *l;

            for (list<LxTriplet*>::iterator m = lPoint->triplets.begin(); m != lPoint->triplets.end(); ++m)
            {
              LxTriplet* lTriplet = *m;

              for (list<pair<LxTriplet*, Double_t> >::iterator n = rTriplet->neighbours.begin(); n != rTriplet->neighbours.end(); ++n)
              {
                pair<LxTriplet*, Double_t>& tc2 = *n;

                if (tc2.first == lTriplet)
                  neighbours.insert(lTriplet);
              }
            }
          }
        }
      }

      out << neighbours.size();
    }

    out << "]";
  }

  Double_t maxCrDx[LXSTATIONS] = {};
  Double_t crDxDx2[LXSTATIONS] = {};
  Double_t maxCrDy[LXSTATIONS] = {};
  Double_t crDyDy2[LXSTATIONS] = {};
  Double_t maxClDx[LXSTATIONS] = {};
  Double_t clDxDx2[LXSTATIONS] = {};
  Double_t maxClDy[LXSTATIONS] = {};
  Double_t clDyDy2[LXSTATIONS] = {};

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    for (list<LxPoint*>::iterator j = hits[i][1].begin(); j != hits[i][1].end(); ++j)
    {
      LxPoint* cPoint = *j;
      Double_t cTx = cPoint->x / cPoint->z;
      Double_t cTy = cPoint->y / cPoint->z;

      for (list<LxPoint*>::iterator k = hits[i][2].begin(); k != hits[i][2].end(); ++k)
      {
        LxPoint* rPoint = *k;
        Double_t crDeltaZ = rPoint->z - cPoint->z;
        Double_t crTx = (rPoint->x - cPoint->x) / crDeltaZ;
        Double_t crTy = (rPoint->y - cPoint->y) / crDeltaZ;
        Double_t crDx = abs(rPoint->x - cPoint->x - cTx * crDeltaZ);
        Double_t crDy = abs(rPoint->y - cPoint->y - cTy * crDeltaZ);

        if (maxCrDx[i] < crDx)
        {
          maxCrDx[i] = crDx;
          crDxDx2[i] = cPoint->dx * cPoint->dx + rPoint->dx * rPoint->dx;
        }

        if (maxCrDy[i] < crDy)
        {
          maxCrDy[i] = crDy;
          crDyDy2[i] = cPoint->dy * cPoint->dy + rPoint->dy * rPoint->dy;
        }

        for (list<LxPoint*>::iterator l = hits[i][0].begin(); l != hits[i][0].end(); ++l)
        {
          LxPoint* lPoint = *l;
          Double_t clDeltaZ = lPoint->z - cPoint->z;
          Double_t clDx = abs(lPoint->x - cPoint->x - crTx * clDeltaZ);
          Double_t clDy = abs(lPoint->y - cPoint->y - crTy * clDeltaZ);

          if (maxClDx[i] < clDx)
          {
            maxClDx[i] = clDx;
            clDxDx2[i] = lPoint->dx * lPoint->dx + cPoint->dx * cPoint->dx + rPoint->dx * rPoint->dx;
          }

          if (maxClDy[i] < clDy)
          {
            maxClDy[i] = clDy;
            clDyDy2[i] = lPoint->dy * lPoint->dy + cPoint->dy * cPoint->dy + rPoint->dy * rPoint->dy;
          }
        }
      }
    }
  }

  out << endl << "Triplet C-->R X dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
    out << "[" << maxCrDx[i] / sqrt(finder.caSpace.stations[i]->xDispRight2 + crDxDx2[i]) << "]";

  out << endl << "Triplet C-->R Y dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
    out << "[" << maxCrDy[i] / sqrt(finder.caSpace.stations[i]->yDispRight2 + crDyDy2[i]) << "]";

  out << endl << "Triplet C-->L X dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
    out << "[" << maxClDx[i] / sqrt(finder.caSpace.stations[i]->xDispRL2 + clDxDx2[i]) << "]";

  out << endl << "Triplet C-->L Y dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS; ++i)
    out << "[" << maxClDy[i] / sqrt(finder.caSpace.stations[i]->yDispRL2 + clDyDy2[i]) << "]";

  Double_t maxDx[LXSTATIONS - 1] = {};
  Double_t dxDx2[LXSTATIONS - 1] = {};
  Double_t maxDy[LXSTATIONS - 1] = {};
  Double_t dyDy2[LXSTATIONS - 1] = {};
  Double_t maxDtx[LXSTATIONS - 1] = {};
  Double_t dtxDtx2[LXSTATIONS - 1] = {};
  Double_t maxDty[LXSTATIONS - 1] = {};
  Double_t dtyDty2[LXSTATIONS - 1] = {};

  for (Int_t i = 1; i < LXSTATIONS; ++i)
  {
    for (list<LxPoint*>::iterator j = hits[i][1].begin(); j != hits[i][1].end(); ++j)
    {
      LxPoint* rPoint = *j;

      for (list<LxPoint*>::iterator k = hits[i - 1][1].begin(); k != hits[i - 1][1].end(); ++k)
      {
        LxPoint* lPoint = *k;
        Double_t deltaZ = lPoint->z - rPoint->z;

        for (list<LxPoint*>::iterator l = hits[i][0].begin(); l != hits[i][0].end(); ++l)
        {
          LxPoint* rPoint0 = *l;

          for (list<LxPoint*>::iterator m = hits[i][2].begin(); m != hits[i][2].end(); ++m)
          {
            LxPoint* rPoint2 = *m;
            Double_t rdz = rPoint2->z - rPoint0->z;
            Double_t rdz2 = rdz * rdz;
            Double_t rtx = (rPoint2->x - rPoint0->x) / rdz;
            Double_t rty = (rPoint2->y - rPoint0->y) / rdz;
            Double_t rdtx2 = (rPoint0->dx2 + rPoint2->dx2) / rdz2;
            Double_t rdty2 = (rPoint0->dy2 + rPoint2->dy2) / rdz2;

#ifdef OUT_DISP_BY_TRIPLET_DIR
            Double_t dx = abs(lPoint->x - rPoint->x - rtx * deltaZ);
            Double_t dy = abs(lPoint->y - rPoint->y - rty * deltaZ);
#else//OUT_DISP_BY_TRIPLET_DIR
            Double_t dx = abs(lPoint->x - rPoint->x - deltaZ * rPoint->x / rPoint->z);
            Double_t dy = abs(lPoint->y - rPoint->y - deltaZ * rPoint->y / rPoint->z);
#endif//OUT_DISP_BY_TRIPLET_DIR

            if (dx > maxDx[i - 1])
            {
              maxDx[i - 1] = dx;
              dxDx2[i - 1] = lPoint->dx2 + rPoint->dx2;
            }

            if (dy > maxDy[i - 1])
            {
              maxDy[i - 1] = dy;
              dyDy2[i - 1] = lPoint->dy2 + rPoint->dy2;
            }

            for (list<LxPoint*>::iterator n = hits[i - 1][0].begin(); n != hits[i - 1][0].end(); ++n)
            {
              LxPoint* lPoint0 = *n;

              for (list<LxPoint*>::iterator o = hits[i - 1][2].begin(); o != hits[i - 1][2].end(); ++o)
              {
                LxPoint* lPoint2 = *o;
                Double_t ldz = lPoint2->z - lPoint0->z;
                Double_t ldz2 = ldz * ldz;
                Double_t ltx = (lPoint2->x - lPoint0->x) / ldz;
                Double_t lty = (lPoint2->y - lPoint0->y) / ldz;
                Double_t ldtx2 = (lPoint0->dx2 + lPoint2->dx2) / ldz2;
                Double_t ldty2 = (lPoint0->dy2 + lPoint2->dy2) / ldz2;

                Double_t dtx = abs(ltx - rtx);
                Double_t dty = abs(lty - rty);

                if (dtx > maxDtx[i - 1])
                {
                  maxDtx[i - 1] = dtx;
                  dtxDtx2[i - 1] = rdtx2 + ldtx2;
                }

                if (dty > maxDty[i - 1])
                {
                  maxDty[i - 1] = dty;
                  dtyDty2[i - 1] = rdty2 + ldty2;
                }
              }
            }
          }
        }
      }
    }
  }

  out << endl << "X dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
#ifdef OUT_DISP_BY_TRIPLET_DIR
    out << "[" << maxDx[i] / sqrt(finder.caSpace.stations[i + 1]->xOutDispTriplet2 + dxDx2[i]) << "]";
#else//OUT_DISP_BY_TRIPLET_DIR
    out << "[" << maxDx[i] / sqrt(finder.caSpace.stations[i + 1]->xOutDispVertex2 + dxDx2[i]) << "]";
#endif//OUT_DISP_BY_TRIPLET_DIR

  out << endl << "Y dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
#ifdef OUT_DISP_BY_TRIPLET_DIR
    out << "[" << maxDy[i] / sqrt(finder.caSpace.stations[i + 1]->yOutDispTriplet2 + dyDy2[i]) << "]";
#else//OUT_DISP_BY_TRIPLET_DIR
  out << "[" << maxDy[i] / sqrt(finder.caSpace.stations[i + 1]->yOutDispVertex2 + dyDy2[i]) << "]";
#endif//OUT_DISP_BY_TRIPLET_DIR

  out << endl << "Tx dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
    out << "[" << maxDtx[i] / sqrt(finder.caSpace.stations[i + 1]->txInterTripletBreak2 + dtxDtx2[i]) << "]";

  out << endl << "Ty dispersions: ";

  for (Int_t i = 0; i < LXSTATIONS - 1; ++i)
    out << "[" << maxDty[i] / sqrt(finder.caSpace.stations[i + 1]->tyInterTripletBreak2 + dtyDty2[i]) << "]";

  out << endl;
}

static Double_t InterLinesDist(LxTrack* track1, LxTrack* track2)
{
  LxTriplet* triplet1 = track1->branches[0];
  LxPoint* point1 = triplet1->center;
  Double_t x1 = point1->x;
  Double_t y1 = point1->y;
  Double_t z1 = point1->z;

  //Double_t tx1 = point1->x / point1->z;
  //Double_t delta1 = triplet1->tx - tx1;

  Double_t ax = triplet1->tx;// - 2 * delta1;
  Double_t ay = triplet1->ty;
  Double_t az = 1.0;

  LxTriplet* triplet2 = track2->branches[0];
  LxPoint* point2 = triplet2->center;
  Double_t x2 = point2->x;
  Double_t y2 = point2->y;
  Double_t z2 = point2->z;

  //Double_t tx2 = point2->x / point2->z;
  //Double_t delta2 = triplet2->tx - tx2;

  Double_t bx = triplet2->tx;// - 2 * delta2;
  Double_t by = triplet2->ty;
  Double_t bz = 1.0;

  Double_t A = ay * bz - az * by;
  Double_t B = az * bx - ax * bz;
  Double_t C = ax * by - ay * bx;
  Double_t P = A * x2 + B * y2 + C * z2;

  Double_t ABCLen = sqrt(A * A + B * B + C * C);

  Double_t cosA = A / ABCLen;
  Double_t cosB = B / ABCLen;
  Double_t cosC = C / ABCLen;
  Double_t p = P / ABCLen;
  Double_t interTrackDistance = abs(cosA * x1 + cosB * y1 + cosC * z1 - p);

  return interTrackDistance;
}

static Double_t RecoTrackChi2(LxTrack* recoTrack)
{
  Double_t breakChi2 = 0;

  for (Int_t j = recoTrack->length - 1; j > 0; --j)
  {
    LxTriplet* rTriplet = recoTrack->branches[j];
    LxTriplet* lTriplet = recoTrack->branches[j - 1];

    LxStation* station = rTriplet->center->layer->station;
    breakChi2 += (lTriplet->tx - rTriplet->tx) * (lTriplet->tx - rTriplet->tx) / (station->txInterTripletBreak2 + lTriplet->dtx2 + rTriplet->dtx2) +
        (lTriplet->ty - rTriplet->ty) * (lTriplet->ty - rTriplet->ty) / (station->tyInterTripletBreak2 + lTriplet->dty2 + rTriplet->dty2);
  }

  breakChi2 /= 2 * (recoTrack->length - 1);

  return breakChi2;
}

void LxEff::CalcRecoEff(bool joinExt)
{
  Double_t result = 0;
  static Int_t signalRecoTracks = 0;
  static Int_t signalMCTracks = 0;
//  static Int_t bgrRecoTracks = 0;
//  static Int_t bgrMCTracks = 0;
  static Int_t mc2recoZeroMatched = 0;
  static Int_t mc2recoIncomplete = 0;

  bool hasPositiveMCTrack = false;
  bool hasNegativeMCTrack = false;

  for (vector<LxMCTrack>::iterator i = finder.MCTracks.begin(); i != finder.MCTracks.end(); ++i)
  {
    LxMCTrack& mcTrack = *i;
    Int_t pdgCode = mcTrack.pdg;
    map<LxTrack*, Int_t> recoTracks;// Mapped value is the number of common points in MC and reconstructed tracks.
    memset(mcTrack.hitsOnStations, 0, sizeof(mcTrack.hitsOnStations));
    bool isSignal = true;
    list<LxPoint*> mcTrackHits[LXSTATIONS][LXLAYERS];

    for (vector<LxMCPoint*>::iterator j = mcTrack.Points.begin(); j != mcTrack.Points.end(); ++j)
    {
      LxMCPoint* pMCPoint = *j;

      for (list<LxPoint*>::iterator k = pMCPoint->lxPoints.begin(); k != pMCPoint->lxPoints.end(); ++k)
      {
        LxPoint* point = *k;
        mcTrack.hitsOnStations[point->layer->station->stationNumber][point->layer->layerNumber] = true;
        mcTrackHits[point->layer->station->stationNumber][point->layer->layerNumber].push_back(point);
        LxTrack* track = point->track;

        if (0 == track)
          continue;
        
        if (track->matched)
          continue;

        if (track->clone)
          continue;

        map<LxTrack*, int>::iterator l = recoTracks.find(track);
        
        if (l != recoTracks.end())
          ++(l->second);
        else
          recoTracks[track] = 1;
      }
    }
    
    bool enoughHits = true;
    mcTrack.stationsWithHits = 0;
    mcTrack.layersWithHits = 0;

    for (Int_t j = 0; j < LXSTATIONS; ++j)
    {
      Int_t hitsOnSt = 0;

      for (Int_t k = 0; k < LXLAYERS; ++k)
      {
        if (mcTrack.hitsOnStations[j][k])
        {
          ++hitsOnSt;
          ++mcTrack.layersWithHits;
        }
      }

      if (hitsOnSt < 3)
      {
        if (j < finder.caSpace.stationsInAlgo)
          enoughHits = false;
      }
      else
        ++mcTrack.stationsWithHits;
    }

    Double_t pt2 = mcTrack.px * mcTrack.px + mcTrack.py * mcTrack.py;

    if (!enoughHits || mcTrack.mother_ID >= 0 || (pdgCode != 13 && pdgCode != -13) || (finder.pPtCut && (mcTrack.p < 3.0 || pt2 < 1.0)))
      isSignal = false;
    else
      ++signalMCTracks;

    if (isSignal)
    {
      if (-13 == pdgCode)
        hasPositiveMCTrack = true;
      else
        hasNegativeMCTrack = true;
    }

    LxTrack* matchTrack = 0;
    Int_t matchedPoints = 0;

    for (map<LxTrack*, Int_t>::iterator j = recoTracks.begin(); j != recoTracks.end(); ++j)
    {
      if (0 == matchTrack || matchedPoints < j->second)
      {
        matchTrack = j->first;
        matchedPoints = j->second;
      }
    }

    if (0 == matchTrack)
    {
#ifdef MAKE_EFF_CALC
      //finder.incomplete_events << finder.eventNumber << " matched points: " << matchedPoints << " ; MC points: " << mcTrack.Points.size() << endl;
      //finder.caSpace.CheckArray(finder.incomplete_events, mcTrack);
#endif//MAKE_EFF_CALC
      //cout << "MC track does not have common point with a reconstructed" << endl;
      if (isSignal)
      {
        ++mc2recoZeroMatched;
        AnalyzeNotMatchedMC(finder.incomplete_events, mcTrackHits);
      }

#ifdef CLUSTER_MODE
      if (isSignal)
        DumpNotMatchedMC(mcTrackHits);
#endif//CLUSTER_MODE

      continue;
    }

    Int_t numberOfMuchMCPoints = LXLAYERS * finder.caSpace.stationsInAlgo;

    if (matchedPoints < 0.7 * numberOfMuchMCPoints)
    {
#ifdef MAKE_EFF_CALC
      //finder.incomplete_events << finder.eventNumber << " matched points: " << matchedPoints << " ; MC points: " << mcTrack.Points.size() << endl;
      //finder.caSpace.CheckArray(finder.incomplete_events, mcTrack);
#endif//MAKE_EFF_CALC
      //cout << "MC track have only " << matchedPoints << " common points with reconstructed" << endl;
      if (isSignal)
      {
        ++mc2recoIncomplete;
        AnalyzeNotMatchedMC(finder.incomplete_events, mcTrackHits);
      }

#ifdef CLUSTER_MODE
      if (isSignal)
        DumpNotMatchedMC(mcTrackHits);
#endif//CLUSTER_MODE

      continue;
    }

    matchTrack->mcTrack = &mcTrack;
    matchTrack->matched = true;

    if (!isSignal)
      continue;

#ifdef CALC_LINK_WITH_STS_EFF
    if (0 != matchTrack && matchedPoints >= 0.7 * numberOfMuchMCPoints)
      matchTrack->mcTracks.push_back(&mcTrack);
#endif//CALC_LINK_WITH_STS_EFF

    if (!joinExt)
      ++signalRecoTracks;
    else
    {
      if (0 == matchTrack->externalTrack)
      {
        if (0 == mcTrack.externalTrack)
          ++signalRecoTracks;
      }
      else if (matchTrack->externalTrack->track == mcTrack.externalTrack)
        ++signalRecoTracks;
    }
  }
  
  result = 100 * signalRecoTracks;
  result /= signalMCTracks;
  
#ifdef MAKE_EFF_CALC
  //if (foundNow != mcNow)
    //finder.incomplete_events << finder.eventNumber << endl;
#endif//MAKE_EFF_CALC

  cout << "LxEff::CalcRecoEff signal efficiency: " << result << "( " << signalRecoTracks << " / " << signalMCTracks << " )" << endl;
  cout << "LxEff::CalcRecoEff: zero-matched: " << mc2recoZeroMatched << ", incomplete: " << mc2recoIncomplete << endl;

  bool hasPositiveTrack = false;
  bool hasNegativeTrack = false;

  for (list<LxTrack*>::iterator i = finder.caSpace.tracks.begin(); i != finder.caSpace.tracks.end(); ++i)
  {
    LxTrack* recoTrack = *i;

    if (recoTrack->clone)
      continue;

    //if (recoTrack->length > finder.caSpace.stationsInAlgo)
      //continue;

    LxTriplet* firstTriplet = recoTrack->branches[0];
    LxPoint* firstPoint = firstTriplet->center;
    Double_t particleSign = firstTriplet->tx - firstPoint->x / firstPoint->z;
    LxTriplet* secondTriplet = recoTrack->branches[1];
    LxPoint* secondPoint = secondTriplet->center;
    Double_t tx = (secondPoint->x - firstPoint->x) / (secondPoint->z - firstPoint->z);
    Double_t particleSign2 = tx - firstPoint->x / firstPoint->z;
//    Double_t yAtZ0 = firstPoint->y - firstTriplet->ty * firstPoint->z;
//    Double_t breakChi2 = RecoTrackChi2(recoTrack);

    //if (-9 <= yAtZ0 && 9 >= yAtZ0)
    //if (breakChi2 <= 3)
    {
      if (particleSign > 0 && particleSign2 > 0)
        hasPositiveTrack = true;
      else if (particleSign < 0 && particleSign2 < 0)
        hasNegativeTrack = true;
    }
  }

  static Int_t numberOfJpsi = 0;

  if (hasPositiveTrack && hasNegativeTrack)
    ++numberOfJpsi;

  static Int_t numberOfMCJpsi = 0;
  static Int_t numberOfTrueJpsi = 0;

  if (hasPositiveMCTrack && hasNegativeMCTrack)
  {
    ++numberOfMCJpsi;

    if (hasPositiveTrack && hasNegativeTrack)
      ++numberOfTrueJpsi;
  }

  cout << "LxEff::CalcRecoEff: number of J/Psi hypoteses " << numberOfJpsi << endl;
  result = 100 * numberOfTrueJpsi;
  result /= numberOfMCJpsi;
  cout << "LxEff::CalcRecoEff: J/Psi efficiency " << result << " % ( " << numberOfTrueJpsi << " / " << numberOfMCJpsi << " )" << endl;

  return;

  // The code below is mostly dedicated to counting ghosts.
  static Int_t recoTracks = 0;
  static Int_t matchedMCTracks = 0;
  static Int_t completeleyUnmatched = 0;
  static Int_t notEnoughPoints = 0;
  static Int_t notEnoughStations = 0;
  static Int_t incompleteStations[LXSTATIONS] = {};
  static Int_t doesntHaveStsPart = 0;
  static Int_t doesntHaveStsPart1 = 0;
  static Int_t doesntHaveStsPart2 = 0;
  static Int_t unmatchedButHaveStsPart = 0;

  for (list<LxTrack*>::iterator i = finder.caSpace.tracks.begin(); i != finder.caSpace.tracks.end(); ++i)
  {
    //++recoTracks;
    LxTrack* recoTrack = *i;

    if (recoTrack->clone)
      continue;

    map<LxMCTrack*, Int_t> mcTracks;// Mapped value is the number of common points in MC and reconstructed tracks.

#ifdef CLUSTER_MODE
    for (Int_t ii = 0; ii < recoTrack->length; ++ii)
    {
      LxRay* ray = recoTrack->rays[ii];

      for (list<LxPoint*>::iterator j = ray->clusterPoints.begin(); j != ray->clusterPoints.end(); ++j)
      {
        LxPoint* recoPoint = *j;
#else//CLUSTER_MODE
    for (Int_t j = 0; j < finder.caSpace.stationsInAlgo * LXLAYERS; ++j)
    {
      LxPoint* recoPoint = recoTrack->points[j];
#endif//CLUSTER_MODE

      if (0 == recoPoint)
        continue;

      for (list<LxMCPoint*>::iterator k = recoPoint->mcPoints.begin(); k != recoPoint->mcPoints.end(); ++k)
      {
        LxMCPoint* mcPoint = *k;
        LxMCTrack* mcTrack = mcPoint->track;

        if (0 == mcTrack)
          continue;

        map<LxMCTrack*, Int_t>::iterator mcIter = mcTracks.find(mcTrack);

        if (mcIter == mcTracks.end())
          mcTracks[mcTrack] = 1;
        else
          ++(mcIter->second);
      }
#ifdef CLUSTER_MODE
    }
#endif//CLUSTER_MODE
    }// for (Int_t j = 0; j < (LXSTATIONS - LXFIRSTSTATION) * LXLAYERS; ++j)

    LxMCTrack* bestMatch = 0;
    Int_t matchedPoints = 0;

    for (map<LxMCTrack*, Int_t>::iterator j = mcTracks.begin(); j != mcTracks.end(); ++j)
    {
      if (0 == bestMatch || j->second > matchedPoints)
      {
        bestMatch = j->first;
        matchedPoints = j->second;
      }
    }

    if (0 == bestMatch)
    {
      ++completeleyUnmatched;
      cout << "This reconstructed track is not intersected with any MC track in any point" << endl;

      if (!joinExt || 0 != recoTrack->externalTrack)
        ++recoTracks;

      continue;
    }

    ++recoTracks;

    bool unmatched = false;

    if (matchedPoints < 0.7 * finder.caSpace.stationsInAlgo * LXLAYERS)
    {
      ++notEnoughPoints;
      cout << "This reconstructed track match with an MC track in only " << matchedPoints << "points" << endl;
      unmatched = true;
    }

    if (bestMatch->stationsWithHits < finder.caSpace.stationsInAlgo)
    {
      ++notEnoughStations;

#ifdef LXDRAW
      LxDraw drawer;
      drawer.ClearView();
      drawer.DrawInputHits();
      drawer.DrawMCTracks();
      drawer.DrawRays();

#ifdef LXDRAWSAVE
      char drawSaveName[64];
      sprintf(drawSaveName, "MC_Rays_%d_", finder.eventNumber);
      drawer.SaveCanvas(drawSaveName);
#endif//LXDRAWSAVE

#ifdef LXDRAWWAIT
      gPad->WaitPrimitive();
#endif//LXDRAWWAIT
#endif//LXDRAW

      for (Int_t j = 0; j < finder.caSpace.stationsInAlgo; ++j)
      {
        Int_t hitsOnStation = 0;

        for (Int_t k = 0; k < LXLAYERS; ++k)
        {
          if (bestMatch->hitsOnStations[j][k])
            ++hitsOnStation;
        }

        if (hitsOnStation < 3)
          ++incompleteStations[j];
      }

      cout << "The matched MC track has hits not on all stations. Only on: " << bestMatch->stationsWithHits << endl;
      unmatched = true;
    }

    if (0 == bestMatch->externalTrack)
    {
      ++doesntHaveStsPart;

      if (unmatched)
        ++doesntHaveStsPart1;
      else
        ++doesntHaveStsPart2;
    }

    if (!unmatched)
    {
      if (!joinExt)
        ++matchedMCTracks;
      else
      {
        if (0 == recoTrack->externalTrack)
        {
          if (0 == bestMatch->externalTrack)
            ++matchedMCTracks;
          else
            cout << "LxEff::CalcRecoEff: STS track not found" << endl;
        }
        else
        {
          if (bestMatch->externalTrack == recoTrack->externalTrack->track)
            ++matchedMCTracks;
          else
            cout << "LxEff::CalcRecoEff: STS track determined wrongly" << endl;
        }
      }
    }
    else if (0 != bestMatch->externalTrack)
      ++unmatchedButHaveStsPart;
  }// for (list<LxTrack*>::iterator i = finder.caSpace.tracks.begin(); i != finder.caSpace.tracks.end(); ++i)

  result = 100 * matchedMCTracks;
  result /= recoTracks;
  cout << "LxEff::CalcRecoEff matching reconstucted efficiency: " << result << "( " << matchedMCTracks << " / " << recoTracks << " )" << endl;
  cout << "LxEff::CalcRecoEff 0-matched: " << completeleyUnmatched << ", not enough points: " << notEnoughPoints <<
      ", not enough stations: " << notEnoughStations << " : ";

  for (Int_t i = 0; i < finder.caSpace.stationsInAlgo; ++i)
    cout << "[" << incompleteStations[i] << "]";

  cout << endl;
  cout << "LxEff::CalcRecoEff doesn't have an STS part: " << doesntHaveStsPart <<
      ", unmatched: " << doesntHaveStsPart1 << ", matched: " << doesntHaveStsPart2 << endl;
  cout << "LxEff::CalcRecoEff unmatched but have an STS part: " << unmatchedButHaveStsPart << endl;
}// void LxEff::CalcRecoEff(bool joinExt)

void LxFinderTriplet::MatchRecoToMC()
{
  static Int_t recoTracks = 0;
  static Int_t matchedTracks = 0;
  LxTrack* positiveSignal = 0;
  Double_t positiveDelta = 0;
  Double_t negativeDelta = 0;
  LxTrack* negativeSignal = 0;
  list<LxTrack*> positiveBgrs;
  list<LxTrack*> negativeBgrs;

  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* recoTrack = *i;

    if (recoTrack->clone)
      continue;

    ++recoTracks;

    map<LxMCTrack*, Int_t> mcTracks;// Mapped value is the number of common points in MC and reconstructed tracks.

    for (Int_t j = 0; j < caSpace.stationsInAlgo * LXLAYERS; ++j)
    {
      LxPoint* recoPoint = recoTrack->points[j];

      if (0 == recoPoint)
        continue;

      for (list<LxMCPoint*>::iterator k = recoPoint->mcPoints.begin(); k != recoPoint->mcPoints.end(); ++k)
      {
        LxMCPoint* mcPoint = *k;
        LxMCTrack* mcTrack = mcPoint->track;

        if (0 == mcTrack)
          continue;

        map<LxMCTrack*, Int_t>::iterator mcIter = mcTracks.find(mcTrack);

        if (mcIter == mcTracks.end())
          mcTracks[mcTrack] = 1;
        else
          ++(mcIter->second);
      }// for (list<LxMCPoint*>::iterator k = recoPoint->mcPoints.begin(); k != recoPoint->mcPoints.end(); ++k)
    }// for (Int_t j = 0; j < caSpace.stationsInAlgo * LXLAYERS; ++j)

    LxMCTrack* bestMatch = 0;
    Int_t matchedPoints = 0;

    for (map<LxMCTrack*, Int_t>::iterator j = mcTracks.begin(); j != mcTracks.end(); ++j)
    {
      if (0 == bestMatch || j->second > matchedPoints)
      {
        bestMatch = j->first;
        matchedPoints = j->second;
      }
    }

    LxTriplet* firstTriplet = recoTrack->branches[0];
    LxPoint* firstPoint = firstTriplet->center;
    Double_t particleSign = firstTriplet->tx - firstPoint->x / firstPoint->z;
    Double_t yAtZ0 = firstPoint->y - firstTriplet->ty * firstPoint->z;
    LxTriplet* secondTriplet = recoTrack->branches[1];
    LxPoint* secondPoint = secondTriplet->center;
    Double_t yAtZ0_2 = firstPoint->y - firstPoint->z * (secondPoint->y - firstPoint->y) / (secondPoint->z - firstPoint->z);
    Double_t breakChi2 = RecoTrackChi2(recoTrack);

    if (0 == bestMatch || matchedPoints < 0.7 * caSpace.stationsInAlgo * LXLAYERS)
    {
      if (particleSign > 0)
        positiveBgrs.push_back(recoTrack);
      else if (particleSign < 0)
        negativeBgrs.push_back(recoTrack);

      bgrYAtZ0->Fill(yAtZ0);
      bgrYAtZ0_2->Fill(yAtZ0_2);
      backgroundChi2Histo->Fill(breakChi2);

      continue;
    }

    ++matchedTracks;

    if (0 > bestMatch->mother_ID && (-13 == bestMatch->pdg || 13 == bestMatch->pdg))
    {
      if (particleSign > 0 && -13 == bestMatch->pdg)
      {
        positiveSignal = recoTrack;
        positiveDelta = particleSign;
      }
      else if (particleSign < 0 && 13 == bestMatch->pdg)
      {
        negativeSignal = recoTrack;
        negativeDelta = particleSign;
      }

      signalYAtZ0->Fill(yAtZ0);
      signalYAtZ0_2->Fill(yAtZ0_2);
      signalChi2Histo->Fill(breakChi2);
    }
    else
    {
      if (particleSign > 0)
        positiveBgrs.push_back(recoTrack);
      else if (particleSign < 0)
        negativeBgrs.push_back(recoTrack);

      bgrYAtZ0->Fill(yAtZ0);
      bgrYAtZ0_2->Fill(yAtZ0_2);
      backgroundChi2Histo->Fill(breakChi2);
    }
  }// for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)

  Double_t result = 100 * matchedTracks;
  result /= recoTracks;
  cout << "LxFinderTriplet::MatchRecoToMC(): efficiency: " << result << " % ( " << matchedTracks << " / " << recoTracks << " )" << endl;

  if (0 != positiveSignal && 0 != negativeSignal)
  {
    signalInterTracksDistance->Fill(InterLinesDist(positiveSignal, negativeSignal));
    signalSignDefect->Fill(positiveDelta + negativeDelta);
  }

  LxTrack* posBgr = 0;
  LxTrack* negBgr = 0;
  Double_t bgrDist = 10000;
  Double_t signDefect = 10000;

  for (list<LxTrack*>::iterator i = positiveBgrs.begin(); i != positiveBgrs.end(); ++i)
  {
    LxTrack* track1 = *i;

    for (list<LxTrack*>::iterator j = negativeBgrs.begin(); j != negativeBgrs.end(); ++j)
    {
      LxTrack* track2 = *j;
      Double_t dist = InterLinesDist(track1, track2);

      if (dist < bgrDist)
      {
        posBgr = track1;
        negBgr = track2;
        bgrDist = dist;
      }

      LxTriplet* firstTriplet1 = track1->branches[0];
      LxPoint* firstPoint1 = firstTriplet1->center;
      Double_t particleSign1 = firstTriplet1->tx - firstPoint1->x / firstPoint1->z;

      LxTriplet* firstTriplet2 = track2->branches[0];
      LxPoint* firstPoint2 = firstTriplet2->center;
      Double_t particleSign2 = firstTriplet2->tx - firstPoint2->x / firstPoint2->z;

      Double_t defect = particleSign1 + particleSign2;

      if (abs(defect) < abs(signDefect))
        signDefect = defect;
    }
  }// for (list<LxTrack*>::iterator i = positiveBgrs.begin(); i != positiveBgrs.end(); ++i)

  if (0 != posBgr && 0 != negBgr)
  {
    bgrInterTracksDistance->Fill(bgrDist);
    bgrSignDefect->Fill(signDefect);
  }
}// void LxFinderTriplet::MatchRecoToMC()
