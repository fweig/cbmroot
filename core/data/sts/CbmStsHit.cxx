/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig, Andrey Lebedev */

/**
 ** \file CbmStsHit.cxx
 ** \author Volker Friese <v.friese@gsi.de>
 ** \since 30.08.06
 **
 ** Updated 14/03/2014 by Andrey Lebedev <andrey.lebedev@gsi.de>.
 ** Updated 10/06/2015 by Volker Friese <v.friese@gsi.de>.
 **/

#include "CbmStsHit.h"

#include <TVector3.h>  // for TVector3

#include <iomanip>  // for operator<<, setprecision
#include <sstream>  // for operator<<, basic_ostream, char_traits

using namespace std;

// -----   Default constructor
CbmStsHit::CbmStsHit() : CbmPixelHit(), fFrontClusterId(-1), fBackClusterId(-1), fDu(-1.), fDv(-1.)
{
  SetTime(-1.);
  SetTimeError(-1.);
}


// -----   Constructor with parameters
CbmStsHit::CbmStsHit(int32_t address, const TVector3& pos, const TVector3& dpos, double dxy, int32_t frontClusterId,
                     int32_t backClusterId, double time, double timeError, double du, double dv)
  : CbmPixelHit(address, pos, dpos, dxy, -1)
  , fFrontClusterId(frontClusterId)
  , fBackClusterId(backClusterId)
  , fDu(du)
  , fDv(dv)
{
  SetTime(time);
  SetTimeError(timeError);
}


// -----   Destructor
CbmStsHit::~CbmStsHit() {}


// --- String output
string CbmStsHit::ToString() const
{
  stringstream ss;
  ss << "StsHit: address " << GetAddress() << " | time " << GetTime() << " +- " << GetTimeError() << " | Position ("
     << std::setprecision(6) << GetX() << ", " << GetY() << ", " << GetZ() << ") cm | Error (" << GetDx() << ", "
     << GetDy() << ", " << GetDz() << ") cm | Cluster (" << fFrontClusterId << ", " << fBackClusterId << ")";
  return ss.str();
}


ClassImp(CbmStsHit)
