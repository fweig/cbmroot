/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                     CbmMvdStationPar source file              -----
// -----                  Created 28/10/14  by P.Sitzmann              -----
// -------------------------------------------------------------------------

#include "CbmMvdStationPar.h"

#include <Logger.h>  // for Logger, LOG

#include <assert.h>  // for assert
#include <cmath>    // for fabs, isnan
#include <limits>    // for numeric_limits


// -----   Default constructor   -------------------------------------------
CbmMvdStationPar::CbmMvdStationPar() : TNamed() {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdStationPar::~CbmMvdStationPar() {}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void CbmMvdStationPar::Print(Option_t* /*opt*/) const
{
  LOG(info) << "MvdStationPar: Initialized parameter file with " << fStationCount << " stations";

  for (int i = 0; i < fStationCount; i++) {
    LOG(debug) << "Z Postion station " << i << ": " << GetZPosition(i);
  }

  for (int i = 0; i < fStationCount; i++) {
    LOG(debug) << "Z Thickness station " << i << ": " << GetZThickness(i);
  }

  for (int i = 0; i < fStationCount; i++) {
    LOG(debug) << "Width station " << i << ": " << GetWidth(i);
  }

  for (int i = 0; i < fStationCount; i++) {
    LOG(debug) << "Height station " << i << ": " << GetHeight(i);
  }

  for (int i = 0; i < fStationCount; i++) {
    LOG(debug) << "Z Radiation Thickness station " << i << ": " << GetZRadThickness(i);
  }
}
// -------------------------------------------------------------------------

void CbmMvdStationPar::Init(Int_t nrOfStations)
{
  fStationCount = nrOfStations;

  constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  // resize the arrays and set all initial values to NaN
  // to ensure that they will be initialized later

  fZPositions.resize(fStationCount, kNaN);
  fZPositionMin.resize(fStationCount, std::numeric_limits<float>::max());
  fZPositionMax.resize(fStationCount, -std::numeric_limits<float>::max());
  fZThicknesses.resize(fStationCount, kNaN);
  fHeights.resize(fStationCount, 0.);
  fWidths.resize(fStationCount, 0.);
  fXResolutions.resize(fStationCount, kNaN);
  fYResolutions.resize(fStationCount, kNaN);
  fZRadThickness.resize(fStationCount, kNaN);
  fBeamHeights.resize(fStationCount, kNaN);
  fBeamWidths.resize(fStationCount, kNaN);
}

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetParameter(const std::vector<Double_t>& parArray, Int_t iStation) const
{
  // return a parameter after out-of-range check
  if ((iStation < 0) || (iStation >= fStationCount)) {
    LOG(error) << "Station number out of Range ";
    return 0.;
  }
  return parArray.at(iStation);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetParameterMax(std::vector<Double_t>& parArray, Int_t iStation, Double_t value)
{
  // add a parameter after out-of-range check
  value = fabs(value);
  if ((iStation < 0) || (iStation >= fStationCount)) { LOG(error) << "Station number out of Range "; }
  else {
    Double_t& v = parArray[iStation];
    if (std::isnan(v) || (v < value)) { v = value; }
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetParameterMin(std::vector<Double_t>& parArray, Int_t iStation, Double_t value)
{
  // add a parameter after out-of-range check
  value = fabs(value);
  if ((iStation < 0) || (iStation >= fStationCount)) { LOG(error) << "Station number out of Range "; }
  else {
    Double_t& v = parArray[iStation];
    if (std::isnan(v) || (v > value)) { v = value; }
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------

Double_t CbmMvdStationPar::GetZPosition(Int_t iStation) const { return GetParameter(fZPositions, iStation); }

Double_t CbmMvdStationPar::GetZThickness(Int_t iStation) const { return GetParameter(fZThicknesses, iStation); }

Double_t CbmMvdStationPar::GetHeight(Int_t iStation) const { return GetParameter(fHeights, iStation); }

Double_t CbmMvdStationPar::GetWidth(Int_t iStation) const { return GetParameter(fWidths, iStation); }

Double_t CbmMvdStationPar::GetXRes(Int_t iStation) const { return GetParameter(fXResolutions, iStation); }

Double_t CbmMvdStationPar::GetYRes(Int_t iStation) const { return GetParameter(fYResolutions, iStation); }

Double_t CbmMvdStationPar::GetZRadThickness(Int_t iStation) const { return GetParameter(fZRadThickness, iStation); }

Double_t CbmMvdStationPar::GetBeamHeight(Int_t iStation) const { return GetParameter(fBeamHeights, iStation); }

Double_t CbmMvdStationPar::GetBeamWidth(Int_t iStation) const { return GetParameter(fBeamWidths, iStation); }

// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdStationPar::AddZPosition(Int_t iStation, Double_t z, Double_t zThickness)
{
  Double_t& zMin = fZPositionMin[iStation];
  Double_t& zMax = fZPositionMax[iStation];
  assert(zThickness >= 0.);
  if (z - zThickness < zMin) zMin = z - zThickness;
  if (z + zThickness > zMax) zMax = z + zThickness;
  fZPositions[iStation]   = 0.5 * (zMin + zMax);
  fZThicknesses[iStation] = zMax - zMin;
}

void CbmMvdStationPar::AddHeight(Int_t iStation, Double_t value) { SetParameterMax(fHeights, iStation, value); }

void CbmMvdStationPar::AddWidth(Int_t iStation, Double_t value) { SetParameterMax(fWidths, iStation, value); }

void CbmMvdStationPar::AddXRes(Int_t iStation, Double_t value) { SetParameterMax(fXResolutions, iStation, value); }

void CbmMvdStationPar::AddYRes(Int_t iStation, Double_t value) { SetParameterMax(fYResolutions, iStation, value); }

void CbmMvdStationPar::AddZRadThickness(Int_t iStation, Double_t value)
{
  SetParameterMax(fZRadThickness, iStation, value);
}

void CbmMvdStationPar::AddBeamHeight(Int_t iStation, Double_t value) { SetParameterMin(fBeamHeights, iStation, value); }

void CbmMvdStationPar::AddBeamWidth(Int_t iStation, Double_t value) { SetParameterMin(fBeamWidths, iStation, value); }
// -------------------------------------------------------------------------


ClassImp(CbmMvdStationPar)
