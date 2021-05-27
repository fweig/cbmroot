/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LxTBBinned.h
 * Author: Timur Ablyazimov
 *
 * Created on 31 Mar 2016
 */

#ifndef LXTBBINNED_H
#define LXTBBINNED_H

#include "LxTBDefinitions.h"
#include "LxTBMatEffs.h"


#ifdef __MACH__
#include <mach/mach_time.h>
#include <sys/time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
inline int clock_gettime(int /*clk_id*/, struct timespec* t)
{
  mach_timebase_info_data_t timebase;
  mach_timebase_info(&timebase);
  uint64_t time;
  time            = mach_absolute_time();
  double nseconds = ((double) time * (double) timebase.numer) / ((double) timebase.denom);
  double seconds  = ((double) time * (double) timebase.numer) / ((double) timebase.denom * 1e9);
  t->tv_sec       = seconds;
  t->tv_nsec      = nseconds;
  return 0;
}
#else
#include <time.h>
#endif

#include "CbmPixelHit.h"

#include <list>
#include <memory>
#include <set>

#define NOF_SIGMAS 1
#define NOF_SIGMAS_SQ NOF_SIGMAS* NOF_SIGMAS
#define TIME_ERROR 4

extern Double_t speedOfLight;

struct LxTbBinnedPoint;

struct LxTbBinnedRay {
  const LxTbBinnedPoint* lPoint;
  scaltype tx;
  scaltype dtxSq;
  scaltype ty;
  scaltype dtySq;
  scaltype chi2;

  LxTbBinnedRay(scaltype deltaZ, const LxTbBinnedPoint& rP, const LxTbBinnedPoint& lP,
                scaltype Chi2); /* : lPoint(&lP), tx((lP.x - rP.x) / deltaZ),
        dtxSq((lP.dx - rP.dx) * (lP.dx - rP.dx) / (deltaZ * deltaZ)), ty((lP.y - rP.y) / deltaZ),
        dtySq((lP.dy - rP.dy) * (lP.dy - rP.dy) / (deltaZ * deltaZ)), chi2(Chi2)
    {
    }*/
};

struct LxTbBinnedPoint {
  scaltype x;
  scaltype dx;
  scaltype y;
  scaltype dy;
  timetype t;
  timetype dt;
  bool use;
  std::list<LxTbBinnedRay> neighbours;
  Int_t refId;

#ifdef LXTB_QA
  const CbmPixelHit* pHit;
  bool isTrd;
  Int_t stationNumber;

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
    , use(Use)
    , neighbours()
    , refId(ri)
#ifdef LXTB_QA
    , pHit(0)
    , isTrd(false)
    , stationNumber(-1)
    , mcRefs()
#endif  //LXTB_QA
  {
  }

  LxTbBinnedPoint(const LxTbBinnedPoint& orig) = default;
  LxTbBinnedPoint& operator=(const LxTbBinnedPoint& orig) = delete;
};

inline LxTbBinnedRay::LxTbBinnedRay(scaltype deltaZ, const LxTbBinnedPoint& rP, const LxTbBinnedPoint& lP,
                                    scaltype Chi2)
  : lPoint(&lP)
  , tx((lP.x - rP.x) / deltaZ)
  , dtxSq((lP.dx - rP.dx) * (lP.dx - rP.dx) / (deltaZ * deltaZ))
  , ty((lP.y - rP.y) / deltaZ)
  , dtySq((lP.dy - rP.dy) * (lP.dy - rP.dy) / (deltaZ * deltaZ))
  , chi2(Chi2)
{
}

struct LxTbXBin {
  std::list<LxTbBinnedPoint> points;
  bool use;

  LxTbXBin() : points(), use(false), maxDx() {}

  void Clear()
  {
    points.clear();
    use = false;
  }

  // New feature
  scaltype maxDx;

  void AddPoint(const LxTbBinnedPoint& point) { points.push_back(point); }
};

struct LxTbYXBin {
  LxTbXBin* xBins;
  int nofXBins;
  bool use;

  LxTbYXBin(int nxbs) : xBins(new LxTbXBin[nxbs]), nofXBins(nxbs), use(false) {}
  LxTbYXBin(const LxTbYXBin&) = delete;
  LxTbYXBin& operator=(const LxTbYXBin&) = delete;

  ~LxTbYXBin() { delete[] xBins; }

  void Clear()
  {
    for (int i = 0; i < nofXBins; ++i)
      xBins[i].Clear();

    use = false;
  }
};

struct LxTbTYXBin {
  LxTbYXBin* yxBins;
  int nofYXBins;
  bool use;

  LxTbTYXBin(int nxbs, int nybs)
    : yxBins(reinterpret_cast<LxTbYXBin*>(new unsigned char[nybs * sizeof(LxTbYXBin)]))
    , nofYXBins(nybs)
    , use(false)
  {
    for (int i = 0; i < nybs; ++i)
      new (&yxBins[i]) LxTbYXBin(nxbs);
  }
  LxTbTYXBin(const LxTbTYXBin&) = delete;
  LxTbTYXBin& operator=(const LxTbTYXBin&) = delete;

  ~LxTbTYXBin() { delete[] reinterpret_cast<unsigned char*>(yxBins); }

  void Clear()
  {
    for (int i = 0; i < nofYXBins; ++i)
      yxBins[i].Clear();

    use = false;
  }
};

struct LxTbBinnedStation {
  struct Q {
    scaltype Q11, Q12, Q21, Q22;
  };

  int stationNumber;
  scaltype z;
  int nofXBins;
  int nofYBins;
  int nofTYXBins;
  int lastXBin;
  int lastYBin;
  scaltype minX;
  scaltype maxX;
  scaltype binSizeX;
  scaltype minY;
  scaltype maxY;
  scaltype binSizeY;
  LxTbAbsorber absorber;
  scaltype deltaThetaX;
  scaltype deltaThetaY;
  scaltype dispX;
  scaltype dispY;
  LxTbTYXBin* tyxBins;
  Q qs[2];

  LxTbBinnedStation(int nofxb, int nofyb, int noftb)
    : stationNumber(-1)
    , z()
    , nofXBins(nofxb)
    , nofYBins(nofyb)
    , nofTYXBins(noftb)
    , lastXBin(nofxb - 1)
    , lastYBin(nofyb - 1)
    , minX()
    , maxX()
    , binSizeX()
    , minY()
    , maxY()
    , binSizeY()
    , absorber()
    , deltaThetaX()
    , deltaThetaY()
    , dispX()
    , dispY()
    , tyxBins(reinterpret_cast<LxTbTYXBin*>(new unsigned char[noftb * sizeof(LxTbTYXBin)]))
    , qs()
  {
    for (int i = 0; i < noftb; ++i)
      new (&tyxBins[i]) LxTbTYXBin(nofXBins, nofYBins);
  }
  LxTbBinnedStation(const LxTbBinnedStation&) = delete;
  LxTbBinnedStation& operator=(const LxTbBinnedStation&) = delete;

  ~LxTbBinnedStation() { delete[] reinterpret_cast<unsigned char*>(tyxBins); }

  void Clear()
  {
    for (int i = 0; i < nofTYXBins; ++i)
      tyxBins[i].Clear();
  }
};

struct LxTbBinnedTrdStation {
  scaltype Zs[4];
  int nofXBins;
  int nofYBins;
  int nofTYXBins;
  int lastXBin;
  int lastYBin;
  scaltype minX;
  scaltype maxX;
  scaltype binSizeX;
  scaltype minY;
  scaltype maxY;
  scaltype binSizeY;
  LxTbAbsorber absorber;
  scaltype deltaThetaX;
  scaltype deltaThetaY;
  scaltype dispXs[4];
  scaltype dispYs[4];
  LxTbTYXBin* tyxBinsArr[4];

  LxTbBinnedTrdStation(int nofxb, int nofyb, int noftb)
    : Zs()
    , nofXBins(nofxb)
    , nofYBins(nofyb)
    , nofTYXBins(noftb)
    , lastXBin(nofxb - 1)
    , lastYBin(nofyb - 1)
    , minX()
    , maxX()
    , binSizeX()
    , minY()
    , maxY()
    , binSizeY()
    , absorber()
    , deltaThetaX()
    , deltaThetaY()
    , dispXs()
    , dispYs()
    , tyxBinsArr()

  {
    for (int i = 0; i < 4; ++i) {
      tyxBinsArr[i] = reinterpret_cast<LxTbTYXBin*>(new unsigned char[noftb * sizeof(LxTbTYXBin)]);

      for (int j = 0; j < noftb; ++j) {
        new (&tyxBinsArr[i][j]) LxTbTYXBin(nofXBins, nofYBins);
      }
    }
  }
  LxTbBinnedTrdStation(const LxTbBinnedTrdStation&) = delete;
  LxTbBinnedTrdStation& operator=(const LxTbBinnedTrdStation&) = delete;

  ~LxTbBinnedTrdStation()
  {
    for (int i = 0; i < 4; ++i)
      delete[] reinterpret_cast<unsigned char*>(tyxBinsArr[i]);
  }

  void Clear()
  {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < nofTYXBins; ++j)
        tyxBinsArr[i][j].Clear();
    }
  }
};

static long fullDuration = 0;

struct LxTbBinnedFinder {

  struct KFParamsCoord {
    scaltype coord, tg, C11, C12, C21, C22;
  };

  struct KFParams {
    KFParamsCoord xParams;
    KFParamsCoord yParams;
    scaltype chi2;
  };

  void KFAddPointCoord(KFParamsCoord& param, const KFParamsCoord& prevParam, scaltype m, scaltype V, scaltype& chi2,
                       int stationNumber, int coordNumber)
  {
    const LxTbBinnedStation& station = stations[stationNumber];
    const LxTbBinnedStation::Q& Q    = station.qs[coordNumber];
    scaltype deltaZ                  = station.z - stations[stationNumber + 1].z;
    scaltype deltaZSq                = deltaZ * deltaZ;

    // Extrapolate.
    param.coord += prevParam.tg * deltaZ;  // params[k].tg is unchanged.

    // Filter.
    param.C11 += prevParam.C12 * deltaZ + prevParam.C21 * deltaZ + prevParam.C22 * deltaZSq + Q.Q11;
    param.C12 += prevParam.C22 * deltaZ + Q.Q12;
    param.C21 += prevParam.C22 * deltaZ + Q.Q21;
    param.C22 += Q.Q22;

    scaltype S      = 1.0 / (V + param.C11);
    scaltype Kcoord = param.C11 * S;
    scaltype Ktg    = param.C21 * S;
    scaltype dzeta  = m - param.coord;
    param.coord += Kcoord * dzeta;
    param.tg += Ktg * dzeta;
    param.C21 -= param.C11 * Ktg;
    param.C22 -= param.C12 * Ktg;
    param.C11 *= 1.0 - Kcoord;
    param.C12 *= 1.0 - Kcoord;
    chi2 += dzeta * S * dzeta;
  }

  void KFAddPoint(KFParams& param, const KFParams& prevParam, scaltype m[2], scaltype V[2], int stationNumber)
  {
    /*LxRay* ray = rays[i];
        LxPoint* point = ray->end;
        LxStation* station = point->layer->station;
        scaltype m[2] = { point->x, point->y };
        scaltype V[2] = { point->dx * point->dx, point->dy * point->dy };
        KFParams pPrev[2] = { params[0], params[1] };
        scaltype deltaZ = point->z - prevPoint->z;
        scaltype deltaZ2 = deltaZ * deltaZ;*/

    KFAddPointCoord(param.xParams, prevParam.xParams, m[0], V[0], param.chi2, stationNumber, 0);
    KFAddPointCoord(param.yParams, prevParam.yParams, m[1], V[1], param.chi2, stationNumber, 1);
  }

  struct Chain {
    LxTbBinnedPoint** points;
    int nofPoints;
    scaltype chi2;
    bool highMom;

    Chain(const LxTbBinnedPoint* const* pts, int nofPts, scaltype Chi2)
      : points(new LxTbBinnedPoint*[nofPts])
      , nofPoints(nofPts)
      , chi2(Chi2)
      , highMom(false)
    {
      for (int i = 0; i < nofPoints; ++i)
        points[i] = new LxTbBinnedPoint(*pts[i]);
    }
    Chain(const Chain&) = delete;
    Chain& operator=(const Chain&) = delete;

    ~Chain()
    {
      for (int i = 0; i < nofPoints; ++i)
        delete points[i];

      delete[] points;
    }
  };

  struct ChainImpl {
    //const LxTbBinnedPoint* points[];// Change to dynamical allocation
    const LxTbBinnedPoint** points;
    scaltype chi2;

    ChainImpl(const LxTbBinnedPoint** pts, int nofPts, scaltype Chi2)
      : points(new const LxTbBinnedPoint*[nofPts])
      , chi2(Chi2)
    {
      memcpy(points, pts, nofPts * sizeof(const LxTbBinnedPoint*));
    }

    void Destroy() { delete[] points; }
  };

  struct TriggerTimeArray {
    TriggerTimeArray(int noftb, int tbl, timetype& mt)
      : nofTimebins(noftb)
      , tbLength(tbl)
      , minT(mt)
      , triggerTimeBins(new std::list<std::pair<timetype, timetype>>[noftb])
    {
    }
    TriggerTimeArray(const TriggerTimeArray&) = delete;
    TriggerTimeArray& operator=(const TriggerTimeArray&) = delete;

    ~TriggerTimeArray() { delete[] triggerTimeBins; }

    void Clear()
    {
      for (int i = 0; i < nofTimebins; ++i)
        triggerTimeBins[i].clear();
    }

    void Insert(const std::pair<timetype, timetype>& v)
    {
      timetype wT = NOF_SIGMAS * sqrt(2.0) * v.second;
      int minInd  = (v.first - wT - minT) / tbLength;
      int maxInd  = (v.first + wT - minT) / tbLength;

      if (minInd < 0) minInd = 0;
      else if (minInd >= nofTimebins)
        minInd = nofTimebins - 1;

      if (maxInd < 0) maxInd = 0;
      else if (maxInd >= nofTimebins)
        maxInd = nofTimebins - 1;

      bool busy = false;

      for (int i = minInd; i <= maxInd; ++i) {
        for (std::list<std::pair<timetype, timetype>>::const_iterator j = triggerTimeBins[i].begin();
             j != triggerTimeBins[i].end(); ++j) {
          const std::pair<timetype, timetype>& p = *j;

          if (fabs(v.first - p.first) <= NOF_SIGMAS * sqrt(v.second * v.second + p.second * p.second)) {
            busy = true;
            break;
          }
        }

        if (busy) break;
      }

      if (!busy) {
        int ind = (v.first - minT) / tbLength;

        if (ind < 0) ind = 0;
        else if (ind >= nofTimebins)
          ind = nofTimebins - 1;

        triggerTimeBins[ind].push_back(v);
      }
    }

    int nofTimebins;
    int tbLength;
    timetype& minT;
    std::list<std::pair<timetype, timetype>>* triggerTimeBins;
  };

  struct SignalParticle {
    const char* fName;
    int fPdgCode;
    scaltype fMinEnergy;
    bool fHasTrd;
  };

  static SignalParticle particleDescs[];
  SignalParticle* fSignalParticle;

  LxTbBinnedStation* stations;
  //int nofStations;
  LxTbBinnedTrdStation trdStation;
  timetype minT;
  timetype maxT;
  std::list<Chain*>* recoTracks;
  int nofTrackBins;
  bool fHasTrd;
  int fNofTrdLayers;

  TriggerTimeArray triggerTimes_trd0_sign0_dist0;
  TriggerTimeArray triggerTimes_trd0_sign0_dist1;
  TriggerTimeArray triggerTimes_trd0_sign1_dist0;
  TriggerTimeArray triggerTimes_trd0_sign1_dist1;
  TriggerTimeArray triggerTimes_trd1_sign0_dist0;
  TriggerTimeArray triggerTimes_trd1_sign0_dist1;
  TriggerTimeArray triggerTimes_trd1_sign1_dist0;
  TriggerTimeArray triggerTimes_trd1_sign1_dist1;
  TriggerTimeArray triggerTimes_trd05_sign0_dist0;
  TriggerTimeArray triggerTimes_trd05_sign0_dist1;
  TriggerTimeArray triggerTimes_trd05_sign1_dist0;
  TriggerTimeArray triggerTimes_trd05_sign1_dist1;
#ifdef LXTB_QA
  std::set<Int_t> triggerEventNumber;
#endif  //LXTB_QA

  int fNofStations;
  int fLastStationNumber;
  int fNofTimeBins;
  int fLastTimeBinNumber;
  int fTimeBinLength;
  int fTimeSliceLength;

  LxTbBinnedFinder(int nofTrdLayers, int nofStations, int nofTimeBins, std::pair<int, int>* nofSpatBins,
                   int nofTrdXBins, int nofTrdYBins, int timeBinLength)
    : fSignalParticle(&particleDescs[0])
    , stations(reinterpret_cast<LxTbBinnedStation*>(new unsigned char[nofStations * sizeof(LxTbBinnedStation)]))
    , trdStation(nofTrdXBins, nofTrdYBins, nofTimeBins)
    , minT(0)
    , maxT(0)
    , recoTracks(new std::list<Chain*>[nofTimeBins])
    , nofTrackBins(nofTimeBins)
    , fHasTrd(nofTrdLayers > 0)
    , fNofTrdLayers(nofTrdLayers)
    , triggerTimes_trd0_sign0_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd0_sign0_dist1(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd0_sign1_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd0_sign1_dist1(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd1_sign0_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd1_sign0_dist1(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd1_sign1_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd1_sign1_dist1(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd05_sign0_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd05_sign0_dist1(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd05_sign1_dist0(nofTimeBins, timeBinLength, minT)
    , triggerTimes_trd05_sign1_dist1(nofTimeBins, timeBinLength, minT)
    ,
#ifdef LXTB_QA
    triggerEventNumber()
    ,
#endif  //LXTB_QA
    fNofStations(nofStations)
    , fLastStationNumber(nofStations - 1)
    , fNofTimeBins(nofTimeBins)
    , fLastTimeBinNumber(nofTimeBins - 1)
    , fTimeBinLength(timeBinLength)
    , fTimeSliceLength(nofTimeBins * timeBinLength)
  {
    for (int i = 0; i < fNofStations; ++i)
      new (&stations[i]) LxTbBinnedStation(nofSpatBins[i].first, nofSpatBins[i].second, nofTimeBins);
  }

  LxTbBinnedFinder(const LxTbBinnedFinder&) = delete;
  LxTbBinnedFinder& operator=(const LxTbBinnedFinder&) = delete;

  virtual ~LxTbBinnedFinder()
  {
    for (int i = 0; i < fNofStations; ++i)
      delete[] reinterpret_cast<unsigned char*>(&stations[i]);

    delete[] recoTracks;
  }

  void SetSignalParticle(const char* name)
  {
    for (int i = 0; true; ++i) {
      if (0 == strlen(particleDescs[i].fName)) break;

      if (0 == strcmp(particleDescs[i].fName, name)) {
        fSignalParticle = &particleDescs[i];
        break;
      }
    }
  }

  void Clear()
  {
    for (int i = 0; i < fNofStations; ++i)
      stations[i].Clear();

    trdStation.Clear();

    for (int i = 0; i < nofTrackBins; ++i)
      recoTracks[i].clear();

    triggerTimes_trd0_sign0_dist0.Clear();
    triggerTimes_trd0_sign0_dist1.Clear();
    triggerTimes_trd0_sign1_dist0.Clear();
    triggerTimes_trd0_sign1_dist1.Clear();
    triggerTimes_trd1_sign0_dist0.Clear();
    triggerTimes_trd1_sign0_dist1.Clear();
    triggerTimes_trd1_sign1_dist0.Clear();
    triggerTimes_trd1_sign1_dist1.Clear();
    triggerTimes_trd05_sign0_dist0.Clear();
    triggerTimes_trd05_sign0_dist1.Clear();
    triggerTimes_trd05_sign1_dist0.Clear();
    triggerTimes_trd05_sign1_dist1.Clear();
#ifdef LXTB_QA
    triggerEventNumber.clear();
#endif  //LXTB_QA
  }

  void SetTSBegin(unsigned long long tsLowBound) { minT = tsLowBound; }

  void Init()
  {
    fHasTrd     = fHasTrd && fSignalParticle->fHasTrd;
    scaltype E  = fSignalParticle->fMinEnergy;  // GeV
    scaltype E0 = E;

    for (int i = 0; i < fNofStations; ++i) {
      stations[i].binSizeX = (stations[i].maxX - stations[i].minX) / stations[i].nofXBins;
      stations[i].binSizeY = (stations[i].maxY - stations[i].minY) / stations[i].nofYBins;
      scaltype L           = stations[i].absorber.width;  // / cos(3.14159265 * 15 / 180);
      E -= EnergyLoss(E, L, &stations[i].absorber);

      if (i > 0) {
        scaltype Escat          = (E0 + E) / 2;
        scaltype deltaTheta     = CalcThetaPrj(Escat, L, &stations[i].absorber);
        stations[i].deltaThetaX = deltaTheta;
        stations[i].deltaThetaY = deltaTheta;
        scaltype q0XSq          = stations[i].deltaThetaX * stations[i].deltaThetaX;
        scaltype q0YSq          = stations[i].deltaThetaY * stations[i].deltaThetaY;
        stations[i].qs[0]       = {q0XSq * L * L / 3, q0XSq * L / 2, q0XSq * L / 2, q0XSq};
        stations[i].qs[1]       = {q0YSq * L * L / 3, q0YSq * L / 2, q0YSq * L / 2, q0YSq};
      }

      E0 = E;
    }

    scaltype Ls[fNofStations + 1];
    Ls[0] = stations[0].absorber.zCoord;

    for (int i = 1; i < fNofStations; ++i)
      Ls[i] = stations[i].absorber.zCoord - Ls[i - 1];

    Ls[fNofStations] = stations[fLastStationNumber].z - Ls[fLastStationNumber];

    scaltype Ls1[fNofStations + 1];
    scaltype Ls2[fNofStations + 1];

    Ls1[0] = 0;
    Ls2[0] = stations[0].absorber.zCoord;

    for (int i = 1; i < fNofStations; ++i) {
      Ls1[i] = stations[i - 1].z - stations[i - 1].absorber.zCoord;
      Ls2[i] = stations[i].absorber.zCoord - stations[i - 1].z;
    }

    Ls1[fNofStations] = stations[fLastStationNumber].z - stations[fLastStationNumber].absorber.zCoord;
    Ls2[fNofStations] = 0;

    for (int s = 1; s < fNofStations; ++s) {
      LxTbBinnedStation& station = stations[s];
      scaltype L                 = station.z;
      int n                      = s + 1;
      scaltype thetaXSq          = 0;
      scaltype thetaYSq          = 0;

      for (int i = 1; i < n; ++i) {
        scaltype sumLi = 0;

        for (int j = 0; j < i; ++j)
          sumLi += Ls[j];

        thetaXSq += sumLi * sumLi * stations[i].deltaThetaX * stations[i].deltaThetaX;
        thetaYSq += sumLi * sumLi * stations[i].deltaThetaY * stations[i].deltaThetaY;
      }

      thetaXSq /= L * L;
      thetaYSq /= L * L;

      thetaXSq += stations[s].deltaThetaX * stations[s].deltaThetaX * Ls2[n - 1] * Ls2[n - 1]
                  / ((Ls2[n - 1] + Ls1[n]) * (Ls2[n - 1] + Ls1[n]));
      thetaYSq += stations[s].deltaThetaY * stations[s].deltaThetaY * Ls2[n - 1] * Ls2[n - 1]
                  / ((Ls2[n - 1] + Ls1[n]) * (Ls2[n - 1] + Ls1[n]));

      scaltype deltaZ = stations[s].z - stations[s - 1].z;
      station.dispX   = deltaZ * sqrt(thetaXSq);
      station.dispY   = deltaZ * sqrt(thetaYSq);
    }

    if (fHasTrd) {
      trdStation.binSizeX = (trdStation.maxX - trdStation.minX) / trdStation.nofXBins;
      trdStation.binSizeY = (trdStation.maxY - trdStation.minY) / trdStation.nofYBins;
      scaltype L          = trdStation.absorber.width;  // / cos(3.14159265 * 15 / 180);
      E -= EnergyLoss(E, L, &trdStation.absorber);
      scaltype Escat         = (E0 + E) / 2;
      scaltype deltaTheta    = CalcThetaPrj(Escat, L, &trdStation.absorber);
      trdStation.deltaThetaX = deltaTheta;
      trdStation.deltaThetaY = deltaTheta;

      for (int i = 0; i < fNofTrdLayers; ++i) {
        scaltype deltaZ      = trdStation.Zs[i] - stations[fLastStationNumber].z;
        trdStation.dispXs[i] = trdStation.deltaThetaX * deltaZ;
        trdStation.dispYs[i] = trdStation.deltaThetaY * deltaZ;
      }

      E0 = E;
    }
  }

  void FindNeighbours(scaltype x, scaltype wX, scaltype y, scaltype wY, timetype t, timetype wT, int layerIndex,
                      std::list<LxTbBinnedPoint*>& results)
  {
    if (x + wX < trdStation.minX || x - wX > trdStation.maxX || y + wY < trdStation.minY || y - wY > trdStation.maxY
        || t + wT < minT || t - wT > maxT)
      return;

    int lTindMin = (t - wT - minT) / fTimeBinLength;

    if (lTindMin < 0) lTindMin = 0;
    else if (lTindMin > fLastTimeBinNumber)
      lTindMin = fLastTimeBinNumber;

    int lTindMax = (t + wT - minT) / fTimeBinLength;

    if (lTindMax < 0) lTindMax = 0;
    else if (lTindMax > fLastTimeBinNumber)
      lTindMax = fLastTimeBinNumber;

    int lYindMin = (y - wY - trdStation.minY) / trdStation.binSizeY;

    if (lYindMin < 0) lYindMin = 0;
    else if (lYindMin > trdStation.lastYBin)
      lYindMin = trdStation.lastYBin;

    int lYindMax = (y + wY - trdStation.minY) / trdStation.binSizeY;

    if (lYindMax < 0) lYindMax = 0;
    else if (lYindMax > trdStation.lastYBin)
      lYindMax = trdStation.lastYBin;

    int lXindMin = (x - wX - trdStation.minX) / trdStation.binSizeX;

    if (lXindMin < 0) lXindMin = 0;
    else if (lXindMin > trdStation.lastXBin)
      lXindMin = trdStation.lastXBin;

    int lXindMax = (x + wX - trdStation.minX) / trdStation.binSizeX;

    if (lXindMax < 0) lXindMax = 0;
    else if (lXindMax > trdStation.lastXBin)
      lXindMax = trdStation.lastXBin;

    LxTbTYXBin* tyxBins = trdStation.tyxBinsArr[layerIndex];

    for (int lTind = lTindMin; lTind <= lTindMax; ++lTind) {
      LxTbTYXBin& lTyxBin = tyxBins[lTind];

      for (int lYind = lYindMin; lYind <= lYindMax; ++lYind) {
        LxTbYXBin& lYxBin = lTyxBin.yxBins[lYind];

        for (int lXind = lXindMin; lXind <= lXindMax; ++lXind) {
          LxTbXBin& lXBin = lYxBin.xBins[lXind];

          for (std::list<LxTbBinnedPoint>::iterator i = lXBin.points.begin(); i != lXBin.points.end(); ++i) {
            LxTbBinnedPoint& point = *i;

            if (point.x > x - wX && point.x < x + wX && point.y > y - wY && point.y < y + wY && point.t > t - wT
                && point.t < t + wT)
              results.push_back(&point);
          }
        }
      }
    }
  }

  void Reconstruct()
  {
    //cout << "LxTbBinnedFinder::Reconstruct() started" << endl;
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long beginTime                 = ts.tv_sec * 1000000000 + ts.tv_nsec;
    timetype c                     = speedOfLight;
    maxT                           = minT + fTimeSliceLength;
    LxTbBinnedStation& lastStation = stations[fLastStationNumber];
    int nofXBins                   = lastStation.nofXBins;
    int nofYBins                   = lastStation.nofYBins;

    /*if (hasTrd)
        {
            scaltype lastZ = lastStation.z;
            scaltype deltaZsTrd[4] = {trdStation.Zs[0] - lastZ, trdStation.Zs[2] - lastZ, trdStation.Zs[2] - lastZ, trdStation.Zs[3] - lastZ};

            for (int i = 0; i < NOF_TIMEBINS; ++i)
            {

                LxTbTYXBin& lTyxBin = lastStation.tyxBins[i];

                for (int j = 0; j < nofYBins; ++j)
                {
                    LxTbYXBin& lYxBin = lTyxBin.yxBins[j];

                    for (int k = 0; k < nofXBins; ++k)
                    {
                        LxTbXBin& lXBin = lYxBin.xBins[k];

                        for (std::list<LxTbBinnedPoint>::iterator l = lXBin.points.begin(); l != lXBin.points.end(); ++l)
                        {
                            LxTbBinnedPoint& lPoint = *l;
                            scaltype tx = lPoint.x / lastZ;
                            scaltype ty = lPoint.y / lastZ;
                            scaltype trdPx0 = lPoint.x + tx * deltaZsTrd[0];
                            scaltype trdPy0 = lPoint.y + ty * deltaZsTrd[0];
                            scaltype trajLen0 = sqrt(1 + tx * tx + ty * ty) * deltaZsTrd[0];
                            scaltype trdPt0 = lPoint.t + 1.e9 * trajLen0 / c; // 1.e9 to convert to ns.
                            scaltype wX0 = NOF_SIGMAS * trdStation.dispXs[0];
                            scaltype wY0 = NOF_SIGMAS * trdStation.dispYs[0];
                            scaltype wT0 = NOF_SIGMAS * sqrt(2.0) * lPoint.dt;
                            std::list<LxTbBinnedPoint*> results0;
                            FindNeighbours(trdPx0, wX0, trdPy0, wY0, trdPt0, wT0, 0, results0);

                            scaltype trdPx1 = lPoint.x + tx * deltaZsTrd[1];
                            scaltype trdPy1 = lPoint.y + ty * deltaZsTrd[1];
                            scaltype trajLen1 = sqrt(1 + tx * tx + ty * ty) * deltaZsTrd[1];
                            scaltype trdPt1 = lPoint.t + 1.e9 * trajLen1 / c; // 1.e9 to convert to ns.
                            scaltype wX1 = NOF_SIGMAS * trdStation.dispXs[1];
                            scaltype wY1 = NOF_SIGMAS * trdStation.dispYs[1];
                            scaltype wT1 = NOF_SIGMAS * sqrt(2.0) * lPoint.dt;
                            std::list<LxTbBinnedPoint*> results1;
                            FindNeighbours(trdPx1, wX1, trdPy1, wY1, trdPt1, wT1, 1, results1);

                            for (std::list<LxTbBinnedPoint*>::const_iterator m = results0.begin(); m != results0.end(); ++m)
                            {
                                LxTbBinnedPoint& trdPoint0 = *(*m);

                                for (std::list<LxTbBinnedPoint*>::const_iterator n = results1.begin(); n != results1.end(); ++n)
                                {
                                    LxTbBinnedPoint& trdPoint1 = *(*n);
                                    scaltype trdTx = (trdPoint1.x - trdPoint0.x) / (trdStation.Zs[1] - trdStation.Zs[0]);
                                    scaltype trdTy = (trdPoint1.y - trdPoint0.y) / (trdStation.Zs[1] - trdStation.Zs[0]);

                                    scaltype trdPx2 = trdPoint1.x + trdTx * (trdStation.Zs[2] - trdStation.Zs[1]);
                                    scaltype trdPy2 = trdPoint1.y + trdTy * (trdStation.Zs[2] - trdStation.Zs[1]);
                                    scaltype trajLen2 = sqrt(1 + trdTx * trdTx + trdTx * trdTx) * (trdStation.Zs[2] - trdStation.Zs[1]);
                                    scaltype trdPt2 = trdPoint1.t + 1.e9 * trajLen2 / c; // 1.e9 to convert to ns.
                                    scaltype wX2 = 0.35135;
                                    scaltype wY2 = 0.33515;
                                    scaltype wT2 = NOF_SIGMAS * sqrt(2.0) * trdPoint1.dt;
                                    std::list<LxTbBinnedPoint*> results2;
                                    FindNeighbours(trdPx2, wX2, trdPy2, wY2, trdPt2, wT2, 2, results2);

                                    if (results2.empty())
                                        continue;

                                    scaltype trdPx3 = trdPoint1.x + trdTx * (trdStation.Zs[3] - trdStation.Zs[1]);
                                    scaltype trdPy3 = trdPoint1.y + trdTy * (trdStation.Zs[3] - trdStation.Zs[1]);
                                    scaltype trajLen3 = sqrt(1 + trdTx * trdTx + trdTx * trdTx) * (trdStation.Zs[3] - trdStation.Zs[1]);
                                    scaltype trdPt3 = trdPoint1.t + 1.e9 * trajLen3 / c; // 1.e9 to convert to ns.
                                    scaltype wX3 = 0.909;
                                    scaltype wY3 = 0.8455;
                                    scaltype wT3 = NOF_SIGMAS * sqrt(2.0) * trdPoint1.dt;
                                    std::list<LxTbBinnedPoint*> results3;
                                    FindNeighbours(trdPx3, wX3, trdPy3, wY3, trdPt3, wT3, 3, results3);

                                    if (results3.empty())
                                        continue;

                                    lPoint.use = true;
                                    lXBin.use = true;
                                    lYxBin.use = true;
                                    lTyxBin.use = true;
                                }
                            }
                        }
                    }
                }
            }
        }*/

    for (int i = fLastStationNumber; i >= 1; --i) {
      LxTbBinnedStation& rStation = stations[i];
      LxTbBinnedStation& lStation = stations[i - 1];
      scaltype binSizeX           = lStation.binSizeX;
      scaltype binSizeY           = lStation.binSizeY;
      scaltype rZ                 = rStation.z;
      scaltype lZ                 = lStation.z;
      scaltype deltaZ             = lZ - rZ;
      scaltype dispXSq            = rStation.dispX * rStation.dispX;
      scaltype dispYSq            = rStation.dispY * rStation.dispY;
      scaltype minX               = lStation.minX;
      scaltype maxX               = lStation.maxX;
      scaltype minY               = lStation.minY;
      scaltype maxY               = lStation.maxY;
      int rNofXBins               = rStation.nofXBins;
      int rNofYBins               = rStation.nofYBins;
      int lastXBin                = lStation.nofXBins - 1;
      int lastYBin                = lStation.nofYBins - 1;

      for (int j = 0; j < fNofTimeBins; ++j) {
        LxTbTYXBin& rTyxBin = rStation.tyxBins[j];

        if (!rTyxBin.use) continue;

        for (int k = 0; k < rNofYBins; ++k) {
          LxTbYXBin& rYxBin = rTyxBin.yxBins[k];

          if (!rYxBin.use) continue;

          for (int l = 0; l < rNofXBins; ++l) {
            LxTbXBin& rXBin = rYxBin.xBins[l];

            if (!rXBin.use) continue;

            for (std::list<LxTbBinnedPoint>::iterator m = rXBin.points.begin(); m != rXBin.points.end(); ++m) {
              LxTbBinnedPoint& rPoint = *m;

              if (!rPoint.use) continue;

              scaltype tx      = rPoint.x / rZ;
              scaltype ty      = rPoint.y / rZ;
              scaltype pX      = rPoint.x + tx * deltaZ;
              scaltype pY      = rPoint.y + ty * deltaZ;
              scaltype trajLen = sqrt(1 + tx * tx + ty * ty) * deltaZ;
              timetype pT      = rPoint.t + 1.e9 * trajLen / c;  // 1.e9 to convert to ns and trajLen is negative.
              scaltype rDxSq   = rPoint.dx * rPoint.dx;
              scaltype xDiv0   = dispXSq + rDxSq;
              scaltype wX      = NOF_SIGMAS * sqrt(xDiv0 + rDxSq);
              scaltype rDySq   = rPoint.dy * rPoint.dy;
              scaltype yDiv0   = dispYSq + rDySq;
              scaltype wY      = NOF_SIGMAS * sqrt(yDiv0 + rDySq);
              timetype wT      = NOF_SIGMAS * sqrt(2.0) * rPoint.dt;

              if (pX + wX < minX || pX - wX > maxX || pY + wY < minY || pY - wY > maxY || pT + wT < minT
                  || pT - wT > maxT)
                continue;

              int lTindMin = (pT - wT - minT) / fTimeBinLength;

              if (lTindMin < 0) lTindMin = 0;
              else if (lTindMin > fLastTimeBinNumber)
                lTindMin = fLastTimeBinNumber;

              int lTindMax = (pT + wT - minT) / fTimeBinLength;

              if (lTindMax < 0) lTindMax = 0;
              else if (lTindMax > fLastTimeBinNumber)
                lTindMax = fLastTimeBinNumber;

              int lYindMin = (pY - wY - minY) / binSizeY;

              if (lYindMin < 0) lYindMin = 0;
              else if (lYindMin > lastYBin)
                lYindMin = lastYBin;

              int lYindMax = (pY + wY - minY) / binSizeY;

              if (lYindMax < 0) lYindMax = 0;
              else if (lYindMax > lastYBin)
                lYindMax = lastYBin;

              int lXindMin = (pX - wX - minX) / binSizeX;

              if (lXindMin < 0) lXindMin = 0;
              else if (lXindMin > lastXBin)
                lXindMin = lastXBin;

              int lXindMax = (pX + wX - minX) / binSizeX;

              if (lXindMax < 0) lXindMax = 0;
              else if (lXindMax > lastXBin)
                lXindMax = lastXBin;

              for (int lTind = lTindMin; lTind <= lTindMax; ++lTind) {
                LxTbTYXBin& lTyxBin = lStation.tyxBins[lTind];

                for (int lYind = lYindMin; lYind <= lYindMax; ++lYind) {
                  LxTbYXBin& lYxBin = lTyxBin.yxBins[lYind];

                  for (int lXind = lXindMin; lXind <= lXindMax; ++lXind) {
                    LxTbXBin& lXBin = lYxBin.xBins[lXind];

                    for (std::list<LxTbBinnedPoint>::iterator n = lXBin.points.begin(); n != lXBin.points.end(); ++n) {
                      LxTbBinnedPoint& lPoint = *n;
                      scaltype xDiv           = xDiv0 + lPoint.dx * lPoint.dx;
                      scaltype wX_prec_sq     = NOF_SIGMAS_SQ * xDiv;
                      scaltype yDiv           = yDiv0 + lPoint.dy * lPoint.dy;
                      scaltype wY_prec_sq     = NOF_SIGMAS_SQ * yDiv;
                      scaltype deltaX         = lPoint.x - pX;
                      scaltype deltaXSq       = deltaX * deltaX;
                      scaltype deltaY         = lPoint.y - pY;
                      scaltype deltaYSq       = deltaY * deltaY;
                      timetype deltaT         = lPoint.t - pT;
                      timetype deltaTSq       = deltaT * deltaT;

                      if (deltaXSq < wX_prec_sq && deltaYSq < wY_prec_sq && deltaTSq < wT * wT) {
                        lPoint.use = true;
                        lXBin.use  = true;
                        rPoint.neighbours.push_back(
                          LxTbBinnedRay(deltaZ, rPoint, lPoint,
                                        deltaXSq / xDiv + deltaYSq / yDiv
                                          + deltaTSq / (rPoint.dt * rPoint.dt + lPoint.dt * lPoint.dt)));
                      }
                    }

                    if (lXBin.use) lYxBin.use = true;
                  }

                  if (lYxBin.use) lTyxBin.use = true;
                }
              }
            }  // for (std::list<LxTbBinnedPoint>::iterator m = rStation
          }    // for (int l = 0; l < NOF_XBINS; ++l)
        }      // for (int k = 0; k < NOF_YBINS; ++k)
      }        // for (int j = 0; j < NOF_TIMEBINS; ++j)
    }          // for (int i = fLastStationNumber; i >=1; --i)

    const LxTbBinnedPoint* points[fNofStations];

    for (int i = 0; i < fNofTimeBins; ++i) {
      LxTbTYXBin& rTyxBin = lastStation.tyxBins[i];

      if (!rTyxBin.use) continue;

      for (int j = 0; j < nofYBins; ++j) {
        LxTbYXBin& rYxBin = rTyxBin.yxBins[j];

        if (!rYxBin.use) continue;

        for (int k = 0; k < nofXBins; ++k) {
          LxTbXBin& rXBin = rYxBin.xBins[k];

          if (!rXBin.use) continue;

          for (std::list<LxTbBinnedPoint>::iterator l = rXBin.points.begin(); l != rXBin.points.end(); ++l) {
            LxTbBinnedPoint& rPoint = *l;
            std::list<ChainImpl> chains;

            KFParams kfParams = {{rPoint.x, rPoint.x / lastStation.z, rPoint.dx * rPoint.dx, 0, 0, 1.0},
                                 {rPoint.y, rPoint.y / lastStation.z, rPoint.dy * rPoint.dy, 0, 0, 1.0},
                                 0};

            FindChains(fLastStationNumber, &rPoint, 0, points, kfParams, chains);
            const ChainImpl* bestChain = 0;
            scaltype chi2              = 0;

            for (std::list<ChainImpl>::const_iterator m = chains.begin(); m != chains.end(); ++m) {
              const ChainImpl& chain = *m;

              if (0 == bestChain || chain.chi2 < chi2) {
                bestChain = &chain;
                chi2      = chain.chi2;
              }
            }

            if (0 != bestChain) {
              int trackBinInd = (bestChain->points[fLastStationNumber]->t - minT) / fTimeBinLength;

              if (trackBinInd < 0) trackBinInd = 0;
              else if (trackBinInd > nofTrackBins)
                trackBinInd = nofTrackBins;

              recoTracks[trackBinInd].push_back(new Chain(bestChain->points, fNofStations, chi2));
            }

            for (std::list<ChainImpl>::iterator m = chains.begin(); m != chains.end(); ++m) {
              ChainImpl& chain = *m;
              delete[] chain.points;
            }
          }
        }
      }
    }

    if (fHasTrd) {
      scaltype lastZ                       = lastStation.z;
      LxTbBinnedStation& beforeLastStation = stations[fLastStationNumber - 1];
      scaltype beforeLastZ                 = beforeLastStation.z;
      scaltype deltaZsTrd[4] = {trdStation.Zs[0] - lastZ, trdStation.Zs[2] - lastZ, trdStation.Zs[2] - lastZ,
                                trdStation.Zs[3] - lastZ};
      scaltype dispX0Sq      = trdStation.dispXs[0] * trdStation.dispXs[0];
      scaltype dispY0Sq      = trdStation.dispYs[0] * trdStation.dispYs[0];
      scaltype dispX1Sq      = trdStation.dispXs[1] * trdStation.dispXs[1];
      scaltype dispY1Sq      = trdStation.dispYs[1] * trdStation.dispYs[1];
      scaltype trdDeltaZ10   = (trdStation.Zs[1] - trdStation.Zs[0]);
      scaltype trdDeltaZ10Sq = trdDeltaZ10 * trdDeltaZ10;
      scaltype trdDeltaZ21   = (trdStation.Zs[2] - trdStation.Zs[1]);
      scaltype trdDeltaZ21Sq = trdDeltaZ21 * trdDeltaZ21;
      scaltype trdDeltaZ31   = (trdStation.Zs[3] - trdStation.Zs[1]);
      scaltype trdDeltaZ31Sq = trdDeltaZ31 * trdDeltaZ31;

      for (int i = 0; i < nofTrackBins; ++i) {
        std::list<Chain*>& recoTracksBin = recoTracks[i];

        for (std::list<Chain*>::iterator j = recoTracksBin.begin(); j != recoTracksBin.end(); ++j) {
          Chain* track             = *j;
          LxTbBinnedPoint& lPoint  = *track->points[track->nofPoints - 1];
          LxTbBinnedPoint& lPointL = *track->points[track->nofPoints - 2];
          scaltype tx              = (lPoint.x - lPointL.x) / (lastZ - beforeLastZ);
          scaltype ty              = (lPoint.y - lPointL.y) / (lastZ - beforeLastZ);
          scaltype trdPx0          = lPoint.x + tx * deltaZsTrd[0];
          scaltype trdPy0          = lPoint.y + ty * deltaZsTrd[0];
          scaltype trajLen0        = sqrt(1 + tx * tx + ty * ty) * deltaZsTrd[0];
          timetype trdPt0          = lPoint.t + 1.e9 * trajLen0 / c;  // 1.e9 to convert to ns.
          scaltype wX0             = NOF_SIGMAS * sqrt(dispX0Sq + 2 * lPoint.dx * lPoint.dx);
          scaltype wY0             = NOF_SIGMAS * sqrt(dispY0Sq + 2 * lPoint.dy * lPoint.dy);
          timetype wT0             = NOF_SIGMAS * sqrt(2.0) * lPoint.dt;
          std::list<LxTbBinnedPoint*> results0;
          FindNeighbours(trdPx0, wX0, trdPy0, wY0, trdPt0, wT0, 0, results0);

          scaltype trdPx1   = lPoint.x + tx * deltaZsTrd[1];
          scaltype trdPy1   = lPoint.y + ty * deltaZsTrd[1];
          scaltype trajLen1 = sqrt(1 + tx * tx + ty * ty) * deltaZsTrd[1];
          timetype trdPt1   = lPoint.t + 1.e9 * trajLen1 / c;  // 1.e9 to convert to ns.
          scaltype wX1      = NOF_SIGMAS * sqrt(dispX1Sq + 2 * lPoint.dx * lPoint.dx);
          scaltype wY1      = NOF_SIGMAS * sqrt(dispY1Sq + 2 * lPoint.dy * lPoint.dy);
          timetype wT1      = NOF_SIGMAS * sqrt(2.0) * lPoint.dt;
          std::list<LxTbBinnedPoint*> results1;
          FindNeighbours(trdPx1, wX1, trdPy1, wY1, trdPt1, wT1, 1, results1);

          for (std::list<LxTbBinnedPoint*>::const_iterator k = results0.begin(); k != results0.end(); ++k) {
            LxTbBinnedPoint& trdPoint0 = *(*k);

            for (std::list<LxTbBinnedPoint*>::const_iterator l = results1.begin(); l != results1.end(); ++l) {
              LxTbBinnedPoint& trdPoint1 = *(*l);
              scaltype trdTx             = (trdPoint1.x - trdPoint0.x) / trdDeltaZ10;
              scaltype trdTy             = (trdPoint1.y - trdPoint0.y) / trdDeltaZ10;
              scaltype trdDtxSq          = (trdPoint0.dx * trdPoint0.dx + trdPoint1.dx * trdPoint1.dx) / trdDeltaZ10Sq;
              scaltype trdDtySq          = (trdPoint0.dy * trdPoint0.dy + trdPoint1.dy * trdPoint1.dy) / trdDeltaZ10Sq;

              scaltype trdPx2   = trdPoint1.x + trdTx * trdDeltaZ21;
              scaltype trdPy2   = trdPoint1.y + trdTy * trdDeltaZ21;
              scaltype trajLen2 = sqrt(1 + trdTx * trdTx + trdTx * trdTx) * trdDeltaZ21;
              timetype trdPt2   = trdPoint1.t + 1.e9 * trajLen2 / c;  // 1.e9 to convert to ns.
              scaltype wX2 =
                NOF_SIGMAS
                * sqrt(0.0878375 * 0.0878375 + trdDtxSq * trdDeltaZ21Sq + trdPoint1.dx * trdPoint1.dx);  //0.35135;
              scaltype wY2 =
                NOF_SIGMAS
                * sqrt(0.0837875 * 0.0837875 + trdDtySq * trdDeltaZ21Sq + trdPoint1.dy * trdPoint1.dy);  //0.33515;
              timetype wT2 = NOF_SIGMAS * sqrt(2.0) * trdPoint1.dt;
              std::list<LxTbBinnedPoint*> results2;
              FindNeighbours(trdPx2, wX2, trdPy2, wY2, trdPt2, wT2, 2, results2);

              if (results2.empty()) continue;

              scaltype trdPx3   = trdPoint1.x + trdTx * trdDeltaZ31;
              scaltype trdPy3   = trdPoint1.y + trdTy * trdDeltaZ31;
              scaltype trajLen3 = sqrt(1 + trdTx * trdTx + trdTx * trdTx) * trdDeltaZ31;
              timetype trdPt3   = trdPoint1.t + 1.e9 * trajLen3 / c;  // 1.e9 to convert to ns.
              scaltype wX3 =
                NOF_SIGMAS * sqrt(0.22725 * 0.22725 + trdDtxSq * trdDeltaZ31Sq + trdPoint1.dx * trdPoint1.dx);  //0.909;
              scaltype wY3 =
                NOF_SIGMAS
                * sqrt(0.211375 * 0.211375 + trdDtySq * trdDeltaZ31Sq + trdPoint1.dy * trdPoint1.dy);  //0.8455;
              timetype wT3 = NOF_SIGMAS * sqrt(2.0) * trdPoint1.dt;
              std::list<LxTbBinnedPoint*> results3;
              FindNeighbours(trdPx3, wX3, trdPy3, wY3, trdPt3, wT3, 3, results3);

              if (!results3.empty()) {
                track->highMom = true;
                break;
              }
            }

            if (track->highMom) break;
          }
        }
      }
    }

    Trigger();

    clock_gettime(CLOCK_REALTIME, &ts);
    long endTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
    fullDuration += endTime - beginTime;
    //cout << "LxTbNaiveFinder::Reconstruct() full duration was: " << fullDuration << endl;
    //cout << "LxTbNaiveFinder::Reconstruct() the number of found tracks: " << recoTracks.size() << endl;
  }

  void FindChains(int stationIndex, const LxTbBinnedPoint* rPoint, const LxTbBinnedRay* /*rRay*/,
                  const LxTbBinnedPoint** points, KFParams kfParamsPrev, std::list<ChainImpl>& chains)
  {
    points[stationIndex] = rPoint;

    if (0 == stationIndex) {
      ChainImpl chain(points, fNofStations, kfParamsPrev.chi2);
      chains.push_back(chain);
      return;
    }

    for (std::list<LxTbBinnedRay>::const_iterator i = rPoint->neighbours.begin(); i != rPoint->neighbours.end(); ++i) {
      const LxTbBinnedRay& lRay     = *i;
      const LxTbBinnedPoint* lPoint = lRay.lPoint;
      /*scaltype chi2_2 = chi2 + lRay.chi2;
            
            if (0 != rRay)
                //chi2_2 += (lRay.tx - rRay->tx) * (lRay.tx - rRay->tx) / (rRay->dtxSq + lRay.dtxSq) +
                            //(lRay.ty - rRay->ty) * (lRay.ty - rRay->ty) / (rRay->dtySq + lRay.dtySq);
                chi2_2 += (lRay.tx - rRay->tx) * (lRay.tx - rRay->tx) / (stations[stationIndex].deltaThetaX * stations[stationIndex].deltaThetaX) +
                        (lRay.ty - rRay->ty) * (lRay.ty - rRay->ty) / (stations[stationIndex].deltaThetaY * stations[stationIndex].deltaThetaY);*/
      KFParams kfParams = kfParamsPrev;
      scaltype m[2]     = {lPoint->x, lPoint->y};
      scaltype V[2]     = {lPoint->dx * lPoint->dx, lPoint->dy * lPoint->dy};
      KFAddPoint(kfParams, kfParamsPrev, m, V, stationIndex - 1);
      FindChains(stationIndex - 1, lPoint, &lRay, points, kfParams, chains);
    }
  }

  void TriggerBin(std::list<Chain*>& recoTracksBin, std::list<Chain*>& borderTracks, int i, bool handleBorder)
  {
    std::list<Chain*>::const_iterator it    = handleBorder ? borderTracks.begin() : recoTracksBin.begin();
    std::list<Chain*>::const_iterator endIt = handleBorder ? borderTracks.end() : recoTracksBin.end();

    for (; it != endIt; ++it) {
      Chain* track           = *it;
      LxTbBinnedPoint* point = track->points[fLastStationNumber];
      timetype dt            = point->dt;
      timetype dtSq          = dt * dt;
      timetype wt            = NOF_SIGMAS * sqrt(2.0) * dt;

      if (!handleBorder && point->t + wt > minT + (i + 1) * fTimeBinLength) borderTracks.push_back(track);

      scaltype trackSign = (track->points[1]->x - track->points[0]->x) / (stations[1].z - stations[0].z)
                           - track->points[0]->x / stations[0].z;
      std::list<Chain*>::const_iterator it2    = handleBorder ? recoTracksBin.begin() : it;
      std::list<Chain*>::const_iterator endIt2 = recoTracksBin.end();

      if (!handleBorder) ++it2;

      for (; it2 != endIt2; ++it2) {
        Chain* track2           = *it2;
        LxTbBinnedPoint* point2 = track2->points[fLastStationNumber];
        timetype dt2            = point2->dt;
        timetype dt2Sq          = dt2 * dt2;

        if (fabs(point2->t - point->t) > NOF_SIGMAS * sqrt(dtSq + dt2Sq)) continue;

        scaltype track2Sign = (track2->points[1]->x - track2->points[0]->x) / (stations[1].z - stations[0].z)
                              - track2->points[0]->x / stations[0].z;
        scaltype dist =
          sqrt((track2->points[0]->x - track->points[0]->x) * (track2->points[0]->x - track->points[0]->x)
               + (track2->points[0]->y - track->points[0]->y) * (track2->points[0]->y - track->points[0]->y));
        std::pair<timetype, timetype> pairTime((point->t + point2->t) / 2, sqrt(dtSq + dt2Sq) / 2);

        if (track->highMom && track2->highMom) {
          if (trackSign * track2Sign < 0) {
            if (dist >= 50.0) {
              triggerTimes_trd1_sign1_dist1.Insert(pairTime);
#ifdef LXTB_QA
              triggerEventNumber.insert(point->mcRefs.front().eventId);
#endif  //LXTB_QA
            }

            triggerTimes_trd1_sign1_dist0.Insert(pairTime);
          }

          if (dist >= 50.0) triggerTimes_trd1_sign0_dist1.Insert(pairTime);

          triggerTimes_trd1_sign0_dist0.Insert(pairTime);
        }

        if (track->highMom || track2->highMom) {
          if (trackSign * track2Sign < 0) {
            if (dist >= 50.0) triggerTimes_trd05_sign1_dist1.Insert(pairTime);

            triggerTimes_trd05_sign1_dist0.Insert(pairTime);
          }

          if (dist >= 50.0) triggerTimes_trd05_sign0_dist1.Insert(pairTime);

          triggerTimes_trd05_sign0_dist0.Insert(pairTime);
        }

        if (trackSign * track2Sign < 0) {
          if (dist >= 50.0) triggerTimes_trd0_sign1_dist1.Insert(pairTime);

          triggerTimes_trd0_sign1_dist0.Insert(pairTime);
        }

        if (dist >= 50.0) triggerTimes_trd0_sign0_dist1.Insert(pairTime);

        triggerTimes_trd0_sign0_dist0.Insert(pairTime);
      }
    }
  }

  void Trigger()
  {
    std::list<Chain*> borderTracks;

    for (int i = 0; i < nofTrackBins; ++i) {
      TriggerBin(recoTracks[i], borderTracks, 0, true);
      borderTracks.clear();
      TriggerBin(recoTracks[i], borderTracks, i, false);
    }
  }
};

#endif /* LXTBBINNED_H */
