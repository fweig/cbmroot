/* Copyright (C) 2017-2019 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Station3D.h"

#include "Tracker.h"

#include <set>

void CbmBinned3DStation::SearchHits(const CbmTrackParam2& stateVec, Double_t,
                                    std::function<void(CbmTBin::HitHolder&)> handleHit)
{
  CbmTrackParam2 minParams = Extrapolate(stateVec, fMinZ);
  //Double_t minC[21];
  //minParams.CovMatrix(minC);
  CbmTrackParam2 maxParams = Extrapolate(stateVec, fMaxZ);
  //Double_t maxC[21];
  //maxParams.CovMatrix(maxC);
  Double_t wXmin = fNofSigmasX * std::sqrt(minParams.GetCovXX() + fDxSq + fScatXSq);
  Double_t wXmax = fNofSigmasX * std::sqrt(maxParams.GetCovXX() + fDxSq + fScatXSq);
  Double_t wYmin = fNofSigmasY * std::sqrt(minParams.GetCovYY() + fDySq + fScatYSq);
  Double_t wYmax = fNofSigmasY * std::sqrt(maxParams.GetCovYY() + fDySq + fScatYSq);

  Double_t xMin = stateVec.GetTx() > 0 ? minParams.GetX() - wXmin : maxParams.GetX() - wXmax;
  Double_t xMax = stateVec.GetTx() > 0 ? maxParams.GetX() + wXmax : minParams.GetX() + wXmin;

  Double_t yMin = stateVec.GetTy() > 0 ? minParams.GetY() - wYmin : maxParams.GetY() - wYmax;
  Double_t yMax = stateVec.GetTy() > 0 ? maxParams.GetY() + wYmax : minParams.GetY() + wYmin;

  int lowerXind = GetXInd(xMin);
  int upperXind = GetXInd(xMax);
  int lowerYind = GetYInd(yMin);
  int upperYind = GetYInd(yMax);
  int lowerTind = GetTInd(0);
  int upperTind = GetTInd(0);

  for (int i = lowerYind; i <= upperYind; ++i) {
    CbmYBin& yBin = fYBins[i];

    for (int j = lowerXind; j <= upperXind; ++j) {
      CbmXBin& xBin = yBin[j];

      for (int k = lowerTind; k <= upperTind; ++k) {
        CbmTBin& tBin                                      = xBin[k];
        std::list<CbmTBin::HitHolder>::iterator hitIter    = tBin.HitsBegin();
        std::list<CbmTBin::HitHolder>::iterator hitIterEnd = tBin.HitsEnd();

        for (; hitIter != hitIterEnd; ++hitIter) {
          CbmTBin::HitHolder& hitHolder = *hitIter;

          if (fStage > hitHolder.stage) continue;

          handleHit(hitHolder);
        }
      }
    }
  }
}

void CbmBinned3DStation::SearchHits(Segment& segment, std::function<void(CbmTBin::HitHolder&)>)
{
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
  Double_t segDeltaZ      = hit2->GetZ() - hit1->GetZ();
  Double_t tx             = (x2 - x1) / segDeltaZ;
  Double_t ty             = (y2 - y1) / segDeltaZ;
  Double_t middleZ        = (hit1->GetZ() + hit2->GetZ()) / 2;
  Double_t deltaZmin      = fMinZ - middleZ;
  Double_t deltaZmax      = fMaxZ - middleZ;
  Double_t coeff1Min      = (0.5 - deltaZmin / segDeltaZ);
  Double_t coeff1MinSq    = coeff1Min * coeff1Min;
  Double_t coeff1Max      = (0.5 - deltaZmax / segDeltaZ);
  Double_t coeff1MaxSq    = coeff1Max * coeff1Max;
  Double_t coeff2Min      = (0.5 + deltaZmin / segDeltaZ);
  Double_t coeff2MinSq    = coeff2Min * coeff2Min;
  Double_t coeff2Max      = (0.5 + deltaZmax / segDeltaZ);
  Double_t coeff2MaxSq    = coeff2Max * coeff2Max;
  Double_t tCoeff         = std::sqrt(1 + tx * tx + ty * ty) / cbmBinnedSOL;

  Double_t searchT;
  Double_t dtSq;
  Double_t wT;
  Double_t tMin;
  Double_t tMax;

  if (0 == hit1->GetTimeError()) {
    searchT = hit2->GetTime();
    dtSq    = hit2->GetTimeError() * hit2->GetTimeError();
    wT      = cbmBinnedSigma * std::sqrt(dtSq + fDtSq);
    tMin    = searchT + tCoeff * (fMinZ - hit2->GetZ()) - wT;
    tMax    = searchT + tCoeff * (fMaxZ - hit2->GetZ()) + wT;
  }
  else {
    searchT = (hit1->GetTime() + hit2->GetTime()) / 2;
    dtSq    = (hit1->GetTimeError() * hit1->GetTimeError() + hit2->GetTimeError() * hit2->GetTimeError()) / 2;
    wT      = cbmBinnedSigma * std::sqrt(dtSq + fDtSq);
    tMin    = searchT + tCoeff * deltaZmin - wT;
    tMax    = searchT + tCoeff * deltaZmax + wT;
  }

  Double_t wXmin = fNofSigmasX * std::sqrt(coeff1MinSq * dx1Sq + coeff2MinSq * dx2Sq + fDxSq + fScatXSq);
  Double_t wXmax = fNofSigmasX * std::sqrt(coeff1MaxSq * dx1Sq + coeff2MaxSq * dx2Sq + fDxSq + fScatXSq);
  Double_t wYmin = fNofSigmasY * std::sqrt(coeff1MinSq * dy1Sq + coeff2MinSq * dy2Sq + fDySq + fScatYSq);
  Double_t wYmax = fNofSigmasY * std::sqrt(coeff1MaxSq * dy1Sq + coeff2MaxSq * dy2Sq + fDySq + fScatYSq);

  Double_t xMin = tx > 0 ? coeff1Min * x1 + coeff2Min * x2 - wXmin : coeff1Max * x1 + coeff2Max * x2 - wXmax;
  Double_t xMax = tx > 0 ? coeff1Max * x1 + coeff2Max * x2 + wXmax : coeff1Min * x1 + coeff2Min * x2 + wXmin;

  Double_t yMin = ty > 0 ? coeff1Min * y1 + coeff2Min * y2 - wYmin : coeff1Max * y1 + coeff2Max * y2 - wYmax;
  Double_t yMax = ty > 0 ? coeff1Max * y1 + coeff2Max * y2 + wYmax : coeff1Min * y1 + coeff2Min * y2 + wYmin;

  int lowerXind = GetXInd(xMin);
  int upperXind = GetXInd(xMax);
  int lowerYind = GetYInd(yMin);
  int upperYind = GetYInd(yMax);
  int lowerTind = GetTInd(tMin);
  int upperTind = GetTInd(tMax);

  for (int i = lowerYind; i <= upperYind; ++i) {
    CbmYBin& yBin = fYBins[i];

    for (int j = lowerXind; j <= upperXind; ++j) {
      CbmXBin& xBin = yBin[j];

      for (int k = lowerTind; k <= upperTind; ++k) {
        CbmTBin& tBin                                      = xBin[k];
        std::list<CbmTBin::HitHolder>::iterator hitIter    = tBin.HitsBegin();
        std::list<CbmTBin::HitHolder>::iterator hitIterEnd = tBin.HitsEnd();

        for (; hitIter != hitIterEnd; ++hitIter) {
          const CbmPixelHit* hit = hitIter->hit;
          Double_t deltaZ        = hit->GetZ() - middleZ;
          Double_t coeff1        = (0.5 - deltaZ / segDeltaZ);
          Double_t coeff1Sq      = coeff1 * coeff1;
          Double_t coeff2        = (0.5 + deltaZ / segDeltaZ);
          Double_t coeff2Sq      = coeff2 * coeff2;
          Double_t y             = coeff1 * y1 + coeff2 * y2;
          Double_t deltaY        = hit->GetY() - y;

          if (deltaY * deltaY
              > fNofSigmasYSq * (coeff1Sq * dy1Sq + coeff2Sq * dy2Sq + hit->GetDy() * hit->GetDy() + fScatYSq))
            continue;

          Double_t x      = coeff1 * x1 + coeff2 * x2;
          Double_t deltaX = hit->GetX() - x;

          if (deltaX * deltaX
              > fNofSigmasXSq * (coeff1Sq * dx1Sq + coeff2Sq * dx2Sq + hit->GetDx() * hit->GetDx() + fScatXSq))
            continue;

          /*Double_t t = 0 == hit1->GetTimeError() ? searchT + tCoeff * (hit->GetZ() - hit2->GetZ()) : searchT + tCoeff * deltaZ;
               Double_t deltaT = hit->GetTime() - t;

               if (deltaT * deltaT > cbmBinnedSigmaSq * (dtSq + hit->GetTimeError() * hit->GetTimeError()))
                  continue;*/

          Segment newSegment(segment.end, &(*hitIter));
          pair<set<Segment, SegmentComp>::iterator, bool> ir = fSegments.insert(newSegment);
          segment.children.push_back(const_cast<Segment*>(&(*ir.first)));
          //handleHit(*hitIter);
        }
      }
    }
  }
}

void CbmBinned3DStation::SearchHits(Double_t, Double_t, Double_t minY, Double_t maxY, Double_t minX, Double_t maxX,
                                    Double_t minT, Double_t maxT, std::function<void(CbmTBin::HitHolder&)> handleHit)
{
  int lowerXind = GetXInd(minX);
  int upperXind = GetXInd(maxX);
  int lowerYind = GetYInd(minY);
  int upperYind = GetYInd(maxY);
  int lowerTind = GetTInd(minT);
  int upperTind = GetTInd(maxT);

  for (int i = lowerYind; i <= upperYind; ++i) {
    CbmYBin& yBin = fYBins[i];

    for (int j = lowerXind; j <= upperXind; ++j) {
      CbmXBin& xBin = yBin[j];

      for (int k = lowerTind; k <= upperTind; ++k) {
        CbmTBin& tBin                                      = xBin[k];
        std::list<CbmTBin::HitHolder>::iterator hitIter    = tBin.HitsBegin();
        std::list<CbmTBin::HitHolder>::iterator hitIterEnd = tBin.HitsEnd();

        for (; hitIter != hitIterEnd; ++hitIter) {
          const CbmPixelHit* hit = hitIter->hit;
          Double_t y             = hit->GetY();

          if (y < minY || y >= maxY) continue;

          Double_t x = hit->GetX();

          if (x < minX || x >= maxX) continue;

          Double_t t = hit->GetTime();

          if (t < minT || t >= maxT) continue;

          handleHit(*hitIter);
        }
      }
    }
  }
}
