/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBeam.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 **/

#include "CbmBeam.h"

#include <cassert>
#include <sstream>


// -----   Default constructor   --------------------------------------------
CbmBeam::CbmBeam(Double_t x, Double_t y, Double_t z, Double_t thetaX, Double_t thetaY)
  : fPosition(x, y, z)
  , fDirection(TMath::Tan(thetaX), TMath::Tan(thetaY), 1.)
{
}
// --------------------------------------------------------------------------


// -----   Extrapolation to a plane   ---------------------------------------
TVector3 CbmBeam::ExtrapolateToPlane(const TVector3& point, const TVector3& norm) const
{

  // The beam should not be parallel to the plane
  assert(norm * fDirection);

  // Calculate intersection point. Just some analytic geometry.
  Double_t numer = norm * (point - fPosition);
  Double_t denom = norm * fDirection;
  return fPosition + (numer / denom) * fDirection;
}
// --------------------------------------------------------------------------


// -----   Info   -----------------------------------------------------------
std::string CbmBeam::ToString() const
{

  std::stringstream ss;
  ss << "Current beam: position (" << fPosition.X() << ", " << fPosition.Y() << ", " << fPosition.Z() << ") cm, angle ("
     << fDirection.X() << ", " << fDirection.Y() << ") rad";

  return ss.str();
}
// --------------------------------------------------------------------------
