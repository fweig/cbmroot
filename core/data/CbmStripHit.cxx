/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmStripHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmStripHit.h"

#include <TVector3.h>  // for TVector3

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::stringstream;


CbmStripHit::CbmStripHit() : CbmStripHit(-1, 0., 0., 0., 0., 0., 0., -1) {}

CbmStripHit::CbmStripHit(int32_t address, const TVector3& pos, const TVector3& err, int32_t refId, double time,
                         double timeError)
  : CbmStripHit(address, pos.X(), pos.Y(), pos.Z(), err.X(), err.Y(), err.Z(), refId, time, timeError)
{
}


CbmStripHit::CbmStripHit(int32_t address, double u, double phi, double z, double du, double dphi, double dz,
                         int32_t refId, double time, double timeError)
  : CbmHit(kSTRIPHIT, z, dz, refId, address, time, timeError)
  , fU(u)
  , fDu(du)
  , fPhi(phi)
  , fDphi(dphi)
{
}

CbmStripHit::~CbmStripHit() {}

std::string CbmStripHit::ToString() const
{
  stringstream ss;
  ss << "CbmStripHit: address=" << GetAddress() << " pos=(" << GetU() << "," << GetPhi() << "," << GetZ() << ") err=("
     << GetDu() << "," << GetDphi() << "," << GetDz() << ") refId=" << GetRefId() << endl;
  return ss.str();
}

ClassImp(CbmStripHit);
