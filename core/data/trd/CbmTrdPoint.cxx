/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                      CbmTrdPoint source file                  -----
// -----                   Created 27/07/04  by V. Friese              -----
// -------------------------------------------------------------------------

#include "CbmTrdPoint.h"

#include <FairMCPoint.h>  // for FairMCPoint
#include <Logger.h>       // for Logger, LOG

#include <TVector3.h>  // for TVector3

// -----   Default constructor   -------------------------------------------
CbmTrdPoint::CbmTrdPoint() : FairMCPoint(), fX_out(0.), fY_out(0.), fZ_out(0.), fPx_out(0.), fPy_out(0.), fPz_out(0.) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrdPoint::CbmTrdPoint(int32_t trackID, int32_t detID, const TVector3& posIn, const TVector3& momIn,
                         const TVector3& posOut, const TVector3& momOut, double tof, double length, double eLoss)
  : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss)
  , fX_out(posOut.X())
  , fY_out(posOut.Y())
  , fZ_out(posOut.Z())
  , fPx_out(momOut.Px())
  , fPy_out(momOut.Py())
  , fPz_out(momOut.Pz())
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrdPoint::~CbmTrdPoint() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmTrdPoint::Print(const Option_t* /*opt*/) const
{
  LOG(info) << "TRD point for track " << fTrackID << " in detector " << fDetectorID;
  LOG(info) << "    Position In (" << fX << ", " << fY << ", " << fZ << ") cm";
  LOG(info) << "    Momentum In (" << fPx << ", " << fPy << ", " << fPz << ") GeV";
  LOG(info) << "    Position Out (" << fX_out << ", " << fY_out << ", " << fZ_out << ") cm";
  LOG(info) << "    Momentum Out (" << fPx_out << ", " << fPy_out << ", " << fPz_out << ") GeV";
  LOG(info) << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV";
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
std::string CbmTrdPoint::ToString() const
{
  std::stringstream ss;
  ss << "CbmTrdPoint: Track " << fTrackID << " Detector " << fDetectorID << "\n";
  ss << "    Position In (" << fX << ", " << fY << ", " << fZ << ") cm" << "\n";
  ss << "    Momentum In (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << "\n";
  ss << "    Position Out (" << fX_out << ", " << fY_out << ", " << fZ_out << ") cm" << "\n";
  ss << "    Momentum Out (" << fPx_out << ", " << fPy_out << ", " << fPz_out << ") GeV" << "\n";
  ss << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV" << "\n";
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmTrdPoint)
