/* Copyright (C) 2011-2016 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                     CbmMvdSensorDataSheet source file                  -----
// -----                  Created 31/01/11  by M. Deveaux              -----
// -------------------------------------------------------------------------

#include "CbmMvdSensorDataSheet.h"

#include <Logger.h>

#include "TMath.h"

#include <ostream>  // for operator<<, stringstream, basic_ostream
using std::endl;

// -----   Public method Print   -------------------------------------------
void CbmMvdSensorDataSheet::Print(Option_t* /*opt*/) const { LOG(info) << ToString(); }

std::string CbmMvdSensorDataSheet::ToString() const
{
  std::stringstream ss;
  ss << " --- " << GetName() << "--------------------------------" << endl;
  ss << " Technical Data:" << endl;
  ss << "--------------------------" << endl;
  ss << "Name of the sensor: " << fMimosaName << endl;
  ss << "Pixel pitch (x/y) [µm]: " << fPixelPitchY * 1e4 << "/" << fPixelPitchY * 1e-4 << endl;
  ss << "N Pixels (rows/cols): " << fNPixelsX << "/" << fNPixelsY << endl;
  ss << "Thickness of active volume[µm]: " << fEpiTh * 1e4 << endl;
  ss << "Integration time [µs]: " << fIntegrationTime * 1e6 << endl;
  ss << "ADC resolution: " << fAdcBits << "bit ADC (1 = discriminator)" << endl;
  ss << "Default noise [e ENC]: " << fNoise << endl;
  ss << "Matrix size (x/y) [cm]: " << fPixelPitchX * fNPixelsX << " / " << fPixelPitchY * fNPixelsY << endl;
  ss << endl;
  ss << "Readout system: " << endl;
  ss << "----------------" << endl;
  ss << "PixelNmb X from left to right?: " << fPixelSignX << endl;
  ss << "PixelNmb Y from up to down?: " << fPixelSignY << endl;
  ss << "Rolling shutter from up to down?: " << fShutterSign << endl;
  ss << endl;
  ss << "ADC / SUZE -Settings: " << endl;
  ss << "----------------" << endl;
  ss << "ADC Range [e]: " << fAdcDynamic << endl;
  ss << "ADC offset [e]: " << fAdcOffset << endl;
  ss << "ADC stepsize [e]: " << fAdcStepSize << endl;
  ss << "SUZE states per banks: " << fStatesPerBank << endl;
  ss << "SUZE states per line : " << fStatesPerLine << endl;
  ss << "SUZE states per frame: " << fStatesPerFrame << endl;
  ss << "SUZE pixels per bank: " << fPixelsPerBank << endl;
  ss << endl;
  ss << "Data card properly initialized: " << fValidData << endl;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdSensorDataSheet)
