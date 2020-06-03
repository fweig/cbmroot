/** @file CbmStsSimSensorDssdStereo.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#include "CbmStsSimSensorDssdStereo.h"

#include <TGeoBBox.h>
#include <TMath.h>
#include <TGeoPhysicalNode.h>

#include <FairLogger.h>

#include "CbmStsElement.h"
#include "CbmStsParSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmStsPhysics.h"


using std::stringstream;


// -----   Constructor   ---------------------------------------------------
CbmStsSimSensorDssdStereo::CbmStsSimSensorDssdStereo(CbmStsElement* element) :
             CbmStsSimSensorDssd(element)
{
}
// -------------------------------------------------------------------------



// -----   Constructor   ---------------------------------------------------
CbmStsSimSensorDssdStereo::CbmStsSimSensorDssdStereo(Double_t dy,
                                                     Int_t nStrips,
                                                     Double_t pitch,
                                                     Double_t stereoF,
                                                     Double_t stereoB,
                                                     CbmStsElement* element) :
    CbmStsSimSensorDssd(element),
    fNofStrips(nStrips),
    fPitch(pitch),
    fStereoF(stereoF),
    fStereoB(stereoB),
    fTanStereo(),
    fCosStereo(),
    fStripShift(),
    fErrorFac(0.)
{
  fDy = dy;
}
// -------------------------------------------------------------------------



// -----   Diffusion   -----------------------------------------------------
void CbmStsSimSensorDssdStereo::Diffusion(Double_t x, Double_t y,
                                       Double_t sigma, Int_t side,
                                       Double_t& fracL, Double_t& fracC,
                                       Double_t& fracR) {

  // Check side qualifier
  assert( side == 0 || side == 1);

  // x coordinate at the readout edge (y = fDy/2 )
  // This x is counted from the left edge.
  Double_t xRo = x + fDx / 2. - ( fDy / 2. - y ) * fTanStereo[side];

  // Centre strip number (w/o cross connection; may be negative or large than
  // the number of strips)
  Int_t iStrip = TMath::FloorNint(xRo / fPitch);

  // Strip boundaries at the readout edge (y = fDy/2)
  Double_t xLeftRo  = Double_t(iStrip) * fPitch;
  Double_t xRightRo = xLeftRo + fPitch;

  // Distance from strip boundaries across the strip
  Double_t dLeft  = ( xRo - xLeftRo )  * fCosStereo[side];
  Double_t dRight = ( xRightRo - xRo ) * fCosStereo[side];

  // Charge fractions
  // The value 0.707107 is 1/sqrt(2)
  fracL = 0.;
  if ( dLeft < 3. * sigma )
    fracL = 0.5 * ( 1. - TMath::Erf( 0.707107 * dLeft  / sigma) );
  fracR = 0.;
  if ( dRight < 3. * sigma )
    fracR = 0.5 * ( 1. - TMath::Erf( 0.707107 * dRight / sigma) );
  fracC = 1. - fracL - fracR;

}
// -------------------------------------------------------------------------



// -----   Get channel number in module   ----------------------------------
Int_t CbmStsSimSensorDssdStereo::GetModuleChannel(Int_t strip, Int_t side,
                                               Int_t sensorId) const {

  // --- Check side argument
  assert( side == 0 || side == 1);

  // --- Account for offset due to stereo angle
  Int_t channel = strip - sensorId * fStripShift[side];

  // --- Account for horizontal cross-connection of strips
  while ( channel < 0 )           channel += fNofStrips;
  while ( channel >= fNofStrips ) channel -= fNofStrips;

  // --- Account for front or back side
  if ( side ) channel += fNofStrips;

  return channel;
}
// -------------------------------------------------------------------------



// -----   Get strip number from coordinates   -----------------------------
Int_t CbmStsSimSensorDssdStereo::GetStripNumber(Double_t x, Double_t y,
                                             Int_t side) const {

  // Cave: This implementation assumes that the centre of the sensor volume
  // is also the centre of the active area, i.e. that the inactive borders
  // (guard ring) are symmetric both and x and y (not necessarily the same
  // in x and y).

  // Check side
  assert( side == 0 || side == 1);

  // Check whether in active area (should have been caught before)
  assert ( TMath::Abs(x) <= fDx / 2. );
  assert ( TMath::Abs(y) <= fDy / 2. );

  // Calculate distance from lower left corner of the active area.
  // Note: the coordinates are given w.r.t. the centre of the volume.
  Double_t xdist = x + 0.5 * fDx;
  Double_t ydist = y + 0.5 * fDy;

  // Project coordinates to readout (top) edge
  Double_t xro = xdist - ( fDy - ydist ) * fTanStereo[side];

  // Calculate corresponding strip number
  Int_t iStrip = TMath::FloorNint( xro / fPitch );

  // Account for horizontal cross-connection of strips
  // not extending to the top edge
  while ( iStrip < 0 )                 iStrip += fNofStrips;
  while ( iStrip >= fNofStrips )       iStrip -= fNofStrips;

  return iStrip;
}
// -------------------------------------------------------------------------



// -----   Initialise   ----------------------------------------------------
Bool_t CbmStsSimSensorDssdStereo::Init() {

  // Check presence of node
  assert(fElement);
  TGeoPhysicalNode* node = fElement->GetPnode();
  assert(node);

  // Check whether parameters are assigned
  assert ( fNofStrips > 0 );

  // Geometric shape of the sensor volume
  TGeoBBox* shape = dynamic_cast<TGeoBBox*>(node->GetShape());
  assert(shape);

  // Active size in x coordinate
  fDx = Double_t(fNofStrips) * fPitch;
  assert ( fDx < 2. * shape->GetDX() );

  // Active size in y coordinate
  assert ( fDy < 2. * shape->GetDY() );

  // Active size in z coordinate
  fDz = 2. * shape->GetDZ();

  // Stereo angle front side must be between -85 and 85 degrees
  assert ( TMath::Abs(fStereoF) < 85. );

  // Stereo angle back side must be between -85 and 85 degrees
  assert ( TMath::Abs(fStereoB) < 85. );

  // Derived variables
  fTanStereo[0]  = TMath::Tan( fStereoF * TMath::DegToRad() );
  fCosStereo[0]  = TMath::Cos( fStereoF * TMath::DegToRad() );
  fStripShift[0] = TMath::Nint(fDy * fTanStereo[0] / fPitch);
  fTanStereo[1]  = TMath::Tan( fStereoB * TMath::DegToRad() );
  fCosStereo[1]  = TMath::Cos( fStereoB * TMath::DegToRad() );
  fStripShift[1] = TMath::Nint(fDy * fTanStereo[1] / fPitch);

  // Set size of charge arrays
  fStripCharge[0].Set(fNofStrips);
  fStripCharge[1].Set(fNofStrips);

  // Factor for the hit position error
  fErrorFac = 1. / ( fTanStereo[1] - fTanStereo[0] )
                     / ( fTanStereo[1] - fTanStereo[0] );

  // --- Flag parameters to be set if test is OK
  fIsSet = kTRUE;

  return fIsSet;
}
// -------------------------------------------------------------------------



// -----   Modify the strip pitch   ----------------------------------------
void CbmStsSimSensorDssdStereo::ModifyStripPitch(Double_t pitch) {

  assert(fIsSet);  // Parameters should have been set before

  // Set new pitch and re-calculate number of strips
  fPitch = pitch;
  fNofStrips = Int_t( fDx / pitch );
  fDx = Double_t(fNofStrips) * pitch;

  // Set size of charge arrays
  fStripCharge[0].Set(fNofStrips);
  fStripCharge[1].Set(fNofStrips);

}
// -------------------------------------------------------------------------



// -----   Propagate charge to the readout strips   ------------------------
void CbmStsSimSensorDssdStereo::PropagateCharge(Double_t x, Double_t y,
                                             Double_t z, Double_t charge,
                                             Double_t bY, Int_t side) {

  // Check side qualifier
  assert( side == 0 || side == 1);

  Double_t xCharge = x;
  Double_t yCharge = y;

  // Lorentz shift on the drift to the readout plane
  if ( fSettings->LorentzShift() ) {
    xCharge += LorentzShift(z, side, bY);
  }

  // Stop if the charge after Lorentz shift is not in the active area.
  // Diffusion into the active area is not treated.
  if ( ! IsInside(xCharge, yCharge) ) {
    return;
  }

  // No diffusion: all charge is in one strip
  if ( ! fSettings->Diffusion() ) {
    Int_t iStrip = GetStripNumber(xCharge, yCharge, side);
    fStripCharge[side][iStrip] += charge;
  } //? Do not use diffusion

  // Diffusion: charge is distributed over centre strip and neighbours
  else {
    // Calculate diffusion width
    Double_t diffusionWidth =
        CbmStsPhysics::DiffusionWidth(z + fDz / 2.,  // distance from back side
                                      fDz,
                                      GetConditions()->GetVbias(),
                                      GetConditions()->GetVfd(),
                                      GetConditions()->GetTemperature(),
                                      side);
    assert (diffusionWidth >= 0.);
    // Calculate charge fractions in strips
    Double_t fracL = 0.;  // fraction of charge in left neighbour
    Double_t fracC = 1.;  // fraction of charge in centre strip
    Double_t fracR = 0.;  // fraction of charge in right neighbour
    Diffusion(xCharge, yCharge, diffusionWidth, side, fracL, fracC, fracR);
    // Calculate strip numbers
    // Note: In this implementation, charge can diffuse out of the sensitive
    // area only for vertical strips. In case of stereo angle (cross-connection
    // of strips), all charge is assigned to some strip, so the edge effects
    // are not treated optimally.
    Int_t iStripC  = GetStripNumber(xCharge, yCharge, side);  // centre strip
    Int_t iStripL  = 0;                                     // left neighbour
    Int_t iStripR  = 0;                                    // right neighbour
    if ( fTanStereo[side] < 0.0001 )  {   // vertical strips, no cross connection
      iStripL = iStripC - 1;  // might be = -1
      iStripR = iStripC + 1;  // might be = nOfStrips
    }
    else {   // stereo angle, cross connection
      iStripL = ( iStripC == 0 ? fNofStrips - 1 : iStripC - 1);
      iStripR = ( iStripC == fNofStrips - 1 ? 0 : iStripC + 1);
    }
    // Collect charge on the readout strips
    if ( fracC > 0. ) {
      fStripCharge[side][iStripC] += charge * fracC;    // centre strip
    }
    if ( fracL > 0. && iStripL >= 0 ) {
      fStripCharge[side][iStripL] += charge * fracL;  // right neighbour
    }
    if ( fracR > 0. && iStripR < fNofStrips ) {
      fStripCharge[side][iStripR] += charge * fracR;  // left neighbour
    }
  } //? Use diffusion

}
// -------------------------------------------------------------------------



// -----   String output   -------------------------------------------------
std::string CbmStsSimSensorDssdStereo::ToString() const {
  stringstream ss;
  assert(fElement);
  ss << fElement->GetName() << " (DssdStereo): ";
  TGeoPhysicalNode* node = fElement->GetPnode();
  if ( ! node ) ss << "no node assigned; ";
  else {
    TGeoBBox* shape = dynamic_cast<TGeoBBox*>(node->GetShape());
    assert(shape);
    ss << "Dimension (" << 2. * shape->GetDX() << ", "
        << 2. * shape->GetDY() << ", " << 2. * shape->GetDZ() << ") cm, ";
  }
  ss << "dy " << fDy << " cm, ";
  ss << "# strips " << fNofStrips << ", pitch " << fPitch << " cm, ";
  ss << "stereo " << fStereoF << "/" << fStereoB << " degrees";
  if ( fConditions) ss << "\n\t Conditions: " << fConditions->ToString();
  return ss.str();
}
// -------------------------------------------------------------------------



ClassImp(CbmStsSimSensorDssdStereo)
