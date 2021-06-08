/* Copyright (C) 2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LXTBBINNED2_H
#define LXTBBINNED2_H

#include "CbmPixelHit.h"

#include <list>

#include "LxTBDefinitions.h"


#define NOF_LAYERS 3
#define LAST_LAYER NOF_LAYERS - 1
#define NOF_STATIONS 4
#define LAST_STATION NOF_STATIONS - 1

#define TIMEBIN_LENGTH 100
#define NOF_SIGMAS 4
#define NOF_SIGMAS_SQ NOF_SIGMAS* NOF_SIGMAS

struct LxTbBinnedTriplet;

struct LxTbBinnedPoint {
  scaltype x;
  scaltype dx;
  scaltype y;
  scaltype dy;
  timetype t;
  timetype dt;
  bool use;
  std::list<LxTbBinnedTriplet*> triplets;
  Int_t refId;

#ifdef LXTB_QA
  const CbmPixelHit* pHit;
  bool isTrd;
  Int_t stationNumber;
  Int_t layerNumber;

  struct PointDesc {
    Int_t eventId;
    Int_t pointId;
    Int_t trackId;
  };

  std::list<PointDesc> mcRefs;
#endif  //LXTB_QA

  LxTbBinnedPoint(scaltype X, scaltype Dx, scaltype Y, scaltype Dy, timetype T, timetype Dt, Int_t ri, bool Use)
    : x(X)
    , dx(Dx)
    , y(Y)
    , dy(Dy)
    , t(T)
    , dt(Dt)
    , refId(ri)
    , use(Use)
#ifdef LXTB_QA
    , pHit(0)
    , isTrd(false)
    , stationNumber(-1)
    , layerNumber(-1)
#endif  //LXTB_QA
  {
  }

  LxTbBinnedPoint(const LxTbBinnedPoint& original)
    : x(original.x)
    , dx(original.dx)
    , y(original.y)
    , dy(original.dy)
    , t(original.t)
    , dt(original.dt)
    , use(original.use)
    , triplets()
    , refId(original.refId)
#ifdef LXTB_QA
    , pHit(original.pHit)
    , isTrd(original.isTrd)
    , stationNumber(original.stationNumber)
    , layerNumber(original.layerNumber)
    , mcRefs(original.mcRefs)
#endif  //LXTB_QA
  {
  }

  ~LxTbBinnedPoint();
};

struct LxTbBinnedTriplet {
  LxTbBinnedPoint* lPoint;
  LxTbBinnedPoint* rPoint;
  scaltype tx;
  scaltype dtx;
  scaltype ty;
  scaltype dty;
  std::list<LxTbBinnedPoint*> neighbours;

  LxTbBinnedTriplet(LxTbBinnedPoint* lp, LxTbBinnedPoint* rp, scaltype deltaZ)
    : lPoint(lp)
    , rPoint(rp)
    , tx((lPoint->x - rPoint->x) / deltaZ)
    , ty((lPoint->y - rPoint->y) / deltaZ)
    , dtx(-sqrt(lPoint->dx * lPoint->dx + rPoint->dx * rPoint->dx) / deltaZ)
    , dty(-sqrt(lPoint->dy * lPoint->dy + rPoint->dy * rPoint->dy) / deltaZ)
  {
  }
};

inline LxTbBinnedPoint::~LxTbBinnedPoint()
{
  for (std::list<LxTbBinnedTriplet*>::iterator i = triplets.begin(); i != triplets.end(); ++i)
    delete *i;
}

struct LxTbXBin {
  LxTbXBin() : use(false), maxDx(0), maxDy(0), maxDt(0) {}

  void Clear()
  {
    points.clear();
    maxDx = 0;
    maxDy = 0;
    maxDt = 0;
    use   = false;
  }

  void AddPoint(const LxTbBinnedPoint& point)
  {
    points.push_back(point);

    if (point.dx > maxDx) maxDx = point.dx;

    if (point.dy > maxDy) maxDy = point.dy;

    if (point.dt > maxDt) maxDt = point.dt;
  }

  std::list<LxTbBinnedPoint> points;
  bool use;
  scaltype maxDx;
  scaltype maxDy;
  timetype maxDt;
};

struct LxTbYXBin {
  LxTbYXBin(int nxbs) : xBins(new LxTbXBin[nxbs]), nofXBins(nxbs), use(false) {}

  ~LxTbYXBin() { delete[] xBins; }

  void Clear()
  {
    for (int i = 0; i < nofXBins; ++i)
      xBins[i].Clear();

    use = false;
  }

  LxTbXBin* xBins;
  int nofXBins;
  bool use;
};

struct LxTbTYXBin {
  LxTbTYXBin(int nxbs, int nybs)
    : yxBins(reinterpret_cast<LxTbYXBin*>(new unsigned char[nybs * sizeof(LxTbYXBin)]))
    , nofYXBins(nybs)
    , use(false)
  {
    for (int i = 0; i < nybs; ++i)
      new (&yxBins[i]) LxTbYXBin(nxbs);
  }

  ~LxTbTYXBin() { delete[] reinterpret_cast<unsigned char*>(yxBins); }

  void Clear()
  {
    for (int i = 0; i < nofYXBins; ++i)
      yxBins[i].Clear();

    use = false;
  }

  LxTbYXBin* yxBins;
  int nofYXBins;
  bool use;
};

struct LxTbLayer {
  LxTbLayer(int nofxb, int nofyb, int noftb)
    : tyxBins(reinterpret_cast<LxTbTYXBin*>(new unsigned char[noftb * sizeof(LxTbTYXBin)]))
    , nofTYXBins(noftb)
    , nofYXBins(nofyb)
    , nofXBins(nofxb)
    , lastTimeBinNumber(noftb - 1)
    , lastYBinNumber(nofyb - 1)
    , lastXBinNumber(nofxb - 1)
    , minX(0)
    , maxX(0)
    , minY(0)
    , maxY(0)
    , minT(0)
    , maxT(0)
    , xBinLength(0)
    , yBinLength(0)
    , timeBinLength(TIMEBIN_LENGTH)
    , z(0)
  {
    for (int i = 0; i < noftb; ++i)
      new (&tyxBins[i]) LxTbTYXBin(nofyb, nofxb);
  }

  ~LxTbLayer() { delete[] reinterpret_cast<unsigned char*>(tyxBins); }

  void Clear()
  {
    for (int i = 0; i < nofTYXBins; ++i)
      tyxBins[i].Clear();
  }

  void Init()
  {
    xBinLength = (maxX - minX) / nofXBins;
    yBinLength = (maxY - minY) / nofYXBins;
    maxT       = minT + TIMEBIN_LENGTH * nofTYXBins;
  }

  void SetMinT(timetype v)
  {
    minT = v;
    maxT = minT + TIMEBIN_LENGTH * nofTYXBins;
  }

  LxTbTYXBin* tyxBins;
  int nofTYXBins;
  int nofYXBins;
  int nofXBins;
  int lastTimeBinNumber;
  int lastYBinNumber;
  int lastXBinNumber;
  scaltype minX;
  scaltype maxX;
  scaltype minY;
  scaltype maxY;
  timetype minT;
  timetype maxT;
  scaltype xBinLength;
  scaltype yBinLength;
  timetype timeBinLength;
  scaltype z;
};


#define LXTB_ITERATE_LAYER_BEGIN                                                                                       \
  if (maxX > layer.minX && minX < layer.maxX && maxY > layer.minY && minY < layer.maxY && maxT > layer.minT            \
      && minT < layer.maxT) {                                                                                          \
    int minTind = (minT - layer.minT) / layer.timeBinLength;                                                           \
                                                                                                                       \
    if (minTind < 0) minTind = 0;                                                                                      \
    else if (minTind > layer.lastTimeBinNumber)                                                                        \
      minTind = layer.lastTimeBinNumber;                                                                               \
                                                                                                                       \
    int maxTind = (maxT - layer.minT) / layer.timeBinLength;                                                           \
                                                                                                                       \
    if (maxTind < 0) maxTind = 0;                                                                                      \
    else if (maxTind > layer.lastTimeBinNumber)                                                                        \
      maxTind = layer.lastTimeBinNumber;                                                                               \
                                                                                                                       \
    int minYind = (minY - layer.minY) / layer.yBinLength;                                                              \
                                                                                                                       \
    if (minYind < 0) minYind = 0;                                                                                      \
    else if (minYind > layer.lastYBinNumber)                                                                           \
      minYind = layer.lastYBinNumber;                                                                                  \
                                                                                                                       \
    int maxYind = (maxY - layer.minY) / layer.yBinLength;                                                              \
                                                                                                                       \
    if (maxYind < 0) maxYind = 0;                                                                                      \
    else if (maxYind > layer.lastYBinNumber)                                                                           \
      maxYind = layer.lastYBinNumber;                                                                                  \
                                                                                                                       \
    int minXind = (minX - layer.minX) / layer.xBinLength;                                                              \
                                                                                                                       \
    if (minXind < 0) minXind = 0;                                                                                      \
    else if (minXind > layer.lastXBinNumber)                                                                           \
      minXind = layer.lastXBinNumber;                                                                                  \
                                                                                                                       \
    int maxXind = (maxX - minX) / layer.xBinLength;                                                                    \
                                                                                                                       \
    if (maxXind < 0) maxXind = 0;                                                                                      \
    else if (maxXind > layer.lastXBinNumber)                                                                           \
      maxXind = layer.lastXBinNumber;                                                                                  \
                                                                                                                       \
    for (int tInd = minTind; tInd <= maxTind; ++tInd) {                                                                \
      LxTbTYXBin& tyxBin = layer.tyxBins[tInd];                                                                        \
                                                                                                                       \
      for (int yInd = minYind; yInd <= maxYind; ++yInd) {                                                              \
        LxTbYXBin& yxBin = tyxBin.yxBins[yInd];                                                                        \
                                                                                                                       \
        for (int xInd = minXind; xInd <= maxXind; ++xInd) {                                                            \
          LxTbXBin& xBin = yxBin.xBins[xInd];                                                                          \
                                                                                                                       \
          for (std::list<LxTbBinnedPoint>::iterator pointIterator = xBin.points.begin();                               \
               pointIterator != xBin.points.end(); ++pointIterator) {                                                  \
            LxTbBinnedPoint& point = *pointIterator;                                                                   \
                                                                                                                       \
            if (point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY && point.t >= minT            \
                && point.t <= maxT) {

#define LXTB_ITERATE_LAYER_END                                                                                         \
  point.use = true;                                                                                                    \
  xBin.use  = true;                                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
                                                                                                                       \
  if (xBin.use) yxBin.use = true;                                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  if (yxBin.use) tyxBin.use = true;                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
  }

#define LXTB_ITERATE_LAYER_END_NO_USE                                                                                  \
  }                                                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
  }                                                                                                                    \
  }


template<class HandlePoint>
void IterateLayer(LxTbLayer& layer, HandlePoint& handlePoint)
{
  for (int i = 0; i < layer.nofTYXBins; ++i) {
    LxTbTYXBin& tyxBin = layer.tyxBins[i];

    if (!tyxBin.use) continue;

    for (int j = 0; j < layer.nofYXBins; ++j) {
      LxTbYXBin& yxBin = tyxBin.yxBins[j];

      if (!yxBin.use) continue;

      for (int k = 0; k < layer.nofXBins; ++k) {
        LxTbXBin& xBin = yxBin.xBins[k];

        if (!xBin.use) continue;

        for (std::list<LxTbBinnedPoint>::iterator l = xBin.points.begin(); l != xBin.points.end(); ++l) {
          LxTbBinnedPoint& point = *l;

          if (!point.use) continue;

          handlePoint(point);
        }  // for (list<LxTbBinnedPoint>::iterator l = xBin.points.begin(); l != xBin.points.end(); ++l)
      }    // for (int k = 0; k < layer.nofXBins; ++k)
    }      // for (int j = 0; j < layer.nofYXBins; ++j)
  }        // for (int i = 0; i < lastLayer.nofTYXBins; ++i)
}

template<class HandlePoint>
void IterateNeighbourhood(LxTbLayer& layer, scaltype x, scaltype dx, scaltype scatX, scaltype y, scaltype dy,
                          scaltype scatY, timetype t, timetype dt, HandlePoint& handlePoint)
{
  scaltype dxSq    = dx * dx;
  scaltype scatXSq = scatX * scatX;
  scaltype varX0   = scatXSq + dxSq;
  scaltype wX      = NOF_SIGMAS * sqrt(varX0 + dxSq);
  scaltype minX    = x - wX;
  scaltype maxX    = x + wX;
  scaltype dySq    = dy * dy;
  scaltype scatYSq = scatY * scatY;
  scaltype varY0   = scatYSq + dySq;
  scaltype wY      = NOF_SIGMAS * sqrt(varY0 + dySq);
  scaltype minY    = y - wY;
  scaltype maxY    = y + wY;
  timetype varT0   = dt * dt;
  timetype wT      = NOF_SIGMAS * sqrt(2.0) * dt;
  timetype minT    = t - wT;
  timetype maxT    = t + wT;

  if (maxX < layer.minX || minX > layer.maxX || maxY < layer.minY || minY > layer.maxY || maxT < layer.minT
      || minT > layer.maxT)
    return;

  int minTind = (minT - layer.minT) / layer.timeBinLength;

  if (minTind < 0) minTind = 0;
  else if (minTind > layer.lastTimeBinNumber)
    minTind = layer.lastTimeBinNumber;

  int maxTind = (maxT - layer.minT) / layer.timeBinLength;

  if (maxTind < 0) maxTind = 0;
  else if (maxTind > layer.lastTimeBinNumber)
    maxTind = layer.lastTimeBinNumber;

  int minYind = (minY - layer.minY) / layer.yBinLength;

  if (minYind < 0) minYind = 0;
  else if (minYind > layer.lastYBinNumber)
    minYind = layer.lastYBinNumber;

  int maxYind = (maxY - layer.minY) / layer.yBinLength;

  if (maxYind < 0) maxYind = 0;
  else if (maxYind > layer.lastYBinNumber)
    maxYind = layer.lastYBinNumber;

  int minXind = (minX - layer.minX) / layer.xBinLength;

  if (minXind < 0) minXind = 0;
  else if (minXind > layer.lastXBinNumber)
    minXind = layer.lastXBinNumber;

  int maxXind = (maxX - layer.minX) / layer.xBinLength;

  if (maxXind < 0) maxXind = 0;
  else if (maxXind > layer.lastXBinNumber)
    maxXind = layer.lastXBinNumber;

  for (int tInd = minTind; tInd <= maxTind; ++tInd) {
    LxTbTYXBin& tyxBin = layer.tyxBins[tInd];

    for (int yInd = minYind; yInd <= maxYind; ++yInd) {
      LxTbYXBin& yxBin = tyxBin.yxBins[yInd];

      for (int xInd = minXind; xInd <= maxXind; ++xInd) {
        LxTbXBin& xBin = yxBin.xBins[xInd];

        for (std::list<LxTbBinnedPoint>::iterator pointIterator = xBin.points.begin();
             pointIterator != xBin.points.end(); ++pointIterator) {
          LxTbBinnedPoint& point = *pointIterator;
          scaltype errWinXSq     = NOF_SIGMAS_SQ * (varX0 + point.dx * point.dx);
          scaltype errWinYSq     = NOF_SIGMAS_SQ * (varY0 + point.dy * point.dy);
          scaltype errWinTSq     = NOF_SIGMAS_SQ * (varT0 + point.dt * point.dt);
          scaltype deltaX        = point.x - x;
          scaltype deltaXSq      = deltaX * deltaX;
          scaltype deltaY        = point.y - y;
          scaltype deltaYSq      = deltaY * deltaY;
          scaltype deltaT        = point.t - t;
          scaltype deltaTSq      = deltaT * deltaT;

          if (deltaXSq > errWinXSq || deltaYSq > errWinYSq || deltaTSq > errWinTSq) continue;

          point.use = true;
          xBin.use  = true;
          handlePoint(point);
        }

        if (xBin.use) yxBin.use = true;
      }

      if (yxBin.use) tyxBin.use = true;
    }
  }
}

template<class HandlePoint>
void IterateNeighbourhoodConst(LxTbLayer& layer, HandlePoint& handlePoint)
{
}

#endif /* LXTBBINNED2_H */
