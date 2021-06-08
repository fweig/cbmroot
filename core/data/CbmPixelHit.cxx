/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmPixelHit.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmPixelHit.h"

#include <TVector3.h>  // for TVector3

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::stringstream;

CbmPixelHit::CbmPixelHit() : CbmPixelHit(-1, 0., 0., 0., 0., 0., 0., 0., -1) {}

CbmPixelHit::CbmPixelHit(Int_t address, const TVector3& pos, const TVector3& err, Double_t dxy, Int_t refId,
                         Double_t time, Double_t timeError)
  : CbmPixelHit(address, pos.X(), pos.Y(), pos.Z(), err.X(), err.Y(), err.Z(), dxy, refId, time, timeError)
{
}

CbmPixelHit::CbmPixelHit(Int_t address, Double_t x, Double_t y, Double_t z, Double_t dx, Double_t dy, Double_t dz,
                         Double_t dxy, Int_t refId, Double_t time, Double_t timeError)
  : CbmHit(kPIXELHIT, z, dz, refId, address, time, timeError)
  , fX(x)
  , fY(y)
  , fDx(dx)
  , fDy(dy)
  , fDxy(dxy)
{
}

CbmPixelHit::~CbmPixelHit() {}

std::string CbmPixelHit::ToString() const
{
  stringstream ss;
  ss << "CbmPixelHit: address=" << GetAddress() << " pos=(" << GetX() << "," << GetY() << "," << GetZ() << ") err=("
     << GetDx() << "," << GetDy() << "," << GetDz() << ") dxy=" << GetDxy() << " refId=" << GetRefId() << endl;
  return ss.str();
}

void CbmPixelHit::Position(TVector3& pos) const { pos.SetXYZ(GetX(), GetY(), GetZ()); }

void CbmPixelHit::PositionError(TVector3& dpos) const { dpos.SetXYZ(GetDx(), GetDy(), GetDz()); }

void CbmPixelHit::SetPosition(const TVector3& pos)
{
  SetX(pos.X());
  SetY(pos.Y());
  SetZ(pos.Z());
}

void CbmPixelHit::SetPositionError(const TVector3& dpos)
{
  SetDx(dpos.X());
  SetDy(dpos.Y());
  SetDz(dpos.Z());
}

ClassImp(CbmPixelHit);
