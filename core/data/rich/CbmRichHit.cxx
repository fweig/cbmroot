/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Boris Polichtchouk, Florian Uhlig, Andrey Lebedev, Denis Bertini [committer] */

/**
 * \file CbmRichHit.cxx
 * \author B. Polichtchouk
 **/
#include "CbmRichHit.h"

#include "CbmHit.h"  // for kRICHHIT

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::endl;
using std::stringstream;

CbmRichHit::CbmRichHit()
  : CbmPixelHit()
  , fPmtId(0)
  ,
  // fNPhotons(0),
  // fAmplitude(0.),
  fToT(0.)
{
  SetType(kRICHHIT);
  SetTime(0.);
}

CbmRichHit::CbmRichHit(double x, double y)
  : CbmPixelHit()
  , fPmtId(0)
  ,
  //  fNPhotons(0),
  //  fAmplitude(0.),
  fToT(0.)
{
  SetType(kRICHHIT);
  SetX(x);
  SetY(y);
  SetTime(0.);
}

CbmRichHit::CbmRichHit(double x, double y, double ts, double tot)
  : CbmPixelHit()
  , fPmtId(0)
  ,
  //    fNPhotons(0),
  //    fAmplitude(0.),
  fToT(tot)
{
  SetType(kRICHHIT);
  SetX(x);
  SetY(y);
  SetTime(ts);
}

CbmRichHit::~CbmRichHit() {}

std::string CbmRichHit::ToString() const
{
  stringstream ss;
  ss << "CbmRichHit: address=" << GetAddress() << " pos=(" << GetX() << "," << GetY() << "," << GetZ() << ") err=("
     << GetDx() << "," << GetDy() << "," << GetDz() << ") dxy=" << GetDxy() << " refId="
     << GetRefId()
     //  << " pmtId=" << GetPmtId() << " nofPhotons=" << GetNPhotons()
     //  << " amplitude=" << GetAmplitude()
     << endl;
  return ss.str();
}

ClassImp(CbmRichHit)
