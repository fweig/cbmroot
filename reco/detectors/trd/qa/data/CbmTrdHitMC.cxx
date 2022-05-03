/* Copyright (C) 2018-2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#include "CbmTrdHitMC.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVector3.h>

#include <sstream>

//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC() : CbmTrdHit() {}

//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC(const CbmTrdHit& h) : CbmTrdHit(h) {}

//_____________________________________________________________________
CbmTrdHitMC::~CbmTrdHitMC() {}

//_____________________________________________________________________
void CbmTrdHitMC::AddCluster(const CbmTrdCluster* c)
{
  fCluster = *c;
}

//_____________________________________________________________________
size_t CbmTrdHitMC::AddPoint(const CbmTrdPoint* p, double t, int id)
{
  fTrdPoints.push_back(std::make_tuple(CbmTrdPoint(*p), t, id));
  return fTrdPoints.size();
}

//_____________________________________________________________________
size_t CbmTrdHitMC::AddSignal(double s, int t)
{
  printf("hit%dD : s(%f) t(%d)\n", (GetClassType() ? 2 : 1), s, t);
  fTrdSignals.push_back(std::make_pair(s, t));
  return fTrdSignals.size();
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

  int pdg = std::get<2>(fTrdPoints[0]);
  double t0(std::get<1>(fTrdPoints[0])), dz(GetZ() - p->GetZ()), t(t0 + p->GetTime()), mass(0);

  TParticlePDG* pmc = (TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg);
  if (pdg < 9999999 && pmc) mass = pmc->Mass();
                       
  TVector3 mom3;
  p->Momentum(mom3);
  t += dz / (p->GetPz() * speedOfLight) * sqrt(mass * mass + mom3.Mag2());
  return GetTime() - t;
}

//_____________________________________________________________________
std::string CbmTrdHitMC::ToString() const
{ 
  std::stringstream ss;
  for (auto mcp : fTrdPoints) {
    ss << "Event time(ns)=" << std::get<1>(mcp) << " partId=" << std::get<2>(mcp) << "\n"; 
    ss << std::get<0>(mcp).ToString();
  }

  ss << "CbmTrdDigi: [" << fTrdSignals.size() << "] Signal / Relative Time\n           ";
  for (auto sgn : fTrdSignals) {
    ss << sgn.first << "/" << sgn.second << " ";
  }
  ss << "\n";

  ss << fCluster.ToString();

  ss << CbmTrdHit::ToString();

  if (fErrMsg != "") ss << "Error : " << fErrMsg << "\n";
  return ss.str();
}

ClassImp(CbmTrdHitMC)
