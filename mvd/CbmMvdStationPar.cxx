/* Copyright (C) 2014-2020 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                     CbmMvdStationPar source file              -----
// -----                  Created 28/10/14  by P.Sitzmann              -----
// -------------------------------------------------------------------------

#include "CbmMvdStationPar.h"

#include <Logger.h>

#include <iostream>

#include <math.h>

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmMvdStationPar::CbmMvdStationPar()
  : TNamed()
  , fStationCount()
  , fZPositions()
  , fThicknesses()
  , fHeights()
  , fWidths()
  , fXResolutions()
  , fYResolutions()
  , fRadiationLength()
  , fBeamHeights()
  , fBeamWidths()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdStationPar::~CbmMvdStationPar() {}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void CbmMvdStationPar::Print(Option_t* /*opt*/) const
{

  LOG(info) << "MvdStationPar: Initialized parameter file with " << fZPositions.size() << " stations";

  LOG(debug) << "Z Postion station 0: " << GetZPosition(0);
  LOG(debug) << "Z Postion station 1: " << GetZPosition(1);
  LOG(debug) << "Z Postion station 2: " << GetZPosition(2);
  LOG(debug) << "Z Postion station 3: " << GetZPosition(3);

  LOG(debug) << "Thickness station 0: " << GetThickness(0);
  LOG(debug) << "Thickness station 1: " << GetThickness(1);
  LOG(debug) << "Thickness station 2: " << GetThickness(2);
  LOG(debug) << "Thickness station 3: " << GetThickness(3);

  LOG(debug) << "Width station 0: " << GetWidth(0);
  LOG(debug) << "Width station 1: " << GetWidth(1);
  LOG(debug) << "Width station 2: " << GetWidth(2);
  LOG(debug) << "Width station 3: " << GetWidth(3);

  LOG(debug) << "Height station 0: " << GetHeight(0);
  LOG(debug) << "Height station 1: " << GetHeight(1);
  LOG(debug) << "Height station 2: " << GetHeight(2);
  LOG(debug) << "Height station 3: " << GetHeight(3);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Bool_t CbmMvdStationPar::Init()
{
  for (Int_t i = 0; i < fStationCount; i++) {
    fZPositions[i]      = 0.;
    fThicknesses[i]     = 0.;
    fHeights[i]         = 0.;
    fWidths[i]          = 0.;
    fXResolutions[i]    = 0.;
    fYResolutions[i]    = 0.;
    fRadiationLength[i] = 0.;
    fBeamHeights[i]     = 0.;
    fBeamWidths[i]      = 0.;
  }
  return 1;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetZPosition(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fZPositions.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetThickness(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fThicknesses.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetHeight(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fHeights.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetWidth(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fWidths.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetXRes(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fXResolutions.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetYRes(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fYResolutions.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetRadLength(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fRadiationLength.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetBeamHeight(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fBeamHeights.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Double_t CbmMvdStationPar::GetBeamWidth(Int_t stationNumber) const
{
  if (stationNumber <= fStationCount) { return fBeamWidths.at(stationNumber); }
  else {
    cout << "Station number out of Range " << endl;
  }
  return 0.;
}

// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdStationPar::SetZPosition(Int_t stationNumber, Double_t z)
{
  if (fZPositions[stationNumber] != 0) {
    fZPositions[stationNumber] = Int_t(((fZPositions[stationNumber] + z) / 2) + 0.5);
  }
  else {
    fZPositions[stationNumber] = z;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetThickness(Int_t stationNumber, Double_t thickness)
{
  Float_t Sensorthickness = fabs(thickness - fZPositions[stationNumber]);
  if (Sensorthickness > fThicknesses[stationNumber]) fThicknesses[stationNumber] = Sensorthickness;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetHeight(Int_t stationNumber, Double_t height)
{
  if (fHeights[stationNumber] < height) { fHeights[stationNumber] = height; }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetWidth(Int_t stationNumber, Double_t width)
{
  if (fWidths[stationNumber] < width) { fWidths[stationNumber] = width; }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetXRes(Int_t stationNumber, Double_t xres) { fXResolutions[stationNumber] = xres; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetYRes(Int_t stationNumber, Double_t yres) { fYResolutions[stationNumber] = yres; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetRadLength(Int_t stationNumber, Double_t length) { fRadiationLength[stationNumber] = length; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetBeamHeight(Int_t stationNumber, Double_t beamheight)
{
  if (fBeamHeights[stationNumber] != 0) {
    if (fBeamHeights[stationNumber] > beamheight) { fBeamHeights[stationNumber] = beamheight; }
  }
  else {
    fBeamHeights[stationNumber] = beamheight;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdStationPar::SetBeamWidth(Int_t stationNumber, Double_t beamwidth)
{
  if (fBeamWidths[stationNumber] != 0) {
    if (fBeamWidths[stationNumber] > beamwidth) { fBeamWidths[stationNumber] = beamwidth; }
  }
  else {
    fBeamWidths[stationNumber] = beamwidth;
  }
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdStationPar)
