#include "LxCATriplets.h"
#include "kdtree++/kdtree.hpp"
#include <iostream>
#include "base/CbmLitToolFactory.h"
#include "utils/CbmLitConverterFairTrackParam.h"

using namespace std;

static Double_t errCoeffTripletRXs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffTripletRYs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffTripletRLXs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffTripletRLYs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffInterTripletXs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffInterTripletYs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffInterTripletTxs[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };
static Double_t errCoeffInterTripletTys[] = { 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxPoint
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxPoint::~LxPoint()
{
  for (list<LxTriplet*>::iterator i = triplets.begin(); i != triplets.end(); ++i)
    delete *i;

#ifdef USE_SEGMENTS
  for (list<LxSegment*>::iterator i = segments.begin(); i != segments.end(); ++i)
    delete *i;
#endif//USE_SEGMENTS
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxLayer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct KDTPointWrap
{
  LxPoint* point;
  Double_t data[2];

  explicit KDTPointWrap(LxPoint* p) : point(p)
  {
    data[0] = p->x;
    data[1] = p->y;
  }

  KDTPointWrap(Double_t x, Double_t y) : point(0)// This constructor is used when setting search-range bounds.
  {
    data[0] = x;
    data[1] = y;
  }

  // Stuff required by libkdtree++
  typedef Double_t value_type;

  value_type operator[] (size_t n) const
  {
    return data[n];
  }
};

typedef KDTree::KDTree<2, KDTPointWrap> KDTPointsStorageType;

LxLayer::LxLayer(LxStation* Station, int LayerNumber) : points_handle(0), zCoord(0), station(Station), layerNumber(LayerNumber)
{
  points_handle = new KDTPointsStorageType;
}

LxLayer::~LxLayer()
{
  KDTPointsStorageType* points = static_cast<KDTPointsStorageType*> (points_handle);
  Clear();
  delete points;
}

void LxLayer::Clear()
{
  KDTPointsStorageType* points = static_cast<KDTPointsStorageType*> (points_handle);

  for (KDTPointsStorageType::iterator i = points->begin(); i != points->end(); ++i)
  {
    KDTPointWrap& wrap = const_cast<KDTPointWrap&> (*i);
    LxPoint* point = wrap.point;
    delete point;
  }

  points->clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxStation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxStation::LxStation(LxSpace* sp, Int_t stN) : space(sp), stationNumber(stN), zCoord(0), xDispLeft(0),
    yDispLeft(0), xDispRight(0), yDispRight(0), xDispLeft2(0), yDispLeft2(0), xDispRight2(0), yDispRight2(0), xDispRL(0),
    yDispRL(0), xDispRL2(0), yDispRL2(0),
#ifdef OUT_DISP_BY_TRIPLET_DIR
    xOutDispTriplet(0), yOutDispTriplet(0), xOutDispTriplet2(0), yOutDispTriplet2(0),
#else//OUT_DISP_BY_TRIPLET_DIR
    xOutDispVertex(0), yOutDispVertex(0), xOutDispVertex2(0), yOutDispVertex2(0),
#endif//OUT_DISP_BY_TRIPLET_DIR
#ifdef USE_SEGMENTS
    txBreakLeft(0), tyBreakLeft(0),
    txBreakRight(0), tyBreakRight(0), txBreakLeft2(0), tyBreakLeft2(0), txBreakRight2(0), tyBreakRight2(0),
    txInterStationBreak(0), tyInterStationBreak(0), txInterStationBreak2(0), tyInterStationBreak2(0),
#else//USE_SEGMENTS
    txInterTripletBreak(0), tyInterTripletBreak(0), txInterTripletBreak2(0), tyInterTripletBreak2(0),
#endif//USE_SEGMENTS
    errCoeffTripletRX(errCoeffTripletRXs[stN]), errCoeffTripletRX2(errCoeffTripletRX * errCoeffTripletRX),
    errCoeffTripletRY(errCoeffTripletRYs[stN]), errCoeffTripletRY2(errCoeffTripletRY * errCoeffTripletRY),
    errCoeffTripletRLX(errCoeffTripletRLXs[stN]), errCoeffTripletRLX2(errCoeffTripletRLX * errCoeffTripletRLX),
    errCoeffTripletRLY(errCoeffTripletRLYs[stN]), errCoeffTripletRLY2(errCoeffTripletRLY * errCoeffTripletRLY),
    errCoeffInterTripletX(errCoeffInterTripletXs[stN]), errCoeffInterTripletX2(errCoeffInterTripletX * errCoeffInterTripletX),
    errCoeffInterTripletY(errCoeffInterTripletYs[stN]), errCoeffInterTripletY2(errCoeffInterTripletY * errCoeffInterTripletY),
    errCoeffInterTripletTx(errCoeffInterTripletTxs[stN]), errCoeffInterTripletTx2(errCoeffInterTripletTx * errCoeffInterTripletTx),
    errCoeffInterTripletTy(errCoeffInterTripletTys[stN]), errCoeffInterTripletTy2(errCoeffInterTripletTy * errCoeffInterTripletTy)
{
  for (Int_t i = 0; i < LXLAYERS; ++i)
    layers[i] = new LxLayer(this, i);
}

LxStation::~LxStation()
{
  for (Int_t i = 0; i < LXLAYERS; ++i)
    delete layers[i];
}

void LxStation::Clear()
{
  for (Int_t i = 0; i < LXLAYERS; ++i)
    layers[i]->Clear();
}

void LxStation::BuildTriplets()
{
  LxLayer* lLayer = layers[0];
  LxLayer* cLayer = layers[1];
  LxLayer* rLayer = layers[2];

  KDTPointsStorageType* lPoints = static_cast<KDTPointsStorageType*> (lLayer->points_handle);
  KDTPointsStorageType* cPoints = static_cast<KDTPointsStorageType*> (cLayer->points_handle);
  KDTPointsStorageType* rPoints = static_cast<KDTPointsStorageType*> (rLayer->points_handle);

  //cout << "[" << stationNumber << "] LxStation::BuildTriplets() hit count: " << lPoints->size() << " : " <<
      //cPoints->size() << " : " << rPoints->size() << endl;

  Double_t lZCoord = lLayer->zCoord;
  Double_t rZCoord = rLayer->zCoord;

  UInt_t tripletCount = 0;

  for (KDTPointsStorageType::iterator i = cPoints->begin(); i != cPoints->end(); ++i)
  {
    KDTPointWrap& cWrap = const_cast<KDTPointWrap&> (*i);
    LxPoint* cPoint = cWrap.point;
    Double_t deltaZc = rZCoord - cPoint->z;
    Double_t cTx = cPoint->x / cPoint->z;
    Double_t cTy = cPoint->y / cPoint->z;
    Double_t rX = cPoint->x + cTx * deltaZc;
    Double_t rY = cPoint->y + cTy * deltaZc;

    // Search corresponding points at right layer.
    Double_t rXLimit = errCoeffTripletRX * sqrt(xDispRight2 + 2 * cPoint->dx2);
    Double_t rYLimit = errCoeffTripletRY * sqrt(yDispRight2 + 2 * cPoint->dy2);
    KDTPointWrap rBoundsWrap(rX - rXLimit, rY - rYLimit);
    KDTree::_Region<2, KDTPointWrap, Double_t, KDTree::_Bracket_accessor<KDTPointWrap>,
      std::less<KDTree::_Bracket_accessor<KDTPointWrap>::result_type> > rRange(rBoundsWrap);
    rRange.set_low_bound(rBoundsWrap, 0);
    rRange.set_low_bound(rBoundsWrap, 1);
    rBoundsWrap.data[0] = rX + rXLimit;
    rBoundsWrap.data[1] = rY + rYLimit;
    rRange.set_high_bound(rBoundsWrap, 0);
    rRange.set_high_bound(rBoundsWrap, 1);
    list<KDTPointWrap> rNeighbours;
    rPoints->find_within_range(rRange, back_insert_iterator<list<KDTPointWrap> > (rNeighbours));

    for (list<KDTPointWrap>::iterator j = rNeighbours.begin(); j != rNeighbours.end(); ++j)
    {
      KDTPointWrap& rWrap = *j;
      LxPoint* rPoint = rWrap.point;
      Double_t rDeltaX = rPoint->x - rX;
      Double_t rDeltaY = rPoint->y - rY;
      Double_t chi2R = rDeltaX * rDeltaX / (xDispRight2 + cPoint->dx2 + rPoint->dx2) +
          rDeltaY * rDeltaY / (yDispRight2 + cPoint->dy2 + rPoint->dy2);

      // Search a point on left layer which corresponds to this pair of points: central and right.
      Double_t deltaZr = rPoint->z - cPoint->z;
      Double_t tx = (rPoint->x - cPoint->x) / deltaZr;
      Double_t ty = (rPoint->y - cPoint->y) / deltaZr;
      deltaZr = lZCoord - cPoint->z;
      Double_t lX = cPoint->x + tx * deltaZr;
      Double_t lY = cPoint->y + ty * deltaZr;
      Double_t lXLimit = errCoeffTripletRLX * sqrt(xDispRL2 + 2 * cPoint->dx2 + rPoint->dx2);
      Double_t lYLimit = errCoeffTripletRLY * sqrt(yDispRL2 + 2 * cPoint->dy2 + rPoint->dy2);
      KDTPointWrap lBoundsWrap(lX - lXLimit, lY - lYLimit);
      KDTree::_Region<2, KDTPointWrap, Double_t, KDTree::_Bracket_accessor<KDTPointWrap>,
        std::less<KDTree::_Bracket_accessor<KDTPointWrap>::result_type> > lRange(lBoundsWrap);
      lRange.set_low_bound(lBoundsWrap, 0);
      lRange.set_low_bound(lBoundsWrap, 1);
      lBoundsWrap.data[0] = lX + lXLimit;
      lBoundsWrap.data[1] = lY + lYLimit;
      lRange.set_high_bound(lBoundsWrap, 0);
      lRange.set_high_bound(lBoundsWrap, 1);
      list<KDTPointWrap> lNeighbours;
      lPoints->find_within_range(lRange, back_insert_iterator<list<KDTPointWrap> > (lNeighbours));

      for (list<KDTPointWrap>::iterator k = lNeighbours.begin(); k != lNeighbours.end(); ++k)
      {
        KDTPointWrap& lWrap = *k;
        LxPoint* lPoint = lWrap.point;
        Double_t lDeltaX = lPoint->x - lX;
        Double_t lDeltaY = lPoint->y - lY;
        Double_t chi2 = chi2R + lDeltaX * lDeltaX / (xDispRL2 + lPoint->dx2 + cPoint->dx2 + rPoint->dx2) +
            lDeltaY * lDeltaY / (yDispRL2 + lPoint->dy2 + cPoint->dy2 + rPoint->dy2);
        LxTriplet* triplet = new LxTriplet(lPoint, cPoint, rPoint, chi2);
        cPoint->triplets.push_back(triplet);
        ++tripletCount;
      }
    }
  }

  //cout << "[" << stationNumber << "] LxStation::BuildTriplets() triplet count: " << tripletCount << endl;
}

#ifdef USE_SEGMENTS

void LxStation::BuildSegments()
{
  LxStation* lStation = space->stations[stationNumber - 1];
  Double_t lZCoord = lStation->zCoord;
  KDTPointsStorageType* rPoints = static_cast<KDTPointsStorageType*> (layers[1]->points_handle);
  KDTPointsStorageType* lPoints = static_cast<KDTPointsStorageType*> (lStation->layers[1]->points_handle);

  UInt_t segmentCount = 0;

  for (KDTPointsStorageType::iterator i = rPoints->begin(); i != rPoints->end(); ++i)
  {
    KDTPointWrap& rWrap = const_cast<KDTPointWrap&> (*i);
    LxPoint* rPoint = rWrap.point;

    if (rPoint->triplets.empty())
      continue;

    // Search corresponding points at the left station.
    Double_t txVertex = rPoint->x / rPoint->z;
    Double_t tyVertex = rPoint->y / rPoint->z;
    Double_t deltaZ = lZCoord - rPoint->z;
    Double_t lX = rPoint->x + txVertex * deltaZ;
    Double_t lY = rPoint->y + tyVertex * deltaZ;
    Double_t lXLimit = errorXcoeff * sqrt(xOutDispVertex2 + 2 * rPoint->dx2);
    Double_t lYLimit = errorYcoeff * sqrt(yOutDispVertex2 + 2 * rPoint->dy2);
    KDTPointWrap lBoundsWrap(lX - lXLimit, lY - lYLimit);
    KDTree::_Region<2, KDTPointWrap, Double_t, KDTree::_Bracket_accessor<KDTPointWrap>,
      std::less<KDTree::_Bracket_accessor<KDTPointWrap>::result_type> > lRange(lBoundsWrap);
    lRange.set_low_bound(lBoundsWrap, 0);
    lRange.set_low_bound(lBoundsWrap, 1);
    lBoundsWrap.data[0] = lX + lXLimit;
    lBoundsWrap.data[1] = lY + lYLimit;
    lRange.set_high_bound(lBoundsWrap, 0);
    lRange.set_high_bound(lBoundsWrap, 1);
    list<KDTPointWrap> lNeighbours;
    lPoints->find_within_range(lRange, back_insert_iterator<list<KDTPointWrap> > (lNeighbours));

    for (list<KDTPointWrap>::iterator k = lNeighbours.begin(); k != lNeighbours.end(); ++k)
    {
      KDTPointWrap& lWrap = *k;
      LxPoint* lPoint = lWrap.point;

      if (lPoint->triplets.empty())
        continue;

      deltaZ = lPoint->z - rPoint->z;
      Double_t deltaZ2 = deltaZ * deltaZ;
      Double_t tx = (lPoint->x - rPoint->x) / deltaZ;
      Double_t ty = (lPoint->y - rPoint->y) / deltaZ;
      Double_t dtx2 = (lPoint->dx2 + rPoint->dx2) / deltaZ2;
      Double_t dty2 = (lPoint->dy2 + rPoint->dy2) / deltaZ2;
      LxSegment* segment = 0;

      for (list<LxTriplet*>::iterator l = rPoint->triplets.begin(); l != rPoint->triplets.end(); ++l)
      {
        LxTriplet* rTriplet = *l;
        Double_t rDeltaTx = rTriplet->tx - tx;
        Double_t rDeltaTx2 = rDeltaTx * rDeltaTx;

        if (rDeltaTx2 > errorTXcoeff2 * (txBreakLeft2 + dtx2 + rTriplet->dtx2))
          continue;

        Double_t rDeltaTy = rTriplet->ty - ty;
        Double_t rDeltaTy2 = rDeltaTy * rDeltaTy;

        if (rDeltaTy2 > errorTYcoeff2 * (tyBreakLeft2 + dty2 + rTriplet->dty2))
          continue;

        for (list<LxTriplet*>::iterator m = lPoint->triplets.begin(); m != lPoint->triplets.end(); ++m)
        {
          LxTriplet* lTriplet = *m;
          Double_t lDeltaTx = lTriplet->tx - tx;
          Double_t lDeltaTx2 = lDeltaTx * rDeltaTx;

          if (lDeltaTx2 > errorTXcoeff2 * (txBreakRight2 + dtx2 + lTriplet->dtx2))
            continue;

          Double_t lDeltaTy = lTriplet->ty - ty;
          Double_t lDeltaTy2 = lDeltaTy * rDeltaTy;

          if (lDeltaTy2 > errorTYcoeff2 * (tyBreakRight2 + dty2 + lTriplet->dty2))
            continue;

          if (0 == segment)
          {
            Double_t lDeltaX = lPoint->x - lX;
            Double_t lDeltaY = lPoint->y - lY;
            Double_t chi2 = lDeltaX * lDeltaX / (xOutDispVertex2 + lPoint->dx2 + rPoint->dx2) +
                lDeltaY * lDeltaY / (yOutDispVertex2 + lPoint->dy2 + rPoint->dy2);
            segment = new LxSegment(rPoint, lPoint, tx, ty, dtx2, dty2, chi2);
            rPoint->segments.push_back(segment);
            ++segmentCount;
          }

          segment->neighbours.push_back(lTriplet);
          rTriplet->neighbours.push_back(segment);
        }
      }
    }
  }

  //cout << "[" << stationNumber << "] LxStation::BuildSegments() segment count: " << segmentCount << endl;
}

#else//USE_SEGMENTS

#ifdef OUT_DISP_BY_TRIPLET_DIR

void LxStation::ConnectTriplets()
{
  LxStation* lStation = space->stations[stationNumber - 1];
  Double_t lZCoord = lStation->zCoord;
  KDTPointsStorageType* rPoints = static_cast<KDTPointsStorageType*> (layers[1]->points_handle);
  KDTPointsStorageType* lPoints = static_cast<KDTPointsStorageType*> (lStation->layers[1]->points_handle);

  for (KDTPointsStorageType::iterator i = rPoints->begin(); i != rPoints->end(); ++i)
  {
    KDTPointWrap& rWrap = const_cast<KDTPointWrap&> (*i);
    LxPoint* rPoint = rWrap.point;

    for (list<LxTriplet*>::iterator j = rPoint->triplets.begin(); j != rPoint->triplets.end(); ++j)
    {
      LxTriplet* rTriplet = *j;
      Double_t deltaZ = lZCoord - rPoint->z;
      Double_t lX = rPoint->x + rTriplet->tx * deltaZ;
      Double_t lY = rPoint->y + rTriplet->ty * deltaZ;
      Double_t lXLimit = errorXcoeff * sqrt(xOutDispTriplet2 + 2 * rPoint->dx2);
      Double_t lYLimit = errorYcoeff * sqrt(yOutDispTriplet2 + 2 * rPoint->dy2);
      KDTPointWrap lBoundsWrap(lX - lXLimit, lY - lYLimit);
      KDTree::_Region<2, KDTPointWrap, Double_t, KDTree::_Bracket_accessor<KDTPointWrap>,
        std::less<KDTree::_Bracket_accessor<KDTPointWrap>::result_type> > lRange(lBoundsWrap);
      lRange.set_low_bound(lBoundsWrap, 0);
      lRange.set_low_bound(lBoundsWrap, 1);
      lBoundsWrap.data[0] = lX + lXLimit;
      lBoundsWrap.data[1] = lY + lYLimit;
      lRange.set_high_bound(lBoundsWrap, 0);
      lRange.set_high_bound(lBoundsWrap, 1);
      list<KDTPointWrap> lNeighbours;
      lPoints->find_within_range(lRange, back_insert_iterator<list<KDTPointWrap> > (lNeighbours));

      for (list<KDTPointWrap>::iterator k = lNeighbours.begin(); k != lNeighbours.end(); ++k)
      {
        KDTPointWrap& lWrap = *k;
        LxPoint* lPoint = lWrap.point;

        for (list<LxTriplet*>::iterator l = lPoint->triplets.begin(); l != lPoint->triplets.end(); ++l)
        {
          LxTriplet* lTriplet = *l;
          Double_t txBreak = lTriplet->tx - rTriplet->tx;
          Double_t txBreak2 = txBreak * txBreak;

          if (txBreak2 > errorTXcoeff2 * (txInterTripletBreak2 + lTriplet->dtx2 + rTriplet->dtx2))
            continue;

          Double_t tyBreak = lTriplet->ty - rTriplet->ty;
          Double_t tyBreak2 = tyBreak * tyBreak;

          if (tyBreak2 > errorTYcoeff2 * (tyInterTripletBreak2 + lTriplet->dty2 + rTriplet->dty2))
            continue;

          Double_t lDeltaX = lPoint->x - lX;
          Double_t lDeltaX2 = lDeltaX * lDeltaX;
          Double_t lDeltaY = lPoint->y - lY;
          Double_t lDeltaY2 = lDeltaY * lDeltaY;
          Double_t chi2 = lDeltaX2 / (xOutDispTriplet2 + lPoint->dx2 + rPoint->dx2) +
              lDeltaY2 / (yOutDispTriplet2 + lPoint->dy2 + rPoint->dy2) +
              txBreak2 / (txInterTripletBreak2 + lTriplet->dtx2 + rTriplet->dtx2) +
              tyBreak2 / (tyInterTripletBreak2 + lTriplet->dty2 + rTriplet->dty2);
          rTriplet->neighbours.push_back(make_pair(lTriplet, chi2));
        }
      }
    }
  }
}

#else//OUT_DISP_BY_TRIPLET_DIR

void LxStation::ConnectTriplets()
{
  LxStation* lStation = space->stations[stationNumber - 1];
  Double_t lZCoord = lStation->zCoord;
  KDTPointsStorageType* rPoints = static_cast<KDTPointsStorageType*> (layers[1]->points_handle);
  KDTPointsStorageType* lPoints = static_cast<KDTPointsStorageType*> (lStation->layers[1]->points_handle);

  for (KDTPointsStorageType::iterator i = rPoints->begin(); i != rPoints->end(); ++i)
  {
    KDTPointWrap& rWrap = const_cast<KDTPointWrap&> (*i);
    LxPoint* rPoint = rWrap.point;

    if (rPoint->triplets.empty())
      continue;

    // Search corresponding points at the left station.
    Double_t txVertex = rPoint->x / rPoint->z;
    Double_t tyVertex = rPoint->y / rPoint->z;
    Double_t deltaZ = lZCoord - rPoint->z;
    Double_t lX = rPoint->x + txVertex * deltaZ;
    Double_t lY = rPoint->y + tyVertex * deltaZ;
    //Double_t scatCoeff2 = 1 + txVertex * txVertex + tyVertex * tyVertex;
    Double_t lXLimit = errCoeffInterTripletX * sqrt(/*scatCoeff2 * */xOutDispVertex2 + 2 * rPoint->dx2);
    Double_t lYLimit = errCoeffInterTripletY * sqrt(/*scatCoeff2 * */yOutDispVertex2 + 2 * rPoint->dy2);
    KDTPointWrap lBoundsWrap(lX - lXLimit, lY - lYLimit);
    KDTree::_Region<2, KDTPointWrap, Double_t, KDTree::_Bracket_accessor<KDTPointWrap>,
      std::less<KDTree::_Bracket_accessor<KDTPointWrap>::result_type> > lRange(lBoundsWrap);
    lRange.set_low_bound(lBoundsWrap, 0);
    lRange.set_low_bound(lBoundsWrap, 1);
    lBoundsWrap.data[0] = lX + lXLimit;
    lBoundsWrap.data[1] = lY + lYLimit;
    lRange.set_high_bound(lBoundsWrap, 0);
    lRange.set_high_bound(lBoundsWrap, 1);
    list<KDTPointWrap> lNeighbours;
    lPoints->find_within_range(lRange, back_insert_iterator<list<KDTPointWrap> > (lNeighbours));

    for (list<KDTPointWrap>::iterator k = lNeighbours.begin(); k != lNeighbours.end(); ++k)
    {
      KDTPointWrap& lWrap = *k;
      LxPoint* lPoint = lWrap.point;

      if (lPoint->triplets.empty())
        continue;

      for (list<LxTriplet*>::iterator l = rPoint->triplets.begin(); l != rPoint->triplets.end(); ++l)
      {
        LxTriplet* rTriplet = *l;

        for (list<LxTriplet*>::iterator m = lPoint->triplets.begin(); m != lPoint->triplets.end(); ++m)
        {
          LxTriplet* lTriplet = *m;
          Double_t txBreak = lTriplet->tx - rTriplet->tx;
          Double_t txBreak2 = txBreak * txBreak;

          if (txBreak2 > errCoeffInterTripletTx2 * (txInterTripletBreak2 + lTriplet->dtx2 + rTriplet->dtx2))
            continue;

          Double_t tyBreak = lTriplet->ty - rTriplet->ty;
          Double_t tyBreak2 = tyBreak * tyBreak;

          if (tyBreak2 > errCoeffInterTripletTy2 * (tyInterTripletBreak2 + lTriplet->dty2 + rTriplet->dty2))
            continue;

          Double_t lDeltaX = lPoint->x - lX;
          Double_t lDeltaX2 = lDeltaX * lDeltaX;
          Double_t lDeltaY = lPoint->y - lY;
          Double_t lDeltaY2 = lDeltaY * lDeltaY;
          Double_t chi2 = lDeltaX2 / (/*scatCoeff2 * */xOutDispVertex2 + lPoint->dx2 + rPoint->dx2) +// Comment to improve suppression ration.
              lDeltaY2 / (/*scatCoeff2 * */yOutDispVertex2 + lPoint->dy2 + rPoint->dy2) +// Comment to improve suppression ration.
              txBreak2 / (txInterTripletBreak2 + lTriplet->dtx2 + rTriplet->dtx2) +
              tyBreak2 / (tyInterTripletBreak2 + lTriplet->dty2 + rTriplet->dty2);
          rTriplet->neighbours.push_back(make_pair(lTriplet, chi2));
        }
      }
    }
  }
}

#endif//OUT_DISP_BY_TRIPLET_DIR

#endif//USE_SEGMENTS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxTrack
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LxTrack::Rebind()
{
  externalTrack = 0;

  for (list<pair<LxExtTrack*, Double_t> >::iterator i = extTrackCandidates.begin(); i != extTrackCandidates.end(); ++i)
  {
    LxExtTrack* extTrack = i->first;
    Double_t aChi2 = i->second;

    if (0 == extTrack->recoTrack.first)
    {
      extTrack->recoTrack.first = this;
      extTrack->recoTrack.second = aChi2;
      externalTrack = extTrack;
      break;
    }
    else if (aChi2 < extTrack->recoTrack.second)
    {
      LxTrack* anotherTrack = extTrack->recoTrack.first;
      extTrack->recoTrack.first = this;
      extTrack->recoTrack.second = aChi2;
      externalTrack = extTrack;
      anotherTrack->Rebind();
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxSpace
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxSpace::LxSpace() : stationsInAlgo(LXSTATIONS)
{
  for (Int_t i = 0; i < LXSTATIONS; ++i)
    stations[i] = new LxStation(this, i);
}

LxSpace::~LxSpace()
{
  Clear();

  for (Int_t i = 0; i < LXSTATIONS; ++i)
    delete stations[i];
}

void LxSpace::Clear()
{
  for (Int_t i = 0; i < LXSTATIONS; ++i)
    stations[i]->Clear();

  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
    delete *i;

  tracks.clear();
  extTracks.clear();
}

void LxSpace::BuildTriplets()
{
  for (Int_t i = 0; i < LXSTATIONS; ++i)
    stations[i]->BuildTriplets();
}

#ifdef USE_SEGMENTS

void LxSpace::BuildSegments()
{
  for (Int_t i = 1; i < LXSTATIONS; ++i)
    stations[i]->BuildSegments();
}

#else//USE_SEGMENTS

void LxSpace::ConnectTriplets()
{
  for (Int_t i = 1; i < LXSTATIONS; ++i)
    stations[i]->ConnectTriplets();
}

#endif//USE_SEGMENTS

#ifdef USE_SEGMENTS
void LxSpace::BuildCandidates(LxTriplet* begin, LxTriplet* triplet, pair<LxSegment*, LxTriplet*>* branches,
    list<LxTrackCandidate*>& candidates, Int_t level, Double_t chi2)
{
  for (list<LxSegment*>::iterator i = triplet->neighbours.begin(); i != triplet->neighbours.end(); ++i)
  {
    LxSegment* segment = *i;
    LxStation* lStation = stations[level - 1];
    LxStation* rStation = stations[level];
    Double_t dtx_1 = segment->tx - triplet->tx;
    Double_t dty_1 = segment->ty - triplet->ty;
    Double_t chi2_1 = chi2 + segment->chi2 + dtx_1 * dtx_1 / (rStation->txBreakLeft2 + triplet->dtx2 + segment->dtx2) +
        dty_1 * dty_1 / (rStation->tyBreakLeft2 + triplet->dty2 + segment->dty2);

    if (false && level < LXSTATIONS - 1)
    {
      LxSegment* rSegment = branches[level].first;
      Double_t dtx_1_s = abs(segment->tx - rSegment->tx);
      Double_t slopeCoeff2 = 1 + rSegment->tx * rSegment->tx + rSegment->ty * rSegment->ty;
      Double_t segTxSigma2 = rStation->txInterStationBreak2 * slopeCoeff2 + segment->dtx2 + rSegment->dtx2;

      if (dtx_1_s > errorTXcoeff * sqrt(segTxSigma2))
        continue;

      Double_t dty_1_s = abs(segment->ty - rSegment->ty);
      Double_t segTySigma2 = rStation->tyInterStationBreak2 * slopeCoeff2 + segment->dty2 + rSegment->dty2;

      if (dty_1_s > errorTYcoeff * sqrt(segTySigma2))
        continue;

      chi2_1 += dtx_1_s * dtx_1_s / segTxSigma2 + dty_1_s * dty_1_s / segTySigma2;
    }

    for (list<LxTriplet*>::iterator j = segment->neighbours.begin(); j != segment->neighbours.end(); ++j)
    {
      LxTriplet* triplet2 = *j;

      if (triplet2->left->used || triplet2->center->used || triplet2->right->used)
        continue;

      Double_t dtx_2 = triplet2->tx - segment->tx;
      Double_t dty_2 = triplet2->ty - segment->ty;
      Double_t chi2_2 = chi2_1 + triplet2->chi2 + dtx_2 * dtx_2 / (lStation->txBreakRight2 + segment->dtx2 + triplet2->dtx2) +
          dty_2 * dty_2 / (lStation->tyBreakRight2 + segment->dty2 + triplet2->dty2);
      branches[level - 1] = make_pair(segment, triplet2);

      if (1 == level)
      {
        LxTrackCandidate* trackCandidate = new LxTrackCandidate(begin, branches, chi2_2);
        candidates.push_back(trackCandidate);
      }
      else
        BuildCandidates(begin, triplet2, branches, candidates, level - 1, chi2_2);
    }
  }
}
#else//USE_SEGMENTS
void LxSpace::BuildCandidates(Int_t endStNum, LxTriplet* triplet, LxTriplet** branches,
    list<LxTrackCandidate*>& candidates, Int_t level, Double_t chi2)
{
  for (list<pair<LxTriplet*, Double_t> >::iterator i = triplet->neighbours.begin(); i != triplet->neighbours.end(); ++i)
  {
    pair<LxTriplet*, Double_t>& tc2 = *i;
    LxTriplet* triplet2 = tc2.first;

    if (triplet2->left->used || triplet2->center->used || triplet2->right->used)
      continue;

    branches[level - 1] = triplet2;

    if (1 == level)
    {
      LxTrackCandidate* trackCandidate = new LxTrackCandidate(endStNum, branches, chi2 + tc2.second + triplet2->chi2);
      candidates.push_back(trackCandidate);
    }
    else
      BuildCandidates(endStNum, triplet2, branches, candidates, level - 1, chi2 + tc2.second + triplet2->chi2);
  }
}
#endif//USE_SEGMENTS

void LxSpace::Reconstruct()
{
  BuildTriplets();
#ifdef USE_SEGMENTS
  BuildSegments();
#else//USE_SEGMENTS
  ConnectTriplets();
#endif//USE_SEGMENTS

  for (int endStNum = LXSTATIONS - 1; endStNum >= stationsInAlgo - 1; --endStNum)
  {
    LxStation* startStation = stations[endStNum];
    KDTPointsStorageType* points = static_cast<KDTPointsStorageType*> (startStation->layers[1]->points_handle);

    for (KDTPointsStorageType::iterator i = points->begin(); i != points->end(); ++i)
    {
      KDTPointWrap& wrap = const_cast<KDTPointWrap&> (*i);
      LxPoint* point = wrap.point;
      list<LxTrackCandidate*> candidates;
      //cout << "LxSpace::Reconstruct(): track candidate maximum size: " << candidates.max_size() << endl;

      for (list<LxTriplet*>::iterator j = point->triplets.begin(); j != point->triplets.end(); ++j)
      {
        LxTriplet* triplet = *j;

        if (triplet->left->used || triplet->center->used || triplet->right->used)
          continue;

        Double_t chi2 = triplet->chi2;
#ifdef USE_SEGMENTS
        pair<LxSegment*, LxTriplet*> branches[LXSTATIONS - 1];
        BuildCandidates(triplet, triplet, branches, candidates, LXSTATIONS - 1, chi2);
#else//USE_SEGMENTS
        LxTriplet* branches[endStNum + 1];
        branches[endStNum] = triplet;
        BuildCandidates(endStNum, triplet, branches, candidates, endStNum, chi2);
#endif//USE_SEGMENTS
      }

      cout << "LxSpace::Reconstruct(): track candidate number: " << candidates.size() << endl;

      LxTrackCandidate* bestCandidate = 0;

      for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
      {
        LxTrackCandidate* candidate = *j;

        if (0 == bestCandidate || candidate->chi2 < bestCandidate->chi2)
          bestCandidate = candidate;
      }

      if (0 != bestCandidate)
      {
        LxTrack* track = new LxTrack(bestCandidate);
        tracks.push_back(track);
      }

      for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
        delete *j;
    }// for (KDTPointsStorageType::iterator i = points->begin(); i != points->end(); ++i)
  }// for (int endStNum = LXSTATIONS - 1; endStNum >= stationsInAlgo - 1; --endStNum)

  RemoveClones();
}

void LxSpace::RemoveClones()
{
  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
  {
    LxTrack* firstTrack = *i;
    list<LxTrack*>::iterator i2 = i;
    ++i2;

    for (; i2 != tracks.end(); ++i2)
    {
      LxTrack* secondTrack = *i2;
      Int_t neighbourPoints = 0;
      Int_t minLen = firstTrack->length < secondTrack->length ? firstTrack->length : secondTrack->length;
      Int_t minPoints = minLen * LXLAYERS;

      for (Int_t j = 0; j < LXSTATIONS * LXLAYERS; ++j)
      {
        if (j >= minPoints)
          continue;

        LxPoint* point1 = firstTrack->points[j];
        LxPoint* point2 = secondTrack->points[j];

        Double_t dx = point1->dx > point2->dx ? point1->dx : point2->dx;
        Double_t dy = point1->dy > point2->dy ? point1->dy : point2->dy;

        if (abs(point2->x - point1->x) < 5.0 * dx && abs(point2->y - point1->y) < 5.0 * dy)
          ++neighbourPoints;
      }

      if (neighbourPoints < minPoints / 2)
        continue;

      if (firstTrack->length > secondTrack->length)
        secondTrack->clone = true;
      else if (secondTrack->length > firstTrack->length)
        firstTrack->clone = true;
      else if (firstTrack->chi2 < secondTrack->chi2)
        secondTrack->clone = true;
      else
        firstTrack->clone = true;
    }// for (; i2 != tracks.end(); ++i2)
  }// for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
}

void LxSpace::JoinExtTracks()
{
  Double_t cutCoeff = 5.0;

  TrackPropagatorPtr fPropagator = CbmLitToolFactory::CreateTrackPropagator("lit");

  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
  {
    LxTrack* track = *i;

    if (track->clone)
      continue;

    LxTriplet* leftTriplet = track->branches[0];
    LxPoint* muchPoint = leftTriplet->center;

    for (list<LxExtTrack>::iterator j = extTracks.begin(); j != extTracks.end(); ++j)
    {
      LxExtTrack* extTrack = &(*j);
      const FairTrackParam* lastL1Param = extTrack->track->GetParamLast();
      CbmLitTrackParam lastParam;
      CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(lastL1Param, &lastParam);

      if (kLITERROR == fPropagator->Propagate(&lastParam, stations[0]->zCoord, 13))
        continue;

      Double_t deltaX = abs(lastParam.GetX() - muchPoint->x);
      Double_t deltaY = abs(lastParam.GetY() - muchPoint->y);
      Double_t deltaTx = abs(lastParam.GetTx() - leftTriplet->tx);
      Double_t deltaTy = abs(lastParam.GetTy() - leftTriplet->ty);
      Double_t sigmaX2 = muchPoint->dx2 + lastParam.GetCovariance(0);
      Double_t sigmaX = sqrt(sigmaX2);
      Double_t sigmaY2 = muchPoint->dy2 + lastParam.GetCovariance(6);
      Double_t sigmaY = sqrt(sigmaY2);
      Double_t sigmaTx2 = leftTriplet->dtx2 + lastParam.GetCovariance(11);
      Double_t sigmaTx = sqrt(sigmaTx2);
      Double_t sigmaTy2 = leftTriplet->dty2 + lastParam.GetCovariance(15);
      Double_t sigmaTy = sqrt(sigmaTy2);

      if (deltaX > cutCoeff * sigmaX || deltaY > cutCoeff * sigmaY ||
          deltaTx > cutCoeff * sigmaTx || deltaTy > cutCoeff * sigmaTy)
      {
        continue;
      }

      Double_t chi2 = deltaX * deltaX / sigmaX2 + deltaY * deltaY / sigmaY2 +
          deltaTx * deltaTx / sigmaTx2 + deltaTy * deltaTy / sigmaTy2;

      list<pair<LxExtTrack*, Double_t> >::iterator k = track->extTrackCandidates.begin();

      for (; k != track->extTrackCandidates.end() && chi2 >= k->second; ++k)
        ;

      pair<LxExtTrack*, Double_t> linkDesc(extTrack, chi2);
        track->extTrackCandidates.insert(k, linkDesc);
    }// for (list<LxExtTrack>::iterator j = extTracks.begin(); j != extTracks.end(); ++j)

    for (list<pair<LxExtTrack*, Double_t> >::iterator j = track->extTrackCandidates.begin();
        j != track->extTrackCandidates.end(); ++j)
    {
      LxExtTrack* extTrack = j->first;
      Double_t chi2 = j->second;

      if (0 == extTrack->recoTrack.first)
      {
        extTrack->recoTrack.first = track;
        extTrack->recoTrack.second = chi2;
        track->externalTrack = extTrack;
        break;
      }
      else if (chi2 < extTrack->recoTrack.second)
      {
        LxTrack* anotherTrack = extTrack->recoTrack.first;
        extTrack->recoTrack.first = track;
        extTrack->recoTrack.second = chi2;
        track->externalTrack = extTrack;
        anotherTrack->Rebind();
        break;
      }
    }
  }
}

void LxSpace::FitTracks()
{
}

LxPoint* LxSpace::AddPoint(Int_t stationNumber, Int_t layerNumber, Int_t hitId, Double_t x, Double_t y, Double_t z, Double_t dx, Double_t dy, Double_t dz)
{
  LxLayer* layer = stations[stationNumber]->layers[layerNumber];
  LxPoint* point = new LxPoint(x, y, z, dx, dy, dz, layer, hitId);
  KDTPointsStorageType* points = static_cast<KDTPointsStorageType*> (layer->points_handle);
  KDTPointWrap pointWrap(point);
  points->insert(pointWrap);
  return point;
}
