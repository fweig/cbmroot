/** @file CbmStsParSensor.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsParSensor.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#include "CbmStsParSensor.h"

#include <TMath.h>  // for Nint

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

ClassImp(CbmStsParSensor)

  // -----   Constructor   ---------------------------------------------------
  CbmStsParSensor::CbmStsParSensor(CbmStsSensorClass sClass)
  : fClass(sClass)
{
}
// -------------------------------------------------------------------------


// -----   Get a parameter   -----------------------------------------------
Float_t CbmStsParSensor::GetPar(UInt_t index) const
{
  Float_t value = 0.;
  if (index < fPar.size()) value = fPar[index];
  return value;
}
// -------------------------------------------------------------------------


// -----   Get the integer value of a parameter   --------------------------
Int_t CbmStsParSensor::GetParInt(UInt_t index) const { return TMath::Nint(GetPar(index)); }
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
std::string CbmStsParSensor::ToString() const
{
  std::stringstream ss;

  switch (fClass) {
    case CbmStsSensorClass::kDssdStereo: ss << "Class DssdStereo | "; break;
    case CbmStsSensorClass::kDssdOrtho: ss << "Class DssdStereo | "; break;
    default: ss << "Class unknown"; break;
  }  //? sensor class

  ss << "size (" << fPar[0] << " x " << fPar[1] << " x " << fPar[2] << ") | active " << fPar[3] << " | strips "
     << fPar[4] << " / " << fPar[5] << " | pitch " << fPar[6] << " / " << fPar[7] << " | stereo " << fPar[8] << " / "
     << fPar[9];

  return ss.str();
}
// -------------------------------------------------------------------------
