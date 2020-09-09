/** @file CbmStsSimSensorFactory.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.03.2020
 **/

#include "CbmStsSimSensorFactory.h"

#include "CbmStsDefs.h"
#include "CbmStsParSensor.h"
#include "CbmStsSimSensorDssdOrtho.h"
#include "CbmStsSimSensorDssdStereo.h"
#include <Logger.h>
#include <cassert>

using UP_sensor = CbmStsSimSensorFactory::UP_sensor;


// -----   Constructor   ---------------------------------------------------
CbmStsSimSensorFactory::CbmStsSimSensorFactory() {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsSimSensorFactory::~CbmStsSimSensorFactory() {}
// -------------------------------------------------------------------------


// -----   Create a simulation sensor   ------------------------------------
UP_sensor CbmStsSimSensorFactory::CreateSensor(const CbmStsParSensor& par) {

  switch (par.GetClass()) {

    // --- Sensor class DssdStereo. Number of strips and pitch must be
    // --- the same for both sides.
    case CbmStsSensorClass::kDssdStereo: {
      Double_t dY      = par.GetPar(3);     // Active size in y
      Int_t nStrips    = par.GetParInt(4);  // Number of strips front side
      Double_t pitch   = par.GetPar(6);     // Strip pitch front side
      Double_t stereoF = par.GetPar(8);     // Stereo angle front side
      Double_t stereoB = par.GetPar(9);     // Stereo angle back side
      assert(nStrips == par.GetParInt(5));  // same number of strips
      assert(TMath::Abs(pitch - par.GetPar(7)) < 0.0001);  // same pitch
      assert(Double_t(nStrips) * pitch < par.GetPar(0));
      assert(dY <= par.GetPar(1));  // Active size fits into geometry
      UP_sensor sensor(
        new CbmStsSimSensorDssdStereo(dY, nStrips, pitch, stereoF, stereoB));
      return sensor;
      break;
    }  //? DssdStereo

    // --- Sensor class DssdOrtho
    case CbmStsSensorClass::kDssdOrtho: {
      Int_t nStripsX  = par.GetParInt(4);  // Number of strips front side
      Int_t nStripsY  = par.GetParInt(5);  // Number of strips back side
      Double_t pitchX = par.GetPar(6);     // Strip pitch front side
      Double_t pitchY = par.GetPar(7);     // Strip pitch back side
      assert(Double_t(nStripsX) * pitchX < par.GetPar(0));
      assert(Double_t(nStripsY) * pitchY < par.GetPar(1));
      UP_sensor sensor(
        new CbmStsSimSensorDssdOrtho(nStripsX, pitchX, nStripsY, pitchY));
      return sensor;
      break;
    }  //? DssdOrtho

    // --- Unknown sensor class
    default: {
      LOG(fatal) << "StsSimSensorFactory: Unknown sensor class!";
      return nullptr;
      break;
    }  //? unknown sensor class

  }  //# switch sensor class

  return nullptr;
}
// -------------------------------------------------------------------------


ClassImp(CbmStsSimSensorFactory)
