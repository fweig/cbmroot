/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                      CbmStsPoint source file                  -----
// -----                  Created 26/07/04  by V. Friese               -----
// -------------------------------------------------------------------------

#include "CbmStsPoint.h"

#include "CbmDefs.h"  // for kMCTrack

#include <FairLink.h>     // for FairLink
#include <FairMCPoint.h>  // for FairMCPoint

#include <sstream>  // for operator<<, basic_ostream, endl, stri...
#include <string>   // for char_traits

#include <cmath>

using std::endl;
using std::string;
using std::stringstream;

// -----   Default constructor   -------------------------------------------
CbmStsPoint::CbmStsPoint()
  : FairMCPoint()
  , fX_out(0.)
  , fY_out(0.)
  , fZ_out(0.)
  , fPx_out(0.)
  , fPy_out(0.)
  , fPz_out(0.)
  , fPid(0)
  , fIndex(0)
  , fFlag(0)
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmStsPoint::CbmStsPoint(int32_t trackID, int32_t detID, TVector3 posIn, TVector3 posOut, TVector3 momIn,
                         TVector3 momOut, double tof, double length, double eLoss, int32_t pid, int32_t eventId,
                         int32_t index, int16_t flag)
  : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss, eventId)
  , fX_out(posOut.X())
  , fY_out(posOut.Y())
  , fZ_out(posOut.Z())
  , fPx_out(momOut.Px())
  , fPy_out(momOut.Py())
  , fPz_out(momOut.Pz())
  , fPid(pid)
  , fIndex(index)
  , fFlag(flag)
{
  SetLink(FairLink(ToIntegralType(ECbmDataType::kMCTrack), trackID));
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsPoint::~CbmStsPoint() {}
// -------------------------------------------------------------------------


// -----   Copy constructor with event and epoch time   --------------------
CbmStsPoint::CbmStsPoint(const CbmStsPoint& point, int32_t eventId, double eventTime, double epochTime)
  : FairMCPoint(point)
  , fX_out(point.fX_out)
  , fY_out(point.fY_out)
  , fZ_out(point.fZ_out)
  , fPx_out(point.fPx_out)
  , fPy_out(point.fPy_out)
  , fPz_out(point.fPz_out)
  , fPid(point.fPid)
  , fIndex(point.fIndex)
  , fFlag(point.fFlag)

{
  //  *this = point;
  if (eventId > 0) fEventId = eventId;
  fTime = point.GetTime() + eventTime - epochTime;
}
// -------------------------------------------------------------------------


// -----   Point x coordinate from linear extrapolation   ------------------
double CbmStsPoint::GetX(double z) const
{
  //  LOG(info) << fZ << " " << z << " " << fZ_out;
  double dz = fZ_out - fZ;
  if (fabs(dz) < 1.e-4) return 0.5 * (fX_out + fX);
  return (fX + (z - fZ) / dz * (fX_out - fX));
}
// -------------------------------------------------------------------------


// -----   Point y coordinate from linear extrapolation   ------------------
double CbmStsPoint::GetY(double z) const
{
  double dz = fZ_out - fZ;
  if (fabs(dz) < 1.e-4) return 0.5 * (fY_out + fY);
  return (fY + (z - fZ) / dz * (fY_out - fY));
}
// -------------------------------------------------------------------------


// -----   Public method IsUsable   ----------------------------------------
bool CbmStsPoint::IsUsable() const
{
  double dz = fZ_out - fZ;
  if (abs(dz) < 1.e-4) return false;
  return true;
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsPoint::ToString() const
{
  stringstream ss;
  ss << "StsPoint: track ID " << fTrackID << ", detector ID " << fDetectorID << endl;
  ss << "          IN  Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  ss << "          OUT Position (" << fX_out << ", " << fY_out << ", " << fZ_out << ") cm" << endl;
  ss << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
  ss << "    Time " << fTime << " ns,  Length " << fLength << " cm,  Energy loss " << fELoss * 1.0e06 << " keV" << endl;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmStsPoint)
