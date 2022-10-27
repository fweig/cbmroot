/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsSimSensor.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/


#include "CbmStsSimSensor.h"

#include "CbmStsAddress.h"
#include "CbmStsElement.cxx"
#include "CbmStsPoint.h"
#include "CbmStsSensorPoint.h"

#include <FairField.h>
#include <FairRun.h>

#include <TGeoBBox.h>

using std::vector;

ClassImp(CbmStsSimSensor)


  // -----   Constructor   ---------------------------------------------------
  CbmStsSimSensor::CbmStsSimSensor(CbmStsElement* element)
  : fElement(element)
{
}
// -------------------------------------------------------------------------


// -----   Get the unique address from the sensor name (static)   ----------
UInt_t CbmStsSimSensor::GetAddressFromName(TString name)
{

  Int_t unit    = 10 * (name[5] - '0') + name[6] - '0' - 1;
  Int_t ladder  = 10 * (name[9] - '0') + name[10] - '0' - 1;
  Int_t hLadder = (name[11] == 'U' ? 0 : 1);
  Int_t module  = 10 * (name[14] - '0') + name[15] - '0' - 1;
  Int_t sensor  = 10 * (name[18] - '0') + name[19] - '0' - 1;

  return CbmStsAddress::GetAddress(unit, ladder, hLadder, module, sensor);
}
// -------------------------------------------------------------------------


// -----   Get the sensor Id within the module   ---------------------------
Int_t CbmStsSimSensor::GetSensorId() const
{
  assert(fElement);
  return CbmStsAddress::GetElementId(fElement->GetAddress(), kStsSensor);
}
// -------------------------------------------------------------------------


// -----   Process a CbmStsPoint  ------------------------------------------
Int_t CbmStsSimSensor::ProcessPoint(const CbmStsPoint* point, Double_t eventTime, const CbmLink& link)
{

  // --- Physical node
  assert(fElement);
  TGeoPhysicalNode* node = fElement->GetPnode();

  // --- Set current link
  fCurrentLink = link;

  // --- Transform start coordinates into local C.S.
  Double_t global[3];
  Double_t local[3];
  global[0] = point->GetXIn();
  global[1] = point->GetYIn();
  global[2] = point->GetZIn();
  node->GetMatrix()->MasterToLocal(global, local);
  Double_t x1 = local[0];
  Double_t y1 = local[1];
  Double_t z1 = local[2];

  // --- Transform stop coordinates into local C.S.
  global[0] = point->GetXOut();
  global[1] = point->GetYOut();
  global[2] = point->GetZOut();
  node->GetMatrix()->MasterToLocal(global, local);
  Double_t x2 = local[0];
  Double_t y2 = local[1];
  Double_t z2 = local[2];

  // --- Average track direction in local c.s.
  Double_t tXav = 0.;
  Double_t tYav = 0.;
  //  Int_t    tZav = 0;
  if (z2 - z1 != 0.) {
    tXav = (x2 - x1) / (z2 - z1);
    tYav = (y2 - y1) / (z2 - z1);
    //  	tZav = 1;
  }

  // --- Normally, the entry and exit coordinates are slightly outside of
  // --- the active node, which is a feature of the transport engine.
  // --- We correct here for this, in case a track was entering or
  // --- exiting the sensor (not for tracks newly created or stopped
  // --- in the sensor volume).
  // --- We here consider only the case of tracks leaving through the front
  // --- or back plane. The rare case of tracks leaving through the sensor
  // --- sides is caught by the digitisation procedure.
  Double_t dZ = dynamic_cast<TGeoBBox*>(node->GetShape())->GetDZ();

  // --- Correct start coordinates in case of entry step
  if (point->IsEntry()) {

    // Get track direction in local c.s.
    global[0] = point->GetPx();
    global[1] = point->GetPy();
    global[2] = point->GetPz();
    Double_t* rot;
    rot = node->GetMatrix()->GetRotationMatrix();
    TGeoHMatrix rotMat;
    rotMat.SetRotation(rot);
    rotMat.MasterToLocal(global, local);
    if (local[2] != 0.) {
      ;                                   // should always be; else no correction
      Double_t tX = local[0] / local[2];  // px/pz
      Double_t tY = local[1] / local[2];  // py/pz

      // New start coordinates
      Double_t xNew = 0.;
      Double_t yNew = 0.;
      Double_t zNew = 0.;
      if (z1 > 0.) zNew = dZ - 1.e-4;  // front plane, safety margin 1 mum
      else
        zNew = 1.e-4 - dZ;  // back plane, safety margin 1 mum
      xNew = x1 + tX * (zNew - z1);
      yNew = y1 + tY * (zNew - z1);

      x1 = xNew;
      y1 = yNew;
      z1 = zNew;
    }  //? pz != 0.

  }  //? track has entered

  // --- Correct stop coordinates in case of being outside the sensor
  if (TMath::Abs(z2) > dZ) {

    // Get track direction in local c.s.
    global[0] = point->GetPxOut();
    global[1] = point->GetPyOut();
    global[2] = point->GetPzOut();
    Double_t* rot;
    rot = node->GetMatrix()->GetRotationMatrix();
    TGeoHMatrix rotMat;
    rotMat.SetRotation(rot);
    rotMat.MasterToLocal(global, local);
    Double_t tX = 0.;
    Double_t tY = 0.;
    // Use momentum components for track direction, if available
    if (local[2] != 0.) {
      tX = local[0] / local[2];  // px/pz
      tY = local[1] / local[2];  // py/pz
    }
    // Sometimes, a track is stopped outside the sensor volume.
    // Then we take the average track direction as best approximation.
    // Note that there may be cases where entry and exit coordinates are
    // the same. In this case, tXav = tYav = 0; there will be no correction
    // of the coordinates.
    else {
      tX = tXav;  // (x2-x1)/(z2-z1) or 0 if z2 = z1
      tY = tYav;  // (y2-y1)/(z2-z1) or 0 if z2 = z1
    }

    // New coordinates
    Double_t xNew = 0.;
    Double_t yNew = 0.;
    Double_t zNew = 0.;
    if (z2 > 0.) zNew = dZ - 1.e-4;  // front plane, safety margin 1 mum
    else
      zNew = 1.e-4 - dZ;  // back plane, safety margin 1 mum
    xNew = x2 + tX * (zNew - z2);
    yNew = y2 + tY * (zNew - z2);

    x2 = xNew;
    y2 = yNew;
    z2 = zNew;

  }  //? track step outside sensor


  // --- Momentum magnitude
  Double_t px = 0.5 * (point->GetPx() + point->GetPxOut());
  Double_t py = 0.5 * (point->GetPy() + point->GetPyOut());
  Double_t pz = 0.5 * (point->GetPz() + point->GetPzOut());
  Double_t p  = TMath::Sqrt(px * px + py * py + pz * pz);

  // --- Get magnetic field
  global[0]          = 0.5 * (point->GetXIn() + point->GetXOut());
  global[1]          = 0.5 * (point->GetYIn() + point->GetYOut());
  global[2]          = 0.5 * (point->GetZIn() + point->GetZOut());
  Double_t bField[3] = {0., 0., 0.};
  if (FairRun::Instance()->GetField()) FairRun::Instance()->GetField()->Field(global, bField);

  // --- Absolute time of StsPoint
  Double_t pTime = eventTime + point->GetTime();

  // --- Create SensorPoint
  // Note: there is a conversion from kG to T in the field values.
  CbmStsSensorPoint* sPoint = new CbmStsSensorPoint(x1, y1, z1, x2, y2, z2, p, point->GetEnergyLoss(), pTime,
                                                    bField[0] / 10., bField[1] / 10., bField[2] / 10., point->GetPid());

  // --- Calculate the detector response
  Int_t result = CalculateResponse(sPoint);
  delete sPoint;

  return result;
}
// -------------------------------------------------------------------------
