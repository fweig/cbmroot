/* Copyright (C) 2018-2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#include "CbmTrdHitMC.h"

#include "CbmTrdDigi.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVector3.h>

#include <sstream>

// Error parametrization
int CbmTrdHitMC::fSx[5][2] = {{190, 2321}, {147, 1920}, {50, 1461}, {22, 2094}, {21, 4297}};
//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC() : CbmTrdHit() {}

//_____________________________________________________________________
CbmTrdHitMC::CbmTrdHitMC(const CbmTrdHit& h) : CbmTrdHit(h) {}

//_____________________________________________________________________
CbmTrdHitMC::~CbmTrdHitMC() {}

//_____________________________________________________________________
void CbmTrdHitMC::AddCluster(const CbmTrdCluster* c) { fCluster = *c; }

//_____________________________________________________________________
size_t CbmTrdHitMC::AddPoint(const CbmTrdPoint* p, double t, int id)
{
  fTrdPoints.push_back(std::make_tuple(CbmTrdPoint(*p), t, id));
  return fTrdPoints.size();
}

//_____________________________________________________________________
size_t CbmTrdHitMC::AddSignal(const CbmTrdDigi* digi, uint64_t t0)
{
  if (GetClassType() == 1) {  // TRD2D type
    int dt;
    double t, r = digi->GetCharge(t, dt);
    fTrdSignals.push_back(std::make_pair(t, digi->GetTimeDAQ() - t0));
    fTrdSignals.push_back(std::make_pair(r, digi->GetTimeDAQ() + dt - t0));
  }
  else  // TRD1D type
    fTrdSignals.push_back(std::make_pair(digi->GetCharge(), digi->GetTime() - t0));

  return fTrdSignals.size();
}

//_____________________________________________________________________
size_t CbmTrdHitMC::PurgeSignals()
{
  if (!GetNSignals()) return 0;
  if (fTrdSignals.front().first < 1.e-3) fTrdSignals.erase(fTrdSignals.begin());
  if (fTrdSignals.back().first < 1.e-3) fTrdSignals.pop_back();

  return fTrdSignals.size();
}

//_____________________________________________________________________
const CbmTrdPoint* CbmTrdHitMC::GetPoint(uint idx) const
{
  if (idx >= fTrdPoints.size()) return nullptr;
  return &std::get<0>(fTrdPoints[idx]);
}

//_____________________________________________________________________
double CbmTrdHitMC::GetSignal(uint idx) const
{
  if (idx >= fTrdSignals.size()) return 0;
  return fTrdSignals[idx].first;
}

//_____________________________________________________________________
CbmTrdHitMC::eCbmTrdHitMCshape CbmTrdHitMC::GetClShape() const
{
  if (fCluster.HasOpenStart()) {
    if (fCluster.HasOpenStop()) return eCbmTrdHitMCshape::kRT;
    else
      return eCbmTrdHitMCshape::kRR;
  }
  else {
    if (fCluster.HasOpenStop()) return eCbmTrdHitMCshape::kTT;
    else
      return eCbmTrdHitMCshape::kTR;
  }
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
double CbmTrdHitMC::GetSx() const
{
  const CbmTrdPoint* p(nullptr);
  if (!(p = GetPoint())) return 1;
  double phi(p->GetPx() / p->GetPz());
  int isz = GetNSignals() - 2;

  if (isz < 0) isz = 0;
  if (isz >= 5) isz = 4;

  return 1.e-4 * (fSx[isz][0] + phi * phi * fSx[isz][1]);  // error in cm
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
double CbmTrdHitMC::GetSy() const { return GetDy(); }

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
