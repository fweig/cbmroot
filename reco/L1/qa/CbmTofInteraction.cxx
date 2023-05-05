/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmTofInteraction.h
/// \date   02.02.2023
/// \brief  Representation of MC track interaction with a TOF module
/// \author P.-A. Loizeau
/// \author S. Zharko


#include "CbmTofInteraction.h"

#include "Logger.h"

#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------
//
CbmTofInteraction::CbmTofInteraction() : CbmTofPoint(), fNofPoints(0) {};

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTofInteraction::AddPoint(const CbmTofPoint* pPoint)
{
  // ----- Update track and detector ID
  if (fNofPoints == 0) {
    fTrackID    = pPoint->GetTrackID();
    fDetectorID = pPoint->GetDetectorID();
  }
  else {
    // Track and detector IDs schold be equal for every point
    LOG_IF(fatal, pPoint->GetTrackID() != fTrackID) << "Attempt to add point with inconsistent track ID";
    LOG_IF(fatal, pPoint->GetDetectorID() != fDetectorID) << "Attempt to add point with inconsistent detector ID";
  }

  // ----- Update position and momenta
  UpdateAverage(pPoint->GetX(), fX);
  UpdateAverage(pPoint->GetY(), fY);
  UpdateAverage(pPoint->GetZ(), fZ);
  UpdateAverage(pPoint->GetTime(), fTime);

  UpdateAverage(pPoint->GetPx(), fPx);
  UpdateAverage(pPoint->GetPy(), fPy);
  UpdateAverage(pPoint->GetPz(), fPz);
  UpdateAverage(pPoint->GetLength(), fLength);  // Is it correct?
  fELoss += pPoint->GetEnergyLoss();            // Is it correct?
  fvpPoints.push_back(pPoint);
  fNofPoints++;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTofInteraction::Clear(Option_t*)
{
  fTrackID    = -1;
  fDetectorID = -1;
  fEventId    = 0;
  fPx         = 0.;
  fPy         = 0.;
  fPz         = 0.;
  fTime       = 0.;
  fLength     = 0.;
  fELoss      = 0.;
  fX          = 0.;
  fY          = 0.;
  fZ          = 0.;
  fNofPoints  = 0;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmTofInteraction::SetFromPoint(const CbmTofPoint* pPoint)
{
  // Check consistency of the point and the interaction
  LOG_IF(fatal, fTrackID != pPoint->GetTrackID() || fDetectorID != pPoint->GetDetectorID())
    << "CbmTofInteraction: attempt to add point with inconsistent track or detector IDs: track " << fTrackID << " vs. "
    << pPoint->GetTrackID() << ", sensor " << fDetectorID << " vs. " << pPoint->GetDetectorID();

  fX      = pPoint->GetX();
  fY      = pPoint->GetY();
  fZ      = pPoint->GetZ();
  fTime   = pPoint->GetTime();
  fPx     = pPoint->GetPx();
  fPy     = pPoint->GetPy();
  fPz     = pPoint->GetPz();
  fLength = pPoint->GetLength();
  fELoss  = pPoint->GetEnergyLoss();
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string CbmTofInteraction::ToString() const
{
  std::stringstream msg;
  msg << CbmTofPoint::ToString();
  msg << "    Number of points: " << fNofPoints;
  return msg.str();
}
