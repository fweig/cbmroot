/* Copyright (C) 2009-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig, Mikhail Ryzhinskiy */

/**
 * \file CbmMuchPixelHit.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmMuchPixelHit.h"

#include "CbmHit.h"  // for kMUCHPIXELHIT

#include <TVector3.h>  // for TVector3

CbmMuchPixelHit::CbmMuchPixelHit() : CbmPixelHit(), fPlaneId(-1), fFlag(0) { SetType(kMUCHPIXELHIT); }

CbmMuchPixelHit::~CbmMuchPixelHit() {}

CbmMuchPixelHit::CbmMuchPixelHit(Int_t address, Double_t x, Double_t y, Double_t z, Double_t dx, Double_t dy,
                                 Double_t dz, Double_t dxy, Int_t refId, Int_t planeId, Double_t t, Double_t dt)
  : CbmPixelHit(address, x, y, z, dx, dy, dz, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(t);
  SetTimeError(dt);
}

CbmMuchPixelHit::CbmMuchPixelHit(Int_t address, const TVector3& pos, const TVector3& err, Double_t dxy, Int_t refId,
                                 Int_t planeId)
  : CbmPixelHit(address, pos, err, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(-1);
  SetTimeError(-1.);
}

CbmMuchPixelHit::CbmMuchPixelHit(Int_t address, const TVector3& pos, const TVector3& err, Double_t dxy, Int_t refId,
                                 Int_t planeId, Double_t /*time*/, Double_t /*dtime*/)
  : CbmPixelHit(address, pos, err, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(-1);
  SetTimeError(-1.);
}

ClassImp(CbmMuchPixelHit);
