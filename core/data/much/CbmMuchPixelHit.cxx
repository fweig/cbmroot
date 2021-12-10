/* Copyright (C) 2009-2020 GSI/JINR-LIT, Darmstadt/Dubna
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

CbmMuchPixelHit::CbmMuchPixelHit(int32_t address, double x, double y, double z, double dx, double dy, double dz,
                                 double dxy, int32_t refId, int32_t planeId, double t, double dt)
  : CbmPixelHit(address, x, y, z, dx, dy, dz, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(t);
  SetTimeError(dt);
}

CbmMuchPixelHit::CbmMuchPixelHit(int32_t address, const TVector3& pos, const TVector3& err, double dxy, int32_t refId,
                                 int32_t planeId)
  : CbmPixelHit(address, pos, err, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(-1);
  SetTimeError(-1.);
}

CbmMuchPixelHit::CbmMuchPixelHit(int32_t address, const TVector3& pos, const TVector3& err, double dxy, int32_t refId,
                                 int32_t planeId, double /*time*/, double /*dtime*/)
  : CbmPixelHit(address, pos, err, dxy, refId)
  , fPlaneId(planeId)
  , fFlag(0)
{
  SetType(kMUCHPIXELHIT);
  SetTime(-1);
  SetTimeError(-1.);
}

ClassImp(CbmMuchPixelHit);
