/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Station4D.h
 * Author: tablyaz
 *
 * Created on August 7, 2017, 2:08 PM
 */

#ifndef STATION4D_H
#define STATION4D_H

#include "Station.h"

class CbmBinned4DStation : public CbmBinnedStation {
public:
  CbmBinned4DStation(ECbmModuleId stationType,
                     Double_t minZ,
                     Double_t maxZ,
                     int nofZBins,
                     int nofYBins,
                     int nofXBins,
                     int nofTBins)
    : CbmBinnedStation(stationType, minZ, maxZ, nofYBins, nofXBins, nofTBins)
    , fZBins(reinterpret_cast<CbmZBin*>(
        new unsigned char[nofZBins * sizeof(CbmZBin)]))
    , fNofZBins(nofZBins)
    , fZBinSize(0)
    , fDtx(0)
    , fDtxSq(0)
    , fDty(0)
    , fDtySq(0) {
    for (int i = 0; i < nofZBins; ++i)
      new (&fZBins[i]) CbmZBin(nofYBins, nofXBins, nofTBins);
  }

  void SetDtx(Double_t v) {
    if (v > fDtx) {
      fDtx   = v;
      fDtxSq = v * v;
    }
  }

  void SetDty(Double_t v) {
    if (v > fDty) {
      fDty   = v;
      fDtySq = v * v;
    }
  }

  void Clear() {
    fDtx   = 0;
    fDtxSq = 0;
    fDty   = 0;
    fDtySq = 0;
    CbmBinnedStation::Clear();

    for (int i = 0; i < fNofZBins; ++i) {
      CbmZBin& zBin = fZBins[i];
      zBin.SetUse(false);

      for (int j = 0; j < fNofYBins; ++j) {
        CbmYBin& yBin = zBin[j];
        yBin.SetUse(false);

        for (int k = 0; k < fNofXBins; ++k) {
          CbmXBin& xBin = yBin[k];
          xBin.SetUse(false);

          for (int l = 0; l < fNofTBins; ++l) {
            CbmTBin& tBin = xBin[l];
            tBin.SetUse(false);
            tBin.Clear();
          }
        }
      }
    }
  }

  void AddHit(ECbmModuleId type, const CbmPixelHit* hit, Int_t index) {
    Double_t z = hit->GetZ();

    if (z < fMinZ || z >= fMaxZ) return;

    Double_t y = hit->GetY();

    if (y < fMinY || y >= fMaxY) return;

    Double_t x = hit->GetX();

    if (x < fMinX || x >= fMaxX) return;

    Double_t t = hit->GetTime();

    if (t < fMinT || t >= fMaxT) return;

    int zInd = (z - fMinZ) / fZBinSize;
    int yInd = (y - fMinY) / fYBinSize;
    int xInd = (x - fMinX) / fXBinSize;
    int tInd = (t - fMinT) / fTBinSize;

    CbmZBin& zBin = fZBins[zInd];
    CbmYBin& yBin = zBin[yInd];
    CbmXBin& xBin = yBin[xInd];
    CbmTBin& tBin = xBin[tInd];
    tBin.AddHit(type, hit, index, fDefaultUse);
    SetDx(hit->GetDx());
    SetDy(hit->GetDy());
    SetDt(hit->GetTimeError());
    SetDtx(hit->GetDx() / hit->GetZ());
    SetDty(hit->GetDy() / hit->GetZ());

    if (fDefaultUse) {
      zBin.SetUse(true);
      yBin.SetUse(true);
      xBin.SetUse(true);
      tBin.SetUse(true);
    }
  }

  void IterateHits(std::function<void(CbmTBin::HitHolder&)> handleHit) {
    for (int i = 0; i < fNofZBins; ++i) {
      CbmZBin& zBin = fZBins[i];

      if (!zBin.Use()) continue;

      for (int j = 0; j < fNofYBins; ++j) {
        CbmYBin& yBin = zBin[j];

        if (!yBin.Use()) continue;

        for (int k = 0; k < fNofXBins; ++k) {
          CbmXBin& xBin = yBin[k];

          if (!xBin.Use()) continue;

          for (int l = 0; l < fNofTBins; ++l) {
            CbmTBin& tBin = xBin[l];

            if (!tBin.Use()) continue;

            for (std::list<CbmTBin::HitHolder>::iterator hi = tBin.HitsBegin();
                 hi != tBin.HitsEnd();
                 ++hi) {
              CbmTBin::HitHolder& hitHolder = *hi;

              if (fStage > hitHolder.stage) continue;

              handleHit(hitHolder);
            }
          }
        }
      }
    }
  }

  int GetZInd(Double_t v) const {
    int ind = (v - fMinZ) / fZBinSize;

    if (ind < 0)
      ind = 0;
    else if (ind >= fNofZBins)
      ind = fNofZBins - 1;

    return ind;
  }

  Double_t GetYEnterZ(int yInd, Double_t ty) const {
    if (ty > 0)
      return (fMinY + yInd * fYBinSize) / ty;
    else
      return (fMinY + (yInd + 1) * fYBinSize) / ty;
  }

  Double_t GetYExitZ(int yInd, Double_t ty) const {
    if (ty > 0)
      return (fMinY + (yInd + 1) * fYBinSize) / ty;
    else
      return (fMinY + yInd * fYBinSize) / ty;
  }

  Double_t GetXEnterZ(int xInd, Double_t tx) const {
    if (tx > 0)
      return (fMinX + xInd * fXBinSize) / tx;
    else
      return (fMinX + (xInd + 1) * fXBinSize) / tx;
  }

  Double_t GetXExitZ(int xInd, Double_t tx) const {
    if (tx > 0)
      return (fMinX + (xInd + 1) * fXBinSize) / tx;
    else
      return (fMinX + xInd * fXBinSize) / tx;
  }

  void Init() {
    fZBinSize = (fMaxZ - fMinZ) / fNofZBins;
    CbmBinnedStation::Init();
  }

  void SearchHits(const CbmTrackParam2& stateVec,
                  Double_t stateZ,
                  std::function<void(CbmTBin::HitHolder&)> handleHit) {
    Double_t deltaZmin = stateZ - fMinZ;
    Double_t tx        = stateVec.GetTx();
    Double_t ty        = stateVec.GetTy();
    //Double_t C[21];
    //stateVec.CovMatrix(C);
    Double_t wTx   = cbmBinnedSigma * std::sqrt(stateVec.GetCovTxTx() + fDtxSq);
    Double_t minTx = tx - wTx;
    Double_t maxTx = tx + wTx;
    Double_t wTy   = cbmBinnedSigma * std::sqrt(stateVec.GetCovTyTy() + fDtySq);
    Double_t minTy = ty - wTy;
    Double_t maxTy = ty + wTy;

    if (minTy > 0 && stateVec.GetY() + minTy * deltaZmin >= fMaxY)
      return;
    else if (maxTy < 0 && stateVec.GetY() + maxTy * deltaZmin < fMinY)
      return;

    if (minTx > 0 && stateVec.GetX() + minTx * deltaZmin >= fMaxX)
      return;
    else if (maxTx < 0 && stateVec.GetX() + minTx * deltaZmin < fMinX)
      return;

    Double_t minZ = fMinZ;
    Double_t maxZ = fMaxZ;

    Double_t searchZ = stateZ;
    Double_t searchX = stateVec.GetX();
    Double_t searchY = stateVec.GetY();

    if (minTy > 0) {
      if ((fMaxY - searchY) / minTy + searchZ < maxZ)
        maxZ = (fMaxY - searchY) / minTy + searchZ;
    } else if (maxTy < 0) {
      if ((fMinY - searchY) / maxTy + searchZ < maxZ)
        maxZ = (fMinY - searchY) / maxTy + searchZ;
    }

    if (minTx > 0) {
      if ((fMaxX - searchX) / minTx + searchZ < maxZ)
        maxZ = (fMaxX - searchX) / minTx + searchZ;
    } else if (maxTx < 0) {
      if ((fMinX - searchX) / maxTx + searchZ < maxZ)
        maxZ = (fMinX - searchX) / maxTx + searchZ;
    }

    int maxZind = GetZInd(maxZ);

    for (int i = 0; i <= maxZind; ++i) {
      CbmZBin& zBin  = fZBins[i];
      Double_t minZi = minZ + i * fZBinSize;
      Double_t maxZi = minZ + (i + 1) * fZBinSize;
      Double_t minY  = minTy > 0 ? minTy * (minZi - searchZ) + searchY
                                : minTy * (maxZi - searchZ) + searchY;
      int minYind   = GetYInd(minY);
      Double_t maxY = maxTy > 0 ? maxTy * (maxZi - searchZ) + searchY
                                : maxTy * (minZi - searchZ) + searchY;
      int maxYind = GetYInd(maxY);

      for (int j = minYind; j <= maxYind; ++j) {
        CbmYBin& yBin  = zBin[j];
        Double_t minZj = maxZi;
        Double_t maxZj = minZi;
        Double_t tmp   = GetYEnterZ(j, minTy);

        if (tmp < minZj && tmp > minZi) minZj = tmp;

        tmp = GetYEnterZ(j, maxTy);

        if (tmp < minZj && tmp > minZi) minZj = tmp;

        if (minZj == maxZi) minZj = minZi;

        tmp = GetYExitZ(j, minTy);

        if (tmp > maxZj && tmp < maxZi) maxZj = tmp;

        tmp = GetYExitZ(j, maxTy);

        if (tmp > maxZj && tmp < maxZi) maxZj = tmp;

        if (maxZj == minZi) maxZj = maxZi;

        Double_t minX = minTx > 0 ? minTx * (minZj - searchZ) + searchX
                                  : minTx * (maxZj - searchZ) + searchX;
        Double_t maxX = maxTx > 0 ? maxTx * (maxZj - searchZ) + searchX
                                  : maxTx * (minZj - searchZ) + searchX;
        int minXind = GetXInd(minX);
        int maxXind = GetXInd(maxX);

        for (int k = minXind; k <= maxXind; ++k) {
          CbmXBin& xBin  = yBin[k];
          Double_t minZk = maxZj;
          Double_t maxZk = minZj;
          Double_t tmp2  = GetXEnterZ(k, minTx);

          if (tmp2 < minZk && tmp2 > minZj) minZk = tmp2;

          tmp2 = GetXEnterZ(k, maxTx);

          if (tmp2 < minZk && tmp2 > minZj) minZk = tmp2;

          if (minZk == maxZj) minZk = minZj;

          tmp2 = GetXExitZ(k, minTx);

          if (tmp2 > maxZk && tmp2 < maxZj) maxZk = tmp2;

          tmp2 = GetXExitZ(k, maxTx);

          if (tmp2 > maxZk && tmp2 < maxZj) maxZk = tmp2;

          if (maxZk == minZj) maxZk = maxZj;

          //Double_t minT = searchT + (minZk - searchZ) * timeCoeff - wT;
          //Double_t maxT = searchT + (maxZk - searchZ) * timeCoeff + wT;
          int minTind = 0;  //GetTInd(minT);
          int maxTind = 0;  //fNofTBins - 1;//GetTInd(maxT);

          for (int l = minTind; l <= maxTind; ++l) {
            CbmTBin& tBin                                   = xBin[l];
            std::list<CbmTBin::HitHolder>::iterator hitIter = tBin.HitsBegin();
            std::list<CbmTBin::HitHolder>::iterator hitIterEnd = tBin.HitsEnd();

            for (; hitIter != hitIterEnd; ++hitIter) {
              /*const CbmPixelHit* hit = hitIter->hit;
                            Double_t z = hit->GetZ();
                            Double_t zSq = z * z;
                            Double_t deltaY = hit->GetY() - ty * z;

                            if (deltaY * deltaY > cbmBinnedSigmaSq * (dTySq * zSq + hit->GetDy() * hit->GetDy()))
                                continue;

                            Double_t deltaX = hit->GetX() - tx * z;

                            if (deltaX * deltaX > cbmBinnedSigmaSq * (dTxSq * zSq + hit->GetDx() * hit->GetDx()))
                                continue;*/

              /*Double_t deltaT = hit->GetTime() - searchT - (z - searchZ) * timeCoeff;

                            if (deltaT * deltaT > cbmBinnedSigmaSq * (dtSq + hit->GetTimeError() * hit->GetTimeError()))
                                continue;*/

              CbmTBin::HitHolder& hitHolder = *hitIter;

              if (fStage > hitHolder.stage) continue;

              handleHit(hitHolder);
            }
          }
        }
      }
    }
  }

  void SearchHits(Segment& segment, std::function<void(CbmTBin::HitHolder&)>) {
    const CbmPixelHit* hit1 = segment.begin->hit;
    Double_t x1             = hit1->GetX();
    Double_t y1             = hit1->GetY();
    Double_t dx1Sq          = hit1->GetDx() * hit1->GetDx();
    Double_t dy1Sq          = hit1->GetDy() * hit1->GetDy();

    const CbmPixelHit* hit2 = segment.end->hit;
    Double_t x2             = hit2->GetX();
    Double_t y2             = hit2->GetY();
    Double_t dx2Sq          = hit2->GetDx() * hit2->GetDx();
    Double_t dy2Sq          = hit2->GetDy() * hit2->GetDy();

    Double_t segDeltaZ   = hit2->GetZ() - hit1->GetZ();
    Double_t segDeltaZSq = segDeltaZ * segDeltaZ;
    Double_t searchZ;
    //Double_t searchT;   (VF) not used
    //Double_t dtSq;   (VF) not used

    if (0 == hit1->GetTimeError()) {
      searchZ = hit2->GetZ();
      // searchT = hit2->GetTime();   (VF) not used
      // dtSq = hit2->GetTimeError() * hit2->GetTimeError();   (VF) not used
    } else {
      searchZ = (hit1->GetZ() + hit2->GetZ()) / 2;
      // searchT = (hit1->GetTime() + hit2->GetTime()) / 2;   (VF) not used
      // dtSq = (hit1->GetTimeError() * hit1->GetTimeError() + hit2->GetTimeError() * hit2->GetTimeError()) / 2;   (VF) not used
    }

    Double_t searchX   = (x1 + x2) / 2;
    Double_t searchY   = (y1 + y2) / 2;
    Double_t deltaZmin = searchZ - fMinZ;
    //Double_t wT = cbmBinnedSigma * std::sqrt(dtSq + fDt * fDt); (VF) unused
    Double_t tx = (x2 - x1) / segDeltaZ;
    Double_t ty = (y2 - y1) / segDeltaZ;
    //Double_t timeCoeff = std::sqrt(1 + tx * tx + ty * ty) / cbmBinnedSOL; (VF) unused
    Double_t dTxSq = (dx1Sq + dx2Sq) / segDeltaZSq;
    Double_t wTx   = cbmBinnedSigma * std::sqrt(dTxSq + fDtxSq);
    Double_t minTx = tx - wTx;
    Double_t maxTx = tx + wTx;
    Double_t dTySq = (dy1Sq + dy2Sq) / segDeltaZSq;
    Double_t wTy   = cbmBinnedSigma * std::sqrt(dTySq + fDtySq);
    Double_t minTy = ty - wTy;
    Double_t maxTy = ty + wTy;

    if (minTy > 0 && searchY + minTy * deltaZmin >= fMaxY)
      return;
    else if (maxTy < 0 && searchY + maxTy * deltaZmin < fMinY)
      return;

    if (minTx > 0 && searchX + minTx * deltaZmin >= fMaxX)
      return;
    else if (maxTx < 0 && searchX + minTx * deltaZmin < fMinX)
      return;

    Double_t minZ = fMinZ;
    Double_t maxZ = fMaxZ;

    if (minTy > 0) {
      if ((fMaxY - searchY) / minTy + searchZ < maxZ)
        maxZ = (fMaxY - searchY) / minTy + searchZ;
    } else if (maxTy < 0) {
      if ((fMinY - searchY) / maxTy + searchZ < maxZ)
        maxZ = (fMinY - searchY) / maxTy + searchZ;
    }

    if (minTx > 0) {
      if ((fMaxX - searchX) / minTx + searchZ < maxZ)
        maxZ = (fMaxX - searchX) / minTx + searchZ;
    } else if (maxTx < 0) {
      if ((fMinX - searchX) / maxTx + searchZ < maxZ)
        maxZ = (fMinX - searchX) / maxTx + searchZ;
    }

    int maxZind = GetZInd(maxZ);

    for (int i = 0; i <= maxZind; ++i) {
      CbmZBin& zBin  = fZBins[i];
      Double_t minZi = minZ + i * fZBinSize;
      Double_t maxZi = minZ + (i + 1) * fZBinSize;
      Double_t minY  = minTy > 0 ? minTy * (minZi - searchZ) + searchY
                                : minTy * (maxZi - searchZ) + searchY;
      int minYind   = GetYInd(minY);
      Double_t maxY = maxTy > 0 ? maxTy * (maxZi - searchZ) + searchY
                                : maxTy * (minZi - searchZ) + searchY;
      int maxYind = GetYInd(maxY);

      for (int j = minYind; j <= maxYind; ++j) {
        CbmYBin& yBin  = zBin[j];
        Double_t minZj = maxZi;
        Double_t maxZj = minZi;
        Double_t tmp   = GetYEnterZ(j, minTy);

        if (tmp < minZj && tmp > minZi) minZj = tmp;

        tmp = GetYEnterZ(j, maxTy);

        if (tmp < minZj && tmp > minZi) minZj = tmp;

        if (minZj == maxZi) minZj = minZi;

        tmp = GetYExitZ(j, minTy);

        if (tmp > maxZj && tmp < maxZi) maxZj = tmp;

        tmp = GetYExitZ(j, maxTy);

        if (tmp > maxZj && tmp < maxZi) maxZj = tmp;

        if (maxZj == minZi) maxZj = maxZi;

        Double_t minX = minTx > 0 ? minTx * (minZj - searchZ) + searchX
                                  : minTx * (maxZj - searchZ) + searchX;
        Double_t maxX = maxTx > 0 ? maxTx * (maxZj - searchZ) + searchX
                                  : maxTx * (minZj - searchZ) + searchX;
        int minXind = GetXInd(minX);
        int maxXind = GetXInd(maxX);

        for (int k = minXind; k <= maxXind; ++k) {
          CbmXBin& xBin  = yBin[k];
          Double_t minZk = maxZj;
          Double_t maxZk = minZj;
          Double_t tmp2  = GetXEnterZ(k, minTx);

          if (tmp2 < minZk && tmp2 > minZj) minZk = tmp2;

          tmp2 = GetXEnterZ(k, maxTx);

          if (tmp2 < minZk && tmp2 > minZj) minZk = tmp2;

          if (minZk == maxZj) minZk = minZj;

          tmp2 = GetXExitZ(k, minTx);

          if (tmp2 > maxZk && tmp2 < maxZj) maxZk = tmp2;

          tmp2 = GetXExitZ(k, maxTx);

          if (tmp2 > maxZk && tmp2 < maxZj) maxZk = tmp2;

          if (maxZk == minZj) maxZk = maxZj;

          //Double_t minT = searchT + (minZk - searchZ) * timeCoeff - wT; (VF) unused
          //Double_t maxT = searchT + (maxZk - searchZ) * timeCoeff + wT; (VF) unused
          int minTind = 0;              //GetTInd(minT);
          int maxTind = fNofTBins - 1;  //GetTInd(maxT);

          for (int l = minTind; l <= maxTind; ++l) {
            CbmTBin& tBin                                   = xBin[l];
            std::list<CbmTBin::HitHolder>::iterator hitIter = tBin.HitsBegin();
            std::list<CbmTBin::HitHolder>::iterator hitIterEnd = tBin.HitsEnd();

            for (; hitIter != hitIterEnd; ++hitIter) {
              const CbmPixelHit* hit = hitIter->hit;
              Double_t z             = hit->GetZ();
              Double_t zSq           = z * z;
              Double_t deltaY        = hit->GetY() - ty * z;

              if (deltaY * deltaY
                  > cbmBinnedSigmaSq
                      * (dTySq * zSq + hit->GetDy() * hit->GetDy()))
                continue;

              Double_t deltaX = hit->GetX() - tx * z;

              if (deltaX * deltaX
                  > cbmBinnedSigmaSq
                      * (dTxSq * zSq + hit->GetDx() * hit->GetDx()))
                continue;

              /*Double_t deltaT = hit->GetTime() - searchT - (z - searchZ) * timeCoeff;

                            if (deltaT * deltaT > cbmBinnedSigmaSq * (dtSq + hit->GetTimeError() * hit->GetTimeError()))
                                continue;*/

              Segment newSegment(segment.end, &(*hitIter));
              std::pair<std::set<Segment, SegmentComp>::iterator, bool> ir =
                fSegments.insert(newSegment);
              segment.children.push_back(const_cast<Segment*>(&(*ir.first)));
              //handleHit(*hitIter);
            }
          }
        }
      }
    }
  }

  void SearchHits(Double_t,
                  Double_t,
                  Double_t,
                  Double_t,
                  Double_t,
                  Double_t,
                  Double_t,
                  Double_t,
                  std::function<void(CbmTBin::HitHolder&)>) {}

private:
  CbmBinned4DStation(const CbmBinned4DStation&) = delete;
  CbmBinned4DStation& operator=(const CbmBinned4DStation&) = delete;

private:
  CbmZBin* fZBins;
  int fNofZBins;
  Double_t fZBinSize;
  Double_t fDtx;
  Double_t fDtxSq;
  Double_t fDty;
  Double_t fDtySq;
};

#endif /* STATION4D_H */
