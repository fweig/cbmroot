/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Florian Uhlig [committer], Volker Friese */

/**
 * \file CbmHit.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 *
 * Former name CbmBaseHit. Renamed 11 May 2015.
 **/
#include "CbmHit.h"

#include "CbmMatch.h"  // for CbmMatch

#include <TObject.h>  // for TObject

CbmHit::CbmHit() : CbmHit(kHIT, 0., 0., -1, -1, -1., -1.) {}

CbmHit::CbmHit(HitType _type, Double_t _z, Double_t _dz, Int_t _refId, Int_t _address, Double_t _time,
               Double_t _timeError)
  : TObject()
  , fType(_type)
  , fZ(_z)
  , fDz(_dz)
  , fRefId(_refId)
  , fAddress(_address)
  , fTime(_time)
  , fTimeError(_timeError)
  , fMatch(nullptr)
{
}

// Only shallow copy needed
CbmHit::CbmHit(const CbmHit& rhs)
  : TObject(rhs)
  , fType(rhs.fType)
  , fZ(rhs.fZ)
  , fDz(rhs.fDz)
  , fRefId(rhs.fRefId)
  , fAddress(rhs.fAddress)
  , fTime(rhs.fTime)
  , fTimeError(rhs.fTimeError)
  , fMatch(nullptr)
{
}

// Only shallow copy needed
CbmHit& CbmHit::operator=(const CbmHit& rhs)
{

  if (this != &rhs) {
    TObject::operator=(rhs);
    fType            = rhs.fType;
    fZ               = rhs.fZ;
    fDz              = rhs.fDz;
    fRefId           = rhs.fRefId;
    fAddress         = rhs.fAddress;
    fTime            = rhs.fTime;
    fTimeError       = rhs.fTimeError;
    fMatch           = nullptr;
  }
  return *this;
}

CbmHit::~CbmHit()
{
  if (fMatch) delete fMatch;
}

void CbmHit::SetMatch(CbmMatch* match)
{
  if (fMatch) delete fMatch;
  fMatch = match;
}

ClassImp(CbmHit);
