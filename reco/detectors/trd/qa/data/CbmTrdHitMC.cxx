/* Copyright (C) 2018-2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#include "CbmTrdHitMC.h"

#include <TVector3.h>

//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC() : CbmTrdHit() {}

//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC(const CbmTrdHit& h) : CbmTrdHit(h) {}

//_____________________________________________________________________
CbmTrdHitMC::~CbmTrdHitMC() {}

//_____________________________________________________________________
size_t CbmTrdHitMC::AddPoint(const CbmTrdPoint* p, double t, double m)
{
  fTrdPoints.push_back(std::make_tuple(CbmTrdPoint(*p), t, m));
  return fTrdPoints.size();
}

//_____________________________________________________________________
const CbmTrdPoint* CbmTrdHitMC::GetPoint(uint idx) const
{
  if (idx >= fTrdPoints.size()) return nullptr;
  return &std::get<0>(fTrdPoints[idx]);
}

//_____________________________________________________________________
double CbmTrdHitMC::GetDx() const
{
  const CbmTrdPoint* p(nullptr);
  if (!(p = GetPoint())) return -999;
  double dz(GetZ() - p->GetZ()), x(p->GetX() + dz * p->GetPx() / p->GetPz());
  return GetX() - x;
}

//_____________________________________________________________________
double CbmTrdHitMC::GetDy() const
{
  const CbmTrdPoint* p(nullptr);
  if (!(p = GetPoint())) return -999;
  double dz(GetZ() - p->GetZ()), y(p->GetY() + dz * p->GetPy() / p->GetPz());
  return GetY() - y;
}

//_____________________________________________________________________
double CbmTrdHitMC::GetDt() const
{
  constexpr double speedOfLight = 29.979246;  // cm/ns
  const CbmTrdPoint* p(nullptr);
  if (!(p = GetPoint())) return -999;

  double t0(std::get<1>(fTrdPoints[0])), mass(std::get<2>(fTrdPoints[0]));
  double dz(GetZ() - p->GetZ()), t(t0 + p->GetTime());

  TVector3 mom3;
  p->Momentum(mom3);
  t += dz / (p->GetPz() * speedOfLight) * sqrt(mass * mass + mom3.Mag2());
  return GetTime() - t;
}

ClassImp(CbmTrdHitMC)
