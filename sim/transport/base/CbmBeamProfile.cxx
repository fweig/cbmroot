/** @file CbmBeamProfile.cxx
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmBeamProfile.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 **/

#include "CbmBeamProfile.h"

#include "TRandom.h"

#include <sstream>


// -----   Default constructor   --------------------------------------------
CbmBeamProfile::CbmBeamProfile()
  : fFocalZ(0.)
  , fMeanPosX(0.)
  , fMeanPosY(0.)
  , fSigmaPosX(-1.)
  , fSigmaPosY(-1.)
  , fMeanThetaX(0.)
  , fMeanThetaY(0.)
  , fSigmaThetaX(-1.)
  , fSigmaThetaY(-1.)
{
}
// --------------------------------------------------------------------------


// -----   Check whether average beam hits the target   ---------------------
Bool_t CbmBeamProfile::CheckWithTarget(const CbmTarget& target) const
{

  // --- Check upstream target surface
  TVector3 surf1 = target.GetSurfaceCentreUp();
  TVector3 sect1 = ExtrapolateToPlane(surf1, target.GetNormal());
  Double_t dist1 = (sect1 - surf1).Mag();
  if (dist1 > 0.5 * target.GetDiameter()) {
    LOG(ERROR) << "EventGen: Average beam does not hit first target surface!";
    LOG(ERROR) << "          Surface centre is (" << surf1.X() << ", " << surf1.Y() << ", " << surf1.Z() << ") cm";
    LOG(ERROR) << "          Intersection point is (" << sect1.X() << ", " << sect1.Y() << ", " << sect1.Z() << ") cm";
    LOG(ERROR) << "          Distance to target surface centre is " << dist1 << " cm, target radius is "
               << 0.5 * target.GetDiameter() << " cm";
    return kFALSE;
  }

  // --- Check downstream target surface
  TVector3 surf2 = target.GetSurfaceCentreDown();
  TVector3 sect2 = ExtrapolateToPlane(surf2, target.GetNormal());
  Double_t dist2 = (sect2 - surf2).Mag();
  if (dist2 > 0.5 * target.GetDiameter()) {
    LOG(ERROR) << "EventGen: Average beam does not hit second target surface!";
    LOG(ERROR) << "          Surface centre is (" << surf2.X() << ", " << surf2.Y() << ", " << surf2.Z() << ") cm";
    LOG(ERROR) << "          Intersection point is (" << sect2.X() << ", " << sect2.Y() << ", " << sect2.Z() << ") cm";
    LOG(ERROR) << "          Distance to target surface centre is " << dist2 << " cm, target radius is "
               << 0.5 * target.GetDiameter() << " cm";
    return kFALSE;
  }

  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Extrapolate the average beam to a plane   ------------------------
TVector3 CbmBeamProfile::ExtrapolateToPlane(const TVector3& point, const TVector3& norm) const
{

  // Average beam trajectory
  CbmBeam beam(fMeanPosX, fMeanPosY, fFocalZ, fMeanThetaX, fMeanThetaY);

  // Intersection of average trajectory with plane
  return beam.ExtrapolateToPlane(point, norm);
}
// --------------------------------------------------------------------------


// -----   Generate a beam trajectory   -------------------------------------
std::unique_ptr<CbmBeam> CbmBeamProfile::GenerateBeam()
{

  // Beam x position
  Double_t x = fMeanPosX;
  if (fSigmaPosX > 0.) x = gRandom->Gaus(fMeanPosX, fSigmaPosX);

  // Beam y position
  Double_t y = fMeanPosY;
  if (fSigmaPosY > 0.) y = gRandom->Gaus(fMeanPosY, fSigmaPosY);

  // Beam angle in x-z plane
  Double_t tx = fMeanThetaX;
  if (fSigmaThetaX > 0.) tx = gRandom->Gaus(fMeanThetaX, fSigmaThetaX);

  // Beam angle in y-z plane
  Double_t ty = fMeanThetaY;
  if (fSigmaThetaY > 0.) ty = gRandom->Gaus(fMeanThetaY, fSigmaThetaY);

  std::unique_ptr<CbmBeam> beam(new CbmBeam(x, y, fFocalZ, tx, ty));

  //return std::move(beam);
  return beam;
}
// --------------------------------------------------------------------------


// -----   Set the parameters for the beam angle   --------------------------
void CbmBeamProfile::SetAngle(Double_t x0, Double_t y0, Double_t sigmaX, Double_t sigmaY)
{
  fMeanThetaX  = x0;
  fMeanThetaY  = y0;
  fSigmaThetaX = sigmaX;
  fSigmaThetaY = sigmaY;
}
// --------------------------------------------------------------------------


// -----   Set the parameters for the beam position   -----------------------
void CbmBeamProfile::SetPosition(Double_t x0, Double_t y0, Double_t sigmaX, Double_t sigmaY, Double_t zF)
{
  fMeanPosX  = x0;
  fMeanPosY  = y0;
  fSigmaPosX = sigmaX;
  fSigmaPosY = sigmaY;
  fFocalZ    = zF;
}
// --------------------------------------------------------------------------


// -----   Info to string   -------------------------------------------------
std::string CbmBeamProfile::ToString() const
{

  std::stringstream ss;
  ss << "Beam profile:";
  ss << "\n\t x position ";
  if (fSigmaPosX > 0.) ss << "mean " << fMeanPosX << " cm, sigma " << fSigmaPosX << " cm";
  else
    ss << fMeanPosX << " cm (fixed)";
  ss << "\n\t y position ";
  if (fSigmaPosY > 0.) ss << "mean " << fMeanPosY << " cm, sigma " << fSigmaPosY << " cm";
  else
    ss << fMeanPosY << " cm (fixed)";
  ss << "\n\t Focal plane: z =  " << fFocalZ << " cm";
  ss << "\n\t x-z angle ";
  if (fSigmaThetaX > 0.) ss << "mean " << fMeanThetaX << " rad, sigma " << fSigmaThetaX << " rad";
  else
    ss << fMeanThetaX << " rad (fixed)";
  ss << "\n\t y-z angle ";
  if (fSigmaThetaY > 0.) ss << "mean " << fMeanThetaY << " rad, sigma " << fSigmaThetaY << " rad";
  else
    ss << fMeanThetaY << " rad (fixed)";

  return ss.str();
}
// --------------------------------------------------------------------------
