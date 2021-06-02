/** @file CbmStsSimSensorDssd.cxx
  * @copyright Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsSimSensorDssd.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#include "CbmStsSimSensorDssd.h"

#include "CbmStsDefs.h"
#include "CbmStsDigitize.h"
#include "CbmStsParSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmStsSensorPoint.h"
#include "CbmStsSetup.h"
#include "CbmStsSimModule.h"

#include <sstream>


using std::string;
using std::stringstream;
using namespace CbmSts;


// -----   Constructor   ---------------------------------------------------
CbmStsSimSensorDssd::CbmStsSimSensorDssd(CbmStsElement* element) : CbmStsSimSensor(element) {}
// -------------------------------------------------------------------------


// -----   Process one MC Point  -------------------------------------------
Int_t CbmStsSimSensorDssd::CalculateResponse(CbmStsSensorPoint* point)
{

  // --- Catch if parameters are not set
  assert(fIsSet);

  // --- Number of created charge signals (coded front/back side)
  Int_t nSignals = 0;

  // --- Reset the strip charge arrays
  fStripCharge[0].Reset();  // front side
  fStripCharge[1].Reset();  // back side

  // --- Produce charge and propagate it to the readout strips
  ProduceCharge(point);

  // --- Cross talk
  if (fSettings->CrossTalk()) {
    Double_t ctcoeff = GetConditions()->GetCrossTalkCoeff();
    CrossTalk(ctcoeff);
  }

  // --- Stop here if no module is connected (e.g. for test purposes)
  if (!GetModule()) return 0;

  // --- Register charges in strips to the module
  Int_t nCharges[2] = {0, 0};
  for (Int_t side = 0; side < 2; side++) {  // front and back side

    for (Int_t strip = 0; strip < GetNofStrips(side); strip++) {
      if (fStripCharge[side][strip] > 0.) {
        RegisterCharge(side, strip, fStripCharge[side][strip], point->GetTime());
        nCharges[side]++;
      }  //? charge in strip
    }    //# strips

  }  //# front and back side

  // Code number of signals
  nSignals = 1000 * nCharges[0] + nCharges[1];

  return nSignals;
}
// -------------------------------------------------------------------------


// -----   Charge status   -------------------------------------------------
string CbmStsSimSensorDssd::ChargeStatus() const
{
  stringstream ss;
  ss << GetName() << ": Charge status: \n";
  for (Int_t side = 0; side < 2; side++) {
    for (Int_t strip = 0; strip < GetNofStrips(side); strip++) {
      if (fStripCharge[side][strip] > 0.)
        ss << "          " << (side ? "Back  " : "Front ") << "strip " << strip << "  charge "
           << fStripCharge[side][strip] << "\n";
    }  //# strips
  }    //# front and back side
  ss << "          Total: front side " << (fStripCharge[0]).GetSum() << ", back side " << (fStripCharge[1]).GetSum();
  return ss.str();
}
// -------------------------------------------------------------------------


// -----   Cross talk calculation   ----------------------------------------
void CbmStsSimSensorDssd::CrossTalk(Double_t ctcoeff)
{

  for (Int_t side = 0; side < 2; side++) {  // front and back side

    // Number of strips for this side
    Int_t nStrips = GetNofStrips(side);

    // First strip
    Double_t qLeft        = 0.;
    Double_t qCurrent     = fStripCharge[side][0];
    fStripCharge[side][0] = (1. - ctcoeff) * qCurrent + ctcoeff * fStripCharge[side][1];

    // Strips 1 to n-2
    for (Int_t strip = 1; strip < nStrips - 1; strip++) {
      qLeft                     = qCurrent;
      qCurrent                  = fStripCharge[side][strip];
      fStripCharge[side][strip] = ctcoeff * (qLeft + fStripCharge[side][strip + 1]) + (1. - 2. * ctcoeff) * qCurrent;
    }  //# strips

    // Last strip
    qLeft                           = qCurrent;
    qCurrent                        = fStripCharge[side][nStrips - 1];
    fStripCharge[side][nStrips - 1] = ctcoeff * qLeft + (1. - ctcoeff) * qCurrent;

  }  //# front and back side
}
// -------------------------------------------------------------------------


// -----   Check whether a point is inside the active area   ---------------
Bool_t CbmStsSimSensorDssd::IsInside(Double_t x, Double_t y)
{
  if (x < -fDx / 2.) return kFALSE;
  if (x > fDx / 2.) return kFALSE;
  if (y < -fDy / 2.) return kFALSE;
  if (y > fDy / 2.) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------


// -----   Lorentz shift   -------------------------------------------------
Double_t CbmStsSimSensorDssd::LorentzShift(Double_t z, Int_t chargeType, Double_t bY) const
{

  assert(chargeType == 0 || chargeType == 1);

  // --- Drift distance to readout plane
  // Electrons drift to the front side (z = d/2), holes to the back side (z = -d/2)
  assert(chargeType == 0 || chargeType == 1);
  Double_t driftZ = 0.;
  if (chargeType == 0) driftZ = fDz / 2. - z;  // electrons
  else if (chargeType == 1)
    driftZ = fDz / 2. + z;  // holes
  else
    driftZ = 0.;

  // --- Hall mobility
  Double_t vBias     = GetConditions()->GetVbias();
  Double_t vFd       = GetConditions()->GetVfd();
  Double_t eField    = CbmStsPhysics::ElectricField(vBias, vFd, fDz, z + fDz / 2.);
  Double_t eFieldMax = CbmStsPhysics::ElectricField(vBias, vFd, fDz, fDz);
  Double_t eFieldMin = CbmStsPhysics::ElectricField(vBias, vFd, fDz, 0.);

  Double_t muHall;
  if (chargeType == 0)  // electrons
    muHall = GetConditions()->GetHallMobility((eField + eFieldMax) / 2., chargeType);
  else  // holes
    muHall = GetConditions()->GetHallMobility((eField + eFieldMin) / 2., chargeType);

  // --- The direction of the shift is the same for electrons and holes.
  // --- Holes drift in negative z direction, the field is in
  // --- positive y direction, thus the Lorentz force v x B acts in positive
  // --- x direction. Electrons drift in the opposite (positive z) direction,
  // --- but the have also the opposite charge sign, so the Lorentz force
  // --- on them is also in the positive x direction.
  Double_t shift = muHall * bY * driftZ * 1.e-4;
  // The factor 1.e-4 is because bZ is in T = Vs/m**2, but muHall is in
  // cm**2/(Vs) and z in cm.

  return shift;
}
// -------------------------------------------------------------------------


// -----   Produce charge and propagate it to the readout strips   ---------
void CbmStsSimSensorDssd::ProduceCharge(CbmStsSensorPoint* point)
{

  // Energy-loss model
  CbmStsELoss eLossModel = fSettings->ELossModel();

  // Total charge created in the sensor: is calculated from the energy loss
  Double_t chargeTotal = point->GetELoss() / CbmStsPhysics::PairCreationEnergy();  // in e

  // For ideal energy loss, just have all charge in the mid-point of the
  // trajectory
  if (eLossModel == CbmStsELoss::kIdeal) {
    Double_t xP = 0.5 * (point->GetX1() + point->GetX2());
    Double_t yP = 0.5 * (point->GetY1() + point->GetY2());
    Double_t zP = 0.5 * (point->GetZ1() + point->GetZ2());
    PropagateCharge(xP, yP, zP, chargeTotal, point->GetBy(), 0);  // front side (n)
    PropagateCharge(xP, yP, zP, chargeTotal, point->GetBy(), 1);  // back side (p)
    return;
  }

  // Kinetic energy
  Double_t mass = CbmStsPhysics::ParticleMass(point->GetPid());
  Double_t eKin = TMath::Sqrt(point->GetP() * point->GetP() + mass * mass) - mass;

  // Length of trajectory inside sensor and its projections
  Double_t trajLx     = point->GetX2() - point->GetX1();
  Double_t trajLy     = point->GetY2() - point->GetY1();
  Double_t trajLz     = point->GetZ2() - point->GetZ1();
  Double_t trajLength = TMath::Sqrt(trajLx * trajLx + trajLy * trajLy + trajLz * trajLz);

  // The trajectory is sub-divided into equidistant steps, with a step size
  // close to 3 micrometer.
  Double_t stepSizeTarget = 3.e-4;  // targeted step size is 3 micrometer
  Int_t nSteps            = TMath::Nint(trajLength / stepSizeTarget);
  if (nSteps == 0) nSteps = 1;  // assure at least one step
  Double_t stepSize  = trajLength / nSteps;
  Double_t stepSizeX = trajLx / nSteps;
  Double_t stepSizeY = trajLy / nSteps;
  Double_t stepSizeZ = trajLz / nSteps;

  // Average charge per step, used for uniform distribution
  Double_t chargePerStep = chargeTotal / nSteps;

  // Stopping power, needed for energy loss fluctuations
  Double_t dedx = 0.;

  if (eLossModel == CbmStsELoss::kUrban) dedx = CbmStsPhysics::Instance()->StoppingPower(eKin, point->GetPid());

  // Stepping over the trajectory
  Double_t chargeSum = 0.;
  Double_t xStep     = point->GetX1() - 0.5 * stepSizeX;
  Double_t yStep     = point->GetY1() - 0.5 * stepSizeY;
  Double_t zStep     = point->GetZ1() - 0.5 * stepSizeZ;
  for (Int_t iStep = 0; iStep < nSteps; iStep++) {
    xStep += stepSizeX;
    yStep += stepSizeY;
    zStep += stepSizeZ;

    // Charge for this step
    Double_t chargeInStep = chargePerStep;  // uniform energy loss
    if (eLossModel == CbmStsELoss::kUrban)  // energy loss fluctuations
      chargeInStep =
        CbmStsPhysics::Instance()->EnergyLoss(stepSize, mass, eKin, dedx) / CbmStsPhysics::PairCreationEnergy();
    chargeSum += chargeInStep;

    // Propagate charge to strips
    PropagateCharge(xStep, yStep, zStep, chargeInStep, point->GetBy(), 0);  // front
    PropagateCharge(xStep, yStep, zStep, chargeInStep, point->GetBy(), 1);  // back

  }  //# steps of the trajectory

  // For fluctuations: normalise to the total charge from GEANT.
  // Since the number of steps is finite (about 100), the average
  // charge per step does not coincide with the expectation value.
  // In order to be consistent with the transport, the charges are
  // re-normalised.
  if (eLossModel == CbmStsELoss::kUrban) {
    for (Int_t side = 0; side < 2; side++) {  // front and back side
      for (Int_t strip = 0; strip < GetNofStrips(side); strip++)
        fStripCharge[side][strip] *= (chargeTotal / chargeSum);
    }  //# front and back side
  }    //? E loss fluctuations
}
// -------------------------------------------------------------------------


// -----   Register charge to the module  ----------------------------------
void CbmStsSimSensorDssd::RegisterCharge(Int_t side, Int_t strip, Double_t charge, Double_t time) const
{

  // --- Check existence of module
  assert(GetModule());

  // --- Determine module channel for given sensor strip
  Int_t channel = GetModuleChannel(strip, side, GetSensorId());

  // --- Get the MC link information
  Int_t index = -1;
  Int_t entry = -1;
  Int_t file  = -1;
  if (GetCurrentLink()) {
    index = GetCurrentLink()->GetIndex();
    entry = GetCurrentLink()->GetEntry();
    file  = GetCurrentLink()->GetFile();
  }

  // --- Send signal to module
  GetModule()->AddSignal(channel, time, charge, index, entry, file);
}
// -------------------------------------------------------------------------


ClassImp(CbmStsSimSensorDssd)
